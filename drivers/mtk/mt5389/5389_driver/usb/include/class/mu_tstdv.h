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
 * MUSBStack-S compliance test device definitions.
 * $Revision: #1 $
 */

#ifndef	__MUSB_COMPLIANCE_DEVICE_H__
#define	__MUSB_COMPLIANCE_DEVICE_H__

#include "mu_tools.h"

/*
* Definitions for USB compliance test device
*/

/** subclass for programmable compliance devices */
#define MUSB_SUBCLASS_COMPLIANCE	0x01

/*
* Command definitions
*/

/** change endpoint characteristics */
#define MUSB_DIAG_REQ_VEN_CHANGE_EP		0x51

/**
 * (optional) disconnect:
 * wValue.MSB=seconds before disconnect (1-60);
 * wValue.LSB=seconds between disconnect/re-connect (1-60)
 */
#define MUSB_DIAG_REQ_VEN_DO_DISCONNECT		0x52

/**
 * get entire endpoint capabilities table 
 * (first byte returned is length of table)
 */
#define MUSB_DIAG_REQ_VEN_GET_EP_TABLE		0x53

/** (optional) set device PID */
#define MUSB_DIAG_REQ_VEN_SET_PID		0x54

/** remote wakeup (wValue=seconds from suspend to remote wakeup) */
#define MUSB_DIAG_REQ_VEN_SET_RMTWK_TO		0x55

/** read endpoint-specific count (4 bytes) */
#define MUSB_DIAG_REQ_VEN_GET_COUNT		0x58

/** read firmware/FPGA version (4 bytes) */
#define MUSB_DIAG_REQ_VEN_GET_VERSION		0x59

/*
 * (high-speed only; optional) set chirp behavior:
 * wValue=chirp delay, microsecs [1-6000]
 * wIndex=chirp duration, microsecs [500-8000]
 */
#define MUSB_DIAG_REQ_VEN_SET_CHIRP		0x5A

/** write arbitrary data on control pipe */
#define MUSB_DIAG_REQ_VEN_CONROL_WRITE_DATA	0x5B

/** read last data written on control pipe */
#define MUSB_DIAG_REQ_VEN_CONROL_READ_DATA	0x5C

#define MUSB_DIAG_REQ_VEN_GET_EP_BUFFERSIZE	0x5D

/** valid */
#define MUSB_S_EPCHANGE_BFLAGS_VALID			0
/** do loopback */
#define MUSB_S_EPCHANGE_BFLAGS_LOOPBACK			1
/** endpoint type */
#define MUSB_S_EPCHANGE_BFLAGS_TYPE			2
/** number of additional transaction opportunities per micro-frame */
#define MUSB_S_EPCHANGE_BFLAGS_EXTRATRANSACTIONS	4

/** normal response mode */
#define MUSB_DIAG_RESPONSE_NORMAL   0
/** always STALL */
#define MUSB_DIAG_RESPONSE_STALL    1
/** always NAK */
#define MUSB_DIAG_RESPONSE_NAK	    2
/** always time out */
#define MUSB_DIAG_RESPONSE_TIMEOUT  4
/** always CRC error */
#define MUSB_DIAG_RESPONSE_CRCERROR 8

#define MUSB_S_DIAG_ATTIBUTES_HIGHBANDWIDTH	0
#define MUSB_S_DIAG_ATTIBUTES_LOOPBACK		1
#define MUSB_S_DIAG_ATTIBUTES_PERFECTSOURCESINK	2
#define MUSB_S_DIAG_ATTIBUTES_TRANSACTIONCOUNT	5

#include "mu_pkon.h"

/**
 * MUSB_EndpointChangeData (for MGC_USB_REQ_VEN_CHANGE_EP).
 * @field bFlags flags; see MUSB_S_EPCHANGE_FLAGS_*
 * @field r0 reserved for future use
 * @field wm_MaxPacketSize the max, modifiable
 * @field dm_Pattern base data pattern by device
 * @field dm_Modifer data pattern modifier
 * @field bPeriod for interrupt, the poll interval
 * @field bFixedRespMode fixed response mode, see MUSB_DIAG_RESPONSE_*
 * @field r1 reserved for future use
 */
typedef struct
{
    uint8_t bFlags;
    uint8_t r0;
    uint16_t wm_MaxPacketSize;
    uint32_t dm_Pattern;
    uint32_t dm_Modifer;
    uint8_t bPeriod;
    uint8_t bFixedRespMode;
    uint16_t r1;
} MUSB_DiagEndpointChangeData;
#include "mu_pkoff.h"

#include "mu_pkon.h"
/**
 * Transfer type capability
 * @field bValid TRUE if transfer type is supported
 * @field bMaxExtraBuffer max for this transfer type
 * @field wMaxPacketSize max for this transfer type
 */
typedef struct
{
    uint8_t bValid;
    uint8_t bMaxExtraBuffer;
    uint16_t wMaxPacketSize;
} MUSB_DiagTransferTypeCapability;
#include "mu_pkoff.h"

#include "mu_pkon.h"
/**
 * Diagnostic device capabilities
 * @field bAddress endpoint address ([3:0]) and direction ([7])
 * @field bLoopbackIndex index of loopback partner
 * @field bmAttributes see MUSB_S_DIAG_ATTIBUTES_*
 * @field bFixedRespCaps for setting fixed response mode
 * @field BulkCapabilities capabilities as a bulk endpoint
 * @field InterruptCapabilities capabilities as an interrupt endpoint
 * @field IsochCapabilities capabilities as an isochronous endpoint
 * @field ControlCapabilities capabilities as a control endpoint
 */
typedef struct 
{
    uint8_t bAddress;
    uint8_t bLoopbackIndex;
    uint8_t bmAttributes;
    uint8_t bFixedRespCaps;
    MUSB_DiagTransferTypeCapability BulkCapabilities;
    MUSB_DiagTransferTypeCapability InterruptCapabilities;
    MUSB_DiagTransferTypeCapability IsochCapabilities;
    MUSB_DiagTransferTypeCapability ControlCapabilities;
} MUSB_DiagCapabilities;
#include "mu_pkoff.h"

#include "mu_pkon.h"
/**
 * Diagnostic device capabilities
 * @field bLength number of table entries
 * @field Table variable-length table
 */
typedef struct
{
    uint8_t bLength;
    MUSB_DiagCapabilities Table[1];
} MUSB_DiagCapabilityTable;

#include "mu_pkoff.h"

#endif	/* multiple inclusion protection */
