/*
 * puma5_pp.c
 *  Description:
 *  See below.
 *
 *  
 * Copyright (C) 2007 Texas Instruments, Inc.  All rights reserved.
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
 
/* Puma-5 Packet Processor initialization.
 * Contains Puma-5 specific initialization. The numbers (addresses etc) are
 * Puma-5 specific. The static structures are filled in with Puma-5 specific
 * data and the generic PPD init function gets called in the end
 * with this data.
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/
#include <asm-arm/arch-avalanche/generic/pal.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41pvt.h>
#include <linux/netdevice.h>
#include <linux/proc_fs.h>
#include <linux/unistd.h>

#include <asm-arm/arch-avalanche/generic/ti_ppd.h>
#include <asm-arm/arch-avalanche/puma5/puma5_pp.h>
#include <asm-arm/arch-avalanche/puma5/puma5_cppi.h>

#define MAX_PREFETCHER_INIT_CB 2 /* increase when needed */
typedef void (*avavalnche_prefetcher_cb_func)(int fail);
static avavalnche_prefetcher_cb_func prefetcher_init_cb[MAX_PREFETCHER_INIT_CB];
static int prefetcher_init_cb_num = 0;
static int prefetcher_init_status = 0xdeadf00d;

/* On-Chip descriptor management
 * IMPORTANT NOTES:
 * ===============
 *  -   Following macros control how the on-chip descriptor memory is devided
 *  into different descriptors. All the base addresses must be varefully
 *  determined not to avoid overlap.
 *  -   Also note that all the On-cip descriptors *must* be of size equal to the
 *  specified in puma5_cppi.c cppi config structure for respective region.
 */
#define PREFETCH_DESC_BASEADDR      AVALANCHE_NWSS_ONCHIPDESC_BASE 
#define REPLICATOR_DESC_BASEADDR    ( PREFETCH_DESC_BASEADDR + (PPFW_PREFETCH_DESC_COUNT*PREFETCH_FD_SIZE) )

PrefchCfg prefCfg_g = 
{
    .pfDescCnt      = PPFW_PREFETCH_DESC_COUNT,
    .pfDescBase     = (Ptr) IO_VIRT2PHY( PREFETCH_DESC_BASEADDR ),
    .pfFQ.qMgr      = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .pfFQ.qNum      = PPFW_CPPI4x_APDSP_FD_QNUM,

    .repDescCnt     = PPFW_REPLICA_DESC_COUNT,
    .repDescBase    = (Ptr) IO_VIRT2PHY( REPLICATOR_DESC_BASEADDR ),
    .repFQ.qMgr     = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .repFQ.qNum     = PPFW_CPPI4x_MPDSP_FD_QNUM,

    .pfBlkCnt       = PPFW_PREFETCH_BUFF_COUNT,
    .pfBlkBase      = (Ptr) IO_VIRT2PHY( AVALANCHE_NWSS_APDSP_PREFBLK_BASE ),
    .pfFBQ.qMgr     = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .pfFBQ.qNum     = PPFW_CPPI4x_APDSP_PREBUF_QNUM,

    .pfDataSize     = PREFETCH_DATA_SIZE,
    .pfDataOffset   = PREFETCH_DATA_OFFSET,
    .pfCmdBase      = (Ptr) AVALANCHE_NWSS_APDSP_CMD_BASE
};

typedef struct 
{
    Uint32  numDesc;
    Uint32  qNum;

    Ptr     firstDescPtr;
    Uint32  isAllocated;

    /* Desc config for the  group */
    Uint32 pktType;
} BDBlkInfo;


/* Embedded descriptor table:
 *  - This table aggregates the embedded descriptors in the system. It contains
 *  information about the first descriptor and allocation status of each
 *  division of the region. Generally this division is based on per driver or
 *  per Q basis.
 */
typedef struct 
{
    Ptr         buffDescRegionPtr;
    Uint32      qMgr;
    Uint32      numDesc;
    Uint32      szDesc;
    Uint32      numBlks;
    BDBlkInfo   BDBlk [PAL_CPPI41_FD_Q_LAST - PAL_CPPI41_FD_Q_BASE];
} EmbBDCfg;

EmbBDCfg ppEmbBDCfg_g = 
{
    .qMgr       = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .numDesc    = 2496 + PPFW_EVENT_DESC_NUM, /* Must be same as in Cppi Cgf structure */
    .szDesc     = 64,   /* Must be same as for specific region */
    .numBlks    = 7,    /* Change for each block added/removed */

    /* Host2PP Infra 0 */
    .BDBlk[0].numDesc   = PPFW_RX_EMBEDDED_BD_NUM_LOW,
    .BDBlk[0].qNum      = PPFW_CPPI4x_FD_QNUM(0), //PPFW_CPPI4x_FD_QMGR
    .BDBlk[0].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* Host2PP Infra 1 */
    .BDBlk[1].numDesc   = PPFW_RX_EMBEDDED_BD_NUM_MED,
    .BDBlk[1].qNum      = PPFW_CPPI4x_FD_QNUM(1), //PPFW_CPPI4x_FD_QMGR
    .BDBlk[1].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* Ethernet */
    .BDBlk[2].numDesc   = CPMAC_RX_EMBEDDED_BD_NUM,
    .BDBlk[2].qNum      = CPMAC_CPPI4x_FD_QNUM(0), //CPMAC_CPPI4x_FD_QMGR
    .BDBlk[2].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

     /* USB */
    .BDBlk[3].numDesc   = 64,
    .BDBlk[3].qNum      = USB_CPPI4x_EP0_FD_QNUM, // 145/146/147/148
    .BDBlk[3].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_USB,

    /* DOCSIS */
    .BDBlk[4].numDesc   = CPPI4x_SR_DOCSIS_RX_FD_EMB_DESC_NUM_LOW,
    .BDBlk[4].qNum      = CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM( PAL_CPPI4x_PRTY_LOW ),
    .BDBlk[4].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    .BDBlk[5].numDesc   = CPPI4x_SR_DOCSIS_RX_FD_EMB_DESC_NUM_HIGH,
    .BDBlk[5].qNum      = CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM( PAL_CPPI4x_PRTY_HIGH ),
    .BDBlk[5].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* PP2Host Events */
    .BDBlk[6].numDesc   = PPFW_EVENT_DESC_NUM,
    .BDBlk[6].qNum      = PP_HOST_EVENT_FDQ,      //PP_HOST_EVENT_FDQMGR
    .BDBlk[6].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,
};

 
/**************************************************************************/
/*! \fn static void setupRecycleInfra (PAL_Handle hnd)
 **************************************************************************
 *  \brief Setup infrastructure DMA for recycling use resources.
 *  \return none.
 **************************************************************************/
