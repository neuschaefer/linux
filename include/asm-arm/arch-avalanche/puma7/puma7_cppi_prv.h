/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014 - 2017 Intel Corporation.

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

  BSD LICENSE

  Copyright(c) 2014 - 2017 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifdef __KERNEL__
#include <generated/autoconf.h>
#endif
#include "puma7_cppi_lqmgr_q.h"
#include "puma7_cppi_gqmgr0_q.h"
#include "puma7_cppi_gqmgr1_q.h"
#include "puma7_cppi_gqmgr2_q.h"
#include "puma7_cppi_dsgqmgr_q.h"
#include "puma7_cppi_usqmgr_q.h"
#include "puma7_cppi_bmgr_pools.h"
#ifndef CONFIG_ARM_AVALANCHE_SOC
#include <linux/netip_mem_util.h>
#endif
#ifndef PUMA7_CPPI_PRV_H
#define PUMA7_CPPI_PRV_H
#ifndef CONFIG_ARM_AVALANCHE_SOC
#define IO_ADDRESS(addr) ((unsigned long)netip_mmio_to_virtual(addr))
#endif
/**********************************************************************************************************************

 #######  ##     ## ######## ##     ## ########    ##     ##    ###    ##    ##    ###     ######   ######## ########
##     ## ##     ## ##       ##     ## ##          ###   ###   ## ##   ###   ##   ## ##   ##    ##  ##       ##     ##
##     ## ##     ## ##       ##     ## ##          #### ####  ##   ##  ####  ##  ##   ##  ##        ##       ##     ##
##     ## ##     ## ######   ##     ## ######      ## ### ## ##     ## ## ## ## ##     ## ##   #### ######   ########
##  ## ## ##     ## ##       ##     ## ##          ##     ## ######### ##  #### ######### ##    ##  ##       ##   ##
##    ##  ##     ## ##       ##     ## ##          ##     ## ##     ## ##   ### ##     ## ##    ##  ##       ##    ##
 ##### ##  #######  ########  #######  ########    ##     ## ##     ## ##    ## ##     ##  ######   ######## ##     ##

**********************************************************************************************************************/
#undef PAL_CPPI_QMGR_Q_ADD
#define PAL_CPPI_QMGR_Q_ADD(qNumber)       qNumber,

/*
+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+-+
|P|P| |Q|u|e|u|e| |M|a|n|a|g|e|r|s|
+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+-+
*/
typedef enum PAL_CPPI_PP_QMGRs
{
    PAL_CPPI_PP_QMGR_G0,
    PAL_CPPI_PP_QMGR_G1,
    PAL_CPPI_PP_QMGR_G2,
    PAL_CPPI_PP_QMGR_LOCAL,
    PAL_CPPI41_NUM_QUEUE_MGR                                                                        /* PP has the maximum number of queue managers */
}PAL_CPPI_PP_QMGRs_e;

/*
+-+-+ +-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
|P|P| |L|o|c|a|l| |Q|u|e|u|e| |M|a|n|a|g|e|r| |Q|u|e|u|e|s|
+-+-+ +-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
*/

#define PAL_CPPI_PP_QMGR_LOCAL_REGS_BASE            (IO_ADDRESS(0xF3480000))
#define PAL_CPPI_PP_QMGR_LOCAL_DESC_BASE            (IO_ADDRESS(0xF3490000))
#define PAL_CPPI_PP_QMGR_LOCAL_QUEUES_BASE          (IO_ADDRESS(0xF34A0000))
#define PAL_CPPI_PP_QMGR_LOCAL_Q_STATS_BASE         (IO_ADDRESS(0xF34B0000))
#define PAL_CPPI_PP_QMGR_LOCAL_LINKING_RAM_BASE     (0xF34C0000)
#define PAL_CPPI_PP_QMGR_LOCAL_LINKING_RAM_SIZE     512


typedef enum PAL_CPPI_PP_QMGR_LOCAL_Qs
{
    PAL_CPPI_PP_QMGR_LOCAL_Q_LIST
    PAL_CPPI_PP_QMGR_LOCAL_TOTAL_Q_COUNT
}PAL_CPPI_PP_QMGR_LOCAL_Qs_e;


/*
+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
|P|P| |G|l|o|b|a|l|0| |Q|u|e|u|e| |M|a|n|a|g|e|r| |Q|u|e|u|e|s|
+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
*/


#define PAL_CPPI_PP_QMGR_G0_REGS_BASE                               (IO_ADDRESS(0xF9100000))
#define PAL_CPPI_PP_QMGR_G0_DESC_BASE                               (IO_ADDRESS(0xF9110000))
#define PAL_CPPI_PP_QMGR_G0_QUEUES_BASE                             (IO_ADDRESS(0xF9120000))
#define PAL_CPPI_PP_QMGR_G0_Q_STATS_BASE                            (IO_ADDRESS(0xF9130000))
#define PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_BASE                    (0xF9140000)
#ifdef CONFIG_MACH_PUMA7_FPGA
#ifdef CONFIG_MACH_PUMA7_FPGA_PP
#define PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_SIZE                    16*1024
#else
#define PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_SIZE                    512
#endif
#else
#define PAL_CPPI_PP_QMGR_GLOBAL_LINKING_RAM_SIZE                    ((64*1024)-1)
#endif
#define PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE                   2048

typedef enum PAL_CPPI_PP_QMGR_G0_Qs
{
    PAL_CPPI_PP_QMGR_G0_Q_LIST
    PAL_CPPI_PP_QMGR_G0_TOTAL_Q_COUNT
}PAL_CPPI_PP_QMGR_G0_Qs_e;


/*
+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
|P|P| |G|l|o|b|a|l|1| |Q|u|e|u|e| |M|a|n|a|g|e|r| |Q|u|e|u|e|s|
+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
*/

#define PAL_CPPI_PP_QMGR_G1_REGS_BASE               (IO_ADDRESS(0xF9300000))
#define PAL_CPPI_PP_QMGR_G1_DESC_BASE               (IO_ADDRESS(0xF9310000))
#define PAL_CPPI_PP_QMGR_G1_QUEUES_BASE             (IO_ADDRESS(0xF9320000))
#define PAL_CPPI_PP_QMGR_G1_Q_STATS_BASE            (IO_ADDRESS(0xF9330000))
#define PAL_CPPI_PP_QMGR_G1_Q_PROXY_BASE            (IO_ADDRESS(0xF9360000))

typedef enum PAL_CPPI_PP_QMGR_G1_Qs
{
    PAL_CPPI_PP_QMGR_G1_Q_LIST
    PAL_CPPI_PP_QMGR_G1_TOTAL_Q_COUNT
}PAL_CPPI_PP_QMGR_G1_Qs_e;

#define PAL_CPPI_PP_QMGR_G1_QOS_Q_BASE              PAL_CPPI_PP_QMGR_G1_QOS_Q0
#define PAL_CPPI_PP_QMGR_G1_QOS_Q_LAST              PAL_CPPI_PP_QMGR_G1_QOS_Q255

/*
+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
|P|P| |G|l|o|b|a|l|2| |Q|u|e|u|e| |M|a|n|a|g|e|r| |Q|u|e|u|e|s|
+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+
*/

#define PAL_CPPI_PP_QMGR_G2_REGS_BASE               (IO_ADDRESS(0xF9380000))
#define PAL_CPPI_PP_QMGR_G2_DESC_BASE               (IO_ADDRESS(0xF9390000))
#define PAL_CPPI_PP_QMGR_G2_QUEUES_BASE             (IO_ADDRESS(0xF93A0000))
#define PAL_CPPI_PP_QMGR_G2_Q_STATS_BASE            (IO_ADDRESS(0xF93B0000))
#define PAL_CPPI_PP_QMGR_G2_Q_PROXY_BASE            (IO_ADDRESS(0xF93E0000))

typedef enum PAL_CPPI_PP_QMGR_G2_Qs
{
    PAL_CPPI_PP_QMGR_G2_Q_LIST
    PAL_CPPI_PP_QMGR_G2_TOTAL_Q_COUNT
}PAL_CPPI_PP_QMGR_G2_Qs_e;

#define PAL_CPPI_CONCAT(a, b)       a ## b

#define PAL_CPPI_PP_QMGR_QUEUES_BASE(qMgr)                                      \
    PAL_CPPI_CONCAT(qMgr,_QUEUES_BASE)

#define PAL_CPPI_PP_QMGR_QUEUES_PHY_ADDR(qMgr, qNum)                            \
    ((qNum << 4) + PAL_CPPI_PP_QMGR_QUEUES_BASE(qMgr))

/*
+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+-+
|D|S|G| |Q|u|e|u|e| |M|a|n|a|g|e|r|s|
+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+-+
*/

typedef enum PAL_CPPI_DSG_QUEUE_MNGRS
{
    PAL_CPPI_OFDM0_QUEUE_MGR,                        /* OFDM0  */
    PAL_CPPI_OFDM1_QUEUE_MGR,                        /* OFDM1  */
    PAL_CPPI_SC_QAM0_QUEUE_MGR,                      /* SC-QAM */
    PAL_CPPI_DSG_NUM_QUEUE_MGR
}PAL_CPPI_DSG_QUEUE_MNGRS_e;

/* DSGs Common Queue Manager Info */
#define PAL_CPPI_DS_DSG_FD_EMB_DESC_SIZE                        128     /* Descriptor Size in Bytes */
#define PAL_CPPI_DSG_DESC_RAM_INTERNAL_ADDRESS                  0x40000
#define PAL_CPPI_DSG_QUEUE_MGR_LINKING_RAM_INTERNAL_ADDRESS     0x45000

/* OFDM0 DSG Queue Manager Info */
#define PAL_CPPI_OFDM0_QUEUE_MGR_QUEUES_BASE            (IO_ADDRESS(0xF2160000))
#define PAL_CPPI_OFDM0_QUEUE_MGR_REGS_BASE              (IO_ADDRESS(0xF2162000))
#define PAL_CPPI_OFDM0_QUEUE_MGR_DESC_BASE              (IO_ADDRESS(0xF2163000))
#define PAL_CPPI_OFDM0_QUEUE_MGR_Q_STATS_BASE           (IO_ADDRESS(0xF2164000))
#define PAL_CPPI_OFDM0_DESC_RAM_BASE                    (IO_ADDRESS(0xF2100000 + PAL_CPPI_DSG_DESC_RAM_INTERNAL_ADDRESS))
#define PAL_CPPI_OFDM0_QUEUE_MGR_LINKING_RAM_SIZE        128    /* Linkning RAM descriptors capacity */
#define PAL_CPPI_OFDM0_FD_EMB_DESC_COUNT                 64     /* Descriptors RAM descriptors capacity */

/* OFDM1 DSG Queue Manager Info */
#define PAL_CPPI_OFDM1_QUEUE_MGR_QUEUES_BASE            (IO_ADDRESS(0xF21E0000))
#define PAL_CPPI_OFDM1_QUEUE_MGR_REGS_BASE              (IO_ADDRESS(0xF21E2000))
#define PAL_CPPI_OFDM1_QUEUE_MGR_DESC_BASE              (IO_ADDRESS(0xF21E3000))
#define PAL_CPPI_OFDM1_QUEUE_MGR_Q_STATS_BASE           (IO_ADDRESS(0xF21E4000))
#define PAL_CPPI_OFDM1_DESC_RAM_BASE                    (IO_ADDRESS(0xF2180000 + PAL_CPPI_DSG_DESC_RAM_INTERNAL_ADDRESS))
#define PAL_CPPI_OFDM1_QUEUE_MGR_LINKING_RAM_SIZE        128    /* Linkning RAM descriptors capacity */
#define PAL_CPPI_OFDM1_FD_EMB_DESC_COUNT                 64     /* Descriptors RAM descriptors capacity */

/* SC-QAM0 DSG Queue Manager Info */
#define PAL_CPPI_SC_QAM0_QUEUE_MGR_QUEUES_BASE          (IO_ADDRESS(0xF2060000))
#define PAL_CPPI_SC_QAM0_QUEUE_MGR_REGS_BASE            (IO_ADDRESS(0xF2062000))
#define PAL_CPPI_SC_QAM0_QUEUE_MGR_DESC_BASE            (IO_ADDRESS(0xF2063000))
#define PAL_CPPI_SC_QAM0_QUEUE_MGR_Q_STATS_BASE         (IO_ADDRESS(0xF2064000))
#define PAL_CPPI_SC_QAM0_DESC_RAM_BASE                  (IO_ADDRESS(0xF2000000 + PAL_CPPI_DSG_DESC_RAM_INTERNAL_ADDRESS))
#define PAL_CPPI_SC_QAM0_QUEUE_MGR_LINKING_RAM_SIZE      256    /* Linkning RAM descriptors capacity */
#define PAL_CPPI_SC_QAM0_FD_EMB_DESC_COUNT               128    /* Descriptors RAM descriptors capacity */


typedef enum PAL_CPPI_DSG_QMGR_Qs
{
    PAL_CPPI_DSG_QMGR_Q_LIST
    PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT
}PAL_CPPI_DSG_QMGR_Qs_e;


/*
+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
|U|S| |Q|u|e|u|e| |M|a|n|a|g|e|r|
+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
*/

typedef enum PAL_CPPI_MAC_US_QMGRs
{
    PAL_CPPI_MAC_US_QMGR_DOCSIS = 3
}
PAL_CPPI_MAC_US_QMGRs_e;

#define PAL_CPPI_MAC_US_QMGR_REGS_BASE              (IO_ADDRESS(0xF2500000))
#define PAL_CPPI_MAC_US_QMGR_DESC_BASE              (IO_ADDRESS(0xF2530000))
#define PAL_CPPI_MAC_US_QMGR_QUEUES_BASE            (IO_ADDRESS(0xF2510000))
#define PAL_CPPI_MAC_US_QMGR_Q_STATS_BASE           (IO_ADDRESS(0xF2520000))
#define PAL_CPPI_MAC_US_QMGR_LINKING_RAM_BASE       (IO_ADDRESS(0xF2580000))
#define PAL_CPPI_MAC_US_QMGR_LINKING_RAM_SIZE       (8*1024)

typedef enum PAL_CPPI_MAC_US_QMGR_Qs
{
    PAL_CPPI_MAC_US_QMGR_DOCSIS_Q_LIST
    PAL_CPPI_MAC_US_QMGR_TOTAL_Q_COUNT
}
PAL_CPPI_MAC_US_QMGR_Qs_e;

#define PAL_CPPI_MAC_US_DESC_SIZE_MONO              128
#define PAL_CPPI_MAC_US_DESC_SIZE_EMB               32
#define PAL_CPPI_MAC_US_MAP_DESC_SIZE_EMB           64

