/*
 *
 * ddc_cpgmac_f_TxRx.c
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


/** \file   ddc_cpgmac_f_TxRx.c
    \brief  DDC CPGMAC_F Send/Receive functionality Source file

    This file contains the send/receive core functionality for CPGMAC_F.

    @author     Greg Guyotte
 */

/* Must define this to get the right header contents */
#define CPMAC_DDC


#include <asm-arm/arch-avalanche/generic/pal.h> /* PAL SYS/OS services required */
#include "ddc_netdev.h"         /* DDC Network Device Interface header */
#include "ddc_cpgmac_f_Drv.h"   /* CPMAC Driver DDC Internal Data Structures */

#ifdef CONFIG_PSP_TRACE
#include <linux/psp_trace.h>
#endif

#define cpmac_min_val(a,b) ((a > b) ? b : a)


CpmacHostDesc *cpmacGetFreeTxBDList(CpmacTxCppiCh * txCppi, int numBDs);
inline CpmacHostDesc *cpmacGetRxBD(CpmacRxCppiCh * rxCppi);
inline CpmacHostDesc *cpmacGetTxBD(CpmacTxCppiCh * txCppi);

/**
 *  cpmacTick
 *    - CPMAC DDC Periodic Timer (Tick) Function
 *    - calls PHY polling function
 *    - If status changed, invokes DDA callback to propogate PHY / Devicestatus
 *
 *  \note "tickArgs" is not used in this implementation
 */
PAL_Result cpmacTick(CpmacDDCObj * hDDC, Ptr tickArgs)
{
    /* Verify proper device state */
    if (hDDC->ddcObj.state != DDC_OPENED) {
        return (CPMAC_ERR_DEV_NOT_OPEN);
    }

    /* If NOPHY specified do not check */
    if (!(hDDC->initCfg.phyMode & SNWAY_NOPHY)) {
        /* Opened and Phy available */
        int tickChange;

        tickChange = cpswHalCommonMiiMdioTic(hDDC->PhyDev);
        if (tickChange == 1) {  /*  MDIO indicated a change  */
            cpmacUpdatePhyStatus(hDDC);
            hDDC->ddaIf->ddaNetIf.ddaFuncTable.ddaControlCb(hDDC->ddcObj.
                                                            hDDA,
                                                            CPMAC_DDA_IOCTL_STATUS_UPDATE,
                                                            (Ptr) & hDDC->
                                                            status, NULL);
        } else if ((hDDC->initCfg.phyMode & SNWAY_AUTOMDIX)
                   && (tickChange & _MIIMDIO_MDIXFLIP)) {
            avalanche_set_mdix_on_chip(hDDC->initCfg.baseAddress, tickChange & 0x1);
        }
    }

    return (CPMAC_SUCCESS);
}

Int cpmacTxPktProcess(CpmacDDCObj * hDDC, int *pktsPending, Ptr pktArgs)
{
    Bool isEOQ = True;
    Uint32 handlePktsAndStatus = 0;
    Int32 pktsProcessed = 0;
#if defined CPMAC_POLL_MODE && defined CPGMAC_USE_ACC_LIST
    Uint32 intc_status, intd_status;
#endif

    handlePktsAndStatus = hDDC->txCppi->chInfo.serviceMax;

    if (pktArgs)
        handlePktsAndStatus = cpmac_min_val(((RxTxParams *) pktArgs)->txPkts, handlePktsAndStatus);

#if defined CPMAC_POLL_MODE && defined CPGMAC_USE_ACC_LIST
    intd_status = *(volatile Uint32 *)(INTDSTATUS);
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TX, 
            "cpmacTxPktProcess: INTD status is %#lx\n", intd_status);
    if (intd_status & (1<<CPGMAC_ACCTX_INTD_NUM))
#endif    
    pktsProcessed = cpmacTxBDProc(hDDC, &handlePktsAndStatus, &isEOQ);

    if (pktArgs)
        ((RxTxParams *) pktArgs)->retTxPkts = pktsProcessed;

    *pktsPending = handlePktsAndStatus;

#ifdef CPMAC_DDC_GETSTATS
    hDDC->txIntCount[txQueue]++;
#endif

    return (CPMAC_SUCCESS);
}

