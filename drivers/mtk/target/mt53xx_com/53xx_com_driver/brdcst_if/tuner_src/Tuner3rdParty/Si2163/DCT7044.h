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
#ifndef  TUNER_DCT7044_SPECIFICS
    #define TUNER_DCT7044_SPECIFICS
    #define TUNER_DLL Emb_L1_RF_DCT7044

    #ifdef BUILDING_DLL
    	#define EMB_L1_RF_DCT7044_API      __declspec(dllimport)
    #else
    	#define EMB_L1_RF_DCT7044_API       /* empty */
    #endif

    #define CHIP_MODE                  chip_mode_pll_xtal
    #define REF_FREQUENCY               16
    #define TUNER_XTAL_MHZ              REF_FREQUENCY
    #define DIV_R                        1
    #define DIV_N                       56
    #define DIV_M                        8
    #define DIV_P                      pll_divp_prescaler_1
    #define DIV_L_DVB_T                 12
    #define DIV_L_DVB_C                 16
    #define DIV_L_DVB_C_LOW_CLK        18
      #define DIV_L_DVB_C_HIGH_CLK                 14	
    #define TUNER_IF_MHZ               36.167000
    #define ADC_SAMPLING_MODE          adc_sampling_mode_if_ovr4

    #define CONTROL_RF_AGC

    #define IF_MODE
    #define SPECTRAL_INVERSION

    #define AGC1_KACQ                   16
    #define AGC1_KLOC                   18
    #define AGC1_POLA                   agc1_pola_inverted

    #define AGC2_MIN                    70

//    #define AGC2_KACQ                   14
    #define AGC2_KACQ                   16
    #define AGC2_KLOC                   18

    #define KP_LOCK                      5

    #define CENTRAL_TAP                 11

    #define TUNER_ADDRESS             0xc0

    #define L1_RF_TUNER_Process             L1_RF_DCT7044_Process
    #define L1_RF_TUNER_Init                L1_RF_DCT7044_Init
    #define L1_RF_TUNER_InitAfterReset      L1_RF_DCT7044_InitAfterReset

    #define L1_RF_TUNER_Get_Infos           L1_RF_DCT7044_Get_Infos
    #define L1_RF_TUNER_Get_I2cChannel      L1_RF_DCT7044_Get_I2cChannel

    #define L1_RF_TUNER_Get_IF              L1_RF_DCT7044_Get_IF
    #define L1_RF_TUNER_Get_RF              L1_RF_DCT7044_Get_RF
    #define L1_RF_TUNER_Get_minRF           L1_RF_DCT7044_Get_minRF
    #define L1_RF_TUNER_Get_maxRF           L1_RF_DCT7044_Get_maxRF
    #define L1_RF_TUNER_Tune                L1_RF_DCT7044_Tune
    #define L1_RF_TUNER_Saw                 L1_RF_DCT7044_Saw
    #define L1_RF_TUNER_Get_Saw             L1_RF_DCT7044_Get_Saw
    #define L1_RF_TUNER_reference_frequency L1_RF_DCT7044_reference_frequency

    #ifdef    LINKING_WITH_TUNER
        #include "Emb_L1_RF_DCT7044_API.h"
    #endif /* LINKING_WITH_TUNER */

#endif /* TUNER_DCT7044_SPECIFICS */
