
/*
 *
 * cppi41_dma.c
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

/*
 * This file implements a DMA  interface using TI's CPPI DMA.
 * For now it's DaVinci-only, but CPPI isn't specific to DaVinci or USB.
 */
#include <linux/usb.h>
#include <linux/skbuff.h>

#include "cppi41_dma.h"
#include "musb_core.h"
#include "musb_host.h"
#include "puma5.h"
#include <asm-arm/arch-avalanche/puma5/puma5_cppi.h>


#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    #define AVALANCHE_PPD_TEARDOWN
#endif

/* CPPI DMA status 7-mar:
 *
 * - See musb_{host,gadget}.c for more info
 *
 * - Correct RX DMA generally forces the engine into irq-per-packet mode,
 *   which can easily saturate the CPU under non-mass-storage loads.
 */

/* REVISIT now we can avoid preallocating these descriptors; or
 * more simply, switch to a global freelist not per-channel ones.
 * Note: at full speed, 64 descriptors == 4K bulk data.
 */


//#define  USBDRV_DEBUG 1
//#define  DEBUG_CPPI_TD
//#define USB_CPPI4_DEBUG
#define USB_CPPI4_DBG_LEVEL  7
#ifdef USBDRV_DEBUG
    #define dprintk(x,...) printk(x, ## __VA_ARGS__)
    #define DBG(level,x,...) printk(x, ## __VA_ARGS__)
#else
    #define dprintk(x,...)
    #define DBG(x,...)
#endif


//#define DBG(level,fmt,args...) xprintk(level,KERN_DEBUG,fmt, ## args)

usbCppiChInfo usbTxChInfo[MAX_USB_CPPI4_CHANNEL+3];
usbCppiChInfo usbRxChInfo[MAX_USB_CPPI4_CHANNEL+3];
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
usbCppiChInfo ppd_usbRxChInfo[4];
#endif

void dma_controller_destroy(struct dma_controller *pController);
static int cppi_channel_abort(struct dma_channel *pChannel);
void  cppi_abort_flushfifo(struct musb *musb, int bEnd,int flushfifo );

static int txCppiChannelTearDown( struct cppi_channel *txCh );
static int rxCppiChannelTearDown( struct cppi_channel *rxCh );

UsbCppi4HostDesc* usbGetRxBD(usbRxCppiCh * rxCppi);
UsbCppi4HostDesc *usbGetFreeTxBDList(usbTxCppiCh * txCppi, int numBDs);
UsbCppi4HostDesc *usbGetFreeRxBDList(usbRxCppiCh * rxCppi, int numBDs);
static int usbTxBDProc(struct cppi * cppi, int txChannel );
static Int usbTxPktProcess(struct cppi *cppi, Ptr pktArgs);
void cppi_mode_update ( struct cppi_channel *c, int bTransmit, int mode );
static Int usbRxPktProcess(struct cppi *cppi, Ptr pktArgs);
static int usbRxBDProc(struct cppi * cppi, int rxChannel );
UsbCppi4HostDesc *alloc_buff_descriptor(struct cppi *c, int numBDs);
int free_buff_descriptor(struct cppi *c,UsbCppi4HostDesc *BDPtr);
void printBD(UsbCppi4HostDesc *currBD, int displayData);
irqreturn_t usbCppiTxPktProcess_Isr(int irq, void *pThis);
irqreturn_t usbCppiRxPktProcess_Isr(int irq, void *pThis);


#ifdef USB_CPPI_TX_TASKLET_MODE
struct tasklet_struct tx_tasklet;
int    tx_musb_tasklet_scheduled;
int    tx_taskInit;
void   usb_cppi_tx_tasklet(unsigned long data);
#endif

#ifdef USB_CPPI_RX_TASKLET_MODE
struct tasklet_struct rx_tasklet;
int    rx_musb_tasklet_scheduled;
int    rx_taskInit;
void   usb_cppi_rx_tasklet(unsigned long data);
#endif



static inline void cpu_drain_writebuffer(void)
{
    wmb();
#ifdef  CONFIG_CPU_ARM926T
    /* REVISIT this "should not be needed",
     * but lack of it sure seemed to hurt ...
     */
    asm("mcr p15, 0, r0, c7, c10, 4 @ drain write buffer\n");
#endif
}
#define CAST (void *__force __iomem)
/*
 * alloc_buff_descriptor
 */
UsbCppi4HostDesc *alloc_buff_descriptor(struct cppi *c, int numBDs)
{
    UsbCppi4HostDesc *BDPtr = NULL;
    struct bdMemChunk *bdPool;
    Uint32 i;

    bdPool = c->bdPool;
    for ( i = 0; i < CPPI41_MAX_USB_EPS; ++i )
    {
        if ( bdPool[i].allocated == 0 )
        {
            bdPool[i].allocated = 1;
            BDPtr = (UsbCppi4HostDesc *)bdPool[i].base;
            break;
        }
    }

    return BDPtr;
}
/*
 * free_buff_descriptor
 */
int free_buff_descriptor(struct cppi *c,UsbCppi4HostDesc *BDPtr)
{

    struct bdMemChunk *bdPool;
    Uint32 i;

    bdPool = c->bdPool;
    for ( i = 0; i < CPPI41_MAX_USB_EPS; ++i )
    {
        if ( bdPool[i].allocated && ((UsbCppi4HostDesc *)bdPool[i].base == BDPtr) )
        {
            bdPool[i].allocated = 0;
            BDPtr = NULL;
            return 0;
        }
    }

    return 1;

}
#ifdef DEBUG_CPPI_TD
void printBDList(UsbCppi4HostDesc *bdPoolHead )
{
    UsbCppi4HostDesc* currBD;
    int cnt = 0;

    currBD = bdPoolHead;
    while ( currBD != NULL )
    {
        if ( cnt % 8 == 0 )
            dprintk("\n%02x ",cnt);
        cnt++;
        dprintk(" %p",currBD);
        currBD = (UsbCppi4HostDesc*)currBD->nextSwBDPtr;
    }
    dprintk("\n");

}
#endif
/**
 *  usbCppi4InitTxChannel
 *    - Allocates memory for TX Ch Control structure, Buffer descriptors
 *    - Initializes the above data structures as per channel configuration
 *    - Stores the cppi specific configuration provided in 'chInfo' for quick
 *    access at txCppi level.
 *    - Sets up accumulator list memory for tx complete as per the number of
 *    pages.  - Initializes each descriptor and chain the TX BD list ready to
 *    be given to hardware
 *
 */
PRIVATE int usbCppi4InitTxChannel(struct cppi * cppi,
                                  struct cppi_channel *txCppiChannel,usbCppiChInfo * chInfo)
{
    PAL_Result retCode;
    Uint32 cnt;
    UsbCppi4HostDesc* currBD;
    usbTxCppiCh *txCppi = NULL;


    DBG(USB_CPPI4_DBG_LEVEL,"\n usbCppi4InitTxChannel:%d", chInfo->chNum);

    /* allocate memory for the usbTxCppiCh structure */
    retCode = PAL_osMemAlloc(0, sizeof(usbTxCppiCh), 0, (Ptr *) & txCppi);
    if ( retCode != PAL_SOK )
    {
        DBG(USB_CPPI4_DBG_LEVEL,
            "\nERROR:usbInitTxChannel:Failed to allocate memory for usbTxCppi Ch %d",
            chInfo->chNum);
        return(retCode);
    }
    PAL_osMemSet(txCppi, 0, sizeof(usbTxCppiCh));
    spin_lock_init (&txCppi->bdlock);
    txCppiChannel->txCppi = txCppi;
    txCppiChannel->pController = cppi;

    txCppi->chInfo = *chInfo;
    txCppi->chInfo.chState = USB_CPPI4_CH_INITIALIZED;

    txCppi->numBD = chInfo->numBD;
    txCppi->numTxQs = chInfo->numTxQs;
    /*TODO numTxQs is set to one, in usb cppi component */
    for ( cnt = 0; cnt < chInfo->numTxQs; cnt++ )
    {
        txCppi->txQueue [cnt] = chInfo->cppi4TxChInfo.txInQueue [cnt];
    }

    txCppi->chInfo.cppi4TxChInfo.tdQueue = chInfo->cppi4TxChInfo.txCompQueue;

    txCppiChannel->txTeardownPending = False;  /* Clear the TX teardown pending flag */

    /* TODO: We may need to check numBD here since CPPI4.1 requires the
     * desc count to be * multiple of 2 power 5 (ie 32). Similarly, the desc
     * size should also be multiple of 32.
     */
    txCppi->bdMem = (Char *)alloc_buff_descriptor( cppi, chInfo->numBD );

    if ( txCppi->bdMem == NULL )
    {
        DBG (1, "\nERROR:usbInitTxChannel: Failed to allocate Tx descriptors");
        return(USB_ERR_CPPI_DESC_REGN_FAIL); /* descriptor region is not available */
    }
    txCppi->allocSize = sizeof(UsbCppi4HostDesc) * chInfo->numBD;

    currBD = (UsbCppi4HostDesc *) txCppi->bdMem;
    txCppi->bdPoolHead = 0;
    for ( cnt = 0; cnt < chInfo->numBD; cnt++ )
    {
        currBD->descInfo    = CPPI4_DESC_TYPE_HOST << CPPI4_DESC_TYPE_SHIFT_HOST;
        currBD->tagInfo     = 0;
        currBD->pktInfo     = (CPPI4_PKT_TYPE_USB << CPPI4_PKT_TYPE_SHIFT)
                              | (CPPI4_PKT_RETPLCY_FULL << CPPI4_PKT_RETPLCY_SHIFT)
                              | (CPPI4_DESC_LOC_OFFCHIP << CPPI4_DESC_LOC_SHIFT)
                              | (USB_CPPI41_TXCMPL_QMGR << CPPI4_PKT_RETQMGR_SHIFT)
                              | (USB_CPPI41_TXCMPL_QNUM << CPPI4_PKT_RETQ_SHIFT);
        currBD->buffLen     = 0;
        currBD->bufPtr      = 0;
        currBD->nextBDPtr   = 0;
        currBD->orgBuffLen  = 0;
        currBD->orgBufPtr   = 0;

        currBD->nextSwBDPtr = (Ptr) txCppi->bdPoolHead;
        txCppi->bdPoolHead = currBD;
        currBD = (UsbCppi4HostDesc*) ((Uint32)currBD + txCppi->chInfo.descAlignment);
    }

    txCppiChannel->txIsCreated = True;

    DBG(USB_CPPI4_DEBUG_LEVEL,
        "\n- cppi4InitTxChannel: Ch=%d",chInfo->chNum);

    return(USB_SUCCESS);
}
/**
 *  usbUnInitTxChannel
 *    - Frees memory previously allocated for Ch Control structure, buffer
 *      descriptors
 *
 *  \note 1. This function assumes that the channel number passed is valid and
 *           this function will not do any error check to avoid duplicate error
 *           checks (done in caller function).
 */
PRIVATE PAL_Result usbUnInitTxChannel(struct cppi *cppi, Uint32 channel)
{
    PAL_Result retCode;
    usbTxCppiCh *txCppi;

    DBG(USB_CPPI4_DEBUG_LEVEL,
        "\n+ usbUnInitTxChannel: Ch=%d",channel);

    /* Check if channel structure is already de-allocated */
    if ( cppi->txCppiCh[channel].txIsCreated == False )
    {
        DBG(1,
            "\nERROR:usbUnInitTxChannel:TX CPPI Ch %d structure already freed",channel);
        return(USB_ERR_TX_CH_ALREADY_CLOSED);
    }

    txCppi = cppi->txCppiCh[channel].txCppi;

    retCode = free_buff_descriptor( cppi, (UsbCppi4HostDesc *)txCppi->bdMem);

    txCppi->bdMem = NULL;
    if ( retCode != 0 )
    {
        DBG (USB_CPPI4_DEBUG_LEVEL,
             "\nERROR:usbUnInitTxChannel:Failed to free tx desc region for Ch %d",
             channel);
    }
    txCppi->numBD = 0;

#ifdef USB_USE_ACC_LIST
    retCode = PAL_osMemFree(0, txCppi->chInfo.txAccChInfo.list.listBase,
                            cppi->accListSize);
    if ( retCode != PAL_SOK )
    {
        DBG(USB_CPPI4_DEBUG_LEVEL,
            "\nERROR:iusbUnInitTxChannel: Failed to free TX accumulator list for Ch %d",
            channel);
    }
#endif

    /* Free the TX Channel structure */
    retCode = PAL_osMemFree(0, txCppi, sizeof(usbTxCppiCh));
    if ( retCode != PAL_SOK )
    {
        DBG(USB_CPPI4_DEBUG_LEVEL,
            "\nERROR:usbUnInitTxChannel: Failed to free TX CPPI channel structure for Ch %d",
            channel);
    }

    cppi->txCppiCh[channel].txCppi = NULL;
    cppi->txCppiCh[channel].txIsCreated = False;

    DBG(USB_CPPI4_DEBUG_LEVEL,"\n- usbUnInitTxChannel:Ch=%d",
        channel);

    return(USB_SUCCESS);
}

/* cppi dma channel resources used for usb OTG controller module

USB       DMA   DMA   QMgr  QNum  Src
          TX    RX                Port
-------------------------------------
EP0_TX    4     4      0    204    1
-------------------------------------
EP1_TX    5     5      0    206    2
-------------------------------------
EP2_TX    6     6      0    208    3
-------------------------------------
EP3_TX    7     7      0    210    4
-------------------------------------
*/
/* usbCppi4InitChInfo
 * Initialize the cppi4 tx/rx channel configuration information.
 * TODO Note : all configuration data are hardcoded.
 */
