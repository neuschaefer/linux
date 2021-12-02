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
 * $RCSfile: mheg5_section.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all memory related interfaces between the MHEG-5
 *         stack from OceanBlue and the OSAI.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/x_handle_grp.h"
#include "mheg5/u_mheg5.h"
#include "mheg5/OceanBlue/glue/ssf/mheg5_section.h"
#include "os/inc/os.h"
#include "res_mngr/drv/x_demux.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "util/x_lnk_list.h"

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_ssf.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Abort stuff. */
#ifdef ABORT
#undef ABORT
#endif

#define ABORT(_cat, _val) DBG_ABORT ((DBG_MOD_MHEG_5 | _cat | _val))

#define DBG_ABRT_CANNOT_OPEN_TUNER      ((UINT32) 0x00000100)
#define DBG_ABRT_CANNOT_CREATE_SEMA     ((UINT32) 0x00000101)
#define DBG_ABRT_CANNOT_LOCK_SEMA       ((UINT32) 0x00000102)
#define DBG_ABRT_CANNOT_UNLOCK_SEMA     ((UINT32) 0x00000103)
#define DBG_ABRT_INV_ARGS               ((UINT32) 0x00000104)
#define DBG_ABRT_OUT_OF_MEMORY          ((UINT32) 0x00000105)
#define DBG_ABRT_CANNOT_LOAD_SECTION    ((UINT32) 0x00000106)
#define DBG_ABRT_CANNOT_FIND_PID_FILTER ((UINT32) 0x00000107)
#define DBG_ABRT_MESSAGE_TOO_LONG       ((UINT32) 0x00000108)
#define DBG_ABRT_CANNOT_CREATE_MSGQ     ((UINT32) 0x00000109)
#define DBG_ABRT_CANNOT_CREATE_THREAD   ((UINT32) 0x00000110)

#define MHEG5_SECF_THREAD_NAME          "mheg5_secf_thread"
#define MHEG5_SECF_THREAD_STACK_SIZE    2048
#define MHEG5_SECF_THREAD_PRIORITY      128

#define MHEG5_SECF_MSGQ_NAME            "mheg5_secf_msgq"
#define MHEG5_SECF_MSGQ_LEN             256
#define MHEG5_SECF_MSGQ_MSG_PRIORITY    128

#define MHEG5_PID_FILTER_BUF_SIZE       (256 * 1024)
#define MAX_SECTION_LEN                 4096

#define MHEG5T_REQ  (HT_GROUP_MHEG_5 + ((HANDLE_TYPE_T) 0))

typedef enum
{
    FILTER_STATE_UNKNOWN,
    FILTER_STATE_PIDF_OPENED,
    FILTER_STATE_PIDF_CONNECTED,
    FILTER_STATE_PIDF_PID_SET,
    FILTER_STATE_PIDF_NFY_SET,
    FILTER_STATE_PIDF_BUFFER_ALLOATED,
    FILTER_STATE_SECF_OPENED,
    FILTER_STATE_SECF_CONNECTED,
    FILTER_STATE_SECF_CONFIGURED
} _FILTER_STATE_T;

typedef struct __FILTER_CTRL_T          _FILTER_CTRL_T;

typedef SLIST_T(__FILTER_CTRL_T)        SLIST_FILTER_CTRL_T;
typedef SLIST_ENTRY_T(__FILTER_CTRL_T)  SLIST_ENTRY_FILTER_CTRL_T;

struct __FILTER_CTRL_T
{
    HANDLE_T                    h_this;
    SLIST_ENTRY_FILTER_CTRL_T   t_next;
    _FILTER_STATE_T             e_state;
    MPEG_2_PID_T                t_pid;
    HANDLE_T                    h_pidf;
    HANDLE_T                    h_secf_0x3b;
    HANDLE_T                    h_secf_0x3c;
    HANDLE_T                    h_secf_0x3d;

#ifdef DEBUG
    UINT32                      ui4_sec_from_dmx;
    UINT32                      ui4_sec_lost_in_isr;
    UINT32                      ui4_sec_lost_in_thrd;
    UINT32                      ui4_sec_sent;
#endif
};

typedef struct __MSG_T
{
    HANDLE_T        h_fc;
    DEMUX_COND_T    e_cond;
    VOID*           pv_src_tag;
    SIZE_T          z_len;
} _MSG_T;

