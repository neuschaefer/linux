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
 * $RCSfile: rtc_drv.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 68ef117a76c31addde11229138dd95b7 $
 *
 * Description: 
 *         This file contains MTK RTC driver API implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
/*
  MTK Middleware include files
*/
#include "x_common.h"
#include "x_start.h"
#include "x_dbg.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_rtc.h"

/*
   Real Time Clock header files.
*/
#include "rtc_drv.h"
#include "x_rtc_drv.h"

#define RTC_NAME			"rtc"

static
INT32 rtc_get_fct 
(
    DRV_COMP_ID_T*        pt_comp_id,
    DRV_GET_TYPE_T        e_get_type,
    VOID*                 pv_get_info,
    SIZE_T*               pz_get_info_len
)
{
	INT32       i4_rc;
	TIME_T      t_time;
               
	switch (e_get_type)
	{
        case RTC_GET_TYPE_TIME_DATE:
        {
            if ( pv_get_info == NULL || (*pz_get_info_len) < sizeof(TIME_T) )
            {
                *pz_get_info_len = sizeof (TIME_T);
                i4_rc = RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                if ( rtc_get_time(&t_time) == TRUE )
                {
                    *((TIME_T*)pv_get_info) = t_time;
                    i4_rc = RMR_OK;
                }
                else
                {
                    *((TIME_T*)pv_get_info) = 0;
                    i4_rc = RMR_DRV_NO_GET_INFO;
                }
            }
        }
		break;

        case RTC_GET_TYPE_STATE:
        {
            if ( pv_get_info == NULL || (*pz_get_info_len) < sizeof(BOOL) )
            {
                *pz_get_info_len = sizeof (BOOL);
                i4_rc = RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *((BOOL*)pv_get_info) = TRUE;
                i4_rc = RMR_OK;
            }
        }
        break;

	    default:
        {
            i4_rc = RMR_DRV_INV_GET_INFO;
        }
        break;
	}
	return i4_rc;
}

static
INT32 rtc_set_fct
(
    DRV_COMP_ID_T*    pt_comp_id,
    DRV_SET_TYPE_T    e_set_type,
    const VOID*       pv_set_info,
    SIZE_T            z_set_info_len
)
{
	INT32        i4_rc;
	UINT32       t_time;

	switch (e_set_type)
	{
        case RTC_SET_TYPE_TIME_DATE:
        {            
            
            t_time = (*(TIME_T*)pv_set_info);
            
            if ( rtc_set_time(t_time) == TRUE )
            {
                i4_rc = RMR_OK;
            }
            else
            {
                i4_rc = RMR_DRV_INV_SET_INFO;
            }
        }
		break;

        default:
        {
            i4_rc = RMR_DRV_INV_SET_INFO;
        }
		break;
	}
    return i4_rc;
}

INT32 RTCInit(void)
{
	INT32               i4_rc;
	DRV_COMP_REG_T      t_comp_id;		
	DRV_COMP_FCT_TBL_T  t_fct_tbl;	    
    UINT8               ui1_num_inp_ports;
    UINT8               ui1_num_out_ports;
    UINT32              ui4_comp_flag;
    SIZE_T              z_comp_data_len;
    VOID*               pv_comp_data;

	/* Setup component register ID */
	t_comp_id.e_type           = DRVT_RTC;
	t_comp_id.e_id_type        = ID_TYPE_IND;	
	t_comp_id.u.t_ind.ui2_id   = RTC_ID; 
	t_comp_id.u.t_ind.pv_tag   = NULL;
    t_comp_id.u.t_ind.ui1_port = 0;

    /* Setup the input and output port. */
    ui1_num_inp_ports = 1;
    ui1_num_out_ports = 1;

    /* Set up RTC component flag, component data, and component
       data size. */
    ui4_comp_flag = 0;
    z_comp_data_len = 0;
    pv_comp_data = NULL;
    
	/* Setup function table */
	t_fct_tbl.pf_rm_connect = NULL;
	t_fct_tbl.pf_rm_disconnect = NULL;
	t_fct_tbl.pf_rm_set = rtc_set_fct;
	t_fct_tbl.pf_rm_get = rtc_get_fct;

	/* Register RTC component to Resource Manager. */
	i4_rc = x_rm_reg_comp
        (
            &t_comp_id,
            ui1_num_inp_ports,
            ui1_num_out_ports,
            RTC_NAME,
            ui4_comp_flag,
            &t_fct_tbl,
            pv_comp_data,			
            z_comp_data_len
        );
	
	if (i4_rc == RMR_OK)
    {
        return (X_DRV_RTC_OK);
    }
    else
    {
		x_dbg_stmt("\nError (%d): fail to register RTC component!\n", i4_rc);
	}
    return (X_DRV_RTC_FAILED);
}


