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
/*----------------------------------------------------------------------------*
 * $RCSfile: fm_cli.c,v $
 * $Revision: #2 $
 * $Date: 2012/05/23 $
 * $Author: hongjun.chu $
 * $CCRevision: /main/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 *
 * Description: 
 *         This file implements CLI command table for File Manager.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "cli/x_cli.h"
#include "file_mngr/fm_util.h"
#include "file_mngr/u_fm.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/u_fm_mfw.h"
#include "file_mngr/x_fm_mfw.h"
#include "file_mngr/fm_ufs.h"
#include "res_mngr/x_rm_dev_types.h"
#include "file_mngr/fm_blkdev.h"
#include "libc/stdio.h"
#include "os/inc/os.h"
#include "cli/_cli.h"
#include "libc/stdlib.h"
#include <sys/vfs.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#if !defined(ANDROID) && !defined(__UCLIBC__)
#include <aio.h>
#endif
#include <string.h>

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))

#ifdef CLI_LVL_ALL

#define _NTFS_TEST_ 
static INT32 _fm_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_mkfs(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_mount(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_umount(INT32 i4_argc, const CHAR** ps_argv);
/*
static INT32 _fm_cli_write(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_read(INT32 i4_argc, const CHAR** ps_argv);
*/
static INT32 _fm_cli_cp(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_rm(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_rmdir(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_diff(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_dir(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_mkdir(INT32 i4_argc, const CHAR** ps_argv);
#if 0
static INT32 _fm_cli_mbr(INT32 i4_argc, const CHAR** ps_argv);
#endif
static INT32 _fm_cli_async_read(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_test( INT32 i4_argc, const CHAR** ps_argv);
#endif

static INT32 _fm_cli_getfsinfo( INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_getdirinfo( INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_getblkdev( INT32 i4_argc, const CHAR** ps_argv);

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_seek_dir( INT32 i4_argc, const CHAR** ps_argv);

/* stress test */
static INT32 _fm_cli_read_file(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_getinfo(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_stress_test(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_test_nor(INT32 i4_argc, const CHAR** ps_argv);
INT32   _fm_cli_more( INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_attack_usb( INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_concurrent_test( INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_max_test( INT32 i4_argc, const CHAR** ps_argv);
/* complex test*/


static INT32 _fm_cli_hdd_mount(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_hdd_read_test(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_hdd_direct_read(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_hdd_direct_write(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_dump_xentries(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _fm_cli_odd_mount_udf(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_odd_read_test(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_odd_direct_read(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_hdd_async_read(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_time_debug(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_partition_dump(INT32 i4_argc, const CHAR** pps_argv);

static INT32 _fm_cli_odd_performance_test(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_recycle_buf(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_sync_fread(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_async_fread(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_sync_fwrite(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_async_fwrite(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_fgets(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_fputs(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_rename(INT32 i4_argc, const CHAR** ps_argv);


static INT32 _fm_cli_async_rw_ex(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_sync_rw_ex(INT32 i4_argc, const CHAR** pps_argv);
#if 1
static INT32 _fm_cli_sync_fcopy(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_async_fcopy(INT32 i4_argc, const CHAR** ps_argv);

static INT32 _fm_cli_abort_async_fcopy(INT32 i4_argc, const CHAR** ps_argv);
#endif

static INT32 _fm_cli_seek_read_test(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_get_volume_label(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_partition_remount(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_partition_triger(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_rw_performance_test(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_anls(INT32 i4_argc, const CHAR** ps_argv);
static INT32 mfw_cret(INT32 i4_argc, const CHAR** ps_argv);
static INT32 mfw_open(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_del(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_lseek(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_read(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_write(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_aread(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_awrite(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_rnwm(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_rmwm(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_armwm(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_rmwn(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_abort(INT32 i4_argc, const CHAR** pps_argv);
static INT32 mfw_getinfo(INT32 i4_argc, const CHAR** pps_argv);
static VOID mfw_rw_task1(VOID* pv_tag);
static VOID mfw_rw_task2(VOID* pv_tag);
static INT32 mfw_rw_stress_test(INT32 i4_argc, const CHAR** ps_argv);
static INT32 mfw_diff(INT32 i4_argc, const CHAR** ps_argv);
static INT32 _fm_cli_raw_dev(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_search(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _fm_cli_search_async(INT32 i4_argc, const CHAR** pps_argv);
static VOID _fm_cli_search_async_nfy(FM_SEARCH_RESULT_T *pt_result);
static INT32 _fm_cli_search_stop(INT32 i4_argc, const CHAR** pps_argv);




/* Unit Test for NTFS */
#ifdef _NTFS_TEST_

#define NTFS_TC_OK   0
#define NTFS_TC_ERR -1

#define NTFS_STRESS_TEST_TIMES1 10
#define NTFS_STRESS_TEST_TIMES2 "10"

/* Async RW tag for NTFS */
typedef struct _NTFS_TC_ASYNC_TAG_T
{
    UINT32 ui4_done_bytes;
    HANDLE_T h_rw_lock;
    UINT32 ui4_total_rcnt;
    UINT32 ui4_total_wcnt;
    BOOL   b_eof;
    
} NTFS_TC_ASYNC_TAG_T;

/* Functions Declaration */
/* Some functions below have an i4_cnt parameter, which indicates
   the function will run i4_cnt times, and that can be seen as a
   stress test. */
static INT32 ntfs_tc_fopen(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_dopen(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fclose(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_dclose(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fcreate(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_dcreate(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fdelete(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_ddelete(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_frename(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_drename(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fread_sync(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fread_async(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fwrite_sync(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fwrite_async(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_fseek(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_get_dir_info(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_read_dir(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_dseek(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_get_finfo(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_set_finfo(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_mount(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_umount(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_cycle_frename(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_cycle_drename(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_cre_del_file(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_cre_del_dir(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_compare_file(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_mt_all(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_mt1(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_mt2(INT32 i4_argc, const CHAR** ps_argv);
static INT32 ntfs_tc_mt3(INT32 i4_argc, const CHAR** ps_argv);
static VOID ntfs_task_1(VOID* pv_tag);
static VOID ntfs_task_2(VOID* pv_tag);
static VOID ntfs_task_3(VOID* pv_tag);
static VOID ntfs_task_4(VOID* pv_tag);
static VOID ntfs_task_5(VOID* pv_tag);
static VOID ntfs_task_6(VOID* pv_tag);
static VOID ntfs_task_7(VOID* pv_tag);


/* Async RW notify function */
static VOID ntfs_tc_async_nfy(
        HANDLE_T h_req,
        VOID* pv_tag,
        FM_ASYNC_COND_T e_async_cond,
        UINT32 ui4_data);
        
/* Get parent component, and the caller must free the returned string*/
CHAR* _ntfs_tc_get_prnt_comp(const CHAR *ps_s);

/* Init async tag for NTFS */
static INT32 _ntfs_tc_init_async_tag(NTFS_TC_ASYNC_TAG_T *pt_tag);

/* Find the first set bit in an int */
static int ntfs_tc_ffs(int x);

#endif /*_NTFS_TEST */
#endif /* CLI_LVL_ALL */

/* File Manager command table */
static CLI_EXEC_T at_fm_cmd_tbl[] = 
{
#ifdef CLI_LVL_ALL
    {CLI_GET_DBG_LVL_STR,   NULL,   _fm_cli_get_dbg_level,  NULL,   CLI_GET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _fm_cli_set_dbg_level,  NULL,   CLI_SET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {"mkfs",        NULL, _fm_cli_mkfs,     NULL, "FAT16 Format",                       CLI_SUPERVISOR},
    {"mount",       NULL, _fm_cli_mount,    NULL, "Mount a filesystem",                 CLI_SUPERVISOR},
    {"umount",      NULL, _fm_cli_umount,   NULL, "Unmount a filesystem",               CLI_SUPERVISOR},
    {"dir",         NULL, _fm_cli_dir,      NULL, "List files in a directory",          CLI_SUPERVISOR},
    {"mkdir",       NULL, _fm_cli_mkdir,    NULL, "Make directory",                     CLI_SUPERVISOR},
    {"diff",        NULL, _fm_cli_diff,     NULL, "Diff two files",                     CLI_SUPERVISOR},
    {"rm",          NULL, _fm_cli_rm,       NULL, "Remove file",                        CLI_SUPERVISOR},
    {"rmdir",       NULL, _fm_cli_rmdir,    NULL, "Remove empty directory",             CLI_SUPERVISOR},
    {"cp",          NULL, _fm_cli_cp,       NULL, "Copy file from memory card to nor flash", CLI_SUPERVISOR},
    {"async_rd",    NULL, _fm_cli_async_read,      NULL, "Test Async Read",             CLI_SUPERVISOR},
    {"read",        NULL, _fm_cli_read_file,NULL, "Read File Out",                      CLI_SUPERVISOR},
    {"test",        NULL, _fm_cli_test,     NULL, "Test Memory Lkg",                    CLI_SUPERVISOR},
    {"getinfo",     NULL, _fm_cli_getinfo,  NULL, "Get  info",                          CLI_SUPERVISOR},
    {"seekdir",     NULL, _fm_cli_seek_dir, NULL, "Test x_fm_seek_dir",                 CLI_SUPERVISOR},
    {"stresstest",  NULL, _fm_cli_stress_test,  NULL, "Stress Test Middleware",         CLI_SUPERVISOR},
    {"testnor",     NULL, _fm_cli_test_nor, NULL, "Test Nand",                          CLI_SUPERVISOR},
    {"more",        NULL, _fm_cli_more,     NULL, "look inside file in text mode",      CLI_SUPERVISOR},
    {"attackusb",   NULL, _fm_cli_attack_usb,   NULL, "Stress test usb device",         CLI_SUPERVISOR},
    {"concurrent_test", NULL, _fm_concurrent_test, NULL, "Concurrent test usb device",  CLI_SUPERVISOR},
    {"max_test",    NULL, _fm_cli_max_test,     NULL, "Concurrent test usb device",     CLI_SUPERVISOR},
    {"fread",       NULL, _fm_cli_sync_fread,   NULL, "fread  [path] [execution count]",CLI_SUPERVISOR},
    {"faread",      NULL, _fm_cli_async_fread,  NULL, "faread [path] [execution count]",CLI_SUPERVISOR},
    {"fwrite",      NULL, _fm_cli_sync_fwrite,  NULL, "fwrite read from [src file path], write to [dest file path]", CLI_SUPERVISOR},
    {"fawrite",     NULL, _fm_cli_async_fwrite, NULL, "fawrite read from [src file path], write to [dest file path]", CLI_SUPERVISOR},
    {"fgets", NULL, _fm_cli_fgets, NULL, "fgets [file path] [string length]", CLI_SUPERVISOR},
    {"fputs", NULL, _fm_cli_fputs, NULL, "fputs [file path] [string]", CLI_SUPERVISOR},
    {"rename", NULL, _fm_cli_rename, NULL, "rename [old file name] [new file name]", CLI_SUPERVISOR},
    {"arw",         NULL, _fm_cli_async_rw_ex,  NULL, "arw  [read:1, write:0] [file path] [offset] [count]", CLI_SUPERVISOR},
    {"rw",          NULL, _fm_cli_sync_rw_ex,   NULL, "rw  [read:1, write:0] [file path] [offset] [count]", CLI_SUPERVISOR},
    {"rawdev",      NULL, _fm_cli_raw_dev,      NULL, "rawdev  [dev path] [offset] [count] ",       CLI_SUPERVISOR},
    {"fcopy",       NULL, _fm_cli_sync_fcopy,   NULL, "fcopy  [src file path] [dest file path]",    CLI_SUPERVISOR},
    {"afcopy",      NULL, _fm_cli_async_fcopy,  NULL, "afcopy  [src file path] [dest file path]",   CLI_SUPERVISOR},
    {"abort_afcopy",NULL, _fm_cli_abort_async_fcopy, NULL, "abort_afcopy  [async copy req handle]", CLI_SUPERVISOR},

    {"seekread",    NULL, _fm_cli_seek_read_test,       NULL, "seekread  [path]",       CLI_SUPERVISOR},
    {"get_vol_lbl", NULL, _fm_cli_get_volume_label,     NULL, "get_vol_lbl  [path]",    CLI_SUPERVISOR},
    {"remount",     NULL, _fm_cli_partition_remount,    NULL, "Remount a partition",    CLI_SUPERVISOR},
    {"ptriger",     NULL, _fm_partition_triger,         NULL, "ptriger  [Partition]",   CLI_SUPERVISOR},
    {"search",      NULL, _fm_cli_search,               NULL, "search [Path] [Pattern]",CLI_SUPERVISOR},
    {"Asearch",     NULL, _fm_cli_search_async,         NULL, "Asearch [Path] [Pattern]",CLI_SUPERVISOR},
    {"stop",        NULL, _fm_cli_search_stop,          NULL, "stop async search",      CLI_SUPERVISOR},
    {"rwptest",     NULL, _fm_cli_rw_performance_test,  NULL, "rwp  [path] [RW count]", CLI_SUPERVISOR},
    /*{"mbr", NULL, _fm_cli_mbr, NULL, "Enter FM Shell", CLI_SUPERVISOR}, */
    {"dump_xentries",  NULL, _fm_cli_dump_xentries,    NULL, "Dump All Entries",        CLI_SUPERVISOR},
    {"hdd_mount",      NULL, _fm_cli_hdd_mount,        NULL, "Mount HDD",               CLI_SUPERVISOR},
    {"hdd_read_test",  NULL, _fm_cli_hdd_read_test,    NULL, "HDD Read Test",           CLI_SUPERVISOR},
    {"hdd_read_direct",NULL, _fm_cli_hdd_direct_read,  NULL, "HDD Direct Read Test",    CLI_SUPERVISOR},
    {"hdd_write_direct", NULL, _fm_cli_hdd_direct_write, NULL, "HDD Direct Write Test", CLI_SUPERVISOR},
    {"odd_mount_udf",  NULL, _fm_cli_odd_mount_udf,    NULL, "Mount ODD UDF",           CLI_SUPERVISOR},
    {"odd_read_test",  NULL, _fm_cli_odd_read_test,    NULL, "ODD Read Test",           CLI_SUPERVISOR},
    {"odd_read_direct",NULL, _fm_cli_odd_direct_read,  NULL, "ODD Direct Read Test",    CLI_SUPERVISOR},
    {"hdd_async_read", NULL, _fm_cli_hdd_async_read,   NULL, "ODD Direct Read Test",    CLI_SUPERVISOR},
    {"time_debug",     NULL, _fm_cli_time_debug,       NULL, "time debug",              CLI_SUPERVISOR},
    {"p_dump",         NULL, _fm_cli_partition_dump,   NULL, "dump physical LBA",       CLI_SUPERVISOR},
    {"odd_perform",    NULL, _fm_cli_odd_performance_test, NULL, "odd performance test",CLI_SUPERVISOR},
    {"recycle_buffer", NULL, _fm_cli_recycle_buf,      NULL, "buffer recycle test",     CLI_SUPERVISOR},     


    {"manls",  NULL, mfw_anls, NULL, "manls [mp path]",                                 CLI_SUPERVISOR},
    {"mcret",  NULL, mfw_cret, NULL, "mcret [head file path] [init size (n*1MB)]",      CLI_SUPERVISOR},     
    {"mopen",  NULL, mfw_open, NULL, "mopen [head file path]",                          CLI_SUPERVISOR},
    {"mclose", NULL, mfw_close,NULL, "mclose [file handle]",                            CLI_SUPERVISOR},     
    {"mdel",   NULL, mfw_del,  NULL, "mcret [head file path]",                          CLI_SUPERVISOR},     
    {"mseek",  NULL, mfw_lseek,NULL, "mseek [head file][offset][whence]",               CLI_SUPERVISOR},
    {"mread",  NULL, mfw_read, NULL, "mread [file handle] [count]",                     CLI_SUPERVISOR},     
    {"mwrite", NULL, mfw_write,NULL, "mwrite [head file] [count]",                      CLI_SUPERVISOR},     
    {"maread", NULL, mfw_aread,NULL, "maread [file handle] [count]",                    CLI_SUPERVISOR},     
    {"mawrite",NULL, mfw_awrite, NULL, "mawrite [head file] [count]",                   CLI_SUPERVISOR},     
    {"mrnwm",  NULL, mfw_rnwm, NULL, "mrnwm [normal path] [wrapper path]",              CLI_SUPERVISOR},     
    {"mrmwm",  NULL, mfw_rmwm, NULL, "mrmwm [wrapper path] [wrapper file path]",        CLI_SUPERVISOR},     
    {"marmwm", NULL, mfw_armwm,NULL, "marmwm [wrapper file path] [wrapper file path]",  CLI_SUPERVISOR},     
    {"mrmwn",  NULL, mfw_rmwn, NULL, "mrmwn [wrapper file path] [normal file path]",    CLI_SUPERVISOR},     
    {"mabort", NULL, mfw_abort,NULL, "mabort [request handle] ",                        CLI_SUPERVISOR},     
    {"minfo",  NULL, mfw_getinfo, NULL, "minfo [1:by name;0:by handle][path/handle] ",  CLI_SUPERVISOR},
    {"mdiff",  NULL, mfw_diff, NULL, "mdiff [path1] [path2] [n*1KB each time]",         CLI_SUPERVISOR},
    {"mstrs",  NULL, mfw_rw_stress_test, NULL, "MFW Stress Test for Read and Write",    CLI_SUPERVISOR},     

    #ifdef _NTFS_TEST_
    /* Command for NTFS unit test*/
    {"-----------------------", NULL, NULL, NULL, "--------------------------",                     CLI_SUPERVISOR},
    {"Command for NTFS test", NULL, NULL, NULL,   "Description & Usage",                            CLI_SUPERVISOR},
    {"ntfs_mount",   NULL, ntfs_tc_mount,   NULL, "Mount a NTFS device, [dev node] [mount point]",  CLI_SUPERVISOR},
    {"ntfs_umount",  NULL, ntfs_tc_umount,  NULL, "Umount a NTFS device, [mount point]",            CLI_SUPERVISOR},
    {"ntfs_fcreate", NULL, ntfs_tc_fcreate, NULL, "Create a file, [file path]",                     CLI_SUPERVISOR},
    {"ntfs_fopen",   NULL, ntfs_tc_fopen,   NULL, "Open a file, [file path] [execution count]",     CLI_SUPERVISOR},
    {"ntfs_fclose",  NULL, ntfs_tc_fclose,  NULL, "Close a file, test in ntfs_fopen",               CLI_SUPERVISOR},
    {"ntfs_fdelete", NULL, ntfs_tc_fdelete, NULL, "Delete a file, [file path]",                     CLI_SUPERVISOR},
    {"ntfs_frename", NULL, ntfs_tc_frename, NULL, "Rename a file, [src file path] [dest file path]",CLI_SUPERVISOR},
    {"ntfs_fseek",   NULL, ntfs_tc_fseek,   NULL, "Seek a file, test in read and write file",       CLI_SUPERVISOR},
    {"ntfs_dcreate", NULL, ntfs_tc_dcreate, NULL, "Create a directory, [dir path]",                 CLI_SUPERVISOR},
    {"ntfs_dopen",   NULL, ntfs_tc_dopen,   NULL, "Open a directory, [dir path] [execution count]", CLI_SUPERVISOR},
    {"ntfs_dclose",  NULL, ntfs_tc_dclose,  NULL, "Close a directory, test in ntfs_dopen",          CLI_SUPERVISOR},
    {"ntfs_ddelete", NULL, ntfs_tc_ddelete, NULL, "Delete a directory, [dir path]",                 CLI_SUPERVISOR},
    {"ntfs_drename", NULL, ntfs_tc_drename, NULL, "Rename a directory, [src dir path] [dest dir path]",   CLI_SUPERVISOR},
    {"ntfs_dseek",   NULL, ntfs_tc_dseek,   NULL, "Seek a directory, [dir path] [execution count]", CLI_SUPERVISOR},
    {"ntfs_get_dinfo",    NULL, ntfs_tc_get_dir_info, NULL, "Get a directory's info, [dir path] [execution count]",     CLI_SUPERVISOR},
    {"ntfs_dir",          NULL, ntfs_tc_read_dir,     NULL, "Read directory, [dir path] [execution count]",             CLI_SUPERVISOR},
    {"ntfs_get_finfo",    NULL, ntfs_tc_get_finfo,    NULL, "Get a file's info, [file path] [execution count]",         CLI_SUPERVISOR},
    {"ntfs_set_finfo",    NULL, ntfs_tc_set_finfo,    NULL, "Set a file's info, [file path] [execution count]",         CLI_SUPERVISOR},
    {"ntfs_sync_read",    NULL, ntfs_tc_fread_sync,   NULL, "Read file in sync mode, [file path] [execution count]",    CLI_SUPERVISOR},
    {"ntfs_async_read",   NULL, ntfs_tc_fread_async,  NULL, "Read file in async mode, [file path] [execution count]",   CLI_SUPERVISOR},
    {"ntfs_sync_write",   NULL, ntfs_tc_fwrite_sync,  NULL, "Write file in sync mode, read from [src file path], write to [dest file path]",  CLI_SUPERVISOR},
    {"ntfs_async_write",  NULL, ntfs_tc_fwrite_async, NULL, "Write file in async mode, read from [src file path], write to [dest file path]", CLI_SUPERVISOR},
    {"ntfs_cycle_frename",NULL, ntfs_tc_cycle_frename, NULL, "Cycle rename file, [Name1] [Name2] [Name3] [execution count]", CLI_SUPERVISOR},
    {"ntfs_cycle_drename",NULL, ntfs_tc_cycle_drename, NULL, "Cycle rename directory, [Name1] [Name2] [Name3] [execution count]", CLI_SUPERVISOR},
    {"ntfs_cre_del_file", NULL, ntfs_tc_cre_del_file, NULL, "Create file and then delete it, [file path] [execution count]", CLI_SUPERVISOR},
    {"ntfs_cre_del_dir",  NULL, ntfs_tc_cre_del_dir,  NULL, "Create dir and then delete it, [dir path] [execution count]",  CLI_SUPERVISOR},
    {"ntfs_cmp_file",     NULL, ntfs_tc_compare_file, NULL, "Compare two files, [src file path] [dest file path]",      CLI_SUPERVISOR},
    {"ntfs_mt1",    NULL, ntfs_tc_mt1, NULL, "Multi thread stress test1, delete opened file",   CLI_SUPERVISOR},
    {"ntfs_mt2",    NULL, ntfs_tc_mt2, NULL, "Multi thread stress test2",                       CLI_SUPERVISOR},
    {"ntfs_mt3",    NULL, ntfs_tc_mt3, NULL, "Multi thread stress test3, read and write",       CLI_SUPERVISOR},
    {"ntfs_mt_all", NULL, ntfs_tc_mt_all, NULL, "Multi thread stress test(mt1,mt2,mt3)",        CLI_SUPERVISOR},

    #endif /* _NTFS_TEST_ */
#endif /* CLI_LVL_ALL */
    {"getfsinfo",   NULL, _fm_cli_getfsinfo,    NULL, "Get file system info",       CLI_GUEST},
    {"getdirinfo",  NULL, _fm_cli_getdirinfo,   NULL, "Get directory info",         CLI_GUEST},
    {"getblkdev",   NULL, _fm_cli_getblkdev,    NULL, "Get physical data of blkdev",CLI_GUEST},

    END_OF_CLI_CMD_TBL
};

/* File Manager root command table */
static CLI_EXEC_T at_fm_root_cmd_tbl[] =
{
    {"fm", NULL, NULL, at_fm_cmd_tbl, "File Manager commands", CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

#endif /* #if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST)) */

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static UINT32 _StrToHex(const CHAR* pszStr, UINT32 u4Len)
{  
    UINT32 u4Idx;   
    UINT32 u4ReturnValue = 0;   
    if ((pszStr == NULL) || (u4Len == 0))   
    {       
        return 0;   
    }
    u4Len = (u4Len > (UINT32)8) ? ((UINT32)8) : u4Len;    
    for (u4Idx = 0; u4Idx < u4Len; u4Idx++)    
    {       
        if ((pszStr[u4Idx] >= '0') && (pszStr[u4Idx] <= '9'))       
        {           
            u4ReturnValue = u4ReturnValue << 4;         
            u4ReturnValue += (UINT32)(UINT8)(pszStr[u4Idx] - '0');      
        }       
        else if ((pszStr[u4Idx] >= 'A') && (pszStr[u4Idx] <= 'F'))
        {           
            u4ReturnValue = u4ReturnValue << 4;         
            u4ReturnValue += (UINT32)(UINT8)(pszStr[u4Idx] - 'A' ) + 10; 
        }       
        else if ((pszStr[u4Idx] >= 'a') && (pszStr[u4Idx] <= 'f')) 
        {   
            u4ReturnValue = u4ReturnValue << 4;         
            u4ReturnValue += (UINT32)(UINT8)(pszStr[u4Idx] - 'a') + 10;
        }       
        else   
        {
            return 0;    
        }   
     }   

    return u4ReturnValue;
}
#endif

#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))
static VOID StringToNumber(const CHAR* ps_string, INT32 *pi4_num)
{
   INT32 i4_len, i4_sign = 1;
   
   *pi4_num = 0;
   i4_len = x_strlen(ps_string);
   if ( *ps_string == '-')
   {
       i4_sign = -1;
       i4_len--;
       ps_string++;
   }
   while( i4_len--)
   {
       *pi4_num = ( (*pi4_num)*10 + (*ps_string - '0'));
       ps_string++;
   }
   *pi4_num = *pi4_num * i4_sign;
}
#endif

#ifdef CLI_LVL_ALL
typedef struct
{
    TIME_T  t_second;
    UINT16  ui2_ms;
} MyTime_T;

static VOID GetTimeDiff( MyTime_T *pt_diff, MyTime_T *pt_end, MyTime_T *pt_start)
{
    UINT64  ui8_time0, ui8_time1, ui8_time2;

    ui8_time0 = pt_end->t_second * 1000 + pt_end->ui2_ms;
    ui8_time1 = pt_start->t_second * 1000 + pt_start->ui2_ms;

    if ( ui8_time0 > ui8_time1)
    {
        ui8_time2 = ui8_time0 - ui8_time1;
    }
    else
    {
        ui8_time2 = ui8_time1 - ui8_time0;
    }

    pt_diff->t_second = ui8_time2 / 1000;
    pt_diff->ui2_ms = (UINT16)ui8_time2 % 1000;
}
static INT32 _fm_cli_read_file(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32    i4_ret;
    INT32    i4_loop, i4_print;
    UINT32  ui4_read_cnt;
    HANDLE_T    h_src_file = NULL_HANDLE;
    UINT8   *buf = NULL;
    UINT8   *mem = NULL;
    FM_FILE_INFO_T  t_src_file_info;
    UINT64  ui8_cur_pos;
    INT32 from, offset, cnt, w_cache;
    if ( i4_argc < 6)
    {
        x_dbg_stmt("Usage: read_file <file_name> <from> <offset> <cnt> <w/wo cache>\n");
        x_dbg_stmt("<from>: 0: begin 1: end\n");
        x_dbg_stmt("<offset>: if <from> == 1, please use negative offset\n");
        x_dbg_stmt("<w/wo cache>: 0: without cache, 1: with cache.\n");
        return CLIR_OK;
    }
    StringToNumber(ps_argv[2], &from);
    StringToNumber(ps_argv[3], &offset);
    StringToNumber(ps_argv[4], &cnt);
    StringToNumber(ps_argv[5], &w_cache);    
    x_dbg_stmt("read file<%s>, from: %d, offset: %d, cnt: %d, w/wo cache: %d\n", 
        ps_argv[1], from, offset, cnt, w_cache);
   /* Open file in I/O Direct mode */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_argv[1], FM_READ_ONLY, 0666,w_cache == 0 ? TRUE :  FALSE , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_argv[1], i4_ret);
         i4_ret =  CLIR_CMD_EXEC_ERROR;
         goto exit;
    }         
    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_argv[1], &t_src_file_info);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
         i4_ret =  CLIR_CMD_EXEC_ERROR;
         goto exit;
    }
    
    x_dbg_stmt("%s size is %d bytes\n",  ps_argv[1], t_src_file_info.ui8_size);
    x_dbg_stmt("%s blk size is %d \n",  ps_argv[1], t_src_file_info.ui4_blk_size);
    x_dbg_stmt("%s blk cnt is %d \n",  ps_argv[1], t_src_file_info.ui8_blk_cnt);

    if ( from== 0)
        i4_ret = x_fm_lseek(h_src_file, offset, FM_SEEK_BGN, &ui8_cur_pos);
    else
        i4_ret = x_fm_lseek(h_src_file, offset, FM_SEEK_END, &ui8_cur_pos);

    buf = (UINT8*)x_mem_alloc(32 * 1024 + 32);
    mem = (UINT8 *)((((UINT32)buf + 32) / 32) * 32);
    if (buf == 0)
    {
        i4_ret =  CLIR_CMD_EXEC_ERROR;
        goto exit;
    }

    i4_ret = x_fm_read(h_src_file, mem, (UINT32)cnt, &ui4_read_cnt);
    if ( i4_ret != 0)
    {
        x_dbg_stmt("FM Read Fail(%d)\n", i4_ret);
        i4_ret =  CLIR_CMD_EXEC_ERROR;
        goto exit;
    }
    x_dbg_stmt("ui4_read_cnt: %d\n", ui4_read_cnt);

    for(i4_loop = 0; i4_loop < 32; i4_loop++)
    {
        x_dbg_stmt("%08Xh :", (16 * i4_loop));

        for(i4_print = 0; i4_print < 16; i4_print++)
        {
            x_dbg_stmt(" %02X", mem[(16 * i4_loop) + i4_print]);    
        }

        x_dbg_stmt("\n");
    }  

    x_dbg_stmt("Read Success.\n");

exit:

    if (h_src_file != NULL_HANDLE)
    {
        x_fm_close( h_src_file);
    }

    if (buf != NULL)
    {
        x_mem_free(buf);
    }

    return CLIR_OK;
}

INT32 _fm_cli_more( INT32 i4_argc, const CHAR** ps_argv)
{
    FILE *fp = fopen(ps_argv[1], "r");
    CHAR    *pc_buf, *ps_string_buf;
    SIZE_T  z_read_cnt;

    x_dbg_stmt("Any key: next page, q: quit\n");
    ps_string_buf = (CHAR*)x_mem_alloc(sizeof(CHAR) * 1024);
    pc_buf = (CHAR*)x_mem_alloc(sizeof(CHAR) * 1024);
    do
    {
        z_read_cnt = fread( pc_buf, sizeof(CHAR), 1024, fp);


        cli_get_string( ps_string_buf, FALSE);
        if ( x_strcmp( (const CHAR*)ps_string_buf, "q"))
        {
            break;
        }
    } while( z_read_cnt);

    fclose(fp);
    return CLIR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _fm_cli_seek_dir
 *
 * Description: This API copy file from memory card to nor flash.
 *                      async_read file_name num_byte cnt_open cnt_read
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_seek_dir( INT32 i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_dir;
    INT32  i4_ent_off, i4_ret;
    FM_DIR_ENTRY_T *pt_dir_entry;
    UINT32  ui4_entries, ui4_idx;

    pt_dir_entry = (FM_DIR_ENTRY_T*)x_mem_alloc(sizeof(FM_DIR_ENTRY_T) * 1);
    StringToNumber(ps_argv[2], &i4_ent_off);
    i4_ret = x_fm_open_dir(FM_ROOT_HANDLE, ps_argv[1], &h_dir);

    x_dbg_stmt("entry offset: %d\n", i4_ent_off);
    i4_ret = x_fm_seek_dir( h_dir, FM_SEEK_BGN, i4_ent_off);
    if ( i4_ret  != FMR_OK)
    {
        
        x_dbg_stmt("seek dir failed: %d\n", i4_ret);
            i4_ret = CLIR_CMD_EXEC_ERROR;
            goto FAIL;
    }
    /*
    StringToNumber(ps_argv[3], &i4_ent_off);
    x_dbg_stmt("then offset: %d\n", i4_ent_off);
    i4_ret = x_fm_seek_dir( h_dir, FM_SEEK_CUR, i4_ent_off);
    if ( i4_ret  != FMR_OK)
    {
            i4_ret = CLIR_CMD_EXEC_ERROR;
            goto FAIL;
    }
    */
    i4_ret = x_fm_read_dir_entries( h_dir, pt_dir_entry, 1, &ui4_entries);
    if ( i4_ret  != FMR_OK)
    {
        x_dbg_stmt("read dir failed: %d\n", i4_ret);
            i4_ret = CLIR_CMD_EXEC_ERROR;
            goto FAIL;
    }

    for( ui4_idx = 0; ui4_idx < ui4_entries; ui4_idx++)
    {
        x_dbg_stmt("entry name: %s\n", pt_dir_entry[ui4_idx].s_name);
    }

    i4_ret = CLIR_OK;
FAIL:
    x_fm_close( h_dir);
    x_mem_free( pt_dir_entry);
    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: _fm_cli_getblkdev
 *
 * Description: This API copy file from memory card to nor flash.
 *                      async_read file_name num_byte cnt_open cnt_read
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))
 static INT32 _fm_cli_getblkdev( INT32 i4_argc, const CHAR** ps_argv)
 {
    UINT32 i;
    UINT8   *pui1_buf = NULL;
    UINT8   *pui1_temp_buf = NULL;
    INT32  ui4_offset, ui4_size;
    FM_LOOKUP_ARG_T t_lookup;
    FM_XENTRY_T *pt_dev_xent = NULL;
    FM_RW_ARG_T t_io;
    HANDLE_T    h_sync_sema = NULL_HANDLE;
    UINT32 ui4_align_bytes;
    INT32   i4_ret;
    if (i4_argc < 4)
    {
        x_dbg_stmt("Usage: getblkdev [device_name] [read_offset] [read_count]\n");
        return CLIR_CMD_EXEC_ERROR;
    } 

    StringToNumber(ps_argv[2], (INT32*)&ui4_offset);
    StringToNumber(ps_argv[3], (INT32*)&ui4_size);


    i4_ret = fm_ufs_lookup(NULL, ps_argv[1], FM_UFS_LOOKUP, FM_UFS_LOCK_LEAF, &t_lookup);
    if ( i4_ret != FMR_OK)
    {
        i4_ret = CLIR_CMD_EXEC_ERROR;
        goto FAIL;
    }
    pt_dev_xent = t_lookup.pt_xent;

    i4_ret = blkdev_open( pt_dev_xent);
    if ( i4_ret != FMR_OK)
    {
        i4_ret = CLIR_CMD_EXEC_ERROR;
        goto FAIL;
    }
    ui4_align_bytes = pt_dev_xent->pt_dev->t_blk.ui4_align_bytes;

    pui1_buf = (UINT8*)x_mem_alloc(ui4_size+ui4_align_bytes-1);
    if (pui1_buf == NULL)
    {
        i4_ret = CLIR_CMD_EXEC_ERROR;
        goto FAIL;
    }

    pui1_temp_buf = (UINT8*)(((UINT32)pui1_buf + ui4_align_bytes-1)/ui4_align_bytes*ui4_align_bytes);

    i4_ret = x_sema_create(&h_sync_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK );
    if ( i4_ret != FMR_OK)
    {
        i4_ret = CLIR_CMD_EXEC_ERROR;
        goto FAIL;
    }    
    t_io.pt_xent        = pt_dev_xent;
    t_io.pv_data        = pui1_temp_buf;
    t_io.ui4_count      = ui4_size;
    t_io.ui8_offset     = ui4_offset;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC | FM_IO_DIRECT;
    t_io.h_bin_sema     = h_sync_sema;
    
    i4_ret = blkdev_read(&t_io);
    if ( i4_ret != FMR_OK)
    {
        i4_ret = CLIR_CMD_EXEC_ERROR;
        goto FAIL;
    }
    for(i = 0; i < ui4_size; i++)
    {
        x_dbg_stmt("%02X\t",pui1_temp_buf[i]);
        if (i != 0 && (i+1) % 16 == 0)
        {
            x_dbg_stmt("\n");
        }
    }

FAIL:
    if (h_sync_sema != NULL_HANDLE)
    {
        x_sema_delete( h_sync_sema);
    }

    if (pt_dev_xent != NULL)
    {
        i4_ret = blkdev_close( pt_dev_xent);
    }

    if (pui1_buf != NULL)
    {
        x_mem_free( pui1_buf);
    }

    return CLIR_OK;
 }
#endif
/*-----------------------------------------------------------------------------
 * Name: _fm_cli_getdevinfo
 *
 * Description: This API copy file from memory card to nor flash.
 *                      async_read file_name num_byte cnt_open cnt_read
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
 #ifdef CLI_LVL_ALL
 static INT32   _fm_cli_getinfo(INT32 i4_argc, const CHAR** pps_argv)
 {
    FM_FILE_INFO_T  t_info;
    HANDLE_T    h_file;
    INT32 i4_ret;

    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("getinfo [filename]\n\r");
        return CLIR_OK;
    }   

    i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                       pps_argv[1],
                       FM_READ_ONLY, 
                       0666, 
                       FALSE, 
                       &h_file);

    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Can not open file:%s\n\r", pps_argv[1]);
        return CLIR_OK;
    }
    
    i4_ret = x_fm_get_info_by_handle(h_file, &t_info);

    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Can not get file info:%s\n\r", pps_argv[1]);
        return CLIR_OK;
    }
   
    x_dbg_stmt("file size: %lld\n\r", t_info.ui8_size);
    x_dbg_stmt("block size: %d\n\r", t_info.ui4_blk_size);
    x_dbg_stmt("block count: %lld\n\r", t_info.ui8_blk_cnt);
    x_dbg_stmt("start LBA: %d\n\r", t_info.ui4_start_lba);

    x_fm_close(h_file);

    return CLIR_OK;
 }
 #endif
/*-----------------------------------------------------------------------------
 * Name: _fm_cli_getfsinfo
 *
 * Description: This API copy file from memory card to nor flash.
 *                      async_read file_name num_byte cnt_open cnt_read
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))
 static INT32   _fm_cli_getfsinfo( INT32 i4_argc, const CHAR** ps_argv)
 {
    FM_FS_INFO_T    t_fs_info;
    INT32 i4_ret;
    if( i4_argc < 2)
    {
        x_dbg_stmt("getfsinfo  [file/dir path]");
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, ps_argv[1], &t_fs_info);
    if ( i4_ret != FMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    switch( t_fs_info.e_type)
    {
        case FM_TYPE_FAT16:
            x_dbg_stmt("FAT16\n");
            break;
        case FM_TYPE_FAT32:
            x_dbg_stmt("FAT32\n");
            break;
        case FM_TYPE_NTFS:
            x_dbg_stmt("NTFS\n");
            break;
        case FM_TYPE_INVAL:
            x_dbg_stmt("Invalid File system\n");
            break;
        default:
            break;
    }

    x_dbg_stmt("align: %d, min_blk_size: %d\n", t_fs_info.ui4_alignment, t_fs_info.ui4_min_blk_size);
    x_dbg_stmt("total blk #: %lld, free blk #: %lld\n", t_fs_info.ui8_total_blk, t_fs_info.ui8_free_blk);
    
    return CLIR_OK;
 }
#endif
/*-----------------------------------------------------------------------------
 * Name: _fm_cli_getdirinfo
 *
 * Description: This API copy file from memory card to nor flash.
 *                      async_read file_name num_byte cnt_open cnt_read
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))
 static INT32   _fm_cli_getdirinfo( INT32 i4_argc, const CHAR** ps_argv)
 {
    FM_DIR_INFO_T t_dir_info;
    INT32   i4_ret;
    
    if( i4_argc < 2)
    {
        x_dbg_stmt("getdirinfo  [dir path]");
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_fm_get_dir_info(NULL_HANDLE, ps_argv[1], &t_dir_info);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Get directory info fail: %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Dir number:%d\n", t_dir_info.ui4_dir_num);
    x_dbg_stmt("File number:%d\n", t_dir_info.ui4_file_num);

    return CLIR_OK;
 }
#endif
/*-----------------------------------------------------------------------------
 * Name: _fm_cli_async_read
 *
 * Description: This API copy file from memory card to nor flash.
 *                      async_read file_name num_byte cnt_open cnt_read
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
UINT32 ui4_total_cnt;
static VOID    my_async_nfy(HANDLE_T   h_req, VOID* pv_tag, FM_ASYNC_COND_T e_async_cond, UINT32 ui4_data)
{
    
    if( e_async_cond == FM_ASYNC_COND_READ_OK)
    {
        ui4_total_cnt += ui4_data;
        /* x_dbg_stmt("Success!h_req: %d, pv_tag: %x, ui4_data: %d, total: %d\n", h_req, (UINT32)pv_tag, ui4_data, ui4_total_cnt);*/
        x_dbg_stmt("Read OK(h_req: %d)\n");
    }
    else if (e_async_cond == FM_ASYNC_COND_ABORT_OK)
    {
        x_dbg_stmt("Abort OK!h_req: %d\n", h_req);
    }
    else
    {
        x_dbg_stmt("FM_ASYNC_READ_FAIL!\n");
        ASSERT(0);
    }
    /*
    x_dbg_stmt("buf: %x,%x,%x,%x\n", *(UINT8*)pv_tag, *((UINT8*)pv_tag+1), *((UINT8*)pv_tag+2), *((UINT8*)pv_tag+3));
    */
    x_dbg_stmt("free %x\n", pv_tag);
    x_mem_free((UINT8*)pv_tag);
    
}
static INT32    _fm_cli_async_read(INT32 i4_argc, const CHAR** ps_argv)
{
    /*
      * 1. Open file in N times.
      * 2. Read it in M times and check x_fm_feof M times.
      *
      */
    INT32    i4_ret, i4_i, i4_req_num;
    INT32    i4_cnt;
    HANDLE_T    h_req, h_src_file;
    UINT8   *buf = NULL;
    FM_FILE_INFO_T  t_src_file_info;
    BOOL    b_eof = FALSE;

    i4_i = 0;
    StringToNumber( ps_argv[2], &i4_cnt);
    
    while( i4_i++ < i4_cnt)
    {
        i4_req_num = 0;
        ui4_total_cnt = 0;
        x_dbg_stmt("The %dth time test!\n", i4_i);
        x_thread_delay(500);
       /* Open file in I/O Direct mode */
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_argv[1], FM_READ_ONLY, 0666, TRUE , &h_src_file);
        if ( i4_ret != FMR_OK)
        {
             x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_argv[1], i4_ret);
             x_fm_close(h_src_file);
             return CLIR_CMD_EXEC_ERROR;
        }         
        i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_argv[1], &t_src_file_info);
        if ( i4_ret != FMR_OK)
        {
             x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
             return CLIR_CMD_EXEC_ERROR;
        }
        
        x_dbg_stmt("%s size is %d bytes\n",  ps_argv[1], t_src_file_info.ui8_size);
        x_dbg_stmt("%s blk size is %d \n",  ps_argv[1], t_src_file_info.ui4_blk_size);
        x_dbg_stmt("%s blk cnt is %d \n",  ps_argv[1], t_src_file_info.ui8_blk_cnt);
  
        b_eof = FALSE;
        while( !b_eof)
        {
            i4_req_num++;
            i4_req_num = i4_req_num % 10;
            do        
            {
                x_thread_delay(1);
                buf = (UINT8*) x_mem_alloc(64* 1024);            
            } while( buf == NULL);

            i4_ret = x_fm_read_async(h_src_file,(VOID *) buf , 64* 1024, 128, my_async_nfy, buf, &h_req);
            x_dbg_stmt("buf: %x, h_req: %d\n", (UINT32)buf, h_req);

            if ( i4_req_num == 0)
            {
                x_dbg_stmt("abort req: %d\n", h_req);
                if ( x_handle_valid( h_req))
                    x_fm_abort_async(h_req);
            }
            
            /* x_dbg_stmt("async rw handle: %d\n", h_req); */
            switch ( i4_ret)
            {
                case FMR_OK:
                    break;
                case FMR_EOF:
                    b_eof = TRUE;
                    x_dbg_stmt("EOF!\n");
                    x_fm_abort_async(h_src_file);
                    break;
                case FMR_INVAL:
                    b_eof = TRUE;
                    x_dbg_stmt("FMR_INVAL!\n");
                    x_mem_free(buf);
                    break;
                default:
                    b_eof = TRUE;
                    x_dbg_stmt("Async Read Fail!(%d).\n", i4_ret);
                    x_mem_free(buf);
                    break;
            }
        }
       x_thread_delay(1000);
       x_fm_close(h_src_file);
    }
    return CLIR_OK;
}


#endif
 
/*-----------------------------------------------------------------------------
 * Name: _fm_cli_cp
 *
 * Description: This API copy file from memory card to nor flash.
 *                      cp   source_file_name    dest_file_name
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_cp( INT32 i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_src_file, h_dest_file;
    INT32   i4_ret;
    UINT32 ui4_read_cnt, ui4_write_cnt, buf_size, ui4_file_size;
    FM_FILE_INFO_T t_src_file_info;
    UINT8 *buf;
    UINT8 *mem;
    INT32   b_ext_buf, i4_n;
    MyTime_T  t_start, t_end, t_start_read, 
                        t_end_read, t_read_time, t_total_read_time,
                        t_start_write, t_end_write, t_write_time, t_total_write_time;

    t_start.t_second = 0;
    t_start.ui2_ms = 0;
    t_end.t_second = 0;
    t_end.ui2_ms = 0;
    t_start_read.t_second = 0;
    t_start_read.ui2_ms = 0;
    t_end_read.t_second = 0;
    t_end_read.ui2_ms = 0;
    t_start_write.t_second = 0;
    t_start_write.ui2_ms = 0;
    t_end_write.t_second = 0;
    t_end_write.ui2_ms = 0;
    
    t_start.t_second = GET_CUR_TIME(&t_start.ui2_ms);
    if( i4_argc < 6)
    {
        x_dbg_stmt("Usage: cp src_file dest_file [read w/wo cache] [write w/wo cache] [buffer size(n * 4KB)]\nExample: cp /mnt/ms/tmp.txt /mnt/nor_flash/tmp.txt 0 1\n");
        return CLIR_CMD_EXEC_ERROR;
     }
    StringToNumber( ps_argv[3], &i4_n);
    b_ext_buf = i4_n == 0 ? TRUE : FALSE;
    /* Check memory card */
    if ( b_ext_buf == TRUE)
        x_dbg_stmt("Cacheless Read!\n");
    else
        x_dbg_stmt("Cached Read! \n");
   i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_argv[1], FM_READ_ONLY, 0666, b_ext_buf, &h_src_file);
   if ( i4_ret != FMR_OK)
   {
        x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_argv[1], i4_ret);
        x_fm_close(h_src_file);
        return CLIR_CMD_EXEC_ERROR;
   } 
   i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_argv[1], &t_src_file_info);
   if ( i4_ret != FMR_OK)
   {
        x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
   }
   x_dbg_stmt("%s size is %d bytes\n",  ps_argv[1], t_src_file_info.ui8_size);
   x_dbg_stmt("%s blk size is %d \n",  ps_argv[1], t_src_file_info.ui4_blk_size);
   x_dbg_stmt("%s blk cnt is %d \n",  ps_argv[1], t_src_file_info.ui8_blk_cnt);

    StringToNumber( ps_argv[4], &i4_n);
    b_ext_buf = i4_n == 0 ? TRUE : FALSE;
    if ( b_ext_buf == TRUE)
    {
        x_dbg_stmt("Write Cacheless\n");
    }
    else
    {
        x_dbg_stmt("Write with Cache\n");
    }
    i4_ret = x_fm_open(FM_ROOT_HANDLE,  
            ps_argv[2], FM_OPEN_CREATE|FM_READ_WRITE | FM_OPEN_TRUNC, 
            0666, 
            b_ext_buf, 
            &h_dest_file);
   if ( i4_ret != FMR_OK)
   {
        x_dbg_stmt("Open dest file: %s Fail: %d\n", ps_argv[2], i4_ret);
        x_fm_close(h_dest_file);
        x_fm_close(h_src_file);
        return CLIR_CMD_EXEC_ERROR;
   } 
    StringToNumber( ps_argv[5], &i4_n);
    ui4_file_size = t_src_file_info.ui8_size;
    buf_size = i4_n * 1024;
    x_dbg_stmt("buffer size: %d\n", (buf_size + 31) / 32 * 32);
    buf = x_mem_alloc(buf_size + 32);
    if ( buf == NULL)
    {
        x_dbg_stmt("There is no enough memory for buf\n");
        x_fm_close(h_dest_file);
        x_fm_close(h_src_file);
        return CLIR_CMD_EXEC_ERROR;
    }

    mem = (UINT8 *)((((UINT32) buf / 32) + 1) * 32);

    t_total_read_time.t_second = 0;
    t_total_read_time.ui2_ms = 0;
    t_total_write_time.t_second = 0;
    t_total_write_time.ui2_ms = 0;
    while( ui4_file_size)
    {
        t_start_read.t_second = GET_CUR_TIME(&t_start_read.ui2_ms);
        i4_ret = x_fm_read(h_src_file, mem, buf_size, &ui4_read_cnt);
        if ( i4_ret != FMR_OK && i4_ret != FMR_EOF)
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            x_dbg_stmt("Read operation fail[%d], %d bytes read\n", i4_ret, ui4_read_cnt);
            return CLIR_CMD_EXEC_ERROR;
        }
        t_end_read.t_second = GET_CUR_TIME(&t_end_read.ui2_ms);
        GetTimeDiff( &t_read_time, &t_end_read, &t_start_read);
        /*
        x_dbg_stmt("Read %d bytes Success!\n", ui4_read_cnt); 
        */
        t_start_write.t_second = GET_CUR_TIME(&t_start_write.ui2_ms);
        i4_ret = x_fm_write(h_dest_file, mem, ui4_read_cnt, &ui4_write_cnt);        
        if ( i4_ret != FMR_OK)
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            x_dbg_stmt("Write operation fail[%d], %d bytes written\n", i4_ret, ui4_write_cnt);
            return CLIR_CMD_EXEC_ERROR;
        }
        t_end_write.t_second = GET_CUR_TIME(&t_end_write.ui2_ms);
        GetTimeDiff( &t_write_time, &t_end_write, &t_start_write);
        
        ui4_file_size -= ui4_read_cnt;
        if( ui4_file_size < buf_size)
            buf_size = ui4_file_size;
        /*
        x_dbg_stmt("Write %d bytes Success!\n", ui4_write_cnt);
        */
        t_total_read_time.t_second += t_read_time.t_second;
        t_total_read_time.ui2_ms += t_read_time.ui2_ms;
        t_total_write_time.t_second += t_write_time.t_second;
        t_total_write_time.ui2_ms += t_write_time.ui2_ms;
    }
    x_dbg_stmt("File Copied!\n");
    
    /* t_start_write.t_second = GET_CUR_TIME(&t_start_write.ui2_ms); */
    x_fm_close(h_dest_file);
/*
    t_end_write.t_second = GET_CUR_TIME(&t_end_write.ui2_ms);    
    GetTimeDiff( &t_write_time, &t_end_write, &t_start_write);
    t_total_write_time.t_second += t_write_time.t_second;
    t_total_write_time.ui2_ms += t_write_time.ui2_ms;
*/    
    x_fm_close(h_src_file);
    t_end.t_second = GET_CUR_TIME(&t_end.ui2_ms);
    x_mem_free(buf);
    MyTime_T  t_delta_time;
    GetTimeDiff( &t_delta_time, &t_end, &t_start);
    x_dbg_stmt("delta time: %lld seconds, %dms\n", t_delta_time.t_second, t_delta_time.ui2_ms);

    t_total_read_time.t_second += (t_total_read_time.ui2_ms / 1000);
    t_total_read_time.ui2_ms %= 1000;
    
    t_total_write_time.t_second += (t_total_write_time.ui2_ms / 1000);
    t_total_write_time.ui2_ms %= 1000;

    x_dbg_stmt("read time: %llds, %dms\n", t_total_read_time.t_second, t_total_read_time.ui2_ms);
    x_dbg_stmt("write time: %llds, %dms\n", t_total_write_time.t_second, t_total_write_time.ui2_ms);
    
    return CLIR_OK;
    
}
#endif
 /*-----------------------------------------------------------------------------
 * Name: _fm_cli_dir
 *
 * Description: List files under a specified directory
 *
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_dir(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    //INT32 i4_ret;
    HANDLE_T    h_dir;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i=0;
    /* 
    x_dbg_stmt("Not support yet!\n");  
    return CLIR_CMD_EXEC_ERROR;
    */
    if ( i4_argc < 2)
    {
        x_dbg_stmt("Usage: dir [directory_name]\n");
        return CLIR_CMD_EXEC_ERROR;
    }    

    if ( FMR_OK != x_fm_open_dir(FM_ROOT_HANDLE, ps_argv[1], &h_dir))
    {
        x_dbg_stmt("Open this directory fail!\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    /* looply get directories under current directory */
    /*
    i4_ret = x_fm_read_dir_entries(h_dir, at_dir_entry, 512, &ui4_num_entries);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("err(%d)\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = 0; 
    while ( ui4_num_entries-- != 0)
    {
        x_dbg_stmt("%s\n", at_dir_entry[i4_ret++].s_name);
    }
    */
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("Offset:%d, Name:%s\n",i++, at_dir_entry[0].s_name);    
    }
    
    x_fm_close(h_dir);
    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_mkdir(INT32 i4_argc, const CHAR** ps_argv)
{
    //HANDLE_T h_dir;

    if (i4_argc < 2)
    {
        x_dbg_stmt("Usage: mkdir [directory_name]\n");
        return CLIR_CMD_EXEC_ERROR;
    }    

    if (FMR_OK != x_fm_create_dir(FM_ROOT_HANDLE, ps_argv[1], 0777))
    {
        x_dbg_stmt("create this directory fail!\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}
#endif

 /*-----------------------------------------------------------------------------
 * Name: _fm_cli_rm
 *
 * Description: List files under a specified directory
 *
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_rm(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    INT32 i4_ret;

    if ( i4_argc != 2)
    {
        x_dbg_stmt("Usage: rm file_name\nExample: rm /mnt/ms/test.txt\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = x_fm_delete_file(FM_ROOT_HANDLE, ps_argv[1]);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Deletion Error[%d].\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_rmdir(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    INT32 i4_ret;

    if ( i4_argc != 2)
    {
        x_dbg_stmt("Usage: rmdir dir_name\nExample: rm /mnt/nand_00_0/test_dir\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = x_fm_delete_dir(FM_ROOT_HANDLE, ps_argv[1]);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Deletion Error[%d].\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}
#endif

 /*-----------------------------------------------------------------------------
 * Name: _fm_cli_diff
 *
 * Description: List files under a specified directory
 *
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
INT32 _file_diff(const CHAR* file_1, const CHAR* file_2)
{
    HANDLE_T h_file1, h_file2;
    FM_FILE_INFO_T t_file1_info, t_file2_info;
    UINT8 *buf1, *buf2;
    UINT32 i, read_cnt1, read_cnt2;
    UINT64  ui8_total = 0, ui8_offset = 0;
    INT32 i4_ret;
    x_dbg_stmt("differing....\n");
    i4_ret = x_fm_open( FM_ROOT_HANDLE, file_1, FM_READ_ONLY, 0666, FALSE, &h_file1);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open file1 error[%d]!\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = x_fm_get_info_by_handle(h_file1, &t_file1_info);

    i4_ret = x_fm_open(FM_ROOT_HANDLE, file_2, FM_READ_ONLY, 0666, FALSE, &h_file2);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open file2 error[%d]!\n", i4_ret);
        x_fm_close(h_file1);
        return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = x_fm_get_info_by_handle(h_file2, &t_file2_info);

    if ( t_file1_info.ui8_size != t_file2_info.ui8_size)
    {
        x_dbg_stmt("Size differ(%lld, %lld).\n", t_file1_info.ui8_size, t_file2_info.ui8_size);
        return CLIR_CMD_EXEC_ERROR;
    }
    buf1 = x_mem_alloc(32 * 1024);
    if ( buf1 == NULL)
    {
        x_dbg_stmt("No enough memory\n");
            x_fm_close(h_file1);
            x_fm_close(h_file2);
        return CLIR_CMD_EXEC_ERROR;
    }
    buf2 = x_mem_alloc(32 * 1024);
    if ( buf2 == NULL)
    {
        x_dbg_stmt("No enough memory\n");
            x_fm_close(h_file1);
            x_fm_close(h_file2);
        x_mem_free(buf1);
        return CLIR_CMD_EXEC_ERROR;
    }
    do
    {
        i4_ret = x_fm_read( h_file1, buf1, 32 * 1024, &read_cnt1);
        if ( i4_ret != FMR_OK)
        {
            x_fm_close(h_file1);
            x_fm_close(h_file2);
            x_mem_free(buf1);
            x_mem_free(buf2);
            x_dbg_stmt("Read file1 error[%d]\n");
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_read( h_file2, buf2, 32 * 1024, &read_cnt2);
        if ( i4_ret != FMR_OK)
        {
            x_fm_close(h_file1);
            x_fm_close(h_file2);
            x_mem_free(buf1);
            x_mem_free(buf2);
            x_dbg_stmt("Read file2 error[%d]\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        if ( read_cnt1 != read_cnt2)
        {
            x_dbg_stmt("read_cnt1: %d, read_cnt: %d differ!\n");
            x_mem_free(buf1);
            x_mem_free(buf2);
            x_fm_close(h_file1);
            x_fm_close(h_file2);
            return CLIR_CMD_EXEC_ERROR;
        }

        for ( i = 0; i < read_cnt2; i++, ui8_offset++)
        {
            if ( buf1[i] != buf2[i])
            {
                x_dbg_stmt("buf1[%lld]: %d, buf2[%lld]: %d differ\n", ui8_offset, buf1[i], ui8_offset, buf2[i]);
                x_thread_delay(100);
            }
        }
        ui8_total += read_cnt1;
    }while( ui8_total < t_file1_info.ui8_size);
    x_fm_close(h_file1);
    x_fm_close(h_file2);
    x_mem_free(buf1);
    x_mem_free(buf2);
    x_dbg_stmt("comparison finished\n");
    return CLIR_OK;
}
static INT32 _fm_cli_diff(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file1, h_file2;
    FM_FILE_INFO_T t_file1_info, t_file2_info;
    UINT8 *buf1, *buf2;
    UINT32 i, read_cnt1, read_cnt2;
    UINT64  ui8_total = 0, ui8_offset = 0;


    if ( i4_argc != 3)
    {
        x_dbg_stmt("Usage: diff file_1 file_2\nOnly absolute path name is supported.\n");
        x_dbg_stmt("Argc: %d\n", i4_argc);
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Differing %s, %s\n", ps_argv[1], ps_argv[2]);
    i4_ret = x_fm_open( FM_ROOT_HANDLE, ps_argv[1], FM_READ_ONLY, 0666, FALSE, &h_file1);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open file1 error[%d]!\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = x_fm_get_info_by_handle(h_file1, &t_file1_info);

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_argv[2], FM_READ_ONLY, 0666, FALSE, &h_file2);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open file2 error[%d]!\n", i4_ret);
        x_fm_close(h_file1);
        return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = x_fm_get_info_by_handle(h_file2, &t_file2_info);

    FM_FS_INFO_T    t_fs_info;
    x_fm_get_fs_info( FM_ROOT_HANDLE, ps_argv[1], &t_fs_info);
    if ( t_fs_info.e_type == FM_TYPE_ROOTFS)
    {
        goto HERE;
    }
    x_fm_get_fs_info( FM_ROOT_HANDLE, ps_argv[2], &t_fs_info);
    if ( t_fs_info.e_type == FM_TYPE_ROOTFS)
    {
        goto HERE;
    }
   
    if ( t_file1_info.ui8_size != t_file2_info.ui8_size)
    {
        x_fm_close(h_file1);
        x_fm_close(h_file2);
        x_dbg_stmt("Size differ(%d, %d).\n", t_file1_info.ui8_size, t_file2_info.ui8_size);
        /*return CLIR_CMD_EXEC_ERROR*/;
    }
HERE:        
    buf1 = x_mem_alloc(32 * 1024);
    if ( buf1 == NULL)
    {
        x_dbg_stmt("No enought memory\n");
        x_fm_close(h_file1);
        x_fm_close(h_file2);
        return CLIR_CMD_EXEC_ERROR;
    }
    buf2 = x_mem_alloc(32 * 1024);
    if ( buf2 == NULL)
    {
        x_dbg_stmt("No enought memory\n");
        x_fm_close(h_file1);
        x_fm_close(h_file2);
        x_mem_free(buf1);
        return CLIR_CMD_EXEC_ERROR;
    }
    do
    {
        i4_ret = x_fm_read( h_file1, buf1, 32 * 1024, &read_cnt1);
        if ( i4_ret != FMR_OK)
        {
            x_fm_close(h_file1);
            x_fm_close(h_file2);
            x_mem_free(buf1);
            x_mem_free(buf2);
            x_dbg_stmt("Read file1 error[%d]\n");
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_read( h_file2, buf2, read_cnt1, &read_cnt2);
        if ( i4_ret != FMR_OK)
        {
            x_fm_close(h_file1);
            x_fm_close(h_file2);
            x_mem_free(buf1);
            x_mem_free(buf2);
            x_dbg_stmt("Read file2 error[%d]\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        if ( read_cnt1 != read_cnt2)
        {
            x_dbg_stmt("read_cnt1: %d, read_cnt: %d differ!\n");
            x_fm_close(h_file1);
            x_fm_close(h_file2);
            x_mem_free(buf1);
            x_mem_free(buf2);
            return CLIR_CMD_EXEC_ERROR;
        }

        for ( i = 0; i < read_cnt2; i++, ui8_offset++)
        {
            if ( buf1[i] != buf2[i])
            {
                x_dbg_stmt("buf1[%lld]: %d, buf2[%lld]: %d differ\n", ui8_offset, buf1[i], ui8_offset, buf2[i]);
            }
        }
        ui8_total += read_cnt1;
    }while( ui8_total < t_file1_info.ui8_size);
    
    x_dbg_stmt("comparison finished\n");
    x_fm_close(h_file1);
    x_fm_close(h_file2);
    x_mem_free(buf1);
    x_mem_free(buf2);
    return CLIR_OK;

}
#endif


/*-----------------------------------------------------------------------------
 * Name: _fm_cli_umount
 *
 * Description: Unmount a file system.
 *
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_umount(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    INT32 i4_ret;

    if ( i4_argc != 2)
    {
        x_dbg_stmt("Usage: umount /mnt/nor_flash\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = x_fm_umount(FM_ROOT_HANDLE, ps_argv[1]);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Unmount %s fail!\n", ps_argv[1]);
    }
    x_dbg_stmt("Unmount %s on /mnt/nor_flash\n", ps_argv[1]);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _fm_cli_mount
 *
 * Description: This API formats the given device as given file system, the FAT16 by default.
 *                      mount  node  mnt_point
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_mount(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    INT32 i4_ret;
    if( i4_argc < 3)
        x_dbg_stmt("Usage: mount device_node mnt_point\nExample: mount /dev/nor_5 /mnt/nor_flash\n");
    
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_argv[2], 0777);
    if( (i4_ret != FMR_OK) && (i4_ret != FMR_EXIST))
    {
        x_dbg_stmt("Make dir %s Fail!: %d\n",ps_argv[2], i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_argv[1], FM_ROOT_HANDLE, ps_argv[2]);
    if( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Mount %s fail: %d\n", ps_argv[1], i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Mount %s on %s\n", ps_argv[1], ps_argv[2]);
    return FMR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _fm_cli_mkfs
 *
 * Description: This API formats the given device as given file system, the FAT16 by default.
 *                      mkfs    node
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_mkfs(
    INT32    i4_argc,
    const CHAR** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_dir = NULL_HANDLE;

    if( i4_argc < 2)
        x_dbg_stmt("Usage: mkfs device_node\nExample: mkfs /dev/nor_5\n");

    x_dbg_stmt("We are going to format %s as FAT16\n", pps_argv[1]);

    i4_ret = x_fm_create_fs(h_dir, pps_argv[1], FM_TYPE_FAT, NULL, 0);
   if ( i4_ret == FMR_OK )
   {
        x_dbg_stmt("format %s successfully\n", pps_argv[1]);
        return CLIR_OK;
   }
   else    
   {        
        x_dbg_stmt("format %s fail: i4_ret = %d\n", pps_argv[1], i4_ret);
        return CLIR_CMD_EXEC_ERROR;
   }
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _fm_cli_get_dbg_level
 *
 * Description: This API gets the current setting of File Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL     
static INT32 _fm_cli_get_dbg_level(
    INT32        i4_argc,
    const CHAR** pps_argv)
{
    /* fm.gdl */
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("fm.gdl\n\r");
        return CLIR_OK;
    }    
        
    return x_cli_show_dbg_level(fm_get_dbg_level());
}
#endif /* CLI_LVL_ALL */ 

/*-----------------------------------------------------------------------------
 * Name: _fm_cli_set_dbg_level
 *
 * Description: This API sets the debug level of File Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_set_dbg_level(
    INT32        i4_argc,
    const CHAR** pps_argv)
{
    INT32       i4_ret;
    UINT16      ui2_dbg_level;     
    
    /* fm.sdl [level] */
    
    /* Check arguments */
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("fm.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info");
        return CLIR_OK;
    }    
        
    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_ret == CLIR_OK)
    {
        if (fm_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_ret = CLIR_CMD_EXEC_ERROR;  
        }
    }
    else
    {
        x_dbg_stmt("fm.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info");
        i4_ret = CLIR_OK;
    }

    x_dbg_stmt("set debug level %s\n\r", (i4_ret == CLIR_OK) ? "successful" : "failed");
    return i4_ret;
}
#endif /* CLI_LVL_ALL */    


#ifdef CLI_LVL_ALL
static INT32 _fm_cli_hdd_mount(INT32 i4_argc, const CHAR** pps_argv)
{

    INT32 i4_ret;
    UINT32 ui4_part_ns;

    UINT32 ui4_cnt;

    /*  2. mount point creation */

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt", 0666);

    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("/mnt is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            x_dbg_stmt("/mnt is already existed.  That is OK.\n");            
        }
        else
        {
            x_dbg_stmt("make /mnt fail\n");
            x_dbg_stmt("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    i4_ret = x_fm_get_part_ns(FM_ROOT_HANDLE, 
                              (const CHAR *) "/dev/hdd_00", 
                              &ui4_part_ns);

    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("x_fm_get_part_ns fails.\n");
        x_dbg_stmt("return value:%d\n", i4_ret);
        goto func_exit;
    }

    for (ui4_cnt = 0; ui4_cnt < ui4_part_ns; ui4_cnt++)
    {
        FM_PART_INFO_T t_part_info;

        CHAR ps_dev_name[32];
        CHAR ps_mnt_name[32];

        i4_ret = x_fm_get_part_info(FM_ROOT_HANDLE, 
                                    (const CHAR *) "/dev/hdd_00",
                                    ui4_cnt,
                                    &t_part_info);             

        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("x_fm_get_part_info fails.\n");
            x_dbg_stmt("return value:%d\n", i4_ret);
            goto func_exit;
        }

        x_strcpy((CHAR *) ps_dev_name, "/dev/");
        x_strcpy((CHAR *) ps_mnt_name, "/mnt/");

        x_strcat((CHAR *) ps_dev_name, (const CHAR *) t_part_info.ps_part_name);
        x_strcat((CHAR *) ps_mnt_name, (const CHAR *) t_part_info.ps_part_name);

        i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_mnt_name, 0666);

        if (i4_ret == FMR_OK)
        {
            x_dbg_stmt("%s is created.\n", ps_mnt_name);
        }
        else if (i4_ret != FMR_OK)
        {
            if (i4_ret == FMR_EXIST)
            {
                x_dbg_stmt("%s is already existed.  That is OK.\n", ps_mnt_name);
            }
            else
            {
                x_dbg_stmt("make %s fail.\n", ps_mnt_name);
                x_dbg_stmt("return value:%d\n", i4_ret);
                goto func_exit;
            }
        }

        i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_dev_name, FM_ROOT_HANDLE, ps_mnt_name);

        if (i4_ret == FMR_OK)
        {
            x_dbg_stmt("%s is mounted as %s.\n", ps_dev_name, ps_mnt_name);
        }
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("ERR: mount %s as %s fail.\n", ps_dev_name, ps_mnt_name);
            x_dbg_stmt("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

func_exit:
  
    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL

static INT32 _fm_cli_hdd_read_test(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret, i4_seek, i4_loop;    
    HANDLE_T h_cur_dir = NULL_HANDLE;
    HANDLE_T h_img_file = NULL_HANDLE;    
    UINT32 i4_read, i4_print;
    UINT64 i8_cur_pos;
    UCHAR pbBuf[16];
    
    /* fm.cftest [filename] [offset] */

    /* Check arguments */
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("hdd_read_test [filename] [offset(hex)]\n\r");
        return CLIR_OK;
    }   
    
    i4_seek = _StrToHex(pps_argv[2], x_strlen(pps_argv[2]));

    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/mnt/hdd_00_0/", &h_cur_dir);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_dir_path fail, ret = %d", i4_ret);
        goto func_exit;
    }
          
    i4_ret = x_fm_open(h_cur_dir, pps_argv[1], FM_READ_ONLY, 0777, FALSE, &h_img_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }
    
    i4_ret = x_fm_lseek(h_img_file, (INT64)i4_seek, FM_SEEK_BGN, &i8_cur_pos);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_lseek fail, ret = %d", i4_ret);
        goto func_exit;
    }
                   
    x_dbg_stmt("The File /mnt/hdd_00_0/%s in Offset %08Xh Data >>\n", pps_argv[1], i4_seek);    
    for(i4_loop = 0; i4_loop < 16; i4_loop++)
    {
        i4_ret = x_fm_read(h_img_file, pbBuf, 16, &i4_read);
        if (FMR_OK != i4_ret)
        {
            x_dbg_stmt("x_fm_read fail, ret = %d", i4_ret);
            goto func_exit;
        }        

        x_dbg_stmt("%08Xh :", i4_seek + 16*i4_loop);

        for(i4_print = 0; i4_print < i4_read; i4_print++)
        {
            x_dbg_stmt(" %02X", pbBuf[i4_print]);    
        }

        x_dbg_stmt("\n");
    }  
    
func_exit:
    
    if (h_cur_dir != NULL_HANDLE)     
    {
        i4_ret = x_fm_close(h_cur_dir);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close cur_dir fail, ret = %d", i4_ret);
        }  
    }
        
    if (h_img_file != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_img_file);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }
    
    return CLIR_OK;  
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_hdd_direct_read(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret, i4_seek, i4_loop;    
    HANDLE_T h_cur_dir = NULL_HANDLE;
    HANDLE_T h_img_file = NULL_HANDLE;    
    UINT32 i4_read, i4_print;
    UINT64 i8_cur_pos;
    UCHAR *pbBuf = NULL;
    UCHAR *pbBuf2 = NULL;
    
    /* fm.cftest [filename] [offset] */

    /* Check arguments */
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("hdd_read_direct [filename] [offset(hex)]\n\r");
        return CLIR_OK;
    }   
    
    i4_seek = _StrToHex(pps_argv[2], x_strlen(pps_argv[2]));

    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/mnt/hdd_00_0/", &h_cur_dir);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_dir_path fail, ret = %d", i4_ret);
        goto func_exit;
    }
          
    i4_ret = x_fm_open(h_cur_dir, pps_argv[1], FM_READ_ONLY, 0777, TRUE, &h_img_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }

    if ((i4_seek % 512) != 0)
    {
        x_dbg_stmt("wrong offset, offset should be 512 byte aligned.");
        goto func_exit;
    }
    
    i4_ret = x_fm_lseek(h_img_file, (INT64)i4_seek, FM_SEEK_BGN, &i8_cur_pos);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_lseek fail, ret = %d", i4_ret);
        goto func_exit;
    }
                   
    x_dbg_stmt("The File /mnt/hdd_00_0/%s in Offset %08Xh Data >>\n", pps_argv[1], i4_seek);    

    pbBuf = x_mem_alloc(1024 + 32);    /*  512 is HDD sector size, and 32 is DMA alignment */
    pbBuf2 = (UCHAR *)((((UINT32) pbBuf / 32) + 1) * 32);

    i4_ret = x_fm_read(h_img_file, pbBuf2, 1024, &i4_read); /*  direct I/O must have aligned size */
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_read fail, ret = %d", i4_ret);
        goto func_exit;
    }        

    for(i4_loop = 0; i4_loop < 64; i4_loop++)
    {
        x_dbg_stmt("%08Xh :", i4_seek + (16 * i4_loop));

        for(i4_print = 0; i4_print < 16; i4_print++)
        {
            x_dbg_stmt(" %02X", pbBuf2[(16 * i4_loop) + i4_print]);    
        }

        x_dbg_stmt("\n");
    }  
    
func_exit:

    if (pbBuf != NULL)
    {
        x_mem_free((VOID *) pbBuf);
    }
    
    if (h_cur_dir != NULL_HANDLE)     
    {
        i4_ret = x_fm_close(h_cur_dir);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close cur_dir fail, ret = %d", i4_ret);
        }  
    }
        
    if (h_img_file != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_img_file);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }
    
    return CLIR_OK;  
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_hdd_direct_write(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret, i4_seek;    
    HANDLE_T h_cur_dir = NULL_HANDLE;
    HANDLE_T h_img_file = NULL_HANDLE;    
    UINT32 i4_write;
    UINT64 i8_cur_pos;
    UCHAR *pbBuf = NULL;
    UCHAR *pbBuf2 = NULL;

    UINT8 ui1_pattern;

    /* fm.cftest [filename] [offset] */

    /* Check arguments */
    if ((i4_argc != 4) || (pps_argv == NULL))
    {
        x_dbg_stmt("hdd_read_write [filename] [offset(hex)] [pattern]\n\r");
        return CLIR_OK;
    }   
    
    i4_seek = _StrToHex(pps_argv[2], x_strlen(pps_argv[2]));

    ui1_pattern = pps_argv[3][0];

    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/mnt/hdd_00_0/", &h_cur_dir);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_dir_path fail, ret = %d", i4_ret);
        goto func_exit;
    }
          
    i4_ret = x_fm_open(h_cur_dir, pps_argv[1], FM_OPEN_CREATE | FM_READ_WRITE, 0777, TRUE, &h_img_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }

    if ((i4_seek % 512) != 0)
    {
        x_dbg_stmt("wrong offset, offset should be 512 byte aligned.");
        goto func_exit;
    }
    
    i4_ret = x_fm_lseek(h_img_file, (INT64)i4_seek, FM_SEEK_BGN, &i8_cur_pos);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_lseek fail, ret = %d", i4_ret);
        goto func_exit;
    }
                   
    pbBuf = x_mem_alloc(1024 + 32);    /*  512 is HDD sector size, and 32 is DMA alignment */
    pbBuf2 = (UCHAR *)((((UINT32) pbBuf / 32) + 1) * 32);

    x_memset((VOID *) pbBuf2, ui1_pattern, 1024 + 32);

    i4_ret = x_fm_write(h_img_file, pbBuf2, 1024, &i4_write); /*  direct I/O must have aligned size */
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_read write, ret = %d", i4_ret);
        goto func_exit;
    }        
   
func_exit:

    if (pbBuf != NULL)
    {
        x_mem_free((VOID *) pbBuf);
    }

    if (h_cur_dir != NULL_HANDLE)     
    {
        i4_ret = x_fm_close(h_cur_dir);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close cur_dir fail, ret = %d", i4_ret);
        }  
    }
        
    if (h_img_file != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_img_file);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }
    
    return CLIR_OK;  
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_dump_xentries(INT32 i4_argc, const CHAR** pps_argv)
{
      extern VOID fm_dump_xentries(VOID);

    fm_dump_xentries();

    return CLIR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: fm_cli_init
 *
 * Description: This API initializes File Manager CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FMR_OK                     Routine successful.
 *          FMR_CLI_ERROR              CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 fm_cli_init(VOID)
{
#if (defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST))
    INT32       i4_ret;    
    
    /* Attach File Manager CLI command table to CLI */
    i4_ret = x_cli_attach_cmd_tbl(at_fm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);
    
    if (i4_ret != CLIR_OK)
    {
        return FMR_CLI_ERROR;
    }

    return FMR_OK;
#else
    return FMR_OK;
#endif      
}

/******************************* QA Test *************************************/
#ifdef CLI_LVL_ALL
/*
  * 1. Mount to /mnt/test
  * 2. Show file system information
  * 3. Show partition size
  * 4. ls /
  * 5. copy /mnt/test/test.mp3 /mnt/test/test0.mp3 (sync)
  * 6. copy /mnt/test/test.mp3 /mnt/test/test1.mp3 (async)
  * 7. diff /mnt/test/test1.mp3 /mnt/test/test2.mp3
  * 8. ls /
  * 9. Unmount
  */

static INT32    _fm_cli_test(INT32  i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_src_file, h_dest_file;
    FM_FS_INFO_T    t_fs_info;
    FM_FILE_INFO_T  t_file_info;;
    INT32   i4_ret;
    UINT64  ui8_file_size;

    x_dbg_stmt("Mount: ");
    i4_ret = x_fm_mount( FM_ROOT_HANDLE, ps_argv[1], FM_ROOT_HANDLE, "/mnt");
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;

    x_dbg_stmt("File System Type: ");
    i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, "/mnt", &t_fs_info);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;
    x_dbg_stmt("type: ");
    switch( t_fs_info.e_type)
    {
        case FM_TYPE_FAT16:
            x_dbg_stmt("FAT16\n");
            break;
        case FM_TYPE_FAT32:
            x_dbg_stmt("FAT32\n");
            break;
        case FM_TYPE_FAT12:
            x_dbg_stmt("FAT12\n");
            break;
        case FM_TYPE_FAT:
            x_dbg_stmt("FAT\n");
            break;
        case FM_TYPE_INVAL:
            x_dbg_stmt("INVAL\n");
            break;
        default:
            x_dbg_stmt("Unknown(%d)\n", t_fs_info.e_type);
            break;
    }
    x_dbg_stmt("block size: %lld\n", t_fs_info.ui8_blk_size);
    x_dbg_stmt("block cnt: %lld\n", t_fs_info.ui8_total_blk);

    x_dbg_stmt("Open src file: ");
    i4_ret = x_fm_open( FM_ROOT_HANDLE, "/mnt/test.mp3", FM_READ_ONLY, 0666, FALSE, &h_src_file);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;

    x_dbg_stmt("Get file info: ");
    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, "/mnt/test.mp3", &t_file_info);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;
    x_dbg_stmt("file size: %lld\n", t_file_info.ui8_size);

    x_dbg_stmt("Open dest file: ");
    i4_ret = x_fm_open( FM_ROOT_HANDLE, "/mnt/test0.mp3", FM_OPEN_CREATE|FM_READ_WRITE, 0666, FALSE, &h_dest_file);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;

    ui8_file_size = t_file_info.ui8_size;
    UINT32  ui4_buf_size = 32 * 1024, ui4_read, ui4_write;
    UINT8   *pui1_buf = x_mem_alloc( ui4_buf_size);
    if ( pui1_buf == NULL)
    {
        x_dbg_stmt("No Enough Memory.\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    UINT8   *pui1_buf1 = x_mem_alloc( ui4_buf_size);
    if ( pui1_buf1 == NULL)
    {
        x_dbg_stmt("No Enough Memory.\n");
        x_mem_free(pui1_buf);
        return CLIR_CMD_EXEC_ERROR;
    }


    x_dbg_stmt("Copying:  ");
    do
    {
        ui4_read = 0;
        ui4_write = 0;
        /* x_dbg_stmt("read %d bytes  ", ui4_buf_size); */
        i4_ret = x_fm_read( h_src_file, pui1_buf, ui4_buf_size, &ui4_read);
        if ( i4_ret != FMR_OK)
        {
            x_dbg_stmt("Read Fail: %d\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }

        /* x_dbg_stmt("write %d bytes  ", ui4_buf_size); */
        i4_ret = x_fm_write( h_dest_file, pui1_buf, ui4_read, &ui4_write);
        if ( i4_ret != FMR_OK)
        {
            x_dbg_stmt("Write Fail: %d\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }
        ui8_file_size -= ui4_read;
    } while( ui8_file_size != 0);
    x_dbg_stmt("Success");

    UINT64 ui8_cur_pos = 0;
    x_dbg_stmt("lseek source to begin ");
    i4_ret = x_fm_lseek( h_src_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");

    x_dbg_stmt("lseek dest to begin ");
    i4_ret = x_fm_lseek( h_dest_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");

    x_dbg_stmt("Compare source file with dest file: ");
    ui8_file_size = t_file_info.ui8_size;
    do
    {
        ui4_read = 0;
        ui4_write = 0;
        /* x_dbg_stmt("read1 %d bytes  ", ui4_buf_size); */
        i4_ret = x_fm_read( h_src_file, pui1_buf, ui4_buf_size, &ui4_read);
        if ( i4_ret != FMR_OK)
        {
            x_dbg_stmt("Read1 Fail: %d\n", i4_ret);
            ASSERT(0);
            return CLIR_CMD_EXEC_ERROR;
        }

        /* x_dbg_stmt("read2 %d bytes  ", ui4_buf_size); */
        i4_ret = x_fm_read( h_dest_file, pui1_buf1, ui4_buf_size, &ui4_write);
        if ( i4_ret != FMR_OK)
        {
            x_dbg_stmt("Read2 Fail: %d\n", i4_ret);
            ASSERT(0);
            return CLIR_CMD_EXEC_ERROR;
        }

        if ( ui4_read != ui4_write)
        {
            x_dbg_stmt("read count mismatch. read1: %d, read2: %d\n", ui4_read, ui4_write);
            x_mem_free( pui1_buf);
            x_mem_free(pui1_buf1);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            ASSERT(0);
            return CLIR_CMD_EXEC_ERROR;
        }

        UINT32  ui4_i;
        for ( ui4_i = 0; ui4_i < ui4_buf_size; ui4_i++)
        {
            if ( pui1_buf[ui4_i] != pui1_buf1[ui4_i])
            {
                x_dbg_stmt("file differ\n");
                x_mem_free( pui1_buf);
                x_mem_free(pui1_buf1);
                x_fm_close(h_src_file);
                x_fm_close(h_dest_file);
                ASSERT(0);
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        
        ui8_file_size -= ui4_read;
    } while( ui8_file_size != 0);
    x_dbg_stmt("Success\n");

    x_dbg_stmt("Close src file: ");
    i4_ret = x_fm_close( h_src_file);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;
    x_dbg_stmt("Close dest file: ");
    i4_ret = x_fm_close( h_dest_file);
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;

    x_dbg_stmt("UnMount: ");
    i4_ret = x_fm_umount( FM_ROOT_HANDLE, "/mnt");
    i4_ret == FMR_OK ? x_dbg_stmt("Success\n") : x_dbg_stmt("Fail\n");
    if ( i4_ret != FMR_OK)  return CLIR_CMD_EXEC_ERROR;

        
    return CLIR_OK;
}
#endif
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_stress_test( INT32 i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_mem_hdl = NULL_HANDLE;
    SIZE_T z_biggest_free;
    SIZE_T z_free_obj;
    SIZE_T z_allocated_obj;
    SIZE_T z_total_size;
    SIZE_T z_alloc_size, z_read_cnt, z_write_cnt;

    SIZE_T  z_initial, z_leak_mem = 0, z_snapshot0, z_snapshot1;
    
    INT32 i4_rpt_time;
    CHAR s_name[32];
    UINT8   *pui4_buf; 
    UINT32  ui4_i;

    StringToNumber(ps_argv[3], &i4_rpt_time);

    os_mem_scan_mem(h_mem_hdl);
    os_mem_cli_stats(h_mem_hdl,
                     &z_initial,
                     &z_biggest_free,
                     &z_free_obj,
                     &z_allocated_obj,
                     &z_total_size,
                     &z_alloc_size,
                     &(s_name[0]));
    pui4_buf = x_mem_alloc(sizeof(UINT8) * 32 * 1024);
    for( ui4_i = 0; ui4_i < i4_rpt_time; ui4_i++)
    {
        x_dbg_stmt("%d time action starts...\n", ui4_i);
        os_mem_scan_mem(h_mem_hdl);
        os_mem_cli_stats(h_mem_hdl,
                         &z_snapshot0,
                         &z_biggest_free,
                         &z_free_obj,
                         &z_allocated_obj,
                         &z_total_size,
                         &z_alloc_size,
                         &(s_name[0]));
        
        if ( z_leak_mem != 0)
        {
            /*fprintf(fp, "leak mem: %d bytes.\n", z_leak_mem);*/
            x_dbg_stmt("leak mem: %d bytes.\n", z_leak_mem);
        }

        FILE *fp1 = fopen(ps_argv[1], "r");
        FILE *fp2 = fopen(ps_argv[2], "w");
        do
        {
            z_read_cnt = fread( pui4_buf, sizeof(UINT8), 32*1024, fp1);
            z_write_cnt = fwrite( pui4_buf, sizeof(UINT8), z_read_cnt, fp2);
            if ( z_write_cnt != z_read_cnt)
            {
                x_dbg_stmt("write cnt != read cnt!\n");
            }
        } while( z_read_cnt == 32*1024);

        fclose( fp1);
        fclose( fp2);
        
        _file_diff( ps_argv[1], ps_argv[2]);
                              
        os_mem_scan_mem(h_mem_hdl);
        os_mem_cli_stats(h_mem_hdl,
                         &z_snapshot1,
                         &z_biggest_free,
                         &z_free_obj,
                         &z_allocated_obj,
                         &z_total_size,
                         &z_alloc_size,
                         &(s_name[0]));
        z_leak_mem = z_snapshot0 - z_snapshot1;
    }
    x_mem_free(pui4_buf);
    
    os_mem_scan_mem(h_mem_hdl);
    os_mem_cli_stats(h_mem_hdl,
                     &z_snapshot1,
                     &z_biggest_free,
                     &z_free_obj,
                     &z_allocated_obj,
                     &z_total_size,
                     &z_alloc_size,
                     &(s_name[0]));
    x_dbg_stmt("after all, leak mem: %d\n", z_initial - z_snapshot1);

    return CLIR_OK;                     
}
#endif
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_test_nor(INT32 argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T    h_nor;
    UINT32 ui4_val = 0, ui4_cnt = 0;
    UINT64  ui8_cur_pos;

    i4_ret = x_fm_make_entry(FM_ROOT_HANDLE, "/dev/mtd6", (FM_MODE_TYPE_CHR | 0666), DRVT_EEPROM, 0);
    if ( i4_ret != FMR_OK && i4_ret != FMR_EXIST)
    {
        x_dbg_stmt("make entry fail: %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_fm_open( FM_ROOT_HANDLE, "/dev/mtd6", FM_READ_WRITE, 0666, FALSE, &h_nor);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("open file fail: %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_fm_read(h_nor, &ui4_val, 4, &ui4_cnt);
    x_dbg_stmt("ui4_val: %d\n", ui4_val);
    
    i4_ret = x_fm_lseek( h_nor, 0, FM_SEEK_BGN, &ui8_cur_pos);

    StringToNumber(ps_argv[1], (INT32*)&ui4_val);
    i4_ret = x_fm_write( h_nor, &ui4_val, sizeof(ui4_val), &ui4_cnt);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("write file fail: %d\n", i4_ret);
        i4_ret = x_fm_close( h_nor);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = x_fm_lseek( h_nor, 0, FM_SEEK_BGN, &ui8_cur_pos);
    i4_ret = x_fm_read(h_nor, &ui4_val, 4, &ui4_cnt);
    x_dbg_stmt("ui4_val: %d\n", ui4_val);
    

    i4_ret = x_fm_close( h_nor);


    return CLIR_OK;
    
}
#endif
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_attack_usb(INT32 i4_argc, const CHAR * * ps_argv)
{
    UINT8 *pui1_buf;
    UINT32  ui4_file_size, ui4_i;
    UINT32 ui4_cnt;
    FILE *fp;
    
    StringToNumber( ps_argv[2], (INT32*)&ui4_file_size);
    StringToNumber( ps_argv[3], (INT32*)&ui4_cnt);
    
    x_dbg_stmt(" reapt r/w %s %d times\n", ps_argv[1], ui4_cnt);

    pui1_buf = (UINT8*)x_mem_alloc(sizeof(UINT8) * 64 * 1024);
    
    for(ui4_i = 0; ui4_i < 64 * 1024; ui4_i++)
    {
        pui1_buf[ui4_i] = ui4_i % 256;
    }
    
    fp = fopen(ps_argv[1], "w");
    for( ui4_i = 0; ui4_i < ui4_file_size; ui4_i++)
    {
        ui4_cnt = fwrite( pui1_buf, sizeof(UINT8), 64*1024, fp);
    }
    fclose(fp);

    UINT64 ui8_offset = 0;
    
    fp = fopen(ps_argv[1], "r");
    for( ui4_i = 0; ui4_i < ui4_file_size; ui4_i++, ui8_offset++)
    {
        ui4_cnt = fread( pui1_buf, sizeof(UINT8), 64*1024, fp);
        if ( pui1_buf[ui4_i] != (ui4_i % 256))
        {
            x_dbg_stmt("pui1_buf[%lld]: %d\n", ui8_offset, pui1_buf[ui4_i]);
        }
    }
    fclose(fp);

    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL
static VOID async_nfy_fct( 
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data)
{
    if ( e_async_cond == FM_ASYNC_COND_FAIL)
    {
        x_dbg_stmt("Async Req: %d FAIL!\n", h_req);
        ASSERT(0);
    }
    else if ( e_async_cond == FM_ASYNC_COND_ABORT_OK)
    {
        x_dbg_stmt("Abort OK\n");
        x_mem_free( pv_tag);
    }
    else if ( e_async_cond == FM_ASYNC_COND_READ_OK)
    {
        x_dbg_stmt("Read OK\n");
        x_mem_free(pv_tag);
    }
    else
    {
        ASSERT(0);
    }
    return;
}
static VOID _fm_concurrent_test_thread2(VOID *pi4_cnt)
{
    HANDLE_T    h_file, h_req;
    UINT8   *pui1_buf;
    INT32   i4_ret;
    UINT64  ui8_cur_pos;
    INT32   i4_cnt = 0, i4_cnt1;

    i4_cnt1 = *(INT32*)pi4_cnt;

    x_fm_open(FM_ROOT_HANDLE, "/mnt/hdd_00_0/test.mp3", FM_READ_ONLY, 0666, TRUE, &h_file);

    while(i4_cnt1)
    {
        i4_cnt++;
        do
        {
            x_thread_delay(10);
            pui1_buf = (UINT8*) x_mem_alloc(sizeof(UINT8) * 32 * 1024);
        } while( pui1_buf == NULL);
        i4_ret = x_fm_read_async( h_file, pui1_buf, 32768, 128, async_nfy_fct, pui1_buf, &h_req);

        if ( i4_ret == FMR_EOF)
        {
            x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
            i4_cnt1--;
        }
        else if ( i4_ret != FMR_OK)
        {   
            x_dbg_stmt("async read() return fail(%d), cur_pos: %lld\n", i4_ret, ui8_cur_pos);
            break;
        }
/*        
        if ( (i4_cnt % 4)== 0)
            x_fm_abort_async(h_req);
*/
    }

    x_fm_close(h_file);
    x_thread_exit();
}
static VOID _fm_concurrent_test_thread1(VOID *pi4_cnt)
{
    INT32   i4_ret;
    INT32 i4_cnt = *(INT32*)pi4_cnt;
    HANDLE_T    h_file1, h_file2;
    UINT8   ui1_val1, ui1_val2;
    UINT32  ui4_read_cnt1, ui4_read_cnt2;
    FM_FILE_INFO_T  t_info1, t_info2;
    UINT64  ui8_cur_pos1 = 0, ui8_cur_pos2 = 0;
    
    x_fm_open(FM_ROOT_HANDLE, "/mnt/hdd_00_0/test.mp3", FM_READ_ONLY, 0666, FALSE, &h_file1);
    x_fm_open(FM_ROOT_HANDLE, "/mnt/hdd_00_0/test1.mp3", FM_READ_ONLY, 0666, FALSE, &h_file2);

    x_fm_get_info_by_handle(h_file1, &t_info1);
    x_fm_get_info_by_handle(h_file2, &t_info2);
    
    x_thread_delay(1234);
    while(i4_cnt)
    {
        i4_ret = x_fm_lseek( h_file1, 1000, FM_SEEK_CUR, &ui8_cur_pos1);
        if ( i4_ret != FMR_OK)
        {
            goto END;
        }
        i4_ret = x_fm_lseek( h_file2, 1000, FM_SEEK_CUR, &ui8_cur_pos2);
        if ( i4_ret != FMR_OK)
        {
            goto END;
        }
/*
        x_dbg_stmt("file1: current pos: %lld\n", ui8_cur_pos1);
        x_dbg_stmt("file2: current pos: %lld\n", ui8_cur_pos2);
*/
        x_fm_read( h_file1, &ui1_val1, 1, &ui4_read_cnt1);
        x_fm_read( h_file2, &ui1_val2, 1, &ui4_read_cnt2);

        i4_ret = x_fm_lseek( h_file1, 1000, FM_SEEK_CUR, &ui8_cur_pos1);
        if ( i4_ret != FMR_OK)
        {
            goto END;
        }
        i4_ret = x_fm_lseek( h_file2, 1000, FM_SEEK_CUR, &ui8_cur_pos2);
        if ( i4_ret != FMR_OK)
        {
            goto END;
        }
        
        if ( ui1_val1 != ui1_val2)
        {
            x_dbg_stmt("ui1_val: %d, ui1_val2: %d differ\n", ui1_val1, ui1_val2);
            goto END;
        }

        if ( (ui8_cur_pos1 + 1000) > t_info1.ui8_size)
            x_fm_lseek( h_file1, 0, FM_SEEK_BGN, &ui8_cur_pos1);
        if ( (ui8_cur_pos2 + 1000) > t_info1.ui8_size)
            x_fm_lseek( h_file2, 0, FM_SEEK_BGN, &ui8_cur_pos2);
        i4_cnt--;
    }
    x_dbg_stmt("Thread   successfully finished!\n");
END:
    x_fm_close(h_file1);
    x_fm_close(h_file2);
    x_thread_exit();    
}
static INT32 _fm_concurrent_test( INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret, i4_cnt;
    HANDLE_T    h_thread1, h_thread2;


    StringToNumber( ps_argv[1], &i4_cnt);    
    i4_ret = x_thread_create(&h_thread1,
                             "test1",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             _fm_concurrent_test_thread1,
                             sizeof(INT32*),
                             (VOID*)&i4_cnt);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread1 error: %d\n", i4_ret);
        i4_ret = FMR_CORE;
        return CLIR_CMD_EXEC_ERROR;
    }
    
    StringToNumber( ps_argv[2], &i4_cnt);
    i4_ret = x_thread_create(&h_thread2,
                             "test2",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             _fm_concurrent_test_thread2,
                             sizeof(INT32*),
                             (VOID*)&i4_cnt);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread2 error: %d\n", i4_ret);
        i4_ret = FMR_CORE;
        return CLIR_CMD_EXEC_ERROR;
    }
    
    return CLIR_OK;
}
#endif
#ifdef CLI_LVL_ALL
static INT32 _fm_cli_max_test( INT32 i4_argc, const CHAR** ps_argv)
{
    INT32   i4_rpt_time, i4_ret;
    HANDLE_T    h_src_file, h_dest_file;
    UINT8   *pui1_buf = x_mem_alloc(sizeof(UINT8)*32768);
    UINT32  ui4_read_cnt, ui4_write_cnt;
    UINT64  ui8_cur_pos , ui8_tmp;
    UINT32  ui4_i;

    if ( i4_argc < 4)
        x_dbg_stmt("Usage: max_test [file_name] [rpt_time]\n");

    StringToNumber( ps_argv[3], &i4_rpt_time);

    while( i4_rpt_time--)
    {
        x_dbg_stmt(" %d time elapsed...\n");
        x_fm_open( FM_ROOT_HANDLE, ps_argv[1], FM_READ_ONLY, 0666, FALSE, &h_src_file);
        x_fm_open( FM_ROOT_HANDLE, ps_argv[2], FM_OPEN_CREATE|FM_READ_WRITE | FM_OPEN_TRUNC, 0666, FALSE, &h_dest_file);
        ui8_cur_pos = 0;
        do
        {
            i4_ret = x_fm_read(h_src_file, pui1_buf, 32768, &ui4_read_cnt);
            if ( i4_ret != FMR_OK && i4_ret != FMR_EOF)
            {
                x_dbg_stmt("read src file err(%d)\n", i4_ret);
                goto EXIT;
            }
            else if ( ui4_read_cnt == 0)
            {
                x_dbg_stmt("read src file err(%d)\n", i4_ret);
                break;
            }
            i4_ret = x_fm_write(h_dest_file, pui1_buf, 32768, &ui4_write_cnt);            
            if ( i4_ret != FMR_OK && i4_ret != FMR_EOF)
            {
                x_dbg_stmt("write dest err(%d)\n", i4_ret);
                goto EXIT;
            }
            
            for( ui4_i = 0; ui4_i < 32768; ui4_i++)
            {
                if ( pui1_buf[ui4_i] != (ui4_i % 256))
                {
                    x_dbg_stmt("read buf[%lld]: %d, error!\n", ui8_cur_pos, pui1_buf[ui4_i]);
                    goto EXIT;
                }
            }

            i4_ret = x_fm_lseek(h_dest_file, -32768, FM_SEEK_CUR, &ui8_tmp);
            if ( i4_ret != FMR_OK)
            {
                x_dbg_stmt("lssek err(%d)\n", i4_ret);
                goto EXIT;
            }
            i4_ret = x_fm_read(h_dest_file, pui1_buf, 32768, &ui4_read_cnt);
            if ( i4_ret != FMR_OK && i4_ret != FMR_EOF)
            {
                x_dbg_stmt("read dest err(%d)\n", i4_ret);
                goto EXIT;
            }
            for( ui4_i = 0; ui4_i < 32768; ui4_i++)
            {
                if ( pui1_buf[ui4_i] != (ui4_i % 256))
                {
                    x_dbg_stmt("write buf[%lld]: %d, error!\n", ui8_cur_pos, pui1_buf[ui4_i]);
                    goto EXIT;
                }
            }
            
            ui8_cur_pos += ui4_read_cnt;
        } while( 1 );
        x_fm_close(h_src_file);
        x_fm_close(h_dest_file);
    }

    return CLIR_OK;
EXIT:
    x_mem_free(pui1_buf);
    x_fm_close(h_src_file);
    x_fm_close(h_dest_file);
    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_odd_mount_udf(INT32 i4_argc, const CHAR** pps_argv)
{

    INT32 i4_ret;

    /*  2. mount point creation */

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt", 0666);

    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("/mnt is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            x_dbg_stmt("/mnt is already existed.  That is OK.\n");            
        }
        else
        {
            x_dbg_stmt("make /mnt fail\n");
            x_dbg_stmt("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt/disc1", 0666);

    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("/mnt/disc1 is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            x_dbg_stmt("/mnt/disc1 is already existed.  That is OK.\n");            
        }
        else
        {
            x_dbg_stmt("make /mnt/disc1 fail\n");
            x_dbg_stmt("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    i4_ret = x_fm_mount(FM_ROOT_HANDLE, "/dev/disc_00_0", FM_ROOT_HANDLE, "/mnt/disc1");

    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("/dev/disc_00_0 is mounted as /mnt/disc1.\n");
    }
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt(("ERR: mount /dev/disc_00_0 /mnt/disc1 fail\n"));
        x_dbg_stmt("return value:%d\n", i4_ret);
        goto func_exit;
    }

func_exit:
  
    return CLIR_OK;
}
#endif

#ifdef CLI_LVL_ALL

static INT32 _fm_cli_odd_read_test(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret, i4_seek, i4_loop;    
    HANDLE_T h_cur_dir = NULL_HANDLE;
    HANDLE_T h_img_file = NULL_HANDLE;    
    UINT32 i4_read, i4_print;
    UINT64 i8_cur_pos;
    UCHAR pbBuf[16];
    
    /* fm.cftest [filename] [offset] */

    /* Check arguments */
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("odd_read_test [filename] [offset(hex)]\n\r");
        return CLIR_OK;
    }   
    
    i4_seek = _StrToHex(pps_argv[2], x_strlen(pps_argv[2]));

    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/mnt/disc1/", &h_cur_dir);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_dir_path fail, ret = %d", i4_ret);
        goto func_exit;
    }
          
    i4_ret = x_fm_open(h_cur_dir, pps_argv[1], FM_READ_ONLY, 0777, FALSE, &h_img_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }
    
    i4_ret = x_fm_lseek(h_img_file, (INT64)i4_seek, FM_SEEK_BGN, &i8_cur_pos);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_lseek fail, ret = %d", i4_ret);
        goto func_exit;
    }
                   
    x_dbg_stmt("The File /mnt/disc1/%s in Offset %08Xh Data >>\n", pps_argv[1], i4_seek);    
    for(i4_loop = 0; i4_loop < 16; i4_loop++)
    {
        i4_ret = x_fm_read(h_img_file, pbBuf, 16, &i4_read);
        if (FMR_OK != i4_ret)
        {
            x_dbg_stmt("x_fm_read fail, ret = %d", i4_ret);
            goto func_exit;
        }        

        x_dbg_stmt("%08Xh :", i4_seek + 16*i4_loop);

        for(i4_print = 0; i4_print < i4_read; i4_print++)
        {
            x_dbg_stmt(" %02X", pbBuf[i4_print]);    
        }

        x_dbg_stmt("\n");
    }  
    
func_exit:
    
    if (h_cur_dir != NULL_HANDLE)     
    {
        i4_ret = x_fm_close(h_cur_dir);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close cur_dir fail, ret = %d", i4_ret);
        }  
    }
        
    if (h_img_file != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_img_file);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }
    
    return CLIR_OK;  
}
#endif

#ifdef CLI_LVL_ALL
static INT32 _fm_cli_odd_direct_read(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret, i4_seek, i4_loop;    
    HANDLE_T h_cur_dir = NULL_HANDLE;
    HANDLE_T h_img_file = NULL_HANDLE;    
    UINT32 i4_read, i4_print;
    UINT64 i8_cur_pos;
    UCHAR *pbBuf;
    
    /* fm.cftest [filename] [offset] */

    /* Check arguments */
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("odd_read_direct [filename] [offset(hex)]\n\r");
        return CLIR_OK;
    }   
    
    i4_seek = _StrToHex(pps_argv[2], x_strlen(pps_argv[2]));

    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/mnt/disc1/", &h_cur_dir);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_dir_path fail, ret = %d", i4_ret);
        goto func_exit;
    }
          
    i4_ret = x_fm_open(h_cur_dir, pps_argv[1], FM_READ_ONLY, 0777, TRUE, &h_img_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }

    if ((i4_seek % 4096) != 0)
    {
        x_dbg_stmt("wrong offset, offset should be 4096 byte aligned.");
        goto func_exit;
    }
    
    i4_ret = x_fm_lseek(h_img_file, (INT64)i4_seek, FM_SEEK_BGN, &i8_cur_pos);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_lseek fail, ret = %d", i4_ret);
        goto func_exit;
    }
                   
    x_dbg_stmt("The File /mnt/disc1/%s in Offset %08Xh Data >>\n", pps_argv[1], i4_seek);    

    pbBuf = x_mem_alloc(4096 + 32);    /*  512 is HDD sector size, and 32 is DMA alignment */
    pbBuf = (UCHAR *)((((UINT32) pbBuf / 32) + 1) * 32);

    i4_ret = x_fm_read(h_img_file, pbBuf, 4096, &i4_read); /*  direct I/O must have aligned size */
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_read fail, ret = %d", i4_ret);
        goto func_exit;
    }        

    for(i4_loop = 0; i4_loop < 256; i4_loop++)
    {
        x_dbg_stmt("%08Xh :", i4_seek + (16 * i4_loop));

        for(i4_print = 0; i4_print < 16; i4_print++)
        {
            x_dbg_stmt(" %02X", pbBuf[(16 * i4_loop) + i4_print]);    
        }

        x_dbg_stmt("\n");
    }  
    
func_exit:
    
    if (h_cur_dir != NULL_HANDLE)     
    {
        i4_ret = x_fm_close(h_cur_dir);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close cur_dir fail, ret = %d", i4_ret);
        }  
    }
        
    if (h_img_file != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_img_file);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }
    
    return CLIR_OK;  
}
#endif

#ifdef CLI_LVL_ALL

static VOID _fm_cli_hdd_async_read_fct(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data)
{
    x_dbg_stmt("async call back condition:%d\n\r", e_async_cond);
    x_dbg_stmt("async call back length:%d\n\r", ui4_data);
}

static INT32 _fm_cli_hdd_async_read(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;    
    HANDLE_T h_cur_dir = NULL_HANDLE;
    HANDLE_T h_img_file = NULL_HANDLE;            
    UCHAR *pbBuf;

    HANDLE_T h_req;

    /* fm.cftest [filename] [offset] */

    /* Check arguments */
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("hdd_async_read [filename]\n\r");
        return CLIR_OK;
    }   
    
    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/mnt/hdd_00_0/", &h_cur_dir);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_dir_path fail, ret = %d", i4_ret);
        goto func_exit;
    }
          
    i4_ret = x_fm_open(h_cur_dir, pps_argv[1], FM_READ_ONLY, 0777, TRUE, &h_img_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }
                  
    pbBuf = x_mem_alloc(1024 + 32);    /*  512 is HDD sector size, and 32 is DMA alignment */
    pbBuf = (UCHAR *)((((UINT32) pbBuf / 32) + 1) * 32);

    while (1)
    {
        i4_ret = x_fm_read_async(h_img_file, 
                                 pbBuf, 1024, 1, 
                                 _fm_cli_hdd_async_read_fct, 
                                 pbBuf, 
                                 &h_req);

        if (i4_ret == FMR_EOF)
        {
            x_dbg_stmt("EOF encountered.");
            break;
        }
        else
        {
            ASSERT(i4_ret == FMR_OK);
        }
    }
   
func_exit:
    
    if (h_cur_dir != NULL_HANDLE)     
    {
        i4_ret = x_fm_close(h_cur_dir);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close cur_dir fail, ret = %d", i4_ret);
        }  
    }
        
    if (h_img_file != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_img_file);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }
    
    return CLIR_OK;  
}

static INT32 _fm_cli_time_debug(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_cur_dir = NULL_HANDLE;
    HANDLE_T h_file = NULL_HANDLE;    

    FM_FILE_INFO_T t_file_info;

    UINT8 pui1_test_data[100];

    UINT32 ui4_sz;

    TIME_T t_time = 0;

    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/mnt/hdd_00_0/", &h_cur_dir);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_dir_path fail, ret = %d\n\r", i4_ret);
        goto func_exit;
    }

    x_fm_delete_file(h_cur_dir, "A.TXT");

    i4_ret = x_fm_open(h_cur_dir, "A.TXT", FM_OPEN_CREATE, 0777, TRUE, &h_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d\n\r", i4_ret);
        goto func_exit;
    }

    x_fm_close(h_file);

    i4_ret = x_fm_get_info_by_name(h_cur_dir, "A.TXT", &t_file_info);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_get_info_by_name fail, ret = %d", i4_ret);
        goto func_exit;
    }

    x_dbg_stmt("A.TXT 1st create time: %lld\n\r", t_file_info.ui8_create_time);
    x_dbg_stmt("A.TXT 1st modify time: %lld\n\r", t_file_info.ui8_modify_time);    

    x_thread_delay(10 * 1000);

    i4_ret = x_fm_open(h_cur_dir, "A.TXT", FM_READ_WRITE, 0777, FALSE, &h_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }

    i4_ret = x_fm_write(h_file, (const VOID *) pui1_test_data, 100, &ui4_sz);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_write fail, ret = %d", i4_ret);
        x_fm_close(h_file);
        goto func_exit;
    }

    x_fm_close(h_file);

    i4_ret = x_fm_get_info_by_name(h_cur_dir, "A.TXT", &t_file_info);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_get_info_by_name fail, ret = %d", i4_ret);
        goto func_exit;
    }

    x_dbg_stmt("A.TXT 2nd create time: %lld\n\r", t_file_info.ui8_create_time);
    x_dbg_stmt("A.TXT 2nd modify time: %lld\n\r", t_file_info.ui8_modify_time);    

    x_thread_delay(10 * 1000);

    t_time = GET_CUR_TIME(NULL);

    i4_ret = x_fm_set_time_by_name(h_cur_dir, "A.TXT", t_time, t_time);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_set_time_by_name fail, ret = %d", i4_ret);
        goto func_exit;
    }

    i4_ret = x_fm_get_info_by_name(h_cur_dir, "A.TXT", &t_file_info);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_get_info_by_name fail, ret = %d", i4_ret);
        goto func_exit;
    }

    x_dbg_stmt("A.TXT 3rd create time: %lld\n\r", t_file_info.ui8_create_time);
    x_dbg_stmt("A.TXT 3rd modify time: %lld\n\r", t_file_info.ui8_modify_time);    

func_exit:

    return CLIR_OK;  

}


#endif


#ifdef CLI_LVL_ALL
static INT32 _fm_cli_partition_dump(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret, i4_loop;    
    INT32 i4_lba;
    UINT32 i4_read, i4_print;
    UINT64 i8_cur_pos;
    UCHAR *pbBuf = NULL;
    UCHAR *pbMem = NULL;

    HANDLE_T h_partition;

    /* fm.cftest [filename] [offset] */

    /* Check arguments */
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("dump [physical device] [LBA] \n\r");
        return CLIR_OK;
    }   
    
    i4_lba = _StrToHex(pps_argv[2], x_strlen(pps_argv[2]));
         
    i4_ret = x_fm_open(FM_ROOT_HANDLE, pps_argv[1], FM_READ_ONLY, 0777, TRUE, &h_partition); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }
   
    i4_ret = x_fm_lseek(h_partition, (INT64)(i4_lba * 512), FM_SEEK_BGN, &i8_cur_pos);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_lseek fail, ret = %d", i4_ret);
        goto func_exit;
    }
                   
    x_dbg_stmt("The partition %s in LBA %d Data >>\n", pps_argv[1], i4_lba);    

    pbBuf = x_mem_alloc(512 + 32);    /*  512 is HDD sector size, and 32 is DMA alignment */
    pbMem = (UCHAR *)((((UINT32) pbBuf / 32) + 1) * 32);

    i4_ret = x_fm_read(h_partition, pbMem, 512, &i4_read); /*  direct I/O must have aligned size */
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_read fail, ret = %d", i4_ret);
        goto func_exit;
    }        

    for(i4_loop = 0; i4_loop < 32; i4_loop++)
    {
        x_dbg_stmt("%08Xh :", (16 * i4_loop));

        for(i4_print = 0; i4_print < 16; i4_print++)
        {
            x_dbg_stmt(" %02X", pbMem[(16 * i4_loop) + i4_print]);    
        }

        x_dbg_stmt("\n");
    }  
    
func_exit:

    if (pbBuf != NULL)
    {
        x_mem_free(pbBuf);
    }
           
    if (h_partition != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_partition);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }
    
    return CLIR_OK;  
}

static INT32 _fm_cli_odd_performance_test(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret, i4_loop;    
    HANDLE_T h_img_file = NULL_HANDLE;    
    UINT32 i4_read;
    UCHAR *pbBuf = NULL;
    UCHAR *pbMem = NULL;

    UINT32 ui4_start_tick, ui4_end_tick;
    
    if ((i4_argc != 1) || (pps_argv == NULL))
    {
        x_dbg_stmt("odd_perform\n\r");
        return CLIR_OK;
    }   
             
    i4_ret = x_fm_open(FM_ROOT_HANDLE, "/mnt/disc_00_0/BDMV/STREAM/00000.m2ts", FM_READ_ONLY, 0777, TRUE, &h_img_file); 
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_open fail, ret = %d", i4_ret);
        goto func_exit;
    }

    pbBuf = x_mem_alloc(32 * 6144 + 32);
    pbMem = (UCHAR *)((((UINT32) pbBuf / 32) + 1) * 32);

    ui4_start_tick = x_os_get_sys_tick();

    for (i4_loop = 0; i4_loop < 1024; i4_loop++)
    {
        i4_ret = x_fm_read(h_img_file, pbMem, 32 * 6144, &i4_read); /*  direct I/O must have aligned size */
        if (FMR_OK != i4_ret)
        {
            x_dbg_stmt("x_fm_read fail, ret = %d", i4_ret);
            goto func_exit;
        }
    }

    ui4_end_tick = x_os_get_sys_tick();

    x_dbg_stmt("read test total bytes = %d\n", 1024 * (32 * 6144));
    x_dbg_stmt("read test total time = %d\n", (ui4_end_tick - ui4_start_tick) * x_os_get_sys_tick_period());

func_exit:
        
    if (h_img_file != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_img_file);    
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("c_fm_close img_file fail, ret = %d", i4_ret);
        }        
    }

    if (pbBuf != NULL)
    {
        x_mem_free(pbBuf);
    }
    
    return CLIR_OK;  
}

static INT32 _fm_cli_recycle_buf(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;    
    
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("recycle_buffer [mount path]\n\r");
        return CLIR_OK;
    }   

    i4_ret = x_fm_recyc_buf(FM_ROOT_HANDLE, pps_argv[1]);

    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("x_fm_recyc_buf, ret = %d", i4_ret);
        goto func_exit;
    }

func_exit:
           
    return CLIR_OK;  
}

#endif

#ifdef CLI_LVL_ALL
#ifdef _NTFS_TEST_ 

static VOID ntfs_tc_async_nfy(HANDLE_T h_req, VOID* pv_tag, FM_ASYNC_COND_T e_async_cond, UINT32 ui4_data)
{
    NTFS_TC_ASYNC_TAG_T *pt_tag = NULL;

    if (pv_tag != NULL)
    {
        pt_tag = (NTFS_TC_ASYNC_TAG_T *)pv_tag;
        
        if( e_async_cond == FM_ASYNC_COND_READ_OK)
        {
            pt_tag->ui4_total_rcnt += ui4_data;
            /* x_dbg_stmt("Success!h_req: %d, pv_tag: %x, ui4_data: %d, total: %d\n", h_req, (UINT32)pv_tag, ui4_data, ui4_total_rcnt);*/
            x_dbg_stmt("Read OK(h_req: %d, total read %d bytes)\n", h_req, pt_tag->ui4_total_rcnt);
        }
        else if( e_async_cond == FM_ASYNC_COND_WRITE_OK)
        {
            pt_tag->ui4_total_wcnt += ui4_data;
            /* x_dbg_stmt("Success!h_req: %d, pv_tag: %x, ui4_data: %d, total: %d\n", h_req, (UINT32)pv_tag, ui4_data, ui4_total_rcnt);*/
            x_dbg_stmt("Write OK(h_req: %d, total write %d bytes)\n", h_req, pt_tag->ui4_total_wcnt);
        }
        else if (e_async_cond == FM_ASYNC_COND_ABORT_OK)
        {
            x_dbg_stmt("Abort OK!h_req: %d\n", h_req);
        }
        else if (e_async_cond == FM_ASYNC_COND_EOF)
        {
            pt_tag->b_eof = TRUE;
            x_dbg_stmt("Reach EOF!h_req: %d\n", h_req);
        }
        else
        {
            x_dbg_stmt("FM_ASYNC_RW_FAIL!\n");
            ASSERT(0);
        }
        /*
        x_dbg_stmt("buf: %x,%x,%x,%x\n", *(UINT8*)pv_tag, *((UINT8*)pv_tag+1), *((UINT8*)pv_tag+2), *((UINT8*)pv_tag+3));
        */

        pt_tag->ui4_done_bytes = ui4_data;
        x_sema_unlock(pt_tag->h_rw_lock);
        /*x_dbg_stmt("Unlock1!\n");*/
        x_handle_free(h_req);
    }
}

/* Get parent component,
   and the caller must free the returned string*/
CHAR* _ntfs_tc_get_prnt_comp(const CHAR *ps_s)
{
    const char c_c = '/';
    UINT16 ui2_len;
    CHAR *ps_prnt = NULL;
    CHAR *ps_temp = NULL;

    ps_temp = x_strrchr(ps_s, c_c);

    if (*(ps_temp+1) != '\0')
    {
        ui2_len = x_strlen(ps_s) - x_strlen(ps_temp);
        ps_prnt = (CHAR*)x_mem_alloc(ui2_len+1);
        x_strncpy(ps_prnt, ps_s, ui2_len);
        *(ps_prnt + ui2_len) = '\0';
    }
    else
    {
        CHAR *ps_temp2 = NULL;
        
        ui2_len = x_strlen(ps_s);
        ps_temp2 = (CHAR*)x_mem_alloc(ui2_len);
        x_strncpy(ps_temp2, ps_s, ui2_len-1);
        *(ps_temp2 + ui2_len - 1) = '\0';

        ps_temp = x_strrchr(ps_temp2, c_c);
        
        ui2_len = x_strlen(ps_temp2) - x_strlen(ps_temp);
        ps_prnt = (CHAR*)x_mem_alloc(ui2_len+1);
        x_strncpy(ps_prnt, ps_temp2, ui2_len);
        *(ps_prnt + ui2_len) = '\0';

        x_mem_free(ps_temp2);
    }

    return ps_prnt;
}

static INT32 _ntfs_tc_init_async_tag(NTFS_TC_ASYNC_TAG_T *pt_tag)
{
    if (pt_tag == NULL)
    {
        return NTFS_TC_ERR;
    }
    else
    {
        INT32 i4_ret;
        HANDLE_T h_rwlock;

        pt_tag->ui4_done_bytes = 0;
        pt_tag->ui4_total_rcnt = 0;
        pt_tag->ui4_total_wcnt = 0;
        pt_tag->b_eof = FALSE;

        i4_ret = x_sema_create(
                        &h_rwlock,
                        X_SEMA_TYPE_BINARY,
                        X_SEMA_STATE_UNLOCK);
        if (i4_ret != OSR_OK)
        {
            return NTFS_TC_ERR;
        }

        pt_tag->h_rw_lock = h_rwlock;

        return NTFS_TC_OK;
    }
}

/* Find the first set bit in an int */
static int ntfs_tc_ffs(int x)
{
    int r = 1;

    if (!x)
    {
        return 0;
    }
    if (!(x & 0xffff)) 
    {
        x >>= 16;
        r += 16;
    }
    if (!(x & 0xff)) 
    {
        x >>= 8;
        r += 8;
    }
    if (!(x & 0xf)) 
    {
        x >>= 4;
        r += 4;
    }
    if (!(x & 3)) 
    {
        x >>= 2;
        r += 2;
    }
    if (!(x & 1)) 
    {
        x >>= 1;
        r += 1;
    }
    return r;
}



/*********************************************************************
 *                                                                   *
 *                        NTFS Function Test                         *
 *                                                                   *
 *********************************************************************/

/* 1. Open File */
static INT32 ntfs_tc_fopen(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL; 
    HANDLE_T h_dir = NULL_HANDLE;
    HANDLE_T h_file = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    FM_FILE_INFO_T  t_src_file_info;
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    int i=1;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_fopen  [file path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    
    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_dir);
    x_mem_free(ps_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    i4_ret = x_fm_close(h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    while(i4_cnt-- > 0)
    {
        x_dbg_stmt("The %d time test\n", i++);
        /* 3.Open an existed file */
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, FALSE , &h_file);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Open src file fail: %d\n", i4_ret);
            return NTFS_TC_ERR;
        }

        /* 4.Get file info */
        i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
        if ( i4_ret != FMR_OK)
        {
             x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
             x_fm_close(h_file);
             return NTFS_TC_ERR;
        }
        x_dbg_stmt("%s size is %d bytes\n",  ps_path, t_src_file_info.ui8_size);
        x_dbg_stmt("%s blk size is %d \n",  ps_path, t_src_file_info.ui4_blk_size);
        x_dbg_stmt("%s blk cnt is %d \n",  ps_path, t_src_file_info.ui8_blk_cnt);

        i4_ret = x_fm_close(h_file);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Close file fail: %d\n", i4_ret);
            return NTFS_TC_ERR;
        }
    }
    return NTFS_TC_OK;
}

/* 2. Open Directory */
static INT32 ntfs_tc_dopen(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL;
    HANDLE_T h_prnt_dir = NULL_HANDLE;
    HANDLE_T h_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    int i = 1;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_dopen  [dir path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    
    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get prnt dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_prnt_dir);
    x_mem_free(ps_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    i4_ret = x_fm_close(h_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    while(i4_cnt-- > 0)
    {   
        x_dbg_stmt("The %d time test\n", i++);
        /*3.Get dir info */
        x_dbg_stmt("The directory %s info:\n", ps_path); 
        i4_ret = x_fm_open_dir(NULL_HANDLE, ps_path, &h_dir);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Open directory fail: %d\n", i4_ret);
            return NTFS_TC_ERR;
        }
        while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
        i4_ret = x_fm_close(h_dir);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Close directory fail: %d\n", i4_ret);
            return NTFS_TC_ERR;
        }
    }
    return NTFS_TC_OK;
}

/* 3. Close File */
static INT32 ntfs_tc_fclose(INT32 i4_argc, const CHAR** ps_argv)
{
    /* Test in open file */
    return NTFS_TC_OK;
}

/* 4. Close Directory */
static INT32 ntfs_tc_dclose(INT32 i4_argc, const CHAR** ps_argv)
{
    /* Test in open dir */
    return NTFS_TC_OK;
}

/* 5. Create File */
static INT32 ntfs_tc_fcreate(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL;
    HANDLE_T h_dir = NULL_HANDLE;
    HANDLE_T h_file = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    FM_FILE_INFO_T  t_src_file_info;
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    const CHAR *ps_path = NULL;

    if( i4_argc < 2)
    {
        x_dbg_stmt("ntfs_fcreate  [file path]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];

    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }

    /* 3.Open an not existed file */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_WRITE | FM_OPEN_CREATE, 0666, FALSE , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open and create src file fail: %d\n", i4_ret);
        x_fm_close(h_dir);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }

    /* 4.Get file info */
    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
         x_fm_close(h_file);
         x_fm_close(h_dir);
         x_mem_free(ps_prnt_dir);
         return NTFS_TC_ERR;
    }
    x_dbg_stmt("%s size is %d bytes\n",  ps_path, t_src_file_info.ui8_size);
    x_dbg_stmt("%s blk size is %d \n",  ps_path, t_src_file_info.ui4_blk_size);
    x_dbg_stmt("%s blk cnt is %d \n",  ps_path, t_src_file_info.ui8_blk_cnt);
    
    i4_ret = x_fm_close(h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close file fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }

    /* 5.Read parent dir again */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);
    x_mem_free(ps_prnt_dir);
    x_fm_seek_dir(h_dir, FM_SEEK_BGN, 0);
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    i4_ret = x_fm_close(h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    return NTFS_TC_OK;
}

/* 6. Create Directory */
static INT32 ntfs_tc_dcreate(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL;
    HANDLE_T h_prnt_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    const CHAR *ps_path = NULL;

    if( i4_argc < 2)
    {
        x_dbg_stmt("ntfs_dcreate  [dir path]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    
    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get prnt dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    /* 3.Create dir */
    i4_ret = x_fm_create_dir(NULL_HANDLE, ps_path, FM_ACCESS_MODE);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Create directory fail: %d\n", i4_ret);
        x_fm_close(h_prnt_dir);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }

    /* 4.Read parent dir again */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir); 
    x_mem_free(ps_prnt_dir);
    x_fm_seek_dir(h_prnt_dir, FM_SEEK_BGN, 0);
    while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    i4_ret = x_fm_close(h_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    return NTFS_TC_OK;
}

/* 7. Delete File */
static INT32 ntfs_tc_fdelete(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL;
    HANDLE_T h_dir = NULL_HANDLE;
    HANDLE_T h_file = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    FM_FILE_INFO_T  t_src_file_info;
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    const CHAR *ps_path = NULL;

    if( i4_argc < 2)
    {
        x_dbg_stmt("ntfs_fdelete  [file path]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }

    /* 3.Open file */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, FALSE , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open and create src file fail: %d\n", i4_ret);
        x_fm_close(h_dir);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }

    /* 4.Get file info */
    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
         x_fm_close(h_dir);
         x_fm_close(h_file);
         x_mem_free(ps_prnt_dir);
         return NTFS_TC_ERR;
    }
    x_dbg_stmt("%s size is %d bytes\n",  ps_path, t_src_file_info.ui8_size);
    x_dbg_stmt("%s blk size is %d \n",  ps_path, t_src_file_info.ui4_blk_size);
    x_dbg_stmt("%s blk cnt is %d \n",  ps_path, t_src_file_info.ui8_blk_cnt);
    i4_ret = x_fm_close(h_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Close file fail: %d\n", i4_ret);
         x_fm_close(h_dir);
         x_mem_free(ps_prnt_dir);
         return NTFS_TC_ERR;
    }

    /* 5.Delete file */
    
    x_dbg_stmt("To delete %s \n",  ps_path);
    i4_ret = x_fm_delete_file(FM_ROOT_HANDLE, ps_path);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Delete file fail: %d\n", i4_ret);
         x_fm_close(h_dir);
         x_mem_free(ps_prnt_dir);
         return NTFS_TC_ERR;
    }
    
    /* 6.Read parent dir again */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir); 
    x_fm_seek_dir(h_dir, FM_SEEK_BGN, 0);
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    x_mem_free(ps_prnt_dir);
    i4_ret = x_fm_close(h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    
    return NTFS_TC_OK;
}

/* 8. Delete Directory */
static INT32 ntfs_tc_ddelete(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL;
    HANDLE_T h_prnt_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    const CHAR *ps_path = NULL;

    if( i4_argc < 2)
    {
        x_dbg_stmt("ntfs_ddelete  [dir path]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get prnt dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    /* 3.Delelte dir */
    i4_ret = x_fm_delete_dir(NULL_HANDLE, ps_path);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Delete directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }

    /* 4.Read parent dir again */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);
    x_mem_free(ps_prnt_dir);
    x_fm_seek_dir(h_prnt_dir, FM_SEEK_BGN, 0);
    while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    i4_ret = x_fm_close(h_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    return NTFS_TC_OK;
}

/* 9. Rename File */
static INT32 ntfs_tc_frename(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_src_prnt_dir = NULL;
    CHAR *ps_dest_prnt_dir = NULL;
    HANDLE_T h_src_dir = NULL_HANDLE;
    HANDLE_T h_dest_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    const CHAR *ps_path = NULL, *ps_new_path = NULL;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_frename  [src file path] [dest file path]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    ps_new_path = ps_argv[2];
    /* 1.Get the parent directory info */
    ps_src_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);
    ps_dest_prnt_dir = _ntfs_tc_get_prnt_comp(ps_new_path);

    /* 2.Get dir info */
    x_dbg_stmt("The src parent directory %s info:\n", ps_src_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_src_prnt_dir, &h_src_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open src parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_src_prnt_dir);
        x_mem_free(ps_dest_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_src_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    if (x_strcmp(ps_src_prnt_dir, ps_dest_prnt_dir))
    {
        x_dbg_stmt("The dest parent directory %s info:\n", ps_dest_prnt_dir);  
        i4_ret = x_fm_open_dir(NULL_HANDLE, ps_dest_prnt_dir, &h_dest_dir);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Open dest parent directory fail: %d\n", i4_ret);
            x_mem_free(ps_src_prnt_dir);
            x_mem_free(ps_dest_prnt_dir);
            x_fm_close(h_src_dir);
            return NTFS_TC_ERR;
        }
        while (x_fm_read_dir_entries(h_dest_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
    }

    /* 3.Rename */
    i4_ret = x_fm_rename(NULL_HANDLE, ps_path, NULL_HANDLE, ps_new_path);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(ps_src_prnt_dir);
        x_mem_free(ps_dest_prnt_dir);
        x_dbg_stmt("Rename file fail: %d\n", i4_ret);
        x_fm_close(h_src_dir);
        if (h_dest_dir != NULL_HANDLE)
        {
            x_fm_close(h_dest_dir);
        }
        return NTFS_TC_ERR;
    }

    /* 4.Get dir info again */
    x_dbg_stmt("The src parent directory %s info:\n", ps_src_prnt_dir);
    
    x_fm_seek_dir(h_src_dir, FM_SEEK_BGN, 0);
    while (x_fm_read_dir_entries(h_src_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    if (x_strcmp(ps_src_prnt_dir, ps_dest_prnt_dir))
    {
        x_dbg_stmt("The dest parent directory %s info:\n", ps_dest_prnt_dir);

        x_fm_seek_dir(h_dest_dir, FM_SEEK_BGN, 0);
        while (x_fm_read_dir_entries(h_dest_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
    }
    x_mem_free(ps_src_prnt_dir);
    x_mem_free(ps_dest_prnt_dir);

    i4_ret = x_fm_close(h_src_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close src parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    if (h_dest_dir != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_dest_dir);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Close dest parent directory fail: %d\n", i4_ret);
            return NTFS_TC_ERR;
        }
    }

    return NTFS_TC_OK;
}

/* 10. Rename Directory */
static INT32 ntfs_tc_drename(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_src_prnt_dir = NULL;
    CHAR *ps_dest_prnt_dir = NULL;
    HANDLE_T h_src_dir = NULL_HANDLE;
    HANDLE_T h_dest_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    const CHAR *ps_path = NULL, *ps_new_path = NULL;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_drename  [src dir path] [dest dir path]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    ps_new_path = ps_argv[2];
    
    /* 1.Get the parent directory info */
    ps_src_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);
    ps_dest_prnt_dir = _ntfs_tc_get_prnt_comp(ps_new_path);

    /* 2.Get dir info */
    x_dbg_stmt("The src parent directory %s info:\n", ps_src_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_src_prnt_dir, &h_src_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open src parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_src_prnt_dir);
        x_mem_free(ps_dest_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_src_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    if (x_strcmp(ps_src_prnt_dir, ps_dest_prnt_dir))
    {
        x_dbg_stmt("The dest parent directory %s info:\n", ps_dest_prnt_dir);  
        i4_ret = x_fm_open_dir(NULL_HANDLE, ps_dest_prnt_dir, &h_dest_dir);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Open dest parent directory fail: %d\n", i4_ret);
            x_mem_free(ps_src_prnt_dir);
            x_mem_free(ps_dest_prnt_dir);
            x_fm_close(h_src_dir);
            return NTFS_TC_ERR;
        }
        while (x_fm_read_dir_entries(h_dest_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
    }

    /* 3.Rename */
    i4_ret = x_fm_rename(NULL_HANDLE, ps_path, NULL_HANDLE, ps_new_path);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Rename dir fail: %d\n", i4_ret);
        x_mem_free(ps_src_prnt_dir);
        x_mem_free(ps_dest_prnt_dir);
        x_fm_close(h_src_dir);
        if (h_dest_dir != NULL_HANDLE)
        {
            x_fm_close(h_dest_dir);
        }
        return NTFS_TC_ERR;
    }

    /* 4.Get dir info again */
    x_dbg_stmt("The src parent directory %s info:\n", ps_src_prnt_dir);  
    x_fm_seek_dir(h_src_dir, FM_SEEK_BGN, 0);
    while (x_fm_read_dir_entries(h_src_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }
    
    if (x_strcmp(ps_src_prnt_dir, ps_dest_prnt_dir))
    {
        x_dbg_stmt("The dest parent directory %s info:\n", ps_dest_prnt_dir);  
        x_fm_seek_dir(h_dest_dir, FM_SEEK_BGN, 0);
        while (x_fm_read_dir_entries(h_dest_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
    }
    x_mem_free(ps_src_prnt_dir);
    x_mem_free(ps_dest_prnt_dir);

    i4_ret = x_fm_close(h_src_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close src parent directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    if (h_dest_dir != NULL_HANDLE)
    {
        i4_ret = x_fm_close(h_dest_dir);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Close dest parent directory fail: %d\n", i4_ret);
            return NTFS_TC_ERR;
        }
    }

    return NTFS_TC_OK;
}

/* 11. Sync Read File */
static INT32 ntfs_tc_fread_sync(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_src_file;
    BOOL     b_eof = FALSE;
    BOOL     b_direct_mode = FALSE;
    UINT8    *buf = NULL, *temp_buf = NULL;
    FM_FS_INFO_T    t_fs_info;
    UINT32     ui4_done_bytes;
    UINT32     ui4_alignment;
    INT32      i4_sep;
    int      i_align_bits, i = 1;    
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_sync_read  [file path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    i4_sep = i4_cnt / 2;

    i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, ps_path, &t_fs_info);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Get file system info faild, %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    ui4_alignment = t_fs_info.ui4_alignment;
    i_align_bits = ntfs_tc_ffs(ui4_alignment) - 1;

    buf = (UINT8*) x_mem_calloc(1, 64 * 1024 + ui4_alignment);
    if (buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    x_dbg_stmt("Alloc Buf: %x\n", (UINT32)buf);
    
    while (i4_cnt -- > 0)
    {   
        if (i4_cnt < i4_sep)
        {
            b_direct_mode = TRUE;
        }
        else
        {
            b_direct_mode = FALSE;
        }
        
        x_dbg_stmt("The %d time test in %s mode!\n", i++, b_direct_mode ? ("direct") : ("NoDirect"));

        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, b_direct_mode , &h_src_file);
        if ( i4_ret != FMR_OK)
        {
             x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_path, i4_ret);
             if (buf != NULL)
             {
                x_mem_free(buf);
                buf = NULL;
             }
             return NTFS_TC_ERR;
        }  
        
        b_eof = FALSE;
  
        if (b_direct_mode)
        {
            /* In direct mode, the buffer address must be aligned
               to specific address due to the hardware's limitation */
            if ((UINT32)buf % ui4_alignment != 0)
            {
                temp_buf=  (UINT8*)(((((UINT32) buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
            }
            else
            {
                temp_buf = buf;
            }
        }
        else
        {
            temp_buf = buf;
        }

        while (!b_eof)
        {
            x_memset(buf, 0, 64 * 1024 + ui4_alignment);
            
            i4_ret = x_fm_read(h_src_file,(VOID *)temp_buf , 64*1024, &ui4_done_bytes);

            switch ( i4_ret)
            {
                case FMR_OK:
                    x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);
                    x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);
                    break;
                case FMR_EOF:
                    b_eof = TRUE;
                    x_dbg_stmt("EOF!\n");
                    break;
                case FMR_INVAL:
                    b_eof = TRUE;
                    x_dbg_stmt("FMR_INVAL!\n");
                    x_mem_free(buf);
                    buf = NULL;
                    break;
                default:
                    b_eof = TRUE;
                    x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                    x_mem_free(buf);
                    buf = NULL;
                    break;
            }
   
            if (i4_ret != FMR_OK)
            {                
                if (i4_ret == FMR_EOF)
                {
                    x_fm_close(h_src_file);
                }
                else
                {
                    return NTFS_TC_ERR;
                }
            }
        }
    }
    x_mem_free(buf);
    buf = NULL;
    return NTFS_TC_OK;
}

/* 12. Async Read File */
static INT32 ntfs_tc_fread_async(INT32 i4_argc, const CHAR** ps_argv)
{
    /*
     * 1. Open file in N times.
     * 2. Read it in M times and check x_fm_feof M times.
     */
    INT32    i4_ret, i4_i, i4_ret2;
    HANDLE_T h_req, h_src_file;
    UINT8    *buf = NULL, *temp_buf = NULL;
    FM_FILE_INFO_T  t_src_file_info;
    BOOL     b_eof = FALSE;
    FM_FS_INFO_T    t_fs_info;
    BOOL       b_direct_mode = TRUE;
    UINT32     ui4_alignment;
    NTFS_TC_ASYNC_TAG_T t_tag;
    int      i_align_bits;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_async_read  [file path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);

    i4_i = 0;

    if (x_fm_get_fs_info(FM_ROOT_HANDLE, ps_path, &t_fs_info))
    {
        return NTFS_TC_ERR;
    }
    ui4_alignment = t_fs_info.ui4_alignment;
    i_align_bits  = ntfs_tc_ffs(ui4_alignment) - 1;

    i4_ret = _ntfs_tc_init_async_tag(&t_tag);
    if (i4_ret != NTFS_TC_OK)
    {
        return NTFS_TC_ERR;
    }

    buf = (UINT8*) x_mem_alloc(64* 1024 + ui4_alignment);
    if (buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    x_dbg_stmt("Alloc Buf: %x\n", (UINT32)buf);

    while( i4_i++ < i4_cnt)
    {
        x_memset(buf, 0, 64 * 1024 + ui4_alignment);

        if (i4_i > i4_cnt/2)
        {
            b_direct_mode = FALSE;
        }
        
        t_tag.ui4_total_rcnt = 0;
        x_dbg_stmt("The %dth time test in %s mode!\n", i4_i, b_direct_mode ? ("direct") : ("NoDirect"));
        x_thread_delay(500);

        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, b_direct_mode , &h_src_file);
        if ( i4_ret != FMR_OK)
        {
             x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_path, i4_ret);
             if (buf != NULL)
             {
                x_mem_free(buf);
                buf = NULL;
             }
             return NTFS_TC_ERR;
        }
        
        if (i4_i % 2 == 0)
        {
            i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
            if ( i4_ret != FMR_OK)
            {
                 x_dbg_stmt("Get src file info by name fail: %d\n", i4_ret);
                 x_fm_close(h_src_file);
                 if (buf != NULL)
                 {
                    x_mem_free(buf);
                    buf = NULL;
                 }
                 return NTFS_TC_ERR;
            }
        }
        else
        {
            i4_ret = x_fm_get_info_by_handle(h_src_file, &t_src_file_info);
            if ( i4_ret != FMR_OK)
            {
                 x_dbg_stmt("Get src file info by handle fail: %d\n", i4_ret);
                 x_fm_close(h_src_file);
                 if (buf != NULL)
                 {
                    x_mem_free(buf);
                    buf = NULL;
                 }
                 return NTFS_TC_ERR;
            }
        }
        
        x_dbg_stmt("%s size is %d bytes\n",  ps_path, t_src_file_info.ui8_size);
        x_dbg_stmt("%s blk size is %d \n",  ps_path, t_src_file_info.ui4_blk_size);
        x_dbg_stmt("%s blk cnt is %d \n",  ps_path, t_src_file_info.ui8_blk_cnt);

        if (b_direct_mode)
        {
            /* In direct mode, the buffer address must be aligned
               to specific address due to the hardware's limitation */
            if ((UINT32)buf % ui4_alignment != 0)
            {
                temp_buf=  (UINT8*)(((((UINT32) buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
            }
            else
            {
                temp_buf = buf;
            }
        }
        else
        {
            temp_buf = buf;
        }
        
        b_eof = FALSE;
        /* Ensure the h_rwlock is unlocked at first */
        x_sema_unlock(t_tag.h_rw_lock);
        
        while (!b_eof)
        {   
            x_memset(buf, 0, 64 * 1024 + ui4_alignment);
            
            if (x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
            {
                if (buf != NULL)
                {
                    x_mem_free(buf);
                    buf = NULL;
                }
                x_fm_close(h_src_file);
                return NTFS_TC_ERR;
            }
            i4_ret = x_fm_read_async(h_src_file,(VOID *) temp_buf , 64*1024, 128, ntfs_tc_async_nfy, (VOID*)&t_tag, &h_req);

            x_thread_delay(50);

            if (i4_ret == FMR_OK)
            {
                /* if the read operation does't finish in 50ms,
                   we just try to abort the operation */
                i4_ret2 = x_sema_lock_timeout(t_tag.h_rw_lock, 50);
                if (i4_ret2 != OSR_OK)
                {
                    if (i4_ret2 == OSR_TIMEOUT)
                    {
                        x_dbg_stmt("Abort req: %d\n", h_req);
                        if (x_handle_valid(h_req))
                        {
                            x_fm_abort_async(h_req);
                        }
                    }
                    else
                    {
                        if (buf != NULL)
                        {
                            x_mem_free(buf);
                            buf = NULL;
                        }
                        x_fm_close(h_src_file);
                        return NTFS_TC_ERR;
                    }
                }  
                else   
                {
                    if (x_sema_unlock(t_tag.h_rw_lock))
                    {
                        if (buf != NULL)
                        {
                            x_mem_free(buf);
                            buf = NULL;
                        }
                        x_fm_close(h_src_file);
                        return NTFS_TC_ERR;
                    }
                }
            }
            else
            {
                if (x_sema_unlock(t_tag.h_rw_lock))
                {
                    if (buf != NULL)
                    {
                        x_mem_free(buf);
                        buf = NULL;
                    }
                    x_fm_close(h_src_file);
                    return NTFS_TC_ERR;
                }
            }
            if (t_tag.b_eof)
            {
                i4_ret = FMR_EOF;
            }
            switch (i4_ret)
            {
                case FMR_OK:
                    x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);
                    x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[t_tag.ui4_done_bytes-4],temp_buf[t_tag.ui4_done_bytes-3],temp_buf[t_tag.ui4_done_bytes-2],temp_buf[t_tag.ui4_done_bytes-1]);
                    break;
                case FMR_EOF:
                    b_eof = TRUE;
                    x_dbg_stmt("EOF!\n");
                    x_fm_abort_async(h_src_file);
                    break;
                case FMR_INVAL:
                    b_eof = TRUE;
                    x_dbg_stmt("FMR_INVAL!\n");
                    x_mem_free(buf);
                    buf = NULL;
                    break;
                default:
                    b_eof = TRUE;
                    x_dbg_stmt("Async Read Fail!(%d).\n", i4_ret);
                    x_mem_free(buf);
                    buf = NULL;
                    break;
            }
        }
        x_thread_delay(1000);
        x_fm_close(h_src_file);
    }
    if (buf != NULL)
    {
        x_mem_free(buf);
        buf = NULL;
    }
    return NTFS_TC_OK;
}

/* 13. Sync Write File */
static INT32 ntfs_tc_fwrite_sync(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_src_file, h_dest_file;
    BOOL     b_eof = FALSE;
    BOOL     b_direct_mode = FALSE;
    UINT8    *buf = NULL, *temp_buf = NULL;
    FM_FS_INFO_T    t_fs_info;
    UINT32 ui4_done_bytes, ui4_wbytes = 0;
    UINT32     ui4_alignment;
    int      i_align_bits;
    const CHAR *ps_src = NULL, *ps_dest = NULL;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_sync_write  read from [src file path], write to [dest file path]");
        return NTFS_TC_ERR;
    }
    ps_src = ps_argv[1];
    ps_dest = ps_argv[2];
    if (x_fm_get_fs_info(FM_ROOT_HANDLE, ps_src, &t_fs_info))
    {
        return NTFS_TC_ERR;
    }
    ui4_alignment = t_fs_info.ui4_alignment;
    i_align_bits = ntfs_tc_ffs(ui4_alignment) - 1;

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_src, FM_READ_ONLY, 0666, b_direct_mode , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_src, i4_ret);
         return NTFS_TC_ERR;
    }
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_dest, FM_READ_WRITE | FM_OPEN_CREATE, 0666, b_direct_mode , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open dest file: %s Fail: %d\n",  ps_dest, i4_ret);
         x_fm_close(h_src_file);
         return NTFS_TC_ERR;
    } 
    
    b_eof = FALSE;
    do        
    {
        x_thread_delay(1);
        buf = (UINT8*) x_mem_calloc(1, 64* 1024 + ui4_alignment);
        if (b_direct_mode)
        {
            /* In direct mode, the buffer address must be aligned
               to specific address due to the hardware's limitation */
            if ((UINT32)buf % ui4_alignment != 0)
            {
                temp_buf=  (UINT8*)(((((UINT32) buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
            }
            else
            {
                temp_buf = buf;
            }
        }
        else
        {
            temp_buf = buf;
        }
    } while( buf == NULL);
    
    x_dbg_stmt("Alloc Buf: %x\n", (UINT32)buf);
    while (!b_eof)
    {
        i4_ret = x_fm_read(h_src_file,(VOID *)temp_buf , 64*1024, &ui4_done_bytes);

        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);

            if (i4_ret == FMR_EOF)
            {
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }
        }

        i4_ret = x_fm_write(h_dest_file,(VOID *)temp_buf , ui4_done_bytes, &ui4_done_bytes);
        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                ui4_wbytes += ui4_done_bytes;
                x_dbg_stmt("Write OK! Total write %d bytes\n", ui4_wbytes);
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }
        }
    }
    return NTFS_TC_OK;
}

/* 14. Async Write File */
static INT32 ntfs_tc_fwrite_async(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_src_file, h_dest_file, h_req;
    BOOL     b_eof = FALSE;
    UINT8    *buf = NULL;
    NTFS_TC_ASYNC_TAG_T t_tag;
    const CHAR *ps_src = NULL, *ps_dest = NULL;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_async_write  read from [src file path] to [dest file path]");
        return NTFS_TC_ERR;
    }
    ps_src = ps_argv[1];
    ps_dest = ps_argv[2];

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_src, FM_READ_ONLY, 0666, FALSE , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_src, i4_ret);
         return NTFS_TC_ERR;
    }  
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_dest, FM_READ_WRITE | FM_OPEN_CREATE, 0666, FALSE , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open dest file: %s Fail: %d\n",  ps_dest, i4_ret);
         x_fm_close(h_src_file);
         return NTFS_TC_ERR;
    } 

    i4_ret = _ntfs_tc_init_async_tag(&t_tag);
    if (i4_ret != NTFS_TC_OK)
    {
        return NTFS_TC_ERR;
    }

    b_eof = FALSE;
    do        
    {
        x_thread_delay(1);
        buf = (UINT8*) x_mem_calloc(1, 64* 1024 + 32);
    } while( buf == NULL);
    
    x_dbg_stmt("Alloc Buf: %x\n", (UINT32)buf);

    /* Ensure the h_rwlock is unlocked at first */
    x_sema_unlock(t_tag.h_rw_lock);
    
    while (!b_eof)
    {
        if (x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return NTFS_TC_ERR;
        }
        /*x_dbg_stmt("Lock1!\n");*/
        i4_ret = x_fm_read_async(h_src_file,(VOID *)buf , 64*1024, 128, ntfs_tc_async_nfy, (VOID*)&t_tag, &h_req);
        x_thread_delay(20);

        if (i4_ret == FMR_OK)
        {
            if (x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
            {
                x_mem_free(buf);
                x_fm_close(h_src_file);
                x_fm_close(h_dest_file);
                return NTFS_TC_ERR;
            }
            /*x_dbg_stmt("Lock2!\n");*/
        }
        if (x_sema_unlock(t_tag.h_rw_lock))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return NTFS_TC_ERR;
        }
        /*x_dbg_stmt("Unlock2!\n");*/
        if (t_tag.b_eof)
        {
            i4_ret = FMR_EOF;
        }

        switch ( i4_ret)
        {
            case FMR_OK:
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_fm_abort_async(h_src_file);
                x_mem_free(buf);
                buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Async Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }
        }
        #if 1
        /* Async write */
        if (x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return NTFS_TC_ERR;
        }
        /*x_dbg_stmt("Lock1!\n");*/
        #if 1
        i4_ret = x_fm_write_async(h_dest_file,(VOID *)buf , t_tag.ui4_done_bytes, 128, ntfs_tc_async_nfy, (VOID*)&t_tag, &h_req);
        x_thread_delay(20);
        #endif

        if (i4_ret == FMR_OK)
        {
            if (x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
            {
                x_mem_free(buf);
                x_fm_close(h_src_file);
                x_fm_close(h_dest_file);
                return NTFS_TC_ERR;
            }
            /*x_dbg_stmt("Lock2!\n");*/
        }
        if (x_sema_unlock(t_tag.h_rw_lock))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return NTFS_TC_ERR;
        }
        /*x_dbg_stmt("Unlock2!\n");*/
        if (t_tag.b_eof)
        {
            i4_ret = FMR_EOF;
        }

        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_mem_free(buf);
                buf = NULL;*/
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_fm_abort_async(h_dest_file);
                x_mem_free(buf);
                buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Async Write Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }
        }
        #endif
    }
    if (buf != NULL)
    {
        x_mem_free(buf);
        buf = NULL;
    }
    x_thread_delay(1000);
    return NTFS_TC_OK;
}

/* 15. Seek File */
static INT32 ntfs_tc_fseek(INT32 i4_argc, const CHAR** ps_argv)
{
    /* Test in read or write file */
    return NTFS_TC_OK;
}

/* 16. Get Directory Info */
static INT32 ntfs_tc_get_dir_info(INT32 i4_argc, const CHAR** ps_argv)
{
    FM_DIR_INFO_T t_dir_info;
    HANDLE_T h_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    int i = 1;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_get_dinfo  [dir path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    x_dbg_stmt("The directory %s info:\n", ps_path);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_path, &h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }

    while(i4_cnt-- > 0)
    {
        x_dbg_stmt("The %d time test\n", i++);
        i4_ret = x_fm_get_dir_info(NULL_HANDLE, ps_path, &t_dir_info);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Get directory info fail: %d\n", i4_ret);
            return NTFS_TC_ERR;
        }

        x_dbg_stmt("Dir number:%d\n", t_dir_info.ui4_dir_num);
        x_dbg_stmt("File number:%d\n", t_dir_info.ui4_file_num);
    }

    i4_ret = x_fm_close(h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    
    return NTFS_TC_OK;
}

/* 17. Read Directory */
static INT32 ntfs_tc_read_dir(INT32 i4_argc, const CHAR** ps_argv)
{
    FM_DIR_INFO_T t_dir_info;
    HANDLE_T h_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    int i = 1;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_dir  [dir path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    x_dbg_stmt("The directory %s info:\n", ps_path);

    i4_ret = x_fm_get_dir_info(NULL_HANDLE, ps_path, &t_dir_info);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Get directory info fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    x_dbg_stmt("Dir number:%d\n", t_dir_info.ui4_dir_num);
    x_dbg_stmt("File number:%d\n", t_dir_info.ui4_file_num);

    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_path, &h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    while(i4_cnt-- > 0)
    {
        x_dbg_stmt("The %d time test\n", i++);
        while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
        x_fm_seek_dir(h_dir, FM_SEEK_BGN, 0);
    }

    i4_ret = x_fm_close(h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    return NTFS_TC_OK;
}

/* 18. Seek Directory */
static INT32 ntfs_tc_dseek(INT32 i4_argc, const CHAR** ps_argv)
{
    FM_DIR_INFO_T  t_dir_info;
    FM_DIR_ENTRY_T at_dir_entry[2];
    UINT32 ui4_total_ent_num;
    UINT32 ui4_entries;
    int i, j=1;
    CHAR *ps[2] = {"FM_SEEK_BGN", "FM_SEEK_CUR"};
    HANDLE_T h_dir;
    const CHAR *ps_path = NULL;
    INT32 i4_ret, i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_dseek  [dir path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_path, &h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    i4_ret = x_fm_get_dir_info(NULL_HANDLE, ps_path, &t_dir_info);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Get directory info fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    x_dbg_stmt("Dir number:%d\n", t_dir_info.ui4_dir_num);
    x_dbg_stmt("File number:%d\n", t_dir_info.ui4_file_num);

    ui4_total_ent_num = t_dir_info.ui4_dir_num + t_dir_info.ui4_file_num;
    x_dbg_stmt("Total entries num: %d\n", ui4_total_ent_num);

    while(i4_cnt-- > 0)
    {
        x_dbg_stmt("The %d time test\n", j++);
        for (i = 0; i < 2; i++)
        {
            x_dbg_stmt("Seek dir, whence: %s, ent_off: -1\n", ps[i]);
            x_fm_seek_dir(h_dir, i+1, -1);
            while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_entries) == FMR_OK)
            {       
                x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
            }
            x_dbg_stmt("Seek dir, whence: %s, ent_off: 0\n", ps[i]);
            x_fm_seek_dir(h_dir, i+1, 0);
            while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_entries) == FMR_OK)
            {       
                x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
            }
            x_dbg_stmt("Seek dir, whence: %s, ent_off: 1\n", ps[i]);
            x_fm_seek_dir(h_dir, i+1, 1);
            while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_entries) == FMR_OK)
            {       
                x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
            }
            x_dbg_stmt("Seek dir, whence: %s, ent_off: 2\n", ps[i]);
            x_fm_seek_dir(h_dir, i+1, 2);
            while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_entries) == FMR_OK)
            {       
                x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
            }
            x_dbg_stmt("Seek dir, whence: %s, ent_off: %d\n", ps[i], ui4_total_ent_num - 2);
            x_fm_seek_dir(h_dir, i+1, ui4_total_ent_num - 2);
            while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_entries) == FMR_OK)
            {       
                x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
            }
            x_dbg_stmt("Seek dir, whence: %s, ent_off: %d\n", ps[i], ui4_total_ent_num - 1);
            x_fm_seek_dir(h_dir, i+1, ui4_total_ent_num - 1);
            while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_entries) == FMR_OK)
            {       
                x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
            }
            x_dbg_stmt("Seek dir, whence: %s, ent_off: %d\n", ps[i], ui4_total_ent_num);
            x_fm_seek_dir(h_dir, i+1, ui4_total_ent_num);
            while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_entries) == FMR_OK)
            {       
                x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
            }
            x_fm_seek_dir(h_dir, FM_SEEK_BGN, 0);
        }
    }

    i4_ret = x_fm_close(h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close directory fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    return NTFS_TC_OK;
}

/* 19. Get File Info */
static INT32 ntfs_tc_get_finfo(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file;
    FM_FILE_INFO_T  t_src_file_info;
    int i = 1;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_get_finfo  [file path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_WRITE | FM_OPEN_CREATE, 0666, FALSE , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open and create src file fail: %d\n", i4_ret);

        return NTFS_TC_ERR;
    }

    /* 4.Get file info */
    while(i4_cnt -- > 0)
    {
        x_dbg_stmt("The %d time test\n", i++);
        if (i4_cnt % 2 == 0)
        {
            i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
            if ( i4_ret != FMR_OK)
            {
                 x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
                 x_fm_close(h_file);
                 return NTFS_TC_ERR;
            }
        }
        else
        {  
            i4_ret = x_fm_get_info_by_handle(h_file, &t_src_file_info);
            if ( i4_ret != FMR_OK)
            {
                 x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
                 x_fm_close(h_file);
                 return NTFS_TC_ERR;
            }
        }
        x_dbg_stmt("%s size is %lld bytes\n",  ps_path, t_src_file_info.ui8_size);
        x_dbg_stmt("%s blk size is %d \n",  ps_path, t_src_file_info.ui4_blk_size);
        x_dbg_stmt("%s blk cnt is %d \n",  ps_path, t_src_file_info.ui8_blk_cnt);
        x_dbg_stmt("%s create time is %lld\n",  ps_path, t_src_file_info.ui8_create_time);
        x_dbg_stmt("%s last access time is %lld \n",  ps_path, t_src_file_info.ui8_access_time);
        x_dbg_stmt("%s last modify time is %lld \n",  ps_path, t_src_file_info.ui8_modify_time);
    }
    
    i4_ret = x_fm_close(h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close file fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    return NTFS_TC_OK;
}

/* 20. Set File Info */
static INT32 ntfs_tc_set_finfo(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file;
    FM_FILE_INFO_T  t_src_file_info;
    TIME_T   t_access_time, t_modify_time;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_set_finfo  [file path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_WRITE | FM_OPEN_CREATE, 0666, FALSE , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open and create src file fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    /* 4.Get file info */
    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
         x_fm_close(h_file);
         return NTFS_TC_ERR;
    }
    x_dbg_stmt("%s size is %d bytes\n",  ps_path, t_src_file_info.ui8_size);
    x_dbg_stmt("%s blk size is %d \n",  ps_path, t_src_file_info.ui4_blk_size);
    x_dbg_stmt("%s blk cnt is %d \n",  ps_path, t_src_file_info.ui8_blk_cnt);
    x_dbg_stmt("%s create time is %lld\n",  ps_path, t_src_file_info.ui8_create_time);
    x_dbg_stmt("%s last access time is %lld \n",  ps_path, t_src_file_info.ui8_access_time);
    x_dbg_stmt("%s last modify time is %lld \n",  ps_path, t_src_file_info.ui8_modify_time);

    t_access_time = t_src_file_info.ui8_access_time + 1000;
    t_modify_time = t_src_file_info.ui8_modify_time + 2000;

    i4_ret = x_fm_set_time_by_name(FM_ROOT_HANDLE, ps_path, t_access_time, t_modify_time);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Set src file info fail: %d\n", i4_ret);
        x_fm_close(h_file);
        return NTFS_TC_ERR;
    }

    #if 0
    if (i4_cnt % 2 == 0)
    {
        i4_ret = x_fm_trunc_by_name(FM_ROOT_HANDLE, ps_path, t_src_file_info.ui8_size);
        if ( i4_ret != FMR_OK)
        {
            x_dbg_stmt("Set src file info fail: %d\n", i4_ret);
            x_fm_close(h_file);
            return NTFS_TC_ERR;
        }
    }
    else
    {
        i4_ret = x_fm_trunc_by_handle(h_file, t_src_file_info.ui8_size);
        if ( i4_ret != FMR_OK)
        {
            x_dbg_stmt("Set src file info fail: %d\n", i4_ret);
            x_fm_close(h_file);
            return NTFS_TC_ERR;
        }
        
    }
    #endif

    i4_ret = x_fm_get_info_by_handle(h_file, &t_src_file_info);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Get src file info fail: %d\n", i4_ret);
        x_fm_close(h_file);
        return NTFS_TC_ERR;
    }
    x_dbg_stmt("%s size is %d bytes\n",  ps_path, t_src_file_info.ui8_size);
    x_dbg_stmt("%s blk size is %d \n",   ps_path, t_src_file_info.ui4_blk_size);
    x_dbg_stmt("%s blk cnt is %d \n",    ps_path, t_src_file_info.ui8_blk_cnt);
    x_dbg_stmt("%s create time is %lld\n",  ps_path, t_src_file_info.ui8_create_time);
    x_dbg_stmt("%s last access time is %lld \n",  ps_path, t_src_file_info.ui8_access_time);
    x_dbg_stmt("%s last modify time is %lld \n",  ps_path, t_src_file_info.ui8_modify_time);


    i4_ret = x_fm_close(h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close file fail: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
  
    return NTFS_TC_OK;
}

/* 21. Mount */
static INT32 ntfs_tc_mount(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    const CHAR *ps_dev_no, *ps_mp;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_mount  [dev node] [mount point]");
        return NTFS_TC_ERR;
    }
    ps_dev_no = ps_argv[1];
    ps_mp = ps_argv[2];
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_mp, 0777);
    if( (i4_ret != FMR_OK) && (i4_ret != FMR_EXIST))
    {
        x_dbg_stmt("Make dir %s Fail!: %d\n",ps_mp, i4_ret);
        return NTFS_TC_ERR;
    }
    
    i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_dev_no, FM_ROOT_HANDLE, ps_mp);
    if( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Mount %s fail: %d\n", ps_dev_no, i4_ret);
        return NTFS_TC_ERR;
    }

    x_dbg_stmt("Mount %s on %s\n", ps_dev_no, ps_mp);

    return NTFS_TC_OK;
}

/* 22. Unmount */
static INT32 ntfs_tc_umount(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    const CHAR  *ps_mp;

    if( i4_argc < 2)
    {
        x_dbg_stmt("ntfs_umount [mount point]");
        return NTFS_TC_ERR;
    }
    ps_mp = ps_argv[1];

    i4_ret = x_fm_umount(FM_ROOT_HANDLE, ps_mp);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Unmount %s fail!\n", ps_mp);
    }
    x_dbg_stmt("Unmount %s on /mnt/usb\n", ps_mp);

    return NTFS_TC_OK;
}

static INT32 ntfs_tc_cycle_frename(INT32 i4_argc, const CHAR** ps_argv)
{
    /* ~~~ -> Name1 -> Name2 -> Name3 -> Name1 -> ~~~ */
    INT32 i4_cnt;
    const CHAR *ps_argv2[3];
    const CHAR *ps_rn_file1 = NULL;
    const CHAR *ps_rn_file2 = NULL;
    const CHAR *ps_rn_file3 = NULL;

    if( i4_argc < 5)
    {
        x_dbg_stmt("ntfs_cycle_frename [Name1] [Name2] [Name3] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_rn_file1 = ps_argv[1];
    ps_rn_file2 = ps_argv[2];
    ps_rn_file3 = ps_argv[3];
    StringToNumber(ps_argv[4], &i4_cnt);

    while (i4_cnt-- > 0)
    {
        ps_argv2[1] = ps_rn_file1;
        ps_argv2[2] = ps_rn_file2;
        ntfs_tc_frename(3, ps_argv2);

        ps_argv2[1] = ps_rn_file2;
        ps_argv2[2] = ps_rn_file3;
        ntfs_tc_frename(3, ps_argv2);

        ps_argv2[1] = ps_rn_file3;
        ps_argv2[2] = ps_rn_file1;
        ntfs_tc_frename(3, ps_argv2);
    }

    return NTFS_TC_OK;
}

static INT32 ntfs_tc_cycle_drename(INT32 i4_argc, const CHAR** ps_argv)
{
    /* ~~~ -> Name1 -> Name2 -> Name3 -> Name1 -> ~~~ */
    INT32 i4_cnt;
    const CHAR *ps_argv2[3];
    const CHAR *ps_rn_dir1 = NULL;
    const CHAR *ps_rn_dir2 = NULL;
    const CHAR *ps_rn_dir3 = NULL;

    if( i4_argc < 5)
    {
        x_dbg_stmt("ntfs_cycle_drename [Name1] [Name2] [Name3] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_rn_dir1 = ps_argv[1];
    ps_rn_dir2 = ps_argv[2];
    ps_rn_dir3 = ps_argv[3];
    StringToNumber(ps_argv[4], &i4_cnt);

    while (i4_cnt-- > 0)
    {
        ps_argv2[1] = ps_rn_dir1;
        ps_argv2[2] = ps_rn_dir2;
        ntfs_tc_drename(3, ps_argv2);

        ps_argv2[1] = ps_rn_dir2;
        ps_argv2[2] = ps_rn_dir3;
        ntfs_tc_drename(3, ps_argv2);

        ps_argv2[1] = ps_rn_dir3;
        ps_argv2[2] = ps_rn_dir1;
        ntfs_tc_drename(3, ps_argv2);
    }

    return NTFS_TC_OK;
}

static INT32 ntfs_tc_cre_del_file(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL;
    HANDLE_T h_dir = NULL_HANDLE;
    HANDLE_T h_file = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    int i = 1;
    INT32 i4_cnt;
    const CHAR  *ps_path;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_cre_del_file [file path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }

    while(i4_cnt-- > 0)
    {
        x_dbg_stmt("The %d time test\n", i++);
        /* 3.Open an not existed file */
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_WRITE | FM_OPEN_CREATE, 0666, FALSE , &h_file);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Open and create src file fail: %d\n", i4_ret);
            x_fm_close(h_dir);
            x_mem_free(ps_prnt_dir);
            return NTFS_TC_ERR;
        }
        
        /* 4.Read parent dir again */
        x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir); 
        x_fm_seek_dir(h_dir, FM_SEEK_BGN, 0);
        while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }

        i4_ret = x_fm_close(h_file);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Close src file fail: %d\n", i4_ret);
            x_fm_close(h_dir);
            x_mem_free(ps_prnt_dir);
            return NTFS_TC_ERR;
        }

        /* 5.Delete file */
        i4_ret = x_fm_delete_file(FM_ROOT_HANDLE, ps_path);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Delete src file fail: %d\n", i4_ret);
            x_fm_close(h_dir);
            x_mem_free(ps_prnt_dir);
            return NTFS_TC_ERR;
        }
            
        /* 6.Read parent dir again */
        x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir); 
        x_fm_seek_dir(h_dir, FM_SEEK_BGN, 0);
        while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
    }

    i4_ret = x_fm_close(h_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }
    
    x_mem_free(ps_prnt_dir);
    return NTFS_TC_OK; 

}

static INT32 ntfs_tc_cre_del_dir(INT32 i4_argc, const CHAR** ps_argv)
{
    CHAR *ps_prnt_dir = NULL;
    HANDLE_T h_prnt_dir = NULL_HANDLE;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    INT32 i4_ret;
    int i = 1;
    INT32 i4_cnt;
    const CHAR  *ps_path;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_cre_del_file [file path] [execution count]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    
    /* 1.Get the parent directory info */
    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_path);

    /* 2.Get prnt dir info */
    x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir);  
    i4_ret = x_fm_open_dir(NULL_HANDLE, ps_prnt_dir, &h_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }
    while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
    }

    while(i4_cnt-- > 0)
    {
        x_dbg_stmt("The %d time test\n", i++);
        /* 3.Create dir */
        i4_ret = x_fm_create_dir(NULL_HANDLE, ps_path, FM_ACCESS_MODE);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Create directory fail: %d\n", i4_ret);
            x_fm_close(h_prnt_dir);
            x_mem_free(ps_prnt_dir);
            return NTFS_TC_ERR;
        }

        /* 4.Read parent dir again */
        x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir); 
        x_fm_seek_dir(h_prnt_dir, FM_SEEK_BGN, 0);
        while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }

        /* 5.Delelte dir */
        i4_ret = x_fm_delete_dir(NULL_HANDLE, ps_path);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("Delete directory fail: %d\n", i4_ret);
            x_mem_free(ps_prnt_dir);
            return NTFS_TC_ERR;
        }

        /* 6.Read parent dir again */
        x_dbg_stmt("The parent directory %s info:\n", ps_prnt_dir); 
        x_fm_seek_dir(h_prnt_dir, FM_SEEK_BGN, 0);
        while (x_fm_read_dir_entries(h_prnt_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
        {       
            x_dbg_stmt("%s\n", at_dir_entry[0].s_name);    
        }
    }

    i4_ret = x_fm_close(h_prnt_dir);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Close parent directory fail: %d\n", i4_ret);
        x_mem_free(ps_prnt_dir);
        return NTFS_TC_ERR;
    }

    x_mem_free(ps_prnt_dir);
    return NTFS_TC_OK;
}

/* Read data from file1, and then write to file2 */
/* This api is used to check the previous written file2 is same as file1 */
static INT32 ntfs_tc_compare_file(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file1, h_file2;
    BOOL     b_eof = FALSE;
    UINT8    *buf1 = NULL,*buf2 = NULL;
    UINT32 ui4_done_bytes;
    UINT64 ui8_match = 1;
    const CHAR *ps_path1 = NULL, *ps_path2 = NULL;
    UINT32 ui4_diff_cnt, ui4_total_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("ntfs_cmp_file  [src file path] [dest file path]");
        return NTFS_TC_ERR;
    }
    ps_path1 = ps_argv[1];
    ps_path2 = ps_argv[2];

    ui4_diff_cnt = ui4_total_cnt = 0;

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path1, FM_READ_ONLY, 0666, FALSE , &h_file1);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_path1, i4_ret);
         return NTFS_TC_ERR;
    }
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path2, FM_READ_ONLY, 0666, FALSE , &h_file2);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_path2, i4_ret);
         return NTFS_TC_ERR;
    } 
    
    b_eof = FALSE;
    do        
    {
        x_thread_delay(1);
        buf1 = (UINT8*) x_mem_calloc(1, 64* 1024 + 32); 
    } while( buf1 == NULL);
    do        
    {
        x_thread_delay(1);
        buf2 = (UINT8*) x_mem_calloc(1, 64* 1024 + 32); 
    } while( buf2 == NULL);
    
    while (!b_eof)
    {
        x_memset(buf1, 0, 64* 1024 + 32);
        x_memset(buf2, 0, 64* 1024 + 32);
        i4_ret = x_fm_read(h_file1,(VOID *)buf1 , 64*1024, &ui4_done_bytes);

        switch (i4_ret)
        {
            case FMR_OK:
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_fm_abort_async(h_file1);
                x_mem_free(buf1);
                buf1 = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf1);
                buf1 = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf1);
                buf1 = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_mem_free(buf2);
            
            x_fm_close(h_file1);
            x_fm_close(h_file2);

            if (i4_ret == FMR_EOF)
            {
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }
        }

        i4_ret = x_fm_read(h_file2,(VOID *)buf2 , 64*1024, &ui4_done_bytes);

        switch (i4_ret)
        {
            case FMR_OK:
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_fm_abort_async(h_file2);
                x_mem_free(buf2);
                buf1 = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf2);
                buf1 = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf2);
                buf1 = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_mem_free(buf1);
            
            x_fm_close(h_file1);
            x_fm_close(h_file2);

            if (i4_ret == FMR_EOF)
            {
                x_dbg_stmt("Total compare %d times\n", ui4_total_cnt);
                x_dbg_stmt("Match %d times\n", ui8_match);
                x_dbg_stmt("Diff %d times\n", ui8_match);
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }
        }
        ui4_total_cnt++;
        /* Compare buf */
        if (x_memcmp(buf1, buf2, 64*1024) != 0)
        {
            ui4_diff_cnt++;
            x_dbg_stmt("Find difference, some errors have occured in previous write operation\n");
            x_thread_delay(5000);
        }
        else
        {
            x_dbg_stmt("Perfect match! The %lld time\n", ui8_match++);
        }
    }

    return NTFS_TC_OK;
}


