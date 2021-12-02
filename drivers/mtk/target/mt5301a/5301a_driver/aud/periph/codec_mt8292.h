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
 * $RCSfile: codec_mt8292.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_MT8291.h
 *  Brief of file codec_MT8291.h. \n
 */

#ifndef CODEC_MT8292_H
#define CODEC_MT8292_H

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
#define MT8292_DEV_ADDR				((UINT8)(0x38<<1))

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define SIF_CLK_DIV 0x100

#define REG_MT8292_HP_GAIN			(0x02)
#define REG_MT8292_HP_UPDATE			(0x03)

#define REG_MT8292_PGA0L_GAIN			(0x08)
#define REG_MT8292_PGA0R_GAIN			(0x09)
#define REG_MT8292_PGA1L_GAIN			(0x0a)
#define REG_MT8292_PGA1R_GAIN			(0x0b)
#define REG_MT8292_PGA2L_GAIN			(0x0c)
#define REG_MT8292_PGA2R_GAIN			(0x0d)

#define REG_OUTPUT_PGA_MUTE			(0x0f)
	#define MT8292_PGA2_MUTE_MASK		(0x03 << 6)
	#define MT8292_PGA1_MUTE_MASK		(0x03 << 4)
	#define MT8292_PGA0_MUTE_MASK		(0x03 << 2)
	#define MT8292_HP_MUTE_MASK			(0x03 << 0)

#define REG_MT8292_MUX_MUTE			(0x10)
/* ADC Mux Control */
#define REG_MT8292_INPUT_MUX            	(0x11)
        /* ADC left channel input mixer control bit */
        #define  MT8292_ADC_INPUT_MUX_MASK		(0xff  << 0)
                 #define MT8292_ADC_CLOSE               	(0x0<<0)        
                 #define MT8292_ADC_IN1L1R              	(0x1  << 0)
                 #define MT8292_ADC_IN2L2R                 (0x2  << 0)
                 #define MT8292_ADC_IN3L3R                 (0x4  << 0)
                 #define MT8292_ADC_IN4L4R                 (0x8  << 0)
                 #define MT8292_ADC_IN5L5R                 (0x10  << 0)
                 #define MT8292_ADC_IN6L6R                 (0x20  << 0)
                 #define MT8292_ADC_IN7L7R                 (0x40  << 0)
                 #define MT8292_ADC_IN8L8R                 (0x80  << 0)


#define REG_MT8292_RAMPMUTE			(0x15)
#define REG_MT8292_SPEED_MUTE			(0x17)



//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern CODEC_T* _CODEC_GetMT8292Tbl(void);

void _MT8292_WRITE(UINT8 u1Addr, UINT8 u1Data);
UINT8 _MT8292_READ(UINT8 u1Addr) ;
void _MT8292_Init(void);
void _MT8292_Mute(UINT8 u1DacId, BOOL fgMute);
void _MT8292_ChlSel(UINT8 u1Indx);
void _MT8292_SetPGAGain(UINT8 u1DacId, UINT8 u1Data);
void _MT8292_SetHpGain(UINT8 u1Data);

#endif /* CODEC_MT8291_H */


