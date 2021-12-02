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
 * $RCSfile: rm_drv_mc_nfy_tv_dec.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the driver specific implementation of the
 *         multi client notify feature.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "handle/x_handle.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "res_mngr/_rm.h"
#include "res_mngr/rm_mc_nfy.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_tv_dec.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Some macros which will convert to and from the notify tag. */
#define MAKE_TAG(_ptr)  ((VOID*) ((((UINT32) (_ptr - pt_tv_dec_dir_head)) & 0x0000ffff) | ((((UINT32) _ptr->ui1_count) << 16) & 0x00ff0000)))

#define GET_IDX(_tag)    ((UINT16) (((UINT32) _tag) & 0x0000ffff))
#define GET_COUNT(_tag)  ((UINT8) ((((UINT32) _tag) & 0x00ff0000) >> 16))

/* Allocation sizes for device directory */
#define NUM_ALLOC_DIR  8

/* Max number of instances per device. */
#define MAX_NUM_INST  21

/* Instance control structure. */
typedef struct _TV_DEC_INST_CTRL_T
{
    x_tv_dec_nfy_fct     pf_tv_dec_nfy;                 /* Status notification. */
    x_tv_dec_cc_nfy_fct  pf_tv_dec_cc_nfy;              /* Closed caption notification. */

    VOID*  pv_nfy_tag;
    VOID*  pv_cc_nfy_tag;

    HANDLE_T  h_obj;
}   TV_DEC_INST_CTRL_T;

#define DEV_FLAG_NFY_FCT_SET     ((UINT8) 0x01)
#define DEV_FLAG_CC_NFY_FCT_SET  ((UINT8) 0x02)

/* Device control structure. */
typedef struct _TV_DEC_DEV_CTRL_T
{
    TV_DEC_INST_CTRL_T  at_tv_dec_inst [MAX_NUM_INST];

    union
    {
        TV_DEC_NFY_INFO_T  tv_dec_nfy_info;             /* The following fields carry the notify info */
        TV_DEC_CC_NFY_INFO_T     tv_dec_cc_nfy_info;    /* in case the info must be modified.         */
    }   u;

    UINT16  ui2_curr_inst_entry;                        /* Last valid entry in the instance array. */

    UINT8  ui1_flags;                                   /* Some flags. */
}   TV_DEC_DEV_CTRL_T;

/* Device directory structure. */
typedef struct _TV_DEC_DIR_CTRL_T
{
    TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev;                  /* References the device control object. */

    UINT16  ui2_dev_id;                                 /* Contains the unique device id. */

    UINT8  ui1_count;                                   /* Used to create unique notify tag values. */
}   TV_DEC_DIR_CTRL_T;


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static UINT16  ui2_num_dir_entries;
static UINT16  ui2_curr_dir_entry;

static TV_DEC_DIR_CTRL_T*  pt_tv_dec_dir_head;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static TV_DEC_DIR_CTRL_T* alloc_dir (VOID);
static TV_DEC_DIR_CTRL_T* find_dir (UINT16  ui2_dev_id);

static TV_DEC_INST_CTRL_T* alloc_inst (TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev);
static TV_DEC_INST_CTRL_T* find_inst (TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev,
                                      HANDLE_T            h_obj);

static INT32 tv_decoder_add_dev (UINT16  ui2_dev_id);
static VOID tv_decoder_del_dev (UINT16  ui2_dev_id);
static VOID tv_decoder_rst_dev (UINT16  ui2_dev_id);
static INT32 tv_decoder_add_inst (HANDLE_T  h_obj,
                                  UINT16    ui2_dev_id);
static VOID tv_decoder_del_inst (HANDLE_T  h_obj,
                                 UINT16    ui2_dev_id);
static BOOL tv_decoder_set (HANDLE_T        h_obj,
                            UINT16          ui2_dev_id,
                            DRV_COMP_ID_T*  pt_comp_id,
                            DRV_SET_TYPE_T  e_set_type,
                            const VOID**    ppv_set_info,
                            SIZE_T*         pz_set_info_len,
                            INT32*          pi4_return);

static VOID tv_dec_nfy_fct (VOID*          pv_nfy_tag,
                            TV_DEC_COND_T  e_nfy_cond,
                            UINT32         ui4_data_1,
                            UINT32         ui4_data_2);
static VOID tv_dec_cc_nfy_fct (VOID*             pv_tag,
                               TV_DEC_CC_COND_T  e_cc_cond,
                               VOID*             pv_arg);


/*-----------------------------------------------------------------------------
                    initialized constant data
 ----------------------------------------------------------------------------*/

