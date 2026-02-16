/*
 
  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.
 
  GPL LICENSE SUMMARY
 
  Copyright(c) 2012-2013 Intel Corporation.
 
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
 
  Copyright(c) 2012-2013 Intel Corporation. All rights reserved.
 
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

#ifndef _PUMA5_CPPI_PRV_H_
#define _PUMA5_CPPI_PRV_H_
/**
 * Queue manager configurations
 */
#define PAL_CPPI41_NUM_QUEUE_MGR                2    /**< Number of queue managers in CPPI system */
#define PAL_CPPI41_MAX_DESC_REGIONS            16    /**< Maximum descriptor regions per queue manager */

#define PAL_CPPI41_QUEUE_MGR0                   0    /* To be removed (Mark) */
#define PAL_CPPI41_QUEUE_MGR_PARTITION_SR       0
#define PAL_CPPI41_QUEUE_MGR_PARTITION_DOCSIS   1


/*
+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
|S|R|  |Q|u|e|u|e| |M|a|n|a|g|e|r|
+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
*/
typedef enum PAL_CPPI41_SR_QMGR_QUEUES 
{ 
//SYNCH
PAL_CPPI41_SR_PP_SYNCH_Q_BASE = 0,
        PAL_CPPI41_SR_PP_SYNCH_Q_0 = PAL_CPPI41_SR_PP_SYNCH_Q_BASE, /* 0 */
        PAL_CPPI41_SR_PP_SYNCH_Q_1,                                 /* 1 */
        PAL_CPPI41_SR_PP_SYNCH_Q_2,                                 /* 2 */
        PAL_CPPI41_SR_PP_SYNCH_Q_3,                                 /* 3 */
        PAL_CPPI41_SR_PP_SYNCH_Q_4,                                 /* 4 */
        PAL_CPPI41_SR_PP_SYNCH_Q_5,                                 /* 5 */
        PAL_CPPI41_SR_PP_SYNCH_Q_6,                                 /* 6 */
        PAL_CPPI41_SR_PP_SYNCH_Q_7,                                 /* 7 */
        PAL_CPPI41_SR_PP_SYNCH_Q_8,                                 /* 8 */
        PAL_CPPI41_SR_PP_SYNCH_Q_9,                                 /* 9 */
        PAL_CPPI41_SR_PP_SYNCH_Q_10,                                /* 10 */
        PAL_CPPI41_SR_PP_SYNCH_Q_11,                                /* 11 */
        PAL_CPPI41_SR_PP_SYNCH_Q_12,                                /* 12 */
        PAL_CPPI41_SR_PP_SYNCH_Q_13,                                /* 13 */
        PAL_CPPI41_SR_PP_SYNCH_Q_14,                                /* 14 */
        PAL_CPPI41_SR_PP_SYNCH_Q_15,                                /* 15 */
        PAL_CPPI41_SR_PP_SYNCH_Q_16,                                /* 16 */
        PAL_CPPI41_SR_PP_SYNCH_Q_17,                                /* 17 */
        PAL_CPPI41_SR_PP_SYNCH_Q_18,                                /* 18 */
        PAL_CPPI41_SR_PP_SYNCH_Q_19,                                /* 19 */
        PAL_CPPI41_SR_PP_SYNCH_Q_20,                                /* 20 */
        PAL_CPPI41_SR_PP_SYNCH_Q_21,                                /* 21 */
        PAL_CPPI41_SR_PP_SYNCH_Q_22,                                /* 22 */
        PAL_CPPI41_SR_PP_SYNCH_Q_23,                                /* 23 */
        PAL_CPPI41_SR_PP_SYNCH_Q_24,                                /* 24 */
        PAL_CPPI41_SR_PP_SYNCH_Q_25,                                /* 25 */
        PAL_CPPI41_SR_PP_SYNCH_Q_26,                                /* 26 */
        PAL_CPPI41_SR_PP_SYNCH_Q_27,                                /* 27 */
        PAL_CPPI41_SR_PP_SYNCH_Q_28,                                /* 28 */
        PAL_CPPI41_SR_PP_SYNCH_Q_29,                                /* 29 */
        PAL_CPPI41_SR_PP_SYNCH_Q_30,                                /* 30 */
        PAL_CPPI41_SR_PP_SYNCH_Q_31,                                /* 31 */
PAL_CPPI41_SR_PP_SYNCH_Q_LAST = PAL_CPPI41_SR_PP_SYNCH_Q_31,

//CPDSP
PAL_CPPI41_CPDSP_FW_Q_BASE   =  32,
        PAL_CPPI41_SR_CPDSP_FW_Q_0 = PAL_CPPI41_CPDSP_FW_Q_BASE,             // 32
        PAL_CPPI41_SR_CPDSP_FW_Q_1,                                          // 33
        PAL_CPPI41_SR_CPDSP_FW_Q_2,                                          // 34
        PAL_CPPI41_SR_CPDSP_FW_Q_3,                                          // 35
        PAL_CPPI41_SR_CPDSP_FW_Q_4,                                          // 36
        PAL_CPPI41_SR_CPDSP_FW_Q_5,                                          // 37
        PAL_CPPI41_SR_CPDSP_FW_Q_6,                                          // 38
        PAL_CPPI41_SR_CPDSP_FW_Q_7,                                          // 39
PAL_CPPI41_CPDSP_FW_Q_LAST = PAL_CPPI41_SR_CPDSP_FW_Q_7,

//MPDSP
PAL_CPPI41_MPDSP_FW_Q_BASE = 40 ,
        PAL_CPPI41_SR_MPDSP_FW_Q_0 = PAL_CPPI41_MPDSP_FW_Q_BASE,             // 40
        PAL_CPPI41_SR_MPDSP_FW_Q_1,                                          // 41
        PAL_CPPI41_SR_MPDSP_FW_Q_2,                                          // 42
        PAL_CPPI41_SR_MPDSP_FW_Q_3,                                          // 43
        PAL_CPPI41_SR_MPDSP_FW_Q_4,                                          // 44
        PAL_CPPI41_SR_MPDSP_FW_Q_5,                                          // 45
        PAL_CPPI41_SR_MPDSP_FW_Q_6,                                          // 46
        PAL_CPPI41_SR_MPDSP_FW_Q_7,                                          // 47
PAL_CPPI41_MPDSP_FW_Q_LAST = PAL_CPPI41_SR_MPDSP_FW_Q_6,

//QPDSP 
PAL_CPPI41_QPDSP_IP_Q_BASE = 48, 
        PAL_CPPI41_QPDSP_IP_Q_0 = PAL_CPPI41_QPDSP_IP_Q_BASE,            //48
        PAL_CPPI41_SR_HOST_TO_QPDSP_LOW_EMB_TYPE_Q_NUM,                  //49
        PAL_CPPI41_QPDSP_IP_Q_2,                                         //50
        PAL_CPPI41_SR_HOST_TO_QPDSP_HIGH_EMB_TYPE_Q_NUM,                 //51
        PAL_CPPI41_QPDSP_IP_Q_4,                                         //52
        PAL_CPPI41_QPDSP_IP_Q_5,                                         //53
        PAL_CPPI41_QPDSP_IP_Q_6,                                         //54
        PAL_CPPI41_QPDSP_IP_Q_7,                                         //55
PAL_CPPI41_QPDSP_IP_Q_LAST    = PAL_CPPI41_QPDSP_IP_Q_7,                    

//Prefetcher PDSP Input Queues
PAL_CPPI41_PPDSP_Q_BASE = 56,                                        
        PAL_CPPI41_SR_PPDSP_LOW_Q_NUM = PAL_CPPI41_PPDSP_Q_BASE,         //56
        PAL_CPPI41_SR_PPDSP_MED_Q_NUM,                                   //57
        PAL_CPPI41_SR_PPDSP_HIGH_Q_NUM,                                  //58  
        PAL_CPPI41_SR_PPDSP_Q_3,                                         //59
        PAL_CPPI41_SR_PPDSP_Q_4,                                         //60
        PAL_CPPI41_SR_PPDSP_Q_5,                                         //61
        PAL_CPPI41_SR_PPDSP_Q_6,                                         //62
        PPFW_CPPI4x_APDSP_PREBUF_QNUM,                                   // 63 /* descriptors to be used as pre-fetch “buffers”*/ 
PAL_CPPI41_PPDSP_IP_Q_LAST = PPFW_CPPI4x_APDSP_PREBUF_QNUM,


//QoS 
PAL_CPPI41_SR_QPDSP_QOS_Q_BASE = 64,    
        PAL_CPPI41_SR_CLUSTER0_DOCSIS_TX_BE0_HIGH_QPDSP_QOS_Q_NUM = PAL_CPPI41_SR_QPDSP_QOS_Q_BASE,      //64
        PAL_CPPI41_SR_CLUSTER0_DOCSIS_TX_BE0_LOW_QPDSP_QOS_Q_NUM,                                        //65
        PAL_CPPI41_SR_CLUSTER1_DOCSIS_TX_BE1_HIGH_QPDSP_QOS_Q_NUM,                                       //66   
        PAL_CPPI41_SR_CLUSTER1_DOCSIS_TX_BE1_LOW_QPDSP_QOS_Q_NUM,                                        //67
        PAL_CPPI41_SR_CLUSTER2_DOCSIS_TX_BE2_HIGH_QPDSP_QOS_Q_NUM,                                       //68
        PAL_CPPI41_SR_CLUSTER2_DOCSIS_TX_BE2_LOW_QPDSP_QOS_Q_NUM,                                        //69
        PAL_CPPI41_SR_CLUSTER3_DOCSIS_TX_BE3_HIGH_QPDSP_QOS_Q_NUM,                                       //70
        PAL_CPPI41_SR_CLUSTER3_DOCSIS_TX_BE3_LOW_QPDSP_QOS_Q_NUM,                                        //71
        PAL_CPPI41_SR_CLUSTER4_DOCSIS_TX_BE4_HIGH_QPDSP_QOS_Q_NUM,                                       //72 
        PAL_CPPI41_SR_CLUSTER4_DOCSIS_TX_BE4_LOW_QPDSP_QOS_Q_NUM,                                        //73
        PAL_CPPI41_SR_CLUSTER5_DOCSIS_TX_BE5_HIGH_QPDSP_QOS_Q_NUM,                                       //74
        PAL_CPPI41_SR_CLUSTER5_DOCSIS_TX_BE5_LOW_QPDSP_QOS_Q_NUM,                                        //75 
        PAL_CPPI41_SR_CLUSTER6_DOCSIS_TX_BE6_HIGH_QPDSP_QOS_Q_NUM,                                       //76
        PAL_CPPI41_SR_CLUSTER6_DOCSIS_TX_BE6_LOW_QPDSP_QOS_Q_NUM,                                        //77
        PAL_CPPI41_SR_CLUSTER7_DOCSIS_TX_BE7_HIGH_QPDSP_QOS_Q_NUM,                                       //78
        PAL_CPPI41_SR_CLUSTER7_DOCSIS_TX_BE7_LOW_QPDSP_QOS_Q_NUM,                                        //79
        CPMAC_CPPI4x_QoS_HIGH_TX_QNUM,                                    //80
        CPMAC_CPPI4x_QoS_HIGH_MED_TX_QNUM,                                //81
        CPMAC_CPPI4x_QoS_LOW_MED_TX_QNUM,                                 //82
        CPMAC_CPPI4x_QoS_LOW_TX_QNUM,                                     //83
        PAL_CPPI41_SR_QPDSP_QOS_Q_20,                                     //84
        PAL_CPPI41_SR_QPDSP_QOS_Q_21,                                     //85
        PAL_CPPI41_SR_QPDSP_QOS_Q_22,                                     //86
        PAL_CPPI41_SR_QPDSP_QOS_Q_23,                                     //87
        PAL_CPPI41_SR_QPDSP_QOS_Q_24,                                     //88
        PAL_CPPI41_SR_QPDSP_QOS_Q_25,                                     //89
        PAL_CPPI41_SR_QPDSP_QOS_Q_26,                                     //90
        PAL_CPPI41_SR_QPDSP_QOS_Q_27,                                     //91
        PAL_CPPI41_SR_QPDSP_QOS_Q_28,                                     //92
        PAL_CPPI41_SR_QPDSP_QOS_Q_29,                                     //93
        PAL_CPPI41_SR_QPDSP_QOS_Q_30,                                     //94
        PAL_CPPI41_SR_QPDSP_QOS_Q_31,                                     //95
PAL_CPPI41_SR_QPDSP_QOS_Q_LAST =  PAL_CPPI41_SR_QPDSP_QOS_Q_31,

        //UNASSIGNED Q96-Q99
        PAL_CPPI41_SR_UNASSIGNED_Q_96,                                           //96
        PAL_CPPI41_SR_UNASSIGNED_Q_97,                                           //97
        PAL_CPPI41_SR_UNASSIGNED_Q_98,                                           //98
        PAL_CPPI41_SR_UNASSIGNED_Q_99,                                           //99

PAL_CPPI41_RX_Q_BASE  = 100,
        PAL_CPPI41_CPMAC_RX_QNUM = PAL_CPPI41_RX_Q_BASE,                //100
        USB_CPPI41_RX1_QNUM,                                            //101
        USB_CPPI41_RX2_QNUM,                                            //102
        USB_CPPI41_RX3_QNUM,                                            //103
        USB_CPPI41_RX4_QNUM,                                            //104
        PAL_CPPI41_CNI_RXCMPL_LOW ,                                     //105
        PAL_CPPI41_CNI_RXCMPL_MED ,                                     //106
        PAL_CPPI41_RX_Q7,                                               //107
        PAL_CPPI41_SR_DOCSIS_MGMT_HOST_RX_Q_NUM ,                       //108
        PAL_CPPI41_SR_VOICE_DSP_C55_HOST_RX_Q_NUM   ,                   //109
        PAL_CPPI41_SR_MPEG_HOST_RX_Q_NUM    ,                           //110
        PAL_CPPI41_RX_Q11   ,                                           //111
        PAL_CPPI41_RX_Q12   ,                                           //112
        PAL_CPPI41_RX_Q13   ,                                           //113
        PAL_CPPI41_RX_Q14   ,                                           //114
        PAL_CPPI41_RX_Q15   ,                                           //115
PAL_CPPI41_RX_Q_LAST = PAL_CPPI41_RX_Q15,                         

        //PP --> Host Events Queues
        PP_HOST_EVENTQ,                                                      //116
        PAL_CPPI41_SR_PP_EVENTS_FD_Q_NUM,                                     //117 //SR event free descriptor queue
        PAL_CPPI41_SR_UNASSIGNED_Q_118,                                       //118
        PAL_CPPI41_SR_UNASSIGNED_Q_119,                                       //119
                    

        //Pre-fetcher free internal descriptors
        PPFW_CPPI4x_APDSP_FD_QNUM  = 120,                                           //120 
        //Replicated packet free internal descriptors                       
        PPFW_CPPI4x_MPDSP_FD_QNUM  = 121,                                           //121

PAL_CPPI41_TX_CMPL_Q_BASE = 122,
        PAL_CPPI41_CPMAC_TX_COMP_QNUM = PAL_CPPI41_TX_CMPL_Q_BASE,              //122
        USB_CPPI41_TXCMPL_QNUM,                                                 //123
        PAL_CPPI41_SR_HOST_TX_COMPLETE_LOW_Q_NUM,                               //124   
        PAL_CPPI41_TX_CMPL_Q3,                                                  //125
        PAL_CPPI41_TX_CMPL_Q4,                                                  //126
        PAL_CPPI41_TX_CMPL_Q5,                                                  //127
PAL_CPPI41_TX_CMPL_Q_LAST= PAL_CPPI41_TX_CMPL_Q5,                       

PAL_CPPI41_FBD_Q_BASE = 128,
        PAL_CPPI41_CNI_RX_INFRA_FD_HOST_LOW = PAL_CPPI41_FBD_Q_BASE,             //128
        PAL_CPPI41_CNI_RX_INFRA_FD_HOST_MED,                                    //129
        PAL_CPPI41_CNI_RX_INFRA_FD_HOST_HIGH,                                   //130
        PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_Q_NUM  ,                           //131  
        PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_Q_NUM  ,                           //132   
        PAL_CPPI41_CPMAC_FBD_QNUM   ,                                           //133
        USB_CPPI41_RX1_FDB_QNUM ,                                               //134
        USB_CPPI41_RX2_FDB_QNUM ,                                               //135
        USB_CPPI41_RX3_FDB_QNUM ,                                               //136
        USB_CPPI41_RX4_FDB_QNUM ,                                               //137
        PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_Q_NUM  ,                           //138
        PAL_CPPI41_SR_HOST_TO_PP_MED_FD_HOST_Q_NUM  ,                           //139
        PAL_CPPI41_SR_HOST_TO_PP_HIGH_FD_HOST_Q_NUM ,                           //140
        RECYCLE_CPPI4x_FBD_QNUM ,                                               //141
        PAL_CPPI41_FBD_Q14  ,                                                   //142
        PAL_CPPI41_FBD_Q15  ,                                                   //143
PAL_CPPI41_FBD_Q_LAST  =     PAL_CPPI41_FBD_Q15,          


PAL_CPPI41_FD_Q_BASE= 144,
        PAL_CPPI4_CPMAC_FD_QNUM =PAL_CPPI41_FD_Q_BASE,                  //144
        USB_CPPI4x_EP0_FD_QNUM,                                         //145
        USB_CPPI4x_EP1_FD_QNUM,                                         //146
        USB_CPPI4x_EP2_FD_QNUM,                                         //147
        USB_CPPI4x_EP3_FD_QNUM,                                         //148
        DMA0_CPPI4x_FTD_QNUM    ,                                       //149  // DMA0 FD_TEARDOWN
        DMA1_CPPI4x_FTD_QNUM    ,                                       //150  // DMA1 FD_TEARDOWN
        PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM,                   //151 
        PAL_CPPI41_SR_DOCSIS_RX_CoP_MED_FD_EMB_Q_NUM,                   //152
        PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_Q_NUM    ,              //153
        PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_Q_NUM ,               //154
        PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_Q_NUM    ,           //155
        PAL_CPPI41_SR_VOICE_DSP_C55_FD_EMB_Q_NUM    ,                   //156
        PAL_CPPI41_SR_MPEG_FD_EMB_Q_NUM ,                               //157
        PAL_CPPI41_SR_MPEG_ENCAP_FD_EMB_Q_NUM   ,                       //158
        PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_NUM   ,                      //159
PAL_CPPI41_FD_Q_LAST=PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_NUM,                         

        //UNASSIGNED Q160 -Q179
        PAL_CPPI41_SR_UNASSIGNED_Q_160	,								 //160
        PAL_CPPI41_SR_UNASSIGNED_Q_161	,								 //161
        PAL_CPPI41_SR_UNASSIGNED_Q_162	,								 //162
        PAL_CPPI41_SR_UNASSIGNED_Q_163	,								 //163
        PAL_CPPI41_SR_UNASSIGNED_Q_164	,								 //164
        PAL_CPPI41_SR_UNASSIGNED_Q_165	,								 //165
        PAL_CPPI41_SR_UNASSIGNED_Q_166	,								 //166
        PAL_CPPI41_SR_UNASSIGNED_Q_167	,								 //167
        PAL_CPPI41_SR_UNASSIGNED_Q_168	,								 //168
        PAL_CPPI41_SR_UNASSIGNED_Q_169	,								 //169
        PAL_CPPI41_SR_UNASSIGNED_Q_170	,								 //170
        PAL_CPPI41_SR_UNASSIGNED_Q_171	,								 //171
        PAL_CPPI41_SR_UNASSIGNED_Q_172	,								 //172
        PAL_CPPI41_SR_UNASSIGNED_Q_173	,								 //173
        PAL_CPPI41_SR_UNASSIGNED_Q_174	,								 //174
        PAL_CPPI41_SR_UNASSIGNED_Q_175	,								 //175
        PAL_CPPI41_SR_UNASSIGNED_Q_176	,								 //176
        PAL_CPPI41_SR_UNASSIGNED_Q_177	,								 //177
        PAL_CPPI41_SR_UNASSIGNED_Q_178	,								 //178
        PAL_CPPI41_SR_UNASSIGNED_Q_179	,								 //179
                  

PAL_CPPI41_DOCSIS_TX_Q_BASE    = 180,
        PAL_CPPI41_SR_DOCSIS_TX_MGMT_Q_NUM  = PAL_CPPI41_DOCSIS_TX_Q_BASE,  //180
        PAL_CPPI41_SR_DOCSIS_TX_VOICE_Q_NUM ,                               //181
        PAL_CPPI41_SR_DOCSIS_TX_BE0_Q_NUM ,                                           //182
        PAL_CPPI41_SR_DOCSIS_TX_BE1_Q_NUM ,                                           //183
        PAL_CPPI41_SR_DOCSIS_TX_BE2_Q_NUM ,                                           //184
        PAL_CPPI41_SR_DOCSIS_TX_BE3_Q_NUM ,                                           //185
        PAL_CPPI41_SR_DOCSIS_TX_BE4_Q_NUM ,                                           //186
        PAL_CPPI41_SR_DOCSIS_TX_BE5_Q_NUM ,                                           //187
        PAL_CPPI41_SR_DOCSIS_TX_BE6_Q_NUM ,                                           //188
        PAL_CPPI41_SR_DOCSIS_TX_BE7_Q_NUM ,                                           //189
PAL_CPPI41_DOCSIS_TX_Q_LAST = PAL_CPPI41_SR_DOCSIS_TX_BE7_Q_NUM,            


        //UNASSIGNED Q190 -Q195
        PAL_CPPI41_SR_UNASSIGNED_Q_190	,								 //190
        PAL_CPPI41_SR_UNASSIGNED_Q_191	,								 //191
        PAL_CPPI41_SR_UNASSIGNED_Q_192	,								 //192
        PAL_CPPI41_SR_UNASSIGNED_Q_193	,								 //193
        PAL_CPPI41_SR_UNASSIGNED_Q_194	,								 //194
        PAL_CPPI41_SR_UNASSIGNED_Q_195	,								 //195

//host--> pp Infra
PAL_CPPI41_HOST2SR_TX_Q_BASE      =              196,
        PAL_CPPI4x_HOST2SR_TX_Q0 = PAL_CPPI41_HOST2SR_TX_Q_BASE,                        //196
        PAL_CPPI4x_HOST2SR_TX_Q1,                                                       //197
        PAL_CPPI4x_HOST2SR_TX_Q2,                                                       //198
        PAL_CPPI4x_HOST2SR_TX_Q3,                                                       //199
        PAL_CPPI4x_HOST2SR_TX_Q4,                                                       //200                                                                                   
        PAL_CPPI4x_HOST2SR_TX_Q5,                                                       //201   
        PAL_CPPI4x_HOST2SR_TX_Q6,                                                       //202                                                                                   
        PAL_CPPI4x_HOST2SR_TX_Q7,                                                       //203                                                                                                                                                                   
PAL_CPPI41_HOST2SR_TX_Q_LAST=   PAL_CPPI4x_HOST2SR_TX_Q7,                                           

/* Total 8 queues 4 used */
PAL_CPPI41_USB_TX_Q_BASE  =  204,
        USB_CPPI41_TX1_QNUM = PAL_CPPI41_USB_TX_Q_BASE,                     //204
        PAL_CPPI4x_USB_UNUSED_Q1,                                           //205
        USB_CPPI41_TX2_QNUM,                                                //206
        PAL_CPPI4x_USB_UNUSED_Q3,                                           //207
        USB_CPPI41_TX3_QNUM,                                                //208                                                                                   
        PAL_CPPI4x_USB_UNUSED_Q5,                                           //209   
        USB_CPPI41_TX4_QNUM,                                                //210                                                                                   
        PAL_CPPI4x_USB_UNUSED_Q7,                                           //211                                                                                                                                                                   
PAL_CPPI41_USB_TX_Q_LAST = PAL_CPPI4x_USB_UNUSED_Q7,



// CPMAC (eth tx)
PAL_CPPI41_MAC_TX_Q_BASE     =       212,
        PAL_CPPI41_CPMAC_TX_Q0 = PAL_CPPI41_MAC_TX_Q_BASE,            // 212
        PAL_CPPI41_CPMAC_TX_Q1,                                       // 213
PAL_CPPI41_MAC_TX_Q_LAST   =PAL_CPPI41_CPMAC_TX_Q1,

        //UNASSIGNED Q214
        PAL_CPPI41_SR_UNASSIGNED_Q_214,								            //214
PAL_CPPI41_VOICE_DSP_C55_INPUT_QNUM=215,								        //215

PAL_CPPI4x_MPEG_TX_Q_BASE = 216,
        PAL_CPPI4x_MPEG_TX_Q_NUM= PAL_CPPI4x_MPEG_TX_Q_BASE,


        //UNASSIGNED Q217
        PAL_CPPI41_SR_UNASSIGNED_Q_217,								                        //217

PAL_CPPI4x_MPEG_OUT_Q_BASE    =      218,
        PAL_CPPI4x_MPEG_OUT_Q_0 = PAL_CPPI4x_MPEG_OUT_Q_BASE,                           // 218
        PAL_CPPI4x_MPEG_OUT_Q_1,                                                        // 219
        PAL_CPPI4x_MPEG_OUT_Q_2,                                                        // 220
        PAL_CPPI4x_MPEG_OUT_Q_3,                                                        // 221
PAL_CPPI4x_MPEG_OUT_Q_LAST = PAL_CPPI4x_MPEG_OUT_Q_3,

PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_BASE  = 222,
        PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_0 = PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_BASE,   // 222
        PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_1,                                            // 223
        PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_2,                                            // 224
PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_LAST =  PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_2, 


        //UNASSIGNED Q225
        PAL_CPPI41_SR_UNASSIGNED_Q_225,								                    //225

//PP-->host usb 
PAL_CPPI41_USB_TO_HOST_PRXY_BASE = 226,
        PAL_CPPI41_USB_TO_HOST_PRXY_Q_0 = PAL_CPPI41_USB_TO_HOST_PRXY_BASE,             //226
        PAL_CPPI41_USB_TO_HOST_PRXY_Q_1,                                                //227
        PAL_CPPI41_USB_TO_HOST_PRXY_Q_2,                                                //228
        PAL_CPPI41_USB_TO_HOST_PRXY_Q_3,                                                //229
PAL_CPPI41_USB_TO_HOST_PRXY_LAST = PAL_CPPI41_USB_TO_HOST_PRXY_Q_3,

//cpmac
PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_BASE     =       230,
        PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_Q0 = PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_BASE,  //230
        PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_Q1,                                           //231
PAL_CPPI41_ETH_TO_HOST_PRXY_LAST   =PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_Q1,

PAL_CPPI41_RECYCLE_INFRA_INPUT_LOW_Q_NUM =    232,                                      //232
        //UNASSIGNED Q233
        PAL_CPPI41_SR_UNASSIGNED_Q_233,								                    //233
PAL_CPPI41_VOICE_DSP_C55_INFRA_INPUT_LOW_Q_NUM=234,                                     //234

//UNASSIGNED Q233 -Q367
        PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT = 368  //  /**< Maximum number of queues per queue manager */ 
}PAL_CPPI41_SR_QMGR_QUEUES_e;


