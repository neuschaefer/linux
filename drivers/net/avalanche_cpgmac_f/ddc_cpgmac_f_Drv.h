/*
 *
 * ddc_cpgmac_f_Drv.h
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


/** \file   ddc_cpgmac_f_Drv.h
    \brief  CPMAC DDC internal header file

    This file contains data structures and interfaces internal to CPGMAC_F
    specific DDC implementation.  This file is compliant to the PSP Framework
    1.0 definitions and prototypes.

    @author     Greg Guyotte
 */


#ifndef __DDC_CPGMAC_F_DRV_H__
#define __DDC_CPGMAC_F_DRV_H__

/*
 * This file will be included ONLY by ALL CPMAC DDC "implementation" files and
 * hence must define CPMAC_DDC macro to get the right definition of CpmacDDCObj
 */
#define CPMAC_DDC

/**
 * \defgroup CPMAC_DDC_Internal CPMAC DDC Internal
 *
 *  CPMAC DDC Layer Internal Data Structures/Functions
 */
/*@{ */

/* Required for hiding internal object name behind the public name */
typedef struct _CpmacDDCObj_t CpmacDDCObj;

/* Data structures and header files required for MII-MDIO module  */
typedef struct _phy_device PHY_DEVICE;

#include "ddc_cpgmac_f.h"       /* DDC CPMAC Interface */
#include "cpswhalcommon_stddef.h"       /* Required for MII module defns */
#include "cpswhalcommon_miimdio.h"
#if defined CPGMAC_USE_ACC_LIST || !defined CPMAC_POLL_MODE

#include <asm-arm/arch-avalanche/generic/avalanche_intd.h>
#endif

/**
 *  \brief CPGMAC_F DDC Debug flags
 *
 *  The makefile must #define CPMAC_DDC_DEBUG in order to enable debug.
 *  IMPORTANT NOTE: The debug flags need to be enabled carefully as it could
 *  flood the console/sink point of the debug traces and also affect the
 *  functionality of the overall system
 */
#define CPMAC_DEBUG_FUNCTION_ENTRY          (0x1 << 1)
#define CPMAC_DEBUG_FUNCTION_EXIT           (0x1 << 2)
#define CPMAC_DEBUG_BUSY_FUNCTION_ENTRY     (0x1 << 3)
#define CPMAC_DEBUG_BUSY_FUNCTION_EXIT      (0x1 << 4)
#define CPMAC_DEBUG_TX                      (0x1 << 5)
#define CPMAC_DEBUG_RX                      (0x1 << 6)
#define CPMAC_DEBUG_PORT_UPDATE             (0x1 << 7)
#define CPMAC_DEBUG_MII                     (0x1 << 8)
#define CPMAC_DEBUG_TEARDOWN                (0x1 << 9)
#define CPMAC_DEBUG_GENERAL                 (0x1 << 10)

#ifdef CPMAC_DDC_DEBUG
#define CPMAC_DDC_LOGERR(format, args...)         hDDC->ddaIf->ddaErrLog(format, ##args);
#define CPMAC_DDC_LOGMSG(flag, format, args... )  { if (flag & CpmacDDCDebug) hDDC->ddaIf->ddaPrintf(format, ## args); }
#else
#define CPMAC_DDC_LOGERR(format, args...)
#define CPMAC_DDC_LOGMSG(flag, format, args... )
#endif

