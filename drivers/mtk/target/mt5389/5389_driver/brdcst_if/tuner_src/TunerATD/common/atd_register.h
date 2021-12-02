/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
//  REGISTER.H  Register map include file
//

#ifndef _ATD_REGISTER_H_
#define _ATD_REGISTER_H_

/*********** MT5388/63/65 Register Addr. **************/

//DEMOD_ADC
#define REG_DEMODADC_0 0x110
#define REG_DEMODADC_1 0x111
#define REG_DEMODADC_2 0x112
#define REG_DEMODADC_3 0x113
#define REG_DEMODADC_4 0x114
#define REG_DEMODADC_5 0x115
#define REG_DEMODADC_6 0x116
#define REG_DEMODADC_7 0x117
#define REG_DEMODADC_8 0x118
#define REG_DEMODADC_9 0x119
#define REG_DEMODADC_A 0x11a
#define REG_DEMODADC_B 0x11b
#define REG_DEMODADC_C 0x11c

//CHIP_CTL
#define REG_CHIP_COMP_RST 0x0d0
#define REG_CHIP_PAD_1 0x0d2
#define REG_CHIP_FS_CFG 0x0d5
#define REG_CHIP_STAT 0x0d6
#define REG_CHIP_MPMD 0x0d8
#define REG_CHIP_ID 0x0d9
#define REG_CHIP_TEST_MODE 0x0da
#define REG_CHIP_MOD_RST 0x0db
#define REG_CHIP_MOD_REGRST 0x0dc
#define REG_CHIP_MISC_SEL 0x0dd
#define REG_CHIP_UPMEM_RST 0x0de
#define REG_CHIP_CLKPD 0x0df
#define REG_CHIP_QAMPD 0x0e5

//RISCIF_CTL
#define REG_RISCIF_CFG 0x4a4
#define REG_RISCIF_TO1 0x4a5
#define REG_RISCIF_TO2 0x4a6
#define REG_RISCIF_TO3 0x4a7
#define REG_RISCIF_PADR0 0x4b4
#define REG_RISCIF_PADR1 0x4b5
#define REG_RISCIF_PADR2 0x4b6
#define REG_RISCIF_PADR3 0x4b7
#define REG_RISCIF_PRDA_0 0x4b8
#define REG_RISCIF_PRDA_1 0x4b9
#define REG_RISCIF_PRDA_2 0x4ba
#define REG_RISCIF_PRDA_3 0x4bb
#define REG_RISCIFSTAT0 0x4bc
#define REG_RISCIF_STAT1 0x4bd
#define REG_RISCIF_STAT2 0x4bf
#define REG_RISCIF_CMDL_0 0x4c0
#define REG_RISCIF_CMDL_1 0x4c1
#define REG_RISCIF_CMDL_2 0x4c2
#define REG_RISCIF_CMDL_3 0x4c3
#define REG_RISCIF_CMDH_0 0x4c4
#define REG_RISCIF_CMDH_1 0x4c5
#define REG_RISCIF_CMDH_2 0x4c6
#define REG_RISCIF_CMDH_3 0x4c7
#define REG_RISCIF_RSPL_0 0x4c8
#define REG_RISCIF_RSPL_1 0x4c9
#define REG_RISCIF_RSPL_2 0x4ca
#define REG_RISCIF_RSPL_3 0x4cb
#define REG_RISCIF_RSPH_0 0x4cc
#define REG_RISCIF_RSPH_1 0x4cd
#define REG_RISCIF_RSPH_2 0x4ce
#define REG_RISCIF_RSPH_3 0x4cf


/*********** Common ATD Register Addr. **************/

//DEMOD_Areg
#define REG_ATD_AREG00 0x100
#define REG_ATD_AREG01 0x101
#define REG_ATD_AREG02 0x102
#define REG_ATD_AREG03 0x103
#define REG_ATD_AREG04 0x104
#define REG_ATD_AREG05 0x105
#define REG_ATD_AREG06 0x106
#define REG_ATD_AREG07 0x107
#define REG_ATD_AREG08 0x108
#define REG_ATD_AREG09 0x109
#define REG_ATD_AREG10 0x10a
#define REG_ATD_AREG11 0x10b
#define REG_ATD_AREG12 0x10c
#define REG_ATD_AREG13 0x10d

//ATD_AUC
#define REG_ATD_AUC_CFG1 0x220
#define REG_ATD_AUC_CFG2 0x221

//ATD_DC
#define REG_ATD_ATD_DC_RFA2 0x128
#define REG_ATD_ATD_DC_CFG2 0x129
#define REG_ATD_ATD_DC_CFG3 0x12a
#define REG_ATD_ATD_DC_RFA1 0x12b

//ATD_EQ
#define REG_ATD_EQ_CFG 0x12c
#define REG_ATD_EQ_WIDX 0x12d
#define REG_ATD_EQ_WMSB 0x12e
#define REG_ATD_EQ_WLSB 0x12f

//ATD_CTL
#define REG_ATD_ADCIF_CFG 0x120
#define REG_ATD_CTL_SYS1 0x130
#define REG_ATD_CTL_CFG 0x131
#define REG_ATD_CTL_TEST 0x132
#define REG_ATD_CTL_CID 0x133
#define REG_ATD_CTL_RST1 0x134
#define REG_ATD_CTL_RST2 0x135
#define REG_ATD_CTL_REGRST1 0x138
#define REG_ATD_CTL_REGRST2 0x139
#define REG_ATD_CTL_DBGSEL0 0x13a
#define REG_ATD_CTL_SYS2 0x13b
#define REG_ATD_CTL_DBGSEL1 0x13c
#define REG_ATD_CTL_CFG_MON 0x13d
#define REG_ATD_CTL_MISC_SEL 0x13e
#define REG_ATD_CTL_TESTCFG 0x13f

