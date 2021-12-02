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
 * $RCSfile: x_netdrv.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Demux driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_NETDRV_H_
#define _X_NETDRV_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"
#include "rm.h"
#include "x_rm_dev_types.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Get operations */
#define NET_GET_TYPE_CTRL           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  0))

#define NET_GET_TYPE_PROMISC        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  1))

#define NET_GET_TYPE_OWN_HWADDR     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  2))

#define NET_GET_TYPE_HWADDR_COUNT   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  3))

#define NET_GET_TYPE_HWADDR         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  4))


/* Set operations */
#define NET_SET_TYPE_CTRL          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  0)) | RM_SET_TYPE_ARG_NO_REF)

#define NET_SET_TYPE_PROMISC       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  1)) | RM_SET_TYPE_ARG_NO_REF)

#define NET_SET_TYPE_MTU           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  2)) | RM_SET_TYPE_ARG_NO_REF)

#define NET_SET_TYPE_IFADDR         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  3))

#define NET_SET_TYPE_ADD_HWADDR     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  4))

#define NET_SET_TYPE_DEL_HWADDR     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  5))

#define NET_SET_TYPE_ADD_MULTI      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  6))

#define NET_SET_TYPE_DEL_MULTI      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  7))

#define NET_SET_TYPE_XMIT_FRAME     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  8))

#define NET_SET_TYPE_UNLOCK_BUFFER  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  9))

#define NET_SET_TYPE_NFY_FCT        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  10))


/* Notify conditions */
typedef enum
{
    NET_DRV_COND_RECV = 0,
    NET_DRV_COND_RECV_LOWMARK,
    NET_DRV_COND_OVERRUN,
    NET_DRV_COND_XMIT_OK,
    NET_DRV_COND_XMIT_FAIL,
    NET_DRV_COND_HW_RESET
}   NET_DRV_COND_T;


/* Notify function */
typedef BOOL (*x_network_nfy_fct) (VOID*            pv_nfy_tag,
                                   NET_DRV_COND_T   e_nfy_cond,
                                   UINT32           ui4_data_1,
                                   UINT32           ui4_data_2);


/* Notify info setting */
typedef struct _NET_DRV_NFY_INFO_T
{
    VOID*  pv_tag;
    x_network_nfy_fct  pf_network_nfy;
}   NET_DRV_NFY_INFO_T;


typedef struct _NET_DRV_BUFFER_T
{
    UINT8       *pui1_data;
    SIZE_T      z_size;
} NET_DRV_BUFFER_T;


typedef struct _NET_DRV_XMIT_FRAME_T
{
    VOID                *pv_priv_tag;
    UINT32              ui4_count;
    NET_DRV_BUFFER_T    t_buf[3];
} NET_DRV_XMIT_FRAME_T;


typedef struct _NET_DRV_RECV_FRAME_T
{
    VOID        *pv_priv_tag;
    UINT8       *pui1_data;
    SIZE_T      z_size;
} NET_DRV_RECV_FRAME_T;


/* one IF IP address template */
typedef struct _NET_DRV_IFADDR_
{
    UINT32  ui4_ifaddr;
} NET_DRV_IFADDR_T;


/* one HW address template */
typedef struct _NET_HWADDR_
{
    UINT8   ui1_hwaddr[6];
} NET_HWADDR_T;


typedef struct _NET_DRV_HWADDR_
{
    UINT8         ui1_index;   /* index (order) in HW store */
    NET_HWADDR_T  t_hwaddr;    /* data space of "ui1_count" of  NET_HWADDR_T */
} NET_DRV_HWADDR_T;


typedef struct _NET_DRV_MULTIADDR_
{
    NET_HWADDR_T    t_lo;
    NET_HWADDR_T    t_hi;
} NET_DRV_MULTIADDR_T;


/* Unlock buffer */
typedef struct _NET_DRV_UNLOCK_BUFFER_INFO_T
{
    VOID*  pv_drv_tag;

} NET_DRV_UNLOCK_BUFFER_INFO_T;


#endif /* _X_NETDRV_H_ */
