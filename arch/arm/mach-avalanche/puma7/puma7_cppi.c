/*
 *
 * puma7_cppi.c
 * Description:
 * Puma-7 CPPI initialization.
 * Contains Puma-7 specific initialization. The numbers (addresses etc) are
 * Puma-7 specific. The static structures are filled in with Puma-6 specific
 * data and the generic CPPI4.1 init function gets called in the end
 * with this data.
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2017 Intel Corporation. All rights reserved.
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */

#include <pal.h>
#include <puma7_cppi.h>
#include <linux/proc_fs.h>
#include <asm-arm/arch-avalanche/generic/sram_api.h>
#include <linux/seq_file.h>
#include <linux/skbuff.h>
#include <linux/interrupt.h>
#include <puma7_cppi_lqmgr_q.h>
#include <puma7_cppi_gqmgr0_q.h>
#include <puma7_cppi_gqmgr1_q.h>
#include <puma7_cppi_gqmgr2_q.h>
#include <puma7_cppi_dsgqmgr_q.h>
#include <puma7_cppi_usqmgr_q.h>
#include <puma7_cppi_bmgr_pools.h>
#include <puma7_pp.h>
#include <linux/mrpc/handshake_server.h>
#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#endif


/* FIXME : uncomment line below to enable debug prints */
//#define DEBUG_PAL(fmt,arg...)  printk(fmt , ##arg);

#ifndef DEBUG_PAL
#define DEBUG_PAL(fmt,arg...)
#endif

static int memProfile = 0;
static Uint32 rx_low_short_shared_ddr_emb_fd_desc_count         =   PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 rx_low_long_shared_ddr_emb_fd_desc_count          =   PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 moca_rx_low_short_private_ddr_emb_fd_desc_count   =   PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 moca_rx_low_long_private_ddr_emb_fd_desc_count    =   PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 moca_rx_low_short_private_packet_ram_emb_fd_desc_count = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL;
static Uint32 moca_rx_low_long_private_packet_ram_emb_fd_desc_count  = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii0_rx_low_short_private_ddr_emb_fd_desc_count =   PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii0_rx_low_long_private_ddr_emb_fd_desc_count  =   PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii0_rx_high_private_ddr_emb_fd_desc_count      =   PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii0_rx_low_short_private_packet_ram_emb_fd_desc_count = PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii0_rx_low_long_private_packet_ram_emb_fd_desc_count  = PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii1_rx_low_short_private_ddr_emb_fd_desc_count =   PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii1_rx_low_long_private_ddr_emb_fd_desc_count  =   PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii1_rx_high_private_ddr_emb_fd_desc_count      =   PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii1_rx_low_short_private_packet_ram_emb_fd_desc_count = PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL;
static Uint32 rgmii1_rx_low_long_private_packet_ram_emb_fd_desc_count = PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL;
static Uint32 wifi_rx_pool_buffer_count = PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_COUNT_FULL;

static PAL_Result cppi_pp_proc_init (Ptr hnd, Ptr param);
Int32 avalanche_cppi_init_pp_docsis_ral_and_qsmon(PAL_Handle palHandle);
Int32 avalanche_cppi_pp_configure_qsmon(PAL_Handle pal_handle, Uint32 manager, qsmonRal_t *qsmon, Bool setCount);

#define PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT           rx_low_short_shared_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT            rx_low_long_shared_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT     moca_rx_low_short_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT      moca_rx_low_long_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT  moca_rx_low_short_private_packet_ram_emb_fd_desc_count
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT   moca_rx_low_long_private_packet_ram_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT   rgmii0_rx_low_short_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT    rgmii0_rx_low_long_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT        rgmii0_rx_high_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT  rgmii0_rx_low_short_private_packet_ram_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT   rgmii0_rx_low_long_private_packet_ram_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT   rgmii1_rx_low_short_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT    rgmii1_rx_low_long_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT        rgmii1_rx_high_private_ddr_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT  rgmii1_rx_low_short_private_packet_ram_emb_fd_desc_count
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT   rgmii1_rx_low_long_private_packet_ram_emb_fd_desc_count
#define PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_COUNT                           wifi_rx_pool_buffer_count

#define PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_COUNT                  PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT                               +   \
                                                                    PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT  +   \
                                                                    PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                         +   \
                                                                    PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                         +   \
                                                                    PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                       +   \
                                                                    PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                       +   \
                                                                    PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                       +   \
                                                                    PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT

#define PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_COUNT                  PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT                                +   \
                                                                    PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT   +   \
                                                                    PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                          +   \
                                                                    PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                          +   \
                                                                    PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        +   \
                                                                    PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        +   \
                                                                    PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        +   \
                                                                    PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        +   \
                                                                    PAL_CPPI_PP_HOST2PP_LOW_INFRA_EMB_FD_DESC_COUNT                                     +   \
                                                                    PAL_CPPI_PP_HOST2PP_HI_INFRA_EMB_FD_DESC_COUNT

#define PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_COUNT                     PAL_CPPI_PP_DOCSIS_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            +   \
                                                                    PAL_CPPI_PP_ATOM_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                              +   \
                                                                    PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            +   \
                                                                    PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            +   \
                                                                    PAL_CPPI_PP_SGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            +   \
                                                                    PAL_CPPI_PP_SGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT

#define PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_COUNT                    PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                +   \
                                                                    PAL_CPPI_PP_RX_LOW_SHORT_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                        +   \
                                                                    PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                  +   \
                                                                    PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                  +   \
                                                                    PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                +   \
                                                                    PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                +   \
                                                                    PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                +   \
                                                                    PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT

#define PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_COUNT                     PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 +   \
                                                                    PAL_CPPI_PP_RX_LOW_LONG_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                         +   \
                                                                    PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                   +   \
                                                                    PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                   +   \
                                                                    PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 +   \
                                                                    PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 +   \
                                                                    PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 +   \
                                                                    PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT

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

/*******************/
/* Add Pools names */
/*******************/

#undef PAL_CPPI_BMGR_P_ADD
#define PAL_CPPI_BMGR_P_ADD(pName)     #pName,
Uint8 *PalCppiPpbmgrNames[PAL_CPPI41_BMGR_MAX_POOLS] =
{
    PAL_CPPI_PP_BMGR_POOL_LIST
};
EXPORT_SYMBOL(PalCppiPpbmgrNames);

/**************/
/* Add Q names*/
/**************/

#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qName)     "LO." #qName,
Uint8 *PalCppiPpLqmgrNames[PAL_CPPI_PP_QMGR_LOCAL_TOTAL_Q_COUNT] =
{
    PAL_CPPI_PP_QMGR_LOCAL_Q_LIST
};
EXPORT_SYMBOL(PalCppiPpLqmgrNames);

#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qName)        "G0." #qName,
Uint8 *PalCppiPpGqmgr0Names[PAL_CPPI_PP_QMGR_G0_TOTAL_Q_COUNT] =
{
    PAL_CPPI_PP_QMGR_G0_Q_LIST
};
EXPORT_SYMBOL(PalCppiPpGqmgr0Names);

#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qName)        "G1." #qName,
Uint8 *PalCppiPpGqmgr1Names[PAL_CPPI_PP_QMGR_G1_TOTAL_Q_COUNT] =
{
    PAL_CPPI_PP_QMGR_G1_Q_LIST
};
EXPORT_SYMBOL(PalCppiPpGqmgr1Names);

#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qName)        "G2." #qName,
Uint8 *PalCppiPpGqmgr2Names[PAL_CPPI_PP_QMGR_G2_TOTAL_Q_COUNT] =
{
    PAL_CPPI_PP_QMGR_G2_Q_LIST
};
EXPORT_SYMBOL(PalCppiPpGqmgr2Names);

#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qName)        "DSG0." #qName,
Uint8 *PalCppiDsg0qmgrNames[PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT] =
{
    PAL_CPPI_DSG_QMGR_Q_LIST
};

#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qName)        "DSG1." #qName,
Uint8 *PalCppiDsg1qmgrNames[PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT] =
{
    PAL_CPPI_DSG_QMGR_Q_LIST
};

#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qName)        "DSG2." #qName,
Uint8 *PalCppiDsg2qmgrNames[PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT] =
{
    PAL_CPPI_DSG_QMGR_Q_LIST
};

#undef PAL_CPPI_QMGR_Q_ADD

Uint32              g_proc_qsm = 1;
Uint32              g_threshold_0 = THRESHOLD_0_VALUE;
Uint32              g_threshold_1 = THRESHOLD_1_VALUE;
Cppi4InitCfg        gCppiPpInitCfg;
EXPORT_SYMBOL(gCppiPpInitCfg);

static Cppi4InitCfg gCppiDsgInitCfg;

/* proc interface  */
static PAL_Handle gPpPalHnd;
static PAL_Handle gDsgPalHnd;

typedef enum 
{
    CPPI_POST_INIT_OK,
    CPPI_POST_INIT_ERR_PAL_HND,
    CPPI_POST_INIT_ERR_BUFF_INIT,
    CPPI_POST_INIT_ERR_DMA_CH,
    CPPI_POST_INIT_ERR_SKB_SH
} cppi_post_init_return_error_e;

FDqueue_t gPpFDqueues[] =
{
    {.qMgr = PAL_CPPI_PP_QMGR_LOCAL,  .qId = PAL_CPPI_PP_QMGR_LOCAL_PP_INTERNAL_EMB_FD_Q_NUM,                           .descRegion = PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION,              .descCount = PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_COUNT,                                         .descSize = PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_SIZE,.descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G1,     .qId = PAL_CPPI_PP_QMGR_G1_US_FW_MONO_FD_Q_NUM,                                   .descRegion = PAL_CPPI_PP_US_FW_MONO_DESC_REGION,                       .descCount = PAL_CPPI_PP_US_FW_MONO_FD_DESC_COUNT,                                                  .descSize = PAL_CPPI_PP_US_FW_MONO_FD_DESC_SIZE,        .descType = CPPI41_DESC_TYPE_MONOLITHIC, .allocType = PAL_CPPI_ALLOC_NONE},

#ifdef CONFIG_IPSEC_SUPPORT
    // IPSEC on-chip free Q. Descriptors are of type Embedded, but are used as contiguous desc+buffer blocks (as expressed by 2K desc size)
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_IPSEC_MONOLITHIC_FD_Q_NUM,                             .descRegion = PAL_CPPI_PP_IPSEC_MONOLITHIC_GLOBAL_DESC_REGION,          .descCount = PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_DESC_COUNT,                                            .descSize = PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    // Free Q for descriptors to be used by crypto driver to pass to PP
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_IPSEC_HOST_FD_Q_NUM,                                   .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_IPSEC_HOST_FD_DESC_COUNT,                                                  .descSize = PAL_CPPI_PP_IPSEC_HOST_FD_DESC_SIZE,        .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
#endif

    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_LOW_INFRA_HOST_FD_Q_NUM,                        .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_DOCSIS_LOW_INFRA_HOST_FD_DESC_COUNT,                                       .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_HI_INFRA_HOST_FD_Q_NUM,                         .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_DOCSIS_HI_INFRA_HOST_FD_DESC_COUNT,                                        .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_NONE }, 
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM,                        .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_SHARED_LOW_INFRA_HOST_FD_DESC_COUNT,                                       .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_ATOM_HI_INFRA_HOST_FD_Q_NUM,                           .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_ATOM_HI_INFRA_HOST_FD_DESC_COUNT,                                          .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII0_HI_INFRA_HOST_FD_Q_NUM,                         .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_RGMII0_HI_INFRA_HOST_FD_DESC_COUNT,                                        .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII1_HI_INFRA_HOST_FD_Q_NUM,                         .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_RGMII1_HI_INFRA_HOST_FD_DESC_COUNT,                                        .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII0_HI_INFRA_HOST_FD_Q_NUM,                         .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_SGMII0_HI_INFRA_HOST_FD_DESC_COUNT,                                        .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII1_HI_INFRA_HOST_FD_Q_NUM,                         .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_SGMII1_HI_INFRA_HOST_FD_DESC_COUNT,                                        .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
#ifdef CONFIG_WIFI_PROXY
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_WIFI_INFRA_HOST_RX_FD_Q_NUM,                           .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_WIFI_INFRA_HOST_RX_FD_DESC_COUNT,                                          .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
#endif
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_TX_MGMT_HOST_FD_Q_NUM,                          .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_DOCSIS_TX_MGMT_HOST_FD_DESC_COUNT,                                         .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_CALLOC }, 
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_HOST2PP_LOW_HOST_FD_Q_NUM,                             .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_HOST2PP_LOW_HOST_FD_DESC_COUNT,                                            .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_HOST2PP_HI_HOST_FD_Q_NUM,                              .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_HOST2PP_HI_HOST_FD_DESC_COUNT,                                             .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,      .allocType = PAL_CPPI_ALLOC_SKB },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,           .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_RX_LOW_SHORT_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT,                          .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,            .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_RX_LOW_LONG_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT,                           .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,                               .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_FULL,                            .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,                                .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT_FULL,                             .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,                               .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_RX_LOW_XLONG_SHARED_DDR_EMB_FD_DESC_COUNT,                                 .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM,   .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT,    .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,           .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                  .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,            .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                   .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_HI_EMB_FD_Q_NUM,                             .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_DOCSIS_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT,                              .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_MGMT_HOST_FD_Q_NUM,                          .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_DOCSIS_RX_MGMT_HOST_FD_DESC_COUNT,                                         .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,     .allocType = PAL_CPPI_ALLOC_CALLOC },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,                 .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL,               .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL,                .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_SHORT_RX_EMB_FD_Q_NUM,                        .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                      .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_LONG_RX_EMB_FD_Q_NUM,                         .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                       .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,                 .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                    .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                     .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_SHORT_RX_EMB_FD_Q_NUM,                        .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT,                           .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,                     .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT,                            .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,                    .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_ATOM_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT,                                .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,               .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL,             .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,                .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL,              .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM,                      .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                    .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,                   .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                     .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                         .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,               .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL,             .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,                .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL,              .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM,                      .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                    .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,                   .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                     .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL,                         .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,               .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                  .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,                .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                   .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM,                      .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT,                         .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,                   .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT,                          .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_SGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT,                              .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,               .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                  .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,                .descRegion = PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                .descCount = PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT,                   .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM,                      .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT,                         .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,                   .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT,                          .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_SGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT,                              .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_LOW_EMB_FD_Q_NUM,                        .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_HOST2PP_LOW_INFRA_EMB_FD_DESC_COUNT,                                       .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_HIGH_EMB_FD_Q_NUM,                       .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_HOST2PP_HI_INFRA_EMB_FD_DESC_COUNT,                                        .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
#ifdef CONFIG_WIFI_PROXY
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_WIFI_FW_RX_EMB_FD_Q_NUM,                               .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_WIFI_FW_RX_EMB_FD_DESC_COUNT,                                              .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    //{.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_WIFI_FW_RX_REASSEMBLY_EMB_FD_Q_NUM,                    .descRegion = PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,      .descCount = PAL_CPPI_PP_WIFI_FW_RX_REASSEMBLY_EMB_FD_DESC_COUNT,                                   .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
#endif
#if CONFIG_NP_APP_DATAPIPE
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_NP2APP_HOST_FD_Q_NUM,                                  .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_NP2APP_HOST_FD_DESC_COUNT,                                                 .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_PPINFO_HOST_FD_Q_NUM,                     .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_NP2APP_INFRA_PPINFO_HOST_FD_DESC_COUNT,                                    .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_DATA_HOST_FD_Q_NUM,                       .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_NP2APP_INFRA_DATA_HOST_FD_DESC_COUNT,                                      .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_APP2NP_HOST_FD_Q_NUM,                                  .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_APP2NP_HOST_FD_DESC_COUNT,                                                 .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_PPINFO_HOST_FD_Q_NUM,                     .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_APP2NP_INFRA_PPINFO_HOST_FD_DESC_COUNT,                                    .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_DATA_HOST_FD_Q_NUM,                       .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_APP2NP_INFRA_DATA_HOST_FD_DESC_COUNT,                                      .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_CTRL_PPINFO_HOST_FD_Q_NUM,                .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_APP2NP_INFRA_HIGH_PPINFO_HOST_FD_DESC_COUNT,                               .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_CTRL_DATA_HOST_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_APP2NP_INFRA_HIGH_DATA_HOST_FD_DESC_COUNT,                                 .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_CTRL_PPINFO_HOST_FD_Q_NUM,                .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_NP2APP_INFRA_HIGH_PPINFO_HOST_FD_DESC_COUNT,                               .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_CTRL_DATA_HOST_FD_Q_NUM,                  .descRegion = PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION, .descCount = PAL_CPPI_PP_NP2APP_INFRA_HIGH_DATA_HOST_FD_DESC_COUNT,                                 .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_HOST,    .allocType = PAL_CPPI_ALLOC_NONE },	
#endif

#ifdef CONFIG_INTEL_KERNEL_VOICE_SUPPORT
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_VOICE_DSP_RX_EMB_FD_Q_NUM,                             .descRegion = PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION,                     .descCount = PAL_CPPI_PP_VOICE_DSP_RX_EMB_FD_DESC_COUNT,                                            .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_VOICE_INFRA_RX_EMB_FD_Q_NUM,                           .descRegion = PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION,                     .descCount = PAL_CPPI_PP_VOICE_INFRA_RX_EMB_FD_DESC_COUNT,                                          .descSize = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE,  .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
#endif
    {.qMgr = PAL_CPPI_PP_QMGR_G2,     .qId = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM,                                     .descRegion = PAL_CPPI_PP_TEARDOWN_GLOBAL_DESC_REGION,                  .descCount = PAL_CPPI_PP_TEARDOWN_FD_DESC_COUNT,                                                    .descSize = PAL_CPPI_PP_TEARDOWN_FD_DESC_SIZE,          .descType = CPPI41_DESC_TYPE_TEARDOWN, .allocType = PAL_CPPI_ALLOC_NONE}
};
EXPORT_SYMBOL(gPpFDqueues);

