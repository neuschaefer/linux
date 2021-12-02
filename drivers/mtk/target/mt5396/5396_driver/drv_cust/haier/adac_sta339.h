/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: adac_apogee.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_apogee.h
 *  Brief of file adac_apogee.h. \n
 */

#ifndef ADAC_STA339_H
#define ADAC_STA339_H

//#define CC_AUD_AMP_SUPPORT_5_1_CH
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#ifdef CC_AUD_AMP_IN_I2C_BUS_2
#define AUD_AMP_BUS_ID      			2
#elif defined (CC_AUD_AMP_IN_I2C_BUS_1)
#define AUD_AMP_BUS_ID      			1
#else
#define AUD_AMP_BUS_ID      			0
#endif

///#define EN_STA_339_EAPD_TWARN

#define REG_MODE_CTL     				0x0A
#define MASTER_MODE    					0x00
#define SLAVE_MODE     					0x20

#define GPIO_PAD_ML  					42
#define GPIO_PAD_MC  					43
#define GPIO_PAD_MD  					44  

#define ST_STA339BW_ADDR                0x38    ///SA=0
#define ST_STA339BW_ADDR_SA_SET_1       0x3A    ///SA=1
#ifdef APP_STA339BW_SUBWOOFER
#define ST_STA339BW_SUBWOOFER_ADDR      0x3A    ///SA=1
#endif

#define SIF_CLK_DIV         			0x100

#define MASTER_VOL_0DB      			(0x0)
#define MASTER_VOL_NEG_38DB 			(0x4c)
#define MASTER_VOL_MUTE     			(0xff)

#define STA339W_CONFA 					0x00
#define STA339W_CONFB 					0x01
#define STA339W_CONFC 					0x02
#define STA339W_CONFD 					0x03
#define STA339W_CONFE 					0x04
#define STA339W_CONFF 					0x05
#define STA339W_MUTELOC 				0x06
#define STA339W_MVOL					0x07
#define STA339W_C1VOL 					0x08
#define STA339W_C2VOL 					0x09
#define STA339W_C3VOL 					0x0A
#define STA339W_AUTO1 					0x0B
#define STA339W_AUTO2 					0x0C
#define STA339W_AUTO3 					0x0D
#define STA339W_C1CFG					0x0E
#define STA339W_C2CFG					0x0F
#define STA339W_C3CFG					0x10
#define STA339W_TONE					0x11
#define STA339W_L1AR     				0x12
#define STA339W_L1ATRT	 				0x13
#define STA339W_L2AR					0x14
#define STA339W_L2ATRT	 				0x15
#define STA339BW_CFADDR     			0x16

#define MUTE_ALL_CHL  					0x01
#define UNMUTE_ALL_CHL 					0

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern AMP_T* _AMP_GetAmpCustFunTbl(void);

#ifdef APP_STA339BW_SUBWOOFER
extern void ST_set_subwoofer_gain(UINT8 ui1_val);
#endif
#endif /* ADAC_STA339_H*/