static void usbCppi4InitChInfo(void)
{
    int i;

    /* configuration data for usb cppi channel 0 */
    for ( i=0; i < MAX_USB_CPPI4_CHANNEL; ++i )
    {
        /* usb Cppi Channel 1/2/3/4 configuration for TX/RX endpoint */
        PAL_osMemSet(&usbTxChInfo[i], 0, sizeof(usbCppiChInfo));
        usbTxChInfo[i].chNum      = i+1;
        usbTxChInfo[i].chDir      = 0;
        usbTxChInfo[i].chState    = 0;
        usbTxChInfo[i].numBD      = USB_CPPI4_EPNUM_BD;
        usbTxChInfo[i].bufSize    = 0;
        usbTxChInfo[i].numTxQs    = 1;
        usbTxChInfo[i].serviceMax = 1;
        usbTxChInfo[i].descAlignment = USB_CPPI4_HOST_DESC_ALGN;
        usbTxChInfo[i].dmaMode = CPPI41_DMA_MODE_ENDPOINT;

        PAL_osMemSet(&usbRxChInfo[i], 0, sizeof(usbCppiChInfo));
        usbRxChInfo[i].chNum      = i+1;
        usbRxChInfo[i].chDir      = 1;
        usbRxChInfo[i].chState    = 0;
        usbRxChInfo[i].numBD      = USB_CPPI4_EPNUM_BD;
        usbRxChInfo[i].bufSize    = 0;
        usbRxChInfo[i].serviceMax = 1;
        usbRxChInfo[i].descAlignment = USB_CPPI4_HOST_DESC_ALGN;
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
        usbRxChInfo[i].dmaMode = CPPI41_DMA_MODE_INFRA;
#else
        usbRxChInfo[i].dmaMode = CPPI41_DMA_MODE_ENDPOINT;
#endif
    }

    /* Tx channel 0 configuration */
    usbTxChInfo[0].accVecNum                        = USB_ACCTX_INTD_VEC;
    usbTxChInfo[0].cppi4TxChInfo.chNum              = USB_CPPI41DMA_EP1_TXCH;
    usbTxChInfo[0].cppi4TxChInfo.dmaNum             = USB_CPPI41_DMA_NUM;
    usbTxChInfo[0].cppi4TxChInfo.txInQueue[0].qMgr  = USB_CPPI41_TX1_QMGR;
    usbTxChInfo[0].cppi4TxChInfo.txInQueue[0].qNum  = USB_CPPI41_TX1_QNUM;
    usbTxChInfo[0].cppi4TxChInfo.txCompQueue.qMgr   = USB_CPPI41_TXCMPL_QMGR;
    usbTxChInfo[0].cppi4TxChInfo.txCompQueue.qNum   = USB_CPPI41_TXCMPL_QNUM;

    /* Tx channel 1 configuration */
    usbTxChInfo[1].accVecNum                        = USB_ACCTX_INTD_VEC;
    usbTxChInfo[1].cppi4TxChInfo.chNum              = USB_CPPI41DMA_EP2_TXCH;
    usbTxChInfo[1].cppi4TxChInfo.dmaNum             = USB_CPPI41_DMA_NUM;
    usbTxChInfo[1].cppi4TxChInfo.txInQueue[0].qMgr  = USB_CPPI41_TX2_QMGR;
    usbTxChInfo[1].cppi4TxChInfo.txInQueue[0].qNum  = USB_CPPI41_TX2_QNUM;
    usbTxChInfo[1].cppi4TxChInfo.txCompQueue.qMgr   = USB_CPPI41_TXCMPL_QMGR ;
    usbTxChInfo[1].cppi4TxChInfo.txCompQueue.qNum   = USB_CPPI41_TXCMPL_QNUM ;

    /* Tx Channel 2 configuration */
    usbTxChInfo[2].accVecNum                        = USB_ACCTX_INTD_VEC;
    usbTxChInfo[2].cppi4TxChInfo.chNum              = USB_CPPI41DMA_EP3_TXCH;
    usbTxChInfo[2].cppi4TxChInfo.dmaNum             = USB_CPPI41_DMA_NUM;
    usbTxChInfo[2].cppi4TxChInfo.txInQueue[0].qMgr  = USB_CPPI41_TX3_QMGR;
    usbTxChInfo[2].cppi4TxChInfo.txInQueue[0].qNum  = USB_CPPI41_TX3_QNUM;
    usbTxChInfo[2].cppi4TxChInfo.txCompQueue.qMgr   = USB_CPPI41_TXCMPL_QMGR;
    usbTxChInfo[2].cppi4TxChInfo.txCompQueue.qNum   = USB_CPPI41_TXCMPL_QNUM;

    /* Tx Channel 3 configuration */
    usbTxChInfo[3].accVecNum                        = USB_ACCTX_INTD_VEC;
    usbTxChInfo[3].cppi4TxChInfo.chNum              = USB_CPPI41DMA_EP4_TXCH;
    usbTxChInfo[3].cppi4TxChInfo.dmaNum             = USB_CPPI41_DMA_NUM;
    usbTxChInfo[3].cppi4TxChInfo.txInQueue[0].qMgr  = USB_CPPI41_TX4_QMGR;
    usbTxChInfo[3].cppi4TxChInfo.txInQueue[0].qNum  = USB_CPPI41_TX4_QNUM;
    usbTxChInfo[3].cppi4TxChInfo.txCompQueue.qMgr   = USB_CPPI41_TXCMPL_QMGR;
    usbTxChInfo[3].cppi4TxChInfo.txCompQueue.qNum   = USB_CPPI41_TXCMPL_QNUM;

    /* Tx channel 0,1,2,3 : Accumulator channel configuration */
    for ( i = 0; i < 4; ++i )
    {
        usbTxChInfo[i].txAccChInfo.accChanNum           = USB_ACC_TX_CHNUM ;
        usbTxChInfo[i].txAccChInfo.mode                 = USB_ACC_MODE_LIST_MODE;
        usbTxChInfo[i].txAccChInfo.queue.qMgr           = USB_CPPI41_TXCMPL_QMGR;
        usbTxChInfo[i].txAccChInfo.queue.qNum           = USB_CPPI41_TXCMPL_QNUM;
        usbTxChInfo[i].txAccChInfo.pacingTickCnt        = USB_ACC_INTR_DELAY ;
        usbTxChInfo[i].txAccChInfo.list.listBase        = 0 ;
        usbTxChInfo[i].txAccChInfo.list.maxPageEntry    = USB_ACC_TX_MAXENTRIES ;
        usbTxChInfo[i].txAccChInfo.list.pacingMode      = USB_ACC_INTR_MODE ;
        usbTxChInfo[i].txAccChInfo.list.stallAvoidance  = USB_ACC_STALL_AVOID ;
        usbTxChInfo[i].txAccChInfo.list.listCountMode   = USB_ACC_LIST_MODE ;
        usbTxChInfo[i].txAccChInfo.list.listEntrySize   = USB_ACC_ENTRY_SIZE_VAL;
        usbTxChInfo[i].txAccChInfo.list.maxPageCnt      = USB_ACC_LIST_DIV;

        /* monitor configuration */
        usbTxChInfo[i].txAccChInfo.monitor.pktCountThresh = 0; /* TBD */
        usbTxChInfo[i].txAccChInfo.monitor.pacingMode     = 0;
    }


    /* Rx channel 0 configuration */
    usbRxChInfo[0].accVecNum            =  USB_ACCRX_INTD_VEC;
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    usbRxChInfo[0].cppi4RxChInfo.chNum      = USB_RX_INFRA_CHNUM;
    usbRxChInfo[0].cppi4RxChInfo.dmaNum         = USB_PPD_INFRA_RXDMA_NUM;
    usbRxChInfo[0].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST;
#else
    usbRxChInfo[0].cppi4RxChInfo.chNum      = USB_CPPI41DMA_EP1_RXCH;
    usbRxChInfo[0].cppi4RxChInfo.dmaNum         = USB_CPPI41_DMA_NUM;
    usbRxChInfo[0].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST; /* Host Packet descriptor */
#endif
    usbRxChInfo[0].cppi4RxChInfo.rxCompQueue.qMgr   = USB_CPPI41_RX1_QMGR;
    usbRxChInfo[0].cppi4RxChInfo.rxCompQueue.qNum   = USB_CPPI41_RX1_QNUM;
    usbRxChInfo[0].cppi4RxChInfo.sopOffset          = 0;
    usbRxChInfo[0].rxAccChInfo.accChanNum = USB_ACC_RX1_CHNUM ;
    usbRxChInfo[0].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qMgr = USB_CPPI41_RX1_FDB_QMGR;
    usbRxChInfo[0].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qNum = USB_CPPI41_RX1_FDB_QNUM;

    /* Rx channel 1 configuration */
    usbRxChInfo[1].accVecNum            =  USB_ACCRX_INTD_VEC;
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    usbRxChInfo[1].cppi4RxChInfo.chNum      = USB_RX_INFRA_CHNUM;
    usbRxChInfo[1].cppi4RxChInfo.dmaNum         = USB_PPD_INFRA_RXDMA_NUM;
    usbRxChInfo[1].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST;
#else
    usbRxChInfo[1].cppi4RxChInfo.chNum      = USB_CPPI41DMA_EP2_RXCH;
    usbRxChInfo[1].cppi4RxChInfo.dmaNum         = USB_CPPI41_DMA_NUM;
    usbRxChInfo[1].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST; /* Host Packet descriptor */
#endif
    usbRxChInfo[1].cppi4RxChInfo.rxCompQueue.qMgr   = USB_CPPI41_RX2_QMGR;
    usbRxChInfo[1].cppi4RxChInfo.rxCompQueue.qNum   = USB_CPPI41_RX2_QNUM;
    usbRxChInfo[1].cppi4RxChInfo.sopOffset          = 0;
    usbRxChInfo[1].rxAccChInfo.accChanNum = USB_ACC_RX2_CHNUM ;
    usbRxChInfo[1].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qMgr = USB_CPPI41_RX2_FDB_QMGR;
    usbRxChInfo[1].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qNum = USB_CPPI41_RX2_FDB_QNUM;

    /* Rx channel 2 configuration */
    usbRxChInfo[2].accVecNum            =  USB_ACCRX_INTD_VEC;
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    usbRxChInfo[2].cppi4RxChInfo.chNum      = USB_RX_INFRA_CHNUM;
    usbRxChInfo[2].cppi4RxChInfo.dmaNum         = USB_PPD_INFRA_RXDMA_NUM;
    usbRxChInfo[2].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST;//CPPI41_DESC_TYPE_EMBEDDED;
#else
    usbRxChInfo[2].cppi4RxChInfo.chNum      = USB_CPPI41DMA_EP3_RXCH;
    usbRxChInfo[2].cppi4RxChInfo.dmaNum         = USB_CPPI41_DMA_NUM;
    usbRxChInfo[2].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST; /* Host Packet descriptor */
#endif
    usbRxChInfo[2].cppi4RxChInfo.rxCompQueue.qMgr   = USB_CPPI41_RX3_QMGR;
    usbRxChInfo[2].cppi4RxChInfo.rxCompQueue.qNum   = USB_CPPI41_RX3_QNUM;
    usbRxChInfo[2].cppi4RxChInfo.sopOffset          = 0;
    usbRxChInfo[2].rxAccChInfo.accChanNum   = USB_ACC_RX3_CHNUM;
    usbRxChInfo[2].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qMgr = USB_CPPI41_RX3_FDB_QMGR;
    usbRxChInfo[2].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qNum = USB_CPPI41_RX3_FDB_QNUM;

    /* Rx channel 3 configuration */
    usbRxChInfo[3].accVecNum            =  USB_ACCRX_INTD_VEC;
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    usbRxChInfo[3].cppi4RxChInfo.chNum      = USB_RX_INFRA_CHNUM;
    usbRxChInfo[3].cppi4RxChInfo.dmaNum         = USB_PPD_INFRA_RXDMA_NUM;
    usbRxChInfo[3].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST;//CCPPI41_DESC_TYPE_EMBEDDED;
#else
    usbRxChInfo[3].cppi4RxChInfo.chNum      = USB_CPPI41DMA_EP4_RXCH;
    usbRxChInfo[3].cppi4RxChInfo.dmaNum         = USB_CPPI41_DMA_NUM;
    usbRxChInfo[3].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_HOST; /* Host Packet descriptor */
#endif
    usbRxChInfo[3].cppi4RxChInfo.rxCompQueue.qMgr   = USB_CPPI41_RX4_QMGR;
    usbRxChInfo[3].cppi4RxChInfo.rxCompQueue.qNum   = USB_CPPI41_RX4_QNUM;
    usbRxChInfo[3].cppi4RxChInfo.sopOffset          = 0;
    usbRxChInfo[3].rxAccChInfo.accChanNum   = USB_ACC_RX4_CHNUM;
    usbRxChInfo[3].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qMgr = USB_CPPI41_RX4_FDB_QMGR;
    usbRxChInfo[3].cppi4RxChInfo.u.hostPktCfg.fdbQueue[0].qNum = USB_CPPI41_RX4_FDB_QNUM;

    for ( i = 0; i < 4; ++i )
    {

        usbRxChInfo[i].cppi4RxChInfo.retryOnStarvation  = 1;

        /* Rx channel 0,1,2,3 : Accumulator channel configuration */
        usbRxChInfo[i].rxAccChInfo.mode         = USB_ACC_MODE_LIST_MODE;
        usbRxChInfo[i].rxAccChInfo.queue = usbRxChInfo[i].cppi4RxChInfo.rxCompQueue ;
        usbRxChInfo[i].rxAccChInfo.queue.qMgr   = usbRxChInfo[i].cppi4RxChInfo.rxCompQueue.qMgr;
        usbRxChInfo[i].rxAccChInfo.queue.qNum   = usbRxChInfo[i].cppi4RxChInfo.rxCompQueue.qNum;

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
        usbRxChInfo[i].rxAccChInfo.pacingTickCnt = USB_ACC_INTR_DELAY ;
#else
        usbRxChInfo[i].rxAccChInfo.pacingTickCnt = USB_ACC_INTR_DELAY ;
#endif
        usbRxChInfo[i].rxAccChInfo.list.listBase = 0 ;
        usbRxChInfo[i].rxAccChInfo.list.maxPageEntry = USB_ACC_RX_MAXENTRIES ;
        usbRxChInfo[i].rxAccChInfo.list.pacingMode = USB_ACC_INTR_MODE ;
        usbRxChInfo[i].rxAccChInfo.list.stallAvoidance = USB_ACC_STALL_AVOID ;
        usbRxChInfo[i].rxAccChInfo.list.listCountMode  = USB_ACC_LIST_MODE ;
        usbRxChInfo[i].rxAccChInfo.list.listEntrySize  = USB_ACC_ENTRY_SIZE_VAL;
        usbRxChInfo[i].rxAccChInfo.list.maxPageCnt     = USB_ACC_LIST_DIV;

        /* monitor configuration */
        usbRxChInfo[i].rxAccChInfo.monitor.pktCountThresh = 0; /* TBD */
        usbRxChInfo[i].rxAccChInfo.monitor.pacingMode     = 0;
    }

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    {
        /* Open Rx endpoint channel in embedded mode if Packet processor
         * is defined
         */
        u8 fdQueueNum[4] = { 
			USB_CPPI4x_EP0_FD_QNUM,
            USB_CPPI4x_EP1_FD_QNUM,
            USB_CPPI4x_EP2_FD_QNUM,
            USB_CPPI4x_EP3_FD_QNUM};

        for ( i=0; i<4; ++i )
        {
            ppd_usbRxChInfo[i].cppi4RxChInfo.retryOnStarvation  = 1;
            ppd_usbRxChInfo[i].cppi4RxChInfo.chNum              = USB_CPPI41DMA_EP1_RXCH + i;
            ppd_usbRxChInfo[i].cppi4RxChInfo.dmaNum             = USB_RX_EPDMA_NUM; //PP_DMA_BLOCK_NUM;
            ppd_usbRxChInfo[i].cppi4RxChInfo.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
            ppd_usbRxChInfo[i].cppi4RxChInfo.sopOffset          = 0;
            ppd_usbRxChInfo[i].cppi4RxChInfo.rxCompQueue.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
            ppd_usbRxChInfo[i].cppi4RxChInfo.rxCompQueue.qNum   = PAL_CPPI41_SR_PPDSP_LOW_Q_NUM;
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fdQueue.qNum = fdQueueNum[0]; //fdQueueNum[i];
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.numBufSlot   = (EMSLOTCNT-1);
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.sopSlotNum   = 1;

            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[0].bMgr = BUF_POOL_MGR0;
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[0].bPool = CPMAC_CPPI4x_POOL_NUM(1);

            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[1].bMgr = 0;
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[1].bPool = CPMAC_CPPI4x_POOL_NUM(0);
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[2].bMgr = 0;
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[2].bPool = CPMAC_CPPI4x_POOL_NUM(1);
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[3].bMgr = 0;
            ppd_usbRxChInfo[i].cppi4RxChInfo.u.embeddedPktCfg.fBufPool[3].bPool = CPMAC_CPPI4x_POOL_NUM(0);
            ppd_usbRxChInfo[i].numBD = USB_RX_EMBEDDED_BD_NUM;
            ppd_usbRxChInfo[i].descAlignment = 64;
            ppd_usbRxChInfo[i].chDir = 1; //USB_CH_DIR_RX;
            ppd_usbRxChInfo[i].dmaMode = CPPI41_DMA_MODE_ENDPOINT;
        }
    }
#endif

}
/**
 *  usbInitRxChannel
 *    - Allocates memory for RX Ch structure and other internal structs
 *    - Stores the cppi specific configuration provided in 'chInfo' for quick
 *    access at rxCppi level.
 *    - Sets up accumulator list memory for rx as per the number of pages.
 *    - prepares the rx packet list
 *
 *  \note
 *        1. This function assumes that the channel number passed is valid and
 *           the  >rxCppi pointer is NULL.
 *        2. This function will not do any error check on these initialization
 *        parameters to avoid duplicate error checks (done in caller function
 *        or init parse functions). Also numBD value and alignment requirement
 *        should be validated as per the specs during init time
 *
 */
PRIVATE PAL_Result usbCppi4InitRxChannel(struct cppi *cppi,
                                         struct cppi_channel *rxCppiChannel,usbCppiChInfo * chInfo, int chIndex)
{
    PAL_Result retCode;
    usbRxCppiCh *rxCppi = NULL;
    UsbCppi4HostDesc *currBD;
    Cppi4AccumulatorCfg* accChCfg;
    Uint32 cnt;

    DBG(USB_CPPI4_DEBUG_LEVEL,
        "\n+ usbInitRxChannel:Ch=%d", chInfo->chNum);

    retCode = PAL_osMemAlloc(0, sizeof(usbRxCppiCh), 0, (Ptr *) & rxCppi);
    if ( retCode != PAL_SOK )
    {
        DBG(1,"\nERROR:usbInitRxChannel: Failed to allocate memory for RX CPPI Ch %d",
            chInfo->chNum);
        goto allocRxCppiFail;
    }
    PAL_osMemSet(rxCppi, 0, sizeof(usbRxCppiCh));

    spin_lock_init (&rxCppi->bdlock);

    rxCppiChannel->rxCppi = rxCppi;
    rxCppiChannel->pController = cppi;

    rxCppi->chInfo = *chInfo;
    rxCppi->chInfo.chState = USB_CPPI4_CH_INITIALIZED;

    rxCppi->rxQueue = chInfo->cppi4RxChInfo.rxCompQueue;

    for ( cnt = 0; cnt < USBOTG_NUM_RXFDB_QS; cnt++ )
    {
        rxCppi->fdbQueue [cnt] = chInfo->cppi4RxChInfo.u.hostPktCfg.fdbQueue [cnt];
    }


    accChCfg = &rxCppi->chInfo.rxAccChInfo;
    accChCfg->queue.qMgr          = chInfo->rxAccChInfo.queue.qMgr;
    accChCfg->queue.qNum          = chInfo->rxAccChInfo.queue.qNum;

    accChCfg->pacingTickCnt       = chInfo->rxAccChInfo.pacingTickCnt;
    accChCfg->list.maxPageEntry   = chInfo->rxAccChInfo.list.maxPageEntry ;
    accChCfg->list.pacingMode     = chInfo->rxAccChInfo.list.pacingMode;
    accChCfg->list.stallAvoidance = chInfo->rxAccChInfo.list.stallAvoidance ;

    accChCfg->list.listCountMode  = chInfo->rxAccChInfo.list.listCountMode;
    accChCfg->list.listEntrySize  = chInfo->rxAccChInfo.list.listEntrySize;
    accChCfg->list.maxPageCnt     = chInfo->rxAccChInfo.list.maxPageCnt;

    rxCppi->accChNum    = accChCfg->accChanNum;
    rxCppi->accVecNum   = chInfo->accVecNum;

    /* allocate memory for accumulator buf list */
    rxCppi->accListSize = (USB_ACC_LIST_DIV) * ((USB_ACC_RX_MAXENTRIES)
                                                * (USB_ACC_ENTRY_SIZE));
    retCode = PAL_osMemAlloc(0, rxCppi->accListSize, USB_ACC_RX_MAXENTRIES, &accChCfg->list.listBase);
    if ( retCode != PAL_SOK )
    {
        DBG(1, "\nERROR:cpmacInitTxChannel:Failed to allocate "
            "%d bytes for accumulator list for ch %d",
            rxCppi->accListSize, chInfo->chNum);

        goto errorOut;
    }

    /* assign the accumulator page base address, it is
       based on number of pages in list buffer */
    for ( cnt = 0; cnt < USB_ACC_LIST_DIV; cnt++ )
        rxCppi->listBuffBase[cnt] = (Ptr) ((Uint32)accChCfg->list.listBase +
                                           (cnt * (USB_ACC_RX_MAXENTRIES) * (USB_ACC_ENTRY_SIZE)));

    /* activeListRgn indicates anticipated page index for next interrupt,
     * Incremented on each interrupt and rolls over at USB_ACC_LIST_DIV..
     */

    *(Uint32*)rxCppi->listBuffBase[0] = 0;


    PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(accChCfg->list.listBase, rxCppi->accListSize);

    rxCppiChannel->rxTeardownPending = False;    /* Clear the RX teardown pending flag */

    /* TODO: We may need to check numBD here since CPPI4.1 requires the
     * desc count to be * multiple of 2 power 5 (ie 32). Similarly, the desc
     * size should also be multiple of 32.
     */
    rxCppi->bdMem = (Char *)alloc_buff_descriptor( cppi, chInfo->numBD);
    rxCppi->numBD = chInfo->numBD;

    if ( rxCppi->bdMem == NULL )
    {
        DBG (1,"\nERROR:usbInitRxChannel: Failed to allocate Rx descriptor");
        retCode = USB_ERR_CPPI_DESC_REGN_FAIL;
        goto errorOut;
    }
    rxCppi->allocSize = (sizeof(UsbCppi4HostDesc) * chInfo->numBD);

    rxCppi->numBD = chInfo->numBD;
    rxCppi->numOfBdInFdbQueue = 0;