FDqueue_t gDsgFDqueues[] =
{
    {.qMgr = PAL_CPPI_OFDM0_QUEUE_MGR,    .qId = PAL_CPPI_DSG_QMGR_FD_EMB_Q_NUM,    .descRegion = PAL_CPPI_DSG_FD_EMB_DESC_REGION,   .descCount = PAL_CPPI_OFDM0_FD_EMB_DESC_COUNT,       .descSize = PAL_CPPI_DS_DSG_FD_EMB_DESC_SIZE,      .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
#ifdef CONFIG_MACH_PUMA7_BOARD
    {.qMgr = PAL_CPPI_OFDM1_QUEUE_MGR,    .qId = PAL_CPPI_DSG_QMGR_FD_EMB_Q_NUM,    .descRegion = PAL_CPPI_DSG_FD_EMB_DESC_REGION,   .descCount = PAL_CPPI_OFDM1_FD_EMB_DESC_COUNT,       .descSize = PAL_CPPI_DS_DSG_FD_EMB_DESC_SIZE,      .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE},
#endif
    {.qMgr = PAL_CPPI_SC_QAM0_QUEUE_MGR,  .qId = PAL_CPPI_DSG_QMGR_FD_EMB_Q_NUM,    .descRegion = PAL_CPPI_DSG_FD_EMB_DESC_REGION,   .descCount = PAL_CPPI_SC_QAM0_FD_EMB_DESC_COUNT,     .descSize = PAL_CPPI_DS_DSG_FD_EMB_DESC_SIZE,      .descType = CPPI41_DESC_TYPE_EMBEDDED, .allocType = PAL_CPPI_ALLOC_NONE}
};
EXPORT_SYMBOL(gDsgFDqueues);

qsmonRal_t gQsmon1[] =
{
    {.qsmonThread = 0, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_RGMII0_TX_HI_Q_NUM, .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    {.qsmonThread = 1, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_RGMII1_TX_HI_Q_NUM, .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    {.qsmonThread = 2, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_SGMII0_TX_HI_Q_NUM, .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    {.qsmonThread = 3, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_SGMII1_TX_HI_Q_NUM, .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    {.qsmonThread = 4, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_ATOM_TX_HI_Q_NUM  , .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    {.qsmonThread = 5, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_MoCA_TX_HI_Q_NUM  , .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    /* ---- DO NOT USE - These threads are dedicated to the runtime configurable QSM ---- */
    {.qsmonThread = 6, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_MoCA_TX_HI_Q_NUM  , .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    {.qsmonThread = 7, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_MoCA_TX_HI_Q_NUM  , .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    {.qsmonThread = 8, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G1, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G1_MoCA_TX_HI_Q_NUM  , .direction = PAL_CPPI_QSMON_DIRECTION_UP},
    /* ---------------------------------------------------------------------------------- */
};

qsmonRal_t gQsmon2[] =
{
    {.qsmonThread =  0, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM        , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_RX_LOW_SHORT_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                        },
    {.qsmonThread =  1, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM         , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_RX_LOW_LONG_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                         },
    {.qsmonThread =  2, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW                            , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_FULL                          },
    {.qsmonThread =  3, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_TX_COMPLETE_Q_NUM                           , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
    {.qsmonThread =  4, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_HOST_FD_Q_NUM                               , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_NP2APP_HOST_FD_DESC_COUNT                                               },
    {.qsmonThread =  5, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_PPINFO_HOST_FD_Q_NUM                  , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_NP2APP_INFRA_PPINFO_HOST_FD_DESC_COUNT                                  },
    {.qsmonThread =  6, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_DATA_HOST_FD_Q_NUM                    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_NP2APP_INFRA_DATA_HOST_FD_DESC_COUNT                                    },
    {.qsmonThread =  7, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_CTRL_DATA_HOST_FD_Q_NUM               , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_NP2APP_INFRA_HIGH_DATA_HOST_FD_DESC_COUNT                               },
    {.qsmonThread =  8, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_HOST_FD_Q_NUM                               , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_APP2NP_HOST_FD_DESC_COUNT                                               },
    {.qsmonThread =  9, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_PPINFO_HOST_FD_Q_NUM                  , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_APP2NP_INFRA_PPINFO_HOST_FD_DESC_COUNT                                  },
    {.qsmonThread = 10, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_DATA_HOST_FD_Q_NUM                    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_APP2NP_INFRA_DATA_HOST_FD_DESC_COUNT                                    },
    {.qsmonThread = 11, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_CTRL_DATA_HOST_FD_Q_NUM               , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_APP2NP_INFRA_HIGH_DATA_HOST_FD_DESC_COUNT                               },
    {.qsmonThread = 12, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_TX_COMPLETE_Q_NUM                           , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
#ifndef CONFIG_IPSEC_SUPPORT
    {.qsmonThread = 13, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_SHORT_RX_EMB_FD_Q_NUM                     , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                    },
    {.qsmonThread = 14, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_LONG_RX_EMB_FD_Q_NUM                      , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                     },
    {.qsmonThread = 15, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM        , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                },
    {.qsmonThread = 16, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM         , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 },
    {.qsmonThread = 17, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM, .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT  },
#else
    {.qsmonThread = 13, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_ENCRYPT_HOST_IN_Q_NUM                              , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
    {.qsmonThread = 14, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DECRYPT_HOST_IN_Q_NUM                              , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
    {.qsmonThread = 15, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_IPSEC_MONOLITHIC_FD_Q_NUM                          , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold =  PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_DESC_COUNT                                         },
    {.qsmonThread = 16, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_CRYPTO_AES_Q_NUM                                   , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
    {.qsmonThread = 17, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_CRYPTO_HCU_Q_NUM                                   , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
#endif
    {.qsmonThread = 18, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM                                , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
    {.qsmonThread = 19, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_MED_LOW_Q_NUM                            , .direction = PAL_CPPI_QSMON_DIRECTION_UP  , .qsmonThreshold = 0                                                                                   },
    {.qsmonThread = 20, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_LOW_INFRA_HOST_FD_Q_NUM                     , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_DOCSIS_LOW_INFRA_HOST_FD_DESC_COUNT                                     },
    {.qsmonThread = 21, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM                     , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_SHARED_LOW_INFRA_HOST_FD_DESC_COUNT                                     },
    {.qsmonThread = 22, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII0_HI_INFRA_HOST_FD_Q_NUM                      , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_RGMII0_HI_INFRA_HOST_FD_DESC_COUNT                                      },
    {.qsmonThread = 23, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII1_HI_INFRA_HOST_FD_Q_NUM                      , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_RGMII1_HI_INFRA_HOST_FD_DESC_COUNT                                      },
    {.qsmonThread = 24, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII0_HI_INFRA_HOST_FD_Q_NUM                      , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_SGMII0_HI_INFRA_HOST_FD_DESC_COUNT                                      },
    {.qsmonThread = 25, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII1_HI_INFRA_HOST_FD_Q_NUM                      , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_SGMII1_HI_INFRA_HOST_FD_DESC_COUNT                                      },
    {.qsmonThread = 26, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_HOST2PP_LOW_HOST_FD_Q_NUM                          , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_HOST2PP_LOW_HOST_FD_DESC_COUNT                                          },
    {.qsmonThread = 27, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_LOW_EMB_FD_Q_NUM                     , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_HOST2PP_LOW_INFRA_EMB_FD_DESC_COUNT                                     },
    {.qsmonThread = 28, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_HIGH_EMB_FD_Q_NUM                    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_HOST2PP_HI_INFRA_EMB_FD_DESC_COUNT                                      },
    /* ---- DO NOT USE - These threads are dedicated to the runtime configurable QSM ---- */
    {.qsmonThread = 29, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_HIGH_EMB_FD_Q_NUM                    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                          },
    {.qsmonThread = 30, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_HIGH_EMB_FD_Q_NUM                    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                          },
    {.qsmonThread = 31, .qsmonChEn = True, .fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2, .fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_HIGH_EMB_FD_Q_NUM                    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, .qsmonThreshold = PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                          },
    /* ---------------------------------------------------------------------------------- */
};



/***************************/
/* QSM Configurable queues */
/***************************/

#define QSMON_SIZE(num) (sizeof(gQsmon##num) / sizeof(gQsmon##num[0]))
#define QSMON_CONF_MGR_THREADS(num) {QSMON_SIZE(num)-3, QSMON_SIZE(num)-2, QSMON_SIZE(num)-1}
#define QSMON_CONF_MAX_THREADS  3
#define QSMON_CONF_HISTORY_NUM_ENTRIES 2

typedef struct {
    Uint32 qsmonThread;
    Uint32 qMgr;
    Uint32 qNum;
    Bool   direction;
    Uint32 qsmonThreshold;
} qsmonConfHistory_t;

qsmonConfHistory_t qsmonConfHistory[QSMON_CONF_HISTORY_NUM_ENTRIES];

int qsmonConfThreadNums[2][QSMON_CONF_MAX_THREADS] = {
    QSMON_CONF_MGR_THREADS(1),
    QSMON_CONF_MGR_THREADS(2)
};

qsmonRal_t gBsmon[PAL_CPPI41_BMGR_MAX_POOLS] =
{
    {.qsmonThread = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
    {.qsmonThread = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
    {.qsmonThread = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
    {.qsmonThread = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL    , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
    {.qsmonThread = PAL_CPPI_PP_WIFI_RX_POOL                  , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_WIFI_RX_POOL                  , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
    {.qsmonThread = PAL_CPPI_PP_BUFFER_POOL5                  , .qsmonChEn = False,  },
    {.qsmonThread = PAL_CPPI_PP_BUFFER_POOL6                  , .qsmonChEn = False   },
    {.qsmonThread = PAL_CPPI_PP_BUFFER_POOL7                  , .qsmonChEn = False   },
    {.qsmonThread = PAL_CPPI_PP_BUFFER_POOL8                  , .qsmonChEn = False   },
    {.qsmonThread = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL   , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL   , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
    {.qsmonThread = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL    , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL    , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
    {.qsmonThread = PAL_CPPI_PP_VOICE_DSP_BUFFER_POOL         , .qsmonChEn = True , .fdQueue.qMgr = PAL_CPPI_PP_BSMON_MGR, .fdQueue.qNum = PAL_CPPI_PP_VOICE_DSP_BUFFER_POOL         , .direction = PAL_CPPI_QSMON_DIRECTION_DOWN, }, // qsmonThreshold initialized runtime
};
EXPORT_SYMBOL(gBsmon);

#define PAL_CPPI_DSG_PSI_SIZE   5

typedef struct
{
    Cppi4EmbdDesc   hw;
    Uint32          psi[PAL_CPPI_DSG_PSI_SIZE];
}
DsgCppiDescType_t;

/*
    Docsis DS Co-Processor commands templates
    Co-Processor command:
        Word0[31-28]    Command index
        Word0[27-24]    Command length
        Word0[23]       Next    0 - indicate last command
                                1 - indicate there are more commands
        Word0[22-0]     command specific
        Word1           optional

    0x21 - BPI command template, (BPI command id, command length of 1)
    0x41 - CRC command template, (CRC command id, command length of 1)
    0x11 - PHS command template, (PHS command id, command length of 1)
    0x72 - RX  command template, (RX  command id, command length of 2)
*/
#define DS_EMBEDDED_DESC_BPI_CMD_TEMPLATE           0x21800000  /* TBD - Idan - please comment how we get to these magic numbers */
#define DS_EMBEDDED_DESC_CRC_CMD_TEMPLATE           0x41800000
#define DS_EMBEDDED_DESC_PHS_CMD_TEMPLATE           0x11800000
#define DS_EMBEDDED_DESC_RX_FIFO_0_CMD_TEMPLATE     0x72000000
#define DS_EMBEDDED_DESC_RX_FIFO_1_CMD_TEMPLATE     0x080404C1

#define DSG_INTERNAL_ADDR_MASK                      ((Uint32)(1 << 19) - 1)
#define DSG_INTERNAL_OFFSET(addr)                   ((addr) & DSG_INTERNAL_ADDR_MASK)

#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
#define INIT_BUFFER_POOLS_MAX_ITER                  250
static struct workqueue_struct *init_buffer_pools_wq      = NULL; /* work queue for buffer pools init process */
static const  char             *init_buffer_pools_wq_name = "Buffer pools init work queue";
static DECLARE_DELAYED_WORK(d_init_buffer_pools_work, NULL);
static void init_buffer_pools(struct work_struct *work);
static int init_buffer_pools_iterations;
extern Uint32 avalanche_cppi_get_skb_shared_info_size( void );
#endif


static ssize_t cppi_pp_dump_all_stats(struct seq_file *m, void *v)
{
    PAL_CPPI_PP_QMGRs_e qMgr;
    Uint16              qNum;
    Uint16              maxQNum;
    Cppi4Queue          cppiQueue;
    Uint32              pktCount;
    Uint32              expectedCount;
    Uint32              i;

    for (qMgr = 0; qMgr < PAL_CPPI41_NUM_QUEUE_MGR; qMgr++)
    {
        switch (qMgr)
        {
#if !defined(CONFIG_MACH_PUMA7_FPGA_PP) && !defined (CONFIG_MACH_PUMA7_BOARD)
        case PAL_CPPI_PP_QMGR_LOCAL:    continue;
        case PAL_CPPI_PP_QMGR_G0:       continue;
    #ifndef CONFIG_MACH_PUMA7_FPGA_US
        case PAL_CPPI_PP_QMGR_G1:       continue;
    #endif
#endif
        default: break;
        }

        maxQNum = (qMgr == PAL_CPPI_PP_QMGR_G0 ? PAL_CPPI_PP_QMGR_G0_TOTAL_Q_COUNT :
                   (qMgr == PAL_CPPI_PP_QMGR_G1 ? PAL_CPPI_PP_QMGR_G1_TOTAL_Q_COUNT :
                    (qMgr == PAL_CPPI_PP_QMGR_G2 ? PAL_CPPI_PP_QMGR_G2_TOTAL_Q_COUNT : PAL_CPPI_PP_QMGR_LOCAL_TOTAL_Q_COUNT)));

        for (qNum = 0; qNum < maxQNum; qNum++)
        {
            cppiQueue.qMgr = qMgr;
            cppiQueue.qNum = qNum;

            PAL_cppi4Control(gPpPalHnd, PAL_CPPI41_IOCTL_GET_QUEUE_ENTRY_COUNT, &cppiQueue, &pktCount);

            expectedCount = 0;

            for (i = 0; i < ARRAY_SIZE(gPpFDqueues); i++)
            {
                if (gPpFDqueues[i].qMgr == qMgr && gPpFDqueues[i].qId == qNum)
                {
                    expectedCount = gPpFDqueues[i].descCount;
                    break;
                }
            }

            if (pktCount != expectedCount)
            {
                seq_printf(m, "%4d %-65s : %4d [%d]\n", qNum, PAL_CPPI_PP_QMGR_GET_Q_NAME(qMgr, qNum), pktCount, expectedCount);
            }
        }
    }

    return 0;
}

static int cppi_pp_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, cppi_pp_dump_all_stats, NULL);
}

static const struct file_operations     cppi_pp_proc_fops =
{
    .open     = cppi_pp_proc_open,
    .read     = seq_read,
    .llseek   = seq_lseek,
    .release  = single_release,
};

static ssize_t cppi_qsm_dump(struct seq_file *m, void *v)
{
    CSL_QSMon_RegsOvly  qsmon;
    PAL_CPPI_PP_QSMON_MGRs_e mngr;
    Uint32              thread;
    CSL_Reg32           reg;

    // check if we want to reset counters
    if (g_proc_qsm == 0) 
    {
        for (mngr = PAL_CPPI_PP_QSMON_MGR1; mngr <= PAL_CPPI_PP_QSMON_MGR2; mngr++)
        {
            qsmon = (Ptr) gCppiPpInitCfg.qsmon[mngr];
            for (thread = 0; thread < 32; thread++)
            {
                reg = qsmon->Cmds[thread].Register_B;
                if (reg & (1 << QSMON_CONFIG_MONITORED_QUEUE_EN_SHIFT))
                {
                    qsmon->Depth[thread].Stats_Qdepth_Watermark = qsmon->Depth[thread].Stats_Qdepth_Cnt;
                }
            }
        }

        // reset the value so that we don't reset counters unless explicitly asked
        g_proc_qsm = 1;
    }

    seq_printf(m, "| QSM | Thread | Queue | Watermark | Current | Min/Max | Qname\n");
    seq_printf(m, "|-----|--------|-------|-----------|---------|---------|------\n");

    for (mngr = PAL_CPPI_PP_QSMON_MGR1; mngr <= PAL_CPPI_PP_QSMON_MGR2; mngr++)
    {
        qsmon = (Ptr) gCppiPpInitCfg.qsmon[mngr];
        for (thread = 0; thread < 32; thread++)
        {
            reg = qsmon->Cmds[thread].Register_B;
            if (reg & (1 << QSMON_CONFIG_MONITORED_QUEUE_EN_SHIFT))
            {
                seq_printf(m, "|  %d  |   %2d   | %d.%3d |   %5d   |  %5d  |   %s   | %s\n",
                           mngr, thread, mngr, (reg & QSMON_CONFIG_MONITORED_QUEUE_NUM_MASK), qsmon->Depth[thread].Stats_Qdepth_Watermark, qsmon->Depth[thread].Stats_Qdepth_Cnt,
                           (reg & (1 << QSMON_CONFIG_MONITORED_QUEUE_DIR_SHIFT)) ? "Min" : "Max",
                           PAL_CPPI_PP_QMGR_GET_Q_NAME(mngr, (reg & QSMON_CONFIG_MONITORED_QUEUE_NUM_MASK)));
            }
        }
    }

    return 0;
}

static int cppi_qsm_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, cppi_qsm_dump, NULL);
}

static ssize_t cppi_qsm_write_proc(struct file *fp, const char __user * buf, size_t count, loff_t *ppos)
{
    unsigned char local_buf[10];
    int ret_val = 0;

    if (count >= 10)
    {
        printk(KERN_ERR "\n%s[%d]: Buffer Overflow\n", __FUNCTION__, __LINE__);
        return -EFAULT;
    }

    if (copy_from_user(local_buf, buf, count))
    {
        return -EFAULT;
    }

    local_buf[count]='\0';
    ret_val = count;

    // set a global flag to signal to the read proc called later whether we want to reset counters
    sscanf(local_buf, "%d", &g_proc_qsm);

    return ret_val;
}

static const struct file_operations     cppi_qsm_proc_fops =
{
    .open     = cppi_qsm_proc_open,
    .read     = seq_read,
    .llseek   = seq_lseek,
    .release  = single_release,
    .write    = cppi_qsm_write_proc
};

static ssize_t cppi_qsmcfg_dump(struct seq_file *m, void *v)
{
    Uint32 history_idx;

    seq_printf(m, "|         | QSM | Thread | Queue | Min/Max |\n");
    seq_printf(m, "|---------|-----|--------|-------|---------|\n");

    for (history_idx = 0; history_idx < QSMON_CONF_HISTORY_NUM_ENTRIES; ++history_idx)
    {
        seq_printf(m, "| %s |  %d  |   %2d   | %d.%3d |   %s   |\n",
                   (history_idx ? "  NEW  " : "DELETED"), qsmonConfHistory[history_idx].qMgr, qsmonConfHistory[history_idx].qsmonThread, 
                   qsmonConfHistory[history_idx].qMgr, qsmonConfHistory[history_idx].qNum, qsmonConfHistory[history_idx].direction ? "Min" : "Max");
    }

    return 0;
}

static int cppi_qsmcfg_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, cppi_qsmcfg_dump, NULL);
}

static ssize_t cppi_qsmcfg_write_proc(struct file *fp, const char __user * buf, size_t count, loff_t *ppos)
{
    char *token, *str;
    char *sep = ", \t.";
    char buffer[128];
    Uint32 qMgr = 0;
    Uint32 qNum = 0;
    Uint32 qsmonThreshold = 0;
    Uint32 qsmonThread;
    Bool direction = 0;
    PAL_Handle palHandle;
    static Uint32 qsmonCurTh[2] = {0};
    qsmonRal_t  *qsmon;

    palHandle = PAL_cppi4Init (NULL,(Ptr)CPPI41_DOMAIN_PP);
    if (palHandle == NULL)
    {
         printk("%s %d Error: palHandle is NULL\n", __FUNCTION__, __LINE__);
         return -1;
    }

    if (count >= 10)
    {
        printk(KERN_ERR "\n%s[%d]: Buffer Overflow\n", __FUNCTION__, __LINE__);
        return -EFAULT;
    }

    if (copy_from_user(buffer, buf, count))
    {
        printk("ERROR: Failed to read from user space\n\n");
        return -EFAULT;
    }

    buffer[count] = '\0';

    str = &buffer[0];

    // Get qMgr
    token = strsep(&str, sep);
    qMgr = (Uint32)simple_strtol(token, NULL, 0);

    if (qMgr < 1 || qMgr > 2)
    {
        printk(KERN_ERR "Invalid qMgr index %d [valid: 1/2]\n\n", qMgr);
        return -EINVAL;
    }

    // Get qNum
    token = strsep(&str, sep);
    qNum = (Uint32)simple_strtol(token, NULL, 0);

    if (qNum < 0 || qNum > 511)
    {
        printk(KERN_ERR "Invalid qNum index %d [valid: 0-511]\n\n", qNum);
        return -EINVAL;
    }

    // Get direction
    token = strsep(&str, sep);
    direction = (Bool)simple_strtol(token, NULL, 0);

    if (direction != 0 && direction != 1)
    {
        printk(KERN_ERR "Invalid direction index %d [valid: 0/1 - UP/DOWN]\n\n", direction);
        return -EINVAL;
    }

    switch (qMgr)
    {
        case PAL_CPPI_PP_QSMON_MGR1:
            qsmon = gQsmon1;
            break;
        case PAL_CPPI_PP_QSMON_MGR2:
            qsmon = gQsmon2;
            break;
        default:
            break;
    }

    qsmonThread = qsmonConfThreadNums[qMgr - 1][qsmonCurTh[qMgr - 1]];
    
    // For direction DOWN set qsmonThreshold to the number of descriptors in queue
    if ( direction )
    {
        if ( PAL_cppi4QueueGetEntryCount(palHandle, qsmon[qsmonThread].fdQueue, &qsmonThreshold) != PAL_SOK ) 
         {
             printk(KERN_ERR "Failed to set qsmonThreshold according to PAL_cppi4QueueGetEntryCount\n\n");
             return -EFAULT;
         }
    }

    // Set "DELETED" history entry
    qsmonConfHistory[0] = (qsmonConfHistory_t){ qsmon[qsmonThread].qsmonThread, qsmon[qsmonThread].fdQueue.qMgr, qsmon[qsmonThread].fdQueue.qNum, qsmon[qsmonThread].direction, qsmon[qsmonThread].qsmonThreshold }; 

    qsmon[qsmonThread].fdQueue.qNum = qNum;
    qsmon[qsmonThread].direction = direction;
    qsmon[qsmonThread].qsmonThreshold = qsmonThreshold;


    if(avalanche_cppi_pp_configure_qsmon(palHandle, qMgr, &qsmon[qsmonThread], False) != PAL_SOK) 
    {
             printk(KERN_ERR "avalanche_cppi_pp_configure_qsmon Failed\n\n");
             return -EFAULT;
    }

    if(avalanche_cppi_pp_configure_qsmon(palHandle, qMgr, &qsmon[qsmonThread], True) != PAL_SOK) 
    {
             printk(KERN_ERR "avalanche_cppi_pp_configure_qsmon Failed\n\n");
             return -EFAULT;
    }

    // Set "New" history entry
    qsmonConfHistory[1] = (qsmonConfHistory_t) { qsmonThread, qMgr, qNum, direction, qsmonThreshold };

    // Update next thread to be 
    qsmonCurTh[qMgr - 1] = (qsmonCurTh[qMgr - 1] + 1) % QSMON_CONF_MAX_THREADS; 

    return count;
}

static const struct file_operations     cppi_qsmcfg_proc_fops =
{
    .open     = cppi_qsmcfg_proc_open,
    .read     = seq_read,
    .llseek   = seq_lseek,
    .release  = single_release,
    .write    = cppi_qsmcfg_write_proc
};

static ssize_t cppi_bsm_dump(struct seq_file *m, void *v)
{
    CSL_QSMon_RegsOvly  bsmon;
    Uint32              i;
    CSL_Reg32           reg;

    seq_printf(m, "| Thread | Pool | Watermark | Current | Min/Max |  Pname                                     |\n");
    seq_printf(m, "|--------|------|-----------|---------|---------|--------------------------------------------|\n");

    bsmon = (Ptr) gCppiPpInitCfg.qsmon[PAL_CPPI_PP_BSMON_MGR];
    for (i = 0; i < (sizeof(gBsmon) / sizeof(qsmonRal_t)); i++)
    {
        if (gBsmon[i].qsmonChEn)
        {
            reg = bsmon->Cmds[gBsmon[i].qsmonThread].Register_B;
            seq_printf(m, "|   %2d   |  %2d  |   %5d   |  %5d  |   %s   | %-42s |\n",
                       gBsmon[i].qsmonThread,
                       (reg & QSMON_CONFIG_MONITORED_QUEUE_NUM_MASK),
                       bsmon->Depth[gBsmon[i].qsmonThread].Stats_Qdepth_Watermark,
                       bsmon->Depth[gBsmon[i].qsmonThread].Stats_Qdepth_Cnt,
                       (reg & (1 << QSMON_CONFIG_MONITORED_QUEUE_DIR_SHIFT)) ? "Min" : "Max",
                       PalCppiPpbmgrNames[(reg & QSMON_CONFIG_MONITORED_QUEUE_NUM_MASK)]);
        }
    }

    return 0;
}

static int cppi_bsm_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, cppi_bsm_dump, NULL);
}

static const struct file_operations     cppi_bsm_proc_fops =
{
    .open     = cppi_bsm_proc_open,
    .read     = seq_read,
    .llseek   = seq_lseek,
    .release  = single_release,
};


static pal_cppi_dbg_cfg_t       puma7_pp_dbg_cfg;

Uint32 avalanche_cppi_init_pp_buffer_pools(PAL_Handle palHandle);
Int32 avalanche_cppi_init_pp_dma_channels(PAL_Handle palHandle);

/* Post init for atom buffers allocation */
static int cppi_post_init( void )
{
    PAL_Handle          palHandle;
    Uint32              err;
    Uint32              skb_shared_info_size;

    palHandle = PAL_cppi4Init(&gCppiPpInitCfg, CPPI41_DOMAIN_PP);
    if(palHandle == NULL)
    {
        printk(KERN_EMERG "ERROR PAL_cppi4Init of CPPI41_DOMAIN_PP failed!\n");
        return CPPI_POST_INIT_ERR_PAL_HND;
    }

    /************************************************************************/
    /*********** Init PP buffer pools used in the system ********************/
    /************************************************************************/
    err = avalanche_cppi_init_pp_buffer_pools(palHandle);
    if (err)
    {
        printk(KERN_ERR "ERROR: PAL_cppi4BufPoolInit for pool %d [CPPI41_DOMAIN_PP] FAILED\n", err-1);
        return CPPI_POST_INIT_ERR_BUFF_INIT;
    }

#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
    /*************************************************************/
    /************ Get skb_shared_info size from ATOM ************/
    /*************************************************************/
    skb_shared_info_size = avalanche_cppi_get_skb_shared_info_size();
    if (!skb_shared_info_size)
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_get_skb_shared_info_size [CPPI41_DOMAIN_PP] FAILED\n");
        return CPPI_POST_INIT_ERR_SKB_SH;
    }

    g_threshold_0 = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_SIZE - PAL_CPPI_PP_START_OF_PACKET_OFFSET - skb_shared_info_size;
    g_threshold_1 = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_SIZE - PAL_CPPI_PP_START_OF_PACKET_OFFSET - skb_shared_info_size;

    DEBUG_PAL(KERN_ERR "avalanche_cppi_get_skb_shared_info_size: g_threshold_0 = %d\n", g_threshold_0);
#endif

    /******************************************/
    /*********** Setup DMA channels ***********/
    /******************************************/
    if (avalanche_cppi_init_pp_dma_channels(palHandle))
    {
        DEBUG_PAL(KERN_ERR "ERROR: avalanche_cppi_init_pp_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    #ifdef CONFIG_MRPC_HANDSHAKE_SERVER
    boot_status_report(HANDSHAKE_ID_CPPI);
    #endif

    return CPPI_POST_INIT_OK;
}

static PAL_Result cppi_pp_proc_init(Ptr hnd, Ptr param)
{
    struct proc_dir_entry * dir_1 = (struct proc_dir_entry *)param;

    gPpPalHnd = (PAL_Handle)hnd;

    puma7_pp_dbg_cfg.handle             = (PAL_Handle)hnd;
    puma7_pp_dbg_cfg.allocQueue.qNum    = -1;
    puma7_pp_dbg_cfg.allocBuf.bPool     = -1;
    puma7_pp_dbg_cfg.outputQueue.qNum   = -1;
    puma7_pp_dbg_cfg.dataLen            = 0;


    if (NULL == (dir_1 = proc_mkdir("sr",       dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create_data("dbg" ,   0, dir_1, &PAL_cppi4dbg_fops,    (void *)&puma7_pp_dbg_cfg))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create_data("dbgCfg" ,0, dir_1, &PAL_cppi4dbgCfg_fops, (void *)&puma7_pp_dbg_cfg))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create     ("pmInfo" ,0, dir_1, &PAL_cppi4PM_fops)))                                {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create     ("qsm" ,   0, dir_1, &cppi_qsm_proc_fops)))                              {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create     ("qsmcfg" ,0, dir_1, &cppi_qsmcfg_proc_fops)))                           {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create     ("bsm" ,   0, dir_1, &cppi_bsm_proc_fops)))                              {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }

    if (NULL == (dir_1 = proc_mkdir("stats",    dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create("all" ,0, dir_1, &cppi_pp_proc_fops))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }

    return (0);
}



int cppi_dsg_dump_all_stats(struct seq_file *m, void *v)
{
    PAL_CPPI_DSG_QUEUE_MNGRS_e qMgr = (PAL_CPPI_DSG_QUEUE_MNGRS_e)m->private;
    Uint16              qNum;
    Cppi4Queue          cppiQueue;
    Uint32              pktCount;
    Uint32              expectedCount;
    Uint32              i;

    for (qNum = 0; qNum < PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT; qNum++)
    {
        cppiQueue.qMgr = qMgr;
        cppiQueue.qNum = qNum;
        PAL_cppi4Control(gDsgPalHnd, PAL_CPPI41_IOCTL_GET_QUEUE_ENTRY_COUNT, &cppiQueue, &pktCount);

        expectedCount = 0;

        for (i = 0; i < ARRAY_SIZE(gDsgFDqueues); i++)
        {
            if (gDsgFDqueues[i].qMgr == qMgr && gDsgFDqueues[i].qId == qNum)
            {
                expectedCount = gDsgFDqueues[i].descCount;
                break;
            }
        }

        if (pktCount != expectedCount)
        {
            seq_printf(m, "%4d %-65s : %4d [%d]\n", qNum, PAL_CPPI_DSG_QMGR_GET_Q_NAME(qMgr, qNum), pktCount, expectedCount);
        }
    }

    return (0);
}

static int cppi_dsg_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, cppi_dsg_dump_all_stats, PDE_DATA(file_inode(file)));
}

static const struct file_operations cppi_dsg_proc_fops =
{
    .open     = cppi_dsg_proc_open,
    .read     = seq_read,
    .llseek   = seq_lseek,
    .release  = single_release,
};



static PAL_Result cppi_dsg_proc_init(Ptr hnd, Ptr param)
{
    struct proc_dir_entry *dir_1;

    gDsgPalHnd = (PAL_Handle)hnd;

    dir_1 = (struct proc_dir_entry *)param;
    if (NULL == (dir_1 = proc_mkdir("dsg0", dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (dir_1 = proc_mkdir("stats",    dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create_data("all", 0, dir_1, &cppi_dsg_proc_fops, (void *)0 ))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }

#ifdef CONFIG_MACH_PUMA7_BOARD
    dir_1 = (struct proc_dir_entry *)param;
    if (NULL == (dir_1 = proc_mkdir("dsg1", dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (dir_1 = proc_mkdir("stats",    dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create_data("all", 0, dir_1, &cppi_dsg_proc_fops, (void *)1 ))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
#endif

    dir_1 = (struct proc_dir_entry *)param;
    if (NULL == (dir_1 = proc_mkdir("dsg2", dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (dir_1 = proc_mkdir("stats",    dir_1))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }
    if (NULL == (proc_create_data("all", 0, dir_1, &cppi_dsg_proc_fops, (void *)2 ))) {   printk("%s:%d ERROR ....\n",__FUNCTION__,__LINE__); return -1;  }

    return (0);
}

void avalanche_cppi_init_pp_queue_managers(void)
{
    /* LQMGR */
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].queueMgrRgnBase                     = (Ptr) PAL_CPPI_PP_QMGR_LOCAL_REGS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].descMemRgnBase                      = (Ptr) PAL_CPPI_PP_QMGR_LOCAL_DESC_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].queueMgmtRgnBase                    = (Ptr) PAL_CPPI_PP_QMGR_LOCAL_QUEUES_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].queueStatusRgnBase                  = (Ptr) PAL_CPPI_PP_QMGR_LOCAL_Q_STATS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].queueProxyRgnBase                   = NULL;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].totalQNum                           = PAL_CPPI_PP_QMGR_LOCAL_TOTAL_Q_COUNT;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].LinkingRAM0Base                     = PAL_CPPI_PP_QMGR_LOCAL_LINKING_RAM_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].LinkingRAM0Size                     = PAL_CPPI_PP_QMGR_LOCAL_LINKING_RAM_SIZE;

    /* GQMGR0 */
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].queueMgrRgnBase                        = (Ptr) PAL_CPPI_PP_QMGR_G0_REGS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].descMemRgnBase                         = (Ptr) PAL_CPPI_PP_QMGR_G0_DESC_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].queueMgmtRgnBase                       = (Ptr) PAL_CPPI_PP_QMGR_G0_QUEUES_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].queueStatusRgnBase                     = (Ptr) PAL_CPPI_PP_QMGR_G0_Q_STATS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].queueProxyRgnBase                      = NULL;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].totalQNum                              = PAL_CPPI_PP_QMGR_G0_TOTAL_Q_COUNT;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].LinkingRAM0Base                        = PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G0].LinkingRAM0Size                        = PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_SIZE;

    /* GQMGR1 */
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].queueMgrRgnBase                        = (Ptr) PAL_CPPI_PP_QMGR_G1_REGS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].descMemRgnBase                         = (Ptr) PAL_CPPI_PP_QMGR_G1_DESC_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].queueMgmtRgnBase                       = (Ptr) PAL_CPPI_PP_QMGR_G1_QUEUES_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].queueStatusRgnBase                     = (Ptr) PAL_CPPI_PP_QMGR_G1_Q_STATS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].queueProxyRgnBase                      = (Ptr) PAL_CPPI_PP_QMGR_G1_Q_PROXY_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].totalQNum                              = PAL_CPPI_PP_QMGR_G1_TOTAL_Q_COUNT;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].LinkingRAM0Base                        = PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G1].LinkingRAM0Size                        = PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_SIZE;

    /* GQMGR2 */
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].queueMgrRgnBase                        = (Ptr) PAL_CPPI_PP_QMGR_G2_REGS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].descMemRgnBase                         = (Ptr) PAL_CPPI_PP_QMGR_G2_DESC_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].queueMgmtRgnBase                       = (Ptr) PAL_CPPI_PP_QMGR_G2_QUEUES_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].queueStatusRgnBase                     = (Ptr) PAL_CPPI_PP_QMGR_G2_Q_STATS_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].queueProxyRgnBase                      = (Ptr) PAL_CPPI_PP_QMGR_G2_Q_PROXY_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].totalQNum                              = PAL_CPPI_PP_QMGR_G2_TOTAL_Q_COUNT;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].LinkingRAM0Base                        = PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_BASE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_G2].LinkingRAM0Size                        = PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_SIZE;
}


