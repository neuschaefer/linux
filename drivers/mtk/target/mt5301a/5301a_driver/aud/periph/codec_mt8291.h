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
 * $RCSfile: codec_mt8291.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_MT8291.h
 *  Brief of file codec_MT8291.h. \n
 */

#ifndef CODEC_MT8291_H
#define CODEC_MT8291_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"
#include "drvcust_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

// I2C Address 
#define MT8291_DEV_ADDR				((UINT8)(0x36<<1))  // (0x36<<1)

// Note that WM8776 has two possible device address, which can be selected using 
// the CE pin, CE=0 => 0x34, CE=1 => 0x36
// I2C Address 
//#define WM8776_DEV_ADDR_LOW             ((UINT8)0x34)
//#define WM8776_DEV_ADDR_HIGH            ((UINT8)0x36)
//#define WM8776_DEV_ADDR                 WM8776_DEV_ADDR_HIGH

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define SIF_CLK_DIV 0x100

 /********************************************************************************
   *                                                                              									 *
   *  MTK 24-bit, 192kHz Stereo, Multi-Channel Audio CODEC MT8291. 						 *
   *                           Register definition                                								 *
   ********************************************************************************/

/* Headphone Analogue Attenuation (Headphone Left, right, master)  */
#define REG_MT8291_HP_L 			(0x00)
#define REG_MT8291_HP_R			(0x01)
#define REG_MT8291_HP_M			(0x02)
	 #define  MT8291_HP_MASK				(0x1F <<  0)
        #define  MT8291_HP_0DB         			(0x1F <<  0)
        /* HEADPNONE zero cross detect enable */
        #define  MT8291_HPM_ZCEN         		(0x1 <<  7)

/* Headphone Control simultaneous update of all Attenuation latches */
#define REG_MT8291_HP_UPDATE	(0x03)
	 #define  MT8291_HPL_UPDATE     		(0x1 <<  0)
	 #define  MT8291_HPR_UPDATE    		(0x1 <<  1)
	 #define  MT8291_HPM_UPDATE    		(0x1 <<  2)        

/* DAC Digital Attenuation (Left, right, master)  */
#define REG_MT8291_DAC0L_ATEN	(0x05)
#define REG_MT8291_DAC0R_ATEN   	(0x06)
#define REG_MT8291_DAC0M_ATEN  	(0x07)
#define REG_MT8291_DAC1L_ATEN    	(0x08)
#define REG_MT8291_DAC1R_ATEN   	(0x09)
#define REG_MT8291_DAC1M_ATEN   	(0x0a)
        /* Attenuation data for DAC channel in 0.5dB steps */
	 #define  MT8291_DAC_Vlo_MASK		(0xFF <<  0)
        #define  MT8291_DAC_initial_vol_0DB     	(0xFF <<  0)
        
 
/* DAC Control simultaneous update of all Attenuation latches */
#define REG_MT8291_DAC_UPDATE	(0x0b)
	#define  MT8291_DAC0L_UPDATE      		(0x1 <<  0)
	#define  MT8291_DAC0R_UPDATE      		(0x1 <<  1)
	#define  MT8291_DAC0M_UPDATE      		(0x1 <<  2)
	#define  MT8291_DAC1L_UPDATE      		(0x1 <<  3)
	#define  MT8291_DAC1R_UPDATE      		(0x1 <<  4)
	#define  MT8291_DAC1M_UPDATE      		(0x1 <<  5)

#define REG_MT8291_DAC0_LCH_PGA_GAIN  (0x0c)
#define REG_MT8291_DAC0_RCH_PGA_GAIN  (0x0d)
#define REG_MT8291_DAC1_LCH_PGA_GAIN  (0x0e)
#define REG_MT8291_DAC1_RCH_PGA_GAIN  (0x0f)

