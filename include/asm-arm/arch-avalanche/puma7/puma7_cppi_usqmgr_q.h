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

#define PAL_CPPI_MAC_US_QMGR_DOCSIS_Q_LIST      \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_0_Q_HIGH_ID )               /*   Q0     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_0_Q_LOW_ID  )               /*   Q1     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_1_Q_HIGH_ID )               /*   Q2     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_1_Q_LOW_ID  )               /*   Q3     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_2_Q_HIGH_ID )               /*   Q4     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_2_Q_LOW_ID  )               /*   Q5     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_3_Q_HIGH_ID )               /*   Q6     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_3_Q_LOW_ID  )               /*   Q7     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_4_Q_HIGH_ID )               /*   Q8     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_4_Q_LOW_ID  )               /*   Q9     */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_5_Q_HIGH_ID )               /*   Q10    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_5_Q_LOW_ID  )               /*   Q11    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_6_Q_HIGH_ID )               /*   Q12    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_6_Q_LOW_ID  )               /*   Q13    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_7_Q_HIGH_ID )               /*   Q14    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_7_Q_LOW_ID  )               /*   Q15    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_8_Q_HIGH_ID )               /*   Q16    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_8_Q_LOW_ID  )               /*   Q17    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_9_Q_HIGH_ID )               /*   Q18    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_9_Q_LOW_ID  )               /*   Q19    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_10_Q_HIGH_ID )              /*   Q20    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_10_Q_LOW_ID  )              /*   Q21    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_11_Q_HIGH_ID )              /*   Q22    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_11_Q_LOW_ID  )              /*   Q23    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_12_Q_HIGH_ID )              /*   Q24    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_12_Q_LOW_ID  )              /*   Q25    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_13_Q_HIGH_ID )              /*   Q26    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_13_Q_LOW_ID  )              /*   Q27    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_14_Q_HIGH_ID )              /*   Q28    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_14_Q_LOW_ID  )              /*   Q29    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_15_Q_HIGH_ID )              /*   Q30    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_15_Q_LOW_ID  )              /*   Q31    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_16_Q_HIGH_ID )              /*   Q32    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_16_Q_LOW_ID  )              /*   Q33    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_17_Q_HIGH_ID )              /*   Q34    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_17_Q_LOW_ID  )              /*   Q35    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_18_Q_HIGH_ID )              /*   Q36    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_18_Q_LOW_ID  )              /*   Q37    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_19_Q_HIGH_ID )              /*   Q38    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_19_Q_LOW_ID  )              /*   Q39    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_20_Q_HIGH_ID )              /*   Q40    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_20_Q_LOW_ID  )              /*   Q41    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_21_Q_HIGH_ID )              /*   Q42    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_21_Q_LOW_ID  )              /*   Q43    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_22_Q_HIGH_ID )              /*   Q44    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_22_Q_LOW_ID  )              /*   Q45    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_23_Q_HIGH_ID )              /*   Q46    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_23_Q_LOW_ID  )              /*   Q47    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_24_Q_HIGH_ID )              /*   Q48    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_24_Q_LOW_ID  )              /*   Q49    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_25_Q_HIGH_ID )              /*   Q50    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_25_Q_LOW_ID  )              /*   Q51    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_26_Q_HIGH_ID )              /*   Q52    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_26_Q_LOW_ID  )              /*   Q53    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_27_Q_HIGH_ID )              /*   Q54    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_27_Q_LOW_ID  )              /*   Q55    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_28_Q_HIGH_ID )              /*   Q56    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_28_Q_LOW_ID  )              /*   Q57    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_29_Q_HIGH_ID )              /*   Q58    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_29_Q_LOW_ID  )              /*   Q59    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_30_Q_HIGH_ID )              /*   Q60    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_30_Q_LOW_ID  )              /*   Q61    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_31_Q_HIGH_ID )              /*   Q62    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_31_Q_LOW_ID  )              /*   Q63    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_32_Q_HIGH_ID )              /*   Q64    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_32_Q_LOW_ID  )              /*   Q65    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_33_Q_HIGH_ID )              /*   Q66    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_33_Q_LOW_ID  )              /*   Q67    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_34_Q_HIGH_ID )              /*   Q68    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_34_Q_LOW_ID  )              /*   Q69    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_35_Q_HIGH_ID )              /*   Q70    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_35_Q_LOW_ID  )              /*   Q71    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_36_Q_HIGH_ID )              /*   Q72    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_36_Q_LOW_ID  )              /*   Q73    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_37_Q_HIGH_ID )              /*   Q74    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_37_Q_LOW_ID  )              /*   Q75    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_38_Q_HIGH_ID )              /*   Q76    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_38_Q_LOW_ID  )              /*   Q77    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_39_Q_HIGH_ID )              /*   Q78    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_39_Q_LOW_ID  )              /*   Q79    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_40_Q_HIGH_ID )              /*   Q80    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_40_Q_LOW_ID  )              /*   Q81    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_41_Q_HIGH_ID )              /*   Q82    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_41_Q_LOW_ID  )              /*   Q83    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_42_Q_HIGH_ID )              /*   Q84    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_42_Q_LOW_ID  )              /*   Q85    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_43_Q_HIGH_ID )              /*   Q86    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_43_Q_LOW_ID  )              /*   Q87    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_44_Q_HIGH_ID )              /*   Q88    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_44_Q_LOW_ID  )              /*   Q89    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_45_Q_HIGH_ID )              /*   Q90    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_45_Q_LOW_ID  )              /*   Q91    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_46_Q_HIGH_ID )              /*   Q92    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_46_Q_LOW_ID  )              /*   Q93    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_47_Q_HIGH_ID )              /*   Q94    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_47_Q_LOW_ID  )              /*   Q95    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_48_Q_HIGH_ID )              /*   Q96    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_48_Q_LOW_ID  )              /*   Q97    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_49_Q_HIGH_ID )              /*   Q98    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_49_Q_LOW_ID  )              /*   Q99    */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_50_Q_HIGH_ID )              /*   Q100   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_50_Q_LOW_ID  )              /*   Q101   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_51_Q_HIGH_ID )              /*   Q102   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_51_Q_LOW_ID  )              /*   Q103   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_52_Q_HIGH_ID )              /*   Q104   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_52_Q_LOW_ID  )              /*   Q105   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_53_Q_HIGH_ID )              /*   Q106   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_53_Q_LOW_ID  )              /*   Q107   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_54_Q_HIGH_ID )              /*   Q108   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_54_Q_LOW_ID  )              /*   Q109   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_55_Q_HIGH_ID )              /*   Q110   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_55_Q_LOW_ID  )              /*   Q111   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_56_Q_HIGH_ID )              /*   Q112   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_56_Q_LOW_ID  )              /*   Q113   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_57_Q_HIGH_ID )              /*   Q114   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_57_Q_LOW_ID  )              /*   Q115   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_58_Q_HIGH_ID )              /*   Q116   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_58_Q_LOW_ID  )              /*   Q117   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_59_Q_HIGH_ID )              /*   Q118   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_59_Q_LOW_ID  )              /*   Q119   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_60_Q_HIGH_ID )              /*   Q120   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_60_Q_LOW_ID  )              /*   Q121   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_61_Q_HIGH_ID )              /*   Q122   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_61_Q_LOW_ID  )              /*   Q123   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_62_Q_HIGH_ID )              /*   Q124   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_62_Q_LOW_ID  )              /*   Q125   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_63_Q_HIGH_ID )              /*   Q126   */ \
PAL_CPPI_QMGR_Q_ADD( OUTSTANDING_SF_63_Q_LOW_ID  )              /*   Q127   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE0_Q_MGMT_ID    )              /*   Q128   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE0_Q_TDOX_ID    )              /*   Q129   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE0_Q_HIGH_ID    )              /*   Q130   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE0_Q_LOW_ID     )              /*   Q131   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE1_Q_MGMT_ID    )              /*   Q132   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE1_Q_TDOX_ID    )              /*   Q133   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE1_Q_HIGH_ID    )              /*   Q134   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE1_Q_LOW_ID     )              /*   Q135   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE2_Q_MGMT_ID    )              /*   Q136   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE2_Q_TDOX_ID    )              /*   Q137   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE2_Q_HIGH_ID    )              /*   Q138   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE2_Q_LOW_ID     )              /*   Q139   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE3_Q_MGMT_ID    )              /*   Q140   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE3_Q_TDOX_ID    )              /*   Q141   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE3_Q_HIGH_ID    )              /*   Q142   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE3_Q_LOW_ID     )              /*   Q143   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE4_Q_MGMT_ID    )              /*   Q144   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE4_Q_TDOX_ID    )              /*   Q145   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE4_Q_HIGH_ID    )              /*   Q146   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE4_Q_LOW_ID     )              /*   Q147   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE5_Q_MGMT_ID    )              /*   Q148   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE5_Q_TDOX_ID    )              /*   Q149   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE5_Q_HIGH_ID    )              /*   Q150   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE5_Q_LOW_ID     )              /*   Q151   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE6_Q_MGMT_ID    )              /*   Q152   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE6_Q_TDOX_ID    )              /*   Q153   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE6_Q_HIGH_ID    )              /*   Q154   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE6_Q_LOW_ID     )              /*   Q155   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE7_Q_MGMT_ID    )              /*   Q156   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE7_Q_TDOX_ID    )              /*   Q157   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE7_Q_HIGH_ID    )              /*   Q158   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE7_Q_LOW_ID     )              /*   Q159   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE8_Q_MGMT_ID    )              /*   Q160   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE8_Q_TDOX_ID    )              /*   Q161   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE8_Q_HIGH_ID    )              /*   Q162   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE8_Q_LOW_ID     )              /*   Q163   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE9_Q_MGMT_ID    )              /*   Q164   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE9_Q_TDOX_ID    )              /*   Q165   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE9_Q_HIGH_ID    )              /*   Q166   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE9_Q_LOW_ID     )              /*   Q167   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE10_Q_MGMT_ID   )              /*   Q168   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE10_Q_TDOX_ID   )              /*   Q169   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE10_Q_HIGH_ID   )              /*   Q170   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE10_Q_LOW_ID    )              /*   Q171   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE11_Q_MGMT_ID   )              /*   Q172   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE11_Q_TDOX_ID   )              /*   Q173   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE11_Q_HIGH_ID   )              /*   Q174   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE11_Q_LOW_ID    )              /*   Q175   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE12_Q_MGMT_ID   )              /*   Q176   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE12_Q_TDOX_ID   )              /*   Q177   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE12_Q_HIGH_ID   )              /*   Q178   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE12_Q_LOW_ID    )              /*   Q179   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE13_Q_MGMT_ID   )              /*   Q180   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE13_Q_TDOX_ID   )              /*   Q181   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE13_Q_HIGH_ID   )              /*   Q182   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE13_Q_LOW_ID    )              /*   Q183   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE14_Q_MGMT_ID   )              /*   Q184   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE14_Q_TDOX_ID   )              /*   Q185   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE14_Q_HIGH_ID   )              /*   Q186   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE14_Q_LOW_ID    )              /*   Q187   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE15_Q_MGMT_ID   )              /*   Q188   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE15_Q_TDOX_ID   )              /*   Q189   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE15_Q_HIGH_ID   )              /*   Q190   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_BE15_Q_LOW_ID    )              /*   Q191   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS0_Q_ID        )              /*   Q192   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS1_Q_ID        )              /*   Q193   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS2_Q_ID        )              /*   Q194   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS3_Q_ID        )              /*   Q195   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS4_Q_ID        )              /*   Q196   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS5_Q_ID        )              /*   Q197   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS6_Q_ID        )              /*   Q198   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS7_Q_ID        )              /*   Q199   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS8_Q_ID        )              /*   Q200   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS9_Q_ID        )              /*   Q201   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS10_Q_ID       )              /*   Q202   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS11_Q_ID       )              /*   Q203   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS12_Q_ID       )              /*   Q204   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS13_Q_ID       )              /*   Q205   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS14_Q_ID       )              /*   Q206   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS15_Q_ID       )              /*   Q207   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS16_Q_ID       )              /*   Q208   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS17_Q_ID       )              /*   Q209   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS18_Q_ID       )              /*   Q210   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS19_Q_ID       )              /*   Q211   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS20_Q_ID       )              /*   Q212   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS21_Q_ID       )              /*   Q213   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS22_Q_ID       )              /*   Q214   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS23_Q_ID       )              /*   Q215   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS24_Q_ID       )              /*   Q216   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS25_Q_ID       )              /*   Q217   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS26_Q_ID       )              /*   Q218   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS27_Q_ID       )              /*   Q219   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS28_Q_ID       )              /*   Q220   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS29_Q_ID       )              /*   Q221   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS30_Q_ID       )              /*   Q222   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS31_Q_ID       )              /*   Q223   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS32_Q_ID       )              /*   Q224   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS33_Q_ID       )              /*   Q225   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS34_Q_ID       )              /*   Q226   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS35_Q_ID       )              /*   Q227   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS36_Q_ID       )              /*   Q228   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS37_Q_ID       )              /*   Q229   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS38_Q_ID       )              /*   Q230   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS39_Q_ID       )              /*   Q231   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS40_Q_ID       )              /*   Q232   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS41_Q_ID       )              /*   Q233   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS42_Q_ID       )              /*   Q234   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS43_Q_ID       )              /*   Q235   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS44_Q_ID       )              /*   Q236   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS45_Q_ID       )              /*   Q237   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS46_Q_ID       )              /*   Q238   */ \
PAL_CPPI_QMGR_Q_ADD( RX_CO_PROC_UGS47_Q_ID       )              /*   Q239   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_0         )              /*   Q240   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_1         )              /*   Q241   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_2         )              /*   Q242   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_3         )              /*   Q243   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_4         )              /*   Q244   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_5         )              /*   Q245   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_6         )              /*   Q246   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_7         )              /*   Q247   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_8         )              /*   Q248   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_9         )              /*   Q249   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_10        )              /*   Q250   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_11        )              /*   Q251   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_12        )              /*   Q252   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_13        )              /*   Q253   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_14        )              /*   Q254   */ \
PAL_CPPI_QMGR_Q_ADD( US_GRANT_QUEUE_CH_15        )              /*   Q255   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM0_ID              )              /*   Q256   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM1_ID              )              /*   Q257   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM2_ID              )              /*   Q258   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM3_ID              )              /*   Q259   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_OFDMA0_ID            )              /*   Q260   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM4_ID              )              /*   Q261   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM5_ID              )              /*   Q262   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM6_ID              )              /*   Q263   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_QAM7_ID              )              /*   Q264   */ \
PAL_CPPI_QMGR_Q_ADD( TX_MAC_OFDMA1_ID            )              /*   Q265   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q266_ID         )              /*   Q266   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q267_ID         )              /*   Q267   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q268_ID         )              /*   Q268   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q269_ID         )              /*   Q269   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q270_ID         )              /*   Q270   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q271_ID         )              /*   Q271   */ \
PAL_CPPI_QMGR_Q_ADD( FRAG_DIVERT_TMP_Q_ID        )              /*   Q272   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC_FD_EMB_ID            )              /*   Q273   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC_FD_DLS_MONOLITIC_ID  )              /*   Q274   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC_FD_MONOLITIC_ID      )              /*   Q275   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q276_ID         )              /*   Q276   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q277_ID         )              /*   Q277   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q278_ID         )              /*   Q278   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q279_ID         )              /*   Q279   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC_DLS_COMMAND_FROM_EXT_APP_ID )       /*   Q280   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q281_ID         )              /*   Q281   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q282_ID         )              /*   Q282   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q283_ID         )              /*   Q283   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q284_ID         )              /*   Q284   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q285_ID         )              /*   Q285   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q286_ID         )              /*   Q286   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q287_ID         )              /*   Q287   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q288_ID         )              /*   Q288   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q289_ID         )              /*   Q289   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q290_ID         )              /*   Q290   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q291_ID         )              /*   Q291   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q292_ID         )              /*   Q292   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q293_ID         )              /*   Q293   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q294_ID         )              /*   Q294   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q295_ID         )              /*   Q295   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q296_ID         )              /*   Q296   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q297_ID         )              /*   Q297   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q298_ID         )              /*   Q298   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q299_ID         )              /*   Q299   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q300_ID         )              /*   Q300   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q301_ID         )              /*   Q301   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q302_ID         )              /*   Q302   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q303_ID         )              /*   Q303   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q304_ID         )              /*   Q304   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q305_ID         )              /*   Q305   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q306_ID         )              /*   Q306   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q307_ID         )              /*   Q307   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q308_ID         )              /*   Q308   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q309_ID         )              /*   Q309   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q310_ID         )              /*   Q310   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q311_ID         )              /*   Q311   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q312_ID         )              /*   Q312   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q313_ID         )              /*   Q313   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q314_ID         )              /*   Q314   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q315_ID         )              /*   Q315   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q316_ID         )              /*   Q316   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q317_ID         )              /*   Q317   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q318_ID         )              /*   Q318   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q319_ID         )              /*   Q319   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q320_ID         )              /*   Q320   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q321_ID         )              /*   Q321   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q322_ID         )              /*   Q322   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q323_ID         )              /*   Q323   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q324_ID         )              /*   Q324   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q325_ID         )              /*   Q325   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q326_ID         )              /*   Q326   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q327_ID         )              /*   Q327   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q328_ID         )              /*   Q328   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q329_ID         )              /*   Q329   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q330_ID         )              /*   Q330   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q331_ID         )              /*   Q331   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q332_ID         )              /*   Q332   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q333_ID         )              /*   Q333   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q334_ID         )              /*   Q334   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q335_ID         )              /*   Q335   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q336_ID         )              /*   Q336   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q337_ID         )              /*   Q337   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q338_ID         )              /*   Q338   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q339_ID         )              /*   Q339   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q340_ID         )              /*   Q340   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q341_ID         )              /*   Q341   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q342_ID         )              /*   Q342   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q343_ID         )              /*   Q343   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q344_ID         )              /*   Q344   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q345_ID         )              /*   Q345   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q346_ID         )              /*   Q346   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q347_ID         )              /*   Q347   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q348_ID         )              /*   Q348   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q349_ID         )              /*   Q349   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q350_ID         )              /*   Q350   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q351_ID         )              /*   Q351   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q352_ID         )              /*   Q352   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q353_ID         )              /*   Q353   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q354_ID         )              /*   Q354   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q355_ID         )              /*   Q355   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q356_ID         )              /*   Q356   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q357_ID         )              /*   Q357   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q358_ID         )              /*   Q358   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q359_ID         )              /*   Q359   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q360_ID         )              /*   Q360   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q361_ID         )              /*   Q361   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q362_ID         )              /*   Q362   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q363_ID         )              /*   Q363   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q364_ID         )              /*   Q364   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q365_ID         )              /*   Q365   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q366_ID         )              /*   Q366   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q367_ID         )              /*   Q367   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q368_ID         )              /*   Q368   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q369_ID         )              /*   Q369   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q370_ID         )              /*   Q370   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q371_ID         )              /*   Q371   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q372_ID         )              /*   Q372   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q373_ID         )              /*   Q373   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q374_ID         )              /*   Q374   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q375_ID         )              /*   Q375   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q376_ID         )              /*   Q376   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q377_ID         )              /*   Q377   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q378_ID         )              /*   Q378   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q379_ID         )              /*   Q379   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q380_ID         )              /*   Q380   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q381_ID         )              /*   Q381   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q382_ID         )              /*   Q382   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q383_ID         )              /*   Q383   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q384_ID         )              /*   Q384   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q385_ID         )              /*   Q385   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q386_ID         )              /*   Q386   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q387_ID         )              /*   Q387   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q388_ID         )              /*   Q388   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q389_ID         )              /*   Q389   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q390_ID         )              /*   Q390   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q391_ID         )              /*   Q391   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q392_ID         )              /*   Q392   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q393_ID         )              /*   Q393   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q394_ID         )              /*   Q394   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q395_ID         )              /*   Q395   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q396_ID         )              /*   Q396   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q397_ID         )              /*   Q397   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q398_ID         )              /*   Q398   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q399_ID         )              /*   Q399   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q400_ID         )              /*   Q400   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q401_ID         )              /*   Q401   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q402_ID         )              /*   Q402   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q403_ID         )              /*   Q403   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q404_ID         )              /*   Q404   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q405_ID         )              /*   Q405   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q406_ID         )              /*   Q406   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q407_ID         )              /*   Q407   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q408_ID         )              /*   Q408   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q409_ID         )              /*   Q409   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q410_ID         )              /*   Q410   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q411_ID         )              /*   Q411   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q412_ID         )              /*   Q412   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q413_ID         )              /*   Q413   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q414_ID         )              /*   Q414   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q415_ID         )              /*   Q415   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q416_ID         )              /*   Q416   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q417_ID         )              /*   Q417   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q418_ID         )              /*   Q418   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q419_ID         )              /*   Q419   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q420_ID         )              /*   Q420   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q421_ID         )              /*   Q421   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q422_ID         )              /*   Q422   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q423_ID         )              /*   Q423   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q424_ID         )              /*   Q424   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q425_ID         )              /*   Q425   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q426_ID         )              /*   Q426   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q427_ID         )              /*   Q427   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q428_ID         )              /*   Q428   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q429_ID         )              /*   Q429   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q430_ID         )              /*   Q430   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q431_ID         )              /*   Q431   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q432_ID         )              /*   Q432   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q433_ID         )              /*   Q433   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q434_ID         )              /*   Q434   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q435_ID         )              /*   Q435   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q436_ID         )              /*   Q436   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q437_ID         )              /*   Q437   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q438_ID         )              /*   Q438   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q439_ID         )              /*   Q439   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q440_ID         )              /*   Q440   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q441_ID         )              /*   Q441   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q442_ID         )              /*   Q442   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q443_ID         )              /*   Q443   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q444_ID         )              /*   Q444   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q445_ID         )              /*   Q445   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q446_ID         )              /*   Q446   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q447_ID         )              /*   Q447   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q448_ID         )              /*   Q448   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q449_ID         )              /*   Q449   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q450_ID         )              /*   Q450   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q451_ID         )              /*   Q451   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q452_ID         )              /*   Q452   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q453_ID         )              /*   Q453   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q454_ID         )              /*   Q454   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q455_ID         )              /*   Q455   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q456_ID         )              /*   Q456   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q457_ID         )              /*   Q457   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q458_ID         )              /*   Q458   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q459_ID         )              /*   Q459   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q460_ID         )              /*   Q460   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q461_ID         )              /*   Q461   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q462_ID         )              /*   Q462   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q463_ID         )              /*   Q463   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q464_ID         )              /*   Q464   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q465_ID         )              /*   Q465   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q466_ID         )              /*   Q466   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q467_ID         )              /*   Q467   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q468_ID         )              /*   Q468   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q469_ID         )              /*   Q469   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q470_ID         )              /*   Q470   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q471_ID         )              /*   Q471   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q472_ID         )              /*   Q472   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q473_ID         )              /*   Q473   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q474_ID         )              /*   Q474   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q475_ID         )              /*   Q475   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q476_ID         )              /*   Q476   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q477_ID         )              /*   Q477   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q478_ID         )              /*   Q478   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q479_ID         )              /*   Q479   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q480_ID         )              /*   Q480   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q481_ID         )              /*   Q481   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q482_ID         )              /*   Q482   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q483_ID         )              /*   Q483   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q484_ID         )              /*   Q484   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q485_ID         )              /*   Q485   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q486_ID         )              /*   Q486   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q487_ID         )              /*   Q487   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q488_ID         )              /*   Q488   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q489_ID         )              /*   Q489   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q490_ID         )              /*   Q490   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q491_ID         )              /*   Q491   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q492_ID         )              /*   Q492   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q493_ID         )              /*   Q493   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q494_ID         )              /*   Q494   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q495_ID         )              /*   Q495   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q496_ID         )              /*   Q496   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q497_ID         )              /*   Q497   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q498_ID         )              /*   Q498   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q499_ID         )              /*   Q499   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q500_ID         )              /*   Q500   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q501_ID         )              /*   Q501   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q502_ID         )              /*   Q502   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q503_ID         )              /*   Q503   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q504_ID         )              /*   Q504   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q505_ID         )              /*   Q505   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q506_ID         )              /*   Q506   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q507_ID         )              /*   Q507   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q508_ID         )              /*   Q508   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q509_ID         )              /*   Q509   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q510_ID         )              /*   Q510   */ \
PAL_CPPI_QMGR_Q_ADD( US_MAC______Q511_ID         )              /*   Q511   */
