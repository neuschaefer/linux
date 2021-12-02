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
/*----------------------------------------------------------------------------*/
/*! @file u_rm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Resource Manager specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_RM_H_
#define _U_RM_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_RES_MNGR
 *  @{
 */
/*----------------------------------------------------------------------------*/

#include "u_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Resource Manager API return values */
#define RMR_ARBITRATION                ((INT32)    2)    /**<need arbitration        */
#define RMR_ASYNC_NFY                  ((INT32)    1)    /**<asysn notify        */
#define RMR_OK                         ((INT32)    0)    /**<ok        */
#define RMR_NOT_INIT                   ((INT32)   -1)    /**<resource manager is not init        */
#define RMR_ALREADY_INIT               ((INT32)   -2)    /**<resource manager already inited        */
#define RMR_INV_ARG                    ((INT32)   -3)    /**<invalid argument       */
#define RMR_INV_HANDLE                 ((INT32)   -4)    /**<invalid handle        */
#define RMR_INV_NAME                   ((INT32)   -5)    /**<invalid name        */
#define RMR_OUT_OF_HANDLES             ((INT32)   -6)    /**<out of handle        */
#define RMR_NO_COMP_FOUND              ((INT32)   -7)    /**<component not found        */
#define RMR_INV_CONNECT                ((INT32)   -8)    /**<can't connect        */
#define RMR_INV_DISCONNECT             ((INT32)   -9)    /**<can't disconnect        */
#define RMR_INV_COND                   ((INT32)  -10)    /**<invalid condtion        */
#define RMR_DUPLICATE_COMP_ID          ((INT32)  -11)    /**<duplicate component id        */
#define RMR_DUPLICATE_NAME             ((INT32)  -12)    /**<duplicate group/comp name        */
#define RMR_INV_COMP_ID                ((INT32)  -13)    /**<invalid component id        */
#define RMR_INV_LIST_ENTRY             ((INT32)  -14)    /**<invalid list entry        */
#define RMR_NOT_ENOUGH_SPACE           ((INT32)  -15)    /**<not enough space        */
#define RMR_DRV_ERROR                  ((INT32)  -16)    /**<driver error        */
#define RMR_NO_RIGHTS                  ((INT32)  -17)    /**<no right for this operation        */
#define RMR_NOT_ENOUGH_MEM             ((INT32)  -18)    /**<not enough memory        */
#define RMR_OUT_OF_RESOURCES           ((INT32)  -19)    /**<out of resource        */
#define RMR_HARD_WIRED_CONFLICT        ((INT32)  -20)    /**<the component is hard wired        */
#define RMR_SUPPORT_COMP_CONFLICT      ((INT32)  -21)    /**<support component conflict        */
#define RMR_INV_PORT_NUMBER            ((INT32)  -22)    /**<invalid port number        */
#define RMR_INV_OBJ_TO_COMP_REL        ((INT32)  -23)    /**<invalid component         */
#define RMR_INV_CTRL_TYPE              ((INT32)  -24)    /**<invalid control type        */
#define RMR_PIPE_IS_CLOSING            ((INT32)  -25)    /**<the pipe is closing        */
#define RMR_SINGLE_INP_PORT_COMP       ((INT32)  -26)    /**<single input port        */
#define RMR_MULTI_INP_PORT_COMP        ((INT32)  -27)    /**<mulit input port        */
#define RMR_DUPLICATE_PORT             ((INT32)  -28)    /**<duplicate port        */
#define RMR_COMP_IS_CLOSING            ((INT32)  -29)    /**<the component is closing        */
#define RMR_COMP_NOT_CONNECTED         ((INT32)  -30)    /**<the component is not connected        */
#define RMR_HARD_WIRED                 ((INT32)  -31)    /**<the component is hard wired        */
#define RMR_CONN_CONFLICT              ((INT32)  -32)    /**<connection conflict        */
#define RMR_CONNECT_FAILED             ((INT32)  -33)    /**<connection failed        */
#define RMR_DISCONNECT_FAILED          ((INT32)  -34)    /**<disconnect failed        */
#define RMR_INV_SET                    ((INT32)  -35)    /**<invalid set operation        */
#define RMR_INV_GET                    ((INT32)  -36)    /**<invalide get operation        */
#define RMR_COMP_EXCL_NFY_SET          ((INT32)  -37)    /**<internel use        */
#define RMR_NO_GET_INFO                ((INT32)  -38)    /**<internel use        */
#define RMR_DUPLICATE_MC_NFY_FCT_TBL   ((INT32)  -39)    /**<internel use        */
#define RMR_DUPLICATE_MC_NFY_DEV_ID    ((INT32)  -40)    /**<internel use        */
#define RMR_UNKNOWN_MC_NFY_DEV_ID      ((INT32)  -41)    /**<internel use        */
#define RMR_DUPLICATE_MC_NFY_INSTANCE  ((INT32)  -40)    /**<internel use        */

/* Driver type */
typedef UINT16 DRV_TYPE_T;    /**<internel use        */

/* Operation types. */
typedef UINT32 DRV_CONN_TYPE_T;    /**<internel use        */
typedef UINT32 DRV_DISC_TYPE_T;    /**<internel use        */
typedef UINT32 DRV_GET_TYPE_T;    /**<internel use        */
typedef UINT32 DRV_SET_TYPE_T;    /**<internel use        */

/* Connect reasons */
#define RM_CONN_REASON_LAST_ENTRY  ((UINT32) 256)    /**<internel use        */

/* Disconnect reasons */
#define RM_DISC_REASON_LAST_ENTRY  ((UINT32) 256)    /**<internel use        */

/* Connection types */
#define RM_CONN_TYPE_LAST_ENTRY  ((DRV_CONN_TYPE_T) 256)    /**<internel use        */

/* Disconnect types */
#define RM_DISC_TYPE_LAST_ENTRY  ((DRV_DISC_TYPE_T) 256)    /**<internel use        */

/* Get types */
#define RM_GET_TYPE_LAST_ENTRY  ((DRV_GET_TYPE_T) 256)    /**<internel use        */

/* Set types */
#define RM_SET_TYPE_LAST_ENTRY  ((DRV_SET_TYPE_T) 256)    /**<internel use        */

/* Ports */
#define ALL_PORTS     ((UINT8) 0xff)    /**<internel use        */
#define SUPPORT_PORT  ((UINT8) 0xfe)  /* Indicates support port. */    /**<internel use       */
#define ANY_PORT_NUM  ALL_PORTS    /**<internel use        */

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_RES_MNGR */
/*----------------------------------------------------------------------------*/

#endif /* _U_RM_H */

