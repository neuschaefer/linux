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
 *   $Workfile: ether_com.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet common header file
 *
 * Author:
 * -------
 *   HsHuang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

#ifndef _ETHER_COM_H_
#define _ETHER_COM_H_

#include "x_bim.h"
#include "x_timer.h"
#include "x_common.h"
//#include "x_hal_1176.h"
#include "x_hal_926.h"
#include "x_net_drv_if.h"

#include "net_if.h"
#include "../net_debug.h"
#include "ether_priv.h"
#include "ether_reg.h"
#include "ether_phy.h"
#include "ether_hal.h"

#define STORE_MAC_ADDRESS_TO_FLASH 	0
//#define SCATTER_TEST 1
//#define  DRV_LOOP_BACK_TEST 1
#define INTERNAL_PHY 1 //1
#define ALIGNMENT_16BYTES_TX_BUFF_TEST 0
#if (INTERNAL_PHY)
#define DEBUG_MII_PIN 0
#define USE_EXT_CLOCK_SOURCE 1
#endif

#ifdef __KERNEL__
#define x_sscanf sscanf
#endif
#define CONFIG_DRV_LINUX_DATA_CONSISTENCY 0
/****************************************************************************
** External ethernet API
****************************************************************************/
#if 0
extern BOOL  GET_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status);
extern void  SET_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status);
extern void  CLR_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status);
#endif

/* The macro reply device status information */
#define GET_ETHER_INFO(prMac, Status)  \
	    (BOOL)(prMac->u4DevStatus & Status)

/* The macro set device status information */
#define SET_ETHER_INFO(prMac, Status)  \
    ETHER_SEMA_LOCK(prMac->hDevStatusSema);\
    (prMac->u4DevStatus |= Status);\
    ETHER_SEMA_UNLOCK(prMac->hDevStatusSema)

/* The macro clear device status information */
#define CLR_ETHER_INFO(prMac, Status)  \
    ETHER_SEMA_LOCK(prMac->hDevStatusSema);\
    (prMac->u4DevStatus &= ~Status);\
    ETHER_SEMA_UNLOCK(prMac->hDevStatusSema)    

/****************************************************************************
** Constant definitions
****************************************************************************/
/* Buffer and Descriptor size */
#ifdef SCATTER_TEST //only for verification
#define	PKT_BUFFER_SIZE             256    /* The size is better be 128-bit aligned and must 

                                               be larger than 1518 byte */

#else
#define	PKT_BUFFER_SIZE             1536    /* The size is better be 128-bit aligned and must 

                                               be larger than 1518 byte */
#endif                                               

#define MAC_CRC_LEN         		4     	/* MAC CRC length */
#define MAC_IP_CHKSUM_LEN   		2     	/* MAC IP checksum length */
#define ETHER_HEADER_LEN    		14    	/* ethernet header length */
#define ETHER_VALID_PKT_LEN    		1518   	/* valid ethernet packet length */

#define ETHER_MAX_ADDR_LEN			6		/* Largest hardware address length */
#define ETHER_MAX_MULTICAST_NUM		15		/* Largest number of multicast list */


/* ethernet device status information */
typedef enum 
{
    ETHER_DEV_INIT      =   ((UINT32)0x01 << 0),    /* device driver is init or not */
    ETHER_DEV_START     =   ((UINT32)0x01 << 1),    /* device driver is started */
	ETHER_LINK_UP		=	((UINT32)0x01 << 2),	/* net device link up or not */
    ETHER_DEV_RDY       =   ((UINT32)0x01 << 3),    /* device driver is ready for tx/rx */
    RX_TO_RECV_PKT      =   ((UINT32)0x01 << 4),    /* rx buffer has packets and need to receive it */
    RX_BUF_OVERRUN      =   ((UINT32)0x01 << 5)     /* rx buffer is overrun */
} ETHER_DEV_INFO_T;

/****************************************************************************
** Macro definitions
****************************************************************************/
#define ETHER_NONCACHE				NONCACHE
#define ETHER_CACHE				    CACHE

/* The macro is used to indicate 6 byte mac address */
#define ETH_MAC2STR(a)  (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]

#endif // _ETHER_COM_H_
