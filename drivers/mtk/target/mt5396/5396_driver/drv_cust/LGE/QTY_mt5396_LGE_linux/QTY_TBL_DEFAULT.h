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
// Quality index is built at 07/08/10 9:44:03 PM
{
    // Quality Table of General
    0x82, // QUALITY_CONTRAST
    0x00, // QUALITY_CONTRAST_MSB
    0x80, // QUALITY_BRIGHTNESS
    0x9C, // QUALITY_SATURATION
    0x00, // QUALITY_SATURATION_MSB
    0x80, // QUALITY_HUE
    // Quality Table of HSHARP
    0x40, //QUALITY_HSHARP_LOW
    0x40, //QUALITY_HSHARP_LOW_NEG
    0x40, //QUALITY_HSHARP_MID
    0x40, //QUALITY_HSHARP_MID_NEG
    0x40, //QUALITY_HSHARP_HIGH
    0x40, //QUALITY_HSHARP_HIGH_NEG
    0x05, //QUALITY_HSHARP_LOW_CORING
    0x05, //QUALITY_HSHARP_MID_CORING
    0x05, //QUALITY_HSHARP_HIGH_CORING
    0xFF, //QUALITY_HSHARP_LOW_LIMIT_POS
    0xFF, //QUALITY_HSHARP_LOW_LIMIT_NEG
    0xFF, //QUALITY_HSHARP_MID_LIMIT_POS
    0xFF, //QUALITY_HSHARP_MID_LIMIT_NEG
    0xFF, //QUALITY_HSHARP_HIGH_LIMIT_POS
    0xFF, //QUALITY_HSHARP_HIGH_LIMIT_NEG
    0x00, //QUALITY_HSHARP_CLIP_POS_EN
    0x00, //QUALITY_HSHARP_CLIP_NEG_EN
    0x00, //QUALITY_HSHARP_GB_CLIP_POS_EN_L
    0x00, //QUALITY_HSHARP_GB_CLIP_NEG_EN_L
    0x00, //QUALITY_HSHARP_GB_CLIP_POS_EN_M
    0x00, //QUALITY_HSHARP_GB_CLIP_NEG_EN_M
    0x00, //QUALITY_HSHARP_GB_CLIP_POS_EN_H
    0x00, //QUALITY_HSHARP_GB_CLIP_NEG_EN_H
    0x00, //QUALITY_HSHARP_CLIP_FRONT_THR_LOW
    0x00, //QUALITY_HSHARP_CLIP_FRONT_THR_HIGH
    0x03, //QUALITY_HSHARP_CLIP_POS
    0x03, //QUALITY_HSHARP_CLIP_NEG
    0x00, //QUALITY_HSHARP_SOFT_CLIP_GAIN
    0x40, //QUALITY_HSHARP_LC_LOW
    0x40, //QUALITY_HSHARP_LC_LOW_NEG
    0x40, //QUALITY_HSHARP_LC_MID
    0x40, //QUALITY_HSHARP_LC_MID_NEG
    0x40, //QUALITY_HSHARP_LC_HIGH
    0x40, //QUALITY_HSHARP_LC_HIGH_NEG
    0x00, //QUALITY_HSHARP_LC_LOW_CORING
    0x05, //QUALITY_HSHARP_LC_MID_CORING
    0x00, //QUALITY_HSHARP_LC_HIGH_CORING
    0xFF, //QUALITY_HSHARP_LC_LOW_LIMIT_POS
    0xFF, //QUALITY_HSHARP_LC_LOW_LIMIT_NEG
    0xFF, //QUALITY_HSHARP_LC_MID_LIMIT_POS
    0xFF, //QUALITY_HSHARP_LC_MID_LIMIT_NEG
    0xFF, //QUALITY_HSHARP_LC_HIGH_LIMIT_POS
    0xFF, //QUALITY_HSHARP_LC_HIGH_LIMIT_NEG
    0x00, //QUALITY_HSHARP_LC_CLIP_POS_EN
    0x00, //QUALITY_HSHARP_LC_CLIP_NEG_EN
    0x00, //QUALITY_HSHARP_LC_TABLE_SEL
    0x00, //QUALITY_HSHARP_LC_TABLE_GAIN
    0x00, //QUALITY_HSHARP_LC_TABLE_OFFSET
    // Quality Table of TDSHARP
    0x40, //QUALITY_TDSHARP_H1_GAIN
    0x40, //QUALITY_TDSHARP_H1_GAIN_NEG
    0x40, //QUALITY_TDSHARP_H2_GAIN
    0x40, //QUALITY_TDSHARP_H2_GAIN_NEG
    0x40, //QUALITY_TDSHARP_H3_GAIN
    0x40, //QUALITY_TDSHARP_H3_GAIN_NEG
    0x40, //QUALITY_TDSHARP_H4_GAIN
    0x40, //QUALITY_TDSHARP_H4_GAIN_NEG
    0x40, //QUALITY_TDSHARP_V1_GAIN
    0x40, //QUALITY_TDSHARP_V1_GAIN_NEG
    0x40, //QUALITY_TDSHARP_V2_GAIN
    0x40, //QUALITY_TDSHARP_V2_GAIN_NEG
    0x40, //QUALITY_TDSHARP_X1_GAIN
    0x40, //QUALITY_TDSHARP_X1_GAIN_NEG
    0x40, //QUALITY_TDSHARP_X2_GAIN
    0x40, //QUALITY_TDSHARP_X2_GAIN_NEG
    0x00, //QUALITY_TDSHARP_H1_CORING
    0x00, //QUALITY_TDSHARP_H1_SOFT_COR_GAIN
    0x00, //QUALITY_TDSHARP_H2_CORING
    0x00, //QUALITY_TDSHARP_H2_SOFT_COR_GAIN
    0x00, //QUALITY_TDSHARP_H3_CORING
    0x00, //QUALITY_TDSHARP_H3_SOFT_COR_GAIN
    0x00, //QUALITY_TDSHARP_H4_CORING
    0x00, //QUALITY_TDSHARP_H4_SOFT_COR_GAIN
    0x00, //QUALITY_TDSHARP_V1_CORING
    0x00, //QUALITY_TDSHARP_V1_SOFT_COR_GAIN
    0x00, //QUALITY_TDSHARP_V2_CORING
    0x00, //QUALITY_TDSHARP_V2_SOFT_COR_GAIN
    0x00, //QUALITY_TDSHARP_X1_CORING
    0x00, //QUALITY_TDSHARP_X1_SOFT_COR_GAIN
    0x00, //QUALITY_TDSHARP_X2_CORING
    0x00, //QUALITY_TDSHARP_X2_SOFT_COR_GAIN
    0xFF, //QUALITY_TDSHARP_H1_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_H1_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_H2_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_H2_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_H3_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_H3_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_H4_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_H4_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_V1_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_V1_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_V2_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_V2_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_X1_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_X1_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_X2_LIMIT_POS
    0xFF, //QUALITY_TDSHARP_X2_LIMIT_NEG
    0xFF, //QUALITY_TDSHARP_LIMIT_POS_ALL
    0xFF, //QUALITY_TDSHARP_LIMIT_NEG_ALL
    0xFF, //QUALITY_TDSHARP_H1_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_H1_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_H1_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_H2_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_H2_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_H2_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_H3_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_H3_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_H3_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_H4_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_H4_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_H4_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_V1_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_V1_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_V1_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_V2_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_V2_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_V2_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_X1_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_X1_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_X1_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_X2_CLIP_THPOS
    0xFF, //QUALITY_TDSHARP_X2_CLIP_THNEG
    0x80, //QUALITY_TDSHARP_X2_SOFT_CLIP_GAIN
    0xFF, //QUALITY_TDSHARP_POS_CLIP
    0xFF, //QUALITY_TDSHARP_NEG_CLIP
    0x80, //QUALITY_TDSHARP_CLIP_GAIN
    0x01, //QUALITY_TDSHARP_AC_LPF_EN
    0x08, //QUALITY_TDSHARP_AC_LPF_COE
    // Quality Table of PBC
    0x00, //QUALITY_PBC_EN_1
    0x00, //QUALITY_PBC_BAND_SEL_1
    0x00, //QUALITY_PBC_GAIN_SIGN_1
    0x80, //QUALITY_PBC_GAIN_1
    0x00, //QUALITY_PBC_CORING_1
    0x00, //QUALITY_PBC_THETA_C_1
    0x00, //QUALITY_PBC_RADIUS_C_1
    0x20, //QUALITY_PBC_THETA_RANGE_1
    0x30, //QUALITY_PBC_RADIUS_RANGE_1
    0x00, //QUALITY_PBC_EN_2
    0x00, //QUALITY_PBC_BAND_SEL_2
    0x00, //QUALITY_PBC_GAIN_SIGN_2
    0x80, //QUALITY_PBC_GAIN_2
    0x00, //QUALITY_PBC_CORING_2
    0x00, //QUALITY_PBC_THETA_C_2
    0x00, //QUALITY_PBC_RADIUS_C_2
    0x20, //QUALITY_PBC_THETA_RANGE_2
    0x30, //QUALITY_PBC_RADIUS_RANGE_2
    0x00, //QUALITY_PBC_EN_3
    0x00, //QUALITY_PBC_BAND_SEL_3
    0x00, //QUALITY_PBC_GAIN_SIGN_3
    0x80, //QUALITY_PBC_GAIN_3
    0x00, //QUALITY_PBC_CORING_3
    0x00, //QUALITY_PBC_THETA_C_3
    0x00, //QUALITY_PBC_RADIUS_C_3
    0x00, //QUALITY_PBC_THETA_RANGE_3
    0x00, //QUALITY_PBC_RADIUS_RANGE_3
    // Quality Table of LTI
    0x40, //QUALITY_LTI_GAIN1
    0x40, //QUALITY_LTI_GAIN_NEG1
    0xFF, //QUALITY_LTI_LIMIT_POS1
    0xFF, //QUALITY_LTI_LIMIT_NEG1
    0x00, //QUALITY_LTI_CORING1
    0x00, //QUALITY_LTI_SOFT_CLIP_GAIN1
    0x40, //QUALITY_LTI_GAIN2
    0x40, //QUALITY_LTI_GAIN_NEG2
    0xFF, //QUALITY_LTI_LIMIT_POS2
    0xFF, //QUALITY_LTI_LIMIT_NEG2
    0x00, //QUALITY_LTI_CORING2
    0x00, //QUALITY_LTI_SOFT_CLIP_GAIN2
    // Quality Table of HLTI
    0x20, //QUALITY_HLTI_HDEG_GAIN
    0x0F, //QUALITY_HLTI_HDIFF_OFFSET
    0x20, //QUALITY_HLTI_VDEG_GAIN
    0x0F, //QUALITY_HLTI_VDIFF_OFFSET
    // Quality Table of CTIF
    0x00, //QUALITY_CTIF_ENA
    0x00, //QUALITY_CTIF_HD
    0x00, //QUALITY_CTIF_LPF1
    0x00, //QUALITY_CTIF_LPF1_SEL
    0x00, //QUALITY_CTIF_FLPF
    0x00, //QUALITY_CTIF_FLPF_SEL
    0x07, //QUALITY_CTIF_U_WND_SZ
    0x07, //QUALITY_CTIF_V_WND_SZ
    0x10, //QUALITY_CTIF_U_STB_GAIN
    0x10, //QUALITY_CTIF_V_STB_GAIN
    0x07, //QUALITY_CTIF_U_STB_OFST1
    0x07, //QUALITY_CTIF_V_STB_OFST1
    0x00, //QUALITY_CTIF_U_STB_OFST2
    0x00, //QUALITY_CTIF_V_STB_OFST2
    0x20, //QUALITY_CTIF_FLAT_GAIN
    0x07, //QUALITY_CTIF_FLAT_OFST1
    0x00, //QUALITY_CTIF_FLAT_OFST2
    0x00, //QUALITY_CTIF_COR
    // Quality Table of CTIR
    0x01, //QUALITY_CTIR_ENA
    0x00, //QUALITY_CTIR_HD
    0x00, //QUALITY_CTIR_LPF1
    0x03, //QUALITY_CTIR_LPF1_SEL
    0x01, //QUALITY_CTIR_FLPF
    0x00, //QUALITY_CTIR_FLPF_SEL
    0x07, //QUALITY_CTIR_U_WND_SZ
    0x07, //QUALITY_CTIR_V_WND_SZ
    0x20, //QUALITY_CTIR_U_STB_GAIN
    0x20, //QUALITY_CTIR_V_STB_GAIN
    0x05, //QUALITY_CTIR_U_STB_OFST1
    0x05, //QUALITY_CTIR_V_STB_OFST1
    0x0A, //QUALITY_CTIR_U_STB_OFST2
    0x0A, //QUALITY_CTIR_V_STB_OFST2
    0x10, //QUALITY_CTIR_FLAT_GAIN
    0x03, //QUALITY_CTIR_FLAT_OFST1
    0x14, //QUALITY_CTIR_FLAT_OFST2
    0x05, //QUALITY_CTIR_COR
    // Quality Table of CBE
    0x00, //QUALITY_CBE_WEI_IT3
    0x00, //QUALITY_CBE_WEI_IT5
    // Quality Table of IRR
    0x00, //QUALITY_IRR_EN
    0x00, //QUALITY_IRR_FILTER
    0x00, //QUALITY_IRR_FILTER_CLIP
    0x00, //QUALITY_IRR_LEVEL
    0x00, //QUALITY_IRR_TAP
    0x00, //QUALITY_IRR_EDGE_TH
    0x00, //QUALITY_IRR_EDGE_SLOPE
    0x00, //QUALITY_IRR_LF_TH
    0x00, //QUALITY_IRR_LF_SLOPE
    0x00, //QUALITY_IRR_HF_TH
    0x00, //QUALITY_IRR_HF_SLOPE
    0x00, //QUALITY_IRR_RIPPLE_TH
    0x00, //QUALITY_IRR_DEACY_ER
    0x00, //QUALITY_IRR_DEACY_TH
    0x00, //QUALITY_IRR_FLAT_TH
    0x00, //QUALITY_IRR_FLAT_SLOPE
    0x00, //QUALITY_IRR_FLAT_LEVEL
    // Quality Table of SNR
    0x01, //QUALITY_SNR_MESSSFT_SM_CO1MO
    0x02, //QUALITY_SNR_MESSTHL_SM_CO1MO
    0x02, //QUALITY_SNR_MESSSFT_MESS_CO1MO
    0x03, //QUALITY_SNR_MESSTHL_MESS_CO1MO
    0x01, //QUALITY_SNR_MESSSFT_EDGE_CO1MO
    0x02, //QUALITY_SNR_MESSTHL_EDGE_CO1MO
    0x01, //QUALITY_SNR_MESSSFT_MOS_CO1MO
    0x03, //QUALITY_SNR_MESSTHL_MOS_CO1MO
    0x01, //QUALITY_SNR_MESSSFT_SM_CO1ST
    0x02, //QUALITY_SNR_MESSTHL_SM_CO1ST
    0x01, //QUALITY_SNR_MESSSFT_MESS_CO1ST
    0x02, //QUALITY_SNR_MESSTHL_MESS_CO1ST
    0x01, //QUALITY_SNR_MESSSFT_EDGE_CO1ST
    0x02, //QUALITY_SNR_MESSTHL_EDGE_CO1ST
    0x03, //QUALITY_SNR_MESSSFT_MOS_CO1ST
    0x02, //QUALITY_SNR_MESSTHL_MOS_CO1ST
    0x08, //QUALITY_SNR_BLDLV_BK_CO1
    0x06, //QUALITY_SNR_BLDLV_SM_CO1
    0x05, //QUALITY_SNR_BLDLV_MESS_CO1
    0x08, //QUALITY_SNR_BLDLV_EDGE_CO1
    0x03, //QUALITY_SNR_BLDLV_MOS_CO1
    0x01, //QUALITY_SNR_MESSSFT_SM_CO2MO
    0x01, //QUALITY_SNR_MESSTHL_SM_CO2MO
    0x01, //QUALITY_SNR_MESSSFT_MESS_CO2MO
    0x01, //QUALITY_SNR_MESSTHL_MESS_CO2MO
    0x01, //QUALITY_SNR_MESSSFT_EDGE_CO2MO
    0x01, //QUALITY_SNR_MESSTHL_EDGE_CO2MO
    0x03, //QUALITY_SNR_MESSSFT_MOS_CO2MO
    0x02, //QUALITY_SNR_MESSTHL_MOS_CO2MO
    0x01, //QUALITY_SNR_MESSSFT_SM_CO2ST
    0x01, //QUALITY_SNR_MESSTHL_SM_CO2ST
    0x01, //QUALITY_SNR_MESSSFT_MESS_CO2ST
    0x01, //QUALITY_SNR_MESSTHL_MESS_CO2ST
    0x01, //QUALITY_SNR_MESSSFT_EDGE_CO2ST
    0x01, //QUALITY_SNR_MESSTHL_EDGE_CO2ST
    0x03, //QUALITY_SNR_MESSSFT_MOS_CO2ST
    0x02, //QUALITY_SNR_MESSTHL_MOS_CO2ST
    0x08, //QUALITY_SNR_BLDLV_BK_CO2
    0x08, //QUALITY_SNR_BLDLV_SM_CO2
    0x08, //QUALITY_SNR_BLDLV_MESS_CO2
    0x08, //QUALITY_SNR_BLDLV_EDGE_CO2
    0x02, //QUALITY_SNR_BLDLV_MOS_CO2
    0x03, //QUALITY_SNR_MESSSFT_SM_CO3MO
    0x02, //QUALITY_SNR_MESSTHL_SM_CO3MO
    0x02, //QUALITY_SNR_MESSSFT_MESS_CO3MO
    0x03, //QUALITY_SNR_MESSTHL_MESS_CO3MO
    0x03, //QUALITY_SNR_MESSSFT_EDGE_CO3MO
    0x02, //QUALITY_SNR_MESSTHL_EDGE_CO3MO
    0x03, //QUALITY_SNR_MESSSFT_MOS_CO3MO
    0x06, //QUALITY_SNR_MESSTHL_MOS_CO3MO
    0x03, //QUALITY_SNR_MESSSFT_SM_CO3ST
    0x02, //QUALITY_SNR_MESSTHL_SM_CO3ST
    0x03, //QUALITY_SNR_MESSSFT_MESS_CO3ST
    0x02, //QUALITY_SNR_MESSTHL_MESS_CO3ST
    0x03, //QUALITY_SNR_MESSSFT_EDGE_CO3ST
    0x02, //QUALITY_SNR_MESSTHL_EDGE_CO3ST
    0x03, //QUALITY_SNR_MESSSFT_MOS_CO3ST
    0x02, //QUALITY_SNR_MESSTHL_MOS_CO3ST
    0x08, //QUALITY_SNR_BLDLV_BK_CO3
    0x08, //QUALITY_SNR_BLDLV_SM_CO3
    0x08, //QUALITY_SNR_BLDLV_MESS_CO3
    0x08, //QUALITY_SNR_BLDLV_EDGE_CO3
    0x03, //QUALITY_SNR_BLDLV_MOS_CO3
    0x03, //QUALITY_SNR_MESSSFT_SM_FRST
    0x02, //QUALITY_SNR_MESSTHL_SM_FRST
    0x03, //QUALITY_SNR_MESSSFT_MESS_FRST
    0x02, //QUALITY_SNR_MESSTHL_MESS_FRST
    0x03, //QUALITY_SNR_MESSSFT_EDGE_FRST
    0x02, //QUALITY_SNR_MESSTHL_EDGE_FRST
    0x03, //QUALITY_SNR_MESSSFT_MOS_FRST
    0x02, //QUALITY_SNR_MESSTHL_MOS_FRST
    0x01, //QUALITY_SNR_BLDLV_BK_FRST
    0x01, //QUALITY_SNR_BLDLV_SM_FRST
    0x01, //QUALITY_SNR_BLDLV_MESS_FRST
    0x01, //QUALITY_SNR_BLDLV_EDGE_FRST
    0x01, //QUALITY_SNR_BLDLV_MOS_FRST
    0x03, //QUALITY_SNR_MESSSFT_SM_MO
    0x02, //QUALITY_SNR_MESSTHL_SM_MO
    0x03, //QUALITY_SNR_MESSSFT_MESS_MO
    0x02, //QUALITY_SNR_MESSTHL_MESS_MO
    0x03, //QUALITY_SNR_MESSSFT_EDGE_MO
    0x02, //QUALITY_SNR_MESSTHL_EDGE_MO
    0x03, //QUALITY_SNR_MESSSFT_MOS_MO
    0x02, //QUALITY_SNR_MESSTHL_MOS_MO
    0x08, //QUALITY_SNR_BLDLV_BK_MO
    0x08, //QUALITY_SNR_BLDLV_SM_MO
    0x08, //QUALITY_SNR_BLDLV_MESS_MO
    0x08, //QUALITY_SNR_BLDLV_EDGE_MO
    0x03, //QUALITY_SNR_BLDLV_MOS_MO
    0x03, //QUALITY_SNR_MESSSFT_SM_ST
    0x02, //QUALITY_SNR_MESSTHL_SM_ST
    0x03, //QUALITY_SNR_MESSSFT_MESS_ST
    0x02, //QUALITY_SNR_MESSTHL_MESS_ST
    0x03, //QUALITY_SNR_MESSSFT_EDGE_ST
    0x02, //QUALITY_SNR_MESSTHL_EDGE_ST
    0x03, //QUALITY_SNR_MESSSFT_MOS_ST
    0x02, //QUALITY_SNR_MESSTHL_MOS_ST
    0x08, //QUALITY_SNR_BLDLV_BK_ST
    0x08, //QUALITY_SNR_BLDLV_SM_ST
    0x08, //QUALITY_SNR_BLDLV_MESS_ST
    0x08, //QUALITY_SNR_BLDLV_EDGE_ST
    0x03, //QUALITY_SNR_BLDLV_MOS_ST
    0x03, //QUALITY_SNR_MESSSFT_SM_BK
    0x02, //QUALITY_SNR_MESSTHL_SM_BK
    0x03, //QUALITY_SNR_MESSSFT_MESS_BK
    0x02, //QUALITY_SNR_MESSTHL_MESS_BK
    0x03, //QUALITY_SNR_MESSSFT_EDGE_BK
    0x02, //QUALITY_SNR_MESSTHL_EDGE_BK
    0x03, //QUALITY_SNR_MESSSFT_MOS_BK
    0x02, //QUALITY_SNR_MESSTHL_MOS_BK
    0x08, //QUALITY_SNR_BLDLV_BK_BK
    0x08, //QUALITY_SNR_BLDLV_SM_BK
    0x08, //QUALITY_SNR_BLDLV_MESS_BK
    0x08, //QUALITY_SNR_BLDLV_EDGE_BK
    0x03, //QUALITY_SNR_BLDLV_MOS_BK
    0x01, //QUALITY_SNR_MESSSFT_SM_DEF
    0x02, //QUALITY_SNR_MESSTHL_SM_DEF
    0x03, //QUALITY_SNR_MESSSFT_MESS_DEF
    0x04, //QUALITY_SNR_MESSTHL_MESS_DEF
    0x03, //QUALITY_SNR_MESSSFT_EDGE_DEF
    0x02, //QUALITY_SNR_MESSTHL_EDGE_DEF
    0x03, //QUALITY_SNR_MESSSFT_MOS_DEF
    0x03, //QUALITY_SNR_MESSTHL_MOS_DEF
    0x08, //QUALITY_SNR_BLDLV_SM_DEF
    0x08, //QUALITY_SNR_BLDLV_MESS_DEF
    0x03, //QUALITY_SNR_BLDLV_EDGE_DEF
    0x03, //QUALITY_SNR_BLDLV_MOS_DEF
    0x04, //QUALITY_SNR_CUR_SM_NUM
    0x0C, //QUALITY_SNR_CUR_SM_THR
    0x04, //QUALITY_SNR_NEAREDGE_SELWIDTH
    0x12, //QUALITY_SNR_NEAREDGE_EDGE_THR
    0x00, //QUALITY_SNR_GLOBAL_BLEND
    // Quality Table of TNR
    0x00, //QUALITY_TNR_REG_TBTHX1
    0x00, //QUALITY_TNR_REG_TBTHX2
    0x00, //QUALITY_TNR_REG_TBTHX3
    0x00, //QUALITY_TNR_REG_TBTHX4
    0x00, //QUALITY_TNR_REG_TBTHX5
    0x00, //QUALITY_TNR_REG_TBTHX6
    0x00, //QUALITY_TNR_REG_TBTHX7
    0x00, //QUALITY_TNR_REG_TBTHX8
    0x0A, //QUALITY_TNR_DEF_TBL0
    0x09, //QUALITY_TNR_DEF_TBL1
    0x07, //QUALITY_TNR_DEF_TBL2
    0x05, //QUALITY_TNR_DEF_TBL3
    0x03, //QUALITY_TNR_DEF_TBL4
    0x02, //QUALITY_TNR_DEF_TBL5
    0x01, //QUALITY_TNR_DEF_TBL6
    0x01, //QUALITY_TNR_DEF_TBL7
    0x0E, //QUALITY_TNR_CIIR_TBL0
    0x0C, //QUALITY_TNR_CIIR_TBL1
    0x0A, //QUALITY_TNR_CIIR_TBL2
    0x08, //QUALITY_TNR_CIIR_TBL3
    0x06, //QUALITY_TNR_CIIR_TBL4
    0x04, //QUALITY_TNR_CIIR_TBL5
    0x02, //QUALITY_TNR_CIIR_TBL6
    0x01, //QUALITY_TNR_CIIR_TBL7
    // Quality Table of AL
    0x40, //QUALITY_AL_GAIN
    0x08, //QUALITY_AL_OFFSET
    0xC0, //QUALITY_AL_LIMIT
    0x04, //QUALITY_AL_BWS_BLACK_LEVEL
    0x20, //QUALITY_AL_BWS_BLACK_GAIN
    0x08, //QUALITY_AL_BWS_BLACK_OFFSET
    0x60, //QUALITY_AL_BWS_BLACK_RATIO
    0x14, //QUALITY_AL_BWS_BLACK_LIMIT
    0x04, //QUALITY_AL_BWS_WHITE_LEVEL
    0x40, //QUALITY_AL_BWS_WHITE_GAIN
    0x00, //QUALITY_AL_BWS_WHITE_OFFSET
    0x40, //QUALITY_AL_BWS_WHITE_RATIO
    0x00, //QUALITY_AL_BWS_WHITE_LIMIT
    0x01, //QUALITY_AL_DYNBS_EN
    0x04, //QUALITY_AL_DYNBS_RANGE
    0x30, //QUALITY_AL_DYNBS_PXL_THD1
    0x50, //QUALITY_AL_DYNBS_PXL_THD2
    0x80, //QUALITY_AL_DYNBS_LMT_BTM
    0x98, //QUALITY_AL_DYNBS_LMT_TOP
    0x30, //QUALITY_AL_DYNBS_DIST_THDL
    0x80, //QUALITY_AL_DYNBS_DIST_THDH
    0x80, //QUALITY_AL_DYNBS_GAIN_BTM
    0xA0, //QUALITY_AL_DYNBS_GAIN_TOP
    0x76, //QUALITY_AL_DYNBS_OFST_BTM
    0x80, //QUALITY_AL_DYNBS_OFST_TOP
},
