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
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains Multi-File-Wrapper exported API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "handle/handle.h"
#include "os/inc/x_os.h"
#include "file_mngr/fm_buf.h"
#include "file_mngr/fm_mfw.h"
#include "file_mngr/x_fm_mfw.h"
#include "file_mngr/u_fm_mfw.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/u_fm.h"
#include "file_mngr/fm_ufs.h"
#include "file_mngr/fm_util.h"
#include "dm/x_dm.h"
#include "dm/u_dm.h"
#include "dbg/u_dbg.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Meta info */
#define MFW_META_VERSION            (0)
#define MFW_META_SIZE               (1024)

#define MFW_META_START_SIGN_FIELD   (0)
#define MFW_META_VERSION_FIELD      (MFW_META_START_SIGN_FIELD + 6)
#define MFW_META_SIZE_FIELD         (MFW_META_VERSION_FIELD + 4)
#define MFW_SUB_FILE_NUM_FIELD      (MFW_META_SIZE_FIELD + 4)
#define MFW_META_VISIBLE_FLAG_FIELD (MFW_SUB_FILE_NUM_FIELD + 4)
#define MFW_SUB_FILE_NAME_PATTERN_LEN_FIELD (MFW_META_VISIBLE_FLAG_FIELD + 1)
#define MFW_SUB_FILE_NAME_PATTERN_FIELD (MFW_SUB_FILE_NAME_PATTERN_LEN_FIELD + 1)

#define MFW_META_END_SIGN_FIELD     (1018)        

#define MFW_OFFSET(bs, off)         \
    (((UINT8 *)(bs)) + (off))
    
#define MFW_GET_VAL8(bs, off)       \
    (*(UINT8 *)MFW_OFFSET(bs, off))
    
#define MFW_SET_VAL8(bs, off, val)  \
    (*(UINT8 *)MFW_OFFSET(bs, off) = val)
    
#if defined _CPU_LITTLE_ENDIAN_

#define MFW_UINT8(ui1)  ((UINT8) ui1)
#define MFW_UINT16(ui2) ((UINT16)ui2)
#define MFW_UINT32(ui4) ((UINT32)ui4)

#define MFW_GET_VAL16(bs, off)      \
    ((UINT16)(*MFW_OFFSET(bs, off)) +              \
    (((UINT16)(*MFW_OFFSET(bs, off + 1))) << 8))
    
#define MFW_GET_VAL32(bs, off)      \
    ((UINT32)(*MFW_OFFSET(bs, off)) +             \
    (((UINT32)(*MFW_OFFSET(bs, off + 1))) << 8) +  \
    (((UINT32)(*MFW_OFFSET(bs, off + 2))) << 16) + \
    (((UINT32)(*MFW_OFFSET(bs, off + 3))) << 24))

#define MFW_SET_VAL16(bs, off, val) \
    (*MFW_OFFSET(bs, off)     = (UINT8)((UINT16)(val) & 0xff));           \
    (*MFW_OFFSET(bs, off + 1) = (UINT8)(((UINT16)(val) >> 8) & 0xff))
    
#define MFW_SET_VAL32(bs, off, val) \
    (*MFW_OFFSET(bs, off)     = (UINT8)((UINT32)(val) & 0xff));           \
    (*MFW_OFFSET(bs, off + 1) = (UINT8)(((UINT32)(val) >> 8) & 0xff));   \
    (*MFW_OFFSET(bs, off + 2) = (UINT8)(((UINT32)(val) >> 16) & 0xff));  \
    (*MFW_OFFSET(bs, off + 3) = (UINT8)(((UINT32)(val) >> 24) & 0xff))
   
#elif defined _CPU_BIG_ENDIAN_

#define MFW_UINT8(ui1)  \
        (ui1)

#define MFW_UINT16(ui2) \
        (((ui2) >> 8) | ((ui2) << 8))
        
#define MFW_UINT32(ui4) \
        ((((ui4) & 0xFF000000) >> 24) |     \
         (((ui4) & 0x00FF0000) >> 8)  |     \
         (((ui4) & 0x0000FF00) << 8)  |     \
         (((ui4) & 0x000000FF) << 24))

#define MFW_GET_VAL16(bs, off)      \
    ((UINT16)(*(UINT8 *)MFW_OFFSET(bs, off + 1)) +          \
    (((UINT16)(*(UINT8 *)MFW_OFFSET(bs, off))) << 8))
    
#define MFW_GET_VAL32(bs, off)      \
    ((UINT32)(*(UINT8 *)MFW_OFFSET(bs,  off + 3)) +         \
    (((UINT32)(*(UINT8 *)MFW_OFFSET(bs, off + 2))) << 8) +    \
    (((UINT32)(*(UINT8 *)MFW_OFFSET(bs, off + 1))) << 16) +   \
    (((UINT32)(*(UINT8 *)MFW_OFFSET(bs, off))) << 24))
    
#define MFW_SET_VAL16(bs, off, val) \
    (*MFW_OFFSET(bs, off)     = (UINT8)(((UINT16)(val) >> 8) & 0xff));    \
    (*MFW_OFFSET(bs, off + 1) = (UINT8)((UINT16)(val) & 0xff))
    
#define MFW_SET_VAL32(bs, off, val) \
    (*MFW_OFFSET(bs, off)     = (UINT8)(((UINT32)(val) >> 24) & 0xff));   \
    (*MFW_OFFSET(bs, off + 1) = (UINT8)(((UINT32)(val) >> 16) & 0xff));   \
    (*MFW_OFFSET(bs, off + 2) = (UINT8)(((UINT32)(val) >> 8) & 0xff));    \
    (*MFW_OFFSET(bs, off + 3) = (UINT8)((UINT32)(val) & 0xff))
    
#else
#error "_CPU_LITTLE_ENDIAN_ or _CPU_BIG_ENDIAN_ must be explictly defined"
#endif /* _CPU_LITTLE_ENDIAN_ */


#define MFW_LOCK_ENTRY_LIST()          \
    x_sema_lock(gh_ent_q_lock, X_SEMA_OPTION_WAIT)

#define MFW_UNLOCK_ENTRY_LIST()        \
    x_sema_unlock(gh_ent_q_lock)

#define MFW_LOCK_ENTRY(pt_ent)         \
    x_sema_lock((pt_ent)->h_mtx, X_SEMA_OPTION_WAIT)

#define MFW_UNLOCK_ENTRY(pt_ent)       \
    x_sema_unlock((pt_ent)->h_mtx)

#define MFW_LOCK_ARW_TAG_LIST()        \
    x_sema_lock(gh_arw_tag_q_lock, X_SEMA_OPTION_WAIT)

#define MFW_UNLOCK_ARW_TAG_LIST()      \
    x_sema_unlock(gh_arw_tag_q_lock)

#define MFW_LOCK_BEFORE_CLOSE()        \
    x_sema_lock(gh_mfw_cret_close_lock, X_SEMA_OPTION_WAIT)

#define MFW_UNLOCK_AFTER_CLOSE()       \
    x_sema_unlock(gh_mfw_cret_close_lock)

#define MFW_LOCK_BEFORE_CREATE()       \
    x_sema_lock(gh_mfw_cret_close_lock, X_SEMA_OPTION_WAIT)

#define MFW_UNLOCK_AFTER_CREATE()      \
    x_sema_unlock(gh_mfw_cret_close_lock)



#define FM_MFW_NFY_REQ_PRIORITY       1
#define FM_MFW_MSG_Q_NAME             "mfw_msg_q"
#define FM_MFW_MSG_Q_NUM              ((UINT16) 256)

#define FM_MFW_MSG_THREAD_NAME                "mfw_msg_thrd"
#define FM_MFW_MSG_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define FM_MFW_MSG_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define FM_MFW_FAT32_SUB_FILE_MAX_SIZE         (0x40000000)
#define FM_MFW_FAT16_SUB_FILE_MAX_SIZE         (0x20000000)
#define FM_MFW_FAT12_SUB_FILE_MAX_SIZE         (0x00800000)

#undef FM_MFW_TIME_MEASURE

#ifdef FM_MFW_TIME_MEASURE
#define MFW_TIME_STAMP(parm)                                                        \
        do                                                                          \
        {                                                                           \
            x_dbg_stmt("{MFW}FUNC:%s, LINE:%d, INFO:[", __FUNCTION__, __LINE__);    \
            x_dbg_stmt parm ;                                                       \
            x_dbg_stmt("], SYS_TICK:%lu\n", x_os_get_sys_tick());                   \
        } while(0)
#else
#define MFW_TIME_STAMP(parm) 
#endif


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT8 g_aui1_sign_start[6] = { 0x4D, 0x46, 0x57, 0xE5, 0xA8, 0xB7 };
static UINT8 g_aui1_sign_end[6]   = { 0x4D, 0x46, 0x57, 0x91, 0xE6, 0xB3 };

static DLIST_T(_FM_MFW_ENTRY_T)        gt_mfw_entry_q;
static DLIST_T(_FM_MFW_ASYNC_RW_TAG_T) gt_mfw_arw_tag_q;

static BOOL     gb_mfw_init       = FALSE;
static HANDLE_T gh_ent_q_lock     = NULL_HANDLE;
static HANDLE_T gh_arw_tag_q_lock = NULL_HANDLE;
static HANDLE_T gh_rel_req_lock   = NULL_HANDLE;
static HANDLE_T gh_mfw_msg_q      = NULL_HANDLE;
static HANDLE_T gh_mfw_thrd       = NULL_HANDLE;
static HANDLE_T gh_dm_nfy         = NULL_HANDLE;
static HANDLE_T gh_mfw_cret_close_lock = NULL_HANDLE;

/*-----------------------------------------------------------------------------
                Internal functions declarations
 ----------------------------------------------------------------------------*/
static FM_MFW_OBJ_T* _fm_mfw_alloc_mfw_obj(
    FM_MFW_ENTRY_T*            pt_mfw_ent);

static VOID  _fm_mfw_free_mfw_obj(
    VOID*                      pv_obj);

static INT32 _fm_mfw_obj_lock(
    VOID*                      pv_obj);

static INT32 _fm_mfw_obj_unlock(
    VOID*                      pv_obj);

static VOID  _fm_mfw_free_rw_obj(
    VOID*                      pv_obj);

static VOID  _fm_mfw_free_anls_cret_obj(
    VOID*                      pv_obj);

static BOOL  _fm_mfw_handle_free_fct(
    HANDLE_T                   h_handle,
    HANDLE_TYPE_T              e_type,
    VOID*                      pv_obj,
    VOID*                      pv_tag,
    BOOL                       b_req_handle);

static INT32 _fm_mfw_write_mfw_meta(
    FM_MFW_VF_META_POLICY_T    e_meta_policy,
    const CHAR*                ps_meta_path,
    const CHAR*                ps_fn_pattern,
    UINT32                     ui4_file_num,
    BOOL                       b_need_cret);

static INT32 _fm_mfw_read_mfw_meta(
    HANDLE_T                   h_dir,
    const CHAR*                ps_head_file,
    VOID*                      pv_meta);

static FM_MFW_ENTRY_T* _fm_mfw_find_entry_by_path(
    const CHAR*                ps_path);

static FM_MFW_ENTRY_T* _fm_mfw_alloc_mfw_entry(
    FM_MFW_VF_META_POLICY_T    e_meta_policy,
    const CHAR*                ps_dir,
    const CHAR*                ps_fn_pattern,
    const CHAR*                ps_meta_full_path);

static FM_MFW_ASYNC_RW_TAG_T* _fm_mfw_acquire_arw_tag(
    BOOL                       b_read);

static VOID  _fm_mfw_async_done(
    HANDLE_T                   h_req,
    VOID*                      pv_tag,
    FM_ASYNC_COND_T            e_async_cond,
    UINT32                     ui4_data);

static INT32 _fm_mfw_rebuild_mfw(
    FM_MFW_INFO_T*             pt_mfw_info);


static VOID  _fm_mfw_free_buf_in_mfw_info(
    FM_MFW_INFO_T*             pt_mfw_info);

static CHAR* _fm_mfw_splice_full_path(
    const CHAR*                ps_dir,
    const CHAR*                ps_comp);

static CHAR* _fm_mfw_splice_full_path_ex(
    HANDLE_T                   h_dir,
    const CHAR*                ps_dir,
    const CHAR*                ps_comp);

static CHAR* _fm_mfw_get_meta_file_path(
    FM_MFW_VF_META_POLICY_T    e_meta_policy,
    const CHAR*                ps_dir,
    const CHAR*                ps_meta_fn,
    const CHAR*                ps_fn_pattern);

static CHAR* _fm_mfw_get_sub_file_path(
    const CHAR*                ps_dir,
    const CHAR*                ps_fn_pattern,
    UINT32                     ui4_file_idx);

static VOID  _fm_mfw_delete_wrapper(
    FM_MFW_INFO_T*             pt_mfw_info);

static INT32 _fm_mfw_extend_last_file_in_wrapper(
    FM_MFW_ENTRY_T*            pt_mfw_ent,
    UINT64                     ui8_new_size,
    BOOL                       b_async_extend);

static VOID  _fm_mfw_dm_nfy_fct(
    DM_EVT_T                   t_evt, 
    VOID*                      pv_data, 
    VOID*                      pv_tag);

static VOID  _fm_mfw_thread_main(
    VOID*                      pv_data);



/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static INT32 _fm_mfw_write_mfw_meta(
    FM_MFW_VF_META_POLICY_T    e_meta_policy,
    const CHAR*                ps_meta_path,
    const CHAR*                ps_fn_pattern,
    UINT32                     ui4_file_num,
    BOOL                       b_need_cret)
{
    INT32 i4_ret;
    HANDLE_T h_file;
    UINT32 ui4_write;
    VOID* pv_meta = NULL;
    SIZE_T z_len;
    
    if ((e_meta_policy > FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE)|| 
        (ps_meta_path == NULL) ||
        (ps_fn_pattern == NULL) || 
        (ui4_file_num == 0))
    {
        DBG_ERROR(("[FM-MFW]%s: Wrong argument.\n", __FUNCTION__));
        return FMR_ARG;
    }
    
    z_len = x_strlen(ps_fn_pattern);
    if (z_len == 0 || z_len > 128)
    {
        DBG_ERROR(("[FM-MFW]%s: fn_pattern is empty.\n", __FUNCTION__));
        return FMR_ARG;
    }
    
    /* Allocate buffer for write MFW meta data */
    pv_meta = x_mem_alloc(MFW_META_SIZE);
    if (pv_meta == NULL)
    {
        DBG_ERROR(("[FM-MFW]%s: Can not alloc pv_meta.\n", __FUNCTION__));
        return FMR_CORE;
    }
    
    /* Generate Meta info */
    x_memcpy((UINT8*)pv_meta + MFW_META_START_SIGN_FIELD , g_aui1_sign_start, 6);
    MFW_SET_VAL32(pv_meta, MFW_META_VERSION_FIELD, MFW_META_VERSION);
    MFW_SET_VAL32(pv_meta, MFW_META_SIZE_FIELD,  MFW_META_SIZE);
    MFW_SET_VAL32(pv_meta, MFW_SUB_FILE_NUM_FIELD,  ui4_file_num);
    if (e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE)
    {
        MFW_SET_VAL32(pv_meta, MFW_META_VISIBLE_FLAG_FIELD,  1);
    }
    else
    {
        MFW_SET_VAL32(pv_meta, MFW_META_VISIBLE_FLAG_FIELD,  0);
    }
    MFW_SET_VAL8(pv_meta, MFW_SUB_FILE_NAME_PATTERN_LEN_FIELD, ((UINT8)z_len));
    x_memcpy((UINT8*)pv_meta + MFW_SUB_FILE_NAME_PATTERN_FIELD, ps_fn_pattern, z_len);
    x_memcpy((UINT8*)pv_meta + MFW_META_END_SIGN_FIELD, g_aui1_sign_end, 6);

    /* Open the head file */
    i4_ret = x_fm_open(
        FM_ROOT_HANDLE, 
        ps_meta_path, 
        (b_need_cret == TRUE)?(FM_READ_WRITE | FM_OPEN_CREATE | FM_OPEN_EXCLUDE):(FM_READ_WRITE), 
        0777, 
        FALSE, 
        &h_file);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s: Can not open meta file.\n", __FUNCTION__));
        x_mem_free(pv_meta);
        return i4_ret;
    }

    /* Write meta data to the head file */
    i4_ret = x_fm_write(
        h_file, 
        pv_meta, 
        MFW_META_SIZE, 
        &ui4_write);
    if (i4_ret != FMR_OK || ui4_write != MFW_META_SIZE)
    {
        DBG_ERROR(("[FM-MFW]%s: Write to meta file failed.\n", __FUNCTION__));
        x_fm_close(h_file);
        x_mem_free(pv_meta);
        return FMR_CORE;
    }

    /* Close head file handle */
    i4_ret = x_fm_close(h_file);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s: Failed to close meta file.\n", __FUNCTION__));
        x_mem_free(pv_meta);
        return i4_ret;
    }

    /* Exit happy */
    x_mem_free(pv_meta);
    return FMR_OK;
}


static INT32 _fm_mfw_read_mfw_meta(
    HANDLE_T        h_dir,
    const CHAR*     ps_head_file,
    VOID*           pv_meta)
{
    INT32 i4_ret;
    HANDLE_T h_file;
    UINT32 ui4_read;
    
    if (ps_head_file == NULL || pv_meta == NULL)
    {
        DBG_ERROR(("[FM-MFW]%s: Wrong Argument.\n", __FUNCTION__));
        return FMR_ARG;
    }

    /* Open head file in which contains the meta data */
    i4_ret = x_fm_open(
        h_dir, 
        ps_head_file, 
        FM_READ_ONLY, 
        0777, 
        FALSE, 
        &h_file);
    if (i4_ret != FMR_OK)
    { 
        DBG_ERROR(("[FM-MFW]%s: Fail to open meta file.\n", __FUNCTION__));
        return i4_ret;
    }

    /* Read out meta data */
    i4_ret = x_fm_read(
        h_file, 
        pv_meta, 
        MFW_META_SIZE, 
        &ui4_read);
    if (i4_ret != FMR_OK/* || ui4_read != MFW_META_SIZE*/)
    {
        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));

        x_fm_close(h_file);  
        return FMR_CORE;
    }

    /* Close head file handle */
    i4_ret = x_fm_close(h_file);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
        return i4_ret;
    }

    return FMR_OK;
}

static FM_MFW_ENTRY_T* _fm_mfw_find_entry_by_path(
    const CHAR* ps_path)
{
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;

    if (gb_mfw_init == FALSE || ps_path == NULL)
    {
        return NULL;
    }
    
    MFW_LOCK_ENTRY_LIST();

    /* Search in the global MFW entry list */
    
    DLIST_FOR_EACH(pt_mfw_ent, &gt_mfw_entry_q, t_link)
    {
        if ((x_strcmp(pt_mfw_ent->t_mfw_info.ps_meta_full_path, ps_path) == 0) &&
            ((pt_mfw_ent->ui4_status & (FM_MFW_ENTRY_INIT | FM_MFW_ENTRY_USING)) != 0))
        {
            /* Found */
            break;
        }
    }
    
    MFW_UNLOCK_ENTRY_LIST();

    /* Return founded entry or NULL */
    return pt_mfw_ent;
}


/* NOTE: ps_path should be a full path */
static FM_MFW_ENTRY_T* _fm_mfw_alloc_mfw_entry(
    FM_MFW_VF_META_POLICY_T e_meta_policy,
    const CHAR* ps_dir,
    const CHAR* ps_fn_pattern,
    const CHAR* ps_meta_full_path)
{
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;

    if (gb_mfw_init == FALSE ||
        e_meta_policy > FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE ||
        ps_dir == NULL ||
        ps_meta_full_path == NULL ||
        ps_fn_pattern == NULL)
    {
        DBG_ERROR(("[FM-MFW]%s,%d, return NULL (%d)\n", __FUNCTION__, __LINE__, FMR_ARG));
        return NULL;
    }
    
    /* Try to find a freed entry */
    MFW_LOCK_ENTRY_LIST();

    DLIST_FOR_EACH(pt_mfw_ent, &gt_mfw_entry_q, t_link)
    {
        if (pt_mfw_ent->ui4_status == FM_MFW_ENTRY_FREED)
        {
            /* Found a freed entry */
            DLIST_REMOVE(pt_mfw_ent, &gt_mfw_entry_q, t_link);
            break;
        }
    }
    
    MFW_UNLOCK_ENTRY_LIST();

    if (pt_mfw_ent == NULL)
    {
        /* Haven't found a freed entry, allocate a new one */
        pt_mfw_ent = x_mem_calloc(1, sizeof(FM_MFW_ENTRY_T));
        if (pt_mfw_ent == NULL)
        {
            DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
            return NULL;
        }

        pt_mfw_ent->t_mfw_info.e_meta_policy = e_meta_policy;
        pt_mfw_ent->t_mfw_info.ui4_file_num = 0;
        pt_mfw_ent->t_mfw_info.ps_meta_full_path = x_mem_alloc(x_strlen(ps_meta_full_path)+1);
        pt_mfw_ent->t_mfw_info.ps_dir = x_mem_alloc(x_strlen(ps_dir)+1);
        pt_mfw_ent->t_mfw_info.ps_fn_pattern = x_mem_alloc(x_strlen(ps_fn_pattern)+1);
        if (pt_mfw_ent->t_mfw_info.ps_meta_full_path == NULL ||
            pt_mfw_ent->t_mfw_info.ps_dir == NULL ||
            pt_mfw_ent->t_mfw_info.ps_fn_pattern == NULL)
        {
            _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);
            x_mem_free(pt_mfw_ent);
            
            DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
            return NULL;
        }
        x_strcpy(pt_mfw_ent->t_mfw_info.ps_meta_full_path, ps_meta_full_path);
        x_strcpy(pt_mfw_ent->t_mfw_info.ps_dir, ps_dir);
        x_strcpy(pt_mfw_ent->t_mfw_info.ps_fn_pattern, ps_fn_pattern);
    
        /* Mutex semaphore */
        if (x_sema_create(&pt_mfw_ent->h_mtx,
                          X_SEMA_TYPE_MUTEX,
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);
            x_mem_free(pt_mfw_ent);

            DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
            return NULL;
        }
    }
    else
    {
        /* The MFDC of this wrapper should be empty */
        ASSERT(DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc));
        
        pt_mfw_ent->t_mfw_info.e_meta_policy = e_meta_policy;
        pt_mfw_ent->t_mfw_info.ui4_file_num = 0;
        pt_mfw_ent->t_mfw_info.ps_meta_full_path = x_mem_alloc(x_strlen(ps_meta_full_path)+1);
        pt_mfw_ent->t_mfw_info.ps_dir = x_mem_alloc(x_strlen(ps_dir)+1);
        pt_mfw_ent->t_mfw_info.ps_fn_pattern = x_mem_alloc(x_strlen(ps_fn_pattern)+1);
        if (pt_mfw_ent->t_mfw_info.ps_meta_full_path == NULL ||
            pt_mfw_ent->t_mfw_info.ps_dir == NULL ||
            pt_mfw_ent->t_mfw_info.ps_fn_pattern == NULL)
        {
            _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);
            
            DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
            return NULL;
        }
        x_strcpy(pt_mfw_ent->t_mfw_info.ps_meta_full_path, ps_meta_full_path);
        x_strcpy(pt_mfw_ent->t_mfw_info.ps_dir, ps_dir);
        x_strcpy(pt_mfw_ent->t_mfw_info.ps_fn_pattern, ps_fn_pattern);
        
    }

    /* Setup the new MFW entry */
    DLIST_INIT(&pt_mfw_ent->t_mfdc);
    pt_mfw_ent->ui4_ref_cnt = 0;
    pt_mfw_ent->ui4_status = FM_MFW_ENTRY_INIT;

    /* Exit happy */
    return pt_mfw_ent;
}