    currBD = (UsbCppi4HostDesc *) rxCppi->bdMem;
    rxCppi->bdPoolHead = 0;

    /* "Slice" BD's one-by-one from the chunk, allocate a buffer and token, */
    for ( cnt = 0; cnt < chInfo->numBD; cnt++ )
    {

        /* Update the hardware descriptor */
        currBD->descInfo    = CPPI4_DESC_TYPE_HOST << CPPI4_DESC_TYPE_SHIFT_HOST;
        currBD->tagInfo     = 0;
        currBD->pktInfo     = (CPPI4_PKT_TYPE_USB << CPPI4_PKT_TYPE_SHIFT)
                              | (CPPI4_PKT_RETPLCY_FULL << CPPI4_PKT_RETPLCY_SHIFT)
                              | (CPPI4_DESC_LOC_OFFCHIP << CPPI4_DESC_LOC_SHIFT)
                              | (rxCppi->fdbQueue[0].qMgr << CPPI4_PKT_RETQMGR_SHIFT)
                              | (rxCppi->fdbQueue[0].qNum << CPPI4_PKT_RETQ_SHIFT);
        currBD->buffLen     = 0;
        currBD->bufPtr      = 0;
        currBD->nextBDPtr   = 0;
        currBD->orgBuffLen  = 0;
        currBD->orgBufPtr   = 0;
        PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
        currBD->nextSwBDPtr = (Ptr)rxCppi->bdPoolHead;
        rxCppi->bdPoolHead = currBD;
        currBD = (UsbCppi4HostDesc*) ((Uint32)currBD + rxCppi->chInfo.descAlignment);

    }

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    rxCppi->epRxChInfo = ppd_usbRxChInfo[chIndex];
#endif


    rxCppiChannel->rxIsCreated = True;

    return(USB_SUCCESS);

    errorOut:
    /* Free rx cppi channel memory */
    PAL_osMemFree(0, rxCppi, sizeof(usbRxCppiCh));
    allocRxCppiFail:
    DBG(USB_CPPI4_DEBUG_LEVEL,
        "\n- usbInitRxChannel:Ch=%d",chInfo->chNum);

    return(retCode);
}

/**
 *  usbUnInitRxChannel
 *    - Frees memory previously allocated for Ch Control structure, buffer
 *      descriptors
 *
 *  \note 1. This function assumes that the channel number passed is valid and
 *           this function will not do any error check to avoid duplicate error
 *           checks (done in caller function).
 */
PRIVATE PAL_Result usbUnInitRxChannel(struct cppi *cppi, Uint32 channel)
{
    PAL_Result retCode;
    usbRxCppiCh *rxCppi;

    DBG(USB_CPPI4_DEBUG_LEVEL,
        "\n+ usbUnInitRxChannel: Ch=%d",channel);

    /* Check if channel structure is already de-allocated */
    if ( cppi->rxCppiCh[channel].rxIsCreated == False )
    {
        DBG(1,
            "\nERROR:usbUnInitRxChannel:RX CPPI Ch %d structure already freed",channel);
        return(USB_ERR_RX_CH_ALREADY_CLOSED);
    }

    rxCppi = cppi->rxCppiCh[channel].rxCppi;

    retCode = free_buff_descriptor( cppi, (UsbCppi4HostDesc *)rxCppi->bdMem);

    rxCppi->bdMem = NULL;

    if ( retCode != 0 )
    {
        DBG (USB_CPPI4_DEBUG_LEVEL,
             "\nERROR:usbUnInitRxChannel:Failed to free RX desc region for Ch %d",
             channel);
    }

#ifdef USB_USE_ACC_LIST
    retCode = PAL_osMemFree(0, rxCppi->chInfo.rxAccChInfo.list.listBase,
                            rxCppi->accListSize);
    if ( retCode != PAL_SOK )
    {
        DBG(USB_CPPI4_DEBUG_LEVEL,
            "\nERROR:iusbUnInitRxChannel: Failed to free RX accumulator list for Ch %d",
            channel);
    }
#endif

    /* Free the RX Channel structure */
    retCode = PAL_osMemFree(0, rxCppi, sizeof(usbRxCppiCh));
    if ( retCode != PAL_SOK )
    {
        DBG(USB_CPPI4_DEBUG_LEVEL,
            "\nERROR:usbUnInitRxChannel: Failed to free RX CPPI channel structure for Ch %d",
            channel);
    }

    cppi->rxCppiCh[channel].rxCppi = NULL;
    cppi->rxCppiCh[channel].rxIsCreated = False;

    DBG(USB_CPPI4_DEBUG_LEVEL,"\n- usbUnInitRxChannel:Ch=%d",
        channel);

    return(USB_SUCCESS);

}
/*
 * cppi_controller_start
 * @pPrivateData : driver's private data & control structure
 * This function initializes the cppi4 tx/rx channel,
 */
static int  cppi_controller_start(struct cppi *pController)
{
    struct cppi *               cppi ;
    void        *__iomem        regBase;
    UsbCppi4HostDesc *          BDPtr;
    struct bdMemChunk *         bdPool;
    int                         i;
    int                         retVal;
    int                         cnt;
    Cppi4AccumulatorCfg*        accChCfg;

    dprintk( "%s: Starting the controller \n", __FUNCTION__ );

    cppi = pController;

    /* initialize the cppi4PAL object */
    pController->cppi4PAL = (Cppi4PALObj *)PAL_cppi4Init(NULL, NULL);

    /* initialzie the tx/rx channel configuration data */
    usbCppi4InitChInfo();

    /* allocate free Buffer Descriptor pool for all tx/rx endpoints */
    pController->bdMem = PAL_cppi4AllocDesc(pController->cppi4PAL,
                                            USB_CPPI4_QMGR_NUM,  USB_CPPI4_MAX_BD , USB_CPPI4_HOST_DESC_ALGN);
    if ( NULL == pController->bdMem )
    {
        DBG(USB_CPPI4_DEBUG_LEVEL, "\nERROR:BD allocation fail%s","");
        return 0;
    }
    pController->numBD              = USB_CPPI4_MAX_BD;
    pController->txdma_IntrLine     = USB_CPPI_TXDMA_INTR;
    pController->rxdma_IntrLine     = USB_CPPI_RXDMA_INTR;
    pController->txIsrAttachedFlag  = 0;
    pController->rxIsrAttachedFlag  = 0;

    /* tx dma interrupt setup */
    pController->accChNum           = USB_ACC_TX_CHNUM;
    pController->accVecNum          = USB_ACCTX_INTD_VEC;


    pController->accChCfg           = usbTxChInfo[0].txAccChInfo;
    accChCfg                        = &pController->accChCfg;

    accChCfg->queue.qMgr            = USB_CPPI41_TXCMPL_QMGR;
    accChCfg->queue.qNum            = USB_CPPI41_TXCMPL_QNUM;

    pController->txCmplQueue        = accChCfg->queue;

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    pController->cppi4InfraTxChOpenFlag = 0;
#endif

#ifdef USB_CPPI_TX_TASKLET_MODE
    tx_musb_tasklet_scheduled   = 0;
    tx_taskInit                 = 0;
#endif

#ifdef USB_CPPI_RX_TASKLET_MODE
    rx_musb_tasklet_scheduled   = 0;
    rx_taskInit                 = 0;
#endif

    /* initialize the buffer pool memory */
    bdPool  = pController->bdPool;
    BDPtr   = (UsbCppi4HostDesc *)pController->bdMem;
    for ( cnt = 0; cnt < CPPI41_MAX_USB_EPS; ++cnt )
    {
        bdPool[cnt].base      = (Char *)BDPtr;
        bdPool[cnt].numBD     = CPPI41_USB_BD_SIZE;
        bdPool[cnt].allocated = 0;
        BDPtr += bdPool[cnt].numBD;
    }


    for ( i = 0; i < ARRAY_SIZE(pController->txCppiCh); i++ )
    {
        PAL_osMemSet(&pController->txCppiCh[i], 0, sizeof(struct cppi_channel));
        pController->txCppiCh[i].bTransmit = TRUE;
        pController->txCppiCh[i].chNo = i;
        pController->txCppiCh[i].txIsOpen = 0;
    }
    for ( i = 0; i < ARRAY_SIZE(pController->rxCppiCh); i++ )
    {
        PAL_osMemSet(&pController->rxCppiCh[i], 0, sizeof(struct cppi_channel));
        pController->rxCppiCh[i].bTransmit = FALSE;
        pController->rxCppiCh[i].chNo = i;
        pController->rxCppiCh[i].rxIsOpen = 0;
    }

    /* configure the tx channel */
    for ( i = 0; i < ARRAY_SIZE(pController->txCppiCh); i++ )
    {
        retVal = usbCppi4InitTxChannel(pController, &pController->txCppiCh[i],
                                       &usbTxChInfo[i]);
        if ( USB_SUCCESS != retVal )
        {
            DBG(USB_CPPI4_DEBUG_LEVEL, "\n usbCppi4InitTxChannel failed for tx %d",i);
            dprintk("\n usbCppi4InitTxChannel failed for tx %d",i);
            return 1;
        }
    }

    /* configure the rx channel  */
    for ( i = 0; i < ARRAY_SIZE(pController->rxCppiCh); i++ )
    {
        retVal = usbCppi4InitRxChannel(pController, &pController->rxCppiCh[i],
                                       &usbRxChInfo[i],i);
        if ( USB_SUCCESS != retVal )
        {
            DBG(USB_CPPI4_DEBUG_LEVEL, "\n usbCppi4InitRxChannel failed for rx %d",i);
            return 2;
        }

    }

    /* initialize the tx completion queue */
    pController->txCmplQueueHnd   = PAL_cppi4QueueOpen (pController->cppi4PAL,
                                                        pController->txCmplQueue);

    if ( NULL == pController->txCmplQueueHnd )
    {
        DBG(1,"Error:cppiChAlloc, QueueOpen Fail,txCmplQueue%s","");
    }

    /* allocate list buffer pages of accumaltor */
    pController->accListSize = (USB_ACC_LIST_DIV) * ((USB_ACC_TX_MAXENTRIES)
                                                     * (USB_ACC_ENTRY_SIZE));
    retVal = PAL_osMemAlloc(0,  pController->accListSize, USB_ACC_TX_MAXENTRIES,
                            &accChCfg->list.listBase);
    if ( retVal != PAL_SOK )
    {
        DBG(1,"\ntx unable allocate accumulator list buffer "
            "%d bytes for accumulator list ",
            cppi->accListSize);
        return 3;
    }

    for ( cnt = 0; cnt < USB_ACC_LIST_DIV; cnt++ )
    {
        pController->listBuffBase[cnt] = (Ptr) ((Uint32)accChCfg->list.listBase +
                                                (cnt * (USB_ACC_TX_MAXENTRIES) * (USB_ACC_ENTRY_SIZE)));
    }

    for ( cnt  = 0; cnt < USB_ACC_LIST_DIV; ++cnt )
    {
        *(Uint32*)pController->listBuffBase[cnt] = 0;
    }

    PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(accChCfg->list.listBase, pController->accListSize);

    if (!pController->txIsrAttachedFlag)
    {
        /* attach accumulator - cppi tx dma interrupt service routine */
        retVal = request_irq( pController->txdma_IntrLine, usbCppiTxPktProcess_Isr,
                              IRQF_DISABLED, dev_name(pController->musb->controller), pController->musb);
        if ( PAL_SOK != retVal )
        {
            DBG(1,
                "\nERROR:cppi_controller_start,request_irq failed %d",
                retVal);
        }

        dprintk("registered tx-dma irq %d \n", pController->txdma_IntrLine);

        /* enable the interrupt corresponding to accumulator channel */
        enableTxIntr(pController->accChNum);
        pController->txIsrAttachedFlag = 1;
    }

    if ( !pController->rxIsrAttachedFlag )
    {

        /* attach accumulator - cppi rx dma interrupt service routine */
        retVal = request_irq( pController->rxdma_IntrLine,usbCppiRxPktProcess_Isr,
                              IRQF_DISABLED, dev_name(pController->musb->controller), pController->musb);
        if ( PAL_SOK != retVal )
        {
            DBG(1,
                "\nERROR:cppi_controller_start,request_irq failed %d",
                retVal);
        }

        dprintk("registered rx-dma irq %d \n", pController->rxdma_IntrLine);

        retVal = enableRxIntr( pController->rxCppiCh[2].rxCppi );

        if ( PAL_SOK != retVal )
        {
            DBG(1,
                "\nERROR:cppi_controller_start,enableRxIntr failed %d",
                retVal);
        }
        pController->rxIsrAttachedFlag = 1;
    }


#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    pController->infraTxChInfo.cppi4TxChInfo.chNum = USB_TX_INFRA_CHNUM;
    pController->infraTxChInfo.cppi4TxChInfo.dmaNum = USB_PPD_INFRA_TXDMA_NUM; //USB_INFRA_CH_DMA_BLOCK_NUM;
    pController->infraTxChInfo.cppi4TxChInfo.tdQueue.qMgr = DMA1_CPPI4x_FTD_QMGR;
    pController->infraTxChInfo.cppi4TxChInfo.tdQueue.qNum = DMA1_CPPI4x_FTD_QNUM;
    pController->infraTxChInfo.cppi4TxChInfo.defDescType  = CPPI41_DESC_TYPE_HOST;
    pController->infraTxChInfo.chDir = 0; //CH_DIR_TX;
    pController->infraTxChInfo.dmaMode = CPPI41_DMA_MODE_INFRA;
    pController->cppi4InfraTxChOpenFlag = 0;
    pController->UsbHostPrxyQ.qMgr = USB_CPPI4x_USB_TO_HOST_PRXY_QMGR;
    pController->UsbHostPrxyQ.qNum = USB_CPPI4x_USB_TO_HOST_PRXY_QNUM(0);
#endif

    /* Do Necessary configuartion in H/w to get started */
    regBase =  pController->mregs - PUMA5_BASE_OFFSET;

    INIT_LIST_HEAD(&pController->tx_complete);

    /* disable auto request mode */
    musb_writel(regBase, PUMA5_AUTOREQ_REG, 0);

    /* disable the CDC/RNDIS modes */
    musb_writel(regBase, PUMA5_MODE_TGCR_REG,  0);

    return 0;
}

/*
 *  Stop Dma controller
 *
 *  De-Init the Dma Controller as necessary.
 */

static int  cppi_controller_stop(struct dma_controller *c)
{
    struct cppi     *pController ;
    void __iomem        *regBase;
    struct musb *pThis;
    int         i, retVal;

    pController = container_of(c, struct cppi, Controller);
    pThis = pController->musb;

    /* DISABLE INDIVIDUAL CHANNEL Interrupts */
    if ( pController->txIsrAttachedFlag )
    {
        free_irq(  pController->txdma_IntrLine , pThis);
        pController->txIsrAttachedFlag = 0;
    }

    if ( pController->rxIsrAttachedFlag )
    {
        free_irq(  pController->rxdma_IntrLine, pThis );
        pController->rxIsrAttachedFlag = 0;
    }

    DBG(USB_CPPI4_DEBUG_LEVEL, "Tearing down RX and TX Channels\n");
    for ( i = 0; i < ARRAY_SIZE(pController->txCppiCh); i++ )
    {
        usbUnInitTxChannel(pController, i );
#ifdef USB_CPPI4_GETSTATS
        pController->txIntCount[i] = 0;
#endif
    }

    /* un-initialize the RxChannel */
    for ( i = 0; i < ARRAY_SIZE(pController->rxCppiCh); i++ )
    {
        usbUnInitRxChannel(pController, i );
#ifdef USB_CPPI4_GETSTATS
        pController->rxIntCount[i] = 0;
#endif
    }

    /* free the host descriptor allocated for all tx/rx channels */
    retVal = PAL_cppi4DeallocDesc(pController->cppi4PAL, USB_CPPI4_QMGR_NUM, pController->bdMem);
    if ( retVal != 0 )
    {
        DBG (USB_CPPI4_DEBUG_LEVEL,"\nERROR:cppi_controller_stop:Failed to free cppihost desc");
    }

    pController->txIsrAttachedFlag = 0;
    pController->rxIsrAttachedFlag = 0;


    regBase =  pController->mregs - PUMA5_BASE_OFFSET;

    /* TODO */
    INIT_LIST_HEAD(&pController->tx_complete);

    /* disable auto request mode */
    musb_writel(regBase, PUMA5_AUTOREQ_REG, 0);

    /* disable the CDC/RNDIS modes */
    musb_writel(regBase, PUMA5_MODE_TGCR_REG,  0);

    return TRUE;
}

/*
 * Allocate a CPPI Channel for DMA.  With CPPI, channels are bound to
 * each transfer direction of a non-control endpoint, so allocating
 * (and deallocating) is mostly a way to notice bad housekeeping on
 * the software side.  We assume the irqs are always active.
 */