const static MC_NFY_FCT_TBL_T  t_mc_nfy_tv_decoder =
{
    DRVT_TV_DEC,

    tv_decoder_add_dev,
    tv_decoder_del_dev,
    tv_decoder_rst_dev,
    tv_decoder_add_inst,
    tv_decoder_del_inst,
    tv_decoder_set
};


/*-----------------------------------------------------------------------------
 * Name: alloc_dir
 *
 * Description: This API will allocate a new directory entry. If required, it
 *              will also expand the directory entry array. In case no entry
 *              could be allocated this function will return NULL.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the allocated directory control object or NULL.
 ----------------------------------------------------------------------------*/
static TV_DEC_DIR_CTRL_T* alloc_dir (VOID)
{
    UINT16             ui2_idx;
    CRIT_STATE_T       t_state;
    TV_DEC_DIR_CTRL_T* pt_tv_dec_dir;
    TV_DEC_DIR_CTRL_T* pt_tv_dec_dir_del;


    pt_tv_dec_dir = NULL;

    /* First, check if there is an empty entry in the existing list. */
    ui2_idx = 0;
    while ((ui2_idx < ui2_curr_dir_entry)                        &&
           (pt_tv_dec_dir_head [ui2_idx].pt_tv_dec_dev != NULL))
    {
        ui2_idx++;
    }

    if (ui2_idx >= ui2_curr_dir_entry)
    {
        if ((ui2_idx = ui2_curr_dir_entry) >= ui2_num_dir_entries)
        {
            /* Ok, we first have to create some space. */
            if ((pt_tv_dec_dir = (TV_DEC_DIR_CTRL_T*) x_mem_alloc ((ui2_num_dir_entries + NUM_ALLOC_DIR) * sizeof (TV_DEC_DIR_CTRL_T))) != NULL)
            {
                x_memset (((VOID*) pt_tv_dec_dir), 0, ((ui2_num_dir_entries + NUM_ALLOC_DIR) * sizeof (TV_DEC_DIR_CTRL_T)));

                /* Copy the existing data. */
                if (pt_tv_dec_dir_head != NULL)
                {
                    x_memcpy (((VOID*) pt_tv_dec_dir), ((VOID*) pt_tv_dec_dir_head), (ui2_num_dir_entries * sizeof (TV_DEC_DIR_CTRL_T)));
                }

                /* And swap the old ad new directories. This operation must */
                /* be done inside a critical section since the directory is */
                /* also accessed from within an ISR context.                */
                t_state = x_crit_start ();

                pt_tv_dec_dir_del  = pt_tv_dec_dir_head;
                pt_tv_dec_dir_head = pt_tv_dec_dir;

                ui2_num_dir_entries += NUM_ALLOC_DIR;

                x_crit_end (t_state);

                /* And free the previous directory if required. */
                if (pt_tv_dec_dir_del != NULL)
                {
                    x_mem_free ((VOID*) pt_tv_dec_dir_del);
                }

                pt_tv_dec_dir = NULL;
            }
        }
    }

    /* And return a reference to the next available directory entry. */
    if (ui2_idx < ui2_num_dir_entries)
    {
        pt_tv_dec_dir = &(pt_tv_dec_dir_head [ui2_idx]);
    }

    return (pt_tv_dec_dir);
}


/*-----------------------------------------------------------------------------
 * Name: find_dir
 *
 * Description: This API searches through the whole directory list for an
 *              entry with matching unique device id.
 *
 * Inputs:  ui2_dev_id  A unique id which identifies this driver component.
 *
 * Outputs: -
 *
 * Returns: A reference to the matching directory control object or NULL.
 ----------------------------------------------------------------------------*/
static TV_DEC_DIR_CTRL_T* find_dir (UINT16  ui2_dev_id)
{
    UINT16             ui2_idx;
    TV_DEC_DIR_CTRL_T* pt_tv_dec_dir;


    pt_tv_dec_dir = NULL;
    ui2_idx       = 0;

    while ((ui2_idx < ui2_num_dir_entries)                          &&
           (pt_tv_dec_dir_head [ui2_idx].ui2_dev_id != ui2_dev_id))
    {
        ui2_idx++;
    }

    if (ui2_idx < ui2_num_dir_entries)
    {
        pt_tv_dec_dir = &(pt_tv_dec_dir_head [ui2_idx]);
    }

    return (pt_tv_dec_dir);
}


/*-----------------------------------------------------------------------------
 * Name: alloc_inst
 *
 * Description: This API will allocate a new instance entry. In case no entry
 *              could be allocated this function will return NULL.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the allocated instance control object or NULL.
 ----------------------------------------------------------------------------*/
