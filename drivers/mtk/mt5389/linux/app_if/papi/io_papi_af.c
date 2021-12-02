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
 * $RCSfile: io_papi_af.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_af.c
 *  PAPI kernel driver implementation: Audio Feature
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "aud_if.h"
#include "aud_drvif.h"
#include "aud_drv.h"
#include "aud_dsp_cfg.h"
#include "papi.h"
#include "papi_types.h"
//#include "papi_af_types.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// Common error code
#define TM_FAIL                     -0x800
#define PAPI_OK 0
#define PAPI_FAIL -1
#define SPEAKER_DAC			2
#define HEADPHONE_DAC		1

// For Outport Masking
#define PAPI_AF_NONE 0
#define PAPI_AF_SPEAKER (1-1)
#define PAPI_AF_HEADPHONE (2-1)
#define PAPI_AF_MONOUT (3-1)
#define PAPI_AF_SPDIF (4-1)

#define PAPI_AF_MASK_SPEAKER (1<<PAPI_AF_SPEAKER)
#define PAPI_AF_MASK_HEADPHONE (1<<PAPI_AF_HEADPHONE)
#define PAPI_AF_MASK_MONOUT (1<<PAPI_AF_MONOUT)
#define PAPI_AF_MASK_SPDIF (1<<PAPI_AF_SPDIF)

// For Logical Channel Selection.
#define PAPI_AF_SSOC_CHANNEL1 1
#define PAPI_AF_SSOC_CHANNEL2 2
#define PAPI_AF_SSOC_CHANNEL3 3

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return TM_FAIL; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return TM_FAIL; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return TM_FAIL; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return TM_FAIL; \
    }
        
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static INT32 _i4SsooVolume = 128; // master volume of speaker
static UINT32 _u4SsooBalance = 0; // Balance of speaker
static UINT32 _u4SsooTreble = 128;//Treble
static UINT32 _u4SsooBass = 128;//Bass
static INT32 _i4DelVol = 0;
static INT32 _i4HpDelVol = 0;
static INT32 _i4HsooVolume = 128;//headphone volume
static Bool _fgSsooMute = TRUE;
static Bool _fgHsooMute = TRUE;
static AUD_DEC_STREAM_FROM_T _aeStrSrc[2] = {AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER};
static UINT32 _u4MuteOutputs = 0;
static UINT32 _u4AutoMuteOutputs = 0;
static UINT32 _u4AutoMuteEnabled = papi_af_smt_Headphone | papi_af_smt_Speaker;
static UINT32 _u4AutoSurround = papi_af_som_useNone;
static UINT32 _u4SurroundMode = papi_af_som_Off;
static UINT32 _u4SurroundLevel = papi_af_som_LevelOff;
static UINT32 _fgAutoVolumeLevel = TRUE;
static UINT32 _fgNightMode = TRUE;
static UINT32 _fgImprovementMode = papi_af_sip_ImprovementNone;
static UINT32 _fgLoudness = FALSE;
static UINT32 _fgDRC = papi_af_sla_DrcModeUnknown;
#ifdef CC_AUD_7BAND_EQUALIZER
static INT32   _i4EqVal[CH_EQ_BAND_NO+1] = {0, 28, 28, 28, 28, 28, 28, 28};
#else
static INT32   _i4EqVal[CH_EQ_BAND_NO+1] = {0, 28, 28, 28, 28, 28};
#endif

static UINT32 _u4SurroundModeStrength[papi_af_som_NumOfMode] = {
papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, 
papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect,
papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect,
papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect
};
static UINT32 _u4SurroundLevelStrength[papi_af_som_NumOfLevel] = {
papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect, papi_af_som_NoEffect
};

static UINT32 _u4ADOutputs = 0;
static INT32 _i4ADVolume = 0;
static UINT8 _u1SetChannel = 0;

static HANDLE_T _hIoAfMutex = NULL;
static HANDLE_T _hIoAfNfySema = NULL;
static UINT32 u4AudNfyHandler = NULL;


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

void papi_af_sound_properties_nfy(void);
void papi_af_drc_allowed_changed_nfy(void);
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
void _af_nfy (void *          pvNfyTag,
                              UINT8           u1DecId,
                              AUD_COND_NFY_T  eNfyCond,
                              UINT32          u4Data1,
                              UINT32          u4Data2)
{
    switch(eNfyCond)
    {
        case AUD_COND_CTRL_DONE:
            printk("AUD_COND_CTRL_DONE\n");
            x_sema_unlock(_hIoAfNfySema);
            break;
        case AUD_COND_AUD_INFO_CHG:
            printk("AUD_COND_AUD_INFO_CHG\n");
            x_sema_unlock(_hIoAfNfySema);
            break;
        case AUD_COND_FLUSH_DONE:
        default:
            break;
    }
}