/**
 *  cpmacRxPktProcess
 *    - receive ISR, registered with interrupt controller
 *    - Invokes cpmacRxBDProc, checks to see if EOQ was reached
 *    - if EOQ not reached, don't EOI, retrigger the interrupt manually
 */
Int cpmacRxPktProcess(CpmacDDCObj * hDDC, Int * pktsPending, Ptr pktArgs)
{
    Bool isEOQ;
    Uint32 handlePktsAndStatus = 0;
    Int32 pktsProcessed = 0;
#if defined CPMAC_POLL_MODE && defined CPGMAC_USE_ACC_LIST
    Uint32 intc_status, intd_status;
#endif

    handlePktsAndStatus = hDDC->rxCppi->chInfo.serviceMax;

    if (pktArgs)
        handlePktsAndStatus = cpmac_min_val(((RxTxParams *) pktArgs)->rxPkts, handlePktsAndStatus);

#if defined CPMAC_POLL_MODE && defined CPGMAC_USE_ACC_LIST
    intd_status = *(volatile Uint32 *)(INTDSTATUS);
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, 
            "cpmacRxPktProcess: INTD status is %#lx\n", intd_status);
    if (intd_status & (1<<CPGMAC_ACCRX_INTD_NUM))
#endif    
    pktsProcessed = cpmacRxBDProc(hDDC, &handlePktsAndStatus, &isEOQ);


    if (pktArgs)
        ((RxTxParams *) pktArgs)->retRxPkts = pktsProcessed;


    *pktsPending = handlePktsAndStatus;

#ifdef CPMAC_DDC_GETSTATS
    hDDC->rxIntCount[rxQueue]++;
#endif

    return (CPMAC_SUCCESS);
}

void cpmacAddBDToRxQueue(CpmacDDCObj * hDDC, CpmacRxCppiCh * rxCppi,
                    CpmacHostDesc * currBD, Char * buffer,
                    DDC_NetDataToken bufToken)
{

    /* Update the hardware descriptor */
    if (buffer != 0) {
        /* replace buffer and token currently attached to this BD */
        currBD->hwDesc.orgBufPtr = PAL_CPPI4_VIRT_2_PHYS(buffer);
        currBD->hwDesc.orgBuffLen  = rxCppi->chInfo.bufSize;
        currBD->dataPtr = (Ptr) buffer;
        currBD->bufToken = bufToken;
    }
    PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);

    {
        Ptr temp = (Ptr) currBD;
        PAL_cppi4QueuePush(rxCppi->fdbQueueHnd [0], (Uint32 *) PAL_CPPI4_VIRT_2_PHYS(temp),
                        CPPI41_QM_HDESC_SIZE_VAL, 0);
    }
}

inline CpmacHostDesc* cpmacGetRxBD(CpmacRxCppiCh * rxCppi)
{
    CpmacHostDesc *currBD;

#ifdef CPGMAC_USE_ACC_LIST
    assert (rxCppi, (rxCppi->listEntryPtr != NULL));

    currBD = (CpmacHostDesc *) (*rxCppi->listEntryPtr);
    currBD = (CpmacHostDesc *)((Uint32) currBD & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);
#else
    currBD = (CpmacHostDesc *) PAL_cppi4QueuePop (rxCppi->rxQueueHnd);
    
#endif

    if (currBD) {
#ifdef CPGMAC_USE_ACC_LIST        
        rxCppi->listEntryPtr = (Uint32 *)((Uint32)rxCppi->listEntryPtr 
                                        + CPGMAC_ACC_ENTRY_SIZE); 
#endif
        currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);
        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
    }
    
    return (currBD);
}

/**
 *  cpmacRxBDProc
 *    - CPMAC DDC RX Buffer Descriptor processing
 *    - processes received packets and passes them to DDA layer
 *    - requeues the buffer descriptor to the receive pool
 *    - channel and queue validations should not be performed for speed
 *
 *  \note return number of pkts processed and returns 1 in handlePktsAndStatus
 *        if pkt processing not completed.
 */
