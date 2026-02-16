/*
 *
 * ddc_cpgmac_f.h 
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


/** \file   ddc_cpgmac_f.h
    \brief  DDC CPGMAC_F header file

    This file is the CPGMAC_F DDC header file to be included by the CPGMAC_F DDA
    driver files. This file is compliant to the PSP Framework 1.0 definitions
    and prototypes.

    \note When including this file from DDC source code, CPMAC_DDC must be
    defined.

    Documentation for #defines that may be included in makefiles:

    CPMAC_MULTIFRAGMENT - should be defined by the make file to support
      multifragment packets - single frag code not available yet
    CPMAC_DDC_DEBUG - should be defined by the makefile for compiling in debug
      statements
    CPMAC_DDC_GETSTATS - enables statistic counters

    Features not supported currently:
    1) Multipacket transmit and receive
    2) Tx interrupt disable mode

    @author     Greg Guyotte
 */

#ifndef __DDC_CPGMAC_F_H__
#define __DDC_CPGMAC_F_H__


#include <asm-arm/arch-avalanche/generic/pal.h> /* PAL SYS/OS services required */               
#include "ddc_netdev.h"         /* Inherit from Network device */
#include "ddc_cpgmac_f_Cfg.h"   /* DDC Configuration file for ready reference to some HW cfg macros */
#include "cslr_cpgmac_f.h"      /* CSL Header file */
#include "ddc_cpgmac_f_ioctl.h" /* Data structures required by Ioctls */
/* CPPI4 PAL interface header file */
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include "ioctl_api.h"

/**
 * \defgroup CPMAC_DDC_Interface    CPMAC DDC Interface
 *
 *  CPMAC DDC Layer Interface
 */
/*@{ */

/**
 *  \brief DDC CPMAC Error Codes
 *
 *  Refer to DDC Error codes description.
 */

/* Defining the macro CPMAC_INSTANCE_CODE to 0 so that it can be usable in DDA */
#define CPMAC_INSTANCE_CODE                     0       /* Arbitrary base */
#define CPMAC_ERROR_CODE                        ((DDC_ERROR | (CPMAC_INSTANCE_CODE << 16)) + DDC_NETDEV_ERROR_MAX)
#define CPMAC_ERROR_INFO                        (CPMAC_ERROR_CODE)
#define CPMAC_ERROR_WARNING                     (CPMAC_ERROR_CODE | 0x10000000)
#define CPMAC_ERROR_MINOR                       (CPMAC_ERROR_CODE | 0x20000000)
#define CPMAC_ERROR_MAJOR                       (CPMAC_ERROR_CODE | 0x30000000)
#define CPMAC_ERROR_CRITICAL                    (CPMAC_ERROR_CODE | 0x40000000)

/* CPMAC Success code */
#define CPMAC_SUCCESS                           PAL_SOK

/* CPMAC Error codes */
#define CPMAC_ERR_DEV_ALREADY_INSTANTIATED(instID) (0x30000000 + DDC_ERROR + DDC_NETDEV_ERROR_MAX + ((instId) << 16) )
#define CPMAC_ERR_DEV_NOT_INSTANTIATED          (CPMAC_ERROR_MAJOR + 1)
#define CPMAC_INVALID_PARAM                     (CPMAC_ERROR_MAJOR + 2)
#define CPMAC_ERR_TX_CH_INVALID                 (CPMAC_ERROR_CRITICAL + 3)
#define CPMAC_ERR_TX_CH_ALREADY_INIT            (CPMAC_ERROR_MAJOR + 4)
#define CPMAC_ERR_TX_CH_ALREADY_CLOSED          (CPMAC_ERROR_MAJOR + 5)
#define CPMAC_ERR_TX_NO_LINK                    (CPMAC_ERROR_MAJOR + 7)
#define CPMAC_ERR_TX_OUT_OF_BD                  (CPMAC_ERROR_MAJOR + 8)
#define CPMAC_ERR_RX_CH_INVALID                 (CPMAC_ERROR_CRITICAL + 9)
#define CPMAC_ERR_RX_CH_ALREADY_INIT            (CPMAC_ERROR_MAJOR + 10)
#define CPMAC_ERR_RX_CH_ALREADY_CLOSED          (CPMAC_ERROR_MAJOR + 11)
#define CPMAC_ERR_RX_CH_NOT_OPEN                (CPMAC_ERROR_MAJOR + 12)
#define CPMAC_ERR_DEV_NOT_OPEN                  (CPMAC_ERROR_MAJOR + 13)
#define CPMAC_ERR_DEV_ALREADY_CLOSED            (CPMAC_ERROR_MAJOR + 14)
#define CPMAC_ERR_DEV_ALREADY_OPEN              (CPMAC_ERROR_MAJOR + 15)
#define DDC_ERROR_INTERNAL_FAILURE              (CPMAC_ERROR_MAJOR + 16)
#define CPMAC_ERR_TX_CH_ALREADY_OPENED          (CPMAC_ERROR_MAJOR + 17)
#define CPMAC_ERR_RX_CH_ALREADY_OPENED          (CPMAC_ERROR_MAJOR + 18)
#define CPMAC_ERR_RX_BUFFER_ALLOC_FAIL          (CPMAC_ERROR_CRITICAL + 19)
#define CPMAC_ERR_CPPI_INIT			(CPMAC_ERROR_MAJOR + 20)
#define CPMAC_ERR_CPPI_TX_CH			(CPMAC_ERROR_MAJOR + 21)
#define CPMAC_ERR_CPPI_RX_CH			(CPMAC_ERROR_MAJOR + 22)
#define CPMAC_ERR_CPPI_DESC_REGN_FAIL		(CPMAC_ERROR_MAJOR + 23)