#define PAL_CPPI_MAC_US_PRE_PROC_MONOLITIC_BASE     (IO_ADDRESS(0xF3FA0000))
#define PAL_CPPI_MAC_US_PRE_PROC_MONOLITIC_COUNT    64
#define PAL_CPPI_MAC_US_TX_MONOLITIC_BASE           (IO_ADDRESS(0xF2863000))
#define PAL_CPPI_MAC_US_TX_MONOLITIC_COUNT          56
#define PAL_CPPI_MAC_US_DLS_MONOLITIC_COUNT         8
#define PAL_CPPI_MAC_US_TX_EMB_BASE                 (IO_ADDRESS(0xF2740000))
#define PAL_CPPI_MAC_US_TX_EMB_COUNT                (3*1024)
#define PAL_CPPI_MAC_US_MAP_EMB_BASE                (IO_ADDRESS(0xF275A800))
#define PAL_CPPI_MAC_US_MAP_EMB_COUNT               (32)

/**********************************************************************************************************************

 #######   ######  ##     ##  #######  ##    ##
##     ## ##    ## ###   ### ##     ## ###   ##
##     ## ##       #### #### ##     ## ####  ##
##     ##  ######  ## ### ## ##     ## ## ## ##
##  ## ##       ## ##     ## ##     ## ##  ####
##    ##  ##    ## ##     ## ##     ## ##   ###
 ##### ##  ######  ##     ##  #######  ##    ##

**********************************************************************************************************************/

typedef enum PAL_CPPI_PP_QSMON_MGRs
{
    PAL_CPPI_PP_QSMON_MGR0,
    PAL_CPPI_PP_QSMON_MGR1,
    PAL_CPPI_PP_QSMON_MGR2,
    PAL_CPPI_PP_QSMON_MGR3,
    PAL_CPPI_PP_QSMON_MGR4,
    PAL_CPPI_PP_BSMON_MGR,
    PAL_CPPI_PP_NUM_QSMON_MGRs
}PAL_CPPI_PP_QSMON_MGRs_e;

#define PAL_CPPI_QSMON_MAX_THREADS                  32
#define PAL_CPPI_QSMON_THREADS_PER_DMA              6
#define PAL_CPPI_QSMON_SHARED_QUEUES_THRESHOLD      (9)  /* 3 for DSGs, 5 for GMIIs, 1 for MoCA */
#define PAL_CPPI_QSMON_DOCSIS_ONLY_THRESHOLD        (3)  /* 3 for DSGs */
#define PAL_CPPI_QSMON_PRIVATE_THRESHOLD            (1)
#define PAL_CPPI_QSMON_DIRECTION_UP                 0
#define PAL_CPPI_QSMON_DIRECTION_DOWN               1

#define PAL_CPPI_QSMON_0_CTRL_REGS_BASE             (IO_ADDRESS(0xF9188300))
#define PAL_CPPI_QSMON_0_CONFIG_REGS_BASE           (IO_ADDRESS(0xF9188400))
#define PAL_CPPI_QSMON_0_STATUS_REGS_BASE           (IO_ADDRESS(0xF9188500))

#define PAL_CPPI_QSMON_1_CTRL_REGS_BASE             (IO_ADDRESS(0xF9350000))
#define PAL_CPPI_QSMON_1_CONFIG_REGS_BASE           (IO_ADDRESS(0xF9350100))
#define PAL_CPPI_QSMON_1_STATUS_REGS_BASE           (IO_ADDRESS(0xF9350200))

#define PAL_CPPI_QSMON_2_CTRL_REGS_BASE             (IO_ADDRESS(0xF93D0000))
#define PAL_CPPI_QSMON_2_CONFIG_REGS_BASE           (IO_ADDRESS(0xF93D0100))
#define PAL_CPPI_QSMON_2_STATUS_REGS_BASE           (IO_ADDRESS(0xF93D0200))

#define PAL_CPPI_QSMON_3_CTRL_REGS_BASE             (IO_ADDRESS(0xF93D0400))
#define PAL_CPPI_QSMON_3_CONFIG_REGS_BASE           (IO_ADDRESS(0xF93D0500))
#define PAL_CPPI_QSMON_3_STATUS_REGS_BASE           (IO_ADDRESS(0xF93D0600))

typedef enum PAL_CPPI_PP_QSMON_3_THREADs
{
    PAL_CPPI_PP_QSMON_3_THREAD_0_RGMII1_CH_0,
    PAL_CPPI_PP_QSMON_3_THREAD_1_RGMII1_CH_1,
    PAL_CPPI_PP_QSMON_3_THREAD_2_RGMII1_CH_2,
    PAL_CPPI_PP_QSMON_3_THREAD_3_RGMII1_CH_4,
    PAL_CPPI_PP_QSMON_3_THREAD_4_RGMII1_CH_5,
    PAL_CPPI_PP_QSMON_3_THREAD_5_RGMII1_CH_6,

    PAL_CPPI_PP_QSMON_3_THREAD_6_ATOM_CH_0,
    PAL_CPPI_PP_QSMON_3_THREAD_7_ATOM_CH_1,
    PAL_CPPI_PP_QSMON_3_THREAD_8_ATOM_CH_2,
    PAL_CPPI_PP_QSMON_3_THREAD_9_ATOM_CH_4,
    PAL_CPPI_PP_QSMON_3_THREAD_10_ATOM_CH_5,
    PAL_CPPI_PP_QSMON_3_THREAD_11_ATOM_CH_6,

    PAL_CPPI_PP_QSMON_3_THREAD_12_MoCA_CH_0,
    PAL_CPPI_PP_QSMON_3_THREAD_13_MoCA_CH_1,
    PAL_CPPI_PP_QSMON_3_THREAD_14_MoCA_CH_2,
    PAL_CPPI_PP_QSMON_3_THREAD_15_MoCA_CH_4,
    PAL_CPPI_PP_QSMON_3_THREAD_16_MoCA_CH_5,
    PAL_CPPI_PP_QSMON_3_THREAD_17_MoCA_CH_6,

    PAL_CPPI_PP_QSMON_3_THREAD_18_RGMII0_CH_0,
    PAL_CPPI_PP_QSMON_3_THREAD_19_RGMII0_CH_1,
    PAL_CPPI_PP_QSMON_3_THREAD_20_RGMII0_CH_2,
    PAL_CPPI_PP_QSMON_3_THREAD_21_RGMII0_CH_4,
    PAL_CPPI_PP_QSMON_3_THREAD_22_RGMII0_CH_5,
    PAL_CPPI_PP_QSMON_3_THREAD_23_RGMII0_CH_6,

    PAL_CPPI_PP_QSMON_3_THREAD_24_SGMII1_CH_0,
    PAL_CPPI_PP_QSMON_3_THREAD_25_SGMII1_CH_1,
    PAL_CPPI_PP_QSMON_3_THREAD_26_SGMII1_CH_2,
    PAL_CPPI_PP_QSMON_3_THREAD_27_SGMII1_CH_4,
    PAL_CPPI_PP_QSMON_3_THREAD_28_SGMII1_CH_5,
    PAL_CPPI_PP_QSMON_3_THREAD_29_SGMII1_CH_6,

    PAL_CPPI_PP_QSMON_3_THREAD_30_UNUSED,
    PAL_CPPI_PP_QSMON_3_THREAD_31_UNUSED,
}PAL_CPPI_PP_QSMON_3_THREADs_e;


#define PAL_CPPI_QSMON_4_CTRL_REGS_BASE             (IO_ADDRESS(0xF93D0700))
#define PAL_CPPI_QSMON_4_CONFIG_REGS_BASE           (IO_ADDRESS(0xF93D0800))
#define PAL_CPPI_QSMON_4_STATUS_REGS_BASE           (IO_ADDRESS(0xF93D0900))

typedef enum PAL_CPPI_PP_QSMON_4_THREADs
{
    PAL_CPPI_PP_QSMON_4_THREAD_0_SGMII0_CH_0,
    PAL_CPPI_PP_QSMON_4_THREAD_1_SGMII0_CH_1,
    PAL_CPPI_PP_QSMON_4_THREAD_2_SGMII0_CH_2,
    PAL_CPPI_PP_QSMON_4_THREAD_3_SGMII0_CH_4,
    PAL_CPPI_PP_QSMON_4_THREAD_4_SGMII0_CH_5,
    PAL_CPPI_PP_QSMON_4_THREAD_5_SGMII0_CH_6,

    PAL_CPPI_PP_QSMON_4_THREAD_6_SCQAM0_CH_0,
    PAL_CPPI_PP_QSMON_4_THREAD_7_SCQAM0_CH_1,
    PAL_CPPI_PP_QSMON_4_THREAD_8_SCQAM0_CH_2,
    PAL_CPPI_PP_QSMON_4_THREAD_9_SCQAM0_CH_4,
    PAL_CPPI_PP_QSMON_4_THREAD_10_SCQAM0_CH_5,
    PAL_CPPI_PP_QSMON_4_THREAD_11_SCQAM0_CH_6,

    PAL_CPPI_PP_QSMON_4_THREAD_12_OFDM1_CH_0,
    PAL_CPPI_PP_QSMON_4_THREAD_13_OFDM1_CH_1,
    PAL_CPPI_PP_QSMON_4_THREAD_14_OFDM1_CH_2,
    PAL_CPPI_PP_QSMON_4_THREAD_15_OFDM1_CH_4,
    PAL_CPPI_PP_QSMON_4_THREAD_16_OFDM1_CH_5,
    PAL_CPPI_PP_QSMON_4_THREAD_17_OFDM1_CH_6,

    PAL_CPPI_PP_QSMON_4_THREAD_18_OFDM0_CH_0,
    PAL_CPPI_PP_QSMON_4_THREAD_19_OFDM0_CH_1,
    PAL_CPPI_PP_QSMON_4_THREAD_20_OFDM0_CH_2,
    PAL_CPPI_PP_QSMON_4_THREAD_21_OFDM0_CH_4,
    PAL_CPPI_PP_QSMON_4_THREAD_22_OFDM0_CH_5,
    PAL_CPPI_PP_QSMON_4_THREAD_23_OFDM0_CH_6,

    PAL_CPPI_PP_QSMON_4_THREAD_24_UNUSED,
    PAL_CPPI_PP_QSMON_4_THREAD_25_UNUSED,
    PAL_CPPI_PP_QSMON_4_THREAD_26_UNUSED,
    PAL_CPPI_PP_QSMON_4_THREAD_27_UNUSED,
    PAL_CPPI_PP_QSMON_4_THREAD_28_UNUSED,
    PAL_CPPI_PP_QSMON_4_THREAD_29_UNUSED,
    PAL_CPPI_PP_QSMON_4_THREAD_30_UNUSED,
    PAL_CPPI_PP_QSMON_4_THREAD_31_UNUSED,
}PAL_CPPI_PP_QSMON_4_THREADs_e;


#define PAL_CPPI_BSMON_CTRL_REGS_BASE               (IO_ADDRESS(0xF9188000))
#define PAL_CPPI_BSMON_CONFIG_REGS_BASE             (IO_ADDRESS(0xF9188100))
#define PAL_CPPI_BSMON_STATUS_REGS_BASE             (IO_ADDRESS(0xF9188200))


/**********************************************************************************************************************

########  ########  ######   ######     ########  ########  ######   ####  #######  ##    ##  ######
##     ## ##       ##    ## ##    ##    ##     ## ##       ##    ##   ##  ##     ## ###   ## ##    ##
##     ## ##       ##       ##          ##     ## ##       ##         ##  ##     ## ####  ## ##
##     ## ######    ######  ##          ########  ######   ##   ####  ##  ##     ## ## ## ##  ######
##     ## ##             ## ##          ##   ##   ##       ##    ##   ##  ##     ## ##  ####       ##
##     ## ##       ##    ## ##    ##    ##    ##  ##       ##    ##   ##  ##     ## ##   ### ##    ##
########  ########  ######   ######     ##     ## ########  ######   ####  #######  ##    ##  ######

**********************************************************************************************************************/

/*
+-+-+-+-+-+ +-+-+-+-+ +-+-+-+-+-+-+
|L|o|c|a|l| |D|e|s|c| |R|e|g|i|o|n|
+-+-+-+-+-+ +-+-+-+-+ +-+-+-+-+-+-+
*/

#define PAL_CPPI_PP_INTERNAL_EMB_LOCAL_DESC_REGION      0

#define PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_BASE                         (IO_ADDRESS(0xF3500000))
#define PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_COUNT                   64
#define PAL_CPPI_PP_QMGR_LOCAL_PREFETCH_FD_DESC_SIZE                    512


/*
+-+-+-+-+-+-+ +-+-+-+-+ +-+-+-+-+-+-+
|G|l|o|b|a|l| |D|e|s|c| |R|e|g|i|o|n|
+-+-+-+-+-+-+ +-+-+-+-+ +-+-+-+-+-+-+
*/

typedef enum PAL_CPPI_PP_DESC_REGIONs
{
    PAL_CPPI_PP_VOICE_GLOBAL_DESC_REGION,                               // 0
    PAL_CPPI_PP_RX_SHORT_NON_DOCSIS_INFRA_GLOBAL_DESC_REGION,           // 1
    PAL_CPPI_PP_RX_LONG_RX_XL_DOCSIS_GLOBAL_DESC_REGION,                // 2
    PAL_CPPI_PP_DESC_REGION3,                                           // 3
    PAL_CPPI_PP_DESC_REGION4,                                           // 4
    PAL_CPPI_PP_DESC_REGION5,                                           // 5
    PAL_CPPI_PP_DESC_REGION6,                                           // 6
    PAL_CPPI_PP_DESC_REGION7,                                           // 7
    PAL_CPPI_PP_DESC_REGION8,                                           // 8
    PAL_CPPI_PP_DESC_REGION9,                                           // 9
    PAL_CPPI_PP_DESC_REGION10,                                          // 10
    PAL_CPPI_PP_TEARDOWN_GLOBAL_DESC_REGION,                            // 11

    // On-Chip regions...
    PAL_CPPI_PP_INTERNAL_EMB_GLOBAL_DESC_REGION,                        // 12 
    PAL_CPPI_PP_IPSEC_MONOLITHIC_GLOBAL_DESC_REGION,                    // 13 
    PAL_CPPI_PP_US_FW_MONO_DESC_REGION,                                 // 14 
    PAL_CPPI_PP_PACKET_RAM_GLOBAL_DESC_REGION,                          // 15

    PAL_CPPI41_MAX_DESC_REGIONS
}PAL_CPPI_PP_DESC_REGIONs_e;



/* Descriptors information - count, size and base address if internal */
#define PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE                                           64
#define PAL_CPPI_PP_START_OF_PACKET_OFFSET                                                  128
#define PAL_CPPI_PP_START_HEADROOM_OFFSET                                                   (PAL_CPPI_PP_START_OF_PACKET_OFFSET - AVALNCHE_PP_WRAP_HEADER_MAX_LEN)

#define PAL_CPPI_PP_PACKET_RAM_SHARED_FD_BASE                                               (IO_ADDRESS(0xF9060000))
#ifdef CONFIG_MACH_PUMA7_FPGA
#define PAL_CPPI_PP_PACKET_RAM_SHARED_FD_DESC_COUNT                                         96
#else
#define PAL_CPPI_PP_PACKET_RAM_SHARED_FD_DESC_COUNT                                         1088
#endif

