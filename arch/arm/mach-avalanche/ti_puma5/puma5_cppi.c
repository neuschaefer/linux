/*
 *
 * puma5_cppi.c
 * Description:
 * Puma-5 CPPI initialization.
 * Contains Puma-5 specific initialization. The numbers (addresses etc) are
 * Puma-5 specific. The static structures are filled in with Puma-5 specific
 * data and the generic CPPI4.1 init function gets called in the end
 * with this data.
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

#include <asm-arm/arch-avalanche/generic/pal.h>
#include <asm-arm/arch-avalanche/puma5/puma5_cppi.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/* FIXME : uncomment line below to enable debug prints */
//#define DEBUG_PAL(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);

#ifndef DEBUG_PAL
#define DEBUG_PAL(fmt,arg...)
#endif

static PAL_Result puma5_cppi_proc_init (Ptr hnd, Ptr param);

/* This CPPI 4.1 global initialization structure. Contains static initialization
 * information for the whole CPPI subsystem. Details regarding structure fields
 * can be obtained from PAL documentation. Some comments on how the structure
 * can be populated/extended are thrown here as well.
 *
 * *** IMPORTANT *** *** ALWAYS REMEMBER *** *** FOLLOW THESE RULES ***
 *
 * - Keep all base addresses of descriptor regions in ascending order.
 *   Where descriptor is allocated by CPPI from SDRAM, the base here is
 *   specified as zero. Treat the zero as SDRAM base address while ensuring
 *   the ascending order. Keep all desc regiosns meant to be on SDRAM
 *   together.
 *
 * - Sort the desc regions defined on SDRAM (base == 0) in descending order of
 *   desc size. This is required to ensure easy alignment (bit 14 aligned to
 *   bit 15 and so on.)
 *
 * - Apply these rules to each queue manager in the system *separately*.
 *
 * - Read the PAL CPPI documentation (especially the Cppi4InitCfg structure doc)
 *   before starting to modify this structure.
 *
 */