void avalanche_cppi_init_pp_queue_status_monitors_managers(void)
{
    gCppiPpInitCfg.qsmon[PAL_CPPI_PP_QSMON_MGR0]                                            = (CSL_QSMon_RegsOvly)PAL_CPPI_QSMON_0_CTRL_REGS_BASE;
    gCppiPpInitCfg.qsmon[PAL_CPPI_PP_QSMON_MGR1]                                            = (CSL_QSMon_RegsOvly)PAL_CPPI_QSMON_1_CTRL_REGS_BASE;
    gCppiPpInitCfg.qsmon[PAL_CPPI_PP_QSMON_MGR2]                                            = (CSL_QSMon_RegsOvly)PAL_CPPI_QSMON_2_CTRL_REGS_BASE;
    gCppiPpInitCfg.qsmon[PAL_CPPI_PP_QSMON_MGR3]                                            = (CSL_QSMon_RegsOvly)PAL_CPPI_QSMON_3_CTRL_REGS_BASE;
    gCppiPpInitCfg.qsmon[PAL_CPPI_PP_QSMON_MGR4]                                            = (CSL_QSMon_RegsOvly)PAL_CPPI_QSMON_4_CTRL_REGS_BASE;
    gCppiPpInitCfg.qsmon[PAL_CPPI_PP_BSMON_MGR]                                             = (CSL_QSMon_RegsOvly)PAL_CPPI_BSMON_CTRL_REGS_BASE;
}



Int32 avalanche_cppi_init_pp_descriptor_regions(void)
{
    Uint32 i;
#ifdef CONFIG_INTEL_KERNEL_VOICE_SUPPORT
    Uint8  memAllocated = 0;
    Uint32 *poolPhysAddr;
#endif

    /* LQMGR */
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].descRegion[PAL_CPPI_PP_INTERNAL_EMB_LOCAL_DESC_REGION].base     = (Ptr) IO_VIRT2PHY(PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_BASE);
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].descRegion[PAL_CPPI_PP_INTERNAL_EMB_LOCAL_DESC_REGION].numDesc  = PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_COUNT;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].descRegion[PAL_CPPI_PP_INTERNAL_EMB_LOCAL_DESC_REGION].szDesc   = PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_SIZE;
    gCppiPpInitCfg.queueMgrInfo[PAL_CPPI_PP_QMGR_LOCAL].descRegion[PAL_CPPI_PP_INTERNAL_EMB_LOCAL_DESC_REGION].isOnChip = 1;

    /* GQMGRs */
    for (i = PAL_CPPI_PP_QMGR_G0; i <= PAL_CPPI_PP_QMGR_G2; i++)
    {
        switch (i)
        {
#if !defined(CONFIG_MACH_PUMA7_FPGA_PP) && !defined (CONFIG_MACH_PUMA7_BOARD)
        case PAL_CPPI_PP_QMGR_G0:       continue;
#ifndef CONFIG_MACH_PUMA7_FPGA_US
        case PAL_CPPI_PP_QMGR_G1:       continue;
#endif
#endif
        default: break;
        }


        gCppiPpInitCfg.queueMgrInfo[i].descRegion[ PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION     ].isOnChip       =   1;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[ PAL_CPPI_PP_IPSEC_MONOLITHIC_GLOBAL_DESC_REGION ].isOnChip       =   1;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[ PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION       ].isOnChip       =   1;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[ PAL_CPPI_PP_US_FW_MONO_DESC_REGION              ].isOnChip       =   1;

        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION].base                 =   (Ptr) IO_VIRT2PHY(PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_BASE);
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION].numDesc              =   PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_COUNT;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION].szDesc               =   PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_SIZE;

        /* IPSEC Monolithic */
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_IPSEC_MONOLITHIC_GLOBAL_DESC_REGION].base             =   (Ptr)IO_VIRT2PHY(PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_BASE);
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_IPSEC_MONOLITHIC_GLOBAL_DESC_REGION].numDesc          =   PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_DESC_COUNT;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_IPSEC_MONOLITHIC_GLOBAL_DESC_REGION].szDesc           =   PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_DESC_SIZE;

        /* Packet RAM */
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION].base                   =   (Ptr) IO_VIRT2PHY( PAL_CPPI_PP_PACKET_RAM_SHARED_FD_BASE );
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION].numDesc                =   PAL_CPPI_PP_PACKET_RAM_SHARED_FD_DESC_COUNT;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION].szDesc                 =   PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE;

        /* US FW */
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_US_FW_MONO_DESC_REGION].base                          =   (Ptr) IO_VIRT2PHY( PAL_CPPI_PP_US_FW_MONO_FD_BASE );
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_US_FW_MONO_DESC_REGION].numDesc                       =   PAL_CPPI_PP_US_FW_MONO_FD_DESC_COUNT;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_US_FW_MONO_DESC_REGION].szDesc                        =   PAL_CPPI_PP_US_FW_MONO_FD_DESC_SIZE;

        /* RX Short + Non DOCSIS Infrastructures */
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION].szDesc  =   PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION].numDesc =   PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT           +
                                                                                                                        PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT     +
                                                                                                                        PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT     +
                                                                                                                        PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT   +
                                                                                                                        PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT   +
                                                                                                                        PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT   +
                                                                                                                        PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT   +
                                                                                                                        PAL_CPPI_PP_SHARED_LOW_INFRA_HOST_FD_DESC_COUNT                 +
                                                                                                                        PAL_CPPI_PP_ATOM_HI_INFRA_HOST_FD_DESC_COUNT                    +
                                                                                                                        PAL_CPPI_PP_RGMII0_HI_INFRA_HOST_FD_DESC_COUNT                  +
                                                                                                                        PAL_CPPI_PP_RGMII1_HI_INFRA_HOST_FD_DESC_COUNT                  +
                                                                                                                        PAL_CPPI_PP_SGMII0_HI_INFRA_HOST_FD_DESC_COUNT                  +
                                                                                                                        PAL_CPPI_PP_SGMII1_HI_INFRA_HOST_FD_DESC_COUNT                  +
#ifdef CONFIG_WIFI_PROXY
                                                                                                                        PAL_CPPI_PP_WIFI_INFRA_HOST_RX_FD_DESC_COUNT                    +
#endif
#if CONFIG_NP_APP_DATAPIPE
                                                                                                                        PAL_CPPI_PP_NP2APP_HOST_FD_DESC_COUNT                           +
                                                                                                                        PAL_CPPI_PP_NP2APP_INFRA_PPINFO_HOST_FD_DESC_COUNT              +
                                                                                                                        PAL_CPPI_PP_NP2APP_INFRA_DATA_HOST_FD_DESC_COUNT                +
                                                                                                                        PAL_CPPI_PP_APP2NP_HOST_FD_DESC_COUNT                           +
                                                                                                                        PAL_CPPI_PP_APP2NP_INFRA_PPINFO_HOST_FD_DESC_COUNT              +
                                                                                                                        PAL_CPPI_PP_APP2NP_INFRA_DATA_HOST_FD_DESC_COUNT                +
#endif
                                                                                                                        PAL_CPPI_PP_HOST2PP_LOW_INFRA_EMB_FD_DESC_COUNT                 +
                                                                                                                        PAL_CPPI_PP_HOST2PP_HI_INFRA_EMB_FD_DESC_COUNT                  +
                                                                                                                        PAL_CPPI_PP_HOST2PP_LOW_HOST_FD_DESC_COUNT                      +
                                                                                                                        PAL_CPPI_PP_HOST2PP_HI_HOST_FD_DESC_COUNT                       + 
                                                                                                                        PAL_CPPI_PP_IPSEC_HOST_FD_DESC_COUNT;


        /* RX Long + RX XLong + DOCSIS */
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION].szDesc       =   PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE;
#if !defined(CONFIG_MACH_PUMA7_FPGA_PP) && !defined (CONFIG_MACH_PUMA7_BOARD)
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION].numDesc      =   PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT            +
                                                                                                                        PAL_CPPI_PP_DOCSIS_RX_MGMT_HOST_FD_DESC_COUNT;
#else
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION].numDesc      =   PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT            +
                                                                                                                        PAL_CPPI_PP_RX_LOW_XLONG_SHARED_DDR_EMB_FD_DESC_COUNT           +
                                                                                                                        PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT      +
                                                                                                                        PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT      +
                                                                                                                        PAL_CPPI_PP_ATOM_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT          +
                                                                                                                        PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT    +
                                                                                                                        PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT        +
                                                                                                                        PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT    +
                                                                                                                        PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT        +
                                                                                                                        PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT    +
                                                                                                                        PAL_CPPI_PP_SGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT        +
                                                                                                                        PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT    +
                                                                                                                        PAL_CPPI_PP_SGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT        +
                                                                                                                        PAL_CPPI_PP_DOCSIS_RX_MGMT_HOST_FD_DESC_COUNT                   +
                                                                                                                        PAL_CPPI_PP_DOCSIS_TX_MGMT_HOST_FD_DESC_COUNT                   +
                                                                                                                        PAL_CPPI_PP_DOCSIS_LOW_INFRA_HOST_FD_DESC_COUNT                 +
                                                                                                                        PAL_CPPI_PP_DOCSIS_HI_INFRA_HOST_FD_DESC_COUNT                  +
#ifdef CONFIG_WIFI_PROXY
                                                                                                                        PAL_CPPI_PP_WIFI_FW_RX_EMB_FD_DESC_COUNT                        +
                                                                                                                        // PAL_CPPI_PP_WIFI_FW_RX_REASSEMBLY_EMB_FD_DESC_COUNT             +
#endif
                                                                                                                        PAL_CPPI_PP_DOCSIS_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT;
#endif

        /* Teardown */
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_TEARDOWN_GLOBAL_DESC_REGION].szDesc                    =  PAL_CPPI_PP_TEARDOWN_FD_DESC_SIZE;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_TEARDOWN_GLOBAL_DESC_REGION].numDesc                   =  PAL_CPPI_PP_TEARDOWN_FD_DESC_COUNT;

        /* VOICE DSP */
#ifdef CONFIG_INTEL_KERNEL_VOICE_SUPPORT
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION].szDesc                       =   PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION].numDesc                      =   PAL_CPPI_PP_VOICE_DSP_RX_EMB_FD_DESC_COUNT +
                                                                                                                         PAL_CPPI_PP_VOICE_INFRA_RX_EMB_FD_DESC_COUNT;
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION].isPreAllocated               =   True;
        if (!memAllocated)
        {
            memAllocated = 1;

            if (avalanche_alloc_no_OperSys_memory(eNO_OperSys_VoiceNI, PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE * PAL_CPPI_PP_VOICE_DSP_C55_EMB_BD_COUNT, (Uint32 *)&poolPhysAddr) != 0)
            {
                printk(KERN_ERR "ERROR: avalanche_alloc_no_OperSys_memory for descriptors failed\n");
                return -1;
            }
        }
        gCppiPpInitCfg.queueMgrInfo[i].descRegion[PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION].base                        =   poolPhysAddr;
#endif
    }

    return 0;
}


void avalanche_cppi_init_pp_dmas(void)
{
    Uint32 i;

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA00_OFDM0_RX].globalCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA00_OFDM0_RX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA00_OFDM0_RX].chCtrlStatusBase                    = (Ptr) PAL_CPPI_PP_DMA00_OFDM0_RX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA00_OFDM0_RX].schedCtrlBase                       = (Ptr) PAL_CPPI_PP_DMA00_OFDM0_RX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA00_OFDM0_RX].ralCtrlBase                         = (Ptr) PAL_CPPI_PP_DMA00_OFDM0_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA00_OFDM0_RX].schedParams.portNum                 = PAL_CPPI_PP_OFDM0_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA00_OFDM0_RX].schedParams.channelArbitrationMode  = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA00_OFDM0_RX].schedParams.rxTxArbitrationMode     = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;


#ifdef CONFIG_MACH_PUMA7_BOARD
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA01_OFDM1_RX].globalCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA01_OFDM1_RX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA01_OFDM1_RX].chCtrlStatusBase                    = (Ptr) PAL_CPPI_PP_DMA01_OFDM1_RX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA01_OFDM1_RX].schedCtrlBase                       = (Ptr) PAL_CPPI_PP_DMA01_OFDM1_RX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA01_OFDM1_RX].ralCtrlBase                         = (Ptr) PAL_CPPI_PP_DMA01_OFDM1_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA01_OFDM1_RX].schedParams.portNum                 = PAL_CPPI_PP_OFDM1_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA01_OFDM1_RX].schedParams.channelArbitrationMode  = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA01_OFDM1_RX].schedParams.rxTxArbitrationMode     = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;
#endif

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA02_SC_QAM_RX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA02_SC_QAM_RX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA02_SC_QAM_RX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA02_SC_QAM_RX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA02_SC_QAM_RX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA02_SC_QAM_RX].ralCtrlBase                        = (Ptr) PAL_CPPI_PP_DMA02_SC_QAM_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA02_SC_QAM_RX].schedParams.portNum                = PAL_CPPI_PP_SC_QAM_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA02_SC_QAM_RX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA02_SC_QAM_RX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

#ifdef CONFIG_MACH_PUMA7_BOARD
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA03_SGMII0_RX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA03_SGMII0_RX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA03_SGMII0_RX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA03_SGMII0_RX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA03_SGMII0_RX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA03_SGMII0_RX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA03_SGMII0_RX].ralCtrlBase                        = (Ptr) PAL_CPPI_PP_DMA03_SGMII0_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA03_SGMII0_RX].schedParams.portNum                = PAL_CPPI_PP_SGMII0_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA03_SGMII0_RX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA03_SGMII0_RX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;


    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA04_SGMII1_RX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA04_SGMII1_RX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA04_SGMII1_RX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA04_SGMII1_RX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA04_SGMII1_RX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA04_SGMII1_RX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA04_SGMII1_RX].ralCtrlBase                        = (Ptr) PAL_CPPI_PP_DMA04_SGMII1_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA04_SGMII1_RX].schedParams.portNum                = PAL_CPPI_PP_SGMII1_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA04_SGMII1_RX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA04_SGMII1_RX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

#endif

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA05_RGMII0_RX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA05_RGMII0_RX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA05_RGMII0_RX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA05_RGMII0_RX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA05_RGMII0_RX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA05_RGMII0_RX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA05_RGMII0_RX].ralCtrlBase                        = (Ptr) PAL_CPPI_PP_DMA05_RGMII0_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA05_RGMII0_RX].schedParams.portNum                = PAL_CPPI_PP_RGMII0_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA05_RGMII0_RX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA05_RGMII0_RX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;


#ifdef CONFIG_MACH_PUMA7_BOARD
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].globalCtrlBase                    = (Ptr) PAL_CPPI_PP_DMA06_MoCA_RX_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].chCtrlStatusBase                  = (Ptr) PAL_CPPI_PP_DMA06_MoCA_RX_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].schedCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA06_MoCA_RX_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].ralCtrlBase                       = (Ptr) PAL_CPPI_PP_DMA06_MoCA_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].schedParams.portNum               = PAL_CPPI_PP_MoCA_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].schedParams.noRecycleOnTx         = 1; /* For MoCA, we do not want the CDMA to do any recycling */
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA06_MoCA_RX_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;
#endif