static BOOL _IsOutMaskSetToSpeaker(UINT32 u4Output)
{
    if ((u4Output & PAPI_AF_MASK_SPEAKER) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static BOOL _IsOutMaskSetToHeadPhone(UINT32 u4Output)
{
    if ((u4Output & PAPI_AF_MASK_HEADPHONE) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsOutMaskSetToMonOut(UINT32 u4Output)
{
    if ((u4Output & PAPI_AF_MASK_MONOUT) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsOutMaskSetToSPDIF(UINT32 u4Output)
{
    if ((u4Output & PAPI_AF_MASK_SPDIF) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


static INT32 _papi_af_ssoo_SetVolume(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    INT32 i4Volume;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4Volume = (INT32)rArg.au4Arg[0];    
    u1DecId = AUD_DEC_MAIN;        

    i4Volume = (i4Volume < papi_af_ssoo_MinVolume) ? papi_af_ssoo_MinVolume:i4Volume;
    i4Volume = (i4Volume > papi_af_ssoo_MaxVolume) ? papi_af_ssoo_MaxVolume:i4Volume;
           
    _i4SsooVolume = i4Volume;
        
    //_AUD_DspMasterVolume(u1DecId, u4Volume * 100 / papi_af_ssoo_MaxVolume);
    _AUD_DspChannelVolume(u1DecId, AUD_CH_FRONT_LEFT, i4Volume * 100 / papi_af_ssoo_MaxVolume);
    _AUD_DspChannelVolume(u1DecId, AUD_CH_FRONT_RIGHT, i4Volume * 100 / papi_af_ssoo_MaxVolume);

    // send callback
    rNotify.i4Type = 26; 
   _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
   
    return TM_OK;
}

static INT32 _papi_af_ssoo_GetVolume(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32* pi4Vol;
    INT32 i4Vol;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pi4Vol = (INT32*)rArg.au4Arg[0];

    // Copy data to user memory
    i4Vol = _i4SsooVolume;
    COPY_TO_USER(pi4Vol, &i4Vol, sizeof (INT32));

    return TM_OK;
}


static INT32 _papi_af_ssoo_SetBalance(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Bal;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Bal = (UINT32)rArg.au4Arg[0];  //-255~255  
    u1DecId = AUD_DEC_MAIN;        
    _u4SsooBalance = u4Bal;
        
    _AUD_DspBalance(u1DecId, (u4Bal+(papi_af_hsoo_MaxBalance)) * 100 / (papi_af_hsoo_MaxBalance-papi_af_hsoo_MinBalance));
    
    return TM_OK;
}

static INT32 _papi_af_ssoo_GetBalance(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Bal;
    UINT32 u4Bal;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Bal = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    u4Bal = _u4SsooBalance;
    COPY_TO_USER(pu4Bal, &u4Bal, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_ssoo_SetBass(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Bass;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Bass = (UINT32)rArg.au4Arg[0];  //-255~255  
    u1DecId = AUD_DEC_MAIN;        

    if(u4Bass == 128){//turn off
        _AUD_DspSuperBassEnable(u1DecId, 0);      
    }else if( _u4SsooBass ==128 && u4Bass!= 128){//turn on
        _AUD_DspBassBoostGain(u1DecId, u4Bass * 28 / papi_af_hsoo_MaxBass);   
        _AUD_DspSuperBassEnable(u1DecId, 1);
    }else{
        _AUD_DspBassBoostGain(u1DecId, u4Bass * 28 / papi_af_hsoo_MaxBass);
    }
    
    _u4SsooBass = u4Bass;        
    
    // send callback    
     rNotify.i4Type = 22; 
   _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
    return TM_OK;
}


static INT32 _papi_af_ssoo_GetBass(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Bass;
    UINT32 u4Bass;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Bass = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    u4Bass = _u4SsooBass;
    COPY_TO_USER(pu4Bass, &u4Bass, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_ssoo_SetTreble(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Tre;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Tre = (UINT32)rArg.au4Arg[0];  //0~255  
    u1DecId = AUD_DEC_MAIN;        

    if(u4Tre == 128){//turn off
        _AUD_DspClearBoostEnable(u1DecId, 0);      
    }else if( _u4SsooTreble ==128 && u4Tre!= 128){//turn on
        _AUD_DspClearBoostGain(u1DecId, u4Tre * 28 / papi_af_hsoo_MaxTreble);
        _AUD_DspClearBoostEnable(u1DecId, 1);    
    }else{
        _AUD_DspClearBoostGain(u1DecId, u4Tre * 28 / papi_af_hsoo_MaxTreble);
    }
    _u4SsooTreble = u4Tre;
    
    // send callback    
     rNotify.i4Type = 25; 
   _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
    return TM_OK;
}


static INT32 _papi_af_ssoo_GetTreble(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Tre;
    UINT32 u4Tre;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Tre = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    u4Tre = _u4SsooTreble;
    COPY_TO_USER(pu4Tre, &u4Tre, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_ssoo_SetMute(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    Bool fgMute;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    fgMute = (Bool)rArg.au4Arg[0];    
    u1DecId = AUD_DEC_MAIN;        
    _fgSsooMute = fgMute;
        
    if (fgMute)
    {
        //AUD_Mute(u1DecId);
       ADAC_SpeakerEnable(FALSE);
        _MT8297_Mute(SPEAKER_DAC,1);  //speaker DAC 0
    }
    else
    {
       //AUD_UnMute(u1DecId);        
        ADAC_SpeakerEnable(TRUE);
         _MT8297_Mute(SPEAKER_DAC,0);  //speaker DAC 0
    }
    
    // send callback
    rNotify.i4Type = 24; 
   _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);    
    return TM_OK;
}

static INT32 _papi_af_ssoo_GetMute(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    Bool* pfgMute;
    Bool fgMute;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgMute = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    fgMute = _fgSsooMute;
    COPY_TO_USER(pfgMute, &fgMute, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_ssoo_Mute_Enable(UINT32 u4Arg)
{
    Bool fgSsooMuteEnabled;

    // Copy arguments from user space
    COPY_FROM_USER(&fgSsooMuteEnabled, u4Arg, sizeof(Bool));


    if(fgSsooMuteEnabled == FALSE)
    {
        _u4AutoMuteEnabled &= ~papi_af_smt_Speaker;
    }
    else
    {
        _u4AutoMuteEnabled |=  papi_af_smt_Speaker;
    }
    return TM_OK;
}

//FIXME!! Should NOT hard code here !!!!
static void _MT8292_WRITE_TMP(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 
    
    u4WriteCount = SIF_Write(0x100, 0x70, u1Addr, &u1Data, 1);

    if (u4WriteCount < 0)   
    {
    	printk("Error MT8292 register setting\n");
    }	
}
UINT8 _MT8292_READ_TMP(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;

    u4ReadCount = SIF_Read(0x100, 0x70, u1Addr, &u1Data, 1);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        printk("MT8292 READ FAIL\n");
    	return FALSE;
    }	
}

//forward declaration
static INT32 _papi_af_smt_Mute_Output_int(UINT32 u4Arg);
static INT32 _papi_af_smt_Mute_Output(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;    
    UINT32 u4MuteOutputs;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4MuteOutputs = (UINT32)rArg.au4Arg[0];
    return _papi_af_smt_Mute_Output_int(u4MuteOutputs);
}

static INT32 _papi_af_smt_Mute_Output_int(UINT32 u4Arg)
{
    UINT32 u4MuteOutputs;
    UINT8  u1Data = 0;
    PAPI_AF_CB_T rNotify;
   
    u4MuteOutputs = u4Arg;
    _u4MuteOutputs = u4MuteOutputs;
    u4MuteOutputs |= _u4AutoMuteOutputs;

    //FIXME!! control MT8292 directly.        
    if (u4MuteOutputs & papi_af_smt_Headphone)
    {
        _fgHsooMute = 1;
        _MT8297_Mute(HEADPHONE_DAC,1);  //headphone DAC 0
        // send callback
        rNotify.i4Type = 2; 
        _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);    
    }
    else
    {
        _fgHsooMute = 0;
        //u1Data &= 0xFC;
        _MT8297_Mute(HEADPHONE_DAC,0);  //headphone DAC 0
    }

    if (u4MuteOutputs & papi_af_smt_Speaker)
    {
        _fgSsooMute = 1;
       ADAC_SpeakerEnable(FALSE);        
        _MT8297_Mute(SPEAKER_DAC,1);  //speaker DAC 0
        // send callback
       rNotify.i4Type = 24; 
       _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);    

    }
    else
    {
        _fgSsooMute = 0;
        //u1Data &= 0xF3;
       ADAC_SpeakerEnable(TRUE);        
        _MT8297_Mute(SPEAKER_DAC,0);  //speaker DAC 0
    }

    if (u4MuteOutputs & papi_af_smt_Monitor)    //todo
    {        
    }
    else
    {        
    }

    if (u4MuteOutputs & papi_af_smt_Spdif)
    {
        AUD_DspIECConfig( AUD_IEC_CFG_PCM, FALSE);
    }
    else
    {
        AUD_DspIECConfig( AUD_IEC_CFG_PCM, TRUE);
    }

    if (u4MuteOutputs & papi_af_smt_Line1)  //todo
    {
        
    }
    else
    {
        
    }

    if (u4MuteOutputs & papi_af_smt_Line2)  //todo
    {
        
    }
    else
    {
        
    }

    //_MT8292_WRITE_TMP(0xF, u1Data);
    return TM_OK;    
}

INT32 papi_af_smt_Auto_Mute_Output(UINT32 u4Arg)
{
    if(u4Arg != 0)
    {
        /* for speaker & headphone, the mute status is decided by EnableMute */
        _u4AutoMuteOutputs = _u4AutoMuteEnabled;

        /* for others, defaultly enable the auto mute functionality */
        _u4AutoMuteOutputs |= (papi_af_smt_Monitor|papi_af_smt_Spdif|papi_af_smt_Line1|papi_af_smt_Line2);
    }
    else
    {
        /* clear all auto mute status */
        _u4AutoMuteOutputs = 0;
    }
    return _papi_af_smt_Mute_Output_int(_u4MuteOutputs);
}

static INT32 _papi_af_smt_GetMuteStatus(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4MuteStatus;
    UINT32  u4MuteStatus;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4MuteStatus = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    u4MuteStatus = _u4MuteOutputs;
    u4MuteStatus |= _u4AutoMuteOutputs;
    COPY_TO_USER(pu4MuteStatus, &u4MuteStatus, sizeof (UINT32));

    return TM_OK;    
}

static INT32 _papi_af_hsoo_SetVolume(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    INT32 i4Volume;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4Volume = (UINT32)rArg.au4Arg[0];    
    u1DecId = AUD_DEC_MAIN;        

    i4Volume = (i4Volume < papi_af_hsoo_MinVolume) ? papi_af_hsoo_MinVolume:i4Volume;
    i4Volume = (i4Volume > papi_af_hsoo_MaxVolume) ? papi_af_hsoo_MaxVolume:i4Volume;
    
    _i4HsooVolume = i4Volume;
        
    //_AUD_DspMasterVolume(u1DecId, );
    // BYPASS L R Channel    
    _AUD_DspChannelVolume(u1DecId, AUD_CH_DMX_LEFT, i4Volume * 100 / papi_af_hsoo_MaxVolume);
    _AUD_DspChannelVolume(u1DecId, AUD_CH_DMX_RIGHT, i4Volume * 100 / papi_af_hsoo_MaxVolume);
    //_AUD_DspChannelVolume(u1DecId, AUD_CH_BYPASS_LEFT, u4Volume * 100 / 255);
    //_AUD_DspChannelVolume(u1DecId, AUD_CH_BYPASS_RIGHT, u4Volume * 100 / 255);                               

    // send callback
    rNotify.i4Type = 4; 
   _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);    
    return TM_OK;
}

static INT32 _papi_af_hsoo_GetVolume(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32* pi4Vol;
    INT32 i4Vol;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pi4Vol = (INT32*)rArg.au4Arg[0];

    // Copy data to user memory
    i4Vol = _i4HsooVolume;
    COPY_TO_USER(pi4Vol, &i4Vol, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_hsoo_SetMute(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    Bool fgMute;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    fgMute = (Bool)rArg.au4Arg[0];    
    u1DecId = AUD_DEC_MAIN;        
    _fgHsooMute = fgMute;
        
    if (fgMute)
    {
        //AUD_Mute(u1DecId);
        //ADAC_SpeakerEnable(FALSE);
       _MT8297_Mute(HEADPHONE_DAC,1);  //headphone DAC 0
        /* synchroized with smt */
        _u4MuteOutputs |= papi_af_smt_Headphone;
    }
    else
    {
        //AUD_UnMute(u1DecId);        
        //ADAC_SpeakerEnable(TRUE);
         _MT8297_Mute(HEADPHONE_DAC,0);  //headphone DAC 0
        /* synchroized with smt */
        _u4MuteOutputs &= ~papi_af_smt_Headphone;
    }
    
    // send callback
     rNotify.i4Type = 2; 
   _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);    
    
    return TM_OK;
}

static INT32 _papi_af_hsoo_GetMute(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    Bool* pfgMute;
    Bool fgMute;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgMute = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    fgMute = _fgHsooMute;
    COPY_TO_USER(pfgMute, &fgMute, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_hsoo_Mute_Enable(UINT32 u4Arg)
{
    Bool fgHsooMuteEnabled;

    // Copy arguments from user space
    COPY_FROM_USER(&fgHsooMuteEnabled, u4Arg, sizeof(Bool));

    if(fgHsooMuteEnabled == FALSE)
    {
        _u4AutoMuteEnabled &= ~papi_af_smt_Headphone;
    }
    else
    {
        _u4AutoMuteEnabled |=  papi_af_smt_Headphone;
    }
    return TM_OK;
}

static INT32 _papi_af_som_GetAutoSurround(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Mode;
    UINT32 u4Mode;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Mode = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    u4Mode = _u4AutoSurround;
    COPY_TO_USER(pu4Mode, &u4Mode, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_som_SetAutoSurround(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Mode;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Mode = (UINT32)rArg.au4Arg[0];    
    u1DecId = AUD_DEC_MAIN;        

    switch (u4Mode){
        case  papi_af_som_useNone:
            _u4AutoSurround = papi_af_som_useNone;
            _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, 0);
            break;              
        case papi_af_som_useMonoStereo:
            _u4AutoSurround = u4Mode;
            _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, 1);
                    // send callback
            rNotify.i4Type = 15;  //papi_af_somN_AutoSurroundModeActiveNotify
            _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
            break;
        case papi_af_som_useWssDpl:
        case papi_af_som_useP50Dpl: 
        case papi_af_som_useSPDIFDpl: 
        case papi_af_som_useSpeakerConfiguration:
            break;
        default:
            break;
    }    
    return TM_OK;
}

static INT32 _papi_af_som_GetSurroundMode(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Mode;
    UINT32 u4Level, u4Mode;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Mode = (UINT32*)rArg.au4Arg[0];
    u4Level = (UINT32)rArg.au4Arg[1];

    switch (u4Level){
        case papi_af_som_CurrentLevel:
            u4Mode = papi_af_som_CurrentMode;
            break;
        case papi_af_som_LevelOff:
            u4Mode = papi_af_som_Off;
            break;
        case papi_af_som_LevelVirtual:
            u4Mode = papi_af_som_IncredibleSurround;
            break;
        case papi_af_som_LevelEnhanced:
        case papi_af_som_LevelFront3:
        case papi_af_som_LevelHall:
        case papi_af_som_LevelMaximum:
            u4Mode = papi_af_som_Off;
            break;
        default:
            u4Mode = papi_af_som_Off;
    }
    // Copy data to user memory
    COPY_TO_USER(pu4Mode, &u4Mode, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_som_SetSurroundMode(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Mode;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Mode = (UINT32)rArg.au4Arg[0];    
    u1DecId = AUD_DEC_MAIN;        

    switch (u4Mode){
        case  papi_af_som_Off :
            if(_u4SurroundMode!=papi_af_som_Off){
                _u4SurroundMode = papi_af_som_Off;
                _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, 0);
                rNotify.i4Type = 19;  //papi_af_somN_OnSurroundModeChanged
                _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
            }            
            break;
        case papi_af_som_CurrentMode :            
            break;
        case papi_af_som_IncredibleSurround :
            if(_u4SurroundMode!=papi_af_som_IncredibleSurround){
                _u4SurroundMode = papi_af_som_IncredibleSurround;
                _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, 1);
                rNotify.i4Type = 19;  //papi_af_somN_OnSurroundModeChanged
                _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
            }
            break;
        case papi_af_som_Spatial :
            if(_u4SurroundMode!=papi_af_som_Spatial){
                _u4SurroundMode = papi_af_som_Spatial;
                _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, 1);
                rNotify.i4Type = 19;  //papi_af_somN_OnSurroundModeChanged
                _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
            }
            break;
        case  papi_af_som_Hall :
        case  papi_af_som_SrsWow :
        case  papi_af_som_Srs3DSound :
        case  papi_af_som_Incredible3DSurround :
        case  papi_af_som_Front3Stereo :
        case  papi_af_som_Stereo4 :
        case  papi_af_som_DolbyProLogic :
        case  papi_af_som_IncredibleMono :
        case  papi_af_som_IncredibleStereo :
        case  papi_af_som_VirtualDolby422 :
        case  papi_af_som_VirtualDolby423 :
        case  papi_af_som_VirtualDolby521 :
        case  papi_af_som_VirtualDolby522 :
        case  papi_af_som_SrsTruSurround :
        case  papi_af_som_SrsTruSurroundWithDPLII :
        case papi_af_som_SrsTruSurroundXT:
        case  papi_af_som_SrsTruSurroundXTWithDPLII :
        case  papi_af_som_Matrix :
        case  papi_af_som_DolbyDigital :
        case  papi_af_som_MPEGMultiChannel :
        case  papi_af_som_DolbyProLogicII :
        case  papi_af_som_VirtualDolbyDigital :
        case  papi_af_som_Digital3D :
        case  papi_af_som_DolbyProLogicPhantom :
            break;
        default:
            break;
    }    
    return TM_OK;
}


static INT32 _papi_af_som_GetSurroundLevel(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Level;
    UINT32 u4Level, u4Mode;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Level = (UINT32*)rArg.au4Arg[0];
    u4Mode = (UINT32)rArg.au4Arg[1];

    switch (u4Mode){
        case  papi_af_som_Off :
            u4Level = papi_af_som_LevelOff;           
            break;
        case papi_af_som_CurrentMode :
            u4Level = papi_af_som_CurrentLevel;
            break;

        case  papi_af_som_IncredibleSurround :
        case papi_af_som_Spatial :
            u4Level = papi_af_som_LevelVirtual;
            break;
        case  papi_af_som_Hall :
        case  papi_af_som_SrsWow :
        case  papi_af_som_Srs3DSound :
        case  papi_af_som_Incredible3DSurround :
        case  papi_af_som_Front3Stereo :
        case  papi_af_som_Stereo4 :
        case  papi_af_som_DolbyProLogic :
        case  papi_af_som_IncredibleMono :
        case  papi_af_som_IncredibleStereo :
        case  papi_af_som_VirtualDolby422 :
        case  papi_af_som_VirtualDolby423 :
        case  papi_af_som_VirtualDolby521 :
        case  papi_af_som_VirtualDolby522 :
        case  papi_af_som_SrsTruSurround :
        case  papi_af_som_SrsTruSurroundWithDPLII :
        case papi_af_som_SrsTruSurroundXT:
        case  papi_af_som_SrsTruSurroundXTWithDPLII :
        case  papi_af_som_Matrix :
        case  papi_af_som_DolbyDigital :
        case  papi_af_som_MPEGMultiChannel :
        case  papi_af_som_DolbyProLogicII :
        case  papi_af_som_VirtualDolbyDigital :
        case  papi_af_som_Digital3D :
        case  papi_af_som_DolbyProLogicPhantom :
            u4Level = papi_af_som_LevelOff;
            break;
        default:
            break;
    }   
    // Copy data to user memory
    COPY_TO_USER(pu4Level, &u4Level, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_som_SetSurroundLevel(UINT32 u4Arg)
{
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Level;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Level = (UINT32)rArg.au4Arg[0];    
    u1DecId = AUD_DEC_MAIN;        
    switch (u4Level){
        case papi_af_som_CurrentLevel:
            break;
        case papi_af_som_LevelOff:
             if(_u4SurroundLevel!=papi_af_som_LevelOff){
                _u4SurroundMode = papi_af_som_Off;
                _u4SurroundLevel = papi_af_som_LevelOff;
                _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, 0);
                rNotify.i4Type = 19;  //papi_af_somN_OnSurroundModeChanged
                _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
                rNotify.i4Type = 17;  //papi_af_somN_OnSurroundLevelChanged
                _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
            }            
            break;
        case papi_af_som_LevelVirtual:
             if(_u4SurroundLevel!=papi_af_som_LevelVirtual){
                _u4SurroundMode = papi_af_som_IncredibleSurround;
                _u4SurroundLevel = papi_af_som_LevelVirtual;
                _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, 1);
                rNotify.i4Type = 19;  //papi_af_somN_OnSurroundModeChanged
                _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
                rNotify.i4Type = 17;  //papi_af_somN_OnSurroundLevelChanged
                _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
            }
            break;
        case papi_af_som_LevelEnhanced:
        case papi_af_som_LevelFront3:
        case papi_af_som_LevelHall:
        case papi_af_som_LevelMaximum:
            break;
        default:
            break;
    }     
    return TM_OK;
}

static INT32 _papi_af_som_GetSurroundLevelEffectStrength(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Strength;
    UINT32 u4Level, u4Strength;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Strength = (UINT32*)rArg.au4Arg[0];
    u4Level = (UINT32)rArg.au4Arg[1];

     switch (u4Level){
        case papi_af_som_CurrentLevel:
            u4Strength = _u4SurroundLevelStrength[_u4SurroundLevel];             
            break;
        case papi_af_som_LevelVirtual:
            u4Strength = _u4SurroundLevelStrength[u4Level];             
            break;
        case papi_af_som_LevelOff:    
        case papi_af_som_LevelEnhanced:
        case papi_af_som_LevelFront3:
        case papi_af_som_LevelHall:
        case papi_af_som_LevelMaximum:
            u4Strength = papi_af_som_NoEffect;
            break;
        default:
            u4Strength = papi_af_som_NoEffect;
            break;
    }     
    // Copy data to user memory
    COPY_TO_USER(pu4Strength, &u4Strength, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_som_SetSurroundLevelEffectStrength(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;    
    UINT32 u4Level, u4Strength;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Strength = (UINT32)rArg.au4Arg[0];    
    u4Level = (UINT32)rArg.au4Arg[1];    
    switch (u4Level){
        case papi_af_som_CurrentLevel:
            if (_u4SurroundLevel==papi_af_som_LevelVirtual){
                goto LEVELVIRTUAL;
            }
            break;
        case papi_af_som_LevelVirtual:
LEVELVIRTUAL:
             if(_u4SurroundLevelStrength[papi_af_som_LevelVirtual] != u4Strength){
                _u4SurroundLevelStrength[papi_af_som_LevelVirtual] = u4Strength;
                switch(u4Strength){ //  Width (1)    0 ~ 0x500000 
                    case papi_af_som_NoEffect :
                        _AUD_Surround_Set_Config(1, 0x0 ); 
                        break;
                    case papi_af_som_MinEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x1AAAAA ); 
                        break;                    
                    case papi_af_som_MedEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x355555 ); 
                        break;                    
                    case papi_af_som_MaxEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x500000 ); 
                        break;
                }
            }
            break;
        case papi_af_som_LevelOff:             
        case papi_af_som_LevelEnhanced:
        case papi_af_som_LevelFront3:
        case papi_af_som_LevelHall:
        case papi_af_som_LevelMaximum:
            _u4SurroundLevelStrength[u4Level] = papi_af_som_NoEffect;
            break;
        default:
            break;
    }     
    return TM_OK;
}

static INT32 _papi_af_som_SetSurroundModeEffectStrength(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;    
    UINT32 u4Mode, u4Strength;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Strength = (UINT32)rArg.au4Arg[0];    
    u4Mode = (UINT32)rArg.au4Arg[1];    

    switch (u4Mode){
        case papi_af_som_CurrentMode :        
            if (_u4SurroundMode==papi_af_som_IncredibleSurround){
                goto INCREDIBLESURR;
            }else if(_u4SurroundMode==papi_af_som_Spatial){
                goto SPATIALSURR;
            }            
            break;

        case papi_af_som_IncredibleSurround :
INCREDIBLESURR :
            if(_u4SurroundModeStrength[papi_af_som_IncredibleSurround] != u4Strength){
                _u4SurroundModeStrength[papi_af_som_IncredibleSurround] = u4Strength;
                switch(u4Strength){ //  Width (1)    0 ~ 0x500000 
                    case papi_af_som_NoEffect :
                        _AUD_Surround_Set_Config(1, 0x0 ); 
                        break;
                    case papi_af_som_MinEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x1AAAAA ); 
                        break;                    
                    case papi_af_som_MedEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x355555 ); 
                        break;                    
                    case papi_af_som_MaxEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x500000 ); 
                        break;
                }
            }
                break;
        case papi_af_som_Spatial :
SPATIALSURR :
            if(_u4SurroundModeStrength[papi_af_som_Spatial] != u4Strength){
                _u4SurroundModeStrength[papi_af_som_Spatial] = u4Strength;
                switch(u4Strength){ //  Width (1)    0 ~ 0x500000 
                    case papi_af_som_NoEffect :
                        _AUD_Surround_Set_Config(1, 0x0 ); 
                        break;
                    case papi_af_som_MinEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x1AAAAA ); 
                        break;                    
                    case papi_af_som_MedEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x355555 ); 
                        break;                    
                    case papi_af_som_MaxEffectStrength :
                        _AUD_Surround_Set_Config(1, 0x500000 ); 
                        break;
                }
            }
            break;
        case  papi_af_som_Off :
        case  papi_af_som_Hall :
        case  papi_af_som_SrsWow :
        case  papi_af_som_Srs3DSound :
        case  papi_af_som_Incredible3DSurround :
        case  papi_af_som_Front3Stereo :
        case  papi_af_som_Stereo4 :
        case  papi_af_som_DolbyProLogic :
        case  papi_af_som_IncredibleMono :
        case  papi_af_som_IncredibleStereo :
        case  papi_af_som_VirtualDolby422 :
        case  papi_af_som_VirtualDolby423 :
        case  papi_af_som_VirtualDolby521 :
        case  papi_af_som_VirtualDolby522 :
        case  papi_af_som_SrsTruSurround :
        case  papi_af_som_SrsTruSurroundWithDPLII :
        case papi_af_som_SrsTruSurroundXT:
        case  papi_af_som_SrsTruSurroundXTWithDPLII :
        case  papi_af_som_Matrix :
        case  papi_af_som_DolbyDigital :
        case  papi_af_som_MPEGMultiChannel :
        case  papi_af_som_DolbyProLogicII :
        case  papi_af_som_VirtualDolbyDigital :
        case  papi_af_som_Digital3D :
        case  papi_af_som_DolbyProLogicPhantom :
            break;
        default:
            break;
    }    
    return TM_OK;
}


static INT32 _papi_af_som_GetSurroundModeEffectStrength(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4Strength;
    UINT32 u4Strength, u4Mode;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Strength = (UINT32*)rArg.au4Arg[0];
    u4Mode = (UINT32)rArg.au4Arg[1];

    switch (u4Mode){
        case papi_af_som_CurrentMode :
            u4Strength = _u4SurroundModeStrength[_u4SurroundMode];
            break;
        case  papi_af_som_IncredibleSurround :
        case papi_af_som_Spatial :
            u4Strength = _u4SurroundModeStrength[u4Mode];             
            break;
        case  papi_af_som_Off :    
        case  papi_af_som_Hall :
        case  papi_af_som_SrsWow :
        case  papi_af_som_Srs3DSound :
        case  papi_af_som_Incredible3DSurround :
        case  papi_af_som_Front3Stereo :
        case  papi_af_som_Stereo4 :
        case  papi_af_som_DolbyProLogic :
        case  papi_af_som_IncredibleMono :
        case  papi_af_som_IncredibleStereo :
        case  papi_af_som_VirtualDolby422 :
        case  papi_af_som_VirtualDolby423 :
        case  papi_af_som_VirtualDolby521 :
        case  papi_af_som_VirtualDolby522 :
        case  papi_af_som_SrsTruSurround :
        case  papi_af_som_SrsTruSurroundWithDPLII :
        case papi_af_som_SrsTruSurroundXT:
        case  papi_af_som_SrsTruSurroundXTWithDPLII :
        case  papi_af_som_Matrix :
        case  papi_af_som_DolbyDigital :
        case  papi_af_som_MPEGMultiChannel :
        case  papi_af_som_DolbyProLogicII :
        case  papi_af_som_VirtualDolbyDigital :
        case  papi_af_som_Digital3D :
        case  papi_af_som_DolbyProLogicPhantom :
            u4Strength = papi_af_som_NoEffect;
            break;
        default:
            u4Strength = papi_af_som_NoEffect;
            break;
    }   
    // Copy data to user memory
    COPY_TO_USER(pu4Strength, &u4Strength, sizeof (UINT32));

    return TM_OK;
}
   

static INT32 _papi_af_sla_GetAutoVolumeLevel(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    Bool* pfgAvl;
    Bool fgAvl;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgAvl = (Bool*)rArg.au4Arg[0];
    fgAvl = _fgAutoVolumeLevel;     
    COPY_TO_USER(pfgAvl, &fgAvl, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_sla_SetAutoVolumeLevel(UINT32 u4Arg){
    IOCTL_2ARG_T rArg;    
    Bool fgAvl;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    fgAvl = (Bool)rArg.au4Arg[0];    
    if (_fgAutoVolumeLevel != fgAvl){
        _fgAutoVolumeLevel = fgAvl;
        _AUD_DspChangeAVCPara(0, _fgAutoVolumeLevel, 0);//type, value, decID
        rNotify.i4Type = 10;  // papi_af_slaN_OnAutoVolumeLevelChanged
        _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
    }    
     return TM_OK;
}

static INT32 _papi_af_sla_GetNightMode(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    Bool* pfgAvl;
    Bool fgAvl;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgAvl = (Bool*)rArg.au4Arg[0];
    fgAvl = _fgNightMode;     
    COPY_TO_USER(pfgAvl, &fgAvl, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_sla_SetNightMode(UINT32 u4Arg){
    IOCTL_2ARG_T rArg;    
    Bool fgAvl;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    fgAvl = (Bool)rArg.au4Arg[0];    
    if (_fgNightMode != fgAvl){
        _fgNightMode = fgAvl;
        _AUD_DspChangeAVCPara(0, _fgNightMode, 0);//type, value, decID
        rNotify.i4Type = 14;  // papi_af_slaN_OnNightModeChanged
        _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
    }    
     return TM_OK;
}

static INT32 _papi_af_sla_GetDRC(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    Bool* pfgDrc;
    Bool fgDrc;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgDrc = (Bool*)rArg.au4Arg[0];
    fgDrc = _fgDRC;     
    COPY_TO_USER(pfgDrc, &fgDrc, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_sla_SetDRC(UINT32 u4Arg){
    IOCTL_2ARG_T rArg;    
    Bool fgDrc;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    fgDrc = (Bool)rArg.au4Arg[0];    
    if (_fgDRC != fgDrc){
        _fgDRC = fgDrc;
        AUD_DspAC3CompMode(3, AUD_DEC_AUX);//RF mode
        rNotify.i4Type = 12;  // papi_af_slaN_OnDynamicRangeCompressionChanged
        _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
    }    
     return TM_OK;
}

static INT32 _papi_af_sla_IsDrcAllowed(UINT32 u4Arg)
{
    UINT32 u4Drc;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;
    IOCTL_2ARG_T rArg; 
    Bool* pfgDrc;
    Bool fgDrc = FALSE;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4Drc = (UINT32)rArg.au4Arg[0];    
    pfgDrc = (Bool*)rArg.au4Arg[1];

    //AUD_DrvGetDecodeState(AUD_DEC_AUX, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(AUD_DEC_AUX);
    
    // current, only AC3 can support DRC.
    // Todo : need to concern about Transport Stream 
    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        if (pDecState->eDecFormat == AUD_FMT_AC3)
            fgDrc = TRUE;   
     }

     COPY_TO_USER(pfgDrc, &fgDrc, sizeof (UINT32));    
    
    return TM_OK;    
}



static INT32 _papi_af_sla_GetImprovementMode(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    int* pi4Mode;
    int i4Mode;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pi4Mode = (int*)rArg.au4Arg[0];
    i4Mode = _fgImprovementMode;     
    COPY_TO_USER(pi4Mode, &i4Mode, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_sla_SetImprovementMode(UINT32 u4Arg){
    IOCTL_2ARG_T rArg;    
    int i4Mode;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4Mode = (int)rArg.au4Arg[0];    
    if (_fgImprovementMode != i4Mode){
        switch(i4Mode){
             case papi_af_sip_ImprovementNone :
                 _fgImprovementMode = papi_af_sip_ImprovementNone;
                 AUD_DspBbeMode(0, 0); // DecId , mode (0: off, 1: BBE, 2: BBE VIVA4)
                 rNotify.i4Type = 6;  // papi_af_sipN_OnImprovementModeChanged
                 _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
                 break;
             case papi_af_sip_ImprovementBbe :
                 _fgImprovementMode = papi_af_sip_ImprovementBbe;
                 AUD_DspBbeMode(0, 1); // DecId , mode (0: off, 1: BBE, 2: BBE VIVA4)
                 rNotify.i4Type = 6;  // papi_af_sipN_OnImprovementModeChanged
                 _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
                 break;
             case papi_af_sip_ImprovementDbe:
             case papi_af_sip_ImprovementUltraBass:
             case papi_af_sip_ImprovementMpb:
                break;
            default :    
                break;
        }
    }
    return TM_OK;
}
static INT32 _papi_af_sla_GetLoudness(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    Bool* pfgLoudness;
    Bool fgLoudness;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgLoudness = (Bool*)rArg.au4Arg[0];
    fgLoudness = _fgLoudness;
    COPY_TO_USER(pfgLoudness, &fgLoudness, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_sla_SetLoudness(UINT32 u4Arg){
    IOCTL_2ARG_T rArg;    
    Bool fgLoudness;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    fgLoudness = (Bool)rArg.au4Arg[0];    
    if (_fgLoudness != fgLoudness){
        _fgLoudness = fgLoudness;
        _AUD_DspSetLoudness(AUD_DEC_MAIN, _fgLoudness);            
    }
    rNotify.i4Type = 10;  // papi_af_sipN_OnLoudnessModeChanged
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
    return TM_OK;
}

static INT32 _papi_af_ssoo_GetEqBandValue(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    int* pi4EqVal;
    int i4EqVal;
    int i4EqBand;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pi4EqVal = (int*)rArg.au4Arg[0];
    i4EqBand = (int)rArg.au4Arg[1];
    i4EqVal = _i4EqVal[i4EqBand+1] + (EQ_BAND_GAIN_DB_MAX * 2);
    COPY_TO_USER(pi4EqVal, &i4EqVal, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_ssoo_SetEqBandValue(UINT32 u4Arg){
    IOCTL_2ARG_T rArg;    
    int i4EqVal;
    int i4EqBand;
    AUD_EQ_TYPE_T eEqType;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4EqBand = (Bool)rArg.au4Arg[0];    
    i4EqVal = (Bool)rArg.au4Arg[1];    

    i4EqVal = (i4EqVal < 0  ) ? 0  :i4EqVal;
    i4EqVal = (i4EqVal > 56) ? 56:i4EqVal;
       
    i4EqBand += 1;

    if (i4EqBand > CH_EQ_BAND_NO || i4EqBand < 1){
        return PAPI_FAIL;
    }    
    i4EqVal -= (EQ_BAND_GAIN_DB_MAX * 2);
    if (_i4EqVal[i4EqBand] != i4EqVal){
        _i4EqVal[i4EqBand] = i4EqVal;
        AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);        
        if (eEqType == AUD_EQ_OFF)
       {
           eEqType = AUD_EQ_CUST1;
       }        
        _AUD_DspChSetEqTable(0, eEqType, _i4EqVal);//u1DecId, eEqType, eqValue);        
    }else{
        return PAPI_FAIL;
    }    
    rNotify.i4Type = 23;  // papi_af_ssooN_OnEqualizerBandValueChanged
    rNotify.i4Data1 = i4EqBand;
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
    return TM_OK;
}

static INT32 _papi_af_ssoo_GetDeltaVolume(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    INT32* pi4DelVol;
    INT32 i4DelVol;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pi4DelVol = (int*)rArg.au4Arg[0];
    i4DelVol = _i4DelVol;
    COPY_TO_USER(pi4DelVol, &i4DelVol, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_ssoo_SetDeltaVolume(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;    
    INT32 i4DelVol, i4TmpVol;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4DelVol = (int)rArg.au4Arg[0];    

    i4DelVol = (i4DelVol < papi_af_ssoo_MinDeltaVolume) ? papi_af_ssoo_MinDeltaVolume:i4DelVol;
    i4DelVol = (i4DelVol > papi_af_ssoo_MaxDeltaVolume) ? papi_af_ssoo_MaxDeltaVolume:i4DelVol;
    
    if (_i4DelVol != i4DelVol)
    {
        _i4DelVol = i4DelVol;
        if(_i4SsooVolume != papi_af_ssoo_MinVolume)
        {
           i4TmpVol = (INT32)_i4SsooVolume + _i4DelVol;
           i4TmpVol = (i4TmpVol < papi_af_ssoo_MinVolume) ? papi_af_ssoo_MinVolume:i4TmpVol;
           i4TmpVol = (i4TmpVol > papi_af_ssoo_MaxVolume) ? papi_af_ssoo_MaxVolume:i4TmpVol;
           _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, i4TmpVol * 100 / papi_af_ssoo_MaxVolume);
           _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, i4TmpVol * 100 / papi_af_ssoo_MaxVolume);           
        }
    }
    rNotify.i4Type = 26;  // papi_af_ssooN_OnVolumeChanged
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
    return TM_OK;
}

static INT32 _papi_af_hsoo_GetDeltaVolume(UINT32 u4Arg){

    IOCTL_2ARG_T rArg;
    int* pi4DelVol;
    int i4DelVol;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pi4DelVol = (int*)rArg.au4Arg[0];
    i4DelVol = _i4HpDelVol;
    COPY_TO_USER(pi4DelVol, &i4DelVol, sizeof (UINT32));
    return TM_OK;
}

static INT32 _papi_af_hsoo_SetDeltaVolume(UINT32 u4Arg){
    IOCTL_2ARG_T rArg;    
    int i4DelVol, i4TmpVol;
    PAPI_AF_CB_T rNotify;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4DelVol = (int)rArg.au4Arg[0];    

    i4DelVol = (i4DelVol < papi_af_hsoo_MinDeltaVolume) ? papi_af_hsoo_MinDeltaVolume:i4DelVol;
    i4DelVol = (i4DelVol > papi_af_hsoo_MaxDeltaVolume) ? papi_af_hsoo_MaxDeltaVolume:i4DelVol;
  
    
    if (_i4HpDelVol != i4DelVol)
    {
        _i4HpDelVol = i4DelVol;
        if(_i4HsooVolume != papi_af_hsoo_MinVolume){
           i4TmpVol = (int)_i4HsooVolume + _i4HpDelVol;
           i4TmpVol = (i4TmpVol < papi_af_hsoo_MinVolume) ? papi_af_hsoo_MinVolume:i4TmpVol;
           i4TmpVol = (i4TmpVol > papi_af_hsoo_MaxVolume) ? papi_af_hsoo_MaxVolume:i4TmpVol;
           _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_DMX_LEFT, i4TmpVol * 100 / papi_af_hsoo_MaxVolume);
           _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_DMX_RIGHT, i4TmpVol * 100 / papi_af_hsoo_MaxVolume);
        }
    }
    rNotify.i4Type = 4;  // papi_af_hsooN_OnVolumeChanged
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);   
    return TM_OK;
}

static BOOL _IsADAllowed(void)
{
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;    
    
    //AUD_DrvGetDecodeState(AUD_DEC_AUX, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(AUD_DEC_AUX);

    // current, only DTV and MPEG can support AD.
    // Todo : need to concern about Transport Stream 
    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        if (pDecState->eDecFormat == AUD_FMT_MPEG)
            return TRUE;        
    }
    
    return FALSE;    
}

static INT32 _papi_af_sad_IsADRoutingAllowed(UINT32 u4Arg)
{
    // If AD is allowed, then Routing for AD is allowed.
    Bool *pfgAllowed;   
    Bool  fgAllowed;
    IOCTL_2ARG_T rArg;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgAllowed = (Bool*)rArg.au4Arg[0];

    if (_IsADAllowed())
    {
        fgAllowed = TRUE;
    }
    else
    {
        fgAllowed = FALSE;
    }

    // Copy data to user memory
    COPY_TO_USER(pfgAllowed, &fgAllowed, sizeof (UINT32));

    return TM_OK;
    
}

static INT32 _papi_af_sad_IsVolCtrlAllowed(UINT32 u4Arg)
{
    // If AD is allowed, then volume for AD is allowed.
    Bool *pfgAllowed;
    Bool  fgAllowed;
    IOCTL_2ARG_T rArg;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pfgAllowed = (Bool*)rArg.au4Arg[0];

    if (_IsADAllowed())
    {
        fgAllowed = TRUE;
    }
    else
    {
        fgAllowed = FALSE;
    }

    // Copy data to user memory
    COPY_TO_USER(pfgAllowed, &fgAllowed, sizeof (UINT32));

    return TM_OK;
}

static INT32 _papi_af_sad_GetADRouting(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* u4POutput;
    UINT32 u4Output;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u4POutput = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    u4Output = _u4ADOutputs;
    COPY_TO_USER(u4POutput, &u4Output, sizeof (UINT32));

    return TM_OK;    
}

static INT32 _papi_af_sad_SetADRouting(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4Output;        // output select, Get from AP
    UINT8 u1SetADMask = 0;  // output select, to set to driver
    BOOL  fgEnable;         // Enable flag, to set to driver

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    u4Output = (UINT32)rArg.au4Arg[0];
    _u4ADOutputs = u4Output;

    // Turn off AD.
    if (u4Output == PAPI_AF_NONE)
    {
        fgEnable = FALSE;
    }
    else
    {
        fgEnable = TRUE;                
        if (_IsOutMaskSetToSpeaker(u4Output) && _IsOutMaskSetToHeadPhone(u4Output))
        {
            u1SetADMask  = 2; //Both LSP and HP. (L&R and CH9 & 10)

        }
        else if (_IsOutMaskSetToSpeaker(u4Output))
        {
            u1SetADMask  = 1; //LSP (L&R)
        }
        else if (_IsOutMaskSetToHeadPhone(u4Output))
        {
            u1SetADMask  = 0; //HP (Ch9 & Ch10)
        }
        else
        {
            // todo : current don't suuport AD to SPDIF and MON.....
            fgEnable = FALSE;
        }
    }
    
    AUD_DspADModeSet(AUD_DEC_MAIN, u1SetADMask);	     
    AUD_DspADEnable(AUD_DEC_MAIN, fgEnable);
    
    return TM_OK;
}

static INT32 _papi_af_sad_SetADVolume(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32 i4SetADVol = 0;   // to set to driver

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    i4SetADVol = (INT32)rArg.au4Arg[0];

    i4SetADVol = (i4SetADVol < 0   ) ? 0    :i4SetADVol;
    i4SetADVol = (i4SetADVol > 100) ? 100:i4SetADVol;

    _i4ADVolume = i4SetADVol;

    // AD volume is Decoder 3 master volume 
    AUD_DspMasterVolume(AUD_DEC_THIRD, i4SetADVol);
    
    return TM_OK;
}

static INT32 _papi_af_sad_GetADVolume(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32* pADVolume;
    INT32  i4Volume;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pADVolume = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    i4Volume = _i4ADVolume;
    COPY_TO_USER(pADVolume, &i4Volume, sizeof (UINT8));

    return TM_OK;    
}

static INT32 _papi_af_ssoc_SetChannel(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT8 u1SetChannel = 0;   // to set to driver
    AUD_DEC_INFO_T pAudDecInfo;
    UINT8 u1DecID = AUD_DEC_AUX;    
    AUD_SOUND_MODE_T eAudSoundMode;
    PAPI_AF_CB_T rNotify;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
    u1SetChannel = (UINT8)rArg.au4Arg[0];
    _u1SetChannel = u1SetChannel;

    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        //todo ...
    }
    else if (pDecState->eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        eAudType = AUD_GetAudInfo(u1DecID);
        if (u1SetChannel == PAPI_AF_SSOC_CHANNEL1)
        {
            // Channel1 is always FM_MONO
            AUD_DspSetAtvOutputMode(u1DecID,AUD_SOUND_MODE_MONO);        
        }
        else if (u1SetChannel == PAPI_AF_SSOC_CHANNEL2)
       {
           
            switch (eAudType)
            {
                case AUD_TYPE_MONO:
                    eAudSoundMode = AUD_SOUND_MODE_MONO;                   
                case AUD_TYPE_STEREO:
                case AUD_TYPE_STEREO_SUB:            
                    eAudSoundMode = AUD_SOUND_MODE_STEREO;         
                case AUD_TYPE_MONO_SUB:
                default :
                    eAudSoundMode = AUD_SOUND_MODE_MONO;                    
                    break;
            }         
            AUD_DspSetAtvOutputMode(u1DecID,eAudSoundMode);
        }
        else if (u1SetChannel == PAPI_AF_SSOC_CHANNEL3)
        {
            switch (eAudType)
            {
                case AUD_TYPE_STEREO:
                    eAudSoundMode = AUD_SOUND_MODE_STEREO;            
                    break;        
                case AUD_TYPE_STEREO_SUB:            
                case AUD_TYPE_MONO_SUB:
                    eAudSoundMode = AUD_SOUND_MODE_SUB_LANG;            
                    break;
                case AUD_TYPE_DUAL_MONO:
                    eAudSoundMode = AUD_SOUND_MODE_DUAL2;
                    break;
                default :
                    eAudSoundMode = AUD_SOUND_MODE_MONO;                    
                    break;
            }         
            AUD_DspSetAtvOutputMode(u1DecID,eAudSoundMode);        
        }
    }
    
    
        
    // send callback
    rNotify.i4Type = 20; //papi_af_ssocN_OnChannelChanged
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
    return TM_OK;
}

static INT32 _papi_af_ssoc_GetChannel(UINT32 u4Arg)
{

    IOCTL_2ARG_T rArg;
    UINT32* pu4Chn;
    UINT32 u4Chn;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4Chn = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    u4Chn = _u1SetChannel;
    COPY_TO_USER(pu4Chn, &u4Chn, sizeof (UINT32));

    return TM_OK;

}
/*
    AUD_TYPE_UNKNOWN = 0,
    AUD_TYPE_MONO,                  // 1/0 
    AUD_TYPE_MONO_SUB,              // 1+sub-language 
    AUD_TYPE_DUAL_MONO,             // 1+1 
    AUD_TYPE_STEREO,                // 2/0 
    AUD_TYPE_STEREO_SUB,            // 2+sub-language 
    AUD_TYPE_STEREO_DOLBY_SURROUND, // 2/0, dolby surround 
    AUD_TYPE_SURROUND_2CH,          // 2/1 
    AUD_TYPE_SURROUND,             / / 3/1 
    AUD_TYPE_3_0,                   // 3/0 
    AUD_TYPE_4_0,                   // 2/2 
    AUD_TYPE_5_1,                   // 3/2 +SW 
    AUD_TYPE_7_1,                   // 5/2 
    AUD_TYPE_FM_MONO_NICAM_MONO,
    AUD_TYPE_FM_MONO_NICAM_STEREO,
    AUD_TYPE_FM_MONO_NICAM_DUAL,    
    AUD_TYPE_OTHERS,
    AUD_TYPE_5_0,                   // 3/2 
*/

static AUD_TYPE_T _GetCurrentAudioType(UINT8 u1DecID) 
{
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;
    
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
    }
    else if (pDecState->eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        eAudType = AUD_GetAudInfo(u1DecID);
    }

    return eAudType;     
}

static INT32 _papi_af_ssoc_IsStereo(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    Bool *pIsSupport;
    Bool fgOK = FALSE;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
    u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pIsSupport = (Bool*)rArg.au4Arg[1];

        
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        if ((eAudType == AUD_TYPE_STEREO)||(eAudType == AUD_TYPE_STEREO_SUB) || (eAudType == AUD_TYPE_STEREO_DOLBY_SURROUND))
        {
            fgOK = TRUE;
        }
    }
    else if (pDecState->eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        eAudType = AUD_GetAudInfo(u1DecID);
        if(u4SetChannel == PAPI_AF_SSOC_CHANNEL2 ){
            if ((eAudType == AUD_TYPE_STEREO)||(eAudType == AUD_TYPE_STEREO_SUB))
            {
                fgOK = TRUE;
            }
        }
        else if(u4SetChannel == PAPI_AF_SSOC_CHANNEL3 && eAudType == AUD_TYPE_STEREO)
       {
            fgOK = TRUE;
        }
        else
        {
            fgOK = FALSE;
        }        
    }   
    // Copy data to user memory
    COPY_TO_USER(pIsSupport, &fgOK, sizeof(Bool));

    return TM_OK;
    
}   

static INT32 _papi_af_ssoc_IsSubLang(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    Bool *pIsSupport;
    Bool fgOK = FALSE;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
    u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pIsSupport = (Bool*)rArg.au4Arg[1];


    
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        if(u4SetChannel == PAPI_AF_SSOC_CHANNEL3 && eAudType == AUD_TYPE_DUAL_MONO)
       {
            fgOK = TRUE;
        }else{
            fgOK = FALSE;
        }
    }
    else if (pDecState->eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        eAudType = AUD_GetAudInfo(u1DecID);
        if(u4SetChannel == PAPI_AF_SSOC_CHANNEL3 ){
            if(eAudType == AUD_TYPE_STEREO_SUB || eAudType == AUD_TYPE_MONO_SUB){
                fgOK = TRUE;            
            }
        }
        else
        {
            fgOK = FALSE;
        }        
    }   
    // Copy data to user memory
    COPY_TO_USER(pIsSupport, &fgOK, sizeof(Bool));

    return TM_OK;    
}


static INT32 _papi_af_ssoc_IsMono(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
   UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    Bool *pIsSupport;
    Bool fgOK = FALSE;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
     u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pIsSupport = (Bool*)rArg.au4Arg[1];

    
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        if ((eAudType == AUD_TYPE_MONO))
        {
            fgOK = TRUE;
        }          
    }
    else if (pDecState->eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        eAudType = AUD_GetAudInfo(u1DecID);
        if(u4SetChannel == PAPI_AF_SSOC_CHANNEL1 ){
            fgOK = TRUE;            
        }
        else if(u4SetChannel == PAPI_AF_SSOC_CHANNEL2 ){
            if(eAudType == AUD_TYPE_MONO || eAudType == AUD_TYPE_MONO_SUB){
                fgOK = TRUE;            
            }
        }
        else if(u4SetChannel == PAPI_AF_SSOC_CHANNEL3 && eAudType == AUD_TYPE_MONO ){
                fgOK = TRUE;                        
        }
        else
        {
            fgOK = FALSE;
        }        
    }   
    // Copy data to user memory
    COPY_TO_USER(pIsSupport, &fgOK, sizeof(Bool));

    return TM_OK;
    
}

static INT32 _papi_af_ssoc_IsDigital(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    Bool *pIsSupport;
    Bool fgOK = FALSE;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
     u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pIsSupport = (Bool*)rArg.au4Arg[1];

    
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER){
        fgOK = FALSE;
    }
    else{
        fgOK = TRUE;
    }    
    // Copy data to user memory
    COPY_TO_USER(pIsSupport, &fgOK, sizeof(Bool));

    return TM_OK;
    
}   

static INT32 _papi_af_ssoc_IsDPLII(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    Bool *pIsSupport;
    Bool fgOK = FALSE;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
    u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pIsSupport = (Bool*)rArg.au4Arg[1];
    
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER){
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        if( eAudType == AUD_TYPE_STEREO_DOLBY_SURROUND){
            fgOK = TRUE;
        }        
    }
    else{
        fgOK = FALSE;
    }    
    // Copy data to user memory
    COPY_TO_USER(pIsSupport, &fgOK, sizeof(Bool));

    return TM_OK;
    
}     

static INT32 _papi_af_ssoc_FrontCh(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    int *pi4ChNum;
    int i4ChNum;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
     u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pi4ChNum = (int*)rArg.au4Arg[1];

    
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER){
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        switch (eAudType){
            case  AUD_TYPE_MONO:
            case  AUD_TYPE_MONO_SUB:
            case  AUD_TYPE_DUAL_MONO:
            case  AUD_TYPE_FM_MONO_NICAM_MONO:
            case  AUD_TYPE_FM_MONO_NICAM_DUAL:
                i4ChNum = 2;//MONO 2 output            
                break;
            case  AUD_TYPE_STEREO:
            case  AUD_TYPE_STEREO_SUB:
            case  AUD_TYPE_STEREO_DOLBY_SURROUND:
            case  AUD_TYPE_SURROUND_2CH:
            case  AUD_TYPE_4_0:
            case  AUD_TYPE_FM_MONO_NICAM_STEREO:
            case  AUD_TYPE_5_0:
            case  AUD_TYPE_OTHERS:
            case  AUD_TYPE_UNKNOWN:
                i4ChNum = 2;
                break;
            case  AUD_TYPE_3_0:
            case  AUD_TYPE_SURROUND:
            case  AUD_TYPE_5_1:            
                i4ChNum = 3;
                break;
            case  AUD_TYPE_7_1:    
                i4ChNum = 5;
                break;
        }        
    }
    else{
        i4ChNum = 2;//ATV always 2 ch out        
    }    
    // Copy data to user memory
    COPY_TO_USER(pi4ChNum, &i4ChNum, sizeof(Bool));

    return TM_OK;
    
}     

static INT32 _papi_af_ssoc_RearCh(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    int *pi4ChNum;
    int i4ChNum;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
         u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pi4ChNum = (int*)rArg.au4Arg[1];

    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER){
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        switch (eAudType){
            case  AUD_TYPE_MONO:
            case  AUD_TYPE_MONO_SUB:
            case  AUD_TYPE_DUAL_MONO:
            case  AUD_TYPE_FM_MONO_NICAM_MONO:
            case  AUD_TYPE_FM_MONO_NICAM_DUAL:               
            case  AUD_TYPE_STEREO:
            case  AUD_TYPE_STEREO_SUB:
            case  AUD_TYPE_STEREO_DOLBY_SURROUND:
            case  AUD_TYPE_FM_MONO_NICAM_STEREO:
            case  AUD_TYPE_OTHERS:
            case  AUD_TYPE_UNKNOWN:
            case  AUD_TYPE_3_0:
                i4ChNum = 0;
                break;
            case  AUD_TYPE_SURROUND:
            case  AUD_TYPE_SURROUND_2CH:
                i4ChNum = 1;
                break;
            case  AUD_TYPE_4_0:
            case  AUD_TYPE_5_0:
            case  AUD_TYPE_5_1:
            case  AUD_TYPE_7_1:
                i4ChNum = 2;
                break;            
        }        
    }
    else{
       i4ChNum = 0;
    }    
    // Copy data to user memory
    COPY_TO_USER(pi4ChNum, &i4ChNum, sizeof(Bool));

    return TM_OK;
    
}     

static INT32 _papi_af_ssoc_LFE(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4SetChannel = 0;   // to set to driver
    UINT8 u1DecID = AUD_DEC_AUX;  
    int *pi4ChNum;
    int i4ChNum;
    //UINT32 u4Address;
    AUD_DECODER_T  *pDecState = NULL;  
    AUD_TYPE_T eAudType = AUD_TYPE_MONO;
    AUD_DEC_INFO_T prAudInfo;


    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));
    u4SetChannel = (UINT32)rArg.au4Arg[0];    
    pi4ChNum = (int*)rArg.au4Arg[1];

    
    //AUD_DrvGetDecodeState(u1DecID, &u4Address);
    //pDecState = (AUD_DECODER_T *)u4Address;
    pDecState = AUD_DrvGetDecodeState(u1DecID);

    if (pDecState->eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER){
        AUD_GetDtvAudInfo(u1DecID,&prAudInfo);
        eAudType = prAudInfo.e_aud_type;
        switch (eAudType){
            case  AUD_TYPE_5_1:
            case  AUD_TYPE_7_1:
                i4ChNum = 1;
                break;
            default:
               i4ChNum = 0;
        }        
    }
    else{
        i4ChNum = 0;
    }    
    // Copy data to user memory
    COPY_TO_USER(pi4ChNum, &i4ChNum, sizeof(Bool));

    return TM_OK;
    
}     
 
static INT32 _papi_af_sound_init(UINT32 u4Arg)
{
    INT32  i4Ret = PAPI_FAIL;
    static BOOL fgInit = FALSE;

    UNUSED(u4Arg);
    
    if (!fgInit)
    {   
        fgInit = TRUE;
    
        i4Ret = AUD_Init(); 

        ADAC_Init();
        ADAC_SpeakerEnable(TRUE);
    
        // Defualt load pcm decoder ..    
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);    

        // Default set to Lt/Rt downmix
        AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN, 0);

        // Setup channel delay to 0
        AUD_DspChannelDelay(0, AUD_CH_ALL, AUD_DEC_MAIN);

        // Default set SPDIF to AC3 raw data
        AUD_DspIECConfig( AUD_IEC_CFG_PCM, TRUE);        

        //Default set master volume to max use channel trim to adjust volume
        _AUD_DspMasterVolume(AUD_DEC_MAIN, 100);

        x_sema_create(&_hIoAfMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK);

        x_sema_create(&_hIoAfNfySema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        AUD_RegNotifyFunc(&u4AudNfyHandler, _af_nfy);
    }   
    else
    {
        i4Ret = PAPI_OK;
    }

    return i4Ret;
}

static void _AudAvInputMuxSel(UINT16 u2Source)
{
    switch(u2Source)                                                                                   
    {                                                                                                           
    case papi_src_srcid_SrcFront:
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_1);
        break;    
    case papi_src_srcid_SrcAv1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_0);
        break;
    case papi_src_srcid_SrcAv2:                                                                           
        AUD_AvInputMuxSel(AUD_INPUT_ID_S_VID_0);
        break;
    case papi_src_srcid_SrcAv3: 
        AUD_AvInputMuxSel(AUD_INPUT_ID_YPBPR_0);
        break;
    case papi_src_srcid_SrcAv4: 
        AUD_AvInputMuxSel(AUD_INPUT_ID_YPBPR_1);
        break;        
    case papi_src_srcid_SrcHdmi1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_0);
        break;                                                                                   
    case papi_src_srcid_SrcHdmi2:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_1);
        break;                                                                                   
    case papi_src_srcid_SrcHdmi3:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_2);
        break;                                                                                     
    case papi_src_srcid_SrcHdmi4:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_3);
        break;                                                                                       
    default:
        // Not support
        printk("Not support AvInputMux\n");                                                                                                   
    }
}    

