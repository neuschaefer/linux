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
#include "dm/c_dm_smb.h"
#include "inet/x_net_smb.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_socket_dev.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
typedef VOID (*c_sktdev_st_nfy_fct)(
    VOID              *pv_nfy_tag,          /*!< private data set from middleware */
    SKTDEV_STATUS_T   e_nfy_st,             /*!< device status */
    VOID              *pv_data              /*!< notification data sent from socket device */
);

/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/
#ifdef _NET_MEMLEAK_DEBUG
#include "inet/memleak/net_memleak.h"
#endif

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
extern VOID x_dm_smb_release_device(UINT32 ui4_id);
extern VOID* x_dm_smb_get_device(UINT32 ui4_id);
extern VOID x_dm_smb_init(VOID);
extern INT32 x_dm_smb_register_notify(x_sktdev_st_nfy_fct pf_sktdev_st_nfy, VOID*  pv_tag);
extern VOID x_dm_smb_deregister_notify(x_sktdev_st_nfy_fct pf_sktdev_st_nfy, VOID*  pv_tag);
extern VOID  x_dm_smb_start(VOID);
extern VOID x_dm_smb_stop(VOID);
extern VOID x_dm_smb_refresh (VOID);
extern VOID  x_dm_smb_suspend(VOID);

VOID c_dm_smb_release_device(UINT32 ui4_id)
{
    x_dm_smb_release_device(ui4_id);
}

VOID* c_dm_smb_get_device(UINT32 ui4_id)
{
    return x_dm_smb_get_device(ui4_id);
}

VOID c_dm_smb_init(VOID)
{
    x_dm_smb_init();
}

INT32 c_dm_smb_register_notify(c_sktdev_st_nfy_fct pf_sktdev_st_nfy0, VOID*  pv_tag)
{
    return x_dm_smb_register_notify(pf_sktdev_st_nfy0, pv_tag);
}

VOID c_dm_smb_deregister_notify(c_sktdev_st_nfy_fct pf_sktdev_st_nfy0, VOID*  pv_tag)
{
    x_dm_smb_deregister_notify(pf_sktdev_st_nfy0, pv_tag);
}

VOID c_dm_smb_start(VOID)
{
    x_dm_smb_start();
}

VOID c_dm_smb_refresh (VOID)
{
    x_dm_smb_refresh ();
}

VOID c_dm_smb_stop(VOID)
{
    x_dm_smb_stop();
}

VOID c_dm_smb_suspend(VOID)
{
    x_dm_smb_suspend();
}

#endif