static void setupRecycleInfra (PAL_Handle hnd)
{
    Cppi4TxChInitCfg txCh;
    Cppi4RxChInitCfg rxCh;
    PAL_Cppi4TxChHnd cppi4TxChHnd;
    PAL_Cppi4RxChHnd cppi4RxChHnd;
    Cppi4Queue tmpQ;

    tmpQ.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    tmpQ.qNum = RECYCLE_CPPI4x_FBD_QNUM;
    PAL_cppi4QueueOpen (hnd, tmpQ);

    /* Set up Rx channel */
    rxCh.chNum              = PAL_CPPI41_DMA1_RX_RECYCLING_INFRA_CHN_NUM; 
    rxCh.dmaNum             = PAL_CPPI41_DMA_BLOCK1;
    rxCh.sopOffset          = 0;
    rxCh.retryOnStarvation  = 0;
    rxCh.rxCompQueue.qMgr   = 0;
    rxCh.rxCompQueue.qNum   = 0;

    rxCh.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    rxCh.u.embeddedPktCfg.fdQueue.qNum = RECYCLE_CPPI4x_FBD_QNUM;
    rxCh.u.embeddedPktCfg.numBufSlot = (EMSLOTCNT-1);
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = 0;
    rxCh.u.embeddedPktCfg.fBufPool[1].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[1].bPool = 0;
    rxCh.u.embeddedPktCfg.fBufPool[2].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[2].bPool = 0;
    rxCh.u.embeddedPktCfg.fBufPool[3].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[3].bPool = 0;

    cppi4RxChHnd        = PAL_cppi4RxChOpen(hnd, &rxCh, NULL);
    
    /* Set up Tx channel */
    txCh.chNum          = PAL_CPPI41_DMA1_TX_RECYCLING_INFRA_CHN_NUM;
    txCh.dmaNum         = PAL_CPPI41_DMA_BLOCK1;
    txCh.tdQueue.qMgr   = DMA1_CPPI4x_FTD_QMGR;
    txCh.tdQueue.qNum   = DMA1_CPPI4x_FTD_QNUM;

    cppi4TxChHnd        = PAL_cppi4TxChOpen(hnd, &txCh, NULL);

    if (!cppi4TxChHnd || !cppi4RxChHnd)
    {
        printk ("%s: infra channel setup failed for channel %d\n",
                __FUNCTION__,PAL_CPPI41_DMA1_TX_RECYCLING_INFRA_CHN_NUM );   
        return;
    }
    
    /* Enable Tx-Rx channels */
    PAL_cppi4EnableRxChannel (cppi4RxChHnd, NULL);
    PAL_cppi4EnableTxChannel (cppi4TxChHnd, NULL);

    return;
}


/* Embedded descriptor table:
 *  - This table aggregates the embedded descriptors in the system. It contains
 *  information about the first descriptor and allocation status of each
 *  division of the region. Generally this division is based on per driver or
 *  per Q basis.
 */
typedef struct 
{
    Ptr         buffDescRegionPtr;
    Uint32      qMgr;
    Uint32      numDesc;
    Uint32      szDesc;
    Uint32      numBlks;
    BDBlkInfo   BDBlk [PAL_CPPI41_FBD_Q_LAST - PAL_CPPI41_FBD_Q_BASE];
} HostBDCfg;


HostBDCfg ppHostBDCfg_g = 
{
    .qMgr       = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .numDesc    = PPFW_HOST_BD_NUM,    /* Must be same as in Cppi Cgf structure */
    .szDesc     = PPFW_HOST_BD_SIZE,   /* Must be same as for specific region   */
    .numBlks    = 3,                   /* Change for each block added/removed   */
    
    /* Host2PP Low */
    .BDBlk[0].numDesc   = PPFW_TX_HOST_BD_NUM_LOW,
    .BDBlk[0].qNum      = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_Q_NUM(0),
    .BDBlk[0].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* Host2PP Medium */
    .BDBlk[1].numDesc   = PPFW_TX_HOST_BD_NUM_MED,
    .BDBlk[1].qNum      = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_Q_NUM(1),
    .BDBlk[1].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,
        
    /* Host2PP Medium */
    .BDBlk[2].numDesc   = PPFW_TX_HOST_BD_NUM_HIGH,
    .BDBlk[2].qNum      = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_Q_NUM(2),
    .BDBlk[2].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,
};

  
void setup_host2pp_infra (PAL_Handle hnd, int infra_chan)
{
    Cppi4TxChInitCfg txCh;
    Cppi4RxChInitCfg rxCh;
    PAL_Cppi4TxChHnd cppi4TxChHnd;
    PAL_Cppi4RxChHnd cppi4RxChHnd;
    
    /* Set up Rx channel */
    rxCh.chNum              = CPPI4x_PPFW_RX_INFRA_CHNUM(infra_chan);
    rxCh.dmaNum             = PAL_CPPI41_DMA_BLOCK1;
    rxCh.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.sopOffset          = 0;
    rxCh.rxCompQueue.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    rxCh.rxCompQueue.qNum   = PPFW_CPPI4x_TX_EGRESS_EMB_QNUM(infra_chan);
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    rxCh.u.embeddedPktCfg.fdQueue.qNum = PPFW_CPPI4x_FD_QNUM(infra_chan);
    rxCh.u.embeddedPktCfg.numBufSlot = (EMSLOTCNT-1);
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = BMGR0_POOL06;
    rxCh.u.embeddedPktCfg.fBufPool[1].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[1].bPool = BMGR0_POOL07;
    rxCh.u.embeddedPktCfg.fBufPool[2].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[2].bPool = BMGR0_POOL07;
    rxCh.u.embeddedPktCfg.fBufPool[3].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[3].bPool = BMGR0_POOL07;
    cppi4RxChHnd        = PAL_cppi4RxChOpen(hnd, &rxCh, NULL);
    
    /* Set up Tx channel */
    txCh.chNum          = CPPI4x_PPFW_TX_INFRA_CHNUM(infra_chan);
    txCh.dmaNum         = PAL_CPPI41_DMA_BLOCK1;
    txCh.tdQueue.qMgr   = DMA1_CPPI4x_FTD_QMGR;
    txCh.tdQueue.qNum   = DMA1_CPPI4x_FTD_QNUM;

    cppi4TxChHnd        = PAL_cppi4TxChOpen(hnd, &txCh, NULL);

    if (!cppi4TxChHnd || !cppi4RxChHnd)
    {
        printk ("%s: infra channel setup failed for channel %d\n",
                __FUNCTION__, infra_chan);   
        return;
    }
    
    /* Enable Tx-Rx channels */
    PAL_cppi4EnableRxChannel (cppi4RxChHnd, NULL);
    PAL_cppi4EnableTxChannel (cppi4TxChHnd, NULL);

    return;
}

