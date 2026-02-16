/*
 *
 * ioctlApi.h
 * Description:
 *  This interface support the MIB-2 counters and management information
 *   for all device drivers
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _MIB_IOCTL_H_
#define _MIB_IOCTL_H_


typedef struct
{
    unsigned long cmd;
    unsigned long port;
    void *data;
} TI_SNMP_CMD_T;




/**************************************************************************
 *                 MIB-2 Common MIB Constants                             *
 **************************************************************************/
#define MIB2_TRUTH_VALUE_TRUE       1
#define MIB2_TRUTH_VALUE_FALSE      2

/**************************************************************************/
/*                 MIB-2 Interface Admin/Oper Status Values               */
/**************************************************************************/
/* Device is in operational status unless status is down. */
#define MIB2_STATUS_UP         1
#define MIB2_STATUS_DOWN       2
#define MIB2_STATUS_TEST       3
#define MIB2_STATUS_UNKNOWN    4
#define MIB2_STATUS_DORMANT    5

/**************************************************************************/
/* Definitions for Interface Group MIB variables                          */
/* GET command definitions                                                */
/**************************************************************************/
#define SIOCGINTFCOUNTERS     (0x1)  /* Get Interface counters */
#define SIOCGINTFPARAMS       (0x2)  /* Get Interface parameters */

/**************************************************************************/
/* SET command definitions                                                */
/**************************************************************************/
#define SIOCSINTFADMINSTATUS  (0x3) /* Set Interface Admin. status */

/************************************************************************/
/* Defines MIB II INTERFACE objects                                     */
/************************************************************************/
struct mib2_ifCounters
{
   unsigned long inBytesLow;          /* low 32-bit of total octets received from media */
   unsigned long inBytesHigh;         /* high 32-bit of total octets received from media */
   unsigned long inUnicastPktsLow;    /* low 32-bit of unicast packets delivered above */
   unsigned long inUnicastPktsHigh;   /* high 32-bit of unicast packets delivered above */
   unsigned long inMulticastPktsLow;  /* low 32-bit of muticast pkts delivered above */
   unsigned long inMulticastPktsHigh; /* high 32-bit of muticast pkts delivered above */
   unsigned long inBroadcastPktsLow;  /* low 32-bit of broadcast pkts delivered above */
   unsigned long inBroadcastPktsHigh; /* high 32-bit of broadcast pkts delivered above */
   unsigned long inDiscardPkts;       /* packets discarded due to resource limit */
   unsigned long inErrorPkts;         /* packets discarded due to format errors */
   unsigned long inUnknownProtPkts;   /* packets for unknown protocols */
   unsigned long outBytesLow;         /* low 32-bit of total octets sent on the media */
   unsigned long outBytesHigh;        /* high 32-bit of total octets sent on the media */
   unsigned long outUnicastPktsLow;   /* low 32-bit of unicast packets from above */
   unsigned long outUnicastPktsHigh;  /* high 32-bit of unicast packets from above */
   unsigned long outMulticastPktsLow; /* low 32-bit of multicast packets from above */
   unsigned long outMulticastPktsHigh;/* high 32-bit of multicast packets from above */
   unsigned long outBroadcastPktsLow; /* low 32-bit of broadcast packets from above */
   unsigned long outBroadcastPktsHigh;/* high 32-bit of broadcast packets from above */
   unsigned long outDiscardPkts;      /* packets discarded due to resource limit */
   unsigned long outErrorPkts;        /* packets discarded due to format errors */
};

struct mib2_ifParams
{
    unsigned long ifSpeed;            /* speed of the interface in bits per second */
    unsigned long ifHighSpeed;        /* speed of the interface in mega-bits per second */
    long          ifOperStatus;       /* current operational status */
    long          ifPromiscuousMode;  /* promiscuous mode interface status */
};

struct mib2_ifCommand
{
    long   ifAdminStatus;               /* desired interface state */
};

/**************************************************************************/
/*                   EtherLike-MIB Constants                              */
/**************************************************************************/
#define MIB2_UNKNOWN_DUPLEX        1
#define MIB2_HALF_DUPLEX           2
#define MIB2_FULL_DUPLEX           3

