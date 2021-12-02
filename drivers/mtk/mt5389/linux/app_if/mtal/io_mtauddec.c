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
 * $RCSfile: io_mtauddec.c,v $
 * $Revision: #4 $
 *---------------------------------------------------------------------------*/

/** @file io_mtauddec.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtauddec.h"
#include "mt_audio.h"
#include <linux/mtal_ioctl.h>
#include "x_bim.h"
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
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_5ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_5ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_5ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_5ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }


#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_6ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_6ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }


#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_auddec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
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
#if MIXSOUND_MEMPLAY_USER_SPACE_BUFFER
static UINT8* _pu1MixSoundBuf = NULL;
#endif


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#if MIXSOUND_MEMPLAY_USER_SPACE_BUFFER
UINT8* _MTAUDDEC_GetMixSoundBufAddr(void)
{
    if (_pu1MixSoundBuf)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}
void _MTAUDDEC_ResetMixSoundBufAddr(void)
{
    _pu1MixSoundBuf = NULL;
}
#endif

/**Initialize Auio Decider module.
*@param void
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_Init (void)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAUDDEC_PRINT(" - void\n");
	x_os_drv_set_timestamp("_MTAUD_Init start");

    eRet = _MTAUD_Init();
	x_os_drv_set_timestamp("_MTAUD_Init end");

    return eRet;
}


/**Get audio decoder type and stream source configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peStreamFrom               Stream source.
*@param prDecType                  Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetDecType (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_DEC_STREAM_FROM_T* peStreamFrom;
    MTAUDDEC_FMT_T* prDecType;

    MTAUDDEC_DEC_STREAM_FROM_T eStreamFrom;
    MTAUDDEC_FMT_T rDecType;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peStreamFrom = (MTAUDDEC_DEC_STREAM_FROM_T *)rArg.ai4Arg[1];
    prDecType = (MTAUDDEC_FMT_T *)rArg.ai4Arg[2];

    eRet = _MTAUD_GetDecType(u1DecId, &eStreamFrom, &rDecType);

    USR_SPACE_ACCESS_VALIDATE_ARG(peStreamFrom,MTAUDDEC_DEC_STREAM_FROM_T);
    COPY_TO_USER_ARG(peStreamFrom,eStreamFrom,MTAUDDEC_DEC_STREAM_FROM_T);

    USR_SPACE_ACCESS_VALIDATE_ARG(prDecType,MTAUDDEC_FMT_T);
    COPY_TO_USER_ARG(prDecType,rDecType,MTAUDDEC_FMT_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, eStreamFrom = %d, rDecType = %d\n",
                   u1DecId,
                   eStreamFrom,
                   rDecType);

    return eRet;
}

#if 1
/**Set  LoadAudioClip.
*@param u1DecId                   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1BufferSize             u1BufferSize
*@param eClipBufferPointer     eClipBufferPointer
*@param eDecFmt                   decoder format
*@param eRepeatNumber        eRepeatNumber
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_LoadAudioClip (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_6ARG_T rArg;
    UINT32 u1BufferSize;
    UINT32 eClipBufferPointer;
    MTAUDDEC_FMT_T eDecFmt;
    UINT32 eRepeatNumber;
    UINT8 u1DecId;
    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg,rArg);
    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1BufferSize = (UINT32)rArg.ai4Arg[1];
    eClipBufferPointer=(UINT32 *)rArg.ai4Arg[2];
    eDecFmt = (MTAUDDEC_FMT_T)rArg.ai4Arg[3];
    eRepeatNumber = (UINT32)rArg.ai4Arg[4];
    eRet=_MTAUD_LoadAudioClip(u1DecId, u1BufferSize, eClipBufferPointer,eDecFmt,eRepeatNumber);
    return eRet;
}
#else
/**Set  LoadAudioClip.
*@param u1DecId                   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1BufferSize             u1BufferSize
*@param eClipBufferPointer     eClipBufferPointer
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_LoadAudioClip (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u1BufferSize;
    UINT32 eClipBufferPointer;
    UINT8 u1DecId;
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);
    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1BufferSize = (UINT32)rArg.ai4Arg[1];
    eClipBufferPointer=(UINT32 *)rArg.ai4Arg[2];
    eRet=_MTAUD_LoadAudioClip(u1DecId, u1BufferSize, eClipBufferPointer);
    return eRet;
}
#endif


/**Set  PlayAudioClip.
*@param u1DecId                   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1ReaptNumber        clip reapt number
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_PlayAudioClip(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 u1DecId;
    UINT32 u1ReaptNumber;
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);
    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1ReaptNumber = (UINT32)rArg.ai4Arg[1];
    eRet=_MTAUD_PlayAudioClip(u1DecId, u1ReaptNumber);
    return eRet;
}

/**Set  StopAudioClip.
*@param u1DecId                   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1BufferSize             u1BufferSize
*@param eClipBufferPointer     eClipBufferPointer
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_StopAudioClip (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    UINT8 u1DecId;
    u1DecId = (UINT8)arg;
    MTAUDDEC_PRINT(" - u1DecId = %d\n",
                   u1DecId);

    eRet = _MTAUD_StopAudioClip(u1DecId);

    return eRet;
}

/**Set  DeleteAudioClip.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_DeleteAudioClip (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAUDDEC_PRINT(" - _MTAUDDEC_DeleteAudioClip\n");
    eRet = _MTAUD_DeleteAudioClip();

    return eRet;
}

/**Set audio decoder type and stream source.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eStreamFrom                Stream source.
*@param eDecFmt                    Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetDecType (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_DEC_STREAM_FROM_T eStreamFrom;
    MTAUDDEC_FMT_T eDecFmt;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eStreamFrom = (MTAUDDEC_DEC_STREAM_FROM_T)rArg.ai4Arg[1];
    eDecFmt = (MTAUDDEC_FMT_T)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eStreamFrom = %d, eDecFmt = %d\n",
                   u1DecId,
                   eStreamFrom,
                   eDecFmt);

    eRet = _MTAUD_SetDecType(u1DecId, eStreamFrom, eDecFmt);

    return eRet;
}


/**Audio decoder playback.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_Play(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    UINT8 u1DecId;

    u1DecId = (UINT8)arg;

    MTAUDDEC_PRINT(" - u1DecId = %d\n",
                   u1DecId);

    eRet = _MTAUD_Play(u1DecId);

    return eRet;
}


/**Resume audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_Resume(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    UINT8 u1DecId;

    u1DecId = (UINT8)arg;

    MTAUDDEC_PRINT(" - u1DecId = %d\n",
                   u1DecId);

    eRet = _MTAUD_Resume(u1DecId);

    return eRet;
}


/**Pause audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_Pause (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    UINT8 u1DecId;

    u1DecId = (UINT8)arg;

    MTAUDDEC_PRINT(" - u1DecId = %d\n",
                   u1DecId);

    eRet = _MTAUD_Pause(u1DecId);

    return eRet;
}


/**Stop audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_Stop (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    UINT8 u1DecId;

    u1DecId = (UINT8)arg;

    MTAUDDEC_PRINT(" - u1DecId = %d\n",
                   u1DecId);

    eRet = _MTAUD_Stop(u1DecId);

    return eRet;
}



/**Audio decoder call back function register.
*@param eFuncType                  The call back function type.
*@param u4FuncPtr                  The call back function pointer.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_RegCb(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    MTAUDDEC_CB_FUNC_ID_T eFuncType;
    UINT32 u4FuncPtr;
    UINT32 u4Arg1;
    UINT32 u4Arg2;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    eFuncType = (MTAUDDEC_CB_FUNC_ID_T)rArg.ai4Arg[0];
    u4FuncPtr = (UINT32)rArg.ai4Arg[1];
    u4Arg1 = (UINT32)rArg.ai4Arg[2];
    u4Arg2 = (UINT32)rArg.ai4Arg[3];

    MTAUDDEC_PRINT(" - eFuncType = %d, u4FuncPtr = %ul, u4Arg1 = %ul, u4Arg2 = %ul\n",
                   eFuncType, u4FuncPtr, u4Arg1, u4Arg2);

    _MTAUD_RegCb(eFuncType, u4FuncPtr, u4Arg1, u4Arg2);

    return MTR_OK;
}


/**Get the information of audio decoder.
*  Since in some case, even application issue play to decoder, but if the
*  bitstream is not feed to decoder, there is no audio output yet.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pfgPlay                    Is decoder real play
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetDecOnPlayingStatus (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL* pfgPlay;

    BOOL fgPlay;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    pfgPlay = (BOOL*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetDecOnPlayingStatus(u1DecId, &fgPlay);

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgPlay,BOOL);
    COPY_TO_USER_ARG(pfgPlay,fgPlay,BOOL);

    MTAUDDEC_PRINT(" - u1DecId = %d, fgPlay = %d\n",
                   u1DecId,
                   fgPlay);

    return eRet;
}

/**Get the information of audio decoder pts.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pePts                        pts
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/

static MT_RESULT_T _MTAUDDEC_GetAudioPTS(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT32 *pePts;
    UINT32 ePts;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    pePts = (UINT32*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetAudioPTS(u1DecId, &ePts);

    USR_SPACE_ACCESS_VALIDATE_ARG(pePts,UINT32);
    COPY_TO_USER_ARG(pePts,ePts,UINT32);

    MTAUDDEC_PRINT(" - u1DecId = %d, ePts = %d\n",
                   u1DecId,
                   ePts);

    return eRet;
}

/**Get the information of audio ac3 es.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peAC3ESInfo             ac3 ES info
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/

static MT_RESULT_T _MTAUDDEC_GetAC3ESInfo(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    MTAUDDEC_AC3_ES_INFO_T *peAC3ESInfo;
    MTAUDDEC_AC3_ES_INFO_T eAC3ESInfo;
    UINT8 u1DecId;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peAC3ESInfo = (MTAUDDEC_AC3_ES_INFO_T *)rArg.ai4Arg[1];

    eRet = _MTAUD_GetAC3ESInfo(u1DecId, &eAC3ESInfo);

    USR_SPACE_ACCESS_VALIDATE_ARG(peAC3ESInfo,MTAUDDEC_AC3_ES_INFO_T);
    COPY_TO_USER_ARG(peAC3ESInfo,eAC3ESInfo,MTAUDDEC_AC3_ES_INFO_T);
    return eRet;
}


/**Get the information of audio mpeg es.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peMPEGESInfo           MPEG ES info
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_GetMPEGESInfo(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    MTAUDDEC_MPEG_ES_INFO_T *peMPEGESInfo;
    MTAUDDEC_MPEG_ES_INFO_T eMPEGESInfo;
    UINT8 u1DecId;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peMPEGESInfo = (MTAUDDEC_MPEG_ES_INFO_T *)rArg.ai4Arg[1];

    eRet = _MTAUD_GetMPEGESInfo(u1DecId, &eMPEGESInfo);

    USR_SPACE_ACCESS_VALIDATE_ARG(peMPEGESInfo,MTAUDDEC_MPEG_ES_INFO_T);
    COPY_TO_USER_ARG(peMPEGESInfo,eMPEGESInfo,MTAUDDEC_MPEG_ES_INFO_T);
    return eRet;
}

/**Get the information of audio HEAAC es.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peHEAACESInfo         HEAAC ES info
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_GetHEAACESInfo(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    MTAUDDEC_HEAAC_ES_INFO_T *peHEAACESInfo;
    MTAUDDEC_HEAAC_ES_INFO_T eHEAACESInfo;
    UINT8 u1DecId;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peHEAACESInfo = (MTAUDDEC_HEAAC_ES_INFO_T *)rArg.ai4Arg[1];

    eRet = _MTAUD_GetHEAACESInfo(u1DecId, &eHEAACESInfo);

    USR_SPACE_ACCESS_VALIDATE_ARG(peHEAACESInfo,MTAUDDEC_HEAAC_ES_INFO_T);
    COPY_TO_USER_ARG(peHEAACESInfo,eHEAACESInfo,MTAUDDEC_HEAAC_ES_INFO_T);
    return eRet;
}

/**Set AV synchronization mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSynMode                   AV sync mode or just free run.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAvSynMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAV_SYNC_MODE_T eSynMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eSynMode = (MTAV_SYNC_MODE_T)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, eSynMode = %d\n",
                   u1DecId,
                   eSynMode);

    eRet = _MTAUD_SetAvSynMode(u1DecId, eSynMode);

    return eRet;
}

/**Setup AC3 compression mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eCompMode                  Compression mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAc3CompMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_AC3_COMP_MODE_T eCompMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eCompMode = (MTAUDDEC_AC3_COMP_MODE_T)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, eCompMode = %d\n",
                   u1DecId,
                   eCompMode);

    eRet = _MTAUD_SetAc3CompMode(u1DecId, eCompMode);

    return eRet;
}

/**Setup AC3 downmix mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDownmixMode               Downmix mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAc3DownmixMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_AC3_DOWNMIX_MODE eDownmixMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eDownmixMode = (MTAUDDEC_AC3_DOWNMIX_MODE)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, eDownmixMode = %d\n",
                   u1DecId,
                   eDownmixMode);

    eRet = _MTAUD_SetAc3DownmixMode(u1DecId, eDownmixMode);

    return eRet;
}

/**Get AC3 downmix mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDownmixMode               Downmix mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetAc3DownmixMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_AC3_DOWNMIX_MODE* peDownmixMode;
    MTAUDDEC_AC3_DOWNMIX_MODE  eDownmixMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peDownmixMode = (MTAUDDEC_AC3_DOWNMIX_MODE*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetAc3DownmixMode(u1DecId, &eDownmixMode);

    USR_SPACE_ACCESS_VALIDATE_ARG(peDownmixMode, MTAUDDEC_MODE_T);
    COPY_TO_USER_ARG(peDownmixMode, eDownmixMode, MTAUDDEC_MODE_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, eDownmixMode = %d\n",
                   u1DecId,
                   eDownmixMode);

    return eRet;
}

/**Get AC3 audio mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peMode                     AC3 channel information.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetAc3Mode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_MODE_T* peMode;
    MTAUDDEC_MODE_T  eMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peMode = (MTAUDDEC_MODE_T*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetAc3Mode(u1DecId, &eMode);

    USR_SPACE_ACCESS_VALIDATE_ARG(peMode, MTAUDDEC_MODE_T);
    COPY_TO_USER_ARG(peMode, eMode, MTAUDDEC_MODE_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, eMode = %d\n",
                   u1DecId,
                   eMode);

    return eRet;
}


/**Get audio sampling rate.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pSmpRate                   Sampling rate.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetSamplingRate (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 u1DecId;
    MTAUDDEC_SAMPLE_FREQ_T* peSmpRate;
    MTAUDDEC_SAMPLE_FREQ_T  eSmpRate;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peSmpRate = (MTAUDDEC_SAMPLE_FREQ_T*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetSamplingRate(u1DecId, &eSmpRate);

    USR_SPACE_ACCESS_VALIDATE_ARG(peSmpRate, MTAUDDEC_SAMPLE_FREQ_T);
    COPY_TO_USER_ARG(peSmpRate, eSmpRate, MTAUDDEC_SAMPLE_FREQ_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, eSmpRate = %d\n",
                   u1DecId,
                   eSmpRate);

    return eRet;
}


/**Setup range for dynamix range compression.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1DrcLevel                 DRC range.
*                                  0 ~ 8 (step size is 0.125)
*                                  0 -> 0.0
*                                  1 -> 0.125
*                                  8 -> 1.0
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAc3DrcRange (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1DrcLevel;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1DrcLevel= (UINT8)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1DrcLevel = %d\n",
                   u1DecId,
                   u1DrcLevel);

    eRet = _MTAUD_SetAc3DrcRange(u1DecId, u1DrcLevel);

    return eRet;
}

/**Setup 0~100 range for dynamix range compression.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1DrcLevel                 DRC range 0~100.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAc3DrcRange100 (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1DrcLevel;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1DrcLevel= (UINT8)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1DrcLevel = %d\n",
                   u1DecId,
                   u1DrcLevel);

    eRet = _MTAUD_SetAc3DrcRange100(u1DecId, u1DrcLevel);

    return eRet;
}

/*
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDualMonoMode              Dual mono mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAc3DualMono (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_AC3_DUALMONO_MODE eDualMonoMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eDualMonoMode= (MTAUDDEC_AC3_DUALMONO_MODE)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1DrcLevel = %d\n",
                   u1DecId,
                   eDualMonoMode);

    eRet = _MTAUD_SetAc3DualMono(u1DecId, eDualMonoMode);

    return eRet;
}


/**Setup AC3 dual mono mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDualMonoMode              Dual mono mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetAc3DualMono (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_AC3_DUALMONO_MODE* peDualMonoMode;
    MTAUDDEC_AC3_DUALMONO_MODE  eDualMonoMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peDualMonoMode = (MTAUDDEC_AC3_DUALMONO_MODE*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetAc3DualMono(u1DecId, &eDualMonoMode);

    USR_SPACE_ACCESS_VALIDATE_ARG(peDualMonoMode,MTAUDDEC_ATV_DETECT_MODE_T);
    COPY_TO_USER_ARG(peDualMonoMode,eDualMonoMode,MTAUDDEC_ATV_DETECT_MODE_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, eDualMonoMode = %d\n",
                   u1DecId,
                   eDualMonoMode);

    return eRet;
}

/**Get the analogy TV stereo mode of current channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peATVMode                  Detected mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetAtvDetectMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_ATV_DETECT_MODE_T* peATVMode;

    MTAUDDEC_ATV_DETECT_MODE_T	eATVMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peATVMode = (MTAUDDEC_ATV_DETECT_MODE_T*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetAtvDetectMode(u1DecId, &eATVMode);

    USR_SPACE_ACCESS_VALIDATE_ARG(peATVMode,MTAUDDEC_ATV_DETECT_MODE_T);
    COPY_TO_USER_ARG(peATVMode,eATVMode,MTAUDDEC_ATV_DETECT_MODE_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, eATVMode = %d\n",
                   u1DecId,
                   eATVMode);

    return eRet;
}

/**Get the analogy TV sound system of current channel.
*@param peATVMode                  Detected mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetAtvSoundSys (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_SIF_STANDARD_T* peATVSoundSys;

    MTAUDDEC_SIF_STANDARD_T	eATVSoundSys;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    peATVSoundSys = (MTAUDDEC_SIF_STANDARD_T*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetAtvSoundSys(u1DecId, &eATVSoundSys);

    USR_SPACE_ACCESS_VALIDATE_ARG(peATVSoundSys,MTAUDDEC_SIF_STANDARD_T);
    COPY_TO_USER_ARG(peATVSoundSys,eATVSoundSys,MTAUDDEC_SIF_STANDARD_T);


    MTAUDDEC_PRINT(" - eATVSoundSys = %d\n",
                   eATVSoundSys);

    return eRet;
}

/**Get the analogy TV sound system strength.
*@param peATVMode                  Detected mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetAtvSoundSysStrength (unsigned long arg)
{

    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT32* peATVSoundStrength;
    UINT32 eATVSoundStrength;
    TV_AUD_SYS_T eAudSys;
    MTAUDDEC_SIF_STANDARD_T	eATVSoundSys;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eATVSoundSys = (MTAUDDEC_SIF_STANDARD_T)rArg.ai4Arg[0];
    peATVSoundStrength = (UINT32*)rArg.ai4Arg[1];
    switch (eATVSoundSys)
    {
    case MTAUDDEC_SIF_BTSC:
        eAudSys = SV_NTSC_M;
        break;
    case MTAUDDEC_SIF_BG:
        eAudSys = SV_PAL_BG;
        break;
    case MTAUDDEC_SIF_I:
        eAudSys = SV_PAL_I;
        break;
    case MTAUDDEC_SIF_DK:
        eAudSys = SV_PAL_DK;
        break;
    case MTAUDDEC_SIF_L:
        eAudSys = SV_SECAM_L;
        break;
    default:
        eAudSys = SV_PAL_BG;
        break;
    }
    eATVSoundStrength = AUD_DspGetAtvTvSystemStrength(eAudSys);

    USR_SPACE_ACCESS_VALIDATE_ARG(peATVSoundStrength,UINT32);
    COPY_TO_USER_ARG(peATVSoundStrength,eATVSoundStrength,UINT32);


    MTAUDDEC_PRINT(" - eATVSoundStrength = %d\n",
                   eATVSoundStrength);

    return eRet;
}

/**Setup analog TV output mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSoundMode                 UNKNOWN = 0,
*                                  MONO,
*                                  STEREO,
*                                  SUB_LANG,
*                                  DUAL1,
*                                  DUAL2,
*                                  NICAM_MONO,
*                                  NICAM_STEREO,
*                                  NICAM_DUAL1,
*                                  NICAM_DUAL2,
*                                  A2_DUAL1,
*                                  A2_DUAL2,
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAtvOutputMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_SOUND_MODE_T eSoundMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eSoundMode = (MTAUDDEC_SOUND_MODE_T)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, eSoundMode = %d\n",
                   u1DecId,
                   eSoundMode);

    eRet = _MTAUD_SetAtvOutputMode(u1DecId, eSoundMode);

    return eRet;
}

/**Setup SIF input mode
* Should set to audio decoder before issue playback at EU/KOREA/EIAJ model.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSIFMode                   MODE_DETECT = 0,
*                                  NICAM BG,
*                                  FM BG,
*                                  A2 BG,
*                                  NICAM I,
*                                  FM I,
*                                  NICAM DK,
*                                  FM DK,
*                                  A2 DK1,
*                                  A2 DK3,
*                                  NICAM L,
*                                  AM L,
*                                  A2 KOREA
*                                  BTSC
*                                  EIAJ
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetSIFMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_SIF_STANDARD_T eSIFMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eSIFMode = (MTAUDDEC_SIF_STANDARD_T)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, eSIFMode = %d\n",
                   u1DecId,
                   eSIFMode);

    eRet = _MTAUD_SetSIFMode(u1DecId, eSIFMode);

    return eRet;
}

/**Get detected SIF mode
* Issue decoder to do SIF mode detection. It may take some time for the detection.
* Notes that application should stop decoder before invoking this function.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSIFMode                   FM DK or AM L
*                                  Since audio decoder cannot distinguish from
*                                  FM DK and AM L, so application should set this
*                                  information to audio decoder at first before
*                                  issue decoder to do detection.
*@param peSIFMode                  Detection result
*                                  MODE_DETECT = 0,
*                                  NICAM BG,
*                                  FM BG,
*                                  A2 BG,
*                                  NICAM I,
*                                  FM I,
*                                  NICAM DK,
*                                  FM DK,
*                                  A2 DK1,
*                                  A2 DK3,
*                                  NICAM L,
*                                  AM L,
*                                  A2 KOREA
*                                  BTSC
*                                  EIAJ
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/

static MT_RESULT_T  _MTAUDDEC_GetDetectSIFMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_SIF_STANDARD_T eSIFMode;
    MTAUDDEC_SIF_STANDARD_T* peSIFMode;

    MTAUDDEC_SIF_STANDARD_T eSIFModeResult;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eSIFMode =  (MTAUDDEC_SIF_STANDARD_T)rArg.ai4Arg[1];
    peSIFMode = (MTAUDDEC_SIF_STANDARD_T*)rArg.ai4Arg[2];

    eRet = _MTAUD_GetDetectSIFMode(u1DecId, eSIFMode, &eSIFModeResult);

    USR_SPACE_ACCESS_VALIDATE_ARG(peSIFMode, MTAUDDEC_SIF_STANDARD_T);
    COPY_TO_USER_ARG(peSIFMode, eSIFModeResult, MTAUDDEC_SIF_STANDARD_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, eSIFMode = %d, eSIFModeResult = %d\n",
                   u1DecId,
                   eSIFMode,
                   eSIFModeResult);

    return eRet;
}

static MT_RESULT_T  _MTAUDDEC_SetPcmInfo (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_PCM_SETTING_T* prPcmSetting;

    MTAUDDEC_PCM_SETTING_T rPcmSetting;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    prPcmSetting = (MTAUDDEC_PCM_SETTING_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prPcmSetting, MTAUDDEC_PCM_SETTING_T);
    COPY_FROM_USER_ARG(prPcmSetting, rPcmSetting, MTAUDDEC_PCM_SETTING_T);

    MTAUDDEC_PRINT(" - u1DecId = %d\n"
                   "rPcmSetting.ePcmDataInfo = %d\n"
                   "rPcmSetting.eSampleFreq = %d\n",
                   u1DecId,
                   rPcmSetting.ePcmDataInfo,
                   rPcmSetting.eSampleFreq);

    eRet = _MTAUD_SetPcmInfo(u1DecId, prPcmSetting);

    return eRet;
}

/**Setup individual channel delay
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u2Delay                    Delay (0~300 ms with 1 ms/step).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetChannelDelay (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_CH_T eChIndex;
    UINT16 u2Delay;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eChIndex = (MTAUDDEC_CH_T)rArg.ai4Arg[1];
    u2Delay = (UINT16)rArg.ai4Arg[2] ;

    MTAUDDEC_PRINT(" - u1DecId = %d, eChIndex = %d, u2Delay = %d\n",
                   u1DecId,
                   eChIndex,
                   u2Delay);

    eRet = _MTAUD_SetChannelDelay(u1DecId, eChIndex, u2Delay);

    return eRet;
}

/**Setup individual channel delay no uop
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u2Delay                    Delay (0~300 ms with 1 ms/step).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetChannelDelayNoUop (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_CH_T eChIndex;
    UINT16 u2Delay;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eChIndex = (MTAUDDEC_CH_T)rArg.ai4Arg[1];
    u2Delay = (UINT16)rArg.ai4Arg[2] ;

    MTAUDDEC_PRINT(" -channel delay no uop u1DecId = %d, eChIndex = %d, u2Delay = %d\n",
                   u1DecId,
                   eChIndex,
                   u2Delay);

    eRet = _MTAUD_SetChannelDelayNoUop(u1DecId, eChIndex, u2Delay);

    return eRet;
}

/**Setup individual channel volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u1Value                    0~100.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetChannelVolume (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_CH_T eChannel;
    UINT8 u1Value;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eChannel = (MTAUDDEC_CH_T)rArg.ai4Arg[1];
    u1Value = (UINT8)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eChannel = %d, u1Value = %d\n",
                   u1DecId,
                   eChannel,
                   u1Value);

    eRet = _MTAUD_SetChannelVolume(u1DecId, eChannel, u1Value);

    return eRet;
}

/**Setup source volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param etreamfrom
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetSourceVolume (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    MTAUDDEC_DEC_STREAM_FROM_T eStreamFrom;
    UINT8 u1DecId;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);
    u1DecId = (UINT8)rArg.ai4Arg[0];
    eStreamFrom = (MTAUDDEC_DEC_STREAM_FROM_T)rArg.ai4Arg[1];
    MTAUDDEC_PRINT(" - u1DecId = %d, eStreamFrom = %d\n",
                   u1DecId,
                   eStreamFrom);
    eRet = _MTAUD_SetSourceVolume(u1DecId);
    return eRet;
}

/**Setup audio description mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Mode                     0x02 0x08 0xa
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetADChannelMode(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 u1DecId;
    UINT16 u1Mode;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);
    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1Mode = (UINT16)rArg.ai4Arg[1];
    MTAUDDEC_PRINT(" - u1DecId = %d, AD Mode = %d\n",
                   u1DecId,
                   u1Mode);
    eRet = _MTAUD_SetADMode(u1DecId, u1Mode);
    return eRet;
}

/**Setup audio description enable.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Enable                   enable/disable
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetADEnable(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 u1DecId;
    BOOL u1Enable;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);
    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1Enable = (BOOL)rArg.ai4Arg[1];
    MTAUDDEC_PRINT(" - u1DecId = %d, AD Enable = %d\n",
                   u1DecId,
                   u1Enable);
    eRet = _MTAUD_SetADEnable(u1DecId, u1Enable);
    return eRet;
}

/**Mute individual channel
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChannel                   Individual channel.
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetChannelMute (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_CH_T eChannel;
    BOOL fgMute;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eChannel = (MTAUDDEC_CH_T)rArg.ai4Arg[1];
    fgMute = (BOOL)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eChannel = %d, fgMute = %d\n",
                   u1DecId,
                   eChannel,
                   fgMute);

    eRet = _MTAUD_SetChannelMute(u1DecId, eChannel, fgMute);

    return eRet;
}

/**Setup extra gain for individual channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChannel                   Individual channel.
*@param i4Value                    -256~96. (-128 ~ 48 dB with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetChannelVolGain (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_CH_T eChannel;
    INT32 i4Value;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eChannel = (MTAUDDEC_CH_T)rArg.ai4Arg[1];
    i4Value = (INT32)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eChannel = %x, i4Value = %d\n",
                   u1DecId,
                   eChannel,
                   i4Value);

    eRet = _MTAUD_SetChannelVolGain(u1DecId, eChannel, i4Value);

    return eRet;
}

/**Setup volume balance.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Balance                  Balance value (0~100).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetBalance (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1Balance;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId= (UINT8)rArg.ai4Arg[0] ;
    u1Balance = (UINT8)rArg.ai4Arg[1];

    printf(" ==> - u1DecId = %d, u1Balance = %d\n",
                   u1DecId,
                   u1Balance);
    MTAUDDEC_PRINT(" - u1DecId = %d, u1Balance = %d\n",
                   u1DecId,
                   u1Balance);

    eRet = _MTAUD_SetBalance(u1DecId, u1Balance);

    return eRet;
}


/**Setup master volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Volume                   Volume value (0~100).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetMasterVolume (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1Volume;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    u1Volume= (UINT8)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1Volume = %d\n",
                   u1DecId,
                   u1Volume);

    eRet = _MTAUD_SetMasterVolume(u1DecId, u1Volume);

    return eRet;
}

/**Setup Auto volume control (AVC).
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAvc(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetAvc(u1DecId, fgEnable);

    return eRet;
}

/**Setup 2 band AVC parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Band                     Band number (0: Low band, 1: High band)
*@param eAVCCfgType                Parameter type
*@param i4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_Set2BandAvcCfg(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_4ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1Band;
    MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType;
    INT32 i4Parameter;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1Band  = (UINT8)rArg.ai4Arg[1];
    eAVCCfgType = (MTAUDDEC_AVC_CFG_TYPE_T)rArg.ai4Arg[2];
    i4Parameter = (INT32)rArg.ai4Arg[3];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1Band = %d, eAVCCfgType = %d, i4Parameter = %d\n",
                   u1DecId,
                   u1Band,
                   eAVCCfgType,
                   i4Parameter);

    eRet = _MTAUD_Set2BandAvcCfg(u1DecId, u1Band, eAVCCfgType, i4Parameter);

    return eRet;
}



/**Setup BBE
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetBBE(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetBBE(u1DecId, fgEnable);

    return eRet;
}


/**Setup SRS
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetSRS(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetSRS(u1DecId, fgEnable);

    return eRet;
}



/**Setup virtural surround
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetVirtualSurround(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetVirtualSurround(u1DecId, fgEnable);

    return eRet;
}

/**Setup bass boost.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetBassBoost (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetBassBoost(u1DecId, fgEnable);

    return eRet;
}

/**Setup bass
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Bass                     bass value  ( +-14 steps, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetBass (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    INT16 i2Bass;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    i2Bass = (INT16)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, i2Bass = %d\n",
                   u1DecId,
                   i2Bass);

    eRet = _MTAUD_SetBass(u1DecId, i2Bass);

    return eRet;
}


/**Setup treble boost.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u2CompMode                 Compression mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetTrebleBoost(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetTrebleBoost(u1DecId, fgEnable);

    return eRet;
}

/**Setup Treble
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Treble                   Treble value  ( +-14 steps, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetTreble (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    INT16 i2Treble;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    i2Treble = (INT16)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, i2Treble = %d\n",
                   u1DecId,
                   i2Treble);

    eRet = _MTAUD_SetTreble(u1DecId, i2Treble);

    return eRet;
}

/**Setup EQ.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetEqualizer(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetEqualizer(u1DecId, fgEnable);

    return eRet;
}

/**Setup EQ band configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@param i2Band0                    band 0 setup ( +-28, with 0.5 dB/step)
*@param i2Band1                    band 1 setup ( +-28, with 0.5 dB/step)
*@param i2Band2                    band 2 setup ( +-28, with 0.5 dB/step)
*@param i2Band3                    band 3 setup ( +-28, with 0.5 dB/step)
*@param i2Band4                    band 4 setup ( +-28, with 0.5 dB/step)
*@param i2Band5                    band 5 setup ( +-28, with 0.5 dB/step)
*@param i2Band6                    band 6 setup ( +-28, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetEqualizerBandConfig(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_8ARG_T rArg;

    UINT8 u1DecId;
    INT16 i2Band0;
    INT16 i2Band1;
    INT16 i2Band2;
    INT16 i2Band3;
    INT16 i2Band4;
    INT16 i2Band5;
    INT16 i2Band6;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    i2Band0 = (INT16)rArg.ai4Arg[1];
    i2Band1 = (INT16)rArg.ai4Arg[2] ;
    i2Band2 = (INT16)rArg.ai4Arg[3];
    i2Band3 = (INT16)rArg.ai4Arg[4];
    i2Band4 = (INT16)rArg.ai4Arg[5];
    i2Band5 = (INT16)rArg.ai4Arg[6];
    i2Band6 = (INT16)rArg.ai4Arg[7];

    MTAUDDEC_PRINT(" - u1DecId = %d\n"
                   "i2Band0 = %d\n"
                   "i2Band1 = %d\n"
                   "i2Band2 = %d\n"
                   "i2Band3 = %d\n"
                   "i2Band4 = %d\n"
                   "i2Band5 = %d\n"
                   "i2Band6 = %d\n",
                   u1DecId,
                   i2Band0,
                   i2Band1,
                   i2Band2,
                   i2Band3,
                   i2Band4,
                   i2Band5,
                   i2Band6);

    eRet = _MTAUD_SetEqualizerBandConfig(u1DecId, i2Band0, i2Band1, i2Band2,
                                         i2Band3, i2Band4, i2Band5, i2Band6);

    return eRet;
}

/**Setup line-in input mux
*@param  eInputId                  Input mux id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAvInputMuxSel (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAUDDEC_INPUT_ID_T eInputId;

    eInputId = (MTAUDDEC_INPUT_ID_T)arg;

    MTAUDDEC_PRINT(" - eInputId = %d\n",
                   eInputId);

    eRet = _MTAUD_SetAvInputMuxSel(eInputId);

    return eRet;
}

/**Setup line-in input mux by AD
*@param eADNum              Input mux id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetAvInputMuxSelByAD(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    UINT8 eADNum;
    eADNum = (UINT8)arg;

    MTAUDDEC_PRINT(" - eADNum = %d\n",
                   eADNum);
    printk("sunman 1 _MTAUDDEC_SetAvInputMuxSelByAD eADNum  = %d",eADNum);
    eRet = _MTAUD_SetAvInputMuxSelByAD(eADNum);

    return eRet;
}

/**Setup DSP dual mode
*@param  fgEnable                  Dual mode enable true/false.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetDualMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    MTAUDDEC_PRINT(" - fgEnable = %d\n",
                   fgEnable);

    eRet = _MTAUD_SetDualMode(fgEnable);

    return eRet;
}

/**Setup High Deveation mode
*@param  fgEnable                  High Deveation mode enable true/false.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetHDevMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    MTAUDDEC_PRINT(" - fgEnable = %d\n",
                   fgEnable);

    eRet = _MTAUD_SetHDevMode(fgEnable);

    return eRet;
}

/**Setup IEC configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eIecCfg                    IEC format.
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetIecConfig (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_IEC_T eIecCfg;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eIecCfg = (MTAUDDEC_IEC_T)rArg.ai4Arg[1] ;
    fgEnable = (BOOL)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eIecCfg = %d, fgEnable = %d\n",
                   u1DecId,
                   eIecCfg,
                   fgEnable);

    eRet = _MTAUD_SetIecConfig(u1DecId, eIecCfg, fgEnable);

    return eRet;
}

/**Setup IEC refer to which channel.
*@param eIecChannel                Channel number.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUDDEC_SetIecChannel (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAUDDEC_PCM_CH_T eCh;

    eCh = (UINT8)arg;

    MTAUDDEC_PRINT(" - eChannel = %d\n",
                   eCh);

    eRet = _MTAUD_SetIecChannel(eCh);

    return eRet;
}

/**Setup IEC copyright.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetIecCopyright(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetIecCopyright(u1DecId, fgEnable);

    return eRet;
}

/**Setup spdif scms.
*@param u1DecId                  Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param spdif_ctrl                  spdif ctrl
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetSpdifSCMS(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8 spdif_ctrl;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    spdif_ctrl = (UINT8)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, spdif_ctrl = %d\n",
                   u1DecId,
                   spdif_ctrl);

    eRet = _MTAUD_SetSpdifSCMS(spdif_ctrl);

    return eRet;
}

/**Get IEC copyright.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pfgEnable                  on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetIecCopyright(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8* pfgEnable;
    UINT8  fgEnable=0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    pfgEnable = (UINT8*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetIecCopyright(u1DecId, &fgEnable);

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgEnable,MTAUDDEC_ATV_DETECT_MODE_T);
    COPY_TO_USER_ARG(pfgEnable,fgEnable,MTAUDDEC_ATV_DETECT_MODE_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    return eRet;
}

/**Setup IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1CategoryCode             Category Code.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetIecCategoryCode(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1CategoryCode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    u1CategoryCode = (UINT8)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1CategoryCode = %d\n",
                   u1DecId,
                   u1CategoryCode);

    eRet = _MTAUD_SetIecCategoryCode(u1DecId, u1CategoryCode);

    return eRet;
}

/**Get IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pu1CategoryCode            Category code.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetIecCategoryCode(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8* pu1CategoryCode;
    UINT8  u1CategoryCode=0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    pu1CategoryCode = (UINT8*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetIecCategoryCode(u1DecId, &u1CategoryCode);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1CategoryCode,MTAUDDEC_ATV_DETECT_MODE_T);
    COPY_TO_USER_ARG(pu1CategoryCode,u1CategoryCode,MTAUDDEC_ATV_DETECT_MODE_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, u1CategoryCode = %d\n",
                   u1DecId,
                   u1CategoryCode);

    return eRet;
}

/**Setup IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1WordLength               Word length.(in bits, range: 16~24)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetIecWordLength(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1WordLength;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    u1WordLength = (UINT8)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1WordLength = %d\n",
                   u1DecId,
                   u1WordLength);

    eRet = _MTAUD_SetIecWordLength(u1DecId, u1WordLength);

    return eRet;
}

/**Get IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pu1WordLength              Word length.(in bits, range: 16~24)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_GetIecWordLength(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT8* pu1WordLength;
    UINT8  u1WordLength=0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    pu1WordLength = (UINT8*)rArg.ai4Arg[1];

    eRet = _MTAUD_GetIecWordLength(u1DecId, &u1WordLength);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1WordLength,MTAUDDEC_ATV_DETECT_MODE_T);
    COPY_TO_USER_ARG(pu1WordLength,u1WordLength,MTAUDDEC_ATV_DETECT_MODE_T);

    MTAUDDEC_PRINT(" - u1DecId = %d, u1WordLength = %d\n",
                   u1DecId,
                   u1WordLength);

    return eRet;
}

/**Setup Speaker output mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eOutputMode    L+R = 0,
*                                  L+L,
*                                  R+R,
*                                  MIX
*
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetSpeakerOutMode (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_SKP_OUTPUT_MODE_T eOutputMode;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eOutputMode = (MTAUDDEC_SKP_OUTPUT_MODE_T)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, eOutputMode = %d\n",
                   u1DecId,
                   eOutputMode);

    eRet = _MTAUD_SetSpeakerOutMode(u1DecId, eOutputMode);

    return eRet;
}


/**Setup individual channel volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u1Value                    0~100.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetChannelVolumeInDB (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_CH_T eChannel;
    INT16 i2Value;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eChannel = (MTAUDDEC_CH_T)rArg.ai4Arg[1];
    i2Value = (INT16)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eChannel = %d, u1Value = %d\n",
                   u1DecId,
                   eChannel,
                   i2Value);

    eRet = _MTAUD_SetChannelVolumeInDB(u1DecId, eChannel, i2Value);

    return eRet;
}

/**Setup balance value in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2LeftDB                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@param i2LeftDB                   Volume value in 0.25dB/step.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetBalanceInDB (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    INT16 i2LeftDB;
    INT16 i2RightDB;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    i2LeftDB = (INT16)rArg.ai4Arg[1];
    i2RightDB = (INT16)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, i2LeftDB = %d, i2RightDB = %d\n",
                   u1DecId,
                   i2LeftDB,
                   i2RightDB);

    eRet = _MTAUD_SetBalanceInDB(u1DecId, i2LeftDB, i2RightDB);

    return eRet;
}


/**Setup master volume in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Volume                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetMasterVolumeInDB (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    INT16 i2Volume;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    i2Volume = (INT16)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, i2Volume = %d\n",
                   u1DecId,
                   i2Volume);

    eRet = _MTAUD_SetMasterVolumeInDB (u1DecId, i2Volume);

    return eRet;
}

/**Setup master volume for bypass channel in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Volume                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetMasterVolumeForBypassChannelInDB (unsigned long arg)

{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    INT16 i2Volume;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    i2Volume = (INT16)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, i2Volume = %d\n",
                   u1DecId,
                   i2Volume);

    eRet = _MTAUD_SetMasterVolumeForBypassChannelInDB(u1DecId, i2Volume);

    return eRet;
}

/**Setup clipper threshold in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512 (0dB ~ -128dB)
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUDDEC_SetClipperThresholdInDB (unsigned long arg)

{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8         u1DecId;
    MTAUDDEC_CH_T eChannel;
    INT16         i2Value;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId  = (UINT8)rArg.ai4Arg[0];
    eChannel = (MTAUDDEC_CH_T)rArg.ai4Arg[1];
    i2Value  = (INT16)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eChannel = %d, i2Value = %d\n",
                   u1DecId,
                   eChannel,
                   i2Value);

    eRet = _MTAUD_SetClipperThresholdInDB(u1DecId, eChannel, i2Value);

    return eRet;
}

/**Setup pre-gain in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512 (0dB ~ -128dB)
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUDDEC_SetPreGainInDB (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    INT16 i2Val;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    i2Val   = (INT16)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, i2Value = %d\n",
                   u1DecId,
                   i2Val);

    eRet = _MTAUD_SetPreGainInDB(u1DecId, i2Val);

    return eRet;
}


/**Setup AVC parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eAVCCfgType                Parameter type
*@param i4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetAvcCfg(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType;
    INT32 i4Parameter;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eAVCCfgType = (MTAUDDEC_AVC_CFG_TYPE_T)rArg.ai4Arg[1];
    i4Parameter = (INT32)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eAVCCfgType = %d, i4Parameter = %d\n",
                   u1DecId,
                   eAVCCfgType,
                   i4Parameter);

    eRet = _MTAUD_SetAvcCfg(u1DecId, eAVCCfgType, i4Parameter);

    return eRet;
}


/**Setup virtural surround parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eVSCfgType                 Parameter type
*@param u4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetVirtualSurroundCfg(unsigned long arg)

{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_VSURR_CFG_TYPE_T eVSCfgType;
    UINT32 u4Parameter;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eVSCfgType = (MTAUDDEC_VSURR_CFG_TYPE_T)rArg.ai4Arg[1];
    u4Parameter = (UINT32)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eVSCfgType = %d, u4Parameter = %d\n",
                   u1DecId,
                   eVSCfgType,
                   u4Parameter);

    eRet = _MTAUD_SetVirtualSurroundCfg(u1DecId, eVSCfgType, u4Parameter);

    return eRet;
}


/**Setup bass enhancer switch
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T  _MTAUDDEC_SetBassEnhancer (unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetBassEnhancer(u1DecId, fgEnable);

    return eRet;
}

/**Setup bass enhancer parameers
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eBassEnhancerType          Parameter type
*@param u4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetBassEnhancerCfg(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTAUDDEC_BE_PARAM_TYPE_T eBassEnhancerType;
    UINT32 u4Parameter;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eBassEnhancerType = (MTAUDDEC_BE_PARAM_TYPE_T)rArg.ai4Arg[1];
    u4Parameter = (UINT32)rArg.ai4Arg[2];

    MTAUDDEC_PRINT(" - u1DecId = %d, eBassEnhancerType = %d, u4Parameter = %d\n",
                   u1DecId,
                   eBassEnhancerType,
                   u4Parameter);

    eRet = _MTAUD_SetBassEnhancerCfg(u1DecId, eBassEnhancerType, u4Parameter);

    return eRet;
}


/**Setup PEQ switch on/off
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   Enable
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetPEQ(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetPEQ(u1DecId, fgEnable);

    return eRet;
}


/**Setup PEQ parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u4Frequency                Frequency
*@param eQValue                    Q
*@param i2Gain                     Gain (+-12dB) with 0.5dB/step
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetPEQCfg(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_6ARG_T rArg;

    UINT8 u1DecId;
    UINT8 u1Set;
    UINT32 u4Frequency;
    MTAUDDEC_PEQ_Q_TYPE_T eQValue;
    INT16 i2Gain;

    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    u1Set   = (UINT8)rArg.ai4Arg[1];
    u4Frequency = (UINT32)rArg.ai4Arg[2];
    eQValue = (MTAUDDEC_PEQ_Q_TYPE_T)rArg.ai4Arg[3];
    i2Gain = (INT16)rArg.ai4Arg[4];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1Set =%d, u4Frequency = %d, eQValue = %d, i2Gain = %d\n",
                   u1DecId,
                   u1Set,
                   u4Frequency,
                   eQValue,
                   i2Gain);

    eRet = _MTAUD_SetPEQCfg(u1DecId, u1Set, u4Frequency, eQValue, i2Gain);

    return eRet;
}

/**Setup Limiter switch on/off
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   Enable
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetLimiter(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n", u1DecId, fgEnable);

    eRet = _MTAUD_SetLimiter(u1DecId, fgEnable);

    return eRet;
}

/**Setup Limiter configuration
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Mode                     limiter mode
*                                  (0: off, 1: adaptive mode, 2: fixed mode).
*@param u1Flag                     limiter flag (0: off, 1: on)
*                                  bit 0: vol
*                                  bit 1: vsurr
*                                  bit 2: sbass
*                                  bit 3: EQ
*                                  bit 4: reverb
*                                  bit 5: virtual bass
*@param  u4Thresh                  limiter threshold (0x0 ~ 0x7fffff)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetLimiterCfg(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_4ARG_T rArg;

    UINT8  u1DecId;
    UINT8  u1Mode;
    UINT16 u2Flag;
    UINT32 u4Thresh;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    u1DecId  = (UINT8)rArg.ai4Arg[0];
    u1Mode   = (UINT8)rArg.ai4Arg[1];
    u2Flag   = (UINT16)rArg.ai4Arg[2];
    u4Thresh = (UINT32)rArg.ai4Arg[3];

    MTAUDDEC_PRINT(" - u1DecId = %d, u1Mode = %d, u2Flag = 0x%04x\n, u4Thresh = 0x%08x\n",
                   u1DecId,
                   u1Mode,
                   u2Flag,
                   u4Thresh);

    eRet = _MTAUD_SetLimiterCfg(u1DecId, u1Mode, u2Flag, u4Thresh);

    return eRet;
}


/**Setup HPF Fc switch
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   On/Off
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetHPF(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    BOOL fgEnable;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    fgEnable = (BOOL)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, fgEnable = %d\n",
                   u1DecId,
                   fgEnable);

    eRet = _MTAUD_SetHPF(u1DecId, fgEnable);

    return eRet;
}


/**Setup HPF Fc
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u2Fc                       Frequency (50 ~ 300 Hz)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetHPFFc(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    UINT8 u1DecId;
    UINT16 u2Fc;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0] ;
    u2Fc = (UINT16)rArg.ai4Arg[1];

    MTAUDDEC_PRINT(" - u1DecId = %d, u2Fc = %d\n",
                   u1DecId,
                   u2Fc);

    eRet = _MTAUD_SetHPFFc(u1DecId, u2Fc);

    return eRet;
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
/**Mixound Play
*@param u4addr
*@param u4len
*@param u4gain                     0x20000 means 0dB
*@param u2config                   bit  0:3  samplerate  0: 48k   1: 44.1k 2: 32k 3: 24k 4:16k 5:12k
*                                            bit  4: source mode : 0->mono, 1->stereo
*                                            bit  5: 0: 8bit, 1 16bits
*                                            bit  6: 0: bigendian  1:littleendian
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_MixSoundPlay(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_4ARG_T rArg;

    UINT32 u4addr;
    UINT32 u4len;
    UINT32 u4gain;
    UINT16 u2config;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    u4addr = (UINT32)rArg.ai4Arg[0] ;
    u4len = (UINT32)rArg.ai4Arg[1];
    u4gain = (UINT32)rArg.ai4Arg[2] ;
    u2config = (UINT16)rArg.ai4Arg[3];

#if MIXSOUND_MEMPLAY_USER_SPACE_BUFFER
    // if pending PCM playing, return with error
    if (_pu1MixSoundBuf)
    {
        return MTR_ERR_INV;
    }
    USR_SPACE_ACCESS_VALIDATE_SIZE(u4addr, u4len);
    _pu1MixSoundBuf = kmalloc(u4len, GFP_KERNEL);
    if (!_pu1MixSoundBuf)
    {
        return MTR_PARAMETER_ERROR;
    }
    COPY_FROM_USER_BUF_SIZE(u4addr, _pu1MixSoundBuf, u4len);
    u4addr = (UINT32)_pu1MixSoundBuf;
#endif

    MTAUDDEC_PRINT(" - u4addr = %d, u4len = %d, u4gain = %d, u2config = %d\n",
                   u4addr,
                   u4len,
                   u4gain,
                   u2config
                  );

    eRet = _MTAUD_MixSoundPlay(u4addr, u4len, u4gain, u2config);

    return eRet;
}


/**Mixsound Stop
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_MixSoundStop(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAUDDEC_PRINT(" - void\n");

    eRet = _MTAUD_MixSoundStop();

    return eRet;
}

/**Mixsound Pause
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_MixSoundPause(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAUDDEC_PRINT(" - void\n");

    eRet = _MTAUD_MixSoundPause();

    return eRet;
}

/**Mixsound Resume
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_MixSoundResume(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAUDDEC_PRINT(" - void\n");

    eRet = _MTAUD_MixSoundResume();

    return eRet;
}


#endif

/**set DSP speed
*@param u4Mode                   step is 0.1, 10: normal speed; 5: 0.5X speed ; 20: 2X speed
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTAUDDEC_SetDspSpeedMode(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    UINT32 u1Mode=10;
    u1Mode = (UINT32)arg;

    MTAUDDEC_PRINT(" - Dsp speed Mode = %d\n",
		u1Mode);
    eRet = _MTAUD_SetDSPSpeedMode(u1Mode);

    return eRet;
}
/**Set the S/PDIF Serial Copying Management System (SCMS) output bits.
*@param c_bit                   bit2
*@param l_bit                    bit15
*@retval MTR_OK               Success.
*@retval MTR_NOT_OK       Fail.
*/
static MT_RESULT_T  _MTAUDEC_SetSCMS(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;

    BOOL c_bit, l_bit;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    c_bit = (BOOL)rArg.ai4Arg[0];
    l_bit = (BOOL)rArg.ai4Arg[1] ;

    MTAUDDEC_PRINT(" - c_bit = %d, l_bit = %d\n", c_bit,l_bit);
    eRet = _MTAUD_SetSCMS(c_bit, l_bit);

    return eRet;
    }

