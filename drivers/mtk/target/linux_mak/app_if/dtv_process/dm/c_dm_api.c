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
 * $RCSfile: c_dm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: $
 *
 * Description: 
 *         This file contains all the transition effect interface APIs  
 *---------------------------------------------------------------------------*/

#include "handle/handle.h"
#include "dm/x_dm.h"
#include "aee/aee.h"

/*-----------------------------------------------------------------------------
 * Name: c_dm_reg_nfy_fct
 *
 * Description: Register the event callback function.
 *
 * Input: -
 *          t_evt_cat
 *              Event type to be listened to.
 *          pf_nfy
 *              Callback function registered by event listener.
 *          pt_filtr
 *              Event filter set by event listener.
 *          pv_tag
 *              Proprietary information set by event listener.  The same
 *              information will be called back to event listener when
 *              event issued.
 *          ui4_stat_ns
 *              Array length of pt_stat allocated by event listener.
 *
 * Output: -
 *          ph_nfy
 *              Handle for the event registration.  This handle is used when
 *              event listener wants to remove the registration.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 *          DMR_HANDLE
 *              Handle acquisition error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_reg_nfy_fct(
    DM_EVT_CAT_T    t_evt_cat,
    x_dm_nfy_fct    pf_nfy,      
    DM_EVT_FILTR_T  *pt_filtr,
    VOID            *pv_tag, 
    HANDLE_T        *ph_nfy)
{
    HANDLE_T  h_aux = NULL_HANDLE;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return DMR_HANDLE;
    }

    i4_ret = x_dm_reg_nfy_fct(t_evt_cat,
                              pf_nfy, 
                              pt_filtr,
                              pv_tag,
                              ph_nfy);
                                  
    if (i4_ret == DMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_nfy);
    }
    else
    {
        aee_release_handle_resource(1);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_unreg_nfy_fct
 *
 * Description: Unregister the event callback function.
 *
 * Input: -
 *          h_nfy
 *              Handle for the event registration that event listener removes.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_unreg_nfy_fct(
    HANDLE_T    h_nfy)
{
    return x_dm_unreg_nfy_fct(h_nfy);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_get_device_info
 *
 * Description: Retrieve the information for a specific device.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pt_inf
 *              information data structure retrieved.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_get_device_info(
    HANDLE_T        h_dev, 
    DM_DEV_INF_T    *pt_inf)
{
    return x_dm_get_device_info(h_dev, pt_inf);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_get_devices
 *
 * Description: Retrieve information for devices that meet a specific 
 *              condition.
 *
 * Input: -
 *          pt_filtr
 *              The filter data to specify the condition.
 *          ui4_dev_ns
 *              The array length of the pt_stat.
 *
 * Output: -
 *          pt_stat
 *              Array of DM_DEV_STAT_T data structure allocated by caller.
 *          pui4_dev_ns
 *              Number of returned elements in pt_stat.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_get_devices(
    DM_DEV_FILTR_T *pt_filtr, 
    DM_DEV_STAT_T *pt_stat,
    UINT32 *pui4_dev_ns)
{
    return x_dm_get_devices(pt_filtr,
                            pt_stat,
                            pui4_dev_ns);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_get_device
 *
 * Description: Retrieve information for devices that meet a specific 
 *              condition.
 *
 * Input: -
 *          pt_spec
 *              The filter data to specify the condition.
 *
 * Output: -
 *          ph_dev
 *              Pointer of the device handle.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/

INT32 c_dm_get_device(
    DM_DEV_SPEC_T *pt_spec,
    HANDLE_T *ph_dev)
{
    return x_dm_get_device(pt_spec, ph_dev);
}

extern INT32 c_dm_umount_remove(CHAR* ps_dev_name)
{
    return x_dm_umount_remove(ps_dev_name);
}


/*-----------------------------------------------------------------------------
 * Name: c_dm_query_automnt
 *
 * Description: Retrieve auto mount setting for a specific device type.
 *
 * Input: -
 *          t_hw_type
 *              The target device's hardware type.
 *          t_dev_type
 *              The target device's device type.
 *
 * Output: -
 *
 * Returns: TRUE
 *              The device type is auto mount enabled.
 *          FALSE
 *              The device type is auto mount disabled.
 ----------------------------------------------------------------------------*/
