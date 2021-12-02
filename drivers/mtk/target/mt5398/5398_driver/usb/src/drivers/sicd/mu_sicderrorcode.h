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

/*!
 * @file      sicderrcode.h
 * @author
 * @version   1.0
 * @brief     This file wraps the SICD error definition. \n
 */
#ifndef _SICDERRCODE_H_
#define _SICDERRCODE_H_

typedef uint32_t MUSB_SicdResult;



/*****************************************************************************
 | 31(1 bit) |30-24(7 bits) |23 - 21(3 bits)| 23-16(5 bits) | 15-0(16 bits) |
 | Indicator | Module ID    | Error Level   |  Group ID     |  error code   |

 Indicator  : 0 - success, 1 - error
 Module ID  : Module ID, defined below
 Error Level: Error level
 Group ID   : Classify group by class definition.
 Err code   : real error code
*****************************************************************************/
#define SICD_ERRORCODE(module_id, error_lev, group_id,error_code) \
               ((0x80000000) \
               | ((module_id) << 24) \
               | ((error_lev) << 21) \
               | ((group_id) << 16) \
               | (error_code))

#define MUSB_SICD_SUCCESS             0
#define MUSB_SICD_DATA_PHASE_CONTINUE 1
#define MUSB_SICD_RESPONSE_PHASE_CONTINUE 2
#define MUSB_SICD_DATA_PHASE_CONTROLLED_BY_MW 3


#define SICD_MODULE               0x20

/***********error level definition***********/
#define SICD_ERROR_LEVEL1   0x01
#define SICD_ERROR_LEVEL2   0x02
#define SICD_ERROR_LEVEL3   0x03

/***********group definition**************/
#define SICD_GRP_PTP_MTP             0x1



/***********real error code definition***********/
#define MUSB_E_SICD_MEMORY_OVERFLOW          SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL3, SICD_GRP_PTP_MTP, 0x01)
#define MUSB_E_SICD_PARAMETER_ERROR     SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x02)
#define MUSB_E_SICD_NULL_POINTER_ERROR    SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL3, SICD_GRP_PTP_MTP, 0x04)
#define MUSB_E_SICD_DEVICE_STATE_ERROR    SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL3, SICD_GRP_PTP_MTP, 0x05)
#define MUSB_E_SICD_DEVICE_BUSY                   SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x06)
#define MUSB_E_SICD_DEVICE_TRANSACTION_CANCELLED                  SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x07)

#define MUSB_E_SICD_DRIVER_NOT_READY     SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x10)
#define MUSB_E_SICD_DRIVER_NOT_IN_DATA_PHASE     SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x14)
#define MUSB_E_SICD_DRIVER_BUSYING          SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x11)
#define MUSB_E_SICD_DRIVER_STOPPING         SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x12)
#define MUSB_E_SICD_DRIVER_RUNNING         SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x13)
#define MUSB_E_SICD_DRIVER_ERROR    SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x14)

#define MUSB_E_SICD_DATA_NOT_READY    SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x20)
#define MUSB_E_SICD_DATA_HEADER_FIRST   SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x21)

#define MUSB_E_SICD_RESPONSE_ERROR    SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x30)

#define MUSB_E_SICD_DEVICE_DISCON       SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x40)

#define MUSB_E_SICD_CMD_RETRY_FAIL      SICD_ERRORCODE(SICD_MODULE, SICD_ERROR_LEVEL1, SICD_GRP_PTP_MTP, 0x50)
#endif //_SICDERRCODE_H_
