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
 * $RCSfile: dsm_avc_group.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains DSM functions for Audio-Video Connector
 *         (AVC) group. 
 *
 *---------------------------------------------------------------------------*/

#include "handle/x_handle.h"
#include "dsm/_dsm.h"


/*--------------------------------------------------------------------------
 * Name: dsm_avc_group_get_ops
 *
 * Description: This API executed 'GET' operation for 
 *              DSM AVC group device.
 *
 * Input -
 *    pt_dsm_obj:     Specifies reference to a DSM component structure.
 *    e_get_type:     Specifies the type of get operation for which the
 *                    data is returned by the avc driver.
 *    pz_size         Contains the length of buffer which is referenced
 *                    by pv_get_info.
 *
 * Output -
 *    pv_get_info:    Pointer to a structure containing informatio
 *    pz_size         Contains the length of the valid data in the
 *                    buffer referenced by the 'pv_get_info' argument.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32 dsm_avc_group_get_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    INT32            i4_rc;
    DEVICE_TYPE_T    t_dev_type;
    UINT16           ui2_num_comps;
    DRV_TYPE_T       e_type;

    /*
      check input arguments.
    */
    if ( pv_get_info == NULL   ||
         pz_size     == 0      ||
         pv_data     == NULL )
    {
        return DSMR_INV_ARG;
    }
         
    switch ( e_get_type )
    {
        case DSM_GET_AVC_NUM_OF_DEV:
        {
            t_dev_type = (DEVICE_TYPE_T) pv_get_info ;

            /*
              convert the device type to driver type.
            */
            if ( dsm_avc_map_dev_type_to_drv_type(t_dev_type, &e_type)
                 == DSMR_OK )
            {
                /*
                  check the size of return buffer, it should be a ref to
                  an UINT16 variable.
                */
                if ( *pz_size == 2 )
                {
                    ui2_num_comps = 0 ;
                    
                    i4_rc = x_rm_get_num_comps_of_type_from_name(
                        NULL, e_type, &ui2_num_comps);
                    
                    if ( i4_rc == RMR_OK )
                    {
                        *((UINT16*)pv_data) = ui2_num_comps;
                    }
                
                    i4_rc = dsm_rm_to_dsm_ret_code(i4_rc);
                }
                else
                {
                    i4_rc = DSMR_INV_ARG;
                }
            }
            else
            {
                 i4_rc = DSMR_INV_GET_INFO;
            }
        }
        break;

        default:
        {
            i4_rc = DSMR_INV_GET_OP;
        }
        break;
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_group_set_ops
 *
 * Description: This API executed 'SET' operation on DSM AVC group.
 *
 * Input -
 *    pt_dsm_obj:     Specifies reference to a DSM component structure.
 *    e_get_type:     Specifies the type of get operation for which the
 *                    data is returned by the avc driver.
 *    pz_size         Contains the length of buffer which is referenced
 *                    by pv_get_info.
 *
 * Output -
 *    pv_get_info:    Pointer to a structure containing informatio
 *    pz_size         Contains the length of the valid data in the
 *                    buffer referenced by the 'pv_get_info' argument.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32 dsm_avc_group_set_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_set_info_len
)
{
    INT32            i4_rc;
    
    switch ( e_set_type )
    {
        default:
        {
            i4_rc = DSMR_INV_ARG;
        }
        break;
    }
    return i4_rc;
}
