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
 * $RCSfile: x_dmx_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/x_handle_grp.h"
#include "os/inc/os.h"
#include "dbg/u_dbg.h"
#include "res_mngr/drv/x_demux.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "util/x_lnk_list.h"

#include "dmx_util/u_dmx_util.h"
#include "dmx_util/x_dmx_util.h"


#ifdef CLI_LVL_ALL
#include "dmx_util/dmx_util_cli.h"
#endif /* CLI_LVL_ALL */

#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{DMX_UT}Info: "
#define _API_HEADER         "{DMX_UT}API: "
#define _ERROR_HEADER       "{DMX_UT}Err: "

#undef  DBG_INFO
#undef  DBG_ERROR
#define DBG_INFO(p)  x_dbg_stmt p
#define DBG_ERROR(p) x_dbg_stmt p


#define DMX_UT_STR      "{DMX_UT}"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define ABORT(_cat, _val) DBG_ABORT ((DBG_MOD_DMX_UTIL | _cat | _val))

#define DBG_ABRT_CANNOT_OPEN_TUNER      ((UINT32) 0x00000100)
#define DBG_ABRT_CANNOT_CREATE_SEMA     ((UINT32) 0x00000101)
#define DBG_ABRT_CANNOT_LOCK_SEMA       ((UINT32) 0x00000102)
#define DBG_ABRT_CANNOT_UNLOCK_SEMA     ((UINT32) 0x00000103)
#define DBG_ABRT_INV_ARGS               ((UINT32) 0x00000104)
#define DBG_ABRT_OUT_OF_MEMORY          ((UINT32) 0x00000105)
#define DBG_ABRT_CANNOT_LOAD_SECTION    ((UINT32) 0x00000106)
#define DBG_ABRT_CANNOT_FIND_PID_FILTER ((UINT32) 0x00000107)
#define DBG_ABRT_CANNOT_CREATE_MSGQ     ((UINT32) 0x00000108)
#define DBG_ABRT_CANNOT_CREATE_THREAD   ((UINT32) 0x00000109)

#define DMX_UT_THREAD_NAME              "dmx_ut_thread"
#define DMX_UT_THREAD_STACK_SIZE        2048
#define DMX_UT_THREAD_PRIORITY          128

#define DMX_UT_MSGQ_NAME                "dmx_ut_msgq"
#define DMX_UT_MSGQ_LEN                 256
#define DMX_UT_MSGQ_MSG_PRIORITY        128

#define MAX_SECTION_LEN                 4096

#define DMX_UT_HT_PIDF                   (HT_GROUP_DMX_UTIL + ((HANDLE_TYPE_T) 0))
#define DMX_UT_HT_SECF                   (HT_GROUP_DMX_UTIL + ((HANDLE_TYPE_T) 1))

typedef enum
{
    DMX_UT_PIDF_CTRL_STATE_UNKNOWN,
    DMX_UT_PIDF_CTRL_STATE_PIDF_OPENED,
    DMX_UT_PIDF_CTRL_STATE_PIDF_CONNECTED,
    DMX_UT_PIDF_CTRL_STATE_PIDF_PID_SET,
    DMX_UT_PIDF_CTRL_STATE_PIDF_NFY_SET,
    DMX_UT_PIDF_CTRL_STATE_PIDF_BUFFER_ALLOATED,
    DMX_UT_PIDF_CTRL_STATE_SECF_OPENED,
    DMX_UT_PIDF_CTRL_STATE_SECF_CONNECTED,
    DMX_UT_PIDF_CTRL_STATE_SECF_CONFIGURED
} DMX_UT_PIDF_STATE_T;

typedef struct _DMX_UT_PIDF_CTRL_T          DMX_UT_PIDF_CTRL_T;

typedef SLIST_T(_DMX_UT_PIDF_CTRL_T)        SLIST_DMX_UT_PIDF_CTRL_T;
typedef SLIST_ENTRY_T(_DMX_UT_PIDF_CTRL_T)  SLIST_ENTRY_PIDF_CTRL_T;

struct _DMX_UT_PIDF_CTRL_T
{
    HANDLE_T                    h_this;
    SLIST_ENTRY_PIDF_CTRL_T     t_next;
    DMX_UT_PIDF_STATE_T         e_state;
    MPEG_2_PID_T                t_pid;
    HANDLE_T                    h_pidf;

    x_dmx_ut_nfy_fct            pf_dmx_ut_nfy;
    UINT32                      ui4_nfy_data;
    
    UINT32                      ui4_secf_count;
    
#ifdef DEBUG
    UINT32                      ui4_sec_from_dmx;
    UINT32                      ui4_sec_lost_in_isr;
    UINT32                      ui4_sec_lost_in_thrd;
    UINT32                      ui4_sec_sent;
#endif
};