Int
cpmacRxBDProc(CpmacDDCObj * hDDC, Uint32 * handlePktsAndStatus,
              Bool * isEOQ)
{
    CpmacRxCppiCh *rxCppi;
    CpmacHostDesc *sopBD;
    Uint32 pktsProcessed = *handlePktsAndStatus;
    DDC_NetPktObj *currPkt;
    Uint32 recycle_pkt;
    Uint32 subChannelNumber;
    Uint32 pktsToBeProcessed = *handlePktsAndStatus;
    char *newBuffer;
    DDC_NetDataToken newBufToken;
    u32 pkt_type;
    DDC_NetBufObj *rxBufObj;

    *handlePktsAndStatus = 0;
    *isEOQ = True;

#ifdef CONFIG_PSP_TRACE
    psp_trace(ETH_DRV_RX_ENTER);
#endif

    rxCppi = hDDC->rxCppi;

    currPkt = &rxCppi->pktQueue[0];

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: pagebase = %x\n", 
            __FUNCTION__, __LINE__, rxCppi->listBuffBase[rxCppi->activeListRgn]); 
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: list page no = %d\n", 
            __FUNCTION__, __LINE__, rxCppi->activeListRgn); 
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: listEntry = %x\n", 
            __FUNCTION__, __LINE__, rxCppi->listEntryPtr); 

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s - %d: pktsToBeProcessed = %d\n", 
            __FUNCTION__, __LINE__, pktsToBeProcessed);    

    while (pktsToBeProcessed && (sopBD = cpmacGetRxBD(rxCppi))) {
#ifdef CONFIG_PSP_TRACE
        psp_trace_par(ETH_DRV_PKT_RX_ENTER, sopBD->bufToken);
#endif

	/* validate the rx descriptor */
        pkt_type = sopBD->hwDesc.pktInfo >> PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT;
        if( pkt_type != PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH ){
		printk("WARN: %s : unknown desriptor (%p),pkt_type=%d\n",__FUNCTION__,sopBD,pkt_type);
		continue;	
	}

        recycle_pkt = 0;

#ifdef CPGMAC_PP_DEBUG
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: TAG => %#x\n", 
                __FUNCTION__, __LINE__, sopBD->hwDesc.tagInfo);
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: EPI0 => %#x\n", 
                __FUNCTION__, __LINE__, sopBD->hwDesc.netInfoWord0);
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: EPI1 => %#x\n", 
                __FUNCTION__, __LINE__, sopBD->hwDesc.netInfoWord1);
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: Org Buff : Buff = %x : %x\n", 
                __FUNCTION__, __LINE__, sopBD->hwDesc.orgBufPtr, sopBD->hwDesc.bufPtr);
#endif
        
        /* get subChannelNumber and pass it up */
        subChannelNumber = (sopBD->hwDesc.tagInfo /*!@@*/ & 0x000f0000) >> 16;

        /* Get a local reference to the DDC buffer structure before walking list */
        rxBufObj = &currPkt->bufList[0];
        rxBufObj->dataPtr = PAL_CPPI4_PHYS_2_VIRT ((Char *) sopBD->hwDesc.bufPtr);
        rxBufObj->length = sopBD->hwDesc.buffLen;
#ifndef CONFIG_TI_PACKET_PROCESSOR
        rxBufObj->bufToken = sopBD->bufToken;
#else
        rxBufObj->bufToken = (void *)&(sopBD->hwDesc.netInfoWord0);
#endif
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: newBuffer = %x\n", 
                __FUNCTION__, __LINE__, rxBufObj->dataPtr);
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: BD : SKB = %x : %x\n", 
                __FUNCTION__, __LINE__, sopBD, rxBufObj->bufToken);

#ifndef CONFIG_TI_PACKET_PROCESSOR
        currPkt->pktToken = currPkt->bufList->bufToken;
#else
	currPkt->pktToken = sopBD->bufToken;
#endif
        currPkt->numBufs = 1;
        currPkt->pktLength = (sopBD->hwDesc.descInfo & CPPI4_BD_PKT_LENGTH_MASK);
        currPkt->pktLength -= 4;    /* !@@ ??? */

        pktsToBeProcessed--;
        
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s - %d: pktsToBeProcessed = %d\n", 
                __FUNCTION__, __LINE__, pktsToBeProcessed);    
        
        newBuffer =
            hDDC->ddaIf->ddaNetIf.ddaNetAllocRxBufCb(hDDC->ddcObj.hDDA,
                                                     rxCppi->chInfo.
                                                     bufSize, &newBufToken,
                                                     NULL);
        if (newBuffer == NULL) {
            CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: recycle BD\n", 
                    __FUNCTION__, __LINE__);
            recycle_pkt = 1;
            goto Recycle_CpmacRxBDProc;