//ATD_CR
#define REG_ATD_ACR_PN_CFG0 0x190
#define REG_ATD_ACR_PN_CFG1 0x191
#define REG_ATD_ACR_PN_CFG2 0x192
#define REG_ATD_ACR_PN_CFG3 0x193
#define REG_ATD_ACR_FN_CFG0 0x194
#define REG_ATD_ACR_SCNF 0x195
#define REG_ATD_ACR_QCNF 0x196
#define REG_ATD_ACR_CFG 0x197
#define MSK_ATD_ACR_CFG_PN_ACFG_EN 0x20
#define MSK_ATD_ACR_CFG_FN_ACFG_EN 0x10
#define REG_ATD_TRAN_STG 0x198
#define REG_ATD_PFLL_CFG 0x199
#define REG_ATD_PFLL_CFG2 0x19a
#define REG_ATD_ACR_FN_CFG1 0x19b
#define REG_ATD_CMETRIC0 0x19c
#define REG_ATD_CMETRIC1 0x19d
#define REG_ATD_CFO_DEV0 0x19e
#define REG_ATD_CFO_DEV1 0x19f
#define REG_ATD_CR_STATE0 0x1a0
#define REG_ATD_CR_STATE1 0x1a1
#define MSK_ATD_CR_STATE0_CR_STATE 0x03
#define MSK_ATD_CR_STATE1_PNIND 0x08
#define MSK_ATD_CR_STATE1_FNIND 0x04
#define REG_ATD_CR_CFO0 0x1a2
#define REG_ATD_CR_CFO1 0x1a3
#define REG_ATD_CR_CFO2 0x1a4
#define REG_ATD_CR_CFO3 0x1a5
#define REG_ATD_CR_CPO0 0x1a6
#define REG_ATD_CR_CPO1 0x1a7
#define REG_ATD_PRR_SS 0x1a8
#define REG_ATD_PRR_TRAN 0x1a9
#define REG_ATD_CR_CFG 0x1aa
#define REG_ATD_PS_LOC 0x1ab
#define REG_ATD_PS_POW0 0x1ac
#define REG_ATD_PS_POW1 0x1ad
#define REG_ATD_PS_BMPOW0 0x1ae
#define REG_ATD_PS_BMP1W0 0x1af
#define REG_ATD_PS_BMLOC 0x1b0
#define REG_ATD_PS_CFG 0x1b1
#define REG_ATD_PRR_PRFK 0x1b2
#define REG_ATD_PRR_DAMRK 0x1b3
#define REG_ATD_PE_CFG0 0x1b4
#define MSK_ATD_PE_CFG0_NULL_REGION 0x70
#define REG_ATD_PE_CFG1 0x1b5
#define REG_ATD_PE_CFOK 0x1b6
#define MSK_ATD_PE_CFOK_CFO_K_TRK 0x07
#define REG_ATD_PJ_CFG 0x1b7
#define REG_ATD_ADVPS_CSTCFO 0x1b8
#define REG_ATD_CR_PNDEC_CFG 0x1b9
#define REG_ATD_CR_GATED 0x1ba
#define REG_ATD_ADVPS_CFG2 0x1bb
#define REG_ATD_ADVPS_PRB0 0x1bc
#define REG_ATD_ADVPS_PRB1 0x1bd
#define REG_ATD_ADVPS_PRB2 0x1be
#define REG_ATD_TRAN_CFG 0x1bf
#define REG_ATD_ADVPS_CFG0 0x250
#define REG_ATD_ADVPS_CFG1 0x251
#define REG_ATD_CR_DEBG 0x252
#define REG_ATD_IPS_MODE0 0x253
#define REG_ATD_IPS_MODE1 0x254
#define REG_ATD_IPS_MODE2 0x255
#define REG_ATD_IPS_SWR 0x256
#define REG_ATD_CR_OVMIND 0x257
#define REG_ATD_OVM_THD 0x258
#define REG_ATD_CR_PATH_TOG 0x259
#define REG_ATD_CFO_LK 0x25a
#define REG_ATD_AVGCFO_1 0x25b
#define REG_ATD_AVGCFO_2 0x25c
#define REG_ATD_AVGCFO_3 0x25d
#define REG_ATD_MATD_CR_CFG0 0x25e
#define REG_ATD_MATD_CR_CFG1 0x25f
#define REG_ATD_MATD_CR_CFG2 0x260
#define REG_ATD_MATD_CR_CFG3 0x261
#define REG_ATD_CR_DD_CFG0 0x264
#define REG_ATD_CR_DD_CFG1 0x265
#define REG_ATD_CR_DD_CFG2 0x266
#define REG_ATD_CR_DD_CFG3 0x267
#define REG_ATD_CR_AVGPE2 0x26A
#define REG_ATD_CR_AVGPE3 0x26B
#define REG_ATD_CR_AVGPE4 0x26C

#if defined(CC_MT5389)//d110701_Haibo:For MT5389,avg mode config is different register
#define REG_ATD_CR_AVGPE1  0x258
#define MSK_ATD_CR_AVGPE1_AVG_PRR_EN 0x40
#else
#define REG_ATD_CR_AVGPE1  0x269
#define MSK_ATD_CR_AVGPE1_AVG_PRR_EN 0x08
#endif
#define REG_ATD_CR_FIFO_SELECTION  0x269//d110707_Haibo:For both MT5389 and later IC,FIFO length selection register is 0x269

