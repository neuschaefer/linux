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
 * $RCSfile: mtadac.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/ 

/** @file mtadac.h
 *  This header file declares exported APIs of Audio DAC module.
 */

#ifndef MT_ADAC_H
#define MT_ADAC_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
#include "mtauddec.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define AUD_DAC_MAIN    (0)
#define AUD_DAC_AUX     (1)
#define AUD_DAC_NUM     (2)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    MTADAC_FORMAT_RJ,
    MTADAC_FORMAT_LJ,  
    MTADAC_FORMAT_I2S
}MTADAC_DATA_FORMAT_T;

typedef enum
{
    MTADAC_MCLK_128FS,
    MTADAC_MCLK_192FS,
    MTADAC_MCLK_256FS,
    MTADAC_MCLK_384FS,
    MTADAC_MCLK_512FS,
    MTADAC_MCLK_768FS
}MTADAC_MCLK_FREQUENCY_T;

#ifdef CC_MT5363
typedef enum
{
    MTADAC_DAC_ID_DAC0 = 0,
    MTADAC_DAC_ID_DAC1,
    MTADAC_DAC_ID_DAC2,
    MTADAC_DAC_ID_DAC3,
    MTADAC_DAC_ID_MAX
}MTADAC_DAC_ID_T;
#else
typedef enum
{
    MTADAC_DAC_ID_DAC0 = 0,
    MTADAC_DAC_ID_DAC1,
    MTADAC_DAC_ID_DAC2,
    MTADAC_DAC_ID_HP,
    MTADAC_DAC_ID_MAX
}MTADAC_DAC_ID_T;
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize ADAC module.
*@param void
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTADAC_Init(void);

/** Mute ADAC.
*@param eDacId                     Audio dac id
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTADAC_SetMute(MTADAC_DAC_ID_T eDacId, BOOL fgMute);

/** Get ADAC mute status.
*@param eDacId                     Audio dac id
*@param pfgMute                    Mute or not. (TRUE: Mute, FALSE: Un-Mute)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTADAC_GetMuteStatus(MTADAC_DAC_ID_T eDacId, BOOL * pfgMute);

/** Setup ADAC refer to which channel.
*@param eDacId                     Audio dac id.
*@param eChannel                   Channel number.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTADAC_SetDacChannel(MTADAC_DAC_ID_T eDacId, 
                                             MTAUDDEC_PCM_CH_T eChannel);

/** Digital I2S format configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eFormat                    I2S format.
*@param eMclk                      MCLK frequency
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTADAC_SetDacFormatCfg(UINT8 u1DecId, MTADAC_DATA_FORMAT_T eFormat, 
                                   MTADAC_MCLK_FREQUENCY_T eMclk);

/** Mute Speaker.
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTADAC_SetSpeakerMute(BOOL fgMute);
/**SetAMPRegister.
*@param u1Address                I2C address.
*@param u1Value                   I2C value.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTADAC_SetAMPRegister(UINT8 u1Address, UINT8 u1Value);
#ifdef __cplusplus
}
#endif

#endif //MT_ADAC_H