static FM_MFW_OBJ_T* _fm_mfw_alloc_mfw_obj(
    FM_MFW_ENTRY_T* pt_mfw_ent)
{
    FM_MFW_OBJ_T* pt_mfw_obj = NULL;

    if (pt_mfw_ent == NULL)
    {
        DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_ARG));
        return NULL;
    }
    /* Allocate a new MFW object */
    pt_mfw_obj = x_mem_calloc(1, sizeof(FM_MFW_OBJ_T));
    if (pt_mfw_obj == NULL)
    {
        DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
        return NULL;
    }

    pt_mfw_obj->pf_free = _fm_mfw_free_mfw_obj;

    pt_mfw_obj->h_cur_file = NULL_HANDLE;
    pt_mfw_obj->h_previous_file = NULL_HANDLE;
    pt_mfw_obj->pt_mfw_entry = pt_mfw_ent;

    /* Create mutex semaphore */
    if (x_sema_create(&pt_mfw_obj->h_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_mfw_obj);

        DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
        return NULL;
    }

    /* Exit happy */
    return pt_mfw_obj;
}

static VOID _fm_mfw_free_mfw_obj(VOID* pv_obj)
{
    FM_MFW_OBJ_T* pt_mfw_obj = NULL;
    INT32 i4_ret;
    
    ASSERT(pv_obj != NULL);

    pt_mfw_obj = (FM_MFW_OBJ_T *)pv_obj;

    /* Close current opened file handle */
    i4_ret = x_fm_close(pt_mfw_obj->h_cur_file);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Close %s failed with (%d)\n",__FUNCTION__,__LINE__,pt_mfw_obj->h_cur_file,i4_ret));
    }

    /* Close previous opened sub file. */
    if ((pt_mfw_obj->h_previous_file != NULL_HANDLE) && (
         pt_mfw_obj->h_cur_file != pt_mfw_obj->h_previous_file))
    {
        i4_ret = x_fm_close(pt_mfw_obj->h_previous_file);
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("[FM-MFW]%s: Close previous file %s failed.(%d)\n"));
        }
    }
    
    /* Release resource */
    x_sema_delete(pt_mfw_obj->h_mtx);
    x_mem_free(pt_mfw_obj);
}


static INT32 _fm_mfw_obj_lock(VOID *pv_obj)
{
    FM_MFW_OBJ_T *pt_mtx_obj = NULL;
    INT32 i4_ret;

    ASSERT(pv_obj != NULL);

    pt_mtx_obj = (FM_MFW_OBJ_T *)pv_obj;
    
    if (pt_mtx_obj->ui4_status & FM_MFW_OBJ_CLOSING)
    {
        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    i4_ret = x_sema_lock(pt_mtx_obj->h_mtx, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
        return FMR_CORE;
    }

    /* Avoid for access a closed or closing wrapper */
    if (pt_mtx_obj->ui4_status & FM_MFW_OBJ_CLOSING)
    {
        x_sema_unlock(pt_mtx_obj->h_mtx);
        pt_mtx_obj->pf_free(pt_mtx_obj);

        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    pt_mtx_obj->ui4_lock_cnt++;
    pt_mtx_obj->ui4_status |= FM_MFW_OBJ_LOCKED;

    return FMR_OK;
}


static INT32 _fm_mfw_obj_unlock(VOID *pv_obj)
{
    FM_MFW_OBJ_T *pt_mtx_obj = NULL;
    INT32 i4_ret;

    if (pv_obj == NULL)
    {
        return FMR_OK;
    }

    pt_mtx_obj = (FM_MFW_OBJ_T *)pv_obj;
    
    if (--pt_mtx_obj->ui4_lock_cnt == 0)
    {
        pt_mtx_obj->ui4_status &= ~FM_OBJ_LOCKED;
    }

    i4_ret = x_sema_unlock(pt_mtx_obj->h_mtx);

    return (i4_ret == OSR_OK) ? FMR_OK : FMR_CORE;
}

static VOID _fm_mfw_free_buf_in_mfw_info(
    FM_MFW_INFO_T* pt_mfw_info)
{
    if (pt_mfw_info != NULL)
    {
        if (pt_mfw_info->ps_dir != NULL)
        {
            x_mem_free(pt_mfw_info->ps_dir);
            pt_mfw_info->ps_dir = NULL;
        }

        if (pt_mfw_info->ps_fn_pattern != NULL)
        {
            x_mem_free(pt_mfw_info->ps_fn_pattern);
            pt_mfw_info->ps_fn_pattern = NULL;
        }
        
        if (pt_mfw_info->ps_meta_full_path != NULL)
        {
            x_mem_free(pt_mfw_info->ps_meta_full_path);
            pt_mfw_info->ps_meta_full_path = NULL;
        }
    }
}

static VOID _fm_mfw_free_rw_obj(VOID* pv_obj)
{
    FM_MFW_ASYNC_RW_OBJ_T* pt_rw_obj = NULL;
    
    ASSERT(pv_obj != NULL);

    pt_rw_obj = (FM_MFW_ASYNC_RW_OBJ_T *)pv_obj;

    if (pt_rw_obj->ph_req != NULL)
    {
        x_mem_free(pt_rw_obj->ph_req);
    }

    x_mem_free(pt_rw_obj);
    
}

static VOID _fm_mfw_free_anls_cret_obj(VOID* pv_obj)
{
    FM_MFW_ANLS_CRET_OBJ_T* pt_anls_cret_obj = NULL;
    
    ASSERT(pv_obj != NULL);

    pt_anls_cret_obj = (FM_MFW_ANLS_CRET_OBJ_T *)pv_obj;

    if (pt_anls_cret_obj->t_anls.s_mp_path != NULL)
    {
        x_mem_free(pt_anls_cret_obj->t_anls.s_mp_path);
        pt_anls_cret_obj->t_anls.s_mp_path = NULL;
    }

    if (pt_anls_cret_obj->t_vf_cfg.s_dir != NULL)
    {
        x_mem_free(pt_anls_cret_obj->t_vf_cfg.s_dir);
        pt_anls_cret_obj->t_vf_cfg.s_dir= NULL;
    }
    
    if (pt_anls_cret_obj->t_vf_cfg.s_fn_pattern!= NULL)
    {
        x_mem_free(pt_anls_cret_obj->t_vf_cfg.s_fn_pattern);
        pt_anls_cret_obj->t_vf_cfg.s_fn_pattern= NULL;
    }
    
    if (pt_anls_cret_obj->t_vf_cfg.s_meta_file!= NULL)
    {
        x_mem_free(pt_anls_cret_obj->t_vf_cfg.s_meta_file);
        pt_anls_cret_obj->t_vf_cfg.s_meta_file= NULL;
    }

    x_sema_delete(pt_anls_cret_obj->h_lock);
    
    x_mem_free(pt_anls_cret_obj);
}


static BOOL _fm_mfw_handle_free_fct(
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID            *pv_obj,
    VOID            *pv_tag,
    BOOL            b_req_handle)
{
    if (b_req_handle == TRUE)
    {
        switch (e_type)
        {
            case FMT_MFW_OBJ:
                _fm_mfw_free_mfw_obj(pv_obj);
                break;

            case FMT_MFW_ASYNC_READ:
            case FMT_MFW_ASYNC_WRITE:
                _fm_mfw_free_rw_obj(pv_obj);
                break;

            case FMT_MFW_ASYNC_ANLS_CRET:
                _fm_mfw_free_anls_cret_obj(pv_obj);
                break;
        }
    }

    return TRUE;
}

/**
   NOTE:
   1.return NULL, if any error occured;
   2.caller need to free the allocated buffer;
**/
static CHAR* _fm_mfw_splice_full_path(
    const CHAR* ps_dir,
    const CHAR* ps_comp)
{
    SIZE_T          z_len;
    CHAR*           ps_full_path = NULL;
    CHAR*           pc_temp = NULL;

    z_len = x_strlen(ps_dir) + x_strlen(ps_comp) + 2;
    ps_full_path = (CHAR*)x_mem_alloc(z_len);
    if (ps_full_path == NULL)
    {
        return NULL;
    }

    x_memset(ps_full_path, 0, z_len);
    x_strcpy(ps_full_path, ps_dir);

    pc_temp = &(ps_full_path[x_strlen(ps_dir)-1]);
    while ((*pc_temp == '/') && (pc_temp >= ps_full_path))
    {
        /* skip the end '/' */
        *pc_temp = '\0';
        pc_temp--;
    }
    pc_temp++;
    *pc_temp = '/';
    pc_temp++;
    *pc_temp = '\0';
    
    x_strcat(ps_full_path, ps_comp);

    ASSERT(x_strlen(ps_full_path) <= z_len);

    return ps_full_path;
}

/**
   NOTE:
   1.if h_dir != NULL_HANDLE, ps_dir is ingored;  
   2.return NULL, if any error occured;
   3.caller need to free the allocated buffer;
**/
static CHAR* _fm_mfw_splice_full_path_ex(
    HANDLE_T    h_dir,
    const CHAR* ps_dir,
    const CHAR* ps_comp)
{
    INT32 i4_ret;
    SIZE_T z_len;
    CHAR* ps_full_path = NULL;
    FM_DIR_LABEL_T* pt_dir_lbl = NULL;
    
    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        return NULL;
    }

    if (pt_dir_lbl != NULL)
    {
        /* Should be enough */
        z_len = x_strlen(pt_dir_lbl->ps_path) + x_strlen(ps_comp) + 1;
        ps_full_path = (CHAR*)x_mem_alloc(z_len);
        if (ps_full_path == NULL)
        {
            goto EXIT;
        }
        x_memset(ps_full_path, 0, z_len);
        x_strcpy(ps_full_path, pt_dir_lbl->ps_path);
        x_strcat(ps_full_path, ps_comp);
    }
    else
    {
        CHAR* pc_temp;

        z_len = x_strlen(ps_dir) + x_strlen(ps_comp) + 2;
        ps_full_path = (CHAR*)x_mem_alloc(z_len);
        if (ps_full_path == NULL)
        {
            goto EXIT;
        }

        x_memset(ps_full_path, 0, z_len);
        x_strcpy(ps_full_path, ps_dir);

        pc_temp = &(ps_full_path[x_strlen(ps_dir)-1]);
        while ((*pc_temp == '/') && (pc_temp >= ps_full_path))
        {
            /* skip the end '/' */
            *pc_temp = '\0';
            pc_temp--;
        }
        pc_temp++;
        *pc_temp = '/';
        pc_temp++;
        *pc_temp = '\0';
        
        x_strcat(ps_full_path, ps_comp);
    }

EXIT:
    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    ASSERT(x_strlen(ps_full_path) <= z_len);

    return ps_full_path;
}

/* The meta file path got  should be freed by caller */
static CHAR* _fm_mfw_get_meta_file_path(
    FM_MFW_VF_META_POLICY_T e_meta_policy,
    const CHAR*             ps_dir,
    const CHAR*             ps_meta_fn,
    const CHAR*             ps_fn_pattern)
{
    CHAR* ps_path = NULL;
    
    if ((e_meta_policy > FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE) ||
        (ps_dir == NULL) ||
        ((e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE) && (ps_meta_fn == NULL)) ||
        ((e_meta_policy == FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE) && (ps_fn_pattern == NULL)))
    {
        return NULL;
    }

    if (e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE)
    {
        ps_path = _fm_mfw_splice_full_path(
                        ps_dir,
                        ps_meta_fn);
    }
    else
    {
        CHAR* ps_temp = NULL;
        SIZE_T z_len;

        z_len = x_strlen(ps_fn_pattern)+32;

        ps_temp = x_mem_alloc(z_len);
        if (ps_temp == NULL)
        {
            return NULL;
        }
        x_memset(ps_temp, 0, z_len);

        x_sprintf(ps_temp, ps_fn_pattern ,0);

        ps_path = _fm_mfw_splice_full_path(
                        ps_dir,
                        ps_temp);
        
        x_mem_free(ps_temp);
        ps_temp = NULL;
    }
    
    return ps_path;
}

/* The sub file path got should be freed by caller */
static CHAR* _fm_mfw_get_sub_file_path(
    const CHAR*             ps_dir,
    const CHAR*             ps_fn_pattern,
    UINT32                  ui4_file_idx)
{
    SIZE_T z_len;
    CHAR* ps_path = NULL;
    CHAR* ps_temp = NULL;

    if ((ps_dir == NULL) || (ps_fn_pattern == NULL))
    {
        return NULL;
    }

    z_len = x_strlen(ps_fn_pattern)+32;

    ps_temp = x_mem_alloc(z_len);
    if (ps_temp == NULL)
    {
        return NULL;
    }
    x_memset(ps_temp, 0, z_len);

    x_sprintf(ps_temp, ps_fn_pattern ,ui4_file_idx);

    ps_path = _fm_mfw_splice_full_path(
                    ps_dir,
                    ps_temp);
    
    x_mem_free(ps_temp);
    ps_temp = NULL;

    return ps_path;

}

static VOID _fm_mfw_delete_wrapper(
    FM_MFW_INFO_T* pt_mfw_info)
{
    INT32 i4_ret;
    UINT32 ui4_i;
    
    if (pt_mfw_info != NULL)
    {       
        /* if this wrapper has visible meta file, also need delete it */
        if (pt_mfw_info->e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE)
        {
            i4_ret = x_fm_delete_file(FM_ROOT_HANDLE ,pt_mfw_info->ps_meta_full_path);
            if (i4_ret != FMR_OK)
            {
                /* if it failed, ingore it */
                DBG_ERROR(("[FM-MFW]%s #%d: Delete %s failed with (%d)\n",__FUNCTION__,__LINE__,pt_mfw_info->ps_meta_full_path,i4_ret));
            }
        }
        /* delete all sub files in the wrapper */
        for (ui4_i = 0; ui4_i < pt_mfw_info->ui4_file_num; ui4_i++)
        {
            CHAR*   ps_sub_path = NULL;
             
            ps_sub_path = _fm_mfw_get_sub_file_path(
                                pt_mfw_info->ps_dir,
                                pt_mfw_info->ps_fn_pattern,
                                ui4_i);
            if (ps_sub_path == NULL)
            {
                i4_ret = FMR_CORE;
                continue;
            }
        
            i4_ret = x_fm_delete_file(FM_ROOT_HANDLE, ps_sub_path);
            if (i4_ret != FMR_OK)
            {
                /* if it failed, ingore it */
                DBG_ERROR(("[FM-MFW]%s #%d: Delete %s failed with (%d)\n",__FUNCTION__,__LINE__,ps_sub_path,i4_ret));
            }

            x_mem_free(ps_sub_path);
            ps_sub_path = NULL;           
        }   
    }
}

static INT32 _fm_mfw_extend_last_file_in_wrapper(
    FM_MFW_ENTRY_T* pt_mfw_ent,
    UINT64          ui8_new_size,
    BOOL            b_async_extend)
{
    INT32 i4_ret;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_new_sgl_fd = NULL;
    CHAR* ps_sub_file_path = NULL;
    UINT32 ui4_limit;
    UINT32 ui4_id;
    UINT64 ui8_remain;
    UINT64 ui8_start_pos;
    FM_FS_INFO_T  t_fs_info;
       
    if (pt_mfw_ent == NULL)
    {
        DBG_ERROR(("[FM-MFW]%s: pt_mfw_ent is NULL.\n", __FUNCTION__));

        return FMR_ARG;
    }
    pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);
    if (pt_sgl_fd == NULL)
    {
        DBG_ERROR(("[FM-MFW]%s: Can not get single fd.\n", __FUNCTION__));
        return FMR_INVAL;
    }

    i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, pt_mfw_ent->t_mfw_info.ps_meta_full_path, &t_fs_info);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s: Fail to get fs info.\n", __FUNCTION__));
        return i4_ret;
    }
    
    if ((UINT64)(t_fs_info.ui8_free_blk * t_fs_info.ui8_blk_size) <= (ui8_new_size - (pt_sgl_fd->ui8_end_pos - pt_sgl_fd->ui8_start_pos)))
    {
        DBG_ERROR(("[FM-MFW]%s: File system is full.\n", __FUNCTION__));
        return FMR_FILE_SYSTEM_FULL;
    }

    /* 1GB each */
    ui4_limit = FM_MFW_FAT32_SUB_FILE_MAX_SIZE;
    
    ui8_remain = ui8_new_size;
    ui8_start_pos = pt_sgl_fd->ui8_start_pos;
    ui4_id = pt_sgl_fd->ui4_id;
    
    if (ui8_remain <= (UINT64)ui4_limit)
    {
        ui4_limit = (UINT32)ui8_remain;
    }
    
    if (ui8_remain > 0)
    {
        DLIST_REMOVE(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
    }

    do
    {
        UINT32   ui4_open_attr = 0;
        
        if (ui4_id == pt_sgl_fd->ui4_id)
        {
            /* This sub file should exist; otherwise some error occured. */
            ui4_open_attr = FM_READ_WRITE;
        }
        else
        {
            /* May exist or not exist */
            ui4_open_attr = (FM_READ_WRITE | FM_OPEN_CREATE | FM_OPEN_EXCLUDE);
        }
        #ifndef LINUX_TURNKEY_SOLUTION
        if (b_async_extend == TRUE)
        {
            FM_MFW_MSG_T t_msg;

            ps_sub_file_path = _fm_mfw_get_sub_file_path(
                                    pt_mfw_ent->t_mfw_info.ps_dir,
                                    pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                                    ui4_id);
            if (ps_sub_file_path == NULL)
            {
                i4_ret = FMR_CORE; 
                DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                break;
            }

            t_msg.e_type = FM_MFW_MSG_EXTEND_SUB_FILE;
            t_msg.u.t_extend_sub_file.ps_sub_file_path = ps_sub_file_path;
            t_msg.u.t_extend_sub_file.ui4_flags = ui4_open_attr;
            t_msg.u.t_extend_sub_file.i8_length = (INT64)ui4_limit;

            i4_ret = x_msg_q_send(gh_mfw_msg_q, 
                                  (const VOID *) &t_msg, 
                                  sizeof(FM_MFW_MSG_T),
                                  FM_MFW_NFY_REQ_PRIORITY);
            
            if (i4_ret != OSR_OK)
            {
                x_mem_free(ps_sub_file_path);
                ps_sub_file_path = NULL;
                DBG_ERROR(("%s: send msg failed.\n", __FUNCTION__));
            }
        }
        else
        #else
        /* If need to create file, just create a 0 byte file */
        if (ui4_open_attr & FM_OPEN_CREATE)
        #endif
        {
            HANDLE_T h_file;

            ps_sub_file_path = _fm_mfw_get_sub_file_path(
                                    pt_mfw_ent->t_mfw_info.ps_dir,
                                    pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                                    ui4_id);
            if (ps_sub_file_path == NULL)
            {
                i4_ret = FMR_CORE; 
                DBG_ERROR(("[FM-MFW]%s:Fail to get sub file path.\n", __FUNCTION__));
                break;
            }
            
            i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                ps_sub_file_path, 
                                ui4_open_attr, 
                                0777, 
                                TRUE, 
                                &h_file );
        
            /* free the buffer for sub file path */
            x_mem_free(ps_sub_file_path);
            ps_sub_file_path = NULL;
            
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("[FM-MFW]%s: Can not open sub file.\n", __FUNCTION__));
                break;
            }
            #ifndef LINUX_TURNKEY_SOLUTION
            /* Note: In FAT, if multi threads come to extend file, it may cause lots of fragments. */
            i4_ret = x_fm_extend_file(h_file, (INT64)ui4_limit);
            if (i4_ret != FMR_OK)
            { 
                DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                break;
            }
            #endif
            
            i4_ret = x_fm_close(h_file);
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("[FM-MFW]%s: Can not close file.\n", __FUNCTION__));
                break;
            }
        }

        pt_new_sgl_fd = x_mem_alloc(sizeof(FM_MFW_SINGLE_FILE_DESC_T));
        if (pt_new_sgl_fd == NULL)
        {
            i4_ret = FMR_CORE;   
            DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
            break;
        }
        
        pt_new_sgl_fd->ui4_id = ui4_id;
        pt_new_sgl_fd->ui8_start_pos = ui8_start_pos;
        pt_new_sgl_fd->ui8_end_pos = ui8_start_pos + ui4_limit;
        ui8_start_pos = pt_new_sgl_fd->ui8_end_pos;
        
        DLIST_INSERT_TAIL(pt_new_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
   
        if (ui8_remain >= (UINT64)ui4_limit)
        {
            ui8_remain -= ((UINT64)ui4_limit);
        }
        else
        {
            ui8_remain = 0;
        }
        
        if (ui8_remain > 0 && ui8_remain < (UINT64)ui4_limit)
        {
            ui4_limit = (UINT32)ui8_remain;
        }
        
        ui4_id++;
        
    }while (ui8_remain >= (UINT64)ui4_limit);

    if (i4_ret != FMR_OK)
    {
    BACK_TO_ORIGINAL:
        /* Back to original status */
        if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
        {
            DLIST_INSERT_TAIL(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
        }
        else
        {
            while ((pt_new_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc)) != NULL)
            {
                if (pt_new_sgl_fd->ui4_id >= pt_sgl_fd->ui4_id)
                {
                    ui4_id = pt_new_sgl_fd->ui4_id;
                    
                    DLIST_REMOVE(pt_new_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
                    
                    x_mem_free(pt_new_sgl_fd);
                    pt_new_sgl_fd = NULL;
                    
                    ps_sub_file_path = _fm_mfw_get_sub_file_path(
                                            pt_mfw_ent->t_mfw_info.ps_dir,
                                            pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                                            ui4_id);
                    if (ps_sub_file_path == NULL)
                    {
                        i4_ret = FMR_CORE; 
                        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                    }
                    else
                    {
                        x_fm_delete_file(FM_ROOT_HANDLE ,ps_sub_file_path);

                        x_mem_free(ps_sub_file_path);
                        ps_sub_file_path = NULL;
                    }
                }
                else
                {
                    DLIST_INSERT_TAIL(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
                    break;
                }
            }
        }
        return i4_ret;
    }
    else
    {
        /* new files created */
        if (ui4_id > pt_mfw_ent->t_mfw_info.ui4_file_num)
        {
            /* write meta data */
            i4_ret = _fm_mfw_write_mfw_meta(
                            pt_mfw_ent->t_mfw_info.e_meta_policy,
                            pt_mfw_ent->t_mfw_info.ps_meta_full_path, 
                            pt_mfw_ent->t_mfw_info.ps_fn_pattern, 
                            ui4_id,
                            FALSE);

            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("[FM-MFW]%s: Fali to write mwf meta.\n", __FUNCTION__));
                goto BACK_TO_ORIGINAL;
            }
        }

        x_mem_free(pt_sgl_fd);
        pt_sgl_fd = NULL;
        
        pt_mfw_ent->t_mfw_info.ui4_file_num = ui4_id;

        return FMR_OK;
    }
}