/*********************************************************************
 *                                                                   *
 *                 NTFS Multi Thread Stress Test                     *
 *                                                                   *
 *********************************************************************/
static VOID ntfs_task_1(VOID* s_file_path)
{
    HANDLE_T    h_src_file;
    INT32       i4_cnt = 0;

    while(i4_cnt++ < NTFS_STRESS_TEST_TIMES1)
    {
        x_dbg_stmt("T1:Open %s, then sleep 10 seconds, then close it\n",  (CHAR*)s_file_path);
        x_fm_open(FM_ROOT_HANDLE, (CHAR*)s_file_path, FM_READ_ONLY, 0666, FALSE, &h_src_file);
        x_thread_delay(10000);
        x_fm_close(h_src_file);
    }
}

static VOID ntfs_task_2(VOID* s_file_path)
{
    INT32   i4_ret;
    INT32   i4_cnt = 0;

    while(i4_cnt++ < NTFS_STRESS_TEST_TIMES1)
    {
        x_dbg_stmt("T2:Delete %s in 3 seconds\n",  (CHAR*)s_file_path);
        x_thread_delay(3000);
        i4_ret = x_fm_delete_file(FM_ROOT_HANDLE,  (CHAR*)s_file_path);
        x_dbg_stmt("T2:Delete() returns: %d\n", i4_ret);
    }
}