#define PAL_CPPI41_SR_QMGR_QUEUES_STR(v)    \
char * v[ PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT ] =   \
{\
"PAL_CPPI41_SR_PP_SYNCH_Q_0",\
"PAL_CPPI41_SR_PP_SYNCH_Q_1",\
"PAL_CPPI41_SR_PP_SYNCH_Q_2",\
"PAL_CPPI41_SR_PP_SYNCH_Q_3",\
"PAL_CPPI41_SR_PP_SYNCH_Q_4",\
"PAL_CPPI41_SR_PP_SYNCH_Q_5",\
"PAL_CPPI41_SR_PP_SYNCH_Q_6",\
"PAL_CPPI41_SR_PP_SYNCH_Q_7",\
"PAL_CPPI41_SR_PP_SYNCH_Q_8",\
"PAL_CPPI41_SR_PP_SYNCH_Q_9",\
"PAL_CPPI41_SR_PP_SYNCH_Q_10",\
"PAL_CPPI41_SR_PP_SYNCH_Q_11",\
"PAL_CPPI41_SR_PP_SYNCH_Q_12",\
"PAL_CPPI41_SR_PP_SYNCH_Q_13",\
"PAL_CPPI41_SR_PP_SYNCH_Q_14",\
"PAL_CPPI41_SR_PP_SYNCH_Q_15",\
"PAL_CPPI41_SR_PP_SYNCH_Q_16",\
"PAL_CPPI41_SR_PP_SYNCH_Q_17",\
"PAL_CPPI41_SR_PP_SYNCH_Q_18",\
"PAL_CPPI41_SR_PP_SYNCH_Q_19",\
"PAL_CPPI41_SR_PP_SYNCH_Q_20",\
"PAL_CPPI41_SR_PP_SYNCH_Q_21",\
"PAL_CPPI41_SR_PP_SYNCH_Q_22",\
"PAL_CPPI41_SR_PP_SYNCH_Q_23",\
"PAL_CPPI41_SR_PP_SYNCH_Q_24",\
"PAL_CPPI41_SR_PP_SYNCH_Q_25",\
"PAL_CPPI41_SR_PP_SYNCH_Q_26",\
"PAL_CPPI41_SR_PP_SYNCH_Q_27",\
"PAL_CPPI41_SR_PP_SYNCH_Q_28",\
"PAL_CPPI41_SR_PP_SYNCH_Q_29",\
"PAL_CPPI41_SR_PP_SYNCH_Q_30",\
"PAL_CPPI41_SR_PP_SYNCH_Q_31",\
"PAL_CPPI41_SR_CPDSP_FW_Q_0",\
"PAL_CPPI41_SR_CPDSP_FW_Q_1",\
"PAL_CPPI41_SR_CPDSP_FW_Q_2",\
"PAL_CPPI41_SR_CPDSP_FW_Q_3",\
"PAL_CPPI41_SR_CPDSP_FW_Q_4",\
"PAL_CPPI41_SR_CPDSP_FW_Q_5",\
"PAL_CPPI41_SR_CPDSP_FW_Q_6",\
"PAL_CPPI41_SR_CPDSP_FW_Q_7",\
"PAL_CPPI41_SR_MPDSP_FW_Q_0",\
"PAL_CPPI41_SR_MPDSP_FW_Q_1",\
"PAL_CPPI41_SR_MPDSP_FW_Q_2",\
"PAL_CPPI41_SR_MPDSP_FW_Q_3",\
"PAL_CPPI41_SR_MPDSP_FW_Q_4",\
"PAL_CPPI41_SR_MPDSP_FW_Q_5",\
"PAL_CPPI41_SR_MPDSP_FW_Q_6",\
"PAL_CPPI41_SR_MPDSP_FW_Q_7",\
"PAL_CPPI41_QPDSP_IP_Q_0",\
"PAL_CPPI41_SR_HOST_TO_QPDSP_LOW_EMB_TYPE_Q_NUM",\
"PAL_CPPI41_QPDSP_IP_Q_2",\
"PAL_CPPI41_SR_HOST_TO_QPDSP_HIGH_EMB_TYPE_Q_NUM",\
"PAL_CPPI41_QPDSP_IP_Q_4",\
"PAL_CPPI41_QPDSP_IP_Q_5",\
"PAL_CPPI41_QPDSP_IP_Q_6",\
"PAL_CPPI41_QPDSP_IP_Q_7",\
"PAL_CPPI41_SR_PPDSP_LOW_Q_NUM",\
"PAL_CPPI41_SR_PPDSP_MED_Q_NUM",\
"PAL_CPPI41_SR_PPDSP_HIGH_Q_NUM",\
"PAL_CPPI41_SR_PPDSP_Q_3",\
"PAL_CPPI41_SR_PPDSP_Q_4",\
"PAL_CPPI41_SR_PPDSP_Q_5",\
"PAL_CPPI41_SR_PPDSP_Q_6",\
"PPFW_CPPI4x_APDSP_PREBUF_QNUM",\
"PAL_CPPI41_SR_CLUSTER0_DOCSIS_TX_BE0_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER0_DOCSIS_TX_BE0_LOW_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER1_DOCSIS_TX_BE1_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER1_DOCSIS_TX_BE1_LOW_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER2_DOCSIS_TX_BE2_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER2_DOCSIS_TX_BE2_LOW_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER3_DOCSIS_TX_BE3_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER3_DOCSIS_TX_BE3_LOW_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER4_DOCSIS_TX_BE4_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER4_DOCSIS_TX_BE4_LOW_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER5_DOCSIS_TX_BE5_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER5_DOCSIS_TX_BE5_LOW_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER6_DOCSIS_TX_BE6_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER6_DOCSIS_TX_BE6_LOW_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER7_DOCSIS_TX_BE7_HIGH_QPDSP_QOS_Q_NUM",\
"PAL_CPPI41_SR_CLUSTER7_DOCSIS_TX_BE7_LOW_QPDSP_QOS_Q_NUM",\
"CPMAC_CPPI4x_QoS_HIGH_TX_QNUM",\
"CPMAC_CPPI4x_QoS_HIGH_MED_TX_QNUM",\
"CPMAC_CPPI4x_QoS_LOW_MED_TX_QNUM",\
"CPMAC_CPPI4x_QoS_LOW_TX_QNUM",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_20",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_21",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_22",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_23",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_24",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_25",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_26",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_27",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_28",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_29",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_30",\
"PAL_CPPI41_SR_QPDSP_QOS_Q_31",\
"PAL_CPPI41_SR_UNASSIGNED_Q_96",\
"PAL_CPPI41_SR_UNASSIGNED_Q_97",\
"PAL_CPPI41_SR_UNASSIGNED_Q_98",\
"PAL_CPPI41_SR_UNASSIGNED_Q_99",\
"PAL_CPPI41_CPMAC_RX_QNUM",\
"USB_CPPI41_RX1_QNUM",\
"USB_CPPI41_RX2_QNUM",\
"USB_CPPI41_RX3_QNUM",\
"USB_CPPI41_RX4_QNUM",\
"PAL_CPPI41_CNI_RXCMPL_LOW",\
"PAL_CPPI41_CNI_RXCMPL_MED",\
"PAL_CPPI41_RX_Q7",\
"PAL_CPPI41_SR_DOCSIS_MGMT_HOST_RX_Q_NUM",\
"PAL_CPPI41_SR_VOICE_DSP_C55_HOST_RX_Q_NUM",\
"PAL_CPPI41_SR_MPEG_HOST_RX_Q_NUM",\
"PAL_CPPI41_RX_Q11",\
"PAL_CPPI41_RX_Q12",\
"PAL_CPPI41_RX_Q13",\
"PAL_CPPI41_RX_Q14",\
"PAL_CPPI41_RX_Q15",\
"PP_HOST_EVENTQ",\
"PAL_CPPI41_SR_PP_EVENTS_FD_Q_NUM",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_118",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_119",\
"PPFW_CPPI4x_APDSP_FD_QNUM",\
"PPFW_CPPI4x_MPDSP_FD_QNUM",\
"PAL_CPPI41_CPMAC_TX_COMP_QNUM ",/*122*/ \
"USB_CPPI41_TXCMPL_QNUM",/*123*/\
"PAL_CPPI41_SR_HOST_TX_COMPLETE_LOW_Q_NUM",/*124*/\
"PAL_CPPI41_TX_CMPL_Q3",/*125*/\
"PAL_CPPI41_TX_CMPL_Q4",/*126*/\
"PAL_CPPI41_TX_CMPL_Q5",/*127*/\
"PAL_CPPI41_CNI_RX_INFRA_FD_HOST_LOW",/*128*/\
"PAL_CPPI41_CNI_RX_INFRA_FD_HOST_MED",/*129*/\
"PAL_CPPI41_CNI_RX_INFRA_FD_HOST_HIGH",\
"PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_Q_NUM ",\
"PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_Q_NUM ",\
"PAL_CPPI41_CPMAC_FBD_QNUM  ",\
"USB_CPPI41_RX1_FDB_QNUM",\
"USB_CPPI41_RX2_FDB_QNUM",\
"USB_CPPI41_RX3_FDB_QNUM",\
"USB_CPPI41_RX4_FDB_QNUM",\
"PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_Q_NUM",\
"PAL_CPPI41_SR_HOST_TO_PP_MED_FD_HOST_Q_NUM",\
"PAL_CPPI41_SR_HOST_TO_PP_HIGH_FD_HOST_Q_NUM",\
"RECYCLE_CPPI4x_FBD_QNUM",\
"PAL_CPPI41_FBD_Q14",\
"PAL_CPPI41_FBD_Q15",\
"PAL_CPPI4_CPMAC_FD_QNUM",\
"USB_CPPI4x_EP0_FD_QNUM",\
"USB_CPPI4x_EP1_FD_QNUM",\
"USB_CPPI4x_EP2_FD_QNUM",\
"USB_CPPI4x_EP3_FD_QNUM",\
"DMA0_CPPI4x_FTD_QNUM",\
"DMA1_CPPI4x_FTD_QNUM",\
"PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_RX_CoP_MED_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_RX_CoP_HIGH_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_HOST_TO_PP_INFRA_HIGH_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_VOICE_DSP_C55_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_MPEG_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_MPEG_ENCAP_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_VOICE_INFRA_FD_EMB_Q_NUM",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_160",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_161",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_162",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_163",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_164",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_165",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_166",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_167 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_168 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_169 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_170 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_171 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_172 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_173 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_174 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_175 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_176 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_177 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_178",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_179",\
"PAL_CPPI41_SR_DOCSIS_TX_MGMT_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_VOICE_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE0_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE1_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE2_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE3_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE4_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE5_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE6_Q_NUM",\
"PAL_CPPI41_SR_DOCSIS_TX_BE7_Q_NUM",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_190",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_191",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_192",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_193",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_194",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_195",\
"PAL_CPPI4x_HOST2SR_TX_Q0",\
"PAL_CPPI4x_HOST2SR_TX_Q1",\
"PAL_CPPI4x_HOST2SR_TX_Q2",\
"PAL_CPPI4x_HOST2SR_TX_Q3",\
"PAL_CPPI4x_HOST2SR_TX_Q4",\
"PAL_CPPI4x_HOST2SR_TX_Q5",\
"PAL_CPPI4x_HOST2SR_TX_Q6",\
"PAL_CPPI4x_HOST2SR_TX_Q7",\
"USB_CPPI41_TX1_QNUM",\
"PAL_CPPI4x_USB_UNUSED_Q1",\
"USB_CPPI41_TX2_QNUM",\
"PAL_CPPI4x_USB_UNUSED_Q3",\
"USB_CPPI41_TX3_QNUM",\
"PAL_CPPI4x_USB_UNUSED_Q5",\
"USB_CPPI41_TX4_QNUM",\
"PAL_CPPI4x_USB_UNUSED_Q7",\
"PAL_CPPI41_CPMAC_TX_Q0 ",\
"PAL_CPPI41_CPMAC_TX_Q1",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_214",\
"PAL_CPPI41_VOICE_DSP_C55_INPUT_QNUM",\
"PAL_CPPI4x_MPEG_TX_Q_NUM",\
"PAL_CPPI41_SR_UNASSIGNED_Q_217",\
"PAL_CPPI4x_MPEG_OUT_Q_0 ",\
"PAL_CPPI4x_MPEG_OUT_Q_1",\
"PAL_CPPI4x_MPEG_OUT_Q_2",\
"PAL_CPPI4x_MPEG_OUT_Q_3",\
"PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_0",\
"PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_1",\
"PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_2",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_225",\
"PAL_CPPI41_USB_TO_HOST_PRXY_Q_0",\
"PAL_CPPI41_USB_TO_HOST_PRXY_Q_1",\
"PAL_CPPI41_USB_TO_HOST_PRXY_Q_2",\
"PAL_CPPI41_USB_TO_HOST_PRXY_Q_3",\
"PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_Q0",\
"PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_Q1",\
"PAL_CPPI41_RECYCLE_INFRA_INPUT_LOW_Q_NUM ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_233 ",\
"PAL_CPPI41_VOICE_DSP_C55_INFRA_INPUT_LOW_Q_NUM",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_235 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_236 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_237 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_238 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_239 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_240 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_241 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_242 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_243 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_244 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_245 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_246 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_247 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_248 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_249 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_250 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_251 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_252 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_253 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_254 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_255 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_256 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_257 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_258 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_259 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_260 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_261 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_262 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_263 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_264 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_265 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_266 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_267 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_268 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_269 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_270 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_271 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_272 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_273 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_274 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_275 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_276 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_277 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_278 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_279 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_280 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_281 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_282 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_283 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_284 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_285 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_286 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_287 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_288 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_289 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_290 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_291 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_292 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_293 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_294 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_295 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_296 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_297 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_298 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_299 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_300 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_301 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_302 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_303 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_304 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_305 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_306 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_307 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_308 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_309 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_310 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_311 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_312 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_313 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_314 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_315 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_316 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_317 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_318 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_319 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_320 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_321 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_322 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_323 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_324 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_325 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_326 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_327 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_328 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_329 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_330 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_331 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_332 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_333 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_334 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_335 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_336 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_337 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_338 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_339 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_340 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_341 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_342 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_343 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_344 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_345 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_346 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_347 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_348 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_349 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_350 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_351 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_352 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_353 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_354 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_355 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_356 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_357 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_358 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_359 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_360 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_361 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_362 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_363 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_364 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_365 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_366 ",\
"PAL_CPPI41_SR_UNASSIGNED_QNUM_367 "\
\
}


/*
+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+
|Q|u|e|u|e| |D|e|f|i|n|i|t|i|o|n|s|
+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+
*/

// CNI --------------------------------------------------------------------------------------
#define PAL_CPPI41_SR_CNI_INFRA_INPUT_Q_NUM(idx)    (PAL_CPPI4x_DOCSIS_TO_HOST_PROXY_Q_BASE + (idx))
#define CPPI4x_CNI_RXCMPL_QNUM(pri)             (PAL_CPPI41_CNI_RXCMPL_LOW + (pri))
#define PAL_CPPI41_SR_CNI_HOST_RX_Q_NUM(pri)    (PAL_CPPI41_CNI_RXCMPL_LOW + pri)

#define CPPI4x_SR_DOCSIS_RX_FD_EMB_DESC_NUM_HIGH        128
#define CPPI4x_SR_DOCSIS_RX_FD_EMB_DESC_NUM_LOW         2048

#define CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_HIGH       16
#define CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_LOW        64

#define CPPI4x_CNI_RX_INFRA_FD_HOST_BUFF_SIZE           1600
#define PAL_CPPI41_SR_CNI_INFRA_FD_HOST_BUFFER_SIZE     CPPI4x_CNI_RX_INFRA_FD_HOST_BUFF_SIZE

#define CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_SIZE           64
#define PAL_CPPI41_SR_CNI_INFRA_FD_HOST_DESC_SIZE       CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_SIZE
#define CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM            ( CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_HIGH  + \
                                                          CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_LOW  )