static FM_MFW_ASYNC_RW_TAG_T* _fm_mfw_acquire_arw_tag(
    BOOL b_read)
{
    FM_MFW_ASYNC_RW_TAG_T* pt_rw_tag = NULL;
    
    MFW_LOCK_ARW_TAG_LIST();

    if ((pt_rw_tag = DLIST_HEAD(&gt_mfw_arw_tag_q)) != NULL)
    {
        /*  Try to find a freed entry */
        DLIST_REMOVE(pt_rw_tag, &gt_mfw_arw_tag_q, t_link);
    }
    
    MFW_UNLOCK_ARW_TAG_LIST();

    /* Allocate a new tag */
    if (pt_rw_tag == NULL)
    { 
        pt_rw_tag = (FM_MFW_ASYNC_RW_TAG_T*)x_mem_calloc(1, sizeof(FM_MFW_ASYNC_RW_TAG_T));

        if (pt_rw_tag != NULL)
        {
            pt_rw_tag->pt_rw_obj = (FM_MFW_ASYNC_RW_OBJ_T*)x_mem_calloc(1, sizeof(FM_MFW_ASYNC_RW_OBJ_T));
            if (pt_rw_tag->pt_rw_obj == NULL)
            {
                x_mem_free(pt_rw_tag);
                pt_rw_tag = NULL;
                
                DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
            }
        }
    }

    if (pt_rw_tag != NULL)
    {
        INT32 i4_ret;
        
        ASSERT(pt_rw_tag->pt_rw_obj != NULL);
        
        /* Alloc a async RW request handle */
        i4_ret = handle_alloc((b_read == TRUE)?((HANDLE_TYPE_T)FMT_MFW_ASYNC_READ):((HANDLE_TYPE_T)FMT_MFW_ASYNC_WRITE),
                              pt_rw_tag->pt_rw_obj,
                              NULL,
                              _fm_mfw_handle_free_fct,
                              &(pt_rw_tag->h_rw_req));

        if (i4_ret != HR_OK)
        {
            x_mem_free(pt_rw_tag->pt_rw_obj);
            pt_rw_tag->pt_rw_obj = NULL;
            x_mem_free(pt_rw_tag);
            pt_rw_tag = NULL;
            
            DBG_ERROR(("[FM-MFW]%s,%d, return NULL(%d)\n", __FUNCTION__, __LINE__, FMR_CORE));
        }
    }

    return pt_rw_tag;
}

static VOID _fm_mfw_anls(
    VOID* pv_data)
{
    INT32                i4_ret; 
    HANDLE_T             h_req;
    x_fm_mfw_async_fct   pf_nfy = NULL;
    FM_MFW_ANLS_CRET_OBJ_T*    pt_obj = NULL;
    HANDLE_TYPE_T        e_obj;
    FM_MFW_ANLS_PARAM_T  t_anls;    
    FM_MFW_PRGS_T        t_prgs;
    FM_MFW_ANLS_RESULT_T t_anls_res;
    FM_MFW_MSG_T         t_msg;
    UINT32               ui4_errno;
    FM_FS_INFO_T         t_fs_info;

    ASSERT(pv_data != NULL);
    
    h_req = *(HANDLE_T *)pv_data;

    ASSERT(h_req != NULL_HANDLE);

    if (handle_get_type_obj(h_req, &e_obj, (VOID**)&pt_obj) != HR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s, #%d, failed to get handle object, exit directly!\n", __FUNCTION__, __LINE__));

        x_thread_exit();
        return;
    }

    if (e_obj != FMT_MFW_ASYNC_ANLS_CRET || pt_obj == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s, #%d, not a anls/cret handle, or handle object is null, exit directly!\n", __FUNCTION__, __LINE__));
        x_thread_exit();
        return;
    }

    t_anls = pt_obj->t_anls;
    pf_nfy = t_anls.pf_nfy;

    if (pf_nfy == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s, #%d, async notify function for anls/cret is null, exit directly!\n", __FUNCTION__, __LINE__));
        x_thread_exit();
        return;
    }

    
    MFW_LOCK_BEFORE_CREATE();

    x_dbg_stmt("%s:Get file system info!\n", __FUNCTION__);

    i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, t_anls.s_mp_path, &t_fs_info);
    if (i4_ret != FMR_OK)
    {
        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
        x_dbg_stmt("[FM] %s: line:%d, return %ld\n", __FUNCTION__,__LINE__, i4_ret);
        
        pf_nfy(h_req, t_anls.pv_tag, FM_MFW_ASYNC_COND_ANLS_FAIL, ui4_errno);
        goto EXIT;
    }


    /* Notify Analyse Progress Directly */
    t_prgs.ui1_cur_phase = 1;
    t_prgs.ui1_total_phases = 1;
    t_prgs.ui4_prgs = 1000;

    pf_nfy(h_req, t_anls.pv_tag, FM_MFW_ASYNC_COND_PRGS, (UINT32)&t_prgs);

    /* Notify Analyse Result Directly */
    t_anls_res.ui8_avail_size = (t_fs_info.ui8_avail_blk) * (t_fs_info.ui8_blk_size);
    t_anls_res.ui8_continuous_size = (t_fs_info.ui8_avail_blk) * (t_fs_info.ui8_blk_size);
    
    pf_nfy(h_req, t_anls.pv_tag, FM_MFW_ASYNC_COND_ANLS_OK, (UINT32)&t_anls_res);

    MFW_UNLOCK_AFTER_CREATE();

EXIT:
    
    t_msg.e_type = FM_MFW_MSG_ASYNC_ANLS_CRET_DONE;
    t_msg.u.h_req = h_req;
    
    i4_ret = x_msg_q_send(gh_mfw_msg_q, 
                          (const VOID *) &t_msg, 
                          sizeof(FM_MFW_MSG_T),
                          FM_MFW_NFY_REQ_PRIORITY);
    
    if (i4_ret != OSR_OK)
    {
        DBG_INFO(("_fm_mfw_async_done: send msg failed.\n"));
    }

    x_thread_exit();
}

static VOID _fm_mfw_cret(
    VOID* pv_data)
{
    INT32                i4_ret;
    INT32                i4_xent_idx;
    HANDLE_T             h_req;
    UINT32               ui4_errno;
    HANDLE_TYPE_T        e_obj;
    FM_MFW_ANLS_RESULT_T t_anls_res;
    FM_MFW_VF_CFG_T      t_vf_cfg;
    FM_LOOKUP_ARG_T      t_lookup_arg;
    FM_MFW_PRGS_T        t_prgs;
    FM_MFW_MSG_T         t_msg;
    FM_MFW_ANLS_CRET_OBJ_T*    pt_obj = NULL;
    FM_XENTRY_T*               pt_prnt_xent = NULL;
    FM_XENTRY_T*               pt_mp_xent = NULL;
    FM_XENTRY_T*               pt_dev_xent = NULL;
    FM_XENTRY_T*               pt_mp_xent_tree[32];
    FM_XENTRY_T*               pt_dev_xent_tree[32];
    FM_MOUNT_T*                pt_mount = NULL;
    FM_MFW_ENTRY_T*            pt_mfw_ent = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    CHAR*                      ps_name = NULL;
    CHAR*                      ps_meta_full_path = NULL;
    x_fm_mfw_async_fct         pf_nfy = NULL;
    FM_FS_INFO_T               t_fs_info;


    ASSERT(pv_data != NULL);

    ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;

    h_req = *(HANDLE_T *)pv_data;

    ASSERT(h_req != NULL_HANDLE);

    if (handle_get_type_obj(h_req, &e_obj, (VOID**)&pt_obj) != HR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s, #%d, failed to get handle object, exit directly!\n", __FUNCTION__, __LINE__));
        return;
    }

    if (e_obj != FMT_MFW_ASYNC_ANLS_CRET || pt_obj == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s, #%d, not a anls/cret handle, or handle object is null, exit directly!\n", __FUNCTION__, __LINE__));
        return;
    }

    t_vf_cfg = pt_obj->t_vf_cfg;

    if (pt_obj->b_cret == TRUE)
    {
        pf_nfy = t_vf_cfg.pf_nfy;
    }

    if (pf_nfy == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s, #%d, async notify function for anls/cret is null, exit directly!\n", __FUNCTION__, __LINE__));
        return;
    }


    MFW_LOCK_BEFORE_CREATE();

    x_dbg_stmt("%s:Get file system info!\n", __FUNCTION__);
    
    /* Check if already aborted */
    if (pt_obj->b_abort == TRUE)
    {
        pt_obj->ui1_state = FM_MFW_ABORTED;
        i4_ret = FMR_OK;
        ui4_errno = FM_MFW_NFY_FAIL_ABORTED;
        
        DBG_ERROR(("[FM-MFW]%s,%d, this request was already aborted!\n", __FUNCTION__, __LINE__));
        goto EXIT;
    }
    
    for (i4_xent_idx = 0; i4_xent_idx < 32; i4_xent_idx++)
    {
        pt_mp_xent_tree[i4_xent_idx] = (FM_XENTRY_T*)0xFFFFFFFF;
        pt_dev_xent_tree[i4_xent_idx] = (FM_XENTRY_T*)0xFFFFFFFF;
    }
    
    i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, t_vf_cfg.s_dir, &t_fs_info);
    if (i4_ret != FMR_OK)
    {
        pt_obj->ui1_state = FM_MFW_FAILED;
        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
        x_dbg_stmt("[FM] %s: line:%d, return %ld\n", __FUNCTION__,__LINE__, i4_ret);
        
        goto EXIT;
    }
    
    ps_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    if (ps_name == NULL)
    {
        i4_ret = FMR_CORE;
        pt_obj->ui1_state = FM_MFW_FAILED;
        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
        
        DBG_ERROR(("[FM-MFW]%s,%d, allocate memory failed!\n", __FUNCTION__, __LINE__));
        goto EXIT;
    }
    i4_xent_idx = 0;
    /* Check if need create vf and if already existed */
    if (pt_obj->b_cret == TRUE)
    {        
        /* 2 phases, analyse in phase1, create in phase2 */
        t_prgs.ui1_total_phases = 2;
        t_prgs.ui1_cur_phase= 1;
        t_prgs.ui4_prgs= 0;

        ps_meta_full_path = _fm_mfw_get_meta_file_path(
                                t_vf_cfg.e_meta_policy,
                                t_vf_cfg.s_dir, 
                                t_vf_cfg.s_meta_file,
                                t_vf_cfg.s_fn_pattern);
        if (ps_meta_full_path == NULL)
        {
            i4_ret = FMR_CORE;
            pt_obj->ui1_state = FM_MFW_FAILED;
            ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
            
            DBG_ERROR(("[FM-MFW]%s,%d, get meta full path failed!\n", __FUNCTION__, __LINE__));
            goto EXIT;
        }

        t_lookup_arg.ps_name = ps_name;
        t_lookup_arg.ui8_lookup_ofst = FM_LOOKUP_NO_HINT;

        /* Check if already existed */
        i4_ret = fm_ufs_lookup(NULL,
                               ps_meta_full_path,
                               FM_UFS_CREATE,
                               FM_UFS_LOCK_PARENT|FM_UFS_LOCK_LEAF,
                               &t_lookup_arg);
        if (i4_ret != FMR_OK)
        {
            pt_obj->ui1_state = FM_MFW_FAILED;
            ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
            
            DBG_ERROR(("[FM-MFW]%s,%d, lookup %s failed(%d)!\n", __FUNCTION__, __LINE__, ps_meta_full_path, i4_ret));
            goto EXIT;
        }

        if (t_lookup_arg.pt_xent == NULL)
        {
            /* NOT existed */
            ASSERT(t_lookup_arg.pt_prnt_xent != NULL);
            
            if (IS_XENTRY_MOUNTED(t_lookup_arg.pt_prnt_xent))
            {
                pt_mount = t_lookup_arg.pt_prnt_xent->pt_mounted->pt_root_xent->pt_mount;
                pt_dev_xent = pt_mount->pt_dev_xent;
            }
            else
            {
                pt_mount = t_lookup_arg.pt_prnt_xent->pt_mount;
                pt_dev_xent = pt_mount->pt_dev_xent;
            }

            pt_mp_xent = pt_mount->pt_mp_xent;

            fm_xentry_unlock(t_lookup_arg.pt_prnt_xent);

            if (pt_dev_xent == NULL || pt_mp_xent == NULL)
            {
                i4_ret = FMR_INVAL;
                pt_obj->ui1_state = FM_MFW_FAILED;
                ui4_errno = FM_MFW_NFY_FAIL_UMOUNTED;
                
                DBG_ERROR(("[FM-MFW]%s,%d, device node was umounted!\n", __FUNCTION__, __LINE__));
                goto EXIT;
            }
        }
        else
        {
            fm_xentry_unlock(t_lookup_arg.pt_xent);
            
            fm_xentry_unlock(t_lookup_arg.pt_prnt_xent);
    
            i4_ret = FMR_EXIST;
            pt_obj->ui1_state = FM_MFW_FAILED;
            ui4_errno = FM_MFW_NFY_FAIL_ALREADY_EXISTED;
    
            DBG_ERROR(("[FM-MFW]%s,%d, %s already existed!\n", __FUNCTION__, __LINE__, ps_meta_full_path));
            goto EXIT;
        }
    }
        
    i4_xent_idx = 0;
    pt_prnt_xent = pt_mp_xent;
    while ((pt_prnt_xent != NULL) && (i4_xent_idx < 32))
    {
        pt_mp_xent_tree[i4_xent_idx++] = pt_prnt_xent;
        pt_prnt_xent = pt_prnt_xent->pt_prnt_xent;
    }
    i4_xent_idx--;
    while (i4_xent_idx >= 0)
    {
        i4_ret = fm_xentry_lock(pt_mp_xent_tree[i4_xent_idx--], X_SEMA_OPTION_WAIT);
        
        if (i4_ret != FMR_OK)
        {
            i4_ret = FMR_CORE;
            pt_obj->ui1_state = FM_MFW_FAILED;
            ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
            
            DBG_ERROR(("[FM-MFW]%s,%d, lock mp xentry tree failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
            goto EXIT;
        }
    }
    
    i4_xent_idx = 0;
    pt_prnt_xent = pt_dev_xent;
    while ((pt_prnt_xent != NULL) && (i4_xent_idx < 32))
    {
        pt_dev_xent_tree[i4_xent_idx++] = pt_prnt_xent;
        pt_prnt_xent = pt_prnt_xent->pt_prnt_xent;
    }
    i4_xent_idx--;
    while (i4_xent_idx >= 0)
    {
        i4_ret = fm_xentry_lock(pt_dev_xent_tree[i4_xent_idx--], X_SEMA_OPTION_WAIT);
        
        if (i4_ret != FMR_OK)
        {
            i4_ret = FMR_CORE;
            pt_obj->ui1_state = FM_MFW_FAILED;
            ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
            
            DBG_ERROR(("[FM-MFW]%s,%d, lock dev xentry tree failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
            goto EXIT;
        }
    }
    
    /* Start to analyse */
    pt_obj->ui1_state = FM_MFW_ANALYSING;

    /* Must be mounted */
    if (!IS_XENTRY_MOUNTED(pt_dev_xent))
    {
        i4_ret = FMR_INVAL;
        pt_obj->ui1_state = FM_MFW_FAILED;
        ui4_errno = FM_MFW_NFY_FAIL_UMOUNTED;
        
        DBG_ERROR(("[FM-MFW]%s,%d, device node was umounted!\n", __FUNCTION__, __LINE__));
        goto EXIT;
    }
    /* check device type */
    if (!IS_XENTRY_BLK(pt_dev_xent) && !IS_XENTRY_CHR(pt_dev_xent))        
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        pt_obj->ui1_state = FM_MFW_FAILED;
        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
        
        DBG_ERROR(("[FM-MFW]%s,%d, neither a blk device, nor a chr device!\n", __FUNCTION__, __LINE__));
        goto EXIT;
    }

    /* FAT/NTFS filesystem */
    if (pt_mount->t_fs_info.e_type == FM_TYPE_FAT || pt_mount->t_fs_info.e_type == FM_TYPE_NTFS)
    {
        /* Notify Analyse Progress Directly */
        t_prgs.ui1_cur_phase = 1;
        t_prgs.ui1_total_phases = 2;
        t_prgs.ui4_prgs = 1000;
        
        pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_PRGS, (UINT32)&t_prgs);
        
        /* Notify Analyse Result Directly */
        t_anls_res.ui8_avail_size = (t_fs_info.ui8_avail_blk) * (t_fs_info.ui8_blk_size);
        t_anls_res.ui8_continuous_size = (t_fs_info.ui8_avail_blk) * (t_fs_info.ui8_blk_size);
        
        pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_ANLS_OK, (UINT32)&t_anls_res);


        /* Now start to create a wrapper file */
        if (pt_obj->b_cret == TRUE)
        {
            UINT64 ui8_start_pos;
            UINT64 ui8_remain;
            UINT32 ui4_limit;
            UINT32 ui4_i;
            HANDLE_T h_file;
            
            pt_obj->ui1_state = FM_MFW_CREATING;

            t_prgs.ui1_total_phases = 2;
            t_prgs.ui1_cur_phase= 2;
            t_prgs.ui4_prgs= 0;
            
            if (pt_obj->b_abort == TRUE)
            {
                pt_obj->ui1_state = FM_MFW_ABORTED;
                i4_ret = FMR_OK;
                ui4_errno = FM_MFW_NFY_FAIL_ABORTED;
                
                DBG_ERROR(("[FM-MFW]%s,%d, this request was aborted!\n", __FUNCTION__, __LINE__));
                goto EXIT;
            }

            if (t_vf_cfg.ui8_init_size > t_anls_res.ui8_avail_size)
            {
                i4_ret = FMR_CORE;
                pt_obj->ui1_state = FM_MFW_FAILED;
                ui4_errno = FM_MFW_NFY_FAIL_NO_ENOUGH_SPACE;
                
                DBG_ERROR(("[FM-MFW]%s,%d, no enough space in disk!\n", __FUNCTION__, __LINE__));
                goto EXIT;
            }

            if ((t_vf_cfg.ui8_init_size > t_anls_res.ui8_continuous_size) &&
                (t_vf_cfg.e_vf_policy == FM_MFW_VF_POLICY_PHYSIC_CONTINS_FORCE))
            {
                i4_ret = FMR_CORE;
                pt_obj->ui1_state = FM_MFW_FAILED;
                ui4_errno = FM_MFW_NFY_FAIL_NO_ENOUGH_CONTINS_SPACE;
                
                DBG_ERROR(("[FM-MFW]%s,%d, no enough continuous space in disk!\n", __FUNCTION__, __LINE__));
                goto EXIT;
            }
            
            /* 1GB each */
            ui4_limit = FM_MFW_FAT32_SUB_FILE_MAX_SIZE/*0xFFFFFFFF*/;

            while ((UINT64)ui4_limit > t_anls_res.ui8_avail_size)
            {
                ui4_limit = ui4_limit / 2;
            }
            /* Allocate a new MFW entry */
            pt_mfw_ent = _fm_mfw_alloc_mfw_entry(
                                t_vf_cfg.e_meta_policy,
                                t_vf_cfg.s_dir, 
                                t_vf_cfg.s_fn_pattern,
                                ps_meta_full_path);
            if (pt_mfw_ent == NULL)
            {
                i4_ret = FMR_CORE;
                pt_obj->ui1_state = FM_MFW_FAILED;
                ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                
                DBG_ERROR(("[FM-MFW]%s,%d, allocate MFW entry failed!\n", __FUNCTION__, __LINE__));
                goto EXIT;
            }

            /* start to create sub files in the wrapper */
            ui8_start_pos = 0;
            
            if (t_vf_cfg.e_vf_policy == FM_MFW_VF_POLICY_NO_CONSTRAINT)
            {
            _NO_CONSTRAINT:
                /* When the meta data is contained in the head file of the wrapper, we need add
                                the META size. */
                ui8_remain = t_vf_cfg.ui8_init_size;
                ui4_i = 0;
                
                if (t_vf_cfg.e_meta_policy == FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE)
                {
                    ui8_remain += MFW_META_SIZE;
                }
                
                if (ui8_remain <= (UINT64)ui4_limit)
                {
                    ui4_limit = (UINT32)ui8_remain;
                }
                
                do
                {
                    CHAR* ps_sub_file_path = NULL;
                    UINT32 ui4_step = 0;

                    /* Check if it was aborted */
                    if (pt_obj->b_abort == TRUE)
                    {
                        pt_obj->ui1_state = FM_MFW_ABORTED;
                        i4_ret = FMR_OK;
                        ui4_errno = FM_MFW_NFY_FAIL_ABORTED;
                        
                        DBG_ERROR(("[FM-MFW]%s,%d, this request was already aborted!\n", __FUNCTION__, __LINE__));
                        goto EXIT;
                    }
                    
                    ps_sub_file_path = _fm_mfw_get_sub_file_path(
                                            t_vf_cfg.s_dir,
                                            t_vf_cfg.s_fn_pattern,
                                            ui4_i);
                    if (ps_sub_file_path == NULL)
                    {
                        i4_ret = FMR_CORE;
                        pt_obj->ui1_state = FM_MFW_FAILED;
                        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                        
                        DBG_ERROR(("[FM-MFW]%s,%d, get sub file path failed!\n", __FUNCTION__, __LINE__));
                        goto EXIT;
                    }

                    i4_ret = x_fm_open(
                        FM_ROOT_HANDLE, 
                        ps_sub_file_path, 
                        (FM_READ_WRITE | FM_OPEN_CREATE | FM_OPEN_EXCLUDE), 
                        0777, 
                        TRUE, 
                        &h_file);

                    /* free the buffer for sub file path */
                    x_mem_free(ps_sub_file_path);
                    ps_sub_file_path = NULL;

                    if (i4_ret != FMR_OK)
                    {
                        i4_ret = FMR_CORE;
                        pt_obj->ui1_state = FM_MFW_FAILED;
                        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                        
                        DBG_ERROR(("[FM-MFW]%s,%d, create sub file failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
                        goto EXIT;
                    }
                    
                    while((ui4_step + 0x200000) < ui4_limit)
                    {
                        ui4_step += 0x200000;
                        
                        i4_ret = x_fm_extend_file(h_file, (INT64)ui4_step);
                        if (i4_ret != FMR_OK)
                        {
                            i4_ret = FMR_CORE;
                            pt_obj->ui1_state = FM_MFW_FAILED;
                            ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                            
                            DBG_ERROR(("[FM-MFW]%s,%d, extend sub file size failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
                            goto EXIT;
                        }
                        
                        if (t_vf_cfg.ui8_init_size == (UINT64)0)
                        {
                            t_prgs.ui4_prgs = 1000;
                        }
                        else
                        {
                            t_prgs.ui4_prgs = (UINT32)(((ui8_start_pos+ui4_step) * 1000) / (t_vf_cfg.ui8_init_size));
                        }
                        if (t_prgs.ui4_prgs > 1000)
                        {
                            t_prgs.ui4_prgs = 1000;
                        }
                        
                        pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_PRGS, (UINT32)&t_prgs);

                        
                    } 
                    if (ui4_step < ui4_limit)
                    {
                        i4_ret = x_fm_extend_file(h_file, (INT64)ui4_limit);
                        if (i4_ret != FMR_OK)
                        {
                            i4_ret = FMR_CORE;
                            pt_obj->ui1_state = FM_MFW_FAILED;
                            ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                            
                            DBG_ERROR(("[FM-MFW]%s,%d, extend sub file size failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
                            goto EXIT;
                        }
                        
                        if (t_vf_cfg.ui8_init_size == (UINT64)0)
                        {
                            t_prgs.ui4_prgs = 1000;
                        }
                        else
                        {
                            t_prgs.ui4_prgs = (UINT32)(((ui8_start_pos+ui4_limit) * 1000) / (t_vf_cfg.ui8_init_size));
                        }
                        if (t_prgs.ui4_prgs > 1000)
                        {
                            t_prgs.ui4_prgs = 1000;
                        }
                        
                        pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_PRGS, (UINT32)&t_prgs);
                    }

                    i4_ret = x_fm_close(h_file);
                    if (i4_ret != FMR_OK)
                    {
                        i4_ret = FMR_CORE;
                        pt_obj->ui1_state = FM_MFW_FAILED;
                        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                        
                        DBG_ERROR(("[FM-MFW]%s,%d, close sub file failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
                        goto EXIT;
                    }

                    pt_sgl_fd = x_mem_alloc(sizeof(FM_MFW_SINGLE_FILE_DESC_T));
                    if (pt_sgl_fd == NULL)
                    {
                        i4_ret = FMR_CORE;
                        pt_obj->ui1_state = FM_MFW_FAILED;
                        ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                        
                        DBG_ERROR(("[FM-MFW]%s,%d, allocate memory failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
                        goto EXIT;
                    }

                    pt_sgl_fd->ui4_id = ui4_i;
                    pt_sgl_fd->ui8_start_pos = ui8_start_pos;
                    pt_sgl_fd->ui8_end_pos = ui8_start_pos + ui4_limit;
                    ui8_start_pos = pt_sgl_fd->ui8_end_pos;
                    
                    DLIST_INSERT_TAIL(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);

                    if (ui8_remain >= (UINT64)ui4_limit)
                    {
                        ui8_remain -= ((UINT64)ui4_limit);
                    }
                    else
                    {
                        ui8_remain = 0;
                    }
                    
                    if (ui8_remain > 0 && ui8_remain < (UINT64)ui4_limit)
                    {
                        ui4_limit = (UINT32)ui8_remain;
                    }
                    
                    ui4_i++;
                } while((ui8_remain >= (UINT64)ui4_limit) && (ui8_remain > 0));

                pt_mfw_ent->t_mfw_info.ui4_file_num = ui4_i;
                pt_mfw_ent->ui8_user_size = t_vf_cfg.ui8_init_size;

                /* write meta data */
                i4_ret = _fm_mfw_write_mfw_meta(
                    t_vf_cfg.e_meta_policy,
                    ps_meta_full_path, 
                    t_vf_cfg.s_fn_pattern, 
                    ui4_i,
                    (t_vf_cfg.e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE)?(TRUE):(FALSE));
                
                if (i4_ret != FMR_OK)
                {
                    i4_ret = FMR_CORE;
                    pt_obj->ui1_state = FM_MFW_FAILED;
                    ui4_errno = FM_MFW_NFY_FAIL_CORE_ERROR;
                    
                    DBG_ERROR(("[FM-MFW]%s,%d, write mfw meta data failed(%d)!\n", __FUNCTION__, __LINE__, i4_ret));
                    goto EXIT;
                }
                
                MFW_LOCK_ENTRY_LIST();
                
                DLIST_INSERT_TAIL(pt_mfw_ent, &gt_mfw_entry_q, t_link);
                
                MFW_UNLOCK_ENTRY_LIST();
                
            }
            else if (t_vf_cfg.e_vf_policy == FM_MFW_VF_POLICY_PHYSIC_CONTINS_FORCE)
            {
                /* Todo in Phase2 */
                goto _NO_CONSTRAINT;
            }
            else
            {
                /* Todo in Phase2 */
                goto _NO_CONSTRAINT;
            }
        }
    }
    else
    {
        /* Todo in phase2 */
        i4_ret = FMR_CORE;
        pt_obj->ui1_state = FM_MFW_FAILED;
        ui4_errno = FM_MFW_NFY_FAIL_NOT_SUPPORTED;
        goto EXIT;
    }

EXIT:
    /* Notify User analyse result */
    if (OSR_OK != x_sema_lock(pt_obj->h_lock, X_SEMA_OPTION_WAIT))
    {
        i4_ret = FMR_CORE;
        goto EXIT2;
    }

    if (i4_ret == FMR_OK)
    {
        if (pt_obj->ui1_state == FM_MFW_ABORTED)
        {
            if (pt_obj->b_cret == TRUE)
            {
                /* Abort OK */
                pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_ABORT_OK, 0);
                
                /* create is failed as it was aborted*/
                pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_CREATE_FAIL, ui4_errno);
            }
            
        }
        else if (pt_obj->ui1_state == FM_MFW_ANALYSING)
        {
            pt_obj->ui1_state = FM_MFW_DONE;
            /* Inform APP of the result */
            pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_ANLS_OK, (UINT32)&t_anls_res);
            
            if (pt_obj->b_abort == TRUE)
            {
                ui4_errno = FM_MFW_NFY_FAIL_ALREADY_DONE;
                /* Just aborted, I treat it as aborted FAIL, because analyse already done */
                pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_ABORT_FAIL, ui4_errno);
            }
        }
        else if (pt_obj->ui1_state == FM_MFW_CREATING)
        {
            pt_obj->ui1_state = FM_MFW_DONE;
            /* Inform APP of the result */
            pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_CREATE_OK, 0);
            
            if (pt_obj->b_abort == TRUE)
            {
                ui4_errno = FM_MFW_NFY_FAIL_ALREADY_DONE;
                /* Just aborted, I treat it as aborted FAIL, because analyse already done */
                pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_ABORT_FAIL, ui4_errno);
            }
        }
        else
        {
            ASSERT(0);
        }
    }
    else
    {
        ASSERT(pt_obj->ui1_state == FM_MFW_FAILED);

        if (pt_obj->b_cret == TRUE)
        {
            pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_CREATE_FAIL, ui4_errno);
            
            if (pt_obj->b_abort == TRUE)
            {
                pf_nfy(h_req, t_vf_cfg.pv_tag, FM_MFW_ASYNC_COND_ABORT_FAIL, ui4_errno);
            }

        }        

    }

    x_sema_unlock(pt_obj->h_lock);

EXIT2:
    if (i4_ret != FMR_OK)
    {
        if (pt_mfw_ent != NULL)
        {
            while ((pt_sgl_fd = DLIST_HEAD(&pt_mfw_ent->t_mfdc)) != NULL)
            {
                DLIST_REMOVE(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);

                x_mem_free(pt_sgl_fd);
                pt_sgl_fd = NULL;
            }
            _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);
            x_sema_delete(pt_mfw_ent->h_mtx);
            x_mem_free(pt_mfw_ent);
        }
    }
    
    if (ps_name != NULL)
    {
        x_mem_free(ps_name);
    }
    if (ps_meta_full_path != NULL)
    {
        x_mem_free(ps_meta_full_path);
    }
    i4_xent_idx = 0;
    while (pt_mp_xent_tree[i4_xent_idx] != (FM_XENTRY_T*)0xFFFFFFFF)
    {
        fm_xentry_unlock(pt_mp_xent_tree[i4_xent_idx]);
        i4_xent_idx++;
    }
    i4_xent_idx = 0;
    while (pt_dev_xent_tree[i4_xent_idx] != (FM_XENTRY_T*)0xFFFFFFFF)
    {
        fm_xentry_unlock(pt_dev_xent_tree[i4_xent_idx]);
        i4_xent_idx++;
    }

    MFW_UNLOCK_AFTER_CREATE();

    t_msg.e_type = FM_MFW_MSG_ASYNC_ANLS_CRET_DONE;
    t_msg.u.h_req = h_req;
    
    i4_ret = x_msg_q_send(gh_mfw_msg_q, 
                          (const VOID *) &t_msg, 
                          sizeof(FM_MFW_MSG_T),
                          FM_MFW_NFY_REQ_PRIORITY);
    
    if (i4_ret != OSR_OK)
    {
        DBG_INFO(("_fm_mfw_async_done: send msg failed.\n"));
    }

    x_thread_exit();
}