/**************************************************************************
 * FUNCTION NAME : pp_test_usage
 **************************************************************************
 * DESCRIPTION   :
 *  Prints the pp_test proc usage
 *
 * RETURNS       :
 *  Nonne
 ***************************************************************************/
void pp_test_usage(void)
{
    printk("\nCommand Usage:\n");
    printk("send pktString[hex byte string with : as delimiter] pktSize[64-1518,pktSize>=length(pktString)] pktNum srcPort[0-%d] dstQueue[0-%d]\n", PAL_CPPI41_SOURCE_PORT_DOCSIS, PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT);
}

static ssize_t pp_test(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
    char    proc_cmd[2000];
    char*   argv[10];
    int     argc = 0;
    char*   ptr_cmd;
    char*   delimitters = " \n\t";
    char*   ptr_next_tok;

    Uint32 pktSize;
    Uint32 pktNum, pktNumIndex;
    Uint32 srcPort;
    Uint32 dstQueue;
    Uint32 pktStringLength;
    Uint32 tmp;
    PAL_Handle hnd = PAL_cppi4Init(NULL, NULL);
    Cppi4Queue queue;
    PAL_Cppi4QueueHnd popQueueHnd, dstQueueHnd;
    Cppi4EmbdDesc* desc = NULL;
    Ptr dataBuffer;
    char *c;
    const char * sep = ":";


    Uint8 pktData[1518] = // Set with default packet. If pktSize > length(pktString) then it will be padded by this packet content
    {
        0x00, 0x1B, 0x21, 0x3C, 0xD9, 0x5E, // ETH DA
        0x00, 0x50, 0xF1, 0x80, 0x00, 0x00, // ETH SA
        0x08, 0x00,                         // ETH Type
        0x45, 0x00,                         // IP Version/Header Length, IP TOS
        0x00, 0x2E,                         // IP Total Length - updated per packet
        0x00, 0x00, 0x00, 0x00,             // IP Identification, IP Fragment
        0x40,                               // IP TTL
        0x11,                               // IP Protocol - UDP
        0x9A, 0x2F,                         // IP Checksum
        0x0A, 0x64, 0x66, 0x64,             // IP SA - 10.100.102.100
        0x0A, 0x64, 0x65, 0x64,             // IP DA - 10.100.101.100
        0x03, 0xE8, 0x03, 0xE9,             // UDP SRC Port (1000), UDP DST Port (1001)
        0x00, 0x1A,                         // UDP Length
        0x00, 0x00,                         // UDP Checksum
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // Payload  
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05  // Payload  
    };
    
    /* Validate the length of data passed. */
    if (count > 2000)
    {
        printk("\nERROR: Command exceeded maximum length\n");
        return -EFAULT;
    }

    /* Initialize the buffer before using it. */
    memset ((void *)&proc_cmd[0], 0, sizeof(proc_cmd));
    memset ((void *)&argv[0], 0, sizeof(argv));

    /* Copy from user space. */
    if (copy_from_user(&proc_cmd, buffer, count))
    {
        pp_test_usage();
        return -EFAULT;
    }

    ptr_next_tok = &proc_cmd[0];

    /* Tokenize the command. Check if there was a NULL entry. If so be the case the user did not know how to use the entry. Print the help screen */
    ptr_cmd = strsep(&ptr_next_tok, delimitters);
    if (ptr_cmd == NULL)
    {
        pp_test_usage();
        return -EFAULT;
    }

    /* Parse all the commands typed. */
    do
    {
        /* Extract the first command. */
        argv[argc++] = ptr_cmd;

        /* Validate if the user entered more commands.*/
        if (argc >= 10)
        {
            pp_test_usage();
            return -EFAULT;
        }

        /* Get the next valid command. */
        ptr_cmd = strsep(&ptr_next_tok, delimitters);
    } while (ptr_cmd != NULL);

    /* We have an extra argument when strsep is used instead of strtok */
    argc--;

    /******************************* Command Handlers *******************************/

    /* ds <pktNum> : Test DS packets */
    if (strncmp(argv[0], "send", strlen("send")) == 0)
    {
        if (!hnd)
        {
            printk("PAL_cppi4Init failed\n");
            return -EFAULT;
        }

        if (6 != argc)
        {
            pp_test_usage();
            return -EFAULT;
        }

        pktSize   = (int) simple_strtol(argv[2], NULL, 0);
        pktNum    = (int) simple_strtol(argv[3], NULL, 0);
        srcPort   = (int) simple_strtol(argv[4], NULL, 0);
        dstQueue  = (int) simple_strtol(argv[5], NULL, 0);

        pktStringLength = 0;
        while ((c = strsep(&argv[1], sep))) 
        {
            pktData[pktStringLength++] = simple_strtol(c, NULL, 16);

            if (pktStringLength >= 1518)
            {
                pp_test_usage();
                return -EFAULT;
            }
        }

        if (pktStringLength > pktSize || pktSize < 64 || pktSize > 1518 || srcPort > PAL_CPPI41_SOURCE_PORT_DOCSIS || dstQueue >= PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT)
        {
            pp_test_usage();
            return -EFAULT;
        }

        queue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        queue.qNum = CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM(PAL_CPPI4x_PRTY_LOW);
        popQueueHnd = PAL_cppi4QueueOpen(hnd, queue);

        queue.qNum = dstQueue;
        dstQueueHnd = PAL_cppi4QueueOpen(hnd, queue);

        for (pktNumIndex = 0; pktNumIndex < pktNum; pktNumIndex++)
        {
            /* Get descriptor from free queue */
            desc = (Cppi4EmbdDesc*)PAL_cppi4QueuePop(popQueueHnd);
            if (!desc)
            {
                printk("Pop descriptor from queue CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM[#%d] failed\n", CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM(PAL_CPPI4x_PRTY_LOW));
                return -EFAULT;
            }
            desc = (Cppi4EmbdDesc*)PAL_CPPI4_PHYS_2_VIRT(desc);

            /* Update Descriptor Info */
            desc->descInfo = 0x18400000 + pktSize; 
            desc->tagInfo = (srcPort << 27) | 0x3FFF;
            
            /* Update Descriptor's Buffer Info */
            desc->Buf[0].BufInfo = 0;                                                       // Slot 0 is empty
            desc->Buf[0].BufPtr = 0;                                                        // Slot 0 is empty

            /* Allocate buffer, set it and put in descriptor */
            {
                /* Get Buffer from pool 0 */
                dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL0});
                while (!dataBuffer)
                {
                    dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL0});
                }
                dataBuffer = PAL_CPPI4_PHYS_2_VIRT(dataBuffer);

                /* Update Buffer Data */
                for (tmp = 0; (tmp < pktSize) && (tmp < BMGR0_POOL00_BUF_SIZE); tmp+=4)
                {
                    *(volatile Uint32*)((Uint8*)dataBuffer + tmp) = (pktData[tmp] << 24) | (pktData[tmp+1] << 16) | (pktData[tmp+2] << 8) | (pktData[tmp+3] << 0);
                }
                
                /* Update buffer in descritptor */            
                desc->Buf[1].BufInfo = 0x80000000 + (PAL_CPPI41_BMGR_POOL0 << 24) + tmp;    // Slot 1 buffer info
                desc->Buf[1].BufPtr = PAL_CPPI4_VIRT_2_PHYS(dataBuffer);                           // Slot 1 buffer pointer
                desc->pktInfo = 0x1C108000 + CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM(PAL_CPPI4x_PRTY_LOW);

                PAL_CPPI4_CACHE_WRITEBACK((unsigned long)dataBuffer, tmp);
            }

            /* Check if another buffer is needed */
            if (tmp < pktSize)
            {
                /* Get Buffer from pool 0 */
                dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL0});
                while (!dataBuffer)
                {
                    dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL0});
                }
                dataBuffer = PAL_CPPI4_PHYS_2_VIRT(dataBuffer);

                /* Update Buffer Data */
                for (; (tmp < pktSize) && (tmp < 2 * BMGR0_POOL00_BUF_SIZE); tmp+=4)
                {
                    *(volatile Uint32*)((Uint8*)dataBuffer + (tmp - BMGR0_POOL00_BUF_SIZE)) = (pktData[tmp] << 24) | (pktData[tmp+1] << 16) | (pktData[tmp+2] << 8) | (pktData[tmp+3] << 0);
                }
                
                /* Update buffer in descritptor */            
                desc->Buf[2].BufInfo = 0x80000000 + (PAL_CPPI41_BMGR_POOL0 << 24) + tmp - BMGR0_POOL00_BUF_SIZE;     // Slot 2 buffer info
                desc->Buf[2].BufPtr = PAL_CPPI4_VIRT_2_PHYS(dataBuffer);                                                    // Slot 2 buffer pointer
                desc->pktInfo = 0x1C208000 + CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM(PAL_CPPI4x_PRTY_LOW);

                PAL_CPPI4_CACHE_WRITEBACK((unsigned long)dataBuffer, tmp - BMGR0_POOL00_BUF_SIZE);
            }
            else
            {
                desc->Buf[2].BufInfo = 0;                                                       // Slot 2 is empty
                desc->Buf[2].BufPtr = 0;                                                        // Slot 2 is empty
            }

            /* Check if another buffer is needed */
            if (tmp < pktSize)
            {
                /* Get Buffer from pool 4 */
                dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL4});
                while (!dataBuffer)
                {
                    dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL4});
                }
                dataBuffer = PAL_CPPI4_PHYS_2_VIRT(dataBuffer);

                /* Update Buffer Data */
                for (; (tmp < pktSize); tmp+=4)
                {
                    *(volatile Uint32*)((Uint8*)dataBuffer + (tmp - (2*BMGR0_POOL00_BUF_SIZE))) = (pktData[tmp] << 24) | (pktData[tmp+1] << 16) | (pktData[tmp+2] << 8) | (pktData[tmp+3] << 0);
                }
                
                /* Update buffer in descritptor */            
                desc->Buf[3].BufInfo = 0x80000000 + (PAL_CPPI41_BMGR_POOL4 << 24) + tmp - (2*BMGR0_POOL00_BUF_SIZE); // Slot 3 buffer info
                desc->Buf[3].BufPtr = PAL_CPPI4_VIRT_2_PHYS(dataBuffer);                                                    // Slot 3 buffer pointer
                desc->pktInfo = 0x1C308000 + CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM(PAL_CPPI4x_PRTY_LOW);

                PAL_CPPI4_CACHE_WRITEBACK((unsigned long)dataBuffer, tmp - (2*BMGR0_POOL00_BUF_SIZE));
            }
            else
            {
                desc->Buf[3].BufInfo = 0;                                                       // Slot 3 is empty
                desc->Buf[3].BufPtr = 0;                                                        // Slot 3 is empty
            }

            
            PAL_CPPI4_CACHE_WRITEBACK((unsigned long)desc, 64);

            /* Push the packet to the dstQueue */
            PAL_cppi4QueuePush(dstQueueHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)desc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(64), pktSize);
        }
    }
    else
    {
        pp_test_usage();
        return -EFAULT;
    }

    return count;
}