//MTK specific tool for 3rd party
static MT_RESULT_T _MTAUDDEC_RecordPCM (unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4ClipBufferPointer;
    UINT32 u4BufferSize;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);
    u4ClipBufferPointer = (UINT32)rArg.ai4Arg[0];
    u4BufferSize = (UINT32)rArg.ai4Arg[1];
    eRet = _MTAUD_RecordPCM(u4ClipBufferPointer, u4BufferSize);
    return eRet;    
}

static MT_RESULT_T _MTAUDDEC_DownloadDram (unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u4BufPtr;
    UINT32 u4Addr;
    UINT32 u4Size;
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);
    u4BufPtr = (UINT32)rArg.ai4Arg[0];
    u4Addr = (UINT32)rArg.ai4Arg[1];
    u4Size = (UINT32)rArg.ai4Arg[2];
    eRet = _MTAUD_DownloadDram(u4BufPtr, u4Addr, u4Size);
    return eRet;    
}

static MT_RESULT_T _MTAUDDEC_UploadDram (unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u4BufPtr;
    UINT32 u4Addr;
    UINT32 u4Size;
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);
    u4BufPtr = (UINT32)rArg.ai4Arg[0];
    u4Addr = (UINT32)rArg.ai4Arg[1];
    u4Size = (UINT32)rArg.ai4Arg[2];
    eRet = _MTAUD_UploadDram(u4BufPtr, u4Addr, u4Size);
    return eRet;    
}