#define PAL_CPPI41_SR_CNI_INFRA_LOW_FD_HOST_DESC_COUNT  CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_LOW
#define PAL_CPPI41_SR_CNI_INFRA_HIGH_FD_HOST_DESC_COUNT CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM_HIGH

#define PAL_CPPI41_SR_CNI_INFRA_FD_HOST_DESC_COUNT      CPPI4x_CNI_RX_INFRA_FD_HOST_DESC_NUM
#define CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_SIZE      64
#define PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_SIZE  CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_SIZE
#define CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_NUM       64
#define PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_COUNT CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_DESC_NUM
#define CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_BUFF_SIZE      2048

#define CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_SIZE      64
#define PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_SIZE  CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_SIZE
#define CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_DESC_NUM       64
#define CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_BUFF_SIZE      2048
#define PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_BUFF_SIZE  CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_BUFF_SIZE

#define CPPI4x_CNI_RX_INFRA_FD_HOST_QNUM(pri)                (PAL_CPPI41_CNI_RX_INFRA_FD_HOST_LOW + (pri))
#define PAL_CPPI41_SR_CNI_INFRA_FD_HOST_Q_NUM(pri)           (PAL_CPPI41_CNI_RX_INFRA_FD_HOST_LOW +  pri)

#define CPPI4x_SR_DOCSIS_RX_FD_EMB_QNUM(pri)                 (PAL_CPPI41_SR_DOCSIS_RX_CoP_LOW_FD_EMB_Q_NUM + (pri))
// CNI  End --------------------------------------------------------------------------------------