#ifdef CONFIG_MACH_PUMA7_BOARD
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA07_ATOM_RX_TX].globalCtrlBase                    = (Ptr) PAL_CPPI_PP_DMA07_ATOM_RX_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA07_ATOM_RX_TX].chCtrlStatusBase                  = (Ptr) PAL_CPPI_PP_DMA07_ATOM_RX_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA07_ATOM_RX_TX].schedCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA07_ATOM_RX_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA07_ATOM_RX_TX].ralCtrlBase                       = (Ptr) PAL_CPPI_PP_DMA07_ATOM_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA07_ATOM_RX_TX].schedParams.portNum               = PAL_CPPI_PP_ATOM_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA07_ATOM_RX_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA07_ATOM_RX_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;


    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX].globalCtrlBase    = (Ptr) PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX].chCtrlStatusBase  = (Ptr) PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX].schedCtrlBase     = (Ptr) PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;


    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA09_INFRA_RX_TX].globalCtrlBase                   = (Ptr) PAL_CPPI_PP_DMA09_INFRA_RX_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA09_INFRA_RX_TX].chCtrlStatusBase                 = (Ptr) PAL_CPPI_PP_DMA09_INFRA_RX_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA09_INFRA_RX_TX].schedCtrlBase                    = (Ptr) PAL_CPPI_PP_DMA09_INFRA_RX_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA09_INFRA_RX_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA09_INFRA_RX_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;
#endif

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA10_INFRA_RX_TX].globalCtrlBase                   = (Ptr) PAL_CPPI_PP_DMA10_INFRA_RX_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA10_INFRA_RX_TX].chCtrlStatusBase                 = (Ptr) PAL_CPPI_PP_DMA10_INFRA_RX_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA10_INFRA_RX_TX].schedCtrlBase                    = (Ptr) PAL_CPPI_PP_DMA10_INFRA_RX_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA10_INFRA_RX_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA10_INFRA_RX_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;


#ifdef CONFIG_MACH_PUMA7_BOARD
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA11_INFRA_RX_TX].globalCtrlBase                   = (Ptr) PAL_CPPI_PP_DMA11_INFRA_RX_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA11_INFRA_RX_TX].chCtrlStatusBase                 = (Ptr) PAL_CPPI_PP_DMA11_INFRA_RX_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA11_INFRA_RX_TX].schedCtrlBase                    = (Ptr) PAL_CPPI_PP_DMA11_INFRA_RX_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA11_INFRA_RX_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA11_INFRA_RX_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;
#endif

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX].globalCtrlBase           = (Ptr) PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX].chCtrlStatusBase         = (Ptr) PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX].schedCtrlBase            = (Ptr) PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

#ifdef CONFIG_MACH_PUMA7_BOARD
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX].globalCtrlBase           = (Ptr) PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX].chCtrlStatusBase         = (Ptr) PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX].schedCtrlBase            = (Ptr) PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA14_SGMII0_TX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA14_SGMII0_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA14_SGMII0_TX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA14_SGMII0_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA14_SGMII0_TX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA14_SGMII0_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA14_SGMII0_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA14_SGMII0_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA15_SGMII1_TX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA15_SGMII1_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA15_SGMII1_TX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA15_SGMII1_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA15_SGMII1_TX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA15_SGMII1_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA15_SGMII1_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA15_SGMII1_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;
#endif

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA16_RGMII0_TX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA16_RGMII0_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA16_RGMII0_TX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA16_RGMII0_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA16_RGMII0_TX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA16_RGMII0_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA16_RGMII0_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA16_RGMII0_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA21_US_COP_RX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA21_US_COP_RX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA21_US_COP_RX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA21_US_COP_RX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA21_US_COP_RX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA21_US_COP_RX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA21_US_COP_RX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA21_US_COP_RX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA22_US_COP_TX].globalCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA22_US_COP_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA22_US_COP_TX].chCtrlStatusBase                   = (Ptr) PAL_CPPI_PP_DMA22_US_COP_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA22_US_COP_TX].schedCtrlBase                      = (Ptr) PAL_CPPI_PP_DMA22_US_COP_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA22_US_COP_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA22_US_COP_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA23_RGMII1_RX_TX].globalCtrlBase                  = (Ptr) PAL_CPPI_PP_DMA23_RGMII1_RX_TX_GLOBAL_CTRL_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA23_RGMII1_RX_TX].chCtrlStatusBase                = (Ptr) PAL_CPPI_PP_DMA23_RGMII1_RX_TX_CH_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA23_RGMII1_RX_TX].schedCtrlBase                   = (Ptr) PAL_CPPI_PP_DMA23_RGMII1_RX_TX_SCHEDULER_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA23_RGMII1_RX_TX].ralCtrlBase                     = (Ptr) PAL_CPPI_PP_DMA23_RGMII1_RX_RAL_CFG_BASE;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA23_RGMII1_RX_TX].schedParams.portNum             = PAL_CPPI_PP_RGMII1_RX_SOURCE_PORT;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA23_RGMII1_RX_TX].schedParams.channelArbitrationMode = DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR;
    gCppiPpInitCfg.dmaBlock[PAL_CPPI_PP_DMA23_RGMII1_RX_TX].schedParams.rxTxArbitrationMode    = DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR;

    for (i = 0; i < PAL_CPPI41_NUM_DMA_BLOCK; i++)
    {
        gCppiPpInitCfg.dmaBlock[i].tdFQueue.qMgr                                            = PAL_CPPI_PP_QMGR_G2;
        gCppiPpInitCfg.dmaBlock[i].tdFQueue.qNum                                            = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    }
}

Uint32 avalanche_cppi_init_pp_buffer_pools(PAL_Handle palHandle)
{
    Cppi4BufPool        tmpBufPool;

    tmpBufPool.bMgr = PAL_CPPI_PP_BUF_MGR;
    tmpBufPool.bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
    if((PAL_cppi4BufPoolInit(palHandle, tmpBufPool, PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_REF_CNT, PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_SIZE, PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_COUNT)) == NULL)
    {
        return (tmpBufPool.bPool + 1);
    }

    tmpBufPool.bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
    if((PAL_cppi4BufPoolInit(palHandle, tmpBufPool, PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_REF_CNT, PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_SIZE, PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_COUNT)) == NULL)
    {
        return (tmpBufPool.bPool + 1);
    }
    tmpBufPool.bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
    if((PAL_cppi4BufPoolInit(palHandle, tmpBufPool, PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_REF_CNT, PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_SIZE, PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_COUNT)) == NULL)
    {
        return (tmpBufPool.bPool + 1);
    }
    tmpBufPool.bPool = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL;
    if((PAL_cppi4BufPoolInit(palHandle, tmpBufPool, PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_REF_CNT, PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_SIZE, PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_COUNT)) == NULL)
    {
        return (tmpBufPool.bPool + 1);
    }    
#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
    tmpBufPool.bPool = PAL_CPPI_PP_WIFI_RX_POOL;
    if((PAL_cppi4BufPoolInit(palHandle, tmpBufPool, PAL_CPPI_PP_WIFI_RX_POOL_REF_CNT, PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_SIZE, PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_COUNT)) == NULL)
    {
        return (tmpBufPool.bPool + 1);
    }
#endif
    tmpBufPool.bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
    if((PAL_cppi4BufPoolDirectInit(palHandle, tmpBufPool, PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_REF_CNT, PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_SIZE, PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_COUNT, (Ptr)PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_BASE)) == NULL)
    {
        return (tmpBufPool.bPool + 1);
    }
    tmpBufPool.bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
    if((PAL_cppi4BufPoolDirectInit(palHandle, tmpBufPool, PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_REF_CNT, PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_SIZE, PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_COUNT, (Ptr)PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_BASE)) == NULL)
    {
        return (tmpBufPool.bPool + 1);
    }

    return 0;
}


u32 cppi_get_pp_free_queues_count(void)
{
    return ARRAY_SIZE(gPpFDqueues);
}

u32 cppi_get_pp_free_queue_info(u32 idx, FDqueue_t *info)
{
    if (!info)
        return EFAULT;
    if (ARRAY_SIZE(gPpFDqueues) <= idx)
        return EINVAL;

    memcpy (info, &gPpFDqueues[idx], sizeof(*info));
    return 0;
}

u32 cppi_get_buffer_pool_bsmon_info(u32 idx, qsmonRal_t *info)
{
    if (!info)
        return EFAULT;
    if (ARRAY_SIZE(gBsmon) <= idx)
        return EINVAL;

    memcpy (info, &gBsmon[idx], sizeof(*info));
    return 0;
}
Int32 avalanche_cppi_init_pp_free_q_descriptors(PAL_Handle palHandle)
{
    Uint32              i;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              descCount;
    Ptr                 currDescRegionPtr[PAL_CPPI41_MAX_DESC_REGIONS];

    for (i = 0; i < sizeof(gPpFDqueues)/sizeof(gPpFDqueues[0]); i++)     /* Go over all free decriptors queues list */
    {
        /************************************************************/
        /*********** Setup PP Internal descriptors ******************/
        /************************************************************/
        /* PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION */
        if (gPpFDqueues[i].descRegion == PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION)
        {
            Uint32 currDesc = (Uint32)PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_BASE;

            tmpQ.qMgr = gPpFDqueues[i].qMgr;
            tmpQ.qNum = gPpFDqueues[i].qId;
            tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
            if (tmpQHnd == NULL)
            {
                printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
                return -1;
            }
            DEBUG_PAL("Initialization of Free Descriptors for PAL_CPPI_PP_QMGR_LOCAL_PP_INTERNAL_EMB_FD_Q_NUM Q[%d]Mgr[%d], base address = 0x%X (PHY: 0x%0X), descCount = %d\n", gPpFDqueues[i].qId, gPpFDqueues[i].qMgr, currDesc,
                      (Ptr)IO_VIRT2PHY((Uint32)currDesc), gPpFDqueues[i].descCount);

            for (descCount = 0; descCount < gPpFDqueues[i].descCount; descCount++)
            {
                PAL_cppi4QueuePush(tmpQHnd, (Ptr)IO_VIRT2PHY((Uint32)currDesc), 0, 0);
                currDesc += gPpFDqueues[i].descSize;
            }
        }

        /********************************************************************************/
        /*********** Setup Packet RAM descriptors & Shared descriptors ******************/
        /********************************************************************************/
        else if (gPpFDqueues[i].descRegion == PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION                 ||
                 gPpFDqueues[i].descRegion == PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION  ||
                 gPpFDqueues[i].descRegion == PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION       ||
                 gPpFDqueues[i].descRegion == PAL_CPPI_PP_TEARDOWN_GLOBAL_DESC_REGION)
        {
            if (PAL_cppi4IsDescRegionAllocated(palHandle, gPpFDqueues[i].qMgr, gPpFDqueues[i].descRegion) == False)
            {
                /* This is the first queue within this region - allocate all the region now */
                currDescRegionPtr[gPpFDqueues[i].descRegion] = PAL_cppi4AllocDescRegion(palHandle, gPpFDqueues[i].qMgr, gPpFDqueues[i].descRegion);
                if (currDescRegionPtr[gPpFDqueues[i].descRegion] == NULL)
                {
                    printk(KERN_CRIT "Descriptor region %d allocation FAILED\n", gPpFDqueues[i].descRegion);
                    return -1;
                }
                /* if region memory is on chip */
                if (gCppiPpInitCfg.queueMgrInfo[gPpFDqueues[i].qMgr].descRegion[gPpFDqueues[i].descRegion].isOnChip)
                {
                    currDescRegionPtr[gPpFDqueues[i].descRegion] = (Ptr)IO_PHY2VIRT(currDescRegionPtr[gPpFDqueues[i].descRegion]);
                }

                descCount = gCppiPpInitCfg.queueMgrInfo[gPpFDqueues[i].qMgr].descRegion[gPpFDqueues[i].descRegion].numDesc;
                PAL_osMemSet(currDescRegionPtr[gPpFDqueues[i].descRegion], 0, descCount * gPpFDqueues[i].descSize);
                DEBUG_PAL("Descriptor region %d allocation, base address = %p, descCount = %d\n", gPpFDqueues[i].descRegion, currDescRegionPtr[gPpFDqueues[i].descRegion], descCount);
            }

            tmpQ.qMgr = gPpFDqueues[i].qMgr;
            tmpQ.qNum = gPpFDqueues[i].qId;
            tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
            if (tmpQHnd == NULL)
            {
                printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
                return -1;
            }

            DEBUG_PAL("Initialization of Free Descriptors Q[%d]Mgr[%d], base address = %p, descCount = %d\n", gPpFDqueues[i].qId, gPpFDqueues[i].qMgr, currDescRegionPtr[gPpFDqueues[i].descRegion], gPpFDqueues[i].descCount);

            if (gPpFDqueues[i].descType == CPPI41_DESC_TYPE_EMBEDDED)
            {
                for (descCount = 0; descCount < gPpFDqueues[i].descCount; descCount++)
                {
                    Cppi4EmbdDescPp* currDesc = (Cppi4EmbdDescPp*)currDescRegionPtr[gPpFDqueues[i].descRegion];

//                  if (gPpFDqueues[i].qId == PAL_CPPI_PP_QMGR_G2_WIFI_FW_RX_REASSEMBLY_EMB_FD_Q_NUM)
//                      /* for wifi reassembly we need embedded descriptor with 2 buffers slot */
//                      currDesc->descInfo = CPPI41_EM_DESCINFO_DTYPE_EMBEDDED | CPPI41_EM_DESCINFO_SLOTCNT_PP_WIFI_RX_REASS;
//                  else
                        currDesc->descInfo = CPPI41_EM_DESCINFO_DTYPE_EMBEDDED | CPPI41_EM_DESCINFO_SLOTCNT_PP;

                    currDesc->tagInfo  = 0x3FFF;
                    currDesc->pktInfo  = (PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH << CPPI41_EM_PKTINFO_PKTTYPE_SHIFT)               |
                                         (PAL_CPPI4_HOSTDESC_PKT_RETPLCY_UNLINKED << CPPI41_EM_PKTINFO_RETPOLICY_SHIFT)     |
                                         (tmpQ.qMgr << CPPI41_EM_PKTINFO_RETQMGR_SHIFT)                                     |
                                         (tmpQ.qNum << CPPI41_EM_PKTINFO_RETQ_SHIFT);

                    PAL_CPPI4_CACHE_WRITEBACK(currDesc, gPpFDqueues[i].descSize);
                    if (gPpFDqueues[i].descRegion == PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION)
                    {
                        PAL_cppi4QueuePush(tmpQHnd, (Ptr)IO_VIRT2PHY((Uint32)currDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(gPpFDqueues[i].descSize), 0);
                    }
                    else
                    {
                        PAL_cppi4QueuePush(tmpQHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)currDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(gPpFDqueues[i].descSize), 0);
                    }

                    currDescRegionPtr[gPpFDqueues[i].descRegion] = (Ptr)((Uint32)currDescRegionPtr[gPpFDqueues[i].descRegion] + gPpFDqueues[i].descSize);
                }
            }
            else if (gPpFDqueues[i].descType == CPPI41_DESC_TYPE_HOST)
            {
                Ptr currBuffer = NULL;

                if (PAL_CPPI_ALLOC_CALLOC == gPpFDqueues[i].allocType)
                {
                    if (PAL_osMemAlloc(0, gPpFDqueues[i].descCount * PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE, 0, (Ptr *)&currBuffer) != PAL_SOK)
                    {
                        printk("%s[%d]: Failed to allocate memory for Management buffer queue", __FUNCTION__, __LINE__);
                        return -1;
                    }
                    PAL_osMemSet(currBuffer, 0, gPpFDqueues[i].descCount * PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE);
                }

                #if !defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL)
                if (PAL_CPPI_ALLOC_SKB == gPpFDqueues[i].allocType)
                {
                    // Cancel the SKB allocation incase PP is not here ...
                    gPpFDqueues[i].allocType = PAL_CPPI_ALLOC_NONE;
                }
                #endif

                for (descCount = 0; descCount < gPpFDqueues[i].descCount; descCount++)
                {
                    Cppi4HostDescLinux* currDesc = (Cppi4HostDescLinux*)currDescRegionPtr[gPpFDqueues[i].descRegion];

                    currDesc->hw.descInfo = PAL_CPPI4_HOSTDESC_DESC_TYPE_HOST << PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT;
                    currDesc->hw.tagInfo  = 0x3FFF;
                    currDesc->hw.pktInfo  = (PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH << PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT)  |
                                            (tmpQ.qMgr << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)                     |
                                            (tmpQ.qNum << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT);

                    if ((PAL_CPPI_ALLOC_CALLOC == gPpFDqueues[i].allocType)&&(currBuffer != NULL))
                    {
                        currDesc->hw.orgBuffLen  = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE;
                        currDesc->hw.orgBufPtr   = PAL_CPPI4_VIRT_2_PHYS(currBuffer);
                        currBuffer = (Ptr)((Uint32)currBuffer + PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE);
                    }
                    else
                    if (PAL_CPPI_ALLOC_SKB == gPpFDqueues[i].allocType) 
                    {
                        struct sk_buff *skb;

                        skb = dev_alloc_skb(PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE);
                        if (NULL == skb)
                        {
                            printk("%s: The SKB allocation FAILED\n", __FUNCTION__);
                            return -1;
                        }

                        skb_reserve (skb, NET_IP_ALIGN);    /* 16 bit align the IP fields. */
                        currDesc->hw.orgBuffLen  = PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE - NET_IP_ALIGN;
                        currDesc->hw.orgBufPtr   = PAL_CPPI4_VIRT_2_PHYS(skb->data);
                        currDesc->skb = skb;
                    }
                    else
                    {
                        currDesc->hw.orgBuffLen = 0;
                        currDesc->hw.orgBufPtr  = NULL;
                        currDesc->skb           = NULL;
                    }

                    PAL_CPPI4_CACHE_WRITEBACK(currDesc, gPpFDqueues[i].descSize);
                    PAL_cppi4QueuePush(tmpQHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)currDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(gPpFDqueues[i].descSize), 0);
                    currDescRegionPtr[gPpFDqueues[i].descRegion] = (Ptr)((Uint32)currDescRegionPtr[gPpFDqueues[i].descRegion] + gPpFDqueues[i].descSize);
                }
            }
            else if (gPpFDqueues[i].descType == CPPI41_DESC_TYPE_TEARDOWN)
            {
                tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
                if (tmpQHnd == NULL)
                {
                    printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
                    return -1;
                }

                for (descCount = 0; descCount < gPpFDqueues[i].descCount; descCount++)
                {
                    Cppi4TeardownDesc *currDesc = (Cppi4TeardownDesc*)currDescRegionPtr[gPpFDqueues[i].descRegion];

                    PAL_CPPI4_CACHE_WRITEBACK(currDesc, gPpFDqueues[i].descSize);
                    PAL_cppi4QueuePush(tmpQHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS((Uint32)currDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(gPpFDqueues[i].descSize), 0);
                    currDescRegionPtr[gPpFDqueues[i].descRegion] = (Ptr)((Uint32)currDescRegionPtr[gPpFDqueues[i].descRegion] + gPpFDqueues[i].descSize);
                }
            }
        }
#ifdef CONFIG_IPSEC_SUPPORT
        /********************************************************************************/
        /***********   Setup Encrypt & Decrypt PDSP Free Q             ******************/
        /********************************************************************************/
        else if (gPpFDqueues[i].descRegion == PAL_CPPI_PP_IPSEC_MONOLITHIC_GLOBAL_DESC_REGION)
        {
            Cppi4EmbdDesc *currDesc = (Cppi4EmbdDesc*)PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_BASE;

            tmpQ.qMgr = gPpFDqueues[i].qMgr;
            tmpQ.qNum = gPpFDqueues[i].qId;
            tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
            if (tmpQHnd == NULL)
            {
                printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
                return 0;
            }

            DEBUG_PAL("Initialization of Free Descriptors Q[%d]Mgr[%d], base address = 0x%X, descCount = %d\n", gPpFDqueues[i].qId, gPpFDqueues[i].qMgr, PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_BASE, gPpFDqueues[i].descCount);

            for (descCount = 0; descCount < gPpFDqueues[i].descCount; descCount++)
            {
                PAL_cppi4QueuePush(tmpQHnd, (Ptr)IO_VIRT2PHY((Uint32)currDesc), 0, 0);
                currDesc = (Cppi4EmbdDesc*)((Uint32)currDesc + gPpFDqueues[i].descSize);
            }

        }
#endif
}

    return 0;
}

#define PAL_CPPI_HOST2PP_SHARED_BUFF_SOP_OFF   16

Int32 avalanche_cppi_init_pp_host2pp_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              i;

    /* Common initializations */
    rxCh.dmaNum             = PAL_CPPI_PP_DMA11_INFRA_RX_TX;
    rxCh.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.sopOffset          = PAL_CPPI_PP_START_HEADROOM_OFFSET;
    rxCh.retryOnStarvation  = 0;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr  = PAL_CPPI_PP_BUF_MGR;
    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
    txCh.dmaNum         = PAL_CPPI_PP_DMA11_INFRA_RX_TX;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;

    /* HOST2PP Infrastructure DMA */
    for (i = 0; i < PAL_CPPI_PP_HOST2PP_INFRA_DMA_CH_COUNT; i++)
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_HOST2PP_INFRA_INPUT_Q_NUM(i);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }

        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        tmpQ.qNum = PAL_CPPI_PP_HOST2PP_PP_INPUT_Q_NUM(i);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }

        /* Set up Rx channel */
        rxCh.chNum              = PAL_CPPI_PP_HOST2PP_INFRA_DMA_CH_NUM(i);
        rxCh.rxCompQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum   = PAL_CPPI_PP_HOST2PP_PP_INPUT_Q_NUM(i);
        rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_HOST2PP_INFRA_EMB_FD_Q_NUM(i);
        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

        /* Set up Tx channel */
        txCh.chNum = PAL_CPPI_PP_HOST2PP_INFRA_DMA_CH_NUM(i);
        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!txChHdl || !rxChHdl)
        {
            printk (KERN_CRIT "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, i);
            return -1;
        }

        /* Enable Tx-Rx channels */
        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;
}

