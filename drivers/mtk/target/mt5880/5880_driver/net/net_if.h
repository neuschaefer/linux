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
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
* 
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: net_if.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Network driver interface header file
 *
 * Author:
 * -------
 *   Hs Huang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

/**************************************************************************** 
Network Driver: Interface 
****************************************************************************/

#ifndef X_NET_IF_H
#define X_NET_IF_H

#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"

#include "u_net_drv_if.h"
#include "u_net_oid.h"

/****************************************************************************
** Constant definitions
****************************************************************************/
#define CONFIG_WLAN_WPD 0
#define CONFIG_DTCP 0
#define CONFIG_CHKSUM 1

#define NET_SUSPEND             0xFFFFFFFFUL    /* network OS suspend */
#define NET_DRV_DBG             1


/* network driver ID */
typedef enum
{
    DRVID_NET_DOCSIS_RFI,
    DRVID_NET_ETHERNET_802_3,
    DRVID_NET_WIRELESS_802_11,
    DRVID_NET_USB,
    DRVID_NET_SERIAL,
    DRVID_NET_LOOPBACK
} NET_DRV_ID_T;


/****************************************************************************
** Structure and Type definitions
****************************************************************************/

//-----------------------------------------------------------------------------
// funtcion error type
//-----------------------------------------------------------------------------
#define E_NET_OK                            ((INT32)    0)
#define E_NET_NOT_INIT                      ((INT32)   -1)
#define E_NET_ALREADY_INIT                  ((INT32)   -2)
#define E_NET_INV_SOURCE                    ((INT32)   -3)
#define E_NET_INV_SET_INFO                  ((INT32)   -4)
#define E_NET_STATE_MISMATCH                ((INT32)   -5)
#define E_NET_OUT_OF_RESOURCE               ((INT32)   -6)
#define E_NET_NOT_ALLOC_RESOURCE            ((INT32)   -7)

#define E_NET_INV_ARG                       ((INT32)   -10)
#define E_NET_INV_ARG1                      ((INT32)   -11)
#define E_NET_INV_ARG2                      ((INT32)   -12)
#define E_NET_INV_ARG3                      ((INT32)   -13)
#define E_NET_INV_ARG4                      ((INT32)   -14)
#define E_NET_INV_ARG5                      ((INT32)   -15)
#define E_NET_INV_ARG6                      ((INT32)   -16)

#define E_NET_NO_EFFECT                     ((INT32)   -1000)
#define E_NET_GENERAL_FAIL                  ((INT32)   -10000)


#define NET_DRV_NO_RESOURCE                 ((INT32)   -15)
#define NET_DRV_NO_ASSOCICATION             ((INT32)   -16)


#if 0
// Copy from linux Errno-base.h
#define	EPERM		 			1	/* Operation not permitted */
#define	ENOENT		 			2	/* No such file or directory */
#define	ESRCH		 			3	/* No such process */
#define	EINTR		 			4	/* Interrupted system call */
#define	EIO		 	 			5	/* I/O error */
#define	ENXIO		 			6	/* No such device or address */
#define	E2BIG		 			7	/* Argument list too long */
#define	ENOEXEC		 			8	/* Exec format error */
#define	EBADF		 			9	/* Bad file number */
#define	ECHILD					10	/* No child processes */
#define	EAGAIN					11	/* Try again */
#define	ENOMEM					12	/* Out of memory */
#define	EACCES					13	/* Permission denied */
#define	EFAULT					14	/* Bad address */
#define	ENOTBLK					15	/* Block device required */
#define	EBUSY					16	/* Device or resource busy */
#define	EEXIST					17	/* File exists */
#define	EXDEV					18	/* Cross-device link */
#define	ENODEV					19	/* No such device */
#define	ENOTDIR					20	/* Not a directory */
#define	EISDIR					21	/* Is a directory */
#define	EINVAL					22	/* Invalid argument */
#define	ENFILE					23	/* File table overflow */
#define	EMFILE					24	/* Too many open files */
#define	ENOTTY					25	/* Not a typewriter */
#define	ETXTBSY					26	/* Text file busy */
#define	EFBIG					27	/* File too large */
#define	ENOSPC					28	/* No space left on device */
#define	ESPIPE					29	/* Illegal seek */
#define	EROFS					30	/* Read-only file system */
#define	EMLINK					31	/* Too many links */
#define	EPIPE					32	/* Broken pipe */
#define	EDOM					33	/* Math argument out of domain of func */
#define	ERANGE					34	/* Math result not representable */
#endif

/****************************************************************************
** External variable
****************************************************************************/

/****************************************************************************
** Export Macro
****************************************************************************/

/****************************************************************************
** External Network API, from wlan_os.h temporally
****************************************************************************/
void vNetTestGpioOut(INT32 i4Val);
void vNetTestGpioToggle(void);
void vNetTestGpioInit(void);
void vNetDelay1us(UINT32 u4Value);
void *pvNetAlignAddr(void *pAddr, UINT32 u4Align);
UINT32 u4NetGetTimeTick(void);
INT32 MW_NET_Init(void);

#endif /* _NET_IF_H_ */