/**
 *  \brief CPMAC DDC Ioctl's
 *
 */
#define DDC_NET_CPMAC_IOCTL_BASE                0       /* Arbitrary base */
#define CPMAC_DDC_IOCTL_GET_SWVER               DDC_IOCTL(DDC_NET_IOCTL_MIN, 2)
#define CPMAC_DDC_IOCTL_GET_HWVER               DDC_IOCTL(DDC_NET_IOCTL_MIN, 3)
#define CPMAC_DDC_IOCTL_SET_RXCFG               DDC_IOCTL(DDC_NET_IOCTL_MIN, 4)
#define CPMAC_DDC_IOCTL_SET_MACCFG              DDC_IOCTL(DDC_NET_IOCTL_MIN, 5)
#define CPMAC_DDC_IOCTL_GET_STATUS              DDC_IOCTL(DDC_NET_IOCTL_MIN, 6)
#define CPMAC_DDC_IOCTL_READ_PHY_REG            DDC_IOCTL(DDC_NET_IOCTL_MIN, 7)
#define CPMAC_DDC_IOCTL_WRITE_PHY_REG           DDC_IOCTL(DDC_NET_IOCTL_MIN, 8)
#define CPMAC_DDC_IOCTL_GET_STATISTICS          DDC_NET_IOCTL_GET_NET_STATS
#define CPMAC_DDC_IOCTL_CLR_STATISTICS          DDC_NET_IOCTL_CLR_NET_STATS
#define CPMAC_DDC_IOCTL_MULTICAST_ADDR          DDC_IOCTL(DDC_NET_IOCTL_MIN, 9)
#define CPMAC_DDC_IOCTL_ALL_MULTI               DDC_IOCTL(DDC_NET_IOCTL_MIN, 10)
#define CPMAC_DDC_IOCTL_TYPE2_3_FILTERING       DDC_IOCTL(DDC_NET_IOCTL_MIN, 11)
#define CPMAC_DDC_IOCTL_SET_MAC_ADDRESS         DDC_IOCTL(DDC_NET_IOCTL_MIN, 12)
#define CPMAC_DDC_IOCTL_SET_SRC_MAC_ADDRESS     DDC_IOCTL(DDC_NET_IOCTL_MIN, 13)
#define CPMAC_DDC_IOCTL_IF_COUNTERS		DDC_IOCTL(DDC_NET_IOCTL_MIN, 14)
#define CPMAC_DDC_IOCTL_ETHER_COUNTERS     	DDC_IOCTL(DDC_NET_IOCTL_MIN, 15)
#define CPMAC_DDC_IOCTL_IF_PARAMS_UPDT  	DDC_IOCTL(DDC_NET_IOCTL_MIN, 16)

/**
 *  \brief CPMAC DDA Ioctl's
 *
 *  Called by the DDC layer, implemented by DDA layer in Control function
 */
