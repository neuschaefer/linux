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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: papi_ioctl_papionly.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file papi_ioctl_papionly.h
 *  IO control of PAPI Frontend interface
 */

#ifndef PAPI_IOCTL_PAPIONLY_H
#define PAPI_IOCTL_PAPIONLY_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "papi_ioctl_base.h"
#include <linux/ioctl.h>


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// key
#define PAPI_IO_PAPIONLY_KEY_ENABLE       		_IOW(PAPI_IOCTYPE_PAPIONLY, 0, int)
#define PAPI_IO_PAPIONLY_KEY_DISABLE      _IOW(PAPI_IOCTYPE_PAPIONLY, 1, int)
#define PAPI_IO_PAPIONLY_KEY_ISENABLED   _IOW(PAPI_IOCTYPE_PAPIONLY, 2, int)
#define PAPI_IO_PAPIONLY_KEY_INIT   _IOW(PAPI_IOCTYPE_PAPIONLY, 3, int)
#define PAPI_IO_PAPIONLY_KEY_POLL   _IOW(PAPI_IOCTYPE_PAPIONLY, 4, int)
#define PAPI_IO_PAPIONLY_KEY_KILL   _IOW(PAPI_IOCTYPE_PAPIONLY, 5, int)

// lkb
#define PAPI_IO_PAPIONLY_LKB_ENABLE         _IOW(PAPI_IOCTYPE_PAPIONLY, 10, int)
#define PAPI_IO_PAPIONLY_LKB_DISABLE      _IOW(PAPI_IOCTYPE_PAPIONLY, 11, int)
#define PAPI_IO_PAPIONLY_LKB_ISENABLED   _IOW(PAPI_IOCTYPE_PAPIONLY, 12, int)
#define PAPI_IO_PAPIONLY_LKB_INIT   _IOW(PAPI_IOCTYPE_PAPIONLY, 13, int)
#define PAPI_IO_PAPIONLY_LKB_POLL   _IOW(PAPI_IOCTYPE_PAPIONLY, 14, int)

//cec
#define PAPI_IO_PAPIONLY_PROGRAMCECWAKEUPCODES		_IOW(PAPI_IOCTYPE_PAPIONLY, 20, IOCTL_2ARG_T)
#define PAPI_IO_PAPIONLY_GETWAKEUPDETAILSCEC		_IOW(PAPI_IOCTYPE_PAPIONLY, 21, IOCTL_3ARG_T)

//wakeup
#define PAPI_IO_PAPIONLY_WAKEUP_PROGRAMWAKEUPREASONS         _IOW(PAPI_IOCTYPE_PAPIONLY, 30, int)
#define PAPI_IO_PAPIONLY_WAKEUP_PROGRAMWAKEUPKEYSRC      _IOW(PAPI_IOCTYPE_PAPIONLY, 31, int)
#define PAPI_IO_PAPIONLY_WAKEUP_PROGRAMWAKEUPLKB   _IOW(PAPI_IOCTYPE_PAPIONLY, 32, int)
#define PAPI_IO_PAPIONLY_WAKEUP_GETWAKEUPDETAILSRC   _IOW(PAPI_IOCTYPE_PAPIONLY, 33, int)
#define PAPI_IO_PAPIONLY_WAKEUP_GETWAKEUPDETAILSLKB   _IOW(PAPI_IOCTYPE_PAPIONLY, 34, int)
#define PAPI_IO_PAPIONLY_WAKEUP_GETPROGRAMMEDWAKEUPREASONS   _IOW(PAPI_IOCTYPE_PAPIONLY, 35, int)
#define PAPI_IO_PAPIONLY_WAKEUP_GETACTUALWAKEUPREASONS   _IOW(PAPI_IOCTYPE_PAPIONLY, 36, int)

// lvds
#define PAPI_IO_PAPIONLY_LVDS_SWITCH _IOW(PAPI_IOCTYPE_PAPIONLY, 50, int)
#define PAPI_IO_PAPIONLY_LVDS_ISENABLED _IOW(PAPI_IOCTYPE_PAPIONLY, 51, int)

// CALLBACK NOTIFICATION
#define PAPI_CB_PAPIONLY_ONGAMUTDATACHANGED 0
#endif  // PAPI_IOCTL_PAPIONLY_H