#ifdef CLI_LVL_ALL
#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_ssf_get_dbg_level()

static UINT16    _ui2_mheg5_ssf_dbg_level = DBG_INIT_LEVEL_MW_MHEG5;
#endif

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T _h_tuner  = NULL_HANDLE;
static HANDLE_T _h_lock   = NULL_HANDLE;
static HANDLE_T _h_msg_q  = NULL_HANDLE;
static HANDLE_T _h_thread = NULL_HANDLE;

static SLIST_FILTER_CTRL_T _t_fc_lst;
static UINT8 _aui1_sec_data[MAX_SECTION_LEN];

#ifdef DEBUG
static INT32 _i4_fc_cnt     = 0;
static INT32 _i4_max_fc_cnt = 0;
#endif
/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

static BOOL _config_pid_filter(_FILTER_CTRL_T* pt_fc);
static VOID _db_lock (VOID);
static VOID _db_unlock (VOID);
static BOOL _demux_pid_filter_nfy(
    VOID*           pv_nfy_tag,
    DEMUX_COND_T    e_nfy_cond,
    UINT32          ui4_data_1,
    UINT32          ui4_data_2
#ifndef MW_CLASSIC_DMX_NFY
    ,
    UINT64          ui8_ext_data_1,
    UINT64          ui8_ext_data_2,
    UINT32          ui4_ext_data_3
#endif
    );
static BOOL _filter_ctrl_free(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle);
static _FILTER_CTRL_T* _find_filter(
    MPEG_2_PID_T            t_pid,
    SLIST_FILTER_CTRL_T*    pt_fc_lst);
static VOID _free_filter(_FILTER_CTRL_T* pt_fc);
static VOID _insert_filter(
    _FILTER_CTRL_T*         pt_fc, 
    SLIST_FILTER_CTRL_T*    pt_fc_lst);
static _FILTER_CTRL_T* _new_filter (MPEG_2_PID_T t_pid);
static VOID _pid_filter_open_nfy(
    HANDLE_T    h_open_pid,
    UINT8       ui1_port,
    RM_COND_T   e_nfy_cond,
    VOID*       pv_tag,
    UINT32      ui4_data);
static VOID _remove_filter(_FILTER_CTRL_T* pt_fc);
static VOID _sec_filter_open_nfy(
    HANDLE_T      h_open_sec,
    UINT8         ui1_port,
    RM_COND_T     e_nfy_cond,
    VOID*         pv_tag,
    UINT32        ui4_data);
static VOID _secf_thread_main(VOID* pv_arg);
static BOOL _setup_sec_filter(
    UINT8               ui1_tid,
    _FILTER_CTRL_T*     pt_fc, 
    HANDLE_T*           ph_secf);
static VOID _src_open_nfy(
    HANDLE_T    h_open_src,
    UINT8       ui1_port,
    RM_COND_T   e_nfy_cond,
    VOID*       pv_tag,
    UINT32      ui4_data);

/*-----------------------------------------------------------------------------
 * Name:  mheg5_section_init
 *
 * Description: This API initializes the MHEG-5 section acquisition module.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID mheg5_section_init (const CHAR* ps_tuner_name)
{
    RM_COND_T e_cond;
    
    /* Open the tuner component */
    if (rm_open_comp(
        NULL_HANDLE,
        OBJ_COMP_REL_IGNORE,
        DRVT_TUNER_TER_DIG,
        1,
        NULL,                   /* name */
        CTRL_TYPE_MONITOR,
        NULL,
        NULL,
        NULL,                   /* nfy tag */
        (rm_nfy_fct) _src_open_nfy,
        &_h_tuner,
        &e_cond) != RMR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_OPEN_TUNER);
    }
        
    /* And create the lock. */
    if (x_sema_create (&_h_lock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_SEMA);
    }

    if (x_msg_q_create(
        &_h_msg_q, 
        MHEG5_SECF_MSGQ_NAME,
        sizeof(_MSG_T), 
        MHEG5_SECF_MSGQ_LEN) != OSR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_MSGQ);
    }
    
    if (OSR_OK != x_thread_create(
        &_h_thread, 
        MHEG5_SECF_THREAD_NAME, 
        MHEG5_SECF_THREAD_STACK_SIZE,
        MHEG5_SECF_THREAD_PRIORITY,
        _secf_thread_main,
        0,
        NULL))
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_THREAD);
    }

    SLIST_INIT(&_t_fc_lst);
}