static struct dma_channel *
cppi_channel_allocate( struct dma_controller *  c,
                       struct musb_hw_ep *      ep,
                       u8                       bTransmit )
{
    struct cppi *           pController;
    u8                      chNum,indx;
    struct cppi_channel *   txCh;
    struct cppi_channel *   rxCh;
    usbTxCppiCh *           txCppi;
    usbRxCppiCh *           rxCppi;
    Ptr                     cppi4TxChHnd;
    Ptr                     cppi4RxChHnd;
    struct musb *           pThis;
    Int32                   retVal;
    int                     bLocalEnd = ep->epnum;

    dprintk( "%s: Allocating DMA channel for ep%d\n", __FUNCTION__, bLocalEnd );

    pController = container_of(c, struct cppi, Controller);
    pThis       = pController->musb;

    /* remember bLocalEnd: 1..Max_EndPt, and cppi ChNum:0..Max_EndPt-1 */
    chNum = bLocalEnd - 1;

    if (1 == pController->txIsrAttachedFlag)
    {
        pController->txAccChHnd = PAL_cppi4AccChOpen( pController->cppi4PAL, &pController->accChCfg );

        if ( pController->txAccChHnd != NULL )
        {
            pController->listEntryPtr = PAL_cppi4AccChGetNextList(pController->txAccChHnd);
            pController->txIsrAttachedFlag++;
        }
        else
        {
            DBG(1,"\n Tx-PAL_cppi4AccChOpen() failed");
            return NULL;
        }
    }

    /* as of now, just return the corresponding CPPI Channel Handle */
    if ( bTransmit )
    {
        if ( bLocalEnd > ARRAY_SIZE(pController->txCppiCh) )
        {
            dprintk("no %cX DMA channel for ep%d\n", 'T', bLocalEnd);
            return NULL;
        }

        txCh = &pController->txCppiCh[ chNum ];

        if ( !txCh->txIsCreated )
        {
            dprintk("Cppi Tx channel is not created chnum=%d\n",chNum);
            return NULL;
        }

        txCppi              = txCh->txCppi;
        txCppi->palCppi4Hnd = pController->cppi4PAL;

        if ( txCh->txIsOpen == True )
        {
            dprintk("txDma channel already allocated\n");
            goto tx_done;
        }

        if ( txCh->pEndPt )
        {
            dprintk("re-allocating DMA%d TX channel %p\n",chNum,txCh);
        }


        for ( indx = 0; indx < txCppi->numTxQs; indx++ )
        {
            txCppi->txQueueHnd[indx] =  PAL_cppi4QueueOpen(pController->cppi4PAL,
                                                           txCppi->txQueue[indx]);
            if ( NULL == txCppi->txQueueHnd[indx] )
            {
                dprintk("Error:cppiChAlloc, QueueOpen fail txQueueHnd%s\n","");
                return NULL;
            }
        }
        /* open the cppi4 dma channel in the CPPI4 PAL */
        cppi4TxChHnd = PAL_cppi4TxChOpen(pController->cppi4PAL,
                                         &txCppi->chInfo.cppi4TxChInfo,NULL);
        if ( NULL == cppi4TxChHnd )
        {
            dprintk("cppi_alloc_channel: Error cppiTxChOpen for channel %d",chNum);
            goto txCppiAlloc_fail2;
        }
        txCppi->cppi4TxChHnd = cppi4TxChHnd;

        /* enable the cppi4 dma channel */
        retVal = PAL_cppi4EnableTxChannel(txCppi->cppi4TxChHnd, NULL);
        if ( PAL_SOK != retVal )
        {
            dprintk("Cppi_alloc_channel: Error in EnableTxChannel\n");
            goto txCppiAlloc_fail3;
        }

        tx_done:

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
        if ( !pController->cppi4InfraTxChOpenFlag )
        {
            Ptr cppi4InfraTxChHnd;
            cppi4InfraTxChHnd = PAL_cppi4TxChOpen(pController->cppi4PAL,
                                                  &pController->infraTxChInfo.cppi4TxChInfo,NULL);
            dprintk(" %s : cppiTxChHnd  = %p,chNum = %d opened\n",__FUNCTION__,cppi4InfraTxChHnd,
                    pController->infraTxChInfo.cppi4TxChInfo.chNum);

            if ( cppi4InfraTxChHnd == NULL )
            {
                DBG(1,"\nERROR:ncppi_alloc_channel: Error CPPI4 Tx PAL_cppi4TxChOpen()");
                goto txCppiAlloc_fail2;
            }
            pController->cppi4InfraTxChOpenFlag = 1;
            pController->cppi4InfraTxChHnd = cppi4InfraTxChHnd;
            retVal = PAL_cppi4EnableTxChannel(cppi4InfraTxChHnd, NULL);
            if ( PAL_SOK != retVal )
            {
                DBG(1, "\nCppi_alloc_channel: Error in EnableTxChannel");
                goto txCppiAlloc_fail3;
            }
        }
#endif

        /* set the all the tx endpoint to Generic mode */
        switch ( ep->dma_mode )
        {
        case 1 :
            cppi_mode_update(txCh, 1, ENDPOINT_MODE_CDC );
            dprintk("tx: %d :[%s] cdc mode configured\n", ep->dma_mode,__FUNCTION__);
            break;
        case 2:
            cppi_mode_update(txCh, 1, ENDPOINT_MODE_RNDIS );
            dprintk("tx: %d :[%s] rndis mode configured\n", ep->dma_mode,__FUNCTION__);
            break;
        default :
            cppi_mode_update(txCh, 1, DEFAULT_TX_ENDPOINT_MODE );
            dprintk("tx: %d :[%s] generic rndis mode configured\n", ep->dma_mode,__FUNCTION__);
        }

        txCh->pEndPt = ep;
        txCh->Channel.status = MUSB_DMA_STATUS_FREE;
        txCh->Channel.private_data = txCh;
        /* redefine this value in order to get correct request_size in dma transfers */
        txCh->Channel.max_len = 0x7fffffff;
        txCh->chNo = chNum;

        txCh->txIsOpen = True;
        txCppi->chInfo.chState = USB_CPPI4_CH_OPENED;

        DBG(USB_CPPI4_DEBUG_LEVEL,"Allocate CPPI4 TxCh%d for ep%d\n",chNum, bLocalEnd);

        return &txCh->Channel;

        txCppiAlloc_fail3 :
        PAL_cppi4TxChClose( txCppi->cppi4TxChHnd, NULL );

        txCppiAlloc_fail2 :

        PAL_cppi4QueueClose(  pController->cppi4PAL,txCppi->txQueueHnd[0] );

        return(NULL);

    }
    else
    {
        if ( bLocalEnd > ARRAY_SIZE(pController->rxCppiCh) )
        {
            DBG(1, "no %cX DMA channel for ep%d\n", 'R', bLocalEnd);
            return NULL;
        }

        rxCh = &pController->rxCppiCh[chNum];
        rxCppi = rxCh->rxCppi;

        if ( NULL == rxCppi )
        {
            DBG(1, "\n+-cppi_channel_allocate: rxCppi structure NULL:%d",chNum);
            return(NULL) ;
        }
        if ( !rxCh->rxIsCreated )
        {
            DBG(1,"Cppi Rx channel is not created chnum=%d",chNum);
            return NULL;
        }

        rxCppi->palCppi4Hnd = pController->cppi4PAL;

        if ( rxCh->pEndPt )
        {
            DBG(1, "re-allocating DMA%d RX channel %p\n",chNum,rxCh);
        }

        if ( rxCh->rxIsOpen == True )
        {
            dprintk("rxDma channel already allocaated\n");
            goto rx_done;
        }

        rxCppi->rxQueueHnd = PAL_cppi4QueueOpen(pController->cppi4PAL,
                                                rxCppi->rxQueue);

        if ( NULL == rxCppi->rxQueueHnd )
        {
            DBG(1, "\ncppi_channel_alloc:QueuOpen Error for rxQueue");
            return NULL;
        }

        rxCppi->fdbQueueHnd[0] = PAL_cppi4QueueOpen(pController->cppi4PAL,
                                                    rxCppi->fdbQueue[0]);

        if ( NULL == rxCppi->fdbQueueHnd[0] )
        {
            DBG(1, "\ncppi_channel_alloc:QueuOpen Error for rxQueue");
            goto rxCppiAlloc_fail1 ;
        }

        cppi4RxChHnd = PAL_cppi4RxChOpen(pController->cppi4PAL,
                                         &rxCppi->chInfo.cppi4RxChInfo, NULL);

        dprintk(" %s : cppiRxChHnd  = %p,chNum = %d opened, rxcmpQ=%d\n",__FUNCTION__,cppi4RxChHnd,
                rxCppi->chInfo.cppi4RxChInfo.chNum,
                rxCppi->chInfo.cppi4RxChInfo.rxCompQueue.qNum );

        if ( NULL == cppi4RxChHnd )
        {
            DBG(1,
                "\ncppi_channel_alloc:Error cppi4RxChOpen");
            goto rxCppiAlloc_fail2 ;
        }
        rxCppi->cppi4RxChHnd = cppi4RxChHnd;

        rxCppi->rxAccChHnd = PAL_cppi4AccChOpen(pController->cppi4PAL, &rxCppi->chInfo.rxAccChInfo);
        if ( rxCppi->rxAccChHnd == NULL )
        {
            DBG(1,"\nERROR:ncppi_channel_alloc: CPPI4 Rx PAL_cppi4AccChOpen()");
            goto rxCppiAlloc_fail2 ;
        }

        rxCppi->listEntryPtr = PAL_cppi4AccChGetNextList(rxCppi->rxAccChHnd);

        /* enable the rx channel */
        retVal = PAL_cppi4EnableRxChannel ( rxCppi->cppi4RxChHnd, NULL );
        if ( PAL_SOK != retVal )
        {
            DBG(1,
                "\nERROR:cppi_alloc_channel,cppi4EnableRxChannel failed %d",
                retVal);
            goto rxCppiAlloc_fail3;
        }

        rx_done:

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
        /* open the channel in the CPPI4 PAL */
        cppi4RxChHnd = PAL_cppi4RxChOpen(pController->cppi4PAL,
                                         &rxCppi->epRxChInfo.cppi4RxChInfo, NULL);
        dprintk(" %s : cppiRxChHnd  = %p,chNum = %d opened, rxcmpQ=%d\n",__FUNCTION__,cppi4RxChHnd,rxCppi->epRxChInfo.cppi4RxChInfo.chNum,
                rxCppi->epRxChInfo.cppi4RxChInfo.rxCompQueue.qNum );

        if ( NULL == cppi4RxChHnd )
        {
            DBG(1,
                "\ncppi_channel_alloc:Error cppi4RxChOpen");
            goto rxCppiAlloc_fail2 ;
        }
        rxCppi->cppi4InfraRxChHnd = cppi4RxChHnd;
        retVal = PAL_cppi4EnableRxChannel ( rxCppi->cppi4InfraRxChHnd, NULL );
        if ( PAL_SOK != retVal )
        {
            DBG(1,
                "\nERROR:cppi_alloc_channel,cppi4EnableRxChannel failed %d",
                retVal);
            goto rxCppiAlloc_fail3;
        }
#endif

        rxCh->pEndPt = ep;
        rxCh->Channel.status = MUSB_DMA_STATUS_FREE;
        rxCh->Channel.private_data = rxCh;
        /* redefine this value in order to get correct request_size in dma transfers */
        rxCh->Channel.max_len = 0x7fffffff;
        DBG(USB_CPPI4_DEBUG_LEVEL,"Allocate CPPI RxCh%d for ep%d\n",chNum,bLocalEnd);

        rxCh->rxIsOpen = True;
        rxCppi->chInfo.chState = USB_CPPI4_CH_OPENED;
        rxCh->chNo = chNum;
#ifdef CONFIG_USB_MUSB_HOST
        if ( (ep->max_packet_sz_rx  % 64 != 0) && ( ep->max_packet_sz_rx  < 64) )
            cppi_mode_update(rxCh, 0, ENDPOINT_MODE_TRANSPARENT );
        else
#endif
        {
            switch ( ep->dma_mode )
            {
            case 1 :
                cppi_mode_update(rxCh, 0, ENDPOINT_MODE_CDC );
                dprintk("rx :%d :[%s] cdc mode configured\n", ep->dma_mode,__FUNCTION__);
                break;
            case 2:
                cppi_mode_update(rxCh, 0, ENDPOINT_MODE_RNDIS );
                dprintk("rx: %d :[%s] rndis mode configured\n", ep->dma_mode,__FUNCTION__);
                break;
            default :
                cppi_mode_update(rxCh, 0, DEFAULT_TX_ENDPOINT_MODE );
                dprintk("rx: %d :[%s] generic rndis mode configured\n", ep->dma_mode,__FUNCTION__);
            }
        }

        return &rxCh->Channel;

        rxCppiAlloc_fail3 :
        PAL_cppi4RxChClose( rxCppi->cppi4RxChHnd , NULL);

        rxCppiAlloc_fail2 :
        PAL_cppi4QueueClose( pController->cppi4PAL,rxCppi->fdbQueueHnd[0] );

        rxCppiAlloc_fail1 :
        PAL_cppi4QueueClose(  pController->cppi4PAL,rxCppi->rxQueueHnd );

        return NULL;

    }
}

/* Release a CPPI Channel.  */
static void cppi_channel_release(struct dma_channel *channel)
{
    struct cppi_channel *c;
    struct cppi_channel *otgChannel = channel->private_data;

    /* REVISIT:  for paranoia, check state and abort if needed... */
    c = container_of(channel, struct cppi_channel, Channel);
    if ( !c->pEndPt )
        DBG(1, "releasing idle DMA channel %p\n", c);

    /* but for now, not its IRQ */
    c->pEndPt = NULL;
    channel->status = MUSB_DMA_STATUS_UNKNOWN;
    otgChannel->Channel.private_data = 0;
    otgChannel->chNo = 0;
}

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    #define ALL_EP_RNDIS_MODE   0x11111111
    #define ALL_EP_CDC_MODE     0x22222222
#endif

void cppi_mode_update ( struct cppi_channel *c, int bTransmit, int mode )
{
    struct cppi *pController; // = c->pController;
    Uint32 chNum = c->chNo,regVal;
    void                    *__iomem mbase;
    void                    *__iomem regBase;

    pController = c->pController;

    mbase = pController->mregs;
    regBase = mbase - PUMA5_BASE_OFFSET;

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    if ( mode == ENDPOINT_MODE_RNDIS )
        regVal = ALL_EP_RNDIS_MODE;
    else if ( mode == ENDPOINT_MODE_CDC )
        regVal = ALL_EP_CDC_MODE;
    else
    {
#endif
        if ( bTransmit )
        {
            c->bLastMode = mode & 0x3;
            regVal = musb_readl(regBase, PUMA5_MODE_TGCR_REG);
            regVal &= ~(0x3 << (chNum * 4));
            regVal |= (c->bLastMode << (chNum * 4));
        }
        else
        {
            c->bLastMode = mode & 0x3;
            regVal = musb_readl(regBase, PUMA5_MODE_TGCR_REG);
            regVal &= ~( (0x3 << (chNum * 4)) << 16);
            regVal |= ((c->bLastMode << (chNum * 4)) << 16);
        }
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    }
#endif

    musb_writel(regBase, PUMA5_MODE_TGCR_REG,regVal);

}

void set_rndis_epsize ( struct cppi_channel *c, Uint32 pktsize)
{
    struct cppi *pController = c->pController;
    Uint32 chNum = c->chNo,offset;
    void                    *__iomem mbase;
    void                    *__iomem regBase;


    mbase = pController->mregs;
    regBase = mbase - PUMA5_BASE_OFFSET;

    offset = PUMA5_GRNDIS_EP1SIZE_REG + 4 * chNum;

    musb_writel(regBase, offset, pktsize);
}

static inline int cppi_autoreq_update(struct cppi_channel *rx,
                                      void *__iomem tibase, int shortpkt, u32 length, u32 max_packet)
{
    u32 tmp, val;

    /* assert(is_host_active(musb)) */
    /* start from "AutoReq never" */
    tmp = musb_readl(tibase, PUMA5_AUTOREQ_REG);
    val = tmp & ~((0x3) << (rx->chNo * 2));
    /* HCD arranged reqpkt for packet #1.  we arrange int
     * for all but the last one, maybe in two segments.
     */

    if ( !shortpkt || (length > max_packet) )
    {
        rx->autoReq = 1;

        val |= (0x1 << (rx->chNo * 2));
    }
    else
    {
        rx->autoReq = 0;
        val &= ~((0x3) << (rx->chNo * 2));
    }

    if ( val != tmp )
        musb_writel(tibase, PUMA5_AUTOREQ_REG, val);

    return rx->autoReq;
}

void printBD(UsbCppi4HostDesc *currBD, int displayData)
{
    printk("\n currBD = %p", currBD);
    if ( currBD == NULL )
    {
        printk("---NULL BD !!");
        return ;
    }

    printk("\ntag=%x  descInfo=%x  bufPtr=%x  orgBufPtr=%x\nbufLen=%d OrgBLen=%d  tagId=%x",
           currBD->tagInfo, currBD->descInfo, currBD->bufPtr, currBD->orgBufPtr,
           currBD->buffLen, currBD->orgBuffLen,currBD->tagId);

    if ( displayData )
    {
        u8 *ptr;
        u32 i;

        ptr = (u8 *)PAL_CPPI4_PHYS_2_VIRT((char *)currBD->bufPtr);
        for ( i=0; i<currBD->buffLen; ++i )
        {
            if ( i % 32 == 0 )
                printk("\n %04x:",i);
            printk(" %02x", ptr[i]);
        }

    }

}
char gBuf[2] = {0};
/*
 * CPPI TX:
 * ========
 * cppi_next_tx_segment - dma write for the next chunk of a buffer
 * @musb: the controller
 * @tx: dma channel
 * @rndis : zero bit in the usb_request is set. Zero length packet
 *     has to be transmitted,if transmit buffer length is exact
  *    multiple of USB tx endpoint size.
 * Context: controller irqlocked
 *
 * the cppi rx channels are configured to work in Generic rndis mode
 */