#if 0 //move to src_src
static INT32 _papi_af_sound_connect(UINT32 u4Arg)
{
    INT32 i4Ret = PAPI_FAIL;
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Designator;
    UINT16 u2Source;  

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u1DecId = AUD_DEC_MAIN;        
    u2Source = (UINT16)rArg.au4Arg[0];
    u4Designator = (UINT32)rArg.au4Arg[1];

    //unmute codec.
    ADAC_Mute(FALSE);
        
    // Set decoder type    
    switch (u2Source)
    {
    case papi_src_srcid_SrcTuner:
        if (u4Designator == papi_src_dsgid_DesignatorDigital)
        {
            AUD_SetDecType(u1DecId, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_MPEG);
            _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_DIGITAL_TUNER;            
            AUD_DSPCmdPlayAsyn(u1DecId);
        }
        else if (u4Designator == papi_src_dsgid_DesignatorAnalog)
        {
            AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_PAL);
            _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_ANALOG_TUNER;            
            AUD_DSPCmdDualDecPlay(u1DecId);
        }
        else
        {
            return PAPI_FAIL;
        }
        break;
    case papi_src_srcid_SrcAv1:
        AUD_SetDecType(u1DecId, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_LINE_IN;      
        _AudAvInputMuxSel(u2Source);
        AUD_DSPCmdPlayAsyn(u1DecId);
        break;
    case papi_src_srcid_SrcInvalid:
    case papi_src_srcid_SrcNone: 
    case papi_src_srcid_SrcFront:
    case papi_src_srcid_SrcAv2: 
    case papi_src_srcid_SrcAv3: 
    case papi_src_srcid_SrcAv4: 
    case papi_src_srcid_SrcAv5: 
    case papi_src_srcid_SrcAv6: 
    case papi_src_srcid_SrcAv7: 
    case papi_src_srcid_SrcAv8: 
    case papi_src_srcid_SrcAv9: 
    case papi_src_srcid_SrcAv10: 
    case papi_src_srcid_SrcHdmi1:
    case papi_src_srcid_SrcHdmi2:
    case papi_src_srcid_SrcHdmi3:
    case papi_src_srcid_SrcHdmi4:
    case papi_src_srcid_SrcHdmi5:
    case papi_src_srcid_SrcUrl: 
    case papi_src_srcid_SrcBolton:
    case papi_src_srcid_SrcSpdif1: 
    case papi_src_srcid_SrcSpdif2: 
    case papi_src_srcid_SrcAnalogLr1: 
    case papi_src_srcid_SrcAnalogLr2:
        return  PAPI_FAIL;
    default:
        return  PAPI_FAIL;
    }

    printk("\nPAPISound: u2Source %u is connected\n", u2Source);
    i4Ret = PAPI_OK;
    
    return i4Ret;
}
#endif

