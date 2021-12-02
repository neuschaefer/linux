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
/*! @file u_sm_cc_event.h
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 405fe63c5869dbe3116cc9b515fbbc93 $
 *
 * @par Description: 
 *         This header file contains Stream Manager CC handler specific 
 *         events.
 *---------------------------------------------------------------------------*/
     
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_CC_HDLR
  *  @{  
  */
/*----------------------------------------------------------------------------*/

     
#ifdef MW_CC_SUPPORT_CUST_1
     /*here we include the .h files in cc_hdlr_c1 directory*/
#include "u_sm_cc_event.h"
#else

#ifndef _U_SM_CC_EVENT_H_
#define _U_SM_CC_EVENT_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sm_event.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define SM_EVN_CCS_APP_OPEN_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 0)  
/**<Event of application request open*/
#define SM_EVN_CCS_APP_PLAY_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 1)  
/**<Event of application request play*/
#define SM_EVN_CCS_APP_STOP_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 2)  
/**<Event of application request stop*/
#define SM_EVN_CCS_APP_CLOSE_REQ          ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 3)  
/**<Event of application request close*/
#define SM_EVN_CCS_DRV_OPENED             ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 4)  
/**<Event of driver opened*/
#define SM_EVN_CCS_DRV_PLAYED             ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 5)  
/**<Event of driver played*/
#define SM_EVN_CCS_DRV_STOPPED            ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 6)  
/**<Event of driver stopped*/
#define SM_EVN_CCS_DRV_CLOSED             ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 7)  
/**<Event of driver closed*/
#define SM_EVN_CCS_DRV_ERR                ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 8)  
/**<Event of driver error*/
#define SM_EVN_CCS_SCDB_RECORD_DEL        ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 9)  
/**<Event of scdb record delete*/
#define SM_EVN_CCS_SCDB_RECORD_MOD        ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 10) 
/**<Event of scdb record modify*/
#define SM_EVN_CCS_INTERNAL_ERR           ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 11) 
/**<Event of internal error*/
#define SM_EVN_CCS_EXISTED_CC_IND_CHG     ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + 12) 
/**<Event of existed cc indicator change*/


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif
#endif /* _U_SM_CC_EVENT_H_ */


/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_STRM_MNGR_CC_HDLR */
/*----------------------------------------------------------------------------*/