//ATD_DRO
#define REG_ATD_DRO_LP_CFG01 0x164
#define REG_ATD_DRO_LP_CFG02 0x165
#define REG_ATD_DRO_LP_CFG03 0x166
#define REG_ATD_DRO_LP_CFG04 0x167
#define REG_ATD_DRO_LP_CFG05 0x168
#define REG_ATD_DRO_DCR_EXT1 0x16a
#define REG_ATD_DRO_DCR_EXT2 0x16b
#define REG_ATD_DRO_PROBE_CFG 0x16c
#define REG_ATD_DRO_RPOBE_1 0x16d
#define REG_ATD_DRO_PROBE_2 0x16e
#define REG_ATD_DRO_WADC_1 0x16f
#define REG_ATD_DRO_WADC_2 0x170
#define REG_ATD_DRO_WADC_3 0x171
#define REG_ATD_DRO_WADC_4 0x172
#define REG_ATD_DRO_WADC_5 0x173
#define REG_ATD_DRO_CVBS_LPF 0x174
#define REG_ATD_DRO_LINEDUR_CFG1 0x175
#define REG_ATD_DRO_LINEDUR_CFG2 0x176
#define REG_ATD_DRO_LINEDUR_CFG3 0x177
#define REG_ATD_DRO_LINEDUR_CFG4 0x178
#define REG_ATD_DRO_TIMER_CFG 0x179
#define REG_ATD_DRO_REF_COEF 0x17a
#define REG_ATD_DRO_REF_CFG1 0x17b
#define REG_ATD_DRO_REF_CFG2 0x17c
#define REG_ATD_DRO_MASK_CFG 0x17d
#define REG_ATD_DRO_RSP_CFG 0x17e
#define REG_ATD_DRO_OBS1 0x17f
#define REG_ATD_DRO_OBS2 0x200
#define REG_ATD_DRO_OBS3 0x201
#define REG_ATD_DRO_REF0_1 0x202
#define REG_ATD_DRO_REF0_2 0x203
#define REG_ATD_DRO_REF1_1 0x204
#define REG_ATD_DRO_REF1_2 0x205
#define REG_ATD_DRO_REF_DIFF_1 0x206
#define REG_ATD_DRO_REF_DIFF_2 0x207
#define REG_ATD_DRO_SCAL_7 0x208
#define REG_ATD_DRO_SCAL_8 0x209
#define REG_ATD_DRO_SCAL_9 0x20a
#define REG_ATD_DRO_SCAL_10 0x20b
#define REG_ATD_DRO_SCAL_11 0x20c
#define REG_ATD_DRO_DEV_EXT_0 0x20d
#define REG_ATD_DRO_DEV_EXT_1 0x20e
#define REG_ATD_DRO_W_ADC_MAX 0x20f
#define REG_ATD_DRO_CVBS_UP_BUD_0 0x210
#define REG_ATD_DRO_CVBS_UP_BUD_1 0x211
#define REG_ATD_DRO_CVBS_LOW_BUD_0 0x212
#define REG_ATD_DRO_CVBS_LOW_BUD_1 0x213
#define REG_ATD_DRO_LSYNC_CTRL 0x214
#define REG_ATD_TD_MAX_0 0x215
#define REG_ATD_TD_MAX_1 0x216
#define REG_ATD_TD_MIN_0 0x217
#define REG_ATD_TD_MIN_1 0x218
#define REG_ATD_TD_NF_MUL_COEF_0 0x219
#define REG_ATD_TD_NF_MUL_COEF_1 0x21a
#define REG_ATD_TD_NF_MUL_COEF_2 0x21b
#define REG_ATD_TD_BPF_MUL_COEF_0 0x21c
#define REG_ATD_TD_BPF_MUL_COEF_1 0x21d
#define REG_ATD_TD_BPF_MUL_COEF_2 0x21e
#define REG_ATD_TD_TONE_CHK 0x21f
#define REG_ATD_DRO_MAG_NOISE_0 0x230
#define REG_ATD_DRO_MAG_NOISE_1 0x231
#define REG_ATD_DRO_CCI_CFG1 0x232
#define REG_ATD_DRO_CCI_CFG2 0x233
#define REG_ATD_DRO_CCI_CFG3 0x234
#define REG_ATD_DRO_CCI_CFG4 0x235
#define REG_ATD_DRO_CCI_OBS 0x236
#define REG_ATD_DRO_CLAMP_CFG1 0x237
#define REG_ATD_DRO_CLAMP_CFG2 0x238
#define REG_ATD_DRO_LSYNC_MAX1 0x239
#define REG_ATD_DRO_LSYNC_MAX2 0x23a
#define REG_ATD_DRO_LSYNC_MIN1 0x23b
#define REG_ATD_DRO_LSYNC_MIN2 0x23c
#define REG_ATD_DRO_MISC_CFG 0x23d
#define REG_ATD_DRO_BLANK_LBND1 0x23e
#define REG_ATD_DRO_BLANK_LBND2 0x23f
#define REG_ATD_SNR_CFG_0 0x240
#define REG_ATD_SNR_CFG_1 0x241
#define REG_ATD_CVBS_SNR_0 0x242
#define REG_ATD_CVBS_SNR_1 0x243
#define REG_ATD_CVBS_SNR_2 0x244
#define REG_ATD_DRO_SCAL_0 0x245
#define REG_ATD_DRO_SCAL_1 0x246
#define REG_ATD_DRO_SCAL_2 0x247
#define REG_ATD_DRO_SCAL_3 0x248
#define REG_ATD_DRO_SCAL_4 0x249
#define REG_ATD_DRO_SCAL_5 0x24a
#define REG_ATD_DRO_SCAL_6 0x24b
#define REG_ATD_DRO_MASK_CFG2 0x24c
#define REG_ATD_TD_CFG 0x24d
#define REG_ATD_DRO_IMP_TIMER_RST 0x24e
#define REG_ATD_DRO_VSI_CFG0 0x24f
#define REG_ATD_DRO_VSI_CFG1 0x270
#define REG_ATD_DRO_VSI_CFG2 0x271
#define REG_ATD_DRO_VSI_CFG3 0x272
#define REG_ATD_DRO_VSI_CFG4 0x273
#define REG_ATD_DRO_ASYNC_CFG0 0x274
#define REG_ATD_DRO_ASYNC_CFG1 0x275
#define REG_ATD_DRO_ASYNC_CFG2 0x276
#define REG_ATD_DRO_SCAL_12 0x277
#define REG_ATD_DRO_ASYNC_CFG3 0x278

//ATD_AVS
#define REG_ATD_AVS_RFA3 0x1c0
#define REG_ATD_AVS_VOL 0x1c1
#define REG_ATD_AVS_AOH 0x1c2
#define REG_ATD_AVS_AOL 0x1c3
#define REG_ATD_AVS_RFA0 0x1c4
#define REG_ATD_AVS_RFA1 0x1c5
#define REG_ATD_AVS_RFA2 0x1c6

//ATD_CPR
#define REG_ATD_CPR_EXT_FREQ1 0x1d0
#define REG_ATD_CPR_EXT_FREQ2 0x1d1
#define REG_ATD_CPR_EXT_FREQ3 0x1d2
#define REG_ATD_CPR_EXT_FREQ4 0x1d3
#define REG_ATD_CPR_CFO_1 0x1d4
#define REG_ATD_CPR_CFO_2 0x1d5
#define REG_ATD_CPR_CFO_CFG 0x1d6

//ATD_DSM
#define REG_ATD_DSM_CVS 0x7eb
#define REG_ATD_DSM_CFG 0x1e8

//ATD_POA
#define REG_ATD_POA_CFG 0x180
#define REG_ATD_POA_W_POA_H 0x181
#define REG_ATD_POA_W_POA_L 0x182
#define REG_ATD_POA_TARG_LVH 0x183
#define REG_ATD_POA_TARG_LVL 0x184
#define REG_ATD_POA_MREF_EDT 0x185
#define REG_ATD_POA_PROBE_MREFM 0x186
#define REG_ATD_POA_PROBE_MREFL 0x187
#define REG_ATD_POA_PROBE_GWRDH 0x188
#define REG_ATD_POA_PROBE_GWRDL 0x189
#define REG_ATD_POA_IIRCFG 0x18a
#define REG_ATD_POA_IM_EN 0x18b
#define REG_ATD_POA_RFA2 0x18c

//ATD_TVDIF
#define REG_ATD_TVDIF_CFG0 0x124
#define REG_ATD_TVDIF_CFG1 0x125
#define REG_ATD_TVDIF_CFG2 0x126
#define REG_ATD_TVDIF_CFG3 0x127
#define REG_ATD_TVDIF_CFG4 0x1e0
#define REG_ATD_TVDIF_CFG5 0x1e1
#define REG_ATD_TVDIF_CFG6 0x1e2
#define REG_ATD_TVDIF_VMASK_DLY0 0x1e3
#define REG_ATD_TVDIF_VMASK_DLY1 0x1e4
#define REG_ATD_TVDIF_VMASK_WID0 0x1e5
#define REG_ATD_TVDIF_VMASK_WID1 0x1e6