static INT32 _papi_af_sound_disconnect(UINT32 u4Arg)
{
    INT32 i4Ret = PAPI_FAIL;
    UINT8 u1DecId;
    IOCTL_2ARG_T rArg;    
    UINT32 u4Designator;
    UINT16 u2Source;  

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    u1DecId = AUD_DEC_MAIN;        
    u2Source = (UINT16)rArg.au4Arg[0];
    u4Designator = (UINT32)rArg.au4Arg[1];

    _aeStrSrc[u1DecId] = AUD_STREAM_FROM_OTHERS;
    // Set decoder Play
    printk("\nPAPISound: u2Source %u is DisConnected\n", u2Source);
    AUD_DSPCmdStop(u1DecId);    

    i4Ret = PAPI_OK;
    return  i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void papi_af_sound_properties_nfy(void)
{
    PAPI_AF_CB_T rNotify;

    rNotify.i4Type = 21; //papi_af_ssocN_OnSoundPropertiesChanged
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
}

void papi_af_drc_allowed_changed_nfy(void)
{
    PAPI_AF_CB_T rNotify;

    rNotify.i4Type = 11; //papi_af_slaN_OnDynamicRangeCompressionAllowedChanged
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);

}

void _af_lock(void)
{
    x_sema_lock(_hIoAfMutex, X_SEMA_OPTION_WAIT);
}

