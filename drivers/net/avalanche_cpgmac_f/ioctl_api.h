/*
 *
 * ioctlApi.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


/**************************************************************************/
 
/**************************************************************************/ 
    /*                                                                        */ 
    /*   MODULE:  ioctlApi.h                                                  */ 
    /*   PURPOSE: Device MIB support functions.                               */ 
    /*                                                                        */ 
    /*   This interface support the MIB-2 counters and management information */ 
    /*   for all device drivers                                               */ 
/**************************************************************************/ 
    
#ifndef _IOCTL_API_H_
#define _IOCTL_API_H_
    
/**************************************************************************
 *                 MIB-2 Common MIB Constants                             *
 **************************************************************************/ 
#define MIB2_TRUTH_VALUE_TRUE  		1
#define MIB2_TRUTH_VALUE_FALSE  	2
    
/**************************************************************************/ 
    /*                 MIB-2 Interface Admin/Oper Status Values               */ 
/**************************************************************************/ 
    /* Device is in operational status unless status is down. */ 
#define MIB2_STATUS_UP     	   1
#define MIB2_STATUS_DOWN   	   2
#define MIB2_STATUS_TEST   	   3
#define MIB2_STATUS_UNKNOWN	   4
#define MIB2_STATUS_DORMANT	   5
    
#ifndef OS_IOCTL_OFFSET  /*OS specific IOCTL_OFFSET, To be defined in the OS specific files */
#define OS_IOCTL_OFFSET 0
#endif /*  */
    
/**************************************************************************/ 
    /* Definitions for Interface Group MIB variables                          */ 
    /* GET command definitions                                                */ 
/**************************************************************************/ 
#define TI_SIOCGINTFCOUNTERS     (OS_IOCTL_OFFSET) + 0x01 /* Get Interface counters */
#define TI_SIOCGINTFPARAMS       (OS_IOCTL_OFFSET) + 0x02 /* Get Interface parameters */
    
/**************************************************************************/ 
    /* SET command definitions                                                */ 
/**************************************************************************/ 
#define TI_SIOCSINTFADMINSTATUS  (OS_IOCTL_OFFSET) + 0x03 /* Set Interface Admin. status */
    
/**************************************************************************/ 
    /* Definitions for Ether-Like Group MIB variables                         */ 
    /* GET command definitions                                                */ 
/**************************************************************************/ 
#define TI_SIOCGETHERCOUNTERS     (OS_IOCTL_OFFSET) +0x04 /* Get Emac PHY counters */
#define TI_SIOCGETHERPARAMS       (OS_IOCTL_OFFSET) +0x05 /* Get Emac PHY parameters */
    
/************************************************************************/ 
    /* Defines MIB II INTERFACE objects                                     */ 
/************************************************************************/ 
struct mib2_ifCounters  {
    unsigned long inBytesLow;  /* low 32-bit of total octets received from media */
    unsigned long inBytesHigh; /* high 32-bit of total octets received from media */
    unsigned long inUnicastPktsLow;    /* low 32-bit of unicast packets delivered above */
    unsigned long inUnicastPktsHigh;   /* high 32-bit of unicast packets delivered above */
    unsigned long inMulticastPktsLow;  /* low 32-bit of muticast pkts delivered above */
    unsigned long inMulticastPktsHigh; /* high 32-bit of muticast pkts delivered above */
    unsigned long inBroadcastPktsLow;  /* low 32-bit of broadcast pkts delivered above */
    unsigned long inBroadcastPktsHigh; /* high 32-bit of broadcast pkts delivered above */
    unsigned long inDiscardPkts;       /* packets discarded due to resource limit */
    unsigned long inErrorPkts; /* packets discarded due to format errors */
    unsigned long inUnknownProtPkts;   /* packets for unknown protocols */
    unsigned long outBytesLow; /* low 32-bit of total octets sent on the media */
    unsigned long outBytesHigh;        /* high 32-bit of total octets sent on the media */
    unsigned long outUnicastPktsLow;   /* low 32-bit of unicast packets from above */
    unsigned long outUnicastPktsHigh;  /* high 32-bit of unicast packets from above */
    unsigned long outMulticastPktsLow; /* low 32-bit of multicast packets from above */
    unsigned long outMulticastPktsHigh;        /* high 32-bit of multicast packets from above */
    unsigned long outBroadcastPktsLow; /* low 32-bit of broadcast packets from above */
    unsigned long outBroadcastPktsHigh;        /* high 32-bit of broadcast packets from above */
    unsigned long outDiscardPkts;      /* packets discarded due to resource limit */
    unsigned long outErrorPkts;        /* packets discarded due to format errors */
};
struct mib2_ifHCCounters  {
    struct mib2_ifCounters Mib2IfCounter;
    unsigned long long inBytesHC;
    unsigned long long inUnicastPktsHC;
    unsigned long long inMulticastPktsHC;
    unsigned long long inBroadcastPktsHC;
    unsigned long long outBytesHC;
    unsigned long long outUnicastPktsHC;
    unsigned long long outMulticastPktsHC;
    unsigned long long outBroadcastPktsHC;
    unsigned long long inBytes;
    unsigned long long inUnicastPkts;
    unsigned long long inMulticastPkts;
    unsigned long long inBroadcastPkts;
    unsigned long long outBytes;
    unsigned long long outUnicastPkts;
    unsigned long long outMulticastPkts;
    unsigned long long outBroadcastPkts;
};
struct mib2_ifParams  {
    unsigned long ifSpeed;     /* speed of the interface in bits per second */
    unsigned long ifHighSpeed; /* speed of the interface in mega-bits per second */
    long ifOperStatus;         /* current operational status */
    long ifPromiscuousMode;    /* promiscuous mode interface status */
};
struct mib2_ifCommand  {
    long ifAdminStatus;        /* desired interface state */
};

/**************************************************************************/ 
    /*                   EtherLike-MIB Constants                              */ 
/**************************************************************************/ 
#define	MIB2_UNKNOWN_DUPLEX     1
#define	MIB2_HALF_DUPLEX 	2
#define	MIB2_FULL_DUPLEX	3
    
/************************************************************************/ 
    /* Defines MIB II EtherLike-MIB  objects                                */ 
/************************************************************************/ 
struct mib2_phyCounters  {
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
struct mib2_ethParams  {
    long ethDuplexStatus;      /* current Emac duplex status */
};

#endif /*  */