typedef struct _DMX_UT_SECF_CTRL_T
{
    HANDLE_T    h_this;
    HANDLE_T    h_pidf_ctrl;
    HANDLE_T    h_secf;
} DMX_UT_SECF_CTRL_T;

typedef struct _DMX_UT_MSG_T
{
    HANDLE_T        h_pidf_ctrl;
    DEMUX_COND_T    e_cond;
    VOID*           pv_src_tag;
    SIZE_T          z_len;
} DMX_UT_MSG_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL     _b_dmx_ut_init  = FALSE;
static HANDLE_T _h_tuner        = NULL_HANDLE;
static HANDLE_T _h_lock         = NULL_HANDLE;
static HANDLE_T _h_msg_q        = NULL_HANDLE;
static HANDLE_T _h_thread       = NULL_HANDLE;


static SLIST_DMX_UT_PIDF_CTRL_T _t_pidf_ctrl_lst;

#ifdef DEBUG
static INT32 _i4_pidf_cnt     = 0;
static INT32 _i4_max_pidf_cnt = 0;
#endif

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
 
 
/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/
static VOID _db_lock (VOID)
{
    if (x_sema_lock (_h_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
    }
}

static VOID _db_unlock (VOID)
{
    if (x_sema_unlock (_h_lock) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
    }
}


static VOID _insert_pidf_ctrl(
    DMX_UT_PIDF_CTRL_T*         pt_pidf_ctrl, 
    SLIST_DMX_UT_PIDF_CTRL_T*   pt_pidf_ctrl_lst)
{
    SLIST_INSERT_HEAD(pt_pidf_ctrl, pt_pidf_ctrl_lst, t_next);
}

static DMX_UT_PIDF_CTRL_T* _find_pidf(
    MPEG_2_PID_T                t_pid,
    SLIST_DMX_UT_PIDF_CTRL_T*   pt_pidf_ctrl_lst)
{
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL;

    SLIST_FOR_EACH(pt_pidf_ctrl, pt_pidf_ctrl_lst, t_next)
    {
        if (t_pid == pt_pidf_ctrl->t_pid)
        {
            break;
        }
    }

    return (pt_pidf_ctrl);
}

static VOID _pid_ctrl_free_filter(DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl)
{
    RM_COND_T e_cond;
    
    if (NULL_HANDLE != pt_pidf_ctrl->h_pidf)
    {
        if (pt_pidf_ctrl->e_state >= DMX_UT_PIDF_CTRL_STATE_PIDF_BUFFER_ALLOATED)
        {
            rm_set(
                pt_pidf_ctrl->h_pidf,
                FALSE,
                ANY_PORT_NUM,
                DEMUX_SET_TYPE_FLUSH_BUFFER,
                NULL,
                (SIZE_T) 0,
                &e_cond);

            rm_set(
                pt_pidf_ctrl->h_pidf,
                FALSE,
                ANY_PORT_NUM,
                DEMUX_SET_TYPE_FREE_BUFFER,
                NULL,
                (SIZE_T) 0,
                &e_cond);
        }

        rm_disconnect(
            pt_pidf_ctrl->h_pidf,
            (DRV_DISC_TYPE_T) NULL,
            NULL,
            (SIZE_T) 0, 
            &e_cond);
            
        rm_close(pt_pidf_ctrl->h_pidf, FALSE, &e_cond);
    }

    x_mem_free(pt_pidf_ctrl);
}


static BOOL _filter_ctrl_free(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    if ((e_type == DMX_UT_HT_PIDF) &&
        (pv_obj !=  NULL))
    {
        _pid_ctrl_free_filter ((DMX_UT_PIDF_CTRL_T*) pv_obj);
    }
    else
    {
        /* This must never happen. very bad. */
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARGS);
    }

    return (TRUE);
}

static BOOL _secf_ctrl_obj_free(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL;
    DMX_UT_SECF_CTRL_T* pt_secf_ctrl = (DMX_UT_SECF_CTRL_T*)pv_obj;
    RM_COND_T           e_cond;
    HANDLE_TYPE_T       e_pidf_type;
    
    if ((e_type == DMX_UT_HT_SECF) &&
        (pt_secf_ctrl !=  NULL))
    {
        /* Disconnect / Close RM component */
        if (pt_secf_ctrl->h_this != NULL_HANDLE)
        {
            rm_disconnect(
                pt_secf_ctrl->h_this, 
                (DRV_DISC_TYPE_T) NULL,
                NULL,
                (SIZE_T) 0, 
                &e_cond);
            
            rm_close(pt_secf_ctrl->h_this, FALSE, &e_cond);
            pt_secf_ctrl->h_this = NULL_HANDLE;
        }

        if (pt_secf_ctrl->h_pidf_ctrl != NULL_HANDLE)
        {
            if ((HR_OK == handle_get_type_obj(pt_secf_ctrl->h_pidf_ctrl, &e_pidf_type, (VOID**) &pt_pidf_ctrl)) &&
                (DMX_UT_HT_PIDF == e_pidf_type) &&
                (NULL != pt_pidf_ctrl))
            {
                if (pt_pidf_ctrl->ui4_secf_count != 0)
                {
                    pt_pidf_ctrl->ui4_secf_count--;
                }
            }
        }

        x_mem_free(pt_secf_ctrl);
    }
    else
    {
        /* This must never happen. very bad. */
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARGS);
    }

    return (TRUE);
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
    DMX_UT_MSG_T    t_msg;
    BOOL            b_ret = FALSE;