// Recycling --------------------------------------------------------------------------------------
#define RECYCLE_INFRA_RX_Q(pri)                 (PAL_CPPI41_RECYCLE_INFRA_INPUT_LOW_Q_NUM)

// Recycling  End --------------------------------------------------------------------------------------


// CPMAC --------------------------------------------------------------------------------------
#define CPMAC_CPPI4x_TX_COMP_QNUM(pri)          (PAL_CPPI41_CPMAC_TX_COMP_QNUM + pri)
#define CPMAC_CPPI4x_RX_QNUM(pri)               (PAL_CPPI41_CPMAC_RX_QNUM)

#define CPMAC_CPPI4x_FBD_QNUM(pri)              (PAL_CPPI41_CPMAC_FBD_QNUM + (pri))

//#define CPMAC_CPPI4x_FD_Q_COUNT                 1
#define CPMAC_CPPI4x_FD_QNUM(pri)               (PAL_CPPI4_CPMAC_FD_QNUM + (pri))

#define CPMAC_CPPI4x_POOL_NUM(sltcnt)             (BMGR0_POOL10 + sltcnt)

#define CPMAC_CPPI4x_TX_QNUM(pri)                 (PAL_CPPI41_CPMAC_TX_Q0 + (pri))
#define CPMAC_CPPI4x_ETH_TO_HOST_PRXY_QNUM(pri)   (PAL_CPPI41_CPMAC_ETH_TO_HOST_PRXY_Q0 + (pri))


// CPMAC  End --------------------------------------------------------------------------------------

// Docsis MGMT 
#define CPPI4x_SR_DOCSIS_MGMT_TX_QNUM(pri)              (PAL_CPPI41_SR_DOCSIS_TX_MGMT_Q_NUM)
#define CPPI4x_SR_DOCSIS_MGMT_RXCMPL_QNUM(pri)  (PAL_CPPI41_SR_DOCSIS_MGMT_HOST_RX_Q_NUM)
#define CPPI4x_SR_DOCSIS_MGMT_RX_FD_HOST_QNUM   (PAL_CPPI41_SR_DOCSIS_MGMT_RX_FD_HOST_Q_NUM)
#define CPPI4x_SR_DOCSIS_MGMT_TX_FD_HOST_QNUM   (PAL_CPPI41_SR_DOCSIS_MGMT_TX_FD_HOST_Q_NUM)


// Voice
#define C55_CPPI4x_RX_QNUM(pri)                 (PAL_CPPI41_SR_VOICE_DSP_C55_HOST_RX_Q_NUM)




// USB --------------------------------------------------------------------------
#define USB_CPPI4x_USB_BD_SIZE                  64
#define USB_MAX_EPS                             8
#define USB_CPPI4x_MAX_USB_DESC                 (64 * USB_MAX_EPS) /* number of Descriptors for all tx/rx endpoints */


#define CPPI41_USB_BD_SIZE                  USB_CPPI4x_USB_BD_SIZE
#define CPPI41_MAX_USB_EPS                  8
#define CPPI41_MAX_USB_DESC                 (64 * CPPI41_MAX_USB_EPS) /* number of Descriptors for all tx/rx endpoints */

#define USB_CPPI4x_USB_TO_HOST_PRXY_QNUM(pri)   (PAL_CPPI41_USB_TO_HOST_PRXY_BASE + (pri))

#define USB_CPPI4x_EP0_TX_QNUM(pri)   (USB_CPPI41_TX1_QNUM)
// USB end --------------------------------------------------------------------------







// PP ----------------------------------------------------------------------
#define PAL_CPPI4x_PRTY_LOW                         0
#define PAL_CPPI4x_PRTY_MED                         1
#define PAL_CPPI4x_PRTY_HIGH                        2
#define PAL_CPPI4x_PRTY_URGENT                      3  // Actually not used

//Host--> PP (tx)
#define CPPI4x_PPFW_TX_INFRA_INPUT_QNUM(pri)      (PAL_CPPI4x_HOST2SR_TX_Q0 + (4 * (pri)))
#define PAL_CPPI41_SR_HOST_TO_PP_FD_HOST_Q_NUM(pri)  (PAL_CPPI41_SR_HOST_TO_PP_LOW_FD_HOST_Q_NUM + (pri))
#define PPFW_CPPI4x_FD_QNUM(pri)                (PAL_CPPI41_SR_HOST_TO_PP_INFRA_LOW_FD_EMB_Q_NUM + (pri))

// Host-->QPDSP
#define PPFW_CPPI4x_TX_EGRESS_Q_COUNT               4
#define PPFW_CPPI4x_TX_EGRESS_EMB_QNUM(pri)         (PAL_CPPI41_SR_HOST_TO_QPDSP_LOW_EMB_TYPE_Q_NUM + 2*(pri))
#define PPFW_CPPI4x_TX_EGRESS_HOST_QNUM(pri)        (PAL_CPPI41_QPDSP_IP_Q_BASE + 2*(pri) + 0)


