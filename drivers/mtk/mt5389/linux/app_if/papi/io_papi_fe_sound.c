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
 * $RCSfile: io_papi_fe_sound.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_fe_sound.c
 *  PAPI kernel driver implementation: Frontend sound functions
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "aud_if.h"
#include "aud_drvif.h"
#include "papi.h"
#include "papi_types.h"
//#include "papi_af_types.h"
#include <linux/papi_ioctl.h>


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

// Sound systerm 
#define  SND_SOUND_PRESENT      (0x1 << 0)
#define  SND_IS_ANALOG          (0x1 << 1)
#define  SND_IS_DIGITAL         (0x1 << 2)  
#define  SND_IS_BASEBAND        (0x1 << 3)
#define  SND_IS_AM              (0x1 << 4)
#define  SND_IS_FM              (0x1 << 5) 
#define  SND_IS_NICAM           (0x1 << 6)
#define  SND_IS_2CS             (0x1 << 7)   
#define  SND_IS_BTSC            (0x1 << 8)
#define  SND_IS_EIAJ            (0x1 << 9)
#define  SND_IS_PCM             (0x1 << 10)
#define  SND_IS_AC3             (0x1 << 11)    
#define  SND_IS_MPEG            (0x1 << 12)    
#define  SND_IS_DTS             (0x1 << 13)    
#define  SND_CONTAINS_DATA      (0x1 << 14)
#define  SND_CONTAINS_DUAL      (0x1 << 15)
#define  SND_CONTAINS_UNRELATED (0x1 << 16)    
#define  SND_CONTAINS_PROLOGIC  (0x1 << 17)    
#define  SND_CONTAINS_DRC       (0x1 << 18)    
#define  SND_CONTAINS_KARAOKE   (0x1 << 19)

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

//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
void papi_fe_sound_nfy(AUD_DEC_INFO_T* prAud_Dec_Info);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _papi_fe_sound_AllowSndSystem(UINT32 u4Arg)
{    
    return TM_OK;
}

static INT32 _papi_fe_sound_GetAllowSndSystem(UINT32 u4Arg)
{    
    IOCTL_2ARG_T rArg;
    UINT32* pu4SndSys;
    UINT32 u4SndSys = 0;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
    
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4SndSys = (UINT32*)rArg.au4Arg[0];

    AUD_GetDecodeType(AUD_DEC_MAIN, &eStreamFrom, &eDecType);

    if (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        u4SndSys = (papi_fe_snd_AllowAnalog | papi_fe_snd_AllowNicam | papi_fe_snd_Allow2CS);
    }
    else if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        u4SndSys = (papi_fe_snd_AllowDigital | papi_fe_snd_AllowPCM | papi_fe_snd_AllowMPEG);
    }
    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        u4SndSys = (papi_fe_snd_AllowAnalog | papi_fe_snd_AllowPCM);
    }    
                    
    // Copy data to user memory
    COPY_TO_USER(pu4SndSys, &u4SndSys, sizeof (UINT32));
    
    return TM_OK;
}