int avalanche_ppd_init(void)
{
    int i;
    Cppi4Queue tmpQ;    /* Just used for filling Q info for opening */
    Cppi4BufPool tmpBufPool; /* Used for Init calls */
    PAL_Handle hnd;
    extern int ti_pp_sys_initialize (void);
    /* Initialize the PP Subsystem. */
    if (ti_pp_sys_initialize () < 0)
    {
        printk ("Error: Failed to initialize the PP Subsystem\n");
        return -1;
    }
    else
    {
        printk("avalanche_ppd_init: PP system  initialized successfully.\n");
    }

    hnd = PAL_cppi4Init (NULL, NULL);
  
    if (!hnd) 
    {
        printk("avalanche_ppd_init: CPPI41 Library NOT initialized.\n");
        return -1;
    }



    /************************************************************************/
    /*********** Init buffer pools used in the system ***********************/
    /************************************************************************/
    tmpBufPool.bMgr     = BUF_POOL_MGR0;

    /************************************************/
    /*       Ethernet and USB shared pools          */
    /*                                              */

    tmpBufPool.bPool    = CPMAC_CPPI4x_POOL_NUM(0);
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL10_REF_CNT,
                        BMGR0_POOL10_BUF_SIZE, 
                        BMGR0_POOL10_BUF_COUNT)) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /*----------------------------------------------*/

   tmpBufPool.bPool    = CPMAC_CPPI4x_POOL_NUM(1);
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL11_REF_CNT, 
                        BMGR0_POOL11_BUF_SIZE, 
                        BMGR0_POOL11_BUF_COUNT)) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    } 
    /************************************************/

    /************************************************/
    /*                PP Firmware                   */
    /*                                              */
    tmpBufPool.bPool    = BMGR0_POOL13;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool, 
                        BMGR0_POOL13_REF_CNT, 
                        BMGR0_POOL13_BUF_SIZE, 
                        BMGR0_POOL13_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /************************************************/

    /************************************************/
    /*                HOST to PP Proxy              */
    /*                                              */
    tmpBufPool.bPool    = BMGR0_POOL06;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool, 
                        BMGR0_POOL06_REF_CNT,
                        BMGR0_POOL06_BUF_SIZE,
                        BMGR0_POOL06_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /*----------------------------------------------*/
    
    tmpBufPool.bPool    = BMGR0_POOL07;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL07_REF_CNT,
                        BMGR0_POOL07_BUF_SIZE,
                        BMGR0_POOL07_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /************************************************/

    /************************************************/
    /*                DOCSIS                        */
    /*                                              */
    tmpBufPool.bPool    = BMGR0_POOL00;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL00_REF_CNT,
                        BMGR0_POOL00_BUF_SIZE,
                        BMGR0_POOL00_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /*----------------------------------------------*/

    tmpBufPool.bPool    = BMGR0_POOL01;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL01_REF_CNT,
                        BMGR0_POOL01_BUF_SIZE,
                        BMGR0_POOL01_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /*----------------------------------------------*/
    
    tmpBufPool.bPool    = BMGR0_POOL04;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL04_REF_CNT,
                        BMGR0_POOL04_BUF_SIZE,
                        BMGR0_POOL04_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /*----------------------------------------------*/
    
    tmpBufPool.bPool    = BMGR0_POOL05;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL05_REF_CNT,
                        BMGR0_POOL05_BUF_SIZE,
                        BMGR0_POOL05_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /*----------------------------------------------*/
    
    tmpBufPool.bPool    = BMGR0_POOL12;
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL12_REF_CNT,
                        BMGR0_POOL12_BUF_SIZE,
                        BMGR0_POOL12_BUF_COUNT )) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /************************************************/

    /************************************************/
    /*                MPEG I/F                      */
    /*                                              */
    tmpBufPool.bPool    = MPEG_CPPI4x_POOL_NUM(0);
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL02_REF_CNT,
                        BMGR0_POOL02_BUF_SIZE,
                        BMGR0_POOL02_BUF_COUNT)) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /*----------------------------------------------*/

    tmpBufPool.bPool    = MPEG_ENCAP_CPPI4x_POOL_NUM(0);
    if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool,
                        BMGR0_POOL08_REF_CNT,
                        BMGR0_POOL08_BUF_SIZE,
                        BMGR0_POOL08_BUF_COUNT)) == NULL)
    {
        printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
        return -1;
    }
    /************************************************/

    /******************** Buffer pool Init done *****************************/



    /************************************************************************/
    /*********** Setup Free Embedded descriptors    *************************/
    /************************************************************************/

    /************************************************/
    /*      Allocate region                         */
    /*                                              */
    ppEmbBDCfg_g.buffDescRegionPtr = 
        PAL_cppi4AllocDesc( hnd,ppEmbBDCfg_g.qMgr, 
                                ppEmbBDCfg_g.numDesc, 
                                ppEmbBDCfg_g.szDesc );

    if (!ppEmbBDCfg_g.buffDescRegionPtr) 
    {
        printk ("Embedded descriptor region allocation FAILED.\n");
        return -1;
    }
    /************************************************/
    
    {
        Cppi4EmbdDesc* currBD;
        currBD = (Cppi4EmbdDesc*)ppEmbBDCfg_g.buffDescRegionPtr;
        
        for (i = 0; i < ppEmbBDCfg_g.numBlks; i++)
        {
            BDBlkInfo* BDBlk = &ppEmbBDCfg_g.BDBlk[i];
            int bd_cnt;
            PAL_Cppi4QueueHnd tmpQHnd; 
            
            tmpQ.qMgr = ppEmbBDCfg_g.qMgr;
            tmpQ.qNum = BDBlk->qNum;
            tmpQHnd = PAL_cppi4QueueOpen (hnd, tmpQ);
            
            for (bd_cnt = 0; bd_cnt < BDBlk->numDesc; bd_cnt++)
            {
                PAL_osMemSet(currBD, 0, ppEmbBDCfg_g.szDesc);

                currBD->descInfo    = CPPI41_EM_DESCINFO_DTYPE_EMBEDDED | CPPI41_EM_DESCINFO_SLOTCNT_MYCNT;
                currBD->tagInfo     = 0;
                currBD->pktInfo     = 
                                     (BDBlk->pktType    << CPPI41_EM_PKTINFO_PKTTYPE_SHIFT) 
                                    |(1                 << CPPI41_EM_PKTINFO_RETPOLICY_SHIFT)
                                    |(1                 << CPPI41_EM_PKTINFO_PROTSPEC_SHIFT)
                                    |(ppEmbBDCfg_g.qMgr << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)
                                    |(BDBlk->qNum       << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT);
                
                PAL_CPPI4_CACHE_WRITEBACK(currBD, ppEmbBDCfg_g.szDesc);
                
                PAL_cppi4QueuePush (tmpQHnd, 
                                    (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)currBD), 
                                    (ppEmbBDCfg_g.szDesc-24)/4, 
                                    0/*!@@*/);
                
                currBD = (Cppi4EmbdDesc*)((Uint32)currBD + ppEmbBDCfg_g.szDesc);
            }
        }
    }
    /********************** Free Embedded desc setup Done  ******************/

    
    /************************************************************************/
    /*********** Setup Free Host descriptors        *************************/
    /************************************************************************/
    
    /************************************************/
    /*      Allocate region                         */
    /*                                              */
    ppHostBDCfg_g.buffDescRegionPtr = 
        PAL_cppi4AllocDesc( hnd,ppHostBDCfg_g.qMgr, 
        ppHostBDCfg_g.numDesc, 
        ppHostBDCfg_g.szDesc );
    
    if (!ppHostBDCfg_g.buffDescRegionPtr) 
    {
        printk ("Host descriptor region allocation FAILED.\n");
        return -1;
    }
    /************************************************/
    
    {
        Cppi4HostDescLinux* currBD;
        
        currBD = (Cppi4HostDescLinux*)ppHostBDCfg_g.buffDescRegionPtr;
        
        for (i = 0; i < ppHostBDCfg_g.numBlks; i++)
        {
            BDBlkInfo* BDBlk = &ppHostBDCfg_g.BDBlk[i];
            int bd_cnt;
            PAL_Cppi4QueueHnd tmpQHnd; 
            
            tmpQ.qMgr = ppHostBDCfg_g.qMgr;
            tmpQ.qNum = BDBlk->qNum;
            tmpQHnd = PAL_cppi4QueueOpen (hnd, tmpQ);
            
            for (bd_cnt = 0; bd_cnt < BDBlk->numDesc; bd_cnt++)
            {
                PAL_osMemSet(currBD, 0, ppHostBDCfg_g.szDesc);

                currBD->hw.descInfo    = (PAL_CPPI4_HOSTDESC_DESC_TYPE_HOST << PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT);
                currBD->hw.tagInfo     = 0x3FFF;
                currBD->hw.pktInfo     = 
                         (BDBlk->pktType                        << PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT) 
                        |(PAL_CPPI4_HOSTDESC_PKT_RETPLCY_LINKED << PAL_CPPI4_HOSTDESC_PKT_RETPLCY_SHIFT)
                        |(PAL_CPPI4_HOSTDESC_DESC_LOC_OFFCHIP   << PAL_CPPI4_HOSTDESC_DESC_LOC_SHIFT)
                        |(ppHostBDCfg_g.qMgr                    << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)
                        |(BDBlk->qNum                           << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT);
                
                PAL_CPPI4_CACHE_WRITEBACK(currBD, ppHostBDCfg_g.szDesc);
                
                PAL_cppi4QueuePush (tmpQHnd, 
                                    (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)currBD), 
                                    (ppHostBDCfg_g.szDesc-24)/4, 
                                    0/*!@@*/);
                
                currBD = (Cppi4HostDescLinux*)((Uint32)currBD + ppHostBDCfg_g.szDesc);
            }
        }
    }

    /********************** Free Host desc setup Done  ******************/



    /*********** Open the Queues common for devices in PP system 
     * Actually the queues only need to be opened here if the handle is required
     * (for push/pop generally) and any driver depending on these Qs would do
     * so. Still opening here to provide idea of the system.
     * Also, opening these queue here would mean that any subsequent calls (from
     * drivers) to open these queues will just return the same handle to the
     * queue without resetting them
     */
    /************************************************************************/
    /* Open Tx Qs. These will be used by drivers to push for Tx. */
    /* Infra Qs */
    for (i = 0; CPPI4x_PPFW_TX_INFRA_CH_COUNT > i; i++)
    {
        /* Input Queue */
        tmpQ.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        tmpQ.qNum   = CPPI4x_PPFW_TX_INFRA_INPUT_QNUM(i);
        PAL_cppi4QueueOpen (hnd, tmpQ);
        
        /* Output Queue */
        tmpQ.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        tmpQ.qNum   = PPFW_CPPI4x_TX_EGRESS_EMB_QNUM(i);
        PAL_cppi4QueueOpen (hnd, tmpQ);
        
        setup_host2pp_infra (hnd, i);
    }

    tmpQ.qMgr   = RECYCLE_INFRA_RX_QMGR;
    tmpQ.qNum   = RECYCLE_INFRA_RX_Q(0);
    PAL_cppi4QueueOpen(hnd, tmpQ);
    /* This is done in order to keep the reference count of these queues non zero so they will not be closed */
    tmpQ.qMgr   = CPMAC_CPPI4x_TX_QMGR;
    tmpQ.qNum   = CPMAC_CPPI4x_TX_QNUM(0);
    PAL_cppi4QueueOpen(hnd, tmpQ);
    tmpQ.qNum   = CPMAC_CPPI4x_TX_QNUM(1);
    PAL_cppi4QueueOpen(hnd, tmpQ);
    setupRecycleInfra(hnd);

    /****************************************************************/
    /*  Set Up the rest of PP Egress queues                         */
    /****************************************************************/
    for (i = 0; PPFW_CPPI4x_TX_EGRESS_Q_COUNT > i; i++)
    {
        tmpQ.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        tmpQ.qNum   = PPFW_CPPI4x_TX_EGRESS_HOST_QNUM(i);
        PAL_cppi4QueueOpen (hnd, tmpQ);
    }
    return 0;
}

