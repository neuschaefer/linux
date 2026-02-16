/*
 * puma6_pp.c
 *  Description:
 * Puma-6 Packet Processor initialization.
 * Contains Puma-6 specific initialization. The numbers (addresses etc) are
 * Puma-6 specific. The static structures are filled in with Puma-6 specific
 * data and the generic PPD init function gets called in the end
 * with this data.
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2013-2014 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution 
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052 
 */

#include <pal.h>
#include <pal_cppi41.h>
#include <pal_cppi41pvt.h>
#include <linux/netdevice.h>
#include <linux/proc_fs.h>
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <ti_ppd.h>
#include <puma6_pp.h>
#include <puma6_cppi.h>

/* Enable only ONE of the following */
//#define TX_COMPLETE_USE_TASKLET
#define TX_COMPLETE_USE_WORKQ


#if (defined(TX_COMPLETE_USE_TASKLET) + defined(TX_COMPLETE_USE_WORKQ) != 1)
#   error "Only Enable ONE of the TX_COMPLETE_USE_ options!!!"
#endif
/*
 * PrefchCfg -
 * Description:
 *  Prefetcher Configuration
 */
typedef struct
{
    volatile Uint32* pfDescBase;
    Uint32           pfDescCnt;
    Cppi4Queue       pfFQ;
} 
PrefchCfg;


/* On-Chip descriptor management
 * IMPORTANT NOTES:
 * ===============
 *  -   Following macros control how the on-chip descriptor memory is devided
 *  into different descriptors. All the base addresses must be varefully
 *  determined not to avoid overlap.
 *  -   Also note that all the On-cip descriptors *must* be of size equal to the
 *  specified in puma6_cppi.c cppi config structure for respective region.
 */

PrefchCfg prefCfg_g =
{
    /* Prefetcher Free Descriptors */
    .pfDescBase = (Ptr)IO_VIRT2PHY( AVALANCHE_NWSS_ONCHIPDESC_BASE ),
    .pfDescCnt  = PAL_CPPI41_SR_PPDSP_PREFETCH_DESC_FD_DESC_COUNT,
    .pfFQ.qMgr  = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .pfFQ.qNum  = PAL_CPPI41_SR_PPDSP_PREFETCH_DESC_FD_Q_NUM,
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
    BDBlkInfo   BDBlk [PAL_CPPI41_SR_FD_EMB_Q_LAST - PAL_CPPI41_SR_FD_EMB_Q_BASE];
} EmbBDCfg;

EmbBDCfg ppEmbBDCfg_g =
{
    .qMgr       = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .numDesc    = PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_DESC_COUNT +
                  PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_DESC_COUNT +
                  PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_DESC_COUNT +
                  PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_DESC_COUNT +
                  PAL_CPPI41_SR_PROXY_PDSP_LOW_FD_EMB_DESC_COUNT +
                  PAL_CPPI41_SR_PROXY_PDSP_HIGH_FD_EMB_DESC_COUNT,

                      /* Must be same as in Cppi Cgf structure    */
    .szDesc     = 64,   /* Must be same as for specific region      */
    .numBlks    = 8,    /* Change for each block added/removed      */

    /* Host2PP Infra 0 */
    .BDBlk[0].numDesc   = PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_DESC_COUNT,
    .BDBlk[0].qNum      = PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_Q_NUM,
    .BDBlk[0].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* Host2PP Infra 1 */
    .BDBlk[1].numDesc   = PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_DESC_COUNT,
    .BDBlk[1].qNum      = PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_Q_NUM,
    .BDBlk[1].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* DOCSIS RX CoP */
    .BDBlk[2].numDesc   = PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_DESC_COUNT,
    .BDBlk[2].qNum      = PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM,
    .BDBlk[2].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,
    .BDBlk[3].numDesc   = PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_DESC_COUNT,
    .BDBlk[3].qNum      = PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_Q_NUM,
    .BDBlk[3].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* Proxy PDSP RX */
    .BDBlk[4].numDesc   = PAL_CPPI41_SR_PROXY_PDSP_LOW_FD_EMB_DESC_COUNT,
    .BDBlk[4].qNum      = PAL_CPPI41_SR_PROXY_PDSP_LOW_FD_EMB_Q_NUM,
    .BDBlk[4].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,
    .BDBlk[5].numDesc   = PAL_CPPI41_SR_PROXY_PDSP_HIGH_FD_EMB_DESC_COUNT,
    .BDBlk[5].qNum      = PAL_CPPI41_SR_PROXY_PDSP_HIGH_FD_EMB_Q_NUM,
    .BDBlk[5].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,
};

typedef struct
{
    Uint32  buffSize;
    Uint32  buffCount;
    Uint32  refCount;
}PAL_CPPI41_SR_BUFF_INFO_t;