//ATD_VOP
#define REG_ATD_VOP_FWCPOIH 0x1c8
#define REG_ATD_VOP_FWCPOIL 0x1c9
#define REG_ATD_VOP_FWCPOQH 0x1ca
#define REG_ATD_VOP_FWCPOQL 0x1cb
#define REG_ATD_VOP_RESERVE0 0x1cc
#define REG_ATD_VOP_RESERVE1 0x1cd
#define REG_ATD_VOP_RESERVE2 0x1ce
#define REG_ATD_VOP_RESERVE3 0x1cf

//ATD_VUSM
#define REG_ATD_VUSM_00 0x228

/*********** MT5387/63 ATD Register Addr. Only **************/

//#if ((defined CC_MT5387) || (defined CC_MT5363))
#if 1
//ATD_PRA
#define REG_ATD_PRA_LP1_CFG01 0x140
#define REG_ATD_PRA_LP1_CFG02 0x141
#define REG_ATD_PRA_LP1_CFG03 0x142
#define REG_ATD_PRA_LP1_CFG04 0x143
#define REG_ATD_PRA_LP1_CFG05 0x144
#define REG_ATD_PRA_LP1_CFG06 0x145
#define REG_ATD_PRA_LP1_CFG07 0x146
#define REG_ATD_PRA_LP1_CFG08 0x147
#define REG_ATD_PRA_LP1_CFG09 0x148
#define REG_ATD_PRA_LP1_CFG10 0x149
#define REG_ATD_PRA_LP1_CFG11 0x14A
#define REG_ATD_PRA_LP1_CFG12 0x14B
#define REG_ATD_PRA_LP1_CFG13 0x14C
#define REG_ATD_PRA_RF_CTL_1 0x14D
#define REG_ATD_PRA_RF_CTL_2 0x14E
#define REG_ATD_PRA_IF_CTL_1 0x14F
#define REG_ATD_PRA_IF_CTL_2 0x150
#define REG_ATD_PRA_RF_BIAS 0x151
#define REG_ATD_PRA_IF_BIAS 0x152
#define REG_ATD_PRA_RF_SLOPE 0x153
#define REG_ATD_PRA_IF_SLOPE 0x154
#define REG_ATD_PRA_RF_MAX 0x155
#define REG_ATD_PRA_IF_MAX 0x156
#define REG_ATD_PRA_RF_MIN 0x157
#define REG_ATD_PRA_IF_MIN 0x158
#define REG_ATD_PRA_CFG_1 0x159
#define REG_ATD_PRA_CFG_2 0x15A
#define REG_ATD_PRA_PROBE_1 0x15B
#define REG_ATD_PRA_PROBE_2 0x15C
#define REG_ATD_PRA_PN_MOD 0x15D
#define REG_ATD_PRA_ST 0x15E
#define REG_ATD_PRA_DR_SEL 0x15F
#define REG_ATD_PRA_TRA_CTL1 0x160
#define REG_ATD_PRA_TRA_CTL2 0x161
#define REG_ATD_PRA_TRA_CTL3 0x162

#endif

/*********** MT8223/5365 ATD Register Addr. Only **************/

#if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || (defined CC_MT5368) || defined(CC_MT5389))

//ATD_FIFO
#define REG_ATD_FIFO_CFG1 0x15f

//ATD_SRF_65
#define REG_ATD_SRF_AG0 0x140
#define REG_ATD_SRF_B01 0x141
#define REG_ATD_SRF_B02 0x142
#define REG_ATD_SRF_AG1 0x143
#define REG_ATD_SRF_B11 0x144
#define REG_ATD_SRF_B12 0x145
#define REG_ATD_SRF_AG2 0x146
#define REG_ATD_SRF_B21 0x147
#define REG_ATD_SRF_B22 0x148
#define REG_ATD_SRF_AG3 0x149
#define REG_ATD_SRF_B31 0x14a
#define REG_ATD_SRF_B32 0x14b
#define REG_ATD_SRF_AG4 0x14c
#define REG_ATD_SRF_B41 0x14d
#define REG_ATD_SRF_B42 0x14e
#define REG_ATD_SRF_AG5 0x14f
#define REG_ATD_SRF_B51 0x150
#define REG_ATD_SRF_B52 0x151
#define REG_ATD_SRF_CVS 0x7d7

//ATD_DSHR_65
#define REG_ATD_DSHR_MBIST_CTRL 0x4d0
#define REG_ATD_DSHR_REG_MAP 0x4d1
#define REG_ATD_DSHR_SYS_CFG 0x4d2
#define REG_ATD_DSHR_EIRQ_ST 0x4d4
#define REG_ATD_DSHR_EIRQ_EN 0x4d8
#define REG_ATD_DSHR_EIRQ_CLR 0x4dc

//ATD_DSHR_RISCIF_65
#define REG_ATD_RISCIF_CFG 0x4a0
#define REG_ATD_RISCIF_TO1 0x4a1
#define REG_ATD_RISCIF_TO2 0x4a2
#define REG_ATD_RISCIF_TO3 0x4a3
#define REG_ATD_RISCIF_PADR0 0x4b4
#define REG_ATD_RISCIF_PADR1 0x4b5
#define REG_ATD_RISCIF_PADR2 0x4b6
#define REG_ATD_RISCIF_PADR3 0x4b7
#define REG_ATD_RISCIF_PRDA_0 0x4b8
#define REG_ATD_RISCIF_PRDA_1 0x4b9
#define REG_ATD_RISCIF_PRDA_2 0x4ba
#define REG_ATD_RISCIF_PRDA_3 0x4bb
#define REG_ATD_RISCIFSTAT0 0x4bc
#define REG_ATD_RISCIF_STAT1 0x4bd
#define REG_ATD_RISCIF_STAT2 0x4bf
#define REG_ATD_RISCIF_CMDL_0 0x4c0
#define REG_ATD_RISCIF_CMDL_1 0x4c1
#define REG_ATD_RISCIF_CMDL_2 0x4c2
#define REG_ATD_RISCIF_CMDL_3 0x4c3
#define REG_ATD_RISCIF_CMDH_0 0x4c4
#define REG_ATD_RISCIF_CMDH_1 0x4c5
#define REG_ATD_RISCIF_CMDH_2 0x4c6
#define REG_ATD_RISCIF_CMDH_3 0x4c7
#define REG_ATD_RISCIF_RSPL_0 0x4c8
#define REG_ATD_RISCIF_RSPL_1 0x4c9
#define REG_ATD_RISCIF_RSPL_2 0x4ca
#define REG_ATD_RISCIF_RSPL_3 0x4cb
#define REG_ATD_RISCIF_RSPH_0 0x4cc
#define REG_ATD_RISCIF_RSPH_1 0x4cd
#define REG_ATD_RISCIF_RSPH_2 0x4ce
#define REG_ATD_RISCIF_RSPH_3 0x4cf

