/*
 *
 * cppi41_dma.h
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
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/


#ifndef _CPPI41_DMA_H_
#define _CPPI41_DMA_H_

/* configuration */
//#define USB_CPPI4_DEBUG
#define USB_USE_ACC_LIST
#define USB_INCLUDE_ASSERT
#define USB_CPPI_TX_TASKLET_MODE
#define USB_CPPI_RX_TASKLET_MODE

#ifdef CONFIG_ARM_AVALANCHE_PPD

#ifndef CONFIG_USB_MUSB_HOST
#define     CONFIG_USB_PPD_SUPPORT
#endif

#define         USB_PPD_INFRA_TXDMA_NUM         1
#define         USB_PPD_INFRA_RXDMA_NUM         1
#define         USB_RX_EPDMA_NUM                1
#endif


#include <linux/slab.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/dmapool.h>

#include "musb_core.h"
#include <asm-arm/arch-avalanche/generic/pal_cppi41pvt.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
#include <asm-arm/arch-avalanche/puma5/puma5_pp.h>
#endif


#define PAL_CPPI4_CACHE_INVALIDATE(addr, size)  dma_cache_inv ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK(addr, size)   dma_cache_wback ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(addr, size)   dma_cache_wback_inv ((unsigned long)(addr), (size))

#ifndef PRIVATE
#define PRIVATE   static
#endif