#if CONFIG_NP_APP_DATAPIPE
Int32 avalanche_cppi_init_pp_data_pipe_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              i;

    /* Common initializations */
    rxCh.dmaNum             = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
    rxCh.defDescType        = CPPI41_DESC_TYPE_HOST;
    rxCh.sopOffset          = 0;
    rxCh.retryOnStarvation  = 0;
    txCh.dmaNum         = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;

    /**********************************/
    /* NP2APP Data Infrastructure DMA */
    /**********************************/

    /* Input Queue (High) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_HI_NP2APP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Input Queue (Low) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_NP2APP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* RX Complete Queue */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_RX_COMPLETE_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* TX Complete Queue */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_TX_COMPLETE_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Set up Rx channel */
    rxCh.chNum              = PAL_CPPI_PP_DMA10_NP2APP_INFRA_RX_CH;
    rxCh.rxCompQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qNum   = PAL_CPPI_PP_QMGR_G2_NP2APP_RX_COMPLETE_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_PPINFO_HOST_FD_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[1].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[1].qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_DATA_HOST_FD_Q_NUM;
    rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

    /* Set up Tx channel */
    txCh.chNum = PAL_CPPI_PP_DMA10_NP2APP_INFRA_TX_CH;
    txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

    if (!txChHdl || !rxChHdl)
    {
        printk (KERN_CRIT "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, i);
        return -1;
    }

    /* Enable Tx-Rx channels */
    PAL_cppi4EnableRxChannel(rxChHdl, NULL);
    PAL_cppi4EnableTxChannel(txChHdl, NULL);



   /**********************************/
    /* NP2APP Ctrl Infrastructure DMA */
    /**********************************/

    /* Input Ctrl Queue (High) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_CTRL_HI_NP2APP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Input Ctrl Queue (Low) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_CTRL_LOW_NP2APP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Set up Rx Ctrl channel */
    rxCh.chNum              = PAL_CPPI_PP_DMA10_NP2APP_INFRA_RX_CTRL_CH;
    rxCh.rxCompQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qNum   = PAL_CPPI_PP_QMGR_G2_NP2APP_RX_COMPLETE_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_CTRL_PPINFO_HOST_FD_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[1].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[1].qNum = PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_CTRL_DATA_HOST_FD_Q_NUM;
    rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

    /* Set up Tx Ctrl channel */
    txCh.chNum = PAL_CPPI_PP_DMA10_NP2APP_INFRA_CTRL_TX_CH;
    txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

    if (!txChHdl || !rxChHdl)
    {
        printk (KERN_CRIT "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, i);
        return -1;
    }

    /* Enable Tx-Rx channels */
    PAL_cppi4EnableRxChannel(rxChHdl, NULL);
    PAL_cppi4EnableTxChannel(txChHdl, NULL);



    /**********************************/
    /* APP2NP Data Infrastructure DMA */
    /**********************************/

    /* Input Queue (High) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_HI_APP2NP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Input Queue (Low) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_APP2NP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* RX Complete Queue */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_RX_COMPLETE_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* TX Complete Queue */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_TX_COMPLETE_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Set up Rx channel */
    rxCh.chNum              = PAL_CPPI_PP_DMA10_APP2NP_INFRA_RX_CH;
    rxCh.rxCompQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qNum   = PAL_CPPI_PP_QMGR_G2_APP2NP_RX_COMPLETE_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_PPINFO_HOST_FD_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[1].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[1].qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_DATA_HOST_FD_Q_NUM;
    rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

    /* Set up Tx channel */
    txCh.chNum = PAL_CPPI_PP_DMA10_APP2NP_INFRA_TX_CH;
    txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

    if (!txChHdl || !rxChHdl )
    {
        printk (KERN_CRIT "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, i);
        return -1;
    }

    /* Enable Tx-Rx Data channels */
    PAL_cppi4EnableRxChannel(rxChHdl, NULL);
    PAL_cppi4EnableTxChannel(txChHdl, NULL);



    /**********************************/
    /* APP2NP Ctrl Infrastructure DMA */
    /**********************************/

    /* Ctrl Input Queue (High) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_CTRL_HI_APP2NP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Ctrl Input Queue (Low) */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_CTRL_LOW_APP2NP_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Set up Rx Ctrl channel */
    rxCh.chNum              = PAL_CPPI_PP_DMA10_APP2NP_INFRA_RX_CTRL_CH;
    rxCh.rxCompQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qNum   = PAL_CPPI_PP_QMGR_G2_APP2NP_RX_HI_COMPLETE_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_CTRL_PPINFO_HOST_FD_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[1].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[1].qNum = PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_CTRL_DATA_HOST_FD_Q_NUM;
    rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

    /* Set up Tx channel */
    txCh.chNum = PAL_CPPI_PP_DMA10_APP2NP_INFRA_CTRL_TX_CH;
    txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

    if (!txChHdl || !rxChHdl )
    {
        printk (KERN_CRIT "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, i);
        return -1;
    }

    /* Enable Tx-Rx Data channels */
    PAL_cppi4EnableRxChannel(rxChHdl, NULL);
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    return 0;
}
#endif

#ifdef CONFIG_WIFI_PROXY

Int32 avalanche_cppi_init_pp_wifi_rx_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              i;

    /****************************/
    /* WIFI HOST infrastructure */
    /****************************/
    /* Common initializations */
    rxCh.dmaNum             = PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX; 
    rxCh.defDescType        = CPPI41_DESC_TYPE_HOST;
    rxCh.sopOffset          = 0;
    rxCh.retryOnStarvation  = 0;
    txCh.dmaNum             = PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX;
    txCh.tdQueue.qMgr       = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum       = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_QMGR_G2_WIFI_INFRA_HOST_RX_FD_Q_NUM;

    /****************************/
    /* WIFI HOST infrastructure */
    /****************************/
    FOR_EACH_WIFI_DEVICE_ID(i)
    {
        DEBUG_PAL("Wifi Device %d\n", i);
        /* output queue */
        tmpQ.qMgr = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH_OUT_Q_MGR;
        tmpQ.qNum = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH_OUT_Q(i);
        tmpQHnd   = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }

        /* rx dma channel */
        rxCh.chNum              = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH(i); 
        rxCh.rxCompQueue.qMgr   = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH_OUT_Q_MGR;
        rxCh.rxCompQueue.qNum   = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH_OUT_Q(i);
        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        DEBUG_PAL("\tHost rx infra rx channel %d: output queue = G%d.%d, free queue = G%d.%d\n", 
               rxCh.chNum,
               rxCh.rxCompQueue.qMgr, rxCh.rxCompQueue.qNum,
               rxCh.u.hostPktCfg.fdbQueue[0].qMgr, rxCh.u.hostPktCfg.fdbQueue[0].qNum);

        /* input queue */
        tmpQ.qMgr = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_TX_CH_INPUT_Q_MGR; 
        tmpQ.qNum = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_TX_CH_INPUT_Q(i);
        tmpQHnd   = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }
        /* tx dma channel */
        txCh.chNum = WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_TX_CH(i);
        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
        DEBUG_PAL("\tHost rx infra tx channel %d: input queue = G%d.%d\n", 
               txCh.chNum, tmpQ.qMgr, tmpQ.qNum);

        if (!txChHdl || !rxChHdl)
        {
            printk (KERN_CRIT "%s[%d]: wifi host rx infra channel setup failed for device %d\n", __FUNCTION__, __LINE__, i);
            return -1;
        }

        /* Enable Tx-Rx channels */
        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : avalanche_cppi_init_pp_wifi_tx_dma_channels(PAL_Handle palHandle)
 **************************************************************************
 * DESCRIPTION   :
 * The function initializes InfraDMA08 to handle MultiCast packets
 * intended for the WiFi Proxy TX in order to support Zero Copy feature.
 * RETURNS       :
 *      0  -   Success
 *      >0  -   Error
 **************************************************************************/
Int32 avalanche_cppi_init_pp_wifi_tx_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              i;

    /* Common initializations */
    rxCh.defDescType                        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.sopOffset                          = PAL_CPPI_PP_START_HEADROOM_OFFSET;
    rxCh.retryOnStarvation                  = 0;
    rxCh.u.embeddedPktCfg.numBufSlot        = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum        = 0;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr      = PAL_CPPI_PP_QMGR_G2; 
    rxCh.u.embeddedPktCfg.fdQueue.qNum      = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW; 
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr  = PAL_CPPI_PP_BUF_MGR;
    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;

    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;

    /*****************************/
    /* WIFI TX FW infrastructure */
    /*****************************/
    FOR_EACH_WIFI_DEVICE_ID(i)
    {
        DEBUG_PAL("Wifi Device %d\n", i);
        /* output queue */
        tmpQ.qMgr = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_RX_CH_OUT_Q_MGR;
        tmpQ.qNum = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_RX_CH_OUT_Q(i);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }

        /* Set up Rx channel */
        rxCh.rxCompQueue.qMgr   = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_RX_CH_OUT_Q_MGR;
        rxCh.rxCompQueue.qNum   = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_RX_CH_OUT_Q(i);

        /* Open rx channel on infraDMA 11 */
        rxCh.dmaNum     = PAL_CPPI_PP_DMA11_INFRA_RX_TX;
        rxCh.chNum      = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_11_RX_CH(i);
        rxChHdl         = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

        /* input queue */
        tmpQ.qMgr = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_TX_CH_INPUT_Q_MGR;
        tmpQ.qNum = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_TX_CH_INPUT_Q(i);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }

        /* Set up Tx channel */
        /* Open tx channel on infraDMA 11 */
        txCh.dmaNum = PAL_CPPI_PP_DMA11_INFRA_RX_TX;
        txCh.chNum  = WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_11_TX_CH(i);
        txChHdl     = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!txChHdl || !rxChHdl) 
        {
            printk (KERN_CRIT "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, i);
            return -1;
        }

        /* Enable Tx-Rx channels */
        PAL_cppi4EnableRxChannel(rxChHdl  , NULL);
        PAL_cppi4EnableTxChannel(txChHdl  , NULL);
    }

    return 0;
}
#endif

Int32 avalanche_cppi_init_pp_sanity_dma_channels(PAL_Handle palHandle, Cppi4Queue freeQ, Cppi4BufPool freeBuf)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              i;

    /* Set up Rx channel */
    rxCh.dmaNum                             = PAL_CPPI_PP_DMA09_INFRA_RX_TX;
    rxCh.chNum                              = PAL_CPPI_PP_DMA09_INFRA_RX_CH_2;
    rxCh.defDescType                        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.sopOffset                          = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation                  = 0;
    rxCh.u.embeddedPktCfg.numBufSlot        = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum        = 0;
    rxCh.u.embeddedPktCfg.fdQueue           = freeQ;
    rxCh.u.embeddedPktCfg.fBufPool[0]       = freeBuf;
    rxCh.rxCompQueue.qMgr                   = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qNum                   = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
    rxChHdl                                 = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

    DEBUG_PAL("\tTX FW infra rx channel %d: output queue = G%d.%d, free queue = G%d.%d, freeBuf mgr = %d pool = %d\n",
           rxCh.chNum,
           rxCh.rxCompQueue.qMgr, rxCh.rxCompQueue.qNum,
           rxCh.u.embeddedPktCfg.fdQueue.qMgr, rxCh.u.embeddedPktCfg.fdQueue.qNum,
           rxCh.u.embeddedPktCfg.fBufPool[0].bMgr, rxCh.u.embeddedPktCfg.fBufPool[0].bPool);

    /* input queue */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_2_HI_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }

    /* Set up Tx channel */
    txCh.dmaNum         = PAL_CPPI_PP_DMA09_INFRA_RX_TX; // TODO - return to prev config
    txCh.chNum          = PAL_CPPI_PP_DMA09_INFRA_TX_CH_2;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    txChHdl             = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

    DEBUG_PAL("\tTX FW infra tx channel %d: input queue = G%d.%d\n", 
              txCh.chNum, tmpQ.qMgr, tmpQ.qNum);

    if (!txChHdl || !rxChHdl ) 
    {
        printk (KERN_CRIT "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, i);
        return -1;
    }

    /* Enable Tx-Rx channels */
    PAL_cppi4EnableRxChannel(rxChHdl  , NULL);
    PAL_cppi4EnableTxChannel(txChHdl  , NULL);

    return 0;
}
EXPORT_SYMBOL(avalanche_cppi_init_pp_sanity_dma_channels);

volatile typedef struct moca_rx_cmd1_s
{
    Uint32    ps_flags:4,
              pkt_type:5,
              ps_word_cnt:5,
              desc_type:4,
              qMgr_idx:2,
              queue_idx:12;
}moca_rx_cmd1_t;

volatile typedef struct moca_rx_cfg0_s
{
    Uint32     res:29,
               no_ps_pop:1,
               discard_enable:1,
               ps_location_mode:1;
}moca_rx_cfg0_t;

volatile typedef struct moca_rx_cfg1_s
{
    Uint32     threshold_1:16,
               threshold_0:16;
}moca_rx_cfg1_t;

Int32 avalanche_cppi_init_pp_moca_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              chNum;
    moca_rx_cmd1_t      *moca_cmd;
    moca_rx_cfg0_t      *moca_cfg0;
    moca_rx_cfg1_t      *moca_cfg1;

    moca_cmd = PAL_CPPI_PP_DMA06_MoCA_RX_CMD1;
    moca_cfg0 = PAL_CPPI_PP_DMA06_MoCA_RX_CFG0;
    moca_cfg1 = PAL_CPPI_PP_DMA06_MoCA_RX_CFG1;

    /* MoCA_RX_CHs */
    rxCh.dmaNum = PAL_CPPI_PP_DMA06_MoCA_RX_TX;
    rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation = 0;
    rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;
    for (chNum = PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum < PAL_CPPI_PP_DMA06_MoCA_RX_CHANNELS_NUM; chNum++)
    {
        rxCh.chNum = chNum;
        switch (chNum)
        {
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_1KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_1KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_LONG_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_4KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
                break;
        }
        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return(-1);
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
    }

    moca_cmd->queue_idx = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
    moca_cmd->qMgr_idx  = PAL_CPPI_PP_QMGR_G2;
    moca_cmd->desc_type = CPPI41_DESC_TYPE_EMBEDDED;
    moca_cmd->pkt_type  = PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH;

    moca_cfg0->no_ps_pop = True;
    moca_cfg1->threshold_0 = g_threshold_0;
    moca_cfg1->threshold_1 = g_threshold_1;

    /* MoCA_TX_CHs */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_MoCA_TX_HI_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_MoCA_TX_LOW_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return -1;
    }
    txCh.dmaNum = PAL_CPPI_PP_DMA06_MoCA_RX_TX;
    txCh.chNum  = PAL_CPPI_PP_DMA06_MoCA_TX_CH;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    txChHdl     = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
    if(txChHdl == NULL)
    {
        printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, txChNum=%d\n", __FUNCTION__, __LINE__, txCh.dmaNum, txCh.chNum);
        return(-1);
    }
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    /* MoCA Infrastructlure DMA */
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_MoCA_Q_NUM;
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }
        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        tmpQ.qNum = PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_MoCA_Q_NUM;
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return -1;
        }
        /* Set up RX channel */
        rxCh.chNum            = PAL_CPPI_PP_DMA10_MoCA_INFRA_RX_CH;
        rxCh.dmaNum           = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        rxCh.defDescType      = CPPI41_DESC_TYPE_HOST;
        rxCh.sopOffset        = 0;
        rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_MoCA_Q_NUM;
        rxCh.retryOnStarvation = 0;
        rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM;

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return (-1);
        }

        /* Set up TX channel */
        txCh.chNum         = PAL_CPPI_PP_DMA10_MoCA_INFRA_TX_CH;
        txCh.dmaNum        = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        txCh.tdQueue.qMgr  = PAL_CPPI_PP_QMGR_G2;
        txCh.tdQueue.qNum  = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
        txCh.defDescType   = CPPI41_DESC_TYPE_EMBEDDED;

        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!rxChHdl || !txChHdl)
        {
            printk ("%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, txCh.chNum);
            return -1;
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;

}

#ifdef CONFIG_INTEL_PP_NO_MOCA_DESC
#define TRUE_IF_MOCA_EN False
#else
#define TRUE_IF_MOCA_EN True
#endif
qsmonRal_t mocaRalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_12_MoCA_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_13_MoCA_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_14_MoCA_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = TRUE_IF_MOCA_EN,
        .ralQEnable     = TRUE_IF_MOCA_EN
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = TRUE_IF_MOCA_EN
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_15_MoCA_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_16_MoCA_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_MoCA_DDR_LONG_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_17_MoCA_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = TRUE_IF_MOCA_EN,
        .ralQEnable     = TRUE_IF_MOCA_EN
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  None   */
        .fdQueue.qNum   = 0,
        .fdQueue.qMgr   = 0,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(mocaRalQsmon);

Int32 avalanche_cppi_init_pp_moca_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_PRIVATE_DDR; chNum++)
    {

        qsMonRalConfig = mocaRalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR3, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR3 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA06_MoCA_RX_TX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA06_MoCA_RX_TX, ralQenBitmap);
        return -1;
    }

    return 0;
}

Int32 avalanche_cppi_init_pp_atom_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              chNum;

    /* ATOM_RX_CHs */
    rxCh.dmaNum = PAL_CPPI_PP_DMA07_ATOM_RX_TX;
    rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation = 0;
    rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;

    for (chNum = PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum < PAL_CPPI_PP_DMA07_ATOM_RX_CHANNELS_NUM; chNum++)
    {
        rxCh.chNum = chNum;
        switch (chNum)
        {
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_1KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_1KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_4KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA07_ATOM_RX_CH_HIGH_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL;
                break;
        }

        if (chNum == PAL_CPPI_PP_DMA07_ATOM_RX_CH_HIGH_DDR)
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_HI_Q_NUM;
        }
        else
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
        }

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return(-1);
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
    }

    /* ATOM_TX_CHs */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_ATOM_TX_HI_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_ATOM_TX_LOW_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    txCh.dmaNum = PAL_CPPI_PP_DMA07_ATOM_RX_TX;
    txCh.chNum  = PAL_CPPI_PP_DMA07_ATOM_TX_CH;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    txChHdl     = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
    if(txChHdl == NULL)
    {
        printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, txChNum=%d\n", __FUNCTION__, __LINE__, txCh.dmaNum, txCh.chNum);
        return(-1);
    }
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    /* ATOM Infrastructure DMA */
    for (chNum = 0; chNum < PAL_CPPI_PP_ATOM_INFRA_DMA_CH_COUNT; chNum++)
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_ATOM_INFRA_INPUT_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        tmpQ.qNum = PAL_CPPI_PP_ATOM_HOST_RX_COMPLETE_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Set up RX channel */
        rxCh.chNum            = PAL_CPPI_PP_ATOM_INFRA_DMA_CH_NUM(chNum);
        rxCh.dmaNum           = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        rxCh.defDescType      = CPPI41_DESC_TYPE_HOST;
        rxCh.sopOffset        = 0;
        rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum = PAL_CPPI_PP_ATOM_HOST_RX_COMPLETE_Q_NUM(chNum);
        rxCh.retryOnStarvation = 0;
        rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_ATOM_INFRA_HOST_FD_Q_NUM(chNum);

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return (-1);
        }

        /* Set up TX channel */
        txCh.chNum         = PAL_CPPI_PP_ATOM_INFRA_DMA_CH_NUM(chNum);
        txCh.dmaNum        = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        txCh.tdQueue.qMgr  = PAL_CPPI_PP_QMGR_G2;
        txCh.tdQueue.qNum  = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
        txCh.defDescType   = CPPI41_DESC_TYPE_EMBEDDED;

        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!rxChHdl || !txChHdl)
        {
            printk ("%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, chNum);
            return(-1);
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;
}

qsmonRal_t atomRalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_6_ATOM_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_7_ATOM_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_8_ATOM_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = True,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_9_ATOM_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_10_ATOM_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_11_ATOM_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = True,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA07_ATOM_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(atomRalQsmon);

Int32 avalanche_cppi_init_pp_atom_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_PRIVATE_DDR; chNum++)
    {

        qsMonRalConfig = atomRalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR3, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR3 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA07_ATOM_RX_TX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA07_ATOM_RX_TX, ralQenBitmap);
        return -1;
    }

    return 0;
}



Int32 avalanche_cppi_init_pp_rgmii0_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              chNum;

    /* RGMII0_RX_CHs */
    rxCh.dmaNum = PAL_CPPI_PP_DMA05_RGMII0_RX;
    rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation = 0;
    rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;

    for (chNum = PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum < PAL_CPPI_PP_DMA05_RGMII0_RX_CHANNELS_NUM; chNum++)
    {
        rxCh.chNum = chNum;
        switch (chNum)
        {
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_1KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_1KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_4KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA05_RGMII0_RX_CH_HIGH_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL;
                break;
        }

        if (chNum == PAL_CPPI_PP_DMA05_RGMII0_RX_CH_HIGH_DDR)
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_HI_Q_NUM;
        }
        else
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
        }

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return(-1);
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
    }

    /* RGMII0_TX_CHs */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_RGMII0_TX_HI_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_RGMII0_TX_LOW_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    txCh.dmaNum = PAL_CPPI_PP_DMA16_RGMII0_TX;
    txCh.chNum  = PAL_CPPI_PP_DMA16_RGMII0_TX_CH;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    txChHdl     = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
    if(txChHdl == NULL)
    {
        printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, txChNum=%d\n", __FUNCTION__, __LINE__, txCh.dmaNum, txCh.chNum);
        return(-1);
    }
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    /* RGMII0 Infrastructure DMA */
    for (chNum = 0; chNum < PAL_CPPI_PP_RGMII0_INFRA_DMA_CH_COUNT; chNum++)
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_RGMII0_INFRA_INPUT_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        tmpQ.qNum = PAL_CPPI_PP_RGMII0_HOST_RX_COMPLETE_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Set up RX channel */
        rxCh.chNum            = PAL_CPPI_PP_RGMII0_INFRA_DMA_CH_NUM(chNum);
        rxCh.dmaNum           = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        rxCh.defDescType      = CPPI41_DESC_TYPE_HOST;
        rxCh.sopOffset        = 0;
        rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum = PAL_CPPI_PP_RGMII0_HOST_RX_COMPLETE_Q_NUM(chNum);
        rxCh.retryOnStarvation = 0;
        rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_RGMII0_INFRA_HOST_FD_Q_NUM(chNum);

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return (-1);
        }

        /* Set up TX channel */
        txCh.chNum         = PAL_CPPI_PP_RGMII0_INFRA_DMA_CH_NUM(chNum);
        txCh.dmaNum        = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        txCh.tdQueue.qMgr  = PAL_CPPI_PP_QMGR_G2;
        txCh.tdQueue.qNum  = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
        txCh.defDescType   = CPPI41_DESC_TYPE_EMBEDDED;

        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!rxChHdl || !txChHdl)
        {
            printk ("%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, chNum);
            return -1;
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;
}

#ifdef CONFIG_INTEL_PP_NO_NRGMII0_DESC
#define TRUE_IF_RGMII0_EN False
#else
#define TRUE_IF_RGMII0_EN True
#endif
qsmonRal_t rgmii0RalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_18_RGMII0_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_19_RGMII0_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_20_RGMII0_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = TRUE_IF_RGMII0_EN,
        .ralQEnable     = TRUE_IF_RGMII0_EN
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = TRUE_IF_RGMII0_EN
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_21_RGMII0_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_22_RGMII0_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_23_RGMII0_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = TRUE_IF_RGMII0_EN,
        .ralQEnable     = TRUE_IF_RGMII0_EN
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA05_RGMII0_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(rgmii0RalQsmon);

Int32 avalanche_cppi_init_pp_rgmii0_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_PRIVATE_DDR; chNum++)
    {

        qsMonRalConfig = rgmii0RalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR3, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR3 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA05_RGMII0_RX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA05_RGMII0_RX, ralQenBitmap);
        return -1;
    }

    return 0;
}


