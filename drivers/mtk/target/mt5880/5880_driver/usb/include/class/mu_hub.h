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
 * MUSBStack-S hub definitions.
 * $Revision: #1 $
 */

#ifndef __MUSB_HUB_H__
#define __MUSB_HUB_H__

LINT_SUPPRESS_BRACE(621)            // Warning 621: Identifier clash

#ifndef MUSB_HUB_MAX_PORTS
#define MUSB_HUB_MAX_PORTS	4
#endif

/** Hub descriptor type */
#define MUSB_DT_HUB			(MUSB_TYPE_CLASS | 0x09)

/**
 * Hub descriptor as per USB 2.0 Specification, 11.23.2.1.
 * @field bLength length of this descriptor in bytes
 * @field bDescriptorType should be MUSB_DT_HUB
 * @field bNbrPorts how many ports on the hub
 * @field wHubCharacteristics characteristics bitmask
 * @field bPwrOn2PwrGood time (in 2ms units) between power on to
 * a port is good
 * @field bHubCtrlCurrent maximum current requirements of the
 * hub controller in mA
 * @field aRemovablePowerMasks the remaining bytes are variable-length:
 * <dl>
 * <dt>DeviceRemovable
 * <dd>The number of bytes necessary to accomodate bNbrPorts+1
 * (8 ports per byte); bit 0 of first byte is reserved;
 * other bits are 0 for removable-device port, 1 for permanent port
 * <dt>PortPowerCtrlMask
 * <dd>The number of bytes necessary to accomodate bNbrPorts
 * (8 ports per byte); all bits should be 1s
 * </ol>
 */
#include "mu_pkon.h"
typedef struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bNbrPorts;
    uint16_t wHubCharacteristics;
    uint8_t bPwrOn2PwrGood;
    uint8_t bHubCtrlCurrent;
    uint8_t aDeviceRemovable[(MUSB_HUB_MAX_PORTS / 8) + 1];
    uint8_t aPortPwrCtrlMask[(MUSB_HUB_MAX_PORTS / 8) + 1];
} MUSB_HubDescriptor;
#include "mu_pkoff.h"

/** Mask for the hub characteristics power field */
#define MUSB_M_HUB_CHARACTERISTICS_POWER	    0x0003
/** Shift for the hub characteristics power field */
#define MUSB_S_HUB_CHARACTERISTICS_POWER	    0
/** Ganged power switching value for the hub characteristics power field */
#define MUSB_HUB_CHARACTERISTICS_POWER_GANGED	    0
/** Individual port power switching value for the hub characteristics power field */
#define MUSB_HUB_CHARACTERISTICS_POWER_PORT	    1
#define MUSB_HUB_CHARACTERISTICS_POWER_RESERVED1    2
#define MUSB_HUB_CHARACTERISTICS_POWER_RESERVED2    3

/** Hub is part of a compound device */
#define MUSB_M_HUB_CHARACTERISTICS_COMPOUND	    0x0004

/** Mask for the hub characteristics over-current protection field */
#define MUSB_M_HUB_CHARACTERISTICS_OVERCURRENT	    0x0018
/** Shift for the hub characteristics over-current protection field */
#define MUSB_S_HUB_CHARACTERISTICS_OVERCURRENT	    3
/** Global overcurrent protection value for the hub characteristics overcurrent field */
#define MUSB_HUB_CHARACTERISTICS_OVERCURRENT_GLOBAL 0
/** Per-port overcurrent protection value for the hub characteristics overcurrent field */
#define MUSB_HUB_CHARACTERISTICS_OVERCURRENT_PORT   1

/** Mask for the hub characteristics TT think time field */
#define MUSB_M_HUB_CHARACTERISTICS_TT_TIME	    0x0060
/** Shift for the hub characteristics TT think time field */
#define MUSB_S_HUB_CHARACTERISTICS_TT_TIME	    5
/** TT requires <=8 FS bit times inter-transaction gap on full/low-speed downstream bus */
#define MUSB_HUB_CHARACTERISTICS_TT_TIME_8	    0
/** TT requires <=16 FS bit times inter-transaction gap on full/low-speed downstream bus */
#define MUSB_HUB_CHARACTERISTICS_TT_TIME_16	    1
/** TT requires <=24 FS bit times inter-transaction gap on full/low-speed downstream bus */
#define MUSB_HUB_CHARACTERISTICS_TT_TIME_24	    2
/** TT requires <=32 FS bit times inter-transaction gap on full/low-speed downstream bus */
#define MUSB_HUB_CHARACTERISTICS_TT_TIME_32	    3

/** Hub has port indicators (controlled by PORT_INDICATOR request) */
#define MUSB_M_HUB_CHARACTERISTICS_PORT_INDICATORS  0x0080

/**
 *  MUSB_HubPortStatus As per 11.24.2.7 USB 2.0 specification.
 *
 *  @field wStaus Tells about present status of the port as per 
 *                MUSB_PortStatusBits.
 * 
 *  @field wStatusChange Tells about which status bits have been
 *                       changed in the HUB port as per MUSB_PortChangeBits.
 */
typedef struct _MUSB_HubPortStatus_ 
{
    uint16_t wStatus;
    uint16_t wStatusChange;
} MUSB_HubPortStatus;