#define DDA_NET_CPMAC_IOCTL_BASE                0       /* Arbitrary Base */
#define CPMAC_DDA_IOCTL_TIMER_START             (DDA_NET_CPMAC_IOCTL_BASE + 1)
#define CPMAC_DDA_IOCTL_TIMER_STOP              (DDA_NET_CPMAC_IOCTL_BASE + 2)
#define CPMAC_DDA_IOCTL_STATUS_UPDATE           (DDA_NET_CPMAC_IOCTL_BASE + 3)
#define CPMAC_DDA_IOCTL_TEARDOWN_PEND           (DDA_NET_CPMAC_IOCTL_BASE + 4)
#define CPMAC_DDA_IOCTL_MIB64_CNT_TIMER_START 	(DDA_NET_CPMAC_IOCTL_BASE + 5)
#define CPMAC_DDA_IOCTL_MIB64_CNT_TIMER_STOP 		(DDA_NET_CPMAC_IOCTL_BASE + 6)


/*
 * \brief MII module port settings
 *
 * DDA sets the Phy mode as a combination of the following in "phyMode" parameter
 * in the init configuration structure
 */
#define SNWAY_AUTOMDIX      (1<<16)     /* Bit 16 and above not used by MII register */
#define SNWAY_FD1000        (1<<13)
#define SNWAY_HD1000        (1<<12)
#define SNWAY_NOPHY         (1<<10)
#define SNWAY_LPBK          (1<<9)
#define SNWAY_FD100         (1<<8)
#define SNWAY_HD100         (1<<7)
#define SNWAY_FD10          (1<<6)
#define SNWAY_HD10          (1<<5)
#define SNWAY_AUTO          (1<<0)
#define SNWAY_AUTOALL       (SNWAY_AUTO|SNWAY_FD1000|SNWAY_FD100|SNWAY_FD10|SNWAY_HD100|SNWAY_HD10)

/**
 *  DDC Status Ioctl - Error status
 *
 *  Note that each error code is a bit position so that multiple errors can be
 *  clubbed together and passed in a integer value
 */
#define CPMAC_DDC_NO_ERROR          0   /**< Ioctl Success */
#define CPMAC_DDC_TX_HOST_ERROR     0x1 /**< TX Host Error - "hwErrInfo" MSB 8 bits indicate "error code""channel no" */
#define CPMAC_DDC_RX_HOST_ERROR     0x2 /**< RX Host Error - "hwErrInfo" LSB 8 bits indicate "error code""channel no" */

/**
 *  \brief DDC Status values
 *
 * Provides status of the device - error status, phy status etc
 *
 */
typedef struct {
    Uint32 hwStatus;              /**< Either NO_ERROR or combination of error bits from above status codes */
    Uint32 hwErrInfo;             /**< If error, then additional info about the error */
    Uint32 PhyLinked;             /**< Link status: 1=Linked, 0=No link */
    Uint32 PhyDuplex;             /**< Duplex status: 1=Full Duplex, 0=Half Duplex */
    Uint32 PhySpeed;              /**< Link Speed = 1=100 mbps, 0=10 Mbbs. (In future this is expected: 10/100/1000 mbps) */
    Uint32 PhyNum;                /**< Phy number - useful if phy number is discovered */
} CpmacDDCStatus;


/**
 *  \brief CPMAC Channel Config Info
 *
 *  Common to both TX/RX.  Used to pass channel config info from DDA to DDC for
 *  CPMAC channels.  Note that the caller of ChOpen is required to fill the
 *  given CPPI4 channel structures with data.
 */
typedef struct {
    Int chNum;                      /**< Channel number */
    DDC_NetChDir chDir;             /**< Channel direction */
    DDC_NetChState chState;         /**< Channel state */
    Int numBD;                      /**< Number of descriptors */
    Int descAlignment;              /**< Alignment requirment for descriptors.
                                      Assuming same sized Tx and Rx
                                      descriptors. */
    Int dmaMode;                    /**< DMA mode of channel InfraMode or Entpoint mode */
    Int bufSize;                    /**< Buffer Size (applicable for RX only) */
    Int serviceMax;                 /**< Maximum BD's processed in one go */
    Int numTxQs;                    /**< Total number of tx queues
                                      supported by the driver to
                                      prioritize tx */
    Int accVecNum;                  /**< Accumulator interrupt vector */
    Cppi4TxChInitCfg cppi4TxChInfo; /**< CPPI4 Tx Ch Parameters */
    Cppi4RxChInitCfg cppi4RxChInfo; /**< CPPI4 Rx Ch Parameters */
    Cppi4AccumulatorCfg txAccChInfo; /**< CPPI4 Tx Accumulator channel parameters */
    Cppi4AccumulatorCfg rxAccChInfo; /**< CPPI4 Rx Accumulator channel parameters */
} CpmacChInfo;