#ifdef CONFIG_MACH_PUMA7_FPGA
#define PAL_CPPI_PP_RX_LOW_SHORT_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                        0
#define PAL_CPPI_PP_RX_LOW_LONG_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                         0
#define PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_FULL                          64
#define PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT_FULL                           64
#else
#define PAL_CPPI_PP_RX_LOW_SHORT_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                        0
#define PAL_CPPI_PP_RX_LOW_LONG_SHARED_PACKET_RAM_EMB_FD_DESC_COUNT                         0
#define PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_REDUCED                       (6 * 1024)
#define PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT_REDUCED                        (6 * 1024)
#define PAL_CPPI_PP_RX_LOW_SHORT_SHARED_DDR_EMB_FD_DESC_COUNT_FULL                          (16 * 1024)
#define PAL_CPPI_PP_RX_LOW_LONG_SHARED_DDR_EMB_FD_DESC_COUNT_FULL                           (12 * 1024)
#endif
#define PAL_CPPI_PP_RX_LOW_XLONG_SHARED_DDR_EMB_FD_DESC_COUNT                               64

#ifdef CONFIG_MACH_PUMA7_FPGA
#define PAL_CPPI_PP_SHARED_LOW_INFRA_HOST_FD_DESC_COUNT                                     64
#else
#define PAL_CPPI_PP_SHARED_LOW_INFRA_HOST_FD_DESC_COUNT                                     512
#endif

#define PAL_CPPI_PP_DOCSIS_LOW_INFRA_HOST_FD_DESC_COUNT                                     512
#define PAL_CPPI_PP_DOCSIS_HI_INFRA_HOST_FD_DESC_COUNT                                      128
#define PAL_CPPI_PP_DOCSIS_RX_MGMT_HOST_FD_DESC_COUNT                                       64
#define PAL_CPPI_PP_DOCSIS_TX_MGMT_HOST_FD_DESC_COUNT                                       64

#ifdef CONFIG_MACH_PUMA7_FPGA
#define PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                0
#define PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT  0
#define PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 0
#define PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT   0
#define PAL_CPPI_PP_DOCSIS_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            64
#else
#define PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                0
#define PAL_CPPI_PP_DOCSIS_RX_LOW_SHORT_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT  0
#define PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 0
#define PAL_CPPI_PP_DOCSIS_RX_LOW_LONG_PRIVATE_DESC_PACKET_RAM_BUFF_DDR_EMB_FD_DESC_COUNT   0
#define PAL_CPPI_PP_DOCSIS_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            256
#endif

#define PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_BASE                                                (IO_ADDRESS(0xF3E40900))
#define PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_DESC_COUNT                                          16
#define PAL_CPPI_PP_IPSEC_MONOLITHIC_FD_DESC_SIZE                                           2048
#define PAL_CPPI_PP_IPSEC_HOST_FD_DESC_COUNT                                                2048
#define PAL_CPPI_PP_IPSEC_HOST_FD_DESC_SIZE                                                 64

#define PAL_CPPI_PP_US_FW_MONO_FD_BASE                                                      (IO_ADDRESS(0xF3FA0000))
#define PAL_CPPI_PP_US_FW_MONO_FD_DESC_COUNT                                                64
#define PAL_CPPI_PP_US_FW_MONO_FD_DESC_SIZE                                                 128

#define PAL_CPPI_PP_ATOM_HI_INFRA_HOST_FD_DESC_COUNT                                        32
#define PAL_CPPI_PP_RGMII0_HI_INFRA_HOST_FD_DESC_COUNT                                      32
#define PAL_CPPI_PP_RGMII1_HI_INFRA_HOST_FD_DESC_COUNT                                      32
#define PAL_CPPI_PP_SGMII0_HI_INFRA_HOST_FD_DESC_COUNT                                      32
#define PAL_CPPI_PP_SGMII1_HI_INFRA_HOST_FD_DESC_COUNT                                      32
#ifdef CONFIG_WIFI_PROXY
#define PAL_CPPI_PP_WIFI_INFRA_HOST_RX_FD_DESC_COUNT                                        512
#endif

#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL             0
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                    128
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL              0
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                     128
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED          0
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED                 0
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED           0
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED                  0
#else
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL             4
#define PAL_CPPI_PP_MoCA_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                    2
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL              4
#define PAL_CPPI_PP_MoCA_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                     2
#endif

#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                  64
#define PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                         64
#define PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                   8
#define PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                          128
#define PAL_CPPI_PP_ATOM_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                              64
#else
#define PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                  4
#define PAL_CPPI_PP_ATOM_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                         64
#define PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                   4
#define PAL_CPPI_PP_ATOM_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                          128
#define PAL_CPPI_PP_ATOM_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                              64
#endif

#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL           64
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                  64
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL            8
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                   128
#define PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                       64
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED        0
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED               0
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED         0
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED                0
#define PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED                    0
#else
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL           12
#define PAL_CPPI_PP_RGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                  64
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL            6
#define PAL_CPPI_PP_RGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                   128
#define PAL_CPPI_PP_RGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                       64
#endif

#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL           32
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                  64
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL            6
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                   128
#define PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                       64
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED        0
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED               0
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_REDUCED         0
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED                0
#define PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_REDUCED                    0
#else
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL           12
#define PAL_CPPI_PP_RGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                  64
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT_FULL            6
#define PAL_CPPI_PP_RGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                   128
#define PAL_CPPI_PP_RGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT_FULL                       64
#endif

#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                32
#define PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                       64
#define PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 6
#define PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        128
#define PAL_CPPI_PP_SGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            64
#else
#define PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                28
#define PAL_CPPI_PP_SGMII0_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                       64
#define PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 6
#define PAL_CPPI_PP_SGMII0_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        128
#define PAL_CPPI_PP_SGMII0_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            64
#endif

#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                256
#define PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                       64
#define PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 28
#define PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        128
#define PAL_CPPI_PP_SGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            64
#else
#define PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                4
#define PAL_CPPI_PP_SGMII1_RX_LOW_SHORT_PRIVATE_DDR_EMB_FD_DESC_COUNT                       64
#define PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_PACKET_RAM_EMB_FD_DESC_COUNT                 6
#define PAL_CPPI_PP_SGMII1_RX_LOW_LONG_PRIVATE_DDR_EMB_FD_DESC_COUNT                        128
#define PAL_CPPI_PP_SGMII1_RX_HIGH_PRIVATE_DDR_EMB_FD_DESC_COUNT                            64
#endif

#define PAL_CPPI_PP_WIFI_FW_TX_FD_BASE                                                      (IO_ADDRESS(0xF3508000))
#define PAL_CPPI_PP_WIFI_FW_RX_EMB_FD_DESC_COUNT                                            (16384)  /* must be aligned to number of buffers allocated by app-cpu for wifi rx pool */
#define PAL_CPPI_PP_WIFI_FW_RX_REASSEMBLY_EMB_FD_DESC_COUNT                                 (32)

#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_HOST2PP_LOW_HOST_FD_DESC_COUNT                                          1024
#define PAL_CPPI_PP_HOST2PP_HI_HOST_FD_DESC_COUNT                                           64
#define PAL_CPPI_PP_HOST2PP_LOW_INFRA_EMB_FD_DESC_COUNT                                     2048
#define PAL_CPPI_PP_HOST2PP_HI_INFRA_EMB_FD_DESC_COUNT                                      64
#else
#define PAL_CPPI_PP_HOST2PP_LOW_HOST_FD_DESC_COUNT                                          64
#define PAL_CPPI_PP_HOST2PP_HI_HOST_FD_DESC_COUNT                                           64
#define PAL_CPPI_PP_HOST2PP_LOW_INFRA_EMB_FD_DESC_COUNT                                     64
#define PAL_CPPI_PP_HOST2PP_HI_INFRA_EMB_FD_DESC_COUNT                                      64
#endif


#ifdef CONFIG_MACH_PUMA7_BOARD
#define PAL_CPPI_PP_NP2APP_HOST_FD_DESC_COUNT                                               512
#define PAL_CPPI_PP_NP2APP_INFRA_PPINFO_HOST_FD_DESC_COUNT                                  512
#define PAL_CPPI_PP_NP2APP_INFRA_DATA_HOST_FD_DESC_COUNT                                    512
#define PAL_CPPI_PP_APP2NP_HOST_FD_DESC_COUNT                                               512
#define PAL_CPPI_PP_APP2NP_INFRA_PPINFO_HOST_FD_DESC_COUNT                                  512
#define PAL_CPPI_PP_APP2NP_INFRA_DATA_HOST_FD_DESC_COUNT                                    512
#define PAL_CPPI_PP_APP2NP_INFRA_HIGH_PPINFO_HOST_FD_DESC_COUNT                             128
#define PAL_CPPI_PP_APP2NP_INFRA_HIGH_DATA_HOST_FD_DESC_COUNT                               128
#define PAL_CPPI_PP_NP2APP_INFRA_HIGH_PPINFO_HOST_FD_DESC_COUNT                             128
#define PAL_CPPI_PP_NP2APP_INFRA_HIGH_DATA_HOST_FD_DESC_COUNT                               128
#else
#define PAL_CPPI_PP_NP2APP_HOST_FD_DESC_COUNT                                               64
#define PAL_CPPI_PP_NP2APP_INFRA_PPINFO_HOST_FD_DESC_COUNT                                  32
#define PAL_CPPI_PP_NP2APP_INFRA_DATA_HOST_FD_DESC_COUNT                                    32
#define PAL_CPPI_PP_APP2NP_HOST_FD_DESC_COUNT                                               64
#define PAL_CPPI_PP_APP2NP_INFRA_PPINFO_HOST_FD_DESC_COUNT                                  32
#define PAL_CPPI_PP_APP2NP_INFRA_DATA_HOST_FD_DESC_COUNT                                    32
#endif

#define PAL_CPPI_PP_VOICE_DSP_RX_EMB_FD_DESC_COUNT                                          128
#define PAL_CPPI_PP_VOICE_INFRA_RX_EMB_FD_DESC_COUNT                                        128


#define PAL_CPPI_DSG_FD_EMB_DESC_REGION             0


/**********************************************************************************************************************

########  ##     ## ######## ######## ######## ########     ##     ##    ###    ##    ##    ###     ######   ######## ########
##     ## ##     ## ##       ##       ##       ##     ##    ###   ###   ## ##   ###   ##   ## ##   ##    ##  ##       ##     ##
##     ## ##     ## ##       ##       ##       ##     ##    #### ####  ##   ##  ####  ##  ##   ##  ##        ##       ##     ##
########  ##     ## ######   ######   ######   ########     ## ### ## ##     ## ## ## ## ##     ## ##   #### ######   ########
##     ## ##     ## ##       ##       ##       ##   ##      ##     ## ######### ##  #### ######### ##    ##  ##       ##   ##
##     ## ##     ## ##       ##       ##       ##    ##     ##     ## ##     ## ##   ### ##     ## ##    ##  ##       ##    ##
########   #######  ##       ##       ######## ##     ##    ##     ## ##     ## ##    ## ##     ##  ######   ######## ##     ##

**********************************************************************************************************************/

extern Int32 avalanche_cppi_get_buffer_count(Uint32 bufPoolID, Uint32* buff_csount);

/*
+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+
|P|P| |B|u|f|f|e|r| |M|a|n|a|g|e|r|
+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+
*/

#define PAL_CPPI_PP_BUF_MGR                     0
#define PAL_CPPI_PP_BUF_MGR_BASE                (IO_ADDRESS(0xF9180000))

#undef PAL_CPPI_BMGR_P_ADD
#define PAL_CPPI_BMGR_P_ADD(pNum)       pNum,

typedef enum PAL_CPPI_PP_BUFFER_POOLs
{
    PAL_CPPI_PP_BMGR_POOL_LIST
    PAL_CPPI41_BMGR_MAX_POOLS
}PAL_CPPI_PP_BUFFER_POOLs_e;


#define PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_SIZE                   1024
#define PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_REF_CNT                0

#define PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_SIZE                   (2 * 1024)
#define PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_REF_CNT                0

#define PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_COUNT                  PAL_CPPI_PP_RX_LOW_XLONG_SHARED_DDR_EMB_FD_DESC_COUNT
#define PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_SIZE                   (4 * 1024)
#define PAL_CPPI_PP_SHARED_RX_LOW_4KB_BUFFER_REF_CNT                0

#define PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_SIZE                      (2 * 1024)
#define PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_REF_CNT                   0

#define PAL_CPPI_PP_BUFFER_POOL4_COUNT                              0
#define PAL_CPPI_PP_BUFFER_POOL4_SIZE                               0
#define PAL_CPPI_PP_WIFI_RX_POOL_REF_CNT                            0
#define PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_SIZE                        (2 * 1024)
#define PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_COUNT_FULL                  (16 * 1024)
#define PAL_CPPI_PP_WIFI_RX_POOL_BUFFER_COUNT_REDUCED               (8 * 1024)
#define PAL_CPPI_PP_BUFFER_POOL5_COUNT                              0
#define PAL_CPPI_PP_BUFFER_POOL5_SIZE                               0
#define PAL_CPPI_PP_WIFI_TX_POOL_REF_CNT                            0

#define PAL_CPPI_PP_BUFFER_POOL6_COUNT                              0
#define PAL_CPPI_PP_BUFFER_POOL6_SIZE                               0
#define PAL_CPPI_PP_BUFFER_POOL6_REF_CNT                            0

#define PAL_CPPI_PP_BUFFER_POOL7_COUNT                              0
#define PAL_CPPI_PP_BUFFER_POOL7_SIZE                               0
#define PAL_CPPI_PP_BUFFER_POOL7_REF_CNT                            0

#define PAL_CPPI_PP_BUFFER_POOL8_COUNT                              0
#define PAL_CPPI_PP_BUFFER_POOL8_SIZE                               0
#define PAL_CPPI_PP_BUFFER_POOL8_REF_CNT                            0

#define PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_SIZE                     512
#define PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_REF_CNT                  0
#define PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_BASE                     0xF9000000

#define PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_SIZE                      (2 * 1024)
#define PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_REF_CNT                   0
#define PAL_CPPI_PP_PACKET_RAM_2KB_BUFFER_BASE                      0xF9040000

#define PAL_CPPI_PP_VOICE_DSP_BUFFER_COUNT                          512
#define PAL_CPPI_PP_VOICE_DSP_BUFFER_SIZE                           1024
#define PAL_CPPI_PP_VOICE_DSP_BUFFER_REF_CNT                        0


/*
+-+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
|D|S|G| |B|u|f|f|e|r| |M|a|n|a|g|e|r|s|
+-+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
*/

typedef enum PAL_CPPI_DSG_BUF_MNGRS
{
    PAL_CPPI_OFDM0_BUF_MGR,                          /* OFDM0  */
    PAL_CPPI_OFDM1_BUF_MGR,                          /* OFDM1  */
    PAL_CPPI_SC_QAM0_BUF_MGR,                        /* SC-QAM */
}
PAL_CPPI_DSG_BUF_MNGRS_e;

