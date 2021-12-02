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
/*! @file u_dlm_net.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $ 
 *  
 *  @par Description:
 *         This header file contains download firmware specific
 *         definitions, which are exported.
 *         
 */
/*----------------------------------------------------------------------------*/


#ifndef __U_DLM_NET_H__
#define __U_DLM_NET_H__

#include "x_common.h"
#include "x_os.h"
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_INET_DLM_NET DOWNLOAD FIRMWARE API
 *  @ingroup    groupMW_INET
 *  @brief      Provide an external definitions about network download firmware
 *  %MW_INET_DLM_NET API would be used by user who wants download firmware via network.
 *              
 *              
 *  @{
 */

#define DLM_NET_INTERVAL_TIMEOUT            60    /**<    used for CURLOPT_LOW_SPEED_TIME    */
#define DLM_NET_CONNECT_TIMEOUT             10    /**<    define connect to server timeout    */
#define DLM_NET_UNKNOW_SIZE                 0xffffffff    /**<  used for chunk content length  */
/*------------------------------------------------------------------*/
/*! @enum DLM_NETR_CODE_T
 *  @brief download firmware return code
 *  @code
 *  typedef enum _DLM_NETR_CODE_T
 *  {
 *      DLM_NETR_OK = 0,
 *      DLM_NETR_OUT_OF_MEMORY = -1,
 *      DLM_NETR_INVALID_ARGUMENT = -2,
 *      DLM_NETR_AUTHENTICATE_ERROR = -3,
 *      DLM_NETR_DOWNLOAD_ERROR = -4,
 *      DLM_NETR_SET_OPT_ERROR = -5,
 *      DLM_NETR_SESSION_ERROR = -6
 *  }DLM_NETR_CODE_T;
 *  @endcode
 *  @li@c  DLM_NETR_OK                                       -operation success
 *  @li@c  DLM_NETR_OUT_OF_MEMORY                            -memory alloc faile
 *  @li@c  DLM_NETR_INVALID_ARGUMENT                         -operation argument error
 *  @li@c  DLM_NETR_AUTHENTICATE_ERROR                           -start authenticate faile
 *  @li@c  DLM_NETR_DOWNLOAD_ERROR                           -start download faile
 *  @li@c  DLM_NETR_SET_OPT_ERROR                            -set http opetation error
 *  @li@c  DLM_NETR_SESSION_ERROR                             -open http session error
 */
/*------------------------------------------------------------------*/
typedef enum _DLM_NETR_CODE_T
{
    DLM_NETR_OK = 0,
    DLM_NETR_OUT_OF_MEMORY = -1,
    DLM_NETR_INVALID_ARGUMENT = -2,
    DLM_NETR_AUTHENTICATE_ERROR = -3,
    DLM_NETR_DOWNLOAD_ERROR = -4,
    DLM_NETR_SET_OPT_ERROR = -5,
    DLM_NETR_SESSION_ERROR = -6
}DLM_NETR_CODE_T;

/*------------------------------------------------------------------*/
/*! @enum DLM_NET_ASYNC_REQ_T
 *  @brief download firmware callback return state
 *  @code
 *  typedef enum _DLM_NET_ASYNC_REQ_T
 *  {
 *      DLM_NET_ASYNC_COND_OK = 0,
 *      DLM_NET_ASYNC_COND_CANCEL = -1,
 *      DLM_NET_ASYNC_COND_RSP_ERROR = -2,
 *      DLM_NET_ASYNC_COND_ERROR = -3
 *  }DLM_NET_ASYNC_REQ_T;
 *  @endcode
 *  @li@c  DLM_NET_ASYNC_COND_OK                             -download success
 *  @li@c  DLM_NET_ASYNC_COND_CANCEL                         -download cancel by application
 *  @li@c  DLM_NET_ASYNC_COND_RSP_ERROR                      -request error
 *  @li@c  DLM_NET_ASYNC_COND_ERROR                          -download error
 */
/*------------------------------------------------------------------*/
typedef enum _DLM_NET_ASYNC_REQ_T
{
    DLM_NET_ASYNC_COND_OK = 0,
    DLM_NET_ASYNC_COND_CANCEL = -1,
    DLM_NET_ASYNC_COND_RSP_ERROR = -2,
    DLM_NET_ASYNC_COND_ERROR = -3
}DLM_NET_ASYNC_REQ_T;

/*------------------------------------------------------------------*/
/*! @brief This is a callback function for progress callback function
 *  @param [in] ui4_total_sz -download total data
 *  @param [in] ui4_now_sz -download now data
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_dlm_net_progress_cb)(UINT32 ui4_total_sz, UINT32 ui4_now_sz);

/*------------------------------------------------------------------*/
/*! @brief This is a callback function for download asynchronized function
 *  @param [in] e_state -the download flow condition when call the callback
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_dlm_net_dl_async_cb)(DLM_NET_ASYNC_REQ_T   e_state);

/*------------------------------------------------------------------*/
/*! @brief This is a callback function for authenticate asynchronized function
 *  @param [in] e_state -the download flow condition when call the callback
 *  @param [in] ps_response -the response data from server
 *  @param [in] ui4_len -the length of response
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_dlm_net_auth_async_cb)(DLM_NET_ASYNC_REQ_T e_state,
                                           CHAR                 *ps_response,
                                           UINT32               ui4_len);




/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/

#endif /* __U_DLM_NET_H__ */


