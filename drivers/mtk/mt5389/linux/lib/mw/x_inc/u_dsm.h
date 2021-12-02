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
/*! @file u_dsm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains Device Status Manager (DSN) macro, typedef,
 *    and data structure definitions, which are exported to other Middleware
 *    components and applications.
 *         
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_DSM Device Status Manager
 *
 *  @ingroup groupMW
 *  @brief Provides external function for TV Application, Provides function for open device group/device or
 *           set/get information for devices .
 *  
 *
 *  @see groupMW
 *  @{ 
 */
 /*----------------------------------------------------------------------------*/
#ifndef _U_DSM_H_
#define _U_DSM_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_device_type.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

/* Return code status from DSM API */
#define   DSMR_ASYNC_NFY              ((INT32)  1)    /**<        */
#define   DSMR_OK                     ((INT32)  0)    /**<        */
#define	  DSMR_FAIL                   ((INT32) -1)    /**<        */
#define   DSMR_OUT_OF_HANDLE          ((INT32) -2)    /**<        */
#define	  DSMR_OUT_OF_MEM             ((INT32) -3)    /**<        */
#define	  DSMR_INV_HANDLE             ((INT32) -4)    /**<        */
#define   DSMR_INV_ARG                ((INT32) -5)    /**<        */
#define	  DSMR_DEV_TYPE_NOT_SUPPORTED ((INT32) -6)    /**<        */
#define	  DSMR_DEV_NOT_FOUND          ((INT32) -7)    /**<        */
#define   DSMR_AEE_OUT_OF_RESOURCES   ((INT32) -8)    /**<        */
#define   DSMR_AEE_NO_RIGHTS          ((INT32) -9)    /**<        */

/*
   Return code from DSM calls to RM API's, these return code are
   also shared with the 1394 get/set APIs.
*/
#define   DSMR_INV_GET_OP            ((INT32) -10)    /**<        */
#define   DSMR_INV_GET_INFO          ((INT32) -11)    /**<        */
#define   DSMR_INV_SET_OP            ((INT32) -12)    /**<        */
#define   DSMR_INV_SET_INFO          ((INT32) -13)    /**<        */
#define   DSMR_NOT_ENOUGH_SPACE	     ((INT32) -14)    /**<        */

/* 
   Return code from DSM get/set APIs to 1394 devices.
*/
#define   DSMR_INV_GET_DATA          ((INT32) -11)    /**<        */
#define   DSMR_INV_SET_DATA          ((INT32) -13)    /**<        */
#define   DSMR_REJECTED              ((INT32) -14)    /**<        */


/* Handle type for the DSM group. */
#define   DSMT_GROUP_T              (HT_GROUP_DSM + ((HANDLE_TYPE_T) 1))    /**<        */

/* Handle type for the DSM device. */
#define   DSMT_DEVICE_T             (HT_GROUP_DSM + ((HANDLE_TYPE_T) 2))    /**<        */

/* Handle type for the DSM component. This handle type is only used internally
   by the DSM to RM interface.
*/
#define   DSMT_DSM_COMP_T           (HT_GROUP_DSM + ((HANDLE_TYPE_T) 3))    /**<        */

/* 
    typedef for DSM device notification condition.
*/
typedef	  UINT32                       DSM_COND_DEV_T;    /**<        */

/* 
    DSM device notification condition. 
*/

#define	DSM_COND_DEV_HANDLE_CLOSED	     ((DSM_COND_DEV_T)(1))    /**<        */

#define	DSM_COND_AVC_DEV		         ((DSM_COND_DEV_T)(128))    /**<        */
#define	DSM_COND_DEV_VIDEO_SIG_STATUS    ((DSM_COND_DEV_T)(DSM_COND_AVC_DEV+1))    /**<        */
#define	DSM_COND_DEV_AUDIO_SIG_STATUS	 ((DSM_COND_DEV_T)(DSM_COND_AVC_DEV+2))    /**<        */
#define	DSM_COND_DEV_MONITOR_STOP	     ((DSM_COND_DEV_T)(DSM_COND_AVC_DEV+3))    /**<        */
#define	DSM_COND_DEV_MONITOR_START	     ((DSM_COND_DEV_T)(DSM_COND_AVC_DEV+4))    /**<        */
#define DSM_COND_DEV_CABLE_STATUS        ((DSM_COND_DEV_T)(DSM_COND_AVC_DEV+5))    /**<        */

/* this is deprecated. */
#define DSM_COND_DEV_SIGNAL_STATUS       ((DSM_COND_DEV_T)(DSM_COND_AVC_DEV+2))    /**<        */


/*
Enum Flag for 'pass by value' or 'pass by reference'
for the 'pv_data' field in the DSM notification functions:
*/ 
/*
typedef enum
{
    DSM_NULL = 0,
    DSM_PASS_BY_VALUE,
    DSM_PASS_BY_REF	
} DSM_NFY_DATA_T ;
*/

/* 
   DSM device group set and get type operation.
*/
typedef   UINT32                       DSM_SET_T;    /**<        */
typedef   UINT32                       DSM_GET_T;    /**<        */


/* 
	types for DSM device set and get type operation.
*/
typedef   UINT32                       DSM_DEV_SET_T;    /**<        */

#define   DSM_SET_AVC_START_MONITOR    ((DSM_DEV_SET_T) 1)    /**<        */
#define   DSM_SET_AVC_STOP_MONITOR     ((DSM_DEV_SET_T) 2)    /**<        */

typedef   UINT32                       DSM_DEV_GET_T;    /**<        */