#define PAL_CPPI_OFDM0_BUF_MGR_BASE                                 (IO_ADDRESS(0xF2168000))
#define PAL_CPPI_OFDM1_BUF_MGR_BASE                                 (IO_ADDRESS(0xF21E8000))
#define PAL_CPPI_SC_QAM0_BUF_MGR_BASE                               (IO_ADDRESS(0xF2068000))

/* OFDM0 DSG Buffer Pool Info */
#define PAL_CPPI_OFDM0_BUFFER_POOL00                                0
#define PAL_CPPI_OFDM0_BUFFER_POOL00_BUF_COUNT                      128
#define PAL_CPPI_OFDM0_BUFFER_RAM_INTERNAL_ADDRESS                  0x20000

/* OFDM1 DSG Buffer Pool Info */
#define PAL_CPPI_OFDM1_BUFFER_POOL00                                0
#define PAL_CPPI_OFDM1_BUFFER_POOL00_BUF_COUNT                      128
#define PAL_CPPI_OFDM1_BUFFER_RAM_INTERNAL_ADDRESS                  0x20000

/* SC-QAMs DSG Buffer Pool Info */
#define PAL_CPPI_SC_QAM0_BUFFER_POOL00                              0
#define PAL_CPPI_SC_QAM0_BUFFER_POOL00_BUF_COUNT                    256
#define PAL_CPPI_SC_QAM0_BUFFER_RAM_INTERNAL_ADDRESS                0x20000

/* DSGs Common Buffer Pool Info */
#define PAL_CPPI_DS_DSG_BUFFER_POOL00_BUF_SIZE                      512
#define PAL_CPPI_DS_DSG_BUFFER_POOL00_REF_CNT                       0

/*
+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+
|U|S| |B|u|f|f|e|r| |M|a|n|a|g|e|r|
+-+-+ +-+-+-+-+-+-+ +-+-+-+-+-+-+-+
*/

#define PAL_CPPI_MAC_US_BUF_MGR                 3
#define PAL_CPPI41_NUM_BUF_MGR                  (PAL_CPPI_MAC_US_BUF_MGR + 1)           /* DSG domain has the maximum number of buffer managers */

#define PAL_CPPI_MAC_US_BUF_MGR_BASE            (IO_ADDRESS(0xF2540000))

typedef enum PAL_CPPI_MAC_US_BUFFER_POOLs
{
    PAL_CPPI_MAC_US_BUFFER_POOL0,

    PAL_CPPI_MAC_US_TX_EMB_2K_BUFFER_POOL = PAL_CPPI_MAC_US_BUFFER_POOL0,
    PAL_CPPI_MAC_US_BUFFER_POOL1,

}
PAL_CPPI_MAC_US_BUFFER_POOLs_e;


#ifdef CONFIG_MACH_PUMA7_FPGA_US
#define PAL_CPPI_MAC_US_TX_EMB_2K_BUFFER_COUNT                  256
#else
#define PAL_CPPI_MAC_US_TX_EMB_2K_BUFFER_COUNT                  (3 * 1024)
#endif
#define PAL_CPPI_MAC_US_TX_EMB_2K_BUFFER_SIZE                   2048
#define PAL_CPPI_MAC_US_TX_EMB_2K_BUFFER_REF_CNT                1



/**********************************************************************************************************************

########  ##     ##    ###        ######  ##     ##    ###    ##    ## ##    ## ######## ##        ######
##     ## ###   ###   ## ##      ##    ## ##     ##   ## ##   ###   ## ###   ## ##       ##       ##    ##
##     ## #### ####  ##   ##     ##       ##     ##  ##   ##  ####  ## ####  ## ##       ##       ##
##     ## ## ### ## ##     ##    ##       ######### ##     ## ## ## ## ## ## ## ######   ##        ######
##     ## ##     ## #########    ##       ##     ## ######### ##  #### ##  #### ##       ##             ##
##     ## ##     ## ##     ##    ##    ## ##     ## ##     ## ##   ### ##   ### ##       ##       ##    ##
########  ##     ## ##     ##     ######  ##     ## ##     ## ##    ## ##    ## ######## ########  ######

**********************************************************************************************************************/

/*
+-+-+ +-+-+-+-+
|P|P| |D|M|A|s|
+-+-+ +-+-+-+-+
*/

typedef enum PAL_CPPI_PP_DMA_BLOCKS
{
    PAL_CPPI_PP_DMA00_OFDM0_RX,
    PAL_CPPI_PP_DMA01_OFDM1_RX,
    PAL_CPPI_PP_DMA02_SC_QAM_RX,
    PAL_CPPI_PP_DMA03_SGMII0_RX,
    PAL_CPPI_PP_DMA04_SGMII1_RX,
    PAL_CPPI_PP_DMA05_RGMII0_RX,
    PAL_CPPI_PP_DMA06_MoCA_RX_TX,
    PAL_CPPI_PP_DMA07_ATOM_RX_TX,
    PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX,
    PAL_CPPI_PP_DMA09_INFRA_RX_TX,
    PAL_CPPI_PP_DMA10_INFRA_RX_TX,
    PAL_CPPI_PP_DMA11_INFRA_RX_TX,
    PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX,
    PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX,
    PAL_CPPI_PP_DMA14_SGMII0_TX,
    PAL_CPPI_PP_DMA15_SGMII1_TX,
    PAL_CPPI_PP_DMA16_RGMII0_TX,
    PAL_CPPI_PP_DMA17_UNUSED,
    PAL_CPPI_PP_DMA18_UNUSED,
    PAL_CPPI_PP_DMA19_UNUSED,
    PAL_CPPI_PP_DMA20_UNUSED,
    PAL_CPPI_PP_DMA21_US_COP_RX,
    PAL_CPPI_PP_DMA22_US_COP_TX,
    PAL_CPPI_PP_DMA23_RGMII1_RX_TX,

    PAL_CPPI41_NUM_DMA_BLOCK

}PAL_CPPI_PP_DMA_BLOCKS_e;


// PAL_CPPI_PP_DMA00_OFDM0_RX
#define PAL_CPPI_PP_DMA00_OFDM0_RX_CH_CFG_BASE          (IO_ADDRESS(0xF3900000))
#define PAL_CPPI_PP_DMA00_OFDM0_RX_GLOBAL_CTRL_BASE     (IO_ADDRESS(0xF3901000))
#define PAL_CPPI_PP_DMA00_OFDM0_RX_SCHEDULER_BASE       (IO_ADDRESS(0xF3901020))
#define PAL_CPPI_PP_DMA00_OFDM0_RX_RAL_CFG_BASE         (IO_ADDRESS(0xF3940018))
typedef enum PAL_CPPI_PP_DMA00_OFDM0_RX_CHANNELS
{
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_512B_DESC_PACKET_RAM_BUFF_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_DESC_PACKET_RAM_BUFF_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_MGMT_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_PLC_EM,       /* PLC Energy Management */
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_FCC_DDR,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_13,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_14,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CH_15,
    PAL_CPPI_PP_DMA00_OFDM0_RX_CHANNELS_NUM
} PAL_CPPI_PP_DMA00_OFDM0_RX_CHANNELS_e;

// PAL_CPPI_PP_DMA01_OFDM1_RX
#define PAL_CPPI_PP_DMA01_OFDM1_RX_CH_CFG_BASE          (IO_ADDRESS(0xF3902000))
#define PAL_CPPI_PP_DMA01_OFDM1_RX_GLOBAL_CTRL_BASE     (IO_ADDRESS(0xF3903000))
#define PAL_CPPI_PP_DMA01_OFDM1_RX_SCHEDULER_BASE       (IO_ADDRESS(0xF3903020))
#define PAL_CPPI_PP_DMA01_OFDM1_RX_RAL_CFG_BASE         (IO_ADDRESS(0xF394001C))
typedef enum PAL_CPPI_PP_DMA01_OFDM1_RX_CHANNELS
{
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_512B_DESC_PACKET_RAM_BUFF_DDR,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_DESC_PACKET_RAM_BUFF_DDR,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_MGMT_DDR,
    PAL_CPPI_PP_DMA00_OFDM1_RX_CH_PLC_EM,       /* PLC Energy Management */
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_FCC_DDR,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_13,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_14,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CH_15,
    PAL_CPPI_PP_DMA01_OFDM1_RX_CHANNELS_NUM
} PAL_CPPI_PP_DMA01_OFDM1_RX_CHANNELS_e;

