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
 * $RCSfile: dt_rtc.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contain implementation of Date Time library API's for
 *         interfacing with the sytem Real Time Clock.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_rtc.h"
#include "dbg/x_dbg.h"
#include "x_dt.h"
#include "_dt.h"


static HANDLE_T    h_rtc=NULL_HANDLE;

/*--------------------------------------------------------------------------
 * Name: dt_rtc_set_time
 *
 * Description - Set the current time on the RTC component.
 *
 * Input  - t_sec:  current system time as the number of seconds
 *                  since 00:00:00Z, Jan 1, 1970.
 *
 * Return - DTR_OK:  system current time is set.
 *          DTR_FAIL: Can not set current time on the RTC component.
 *          DTR_NO_RTC: No RTC component is registered with resource
 *                      manager.
 --------------------------------------------------------------------------*/
INT32 dt_rtc_set_time(TIME_T t_sec)
{
    INT32              i4_rc;
    BOOL               b_sel_out_port = FALSE;
    UINT8              ui1_port       = ANY_PORT_NUM ;
    DRV_SET_TYPE_T     e_set_type     = RTC_SET_TYPE_TIME_DATE;
    const VOID*        pv_set_info;
    SIZE_T             z_set_info_len;
    RM_COND_T          e_rm_cond;
    
    if ( h_rtc != NULL_HANDLE )
    {
        pv_set_info = &t_sec;
        z_set_info_len=sizeof(TIME_T);
        
        i4_rc=rm_set(h_rtc,
                     b_sel_out_port,
                     ui1_port,
                     e_set_type,
                     pv_set_info,
                     z_set_info_len,
                     &e_rm_cond);             
        if ( i4_rc == RMR_OK )
        {
            i4_rc = DTR_OK;
        }
        else
        {
            i4_rc = DTR_FAIL;
        }
    }
    else
    {
        i4_rc=DTR_NO_RTC;
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dt_rtc_set_sync_time
 *
 * Description - Set the sync time on the RTC component.
 *
 * Input  - t_sec:  current system time as the number of seconds
 *                  since 00:00:00Z, Jan 1, 1970.
 *
 * Return - DTR_OK:  system current time is set.
 *          DTR_FAIL: Can not set current time on the RTC component.
 *          DTR_NO_RTC: No RTC component is registered with resource
 *                      manager.
 --------------------------------------------------------------------------*/
VOID dt_rtc_set_sync_time(TIME_T t_sec, UINT32 ui4_seq)
{
    BOOL               b_sel_out_port = FALSE;
    UINT8              ui1_port       = ANY_PORT_NUM ;
    DRV_SET_TYPE_T     e_set_type     = RTC_SET_TYPE_SYNC_TIME_DATE;
    const VOID*        pv_set_info;
    SIZE_T             z_set_info_len;
    RM_COND_T          e_rm_cond;
    RTC_SYNC_TIME_DATE_T t_sync_time_date;


    if ( h_rtc != NULL_HANDLE )
    {
        t_sync_time_date.t_time_date    = t_sec;
        t_sync_time_date.ui4_ch_seq_num = ui4_seq;

        pv_set_info = &t_sync_time_date;
        z_set_info_len=sizeof(RTC_SYNC_TIME_DATE_T);

        rm_set(h_rtc,
               b_sel_out_port,
               ui1_port,
               e_set_type,
               pv_set_info,
               z_set_info_len,
               &e_rm_cond);
    }
}

/*--------------------------------------------------------------------------
 * Name: dt_rtc_get_time
 *
 * Description - Get the current time stored in RTC component.
 *
 * Output - pt_sec:  current system time as the number of seconds
 *                   since 00:00:00Z, Jan 1, 1970.  If RTC return
 *                   an error code, then pt_sec is set to 0 seconds.
 *
 * Return - DTR_OK:  system current time is returned.
 *          DTR_FAIL: Can not obtain the info on RTC component.
 *          DTR_NO_RTC: No RTC component is registered with resource
 *                      manager.
 --------------------------------------------------------------------------*/
INT32 dt_rtc_get_time(TIME_T* pt_sec)
{
    INT32              i4_rc;
    BOOL               b_sel_out_port = FALSE;
    UINT8              ui1_port       = ANY_PORT_NUM ;
    DRV_SET_TYPE_T     e_get_type     = RTC_GET_TYPE_TIME_DATE;
    VOID*              pv_get_info;
    SIZE_T             z_get_info_len;
    RM_COND_T          e_rm_cond;

    BOOL               b_rtc_status;

    pv_get_info = pt_sec ;
    z_get_info_len = sizeof(TIME_T);
    
    if ( h_rtc != NULL_HANDLE )
    {
        /*
          Check the status of RTC component.
        */
        i4_rc=dt_rtc_status(&b_rtc_status);

        if ( i4_rc == DTR_OK     &&
             b_rtc_status == TRUE )
        {
            i4_rc=rm_get(h_rtc,
                         b_sel_out_port,
                         ui1_port,
                         e_get_type,
                         pv_get_info,
                         &z_get_info_len,
                         &e_rm_cond);

            if ( i4_rc == RMR_OK )
            {
                i4_rc = DTR_OK;
            }
            else
            {
                i4_rc = DTR_FAIL;
                *pt_sec = NULL_TIME;
            }
        }
        else
        {   /*
              If RTC does not contain valid date and time
              value, we just return NULL time.
            */
            *pt_sec=NULL_TIME;
        }
    }
    else
    {
        /*
           If no RTC component, we just return NULL time.
        */
        *pt_sec=NULL_TIME;
        i4_rc=DTR_NO_RTC;
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dt_rtc_status
 *
 * Description - Get the state of the RTC component.
 *
 * Output - pb_rtc_state:  State of RTC component.
 *                         TRUE: RTC contains valid date and time value.
 *                         FALSE: RTC does not currently contain a valid
 *                                date and time value.
 *
 * Return - DTR_OK:  RTC state is returned.
 *          DTR_FAIL: Can not obtain the info on RTC component.
 *          DTR_NO_RTC: No RTC component is registered with resource
 *                      manager.
 --------------------------------------------------------------------------*/
INT32 dt_rtc_status(BOOL* pb_rtc_status)
{
    INT32              i4_rc;
    BOOL               b_sel_out_port = FALSE;
    UINT8              ui1_port       = ANY_PORT_NUM ;
    DRV_SET_TYPE_T     e_get_type     = RTC_GET_TYPE_STATE;
    VOID*              pv_get_info;
    SIZE_T             z_get_info_len;
    RM_COND_T          e_rm_cond;
    
    pv_get_info = pb_rtc_status ;
    z_get_info_len = sizeof(BOOL);
    
    if ( h_rtc != NULL_HANDLE )
    {
        i4_rc=rm_get(h_rtc,
                     b_sel_out_port,
                     ui1_port,
                     e_get_type,
                     pv_get_info,
                     &z_get_info_len,
                     &e_rm_cond);

        if ( i4_rc == RMR_OK )
        {
            i4_rc = DTR_OK;
        }
        else
        {
            i4_rc = DTR_FAIL;
        }
    }
    else
    {
        i4_rc=DTR_NO_RTC;
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dt_rtc_handle_nfy_fct
 *
 * Description - Handle notify function for the RTC component handle. This
 *               function is called by the Resource Manager when the RTC 
 *               component's state or condition changed.
 *
 * Input  - h_obj:  handle to the RTC component
 *          ui1_port:  Not used by the RTC component. 
 *          e_nfy_cond:  Condition of RTC component 
 *          pv_tag:    Tag pass into Resource Manager when opening
 *                     this RTC component.
 *          ui4_data:  Private data, not used by the RTC component.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
static 
VOID dt_rtc_handle_nfy_fct 
(
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data
)
{
    switch (e_nfy_cond)
    {
        case RM_COND_IGNORE:
            break;
            
        case RM_COND_CLOSED:
        case RM_COND_CLOSING:
        {
            /* Set the RTC handle to NULL */
            h_rtc = NULL_HANDLE;
        }
        break;

        default:
            break;
    }
    return ;
}

/*--------------------------------------------------------------------------
 * Name: dt_rtc_get_handle
 *
 * Description - Get handle to the RTC component from the Resource Mananger.
 *
 * Output - None.
 *
 * Return - DTR_OK:  system current time is returned.
 *          DTR_FAIL: No RTC component is registered with resource
 *                    manager.
 --------------------------------------------------------------------------*/
INT32 dt_rtc_get_handle(VOID)
{
    INT32              i4_rc;
    OBJ_TO_COMP_REL_T  e_obj_to_comp_rel = OBJ_COMP_REL_IGNORE;
    DRV_TYPE_T         e_type            = DRVT_RTC;
    UINT16             ui2_id            = ANY_COMP_ID;
    const CHAR*        ps_name           = NULL;
    CTRL_TYPE_T        e_ctrl            = CTRL_TYPE_SHARED;
    VOID*              pv_comp_sel_data  = NULL;
    rm_comp_sel_fct    pf_comp_sel       = NULL;
    VOID*              pv_tag            = NULL;
    rm_nfy_fct         pf_nfy            = dt_rtc_handle_nfy_fct;
    RM_COND_T          e_rm_cond;
        
    if ( h_rtc == NULL_HANDLE )
    {
        i4_rc=rm_open_comp(NULL_HANDLE,
                           e_obj_to_comp_rel,
                           e_type,
                           ui2_id,
                           ps_name,
                           e_ctrl,
                           pv_comp_sel_data,
                           pf_comp_sel,
                           pv_tag,
                           pf_nfy,
                           &h_rtc,
                           &e_rm_cond);
        if ( i4_rc == RMR_OK )
        {
            x_dbg_stmt("{DT} Real Time Clock is available on this system.\n");
            
            i4_rc = DTR_OK;
        }
        else
        {
            i4_rc = DTR_NO_RTC;
        }
    }
    else
    {
        /* handle to RTC is already initialized. */
        i4_rc=DTR_OK;
    }
    return i4_rc;
}