static VOID ntfs_task_3(VOID* s_file_path)
{
    HANDLE_T h_src_file;
    INT32    i4_ret;
    INT32    i4_cnt = 0;

    while(i4_cnt++ < NTFS_STRESS_TEST_TIMES1)
    {
        x_dbg_stmt("T3:Open(CREATE) %s in 5 seconds\n", (CHAR*)s_file_path);
        x_thread_delay(5000);
        i4_ret = x_fm_open(FM_ROOT_HANDLE,  (CHAR*)s_file_path, FM_READ_WRITE | FM_OPEN_CREATE, 0666, FALSE, &h_src_file);
        x_dbg_stmt("T3:Open(CREATE) returns %d\n", i4_ret);
    }
}
    
static INT32 ntfs_tc_mt1(INT32 i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_task_1, h_task_2, h_task_3;
    INT32 i4_ret;
    const CHAR  *ps_path;

    if( i4_argc < 2)
    {
        x_dbg_stmt("ntfs_mt1 [file_path]");
        return NTFS_TC_ERR;
    }
    ps_path = ps_argv[1];
    x_dbg_stmt("File: %s\n", ps_path);
    
    i4_ret = x_thread_create(&h_task_1,
                             "task1",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             ntfs_task_1,
                             sizeof(CHAR) * x_strlen(ps_path),
                             (VOID*)ps_path);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread1 error: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    i4_ret = x_thread_create(&h_task_2,
                             "task2",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             ntfs_task_2,
                             sizeof(CHAR) * x_strlen(ps_path),
                             (VOID*)ps_path);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread2 error: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    i4_ret = x_thread_create(&h_task_3,
                             "task3",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             ntfs_task_3,
                             sizeof(CHAR) * x_strlen(ps_path),
                             (VOID*)ps_path);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread3 error: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    
    x_thread_delay(2000);
    
    return NTFS_TC_OK;
}