static const struct file_operations pp_test_proc_fops = {
  .write = pp_test
};

int avalanche_prefetcher_register_callback(void (*cb)(int))
{
    if (prefetcher_init_status != 0xdeadf00d) {
        cb(prefetcher_init_status);
        return 0;
    }
    if (prefetcher_init_cb_num == MAX_PREFETCHER_INIT_CB) {
        printk("Failed to register prefetcher callback: too many callbacks\n");
        return -1;
    }
    prefetcher_init_cb[prefetcher_init_cb_num++] = cb;
    return 0;
}

int avalanche_prefetcher_init (void)
{
    int i;
    int ret = 0;
    Uint32 pRcbMem;
    APDSP_Command_Buffer_RegsOvly apdsp;
    PAL_Handle hnd, preFQHnd, preFBQHnd; 
    hnd = PAL_cppi4Init (NULL, NULL);
    /*****************************************************************
     ******************* Setup Prefetcher ***************************
     ****************************************************************/

    /* Push Prefetcher free desc */
    pRcbMem = (Uint32)prefCfg_g.pfDescBase;
    preFQHnd = PAL_cppi4QueueOpen (hnd, prefCfg_g.pfFQ);
    for( i=0; i<prefCfg_g.pfDescCnt; i++ ) {
        PAL_cppi4QueuePush (preFQHnd, (Ptr) pRcbMem, 0, 0);
        pRcbMem += PREFETCH_FD_SIZE;    // Next buffer descriptor
    }
    /* Push replicator free desc */
    pRcbMem = (Uint32)prefCfg_g.repDescBase;
    preFQHnd = PAL_cppi4QueueOpen (hnd, prefCfg_g.repFQ);
    for( i=0; i<prefCfg_g.repDescCnt; i++ ) {
        PAL_cppi4QueuePush (preFQHnd, (Ptr) pRcbMem, 0, 0);
        pRcbMem += PREFETCH_FD_SIZE;    // Next buffer descriptor
    }

    /* Setup prefetch buffers */
    pRcbMem = (Uint32)prefCfg_g.pfBlkBase;
    preFBQHnd = PAL_cppi4QueueOpen (hnd, prefCfg_g.pfFBQ);
    for( i=0; i<prefCfg_g.pfBlkCnt; i++ )
    {
        PAL_cppi4QueuePush (preFBQHnd, (Ptr) pRcbMem, 0, 0);
        pRcbMem += 128;                  // Next buffer descriptor
    }    


    apdsp = prefCfg_g.pfCmdBase;
    //--------------------------------------------------------
    // Config Prefetcher
    //
    // Setup prefetch to fetch 96 bytes with a 24 byte offset
    // NOTE: Use 128 byte fetch with no offset on USB
    //
    //apdsp->Parameter0 = (24<<8) | 96;

    

    apdsp->Parameter0 = (0<<8) | 128;
    apdsp->Command = ACMD_COMMAND(ACMD_CONFIG_PREFETCH);
    for( i=0; i<1000000 && ACMD_GET_COMMAND(apdsp->Command); i++);
    if( i==1000000 ) { 
        printk("%s(%d): Error - APDSP firmware not responding!\n", 
                __FUNCTION__, __LINE__); 
        ret = -1;
        goto out;
    }

    if (ACMD_GET_RETCODE(apdsp->Command) != 1)
        printk("%s(%d):return code 0x%02x\n", 
                __FUNCTION__, __LINE__, ACMD_GET_RETCODE(apdsp->Command));
    
    //--------------------------------------------------------
    //
    // Setup Teardown descriptor queue
    // Note:  The prefetcher is configured to route the teardown
    // descriptors back to free teardown desc queue. This avoids prefetcher
    // going into invalid state when TD is received. 
    // CHI: __IMPORTANT__ This configuration constraints ALL PP related Endpoint
    // Rx channels to be configured to use SAME Teardown Queue otherwise TDs
    // will be lost (since the queue number specified in DMA configuration to
    // pick the TD and Queue number for Prefetcher to queue the TD will be
    // different).
    //
    apdsp->Parameter0 = (DMA1_CPPI4x_FTD_QMGR<<16) | DMA1_CPPI4x_FTD_QNUM;
    apdsp->Command = ACMD_COMMAND(ACMD_CONFIG_TDQ);
    for( i=0; i<1000000 && ACMD_GET_COMMAND(apdsp->Command); i++);
    if( i==1000000 ) { 
        printk("%s(%d): Error - APDSP firmware not responding!\n", 
                __FUNCTION__, __LINE__); 
        ret = -1;
        goto out;
    }
    if (ACMD_GET_RETCODE(apdsp->Command) != 1)
        printk("%s(%d):return code 0x%02x\n", 
                __FUNCTION__, __LINE__, ACMD_GET_RETCODE(apdsp->Command));


    //--------------------------------------------------------
    // Enable Prefetcher
    //
    // Enable prefetcher
    //
    apdsp->Command = ACMD_INDEX(1) |
        ACMD_COMMAND(ACMD_ENABLE_PREFETCH);

    for( i=0; i<1000000 && ACMD_GET_COMMAND(apdsp->Command); i++);
    if( i==1000000 ) {
        printk("%s(%d): Error - APDSP firmware not responding!\n", 
                __FUNCTION__, __LINE__); 
        ret = -1;
        goto out;
    }

    if (ACMD_GET_RETCODE(apdsp->Command) != 1)
        printk("%s(%d):return code 0x%02x\n", 
                __FUNCTION__, __LINE__, ACMD_GET_RETCODE(apdsp->Command));
    /*********** Prefetcher setup done **************************/
    {
        struct proc_dir_entry *pp_test_proc = proc_create("pp_test" ,0644, init_net.proc_net, &pp_test_proc_fops);
        if(!pp_test_proc)
        {
            printk("%s(%d): Error - pp_test proc creation failed\n", __FUNCTION__, __LINE__);
            ret = -1;
            goto out;
        }
    }

out:
    prefetcher_init_status = ret;
    for (i = 0; i < prefetcher_init_cb_num; i++) {
        prefetcher_init_cb[i](ret);
    }
    prefetcher_init_cb_num = 0; /* just to protect from double calls. */
    return 0;
}


