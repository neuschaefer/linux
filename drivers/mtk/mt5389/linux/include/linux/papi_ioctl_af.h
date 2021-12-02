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
 * $RCSfile: papi_ioctl_af.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file papi_ioctl_af.h
 *  IO control of PAPI AF interface
 */

#ifndef PAPI_IOCTL_AF_H
#define PAPI_IOCTL_AF_H

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

/* PAPI_IOCTYPE_AF */

#define PAPI_IO_AF_SSOO_SETVOLUME                  _IOW(PAPI_IOCTYPE_AF, 0, int)
#define PAPI_IO_AF_SSOO_GETVOLUME                  _IOW(PAPI_IOCTYPE_AF, 1, int)
#define PAPI_IO_AF_SSOO_SETMUTE                      _IOW(PAPI_IOCTYPE_AF, 2, int)
#define PAPI_IO_AF_SSOO_GETMUTE                      _IOW(PAPI_IOCTYPE_AF, 3, int)
#define PAPI_IO_AF_SSOO_MUTE_ENABLE              _IOW(PAPI_IOCTYPE_AF, 4, int)
#define PAPI_IO_AF_SSOO_SETBALANCE                 _IOW(PAPI_IOCTYPE_AF, 5, int)
#define PAPI_IO_AF_SSOO_GETBALANCE                 _IOW(PAPI_IOCTYPE_AF, 6, int)
#define PAPI_IO_AF_SSOO_SETBASS                       _IOW(PAPI_IOCTYPE_AF, 7, int)
#define PAPI_IO_AF_SSOO_GETBASS                       _IOW(PAPI_IOCTYPE_AF, 8, int)
#define PAPI_IO_AF_SSOO_SETTREBLE                    _IOW(PAPI_IOCTYPE_AF, 9, int)
#define PAPI_IO_AF_SSOO_GETTREBLE                    _IOW(PAPI_IOCTYPE_AF, 10, int)
#define PAPI_IO_AF_SSOO_GETEQBANDVALUE        _IOW(PAPI_IOCTYPE_AF, 11, int)
#define PAPI_IO_AF_SSOO_SETEQBANDVALUE        _IOW(PAPI_IOCTYPE_AF, 12, int)
#define PAPI_IO_AF_SSOO_GETDELTAVOLUME        _IOW(PAPI_IOCTYPE_AF, 13, int)
#define PAPI_IO_AF_SSOO_SETDELTAVOLUME        _IOW(PAPI_IOCTYPE_AF, 14, int)

#define PAPI_IO_AF_HSOO_SETVOLUME                    _IOW(PAPI_IOCTYPE_AF, 16, int)
#define PAPI_IO_AF_HSOO_GETVOLUME                    _IOW(PAPI_IOCTYPE_AF, 17, int)
#define PAPI_IO_AF_HSOO_SETMUTE                        _IOW(PAPI_IOCTYPE_AF, 18, int)
#define PAPI_IO_AF_HSOO_GETMUTE                        _IOW(PAPI_IOCTYPE_AF, 19, int)
#define PAPI_IO_AF_HSOO_GETDELTAVOLUME          _IOW(PAPI_IOCTYPE_AF, 20, int)
#define PAPI_IO_AF_HSOO_SETDELTAVOLUME          _IOW(PAPI_IOCTYPE_AF, 21, int)

#define PAPI_IO_AF_SOM_GETAUTOSURROUND     _IOW(PAPI_IOCTYPE_AF, 30, int)
#define PAPI_IO_AF_SOM_SETAUTOSURROUND     _IOW(PAPI_IOCTYPE_AF, 31, int)
#define PAPI_IO_AF_SOM_GETSURROUNDMODE     _IOW(PAPI_IOCTYPE_AF, 32, int)
#define PAPI_IO_AF_SOM_SETSURROUNDMODE     _IOW(PAPI_IOCTYPE_AF, 33, int)
#define PAPI_IO_AF_SOM_GETSURROUNDLEVEL     _IOW(PAPI_IOCTYPE_AF, 34, int)
#define PAPI_IO_AF_SOM_SETSURROUNDLEVEL     _IOW(PAPI_IOCTYPE_AF, 35, int)
#define PAPI_IO_AF_SOM_GETSURROUNDMODEEFFECTSTRENGTH     _IOW(PAPI_IOCTYPE_AF, 36, int)
#define PAPI_IO_AF_SOM_SETSURROUNDMODEEFFECTSTRENGTH     _IOW(PAPI_IOCTYPE_AF, 37, int)
#define PAPI_IO_AF_SOM_GETSURROUNDLEVELEFFECTSTRENGTH     _IOW(PAPI_IOCTYPE_AF, 38, int)
#define PAPI_IO_AF_SOM_SETSURROUNDLEVELEFFECTSTRENGTH     _IOW(PAPI_IOCTYPE_AF, 39, int)

