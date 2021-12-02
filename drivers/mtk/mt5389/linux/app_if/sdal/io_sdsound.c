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
 * $RCSfile: io_sdsound.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdtsdata.c
 *  SDAL kernel driver implementation: Sound
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "SdSound.h"
#include "sd_audio.h"
#include "fbm_drvif.h"
#include <linux/sdal_ioctl.h>


//-----------------------------------------------------------------------------
// Configurations
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

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \         
    {                                                       \        
        printk("_AvDec_Dmx_Allocate argument error\n");     \
        return -1;                                          \
    }                                                       \    

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(SDAL_IOCTL_2ARG_T)))              \         
    {                                                       \        
        printk("_AvDec_Dmx_Allocate argument error\n");     \
        return -1;                                          \
    }                                                       \    

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(SDAL_IOCTL_3ARG_T)))              \         
    {                                                       \        
        printk("_AvDec_Dmx_Allocate argument error\n");     \
        return -1;                                          \
    }                                                       \    

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(SDAL_IOCTL_4ARG_T)))              \         
    {                                                       \        
        printk("_AvDec_Dmx_Allocate argument error\n");     \
        return -1;                                          \
    }                                                       \    

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("sdal_ioctl_sdsound argument error\n");      \    
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(SDAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("sdal_ioctl_sdsound argument error\n");      \    
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(SDAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("sdal_ioctl_sdsound argument error\n");      \    
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(SDAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("sdal_ioctl_sdsound argument error\n");      \    
        return -1;                                          \
    }                                                       \
    

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static FBM_POOL_T* _pFbmPool = NULL;
static UINT32      _u4PlayAUIBufStart = 0;
static UINT32      _u4PlayAUIBufSize = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Initialize SdSound module
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_Init(unsigned long arg)                      
{
    INT32             i4Ret;
    SDAL_IOCTL_2ARG_T rArg;
    SdSound_Step_k    eSoundStep;
    SdPowerState_k    ePowerState;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    eSoundStep  = (SdSound_Step_k)rArg.au4Arg[0];
    ePowerState = (SdPowerState_k)rArg.au4Arg[1];

    i4Ret = _SD_AUD_Init(eSoundStep, ePowerState); 
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _SdSound_Connect
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_Connect(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_3ARG_T rArg;
    SdSoundID_k       eSoundId;
    SdSource_k        eSrc;
    SdBackEndInID_k   eInputId;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    
    eSoundId    = (SdSoundID_k)rArg.au4Arg[0];
    eSrc        = (SdSource_k)rArg.au4Arg[1];
    eInputId    = (SdBackEndInID_k)rArg.au4Arg[2];

    i4Ret = _SD_AUD_Connect(eSoundId, eSrc, eInputId);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _SdSound_Disconnect
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_Disconnect(unsigned long arg)
{
    INT32             i4Ret;
    INT32             rArg;
    SdSoundID_k       eSoundId;
    
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, rArg);
    
    eSoundId    = (SdSoundID_k)rArg;
    
    i4Ret = _SD_AUD_Disconnect(eSoundId); 
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** SdSound_SpeakerConnect
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_SpeakerConnect(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_2ARG_T rArg;
    SdSpeakerID_k     eSpeakerId;
    SdSoundID_k       eSoundId;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    eSpeakerId    = (SdSpeakerID_k)rArg.au4Arg[0];
    eSoundId      = (SdSoundID_k)rArg.au4Arg[1];
 
    i4Ret = _SD_AUD_SpeakerConnect(eSpeakerId, eSoundId); 
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** SdSound_SpeakerDisconnect
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_SpeakerDisconnect(unsigned long arg)
{
    INT32             i4Ret;
    INT32             rArg;
    SdSpeakerID_k     eSpeakerId;
    
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, rArg);
    
    eSpeakerId = (SdSpeakerID_k)rArg;
    
    i4Ret = _SD_AUD_SpeakerDisconnect(eSpeakerId); 
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** SdSound_Get
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_Get(unsigned long arg)
{
    INT32                i4Ret;
    SDAL_IOCTL_2ARG_T    rArg;
    SdSoundID_k          eSoundId;
    SdSound_Settings_t   rSettings;
    SdSound_Settings_t * prSettings;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
   
    eSoundId   = (SdSoundID_k)rArg.au4Arg[0]; 
    prSettings = (SdSound_Settings_t *)rArg.au4Arg[1];

    i4Ret = _SD_AUD_GetInfo(eSoundId, &rSettings);
        
    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)prSettings,
                   sizeof(SdSound_Settings_t)))
    {
        printk("_SdSound_Get argument error\n");
        return -1;
    }

    if (copy_to_user((void __user *)prSettings, &rSettings,
                     sizeof(SdSound_Settings_t)))
    {
        printk("_SdSound_Get argument error\n");
        return -1;
    }

    return SD_OK;
}

//-----------------------------------------------------------------------------
/** SdSound_Set
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_Set(unsigned long arg)
{
    INT32                i4Ret;
    SDAL_IOCTL_2ARG_T    rArg;
    SdSoundID_k          eSoundId;
    SdSound_Settings_t   rSettings;
    SdSound_Settings_t * prSettings;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eSoundId      = (SdSoundID_k)rArg.au4Arg[0];
    prSettings    = (SdSound_Settings_t *)rArg.au4Arg[1];

    if (!access_ok(VERIFY_READ, (void __user *)prSettings,
                   sizeof(SdSound_Settings_t)))     
    {                                              
        printk("_SdSound_Set argument error\n");
        return -1;                                     
    }        

    if (copy_from_user(&rSettings, (void __user *)prSettings,
                       sizeof(SdSound_Settings_t)))       
    {                                                    
        printk("_SdSound_Set argument error\n");   
        return -1;                                       
    }                                                    
    
    i4Ret = _SD_AUD_SetInfo(eSoundId, &rSettings); 
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** SdSound_GetStatus
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_GetStatus(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_4ARG_T rArg;
    SdSoundID_k       eSoundId;
    SdMask_t          eSdMask;
    SdSound_Status_t  eStatus;
    SdSound_Status_t* peStatus;
    INT32             i4TimeOut;
    
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    eSoundId       = (SdSoundID_k)rArg.au4Arg[0];
    eSdMask        = (SdMask_t)rArg.au4Arg[1];
    peStatus       = (SdSound_Status_t *)rArg.au4Arg[2];
    i4TimeOut      = (UINT32)rArg.au4Arg[3];

    i4Ret = _SD_AUD_GetStatus(eSoundId, eSdMask, &eStatus, i4TimeOut);

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)peStatus,
                   sizeof(SdSound_Status_t)))
    {
        printk("_SdSound_GetStatus argument error\n");
        return -1;
    }

    if (copy_to_user((void __user *)peStatus, &eStatus,
                     sizeof(SdSound_Status_t)))
    {
        printk("_SdSound_GetStatus argument error\n");
        return -1;
    }
    
    return SD_OK;
}

//-----------------------------------------------------------------------------
/** SdSound_AmpMute
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_AmpMute(unsigned long arg)
{
    INT32                i4Ret;
    SDAL_IOCTL_2ARG_T    rArg;
    SdSpeakerID_k        eSpeakerId;
    BOOL                 fgMuteOn;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    eSpeakerId = (SdSpeakerID_k)rArg.au4Arg[0];
    fgMuteOn   = (BOOL)rArg.au4Arg[1];
    
    i4Ret = _SD_AUD_AmpMute(eSpeakerId, fgMuteOn);    

    return SD_OK;
}

//-----------------------------------------------------------------------------
/** SdSound_SpeakerMute
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_SpeakerMute(unsigned long arg)
{
    INT32                i4Ret;
    SDAL_IOCTL_2ARG_T    rArg;
    SdSpeakerID_k        eSpeakerId;
    BOOL                 fgMuteOn;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    eSpeakerId = (SdSpeakerID_k)rArg.au4Arg[0];
    fgMuteOn   = (BOOL)rArg.au4Arg[1];
    
    i4Ret = _SD_AUD_SpeakerMute(eSpeakerId, fgMuteOn);    

    return SD_OK;
}

//-----------------------------------------------------------------------------
/** SdSound_PlayBeeper
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_PlayBeeper(unsigned long arg)
{
    INT32                i4Ret;
    SDAL_IOCTL_2ARG_T    rArg;
    SdSoundID_k          eSoundId;
    SdSound_MelodyMode_k eMelodyMode;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
   
    eSoundId      = (SdSoundID_k)rArg.au4Arg[0];
    eMelodyMode   = (SdSound_MelodyMode_k)rArg.au4Arg[1];

    i4Ret = _SD_AUD_PlayBeeper(eSoundId, eMelodyMode);

    return SD_OK;
}
//-----------------------------------------------------------------------------
/** SdSound_GetStatus
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_GetDecStatus(unsigned long arg)
{
#if 0
    INT32             i4Ret;
    SDAL_IOCTL_2ARG_T rArg;
    SdSoundID_k               eSoundId;
    SdSound_DecStatus_t* pDecStatus;
    SdSound_DecStatus_t   eDecStatus;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    
    eSoundId = (SdSoundID_k)rArg.au4Arg[0];
    pDecStatus = (SdSound_DecStatus_t*)rArg.au4Arg[1];

    switch(eSoundId)
    {
        case SD_SOUND0:
        default:
        eDecStatus.bAFIFOEmpty = (SdBool_t)_SD_AUD_GetAUIStatus();
        eDecStatus.bAFIFOFull = (SdBool_t)(!_SD_AUD_GetAUIStatus());
        eDecStatus.bAUIDone = _SD_AUD_GetAUIStatus();
        break;
        case SD_SOUND1:
        eDecStatus.bAFIFOEmpty = (SdBool_t)_SD_AUD_GetAUIStatus();   
        eDecStatus.bAFIFOFull = (SdBool_t)(!_SD_AUD_GetAUIStatus());
        eDecStatus.bAUIDone = _SD_AUD_GetAUIStatus();
        break;
    } 

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pDecStatus,
                   sizeof(SdSound_Status_t)))
    {
        printk("_SdSound_GetDecStatus argument error\n");
        return -1;
    }

    if (copy_to_user((void __user *)pDecStatus, &eDecStatus,
                     sizeof(SdSound_Status_t)))
    {
        printk("_SdSound_GetDecStatus argument error\n");
        return -1;
    }
#endif    
    return SD_OK;
}

//-----------------------------------------------------------------------------
/** _SdSound_PlayAUI
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_PlayAUI(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_4ARG_T rArg;
    SdSoundID_k       eSoundId;
    SdAudioFormat_k   eFormat;
    UINT8            *pu1Buffer;
    UINT32            u4Size;
      
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    eSoundId  = (SdSoundID_k)rArg.au4Arg[0];
    eFormat   = (SdAudioFormat_k)rArg.au4Arg[1];
    pu1Buffer   = (UINT8 *)rArg.au4Arg[2];
    u4Size    = (UINT32)rArg.au4Arg[3];

    // Get continuous buffer from FBM
    if (_pFbmPool == NULL)
    {
        _pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
        _u4PlayAUIBufStart = VIRTUAL(_pFbmPool->u4Addr);
        _u4PlayAUIBufSize  = _pFbmPool->u4Size;
    }

    // Check buffer size
    if (u4Size > _u4PlayAUIBufSize)
    {
        printk("_SdSound_PlayAUI : not enough buffer space ! Req = 0x%08x, Available = 0x%08x\n", u4Size, _u4PlayAUIBufSize);
        return (SD_ERR_RESOURCE);
    }
    
    // Check access available
    if (!access_ok(VERIFY_READ, (void __user *)pu1Buffer, u4Size))
    {                                              
        printk("_SdSound_PlayAUI argument error\n");
        return -1;                                     
    }

    // Copy user to fbm buffer
    if (copy_from_user((UINT8 *)_u4PlayAUIBufStart, (void __user *)pu1Buffer, u4Size))    
    {                                                    
        printk("_SdSound_PlayAUI argument error\n");   
        return -1;                                       
    }            
            
    _SD_AUD_PlayAUI(eSoundId, eFormat, (UINT8 *)_u4PlayAUIBufStart, u4Size);
 
    return SD_OK;
}


//-----------------------------------------------------------------------------
/** _SdSound_SetSifType
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_SetSifType(unsigned long arg)
{

    INT32               i4Ret;
    SDAL_IOCTL_3ARG_T   rArg;
    SdSoundID_k         eSoundId;
    SdSource_k          eSrc;
    SdSound_SoundSys_k  eSoundTyp;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    
    eSoundId    = (SdSoundID_k)rArg.au4Arg[0];
    eSoundTyp   = (SdSound_SoundSys_k)rArg.au4Arg[1];

    i4Ret = _SD_AUD_SetSifType(eSoundId, eSoundTyp);

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _SdSound_GetSys
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static int _SdSound_GetSys(unsigned long arg)
{
    INT32             i4Ret;
    SDAL_IOCTL_4ARG_T rArg;
    SdSoundID_k       eSoundId;
    SdSound_SoundSys_k eDefaultSys;
    SdSound_SoundSys_k * peSoundSys;
    SdSound_SoundSys_k  eSoundSys;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    eSoundId  = (SdSoundID_k)rArg.au4Arg[0];
    eDefaultSys   = (SdSound_SoundSys_k)rArg.au4Arg[1];
    peSoundSys   = (SdSound_SoundSys_k *)rArg.au4Arg[2];

    i4Ret = _SD_AUD_GetSoundSys(eSoundId, eDefaultSys, &eSoundSys);

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)peSoundSys,
                   sizeof(SdSound_SoundSys_k)))
    {
        printk("_SdSound_GetSys argument error\n");
        return -1;
    }

    if (copy_to_user((void __user *)peSoundSys, &eSoundSys,
                     sizeof(SdSound_SoundSys_k)))
    {
        printk("_SdSound_GetSys argument error\n");
        return -1;
    }

    return SD_OK;
}

//-----------------------------------------------------------------------------
/** SdSound_Debug
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static void _SdSound_Debug(void)
{
    _SD_AUD_Debug();
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int sdal_ioctl_sdsound(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case SDAL_IO_SOUND_INIT:
        i4Ret = _SdSound_Init(arg);
        break;
    case SDAL_IO_SOUND_CONNECT:
        i4Ret = _SdSound_Connect(arg);
        break;
    case SDAL_IO_SOUND_DISCONNECT:
        i4Ret = _SdSound_Disconnect(arg);
        break;
    case SDAL_IO_SOUND_SPK_CONNECT:
        i4Ret = _SdSound_SpeakerConnect(arg);
        break;
    case SDAL_IO_SOUND_SPK_DISCONNECT:
        i4Ret = _SdSound_SpeakerDisconnect(arg);
        break;
    case SDAL_IO_SOUND_GET:
        i4Ret = _SdSound_Get(arg);
        break;
    case SDAL_IO_SOUND_SET:
        i4Ret = _SdSound_Set(arg);
        break;
    case SDAL_IO_SOUND_GETSTATUS:
        i4Ret = _SdSound_GetStatus(arg);
        break;
    case SDAL_IO_SOUND_AMP_MUTE:
        i4Ret = _SdSound_AmpMute(arg);
        break;
    case SDAL_IO_SOUND_SPK_MUTE:
        i4Ret = _SdSound_SpeakerMute(arg);
        break;
    case SDAL_IO_SOUND_PLAY_BEEPER:
        i4Ret = _SdSound_PlayBeeper(arg);
        break;
    case SDAL_IO_SOUND_GET_DECSTATUS:
        i4Ret = _SdSound_GetDecStatus(arg);
        break;
    case SDAL_IO_SOUND_PLAY_AUI:
        i4Ret = _SdSound_PlayAUI(arg);
        break;
    case SDAL_IO_SOUND_SET_SIF_TYPE:
        i4Ret = _SdSound_SetSifType(arg);
        break;
    case SDAL_IO_SOUND_DEBUG:
        _SdSound_Debug();
        break;
    case SDAL_IO_SOUND_GETSYS:  //wanting
        i4Ret = _SdSound_GetSys(arg);
        break;
    default:
        printk("SDSound: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}