void _af_unlock(void)
{
    x_sema_unlock(_hIoAfMutex);
}


void _af_wait(void)
{
    x_sema_lock(_hIoAfNfySema, X_SEMA_OPTION_WAIT);
}

#if 0
void papi_af_ssoo_vol_changed_nfy(int data1)
{
    PAPI_AF_CB_T rNotify;
    
    rNotify.i4Type = (PAPI_AF_NOTIFY_T)PAPI_AF_SSOO_VOLUME_CHANGED; 
    rNotify.i4Data1 = data1;
    rNotify.i4Data2 = 0;
    
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
}

void papi_af_ssoo_mute_changed_nfy(int data1)
{
    PAPI_AF_CB_T rNotify;
    
    rNotify.i4Type = (PAPI_AF_NOTIFY_T)PAPI_AF_SSOO_MUTE_CHANGED; 
    rNotify.i4Data1 = data1;
    rNotify.i4Data2 = 0;
    
    _CB_PutEvent(CB_PAPI_AF_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
}
#endif
int papi_ioctl_af(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = PAPI_FAIL;

    _af_lock();

    switch (cmd)
    {
    case PAPI_IO_AF_SOUND_INIT:
        i4Ret = _papi_af_sound_init(arg);
        break;
#if 0   // move to src_src
    case PAPI_IO_AF_SOUND_CONNECT:
        i4Ret = _papi_af_sound_connect(arg);
        break;
#endif        
    case PAPI_IO_AF_SOUND_DISCONNECT:
        i4Ret = _papi_af_sound_disconnect(arg);
        break;        
    case PAPI_IO_AF_SSOO_SETVOLUME:
        i4Ret = _papi_af_ssoo_SetVolume(arg);
        break;
    case PAPI_IO_AF_SSOO_GETVOLUME:
        i4Ret = _papi_af_ssoo_GetVolume(arg);
        break;
    case PAPI_IO_AF_SSOO_SETBALANCE:
        i4Ret = _papi_af_ssoo_SetBalance(arg);
        break;
    case PAPI_IO_AF_SSOO_GETBALANCE:
        i4Ret = _papi_af_ssoo_GetBalance(arg);
        break;
    case PAPI_IO_AF_SSOO_SETBASS:
        i4Ret = _papi_af_ssoo_SetBass(arg);
        break;
    case PAPI_IO_AF_SSOO_GETBASS:
        i4Ret = _papi_af_ssoo_GetBass(arg);
        break;
    case PAPI_IO_AF_SSOO_SETTREBLE:
        i4Ret = _papi_af_ssoo_SetTreble(arg);
        break;
    case PAPI_IO_AF_SSOO_GETTREBLE:
        i4Ret = _papi_af_ssoo_GetTreble(arg);
        break;
    case PAPI_IO_AF_SSOO_SETMUTE:
        i4Ret = _papi_af_ssoo_SetMute(arg);
        break;
    case PAPI_IO_AF_SSOO_GETMUTE:
        i4Ret = _papi_af_ssoo_GetMute(arg);
        break;
    case PAPI_IO_AF_SSOO_MUTE_ENABLE:
        i4Ret = _papi_af_ssoo_Mute_Enable(arg);
        break;        
    case PAPI_IO_AF_SSOO_GETEQBANDVALUE :
        i4Ret = _papi_af_ssoo_GetEqBandValue(arg);
        break;        
    case PAPI_IO_AF_SSOO_SETEQBANDVALUE :
        i4Ret = _papi_af_ssoo_SetEqBandValue(arg);
        break;        
    case PAPI_IO_AF_SSOO_GETDELTAVOLUME :
        i4Ret = _papi_af_ssoo_GetDeltaVolume(arg);
        break;        
    case PAPI_IO_AF_SSOO_SETDELTAVOLUME :
        i4Ret = _papi_af_ssoo_SetDeltaVolume(arg);
        break;        

//****HSOO****
        
    case PAPI_IO_AF_HSOO_GETVOLUME:
        i4Ret = _papi_af_hsoo_GetVolume(arg);
        break;
    case PAPI_IO_AF_HSOO_SETVOLUME:
        i4Ret = _papi_af_hsoo_SetVolume(arg);
        break;
    case PAPI_IO_AF_HSOO_SETMUTE:
        i4Ret = _papi_af_hsoo_SetMute(arg);
        break;
    case PAPI_IO_AF_HSOO_GETMUTE:
        i4Ret = _papi_af_hsoo_GetMute(arg);
        break;
    case PAPI_IO_AF_HSOO_GETDELTAVOLUME :
        i4Ret = _papi_af_hsoo_GetDeltaVolume(arg);
        break;        
    case PAPI_IO_AF_HSOO_SETDELTAVOLUME :
        i4Ret = _papi_af_hsoo_SetDeltaVolume(arg);
        break;        
        

//****SMT****
        
    case PAPI_IO_AF_SMT_MUTE_OUTPUT:
        i4Ret = _papi_af_smt_Mute_Output(arg); 
        break;
    case PAPI_IO_AF_SMT_GET_MUTE_STATUS:
        i4Ret = _papi_af_smt_GetMuteStatus(arg); 
        break;


//****SOM****

    case PAPI_IO_AF_SOM_GETAUTOSURROUND:
        i4Ret = _papi_af_som_GetAutoSurround(arg);
        break;
    case PAPI_IO_AF_SOM_SETAUTOSURROUND:
        i4Ret = _papi_af_som_SetAutoSurround(arg);
        break;
    case PAPI_IO_AF_SOM_GETSURROUNDMODE:
        i4Ret = _papi_af_som_GetSurroundMode(arg);
        break;
    case PAPI_IO_AF_SOM_SETSURROUNDMODE:
        i4Ret = _papi_af_som_SetSurroundMode(arg);
        break;
    case PAPI_IO_AF_SOM_GETSURROUNDLEVEL:
        i4Ret = _papi_af_som_GetSurroundLevel(arg);
        break;
    case PAPI_IO_AF_SOM_SETSURROUNDLEVEL:
        i4Ret = _papi_af_som_SetSurroundLevel(arg);
        break;
    case PAPI_IO_AF_SOM_GETSURROUNDMODEEFFECTSTRENGTH:
        i4Ret = _papi_af_som_GetSurroundModeEffectStrength(arg);
        break;
    case PAPI_IO_AF_SOM_SETSURROUNDMODEEFFECTSTRENGTH:
        i4Ret = _papi_af_som_SetSurroundModeEffectStrength(arg);
        break;
    case PAPI_IO_AF_SOM_GETSURROUNDLEVELEFFECTSTRENGTH:
        i4Ret = _papi_af_som_GetSurroundLevelEffectStrength(arg);
        break;
    case PAPI_IO_AF_SOM_SETSURROUNDLEVELEFFECTSTRENGTH:
        i4Ret = _papi_af_som_SetSurroundLevelEffectStrength(arg);
        break;
    
//****SLA****
    case PAPI_IO_AF_SLA_GETAVL:
        i4Ret = _papi_af_sla_GetAutoVolumeLevel(arg);
        break;
    case PAPI_IO_AF_SLA_SETAVL:
        i4Ret = _papi_af_sla_SetAutoVolumeLevel(arg);
        break;
    case PAPI_IO_AF_SLA_GETNIGHTMODE:
        i4Ret = _papi_af_sla_GetNightMode(arg);
        break;
    case PAPI_IO_AF_SLA_SETNIGHTMODE:
        i4Ret = _papi_af_sla_SetNightMode(arg);
        break;
    case PAPI_IO_AF_SLA_GETDRC:
        i4Ret = _papi_af_sla_GetDRC(arg);
        break;
    case PAPI_IO_AF_SLA_SETDRC:
        i4Ret = _papi_af_sla_SetDRC(arg);
        break;
    case PAPI_IO_AF_SIP_ISDRCALLOWED:
         i4Ret = _papi_af_sla_IsDrcAllowed(arg);
        break;

//****SIP****
    case PAPI_IO_AF_SIP_GETIMPROVEMENTMODE :
        i4Ret = _papi_af_sla_GetImprovementMode(arg);
        break;
    case PAPI_IO_AF_SIP_SETIMPROVEMENTMODE :   
        i4Ret = _papi_af_sla_SetImprovementMode(arg);
        break;
    case PAPI_IO_AF_SIP_GETLOUDNESS :                 
        i4Ret = _papi_af_sla_GetLoudness(arg);
        break;
    case PAPI_IO_AF_SIP_SETLOUDNESS :                 
        i4Ret = _papi_af_sla_SetLoudness(arg);
        break;

//****SAD****    
    case PAPI_IO_AF_SAD_ISADROUTINGALLOWED:
        i4Ret = _papi_af_sad_IsADRoutingAllowed(arg);
        break;         
    case PAPI_IO_AF_SAD_SETROUTING:
        i4Ret = _papi_af_sad_SetADRouting(arg);
        break;
    case PAPI_IO_AF_SAD_GETROUTING:
        i4Ret = _papi_af_sad_GetADRouting(arg);
        break;        
    case PAPI_IO_AF_SAD_IS_VOLCTRL_ALLOWED:
        i4Ret = _papi_af_sad_IsVolCtrlAllowed(arg);
        break;    
    case PAPI_IO_AF_SAD_SETVOLUME:
        i4Ret = _papi_af_sad_SetADVolume(arg);
        break;
    case PAPI_IO_AF_SAD_GETVOLUME:
        i4Ret = _papi_af_sad_GetADVolume(arg);
        break;            

//****SSOC****
    case PAPI_IO_AF_SSOC_SET_CHN:
        i4Ret = _papi_af_ssoc_SetChannel(arg);
        break;
    case PAPI_IO_AF_SSOC_GET_CHN:
        i4Ret = _papi_af_ssoc_GetChannel(arg);
        break;        
    case PAPI_IO_AF_SSOC_IS_STEREO:
        i4Ret = _papi_af_ssoc_IsStereo(arg);
        break;        
    case PAPI_IO_AF_SSOC_IS_SUBLANG:
        i4Ret = _papi_af_ssoc_IsSubLang(arg);
        break;                
    case PAPI_IO_AF_SSOC_IS_MONO:
        i4Ret = _papi_af_ssoc_IsMono(arg);
        break;                   
    case PAPI_IO_AF_SSOC_IS_DIGITAL:
        i4Ret = _papi_af_ssoc_IsDigital(arg);
        break;                   
    case PAPI_IO_AF_SSOC_IS_DPLII:
        i4Ret = _papi_af_ssoc_IsDPLII(arg);
        break;                   
    case PAPI_IO_AF_SSOC_FRONTCH :
        i4Ret = _papi_af_ssoc_FrontCh(arg);
        break;                   
    case PAPI_IO_AF_SSOC_REARCH:
        i4Ret = _papi_af_ssoc_RearCh(arg);
        break;                   
    case PAPI_IO_AF_SSOC_LFE:
        i4Ret = _papi_af_ssoc_LFE(arg);
        break;                          
       
    default:
        printk("papi_ioctl_af: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    _af_unlock();

    return i4Ret;
}