static VOID _fm_mfw_async_done(
    HANDLE_T        h_req,
    VOID*           pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data)
{
    INT32 i4_ret;
    FM_MFW_MSG_T t_msg;
    FM_MFW_ASYNC_RW_TAG_T *pt_arg;
    
    ASSERT(pv_tag != NULL);

    pt_arg = (FM_MFW_ASYNC_RW_TAG_T *)pv_tag;
    pt_arg->ui4_done_bytes += ui4_data;

    MFW_TIME_STAMP(("Async REQ(0x%x) done!", pt_arg->h_rw_req));

    switch (e_async_cond)
    {
        case FM_ASYNC_COND_ABORT_OK:
            pt_arg->ui4_req_ok_cnt++;
            break;
        case FM_ASYNC_COND_READ_OK:
            pt_arg->ui4_req_ok_cnt++;
            break;
        case FM_ASYNC_COND_WRITE_OK:
            pt_arg->ui4_req_ok_cnt++;
            break;
        case FM_ASYNC_COND_FAIL:
        case FM_ASYNC_COND_BLANK_SECTOR:
        case FM_ASYNC_COND_CMD_TIMEOUT:
            pt_arg->ui4_req_fail_cnt++;
            break;
        case FM_ASYNC_COND_ABORT_FAIL:
            pt_arg->ui4_req_fail_cnt++;
            break;
        default:
            ASSERT(0);
            break;

    }

    if ((pt_arg->b_all_req_sent == TRUE) &&
        (pt_arg->ui4_req_cnt == (pt_arg->ui4_req_ok_cnt + pt_arg->ui4_req_fail_cnt)))
    {
        if (pt_arg->ui4_req_fail_cnt == 0)
        {
            if (pt_arg->pf_nfy_fct)
            {
                if (pt_arg->ui4_done_bytes < pt_arg->ui4_count)
                {
                    pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                       pt_arg->pv_tag, 
                                       FM_ASYNC_COND_ABORT_OK, 
                                       pt_arg->ui4_done_bytes);
                }
                else
                {
                    if (pt_arg->b_aborted == TRUE)
                    {
                        pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                           pt_arg->pv_tag, 
                                           FM_ASYNC_COND_ABORT_FAIL, 
                                           pt_arg->ui4_done_bytes);
                    }
                    else
                    {
                        pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                           pt_arg->pv_tag, 
                                           e_async_cond, 
                                           pt_arg->ui4_done_bytes);
                    }
                }
            }
            t_msg.e_type = FM_MFW_MSG_ASYNC_RW_REQ_DONE;
            t_msg.u.pt_arw_tag = pt_arg;
            
            i4_ret = x_msg_q_send(gh_mfw_msg_q, 
                                  (const VOID *) &t_msg, 
                                  sizeof(FM_MFW_MSG_T),
                                  FM_MFW_NFY_REQ_PRIORITY);
            
            if (i4_ret != OSR_OK)
            {
                DBG_ERROR(("_fm_mfw_async_done: send msg failed.\n"));
            }
        }
        else
        {
            if (pt_arg->ui4_done_bytes < pt_arg->ui4_count)
            {
                pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                   pt_arg->pv_tag, 
                                   e_async_cond, 
                                   pt_arg->ui4_done_bytes);
            }

            
            t_msg.e_type = FM_MFW_MSG_ASYNC_RW_REQ_DONE;
            t_msg.u.pt_arw_tag = pt_arg;
            
            i4_ret = x_msg_q_send(gh_mfw_msg_q, 
                                  (const VOID *) &t_msg, 
                                  sizeof(FM_MFW_MSG_T),
                                  FM_MFW_NFY_REQ_PRIORITY);
            
            if (i4_ret != OSR_OK)
            {
                DBG_ERROR(("_fm_mfw_async_done: send msg failed.\n"));
            }
        }
    }
}

static INT32 _fm_mfw_rebuild_mfw(
    FM_MFW_INFO_T* pt_mfw_info)
{
    INT32 i4_ret;
    UINT32 ui4_i;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    UINT64 ui8_start_pos;
    FM_FILE_INFO_T      t_info;

    if (pt_mfw_info == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if ((pt_mfw_info->e_meta_policy > FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE) ||
        (pt_mfw_info->ps_dir == NULL) ||
        (pt_mfw_info->ps_fn_pattern == NULL) ||
        (pt_mfw_info->ps_meta_full_path == NULL))
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    
    /* allocate new MFW entry */
    pt_mfw_ent = _fm_mfw_alloc_mfw_entry( pt_mfw_info->e_meta_policy,
                                          pt_mfw_info->ps_dir,
                                          pt_mfw_info->ps_fn_pattern,
                                          pt_mfw_info->ps_meta_full_path);
    if (pt_mfw_ent == NULL)
    {
        i4_ret = FMR_CORE;
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        goto EXIT;
    }
    
    /* Lock it */
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        goto EXIT;
    }
    
    ui8_start_pos = 0;
    /* Rebuild MFDC */
    for (ui4_i = 0; ui4_i < pt_mfw_info->ui4_file_num; ui4_i++)
    {                    
        CHAR* ps_sub_path = NULL;
        
        ps_sub_path = _fm_mfw_get_sub_file_path(
                            pt_mfw_info->ps_dir,
                            pt_mfw_info->ps_fn_pattern,
                            ui4_i);
        if (ps_sub_path == NULL)
        {
            i4_ret = FMR_CORE;
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }

        i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_sub_path, &t_info);
        x_mem_free(ps_sub_path);
        ps_sub_path = NULL;
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            i4_ret = FMR_CORE;
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            goto EXIT;
        }

        pt_sgl_fd = x_mem_alloc(sizeof(FM_MFW_SINGLE_FILE_DESC_T));
        if (pt_sgl_fd == NULL)
        {
            i4_ret = FMR_CORE;
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }

        pt_sgl_fd->ui4_id = ui4_i;
        pt_sgl_fd->ui8_start_pos = ui8_start_pos;
        pt_sgl_fd->ui8_end_pos = ui8_start_pos+t_info.ui8_size;
        ui8_start_pos = pt_sgl_fd->ui8_end_pos;

        pt_mfw_ent->ui8_user_size = pt_sgl_fd->ui8_end_pos;

        DLIST_INSERT_TAIL(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link); 
    }  
    
    pt_mfw_ent->t_mfw_info.ui4_file_num = ui4_i;

    /* Add the MFW entry into global entry list */
    MFW_LOCK_ENTRY_LIST();
    
    DLIST_INSERT_TAIL(pt_mfw_ent, &gt_mfw_entry_q, t_link);
    
    MFW_UNLOCK_ENTRY_LIST();

    MFW_UNLOCK_ENTRY(pt_mfw_ent);
 
EXIT:
    if (i4_ret != FMR_OK)
    {
        if (pt_mfw_ent != NULL)
        {
            while ((pt_sgl_fd = DLIST_HEAD(&pt_mfw_ent->t_mfdc)) != NULL)
            {
                DLIST_REMOVE(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);

                x_mem_free(pt_sgl_fd);
                pt_sgl_fd = NULL;
            }
            
            _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);
            x_sema_delete(pt_mfw_ent->h_mtx);
            x_mem_free(pt_mfw_ent);
        }
    }

    return i4_ret;
}

/*
   Check if a file is a wrapper, and if the arapper is OK, then get sub file 
   number if necessary. 
*/
static INT32 _fm_mfw_check_ex(
    HANDLE_T                   h_dir,
    const CHAR*                ps_path,
    BOOL*                      pb_mfw,
    BOOL*                      pb_ok,
    FM_MFW_INFO_T*             pt_mfw_info)
{
    INT32  i4_ret;
    UINT32 ui4_file_num;
    UINT32 ui4_meta_size;
    UINT32 ui4_ver;
    UINT8* pui1_meta = NULL;
    CHAR*  ps_name = NULL;
    CHAR*  ps_fn_pattern = NULL;
    CHAR*  ps_dir = NULL;
    FM_DIR_LABEL_T* pt_dir_lbl = NULL;
    UINT32 ui4_i;
    BOOL   b_visible_meta;
    UINT8  ui1_fn_pattern_len;
    FM_LOOKUP_ARG_T t_lookup_arg;
  
    if(ps_path == NULL || pb_mfw == NULL || pb_ok == NULL)
    {
        return FMR_ARG;
    }

    *pb_mfw = FALSE;
    *pb_ok = FALSE;
    i4_ret = FMR_OK;
    x_memset(pt_mfw_info, 0, sizeof(FM_MFW_INFO_T));

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }
    
    ps_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    if (ps_name == NULL)
    {
        i4_ret = FMR_CORE;
        goto EXIT;
    }
    
    pui1_meta = x_mem_alloc(MFW_META_SIZE);
    if (pui1_meta == NULL)
    {
        i4_ret = FMR_CORE;
        goto EXIT;
    }

    i4_ret = _fm_mfw_read_mfw_meta(h_dir, ps_path, (VOID*)pui1_meta);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        goto EXIT;
    }

    /* check signature bytes */
    if (pui1_meta[MFW_META_START_SIGN_FIELD+0] != g_aui1_sign_start[0] ||
        pui1_meta[MFW_META_START_SIGN_FIELD+1] != g_aui1_sign_start[1] ||
        pui1_meta[MFW_META_START_SIGN_FIELD+2] != g_aui1_sign_start[2] ||
        pui1_meta[MFW_META_START_SIGN_FIELD+3] != g_aui1_sign_start[3] ||
        pui1_meta[MFW_META_START_SIGN_FIELD+4] != g_aui1_sign_start[4] ||
        pui1_meta[MFW_META_START_SIGN_FIELD+5] != g_aui1_sign_start[5])
    {
        i4_ret = FMR_OK;
        goto EXIT;
    }

    /* check signature bytes */
    if (pui1_meta[MFW_META_END_SIGN_FIELD+0] != g_aui1_sign_end[0] ||
        pui1_meta[MFW_META_END_SIGN_FIELD+1] != g_aui1_sign_end[1] ||
        pui1_meta[MFW_META_END_SIGN_FIELD+2] != g_aui1_sign_end[2] ||
        pui1_meta[MFW_META_END_SIGN_FIELD+3] != g_aui1_sign_end[3] ||
        pui1_meta[MFW_META_END_SIGN_FIELD+4] != g_aui1_sign_end[4] ||
        pui1_meta[MFW_META_END_SIGN_FIELD+5] != g_aui1_sign_end[5])
    {
        i4_ret = FMR_OK;
        goto EXIT;
    }
    
    ui4_ver = MFW_GET_VAL32(pui1_meta, MFW_META_VERSION_FIELD);
    ui4_meta_size = MFW_GET_VAL32(pui1_meta, MFW_META_SIZE_FIELD);   
    ui1_fn_pattern_len = MFW_GET_VAL8(pui1_meta, MFW_SUB_FILE_NAME_PATTERN_LEN_FIELD);
    b_visible_meta = MFW_GET_VAL8(pui1_meta, MFW_META_VISIBLE_FLAG_FIELD);
    ui4_file_num = MFW_GET_VAL32(pui1_meta, MFW_SUB_FILE_NUM_FIELD);

    if ((ui4_ver > MFW_META_VERSION) || (ui4_meta_size != MFW_META_SIZE) || (ui1_fn_pattern_len == 0))
    {
        i4_ret = FMR_OK;
        goto EXIT;
    }
    
    /* it is a mfw file */
    *pb_mfw = TRUE;

    ps_fn_pattern = x_mem_alloc((SIZE_T)(ui1_fn_pattern_len + 1));
    if (ps_fn_pattern == NULL)
    {
        i4_ret = FMR_CORE;
        goto EXIT;
    }
    x_memset(ps_fn_pattern, 0, (SIZE_T)(ui1_fn_pattern_len + 1));
    x_memcpy(ps_fn_pattern, &pui1_meta[MFW_SUB_FILE_NAME_PATTERN_FIELD], ui1_fn_pattern_len);
    ps_fn_pattern[ui1_fn_pattern_len] = '\0';

    if (pt_dir_lbl != NULL)
    {
        SIZE_T z_len;
        
        z_len = x_strlen(pt_dir_lbl->ps_path) + 1;
        ps_dir = (CHAR*)x_mem_alloc(z_len);
        if (ps_dir == NULL)
        {
            i4_ret = FMR_CORE;
            goto EXIT;
        }
        x_memset(ps_dir, 0, z_len);
        x_strcpy(ps_dir, ps_path);
    }
    else
    {
        SIZE_T z_len;
        CHAR* pc_temp;

        z_len = x_strlen(ps_path) + 1;
        ps_dir = (CHAR*)x_mem_alloc(z_len);
        if (ps_dir == NULL)
        {
            i4_ret = FMR_CORE;
            goto EXIT;
        }
        x_memset(ps_dir, 0, z_len);
        x_strcpy(ps_dir, ps_path);

        pc_temp = &(ps_dir[z_len - 1]);
        while ((*pc_temp == '/') && (pc_temp >= ps_dir))
        {
            /* skip the end '/' */
            *pc_temp = '\0';
            pc_temp--;
        }
        
        pc_temp = x_strrchr(ps_dir, '/');
        if (pc_temp == NULL)
        {
            i4_ret = FMR_CORE;
            goto EXIT;
        }
        else
        {
            pc_temp++;
            *pc_temp = '\0';
        }
    }

    for (ui4_i = 0; ui4_i < ui4_file_num; ui4_i++)
    {
        CHAR* ps_sub_path = NULL;
        
        ps_sub_path = _fm_mfw_get_sub_file_path(
            ps_dir,
            ps_fn_pattern,
            ui4_i);
        if (ps_sub_path == NULL)
        {
            i4_ret = FMR_CORE;
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }

        t_lookup_arg.ps_name = ps_name;
        t_lookup_arg.ui8_lookup_ofst = FM_LOOKUP_NO_HINT;

        i4_ret = fm_ufs_lookup(NULL,
                               ps_sub_path,
                               FM_UFS_CREATE,
                               FM_UFS_LOCK_LEAF|FM_UFS_LOCK_PARENT,
                               &t_lookup_arg);
        x_mem_free(ps_sub_path);
        ps_sub_path = NULL;

        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }        
        else
        {
            if (t_lookup_arg.pt_xent != NULL)
            {
                /* A sub file is found */
                fm_xentry_unlock(t_lookup_arg.pt_xent);
                fm_xentry_unlock(t_lookup_arg.pt_prnt_xent);
            }
            else
            {
                /* A sub file is lost */
                fm_xentry_unlock(t_lookup_arg.pt_prnt_xent);
                goto EXIT1;
            }
        }
    }
    
    /* Everything is OK */
    *pb_ok = TRUE;
    