#define   DSM_GET_AVC_NUM_OF_DEV        ((DSM_DEV_GET_T) 32)    /**<        */
#define   DSM_GET_AVC_DEV_TYPE          ((DSM_DEV_GET_T) 33)    /**<        */
#define	  DSM_GET_AVC_VIDEO_SIG_STATUS  ((DSM_DEV_GET_T) 34)    /**<        */
#define	  DSM_GET_AVC_AUDIO_SIG_STATUS  ((DSM_DEV_GET_T) 35)    /**<        */
#define   DSM_GET_AVC_CABLE_STATUS      ((DSM_DEV_GET_T) 36)    /**<        */

/* this is deprecated. */
/* #define   DSM_GET_AVC_SIGNAL_STATUS     ((DSM_DEV_GET_T) 36) */


#define   DSM_VIEDO_TYPE_UNKNOWN             (0)    /**<        */
#define   DSM_VIDEO_TYPE_COMPOSITE           (1)    /**<        */
#define   DSM_VIDEO_TYPE_RGB                 (2)    /**<        */
#define   DSM_VIDEO_TYPE_MIXED               (3)    /**<        */
#define   DSM_VIDEO_TYPE_S_VIDEO             (4)    /**<        */

/*
  Video signal status information

  The status are bit-masked.  The SCART device provides
  video signal aspect ratio.  For example, when client receive notification
  or calling x_dsm_dev_get() API on the SCART video signal status, it can checks
  masked-bit to determine the value of the aspect ratio.

  For example:

  signal on and aspect ratio is 4:3

  (DSM_VIDEO_SIGNAL_ON | DSM_VIDEO_SIGNAL_ASPECT_RATIO_4_3)

  signal on and aspect ratio is 16:9
                
  (DSM_VIDEO_SIGNAL_ON | DSM_VIDEO_SIGNAL_ASPECT_RATIO_16_9)

  no video signal

  (DSM_VIDEO_SIGNAL_OFF)

*/
  
#define   DSM_VIDEO_SIGNAL_UNKNOWN           (0)    /**<        */
#define   DSM_VIDEO_SIGNAL_OFF               (1)    /**<        */
#define   DSM_VIDEO_SIGNAL_ON                (2)    /**<        */
#define   DSM_VIDEO_SIGNAL_ASPECT_RATIO_4_3  (4)    /**<        */
#define   DSM_VIDEO_SIGNAL_ASPECT_RATIO_16_9 (8)    /**<        */


/*
  Default parameters for the DSM thread and message queue.
*/
#define   DSM_DEFAULT_STACK_SZ      2048    /**<        */
#define   DSM_DEFAULT_PRIORITY      128    /**<        */
#define   DSM_DEFAULT_MSG_SIZE      64    /**<        */
#define   DSM_DEFAULT_NUM_OF_MSG    32    /**<        */

#define   DSM_DEFAULT_THREAD_NAME   "dsm_thread"    /**<        */
#define   DSM_MSG_QUEUE_NAME        "dsm_msg_q"    /**<        */
/*---------------------------------------------------------------------------
 Structures
----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @struct DSM_CONFIG_T
 *  @brief DSM config information
 *  @code
 *  typedef struct _DSM_CONFIG_T_
 *  {
 *      SIZE_T   z_stack_sz;          
 *      UINT8    ui1_thread_priority; 
 *      UINT16   z_msg_size;          
 *      UINT16   ui2_msg_count;       
 *  }  DSM_CONFIG_T;
 *  @endcode
 *  @li@c  z_stack_sz                                        - stack size ,default size is 2048
 *  @li@c  ui1_thread_priority                               -thread priority  default is 128
 *  @li@c  z_msg_size                                        - msg size ,default is 64
 *  @li@c  ui2_msg_count                                     -msg number,default is 32
 */
/*------------------------------------------------------------------*/
typedef struct _DSM_CONFIG_T_
{
    SIZE_T   z_stack_sz;          /* If 0, then use the default size of 1024 */
    UINT8    ui1_thread_priority; /* if 0, then use priority of 32. */
    UINT16   z_msg_size;          /* If 0, then use default size of 64. */
    UINT16   ui2_msg_count;       /* If 0, then use the default msg count of = 32 */
}  DSM_CONFIG_T;
 
/*---------------------------------------------------------------------------
 Type definition
----------------------------------------------------------------------------*/

/*
  typedef for the DSM device notification function.  This fucntion
  is registered by the client to the DSM when calling x_dsm_dev_open.
*/
/*------------------------------------------------------------------*/
/*! @brief  Prototype for the device notification function. 
 *  @param [in] h_dsm_dev	handle to the device or device group.
 *  @param [in] e_cond	      Condition for the notification. This condition is defined by the actual device layer.  
 *                                     The DSM will translate the device notification condition to DSM device condition. 
 *                                    For example: if the source of notification is DEV_AVC_COMBI, then the notification is 
 *                                    one of the following value:
 *                          		DSM_COND_DEV_SIGNAL_STATUS
 *                          		DSM_COND_DEV_HANDLE_CLOSED
 *                         		DSM_COND_DEV_MONITOR_STOP
 *  @param [in]  pv_tag	client's tag value.
 *  @param [in]  ui4_data_1	 contain data returned to the client for the notification.  The interpretation of this data depends 
 *                                      on the notification source and condition of the notification.  
 *  @param [in] ui4_data_2:	 contain additional data to the client for the notification.
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*dsm_dev_nfy_fct)
(
    HANDLE_T          h_dsm_dev,
    DSM_COND_DEV_T    e_cond,
    VOID*             pv_tag,
    UINT32            ui4_data_1,
    UINT32            ui4_data_2
);


#endif /* _U_DSM_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_DSM */
/*----------------------------------------------------------------------------*/
