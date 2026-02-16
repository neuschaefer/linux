/* 
 GPL LICENSE SUMMARY

 Copyright(c) 2014 Intel Corporation.

 This program is free software; you can redistribute it and/or modify 
 it under the terms of version 2 of the GNU General Public License as
 published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but 
 WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 General Public License for more details.

 You should have received a copy of the GNU General Public License 
 along with this program; if not, write to the Free Software 
 Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 The full GNU General Public License is included in this distribution 
 in the file called LICENSE.GPL.

 Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
*/
/*
 *
 * puma6_cppi.c
 * Description:
 * Puma-6 CPPI initialization.
 * Contains Puma-6 specific initialization. The numbers (addresses etc) are
 * Puma-6 specific. The static structures are filled in with Puma-6 specific
 * data and the generic CPPI4.1 init function gets called in the end
 * with this data.
 *
 */

#include <pal.h>
#include <puma6_cppi.h>
#include <linux/proc_fs.h>
#include <asm-arm/arch-avalanche/generic/sram_api.h>
#include <linux/seq_file.h>

/* FIXME : uncomment line below to enable debug prints */
//#define DEBUG_PAL(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);

#ifndef DEBUG_PAL
#define DEBUG_PAL(fmt,arg...)
#endif

static PAL_Result puma6_cppi_proc_init (Ptr hnd, Ptr param);

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

    .bufMgrBase  [PAL_CPPI41_BUF_MGR_PARTITION_SR]                          = (CSL_BufMgr_RegsOvly) AVALANCHE_NWSS_BMGR_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueMgrRgnBase        = (Ptr) AVALANCHE_NWSS_QMGR_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descMemRgnBase         = (Ptr) AVALANCHE_NWSS_DESCMEM_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueMgmtRgnBase       = (Ptr) AVALANCHE_NWSS_QMGMT_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueProxyRgnBase      = (Ptr) AVALANCHE_NWSS_QPROXY_0_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].queueStatusRgnBase     = (Ptr) AVALANCHE_NWSS_QSTATUS_RGN_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].totalQNum              = PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT,

    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].LinkingRAM0Base        = IO_VIRT2PHY( AVALANCHE_NWSS_PACKET_RAM_BASE ),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].LinkingRAM0Size        = 23040, /* Maximum number of descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].LinkingRAM1Base        = 0,

    /* DOCSIS TX Internal Memory, used for DOCSIS TX CoP Monolithic */ 
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_DOCSIS_SS_COP_US_MONOLITHIC_DESC_RAM_BASE ),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].szDesc   = PAL_CPPI41_SR_DOCSIS_TX_MONOLITHIC_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].numDesc  = 64,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[0].isOnChip = 1,

    /* DOCSIS TX Internal Memory, used for DOCSIS TX CoP Monolithic */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_DOCSIS_SS_US_PACKET_RAM_BASE ),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].szDesc   = PAL_CPPI41_SR_DOCSIS_TX_MONOLITHIC_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].numDesc  = PAL_CPPI41_SR_DOCSIS_TX_MONOLITHIC_DESC_COUNT,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[1].isOnChip = 1,

    /* PP Internal Memory, used for fragmentation and event descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[2].numDesc  = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[2].isOnChip = 1,

    /* PP Internal Memory, used for prefetcher descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[3].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_NWSS_ONCHIPDESC_BASE ),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[3].szDesc   = PAL_CPPI41_SR_PPDSP_PREFETCH_DESC_FD_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[3].numDesc  = PAL_CPPI41_SR_PPDSP_PREFETCH_DESC_FD_DESC_COUNT,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[3].isOnChip = 1,

    /* PP Internal Memory, used for COE Monolithic */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[4].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_NWSS_CoePDSP_MONOLITHIC_DESC_RAM_BASE ),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[4].szDesc   = PAL_CPPI41_SR_COE_MONOLITHIC_FD_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[4].numDesc  = PAL_CPPI41_SR_COE_MONOLITHIC_FD_DESC_COUNT,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[4].isOnChip = 1,

    /* PP Internal Memory, used for COE Forwarding descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[5].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_NWSS_CoePDSP_FORWARDING_DESC_RAM_BASE ),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[5].szDesc   = PAL_CPPI41_SR_COE_FORWARDING_FD_HOST_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[5].numDesc  = PAL_CPPI41_SR_COE_FORWARDING_FD_HOST_DESC_COUNT,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[5].isOnChip = 1,

    /* PP Internal Memory, used for COE Streaming descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[6].base     = (Ptr) IO_VIRT2PHY( AVALANCHE_NWSS_CoePDSP_STREAMING_DESC_RAM_BASE ),
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[6].szDesc   = PAL_CPPI41_SR_COE_STREAMING_FD_HOST_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[6].numDesc  = PAL_CPPI41_SR_COE_STREAMING_FD_HOST_DESC_COUNT,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[6].isOnChip = 1,

    /* Host RX & TX DOCSIS Management Free Host Descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[7].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[7].szDesc   = PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_SIZE, /* 64 byte sized */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[7].numDesc  = PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_COUNT + PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_COUNT,

    /* Group of embedded descriptors: DOCSIS RX CoP, Host2PP Infrastructure & PP Events */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[8].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[8].szDesc   = 64,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[8].numDesc  = PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_DESC_COUNT +
                                                                              PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_DESC_COUNT +
                                                                              PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_DESC_COUNT +
                                                                              PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_DESC_COUNT +
                                                                              PAL_CPPI41_SR_PROXY_PDSP_LOW_FD_EMB_DESC_COUNT +
                                                                              PAL_CPPI41_SR_PROXY_PDSP_HIGH_FD_EMB_DESC_COUNT,

    /* CNI Infrastructure Free Host Descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[9].base    = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[9].szDesc  = PAL_CPPI41_SR_CNI_INFRA_FD_HOST_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[9].numDesc = PAL_CPPI41_SR_CNI_INFRA_FD_HOST_DESC_COUNT, /*128+64*/

    /* Host2PP Free Host Descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[10].base    = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[10].szDesc  = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[10].numDesc = PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_DESC_COUNT, /*128+16*/

    /* L2SW Infrastructure Free Host Descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[11].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[11].szDesc   = PAL_CPPI41_SR_L2SW_INFRA_FD_HOST_DESC_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[11].numDesc  = PAL_CPPI41_SR_L2SW_INFRA_FD_HOST_DESC_COUNT, /*512*/

    /* Teardown descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[12].base    = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[12].szDesc  = sizeof(Cppi4TeardownDesc),    /* 32 byte sized        */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[12].numDesc = PAL_CPPI41_NUM_TD_DESC * 4,   /* Have to serve 4 DMAs */

    /* MPEG ENCAP */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[13].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[13].szDesc   = MPEG_ENCAP_BD_SIZE,                  /*128*/
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[13].numDesc  = DMAC_MPEG_ENCAP_RX_EMBEDDED_BD_NUM,  /*256*/

    /* MPEG */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[14].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[14].szDesc   = MPEG_BD_SIZE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[14].numDesc  = DMAC_MPEG_RX_EMBEDDED_BD_NUM,        /*512*/


    /* VOICE DSP RX Free Embedded Descriptors */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[15].base     = 0,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[15].szDesc   = PAL_CPPI41_VOICE_DSP_C55_EMB_BD_SIZE, /* 64 byte sized */
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].descRegion[15].numDesc  = PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT,


    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].basefdQNum             = PAL_CPPI41_SR_FD_EMB_Q_BASE,
    .queueMgrInfo[PAL_CPPI41_QUEUE_MGR_PARTITION_SR].basefdbQNum            = PAL_CPPI41_SR_FD_HOST_Q_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].globalCtrlBase     = (Ptr) AVALANCHE_NWSS_DMA0_GBLCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].chCtrlStatusBase   = (Ptr) AVALANCHE_NWSS_DMA0_CHNCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedCtrlBase      = (Ptr) AVALANCHE_NWSS_DMA0_SCHEDCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedTableBase     = (Ptr) AVALANCHE_NWSS_DMA0_SCHEDTBL_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].tdFQueue.qMgr      = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].tdFQueue.qNum      = PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].globalCtrlBase     = (Ptr) AVALANCHE_NWSS_DMA1_GBLCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].chCtrlStatusBase   = (Ptr) AVALANCHE_NWSS_DMA1_CHNCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedCtrlBase      = (Ptr) AVALANCHE_NWSS_DMA1_SCHEDCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedTableBase     = (Ptr) AVALANCHE_NWSS_DMA1_SCHEDTBL_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].tdFQueue.qMgr      = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].tdFQueue.qNum      = PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].globalCtrlBase     = (Ptr) AVALANCHE_NWSS_DMA2_GBLCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].chCtrlStatusBase   = (Ptr) AVALANCHE_NWSS_DMA2_CHNCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].schedCtrlBase      = (Ptr) AVALANCHE_NWSS_DMA2_SCHEDCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].schedTableBase     = (Ptr) AVALANCHE_NWSS_DMA2_SCHEDTBL_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].tdFQueue.qMgr      = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].tdFQueue.qNum      = PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].globalCtrlBase     = (Ptr) AVALANCHE_NWSS_DMA3_GBLCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].chCtrlStatusBase   = (Ptr) AVALANCHE_NWSS_DMA3_CHNCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].schedCtrlBase      = (Ptr) AVALANCHE_NWSS_DMA3_SCHEDCFG_BASE,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].schedTableBase     = (Ptr) AVALANCHE_NWSS_DMA3_SCHEDTBL_BASE,

    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].tdFQueue.qMgr      = PAL_CPPI41_QUEUE_MGR_PARTITION_SR,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].tdFQueue.qNum      = PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM,


    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedTable.numEntries  = 9,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK0].schedTable.entry =
    {
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_MGMT_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_COE_DMA01_RX_CH_NUM   ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_256B_NONMC_EVEN_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_256B_NONMC_ODD_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_512B_NONMC_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_2KB_NONMC_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_2KB_MC_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_HIGH_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_VOICE_DMA01_RX_MAX_CH   )
    },

    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedTable.numEntries  = 9,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK1].schedTable.entry =
    {
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_MGMT_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_COE_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_256B_NONMC_EVEN_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_256B_NONMC_ODD_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_512B_NONMC_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_2KB_NONMC_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_LOW_2KB_MC_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_HIGH_DMA01_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_DS_CoP_VOICE_DMA01_RX_MAX_CH   )
    },

    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].schedTable.numEntries  = 15,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK2].schedTable.entry =
    {
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US0_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US1_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US2_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US3_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US4_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US5_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US6_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US7_DMA2_TX_CH_NUM  ),


        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_RX_CH, PAL_CPPI41_SR_DOCSIS_TX_COP_DMA_RX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_COP_DMA_TX_CH_NUM  ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_CNI_LOW_INFRA_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_CNI_HIGH_INFRA_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_L2SW_DATA0_INFRA_DMA2_TX_CH_NUM  ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_L2SW_MGMT0_INFRA_DMA2_TX_CH_NUM  ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_VOICE_DSP_C55_INFRA_CHN ),
    },

    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].schedTable.numEntries  = 5,
    .dmaBlock[PAL_CPPI41_DMA_BLOCK3].schedTable.entry =
    {
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_MPEG_OUT_PROGRAM0_DMA3_TX_CH_NUM ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_MPEG_OUT_PROGRAM1_DMA3_TX_CH_NUM ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_HOST_TO_PP_LOW_INFRA_DMA3_TX_CH_NUM   ),
        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_HOST_TO_PP_HIGH_INFRA_DMA3_TX_CH_NUM   ),

        PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_RECYCLING_INFRA_DMA3_TX_CH_NUM ),
    },
    .apdspInfo.pdspCmdBase      = (Ptr)     AVALANCHE_NWSS_APDSP_CMD_BASE,

    .debugToolBind = puma6_cppi_proc_init,
};



