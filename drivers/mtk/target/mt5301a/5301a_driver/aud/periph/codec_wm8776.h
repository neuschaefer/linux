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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: codec_wm8776.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_WM8776.h
 *  Brief of file codec_WM8776.h. \n
 */

#ifndef CODEC_WM8776_H
#define CODEC_WM8776_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"
#include "drvcust_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

// Note that WM8776 has two possible device address, which can be selected using 
// the CE pin, CE=0 => 0x34, CE=1 => 0x36

// I2C Address 
#define WM8776_DEV_ADDR_LOW             ((UINT8)0x34)
#define WM8776_DEV_ADDR_HIGH            ((UINT8)0x36)

#define WM8776_DEV_ADDR                 WM8776_DEV_ADDR_HIGH

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define SIF_CLK_DIV 0x100

 /********************************************************************************
   *                                                                              *
   *  WOLFSONW 24-bit, 192kHz Stereo Codec with 5 Channel I/P Multiplexer WM8776. *
   *                           Register definition                                *
   ********************************************************************************/

/* Headphone Analogue Attenuation (Headphone Left, right, master)  */
#define WM8776_REG00            (0x00)
#define WM8776_REG01            (0x01)
#define WM8776_REG02            (0x02)
        /* Attenuation data for HEADPNONE  channel in 1dB steps */
        #define  B_ADDAC_HP_ATTE         ((UINT16)0x007F <<  0)
        /* HEADPNONE zero cross detect enable */
        #define  B_ADDAC_HP_ZCEN         ((UINT16)0x0001 <<  7)
                 #define D_ADDA_HP_ZCEN                      (0x0080)
        /* Control simultaneous update of all Attenuation latches */
        #define  B_ADDAC_HP_UPDATE       ((UINT16)0x0001 <<  8)
                 #define D_ADDA_HP_LATCH                     (0x0100)


/* DAC Digital Attenuation (DAC Left, right, master)  */
#define WM8776_REG03            (0x03)
#define WM8776_REG04            (0x04)
#define WM8776_REG05            (0x05)
        /* Attenuation data for DAC channel in 0.5dB steps */
        #define  B_ADDAC_DAC_ATTE        ((UINT16)0x00FF <<  0)
        /* Control simultaneous update of all Attenuation latches */
        #define  B_ADDAC_DAC_UPDATE      ((UINT16)0x0001 <<  8)
                 #define D_ADDA_DAC_LATCH                    (0x0100)


/* DAC output Phase swaps register*/
#define WM8776_REG06            (0x06)
        /* DAC Phase invert */
        #define  B_DACL_PHASE_INV        ((UINT16)0x0001 <<  0)
        #define  B_DACR_PHASE_INV        ((UINT16)0x0001 <<  1)

/* DAC Control */
#define WM8776_REG07            (0x07)
        /* DAC Digital Volume Zero Cross Enable */
        #define  B_DAC_DZC_EN            ((UINT16)0x0001 <<  0)
        /* DAC Attenuator Control */
        #define  B_DAC_ATC               ((UINT16)0x0001 <<  1)
        /* Infinite zero detection circuit control and automute control */
        #define  B_DAC_IZD               ((UINT16)0x0001 <<  2)
        /* DAC and ADC analogue zero cross detect timeout disable */
        #define  B_DAC_TOD               ((UINT16)0x0001 <<  3)
        /* DAC Output Control */
        #define  B_DAC_PL                ((UINT16)0x000F <<  4)
                 #define D_DAC_PL_M_M                        (0x0000)
                 #define D_DAC_PL_L_M                        (0x0010)
                 #define D_DAC_PL_R_M                        (0x0020)
                 #define D_DAC_PL_LR2_M                      (0x0030)
                 #define D_DAC_PL_M_L                        (0x0040)
                 #define D_DAC_PL_L_L                        (0x0050)
                 #define D_DAC_PL_R_L                        (0x0060)
                 #define D_DAC_PL_LR2_L                      (0x0070)
                 #define D_DAC_PL_M_R                        (0x0080)
                 #define D_DAC_PL_L_R                        (0x0090)
                 #define D_DAC_PL_R_R                        (0x00A0)
                 #define D_DAC_PL_LR2_R                      (0x00B0)
                 #define D_DAC_PL_M_LR2                      (0x00C0)
                 #define D_DAC_PL_L_LR2                      (0x00D0)
                 #define D_DAC_PL_R_LR2                      (0x00E0)
                 #define D_DAC_PL_LR2_LR2                    (0x00F0)