PAL_CPPI41_SR_BUFF_INFO_t ppBuffPoolInfo[PAL_CPPI41_BMGR_MAX_POOLS] =
{
    {.buffSize = 0,                     .buffCount = 0,                      .refCount = 0},
    {.buffSize = BMGR0_POOL01_BUF_SIZE, .buffCount = BMGR0_POOL01_BUF_COUNT, .refCount = BMGR0_POOL01_REF_CNT},
    {.buffSize = BMGR0_POOL02_BUF_SIZE, .buffCount = BMGR0_POOL02_BUF_COUNT, .refCount = BMGR0_POOL02_REF_CNT},
    {.buffSize = BMGR0_POOL03_BUF_SIZE, .buffCount = BMGR0_POOL03_BUF_COUNT, .refCount = BMGR0_POOL03_REF_CNT},
    {.buffSize = BMGR0_POOL04_BUF_SIZE, .buffCount = BMGR0_POOL04_BUF_COUNT, .refCount = BMGR0_POOL04_REF_CNT},
    {.buffSize = BMGR0_POOL05_BUF_SIZE, .buffCount = BMGR0_POOL05_BUF_COUNT, .refCount = BMGR0_POOL05_REF_CNT},
    {.buffSize = BMGR0_POOL06_BUF_SIZE, .buffCount = BMGR0_POOL06_BUF_COUNT, .refCount = BMGR0_POOL06_REF_CNT},
    {.buffSize = BMGR0_POOL07_BUF_SIZE, .buffCount = BMGR0_POOL07_BUF_COUNT, .refCount = BMGR0_POOL07_REF_CNT},
    {.buffSize = BMGR0_POOL08_BUF_SIZE, .buffCount = BMGR0_POOL08_BUF_COUNT, .refCount = BMGR0_POOL08_REF_CNT},
    {.buffSize = BMGR0_POOL09_BUF_SIZE, .buffCount = BMGR0_POOL09_BUF_COUNT, .refCount = BMGR0_POOL09_REF_CNT},
    {.buffSize = BMGR0_POOL10_BUF_SIZE, .buffCount = BMGR0_POOL10_BUF_COUNT, .refCount = BMGR0_POOL10_REF_CNT},
    {.buffSize = BMGR0_POOL11_BUF_SIZE, .buffCount = BMGR0_POOL11_BUF_COUNT, .refCount = BMGR0_POOL11_REF_CNT},
    {.buffSize = 0,                     .buffCount = 0,                       .refCount = 0},
    {.buffSize = BMGR0_POOL13_BUF_SIZE, .buffCount = BMGR0_POOL13_BUF_COUNT, .refCount = BMGR0_POOL13_REF_CNT},
    {.buffSize = 0,                     .buffCount = 0,                      .refCount = 0},
    {.buffSize = 0,                     .buffCount = 0,                      .refCount = 0},
    {.buffSize = 0,                     .buffCount = 0,                      .refCount = 0},
    {.buffSize = 0,                     .buffCount = 0,                      .refCount = 0}
};

#ifdef P6_PP_DEBUG
/* note: prints function name for you */
#  define DPRINTK(fmt, args...) printk("%s: " fmt, __FUNCTION__ , ## args)
#else
#  define DPRINTK(fmt, args...)
#endif

#define PAL_CPPI41_ACC_MAX_PAGE_ENTRIES                32
#define PAL_CPPI41_ACC_LIST_NULL_TERM                  0
#define PAL_CPPI41_ACC_PACE_MODE_LASTINTR              1
#define PAL_CPPI41_ACC_PACE_TICK_CNT                   40
#define PAL_CPPI41_ACC_MAX_PAGE_COUNT                  2

#if defined(TX_COMPLETE_USE_TASKLET)
struct tasklet_struct   gTxCompleteTasklet;     /* Tx completion processing tasklet */
#elif defined (TX_COMPLETE_USE_WORKQ)
typedef struct
{
    struct work_struct work;
} tx_work_t;

struct workqueue_struct *tx_wq;
tx_work_t         tx_work;

#endif

PAL_Cppi4AccChHnd       gTxCompleteAccChHnd[PAL_CPPI41_TX_COMPLETE_ACC_CH_COUNT];
Ptr                     gTxCompleteAccListBase[PAL_CPPI41_TX_COMPLETE_ACC_CH_COUNT];
Cppi4HostDescLinux**    gTxCompleteAccList[PAL_CPPI41_TX_COMPLETE_ACC_CH_COUNT];
PAL_Cppi4QueueHnd       gHost2ppFreeHostDescQueueHnd[PAL_CPPI41_SR_HOST_TO_PP_Q_COUNT];



