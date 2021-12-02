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


/*********** bank 0 Register Addr. **************/

/*********** bank 1 Register Addr. **************/

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
                              
#endif // _MT5360_REGISTER_H_

































