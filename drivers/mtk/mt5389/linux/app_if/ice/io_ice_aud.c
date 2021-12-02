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
 * $Author: kai.wang $
 * $Date: 2012/08/30 $
 * $RCSfile: io_ice_vga.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_vga.c
 *  ICE VGA driver interface.
 */

#include "mttype.h"
#include "ice.h"
#include "ice_audio.h"
#include "ice_audio_types.h"
#include "aud_drv.h"
#include "aud_if.h"
#include "ice_sound_controls.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#if 0
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_6ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_6ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }


#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#endif
#define COPY_FROM_USER_BUF_SIZE(ArgUsr, BufKernel, size)       \
    if (copy_from_user((void*)BufKernel, (void __user *)ArgUsr,   \
                       (size)))                             \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }      

#define USR_SPACE_ACCESS_VALIDATE_SIZE(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   (size)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }   
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static MT_RESULT_T _ICE_AUDIO_IsAudioStreamTypeSupport(unsigned long arg)
{

  MT_RESULT_T eRet=MTR_NOT_OK;  
  MTAUDDEC_FMT_T eDecType;
  eDecType = (MTAUDDEC_FMT_T)(UINT8)arg;
  
  printf("_ICE_AUDIO_IsAudioStreamTypeSupport audio_stream_type=%d.\n",eDecType);

  if(AUD_IsFmtSupport(eDecType))
  {
    eRet = MTR_OK;
  }
  else
  {
    eRet = MTR_NOT_OK;
  }

  return eRet;
  	
  
}

static MT_RESULT_T _ICE_AUDIO_PlayBeep(unsigned long arg)
{
    MT_RESULT_T eRet=MTR_OK; 
    ICE_IOCTL_4ARG_T rArg;
    UINT8 u1DecId,frequency,percent_volume_level;
    UINT16 duration;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    frequency = (UINT8)rArg.ai4Arg[1] ;
    duration = (UINT16)rArg.ai4Arg[2] ;
    percent_volume_level = (UINT8)rArg.ai4Arg[3] ;

    AUD_PlayBeep(u1DecId,frequency,duration,percent_volume_level);

    printf("_ICE_AUDIO_PlayBeep u1DecId=%d frequency=%d duration=%d percent_volume_level=%d.\n",u1DecId,frequency,duration,percent_volume_level);
    
    

    return eRet;
}

static MT_RESULT_T _ICE_SOUND_GET_EQ_Value(unsigned long arg)
{
    MT_RESULT_T eRet=MTR_OK; 
    ICE_IOCTL_4ARG_T rArg;
    frost_uint source;
    ICE_SoundProfile mode;
    frost_uint8 band;
    frost_int8* pi1EqValue;
    frost_int8 i1EqValue;
    INT8 eqValue[CH_EQ_BAND_NO];
    

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    source = (frost_uint)rArg.ai4Arg[0] ;
    mode = (ICE_SoundProfile)rArg.ai4Arg[1] ;
    band = (frost_uint8)rArg.ai4Arg[2] ;
    pi1EqValue = (frost_int8*)rArg.ai4Arg[3] ;

    AUD_DspChQryEqTable(0,(AUD_EQ_TYPE_T)mode,eqValue);

    if(band>=CH_EQ_BAND_NO)
    {
        band = CH_EQ_BAND_NO-1;
    }
   
    i1EqValue = eqValue[band];

    USR_SPACE_ACCESS_VALIDATE_ARG(pi1EqValue, frost_int8);
    COPY_TO_USER_ARG(pi1EqValue, i1EqValue, frost_int8);

    return eRet;
}

static MT_RESULT_T _ICE_SOUND_Check_Headphone_Cond(unsigned long arg)
{

  MT_RESULT_T eRet=MTR_NOT_OK;  
  frost_uint8 eDecID;
  eDecID = (frost_uint8)arg;

  if(AUD_GetHeadphoneCond((UINT8)eDecID)==AUD_HP_COND_HEADPHONE_COND_PLUGED)
  {
    eRet = TRUE;
  }
  else
  {
    eRet = FALSE;
  }

  return eRet;
  	
  
}

static MT_RESULT_T _ICE_Audio_Get_Channel_Info(unsigned long arg)
{

  MT_RESULT_T eRet=MTR_OK;  
  UINT8 u1DecID;
  ICE_AudioChannelInfo* paudio_channel_info;
  ICE_IOCTL_2ARG_T rArg;
  AUD_DEC_INFO_T  rAudInfo;

  USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
  COPY_FROM_USER_4ARG(arg,rArg);
  u1DecID = (UINT8)rArg.ai4Arg[0];
  paudio_channel_info = (ICE_AudioChannelInfo*) rArg.ai4Arg[1];

  AUD_GetDtvAudInfo(u1DecID,&rAudInfo);
  switch(rAudInfo.e_aud_type)
  {
     case AUD_TYPE_DUAL_MONO:
        *paudio_channel_info = ICE_audio_channel_info_dual_mono;
        break;
     case AUD_TYPE_MONO:
        *paudio_channel_info = ICE_audio_channel_info_single_mono;
        break;
     default :
        *paudio_channel_info = ICE_audio_channel_info_stereo;
        break;
  }


  return eRet;
  	
  
}

