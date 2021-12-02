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
 * $RCSfile: handle.c,v $
 * $Revision: #2 $
 * $Date: 2012/06/01 $
 * $Author: muran.xiang $
 *
 * Description:
 *         This file contains all handle related API's. Note that this library
 *         will not use semaphores to protect common resources but uses
 *         critical section start / end.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "handle.h"
#include "x_handle_grp.h"
#include "x_common.h"
#include "x_assert.h"
#include "x_os.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Abort stuff. */
#ifdef ABORT
#undef ABORT
#endif

#ifdef CC_MTK_LOADER
#define ABORT(_cat, _val) ASSERT(0)
#else
#define ABORT(_cat, _val) DBG_ABORT ((DBG_MOD_HANDLE | _cat | _val))
#endif

#ifdef CC_MTK_LOADER

#ifdef x_crit_start
#undef x_crit_start
#endif

#ifdef x_crit_end
#undef x_crit_end
#endif

#define x_sprintf                   SPrintf
#define x_sema_create(a, b, c)      (OSR_OK)
#define x_sema_delete(a)
#define x_crit_start()              ((CRIT_STATE_T)0)
#define x_crit_end(a)
#define x_dbg_stmt(fmt...)
#define x_rwl_create_lock(a)        (0)
#define x_rwl_read_lock(a, b)       (0)
#define x_rwl_read_lock_grabbed(a)  (0)
//#define x_rwl_release_lock(a)       _GetZero()
#define x_rwl_write_lock(a, b)      (0)
#define x_rwl_write_lock_grabbed(a) (0)
//#define x_sema_lock(a, b)           _GetZero()
//#define x_sema_unlock(a)            _GetZero()
#if 0
static UINT32 _GetZero()
{
    return 0;
}
#endif
#endif /* CC_MTK_LOADER */

#define DBG_ABRT_INV_HANDLE_LINK_SIZE           ((UINT32) 0x00000000)
#define DBG_ABRT_INV_PARSE_OP                   ((UINT32) 0x00000001)
#define DBG_ABRT_INV_PARSE_RET_VALUE            ((UINT32) 0x00000002)
#define DBG_ABRT_INV_AUX_LINK_HEAD_HANDLE_TYPE  ((UINT32) 0x00000003)
#define DBG_ABRT_HANDLE_IS_STILL_LINKED         ((UINT32) 0x00000004)
#define DBG_ABRT_NULL_AUTOFREE_FUNCTION         ((UINT32) 0x00000005)
#define DBG_ABRT_AUTOFREE_NOT_REGISTERED        ((UINT32) 0x00000006)
#define DBG_ABRT_AUTOFREE_GET_TYPE_FAILED       ((UINT32) 0x00000007)
#define DBG_ABRT_AUTOFREE_FAILED                ((UINT32) 0x00000008)
#define DBG_ABRT_MISSING_FREE_CALLBACK          ((UINT32) 0x00000009)
#define DBG_ABRT_ALLOC_AUX_LINK_FAILED          ((UINT32) 0x0000000a)

#define NULL_IDX  ((UINT16) 0)

#define MAX_NUM_HANDLES  ((UINT16) 0xffff)

/* Some CLI definitions. */
#define CLI_HANDLE_MODULE       "handle"
#define CLI_HANDLE_MODULE_HELP  "HANDLE module commands"

#define CLI_GET_STATS_LONG   "stats"
#define CLI_GET_INFO_LONG    "info"
#define CLI_GET_STATS_SHORT  "s"
#define CLI_GET_INFO_SHORT   "i"

#define CLI_GET_STATS_HELP  "Returns handle statistics."
#define CLI_GET_INFO_HELP   "Returns the info of an individual handle"

#define CLI_NUM_ARGS_INFO        ((INT32) 2)
#define CLI_ARG_IDX_INFO_HANDLE  1

/* Handle control flag. */
#define H_CTRL_FLAG_ACTIVE   ((UINT8) 0x01)
#define H_CTRL_FLAG_PARSING  ((UINT8) 0x02)
#define H_CTRL_FLAG_LINKED   ((UINT8) 0x04)
#define H_CTRL_FLAG_FREEING  ((UINT8) 0x08)

typedef struct _LNK_T
{
    UINT16  ui2_next;
    UINT16  ui2_prev;
}   LNK_T;

/* Handle control structure */
typedef struct _H_CTRL_T
{
    union
    {
        VOID*  pv_tag;                     /* Handle tag. */
        handle_aux_free_fct  pf_aux_free;  /* Auxilary free function. */
    }   u;

    VOID*  pv_obj;                         /* Data object reference. */

    handle_free_fct  pf_free;              /* Data object free function. */

    LNK_T  t_h_link;                       /* Link fields used via H_LINK_T. */
    LNK_T  t_aux_link;                     /* Link fields used for the auxilary linking. */

    UINT16  ui2_aux_link_head;             /* References the auxilary link head. */
    UINT16  ui2_aux_link_count;            /* Number of handles attached to the auxilary link head. */

    HANDLE_TYPE_T  e_type;                 /* Handle type. */

    UINT8  ui1_count;                      /* Used to create unique handle values. */
    UINT8  ui1_flags;                      /* Some flags. */
}   H_CTRL_T;


/* Handle link flags. */
#define H_LINK_FLAG_PARSING   ((UINT8) 0x01)
#define H_LINK_FLAG_FREE_ALL  ((UINT8) 0x02)

/* Handle link structure */
typedef struct _H_LINK_T
{
    UINT16  ui2_head;         /* First handle referencing the same data object. */
    UINT16  ui2_tail;         /* Last handle referencing the same data object. */

    UINT16  ui2_h_count;      /* Number of handles referencing the same data object. */

    UINT8 ui1_flags;          /* Some flags. */
}   H_LINK_T;


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static UINT16    ui2_free_handles;                  /* Maintains a list of free handles. */
static H_CTRL_T* pt_h_ctrl_arr;                     /* Points to the beginning of the handle array. */

static UINT16 ui2_num_total;                        /* Total number of handles. */
static UINT16 ui2_num_free;                         /* Number of free handles. */
static UINT16 ui2_num_peak_used;                    /* Number of used handles as a peak. */

static UINT16 ui2_num_used_per_grp [HT_NB_GROUPS];  /* Tracks the number of allocated handles per handle group. */

static BOOL b_init = FALSE;                         /* Tracks if the handle library has been initialized. */

static handle_autofree_fct* apf_handle_clean_up_functions[HT_NB_GROUPS];


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

static HANDLE_T make_handle (H_CTRL_T*  pt_h_ctrl);
static H_CTRL_T* get_h_ctrl_from_handle (HANDLE_T  h_handle);
static H_CTRL_T* get_h_ctrl_from_idx (UINT16  ui2_h_idx);
static BOOL aux_link_head_free (HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,
                                BOOL           b_req_handle);
static VOID delink_from_aux (H_CTRL_T*  pt_h_ctrl);


/* The following is CLI related stuff. */
#if 0 // [5391 loader]
#ifdef CLI_SUPPORT
static INT32 cli_get_stats (INT32         i4_argc,
                            const CHAR**  pps_argv);
static INT32 cli_get_info (INT32         i4_argc,
                           const CHAR**  pps_argv);