#define PAPI_IO_AF_SLA_GETAVL                    _IOW(PAPI_IOCTYPE_AF, 40, int)
#define PAPI_IO_AF_SLA_SETAVL                    _IOW(PAPI_IOCTYPE_AF, 41, int)
#define PAPI_IO_AF_SLA_GETNIGHTMODE    _IOW(PAPI_IOCTYPE_AF, 42, int)
#define PAPI_IO_AF_SLA_SETNIGHTMODE     _IOW(PAPI_IOCTYPE_AF, 43, int)
#define PAPI_IO_AF_SLA_GETDRC                    _IOW(PAPI_IOCTYPE_AF, 44, int)
#define PAPI_IO_AF_SLA_SETDRC                    _IOW(PAPI_IOCTYPE_AF, 45, int)
#define PAPI_IO_AF_SIP_ISDRCALLOWED       _IOW(PAPI_IOCTYPE_AF, 46, int)

#define PAPI_IO_AF_SIP_GETIMPROVEMENTMODE    _IOW(PAPI_IOCTYPE_AF, 50, int)
#define PAPI_IO_AF_SIP_SETIMPROVEMENTMODE    _IOW(PAPI_IOCTYPE_AF, 51, int)
#define PAPI_IO_AF_SIP_GETLOUDNESS                   _IOW(PAPI_IOCTYPE_AF, 52, int)
#define PAPI_IO_AF_SIP_SETLOUDNESS                   _IOW(PAPI_IOCTYPE_AF, 53, int)

#define PAPI_IO_AF_SAD_ISADROUTINGALLOWED   _IOW(PAPI_IOCTYPE_AF, 70, int)
#define PAPI_IO_AF_SAD_SETROUTING      _IOW(PAPI_IOCTYPE_AF, 71, int)
#define PAPI_IO_AF_SAD_GETROUTING      _IOW(PAPI_IOCTYPE_AF, 72, int)
#define PAPI_IO_AF_SAD_IS_VOLCTRL_ALLOWED     _IOW(PAPI_IOCTYPE_AF, 73, int)
#define PAPI_IO_AF_SAD_SETVOLUME        _IOW(PAPI_IOCTYPE_AF, 74, int)
#define PAPI_IO_AF_SAD_GETVOLUME        _IOW(PAPI_IOCTYPE_AF, 75, int)

#define PAPI_IO_AF_SSOC_SET_CHN         _IOW(PAPI_IOCTYPE_AF, 80, int)
#define PAPI_IO_AF_SSOC_GET_CHN         _IOW(PAPI_IOCTYPE_AF, 81, int)
#define PAPI_IO_AF_SSOC_IS_STEREO       _IOW(PAPI_IOCTYPE_AF, 82, int)
#define PAPI_IO_AF_SSOC_IS_SUBLANG      _IOW(PAPI_IOCTYPE_AF, 83, int)
#define PAPI_IO_AF_SSOC_IS_MONO         _IOW(PAPI_IOCTYPE_AF, 84, int)
#define PAPI_IO_AF_SSOC_IS_DIGITAL         _IOW(PAPI_IOCTYPE_AF, 85, int)
#define PAPI_IO_AF_SSOC_IS_DPLII         _IOW(PAPI_IOCTYPE_AF, 86, int)
#define PAPI_IO_AF_SSOC_FRONTCH         _IOW(PAPI_IOCTYPE_AF, 87, int)
#define PAPI_IO_AF_SSOC_REARCH         _IOW(PAPI_IOCTYPE_AF, 88, int)
#define PAPI_IO_AF_SSOC_LFE         _IOW(PAPI_IOCTYPE_AF, 89, int)

#define PAPI_IO_AF_SOUND_INIT                            _IOW(PAPI_IOCTYPE_AF, 90, int)
#define PAPI_IO_AF_SOUND_CONNECT                     _IOW(PAPI_IOCTYPE_AF, 91, int)
#define PAPI_IO_AF_SOUND_DISCONNECT               _IOW(PAPI_IOCTYPE_AF, 92, int)

#define PAPI_IO_AF_SMT_MUTE_OUTPUT                 _IOW(PAPI_IOCTYPE_AF, 93, int) 
#define PAPI_IO_AF_SMT_GET_MUTE_STATUS         _IOW(PAPI_IOCTYPE_AF, 94, int)





#endif  // PAPI_IOCTL_AF_H

