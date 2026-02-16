/*
 *
 * puma7_cppi.h
 * Description:
 * File containing CPPI configurations for each driver.
 * Put into a single file to (hopefully) avoid configuration
 * clashes.

   This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2014 Intel Corporation. All rights reserved.

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


#ifndef __PUMA7_CPPI_H__
#define __PUMA7_CPPI_H__

#include "puma7_cppi_prv.h"
#ifdef CONFIG_ARM_AVALANCHE_SOC
#define PAL_CPPI4_CACHE_INVALIDATE(addr, size)              dma_cache_inv ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK(addr, size)               dma_cache_wback ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(addr, size)    dma_cache_wback_inv ((unsigned long)(addr), (size))
#endif
extern int Puma_DOCSIS_CPPI_Init(void);
extern int Puma_FCC_CPPI_Init(void);

extern Uint8 *PalCppiPpLqmgrNames[PAL_CPPI_PP_QMGR_LOCAL_TOTAL_Q_COUNT];
extern Uint8 *PalCppiPpGqmgr0Names[PAL_CPPI_PP_QMGR_G0_TOTAL_Q_COUNT];
extern Uint8 *PalCppiPpGqmgr1Names[PAL_CPPI_PP_QMGR_G1_TOTAL_Q_COUNT];
extern Uint8 *PalCppiPpGqmgr2Names[PAL_CPPI_PP_QMGR_G2_TOTAL_Q_COUNT];
extern Uint8 *PalCppiPpbmgrNames[PAL_CPPI41_BMGR_MAX_POOLS];
extern Uint8 *PalCppiDsg0qmgrNames[PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT];
extern Uint8 *PalCppiDsg1qmgrNames[PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT];
extern Uint8 *PalCppiDsg2qmgrNames[PAL_CPPI_DSG_QMGR_TOTAL_Q_COUNT];

#define PAL_CPPI_QMGR_SHIFT      12
#define PAL_CPPI_QMGR_MASK       0x3
#define PAL_CPPI_QNUM_MASK       0xFFF

#define PAL_CPPI_PP_QMGR_GET_Q_NAME(qMgr, qNum)                         \
    ((qMgr) == PAL_CPPI_PP_QMGR_G0 ? PalCppiPpGqmgr0Names[(qNum)] :     \
    ((qMgr) == PAL_CPPI_PP_QMGR_G1 ? PalCppiPpGqmgr1Names[(qNum)] :     \
    ((qMgr) == PAL_CPPI_PP_QMGR_G2 ? PalCppiPpGqmgr2Names[(qNum)] : PalCppiPpLqmgrNames[(qNum)])))

#define PAL_CPPI_PP_GET_QMGR(qMgrNum)       (((qMgrNum) >> PAL_CPPI_QMGR_SHIFT) & PAL_CPPI_QMGR_MASK)
#define PAL_CPPI_PP_GET_QNUM(qMgrNum)       ((qMgrNum) & PAL_CPPI_QNUM_MASK)
#define PAL_CPPI_PP_GET_QNAME(qMgrNum)      PAL_CPPI_PP_QMGR_GET_Q_NAME(PAL_CPPI_PP_GET_QMGR(qMgrNum), PAL_CPPI_PP_GET_QNUM(qMgrNum))


#define PAL_CPPI_DSG_QMGR_GET_Q_NAME(qMgr, qNum)                            \
    ((qMgr) == PAL_CPPI_OFDM0_QUEUE_MGR ? PalCppiDsg0qmgrNames[(qNum)] :     \
    ((qMgr) == PAL_CPPI_OFDM1_QUEUE_MGR ? PalCppiDsg1qmgrNames[(qNum)] : PalCppiDsg2qmgrNames[(qNum)]))

#define PAL_CPPI_DSG_GET_QNAME(qMgrNum)     PAL_CPPI_DSG_QMGR_GET_Q_NAME((((qMgrNum) >> PAL_CPPI_QMGR_SHIFT) & PAL_CPPI_QMGR_MASK), ((qMgrNum) & PAL_CPPI_QNUM_MASK))


/***********************************/
/* Backward compatible definitions */
/***********************************/
// The following defines are to prevent many #ifdef in the code due to name convention change in P7
#define PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT                    PAL_CPPI_PP_QMGR_G0_TOTAL_Q_COUNT   /* This is the Qmgr with the maximum number of queues */
#define PAL_CPPI41_SR_QPDSP_QOS_Q_LAST                      PAL_CPPI_PP_QMGR_G1_QOS_Q_LAST
#define PAL_CPPI41_SR_QPDSP_QOS_Q_BASE                      PAL_CPPI_PP_QMGR_G1_QOS_Q_BASE
#define PAL_CPPI41_SR_DOCSIS_TX_QPDSP_QOS_Q_BASE            PAL_CPPI_PP_QMGR_G1_QOS_US_Q_BASE
#define PAL_CPPI41_SR_DOCSIS_TX_QPDSP_QOS_Q_LAST            PAL_CPPI_PP_QMGR_G1_QOS_US_Q_LAST
#define PAL_CPPI_PP_QMGR_G2_AQM_US_BE_BASE                  PAL_CPPI_PP_QMGR_G2_AQM_US_BE0_HI_Q_NUM
#define PAL_CPPI_PP_QMGR_G2_AQM_US_BE_LAST                  PAL_CPPI_PP_QMGR_G2_AQM_US_BE15_LOW_Q_NUM

#define IS_QOS_Q(q) ((q >= PAL_CPPI_NETDEV_BUILD_Q_INFO(PAL_CPPI_PP_QMGR_G1, PAL_CPPI_PP_QMGR_G1_QOS_Q_BASE))       && (q <= PAL_CPPI_NETDEV_BUILD_Q_INFO(PAL_CPPI_PP_QMGR_G1, PAL_CPPI_PP_QMGR_G1_QOS_Q_LAST)) )
#define IS_AQM_Q(q) ((q >= PAL_CPPI_NETDEV_BUILD_Q_INFO(PAL_CPPI_PP_QMGR_G2, PAL_CPPI_PP_QMGR_G2_AQM_US_BE_BASE))   && (q <= PAL_CPPI_NETDEV_BUILD_Q_INFO(PAL_CPPI_PP_QMGR_G2, PAL_CPPI_PP_QMGR_G2_AQM_US_BE_LAST)) )

#define PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_SIZE      PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE
#define PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_SIZE      PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_DESC_SIZE
#define PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_BUFF_SIZE      PAL_CPPI_PP_QMGR_GLOBAL_DEFAULT_BUFF_SIZE

#endif /* __PUMA7_CPPI_H__ */