static Cppi4InitCfg cppi4InitCfg_g =
{
    .resetLine              = 0,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].globalCtrlBase     = (Ptr) AVALANCHE_NWSS_DMA0_GBLCFG_BASE,//changed from DMA1 to DMA0
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].chCtrlStatusBase   = (Ptr) AVALANCHE_NWSS_DMA0_CHNCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedCtrlBase      = (Ptr) AVALANCHE_NWSS_DMA0_SCHEDCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedTableBase     = (Ptr) AVALANCHE_NWSS_DMA0_SCHEDTBL_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].tdFQueue.qMgr      = DMA0_CPPI4x_FTD_QMGR,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].tdFQueue.qNum      = DMA0_CPPI4x_FTD_QNUM,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].globalCtrlBase     = (Ptr) AVALANCHE_NWSS_DMA1_GBLCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].chCtrlStatusBase   = (Ptr) AVALANCHE_NWSS_DMA1_CHNCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedCtrlBase      = (Ptr) AVALANCHE_NWSS_DMA1_SCHEDCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedTableBase     = (Ptr) AVALANCHE_NWSS_DMA1_SCHEDTBL_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].tdFQueue.qMgr      = DMA1_CPPI4x_FTD_QMGR,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].tdFQueue.qNum      = DMA1_CPPI4x_FTD_QNUM,


    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueMgrRgnBase        = (Ptr) AVALANCHE_NWSS_QMGR_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descMemRgnBase         = (Ptr) AVALANCHE_NWSS_DESCMEM_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueMgmtRgnBase       = (Ptr) AVALANCHE_NWSS_QMGMT_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueProxyRgnBase      = (Ptr) AVALANCHE_NWSS_QPROXY_0_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueStatusRgnBase     = (Ptr) AVALANCHE_NWSS_QSTATUS_RGN_BASE,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].LinkingRAM0Base        = IO_VIRT2PHY(AVALANCHE_NWSS_PACKET_RAM_BASE),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].LinkingRAM0Size        = 7168,
            /* Max pointers in this region could be 7K (assuming 32-bit pointers). */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].LinkingRAM1Base        = 0,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].base     = (Ptr) 0x01119000,   /* Docsis MAC internal memory */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].szDesc   = 128,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].numDesc  = 32,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].isOnChip = 1,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_NWSS_ONCHIPDESC_BASE ),    /* Onchip */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].szDesc   = PPFW_CPPI4x_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].numDesc  = PPFW_PREFETCH_DESC_COUNT + PPFW_REPLICA_DESC_COUNT,     /* Total BD Count: Prefetcher + Replication */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].isOnChip = 1,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[2].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_NWSS_APDSP_PREFBLK_BASE ), /* Prefetch */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[2].szDesc   = PPFW_CPPI4x_BUF_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[2].numDesc  = PPFW_PREFETCH_BUFF_COUNT,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[2].isOnChip = 1,

    /* Reserve region with following specifics for Embedded */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[3].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[3].szDesc   = 64,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[3].numDesc  = 2496 + PPFW_EVENT_DESC_NUM /* !@@ Use as per PP system req */,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[4].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[4].szDesc   = USB_CPPI4x_USB_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[4].numDesc  = USB_CPPI4x_MAX_USB_DESC,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[5].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[5].szDesc   = CPMAC_CPPI4x_TX_HOST_BD_SIZE, /* sizeof (Cppi4HostDesc) + alignment */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[5].numDesc  = CPMAC_TX_HOST_BD_NUM,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[6].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[6].szDesc   = CPMAC_CPPI4x_RX_HOST_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[6].numDesc  = CPMAC_RX_HOST_BD_NUM,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[7].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[7].szDesc   = CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_SIZE, /* 64 byte sized */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[7].numDesc  = CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_NUM + CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_NUM,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[8].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[8].szDesc   = sizeof(Cppi4TeardownDesc), /* 32 byte sized */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[8].numDesc  = PAL_CPPI41_NUM_TD_DESC * 2,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[9].base    = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[9].szDesc  = CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[9].numDesc = CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[10].base    = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[10].szDesc  = PPFW_HOST_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[10].numDesc = PPFW_HOST_BD_NUM,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[11].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[11].szDesc   = MPEG_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[11].numDesc  = DMAC_MPEG_RX_EMBEDDED_BD_NUM,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[12].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[12].szDesc   = MPEG_ENCAP_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[12].numDesc  = DMAC_MPEG_ENCAP_RX_EMBEDDED_BD_NUM,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[13].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[13].szDesc   = PAL_CPPI41_VOICE_DSP_C55_EMB_BD_SIZE, /* 64 byte sized */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[13].numDesc  = PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].basefdQNum              = PAL_CPPI41_FD_Q_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].basefdbQNum             = PAL_CPPI41_FBD_Q_BASE,


    .bufMgrBase  [PAL_CPPI41_BUF_MGR_PARTITION_SR]                          = (CSL_BufMgr_RegsOvly) AVALANCHE_NWSS_BMGR_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedTable.numEntries  = 2,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedTable.entry =
    {
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPMAC_CPPI4x_RX_DMA_CHNUM(0) ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPMAC_CPPI4x_TX_DMA_CHNUM(0) ),
    },

    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedTable.numEntries  = 39,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedTable.entry =
    {
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_DMA1_RX_ETH2HOST_CHN_NUM ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_DMA1_TX_ETH2HOST_CHN_NUM ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_DMA1_RX_RECYCLING_INFRA_CHN_NUM ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_DMA1_TX_RECYCLING_INFRA_CHN_NUM ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_VOICE_DSP_C55_INFRA_CHN ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_DMA1_TX_VOICE_DSP_C55_INFRA_CHN ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, USB_CPPI4x_RX_DMA_CHNUM(0)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, USB_CPPI4x_TX_DMA_CHNUM(0)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, USB_CPPI4x_RX_DMA_CHNUM(1)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, USB_CPPI4x_TX_DMA_CHNUM(1)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, USB_CPPI4x_RX_DMA_CHNUM(2)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, USB_CPPI4x_TX_DMA_CHNUM(2)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, USB_CPPI4x_RX_DMA_CHNUM(3)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, USB_CPPI4x_TX_DMA_CHNUM(3)  ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_SR_DOCSIS_RX_DMA_CHNUM(0)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_SR_DOCSIS_RX_DMA_CHNUM(1)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_SR_DOCSIS_RX_DMA_CHNUM(2)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, MPEG_CPPI4x_RX_DMA_CHNUM    ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, MPEGOUT_CPPI4x_CHNUM        ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_SR_DOCSIS_TX_DMA_CHNUM(0)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_SR_DOCSIS_TX_DMA_CHNUM(1)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_SR_DOCSIS_TX_DMA_CHNUM(2)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_SR_DOCSIS_TX_DMA_CHNUM(3)  ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_DMA1_RX_DOCSIS_DS_CoP_MGMT_CHN_NUM ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_DMA1_TX_DOCSIS_MGMT_CHN_NUM ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_DMA1_RX_DOCSIS_TX_CoP_CHN_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_DMA1_TX_DOCSIS_TX_CoP_CHN_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_CNI_RX_INFRA_CH_NUM(0)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_CNI_TX_INFRA_CH_NUM(0)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_CNI_RX_INFRA_CH_NUM(1)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_CNI_TX_INFRA_CH_NUM(1)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_CNI_RX_INFRA_CH_NUM(2)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_CNI_TX_INFRA_CH_NUM(2)  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, USB_CPPI4x_USB2HOST_PROXY_CHNUM(0)      ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, USB_CPPI4x_USB2HOST_PROXY_CHNUM(0)      ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_PPFW_RX_INFRA_CHNUM(0)   ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_PPFW_TX_INFRA_CHNUM(0)   ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, CPPI4x_PPFW_RX_INFRA_CHNUM(1)   ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, CPPI4x_PPFW_TX_INFRA_CHNUM(1)   ),
    },
    .apdspInfo.pdspCmdBase      = (Ptr)     AVALANCHE_NWSS_APDSP_CMD_BASE,
	.debugToolBind = puma5_cppi_proc_init,
};