/* DAC mute */
#define WM8776_REG08            (0x08)
        /* DAC Soft Mute select */
        #define  B_DAC_SOFT_MUTE         ((UINT16)0x0001 <<  0)


/* DAC control register */
#define WM8776_REG09            (0x09)
        /* DAC De-emphasis mode select bits */
        #define  B_DAC_DEEMP             ((UINT16)0x0001 <<  0)
                 #define D_DAC_DEEM_MODE                     (0x0001)
        /* DAC zero flag */
        #define  B_DAC_DZFM              ((UINT16)0x0003 <<  1)


/* Digital audio interface control register */
#define WM8776_REG0A            (0x0A)
#define WM8776_REG0B            (0x0B)
        /* ADC/DAC interface format select bits */
        #define  B_ADDAC_INF_FORMAT      ((UINT16)0x0003 <<  0)
                 #define D_ADDA_R_JUST                       (0x0000)
                 #define D_ADDA_L_JUST                       (0x0001)
                 #define D_ADDA_I2S                          (0x0002)
                 #define D_ADDA_DSP                          (0x0003)
        /* ADC/DAC LR polarity bits */
        #define  B_ADDAC_INF_LR_POLTY    ((UINT16)0x0001 <<  2)
        /* ADC/DAC BCLK polarity bits */
        #define  B_ADDAC_INF_BCLK_POLTY  ((UINT16)0x0001 <<  3)
        /* ADC/DAC input word length bits */
        #define  B_ADDAC_INF_WL          ((UINT16)0x0003 <<  4)
                 #define D_ADDA_WL_16                        (0x0000)
                 #define D_ADDA_WL_20                        (0x0010)
                 #define D_ADDA_WL_24                        (0x0020)
                 #define D_ADDA_WL_32                        (0x0030)

/* Master Mode Control register */
#define WM8776_REG0C            (0x0C)
        /* Master Mode ADCMCLK/ADCLRC ratio select */
        #define  B_ADC_RATE              ((UINT16)0x0007 <<  0)
                 #define D_ADC_RATE_256FS                    (0x0002)
                 #define D_ADC_RATE_384FS                    (0x0003)
                 #define D_ADC_RATE_512FS                    (0x0004)
        /* ADC oversample rate select */
        #define  B_ADC_OSR               ((UINT16)0x0001 <<  3)
                 #define D_ADC_OSR_128                       (0x0000)
                 #define D_ADC_OSR_64                        (0x0008)
        /* Master Mode DACMCLK/DACLRC ratio select */
        #define  B_DAC_RATE              ((UINT16)0x0007 <<  4)
                 #define D_DAC_RATE_128FS                    (0x0000)
                 #define D_DAC_RATE_192FS                    (0x0010)
                 #define D_DAC_RATE_256FS                    (0x0020)
                 #define D_DAC_RATE_384FS                    (0x0030)
                 #define D_DAC_RATE_512FS                    (0x0040)
        /* DAC Master/Slave interface mode select */
        #define  B_DAC_MS                ((UINT16)0x0001 <<  7)
                 #define D_DAC_MASTER_MODE                   (0x0080)
                 #define D_DAC_SLAVE_MODE                    (0x0000)
        /* ADC Master/Slave interface mode select */
        #define  B_ADC_MS                ((UINT16)0x0001 <<  8)
                 #define D_ADC_MASTER_MODE                   (0x0100)
                 #define D_ADC_SLAVE_MODE                    (0x0000)


/* Power down control register */
#define WM8776_REG0D            (0x0D)
        /* Chip power down control bit */
        #define  B_ADDA_PDWN             ((UINT16)0x0001 <<  0)
        /* ADC power down control bit */
        #define  B_ADDA_ADCPD            ((UINT16)0x0001 <<  1)
        /* DAC power down control bit */
        #define  B_ADDA_DACPD            ((UINT16)0x0001 <<  2)
        /* Headphone Output/PGA's power down control bit */
        #define  B_ADDA_HPPD             ((UINT16)0x0001 <<  3)
        /* AINPD power down control bit */
        #define  B_ADDA_AINPD            ((UINT16)0x0001 <<  6)