static Int32 pp_pref_exec_stats_cmd (int stats_index)
{
    int i, ret_val;
    APDSP_Command_Buffer_RegsOvly apdsp;

    apdsp = prefCfg_g.pfCmdBase;
    
    printk ("Writing %#x\n", ACMD_INDEX(stats_index)
                        | ACMD_COMMAND(ACMD_READ_STATISTICS));

    apdsp->Command = ACMD_INDEX(stats_index)
                        | ACMD_COMMAND(ACMD_READ_STATISTICS);

    for( i=0; i<1000000 && ACMD_GET_COMMAND(apdsp->Command); i++);
    if( i==1000000 ) {
        printk("%s(%d): Error - APDSP firmware not responding!\n",
                __FUNCTION__, __LINE__);
        return -1;
    }

    if ((ret_val = ACMD_GET_RETCODE(apdsp->Command)) != 1){
        printk("%s(%d):return code 0x%02x\n",
                __FUNCTION__, __LINE__, ACMD_GET_RETCODE(apdsp->Command));
        return -(ret_val);
    }

    return 0;
}

Int32 ti_pp_get_n_clear_pref_stats (TI_PP_PREF_STATS *stats)
{
    int i, ret_val;
    APDSP_Command_Buffer_RegsOvly apdsp;
    apdsp = prefCfg_g.pfCmdBase;

    /* Get and clear Group A stats */
    if (!(ret_val = pp_pref_exec_stats_cmd (0)))
    {
        stats->grp_a_preproc_pkts       = apdsp->Parameter0;
        stats->grp_a_pref_buf_pkts      = apdsp->Parameter1;
        stats->grp_a_pref_descbuff_pkts = apdsp->Parameter2;
        stats->grp_a_desc_starv_cnt     = apdsp->Parameter3;
    }
    else 
        return ret_val;

    /* Get and clear Group B stats */
    if (!(ret_val = pp_pref_exec_stats_cmd (1)))
    {
        stats->grp_b_preproc_pkts       = apdsp->Parameter0;
        stats->grp_b_pref_buf_pkts      = apdsp->Parameter1;
        stats->grp_b_pref_descbuff_pkts = apdsp->Parameter2;
        stats->grp_b_desc_starv_cnt     = apdsp->Parameter3;
    }
    else 
        return ret_val;

    /* Get and clear input Queue stats */
    for (i = 0; i < 6; i++)
    {
        if (!(ret_val = pp_pref_exec_stats_cmd (2+i)))
        {
            stats->in_q_congst_discards[i]  = apdsp->Parameter0;
            stats->in_q_congst_thrsh[i]     = apdsp->Parameter1;
        }
        else 
            return ret_val;
    }

    return 0;
}

