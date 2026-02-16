/*

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

#define PAL_CPPI_PP_QMGR_G2_Q_LIST                                                                                        \
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q0 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_MED_LOW_Q_NUM)                             /* PAL_CPPI_PP_QMGR_G2_Q1 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_MED_HI_Q_NUM)                              /* PAL_CPPI_PP_QMGR_G2_Q2 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q3 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_Q4)                                        /* PAL_CPPI_PP_QMGR_G2_Q4 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_Q5)                                        /* PAL_CPPI_PP_QMGR_G2_Q5 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_Q6)                                        /* PAL_CPPI_PP_QMGR_G2_Q6 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PREFETCHER_Q7)                                        /* PAL_CPPI_PP_QMGR_G2_Q7 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_LOW_TX_COMPLETE_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q8 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_HI_TX_COMPLETE_Q_NUM)                         /* PAL_CPPI_PP_QMGR_G2_Q9 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_NP2APP_RX_COMPLETE_Q_NUM)                             /* PAL_CPPI_PP_QMGR_G2_Q10 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_RX_COMPLETE_Q_NUM)                             /* PAL_CPPI_PP_QMGR_G2_Q11 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_NP2APP_TX_COMPLETE_Q_NUM)                             /* PAL_CPPI_PP_QMGR_G2_Q12 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_TX_COMPLETE_Q_NUM)                             /* PAL_CPPI_PP_QMGR_G2_Q13 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_VOICE_Q_NUM)                         /* PAL_CPPI_PP_QMGR_G2_Q14 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_FCC_FORWARDING_Q_NUM)                /* PAL_CPPI_PP_QMGR_G2_Q15 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_DOCSIS_MGMT_Q_NUM)                   /* PAL_CPPI_PP_QMGR_G2_Q16 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_DOCSIS_LOW_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q17 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_DOCSIS_HI_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q18 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_MoCA_Q_NUM)                          /* PAL_CPPI_PP_QMGR_G2_Q19 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_ATOM_LOW_Q_NUM)                      /* PAL_CPPI_PP_QMGR_G2_Q20 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_ATOM_HI_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q21 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_RGMII0_LOW_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q22 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_RGMII0_HI_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q23 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_RGMII1_LOW_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q24 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_RGMII1_HI_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q25 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_SGMII0_LOW_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q26 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_SGMII0_HI_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q27 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_SGMII1_LOW_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q28 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_SGMII1_HI_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q29 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_WIFI_LOW_Q_NUM)                      /* PAL_CPPI_PP_QMGR_G2_Q30 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_WIFI_HI_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q31 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q0)                                          /* PAL_CPPI_PP_QMGR_G2_Q32 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q1)                                          /* PAL_CPPI_PP_QMGR_G2_Q33 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q2)                                          /* PAL_CPPI_PP_QMGR_G2_Q34 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q3)                                          /* PAL_CPPI_PP_QMGR_G2_Q35 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q4)                                          /* PAL_CPPI_PP_QMGR_G2_Q36 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q5)                                          /* PAL_CPPI_PP_QMGR_G2_Q37 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q6)                                          /* PAL_CPPI_PP_QMGR_G2_Q38 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q7)                                          /* PAL_CPPI_PP_QMGR_G2_Q39 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q8)                                          /* PAL_CPPI_PP_QMGR_G2_Q40 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q9)                                          /* PAL_CPPI_PP_QMGR_G2_Q41 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q10)                                         /* PAL_CPPI_PP_QMGR_G2_Q42 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q11)                                         /* PAL_CPPI_PP_QMGR_G2_Q43 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q12)                                         /* PAL_CPPI_PP_QMGR_G2_Q44 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q13)                                         /* PAL_CPPI_PP_QMGR_G2_Q45 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q14)                                         /* PAL_CPPI_PP_QMGR_G2_Q46 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DPI_PDSP_Q15)                                         /* PAL_CPPI_PP_QMGR_G2_Q47 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ENCRYPT_PP_IN_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q48 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ENCRYPT_HOST_IN_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q49 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ENCRYPT_CRYPTO_IN_Q_NUM)                              /* PAL_CPPI_PP_QMGR_G2_Q50 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ENCRYPT_HOST_OUT_Q_NUM)                               /* PAL_CPPI_PP_QMGR_G2_Q51 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DECRYPT_PP_IN_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q52 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DECRYPT_HOST_IN_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q53 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DECRYPT_CRYPTO_IN_Q_NUM)                              /* PAL_CPPI_PP_QMGR_G2_Q54 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DECRYPT_HOST_OUT_Q_NUM)                               /* PAL_CPPI_PP_QMGR_G2_Q55 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q0)                                           /* PAL_CPPI_PP_QMGR_G2_Q56 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q1)                                           /* PAL_CPPI_PP_QMGR_G2_Q57 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q2)                                           /* PAL_CPPI_PP_QMGR_G2_Q58 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q3)                                           /* PAL_CPPI_PP_QMGR_G2_Q59 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q4)                                           /* PAL_CPPI_PP_QMGR_G2_Q60 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q5)                                           /* PAL_CPPI_PP_QMGR_G2_Q61 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q6)                                           /* PAL_CPPI_PP_QMGR_G2_Q62 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MC_PDSP_Q7)                                           /* PAL_CPPI_PP_QMGR_G2_Q63 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PHY_Q_NUM)                                            /* PAL_CPPI_PP_QMGR_G2_Q64 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_WIFI_DEVICE0_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q65 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_WIFI_DEVICE1_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q65 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_WIFI_DEVICE2_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q65 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_RX_COMPLETE_WIFI_DEVICE3_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q65 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_RX_HI_COMPLETE_Q_NUM)                          /* PAL_CPPI_PP_QMGR_G2_Q69 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q70)                                                  /* PAL_CPPI_PP_QMGR_G2_Q70 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q71)                                                  /* PAL_CPPI_PP_QMGR_G2_Q71 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q72)                                                  /* PAL_CPPI_PP_QMGR_G2_Q72 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q73)                                                  /* PAL_CPPI_PP_QMGR_G2_Q73 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q74)                                                  /* PAL_CPPI_PP_QMGR_G2_Q74 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q75)                                                  /* PAL_CPPI_PP_QMGR_G2_Q75 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q76)                                                  /* PAL_CPPI_PP_QMGR_G2_Q76 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q77)                                                  /* PAL_CPPI_PP_QMGR_G2_Q77 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q78)                                                  /* PAL_CPPI_PP_QMGR_G2_Q78 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q79)                                                  /* PAL_CPPI_PP_QMGR_G2_Q79 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q80)                                                  /* PAL_CPPI_PP_QMGR_G2_Q80 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q81)                                                  /* PAL_CPPI_PP_QMGR_G2_Q81 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q82)                                                  /* PAL_CPPI_PP_QMGR_G2_Q82 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q83)                                                  /* PAL_CPPI_PP_QMGR_G2_Q83 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q84)                                                  /* PAL_CPPI_PP_QMGR_G2_Q84 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q85)                                                  /* PAL_CPPI_PP_QMGR_G2_Q85 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q86)                                                  /* PAL_CPPI_PP_QMGR_G2_Q86 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q87)                                                  /* PAL_CPPI_PP_QMGR_G2_Q87 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q88)                                                  /* PAL_CPPI_PP_QMGR_G2_Q88 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q89)                                                  /* PAL_CPPI_PP_QMGR_G2_Q89 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q90)                                                  /* PAL_CPPI_PP_QMGR_G2_Q90 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q91)                                                  /* PAL_CPPI_PP_QMGR_G2_Q91 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q92)                                                  /* PAL_CPPI_PP_QMGR_G2_Q92 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q93)                                                  /* PAL_CPPI_PP_QMGR_G2_Q93 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q94)                                                  /* PAL_CPPI_PP_QMGR_G2_Q94 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q95)                                                  /* PAL_CPPI_PP_QMGR_G2_Q95 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q96)                                                  /* PAL_CPPI_PP_QMGR_G2_Q96 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q97)                                                  /* PAL_CPPI_PP_QMGR_G2_Q97 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_LOW_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q98 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_MED_LOW_Q_NUM)                              /* PAL_CPPI_PP_QMGR_G2_Q99 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_MED_HI_Q_NUM)                               /* PAL_CPPI_PP_QMGR_G2_Q100 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_HI_Q_NUM)                                   /* PAL_CPPI_PP_QMGR_G2_Q101 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_CYCLE2_LOW_Q_NUM)                           /* PAL_CPPI_PP_QMGR_G2_Q102 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_CYCLE2_MED_LOW_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q103 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_CYCLE2_MED_HI_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q104 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SEQUENCER_CYCLE2_HI_Q_NUM)                            /* PAL_CPPI_PP_QMGR_G2_Q105 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_ENCRYPT_IN_Q_NUM)                              /* PAL_CPPI_PP_QMGR_G2_Q106 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_DECRYPT_IN_Q_NUM)                              /* PAL_CPPI_PP_QMGR_G2_Q107 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_AES_Q_NUM)                                     /* PAL_CPPI_PP_QMGR_G2_Q108 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_HCU_Q_NUM)                                     /* PAL_CPPI_PP_QMGR_G2_Q109 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_PDSP_Q4)                                       /* PAL_CPPI_PP_QMGR_G2_Q110 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_PDSP_Q5)                                       /* PAL_CPPI_PP_QMGR_G2_Q111 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_PDSP_Q6)                                       /* PAL_CPPI_PP_QMGR_G2_Q112 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CRYPTO_PDSP_Q7)                                       /* PAL_CPPI_PP_QMGR_G2_Q113 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_0_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q114 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_0_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q115 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_1_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q116 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_1_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q117 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_2_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q118 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_2_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q119 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_3_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q120 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_3_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q121 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_4_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q122 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_4_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q123 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_5_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q124 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_5_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q125 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_6_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q126 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_6_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q127 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_7_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q128 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_7_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q129 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_8_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q130 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_8_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q131 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_9_HI_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q132 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_9_LOW_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q133 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_10_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q134 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_10_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q135 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_11_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q136 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_11_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q137 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_12_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q138 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_12_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q139 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_13_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q140 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_13_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q141 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_14_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q142 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_14_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q143 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_15_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q144 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_15_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q145 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_16_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q146 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_16_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q147 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_17_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q148 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_17_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q149 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_18_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q150 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_18_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q151 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_19_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q152 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_19_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q153 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_20_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q154 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_20_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q155 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_21_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q156 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_21_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q157 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_22_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q158 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_22_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q159 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_23_HI_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q160 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA9_INFRA_INPUT_LOW_TX_CH_23_LOW_Q_NUM)             /* PAL_CPPI_PP_QMGR_G2_Q161 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_TurboDOX_LOW_Q_NUM)                                   /* PAL_CPPI_PP_QMGR_G2_Q162 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_TurboDOX_MED_LOW_Q_NUM)                               /* PAL_CPPI_PP_QMGR_G2_Q163 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_TurboDOX_MED_HI_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q164 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_TurboDOX_HI_Q_NUM)                                    /* PAL_CPPI_PP_QMGR_G2_Q165 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q0)                         /* PAL_CPPI_PP_QMGR_G2_Q166 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q1)                         /* PAL_CPPI_PP_QMGR_G2_Q167 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q2)                         /* PAL_CPPI_PP_QMGR_G2_Q168 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q3)                         /* PAL_CPPI_PP_QMGR_G2_Q169 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q4)                         /* PAL_CPPI_PP_QMGR_G2_Q170 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q5)                         /* PAL_CPPI_PP_QMGR_G2_Q171 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q6)                         /* PAL_CPPI_PP_QMGR_G2_Q172 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_RESEQUENCER_INPUT_Q7)                         /* PAL_CPPI_PP_QMGR_G2_Q173 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q0)                                         /* PAL_CPPI_PP_QMGR_G2_Q174 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q1)                                         /* PAL_CPPI_PP_QMGR_G2_Q175 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q2)                                         /* PAL_CPPI_PP_QMGR_G2_Q176 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q3)                                         /* PAL_CPPI_PP_QMGR_G2_Q177 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q4)                                         /* PAL_CPPI_PP_QMGR_G2_Q178 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q5)                                         /* PAL_CPPI_PP_QMGR_G2_Q179 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q6)                                         /* PAL_CPPI_PP_QMGR_G2_Q180 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q7)                                         /* PAL_CPPI_PP_QMGR_G2_Q181 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q8)                                         /* PAL_CPPI_PP_QMGR_G2_Q182 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q9)                                         /* PAL_CPPI_PP_QMGR_G2_Q183 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q10)                                        /* PAL_CPPI_PP_QMGR_G2_Q184 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q11)                                        /* PAL_CPPI_PP_QMGR_G2_Q185 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q12)                                        /* PAL_CPPI_PP_QMGR_G2_Q186 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q13)                                        /* PAL_CPPI_PP_QMGR_G2_Q187 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q14)                                        /* PAL_CPPI_PP_QMGR_G2_Q188 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q15)                                        /* PAL_CPPI_PP_QMGR_G2_Q189 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q16)                                        /* PAL_CPPI_PP_QMGR_G2_Q190 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q17)                                        /* PAL_CPPI_PP_QMGR_G2_Q191 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q18)                                        /* PAL_CPPI_PP_QMGR_G2_Q192 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q19)                                        /* PAL_CPPI_PP_QMGR_G2_Q193 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q20)                                        /* PAL_CPPI_PP_QMGR_G2_Q194 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q21)                                        /* PAL_CPPI_PP_QMGR_G2_Q195 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q22)                                        /* PAL_CPPI_PP_QMGR_G2_Q196 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q23)                                        /* PAL_CPPI_PP_QMGR_G2_Q197 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q24)                                        /* PAL_CPPI_PP_QMGR_G2_Q198 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q25)                                        /* PAL_CPPI_PP_QMGR_G2_Q199 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q26)                                        /* PAL_CPPI_PP_QMGR_G2_Q200 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q27)                                        /* PAL_CPPI_PP_QMGR_G2_Q201 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q28)                                        /* PAL_CPPI_PP_QMGR_G2_Q202 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q29)                                        /* PAL_CPPI_PP_QMGR_G2_Q203 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q30)                                        /* PAL_CPPI_PP_QMGR_G2_Q204 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC0_PDSP_Q31)                                        /* PAL_CPPI_PP_QMGR_G2_Q205 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE0_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q206 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE0_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q207 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE1_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q208 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE1_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q209 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE2_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q210 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE2_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q211 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE3_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q212 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE3_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q213 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE4_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q214 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE4_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q215 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE5_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q216 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE5_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q217 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE6_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q218 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE6_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q219 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE7_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q220 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE7_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q221 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE8_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q222 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE8_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q223 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE9_HI_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q224 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE9_LOW_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q225 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE10_HI_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q226 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE10_LOW_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q227 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE11_HI_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q228 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE11_LOW_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q229 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE12_HI_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q230 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE12_LOW_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q231 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE13_HI_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q232 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE13_LOW_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q233 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE14_HI_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q234 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE14_LOW_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q235 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE15_HI_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q236 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_US_BE15_LOW_Q_NUM)                                /* PAL_CPPI_PP_QMGR_G2_Q237 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q32)                                         /* PAL_CPPI_PP_QMGR_G2_Q238 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q33)                                         /* PAL_CPPI_PP_QMGR_G2_Q239 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q34)                                         /* PAL_CPPI_PP_QMGR_G2_Q240 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q35)                                         /* PAL_CPPI_PP_QMGR_G2_Q241 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q36)                                         /* PAL_CPPI_PP_QMGR_G2_Q242 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q37)                                         /* PAL_CPPI_PP_QMGR_G2_Q243 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q38)                                         /* PAL_CPPI_PP_QMGR_G2_Q244 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q39)                                         /* PAL_CPPI_PP_QMGR_G2_Q245 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q40)                                         /* PAL_CPPI_PP_QMGR_G2_Q246 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q41)                                         /* PAL_CPPI_PP_QMGR_G2_Q247 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q42)                                         /* PAL_CPPI_PP_QMGR_G2_Q248 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q43)                                         /* PAL_CPPI_PP_QMGR_G2_Q249 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q44)                                         /* PAL_CPPI_PP_QMGR_G2_Q250 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q45)                                         /* PAL_CPPI_PP_QMGR_G2_Q251 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q46)                                         /* PAL_CPPI_PP_QMGR_G2_Q252 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q47)                                         /* PAL_CPPI_PP_QMGR_G2_Q253 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q48)                                         /* PAL_CPPI_PP_QMGR_G2_Q254 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q49)                                         /* PAL_CPPI_PP_QMGR_G2_Q255 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q50)                                         /* PAL_CPPI_PP_QMGR_G2_Q256 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q51)                                         /* PAL_CPPI_PP_QMGR_G2_Q257 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q52)                                         /* PAL_CPPI_PP_QMGR_G2_Q258 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q53)                                         /* PAL_CPPI_PP_QMGR_G2_Q259 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q54)                                         /* PAL_CPPI_PP_QMGR_G2_Q260 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q55)                                         /* PAL_CPPI_PP_QMGR_G2_Q261 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q56)                                         /* PAL_CPPI_PP_QMGR_G2_Q262 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q57)                                         /* PAL_CPPI_PP_QMGR_G2_Q263 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q58)                                         /* PAL_CPPI_PP_QMGR_G2_Q264 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q59)                                         /* PAL_CPPI_PP_QMGR_G2_Q265 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q60)                                         /* PAL_CPPI_PP_QMGR_G2_Q266 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q61)                                         /* PAL_CPPI_PP_QMGR_G2_Q267 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q62)                                         /* PAL_CPPI_PP_QMGR_G2_Q268 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_AQM_PDSP_Q63)                                         /* PAL_CPPI_PP_QMGR_G2_Q269 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_VOICE_DSP_TX_QNUM)                                    /* PAL_CPPI_PP_QMGR_G2_Q270 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_VOICE_DSP_Q1)                                         /* PAL_CPPI_PP_QMGR_G2_Q271 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_VOICE_DSP_Q2)                                         /* PAL_CPPI_PP_QMGR_G2_Q272 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_VOICE_DSP_Q3)                                         /* PAL_CPPI_PP_QMGR_G2_Q273 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q274)                                                 /* PAL_CPPI_PP_QMGR_G2_Q274 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q275)                                                 /* PAL_CPPI_PP_QMGR_G2_Q275 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q276)                                                 /* PAL_CPPI_PP_QMGR_G2_Q276 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q277)                                                 /* PAL_CPPI_PP_QMGR_G2_Q277 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q278)                                                 /* PAL_CPPI_PP_QMGR_G2_Q278 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q279)                                                 /* PAL_CPPI_PP_QMGR_G2_Q279 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q280)                                                 /* PAL_CPPI_PP_QMGR_G2_Q280 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q281)                                                 /* PAL_CPPI_PP_QMGR_G2_Q281 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q282)                                                 /* PAL_CPPI_PP_QMGR_G2_Q282 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q283)                                                 /* PAL_CPPI_PP_QMGR_G2_Q283 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q284)                                                 /* PAL_CPPI_PP_QMGR_G2_Q284 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q285)                                                 /* PAL_CPPI_PP_QMGR_G2_Q285 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q286)                                                 /* PAL_CPPI_PP_QMGR_G2_Q286 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q287)                                                 /* PAL_CPPI_PP_QMGR_G2_Q287 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q288)                                                 /* PAL_CPPI_PP_QMGR_G2_Q288 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q289)                                                 /* PAL_CPPI_PP_QMGR_G2_Q289 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q290)                                                 /* PAL_CPPI_PP_QMGR_G2_Q290 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q291)                                                 /* PAL_CPPI_PP_QMGR_G2_Q291 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q292)                                                 /* PAL_CPPI_PP_QMGR_G2_Q292 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RECYCLER_LOW_Q_NUM)                                   /* PAL_CPPI_PP_QMGR_G2_Q293 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RECYCLER_HI_Q_NUM)                                    /* PAL_CPPI_PP_QMGR_G2_Q294 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q0)                                            /* PAL_CPPI_PP_QMGR_G2_Q295 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q0)                                        /* PAL_CPPI_PP_QMGR_G2_Q296 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q1)                                            /* PAL_CPPI_PP_QMGR_G2_Q297 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q1)                                        /* PAL_CPPI_PP_QMGR_G2_Q298 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q2)                                            /* PAL_CPPI_PP_QMGR_G2_Q299 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q2)                                        /* PAL_CPPI_PP_QMGR_G2_Q300 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q3)                                            /* PAL_CPPI_PP_QMGR_G2_Q301 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q3)                                        /* PAL_CPPI_PP_QMGR_G2_Q302 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q4)                                            /* PAL_CPPI_PP_QMGR_G2_Q303 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q4)                                        /* PAL_CPPI_PP_QMGR_G2_Q304 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q5)                                            /* PAL_CPPI_PP_QMGR_G2_Q305 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q5)                                        /* PAL_CPPI_PP_QMGR_G2_Q306 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q6)                                            /* PAL_CPPI_PP_QMGR_G2_Q307 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q6)                                        /* PAL_CPPI_PP_QMGR_G2_Q308 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_TX_Q7)                                            /* PAL_CPPI_PP_QMGR_G2_Q309 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_UNUSED_Q7)                                        /* PAL_CPPI_PP_QMGR_G2_Q310 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SANITY_TESTS_INGRESS_Q)                               /* PAL_CPPI_PP_QMGR_G2_Q311 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q312)                                                 /* PAL_CPPI_PP_QMGR_G2_Q312 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q313)                                                 /* PAL_CPPI_PP_QMGR_G2_Q313 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q314)                                                 /* PAL_CPPI_PP_QMGR_G2_Q314 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q315)                                                 /* PAL_CPPI_PP_QMGR_G2_Q315 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q316)                                                 /* PAL_CPPI_PP_QMGR_G2_Q316 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q317)                                                 /* PAL_CPPI_PP_QMGR_G2_Q317 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q318)                                                 /* PAL_CPPI_PP_QMGR_G2_Q318 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q319)                                                 /* PAL_CPPI_PP_QMGR_G2_Q319 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q320)                                                 /* PAL_CPPI_PP_QMGR_G2_Q320 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q321)                                                 /* PAL_CPPI_PP_QMGR_G2_Q321 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q322)                                                 /* PAL_CPPI_PP_QMGR_G2_Q322 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q323)                                                 /* PAL_CPPI_PP_QMGR_G2_Q323 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q324)                                                 /* PAL_CPPI_PP_QMGR_G2_Q324 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q325)                                                 /* PAL_CPPI_PP_QMGR_G2_Q325 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q326)                                                 /* PAL_CPPI_PP_QMGR_G2_Q326 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE0_HOST_RX_HI_Q_NUM)      /* PAL_CPPI_PP_QMGR_G2_Q327 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE0_HOST_RX_LOW_Q_NUM)     /* PAL_CPPI_PP_QMGR_G2_Q328 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE1_HOST_RX_HI_Q_NUM)      /* PAL_CPPI_PP_QMGR_G2_Q329 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE1_HOST_RX_LOW_Q_NUM)     /* PAL_CPPI_PP_QMGR_G2_Q330 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE2_HOST_RX_HI_Q_NUM)      /* PAL_CPPI_PP_QMGR_G2_Q331 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE2_HOST_RX_LOW_Q_NUM)     /* PAL_CPPI_PP_QMGR_G2_Q332 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE3_HOST_RX_Hi_Q_NUM)      /* PAL_CPPI_PP_QMGR_G2_Q333 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_CDMA8_INFRA_INPUT_WIFI_DEVICE3_HOST_RX_LOW_Q_NUM)     /* PAL_CPPI_PP_QMGR_G2_Q334 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q335)                                                 /* PAL_CPPI_PP_QMGR_G2_Q335 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q336)                                                 /* PAL_CPPI_PP_QMGR_G2_Q336 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q337)                                                 /* PAL_CPPI_PP_QMGR_G2_Q337 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q338)                                                 /* PAL_CPPI_PP_QMGR_G2_Q338 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q339)                                                 /* PAL_CPPI_PP_QMGR_G2_Q339 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q340)                                                 /* PAL_CPPI_PP_QMGR_G2_Q340 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q341)                                                 /* PAL_CPPI_PP_QMGR_G2_Q341 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q342)                                                 /* PAL_CPPI_PP_QMGR_G2_Q342 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q343)                                                 /* PAL_CPPI_PP_QMGR_G2_Q343 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PP_EVENTS_Q0)                                         /* PAL_CPPI_PP_QMGR_G2_Q344 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PP_EVENTS_Q1)                                         /* PAL_CPPI_PP_QMGR_G2_Q345 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PP_EVENTS_Q2)                                         /* PAL_CPPI_PP_QMGR_G2_Q346 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_PP_EVENTS_Q3)                                         /* PAL_CPPI_PP_QMGR_G2_Q347 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q0)                                   /* PAL_CPPI_PP_QMGR_G2_Q348 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q1)                                   /* PAL_CPPI_PP_QMGR_G2_Q349 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q2)                                   /* PAL_CPPI_PP_QMGR_G2_Q350 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q3)                                   /* PAL_CPPI_PP_QMGR_G2_Q351 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q4)                                   /* PAL_CPPI_PP_QMGR_G2_Q352 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q5)                                   /* PAL_CPPI_PP_QMGR_G2_Q353 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q6)                                   /* PAL_CPPI_PP_QMGR_G2_Q354 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q7)                                   /* PAL_CPPI_PP_QMGR_G2_Q355 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q8)                                   /* PAL_CPPI_PP_QMGR_G2_Q356 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q9)                                   /* PAL_CPPI_PP_QMGR_G2_Q357 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q10)                                  /* PAL_CPPI_PP_QMGR_G2_Q358 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q11)                                  /* PAL_CPPI_PP_QMGR_G2_Q359 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q12)                                  /* PAL_CPPI_PP_QMGR_G2_Q360 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q13)                                  /* PAL_CPPI_PP_QMGR_G2_Q361 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q14)                                  /* PAL_CPPI_PP_QMGR_G2_Q362 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q15)                                  /* PAL_CPPI_PP_QMGR_G2_Q363 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q16)                                  /* PAL_CPPI_PP_QMGR_G2_Q364 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q17)                                  /* PAL_CPPI_PP_QMGR_G2_Q365 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q18)                                  /* PAL_CPPI_PP_QMGR_G2_Q366 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q19)                                  /* PAL_CPPI_PP_QMGR_G2_Q367 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q20)                                  /* PAL_CPPI_PP_QMGR_G2_Q368 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q21)                                  /* PAL_CPPI_PP_QMGR_G2_Q369 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q22)                                  /* PAL_CPPI_PP_QMGR_G2_Q370 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q23)                                  /* PAL_CPPI_PP_QMGR_G2_Q371 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q24)                                  /* PAL_CPPI_PP_QMGR_G2_Q372 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q25)                                  /* PAL_CPPI_PP_QMGR_G2_Q373 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q26)                                  /* PAL_CPPI_PP_QMGR_G2_Q374 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q27)                                  /* PAL_CPPI_PP_QMGR_G2_Q375 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q28)                                  /* PAL_CPPI_PP_QMGR_G2_Q376 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q29)                                  /* PAL_CPPI_PP_QMGR_G2_Q377 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q30)                                  /* PAL_CPPI_PP_QMGR_G2_Q378 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q31)                                  /* PAL_CPPI_PP_QMGR_G2_Q379 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q32)                                  /* PAL_CPPI_PP_QMGR_G2_Q380 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q33)                                  /* PAL_CPPI_PP_QMGR_G2_Q381 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q34)                                  /* PAL_CPPI_PP_QMGR_G2_Q382 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q35)                                  /* PAL_CPPI_PP_QMGR_G2_Q383 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q36)                                  /* PAL_CPPI_PP_QMGR_G2_Q384 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q37)                                  /* PAL_CPPI_PP_QMGR_G2_Q385 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q38)                                  /* PAL_CPPI_PP_QMGR_G2_Q386 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q39)                                  /* PAL_CPPI_PP_QMGR_G2_Q387 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q40)                                  /* PAL_CPPI_PP_QMGR_G2_Q388 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q41)                                  /* PAL_CPPI_PP_QMGR_G2_Q389 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q42)                                  /* PAL_CPPI_PP_QMGR_G2_Q390 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q43)                                  /* PAL_CPPI_PP_QMGR_G2_Q391 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q44)                                  /* PAL_CPPI_PP_QMGR_G2_Q392 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q45)                                  /* PAL_CPPI_PP_QMGR_G2_Q393 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q46)                                  /* PAL_CPPI_PP_QMGR_G2_Q394 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q47)                                  /* PAL_CPPI_PP_QMGR_G2_Q395 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q48)                                  /* PAL_CPPI_PP_QMGR_G2_Q396 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q49)                                  /* PAL_CPPI_PP_QMGR_G2_Q397 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q50)                                  /* PAL_CPPI_PP_QMGR_G2_Q398 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q51)                                  /* PAL_CPPI_PP_QMGR_G2_Q399 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q52)                                  /* PAL_CPPI_PP_QMGR_G2_Q400 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q53)                                  /* PAL_CPPI_PP_QMGR_G2_Q401 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q54)                                  /* PAL_CPPI_PP_QMGR_G2_Q402 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q55)                                  /* PAL_CPPI_PP_QMGR_G2_Q403 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q56)                                  /* PAL_CPPI_PP_QMGR_G2_Q404 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q57)                                  /* PAL_CPPI_PP_QMGR_G2_Q405 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q58)                                  /* PAL_CPPI_PP_QMGR_G2_Q406 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q59)                                  /* PAL_CPPI_PP_QMGR_G2_Q407 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q60)                                  /* PAL_CPPI_PP_QMGR_G2_Q408 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q61)                                  /* PAL_CPPI_PP_QMGR_G2_Q409 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q62)                                  /* PAL_CPPI_PP_QMGR_G2_Q410 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MULTICAST_CACHE_Q63)                                  /* PAL_CPPI_PP_QMGR_G2_Q411 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q412)                                                 /* PAL_CPPI_PP_QMGR_G2_Q412 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q413)                                                 /* PAL_CPPI_PP_QMGR_G2_Q413 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q414)                                                 /* PAL_CPPI_PP_QMGR_G2_Q414 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q415)                                                 /* PAL_CPPI_PP_QMGR_G2_Q415 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q416)                                                 /* PAL_CPPI_PP_QMGR_G2_Q416 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q417)                                                 /* PAL_CPPI_PP_QMGR_G2_Q417 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q418)                                                 /* PAL_CPPI_PP_QMGR_G2_Q418 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q419)                                                 /* PAL_CPPI_PP_QMGR_G2_Q419 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_IPSEC_MONOLITHIC_FD_Q_NUM)                            /* PAL_CPPI_PP_QMGR_G2_Q420 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_IPSEC_HOST_FD_Q_NUM)                                  /* PAL_CPPI_PP_QMGR_G2_Q421 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_FCC_STREAMING_FD_Q_NUM)                               /* PAL_CPPI_PP_QMGR_G2_Q422 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_LOW_INFRA_HOST_FD_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q423 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_HI_INFRA_HOST_FD_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q424 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SHARED_LOW_INFRA_HOST_FD_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q425 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ATOM_HI_INFRA_HOST_FD_Q_NUM)                          /* PAL_CPPI_PP_QMGR_G2_Q426 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII0_HI_INFRA_HOST_FD_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q427 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII1_HI_INFRA_HOST_FD_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q428 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII0_HI_INFRA_HOST_FD_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q429 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII1_HI_INFRA_HOST_FD_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q430 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_WIFI_INFRA_HOST_RX_FD_Q_NUM)                          /* PAL_CPPI_PP_QMGR_G2_Q431 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_NP2APP_HOST_FD_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q432 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_PPINFO_HOST_FD_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q433 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_DATA_HOST_FD_Q_NUM)                      /* PAL_CPPI_PP_QMGR_G2_Q434 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_HOST_FD_Q_NUM)                                 /* PAL_CPPI_PP_QMGR_G2_Q435 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_PPINFO_HOST_FD_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q436 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_DATA_HOST_FD_Q_NUM)                      /* PAL_CPPI_PP_QMGR_G2_Q437 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_FD_Q15)                                          /* PAL_CPPI_PP_QMGR_G2_Q438 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_FD_Q16)                                          /* PAL_CPPI_PP_QMGR_G2_Q439 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_FD_Q17)                                          /* PAL_CPPI_PP_QMGR_G2_Q440 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_FD_Q18)                                          /* PAL_CPPI_PP_QMGR_G2_Q441 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_TX_MGMT_HOST_FD_Q_NUM)                         /* PAL_CPPI_PP_QMGR_G2_Q442 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_LOW_HOST_FD_Q_NUM)                            /* PAL_CPPI_PP_QMGR_G2_Q443 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_HI_HOST_FD_Q_NUM)                             /* PAL_CPPI_PP_QMGR_G2_Q444 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_FD_Q22)                                          /* PAL_CPPI_PP_QMGR_G2_Q445 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST_FD_Q23)                                          /* PAL_CPPI_PP_QMGR_G2_Q446 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM)          /* PAL_CPPI_PP_QMGR_G2_Q447 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SHARED_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM)           /* PAL_CPPI_PP_QMGR_G2_Q448 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SHARED_DDR_SHORT_RX_LOW)                              /* PAL_CPPI_PP_QMGR_G2_Q449 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SHARED_DDR_LONG_RX_LOW)                               /* PAL_CPPI_PP_QMGR_G2_Q450 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SHARED_DDR_XLONG_RX_LOW)                              /* PAL_CPPI_PP_QMGR_G2_Q451 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_DESC_PACKET_RAM_BUFF_DDR_RX_LOW_EMB_FD_Q_NUM)  /* PAL_CPPI_PP_QMGR_G2_Q452 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_SHORT_RX_LOW_EMB_FD_Q_NUM)          /* PAL_CPPI_PP_QMGR_G2_Q453 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_PACKET_RAM_LONG_RX_LOW_EMB_FD_Q_NUM)           /* PAL_CPPI_PP_QMGR_G2_Q454 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_HI_EMB_FD_Q_NUM)                            /* PAL_CPPI_PP_QMGR_G2_Q455 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_DOCSIS_RX_MGMT_HOST_FD_Q_NUM)                         /* PAL_CPPI_PP_QMGR_G2_Q456 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM)                /* PAL_CPPI_PP_QMGR_G2_Q457 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MoCA_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q458 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MoCA_DDR_SHORT_RX_EMB_FD_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q459 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_MoCA_DDR_LONG_RX_EMB_FD_Q_NUM)                        /* PAL_CPPI_PP_QMGR_G2_Q460 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM)                /* PAL_CPPI_PP_QMGR_G2_Q461 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ATOM_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q462 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ATOM_DDR_SHORT_RX_EMB_FD_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q463 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_LOW_EMB_FD_Q_NUM)                    /* PAL_CPPI_PP_QMGR_G2_Q464 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_ATOM_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM)                   /* PAL_CPPI_PP_QMGR_G2_Q465 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q466 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q467 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q468 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q469 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q470 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q471 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q472 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q473 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q474 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_RGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q475 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q476 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII0_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q477 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_SHORT_RX_EMB_FD_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q478 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_LOW_EMB_FD_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q479 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII0_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q480 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_SHORT_RX_EMB_FD_Q_NUM)              /* PAL_CPPI_PP_QMGR_G2_Q481 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII1_PACKET_RAM_LONG_RX_EMB_FD_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q482 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_SHORT_RX_EMB_FD_Q_NUM)                     /* PAL_CPPI_PP_QMGR_G2_Q483 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_LOW_EMB_FD_Q_NUM)                  /* PAL_CPPI_PP_QMGR_G2_Q484 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_SGMII1_DDR_LONG_RX_HIGH_EMB_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q485 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_LOW_EMB_FD_Q_NUM)                       /* PAL_CPPI_PP_QMGR_G2_Q486 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_HOST2PP_INFRA_HIGH_EMB_FD_Q_NUM)                      /* PAL_CPPI_PP_QMGR_G2_Q487 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q488)                                                 /* PAL_CPPI_PP_QMGR_G2_Q488 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_WIFI_FW_RX_EMB_FD_Q_NUM)                              /* PAL_CPPI_PP_QMGR_G2_Q489 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_VOICE_DSP_RX_EMB_FD_Q_NUM)                            /* PAL_CPPI_PP_QMGR_G2_Q490 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_VOICE_INFRA_RX_EMB_FD_Q_NUM)                          /* PAL_CPPI_PP_QMGR_G2_Q491 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_WIFI_FW_RX_REASSEMBLY_EMB_FD_Q_NUM)                   /* PAL_CPPI_PP_QMGR_G2_Q492 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q46)                                           /* PAL_CPPI_PP_QMGR_G2_Q493 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q47)                                           /* PAL_CPPI_PP_QMGR_G2_Q494 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q48)                                           /* PAL_CPPI_PP_QMGR_G2_Q495 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q49)                                           /* PAL_CPPI_PP_QMGR_G2_Q496 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q50)                                           /* PAL_CPPI_PP_QMGR_G2_Q497 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q51)                                           /* PAL_CPPI_PP_QMGR_G2_Q498 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_CTRL_PPINFO_HOST_FD_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q499 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_APP2NP_INFRA_CTRL_DATA_HOST_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q500 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_CTRL_PPINFO_HOST_FD_Q_NUM)               /* PAL_CPPI_PP_QMGR_G2_Q501 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_NP2APP_INFRA_CTRL_DATA_HOST_FD_Q_NUM)                 /* PAL_CPPI_PP_QMGR_G2_Q502 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q56)                                           /* PAL_CPPI_PP_QMGR_G2_Q503 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q57)                                           /* PAL_CPPI_PP_QMGR_G2_Q504 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q58)                                           /* PAL_CPPI_PP_QMGR_G2_Q505 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q59)                                           /* PAL_CPPI_PP_QMGR_G2_Q506 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q60)                                           /* PAL_CPPI_PP_QMGR_G2_Q507 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q61)                                           /* PAL_CPPI_PP_QMGR_G2_Q508 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_EMB_FD_Q62)                                           /* PAL_CPPI_PP_QMGR_G2_Q509 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_TEARDOWN_FD_Q_NUM)                                    /* PAL_CPPI_PP_QMGR_G2_Q510 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_G2_Q511)                                                 /* PAL_CPPI_PP_QMGR_G2_Q511 */