#ifdef DEBUG
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL;
    HANDLE_TYPE_T   e_type;
#endif

    if (((e_nfy_cond != DEMUX_COND_DATA_REC) && 
         (e_nfy_cond != DEMUX_COND_OVERFLOW)) ||
        ((e_nfy_cond == DEMUX_COND_DATA_REC) &&
         ((ui4_data_2 > (UINT32) MAX_SECTION_LEN) || (ui4_data_2 < (UINT32) 3))))
    {
    #ifdef DEBUG
        DBG_ERROR((_ERROR_HEADER"_demux_pid_filter_nfy: INVALID section (%d)\n", (UINT32) pt_pidf_ctrl->t_pid));
    #endif
        return(FALSE);
    }

#ifdef DEBUG
    if (HR_OK != handle_get_type_obj((HANDLE_T) pv_nfy_tag, &e_type, (VOID**) &pt_pidf_ctrl) ||
        DMX_UT_HT_PIDF != e_type)
    {
        pt_pidf_ctrl = NULL;
    }

    if (pt_pidf_ctrl)
    {
        pt_pidf_ctrl->ui4_sec_from_dmx++;
    }
#endif
    
    t_msg.h_pidf_ctrl       = (HANDLE_T) pv_nfy_tag;
    t_msg.e_cond     = e_nfy_cond;
    t_msg.pv_src_tag = (VOID*) ui4_data_1;
    t_msg.z_len      = (SIZE_T) ui4_data_2;

    if (OSR_OK == x_msg_q_send(
        _h_msg_q,
        &t_msg,
        sizeof(t_msg),
        DMX_UT_MSGQ_MSG_PRIORITY))
    {
        b_ret = TRUE;
    }
    else
    {
    #ifdef DEBUG
        if (pt_pidf_ctrl)
        {
            pt_pidf_ctrl->ui4_sec_lost_in_isr++;
        }
    #endif
    }
        
    return(b_ret);
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
        DBG_ERROR((_ERROR_HEADER"Tuner becomes ACTIVE....\n"));
    }
    else
    {
        DBG_ERROR((_ERROR_HEADER"Tuner becomes INACTIVE....\n"));
    }

    if (rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_CONN_STATE) == RM_COND_CONNECTED)
    {
        DBG_ERROR((_ERROR_HEADER"Tuner becomes CONNECTED....\n"));
    }
    else
    {
        DBG_ERROR((_ERROR_HEADER"Tuner becomes DISCONNECTED....\n"));
    }
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
        DBG_ERROR((_ERROR_HEADER"PID filter (%d) CONNECTED....\n", (UINT32) pv_tag));
    }
    else
    {
        DBG_ERROR((_ERROR_HEADER"PID filter (%d) DISCONNECTED....\n", (UINT32) pv_tag));
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
        DBG_ERROR((_ERROR_HEADER"PID filter (%d) CONNECTED....\n", (UINT32) pv_tag));
    }
    else
    {
        DBG_ERROR((_ERROR_HEADER"PID filter (%d) DISCONNECTED....\n", (UINT32) pv_tag));
    }
}


