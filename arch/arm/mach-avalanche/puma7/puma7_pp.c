/*
 * puma7_pp.c
 *  Description:
 * Puma-7 Packet Processor initialization.
 * Contains Puma-7 specific initialization. The numbers (addresses etc) are
 * Puma-7 specific. The static structures are filled in with Puma-6 specific
 * data and the generic PPD init function gets called in the end
 * with this data.
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2017 Intel Corporation.
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
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs_macros.h>
#include <ti_ppd.h>
#include <puma7_pp.h>
#include <puma7_cppi.h>
#include <linux/mrpc/handshake_server.h>


typedef struct
{
    Uint32  numDesc;
    Uint32  qNum;

    Ptr     firstDescPtr;
    Uint32  isAllocated;

    /* Desc config for the  group */
    Uint32 pktType;
} BDBlkInfo;


#define READ_PP_REG_IS_BIT_SET(addr, bit_mask)     ((*((volatile unsigned int *)(addr)) & bit_mask) !=0 )


#ifdef P6_PP_DEBUG
/* note: prints function name for you */
#  define DPRINTK(fmt, args...) printk("%s: " fmt, __FUNCTION__ , ## args)
#else
#  define DPRINTK(fmt, args...)
#endif

#ifdef CONFIG_ARM_AVALANCHE_SOC
/* interrupts NPCPU*/
#define DWC_REQUEST_IRQ(irq, handler, flags, name, dev) \
                                    request_irq(irq, handler, flags, name, dev)
#define DWC_FREE_IRQ(irq, dev)      free_irq(irq, dev)
#define DWC_ENABLE_IRQ(irq)         enable_irq(irq)
#define DWC_DISABLE_IRQ(irq)        disable_irq(irq)
#define DWC_DISABLE_IRQ_NOSYC(irq)  disable_irq_nosync(irq)
#define DWC_ACK_IRQ(irq)            ack_irq(irq)

#else

/* interrupts APPCPU */
#define DWC_REQUEST_IRQ(irq, handler, flags, name, dev) \
                                    netss_request_npcpu_irq(irq, name, handler, dev)
#define DWC_FREE_IRQ(irq, dev)      do {} while(0) /* TODO: need to implement free_irq for netss module */
#define DWC_ENABLE_IRQ(irq)         avalanche_intc_enable_irq(irq)
#define DWC_DISABLE_IRQ(irq)        avalanche_intc_disable_irq(irq)
#define DWC_DISABLE_IRQ_NOSYC(irq)  avalanche_intc_disable_irq(irq)
#define DWC_ACK_IRQ(irq)            avalanche_intc_clear_status(irq)
#endif

#define ACK_IRQ_TX_COMPLETE() (DWC_ACK_IRQ(MAP_INTD_TO_INTC(PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_INTV_NUM)))
#define ENABLE_IRQ_TX_COMPLETE() (DWC_ENABLE_IRQ(MAP_INTD_TO_INTC(PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_INTV_NUM)))
#define DISABLE_IRQ_NOSYNC_TX_COMPLETE() (DWC_DISABLE_IRQ_NOSYC(MAP_INTD_TO_INTC(PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_INTV_NUM)))
#define INTD_WRITE_EOI_TX_COMPLETE() (avalanche_intd_write_eoi(PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_INTV_NUM))

#define PAL_CPPI41_ACC_MAX_PAGE_ENTRIES                32
#define PAL_CPPI41_ACC_LIST_NULL_TERM                  0
#define PAL_CPPI41_ACC_PACE_MODE_LASTINTR              1
#define PAL_CPPI41_ACC_PACE_TICK_CNT                   40
#define PAL_CPPI41_ACC_MAX_PAGE_COUNT                  2
#define NETDEV_TX_SERVICE_MAX                          ((PAL_CPPI41_ACC_MAX_PAGE_ENTRIES - 1) * 2)