// QoS
//#define PPFW_CPPI4x_QoS_Q_COUNT                     ((PAL_CPPI41_SR_QPDSP_QOS_Q_LAST - PAL_CPPI41_SR_QPDSP_QOS_Q_BASE) + 1)
#define PPFW_CPPI4x_QoS_QNUM(pri)                   (PAL_CPPI41_SR_QPDSP_QOS_Q_BASE + (pri))
#define PAL_CPPI41_QPDSP_FW_Q_BASE                  PAL_CPPI41_SR_QPDSP_QOS_Q_BASE
#define PAL_CPPI41_QPDSP_FW_Q_LAST                  PAL_CPPI41_SR_QPDSP_QOS_Q_LAST

#define PAL_CPPI41_SR_DOCSIS_TX_DATA_Q_COUNT            9
#define PAL_CPPI41_SR_DOCSIS_TX_DATA_Q_NUM(pri)         (PAL_CPPI41_SR_DOCSIS_TX_VOICE_Q_NUM +  (pri))
#define PAL_CPPI41_SR_DOCSIS_TX_DATA_Q_OFFSET(pri)      PAL_CPPI41_SR_DOCSIS_TX_DATA_Q_NUM(pri)

#define PAL_CPPI41_SR_DOCSIS_TX_HIGH_QPDSP_QOS_Q_NUM(clst)               PPFW_CPPI4x_QoS_QNUM(0 + (clst)*2)
#define PAL_CPPI41_SR_DOCSIS_TX_LOW_QPDSP_QOS_Q_NUM(clst)                PPFW_CPPI4x_QoS_QNUM(1 + (clst)*2)
#define PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_COUNT       8
#define PAL_CPPI41_SR_DOCSIS_TX_QOS_CLUSTER_BASE        0

#define CPMAC_CPPI4x_QoS_CLUSTER_COUNT            1
#define CPMAC_CPPI4x_QoS_CLUSTER_IDX              8

#define PAL_CPPI41_SR_HOST_TO_PP_Q_COUNT               3
#define PAL_CPPI41_SR_HOST_TO_PP_INFRA_INPUT_Q_NUM(pri)            (((pri)>(CPPI4x_PPFW_TX_INFRA_CH_COUNT - 1)) ? PPFW_CPPI4x_TX_EGRESS_HOST_QNUM(pri) : CPPI4x_PPFW_TX_INFRA_INPUT_QNUM(pri))


#define PPFW_RX_EMBEDDED_BD_NUM_LOW                 128
#define PPFW_RX_EMBEDDED_BD_NUM_MED                 64

#define PPFW_TX_HOST_BD_NUM_LOW                     8
#define PPFW_TX_HOST_BD_NUM_MED                     8
#define PPFW_TX_HOST_BD_NUM_HIGH                    16

#define PPFW_HOST_BD_SIZE                           64
#define PPFW_HOST_BD_NUM       (PPFW_TX_HOST_BD_NUM_LOW + \
                                PPFW_TX_HOST_BD_NUM_MED + \
                                PPFW_TX_HOST_BD_NUM_HIGH)

/* PP-FW parameters */
#define PPFW_CPPI4x_BD_SIZE                                 64
#define PPFW_CPPI4x_BUF_BD_SIZE                            128
#define PPFW_EVENT_DESC_NUM                 256
#define PPFW_REPLICA_DESC_COUNT             (0)
#define PPFW_PREFETCH_DESC_COUNT            (510)
#define PPFW_PREFETCH_BUFF_COUNT            (24)

//PP --> Host Envets
#define PP_HOST_EVENT_FDQ       PAL_CPPI41_SR_PP_EVENTS_FD_Q_NUM 

// PP End ----------------------------------------------------------------------

// MPEG --------------------------------------------------------------------------
#define MPEG_CPPI4x_POOL_NUM(sltcnt)            (BMGR0_POOL02)

#define MPEG_ENCAP_CPPI4x_POOL_NUM(sltcnt)      (BMGR0_POOL08)

#define MPEG_BD_SIZE                            64
#define MPEG_ENCAP_BD_SIZE                      128

#define MPEGOUT_CPPI4x_TX_QNUM(idx)             (PAL_CPPI4x_MPEG_OUT_Q_0 + (idx))

#define DMAC_MPEG_RX_EMBEDDED_BD_NUM            512
#define DMAC_MPEG_ENCAP_RX_EMBEDDED_BD_NUM      256
#define MPEG_CPPI4x_TX_SESSION_QNUM(idx)        (PAL_CPPI4x_MPEG_TX_Q_BASE + (idx)) 
// MPEG end --------------------------------------------------------------------------


// C55 DSP  -------------------------------------------------------------------------- 
#define PAL_CPPI41_VOICE_DSP_C55_QMGR                       PAL_CPPI41_QUEUE_MGR_PARTITION_SR

//#define PAL_CPPI41_VOICE_DSP_C55_INFRA_CHN                  PAL_CPPI41_SR2C55_DMA_CHN_NUM_BASE
//#define PAL_CPPI41_VOICE_DSP_C55_INFRA_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define PAL_CPPI41_VOICE_DSP_C55_INFRA_DMA_ID               PAL_CPPI41_DMA_BLOCK1
#define PAL_CPPI41_VOICE_DSP_C55_INFRA_TD_QNUM              DMA1_CPPI4x_FTD_QNUM


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

#define PAL_CPPI41_VOICE_DSP_C55_HOST_RX_Q_NUM              C55_CPPI4x_RX_QNUM(0)
#define PAL_CPPI41_VOICE_NI_OUTPUT_QNUM                     PPFW_CPPI4x_TX_EGRESS_EMB_QNUM(PAL_CPPI4x_PRTY_HIGH)

// -------------------------------------------------------------------------- 


/*
+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
|D|O|C|S|I|S| |Q|u|e|u|e| |M|a|n|a|g|e|r|
+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
*/

typedef enum PAL_CPPI41_DOCSIS_US_QMGR_QUEUES
{
    PAL_CPPI41_DOCSIS_INTERNAL_Q_BASE,  
    PAL_CPPI41_DOCSIS_INTERNAL_Q_0  = PAL_CPPI41_DOCSIS_INTERNAL_Q_BASE,                    //0
    PAL_CPPI41_DOCSIS_INTERNAL_Q_1,                     //1
    PAL_CPPI41_DOCSIS_INTERNAL_Q_2,                     //2
    PAL_CPPI41_DOCSIS_INTERNAL_Q_3,                     //3
    PAL_CPPI41_DOCSIS_INTERNAL_Q_4,                     //4
    PAL_CPPI41_DOCSIS_INTERNAL_Q_5,                     //5
    PAL_CPPI41_DOCSIS_INTERNAL_Q_6,                     //6
    PAL_CPPI41_DOCSIS_INTERNAL_Q_7,                     //7
    CNI_CPPI4x_DOCSIS_DS_CoP_Q,                         //8
    PAL_CPPI41_DOCSIS_INTERNAL_Q_9  ,                   //9
    PAL_CPPI41_DOCSIS_INTERNAL_Q_10 ,                   //10
    PAL_CPPI41_DOCSIS_INTERNAL_Q_11 ,                   //11
    PAL_CPPI41_DOCSIS_INTERNAL_Q_12 ,                   //12
    PAL_CPPI41_DOCSIS_INTERNAL_Q_13 ,                   //13
    PAL_CPPI41_DOCSIS_INTERNAL_Q_14 ,                   //14
    PAL_CPPI41_DOCSIS_INTERNAL_Q_15 ,                   //15
    PAL_CPPI41_DOCSIS_INTERNAL_Q_16 ,                   //16
    PAL_CPPI41_DOCSIS_INTERNAL_Q_17 ,                   //17
    PAL_CPPI41_DOCSIS_INTERNAL_Q_18 ,                   //18
    PAL_CPPI41_DOCSIS_INTERNAL_Q_19 ,                   //19
    PAL_CPPI41_DOCSIS_INTERNAL_Q_20 ,                   //20
    PAL_CPPI41_DOCSIS_INTERNAL_Q_21 ,                   //21
    PAL_CPPI41_DOCSIS_INTERNAL_Q_22 ,                   //22
    PAL_CPPI41_DOCSIS_INTERNAL_Q_23 ,                   //23
    PAL_CPPI41_DOCSIS_INTERNAL_Q_24 ,                   //24
    PAL_CPPI41_DOCSIS_INTERNAL_Q_25 ,                   //25
    PAL_CPPI41_DOCSIS_INTERNAL_Q_26 ,                   //26
    PAL_CPPI41_DOCSIS_INTERNAL_Q_27 ,                   //27
    PAL_CPPI41_DOCSIS_INTERNAL_Q_28 ,                   //28
    PAL_CPPI41_DOCSIS_INTERNAL_Q_29 ,                   //29
    PAL_CPPI41_DOCSIS_INTERNAL_Q_30 ,                   //30
    PAL_CPPI41_DOCSIS_INTERNAL_Q_31 ,                   //31
    PAL_CPPI41_DOCSIS_INTERNAL_Q_32 ,                   //32
    PAL_CPPI41_DOCSIS_INTERNAL_Q_33 ,                   //33
    PAL_CPPI41_DOCSIS_INTERNAL_Q_34 ,                   //34
    PAL_CPPI41_DOCSIS_INTERNAL_Q_35 ,                   //35
    PAL_CPPI41_DOCSIS_INTERNAL_Q_36 ,                   //36
    PAL_CPPI41_DOCSIS_INTERNAL_Q_37 ,                   //37
    PAL_CPPI41_DOCSIS_INTERNAL_Q_38 ,                   //38
    PAL_CPPI41_DOCSIS_INTERNAL_Q_39 ,                   //39
    PAL_CPPI41_DOCSIS_INTERNAL_Q_40 ,                   //40
    PAL_CPPI41_DOCSIS_INTERNAL_Q_41 ,                   //41
    PAL_CPPI41_DOCSIS_INTERNAL_Q_42 ,                   //42
    PAL_CPPI41_DOCSIS_INTERNAL_Q_43 ,                   //43
    PAL_CPPI41_DOCSIS_INTERNAL_Q_44 ,                   //44
    PAL_CPPI41_DOCSIS_INTERNAL_Q_45 ,                   //45
    PAL_CPPI41_DOCSIS_INTERNAL_Q_46 ,                   //46
    PAL_CPPI41_DOCSIS_INTERNAL_Q_47 ,                   //47
    PAL_CPPI41_DOCSIS_INTERNAL_Q_48 ,                   //48
    PAL_CPPI41_DOCSIS_INTERNAL_Q_49 ,                   //49
    PAL_CPPI41_DOCSIS_INTERNAL_Q_50 ,                   //50
    PAL_CPPI41_DOCSIS_INTERNAL_Q_51 ,                   //51
    PAL_CPPI41_DOCSIS_INTERNAL_Q_52 ,                   //52
    PAL_CPPI41_DOCSIS_INTERNAL_Q_53 ,                   //53
    PAL_CPPI41_DOCSIS_INTERNAL_Q_54 ,                   //54
    PAL_CPPI41_DOCSIS_INTERNAL_Q_55 ,                   //55
    PAL_CPPI41_DOCSIS_INTERNAL_Q_56 ,                   //56
    PAL_CPPI41_DOCSIS_INTERNAL_Q_57 ,                   //57
    CNI_CPPI4x_DOCSIS_DS_FDQ,                           //58
    PAL_CPPI41_DOCSIS_TX_MONOLITHIC_Q_NUM ,             //59
    CNI_CPPI4x_DOCSIS_US_FDQ,                           // 60
    PAL_CPPI41_DOCSIS_INTERNAL_Q_61 ,                   //61
    PAL_CPPI41_DOCSIS_INTERNAL_Q_62 ,                   //62
    PAL_CPPI41_DOCSIS_INTERNAL_Q_LAST,         // 63
    PAL_CPPI41_DOCSIS_INTERNAL_Q_COUNT
}PAL_CPPI41_DOCSIS_US_QMGR_QUEUES_e;


#define CNI_CPPI4x_DOCSIS_DS_DESC_SIZE                  128
#define CNI_CPPI4x_DOCSIS_DS_DESC_NUM                   32

#define CNI_CPPI4x_DOCSIS_US_DESC_SIZE                  64
#define CNI_CPPI4x_DOCSIS_US_DESC_NUM                   512