static void
cppi_next_tx_segment(struct musb *musb, struct cppi_channel *tx, int rndis,struct dma_channel *pChannel)
{
    unsigned        maxpacket = tx->pktSize;
    size_t          length = tx->transferSize - tx->currOffset;
    unsigned        lockKey,zlp,n_bds;
    usbTxCppiCh *txCppi = tx->txCppi;
    UsbCppi4HostDesc *currBD,*sopBD, *nextBD;
    struct dma_controller *dma;

    dma = musb->dma_controller;

    /* check if the usb link is up */

    /*
     * \note: The critical section is required to protect the "txCppi" data
     * structure which is being used by this function and by usbTxBDProc().
     * Since the function requires to protect the resources in many places in the
         * function below, it would be wise to get and release the crit section only
         * once rather than doing it in many places below. Interrupt protection is
         * sought here so that this function or the usbTxBDProc() function can be
         * invoked in any context and still will be safe to do so.
         */

    /* if length of transmit buffer is multiple of usb endpoint size
       then send the zero length packet */
    zlp = 0;
    n_bds = 1;

#ifdef CONFIG_USB_MUSB_HOST
    if ( rndis )
        rndis = 0;
    if ( rndis && (length % maxpacket == 0) )
    {
#else
    if ( !rndis && (length % maxpacket == 0) )
    {
#endif
        zlp = 1;
        n_bds = 2;
    }
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    spin_lock (&txCppi->bdlock);
    /* get the free Transmit Host buffer descriptor from TX BD pool */
    currBD = usbGetFreeTxBDList(txCppi, n_bds);
    spin_unlock (&txCppi->bdlock);
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

    if ( currBD == NULL )
    {
#ifdef  USB_GET_STATS
        txCppi->outOfTxBD++;
#endif
        /* retVal = USB_ERR_TX_OUT_OF_BD;  TODO */
        DBG(4,"\n tx NULL BD ");
        return ;
    }

    sopBD = currBD;

    currBD->tagInfo   = /*txCppi->chInfo.chNum  << 16 */ 0;
    /* TODO source TagNumber(Bit31:27),
    ChannelNumber(Bit26:21), SubChannel Number(Bit20:16)
    Destination tag (Bit 16:0) */

    if ( zlp )
        currBD->descInfo  = (CPPI4_DESC_TYPE_HOST << CPPI4_DESC_TYPE_SHIFT_HOST)
                            | (length+1);
    else
        currBD->descInfo  = (CPPI4_DESC_TYPE_HOST << CPPI4_DESC_TYPE_SHIFT_HOST)
                            | length;

    currBD->bufPtr      = PAL_CPPI4_VIRT_2_PHYS((Int32 *)tx->startAddr);
    currBD->orgBufPtr   = currBD->bufPtr;
    currBD->buffLen     = length;
    if ( zlp )
        currBD->orgBuffLen  = length + 1;
    else
        currBD->orgBuffLen  = length;
    currBD->zlp         = 0;
    currBD->tagId       = ( tx->chNo << 4)| (tx->pEndPt->epnum);
#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    #ifdef CONFIG_TI_PACKET_PROCESSOR
    //PAL_osMemCopy((Ptr)&currBD->netInfoWord0,(Ptr)bufList->bufToken,EPI_HEADER_LEN);
    currBD->netInfoWord1 &= ~(0xFFFF);
    currBD->netInfoWord1 |= txCppi->txQueue[0].qNum;
    #endif
    currBD->netInfoWord1 = txCppi->txQueue[0].qNum;
#endif

#ifdef CONFIG_USB_MUSB_GADGET
    currBD->dwMaxLength = pChannel->max_len;
    currBD->dwActualLength = pChannel->actual_len;
    currBD->status = pChannel->status;
    currBD->desired_mode = pChannel->desired_mode;
#endif


    PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);

    if ( zlp )
    {

        nextBD = (UsbCppi4HostDesc *)currBD->nextSwBDPtr;
        if ( nextBD )
        {

            currBD->nextBDPtr = (Ptr)nextBD;
            nextBD->tagInfo   = /*txCppi->chInfo.chNum  << 16 */ 0;
            /* TODO source TagNumber(Bit31:27),
                ChannelNumber(Bit26:21), SubChannel Number(Bit20:16)
                Destination tag (Bit 16:0) */
            nextBD->descInfo  = (CPPI4_DESC_TYPE_HOST << CPPI4_DESC_TYPE_SHIFT_HOST);
            nextBD->bufPtr     = PAL_CPPI4_VIRT_2_PHYS((Int32 *)gBuf);
            nextBD->orgBufPtr  = nextBD->bufPtr;
            nextBD->buffLen    = 1;
            nextBD->orgBuffLen  = length+1;
            nextBD->nextBDPtr =  NULL ;  /* (Ptr) PAL_CPPI4_VIRT_2_PHYS(currBD->nextSwBDPtr); */
            nextBD->zlp = 1;
            PAL_CPPI4_CACHE_WRITEBACK(nextBD, CPPI4_BD_LENGTH_FOR_CACHE);
        }
        else
        {
            dprintk(" zlp next BD is null \n");
        }
    }
    else
        currBD->nextBDPtr   =  NULL ;  /* (Ptr) PAL_CPPI4_VIRT_2_PHYS(currBD->nextSwBDPtr); */

    /* TODO Add tx priority support . */
    PAL_cppi4QueuePush(txCppi->txQueueHnd[0],
                       (Ptr)PAL_CPPI4_VIRT_2_PHYS((u32)sopBD),
                       txCppi->chInfo.descAlignment, length);

}
/*
 * CPPI RX:
 * ========
 */

/**
 * cppi_next_rx_segment - dma read for the next chunk of a buffer
 * @musb: the controller
 * @rx: dma channel
 * @onepacket: true unless caller treats short reads as errors, and
 *  performs fault recovery above usbcore.
 * Context: controller irqlocked
 *
 * the cppi rx channels are configured to work in Generic rndis mode
 * this mode is identical to the normal RNDIS mode in neraly all respects
 * except for the exception case where data is being received or transmitted
 * and the last packet of the segment exaclty matches the max USB packet size.
 *  This mode will receive the USB packets in the same manner as RNDIS mode
 * closing the CPPI packets when a USB packet is rececieved that is less than
 * the endpoint size. otherwise the packet will be closed when the value in
 * Generic RNDIS EP Size register is reached.
 *  This mode will work for network class driver (CDC/RNDIS) as well as
 * for mass storage class, where there is no short packet.
 *
 */
static void
cppi_next_rx_segment(struct musb *musb, struct cppi_channel *rx, int shortpkt,struct dma_channel *pChannel)
{
    dma_addr_t      addr = rx->startAddr + rx->currOffset;
    size_t          length= rx->transferSize - rx->currOffset;
    usbRxCppiCh *rxCppi = rx->rxCppi;
    UsbCppi4HostDesc *currBD ;
    struct  cppi *cppi;
    struct dma_controller *dma;
    void                    *__iomem *regs;
    unsigned        lockKey;

    dma = musb->dma_controller;
    cppi = container_of(dma, struct cppi, Controller);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    spin_lock (&rxCppi->bdlock);
    /* get the free Rx Buffer Descriptor from free Rx BD Pool */
    currBD = usbGetFreeRxBDList(rxCppi, 1);
    spin_unlock (&rxCppi->bdlock);
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

    if ( currBD == NULL )
    {
#ifdef  USB_GET_STATS
        rxCppi->outOfRxBD++;
#endif
        DBG(4,"\n rx NULL BD  ");
        return ;
    }



    currBD->bufPtr     = PAL_CPPI4_VIRT_2_PHYS( (Int32 *)addr );
    currBD->buffLen    = length;
    currBD->orgBufPtr  = currBD->bufPtr;
    currBD->orgBuffLen = length;
    currBD->nextBDPtr  = NULL ; /* (Ptr) PAL_CPPI4_VIRT_2_PHYS(currBD->nextSwBDPtr); */
    currBD->tagId       = ( rx->chNo << 4)| (rx->pEndPt->epnum);

#ifdef CONFIG_USB_MUSB_GADGET
    currBD->dwMaxLength = pChannel->max_len;
    currBD->dwActualLength = pChannel->actual_len;
    currBD->status = pChannel->status;
    currBD->desired_mode = pChannel->desired_mode;
#endif
    PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);


    /* set received data length in Generic RNDIS ep1/2/3/4 size register */
    if ( length < 64 )
    {
        set_rndis_epsize ( rx, 64     );
    }
    else
    {
        set_rndis_epsize ( rx, length );
    }

    if ( is_host_active(musb) )
    {
        regs =  cppi->mregs - PUMA5_BASE_OFFSET;
        cppi_autoreq_update(rx, regs, shortpkt,length, rx->pEndPt->max_packet_sz_rx);
    }

    /* push the free rx Buffer descripotr to the fdbQueue */
    PAL_cppi4QueuePush(rxCppi->fdbQueueHnd[0], (Ptr)PAL_CPPI4_VIRT_2_PHYS((u32)currBD),
                       rxCppi->chInfo.descAlignment, 0);

    rxCppi->numOfBdInFdbQueue++;

}

/**
 * cppi_channel_program - program channel for data transfer
 * @pChannel: the channel
 * @wPacketSz: max packet size
 * @mode: For RX, 1 unless the usb protocol driver promised to treat
 *  all short reads as errors and kick in high level fault recovery.
 *  For TX, 0 unless the protocol driver _requires_ short-packet
 *  termination mode.
 * @dma_addr: dma address of buffer
 * @dwLength: length of buffer
 * Context: controller irqlocked
 */
static int  cppi_channel_program(struct dma_channel *pChannel,
                                 u16 wPacketSz, u8 mode,
                                 dma_addr_t dma_addr, u32 dwLength)
{
    struct cppi_channel *otgChannel = container_of(pChannel, struct cppi_channel,
                                                   Channel);
    struct cppi     *pController = otgChannel->pController;
    struct musb     *musb = pController->musb;

    switch ( pChannel->status )
    {
    case MUSB_DMA_STATUS_BUS_ABORT:
    case MUSB_DMA_STATUS_CORE_ABORT:
        /* fault irq handler should have handled cleanup */
        WARN("%cX DMA%d not cleaned up after abort!\n",
             otgChannel->bTransmit ? 'T' : 'R',
             otgChannel->chNo);
        //WARN_ON(1);
        break;
    case MUSB_DMA_STATUS_BUSY:
        WARN("program active channel?  %cX DMA%d\n",
             otgChannel->bTransmit ? 'T' : 'R',
             otgChannel->chNo);
        //WARN_ON(1);
        break;
    case MUSB_DMA_STATUS_UNKNOWN:
        DBG(1, "%cX DMA%d not allocated!\n",
            otgChannel->bTransmit ? 'T' : 'R',
            otgChannel->chNo);
        /* FALLTHROUGH */
    case MUSB_DMA_STATUS_FREE:
        break;
    }

#if defined(CONFIG_USB_MUSB_HOST) && !defined(CONFIG_USB_MUSB_DUAL_ROLE)
    pChannel->status = MUSB_DMA_STATUS_BUSY;
#endif

    /* set transfer parameters, then queue up its first segment */
    otgChannel->startAddr = dma_addr;
    otgChannel->currOffset = 0;
    otgChannel->pktSize = wPacketSz;
    otgChannel->actualLen = 0;
    otgChannel->transferSize = dwLength;
    otgChannel->rxMode = mode;
    otgChannel->reqcomplete = 0;
    otgChannel->autoReq = 0;

    /* TX channel? or RX? */
    if ( otgChannel->bTransmit )
        cppi_next_tx_segment(musb, otgChannel, mode,pChannel);
    else
        cppi_next_rx_segment(musb, otgChannel, mode,pChannel);

    return TRUE;
}
#ifdef DEBUG_CPPI_TD
struct musb *gpThis;
#endif
/* Instantiate a software object representing a DMA controller. */
struct dma_controller * __init
dma_controller_create(struct musb *musb, void __iomem *mregs)
{
    struct dma_controller   *pResult = NULL;
    struct cppi     *pController;
    int ret = 0;

    pController = kzalloc(sizeof *pController, GFP_KERNEL);
    if ( !pController )
        goto kzalloc_fail;

    /* Initialize the Cppi DmaController  structure */
    //pController->dma_completed = dma_completed;
    pController->mregs = mregs;
    pController->tibase = mregs - PUMA5_BASE_OFFSET;
    pController->musb = musb;
#if 0
    //pController->Controller.pPrivateData = pController;
    pController->Controller.start = cppi_controller_start;
    pController->Controller.stop = cppi_controller_stop;
#endif
    pController->Controller.channel_alloc = cppi_channel_allocate;
    pController->Controller.channel_release = cppi_channel_release;
    pController->Controller.channel_program = cppi_channel_program;
    pController->Controller.channel_abort = cppi_channel_abort;
    pResult = &(pController->Controller);

    /* In Linux 3.12 we call start controller after creating instead from musb core */
    ret = cppi_controller_start(pController);
    if (ret)
        goto plat_get_fail;

#ifdef DEBUG_CPPI_TD
    gpThis = musb;
#endif
    return pResult;

plat_get_fail:
    kfree(pController);
kzalloc_fail:
    return NULL;
}

/*
 *  Destroy a previously-instantiated DMA controller.
 */
void dma_controller_destroy(struct dma_controller *pController)
{
    struct cppi *cppi = container_of(pController, struct cppi, Controller);;

    /*In Linux 3.12 we stop controller before destroy instead stopping from musb core*/
    cppi_controller_stop(pController);

    /* free the cppi object */
    if ( cppi )
    {
        kfree(cppi);
    }

}

/*
 * Context: controller irqlocked, endpoint selected
 */
static int cppi_channel_abort(struct dma_channel *pChannel)
{
    struct cppi_channel *otgCh =  container_of(pChannel, struct cppi_channel,
                                               Channel);
    struct cppi     *pController = otgCh->pController;
    int         chNum = otgCh->chNo;
    void            *__iomem mbase;
    void            *__iomem regBase;
    u32         regVal;
    u32         csr;
    usbTxCppiCh *txCppi;
    usbRxCppiCh *rxCppi;

    switch ( pChannel->status )
    {

    case MUSB_DMA_STATUS_BUS_ABORT:
    case MUSB_DMA_STATUS_CORE_ABORT:
        /* from RX or TX fault irq handler */
    case MUSB_DMA_STATUS_BUSY:
        /* the hardware needs shutting down */
        dprintk("channel_release:dma status busy = %x\n",pChannel->status);
        break;
    case MUSB_DMA_STATUS_UNKNOWN:
        DBG(8, "%cX DMA%d not allocated\n",
            otgCh->bTransmit ? 'T' : 'R',
            otgCh->chNo);
        /* FALLTHROUGH */
    case MUSB_DMA_STATUS_FREE:

        break;
    }


    mbase = pController->mregs;
    regBase = mbase - PUMA5_BASE_OFFSET;

    /* REVISIT should rely on caller having done this,
     * and caller should rely on us not changing it.
     * peripheral code is safe ... check host too.
     */
    musb_ep_select(mbase,chNum + 1);

    if ( otgCh->bTransmit )
    {
        u32 indx;

        dprintk("tx dma channel(%d) teardown, otgCh = %p\n",chNum, otgCh);

        disable_irq(USB_CPPI_TXDMA_INTR);
        txCppi = otgCh->txCppi;

        otgCh->txTeardownPending = True;

        /* tear down the tx channel */
        txCppiChannelTearDown( otgCh );

        otgCh->txTeardownPending = False;

        /* FIXME clean up the transfer state ... here?
         * the completion routine should get called with
         * an appropriate status code.
         */
        regVal = musb_readw(otgCh->pEndPt->regs, MUSB_TXCSR);
        regVal |= MUSB_TXCSR_FLUSHFIFO;
        musb_writew(otgCh->pEndPt->regs, MUSB_TXCSR, regVal);
        musb_writew(otgCh->pEndPt->regs, MUSB_TXCSR, regVal);

        for ( indx = 0; indx < txCppi->numTxQs; indx++ )
        {
            PAL_cppi4QueueClose ( pController->cppi4PAL,txCppi->txQueueHnd[indx]);
        }
        /* Mark channel closed */
        otgCh->txIsOpen = False;
        txCppi->chInfo.chState = USB_CPPI4_CH_CLOSED;

        enable_irq(USB_CPPI_TXDMA_INTR);
    }
    else /* RX */
    {

        dprintk("rx dma channel (%d) teardown, otgCh = %p\n",chNum,otgCh);
        disable_irq(USB_CPPI_RXDMA_INTR);
        rxCppi = otgCh->rxCppi;

        otgCh->rxTeardownPending = True;

        /* flush the fifo of endpoint */
        regVal = musb_readw(otgCh->pEndPt->regs, MUSB_RXCSR);
        regVal |= MUSB_RXCSR_FLUSHFIFO | MUSB_RXCSR_P_WZC_BITS;
        musb_writew(otgCh->pEndPt->regs, MUSB_RXCSR, regVal);
        musb_writew(otgCh->pEndPt->regs, MUSB_RXCSR, regVal);

        /* tear down the rx channel */
        rxCppiChannelTearDown( otgCh );

        otgCh->rxTeardownPending = False;

        /* NOTE: docs don't guarantee any of this works ...  we
         * expect that if the usb core stops telling the cppi core
         * to pull more data from it, then it'll be safe to flush
         * current RX DMA state iff any pending fifo transfer is done.
         */

        /* for host, ensure ReqPkt is never set again */
        if ( is_host_active(otgCh->pController->musb) )
        {
            regVal = musb_readl(regBase, PUMA5_AUTOREQ_REG);
            regVal &= ~((0x3) << (otgCh->chNo * 2));
            musb_writel(regBase, PUMA5_AUTOREQ_REG, regVal);
        }

        csr = musb_readw(otgCh->pEndPt->regs, MUSB_RXCSR);

        /* for host, clear (just) ReqPkt at end of current packet(s) */
        if ( is_host_active(otgCh->pController->musb) )
        {
            csr |= MUSB_RXCSR_H_WZC_BITS;
            csr &= ~MUSB_RXCSR_H_REQPKT;
        }
        else
            csr |= MUSB_RXCSR_P_WZC_BITS;

        /* clear dma enable */
        csr &= ~(MUSB_RXCSR_DMAENAB);
        musb_writew(otgCh->pEndPt->regs, MUSB_RXCSR, csr);

        /* flush the fifo of endpoint */
        regVal = musb_readw(otgCh->pEndPt->regs, MUSB_RXCSR);
        regVal |= MUSB_RXCSR_FLUSHFIFO | MUSB_RXCSR_P_WZC_BITS;
        musb_writew(otgCh->pEndPt->regs, MUSB_RXCSR, regVal);
        musb_writew(otgCh->pEndPt->regs, MUSB_RXCSR, regVal);

        PAL_cppi4QueueClose ( pController->cppi4PAL,rxCppi->rxQueueHnd);
        PAL_cppi4QueueClose ( pController->cppi4PAL,rxCppi->fdbQueueHnd[0]);
        /* Mark channel closed */
        otgCh->rxIsOpen = False;

        enable_irq(USB_CPPI_RXDMA_INTR);
    }

    pChannel->status = MUSB_DMA_STATUS_FREE;
    otgCh->startAddr = 0;
    otgCh->currOffset = 0;
    otgCh->transferSize = 0;
    otgCh->pktSize = 0;

    return 0;
}

inline UsbCppi4HostDesc *usbGetTxBD(struct cppi * txCppi)
{
    UsbCppi4HostDesc *currBD;

#ifdef USB_USE_ACC_LIST
    currBD = (UsbCppi4HostDesc *) (*txCppi->listEntryPtr);
    currBD = (UsbCppi4HostDesc *)((Uint32) currBD & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);
#else
    currBD = (UsbCppi4HostDesc *) PAL_cppi4QueuePop (txCppi->txCmplQueueHnd);
#endif

    if ( currBD )
    {
#ifdef USB_USE_ACC_LIST
        txCppi->listEntryPtr = (Uint32 *)((Uint32)txCppi->listEntryPtr
                                          + USB_ACC_ENTRY_SIZE);
#endif
        currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);

#ifdef USB_CPPI4_DEBUG
        printk("processed BD %x\n", (Uint32)currBD);
#endif

        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
    }
    return(currBD);
}

