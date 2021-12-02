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
 * $RCSfile: dm_dlna.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 34076dbf72c1e756a741024f89fac47f $
 *
 * Description: 
 *         This file contains DLNA DM exported API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifdef DLNA_SUPPORT

#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "inc/common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "libc/ctype.h"
#include "util/x_lnk_list.h"
#include "dm/x_dm.h"
#include "dm/x_dm_dlna.h"
#include "dm/c_dm_dlna.h"
#include "inet/dlna/x_dlna_dmp_api.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_socket_dev.h"
#include "inet/dlna/u_dlna_dmr.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define MAX_DLNA_DEV_NUM                256
#define MAX_DLNA_NFY_NUM                10

typedef struct _DM_DLNA_CALLBACK_T DM_DLNA_CALLBACK_T;
typedef struct _DM_DLNA_DEVICE_T DM_DLNA_DEVICE_T;

struct _DM_DLNA_CALLBACK_T
{
    BOOL                b_used;
    x_sktdev_st_nfy_fct pf_nfy;
    VOID*               pv_tag;
};


struct _DM_DLNA_DEVICE_T
{
    UINT32              ui4_id;
    BOOL                b_used;
    BOOL                b_added;
    VOID*               pv_dev;
    SKTDEV_DEV_INFO_T   t_dev_info;
};


/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/
static BOOL b_dmp_start = FALSE;
static BOOL b_dmp_init = FALSE;

static DM_DLNA_DEVICE_T     a_dev_list[MAX_DLNA_DEV_NUM];
static DM_DLNA_CALLBACK_T   a_nfy_list[MAX_DLNA_NFY_NUM];

static SKTDEV_DEV_INFO_T    t_sktdev_info;