static VOID ntfs_task_4(VOID* pv_tag)
{
    INT32 i = 1;
    const CHAR* ps_argv[3];
    CHAR *ps_prnt_dir = NULL;
    #if 1
    CHAR *ps_src =       "/mnt/usb/sda/kkkooo/Dust_to_Glory_640.mp4";
    #else
    CHAR *ps_src =       "/mnt/usb/Mass-000/ntfs_test/music/Test2.mp3";
    #endif
    CHAR *ps_del_file =  "/mnt/usb/sda/kkkooo/todel1.txt";
    CHAR *ps_del_dir =   "/mnt/usb/sda/kkkooo/todel1";
    CHAR *ps_rn_file1 =  "/mnt/usb/sda/kkkooo/d1/rn1.txt";
    CHAR *ps_rn_file2 =  "/mnt/usb/sda/kkkooo/d1/rn2.txt";
    CHAR *ps_rn_file3 =  "/mnt/usb/sda/kkkooo/d2/rn3.txt";
    CHAR *ps_rn_dir1 =   "/mnt/usb/sda/kkkooo/d1/rn1";
    CHAR *ps_rn_dir2 =   "/mnt/usb/sda/kkkooo/d1/rn2";
    CHAR *ps_rn_dir3 =   "/mnt/usb/sda/kkkooo/d2/rn3";
    
    while (i++ < NTFS_STRESS_TEST_TIMES1)
    {
        ps_argv[1] = ps_rn_file1;
        ps_argv[2] = ps_rn_file2;
        ntfs_tc_frename(3, ps_argv);

        ps_argv[1] = ps_rn_file2;
        ps_argv[2] = ps_rn_file3;
        ntfs_tc_frename(3, ps_argv);

        ps_argv[1] = ps_rn_file3;
        ps_argv[2] = ps_rn_file1;
        ntfs_tc_frename(3, ps_argv);

        ps_argv[1] = ps_rn_dir1;
        ps_argv[2] = ps_rn_dir2;
        ntfs_tc_drename(3, ps_argv);

        ps_argv[1] = ps_rn_dir2;
        ps_argv[2] = ps_rn_dir3;
        ntfs_tc_drename(3, ps_argv);

        ps_argv[1] = ps_rn_dir3;
        ps_argv[2] = ps_rn_dir1;
        ntfs_tc_drename(3, ps_argv);
    }

    ps_argv[1] = ps_del_file;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_cre_del_file(3, ps_argv); 
    x_thread_delay(100);
    ps_argv[1] = ps_del_dir;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_cre_del_dir(3, ps_argv);
    x_thread_delay(200);

    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_src);

    ps_argv[1] = ps_src;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_fopen(3, ps_argv);
    x_thread_delay(300);
    ntfs_tc_fread_sync(3, ps_argv);
    x_thread_delay(400);
    ntfs_tc_fread_async(3, ps_argv);
    x_thread_delay(500);
    ntfs_tc_get_finfo(3, ps_argv);
    x_thread_delay(600);
    ntfs_tc_set_finfo(3, ps_argv);
    x_thread_delay(700);

    ps_argv[1] = ps_prnt_dir;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_dopen(3, ps_argv);
    x_thread_delay(800);
    ntfs_tc_get_dir_info(3, ps_argv);
    x_thread_delay(900);
    ntfs_tc_read_dir(3, ps_argv);
    x_thread_delay(1000);
    ntfs_tc_dseek(3, ps_argv);

    x_mem_free(ps_prnt_dir);

    x_dbg_stmt("Task4 finished\n");
}