Int32 ti_pp_get_pref_stats (TI_PP_PREF_STATS *stats)
{
    int i;
    volatile Uint32* pref_statsblk_base 
        = (volatile Uint32*)((Uint32)(AVALANCHE_NWSS_APDSP_PREFBLK_BASE) 
            + 0xC20);

    /* Get Group A stats */
    stats->grp_a_preproc_pkts       = *(pref_statsblk_base + 0);
    stats->grp_a_pref_buf_pkts      = *(pref_statsblk_base + 1);
    stats->grp_a_pref_descbuff_pkts = *(pref_statsblk_base + 2);
    stats->grp_a_desc_starv_cnt     = *(pref_statsblk_base + 3);

    /* Get Group B stats */
    stats->grp_b_preproc_pkts       = *(pref_statsblk_base + 4);
    stats->grp_b_pref_buf_pkts      = *(pref_statsblk_base + 5);
    stats->grp_b_pref_descbuff_pkts = *(pref_statsblk_base + 6);
    stats->grp_b_desc_starv_cnt     = *(pref_statsblk_base + 7);


    /* Get input Queue stats */
    for (i = 0; i < 6; i++)
    {
        stats->in_q_congst_discards[i]  = *(pref_statsblk_base + 8 + i);
        stats->in_q_congst_thrsh[i]     = *(pref_statsblk_base + 8 + i);
    }

    return 0;
}