//ATD_DSHR_SIF_65
#define REG_ATD_DSHR_SIF_01 0x044
#define REG_ATD_DSHR_SIF_02 0x045
#define REG_ATD_DSHR_SIF_03 0x046

//ATD_DSHR_SIFIF_65
#define REG_ATD_DSHR_SIFIF_01 0x047
#define REG_ATD_DSHR_SIFIF_02 0x048
#define REG_ATD_DSHR_SIFIF_03 0x04a

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)//d110330_Haibo:SLD part: register bank is from 0x7xx
//GPM
#define REG_ATD_DEMOD_GPM_E00 0x600 //0xe00.demod mode select
#define REG_ATD_DEMOD_GPM_E01 0x601 //demod ana_if

//SLD
#define REG_ATD_SLD_RST   0x760
#define REG_ATD_SLD_CFG_1 0x762  

//SLD_ADCIF_96
#define REG_ATD_SLD_ADCIF_CFG1 0x780
#define REG_ATD_SLD_ADCIF_CFG2 0x781
#define REG_ATD_SLD_ADCIF_DSM4A 0x782
#define REG_ATD_SLD_ADCIF_DSM4B 0x783
#define REG_ATD_SLD_ADCIF_CDC 0x784
#define REG_ATD_SLD_ADCIF_PGCLD 0x785
#define REG_ATD_SLD_ADCIF_PGCLP 0x786
#define REG_ATD_SLD_ADCIF_THC 0x787

//SLD_ADTST_96
#define REG_ATD_ADTST_FIFOCFG 0x78e
#define REG_ATD_ADTST_DSCFG 0x78f

//SLD_DAGC_96
#define REG_ATD_SLD_DAGC_00 0x790
#define REG_ATD_SLD_DAGC_01 0x791
#define REG_ATD_SLD_DAGC_02 0x792
#define REG_ATD_SLD_DAGC_03 0x793
#define REG_ATD_SLD_DAGC_04 0x794
#define REG_ATD_SLD_DAGC_05 0x795
#define REG_ATD_SLD_DAGC_06 0x796
#define REG_ATD_SLD_DAGC_07 0x797
#define REG_ATD_SLD_DAGC_08 0x798
#define REG_ATD_SLD_DAGC_09 0x799
#define REG_ATD_SLD_DAGC_0A 0x79a
#define REG_ATD_SLD_DAGC_0B 0x79b
#define REG_ATD_SLD_DAGC_0C 0x79c
#define REG_ATD_SLD_DAGC_0D 0x79d
#define REG_ATD_SLD_DAGC_0E 0x79e
#define REG_ATD_SLD_DAGC_0F 0x79f
#define REG_ATD_SLD_DAGC_10 0x7a0
#define REG_ATD_SLD_DAGC_11 0x7a1
#define REG_ATD_SLD_DAGC_12 0x7a2
#define REG_ATD_SLD_DAGC_13 0x7a3
#define REG_ATD_SLD_DAGC_14 0x7a4
#define REG_ATD_SLD_DAGC_15 0x7a5

//SLD_PRA_96
#define REG_ATD_SLD_PRA_00 0x7b0
#define REG_ATD_SLD_PRA_01 0x7b1
#define REG_ATD_SLD_PRA_02 0x7b2
#define REG_ATD_SLD_PRA_03 0x7b3
#define REG_ATD_SLD_PRA_04 0x7b4
#define REG_ATD_SLD_PRA_05 0x7b5
#define REG_ATD_SLD_PRA_06 0x7b6
#define REG_ATD_SLD_PRA_07 0x7b7
#define REG_ATD_SLD_PRA_08 0x7b8
#define REG_ATD_SLD_PRA_09 0x7b9
#define REG_ATD_SLD_PRA_0A 0x7ba
#define REG_ATD_SLD_PRA_0B 0x7bb
#define REG_ATD_SLD_PRA_0C 0x7bc
#define REG_ATD_SLD_PRA_0D 0x7bd
#define REG_ATD_SLD_PRA_0E 0x7be
#define REG_ATD_SLD_PRA_0F 0x7bf
#define REG_ATD_SLD_PRA_10 0x7c0
#define REG_ATD_SLD_PRA_11 0x7c1
#define REG_ATD_SLD_PRA_12 0x7c2
#define REG_ATD_SLD_PRA_13 0x7c3
#define REG_ATD_SLD_PRA_14 0x7c4
#define REG_ATD_SLD_PRA_15 0x7c5
#define REG_ATD_SLD_PRA_16 0x7c6
#define REG_ATD_SLD_PRA_17 0x7c7
#define REG_ATD_SLD_PRA_18 0x7c8
#define REG_ATD_SLD_PRA_19 0x7c9
#define REG_ATD_SLD_PRA_1A 0x7ca
#define REG_ATD_SLD_PRA_1B 0x7cb
#define REG_ATD_SLD_PRA_1C 0x7cc
#define REG_ATD_SLD_PRA_1D 0x7cd
#define REG_ATD_SLD_PRA_1E 0x7ce
#define REG_ATD_SLD_PRA_1F 0x7cf
#define REG_ATD_SLD_PRA_20 0x7d0
#define REG_ATD_SLD_PRA_21 0x7d1
#define REG_ATD_SLD_PRA_22 0x7d2
#define REG_ATD_SLD_PRA_23 0x7d3
#define REG_ATD_SLD_PRA_24 0x7d4
#define REG_ATD_SLD_PRA_25 0x7d5
#define REG_ATD_SLD_PRA_26 0x7d6
#define REG_ATD_SLD_PRA_27 0x7d7
#define REG_ATD_SLD_PRA_28 0x7d8
#define REG_ATD_SLD_PRA_29 0x7d9
#define REG_ATD_SLD_PRA_2A 0x7da
#define REG_ATD_SLD_PRA_2B 0x7db
#define REG_ATD_SLD_PRA_2C 0x7dc
#define REG_ATD_SLD_PRA_2D 0x7dd
#define REG_ATD_SLD_PRA_2E 0x7de
#define REG_ATD_SLD_PRA_2F 0x7df
#define REG_ATD_SLD_PRA_30 0x7e0
#define REG_ATD_SLD_PRA_31 0x7e1
#define REG_ATD_SLD_PRA_32 0x7e2
#define REG_ATD_SLD_PRA_33 0x7e3
#define REG_ATD_SLD_PRA_34 0x7e4
#define REG_ATD_SLD_PRA_35 0x7e5
#define REG_ATD_SLD_PRA_36 0x7e6
#define REG_ATD_SLD_PRA_37 0x7e7
#define REG_ATD_SLD_PRA_38 0x7e8
#define REG_ATD_SLD_PRA_39 0x7e9
#define REG_ATD_SLD_PRA_3A 0x7ea
#define REG_ATD_SLD_PRA_3B 0x7eb
#define REG_ATD_SLD_PRA_3C 0x7ec
#define REG_ATD_SLD_PRA_3D 0x7ed
#define REG_ATD_SLD_PRA_3E 0x7ee
#define REG_ATD_SLD_PRA_3F 0x7ef
#define REG_ATD_SLD_PRA_40 0x7f0
#define REG_ATD_SLD_PRA_41 0x7f1
#define REG_ATD_SLD_PRA_42 0x7f2
#define REG_ATD_SLD_PRA_43 0x7f3
#define REG_ATD_SLD_PRA_44 0x7f4
#define REG_ATD_SLD_PRA_45 0x7f5
#define REG_ATD_SLD_PRA_46 0x7f6
#define REG_ATD_SLD_PRA_47 0x7f7
#define REG_ATD_SLD_PRA_48 0x7f8
#define REG_ATD_SLD_PRA_49 0x7f9
#define REG_ATD_SLD_PRA_4A 0x7fa
#define REG_ATD_SLD_PRA_4B 0x7fb

