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
 * $RCSfile: dsm_translate.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains API's for tanslating device driver condition,
 *         error code to DSM conditions and error code.
 *
 *---------------------------------------------------------------------------*/

#include "dsm/_dsm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"

/*--------------------------------------------------------

static internal array variables holding the tuner information.
  
----------------------------------------------------------*/


/*--------------------------------------------------------------------------
 * Name: dsm_rm_to_dsm_err_code
 *
 * Description - API to convert RM return code to DSM return code.
 *
 * Input  -
 *   i4_err:   RM return code..
 *
 * Output -
 *   None
 *
 * Return - 
 *   DSM return code.
------------------------------------------------------------------------- */
INT32 dsm_rm_to_dsm_ret_code(INT32  i4_err)
{
    INT32     i4_rc;

    switch(i4_err)
    {
        case RMR_ASYNC_NFY:
        {
            i4_rc=DSMR_ASYNC_NFY;
        }
        break;
        
        case RMR_ARBITRATION:
        case RMR_OK:
        {
            i4_rc=DSMR_OK;
        }
        break;

        case RMR_INV_ARG:
        {
            i4_rc=DSMR_INV_ARG;
        }
        break;

        case RMR_INV_GET:
        {
            i4_rc=DSMR_INV_GET_OP;
        }
        break;

        case RMR_NOT_ENOUGH_SPACE:
        {
            i4_rc=DSMR_NOT_ENOUGH_SPACE;
        }
        break;

        default:
            i4_rc=DSMR_FAIL;
    }       
    
    return     i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: dsm_rm_cond_code
 *
 * Description: This API prints out the RM handle condition.
 *
 * Inputs: 
 *   e_nfy_cond  Contains the notify condition.
 *          
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID dsm_rm_cond_code
(
    HANDLE_T       h_hdle,
    RM_COND_T      e_nfy_cond
)
{
    
#if 0
    
    RM_COND_T      e_rm_cond;

    x_dbg_stmt("{DSM} handle: %d rm cond: [%x] ", h_hdle, e_nfy_cond);
        
    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE);
    switch  (e_rm_cond)
    {
        case RM_COND_OPENING:
        {
            x_dbg_stmt(" opening ");
        }
        break;

        case RM_COND_OPENED:
        {
            x_dbg_stmt(" opened ");
        }
        break;

        case RM_COND_CLOSING:
        {
            x_dbg_stmt(" closing ");
        }
        break;

        case RM_COND_CLOSED:
        {
            x_dbg_stmt(" closed ");
        }
    }
    
    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_CONN_STATE);
    switch (e_rm_cond)
    {
        case RM_COND_CONNECTING:
        {
            x_dbg_stmt(" connecting ");
        }
        break;

        case RM_COND_CONNECTED:
        {
            x_dbg_stmt(" connected ");
        }
        break;

        case RM_COND_DISCONNECTING:
        {
            x_dbg_stmt(" disconnecting ");
        }
        break;

        case RM_COND_DISCONNECTED:
        {
            x_dbg_stmt(" disconnected ");
        }
        break;
    }
    
    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_SHARE_STATE);
    switch (e_rm_cond)
    {
        case RM_COND_MASTER:
        {
            x_dbg_stmt(" master ");
        }
        break;

        case RM_COND_SLAVE:
        {
            x_dbg_stmt(" slave ");
        }
        break;

        case RM_COND_SHARED:
        {
            x_dbg_stmt(" shared ");
        }
        break;
    }

    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE,
                                 COND_GRP_REASON);
    switch (e_rm_cond)
    {
        case RM_COND_REASON_RIGHTS:
        {
            x_dbg_stmt(" reason rights ");
        }
        break;

        case RM_COND_REASON_MULTI_COMPS:
        {
            x_dbg_stmt(" reason multi comp ");
        }
        break;

        case RM_COND_REASON_CONF_CTRL_MODE:
        {
            x_dbg_stmt(" reason conflict control mode ");
        }
        break;

        case RM_COND_REASON_CONF_COMP:
        {
            x_dbg_stmt(" reason conflict component ");
        }
        break;

        case RM_COND_REASON_ARBITRATION:
        {
            x_dbg_stmt(" reason arbitration. ");
        }
        break;
    }
    

    e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_CONN_ORIG);

    switch (e_rm_cond)
    {
        case RM_COND_CONN_ORIGINATOR:
        {
            x_dbg_stmt(" conn originator ");
        }
        break;

        case RM_COND_CONN_PENDING:
        {
            x_dbg_stmt(" conn pending ");
        }
        break;
    }

    x_dbg_stmt("\n");
    
#endif
    
    return;
}

