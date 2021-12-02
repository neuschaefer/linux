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
 * $RCSfile: dm_smb.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 34076dbf72c1e756a741024f89fac47f $
 *
 * Description: 
 *         This file contains SMB DM exported API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifdef SMB_SUPPORT

#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "inc/common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "libc/ctype.h"
#include "util/x_lnk_list.h"
#include "dm/x_dm.h"
#include "inet/x_net_smb.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_socket_dev.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define MAX_SMB_DEV_NUM                256
#define MAX_SMB_NFY_NUM                10

typedef struct _DM_SMB_CALLBACK_T DM_SMB_CALLBACK_T;
typedef struct _DM_SMB_DEVICE_T DM_SMB_DEVICE_T;

struct _DM_SMB_CALLBACK_T
{
    BOOL                b_used;
    x_sktdev_st_nfy_fct pf_nfy;
    VOID*               pv_tag;
};


struct _DM_SMB_DEVICE_T
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
static BOOL b_smb_start = FALSE;
static BOOL b_smb_init = FALSE;

static DM_SMB_DEVICE_T     a_dev_list[MAX_SMB_DEV_NUM];
static DM_SMB_CALLBACK_T   a_nfy_list[MAX_SMB_NFY_NUM];

static SKTDEV_DEV_INFO_T    t_sktdev_info;

#ifdef _NET_MEMLEAK_DEBUG
#include "inet/memleak/net_memleak.h"
#endif

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static VOID _smb_device_nfy (NET_SMB_DEVICE_EVENT_T t_event, const CHAR* ps_srv, VOID * pv_tag)
{
    BOOL                b_found;
    UINT16              ui2_i;

    /*printf("[FakeDM/SMB]_smb_device_nfy: general event:0x%x pv_tag:0x%x srv:%s\n", 
                          t_event,
                          pv_tag,
                          ps_srv);*/

    /* DMR is treated as a DMS in smb file system, only difference is they operations,
     * the differences between them are handled in smb file system.
     * Here, DMR also holds a device file member as DMS. DM and FM don't know they are different type of file.*/
    if (NET_SMB_DEVICE_EVENT_ADD == t_event)
    {
        b_found = FALSE;
        
        for (ui2_i=0; ui2_i<MAX_SMB_DEV_NUM; ui2_i++)
        {
            if (a_dev_list[ui2_i].b_used)
            {
                if (x_strcmp (a_dev_list[ui2_i].pv_dev, ps_srv) == 0)
                {
                    b_found = TRUE;
                    break;
                }
            }
        }
        
        if (b_found)
        {
            
            /*printf("[FakeDM/SMB]%s #%d return\n",__FUNCTION__,__LINE__); */
            return;
        }
        
        b_found = FALSE;
        for (ui2_i=0; ui2_i<MAX_SMB_DEV_NUM; ui2_i++)
        {
            if (!a_dev_list[ui2_i].b_used)
            {
                a_dev_list[ui2_i].b_used = TRUE;
                a_dev_list[ui2_i].b_added = TRUE;
                if ((a_dev_list[ui2_i].pv_dev = x_strdup (ps_srv)) == NULL)
                {
                    continue;
                }
                /*printf ("[SMB.dm] %s, %s.%d, x_strdup 0x%08x\n", __FUNCTION__, __FILE__, __LINE__, a_dev_list[ui2_i].pv_dev);*/
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

            t_sktdev_info.u_info.rSupDev.u4DevType = DRVT_SMB;
            t_sktdev_info.u_info.rSupDev.u4CompId = ui2_i;
            t_sktdev_info.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;

            for (ui2_i=0; ui2_i<MAX_SMB_NFY_NUM; ui2_i++)
            {
                if (a_nfy_list[ui2_i].b_used)
                {
                    a_nfy_list[ui2_i].pf_nfy(a_nfy_list[ui2_i].pv_tag, SKTDEV_STATUS_DEV_ATTACH, &t_sktdev_info);
                }
            }
        }

    }
    else if (NET_SMB_DEVICE_EVENT_REMOVE == t_event)
    {
        INT32 i4_del_index = -1;

        b_found = FALSE;
        
        for (ui2_i=0; ui2_i<MAX_SMB_DEV_NUM; ui2_i++)
        {
            if (a_dev_list[ui2_i].b_used)
            {
                if (x_strcmp (a_dev_list[ui2_i].pv_dev, ps_srv) == 0)
                {
                    a_dev_list[ui2_i].b_added = FALSE;
                    b_found = TRUE;
                    i4_del_index = (INT32)ui2_i;

                    break;
                }
            }
        }

        if (b_found&&(i4_del_index!=-1))
        {
            /*send device removal event to DM*/
            t_sktdev_info.eDevSt = SKTDEV_STATUS_DEV_DETACH;
            t_sktdev_info.u4SocketNo = ui2_i;

            t_sktdev_info.u_info.rSupDev.u4DevType = DRVT_SMB;
            t_sktdev_info.u_info.rSupDev.u4CompId = ui2_i;
            t_sktdev_info.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;

            for (ui2_i=0; ui2_i<MAX_SMB_NFY_NUM; ui2_i++)
            {
                if (a_nfy_list[ui2_i].b_used)
                {
                    /*printf ("[SMB.DM] call dm callback remove server: %s\n", ps_srv);*/
                    a_nfy_list[ui2_i].pf_nfy(a_nfy_list[ui2_i].pv_tag, SKTDEV_STATUS_DEV_DETACH, &t_sktdev_info);
                }
            }
            /* call smb umount and may clean cahce */
            x_net_smb_umount_server (a_dev_list[i4_del_index].pv_dev);
            /*printf ("[SMB.dm] %s, %s.%d, x_mem_free 0x%08x\n", __FUNCTION__, __FILE__, __LINE__, a_dev_list[i4_del_index].pv_dev);*/
            x_mem_free (a_dev_list[i4_del_index].pv_dev);
            a_dev_list[i4_del_index].pv_dev = NULL;
            a_dev_list[i4_del_index].b_used = FALSE;
        }
    }
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
VOID x_dm_smb_release_device(UINT32 ui4_id)
{
    /* comment by jinlong. before program goes here, the device have been set to be unused
     * in the device notify caller thread context.
     * This function will be called async, this will influence a new mount operation.*/
    /* a_dev_list[ui4_id].b_used = FALSE; */
}

VOID* x_dm_smb_get_device(UINT32 ui4_id)
{
    if (ui4_id > MAX_SMB_DEV_NUM -1)
    {
        return NULL;
    }

    if (a_dev_list[ui4_id].b_used && a_dev_list[ui4_id].b_added)
    {
        return a_dev_list[ui4_id].pv_dev;
    }
    else
    {
        return NULL;
    }
}

VOID x_dm_smb_init(VOID)
{
    if (!b_smb_init)
    {
        x_memset(a_dev_list, 0, MAX_SMB_DEV_NUM*sizeof(DM_SMB_DEVICE_T));
        x_memset(a_nfy_list, 0, MAX_SMB_NFY_NUM*sizeof(DM_SMB_CALLBACK_T));
        x_memset(&t_sktdev_info, 0, sizeof(SKTDEV_DEV_INFO_T));
        
        b_smb_init = TRUE;
    }
}

INT32 x_dm_smb_register_notify(x_sktdev_st_nfy_fct pf_sktdev_st_nfy, VOID*  pv_tag)
{
    BOOL                b_found;
    UINT16              ui2_i;
    
    b_found = FALSE;

    for (ui2_i=0; ui2_i<MAX_SMB_NFY_NUM; ui2_i++)
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
        return -1;
    }

    b_found = FALSE;
    for (ui2_i=0; ui2_i<MAX_SMB_NFY_NUM; ui2_i++)
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
        return -2;
    }
    
    return 0;
}