static BOOL _config_pid_filter
(
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl,
    SIZE_T              z_buf_size)
{
    DEMUX_NFY_INFO_T    t_demux_nfy;
    RM_COND_T           e_cond;
    INT32               i4_ret;
    BOOL                b_succeeded = FALSE;
    
    i4_ret = rm_set(
        pt_pidf_ctrl->h_pidf,
        FALSE,
        ANY_PORT_NUM,
        DEMUX_SET_TYPE_CTRL,
        (const VOID *) FALSE,
        (SIZE_T) sizeof(BOOL),
        &e_cond);

    if (RMR_OK == i4_ret)
    {
        i4_ret = rm_set(
            pt_pidf_ctrl->h_pidf,
            FALSE,
            ANY_PORT_NUM,
            DEMUX_SET_TYPE_PID,
            (const VOID *) ((UINT32) pt_pidf_ctrl->t_pid),
            (SIZE_T) sizeof(MPEG_2_PID_T),
            &e_cond);

        if (i4_ret == RMR_OK)
        {
            pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_PIDF_PID_SET;
                
            t_demux_nfy.pv_tag = (VOID*) (pt_pidf_ctrl->h_this);
            t_demux_nfy.pf_demux_nfy = _demux_pid_filter_nfy;

            i4_ret = rm_set(
                pt_pidf_ctrl->h_pidf,
                FALSE,
                ANY_PORT_NUM,
                DEMUX_SET_TYPE_NFY_FCT,
                (const VOID *) &t_demux_nfy,
                (SIZE_T) sizeof(DEMUX_NFY_INFO_T),
                &e_cond);

            if (RMR_OK == i4_ret)
            {
                pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_PIDF_NFY_SET;
                
                i4_ret = rm_set(
                    pt_pidf_ctrl->h_pidf,
                    FALSE,
                    ANY_PORT_NUM,
                    DEMUX_SET_TYPE_ALLOC_BUFFER,
                    (const VOID*) z_buf_size,
                    (SIZE_T) sizeof(SIZE_T),
                    &e_cond);

                if (RMR_OK == i4_ret)
                {
                    pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_PIDF_BUFFER_ALLOATED;
                    
                    i4_ret = rm_set(
                        pt_pidf_ctrl->h_pidf,
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


static INT32 _create_pidf_filter(
        MPEG_2_PID_T            t_pid,
        SIZE_T                  z_buf_size,
        HANDLE_T*               ph_obj,
        DMX_UT_PIDF_CTRL_T**    ppt_pidf_ctrl)
{
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL;
    INT32               i4_ret;
    RM_COND_T           e_cond;
    BOOL                b_succeeded = FALSE;
    
    pt_pidf_ctrl = x_mem_alloc(sizeof(DMX_UT_PIDF_CTRL_T));
    if (!pt_pidf_ctrl)
    {
        return DMXR_UT_NO_RESOURCE;
    }
    else
    {
        x_memset(pt_pidf_ctrl, 0, sizeof(DMX_UT_PIDF_CTRL_T));
        
        pt_pidf_ctrl->h_this      = NULL_HANDLE;
        pt_pidf_ctrl->e_state     = DMX_UT_PIDF_CTRL_STATE_UNKNOWN;
        pt_pidf_ctrl->t_pid       = t_pid;
        pt_pidf_ctrl->h_pidf      = NULL_HANDLE;
        pt_pidf_ctrl->ui4_secf_count = 0;
        
    #ifdef DEBUG
        pt_pidf_ctrl->ui4_sec_from_dmx     = 0;
        pt_pidf_ctrl->ui4_sec_lost_in_isr  = 0;
        pt_pidf_ctrl->ui4_sec_lost_in_thrd = 0;
        pt_pidf_ctrl->ui4_sec_sent         = 0;
    #endif

        if (handle_alloc(
            DMX_UT_HT_PIDF,
            pt_pidf_ctrl,
            NULL,
            _filter_ctrl_free,
            ph_obj) == HR_OK)
        {
            pt_pidf_ctrl->h_this = *ph_obj;
        
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
                (VOID*)(UINT32) t_pid,
                (rm_nfy_fct) _pid_filter_open_nfy,
                &(pt_pidf_ctrl->h_pidf),
                &e_cond);

            if ((RMR_OK == i4_ret) &&
                (RM_COND_OPENED == rm_cond_chg(e_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE)))
            {
                pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_PIDF_OPENED;
                
                i4_ret = rm_connect(
                    pt_pidf_ctrl->h_pidf,
                    RM_CONN_TYPE_HANDLE,
                    (VOID *) _h_tuner,
                    sizeof(HANDLE_T),
                    NULL,
                    &e_cond);

                if (RMR_OK == i4_ret)
                {
                    pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_PIDF_CONNECTED;
                    
                    if (_config_pid_filter(pt_pidf_ctrl, z_buf_size))
                    {

                        pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_SECF_CONFIGURED;
                        b_succeeded = TRUE;
                    }
                }
            }
        }
    }

    if (!b_succeeded)
    {
        if (pt_pidf_ctrl)
        {
            if (NULL_HANDLE != pt_pidf_ctrl->h_this)
            {
                handle_free(pt_pidf_ctrl->h_this, TRUE);
            }
            else
            {
                x_mem_free(pt_pidf_ctrl);
            }
            
            pt_pidf_ctrl = NULL;
        }
        return DMXR_UT_INTERNAL_ERR;
    }
    else
    {
        *ppt_pidf_ctrl = pt_pidf_ctrl;
        return DMXR_UT_OK;
    }
}

static VOID _dmx_ut_thread_main(VOID* pv_arg)
{
    SIZE_T                      z_msg_size = sizeof(DMX_UT_MSG_T);
    UINT16                      ui2_index = 0;
    INT32                       i4_ret;
    DMX_UT_MSG_T                t_msg;
    DMX_UT_PIDF_CTRL_T*         pt_pidf_ctrl;
    HANDLE_TYPE_T               e_type;
    DMX_UT_COND_T               e_nfy_cond;
    BOOL                        b_nfy;;
    
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
            
            if ((HR_OK == handle_get_type_obj(t_msg.h_pidf_ctrl, &e_type, (VOID**) &pt_pidf_ctrl)) &&
                (DMX_UT_HT_PIDF == e_type) &&
                (NULL != pt_pidf_ctrl))
            {
                _db_unlock();
    
                b_nfy = TRUE;
                switch (t_msg.e_cond)
                {
                    case DEMUX_COND_DATA_REC:
                        e_nfy_cond = DMX_UT_COND_DATA_REC;
                        break;
                        
                    case DEMUX_COND_OVERFLOW:
                        e_nfy_cond = DMX_UT_COND_DATA_OVERFLOW;
#if 1
                        DBG_ERROR((_ERROR_HEADER"DEMUX overflow. Flush DEMUX buffer!\n"));                           
#endif
                        break;

                    default:
                        b_nfy = FALSE;
                        break;
                }
                if (b_nfy && pt_pidf_ctrl->pf_dmx_ut_nfy)
                {
                    pt_pidf_ctrl->pf_dmx_ut_nfy(e_nfy_cond, 
                                                t_msg.pv_src_tag, 
                                                t_msg.z_len,
                                                pt_pidf_ctrl->ui4_nfy_data);
                }
            }
            else
            {
                _db_unlock();
            }

        }
    }
}
 
/*-----------------------------------------------------------------------------
                    public functions implementation
 ----------------------------------------------------------------------------*/
INT32 x_dmx_ut_init(VOID* pv_data)
{
    RM_COND_T   e_cond;
#ifdef CLI_LVL_ALL
    INT32       i4_ret;
#endif
    
    if (_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }

#ifdef CLI_LVL_ALL
    i4_ret = dmx_ut_cli_init();
    if (i4_ret != DMXR_UT_OK)
    {
        return i4_ret;    
    }
#endif
    
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
        DMX_UT_MSGQ_NAME,
        sizeof(DMX_UT_MSG_T), 
        DMX_UT_MSGQ_LEN) != OSR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_MSGQ);
    }
   
    if (OSR_OK != x_thread_create(
        &_h_thread, 
        DMX_UT_THREAD_NAME, 
        DMX_UT_THREAD_STACK_SIZE,
        DMX_UT_THREAD_PRIORITY,
        _dmx_ut_thread_main,
        0,
        NULL))
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_THREAD);
    }
    SLIST_INIT(&_t_pidf_ctrl_lst);
    
    _b_dmx_ut_init = TRUE;
    
    return DMXR_UT_OK;    
}