/* Attenuation ADCL / ADCR */
#define WM8776_REG0E            (0x0E)
#define WM8776_REG0F            (0x0F)
        /* Attenuation data for L/R channel ADC gain in 0.5dB steps */
        #define  B_ADC_ATTEN_GAIN        ((UINT16)0x00FF <<  0)
        #define  B_ADC_ZERO_CROSS_EN     ((UINT16)0x0001 <<  8)
                 #define D_ADC_ZE_EN                         (0x0100)


/* ALC control 1 */
#define WM8776_REG10            (0x10)
        /* Limiter threshold /ALC target level */
        #define  B_ALC_LCT               ((UINT16)0x000F <<  0)
        /* Set Maximum Gain of PGA */
        #define  B_ALC_MAXGAIN           ((UINT16)0x0007 <<  4)
        /* ALC/Limiter function select */
        #define  B_ALC_LCSEL             ((UINT16)0x0003 <<  7)


/* ALC control 2 */
#define WM8776_REG11            (0x11)
        /* ALC hold time before gain is increased. */
        #define  B_ALC_HLD               ((UINT16)0x000F <<  0)
        /* ALC uses zero corss detection circuit. */
        #define  B_ALC_ALCZC             ((UINT16)0x0001 <<  7)
        /* Enable Gain control circuit */
        #define  B_ALC_LCEN              ((UINT16)0x0001 <<  8)


/* ALC control 3 */
#define WM8776_REG12            (0x12)
        /* ALC/Limiter attack (gain ramp-down) time */
        #define  B_ALC_ATK               ((UINT16)0x000F <<  0)
        /* ALC/Limiter decay (gain ramp-down) time */
        #define  B_ALC_DCY               ((UINT16)0x000F <<  4)
        /* Frequency dependant decay enable (Limiter only) */
        #define  B_ALC_FDECAY            ((UINT16)0x0001 <<  8)


/* ALC Noise Gate */
#define WM8776_REG13            (0x13)
        /* Noise gate enable (ALC only) */
        #define  B_ALC_NOISE_GATE        ((UINT16)0x0001 <<  0)
        /* Noise gate threshold (ALC only) */
        #define  B_ALC_NGATE_TH          ((UINT16)0x0007 <<  2)


/* Limiter Control */
#define WM8776_REG14            (0x14)
        /* Maximum attenuation of PGA */
        #define  B_LC_MAXATTEN           ((UINT16)0x000F <<  0)
        /* Length of Transient Window */
        #define  B_LC_TRANWIN            ((UINT16)0x0007 <<  4)
        /* Gain change allowable per zero cross */
        #define  B_LC_CHGPERZC           ((UINT16)0x0003 <<  7)


/* ADC Mux Control */
#define WM8776_REG15            (0x15)
        /* ADC left channel input mixer control bit */
        #define  B_ADC_INPUT_MUX         ((UINT16)0x001F <<  0)
                 #define D_ADC_IN1L1R                        (0x0001)
                 #define D_ADC_IN2L2R                        (0x0002)
                 #define D_ADC_IN3L3R                        (0x0004)
                 #define D_ADC_IN4L4R                        (0x0008)
                 #define D_ADC_IN5L5R                        (0x0010)
        /* ADC Left/Right Mute select bits */
        #define  B_ADC_LR_MUTE           ((UINT16)0x0003 <<  6)
                 #define D_ADC_MUTE_RIGHT                    (0x0040)
                 #define D_ADC_MUTE_LEFT                     (0x0080)
                 #define D_ADC_MUTE_BOTH                     (0x00C0)
                 #define D_ADC_MUTE_NONE                     (0x0000)
        /* ADC LRBOTH */
        #define  B_ADC_LRBOTH            ((UINT16)0x0001 <<  8)

 
/* VOUT Output Mux  */
#define WM8776_REG16            (0x16)
        /*  VOUT Output select (Analogue ByPass Enable/Disable) */
        #define  B_VOUT_OUTPUT_MUX       ((UINT16)0x0007 <<  0)
                 #define D_VOUT_DAC                          (0x0001)
                 #define D_VOUT_AUX                          (0x0002)
                 #define D_VOUT_BYPASS                       (0x0004)

/* Software Register Reset */
#define WM8776_REG17            (0x17)


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern CODEC_T* _CODEC_GetWM8776Tbl(void);

#endif /* CODEC_WM8776_H */

