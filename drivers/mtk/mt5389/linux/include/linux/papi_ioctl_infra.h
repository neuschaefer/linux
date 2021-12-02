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
 * $RCSfile: papi_ioctl_infra.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file papi_ioctl_papionly.h
 *  IO control of PAPI Frontend interface
 */

#ifndef PAPI_IOCTL_INFRA_H
#define PAPI_IOCTL_INFRA_H

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
// CEC callback command type
#define CEC_CB_ONRECEIVED                 0
#define CEC_CB_ONTRANSMITCOMPLETED        1

//  callback command type
#define CLK_CB_ONCLOCKCHANGED                 80
#define CLK_CB_ONALARM        81

// gio
#define PAPI_IO_INFRA_GIO_PINGETLATCH               _IOW(PAPI_IOCTYPE_INFRA, 0, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_GIO_PINGETLINE                _IOW(PAPI_IOCTYPE_INFRA, 1, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_GIO_PINGETMODE                _IOW(PAPI_IOCTYPE_INFRA, 2, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_GIO_PINSETLATCH               _IOW(PAPI_IOCTYPE_INFRA, 3, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_GIO_PINSETMODE                _IOW(PAPI_IOCTYPE_INFRA, 4, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_LED_SETBLINKING               _IOW(PAPI_IOCTYPE_INFRA, 5, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_LED_SETFLASH                  _IOW(PAPI_IOCTYPE_INFRA, 6, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_LED_SETPERMANENT              _IOW(PAPI_IOCTYPE_INFRA, 7, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_I2CC_MASTERENABLE              _IOW(PAPI_IOCTYPE_INFRA, 8, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_I2CC_MASTERDISABLE              _IOW(PAPI_IOCTYPE_INFRA, 9, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_I2CC_ENTERIDLEMODE              _IOW(PAPI_IOCTYPE_INFRA, 10, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_I2CC_LEAVEIDLEMODE              _IOW(PAPI_IOCTYPE_INFRA, 11, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_I2CC_GETCONTROLWORD              _IOW(PAPI_IOCTYPE_INFRA, 12, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_I2CC_SETBITRATE              _IOW(PAPI_IOCTYPE_INFRA, 13, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_I2CC_SETMASTERTIMEOUT              _IOW(PAPI_IOCTYPE_INFRA, 14, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_I2CM_READ              _IOW(PAPI_IOCTYPE_INFRA, 15, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_I2CM_WRITE              _IOW(PAPI_IOCTYPE_INFRA, 16, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_I2CM_WRITEREAD              _IOW(PAPI_IOCTYPE_INFRA, 17, IOCTL_6ARG_T)

// clk
#define PAPI_IO_INFRA_CLK_GETALARM              _IOW(PAPI_IOCTYPE_INFRA, 80, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CLK_ALARMISSET              _IOW(PAPI_IOCTYPE_INFRA, 81, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CLK_SETALARM              _IOW(PAPI_IOCTYPE_INFRA, 82, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CLK_CANCELALARM              _IOW(PAPI_IOCTYPE_INFRA, 83, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CLK_CLOCKISSET              _IOW(PAPI_IOCTYPE_INFRA, 84, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CLK_SETCLOCK              _IOW(PAPI_IOCTYPE_INFRA, 85, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_CLK_GETLOCALTIME              _IOW(PAPI_IOCTYPE_INFRA, 86, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_CLK_GETLOCALTIMEOFFSET              _IOW(PAPI_IOCTYPE_INFRA, 87, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_CLK_RUNDURINGSTANDBY              _IOW(PAPI_IOCTYPE_INFRA, 88, IOCTL_4ARG_T)
#define PAPI_IO_INFRA_CLK_SECONDSSINCEWAKEUP              _IOW(PAPI_IOCTYPE_INFRA, 89, IOCTL_6ARG_T)
#define PAPI_IO_INFRA_CLK_GETUNIVERSALTIME              _IOW(PAPI_IOCTYPE_INFRA, 90, IOCTL_6ARG_T)

// cec
#define PAPI_IO_INFRA_CECCTRL_SETVERSION            _IOW(PAPI_IOCTYPE_INFRA, 20, int)
#define PAPI_IO_INFRA_CECCTRL_GETVERSION            _IOW(PAPI_IOCTYPE_INFRA, 21, int)
#define PAPI_IO_INFRA_CECCTRL_SETOSDNAME            _IOW(PAPI_IOCTYPE_INFRA, 22, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CECCTRL_GETOSDNAME            _IOW(PAPI_IOCTYPE_INFRA, 23, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CECCTRL_SETDEVICEVENDORID		_IOW(PAPI_IOCTYPE_INFRA, 24, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CECCTRL_GETDEVICEVENDORID		_IOW(PAPI_IOCTYPE_INFRA, 25, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CECCTRL_SETLOGICALADDRESS		_IOW(PAPI_IOCTYPE_INFRA, 26, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CECCTRL_GETLOGICALADDRESS		_IOW(PAPI_IOCTYPE_INFRA, 27, IOCTL_2ARG_T)
#define PAPI_IO_INFRA_CECCTRL_ENABLE				_IOW(PAPI_IOCTYPE_INFRA, 28, int)
#define PAPI_IO_INFRA_CECCTRL_DISABLE				_IOW(PAPI_IOCTYPE_INFRA, 29, int)
#define PAPI_IO_INFRA_CECCTRL_ENABLERECEIVE			_IOW(PAPI_IOCTYPE_INFRA, 30, int)
#define PAPI_IO_INFRA_CECCTRL_DISABLERECEIVE		_IOW(PAPI_IOCTYPE_INFRA, 31, int)
#define PAPI_IO_INFRA_CECCTRL_ENABLETRANSMITSTAUTS	_IOW(PAPI_IOCTYPE_INFRA, 32, int)
#define PAPI_IO_INFRA_CECCTRL_DISABLETRANSMITSTAUTS	_IOW(PAPI_IOCTYPE_INFRA, 33, int)
#define PAPI_IO_INFRA_CECCTRL_GETBUFFEREDMSG		_IOW(PAPI_IOCTYPE_INFRA, 34, int)
#define PAPI_IO_INFRA_CECACK_MESSAGEPROCESSED		_IOW(PAPI_IOCTYPE_INFRA, 35, int)
#define PAPI_IO_INFRA_CECTX_TRANSMIT				_IOW(PAPI_IOCTYPE_INFRA, 36, int)
#define PAPI_IO_INFRA_CECCTRL_SETNFY				_IOW(PAPI_IOCTYPE_INFRA, 37, int)

#endif  // PAPI_IOCTL_INFRA_H

