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
 * $RCSfile: avc_handler_debug.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/13 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 0ab0deeae753376bea53d108a3e236f2 $
 *
 * Description:
 *         This file contains the AVC Connection Handler functions used for
 *         debugging.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "conn_mngr/avc_handler/_avc_handler.h"
#include "conn_mngr/avc_handler/avc_handler_debug.h"
#include "dbg/u_dbg.h"
#include "tbl_mngr/x_tm.h"
#include "dt/x_dt.h"


/*-----------------------------------------------------------------------------
 * Name: avc_handler_debug_scdb_add_rec
 *
 * Description: This API shows information about the record being added to
 *              an SCDB.
 *
 * Inputs:  pt_stream_comp_id  References the stream.
 *          pt_scdb_rec        References the SCDB record.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID
avc_handler_debug_scdb_add_rec (const STREAM_COMP_ID_T*  pt_stream_comp_id,
                                const SCDB_REC_T*        pt_scdb_rec)
{
#ifdef DEBUG
    switch (pt_stream_comp_id->e_type)
    {
        case ST_AUDIO:
        {
            DBG_INFO((DBG_PREFIX"Adding audio stream (tag:%p)\n",
                      pt_stream_comp_id->pv_stream_tag));
        }
        break;
        
        case ST_VIDEO:
        {
            DBG_INFO((DBG_PREFIX"Adding video stream (tag:%p)\n",
                      pt_stream_comp_id->pv_stream_tag));
        }
        break;
        
        default:
            DBG_INFO((DBG_PREFIX"Adding unknown stream (tag:%p)\n",
                      pt_stream_comp_id->pv_stream_tag));
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_debug_scdb_del_rec
 *
 * Description: This API shows information about the record being removed from
 *              an SCDB.
 *
 * Inputs:  pt_stream_comp_id  References the stream.
 *          pt_scdb_rec        References the SCDB record.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID
avc_handler_debug_scdb_del_rec (const STREAM_COMP_ID_T*  pt_stream_comp_id,
                                const SCDB_REC_T*        pt_scdb_rec)
{
#ifdef DEBUG
    switch (pt_stream_comp_id->e_type)
    {
        case ST_AUDIO:
        {
            DBG_INFO((DBG_PREFIX"Removing audio stream (tag:%p)\n",
                      pt_stream_comp_id->pv_stream_tag));
        }
        break;
        
        case ST_VIDEO:
        {
            DBG_INFO((DBG_PREFIX"Removing video stream (tag:%p)\n",
                      pt_stream_comp_id->pv_stream_tag));
        }
        break;
        
        default:
            DBG_INFO((DBG_PREFIX"Removing unknown stream (tag:%p)\n",
                      pt_stream_comp_id->pv_stream_tag));
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_debug_show_time
 *
 * Description: This API shows the local time (MMM DD, YYY  HH:MM:SS format)
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID avc_handler_show_time (VOID)
{
#ifdef DEBUG
    static const CHAR*  as_months[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    static const UINT8  aui1_hours[24] = {
        12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
        12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    };
    
    TIME_T  t_time;
    DTG_T   t_dtg;
    
    t_time = x_dt_get_utc(NULL, NULL);
    
    if (x_dt_utc_sec_to_loc_dtg(t_time, & t_dtg) == DTR_OK)
    {
        DBG_INFO((DBG_PREFIX"%s %d, %d  %d:%02d:%02d%s\n",
                  as_months[t_dtg.ui1_mo - 1],
                  t_dtg.ui1_day,
                  t_dtg.ui2_yr,
                  aui1_hours[t_dtg.ui1_hr],
                  t_dtg.ui1_min,
                  t_dtg.ui1_sec,
                  (t_dtg.ui1_hr < 12) ? "am" : "pm"));
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: avc_device_type_str
 *
 * Description: This API returns a device type's string description. 
 *
 * Inputs:  e_type  Contains the device type.
 *
 * Outputs: -
 *
 * Returns: the string representation of the device type.
 ----------------------------------------------------------------------------*/
const CHAR* avc_device_type_str (DEVICE_TYPE_T  e_type)
{
    const CHAR*  ps_str;
    
    switch (e_type)
    {
        case DEV_AVC_COMP_VIDEO: ps_str = "Composite video"; break;
        case DEV_AVC_S_VIDEO:    ps_str = "S-Video";         break;
        case DEV_AVC_Y_PB_PR:    ps_str = "YPbPr";           break;
        case DEV_AVC_VGA:        ps_str = "VGA";             break;
        case DEV_AVC_SCART:      ps_str = "SCART";           break;
        case DEV_AVC_DVI:        ps_str = "DVI";             break;
        case DEV_AVC_HDMI:       ps_str = "HDMI";            break;
        case DEV_AVC_AUDIO_INP:  ps_str = "Composite audio"; break;
        case DEV_AVC_SPDIF:      ps_str = "SPDIF";           break;
        case DEV_AVC_COMBI:      ps_str = "Combi";           break;
        default:                 ps_str = "UNKNOWN";         break;
    }
    
    return ps_str;
}