#else//65/95
//SLD_ADCIF_65
#define REG_ATD_SLD_ADCIF_CFG1 0x280
#define REG_ATD_SLD_ADCIF_CFG2 0x281
#define REG_ATD_SLD_ADCIF_DSM4A 0x282
#define REG_ATD_SLD_ADCIF_DSM4B 0x283
#define REG_ATD_SLD_ADCIF_CDC 0x284
#define REG_ATD_SLD_ADCIF_PGCLD 0x285
#define REG_ATD_SLD_ADCIF_PGCLP 0x286
#define REG_ATD_SLD_ADCIF_THC 0x287

//SLD_ADTST_65
#define REG_ATD_ADTST_FIFOCFG 0x28e
#define REG_ATD_ADTST_DSCFG 0x28f

//SLD_DAGC_65
#define REG_ATD_SLD_DAGC_00 0x290
#define REG_ATD_SLD_DAGC_01 0x291
#define REG_ATD_SLD_DAGC_02 0x292
#define REG_ATD_SLD_DAGC_03 0x293
#define REG_ATD_SLD_DAGC_04 0x294
#define REG_ATD_SLD_DAGC_05 0x295
#define REG_ATD_SLD_DAGC_06 0x296
#define REG_ATD_SLD_DAGC_07 0x297
#define REG_ATD_SLD_DAGC_08 0x298
#define REG_ATD_SLD_DAGC_09 0x299
#define REG_ATD_SLD_DAGC_0A 0x29a
#define REG_ATD_SLD_DAGC_0B 0x29b
#define REG_ATD_SLD_DAGC_0C 0x29c
#define REG_ATD_SLD_DAGC_0D 0x29d
#define REG_ATD_SLD_DAGC_0E 0x29e
#define REG_ATD_SLD_DAGC_0F 0x29f
#define REG_ATD_SLD_DAGC_10 0x2a0
#define REG_ATD_SLD_DAGC_11 0x2a1
#define REG_ATD_SLD_DAGC_12 0x2a2
#define REG_ATD_SLD_DAGC_13 0x2a3
#define REG_ATD_SLD_DAGC_14 0x2a4
#define REG_ATD_SLD_DAGC_15 0x2a5

//SLD_PRA_65
#define REG_ATD_SLD_PRA_00 0x2b0
#define REG_ATD_SLD_PRA_01 0x2b1
#define REG_ATD_SLD_PRA_02 0x2b2
#define REG_ATD_SLD_PRA_03 0x2b3
#define REG_ATD_SLD_PRA_04 0x2b4
#define REG_ATD_SLD_PRA_05 0x2b5
#define REG_ATD_SLD_PRA_06 0x2b6
#define REG_ATD_SLD_PRA_07 0x2b7
#define REG_ATD_SLD_PRA_08 0x2b8
#define REG_ATD_SLD_PRA_09 0x2b9
#define REG_ATD_SLD_PRA_0A 0x2ba
#define REG_ATD_SLD_PRA_0B 0x2bb
#define REG_ATD_SLD_PRA_0C 0x2bc
#define REG_ATD_SLD_PRA_0D 0x2bd
#define REG_ATD_SLD_PRA_0E 0x2be
#define REG_ATD_SLD_PRA_0F 0x2bf
#define REG_ATD_SLD_PRA_10 0x2c0
#define REG_ATD_SLD_PRA_11 0x2c1
#define REG_ATD_SLD_PRA_12 0x2c2
#define REG_ATD_SLD_PRA_13 0x2c3
#define REG_ATD_SLD_PRA_14 0x2c4
#define REG_ATD_SLD_PRA_15 0x2c5
#define REG_ATD_SLD_PRA_16 0x2c6
#define REG_ATD_SLD_PRA_17 0x2c7
#define REG_ATD_SLD_PRA_18 0x2c8
#define REG_ATD_SLD_PRA_19 0x2c9
#define REG_ATD_SLD_PRA_1A 0x2ca
#define REG_ATD_SLD_PRA_1B 0x2cb
#define REG_ATD_SLD_PRA_1C 0x2cc
#define REG_ATD_SLD_PRA_1D 0x2cd
#define REG_ATD_SLD_PRA_1E 0x2ce
#define REG_ATD_SLD_PRA_1F 0x2cf
#define REG_ATD_SLD_PRA_20 0x2d0
#define REG_ATD_SLD_PRA_21 0x2d1
#define REG_ATD_SLD_PRA_22 0x2d2
#define REG_ATD_SLD_PRA_23 0x2d3
#define REG_ATD_SLD_PRA_24 0x2d4
#define REG_ATD_SLD_PRA_25 0x2d5
#define REG_ATD_SLD_PRA_26 0x2d6
#define REG_ATD_SLD_PRA_27 0x2d7
#define REG_ATD_SLD_PRA_28 0x2d8
#define REG_ATD_SLD_PRA_29 0x2d9
#define REG_ATD_SLD_PRA_2A 0x2da
#define REG_ATD_SLD_PRA_2B 0x2db
#define REG_ATD_SLD_PRA_2C 0x2dc
#define REG_ATD_SLD_PRA_2D 0x2dd
#define REG_ATD_SLD_PRA_2E 0x2de
#define REG_ATD_SLD_PRA_2F 0x2df
#define REG_ATD_SLD_PRA_30 0x2e0
#define REG_ATD_SLD_PRA_31 0x2e1
#define REG_ATD_SLD_PRA_32 0x2e2
#define REG_ATD_SLD_PRA_33 0x2e3
#define REG_ATD_SLD_PRA_34 0x2e4
#define REG_ATD_SLD_PRA_35 0x2e5
#define REG_ATD_SLD_PRA_36 0x2e6
#define REG_ATD_SLD_PRA_37 0x2e7
#define REG_ATD_SLD_PRA_38 0x2e8
#define REG_ATD_SLD_PRA_39 0x2e9
#define REG_ATD_SLD_PRA_3A 0x2ea
#define REG_ATD_SLD_PRA_3B 0x2eb
#define REG_ATD_SLD_PRA_3C 0x2ec
#define REG_ATD_SLD_PRA_3D 0x2ed
#define REG_ATD_SLD_PRA_3E 0x2ee
#define REG_ATD_SLD_PRA_3F 0x2ef
#define REG_ATD_SLD_PRA_40 0x2f0
#define REG_ATD_SLD_PRA_41 0x2f1
#define REG_ATD_SLD_PRA_42 0x2f2
#define REG_ATD_SLD_PRA_43 0x2f3
#define REG_ATD_SLD_PRA_44 0x2f4
#define REG_ATD_SLD_PRA_45 0x2f5
#define REG_ATD_SLD_PRA_46 0x2f6
#define REG_ATD_SLD_PRA_47 0x2f7
#define REG_ATD_SLD_PRA_48 0x2f8
#define REG_ATD_SLD_PRA_49 0x2f9
#define REG_ATD_SLD_PRA_4A 0x2fa
#define REG_ATD_SLD_PRA_4B 0x2fb
#endif
#endif