/*
 * @In:     t_pid
 * @Out:    ph_pidf_ctrl
 */
INT32 x_dmx_ut_pid_filter_alloc(MPEG_2_PID_T    t_pid,
                                SIZE_T          z_buf_size,
                                HANDLE_T*       ph_pidf_ctrl)
{
    INT32               i4_ret;
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL;
    
    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }
    
    if (!ph_pidf_ctrl)
    {
        return DMXR_UT_INV_ARGS;
    }
    
    if (t_pid >= MPEG_2_NULL_PID)
    {
        DBG_ERROR((_ERROR_HEADER"%s: invalid pid value(PID=%d)\n", __FUNCTION__, t_pid));
        return DMXR_UT_INV_ARGS;
    }
    else
    {
        _db_lock();
        
        pt_pidf_ctrl = _find_pidf(t_pid, &_t_pidf_ctrl_lst);
        if (pt_pidf_ctrl)
        {
            _db_unlock();
            DBG_ERROR((_ERROR_HEADER"%s(PID=%d): filter already existed\n", __FUNCTION__, t_pid));
            return DMXR_UT_INV_OP;
        }
        else
        {
            /* Create a new section control object. */
            i4_ret = _create_pidf_filter(t_pid, z_buf_size, ph_pidf_ctrl, &pt_pidf_ctrl);
            if (i4_ret == DMXR_UT_OK)
            {
                _insert_pidf_ctrl(pt_pidf_ctrl, &_t_pidf_ctrl_lst);
            #ifdef DEBUG
                _i4_pidf_cnt++;

                if (_i4_pidf_cnt > _i4_max_pidf_cnt)
                {
                    _i4_max_pidf_cnt = _i4_pidf_cnt;
                }
            #endif
            }
            else
            {
                DBG_ERROR((_ERROR_HEADER"%s(PID=%d): create filter failed\n", __FUNCTION__, t_pid));
            }
        }

        _db_unlock();
    }
    return i4_ret;    
}