/**************************************************************************/
/*! \fn static void __setup_Recycle_Infra (PAL_Handle hnd)
 **************************************************************************
 *  \brief Setup infrastructure DMA for recycling use resources.
 *  \return none.
 **************************************************************************/
static void __setup_Recycle_Infra (PAL_Handle hnd)
{
    Cppi4TxChInitCfg txCh;
    Cppi4RxChInitCfg rxCh;
    PAL_Cppi4TxChHnd cppi4TxChHnd;
    PAL_Cppi4RxChHnd cppi4RxChHnd;
    Cppi4Queue tmpQ;

    tmpQ.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    tmpQ.qNum = PAL_CPPI41_SR_RECYCLE_FD_HOST_Q_NUM;
    PAL_cppi4QueueOpen (hnd, tmpQ);

    /* Set up Rx channel */
    rxCh.chNum              = PAL_CPPI41_SR_RECYCLING_INFRA_DMA3_RX_CH_NUM;
    rxCh.dmaNum             = PAL_CPPI41_DMA_BLOCK3;
    rxCh.sopOffset          = 0;
    rxCh.retryOnStarvation  = 0;
    rxCh.rxCompQueue.qMgr   = 0;
    rxCh.rxCompQueue.qNum   = 0;

    rxCh.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI41_SR_RECYCLE_FD_HOST_Q_NUM;
    rxCh.u.embeddedPktCfg.numBufSlot = CPPI41_EM_DESCINFO_SLOTCNT_PP;
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
    txCh.chNum          = PAL_CPPI41_SR_RECYCLING_INFRA_DMA3_TX_CH_NUM;
    txCh.dmaNum         = PAL_CPPI41_DMA_BLOCK3;
    txCh.tdQueue.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    txCh.tdQueue.qNum   = PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM;

    cppi4TxChHnd        = PAL_cppi4TxChOpen(hnd, &txCh, NULL);

    if (!cppi4TxChHnd || !cppi4RxChHnd)
    {
        printk ("%s: infra channel setup failed for channel %d\n", __FUNCTION__, PAL_CPPI41_SR_RECYCLING_INFRA_DMA3_TX_CH_NUM);
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
    BDBlkInfo   BDBlk [PAL_CPPI41_SR_FD_HOST_Q_LAST - PAL_CPPI41_SR_FD_HOST_Q_BASE];
} HostBDCfg;


HostBDCfg ppHostBDCfg_g =
{
    .qMgr       = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .numDesc    = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_DESC_COUNT,    /* Must be same as in Cppi Cgf structure */
    .szDesc     = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_DESC_SIZE,   /* Must be same as for specific region   */
    .numBlks    = 2,                                            /* Change for each block added/removed   */

    /* Host2PP Low */
    .BDBlk[0].numDesc   = PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_DESC_COUNT,
    .BDBlk[0].qNum      = PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_Q_NUM,
    .BDBlk[0].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,

    /* Host2PP High */
    .BDBlk[1].numDesc   = PAL_CPPI41_SR_HOST_TO_PP_HIGH_FD_HOST_DESC_COUNT,
    .BDBlk[1].qNum      = PAL_CPPI41_SR_HOST_TO_PP_HIGH_FD_HOST_Q_NUM, //PPFW_CPPI4x_FD_QMGR
    .BDBlk[1].pktType   = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH,
};


static void __setup_Host2pp_Infra(PAL_Handle hnd, int infra_chan)
{
    Cppi4TxChInitCfg txCh;
    Cppi4RxChInitCfg rxCh;
    PAL_Cppi4TxChHnd cppi4TxChHnd;
    PAL_Cppi4RxChHnd cppi4RxChHnd;

    /* Set up Rx channel */
    rxCh.chNum              = PAL_CPPI41_SR_HOST_TO_PP_INFRA_DMA_CH_NUM(infra_chan);
    rxCh.dmaNum             = PAL_CPPI41_DMA_BLOCK3;
    rxCh.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.sopOffset          = 4; /* 4 bytes headroom for L2SW UDMA metadata */
    rxCh.rxCompQueue.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    rxCh.rxCompQueue.qNum   = PAL_CPPI41_SR_HOST_TO_QPDSP_EMB_TYPE_Q_NUM(infra_chan);
    rxCh.retryOnStarvation  = 0;

    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI41_SR_HOST_TO_PP_INFRA_FD_EMB_Q_NUM(infra_chan);
    rxCh.u.embeddedPktCfg.numBufSlot = CPPI41_EM_DESCINFO_SLOTCNT_PP;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI41_BMGR_POOL9;
    rxCh.u.embeddedPktCfg.fBufPool[1].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[1].bPool = PAL_CPPI41_BMGR_POOL9;
    rxCh.u.embeddedPktCfg.fBufPool[2].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[2].bPool = PAL_CPPI41_BMGR_POOL9;
    rxCh.u.embeddedPktCfg.fBufPool[3].bMgr  = BUF_POOL_MGR0;
    rxCh.u.embeddedPktCfg.fBufPool[3].bPool = PAL_CPPI41_BMGR_POOL9;
    cppi4RxChHnd        = PAL_cppi4RxChOpen(hnd, &rxCh, NULL);

    /* Set up Tx channel */
    txCh.chNum          = PAL_CPPI41_SR_HOST_TO_PP_INFRA_DMA_CH_NUM(infra_chan);
    txCh.dmaNum         = PAL_CPPI41_DMA_BLOCK3;
    txCh.tdQueue.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    txCh.tdQueue.qNum   = PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM;

    cppi4TxChHnd        = PAL_cppi4TxChOpen(hnd, &txCh, NULL);

    if (!cppi4TxChHnd || !cppi4RxChHnd)
    {
        printk ("%s: infra channel setup failed for channel %d\n", __FUNCTION__, infra_chan);
        return;
    }

    /* Enable Tx-Rx channels */
    PAL_cppi4EnableRxChannel (cppi4RxChHnd, NULL);
    PAL_cppi4EnableTxChannel (cppi4TxChHnd, NULL);

    return;
}


#if defined(TX_COMPLETE_USE_TASKLET)
static void __do_tx_complete(unsigned long data)
#elif defined(TX_COMPLETE_USE_WORKQ)
static void __do_tx_complete(struct work_struct *work)
#endif
{
    Cppi4HostDescLinux* hostDesc;
    Uint32      packets_processed = 0;
    Int32       priority;

#if defined(TX_COMPLETE_USE_WORKQ)
    tx_work_t *curr_work = (tx_work_t *)work;
#endif

#if defined(TX_COMPLETE_USE_WORKQ)
    static int first = 1;
    struct task_struct *ctask = current;

    if (first)
    {
        /* Set priority */
        set_user_nice(current, -10);
        first = 0;

        printk("**** ---- >>>> tx_complete - name \"%s\", pid %d\n", ctask->comm, ctask->pid);
    }
#endif

    /* Start with high priority channel */
    for (priority = PAL_CPPI41_TX_COMPLETE_ACC_CH_COUNT - 1; priority >= 0; priority--)
    {
        /* While there are ready pages... */
        while(avalanche_intd_get_interrupt_count(0, PAL_CPPI41_TX_COMPLETE_ACC_CH_NUM(priority)))
        {
            /* While there are descriptors in the page... */
            while((hostDesc = (Cppi4HostDescLinux*)((unsigned long)*gTxCompleteAccList[priority] & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK)))
            {
                hostDesc = PAL_CPPI4_PHYS_2_VIRT(hostDesc);
                PAL_CPPI4_CACHE_INVALIDATE(hostDesc, PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_DESC_SIZE);

                dev_kfree_skb_any(hostDesc->skb);
                hostDesc->skb = NULL;

                /* Queue back the hostDesc to free pool */
                PAL_cppi4QueuePush(gHost2ppFreeHostDescQueueHnd[priority] , (Ptr)PAL_CPPI4_VIRT_2_PHYS(hostDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_DESC_SIZE), 0);

                packets_processed++;
                gTxCompleteAccList[priority]++;
            }

            /* Update the list entry for next time */
            gTxCompleteAccList[priority] = PAL_cppi4AccChGetNextList(gTxCompleteAccChHnd[priority]);

            /* Decrement number of pages by 1 */
            avalanche_intd_set_interrupt_count(0, PAL_CPPI41_TX_COMPLETE_ACC_CH_NUM(priority), 1);
        }
    }

    /* First clear the IRQ in order not to get a false interrupt since INTD is level */
    ack_irq(MAP_INTD_TO_INTC(PAL_CPPI41_TX_COMPLETE_ACC_INTV_NUM));

    /* Send INTD EOI */
    avalanche_intd_write_eoi(PAL_CPPI41_TX_COMPLETE_ACC_INTV_NUM);

    /* It could be that between INTD count decrement and EOI the accumulator will issue another interrupt.
       The logic of INTD is such that level will remain active high even after EOI is set, so INTC will
       lose the interrupt after ack_irq is done (it now expects INTD polarity change).
       Therefore we must check INTD count and if it is not 0 - reschedule the tasklet */
    for (priority = PAL_CPPI41_TX_COMPLETE_ACC_CH_COUNT - 1; priority >= 0; priority--)
    {
        if (avalanche_intd_get_interrupt_count(0, PAL_CPPI41_TX_COMPLETE_ACC_CH_NUM(priority)))
        {
#if defined(TX_COMPLETE_NETDEV_USE_TASKLET)
            tasklet_schedule(&gTxCompleteTasklet);
#elif defined(TX_COMPLETE_USE_WORKQ)

            /* Que work */
            queue_work(tx_wq, &(tx_work.work));
#endif            
            return;
        }
    }

    /* Now enable the IRQ */
    enable_irq(MAP_INTD_TO_INTC(PAL_CPPI41_TX_COMPLETE_ACC_INTV_NUM));
}

static int __init_acc_channel(PAL_Handle pal_hnd, int chan_num, Cppi4Queue queue, PAL_Cppi4AccChHnd* acc_hnd)
{
    Cppi4AccumulatorCfg cfg;
    unsigned int accListSize;

    *acc_hnd = NULL;

    cfg.accChanNum             = chan_num;
    cfg.list.maxPageEntry      = PAL_CPPI41_ACC_MAX_PAGE_ENTRIES;   /* This is entries per page (and we have 2 pages) */
    cfg.list.listEntrySize     = PAL_CPPI41_ACC_ENTRY_TYPE_D;   /* Only interested in register 'D' which has the desc pointer */
    cfg.list.listCountMode     = PAL_CPPI41_ACC_LIST_NULL_TERM;     /* Zero indicates null terminated list. */
    cfg.list.pacingMode        = PAL_CPPI41_ACC_PACE_MODE_LASTINTR; /* Wait for time since last interrupt */
    cfg.pacingTickCnt          = PAL_CPPI41_ACC_PACE_TICK_CNT;      /* Wait for 1000uS == 1ms */
    cfg.list.maxPageCnt        = PAL_CPPI41_ACC_MAX_PAGE_COUNT;     /* Use two pages */
    cfg.list.stallAvoidance    = 1;                             /* Use the stall avoidance feature */
    cfg.queue                  = queue;
    cfg.mode                   = 0;

    accListSize = (cfg.list.maxPageEntry * (cfg.list.listEntrySize + 1)) * cfg.list.maxPageCnt * sizeof(Uint32);
    if(!(cfg.list.listBase = kzalloc(accListSize, GFP_KERNEL)))
    {
        DPRINTK("Unable to allocate list page of size %d\n", accListSize);
        return -1;
    }

    PAL_CPPI4_CACHE_WRITEBACK((unsigned long)cfg.list.listBase, accListSize);

    if(!(*acc_hnd = PAL_cppi4AccChOpen(pal_hnd, &cfg)))
    {
        DPRINTK("Unable to open accumulator channel #%d\n", chan_num);
        kfree(cfg.list.listBase);
        return -1;
    }

    return 0;
}

irqreturn_t tx_complete_interrupt(int irq, void *dev)
{
    /* Since the INTD interrupts are level, need to disable the IRQ in order to run the tasklet */
    disable_irq_nosync(MAP_INTD_TO_INTC(PAL_CPPI41_TX_COMPLETE_ACC_INTV_NUM));
#if defined(TX_COMPLETE_USE_TASKLET)
    tasklet_schedule(&gTxCompleteTasklet);
#elif defined(TX_COMPLETE_USE_WORKQ)
    queue_work(tx_wq, &(tx_work.work));
#endif

    return IRQ_RETVAL(1);
}



static Int32 __setup_txcomplete(PAL_Handle palHnd)
{
    Cppi4Queue  txCmplQ;
    Cppi4Queue  fdHostQ;
    Uint8       priority;

    for (priority = 0; priority < PAL_CPPI41_SR_HOST_TO_PP_Q_COUNT; priority++)
    {
        /************************************************/
        /* reset Tx complete queue                      */
        /************************************************/
        txCmplQ.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        txCmplQ.qNum = PAL_CPPI41_SR_HOST_TX_COMPLETE_Q_NUM(priority);
        PAL_cppi4QueueClose(palHnd, PAL_cppi4QueueOpen(palHnd, txCmplQ));

        fdHostQ.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        fdHostQ.qNum = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_Q_NUM(priority);

        if(!(gHost2ppFreeHostDescQueueHnd[priority] = PAL_cppi4QueueOpen(palHnd, fdHostQ)))
        {
            printk("unable to open FD Host Queue #%d for TX Complete task\n", fdHostQ.qNum);
            return -1;
        }

        /************************************************/
        /* Init the Tx complete accumulator channel     */
        /************************************************/
        if(__init_acc_channel(palHnd, PAL_CPPI41_TX_COMPLETE_ACC_CH_NUM(priority), txCmplQ, &gTxCompleteAccChHnd[priority]))
        {
            printk("unable to open accumulator channel #%d for TX Complete task\n", PAL_CPPI41_TX_COMPLETE_ACC_CH_NUM(priority));
            return -1;
        }

        gTxCompleteAccListBase[priority] = gTxCompleteAccList[priority] = PAL_cppi4AccChGetNextList(gTxCompleteAccChHnd[priority]);

        /* request the Tx Complete IRQs - one IRQ per all TX complete priorities */
        if (priority == PAL_CPPI4x_PRTY_LOW)
        {
#if defined (TX_COMPLETE_USE_TASKLET)        
            tasklet_init(&gTxCompleteTasklet, __do_tx_complete, 0);
#elif defined(TX_COMPLETE_USE_WORKQ)
            tx_wq = create_workqueue("tx_wq"); 
            printk("**** ---- >>>> __setup_txcomplete  - create_workqueue\n");
	    
            if (tx_wq == NULL)
            {
                printk("Failed to create l2sw_rx_wq\n");
                return -ENOMEM;
            }               
            /* Init the work */
            INIT_WORK(&(tx_work.work), __do_tx_complete);
#endif

            if(request_irq(MAP_INTD_TO_INTC(PAL_CPPI41_TX_COMPLETE_ACC_INTV_NUM), tx_complete_interrupt, IRQF_DISABLED, "TX Complete", NULL))
            {
                printk("unable to get IRQ #%d for TX Complete task\n", MAP_INTD_TO_INTC(PAL_CPPI41_TX_COMPLETE_ACC_INTV_NUM));
                return -1;
            }
        }
    }

    return 0;
}

Uint8 pp_test_pktData[1518] = // Set with default packet. If pktSize > length(pktString) then it will be padded by this packet content
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
    printk("send pktString[hex byte string with : as delimiter] pktSize[64-1518,pktSize>=length(pktString)] pktNum srcPort[0-%d] dstQueue[0-%d] recyclePrxQ[0,1]\n", PAL_CPPI41_MAX_SOURCE_PORTS-1, PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT);
}