static MT_RESULT_T ICE_Audio_Set_Channel_Mode(unsigned long arg)
{

  MT_RESULT_T eRet= MTR_OK;  
  ICE_IOCTL_2ARG_T rArg;
  ICE_AudioChannelMode audio_channel_mode;
  AUD_MATRIX_OUT_T eMatrixMode;  
  UINT8 u1DecId;
  USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
  COPY_FROM_USER_2ARG(arg,rArg);

  u1DecId = (UINT8)rArg.ai4Arg[0] ;
  audio_channel_mode = (ICE_AudioChannelMode)rArg.ai4Arg[1] ;
  
  printf("ICE_Audio_Set_Channel_Mode audio_channel_mode=%d.\n",audio_channel_mode);

  switch(audio_channel_mode)
  {
    case ICE_audio_channel_mode_mono:
        eMatrixMode = AUD_MATRIX_OUT_LR_MIX;
        break;
    case ICE_audio_channel_mode_stereo:
        eMatrixMode = AUD_MATRIX_OUT_OFF;
        break;
    case ICE_audio_channel_mode_dual_mono_left:
        eMatrixMode = AUD_MATRIX_OUT_L_MONO;
        break;
    case ICE_audio_channel_mode_dual_mono_right:
        eMatrixMode = AUD_MATRIX_OUT_R_MONO;
        break;
    default :
        eMatrixMode = AUD_MATRIX_OUT_LR_MIX;
        break;      
  }
  AUD_DspSetMatrixOutMode(u1DecId,eMatrixMode);
  return eRet;
  	
  
}

static MT_RESULT_T _ICE_AUDIO_IsAudioStreamHeAACL2(unsigned long arg) 
{

  MT_RESULT_T eRet=MTR_NOT_OK;  
  
  printf("_ICE_AUDIO_IsAudioStreamHeAACL2.\n");

  if(AUD_IsAACL2())
  {
    eRet = MTR_OK;
  }
  else
  {
    eRet = MTR_NOT_OK;
  }

  return eRet;
  	
  
}


static MT_RESULT_T _ICE_AUDIO_GetAQVersion(unsigned long arg) 
{

  MT_RESULT_T eRet=MTR_OK;    
  ICE_IOCTL_3ARG_T rArg;
  UINT8 u1StartAddr;
  UINT8 u1Size;
  UINT8* pu1Version;
  
  
    
  USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
  COPY_FROM_USER_3ARG(arg,rArg);

  u1StartAddr = (UINT8)rArg.ai4Arg[0];
  u1Size = (UINT8)rArg.ai4Arg[1];
  pu1Version = (UINT8*)rArg.ai4Arg[2];
  AUD_FlashAQReadEx((UINT8)DRVCUST_OptGet(eAudioFlashAQPartition),(UINT16)u1StartAddr,pu1Version,(UINT32)u1Size);
 // printf("_ICE_AUDIO_GetAQVersion pu1Version = %s DRVCUST_OptGet(eAudioFlashAQPartition)=%d u1StartAddr = %d u1Size=%d.\n",(UINT8*)pu1Version,DRVCUST_OptGet(eAudioFlashAQPartition),u1StartAddr,u1Size);
  return eRet;
  	
  
}


static MT_RESULT_T _ICE_AUDIO_SetSRSFLAG(unsigned long arg)
{

  MT_RESULT_T eRet=MTR_NOT_OK;  
  UINT32 fgSRS;
  fgSRS = (UINT32)arg;
  
  printf("_ICE_AUDIO_SetSRSFLAG fgSRS=%d.\n",fgSRS);

  if(AUD_SaveSRSFlag(fgSRS))
  {
    eRet = MTR_OK;
  }
  else
  {
    eRet = MTR_NOT_OK;
  }

  return eRet;
    
  
}

static MT_RESULT_T _ICE_AUDIO_SetMpeg4FLAG(unsigned long arg)
{

  MT_RESULT_T eRet=MTR_NOT_OK;  
  UINT32 fgMpeg4;
  fgMpeg4 = (UINT32)arg;
  
  printf("_ICE_AUDIO_SetMpeg4FLAG fgMpeg4=%d.\n",fgMpeg4);

  if(AUD_SaveMpeg4Flag(fgMpeg4))
  {
    eRet = MTR_OK;
  }
  else
  {
    eRet = MTR_NOT_OK;
  }

  return eRet;
    
  
}






//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_aud(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
   

    switch (cmd)
    {
        case ICE_IO_AUDIO_ISAUDIOSTREMTYPESUPPORTED:
            i4Ret = _ICE_AUDIO_IsAudioStreamTypeSupport(arg);  
            break;
        case ICE_IO_AUDIO_PLAY_BEEP:
            i4Ret = _ICE_AUDIO_PlayBeep(arg);  
            break;  
        case ICE_IO_AUDIO_GETEQVALUE:
            i4Ret = _ICE_SOUND_GET_EQ_Value(arg); 
            break;
        case ICE_IO_AUDIO_CHECKHEADPHONECOND:
            i4Ret = _ICE_SOUND_Check_Headphone_Cond(arg);
            break;
        case ICE_IO_AUDIO_GETAUDIOCHLINFO:
            i4Ret = _ICE_Audio_Get_Channel_Info(arg);
            break;
        case ICE_IO_AUDIO_CHANNEL_MODE:
            i4Ret = ICE_Audio_Set_Channel_Mode(arg);
            break;
        case ICE_IO_AUDIO_ISHEAACL2:
            i4Ret = _ICE_AUDIO_IsAudioStreamHeAACL2(arg);
            break;           
        case ICE_IO_AUDIO_GET_AQ_VERSION:
            i4Ret = _ICE_AUDIO_GetAQVersion(arg);
            break;
        case ICE_IO_AUDIO_SET_SRS_FLAG:
            i4Ret = _ICE_AUDIO_SetSRSFLAG(arg);
            break;
        case ICE_IO_AUDIO_SET_MPEG4_FLAG:
            i4Ret = _ICE_AUDIO_SetMpeg4FLAG(arg);
            break;
        default:
            break;
    }
    return i4Ret;
}
