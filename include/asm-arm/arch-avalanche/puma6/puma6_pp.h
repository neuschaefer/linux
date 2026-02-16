/* 
  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

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
/*
 * puma6_pp.h
 * Description:
 * See below.
 *
 */

/*
 * This file contains Packet Processor configuration. All the configurations
 * here are system level and hence a complete knowledge of configurations of all
 * devices/drivers related to the Packet Processor is required before modifying
 * any value.
 *
 * The private data structures used by avalanche_ppd_init() are also present in
 * this file and should not be modified unless the function avalanche_ppd_init
 * in file puma6_pp.c is also required to be modified for PP configuration.
 *
 */

#ifndef _INCLUDE_PUMA6_PP_H
#define _INCLUDE_PUMA6_PP_H

#define MAX_IP_PACKET_SIZE      1514
#define MIN_IP_PACKET_SIZE      64

/*
 * PID Range configurations
 */
typedef enum PP_PID_NUM
{
    PP_L2SW_PID_BASE = 0,
        PP_L2SW_APP0_PID_NUM = PP_L2SW_PID_BASE,                    // 0
        PP_L2SW_MoCA0_PID_NUM,                                      // 1
        PP_L2SW_ETH0_PID_NUM,                                       // 2
        PP_L2SW_ETH1_PID_NUM,                                       // 3
        PP_L2SW_WLAN1_PID_NUM,                                      // 4
        PP_L2SW_WLAN0_PID_NUM,                                      // 5
    PP_L2SW_PID_LAST = PP_L2SW_WLAN0_PID_NUM,

    PP_CNI_PID_BASE,                                                // 6
    PP_L2SW_L2SW0_PID_NUM,                                          // 7
}PP_PID_NUM_e;

#define PP_L2SW_PID_COUNT   PP_L2SW_PID_LAST - PP_L2SW_PID_BASE + 1
#define PP_CNI_PID_COUNT    1

#define PP_ETH_PID_COUNT    1   // for backward compatible
#define PP_ETH_PID_BASE     31  // for backward compatible
#define PP_USB_PID_COUNT    4   // for backward compatible
#define PP_USB_PID_BASE     26  // for backward compatible

#define PP_C55_PID_COUNT    1   /* configuration for C55 DSP */
#define PP_C55_PID_BASE     15  /* configuration for C55 DSP */

/* QoS Clusters definitions */
typedef enum PAL_CPPI41_SR_QOS_CLUSTERS
{
    PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_BASE,
        PAL_CPPI41_SR_DOCSIS_TX_BE0_QOS_CLUSTER_NUM = PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_BASE,     // 0
        PAL_CPPI41_SR_DOCSIS_TX_BE1_QOS_CLUSTER_NUM,                                                // 1
        PAL_CPPI41_SR_DOCSIS_TX_BE2_QOS_CLUSTER_NUM,                                                // 2
        PAL_CPPI41_SR_DOCSIS_TX_BE3_QOS_CLUSTER_NUM,                                                // 3
        PAL_CPPI41_SR_DOCSIS_TX_BE4_QOS_CLUSTER_NUM,                                                // 4
        PAL_CPPI41_SR_DOCSIS_TX_BE5_QOS_CLUSTER_NUM,                                                // 5
        PAL_CPPI41_SR_DOCSIS_TX_BE6_QOS_CLUSTER_NUM,                                                // 6
        PAL_CPPI41_SR_DOCSIS_TX_BE7_QOS_CLUSTER_NUM,                                                // 7
        PAL_CPPI41_SR_DOCSIS_TX_BE8_QOS_CLUSTER_NUM,                                                // 8
        PAL_CPPI41_SR_DOCSIS_TX_BE9_QOS_CLUSTER_NUM,                                                // 9
        PAL_CPPI41_SR_DOCSIS_TX_BE10_QOS_CLUSTER_NUM,                                               // 10
        PAL_CPPI41_SR_DOCSIS_TX_BE11_QOS_CLUSTER_NUM,                                               // 11
        PAL_CPPI41_SR_DOCSIS_TX_BE12_QOS_CLUSTER_NUM,                                               // 12
        PAL_CPPI41_SR_DOCSIS_TX_BE13_QOS_CLUSTER_NUM,                                               // 13
        PAL_CPPI41_SR_DOCSIS_TX_BE14_QOS_CLUSTER_NUM,                                               // 14
        PAL_CPPI41_SR_DOCSIS_TX_BE15_QOS_CLUSTER_NUM,                                               // 15
    PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_LAST = PAL_CPPI41_SR_DOCSIS_TX_BE15_QOS_CLUSTER_NUM,

    PAL_CPPI41_SR_L2SW_QOS_CLUSTER_BASE = 16,
        PAL_CPPI41_SR_L2SW_DATA0_QOS_CLUSTER_NUM = PAL_CPPI41_SR_L2SW_QOS_CLUSTER_BASE,             // 16
        PAL_CPPI41_SR_L2SW_MGMT0_QOS_CLUSTER_NUM,                                                   // 17
        PAL_CPPI41_SR_L2SW_MoCA_VIRT_QOS_CLUSTER_NUM,                                               // 18
    PAL_CPPI41_SR_L2SW_QOS_CLUSTER_LAST = PAL_CPPI41_SR_L2SW_MoCA_VIRT_QOS_CLUSTER_NUM,

    PAL_CPPI41_SR_QOS_CLUSTERS_COUNT,

    PAL_CPPI41_SR_QOS_CLUSTERS_MAX = 32

}PAL_CPPI41_SR_QOS_CLUSTERS_e;

#define PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_COUNT       (PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_LAST - PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_BASE + 1)
#define PAL_CPPI41_SR_L2SW_QOS_CLUSTER_COUNT            (PAL_CPPI41_SR_L2SW_QOS_CLUSTER_LAST - PAL_CPPI41_SR_L2SW_QOS_CLUSTER_BASE + 1)

extern int avalanche_pp_kernel_post_init (void);

#endif /* !_INCLUDE_PUMA6_PP_H */