/**
 *  \brief CPMAC RX configuration
 *
 *  This data structure contains configuration items related to the CPMAC
 *  receive side.
 */
typedef struct {
    Uint32 hiPriThresh;                   /**< High Priority Threshold */
    Bool copyMACControlFramesEnable;      /**< Copy MAC Control frames to packet memory */
    Bool copyShortFramesEnable;           /**< Copy Short frames to packet memory */
    Bool copyErrorFramesEnable;           /**< Copy Errored frames to packet memory */
    Bool promiscousEnable;                /**< Copy ALL (Promiscous) frames to packet memory */
    Uint32 promiscousChannel;             /**< Promiscous receive channel */
    Bool broadcastEnable;                 /**< Receive broadcast frames */
    Uint32 broadcastChannel;              /**< Broadcast receive channel */
    Bool multicastEnable;                 /**< Receive multicast frames */
    Uint32 multicastChannel;              /**< Multicast receive channel */
    Uint32 maxRxPktLength;                /**< Max receive packet length */
    Uint32 rxFlowThreshold;               /**< Receive flow control threshold */
} CpmacRxConfig;

/**
 *  \brief CPMAC MAC configuration
 *
 *  This data structure contains configuration items largely related to the
 *  Mac Control register fields, as well as the value for the port VLAN register.
 */
typedef struct {
    Bool rxVlanEn;                        /**< Receive VLAN enable */
    Bool extEn;                           /**< External enable */
    Bool gigForce;                        /**< Force Gigabit mode */
    Bool ifCtlB;                          /**< general purpose output GMII */
    Bool ifCtlA;                          /**< general purpose output GMII */
    Bool rxFifoFlowEnable;                /**< rx fifo flow control enable (CFIG3 only) */
    Bool cmdIdle;                         /**< enable / disable idle command*/
    Bool txShortGapEnable;                /**< Short Gap enable on transmit - CP(G)MAC only */
    Bool txPacingEnable;                  /**< Transmit pacing enabled ? */
    Bool txFlowEnable;                    /**< TX Flow Control enabled ? */
    Bool rxFlowEnable;                    /**< TX Flow Control enabled ? */
    Bool loopbackEnable;                  /**< Loopback mode enabled ? */
    Int portVlan;                         /**< configured port VLAN register */
} CpmacMacConfig;

/**
 *  \brief CPMAC Init Configuration
 *
 *  Configuration information provided to DDC layer during initialization.
 *  DDA gets the config information from the OS/PAL layer and passes the relevant
 *  config to the DDC during initialization. The config info can come from
 *  various sources - static compiled in info, boot time (ENV, Flash) info etc.
 */
typedef struct {
    Uint32 instId;                /**< DDC Init Cfg: Instance number */
    Uint32 numTxChannels;         /**< DDCNet Init Cfg: Number of Tx Channels to be supported */
    Uint32 numRxChannels;         /**< DDCNet Init Cfg: Number of Rx Channels to be supported */
    Uint32 cpmacBusFrequency;     /**< Bus frequency at which this module is operating */
    Uint32 baseAddress;           /**< CPMAC peripheral device's register overlay address */
    Uint32 rxMaxFrags;            /**< Maximum number of rx fragments to support - must be at least 1 */
    Uint32 mdioBaseAddress;       /**< MDIO Device base address */
    Uint32 mdioResetLine;         /**< MDIO Device Reset line number within the system */
    Uint32 PhyMask;               /**< Phy Mask for this CPMAC Phy  */
    Uint32 MLinkMask;             /**< MLink Mask for this CPMAC Phy  */
    Uint32 MdioBusFrequency;      /**< Bus frequency for the MII module */
    Uint32 MdioClockFrequency;    /**< Clock frequency for MDIO link */
    Uint32 TxIntrLine;                  /**< Tx completion queue interrupt */
    Uint32 RxIntrLine;                                  /**< Rx Queue interrupt */
    Uint32 MdioTickMSec;          /**< DDC MDIO Tick count in milliSeconds */
    Uint32 Mib64CntMsec;
    Uint32 phyMode;               /**< Phy mode settings - Speed,Duplex */
    CpmacRxConfig rxCfg;          /**< RX common configuration */
    CpmacMacConfig macCfg;        /**< MAC common configuration */
    Cppi4InitCfg cppi4InitCfg; /**< CPPI4 PAL Initialization Config */
} CpmacInitConfig;