/*
 * txCppiChannelTearDown
 */
static int txCppiChannelTearDown( struct cppi_channel *txCh )
{
    struct cppi *pController = txCh->pController, *cppi;
    usbTxCppiCh *txCppi = txCh->txCppi,*ntxCppi;
    int retVal,ret,bLocalEnd ;
    UsbCppi4HostDesc *currBD,*tempBD,*tcurrBD;
    Uint32 chNum = txCh->chNo,tdreg,numBD=0;
    void                    *__iomem mbase;
    void                    *__iomem regBase;
    struct musb *pThis = pController->musb;
    struct dma_channel *dma;
    unsigned lockKey;
    int nPages,done,timeout = 100, td_rcvd;
#ifdef CONFIG_USB_MUSB_GADGET
    struct musb_ep *pEnd;
#endif
#if defined(CONFIG_USB_MUSB_HOST) && !defined(CONFIG_USB_MUSB_DUAL_ROLE)
    struct musb_hw_ep       *hw_ep;
#endif

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    Cppi4EmbdDesc * tempEmbBD, *currEmbBD;
    int bufValid,i,count = 0;
    Cppi4Queue fdQueue;
    Cppi4BufPool pool;
    Ptr rxfdQEmbHnd;
#endif


    mbase = pController->mregs;
    regBase = mbase - PUMA5_BASE_OFFSET;
    cppi = pController;

#ifdef DEBUG_CPPI_TD
    printk(" before tear down \n");
    printBDList(txCppi->bdPoolHead);
#endif
    /* disable the associated accumulator channel and
       initiate the channel teardown for txCppiDma channel */
    retVal = PAL_cppi4TxChClose(txCppi->cppi4TxChHnd, NULL);
    if ( retVal )
    {
        DBG(1, "\nERROR:dmaAbortChannel:PalTxChClose retVal = %d",
            retVal);
        return retVal;
    }

    retVal  = PAL_cppi4TxChStatus(txCppi->cppi4TxChHnd , NULL);
    if ( retVal )
    {
        dprintk(" %s: PAL_cppi4TxChStatus failed retval = %d\n",__FUNCTION__,retVal);
    }
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    /* There might be packets on the Tx Queue that were not sent,
       they need to be recycled properly */
    while ( (currBD = (UsbCppi4HostDesc *)PAL_cppi4QueuePop(txCppi->txQueueHnd[0])) )
    {

        currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
        /* check if the currBD is embDesc, then move it to corresponding queue
           */
        tempEmbBD = (Cppi4EmbdDesc *)currBD;
        currEmbBD =  (Cppi4EmbdDesc *)(((u32)tempEmbBD & 0x80000000) ? (u32)tempEmbBD : ((u32)tempEmbBD | 0xd0000000));
        PAL_CPPI4_CACHE_INVALIDATE(currEmbBD, CPPI4_BD_LENGTH_FOR_CACHE);

        if ( currEmbBD->descInfo & 0x3FFFFFFF )
        {
            dprintk(" EmbBDDesc (%p) popped from txQueue\n",currEmbBD);
            /* TODO Is number of buffers ok */
            /* find the Buffers attached to the Embedded descripors  and push it back to respective pools */
            for ( i=0;i<4;i++ )
            {
                bufValid = (currEmbBD->Buf[i].BufInfo & CPPI41_EM_BUF_VALID_MASK);
                if ( bufValid )
                {
                    pool.bMgr = (currEmbBD->Buf[i].BufInfo & CPPI41_EM_BUF_MGR_MASK) >> CPPI41_EM_BUF_MGR_SHIFT;
                    pool.bPool =(currEmbBD->Buf[i].BufInfo & CPPI41_EM_BUF_POOL_MASK) >> CPPI41_EM_BUF_POOL_SHIFT;
                    PAL_cppi4BufDecRefCnt(pController->cppi4PAL,pool,(Ptr)currEmbBD->Buf[i].BufPtr);
                }
            }
            /* Push back the embedded descriptor the the Free descriptor queue of USB*/
            fdQueue.qNum  = (currEmbBD->pktInfo & CPPI41_EM_PKTINFO_RETQ_MASK) >> CPPI41_EM_PKTINFO_RETQ_SHIFT;
            fdQueue.qMgr  = (currEmbBD->pktInfo & CPPI41_EM_PKTINFO_RETQMGR_MASK) >> CPPI41_EM_PKTINFO_RETQMGR_SHIFT;
            rxfdQEmbHnd = PAL_cppi4QueueOpen(pController->cppi4PAL, fdQueue);
            if ( NULL ==  rxfdQEmbHnd )
            {
                printk(" %s : fail to open Queue (%d)",__FUNCTION__,fdQueue.qNum);
                continue;
            }
            tempEmbBD = (Cppi4EmbdDesc *)PAL_CPPI4_VIRT_2_PHYS((Uint32)currBD);
            PAL_cppi4QueuePush(rxfdQEmbHnd,(Ptr)tempEmbBD,(USB_CPPI4x_RX_EMB_BD_SIZE - 24)/4,0);
            PAL_cppi4QueueClose(pController->cppi4PAL,rxfdQEmbHnd);
            /* FIXME
               when usb cable is unplugged when session (eth->usb) is on ,eth frames getting accumulated
               at usb-txqueue immediately after recycling, to avoid this infinite loop, after 64 descriptor count
               loop is breaked.
             */
            count++;
            if ( count > 64 )
                break;
            else
                continue;
        }/* End of while loop */
#endif

        chNum = (currBD->tagId & 0xFF) ;
        bLocalEnd = chNum & 0xF;
        chNum = chNum >> 4;
        txCh->actualLen = currBD->buffLen;
        txCh->Channel.actual_len = txCh->actualLen;
        txCh->Channel.status = MUSB_DMA_STATUS_FREE;

        dprintk(" usbtxBD (%p) popped from txQueue for ch(%d)\n",currBD,chNum);

#ifdef CONFIG_USB_MUSB_GADGET
        pEnd = &pThis->endpoints[bLocalEnd].ep_in;
        dma = pEnd->dma;
#else
        hw_ep = &pThis->endpoints[bLocalEnd];
        dma = hw_ep->tx_channel;
#endif
        /* TODO */
        dma->status = MUSB_DMA_STATUS_FREE;


        /* Return TX BD's to the software list - this is protected by critical
           section */
        ntxCppi = pController->txCppiCh[chNum].txCppi;
        tcurrBD = currBD;
        numBD = 1;
        while ( tcurrBD->nextSwBDPtr != NULL )
        {
            tcurrBD = (UsbCppi4HostDesc *)tcurrBD->nextSwBDPtr;
            numBD++;
        }
        tcurrBD->nextSwBDPtr = (Ptr)ntxCppi->bdPoolHead;
        ntxCppi->bdPoolHead = currBD;
        ntxCppi->numBD += numBD;
    }/* End of while loop */

    ret = False;

    do
    {

        /* issue tear down for tx channel -- mentor core register */
        tdreg = musb_readl(regBase, PUMA5_TEARDOWN_REG);
        tdreg |= ( (1 << chNum) << TX_TEARDOWN_SHIFT_CNT );
        musb_writel(regBase, PUMA5_TEARDOWN_REG, tdreg);


#ifdef USB_USE_ACC_LIST
        nPages = avalanche_intd_get_interrupt_count (USB_HOST, cppi->accChNum);
        td_rcvd = 0;
        while ( nPages )
        {
            /* only one tx completion queue is used for all tx channel 1,2,3,4
               hence each buffer descriptor from txcompletionQ has to be interpreted
               for soruce tx channel number by using tagId field of currBD */
            done = 0;
            while ( (currBD = usbGetTxBD(cppi)) )
            {
                /* get the currBD from transmit completion queue
                   get the tx channel number from tagId of currBD
                 */
                if ( (((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD)>> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD) )
                {
                    PAL_cppi4GetTdInfo (txCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
                    ret = True;
                    dprintk(" tx TD-BD (%p) popped from accumulator\n",currBD);
                    //    avalanche_intd_set_interrupt_count (USB_HOST, cppi->accChNum, 1);
                    td_rcvd = 1;
                    continue;
                }


                chNum = (currBD->tagId & 0xFF) ;
                bLocalEnd = chNum & 0xF;
                chNum = chNum >> 4;
                txCh->actualLen = currBD->buffLen;
                txCh->Channel.actual_len = txCh->actualLen;
                txCh->Channel.status = MUSB_DMA_STATUS_FREE;

                dprintk(" txBD (%p) popped from accumulator for ch(%d)\n",currBD, chNum);
    #ifdef CONFIG_USB_MUSB_GADGET
                pEnd = &pThis->endpoints[bLocalEnd].ep_in;
                dma = pEnd->dma;
    #else
                hw_ep = &pThis->endpoints[bLocalEnd];
                dma = hw_ep->tx_channel;
    #endif
                if ( dma )
                {
                    /* TODO tx completion routine call back */
                    dma->status = MUSB_DMA_STATUS_FREE;
                    dma->actual_len = currBD->buffLen;
                }


                /* Return TX BD's to the software list - this is protected by critical
                   section */
                ntxCppi = pController->txCppiCh[chNum].txCppi;
                tcurrBD = currBD;
                numBD = 1;
                while ( tcurrBD->nextSwBDPtr != NULL )
                {
                    tcurrBD = (UsbCppi4HostDesc *)tcurrBD->nextSwBDPtr;
                    numBD++;
                }
                tcurrBD->nextSwBDPtr = (Ptr)ntxCppi->bdPoolHead;
                ntxCppi->bdPoolHead = currBD;
                ntxCppi->numBD += numBD;
                done = 1;
            }

            if ( done || td_rcvd )
                cppi->listEntryPtr = PAL_cppi4AccChGetNextList(cppi->txAccChHnd);

            avalanche_intd_set_interrupt_count (USB_HOST, cppi->accChNum, 1);

            nPages = avalanche_intd_get_interrupt_count (USB_HOST, cppi->accChNum);
            if ( nPages )
                printk("Warning: more acc page interrupts\n");
        }

        doTxEOI( cppi->accVecNum);

#endif /* USB_USE_ACC_CH */

        while ( (tempBD = (UsbCppi4HostDesc *) PAL_cppi4QueuePop(cppi->txCmplQueueHnd)) )
        {

            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);

            /* check for teardown condition here.  This allows packets to
               be processed normally up until the teardown descriptor shows up */

            PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
            if ( ((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD)
                  >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD )
            {
                /* TODO what has to done with TearDown descriptor , whether
                        TD has to queued to default free teardown queue */
                dprintk(" tx TD-BD (%p) popped from txCmplQ\n",currBD);
                PAL_cppi4GetTdInfo (txCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
                ret = True;
                continue;
            }

            /* Return TX BD's to the software list - this is protected by critical
               section */
            chNum = (currBD->tagId & 0xFF) ;
            bLocalEnd = chNum & 0xF;
            chNum = chNum >> 4;
            txCh->actualLen = currBD->buffLen;
            txCh->Channel.actual_len = txCh->actualLen;
            txCh->Channel.status = MUSB_DMA_STATUS_FREE;

            dprintk(" txBD (%p) popped from txCmplQueue for ch(%d)\n",currBD, chNum);
#ifdef CONFIG_USB_MUSB_GADGET
            pEnd = &pThis->endpoints[bLocalEnd].ep_in;
            dma = pEnd->dma;
#else
            hw_ep = &pThis->endpoints[bLocalEnd];
            dma = hw_ep->tx_channel;
#endif
            /* TODO tx completion routine call back */
            if ( dma )
            {
                dma->status = MUSB_DMA_STATUS_FREE;
                dma->actual_len = currBD->buffLen;
            }

            ntxCppi = pController->txCppiCh[chNum].txCppi;
            tcurrBD = currBD;
            numBD = 1;
            while ( tcurrBD->nextSwBDPtr != NULL )
            {
                tcurrBD = (UsbCppi4HostDesc *)tcurrBD->nextSwBDPtr;
                numBD++;
            }
            tcurrBD->nextSwBDPtr = (Ptr)ntxCppi->bdPoolHead;
            ntxCppi->bdPoolHead = currBD;
            ntxCppi->numBD += numBD;
        }
        if ( ret == True )
        {
            dprintk("tear down descriptor received\n");
            break;
        }
    }while ( --timeout );

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
#ifdef DEBUG_CPPI_TD
    dprintk(" after teardown\n");
    printBDList(txCppi->bdPoolHead);
#endif
    if ( ret != True )
    {
        DBG(1,"\n ERROR: TearDown BD not found.");
    }

    /* issue tear down for tx channel -- mentor core register */
    tdreg = musb_readl(regBase, PUMA5_TEARDOWN_REG);
    tdreg |= ( (1 << chNum) << TX_TEARDOWN_SHIFT_CNT );
    musb_writel(regBase, PUMA5_TEARDOWN_REG, tdreg);

#ifdef CONFIG_USB_MUSB_GADGET
    retVal = PAL_cppi4DisableTxChannel(txCppi->cppi4TxChHnd, NULL);
    if ( retVal != USB_SUCCESS )
    {
        DBG(1,
            "\nERROR:AbortDmaCh: Error %08X from PalDisableTxChannel()",
            retVal);
        return(retVal);
    }

    retVal = PAL_cppi4TxChDestroy(txCppi->cppi4TxChHnd, NULL);
    if ( retVal != USB_SUCCESS )
    {
        DBG(1,
            "\nERROR:AbortDmaCh: Error %08X from PalDisableTxChannel()",
            retVal);
        return(retVal);
    }
#endif

    return USB_SUCCESS;

}
/*
 * rxCppiChannelTearDown
 */
static int rxCppiChannelTearDown( struct cppi_channel *rxCh )
{

    struct cppi *pController = rxCh->pController;
    usbRxCppiCh *rxCppi = rxCh->rxCppi, *nrxCppi;
    int retVal,ret;
    UsbCppi4HostDesc *currBD,*tempBD;
    Uint32 chNum = rxCh->chNo,tdreg;
    void                    *__iomem mbase;
    void                    *__iomem regBase;
    int rPages, done, td_rcvd;
    unsigned        lockKey;


    mbase = pController->mregs;
    regBase = mbase - PUMA5_BASE_OFFSET;

    /* issue tear down for rx channel -- otg teardown register */
    tdreg = musb_readl(regBase, PUMA5_TEARDOWN_REG);
    tdreg |= ( (1 << chNum) << RX_TEARDOWN_SHIFT_CNT );
    musb_writel(regBase, PUMA5_TEARDOWN_REG, tdreg);

#ifdef DEBUG_CPPI_TD
    dprintk(" before tear down \n");
    printBDList(rxCppi->bdPoolHead);
#endif

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    dprintk(" %s:drain fdbQueue\n",__FUNCTION__);
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    while ( (currBD = (UsbCppi4HostDesc *)PAL_cppi4QueuePop(rxCppi->fdbQueueHnd[0])) )
    {

        currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);
        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);

        chNum = (currBD->tagId & 0xFF) ;

        chNum = chNum >> 4;
        rxCh->Channel.status = MUSB_DMA_STATUS_FREE;

        dprintk ("Rx:BD(%p) popped from  fdbQueue for chNum(%d)\n",currBD, chNum);

        /* Return RX BD's to the software list - this is protected by critical
           section */
        nrxCppi = pController->rxCppiCh[chNum].rxCppi;
        currBD->nextSwBDPtr = (Ptr)nrxCppi->bdPoolHead;
        nrxCppi->bdPoolHead = currBD;
        nrxCppi->numBD++;
    }
    dprintk(" %s : numBD = %d\n",__FUNCTION__, rxCppi->numBD);
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
#endif

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    #ifdef AVALANCHE_PPD_TEARDOWN

    /* issue rx endpoint dma channel teardown */
    dprintk(" issue rx endpoint dma channel teardown \n");
    retVal = PAL_cppi4RxChClose(rxCppi->cppi4InfraRxChHnd, NULL);
    if ( retVal )
    {
        DBG(1, "\nERROR:dmaAbortChannel:PalRxChClose retVal = %d",
            retVal);
        return retVal;
    }
    retVal = PAL_cppi4RxChStatus(rxCppi->cppi4InfraRxChHnd, NULL);
    if ( retVal )
        dprintk( "PAL_cppi4RxChStatus failed (%s),retval=%d\n",__FUNCTION__,retVal);

    dprintk(" wait till all descriptors from usb2Host are routed\n");
    udelay(1000);

    if ( pController->cppi4InfraTxChOpenFlag )
    {

        if ( pController->cppi4InfraTxChHnd != NULL )
        {
            dprintk(" issue tx infra channel teardown \n");
            retVal  = PAL_cppi4TxChClose(pController->cppi4InfraTxChHnd , NULL);
            if ( retVal )
                printk("\nERROR:dmaAbortChannel:PalTxInfraChClose retVal = %d",retVal);

            retVal  = PAL_cppi4TxChStatus(pController->cppi4InfraTxChHnd , NULL);
            if ( retVal )
                printk(" %s: PAL_cppi4TxChStatus failed retval = %d\n",__FUNCTION__,retVal);

            PAL_cppi4DisableTxChannel(pController->cppi4InfraTxChHnd , NULL);
            retVal = PAL_cppi4TxChDestroy(pController->cppi4InfraTxChHnd, NULL);
            if ( retVal != USB_SUCCESS )
                printk("\nERROR:AbortDmaCh: Error %08X from PalDisableTxChannel()",retVal);
            pController->cppi4InfraTxChHnd = NULL;
        }
    }
    /* issue rx dma channel teardown note that if ppd is defined
       then cppi4RxChHnd is rx infra dma channel */
    dprintk(" issue rx infra dma channel teardown\n");
    #endif
#endif

    /* issue teardown for rx-dma channel */
    retVal = PAL_cppi4RxChClose(rxCppi->cppi4RxChHnd, NULL);
    if ( retVal )
    {
        DBG(1, "\nERROR:dmaAbortChannel:PalRxChClose retVal = %d",
            retVal);
        return retVal;
    }

    dprintk(" drain all free descriptor from host fdbQ \n");
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    while ( (currBD = (UsbCppi4HostDesc *)PAL_cppi4QueuePop(rxCppi->fdbQueueHnd[0])) )
    {


        currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);
        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);

        chNum = (currBD->tagId & 0xFF) ;

        chNum = chNum >> 4;
        rxCh->Channel.status = MUSB_DMA_STATUS_FREE;

        dprintk ("Rx:BD(%p) popped from  fdbQueue chNum(%d)\n",currBD, chNum);
        /* Return RX BD's to the software list - this is protected by critical
           section */
        nrxCppi = pController->rxCppiCh[chNum].rxCppi;
        currBD->nextSwBDPtr = (Ptr)nrxCppi->bdPoolHead;
        nrxCppi->bdPoolHead = currBD;
        nrxCppi->numBD++;
    }

    dprintk(" look for rx teardown descriptors from rxQueue(host)\n");