INT32 x_dmx_ut_pid_filter_set_nfy(
            HANDLE_T            h_pidf_ctrl,
            x_dmx_ut_nfy_fct    pf_dmx_ut_nfy,
            UINT32              ui4_nfy_data)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL; 

    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }
    
    if (!pf_dmx_ut_nfy)
    {
        return DMXR_UT_INV_ARGS;    
    }

    if (h_pidf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }
    
   _db_lock();

    /* find filter control object */
    i4_ret = handle_get_type_obj(h_pidf_ctrl, &e_type, (VOID**) &pt_pidf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_PIDF != e_type ||
        NULL == pt_pidf_ctrl)
    {
        _db_unlock();
        return DMXR_UT_INV_HANDLE;
    }

    pt_pidf_ctrl->pf_dmx_ut_nfy = pf_dmx_ut_nfy;
    pt_pidf_ctrl->ui4_nfy_data = ui4_nfy_data;
    
    _db_unlock();

    return DMXR_UT_OK;    
}


INT32 x_dmx_ut_pid_filter_free(HANDLE_T h_pidf_ctrl)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL; 

    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }

    if (h_pidf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }
    _db_lock();
    
    /* find filter control object */
    i4_ret = handle_get_type_obj(h_pidf_ctrl, &e_type, (VOID**) &pt_pidf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_PIDF != e_type ||
        NULL == pt_pidf_ctrl)
    {
        _db_unlock();
        return DMXR_UT_INV_HANDLE;
    }

    if (pt_pidf_ctrl->ui4_secf_count != 0)
    {
        DBG_INFO((_INFO_HEADER"section filter should be free before free pid filter\n"));
        _db_unlock();
        return DMXR_UT_INV_OP;
    }

    handle_free(h_pidf_ctrl, TRUE);

    _db_unlock();
    
    return DMXR_UT_OK;    
}

INT32 x_dmx_ut_sec_filter_alloc(
    HANDLE_T            h_pidf_ctrl,
    HANDLE_T*           ph_secf_ctrl)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    DMX_UT_SECF_CTRL_T* pt_secf_ctrl = NULL;
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL; 
    RM_COND_T           e_cond;
    BOOL                b_succeeded = FALSE;

    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }

    if (!ph_secf_ctrl)
    {
        return DMXR_UT_INV_ARGS;    
    }

    if (h_pidf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }
    
    _db_lock();

    /* find filter control object */
    i4_ret = handle_get_type_obj(h_pidf_ctrl, &e_type, (VOID**) &pt_pidf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_PIDF != e_type ||
        NULL == pt_pidf_ctrl)
    {
        _db_unlock();
        return DMXR_UT_INV_HANDLE;
    }

    pt_secf_ctrl = x_mem_alloc(sizeof(DMX_UT_SECF_CTRL_T));
    if (!pt_secf_ctrl)
    {
        _db_unlock();
        return DMXR_UT_NO_RESOURCE;
    }
    
    if (handle_alloc(
        DMX_UT_HT_SECF,
        pt_secf_ctrl,
        NULL,
        _secf_ctrl_obj_free,
        ph_secf_ctrl) == HR_OK)
    {
        pt_secf_ctrl->h_pidf_ctrl = h_pidf_ctrl;

        /* Open Section Filter */
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
            &(pt_secf_ctrl->h_secf),
            &e_cond);
        if ((RMR_OK == i4_ret) &&
            (RM_COND_OPENED == rm_cond_chg(e_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE)))
        {
            pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_SECF_OPENED;
    
            /* Connect Section Filter */
            i4_ret = rm_connect(
                pt_secf_ctrl->h_secf,
                RM_CONN_TYPE_HANDLE,
                (const VOID *) (pt_pidf_ctrl->h_pidf),
                sizeof(HANDLE_T),
                (VOID *) NULL,
                &e_cond);
            if (RMR_OK == i4_ret)
            {
                pt_pidf_ctrl->e_state = DMX_UT_PIDF_CTRL_STATE_SECF_CONNECTED;
                b_succeeded = TRUE;
            }
        }
        pt_pidf_ctrl->ui4_secf_count++;
        
        _db_unlock();
    
        if (!b_succeeded)
        {
            return DMXR_UT_RM_ERR;
        }
        else
        {
            return DMXR_UT_OK;
        }
    }
    else
    {
        return DMXR_UT_INTERNAL_ERR;
    }
    

}

