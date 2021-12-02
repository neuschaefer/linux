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
 * $Date  $
 * $RCSfile: api_eep.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @EEPROM service function
 *
 * @author Kenny Huang
 */

#ifndef _API_EEP_H_
#define _API_EEP_H_

/**
 * read a UINT8 data from EEPROM shadow
 * If the address is valid, the return value is the data in the given address.
 * else the return value is always 0.
 *
 * @param wAddr EEPROM data address to be read
 * @return read data
 */
extern UINT8 bApiEepromReadByte(UINT16 wAddr);

/**
 * write a UINT8 data into EEPROM shadow
 * If not write shadow only, a write command will be added into command queue.
 *
 * @param wAddr EEPROM data address for writting data
 * @param bData data to be written
 * @return 0 or 1
 * @retval = 1 data is added into shadow and command is added into queue
 * @retval = 0 EEPROM write queue is full or the given address is invalid
 */
extern BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData);

/**
 * read data from EEPROM shadow area in DRAM
 *
 * @param wPrm item ID that will be retrieved
 * @param pDst destination address
 * @param wLen number of bytes
 * @return NONE
 */
extern void vApiEepromDmaRead(UINT16 wPrm, void *pDst, UINT16 wLen);

/**
 * write data into EEPROM shadow area in DRAM
 *
 * @param wPrm item ID that will be written
 * @param pDst source address
 * @param wLen number of bytes
 * @return NONE
 */
extern void vApiEepromDmaWrite(UINT16 wPrm, void *pSrc, UINT16 wLen);

/**
 * set EEPROM shadow area in DRAM to specified value
 *
 * @param wPrm item ID that will be set
 * @param pDst value to set
 * @param wLen number of bytes
 * @return NONE
 */
extern void vApiEepromSet(UINT16 wPrm, UINT8 bVal, UINT16 wLen);



/**
 * Check EEPROM of NPTV valid or not
 *
 * @return TRUE or FALSE
 */
extern BOOL fgApiIsEepromMagicOK(void);
extern void  vApiEepromDefaultInit(void) ;


extern UINT8 code _pbEepromMagic[4];

extern UINT16 code wEEP_VIDEO_AUTO_COLOR_START;
//extern UINT16 code wEEP_VIDEO_AUTO_COLOR_VGA_DONE;
//extern UINT16 code wEEP_VIDEO_AUTO_COLOR_HDTV_DONE;
//extern UINT16 code wEEP_VIDEO_AUTO_COLOR_DIFF_1;
extern UINT16 code wEEP_ON_CHIP_AUTO_COLOR_GAIN_DONE;
extern UINT16 code wEEP_ON_CHIP_AUTO_COLOR_OFFSET_ADDRESS_START;
#if 0
extern UINT16 code wEEP_EXT_AUTO_COLOR_START;
extern UINT16 code wEEP_VGA_CLOCK_START;
extern UINT16 code wEEP_VGA_PHASE_START;
extern UINT16 code wEEP_VGA_H_POSITION_START;
extern UINT16 code wEEP_VGA_V_POSITION_START;
extern UINT16 code wEEP_BYTE_TV_REGION;
extern UINT16 code wEEP_CUSTOM_NAMED_CH_NUM;
extern UINT16 code wEEP_CUSTOM_NAMED_CH;
extern UINT16 code wEEP_CUSTOM_CH_NAME;
extern UINT16 code wEEP_ADC_OFFSET_TV_CLAMP_DONE;
extern UINT16 code wEEP_ADC_INPUT_OFFSET_TV_CH0_CLAMP;
extern UINT16 code wEEP_ADC_OUTPUT_OFFSET_TV_CH0_CLAMP;
extern UINT16 code wEEP_ADC_INPUT_OFFSET_TV_CH1_CLAMP;
extern UINT16 code wEEP_ADC_OUTPUT_OFFSET_TV_CH1_CLAMP;
extern UINT16 code wEEP_ADC_DFE_LEVEL_TV_DCLAMP;
extern UINT16 code wEEP_ADC_OFFSET_CLAMP_BUT_LOW;
extern UINT16 code wEEP_ADC_OFFSET_HDTV_CLAMP;
#endif



#endif //_API_EEP_H_