typedef struct FDqueue
{
    PAL_CPPI41_SR_QMGR_QUEUES_e     id;
    unsigned int                    amount;
}
FDqueue_t;

FDqueue_t gFDqueues[] =
{
    {   .id = PPFW_CPPI4x_APDSP_PREBUF_QNUM,	 					.amount = PPFW_PREFETCH_BUFF_COUNT    },
	{   .id = PPFW_CPPI4x_APDSP_FD_QNUM,							.amount = PPFW_PREFETCH_DESC_COUNT,    },
	{   .id = PAL_CPPI41_SR_PP_EVENTS_FD_Q_NUM,  					.amount = PPFW_EVENT_DESC_NUM  },
	{   .id = PPFW_CPPI4x_MPDSP_FD_QNUM,							.amount = PPFW_REPLICA_DESC_COUNT,    },
	{   .id = PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_Q_NUM,			.amount = CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_NUM,    },
	{   .id = PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_Q_NUM,			.amount = CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_NUM,    },
	{   .id = PAL_CPPI41_CNI_RX_INFRA_FD_HOST_LOW,			        .amount = CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_LOW,     },
    {   .id = PAL_CPPI41_CNI_RX_INFRA_FD_HOST_MED,			        .amount = CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_HIGH,     },
    {   .id = PAL_CPPI41_CPMAC_FBD_QNUM,                            .amount = 64  },
    {   .id = DMA0_CPPI4x_FTD_QNUM,                                 .amount = PAL_CPPI41_NUM_TD_DESC                 },
    {   .id = DMA1_CPPI4x_FTD_QNUM,                                 .amount = PAL_CPPI41_NUM_TD_DESC                },
    {   .id = PAL_CPPI41_SR_UNASSIGNED_Q_160,                       .amount = 16  },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_Q_NUM,           .amount = PPFW_TX_HOST_BD_NUM_LOW       },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_MED_FD_HOST_Q_NUM,           .amount = PPFW_TX_HOST_BD_NUM_MED       },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_HIGH_FD_HOST_Q_NUM,          .amount = PPFW_TX_HOST_BD_NUM_HIGH       },
    {   .id = PAL_CPPI4_CPMAC_FD_QNUM ,                             .amount = CPMAC_RX_EMBEDDED_BD_NUM       },
    {   .id = USB_CPPI4x_EP0_FD_QNUM ,                              .amount = 64       },
    {   .id = PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM,         .amount = CPPI4x_SR_DOCSIS_RX_FD_EMB_DESC_NUM_LOW     },
    {   .id = PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_Q_NUM,        .amount = CPPI4x_SR_DOCSIS_RX_FD_EMB_DESC_NUM_HIGH    },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_Q_NUM,      .amount = PPFW_RX_EMBEDDED_BD_NUM_LOW  },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_Q_NUM,     .amount = PPFW_RX_EMBEDDED_BD_NUM_MED },
    {   .id = PAL_CPPI41_SR_MPEG_FD_EMB_Q_NUM,                      .amount = DMAC_MPEG_RX_EMBEDDED_BD_NUM},
	{   .id = PAL_CPPI41_SR_MPEG_ENCAP_FD_EMB_Q_NUM,                .amount = DMAC_MPEG_ENCAP_RX_EMBEDDED_BD_NUM},
    {   .id = PAL_CPPI41_SR_VOICE_DSP_C55_FD_EMB_Q_NUM,             .amount = PAL_CPPI41_SR_VOICE_DSP_VNI_FD_EMB_Q_COUNT},
    {   .id = PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_NUM,               .amount = PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_COUNT},
    
};
/* proc interface  */
static PAL_Handle puma5palHnd;