EXIT1:
    if (pt_mfw_info != NULL)
    {
        /* caller should free these buffer after using */
        pt_mfw_info->e_meta_policy = (b_visible_meta == TRUE) ? (FM_MFW_VF_META_POLICY_VISIBLE_META_FILE) : (FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE);
        pt_mfw_info->ui4_file_num = ui4_file_num;
        pt_mfw_info->ps_dir = x_mem_alloc(x_strlen(ps_dir)+1);
        if (pt_mfw_info->ps_dir != NULL)
        {
            x_strcpy(pt_mfw_info->ps_dir, ps_dir);
        }
        pt_mfw_info->ps_fn_pattern = x_mem_alloc(x_strlen(ps_fn_pattern)+1);
        if (pt_mfw_info->ps_fn_pattern != NULL)
        {
            x_strcpy(pt_mfw_info->ps_fn_pattern, ps_fn_pattern);
        }
        if (h_dir == FM_ROOT_HANDLE)
        {
            pt_mfw_info->ps_meta_full_path = (CHAR*)x_mem_alloc(x_strlen(ps_path)+1);
            if (pt_mfw_info->ps_meta_full_path != NULL)
            {
                x_strcpy(pt_mfw_info->ps_meta_full_path, ps_path);
            }
            
        }
        else
        {
            pt_mfw_info->ps_meta_full_path = _fm_mfw_splice_full_path_ex(h_dir, NULL, ps_path);
        }
        
    }


EXIT:
    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }
    
    if (ps_name != NULL)
    {
        x_mem_free(ps_name);
    }

    if (pui1_meta != NULL)
    {
        x_mem_free(pui1_meta);
    }

    if (ps_dir != NULL)
    {
        x_mem_free(ps_dir);
    }

    if (ps_fn_pattern != NULL)
    {
        x_mem_free(ps_fn_pattern);
    }
    
    return i4_ret;
}

static VOID _fm_mfw_dm_nfy_fct(
    DM_EVT_T t_evt, 
    VOID*    pv_data, 
    VOID*    pv_tag)
{
    INT32 i4_ret;
    
    switch(t_evt)
    {
        case DM_FS_EVT_UNMOUNTED:
            {
                FM_MFW_MSG_T t_msg;
                DM_FS_EVT_DATA_T* pt_fs_evt_data = NULL;

                pt_fs_evt_data = (DM_FS_EVT_DATA_T*)pv_data;
                
                if (pt_fs_evt_data != NULL)
                {
                    t_msg.e_type = FM_MFW_MSG_REMOVE_UMOUNTED_ENTRIES;
                    
                    x_memcpy( t_msg.u.ps_mnt_path, 
                              pt_fs_evt_data->ps_mnt_path,
                              DM_MAX_LABEL_NAME_LEN );
                    
                    i4_ret = x_msg_q_send(gh_mfw_msg_q, 
                                          (const VOID *) &t_msg, 
                                          sizeof(FM_MFW_MSG_T),
                                          FM_MFW_NFY_REQ_PRIORITY);
                    
                    if (i4_ret != OSR_OK)
                    {
                        DBG_ERROR(("_mfw_dm_nfy_fct: send msg failed.\n"));
                    }
                }
            }
            break;

        default:
            break;
    }
}



static VOID _fm_mfw_thread_main(
    VOID* pv_data)
{
#ifndef ANDROID
    FM_MFW_MSG_T t_msg;
    UINT16 ui2_q_idx;
    SIZE_T z_msg_size;
    DM_EVT_FILTR_T t_dm_evt_filtr;
    DM_COND_T t_dm_cond;
#ifndef LINUX_TURNKEY_SOLUTION
    static CHAR* ps_extending_path = NULL;
    static HANDLE_T h_extending_file = NULL_HANDLE;
    static CHAR* ps_writing_path = NULL;
    static HANDLE_T h_writing_file = NULL_HANDLE;
#endif
    INT32 i4_ret;

    /* Register DM FileSystem event listener */
    t_dm_cond.t_hw_type  = DM_HW_TYPE_USB;
    t_dm_cond.t_dev_type = DM_DEV_MED_TYPE_MASS_STRG;
    t_dm_evt_filtr.pt_evt_filtr = &t_dm_cond;
    t_dm_evt_filtr.ui4_evt_filtr_ns = 1;

    while (1)
    {
        /* It is a little tricky here. As FM would be(and should be) initialized before DM, we have
               to wait till DM is initialized.  */
        i4_ret = x_dm_reg_nfy_fct(DM_EVT_CAT_FS, 
                                  _fm_mfw_dm_nfy_fct, 
                                  &t_dm_evt_filtr,
                                  NULL,
                                  &gh_dm_nfy);
        if (i4_ret != DMR_OK)
        {
            if (i4_ret == DMR_NOT_INIT)
            {
                x_thread_delay(100);
                continue;
            }
            DBG_ERROR(("[FM-MFW] %s #%d:Failed to register DM FileSystem event listener,Deinit MFW instance (%d)\n", __FUNCTION__,__LINE__, i4_ret));
            x_sema_delete(gh_ent_q_lock);
            x_sema_delete(gh_rel_req_lock);
            x_msg_q_delete(gh_mfw_msg_q);
            gb_mfw_init = FALSE;
            goto FAIL;
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        z_msg_size = sizeof(FM_MFW_MSG_T);

        i4_ret = x_msg_q_receive(&ui2_q_idx,
                                 &t_msg,
                                 &z_msg_size,
                                 &gh_mfw_msg_q,
                                 1,
                                 X_MSGQ_OPTION_WAIT);

        if (i4_ret != OSR_OK)
        {
            DBG_ERROR(("_fm_mfw_thread_main receive msg error."));
            goto FAIL;
        }

        switch(t_msg.e_type)
        {
            case FM_MFW_MSG_ASYNC_RW_REQ_DONE:
                {
                    FM_MFW_ASYNC_RW_TAG_T* pt_rw_tag = NULL;

                    pt_rw_tag = t_msg.u.pt_arw_tag;

                    i4_ret = x_sema_lock(gh_rel_req_lock, X_SEMA_OPTION_WAIT);
                    if (i4_ret != OSR_OK)
                    {
                        goto FAIL;
                    }
                    /* Can we reuse the request handle here? */
                    handle_free(pt_rw_tag->h_rw_req, FALSE);

                    x_sema_unlock(gh_rel_req_lock);

                    pt_rw_tag->h_rw_req = NULL_HANDLE;

                    MFW_LOCK_ARW_TAG_LIST();

                    DLIST_INSERT_TAIL(pt_rw_tag, &gt_mfw_arw_tag_q, t_link);

                    MFW_UNLOCK_ARW_TAG_LIST();
                }
                break;
                
            case FM_MFW_MSG_ASYNC_ANLS_CRET_DONE:
                {
                    i4_ret = x_sema_lock(gh_rel_req_lock, X_SEMA_OPTION_WAIT);
                    if (i4_ret != OSR_OK)
                    {
                        goto FAIL;
                    }

                    x_handle_free(t_msg.u.h_req);

                    x_sema_unlock(gh_rel_req_lock);
                }
                break;

            case FM_MFW_MSG_REMOVE_UMOUNTED_ENTRIES:
                {
                    FM_MFW_ENTRY_T*   pt_mfw_ent = NULL;
                    
                    #ifndef LINUX_TURNKEY_SOLUTION
                    if (ps_extending_path != NULL)
                    {
                        x_mem_free(ps_extending_path);
                        ps_extending_path = NULL;
                    }
                    
                    if (h_extending_file != NULL_HANDLE)
                    {
                        x_fm_close(h_extending_file);
                    }
                    
                    if (ps_writing_path != NULL)
                    {
                        x_mem_free(ps_writing_path);
                        ps_writing_path = NULL;
                    }

                    if (h_writing_file != NULL_HANDLE)
                    {
                        x_fm_close(h_writing_file);
                    }
                    #endif
                    MFW_LOCK_ENTRY_LIST();
                    
                    DLIST_FOR_EACH(pt_mfw_ent, &gt_mfw_entry_q, t_link)
                    {
                        if (NULL != x_strstr(pt_mfw_ent->t_mfw_info.ps_meta_full_path, t_msg.u.ps_mnt_path))
                        {
                            if (MFW_LOCK_ENTRY(pt_mfw_ent) == OSR_OK)
                            {
                                if (pt_mfw_ent->ui4_ref_cnt > 0)
                                {
                                    pt_mfw_ent->ui4_status |= FM_MFW_ENTRY_UNLINKED;
                                }
                                else
                                {
                                    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
                                    
                                    /* release MFDC */
                                    while ((pt_sgl_fd = DLIST_HEAD(&pt_mfw_ent->t_mfdc)) != NULL)
                                    {
                                        DLIST_REMOVE(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
                                    
                                        x_mem_free(pt_sgl_fd);
                                        pt_sgl_fd = NULL;
                                    }
                                    
                                    _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);

                                    pt_mfw_ent->ui8_user_size = 0;
                                    
                                    pt_mfw_ent->t_mfw_info.ui4_file_num = 0;
                                    
                                    pt_mfw_ent->ui4_status = FM_MFW_ENTRY_FREED;
                                }

                                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                            }
                        }
                    }
                    
                    MFW_UNLOCK_ENTRY_LIST();
                }
                break;

            case FM_MFW_MSG_EXTEND_SUB_FILE:
            #ifndef LINUX_TURNKEY_SOLUTION
            {
                /* Extend the same sub file, unnecessary to open file again */
                if (!x_strcmp(ps_extending_path, t_msg.u.t_extend_sub_file.ps_sub_file_path))
                {
                    x_mem_free(t_msg.u.t_extend_sub_file.ps_sub_file_path);
                    t_msg.u.t_extend_sub_file.ps_sub_file_path = NULL;
                }
                else
                {
                    if (ps_extending_path != NULL)
                    {
                        x_mem_free(ps_extending_path);
                        ps_extending_path = NULL;
                    }

                    ps_extending_path = t_msg.u.t_extend_sub_file.ps_sub_file_path;

                    if (h_extending_file != NULL_HANDLE)
                    {
                        i4_ret = x_fm_close(h_extending_file);
                        if (i4_ret != FMR_OK)
                        {
                            x_mem_free(ps_extending_path);
                            ps_extending_path = NULL;
                            h_extending_file = NULL_HANDLE;
                            DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                            break;
                        }
                    }

                    h_extending_file = NULL_HANDLE;
                   
                    i4_ret = x_fm_open(
                        FM_ROOT_HANDLE, 
                        ps_extending_path, 
                        t_msg.u.t_extend_sub_file.ui4_flags, 
                        0777, 
                        TRUE, 
                        &h_extending_file);

                    if (i4_ret != FMR_OK)
                    {
                        x_mem_free(ps_extending_path);
                        ps_extending_path = NULL;
                        h_extending_file = NULL_HANDLE;
                        
                        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                        break;
                    }
                }

                /* Note: In FAT, if multi threads come to extend file, it may cause lots of fragments. */
                MFW_TIME_STAMP(("Before extending file"));
                i4_ret = x_fm_extend_file(h_extending_file, t_msg.u.t_extend_sub_file.i8_length);
                MFW_TIME_STAMP(("After extending file"));
                if (i4_ret != FMR_OK)
                { 
                    DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                    break;
                }
            }
            #endif
            break;

            case FM_MFW_MSG_ASYNC_WRITE_SUB_FILE:
            #ifndef LINUX_TURNKEY_SOLUTION
            {
                FM_MFW_ASYNC_WRITE_SUB_FILE_T t_aw;
                UINT64 ui8_cur_pos;

                x_memcpy(&t_aw, &t_msg.u.t_async_write_sub_file, sizeof(FM_MFW_ASYNC_WRITE_SUB_FILE_T));

                /* Write the same sub file, unnecessary to open file again */
                if (!x_strcmp(ps_writing_path, t_aw.ps_sub_file_path))
                {
                    x_mem_free(t_aw.ps_sub_file_path);
                    t_aw.ps_sub_file_path = NULL;
                }
                else
                {
                    if (ps_writing_path != NULL)
                    {
                        x_mem_free(ps_writing_path);
                        ps_writing_path = NULL;
                    }

                    ps_writing_path = t_aw.ps_sub_file_path;

                    if (h_writing_file != NULL_HANDLE)
                    {
                        i4_ret = x_fm_close(h_writing_file);
                        if (i4_ret != FMR_OK)
                        {
                            x_mem_free(ps_writing_path);
                            ps_writing_path = NULL;
                            h_writing_file = NULL_HANDLE;
                            DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                            break;
                        }
                    }

                    h_writing_file = NULL_HANDLE;
                   
                    i4_ret = x_fm_open(
                        FM_ROOT_HANDLE, 
                        ps_writing_path, 
                        t_aw.ui4_flags, 
                        t_aw.ui4_mode, 
                        t_aw.b_ext_buf, 
                        &h_writing_file);

                    if (i4_ret != FMR_OK)
                    {
                        x_mem_free(ps_writing_path);                        
                        ps_writing_path = NULL;
                        h_writing_file = NULL_HANDLE;
                        
                        DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                        break;
                    }
                }
                
                i4_ret = x_fm_lseek(h_writing_file, (INT64)t_aw.ui8_write_ofst, FM_SEEK_BGN, &ui8_cur_pos);
                if (i4_ret != FMR_OK)
                {
                    DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                    break;
                }
                #if 1
                
                MFW_TIME_STAMP(("Before async writing file"));
                i4_ret = x_fm_write_async(
                    h_writing_file,
                    t_aw.pv_data,
                    t_aw.ui4_count,
                    t_aw.ui1_pri,
                    t_aw.pf_nfy_fct,
                    t_aw.pv_tag,
                    t_aw.ph_req);
                
                MFW_TIME_STAMP(("After async writing file(REQ:%x)", ((FM_MFW_ASYNC_RW_TAG_T *)t_aw.pv_tag)->h_rw_req));
                if (i4_ret != FMR_OK)
                {
                    DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                    break;
                }
                #else
                {
                    UINT32 ui4_written = 0;
                    FM_MFW_ASYNC_RW_TAG_T* pt_arg = NULL;
                    
                    pt_arg = (FM_MFW_ASYNC_RW_TAG_T *)t_aw.pv_tag;
                    
                    i4_ret = x_fm_write(h_file, t_aw.pv_data, t_aw.ui4_count, &ui4_written);
                    if (i4_ret == FMR_OK)
                    {
                        pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                        pt_arg->pv_tag, 
                                        FM_ASYNC_COND_WRITE_OK, 
                                        ui4_written);
                    }
                    else
                    {
                        pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                        pt_arg->pv_tag, 
                                        FM_ASYNC_COND_FAIL, 
                                        0);
                    }

                    
                    x_sema_lock(gh_rel_req_lock, X_SEMA_OPTION_WAIT);
                    /* Can we reuse the request handle here? */
                    handle_free(pt_arg->h_rw_req, FALSE);

                    x_sema_unlock(gh_rel_req_lock);

                    pt_arg->h_rw_req = NULL_HANDLE;

                    MFW_LOCK_ARW_TAG_LIST();

                    DLIST_INSERT_TAIL(pt_arg, &gt_mfw_arw_tag_q, t_link);

                    MFW_UNLOCK_ARW_TAG_LIST();
                    
                }
                #endif
            }
            #endif
            break;

            default:
                break;
        }

    }

FAIL:

    x_thread_exit();
#endif
}