/*********** Older ATD Register Addr. will be remove **************/

//#if ((defined CC_MT5387) || (defined CC_MT5363))
#if 0
//CTL

#define	REG_DEMOD_ADCIF_CFG		0x120

#define REG_DEMOD_CTL_SYS1		0x130
#define REG_DEMOD_CTL_CFG		0x131
#define REG_DEMOD_CTL_TEST		0x132
#define REG_DEMOD_CTL_CID		0x133
#define REG_DEMOD_CTL_RST1		0x134
#define REG_DEMOD_CTL_RST2		0x135
#define REG_DEMOD_CTL_REGRST1	0x138
#define REG_DEMOD_CTL_REGRST2	0x139
#define REG_DEMOD_CTL_SYS2		0x13B

//PRA
#define REG_DEMOD_PRA_LP1_CFG01 0x140
#define REG_DEMOD_PRA_LP1_CFG02 0x141
#define REG_DEMOD_PRA_LP1_CFG03 0x142
#define REG_DEMOD_PRA_LP1_CFG04 0x143
#define REG_DEMOD_PRA_LP1_CFG05 0x144
#define REG_DEMOD_PRA_LP1_CFG06 0x145
#define REG_DEMOD_PRA_LP1_CFG07 0x146
#define REG_DEMOD_PRA_LP1_CFG08 0x147
#define REG_DEMOD_PRA_LP1_CFG09 0x148
#define REG_DEMOD_PRA_LP1_CFG10 0x149
#define REG_DEMOD_PRA_LP1_CFG11 0x14A
#define REG_DEMOD_PRA_LP1_CFG12 0x14B
#define REG_DEMOD_PRA_LP1_CFG13 0x14C
#define REG_DEMOD_PRA_RF_CTL_1  0x14D
#define REG_DEMOD_PRA_RF_CTL_2  0x14E
#define REG_DEMOD_PRA_IF_CTL_1  0x14F
#define REG_DEMOD_PRA_IF_CTL_2  0x150
#define REG_DEMOD_PRA_RF_BIAS   0x151
#define REG_DEMOD_PRA_IF_BIAS   0x152
#define REG_DEMOD_PRA_RF_SLOPE  0x153
#define REG_DEMOD_PRA_IF_SLOPE  0x154
#define REG_DEMOD_PRA_RF_MAX    0x155
#define REG_DEMOD_PRA_IF_MAX    0x156
#define REG_DEMOD_PRA_RF_MIN    0x157
#define REG_DEMOD_PRA_IF_MIN    0x158
#define REG_DEMOD_PRA_CFG_1     0x159
#define REG_DEMOD_PRA_CFG_2     0x15A
#define REG_DEMOD_PRA_PROBE_1   0x15B
#define REG_DEMOD_PRA_PROBE_2   0x15C
#define REG_DEMOD_PRA_PN_MOD    0x15D
#define REG_DEMOD_PRA_ST        0x15E
#define REG_DEMOD_PRA_DR_SEL    0x15F

#define REG_DEMOD_PRA_TRA_CTL1  0x160
#define REG_DEMOD_PRA_TRA_CTL2  0x161
#define REG_DEMOD_PRA_TRA_CTL3  0x162


#define REG_DEMOD_ATD_POA_CFG	0x180
#define REG_DEMOD_POA_W_POA_H	0x181
#define REG_DEMOD_POA_W_POA_L	0x182
#define REG_DEMOD_POA_TARG_LVH	0x183
#define REG_DEMOD_POA_TARG_LVL	0x184
#define REG_DEMOD_POA_MREF_EDT	0x185
#define REG_DEMOD_POA_PROBE_MREFM	0x186
#define REG_DEMOD_POA_PROBE_MREFL	0x187
#define REG_DEMOD_POA_PROBE_GWRDH	0x188
#define REG_DEMOD_POA_PROBE_GWRDL	0x189
#define REG_DEMOD_POA_IIRCFG	0x18A
#define REG_DEMOD_POA_RFA1		0x18B
#define REG_DEMOD_POA_RFA2		0x18C