static BOOL _config_pid_filter(_FILTER_CTRL_T* pt_fc)
{
    DEMUX_NFY_INFO_T    t_demux_nfy;
    RM_COND_T           e_cond;
    INT32               i4_ret;
    BOOL                b_succeeded = FALSE;
    
    i4_ret = rm_set(
        pt_fc->h_pidf,
        FALSE,
        ANY_PORT_NUM,
        DEMUX_SET_TYPE_CTRL,
        (const VOID *) FALSE,
        (SIZE_T) sizeof(BOOL),
        &e_cond);

    if (RMR_OK == i4_ret)
    {
        i4_ret = rm_set(
            pt_fc->h_pidf,
            FALSE,
            ANY_PORT_NUM,
            DEMUX_SET_TYPE_PID,
            (const VOID *) ((UINT32) pt_fc->t_pid),
            (SIZE_T) sizeof(MPEG_2_PID_T),
            &e_cond);

        if (i4_ret == RMR_OK)
        {
            pt_fc->e_state = FILTER_STATE_PIDF_PID_SET;
                
            t_demux_nfy.pv_tag = (VOID*) (pt_fc->h_this);
            t_demux_nfy.pf_demux_nfy = _demux_pid_filter_nfy;

            i4_ret = rm_set(
                pt_fc->h_pidf,
                FALSE,
                ANY_PORT_NUM,
                DEMUX_SET_TYPE_NFY_FCT,
                (const VOID *) &t_demux_nfy,
                (SIZE_T) sizeof(DEMUX_NFY_INFO_T),
                &e_cond);

            if (RMR_OK == i4_ret)
            {
                pt_fc->e_state = FILTER_STATE_PIDF_NFY_SET;
                
                i4_ret = rm_set(
                    pt_fc->h_pidf,
                    FALSE,
                    ANY_PORT_NUM,
                    DEMUX_SET_TYPE_ALLOC_BUFFER,
                    (const VOID*) MHEG5_PID_FILTER_BUF_SIZE,
                    (SIZE_T) sizeof(SIZE_T),
                    &e_cond);

                if (RMR_OK == i4_ret)
                {
                    pt_fc->e_state = FILTER_STATE_PIDF_BUFFER_ALLOATED;
                    
                    i4_ret = rm_set(
                        pt_fc->h_pidf,
                        FALSE,
                        ANY_PORT_NUM,
                        DEMUX_SET_TYPE_CTRL,
                        (const VOID *) TRUE,
                        (SIZE_T) sizeof(BOOL),
                        &e_cond);

                    if (RMR_OK == i4_ret)
                    {
                        b_succeeded = TRUE;
                    }
                }
            }
        }
    }

    return (b_succeeded);
}