/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_init
 *
 * Description: Init Multi File Wrapper
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_init(VOID)
{
    INT32 i4_ret;
    
    if (gb_mfw_init == TRUE)
    {
        return FMR_OK;
    }

    i4_ret = x_sema_create(
        &gh_ent_q_lock, 
        X_SEMA_TYPE_BINARY, 
        X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        return FMR_CORE;
    }
    
    i4_ret = x_sema_create(
        &gh_arw_tag_q_lock, 
        X_SEMA_TYPE_BINARY, 
        X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        x_sema_delete(gh_ent_q_lock);
        return FMR_CORE;
    }

    i4_ret = x_sema_create(
        &gh_rel_req_lock, 
        X_SEMA_TYPE_BINARY, 
        X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        x_sema_delete(gh_ent_q_lock);
        x_sema_delete(gh_arw_tag_q_lock);
        return FMR_CORE;
    }

    i4_ret = x_sema_create(
        &gh_mfw_cret_close_lock, 
        X_SEMA_TYPE_BINARY, 
        X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        x_sema_delete(gh_ent_q_lock);
        x_sema_delete(gh_arw_tag_q_lock);
        x_sema_delete(gh_rel_req_lock);
        return FMR_CORE;
    }
    
    i4_ret = x_msg_q_create(&gh_mfw_msg_q,
                            FM_MFW_MSG_Q_NAME,
                            sizeof(FM_MFW_MSG_T),
                            FM_MFW_MSG_Q_NUM);

    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        x_sema_delete(gh_ent_q_lock);
        x_sema_delete(gh_arw_tag_q_lock);
        x_sema_delete(gh_rel_req_lock);
        x_sema_delete(gh_mfw_cret_close_lock);
        return FMR_CORE;
    }

    /* Create a thread for analysing */
    i4_ret = x_thread_create(&gh_mfw_thrd, 
                             FM_MFW_MSG_THREAD_NAME, 
                             FM_MFW_MSG_THREAD_DEFAULT_STACK_SIZE,
                             FM_MFW_MSG_THREAD_DEFAULT_PRIORITY, 
                             _fm_mfw_thread_main,
                             0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Create a analysing thread failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        x_sema_delete(gh_ent_q_lock);
        x_sema_delete(gh_arw_tag_q_lock);
        x_sema_delete(gh_rel_req_lock);
        x_sema_delete(gh_mfw_cret_close_lock);
        x_msg_q_delete(gh_mfw_msg_q);
        return FMR_CORE;
    }

    DLIST_INIT(&gt_mfw_entry_q);
    DLIST_INIT(&gt_mfw_arw_tag_q);

    gb_mfw_init = TRUE;

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_analyse
 *
 * Description: Analyse a volume and get the size of all the continuous region.
 *              NOTE: 
 *              It is an asynchronous function, and the result will be passed
 *              to caller in the notification(FM_MFW_ASYNC_COND_ANLS_OK, 
 *              FM_MFW_ASYNC_COND_ANLS_FAIL).
 *              A special request handle shall be returned to caller and caller
 *              can cancel it via the handle.
 *
 * Inputs:  pt_param
 *              Analyse parameters.
 *
 * Outputs: ph_req
 *              Handle of this analyse request.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pt_param is NULL or ph_req is NULL.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_analyse(
    const FM_MFW_ANLS_PARAM_T* pt_param,
    HANDLE_T*                  ph_req)
{
    INT32 i4_ret;
    FM_MFW_ANLS_CRET_OBJ_T* pt_anls_obj;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    /* check parameters */
    if (pt_param == NULL || ph_req == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    if (pt_param->ui1_strct_ver != 0 ||
        pt_param->s_mp_path == NULL ||
        pt_param->pf_nfy == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        
        return FMR_ARG;
    }

    pt_anls_obj = x_mem_calloc(1, sizeof(FM_MFW_ANLS_CRET_OBJ_T));
    if (pt_anls_obj == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        
        return FMR_CORE;
    }

    /* Allocate a request handle for analysing */
    i4_ret = handle_alloc((HANDLE_TYPE_T)FMT_MFW_ASYNC_ANLS_CRET,
                          pt_anls_obj,
                          NULL,
                          _fm_mfw_handle_free_fct,
                          ph_req);
    if (i4_ret != HR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Allocate a analysing handle failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        x_mem_free(pt_anls_obj);
        return FMR_HANDLE;
    }

    /* Allocate a sema lock */
    i4_ret = x_sema_create(&pt_anls_obj->h_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Allocate a sema lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        x_handle_free(*ph_req);
        return FMR_HANDLE;
    }
    
    pt_anls_obj->b_cret = FALSE;
    pt_anls_obj->b_abort = FALSE;
    pt_anls_obj->h_req = *ph_req;
    pt_anls_obj->t_anls = *pt_param;
    pt_anls_obj->ui1_state = FM_MFW_INIT;
    /* alloc string buffer */
    pt_anls_obj->t_anls.s_mp_path= (CHAR*)x_mem_alloc(x_strlen(pt_param->s_mp_path)+1);
    if (pt_anls_obj->t_anls.s_mp_path== NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        x_handle_free(*ph_req);
        return FMR_CORE;
    }
    
    /* Don't forget free that */
    x_strcpy(pt_anls_obj->t_anls.s_mp_path, pt_param->s_mp_path);

    /* Create a thread for analysing */
    i4_ret = x_thread_create(&pt_anls_obj->h_thread, 
                             "MFW_ANLS", 
                             4096,
                             128, 
                             _fm_mfw_anls,
                             sizeof(HANDLE_T),
                             (VOID *)ph_req);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Create a analysing thread failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        x_handle_free(*ph_req);
        return FMR_CORE;
    }

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_create
  *
  * Description: Create a wrapper file according to the configs. This is a asynchronous
  *              a function, and it will analyse the device to get the available space 
  *              and the continuous space first, and then do the creating things. Wrapper
  *              will inform the caller of the analysing progress in phase1 and creating
  *              progress in phase2. When the creating is done, inform caller of the 
  *              result. After that, wrapper will free the handle itself.
  * 
  * Input:  pt_cfg
  *             Configs for creating.
  *
  * Output: h_req
  *             Handle of the creating request.
  *
  * Return: FMR_OK
  *                 Success!
  *         FMR_ARG
  *                 pt_cfg is NULL or ph_req is NULL or pt_cfg has invalid member
  *                 varialbes.
  *         FMR_CORE
  *                 Internal error.
  *
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_create(
    const FM_MFW_VF_CFG_T*     pt_cfg,
    HANDLE_T*                  ph_req)
{
    INT32 i4_ret;
    FM_MFW_ANLS_CRET_OBJ_T* pt_cret_obj;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    /* check parameters */
    if (pt_cfg == NULL || ph_req == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    if ((pt_cfg->ui1_strct_ver != 0) ||
        (pt_cfg->s_dir == NULL) ||
        (pt_cfg->s_fn_pattern == NULL) ||
        (pt_cfg->pf_nfy == NULL) ||
        (pt_cfg->e_vf_policy > FM_MFW_VF_POLICY_PHYSIC_CONTINS_PREFER) ||
        (pt_cfg->e_meta_policy > FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE) ||
        ((pt_cfg->e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE) && (pt_cfg->s_meta_file == NULL)))
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    
    pt_cret_obj = x_mem_calloc(1, sizeof(FM_MFW_ANLS_CRET_OBJ_T));
    if (pt_cret_obj == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    /* Allocate a request handle for analysing */
    i4_ret = handle_alloc((HANDLE_TYPE_T)FMT_MFW_ASYNC_ANLS_CRET,
                          pt_cret_obj,
                          NULL,
                          _fm_mfw_handle_free_fct,
                          ph_req);
    if (i4_ret != HR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Allocate a creating handle failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        x_mem_free(pt_cret_obj);
        return FMR_HANDLE;
    }

    /* Allocate a sema lock */
    i4_ret = x_sema_create(&pt_cret_obj->h_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Allocate a sema lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        x_handle_free(*ph_req);
        return FMR_HANDLE;
    }
    pt_cret_obj->b_cret = TRUE;
    pt_cret_obj->b_abort = FALSE;
    pt_cret_obj->h_req = *ph_req;
    pt_cret_obj->t_vf_cfg = *pt_cfg;
    pt_cret_obj->ui1_state = FM_MFW_INIT;

    /* alloc string buffer */
    pt_cret_obj->t_vf_cfg.s_dir = (CHAR*)x_mem_alloc(x_strlen(pt_cfg->s_dir)+1);
    if (pt_cret_obj->t_vf_cfg.s_dir == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        x_handle_free(*ph_req);
        return FMR_CORE;
    }
    x_strcpy(pt_cret_obj->t_vf_cfg.s_dir, pt_cfg->s_dir);

    pt_cret_obj->t_vf_cfg.s_fn_pattern = (CHAR*)x_mem_alloc(x_strlen(pt_cfg->s_fn_pattern)+1);
    if (pt_cret_obj->t_vf_cfg.s_fn_pattern == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        x_handle_free(*ph_req);
        return FMR_CORE;
    }
    x_strcpy(pt_cret_obj->t_vf_cfg.s_fn_pattern, pt_cfg->s_fn_pattern);

    if (pt_cfg->e_meta_policy == FM_MFW_VF_META_POLICY_VISIBLE_META_FILE)
    {
        pt_cret_obj->t_vf_cfg.s_meta_file = (CHAR*)x_mem_alloc(x_strlen(pt_cfg->s_meta_file)+1);
        if (pt_cret_obj->t_vf_cfg.s_meta_file == NULL)
        {
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
            x_handle_free(*ph_req);
            return FMR_CORE;
        }
        x_strcpy(pt_cret_obj->t_vf_cfg.s_meta_file, pt_cfg->s_meta_file);
    }
    else
    {
        pt_cret_obj->t_vf_cfg.s_meta_file = NULL;
    }
    
    /* Create a thread for analysing and creating*/
    i4_ret = x_thread_create(&pt_cret_obj->h_thread, 
                             "MFW_CRET", 
                             4096,
                             128, 
                             _fm_mfw_cret,
                             sizeof(HANDLE_T),
                             (VOID *)ph_req);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: Create a creating thread failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        x_handle_free(*ph_req);
        return FMR_CORE;
    }

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_delete
  *
  * Description: Delete a wrapper file.
  * 
  * Input: h_dir
  *             Handle of a directory label.
  *        ps_path
  *             Path name of the wrapper file to be deleted.
  *
  * Output: -
  *
  * Return: FMR_OK
  *                 Success!
  *         FMR_ARG
  *                 ps_path is NULL.
  *         FMR_CORE
  *                 Internal error.
  *
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_delete(
    HANDLE_T                   h_dir,
    const CHAR*                ps_path)
{
    INT32 i4_ret;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    BOOL b_mfw,b_ok;
    FM_MFW_INFO_T t_mfw_info;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    /* Check parameters */
    if (ps_path == NULL)
    {
        return FMR_ARG;
    }
    x_memset(&t_mfw_info, 0, sizeof(FM_MFW_INFO_T));
    /* Check the path first */
    i4_ret = _fm_mfw_check_ex(
        h_dir, 
        ps_path, 
        &b_mfw, 
        &b_ok,
        &t_mfw_info);

    if (i4_ret != FMR_OK)
    {  
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        return i4_ret;
    }

    if (b_mfw == TRUE)
    {       
        /* Find the entry if it is already existed in the global list */
        pt_mfw_ent = _fm_mfw_find_entry_by_path(t_mfw_info.ps_meta_full_path);

        if (pt_mfw_ent != NULL)
        {
            /* Found, lock it */
            i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
            if (i4_ret != OSR_OK)
            {
                i4_ret = FMR_CORE;
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                goto FAIL;
            }
            /* If it was referenced more than once, unlink it now */
            if (pt_mfw_ent->ui4_ref_cnt != 0)
            {
                pt_mfw_ent->ui4_status |= FM_MFW_ENTRY_UNLINKED;
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
            
                i4_ret = FMR_BUSY;
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                goto FAIL;
            }
            /* release MFDC */
            while ((pt_sgl_fd = DLIST_HEAD(&pt_mfw_ent->t_mfdc)) != NULL)
            {
                DLIST_REMOVE(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
            
                /*x_mem_free(pt_sgl_fd->ps_path);*/
                x_mem_free(pt_sgl_fd);
                pt_sgl_fd = NULL;
            }
            
            _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);

            pt_mfw_ent->ui8_user_size = 0;
            
            pt_mfw_ent->t_mfw_info.ui4_file_num = 0;
            
            pt_mfw_ent->ui4_status = FM_MFW_ENTRY_FREED;
            
            /* now it is a free entry, we can reused it in the future */
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
        }
        
        /* Now start to delete the wrapper */
        _fm_mfw_delete_wrapper(&t_mfw_info);
        
        _fm_mfw_free_buf_in_mfw_info(&t_mfw_info);
        /* Exit happy */
        return FMR_OK;

    FAIL:
        _fm_mfw_free_buf_in_mfw_info(&t_mfw_info);
        
        return i4_ret;
    }
    else
    {
        /* Not a wrapper, run normally */
        return x_fm_delete_file(h_dir, ps_path);
    }

}


/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_check
  *
  * Description: Check if the file is a wrapper file. If it is a wrapper file, 
  *              then check if all sub files in the wrapper are OK.
  * 
  * Input:  h_dir
  *             Handle of a directory label.
  *         ps_path
  *             Path name of the wrapper file to be deleted.
  *
  * Output: pb_mfw
  *             A wrapper file or not.
  *         pb_ok
  *             All sub files in the wrapper are OK or not.
  *
  * Return: FMR_OK
  *                 Success!
  *         FMR_ARG
  *                 ps_path is NULL.
  *         FMR_CORE
  *                 Internal error.
  *
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_check(
    HANDLE_T                   h_dir,
    const CHAR*                ps_path,
    BOOL*                      pb_mfw,
    BOOL*                      pb_ok)
{
    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }
    else
    {
        return _fm_mfw_check_ex(h_dir, ps_path, pb_mfw, pb_ok, NULL);
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_open
 *
 * Description: Create a multi-file wrapper, and return a mfw handle.
 *
 * Inputs:  a_ps_path
 *              File path array.
 *          ps_dev_path
 *              Entry number of the array.
 *
 * Outputs: ph_file
 *              Handle of the created multi-file wrapper
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              a_ps_path is NULL or any element in the array is NULL or
 *              ui1_ent_num is 0 or ph_file is NULL.
 *          FMR_NO_ENTRY
 *              Any element file in the array is not found.
 *          FMR_IS_DIR
 *              Any element file in the array is a directory.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_open (
    HANDLE_T                   h_dir,
    const CHAR*                ps_path,
    UINT32                     ui4_flags,
    UINT32                     ui4_mode,
    BOOL                       b_ext_buf,
    HANDLE_T*                  ph_file)
{
    INT32 i4_ret;
    CHAR*   ps_head_file = NULL;
    CHAR*   ps_meta_path = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    FM_MFW_OBJ_T* pt_mfw_obj = NULL;
    BOOL b_mfw,b_ok;
    UINT64 ui8_temp;
    FM_MFW_INFO_T t_mfw_info;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    /* Check parameters */
    if (ps_path == NULL || ph_file == NULL)
    {
        return FMR_ARG;
    }

    /* 
           If user want to create a wrapper, he must call x_fm_mfw_create();
           If user want to create a normal file, he must call x_fm_open(...,FM_OPEN_CREATE,...);
       */
    if (ui4_flags & FM_OPEN_CREATE)
    {
        return FMR_INVAL;
    }
    
    x_memset(&t_mfw_info, 0, sizeof(FM_MFW_INFO_T));

    i4_ret = _fm_mfw_check_ex(
        h_dir, 
        ps_path, 
        &b_mfw, 
        &b_ok,
        &t_mfw_info);

    if (i4_ret != FMR_OK)
    {
        _fm_mfw_free_buf_in_mfw_info(&t_mfw_info);
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        return i4_ret;
    }

    if (b_mfw == TRUE && b_ok == FALSE)
    {
        /* A broken wrapper */
        i4_ret = FMR_INVAL;
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        goto EXIT;
    }

    if (b_mfw == TRUE)
    {
        ps_meta_path = t_mfw_info.ps_meta_full_path;
        
        pt_mfw_ent = _fm_mfw_find_entry_by_path(ps_meta_path);
        if (pt_mfw_ent == NULL)
        {
            /* Rebuild the wrapper */
            i4_ret = _fm_mfw_rebuild_mfw(&t_mfw_info);
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                goto EXIT;
            }
            /* Try again */
            pt_mfw_ent = _fm_mfw_find_entry_by_path(ps_meta_path);
            ASSERT(pt_mfw_ent != NULL);
            if (pt_mfw_ent == NULL)
            {
                i4_ret = FMR_CORE;
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                goto EXIT;
            }
        }
        
        i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }
        /* Do not allow open a deleted wrapper */
        if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)    
        {
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            i4_ret = FMR_DELETING_ENTRY;
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }
        
        /* Allocate MFW object */
        pt_mfw_obj = _fm_mfw_alloc_mfw_obj(pt_mfw_ent);
        if (pt_mfw_obj == NULL)
        {
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            i4_ret =  FMR_CORE;
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }

        /* Allocate MFW handle */
        i4_ret = handle_alloc((HANDLE_TYPE_T)FMT_MFW_OBJ,
                              pt_mfw_obj,
                              NULL,
                              _fm_mfw_handle_free_fct,
                              ph_file);
        if (i4_ret != HR_OK)
        {            
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            i4_ret = FMR_HANDLE;
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }

        i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
        if (i4_ret != FMR_OK)
        {
            x_handle_free(*ph_file);
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            i4_ret = FMR_CORE;
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }

        pt_mfw_obj->ui4_flags = ui4_flags;
        pt_mfw_obj->ui4_mode = ui4_mode;
        pt_mfw_obj->b_ext_buf = b_ext_buf;
        pt_mfw_obj->ui4_cur_fid = 0;

        /* Open head file as current using sub file */
        ps_head_file = _fm_mfw_get_sub_file_path(
                            t_mfw_info.ps_dir,
                            t_mfw_info.ps_fn_pattern,
                            0);
        if (ps_head_file == NULL)
        {
            x_handle_free(*ph_file);
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            i4_ret = FMR_CORE;
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            goto EXIT;
        }
        
        i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                            ps_head_file, 
                            ui4_flags, 
                            ui4_mode, 
                            b_ext_buf, 
                            &pt_mfw_obj->h_cur_file );
        
        x_mem_free(ps_head_file);
        ps_head_file = NULL;
        
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("[FM-MFW]%s #%d: Open %s failed with (%d)\n",__FUNCTION__,__LINE__,ps_path,i4_ret));

            x_handle_free(*ph_file);
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            goto EXIT;
        }

        /* Skip the meta data */
        if (t_mfw_info.e_meta_policy == FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE)
        {
            i4_ret = x_fm_lseek( pt_mfw_obj->h_cur_file, 
                                 MFW_META_SIZE, 
                                 FM_SEEK_BGN, 
                                 &ui8_temp);
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("[FM-MFW]%s #%d: lseek %s failed with (%d)\n",__FUNCTION__,__LINE__,ps_path,i4_ret));

                x_handle_free(*ph_file);
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                goto EXIT;
            }

            pt_mfw_obj->ui8_cur_pos = MFW_META_SIZE;
        }
        else
        {
            pt_mfw_obj->ui8_cur_pos = 0;
        }
        
        pt_mfw_ent->ui4_ref_cnt++;
        pt_mfw_ent->ui4_status = FM_MFW_ENTRY_USING;

        _fm_mfw_obj_unlock(pt_mfw_obj);
        MFW_UNLOCK_ENTRY(pt_mfw_ent);

        DBG_INFO(("[FM-MFW]%s #%d: Exit happy!\n",__FUNCTION__,__LINE__));

   EXIT:
        _fm_mfw_free_buf_in_mfw_info(&t_mfw_info);

        return i4_ret; 
    }
    else
    {
        /* Normal file */
        i4_ret = x_fm_open( h_dir, 
                            ps_path, 
                            ui4_flags, 
                            ui4_mode, 
                            b_ext_buf, 
                            ph_file );

        return i4_ret;
    }
}

/* When reading and lseek in mfw file. The position may pass sub file end.
 * If the previous sub file is closed immediately, unfinished async request 
 * will be aborted. When Playback receive ABORTED_OK notify, it will stop play.
 * So we record previous file handle and closed when next file is opened or 
 * mfw file is closed. 
 */
INT32 x_fm_mfw_delay_close_current_file(FM_MFW_OBJ_T* pt_mfw_obj)
{
    INT32 ret;
    if (pt_mfw_obj->h_previous_file != NULL_HANDLE)
    {
        ret = x_fm_close(pt_mfw_obj->h_previous_file);
        if (ret != FMR_OK)
        {
             DBG_ERROR(("[FM-MFW]%s: Close previous file %s failed.(%d)\n"));
        }
    }
    pt_mfw_obj->h_previous_file = pt_mfw_obj->h_cur_file;
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_close
 *
 * Description: Close a multi-file wrapper.
 *
 * Inputs:  h_file
 *              Handle of the created multi-file wrapper
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_file is invalid.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_close(
    HANDLE_T    h_file)
{
    INT32   i4_ret;
    FM_MFW_OBJ_T* pt_mfw_obj = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    HANDLE_TYPE_T   e_obj;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    /* Check parameters */
    if (h_file == NULL_HANDLE)
    {
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    {
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        /* go normally */
        return x_fm_close(h_file);
    }
    else
    {
        MFW_LOCK_BEFORE_CLOSE();
        
        i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
        if (i4_ret != FMR_OK)
        {
            MFW_UNLOCK_AFTER_CLOSE();
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            return i4_ret;
        }
        pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
        
        i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);  
        if (i4_ret != FMR_OK)
        {
            _fm_mfw_obj_unlock(pt_mfw_obj);
            MFW_UNLOCK_AFTER_CLOSE();
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            return i4_ret;
        }

        pt_mfw_obj->ui4_status |= FM_MFW_OBJ_CLOSING;
        
        x_handle_free(h_file);
        
        pt_mfw_ent->ui4_ref_cnt--;

        /* Delete the unlinked entry if no one referenced it */
        if (pt_mfw_ent->ui4_ref_cnt == 0)
        {
            if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
            {            
                while ((pt_sgl_fd = DLIST_HEAD(&pt_mfw_ent->t_mfdc)) != NULL)
                {
                    DLIST_REMOVE(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);

                    x_mem_free(pt_sgl_fd);
                    pt_sgl_fd = NULL;
                }

                _fm_mfw_delete_wrapper(&pt_mfw_ent->t_mfw_info);

                _fm_mfw_free_buf_in_mfw_info(&pt_mfw_ent->t_mfw_info);

                pt_mfw_ent->ui8_user_size = 0;
                pt_mfw_ent->t_mfw_info.ui4_file_num = 0;
                pt_mfw_ent->ui4_status = FM_MFW_ENTRY_FREED;
            }
            else
            {
                /* [20091128, by Chun] Truncate the wrapper file*/
                while ((pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc)) != NULL)
                {
                    UINT32 ui4_id;
                    CHAR* ps_sub_file_path = NULL;

                    if (pt_sgl_fd->ui8_start_pos > pt_mfw_ent->ui8_user_size)
                    {
                        ui4_id = pt_sgl_fd->ui4_id;
                        
                        DLIST_REMOVE(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link);
                        
                        x_mem_free(pt_sgl_fd);
                        pt_sgl_fd = NULL;
                        
                        ps_sub_file_path = _fm_mfw_get_sub_file_path(
                                                pt_mfw_ent->t_mfw_info.ps_dir,
                                                pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                                                ui4_id);
                        if (ps_sub_file_path == NULL)
                        {
                            i4_ret = FMR_CORE; 
                            DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                        }
                        else
                        {
                            x_fm_delete_file(FM_ROOT_HANDLE ,ps_sub_file_path);

                            pt_mfw_ent->t_mfw_info.ui4_file_num--;
                            
                            i4_ret = _fm_mfw_write_mfw_meta(
                            pt_mfw_ent->t_mfw_info.e_meta_policy,
                            pt_mfw_ent->t_mfw_info.ps_meta_full_path, 
                            pt_mfw_ent->t_mfw_info.ps_fn_pattern, 
                            pt_mfw_ent->t_mfw_info.ui4_file_num,
                            FALSE);

                            //x_dbg_stmt("[FM-MFW]%s,%d, file number: %d, return value(%d)\n", __FUNCTION__, __LINE__, pt_mfw_ent->t_mfw_info.ui4_file_num, i4_ret);

                            x_mem_free(ps_sub_file_path);
                            ps_sub_file_path = NULL;
                        }
                    }
                    else if (pt_sgl_fd->ui8_end_pos > pt_mfw_ent->ui8_user_size)
                    {
                        ui4_id = pt_sgl_fd->ui4_id;
                        
                        ps_sub_file_path = _fm_mfw_get_sub_file_path(
                                                pt_mfw_ent->t_mfw_info.ps_dir,
                                                pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                                                ui4_id);
                        if (ps_sub_file_path == NULL)
                        {
                            i4_ret = FMR_CORE; 
                            DBG_ERROR(("[FM-MFW]%s,%d, return error(%d)\n", __FUNCTION__, __LINE__, i4_ret));
                        }
                        else
                        {
                            HANDLE_T h_sub_file;
                            
                            i4_ret = x_fm_open(
                                FM_ROOT_HANDLE, 
                                ps_sub_file_path, 
                                FM_READ_WRITE, 
                                0777, 
                                TRUE, 
                                &h_sub_file);
                            
                            x_mem_free(ps_sub_file_path);
                            ps_sub_file_path = NULL;
                            
                            if (i4_ret != FMR_OK)
                            {
                                /* if it failed, ingore it */
                                DBG_ERROR(("[FM-MFW]%s #%d: Truncate %s failed with (%d)\n",__FUNCTION__,__LINE__,ps_sub_file_path,i4_ret));

                                break;
                            }
                            
                            i4_ret = x_fm_trunc_by_handle(h_sub_file, (INT64)(pt_mfw_ent->ui8_user_size - pt_sgl_fd->ui8_start_pos));
                            if (i4_ret != FMR_OK)
                            {
                                /* if it failed, ingore it */
                                DBG_ERROR(("[FM-MFW]%s #%d: Truncate %s failed with (%d)\n",__FUNCTION__,__LINE__,ps_sub_file_path,i4_ret));
                            }

                            x_fm_close(h_sub_file);

                            pt_sgl_fd->ui8_end_pos = pt_mfw_ent->ui8_user_size;
                            
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

            }
        }

        /* Unlock entry, exit happy */
        MFW_UNLOCK_ENTRY(pt_mfw_ent);

        MFW_UNLOCK_AFTER_CLOSE();
        
        return FMR_OK;
    }

}


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_lseek
 *
 * Description: Change the current postion of the opened multi-file wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened multi-file wrapper.
 *          i8_offset
 *              Offset from ui4_whence in bytes. Positive number means move
 *              current position backward while negative number means move
 *              forward.
 *          ui1_whence
 *              FM_SEEK_BGN tells File Manager to move current position to
 *              i8_offset from the begging of the wrapper. if i8_offset is
 *              negative, current position is set to 0, that is the begging
 *              of wrapper.
 *              FM_SEEK_CUR tells File Manager to move current position to
 *              i8_offset from current position.
 *              FM_SEEK_END tells File Manager to move current position to
 *              i8_offset from the end of the wrapper. If i8_offset is a
 *              positive number, current position is set to the size of
 *              the file, that is the end of wrapper.
 *
 * Outputs: pui8_cur_pos
 *              Current position.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pui8_cur_pos is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed or ui1_whence is not defined.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_lseek(
    HANDLE_T        h_file,
    INT64           i8_offset,
    UINT8           ui1_whence,
    UINT64*         pui8_cur_pos)
{
    INT32           i4_ret = FMR_OK;
    FM_MFW_OBJ_T*   pt_mfw_obj = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    UINT64          ui8_relative_ofst;
    HANDLE_TYPE_T   e_obj;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    DBG_INFO(("[FM-MFW] %s: "
               "h_file %d, "
               "i8_offset %lld, "
               "ui1_whence %d\n",
               __FUNCTION__,
               h_file,
               i8_offset,
               ui1_whence));
    
    if (pui8_cur_pos == NULL || ui1_whence > FM_SEEK_END)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    { 
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_HANDLE));
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        return x_fm_lseek( h_file, 
                           i8_offset, 
                           ui1_whence, 
                           pui8_cur_pos );
    }

    i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        return i4_ret;
    }

    pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
    ASSERT(pt_mfw_ent != NULL);
    
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != OSR_OK)
    {
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        return FMR_CORE;
    }
    
    if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_DELETING_ENTRY));
        return FMR_DELETING_ENTRY;
    }

    if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    /* Do not forget to skip the space held by MFW meta data */
    if (ui1_whence == FM_SEEK_BGN)
    {
        if (pt_mfw_ent->t_mfw_info.e_meta_policy == FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE)
        {
            i8_offset += MFW_META_SIZE;
        }
    }
    else if (ui1_whence == FM_SEEK_CUR)
    {
        i8_offset += pt_mfw_obj->ui8_cur_pos;
    }
    else
    {
        pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);
        
        i8_offset += pt_sgl_fd->ui8_end_pos;
    }

    if (i8_offset < 0)
    {
        i8_offset = 0;
    }
    else
    {
        pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);
        
        if ((UINT64)i8_offset > pt_sgl_fd->ui8_end_pos)
        {
            i8_offset = (INT64)pt_sgl_fd->ui8_end_pos;
        }
    }

    DLIST_FOR_EACH_BACKWARD(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
    {
        if (pt_sgl_fd->ui8_start_pos <= (UINT64)i8_offset)
        {
            break;
        }
    }
    
    if (pt_sgl_fd != NULL)
    {
        if (pt_sgl_fd->ui4_id != pt_mfw_obj->ui4_cur_fid)
        {
            /* Located in new sub file */
            CHAR* ps_path = NULL;

            ps_path = _fm_mfw_get_sub_file_path(
                pt_mfw_ent->t_mfw_info.ps_dir,
                pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                pt_sgl_fd->ui4_id);
            
            if (ps_path == NULL)
            {
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
                return FMR_CORE;
            }

            /* Close current opened sub file */
            i4_ret = x_fm_mfw_delay_close_current_file(pt_mfw_obj);

            /* Open the new located sub file */
            i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                ps_path, 
                                pt_mfw_obj->ui4_flags,
                                pt_mfw_obj->ui4_mode,
                                pt_mfw_obj->b_ext_buf,
                                &pt_mfw_obj->h_cur_file );
            if (i4_ret != FMR_OK)
            {
                x_mem_free(ps_path);
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: open %s return %d\n", __FUNCTION__,__LINE__,ps_path, FMR_CORE));
                return FMR_CORE;
            }
            
            x_mem_free(ps_path);
            ps_path = NULL;

            pt_mfw_obj->ui4_cur_fid = pt_sgl_fd->ui4_id;
        }
        /* Seek to relative offset of this file */
        ui8_relative_ofst = (UINT64)i8_offset - pt_sgl_fd->ui8_start_pos;

        i4_ret = x_fm_lseek(pt_mfw_obj->h_cur_file,
                            (INT64)ui8_relative_ofst,
                            FM_SEEK_BGN,
                            pui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
            return FMR_INVAL;
        }
    
        pt_mfw_obj->ui8_cur_pos = (UINT64)i8_offset;

        /* User should not know that, --! */
        if (pt_mfw_ent->t_mfw_info.e_meta_policy == FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE)
        {
            *pui8_cur_pos = pt_mfw_obj->ui8_cur_pos - MFW_META_SIZE;
        }
        else
        {
            *pui8_cur_pos = pt_mfw_obj->ui8_cur_pos;
        }
   
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_INFO(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_OK));

        /* Exit happy */
        return FMR_OK;
    }
    else
    {
        ASSERT(0);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    
}
    