/* proc interface  */
static PAL_Handle puma6palHnd;

typedef struct FDqueue
{
    PAL_CPPI41_SR_QMGR_QUEUES_e     id;
    unsigned int                    amount;
}
FDqueue_t;

FDqueue_t gFDqueues[] =
{
#ifdef CONFIG_CABLE_VFE_COE
    {   .id = PAL_CPPI41_SR_COE_MONOLITHIC_FD_Q_NUM,            .amount = PAL_CPPI41_SR_COE_MONOLITHIC_FD_DESC_COUNT            },
    {   .id = PAL_CPPI41_SR_COE_FORWARDING_FD_HOST_Q_NUM,       .amount = PAL_CPPI41_SR_COE_FORWARDING_FD_HOST_DESC_COUNT       },
    {   .id = PAL_CPPI41_SR_COE_STREAMING_FD_HOST_Q_NUM,        .amount = PAL_CPPI41_SR_COE_STREAMING_FD_HOST_DESC_COUNT        },
#endif
    {   .id = PAL_CPPI41_SR_PPDSP_PREFETCH_DESC_FD_Q_NUM,       .amount = PAL_CPPI41_SR_PPDSP_PREFETCH_DESC_FD_DESC_COUNT       },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_Q_NUM,       .amount = PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_DESC_COUNT       },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_HIGH_FD_HOST_Q_NUM,      .amount = PAL_CPPI41_SR_HOST_TO_PP_HIGH_FD_HOST_DESC_COUNT      },
    {   .id = PAL_CPPI41_SR_L2SW_INFRA_FD_HOST_Q_NUM,           .amount = PAL_CPPI41_SR_L2SW_INFRA_FD_HOST_DESC_COUNT           },
    {   .id = PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM,     .amount = PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_DESC_COUNT     },
    {   .id = PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_Q_NUM,    .amount = PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_DESC_COUNT    },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_Q_NUM,  .amount = PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_DESC_COUNT  },
    {   .id = PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_Q_NUM, .amount = PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_DESC_COUNT },
    {   .id = PAL_CPPI41_SR_PROXY_PDSP_LOW_FD_EMB_Q_NUM,        .amount = PAL_CPPI41_SR_PROXY_PDSP_LOW_FD_EMB_DESC_COUNT        },
    {   .id = PAL_CPPI41_SR_PROXY_PDSP_HIGH_FD_EMB_Q_NUM,       .amount = PAL_CPPI41_SR_PROXY_PDSP_HIGH_FD_EMB_DESC_COUNT       },
    {   .id = PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM,              .amount = PAL_CPPI41_NUM_TD_DESC * 4                            },
    {   .id = PAL_CPPI41_SR_DOCSIS_TX_MONOLITHIC_Q_NUM,         .amount = 80    },
    {   .id = PAL_CPPI41_SR_CNI_INFRA_LOW_FD_HOST_Q_NUM,        .amount = PAL_CPPI41_SR_CNI_INFRA_LOW_FD_HOST_DESC_COUNT        },
    {   .id = PAL_CPPI41_SR_CNI_INFRA_HIGH_FD_HOST_Q_NUM,       .amount = PAL_CPPI41_SR_CNI_INFRA_HIGH_FD_HOST_DESC_COUNT       },
    {   .id = PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_Q_NUM,       .amount = PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_COUNT       },
    {   .id = PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_Q_NUM,       .amount = PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_COUNT       },
    {   .id = PAL_CPPI41_SR_MPEG_FD_EMB_Q_NUM,                  .amount = DMAC_MPEG_RX_EMBEDDED_BD_NUM                          },
    {   .id = PAL_CPPI41_SR_MPEG_ENCAP_FD_EMB_Q_NUM,            .amount = DMAC_MPEG_ENCAP_RX_EMBEDDED_BD_NUM                    },
    {   .id = PAL_CPPI41_SR_VOICE_DSP_C55_FD_EMB_Q_NUM,         .amount = PAL_CPPI41_SR_VOICE_DSP_VNI_FD_EMB_Q_COUNT            },
    {   .id = PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_NUM,           .amount = PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_COUNT              },

};