/*-----------------------------------------------------------------------------
 * Name:  db_lock
 *
 * Description: This API locks the database semaphore. This API will abort if
 *              an error is detected.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _db_lock (VOID)
{
    if (x_sema_lock (_h_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
    }
}


/*-----------------------------------------------------------------------------
 * Name:  db_unlock
 *
 * Description: This API unlocks the database semaphore. This API will abort
 *              if an error is detected.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _db_unlock (VOID)
{
    if (x_sema_unlock (_h_lock) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
    }
}

static BOOL _demux_pid_filter_nfy(
    VOID*           pv_nfy_tag,
    DEMUX_COND_T    e_nfy_cond,
    UINT32          ui4_data_1,
    UINT32          ui4_data_2
#ifndef MW_CLASSIC_DMX_NFY
    ,
    UINT64          ui8_ext_data_1,
    UINT64          ui8_ext_data_2,
    UINT32          ui4_ext_data_3
#endif
    )
{
    _MSG_T          t_msg;
    BOOL            b_ret = FALSE;
#ifdef DEBUG
    _FILTER_CTRL_T* pt_fc = NULL;
    HANDLE_TYPE_T   e_type;
#endif

    if (((e_nfy_cond != DEMUX_COND_DATA_REC) && 
         (e_nfy_cond != DEMUX_COND_OVERFLOW)) ||
        ((e_nfy_cond == DEMUX_COND_DATA_REC) &&
         ((ui4_data_2 > (UINT32) MAX_SECTION_LEN) || (ui4_data_2 < (UINT32) 3))))
    {
        DBG_ERROR(("{MHEG-5 CI} _demux_pid_filter_nfy: INVALID section (%d)\n", (UINT32) pt_fc->t_pid));
        return(FALSE);
    }

#ifdef DEBUG
    if (HR_OK != handle_get_type_obj((HANDLE_T) pv_nfy_tag, &e_type, (VOID**) &pt_fc) ||
        MHEG5T_REQ != e_type)
    {
        pt_fc = NULL;
    }

    if (pt_fc)
    {
        pt_fc->ui4_sec_from_dmx++;
    }
#endif
    
    t_msg.h_fc       = (HANDLE_T) pv_nfy_tag;
    t_msg.e_cond     = e_nfy_cond;
    t_msg.pv_src_tag = (VOID*) ui4_data_1;
    t_msg.z_len      = (SIZE_T) ui4_data_2;

    if (OSR_OK == x_msg_q_send(
        _h_msg_q,
        &t_msg,
        sizeof(t_msg),
        MHEG5_SECF_MSGQ_MSG_PRIORITY))
    {
        b_ret = TRUE;
    }
    else
    {
    #ifdef DEBUG
        if (pt_fc)
        {
            pt_fc->ui4_sec_lost_in_isr++;
        }
    #endif
    }
        
    return(b_ret);
}

static _FILTER_CTRL_T* _find_filter(
    MPEG_2_PID_T            t_pid,
    SLIST_FILTER_CTRL_T*    pt_fc_lst)
{
    _FILTER_CTRL_T* pt_fc;

    SLIST_FOR_EACH(pt_fc, pt_fc_lst, t_next)
    {
        if (t_pid == pt_fc->t_pid)
        {
            break;
        }
    }

    return (pt_fc);
}

static VOID _remove_filter(_FILTER_CTRL_T* pt_fc)
{
    SLIST_REMOVE(pt_fc, t_next);
}

static BOOL _filter_ctrl_free(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    if ((e_type == MHEG5T_REQ) &&
        (pv_obj !=  NULL))
    {
        _free_filter ((_FILTER_CTRL_T*) pv_obj);
    }
    else
    {
        /* This must never happen. very bad. */
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARGS);
    }

    return (TRUE);
}

static VOID _free_filter(_FILTER_CTRL_T* pt_fc)
{
    RM_COND_T e_cond;
    
    if (NULL_HANDLE != pt_fc->h_secf_0x3b)
    {
        rm_disconnect(
            pt_fc->h_secf_0x3b, 
            (DRV_DISC_TYPE_T) NULL,
            NULL,
            (SIZE_T) 0, 
            &e_cond);

        rm_close(pt_fc->h_secf_0x3b, FALSE, &e_cond);
    }

    if (NULL_HANDLE != pt_fc->h_secf_0x3c)
    {
        rm_disconnect(
            pt_fc->h_secf_0x3c, 
            (DRV_DISC_TYPE_T) NULL,
            NULL,
            (SIZE_T) 0, 
            &e_cond);
        
        rm_close(pt_fc->h_secf_0x3c, FALSE, &e_cond);
    }

    if (NULL_HANDLE != pt_fc->h_secf_0x3d)
    {
        rm_disconnect(
            pt_fc->h_secf_0x3d, 
            (DRV_DISC_TYPE_T) NULL,
            NULL,
            (SIZE_T) 0, 
            &e_cond);
        
        rm_close(pt_fc->h_secf_0x3d, FALSE, &e_cond);
    }

    if (NULL_HANDLE != pt_fc->h_pidf)
    {
        if (pt_fc->e_state >= FILTER_STATE_PIDF_BUFFER_ALLOATED)
        {
            rm_set(
                pt_fc->h_pidf,
                FALSE,
                ANY_PORT_NUM,
                DEMUX_SET_TYPE_FLUSH_BUFFER,
                NULL,
                (SIZE_T) 0,
                &e_cond);

            rm_set(
                pt_fc->h_pidf,
                FALSE,
                ANY_PORT_NUM,
                DEMUX_SET_TYPE_FREE_BUFFER,
                NULL,
                (SIZE_T) 0,
                &e_cond);
        }

        rm_disconnect(
            pt_fc->h_pidf,
            (DRV_DISC_TYPE_T) NULL,
            NULL,
            (SIZE_T) 0, 
            &e_cond);
            
        rm_close(pt_fc->h_pidf, FALSE, &e_cond);
    }

    x_mem_free(pt_fc);
}