//#define TX_COMPLETE_NETDEV_USE_TASKLET
#define TX_COMPLETE_NETDEV_USE_NAPI
#if defined(TX_COMPLETE_NETDEV_USE_NAPI)
    struct napi_struct      gTxCompleteNapi;
    struct net_device       dummyDev;
    static int              netdev_tx_poll(struct napi_struct *napi , int budget);
#elif defined(TX_COMPLETE_NETDEV_USE_TASKLET)
    struct tasklet_struct   gTxCompleteTasklet;     /* Tx completion processing tasklet */
#endif

PAL_Cppi4AccChHnd       gTxCompleteAccChHnd[PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_COUNT];
Ptr                     gTxCompleteAccListBase[PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_COUNT];
Cppi4HostDescLinux**    gTxCompleteAccList[PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_COUNT];

PAL_Cppi4QueueHnd       gHost2ppFreeHostDescQueueHnd[PAL_CPPI_PP_HOST2PP_INFRA_DMA_CH_COUNT];

#ifndef CONFIG_ARM_AVALANCHE_SOC
static inline int get_list_entry_count(int priority)
{
    int list_entry_count;
    unsigned long long timeout = 1<<31; /* long timeout */

            do {
                PAL_CPPI4_CACHE_INVALIDATE(gTxCompleteAccList[priority], sizeof(int *));
                list_entry_count = be32_to_cpu((unsigned long)*gTxCompleteAccList[priority]);
            pr_debug("%s:%d: list_entry_count %x\n", __func__, __LINE__, list_entry_count);
    } while(!list_entry_count && --timeout);

    BUG_ON(!timeout);

            *gTxCompleteAccList[priority] = NULL;
            PAL_CPPI4_CACHE_INVALIDATE(gTxCompleteAccList[priority], sizeof(int *));
            gTxCompleteAccList[priority]++;
    return list_entry_count;
}
#endif