static VOID ntfs_task_5(VOID* pv_tag)
{
    INT32 i = 1;
    const CHAR* ps_argv[3];
    CHAR *ps_prnt_dir = NULL;
#if 1
    CHAR *ps_src =       "/mnt/usb/sda/kkkooo/Dust_to_Glory_640.mp4";
#else
    CHAR *ps_src =       "/mnt/usb/Mass-000/ntfs_test/music/Test2.mp3";
#endif
    CHAR *ps_del_file =  "/mnt/usb/sda/kkkooo/todel2.txt";
    CHAR *ps_del_dir =   "/mnt/usb/sda/kkkooo/todel2";
    CHAR *ps_rn_file1 =  "/mnt/usb/sda/kkkooo/d3/rn1.txt";
    CHAR *ps_rn_file2 =  "/mnt/usb/sda/kkkooo/d3/rn2.txt";
    CHAR *ps_rn_file3 =  "/mnt/usb/sda/kkkooo/d4/rn3.txt";
    CHAR *ps_rn_dir1 =   "/mnt/usb/sda/kkkooo/d3/rn1";
    CHAR *ps_rn_dir2 =   "/mnt/usb/sda/kkkooo/d3/rn2";
    CHAR *ps_rn_dir3 =   "/mnt/usb/sda/kkkooo/d4/rn3";

    
    while (i++ < NTFS_STRESS_TEST_TIMES1)
    {
        ps_argv[1] = ps_rn_file1;
        ps_argv[2] = ps_rn_file2;
        ntfs_tc_frename(3, ps_argv);

        ps_argv[1] = ps_rn_file2;
        ps_argv[2] = ps_rn_file3;
        ntfs_tc_frename(3, ps_argv);

        ps_argv[1] = ps_rn_file3;
        ps_argv[2] = ps_rn_file1;
        ntfs_tc_frename(3, ps_argv);

        ps_argv[1] = ps_rn_dir1;
        ps_argv[2] = ps_rn_dir2;
        ntfs_tc_drename(3, ps_argv);

        ps_argv[1] = ps_rn_dir2;
        ps_argv[2] = ps_rn_dir3;
        ntfs_tc_drename(3, ps_argv);

        ps_argv[1] = ps_rn_dir3;
        ps_argv[2] = ps_rn_dir1;
        ntfs_tc_drename(3, ps_argv);
    }

    ps_argv[1] = ps_del_file;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_cre_del_file(3, ps_argv); 
    x_thread_delay(100);
    ps_argv[1] = ps_del_dir;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_cre_del_dir(3, ps_argv);
    x_thread_delay(200);

    ps_prnt_dir = _ntfs_tc_get_prnt_comp(ps_src);

    ps_argv[1] = ps_src;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_fopen(3, ps_argv);
    x_thread_delay(100);
    ntfs_tc_fread_sync(3, ps_argv);
    x_thread_delay(200);
    ntfs_tc_fread_async(3, ps_argv);
    x_thread_delay(100);
    ntfs_tc_get_finfo(3, ps_argv);
    x_thread_delay(200);
    ntfs_tc_set_finfo(3, ps_argv);
    x_thread_delay(100);

    ps_argv[1] = ps_prnt_dir;
    ps_argv[2] = NTFS_STRESS_TEST_TIMES2;
    ntfs_tc_dopen(3, ps_argv);
    x_thread_delay(200);
    ntfs_tc_get_dir_info(3, ps_argv);
    x_thread_delay(100);
    ntfs_tc_read_dir(3, ps_argv);
    x_thread_delay(200);
    ntfs_tc_dseek(3, ps_argv);

    x_mem_free(ps_prnt_dir);

    x_dbg_stmt("Task5 finished\n");
}