static int pp_test(struct file *file, const char *buffer, unsigned long count, void *data)
{
    char*   proc_cmd;
    char*   argv[10];
    int     argc = 0;
    char*   ptr_cmd;
    char*   delimitters = " \n\t";
    char*   ptr_next_tok;

    Uint32 pktSize;
    Uint32 pktNum, pktNumIndex;
    Uint32 srcPort;
    Uint32 dstQueue;
    Uint32 doRecycle;
    Uint32 pktStringLength;
    Uint32 tmp;
    PAL_Handle hnd = PAL_cppi4Init(NULL, NULL);
    Cppi4Queue queue;
    PAL_Cppi4QueueHnd popQueueHnd, dstQueueHnd;
    PAL_Cppi4QueueHnd prxQHnd;
    PAL_Cppi4QueueHnd recycleQHnd;
    Cppi4EmbdDescPp* desc = NULL;
    Ptr dataBuffer;
    char *c;
    const char * sep = ":";

    proc_cmd = kmalloc(count, GFP_KERNEL);
    if (proc_cmd == NULL)
    {
        printk("Could not allocate %d bytes for proc_cmd\n", (int)count);
        return -ENOMEM;
    }

    /* Initialize the buffer before using it. */
    memset ((void *)proc_cmd, 0, count);
    memset ((void *)&argv[0], 0, sizeof(argv));

    /* Copy from user space. */
    if (copy_from_user(proc_cmd, buffer, count))
    {
        pp_test_usage();
        kfree(proc_cmd);
        return -EFAULT;
    }

    ptr_next_tok = proc_cmd;

    /* Tokenize the command. Check if there was a NULL entry. If so be the case the user did not know how to use the entry. Print the help screen */
    ptr_cmd = strsep(&ptr_next_tok, delimitters);
    if (ptr_cmd == NULL)
    {
        pp_test_usage();
        kfree(proc_cmd);
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
            kfree(proc_cmd);
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
            kfree(proc_cmd);
            return -EFAULT;
        }

        if (7 != argc)
        {
            pp_test_usage();
            kfree(proc_cmd);
            return -EFAULT;
        }

        pktSize   = (int) simple_strtol(argv[2], NULL, 0);
        pktNum    = (int) simple_strtol(argv[3], NULL, 0);
        srcPort   = (int) simple_strtol(argv[4], NULL, 0);
        dstQueue  = (int) simple_strtol(argv[5], NULL, 0);
        doRecycle = (int) simple_strtol(argv[6], NULL, 0);

        pktStringLength = 0;
        while ((c = strsep(&argv[1], sep)))
        {
            pp_test_pktData[pktStringLength++] = simple_strtol(c, NULL, 16);

            if (pktStringLength >= 1518)
            {
                pp_test_usage();
                kfree(proc_cmd);
                return -EFAULT;
            }
        }

        if (pktStringLength > pktSize || pktSize < 64 || pktSize > 1518 || srcPort >= PAL_CPPI41_MAX_SOURCE_PORTS || dstQueue >= PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT || doRecycle > 1)
        {
            pp_test_usage();
            kfree(proc_cmd);
            return -EFAULT;
        }

        queue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        queue.qNum = PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM;
        popQueueHnd = PAL_cppi4QueueOpen(hnd, queue);

        queue.qNum = dstQueue;
        dstQueueHnd = PAL_cppi4QueueOpen(hnd, queue);

        for (pktNumIndex = 0; pktNumIndex < pktNum; pktNumIndex++)
        {
            /* Get descriptor from free queue */


            desc = (Cppi4EmbdDescPp*)PAL_cppi4QueuePop(popQueueHnd);
            if (!desc)
            {
                printk("Pop descriptor from queue PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM[#%d] failed\n", PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM);
                kfree(proc_cmd);
                return -EFAULT;
            }
            desc = (Cppi4EmbdDescPp*)PAL_CPPI4_PHYS_2_VIRT(desc);

            /* Update Descriptor Info */
            desc->descInfo = 0x400000 + pktSize;
            desc->tagInfo = (srcPort << 27) | 0x3FFF;

            /* Allocate buffer, set it and put in descriptor */
            {
                /* Get Buffer from pool 5 */
                dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL5});
                while (!dataBuffer)
                {
                    dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI41_QUEUE_MGR_PARTITION_SR, PAL_CPPI41_BMGR_POOL5});
                }
                dataBuffer = PAL_CPPI4_PHYS_2_VIRT(dataBuffer);

                /* Update Buffer Data */
                for (tmp = 0; (tmp < pktSize) && (tmp < BMGR0_POOL05_BUF_SIZE); tmp+=4)
                {
                    *(volatile Uint32*)((Uint8*)dataBuffer + tmp) = (pp_test_pktData[tmp] << 24) | (pp_test_pktData[tmp+1] << 16) | (pp_test_pktData[tmp+2] << 8) | (pp_test_pktData[tmp+3] << 0);
                }

                /* Update buffer in descritptor */
                desc->Buf.BufInfo = 0x80000000 + (PAL_CPPI41_BMGR_POOL5 << 24) + tmp;    // Slot 1 buffer info
                desc->Buf.BufPtr = PAL_CPPI4_VIRT_2_PHYS(dataBuffer);                           // Slot 1 buffer pointer
                desc->pktInfo = 0x1C108000 + PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM;

                PAL_CPPI4_CACHE_WRITEBACK((unsigned long)dataBuffer, tmp);
            }

            PAL_CPPI4_CACHE_WRITEBACK((unsigned long)desc, 64);

            /* Push the packet to the dstQueue */
            PAL_cppi4QueuePush(dstQueueHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)desc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(64), pktSize);
        }

        if (doRecycle)
        {
            /* Clear PrxPDSP queues */
            queue.qNum = PAL_CPPI41_RECYCLE_INFRA_INPUT_LOW_Q_NUM;
            recycleQHnd = PAL_cppi4QueueOpen(hnd, queue);

            queue.qNum = PAL_CPPI41_SR_PrxPDSP_Q_BASE;
            prxQHnd = PAL_cppi4QueueOpen(hnd, queue);
            do
            {
                desc = (Cppi4EmbdDescPp*)PAL_cppi4QueuePop(prxQHnd);
                if (desc != NULL)
                {
                    PAL_cppi4QueuePush(recycleQHnd, (Ptr)desc, PAL_CPPI4_DESCSIZE_2_QMGRSIZE(64), 0);
                }
            }
            while (desc != NULL);
        }
    }
    else
    {
        pp_test_usage();
        kfree(proc_cmd);
        return -EFAULT;
    }

    kfree(proc_cmd);
    return count;
}