Int32 avalanche_cppi_init_pp_rgmii1_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              chNum;

    /* RGMII1_RX_CHs */
    rxCh.dmaNum = PAL_CPPI_PP_DMA23_RGMII1_RX_TX;
    rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation = 0;
    rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;

    for (chNum = PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum < PAL_CPPI_PP_DMA23_RGMII1_RX_CHANNELS_NUM; chNum++)
    {
        rxCh.chNum = chNum;
        switch (chNum)
        {
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_1KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_1KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_4KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA23_RGMII1_RX_CH_HIGH_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL;
                break;
        }

        if (chNum == PAL_CPPI_PP_DMA23_RGMII1_RX_CH_HIGH_DDR)
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_HI_Q_NUM;
        }
        else
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
        }

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return(-1);
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
    }

    /* RGMII1_TX_CHs */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_RGMII1_TX_HI_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_RGMII1_TX_LOW_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    txCh.dmaNum = PAL_CPPI_PP_DMA23_RGMII1_RX_TX;
    txCh.chNum  = PAL_CPPI_PP_DMA23_RGMII_TX_CH;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;

    txChHdl     = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
    if(txChHdl == NULL)
    {
        printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, txChNum=%d\n", __FUNCTION__, __LINE__, txCh.dmaNum, txCh.chNum);
        return(-1);
    }
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    /* RGMII1 Infrastructure DMA */
    for (chNum = 0; chNum < PAL_CPPI_PP_RGMII1_INFRA_DMA_CH_COUNT; chNum++)
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_RGMII1_INFRA_INPUT_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        tmpQ.qNum = PAL_CPPI_PP_RGMII1_HOST_RX_COMPLETE_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Set up RX channel */
        rxCh.chNum            = PAL_CPPI_PP_RGMII1_INFRA_DMA_CH_NUM(chNum);
        rxCh.dmaNum           = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        rxCh.defDescType      = CPPI41_DESC_TYPE_HOST;
        rxCh.sopOffset        = 0;
        rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum = PAL_CPPI_PP_RGMII1_HOST_RX_COMPLETE_Q_NUM(chNum);
        rxCh.retryOnStarvation = 0;
        rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_RGMII1_INFRA_HOST_FD_Q_NUM(chNum);

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return (-1);
        }

        /* Set up TX channel */
        txCh.chNum         = PAL_CPPI_PP_RGMII1_INFRA_DMA_CH_NUM(chNum);
        txCh.dmaNum        = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        txCh.tdQueue.qMgr  = PAL_CPPI_PP_QMGR_G2;
        txCh.tdQueue.qNum  = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
        txCh.defDescType   = CPPI41_DESC_TYPE_EMBEDDED;

        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!rxChHdl || !txChHdl)
        {
            printk ("%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, chNum);
            return -1;
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;
}

#ifdef CONFIG_INTEL_PP_NO_NRGMII1_DESC
#define TRUE_IF_RGMII1_EN False
#else
#define TRUE_IF_RGMII1_EN True
#endif
qsmonRal_t rgmii1RalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_0_RGMII1_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_1_RGMII1_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_2_RGMII1_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = TRUE_IF_RGMII1_EN,
        .ralQEnable     = TRUE_IF_RGMII1_EN
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = TRUE_IF_RGMII1_EN
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_3_RGMII1_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_4_RGMII1_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_5_RGMII1_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = TRUE_IF_RGMII1_EN,
        .ralQEnable     = TRUE_IF_RGMII1_EN
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA23_RGMII1_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(rgmii1RalQsmon);

Int32 avalanche_cppi_init_pp_rgmii1_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_PRIVATE_DDR; chNum++)
    {
        qsMonRalConfig = rgmii1RalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR3, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR3 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA23_RGMII1_RX_TX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA23_RGMII1_RX_TX, ralQenBitmap);
        return -1;
    }

    return 0;
}


Int32 avalanche_cppi_init_pp_sgmii0_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              chNum;

    /* SGMII0_RX_CHs */
    rxCh.dmaNum = PAL_CPPI_PP_DMA03_SGMII0_RX;
    rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation = 0;
    rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;

    for (chNum = PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum < PAL_CPPI_PP_DMA03_SGMII0_RX_CHANNELS_NUM; chNum++)
    {
        rxCh.chNum = chNum;
        switch (chNum)
        {
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_1KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_1KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_4KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA03_SGMII0_RX_CH_HIGH_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL;
                break;
        }

        if (chNum == PAL_CPPI_PP_DMA03_SGMII0_RX_CH_HIGH_DDR)
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_HI_Q_NUM;
        }
        else
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
        }

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return(-1);
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
    }

    /* SGMII0_TX_CHs */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_SGMII0_TX_HI_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_SGMII0_TX_LOW_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    txCh.dmaNum = PAL_CPPI_PP_DMA14_SGMII0_TX;
    txCh.chNum  = PAL_CPPI_PP_DMA14_SGMII0_TX_CH;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    txChHdl     = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
    if(txChHdl == NULL)
    {
        printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, txChNum=%d\n", __FUNCTION__, __LINE__, txCh.dmaNum, txCh.chNum);
        return(-1);
    }
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    /* SGMII0 Infrastructure DMA */
    for (chNum = 0; chNum < PAL_CPPI_PP_SGMII0_INFRA_DMA_CH_COUNT; chNum++)
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_SGMII0_INFRA_INPUT_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        tmpQ.qNum = PAL_CPPI_PP_SGMII0_HOST_RX_COMPLETE_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Set up RX channel */
        rxCh.chNum            = PAL_CPPI_PP_SGMII0_INFRA_DMA_CH_NUM(chNum);
        rxCh.dmaNum           = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        rxCh.defDescType      = CPPI41_DESC_TYPE_HOST;
        rxCh.sopOffset        = 0;
        rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum = PAL_CPPI_PP_SGMII0_HOST_RX_COMPLETE_Q_NUM(chNum);
        rxCh.retryOnStarvation = 0;
        rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_SGMII0_INFRA_HOST_FD_Q_NUM(chNum);

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return (-1);
        }

        /* Set up TX channel */
        txCh.chNum         = PAL_CPPI_PP_SGMII0_INFRA_DMA_CH_NUM(chNum);
        txCh.dmaNum        = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        txCh.tdQueue.qMgr  = PAL_CPPI_PP_QMGR_G2;
        txCh.tdQueue.qNum  = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
        txCh.defDescType   = CPPI41_DESC_TYPE_EMBEDDED;

        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!rxChHdl || !txChHdl)
        {
            printk ("%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, chNum);
            return -1;
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;
}

qsmonRal_t sgmii0RalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_0_SGMII0_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_1_SGMII0_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_2_SGMII0_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = True,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_3_SGMII0_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_4_SGMII0_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_5_SGMII0_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = True,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA03_SGMII0_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(sgmii0RalQsmon);

Int32 avalanche_cppi_init_pp_sgmii0_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_PRIVATE_DDR; chNum++)
    {

        qsMonRalConfig = sgmii0RalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR4, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR4 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA03_SGMII0_RX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA03_SGMII0_RX, ralQenBitmap);
        return -1;
    }

    return 0;
}

Int32 avalanche_cppi_init_pp_sgmii1_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              chNum;

    /* SGMII1_RX_CHs */
    rxCh.dmaNum = PAL_CPPI_PP_DMA04_SGMII1_RX;
    rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation = 0;
    rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;

    for (chNum = PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum < PAL_CPPI_PP_DMA04_SGMII1_RX_CHANNELS_NUM; chNum++)
    {
        rxCh.chNum = chNum;
        switch (chNum)
        {
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_1KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_1KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_SHARED_PACKET_RAM:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_PRIVATE_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_4KB_SHARED_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
                break;
            case PAL_CPPI_PP_DMA04_SGMII1_RX_CH_HIGH_DDR:
                rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM;
                rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL;
                break;
        }

        if (chNum == PAL_CPPI_PP_DMA04_SGMII1_RX_CH_HIGH_DDR)
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_HI_Q_NUM;
        }
        else
        {
            rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
        }

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return(-1);
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
    }

    /* SGMII1_TX_CHs */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_SGMII1_TX_HI_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_SGMII1_TX_LOW_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return(-1);
    }
    txCh.dmaNum = PAL_CPPI_PP_DMA15_SGMII1_TX;
    txCh.chNum  = PAL_CPPI_PP_DMA15_SGMII1_TX_CH;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
    txChHdl     = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
    if(txChHdl == NULL)
    {
        printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, txChNum=%d\n", __FUNCTION__, __LINE__, txCh.dmaNum, txCh.chNum);
        return(-1);
    }
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    /* SGMII1 Infrastructure DMA */
    for (chNum = 0; chNum < PAL_CPPI_PP_SGMII1_INFRA_DMA_CH_COUNT; chNum++)
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_SGMII1_INFRA_INPUT_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
        tmpQ.qNum = PAL_CPPI_PP_SGMII1_HOST_RX_COMPLETE_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return(-1);
        }
        /* Set up RX channel */
        rxCh.chNum            = PAL_CPPI_PP_SGMII1_INFRA_DMA_CH_NUM(chNum);
        rxCh.dmaNum           = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        rxCh.defDescType      = CPPI41_DESC_TYPE_HOST;
        rxCh.sopOffset        = 0;
        rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum = PAL_CPPI_PP_SGMII1_HOST_RX_COMPLETE_Q_NUM(chNum);
        rxCh.retryOnStarvation = 0;
        rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_SGMII1_INFRA_HOST_FD_Q_NUM(chNum);

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return (-1);
        }

        /* Set up TX channel */
        txCh.chNum         = PAL_CPPI_PP_SGMII1_INFRA_DMA_CH_NUM(chNum);
        txCh.dmaNum        = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        txCh.tdQueue.qMgr  = PAL_CPPI_PP_QMGR_G2;
        txCh.tdQueue.qNum  = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
        txCh.defDescType   = CPPI41_DESC_TYPE_EMBEDDED;

        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!rxChHdl || !txChHdl)
        {
            printk ("%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, chNum);
            return -1;
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;
}

qsmonRal_t sgmii1RalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_24_SGMII1_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_25_SGMII1_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_26_SGMII1_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = True,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_27_SGMII1_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_28_SGMII1_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_PRIVATE_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_3_THREAD_29_SGMII1_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_PRIVATE_THRESHOLD,
        .qsmonChEn      = True,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA04_SGMII1_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(sgmii1RalQsmon);

Int32 avalanche_cppi_init_pp_sgmii1_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_PRIVATE_DDR; chNum++)
    {

        qsMonRalConfig = sgmii1RalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR3, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR3 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA04_SGMII1_RX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA04_SGMII1_RX, ralQenBitmap);
        return -1;
    }

    return 0;
}

Int32 avalanche_cppi_pp_configure_qsmon(PAL_Handle pal_handle, Uint32 manager, qsmonRal_t *qsmon, Bool setCount)
{
    PAL_Result  err = 0;

    if (NULL == qsmon)
    {
        printk(KERN_ERR "ERROR: %s: qsmon is NULL\n", __FUNCTION__);
        return -1;
    }

    if (setCount)
    {
        if (qsmon->direction == PAL_CPPI_QSMON_DIRECTION_DOWN)
        {
            err |= PAL_cppi4QSMonSetQdepthCnt(pal_handle,
                                              manager,
                                              qsmon->qsmonThread, 
                                              qsmon->qsmonThreshold); 
        }
    }
    else
    {
        err |= PAL_cppi4QSMonEnable(pal_handle, manager,
                                    qsmon->qsmonThread, qsmon->fdQueue,
                                    0, qsmon->direction); 
    }

    return err;
}

Int32 avalanche_cppi_init_pp_debug_qsmon(PAL_Handle palHandle, Bool setCount)
{
    PAL_Result  err = 0;
    Uint32      manager;
    Uint32      thread, max_threads;
    qsmonRal_t  *qsmon;

    for (manager = PAL_CPPI_PP_QSMON_MGR0; manager <= PAL_CPPI_PP_BSMON_MGR; manager++)
    {
        switch (manager)
        {
            case PAL_CPPI_PP_QSMON_MGR1:
                qsmon = gQsmon1;
                max_threads = (sizeof(gQsmon1) / sizeof(gQsmon1[0]));
                break;

            case PAL_CPPI_PP_QSMON_MGR2:
                qsmon = gQsmon2;
                max_threads = (sizeof(gQsmon2) / sizeof(gQsmon2[0]));
                break;

            case PAL_CPPI_PP_BSMON_MGR:
                qsmon = gBsmon;
                max_threads = (sizeof(gBsmon) / sizeof(gBsmon[0]));
                break;

            default:
                continue;
                break;
        }


        for (thread = 0; thread < max_threads; thread++)
        {
            if (!qsmon[thread].qsmonChEn)
                continue;
            err |= avalanche_cppi_pp_configure_qsmon(palHandle, manager, &qsmon[thread], setCount);
        }
    }

    if (err)
    {
        printk(KERN_ERR "ERROR: %s failed\n", __FUNCTION__);
        return -1;
    }

    return 0;
}