static INT32 ntfs_tc_mt2(INT32 i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_task_4, h_task_5;
    INT32 i4_ret;
    
    i4_ret = x_thread_create(&h_task_4,
                             "task4",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             ntfs_task_4,
                             0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread4 error: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    i4_ret = x_thread_create(&h_task_5,
                             "task5",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             ntfs_task_5,
                             0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread5 error: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    
    x_thread_delay(2000);
    
    return NTFS_TC_OK;
}

static VOID ntfs_task_6(VOID* pv_tag)
{
    INT32 i4_cnt = 1;
    const CHAR* ps_argv[3];  
    #if 1
        CHAR *ps_src =       "/mnt/usb/sda/kkkooo/Dust_to_Glory_640.mp4";
        CHAR *ps_dest =      "/mnt/usb/sda/kkkooo/Dust_to_Glory_640_w1.mp4";
    #else
        CHAR *ps_src =  "/mnt/usb/Mass-000/ntfs_test/music/Test2.mp3";
        CHAR *ps_dest = "/mnt/usb/Mass-000/ntfs_test/music/Test2w1.mp3";
    #endif

    ps_argv[1] = ps_src;
    ps_argv[2] = ps_dest;

    while(i4_cnt ++ < NTFS_STRESS_TEST_TIMES1)
    {
        x_dbg_stmt("The %d time test\n", i4_cnt);

        if (i4_cnt % 2 == 0)
        {
            ntfs_tc_fwrite_sync(3, ps_argv);
        }
        else
        {
            ntfs_tc_fwrite_async(3, ps_argv);
        }
        /* we don't delete the dest file in last time for a future comparation */
        if (i4_cnt < NTFS_STRESS_TEST_TIMES1)
        {
            x_fm_delete_file(FM_ROOT_HANDLE, ps_dest);
        }
    }

    x_dbg_stmt("Task6 finished\n");
}

static VOID ntfs_task_7(VOID* pv_tag)
{
    INT32 i4_cnt = 1;
    const CHAR* ps_argv[3];
    #if 1
        CHAR *ps_src =       "/mnt/usb/sda/kkkooo/Dust_to_Glory_640.mp4";
        CHAR *ps_dest =      "/mnt/usb/sda/kkkooo/Dust_to_Glory_640_w2.mp4";
    #else
            CHAR *ps_src =  "/mnt/usb/Mass-000/ntfs_test/music/Test2.mp3";
            CHAR *ps_dest = "/mnt/usb/Mass-000/ntfs_test/music/Test2w2.mp3";
    #endif

    ps_argv[1] = ps_src;
    ps_argv[2] = ps_dest;

    while(i4_cnt ++ < NTFS_STRESS_TEST_TIMES1)
    {
        x_dbg_stmt("The %d time test\n", i4_cnt);

        if (i4_cnt % 2 == 0)
        {
            ntfs_tc_fwrite_sync(3, ps_argv);
        }
        else
        {
            ntfs_tc_fwrite_async(3, ps_argv);
        }
        /* we don't delete the dest file in last time for a future comparation */
        if (i4_cnt < NTFS_STRESS_TEST_TIMES1)
        {
            x_fm_delete_file(FM_ROOT_HANDLE, ps_dest);
        }
    }

    x_dbg_stmt("Task7 finished\n");
}

static INT32 ntfs_tc_mt3(INT32 i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_task_6, h_task_7;
    INT32 i4_ret;
    
    i4_ret = x_thread_create(&h_task_6,
                             "task6",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             ntfs_task_6,
                             0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread6 error: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }

    i4_ret = x_thread_create(&h_task_7,
                             "task7",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             ntfs_task_7,
                             0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread7 error: %d\n", i4_ret);
        return NTFS_TC_ERR;
    }
    
    x_thread_delay(2000);
    
    return NTFS_TC_OK;
}

static INT32 ntfs_tc_mt_all(INT32 i4_argc, const CHAR** ps_argv)
{
    const CHAR* ps_argv_local[2];

    ps_argv_local[1] = "/mnt/usb/sda/kkkooo/cre_del.txt";

    ntfs_tc_mt1(2, ps_argv_local);

    ntfs_tc_mt2(1 ,NULL);
    
    ntfs_tc_mt3(1 ,NULL);

    return NTFS_TC_OK;
}

#endif /*_NTFS_TEST_*/
#endif /* CLI_LVL_ALL */

#ifdef CLI_LVL_ALL
static VOID _mfw_async_rw_fct(
        HANDLE_T            h_req,
        VOID*               pv_tag,
        FM_ASYNC_COND_T     e_async_cond,
        UINT32              ui4_data)
{
    HANDLE_T h_sema;

    
    if (pv_tag != NULL)
    {
        h_sema = *(HANDLE_T *)pv_tag;
        
        if( e_async_cond == FM_ASYNC_COND_READ_OK)
        {
            x_dbg_stmt("Read OK(h_req: %lu, read %lu bytes)\n", h_req, ui4_data);
        }
        else if( e_async_cond == FM_ASYNC_COND_WRITE_OK)
        {
            /* x_dbg_stmt("Success!h_req: %d, pv_tag: %x, ui4_data: %d, total: %d\n", h_req, (UINT32)pv_tag, ui4_data, ui4_total_rcnt);*/
            x_dbg_stmt("Write OK(h_req: %lu, write %lu bytes)\n", h_req, ui4_data);
        }
        else if (e_async_cond == FM_ASYNC_COND_ABORT_OK)
        {
            x_dbg_stmt("Abort OK!h_req: %d\n", h_req);
        }
        else
        {
            x_dbg_stmt("FM_ASYNC_RW_FAIL!\n");
            ASSERT(0);
        }
        /*
        x_dbg_stmt("buf: %x,%x,%x,%x\n", *(UINT8*)pv_tag, *((UINT8*)pv_tag+1), *((UINT8*)pv_tag+2), *((UINT8*)pv_tag+3));
        */
        x_sema_unlock(h_sema);
    }
}



static VOID _mfw_async_anls_cret_fct(
        HANDLE_T            h_req,
        VOID*               pv_tag,
        FM_MFW_ASYNC_COND_T e_async_cond,
        UINT32              ui4_data)
{

    switch(e_async_cond)
    {
        case FM_MFW_ASYNC_COND_CREATE_FAIL:
            {
                x_dbg_stmt("create request %lu failed, errno:%lu\n", h_req, ui4_data);
            }
            break;
        case FM_MFW_ASYNC_COND_ANLS_FAIL:
            {
                x_dbg_stmt("analyse request %lu failed, errno:%lu\n", h_req, ui4_data);
            }
            break;
        case FM_MFW_ASYNC_COND_ABORT_FAIL:
            {
                x_dbg_stmt("abort request %lu failed, errno:%lu\n", h_req, ui4_data);
            }
            break;
        case FM_MFW_ASYNC_COND_ABORT_OK:
            {
                x_dbg_stmt("abort request %lu OK, errno:%lu\n", h_req, ui4_data);
            }
            break;
        case FM_MFW_ASYNC_COND_ANLS_OK:
            {
                FM_MFW_ANLS_RESULT_T* pt_res;

                pt_res = (FM_MFW_ANLS_RESULT_T*)ui4_data;
                ASSERT(pt_res != NULL);
                x_dbg_stmt("analyse request %lu OK, continuous space:%llu, available space:%llu\n", h_req, pt_res->ui8_continuous_size, pt_res->ui8_avail_size);
                
            }
            break;
        case FM_MFW_ASYNC_COND_CREATE_OK:
            {
                x_dbg_stmt("create request %lu OK\n", h_req);
            }
            break;
        case FM_MFW_ASYNC_COND_PRGS:
            {
                FM_MFW_PRGS_T* pt_prgs;

                pt_prgs = (FM_MFW_PRGS_T*)ui4_data;
                ASSERT(pt_prgs != NULL);
                
                x_dbg_stmt("async request %lu, total phase:%d, cur phase:%d, progress:%lu \n", h_req, pt_prgs->ui1_total_phases, pt_prgs->ui1_cur_phase, pt_prgs->ui4_prgs);
            }
            break;
        case FM_MFW_ASYNC_COND_STATUS:
            {
                x_dbg_stmt("async request %lu status:%ul\n", h_req, ui4_data);
            }
            break;
        default:
            ASSERT(0);
    }
}


static INT32 mfw_anls(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    FM_MFW_ANLS_PARAM_T t_param;
    HANDLE_T h_req;
    
    if( i4_argc < 2)
    {
        x_dbg_stmt("anls [mount point path] \n");
        return NTFS_TC_ERR;
    }

    t_param.ui1_strct_ver = 0;
    t_param.s_mp_path = (CHAR*)pps_argv[1];
    t_param.pv_tag = NULL;
    t_param.pf_nfy = _mfw_async_anls_cret_fct;

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_analyse(&t_param, &h_req);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("anls %s failed\n", pps_argv[1]);
    }
    else
    {
        x_dbg_stmt("anls %s, request handle:%lu\n",pps_argv[1], h_req);
    }
    return i4_ret;
}

static INT32 mfw_cret(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    FM_MFW_VF_CFG_T t_cfg;
    HANDLE_T h_req;
    UINT32 ui4_n;
    
    if( i4_argc < 5)
    {
        x_dbg_stmt("mcret [1:Visible meta file, 0:Invisible meta file] [dir path] [meta file name(When meta file is visible)] [file name pattern] [Init size(n * 1MB)] \n");
        return NTFS_TC_ERR;
    }
    StringToNumber(pps_argv[1], (INT32*)&ui4_n);
    if (ui4_n == 0)
    {
        t_cfg.e_meta_policy = FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE;
    }
    else
    {
        t_cfg.e_meta_policy = FM_MFW_VF_META_POLICY_VISIBLE_META_FILE;
    }
    
    t_cfg.ui1_strct_ver = 0;
    t_cfg.s_dir= (CHAR*)pps_argv[2];
    if (t_cfg.e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE)
    {
        t_cfg.s_meta_file= (CHAR*)pps_argv[3];
        t_cfg.s_fn_pattern= (CHAR*)pps_argv[4];
        StringToNumber(pps_argv[5], (INT32*)&ui4_n);
    }
    else
    {
        t_cfg.s_meta_file= NULL;
        t_cfg.s_fn_pattern= (CHAR*)pps_argv[3];
        StringToNumber(pps_argv[4], (INT32*)&ui4_n);
    }
    t_cfg.e_vf_policy = FM_MFW_VF_POLICY_NO_CONSTRAINT;
    t_cfg.ui8_init_size = (UINT64)((UINT64)ui4_n * (UINT64)0x00100000);
    t_cfg.pv_tag = NULL;
    t_cfg.pf_nfy = _mfw_async_anls_cret_fct;

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_create(&t_cfg, &h_req);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("create %s failed\n", pps_argv[1]);
    }
    else
    {
        x_dbg_stmt("create %s, request handle:%lu\n",pps_argv[1], h_req);
    }
    return i4_ret;
}


static INT32 mfw_open(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file;
    
    if( i4_argc < 2)
    {
        x_dbg_stmt("mopen [file path] \n");
        return NTFS_TC_ERR;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pps_argv[1], FM_READ_WRITE, 0777, TRUE, &h_file );
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("open %s failed\n", pps_argv[1]);
    }
    else
    {
        x_dbg_stmt("open %s, file handle:%lu\n",pps_argv[1], h_file);
    }
    
    return i4_ret;
}


static INT32 mfw_close(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file;
    
    if( i4_argc < 2)
    {
        x_dbg_stmt("mclose [file handle] \n");
        return NTFS_TC_ERR;
    }
    StringToNumber(pps_argv[1], (INT32*)&h_file);

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_close( h_file );
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("close %lu failed\n", h_file);
    }
    else
    {
        x_dbg_stmt("close %lu OK\n",h_file);
    }
    
    return i4_ret;
}

static INT32 mfw_del(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    
    if( i4_argc < 2)
    {
        x_dbg_stmt("mdel [file path] \n");
        return NTFS_TC_ERR;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_delete(FM_ROOT_HANDLE, pps_argv[1]);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("delete %s failed\n", pps_argv[1]);
    }
    else
    {
        x_dbg_stmt("delete %s OK\n",pps_argv[1]);
    }
    
    return i4_ret;
}

static INT32 mfw_lseek(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file;
    INT32 i4_offset;
    INT32 i4_whence;
    UINT64 ui8_cur_pos;
    
    if( i4_argc < 4)
    {
        x_dbg_stmt("mseek [file handle] [offset] [whence] \n");
        return NTFS_TC_ERR;
    }
    i4_offset = 0;
    StringToNumber(pps_argv[1], (INT32*)&h_file);
    StringToNumber(pps_argv[2], (INT32*)&i4_offset);
    StringToNumber(pps_argv[3], (INT32*)&i4_whence);

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_lseek( h_file, (INT64)i4_offset, (UINT8)i4_whence, &ui8_cur_pos);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("lseek %lu failed\n", h_file);
    }
    else
    {
        x_dbg_stmt("lseek %lu OK\n",h_file);
    }
    
    return i4_ret;
}

static INT32 mfw_read(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count, ui4_done,ui4_alignment;
    HANDLE_T h_file;
    UINT32 i;
    int i_align_bits;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("mread [file handle] [count]  \n");
        return NTFS_TC_ERR;
    }
    StringToNumber(pps_argv[1], (INT32*)&h_file);
    StringToNumber(pps_argv[2], (INT32*)&ui4_count);

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_read( h_file, pui1_temp_buf, ui4_count, &ui4_done);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("read %lu failed\n", h_file);
    }
    else
    {
        x_dbg_stmt("read %lu OK, read %lu bytes\n",h_file, ui4_done);

        for (i = 0; i < ui4_done; i++)
        {
            if (i % 16 == 0)
            {
                x_dbg_stmt("\n\t");
            }
            x_dbg_stmt("%02x ", pui1_temp_buf[i]);
        }
        x_mem_free(pui1_buf);
    }
    
    return i4_ret;
}

static INT32 mfw_write(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count, ui4_done,ui4_alignment;
    HANDLE_T h_file;
    UINT32 i;
    int i_align_bits;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("mwrite [file handle] [count]  \n");
        return NTFS_TC_ERR;
    }
    StringToNumber(pps_argv[1], (INT32*)&h_file);
    StringToNumber(pps_argv[2], (INT32*)&ui4_count);

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }
    for (i=0;i<ui4_count;i++)
    {
        pui1_temp_buf[i] = i % 256;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_write( h_file, pui1_temp_buf, ui4_count, &ui4_done);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("write %lu failed\n", h_file);
    }
    else
    {
        x_dbg_stmt("write %lu OK, write %lu bytes\n",h_file, ui4_done);

        x_mem_free(pui1_buf);
    }
    
    return i4_ret;
}