#if defined(TX_COMPLETE_NETDEV_USE_NAPI)
static int  __do_tx_complete(struct net_device* dev, int budget)
#elif defined(TX_COMPLETE_NETDEV_USE_TASKLET)
static void __do_tx_complete(unsigned long data)
#else
#error "Please choose packet processing framework"
#endif // TX_COMPLETE_NETDEV_USE_NAPI
{
    Cppi4HostDescLinux* hostDesc;
    Uint32      packets_processed = 0;
    Int32       priority;

    /* Start with high priority channel */
	for (priority = PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_COUNT - 1; priority >= 0; priority--) {
        /* While there are ready pages... */
		while (avalanche_intd_get_interrupt_count(0, PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_NUM(priority)) &&
                       (packets_processed <= NETDEV_TX_SERVICE_MAX)) {
#ifndef CONFIG_ARM_AVALANCHE_SOC
        int list_entry_count = get_list_entry_count(priority);

        BUG_ON(list_entry_count > PAL_CPPI41_ACC_MAX_PAGE_ENTRIES);

            while(list_entry_count--) {
                do {
                    PAL_CPPI4_CACHE_INVALIDATE(gTxCompleteAccList[priority], sizeof(int *));
                    hostDesc = (Cppi4HostDescLinux*)(be32_to_cpu((unsigned long)*gTxCompleteAccList[priority]) & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);
                    pr_debug("%s:%d: hostDesc 0x%x\n", __func__, __LINE__, (unsigned long)hostDesc);
                } while (!hostDesc);

                *gTxCompleteAccList[priority] = NULL;
                PAL_CPPI4_CACHE_INVALIDATE(gTxCompleteAccList[priority], sizeof(int *));

#else
            /* While there are descriptors in the page... */
            while((hostDesc = (Cppi4HostDescLinux*)(be32_to_cpu((unsigned long)*gTxCompleteAccList[priority]) & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK))) {

#endif
                hostDesc = PAL_CPPI4_PHYS_2_VIRT(hostDesc);
                PAL_CPPI4_CACHE_INVALIDATE(hostDesc, PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE);

                dev_kfree_skb_any(be32_to_cpu(hostDesc->skb));
                hostDesc->skb = NULL;

                /* Queue back the hostDesc to free pool */
                PAL_cppi4QueuePush(gHost2ppFreeHostDescQueueHnd[priority], (Ptr)PAL_CPPI4_VIRT_2_PHYS(hostDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE), 0);

                packets_processed++;
                gTxCompleteAccList[priority]++;
            }

            /* Update the list entry for next time */
            gTxCompleteAccList[priority] = PAL_cppi4AccChGetNextList(gTxCompleteAccChHnd[priority]);

            /* Decrement number of pages by 1 */
            avalanche_intd_set_interrupt_count(0, PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_NUM(priority), 1);

#if defined(TX_COMPLETE_NETDEV_USE_NAPI)
            /* thats it, we did enough. Jump out now! */
            if (packets_processed >= budget)
            {
                return packets_processed;
            }
#endif // TX_COMPLETE_NETDEV_USE_NAPI
        }
    }
#if defined(TX_COMPLETE_NETDEV_USE_TASKLET)
    /* First clear the IRQ in order not to get a false interrupt since INTD is level */
    ACK_IRQ_TX_COMPLETE();

    /* Send INTD EOI */
    INTD_WRITE_EOI_TX_COMPLETE();

    /* It could be that between INTD count decrement and EOI the accumulator will issue another interrupt.
       The logic of INTD is such that level will remain active high even after EOI is set, so INTC will
       lose the interrupt after ack_irq is done (it now expects INTD polarity change).
       Therefore we must check INTD count and if it is not 0 - reschedule the tasklet */
	for (priority = PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_COUNT - 1; priority >= 0; priority--) {
		if (avalanche_intd_get_interrupt_count(0, PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_NUM(priority))) {
            tasklet_schedule(&gTxCompleteTasklet);
            return;
        }
	}

    ENABLE_IRQ_TX_COMPLETE();

#endif
#if defined(TX_COMPLETE_NETDEV_USE_NAPI)
    return packets_processed;
#endif
}

static int __init_acc_channel(PAL_Handle pal_hnd, int chan_num, Cppi4Queue queue, PAL_Cppi4AccChHnd* acc_hnd)
{
    Cppi4AccumulatorCfg cfg;
    unsigned int accListSize;

    *acc_hnd = NULL;

    cfg.accChanNum             = chan_num;
    cfg.list.maxPageEntry      = PAL_CPPI41_ACC_MAX_PAGE_ENTRIES;   /* This is entries per page (and we have 2 pages) */
    cfg.list.listEntrySize     = PAL_CPPI41_ACC_ENTRY_TYPE_D;   /* Only interested in register 'D' which has the desc pointer */
#ifndef CONFIG_ARM_AVALANCHE_SOC
    cfg.list.listCountMode     = PAL_CPPI41_ACC_PACE_MODE_LASTINTR;    /* One indicates Entry Count Mode */
#else
    cfg.list.listCountMode     = PAL_CPPI41_ACC_LIST_NULL_TERM;        /* Zero indicates null terminated list. */
#endif
    cfg.list.pacingMode        = PAL_CPPI41_ACC_PACE_MODE_LASTINTR; /* Wait for time since last interrupt */
    cfg.pacingTickCnt          = PAL_CPPI41_ACC_PACE_TICK_CNT;      /* Wait for 1000uS == 1ms */
    cfg.list.maxPageCnt        = PAL_CPPI41_ACC_MAX_PAGE_COUNT;     /* Use two pages */
    cfg.list.stallAvoidance    = 1;                             /* Use the stall avoidance feature */
    cfg.queue                  = queue;
    cfg.mode                   = 0;

    accListSize = (cfg.list.maxPageEntry * (cfg.list.listEntrySize + 1)) * cfg.list.maxPageCnt * sizeof(Uint32);
	if (!(cfg.list.listBase = kzalloc(accListSize, GFP_KERNEL))) {
		pr_err("Unable to allocate list page of size %d\n", accListSize);
        return -1;
    }

    PAL_CPPI4_CACHE_WRITEBACK((unsigned long)cfg.list.listBase, accListSize);

	if (!(*acc_hnd = PAL_cppi4AccChOpen(pal_hnd, &cfg))) {
		pr_err("Unable to open accumulator channel #%d\n", chan_num);
        kfree(cfg.list.listBase);
        return -1;
    }

    return 0;
}

irqreturn_t tx_complete_interrupt(int irq, void *dev)
{
    DISABLE_IRQ_NOSYNC_TX_COMPLETE();

#if defined(TX_COMPLETE_NETDEV_USE_NAPI)
    napi_schedule(&gTxCompleteNapi);
#elif defined(TX_COMPLETE_NETDEV_USE_TASKLET)
    tasklet_schedule(&gTxCompleteTasklet);
#endif

    return IRQ_RETVAL(1);
}

static Int32 __setup_txcomplete(PAL_Handle palHnd)
{
    Cppi4Queue  txCmplQ;
    Cppi4Queue  fdHostQ;
    Uint8       priority;

	for (priority = 0; priority < PAL_CPPI_PP_HOST2PP_INFRA_DMA_CH_COUNT; priority++) {
        /************************************************/
        /* reset Tx complete queue                      */
        /************************************************/
        txCmplQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        txCmplQ.qNum = PAL_CPPI_PP_HOST2PP_TX_COMPLETE_Q_NUM(priority);
        PAL_cppi4QueueClose(palHnd, PAL_cppi4QueueOpen(palHnd, txCmplQ));

        fdHostQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        fdHostQ.qNum = PAL_CPPI_PP_HOST2PP_HOST_FD_Q_NUM(priority);

		if (!(gHost2ppFreeHostDescQueueHnd[priority] = PAL_cppi4QueueOpen(palHnd, fdHostQ))) {
			pr_err("unable to open FD Host Queue #%d for TX Complete task\n", fdHostQ.qNum);
            return -1;
        }

        /************************************************/
        /* Init the Tx complete accumulator channel     */
        /************************************************/
        if (__init_acc_channel(palHnd, PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_NUM(priority), txCmplQ, &gTxCompleteAccChHnd[priority])) {
			pr_err("unable to open accumulator channel #%d for TX Complete task\n", PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_NUM(priority));
            return -1;
        }

        gTxCompleteAccListBase[priority] = gTxCompleteAccList[priority] = PAL_cppi4AccChGetNextList(gTxCompleteAccChHnd[priority]);

        /* request the Tx Complete IRQs - one IRQ per all TX complete priorities */
		if (priority == 0) {
#if defined(TX_COMPLETE_NETDEV_USE_NAPI)
            init_dummy_netdev(&dummyDev);
            netif_napi_add(&dummyDev, &gTxCompleteNapi, netdev_tx_poll, NETDEV_TX_SERVICE_MAX); 
            napi_enable(&gTxCompleteNapi); 
#elif defined(TX_COMPLETE_NETDEV_USE_TASKLET)
            tasklet_init(&gTxCompleteTasklet, __do_tx_complete, 0);
#endif
            if (DWC_REQUEST_IRQ(MAP_INTD_TO_INTC(PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_INTV_NUM), tx_complete_interrupt, IRQF_DISABLED, "TX Complete", NULL))
            {
                pr_err("unable to get IRQ #%d for TX Complete task\n", MAP_INTD_TO_INTC(PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_INTV_NUM));
                return -1;
            }
        }
    }

    return 0;
}

#if defined(TX_COMPLETE_NETDEV_USE_NAPI)
/**************************************************************************/
/*! \fn         netdev_tx_poll
 **************************************************************************
 *
 *
 *  \param[in]  Net Device
 *  \param[in]  Processed packets budget
 *  \return     Number of processed packets
 **************************************************************************/
static int netdev_tx_poll(struct napi_struct *napi , int budget)
{
    int work_done, priority;
    unsigned long flags;

    work_done = __do_tx_complete(NULL, budget);

    if (likely(work_done >= budget))
        return budget;

    /* order is important here. If we do EOI before calling netif_tx_complete, an interrupt
     * can occur just before we take ourselves out of the poll list; we will not
     * schedule NAPI thread on that interrupt, no further Tx interrupts and
     * Tx will stall forever. Scary...
     * */
    napi_complete(napi);

    /* Accumulator looks at INTD counter in order to know if it can issue another interrupt.
       Since we decrement the counter at l2sw_netdev_tx_complete it is possible that accumulator issued another interrupt.
       Due to the fact that interrupt is level and we do not want to get a false interrupt, we clear the INTC at the end of l2sw_netdev_tx_complete.
       Next time INTC will wait for INTD to become active.
       But, since INTD is level there is a possibility that INTD will remain active.
       This can happen if accumulator issues an interrupt before the host sent EOI (this is done in next line of code).
       So, in this case we have INTD status not changed - still active, while INTC now waits for it to become active.
       This can lead to not getting the interrupt forever. This is why we must check if counter>0 and if so re-schedule NAPI.
       We lock the interrupts b4 doing EOI and up until NAPI schedule in order not to get double interrupt in the case that
       an interrupt is really issued between EOI and checking INTD count - we are going to reschedule NAPI anyway... */

    ACK_IRQ_TX_COMPLETE();
    INTD_WRITE_EOI_TX_COMPLETE();

    /* It could be that between INTD count decrement and EOI the accumulator will issue another interrupt.
       The logic of INTD is such that level will remain active high even after EOI is set, so INTC will
       lose the interrupt after ack_irq is done (it now expects INTD polarity change).
       Therefore we must check INTD count and if it is not 0 - reschedule the tasklet */

    for (priority = PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_COUNT - 1; priority >= 0; priority--)
    {
        if (avalanche_intd_get_interrupt_count(0, PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_NUM(priority)))
        {
                napi_schedule(napi);
                return work_done;
        }
    }

    ENABLE_IRQ_TX_COMPLETE();

    return work_done;
}

#endif

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
    printk("\nCommand Usage:\n"
          "send pktString[hex byte string with : as delimiter] pktSize[64-1518, pktSize>=length(pktString)] pktNum srcPort[0-%d] dstQueue[0-%d] recyclePrxQ[0,1]\n",
          PAL_CPPI_PP_MAX_SOURCE_PORTS-1, PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT);
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

        if (pktStringLength > pktSize || pktSize < 64 || pktSize > 1518 || srcPort >= PAL_CPPI_PP_MAX_SOURCE_PORTS || dstQueue >= PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT || doRecycle > 1)
        {
            pp_test_usage();
            kfree(proc_cmd);
            return -EFAULT;
        }

        queue.qMgr = PAL_CPPI_PP_QMGR_G2;
        queue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
        popQueueHnd = PAL_cppi4QueueOpen(hnd, queue);

        queue.qNum = dstQueue;
        dstQueueHnd = PAL_cppi4QueueOpen(hnd, queue);

        for (pktNumIndex = 0; pktNumIndex < pktNum; pktNumIndex++)
        {
            /* Get descriptor from free queue */


            desc = (Cppi4EmbdDescPp*)PAL_cppi4QueuePop(popQueueHnd);
            if (!desc)
            {
                printk("Pop descriptor from queue PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW[#%d] failed\n", queue.qNum);
                kfree(proc_cmd);
                return -EFAULT;
            }
            desc = (Cppi4EmbdDescPp*)PAL_CPPI4_PHYS_2_VIRT(desc);

            /* Update Descriptor Info */
            desc->descInfo = 0x400000 + pktSize;
            desc->tagInfo = (srcPort << 27) | 0x3FFF;

            /* Allocate buffer, set it and put in descriptor */
            {
                /* Get Buffer from pool */
                dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI_PP_BUF_MGR, PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL});
                while (!dataBuffer)
                {
                    dataBuffer = PAL_cppi4BufPopBuf(hnd, (Cppi4BufPool){PAL_CPPI_PP_BUF_MGR, PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL});
                }
                dataBuffer = PAL_CPPI4_PHYS_2_VIRT(dataBuffer);

                /* Update Buffer Data */
                for (tmp = 0; (tmp < pktSize) && (tmp < PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_SIZE); tmp+=4)
                {
                    *(volatile Uint32*)((Uint8*)dataBuffer + tmp) = (pp_test_pktData[tmp] << 24) | (pp_test_pktData[tmp+1] << 16) | (pp_test_pktData[tmp+2] << 8) | (pp_test_pktData[tmp+3] << 0);
                }

                /* Update buffer in descritptor */
                desc->Buf.BufInfo = 0x80000000 + (PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL << 24) + tmp;    // Slot 1 buffer info
                desc->Buf.BufPtr = PAL_CPPI4_VIRT_2_PHYS(dataBuffer);                           // Slot 1 buffer pointer
                desc->pktInfo = 0x1C108000 + (PAL_CPPI_PP_QMGR_G2 << 12) + PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;

                PAL_CPPI4_CACHE_WRITEBACK((unsigned long)dataBuffer, tmp);
            }

            PAL_CPPI4_CACHE_WRITEBACK((unsigned long)desc, 64);

            /* Push the packet to the dstQueue */
            PAL_cppi4QueuePush(dstQueueHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)desc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(64), pktSize);
        }