//CR
#define REG_DEMOD_ACR_PN_CFG0		0x190
#define REG_DEMOD_ACR_PN_CFG1		0x191		
#define REG_DEMOD_ACR_PN_CFG2		0x192
#define REG_DEMOD_ACR_PN_CFG3		0x193
#define REG_DEMOD_ACR_FN_CFG0		0x194
#define REG_DEMOD_ACR_SCNF			0x195
#define REG_DEMOD_ACR_QCNF			0x196
#define REG_DEMOD_ACR_CFG			0x197
#define REG_DEMOD_TRAN_STG			0x198
#define REG_DEMOD_PFLL_CFG			0x199
#define REG_DEMOD_PFLL_CFG2			0x19A
#define REG_DEMOD_CR_INDST			0x19B
#define REG_DEMOD_CMETRIC0			0x19C
#define REG_DEMOD_CMETRIC1			0x19D
#define REG_DEMOD_CFO_DEV0			0x19E
#define REG_DEMOD_CFO_DEV1			0x19F
#define REG_DEMOD_CR_STATE0			0x1A0
#define REG_DEMOD_CR_STATE1			0x1A1
#define REG_DEMOD_CR_CFO0		    0x1A2
#define REG_DEMOD_CR_CFO1		    0x1A3
#define REG_DEMOD_CR_CFO2		    0x1A4
#define REG_DEMOD_CR_CFO3		    0x1A5
#define REG_DEMOD_CR_CPO0		    0x1A6
#define REG_DEMOD_CR_CPO1		    0x1A7
#define REG_DEMOD_PFLT_SS		    0x1A8
#define REG_DEMOD_PFLT_TRAN	        0x1A9
#define REG_DEMOD_CR_CFG		    0x1AA
#define REG_DEMOD_PS_LOC		    0x1AB
#define REG_DEMOD_PS_POW0		    0x1AC
#define REG_DEMOD_PS_POW1		    0x1AD
#define REG_DEMOD_PS_BMPOW0	        0x1AE
#define REG_DEMOD_PS_BMP1W0	        0x1AF
#define REG_DEMOD_PS_BMLOC	        0x1B0
#define REG_DEMOD_PS_CFG		    0x1B1
#define REG_DEMOD_PRR_PRFK	        0x1B2
#define REG_DEMOD_PRR_DAMRK	        0x1B3
#define REG_DEMOD_PE_CFG0		    0x1B4
#define REG_DEMOD_PE_CFG1		    0x1B5
#define REG_DEMOD_PE_CFOK		    0x1B6
#define REG_DEMOD_PJ_CFG		    0x1B7
#define REG_DEMOD_ADVPS_CSTCFO      0x1B8
#define REG_DEMOD_ADVPS_CFG0	    0x1B9
#define REG_DEMOD_ADVPS_CFG1	    0x1BA
#define REG_DEMOD_ADVPS_CFG2	    0x1BB
#define REG_DEMOD_ADVPS_PRB0	    0x1BC
#define REG_DEMOD_ADVPS_PRB1	    0x1BD
#define REG_DEMOD_ADVPS_PRB2	    0x1BE
#define REG_DEMOD_TRAN_CFG		    0x1BF

#define REG_DEMOD_AVS_VOH			0x1C0
#define REG_DEMOD_AVS_VOL			0x1C1
#define REG_DEMOD_AVS_AOH			0x1C2
#define REG_DEMOD_AVS_AOL			0x1C3
#define REG_DEMOD_AVS_RFA0			0x1C4
#define REG_DEMOD_AVS_RFA1			0x1C5
#define REG_DEMOD_AVS_RFA2			0x1C6

#define REG_DEMOD_VOP_CFG			0x1C8
#define REG_DEMOD_VOP_CPOH			0x1C9
#define REG_DEMOD_VOP_CPOL			0x1CA
#define REG_DEMOD_VOP_PHLT			0x1CB
#define REG_DEMOD_VOP_HW_CPOH		0x1CC
#define REG_DEMOD_VOP_HW_CPOL		0x1CD
#define REG_DEMOD_VOP_PHCMP			0x1CE
#define REG_DEMOD_VOP_PREROT		0x1CF

//DRO
#define REG_DEMOD_DRO_LP_CFG01  	0x164
#define REG_DEMOD_DRO_LP_CFG02      0x165
#define REG_DEMOD_DRO_LP_CFG03      0x166
#define REG_DEMOD_DRO_LP_CFG04      0x167
#define REG_DEMOD_DRO_LP_CFG05      0x168
#define REG_DEMOD_DRO_DCR_EXT1      0x16A
#define REG_DEMOD_DRO_DCR_EXT2      0x16B
#define REG_DEMOD_DRO_PROBE_CFG     0x16C
#define REG_DEMOD_DRO_PROBE_1       0x16D
#define REG_DEMOD_DRO_PROBE_2       0x16E
#define REG_DEMOD_DRO_WADC_1        0x16F
#define REG_DEMOD_DRO_WADC_2        0x170
#define REG_DEMOD_DRO_WADC_3        0x171
#define REG_DEMOD_DRO_WADC_4        0x172
#define REG_DEMOD_DRO_WADC_5        0x173
#define REG_DEMOD_DRO_CVBS_LPF      0x174
#define REG_DEMOD_DRO_LINEDUR_CFG1  0x175
#define REG_DEMOD_DRO_LINEDUR_CFG2  0x176
#define REG_DEMOD_DRO_LINEDUR_CFG3  0x177
#define REG_DEMOD_DRO_LINEDUR_CFG4  0x178
#define REG_DEMOD_DRO_TIMER_CFG     0x179
#define REG_DEMOD_DRO_REF_COEF      0x17A
#define REG_DEMOD_DRO_REF_CFG2      0x17C
#define REG_DEMOD_DRO_MASK_CFG      0x17D
#define REG_DEMOD_DRO_RSP_CFG       0x17E
#define REG_DEMOD_DRO_OBS1          0x17F
#define REG_DEMOD_DRO_OBS2          0x200
#define REG_DEMOD_DRO_OBS3          0x201
#define REG_DEMOD_DRO_REF0_1        0x202
#define REG_DEMOD_DRO_REF0_2        0x203
#define REG_DEMOD_DRO_REF1_1        0x204
#define REG_DEMOD_DRO_REF1_2        0x205
#define REG_DEMOD_DRO_REF_DIFF_1    0x206
#define REG_DEMOD_DRO_REF_DIFF_2    0x207
#define REG_DEMOD_DRO_RFA1          0x208
#define REG_DEMOD_DRO_RFA2          0x209
                                    

#define ATD_REG_DEMOD_CR_CFO0     (UINT16) 0x9A2
#define ATD_REG_DEMOD_CR_CFO1     (UINT16) 0x9A3
#define ATD_REG_DEMOD_VOP_HW_CPOH (UINT16) 0x9CC
#define ATD_REG_DEMOD_PRA_ST      (UINT16) 0x95E
#define ATD_REG_DEMOD_PS_CFG      (UINT16) 0x9B1
#define ATD_REG_DEMOD_CTL_RST1    (UINT16) 0x934
#define ATD_REG_DEMOD_CTL_RST2    (UINT16) 0x935
#define ATD_REG_DEMOD_PRA_LP1_CFG02 (UINT16) 0x941
#define ATD_REG_DEMOD_PRA_LP1_CFG12 (UINT16) 0x94B
#define ATD_REG_DEMOD_PRA_LP1_CFG13 (UINT16) 0x94C
                              
#endif
#endif // _MT5360_REGISTER_H_
