int mtal_ioctl_mtauddec(struct inode *inode, struct file *file, unsigned int cmd,
                        unsigned long arg)
{
    switch (cmd)
    {
    case MTAL_IO_AUDDEC_INIT:
        return _MTAUDDEC_Init();
    case MTAL_IO_AUDDEC_GETDECTYPE:
        return _MTAUDDEC_GetDecType(arg);
    case MTAL_IO_AUDDEC_SETDECTYPE:
        return _MTAUDDEC_SetDecType(arg);
    case MTAL_IO_AUDDEC_PLAY:
        return _MTAUDDEC_Play(arg);
    case MTAL_IO_AUDDEC_RESUME:
        return _MTAUDDEC_Resume(arg);
    case MTAL_IO_AUDDEC_PAUSE:
        return _MTAUDDEC_Pause(arg);
    case MTAL_IO_AUDDEC_STOP:
        return _MTAUDDEC_Stop(arg);
    case MTAL_IO_AUDDEC_GETDECONPLAYINGSTATUS:
        return _MTAUDDEC_GetDecOnPlayingStatus(arg);
    case MTAL_IO_AUDDEC_SETAVSYNMODE:
        return _MTAUDDEC_SetAvSynMode(arg);
    case MTAL_IO_AUDDEC_SETAC3COMPMODE:
        return _MTAUDDEC_SetAc3CompMode(arg);
    case MTAL_IO_AUDDEC_SETAC3DOWNMIXMODE:
        return _MTAUDDEC_SetAc3DownmixMode(arg);
    case MTAL_IO_AUDDEC_SETAC3DRCRANGE:
        return _MTAUDDEC_SetAc3DrcRange(arg);
    case MTAL_IO_AUDDEC_GETATVDETECTMODE:
        return _MTAUDDEC_GetAtvDetectMode(arg);
    case MTAL_IO_AUDDEC_GETATVSOUNDSYS:
        return _MTAUDDEC_GetAtvSoundSys(arg);
    case MTAL_IO_AUDDEC_GETATVSOUNDSYSSTRENGTH:
        return _MTAUDDEC_GetAtvSoundSysStrength(arg);
    case MTAL_IO_AUDDEC_SETATVOUTPUTMODE:
        return _MTAUDDEC_SetAtvOutputMode(arg);
    case MTAL_IO_AUDDEC_SETSIFMODE:
        return _MTAUDDEC_SetSIFMode(arg);
    case MTAL_IO_AUDDEC_GETDETECTSIFMODE:
        return _MTAUDDEC_GetDetectSIFMode(arg);
    case MTAL_IO_AUDDEC_SETPCMINFO:
        return _MTAUDDEC_SetPcmInfo(arg);
    case MTAL_IO_AUDDEC_SETCHANNELDELAY:
        return _MTAUDDEC_SetChannelDelay(arg);
    case MTAL_IO_AUDDEC_SETCHANNELVOLUME:
        return _MTAUDDEC_SetChannelVolume(arg);
    case MTAL_IO_AUDDEC_SETCHANNELMUTE:
        return _MTAUDDEC_SetChannelMute(arg);
    case MTAL_IO_AUDDEC_SETCHANNELVOLGAIN:
        return _MTAUDDEC_SetChannelVolGain(arg);
    case MTAL_IO_AUDDEC_SETBALANCE:
        return _MTAUDDEC_SetBalance(arg);
    case MTAL_IO_AUDDEC_SETMASTERVOLUME:
        return _MTAUDDEC_SetMasterVolume(arg);
    case MTAL_IO_AUDDEC_SETAVC:
        return _MTAUDDEC_SetAvc(arg);
    case MTAL_IO_AUDDEC_SETBBE:
        return _MTAUDDEC_SetBBE(arg);
    case MTAL_IO_AUDDEC_SETSRS:
        return _MTAUDDEC_SetSRS(arg);
    case MTAL_IO_AUDDEC_SETVIRTUALSURROUND:
        return _MTAUDDEC_SetVirtualSurround(arg);
    case MTAL_IO_AUDDEC_SETBASSBOOST:
        return _MTAUDDEC_SetBassBoost(arg);
    case MTAL_IO_AUDDEC_SETBASS:
        return _MTAUDDEC_SetBass(arg);
    case MTAL_IO_AUDDEC_SETTREBLEBOOST:
        return _MTAUDDEC_SetTrebleBoost(arg);
    case MTAL_IO_AUDDEC_SETTREBLE:
        return _MTAUDDEC_SetTreble(arg);
    case MTAL_IO_AUDDEC_SETEQUALIZER:
        return _MTAUDDEC_SetEqualizer(arg);
    case MTAL_IO_AUDDEC_SETEQUALIZERBANDCONFIG:
        return _MTAUDDEC_SetEqualizerBandConfig(arg);
    case MTAL_IO_AUDDEC_SETAVINPUTMUXSEL:
        return _MTAUDDEC_SetAvInputMuxSel(arg);
    case MTAL_IO_AUDDEC_SETDUALMODE:
        return _MTAUDDEC_SetDualMode(arg);
    case MTAL_IO_AUDDEC_SETHDEVMODE:
        return _MTAUDDEC_SetHDevMode(arg);
    case MTAL_IO_AUDDEC_SETIECCONFIG:
        return _MTAUDDEC_SetIecConfig(arg);
    case MTAL_IO_AUDDEC_SETIECCOPYRIGHT:
        return _MTAUDDEC_SetIecCopyright(arg);
    case MTAL_IO_AUDDEC_SETIECCATEGORYCODE:
        return _MTAUDDEC_SetIecCategoryCode(arg);
    case MTAL_IO_AUDDEC_SETSPEAKEROUTMODE:
        return _MTAUDDEC_SetSpeakerOutMode(arg);
    case MTAL_IO_AUDDEC_SETHPFFC:
        return _MTAUDDEC_SetHPFFc(arg);
    case MTAL_IO_AUDDEC_SETHPF:
        return _MTAUDDEC_SetHPF(arg);
    case MTAL_IO_AUDDEC_SETPEQCFG:
        return _MTAUDDEC_SetPEQCfg(arg);
    case MTAL_IO_AUDDEC_SETPEQ:
        return _MTAUDDEC_SetPEQ(arg);
    case MTAL_IO_AUDDEC_SETBASSENHANCERCFG:
        return _MTAUDDEC_SetBassEnhancerCfg(arg);
    case MTAL_IO_AUDDEC_SETVIRTUALSURROUDCFG:
        return _MTAUDDEC_SetVirtualSurroundCfg(arg);
    case MTAL_IO_AUDDEC_SETAVCCFG:
        return _MTAUDDEC_SetAvcCfg(arg);
    case MTAL_IO_AUDDEC_SET2BANDAVCCFG:
        return _MTAUDDEC_Set2BandAvcCfg(arg);
    case MTAL_IO_AUDDEC_SETMASTERVOLUMEFORBYPASSCHANNELINDB:
        return _MTAUDDEC_SetMasterVolumeForBypassChannelInDB(arg);
    case MTAL_IO_AUDDEC_SETMASTERVOLUMEINDB:
        return _MTAUDDEC_SetMasterVolumeInDB(arg);
    case MTAL_IO_AUDDEC_SETBALANCEINDB:
        return _MTAUDDEC_SetBalanceInDB(arg);
    case MTAL_IO_AUDDEC_SETCHANNELVOLUMEINDB:
        return _MTAUDDEC_SetChannelVolumeInDB(arg);
    case MTAL_IO_AUDDEC_SETBASSENHANCER:
        return _MTAUDDEC_SetBassEnhancer(arg);
    case MTAL_IO_AUDDEC_SETIECCHANNEL:
        return _MTAUDDEC_SetIecChannel(arg);
    case MTAL_IO_AUDDEC_SETLIMITER:
        return _MTAUDDEC_SetLimiter(arg);
    case MTAL_IO_AUDDEC_SETLIMITERCFG:
        return _MTAUDDEC_SetLimiterCfg(arg);
    case MTAL_IO_AUDDEC_SETCLIPPERTHRESHOLDINDB:
        return _MTAUDDEC_SetClipperThresholdInDB(arg);
    case MTAL_IO_AUDDEC_SETPREGAININDB:
        return _MTAUDDEC_SetPreGainInDB(arg);
    case MTAL_IO_AUDDEC_GETAC3DOWNMIXMODE:
        return _MTAUDDEC_GetAc3DownmixMode(arg);
    case MTAL_IO_AUDDEC_GETAC3MODE:
        return _MTAUDDEC_GetAc3Mode(arg);
    case MTAL_IO_AUDDEC_GETSAMPLINGRATE:
        return _MTAUDDEC_GetSamplingRate(arg);
    case MTAL_IO_AUDDEC_SETAC3DUALMONO:
        return _MTAUDDEC_SetAc3DualMono(arg);
    case MTAL_IO_AUDDEC_GETAC3DUALMONO:
        return _MTAUDDEC_GetAc3DualMono(arg);
    case MTAL_IO_AUDDEC_GETIECCOPYRIGHT:
        return _MTAUDDEC_GetIecCopyright(arg);
    case MTAL_IO_AUDDEC_GETIECCATEGORYCODE:
        return _MTAUDDEC_GetIecCategoryCode(arg);
    case MTAL_IO_AUDDEC_SETIECWORDLENGTH:
        return _MTAUDDEC_SetIecWordLength(arg);
    case MTAL_IO_AUDDEC_GETIECWORDLENGTH:
        return _MTAUDDEC_GetIecWordLength(arg);
    case MTAL_IO_AUDDEC_SETADMODE:
        return _MTAUDDEC_SetADChannelMode(arg);
    case MTAL_IO_AUDDEC_REGCB:
        return _MTAUDDEC_RegCb(arg);
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    case MTAL_IO_AUDDEC_SETMIXSOUNDPLAY:
        return _MTAUDDEC_MixSoundPlay(arg);
    case MTAL_IO_AUDDEC_SETMIXSOUNDSTOP:
        return _MTAUDDEC_MixSoundStop(arg);
    case MTAL_IO_AUDDEC_SETMIXSOUNDPAUSE:
        return _MTAUDDEC_MixSoundPause(arg);
    case MTAL_IO_AUDDEC_SETMIXSOUNDRESUME:
        return _MTAUDDEC_MixSoundResume(arg);
#endif
    case MTAL_IO_AUDDEC_SETSOURCEVOLUME:
        return _MTAUDDEC_SetSourceVolume(arg);
    case MTAL_IO_AUDDEC_SETCHANNELDELAYNOUOP:
        return _MTAUDDEC_SetChannelDelayNoUop(arg);
    case MTAL_IO_AUDDEC_SETADENABLE:
        return _MTAUDDEC_SetADEnable(arg);
    case MTAL_IO_AUDDEC_LOADAUDIOCLIP:
        return _MTAUDDEC_LoadAudioClip(arg);
    case MTAL_IO_AUDDEC_PLAYAUDIOCLIP:
        return _MTAUDDEC_PlayAudioClip(arg);
    case MTAL_IO_AUDDEC_STOPAUDIOCLIP:
        return _MTAUDDEC_StopAudioClip(arg);
    case MTAL_IO_AUDDEC_DELETEAUDIOCLIP:
        return _MTAUDDEC_DeleteAudioClip(arg);
    case MTAL_IO_AUDDEC_GETAUDIOPTS:
        return _MTAUDDEC_GetAudioPTS(arg);
    case MTAL_IO_AUDDEC_SETSPDIFSCMS:
        return _MTAUDDEC_SetSpdifSCMS(arg);
    case MTAL_IO_AUDDEC_GETAC3ESINFO:
        return _MTAUDDEC_GetAC3ESInfo(arg);
    case MTAL_IO_AUDDEC_GETMPEGESINFO:
        return _MTAUDDEC_GetMPEGESInfo(arg);
    case MTAL_IO_AUDDEC_GETHEAACESINFO:
        return _MTAUDDEC_GetHEAACESInfo(arg);
    case MTAL_IO_AUDDEC_SETAVINPUTMUXSELBYAD:
        return _MTAUDDEC_SetAvInputMuxSelByAD(arg);
    case MTAL_IO_AUDDEC_SETAC3DRCRANGE100:
        return _MTAUDDEC_SetAc3DrcRange100(arg);
	case MTAL_IO_AUDDEC_SETDSPSPEEDMODE:
		return _MTAUDDEC_SetDspSpeedMode(arg);
    //MTK specific tool for 3rd party
    case MTAL_IO_AUDDEC_RECORDPCM:
        return _MTAUDDEC_RecordPCM(arg);
    case MTAL_IO_AUDDEC_DOWNLOAD_DRAM:
        return _MTAUDDEC_DownloadDram(arg);
    case MTAL_IO_AUDDEC_UPLOAD_DRAM:
        return _MTAUDDEC_UploadDram(arg);   
		
	case MTAL_IO_AUDDEC_SETSCMS:
		return _MTAUDEC_SetSCMS(arg);
    default:
        return MTR_NOT_SUPPORTED;
    }
    return MTR_NOT_OK;
}