static HANDLE_T  h_op_timer = NULL_HANDLE;
#define DLNA_ON_OFF_TIME 1000*2
static  HANDLE_T    h_op_lock;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static VOID _dlna_device_nfy (DLNA_DEVICE_EVENT_T t_event, 
                              VOID * pv_tag,
                              VOID * pv_arg)
{
    BOOL                b_found;
    UINT16              ui2_i;
    DLNA_DEVICE_TYPE_T  e_type;


    /*printf("[FakeDM/DLNA]_dlna_device_nfy: general event:0x%x pv_tag:0x%x pv_arg:0x%x\n", 
                          t_event,
                          pv_tag,
                          pv_arg);*/

    /* check is type equal to DMS */
    if(t_event == DLNA_DEVICE_EVENT_DMR_START){
        /* for DMR implementation
         * Do nothing here, just go down.*/
    }
    else{
        if(t_event != DLNA_DEVICE_EVENT_DMR_STOP){ /* DON'T use dmp method to check dmr object. */
            x_dlna_dmp_get_device_type(pv_arg, &e_type);
            /* x_dbg_stmt("dm_dlna.c receive event, e_type = %d, t_event=%d\n", e_type, t_event); */
            if ((DLNA_DEVICE_TYPE_DMS != e_type) && ( DLNA_DEVICE_TYPE_M_DMS != e_type))
            {
                /* printf("[FakeDM/DLNA]%s #%d return\n",__FUNCTION__,__LINE__);  */
                return;
            }
        }
    }
    /* DMR is treated as a DMS in dlna file system, only difference is they operations,
     * the differences between them are handled in dlna file system.
     * Here, DMR also holds a device file member as DMS. DM and FM don't know they are different type of file.*/
    if (DLNA_DEVICE_EVENT_FOUND_DEVS == t_event || DLNA_DEVICE_EVENT_DMR_START == t_event)
    {
        b_found = FALSE;
        
        for (ui2_i=0; ui2_i<MAX_DLNA_DEV_NUM; ui2_i++)
        {
            if (a_dev_list[ui2_i].b_used)
            {
                if (a_dev_list[ui2_i].pv_dev == pv_arg)
                {
                    b_found = TRUE;
                    break;
                }
            }
        }
        
        if (b_found)
        {
            
            /* printf("[FakeDM/DLNA]%s #%d return\n",__FUNCTION__,__LINE__);  */
            return;
        }
        
        b_found = FALSE;
        for (ui2_i=0; ui2_i<MAX_DLNA_DEV_NUM; ui2_i++)
        {
            if (!a_dev_list[ui2_i].b_used)
            {
                a_dev_list[ui2_i].b_used = TRUE;
                a_dev_list[ui2_i].b_added = TRUE;
                a_dev_list[ui2_i].pv_dev = pv_arg;
                a_dev_list[ui2_i].ui4_id = (UINT32)ui2_i;
                b_found = TRUE;
                break;
            }
        }
        
        if (b_found)
        {
            /*send device insertion event to DM*/
            t_sktdev_info.eDevSt = SKTDEV_STATUS_DEV_ATTACH;
            t_sktdev_info.u4SocketNo = ui2_i;

            t_sktdev_info.u_info.rSupDev.u4DevType = DRVT_DLNA_DMS;
            t_sktdev_info.u_info.rSupDev.u4CompId = ui2_i;
            t_sktdev_info.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;

            for (ui2_i=0; ui2_i<MAX_DLNA_NFY_NUM; ui2_i++)
            {
                if (a_nfy_list[ui2_i].b_used)
                {
                    a_nfy_list[ui2_i].pf_nfy(a_nfy_list[ui2_i].pv_tag, SKTDEV_STATUS_DEV_ATTACH, &t_sktdev_info);
                }
            }
        }

    }
    else if ((DLNA_DEVICE_EVENT_BYEBYE == t_event) || (DLNA_DEVICE_EVENT_UNAVAILABLE == t_event)
             || (DLNA_DEVICE_EVENT_DMR_STOP == t_event))
    {
        INT32 i4_del_index = -1;

        b_found = FALSE;
        
        for (ui2_i=0; ui2_i<MAX_DLNA_DEV_NUM; ui2_i++)
        {
            if (a_dev_list[ui2_i].b_used)
            {
                if (a_dev_list[ui2_i].pv_dev == pv_arg)
                {
                    a_dev_list[ui2_i].b_added = FALSE;
                    b_found = TRUE;
                    i4_del_index = (INT32)ui2_i;

                    break;
                }
            }
        }

        if (b_found)
        {
            /*send device removal event to DM*/
            t_sktdev_info.eDevSt = SKTDEV_STATUS_DEV_DETACH;
            t_sktdev_info.u4SocketNo = ui2_i;

            t_sktdev_info.u_info.rSupDev.u4DevType = DRVT_DLNA_DMS;
            t_sktdev_info.u_info.rSupDev.u4CompId = ui2_i;
            t_sktdev_info.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;

            for (ui2_i=0; ui2_i<MAX_DLNA_NFY_NUM; ui2_i++)
            {
                if (a_nfy_list[ui2_i].b_used)
                {
                    a_nfy_list[ui2_i].pf_nfy(a_nfy_list[ui2_i].pv_tag, SKTDEV_STATUS_DEV_DETACH, &t_sktdev_info);
                }
            }
            a_dev_list[i4_del_index].b_used = FALSE;
        }
    }
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
VOID x_dm_dlna_release_device(UINT32 ui4_id)
{
    /* comment by jinlong. before program goes here, the device have been set to be unused
     * in the device notify caller thread context.
     * This function will be called async, this will influence a new mount operation.*/
    /* a_dev_list[ui4_id].b_used = FALSE; */
}

VOID* x_dm_dlna_get_device(UINT32 ui4_id)
{
    if (a_dev_list[ui4_id].b_used && a_dev_list[ui4_id].b_added)
    {
        return a_dev_list[ui4_id].pv_dev;
    }
    else
    {
        return NULL;
    }
}

VOID x_dm_dlna_init(VOID)
{
    if (!b_dmp_init)
    {
        x_memset(a_dev_list, 0, MAX_DLNA_DEV_NUM*sizeof(DM_DLNA_DEVICE_T));
        x_memset(a_nfy_list, 0, MAX_DLNA_NFY_NUM*sizeof(DM_DLNA_CALLBACK_T));
        x_memset(&t_sktdev_info, 0, sizeof(SKTDEV_DEV_INFO_T));
        x_sema_create(&h_op_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK); /* todo ... need free somewhere ? */
        b_dmp_init = TRUE;
    }
}

INT32 x_dm_dlna_register_notify(x_sktdev_st_nfy_fct pf_sktdev_st_nfy, VOID*  pv_tag)
{
    BOOL                b_found;
    UINT16              ui2_i;
    
    b_found = FALSE;

    for (ui2_i=0; ui2_i<MAX_DLNA_NFY_NUM; ui2_i++)
    {
        if (a_nfy_list[ui2_i].b_used)
        {
            if ((a_nfy_list[ui2_i].pf_nfy == pf_sktdev_st_nfy) && (a_nfy_list[ui2_i].pv_tag == pv_tag))
            {
                b_found = TRUE;
                break;
            }
        }
    }

    if (b_found)
    {
        return DMR_ALREADY_INIT;
    }

    b_found = FALSE;
    for (ui2_i=0; ui2_i<MAX_DLNA_NFY_NUM; ui2_i++)
    {
        if (!a_nfy_list[ui2_i].b_used)
        {
            a_nfy_list[ui2_i].b_used = TRUE;
            a_nfy_list[ui2_i].pf_nfy = pf_sktdev_st_nfy;
            a_nfy_list[ui2_i].pv_tag = pv_tag;
            b_found = TRUE;
            break;
        }
    }

    if (!b_found)
    {
        return DMR_NO_MEM;
    }
    
    return DMR_OK;
}

VOID x_dm_dlna_deregister_notify(x_sktdev_st_nfy_fct pf_sktdev_st_nfy, VOID*  pv_tag)
{
    BOOL                b_found;
    UINT16              ui2_i;

    b_found = FALSE;
    
    for (ui2_i=0; ui2_i<MAX_DLNA_NFY_NUM; ui2_i++)
    {
        if (a_nfy_list[ui2_i].b_used)
        {
            if ((a_nfy_list[ui2_i].pf_nfy == pf_sktdev_st_nfy) && (a_nfy_list[ui2_i].pv_tag == pv_tag))
            {
                b_found = TRUE;
                break;
            }
        }
    }

    if (b_found)
    {
        a_nfy_list[ui2_i].b_used = FALSE;
    }
}

extern VOID x_dlnafs_pause(VOID);
extern VOID x_dlnafs_resume(VOID);

VOID _dlna_on_off_op_delay_finished(HANDLE_T pt_tm_handle, VOID *pv_tag)
{
    /* x_dbg_stmt("jl unlock by timer\n"); */
    x_sema_unlock(h_op_lock);
}

VOID  x_dm_dlna_dmp_start(VOID)
{
    if (!b_dmp_start)
    {
        INT32 i4_ret;
        if(h_op_timer != NULL_HANDLE){
            x_timer_delete(h_op_timer);
        }
        i4_ret = x_timer_create(&h_op_timer);
        if (i4_ret != OSR_OK)
        {
            /* return; */ 
        }
        x_dlna_dmp_start(_dlna_device_nfy, NULL);
        x_dlnafs_resume();
        b_dmp_start = TRUE;
        x_sema_lock(h_op_lock, X_SEMA_OPTION_WAIT);
        /* x_dbg_stmt("locked before timer\n"); */
        x_timer_start(h_op_timer, DLNA_ON_OFF_TIME, X_TIMER_FLAG_ONCE, _dlna_on_off_op_delay_finished, NULL);
    }
}

VOID _dlna_dmr_device_nfy(DLNA_DEVICE_EVENT_T t_event, 
                              VOID * pv_tag,
                              VOID * pv_arg)
{
    _dlna_device_nfy(t_event, pv_tag, pv_arg);
}

VOID x_dm_dlna_dmp_stop(VOID)
{
    if (b_dmp_start)
    {
        /* x_dbg_stmt("jl lock before stop called\n"); */
        x_sema_lock(h_op_lock, X_SEMA_OPTION_WAIT);
        x_dlna_dmp_stop();
        x_dlnafs_pause();
        b_dmp_start = FALSE;
        x_sema_unlock(h_op_lock);
        /* x_dbg_stmt("jl unlock after stop called\n"); */
    }
}

VOID  x_dm_dlna_dmp_suspend(VOID)
{
    UINT16              ui2_i;
    
    x_dlna_dmp_suspend();
    x_dlnafs_pause();
    
    for (ui2_i=0; ui2_i<MAX_DLNA_DEV_NUM; ui2_i++)
    {
        if (a_dev_list[ui2_i].b_used)
        {
            a_dev_list[ui2_i].b_added = FALSE;
            /*send device removal event to DM*/
        }
    }

}

VOID  x_dm_dlna_dmp_resume(VOID)
{
    x_dlna_dmp_resume();
    x_dlnafs_resume();
}

VOID x_dm_dlna_dmp_refresh_dms_state(VOID)
{
    x_dm_dlna_dmp_suspend();
    x_dm_dlna_dmp_resume();
}

#endif