/*
 *  The CPMAC DDC object internals are hidden from the upper layers. Hence the
 *  internal data structure for CpmacDDCObj is in the DDC internal header and the
 *  upper layers interpret DDC_Handle as a Ptr.  All DDC files must be compiled
 *  with the switch "CPMAC_DDC" so that the DDC can interpret a DDC_Handle as
 *  a data structure.
 */
#ifndef CPMAC_DDC
typedef DDC_Handle CpmacDDCObj;
#endif

/********************** CPGMAC_F DDC External API Documentation ****************/

/**
 *  \brief DDC_CpmacTick
 *
 *  Used to check the device status and report state changes.
 *  DDC may use ControlCb() to set the tick time period based upon its needs.
 *  The existing CPMAC hardware supports MDIO interrupt, but is not working and
 *  hence a periodic poll function needs to check MDIO state and status. The DDA
 *  may provide a periodic timeout using OS services.
 *  Note: This can be done using PAL software timer service.
 *
 *  @param  hDDC        DDC Handle
 *  @param  tickArgs    Arguments if any, else NULL. Not used in current
 *                      implementation.
 *  @return PAL_SUCCESS or PAL Error code
 */
typedef PAL_Result(*DDC_CpmacTick) (CpmacDDCObj * hDDC, Ptr tickArgs);

/**
 *  \brief DDC_CpmacTxPktProcess
 *
 *  DDA calls the packet packet processing function when the device interrupt is
 *  pending.  DDA typically calls this function in a thread context.
 *
 *  @param  hDDC        DDC Handle
 *  @param  channel     Interrupt channel number
 *  @param  pktsPending Not currently used.
 *  @param  pktArgs     Not currently used.
 *  @return Number of packets processed
 */
typedef Int(*DDC_CpmacTxPktProcess) (CpmacDDCObj * hDDC,
                                     Int * pktsPending, Ptr pktArgs);

/**
 *  \brief DDC_CpmacRxPktProcess
 *
 *  DDA calls the Rx packet processing function when there are pending Rx packets
 *  to be processed.  DDA typically calls this function in a thread context.
 *
 *  @param  hDDC        DDC Handle
 *  @param  channel     Channel number
 *  @param  pktsPending Not currently used.
 *  @param  pktArgs     Not currently used.
 *  @return Number of packets processed
 */
typedef Int(*DDC_CpmacRxPktProcess) (CpmacDDCObj * hDDC,
                                     Int * pktsPending, Ptr pktArgs);


/**
 *  \brief CPMAC Add RX Buffer-BD function
 *
 *  When the DDA receive IOCTL to add RX buffers DDC allocates additional RX buffer.
 * (additional to the RX BD which were allocate during Open)
 *  When a buffer is allocated, the BD ptr is passed along to the DDA RX buffer allocation function
 *  which makes sure that when the received packet is processed.
 *
 *  @param  hDDC         DDC Handle
 *  @param  chInfo       Channel number
 *  @param  numOfBd2Add  Number of RX Buffer Descriptor to add
 *  @return Success or Failure if the allocation faild.
 */
typedef Int(*DDC_CpmacAddRxBd) (CpmacDDCObj * hDDC,
                                CpmacChInfo * chInfo,
                                unsigned int numOfBd2Add);


/**
 *  \brief CPMAC Packet Processing End Function (to be called by the DDA)
 *
 *  DDA processes packets using the TX and RX packets processing functions. When no packets are
 *  pending or the desired number of packets is processed, device level interrupt must be enabled.
 *  This call triggers the device level interrupt logic to raise an interrupt when new  packets have
 *  arrived or are still pending to be processed.
 *
 *  @param  hDDC        DDC Handle
 *  @param  procArgs    Arguments if any, else NULL. Kept for future enhancement.
 *  @return Success or Failure. Currently since this will be a reg write only, success is returned.
 */
typedef Int(*DDC_CpmacPktProcessEnd) (CpmacDDCObj * hDDC, Ptr procArgs);


typedef Int(*DDC_CpmacTxPktProcessEnd) (CpmacDDCObj * hDDC, Ptr procArgs);

/**
 * \brief CPMAC DDC Interface Structure
 *
 * CPMAC DDC Interface object - inherits interfaces from network device
 */