BOOL c_dm_query_automnt(
    DM_HW_TYPE_T  t_hw_type,
    DM_DEV_TYPE_T t_dev_type)
{
    return x_dm_query_automnt(t_hw_type, t_dev_type);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_spindle_control
 *
 * Description: Control spindle status for the optical disc drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_spindle_control(
    HANDLE_T h_dev, 
    DM_DEV_IO_SPIN_CTRL_T t_spin_ctrl,
    VOID *pv_tag,
    x_dm_ioc_fct pf_ioc_fct,
    HANDLE_T *ph_ioc)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return DMR_HANDLE;
    }

    i4_ret = x_dm_odd_spindle_control(h_dev, 
                                      t_spin_ctrl,
                                      pv_tag,
                                      pf_ioc_fct,
                                      ph_ioc);

    if (i4_ret == DMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_ioc);
    }
    else
    {
        aee_release_handle_resource(1);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_hdd_spindle_control
 *
 * Description: Control spindle status for the HDD.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_hdd_spindle_control(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_hdd_spindle_control(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_tray_control
 *
 * Description: Control tray status for the optical disc drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_tray_control(
    HANDLE_T h_dev, 
    DM_DEV_IO_TRAY_CTRL_T t_tray_ctrl,
    VOID *pv_tag,
    x_dm_ioc_fct pf_ioc_fct,
    HANDLE_T *ph_ioc)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return DMR_HANDLE;
    }

    i4_ret = x_dm_odd_tray_control(h_dev, 
                                 t_tray_ctrl,
                                 pv_tag,
                                 pf_ioc_fct,
                                 ph_ioc);

    if (i4_ret == DMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_ioc);
    }
    else
    {
        aee_release_handle_resource(1);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_dvd_struct
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pt_struct
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_dvd_struct(
    HANDLE_T h_dev,
    DM_DEV_IO_MED_READ_DVD_STRUCT_T *pt_struct)
{
    return x_dm_odd_read_dvd_struct(h_dev, pt_struct);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_kcd
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pt_kcd
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_kcd(
    HANDLE_T h_dev,
    DM_DEV_IO_MED_READ_KCD_T *pt_kcd)
{
    return x_dm_odd_read_kcd(h_dev, pt_kcd);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_com_report_key
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pt_key
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_com_report_key(
    HANDLE_T h_dev, 
    DM_DEV_IO_COM_REPORT_KEY_T *pt_key)
{
    return x_dm_odd_com_report_key(h_dev, pt_key);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_com_send_key
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pt_key
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_com_send_key(
    HANDLE_T h_dev, 
    DM_DEV_IO_COM_SEND_KEY_T *pt_key)
{
    return x_dm_odd_com_send_key(h_dev, pt_key);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_com_get_feature
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pt_feature
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_com_get_feature(
    HANDLE_T h_dev, 
    DM_DEV_IO_COM_GET_FEATURE_T *pt_feature)
{
    return x_dm_odd_com_get_feature(h_dev, pt_feature);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_cd
 *
 * Description: Raw LBA read operation for CD.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_cd(
    HANDLE_T h_dev, 
    DM_DEV_MED_DISC_READ_CD_T *pt_read_cd)
{
    return x_dm_odd_read_cd(h_dev, pt_read_cd);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_cd_async
 *
 * Description: Raw LBA read operation for CD.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_cd_async(
    HANDLE_T h_dev, 
    DM_DEV_MED_DISC_READ_CD_T *pt_read_cd,
    VOID *pv_tag,
    x_dm_ioc_fct pf_ioc_fct,
    HANDLE_T *ph_ioc)
{
    return x_dm_odd_read_cd_async(h_dev, 
                                  pt_read_cd,
                                  pv_tag,
                                  pf_ioc_fct,
                                  ph_ioc);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_cd_text
 *
 * Description: Raw LBA read operation for CD.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_cd_text(
    HANDLE_T h_dev, 
    DM_DEV_MED_READ_CD_TEXT_T *pt_read_txt)
{
    return x_dm_odd_read_cd_text(h_dev, pt_read_txt);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_disc_info
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_disc_info(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_read_disc_info(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_standby_timer
 *
 * Description: Retrieve current standby timer setting for optical disc drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pui4_time
 *              Returned timer setting.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_standby_timer(
    HANDLE_T h_dev, 
    UINT32 *pui4_time)
{
    return x_dm_odd_get_standby_timer(h_dev, pui4_time);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_hdd_get_standby_timer
 *
 * Description: Retrieve current standby timer setting for HDD.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pui4_time
 *              Returned timer setting.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_hdd_get_standby_timer(
    HANDLE_T h_dev, 
    UINT32 *pui4_time)
{
    return x_dm_hdd_get_standby_timer(h_dev, pui4_time);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_set_standby_timer
 *
 * Description: Set standby timer setting for optical disc drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          ui4_time
 *              Standby timer setting.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_set_standby_timer(
    HANDLE_T h_dev, 
    UINT32 ui4_time)
{
    return x_dm_odd_set_standby_timer(h_dev, ui4_time);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_hdd_set_standby_timer
 *
 * Description: Set standby timer setting for HDD.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          ui4_time
 *              Standby timer setting.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_hdd_set_standby_timer(
    HANDLE_T h_dev, 
    UINT32 ui4_time)
{
    return x_dm_hdd_set_standby_timer(h_dev, ui4_time);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_flush_cache
 *
 * Description: Flush cache for the optical disc drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_flush_cache(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_flush_cache(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_hdd_flush_cache
 *
 * Description: Flush cache for HDD.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_hdd_flush_cache(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_hdd_flush_cache(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_low_format
 *
 * Description: Erase sector 0x0 ~ 0x4000 for a optical disc.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_low_format(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_low_format(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_nand_low_format
 *
 * Description: Fully erase the target NAND flash.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_nand_low_format(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_nand_low_format(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_disc_finalize
 *
 * Description: Finalize the optical disc.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_disc_finalize(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_disc_finalize(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_disc_unfinalize
 *
 * Description: Unfinalize the optical disc.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_disc_unfinalize(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_disc_unfinalize(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_disc_upgrade
 *
 * Description: Upgrade system firmware by optical disc.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_disc_upgrade(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_disc_upgrade(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_set_io_speed
 *
 * Description: Set I/O speed for optical device drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_set_io_speed(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_set_io_speed(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_hw_reset
 *
 * Description: Reset hardware for optical device drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_hw_reset(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_hw_reset(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_release_bus
 *
 * Description: Release hardware bus for optical device drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_release_bus(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_release_bus(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_test_unit_ready
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_test_unit_ready(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_test_unit_ready(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_event_status
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_event_status(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_get_event_status(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_mechanism_status
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_mechanism_status(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_mechanism_status(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_set_book_type
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_set_book_type(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_set_book_type(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_flashless_download
 *
 * Description: Download loader firmware in flashless case.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_flashless_download(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_flashless_download(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_set_transfer_mode
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_set_transfer_mode(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_set_transfer_mode(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_buffer_capacity
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_buffer_capacity(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_read_buffer_capacity(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_do_opc
 *
 * Description: DO OPC operation for optical device drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_do_opc(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_do_opc(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_read_dvd_structure
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_read_dvd_structure(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_read_dvd_structure(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_vendor_inquiry
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_vendor_inquiry(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_vendor_inquiry(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_auto_tune
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pv_parm
 *              Parameter for the operation.
 *
 * Output: -
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_auto_tune(
    HANDLE_T h_dev, 
    VOID *pv_parm)
{
    return x_dm_odd_auto_tune(h_dev, pv_parm);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_med_nwa
 *
 * Description: Retrieve Next Writable Address for optical disc.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pui4_nwa
 *              Returned NWA.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_med_nwa(
    HANDLE_T h_dev, 
    UINT32 *pui4_nwa)
{
    return x_dm_odd_get_med_nwa(h_dev, pui4_nwa);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_med_toc_hdr
 *
 * Description: Retrieve TOC header data structure for CD medium.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pt_toc_hdr
 *              Returned TOC header data structure.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_med_toc_hdr(
    HANDLE_T h_dev, 
    DM_DEV_MED_DISC_TOC_HEADER_T *pt_toc_hdr)
{
    return x_dm_odd_get_med_toc_hdr(h_dev, pt_toc_hdr);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_med_toc_ent
 *
 * Description: Retrieve TOC entry data structure for CD medium.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          pt_toc_ent->ui1_trk_number
 *              Track number to query TOC entry
 * Output: -
 *          pt_toc_ent
 *              Returned TOC entry data structure.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_med_toc_ent(
    HANDLE_T h_dev,
    DM_DEV_MED_DISC_TOC_ENTRY_T *pt_toc_ent)
{
    return x_dm_odd_get_med_toc_ent(h_dev, pt_toc_ent);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_sess_ns
 *
 * Description: Retrieve session number on the disc.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pui4_sess_ns
 *              Returned session number.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_sess_ns(
    HANDLE_T h_dev,
    UINT32 *pui4_sess_ns)
{
    return x_dm_odd_get_sess_ns(h_dev, pui4_sess_ns);
}


/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_sess_inf
 *
 * Description: Retrieve session information.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          ui4_sess_no
 *              Target session number.
 *
 * Output: -
 *          pt_inf
 *              Returned session information.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_sess_inf(
    HANDLE_T h_dev,
    UINT32 ui4_sess_no,
    DM_DEV_MED_DISC_SESS_INFO_T *pt_inf)
{
    return x_dm_odd_get_sess_inf(h_dev, 
                                 ui4_sess_no,
                                 pt_inf);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_trk_ns
 *
 * Description: Retrieve track number on the disc.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pui4_trk_ns
 *              Returned track number.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_trk_ns(
    HANDLE_T h_dev,
    UINT32 *pui4_trk_ns)
{
    return x_dm_odd_get_trk_ns(h_dev, pui4_trk_ns);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_sess_inf
 *
 * Description: Retrieve session information.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *          ui4_trk_no
 *              Target track number.
 *
 * Output: -
 *          pt_inf
 *              Returned track information.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_trk_inf(
    HANDLE_T h_dev,
    UINT32 ui4_trk_no,
    DM_DEV_MED_DISC_TRK_INFO_T *pt_inf)
{
    return x_dm_odd_get_trk_inf(h_dev, 
                                ui4_trk_no,
                                pt_inf);
}


/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_sector_sz
 *
 * Description: Retrieve size of a specific LBA.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          ui4_lba
 *              LBA to get sector size.
 *          pt_sz
 *              Returned sector size.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_sector_sz(
    HANDLE_T h_dev,
    UINT32 ui4_lba,
    DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T *pt_sz)
{
    return x_dm_odd_get_sector_sz(h_dev, 
                                  ui4_lba,
                                  pt_sz);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_last_sense_key
 *
 * Description:
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pui4_key
 *
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_last_sense_key(
    HANDLE_T h_dev, 
    UINT32 *pui4_key)
{
    return x_dm_odd_get_last_sense_key(h_dev, pui4_key);
}

/*-----------------------------------------------------------------------------
 * Name: c_dm_odd_get_current_speed
 *
 * Description: Retrieve current speed of optical disc drive.
 *
 * Input: -
 *          h_dev
 *              Handle for the target device.
 *
 * Output: -
 *          pui4_speed
 *              Returned speed.
 *
 * Returns: DMR_OK
 *              Success.
 *          DMR_ERR
 *              General error return.
 *          DMR_INVALID_PARM
 *              Invalid parameter data.
 *          DMR_INTERNAL
 *              Internal data structure error.
 *          DMR_OS
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_dm_odd_get_current_speed(
    HANDLE_T h_dev, 
    UINT32 *pui4_speed)
{
    return x_dm_odd_get_current_speed(h_dev, pui4_speed);
}

/*zink CSR BT*/
extern INT32 c_dm_bt_send_io_req(
    HANDLE_T                h_dev, 
    BOOL                    b_in_isr,
    DM_USB_BT_IO_REQ_T*     pt_io_req)
{
    return x_dm_bt_send_io_req(h_dev, b_in_isr, pt_io_req);
}
                          
extern INT32 c_dm_bt_get_io_req_num(
    HANDLE_T                    h_dev, 
    DM_USB_BT_IO_REQ_NUM_T*     pt_req_num)
{
    return x_dm_bt_get_io_req_num(h_dev, pt_req_num);
}

INT32 c_dm_get_usb_port_number(
    UINT8*                      pui1_usb_port_num)
{
    return x_dm_get_usb_port_number(pui1_usb_port_num);
}



