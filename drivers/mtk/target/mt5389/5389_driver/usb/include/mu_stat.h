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

/*
 * MUSBStack-S status codes.
 * $Revision: #1 $
 */

#ifndef __MUSB_STATUS_H__
#define __MUSB_STATUS_H__

/*
 * Status codes (bit 7 means error)
 */
#define MGC_M_STATUS_ERROR		0x80
#define MGC_M_STATUS_WARNING		0x40

/*
 * Macros to test status type
 */

/**
 * Test code for error.
 * @param _code_ status code
 * @return TRUE if code represents an error; FALSE otherwise
 */
#define MUSB_STATUS_ISERROR(_code_) (_code_ & MGC_M_STATUS_ERROR)

/**
 * Test code for warning.
 * @param _code_ status code
 * @return TRUE if code represents a warning; FALSE otherwise
 */
#define MUSB_STATUS_ISWARNING(_code_) (_code_ & MGC_M_STATUS_WARNING)

/**
 * Test code for success.
 * @param _code_ status code
 * @return TRUE if code represents success; FALSE otherwise
 */
#define MUSB_STATUS_ISOK(_code_) (0 == (_code_ & (MGC_M_STATUS_ERROR | MGC_M_STATUS_WARNING)))

/** Successful status */
#define MUSB_STATUS_NOERROR		0

/** Successful status */
#define MUSB_STATUS_SUCCESS		0

/** Successful status */
#define MUSB_STATUS_OK			0

/** Connect child device status unknow */
#define MUSB_STATUS_CHILD_UNKNOW  (0x0)

/** Successful connect child device */
#define MUSB_STATUS_CHILD_CONNECT   (0x01)

/** Successful disconnect child device */
#define MUSB_STATUS_CHILD_DISCONNECT   (0x02)

/** Do not support connect child device */
#define MUSB_STATUS_CHILD_CONNECT_UNSUPPORT   (0x03)

/** Do not support disconnect child device */
#define MUSB_STATUS_CHILD_DISCONNECT_UNSUPPORT   (0x04)

/** IRP was started, but is not complete (pending) */
#define MUSB_STATUS_IRP_PENDING		0x10

/** warning: device not supported (OTG targetted peripheral list) */
#define MUSB_STATUS_UNSUPPORTED_DEVICE	(MGC_M_STATUS_WARNING | 0x01)

/** warning: hub not supported (OTG targetted peripheral list) */
#define MUSB_STATUS_UNSUPPORTED_HUB	(MGC_M_STATUS_WARNING | 0x02)

/** warning: not supported device is removed (OTG targetted peripheral list) */
#define MUSB_STATUS_UNSUPPORTED_REMOVE	(MGC_M_STATUS_WARNING | 0x03)

/** error: IRP timeout */
#define MUSB_STATUS_TIMEOUT		(MGC_M_STATUS_ERROR | 0x01)

/** error: NACK limit reached */
#define MUSB_STATUS_NACK_LIMIT		(MGC_M_STATUS_ERROR | 0x02)

/** error: ACK timeout */
#define MUSB_STATUS_ACK_TIMEOUT		(MGC_M_STATUS_ERROR | 0x03)

/** error: unsupported operation requested */
#define MUSB_STATUS_UNSUPPORTED		(MGC_M_STATUS_ERROR | 0x04)

/** error: the requested endpoint is busy (e.g. pending IRP prevents starting request) */
#define MUSB_STATUS_ENDPOINT_BUSY	(MGC_M_STATUS_ERROR | 0x05)

/** error: short transfer */
#define MUSB_STATUS_SHORT_TRANSFER	(MGC_M_STATUS_ERROR | 0x06)

/** error: internal error */
#define MUSB_STATUS_INTERNAL_ERROR	(MGC_M_STATUS_ERROR | 0x07)

/** error: general transmit error */
#define MUSB_STATUS_TRANSMIT_ERROR	(MGC_M_STATUS_ERROR | 0x08)

/** error: general receive error */
#define MUSB_STATUS_RECEIVE_ERROR	(MGC_M_STATUS_ERROR | 0x09)

/** error: CRC error */
#define MUSB_STATUS_CRC_ERROR		(MGC_M_STATUS_ERROR | 0x10)
/** error: bit-stuffing error */
#define MUSB_STATUS_BITSTUFF_ERROR	(MGC_M_STATUS_ERROR | 0x11)
/** error: data overrun */
#define MUSB_STATUS_DATA_OVERRUN	(MGC_M_STATUS_ERROR | 0x12)
/** error: data underrun */
#define MUSB_STATUS_DATA_UNDERRUN	(MGC_M_STATUS_ERROR | 0x13)
/** error: buffer overrun */
#define MUSB_STATUS_BUFFER_OVERRUN	(MGC_M_STATUS_ERROR | 0x14)
/** error: buffer underrun */
#define MUSB_STATUS_BUFFER_UNDERRUN	(MGC_M_STATUS_ERROR | 0x15)
/** error: DMA error */
#define MUSB_STATUS_DMA_ERROR		(MGC_M_STATUS_ERROR | 0x16)
/** error: received USB stall handshake */
#define MUSB_STATUS_STALLED		(MGC_M_STATUS_ERROR | 0x17)

/** error: transfer aborted because peer was removed from bus */
#define MUSB_STATUS_PEER_REMOVED	(MGC_M_STATUS_ERROR | 0x20)
/** error: babble detected */
#define MUSB_STATUS_BABBLING_HOST	(MGC_M_STATUS_ERROR | 0x21)
/** error: out of bandwidth */
#define MUSB_STATUS_NO_BANDWIDTH	(MGC_M_STATUS_ERROR | 0x23)
/** error: bad traffic type */
#define MUSB_STATUS_INVALID_TYPE	(MGC_M_STATUS_ERROR | 0x24)
/** error: out of resources (usually local ends) */
#define MUSB_STATUS_NO_RESOURCES	(MGC_M_STATUS_ERROR | 0x25)
/** error: out of memory */
#define MUSB_STATUS_NO_MEMORY		(MGC_M_STATUS_ERROR | 0x26)
/** error: inconsistency in descriptors */
#define MUSB_STATUS_BAD_DESCRIPTORS	(MGC_M_STATUS_ERROR | 0x27)
/** error: invalid argument */
#define MUSB_STATUS_INVALID_ARGUMENT	(MGC_M_STATUS_ERROR | 0x28)

/*
 *	Added for OTG
 */

/** error: invalid Vbus detected */
#define MUSB_STATUS_OTG_VBUS_INVALID	(MGC_M_STATUS_ERROR | 0x30)

/** Device did not respond (OTG requires this be signalled to user */
#define MUSB_STATUS_OTG_NO_RESPONSE	(MGC_M_STATUS_ERROR | 0x31)

/** SRP failed (OTG requires this be signalled to user */
#define MUSB_STATUS_OTG_SRP_FAILED	(MGC_M_STATUS_ERROR | 0x32)

#endif	/* multiple inclusion protection */