static INT32 _papi_fe_sound_GetSndSystem(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4SndSys;
    UINT32 u4SndSys = 0;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
      
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pu4SndSys = (UINT32*)rArg.au4Arg[0];

   AUD_GetDecodeType(AUD_DEC_MAIN, &eStreamFrom, &eDecType);

    if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        AUD_GetDecodeType(AUD_DEC_AUX, &eStreamFrom, &eDecType);
        u4SndSys = (SND_SOUND_PRESENT|SND_IS_DIGITAL);
        switch (eDecType)
        {        
        case AUD_FMT_MPEG:
        case AUD_FMT_MP3:    
            u4SndSys = u4SndSys|SND_IS_MPEG;
            break;    
        case AUD_FMT_PCM:
            u4SndSys = u4SndSys|SND_IS_PCM;
            break;
        case AUD_FMT_AC3:
            u4SndSys = u4SndSys|SND_IS_AC3;
            break;            
        }        
    }
    else if (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        AUD_GetDecodeType(AUD_DEC_AUX, &eStreamFrom, &eDecType);
        u4SndSys = (SND_SOUND_PRESENT|SND_IS_ANALOG);
        switch (eDecType)
        {        
        case AUD_FMT_PAL:
            u4SndSys = u4SndSys|SND_IS_NICAM;
            break;    
        case AUD_FMT_A2:
            u4SndSys = u4SndSys|SND_IS_FM;
            break;
        }        
    }else{
            if(eDecType == AUD_FMT_PCM )
                u4SndSys = (SND_SOUND_PRESENT|SND_IS_PCM);
    }    
    // Copy data to user memory
    COPY_TO_USER(pu4SndSys, &u4SndSys, sizeof (UINT32));
    return TM_OK;
    
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void papi_fe_sound_nfy(AUD_DEC_INFO_T* prAud_Dec_Info)
{
    PAPI_AF_CB_T rNotify;
#if 0
    UINT32 u4SndMode = 0;

    switch (prAud_Dec_Info->e_aud_fmt)
    {
    case AUD_FMT_MPEG:
        u4SndMode = (SND_SOUND_PRESENT | SND_IS_DIGITAL | SND_IS_MPEG);        
        break;     
    case AUD_FMT_PAL:
        u4SndMode = (SND_SOUND_PRESENT | SND_IS_ANALOG | SND_IS_FM);        
        break;
    case AUD_FMT_PCM:
        u4SndMode = (SND_SOUND_PRESENT | SND_IS_DIGITAL | SND_IS_PCM);        
        break;         
    default:
        break;
    }
            
    rNotify.i4Type = (PAPI_AF_NOTIFY_T)PAPI_FE_SSOO_SOUNDSYS_CHANGED; 
    rNotify.i4Data1 = u4SndMode;
    rNotify.i4Data2 = 0;
#endif
    rNotify.i4Type = 0; 
    _CB_PutEvent(CB_PAPI_FE_SND_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
    rNotify.i4Type = 1; 
    _CB_PutEvent(CB_PAPI_FE_SND_TRIGGER, sizeof(PAPI_AF_CB_T), (void *)&rNotify);
}

#if 0
int papi_fe_sound_tvsnd_detect(tmFe_TvSystem_t tvSystem, Nat32 *retval)
{
    AUD_FMT_T eAudFmt; 
    TV_AUD_SYS_T eTvSndSys;
    UINT32 u4TvSoundSys;

    u4TvSoundSys = 0;        
    if (AUD_DspATVFmtDetectSync(&eAudFmt, &eTvSndSys))
    {
        return TM_FAIL;
    }

    switch(eTvSndSys)
    {
    case SV_A2_BG:
	case SV_A2_DK1:
	case SV_A2_DK2:
	case SV_A2_DK3:
        u4TvSoundSys = (SND_SOUND_PRESENT | SND_IS_ANALOG | SND_IS_AM);
        break;
	case SV_PAL_I:
	case SV_PAL_BG:
	case SV_PAL_DK:
        u4TvSoundSys = (SND_SOUND_PRESENT | SND_IS_ANALOG | SND_IS_FM);
        break; 
    case SV_SECAM_L:
        u4TvSoundSys = (SND_SOUND_PRESENT | SND_IS_ANALOG | SND_IS_AM);
        break;
    }

    *retval = u4TvSoundSys;
    return TM_OK;
}
#endif
int papi_ioctl_fe_sound(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = PAPI_OK;

    switch (cmd)
    {
    case PAPI_IO_FE_SOUND_GETSNDSYS:
        i4Ret = _papi_fe_sound_GetSndSystem(arg);
        break;
    case PAPI_IO_FE_SOUND_H_GETSNDSYS:
        //i4Ret = _papi_fe_sound_H_GetSndSystem(arg);
        i4Ret = _papi_fe_sound_GetSndSystem(arg);
        break;
    case PAPI_IO_FE_SOUND_GET_ALLOW_SNDSYS:
        i4Ret = _papi_fe_sound_GetAllowSndSystem(arg);
        break;
    case PAPI_IO_FE_SOUND_ALLOW_SNDSYS:
        i4Ret = _papi_fe_sound_AllowSndSystem(arg);
        break;    
    default:
        printk("papi_ioctl_fe_sound: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}