#ifdef CPMAC_DDC_GETSTATS
            ++rxCppi->outOfRxBuffers;
#endif
        }
        /* Send packet to DDA.  Assuming that the NetPktObj that is sent can be
         * reused upon return of this call. */
        hDDC->ddaIf->ddaNetIf.ddaNetrxCb(hDDC->ddcObj.hDDA,
                                         &rxCppi->pktQueue[0],
                                         (Ptr) subChannelNumber,
                                         (Ptr) sopBD);

        sopBD->hwDesc.orgBufPtr = PAL_CPPI4_VIRT_2_PHYS(newBuffer);
        sopBD->dataPtr = (Ptr) newBuffer;
        sopBD->bufToken = newBufToken;
        /* !@1 sopBD->orgBuffLen = rxCppi->chInfo.bufSize; */

        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: newBuffer = %x\n", 
                            __FUNCTION__, __LINE__, newBuffer);
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: BD : new SKB = %x : %x\n", 
                            __FUNCTION__, __LINE__, sopBD, newBufToken);
      
Recycle_CpmacRxBDProc:
        PAL_CPPI4_CACHE_WRITEBACK(sopBD, CPPI4_BD_LENGTH_FOR_CACHE);
        {
            Ptr temp = (Ptr) sopBD;                
            PAL_cppi4QueuePush(rxCppi->fdbQueueHnd [0], (Uint32 *) PAL_CPPI4_VIRT_2_PHYS(temp),
                            CPPI41_QM_HDESC_SIZE_VAL, 0);
        }
        if (recycle_pkt)
            break;

    }   /* End of while receive packet processing loop */

#ifdef CPGMAC_USE_ACC_LIST
    if (*rxCppi->listEntryPtr) {
        *isEOQ = False;
        *handlePktsAndStatus = 1;
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s %d: list not emptied\n", 
                            __FUNCTION__, __LINE__);
    }
#else
    if (pktsToBeProcessed == 0) {
        /* XXX - There is an extra scheduling involved here in case
         * pktsToBeProcessed=0 and PAL_cppi4QueuePop would also be NULL. 
         * We may avoid this using savedBD and an extra POP.
         */
        *isEOQ = False;
        *handlePktsAndStatus = 1;
    }
#endif

    pktsProcessed -= pktsToBeProcessed;

#ifdef CPMAC_DDC_GETSTATS
    if (pktsProcessed == 0)
        hDDC->rxEmptyIntCount[queue]++;
#endif

#ifdef CONFIG_PSP_TRACE
    psp_trace_par(ETH_DRV_RX_EXIT, pktsProcessed);
#endif

    return (pktsProcessed);
}

CpmacHostDesc *cpmacGetFreeTxBDList(CpmacTxCppiCh * txCppi, int numBDs)
{
    CpmacHostDesc *currBD, *tempBD;

    if (numBDs > txCppi->numBD)
        return NULL;

    txCppi->numBD -= numBDs;

    currBD = txCppi->bdPoolHead;
    tempBD = currBD;

    while (--numBDs) {
        tempBD = tempBD->nextSwBDPtr;
    }

    txCppi->bdPoolHead = tempBD->nextSwBDPtr;
    tempBD->nextSwBDPtr = 0;
    currBD->txEopBD = (Ptr) tempBD;

    return (currBD);
}

/**
 *  DDC_cpmacSend
 *    - queues a packet onto a hardware queue
 *    - supports multifragment packets
 *    - no channel validation performed in the interest of speed
 *    - sendArgs Bit Mapping as follows:
 *
 *        (Note: Bits 15-0 map directly to CPPI4 BD Word 2 bits 31-16.)
 *        16    Priority Queue.  0 or 1.
 *        15-12 Packet Type
 *        11    Descriptor Type (0 = Host, 1 = Embedded Mode)
 *        10-8  Additional Buffer Count (0-7).  Set to 0 for Host Mode.
 *        7     Reserved
 *        6-4   Protocol Specific Region Offset.  Set to 0 for Host Mode.
 *        3     Pass CRC Bit.  When 0, the MAC calculates and inserts CRC.  When
 *              set, the MAC will expect a CRC to be in the data buffer with the
 *              packet data.
 *        2     No VLAN Bit.  Remove VLAN tag if set.
 *        1-0   Reserved.
 */