#if 0
        if (doRecycle)
        {
            /* Clear PrxPDSP queues */
            queue.qMgr = PAL_CPPI_PP_QMGR_G0;
            queue.qNum = PAL_CPPI_PP_QMGR_G0_RECYCLER_LOW_Q_NUM;
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
#endif
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


int pp_hw_status_proc_read(struct seq_file *m, void *v)
{
    seq_printf(m, "PP Clock Control Status:\n");
    seq_printf(m, "DS OFDM0 CDMA Clock ............. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_DS_OFDM0                        )?"Enabled":"Disabled");
    seq_printf(m, "DS OFDM1 CDMA Clock ............. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_DS_OFDM1                        )?"Enabled":"Disabled");
    seq_printf(m, "DOCSIS 3.0 CDMA Clock ........... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_DOCSIS30                        )?"Enabled":"Disabled");
    seq_printf(m, "SGMII RX0 CDMA Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_SGMII_RX0                       )?"Enabled":"Disabled");
    seq_printf(m, "SGMII RX1 CDMA Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_SGMII_RX1                       )?"Enabled":"Disabled");
    seq_printf(m, "SGMII RX2 CDMA Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_SGMII_RX2                       )?"Enabled":"Disabled");
    seq_printf(m, "MOCA CDMA Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_MOCA                            )?"Enabled":"Disabled");
    seq_printf(m, "Atom GBE CDMA Clock ............. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_ATOM_GBE                        )?"Enabled":"Disabled");
    seq_printf(m, "MP2TS CDMA Clock ................ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_MP2TS                           )?"Enabled":"Disabled");
    seq_printf(m, "Atom WiFi CDMA Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_ATOM_WIFI                       )?"Enabled":"Disabled");
    seq_printf(m, "USB3 Infra PP2Atom CDMA Clock ... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_USB_PP2ATOM                     )?"Enabled":"Disabled");
    seq_printf(m, "USB3 Infra Atom2PP CDMA Clock ... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_USB_ATOM2PP                     )?"Enabled":"Disabled");
    seq_printf(m, "US OFDMA0 CDMA Clock ............. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_US_OFDMA0                      )?"Enabled":"Disabled");
    seq_printf(m, "US OFDMA1 CDMA Clock ............. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_US_OFDMA1                      )?"Enabled":"Disabled");
    seq_printf(m, "SGMII TX0 CDMA Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_SGMII_TX0                       )?"Enabled":"Disabled");
    seq_printf(m, "SGMII TX1 CDMA Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_SGMII_TX1                       )?"Enabled":"Disabled");
    seq_printf(m, "SGMII TX2 CDMA Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_SGMII_TX2                       )?"Enabled":"Disabled");
    seq_printf(m, "Coprocessor USI CDMA Clock ...... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_COPROC_USI                      )?"Enabled":"Disabled");
    seq_printf(m, "Coprocessor USO CDMA Clock ...... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_COPROC_USO                      )?"Enabled":"Disabled");
    seq_printf(m, "SGMII3 CDMA Clock ............... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_SGMII3                          )?"Enabled":"Disabled");
    seq_printf(m, "DMAC MPEG TOP Module Clock ...... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_CDMA_CLK_CTRL_REG, AVALANCHE_PP_CDMA_CLK_CTRL_DMAC_MPEG_TOP                   )?"Enabled":"Disabled");
    seq_printf(m, "Classifier1 PDSP0 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_1_0    )?"Enabled":"Disabled");
    seq_printf(m, "Classifier1 PDSP1 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_1_1    )?"Enabled":"Disabled");
    seq_printf(m, "Classifier1 PDSP2 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_1_2    )?"Enabled":"Disabled");
    seq_printf(m, "Classifier1 PDSP3 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_1_3    )?"Enabled":"Disabled");
    seq_printf(m, "Classifier2 PDSP0 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_2_0    )?"Enabled":"Disabled");
    seq_printf(m, "Classifier2 PDSP1 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_2_1    )?"Enabled":"Disabled");
    seq_printf(m, "Classifier2 PDSP2 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_2_2    )?"Enabled":"Disabled");
    seq_printf(m, "Classifier2 PDSP3 Clock ......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_CLASSIFIER_2_3    )?"Enabled":"Disabled");
    seq_printf(m, "Modifier PDSP0 Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_MODIFIER_0        )?"Enabled":"Disabled");
    seq_printf(m, "Modifier PDSP1 Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_MODIFIER_1        )?"Enabled":"Disabled");
    seq_printf(m, "Modifier PDSP2 Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_MODIFIER_2        )?"Enabled":"Disabled");
    seq_printf(m, "Modifier PDSP3 Clock ............ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_MODIFIER_3        )?"Enabled":"Disabled");
    seq_printf(m, "Prefetcher PDSP0 Clock .......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_PREFETCHER_0      )?"Enabled":"Disabled");
    seq_printf(m, "Prefetcher PDSP1 Clock .......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_PREFETCHER_1      )?"Enabled":"Disabled");
    seq_printf(m, "Prefetcher PDSP2 Clock .......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_PREFETCHER_2      )?"Enabled":"Disabled");
    seq_printf(m, "Prefetcher PDSP3 Clock .......... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_PREFETCHER_3      )?"Enabled":"Disabled");
    seq_printf(m, "TDOX PDSP0 Clock ................ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_TDOX              )?"Enabled":"Disabled");
    seq_printf(m, "RESEQ PDSP0 Clock ............... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_RE_SEQ            )?"Enabled":"Disabled");
    seq_printf(m, "Session Cache PDSP Clock ........ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE1, AVALANCHE_PP_MAILBOX_CLK_CTRL_SESSION_CACHE     )?"Enabled":"Disabled");
    seq_printf(m, "AQM PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_AQM               )?"Enabled":"Disabled");
    seq_printf(m, "SEQ PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_SEQUENCER         )?"Enabled":"Disabled");
    seq_printf(m, "SEC PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_CRYPTO_PDSP       )?"Enabled":"Disabled");
    seq_printf(m, "SEC ENGINE Clock ................ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_SEC_ENGINE        )?"Enabled":"Disabled");
    seq_printf(m, "MC PDSP0 Clock .................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_MC                )?"Enabled":"Disabled");
    seq_printf(m, "WIFI1 PDSP0 Clock ............... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_WIFI_RX           )?"Enabled":"Disabled");
    seq_printf(m, "WIFI0 PDSP0 Clock ............... %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_WIFI_TX           )?"Enabled":"Disabled");
    seq_printf(m, "MOCA PDSP0 Clock ................ %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_MOCA              )?"Enabled":"Disabled");
    seq_printf(m, "ACC PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_ACCUMULATOR       )?"Enabled":"Disabled");
    seq_printf(m, "DPDSP2 PDSP0 Clock .............. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_DS_RESEQ          )?"Enabled":"Disabled");
    seq_printf(m, "FCC PDSP1 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_DECRYPT           )?"Enabled":"Disabled");
    seq_printf(m, "FCC PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_ENCRYPT           )?"Enabled":"Disabled");
    seq_printf(m, "RCE PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_RCE               )?"Enabled":"Disabled");
    seq_printf(m, "PRE PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_RECYCLER          )?"Enabled":"Disabled");
    seq_printf(m, "DPI PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_DPI               )?"Enabled":"Disabled");
    seq_printf(m, "QOS PDSP1 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_QOS_1             )?"Enabled":"Disabled");
    seq_printf(m, "QOS PDSP0 Clock ................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_QOS_0             )?"Enabled":"Disabled");
    seq_printf(m, "US PDSP0 Clock .................. %s\n", READ_PP_REG_IS_BIT_SET(AVALANCHE_PP_MAILBOX_CLK_CTRL_REG_STAGE2, AVALANCHE_PP_MAILBOX_CLK_CTRL_US_PROCESSING     )?"Enabled":"Disabled");
}

DECLARE_PROCFS_READ_ENTRY(pp_hw_status, pp_hw_status_proc_read)


int avalanche_pp_cppi_init(void)
{
    PAL_Handle hnd;

    hnd = PAL_cppi4Init(NULL, NULL);

    if (!hnd)
    {
        printk("avalanche_ppd_init: CPPI41 Library NOT initialized.\n");
        return -1;
    }

    return 0;
}


static const struct file_operations pp_test_proc_fops = {
    .write = pp_test
};

int avalanche_pp_kernel_post_init (void)
{
    PAL_Handle hnd = PAL_cppi4Init (NULL, NULL);

    printk(KERN_INFO "PP: Initializing Intel(R) Packet Processor driver\n");

    /*********** Prefetcher setup done **************************/

#ifdef CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL
    if (__setup_txcomplete(hnd))
    {
        printk("%s(%d): Error - setup_txcomplete failed!\n", __FUNCTION__, __LINE__);
        return -1;
    }
#endif

    struct proc_dir_entry *pp_test_proc = proc_create("pp_test" , 0644, init_net.proc_net, &pp_test_proc_fops);

    if (!pp_test_proc)
    {
        printk("%s(%d): Error - pp_test proc creation failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    proc_mkdir("pp", NULL);
    proc_create("pp/hw_status", 0644, NULL, &pp_hw_status_proc_fops);
    printk(KERN_INFO "PP: /proc/pp/hw_status created\n");

    if (hnd)
    {
#ifdef CONFIG_MRPC_HANDSHAKE_SERVER
        boot_status_report(HANDSHAKE_ID_CPPI_POST);
#endif
        return 0;
    }

    return -1;
}
EXPORT_SYMBOL(avalanche_pp_kernel_post_init);


subsys_initcall(avalanche_pp_cppi_init);