static TV_DEC_INST_CTRL_T* alloc_inst (TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev)
{
    UINT16              ui2_idx;
    TV_DEC_INST_CTRL_T* pt_tv_dec_inst;


    pt_tv_dec_inst = NULL;
    ui2_idx        = 0;

    while ((ui2_idx < MAX_NUM_INST)                                        &&
           (pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].h_obj != NULL_HANDLE))
    {
        ui2_idx++;
    }

    if (ui2_idx < MAX_NUM_INST)
    {
        pt_tv_dec_inst = &(pt_tv_dec_dev->at_tv_dec_inst [ui2_idx]);
    }

    return (pt_tv_dec_inst);
}


/*-----------------------------------------------------------------------------
 * Name: find_inst
 *
 * Description: This API searches through the whole instance list of a
 *              directory entry for an entry with matching object handle.
 *
 * Inputs:  pt_tv_dec_dev  Refernces the device control object.
 *          h_obj          Contains the object handle.
 *
 * Outputs: -
 *
 * Returns: A reference to the matching instance control object or NULL.
 ----------------------------------------------------------------------------*/
static TV_DEC_INST_CTRL_T* find_inst (TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev,
                                      HANDLE_T            h_obj)
{
    UINT16              ui2_idx;
    TV_DEC_INST_CTRL_T* pt_tv_dec_inst;


    pt_tv_dec_inst = NULL;
    ui2_idx        = 0;

    while ((ui2_idx < MAX_NUM_INST)                                  &&
           (pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].h_obj != h_obj))
    {
        ui2_idx++;
    }

    if (ui2_idx < MAX_NUM_INST)
    {
        pt_tv_dec_inst = &(pt_tv_dec_dev->at_tv_dec_inst [ui2_idx]);
    }

    return (pt_tv_dec_inst);
}


/*-----------------------------------------------------------------------------
 * Name: tv_decoder_add_dev
 *
 * Description: This API will allocate a device control block for a single
 *              TV-Decoder driver component.
 *
 * Inputs:  ui2_dev_id  A unique id which identifies this driver component.
 *
 * Outputs: -
 *
 * Returns: RMR_OK                       Routine successful.
 *          RMR_NOT_ENOUGH_MEM           Out of memory.
 *          RMR_DUPLICATE_MC_NFY_DEV_ID  Duplicate device id.
 ----------------------------------------------------------------------------*/