#define PAL_CPPI41_DOCSIS_QMGR_QUEUES_STR(v)    \
static char * v[ PAL_CPPI41_DOCSIS_INTERNAL_Q_COUNT] =   \
{\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_0",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_1",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_2",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_3",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_4",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_5",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_6",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_7",\
"CNI_CPPI4x_DOCSIS_DS_CoP_Q",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_9",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_10",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_11",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_12",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_13",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_14",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_15",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_16",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_17",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_18",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_19",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_20",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_21",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_22",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_23",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_24",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_25",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_26",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_27",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_28",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_29",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_30",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_31",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_32",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_33",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_34",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_35",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_36",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_37",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_38",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_39",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_40",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_41",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_42",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_43",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_44",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_45",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_46",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_47",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_48",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_49",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_50",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_51",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_52",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_53",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_54",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_55",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_56",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_57",\
"CNI_CPPI4x_DOCSIS_DS_FDQ",\
"PAL_CPPI41_DOCSIS_TX_MONOLITHIC_Q_NUM",\
"CNI_CPPI4x_DOCSIS_US_FDQ",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_61",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_62",\
"PAL_CPPI41_DOCSIS_INTERNAL_Q_63"\
\
}

/* ---------------------------------------------------------------- */
/********************** Shared resources parameters *****************/
/* ---------------------------------------------------------------- */

/* DOCSIS MAC parameters */
#define DMAC_US_BD_BASE                     0x01119000
#define DMAC_US_BD_SIZE                            128

/************************************************************************/

// QUEUE manager end


/**********************************************************************************************************************

########  ##     ## ######## ######## ######## ########     ##     ##    ###    ##    ##    ###     ######   ######## ########
##     ## ##     ## ##       ##       ##       ##     ##    ###   ###   ## ##   ###   ##   ## ##   ##    ##  ##       ##     ##
##     ## ##     ## ##       ##       ##       ##     ##    #### ####  ##   ##  ####  ##  ##   ##  ##        ##       ##     ##
########  ##     ## ######   ######   ######   ########     ## ### ## ##     ## ## ## ## ##     ## ##   #### ######   ########
##     ## ##     ## ##       ##       ##       ##   ##      ##     ## ######### ##  #### ######### ##    ##  ##       ##   ##
##     ## ##     ## ##       ##       ##       ##    ##     ##     ## ##     ## ##   ### ##     ## ##    ##  ##       ##    ##
########   #######  ##       ##       ######## ##     ##    ##     ## ##     ## ##    ## ##     ##  ######   ######## ##     ##

Buffer Manager 0
pool#   pool size   buf size    RefC    Usage
0       2048        256         0       DOCSIS RX Low Slot1,2 DSG0
1       2048        256         0       DOCSIS RX Low Slot1,2 DSG0
2       2048        256         1       MPEG
3       0           256         0       DOCSIS RX Low Slot1,2 DSG1
4       512         1024        0       DOCSIS RX Low Slot3
5       128         512         0       DOCSIS RX High+Voice Slots1,2,3
6       512         256         0       Host-->PP Infra Slot0
7       1024        512         0       Host-->PP Infra Slot1
8       512         128         0       MPEG Encap
9       0           256         0       VOICE DSP Infra
10      1024        256         1       ETH+USB Slot1,2
11      512         2048        1       ETH+USB Slot3
12      256         2048        1       DOCSIS TX US CoP
13      1024        128         0       MPDSP Slot0
 
 
Buffer Manager 1 
pool#   pool size   buf size    RefC    Usage
0       64          512          1       DOCSIS TX US CoP (Q60)
 
**********************************************************************************************************************/
/**
 * Buffer manager configurations
 */
#define PAL_CPPI41_NUM_BUF_MGR                  2    /**< Number of buffer managers in the system */

#define PAL_CPPI41_BUF_MGR_PARTITION_SR         0
#define PAL_CPPI41_BUF_MGR_PARTITION_DOCSIS     1

typedef enum PAL_CPPI41_BUFFER_POOL_MANAGERS
{
    BUF_POOL_MGR0,
    BUF_POOL_MGR1,
    PAL_CPPI41_BUFFER_POOL_MAX_MANAGERES
}PAL_CPPI41_BUFFER_POOL_MANAGERS_e;

typedef enum PAL_CPPI41_BUFFER_POOL_NUM
{
    PAL_CPPI41_BMGR_POOL0,
    PAL_CPPI41_BMGR_POOL1,
    PAL_CPPI41_BMGR_POOL2,
    PAL_CPPI41_BMGR_POOL3,
    PAL_CPPI41_BMGR_POOL4,
    PAL_CPPI41_BMGR_POOL5,
    PAL_CPPI41_BMGR_POOL6,
    PAL_CPPI41_BMGR_POOL7,
    PAL_CPPI41_BMGR_POOL8,
    PAL_CPPI41_BMGR_POOL9,
    PAL_CPPI41_BMGR_POOL10,
    PAL_CPPI41_BMGR_POOL11,
    PAL_CPPI41_BMGR_POOL12,
    PAL_CPPI41_BMGR_POOL13,
    PAL_CPPI41_BMGR_MAX_POOLS = 14  //< Maximum number of buffer pools per manager 
}PAL_CPPI41_BUFFER_POOL_NUM_e;

#define BMGR0_POOL00 (PAL_CPPI41_BMGR_POOL0)                 
#define BMGR0_POOL01 PAL_CPPI41_BMGR_POOL1                    
#define BMGR0_POOL02 PAL_CPPI41_BMGR_POOL2                    
#define BMGR0_POOL03 PAL_CPPI41_BMGR_POOL3                    
#define BMGR0_POOL04 PAL_CPPI41_BMGR_POOL4                   
#define BMGR0_POOL05 PAL_CPPI41_BMGR_POOL5                   
#define BMGR0_POOL06 PAL_CPPI41_BMGR_POOL6                    
#define BMGR0_POOL07 PAL_CPPI41_BMGR_POOL7                 
#define BMGR0_POOL08 PAL_CPPI41_BMGR_POOL8                    
#define BMGR0_POOL09 PAL_CPPI41_BMGR_POOL9                    
#define BMGR0_POOL10 PAL_CPPI41_BMGR_POOL10                   
#define BMGR0_POOL11 PAL_CPPI41_BMGR_POOL11                   
#define BMGR0_POOL12 PAL_CPPI41_BMGR_POOL12                   
#define BMGR0_POOL13 PAL_CPPI41_BMGR_POOL13                   

/* ---------------------------------------------------------------------- */
/*************Buffer pool allocation******************/
/* ---------------------------------------------------------------------- */

/*
+-+-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+
|B|u|f|f|e|r| |M|a|n|a|g|e|r| |0|
+-+-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+
*/

#define BMGR0_POOL00_BUF_COUNT          2048
#define BMGR0_POOL01_BUF_COUNT          2048
#define BMGR0_POOL02_BUF_COUNT          2048
#define BMGR0_POOL03_BUF_COUNT          0
#define BMGR0_POOL04_BUF_COUNT          1024
#define BMGR0_POOL05_BUF_COUNT          128
#define BMGR0_POOL06_BUF_COUNT          512
#define BMGR0_POOL07_BUF_COUNT          1024
#define BMGR0_POOL08_BUF_COUNT          512
#define BMGR0_POOL09_BUF_COUNT          512
#define BMGR0_POOL10_BUF_COUNT          1024
#define BMGR0_POOL11_BUF_COUNT          512
#define BMGR0_POOL12_BUF_COUNT          256
#define BMGR0_POOL13_BUF_COUNT          1024
 

#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT   // Support 1800 MTU
#define BMGR0_POOL00_BUF_SIZE           512
#define BMGR0_POOL01_BUF_SIZE           512
#else
#define BMGR0_POOL00_BUF_SIZE           256
#define BMGR0_POOL01_BUF_SIZE           256
#endif
#define BMGR0_POOL02_BUF_SIZE           256
#define BMGR0_POOL03_BUF_SIZE           0
#define BMGR0_POOL04_BUF_SIZE           1024
#define BMGR0_POOL05_BUF_SIZE           512
#define BMGR0_POOL06_BUF_SIZE           256
#define BMGR0_POOL07_BUF_SIZE           512
#define BMGR0_POOL08_BUF_SIZE           128
#define BMGR0_POOL09_BUF_SIZE           1024
#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT   // Support 1800 MTU
#define BMGR0_POOL10_BUF_SIZE           512
#else
#define BMGR0_POOL10_BUF_SIZE           256
#endif
#define BMGR0_POOL11_BUF_SIZE           1024
#define BMGR0_POOL12_BUF_SIZE           2048
#define BMGR0_POOL13_BUF_SIZE           128

#define BMGR0_POOL00_REF_CNT            1
#define BMGR0_POOL01_REF_CNT            1
#define BMGR0_POOL02_REF_CNT            1
#define BMGR0_POOL03_REF_CNT            0
#define BMGR0_POOL04_REF_CNT            0
#define BMGR0_POOL05_REF_CNT            1
#define BMGR0_POOL06_REF_CNT            1
#define BMGR0_POOL07_REF_CNT            0
#define BMGR0_POOL08_REF_CNT            0
#define BMGR0_POOL09_REF_CNT            0
#define BMGR0_POOL10_REF_CNT            1
#define BMGR0_POOL11_REF_CNT            1
#define BMGR0_POOL12_REF_CNT            1
#define BMGR0_POOL13_REF_CNT            0

/*
+-+-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+
|B|u|f|f|e|r| |M|a|n|a|g|e|r| |1|
+-+-+-+-+-+-+ +-+-+-+-+-+-+-+ +-+
*/
#define BMGR1_POOL00                    (PAL_CPPI41_BMGR_POOL0)

#define BMGR1_POOL00_BUF_COUNT          64     /* DOCSIS TX CoP After */
#define BMGR1_POOL01_BUF_COUNT          0
#define BMGR1_POOL02_BUF_COUNT          0
#define BMGR1_POOL03_BUF_COUNT          0
#define BMGR1_POOL04_BUF_COUNT          0
#define BMGR1_POOL05_BUF_COUNT          0
#define BMGR1_POOL06_BUF_COUNT          0
#define BMGR1_POOL07_BUF_COUNT          0
#define BMGR1_POOL08_BUF_COUNT          0
#define BMGR1_POOL09_BUF_COUNT          0
#define BMGR1_POOL10_BUF_COUNT          0
#define BMGR1_POOL11_BUF_COUNT          0
#define BMGR1_POOL12_BUF_COUNT          0
#define BMGR1_POOL13_BUF_COUNT          0
#define BMGR1_POOL00_BUF_SIZE           512
#define BMGR1_POOL00_REF_CNT            0

#define CPPI41_SRCPORT_VOICE_DSP_C55            7
/**********************************************************************************************************************

########  ##     ##    ###        ######  ##     ##    ###    ##    ## ##    ## ######## ##        ######
##     ## ###   ###   ## ##      ##    ## ##     ##   ## ##   ###   ## ###   ## ##       ##       ##    ##
##     ## #### ####  ##   ##     ##       ##     ##  ##   ##  ####  ## ####  ## ##       ##       ##
##     ## ## ### ## ##     ##    ##       ######### ##     ## ## ## ## ## ## ## ######   ##        ######
##     ## ##     ## #########    ##       ##     ## ######### ##  #### ##  #### ##       ##             ##
##     ## ##     ## ##     ##    ##    ## ##     ## ##     ## ##   ### ##   ### ##       ##       ##    ##
########  ##     ## ##     ##     ######  ##     ## ##     ## ##    ## ##    ## ######## ########  ######

**********************************************************************************************************************/

typedef enum PAL_CPPI41_SR_DMA_BLOCKS
{
    PAL_CPPI41_DMA_BLOCK0,
    PAL_CPPI41_DMA_BLOCK1,
    PAL_CPPI41_NUM_DMA_BLOCK                         /**< Total number of DMA blocks in the system */
}PAL_CPPI41_SR_DMA_BLOCKS_e;

/**
 * DMA channel configuration0
 */
#define PAL_CPPI41_SR_DMA_MAX_TX_CHANNELS             22    /**< Max CPPI4.1 Tx channles that could be opened */
#define PAL_CPPI41_SR_DMA_MAX_RX_CHANNELS             22    /**< Max CPPI4.1 Rx channles that could be opened */
#define PAL_CPPI41_NUM_TOTAL_CHAN              22    /**< Number of total channels */

/*
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
|D|M|A| |0|  |R|X| |C|h|a|n|n|e|l|s|
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
*/
typedef enum PAL_CPPI41_SR_DMA0_RX_CHANNELS
{
    PAL_CPPI41_DMA0_RX_ETH_CHN_NUM = 8,                   //    8    ETH     CDMA_CH_EMAC0                                     
}PAL_CPPI41_SR_DMA0_RX_CHANNELS_e;

/*
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
|D|M|A| |0|  |T|X| |C|h|a|n|n|e|l|s|
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
*/
typedef enum PAL_CPPI41_SR_DMA0_TX_CHANNELS
{
    PAL_CPPI41_DMA0_TX_ETH_CHN_NUM = 8,                   //8  ETH                                          
}PAL_CPPI41_SR_DMA0_TX_CHANNELS_e;


/*
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
|D|M|A| |1|  |R|X| |C|h|a|n|n|e|l|s|
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
*/
typedef enum PAL_CPPI41_SR_DMA1_RX_CHANNELS
{
    PAL_CPPI41_DMA1_RX_HOST2SR_INFRA_0_CHN_NUM  ,        //0 Host-->PP infra low
    PAL_CPPI41_DMA1_RX_HOST2SR_INFRA_1_CHN_NUM  ,        //1 Host-->PP med
    PAL_CPPI41_DMA1_RX_RECYCLING_INFRA_CHN_NUM,          //2 recycle 
    PAL_CPPI41_VOICE_DSP_C55_INFRA_CHN,                //3                                        
    PAL_CPPI41_DMA1_RX_USB_0_CHN_NUM,                    //4    4-7 USB RX
    PAL_CPPI41_DMA1_RX_USB_1_CHN_NUM,                    //5
    PAL_CPPI41_DMA1_RX_USB_2_CHN_NUM,                    //6
    PAL_CPPI41_DMA1_RX_USB_3_CHN_NUM,                    //7
    PAL_CPPI41_DMA1_RX_RESERVED_CH_NUM_8,                //8  eth                                                     //
    PAL_CPPI41_DMA1_RX_DOCSIS_DS_CoP_ODD_CH_NUM,        //9    9-11 = 3* DOCSIS RX CoP 
    PAL_CPPI41_DMA1_RX_DOCSIS_DS_CoP_EVEN_CH_NUM,        //10
    PAL_CPPI41_DMA1_RX_DOCSIS_DS_CoP_HIGH_CH_NUM,        //11   
    PAL_CPPI41_DMA1_RX_MPEG_CHN_NUM,                           //12   RX MPEG
    PAL_CPPI41_DMA1_RX_RESERVED_CH_NUM_13,               //13 -?
    PAL_CPPI41_DMA1_RX_DOCSIS_DS_CoP_MGMT_CHN_NUM,       //14- DOCSIS_MGMT
    PAL_CPPI41_DMA1_RX_DOCSIS_TX_CoP_CHN_NUM,            //15-  TX_CoP
    PAL_CPPI41_DMA1_RX_DOC2HOST_0_CHN_NUM,               //16  PP->host docss infra low
    PAL_CPPI41_DMA1_RX_DOC2HOST_1_CHN_NUM,              //17   PP->host docss infra med
    PAL_CPPI41_DMA1_RX_DOC2HOST_2_CHN_NUM,              //18   PP->host docss infra high
    PAL_CPPI41_DMA1_RX_RESERVED_CH_NUM_19,              //19
    PAL_CPPI41_DMA1_RX_USB2HOST_CHN_NUM,                //20   PP-> USB2HOST infra                                          
    PAL_CPPI41_DMA1_RX_ETH2HOST_CHN_NUM,                //21   PP-> ETH2HOST infra
    PAL_CPPI41_DMA1_RX_CHANNELS = 22
}PAL_CPPI41_SR_DMA1_RX_CHANNELS_e;

/*
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
|D|M|A| |1|  |T|X| |C|h|a|n|n|e|l|s|
+-+-+-+ +-+-+ +-+-+ +-+-+-+-+-+-+-+-
*/
typedef enum PAL_CPPI41_SR_DMA1_TX_CHANNELS
{
    PAL_CPPI41_DMA1_TX_HOST2SR_INFRA_0_CHN_NUM  ,           //0 Host-->PP infra low
    PAL_CPPI41_DMA1_TX_HOST2SR_INFRA_1_CHN_NUM  ,           //1 Host-->PP infra high
    PAL_CPPI41_DMA1_TX_RECYCLING_INFRA_CHN_NUM  ,           //2 recycle
    PAL_CPPI41_DMA1_TX_VOICE_DSP_C55_INFRA_CHN   ,               //3
    PAL_CPPI41_DMA1_TX_USB_0_CHN_NUM    ,                   //4     4-7 USB TX
    PAL_CPPI41_DMA1_TX_USB_1_CHN_NUM    ,                   //5
    PAL_CPPI41_DMA1_TX_USB_2_CHN_NUM    ,                   //6
    PAL_CPPI41_DMA1_TX_USB_3_CHN_NUM    ,                   //7
    PAL_CPPI41_DMA1_TX_RESERVED_CH_NUM_8,                //8  eth
    PAL_CPPI41_DMA1_TX_DOCSIS_TX_COP_0_CH_NUM ,                //9  9-12 = 4* TX_DOCSIS  
    PAL_CPPI41_DMA1_TX_DOCSIS_TX_COP_1_CH_NUM ,                   //10
    PAL_CPPI41_DMA1_TX_DOCSIS_TX_COP_2_CH_NUM ,                   //11
    PAL_CPPI41_DMA1_TX_DOCSIS_TX_COP_3_CH_NUM ,                   //12
    PAL_CPPI41_DMA1_TX_MPEG_CHN_NUM    ,                     //13   MPEG TX
    PAL_CPPI41_DMA1_TX_DOCSIS_MGMT_CHN_NUM  ,                //14  DOCSIS_MGMT
    PAL_CPPI41_DMA1_TX_DOCSIS_TX_CoP_CHN_NUM    ,            //15  TX_CoP
    PAL_CPPI41_DMA1_TX_DOC2HOST_0_CHN_NUM   ,                //16  PP->host docss infra low
    PAL_CPPI41_DMA1_TX_DOC2HOST_1_CHN_NUM   ,                //17  PP->host docss infra med
    PAL_CPPI41_DMA1_TX_DOC2HOST_2_CHN_NUM   ,                //18  PP->host docss infra high
    PAL_CPPI41_DMA1_TX_RESERVED_CHN_NUM_19  ,            //19-?
    PAL_CPPI41_DMA1_TX_USB2HOST_CHN_NUM ,                   //20   PP ->    USB2HOST infra
    PAL_CPPI41_DMA1_TX_ETH2HOST_CHN_NUM,                    //21   PP ->    ETH2HOST infra
    PAL_CPPI41_DMA1_TX_CHANNELS = 22
}PAL_CPPI41_SR_DMA1_TX_CHANNELS_e;

/**
 * DMA scheduler entries.
 */



#define CPPI4x_PPFW_TX_INFRA_CH_COUNT       2
#define CPPI4x_PPFW_TX_INFRA_CHNUM(idx)     (PAL_CPPI41_DMA1_TX_HOST2SR_INFRA_0_CHN_NUM + (idx))
#define CPPI4x_PPFW_RX_INFRA_CHNUM(idx)     (PAL_CPPI41_DMA1_RX_HOST2SR_INFRA_0_CHN_NUM + (idx))

//#define RECYCLE_INFRA_CHN(pri)                  (PAL_CPPI41_DMA1_RX_RECYCLE_CHN_NUM)


//#define CPMAC_CPPI4x_RX_DMA_CH_COUNT              1
#define CPMAC_CPPI4x_RX_DMA_CHNUM(idx)            (PAL_CPPI41_DMA0_RX_ETH_CHN_NUM + idx)
#define CPMAC_CPPI4x_RX_HOST_BD_SIZE             64 /* sizeof Cppi4HostDesc + align */

//#define CPMAC_CPPI4x_TX_DMA_CH_COUNT              1
#define CPMAC_CPPI4x_TX_DMA_CHNUM(idx)            (PAL_CPPI41_DMA0_TX_ETH_CHN_NUM + idx)
#define CPMAC_CPPI4x_TX_HOST_BD_SIZE             64 /* sizeof Cppi4HostDesc + align */


//keep for backward compatibility
//#define CPMAC_CPPI4x_ETH2HOST_PROXY_CH_COUNT      1
#define CPMAC_CPPI4x_ETH2HOST_PROXY_CHNUM(idx)    (PAL_CPPI41_DMA1_RX_ETH2HOST_CHN_NUM + idx)
#define CPMAC_CPPI4x_RX_EMB_BD_SIZE              64 /* sizeof Cppi4HostDesc + align */

#define CPPI4x_SR_DOCSIS_RX_DMA_CH_COUNT                3
#define CPPI4x_SR_DOCSIS_RX_DMA_CHNUM(idx)              (PAL_CPPI41_DMA1_RX_DOCSIS_DS_CoP_ODD_CH_NUM + (idx))

#define CPPI4x_SR_DOCSIS_TX_DMA_CH_COUNT                4
#define CPPI4x_SR_DOCSIS_TX_DMA_CHNUM(idx)              (PAL_CPPI41_DMA1_TX_DOCSIS_TX_COP_0_CH_NUM + (idx))

//#define MPEG_CPPI4x_RX_DMA_CH_COUNT                   1
//#define MPEG_CPPI4x_RX_DMA_CHNUM                      (PAL_CPPI41_DMA1_DOCSIS_RX_MPEG_CHN_NUM)

#define CPPI4x_CNI_RX_INFRA_CH_COUNT                    2
#define PAL_CPPI41_SR_CNI_INFRA_DMA_CH_COUNT            CPPI4x_CNI_RX_INFRA_CH_COUNT
#define CPPI4x_CNI_RX_INFRA_CH_NUM(idx)                 (PAL_CPPI41_DMA1_RX_DOC2HOST_0_CHN_NUM + (idx))
#define CPPI4x_CNI_TX_INFRA_CH_NUM(idx)                 (PAL_CPPI41_DMA1_TX_DOC2HOST_0_CHN_NUM + (idx))

//#define USB_CPPI4x_USB2HOST_PROXY_CH_COUNT      1
#define USB_CPPI4x_USB2HOST_PROXY_CHNUM(idx)    (PAL_CPPI41_DMA1_RX_USB2HOST_CHN_NUM)
#define USB_CPPI4x_RX_EMB_BD_SIZE               64 /* sizeof Cppi4HostDesc + align */


//#define USB_CPPI4x_RX_DMA_CH_COUNT              4
#define USB_CPPI4x_RX_DMA_CHNUM(idx)            (PAL_CPPI41_DMA1_RX_USB_0_CHN_NUM + (idx))

//#define USB_CPPI4x_TX_DMA_CH_COUNT              4
#define USB_CPPI4x_TX_DMA_CHNUM(idx)            (PAL_CPPI41_DMA1_TX_USB_0_CHN_NUM + (idx))

//#define PAL_CPPI41_DMA1_TX_DOCSIS_TX_CoP_CHN_NUM(idx)       (PAL_CPPI41_DMA1_TX_DOCSIS_TX_CoP_CHN_NUM)
//#define CPPI4x_SR_DOCSIS_TX_CoP_DMA_RX_CHNUM(idx)       (PAL_CPPI41_DMA1_RX_DOCSIS_TX_CoP_CHN_NUM)

//keep for backward compatibility
#define CPPI4x_SR_DOCSIS_MGMT_RXCMPL_CHNUM              (PAL_CPPI41_DMA1_RX_DOCSIS_DS_CoP_MGMT_CHN_NUM)
                                                         
#define MPEG_CPPI4x_RX_DMA_CHNUM               ( PAL_CPPI41_DMA1_RX_MPEG_CHN_NUM )
#define MPEGOUT_CPPI4x_CHNUM                    ( PAL_CPPI41_DMA1_TX_MPEG_CHN_NUM )
//DMA end
/**********************************************************************************************************************/




/**********************************************************************************************************************

   ###     ######   ######  ##     ## ##     ## ##     ## ##          ###    ########  #######  ########
  ## ##   ##    ## ##    ## ##     ## ###   ### ##     ## ##         ## ##      ##    ##     ## ##     ##
 ##   ##  ##       ##       ##     ## #### #### ##     ## ##        ##   ##     ##    ##     ## ##     ##
##     ## ##       ##       ##     ## ## ### ## ##     ## ##       ##     ##    ##    ##     ## ########
######### ##       ##       ##     ## ##     ## ##     ## ##       #########    ##    ##     ## ##   ##
##     ## ##    ## ##    ## ##     ## ##     ## ##     ## ##       ##     ##    ##    ##     ## ##    ##
##     ##  ######   ######   #######  ##     ##  #######  ######## ##     ##    ##     #######  ##     ##

    Accumulator Configuration
    =========================
    Channel INTV   Usage
    -------------------------------------------
    0       0       free
    1               
    -------------------------------------------
    2       1       USB RX
    3               
    4
    5
    -------------------------------------------
    6       2       CPG MAC RX 
    -------------------------------------------
    7       3       CPG MAC TXCMPL 
    -------------------------------------------
    8       4       USB TXCMPL
    -------------------------------------------
    9       5       CNI_LOW
    -------------------------------------------
    10      6       CNI_HIGH
    -------------------------------------------
    11      7       free
    -------------------------------------------
    12      8       free 
    -------------------------------------------
    13      9       free
    -------------------------------------------
    14      10      (CNI) DOCSIS_RX_MGMT 
    -------------------------------------------
    15      11      Voice (C55) DSP Rx
    -------------------------------------------
    16      12      MPEG RX
    17
    18
    19
    -------------------------------------------
    20      13       free
    21
    22
    23
    -------------------------------------------
    24      14      CNI_TXCMPL
    25              free
    26              free
    27              free
    -------------------------------------------
    28      15      free  
    29
    30
    31      
    -------------------------------------------
*********************************************************************************************************************/

//#define PAL_ACC_CHN_BASE                                 0
//#define PAL_ACC_CHN_LAST                                31
/*
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
|A|c|c|u|m|u|l|a|t|o|r| |C|h|a|n|n|e|l|s|
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+
*/
typedef enum PAL_CPPI41_ACCUMULATOR_CHANNELS
{
    PAL_CPPI41_RESERVED_ACC_CH_NUM_0,               //0
    PAL_CPPI41_RESERVED_ACC_CH_NUM_1,               //1
    USB_EP0_ACC_RX_CHNUM,                           //2
    USB_EP1_ACC_RX_CHNUM,                           //3
    USB_EP2_ACC_RX_CHNUM,                           //4
    USB_EP3_ACC_RX_CHNUM,                           //5
    CPMAC_ACC_RX_CHNUM,                             //6
    CPMAC_ACC_TXCMPL_CHNUM,                         //7
    USB_ACC_TXCMPL_CHNUM,                           //8
    CNI_ACC_RX_CHNUM_0,                             //9
    CNI_ACC_RX_CHNUM_1,                             //10
    PAL_CPPI41_RESERVED_ACC_CH_NUM_11,              //11
    PAL_CPPI41_RESERVED_ACC_CH_NUM_12,              //12
    PAL_CPPI41_RESERVED_ACC_CH_NUM_13,              //13
    PAL_CPPI41_DOCSIS_RX_MGMT_ACC_CH_NUM,           //14
    PAL_CPPI41_VOICE_DSP_C55_ACC_RX_CHNUM,          //15
    MPEG_ACC_RX_CHNUM_0,                            //16
    MPEG_ACC_RX_CHNUM_1,                            //17
    MPEG_ACC_RX_CHNUM_2,                            //18
    MPEG_ACC_RX_CHNUM_3,                            //19
    PAL_CPPI41_RESERVED_ACC_CH_NUM_20,              //20
    PAL_CPPI41_RESERVED_ACC_CH_NUM_21,              //21
    PAL_CPPI41_RESERVED_ACC_CH_NUM_22,              //22
    PAL_CPPI41_RESERVED_ACC_CH_NUM_23,              //23
    CNI_ACC_TXCMPL_CHNUM_0,                         //24
    PAL_CPPI41_RESERVED_ACC_CH_NUM_25,              //25
    PAL_CPPI41_RESERVED_ACC_CH_NUM_26,              //26
    PAL_CPPI41_RESERVED_ACC_CH_NUM_27,              //27
    PAL_CPPI41_RESERVED_ACC_CH_NUM_28,              //28
    PAL_CPPI41_RESERVED_ACC_CH_NUM_29,              //29
    PAL_CPPI41_RESERVED_ACC_CH_NUM_30,              //30
    PAL_CPPI41_RESERVED_ACC_CH_NUM_31,              //31
}PAL_CPPI41_ACCUMULATOR_CHANNELS_e;

/*
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+
|A|c|c|u|m|u|l|a|t|o|r| |I|n|t|e|r|r|u|p|t| |V|e|c|t|o|r|s|
+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+
*/
typedef enum PAL_CPPI41_ACCUMULATOR_INTERRUPT_VECTORS
{
    PAL_CPPI41_RESERVED_ACC_INTV_NUM_0,         //0
    USB_ALL_ACC_RX_INTV,                        //1
    CPMAC_ACC_RX_INTV,                          //2
    CPMAC_ACC_TXCMPL_INTV,                      //3
    USB_ACC_TXCMPL_INTV,                        //4
    PAL_CPPI41_CNI_LOW_ACC_CH_INTV_NUM,         //5
    PAL_CPPI41_CNI_HIGH_ACC_CH_INTV_NUM,        //6
    PAL_CPPI41_RESERVED_ACC_INTV_NUM_7,         //7
    PAL_CPPI41_RESERVED_ACC_INTV_NUM_8,         //8
    PAL_CPPI41_RESERVED_ACC_INTV_NUM_9,         //9
    PAL_CPPI41_DOCSIS_RX_MGMT_ACC_INTV_NUM,     //10
    PAL_CPPI41_VOICE_DSP_C55_ACC_RX_INTV,       //11
    PAL_CPPI41_MPEG_ACC_RX_INTV_NUM,            //12
    PAL_CPPI41_RESERVED_ACC_INTV_NUM_13,        //13
    PAL_CPPI41_TX_COMPLETE_ACC_INTV_NUM,        //14
    PAL_CPPI41_RESERVED_ACC_INTV_NUM_15,        //15
}PAL_CPPI41_ACCUMULATOR_INTERRUPT_VECTORS_e;


#define USB_ACC_RX1_CHNUM                   USB_EP0_ACC_RX_CHNUM
#define USB_ACC_RX2_CHNUM                   USB_EP1_ACC_RX_CHNUM
#define USB_ACC_RX3_CHNUM                   USB_EP2_ACC_RX_CHNUM
#define USB_ACC_RX4_CHNUM                   USB_EP3_ACC_RX_CHNUM
#define USB_ACCRX_INTD_VEC                  USB_ALL_ACC_RX_INTV

#define USB_ACC_TX_CHNUM                    USB_ACC_TXCMPL_CHNUM
#define USB_ACCTX_INTD_VEC                  USB_ACC_TXCMPL_INTV


//#define CNI_ACC_RX_CH_COUNT                     2
#define PAL_CPPI41_CNI_ACC_CH_NUM(idx)          (CNI_ACC_RX_CHNUM_0 + (idx))

#define CNI_ACC_TX_CH_COUNT                     1
#define PAL_CPPI41_TX_COMPLETE_LOW_ACC_CH_NUM   CNI_ACC_TXCMPL_CHNUM_0

#define MPEG_ACC_RX_CH_COUNT                    1
#define PAL_CPPI41_MPEG_ACC_RX_CH_NUM           (MPEG_ACC_RX_CHNUM_0)
/*********************************************************************************************************************/


/**********************************************************************************************************************

 ######   #######  ##     ## ########   ######  ########    ########   #######  ########  ########  ######
##    ## ##     ## ##     ## ##     ## ##    ## ##          ##     ## ##     ## ##     ##    ##    ##    ##
##       ##     ## ##     ## ##     ## ##       ##          ##     ## ##     ## ##     ##    ##    ##
 ######  ##     ## ##     ## ########  ##       ######      ########  ##     ## ########     ##     ######
      ## ##     ## ##     ## ##   ##   ##       ##          ##        ##     ## ##   ##      ##          ##
##    ## ##     ## ##     ## ##    ##  ##    ## ##          ##        ##     ## ##    ##     ##    ##    ##
 ######   #######   #######  ##     ##  ######  ########    ##         #######  ##     ##    ##     ######

**********************************************************************************************************************/
/* CPPI source port numbers - not configuration, hardware defined */
#define CPPI41_SRCPORT_USBEP0                   1
#define CPPI41_SRCPORT_USBEP1                   2
#define CPPI41_SRCPORT_USBEP2                   3
#define CPPI41_SRCPORT_USBEP3                   4
#define CPPI41_SRCPORT_CPMAC0                   5
#define PAL_CPPI41_SOURCE_PORT_DOCSIS           6 /* only one CNI port 6 */
#define CPPI41_SRCPORT_VOICE_DSP_C55            7


//----------------------------------------------------------
//  QManagers for backward compatibility
//----------------------------------------------------------

//recycle infra
#define RECYCLE_INFRA_RX_QMGR               PAL_CPPI41_QUEUE_MGR_PARTITION_SR

//USB
#define USB_CPPI41_TX1_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_TX2_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_TX3_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_TX4_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR

#define USB_CPPI4x_TX_COMP_QMGR             PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_TXCMPL_QMGR              PAL_CPPI41_QUEUE_MGR_PARTITION_SR

#define USB_CPPI41_RX1_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_RX2_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_RX3_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_RX4_QMGR                 PAL_CPPI41_QUEUE_MGR_PARTITION_SR

#define USB_CPPI41_RX1_FDB_QMGR             PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_RX2_FDB_QMGR             PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_RX3_FDB_QMGR             PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI41_RX4_FDB_QMGR             PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define USB_CPPI4x_USB_TO_HOST_PRXY_QMGR    PAL_CPPI41_QUEUE_MGR_PARTITION_SR

//cpmac
#define CPMAC_CPPI4x_TX_QMGR                PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define CPMAC_CPPI4x_RX_QMGR                PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define CPMAC_CPPI4x_FD_QMGR                PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define CPMAC_CPPI4x_FBD_QMGR               PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define CPMAC_CPPI4x_ETH_TO_HOST_PRXY_QMGR  PAL_CPPI41_QUEUE_MGR_PARTITION_SR


//MPEG
#define MPEGOUT_CPPI4x_TX_QMGR              PAL_CPPI41_QUEUE_MGR_PARTITION_SR


//DMA
#define DMA0_CPPI4x_FTD_QMGR                PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define DMA1_CPPI4x_FTD_QMGR                PAL_CPPI41_QUEUE_MGR_PARTITION_SR

//PP --> Host Envets
#define PP_HOST_EVENT_FDQMGR                PAL_CPPI41_QUEUE_MGR_PARTITION_SR
#define PP_HOST_EVENTQMGR                   PAL_CPPI41_QUEUE_MGR_PARTITION_SR







//----------------------------------------------------------
//  -- Jenia: U N U S E D
//----------------------------------------------------------

/* External descriptor common size */
//#define PAL_CPPI_COMMON1_BD_SIZE                        64
//#define PAL_CPPI_COMMON2_BD_SIZE                        64
//#define PAL_CPPI_COMMON3_BD_SIZE                        64
//#define PAL_CPPI_COMMON4_BD_SIZE                        64

//#define USB_CPPI4x_USB_TO_HOST_PRXY_Q_COUNT     4

//#define CPPI4x_SR_DOCSIS_MGMT_TX_Q_COUNT                1
//#define PAL_CPPI41_SR_DOCSIS_TX_DATA_Q_COUNT            9

//#define CPMAC_CPPI4x_TX_COMP_Q_COUNT            1
//#define USB_CPPI4x_TX_COMP_Q_COUNT              1
//#define CPPI4x_SR_DOCSIS_DATA_TX_COMP_Q_COUNT              3

//#define CPMAC_CPPI4x_RX_Q_COUNT                 1

//#define CPPI4x_CNI_RXCMPL_Q_COUNT               2
//#define CPPI4x_SR_DOCSIS_MGMT_RXCMPL_Q_COUNT    1
//
//#define C55_CPPI4x_RX_Q_COUNT                   1
//#define MPEG_CPPI4x_RX_Q_COUNT                  1


//#define CPPI4x_CNI_RX_INFRA_FD_HOST_Q_COUNT                  2
//#define CPPI4x_SR_DOCSIS_MGMT_FD_HOST_Q_COUNT   2


//#define CPMAC_CPPI4x_FBD_Q_COUNT                1
//#define USB_CPPI4x_FBD_Q_COUNT                  4
//#define PPFW_CPPI4x_FDB_Q_COUNT                 3
//#define RECYCLE_CPPI4x_FBD_Q_COUNT              1

//#define C55_CPPI4x_FD_Q_COUNT                   1
//#define MPEG_CPPI4x_FD_Q_COUNT                  1
//#define MPEG_ENCAP_CPPI4x_FD_Q_COUNT            1


//#define CPMAC_CPPI4x_ETH_TO_HOST_PRXY_Q_COUNT     2
//#define USB_CPPI4x_TX_Q_COUNT                   8

//#define MPEG_CPPI4x_TX_SESSION_Q_COUNT          1



#endif // _PUMA5_CPPI_PRV_H_