#define REG_MT8291_DAC_OUPUT_PGA_CTRL	(0x13)
	#define MT8291_PGA_MUTE			(0xf3 << 0)
	#define MT8291_PGA_UNMUTE			(0x00 << 0)
	#define  MT8291_HP_MUTEL				(0x1 << 0)
	#define  MT8291_HP_MUTER				(0x1 << 1)
	#define  MT8291_DAC0_MUTEL			(0x1 << 4)
	#define  MT8291_DAC0_MUTER			(0x1 << 5)
	#define  MT8291_DAC1_MUTEL			(0x1 << 6)
	#define  MT8291_DAC1_MUTER			(0x1 << 7)

#define REG_MT8291_DAC_GAIN_OFFSET (0x19)

/* Power down control register */
#define REG_MT8291_POW            	(0x26)
        /* Chip power down control bit */
        #define  MT8291_ADDA_PDWN             	(0x1 <<  0)
        /* ADC power down control bit */
        #define  MT8291_ADC_L_PD   			(0x1 <<  1)
        #define  MT8291_ADC_R_PD   			(0x1 <<  2)
        /* AINPD power down control bit */
        #define  MT8291_AINPD            			(0x1 <<  3)
        /* DAC power down control bit */
        #define  MT8291_DAC_R0_PD            	(0x1 <<  4)
        #define  MT8291_DAC_R1_PD            	(0x1 <<  5)
        #define  MT8291_DAC_L0_PD            	(0x1 <<  6)
        #define  MT8291_DAC_L1_PD            	(0x1 <<  7)

/* Headphone Output/PGA's power down control bit */
#define REG_MT8291_POW2			(0x27)
	 #define MT8291_PGA_PD				(0x1  << 0)
	 #define MT8291_DAC0_PD				(0x1  << 1)
	 #define MT8291_DAC1_PD				(0x1  << 2)
        #define  MT8291_HP_PD             		(0x1 <<  3)



/* DAC Control */
#define REG_MT8291_DAC0_CTRL1    	(0x28)
#define REG_MT8291_DAC1_CTRL1    	(0x2b)
	/* DAC Digital Volume Zero Cross Enable */
	 #define MT8291_DAC_ZCEN0			(0x1 << 0)
	/* DAC Attenuator Control */
        #define  MT8291_DAC_ATSEL0               	(0x1 <<  1)
       /* Infinite zero detection circuit control and automute control */
        #define  MT8291_DAC_IDZ0               	(0x1 <<  2)
	/* DAC and ADC analogue zero cross detect timeout disable */
        #define  MT8291_DAC_ZCTOUTEN    		(0x1 <<  3)
       /* DAC Output Control */
        #define  MT8291_DAC_OUTSEL_MASK	(0xF  << 4)
		   #define DAC_SEL_M_M               	(0x0  << 4)
                 #define DAC_SEL_L_M              	(0x1  << 4)
                 #define DAC_SEL_R_M                	(0x2  << 4)
                 #define DAC_SEL_LR2_M              	(0x3  << 4)
                 #define DAC_SEL_M_L                   	(0x4  << 4)
                 #define DAC_SEL_L_L                   	(0x5  << 4)
                 #define DAC_SEL_R_L                  	(0x6  << 4)
                 #define DAC_SEL_LR2_L                	(0x7  << 4)
                 #define DAC_SEL_M_R                	(0x8  << 4)
                 #define DAC_SEL_L_R                  	(0x9  << 4)
                 #define DAC_SEL_R_R                  	(0xA  << 4)
                 #define DAC_SEL_LR2_R                	(0xB  << 4)
                 #define DAC_SEL_M_LR2               	(0xC  << 4)
                 #define DAC_SEL_L_LR2                	(0xD  << 4)
                 #define DAC_SEL_R_LR2               	(0xE	  << 4)
                 #define DAC_SEL_LR2_LR2           	(0xF   << 4)