Int32 avalanche_cppi_init_pp_dma_channels(PAL_Handle palHandle)
{
    /*********************************************/
    /*********** HOST2PP DMA channels ************/
    /*********************************************/
    if (avalanche_cppi_init_pp_host2pp_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_host2pp_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

#ifdef CONFIG_MACH_PUMA7_BOARD
    /******************************************/
    /*********** MoCA DMA channels ************/
    /******************************************/
    if (avalanche_cppi_init_pp_moca_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_moca_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    /******************************************/
    /*********** ATOM DMA channels ************/
    /******************************************/
    if (avalanche_cppi_init_pp_atom_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_atom_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }
#endif

    /********************************************/
    /*********** RGMII0 DMA channels ************/
    /********************************************/
    if (avalanche_cppi_init_pp_rgmii0_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_rgmii0_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    /********************************************/
    /*********** RGMII1 DMA channels ************/
    /********************************************/
    if (avalanche_cppi_init_pp_rgmii1_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_rgmii1_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

#ifdef CONFIG_MACH_PUMA7_BOARD
    /********************************************/
    /*********** SGMII0 DMA channels ************/
    /********************************************/
    if (avalanche_cppi_init_pp_sgmii0_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_sgmii0_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    /********************************************/
    /*********** SGMII1 DMA channels ************/
    /********************************************/
    if (avalanche_cppi_init_pp_sgmii1_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_sgmii1_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }
#endif

#if CONFIG_NP_APP_DATAPIPE
    /********************************************/
    /********* Data Pipe DMA channels ***********/
    /********************************************/
    if (avalanche_cppi_init_pp_data_pipe_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_data_pipe_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }
#endif

#ifdef CONFIG_WIFI_PROXY
    /********************************************/
    /********* WiFi proxy DMA channels **********/
    /********************************************/
    if (avalanche_cppi_init_pp_wifi_rx_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_wifi_rx_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    if (avalanche_cppi_init_pp_wifi_tx_dma_channels(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_wifi_tx_dma_channels [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }
#endif

    return 0;
}

Int32 avalanche_cppi_init_pp_ral_and_qsmon(PAL_Handle palHandle)
{
#ifdef CONFIG_MACH_PUMA7_BOARD
    /*****************************************/
    /*********** MoCA RAL & QSMON ************/
    /*****************************************/
    if (avalanche_cppi_init_pp_moca_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_moca_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    /*****************************************/
    /*********** ATOM RAL & QSMON ************/
    /*****************************************/
    if (avalanche_cppi_init_pp_atom_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_atom_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }
#endif

    /*******************************************/
    /*********** RGMII0 RAL & QSMON ************/
    /*******************************************/
    if (avalanche_cppi_init_pp_rgmii0_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_rgmii0_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    /*******************************************/
    /*********** RGMII1 RAL & QSMON ************/
    /*******************************************/
    if (avalanche_cppi_init_pp_rgmii1_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_rgmii1_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

#ifdef CONFIG_MACH_PUMA7_BOARD
    /*******************************************/
    /*********** SGMII0 RAL & QSMON ************/
    /*******************************************/
    if (avalanche_cppi_init_pp_sgmii0_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_sgmii0_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    /*******************************************/
    /*********** SGMII1 RAL & QSMON ************/
    /*******************************************/
    if (avalanche_cppi_init_pp_sgmii1_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_sgmii1_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }
#endif


    return 0;
}

#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
static void init_buffer_pools(struct work_struct *work)
{
    struct file *f;
    char buf[128];
    int ret = 0;

    if (init_buffer_pools_iterations > INIT_BUFFER_POOLS_MAX_ITER)
    {
        DEBUG_PAL("done the maximum iterations polling mrpc init for buffer pools init\n");
        cancel_delayed_work_sync(&d_init_buffer_pools_work); 
        flush_workqueue(init_buffer_pools_wq);
        destroy_workqueue(init_buffer_pools_wq);
        init_buffer_pools_wq = NULL;
        return;
    }

    init_buffer_pools_iterations++;

    DEBUG_PAL("Polling MRPC iteration %d", init_buffer_pools_iterations);

    f = filp_open("/sys/devices/platform/cppi_client/status", O_RDONLY, 0);
    DEBUG_PAL("After file open");

    if(f == NULL)
    {
        DEBUG_PAL("MRPC Client is not ready... rescheduling...\n");
        /* Schedule the work */
        DEBUG_PAL("Start polling MRPC for buffer pools init in %u ms, %lu jiffies", 100, msecs_to_jiffies(100));
        queue_delayed_work(init_buffer_pools_wq, &d_init_buffer_pools_work, msecs_to_jiffies(100));
    }
    else
    {
        DEBUG_PAL("MRPC Client is ready... trying to allocate buffer pools..\n");
        ret = cppi_post_init();
        if (ret == CPPI_POST_INIT_ERR_BUFF_INIT)
        {
            DEBUG_PAL("Failed to initiate buffer pools, scheduling a new workqueue..\n");
            /* Schedule the work */
            DEBUG_PAL("Start polling MRPC for buffer pools init in %u ms, %lu jiffies, init_buffer_pools_wq 0x%x, ", 100, msecs_to_jiffies(100));
            queue_delayed_work(init_buffer_pools_wq, &d_init_buffer_pools_work, msecs_to_jiffies(100));
        }
        else 
        {
            if (ret == CPPI_POST_INIT_OK)
            {
                DEBUG_PAL("Buffer pools allocated successfully, destroying worqueue...\n");
            }
            else 
            {
                DEBUG_PAL("Error during post init stage, destroying worqueue...\n");
            }
            cancel_delayed_work_sync(&d_init_buffer_pools_work); 
            flush_workqueue(init_buffer_pools_wq);
            destroy_workqueue(init_buffer_pools_wq);
            init_buffer_pools_wq = NULL;
        }
    }

    return;
}
#endif

Int32 avalanche_cppi_get_buffer_count(Uint32 bufPoolID, Uint32* buff_csount)
{
    if (bufPoolID>=PAL_CPPI41_BMGR_MAX_POOLS || buff_csount==NULL)
        return 1;

    switch(bufPoolID)
    {
        case PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL:
            *buff_csount = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_COUNT;
            break;
        case PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL:
            *buff_csount = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_COUNT;
            break;
        case PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL:
            *buff_csount = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_COUNT;
            break;
        case PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL:
            *buff_csount = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_COUNT;
            break;
        case PAL_CPPI_PP_WIFI_RX_POOL:
            *buff_csount = PAL_CPPI_PP_BUFFER_POOL4_COUNT;
            break;
        case PAL_CPPI_PP_BUFFER_POOL5:
            *buff_csount = PAL_CPPI_PP_BUFFER_POOL5_COUNT;
            break;
        case PAL_CPPI_PP_BUFFER_POOL6:
            *buff_csount = PAL_CPPI_PP_BUFFER_POOL6_COUNT;
            break;
        case PAL_CPPI_PP_BUFFER_POOL7:
            *buff_csount = PAL_CPPI_PP_BUFFER_POOL7_COUNT;
            break;
        case PAL_CPPI_PP_BUFFER_POOL8:
            *buff_csount = PAL_CPPI_PP_BUFFER_POOL8_COUNT;
            break;
        case PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL:
            *buff_csount = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_COUNT;
            break;
        case PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL:
            *buff_csount = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_COUNT;
            break;
        case PAL_CPPI_PP_VOICE_DSP_BUFFER_POOL:
            *buff_csount = PAL_CPPI_PP_VOICE_DSP_BUFFER_COUNT;
            break;
        default:
            BUG_ON(1);
            break;
    }
    return 0;
}
EXPORT_SYMBOL(avalanche_cppi_get_buffer_count);

static void inline __mem_profile_init(void)
{
    int i;

    printk(KERN_ERR "CPPI memProfile %d\n", memProfile);
    if(memProfile == 0)
        goto set_qsmonThreshold;

    /************************************************************************/
    /********* Override initial values for low memory configuration *********/
    /************************************************************************/

    rx_low_short_shared_ddr_emb_fd_desc_count                = PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_REDUCED;
    rx_low_long_shared_ddr_emb_fd_desc_count                 = PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT_REDUCED;
    moca_rx_low_short_private_ddr_emb_fd_desc_count          = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    moca_rx_low_long_private_ddr_emb_fd_desc_count           = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    moca_rx_low_short_private_packet_ram_emb_fd_desc_count   = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
    moca_rx_low_long_private_packet_ram_emb_fd_desc_count    = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
    rgmii0_rx_low_short_private_ddr_emb_fd_desc_count        = PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    rgmii0_rx_low_long_private_ddr_emb_fd_desc_count         = PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    rgmii0_rx_high_private_ddr_emb_fd_desc_count             = PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    rgmii0_rx_low_short_private_packet_ram_emb_fd_desc_count = PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
    rgmii0_rx_low_long_private_packet_ram_emb_fd_desc_count  = PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
    rgmii1_rx_low_short_private_ddr_emb_fd_desc_count        = PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    rgmii1_rx_low_long_private_ddr_emb_fd_desc_count         = PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    rgmii1_rx_high_private_ddr_emb_fd_desc_count             = PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
    rgmii1_rx_low_short_private_packet_ram_emb_fd_desc_count = PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
    rgmii1_rx_low_long_private_packet_ram_emb_fd_desc_count  = PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
    wifi_rx_pool_buffer_count                                = PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_COUNT_REDUCED;

    for (i = 0; i < sizeof(gQsmon2)/sizeof(gQsmon2[0]); i++)
    {
        switch(gQsmon2[i].fdQueue.qNum)
        {
            case PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW:
                gQsmon2[i].qsmonThreshold = PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
#ifndef CONFIG_IPSEC_SUPPORT
            case PAL_CPPI_PP_QMGR_G2_MoCA_DDR_SHORT_RX_EMB_FD_Q_NUM:
                gQsmon2[i].qsmonThreshold = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_MoCA_DDR_LONG_RX_EMB_FD_Q_NUM:
                gQsmon2[i].qsmonThreshold = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
#endif
        }
    }

    /* Go over all free decriptors queues list */
    for (i = 0; i < sizeof(gPpFDqueues)/sizeof(gPpFDqueues[0]); i++)
    {
        switch(gPpFDqueues[i].qId)
        {
            // Shared
            case PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            // MoCA
            case PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_MoCA_DDR_SHORT_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_MoCA_DDR_LONG_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            // RGMII0
            case PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            // RGMII1
            case PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;
            case PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM:
                gPpFDqueues[i].descCount = PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED;
                break;

        }
    }

set_qsmonThreshold:
    /************************************************************************/
    /******************* Initialize buffer pool sizes ***********************/
    /************************************************************************/

    gBsmon[PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL].qsmonThreshold = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_COUNT;
    gBsmon[PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL].qsmonThreshold = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_COUNT;
    gBsmon[PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL].qsmonThreshold = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_COUNT;
    gBsmon[PAL_CPPI_PP_WIFI_RX_POOL].qsmonThreshold = PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_COUNT;
    gBsmon[PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL].qsmonThreshold = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_COUNT;
    gBsmon[PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL].qsmonThreshold = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_COUNT;
    gBsmon[PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL].qsmonThreshold = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_COUNT;
    gBsmon[PAL_CPPI_PP_VOICE_DSP_BUFFER_POOL].qsmonThreshold = PAL_CPPI_PP_VOICE_DSP_BUFFER_COUNT;

    return;
}

int avalanche_cppi_init(void)
{
    PAL_Handle          palHandle;
    Uint32              ret;

    PAL_osMemSet(&gCppiPpInitCfg, 0, sizeof(gCppiPpInitCfg));

    gCppiPpInitCfg.debugToolBind = cppi_pp_proc_init;

    /************************************************/
    /* Init variables limited by the memory profile */
    /************************************************/
    __mem_profile_init();

    /***********************/
    /* Init Buffer Manager */
    /***********************/
    gCppiPpInitCfg.bufMgrBase[PAL_CPPI_PP_BUF_MGR] = (CSL_BufMgr_RegsOvly)PAL_CPPI_PP_BUF_MGR_BASE;

    /***********************/
    /* Init Queue Managers */
    /***********************/
    avalanche_cppi_init_pp_queue_managers();

    /******************************/
    /* Init Queue Status Monitors */
    /******************************/
    avalanche_cppi_init_pp_queue_status_monitors_managers();

    /****************************/
    /* Init Descriptors Regions */
    /****************************/
    if (avalanche_cppi_init_pp_descriptor_regions())
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_descriptor_regions [CPPI41_DOMAIN_PP] FAILED\n");
        return 0;
    }

    /******************************************************************/
    /*********** Init DMA addresses and attributes ********************/
    /******************************************************************/
    avalanche_cppi_init_pp_dmas();

    /***********************/
    /* Init PP CPPI Domain */
    /***********************/
    palHandle = PAL_cppi4Init(&gCppiPpInitCfg, CPPI41_DOMAIN_PP);
    if(palHandle == NULL)
    {
        printk(KERN_EMERG "ERROR PAL_cppi4Init of CPPI41_DOMAIN_PP failed!\n");
        return 0;
    }
    printk(KERN_NOTICE "PAL_cppi4Init of CPPI41_DOMAIN_PP initialized successfully\n");

    /*****************************************/
    /*********** Setup RAL & QSMON ***********/
    /*****************************************/
    if (avalanche_cppi_init_pp_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return 0;
    }

    /************************************************/
    /*********** Setup Docsis RAL & QSMON ***********/
    /************************************************/
    if (avalanche_cppi_init_pp_docsis_ral_and_qsmon(palHandle))
    {
        printk (KERN_ERR "ERROR: avalanche_cppi_init_pp_docsis_ral_and_qsmon FAILED\n");
        return (-1);
    }

    /*****************************************/
    /************ Debug QSMON PRE ************/
    /*****************************************/
    if (avalanche_cppi_init_pp_debug_qsmon(palHandle, False))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_debug_qsmon [CPPI41_DOMAIN_PP] pre FAILED\n");
        return -1;
    }

    /**************************************************************/
    /*********** Setup descriptors according to regions ***********/
    /**************************************************************/
    if (avalanche_cppi_init_pp_free_q_descriptors(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_free_q_descriptors [CPPI41_DOMAIN_PP] FAILED\n");
        return 0;
    }

    /*****************************************/
    /************ Debug QSMON POST ***********/
    /*****************************************/
    if (avalanche_cppi_init_pp_debug_qsmon(palHandle, True))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_debug_qsmon [CPPI41_DOMAIN_PP] post FAILED\n");
        return -1;
    }

#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
    init_buffer_pools_iterations = 0;

    DEBUG_PAL("Create single threaded work queue for atom buffers allocation\n");

    /* create single threaded work queue */
    if (NULL == init_buffer_pools_wq) {
        init_buffer_pools_wq = alloc_workqueue("%s", WQ_UNBOUND | WQ_MEM_RECLAIM, 1, init_buffer_pools_wq_name);
        if (NULL == init_buffer_pools_wq)
        {
            printk(KERN_ERR "ERROR: Failed to create %s\n", init_buffer_pools_wq_name);
            return -1;
        }
    }

    DEBUG_PAL("[Starting poll MRPC wq for atom buffer pools allocation\n");
    /* Init the work */
    INIT_DELAYED_WORK(&d_init_buffer_pools_work, init_buffer_pools);
    /* schedule the work */
    DEBUG_PAL("Start polling MRPC for buffer pools init in %u ms, %lu jiffies", 100, msecs_to_jiffies(100));
    queue_delayed_work(init_buffer_pools_wq, &d_init_buffer_pools_work, msecs_to_jiffies(100));
#else
    ret = cppi_post_init();
    if( ret != CPPI_POST_INIT_OK ) {
        printk(KERN_ERR "ERROR: cppi_post_init failed ret(%d)\n", ret);
        return -1;
        
    }
#endif /* CONFIG_MRPC_CPPI_CLIENT */

    return 0;
}


Int32 avalanche_cppi_init_pp_docsis_dma_channels(PAL_Handle palHandle)
{
    Cppi4TxChInitCfg    txCh;
    PAL_Cppi4TxChHnd    txChHdl;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4RxChHnd    rxChHdl;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              dmaNum;
    Uint32              chNum;

    /* DOCSIS_RX_CHs */
    rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
    rxCh.retryOnStarvation = 0;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;

    for (dmaNum = PAL_CPPI_PP_DMA00_OFDM0_RX; dmaNum <= PAL_CPPI_PP_DMA02_SC_QAM_RX; dmaNum++)
    {
#ifdef CONFIG_MACH_PUMA7_FPGA
        if (PAL_CPPI_PP_DMA01_OFDM1_RX == dmaNum)
        {
            continue;
        }
#endif
        rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.dmaNum = dmaNum;
        for (chNum = PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA00_OFDM0_RX_CH_13; chNum++) 
        {
            rxCh.chNum = chNum;    
            rxCh.sopOffset = PAL_CPPI_PP_START_OF_PACKET_OFFSET;
            rxCh.retryOnStarvation = 0;
            rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
            rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
            rxCh.u.embeddedPktCfg.sopSlotNum = 0;
            rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_PP_BUF_MGR;


            switch (chNum)
            {
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_SHARED_PACKET_RAM:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_DESC_PACKET_RAM_BUFF_DDR:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_1KB_SHARED_DDR:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_SHARED_PACKET_RAM:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_DESC_PACKET_RAM_BUFF_DDR:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_SHARED_DDR:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_4KB_SHARED_DDR:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_HIGH_DDR:
                    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_HI_EMB_FD_Q_NUM;
                    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_MGMT_DDR:
                    rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
                    rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_MGMT_HOST_FD_Q_NUM;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_PLC_EM:
                    rxCh.u.monolithicPktCfg.fdQueue.qMgr = PAL_CPPI_MAC_US_QMGR_DOCSIS;
                    rxCh.u.monolithicPktCfg.fdQueue.qNum = US_MAC_FD_DLS_MONOLITIC_ID;
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_FCC_DDR:
                    break;
                case PAL_CPPI_PP_DMA00_OFDM0_RX_CH_13:
                    {
                        rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_MAC_US_QMGR_DOCSIS;
                        rxCh.u.embeddedPktCfg.fdQueue.qNum = 276;
                        rxCh.u.embeddedPktCfg.fBufPool[0].bPool = 1;
                        rxCh.u.embeddedPktCfg.fBufPool[0].bMgr = PAL_CPPI_MAC_US_BUF_MGR;
                    }
                    break;
                    
            }

            if (chNum == PAL_CPPI_PP_DMA00_OFDM0_RX_CH_HIGH_DDR)
            {
                rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
                rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_HI_Q_NUM;
                rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
            }
            else if (chNum == PAL_CPPI_PP_DMA00_OFDM0_RX_CH_MGMT_DDR)
            {
                tmpQ.qMgr = PAL_CPPI_PP_QMGR_G2;
                tmpQ.qNum = PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_DOCSIS_MGMT_Q_NUM;
                tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
                if (tmpQHnd == NULL)
                {
                    printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
                    return (-1);
                }
                rxCh.rxCompQueue.qMgr = tmpQ.qMgr;
                rxCh.rxCompQueue.qNum = tmpQ.qNum;

                rxCh.defDescType = CPPI41_DESC_TYPE_HOST;
            }
            else if (chNum == PAL_CPPI_PP_DMA00_OFDM0_RX_CH_PLC_EM)
            {
                rxCh.rxCompQueue.qMgr = PAL_CPPI_MAC_US_QMGR_DOCSIS;
                rxCh.rxCompQueue.qNum = US_MAC_DLS_COMMAND_FROM_EXT_APP_ID;
                rxCh.defDescType = CPPI41_DESC_TYPE_MONOLITHIC;
            }
            else
            {
                rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
                rxCh.rxCompQueue.qNum = PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM;
                rxCh.defDescType = CPPI41_DESC_TYPE_EMBEDDED;
            }

            rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
            if(rxChHdl == NULL)
            {
                printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
                return(-1);
            }

            PAL_cppi4EnableRxChannel(rxChHdl, NULL);
            DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
        }
    }

    /* TX_CH_MGMT */
    tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
    tmpQ.qNum = PAL_CPPI_PP_QMGR_G1_US_PREPROCESSING_MGMT_Q_NUM;
    tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
    if (tmpQHnd == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
        return 0;
    }

    /* Setup DOCSIS Infrastructure DMA */
    for (chNum = 0; chNum < PAL_CPPI_PP_DOCSIS_INFRA_DMA_CH_COUNT; chNum++)
    {
        /* Input Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_DOCSIS_INFRA_INPUT_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return 0;
        }

        /* Output Queue */
        tmpQ.qMgr = PAL_CPPI_PP_QMGR_G1;
        tmpQ.qNum = PAL_CPPI_PP_DOCSIS_HOST_RX_COMPLETE_Q_NUM(chNum);
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return 0;
        }

        /* Set up Rx channel */
        rxCh.chNum            = PAL_CPPI_PP_DOCSIS_INFRA_DMA_CH_NUM(chNum);
        rxCh.dmaNum           = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        rxCh.defDescType      = CPPI41_DESC_TYPE_HOST;
        rxCh.sopOffset        = 0;
        rxCh.rxCompQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.rxCompQueue.qNum = PAL_CPPI_PP_DOCSIS_HOST_RX_COMPLETE_Q_NUM(chNum);
        rxCh.retryOnStarvation = 0;
        rxCh.u.hostPktCfg.fdbQueue[0].qMgr = PAL_CPPI_PP_QMGR_G2;
        rxCh.u.hostPktCfg.fdbQueue[0].qNum = PAL_CPPI_PP_DOCSIS_INFRA_HOST_FD_Q_NUM(chNum);

        rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
        if(rxChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, rxCh.dmaNum, rxCh.chNum);
            return (-1);
        }

        txCh.chNum         = PAL_CPPI_PP_DOCSIS_INFRA_DMA_CH_NUM(chNum);
        txCh.dmaNum        = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
        txCh.tdQueue.qMgr  = PAL_CPPI_PP_QMGR_G2;
        txCh.tdQueue.qNum  = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;
        txCh.defDescType   = CPPI41_DESC_TYPE_EMBEDDED;

        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

        if (!rxChHdl || !txChHdl)
        {
            printk(KERN_ERR "%s[%d]: infra channel setup failed for channel %d\n", __FUNCTION__, __LINE__, chNum);
            return -1;
        }

        PAL_cppi4EnableRxChannel(rxChHdl, NULL);
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
    }

    return 0;

}

qsmonRal_t scQamRalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_6_SCQAM0_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_7_SCQAM0_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_DESC_PACKET_RAM_BUFF_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_8_SCQAM0_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_9_SCQAM0_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_10_SCQAM0_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_DESC_PACKET_RAM_BUFF_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_11_SCQAM0_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_HI_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(scQamRalQsmon);

Int32 avalanche_cppi_init_pp_sc_qam_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_4KB_SHARED_DDR; chNum++)
    {

        qsMonRalConfig = scQamRalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR4, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR4 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA02_SC_QAM_RX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA02_SC_QAM_RX, ralQenBitmap);
        return -1;
    }

    return 0;
}

qsmonRal_t ofdm0RalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_18_OFDM0_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_19_OFDM0_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_DESC_PACKET_RAM_BUFF_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_20_OFDM0_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_21_OFDM0_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_22_OFDM0_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_DESC_PACKET_RAM_BUFF_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_23_OFDM0_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA00_OFDM0_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_HI_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(ofdm0RalQsmon);

Int32 avalanche_cppi_init_pp_ofdm0_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_4KB_SHARED_DDR; chNum++)
    {

        qsMonRalConfig = ofdm0RalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR4, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR4 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA00_OFDM0_RX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA00_OFDM0_RX, ralQenBitmap);
        return -1;
    }

    return 0;
}


qsmonRal_t ofdm1RalQsmon[] =
{
    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_12_OFDM1_CH_0,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_13_OFDM1_CH_1,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_DESC_PACKET_RAM_BUFF_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_14_OFDM1_CH_2,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_SHARED_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_15_OFDM1_CH_4,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_SHARED_PACKET_RAM  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_16_OFDM1_CH_5,
        .qsmonThreshold = PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_DESC_PACKET_RAM_BUFF_DDR  */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonThread    = PAL_CPPI_PP_QSMON_4_THREAD_17_OFDM1_CH_6,
        .qsmonThreshold = PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = True
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_4KB_SHARED_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    },

    {   /*  PAL_CPPI_PP_DMA01_OFDM1_RX_CH_HIGH_DDR   */
        .fdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_HI_EMB_FD_Q_NUM,
        .fdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2,
        .qsmonChEn      = False,
        .ralQEnable     = False
    }
};
EXPORT_SYMBOL(ofdm1RalQsmon);

Int32 avalanche_cppi_init_pp_ofdm1_ral_and_qsmon(PAL_Handle palHandle)
{
    Uint32              chNum;
    Uint32              ralQenBitmap;
    qsmonRal_t          qsMonRalConfig;

    ralQenBitmap = 0;

    for (chNum = PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM; chNum <= PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_4KB_SHARED_DDR; chNum++)
    {

        qsMonRalConfig = ofdm1RalQsmon[chNum];

        if (qsMonRalConfig.ralQEnable)
        {
            ralQenBitmap |= (1 << chNum);
        }

        if (qsMonRalConfig.qsmonChEn)
        {
            if (PAL_cppi4QSMonEnable(palHandle, PAL_CPPI_PP_QSMON_MGR4, qsMonRalConfig.qsmonThread, qsMonRalConfig.fdQueue, qsMonRalConfig.qsmonThreshold, PAL_CPPI_QSMON_DIRECTION_UP))
            {
                printk(KERN_ERR "ERROR: PAL_cppi4QSMonEnable failed for PAL_CPPI_PP_QSMON_MGR4 qsmonThread=%d\n", qsMonRalConfig.qsmonThread);
                return -1;
            }
        }
    }

    if (PAL_cppi4RalEnable(palHandle, PAL_CPPI_PP_DMA01_OFDM1_RX, ralQenBitmap))
    {
        printk(KERN_ERR "ERROR: PAL_cppi4RalEnable failed for dmaNum=%d, ralQenBitmap=%d\n", PAL_CPPI_PP_DMA01_OFDM1_RX, ralQenBitmap);
        return -1;
    }

    return 0;
}

Int32 avalanche_cppi_init_pp_docsis_ral_and_qsmon(PAL_Handle palHandle)
{

    if (0 != avalanche_cppi_init_pp_sc_qam_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_sc_qam_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

    if (0 != avalanche_cppi_init_pp_ofdm0_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_ofdm0_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

#ifndef CONFIG_MACH_PUMA7_FPGA

    if (0 != avalanche_cppi_init_pp_ofdm1_ral_and_qsmon(palHandle))
    {
        printk(KERN_ERR "ERROR: avalanche_cppi_init_pp_ofdm1_ral_and_qsmon [CPPI41_DOMAIN_PP] FAILED\n");
        return -1;
    }

#endif
    return 0;
}

int Puma_DOCSIS_CPPI_Init(void)
{
    PAL_Handle          palHandle;
    Cppi4BufPool        tmpBufPool;
    Uint32              i;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Uint32              descCount;
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;
    Uint32              dmaNum;
    Uint32              chNum;

/*
    -------------------------------------------------------------------------------------

    ########   ######   ######      ########   #######  ##     ##    ###    #### ##    ##
    ##     ## ##    ## ##    ##     ##     ## ##     ## ###   ###   ## ##    ##  ###   ##
    ##     ## ##       ##           ##     ## ##     ## #### ####  ##   ##   ##  ####  ##
    ##     ##  ######  ##   ####    ##     ## ##     ## ## ### ## ##     ##  ##  ## ## ##
    ##     ##       ## ##    ##     ##     ## ##     ## ##     ## #########  ##  ##  ####
    ##     ## ##    ## ##    ##     ##     ## ##     ## ##     ## ##     ##  ##  ##   ###
    ########   ######   ######      ########   #######  ##     ## ##     ## #### ##    ##

    -------------------------------------------------------------------------------------
*/

    PAL_osMemSet(&gCppiDsgInitCfg, 0, sizeof(gCppiDsgInitCfg));

    gCppiDsgInitCfg.debugToolBind = cppi_dsg_proc_init;

    /******************************/
    /******************************/
    /* Buffer Manager Information */
    /******************************/
    /******************************/
    gCppiDsgInitCfg.bufMgrBase[PAL_CPPI_OFDM0_BUF_MGR]                                                              = (CSL_BufMgr_RegsOvly)PAL_CPPI_OFDM0_BUF_MGR_BASE;
#ifndef CONFIG_MACH_PUMA7_FPGA
    gCppiDsgInitCfg.bufMgrBase[PAL_CPPI_OFDM1_BUF_MGR]                                                              = (CSL_BufMgr_RegsOvly)PAL_CPPI_OFDM1_BUF_MGR_BASE;
#endif
    gCppiDsgInitCfg.bufMgrBase[PAL_CPPI_SC_QAM0_BUF_MGR]                                                            = (CSL_BufMgr_RegsOvly)PAL_CPPI_SC_QAM0_BUF_MGR_BASE;

    /****************************************************/
    /****************************************************/
    /* Queue Managers + Descriptors Regions Information */
    /****************************************************/
    /****************************************************/

    /* OFDM0 DSG */
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].queueMgrRgnBase                                          = (Ptr) PAL_CPPI_OFDM0_QUEUE_MGR_REGS_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].descMemRgnBase                                           = (Ptr) PAL_CPPI_OFDM0_QUEUE_MGR_DESC_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].queueMgmtRgnBase                                         = (Ptr) PAL_CPPI_OFDM0_QUEUE_MGR_QUEUES_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].queueStatusRgnBase                                       = (Ptr) PAL_CPPI_OFDM0_QUEUE_MGR_Q_STATS_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].queueProxyRgnBase                                        = NULL;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].totalQNum                                                = PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].LinkingRAM0Base                                          = PAL_CPPI_DSG_QUEUE_MGR_LINKING_RAM_INTERNAL_ADDRESS;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].LinkingRAM0Size                                          = PAL_CPPI_OFDM0_QUEUE_MGR_LINKING_RAM_SIZE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].base         = (Ptr)PAL_CPPI_DSG_DESC_RAM_INTERNAL_ADDRESS;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].szDesc       = PAL_CPPI_DS_DSG_FD_EMB_DESC_SIZE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].numDesc      = PAL_CPPI_OFDM0_FD_EMB_DESC_COUNT;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].isOnChip     = 1;

#ifndef CONFIG_MACH_PUMA7_FPGA
    /* OFDM1 DSG */
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].queueMgrRgnBase                                          = (Ptr) PAL_CPPI_OFDM1_QUEUE_MGR_REGS_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].descMemRgnBase                                           = (Ptr) PAL_CPPI_OFDM1_QUEUE_MGR_DESC_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].queueMgmtRgnBase                                         = (Ptr) PAL_CPPI_OFDM1_QUEUE_MGR_QUEUES_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].queueStatusRgnBase                                       = (Ptr) PAL_CPPI_OFDM1_QUEUE_MGR_Q_STATS_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].queueProxyRgnBase                                        = NULL;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].totalQNum                                                = PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].LinkingRAM0Base                                          = PAL_CPPI_DSG_QUEUE_MGR_LINKING_RAM_INTERNAL_ADDRESS;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].LinkingRAM0Size                                          = PAL_CPPI_OFDM1_QUEUE_MGR_LINKING_RAM_SIZE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].base         = (Ptr)PAL_CPPI_DSG_DESC_RAM_INTERNAL_ADDRESS;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].szDesc       = PAL_CPPI_DS_DSG_FD_EMB_DESC_SIZE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].numDesc      = PAL_CPPI_OFDM1_FD_EMB_DESC_COUNT;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_OFDM1_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].isOnChip     = 1;
#endif

    /* SC-QAMs DSG */
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].queueMgrRgnBase                                        = (Ptr) PAL_CPPI_SC_QAM0_QUEUE_MGR_REGS_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].descMemRgnBase                                         = (Ptr) PAL_CPPI_SC_QAM0_QUEUE_MGR_DESC_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].queueMgmtRgnBase                                       = (Ptr) PAL_CPPI_SC_QAM0_QUEUE_MGR_QUEUES_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].queueStatusRgnBase                                     = (Ptr) PAL_CPPI_SC_QAM0_QUEUE_MGR_Q_STATS_BASE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].queueProxyRgnBase                                      = NULL;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].totalQNum                                              = PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].LinkingRAM0Base                                        = PAL_CPPI_DSG_QUEUE_MGR_LINKING_RAM_INTERNAL_ADDRESS;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].LinkingRAM0Size                                        = PAL_CPPI_SC_QAM0_QUEUE_MGR_LINKING_RAM_SIZE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].base       = (Ptr)PAL_CPPI_DSG_DESC_RAM_INTERNAL_ADDRESS;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].szDesc     = PAL_CPPI_DS_DSG_FD_EMB_DESC_SIZE;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].numDesc    = PAL_CPPI_SC_QAM0_FD_EMB_DESC_COUNT;
    gCppiDsgInitCfg.queueMgrInfo[PAL_CPPI_SC_QAM0_QUEUE_MGR].descRegion[PAL_CPPI_DSG_FD_EMB_DESC_REGION].isOnChip   = 1;

    /********/
    /********/
    /* DMAs */
    /********/
    /********/

    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_RX_DMA].globalCtrlBase                              = (Ptr)PAL_CPPI_OFDM0_RX_DMA_GLOBAL_CTRL_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_RX_DMA].chCtrlStatusBase                            = (Ptr)PAL_CPPI_OFDM0_RX_DMA_CH_CFG_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_RX_DMA].schedCtrlBase                               = (Ptr)PAL_CPPI_OFDM0_RX_DMA_SCHEDULER_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_RX_DMA].tdFQueue.qMgr                               = PAL_CPPI_OFDM0_QUEUE_MGR;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_RX_DMA].tdFQueue.qNum                               = PAL_CPPI_DSG_QMGR_TEARDOWN_Q_NUM;

    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_TX_DMA].globalCtrlBase                              = (Ptr)PAL_CPPI_OFDM0_TX_DMA_GLOBAL_CTRL_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_TX_DMA].chCtrlStatusBase                            = (Ptr)PAL_CPPI_OFDM0_TX_DMA_CH_CFG_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_TX_DMA].schedCtrlBase                               = (Ptr)PAL_CPPI_OFDM0_TX_DMA_SCHEDULER_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_TX_DMA].tdFQueue.qMgr                               = PAL_CPPI_OFDM0_QUEUE_MGR;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM0_TX_DMA].tdFQueue.qNum                               = PAL_CPPI_DSG_QMGR_TEARDOWN_Q_NUM;