INT32 x_dmx_ut_sec_filter_set(
    HANDLE_T            h_secf_ctrl,
    DMX_UT_SF_INFO_T*   pt_sf_info)
{
    INT32                           i4_ret;
    RM_COND_T                       e_cond;
    HANDLE_TYPE_T                   e_type;
    DMX_UT_SECF_CTRL_T*             pt_secf_ctrl = NULL;
    BOOL                            b_succeeded = FALSE;
    DEMUX_SECTION_FILTER_INFO_T     t_sec_info;
    
    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }
    if (!pt_sf_info)
    {
        return DMXR_UT_INV_ARGS;    
    }

    if (h_secf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }

    /* find filter control object */
    i4_ret = handle_get_type_obj(h_secf_ctrl, &e_type, (VOID**) &pt_secf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_SECF != e_type ||
        NULL == pt_secf_ctrl)
    {
        return DMXR_UT_INV_HANDLE;
    }
            
    /* Disable section filter before set parameter to it */
    i4_ret = rm_set(
        pt_secf_ctrl->h_secf,
        FALSE,
        ANY_PORT_NUM,
        DEMUX_SET_TYPE_CTRL,
        (const VOID*) ((UINT32) FALSE),
        (SIZE_T) sizeof(BOOL),
        &e_cond);
    if (RMR_OK == i4_ret)
    {
        t_sec_info.ui1_tbl_id_pattern = pt_sf_info->ui1_tbl_id_pattern;
        t_sec_info.ui1_tbl_id_mask    = pt_sf_info->ui1_tbl_id_mask;
        t_sec_info.ui1_pattern_len    = pt_sf_info->ui1_pattern_len;
        t_sec_info.ui1_pattern_offset = pt_sf_info->ui1_pattern_offset;
        t_sec_info.pui1_pos_pattern   = pt_sf_info->pui1_pos_pattern;
        t_sec_info.pui1_pos_mask      = pt_sf_info->pui1_pos_mask;
        t_sec_info.pui1_neg_pattern   = pt_sf_info->pui1_neg_pattern;
        t_sec_info.pui1_neg_mask      = pt_sf_info->pui1_neg_mask;
        t_sec_info.b_check_crc        = pt_sf_info->b_check_crc;
        
        /* Set section filter parameter */
        i4_ret = rm_set(
            pt_secf_ctrl->h_secf,
            FALSE,
            ANY_PORT_NUM,
            DEMUX_SET_TYPE_SECTION_FILTER,
            (const VOID *) &t_sec_info,
            (SIZE_T) sizeof(DEMUX_SECTION_FILTER_INFO_T),
            &e_cond);
        if (RMR_OK == i4_ret)
        {
            b_succeeded = TRUE;
        }
        else
        {
            /*Set section filter fail*/    
        }
    }
    else
    {
        /*Disable section filter fail*/    
    }

    if (!b_succeeded)
    {
        return DMXR_UT_RM_ERR;
    }
    else
    {
        return DMXR_UT_OK;
    }
}

INT32 x_dmx_ut_sec_filter_ctrl(
    HANDLE_T    h_secf_ctrl,
    BOOL        b_enable)
{
    INT32                   i4_ret;
    HANDLE_TYPE_T           e_type;
    DMX_UT_SECF_CTRL_T*     pt_secf_ctrl = NULL;
    RM_COND_T               e_cond;
   
    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }

    /* find filter control object */
    i4_ret = handle_get_type_obj(h_secf_ctrl, &e_type, (VOID**) &pt_secf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_SECF != e_type ||
        NULL == pt_secf_ctrl)
    {
        return DMXR_UT_INV_HANDLE;
    }

    /* Enable Section Filter */
    i4_ret = rm_set(
        pt_secf_ctrl->h_secf,
        FALSE,
        ANY_PORT_NUM,
        DEMUX_SET_TYPE_CTRL,
        (const VOID*) ((UINT32) b_enable),
        (SIZE_T) sizeof(BOOL),
        &e_cond);
    if (RMR_OK != i4_ret)
    {
        return DMXR_UT_RM_ERR;
    }

    return DMXR_UT_OK;    
}

INT32 x_dmx_ut_sec_filter_free(HANDLE_T h_secf_ctrl)
{
    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }
    if (h_secf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }
    
    handle_free(h_secf_ctrl, TRUE);
    return DMXR_UT_OK;
}

INT32 x_dmx_ut_sec_data_read(HANDLE_T   h_pidf_ctrl,
                             VOID*      pv_src_tag,
                             SIZE_T     z_skip_len,
                             SIZE_T     z_copy_len,
                             UINT8*     pui1_buf)
{
    INT32                       i4_ret;
    SIZE_T                      z_size;
    RM_COND_T                   e_cond;
    DEMUX_READ_BUFFER_INFO_T    t_read;
    HANDLE_TYPE_T               e_type;
    DMX_UT_PIDF_CTRL_T*         pt_pidf_ctrl = NULL;

    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }
    
    if (!pui1_buf)
    {
        return DMXR_UT_INV_ARGS;    
    }

    if (h_pidf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }

    _db_lock();
    
    /* find filter control object */
    i4_ret = handle_get_type_obj(h_pidf_ctrl, &e_type, (VOID**) &pt_pidf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_PIDF != e_type ||
        NULL == pt_pidf_ctrl)
    {
        _db_unlock();
        return DMXR_UT_INV_HANDLE;
    }

    t_read.pui1_dest = pui1_buf;
    t_read.pv_src_tag = pv_src_tag;
    t_read.z_skip_len = z_skip_len;
    t_read.z_copy_len = z_copy_len;

    z_size = sizeof(DEMUX_READ_BUFFER_INFO_T);
    
    i4_ret = rm_get(
        pt_pidf_ctrl->h_pidf,
        FALSE,
        ANY_PORT_NUM,
        DEMUX_GET_TYPE_READ_BUFFER,
        (VOID *) &t_read,
        (SIZE_T *) &z_size,
        &e_cond);
    
    if (RMR_OK == i4_ret)
    {
        _db_unlock();
        return DMXR_UT_OK;    
    }
    else
    {
#if 0
    #ifdef DEBUG
        pt_pidf_ctrl->ui4_sec_lost_in_thrd++;
    #endif
#endif
        DBG_ERROR((_ERROR_HEADER"Get section from DMX FAILED (%d)!\n",
            i4_ret));
    }
    _db_unlock();
    return DMXR_UT_OK;    
}

