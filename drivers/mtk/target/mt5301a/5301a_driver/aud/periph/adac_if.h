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
 * $RCSfile: adac_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_if.h
 * Declare audio  related interface extern function.
 *
 */
 
#ifndef ADAC_IF_H
#define ADAC_IF_H

#include "x_typedef.h"
#include "aud_if.h"

/****************************************************************************
** Type definitions
****************************************************************************/

/****************************************************************************
** Constant definitions
****************************************************************************/

/****************************************************************************
** Export API
****************************************************************************/

//-----------------------------------------------------------------------------
// ADAC_Init
//
/** Initial audio codec and digital amp.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Init(void);

//-----------------------------------------------------------------------------
// ADAC_Enable
//
/** Audio codec and amp function table init.
 *
 *  @param[in]     fgEnable   TRUE for initial.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Enable(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_Mute
//
/** Audio codec and speaker mute.
 *
 *  @param[in]     fgEnable   "TRUE" for mute,"FALSE" for unmute.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Mute(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_DacFormatCfg
//
/** Setting DAC Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DacFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);

//-----------------------------------------------------------------------------
// ADAC_AdcFormatCfg
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_AdcFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);

//-----------------------------------------------------------------------------
// ADAC_HeadphoneVolCtl
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     i2VolInHalfDb   volume value.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HeadphoneVolCtl(INT16 i2VolInHalfDb);

//-----------------------------------------------------------------------------
// ADAC_GetHeadphoneVolMaxMin
/**  Setting the max and min value of headphone volume.
*
*  @param[in]     pi2Max   volume max value.
*  @param[in]     pi2Min   volume min value.
*
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GetHeadphoneVolMaxMin(INT16* pi2Max,INT16* pi2Min);

//-----------------------------------------------------------------------------
// ADAC_DcDetectSpkEnable
//
/** Detect the DC and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DcDetectSpkEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_HpDetectSpkEnable
//
/** Detect the headphone plug in and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectSpkEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_HpDetectCodecMute
//
/** Detect the headphone plug in and mute codec.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectCodecMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_HpDetectDacMute
//
/** Detect the headphone plug in and mute internaldac.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectDacMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_HdmiUnstableSpkEnable
//
/** HDMI signal unstable and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HdmiUnstableSpkEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_ClkChangeCodecMute
//
/** Some codec have to mute before clock source change 
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_ClkChangeCodecMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_SpeakerEnable
//
/** Speaker enable routine.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_SpeakerEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_CodecMute
//
/** Codec enable routine.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_GpioCodecMute
//
/** Codec GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute codec from GPIO.
 *
 *  @retval void
 */

//-----------------------------------------------------------------------------
// ADAC_CodecHdmiUnstableMute
//
/** Codec mute when hdmi signal unstable.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecHdmiUnstableMute(BOOL fgMute);

//-----------------------------------------------------------------------------
extern void ADAC_GpioCodecMute(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_GpioAmpEnable
//
/** Speaker GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GpioAmpEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_CodecAdChlSel
//
/** Codec ad channel select.
 *
 *  @param[in]     u1Chl   Input data channel.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecAdChlSel(UINT8 u1Chl);

//-----------------------------------------------------------------------------
// ADAC_VolumeMuteSpeakerEnable
//
/** Mute speaker when volume value equal to zero.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_VolumeMuteSpeakerEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_CodecRegRead
//
/** Read register of external codec.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval 	UINT8	Data.
 */
//-----------------------------------------------------------------------------
extern UINT8 ADAC_CodecRegRead(UINT8 u1Offset);

//-----------------------------------------------------------------------------
// ADAC_CodecRegWrite
//
/** Write register of external codec.
 *
 *  @param[in]     u1Offset   	Address.
 *  @param[in]     u1Data   	Data.
 *
 *  @retval 	void.
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecRegWrite(UINT8 u1Offset, UINT8 u1Data);

//-----------------------------------------------------------------------------
// ADAC_AmpRegRead
//
/** Read register of external digital amp.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval 	UINT8	Data.
 */
//-----------------------------------------------------------------------------
extern UINT8 ADAC_AmpRegRead(UINT8 u1Offset);

//-----------------------------------------------------------------------------
// ADAC_AmpRegWrite
//
/** Write register of external digital amplify.
 *
 *  @param[in]     u1Offset   	Address.
 *  @param[in]     u1Data   	Data.
 *
 *  @retval 	void.
 */
//-----------------------------------------------------------------------------
extern void ADAC_AmpRegWrite(UINT8 u1Offset, UINT8 u1Data);

extern BOOL _IsUseMT8292(void);

#endif /* ADAC_IF_H_ */