/* DAC soft mute, output Phase swaps, de-emphasis mode register*/
#define REG_MT8291_DAC0_CTRL2	(0x29)	
#define REG_MT8291_DAC1_CTRL2	(0x2C)	
	 /* Soft Mute select */
	#define  MT8291_DAC_SOFT_MUTE	   	(0x1 << 0)
        /* Phase invert */
        #define  MT8291_DAC_PHASE_INV        (0x1 << 1)
	/* de-emphasis */
	 #define  MT8291_DAC_DEEMP		   	(0x1 << 3)

/* DAC control register */
//#define MT8291_REG09            (0x09)
//?????
        /* DAC zero flag */
//        #define  B_DAC_DZFM              ((UINT16)0x0003 <<  1)


/* DAC interface control register */
#define REG_MT8291_DAC0_INF_CTRL		(0x2E)
#define REG_MT8291_DAC1_INF_CTRL		(0x2F)
#define MT8291_DACFMTCFG_MASK			(0x77  << 0)
        /* DAC interface format select bits */
        #define  MT8291_DACFMT_MASK      		(0x7 <<  0)
                 #define MT8291_DACFMT_RJ      		(0x0  << 0)
                 #define MT8291_DACFMT_LJ      		(0x1  << 0)
                 #define MT8291_DACFMT_I2S      	(0x2  << 0)
        /* DAC BCLK polarity bits */
        #define  MT8291_DAC_INF_BCLK_POLTY	(0x1 <<  3)
        /* DAC input word length bits */
        #define MT8291_DAC_INF_WL_MASK 		(0x2 <<  4)
                 #define MT8291_DAC_WL_16			(0x0  << 4)
                 #define MT8291_DAC_WL_20 		(0x1  << 4)
                 #define MT8291_DAC_WL_24 		(0x2  << 4)
	/* DAC LRCK polarity bits */
        #define  MT8291_DAC_INF_LRCK_MASK   	(1 << 6)
            #define  MT8291_DAC_INF_LRCK_POLTY	(1 <<  6)
            #define  MT8291_DAC_INF_LRCK_POLTY0 (0 <<  6)

/* DAC Master Mode Control register */
#define REG_MT8291_DAC0_I2S_CTRL		(0x31)
#define REG_MT8291_DAC1_I2S_CTRL		(0x32)
	 #define MT8291_DAC_I2S_MASK			(0xf  << 0)
        /* Master Mode DACMCLK/DACLRC ratio select */
        #define  MT8291_DAC_RATE_MASK		(0x7  << 0)
                 #define MT8291_DAC_RATE_128FS	(0x0  << 0)
                 #define MT8291_DAC_RATE_192FS	(0x1  << 0)
                 #define MT8291_DAC_RATE_256FS	(0x2  << 0)
                 #define MT8291_DAC_RATE_384FS	(0x3  << 0)
                 #define MT8291_DAC_RATE_512FS	(0x4  << 0)
                 #define MT8291_DAC_RATE_768FS	(0x5  << 0)
        /* DAC Master/Slave interface mode select */
        #define MT8291_DAC_M_MODE			(0x1  << 3)
        #define MT8291_DAC_S_MODE			(0x0  << 3)
	/* DAC I2S source select */
                 #define MT8291_DACSEL				(0x3  << 4)
                 #define MT8291_DAC_FROM_ADC		(0x0  << 4)
                 #define MT8291_DAC_FROM_DAC0	(0x1  << 4)
                 #define MT8291_DAC_FROM_IO0		(0x2  << 4)
                 #define MT8291_DAC_FROM_IO1		(0x3  << 4)
	/* DAC CLK source select */
       	   #define MT8291_DAC_SYN_M0		(0x0  << 6)
       	   #define MT8291_DAC_SYN_M1		(0x1  << 6)