static CLI_EXEC_T at_cli_handle_cmd [] =
{
    {CLI_GET_STATS_LONG, CLI_GET_STATS_SHORT, cli_get_stats, NULL, CLI_GET_STATS_HELP, CLI_SUPERVISOR},
    {CLI_GET_INFO_LONG,  CLI_GET_INFO_SHORT,  cli_get_info,  NULL, CLI_GET_INFO_HELP,  CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_handle [] =
{
    {CLI_HANDLE_MODULE, NULL, NULL, &(at_cli_handle_cmd [0]), CLI_HANDLE_MODULE_HELP, CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif
#endif

/*-----------------------------------------------------------------------------
 * Name:  make_handle
 *
 * Description: This API creates a handle given a pointer to a handle control
 *              structure. This routine will not validate any arguments.
 *
 * Inputs:  pt_handle_arr  Points to the handle control structure.
 *
 * Outputs: -
 *
 * Returns: The handle associated with the handle control structure.
 ----------------------------------------------------------------------------*/
static HANDLE_T make_handle (H_CTRL_T*  pt_h_ctrl)
{
    return ((HANDLE_T) ((((UINT32) (pt_h_ctrl - pt_h_ctrl_arr)) & 0x0000ffff) + (((UINT32) pt_h_ctrl->ui1_count) << 16)));
}


/*-----------------------------------------------------------------------------
 * Name:  get_h_ctrl_from_handle
 *
 * Description: This API returns a pointer to a handle control structure given
 *              a handle. In case the handle is invalid a NULL pointer is
 *              returned.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: -
 *
 * Returns: A pointer to the handle control structure.
 ----------------------------------------------------------------------------*/
static H_CTRL_T* get_h_ctrl_from_handle (HANDLE_T  h_handle)
{
    UINT16    ui2_h_idx;
    H_CTRL_T* pt_h_ctrl;


    pt_h_ctrl = NULL;

    /* Validate handle. */
    if (((((UINT32) h_handle) & 0xff000000) == 0)                                                      &&
        ((ui2_h_idx = ((UINT16) (((UINT32) h_handle) & 0x0000ffff))) > NULL_IDX)                       &&
        (ui2_h_idx <= ui2_num_total)                                                                   &&
        (pt_h_ctrl_arr [ui2_h_idx].ui1_count == ((UINT8) ((((UINT32) h_handle) >> 16) & 0x000000ff)))  &&
        ((pt_h_ctrl_arr [ui2_h_idx].ui1_flags & H_CTRL_FLAG_ACTIVE) != 0))
    {
        pt_h_ctrl = &(pt_h_ctrl_arr [ui2_h_idx]);
    }

    return (pt_h_ctrl);
}


/*-----------------------------------------------------------------------------
 * Name:  get_h_ctrl_from_idx
 *
 * Description: This API returns a pointer to a handle control structure given
 *              a handle index. In case the handle index is invalid a NULL
 *              pointer is returned.
 *
 * Inputs:  ui2_h_idx  Contains the handle index.
 *
 * Outputs: -
 *
 * Returns: A pointer to the handle control structure.
 ----------------------------------------------------------------------------*/
static H_CTRL_T* get_h_ctrl_from_idx (UINT16  ui2_h_idx)
{
    H_CTRL_T* pt_h_ctrl;


    pt_h_ctrl = NULL;

    if (ui2_h_idx != NULL_IDX)
    {
        pt_h_ctrl = &(pt_h_ctrl_arr [ui2_h_idx]);
    }

    return (pt_h_ctrl);
}


/*-----------------------------------------------------------------------------
 * Name: aux_link_head_free
 *
 * Description: This API is called whenever an auxilary link head handle is
 *              being freed. The freeing is only allowed if no more handles are
 *              linked to the auxilary link head.
 *
 * Inputs:  h_obj         Contains the handle to the element.
 *          e_type        Contains the element handle type and must be set to
 *                        HT_AUX_LINK_HEAD.
 *          pv_obj        Ignored.
 *          pv_tag        Ignored.
 *          b_req_handle  Ignored.
 *
 * Outputs: -
 *
 * Returns: TRUE if no more handles are attached else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL aux_link_head_free (HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,
                                BOOL           b_req_handle)
{
    BOOL         b_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    b_return = FALSE;

    /* This access must be made atomic. */
//    t_state = x_crit_start ();

    /* Only do the following if it is a auxilary link head handle type. */
    if ((e_type == HT_AUX_LINK_HEAD)                               &&
        ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL))
    {
        /* Only allow the freeing if no more handle is attached. */
        b_return = (pt_h_ctrl->ui2_aux_link_count == 0);
    }
    else
    {
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_AUX_LINK_HEAD_HANDLE_TYPE);
    }

    /* Don't forget to close the critical section part. */
//    x_crit_end (t_state);

    return (b_return);
}


/*-----------------------------------------------------------------------------
 * Name: delink_from_aux
 *
 * Description: This API delinks a handle from the auxilary linked list. It is
 *              assumed that the it has alreday been verified that the handle
 *              is linked to the auxilary linked list. Also, this API must only
 *              be called from within a critical section.
 *
 * Inputs:  pt_h_ctrl  References the control structure of the handle which
 *                     shall be delinked.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID delink_from_aux (H_CTRL_T*  pt_h_ctrl)
{
    H_CTRL_T* pt_aux_head;

    /* Delink the handle control object. */
    pt_aux_head = &(pt_h_ctrl_arr [pt_h_ctrl->ui2_aux_link_head]);

    /* Let the owner of the aux head know that a handle has been delinked. */
    if (pt_aux_head->u.pf_aux_free != NULL)
    {
        pt_aux_head->u.pf_aux_free (pt_aux_head->pv_obj);
    }

    if (pt_h_ctrl->t_aux_link.ui2_next != NULL_IDX)
    {
        pt_h_ctrl_arr [pt_h_ctrl->t_aux_link.ui2_next].t_aux_link.ui2_prev = pt_h_ctrl->t_aux_link.ui2_prev;
    }

    if (pt_h_ctrl->t_aux_link.ui2_prev != NULL_IDX)
    {
        pt_h_ctrl_arr [pt_h_ctrl->t_aux_link.ui2_prev].t_aux_link.ui2_next = pt_h_ctrl->t_aux_link.ui2_next;
    }
    else
    {
        pt_aux_head->t_aux_link.ui2_next = pt_h_ctrl->t_aux_link.ui2_next;
    }

    /* Reset the auxilary link fields. */
    pt_h_ctrl->t_aux_link.ui2_next = NULL_IDX;
    pt_h_ctrl->t_aux_link.ui2_prev = NULL_IDX;
    pt_h_ctrl->ui2_aux_link_head   = NULL_IDX;

    /* And deceremnt the link counter. */
    if (pt_aux_head->ui2_aux_link_count > 0)
    {
        pt_aux_head->ui2_aux_link_count--;
    }
}

#if 0 // [5391 loader]
#ifdef CLI_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: cli_get_stats
 *
 * Description: This function is called by the CLI and handles the presentation
 *              of the handle library statistics.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 ----------------------------------------------------------------------------*/