#ifdef CPMAC_INCLUDE_ASSERT
#define assert(handle_parent, expr) \
    if(!(expr)) {                                           \
        handle_parent->hDDC->ddaIf->ddaErrLog( "Assertion failed! %s,%s,%s,line=%d\n",    \
            #expr,__FILE__,__FUNCTION__,__LINE__); BUG (); }
#else
#define assert(handle_parent, expr)
#endif

/**
 *  Host descriptor structure. 
 * Note: This structure can represent either Host Packet Descriptor or Host 
 * Buffer Descriptor at a time, so validity of some fields depends on the 
 * type of descriptor.
 */
typedef volatile struct {
    /* hardware descriptor fields from this point */
    Cppi4HostDesc  hwDesc;

    /* software only fields from this point */
    Ptr nextSwBDPtr;
    Ptr dataPtr;        /**< DataPtr (virtual address) of the buffer allocated */
    Ptr bufToken;       /**< Placeholder for a pointer to a token */
    Ptr txEopBD;        /**< Pointer to end of packet BD (used on TX int) */
    Ptr pktToken;       /**< Used to store a packet token on Send */
    Uint32 numFrags;
} CpmacHostDesc;

/**
 *  Embedded descriptor structure. 
 */

typedef volatile struct {
    /* hardware descriptor fields from this point */
    Cppi4EmbdDesc  hwDesc;
    /* software only fields from this point */
	UINT32 ProtSpecific[3];
} CpmacEmbdDesc;


typedef struct {
    Char *bdMemPtr;                   /**< Pointer to descriptor block */
    Uint32 bdMemSize;                 /**< Size of descriptor block */
    Uint32 numBd;                     /**< Number of descriptors in block */
    struct BdMemChunk *ptrNext;  /**< Pointer to next mem chunk */
} BdMemChunk;

/* Forward declaration */
typedef struct _CpmacRxCppiCh_t CpmacRxCppiCh;

/**
 *  \brief TX Channel Control Structure
 *
 *  Used by CPMAC DDC code to process TX Buffer Descriptors
 */
typedef struct {
    CpmacChInfo chInfo;               /**< Channel config/info */
    Uint32 allocSize;                 /**< Tx BD pool allocated memory size */
    Char *bdMem;                      /**< Tx BD Memory pointer */
    CpmacHostDesc* bdPoolHead;        /**< Free BD Pool Head */
   
    Ptr palCppi4Hnd;                  /**< Holds CPPI4 handle needed for push
                                        pop APIs */ 
    Ptr cppi4TxChHnd;                 /**< CPPI4 Tx Channel Handle */
    PAL_Cppi4AccChHnd txAccChHnd;     /**<  acccumulator channel handle */
    Uint32 numBD;                     /** Number of Free descriptors available */
    
    /* 
     * CPPI specific information needed by the driver. Note that most of this
     * info comes from Cppi4TxChInfo which is filled during initialization
     */
    Uint32 numTxQs;                       /**< Number of Tx queues available. */
    Cppi4Queue txQueue[CPGMAC_MAX_TX_QS]; /**< Queues used for transmit. Can
                                             schedule the queuing among
                                             numTxCmplQs for prioritization */
    PAL_Handle txQueueHnd[CPGMAC_MAX_TX_QS];/**< Array of handles to tx queues */
    
    /* !@@ TODO: Confirm that there will be only 1 tx completion queue. in
     * that case, numTxCmplQs and usage of arrays can be avoided
     */
    Cppi4Queue txCmplQueue;     /**< Tx completion queue */
    PAL_Handle txCmplQueueHnd;  /**< Handle to tx compl queue */

    Ptr listBuffBase[CPGMAC_ACC_LIST_DIV];  /**< Contains array of list region
                                              bases divided according the
                                              ping-pong buffer size */ 
    Uint32* listEntryPtr;                   /**< Pointer to list entry.
                                              Points to unread entry in the
                                              list after interrupts start. */ 
    Uint32 activeListRgn;                   /**< Indicates which of the
                                              CPGMAC_ACC_LIST_DIV regions of
                                              the list area are to be used by
                                              the driver. The range is 0 to
                                              (CPGMAC_ACC_LIST_DIV-1) and updated
                                              per EOI */
    Uint32 accListSize;                     /**< Total size in bytes of the
                                              list region */
    
    Uint32 accChNum;            /**< Accumulator channel number */
    Uint32 accVecNum;           /**< Accumulator interrupt vector */
            
#ifdef CPMAC_DDC_GETSTATS
    Uint32 outOfTxBD;                 /**< out of tx bd errors */
#endif

#ifdef CPMAC_INCLUDE_ASSERT
   CpmacDDCObj * hDDC; 
#endif
} CpmacTxCppiCh;

/**
 *  \brief RX Channel Control Structure
 *
 *  Used by CPMAC DDC code to process RX Buffer Descriptors
 */
typedef struct _CpmacRxCppiCh_t {
    CpmacChInfo chInfo;               /**< Channel config/info */
    DDC_NetPktObj *pktQueue;          /**< Array of NetPktObj for encapsulating
				       received packet data and sending to DDA */
    DDC_NetBufObj *bufQueue;          /**< Array of NetBufObj for storing buffer
				       information, part of NetPktObj */
    BdMemChunk *MemChunk;
    BdMemChunk *BdMemList;            /**< Pointer to linked list of memory chunk structs */
    Ptr palCppi4Hnd;                  /**< Holds CPPI4 handle needed for push
                                        pop APIs */ 
    Ptr cppi4RxChHnd;                 /**< CPPI4 Rx Channel Handle */
    PAL_Cppi4AccChHnd rxAccChHnd;     /**<  acccumulator channel handle */    
    
    /* 
     * CPPI specific information needed by the driver. Note that most of this
     * info comes from Cppi4RxChInfo which is filled during initialization.
     *
     * !@@ TODO: Need to add members for Embedded and Monolithic too. 
     */
    Cppi4Queue rxQueue;                         /**< Receive queue */
    PAL_Handle rxQueueHnd;                      /**< Handle to receive queue */
    
    Cppi4Queue fdbQueue[CPGMAC_NUM_RXFDB_QS];   /**< Free desc/buffer queues
                                                  used for host packets */
    PAL_Handle fdbQueueHnd[CPGMAC_NUM_RXFDB_QS];/**< Array of handles to free
                                                  desc/buffer queues */

    Ptr listBuffBase[CPGMAC_ACC_LIST_DIV];  /**< Contains array of list region
                                              bases divided according the
                                              ping-pong buffer size */ 
    Uint32* listEntryPtr;                   /**< Pointer to list entry.
                                              Points to unread entry in the
                                              list after interrupts start. */ 
    Uint32 activeListRgn;                   /**< Indicates which of the
                                              CPGMAC_ACC_LIST_DIV regions of
                                              the list area are to be used by
                                              the driver. The range is 0 to
                                              (CPGMAC_ACC_LIST_DIV-1) and updated
                                              per EOI */
    Uint32 accListSize;                     /**< Total size in bytes of the
                                              list region */

    Uint32 accChNum;            /**< Accumulator channel number */
    Uint32 accVecNum;           /**< Accumulator interrupt vector */
            
    /* CPMAC (ethernet) specific configuration info */
    char macAddr[6];                  /**< Ethernet MAC address */

#ifdef CPMAC_DDC_GETSTATS
    Uint32 outOfRxBuffers;            /**< NO buffers available */
#endif

#ifdef CPMAC_INCLUDE_ASSERT
   CpmacDDCObj * hDDC; 
#endif
} _CpmacRxCppiCh;

/**
 *  \brief CPMAC DDC object
 *
 *  CPMAC DDC layer Object - encapsulates all bookeeping and data structure for
 *  CPMAC DDC.
 */
typedef struct _CpmacDDCObj_t {
  /** DDC generic parameters */
    DDC_Obj ddcObj;                /**< Contains ver, inst id and state */

  /** CPMAC specific parameters - DDC device specifics */
    Ptr cppi4PAL;                       /**< CPPI4 PAL handle */
    CpmacInitConfig initCfg;            /**< Initialization Configuration */
    CpmacTxCppiCh *txCppi;              /**< Tx Control structure pointer */
    CpmacRxCppiCh *rxCppi;              /**< Rx Control structure pointer */
#ifdef CONFIG_ARM_AVALANCHE_PPD
    CpmacTxCppiCh *infraTxCppi;
    CpmacRxCppiCh *epRxCppi;
#endif
    Bool txIsCreated;                   /**< TX Channel created ? */
    Bool rxIsCreated;                   /**< RX Channel created ? */
    Bool txIsOpen;                      /**< TX channel opened ? */
    Bool rxIsOpen;                      /**< RX channel opened ? */
    Bool txTeardownPending;             /**< Is TX teardown pending ? */
    Bool rxTeardownPending;             /**< Is RX teardown pending ? */
    Uint32 txIntLine;                   /**< Tx interrupt line number */
    Uint32 rxIntLine;                   /**< Rx interrupt line number */
    CpmacDDCStatus status;              /**< hardware status */
    Uint32 multicastHashCnt[CPMAC_NUM_MULTICAST_BITS];          /**< Number of multicast hash bits used in hardware */
    Uint32 RxAddrType;                  /**< Addr Type: 0 (CPMAC), 1 or 2 (CPGMAC) */
    Char macAddr[CPMAC_MAX_NUM_ADDRESSES][6];             /**< Ethernet MAC addresses */
    Char macSrcAddr[6];                 /**< Mac address used on TX pause frames */
    PHY_DEVICE *PhyDev;                 /**< MII-MDIO module device structure */
    CSL_Cpgmac_f_RegsOvly regs;         /**< Pointer points to CPMAC Base addr */
    struct mib2_ifHCCounters Mib2IfHCCounter;
#ifdef CPMAC_DDC_GETSTATS
    Uint32 rxIntCount[CPPI4_NUM_RX_POP_QUEUES];                  /**< # of times CpmacRxBDProc called */
    Uint32 txIntCount[CPPI4_NUM_TX_POP_QUEUES];                  /**< # of times CpmacTxBDProc called */
    Uint32 rxEmptyIntCount[CPPI4_NUM_RX_POP_QUEUES];             /**< # of interrupts with nothing to do */
    Uint32 txEmptyIntCount[CPPI4_NUM_TX_POP_QUEUES];             /**< # of interrupts with nothing to do */
#endif

    /* Register Mirror values - maintained to avoid costly register reads */
    Uint32 Rx_Unicast_Set;              /**< Unicast Set Register */
    Uint32 Rx_Unicast_Clear;            /**< Unicast Clear Register */
    Uint32 Rx_MBP_Enable;               /**< RX MBP Register */
    Uint32 MacHash1;                    /**< MAC Hash 1 Register */
    Uint32 MacHash2;                    /**< MAC Hash 2 Register */
    Uint32 MacControl;                  /**< MACControl Register */

    /* Function tables */
    CpmacDDACbIf *ddaIf;                /**< DDA provided callback functions */
    CpmacDDCIf *ddcIf;                  /**< DDC implemented functions */
} _CpmacDDCObj;

/* Function prototypes */
PAL_Result DDC_cpmacSend(CpmacDDCObj * hDDC, DDC_NetPktObj * pkt,
                         Int channel, Ptr sendArgs);
PAL_Result cpmacTick(CpmacDDCObj * hDDC, Ptr tickArgs);


Int cpmacTxPktProcess(CpmacDDCObj * hDDC, Int * pktsPending, Ptr pktArgs);

int cpmacRxPktProcess(CpmacDDCObj * hDDC, Int * pktsPending, Ptr pktArgs);

Int cpmacRxBDProc(CpmacDDCObj * hDDC, Uint32 * handlePktsAndStatus,
                  Bool * isEOQ);
Int cpmacTxBDProc(CpmacDDCObj * hDDC, Uint32 * handlePktsAndStatus,
                  Bool * isEOQ);
Int cpmacPktProcessEnd(CpmacDDCObj * hDDC, Ptr procArgs);
Int cpmacTxPktProcessEnd(CpmacDDCObj * hDDC, Ptr procArgs);
void cpmacAddBDToRxQueue(CpmacDDCObj * hDDC, CpmacRxCppiCh * rxCppi,
                         CpmacHostDesc * currBD, Char * buffer,
                         DDC_NetDataToken bufToken);


Int cpmacUpdatePhyStatus(CpmacDDCObj * hDDC);

/*************************** Interrupt related functions **********************/
static inline int enableTxIntr (CpmacTxCppiCh* txCppi)
{
#ifndef CPMAC_POLL_MODE
    return avalanche_intd_enable_interrupt (CPGMAC_INTD_HOST_NUM, txCppi->accChNum);
#else 
    return 0;
#endif
} 

static inline int disableTxIntr (CpmacTxCppiCh* txCppi)
{
#ifndef CPMAC_POLL_MODE
    return avalanche_intd_disable_interrupt (CPGMAC_INTD_HOST_NUM, txCppi->accChNum);
#else
    return 0;
#endif
} 

static inline int enableRxIntr (CpmacRxCppiCh* rxCppi)
{
#ifndef CPMAC_POLL_MODE
    return avalanche_intd_enable_interrupt (CPGMAC_INTD_HOST_NUM, rxCppi->accChNum);
#else 
    return 0;
#endif
} 

static inline int disableRxIntr (CpmacRxCppiCh* rxCppi)
{
#ifndef CPMAC_POLL_MODE
    return avalanche_intd_disable_interrupt (CPGMAC_INTD_HOST_NUM, rxCppi->accChNum);
#else
    return 0;
#endif
} 

static inline int doTxEOI (CpmacTxCppiCh* txCppi)
{
#if defined CPGMAC_USE_ACC_LIST || !defined CPMAC_POLL_MODE
    return avalanche_intd_write_eoi (txCppi->accVecNum);
#else
    return 0;
#endif
} 

static inline int doRxEOI (CpmacRxCppiCh* rxCppi)
{
#if defined CPGMAC_USE_ACC_LIST || !defined CPMAC_POLL_MODE
    return avalanche_intd_write_eoi (rxCppi->accVecNum);
#else
    return 0;
#endif
} 

/************************ End - Interrupt related functions *******************/


/*@} */

#endif /* __DDC_CPGMAC_F_DRV_H__ */