//ADC interface is seperate out from DAC interface config
/* ADC interface control register */
#define REG_MT8291_ADC_INF_CTRL		(0x33)
#define MT8291_ADCFMTCFG_MASK		(0x77  << 0)
        /* ADC interface format select bits */
        #define  MT8291_ADCFMT_MASK      		(0x7  << 0)
                 #define MT8291_ADCFMT_RJ	    		(0x0  << 0)
                 #define MT8291_ADCFMT_LJ			(0x1  << 0)
                 #define MT8291_ADCFMT_I2S		(0x2  << 0)
        /* ADC BCLK polarity bits */
        #define  MT8291_ADC_INF_BCLK_POLTY 	(0x1  << 3)
        /* ADC input word length bits */
        #define  MT8291_ADC_INF_WL_MASK		(0x2  << 4)
                 #define MT8291_ADC_WL_16			(0x0  << 4)
                 #define MT8291_ADC_WL_20			(0x1  << 4)
                 #define MT8291_ADC_WL_24			(0x2  << 4)
	/* ADC LRCK polarity bits */
        #define  MT8291_ADC_INF_LRCK_MASK        (0x1  << 6)
                #define  MT8291_ADC_INF_LRCK_POLTY	(1  << 6)
                #define  MT8291_ADC_INF_LRCK_POLTY0 (0  << 6) 


//ADC interface is seperate out from DAC interface config
/* ADC Master Mode Control register */
#define REG_MT8291_ADC_I2S_CTRL		(0x34)
	 #define MT8291_ADC_I2S_MASK			(0x1f  << 0)
        /* Master Mode ADCMCLK/ADCLRC ratio select */
        #define  MT8291_ADC_RATE_MASK          	(0x7  << 0)
                 #define MT8291_ADC_RATE_256FS	(0x2  << 0)
                 #define MT8291_ADC_RATE_384FS	(0x3  << 0)
                 #define MT8291_ADC_RATE_512FS	(0x4  << 0)
                 #define MT8291_ADC_RATE_768FS	(0x5  << 0)
        /* ADC Master/Slave interface mode select */
        #define  MT8291_ADC_MS_SEL_MASK	(0x1  << 3)
                 #define MT8291_ADC_M_MODE		(0x1  << 3)
                 #define MT8291_ADC_S_MODE		(0x0  << 3)
        /* ADC oversample rate select */
        #define MT8291_ADC_OSR_128			(0x0  << 3)
        #define MT8291_ADC_OSR_64			(0x1  << 3)
        /* ADC CLK source select */
       	   #define MT8291_ADC_SYN_M0		(0x0  << 6)
       	   #define MT8291_ADC_SYN_M1		(0x1  << 6)


/* Attenuation ADCL / ADCR */
#define REG_MT8291_ADC_L_ATTEN		(0x1B)
#define REG_MT8291_ADC_R_ATTEN		(0x1C)
        /* Attenuation data for L/R channel ADC gain in 0.5dB steps */
        #define  MT8291_ADC_ATTEN_GAIN        	(0xFF  << 0)

/* ADC Input Control */
#define REG_MT8291_ADC_INPUT      		(0x1D)
        /* ADC Left/Right Mute select bits */
        #define  MT8291_ADC_L_MUTE           		(0x1 <<  0)
        #define  MT8291_ADC_R_MUTE           		(0x1 <<  1)
	 /* both channel mute see REG 0x26 */ 
        /* ADC enable zero cross detection */  
        #define  MT8291_ADC_L_ZERO_CROSS_EN	(0x1 <<  2)
        #define  MT8291_ADC_R_ZERO_CROSS_EN	(0x1 <<  3)

/* ADC Mux Control */
#define REG_MT8291_INPUT_MUX            	(0x1E)
        /* ADC left channel input mixer control bit */
        #define  MT8291_ADC_INPUT_MUX_MASK		(0xff  << 0)
                 #define MT8291_ADC_IN1L1R              	(0x1  << 0)
                 #define MT8291_ADC_IN2L2R                 (0x2  << 0)
                 #define MT8291_ADC_IN3L3R                 (0x4  << 0)
                 #define MT8291_ADC_IN4L4R                 (0x8  << 0)
                 #define MT8291_ADC_IN5L5R                 (0x10  << 0)
                 #define MT8291_ADC_IN6L6R                 (0x20  << 0)
                 #define MT8291_ADC_IN7L7R                 (0x40  << 0)

        /* ADC LRBOTH */
