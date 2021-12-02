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
 * $RCSfile: x_dm.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: $
 *
 * Description: 
 *         This file contains all the transition effect interface APIs  
 *---------------------------------------------------------------------------*/

#ifndef _X_DM_H_
#define _X_DM_H_

#include "u_dm.h"

extern INT32 x_dm_reg_nfy_fct(
    DM_EVT_CAT_T    t_evt_cat,
    x_dm_nfy_fct    pf_nfy,      
    DM_EVT_FILTR_T  *pt_filtr,
    VOID            *pv_tag, 
    HANDLE_T        *ph_nfy);

extern INT32 x_dm_unreg_nfy_fct(
    HANDLE_T        h_nfy);

extern INT32 x_dm_get_device_info(
    HANDLE_T        h_dev, 
    DM_DEV_INF_T    *pt_inf);

extern INT32 x_dm_get_devices(
    DM_DEV_FILTR_T *pt_filtr, 
    DM_DEV_STAT_T *pt_stat,
    UINT32 *pui4_dev_ns);

extern INT32 x_dm_get_device(
    DM_DEV_SPEC_T *pt_spec,
    HANDLE_T *ph_dev);

extern BOOL x_dm_query_automnt(
    DM_HW_TYPE_T  t_hw_type,
    DM_DEV_TYPE_T t_dev_type);

extern INT32 x_dm_odd_spindle_control(
    HANDLE_T h_dev, 
    DM_DEV_IO_SPIN_CTRL_T t_spin_ctrl,
    VOID *pv_tag,
    x_dm_ioc_fct pf_ioc_fct,
    HANDLE_T *ph_ioc);

extern INT32 x_dm_hdd_spindle_control(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_tray_control(
    HANDLE_T h_dev, 
    DM_DEV_IO_TRAY_CTRL_T t_tray_ctrl,
    VOID *pv_tag,
    x_dm_ioc_fct pf_ioc_fct,
    HANDLE_T *ph_ioc);

extern INT32 x_dm_odd_read_dvd_struct(
    HANDLE_T h_dev,
    DM_DEV_IO_MED_READ_DVD_STRUCT_T *pt_struct);

extern INT32 x_dm_odd_read_kcd(
    HANDLE_T h_dev,
    DM_DEV_IO_MED_READ_KCD_T *pt_kcd);

extern INT32 x_dm_odd_com_report_key(
    HANDLE_T h_dev, 
    DM_DEV_IO_COM_REPORT_KEY_T *pt_key);

extern INT32 x_dm_odd_com_send_key(
    HANDLE_T h_dev, 
    DM_DEV_IO_COM_SEND_KEY_T *pt_key);

extern INT32 x_dm_odd_com_get_feature(
    HANDLE_T h_dev, 
    DM_DEV_IO_COM_GET_FEATURE_T *pt_feature);

extern INT32 x_dm_odd_read_cd(
    HANDLE_T h_dev, 
    DM_DEV_MED_DISC_READ_CD_T *pt_read_cd);

extern INT32 x_dm_odd_read_cd_async(
    HANDLE_T h_dev, 
    DM_DEV_MED_DISC_READ_CD_T *pt_read_cd,
    VOID *pv_tag,
    x_dm_ioc_fct pf_ioc_fct,
    HANDLE_T *ph_ioc);

extern INT32 x_dm_odd_read_cd_text(
    HANDLE_T h_dev, 
    DM_DEV_MED_READ_CD_TEXT_T *pt_read_txt);

extern INT32 x_dm_odd_read_disc_info(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_get_standby_timer(
    HANDLE_T h_dev, 
    UINT32 *pui4_time);

extern INT32 x_dm_hdd_get_standby_timer(
    HANDLE_T h_dev, 
    UINT32 *pui4_time);

extern INT32 x_dm_odd_set_standby_timer(
    HANDLE_T h_dev, 
    UINT32 ui4_time);

extern INT32 x_dm_hdd_set_standby_timer(
    HANDLE_T h_dev, 
    UINT32 ui4_time);

extern INT32 x_dm_odd_flush_cache(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_hdd_flush_cache(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_low_format(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_nand_low_format(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_disc_finalize(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_disc_unfinalize(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_disc_upgrade(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_set_io_speed(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_hw_reset(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_release_bus(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_test_unit_ready(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_get_event_status(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_mechanism_status(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_set_book_type(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_flashless_download(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_set_transfer_mode(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_read_buffer_capacity(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_do_opc(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_read_dvd_structure(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_vendor_inquiry(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_auto_tune(
    HANDLE_T h_dev, 
    VOID *pv_parm);

extern INT32 x_dm_odd_get_med_nwa(
    HANDLE_T h_dev, 
    UINT32 *pui4_nwa);

extern INT32 x_dm_odd_get_med_toc_hdr(
    HANDLE_T h_dev, 
    DM_DEV_MED_DISC_TOC_HEADER_T *pt_toc_hdr);

extern INT32 x_dm_odd_get_med_toc_ent(
    HANDLE_T h_dev,
    DM_DEV_MED_DISC_TOC_ENTRY_T *pt_toc_ent);

extern INT32 x_dm_odd_get_sess_ns(
    HANDLE_T h_dev,
    UINT32 *pui4_sess_ns);

extern INT32 x_dm_odd_get_sess_inf(
    HANDLE_T h_dev,
    UINT32 ui4_sess_no,
    DM_DEV_MED_DISC_SESS_INFO_T *pt_inf);

extern INT32 x_dm_odd_get_trk_ns(
    HANDLE_T h_dev,
    UINT32 *pui4_trk_ns);

extern INT32 x_dm_odd_get_trk_inf(
    HANDLE_T h_dev,
    UINT32 ui4_trk_no,
    DM_DEV_MED_DISC_TRK_INFO_T *pt_inf);

extern INT32 x_dm_odd_get_sector_sz(
    HANDLE_T h_dev,
    UINT32 ui4_lba,
    DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T *pt_sz);

extern INT32 x_dm_odd_get_last_sense_key(
    HANDLE_T h_dev, 
    UINT32 *pui4_key);

extern INT32 x_dm_odd_get_current_speed(
    HANDLE_T h_dev, 
    UINT32 *pui4_speed);

/*zink CSR BT*/
extern INT32 x_dm_bt_send_io_req(
    HANDLE_T            h_dev,
    BOOL                b_in_isr,
    DM_USB_BT_IO_REQ_T* pt_io_req);
                          
extern INT32 x_dm_bt_get_io_req_num(
    HANDLE_T                    h_dev, 
    DM_USB_BT_IO_REQ_NUM_T*     pt_req_num); 

extern INT32 x_dm_get_usb_port_number(
    UINT8*                      pui1_usb_port_num);


#endif