#ifdef USB_INCLUDE_ASSERT
#define assert(handle_parent, expr) \
    if(!(expr)) {                                           \
        handle_parent->hDDC->ddaIf->ddaErrLog( "Assertion failed! %s,%s,%s,line=%d\n",    \
            #expr,__FILE__,__FUNCTION__,__LINE__); BUG (); }
#else
#define assert(handle_parent, expr)
#endif

#define USB_CPPI4_DEBUG_LEVEL   4


#define USB_HOST        0
#define USBOTG_MAX_TX_QS    1
#define MAX_USB_CPPI4_CHANNEL   4
#define USB_OTG_MAX_TX_QS   1
#define USBOTG_NUM_RXFDB_QS     1 /* TODO */
#define CPPI4_PKT_TYPE_USB  5


#define USB_RX_EMBEDDED_BD_NUM   64

/* DMA number 1 is used for usb */
#define USB_CPPI41_DMA_NUM      1//1
/* CPPI4.1 DMA Tx channels for ep1 to ep4*/
#define USB_CPPI41DMA_EP1_TXCH      4
#define USB_CPPI41DMA_EP2_TXCH      5
#define USB_CPPI41DMA_EP3_TXCH      6
#define USB_CPPI41DMA_EP4_TXCH      7
/* CPPI4.1 DMA Rx channels for ep1 to ep4*/
#define USB_CPPI41DMA_EP1_RXCH      4
#define USB_CPPI41DMA_EP2_RXCH      5
#define USB_CPPI41DMA_EP3_RXCH      6
#define USB_CPPI41DMA_EP4_RXCH      7

/* accumulator configuration */
#define USB_ACC_MODE_LIST_MODE      0   /*  0 => list mode */
#define USB_ACC_MODE_MONITOR_MODE   1   /* 1 => monitor mode */

/* error defintiorns */
#define USB_SUCCESS         0
#define USB_ERR_TX_OUT_OF_BD        1
#define USB_ERR_CPPI_TX_CH      2
#define USB_ERR_CPPI_DESC_REGN_FAIL     3
#define USB_ERR_TX_CH_ALREADY_CLOSED    4
#define USB_ERR_RX_CH_ALREADY_CLOSED    5
#define USB_ERR_RXCH_INVALID        6
#define USB_ERR_RX_OUT_OF_BD        7


/* CPPI tx/rx channel states */
#define USB_CPPI4_CH_INITIALIZED    1
#define USB_CPPI4_CH_OPENED     2
#define USB_CPPI4_CH_CLOSED     3


/* CPPI4 Buffer Descriptor Macros
 * CPPI41 related MACROS
 * TODO: Need to move to a better place (inside CPPI module?).
 */
#define CPPI4_DESC_TYPE_SHIFT_HOST      27
#define CPPI4_DESC_TYPE_HOST            16
#define CPPI4_PKT_TYPE_SHIFT            26
#define CPPI4_PKT_TYPE_ETH              7
#define CPPI4_PKT_RETPLCY_SHIFT         15
#define CPPI4_PKT_RETPLCY_FULL          0
#define CPPI4_DESC_LOC_SHIFT            14
#define CPPI4_DESC_LOC_OFFCHIP          0
#define CPPI4_PKT_RETQMGR_SHIFT         12
#define CPPI4_PKT_RETQ_SHIFT            0
#define CPPI4_DESC_TYPE_SHIFT_TD        27
#define CPPI4_DESC_TYPE_MASK_TD         (0x1F << CPPI4_DESC_TYPE_SHIFT_TD)
#define CPPI4_DESC_TYPE_TD              19
#define CPPI4_BD_BUF_SIZE_MASK          0xFFFF
#define CPPI4_BD_PKT_LENGTH_MASK        0x3FFFFF
/* Only CPPI specified bytes need to be invalidated */
#define CPPI4_BD_LENGTH_FOR_CACHE       64

/* TODO to be moved to arch/asm/mach-avalanch.. common header file */
#define USB_CPPI4_HOST_DESC_ALGN    64
#define USB_CPPI4_EPNUM_BD      64
#define USB_CPPI4_MAX_BD        (CPPI41_MAX_USB_DESC)
#define USB_CPPI4_QMGR_NUM      0
/*
 * INTD registers defintions
 */

#define INTCBASE            0x50000000
#define INTCACCUMMASK       0x18
#define INTDBASE            (IO_ADDRESS(0x03064000))
#define INTDSTATUS          (INTDBASE + 0x204)
#define INTD_COUNT_REGBASE  (INTDBASE + 0x300)
#define INTD_EOI_REG        (INTDBASE + 0x10)

/*
 * accumulator list configuration data
 */

//#define USB_USE_ACCUMULATOR_LIST

#define USBOTG_ACC_LIST_DIV     2
#define USB_ACC_ENTRY_SIZE_VAL      CPPI41_ACC_ENTRY_D
#define USB_ACC_ENTRY_SIZE      4
#define USB_ACC_LIST_MODE       0
#define USB_ACC_RX_MAXENTRIES       32//32
#define USB_ACC_TX_MAXENTRIES       32//32
#define USB_ACC_INTR_MODE       2
#define USB_ACC_INTR_DELAY      5 // = 125usec
#define USB_ACC_LIST_DIV        2
#define USB_ACC_STALL_AVOID     0
#define CPPI41_ACC_ENTRY_D              0
#define CPPI41_ACC_ENTRY_CD             1
#define CPPI41_ACC_ENTRY_ABCD           2

#define USB_EP1_4TX_INTVEC      (INTC_CPINTD_VEC_BASE + USB_EP_TX_ACC_VEC_NUM)
#define USB_EP1_4RX_INTVEC      (INTC_CPINTD_VEC_BASE + USB_EP_RX_ACC_VEC_NUM)

#define USB_CPPI_TXDMA_INTR     (AVALANCHE_INTD_BASE_INT + USB_ACCTX_INTD_VEC )
#define USB_CPPI_RXDMA_INTR     (AVALANCHE_INTD_BASE_INT + USB_ACCRX_INTD_VEC )
#define USB_ACC_TXCH_MASK       ( 1 << USB_ACC_TX_CHNUM )
#define USB_ACC_TXCH_SHIFT_CNT      (USB_ACC_TX_CHNUM)
#define USB_ACC_RXCH_MASK       (0xF <<USB_ACC_RX1_CHNUM )
#define USB_ACC_RXCH_SHIFT_CNT      (USB_ACC_RX1_CHNUM)

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)

#define MAX_USB_ENDPOINT            4
#define USB_INFRA_CH_DMA_BLOCK_NUM  1
#define USB_TX_INFRA_CHNUM      USB_CPPI4x_USB2HOST_PROXY_CHNUM(0)
#define USB_RX_INFRA_CHNUM      USB_CPPI4x_USB2HOST_PROXY_CHNUM(0)
#define PP_DMA_BLOCK_NUM        0

/* tx configuration */
#define USB_TX_INFRA_CHNUM1           USB_CPPI4x_USB2HOST_PROXY_CHNUM(0)
#define USB_TX_INFRA_CHNUM2           USB_CPPI4x_USB2HOST_PROXY_CHNUM(1)
#define USB_TX_INFRA_CHNUM3           USB_CPPI4x_USB2HOST_PROXY_CHNUM(2)
#define USB_TX_INFRA_CHNUM4           USB_CPPI4x_USB2HOST_PROXY_CHNUM(3)



#endif

/* CPPI data structure definitions */

/*
 *  CPPI  Buffer Descriptor
 *
 *  Buffer Descriptor structure for USB OTG Module CPPI.Using the same across Tx/Rx
 */
struct usbCppi4HostDesc {
    /* hardware descriptor fields from this point */
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, pkt len (valid only in Host PD)*/
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) (valid only in Host PD)*/
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Uint32 buffLen;      /**< Number of valid data bytes in the buffer */
    Uint32 bufPtr;       /**< Pointer to the buffer associated with this descriptor */
    Ptr nextBDPtr;       /**< Pointer to the next buffer descriptor */
    Uint32 orgBuffLen;   /**< Original buffer size */
    Uint32 orgBufPtr;    /**< Original buffer pointer */
    Uint32 netInfoWord0; /**< Network stack private communications info (valid only in Host PD) */
    Uint32 netInfoWord1; /**< Network stack private communications info (valid only in Host PD) */

      //Cppi4HostDesc hwDesc;
    /* Protocol specific data */
    Ptr *nextSwBDPtr;
//  dma_addr_t dma;     /* address of this descriptor */
    Int32 zlp;
    Uint32 tagId;      /* sourceId (31:16) Endpoint IN/OUT (15:8), EndPointNumber (7:0) */
    u32 dwMaxLength;
    u32 dwActualLength;
    u8 status;
    u8 desired_mode;
    u8  reserved[2];
//  Int32 reserved[2];

};