//        #define  B_ADC_LRBOTH            ((UINT16)0x0001 <<  8)



/* AGC control 1 */
#define REG_MT8291_AGC1            		(0x36)
        /* Limiter threshold /ALC target level */
        #define  MT8291_AGC_THRLD               		(0xF <<  0)
        /* Set Maximum Gain of PGA */
        #define  MT8291_AGC_MAXGAIN           		(0x7 <<  4)


/* AGC control 2 */
#define REG_MT8291_AGC2            		(0x37)
        /* ALC/Limiter function select */
        #define  MT8291_AGC_SEL             			(0x3 <<  0)
        /* ALC hold time before gain is increased. */
        #define  MT8291_AGC_UP_HLD               		(0xF <<  2)
        /* ALC uses zero corss detection circuit. */
        #define  MT8291_AGC_ZCEN             		(0x1 <<  6)
        /* Enable Gain control circuit */
        #define  MT8291_AGC_EN   					(0x1 <<  7)


/* AGC control 3 */
#define REG_MT8291_AGC3            		(0x38)
        /* ALC/Limiter attack (gain ramp-down) time */
        #define  MT8291_RDN               				(0xF <<  0)
        /* ALC/Limiter decay (gain ramp-down) time */
        #define  MT8291_RUP               				(0xF <<  4)

	//mt8291 without this
        /* Frequency dependant decay enable (Limiter only) */
        //#define  B_ALC_FDECAY            ((UINT16)0x0001 <<  8)

/* AGC Noise Gate */
#define REG_MT8291_AGC_N            		(0x39)
        /* Noise gate enable (ALC only) */
        #define  MT8291_NGAT_EN					(0x1 <<  0)
        /* Noise gate threshold (ALC only) */
        #define  MT8291_NGATE_THRLD          		(0x7 <<  2)


/* Limiter Control */
#define REG_MT8291_AGC_LIMITER		(0x3A)
        /* Maximum attenuation of PGA */
        #define  MT8291_MAX_ATTEN           			(0xF <<  0)

	//mt8291 without this
        /* Length of Transient Window */
        //#define  B_LC_TRANWIN            ((UINT16)0x0007 <<  4)
        /* Gain change allowable per zero cross */
        //#define  B_LC_CHGPERZC           ((UINT16)0x0003 <<  7)

#define REG_MT8291_DAC_SWRESET		(0x46)
	#define  MT8291_DAC1_256FS_RESET			(0x0 <<  6)
	#define  MT8291_DAC2_256FS_RESET			(0x0 <<  7)

 
/* MT8291 without VOUT Output Mux  */


/* MT8291 without Software Register Reset *
  * In fact, wm8776 also without this reg      */
//#define WM8776_REG17            (0x17)


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern CODEC_T* _CODEC_GetMT8291Tbl(void);

void _MT8291_WRITE(UINT8 u1Addr, UINT8 u1Data);
UINT8 _MT8291_READ(UINT8 u1Addr) ;
void _MT8291_Init(void);
void _MT8291_Enable(BOOL fgEnalbe);
void _MT8291_Mute(UINT8 u1DacId,BOOL fgMute);
void _MT8291_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT8291_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT8291_ChlSel(UINT8 u1Indx);
void _MT8291_HpVolCtl(UINT8 u1HpId, INT16 i2Value);
void _MT8291_SetAdcGain(UINT8 u1DecId, INT16 i2Value);
void _MT8291_PLLReset(void);

#endif /* CODEC_MT8291_H */