int puma5_cppi_sr_dump_all_stats(struct seq_file *m, void *v)
{                                                                                                                                                  
    int queue = 0;                                                                                                                                 
    int i;                                                                                                                                         
    unsigned int pktCount;                                                                                                                         
    unsigned int expectedCount;                                                                                                                    
    Cppi4Queue  cppiQueue;                                                                                                                         
                                                                                                                                                   
    for (queue = 0; queue < PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT; queue++)                                                                         
    {                                                                                                                                          
        cppiQueue.qMgr = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;                                                                                    
        cppiQueue.qNum = queue ;                                                                                                               
                                                                                                                                               
        PAL_cppi4Control(puma5palHnd, PAL_CPPI41_IOCTL_GET_QUEUE_ENTRY_COUNT, &cppiQueue, &pktCount);                                        
                                                                                                                                               
        expectedCount = 0;                                                                                                                     
                                                                                                                                               
        for (i=0; i<ARRAY_SIZE(gFDqueues); i++)                                                                                                
        {                                                                                                                                      
            if (gFDqueues[i].id == queue)                                                                                                      
            {                                                                                                                                  
                expectedCount = gFDqueues[i].amount;                                                                                           
                break;                                                                                                                         
            }                                                                                                                                  
        }                                                                                                                                      

        if (pktCount != expectedCount )                                                                                                         
        {                                                                                                                                      
            // TBD: need to revert change of shorter output                                                                                    
            seq_printf (m, "%4d %-27s : %4d [%d]\n", queue, PAL_CPPI41_GET_QNAME(queue), pktCount, expectedCount);
        }                                                                                                                                      
    }                                                                                                                                          
 
    return 0; 
}


static int puma5_cppi_sr_dump_all_stats_open(struct inode *inode, struct file *file)
{
    return single_open(file, puma5_cppi_sr_dump_all_stats, NULL); 
}

static const struct file_operations puma5_cppi_sr_dump_all_stats_ops = {
    .open    = puma5_cppi_sr_dump_all_stats_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};

static PAL_Result puma5_cppi_proc_init (Ptr hnd, Ptr param)                                                                                                            
{                                                                                                                                                                      
    struct proc_dir_entry * dir_1 = (struct proc_dir_entry *)param;                                                                                                    
                                                                                                                                                                       
    puma5palHnd = (PAL_Handle)hnd;
    if (NULL == (dir_1 = proc_mkdir("sr",       dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (dir_1 = proc_mkdir("stats",    dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create( "all" , 0, dir_1, &puma5_cppi_sr_dump_all_stats_ops))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    return (0);
}                                                                                                                                                                      

int avalanche_cppi_init(void)
{
#ifdef CONFIG_INTEL_KERNEL_VOICE_SUPPORT
    int i;
    for (i=0; i<PAL_CPPI41_MAX_DESC_REGIONS; i++)
    {
        if (
            (PAL_CPPI41_VOICE_DSP_C55_EMB_BD_SIZE  == cppi4InitCfg_g.queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[i].szDesc) &&
            (PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT == cppi4InitCfg_g.queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[i].numDesc)
           )
        {
            unsigned int *  poolPhysAddr;

            if ( avalanche_alloc_no_OperSys_memory(eNO_OperSys_VoiceNI, PAL_CPPI41_VOICE_DSP_C55_EMB_BD_SIZE * PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT, (unsigned int *)&poolPhysAddr) != 0)
            {
                printk("PAL_cppi4Init: Alloc API failed!\n");
                return -1;
            }
            cppi4InitCfg_g.queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[i].isPreAllocated = True;
            cppi4InitCfg_g.queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[i].base           = poolPhysAddr;
            break;
        }
    }
#endif

    if(PAL_cppi4Init(&cppi4InitCfg_g, NULL) != NULL)
    {
        printk("PAL_cppi4Init: CPPI 4.1 API initialized successfully.\n");
    }
    else
    {
        printk("PAL_cppi4Init: ERROR: CPPI 4.1 API initialization failed!\n");
    }

    return 0;


}

// Define QUEUEs Names 
PAL_CPPI41_SR_QMGR_QUEUES_STR(qMNGRname);