typedef volatile struct usbCppi4HostDesc UsbCppi4HostDesc;
/**
 *  \brief cppi Channel Config Info
 *
 *  Common to both TX/RX.  Used to pass channel config info from DDA to DDC for
 *  CPMAC channels.  Note that the caller of ChOpen is required to fill the
 *  given CPPI4 channel structures with data.
 */
typedef struct {
    Int chNum;                      /**< Channel number */
    Int chDir;                 /**< Channel direction */
    Int chState;                /**< Channel state */
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

} usbCppiChInfo;


typedef struct {
    usbCppiChInfo chInfo;               /**< Channel config/info */
    Uint32 allocSize;                 /**< Tx BD pool allocated memory size */
    Char *bdMem;                      /**< Tx BD Memory pointer */
    spinlock_t            bdlock;
    UsbCppi4HostDesc* bdPoolHead;        /**< Free BD Pool Head */

    Ptr palCppi4Hnd;                  /**< Holds CPPI4 handle needed for push
                                        pop APIs */
    Ptr cppi4TxChHnd;                 /**< CPPI4 Tx Channel Handle */
    Uint32 numBD;                     /** Number of Free descriptors available */

    /*
     * CPPI specific information needed by the driver. Note that most of this
     * info comes from Cppi4TxChInfo which is filled during initialization
     */
    Uint32 numTxQs;                       /**< Number of Tx queues available. */
    Cppi4Queue txQueue[USBOTG_MAX_TX_QS]; /**< Queues used for transmit. Can
                                             schedule the queuing among
                                             numTxCmplQs for prioritization */

    PAL_Handle txQueueHnd[USBOTG_MAX_TX_QS];/**< Array of handles to tx queues */

#if 0
    Cppi4Queue txCmplQueue;     /**< Tx completion queue */
    PAL_Handle txCmplQueueHnd;  /**< Handle to tx compl queue */
    PAL_Cppi4AccChHnd txAccChHnd;

    Ptr listBuffBase[USBOTG_ACC_LIST_DIV];  /**< Contains array of list region
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
#endif
#ifdef  USB_GET_STATS
    Uint32  outOfTxBD;
#endif

} usbTxCppiCh;


/**
 *  \brief RX Channel Control Structure
 *
 *  Used by CPMAC DDC code to process RX Buffer Descriptors
 */
typedef struct _CpmacRxCppiCh_t {
    usbCppiChInfo chInfo;               /**< Channel config/info */
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    usbCppiChInfo epRxChInfo;
    Ptr cppi4InfraRxChHnd;                 /**< CPPI4 Rx Infra dma Channel Handle */
#endif
    Uint32 allocSize;                 /**< Tx BD pool allocated memory size */
    Char *bdMem;                      /**< Tx BD Memory pointer */
    spinlock_t            bdlock;
    UsbCppi4HostDesc* bdPoolHead;        /**< Free BD Pool Head */
    Uint32 numBD;
    Uint32 numOfBdInFdbQueue;        /* number of free buffer descriptor added in fdbQueue*/

    Ptr palCppi4Hnd;                  /**< Holds CPPI4 handle needed for push
                                        pop APIs */
    Ptr cppi4RxChHnd;                 /**< CPPI4 Rx Channel Handle */

    /*
     * CPPI specific information needed by the driver. Note that most of this
     * info comes from Cppi4RxChInfo which is filled during initialization.
     *
     * !@@ TODO: Need to add members for Embedded and Monolithic too.
     */
    Cppi4Queue rxQueue;                         /**< Receive queue */
    PAL_Handle rxQueueHnd;                      /**< Handle to receive queue */

    Cppi4Queue fdbQueue[USBOTG_NUM_RXFDB_QS];   /**< Free desc/buffer queues
                                                  used for host packets */
    PAL_Handle fdbQueueHnd[USBOTG_NUM_RXFDB_QS];/**< Array of handles to free
                                                  desc/buffer queues */
    PAL_Cppi4AccChHnd rxAccChHnd;

    Ptr listBuffBase[USBOTG_ACC_LIST_DIV];  /**< Contains array of list region
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

#ifdef  USB_GET_STATS
    Uint32  outOfRxBD;
#endif
} usbRxCppiCh;

/* forward declaration for CppiDmaController structure */
struct cppi;

/**
 *  Channel Control Structure
 *
 * CPPI  Channel Control structure. Using he same for Tx/Rx. If need be
 * derive out of this later.
 */
struct cppi_channel {
    /* First field must be dma_channel for easy type casting
     * FIXME just use container_of() and be typesafe instead!
     */
    struct dma_channel Channel;

    usbTxCppiCh *txCppi;
    u32 txIsCreated;
    u32 txTeardownPending;
    u32 txIsOpen;

    usbRxCppiCh *rxCppi;
    u32 rxTeardownPending;
    u32 rxIsCreated;
    u32 rxIsOpen;

    /* back pointer to the Dma Controller structure */
    struct cppi     *pController;

    /* which direction of which endpoint? */
    struct musb_hw_ep   *pEndPt;
    u8          bTransmit;
    u8          chNo; /* channel number of tx/rx 0,1,2,3 */

    /* DMA modes:  RNDIS or "transparent", CDC or Generic Mode TODO */
    u8          bLastMode;
    u8          autoReq;
    /* Rx Requested mode */
    u8          rxMode;
    u8          reqcomplete; // zero packet handling
    /* book keeping for current transfer request */
    dma_addr_t      startAddr;
    u32         transferSize;
    u32         pktSize;
    u32         currOffset; /* requested segments */
    u32         actualLen;  /* completed (Channel.actual) */
        dma_addr_t      actStartAddr;
    u32         no_txbd;
    /* use tx_complete in host role to track endpoints waiting for
     * FIFONOTEMPTY to clear.
     */
    struct list_head    tx_complete;
};


struct bdMemChunk{
    Char   *base;
    Uint32 numBD;
    Uint32 allocated;
};
/**
 *  CPPI Dma Controller Object
 *
 *  CPPI Dma controller object.Encapsulates all bookeeping and Data
 *  structures pertaining to the CPPI Dma Controller.
 */
struct cppi
{
    /* FIXME switchover to container_of() and remove the
     * unsafe typecasts...
     */
    struct dma_controller           Controller;
    struct musb *                   musb;
    void __iomem *                  mregs;
    void __iomem *                  tibase;
    void __iomem *                  pCoreBase;

    //MGC_pfDmaChannelStatusChanged dma_completed;

    Cppi4PALObj *                   cppi4PAL;
    struct cppi_channel             txCppiCh[ MUSB_C_NUM_EPT - 1 ];
    struct cppi_channel             rxCppiCh[ MUSB_C_NUM_EPR - 1 ];

    Uint32                          txdma_IntrLine;
    Uint32                          rxdma_IntrLine;
    Uint32                          txIsrAttachedFlag;
    Uint32                          rxIsrAttachedFlag;

    /* accumulator configuration for tx-dma interrupt */
    Cppi4AccumulatorCfg             accChCfg;
    PAL_Cppi4AccChHnd               txAccChHnd;

    Ptr                             listBuffBase[USBOTG_ACC_LIST_DIV];  /**< Contains array of list region bases divided according the ping-pong buffer size */

    Uint32 *                        listEntryPtr;                       /**< Pointer to list entry. Points to unread entry in the list after interrupts start. */
    Uint32                          activeListRgn;                      /**< Indicates which of the CPGMAC_ACC_LIST_DIV regions of the list area are to be used
                                                                             by the driver. The range is 0 to (CPGMAC_ACC_LIST_DIV-1) and updated per EOI */
    Uint32                          accListSize;                        /**< Total size in bytes of the list region */

    Uint32                          accChNum;                           /**< Accumulator channel number */
    Uint32                          accVecNum;                          /**< Accumulator interrupt vector */

    Cppi4Queue                      txCmplQueue;                        /**< Tx completion queue */
    PAL_Handle                      txCmplQueueHnd;                     /**< Handle to tx compl queue */

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    usbCppiChInfo                   infraTxChInfo;
    Uint32                          cppi4InfraTxChOpenFlag;
    Ptr                             cppi4InfraTxChHnd;
    Cppi4Queue                      UsbHostPrxyQ;
#endif

#ifdef USB_CPPI4_GETSTATS
    Uint32                          txIntCount[ MUSB_C_NUM_EPT - 1 ];
    Uint32                          rxIntCount[ MUSB_C_NUM_EPT - 1 ];
#endif

    u32                             txTeardownPending;

    struct dma_pool *               pool;
    struct bdMemChunk               bdPool[ CPPI41_MAX_USB_EPS ];
    Char *                          bdMem;                              /**< Tx BD Memory pointer */
    Uint32                          numBD;

    int                             tx_state;
    struct list_head                tx_complete;
};


/* irq handling hook */
extern void cppi_completion(struct musb *, u32 rx, u32 tx);

#endif              /* end of ifndef _CPPI_DMA_H_ */


/*************************** Interrupt related functions **********************/
#if 0
static inline int enableTxIntr (usbTxCppiCh* txCppi)
{
    return avalanche_intd_enable_interrupt (USB_HOST, txCppi->accChNum);
}

static inline int disableTxIntr (usbTxCppiCh* txCppi)
{
    return avalanche_intd_disable_interrupt (USB_HOST, txCppi->accChNum);
}
#endif
static inline int enableTxIntr (int accChNum)
{
    return avalanche_intd_enable_interrupt (USB_HOST, accChNum);
}

static inline int disableTxIntr (int accChNum)
{
    return avalanche_intd_disable_interrupt (USB_HOST, accChNum);
}

static inline int enableRxIntr (usbRxCppiCh* rxCppi)
{
    return avalanche_intd_enable_interrupt (USB_HOST, rxCppi->accChNum);
}

static inline int disableRxIntr (usbRxCppiCh* rxCppi)
{
    return avalanche_intd_disable_interrupt (USB_HOST, rxCppi->accChNum);
}
#if 0
static inline int doTxEOI (usbTxCppiCh* txCppi)
{
#if defined USB_USE_ACC_LIST
    return avalanche_intd_write_eoi (txCppi->accVecNum);
#else
    return 0;
#endif
}
#endif
static inline int doTxEOI (int accVecNum)
{
#if defined USB_USE_ACC_LIST
    return avalanche_intd_write_eoi (accVecNum);
#else
    return 0;
#endif
}

static inline int doRxEOI (usbRxCppiCh* rxCppi)
{
#if defined USB_USE_ACC_LIST
    return avalanche_intd_write_eoi (rxCppi->accVecNum);
#else
    return 0;
#endif
}

/************************ End - Interrupt related functions *******************/