PAL_Result
DDC_cpmacSend(CpmacDDCObj * hDDC, DDC_NetPktObj * pkt, Int channel,
              Ptr sendArgs)
{
    PAL_Result retVal = CPMAC_SUCCESS;
    CpmacHostDesc *currBD, *sopBD;
    CpmacTxCppiCh *txCppi;
    DDC_NetBufObj *bufList;
    Uint32 lockKey;
    Uint32 outputQueue;

    /* Check ethernet link state. If not linked, return error */
    if (!hDDC->status.PhyLinked)
        return (CPMAC_ERR_TX_NO_LINK);

    bufList = pkt->bufList;     /* Get handle to the buffer array */

    /* Check packet size and if < CPMAC_MIN_ETHERNET_PKT_SIZE, pad it up */
    if (pkt->pktLength < CPMAC_MIN_ETHERNET_PKT_SIZE) {
        bufList[pkt->numBufs - 1].length += (CPMAC_MIN_ETHERNET_PKT_SIZE - pkt->pktLength);
        pkt->pktLength = CPMAC_MIN_ETHERNET_PKT_SIZE;
    }

    /*
     * \note: The critical section is required to protect the "txCppi" data
     * structure which is being used by this function and by cpmacTxBDProc().
     * Since the function requires to protect the resources in many places in the
     * function below, it would be wise to get and release the crit section only
     * once rather than doing it in many places below. Interrupt protection is
     * sought here so that this function or the cpmacTxBDProc() function can be
     * invoked in any context and still will be safe to do so.
     */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);

    txCppi = hDDC->txCppi;
    currBD = cpmacGetFreeTxBDList(txCppi, pkt->numBufs);

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

    if (currBD == NULL) {
#ifdef CPMAC_DDC_GETSTATS
        txCppi->outOfTxBD++;
#endif
        retVal = CPMAC_ERR_TX_OUT_OF_BD;
        return (retVal);

    }

    sopBD = currBD;

    /* set the channel number in the SOP tag only */
    currBD->hwDesc.tagInfo = channel << 16;
    currBD->hwDesc.descInfo = (PAL_CPPI4_HOSTDESC_DESC_TYPE_HOST << PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT) 
                        | pkt->pktLength;
    
    currBD->numFrags = pkt->numBufs;
    currBD->pktToken = pkt->pktToken;

    /* Is QoS active ?*/
    if (((Uint32)(sendArgs) >> 24) & 0x1)
    {
        outputQueue = CPMAC_CPPI4x_QoS_HIGH_TX_QNUM + (((Uint32)(sendArgs) >> 16) & 0x3);
    }
    else
    {
        outputQueue = CPMAC_CPPI4x_TX_QNUM((( (Uint32)(sendArgs) >> 16) & 0x1));
    }

#ifdef CONFIG_ARM_AVALANCHE_PPD
#ifdef CONFIG_TI_PACKET_PROCESSOR
    if(bufList->bufToken)
    {
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, "%s PTID FOUND\n\n", __FUNCTION__);
        PAL_osMemCopy((Ptr)(&currBD->hwDesc.netInfoWord0),(Ptr)bufList->bufToken,EPI_HEADER_LEN);
        currBD->hwDesc.netInfoWord1 &= ~(0xFFFF);
        currBD->hwDesc.netInfoWord1 |= outputQueue;

        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, 
                            "cpmacSend word0 = %x and word1 = %x\n",
                            currBD->hwDesc.netInfoWord0, 
                            currBD->hwDesc.netInfoWord1);
    }
    else
#endif /* CONFIG_TI_PACKET_PROCESSOR */
    {
        currBD->hwDesc.netInfoWord1 = outputQueue ;
    }
