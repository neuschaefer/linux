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

#define PAL_CPPI_PP_QMGR_LOCAL_Q_LIST                                                                                     \
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER1_LOW_Q_NUM)                          /* PAL_CPPI_PP_QMGR_LOCAL_Q0 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER1_MED_LOW_Q_NUM)                      /* PAL_CPPI_PP_QMGR_LOCAL_Q1 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER1_MED_HI_Q_NUM)                       /* PAL_CPPI_PP_QMGR_LOCAL_Q2 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER1_HI_Q_NUM)                           /* PAL_CPPI_PP_QMGR_LOCAL_Q3 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER2_LOW_Q_NUM)                          /* PAL_CPPI_PP_QMGR_LOCAL_Q4 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER2_MED_LOW_Q_NUM)                      /* PAL_CPPI_PP_QMGR_LOCAL_Q5 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER2_MED_HI_Q_NUM)                       /* PAL_CPPI_PP_QMGR_LOCAL_Q6 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_CLASSIFIER2_HI_Q_NUM)                           /* PAL_CPPI_PP_QMGR_LOCAL_Q7 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_MODIFIER_LOW_Q_NUM)                             /* PAL_CPPI_PP_QMGR_LOCAL_Q8 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_MODIFIER_MED_LOW_Q_NUM)                         /* PAL_CPPI_PP_QMGR_LOCAL_Q9 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_MODIFIER_MED_HI_Q_NUM)                          /* PAL_CPPI_PP_QMGR_LOCAL_Q10 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_MODIFIER_HI_Q_NUM)                              /* PAL_CPPI_PP_QMGR_LOCAL_Q11 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RECYCLER_LOW_Q_NUM)                             /* PAL_CPPI_PP_QMGR_LOCAL_Q12 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RECYCLER_HI_Q_NUM)                              /* PAL_CPPI_PP_QMGR_LOCAL_Q13 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_SESSION_CACHE_LOW_Q_NUM)                        /* PAL_CPPI_PP_QMGR_LOCAL_Q14 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_SESSION_CACHE_MED_LOW_Q_NUM)                    /* PAL_CPPI_PP_QMGR_LOCAL_Q15 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_SESSION_CACHE_MED_HI_Q_NUM)                     /* PAL_CPPI_PP_QMGR_LOCAL_Q16 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_SESSION_CACHE_HI_Q_NUM)                         /* PAL_CPPI_PP_QMGR_LOCAL_Q17 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_TurboDOX_LOW_Q_NUM)                             /* PAL_CPPI_PP_QMGR_LOCAL_Q18 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_TurboDOX_MED_LOW_Q_NUM)                         /* PAL_CPPI_PP_QMGR_LOCAL_Q19 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_TurboDOX_MED_HI_Q_NUM)                          /* PAL_CPPI_PP_QMGR_LOCAL_Q20 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_TurboDOX_HI_Q_NUM)                              /* PAL_CPPI_PP_QMGR_LOCAL_Q21 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_LOW_Q_NUM)                          /* PAL_CPPI_PP_QMGR_LOCAL_Q22 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_MED_LOW_Q_NUM)                      /* PAL_CPPI_PP_QMGR_LOCAL_Q23 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_MED_HI_Q_NUM)                       /* PAL_CPPI_PP_QMGR_LOCAL_Q24 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_HI_Q_NUM)                           /* PAL_CPPI_PP_QMGR_LOCAL_Q25 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_Q26)                                /* PAL_CPPI_PP_QMGR_LOCAL_Q26 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_Q27)                                /* PAL_CPPI_PP_QMGR_LOCAL_Q27 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_Q28)                                /* PAL_CPPI_PP_QMGR_LOCAL_Q28 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_RESEQUENCER_Q29)                                /* PAL_CPPI_PP_QMGR_LOCAL_Q29 */\
PAL_CPPI_QMGR_Q_ADD(PAL_CPPI_PP_QMGR_LOCAL_PP_INTERNAL_EMB_FD_Q_NUM)                       /* PAL_CPPI_PP_QMGR_LOCAL_Q30 */