#ifndef CONFIG_MACH_PUMA7_FPGA
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_RX_DMA].globalCtrlBase                              = (Ptr)PAL_CPPI_OFDM1_RX_DMA_GLOBAL_CTRL_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_RX_DMA].chCtrlStatusBase                            = (Ptr)PAL_CPPI_OFDM1_RX_DMA_CH_CFG_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_RX_DMA].schedCtrlBase                               = (Ptr)PAL_CPPI_OFDM1_RX_DMA_SCHEDULER_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_RX_DMA].tdFQueue.qMgr                               = PAL_CPPI_OFDM1_QUEUE_MGR;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_RX_DMA].tdFQueue.qNum                               = PAL_CPPI_DSG_QMGR_TEARDOWN_Q_NUM;

    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_TX_DMA].globalCtrlBase                              = (Ptr)PAL_CPPI_OFDM1_TX_DMA_GLOBAL_CTRL_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_TX_DMA].chCtrlStatusBase                            = (Ptr)PAL_CPPI_OFDM1_TX_DMA_CH_CFG_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_TX_DMA].schedCtrlBase                               = (Ptr)PAL_CPPI_OFDM1_TX_DMA_SCHEDULER_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_TX_DMA].tdFQueue.qMgr                               = PAL_CPPI_OFDM1_QUEUE_MGR;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_OFDM1_TX_DMA].tdFQueue.qNum                               = PAL_CPPI_DSG_QMGR_TEARDOWN_Q_NUM;
#endif

    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_RX_DMA].globalCtrlBase                            = (Ptr)PAL_CPPI_SC_QAM0_RX_DMA_GLOBAL_CTRL_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_RX_DMA].chCtrlStatusBase                          = (Ptr)PAL_CPPI_SC_QAM0_RX_DMA_CH_CFG_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_RX_DMA].schedCtrlBase                             = (Ptr)PAL_CPPI_SC_QAM0_RX_DMA_SCHEDULER_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_RX_DMA].tdFQueue.qMgr                             = PAL_CPPI_SC_QAM0_QUEUE_MGR;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_RX_DMA].tdFQueue.qNum                             = PAL_CPPI_DSG_QMGR_TEARDOWN_Q_NUM;

    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_TX_DMA].globalCtrlBase                            = (Ptr)PAL_CPPI_SC_QAM0_TX_DMA_GLOBAL_CTRL_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_TX_DMA].chCtrlStatusBase                          = (Ptr)PAL_CPPI_SC_QAM0_TX_DMA_CH_CFG_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_TX_DMA].schedCtrlBase                             = (Ptr)PAL_CPPI_SC_QAM0_TX_DMA_SCHEDULER_BASE;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_TX_DMA].tdFQueue.qMgr                             = PAL_CPPI_SC_QAM0_QUEUE_MGR;
    gCppiDsgInitCfg.dmaBlock[PAL_CPPI_SC_QAM0_TX_DMA].tdFQueue.qNum                             = PAL_CPPI_DSG_QMGR_TEARDOWN_Q_NUM;

    palHandle = PAL_cppi4Init(&gCppiDsgInitCfg, (Ptr)CPPI41_DOMAIN_DSG);
    if(palHandle == NULL)
    {
        printk(KERN_EMERG "ERROR PAL_cppi4Init of CPPI41_DOMAIN_DSG failed!\n");
        return 0;
    }
    printk(KERN_NOTICE "PAL_cppi4Init of CPPI41_DOMAIN_DSG initialized successfully\n");

    /*************************************************************************/
    /*********** Init DSG buffer pools used in the system ********************/
    /*************************************************************************/
    /* OFDM0 DSG Buffer Pool init */
    tmpBufPool.bPool = PAL_CPPI_OFDM0_BUFFER_POOL00;
    tmpBufPool.bMgr = PAL_CPPI_OFDM0_BUF_MGR;
    if ((PAL_cppi4BufPoolDirectInit(palHandle, tmpBufPool,
                                    PAL_CPPI_DS_DSG_BUFFER_POOL00_REF_CNT,
                                    PAL_CPPI_DS_DSG_BUFFER_POOL00_BUF_SIZE,
                                    PAL_CPPI_OFDM0_BUFFER_POOL00_BUF_COUNT,
                               (Ptr)PAL_CPPI_OFDM0_BUFFER_RAM_INTERNAL_ADDRESS)) == NULL)
    {
        printk(KERN_ERR "PAL_cppi4BufPoolDirectInit for pool %d [CPPI41_DOMAIN_DSG] FAILED\n", tmpBufPool.bPool);
        return -1;
    }

#ifndef CONFIG_MACH_PUMA7_FPGA
    /* OFDM1 DSG Buffer Pool init */
    tmpBufPool.bPool = PAL_CPPI_OFDM1_BUFFER_POOL00;
    tmpBufPool.bMgr = PAL_CPPI_OFDM1_BUF_MGR;
    if ((PAL_cppi4BufPoolDirectInit(palHandle, tmpBufPool,
                                    PAL_CPPI_DS_DSG_BUFFER_POOL00_REF_CNT,
                                    PAL_CPPI_DS_DSG_BUFFER_POOL00_BUF_SIZE,
                                    PAL_CPPI_OFDM1_BUFFER_POOL00_BUF_COUNT,
                               (Ptr)PAL_CPPI_OFDM1_BUFFER_RAM_INTERNAL_ADDRESS)) == NULL)
    {
        printk(KERN_ERR "PAL_cppi4BufPoolDirectInit for pool %d [CPPI41_DOMAIN_DSG] FAILED\n", tmpBufPool.bPool);
        return -1;
    }
#endif

    /* SC-QAM DSG Buffer Pool init */
    tmpBufPool.bPool = PAL_CPPI_SC_QAM0_BUFFER_POOL00;
    tmpBufPool.bMgr = PAL_CPPI_SC_QAM0_BUF_MGR;
    if ((PAL_cppi4BufPoolDirectInit(palHandle, tmpBufPool,
                                    PAL_CPPI_DS_DSG_BUFFER_POOL00_REF_CNT,
                                    PAL_CPPI_DS_DSG_BUFFER_POOL00_BUF_SIZE,
                                    PAL_CPPI_SC_QAM0_BUFFER_POOL00_BUF_COUNT,
                               (Ptr)PAL_CPPI_SC_QAM0_BUFFER_RAM_INTERNAL_ADDRESS)) == NULL)
    {
        printk(KERN_ERR "PAL_cppi4BufPoolDirectInit for pool %d [CPPI41_DOMAIN_DSG] FAILED\n", tmpBufPool.bPool);
        return -1;
    }

    /****************************************************/
    /*********** Setup DSG descriptors ******************/
    /****************************************************/
    for (i = 0; i < sizeof(gDsgFDqueues)/sizeof(gDsgFDqueues[0]); i++)     /* Go over all free decriptors queues list */
    {
        DsgCppiDescType_t* currDesc = NULL;

        switch (gDsgFDqueues[i].qMgr)
        {
            case PAL_CPPI_OFDM0_QUEUE_MGR:
                currDesc = (DsgCppiDescType_t *)PAL_CPPI_OFDM0_DESC_RAM_BASE;
                break;

            case PAL_CPPI_OFDM1_QUEUE_MGR:
                currDesc = (DsgCppiDescType_t *)PAL_CPPI_OFDM1_DESC_RAM_BASE;
                break;

            case PAL_CPPI_SC_QAM0_QUEUE_MGR:
                currDesc = (DsgCppiDescType_t *)PAL_CPPI_SC_QAM0_DESC_RAM_BASE;
                break;

            default:
                printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d index %d FAILED",gDsgFDqueues[i].qMgr,i);
            return -1;

        }

        tmpQ.qMgr = gDsgFDqueues[i].qMgr;
        tmpQ.qNum = gDsgFDqueues[i].qId;
        tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
        if (tmpQHnd == NULL)
        {
            printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_DSG] FAILED", tmpQ.qMgr, tmpQ.qNum);
            return 0;
        }


        memset((void *)currDesc, 0, gDsgFDqueues[i].descSize * gDsgFDqueues[i].descCount);

        for (descCount = 0; descCount < gDsgFDqueues[i].descCount; descCount++)
        {
            currDesc->hw.descInfo = CPPI41_EM_DESCINFO_DTYPE_EMBEDDED | (PAL_CPPI_DSG_PSI_SIZE << CPPI41_EM_DESCINFO_PSWSIZE_SHIFT);
            currDesc->hw.tagInfo  = 0;
            currDesc->hw.pktInfo  = (1 << CPPI41_EM_PKTINFO_RETPOLICY_SHIFT)        |
                                    (1 << CPPI41_EM_PKTINFO_ONCHIP_SHIFT)           |
                                    (0 << CPPI41_EM_PKTINFO_RETQMGR_SHIFT)  |
                                    (tmpQ.qNum << CPPI41_EM_PKTINFO_RETQ_SHIFT);

            currDesc->psi[0] = DS_EMBEDDED_DESC_BPI_CMD_TEMPLATE;
            currDesc->psi[1] = DS_EMBEDDED_DESC_CRC_CMD_TEMPLATE;
            currDesc->psi[2] = DS_EMBEDDED_DESC_PHS_CMD_TEMPLATE;
            currDesc->psi[3] = DS_EMBEDDED_DESC_RX_FIFO_0_CMD_TEMPLATE;
            currDesc->psi[4] = DS_EMBEDDED_DESC_RX_FIFO_1_CMD_TEMPLATE;

            PAL_CPPI4_CACHE_WRITEBACK(currDesc, gDsgFDqueues[i].descSize);

            PAL_cppi4QueuePush(tmpQHnd, (Ptr)DSG_INTERNAL_OFFSET((Uint32)currDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(gDsgFDqueues[i].descSize), 0);

            currDesc = (DsgCppiDescType_t*)((Uint32)currDesc + gDsgFDqueues[i].descSize);
        }
    }


    /*******************************************************/
    /*********** Setup DSG RX DMA channels *****************/
    /*******************************************************/
    for (dmaNum = PAL_CPPI_OFDM0_RX_DMA; dmaNum <= PAL_CPPI_SC_QAM0_RX_DMA; dmaNum++)
    {
#ifdef CONFIG_MACH_PUMA7_FPGA
        if (PAL_CPPI_OFDM1_RX_DMA == dmaNum)
        {
            continue;
        }
#endif

        tmpBufPool.bMgr = 0;  // DSG has to see it as 0 always.

        for (chNum = 0; chNum < PAL_CPPI_DSG_NUM_TOTAL_CHAN; chNum++)
        {
            rxCh.chNum              = chNum;
            rxCh.dmaNum             = dmaNum;
            rxCh.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
            rxCh.sopOffset          = 0;
            rxCh.rxCompQueue.qMgr   = PAL_CPPI_DMA_NUM_TO_QMGR_NUM(dmaNum);
            rxCh.rxCompQueue.qNum   = PAL_CPPI_DSG_QMGR_Q_0;
            rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_DMA_NUM_TO_QMGR_NUM(dmaNum);
            rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_DSG_QMGR_FD_EMB_Q_NUM;
            rxCh.u.embeddedPktCfg.numBufSlot = (EMSLOTCNT - 1);
            rxCh.u.embeddedPktCfg.sopSlotNum = 0;
            rxCh.u.embeddedPktCfg.fBufPool[0] = tmpBufPool;
            rxCh.u.embeddedPktCfg.fBufPool[1] = tmpBufPool;
            rxCh.u.embeddedPktCfg.fBufPool[2] = tmpBufPool;
            rxCh.u.embeddedPktCfg.fBufPool[3] = tmpBufPool;
            rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);
            if(rxChHdl == NULL)
            {
                printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, rxChNum=%d\n", __FUNCTION__, __LINE__, dmaNum, rxCh.chNum);
                return(-1);
            }
            PAL_cppi4EnableRxChannel(rxChHdl, NULL);
            DEBUG_PAL("%s: Open and enable dmaNum=%d, rxChNum=%d\n", __FUNCTION__, dmaNum, rxCh.chNum);
        }
    }

    /*******************************************************/
    /*********** Setup DSG TX DMA channels *****************/
    /*******************************************************/
    for (dmaNum = PAL_CPPI_OFDM0_TX_DMA; dmaNum <= PAL_CPPI_SC_QAM0_TX_DMA; dmaNum++)
    {
#ifdef CONFIG_MACH_PUMA7_FPGA
        if (PAL_CPPI_OFDM1_TX_DMA == dmaNum)
        {
            continue;
        }
#endif
        txCh.chNum          = 0;
        txCh.dmaNum         = dmaNum;
        txCh.tdQueue.qMgr   = PAL_CPPI_DMA_NUM_TO_QMGR_NUM(dmaNum);
        txCh.tdQueue.qNum   = PAL_CPPI_DSG_QMGR_TEARDOWN_Q_NUM;
        txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);
        if(txChHdl == NULL)
        {
            printk (KERN_CRIT "%s[%d]: Unable to open dmaNum=%d, txChNum=%d\n", __FUNCTION__, __LINE__, dmaNum, txCh.chNum);
            return(-1);
        }
        PAL_cppi4EnableTxChannel(txChHdl, NULL);
        DEBUG_PAL("%s: Open and enable dmaNum=%d, txChNum=%d\n", __FUNCTION__, dmaNum, txCh.chNum);
    }

/*
    ----------------------------------------------------------------------------

    ##     ##  ######     ########   #######  ##     ##    ###    #### ##    ##
    ##     ## ##    ##    ##     ## ##     ## ###   ###   ## ##    ##  ###   ##
    ##     ## ##          ##     ## ##     ## #### ####  ##   ##   ##  ####  ##
    ##     ##  ######     ##     ## ##     ## ## ### ## ##     ##  ##  ## ## ##
    ##     ##       ##    ##     ## ##     ## ##     ## #########  ##  ##  ####
    ##     ## ##    ##    ##     ## ##     ## ##     ## ##     ##  ##  ##   ###
     #######   ######     ########   #######  ##     ## ##     ## #### ##    ##

    ----------------------------------------------------------------------------

    The DOCSIS US Domain related initializatios are in docsis_cppi_puma7 module.
*/

/*
    ----------------------------------------------------------------------------

    ########  ########     ########   #######  ##     ##    ###    #### ##    ##
    ##     ## ##     ##    ##     ## ##     ## ###   ###   ## ##    ##  ###   ##
    ##     ## ##     ##    ##     ## ##     ## #### ####  ##   ##   ##  ####  ##
    ########  ########     ##     ## ##     ## ## ### ## ##     ##  ##  ## ## ##
    ##        ##           ##     ## ##     ## ##     ## #########  ##  ##  ####
    ##        ##           ##     ## ##     ## ##     ## ##     ##  ##  ##   ###
    ##        ##           ########   #######  ##     ## ##     ## #### ##    ##

    ----------------------------------------------------------------------------
*/

    palHandle = PAL_cppi4Init(NULL, CPPI41_DOMAIN_PP);

    /* No need to initialize buffer pools and free queues since they are shared and were already initialized */

    if (avalanche_cppi_init_pp_docsis_dma_channels(palHandle))
    {
        printk (KERN_ERR "ERROR: avalanche_cppi_init_pp_docsis_dma_channels FAILED\n");
        return (-1);
    }

    return 0;
}

#ifdef CONFIG_INTEL_KERNEL_VOICE_SUPPORT
int Puma_VoiceDSP_CPPI_Init(void)
{
    PAL_Handle          palHandle;
    Uint32              i;
    Cppi4Queue          tmpQ;
    PAL_Cppi4QueueHnd   tmpQHnd;
    Ptr                 currDescRegionPtr;
    Uint32              descCount;
    Cppi4BufPool        tmpBufPool;
    Cppi4TxChInitCfg    txCh;
    Cppi4RxChInitCfg    rxCh;
    PAL_Cppi4TxChHnd    txChHdl;
    PAL_Cppi4RxChHnd    rxChHdl;

    currDescRegionPtr = NULL;
    palHandle = PAL_cppi4Init(NULL, CPPI41_DOMAIN_PP);
    if(palHandle == NULL)
    {
        printk(KERN_CRIT "PAL_cppi4Init FAILED\n");
        return -1;
    }

    /**********************************************/
    /******** Setup VOICE DSP descriptors *********/
    /**********************************************/

    for (i = 0; i < sizeof(gPpFDqueues)/sizeof(gPpFDqueues[0]); i++)     /* Go over all free decriptors queues list */
    {
        if (gPpFDqueues[i].descRegion == PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION)
        {
            if (PAL_cppi4IsDescRegionAllocated(palHandle, gPpFDqueues[i].qMgr, gPpFDqueues[i].descRegion) == False)
            {
                /* This is the first queue within this region - allocate all the region now */
                currDescRegionPtr = PAL_cppi4AllocDescRegion(palHandle, gPpFDqueues[i].qMgr, gPpFDqueues[i].descRegion);
                if (currDescRegionPtr == NULL)
                {
                    printk(KERN_CRIT "Descriptor region %d allocation FAILED\n", gPpFDqueues[i].descRegion);
                    return -1;
                }

                currDescRegionPtr = (Ptr)avalanche_no_OperSys_memory_phys_to_virt(currDescRegionPtr);
                descCount = gCppiPpInitCfg.queueMgrInfo[gPpFDqueues[i].qMgr].descRegion[gPpFDqueues[i].descRegion].numDesc;
                PAL_osMemSet(currDescRegionPtr, 0, descCount * gPpFDqueues[i].descSize);
                DEBUG_PAL("Descriptor region %d allocation, base address = %p, descCount = %d\n", gPpFDqueues[i].descRegion, currDescRegionPtr, descCount);
            }

            tmpQ.qMgr = gPpFDqueues[i].qMgr;
            tmpQ.qNum = gPpFDqueues[i].qId;
            tmpQHnd = PAL_cppi4QueueOpen(palHandle, tmpQ);
            if (tmpQHnd == NULL)
            {
                printk(KERN_CRIT "PAL_cppi4QueueOpen for qMgr %d, qNum %d [CPPI41_DOMAIN_PP] FAILED", tmpQ.qMgr, tmpQ.qNum);
                return -1;
            }
            if (currDescRegionPtr == NULL)
            {
                printk(KERN_CRIT "Descriptor region %d allocation FAILED currDescRegionPtr = NULL \n", gPpFDqueues[i].descRegion);
                return -1;
            }
            for (descCount = 0; descCount < gPpFDqueues[i].descCount; descCount++)
            {
                Cppi4EmbdDescPp* currDesc = (Cppi4EmbdDescPp*)currDescRegionPtr;

                currDesc->descInfo = CPPI41_EM_DESCINFO_DTYPE_EMBEDDED | CPPI41_EM_DESCINFO_SLOTCNT_PP;
                currDesc->tagInfo  = (PAL_CPPI_PP_VOICE_DSP_C55_SOURCE_PORT << CPPI41_EM_TAGINFO_SRCPORT_SHIFT) | 0x3fff;
                currDesc->pktInfo  = (PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH << CPPI41_EM_PKTINFO_PKTTYPE_SHIFT)               |
                                     CPPI41_EM_PKTINFO_RETPOLICY_RETURN                                                 |
                                     (tmpQ.qMgr << CPPI41_EM_PKTINFO_RETQMGR_SHIFT)                                     |
                                     (tmpQ.qNum << CPPI41_EM_PKTINFO_RETQ_SHIFT);

                PAL_CPPI4_CACHE_WRITEBACK(currDesc, gPpFDqueues[i].descSize);
                PAL_cppi4QueuePush(tmpQHnd, (Ptr)avalanche_no_OperSys_memory_virt_to_phys((Uint32)currDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(gPpFDqueues[i].descSize), 0);

                currDescRegionPtr = (Ptr)((Uint32)currDescRegionPtr + gPpFDqueues[i].descSize);
            }
        }
    }

    /*****************************************************/
    /*********** Init VOICE DSP buffer pool **************/
    /*****************************************************/
    {
        Uint32* poolPhysAddr;

        if (avalanche_alloc_no_OperSys_memory(eNO_OperSys_VoiceNI, PAL_CPPI_PP_VOICE_DSP_BUFFER_COUNT * PAL_CPPI_PP_VOICE_DSP_BUFFER_SIZE, (unsigned int *)&poolPhysAddr) != 0)
        {
            printk(KERN_ERR "ERROR: avalanche_alloc_no_OperSys_memory for buffer pool failed\n");
            return -1;
        }

        tmpBufPool.bMgr = PAL_CPPI_PP_BUF_MGR;
        tmpBufPool.bPool = PAL_CPPI_PP_VOICE_DSP_BUFFER_POOL;
        if((PAL_cppi4BufPoolDirectInit(palHandle, tmpBufPool, PAL_CPPI_PP_VOICE_DSP_BUFFER_REF_CNT, PAL_CPPI_PP_VOICE_DSP_BUFFER_SIZE, PAL_CPPI_PP_VOICE_DSP_BUFFER_COUNT, (void*)poolPhysAddr)) == NULL)
        {
            printk ("PAL_cppi4BufPoolDirectInit for pool %d failed\n", tmpBufPool.bPool);
            return -1;
        }
    }

    /****************************/
    /* Setup VOICE DMA Channels */
    /****************************/

    /* Set up Rx channel */
    rxCh.dmaNum             = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
    rxCh.chNum              = PAL_CPPI_PP_DMA10_VOICE_DSP_INFRA_RX_CH;
    rxCh.defDescType        = CPPI41_DESC_TYPE_EMBEDDED;
    rxCh.sopOffset          = 0;
    rxCh.rxCompQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    rxCh.rxCompQueue.qNum   = PAL_CPPI_PP_QMGR_G2_VOICE_DSP_TX_QNUM;
    rxCh.u.embeddedPktCfg.fdQueue.qMgr = PAL_CPPI_PP_QMGR_G2;
    rxCh.u.embeddedPktCfg.fdQueue.qNum = PAL_CPPI_PP_QMGR_G2_VOICE_INFRA_RX_EMB_FD_Q_NUM;
    rxCh.u.embeddedPktCfg.numBufSlot = EMSLOTCNT_PP - 1;
    rxCh.u.embeddedPktCfg.sopSlotNum = 0;
    rxCh.u.embeddedPktCfg.fBufPool[0].bMgr  = PAL_CPPI_PP_BUF_MGR;
    rxCh.u.embeddedPktCfg.fBufPool[0].bPool = PAL_CPPI_PP_VOICE_DSP_BUFFER_POOL;
    rxChHdl = PAL_cppi4RxChOpen(palHandle, &rxCh, NULL);

    /* Set up Tx channel */
    txCh.dmaNum         = PAL_CPPI_PP_DMA10_INFRA_RX_TX;
    txCh.chNum          = PAL_CPPI_PP_DMA10_VOICE_DSP_INFRA_TX_CH;
    txCh.tdQueue.qMgr   = PAL_CPPI_PP_QMGR_G2;
    txCh.tdQueue.qNum   = PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM;

    txChHdl = PAL_cppi4TxChOpen(palHandle, &txCh, NULL);

    if (!txChHdl || !rxChHdl)
    {
        printk ("ERROR in %s: infra channel setup failed for dma %d, channel %d\n", __FUNCTION__, rxCh.dmaNum, rxCh.chNum);
        return -1;
    }

    /* Enable Tx-Rx channels */
    PAL_cppi4EnableRxChannel(rxChHdl, NULL);
    PAL_cppi4EnableTxChannel(txChHdl, NULL);

    return 0;
}
#endif

core_param(memProfile, memProfile, int, 0644);
EXPORT_SYMBOL(Puma_DOCSIS_CPPI_Init);
#ifdef CONFIG_INTEL_KERNEL_VOICE_SUPPORT
EXPORT_SYMBOL(Puma_VoiceDSP_CPPI_Init);
#endif