// PAL_CPPI_PP_DMA02_SC_QAM_RX
#define PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_CFG_BASE         (IO_ADDRESS(0xF3904000))
#define PAL_CPPI_PP_DMA02_SC_QAM_RX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF3905000))
#define PAL_CPPI_PP_DMA02_SC_QAM_RX_SCHEDULER_BASE      (IO_ADDRESS(0xF3905020))
#define PAL_CPPI_PP_DMA02_SC_QAM_RX_RAL_CFG_BASE        (IO_ADDRESS(0xF3940020))
typedef enum PAL_CPPI_PP_DMA02_SC_QAM_RX_CHANNELS
{
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_512B_DESC_PACKET_RAM_BUFF_DDR,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_DESC_PACKET_RAM_BUFF_DDR,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_MGMT_DDR,
    PAL_CPPI_PP_DMA00_SC_QAM_RX_CH_PLC_EM,      /* to be align with OFDM DMAs */
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_FCC_DDR,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_13,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_14,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CH_15,
    PAL_CPPI_PP_DMA02_SC_QAM_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA02_SC_QAM_RX_CHANNELS_e;

// PAL_CPPI_PP_DMA03_SGMII0_RX
#define PAL_CPPI_PP_DMA03_SGMII0_RX_CH_CFG_BASE         (IO_ADDRESS(0xF3906000))
#define PAL_CPPI_PP_DMA03_SGMII0_RX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF3907000))
#define PAL_CPPI_PP_DMA03_SGMII0_RX_SCHEDULER_BASE      (IO_ADDRESS(0xF3907020))
#define PAL_CPPI_PP_DMA03_SGMII0_RX_RAL_CFG_BASE        (IO_ADDRESS(0xF3940024))
typedef enum PAL_CPPI_PP_DMA03_SGMII0_RX_CHANNELS
{
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_1KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA03_SGMII0_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA03_SGMII0_RX_CHANNELS_e;

// PAL_CPPI_PP_DMA04_SGMII1_RX
#define PAL_CPPI_PP_DMA04_SGMII1_RX_CH_CFG_BASE         (IO_ADDRESS(0xF3908000))
#define PAL_CPPI_PP_DMA04_SGMII1_RX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF3909000))
#define PAL_CPPI_PP_DMA04_SGMII1_RX_SCHEDULER_BASE      (IO_ADDRESS(0xF3909020))
#define PAL_CPPI_PP_DMA04_SGMII1_RX_RAL_CFG_BASE        (IO_ADDRESS(0xF3940028))
typedef enum PAL_CPPI_PP_DMA04_SGMII1_RX_CHANNELS
{
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_1KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA04_SGMII1_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA04_SGMII1_RX_CHANNELS_e;

// PAL_CPPI_PP_DMA05_RGMII0_RX
#define PAL_CPPI_PP_DMA05_RGMII0_RX_CH_CFG_BASE         (IO_ADDRESS(0xF390A000))
#define PAL_CPPI_PP_DMA05_RGMII0_RX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF390B000))
#define PAL_CPPI_PP_DMA05_RGMII0_RX_SCHEDULER_BASE      (IO_ADDRESS(0xF390B020))
#define PAL_CPPI_PP_DMA05_RGMII0_RX_RAL_CFG_BASE        (IO_ADDRESS(0xF394002C))
typedef enum PAL_CPPI_PP_DMA05_RGMII0_RX_CHANNELS
{
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_1KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA05_RGMII0_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA05_RGMII0_RX_CHANNELS_e;

// PAL_CPPI_PP_DMA06_MoCA_RX_TX
#define PAL_CPPI_PP_DMA06_MoCA_RX_TX_CH_CFG_BASE        (IO_ADDRESS(0xF390C000))
#define PAL_CPPI_PP_DMA06_MoCA_RX_TX_GLOBAL_CTRL_BASE   (IO_ADDRESS(0xF390D000))
#define PAL_CPPI_PP_DMA06_MoCA_RX_TX_SCHEDULER_BASE     (IO_ADDRESS(0xF390D020))
#define PAL_CPPI_PP_DMA06_MoCA_RX_RAL_CFG_BASE          (IO_ADDRESS(0xF3940030))
#define PAL_CPPI_PP_DMA06_MoCA_RX_CMD1                  (IO_ADDRESS(0xf3940008))
#define PAL_CPPI_PP_DMA06_MoCA_RX_CFG0                  (IO_ADDRESS(0xf394000c))
#define PAL_CPPI_PP_DMA06_MoCA_RX_CFG1                  (IO_ADDRESS(0xf3940010))

// PAL_CPPI_PP_DMA06_MoCA_RX
typedef enum PAL_CPPI_PP_DMA06_MoCA_RX_CHANNELS
{
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_1KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA06_MoCA_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA06_MoCA_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA06_MoCA_RX_CHANNELS_e;
// PAL_CPPI_PP_DMA06_MoCA_TX
#define PAL_CPPI_PP_DMA06_MoCA_TX_CH            0
#define PAL_CPPI_PP_DMA06_MoCA_TX_CHANNELS_NUM  1

// PAL_CPPI_PP_DMA07_ATOM_RX_TX
#define PAL_CPPI_PP_DMA07_ATOM_RX_TX_CH_CFG_BASE        (IO_ADDRESS(0xF390E000))
#define PAL_CPPI_PP_DMA07_ATOM_RX_TX_GLOBAL_CTRL_BASE   (IO_ADDRESS(0xF390F000))
#define PAL_CPPI_PP_DMA07_ATOM_RX_TX_SCHEDULER_BASE     (IO_ADDRESS(0xF390F020))
#define PAL_CPPI_PP_DMA07_ATOM_RX_RAL_CFG_BASE          (IO_ADDRESS(0xF3940034))
// PAL_CPPI_PP_DMA07_ATOM_RX
typedef enum PAL_CPPI_PP_DMA07_ATOM_RX_CHANNELS
{
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_1KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA07_ATOM_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA07_ATOM_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA07_ATOM_RX_CHANNELS_e;
// PAL_CPPI_PP_DMA07_ATOM_TX
#define PAL_CPPI_PP_DMA07_ATOM_TX_CH            0
#define PAL_CPPI_PP_DMA07_ATOM_TX_CHANNELS_NUM  1

// PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX
#define PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX_CH_CFG_BASE        (IO_ADDRESS(0xF3910000))
#define PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX_GLOBAL_CTRL_BASE   (IO_ADDRESS(0xF3911000))
#define PAL_CPPI_PP_DMA08_RX_TX_INFRA_WIFI_PROXY_RX_TX_SCHEDULER_BASE     (IO_ADDRESS(0xF3911020))
// PAL_CPPI_PP_DMA08_MPEG_RX_INFRA_RX
typedef enum PAL_CPPI_PP_DMA08_RX_WIFI_PROXY_INFRA_RX_CHANNELS
{
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_0,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_1,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_2,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_3,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_4,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_5,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_6,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_7,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_8,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_9,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_10,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_11,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_12,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_13,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_14,
    PAL_CPPI_PP_DMA08_MP2TS_RX_CH_15,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE0_HOST_RX_INFRA_RX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE1_HOST_RX_INFRA_RX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE2_HOST_RX_INFRA_RX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE3_HOST_RX_INFRA_RX_CH,
    PAL_CPPI_PP_DMA08_UNUSED_RX_CH_20,
    PAL_CPPI_PP_DMA08_UNUSED_RX_CH_21,
    PAL_CPPI_PP_DMA08_UNUSED_RX_CH_22,
    PAL_CPPI_PP_DMA08_UNUSED_RX_CH_23,
    PAL_CPPI_PP_DMA08_RX_WIFI_PROXY_INFRA_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA08_RX_WIFI_PROXY_INFRA_RX_CHANNELS_e;
// PAL_CPPI_PP_DMA08_RX_WIFI_PROXY_INFRA_TX_CHANNELS
typedef enum PAL_CPPI_PP_DMA08_RX_WIFI_PROXY_INFRA_TX_CHANNELS
{
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_0,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_1,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_2,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_3,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_4,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_5,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_6,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_7,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_8,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_9,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_10,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_11,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_12,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_13,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_14,
    PAL_CPPI_PP_DMA08_UNUSED_TX_CH_15,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE0_HOST_RX_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE1_HOST_RX_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE2_HOST_RX_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE3_HOST_RX_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE0_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE1_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE2_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_WIFI_DEVICE3_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA08_RX_WIFI_PROXY_INFRA_TX_CHANNELS_NUM
}PAL_CPPI_PP_DMA08_RX_WIFI_PROXY_INFRA_TX_CHANNELS_e;


// PAL_CPPI_PP_DMA09_INFRA_RX_TX
#define PAL_CPPI_PP_DMA09_INFRA_RX_TX_CH_CFG_BASE           (IO_ADDRESS(0xF3912000))
#define PAL_CPPI_PP_DMA09_INFRA_RX_TX_GLOBAL_CTRL_BASE      (IO_ADDRESS(0xF3913000))
#define PAL_CPPI_PP_DMA09_INFRA_RX_TX_SCHEDULER_BASE        (IO_ADDRESS(0xF3913020))
// PAL_CPPI_PP_DMA09_INFRA_RX
typedef enum PAL_CPPI_PP_DMA09_INFRA_RX_CHANNELS
{
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_0,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_1,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_2,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_3,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_4,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_5,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_6,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_7,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_8,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_9,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_10,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_11,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_12,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_13,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_14,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_15,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_16,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_17,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_18,
    PAL_CPPI_PP_DMA09_INFRA_RX_CH_19,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE0_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE1_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE2_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE3_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA09_INFRA_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA09_INFRA_RX_CHANNELS_e;
// PAL_CPPI_PP_DMA09_INFRA_TX
typedef enum PAL_CPPI_PP_DMA09_INFRA_TX_CHANNELS
{
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_0,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_1,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_2,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_3,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_4,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_5,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_6,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_7,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_8,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_9,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_10,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_11,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_12,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_13,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_14,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_15,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_16,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_17,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_18,
    PAL_CPPI_PP_DMA09_INFRA_TX_CH_19,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE0_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE1_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE2_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA09_WIFI_DEVICE3_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA09_INFRA_TX_CHANNELS_NUM
}PAL_CPPI_PP_DMA09_INFRA_TX_CHANNELS_e;

// PAL_CPPI_PP_DMA10_INFRA_RX_TX
#define PAL_CPPI_PP_DMA10_INFRA_RX_TX_CH_CFG_BASE           (IO_ADDRESS(0xF3914000))
#define PAL_CPPI_PP_DMA10_INFRA_RX_TX_GLOBAL_CTRL_BASE      (IO_ADDRESS(0xF3915000))
#define PAL_CPPI_PP_DMA10_INFRA_RX_TX_SCHEDULER_BASE        (IO_ADDRESS(0xF3915020))
// PAL_CPPI_PP_DMA10_INFRA_RX
typedef enum PAL_CPPI_PP_DMA10_INFRA_RX_CHANNELS
{
    PAL_CPPI_PP_DMA10_DOCSIS_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_DOCSIS_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_MoCA_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_ATOM_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_ATOM_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_RGMII0_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_RGMII0_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_RGMII1_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_RGMII1_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_SGMII0_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_SGMII0_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_SGMII1_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_SGMII1_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_WiFi_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_WiFi_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_INFRA_RX_CH_15,
    PAL_CPPI_PP_DMA10_INFRA_RX_CH_16,
    PAL_CPPI_PP_DMA10_VOICE_DSP_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_NP2APP_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_APP2NP_INFRA_RX_CH,
    PAL_CPPI_PP_DMA10_APP2NP_INFRA_RX_CTRL_CH,
    PAL_CPPI_PP_DMA10_NP2APP_INFRA_RX_CTRL_CH,
    PAL_CPPI_PP_DMA10_INFRA_RX_CH_22,
    PAL_CPPI_PP_DMA10_INFRA_RX_CH_23,
    PAL_CPPI_PP_DMA10_INFRA_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA10_INFRA_RX_CHANNELS_e;
// PAL_CPPI_PP_DMA10_INFRA_TX
typedef enum PAL_CPPI_PP_DMA10_INFRA_TX_CHANNELS
{
    PAL_CPPI_PP_DMA10_DOCSIS_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_DOCSIS_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_MoCA_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_ATOM_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_ATOM_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_RGMII0_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_RGMII0_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_RGMII1_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_RGMII1_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_SGMII0_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_SGMII0_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_SGMII1_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_SGMII1_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_WiFi_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_WiFi_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_INFRA_TX_CH_15,
    PAL_CPPI_PP_DMA10_INFRA_TX_CH_16,
    PAL_CPPI_PP_DMA10_VOICE_DSP_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_NP2APP_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_APP2NP_INFRA_TX_CH,
    PAL_CPPI_PP_DMA10_APP2NP_INFRA_CTRL_TX_CH,
    PAL_CPPI_PP_DMA10_NP2APP_INFRA_CTRL_TX_CH,
    PAL_CPPI_PP_DMA10_INFRA_TX_CH_22,
    PAL_CPPI_PP_DMA10_INFRA_TX_CH_23,
    PAL_CPPI_PP_DMA10_INFRA_TX_CHANNELS_NUM
}PAL_CPPI_PP_DMA10_INFRA_TX_CHANNELS_e;


// PAL_CPPI_PP_DMA11_INFRA_RX_TX
#define PAL_CPPI_PP_DMA11_INFRA_RX_TX_CH_CFG_BASE           (IO_ADDRESS(0xF3916000))
#define PAL_CPPI_PP_DMA11_INFRA_RX_TX_GLOBAL_CTRL_BASE      (IO_ADDRESS(0xF3917000))
#define PAL_CPPI_PP_DMA11_INFRA_RX_TX_SCHEDULER_BASE        (IO_ADDRESS(0xF3917020))
// PAL_CPPI_PP_DMA11_INFRA_RX
typedef enum PAL_CPPI_PP_DMA11_INFRA_RX_CHANNELS
{
    PAL_CPPI_PP_DMA11_HOST2PP_LOW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA11_HOST2PP_HIGH_INFRA_RX_CH,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_2,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_3,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE0_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE1_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE2_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE3_TX_FW_INFRA_RX_CH,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_8,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_9,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_10,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_11,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_12,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_13,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_14,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_15,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_16,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_17,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_18,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_19,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_20,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_21,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_22,
    PAL_CPPI_PP_DMA11_INFRA_RX_CH_23,
    PAL_CPPI_PP_DMA11_INFRA_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA11_INFRA_RX_CHANNELS_e;
// PAL_CPPI_PP_DMA11_INFRA_TX
typedef enum PAL_CPPI_PP_DMA11_INFRA_TX_CHANNELS
{
    PAL_CPPI_PP_DMA11_HOST2PP_LOW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA11_HOST2PP_HIGH_INFRA_TX_CH,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_2,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_3,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE0_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE1_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE2_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA11_WIFI_DEVICE3_TX_FW_INFRA_TX_CH,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_8,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_9,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_10,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_11,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_12,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_13,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_14,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_15,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_16,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_17,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_18,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_19,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_20,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_21,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_22,
    PAL_CPPI_PP_DMA11_INFRA_TX_CH_23,
    PAL_CPPI_PP_DMA11_INFRA_TX_CHANNELS_NUM
}PAL_CPPI_PP_DMA11_INFRA_TX_CHANNELS_e;


// PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX
#define PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CH_CFG_BASE           (IO_ADDRESS(0xF3918000))
#define PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_GLOBAL_CTRL_BASE      (IO_ADDRESS(0xF3919000))
#define PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_SCHEDULER_BASE        (IO_ADDRESS(0xF3919020))
typedef enum PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS
{
    PAL_CPPI_PP_DMA12_OFDM0_TX_CH,
    PAL_CPPI_PP_DMA12_SC_QAM0_TX_CH,
    PAL_CPPI_PP_DMA12_SC_QAM1_TX_CH,
    PAL_CPPI_PP_DMA12_SC_QAM2_TX_CH,
    PAL_CPPI_PP_DMA12_SC_QAM3_TX_CH,
    PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS_NUM
}PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS_e;

// PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX
#define PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_CH_CFG_BASE           (IO_ADDRESS(0xF391A000))
#define PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_GLOBAL_CTRL_BASE      (IO_ADDRESS(0xF391B000))
#define PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_SCHEDULER_BASE        (IO_ADDRESS(0xF391B020))
typedef enum PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_CHANNELS
{
    PAL_CPPI_PP_DMA13_OFDM1_TX_CH,
    PAL_CPPI_PP_DMA13_SC_QAM4_TX_CH,
    PAL_CPPI_PP_DMA13_SC_QAM5_TX_CH,
    PAL_CPPI_PP_DMA13_SC_QAM6_TX_CH,
    PAL_CPPI_PP_DMA13_SC_QAM7_TX_CH,
    PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_CHANNELS_NUM
}PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_CHANNELS_e;

// PAL_CPPI_PP_DMA14_SGMII0_TX
#define PAL_CPPI_PP_DMA14_SGMII0_TX_CH_CFG_BASE         (IO_ADDRESS(0xF391C000))
#define PAL_CPPI_PP_DMA14_SGMII0_TX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF391D000))
#define PAL_CPPI_PP_DMA14_SGMII0_TX_SCHEDULER_BASE      (IO_ADDRESS(0xF391D020))
#define PAL_CPPI_PP_DMA14_SGMII0_TX_CH                  0
#define PAL_CPPI_PP_DMA14_SGMII0_TX_CHANNELS_NUM        1

// PAL_CPPI_PP_DMA15_SGMII1_TX
#define PAL_CPPI_PP_DMA15_SGMII1_TX_CH_CFG_BASE         (IO_ADDRESS(0xF391E000))
#define PAL_CPPI_PP_DMA15_SGMII1_TX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF391F000))
#define PAL_CPPI_PP_DMA15_SGMII1_TX_SCHEDULER_BASE      (IO_ADDRESS(0xF391F020))
#define PAL_CPPI_PP_DMA15_SGMII1_TX_CH                  0
#define PAL_CPPI_PP_DMA15_SGMII1_TX_CHANNELS_NUM        1

// PAL_CPPI_PP_DMA16_RGMII0_TX
#define PAL_CPPI_PP_DMA16_RGMII0_TX_CH_CFG_BASE         (IO_ADDRESS(0xF3920000))
#define PAL_CPPI_PP_DMA16_RGMII0_TX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF3921000))
#define PAL_CPPI_PP_DMA16_RGMII0_TX_SCHEDULER_BASE      (IO_ADDRESS(0xF3921020))
#define PAL_CPPI_PP_DMA16_RGMII0_TX_CH                  0
#define PAL_CPPI_PP_DMA16_RGMII0_TX_CHANNELS_NUM        1

// PAL_CPPI_PP_DMA21_US_COP_RX
#define PAL_CPPI_PP_DMA21_US_COP_RX_CH_CFG_BASE         (IO_ADDRESS(0xF392A000))
#define PAL_CPPI_PP_DMA21_US_COP_RX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF392B000))
#define PAL_CPPI_PP_DMA21_US_COP_RX_SCHEDULER_BASE      (IO_ADDRESS(0xF392B020))
#define PAL_CPPI_PP_DMA21_US_COP_RX_CH                  0
#define PAL_CPPI_PP_DMA21_US_COP_RX_CHANNELS_NUM        1

// PAL_CPPI_PP_DMA22_US_COP_TX
#define PAL_CPPI_PP_DMA22_US_COP_TX_CH_CFG_BASE         (IO_ADDRESS(0xF392C000))
#define PAL_CPPI_PP_DMA22_US_COP_TX_GLOBAL_CTRL_BASE    (IO_ADDRESS(0xF392D000))
#define PAL_CPPI_PP_DMA22_US_COP_TX_SCHEDULER_BASE      (IO_ADDRESS(0xF392D020))
#define PAL_CPPI_PP_DMA22_US_COP_TX_CH                  0
#define PAL_CPPI_PP_DMA22_US_COP_TX_CHANNELS_NUM        1


// PAL_CPPI_PP_DMA23_RGMII1_RX_TX
#define PAL_CPPI_PP_DMA23_RGMII1_RX_TX_CH_CFG_BASE      (IO_ADDRESS(0xF392E000))
#define PAL_CPPI_PP_DMA23_RGMII1_RX_TX_GLOBAL_CTRL_BASE (IO_ADDRESS(0xF392F000))
#define PAL_CPPI_PP_DMA23_RGMII1_RX_TX_SCHEDULER_BASE   (IO_ADDRESS(0xF392F020))
#define PAL_CPPI_PP_DMA23_RGMII1_RX_RAL_CFG_BASE        (IO_ADDRESS(0xF3940038))
// PAL_CPPI_PP_DMA23_RGMII1_RX
typedef enum PAL_CPPI_PP_DMA23_RGMII1_RX_CHANNELS
{
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_512B_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_1KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_1KB_SHARED_DDR,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_PRIVATE_PACKET_RAM,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_SHARED_PACKET_RAM,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_PRIVATE_DDR,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_2KB_SHARED_DDR,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_LOW_4KB_SHARED_DDR,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CH_HIGH_DDR,
    PAL_CPPI_PP_DMA23_RGMII1_RX_CHANNELS_NUM
}PAL_CPPI_PP_DMA23_RGMII1_RX_CHANNELS_e;
// PAL_CPPI_PP_DMA23_RGMII_TX
#define PAL_CPPI_PP_DMA23_RGMII_TX_CH                   0
#define PAL_CPPI_PP_DMA23_RGMII_TX_CHANNELS_NUM         1


#define PAL_CPPI41_NUM_TOTAL_CHAN                       24
#define PAL_CPPI41_PP_NUM_TOTAL_CHAN                    24
#define PAL_CPPI41_DSG_NUM_TOTAL_CHAN                   32
// TBD - US team to set their own value
#define PAL_CPPI41_MAX_TOTAL_CHAN                       32


/*
+-+-+-+ +-+-+-+-+
|D|S|G| |D|M|A|s|
+-+-+-+ +-+-+-+-+
*/


typedef enum PAL_CPPI_DSG_DMA_BLOCKS
{
    PAL_CPPI_OFDM0_RX_DMA,                           /* OFDM0  */
    PAL_CPPI_OFDM1_RX_DMA,                           /* OFDM1  */
    PAL_CPPI_SC_QAM0_RX_DMA,                         /* SC-QAM */

    PAL_CPPI_OFDM0_TX_DMA,                           /* OFDM0  */
    PAL_CPPI_OFDM1_TX_DMA,                           /* OFDM1  */
    PAL_CPPI_SC_QAM0_TX_DMA,                         /* SC-QAM */

    PAL_CPPI_DSG_NUM_DMA_BLOCK

}PAL_CPPI_DSG_DMA_BLOCKS_e;

#define PAL_CPPI_DMA_NUM_TO_QMGR_NUM(dmaNum)        0

#define PAL_CPPI_DSG_NUM_TOTAL_CHAN                 32

/* PAL_CPPI_OFDM0_RX_DMA */
#define PAL_CPPI_OFDM0_RX_DMA_CH_CFG_BASE              (IO_ADDRESS(0xF2150000))
#define PAL_CPPI_OFDM0_RX_DMA_GLOBAL_CTRL_BASE         (IO_ADDRESS(0xF2151000))
#define PAL_CPPI_OFDM0_RX_DMA_SCHEDULER_BASE           (IO_ADDRESS(0xF214E000))

/* PAL_CPPI_OFDM0_TX_DMA */
#define PAL_CPPI_OFDM0_TX_DMA_CH_CFG_BASE              (IO_ADDRESS(0xF2152000))
#define PAL_CPPI_OFDM0_TX_DMA_GLOBAL_CTRL_BASE         (IO_ADDRESS(0xF2153000))
#define PAL_CPPI_OFDM0_TX_DMA_SCHEDULER_BASE           NULL

/* PAL_CPPI_OFDM1_RX_DMA */
#define PAL_CPPI_OFDM1_RX_DMA_CH_CFG_BASE              (IO_ADDRESS(0xF21D0000))
#define PAL_CPPI_OFDM1_RX_DMA_GLOBAL_CTRL_BASE         (IO_ADDRESS(0xF21D1000))
#define PAL_CPPI_OFDM1_RX_DMA_SCHEDULER_BASE           (IO_ADDRESS(0xF21CE000))

/* PAL_CPPI_OFDM1_TX_DMA */
#define PAL_CPPI_OFDM1_TX_DMA_CH_CFG_BASE              (IO_ADDRESS(0xF21D2000))
#define PAL_CPPI_OFDM1_TX_DMA_GLOBAL_CTRL_BASE         (IO_ADDRESS(0xF21D3000))
#define PAL_CPPI_OFDM1_TX_DMA_SCHEDULER_BASE           NULL

/* PAL_CPPI_SC_QAM0_RX_DMA */
#define PAL_CPPI_SC_QAM0_RX_DMA_CH_CFG_BASE            (IO_ADDRESS(0xF2050000))
#define PAL_CPPI_SC_QAM0_RX_DMA_GLOBAL_CTRL_BASE       (IO_ADDRESS(0xF2051000))
#define PAL_CPPI_SC_QAM0_RX_DMA_SCHEDULER_BASE         (IO_ADDRESS(0xF204E000))

/* PAL_CPPI_SC_QAM0_TX_DMA */
#define PAL_CPPI_SC_QAM0_TX_DMA_CH_CFG_BASE            (IO_ADDRESS(0xF2052000))
#define PAL_CPPI_SC_QAM0_TX_DMA_GLOBAL_CTRL_BASE       (IO_ADDRESS(0xF2053000))
#define PAL_CPPI_SC_QAM0_TX_DMA_SCHEDULER_BASE         NULL


/**********************************************************************************************************************

   ###     ######   ######  ##     ## ##     ## ##     ## ##          ###    ########  #######  ########
  ## ##   ##    ## ##    ## ##     ## ###   ### ##     ## ##         ## ##      ##    ##     ## ##     ##
 ##   ##  ##       ##       ##     ## #### #### ##     ## ##        ##   ##     ##    ##     ## ##     ##
##     ## ##       ##       ##     ## ## ### ## ##     ## ##       ##     ##    ##    ##     ## ########
######### ##       ##       ##     ## ##     ## ##     ## ##       #########    ##    ##     ## ##   ##
##     ## ##    ## ##    ## ##     ## ##     ## ##     ## ##       ##     ##    ##    ##     ## ##    ##
##     ##  ######   ######   #######  ##     ##  #######  ######## ##     ##    ##     #######  ##     ##

    Accumulator INTD0 Configuration                                             Accumulator INTD1 Configuration
    ===============================                                             ===============================

    Channel Interrupt   Usage                                                   Channel Interrupt   Usage
    -------------------------------------------                                 -------------------------------------------
    0       0           HOST to PP Tx Complete LOW                              0       0           In Use
    1                   HOST to PP Tx Complete HIGH                             1
    -------------------------------------------                                 -------------------------------------------
    2       1           MoCA RX                                                 2       1           
    3                                                                           3
    4                                                                           4
    5                                                                           5
    -------------------------------------------                                 -------------------------------------------
    6       2           SGMII0 RX Low                                           6       2           Encrypt
    -------------------------------------------                                 -------------------------------------------
    7       3           SGMII0 RX High                                          7       3           APP2NP RX HI
    -------------------------------------------                                 -------------------------------------------
    8       4           SGMII1 RX Low                                           8       4           NP2APP RX
    -------------------------------------------                                 -------------------------------------------
    9       5           SGMII1 RX High                                          9       5           APP2NP RX
    -------------------------------------------                                 -------------------------------------------
    10      6           RGMII0 RX Low                                           10      6           NP2APP TX Complete
    -------------------------------------------                                 -------------------------------------------
    11      7           RGMII0 RX High                                          11      7           APP2NP TX Complete
    -------------------------------------------                                 -------------------------------------------
    12      8           RGMII1 RX Low                                           12      8           WiFi RX 0
    -------------------------------------------                                 -------------------------------------------
    13      9           RGMII1 RX High                                          13      9           WiFi RX 1
    -------------------------------------------                                 -------------------------------------------
    14      10          ATOM RX Low                                             14      10          WiFi RX 2
    -------------------------------------------                                 -------------------------------------------
    15      11          ATOM RX High                                            15      11          WiFi RX 3
    -------------------------------------------                                 -------------------------------------------
    16      12          DOCSIS Management RX
    17
    18
    19
    -------------------------------------------
    20      13          DOCSIS RX Low
    21
    22
    23
    -------------------------------------------
    24      14          DOCSIS RX High
    25
    26
    27
    -------------------------------------------
    28      15          Voice RX
    29
    30
    31
    -------------------------------------------

**********************************************************************************************************************/

/*
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
|A|c|c|u|m|u|l|a|t|o|r| |C|h|a|n|n|e|l|s|
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
*/
typedef enum PAL_CPPI_PP_ACCUMULATOR_INTD0_CHANNELS
{
    PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_CH_BASE,
        PAL_CPPI_PP_HOST2PP_TX_COMPLETE_LOW_INTD0_ACC_CH_NUM = PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_CH_BASE,   // 0
        PAL_CPPI_PP_HOST2PP_TX_COMPLETE_HIGH_INTD0_ACC_CH_NUM,                                                      // 1
    PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_CH_LAST = PAL_CPPI_PP_HOST2PP_TX_COMPLETE_HIGH_INTD0_ACC_CH_NUM,

    PAL_CPPI_PP_MoCA_RX_INTD0_ACC_CH_NUM,                                                                           // 2
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_3,                                                                        // 3
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_4,                                                                        // 4
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_5,                                                                        // 5
    PAL_CPPI_PP_SGMII0_RX_LOW_INTD0_ACC_CH_NUM,                                                                     // 6
    PAL_CPPI_PP_SGMII0_RX_HIGH_INTD0_ACC_CH_NUM,                                                                    // 7
    PAL_CPPI_PP_SGMII1_RX_LOW_INTD0_ACC_CH_NUM,                                                                     // 8
    PAL_CPPI_PP_SGMII1_RX_HIGH_INTD0_ACC_CH_NUM,                                                                    // 9
    PAL_CPPI_PP_RGMII0_RX_LOW_INTD0_ACC_CH_NUM,                                                                     // 10
    PAL_CPPI_PP_RGMII0_RX_HIGH_INTD0_ACC_CH_NUM,                                                                    // 11
    PAL_CPPI_PP_RGMII1_RX_LOW_INTD0_ACC_CH_NUM,                                                                     // 12
    PAL_CPPI_PP_RGMII1_RX_HIGH_INTD0_ACC_CH_NUM,                                                                    // 13
    PAL_CPPI_PP_ATOM_RX_LOW_INTD0_ACC_CH_NUM,                                                                       // 14
    PAL_CPPI_PP_ATOM_RX_HIGH_INTD0_ACC_CH_NUM,                                                                      // 15
    PAL_CPPI_PP_DOCSIS_RX_MGMT_INTD0_ACC_CH_NUM,                                                                    // 16
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_17,                                                                       // 17
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_18,                                                                       // 18
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_19,                                                                       // 19
    PAL_CPPI_PP_DOCSIS_RX_LOW_INTD0_ACC_CH_NUM,                                                                     // 20
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_21,                                                                       // 21
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_22,                                                                       // 22
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_23,                                                                       // 23
    PAL_CPPI_PP_DOCSIS_RX_HIGH_INTD0_ACC_CH_NUM,                                                                    // 24
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_25,                                                                       // 25
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_26,                                                                       // 26
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_27,                                                                       // 27
    PAL_CPPI_PP_VOICE_RX_INTD0_ACC_CH_NUM,                                                                          // 28
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_29,                                                                       // 29
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_30,                                                                       // 30
    PAL_CPPI_PP_RESERVED_INTD0_ACC_CH_NUM_31,                                                                       // 31
    PAL_CPPI_PP_INTD0_ACCUMULATOR_MAX_CHANNELS
}PAL_CPPI_PP_ACCUMULATOR_INTD0_CHANNELS_e;

typedef enum PAL_CPPI_PP_ACCUMULATOR_INTD1_CHANNELS
{
    PAL_CPPI_PP_RESERVED_INTD1_ACC_CH_NUM_0,                                                                        // 0 *** This INTD is already in use ***
    PAL_CPPI_PP_RESERVED_INTD1_ACC_CH_NUM_1,                                                                        // 1 *** This INTD is already in use ***
    PAL_CPPI_PP_RESERVED_INTD1_ACC_CH_NUM_2,                                                                        // 2
    PAL_CPPI_PP_RESERVED_INTD1_ACC_CH_NUM_3,                                                                        // 3
    PAL_CPPI_PP_RESERVED_INTD1_ACC_CH_NUM_4,                                                                        // 4
    PAL_CPPI_PP_RESERVED_INTD1_ACC_CH_NUM_5,                                                                        // 5
    PAL_CPPI_PP_IPSEC_ENCRYPT_INTD1_ACC_CH_NUM,                                                                     // 6
    PAL_CPPI_PP_APP2NP_RX_HI_INTD1_ACC_CH_NUM,                                                                      // 7
    PAL_CPPI_PP_NP2APP_RX_INTD1_ACC_CH_NUM,                                                                         // 8
    PAL_CPPI_PP_NP2APP_TX_COMPLETE_INTD1_ACC_CH_NUM,                                                                // 9
    PAL_CPPI_PP_APP2NP_RX_INTD1_ACC_CH_NUM,                                                                         // 10
    PAL_CPPI_PP_APP2NP_TX_COMPLETE_INTD1_ACC_CH_NUM,                                                                // 11
    PAL_CPPI_PP_WiFi_RX_0_INTD1_ACC_CH_NUM,                                                                         // 12
    PAL_CPPI_PP_WiFi_RX_1_INTD1_ACC_CH_NUM,                                                                         // 13
    PAL_CPPI_PP_WiFi_RX_2_INTD1_ACC_CH_NUM,                                                                         // 14
    PAL_CPPI_PP_WiFi_RX_3_INTD1_ACC_CH_NUM,                                                                         // 15
    PAL_CPPI_PP_INTD1_ACCUMULATOR_MAX_CHANNELS
}PAL_CPPI_PP_ACCUMULATOR_INTD1_CHANNELS_e;

#define PAL_CPPI_PP_ACC_INTD1_CH_TO_ACC_CH(ch)		    ((ch) + PAL_CPPI_PP_INTD0_ACCUMULATOR_MAX_CHANNELS)

/*
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+
|A|c|c|u|m|u|l|a|t|o|r| |I|n|t|e|r|r|u|p|t| |V|e|c|t|o|r|s|
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+
*/
typedef enum PAL_CPPI_PP_ACCUMULATOR_INTD0_INTERRUPT_VECTORS
{
    PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_INTV_NUM,                                                             // 0
    PAL_CPPI_PP_MoCA_RX_INTD0_ACC_INTV_NUM,                                                                         // 1
    PAL_CPPI_PP_SGMII0_LOW_RX_INTD0_ACC_INTV_NUM,                                                                   // 2
    PAL_CPPI_PP_SGMII0_HIGH_RX_INTD0_ACC_INTV_NUM,                                                                  // 3
    PAL_CPPI_PP_SGMII1_LOW_RX_INTD0_ACC_INTV_NUM,                                                                   // 4
    PAL_CPPI_PP_SGMII1_HIGH_RX_INTD0_ACC_INTV_NUM,                                                                  // 5
    PAL_CPPI_PP_RGMII0_LOW_RX_INTD0_ACC_INTV_NUM,                                                                   // 6
    PAL_CPPI_PP_RGMII0_HIGH_RX_INTD0_ACC_INTV_NUM,                                                                  // 7
    PAL_CPPI_PP_RGMII1_LOW_RX_INTD0_ACC_INTV_NUM,                                                                   // 8
    PAL_CPPI_PP_RGMII1_HIGH_RX_INTD0_ACC_INTV_NUM,                                                                  // 9
    PAL_CPPI_PP_ATOM_LOW_RX_INTD0_ACC_INTV_NUM,                                                                     // 10
    PAL_CPPI_PP_ATOM_HIGH_RX_INTD0_ACC_INTV_NUM,                                                                    // 11
    PAL_CPPI_PP_DOCSIS_RX_MGMT_INTD0_ACC_INTV_NUM,                                                                  // 12
    PAL_CPPI_PP_DOCSIS_RX_LOW_INTD0_ACC_INTV_NUM,                                                                   // 13
    PAL_CPPI_PP_DOCSIS_RX_HIGH_INTD0_ACC_INTV_NUM,                                                                  // 14
    PAL_CPPI_PP_VOICE_RX_INTD0_INTV_NUM,                                                                            // 15
    PAL_CPPI_PP_ACCUMULATOR_INTD0_MAX_INTERRUPT_VECTORS
}PAL_CPPI_PP_ACCUMULATOR_INTERRUPT_VECTORS_e;

#define PAL_CPPI41_GBE_ACC_INTV_NUM(devInstance, pri)       (PAL_CPPI_PP_SGMII0_LOW_RX_INTD0_ACC_INTV_NUM + ((devInstance)*2) + (pri))

typedef enum PAL_CPPI_PP_ACCUMULATOR_INTD1_INTERRUPT_VECTORS
{
    PAL_CPPI_PP_RESERVED_INTD1_ACC_INTV_NUM_0,                                                                      // 0 *** This INTV is already in use ***
    PAL_CPPI_PP_RESERVED_INTD1_ACC_INTV_NUM_1,                                                                      // 1
    PAL_CPPI_PP_IPSEC_ENCRYPT_INTD1_ACC_INTV_NUM,                                                                   // 2
    PAL_CPPI_PP_APP2NP_RX_HI_INTD1_ACC_INTV_NUM,                                                                    // 3
    PAL_CPPI_PP_NP2APP_RX_INTD1_ACC_INTV_NUM,                                                                       // 4
    PAL_CPPI_PP_NP2APP_TX_COMPLETE_INTD1_ACC_INTV_NUM,                                                              // 5
    PAL_CPPI_PP_APP2NP_RX_INTD1_ACC_INTV_NUM,                                                                       // 6
    PAL_CPPI_PP_APP2NP_TX_COMPLETE_INTD1_ACC_INTV_NUM,                                                              // 7
    PAL_CPPI_PP_WiFi_0_INTD1_ACC_INTV_NUM,                                                                          // 8
    PAL_CPPI_PP_WiFi_1_INTD1_ACC_INTV_NUM,                                                                          // 9
    PAL_CPPI_PP_WiFi_2_INTD1_ACC_INTV_NUM,                                                                          // 10
    PAL_CPPI_PP_WiFi_3_INTD1_ACC_INTV_NUM,                                                                          // 11
    PAL_CPPI_PP_ACCUMULATOR_INTD1_MAX_INTERRUPT_VECTORS
}PAL_CPPI_PP_ACCUMULATOR_INTD1_INTERRUPT_VECTORS_e;

#define PAL_CPPI41_WPD_ACC_INTV_NUM(devInstance)       (PAL_CPPI_PP_WiFi_0_INTD1_ACC_INTV_NUM + (devInstance))

/**********************************************************************************************************************

 ######   #######  ##     ## ########   ######  ########    ########   #######  ########  ########  ######
##    ## ##     ## ##     ## ##     ## ##    ## ##          ##     ## ##     ## ##     ##    ##    ##    ##
##       ##     ## ##     ## ##     ## ##       ##          ##     ## ##     ## ##     ##    ##    ##
 ######  ##     ## ##     ## ########  ##       ######      ########  ##     ## ########     ##     ######
      ## ##     ## ##     ## ##   ##   ##       ##          ##        ##     ## ##   ##      ##          ##
##    ## ##     ## ##     ## ##    ##  ##    ## ##          ##        ##     ## ##    ##     ##    ##    ##
 ######   #######   #######  ##     ##  ######  ########    ##         #######  ##     ##    ##     ######

**********************************************************************************************************************/

typedef enum PAL_CPPI_PP_SOURCE_PORTS
{
    PAL_CPPI_PP_OFDM0_RX_SOURCE_PORT,               // 0
    PAL_CPPI_PP_OFDM1_RX_SOURCE_PORT,               // 1
    PAL_CPPI_PP_SC_QAM_RX_SOURCE_PORT,              // 2
    PAL_CPPI_PP_SGMII0_RX_SOURCE_PORT,              // 3
    PAL_CPPI_PP_SGMII1_RX_SOURCE_PORT,              // 4
    PAL_CPPI_PP_RGMII0_RX_SOURCE_PORT,              // 5
    PAL_CPPI_PP_MoCA_RX_SOURCE_PORT,                // 6
    PAL_CPPI_PP_ATOM_RX_SOURCE_PORT,                // 7
    PAL_CPPI_PP_RGMII1_RX_SOURCE_PORT,              // 8
    PAL_CPPI_PP_VOICE_DSP_C55_SOURCE_PORT,          // 9
    PAL_CPPI_PP_RESERVED_SOURCE_PORT_10,            // 10
    PAL_CPPI_PP_RESERVED_SOURCE_PORT_11,            // 11
    PAL_CPPI_PP_RESERVED_SOURCE_PORT_12,            // 12
    PAL_CPPI_PP_RESERVED_SOURCE_PORT_13,            // 13
    PAL_CPPI_PP_RESERVED_SOURCE_PORT_14,            // 14
    PAL_CPPI_PP_RESERVED_SOURCE_PORT_15,            // 15
    PAL_CPPI_PP_WiFi_PORT0_SOURCE_PORT,             // 16
    PAL_CPPI_PP_WiFi_PORT1_SOURCE_PORT,             // 17
    PAL_CPPI_PP_WiFi_PORT2_SOURCE_PORT,             // 18
    PAL_CPPI_PP_WiFi_PORT3_SOURCE_PORT,             // 19
    PAL_CPPI_PP_WiFi_PORT4_SOURCE_PORT,             // 20
    PAL_CPPI_PP_WiFi_PORT5_SOURCE_PORT,             // 21
    PAL_CPPI_PP_WiFi_PORT6_SOURCE_PORT,             // 22
    PAL_CPPI_PP_WiFi_PORT7_SOURCE_PORT,             // 23
    PAL_CPPI_PP_WiFi_PORT8_SOURCE_PORT,             // 24
    PAL_CPPI_PP_WiFi_PORT9_SOURCE_PORT,             // 25
    PAL_CPPI_PP_WiFi_PORT10_SOURCE_PORT,            // 26
    PAL_CPPI_PP_WiFi_PORT11_SOURCE_PORT,            // 27
    PAL_CPPI_PP_WiFi_PORT12_SOURCE_PORT,            // 28
    PAL_CPPI_PP_WiFi_PORT13_SOURCE_PORT,            // 29
    PAL_CPPI_PP_WiFi_PORT14_SOURCE_PORT,            // 30
    PAL_CPPI_PP_WiFi_PORT15_SOURCE_PORT,            // 31

    PAL_CPPI_PP_MAX_SOURCE_PORTS

}PAL_CPPI_PP_SOURCE_PORTS_e;

#define CPPI41_SRCPORT_VOICE_DSP_C55      PAL_CPPI_PP_VOICE_DSP_C55_SOURCE_PORT






/**********************************************************************************************************************

 ######   ######## ##    ## ######## ########     ###    ##
##    ##  ##       ###   ## ##       ##     ##   ## ##   ##
##        ##       ####  ## ##       ##     ##  ##   ##  ##
##   #### ######   ## ## ## ######   ########  ##     ## ##
##    ##  ##       ##  #### ##       ##   ##   ######### ##
##    ##  ##       ##   ### ##       ##    ##  ##     ## ##
 ######   ######## ##    ## ######## ##     ## ##     ## ########

**********************************************************************************************************************/

/*
+-+-+ +-+-+-+-+-+-+-+
|P|P| |G|e|n|e|r|a|l|
+-+-+ +-+-+-+-+-+-+-+
*/
#define PAL_CPPI_PP_ATOM_INFRA_INPUT_Q_NUM(pri)                                (PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_ATOM_LOW_Q_NUM + (2*(pri)))
#define PAL_CPPI_PP_RGMII0_INFRA_INPUT_Q_NUM(pri)                              (PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_RGMII0_LOW_Q_NUM + (2*(pri)))
#define PAL_CPPI_PP_RGMII1_INFRA_INPUT_Q_NUM(pri)                              (PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_RGMII1_LOW_Q_NUM + (2*(pri)))
#define PAL_CPPI_PP_SGMII0_INFRA_INPUT_Q_NUM(pri)                              (PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_SGMII0_LOW_Q_NUM + (2*(pri)))
#define PAL_CPPI_PP_SGMII1_INFRA_INPUT_Q_NUM(pri)                              (PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_SGMII1_LOW_Q_NUM + (2*(pri)))
#define PAL_CPPI_PP_ATOM_HOST_RX_COMPLETE_Q_NUM(pri)                           (PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_ATOM_LOW_Q_NUM + (pri))
#define PAL_CPPI_PP_RGMII0_HOST_RX_COMPLETE_Q_NUM(pri)                         (PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_RGMII0_LOW_Q_NUM + (pri))
#define PAL_CPPI_PP_RGMII1_HOST_RX_COMPLETE_Q_NUM(pri)                         (PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_RGMII1_LOW_Q_NUM + (pri))
#define PAL_CPPI_PP_SGMII0_HOST_RX_COMPLETE_Q_NUM(pri)                         (PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_SGMII0_LOW_Q_NUM + (pri))
#define PAL_CPPI_PP_SGMII1_HOST_RX_COMPLETE_Q_NUM(pri)                         (PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_SGMII1_LOW_Q_NUM + (pri))
#define PAL_CPPI_PP_ATOM_INFRA_DMA_CH_COUNT                                    (PAL_CPPI_PP_DMA10_ATOM_HIGH_INFRA_RX_CH - PAL_CPPI_PP_DMA10_ATOM_LOW_INFRA_RX_CH + 1)
#define PAL_CPPI_PP_RGMII0_INFRA_DMA_CH_COUNT                                  (PAL_CPPI_PP_DMA10_RGMII0_HIGH_INFRA_RX_CH - PAL_CPPI_PP_DMA10_RGMII0_LOW_INFRA_RX_CH + 1)
#define PAL_CPPI_PP_RGMII1_INFRA_DMA_CH_COUNT                                  (PAL_CPPI_PP_DMA10_RGMII1_HIGH_INFRA_RX_CH - PAL_CPPI_PP_DMA10_RGMII1_LOW_INFRA_RX_CH + 1)
#define PAL_CPPI_PP_SGMII0_INFRA_DMA_CH_COUNT                                  (PAL_CPPI_PP_DMA10_SGMII0_HIGH_INFRA_RX_CH - PAL_CPPI_PP_DMA10_SGMII0_LOW_INFRA_RX_CH + 1)
#define PAL_CPPI_PP_SGMII1_INFRA_DMA_CH_COUNT                                  (PAL_CPPI_PP_DMA10_SGMII1_HIGH_INFRA_RX_CH - PAL_CPPI_PP_DMA10_SGMII1_LOW_INFRA_RX_CH + 1)
#define PAL_CPPI_PP_ATOM_INFRA_DMA_CH_NUM(ch)                                  (PAL_CPPI_PP_DMA10_ATOM_LOW_INFRA_RX_CH + (ch))
#define PAL_CPPI_PP_RGMII0_INFRA_DMA_CH_NUM(ch)                                (PAL_CPPI_PP_DMA10_RGMII0_LOW_INFRA_RX_CH + (ch))
#define PAL_CPPI_PP_RGMII1_INFRA_DMA_CH_NUM(ch)                                (PAL_CPPI_PP_DMA10_RGMII1_LOW_INFRA_RX_CH + (ch))
#define PAL_CPPI_PP_SGMII0_INFRA_DMA_CH_NUM(ch)                                (PAL_CPPI_PP_DMA10_SGMII0_LOW_INFRA_RX_CH + (ch))
#define PAL_CPPI_PP_SGMII1_INFRA_DMA_CH_NUM(ch)                                (PAL_CPPI_PP_DMA10_SGMII1_LOW_INFRA_RX_CH + (ch))

#define PAL_CPPI_PP_ATOM_INFRA_HOST_FD_Q_NUM(pri)                              ((pri) == 0 ? PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM : PAL_CPPI_PP_QMGR_G2_ATOM_HI_INFRA_HOST_FD_Q_NUM)
#define PAL_CPPI_PP_RGMII0_INFRA_HOST_FD_Q_NUM(pri)                            ((pri) == 0 ? PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM : PAL_CPPI_PP_QMGR_G2_RGMII0_HI_INFRA_HOST_FD_Q_NUM)
#define PAL_CPPI_PP_RGMII1_INFRA_HOST_FD_Q_NUM(pri)                            ((pri) == 0 ? PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM : PAL_CPPI_PP_QMGR_G2_RGMII1_HI_INFRA_HOST_FD_Q_NUM)
#define PAL_CPPI_PP_SGMII0_INFRA_HOST_FD_Q_NUM(pri)                            ((pri) == 0 ? PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM : PAL_CPPI_PP_QMGR_G2_SGMII0_HI_INFRA_HOST_FD_Q_NUM)
#define PAL_CPPI_PP_SGMII1_INFRA_HOST_FD_Q_NUM(pri)                            ((pri) == 0 ? PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM : PAL_CPPI_PP_QMGR_G2_SGMII1_HI_INFRA_HOST_FD_Q_NUM)
#define PAL_CPPI_PP_WIFI_INFRA_HOST_FD_Q_NUM(pri)                              ((pri) == 0 ? PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM : PAL_CPPI_PP_QMGR_G2_WIFI_INFRA_HOST_RX_FD_Q_NUM)

/* DOCSIS */
#define PAL_CPPI_PP_DOCSIS_INFRA_INPUT_Q_NUM(pri)                               (PAL_CPPI_PP_QMGR_G1_CDMA10_INFRA_INPUT_LOW_DOCSIS_LOW_Q_NUM + (2*(pri)))
#define PAL_CPPI_PP_DOCSIS_HOST_RX_COMPLETE_Q_NUM(pri)                          (PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_DOCSIS_LOW_Q_NUM + (pri))
#define PAL_CPPI_PP_DOCSIS_INFRA_DMA_CH_COUNT                                   (PAL_CPPI_PP_DMA10_DOCSIS_HIGH_INFRA_RX_CH - PAL_CPPI_PP_DMA10_DOCSIS_LOW_INFRA_RX_CH + 1)
#define PAL_CPPI_PP_DOCSIS_INFRA_DMA_CH_NUM(ch)                                 (PAL_CPPI_PP_DMA10_DOCSIS_LOW_INFRA_RX_CH + (ch))
#define PAL_CPPI_PP_DOCSIS_INFRA_HOST_FD_Q_NUM(pri)                             (PAL_CPPI_PP_QMGR_G2_DOCSIS_LOW_INFRA_HOST_FD_Q_NUM + (pri))
#define PAL_CPPI_PP_DOCSIS_TX_DATA_Q_NUM(pri)                                   (PAL_CPPI_PP_QMGR_G1_US_PREPROCESSING_VOICE_Q_NUM + (pri))
#define PAL_CPPI_PP_DOCSIS_TX_DATA_Q_COUNT                                      (PAL_CPPI_PP_QMGR_G1_US_PREPROCESSING_BE15_LOW_Q_NUM - PAL_CPPI_PP_QMGR_G1_US_PREPROCESSING_VOICE_Q_NUM + 1)
#define PAL_CPPI_PP_DOCSIS_RX_SOURCE_PORTS_COUNT                                (PAL_CPPI_PP_SC_QAM_RX_SOURCE_PORT - PAL_CPPI_PP_OFDM0_RX_SOURCE_PORT + 1)
#define PAL_CPPI_PP_DOCSIS_RX_SOURCE_PORT(dsg)                                  (PAL_CPPI_PP_OFDM0_RX_SOURCE_PORT + (dsg))

#ifdef CONFIG_MACH_PUMA7_FPGA_US
#define PAL_CPPI_PP_DOCSIS_TX_DMA_CH_COUNT                          (PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS_NUM)
#else
#define PAL_CPPI_PP_DOCSIS_TX_DMA_CH_COUNT                          (PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS_NUM + PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX_CHANNELS_NUM)
#endif
#define PAL_CPPI_PP_DOCSIS_TX_DMA_CH_NUM(ch)                        (((ch) <  PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS_NUM)? (ch):(ch)-PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS_NUM)
#define PAL_CPPI_PP_DOCSIS_TX_DMA_BLOCK(ch)                         (((ch) <  PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX_CHANNELS_NUM)? PAL_CPPI_PP_DMA12_OFDM0_TX_QAM_0_3_TX : PAL_CPPI_PP_DMA13_OFDM1_TX_QAM_4_7_TX )

#define PAL_CPPI_PP_DOCSIS_US_COP_TX_DMA_BLOCK                      ( PAL_CPPI_PP_DMA22_US_COP_TX )
#define PAL_CPPI_PP_DOCSIS_US_COP_RX_DMA_BLOCK                      ( PAL_CPPI_PP_DMA21_US_COP_RX )

/* Host --> PP */
#define PAL_CPPI_PP_HOST2PP_Q_COUNT                                             (PAL_CPPI_PP_QMGR_G2_HOST2PP_HI_HOST_FD_Q_NUM - PAL_CPPI_PP_QMGR_G2_HOST2PP_LOW_HOST_FD_Q_NUM + 1)
#define PAL_CPPI_PP_HOST2PP_PP_INPUT_Q_NUM(pri)                                 (PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q0 + (pri))
#define PAL_CPPI_PP_HOST2PP_INFRA_INPUT_Q_NUM(pri)                              (PAL_CPPI_PP_QMGR_G1_CDMA11_INFRA_INPUT_HI_HOST2PP_LOW_Q_NUM + (2*(pri)))
#define PAL_CPPI_PP_HOST2PP_INFRA_EMB_FD_Q_NUM(pri)                             (PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_LOW_EMB_FD_Q_NUM + (pri))
#define PAL_CPPI_PP_HOST2PP_INFRA_DMA_CH_COUNT                                  (PAL_CPPI_PP_DMA11_HOST2PP_HIGH_INFRA_RX_CH - PAL_CPPI_PP_DMA11_HOST2PP_LOW_INFRA_RX_CH + 1)
#define PAL_CPPI_PP_HOST2PP_INFRA_DMA_CH_NUM(ch)                                (PAL_CPPI_PP_DMA11_HOST2PP_LOW_INFRA_RX_CH + (ch))
#define PAL_CPPI_PP_HOST2PP_TX_COMPLETE_Q_NUM(pri)                              (PAL_CPPI_PP_QMGR_G2_HOST2PP_LOW_TX_COMPLETE_Q_NUM + (pri))
#define PAL_CPPI_PP_HOST2PP_HOST_FD_Q_NUM(pri)                                  (PAL_CPPI_PP_QMGR_G2_HOST2PP_LOW_HOST_FD_Q_NUM + (pri))
#define PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_NUM(pri)                         (PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_CH_BASE + (pri))
#define PAL_CPPI_PP_HOST2PP_TX_COMPLETE_ACC_CH_COUNT                            (PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_CH_LAST - PAL_CPPI_PP_HOST2PP_TX_COMPLETE_INTD0_ACC_CH_BASE + 1)
#define PAL_CPPI_NETDEV_BUILD_Q_INFO(qMgr, qNum)                                ( ((qMgr) << CPPI41_EM_PKTINFO_RETQMGR_SHIFT) | (qNum) )

#ifdef CONFIG_MACH_PUMA7_FPGA
#define PAL_CPPI_PP_TEARDOWN_FD_DESC_COUNT                                      (2 * PAL_CPPI41_NUM_DMA_BLOCK)
#else
#define PAL_CPPI_PP_TEARDOWN_FD_DESC_COUNT                                      (PAL_CPPI41_NUM_TOTAL_CHAN * 2 * PAL_CPPI41_NUM_DMA_BLOCK)
#endif
#define PAL_CPPI_PP_TEARDOWN_FD_DESC_SIZE                                       (32)


#define PAL_CPPI41_SR_GBE_INFRA_FD_HOST_DESC_COUNT                              (512)

#define PAL_CPPI41_SR_GBE_HOST_RX_Q_BASE                                        (20)
#define PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM                                     (PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM)
#define PAL_CPPI_GBE_INFRA_DMA_CH_COUNT                                         (2)
#define PAL_CPPI_WPD_INFRA_DMA_CH_COUNT                                         (2)


#define PAL_CPPI41_SR_GBE_TX_DATA_Q_COUNT                                       (1)
#define PAL_CPPI41_SR_WPD_TX_DATA_Q_COUNT                                       (1)

#define PAL_CPPI41_SR_GBE_QOS_Q_PRIORITIES_COUNT                                (4)
#define PAL_CPPI41_SR_GBE_QOS_Q_PRIORITY_HIGH                                   (3)
#define PAL_CPPI41_SR_GBE_QOS_Q_PRIORITY_MEDHIGH                                (2)
#define PAL_CPPI41_SR_GBE_QOS_Q_PRIORITY_MEDLOW                                 (1)
#define PAL_CPPI41_SR_GBE_QOS_Q_PRIORITY_LOW                                    (0)

#define PAL_CPPI41_SR_WPD_QOS_Q_PRIORITIES_COUNT                                (4)
#define PAL_CPPI41_SR_WPD_QOS_Q_PRIORITY_HIGH                                   (3)
#define PAL_CPPI41_SR_WPD_QOS_Q_PRIORITY_MEDHIGH                                (2)
#define PAL_CPPI41_SR_WPD_QOS_Q_PRIORITY_MEDLOW                                 (1)
#define PAL_CPPI41_SR_WPD_QOS_Q_PRIORITY_LOW                                    (0)

#define PAL_CPPI41_SR_HOST_TO_PP_Q_COUNT                                        (PAL_CPPI_PP_HOST2PP_Q_COUNT)


/* WIFI */
/* host rx - rx channels */
/* convert wifi device id to host rx infra dma rx channel */
#define WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH(dev_id)                         \
    ((dev_id) + PAL_CPPI_PP_DMA08_WIFI_DEVICE0_HOST_RX_INFRA_RX_CH)

/* convert wifi device id to host rx infra dma rx channel output queue */
#define WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH_OUT_Q(dev_id)                   \
    ((dev_id) + PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_WIFI_DEVICE0_Q_NUM)

/* host rx infra dma rx channel output queue manager */
#define WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_RX_CH_OUT_Q_MGR                       \
    (PAL_CPPI_PP_QMGR_G2)

/* host rx - tx channels */
/* convert wifi device id to host rx infra dma tx channel */
#define WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_TX_CH(dev_id)                         \
    ((dev_id) + PAL_CPPI_PP_DMA08_WIFI_DEVICE0_HOST_RX_INFRA_TX_CH)

/* convert wifi device id to host rx infra dma tx channel input queue */
#define WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_TX_CH_INPUT_Q(dev_id)                 \
    (((dev_id)*2) + PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE0_HOST_RX_HI_Q_NUM)

/* host rx infra dma tx channel input queue manager*/
#define WIFI_DEVICE_TO_WIFI_HOST_RX_INFRA_TX_CH_INPUT_Q_MGR                     \
    (PAL_CPPI_PP_QMGR_G2)

/* tx fw - rx channels */
/* convert wifi device id to tx fw infra dma 08 rx channel */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_08_RX_CH(dev_id)                        \
    ((dev_id) + PAL_CPPI_PP_DMA08_WIFI_DEVICE0_TX_FW_INFRA_RX_CH)

/* convert wifi device id to tx fw infra dma 09 rx channel */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_09_RX_CH(dev_id)                        \
    ((dev_id) + PAL_CPPI_PP_DMA09_WIFI_DEVICE0_TX_FW_INFRA_RX_CH)

/* tx fw - rx channels */
/* convert wifi device id to tx fw infra dma 11 rx channel */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_11_RX_CH(dev_id)                        \
    ((dev_id) + PAL_CPPI_PP_DMA11_WIFI_DEVICE0_TX_FW_INFRA_RX_CH)


/* convert wifi device id to tx fw infra dma rx channel output queue */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_RX_CH_OUT_Q(dev_id)                     \
    ((dev_id) + PAL_CPPI_PP_QMGR_G1_WIFI_TX_DEV0_INPUT_Q)

/* tx fw infra dma rx channel output queue manager */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_RX_CH_OUT_Q_MGR                         \
    (PAL_CPPI_PP_QMGR_G1)

/* convert wifi device id to device's tx fw input queue */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INPUT_Q(dev_id)                               \
    ((dev_id) + PAL_CPPI_PP_QMGR_G1_WIFI_TX_DEV0_INPUT_Q)

/* wifi device's tx fw input queue */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INPUT_Q_MGR                                   \
    (PAL_CPPI_PP_QMGR_G1)

/* tx fw - tx channels */
/* convert wifi device id to tx fw infra dma 08 tx channel */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_08_TX_CH(dev_id)                        \
    ((dev_id) + PAL_CPPI_PP_DMA08_WIFI_DEVICE0_TX_FW_INFRA_TX_CH)

/* convert wifi device id to tx fw infra dma 09 tx channel */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_09_TX_CH(dev_id)                        \
    ((dev_id) + PAL_CPPI_PP_DMA09_WIFI_DEVICE0_TX_FW_INFRA_TX_CH)

/* tx fw - tx channels */
/* convert wifi device id to tx fw infra dma 11 tx channel */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_11_TX_CH(dev_id)                        \
    ((dev_id) + PAL_CPPI_PP_DMA11_WIFI_DEVICE0_TX_FW_INFRA_TX_CH)


/* convert wifi device id to tx fw infra dma tx channel input queue */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_TX_CH_INPUT_Q(dev_id)                   \
    (((dev_id)*2) + PAL_CPPI_PP_QMGR_G1_CDMA11_INFRA_INPUT_WIFI_DEVICE0_HOST_TX_HI_Q_NUM)

/* tx fw infra dma tx channel input queue manager */
#define WIFI_DEVICE_TO_WIFI_TX_FW_INFRA_TX_CH_INPUT_Q_MGR                       \
    (PAL_CPPI_PP_QMGR_G1)

/* convert buffer pool num to BPQ address */
#define WIFI_TX_BUFFER_POOL_NUM_TO_QUEUE_NUM(poolNum)                           \
    ((poolNum) + PAL_CPPI_PP_QMGR_G1_WIFI_TX_BPQ_0)

#define WIFI_TX_BUFFER_POOL_NUM_TO_BPQ_ADDR(poolNum)                            \
    PAL_CPPI_PP_QMGR_QUEUES_PHY_ADDR(PAL_CPPI_PP_QMGR_G1, WIFI_TX_BUFFER_POOL_NUM_TO_QUEUE_NUM(poolNum))


/* VOICE */
#define PAL_CPPI_PP_VOICE_DSP_C55_EMB_BD_COUNT                                  (256)
#define PAL_CPPI41_VOICE_DSP_C55_EMB_BD_SIZE                                    (PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE)

#if 0

/************************************************************************/
/*                                                                      */
/*                ____ ____ ____    ____  ____  ____                    */
/*               / ___| ___| ___|  |  _ \/ ___||  _ \                   */
/*              | |   |___ \___ \  | | | \___ \| |_) |                  */
/*              | |___ ___) |__) | | |_| |___) |  __/                   */
/*               \____|____/____/  |____/|____/|_|                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/
#define PAL_CPPI41_VOICE_DSP_C55_QMGR                       PAL_CPPI41_QUEUE_MGR_PARTITION_SR

#define PAL_CPPI41_VOICE_DSP_C55_ACC_RX_INTV                PAL_CPPI41_VOICE_DSP_C55_ACC_RX_INTV_NUM
#define PAL_CPPI41_VOICE_DSP_C55_ACC_RX_CHNUM               PAL_CPPI41_VOICE_DSP_C55_ACC_RX_CH_NUM

#define PAL_CPPI41_VOICE_DSP_C55_INFRA_CHN                  PAL_CPPI41_SR_C55_INFRA_DMA2_TX_CH_NUM
#define PAL_CPPI41_VOICE_DSP_C55_INFRA_DMA_ID               PAL_CPPI41_DMA_BLOCK2
#define PAL_CPPI41_VOICE_DSP_C55_INFRA_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define PAL_CPPI41_VOICE_DSP_C55_INFRA_TD_QNUM              PAL_CPPI41_SR_DMA_FD_TEARDOWN_Q_NUM
#define PAL_CPPI41_VOICE_DSP_C55_INFRA_INPUT_LOW_Q_NUM      (PAL_CPPI41_SR_DMA2_C55_INFRA_INPUT_LOW_Q_NUM)

#ifdef CONFIG_INTEL_KERNEL_VOICE_SUPPORT
#define PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT                 256
#define PAL_CPPI41_SR_VOICE_DSP_VNI_FD_EMB_Q_COUNT            ((PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT) / 2)
#define PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_COUNT              ((PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT) / 2)
#define PAL_CPPI41_VOICE_DSP_C55_EMB_BD_SIZE                  64
#else
#define PAL_CPPI41_VOICE_DSP_C55_EMB_BD_COUNT                 0
#define PAL_CPPI41_SR_VOICE_DSP_VNI_FD_EMB_Q_COUNT            0
#define PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_COUNT              0
#define PAL_CPPI41_VOICE_DSP_C55_EMB_BD_SIZE                  0
#endif


#define PAL_CPPI41_VOICE_DSP_C55_INPUT_QNUM         PAL_CPPI41_SR_VOICE_DSP_C55_INPUT_Q_NUM

#define PAL_CPPI41_VOICE_DSP_C55_HOST_RX_Q_NUM      PAL_CPPI41_SR_VOICE_DSP_C55_HOST_RX_Q_NUM


#define PAL_CPPI41_VOICE_NI_OUTPUT_QNUM             PAL_CPPI41_SR_HOST_TO_QPDSP_EMB_TYPE_Q_NUM(PAL_CPPI4x_PRTY_HIGH)
#endif

#endif