static VOID _insert_filter(
    _FILTER_CTRL_T*         pt_fc, 
    SLIST_FILTER_CTRL_T*    pt_fc_lst)
{
    SLIST_INSERT_HEAD(pt_fc, pt_fc_lst, t_next);
}

static _FILTER_CTRL_T* _new_filter (MPEG_2_PID_T t_pid)
{
    HANDLE_T        h_obj;
    _FILTER_CTRL_T* pt_fc;
    INT32           i4_ret;
    RM_COND_T       e_cond;
    BOOL            b_succeeded = FALSE;
    
    pt_fc = x_mem_alloc(sizeof(_FILTER_CTRL_T));

    if (pt_fc)
    {
        pt_fc->h_this      = NULL_HANDLE;
        pt_fc->e_state     = FILTER_STATE_UNKNOWN;
        pt_fc->t_pid       = t_pid;
        pt_fc->h_pidf      = NULL_HANDLE;
        pt_fc->h_secf_0x3b = NULL_HANDLE;
        pt_fc->h_secf_0x3c = NULL_HANDLE;
        pt_fc->h_secf_0x3d = NULL_HANDLE;
    #ifdef DEBUG
        pt_fc->ui4_sec_from_dmx     = 0;
        pt_fc->ui4_sec_lost_in_isr  = 0;
        pt_fc->ui4_sec_lost_in_thrd = 0;
        pt_fc->ui4_sec_sent         = 0;
    #endif

        if (handle_alloc(
            MHEG5T_REQ,
            pt_fc,
            NULL,
            _filter_ctrl_free,
            &h_obj) == HR_OK)
        {
            pt_fc->h_this = h_obj;
        
            /* open a DRVT_DEMUX_TS_SECTION_MEMORY component */
            i4_ret = rm_open_comp(
                NULL_HANDLE,
                OBJ_COMP_REL_IGNORE,
                DRVT_DEMUX_TS_SECTION_MEMORY,
                ANY_COMP_ID,
                NULL,                         /* name */
                CTRL_TYPE_SINGLE,
                NULL,
                NULL,
                (VOID*)((UINT32)t_pid),
                (rm_nfy_fct) _pid_filter_open_nfy,
                &(pt_fc->h_pidf),
                &e_cond);

            if ((RMR_OK == i4_ret) &&
                (RM_COND_OPENED == rm_cond_chg(e_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE)))
            {
                pt_fc->e_state = FILTER_STATE_PIDF_OPENED;
                
                i4_ret = rm_connect(
                    pt_fc->h_pidf,
                    RM_CONN_TYPE_HANDLE,
                    (VOID *) _h_tuner,
                    sizeof(HANDLE_T),
                    NULL,
                    &e_cond);

                if (RMR_OK == i4_ret)
                {
                    pt_fc->e_state = FILTER_STATE_PIDF_CONNECTED;
                    
                    if (_config_pid_filter(pt_fc))
                    {
                        if (_setup_sec_filter(
                            (UINT8) 0x3b,
                            pt_fc,
                            &(pt_fc->h_secf_0x3b)))
                        {
                            if (_setup_sec_filter(
                                (UINT8) 0x3c,
                                pt_fc,
                                &(pt_fc->h_secf_0x3c)))
                            {
                                if (_setup_sec_filter(
                                    (UINT8) 0x3d,
                                    pt_fc,
                                    &(pt_fc->h_secf_0x3d)))
                                {
                                    pt_fc->e_state = FILTER_STATE_SECF_CONFIGURED;
                                    b_succeeded = TRUE;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (!b_succeeded)
    {
        if (pt_fc)
        {
            if (NULL_HANDLE != pt_fc->h_this)
            {
                handle_free(pt_fc->h_this, TRUE);
            }
            else
            {
                x_mem_free(pt_fc);
            }
            
            pt_fc = NULL;
        }
    }

    return (pt_fc);
}

static VOID _pid_filter_open_nfy(
    HANDLE_T    h_open_pid,
    UINT8       ui1_port,
    RM_COND_T   e_nfy_cond,
    VOID*       pv_tag,
    UINT32      ui4_data)
{
    RM_COND_T e_rm_cond;
    
    e_rm_cond = e_nfy_cond & RM_COND_CONNECTED;
    
    if (RM_COND_CONNECTED == e_rm_cond)
    {
        DBG_INFO(("{MHEG-5 CI} PID filter (%d) CONNECTED....\n", (UINT32) pv_tag));
    }
    else
    {
        DBG_INFO(("{MHEG-5 CI} PID filter (%d) DISCONNECTED....\n", (UINT32) pv_tag));
    }
}

static VOID _sec_filter_open_nfy(
    HANDLE_T      h_open_sec,
    UINT8         ui1_port,
    RM_COND_T     e_nfy_cond,
    VOID*         pv_tag,
    UINT32        ui4_data)
{
    RM_COND_T e_rm_cond = e_nfy_cond & RM_COND_CONNECTED;
    
    if (RM_COND_CONNECTED == e_rm_cond)
    {
        DBG_INFO(("{MHEG-5 CI} PID filter (%d) CONNECTED....\n", (UINT32) pv_tag));
    }
    else
    {
        DBG_INFO(("{MHEG-5 CI} PID filter (%d) DISCONNECTED....\n", (UINT32) pv_tag));
    }
}

static VOID _secf_thread_main(VOID* pv_arg)
{
    SIZE_T                      z_msg_size = sizeof(_MSG_T);
    SIZE_T                      z_size;
    UINT16                      ui2_index = 0;
    INT32                       i4_ret;
    _MSG_T                      t_msg;
    RM_COND_T                   e_cond;
    DEMUX_READ_BUFFER_INFO_T    t_read;
    DEMUX_UNLOCK_BUFFER_INFO_T  t_unlock_buf;
    _FILTER_CTRL_T*             pt_fc;
    HANDLE_TYPE_T               e_type;
    
    while (1)
    {
        i4_ret = x_msg_q_receive(
            &ui2_index,
            (VOID*) &t_msg,
            &z_msg_size,
            &_h_msg_q,
            1,
            X_MSGQ_OPTION_WAIT);

        if (OSR_OK == i4_ret)
        {
            _db_lock();
            
            if ((HR_OK == handle_get_type_obj(t_msg.h_fc, &e_type, (VOID**) &pt_fc)) &&
                (MHEG5T_REQ == e_type) &&
                (NULL != pt_fc))
            {
                switch (t_msg.e_cond)
                {
                    case DEMUX_COND_DATA_REC:
                        t_read.pui1_dest = &(_aui1_sec_data[0]);
                        t_read.pv_src_tag = t_msg.pv_src_tag;
                        t_read.z_skip_len = 0;
                        t_read.z_copy_len = t_msg.z_len;

                        DBG_INFO(("{MHEG5 CI}DEMUX_COND_DATA_REC!\n"));

                        z_size = sizeof(DEMUX_READ_BUFFER_INFO_T);

                        i4_ret = rm_get(
                            pt_fc->h_pidf,
                            FALSE,
                            ANY_PORT_NUM,
                            DEMUX_GET_TYPE_READ_BUFFER,
                            (VOID *) &t_read,
                            (SIZE_T *) &z_size,
                            &e_cond);

                        if (RMR_OK == i4_ret)
                        {
                            MHEGSSF_ProcessDvpSection(
                                (void*) &(_aui1_sec_data[0]),
                                (U32BIT)t_msg.z_len,
                                &(pt_fc->t_pid));

                        #ifdef DEBUG
                            pt_fc->ui4_sec_sent++;
                        #endif

                            /* force context switch to make MHEG engine be able to process sections [DTV00129734] */
                            x_thread_delay(1);
                        }
                        else
                        {
                        #ifdef DEBUG
                            pt_fc->ui4_sec_lost_in_thrd++;
                        #endif
                            DBG_ERROR((
                                "{MHEG5 CI} Get section from DMX FAILED (%d)!\n",
                                i4_ret));
                        }
                        x_memset(&t_unlock_buf, 0, sizeof(DEMUX_UNLOCK_BUFFER_INFO_T));
                        t_unlock_buf.pv_src_tag = t_msg.pv_src_tag;
                        t_unlock_buf.z_src_len  = t_msg.z_len;
                        z_size = sizeof (DEMUX_UNLOCK_BUFFER_INFO_T);

                        rm_set(pt_fc->h_pidf,
                                FALSE,
                                0,
                                DEMUX_SET_TYPE_UNLOCK_BUFFER,
                                (VOID *) &t_unlock_buf,
                                z_size,
                                &e_cond);

                        break;
                        
                    case DEMUX_COND_OVERFLOW:
                        DBG_INFO(("{MHEG5 CI} DEMUX overflow. Flush DEMUX buffer!\n"));
                            
                        rm_set(
                            pt_fc->h_pidf,
                            FALSE,
                            ANY_PORT_NUM,
                            DEMUX_SET_TYPE_FLUSH_BUFFER,
                            NULL,
                            (SIZE_T) 0,
                            &e_cond);

                        rm_set(
                            pt_fc->h_pidf,
                            FALSE,
                            ANY_PORT_NUM,
                            DEMUX_SET_TYPE_CTRL,
                            (const VOID*) ((UINT32) TRUE),
                            (SIZE_T) sizeof(BOOL),
                            &e_cond);
                        break;

                    default:
                        break;
                }
            }

            _db_unlock();
        }
    }
}
static BOOL _setup_sec_filter(
    UINT8               ui1_tid,
    _FILTER_CTRL_T*     pt_fc, 
    HANDLE_T*           ph_secf)
{
    #define PATTERN_INIT_ARRAY  {0, 0, 0, 0, 0, 0, 0, 0}
    
    DEMUX_SECTION_FILTER_INFO_T t_sec_info;
    RM_COND_T                   e_cond;
    INT32                       i4_ret;
    BOOL                        b_succeeded = FALSE;
    UINT8                       aui1_pattern_mask[8] = PATTERN_INIT_ARRAY;
    
    i4_ret = rm_open_comp(
        NULL_HANDLE,
        OBJ_COMP_REL_IGNORE,
        DRVT_DEMUX_SECTION_FILTER,
        ANY_COMP_ID,
        NULL,                         /* name */
        CTRL_TYPE_SINGLE,
        NULL,
        NULL,
        NULL,
        (rm_nfy_fct) _sec_filter_open_nfy,
        ph_secf,
        &e_cond);

    if ((RMR_OK == i4_ret) &&
        (RM_COND_OPENED == rm_cond_chg(e_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE)))
    {
        pt_fc->e_state = FILTER_STATE_SECF_OPENED;

        i4_ret = rm_connect(
            *ph_secf,
            RM_CONN_TYPE_HANDLE,
            (const VOID *) (pt_fc->h_pidf),
            sizeof(HANDLE_T),
            (VOID *) NULL,
            &e_cond);

        if (RMR_OK == i4_ret)
        {
            pt_fc->e_state = FILTER_STATE_SECF_CONNECTED;

            i4_ret = rm_set(
                *ph_secf,
                FALSE,
                ANY_PORT_NUM,
                DEMUX_SET_TYPE_CTRL,
                (const VOID*) ((UINT32) FALSE),
                (SIZE_T) sizeof(BOOL),
                &e_cond);

            if (RMR_OK == i4_ret)
            {
                t_sec_info.ui1_tbl_id_pattern = ui1_tid;
                t_sec_info.ui1_tbl_id_mask    = 0xff;
                t_sec_info.ui1_pattern_len    = 0;
                t_sec_info.ui1_pattern_offset = 0;
                t_sec_info.pui1_pos_pattern   = &(aui1_pattern_mask[0]);
                t_sec_info.pui1_pos_mask      = &(aui1_pattern_mask[0]);
                t_sec_info.pui1_neg_pattern   = NULL;
                t_sec_info.pui1_neg_mask      = NULL;
                t_sec_info.b_check_crc        = TRUE;
                
                i4_ret = rm_set(
                    *ph_secf,
                    FALSE,
                    ANY_PORT_NUM,
                    DEMUX_SET_TYPE_SECTION_FILTER,
                    (const VOID *) &t_sec_info,
                    (SIZE_T) sizeof(DEMUX_SECTION_FILTER_INFO_T),
                    &e_cond);

                if (RMR_OK == i4_ret)
                {
                    i4_ret = rm_set(
                        *ph_secf,
                        FALSE,
                        ANY_PORT_NUM,
                        DEMUX_SET_TYPE_CTRL,
                        (const VOID*) ((UINT32) TRUE),
                        (SIZE_T) sizeof(BOOL),
                        &e_cond);

                    if (RMR_OK == i4_ret)
                    {
                        b_succeeded = TRUE;
                    }
                }
            }
        }
    }

    return (b_succeeded);
}

static VOID _src_open_nfy(
    HANDLE_T    h_open_src,
    UINT8       ui1_port,
    RM_COND_T   e_nfy_cond,
    VOID*       pv_tag,
    UINT32      ui4_data)
{
    if (rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_MON_COMP_STATE) == RM_COND_COMP_ACTIVE)
    {
        DBG_INFO(("{MHEG-5 CI} Tuner becomes ACTIVE....\n"));
    }
    else
    {
        DBG_INFO(("{MHEG-5 CI} Tuner becomes INACTIVE....\n"));
    }

    if (rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_CONN_STATE) == RM_COND_CONNECTED)
    {
        DBG_INFO(("{MHEG-5 CI} Tuner becomes CONNECTED....\n"));
    }
    else
    {
        DBG_INFO(("{MHEG-5 CI} Tuner becomes DISCONNECTED....\n"));
    }
}

void MHEGSSF_HwInit(void)
{
}

void MHEGSSF_FreeHwBuffer(void* pSectionInfo)
{
}

void MHEGSSF_SetupHwFilters(U16BIT pid)
{
    _FILTER_CTRL_T* pt_fc;
    
    if (((MPEG_2_PID_T) pid) < MPEG_2_NULL_PID)
    {
        DBG_INFO(("{MHEG-5 CI} MHEGSSF_SetupHwFilters(PID=%d)\n", pid));

        _db_lock();
        
        pt_fc = _find_filter((MPEG_2_PID_T) pid, &_t_fc_lst);

        if (pt_fc)
        {
            DBG_INFO(("{MHEG-5 CI} MHEGSSF_SetupHwFilters(PID=%d): filter already existed\n", pid));
        }
        else
        {
            /* Create a new section control object. */
            if ((pt_fc = _new_filter ((MPEG_2_PID_T) pid)) != NULL)
            {
                _insert_filter(pt_fc, &_t_fc_lst);

            #ifdef DEBUG
                _i4_fc_cnt++;

                if (_i4_fc_cnt > _i4_max_fc_cnt)
                {
                    _i4_max_fc_cnt = _i4_fc_cnt;
                }
            #endif
            }
            else
            {
                DBG_ERROR(("{MHEG-5 CI} MHEGSSF_SetupHwFilters(PID=%d): create filter failed\n", pid));
            }
        }

        _db_unlock();
    }
}

void MHEGSSF_RemoveHwFilters(U16BIT pid)
{
    _FILTER_CTRL_T* pt_fc;
    
    if (((MPEG_2_PID_T) pid) < MPEG_2_NULL_PID)
    {
        DBG_INFO(("{MHEG-5 CI} MHEGSSF_RemoveHwFilters(PID=%d)\n", pid));

        _db_lock();
        
        pt_fc = _find_filter((MPEG_2_PID_T) pid, &_t_fc_lst);

        if (pt_fc)
        {
            _remove_filter(pt_fc);
            handle_free(pt_fc->h_this, TRUE);
            DBG_INFO(("{MHEG-5 CI} MHEGSSF_RemoveHwFilters(PID=%d): freed\n", pid));
        #ifdef DEBUG
            _i4_fc_cnt--;
        #endif
        }
        else
        {
            DBG_ERROR(("{MHEG-5 CI} MHEGSSF_RemoveHwFilters(PID=%d): not found\n", pid));
        }

        _db_unlock();
    }
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: mheg5_ssf_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 mheg5_ssf_get_dbg_level(VOID)
{
    return _ui2_mheg5_ssf_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: mheg5_ssf_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
BOOL mheg5_ssf_set_dbg_level(UINT16 ui2_level)
{

    _ui2_mheg5_ssf_dbg_level = ui2_level;
    return TRUE;

}
#endif

