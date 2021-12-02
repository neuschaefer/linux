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
 * $RCSfile: sd_audio.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sd_audio.h
 *  SDAL kernel driver implementation: Sound
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifndef	SDAL_AUDIO_H
#define SDAL_AUDIO_H

#include "x_linux.h"
#include "SdSound.h"
#include "SdAVDec.h"
#include "SdAuxOut.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct _AUD_DEC_INFOR_T_
{ 
    BOOL                  fgConnected;
    BOOL                  fgOnPlay;
    BOOL                  fgUseNormalSoundConnect;
    SdSource_k            eSourcComp;
    SoundInputPort_k      eSoundInput;
    AUD_DEC_STREAM_FROM_T eStrSrc; 
    AUD_FMT_T             eAudFmt;
    AUD_FMT_T             eAudAtvFmt;
    SdSound_Status_t      rSoundStatus;
//    SdAVDec_HDMIAudioSettings_t rHdmiSetting;
    AUD_DEC_INFO_T        rDecInfo;
    SdAVDec_PCMSettings_t rPcmSetting;
    SdSound_Settings_t *  prSoundSetting;
}   AUD_DEC_INFOR_T;

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void _SD_AUD_DtvPlay(UINT8 u1DecId);
extern void _SD_AUD_DtvStop(UINT8 u1DecId);
extern void _SD_AUD_AtvPlay(UINT8 u1DecId);
extern void _SD_AUD_AtvStop(UINT8 u1DecId);
extern void _SD_AUD_ExtInPlay(UINT8 u1DecId);
extern void _SD_AUD_ExtInStop(UINT8 u1DecId);
extern SdResult _SD_AUD_SetAudFmt(SdSoundID_k eSoundId, SdAudioFormat_k eSoundFmt, SdAVDec_Settings_t* pSettings);
extern SdResult _SD_AUD_Init(SdSound_Step_k step, SdPowerState_k ePowerState);
extern SdResult _SD_AUD_Connect(SdSoundID_k eSoundId, SdSource_k eSrc, SdBackEndInID_k eInputId);
extern SdResult _SD_AUD_MiscConnect(SdSoundID_k eSoundId, SoundInputPort_k eSoundInput, SdBackEndInID_k eInputId);
extern SdResult _SD_AUD_Disconnect(SdSoundID_k eSoundId);
extern SdResult _SD_AUD_SpeakerConnect(SdSpeakerID_k eSpeakerId, SdSoundID_k eSoundId);
extern SdResult _SD_AUD_SpeakerDisconnect(SdSpeakerID_k eSpeakerId);
extern SdResult _SD_AUD_GetInfo(SdSoundID_k eSoundId, SdSound_Settings_t *pSettings);
extern SdResult _SD_AUD_SetInfo(SdSoundID_k eSoundId, SdSound_Settings_t *pSettings);
extern SdResult _SD_AUD_GetStatus(SdSoundID_k eSoundId, SdMask_t mask, 
                                  SdSound_Status_t *pState, uint32_t timeout);
extern SdResult _SD_AUD_AmpMute(SdSpeakerID_k eSpeakerId, SdBool_t bMuteOn);
extern SdResult _SD_AUD_SpeakerMute(SdSpeakerID_k eSpeakerId, SdBool_t bMuteOn);
extern SdResult _SD_AUD_PlayBeeper(SdSoundID_k eSoundId, SdSound_MelodyMode_k eMelodyMode);
//extern SdResult _SD_AUD_HDMIAudioConnect(SdAVDecID_k eAVDecId, SdExtInID_k eExtInId);
//extern SdResult _SD_AUD_HDMIAudioDisconnect(SdAVDecID_k eAVDecId);
//extern SdResult _SD_AUD_HDMIAudioSet(SdAVDecID_k eAVDecId, SdAVDec_HDMIAudioSettings_t *pHdmiAudioSettings);
//extern SdResult _SD_AUD_HDMIAudioGet(SdAVDecID_k eAVDecId, SdAVDec_HDMIAudioSettings_t *pHdmiAudioSettings);
extern SdResult _SD_AUD_AuxOutMute(SdAuxOutID_k eAuxOutId, SdBool_t bMuteOn, SdAuxOut_Audio_k eAuxAudio);
extern BOOL _SD_AUD_GetAUIStatus(void);
extern void _SD_AUD_Debug(void);
extern SdResult _SD_AUD_GetSoundSys(SdSoundID_k eSoundId, SdSound_SoundSys_k eDefaultSys, SdSound_SoundSys_k *peSoundSys);
#endif  //SDAL_AUDIO_H