/**************************************************************************/
/* Definitions for Ether-Like Group MIB variables                         */
/* GET command definitions                                                */
/**************************************************************************/
#define SIOCGETHERCOUNTERS     (0x4) /* Get Emac PHY counters */
#define SIOCGETHERPARAMS       (0x5) /* Get Emac PHY parameters */

/************************************************************************/
/* Defines MIB II EtherLike-MIB  objects                                */
/************************************************************************/
struct mib2_phyCounters
{
   unsigned long ethAlignmentErrors;
   unsigned long ethFCSErrors;
   unsigned long ethSingleCollisions;
   unsigned long ethMultipleCollisions;
   unsigned long ethSQETestErrors;
   unsigned long ethDeferredTxFrames;
   unsigned long ethLateCollisions;
   unsigned long ethExcessiveCollisions;
   unsigned long ethInternalMacTxErrors;
   unsigned long ethCarrierSenseErrors;
   unsigned long ethTooLongRxFrames;
   unsigned long ethInternalMacRxErrors;
   unsigned long ethSymbolErrors;
};

struct mib2_ethParams
{
  long           ethDuplexStatus;        /* current Emac duplex status */
};

/**************************************************************************/
/*                   USB-MIB Constants                                    */
/**************************************************************************/
#define MIB2_USB_POWER_UNKNOWN        1
#define MIB2_USB_POWER_SELF           2
#define MIB2_USB_POWER_BUS            3
#define MIB2_USB_ACTIVE_CLASS_OTHER   1
#define MIB2_USB_ACTIVE_CLASS_CDC     2
#define MIB2_USB_STATUS_UNATTACHED    1
#define MIB2_USB_STATUS_ATTACHED      2
#define MIB2_USB_STATUS_POWERED       3
#define MIB2_USB_STATUS_DEFAULT       4
#define MIB2_USB_STATUS_ADDRESS       5
#define MIB2_USB_STATUS_CONFIGURED    6
#define MIB2_USB_STATUS_SUSPENDED     7
#define MIB2_USB_SUBCLASS_OTHER       0
#define MIB2_USB_SUBCLASS_DIRECTLINE  1
#define MIB2_USB_SUBCLASS_ACM         2
#define MIB2_USB_SUBCLASS_TELEPHONY   3
#define MIB2_USB_SUBCLASS_MCHANNEL    4
#define MIB2_USB_SUBCLASS_CAPI        5
#define MIB2_USB_SUBCLASS_ETHERNET    6
#define MIB2_USB_SUBCLASS_ATM         7
#define MIB2_USB_TX_TYPE_SYNC         1
#define MIB2_USB_TX_TYPE_ASYNC        2

/**************************************************************************/
/* Definitions for USB Group MIB variables                                */
/* GET command definitions                                                */
/**************************************************************************/
#define SIOCGUSBCOUNTERS     (0x6) /* Get USB PHY counters */
#define SIOCGUSBPARAMS       (0x7) /* Get USB PHY parameters */

/************************************************************************/
/* Defines MIB II USB-MIB  objects                                      */
/************************************************************************/
/* Note, that SIOCGUSBCOUNTERS may be the same as SIOCGETHERCOUNTERS    */
/* and use the same mib2_phyCounters structure defined for Ether-Like   */
/* At least 2 counters:                                                 */
/* ethAlignmentErrors and ethFCSErrors MUST be supported for USB        */
/************************************************************************/

struct mib2_usbParams
{
    long           usbDevicePower;
    unsigned long  usbDeviceVendorID;
    unsigned long  usbDeviceProductID;
    long           usbDeviceNumberConfigurations;
    long           usbDeviceActiveClass;
    long           usbDeviceStatus;
    unsigned long  usbDeviceNumEnumCounter;
    long           usbDeviceRemoteWakeup;
    long           usbDeviceRemoteWakeupOn;
    long           usbCDCSubclass;
    long           usbCDCDataTransferType;
    long           usbCDCEndpoints;
    unsigned long  usbCDCStalls;
    char           *usbCDCEtherPacketFilter;
    long           usbCDCEtherPacketFilterLength;
    char           *usbCDCEtherStatCapabilities;
    long           usbCDCEtherStatCapabilitiesLength;
    unsigned short usbCDCVersion;
};

#endif /* _MIB_IOCTL_H_ */





