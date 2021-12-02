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
/*! @file u_device_type.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains Device Type definitions, which are exported
 *    to other Middleware components and applications.
 *         
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_DEVICE_TYPE_H_
#define _U_DEVICE_TYPE_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/


/*
    A device id is used to identify a specific device object.
    The id consists of the device group name (the upper 8 bits) and device name id
    (the lower 24 bits). 
*/
typedef  UINT32                DEVICE_TYPE_T;    /**< defination of device type      */

/*
   Macro for the device groups.  The device groups are
   logical grouping of source devices.
*/

#define	 DEV_UNKNOWN           ((DEVICE_TYPE_T) 0x00000000)    /**< Unknown device type       */

#define  DEV_1394              ((DEVICE_TYPE_T) 0x01000000) /* 1394 devices group. */    /**<        */
#define  DEV_TUNER             ((DEVICE_TYPE_T) 0x02000000) /* tuner device group. */    /**<        */
#define	 DEV_AVC               ((DEVICE_TYPE_T) 0x03000000) /* AV Connect group. */    /**<        */
#define  DEV_VTRL              ((DEVICE_TYPE_T) 0x04000000) /* virtual connect group. */    /**<        */

/* End marker. */
#define	 DEV_RESERVED          ((DEVICE_TYPE_T) 0x05000000)    /**<   reserved device     */


/* Get DEV group type. */
#define  DEV_GET_GROUP_ID(_dev_)  (_dev_ & 0xFF000000)    /**<    get group id    */

/* Get DEV device id. */
#define  DEV_GET_DEV_ID(_dev_) (_dev_ & 0x00FFFFFF)    /**<   get device id     */

/* Check for valid DEV group */
#define  DEV_IS_VALID_GROUP(_group_name_) \
 (((_group_name_>>24)>DEV_UNKNOWN)&&((_group_name_>>24)<(DEV_RESERVED>>24))?TRUE:FALSE)    /**<    is valid group    */

/*
 	Macros for the device names 
*/

/*   1394 Devices */
#define   DEV_1394_MONITOR          (DEV_1394 |  0x00000001)    /**<  1394 monitor      */
#define	  DEV_1394_AUDIO            (DEV_1394 |  0x00000002)    /**<  1394 audio     */
#define	  DEV_1394_PRINTER          (DEV_1394 |  0x00000003)    /**<  1394 printer     */
#define	  DEV_1394_DISC             (DEV_1394 |  0x00000004)    /**<  1394 disc      */
#define	  DEV_1394_TAPE             (DEV_1394 |  0x00000005)    /**<  1394 tape      */
#define	  DEV_1394_TUNER            (DEV_1394 |  0x00000006)    /**<  1394 tuner      */
#define	  DEV_1394_CA               (DEV_1394 |  0x00000007)    /**<  1394 ca      */
#define	  DEV_1394_CAMERA           (DEV_1394 |  0x00000008)    /**<  1394 camerra      */
#define	  DEV_1394_PANEL            (DEV_1394 |  0x00000009)    /**<  1394 panel      */

#define   DEV_1394_RESERVED		    (DEV_1394 |  0x0000000A)    /**<   reserved     */

/*   AV connector device */
#define   DEV_AVC_COMP_VIDEO        (DEV_AVC  |  0x00000001)    /**<   comp video     */
#define	  DEV_AVC_S_VIDEO           (DEV_AVC  |  0x00000002)    /**<    s video    */
#define	  DEV_AVC_Y_PB_PR           (DEV_AVC  |  0x00000003)    /**<   y pb pr     */
#define	  DEV_AVC_VGA               (DEV_AVC  |  0x00000004)    /**<   vga     */
#define	  DEV_AVC_SCART	            (DEV_AVC  |  0x00000005)    /**<   scart     */
#define	  DEV_AVC_DVI               (DEV_AVC  |  0x00000006)    /**<   dvi     */
#define	  DEV_AVC_HDMI              (DEV_AVC  |  0x00000007)    /**<   hdmi     */
#define	  DEV_AVC_AUDIO_INP         (DEV_AVC  |  0x00000008)    /**<   audio     */
#define	  DEV_AVC_SPDIF             (DEV_AVC  |  0x00000009)    /**<   spdif     */
#define	  DEV_AVC_COMBI             (DEV_AVC  |  0x0000000A)    /**<   combi     */

#define	  DEV_AVC_RESERVED          (DEV_AVC  |  0x0000000B)    /**<   reserved     */

/* Tuner Devices */
#define	  DEV_TUNER_SAT_DIG         (DEV_TUNER |  0x00000001)    /**<  tuner sat digital      */
#define	  DEV_TUNER_CAB_DIG         (DEV_TUNER |  0x00000002)    /**<  tuner cab digital      */
#define	  DEV_TUNER_TER_DIG         (DEV_TUNER |  0x00000003)    /**<  tuner ter digital      */
#define   DEV_TUNER_SAT_ANA         (DEV_TUNER |  0x00000004)    /**<  tuner sat analog     */
#define	  DEV_TUNER_CAB_ANA         (DEV_TUNER |  0x00000005)    /**<  tuner cab analog      */
#define	  DEV_TUNER_TER_ANA         (DEV_TUNER |  0x00000006)    /**<  tuner ter analog      */
#define	  DEV_TUNER_CAB_DIG_OOB_TX  (DEV_TUNER |  0x00000007)    /**<  tuner cab digital oob tx      */
#define	  DEV_TUNER_CAB_DIG_OOB_RX  (DEV_TUNER |  0x00000008)    /**<  tuner cab digital oob rx      */

#define	  DEV_TUNER_RESERVED        (DEV_TUNER |  0x00000009)    /**<   reserved     */

/* Virtual connector devices */
#define	  DEV_VTRL_MMP              (DEV_VTRL |  0x00000001)    /**<  mmp      */
#define	  DEV_VTRL_MMP_PHOTO        (DEV_VTRL |  0x00000002)    /**<   mmp photo     */
#define	  DEV_VTRL_MMP_AUDIO        (DEV_VTRL |  0x00000003)    /**<   mmp audio    */
#define	  DEV_VTRL_MMP_VIDEO        (DEV_VTRL |  0x00000004)    /**<   mmp video    */

#define	  DEV_VTRL_RESERVED         (DEV_VTRL |  0x00000005)    /**<   reserved     */

#endif /* _U_DEVICE_TYPE_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW*/
/*----------------------------------------------------------------------------*/