/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_read
 *
 * Description: Read data from wrapper to buffer.
 *
 * Inputs:  h_file
 *              Handle of an opened wrapper.
 *          pv_data
 *              Buffer to hold read data.
 *          ui4_count
 *              Size of pv_data in bytes.
 *
 * Outputs: pui4_read
 *              Actual read bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL or pui4_read is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed.
 *          FMR_EOF
 *              Reaches EOF.
 *          FMR_DEVICE_ERROR
 *              Device read error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_read(
    HANDLE_T        h_file,
    VOID*           pv_data,
    UINT32          ui4_count,
    UINT32*         pui4_read)
{
    INT32           i4_ret = FMR_OK;
    FM_MFW_OBJ_T*   pt_mfw_obj = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    HANDLE_TYPE_T   e_obj;
    UINT32          ui4_read, ui4_unread, ui4_this_unread, ui4_read_cnt, ui4_done;
    UINT64          ui8_cur_pos;

    MFW_TIME_STAMP(("Sync read enter"));
    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    if (h_file == NULL_HANDLE || pv_data == NULL || pui4_read == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    {
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        return x_fm_read( h_file, pv_data, ui4_count, pui4_read);
    }

    i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        return i4_ret;
    }

    pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
    ASSERT(pt_mfw_ent != NULL);
    
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != OSR_OK)
    {
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        return FMR_CORE;
    }
    
    if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_DELETING_ENTRY));
        return FMR_DELETING_ENTRY;
    }

    if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    
    pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);

    /* We do not read over the EOF */
    if (pt_mfw_obj->ui8_cur_pos >= pt_sgl_fd->ui8_end_pos)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
        return FMR_EOF;
    }
    
    if (pt_mfw_obj->ui8_cur_pos + ui4_count > pt_sgl_fd->ui8_end_pos)
    {
        ui4_count = (UINT32)(pt_sgl_fd->ui8_end_pos - pt_mfw_obj->ui8_cur_pos);
    }

    ui4_read = 0;
    ui4_unread = ui4_count;
    ui8_cur_pos = pt_mfw_obj->ui8_cur_pos;

    while (ui4_unread > 0)
    {
        DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
        {
            if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
            {
                break;
            }
        }
        ASSERT(pt_sgl_fd != NULL);
        if (pt_sgl_fd == NULL)
        {
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));

            return FMR_CORE;
        }

        /* Get unread bytes in this file */
        ui4_this_unread = (UINT32)(pt_sgl_fd->ui8_end_pos - ui8_cur_pos);

        /* Get read count this time */
        if (ui4_this_unread >= ui4_unread)
        {
            ui4_read_cnt = ui4_unread;
        }
        else
        {
            ui4_read_cnt = ui4_this_unread;
        }
        
        /* read in current file */
        i4_ret = x_fm_read( pt_mfw_obj->h_cur_file,
                            (VOID*)((UINT8*)pv_data+ui4_read),
                            ui4_read_cnt,
                            &ui4_done );

        if (i4_ret != FMR_OK)
        {
            if (i4_ret != FMR_EOF)
            {
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                return i4_ret;
            }
        }
        
        ui4_read += ui4_done;
        ui4_unread -= ui4_done;
        ui4_this_unread -= ui4_done;
        
        if (ui4_this_unread == 0)
        {
            /* read over the current file */
            /* open next file */
            pt_mfw_obj->ui4_cur_fid++;

            DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
            {
                if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
                {
                    break;
                }
            }
            
            if (pt_sgl_fd == NULL)
            {
                /* Current file is the last file in the MFDC */
                pt_mfw_obj->ui4_cur_fid--;
                *pui4_read = ui4_read;
                pt_mfw_obj->ui8_cur_pos += ui4_read;
                
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                return FMR_OK;
            }
            else
            {
                CHAR* ps_path = NULL;

                ps_path = _fm_mfw_get_sub_file_path(
                    pt_mfw_ent->t_mfw_info.ps_dir,
                    pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                    pt_sgl_fd->ui4_id);

                if (ps_path == NULL)
                {
                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
                    return FMR_CORE;
                }

                /* Close current file */
                i4_ret = x_fm_close(pt_mfw_obj->h_cur_file);
                
                /* Open new located file */
                i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                    ps_path, 
                                    pt_mfw_obj->ui4_flags,
                                    pt_mfw_obj->ui4_mode,
                                    pt_mfw_obj->b_ext_buf,
                                    &pt_mfw_obj->h_cur_file );
                if (i4_ret != FMR_OK)
                {   
                    x_mem_free(ps_path);

                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: open %s return %d\n", __FUNCTION__,__LINE__,ps_path, i4_ret));
                    return i4_ret;
                }
                
                x_mem_free(ps_path);
                ps_path = NULL;
            }

            if (ui4_unread > 0)
            {
                /* Still need to read */
                ui8_cur_pos += ui4_done;
                continue;
            }
            else
            {
                /* Read completed */
                break;
            }
        }
        else if (ui4_unread > 0 && ui4_this_unread > 0)
        {
            /* It should not happen */
            ASSERT(0);
        }
        else/* (ui4_unread == 0 && ui4_this_unread > 0) */
        {
            /* Read completed, current file didn't change */
            pt_mfw_obj->ui8_cur_pos += ui4_read;
            *pui4_read = ui4_read;
            
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);

            /* Exit happy */
            return FMR_OK;
        }
    }

    *pui4_read = ui4_read;    
    pt_mfw_obj->ui8_cur_pos += ui4_read;
    
    MFW_UNLOCK_ENTRY(pt_mfw_ent);
    _fm_mfw_obj_unlock(pt_mfw_obj);
    MFW_TIME_STAMP(("Sync read exit."));

    /* Exit happy */
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_read_async
 *
 * Description: Asynchronously read data from the opened wrapper.
 *
 * Inputs:  h_file
 *              Handle of the opened wrapper.
 *          pv_data
 *              Reference to a buffer to hold the read data.
 *          ui4_count
 *              Number of bytes to read.
 *          ui1_pri
 *              I/O priority.
 *          pf_nfy_fct
 *              Callback function while read is done.
 *          pv_tag
 *              Tag value, which will be returned in the callback.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device read error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_read_async(
    HANDLE_T        h_file,
    VOID*           pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID*           pv_tag,
    HANDLE_T*       ph_req)
{
    INT32           i4_ret = FMR_OK;
    FM_MFW_OBJ_T*   pt_mfw_obj = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    HANDLE_TYPE_T   e_obj;
    FM_MFW_ASYNC_RW_TAG_T* pt_rw_tag = NULL;
    FM_MFW_ASYNC_RW_OBJ_T* pt_rw_obj = NULL;
    UINT32 ui4_read_cnt;
    size_t size_already_read = 0;
    size_t current_file_left_size = 0;
    size_t size_need_to_read = ui4_count;
    UINT64 ui8_cur_pos;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    if (h_file == NULL_HANDLE || pv_data == NULL || ph_req == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    {
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        return x_fm_read_async( h_file, pv_data, ui4_count, ui1_pri, pf_nfy_fct, pv_tag, ph_req);
    }

    i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        return i4_ret;
    }

    pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
    ASSERT(pt_mfw_ent != NULL);
    
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != OSR_OK)
    {
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_DELETING_ENTRY));
        return FMR_DELETING_ENTRY;
    }

    if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);

    if (pt_mfw_obj->ui8_cur_pos >= pt_sgl_fd->ui8_end_pos)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
        return FMR_EOF;
    }
    /* We do not read over the EOF */
    if (pt_mfw_obj->ui8_cur_pos + ui4_count > pt_sgl_fd->ui8_end_pos)
    {
        ui4_count = (UINT32)(pt_sgl_fd->ui8_end_pos - pt_mfw_obj->ui8_cur_pos);
    }

    /* Enhancement here, we reuse the existed freed asynchronous tag */
    pt_rw_tag = _fm_mfw_acquire_arw_tag(TRUE);
    if (pt_rw_tag == NULL)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    pt_rw_obj = pt_rw_tag->pt_rw_obj;  
    pt_rw_obj->ui4_req_num = 0;

    *ph_req = pt_rw_tag->h_rw_req;

    /* Setup the tag */
    pt_rw_tag->ui4_count = ui4_count;
    pt_rw_tag->ui4_done_bytes = 0;
    pt_rw_tag->pf_nfy_fct = pf_nfy_fct;
    pt_rw_tag->pv_tag = pv_tag;
    pt_rw_tag->h_rw_req = *ph_req;

    pt_rw_tag->b_aborted = FALSE;
    pt_rw_tag->b_all_req_sent = FALSE;
    pt_rw_tag->ui4_req_cnt = 0;
    pt_rw_tag->ui4_req_fail_cnt = 0;
    pt_rw_tag->ui4_req_ok_cnt = 0;

    ui8_cur_pos = pt_mfw_obj->ui8_cur_pos;
    while (size_need_to_read > 0)
    {
        DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
        {
            if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
            {
                break;
            }
        }
        ASSERT(pt_sgl_fd != NULL);
        if (pt_sgl_fd == NULL)
        {
            x_mem_free(pt_rw_tag);
            x_handle_free(*ph_req);
            
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);

            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));

            return FMR_CORE;
        }
        
        current_file_left_size = (size_t)(pt_sgl_fd->ui8_end_pos - ui8_cur_pos);

        if (current_file_left_size >= size_need_to_read)
        {
            ui4_read_cnt = size_need_to_read;
        }
        else
        {
            ui4_read_cnt = current_file_left_size;
        }

        if (pt_rw_obj->ui4_req_num % 10 == 0)
        {
            /* This array is used to save async RW request handle returned by FM */
            pt_rw_obj->ph_req = x_mem_realloc(pt_rw_obj->ph_req, sizeof(HANDLE_T)*(pt_rw_obj->ui4_req_num/10+1)*10);
            if (pt_rw_obj->ph_req == NULL)
            {
                x_mem_free(pt_rw_tag);
                x_handle_free(*ph_req);

                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                return FMR_CORE;
            }
        }
        /* NOTE: the following set operations must be done before sending the request;
           otherwise, in the async callback function may not be aware of that, and 
           we may never have chance to free the resource. */
        pt_rw_tag->ui4_req_cnt++;
        
        if (size_need_to_read <= ui4_read_cnt)
        {
            pt_rw_tag->b_all_req_sent = TRUE;
        }
        
        if ((current_file_left_size <= ui4_read_cnt) && 
            ((pt_mfw_obj->ui4_cur_fid + 1) == pt_mfw_ent->t_mfw_info.ui4_file_num))
        {
            pt_rw_tag->b_all_req_sent = TRUE;
        }
        
        MFW_TIME_STAMP(("Before async reading file"));

        i4_ret = x_fm_read_async( pt_mfw_obj->h_cur_file,
                                  (VOID*)((UINT8*)pv_data + size_already_read),
                                  ui4_read_cnt,
                                  ui1_pri,
                                  _fm_mfw_async_done,
                                  pt_rw_tag,
                                  &pt_rw_obj->ph_req[pt_rw_obj->ui4_req_num++]);
        MFW_TIME_STAMP(("After async reading file(REQ:%x)", pt_rw_obj->ph_req[pt_rw_obj->ui4_req_num-1]));

        if (i4_ret != FMR_OK)
        {
            if (i4_ret != FMR_EOF)
            {
                x_mem_free(pt_rw_tag);
                x_handle_free(*ph_req);

                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                return i4_ret;
            }
        }

        size_already_read += ui4_read_cnt;
        size_need_to_read -= ui4_read_cnt;
        current_file_left_size -= ui4_read_cnt;
        
        if (current_file_left_size == 0)
        {
            /* read over the current file */
            /* open next file */
            
            pt_mfw_obj->ui4_cur_fid++;

            DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
            {
                if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
                {
                    break;
                }
            }
            
            if (pt_sgl_fd == NULL)
            {
                pt_mfw_obj->ui4_cur_fid--;
                pt_mfw_obj->ui8_cur_pos += size_already_read;
                
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                return FMR_OK;
            }
            else
            {
                CHAR* ps_path = NULL;

                ps_path = _fm_mfw_get_sub_file_path(
                    pt_mfw_ent->t_mfw_info.ps_dir,
                    pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                    pt_sgl_fd->ui4_id);
                
                if (ps_path == NULL)
                {
                    x_mem_free(pt_rw_tag);
                    x_handle_free(*ph_req);
                    
                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
                    return FMR_CORE;
                }
 
                x_fm_mfw_delay_close_current_file(pt_mfw_obj);


                i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                    ps_path, 
                                    pt_mfw_obj->ui4_flags,
                                    pt_mfw_obj->ui4_mode,
                                    pt_mfw_obj->b_ext_buf,
                                    &pt_mfw_obj->h_cur_file );
                if (i4_ret != FMR_OK)
                {
                    x_mem_free(pt_rw_tag);
                    x_handle_free(*ph_req);
                    x_mem_free(ps_path);
                    
                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: open %s return %d\n", __FUNCTION__,__LINE__,ps_path, i4_ret));
                    return i4_ret;
                }
                
                x_mem_free(ps_path);
            }

            if (size_need_to_read > 0)
            {
                ui8_cur_pos += ui4_read_cnt;
                continue;
            }
            else
            {
                break;
            }
        }
        else if (size_need_to_read > 0 && current_file_left_size > 0)
        {
            /* It should not happen */
            ASSERT(0);
        }
        else/* (size_need_to_read == 0 && current_file_left_size > 0) */
        {
            pt_mfw_obj->ui8_cur_pos += size_already_read;
            
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);

            /* Exit happy */
            return FMR_OK;
        }
    }
        
    pt_mfw_obj->ui8_cur_pos += size_already_read;
    
    MFW_UNLOCK_ENTRY(pt_mfw_ent);
    _fm_mfw_obj_unlock(pt_mfw_obj);

    /* Exit happy */
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_write
 *
 * Description: Write data from buffer to wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened wrapper.
 *          pv_data
 *              Buffer to write.
 *          ui4_count
 *              Number of bytes to write.
 *
 * Outputs: pui4_written
 *              Actual written bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL or pui4_written is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed.
 *          FMR_EOF
 *              Reaches EOF.
 *          FMR_DEVICE_ERROR
 *              Device write error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_write(
    HANDLE_T        h_file,
    const VOID*     pv_data,
    UINT32          ui4_count,
    UINT32*         pui4_written)
{
    INT32           i4_ret = FMR_OK;
    FM_MFW_OBJ_T*   pt_mfw_obj = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    HANDLE_TYPE_T   e_obj;
    UINT32          ui4_written, ui4_unwritten, ui4_this_unwritten, ui4_write_cnt, ui4_done;
    UINT64          ui8_cur_pos;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    if (h_file == NULL_HANDLE || pv_data == NULL || pui4_written == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    {
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_HANDLE));
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        return x_fm_write( h_file, pv_data, ui4_count, pui4_written);
    }

    i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        return i4_ret;
    }

    pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
    ASSERT(pt_mfw_ent != NULL);
    
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != OSR_OK)
    {
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_DELETING_ENTRY));
        return FMR_DELETING_ENTRY;
    }

    if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
        return FMR_INVAL;
    }
    
    pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);
    
    if (pt_mfw_obj->ui8_cur_pos + ui4_count > pt_sgl_fd->ui8_end_pos)
    {
        UINT64 ui8_new_size;

        ui8_new_size = pt_mfw_obj->ui8_cur_pos + ui4_count - pt_sgl_fd->ui8_start_pos;
        /* [20091128, Chun] To improve PVR write performance, extend much more bytes each time*/
        i4_ret = _fm_mfw_extend_last_file_in_wrapper(pt_mfw_ent, (ui8_new_size/*+0x200000*/), FALSE);
        if (i4_ret != FMR_OK)
        {
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            return i4_ret;
        }
    }
    
    if (pt_mfw_obj->ui8_cur_pos + ui4_count > pt_mfw_ent->ui8_user_size)
    {
        pt_mfw_ent->ui8_user_size = pt_mfw_obj->ui8_cur_pos + ui4_count;
    }

    ui4_written = 0;
    ui4_unwritten = ui4_count;
    ui8_cur_pos = pt_mfw_obj->ui8_cur_pos;

    while (ui4_unwritten > 0)
    {
        DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
        {
            if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
            {
                break;
            }
        }
        ASSERT(pt_sgl_fd != NULL);
        if (pt_sgl_fd == NULL)
        {
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);

            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));

            return FMR_CORE;
        }
        
        ui4_this_unwritten = (UINT32)(pt_sgl_fd->ui8_end_pos - ui8_cur_pos);

        if (ui4_this_unwritten >= ui4_unwritten)
        {
            ui4_write_cnt = ui4_unwritten;
        }
        else
        {
            ui4_write_cnt = ui4_this_unwritten;
        }

        if (ui4_write_cnt > 0)
        {
            i4_ret = x_fm_write( pt_mfw_obj->h_cur_file,
                                (VOID*)((UINT8*)pv_data+ui4_written),
                                ui4_write_cnt,
                                &ui4_done );

            if (i4_ret != FMR_OK)
            {
                if (i4_ret != FMR_EOF)
                {
                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                    return i4_ret;
                }
            }
        }
        else
        {
            /* For case: ui4_this_unwritten == 0 */
            ui4_done = 0;
        }
        
        ui4_written += ui4_done;
        ui4_unwritten -= ui4_done;
        ui4_this_unwritten -= ui4_done;
        
        if (ui4_this_unwritten == 0)
        {
            pt_mfw_obj->ui4_cur_fid++;

            DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
            {
                if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
                {
                    break;
                }
            }
            
            if (pt_sgl_fd == NULL)
            {
                pt_mfw_obj->ui4_cur_fid--;
            #if 0
                *pui4_written = ui4_written;
                pt_mfw_obj->ui8_cur_pos += ui4_written;
                
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);

                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
                
                return FMR_EOF;
            #else
                break;
            #endif
            }
            else
            {
                CHAR* ps_path = NULL;

                ps_path = _fm_mfw_get_sub_file_path(
                    pt_mfw_ent->t_mfw_info.ps_dir,
                    pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                    pt_sgl_fd->ui4_id);

                if (ps_path == NULL)
                {
                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
                    return FMR_CORE;
                }

                i4_ret = x_fm_close(pt_mfw_obj->h_cur_file);

                i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                    ps_path, 
                                    pt_mfw_obj->ui4_flags,
                                    pt_mfw_obj->ui4_mode,
                                    pt_mfw_obj->b_ext_buf,
                                    &pt_mfw_obj->h_cur_file );
                if (i4_ret != FMR_OK)
                {   
                    x_mem_free(ps_path);

                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: open %s return %d\n", __FUNCTION__,__LINE__,ps_path, i4_ret));
                    return i4_ret;
                }
                
                x_mem_free(ps_path);
            }

            if (ui4_unwritten > 0)
            {
                ui8_cur_pos += ui4_done;
                continue;
            }
            else
            {
                break;
            }
        }
        else if (ui4_unwritten > 0 && ui4_this_unwritten > 0)
        {
            /* It should not happen */
            ASSERT(0);
        }
        else/* (ui4_unread == 0 && ui4_this_unread > 0) */
        {
            pt_mfw_obj->ui8_cur_pos += ui4_written;
            *pui4_written = ui4_written;
            
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);
            
            return FMR_OK;
        }
    }

    *pui4_written = ui4_written;    
    pt_mfw_obj->ui8_cur_pos += ui4_written;
    
    MFW_UNLOCK_ENTRY(pt_mfw_ent);
    _fm_mfw_obj_unlock(pt_mfw_obj);
    
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_write_async
 *
 * Description: Asynchronously write data to the opened wrapper.
 *
 * Inputs:  h_file
 *              Handle of the opened wrapper.
 *          pv_data
 *              Buffer to write.
 *          ui4_count
 *              Number of bytes to write.
 *          ui1_pri
 *              I/O priority.
 *          pf_nfy_fct
 *              Callback function while write is done.
 *          pv_tag
 *              Tag value, which will be returned in the callback.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device write error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_write_async(
    HANDLE_T        h_file,
    const VOID*     pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID*           pv_tag,
    HANDLE_T*       ph_req)
{
    INT32           i4_ret = FMR_OK;
    FM_MFW_OBJ_T*   pt_mfw_obj = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    HANDLE_TYPE_T   e_obj;
    FM_MFW_ASYNC_RW_OBJ_T* pt_rw_obj = NULL;
    FM_MFW_ASYNC_RW_TAG_T* pt_rw_tag = NULL;
    UINT32          ui4_written, ui4_unwritten, ui4_this_unwritten, ui4_write_cnt;
    UINT64          ui8_cur_pos;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    if (h_file == NULL_HANDLE || pv_data == NULL || ph_req == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_HANDLE));
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        return x_fm_write_async( h_file, pv_data, ui4_count, ui1_pri, pf_nfy_fct, pv_tag, ph_req);
    }

    i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        return i4_ret;
    }

    pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
    ASSERT(pt_mfw_ent != NULL);
    
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != OSR_OK)
    {
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    
    if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_DELETING_ENTRY));
        return FMR_DELETING_ENTRY;
    }

    if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);

    if (pt_mfw_obj->ui8_cur_pos + ui4_count > pt_sgl_fd->ui8_end_pos)
    {
        UINT64 ui8_new_size;

        ui8_new_size = pt_mfw_obj->ui8_cur_pos + ui4_count - pt_sgl_fd->ui8_start_pos;
        
        /* [20091128, Chun] To improve PVR write performance, extend much more bytes each time*/
        i4_ret = _fm_mfw_extend_last_file_in_wrapper(pt_mfw_ent, (ui8_new_size/*+0x200000*/), TRUE);
        if (i4_ret != FMR_OK)
        {
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
            return i4_ret;
        }        
    }
    
    if (pt_mfw_obj->ui8_cur_pos + ui4_count > pt_mfw_ent->ui8_user_size)
    {
        pt_mfw_ent->ui8_user_size = pt_mfw_obj->ui8_cur_pos + ui4_count;
    }

    pt_rw_tag = _fm_mfw_acquire_arw_tag(FALSE);
    if (pt_rw_tag == NULL)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    pt_rw_obj = pt_rw_tag->pt_rw_obj;  
    pt_rw_obj->ui4_req_num = 0;

    *ph_req = pt_rw_tag->h_rw_req;

    pt_rw_tag->ui4_count = ui4_count;
    pt_rw_tag->ui4_done_bytes = 0;
    pt_rw_tag->pf_nfy_fct = pf_nfy_fct;
    pt_rw_tag->pv_tag = pv_tag;
    pt_rw_tag->h_rw_req = *ph_req;

    pt_rw_tag->b_aborted = FALSE;
    pt_rw_tag->b_all_req_sent = FALSE;
    pt_rw_tag->ui4_req_cnt = 0;
    pt_rw_tag->ui4_req_fail_cnt = 0;
    pt_rw_tag->ui4_req_ok_cnt = 0;


    ui4_written = 0;
    ui4_unwritten = ui4_count;
    ui8_cur_pos = pt_mfw_obj->ui8_cur_pos;

    while (ui4_unwritten > 0)
    {
        DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
        {
            if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
            {
                break;
            }
        }
        ASSERT(pt_sgl_fd != NULL);
        if (pt_sgl_fd == NULL)
        {
            x_mem_free(pt_rw_tag);
            x_handle_free(*ph_req);
            
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);
            
            DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
            return FMR_CORE;
        }
        ui4_this_unwritten = (UINT32)(pt_sgl_fd->ui8_end_pos - ui8_cur_pos);

        if (ui4_this_unwritten >= ui4_unwritten)
        {
            ui4_write_cnt = ui4_unwritten;
        }
        else
        {
            ui4_write_cnt = ui4_this_unwritten;
        }

        if (ui4_write_cnt > 0)
        {
        #ifndef LINUX_TURNKEY_SOLUTION
            FM_MFW_MSG_T t_msg;
            FM_MFW_ASYNC_WRITE_SUB_FILE_T t_aw;
            CHAR* ps_sub_file_path = NULL;
            UINT64 ui8_temp;
        #endif    
            if (pt_rw_obj->ui4_req_num % 10 == 0)
            {
                pt_rw_obj->ph_req = x_mem_realloc(pt_rw_obj->ph_req, sizeof(HANDLE_T)*(pt_rw_obj->ui4_req_num/10+1)*10);
                if (pt_rw_obj->ph_req == NULL)
                {
                    x_mem_free(pt_rw_tag);
                    x_handle_free(*ph_req);

                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                    return FMR_CORE;
                }
            }
            pt_rw_tag->ui4_req_cnt++;
            
            if (ui4_unwritten <= ui4_write_cnt)
            {
                pt_rw_tag->b_all_req_sent = TRUE;
            }
            
            if ((ui4_this_unwritten <= ui4_write_cnt) && 
                ((pt_mfw_obj->ui4_cur_fid + 1) == pt_mfw_ent->t_mfw_info.ui4_file_num))
            {
                pt_rw_tag->b_all_req_sent = TRUE;
            }
        #ifndef LINUX_TURNKEY_SOLUTION
            i4_ret = x_fm_lseek(pt_mfw_obj->h_cur_file, 0, FM_SEEK_CUR, &t_aw.ui8_write_ofst);
            if (i4_ret != FMR_OK)
            {
                x_mem_free(pt_rw_tag);
                x_handle_free(*ph_req);
                
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                return FMR_CORE;
            }

            ps_sub_file_path = _fm_mfw_get_sub_file_path(
                                    pt_mfw_ent->t_mfw_info.ps_dir,
                                    pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                                    pt_mfw_obj->ui4_cur_fid);
            if (ps_sub_file_path == NULL)
            {
                x_mem_free(pt_rw_tag);
                x_handle_free(*ph_req);
                
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                return FMR_CORE;
            }

            t_aw.ps_sub_file_path  = ps_sub_file_path;
            t_aw.ui4_flags = pt_mfw_obj->ui4_flags;
            t_aw.ui4_mode = pt_mfw_obj->ui4_mode;
            t_aw.b_ext_buf = pt_mfw_obj->b_ext_buf;
            t_aw.pv_data = (const VOID*)((UINT8*)pv_data+ui4_written);
            t_aw.ui4_count = ui4_write_cnt;
            t_aw.ui1_pri   = ui1_pri;
            t_aw.pf_nfy_fct = _fm_mfw_async_done;
            t_aw.pv_tag = pt_rw_tag;
            t_aw.ph_req = &pt_rw_obj->ph_req[pt_rw_obj->ui4_req_num++];

            t_msg.e_type = FM_MFW_MSG_ASYNC_WRITE_SUB_FILE;
            x_memcpy(&t_msg.u.t_async_write_sub_file, &t_aw, sizeof(FM_MFW_ASYNC_WRITE_SUB_FILE_T));

            i4_ret = x_msg_q_send(gh_mfw_msg_q, 
                                  (const VOID *) &t_msg, 
                                  sizeof(FM_MFW_MSG_T),
                                  FM_MFW_NFY_REQ_PRIORITY);
            
            if (i4_ret != OSR_OK)
            {
                DBG_ERROR(("%s: send msg failed.\n", __FUNCTION__));

                x_mem_free(ps_sub_file_path);
                x_mem_free(pt_rw_tag);
                x_handle_free(*ph_req);
                
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                return i4_ret;
            }
            
            i4_ret = x_fm_lseek(pt_mfw_obj->h_cur_file, (INT64)(t_aw.ui8_write_ofst+ui4_write_cnt), FM_SEEK_BGN, &ui8_temp);
            if (i4_ret != FMR_OK)
            {
                x_mem_free(ps_sub_file_path);
                x_mem_free(pt_rw_tag);
                x_handle_free(*ph_req);
                
                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, i4_ret));
                return FMR_CORE;
            }
        #else
            i4_ret = x_fm_write_async(
                pt_mfw_obj->h_cur_file,
                (const VOID*)((UINT8*)pv_data+ui4_written),
                ui4_write_cnt,
                ui1_pri,
                _fm_mfw_async_done,
                pt_rw_tag,
                &pt_rw_obj->ph_req[pt_rw_obj->ui4_req_num++]);
        #endif
        }
        
        ui4_written += ui4_write_cnt;
        ui4_unwritten -= ui4_write_cnt;
        ui4_this_unwritten -= ui4_write_cnt;
        
        if (ui4_this_unwritten == 0)
        {
            pt_mfw_obj->ui4_cur_fid++;

            DLIST_FOR_EACH(pt_sgl_fd, &pt_mfw_ent->t_mfdc, t_link)
            {
                if (pt_sgl_fd->ui4_id == pt_mfw_obj->ui4_cur_fid)
                {
                    break;
                }
            }
            
            if (pt_sgl_fd == NULL)
            {                
                pt_mfw_obj->ui4_cur_fid--;
                pt_mfw_obj->ui8_cur_pos += ui4_written;

                MFW_UNLOCK_ENTRY(pt_mfw_ent);
                _fm_mfw_obj_unlock(pt_mfw_obj);
                
                return FMR_OK;
            }
            else
            {
                CHAR* ps_path = NULL;

                ps_path = _fm_mfw_get_sub_file_path(
                    pt_mfw_ent->t_mfw_info.ps_dir,
                    pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                    pt_sgl_fd->ui4_id);
                
                if (ps_path == NULL)
                {
                    x_mem_free(pt_rw_tag);
                    x_handle_free(*ph_req);
                    
                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
                    return FMR_CORE;
                }

                i4_ret = x_fm_mfw_delay_close_current_file(pt_mfw_obj);

                i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                    ps_path, 
                                    pt_mfw_obj->ui4_flags,
                                    pt_mfw_obj->ui4_mode,
                                    pt_mfw_obj->b_ext_buf,
                                    &pt_mfw_obj->h_cur_file );
                if (i4_ret != FMR_OK)
                {
                    x_mem_free(pt_rw_tag);
                    x_handle_free(*ph_req);
                    x_mem_free(ps_path);
                    
                    MFW_UNLOCK_ENTRY(pt_mfw_ent);
                    _fm_mfw_obj_unlock(pt_mfw_obj);
                    
                    DBG_ERROR(("[FM-MFW] %s #%d: open %s return %d\n", __FUNCTION__,__LINE__,ps_path, i4_ret));
                    return i4_ret;
                }
                
                x_mem_free(ps_path);
            }

            if (ui4_unwritten > 0)
            {
                ui8_cur_pos += ui4_write_cnt;
                continue;
            }
            else
            {
                break;
            }
        }
        else if (ui4_unwritten > 0 && ui4_this_unwritten > 0)
        {
            /* It should not happen */
            ASSERT(0);
        }
        else/* (ui4_unwritten == 0 && ui4_this_unwritten > 0) */
        {            
            pt_mfw_obj->ui8_cur_pos += ui4_written;
            
            MFW_UNLOCK_ENTRY(pt_mfw_ent);
            _fm_mfw_obj_unlock(pt_mfw_obj);
            
            return FMR_OK;
        }
    }

    pt_mfw_obj->ui8_cur_pos += ui4_written;
    
    MFW_UNLOCK_ENTRY(pt_mfw_ent);
    _fm_mfw_obj_unlock(pt_mfw_obj);
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_feof
 *
 * Description: Tell if current position reaches the end of wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened wrapper.
 *
 * Outputs: pb_eof
 *              TRUE if it is at EOF, otherwise FALSE is returned.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pb_eof is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              file is beging closed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_feof(
    HANDLE_T        h_file,
    BOOL            *pb_eof)
{
    INT32           i4_ret = FMR_OK;
    FM_MFW_OBJ_T*   pt_mfw_obj = NULL;
    FM_MFW_SINGLE_FILE_DESC_T* pt_sgl_fd = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    HANDLE_TYPE_T   e_obj;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    if (h_file == NULL_HANDLE || pb_eof == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_HANDLE));
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        return x_fm_feof(h_file, pb_eof);
    }

    i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        return i4_ret;
    }

    pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
    ASSERT(pt_mfw_ent != NULL);
    
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != OSR_OK)
    {
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_DELETING_ENTRY));
        return FMR_DELETING_ENTRY;
    }

    if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    pt_sgl_fd = DLIST_TAIL(&pt_mfw_ent->t_mfdc);
    ASSERT(pt_sgl_fd != NULL);

    *pb_eof = (pt_mfw_obj->ui8_cur_pos >= pt_sgl_fd->ui8_end_pos) ? TRUE : FALSE;

    MFW_UNLOCK_ENTRY(pt_mfw_ent);
    _fm_mfw_obj_unlock(pt_mfw_obj);

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_abort_async
  *
  * Description: Abort async-ed r/w request
  *
  * Input: h_req
  *             Handle of the r/w request.
  *
  * Output: FMR_OK
  *                 Success!
  *             FMR_INVAL
  *                 No such request in queue.
  *             FMR_CANCELING
  *                 Caller will get a e_cond in callback function.
  *
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_abort_async(
    HANDLE_T h_req)
{
    INT32           i4_ret = FMR_OK;
    VOID*           pv_obj;
    FM_MFW_ANLS_CRET_OBJ_T* pt_anls_cret_obj = NULL;
    FM_MFW_ASYNC_RW_OBJ_T* pt_rw_obj = NULL;
    HANDLE_TYPE_T   e_obj;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }
   
    if (h_req == NULL_HANDLE)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = x_sema_lock(gh_rel_req_lock, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return FMR_CORE;
    }

    if (handle_get_type_obj(h_req, &e_obj, (VOID**)&pv_obj) != HR_OK)
    {
        x_sema_unlock(gh_rel_req_lock);
        return FMR_HANDLE;
    }

    if (e_obj == FMT_MFW_ASYNC_ANLS_CRET)
    {
        pt_anls_cret_obj = (FM_MFW_ANLS_CRET_OBJ_T*)pv_obj;
        ASSERT(pt_anls_cret_obj != NULL);

        i4_ret = x_sema_lock(pt_anls_cret_obj->h_lock, X_SEMA_OPTION_WAIT);
        if (i4_ret != OSR_OK)
        {
            x_sema_unlock(gh_rel_req_lock);
            return FMR_CORE;
        }

        pt_anls_cret_obj->b_abort = TRUE;

        x_sema_unlock(pt_anls_cret_obj->h_lock);

        x_sema_unlock(gh_rel_req_lock);
        return FMR_OK;
    }
    else if ((e_obj == FMT_MFW_ASYNC_READ) || (e_obj == FMT_MFW_ASYNC_WRITE))
    {
        UINT32 i;

        pt_rw_obj= (FM_MFW_ASYNC_RW_OBJ_T*)pv_obj;
        ASSERT(pt_rw_obj != NULL);

        for (i = 0; i < pt_rw_obj->ui4_req_num; i++)
        {
            i4_ret = x_fm_abort_async(pt_rw_obj->ph_req[i]);
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("[FM-MFW]%s #%d, abort async IO request %d failed with(%d)\n", __FUNCTION__,__LINE__, pt_rw_obj->ph_req[i], i4_ret));
            }
        }

        x_sema_unlock(gh_rel_req_lock);
        return FMR_OK;
    }
    else
    {
        x_sema_unlock(gh_rel_req_lock);

        return x_fm_abort_async(h_req);
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_get_info_by_name
 *
 * Description: Get information about a wrapper.
 *
 * Inputs:  h_dir
 *              Handle of a directory where the ps_path will be created.
 *              FM_ROOT_HANDLE can be used to indicate the root directory.
 *              NULL_HANDLE is equivalent to FM_ROOT_HANDLE.
 *          ps_path
 *              Path name of the entry to be created. It can be a single
 *              file name or a path. Note that if the first character of
 *              ps_path is '/', then h_dir will be ignored and ps_path is
 *              regarded as absolute path. This argument cannot be NULL.
 *
 * Outputs: pt_info
 *              Reference to the returned information about the wrapper.  
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pt_info is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_get_info_by_name(
	HANDLE_T        h_dir,
    const CHAR*     ps_path,
    FM_FILE_INFO_T* pt_info)
{
    INT32           i4_ret = FMR_OK;
    CHAR*           ps_sub_path = NULL;
    FM_FILE_INFO_T  t_info;
    UINT32          ui4_i;
    FM_MFW_INFO_T   t_mfw_info;
    BOOL            b_mfw, b_ok;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    if (pt_info == NULL || ps_path == NULL)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = _fm_mfw_check_ex(
        h_dir, 
        ps_path, 
        &b_mfw, 
        &b_ok,
        &t_mfw_info);

    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    if (b_mfw == TRUE && b_ok == FALSE)
    {
        _fm_mfw_free_buf_in_mfw_info(&t_mfw_info);
        return FMR_INVAL;
    }

    if (b_mfw == TRUE)
    {
        for (ui4_i = 0; ui4_i < t_mfw_info.ui4_file_num; ui4_i++)
        {
            ps_sub_path = _fm_mfw_get_sub_file_path(
                                t_mfw_info.ps_dir,
                                t_mfw_info.ps_fn_pattern,
                                ui4_i);
            if (ps_sub_path == NULL)
            {
                i4_ret = FMR_CORE;
                break;
            }
            if (ui4_i == 0)
            {
                i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_sub_path, pt_info);
                if (i4_ret != FMR_OK)
                {
                    x_mem_free(ps_sub_path);
                    ps_sub_path = NULL;
                    
                    break;
                }
                
                if (t_mfw_info.e_meta_policy == FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE)
                {
                    pt_info->ui8_size -= MFW_META_SIZE;
                    pt_info->ui8_blk_cnt -= ((MFW_META_SIZE + pt_info->ui4_blk_size -1) / (pt_info->ui4_blk_size));
                }
            }
            else
            {
                i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_sub_path, &t_info);
                if (i4_ret != FMR_OK)
                {
                    x_mem_free(ps_sub_path);
                    ps_sub_path = NULL;
                    
                    break;
                }
                
                pt_info->ui8_blk_cnt += t_info.ui8_blk_cnt;
                pt_info->ui8_size += t_info.ui8_size;
            }

            x_mem_free(ps_sub_path);
            ps_sub_path = NULL;
        }
        
        _fm_mfw_free_buf_in_mfw_info(&t_mfw_info);
        
        return i4_ret;
    }
    else
    {
        return x_fm_get_info_by_name(h_dir, ps_path, pt_info);
    }

}


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_get_info_by_handle
 *
 * Description: Get information about an opened wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *
 * Outputs: pt_info
 *              Reference to the returned information about the wrapper.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              h_file is NULL_HANDLE or pt_info is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_get_info_by_handle(
    HANDLE_T        h_file,
    FM_FILE_INFO_T* pt_info)
{
    INT32           i4_ret = FMR_OK;
    FM_MFW_OBJ_T*   pt_mfw_obj = NULL;
    FM_MFW_ENTRY_T* pt_mfw_ent = NULL;
    CHAR* ps_sub_path = NULL;
    UINT32 ui4_i;
    HANDLE_TYPE_T   e_obj;
    FM_FILE_INFO_T  t_info;

    if (gb_mfw_init == FALSE)
    {
        return FMR_NOT_INIT;
    }

    if (pt_info == NULL || h_file == NULL_HANDLE)
    {
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)&pt_mfw_obj) != HR_OK)
    {
        return FMR_HANDLE;
    }

    if (e_obj != FMT_MFW_OBJ)
    {
        return x_fm_get_info_by_handle(h_file, pt_info);
    }

    
    i4_ret = _fm_mfw_obj_lock(pt_mfw_obj);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM-MFW]%s #%d: lock failed with (%d)\n",__FUNCTION__,__LINE__,i4_ret));
        return i4_ret;
    }

    pt_mfw_ent = pt_mfw_obj->pt_mfw_entry;
    ASSERT(pt_mfw_ent != NULL);
    
    i4_ret = MFW_LOCK_ENTRY(pt_mfw_ent);
    if (i4_ret != OSR_OK)
    {
        _fm_mfw_obj_unlock(pt_mfw_obj);
        return FMR_CORE;
    }
    
    if (pt_mfw_ent->ui4_status & FM_MFW_ENTRY_UNLINKED)
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_DELETING_ENTRY));
        return FMR_DELETING_ENTRY;
    }

    if (DLIST_IS_EMPTY(&pt_mfw_ent->t_mfdc))
    {
        MFW_UNLOCK_ENTRY(pt_mfw_ent);
        _fm_mfw_obj_unlock(pt_mfw_obj);
        
        DBG_ERROR(("[FM-MFW] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    for (ui4_i = 0; ui4_i < pt_mfw_ent->t_mfw_info.ui4_file_num; ui4_i++)
    {
        ps_sub_path = _fm_mfw_get_sub_file_path(
                            pt_mfw_ent->t_mfw_info.ps_dir,
                            pt_mfw_ent->t_mfw_info.ps_fn_pattern,
                            ui4_i);
        if (ps_sub_path == NULL)
        {
            i4_ret = FMR_CORE;
            break;
        }
        
        if (ui4_i == 0)
        {
            i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_sub_path, pt_info);
            if (i4_ret != FMR_OK)
            {
                x_mem_free(ps_sub_path);
                ps_sub_path = NULL;
                
                break;
            }
            
            if (pt_mfw_ent->t_mfw_info.e_meta_policy == FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE)
            {
                pt_info->ui8_size -= MFW_META_SIZE;
                pt_info->ui8_blk_cnt -= ((MFW_META_SIZE + pt_info->ui4_blk_size -1) / (pt_info->ui4_blk_size));
            }
        }
        else
        {
            i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_sub_path, &t_info);
            if (i4_ret != FMR_OK)
            {
                x_mem_free(ps_sub_path);
                ps_sub_path = NULL;
                
                break;
            }
            
            pt_info->ui8_blk_cnt += t_info.ui8_blk_cnt;
            pt_info->ui8_size += t_info.ui8_size;
        }
    
        x_mem_free(ps_sub_path);
        ps_sub_path = NULL;
    }
    
    MFW_UNLOCK_ENTRY(pt_mfw_ent);
    _fm_mfw_obj_unlock(pt_mfw_obj);

    return i4_ret;

}

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_get_fs_info
 *
 * Description: Get a mounted file system information.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              A file/directory path name in the file system.
 *
 * Outputs: pt_fs_info
 *              Reference to file system information object.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pt_fs_info is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NO_ENTRY
 *              The directory component in ps_path or ps_path does not exist.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NOT_DIR
 *              The directory component in ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_mfw_get_fs_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FS_INFO_T    *pt_fs_info)
{
    return x_fm_get_fs_info( h_dir, ps_path, pt_fs_info);
}