static INT32 cli_get_stats (INT32         i4_argc,
                            const CHAR**  pps_argv)
{
    UINT16       ui2_total;
    UINT16       ui2_free;
    UINT16       ui2_peak_used;
    UINT16       ui2_used_per_grp [HT_NB_GROUPS];
    UINT16       ui2_idx;
    CRIT_STATE_T t_state;

    /* Get the statistics info using an atomic operation. */
    t_state = x_crit_start ();

    ui2_total     = ui2_num_total;
    ui2_free      = ui2_num_free;
    ui2_peak_used = ui2_num_peak_used;

    for (ui2_idx = 0; ui2_idx < HT_NB_GROUPS; ui2_idx++)
    {
        ui2_used_per_grp [ui2_idx] = ui2_num_used_per_grp [ui2_idx];
    }

    x_crit_end (t_state);

    /* And present the handle group usage. */
    for (ui2_idx = 0; ui2_idx < HT_NB_GROUPS; ui2_idx++)
    {
        x_dbg_stmt ("Group:   %3d  Used: %5d\n",
                    ui2_idx,
                    ui2_used_per_grp [ui2_idx]);
    }

    if (ui2_total >= ui2_free)
    {
        x_dbg_stmt ("Total: %5d  Used: %5d  Free: %5d  Peak used: %5d\n",
                    ui2_total,
                    (ui2_total - ui2_free),
                    ui2_free,
                    ui2_peak_used);
    }
    else
    {
        x_dbg_stmt ("Total: %5d  Used: -----  Free: %5d  Peak used: %5d\n",
                    ui2_total,
                    ui2_free,
                    ui2_peak_used);
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_get_info
 *
 * Description: This function is called by the CLI and handles the presentation
 *              of the fields of an individual handle.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK       Successful
 *          CLIR_INV_ARG  One or more invalid arguments
 ----------------------------------------------------------------------------*/
static INT32 cli_get_info (INT32         i4_argc,
                           const CHAR**  pps_argv)
{
    INT32        i4_return;
    HANDLE_T     h_handle;
    H_CTRL_T     t_h_ctrl;
    H_CTRL_T*    pt_h_ctrl;
    const CHAR*  ps_handle;
    CRIT_STATE_T t_state;


    i4_return = CLIR_INV_ARG;

    if ((i4_argc >= CLI_NUM_ARGS_INFO)                              &&
        (pps_argv != NULL)                                          &&
        ((ps_handle = pps_argv [CLI_ARG_IDX_INFO_HANDLE]) != NULL))
    {
        i4_return = CLIR_OK;

        /* Get the handle control structure. */
        h_handle = (HANDLE_T) x_strtoull (ps_handle, NULL, 0);

        t_state = x_crit_start ();

        if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
        {
            /* Must make a copy so that we can stop the */
            /* critical section again.                  */
            t_h_ctrl = (*pt_h_ctrl);

            x_crit_end (t_state);

            /* And now, present the data. */
            x_dbg_stmt ("Handle: %s\n", ps_handle);

            x_dbg_stmt ("  Type: %5d  (Group: %3d  Idx: %3d)  Flags: 0x%02x\n",
                        t_h_ctrl.e_type,
                        (t_h_ctrl.e_type / HT_GROUP_SIZE),
                        (t_h_ctrl.e_type % HT_GROUP_SIZE),
                        t_h_ctrl.ui1_flags);
            x_dbg_stmt ("  Obj: 0x%08x  Free funct: 0x%08x\n",
                        ((UINT32) t_h_ctrl.pv_obj),
                        ((UINT32) t_h_ctrl.pf_free));
            x_dbg_stmt ("  Link      Next [h]: %7d  Prev [h]: %7d\n",
                        make_handle (&(pt_h_ctrl_arr [t_h_ctrl.t_h_link.ui2_next])),
                        make_handle (&(pt_h_ctrl_arr [t_h_ctrl.t_h_link.ui2_prev])));
            x_dbg_stmt ("  AUX Link  Next [h]: %7d  Prev [h]: %7d\n",
                        make_handle (&(pt_h_ctrl_arr [t_h_ctrl.t_aux_link.ui2_next])),
                        make_handle (&(pt_h_ctrl_arr [t_h_ctrl.t_aux_link.ui2_prev])));

            if (t_h_ctrl.e_type != HT_AUX_LINK_HEAD)
            {
                x_dbg_stmt ("  Tag: 0x%08x  AUX Link Head [h]: %7d\n",
                            ((UINT32) t_h_ctrl.u.pv_tag),
                            make_handle (&(pt_h_ctrl_arr [t_h_ctrl.ui2_aux_link_head])));
            }
            else
            {
                x_dbg_stmt ("  AUX free funct: 0x%08x  Num AUX linked: %5d\n",
                            ((UINT32) t_h_ctrl.u.pf_aux_free),
                            t_h_ctrl.ui2_aux_link_count);
            }
        }
        else
        {
            /* Invalid handle. */
            x_crit_end (t_state);

            x_dbg_stmt ("Handle %s is invalid or free\n", ps_handle);
        }
    }

    return (i4_return);
}
#endif
#endif

/*-----------------------------------------------------------------------------
 * Name:  handle_alloc
 *
 * Description: This API allocates a new handle to the caller and initialise
 *              it.
 *
 * Inputs:  e_type   Contains the handle type.
 *          pv_obj   Contains a reference to a data object.
 *          pv_tag   Contains a handle tag.
 *          pf_free  References the handle free function.
 *
 * Outputs: ph_handle  Contains the newly allocated handle.
 *
 * Returns: HR_OK               Routine successful.
 *          HR_INV_ARG          One or more arguments are invalid.
 *          HR_OUT_OF_HANDLES   No free handles available.
 *          HR_NOT_INIT         The handle library is not initialized yet.
 *          HR_INV_HANDLE_TYPE  Invalid handle type.
 ----------------------------------------------------------------------------*/
INT32 handle_alloc (HANDLE_TYPE_T    e_type,
                    VOID*            pv_obj,
                    VOID*            pv_tag,
                    handle_free_fct  pf_free,
                    HANDLE_T*        ph_handle)
{
    INT32        i4_return;
    UINT16       ui2_idx;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate arguments. */
        if ((ph_handle != NULL)  &&
            (pf_free   != NULL))
        {
            i4_return = HR_INV_HANDLE_TYPE;

            /* Bug-20  2004-12-21  ffr. Don't allow invalid handle type values. */
            if (e_type != INV_HANDLE_TYPE)
            {
                i4_return = HR_OUT_OF_HANDLES;

                /* Get next free handle control structure. */
//                t_state = x_crit_start ();

                if ((pt_h_ctrl = get_h_ctrl_from_idx (ui2_free_handles)) != NULL)
                {
                    ui2_free_handles = pt_h_ctrl->t_h_link.ui2_next;

                    /* Update the statistics. */
                    ui2_num_free--;

                    if ((ui2_idx = e_type / HT_GROUP_SIZE) < HT_NB_GROUPS)
                    {
                        ui2_num_used_per_grp [ui2_idx]++;
                    }

                    if ((ui2_num_total >= ui2_num_free)                       &&
                        ((ui2_num_total - ui2_num_free) > ui2_num_peak_used))
                    {
                        ui2_num_peak_used = ui2_num_total - ui2_num_free;
                    }

//                    x_crit_end (t_state);

                    i4_return = HR_OK;

                    /* And initialize handle control structure. */
                    pt_h_ctrl->u.pv_tag            = pv_tag;
                    pt_h_ctrl->pv_obj              = pv_obj;
                    pt_h_ctrl->pf_free             = pf_free;
                    pt_h_ctrl->t_h_link.ui2_next   = NULL_IDX;
                    pt_h_ctrl->t_h_link.ui2_prev   = NULL_IDX;
                    pt_h_ctrl->t_aux_link.ui2_next = NULL_IDX;
                    pt_h_ctrl->t_aux_link.ui2_prev = NULL_IDX;
                    pt_h_ctrl->ui2_aux_link_head   = NULL_IDX;
                    pt_h_ctrl->ui2_aux_link_count  = 0;
                    pt_h_ctrl->e_type              = e_type;
                    pt_h_ctrl->ui1_flags           = H_CTRL_FLAG_ACTIVE;

                    pt_h_ctrl->ui1_count++;

                    (*ph_handle) = make_handle (pt_h_ctrl);
                }
                else
                {
                    /* Don't forget to close the critical section part. */
//                    x_crit_end (t_state);
                }
            }
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  handle_alloc_and_obj
 *
 * Description: This API allocates a new handle to the caller and initialise
 *              it. In addition, it may also allocate an object, initialize the
 *              object to '0' and return a reference to the object.
 *
 * Inputs:  e_type   Contains the handle type.
 *          pv_tag   Contains a handle tag.
 *          z_size   Contains the size of the object which shall be allocated.
 *          pf_free  References the handle free function.
 *
 * Outputs: ph_handle  Contains the newly allocated handle.
 *          ppv_obj    Contains the reference to the allocated object.
 *
 * Returns: HR_OK               Routine successful.
 *          HR_INV_ARG          One or more arguments are invalid.
 *          HR_OUT_OF_HANDLES   No free handles available.
 *          HR_NOT_ENOUGH_MEM   Not enough memory.
 *          HR_NOT_INIT         The handle library is not initialized yet.
 *          HR_INV_HANDLE_TYPE  Invalid handle type.
 ----------------------------------------------------------------------------*/
 #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_alloc_and_obj (HANDLE_TYPE_T    e_type,
                            VOID*            pv_tag,
                            SIZE_T           z_size,
                            handle_free_fct  pf_free,
                            HANDLE_T*        ph_handle,
                            VOID**           ppv_obj)
{
    VOID* pv_obj;
    INT32 i4_return;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate arguments. */
        if ((ph_handle != NULL)  &&
            (pf_free   != NULL))
        {
            i4_return = HR_NOT_ENOUGH_MEM;

            /* Initialize arguments. */
            pv_obj = NULL;
            if (ppv_obj != NULL)
            {
                (*ppv_obj) = NULL;
            }

            if ((ppv_obj == NULL)                              ||
                (z_size == 0)                                  ||
                ((pv_obj = x_mem_calloc (1, z_size)) != NULL))
            {
                if ((i4_return = handle_alloc (e_type, pv_obj, pv_tag, pf_free, ph_handle)) == HR_OK)
                {
                    if (pv_obj != NULL)
                    {
                        /* And return the reference of the allocated object. At   */
                        /* this point we know that 'ppv_obj' cannot be NULL since */
                        /* 'pv_obj' is only set when 'ppv_obj' is not NULL.       */
                        (*ppv_obj) = pv_obj;
                    }
                }
                else
                {
                    /* Oops, could not allocate a handle. Free */
                    /* the allocated object.                   */
                    if (pv_obj != NULL)
                    {
                        x_mem_free (pv_obj);
                    }
                }
            }
        }
    }

    return (i4_return);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_delink
 *
 * Description: This API delinks a handle from the handle link structure. No
 *              check is performed to verify that the handle belongs to the
 *              handle link structure.
 *
 * Inputs:  pt_h_link  References the handle link structure.
 *          h_handle   Contains the handle which shall be delinked.
 *
 * Outputs: -
 *
 * Returns: HR_OK           Routine successful.
 *          HR_INV_ARG      One or more arguments are invalid.
 *          HR_INV_HANDLE   The provided handle is invalid.
 *          HR_NOT_INIT     The handle library is not initialized yet.
 *          HR_NOT_ALLOWED  This API is called from a parsing function with
 *                          an illegal handle.
 *          HR_NOT_LINKED   The handle is not linked.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_delink (HANDLE_LINK_T*  pt_h_link,
                     HANDLE_T        h_handle)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (pt_h_link != NULL)
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            /* Bug-14  2004-12-08  ffr. Do not allow the linking / delinking of special */
            /* handle types, like the ones belonging to the handle library.             */
            if (((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL) &&
                (pt_h_ctrl->e_type >= HT_RES_BY_HANDLE_LIB))
            {
                i4_return = HR_NOT_LINKED;

                if ((pt_h_ctrl->ui1_flags & H_CTRL_FLAG_LINKED) != 0)
                {
                    i4_return = HR_NOT_ALLOWED;

                    /* Prevent invalid handle delink. */
                    if (((((H_LINK_T*) pt_h_link)->ui1_flags & H_LINK_FLAG_PARSING) == 0) ||
                        ((pt_h_ctrl->ui1_flags & H_CTRL_FLAG_PARSING) != 0))
                    {
                        i4_return = HR_OK;

                        /* Sinal that the handle has been delinked. */
                        pt_h_ctrl->ui1_flags &= ~(H_CTRL_FLAG_LINKED);

//                        x_crit_end (t_state);

                        /* Delink handle. */
                        if (pt_h_ctrl->t_h_link.ui2_next != NULL_IDX)
                        {
                            pt_h_ctrl_arr [pt_h_ctrl->t_h_link.ui2_next].t_h_link.ui2_prev = pt_h_ctrl->t_h_link.ui2_prev;
                        }
                        else
                        {
                            ((H_LINK_T*) pt_h_link)->ui2_tail = pt_h_ctrl->t_h_link.ui2_prev;
                        }

                        if (pt_h_ctrl->t_h_link.ui2_prev != NULL_IDX)
                        {
                            pt_h_ctrl_arr [pt_h_ctrl->t_h_link.ui2_prev].t_h_link.ui2_next = pt_h_ctrl->t_h_link.ui2_next;
                        }
                        else
                        {
                            ((H_LINK_T*) pt_h_link)->ui2_head = pt_h_ctrl->t_h_link.ui2_next;
                        }

                        pt_h_ctrl->t_h_link.ui2_next = NULL_IDX;
                        pt_h_ctrl->t_h_link.ui2_prev = NULL_IDX;

                        /* And decrement the handle count value. */
                        if (((H_LINK_T*) pt_h_link)->ui2_h_count > 0)
                        {
                            ((H_LINK_T*) pt_h_link)->ui2_h_count--;
                        }
                    }
                    else
                    {
                        /* Don't forget to end critical section. */
//                        x_crit_end (t_state);
                    }
                }
                else
                {
                    /* Don't forget to end critical section. */
//                    x_crit_end (t_state);
                }
            }
            else
            {
                /* Don't forget to end critical section. */
//                x_crit_end (t_state);
            }
        }
    }

    return (i4_return);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_free
 *
 * Description: This API calls the handles free function which will delink the
 *              handle from the data object and afterwards the handle is
 *              returned to the free list.
 *
 * Inputs:  h_handle      Contains the handle which shall be freed.
 *          b_req_handle  Indicates if the operation is performed on this
 *                        handle or if the free operation is performed due to
 *                        some other operation.
 *
 * Outputs: -
 *
 * Returns: HR_OK                Routine successful.
 *          HR_INV_HANDLE        The provided handle is invalid.
 *          HR_NOT_INIT          The handle library is not initialized yet.
 *          HR_FREE_NOT_ALLOWED  The handle is not allowed to be freed.
 ----------------------------------------------------------------------------*/
INT32 handle_free (HANDLE_T  h_handle,
                   BOOL      b_req_handle)
{
    VOID*        pv_obj;
    VOID*        pv_tag;
    INT32        i4_return;
    UINT16       ui2_idx;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_HANDLE;

        /* Make access to handle reference atomic. */
//        t_state = x_crit_start ();

        /* Validate handle. */
        if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
        {
            i4_return = HR_FREE_NOT_ALLOWED;

            /* Before calling the free callback function */
            /* establish the object pointer.             */
            pv_obj = NULL;

            /* Bug-16  2004-12-14  ffr. Don't all recursive freeing of the same handle. */
            if ((pt_h_ctrl->ui1_flags & H_CTRL_FLAG_FREEING) == 0)
            {
                pv_tag = pt_h_ctrl->u.pv_tag;
                pv_obj = pt_h_ctrl->pv_obj;

                pt_h_ctrl->ui1_flags |= H_CTRL_FLAG_FREEING;

//                x_crit_end (t_state);

                /* Abort if the free callback function is missing. Serious error. */
                if (pt_h_ctrl->pf_free == NULL)
                {
                    ABORT (DBG_CAT_INV_OP, DBG_ABRT_MISSING_FREE_CALLBACK);
                }

                /* Handles free callback. It is the callbacks responsibility  */
                /* to have the handle delinked from the data object. The free */
                /* callback function will also signal if the free operation   */
                /* is allowed or not.                                         */
                if (pt_h_ctrl->pf_free (h_handle,
                                        pt_h_ctrl->e_type,
                                        pv_obj,
                                        pv_tag,
                                        b_req_handle))
                {
                    i4_return = HR_OK;

                    /* Here we assume that the handle has been delinked. */
                    /* Hence, make certain that the handle is being      */
                    /* invalidated.                                      */
//                    t_state = x_crit_start ();

                    /* Bug-25  01-05-2005  ffr. Abort if the handle is still linked. */
                    if ((pt_h_ctrl->ui1_flags & H_CTRL_FLAG_LINKED) != 0)
                    {
                        ABORT (DBG_CAT_HANDLE, DBG_ABRT_HANDLE_IS_STILL_LINKED);
                    }

                    /* Delink the handle from the auxilary linked list. */
                    if (pt_h_ctrl->ui2_aux_link_head != NULL_IDX)
                    {
                        delink_from_aux (pt_h_ctrl);
                    }

                    pt_h_ctrl->ui1_count++;
                    pt_h_ctrl->ui1_flags &= ~(H_CTRL_FLAG_ACTIVE);

                    /* And return the handle to the free list. */
                    pt_h_ctrl->t_h_link.ui2_next = NULL_IDX;
                    pt_h_ctrl->t_h_link.ui2_prev = NULL_IDX;

                    pt_h_ctrl->t_h_link.ui2_next = ui2_free_handles;
                    ui2_free_handles             = (UINT16) (pt_h_ctrl - pt_h_ctrl_arr);

                    /* Update the statistics. */
                    ui2_num_free++;

                    if (((ui2_idx = pt_h_ctrl->e_type / HT_GROUP_SIZE) < HT_NB_GROUPS) &&
                        (ui2_num_used_per_grp [ui2_idx] > 0))
                    {
                        ui2_num_used_per_grp [ui2_idx]--;
                    }

//                    x_crit_end (t_state);
                }
                else
                {
                    /* Make access to handle reference atomic. */
//                    t_state = x_crit_start ();

                    pt_h_ctrl->ui1_flags &= ~(H_CTRL_FLAG_FREEING);

//                    x_crit_end (t_state);
                }
            }
            else
            {
                /* Don't forget to end critical section. */
//                x_crit_end (t_state);
            }
        }
        else
        {
            /* Don't forget to end critical section. */
//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  handle_free_all
 *
 * Description: This API free's all handles attached to a data object.
 *
 * Inputs:  pt_h_link  References the handle link structure.
 *
 * Outputs: -
 *
 * Returns: HR_OK                Routine successful.
 *          HR_INV_ARG           One or more arguments are invalid.
 *          HR_NOT_INIT          The handle library is not initialized yet.
 *          HR_RECURSION_EROR    This API is called in a recursive manner.
 *          HR_FREE_NOT_ALLOWED  The handle is not allowed to be freed.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_free_all (HANDLE_LINK_T*  pt_h_link)
{
    BOOL         b_success;
    VOID*        pv_obj;
    VOID*        pv_tag;
    INT32        i4_return;
    UINT16       ui2_idx;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (pt_h_link != NULL)
        {
            i4_return = HR_NOT_ALLOWED;

            /* Prevent calls from within (*handle_parse_fct). */
            if (((((H_LINK_T*) pt_h_link)->ui1_flags) & H_LINK_FLAG_PARSING) == 0)
            {
                i4_return = HR_RECURSION_ERROR;

                /* Prevent recursion. */
                if (((((H_LINK_T*) pt_h_link)->ui1_flags) & H_LINK_FLAG_FREE_ALL) == 0)
                {
                    i4_return = HR_FREE_NOT_ALLOWED;

                    /* Prevent recursion. */
                    ((H_LINK_T*) pt_h_link)->ui1_flags |= H_LINK_FLAG_FREE_ALL;

                    /* And free all handles attached to the handle link */
                    /* structure. It is the callbacks responsibility to */
                    /* delink the handle from the data object.          */
                    b_success = TRUE;

                    /* Make access to handle reference atomic. */
//                    t_state = x_crit_start ();

                    while ((b_success)                                                                      &&
                           ((pt_h_ctrl = get_h_ctrl_from_idx (((H_LINK_T*) pt_h_link)->ui2_head)) != NULL))
                    {
                        /* Bug-16  2004-12-14  ffr. Don't all recursive freeing of the same handle. */
                        if ((pt_h_ctrl->ui1_flags & H_CTRL_FLAG_FREEING) == 0)
                        {
                            pv_tag = pt_h_ctrl->u.pv_tag;
                            pv_obj = pt_h_ctrl->pv_obj;

                            pt_h_ctrl->ui1_flags |= H_CTRL_FLAG_FREEING;

//                            x_crit_end (t_state);

                            /* Abort if the free callback function is missing. Serious error. */
                            if (pt_h_ctrl->pf_free == NULL)
                            {
                                ABORT (DBG_CAT_INV_OP, DBG_ABRT_MISSING_FREE_CALLBACK);
                            }

                            /* Handles free callback. It is the callbacks responsibility  */
                            /* to have the handle delinked from the data object. The free */
                            /* callback function will also signal if the free operation   */
                            /* is allowed or not.                                         */
                            b_success = pt_h_ctrl->pf_free (make_handle (pt_h_ctrl),
                                                            pt_h_ctrl->e_type,
                                                            pv_obj,
                                                            pv_tag,
                                                            FALSE);

                            if (b_success)
                            {
                                /* Here we assume that the handle has been delinked. */
                                /* Hence, make certain that the handle is being      */
                                /* invalidated.                                      */
//                                t_state = x_crit_start ();

                                /* Bug-25  01-05-2005  ffr. Abort if the handle is still linked. */
                                if ((pt_h_ctrl->ui1_flags & H_CTRL_FLAG_LINKED) != 0)
                                {
                                    ABORT (DBG_CAT_HANDLE, DBG_ABRT_HANDLE_IS_STILL_LINKED);
                                }

                                /* Delink the handle from the auxilary linked list. */
                                if (pt_h_ctrl->ui2_aux_link_head != NULL_IDX)
                                {
                                    delink_from_aux (pt_h_ctrl);
                                }

                                pt_h_ctrl->ui1_count++;
                                pt_h_ctrl->ui1_flags &= ~(H_CTRL_FLAG_ACTIVE);

                                /* And return the handle to the free list. */
                                pt_h_ctrl->t_h_link.ui2_next = NULL_IDX;
                                pt_h_ctrl->t_h_link.ui2_prev = NULL_IDX;

                                pt_h_ctrl->t_h_link.ui2_next = ui2_free_handles;
                                ui2_free_handles             = (UINT16) (pt_h_ctrl - pt_h_ctrl_arr);

                                /* Update the statistics. */
                                ui2_num_free++;

                                if (((ui2_idx = pt_h_ctrl->e_type / HT_GROUP_SIZE) < HT_NB_GROUPS) &&
                                    (ui2_num_used_per_grp [ui2_idx] > 0))
                                {
                                    ui2_num_used_per_grp [ui2_idx]--;
                                }
                            }
                            else
                            {
//                                t_state = x_crit_start ();

                                pt_h_ctrl->ui1_flags &= ~(H_CTRL_FLAG_FREEING);
                            }
                        }
                        else
                        {
                            /* Bug-16  2004-12-14  ffr. Don't all recursive freeing of the same handle. */
                            b_success = FALSE;
                        }
                    }

                    /* And don't forget to end critical section. */
//                    x_crit_end (t_state);

                    if (b_success)
                    {
                        /* All handles could be freed. */
                        i4_return = HR_OK;
                    }

                    /* Allow new call to this API. */
                    ((H_LINK_T*) pt_h_link)->ui1_flags &= ~(H_LINK_FLAG_FREE_ALL);
                }
            }
        }
    }

    return (i4_return);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_get_obj
 *
 * Description: This API returns a handles data object reference.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: ppv_obj   Contains the data object reference.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_get_obj (HANDLE_T  h_handle,
                      VOID**    ppv_obj)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (ppv_obj != NULL)
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
            {
                i4_return = HR_OK;

                /* Return data object reference. */
                (*ppv_obj) = pt_h_ctrl->pv_obj;
            }

//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  handle_get_type_obj
 *
 * Description: This API returns the handle type and object reference.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: pe_type  Contains the handle type.
 *          ppv_obj  Contains the data object reference.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 handle_get_type_obj (HANDLE_T        h_handle,
                           HANDLE_TYPE_T*  pe_type,
                           VOID**          ppv_obj)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if ((pe_type != NULL)  &&
            (ppv_obj != NULL))
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
            {
                i4_return = HR_OK;

                /* Return data object type and reference. */
                (*pe_type) = pt_h_ctrl->e_type;
                (*ppv_obj) = pt_h_ctrl->pv_obj;
            }

//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  handle_init
 *
 * Description: This API initialises the handle library. This routine will
 *              abort if the size of HANDLE_LINK_T is less than the size of
 *              H_LINK_T. Note that this routine will not use the x_mem_alloc
 *              function to allocate memory for the handle control structures
 *              because the memory handler is not yet initialized. That is the
 *              reason this API contains arguments for the memory address and
 *              size. Basically, it will grab the required memory size from
 *              the given chunk of memory and return the reminder.
 *
 * Inputs:  ui2_num_handles  Contains the number of handles the handle library
 *                           shall allocate.
 *          ppv_mem_addr     Contains the start address of the memory chunk.
 *          pz_mem_size      Contains the size (in Bytes) of the chunk of
 *                           memory.
 *
 * Outputs: ppv_mem_addr  Contains the updated start address of the chunk of
 *                        memory.
 *          pz_mem_size   Contains the updated size (in Bytes) of the chunk of
 *                        memory.
 *
 * Returns: HR_OK              Routine successful.
 *          HR_INV_ARG         One or more arguments are invalid.
 *          HR_NOT_ENOUGH_MEM  Not enough memory available.
 *          HR_ALREDAY_INIT    Handle library already initialised.
 ----------------------------------------------------------------------------*/
INT32 handle_init (UINT16   ui2_num_handles,
                   VOID**   ppv_mem_addr,
                   SIZE_T*  pz_mem_size)
{
    INT32     i4_return;
    VOID*     pv_new_mem_addr;
    UINT16    ui2_idx;
    SIZE_T    z_len;
    SIZE_T    z_new_mem_size;
    H_CTRL_T* pt_h_ctrl;


    /* Abort if sizeof (HANDLE_LINK_T) is less than sizeof (H_LINK_T). */
    /*
    if (sizeof (HANDLE_LINK_T) < sizeof (H_LINK_T))
    {
        ABORT (DBG_CAT_INV_OP, DBG_ABRT_INV_HANDLE_LINK_SIZE);
    }
    */

    i4_return = HR_ALREADY_INIT;

    if (! (b_init))
    {
        i4_return = HR_INV_ARG;

        /* Validate argument.*/
        if ((ui2_num_handles > 0)               &&
            (ui2_num_handles < MAX_NUM_HANDLES) &&
            (ppv_mem_addr    != NULL)           &&
            ((*ppv_mem_addr) != NULL)           &&
            (pz_mem_size     != NULL)           &&
            ((*pz_mem_size)  != 0))
        {
            i4_return = HR_NOT_ENOUGH_MEM;

            /* Adjust memory start address to a multiple of 4 Bytes. */
            z_len = ((UINT32) (*ppv_mem_addr)) % sizeof (UINT32);
            z_len = (sizeof(UINT32) - z_len) % sizeof(UINT32);

            pv_new_mem_addr = (VOID*) (((UINT32) (*ppv_mem_addr)) + z_len);
            z_new_mem_size  = (*pz_mem_size) - z_len;

            /* If enough memory is available grab it from the beginning and */
            /* updated the start address and size of the memory chunk.      */
            z_len = sizeof (H_CTRL_T) * (ui2_num_handles + 1);
            if (z_len < z_new_mem_size)
            {
                i4_return = HR_OK;

                /* Grab memory. */
                pt_h_ctrl_arr = (H_CTRL_T*) pv_new_mem_addr;

                (*ppv_mem_addr) = (VOID*) (((UINT32) pv_new_mem_addr) + z_len);
                (*pz_mem_size)  = z_new_mem_size - z_len;

                /* And initialise the handle control structures. Remember that */
                /* the very first element is always a sacrificial element.     */
                ui2_num_total     = ui2_num_handles;
                ui2_num_free      = 0;
                ui2_num_peak_used = 0;

                for (ui2_idx = 0; ui2_idx < HT_NB_GROUPS; ui2_idx++)
                {
                    ui2_num_used_per_grp [ui2_idx] = 0;
                }

                ui2_free_handles = NULL_IDX;

                for (ui2_idx = 0; ui2_idx < ui2_num_handles; ui2_idx++)
                {
                    pt_h_ctrl = &(pt_h_ctrl_arr [ui2_idx+1]);

                    pt_h_ctrl->u.pv_tag  = NULL;
                    pt_h_ctrl->pv_obj    = NULL;
                    pt_h_ctrl->pf_free   = NULL;
                    pt_h_ctrl->e_type    = INV_HANDLE_TYPE;
                    pt_h_ctrl->ui1_count = 0;
                    pt_h_ctrl->ui1_flags = 0;

                    pt_h_ctrl->t_h_link.ui2_next = ui2_free_handles;
                    pt_h_ctrl->t_h_link.ui2_prev = NULL_IDX;

                    /* Bug-14  2004-12-08  ffr. Initialize the new fields. */
                    pt_h_ctrl->t_aux_link.ui2_next = NULL_IDX;
                    pt_h_ctrl->t_aux_link.ui2_prev = NULL_IDX;
                    pt_h_ctrl->ui2_aux_link_head   = NULL_IDX;
                    pt_h_ctrl->ui2_aux_link_count  = 0;

                    ui2_free_handles = ui2_idx + 1;

                    ui2_num_free++;
                }

                /* And indicate that the library has been initialized. */
                b_init = TRUE;
            }
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  handle_link
 *
 * Description: This API links a handle to a handle link structure. The handle
 *              will always be linked to the tail of a handle link structure.
 *
 * Inputs:  pt_h_link  References the handle link structure.
 *          h_handle   Contains the handle which shall be linked.
 *
 * Outputs: -
 *
 * Returns: HR_OK              Routine successful.
 *          HR_INV_ARG         One or more arguments are invalid.
 *          HR_INV_HANDLE      The provided handle is invalid.
 *          HR_NOT_INIT        The handle library is not initialized yet.
 *          HR_ALREADY_LINKED  The handle is alreday linked.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_link (HANDLE_LINK_T*  pt_h_link,
                   HANDLE_T        h_handle)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (pt_h_link != NULL)
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            /* Bug-14  2004-12-08  ffr. Do not allow the linking / delinking of special */
            /* handle types, like the ones belonging to the handle library.             */
            if (((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL) &&
                (pt_h_ctrl->e_type >= HT_RES_BY_HANDLE_LIB))
            {
                i4_return = HR_ALREADY_LINKED;

                if ((pt_h_ctrl->ui1_flags & H_CTRL_FLAG_LINKED) == 0)
                {
                    i4_return = HR_OK;

                    /* Sinal that the handle has been linked. */
                    pt_h_ctrl->ui1_flags |= H_CTRL_FLAG_LINKED;

//                    x_crit_end (t_state);

                    /* Link handle. */
                    pt_h_ctrl->t_h_link.ui2_next = NULL_IDX;

                    if ((pt_h_ctrl->t_h_link.ui2_prev = ((H_LINK_T*) pt_h_link)->ui2_tail) != NULL_IDX)
                    {
                        pt_h_ctrl_arr [((H_LINK_T*) pt_h_link)->ui2_tail].t_h_link.ui2_next = (UINT16) (pt_h_ctrl - pt_h_ctrl_arr);
                    }
                    else
                    {
                        ((H_LINK_T*) pt_h_link)->ui2_head = (UINT16) (pt_h_ctrl - pt_h_ctrl_arr);
                    }

                    ((H_LINK_T*) pt_h_link)->ui2_tail = (UINT16) (pt_h_ctrl - pt_h_ctrl_arr);

                    /* And increment the handle count value. */
                    ((H_LINK_T*) pt_h_link)->ui2_h_count++;
                }
                else
                {
                    /* Don't forget to end the critical section. */
//                    x_crit_end (t_state);
                }
            }
            else
            {
                /* Don't forget to end the critical section. */
//                x_crit_end (t_state);
            }
        }
    }

    return (i4_return);
}
#endif


/*-----------------------------------------------------------------------------
 * Name:  handle_link_init
 *
 * Description: This API initializes a data objects handle link structure.
 *
 * Inputs:  pt_h_link  References the handle link structure.
 *
 * Outputs: -
 *
 * Returns: HR_OK       Routine successful.
 *          HR_INV_ARG  One or more arguments are invalid.
----------------------------------------------------------------------------*/
#ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_link_init (HANDLE_LINK_T*  pt_h_link)
{
    INT32 i4_return;


    i4_return = HR_INV_ARG;

    /* Validate argument. */
    if (pt_h_link != NULL)
    {
        i4_return = HR_OK;

        /* Initialize handle link structure. */
        ((H_LINK_T*) pt_h_link)->ui2_head    = NULL_IDX;
        ((H_LINK_T*) pt_h_link)->ui2_tail    = NULL_IDX;
        ((H_LINK_T*) pt_h_link)->ui2_h_count = 0;
        ((H_LINK_T*) pt_h_link)->ui1_flags   = 0;
    }

    return (i4_return);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_link_is_empty
 *
 * Description: This API checks if a handle is attached to the handle link
 *              structure. If argument pt_link contains a NULL pointer this
 *              routine will return TRUE.
 *
 * Inputs:  pt_h_link  References the handle link structure.
 *
 * Outputs: -
 *
 * Returns: TRUE   No handle is attached.
 *          FALSE  One or more handles are attached.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
BOOL handle_link_is_empty (HANDLE_LINK_T*  pt_h_link)
{
    return ((pt_h_link == NULL) || (((H_LINK_T*) pt_h_link)->ui2_h_count == 0));
}
#endif


/*-----------------------------------------------------------------------------
 * Name:  handle_num_links
 *
 * Description: This API returns the number of handles attached to a handle
 *              link structure. If argument pt_link contains a NULL pointer
 *              this routine will return '0'.
 *
 * Inputs:  pt_h_link  References the handle link structure.
 *
 * Outputs: -
 *
 * Returns: The number of handles attached to the link structure.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
UINT16 handle_num_links (HANDLE_LINK_T*  pt_h_link)
{
    UINT16 ui2_count;


    ui2_count = 0;

    if (pt_h_link != NULL)
    {
        ui2_count = ((H_LINK_T*) pt_h_link)->ui2_h_count;
    }

    return (ui2_count);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_parse
 *
 * Description: This API calls for all handles attached to the handle link
 *              structure the parse function. It is assumed that for a given
 *              data object
 *
 * Inputs:  pt_h_link      References the handle link structure.
 *          pf_parse       References the parse function.
 *          pv_parse_data  References some data which is interpreted by the
 *                         parse function.
 *
 * Outputs: -
 *
 * Returns: HR_OK              Routine successful.
 *          HR_INV_ARG         One or more arguments are invalid.
 *          HR_NOT_INIT        The handle library is not initialized yet.
 *          HR_RECURSION_EROR  This API is called in a recursive manner.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_parse (HANDLE_LINK_T*    pt_h_link,
                    handle_parse_fct  pf_parse,
                    VOID*             pv_parse_data)
{
    INT32     i4_return;
    UINT16    ui2_curr_idx;
    UINT16    ui2_h_count;
    H_CTRL_T* pt_curr_h_ctrl;
    H_CTRL_T* pt_next_h_ctrl;
    HP_TYPE_T e_parse_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Bug-18  2004-12-21  ffr. Also make certain that the */
        /* parse function reference is not NULL.               */
        if ((pt_h_link != NULL)  &&
            (pf_parse  != NULL))
        {
            i4_return = HR_RECURSION_ERROR;

            if ((((H_LINK_T*) pt_h_link)->ui1_flags & H_LINK_FLAG_PARSING) == 0)
            {
                i4_return = HR_OK;

                /* Prevent recursion. */
                ((H_LINK_T*) pt_h_link)->ui1_flags |= H_LINK_FLAG_PARSING;

                pt_curr_h_ctrl = get_h_ctrl_from_idx (((H_LINK_T*) pt_h_link)->ui2_head);
                ui2_curr_idx   = 0;

                /* And cycle through all handles. */
                while (pt_curr_h_ctrl != NULL)
                {
                    /* Always keep track of the next linked handle */
                    /* because the current one may be freed in the */
                    /* parse function.                             */
                    pt_next_h_ctrl = get_h_ctrl_from_idx (pt_curr_h_ctrl->t_h_link.ui2_next);
                    ui2_h_count    = ((H_LINK_T*) pt_h_link)->ui2_h_count;

                    /* Signal that this handle is being parsed. */
                    pt_curr_h_ctrl->ui1_flags |= H_CTRL_FLAG_PARSING;

                    e_parse_state = pf_parse (ui2_curr_idx,
                                              ((H_LINK_T*) pt_h_link)->ui2_h_count,
                                              make_handle (pt_curr_h_ctrl),
                                              pt_curr_h_ctrl->e_type,
                                              pt_curr_h_ctrl->pv_obj,
                                              pt_curr_h_ctrl->u.pv_tag,
                                              pv_parse_data);

                    /* Reset signal that this handle is being parsed. */
                    pt_curr_h_ctrl->ui1_flags &= ~(H_CTRL_FLAG_PARSING);

                    switch (e_parse_state)
                    {
                        case HP_NEXT:
                            /* If the handle was freed in the parsing function */
                            /* make certain that it was the current handle and */
                            /* not some other one attached to the data object. */
                            if (((H_LINK_T*) pt_h_link)->ui2_h_count >= ui2_h_count)
                            {
                                pt_curr_h_ctrl = pt_next_h_ctrl;
                                ui2_curr_idx++;
                            }
                            else
                            {
                                /* Only one handle may be removed and it must have */
                                /* been the current parsed one. In all other cases */
                                /* simply abort because something went terrible    */
                                /* wrong during parsing.                           */
                                if ((ui2_h_count - ((H_LINK_T*) pt_h_link)->ui2_h_count) == 1)
                                {
                                    /* Don't increment ui2_curr_idx because a   */
                                    /* handle was removed from the linked list. */
                                    pt_curr_h_ctrl = pt_next_h_ctrl;
                                }
                                else
                                {
                                    ABORT (DBG_CAT_INV_OP, DBG_ABRT_INV_PARSE_OP);
                                }
                            }
                            break;

                        case HP_BREAK:
                            pt_curr_h_ctrl = NULL;
                            break;

                        case HP_RESTART:
                            pt_curr_h_ctrl = get_h_ctrl_from_idx (((H_LINK_T*) pt_h_link)->ui2_head);
                            ui2_curr_idx   = 0;
                            break;

                        default:
                            ABORT (DBG_CAT_INV_OP, DBG_ABRT_INV_PARSE_RET_VALUE);
                            break;
                    }
                }

                /* Allow new call to this API. */
                ((H_LINK_T*) pt_h_link)->ui1_flags &= ~(H_LINK_FLAG_PARSING);
            }
        }
    }

    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  handle_reg_cli
 *
 * Description: This API registers the Handle library with the CLI module. The
 *              registration can only be done once the CLI module has been
 *              initialized, which does not happen before "handle_init". Hence
 *              this additional CLI registration function.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: HR_OK                   Routine successful.
 *          HR_NOT_INIT             The handle library is not initialized yet.
 *          HR_CANNOT_REG_WITH_CLI  registration with CLI failed.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_reg_cli (VOID)
{
    INT32 i4_return;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
         i4_return = HR_OK;
    }

    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  handle_set_obj
 *
 * Description: This API allows the caller to set a handles data object
 *              reference.
 *
 * Inputs:  h_handle  Contains the handle for which the data object reference
 *                    shall be returned.
 *          pv_obj    Contains the data object reference.
 *
 * Outputs: -
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_set_obj (HANDLE_T  h_handle,
                      VOID*     pv_obj)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (pv_obj != NULL)
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
            {
                i4_return = HR_OK;

                /* Set the handle data object reference. */
                pt_h_ctrl->pv_obj = pv_obj;
            }

//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_alloc_aux_link_head
 *
 * Description: This API allocates a new auxilary link head object and
 *              initialise it.
 *
 * Inputs:  pv_obj       Contains a reference to a data object.
 *          pf_aux_free  References a callback function which is called
 *                       whenever a handle is delinked from the aux head.
 *
 * Outputs: ph_aux_head  Contains a handle referencing the newly allocated
 *                       auxilary head object.
 *
 * Returns: HR_OK              Routine successful.
 *          HR_INV_ARG         One or more arguments are invalid.
 *          HR_OUT_OF_HANDLES  No free handles available.
 *          HR_NOT_INIT        The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_alloc_aux_link_head (VOID*                pv_obj,
                                  handle_aux_free_fct  pf_aux_free,
                                  HANDLE_T*            ph_aux_head)
{
    INT32        i4_return;
    H_CTRL_T*    pt_aux_ctrl;
//    CRIT_STATE_T t_state;


    if ((i4_return = handle_alloc (HT_AUX_LINK_HEAD, pv_obj, NULL, aux_link_head_free, ph_aux_head)) == HR_OK)
    {
        /* Make access to handle reference atomic. */
//        t_state = x_crit_start ();

        if ((pt_aux_ctrl = get_h_ctrl_from_handle (*ph_aux_head)) == NULL)
        {
            /* Something is seriously wrong here. */
            ABORT (DBG_CAT_INV_OP, DBG_ABRT_ALLOC_AUX_LINK_FAILED);
        }

        /* And now set the auxilary free function. */
        pt_aux_ctrl->u.pf_aux_free = pf_aux_free;

//        x_crit_end (t_state);
    }

    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  handle_link_to_aux
 *
 * Description: This API links a handle to the auxilary link head. The handle
 *              will always be linked to the head of the linked list.
 *
 * Inputs:  h_aux_head  References the auxilary link head handle.
 *          h_handle    Contains the handle which shall be linked.
 *
 * Outputs: -
 *
 * Returns: HR_OK              Routine successful.
 *          HR_INV_HANDLE      The provided handle is invalid.
 *          HR_NOT_INIT        The handle library is not initialized yet.
 *          HR_ALREADY_LINKED  The handle is alreday linked.
 *          HR_INV_AUX_HEAD    The provided auxilary link head is invalid.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_link_to_aux (HANDLE_T  h_aux_head,
                          HANDLE_T  h_handle)
{
    INT32        i4_return;
    H_CTRL_T*    pt_aux_ctrl;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_AUX_HEAD;

        /* Make access to handle reference atomic. */
//        t_state = x_crit_start ();

        if (((pt_aux_ctrl = get_h_ctrl_from_handle (h_aux_head)) != NULL) &&
            (pt_aux_ctrl->e_type == HT_AUX_LINK_HEAD))
        {
            i4_return = HR_INV_HANDLE;

            if (((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL) &&
                (pt_h_ctrl->e_type >= HT_RES_BY_HANDLE_LIB))
            {
                i4_return = HR_ALREADY_LINKED;

                if (pt_h_ctrl->ui2_aux_link_head == NULL_IDX)
                {
                    i4_return = HR_OK;

                    /* And link the handle to the head of the linked list. */
                    if ((pt_h_ctrl->t_aux_link.ui2_next = pt_aux_ctrl->t_aux_link.ui2_next) != NULL_IDX)
                    {
                        pt_h_ctrl_arr [pt_aux_ctrl->t_aux_link.ui2_next].t_aux_link.ui2_prev = (UINT16) (pt_h_ctrl - pt_h_ctrl_arr);
                    }

                    pt_h_ctrl->t_aux_link.ui2_prev = NULL_IDX;

                    pt_aux_ctrl->t_aux_link.ui2_next = (UINT16) (pt_h_ctrl - pt_h_ctrl_arr);

                    /* Reference the auxilary link head object. */
                    pt_h_ctrl->ui2_aux_link_head = (UINT16) (pt_aux_ctrl - pt_h_ctrl_arr);

                    /* And increment the auxilary link counter. */
                    pt_aux_ctrl->ui2_aux_link_count++;
                }
            }
        }

//        x_crit_end (t_state);
    }

    return (i4_return);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_delink_from_aux
 *
 * Description: This API delinks a handle from the auxilary link list.
 *
 * Inputs:  h_handle  Contains the handle which shall be delinked.
 *
 * Outputs: -
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 *          HR_NOT_LINKED  The handle is not linked.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_delink_from_aux (HANDLE_T  h_handle)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_HANDLE;

        /* Make access to handle reference atomic. */
//        t_state = x_crit_start ();

        if (((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL) &&
            (pt_h_ctrl->e_type >= HT_RES_BY_HANDLE_LIB))
        {
            i4_return = HR_NOT_LINKED;

            if (pt_h_ctrl->ui2_aux_link_head != NULL_IDX)
            {
                i4_return = HR_OK;

                delink_from_aux (pt_h_ctrl);
            }
        }

        /* Don't forget to end critical section. */
//        x_crit_end (t_state);
    }

    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  handle_next_aux_linked
 *
 * Description: This API returns the next handle in the auxilary linked list.
 *
 * Inputs:  h_curr_handle  Contains the current handle.
 *
 * Outputs: ph_next_handle  Contains the next handle in the list or
 *                          NULL_HANDLE when no more is available.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_next_aux_linked (HANDLE_T   h_curr_handle,
                              HANDLE_T*  ph_next_handle)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (ph_next_handle != NULL)
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            if ((pt_h_ctrl = get_h_ctrl_from_handle (h_curr_handle)) != NULL)
            {
                i4_return = HR_OK;

                /* Return the handle to the next object in sequence. */
                (*ph_next_handle) = NULL_HANDLE;

                if (pt_h_ctrl->t_aux_link.ui2_next != NULL_IDX)
                {
                    (*ph_next_handle) = make_handle (&(pt_h_ctrl_arr [pt_h_ctrl->t_aux_link.ui2_next]));
                }
            }

            /* Don't forget to end critical section. */
//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_num_aux_link
 *
 * Description: This API returns the number of handles linked via the auxilary
 *              link and attached to the specified auxilary link head handle.
 *              In case the specified handle is invalid this API will return
 *              '0'.
 *
 * Inputs:  h_aux_head  References the auxilary link head handle.
 *
 * Outputs: -
 *
 * Returns: The number of handles attached to the auxilary link structure.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
UINT16 handle_num_aux_links (HANDLE_T  h_aux_head)
{
    UINT16       ui2_count;
    H_CTRL_T*    pt_aux_ctrl;
//    CRIT_STATE_T t_state;


    ui2_count = 0;

    if (b_init)
    {
        /* Make access to handle reference atomic. */
//        t_state = x_crit_start ();

        if (((pt_aux_ctrl = get_h_ctrl_from_handle (h_aux_head)) != NULL) &&
            (pt_aux_ctrl->e_type == HT_AUX_LINK_HEAD))
        {
            ui2_count = pt_aux_ctrl->ui2_aux_link_count;
        }

        /* Don't forget to end critical section. */
//        x_crit_end (t_state);
    }

    return (ui2_count);
}

#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_set_autofree_tbl
 *
 * Description: This API sets the "autofree" function table for a specified
 *              handle group.
 *
 * Inputs:  ui4_group         Contains the handle group.
 *          pf_autofree_fcts  References the array of functions.
 *
 * Outputs: -
 *
 * Returns: HR_OK       Routine successful.
 *          HR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 handle_set_autofree_tbl (HANDLE_TYPE_T         e_group,
                               handle_autofree_fct*  pf_autofree_fcts)
{
    UINT32  ui4_group_nb;
    INT32   i4_res;

    ui4_group_nb = (((UINT32) e_group) / HT_GROUP_SIZE);

    if ((ui4_group_nb < HT_NB_GROUPS) && (pf_autofree_fcts != NULL))
    {
//        CRIT_STATE_T  t_state;

//        t_state = x_crit_start();

        apf_handle_clean_up_functions[ui4_group_nb] = pf_autofree_fcts;

//        x_crit_end(t_state);

        i4_res = HR_OK;
    }
    else
    {
        i4_res = HR_INV_ARG;
    }

    return i4_res;
}
#endif
/*-----------------------------------------------------------------------------
 * Name:  handle_autofree
 *
 * Description: This API call the handle group's autofree function. It is
 *              called when an application terminates and all the handles are
 *              to be deleted. This API will abort in case of failure.
 *
 * Inputs:  h_handle  Contains the handle which shall be deleted.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
VOID handle_autofree (HANDLE_T  h_handle)
{
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;

    i4_res = x_handle_get_type(h_handle, & e_type);

    if (i4_res == HR_OK)
    {
        handle_autofree_fct*  pf_group_autfree_fct;

        pf_group_autfree_fct =
            apf_handle_clean_up_functions[e_type / HT_GROUP_SIZE];

        if (pf_group_autfree_fct != NULL)
        {
            handle_autofree_fct  pf_autofree;

            pf_autofree = pf_group_autfree_fct[e_type & (HT_GROUP_SIZE - 1)];

            if (pf_autofree != NULL)
            {
                if (pf_autofree(h_handle, e_type) != HR_OK)
                {
                    ABORT(DBG_CAT_HANDLE, DBG_ABRT_AUTOFREE_FAILED);
                }
            }
            else
            {
                ABORT(DBG_CAT_HANDLE, DBG_ABRT_NULL_AUTOFREE_FUNCTION);
            }
        }
        else
        {
            ABORT(DBG_CAT_HANDLE, DBG_ABRT_AUTOFREE_NOT_REGISTERED);
        }
    }
    else if (i4_res != HR_INV_HANDLE)
    {
        ABORT(DBG_CAT_HANDLE, DBG_ABRT_AUTOFREE_GET_TYPE_FAILED);
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  x_handle_free
 *
 * Description: This API calls the handle_free API with argument b_req_handle
 *              set to TRUE.
 *
 * Inputs:  h_handle  Contains the handle which shall be freed.
 *
 * Outputs: -
 *
 * Returns: HR_OK                Routine successful.
 *          HR_INV_HANDLE        The provided handle is invalid.
 *          HR_NOT_INIT          The handle library is not initialized yet.
 *          HR_FREE_NOT_ALLOWED  The handle is not allowed to be freed.
 ----------------------------------------------------------------------------*/
INT32 x_handle_free (HANDLE_T  h_handle)
{
    return (handle_free (h_handle, TRUE));
}


/*-----------------------------------------------------------------------------
 * Name:  x_handle_get_info
 *
 * Description: This API returns a handles type and tag value.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: pe_type  Contains the handle type.
 *          ppv_tag  Contains the tag value.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 x_handle_get_info (HANDLE_T        h_handle,
                         HANDLE_TYPE_T*  pe_type,
                         VOID**          ppv_tag)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate arguments. */
        if ((pe_type != NULL)  &&
            (ppv_tag != NULL))
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
            {
                i4_return = HR_OK;

                /* Return handle info. */
                (*pe_type) = pt_h_ctrl->e_type;
                (*ppv_tag) = pt_h_ctrl->u.pv_tag;
            }

//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  x_handle_get_tag
 *
 * Description: This API returns a handles tag value.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: ppv_tag  Contains the tag value.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 x_handle_get_tag (HANDLE_T  h_handle,
                        VOID**    ppv_tag)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (ppv_tag != NULL)
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
            {
                i4_return = HR_OK;

                /* Return handle tag value. */
                (*ppv_tag) = pt_h_ctrl->u.pv_tag;
            }

//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  x_handle_get_type
 *
 * Description: This API returns the handle type.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: pe_type  Contains the handle type.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 x_handle_get_type (HANDLE_T        h_handle,
                         HANDLE_TYPE_T*  pe_type)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_ARG;

        /* Validate argument. */
        if (pe_type != NULL)
        {
            i4_return = HR_INV_HANDLE;

            /* Make access to handle reference atomic. */
//            t_state = x_crit_start ();

            if ((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL)
            {
                i4_return = HR_OK;

                /* Return handle tag value. */
                (*pe_type) = pt_h_ctrl->e_type;
            }

//            x_crit_end (t_state);
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  x_handle_set_tag
 *
 * Description: This API re-sets a handles tag value.
 *
 * Inputs:  h_handle  Contains the handle.
 *          pv_tag    Contains the tag value.
 *
 * Outputs: -
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 x_handle_set_tag (HANDLE_T  h_handle,
                        VOID*     pv_tag)
{
    INT32        i4_return;
    H_CTRL_T*    pt_h_ctrl;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_INV_HANDLE;

        /* Make access to handle reference atomic. */
//        t_state = x_crit_start ();

        if (((pt_h_ctrl = get_h_ctrl_from_handle (h_handle)) != NULL) &&
            (pt_h_ctrl->e_type >= HT_RES_BY_HANDLE_LIB))
        {
            i4_return = HR_OK;

            /* Return handle tag value. */
            pt_h_ctrl->u.pv_tag = pv_tag;
        }

//        x_crit_end (t_state);
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  x_handle_status
 *
 * Description: This API returns some handle statistics which can be used for
 *              debugging purpose.
 *
 * Inputs:  -
 *
 * Outputs: pui2_num_handles  Contains the total number of handles.
 *          pui2_num_alloc    Contains the number of handles in use.
 *          pui2_num_free     Contains the number of free handles.
 *
 * Returns: HR_OK        Routine successful.
 *          HR_NOT_INIT  The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 x_handle_status (UINT16*  pui2_num_handles,
                       UINT16*  pui2_num_free)
{
    INT32        i4_return;
    UINT16       ui2_total;
    UINT16       ui2_free;
//    CRIT_STATE_T t_state;


    i4_return = HR_NOT_INIT;

    if (b_init)
    {
        i4_return = HR_OK;

        /* get snap-shot of current state values. */
//        t_state = x_crit_start ();

        ui2_total = ui2_num_total;
        ui2_free  = ui2_num_free;

//        x_crit_end (t_state);

        if (pui2_num_handles != NULL)
        {
            (*pui2_num_handles) = ui2_total;
        }

        if (pui2_num_free != NULL)
        {
            (*pui2_num_free) = ui2_free;
        }
    }

    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  x_handle_valid
 *
 * Description: This API returns TRUE if the given handle is valid else FALSE.
 *
 * Inputs:  h_handle  Contains the handle which shall be validated.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle is valid else FALSE.
 ----------------------------------------------------------------------------*/
BOOL x_handle_valid (HANDLE_T  h_handle)
{
    BOOL         b_valid;
//    CRIT_STATE_T t_state;


    /* Make access to handle reference atomic. */
//    t_state = x_crit_start ();

    b_valid = ((b_init) && (get_h_ctrl_from_handle (h_handle) != NULL));

//    x_crit_end (t_state);

    return (b_valid);
}


/*-----------------------------------------------------------------------------
 * Name:  c_handle_free
 *
 * Description: This API calls the x_handle_free API.
 *
 * Inputs:  h_handle  Contains the handle which shall be freed.
 *
 * Outputs: -
 *
 * Returns: HR_OK                Routine successful.
 *          HR_INV_HANDLE        The provided handle is invalid.
 *          HR_NOT_INIT          The handle library is not initialized yet.
 *          HR_FREE_NOT_ALLOWED  The handle is not allowed to be freed.
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_HANDLE
INT32 c_handle_free (HANDLE_T  h_handle)
{
    return (x_handle_free (h_handle));
}


/*-----------------------------------------------------------------------------
 * Name:  c_handle_get_info
 *
 * Description: This API returns a handles type and tag value.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: pe_type  Contains the handle type.
 *          ppv_tag  Contains the tag value.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 c_handle_get_info (HANDLE_T        h_handle,
                         HANDLE_TYPE_T*  pe_type,
                         VOID**          ppv_tag)
{
    return (x_handle_get_info (h_handle, pe_type, ppv_tag));
}


/*-----------------------------------------------------------------------------
 * Name:  c_handle_get_tag
 *
 * Description: This API returns a handles tag value.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: ppv_tag  Contains the tag value.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 c_handle_get_tag (HANDLE_T  h_handle,
                        VOID**    ppv_tag)
{
    return (x_handle_get_tag (h_handle, ppv_tag));
}


/*-----------------------------------------------------------------------------
 * Name:  c_handle_get_type
 *
 * Description: This API returns the handle type.
 *
 * Inputs:  h_handle  Contains the handle.
 *
 * Outputs: pe_type  Contains the handle type.
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_ARG     One or more arguments are invalid.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 c_handle_get_type (HANDLE_T        h_handle,
                         HANDLE_TYPE_T*  pe_type)
{
    return (x_handle_get_type (h_handle, pe_type));
}


/*-----------------------------------------------------------------------------
 * Name:  c_handle_set_tag
 *
 * Description: This API re-sets a handles tag value.
 *
 * Inputs:  h_handle  Contains the handle.
 *          pv_tag    Contains the tag value.
 *
 * Outputs: -
 *
 * Returns: HR_OK          Routine successful.
 *          HR_INV_HANDLE  The provided handle is invalid.
 *          HR_NOT_INIT    The handle library is not initialized yet.
 ----------------------------------------------------------------------------*/
INT32 c_handle_set_tag (HANDLE_T  h_handle,
                        VOID*     pv_tag)
{
    return (x_handle_set_tag (h_handle, pv_tag));
}


/*-----------------------------------------------------------------------------
 * Name:  c_handle_valid
 *
 * Description: This API returns TRUE if the given handle is valid else FALSE.
 *
 * Inputs:  h_handle  Contains the handle which shall be validated.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle is valid else FALSE.
 ----------------------------------------------------------------------------*/
BOOL c_handle_valid (HANDLE_T  h_handle)
{
    return (x_handle_valid (h_handle));
}
#endif