static INT32 mfw_aread(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count,ui4_alignment;
    HANDLE_T h_file;
    UINT32 i;
    int i_align_bits;
    HANDLE_T h_req;
    HANDLE_T h_sema;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("maread [file handle] [count]  \n");
        return NTFS_TC_ERR;
    }
    StringToNumber(pps_argv[1], (INT32*)&h_file);
    StringToNumber(pps_argv[2], (INT32*)&ui4_count);

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_sema_create(&h_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    if (i4_ret != OSR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("create sema failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_read_async( h_file, pui1_temp_buf, ui4_count, 128, _mfw_async_rw_fct, (VOID*)&h_sema, &h_req);
    if (i4_ret != FMR_OK)
    {
        x_sema_delete(h_sema);
        x_mem_free(pui1_buf);
        x_dbg_stmt("aread %lu failed\n", h_file);
    }
    else
    {
        x_dbg_stmt("Async read %lu OK, request handle:%lu\n",h_file, h_req);

        x_sema_lock(h_sema, X_SEMA_OPTION_WAIT);
        x_sema_unlock(h_sema);
        
        for (i = 0; i < ui4_count; i++)
        {
            if (i % 16 == 0)
            {
                x_dbg_stmt("\n\t");
            }
            x_dbg_stmt("%02x ", pui1_temp_buf[i]);
        }
        x_sema_delete(h_sema);
        x_mem_free(pui1_buf);
    }
    
    return i4_ret;
}



static INT32 mfw_awrite(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count,ui4_alignment;
    HANDLE_T h_file;
    UINT32 i;
    int i_align_bits;
    HANDLE_T h_req;
    HANDLE_T h_sema;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("mawrite [file handle] [count]  \n");
        return NTFS_TC_ERR;
    }
    StringToNumber(pps_argv[1], (INT32*)&h_file);
    StringToNumber(pps_argv[2], (INT32*)&ui4_count);

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }
    for (i=0;i<ui4_count;i++)
    {
        pui1_temp_buf[i] = i % 256;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }

    i4_ret = x_sema_create(&h_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    if (i4_ret != OSR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("create sema failed\n");
        return i4_ret;
    }

    i4_ret = x_fm_mfw_write_async( h_file, pui1_temp_buf, ui4_count, 128, _mfw_async_rw_fct, (VOID*)&h_sema, &h_req);
    if (i4_ret != FMR_OK)
    {
        x_sema_delete(h_sema);
        x_mem_free(pui1_buf);
        x_dbg_stmt("awrite %lu failed\n", h_file);
    }
    else
    {
        x_dbg_stmt("Async write %lu OK, request handle:%lu\n",h_file, h_req);

        x_sema_lock(h_sema, X_SEMA_OPTION_WAIT);
        x_sema_unlock(h_sema);
        
        x_sema_delete(h_sema);
        x_mem_free(pui1_buf);
    }
    
    return i4_ret;
}

/* read from normal file, write to MFW */
static INT32 mfw_rnwm(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count, ui4_done,ui4_alignment,ui4_wbytes = 0;
    HANDLE_T h_src_file,h_dest_file;
    int i_align_bits;
    BOOL b_eof;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("mfw_rnwm [normal file path] [MFW path]  \n");
        return NTFS_TC_ERR;
    }
    ui4_count = 64*1024;

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }
    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, pps_argv[1], FM_READ_ONLY, 0666, TRUE , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  pps_argv[1], i4_ret);
         return NTFS_TC_ERR;
    }
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pps_argv[2], FM_READ_WRITE, 0666, TRUE , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open dest file: %s Fail: %d\n",  pps_argv[2], i4_ret);
         x_fm_close(h_src_file);
         return NTFS_TC_ERR;
    } 
    
    b_eof = FALSE;

    
    while (!b_eof)
    {
        i4_ret = x_fm_read(h_src_file,(VOID *)pui1_temp_buf , ui4_count, &ui4_done);

        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_close(h_src_file);
            x_fm_mfw_close(h_dest_file);

            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }

        i4_ret = x_fm_mfw_write(h_dest_file,(VOID *)pui1_temp_buf , ui4_count, &ui4_done);
        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                ui4_wbytes += ui4_done;
                x_dbg_stmt("Write OK! Total write %lu bytes\n", ui4_wbytes);
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync write Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_close(h_src_file);
            x_fm_mfw_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }
    }
    return CLIR_OK;
}

/* read from MFW, write to normal file */
static INT32 mfw_rmwn(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count, ui4_done,ui4_alignment,ui4_wbytes = 0;
    HANDLE_T h_src_file,h_dest_file;
    int i_align_bits;
    BOOL b_eof;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("mfw_rmwn [Src MFW path] [Dest normal file path]  \n");
        return NTFS_TC_ERR;
    }
    ui4_count = 64*1024;

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }
    
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pps_argv[1], FM_READ_ONLY, 0666, TRUE , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  pps_argv[1], i4_ret);
         return NTFS_TC_ERR;
    }
    i4_ret = x_fm_open(FM_ROOT_HANDLE, pps_argv[2], FM_READ_WRITE|FM_OPEN_CREATE, 0666, TRUE , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open dest file: %s Fail: %d\n",  pps_argv[2], i4_ret);
         x_fm_close(h_src_file);
         return NTFS_TC_ERR;
    } 
    
    b_eof = FALSE;

    
    while (!b_eof)
    {
        i4_ret = x_fm_mfw_read(h_src_file,(VOID *)pui1_temp_buf , ui4_count, &ui4_done);

        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_mfw_close(h_src_file);
            x_fm_close(h_dest_file);

            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }

        i4_ret = x_fm_write(h_dest_file,(VOID *)pui1_temp_buf , ui4_done, &ui4_done);
        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                ui4_wbytes += ui4_done;
                x_dbg_stmt("Write OK! Total write %d bytes\n", ui4_wbytes);
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Write Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_mfw_close(h_src_file);
            x_fm_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }
    }
    return CLIR_OK;
}



/* read from MFW, write to MFW */
static INT32 mfw_rmwm(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count, ui4_done,ui4_alignment,ui4_wbytes = 0;
    HANDLE_T h_src_file,h_dest_file;
    int i_align_bits;
    BOOL b_eof;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("mfw_rmwm [Src MFW path] [Dest MFW path]  \n");
        return NTFS_TC_ERR;
    }
    ui4_count = 64*1024;

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }
    
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pps_argv[1], FM_READ_ONLY, 0666, TRUE , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  pps_argv[1], i4_ret);
         return NTFS_TC_ERR;
    }
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pps_argv[2], FM_READ_WRITE, 0666, TRUE , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open dest file: %s Fail: %d\n",  pps_argv[2], i4_ret);
         x_fm_close(h_src_file);
         return NTFS_TC_ERR;
    } 
    
    b_eof = FALSE;

    
    while (!b_eof)
    {
        i4_ret = x_fm_mfw_read(h_src_file,(VOID *)pui1_temp_buf , ui4_count, &ui4_done);

        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_mfw_close(h_src_file);
            x_fm_mfw_close(h_dest_file);

            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }

        i4_ret = x_fm_mfw_write(h_dest_file,(VOID *)pui1_temp_buf , ui4_done, &ui4_done);
        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                ui4_wbytes += ui4_done;
                x_dbg_stmt("Write OK! Total write %lu bytes\n", ui4_wbytes);
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Write Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_mfw_close(h_src_file);
            x_fm_mfw_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }
    }
    return CLIR_OK;
}

/* async read from MFW, async write to MFW */
static INT32 mfw_armwm(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;
    UINT32 ui4_count, ui4_alignment,ui4_wbytes = 0;
    HANDLE_T h_src_file,h_dest_file,h_sema,h_req;
    int i_align_bits;
    BOOL b_eof;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("mfw_armwm [Src MFW path] [Dest MFW path]  \n");
        return NTFS_TC_ERR;
    }
    ui4_count = 64*1024;

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs((int)ui4_alignment) - 1;

    pui1_buf = (UINT8*) x_mem_calloc(1, ui4_count + ui4_alignment);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return NTFS_TC_ERR;
    }
    if ((UINT32)pui1_buf % ui4_alignment != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }

    i4_ret = x_fm_mfw_init();
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("init mfw failed\n");
        return i4_ret;
    }
    
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pps_argv[1], FM_READ_ONLY, 0666, TRUE , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  pps_argv[1], i4_ret);
         return NTFS_TC_ERR;
    }
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pps_argv[2], FM_READ_WRITE, 0666, TRUE , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open dest file: %s Fail: %d\n",  pps_argv[2], i4_ret);
         x_fm_close(h_src_file);
         return NTFS_TC_ERR;
    } 
    
    b_eof = FALSE;
    i4_ret = x_sema_create(&h_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        x_mem_free(pui1_buf);
        x_dbg_stmt("create sema failed\n");
        return i4_ret;
    }

    
    while (!b_eof)
    {
        
        x_sema_lock(h_sema, X_SEMA_OPTION_WAIT);
        
        i4_ret = x_fm_mfw_read_async( h_src_file, pui1_temp_buf, ui4_count, 128, _mfw_async_rw_fct, (VOID*)&h_sema, &h_req);

        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Async Read Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_sema_delete(h_sema);
            x_fm_mfw_close(h_src_file);
            x_fm_mfw_close(h_dest_file);

            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }
        x_sema_lock(h_sema, X_SEMA_OPTION_WAIT);

        i4_ret = x_fm_mfw_write_async( h_dest_file, pui1_temp_buf, ui4_count, 128, _mfw_async_rw_fct, (VOID*)&h_sema, &h_req);
        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                ui4_wbytes += ui4_count;
                x_dbg_stmt("Write OK! Total write %lu bytes\n", ui4_wbytes);
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Async write Fail!(%d).\n", i4_ret);
                x_mem_free(pui1_buf);
                pui1_buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_sema_delete(h_sema);
            
            x_fm_mfw_close(h_src_file);
            x_fm_mfw_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return i4_ret;
            }
        }
    }
    return CLIR_OK;
}


static INT32 mfw_abort(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_req;

    if( i4_argc < 2)
    {
        x_dbg_stmt("mabort [request handle] \n");
        return NTFS_TC_ERR;
    }
    StringToNumber(pps_argv[1], (INT32*)&h_req);

    i4_ret = x_fm_mfw_abort_async(h_req);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    return CLIR_OK;
}

static INT32 mfw_getinfo(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    INT32 i4_by_name;
    HANDLE_T h_file;
    FM_FILE_INFO_T  t_info;

    if( i4_argc < 3)
    {
        x_dbg_stmt("minfo [1:by name;0:by handle][mfw path/mfw handle] \n");
        return NTFS_TC_ERR;
    }

    StringToNumber(pps_argv[1], &i4_by_name);

    if (i4_by_name == 0)
    {
        StringToNumber(pps_argv[2], (INT32*)&h_file);
        i4_ret = x_fm_mfw_get_info_by_handle(h_file, &t_info);
    }
    else
    {
        i4_ret = x_fm_mfw_get_info_by_name(FM_ROOT_HANDLE, pps_argv[2], &t_info);
    }
    
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("get info failed(%d) \n", i4_ret);
    }
    else
    {
        x_dbg_stmt("size is %lld bytes\n", t_info.ui8_size);
        x_dbg_stmt("blk size is %d \n", t_info.ui4_blk_size);
        x_dbg_stmt("blk cnt is %d \n", t_info.ui8_blk_cnt);
        x_dbg_stmt("create time is %lld\n", t_info.ui8_create_time);
        x_dbg_stmt("last access time is %lld \n", t_info.ui8_access_time);
        x_dbg_stmt("last modify time is %lld \n", t_info.ui8_modify_time);
    }

    return CLIR_OK;

}