#endif /* CONFIG_ARM_AVALANCHE_PPD */
    /* Multiple Tx BD for the packet to be sent */
    while (currBD) {
        /* Populate the BD contents to be added to the TX list */
        currBD->hwDesc.bufPtr =
            PAL_CPPI4_VIRT_2_PHYS((Int32 *) bufList->dataPtr);
        currBD->hwDesc.buffLen = bufList->length;
        currBD->bufToken = bufList->bufToken;
        
        currBD->hwDesc.nextBDPtr =
            (Ptr) PAL_CPPI4_VIRT_2_PHYS(currBD->nextSwBDPtr);

        PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);

        /*  prevBD = currBD; */
        currBD = currBD->nextSwBDPtr;

        bufList++;
    }

    /* !@@ TODO: Add tx priority support. Currently only the first queue is used
     * for tx. The priority in sendArgs could be used to determine txQueue to
     * use for Push
     */  
   
    {
        Ptr temp = (Ptr) sopBD;            
        PAL_cppi4QueuePush(txCppi->txQueueHnd [0], (Uint32 *) PAL_CPPI4_VIRT_2_PHYS(temp),
                    CPPI41_QM_HDESC_SIZE_VAL, pkt->pktLength);
    }

    return (retVal);
}

inline CpmacHostDesc *cpmacGetTxBD(CpmacTxCppiCh * txCppi)
{
    CpmacHostDesc *currBD;

#ifdef CPGMAC_USE_ACC_LIST
    assert (txCppi, (txCppi->listEntryPtr != NULL));

    currBD = (CpmacHostDesc *) (*txCppi->listEntryPtr);
    currBD = (CpmacHostDesc *)((Uint32) currBD & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);
#else
    currBD = (CpmacHostDesc *) PAL_cppi4QueuePop (txCppi->txCmplQueueHnd);
#endif
    
    if (currBD) {
#ifdef CPGMAC_USE_ACC_LIST        
        txCppi->listEntryPtr = (Uint32 *)((Uint32)txCppi->listEntryPtr 
                                        + CPGMAC_ACC_ENTRY_SIZE); 
#endif
        currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TX, "processed BD %x\n", currBD); 
        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
    }
    
    return (currBD);
}

/**
 *  cpmacTxBDProc
 *    - CPMAC DDC TX Buffer Descriptor processing
 *    - processes transmit completed packets and returns the handles to DDA layer
 *    - 'queue' (0-7) indicates the completion queue to process
 *    - channel and queue validations should not be performed for speed
 *
 *  \note returns number of pkts processed and returns false in isEOQ if pkt
 *        completion processing pending.
 */
int
cpmacTxBDProc(CpmacDDCObj * hDDC, Uint32 * handlePktsAndStatus,
              Bool * isEOQ)
{
    CpmacHostDesc *currBD, *eopBD;
    CpmacTxCppiCh *txCppi;
    Uint32 pktsProcessed = *handlePktsAndStatus, lockKey;
    Uint32 pktsToProcess = *handlePktsAndStatus;
    *handlePktsAndStatus = 0;   /* Status. */
    *isEOQ = True;

#ifdef CONFIG_PSP_TRACE
    psp_trace(ETH_DRV_TX_COMPLETE_ENTER);
#endif

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);

    txCppi = hDDC->txCppi;

#ifdef CPGMAC_DEBUG
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TX, "%s %d: pagebase = %x\n", 
                        __FUNCTION__, __LINE__, 
                        txCppi->listBuffBase[txCppi->activeListRgn]); 
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TX, "%s %d: list page no = %d\n", 
                        __FUNCTION__, __LINE__, txCppi->activeListRgn); 
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TX, "%s %d: listEntry = %x\n", 
                        __FUNCTION__, __LINE__, txCppi->listEntryPtr); 
#endif

    while (pktsToProcess && (currBD = cpmacGetTxBD(txCppi))) {

#ifdef CONFIG_PSP_TRACE
        psp_trace_par(ETH_DRV_PKT_TX_COMPLETE_ENTER, currBD->bufToken);
#endif

        /* The EOP BD pointer was saved in the descriptor by Send */
        eopBD = currBD->txEopBD;
        txCppi->numBD += currBD->numFrags;


        hDDC->ddaIf->ddaNetIf.ddaNettxCompleteCb(hDDC->ddcObj.hDDA,
                                                 (DDC_NetDataToken) &
                                                 currBD->pktToken, 1, 0);

        /* Return TX BD's to the software list - this is protected by critical
           section */
        eopBD->nextSwBDPtr = (Ptr) txCppi->bdPoolHead;
        txCppi->bdPoolHead = currBD;
        pktsToProcess--;

    }                   /* End of while loop */