typedef struct {
    /* Always super class members first */
    DDC_NetFuncTable ddcNetIf;           /**< DDC and Net function table */

  /** DDC CPMAC specific functions */
    DDC_CpmacTick ddctick;               /**< DDC CPMAC Tick function */
    DDC_CpmacPktProcessEnd pktProcessEnd;        /**< DDC CPMAC End of packet processing function */
    DDC_CpmacTxPktProcessEnd txPktProcessEnd;      /**< DDC CPMAC End of packet processing function */
    DDC_CpmacTxPktProcess txPktProcess;  /**< DDC CPMAC pkt processing func */
    DDC_CpmacRxPktProcess rxPktProcess;  /**< DDC CPMAC pkt processing func */
    DDC_CpmacAddRxBd AddRxBd;                              /**< DDC CPMAC add Rx BD function */
} CpmacDDCIf;

/**
 *  \brief Debug printf / Error log s function (provided by DDA, used by DDC for debug prints
 *
 *  DDA provides this callback function to be used by DDC layer for debug printing and error logging.
 */
typedef Int(*DDA_Printf) (const char *format, ...);
typedef Int(*DDA_ErrLog) (const char *format, ...);

/**
 * \brief DDA Callback Interface Structure
 *
 * Callback functions provided by the DDA layer to be called by DDC.
 * Inherits from Network device callback functions
 */
typedef struct {
    /* Always super class members first */
    DDA_NetFuncTable ddaNetIf;           /**< Net function table,*/
    /* includes DDC level DDA Cb table */

    /* Debug printf and error functions */
    DDA_Printf ddaPrintf;                /**< DDA debug printing function */
    DDA_ErrLog ddaErrLog;                /**< DDA error logging function */
} CpmacDDACbIf;

/**
 *  \brief DDC_cpmacGetVersionInfo
 *
 *  This function returns the version information of the DDC implementation for
 *  CPMAC device. The version number is returned in the pointer parameter passed
 *  to the function and the string is returned as the return value of the
 *  function.
 *
 *  @param  swVer       Placeholder for returning Version Id (major/minor
 *                      version).  Major Version Id (MSB 16 bits), minor version
 *                      Id (LSB 16 bits)
 *  @return Returns version string (array of char's stored in DDC)
 */
String DDC_cpmacGetVersionInfo(Uint32 * swVer);

/**
 *  \brief DDC_cpmacCreateInstance
 *
 *  CPMAC Instance creation function. DDA/DDC Handle and interface pointer
 *  exchange happens here.
 *
 *  @param  instId          Instance identifier
 *  @param  hDDA            Handle to the DDA layer
 *  @param  hDDACbIf        Handle to the DDA Callback interface structure
 *  @param  hDDC            Placeholder for Handle to the DDC layer
 *  @param  hDDCIf          Placeholder for Handle to the DDC interface structure
 *  @param  param           Not used in this implementation. For future use.
 *  @return Success or failure (PAL success/error code)
 */
PAL_Result DDC_cpmacCreateInstance(Uint32 instId, DDA_Handle hDDA,
                                   CpmacDDACbIf * hDDACbIf,
                                   DDC_Handle ** hDDC,
                                   CpmacDDCIf ** hDDCIf, Ptr param);

/**
 *  \brief DDC_cpmacGetPhyDev
 *
 *  Returns pointer to an internal PhyDev structure.  This should be moved to
 *  Control().
 *
 *  @param  hDDC            DDC Handle.
 *  @return Pointer to PhyDev structure.
 */
Ptr DDC_cpmacGetPhyDev(CpmacDDCObj * hDDC);


#ifdef CPMAC_DDC_GETSTATS
void cpmacDDCStats(Uint32 instId, Uint8 txQueue, Uint8 rxQueue);
#endif

/*@} */

typedef struct {
    Uint32 rxPkts;              /* Number of rx pkts to be processed */
    Uint32 txPkts;              /* Number of tx pkts to be processed */

    Uint32 retRxPkts;           /* Number of rx pkts processed */
    Uint32 retTxPkts;           /* Number of tx pkts processed */

    Uint32 rxMaxService;        /* Number of max rx that can be serviced */
    Uint32 txMaxService;        /* Number of max tx that can be serviced */
} RxTxParams;

/* Added by Hardik since now this function is called from DDA layer 
 *  and there is not function pointer for this
 */
#if defined(CONFIG_MACH_PUMA5EVM) 
void cpmacSetMacHwCfg(CpmacDDCObj * hDDC);
#endif

#endif /* __DDC_CPGMAC_F_H__ */