int avalanche_pp_cppi_init(void)
{
    int i;
    Cppi4Queue tmpQ;    /* Just used for filling Q info for opening */
    Cppi4BufPool tmpBufPool; /* Used for Init calls */
    Uint32 pRcbMem;
    PAL_Handle hnd, preFQHnd;

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

    for (i = PAL_CPPI41_BMGR_POOL0; i < PAL_CPPI41_BMGR_MAX_POOLS; i++)
    {
        if (ppBuffPoolInfo[i].buffCount != 0)
        {
            tmpBufPool.bPool = i;
            if ((PAL_cppi4BufPoolInit(hnd, tmpBufPool, ppBuffPoolInfo[i].refCount, ppBuffPoolInfo[i].buffSize, ppBuffPoolInfo[i].buffCount)) == NULL)
            {
                printk ("PAL_cppi4BufPoolInit for pool %d FAILED.\n", tmpBufPool.bPool);
                return -1;
            }
        }
    }
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
        Cppi4EmbdDescPp* currBD;
        PAL_Cppi4QueueHnd tmpQHnd;
        int bd_cnt;

        currBD = (Cppi4EmbdDescPp*)ppEmbBDCfg_g.buffDescRegionPtr;
        for (i = 0; i < ppEmbBDCfg_g.numBlks; i++)
        {
            BDBlkInfo* BDBlk = &ppEmbBDCfg_g.BDBlk[i];

            tmpQ.qMgr = ppEmbBDCfg_g.qMgr;
            tmpQ.qNum = BDBlk->qNum;
            tmpQHnd = PAL_cppi4QueueOpen (hnd, tmpQ);

            for (bd_cnt = 0; bd_cnt < BDBlk->numDesc; bd_cnt++)
            {
                PAL_osMemSet(currBD, 0, ppEmbBDCfg_g.szDesc);

                currBD->descInfo    = CPPI41_EM_DESCINFO_DTYPE_EMBEDDED | CPPI41_EM_DESCINFO_SLOTCNT_PP;
                currBD->tagInfo     = 0x3FFF;
                currBD->pktInfo     =
                                     (BDBlk->pktType    << CPPI41_EM_PKTINFO_PKTTYPE_SHIFT)
                                    |(1                 << CPPI41_EM_PKTINFO_RETPOLICY_SHIFT)
                                    |(1                 << CPPI41_EM_PKTINFO_PROTSPEC_SHIFT)
                                    |(ppEmbBDCfg_g.qMgr << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)
                                    |(BDBlk->qNum       << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT);

                PAL_CPPI4_CACHE_WRITEBACK(currBD, ppEmbBDCfg_g.szDesc);

                PAL_cppi4QueuePush (tmpQHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)currBD), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(ppEmbBDCfg_g.szDesc), 0);

                currBD = (Cppi4EmbdDescPp*)((Uint32)currBD + ppEmbBDCfg_g.szDesc);
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

                PAL_cppi4QueuePush (tmpQHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)currBD), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(ppHostBDCfg_g.szDesc), 0);

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
    /* Open Tx Qs. These will be used by drivers to push for TX Infra Qs */
    tmpQ.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    for (i = 0; PAL_CPPI41_SR_HOST_TO_PP_INFRA_DMA_CH_COUNT > i; i++)
    {
        /* Input Queue */
        tmpQ.qNum   = PAL_CPPI41_SR_HOST_TO_PP_INFRA_INPUT_Q_NUM(i);
        PAL_cppi4QueueOpen (hnd, tmpQ);

        /* Output Queue */
        tmpQ.qNum   = PAL_CPPI41_SR_HOST_TO_QPDSP_EMB_TYPE_Q_NUM(i);
        PAL_cppi4QueueOpen (hnd, tmpQ);

        /* Infrastructure Channels */
        __setup_Host2pp_Infra(hnd, i);
    }

    tmpQ.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
    tmpQ.qNum   = PAL_CPPI41_RECYCLE_INFRA_INPUT_LOW_Q_NUM;
    PAL_cppi4QueueOpen(hnd, tmpQ);
    /* This is done in order to keep the reference count of these queues non zero so they will not be closed */
    __setup_Recycle_Infra(hnd);

    /*****************************************************************
     ******************* Setup Prefetcher ***************************
     ****************************************************************/

    /* Push Prefetcher free desc */
    pRcbMem = (Uint32)prefCfg_g.pfDescBase;
    preFQHnd = PAL_cppi4QueueOpen (hnd, prefCfg_g.pfFQ);
    for(i = 0; i < prefCfg_g.pfDescCnt; i++)
    {
        PAL_cppi4QueuePush (preFQHnd, (Ptr) pRcbMem, 0, 0);
        pRcbMem += PAL_CPPI41_SR_PPDSP_PREFETCH_DESC_FD_DESC_SIZE;
    }

    return 0;
}


static const struct file_operations pp_test_proc_fops = {
    .write = pp_test
};

int avalanche_pp_kernel_post_init (void)
{
    PAL_Handle hnd = PAL_cppi4Init (NULL, NULL);

    /*********** Prefetcher setup done **************************/
    if (__setup_txcomplete(hnd))
    {
        printk("%s(%d): Error - setup_txcomplete failed!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    {
        struct proc_dir_entry *pp_test_proc = proc_create("pp_test" ,0644, init_net.proc_net, &pp_test_proc_fops);

        if(!pp_test_proc)
        {
            printk("%s(%d): Error - pp_test proc creation failed\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }

    return 0;
}
EXPORT_SYMBOL(avalanche_pp_kernel_post_init);


subsys_initcall(avalanche_pp_cppi_init);