static INT32 mfw_diff(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_file1, h_file2;
    BOOL     b_eof = FALSE;
    UINT8    *buf1 = NULL,*buf2 = NULL;
    UINT8    *temp_buf1 = NULL,*temp_buf2 = NULL;
    UINT32 ui4_done_bytes;
    UINT64 ui8_match = 0;
    const CHAR *ps_path1 = NULL, *ps_path2 = NULL;
    UINT32 ui4_diff_cnt, ui4_total_cnt;
    UINT32 ui4_alignment;
    UINT32 ui4_unit;
    int i_align_bits;

    if( i4_argc < 4)
    {
        x_dbg_stmt("ntfs_cmp_file  [src file path] [dest file path] [n*1KB each time]");
        return NTFS_TC_ERR;
    }
    ps_path1 = ps_argv[1];
    ps_path2 = ps_argv[2];
    StringToNumber(ps_argv[3], (INT32*)&ui4_unit);

    ui4_alignment = 32;
    i_align_bits = ntfs_tc_ffs(ui4_alignment) - 1;

    ui4_diff_cnt = ui4_total_cnt = 0;

    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, ps_path1, FM_READ_ONLY, 0666, FALSE , &h_file1);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_path1, i4_ret);
         return NTFS_TC_ERR;
    }
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, ps_path2, FM_READ_ONLY, 0666, FALSE , &h_file2);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("Open src file: %s Fail: %d\n",  ps_path2, i4_ret);
         return NTFS_TC_ERR;
    } 
    
    b_eof = FALSE;
    do        
    {
        x_thread_delay(1);
        buf1 = (UINT8*) x_mem_calloc(1, ui4_unit* 1024 + 32); 
    } while( buf1 == NULL);
    do        
    {
        x_thread_delay(1);
        buf2 = (UINT8*) x_mem_calloc(1, ui4_unit* 1024 + 32); 
    } while( buf2 == NULL);

    if ((UINT32)buf1 % ui4_alignment != 0)
    {
        temp_buf1=  (UINT8*)(((((UINT32) buf1 ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        temp_buf1 = buf1;
    }

    if ((UINT32)buf2 % ui4_alignment != 0)
    {
        temp_buf2=  (UINT8*)(((((UINT32) buf2 ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
    }
    else
    {
        temp_buf2 = buf2;
    }

    
    while (!b_eof)
    {
        x_memset(buf1, 0, ui4_unit* 1024 + 32);
        x_memset(buf2, 0, ui4_unit* 1024 + 32);
        i4_ret = x_fm_mfw_read(h_file1,(VOID *)temp_buf1 , ui4_unit*1024, &ui4_done_bytes);

        switch (i4_ret)
        {
            case FMR_OK:
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_fm_mfw_abort_async(h_file1);
                x_mem_free(buf1);
                buf1 = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf1);
                buf1 = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf1);
                buf1 = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_mem_free(buf2);
            
            x_fm_mfw_close(h_file1);
            x_fm_mfw_close(h_file2);

            if (i4_ret == FMR_EOF)
            {
                x_dbg_stmt("Total compare %lu times\n", ui4_total_cnt);
                x_dbg_stmt("Match %llu times\n", ui8_match);
                x_dbg_stmt("Diff %lu times\n", ui4_diff_cnt);
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }

        }

        i4_ret = x_fm_mfw_read(h_file2,(VOID *)temp_buf2 , ui4_unit*1024, &ui4_done_bytes);

        switch (i4_ret)
        {
            case FMR_OK:
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("EOF!\n");
                x_fm_mfw_abort_async(h_file2);
                x_mem_free(buf2);
                buf2 = NULL;
                break;
            case FMR_INVAL:
                b_eof = TRUE;
                x_dbg_stmt("FMR_INVAL!\n");
                x_mem_free(buf2);
                buf2 = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("Sync Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf2);
                buf2 = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_mem_free(buf1);
            
            x_fm_mfw_close(h_file1);
            x_fm_mfw_close(h_file2);

            if (i4_ret == FMR_EOF)
            {
                x_dbg_stmt("Total compare %lu times\n", ui4_total_cnt);
                x_dbg_stmt("Match %llu times\n", ui8_match);
                x_dbg_stmt("Diff %lu times\n", ui4_diff_cnt);
                return NTFS_TC_OK;
            }
            else
            {
                return NTFS_TC_ERR;
            }
        }
        ui4_total_cnt++;
        /* Compare buf */
        if (x_memcmp(temp_buf1, temp_buf2, ui4_unit*1024) != 0)
        {
            ui4_diff_cnt++;
            x_dbg_stmt("Find difference, some errors have occured in previous write operation\n");
            x_thread_delay(5000);
        }
        else
        {
            x_dbg_stmt("Perfect match! The %llu time\n", ++ui8_match);
        }
    }

    return NTFS_TC_OK;
}



static VOID mfw_rw_task1(VOID* pv_tag)
{
    INT32 i4_cnt = 1;
    const CHAR* ps_argv[3];  
    CHAR *ps_src =  "/mnt/usb/Mass-001/MFW_Header.bin";
    CHAR *ps_dest = "/mnt/usb/Mass-001/MFW_Header3.bin";

    while(i4_cnt ++ < NTFS_STRESS_TEST_TIMES1)
    {
        x_dbg_stmt("[MFW-RW-Task1]The %d time test\n", i4_cnt);

        if (i4_cnt % 2 == 0)
        {  
            ps_argv[1] = ps_src;
            ps_argv[2] = ps_dest;
            
            mfw_rmwm(3, ps_argv);
        }
        else
        {
            ps_argv[2] = ps_src;
            ps_argv[1] = ps_dest;
            
            mfw_armwm(3, ps_argv);
        }
    }

    x_dbg_stmt("[MFW-RW-Task1] Finished\n");
}

static VOID mfw_rw_task2(VOID* pv_tag)
{
    INT32 i4_cnt = 1;
    const CHAR* ps_argv[3];
    CHAR *ps_src =  "/mnt/usb/Mass-001/MFW_Header2.bin";
    CHAR *ps_dest = "/mnt/usb/Mass-001/MFW_SUB4_000.avi";


    ps_argv[1] = ps_src;
    ps_argv[2] = ps_dest;

    while(i4_cnt ++ < NTFS_STRESS_TEST_TIMES1)
    {
        x_dbg_stmt("[MFW-RW-Task2]The %d time test\n", i4_cnt);

        if (i4_cnt % 2 == 0)
        {
            ps_argv[1] = ps_src;
            ps_argv[2] = ps_dest;

            mfw_armwm(3, ps_argv);
        }
        else
        {
            ps_argv[2] = ps_src;
            ps_argv[1] = ps_dest;

            mfw_rmwm(3, ps_argv);
        }
    }

    x_dbg_stmt("[MFW-RW-Task2] Finished\n");
}

static INT32 mfw_rw_stress_test(INT32 i4_argc, const CHAR** ps_argv)
{
    HANDLE_T    h_task_1, h_task_2;
    INT32 i4_ret;
    
    i4_ret = x_thread_create(&h_task_1,
                             "mfw_rw_task1",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             mfw_rw_task1,
                             0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread1 error: %d\n", i4_ret);
        return i4_ret;
    }

    i4_ret = x_thread_create(&h_task_2,
                             "mfw_rw_task2",
                             FM_DEV_THREAD_DEFAULT_STACK_SIZE * 4,
                             FM_DEV_THREAD_DEFAULT_PRIORITY,
                             mfw_rw_task2,
                             0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("create thread2 error: %d\n", i4_ret);
        return i4_ret;
    }
    
    x_thread_delay(2000);
    
    return CLIR_OK;
}

#endif /* CLI_LVL_ALL */

#ifdef CLI_LVL_ALL
/* Sync Read File */
static INT32 _fm_cli_sync_fread(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_src_file;
    BOOL     b_eof = FALSE;
    BOOL     b_direct_mode = FALSE;
    UINT8    *buf = NULL, *temp_buf = NULL;
    FM_FS_INFO_T    t_fs_info;
    UINT32     ui4_done_bytes;
    UINT32     ui4_alignment;
    INT32      i4_sep;
    int      i_align_bits, i = 1;    
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("read  [file path] [execution count]");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);
    i4_sep = i4_cnt / 2;

    i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, ps_path, &t_fs_info);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[Sync Read]Get file system info faild, %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    ui4_alignment = t_fs_info.ui4_alignment;
    i_align_bits = ntfs_tc_ffs(ui4_alignment) - 1;

    buf = (UINT8*) x_mem_calloc(1, 64 * 1024 + ui4_alignment);
    if (buf == NULL)
    {
        x_dbg_stmt("[Sync Read]Allocate 64KB buf failed\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    while (i4_cnt -- > 0)
    {   
        if (i4_cnt < i4_sep)
        {
            b_direct_mode = TRUE;
        }
        else
        {
            b_direct_mode = FALSE;
        }
        
        x_dbg_stmt("[Sync Read]The %d time test in %s mode!\n", i++, b_direct_mode ? ("direct") : ("cache"));

        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, b_direct_mode , &h_src_file);
        if ( i4_ret != FMR_OK)
        {
             x_dbg_stmt("[Sync Read]Open src file: %s Fail(%d)\n",  ps_path, i4_ret);
             if (buf != NULL)
             {
                x_mem_free(buf);
                buf = NULL;
             }
             return CLIR_CMD_EXEC_ERROR;
        }  
        
        b_eof = FALSE;
  
        if (b_direct_mode)
        {
            /* In direct mode, the buffer address must be aligned to specific address due to the hardware's limitation */
            if ((UINT32)buf % ui4_alignment != 0)
            {
                temp_buf=  (UINT8*)(((((UINT32) buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
            }
            else
            {
                temp_buf = buf;
            }
        }
        else
        {
            temp_buf = buf;
        }

        while (!b_eof)
        {
            x_memset(buf, 0, 64 * 1024 + ui4_alignment);
            
            i4_ret = x_fm_read(h_src_file,(VOID *)temp_buf , 64*1024, &ui4_done_bytes);

            switch ( i4_ret)
            {
                case FMR_OK:
                    break;
                case FMR_EOF:
                    b_eof = TRUE;
                    x_dbg_stmt("[Sync Read]EOF!\n");
                    break;
                default:
                    b_eof = TRUE;
                    x_dbg_stmt("[Sync Read]Read Fail!(%d).\n", i4_ret);
                    x_mem_free(buf);
                    buf = NULL;
                    break;
            }
   
            if (i4_ret != FMR_OK)
            {                
                if (i4_ret == FMR_EOF)
                {
                    x_fm_close(h_src_file);
                }
                else
                {
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
        }
    }
    x_mem_free(buf);
    buf = NULL;
    return CLIR_OK;
}

/* 12. Async Read File */
static INT32 _fm_cli_async_fread(INT32 i4_argc, const CHAR** ps_argv)
{
    /*
     * 1. Open file in N times.
     * 2. Read it in M times and check x_fm_feof M times.
     */
    INT32    i4_ret, i4_i, i4_ret2;
    HANDLE_T h_req, h_src_file;
    UINT8    *buf = NULL, *temp_buf = NULL;
    BOOL     b_eof = FALSE;
    FM_FS_INFO_T    t_fs_info;
    BOOL       b_direct_mode = TRUE;
    UINT32     ui4_alignment;
    NTFS_TC_ASYNC_TAG_T t_tag;
    int      i_align_bits;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt;

    if( i4_argc < 3)
    {
        x_dbg_stmt("aread  [file path] [execution count]");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);

    i4_i = 0;

    if (x_fm_get_fs_info(FM_ROOT_HANDLE, ps_path, &t_fs_info))
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    ui4_alignment = t_fs_info.ui4_alignment;
    i_align_bits  = ntfs_tc_ffs(ui4_alignment) - 1;

    i4_ret = _ntfs_tc_init_async_tag(&t_tag);
    if (i4_ret != NTFS_TC_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    buf = (UINT8*) x_mem_alloc(64* 1024 + ui4_alignment);
    if (buf == NULL)
    {
        x_dbg_stmt("[Async read]Allocate buf failed\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    while( i4_i++ < i4_cnt)
    {
        x_memset(buf, 0, 64 * 1024 + ui4_alignment);

        if (i4_i > i4_cnt/2)
        {
            b_direct_mode = FALSE;
        }
        
        t_tag.ui4_total_rcnt = 0;
        x_dbg_stmt("[Async read]The %dth time test in %s mode!\n", i4_i, b_direct_mode ? ("direct") : ("cache"));
        x_thread_delay(500);

        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, b_direct_mode , &h_src_file);
        if ( i4_ret != FMR_OK)
        {
             x_dbg_stmt("[Async read]Open src file: %s Fail: %d\n",  ps_path, i4_ret);
             if (buf != NULL)
             {
                x_mem_free(buf);
                buf = NULL;
             }
             return CLIR_CMD_EXEC_ERROR;
        }

        if (b_direct_mode)
        {
            /* In direct mode, the buffer address must be aligned
               to specific address due to the hardware's limitation */
            if ((UINT32)buf % ui4_alignment != 0)
            {
                temp_buf=  (UINT8*)(((((UINT32) buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
            }
            else
            {
                temp_buf = buf;
            }
        }
        else
        {
            temp_buf = buf;
        }
        
        b_eof = FALSE;
        /* Ensure the h_rwlock is unlocked at first */
        x_sema_unlock(t_tag.h_rw_lock);
        
        while (!b_eof)
        {   
            x_memset(buf, 0, 64 * 1024 + ui4_alignment);
            
            if (OSR_OK != x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
            {
                if (buf != NULL)
                {
                    x_mem_free(buf);
                    buf = NULL;
                }
                x_fm_close(h_src_file);
                return CLIR_CMD_EXEC_ERROR;
            }
            i4_ret = x_fm_read_async(h_src_file,(VOID *) temp_buf , 64*1024, 128, ntfs_tc_async_nfy, (VOID*)&t_tag, &h_req);

            x_thread_delay(50);

            if (i4_ret == FMR_OK)
            {
                /* if the read operation does't finish in 50ms,
                   we just try to abort the operation */
                i4_ret2 = x_sema_lock_timeout(t_tag.h_rw_lock, 50);
                if (i4_ret2 != OSR_OK)
                {
                    if (i4_ret2 == OSR_TIMEOUT)
                    {
                        x_dbg_stmt("[Async read]Abort req: %d\n", h_req);
                        if (x_handle_valid(h_req))
                        {
                            x_fm_abort_async(h_req);
                        }
                    }
                    else
                    {
                        if (buf != NULL)
                        {
                            x_mem_free(buf);
                            buf = NULL;
                        }
                        x_fm_close(h_src_file);
                        return CLIR_CMD_EXEC_ERROR;
                    }
                }  
                else   
                {
                    if (x_sema_unlock(t_tag.h_rw_lock))
                    {
                        if (buf != NULL)
                        {
                            x_mem_free(buf);
                            buf = NULL;
                        }
                        x_fm_close(h_src_file);
                        return CLIR_CMD_EXEC_ERROR;
                    }
                }
            }
            else
            {
                if (OSR_OK != x_sema_unlock(t_tag.h_rw_lock))
                {
                    if (buf != NULL)
                    {
                        x_mem_free(buf);
                        buf = NULL;
                    }
                    x_fm_close(h_src_file);
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
            
            switch (i4_ret)
            {
                case FMR_OK:
                    break;
                case FMR_EOF:
                    b_eof = TRUE;
                    x_dbg_stmt("[Async read]EOF!\n");
                    break;
                default:
                    b_eof = TRUE;
                    x_dbg_stmt("[Async read]Read Fail!(%d).\n", i4_ret);
                    x_mem_free(buf);
                    buf = NULL;
                    break;
            }
        }
        x_thread_delay(1000);
        x_fm_close(h_src_file);
    }
    if (buf != NULL)
    {
        x_mem_free(buf);
        buf = NULL;
    }
    return CLIR_OK;
}

/* 13. Sync Write File */
static INT32 _fm_cli_sync_fwrite(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_src_file, h_dest_file;
    BOOL     b_eof = FALSE;
    BOOL     b_direct_mode = FALSE;
    UINT8    *buf = NULL, *temp_buf = NULL;
    FM_FS_INFO_T    t_fs_info;
    UINT32 ui4_done_bytes, ui4_wbytes = 0;
    UINT32     ui4_alignment;
    int      i_align_bits;
    const CHAR *ps_src = NULL, *ps_dest = NULL;

    if( i4_argc < 3)
    {
        x_dbg_stmt("write  read from [src file path], write to [dest file path]");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_src = ps_argv[1];
    ps_dest = ps_argv[2];
    if (x_fm_get_fs_info(FM_ROOT_HANDLE, ps_src, &t_fs_info))
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    ui4_alignment = t_fs_info.ui4_alignment;
    i_align_bits = ntfs_tc_ffs(ui4_alignment) - 1;

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_src, FM_READ_ONLY, 0666, b_direct_mode , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("[Sync Write]Open src file: %s Fail: %d\n",  ps_src, i4_ret);
         return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_dest, FM_READ_WRITE | FM_OPEN_CREATE, 0666, b_direct_mode , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("[Sync Write]Open dest file: %s Fail: %d\n",  ps_dest, i4_ret);
         x_fm_close(h_src_file);
         return CLIR_CMD_EXEC_ERROR;
    } 
    
    b_eof = FALSE;
    do        
    {
        x_thread_delay(1);
        buf = (UINT8*) x_mem_calloc(1, 64* 1024 + ui4_alignment);
        if (b_direct_mode)
        {
            /* In direct mode, the buffer address must be aligned
               to specific address due to the hardware's limitation */
            if ((UINT32)buf % ui4_alignment != 0)
            {
                temp_buf=  (UINT8*)(((((UINT32) buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
            }
            else
            {
                temp_buf = buf;
            }
        }
        else
        {
            temp_buf = buf;
        }
    } while( buf == NULL);
    
    while (!b_eof)
    {
        i4_ret = x_fm_read(h_src_file,(VOID *)temp_buf , 64*1024, &ui4_done_bytes);

        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("[Sync Read]EOF!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("[Sync Read]Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);

            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return CLIR_CMD_EXEC_ERROR;
            }
        }

        i4_ret = x_fm_write(h_dest_file,(VOID *)temp_buf , ui4_done_bytes, &ui4_done_bytes);
        switch ( i4_ret)
        {
            case FMR_OK:
                /*x_dbg_stmt("0x%x%x%x%x  ---   ",temp_buf[0],temp_buf[1],temp_buf[2],temp_buf[3]);*/
                /*x_dbg_stmt("0x%x%x%x%x  \n",temp_buf[ui4_done_bytes-4],temp_buf[ui4_done_bytes-3],temp_buf[ui4_done_bytes-2],temp_buf[ui4_done_bytes-1]);*/
                ui4_wbytes += ui4_done_bytes;
                x_dbg_stmt("Sync WriteWrite OK! Total write %d bytes\n", ui4_wbytes);
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("Sync WriteEOF!\n");
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("[Sync Write]Write Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return CLIR_CMD_EXEC_ERROR;
            }
        }
    }
    return CLIR_OK;
}

/* 14. Async Write File */
static INT32 _fm_cli_async_fwrite(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_src_file, h_dest_file, h_req;
    BOOL     b_eof = FALSE;
    UINT8    *buf = NULL;
    UINT8    *temp_buf = NULL;
    NTFS_TC_ASYNC_TAG_T t_tag;
    const CHAR *ps_src = NULL, *ps_dest = NULL;
    BOOL b_direct_mode = TRUE;
    FM_FS_INFO_T    t_fs_info;
    UINT32   ui4_alignment;
    int      i_align_bits;

    if( i4_argc < 3)
    {
        x_dbg_stmt("awrite  read from [src file path], write to [dest file path]");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_src = ps_argv[1];
    ps_dest = ps_argv[2];

    if (x_fm_get_fs_info(FM_ROOT_HANDLE, ps_src, &t_fs_info))
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    ui4_alignment = t_fs_info.ui4_alignment;
    i_align_bits  = ntfs_tc_ffs(ui4_alignment) - 1;

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_src, FM_READ_ONLY, 0666, b_direct_mode , &h_src_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("[Async write]Open src file: %s Fail: %d\n",  ps_src, i4_ret);
         return CLIR_CMD_EXEC_ERROR;
    }  
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_dest, FM_READ_WRITE | FM_OPEN_CREATE, 0666, b_direct_mode , &h_dest_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("[Async write]Open dest file: %s Fail: %d\n",  ps_dest, i4_ret);
         x_fm_close(h_src_file);
         return CLIR_CMD_EXEC_ERROR;
    } 

    i4_ret = _ntfs_tc_init_async_tag(&t_tag);
    if (i4_ret != NTFS_TC_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    b_eof = FALSE;
    do        
    {
        x_thread_delay(1);
        buf = (UINT8*) x_mem_calloc(1, 64* 1024 + 32);
    } while( buf == NULL);

    if (b_direct_mode)
    {
        /* In direct mode, the buffer address must be aligned
           to specific address due to the hardware's limitation */
        if ((UINT32)buf % ui4_alignment != 0)
        {
            temp_buf=  (UINT8*)(((((UINT32) buf ) + ui4_alignment - 1) >> i_align_bits) << i_align_bits);
        }
        else
        {
            temp_buf = buf;
        }
    }
    else
    {
        temp_buf = buf;
    }

    /* Ensure the h_rwlock is unlocked at first */
    x_sema_unlock(t_tag.h_rw_lock);
    
    while (!b_eof)
    {
        if (OSR_OK != x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_read_async(h_src_file,(VOID *)temp_buf , 64*1024, 128, ntfs_tc_async_nfy, (VOID*)&t_tag, &h_req);

        if (i4_ret == FMR_OK)
        {
            if (OSR_OK != x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
            {
                x_mem_free(buf);
                x_fm_close(h_src_file);
                x_fm_close(h_dest_file);
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        if (OSR_OK != x_sema_unlock(t_tag.h_rw_lock))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return CLIR_CMD_EXEC_ERROR;
        }

        switch ( i4_ret)
        {
            case FMR_OK:
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("[Async Read]EOF!\n");
                x_fm_abort_async(h_src_file);
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("[Async Read]Read Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        #if 1
        /* Async write */
        if (OSR_OK != x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_write_async(h_dest_file,(VOID *)temp_buf , t_tag.ui4_done_bytes, 128, ntfs_tc_async_nfy, (VOID*)&t_tag, &h_req);

        if (i4_ret == FMR_OK)
        {
            if (OSR_OK != x_sema_lock(t_tag.h_rw_lock, X_SEMA_OPTION_WAIT))
            {
                x_mem_free(buf);
                x_fm_close(h_src_file);
                x_fm_close(h_dest_file);
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        if (x_sema_unlock(t_tag.h_rw_lock))
        {
            x_mem_free(buf);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            return CLIR_CMD_EXEC_ERROR;
        }

        switch ( i4_ret)
        {
            case FMR_OK:
                break;
            case FMR_EOF:
                b_eof = TRUE;
                x_dbg_stmt("[Async write]EOF!\n");
                x_fm_abort_async(h_dest_file);
                x_mem_free(buf);
                buf = NULL;
                break;
            default:
                b_eof = TRUE;
                x_dbg_stmt("[Async write]Write Fail!(%d).\n", i4_ret);
                x_mem_free(buf);
                buf = NULL;
                break;
        }
        if (i4_ret != FMR_OK)
        {
            x_fm_sync_file(h_dest_file);
            x_fm_close(h_src_file);
            x_fm_close(h_dest_file);
            if (i4_ret == FMR_EOF)
            {
                return CLIR_OK;
            }
            else
            {
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        #endif
    }
    if (buf != NULL)
    {
        x_mem_free(buf);
        buf = NULL;
    }
    x_thread_delay(1000);
    return CLIR_OK;
}


static INT32 _fm_cli_fgets(
    INT32       i4_argc,
    const CHAR  **ps_argv)
{
    HANDLE_T h_file = NULL_HANDLE;
    UINT8    *buf = NULL;
    const CHAR *ps_path = NULL;
    INT32 i4_cnt = 0;
    INT32 i4_ret = FMR_OK;

    if( i4_argc < 3)
    {
        x_dbg_stmt("fgets  [file path] [string length]");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_path = ps_argv[1];
    StringToNumber(ps_argv[2], &i4_cnt);

    buf = (UINT8*) x_mem_alloc(i4_cnt);
    if (buf == NULL)
    {
        x_dbg_stmt("[fgets]Allocate %dB buf failed\n", i4_cnt);
        return CLIR_CMD_EXEC_ERROR;
    }
    

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, FALSE, &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[fgets]Open src file: %s Fail(%d)\n",  ps_path, i4_ret);
        if (buf != NULL)
        {
            x_mem_free(buf);
            buf = NULL;
        }
        return CLIR_CMD_EXEC_ERROR;
    }  
        
    x_memset(buf, 0, i4_cnt);
            
    i4_ret = x_fm_fgets(h_file, buf, i4_cnt);

    if(i4_ret == FMR_OK)
    {
        x_dbg_stmt("Get String: %s\n", buf);
    }
    else
    {
        x_dbg_stmt("Get String Fail: %d\n", i4_ret);
    }

    x_fm_close(h_file);
    if(buf != NULL)
    {
        x_mem_free(buf);
        buf = NULL;
    }
   
    return CLIR_OK;
}

static INT32 _fm_cli_fputs(
    INT32       i4_argc,
    const CHAR  **ps_argv)
{
    HANDLE_T h_file = NULL_HANDLE;
    const CHAR *ps_str = NULL, *ps_dest = NULL;
    INT32 i4_ret = FMR_OK;

    if( i4_argc < 3)
    {
        x_dbg_stmt("fputs [file path] [string]");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_dest = ps_argv[1];
    ps_str = ps_argv[2];

    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_dest, FM_READ_WRITE | FM_OPEN_CREATE, 0666, FALSE , &h_file);
    if ( i4_ret != FMR_OK)
    {
         x_dbg_stmt("[fputs]Open dest file: %s Fail: %d\n",  ps_dest, i4_ret);
         return CLIR_CMD_EXEC_ERROR;
    } 

    i4_ret = x_fm_fputs(h_file, (VOID *)ps_str);
    x_fm_close(h_file);

    if(i4_ret > 0)
    {
        x_dbg_stmt("Success to write a string!\n");
        return CLIR_OK;
    }
    else
    {
        x_dbg_stmt("Fail to write a string!: %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
}


static INT32 _fm_cli_rename(
    INT32       i4_argc,
    const CHAR  **ps_argv)
{
    const CHAR *ps_old_path = NULL, *ps_new_path = NULL;
    INT32 i4_ret = FMR_OK;

    if (i4_argc < 3)
    {
        x_dbg_stmt("rename  [src file path] [dest file path]");
        return FMR_OK;
    }
    ps_old_path = ps_argv[1];
    ps_new_path = ps_argv[2];

    i4_ret = x_fm_rename(NULL_HANDLE, ps_old_path, NULL_HANDLE, ps_new_path);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Rename file fail: %d\n", i4_ret);
        return i4_ret;
    }

    x_dbg_stmt("Success to rename!\n");

    return FMR_OK;
}

static VOID _fm_async_rw_ex_fct(
        HANDLE_T            h_req,
        VOID*               pv_tag,
        FM_ASYNC_COND_T     e_async_cond,
        UINT32              ui4_data)
{
    if( e_async_cond == FM_ASYNC_COND_READ_OK)
    {
        x_dbg_stmt("[*****FM_CallBack: Read OK(h_req: 0X%X, read %lu bytes)*****]\n", h_req, ui4_data);
        if (pv_tag != NULL)
        {
            HANDLE_T* ph_sema = (HANDLE_T*)pv_tag;

            x_sema_unlock(*ph_sema);
        }

    }
    else if( e_async_cond == FM_ASYNC_COND_WRITE_OK)
    {
        x_dbg_stmt("[*****FM_CallBack: Write OK(h_req: %lu, write %lu bytes)*****]\n", h_req, ui4_data);
    }
    else if (e_async_cond == FM_ASYNC_COND_ABORT_OK)
    {
        x_dbg_stmt("[*****FM_CallBack: Abort OK!h_req: %d*****]\n", h_req);
    }
    else
    {
        x_dbg_stmt("[*****FM_CallBack: FM_ASYNC_RW_FAIL!*****]\n");
        ASSERT(0);
    }
}


static INT32 _fm_cli_async_rw_ex(INT32 i4_argc, const CHAR** pps_argv)
{
    const CHAR *ps_path = NULL;
    INT32 i, i4_ret, i4_offset, i4_count, i4_rw;
    BOOL b_direct_mode = FALSE;
    HANDLE_T h_file = NULL_HANDLE, h_req = NULL_HANDLE;
    UINT64 ui8_cur_pos;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;

    if( i4_argc < 5)
    {
        x_dbg_stmt("arw  [read:1, write:0] [file path] [offset] [count]");
        return CLIR_CMD_EXEC_ERROR;
    }

    ps_path = pps_argv[2];

    StringToNumber(pps_argv[1], (INT32*)&i4_rw);
    StringToNumber(pps_argv[3], (INT32*)&i4_offset);
    StringToNumber(pps_argv[4], (INT32*)&i4_count);

    pui1_buf = (UINT8*) x_mem_calloc(1, i4_count + 32);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    if ((UINT32)pui1_buf % 32 != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + 32 - 1) >> 5) << 5);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }
    
    if (i4_rw == 0)
    {
        for (i=0;i<i4_count;i++)
        {
            pui1_temp_buf[i] = i % 256;
        }
        
        x_dbg_stmt("[----Write in %s, from %d, write %d bytes----]\n", ps_path, i4_offset, i4_count);
    }
    else
    {
        
        x_dbg_stmt("[----Read in %s, from %d, read %d bytes----]\n", ps_path, i4_offset, i4_count);
    }


    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_WRITE | FM_OPEN_CREATE, 0666, b_direct_mode , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[----Open(or create) %s failed, return value:%d----]\n", ps_path, i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_fm_lseek(h_file,(INT64)i4_offset, FM_SEEK_BGN, &ui8_cur_pos);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[----Fseek in %s failed, return value:%d----]\n", ps_path, i4_ret);
        goto EXIT;
    }
    
    if (i4_rw == 0)
    {
        i4_ret = x_fm_write_async(h_file, pui1_temp_buf, i4_count, 128, _fm_async_rw_ex_fct, NULL, &h_req);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[----Send async write request failed, return value:%d----]\n", i4_ret);
            goto EXIT;
        }
        else
        {
            x_dbg_stmt("[----Send async write request 0X%X----]\n", h_req);
        }
    }
    else
    {
        HANDLE_T h_sema;

        i4_ret = x_sema_create(&h_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        if (i4_ret != OSR_OK)
        {
            x_dbg_stmt("[----Create semaphore lock failed, return value:%d----]\n", i4_ret);
            goto EXIT;
        }
        
        i4_ret = x_fm_read_async(h_file, pui1_temp_buf, i4_count, 128, _fm_async_rw_ex_fct, (VOID*)&h_sema, &h_req);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[----Send async read request failed, return value:%d----]\n", i4_ret);
            x_sema_delete(h_sema);
            goto EXIT;
        }
        else
        {
            x_dbg_stmt("[----Send async read request 0X%X----]\n", h_req);

            i4_ret = x_sema_lock_timeout(h_sema, 5000);
            if (i4_ret == OSR_OK)
            {
                x_dbg_stmt("[----Async read request 0X%X completed----]\n [----Dump data as follow:----]\n", h_req);
                
                x_sema_unlock(h_sema);
                
                for (i = 0; i < i4_count; i++)
                {
                    if (i % 16 == 0)
                    {
                        x_dbg_stmt("\n\t");
                    }
                    x_dbg_stmt("%02x ", pui1_temp_buf[i]);
                }
                x_dbg_stmt("\n");
                x_sema_delete(h_sema);
            }
            else if (i4_ret == OSR_TIMEOUT)
            {
                x_dbg_stmt("[----Async read request 0X%X timeout----]\n", h_req);
                x_sema_delete(h_sema);
            }
            else
            {
                x_dbg_stmt("[----OSAI error----]\n");
                x_sema_delete(h_sema);
            }
        }
    }
    
EXIT:
    if (pui1_buf != NULL)
    {
        x_mem_free(pui1_buf);
    }
    
    x_fm_close(h_file);

    return i4_ret;
    
    
}

static INT32 _fm_cli_sync_rw_ex(INT32 i4_argc, const CHAR** pps_argv)
{
    const CHAR *ps_path = NULL;
    INT32 i, i4_ret, i4_offset, i4_count, i4_rw;
    BOOL b_direct_mode = FALSE;
    HANDLE_T h_file = NULL_HANDLE;
    UINT32 ui4_done = 0;
    UINT64 ui8_cur_pos;
    UINT8* pui1_buf = NULL;
    UINT8* pui1_temp_buf = NULL;

    if( i4_argc < 5)
    {
        x_dbg_stmt("rw  [read:1, write:0] [file path] [offset] [count]");
        return CLIR_CMD_EXEC_ERROR;
    }

    ps_path = pps_argv[2];

    StringToNumber(pps_argv[1], (INT32*)&i4_rw);
    StringToNumber(pps_argv[3], (INT32*)&i4_offset);
    StringToNumber(pps_argv[4], (INT32*)&i4_count);

    pui1_buf = (UINT8*) x_mem_calloc(1, i4_count + 32);
    if (pui1_buf == NULL)
    {
        x_dbg_stmt("Allocate buf failed\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    if ((UINT32)pui1_buf % 32 != 0)
    {
        pui1_temp_buf=  (UINT8*)(((((UINT32) pui1_buf ) + 32 - 1) >> 5) << 5);
    }
    else
    {
        pui1_temp_buf = pui1_buf;
    }
    
    if (i4_rw == 0)
    {
        for (i=0;i<i4_count;i++)
        {
            pui1_temp_buf[i] = i % 256;
        }
        
        x_dbg_stmt("[----Write in %s, from %d, write %d bytes----]\n", ps_path, i4_offset, i4_count);
    }
    else
    {
        
        x_dbg_stmt("[----Read in %s, from %d, read %d bytes----]\n", ps_path, i4_offset, i4_count);
    }


    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_WRITE | FM_OPEN_CREATE, 0666, b_direct_mode , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[----Open(or create) %s failed, return value:%d----]\n", ps_path, i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_fm_lseek(h_file,(INT64)i4_offset, FM_SEEK_BGN, &ui8_cur_pos);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[----Fseek in %s failed, return value:%d----]\n", ps_path, i4_ret);
        goto EXIT;
    }
    
    if (i4_rw == 0)
    {
        i4_ret = x_fm_write(h_file, pui1_temp_buf, (UINT32)i4_count, &ui4_done);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[----Sync write failed, return value:%d----]\n", i4_ret);
            goto EXIT;
        }
        else
        {
            x_dbg_stmt("[----Sync write successfully ----]\n");
        }
    }
    else
    {
        i4_ret = x_fm_read(h_file, pui1_temp_buf, (UINT32)i4_count, &ui4_done);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[----Sync read request failed, return value:%d----]\n", i4_ret);
            goto EXIT;
        }
        else
        {
            x_dbg_stmt("[----Sync read successfully----]\n");

            x_dbg_stmt(" [----Dump data as follow:----]\n");
                            
            for (i = 0; i < i4_count; i++)
            {
                if (i % 16 == 0)
                {
                    x_dbg_stmt("\n\t");
                }
                x_dbg_stmt("%02x ", pui1_temp_buf[i]);
            }
            x_dbg_stmt("\n");
        }
    }
    
EXIT:
    if (pui1_buf != NULL)
    {
        x_mem_free(pui1_buf);
    }
    
    x_fm_close(h_file);

    return i4_ret;
    
    
}

#if 1
VOID _fm_copy_nfy(
    HANDLE_T           h_req,
    VOID*              pv_tag,
    FM_COPY_NFY_COND_T e_copy_cond,
    UINT32             ui4_data)
{
    x_dbg_stmt("%s:(h_req:%d, pv_tag:%p, e_copy_cond:%d, ui4_data:%lu)\n",__FUNCTION__, h_req, pv_tag, e_copy_cond, ui4_data);

    if (e_copy_cond == FM_COPY_NFY_COND_CONFIRM_FILE_REPLACE)
    {
        FM_CONFIRM_FILE_REPLACE_T* pt_confirm = (FM_CONFIRM_FILE_REPLACE_T*)ui4_data;

        x_dbg_stmt("%s: %s is existed in dest, replace it\n",__FUNCTION__, pt_confirm->ps_exist_file);

        pt_confirm->b_replace_existing_file = TRUE;
    }
    else if (e_copy_cond == FM_COPY_NFY_COND_PRGS)
    {
        FM_COPY_FILE_PRGS_T* pt_prgs = (FM_COPY_FILE_PRGS_T*)ui4_data;
  
        x_dbg_stmt("%s: \n"
                   "CopyingFile:\"%s\", from \"%s\" to \"%s\"\n"
                   "total file size:%llu, transferred bytes:%llu\n",
                   __FUNCTION__, pt_prgs->ps_copying, pt_prgs->ps_from, pt_prgs->ps_to, pt_prgs->ui8_file_size, pt_prgs->ui8_transferred_bytes);
        pt_prgs->b_abort = FALSE;

    }


}

static INT32 _fm_cli_sync_fcopy(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    CHAR* ps_src_path = NULL;
    CHAR* ps_dest_path = NULL;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("fcopy  [src file path] [dest file path]\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    ps_src_path = (CHAR* )ps_argv[1];
    ps_dest_path = (CHAR* )ps_argv[2];

    i4_ret = x_fm_copy_file(NULL_HANDLE, ps_src_path, NULL_HANDLE, ps_dest_path, _fm_copy_nfy);
    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("%s, OK\n",__FUNCTION__);
    }
    else
    {
        x_dbg_stmt("%s, failed as %d\n",__FUNCTION__, i4_ret);
    }

    return i4_ret;
}


static INT32 _fm_cli_async_fcopy(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_req;
    CHAR* ps_src_path = NULL;
    CHAR* ps_dest_path = NULL;
    
    if( i4_argc < 3)
    {
        x_dbg_stmt("afcopy  [src file path] [dest file path]\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    ps_src_path = (CHAR* )ps_argv[1];
    ps_dest_path = (CHAR* )ps_argv[2];

    i4_ret = x_fm_async_copy_file(NULL_HANDLE, ps_src_path, NULL_HANDLE, ps_dest_path, _fm_copy_nfy, NULL, &h_req);
    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("%s, OK, h_req:%d\n",__FUNCTION__, h_req);
    }
    else
    {
        x_dbg_stmt("%s, failed as %d\n",__FUNCTION__, i4_ret);
    }
    return i4_ret;
}

static INT32 _fm_cli_abort_async_fcopy(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_req;
    
    if( i4_argc < 2)
    {
        x_dbg_stmt("abort_afcopy  [async copy req handle]\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    StringToNumber(ps_argv[1], (INT32*)&h_req);
    
    i4_ret = x_fm_abort_async(h_req);
    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("%s, OK \n",__FUNCTION__);
    }
    else
    {
        x_dbg_stmt("%s, failed as %d\n",__FUNCTION__, i4_ret);
    }
    return i4_ret;
}
#endif


static INT32 _fm_cli_seek_read_test(INT32 i4_argc, const CHAR** ps_argv)
{
    const CHAR *ps_path = NULL;
    HANDLE_T h_file;
    INT32 i4_ret;
    FM_FILE_INFO_T  t_src_file_info;
    INT32 i4_offset;
    UINT64 ui8_cur_pos;
    UINT32 ui4_buf, ui4_read, ui4_start, ui4_end, ui4_cnt;

    if( i4_argc < 2)
    {
        x_dbg_stmt("seek_read  [file path] ");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_path = ps_argv[1];

    
    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
    if (i4_ret != FMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("[SeekReadTest]FileSize:%llu\n", t_src_file_info.ui8_size);
    if (t_src_file_info.ui8_size <= 0x00A00000)
    {
        x_dbg_stmt("[SeekReadTest]The size of the file is too small. Test with a file at least 10MB.\n");
        
        return CLIR_CMD_EXEC_ERROR;
    }

    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_ONLY, 0666, FALSE , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[SeekReadTest]Open file failed as %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    
    x_dbg_stmt("[SeekReadTest]Random seek in the range[0 - 1048576], then read 4 bytes, executed 500 times\n");
    ui4_cnt = 0;

    ui4_start = x_os_get_sys_tick();
    do
    {
        i4_offset = rand() % 1048576;

        i4_ret = x_fm_lseek(h_file, (INT64)i4_offset, FM_SEEK_BGN ,&ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[SeekReadTest]Seek file failed as %d\n", i4_ret);
            break;
        }

        i4_ret = x_fm_read(h_file,(VOID *)&ui4_buf, 4, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[SeekReadTest]Seek file failed as %d\n", i4_ret);
            break;
        }
        
    } while(ui4_cnt++<500);
    
    ui4_end = x_os_get_sys_tick();
    
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[SeekReadTest]unexpected failure happened\n");
        x_fm_close(h_file);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    x_dbg_stmt("[SeekReadTest]Totally cost %lu ms\n", (ui4_end-ui4_start)*x_os_get_sys_tick_period());


    x_dbg_stmt("[SeekReadTest]Random seek in the range[%llu - %llu], then read 4 bytes, executed 500 times\n", (t_src_file_info.ui8_size -  (UINT64)1048576 - (UINT64)4), t_src_file_info.ui8_size);
    ui4_cnt = 0;

    ui4_start = x_os_get_sys_tick();
    do
    {
        i4_offset = (INT32)(t_src_file_info.ui8_size - (rand() % 1048576) - 4);

        i4_ret = x_fm_lseek(h_file, (INT64)i4_offset, FM_SEEK_BGN ,&ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[SeekReadTest]Seek file failed as %d\n", i4_ret);
            break;
        }

        i4_ret = x_fm_read(h_file,(VOID *)&ui4_buf, 4, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[SeekReadTest]Seek file failed as %d\n", i4_ret);
            break;
        }
        
    } while(ui4_cnt++<500);

    ui4_end = x_os_get_sys_tick();

    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[SeekReadTest]unexpected failure happened\n");
        x_fm_close(h_file);
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("[SeekReadTest]Totally cost %lu ms\n", (ui4_end-ui4_start)*x_os_get_sys_tick_period());

    x_fm_close(h_file);
    return CLIR_OK;

}


static INT32 _fm_cli_rw_performance_test(INT32 i4_argc, const CHAR** pps_argv)
{
    const CHAR *ps_path = NULL;
    HANDLE_T h_file;
    INT32 i4_ret;
    FM_FILE_INFO_T  t_src_file_info;
    INT32 i4_offset, i4_count;
    UINT64 ui8_cur_pos;
    UINT32 ui4_written, ui4_read, ui4_start, ui4_end;
    VOID* pv_buf = NULL;

    if( i4_argc < 3)
    {
        x_dbg_stmt("rw  [file path] [count]");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_path = pps_argv[1];
    StringToNumber(pps_argv[2], (INT32*)&i4_count);

    pv_buf = x_mem_calloc(i4_count, 1);
    if (pv_buf == NULL)
    {
        x_dbg_stmt("[RWPTest]Alloc buffer(size:%ld) failed\n", i4_count);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_path, &t_src_file_info);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pv_buf);
        
        x_dbg_stmt("[RWPTest] Get file info failed as %ld\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("[RWPTest]FileSize:%llu\n", t_src_file_info.ui8_size);
    if (t_src_file_info.ui8_size <= 0x01400000)
    {
        x_mem_free(pv_buf);
        x_dbg_stmt("[RWPTest]The size of the file is too small. Test with a file at least 20MB.\n");
        
        return CLIR_CMD_EXEC_ERROR;
    }

    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path, FM_READ_WRITE, 0666, FALSE , &h_file);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pv_buf);
        x_dbg_stmt("[RWPTest]Open file failed as %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("[RWPTest]Random seek in the range[0 - 10MB], then read %ld bytes\n", i4_count);

    ui4_start = x_os_get_sys_tick();
    do
    {
        i4_offset = rand() % 0xA00000;

        i4_ret = x_fm_lseek(h_file, (INT64)i4_offset, FM_SEEK_BGN ,&ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[RWPTest]Seek file failed as %d\n", i4_ret);
            break;
        }

        i4_ret = x_fm_read(h_file, pv_buf, i4_count, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[RWPTest]read file failed as %d\n", i4_ret);
            break;
        }
        
    } while(0);
    
    ui4_end = x_os_get_sys_tick();
    
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pv_buf);
        x_dbg_stmt("[RWPTest]unexpected failure happened\n");
        x_fm_close(h_file);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    x_dbg_stmt("[RWPTest]READ Totally cost %lu ms\n", (ui4_end-ui4_start)*x_os_get_sys_tick_period());


    x_dbg_stmt("[RWPTest]Random seek in the range[0 - 10MB], then write %ld bytes\n", i4_count);

    ui4_start = x_os_get_sys_tick();
    do
    {
        i4_offset = rand() % 0xA00000;

        i4_ret = x_fm_lseek(h_file, (INT64)i4_offset, FM_SEEK_BGN ,&ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[RWPTest]Seek file failed as %d\n", i4_ret);
            break;
        }

        i4_ret = x_fm_write(h_file, pv_buf, i4_count, &ui4_written);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[RWPTest]write file failed as %d\n", i4_ret);
            break;
        }
        
    } while(0);

    ui4_end = x_os_get_sys_tick();

    if (i4_ret != FMR_OK)
    {
        x_mem_free(pv_buf);
        x_dbg_stmt("[RWPTest]unexpected failure happened\n");
        x_fm_close(h_file);
        return CLIR_CMD_EXEC_ERROR;
    }
    x_mem_free(pv_buf);

    x_dbg_stmt("[RWPTest]WRITE Totally cost %lu ms\n", (ui4_end-ui4_start)*x_os_get_sys_tick_period());

    x_fm_close(h_file);
    return CLIR_OK;

}


static INT32 _fm_cli_raw_dev(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_fd, i4_offset, i4_cnt;
    const CHAR *ps_name = NULL;

    if( i4_argc < 2)
    {
        x_dbg_stmt("rawdev  [dev path] [offset] [count] ");
        return CLIR_CMD_EXEC_ERROR;
    }

    ps_name = pps_argv[1];
    StringToNumber(pps_argv[2], (INT32*)&i4_offset);
    StringToNumber(pps_argv[3], (INT32*)&i4_cnt);

    x_dbg_stmt("device name:%s\n",ps_name);

    do
    {
        UINT32 ui4_i;
        UINT8* ps_buf = NULL;


        i4_fd = open(ps_name, O_RDWR, 0777);
        if (i4_fd < 0)
        {
            x_dbg_stmt("[FM] Open %s fail %s\n",ps_name, strerror(errno));
            i4_fd = open(ps_name, O_RDONLY, 0777);
            if (i4_fd < 0)
            {
                x_dbg_stmt("[FM] Open %s fail %s\n",ps_name, strerror(errno));
                i4_fd = open(ps_name, O_WRONLY, 0777);
                if (i4_fd < 0)
                {
                    x_dbg_stmt("[FM] Open %s fail %s\n",ps_name, strerror(errno));
                    break;
                }
            }
        }

        ps_buf = x_mem_alloc(i4_cnt);
        if (ps_buf != NULL)
        {
            x_dbg_stmt("read %ld bytes from offset %ld of %s\n", i4_cnt, i4_offset, ps_name);
            
            if (pread(i4_fd, ps_buf, i4_cnt, i4_offset) != i4_cnt)
            {
                x_mem_free(ps_buf);
                x_dbg_stmt("read error(%s)\n", strerror(errno));
                break;
            }
            
            for (ui4_i = 0; ui4_i < i4_cnt; ui4_i++)
            {
                if (ui4_i % 16 == 0)
                {
                    x_dbg_stmt("\n\t");
                }
                x_dbg_stmt("%02x ", ps_buf[ui4_i]);
            }
            
            x_mem_free(ps_buf);

            close(i4_fd);
        }
        else
        {
            close(i4_fd);
            break;
        }
        
    }while(0);

    return CLIR_OK;
}

static INT32 _fm_cli_get_volume_label(INT32 i4_argc, const CHAR** ps_argv)
{
    INT32 i = 0;
    INT32 i4_ret;
    const CHAR *ps_path = NULL;
    CHAR s_vol_lbl[FM_MAX_VOLUME_LABEL_LEN];
    UINT32 ui4_len = 128;
    
    if( i4_argc < 2)
    {
        x_dbg_stmt("get_vol_lbl  [file path] ");
        return CLIR_CMD_EXEC_ERROR;
    }
    ps_path = ps_argv[1];

    i4_ret = x_fm_get_volume_label(FM_ROOT_HANDLE, ps_path, s_vol_lbl, &ui4_len);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Get volume label failed as %ld\n", i4_ret);
        
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("volume label is %s\n", s_vol_lbl);
    
    while (s_vol_lbl[i])
    {
        x_dbg_stmt("%x ",(UCHAR)s_vol_lbl[i]);
        i++;
    }
    x_dbg_stmt("\n");
    return CLIR_OK;
}


static INT32 _fm_cli_partition_remount(INT32 i4_argc, const CHAR** pps_argv)
{
    int ret;
    if (i4_argc < 2)
    {
        x_dbg_stmt("remount [Mount Path] -- 0 ReadOnly; 1 ReadWrite");
        return CLIR_CMD_EXEC_ERROR;
    }

    if (*pps_argv[2] == '0')
    {
        ret = x_fm_remount_ro(pps_argv[1]);
    }
    else
    {
        ret = x_fm_remount_rw(pps_argv[1]);
    }

    if (ret !=0)
    {
        x_dbg_stmt("Error: Remount %s Ret=%d", ret);
        return ret;
    }
    else
    {     
        return CLIR_OK;
    }

}


static INT32 _fm_cli_partition_nfy(UINT32 ui4_tag, FS_PARTITION_T e_partition)
{
    x_dbg_stmt("Notify with tag %d\n", ui4_tag);
    switch (e_partition)
    {
        case FS_PARTITION_BASIC:
            x_dbg_stmt("Basic partition mounted.\n");
            break;
        case FS_PARTITION_3RD:
            x_dbg_stmt("3RD partition mounted.\n");
            break;
        default:
            x_dbg_stmt("Unknown partition.\n");
    }
    return 0;
}

static INT32 _fm_partition_triger(INT32 i4_argc, const CHAR** pps_argv)
{
    FS_PARTITION_T e_ready_partition = 0;

    if( i4_argc < 2)
    {
        x_dbg_stmt("pt [Partition]    -- 0: Basic 1: 3RD");
        return CLIR_CMD_EXEC_ERROR;
    }

    x_fm_partition_reg_nfy(_fm_cli_partition_nfy, 1111);

    StringToNumber(pps_argv[1], (INT32 *)&e_ready_partition);
    x_fm_partition_ready(e_ready_partition);
    return CLIR_OK; 
}

static INT32 _fm_cli_search(INT32 i4_argc, const CHAR** pps_argv)
{
    int ret;
    int i;
#ifndef ANDROID
    FM_SEARCH_RESULT_T search_result;
    if (i4_argc < 3)
    {
        x_dbg_stmt("search [path] [pattern]");
        return CLIR_CMD_EXEC_ERROR;
    }

    ret = x_fm_search(pps_argv[1], pps_argv[2], &search_result);
    if (ret != 0)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    if(search_result.ui4_matched_pathc == 0)
    {
        x_dbg_stmt("There is no matched file.\n");
    }

    for (i = 0; i < search_result.ui4_matched_pathc; i++)
    {
        x_dbg_stmt("%s\n", search_result.pps_pathv[i]);
    }

    x_fm_search_result_free(&search_result);
#endif
    return CLIR_OK;
}


static INT32 _fm_cli_search_async(INT32 i4_argc, const CHAR** pps_argv)
{
    int i4_ret;
#ifndef ANDROID    
    if (i4_argc < 3)
    {
        x_dbg_stmt("search [path] [pattern]");
        return CLIR_CMD_EXEC_ERROR;
    }
    FM_SEARCH_RESULT_T *pt_result = x_mem_alloc(sizeof(FM_SEARCH_RESULT_T));
    if(pt_result == NULL)
    {
        x_dbg_stmt("alloc memory fail\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = x_fm_async_search(pps_argv[1], pps_argv[2], _fm_cli_search_async_nfy, pt_result);
    if (i4_ret != 0)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
#endif
    return CLIR_OK;
}

static INT32 _fm_cli_search_stop(INT32 i4_argc, const CHAR** pps_argv)
{
#ifndef ANDROID    
    x_fm_search_stop();
#endif    
    return 0;
}

static VOID _fm_cli_search_async_nfy(FM_SEARCH_RESULT_T *pt_result)
{
#ifndef ANDROID    
    if(pt_result == NULL)
    {
        x_dbg_stmt("parameter is invalid\n");
        return;
    }
    if(pt_result->ui4_matched_pathc == 0)
    {
        x_dbg_stmt("There is no matched file.\n");
    }

    int i = 0;
    for (i = 0; i < pt_result->ui4_matched_pathc; i++)
    {
        x_dbg_stmt("%s\n", pt_result->pps_pathv[i]);
    }

    x_fm_search_result_free(pt_result);
    x_mem_free(pt_result);
#endif    
}


#endif
