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
 * $RCSfile: c_rm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Resource Manager specific definitions,
 *         which are exported to applications.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "res_mngr/u_rm.h"
#include "res_mngr/x_rm.h"


/*-----------------------------------------------------------------------------
 * Name: c_rm_set_comp
 *
 * Description: This API sets information for a component without having to
 *              open the it first.
 *
 * Inputs:  e_type          Specifies the component type.
 *          ui2_id          Contains a specific component id or ANY_COMP_ID.
 *          b_sel_out_port  Specifies if argument "ui1_port" references an
 *                          input or output port.
 *          ui1_port        Contains the input or output port number.
 *          e_set_type      Specifies the type of information which is
 *                          referenced by argument pv_set_info.
 *          pv_set_info     Contains the set information.
 *          z_set_info_len  Indicates the length of the data in the buffer
 *                          referenced by argument pv_set_info.
 *
 * Outputs: -
 *
 * Returns: RMR_OK                Routine successful.
 *          RMR_INV_ARG           One or more of the arguments contain
 *                                invalid data.
 *          RMR_NOT_ENOUGH_SPACE  Argument pv_get_info is not big enough to
 *                                hold the requested information.
 *          RMR_DRV_ERROR         A driver error occured.
 *          RMR_INV_PORT_NUMBER   Invalid port number specified.
 *          RMR_INV_SET           Unknown get operation.
 ----------------------------------------------------------------------------*/
INT32 c_rm_set_comp (DRV_TYPE_T      e_type,
                     UINT16          ui2_id,
                     BOOL            b_sel_out_port,
                     UINT8           ui1_port,
                     DRV_SET_TYPE_T  e_set_type,
                     const VOID*     pv_set_info,
                     SIZE_T          z_set_info_len)
{
    return x_rm_set_comp(e_type, ui2_id, b_sel_out_port, ui1_port,
                         e_set_type, pv_set_info, z_set_info_len);
}

/*-----------------------------------------------------------------------------
 * Name: c_rm_get_comp
 *
 * Description: This API gets information about a component without having to
 *              open the it first.
 *
 * Inputs:  e_type           Specifies the component type.
 *          ui2_id           Contains a specific component id or ANY_COMP_ID.
 *          b_sel_out_port   Specifies if argument "ui1_port" references an
 *                           input or output port.
 *          ui1_port         Contains the input or output port number.
 *          e_get_type       Specifies the type of information which shall be
 *                           returned in the buffer, referenced by argument
 *                           pv_get_info.
 *          pz_get_info_len  Contains the length of the buffer which is
 *                           referenced by argument pv_get_info.
 *
 * Outputs: pv_get_info      Contains the retrieved information.
 *          pz_get_info_len  Indicates the length of the valid data in the
 *                           buffer referenced by argument pv_get_info.
 *
 * Returns: RMR_OK                Routine successful.
 *          RMR_INV_ARG           One or more of the arguments contain
 *                                invalid data.
 *          RMR_NOT_ENOUGH_SPACE  Argument pv_get_info is not big enough to
 *                                hold the requested information.
 *          RMR_DRV_ERROR         A driver error occured.
 *          RMR_INV_PORT_NUMBER   Invalid port number specified.
 *          RMR_INV_GET           Unknown get operation.
 ----------------------------------------------------------------------------*/
INT32 c_rm_get_comp (DRV_TYPE_T      e_type,
                     UINT16          ui2_id,
                     BOOL            b_sel_out_port,
                     UINT8           ui1_port,
                     DRV_GET_TYPE_T  e_get_type,
                     VOID*           pv_get_info,
                     SIZE_T*         pz_get_info_len)
{
    return x_rm_get_comp(e_type, ui2_id, b_sel_out_port, ui1_port,
                         e_get_type, pv_get_info, pz_get_info_len);
}