#ifdef CPGMAC_USE_ACC_LIST
    if (*txCppi->listEntryPtr) {
        *isEOQ = False;
        *handlePktsAndStatus = 1;
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TX, "%s %d: list not emptied\n", 
                            __FUNCTION__, __LINE__);
    }
#else
    if (pktsToProcess == 0) {
        /* XXX - There is an extra scheduling involved here in case
         * pktsToProcess=0 and PAL_cppi4QueuePop would also be NULL. We may
         * avoid this using savedBD and an extra POP.
         */
        *isEOQ = False;
        *handlePktsAndStatus = 1;
    }
#endif

    pktsProcessed -= pktsToProcess;

#ifdef CPMAC_DDC_GETSTATS
    if (pktsProcessed == 0)
        hDDC->txEmptyIntCount[queue]++;
#endif

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

#ifdef CONFIG_PSP_TRACE
    psp_trace_par(ETH_DRV_TX_COMPLETE_EXIT, pktsProcessed);;
#endif

    return (pktsProcessed);
}


/**
 * CPMAC DDC Signal Packet processing end to hardware
 *  - Moves the active list reference to the list page available on next
 *  interrupt
 *  - Writes INTD count register to indicate processing done of current page
 *  - programs the EOI vector register so that if there are pending
 *  packets in hardware queue an interrupt can be generated by the hardware
 *
 *  Note - This implementation supports "1 page per interrupt", so count update
 *  and EOI are done in one go. Implementation not following this rule may need
 *  to seggregate INTD count updation and EOI.
 */
Int cpmacPktProcessEnd(CpmacDDCObj * hDDC, Ptr procArgs)
{
    CpmacRxCppiCh *rxCppi;
    rxCppi = hDDC->rxCppi;

#ifdef CPGMAC_USE_ACC_LIST
#if defined CPMAC_POLL_MODE
    if (procArgs && (((RxTxParams *) procArgs)->retRxPkts == 0)) {
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_RX, 
                "cpmacPktProcessEnd: Not moving Rx page, 0 pkts processed\n");
        return (CPMAC_SUCCESS);
    }
#endif
    rxCppi->listEntryPtr = PAL_cppi4AccChGetNextList(rxCppi->rxAccChHnd);
    
    avalanche_intd_set_interrupt_count (CPGMAC_INTD_HOST_NUM, rxCppi->accChNum, 1);
#endif /* CPGMAC_USE_ACC_LIST */

    doRxEOI (rxCppi);
    return (CPMAC_SUCCESS);
}

/**
 * CPMAC DDC Signal Packet processing end to hardware
 *  - Moves the active list reference to the list page available on next
 *  interrupt
 *  - Writes INTD count register to indicate processing done of current page
 *  - programs the EOI vector register so that if there are pending
 *  packets in hardware queue an interrupt can be generated by the hardware
 *
 *  Note - This implementation supports "1 page per interrupt", so count update
 *  and EOI are done in one go. Implementation not following this rule may need
 *  to seggregate INTD count updation and EOI.
 */
Int cpmacTxPktProcessEnd(CpmacDDCObj * hDDC, Ptr procArgs)
{
    CpmacTxCppiCh *txCppi;
    txCppi = hDDC->txCppi;

#ifdef CPGMAC_USE_ACC_LIST
#if defined CPMAC_POLL_MODE
    if (procArgs && (((RxTxParams *) procArgs)->retTxPkts == 0)) {
        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TX, 
                "cpmacTxPktProcessEnd: Not moving Tx page, 0 pkts processed\n");
        return (CPMAC_SUCCESS);
    }
#endif
    txCppi->listEntryPtr = PAL_cppi4AccChGetNextList(txCppi->txAccChHnd);
    
    avalanche_intd_set_interrupt_count (CPGMAC_INTD_HOST_NUM, txCppi->accChNum, 1);
#endif /* CPGMAC_USE_ACC_LIST */

    doTxEOI (txCppi);
    return (CPMAC_SUCCESS);
}