#ifdef USB_USE_ACC_LIST
    td_rcvd = 0;

    //Added by Hai
    disableRxIntr (rxCppi);
    retVal = PAL_cppi4AccChClose(rxCppi->rxAccChHnd, NULL);

    rPages = avalanche_intd_get_interrupt_count (USB_HOST, rxCppi->accChNum);
    while ( rPages )
    {
        done = 0;
        while ( (currBD = usbGetRxBD(rxCppi)) )
        {

            if ( ((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD) >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD )
            {

                PAL_cppi4GetTdInfo (rxCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
                ret = True;
                td_rcvd = 1;
                mdelay(1);
                dprintk ("Rx:TD BD(%p) popped from acc list\n",currBD);
                continue;
            }

            /* extract the data from recevied pakcet descriptor */
            rxCh->actualLen = currBD->buffLen;
            rxCh->Channel.actual_len = rxCh->actualLen;
            rxCh->actStartAddr = (Uint32)PAL_CPPI4_PHYS_2_VIRT( (char *)currBD->bufPtr);
            rxCh->Channel.status = MUSB_DMA_STATUS_FREE;

            chNum = (currBD->tagId & 0xFF) ;

            chNum = chNum >> 4;
            dprintk ("Rx:BD(%p) popped from accumulator, chNum(%d)\n",currBD, chNum);

            done =1;
            /* Return RX BD's to the software list - this is protected by critical
               section */
            currBD->buffLen = currBD->orgBuffLen = 0;
            currBD->bufPtr  = currBD->orgBufPtr  = 0;

            nrxCppi = pController->rxCppiCh[chNum].rxCppi;
            currBD->nextSwBDPtr = (Ptr)nrxCppi->bdPoolHead;
            nrxCppi->bdPoolHead = currBD;
            nrxCppi->numBD++;
        }/* End of while loop */

        if ( done || td_rcvd )
        {
            if ( !td_rcvd )
            {
                dprintk( "No TD BD recieved!\n" );
            }

            rxCppi->listEntryPtr = PAL_cppi4AccChGetNextList(rxCppi->rxAccChHnd);
            avalanche_intd_set_interrupt_count (USB_HOST, rxCppi->accChNum, 1);
        }
        rPages = avalanche_intd_get_interrupt_count (USB_HOST, rxCppi->accChNum);
    }
    doRxEOI( rxCppi );

    #ifdef CONFIG_USB_MUSB_HOST
    enableRxIntr( rxCppi );
    #endif

    while ( (tempBD = (UsbCppi4HostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd)) )
    {
        if ( tempBD == NULL )
            break;

        currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);

        /* check for teardown condition here.  This allows packets to
           be processed normally up until the teardown descriptor shows up */


        if ( ((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD)
              >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD )
        {
            dprintk ("Rx:TD BD (%p) popped from rxQueue\n",currBD);
            PAL_cppi4GetTdInfo (rxCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
            ret = True;
            mdelay(1);
            continue;
        }

        chNum = (currBD->tagId & 0xFF) ;

        chNum = chNum >> 4;

        dprintk ("Rx:BD (%p) popped from rxQueue, chNum(%d)\n",currBD, chNum);
        /* Return RX BD's to the software list - this is protected by critical
           section */
        nrxCppi = pController->rxCppiCh[chNum].rxCppi;
        currBD->nextSwBDPtr = (Ptr)rxCppi->bdPoolHead;
        rxCppi->bdPoolHead = currBD;
        rxCppi->numBD++;

    }
#else /* ifndef USB_USE_ACC_LIST */

    while ( (tempBD = (UsbCppi4HostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd)) )
    {

        currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);
        chNum = (currBD->tagId & 0xFF) ;

        chNum = chNum >> 4;


        /* check for teardown condition here.  This allows packets to
           be processed normally up until the teardown descriptor shows up */

        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
        if ( ((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD)
              >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD )
        {
            ret = True;
            PAL_cppi4GetTdInfo (rxCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
            continue ; /* break; */
        }

        /* Return RX BD's to the software list - this is protected by critical
           section */
        currBD->nextSwBDPtr = (Ptr)rxCppi->bdPoolHead;
        rxCppi->bdPoolHead = currBD;
        rxCppi->numBD++;

    }
#endif /* USB_USE_ACC_LIST */

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

#ifdef DEBUG_CPPI_TD
    dprintk(" after tear down \n");
    printBDList(rxCppi->bdPoolHead);
#endif

    if ( ret != True )
        printk("ERROR:Teardown buffer descriptor not found.\n");

#ifdef CONFIG_USB_MUSB_GADGET
    retVal = PAL_cppi4DisableRxChannel(rxCppi->cppi4RxChHnd, NULL);
    if ( retVal != USB_SUCCESS )
    {
        DBG(1,
            "\nERROR:DisableChannel: Error %08X from CPPI4 PAL DisableRxChannel()",
            retVal);
        return(retVal);
    }

    retVal = PAL_cppi4RxChDestroy(rxCppi->cppi4RxChHnd, NULL);
    if ( retVal != USB_SUCCESS )
    {
        DBG(1,
            "\nERROR:DisableChannel: Error %08X from CPPI4 PAL DisableRxChannel()",
            retVal);
        return(retVal);
    }
#endif

#if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
    #ifdef AVALANCHE_PPD_TEARDOWN
    if ( pController->cppi4InfraTxChOpenFlag )
    {
        Ptr proxyQueueHnd, rxfdQEmbHnd;
        Cppi4EmbdDesc *currEmbBD,*tempEmbBD;
        int bufValid,i;
        Cppi4BufPool pool;
        Cppi4Queue fdQueue;
        dprintk(" drain all emb desc from usb2host proxy queue\n");
        dprintk(" %s read from proxy queue (%d)\n",__FUNCTION__, pController->UsbHostPrxyQ.qNum);

        /* read the embedded descriptor from usb to host proxy queue if any
           queue back to free descriptor queue of rx endpoint dma channel */
        proxyQueueHnd = PAL_cppi4QueueOpen (pController->cppi4PAL,
                                            pController->UsbHostPrxyQ);

        if ( NULL == proxyQueueHnd )
        {
            printk("Error:teardown, QueueOpen Fail,proxyQueue(%d)",pController->UsbHostPrxyQ.qNum);
            return 1;
        }

        while ( (tempEmbBD = (Cppi4EmbdDesc *)PAL_cppi4QueuePop(proxyQueueHnd)) )
        {

            currEmbBD = PAL_CPPI4_PHYS_2_VIRT(tempEmbBD);
            currEmbBD =  (Cppi4EmbdDesc *)(((u32)currEmbBD & 0x80000000) ? (u32)currEmbBD : ((u32)currEmbBD | 0xd0000000));
            PAL_CPPI4_CACHE_INVALIDATE(currEmbBD, CPPI4_BD_LENGTH_FOR_CACHE);

            if ( currEmbBD->descInfo & 0x3FFFFFFF )
            {

                PAL_CPPI4_CACHE_INVALIDATE(currEmbBD, CPPI4_BD_LENGTH_FOR_CACHE);
                printk(" EmbBDDesc (%p) popped from txProxyQueue\n",currEmbBD);
                /* TODO Is number of buffers ok */
                /* find the Buffers attached to the Embedded descripors  and push it back to respective pools */
                for ( i=0;i<4;i++ )
                {
                    bufValid = (currEmbBD->Buf[i].BufInfo & CPPI41_EM_BUF_VALID_MASK);
                    if ( bufValid )
                    {
                        pool.bMgr = (currEmbBD->Buf[i].BufInfo & CPPI41_EM_BUF_MGR_MASK) >> CPPI41_EM_BUF_MGR_SHIFT;
                        pool.bPool =(currEmbBD->Buf[i].BufInfo & CPPI41_EM_BUF_POOL_MASK) >> CPPI41_EM_BUF_POOL_SHIFT;
                        PAL_cppi4BufDecRefCnt(pController->cppi4PAL,pool,(Ptr)currEmbBD->Buf[i].BufPtr);
                    }
                }

                /* Push back the embedded descriptor the the Free descriptor queue of USB*/
                fdQueue.qNum  = (currEmbBD->pktInfo & CPPI41_EM_PKTINFO_RETQ_MASK) >> CPPI41_EM_PKTINFO_RETQ_SHIFT;
                fdQueue.qMgr  = (currEmbBD->pktInfo & CPPI41_EM_PKTINFO_RETQMGR_MASK) >> CPPI41_EM_PKTINFO_RETQMGR_SHIFT;
                rxfdQEmbHnd  = PAL_cppi4QueueOpen(pController->cppi4PAL, fdQueue);
                if ( NULL ==  rxfdQEmbHnd )
                {
                    printk(" %s : fail to open Queue (%d)",__FUNCTION__,fdQueue.qNum);
                    continue;
                }
                /* Push back the embedded descriptor the the Free descriptor queue of USB*/
                tempEmbBD = (Cppi4EmbdDesc *)PAL_CPPI4_VIRT_2_PHYS((Uint32)tempEmbBD);
                PAL_cppi4QueuePush(rxfdQEmbHnd,(Ptr)tempEmbBD,(USB_CPPI4x_RX_EMB_BD_SIZE - 24)/4,0);
                PAL_cppi4QueueClose(pController->cppi4PAL,rxfdQEmbHnd);
            }/* End of while loop */
        }
        pController->cppi4InfraTxChOpenFlag = 0;
        PAL_cppi4QueueClose(pController->cppi4PAL, proxyQueueHnd);
    }
    #endif
#endif
    return retVal ;
}

/* ***************   TODO     T x      P R O C E S S  ****************/
/* TBD Queries:
 *
 */
UsbCppi4HostDesc *usbGetFreeTxBDList(usbTxCppiCh * txCppi, int numBDs)
{
    UsbCppi4HostDesc *currBD, *tempBD;

    if ( numBDs > txCppi->numBD )
    {
        return NULL;
    }

    txCppi->numBD -= numBDs;
    currBD = txCppi->bdPoolHead;
    tempBD = currBD;

    while ( --numBDs )
    {
        tempBD = (UsbCppi4HostDesc *)tempBD->nextSwBDPtr;
    }

    txCppi->bdPoolHead = (UsbCppi4HostDesc *)tempBD->nextSwBDPtr;
    tempBD->nextSwBDPtr = 0;
    return(currBD);
}
/*
 * usbGetFreeRxBD
 */
UsbCppi4HostDesc *usbGetFreeRxBDList(usbRxCppiCh * rxCppi, int numBDs)
{
    UsbCppi4HostDesc *currBD, *tempBD;

    if ( numBDs > rxCppi->numBD )
    {
        printk("\n no free BD ");
        return NULL;
    }

    rxCppi->numBD -= numBDs;

    currBD = rxCppi->bdPoolHead;
    tempBD = currBD;

    while ( --numBDs )
    {
        tempBD = (UsbCppi4HostDesc *)tempBD->nextSwBDPtr;
    }

    rxCppi->bdPoolHead = (UsbCppi4HostDesc *)tempBD->nextSwBDPtr;
    tempBD->nextSwBDPtr = 0;

    return(currBD);
}
/*
 * usbPutFreeRxBD
 */
void usbPutFreeRxBD(usbRxCppiCh *rxCppi, UsbCppi4HostDesc *freeRxBD)
{
    UsbCppi4HostDesc *currBD;

    currBD = freeRxBD;
    currBD->tagInfo = 0;
    currBD->buffLen = 0;
    currBD->bufPtr = 0;
    currBD->orgBuffLen = 0;
    currBD->orgBufPtr = 0;
    PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
    currBD->nextSwBDPtr = (Ptr)rxCppi->bdPoolHead;
    rxCppi->bdPoolHead = currBD;
    rxCppi->numBD++;
}
/*
 * usbGetRxBD
 * Get the buffer descriptor from queue.
 */
UsbCppi4HostDesc* usbGetRxBD(usbRxCppiCh * rxCppi)
{
    UsbCppi4HostDesc *currBD = NULL;

#ifdef USB_USE_ACC_LIST
    currBD = (UsbCppi4HostDesc *) (*rxCppi->listEntryPtr);
    currBD = (UsbCppi4HostDesc *)((Uint32) currBD & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);
#else
    currBD = (UsbCppi4HostDesc *) PAL_cppi4QueuePop (rxCppi->rxQueueHnd);
#endif

    if ( currBD )
    {
#ifdef USB_USE_ACC_LIST
        rxCppi->listEntryPtr = (Uint32 *)((Uint32)rxCppi->listEntryPtr
                                          + USB_ACC_ENTRY_SIZE);
#endif
        currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);
#ifdef USB_CPPI4_DEBUG
        printk ("Virt addr of currBD = %x\n", (Uint32)currBD);
#endif
        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
    }

    return(currBD);
}
/*
 * usbTxPktProcesss
 * process all the txCompleteQueue for tx endpoint 1 to 4.
 */

static Int usbTxPktProcess(struct cppi *cppi, Ptr pktArgs)
{

#if defined USB_USE_ACC_LIST
    Uint32 intd_status,tx,i;
#endif


#if defined USB_USE_ACC_LIST
    intd_status = *(volatile Uint32 *)(INTDSTATUS);

    #ifdef USB_CPPI4_DEBUG
    DBG (USB_CPPI4_DEBUG_LEVEL,"usbTxPktProcess: INTD status is %#lx\n", intd_status);
    #endif

    if ( !cppi )
    {
        return(USB_SUCCESS);
    }

    intd_status = (intd_status & USB_ACC_TXCH_MASK) >> USB_ACC_TXCH_SHIFT_CNT;
    for ( i = 0, tx = intd_status;  tx; tx = tx >> 1, i++ )
    {
        if ( (tx & 1) == 0 )
            continue ;

        /* process the txBuffer Descriptor from tx completed Queue */
        usbTxBDProc(cppi, i );

        /* give EOI */
        doTxEOI( cppi->accVecNum);

    #ifdef USB_CPPI4_GETSTATS
        cppi->txIntCount[i]++;
    #endif
    }
#endif

    return(USB_SUCCESS);
}

/**
*  usbTxBDProc
*/
static int usbTxBDProc(struct cppi * cppi, int txChannel )
{
    UsbCppi4HostDesc *currBD,*tcurrBD;
    usbTxCppiCh *ntxCppi;
    Uint32 chNum = 0 ;
    Uint32 bLocalEnd;
    struct musb *pThis = cppi->musb;
    struct dma_channel *dma;
    int done = 0,nPages, count=0, tx_cnt;
    struct cppi_channel     *txCh;
    Uint32 numBD=0;
    unsigned lockKey;

#if defined(CONFIG_USB_MUSB_HOST) && !defined(CONFIG_USB_MUSB_DUAL_ROLE)
    struct musb_hw_ep       *hw_ep;
#endif
#ifdef CONFIG_USB_MUSB_GADGET
    struct musb_ep *pEnd;
#endif
    while (avalanche_intd_get_interrupt_count(USB_HOST, cppi->accChNum))
    {
        /* only one tx completion queue is used for all tx channel 1,2,3,4
        hence each buffer descriptor from txcompletionQ has to be interpreted
        for soruce tx channel number by using tagId field of currBD */
        tx_cnt = 0;
        while ( (currBD = usbGetTxBD(cppi)) )
        {
            /* get the currBD from transmit completion queue
            get the tx channel number from tagId of currBD
                */
            if ( ((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD)
                  >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD )
            {
                printk ( KERN_ERR "%s: TD BD (%p) popped from rxQueue\n",__FUNCTION__, currBD);
                PAL_cppi4GetTdInfo (cppi->cppi4PAL, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
                continue;
            }

            PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);

            chNum = (currBD->tagId & 0xFF) ;
            bLocalEnd = chNum & 0xF;
            chNum = chNum >> 4;

            if ( chNum > 3 || bLocalEnd >= MUSB_C_NUM_EPS )
            {
                printk( KERN_ERR "%s: Unknown channel %d or end point %d\n",__FUNCTION__, chNum, bLocalEnd);
                return USB_SUCCESS;
            }

            txCh  = &cppi->txCppiCh[chNum];

            if ( !txCh )
            {
                return USB_SUCCESS;
            }

            txCh->actualLen = currBD->buffLen;
            txCh->Channel.actual_len = txCh->actualLen;
            txCh->Channel.status = MUSB_DMA_STATUS_FREE;
            txCh->Channel.max_len = currBD->dwMaxLength;
            txCh->Channel.desired_mode = currBD->desired_mode;

            /* tx completion routine call back */
#ifdef CONFIG_USB_MUSB_GADGET
            pEnd = &pThis->endpoints[bLocalEnd].ep_in;
            dma = pEnd->dma;
#else
            hw_ep = &pThis->endpoints[bLocalEnd];
            dma = hw_ep->tx_channel;
#endif
            if ( dma )
            {
                dma->status = MUSB_DMA_STATUS_FREE;
                dma->actual_len = currBD->buffLen;
            }
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

#ifdef CONFIG_USB_MUSB_GADGET
    #if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
            /************************************************/
            /* Restore SYNC Q PTID info from skb for egress */
            {
                struct usb_request  *pRequest;
                struct sk_buff      *rxskb;

                pRequest = next_request(pEnd);

                if ( pRequest )
                {
                    rxskb = (struct sk_buff *)pRequest->context;

                    if ( rxskb && rxskb->pp_packet_info.flags == TI_HIL_PACKET_FLAG_PP_SESSION_INGRESS_RECORDED )
                    {
                        memcpy((void *)&(currBD->netInfoWord0), rxskb->pp_packet_info.ti_epi_header, sizeof(rxskb->pp_packet_info.ti_epi_header));
                        PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
                    }
                }
            }
            /************************************************/
    #endif
#endif

            musb_dma_completion(cppi->musb, bLocalEnd, 1);
            /* Return TX BD's to the software list - this is protected by critical
            section */
            PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
            ntxCppi = cppi->txCppiCh[chNum].txCppi;
            spin_lock(&ntxCppi->bdlock);
            tcurrBD = currBD;
            numBD = 1;
            while ( tcurrBD->nextSwBDPtr != NULL )
            {
                tcurrBD = (UsbCppi4HostDesc *)tcurrBD->nextSwBDPtr;
                numBD++;
            }
            tcurrBD->nextSwBDPtr = (Ptr)ntxCppi->bdPoolHead;
            ntxCppi->bdPoolHead = currBD;
            ntxCppi->numBD += numBD;
            spin_unlock(&ntxCppi->bdlock);
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

            tx_cnt++;
            if ( tx_cnt > 32 )
            {
                dprintk("Warning: tx_cnt=%d\n",tx_cnt);
            }
        }

#ifdef USB_USE_ACC_LIST
        /* switch to next page and release current */
        cppi->listEntryPtr = PAL_cppi4AccChGetNextList(cppi->txAccChHnd);
        avalanche_intd_set_interrupt_count (USB_HOST, cppi->accChNum, 1);
#endif
    };
    return USB_SUCCESS;
}


#ifdef USB_CPPI_TX_TASKLET_MODE
void usb_cppi_tx_tasklet(unsigned long data)
{
    struct musb *musb = (struct musb *)data;
    unsigned long flags;
    #if defined USB_USE_ACC_LIST
    Uint32 intd_status = 0;
    #endif
    struct cppi *cppi;


    #if defined USB_USE_ACC_LIST
    spin_lock_irqsave(&musb->lock, flags);
    cppi = container_of( musb->dma_controller, struct cppi, Controller);

    if ( !cppi )
    {
        tx_musb_tasklet_scheduled = 0;
        enable_irq(USB_CPPI_TXDMA_INTR);
        spin_unlock_irqrestore(&musb->lock, flags);
        return;
    }

    usbTxPktProcess(cppi, NULL );
    intd_status = *(volatile Uint32 *)(INTDSTATUS);

        #ifdef USB_CPPI4_DEBUG
    DBG (USB_CPPI4_DEBUG_LEVEL,"usb_cppi_tx_tasklet: INTD status is %#lx\n", intd_status);
        #endif

    intd_status = (intd_status & USB_ACC_TXCH_MASK) >> USB_ACC_TXCH_SHIFT_CNT;
    if ( intd_status )
    {
        tasklet_schedule(&tx_tasklet);
    }
    else
    {
        tx_musb_tasklet_scheduled = 0;
        enable_irq(USB_CPPI_TXDMA_INTR);
    }
    spin_unlock_irqrestore(&musb->lock, flags);
    #endif

}
#endif


irqreturn_t usbCppiTxPktProcess_Isr(int irq, void *pThis)
{
    irqreturn_t retval = IRQ_NONE;
    unsigned long flags;
    struct musb *musb= pThis;
#ifndef USB_CPPI_TX_TASKLET_MODE
    struct musb *musb = pThis;
    struct  cppi *cppi;
    struct dma_controller *dmaCtrl;
#endif

    /* FIXME */
#ifdef USB_CPPI_TX_TASKLET_MODE
    spin_lock_irqsave(&musb->lock, flags);
    if ( tx_musb_tasklet_scheduled )
    {
        retval = IRQ_HANDLED;
    }
    else
    {
        disable_irq_nosync(USB_CPPI_TXDMA_INTR);
        /* trigger tasklet action */
        if ( !tx_taskInit )
        {
            tasklet_init(&tx_tasklet, usb_cppi_tx_tasklet, (unsigned long)pThis);
            tx_taskInit = 1;
        }
        tasklet_schedule(&tx_tasklet);
        tx_musb_tasklet_scheduled = 1;

        retval = IRQ_HANDLED;
    }/* end of else conditional block */
    spin_unlock_irqrestore(&musb->lock, flags);
#else

//  usb_cppi_tx_tasklet((u32)pThis);
    /* interrupt mode context */
    spin_lock_irqsave(&musb->lock, flags);
    dmaCtrl = musb->dma_controller;
    cppi = container_of(dmaCtrl, struct cppi, Controller);
    usbTxPktProcess(cppi, NULL );
    spin_unlock_irqrestore(&musb->lock, flags);
#endif

    /* after the processing all the list buffer pages for all
    the tx channels[1..4] associate with interupt , it must then perform
    an EOI by writing the accumulator interrupt vector(0-15) to
    the INTD eoi register */

    retval = IRQ_HANDLED;
    return(retval);

}

/*
 ************* R X    P A C K E T    P R O C E S S      ***************
*/
static Int usbRxPktProcess(struct cppi *cppi, Ptr pktArgs)
{
    Uint32 i=0;

#if defined USB_USE_ACC_LIST
    Uint32 intd_status,rx;
#endif

#if defined USB_USE_ACC_LIST
    intd_status = *(volatile Uint32 *)(INTDSTATUS);
    #ifdef USB_CPPI4_DEBUG
    DBG (USB_CPPI4_DEBUG_LEVEL,"usbRxPktProcess: INTD status is %#lx\n", intd_status);
    #endif
    intd_status = (intd_status & USB_ACC_RXCH_MASK) >> USB_ACC_RXCH_SHIFT_CNT;
    for ( i = 0, rx = intd_status;  rx; rx = rx >> 1, i++ )
    {
        if ( (rx & 1) == 0 )
            continue ;
        /* process the txBuffer Descriptor from tx completed Queue */
        usbRxBDProc(cppi,i);

    #ifdef USB_CPPI4_GETSTATS
        cppi->rxIntCount[i]++;
    #endif
    }
#endif

    return(USB_SUCCESS);
}

/**
 *  usbRxBDProc
 */
static int usbRxBDProc(struct cppi * cppi, int rxChannel )
{
    UsbCppi4HostDesc *currBD;
    usbRxCppiCh *rxCppi,*nrxCppi;
    struct cppi_channel *rxCppiCh;
    Uint32 chNum,bLocalEnd;
    int done = 0,rPages,count = 0;
#if defined(CONFIG_USB_MUSB_HOST) && !defined(CONFIG_USB_MUSB_DUAL_ROLE)
    struct musb_hw_ep       *hw_ep;
#endif
#ifdef CONFIG_USB_MUSB_GADGET
    struct musb_ep *pEnd;
#endif
    struct dma_channel *dma;
    struct musb *pThis = cppi->musb;
    unsigned lockKey;

    rxCppiCh = &cppi->rxCppiCh[rxChannel];

    if ( !rxCppiCh->rxIsOpen )
        return USB_ERR_RXCH_INVALID;

    rxCppi = rxCppiCh->rxCppi;

    do
    {
        done = 0;

        while ( (currBD = usbGetRxBD(rxCppi)) )
        {
            if ( ((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD) >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD )
            {
                PAL_cppi4GetTdInfo (rxCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
                printk ( KERN_ERR "%s: TD BD (%p) popped from rxQueue\n",__FUNCTION__, currBD);
                return 0;
            }
            PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);

            /* extract the data from recevied pakcet descriptor */
            rxCppiCh->actualLen = currBD->buffLen;
            rxCppiCh->Channel.actual_len = rxCppiCh->actualLen;
            rxCppiCh->actStartAddr = (Uint32)PAL_CPPI4_PHYS_2_VIRT( (char *)currBD->bufPtr);
            rxCppiCh->Channel.status = MUSB_DMA_STATUS_FREE;

            rxCppiCh->Channel.max_len = currBD->dwMaxLength;
            rxCppiCh->Channel.actual_len = currBD->buffLen;
            rxCppiCh->Channel.status = MUSB_DMA_STATUS_FREE;
            rxCppiCh->Channel.desired_mode = currBD->desired_mode;

            chNum = (currBD->tagId & 0xFF) ;
            bLocalEnd = chNum & 0xF;
            chNum = chNum >> 4;

            if ( chNum > 3 || bLocalEnd >= MUSB_C_NUM_EPS )
            {
                printk( KERN_ERR "%s: Unknown channel %d or end point %d\n",__FUNCTION__, chNum, bLocalEnd);
                return USB_SUCCESS;
            }
            /* TODO check whether this code is required*/
#ifdef CONFIG_USB_MUSB_GADGET
            pEnd = &pThis->endpoints[bLocalEnd].ep_in;
            dma = pEnd->dma;
#else
            hw_ep = &pThis->endpoints[bLocalEnd];
            dma = hw_ep->rx_channel;
#endif
            if ( dma )
            {
                dma->status = MUSB_DMA_STATUS_FREE;
                dma->actual_len = currBD->buffLen;
            }
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

#ifdef CONFIG_USB_MUSB_GADGET
    #if defined(CONFIG_ARM_AVALANCHE_PPD) && defined(CONFIG_USB_PPD_SUPPORT)
            /************************************************/
            /* Keep SYNC Q PTID info in skb for egress      */
            {
                struct usb_request  *pRequest;
                struct sk_buff      *rxskb;
                pEnd = &pThis->endpoints[bLocalEnd].ep_out;
                pRequest = next_request(pEnd);

                if ( pRequest )
                {
                    rxskb = (struct sk_buff *)pRequest->context;

                    if ( rxskb && currBD->netInfoWord1 )
                    {
                        memcpy(rxskb->pp_packet_info.ti_epi_header, (void *)&(currBD->netInfoWord0), sizeof(rxskb->pp_packet_info.ti_epi_header));
                        rxskb->pp_packet_info.flags = TI_HIL_PACKET_FLAG_PP_SESSION_INGRESS_RECORDED;
                    }
                }
            }
            /************************************************/
    #endif
#endif

            /* rx completion routine call back */
            musb_dma_completion(cppi->musb, bLocalEnd, 0);
            done =1;
            /* Return RX BD's to the software list - this is protected by critical
               section */

            currBD->buffLen = currBD->orgBuffLen = 0;
            currBD->bufPtr  = currBD->orgBufPtr  = 0;

            PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
            nrxCppi = cppi->rxCppiCh[chNum].rxCppi;
            spin_lock(&nrxCppi->bdlock);
            currBD->nextSwBDPtr = (Ptr)nrxCppi->bdPoolHead;
            nrxCppi->bdPoolHead = currBD;
            nrxCppi->numBD++;
            spin_unlock(&nrxCppi->bdlock);
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

        }/* End of while loop */

#ifdef USB_USE_ACC_LIST
        if ( done )
        {
            rxCppi->listEntryPtr = PAL_cppi4AccChGetNextList(rxCppi->rxAccChHnd);
            avalanche_intd_set_interrupt_count (USB_HOST, rxCppi->accChNum, 1);
        }
        else
        {
            printk(KERN_ERR "Rx-dma intr but no bd found\n");
        }

#endif
        count++;
        rPages = avalanche_intd_get_interrupt_count (USB_HOST, rxCppi->accChNum);
        if ( count > 10 )
        {
            dprintk( "rx-Warning [%d,%d]",done,rPages);
        }
    }while ( rPages );

    return USB_SUCCESS;
}


#ifdef USB_CPPI_RX_TASKLET_MODE
void usb_cppi_rx_tasklet(unsigned long data)
{
    struct musb *musb = (struct musb *)data;
    struct cppi *cppi;
    int rxChannel = 0;
    usbRxCppiCh *rxCppi;
    unsigned long flags;

    #if defined USB_USE_ACC_LIST
    Uint32 intd_status = 0;
    #endif
    #if defined USB_USE_ACC_LIST
    spin_lock_irqsave(&musb->lock, flags);
    cppi = container_of(musb->dma_controller, struct cppi, Controller);
    rxCppi = cppi->rxCppiCh[rxChannel].rxCppi;
    usbRxPktProcess(cppi, NULL );
    doRxEOI( rxCppi );

    intd_status = *(volatile Uint32 *)(INTDSTATUS);

        #ifdef USB_CPPI4_DEBUG
    DBG (USB_CPPI4_DEBUG_LEVEL,"usb_cppi_rx_tasklet: INTD status is %#lx\n", intd_status);
        #endif
    intd_status = (intd_status & USB_ACC_RXCH_MASK) >> USB_ACC_RXCH_SHIFT_CNT;
    if ( intd_status )
    {
        tasklet_schedule(&rx_tasklet);
    }
    else
    {
        rx_musb_tasklet_scheduled = 0;
        enable_irq(USB_CPPI_RXDMA_INTR);
    }
    spin_unlock_irqrestore(&musb->lock, flags);

    /* after the processing all the list buffer pages for all
    the tx channels[1..4] associate with interupt , it must then perform
    an EOI by writing the accumulator interrupt vector(0-15) to
    the INTD eoi register */
/*        *(Uint32 *)INTD_EOI_REG = USB_EP_TX_ACC_VEC_NUM ;
        TODO eoi already given in TxIsr function, is this is right? */
    #endif

}
#endif

/*
 * usbCppiRxPktProcess_Isr
 */
irqreturn_t usbCppiRxPktProcess_Isr(int irq, void *pThis)
{
    irqreturn_t retval = IRQ_NONE;
    unsigned long flags;
    struct musb *musb= pThis;
#ifndef USB_CPPI_RX_TASKLET_MODE
    struct musb *musb = pThis;
    int rxChannel = 0;
    struct  cppi *cppi;
    struct dma_controller *dmaCtrl;
    usbRxCppiCh *rxCppi;
#endif
	
    static int first = 1;
    struct task_struct *ctask = current;
    struct sched_param param = {.sched_priority = 0};
    if (first)
    {
        first = 0;
        sched_setscheduler(ctask, SCHED_NORMAL, &param);
        printk("**** ---- >>>> %s - name \"%s\", pid %d\n", __func__, ctask->comm, ctask->pid);
    }

#ifdef USB_CPPI_RX_TASKLET_MODE
    spin_lock_irqsave(&musb->lock, flags);
    if ( rx_musb_tasklet_scheduled )
    {
        retval = IRQ_HANDLED;
    }
    else
    {
        disable_irq_nosync(USB_CPPI_RXDMA_INTR);
        /* trigger tasklet action */
        if ( !rx_taskInit )
        {
            tasklet_init(&rx_tasklet, usb_cppi_rx_tasklet, (unsigned long)pThis);
            rx_taskInit = 1;
        }
        tasklet_schedule(&rx_tasklet);
        rx_musb_tasklet_scheduled = 1;
        retval = IRQ_HANDLED;
    }/* end of else conditional block */
    spin_unlock_irqrestore(&musb->lock, flags);
#else
    spin_lock_irqsave(&musb->lock, flags);
    dmaCtrl = musb->dma_controller;
    cppi = container_of(dmaCtrl, struct cppi, Controller);
    rxCppi = cppi->rxCppiCh[rxChannel].rxCppi;
    usbRxPktProcess(cppi, NULL );
    doRxEOI( rxCppi );
    spin_unlock_irqrestore(&musb->lock, flags);
#endif
    retval = IRQ_HANDLED;
    return(retval);

}

#ifdef DEBUG_CPPI_TD
void Debug_monitor(int debug_queue, int dispBD );
void Debug_monitor(int debug_queue, int dispBD )
{
    Cppi4PALObj                     *cppi4PAL;
    Cppi4Queue queue;
    PAL_Handle txQueueHnd;
    int i,flag;
    //UsbCppi4HostDesc *currBD;
    //Cppi4EmbdDesc *currBD;
    PAL_Cppi4QueueObj* qObj ;
    CSL_Queue_Mgmt_Regs* regs;

    Cppi4EmbdDesc *currEmbBD, *tempEmbBD;

    printk(" %s \n",__FUNCTION__);
    printk("  IO_ADDRESS(0x031100040)= %08x\n", IO_ADDRESS(0x03110040));
    currEmbBD  = (Cppi4EmbdDesc *)0x03110040;

    #if 1
    printk(" currEmbBD = %p\n",currEmbBD);
    tempEmbBD = PAL_CPPI4_PHYS_2_VIRT(currEmbBD);
    printk(" PAL_CPPI4_PHYS_2_VIRT(currEmbBD) = %p\n",tempEmbBD);
    flag = (u32)tempEmbBD & 0x80000000;
    currEmbBD =  (Cppi4EmbdDesc *)( ((u32)tempEmbBD & 0x80000000) ? (u32)tempEmbBD : ((u32)tempEmbBD | 0xd0000000));

    PAL_CPPI4_CACHE_INVALIDATE(currEmbBD, CPPI4_BD_LENGTH_FOR_CACHE);
    printk(" (currEmbBD) = %p\n",currEmbBD);

    printk("embBD->descInfo = %08x\n",currEmbBD->descInfo);
    printk("embBD->tagInfo = %08x\n",currEmbBD->tagInfo);
    printk("embBD->pktInfo = %08x\n",currEmbBD->pktInfo);
    for ( i=0; i<4; ++i )
    {
        printk("embBD->Buf[%d].BufInfo = %08x\n",i,currEmbBD->Buf[i].BufInfo);
        printk("embBD->Buf[%d].BufPtr  = %08x\n",i,currEmbBD->Buf[i].BufPtr);
    }
    printk("embBD->EPI[0] = %08x\n",currEmbBD->EPI[0]);
    printk("embBD->EPI[1] = %08x\n",currEmbBD->EPI[1]);

    #endif
    printk("\n Monitoring queue : %d ",debug_queue);
    queue.qMgr = 0;
    queue.qNum = debug_queue;
    cppi4PAL = PAL_cppi4Init(NULL, NULL);
    txQueueHnd = PAL_cppi4QueueOpen(cppi4PAL,queue);
    qObj = (PAL_Cppi4QueueObj*)txQueueHnd;
    regs = qObj->baseAddress;
    printk(" Queuecnt = %d \n",regs->Queue_Reg_A);


}
void  cppi_print_debug(int param )
{
    struct musb *musb = gpThis;
    int bEnd = param;
    //struct musb_ep          *pEnd = &musb->aLocalEnd[bEnd * 2 -1].ep_in;
    void __iomem            *epio = musb->endpoints[bEnd].regs;
    u8 __iomem              *pBase = musb->mregs;
    u16 wCsrVal;
    //u32 timeout = 0;

    switch ( param )
    {
    case 0 :
        {
            Uint32 intd_status ;
            intd_status = *(volatile Uint32 *)(INTDSTATUS);
            intd_status = (intd_status & USB_ACC_TXCH_MASK) >> USB_ACC_TXCH_SHIFT_CNT;
            printk("usb_cppi_tx_tasklet: INTD status is %#lx\n", intd_status);
            //tasklet_schedule(&tx_tasklet);
        }
        break;
    default :
        break;
    }
}
EXPORT_SYMBOLS(Debug_monitor);
#endif