int puma6_cppi_sr_dump_all_stats(struct seq_file *m, void *v)
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

        PAL_cppi4Control(puma6palHnd, PAL_CPPI41_IOCTL_GET_QUEUE_ENTRY_COUNT,   &cppiQueue, &pktCount);

        expectedCount = 0;

        for (i=0; i<ARRAY_SIZE(gFDqueues); i++)
        {
            if (gFDqueues[i].id == queue)
            {
                expectedCount = gFDqueues[i].amount;
                break;
            }
        }

        if (pktCount != expectedCount)
        {
            // TBD: need to revert change of shorter output
            seq_printf (m, "%4d %-65s : %4d [%d]\n", queue, PAL_CPPI41_GET_QNAME(queue), pktCount, expectedCount);
        }
    }

    return 0;
}

static int puma6_cppi_sr_all_stats_open(struct inode *inode, struct file *file)
{
    return single_open(file, puma6_cppi_sr_dump_all_stats, NULL); 
}

static const struct file_operations puma6_cppi_sr_all_stats_fops = {
    .open           = puma6_cppi_sr_all_stats_open,
    .read           = seq_read,
    .llseek         = seq_lseek,
    .release        = single_release,
};

static PAL_Result puma6_cppi_proc_init (Ptr hnd, Ptr param)
{
    struct proc_dir_entry * dir_1 = (struct proc_dir_entry *)param;

    puma6palHnd = (PAL_Handle)hnd;

    if (NULL == (dir_1 = proc_mkdir("sr",       dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (dir_1 = proc_mkdir("stats",    dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create( "all" , 0, dir_1, &puma6_cppi_sr_all_stats_fops))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }

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

    {
        unsigned int paramVal;

        /* Check silicon stepping */
        SRAM_Get_Boot_Param(SILICON_STEPPING_ID, &paramVal);
        if ( paramVal >= SILICON_STEPPING_ID_C_1)
        {
            int idx;
            int idx_new;
            unsigned char   tmp[ 256 ];

            /* ************************************************************************************************ */
            /*    Remove the upper DOCSIS US channels from the DMA2 list....                                    */
            /*                                                                                                  */
            for ( idx_new=0, idx=0; idx < cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK2 ].schedTable.numEntries; idx++ )
            {
                unsigned char entry = cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK2 ].schedTable.entry[ idx ];

                if ( ( entry < PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US4_DMA2_TX_CH_NUM ) ) &&
                     ( entry > PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US7_DMA2_TX_CH_NUM ) ) )
                {
                    tmp[ idx_new++ ] = entry; 
                }
            }

            for ( idx=0; idx<idx_new; idx++ )
            {
                cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK2 ].schedTable.entry[ idx ] = tmp[ idx ];
            }

            cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK2 ].schedTable.numEntries = idx_new;
            /* ************************************************************************************************* */

            /* ************************************************************************************************ */
            /*    Add the upper DOCSIS US channels to the DMA3 list....                                         */
            /*                                                                                                  */
            idx = cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK3 ].schedTable.numEntries;

            cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK3 ].schedTable.entry[ idx++ ] = PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US4_DMA3_TX_CH_NUM );
            cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK3 ].schedTable.entry[ idx++ ] = PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US5_DMA3_TX_CH_NUM );
            cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK3 ].schedTable.entry[ idx++ ] = PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US6_DMA3_TX_CH_NUM );
            cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK3 ].schedTable.entry[ idx++ ] = PAL_CPPI41_DMA_CH_CONFIG( PAL_CPPI41_DMA_TX_CH, PAL_CPPI41_SR_DOCSIS_TX_US7_DMA3_TX_CH_NUM );

            cppi4InitCfg_g.dmaBlock[ PAL_CPPI41_DMA_BLOCK3 ].schedTable.numEntries = idx;
            /* ************************************************************************************************ */
        }
    }

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




