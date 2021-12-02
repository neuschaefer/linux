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

#ifndef	__MUSB_TRANSPORT_TEST_H__
#define	__MUSB_TRANSPORT_TEST_H__

/*
 * MUSBStack-S "transport" test definitions.
 * $Revision: #1 $
 */

#include "mu_dev.h"

#define MUSB_TRANSPORT_VID	0x04d6
#define MUSB_TRANSPORT_PID	0xffff

/* sent as vendor-specific to other recipient */
#define MUSB_TRANSPORT_REQ_CONFIGURE_END    1

/*
* Command definitions
*/

#include "mu_pkon.h"
/**
 * Command to configure endpoint(s)
 * @field bType one of the MUSB_TRANSPORT_TYPE_* constants
 * @field bOptions one of the MUSB_TRANSPORT_OPTIONS_* constants
 * @field bmAttributes as per chapter 9, protocol only
 * @field bInOutEnds IN endpoint address (0 => no IN) in upper 4 bits;
 * OUT endpoint address (0 => no OUT) in lower 4 bits
 * @field bStartPattern starting data pattern (for source only)
 * @field bPacketCount packets per transfer
 * @field wMaxPacketSize maximum packet size
 */
typedef struct
{
    uint8_t bType;
    uint8_t bOptions;
    uint8_t bmAttributes;
    uint8_t bInOutEnds;
    uint8_t bStartPattern;
    uint8_t bPacketCount;
    uint16_t wMaxPacketSize;
} MUSB_TransportConfigureEnd;
#include "mu_pkoff.h"

#define MUSB_TRANSPORT_REQ_SET_NONE		0
#define MUSB_TRANSPORT_REQ_SET_LOOPBACK		1
#define MUSB_TRANSPORT_REQ_SET_SINK		2
#define MUSB_TRANSPORT_REQ_SET_SOURCE		3

#define MUSB_TRANSPORT_OPTIONS_NONE		0
#define MUSB_TRANSPORT_OPTIONS_INCREMENT	1
#define MUSB_TRANSPORT_OPTIONS_DECREMENT	2
#define MUSB_TRANSPORT_OPTIONS_ROTATE_LEFT	3
#define MUSB_TRANSPORT_OPTIONS_ROTATE_RIGHT	4

#define MUSB_M_TRANSPORT_IN_END			0x03
#define MUSB_S_TRANSPORT_IN_END			4
#define MUSB_M_TRANSPORT_OUT_END		0x03
#define MUSB_S_TRANSPORT_OUT_END		0

#endif	/* multiple inclusion protection */