INT32 x_dmx_ut_sec_data_release(HANDLE_T   h_pidf_ctrl,
                                VOID*      pv_src_tag,
                                SIZE_T     z_sec_size)
{
    INT32                       i4_ret;
    DEMUX_UNLOCK_BUFFER_INFO_T  t_buf;
    RM_COND_T                   e_cond;
    HANDLE_TYPE_T               e_type;
    DMX_UT_PIDF_CTRL_T*         pt_pidf_ctrl = NULL;    
    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }
    
    if (h_pidf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }    
    _db_lock();
    
    /* find filter control object */
    i4_ret = handle_get_type_obj(h_pidf_ctrl, &e_type, (VOID**) &pt_pidf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_PIDF != e_type ||
        NULL == pt_pidf_ctrl)
    {
        _db_unlock();
        return DMXR_UT_INV_HANDLE;
    }
    
    x_memset(&t_buf, 0, sizeof(t_buf));
    t_buf.pv_src_tag    = pv_src_tag;
    t_buf.z_src_len     = z_sec_size;
    
    (VOID) rm_set(pt_pidf_ctrl->h_pidf,
                  FALSE,
                  ANY_PORT_NUM,
                  DEMUX_SET_TYPE_UNLOCK_BUFFER,
                  (const VOID *) &t_buf,
                  (SIZE_T) sizeof(DEMUX_UNLOCK_BUFFER_INFO_T),
                  &e_cond);

    _db_unlock();
    
    return DMXR_UT_OK;
}

INT32 x_dmx_ut_sec_buf_flush(HANDLE_T   h_pidf_ctrl)
{
    INT32               i4_ret;
    RM_COND_T           e_cond;
    HANDLE_TYPE_T       e_type;
    DMX_UT_PIDF_CTRL_T* pt_pidf_ctrl = NULL;

    if (!_b_dmx_ut_init)
    {
        return DMXR_UT_NOT_INIT;    
    }

    if (h_pidf_ctrl == NULL_HANDLE)  
    {
        return DMXR_UT_INV_HANDLE;    
    }
    
    _db_lock();

    /* find filter control object */
    i4_ret = handle_get_type_obj(h_pidf_ctrl, &e_type, (VOID**) &pt_pidf_ctrl);
    if (HR_OK != i4_ret ||
        DMX_UT_HT_PIDF != e_type ||
        NULL == pt_pidf_ctrl)
    {
        _db_unlock();
        return DMXR_UT_INV_HANDLE;
    }

    do 
    {
        i4_ret = rm_set(
            pt_pidf_ctrl->h_pidf,
            FALSE,
            ANY_PORT_NUM,
            DEMUX_SET_TYPE_CTRL,
            (const VOID*) ((UINT32) FALSE),
            (SIZE_T) sizeof(BOOL),
            &e_cond);
        if (i4_ret != RMR_OK)
        {
            break;    
        }
        
        i4_ret = rm_set(
            pt_pidf_ctrl->h_pidf,
            FALSE,
            ANY_PORT_NUM,
            DEMUX_SET_TYPE_FLUSH_BUFFER,
            NULL,
            (SIZE_T) 0,
            &e_cond);
        if (i4_ret != RMR_OK)
        {
            break;    
        }
        
        i4_ret = rm_set(
            pt_pidf_ctrl->h_pidf,
            FALSE,
            ANY_PORT_NUM,
            DEMUX_SET_TYPE_CTRL,
            (const VOID*) ((UINT32) TRUE),
            (SIZE_T) sizeof(BOOL),
            &e_cond);
        if (i4_ret != RMR_OK)
        {
            break;    
        }
        _db_unlock();
        return DMXR_UT_OK;
    } while(0);

    _db_unlock();
    return DMXR_UT_RM_ERR;
}

#ifdef CLI_LVL_ALL
static UINT16  ui2_g_dmx_ut_dbg_level  = DBG_LEVEL_NONE;

UINT16 dmx_ut_get_dbg_level(VOID)
{
    return ui2_g_dmx_ut_dbg_level;
}

VOID dmx_ut_set_dbg_level(
                    UINT16                      ui2_db_level
                    )
{
    ui2_g_dmx_ut_dbg_level = ui2_db_level;
}
#endif