static INT32 tv_decoder_add_dev (UINT16  ui2_dev_id)
{
    INT32              i4_return;
    CRIT_STATE_T       t_state;
    TV_DEC_DEV_CTRL_T* pt_tv_dec_dev;
    TV_DEC_DIR_CTRL_T* pt_tv_dec_dir;


    i4_return = RMR_DUPLICATE_MC_NFY_DEV_ID;

    if (find_dir (ui2_dev_id) == NULL)
    {
        i4_return = RMR_NOT_ENOUGH_MEM;

        if (((pt_tv_dec_dir = alloc_dir ())                                                  != NULL)  &&
            ((pt_tv_dec_dev = (TV_DEC_DEV_CTRL_T*) x_mem_alloc (sizeof (TV_DEC_DEV_CTRL_T))) != NULL))
        {
            i4_return = RMR_OK;

            x_memset (((VOID*) pt_tv_dec_dev), 0, sizeof (TV_DEC_DEV_CTRL_T));

            /* The assignment must be done inside a critical section since the */
            /* device control blcok may be accessed from an ISR context.       */
            t_state = x_crit_start ();

            pt_tv_dec_dir->pt_tv_dec_dev = pt_tv_dec_dev;
            pt_tv_dec_dir->ui2_dev_id    = ui2_dev_id;

            x_crit_end (t_state);

            /* And update the current directory entry. Note that this must be done */
            /* inside a critical section since the varible 'ui2_curr_dir_entry'    */
            /* may also be accessed from within an ISR context.                    */
            if ((UINT16) (pt_tv_dec_dir - pt_tv_dec_dir_head) == ui2_curr_dir_entry)
            {
                t_state = x_crit_start ();

                ui2_curr_dir_entry++;

                x_crit_end (t_state);
            }
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: tv_decoder_del_dev
 *
 * Description: This API will free a device control block for a single
 *              TV-Decoder driver component.
 *
 * Inputs:  ui2_dev_id  A unique id which identifies this driver component.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID tv_decoder_del_dev (UINT16  ui2_dev_id)
{
    CRIT_STATE_T       t_state;
    TV_DEC_DEV_CTRL_T* pt_tv_dec_dev;
    TV_DEC_DIR_CTRL_T* pt_tv_dec_dir;


    if ((pt_tv_dec_dir = find_dir (ui2_dev_id)) != NULL)
    {
        /* First, increment the count value to ensure that any future   */
        /* notification for this device will be rejected and also clear */
        /* the device control object reference.                         */
        pt_tv_dec_dev = pt_tv_dec_dir->pt_tv_dec_dev;

        t_state = x_crit_start ();

        pt_tv_dec_dir->pt_tv_dec_dev = NULL;
        pt_tv_dec_dir->ui2_dev_id    = 0;
        pt_tv_dec_dir->ui1_count++;

        x_crit_end (t_state);

        /* And now, free up the device control object. */
        if (pt_tv_dec_dev != NULL)
        {
            x_mem_free ((VOID*) pt_tv_dec_dev);
        }

        /* And at last, update the current entry index if required. Again, */
        /* this must be done inside a critical section since this variable */
        /* might also be accessed from within an ISR context.              */
        if ((UINT16) ((pt_tv_dec_dir - pt_tv_dec_dir_head) + 1) == ui2_curr_dir_entry)
        {
            t_state = x_crit_start ();

            ui2_curr_dir_entry--;

            x_crit_end (t_state);
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: tv_decoder_rst_dev
 *
 * Description: This API will reset a device control block for a single
 *              TV-Decoder driver component.
 *
 * Inputs:  ui2_dev_id  A unique id which identifies this driver component.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID tv_decoder_rst_dev (UINT16  ui2_dev_id)
{
    UINT16             ui2_idx;
    CRIT_STATE_T       t_state;
    TV_DEC_DIR_CTRL_T* pt_tv_dec_dir;
    TV_DEC_DEV_CTRL_T* pt_tv_dec_dev;


    if ((pt_tv_dec_dir = find_dir (ui2_dev_id)) != NULL)
    {
        if ((pt_tv_dec_dev = pt_tv_dec_dir->pt_tv_dec_dev) == NULL)
        {
            /* Very bad. Must never happen. */
            ABORT (DBG_CAT_MEMORY, DBG_ABRT_DB_CORRUPTION);
        }

        /* First, make certain that no further notify function can be   */
        /* processed. This must be done within a critical section since */
        /* since the notification may be issued from an ISR context.    */
        t_state = x_crit_start ();

        pt_tv_dec_dir->ui1_count++;
        pt_tv_dec_dev->ui1_flags = 0;

        x_crit_end (t_state);

        /* And now cycle through all instances and reset them. This */
        /* operation does not need to be encapsulated inside a      */
        /* critical section since the field 'ui2_curr_inst_entry'   */
        /* has already been reset.                                  */
        ui2_idx = 0;

        while (ui2_idx < MAX_NUM_INST)
        {
            pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].pf_tv_dec_nfy    = NULL;
            pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].pf_tv_dec_cc_nfy = NULL;
            pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].pv_nfy_tag       = NULL;
            pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].pv_cc_nfy_tag    = NULL;

            ui2_idx++;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: tv_decoder_add_inst
 *
 * Description: This API will allocate an instance for a single TV-Decoder
 *              driver component.
 *
 * Inputs:  h_obj       Contains a unique handle which identifies the
 *                      instance.
 *          ui2_dev_id  A unique id which identifies this driver component.
 *
 * Outputs: -
 *
 * Returns: RMR_OK                         Routine successful.
 *          RMR_NOT_ENOUGH_MEM             Out of memory.
 *          RMR_UNKNOWN_MC_NFY_DEV_ID      Unknown device id.
 *          RMR_DUPLICATE_MC_NFY_INSTANCE  Duplicate handle object.
 ----------------------------------------------------------------------------*/
static INT32 tv_decoder_add_inst (HANDLE_T  h_obj,
                                  UINT16    ui2_dev_id)
{
    INT32               i4_return;
    CRIT_STATE_T        t_state;
    TV_DEC_DIR_CTRL_T*  pt_tv_dec_dir;
    TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev;
    TV_DEC_INST_CTRL_T* pt_tv_dec_inst;


    i4_return = RMR_UNKNOWN_MC_NFY_DEV_ID;

    if ((pt_tv_dec_dir = find_dir (ui2_dev_id)) != NULL)
    {
        i4_return = RMR_DUPLICATE_MC_NFY_INSTANCE;

        if ((pt_tv_dec_dev = pt_tv_dec_dir->pt_tv_dec_dev) == NULL)
        {
            /* Very bad. Must never happen. */
            ABORT (DBG_CAT_MEMORY, DBG_ABRT_DB_CORRUPTION);
        }

        /* Ensure that this handle is not alreday part of the database. */
        if (find_inst (pt_tv_dec_dev, h_obj) == NULL)
        {
            i4_return = RMR_NOT_ENOUGH_MEM;

            if ((pt_tv_dec_inst = alloc_inst (pt_tv_dec_dev)) != NULL)
            {
                i4_return = RMR_OK;

                /* Ok, enter the new entry. Note that the new entry can be added */
                /* outside of the critical section since the field               */
                /* 'ui2_curr_inst_entry' defines the number of valid entries.    */
                pt_tv_dec_inst->pf_tv_dec_nfy    = NULL;
                pt_tv_dec_inst->pf_tv_dec_cc_nfy = NULL;
                pt_tv_dec_inst->pv_nfy_tag       = NULL;
                pt_tv_dec_inst->pv_cc_nfy_tag    = NULL;
                pt_tv_dec_inst->h_obj            = h_obj;

                /* And now, update the number of entries if required. */
                if ((UINT16) (pt_tv_dec_inst - &(pt_tv_dec_dev->at_tv_dec_inst [0])) >= pt_tv_dec_dev->ui2_curr_inst_entry)
                {
                    t_state = x_crit_start ();

                    pt_tv_dec_dev->ui2_curr_inst_entry++;

                    x_crit_end (t_state);
                }
            }
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: tv_decoder_del_inst
 *
 * Description: This API will delete an instance for a single TV-Decoder
 *              driver component.
 *
 * Inputs:  h_obj       Contains a unique handle which identifies the
 *                      instance.
 *          ui2_dev_id  A unique id which identifies this driver component.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID tv_decoder_del_inst (HANDLE_T  h_obj,
                                 UINT16    ui2_dev_id)
{
    CRIT_STATE_T        t_state;
    TV_DEC_DIR_CTRL_T*  pt_tv_dec_dir;
    TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev;
    TV_DEC_INST_CTRL_T* pt_tv_dec_inst;


    if ((pt_tv_dec_dir = find_dir (ui2_dev_id)) != NULL)
    {
        if ((pt_tv_dec_dev = pt_tv_dec_dir->pt_tv_dec_dev) == NULL)
        {
            /* Very bad. Must never happen. */
            ABORT (DBG_CAT_MEMORY, DBG_ABRT_DB_CORRUPTION);
        }

        /* Ensure that this handle is part of the database. */
        if ((pt_tv_dec_inst = find_inst (pt_tv_dec_dev, h_obj)) != NULL)
        {
            /* The deletion of the instance entry must be done within */
            /* a critical section since these fields can be accessed  */
            /* from within an ISR context.                            */
            t_state = x_crit_start ();

            pt_tv_dec_inst->pf_tv_dec_nfy    = NULL;
            pt_tv_dec_inst->pf_tv_dec_cc_nfy = NULL;

            x_crit_end (t_state);

            pt_tv_dec_inst->pv_nfy_tag    = NULL;
            pt_tv_dec_inst->pv_cc_nfy_tag = NULL;
            pt_tv_dec_inst->h_obj         = NULL_HANDLE;

            /* And update the number of entries if required. */
            if ((UINT16) (pt_tv_dec_inst - &(pt_tv_dec_dev->at_tv_dec_inst [0])) >= pt_tv_dec_dev->ui2_curr_inst_entry)
            {
                /* And now clear out all the old deleted instances. */
                while ((pt_tv_dec_dev->ui2_curr_inst_entry > 0)                                                       &&
                       (pt_tv_dec_dev->at_tv_dec_inst [pt_tv_dec_dev->ui2_curr_inst_entry - 1].h_obj == NULL_HANDLE))
                {
                    t_state = x_crit_start ();

                    pt_tv_dec_dev->ui2_curr_inst_entry--;

                    x_crit_end (t_state);
                }
            }
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: tv_decoder_set
 *
 * Description: This API will intercept the set notify functions to the TV-
 *              Decoder and route them to its own callback functions, which
 *              will in turn allow the notification of multiple clients. If
 *              this API determines that the set operation shall be executed
 *              by the driver component, it will return TRUE, else FALSE.
 *
 * Inputs:  h_obj            Contains a unique handle which identifies the
 *                           instance.
 *          ui2_dev_id       A unique id which identifies this driver
 *                           component.
 *          pt_comp_id       References the component ids.
 *          e_set_type       Contains the set operation.
 *          ppv_set_info     References the set info data.
 *          pz_set_info_len  Contains the set info data length.
 *
 * Outputs: ppv_set_info     References the (maybe) modified set info data.
 *          pz_set_info_len  Contains the (maybe) modified set info data
 *                           length.
 *          pi4_return       Contains the return value to the Resource
 *                           Manager client in case this API returns FALSE.
 *
 * Returns: TRUE if the set operation shall be executed by the driver
 *          component else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL tv_decoder_set (HANDLE_T        h_obj,
                            UINT16          ui2_dev_id,
                            DRV_COMP_ID_T*  pt_comp_id,
                            DRV_SET_TYPE_T  e_set_type,
                            const VOID**    ppv_set_info,
                            SIZE_T*         pz_set_info_len,
                            INT32*          pi4_return)
{
    BOOL                b_drv_exec;
    UINT16              ui2_idx;
    CRIT_STATE_T        t_state;
    TV_DEC_DIR_CTRL_T*  pt_tv_dec_dir;
    TV_DEC_DEV_CTRL_T*  pt_tv_dec_dev;
    TV_DEC_INST_CTRL_T* pt_tv_dec_inst;


    /* Default operation is to let the set operation go to the driver component. */
    b_drv_exec = TRUE;

    /* Only process the set notify function set operations. */
    if ((e_set_type == TV_DEC_SET_TYPE_NFY_FCT)  ||
        (e_set_type == TV_DEC_SET_TYPE_PLAY_CC)  ||
        (e_set_type == TV_DEC_SET_TYPE_STOP_CC))
    {
        /* Here, the default return value indicates that the set operations */
        /* must not be performed by the driver component.                   */
        b_drv_exec    = FALSE;
        (*pi4_return) = RMR_DRV_ERROR;

        if ((pt_tv_dec_dir = find_dir (ui2_dev_id)) != NULL)
        {
            if ((pt_tv_dec_dev = pt_tv_dec_dir->pt_tv_dec_dev) == NULL)
            {
                /* Very bad. Must never happen. */
                ABORT (DBG_CAT_MEMORY, DBG_ABRT_DB_CORRUPTION);
            }

            /* Ensure that this handle is part of the database. */
            if ((pt_tv_dec_inst = find_inst (pt_tv_dec_dev, h_obj)) != NULL)
            {
                switch (e_set_type)
                {
                    case TV_DEC_SET_TYPE_NFY_FCT:
                        /* Validate the arguments. */
                        if ((ppv_set_info    != NULL)  &&
                            ((*ppv_set_info) != NULL)  &&
                            (pz_set_info_len != NULL))
                        {
                            (*pi4_return) = RMR_OK;

                            /* Remeber the notify structure. */
                            pt_tv_dec_dev->u.tv_dec_nfy_info = (*((TV_DEC_NFY_INFO_T*) (*ppv_set_info)));

                            /* Set the instance database. This must be done inside a critical section */
                            /* since the database may be accessed from within an ISR context.         */
                            t_state = x_crit_start ();

                            pt_tv_dec_inst->pf_tv_dec_nfy = ((TV_DEC_NFY_INFO_T*) (*ppv_set_info))->pf_tv_dec_nfy;
                            pt_tv_dec_inst->pv_nfy_tag    = ((TV_DEC_NFY_INFO_T*) (*ppv_set_info))->pv_tag;

                            x_crit_end (t_state);

                            if ((pt_tv_dec_dev->ui1_flags & DEV_FLAG_NFY_FCT_SET) == 0)
                            {
                                /* Signal that we still need to issue the set opartion */
                                /* to the actual driver component.                     */
                                b_drv_exec = TRUE;

                                /* Ok, we still need to set the notify function. */
                                pt_tv_dec_dev->u.tv_dec_nfy_info.pf_tv_dec_nfy = tv_dec_nfy_fct;
                                pt_tv_dec_dev->u.tv_dec_nfy_info.pv_tag        = MAKE_TAG (pt_tv_dec_dir);

                                (*ppv_set_info)    = (VOID*) &(pt_tv_dec_dev->u.tv_dec_nfy_info);
                                (*pz_set_info_len) = sizeof (TV_DEC_NFY_INFO_T);

                                /* And signal that the notify function has been set. */
                                t_state = x_crit_start ();

                                pt_tv_dec_dev->ui1_flags |= DEV_FLAG_NFY_FCT_SET;

                                x_crit_end (t_state);
                            }
                        }
                        break;

                    case TV_DEC_SET_TYPE_PLAY_CC:
                        /* Validate the arguments. */
                        if ((ppv_set_info    != NULL)  &&
                            ((*ppv_set_info) != NULL)  &&
                            (pz_set_info_len != NULL))
                        {
                            (*pi4_return) = RMR_OK;

                            /* Remeber the notify structure. */
                            pt_tv_dec_dev->u.tv_dec_cc_nfy_info = (*((TV_DEC_CC_NFY_INFO_T*) (*ppv_set_info)));

                            /* Set the instance database. This must be done inside a critical section */
                            /* since the database may be accessed from within an ISR context.         */
                            t_state = x_crit_start ();

                            pt_tv_dec_inst->pf_tv_dec_cc_nfy = ((TV_DEC_CC_NFY_INFO_T*) (*ppv_set_info))->pf_cc_nfy;
                            pt_tv_dec_inst->pv_cc_nfy_tag    = ((TV_DEC_CC_NFY_INFO_T*) (*ppv_set_info))->pv_tag;

                            x_crit_end (t_state);

                            if ((pt_tv_dec_dev->ui1_flags & DEV_FLAG_CC_NFY_FCT_SET) == 0)
                            {
                                /* Signal that we still need to issue the set opartion */
                                /* to the actual driver component.                     */
                                b_drv_exec = TRUE;

                                /* Ok, we still need to set the notify function. */
                                pt_tv_dec_dev->u.tv_dec_cc_nfy_info.pf_cc_nfy = tv_dec_cc_nfy_fct;
                                pt_tv_dec_dev->u.tv_dec_cc_nfy_info.pv_tag    = MAKE_TAG (pt_tv_dec_dir);

                                (*ppv_set_info)    = (VOID*) &(pt_tv_dec_dev->u.tv_dec_cc_nfy_info);
                                (*pz_set_info_len) = sizeof (TV_DEC_CC_NFY_INFO_T);

                                /* And signal that the notify function has been set. */
                                t_state = x_crit_start ();

                                pt_tv_dec_dev->ui1_flags |= DEV_FLAG_CC_NFY_FCT_SET;

                                x_crit_end (t_state);
                            }
                        }
                        break;

                    case TV_DEC_SET_TYPE_STOP_CC:
                        (*pi4_return) = RMR_OK;

                        if ((pt_tv_dec_dev->ui1_flags & DEV_FLAG_CC_NFY_FCT_SET) != 0)
                        {
                            /* Signal that we still need to issue the set opartion */
                            /* to the actual driver component.                     */
                            b_drv_exec = TRUE;

                            /* And reset the CC notify parts for all instances. */
                            t_state = x_crit_start ();

                            pt_tv_dec_dev->ui1_flags &= ~(DEV_FLAG_CC_NFY_FCT_SET);

                            x_crit_end (t_state);

                            ui2_idx = 0;

                            while (ui2_idx < MAX_NUM_INST)
                            {
                                pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].pf_tv_dec_cc_nfy = NULL;
                                pt_tv_dec_dev->at_tv_dec_inst [ui2_idx].pv_cc_nfy_tag    = NULL;

                                ui2_idx++;
                            }
                        }
                        break;

                    default:
                        /* Will never reach here. */
                        break;
                }
            }
        }
    }

    return (b_drv_exec);
}


/*-----------------------------------------------------------------------------
 * Name: tv_dec_nfy_fct
 *
 * Description: This is the status notify function called directly by the
 *              TV-Decoder driver component.
 *
 * Inputs:  pv_tag      The tag value when the notify function was set.
 *          e_nfy_cond  The notify condition.
 *          ui4_data_1  Data which depends on the notify condition.
 *          ui4_data_2  Data which depends on the notify condition.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID tv_dec_nfy_fct (VOID*          pv_tag,
                            TV_DEC_COND_T  e_nfy_cond,
                            UINT32         ui4_data_1,
                            UINT32         ui4_data_2)
{
    VOID*                pv_nfy_tag;
    UINT8                ui1_count;
    UINT16               ui2_idx;
    CRIT_STATE_T         t_state;
    x_tv_dec_nfy_fct     pf_tv_dec_nfy;
    TV_DEC_DIR_CTRL_T*   pt_tv_dec_dir;
    TV_DEC_DEV_CTRL_T*   pt_tv_dec_dev;
    TV_DEC_INST_CTRL_T*  pt_tv_dec_inst;


    /* First, validate the tag. This must be done inside a critical section */
    /* since the database could be updated from a thread context.           */
    t_state = x_crit_start ();

    if (((ui2_idx = GET_IDX (pv_tag)) < ui2_curr_dir_entry)                          &&
        ((ui1_count = GET_COUNT (pv_tag)) == pt_tv_dec_dir_head [ui2_idx].ui1_count) &&
        ((pt_tv_dec_dev = pt_tv_dec_dir_head [ui2_idx].pt_tv_dec_dev) != NULL)       &&
        ((pt_tv_dec_dev->ui1_flags & DEV_FLAG_NFY_FCT_SET) != 0))
    {
        /* Tag is valid. Now notify all interested parties. */
        x_crit_end (t_state);

        pt_tv_dec_dir  = &(pt_tv_dec_dir_head [ui2_idx]);
        pt_tv_dec_inst = &(pt_tv_dec_dev->at_tv_dec_inst [0]);

        ui2_idx = 0;

        /* The following loop checking must be done inside a critical section  */
        /* since it may be updated from within a thread context. One must also */
        /* ensure that the directory control object has not been modified.     */
        t_state = x_crit_start ();

        while ((ui1_count == pt_tv_dec_dir->ui1_count)         &&
               (ui2_idx < pt_tv_dec_dev->ui2_curr_inst_entry))
        {
            if ((pf_tv_dec_nfy = pt_tv_dec_inst->pf_tv_dec_nfy) != NULL)
            {
                pv_nfy_tag = pt_tv_dec_inst->pv_nfy_tag;

                x_crit_end (t_state);

                /* And exeute the clients real callback function. */
                pf_tv_dec_nfy (pv_nfy_tag, e_nfy_cond, ui4_data_1, ui4_data_2);
            }
            else
            {
                x_crit_end (t_state);
            }

            /* Select the next instance. */
            ui2_idx++;
            pt_tv_dec_inst++;

            t_state = x_crit_start ();
        }
    }

    /* And don't forget to end the critical section. */
    x_crit_end (t_state);
}


/*-----------------------------------------------------------------------------
 * Name: tv_dec_cc_nfy_fct
 *
 * Description: This is the closed caption notify function called directly by
 *              the TV-Decoder driver component.
 *
 * Inputs:  pv_tag         The tag value when the notify function was set.
 *          e_cc_nfy_cond  The notify condition.
 *          pv_arg         Closed caption data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID tv_dec_cc_nfy_fct (VOID*             pv_tag,
                               TV_DEC_CC_COND_T  e_cc_nfy_cond,
                               VOID*             pv_arg)
{
    VOID*                pv_cc_nfy_tag;
    UINT8                ui1_count;
    UINT16               ui2_idx;
    CRIT_STATE_T         t_state;
    TV_DEC_DIR_CTRL_T*   pt_tv_dec_dir;
    TV_DEC_DEV_CTRL_T*   pt_tv_dec_dev;
    TV_DEC_INST_CTRL_T*  pt_tv_dec_inst;
    x_tv_dec_cc_nfy_fct  pf_tv_dec_cc_nfy;


    /* First, validate the tag. This must be done inside a critical section */
    /* since the database could be updated from a thread context.           */
    t_state = x_crit_start ();

    if (((ui2_idx = GET_IDX (pv_tag)) < ui2_curr_dir_entry)                          &&
        ((ui1_count = GET_COUNT (pv_tag)) == pt_tv_dec_dir_head [ui2_idx].ui1_count) &&
        ((pt_tv_dec_dev = pt_tv_dec_dir_head [ui2_idx].pt_tv_dec_dev) != NULL)       &&
        ((pt_tv_dec_dev->ui1_flags & DEV_FLAG_CC_NFY_FCT_SET) != 0))
    {
        /* Tag is valid. Now notify all interested parties. */
        x_crit_end (t_state);

        pt_tv_dec_dir  = &(pt_tv_dec_dir_head [ui2_idx]);
        pt_tv_dec_inst = &(pt_tv_dec_dev->at_tv_dec_inst [0]);

        ui2_idx = 0;

        /* The following loop checking must be done inside a critical section  */
        /* since it may be updated from within a thread context. One must also */
        /* ensure that the directory control object has not been modified.     */
        t_state = x_crit_start ();

        while ((ui1_count == pt_tv_dec_dir->ui1_count)         &&
               (ui2_idx < pt_tv_dec_dev->ui2_curr_inst_entry))
        {
            if ((pf_tv_dec_cc_nfy = pt_tv_dec_inst->pf_tv_dec_cc_nfy) != NULL)
            {
                pv_cc_nfy_tag = pt_tv_dec_inst->pv_cc_nfy_tag;

                x_crit_end (t_state);

                /* And exeute the clients real callback function. */
                pf_tv_dec_cc_nfy (pv_cc_nfy_tag, e_cc_nfy_cond, pv_arg);
            }
            else
            {
                x_crit_end (t_state);
            }

            /* Select the next instance. */
            ui2_idx++;
            pt_tv_dec_inst++;

            t_state = x_crit_start ();
        }
    }

    /* And don't forget to end the critical section. */
    x_crit_end (t_state);
}


/*-----------------------------------------------------------------------------
 * Name: rm_drv_mc_nfy_tv_dec_init
 *
 * Description: This API will initialize the multi client notify feature for
 *              a TV-Decoder driver component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: RMR_OK                        Routine successful.
 *          RMR_NOT_INIT                  The Resource Manager is not
 *                                        initialized.
 *          RMR_INV_ARG                   One or more of the arguments contain
 *                                        invalid data.
 *          RMR_NOT_ENOUGH_MEM            Out of memory.
 *          RMR_DUPLICATE_MC_NFY_FCT_TBL  Multi client notify function table
 *                                        for component type exists alreday.
 ----------------------------------------------------------------------------*/
INT32 rm_drv_mc_nfy_tv_dec_init (VOID)
{
    ui2_num_dir_entries = 0;
    ui2_curr_dir_entry  = 0;

    pt_tv_dec_dir_head = NULL;

    return (rm_reg_mc_nfy (&t_mc_nfy_tv_decoder));
}

