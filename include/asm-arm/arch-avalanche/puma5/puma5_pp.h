/*
 * puma5_pp.h
 * Description:
 * See below.
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
 */

/*
 * This file contains Packet Processor configuration. All the configurations
 * here are system level and hence a complete knowledge of configurations of all
 * devices/drivers related to the Packet Processor is required before modifying
 * any value.
 *
 * The private data structures used by avalanche_ppd_init() are also present in
 * this file and should not be modified unless the function avalanche_ppd_init
 * in file puma5_pp.c is also required to be modified for PP configuration.
 *
 */

#ifndef _INCLUDE_PUMA5_PP_H
#define _INCLUDE_PUMA5_PP_H

#define MAX_IP_PACKET_SIZE      1514
#define MIN_IP_PACKET_SIZE      64

/* Prefetcher configuration
 *  - Setup prefetch to fetch 96 bytes with a 24 byte offset
 */
#define PREFETCH_DATA_SIZE          96
#define PREFETCH_DATA_OFFSET        24

#define PREFETCH_FD_SIZE            64
#define PREFETCH_BUFF_SIZE          128

/*
 * PID Range configurations
 *  -   The following macros cover the complete PID range organized for PP
 *      system and needs to be obeyed by all the drivers in PP system.
 *  -   Currently only Ethernet configuration is present but new configurations
 *      (ranges) need to be added per newly added driver in PP system
 */
#define PP_ETH_PID_COUNT    1
#define PP_ETH_PID_BASE     31

#define PP_CNI_PID_COUNT    1
#define PP_CNI_PID_BASE     9

#define PP_USB_PID_COUNT    4
#define PP_USB_PID_BASE     26

#define PP_C55_PID_COUNT    1   /* configuration for C55 DSP */
#define PP_C55_PID_BASE     15  /* configuration for C55 DSP */

#define ACMD_CONFIG_PREFETCH    0x84
#define ACMD_ENABLE_PREFETCH    0x83
#define ACMD_READ_STATISTICS    0x86
#define ACMD_CONFIG_TDQ         0x87
#define ACMD_COMMAND_SHIFT      8
#define ACMD_COMMAND_MASK       (0xFFu<<ACMD_COMMAND_SHIFT)
#define ACMD_COMMAND(x)         (((x)<<ACMD_COMMAND_SHIFT)&ACMD_COMMAND_MASK)
#define ACMD_GET_COMMAND(x)     (((x)&ACMD_COMMAND_MASK)>>ACMD_COMMAND_SHIFT)
#define ACMD_INDEX_SHIFT        0
#define ACMD_INDEX_MASK         (0xFFu<<ACMD_INDEX_SHIFT)
#define ACMD_INDEX(x)           (((x)<<ACMD_INDEX_SHIFT)&ACMD_INDEX_MASK)
#define ACMD_RETCODE_SHIFT      24
#define ACMD_RETCODE_MASK       (0xFFu<<ACMD_RETCODE_SHIFT)
#define ACMD_GET_RETCODE(x)     (((x)&ACMD_RETCODE_MASK)>>ACMD_RETCODE_SHIFT)

/*
 * Data structures used to hold and perform various PP configurations
 */
typedef volatile struct
{
    CSL_Reg32 Command;
    CSL_Reg32 Parameter0;
    CSL_Reg32 Parameter1;
    CSL_Reg32 Parameter2;
    CSL_Reg32 Parameter3;
} APDSP_Command_Buffer;

typedef APDSP_Command_Buffer *APDSP_Command_Buffer_RegsOvly;

/*
 * PrefchCfg -
 * Description:
 *  Prefetcher Configuration
 */
typedef struct
{
    Uint32 pfDescCnt;
    volatile Uint32* pfDescBase;
    Cppi4Queue pfFQ;

    Uint32 repDescCnt;
    volatile Uint32* repDescBase;
    Cppi4Queue repFQ;

    Uint32 pfBlkCnt;
    volatile Uint32* pfBlkBase;
    Cppi4Queue pfFBQ;

    Uint8 pfDataSize;
    Uint8 pfDataOffset;
    APDSP_Command_Buffer_RegsOvly  pfCmdBase;
} PrefchCfg;

/* TI_PP_PREF_STATS -
 *  Prefetcher statistics structure
 */
typedef struct
{
    Uint32  grp_a_preproc_pkts;         /* Group A: Preprocessed Packets */
    Uint32  grp_a_pref_buf_pkts;        /* Group A: Packets with pre-fetched
                                           buffers */
    Uint32  grp_a_pref_descbuff_pkts;   /* Group A: Packets with pre-fetched
                                           descriptors and buffers */
    Uint32  grp_a_desc_starv_cnt;       /* Group A: Packet Pushback */

    Uint32  grp_b_preproc_pkts;         /* Group B: Preprocessed Packets */
    Uint32  grp_b_pref_buf_pkts;        /* Group B: Packets with pre-fetched
                                           buffers */
    Uint32  grp_b_pref_descbuff_pkts;   /* Group B: Packets with pre-fetched
                                           descriptors and buffers */
    Uint32  grp_b_desc_starv_cnt;       /* Group B: Packet Pushback */

    Uint32  in_q_congst_discards[6];    /* Input Queue (0-5): Packet Congestion
                                           Discards */
    Uint32  in_q_congst_thrsh[6];       /* Input Queue (0-5): Packet Congestion
                                           Threshold */
} TI_PP_PREF_STATS;

/*
 * ti_pp_get_pref_stats -
 *
 * Description:
 *  This API provides the prefetcher statistics. Note that the statictics in
 *  firmware are NOT cleared by this API.
 *
 * Precondition:
 *  APDSP must be up and prefetcher must have been initialized.
 *
 * Parameters:
 *  stats (OUT)         - Prefetcher statistics.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_pp_get_pref_stats (TI_PP_PREF_STATS *stats);

/*
 * ti_pp_get_n_clear_pref_stats -
 *
 * Description:
 *  This API provides the prefetcher statistics. The statictics are cleared.
 *
 * Precondition:
 *  APDSP must be up and prefetcher must have been initialized.
 *
 * Parameters:
 *  stats (OUT)         - Prefetcher statistics.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_pp_get_n_clear_pref_stats (TI_PP_PREF_STATS *stats);

/*
 * ti_pp_enable_psm -
 *
 * Description:
 *  This API enables the PP PSM mode. First the packet processor is set in PSM
 *  by using PPD API which also halts the PDSPs and then prefetcher PSM is
 *  enabled so that packets are forwarded as per pid configuration as well as tx
 *  is performed as per desc info bypassing PP.
 *
 * Precondition:
 *  PPD must have been initialized with ti_ppd_init, APDSP must be up and
 *  prefetcher must have been initialized.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_pp_enable_psm (void);

/*
 * ti_pp_disable_psm -
 *
 * Description:
 *  This API disables the PP PSM mode.First the prefetcher PSM is disabled and
 *  then packet processor PSM is disabled.
 *
 * Precondition:
 *  PSM must have been enabled using ti_pp_enable_psm.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_pp_disable_psm (void);

#endif /* !_INCLUDE_PUMA5_PP_H */