Int32 ti_pp_enable_psm (void)
{
    int i, ret_val;
    APDSP_Command_Buffer_RegsOvly apdsp;
    apdsp = prefCfg_g.pfCmdBase;
    
    printk("%s: Enable prefetcher PSM mode\n", __FUNCTION__);
    /* Enable prefetcher PSM */
    apdsp->Command = ACMD_INDEX(2) | ACMD_COMMAND(ACMD_ENABLE_PREFETCH);
    
    /* Check APDSP responsiveness */
    for(i = 0; i < 1000000 && ACMD_GET_COMMAND(apdsp->Command); i++);
    if(i==1000000)
    {
        printk("%s(%d): Error - APDSP firmware not responding!\n", __FUNCTION__, __LINE__); 
        return -1; 
    }
    
    ret_val = ACMD_GET_RETCODE(apdsp->Command);
    
    /* For now, treat IDLE, NON IDLE as success */
    if ((ret_val) && (ret_val != 1) && (ret_val != 8))
    {
        printk("%s(%d):return code 0x%02x\n", __FUNCTION__, __LINE__, ret_val);
        return -(ret_val);
    }
    
    /* 
    * CHK: Either use SETPSM command of SR or use PDSP control API to halt
    * the SR PDSPs
    */
    
    printk("%s: Enable PPD PSM mode\n", __FUNCTION__);
    i = 1;
    ti_ppd_pdsp_control (0, TI_PP_PDSPCTRL_PSM, (Ptr)&i);
    
    printk("%s: Halt PP PDSPs\n", __FUNCTION__);
    /* Set LPM for PP PDSPs - clock gated, reset DE-asserted */
    /* Note: Do not try to power down LUT (PSC_SR_CLK2) */
    // PAL_sysPowerCtrl((INT32)PSC_SR_CLK2, PSC_DISABLE); /* Power Down QPDSP */
    PAL_sysPowerCtrl((INT32)PSC_SR_CLK1, PSC_DISABLE); /* Power Down MPDSP */
    PAL_sysPowerCtrl((INT32)PSC_SR_CLK0, PSC_DISABLE); /* Power Down CPDSP */
    
    return 0;
}

Int32 ti_pp_disable_psm (void)
{
    int i, ret_val;
    APDSP_Command_Buffer_RegsOvly apdsp;
    apdsp = prefCfg_g.pfCmdBase;

    printk("%s: Run PP PDSPs\n", __FUNCTION__);

    PAL_sysPowerCtrl((INT32)PSC_SR_CLK0, PSC_ENABLE); /* Power Up CPDSP */
    PAL_sysPowerCtrl((INT32)PSC_SR_CLK1, PSC_ENABLE); /* Power Up MPDSP */
    PAL_sysPowerCtrl((INT32)PSC_SR_CLK2, PSC_ENABLE); /* Power Up QPDSP */

    /* Disable prefetcher PSM */
    printk("%s: Disable prefetcher PSM mode\n", __FUNCTION__);
    apdsp->Command = ACMD_INDEX(1) | ACMD_COMMAND(ACMD_ENABLE_PREFETCH);

    for(i = 0; i < 1000000 && ACMD_GET_COMMAND(apdsp->Command); i++);
    if(i==1000000) 
    {
        printk("%s(%d): Error - APDSP firmware not responding!\n", __FUNCTION__, __LINE__); 
        return -1; 
    }

    ret_val = ACMD_GET_RETCODE(apdsp->Command);
    /* For now, treat IDLE, NON IDLE as success */
    if ((ret_val) && (ret_val != 1) && (ret_val != 8))
    {
        printk("%s(%d):return code 0x%02x\n", __FUNCTION__, __LINE__, ret_val);
        return -(ret_val);
    }

    /* 
     * CHK: Either use SETPSM command of SR or use PDSP control API to resume
     * the SR PDSPs
     */
    printk("%s: Disable PPD PSM mode\n", __FUNCTION__);
    i = 0;
    ti_ppd_pdsp_control (0, TI_PP_PDSPCTRL_PSM, (Ptr)&i);

    return 0;
}

subsys_initcall(avalanche_ppd_init);
EXPORT_SYMBOL(avalanche_prefetcher_init);
EXPORT_SYMBOL(avalanche_prefetcher_register_callback);
EXPORT_SYMBOL(ti_pp_get_pref_stats);
EXPORT_SYMBOL(ti_pp_get_n_clear_pref_stats);
EXPORT_SYMBOL(ti_pp_enable_psm);
EXPORT_SYMBOL(ti_pp_disable_psm);

/*
 * Exported PPD APIs to facilitate use from modules. Coule have put these in
 * avalanche_misc.c instead of here, but since ti_ppd.h depends on ti_ppm.h and
 * ti_ppm.h is not in standard path, we get build errors wherever pformCfg.h is
 * included.
 */
EXPORT_SYMBOL(ti_ppd_init);
EXPORT_SYMBOL(ti_ppd_create_session);
EXPORT_SYMBOL(ti_ppd_delete_session);
EXPORT_SYMBOL(ti_ppd_config_pid_range);
EXPORT_SYMBOL(ti_ppd_remove_pid_range);
EXPORT_SYMBOL(ti_ppd_create_pid);
EXPORT_SYMBOL(ti_ppd_set_pid_flags);
EXPORT_SYMBOL(ti_ppd_delete_pid);
EXPORT_SYMBOL(ti_ppd_create_vpid);
EXPORT_SYMBOL(ti_ppd_set_vpid_flags);
EXPORT_SYMBOL(ti_ppd_delete_vpid);
EXPORT_SYMBOL(ti_ppd_get_vpid_stats);
EXPORT_SYMBOL(ti_ppd_clear_vpid_stats);
EXPORT_SYMBOL(ti_ppd_get_n_clear_vpid_stats);
EXPORT_SYMBOL(ti_ppd_get_srl_pkt_stats);
EXPORT_SYMBOL(ti_ppd_clear_srl_pkt_stats);
EXPORT_SYMBOL(ti_ppd_get_n_clear_srl_pkt_stats);
EXPORT_SYMBOL(ti_ppd_get_session_pkt_stats);
EXPORT_SYMBOL(ti_ppd_clear_session_pkt_stats);
EXPORT_SYMBOL(ti_ppd_get_n_clear_session_pkt_stats);
EXPORT_SYMBOL(ti_ppd_register_event_handler);
EXPORT_SYMBOL(ti_ppd_deregister_event_handler);
EXPORT_SYMBOL(ti_ppd_health_check);
EXPORT_SYMBOL(ti_ppd_qos_cluster_setup);
EXPORT_SYMBOL(ti_ppd_qos_cluster_enable);
EXPORT_SYMBOL(ti_ppd_qos_cluster_disable);
EXPORT_SYMBOL(ti_ppd_get_qos_q_stats);
EXPORT_SYMBOL(ti_ppd_get_n_clear_qos_q_stats);
EXPORT_SYMBOL(ti_ppd_get_pdsp_status);
EXPORT_SYMBOL(ti_ppd_pdsp_control);
EXPORT_SYMBOL(ti_ppd_get_ses_age);