/** Feature selector used in SET and CLEAR feature request for Hub. */
typedef enum {
    MUSB_C_HUB_LOCAL_POWER_FEATURE          = 0x00,
    MUSB_C_HUB_OVER_CURRENT_FEATURE         = 0x01
}MUSB_HubFeatureSelector;

/** Feature selector used in SET and CLEAR feature request for Port. */
typedef enum {
    /** Port connection */
    MUSB_HUB_PORT_CONNECTION_FEATURE        = 0x0000, 
    /**  Port Enable     */
    MUSB_HUB_PORT_ENABLE_FEATURE            = 0x0001,
    /** Port suspended   */
    MUSB_HUB_PORT_SUSPEND_FEATURE           = 0x0002,
    /**  Over Current port basis */
    MUSB_HUB_PORT_OVER_CURRENT_FEATURE      = 0x0003,                 
    /** Port reset */
    MUSB_HUB_PORT_RESET_FEATURE             = 0x0004,
    /**  Power on state. */
    MUSB_HUB_PORT_POWER_FEATURE             = 0x0008,
    /** Low speed device is connected. */
    MUSB_HUB_PORT_LOW_SPEED_FEATURE         = 0x0009,

    MUSB_HUB_C_PORT_CONNECTION_FEATURE      = 0x0010,
    MUSB_HUB_C_PORT_ENABLE_FEATURE          = 0x0011,
    MUSB_HUB_C_PORT_SUSPEND_FEATURE         = 0x0012,
    MUSB_HUB_C_PORT_OVER_CURRENT_FEATURE    = 0x0013,                 
    MUSB_HUB_C_PORT_RESET_FEATURE           = 0x0014,
    MUSB_HUB_PORT_TEST_FEATURE              = 0x0015,
    MUSB_HUB_PORT_INDICATOR_FEATURE         = 0x0016
}MUSB_HubPortFeatureSelector;

/**
 * MUSB_HubStatusBits (wStatus in MUSB_HubStatus As per 11.24.2.6 USB 2.0 specification.)
 */
typedef enum {
    /** Local Power supply LOST */
    MUSB_HUB_LOCAL_POWER_SUPLY_LOST_BM    =  0x0001,
    /** Over current condition exists */
    MUSB_HUB_OVER_CURRENT_EXISTS_BM       =  0x0002
} MUSB_HubStatusBits;

/**
 * MUSB_HubChangeBits (wStatusChange in the MUSB_HubStatus As per 11.24.2.6 USB 2.0 specification). 
 */
typedef enum {
    /** Local Power supply status Changed */
    MUSB_HUB_LOCAL_POWER_STATUS_CHANGED_BM  = 0x0001,
    /** Over current condition status changed */
    MUSB_HUB_OVER_CURRENT_STATUS_CHANGED_BM = 0x0002
} MUSB_HubChangeBits;

/** MUSB_PortStatusBits (wStatus in the MUSB_HubPortStatus 
 *  As per 11.24.2.7.1 USB 2.0 specification). 
 */
typedef enum
{
    /**  Device is connected to the Hub's Port */
    MUSB_HUB_PORT_CONNECTION_BM    = 0x0001, 
    /**  Hub Port is Enable     */
    MUSB_HUB_PORT_ENABLE_BM        = 0x0002,
    /** Hub Port is suspended   */
    MUSB_HUB_PORT_SUSPEND_BM       = 0x0004,
    /**  When Port takes extra current. */
    MUSB_HUB_PORT_OVER_CURRENT_BM  = 0x0008,                 
    /** When host resets the attached device.  */
    MUSB_HUB_PORT_RESET_BM         = 0x0010,
    /**  Power on state. */
    MUSB_HUB_PORT_POWER_BM         = 0x0100,
    /** Low speed device is connected. */
    MUSB_HUB_PORT_LOW_SPEED_BM     = 0x0200,
    /** High speed device is connected. */
    MUSB_HUB_PORT_HIGH_SPEED_BM    = 0x0400,
    /** Port's test mode */
    MUSB_HUB_PORT_TEST_BM          = 0x0800,
    /**  Software control of the port indicator. */
    MUSB_HUB_PORT_INDICATOR_BM     = 0x1000
} MUSB_PortStatusBits;

/**
 * wStatusChange in the MUSB_HubPortStatus structure. 
 * As per 11.24.2.7.2 USB 2.0 specification. 
 */
typedef enum
{
    /** Connect status changed i.e. either device is attached
     * or detached. 
     */
    MUSB_HUB_C_PORT_CONNECTION_BM      = 0x0001,
    /** This bit set to 1 when port is disable */
    MUSB_HUB_C_PORT_ENABLE_BM          = 0x0002,
    /** This indicates changes of suspend state of 
     *  the attached device 
     */
    MUSB_HUB_C_PORT_SUSPEND_BM         = 0x0004,
    /** This field tells that over current indicator has changed.
     */
    MUSB_HUB_C_PORT_OVER_CURRENT_BM    = 0x0008,                 
    /** This bit is set when RESET processing in this port is complete
     */
    MUSB_HUB_C_PORT_RESET_BM           = 0x0010
} MUSB_PortChangeBits;

#endif	/* multiple inclusion protection */