VOID x_dm_smb_deregister_notify(x_sktdev_st_nfy_fct pf_sktdev_st_nfy, VOID*  pv_tag)
{
    BOOL                b_found;
    UINT16              ui2_i;

    b_found = FALSE;
    
    for (ui2_i=0; ui2_i<MAX_SMB_NFY_NUM; ui2_i++)
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

    if (b_found&&(ui2_i<MAX_SMB_NFY_NUM))
    {
        a_nfy_list[ui2_i].b_used = FALSE;
    }
}

VOID  x_dm_smb_start(VOID)
{
    if (!b_smb_start)
    {
        if (x_net_smb_start (_smb_device_nfy) != 0)
        {
            return;
        }
        b_smb_start = TRUE;
    }
}

VOID x_dm_smb_stop(VOID)
{
    if (b_smb_start)
    {
        x_net_smb_stop();
        b_smb_start = FALSE;
    }
}

VOID x_dm_smb_refresh (VOID)
{
    x_net_smb_refresh ();
}

VOID  x_dm_smb_suspend(VOID)
{
    UINT16              ui2_i;
    
    for (ui2_i=0; ui2_i<MAX_SMB_DEV_NUM; ui2_i++)
    {
        if (a_dev_list[ui2_i].b_used)
        {
            a_dev_list[ui2_i].b_added = FALSE;
            /*send device removal event to DM*/
        }
    }

}

#endif

