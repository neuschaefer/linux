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
 * $RCSfile: io_papi_gen_sbp.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_gen_sbp.c
 *  PAPI kernel driver implementation: SoundBeeperGeneratorTestPattern
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "aud_if.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>
#include "papi_types.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TM_FAIL                     (-0x800)

const UINT32 SBP_TABLE[5][4] = {
    {7,7,6,6}, //200
    {8,8,8,8}, //400
    {3,2,1,0}, //1000
    {5,5,5,5}, //4000 
    {4,4,4,4} //20000
};
/*****************************
0  1K tone 0dB
1  1K tone -20dB
2  1K tone -60dB
3  1K tone -120dB
4  20K tone 0dB
5  4K tone 0dB
6  200 tone 0dB
7  200 tone -60dB
8  400 tone 0dB
9  1K/3K tone 0dB
10  1K tone 0dB L channel only
*******************************/


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
        return TM_FAIL ; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return TM_FAIL ; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return TM_FAIL ; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return TM_FAIL ; \
    }

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT32 _u4CurrentFreq = 0;
static UINT32 _u4CurrentVolume = 0;
static UINT32 _u4CurrentMute = 1;
static UINT32 _u4CurrentSelection = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _ioctl_SBP_GetVolume(UINT32 u4Arg)
{ 
    IOCTL_2ARG_T rArg;
    int i4Vol;
    int* pi4Vol;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_2ARG_T));

    // Get arguments
    pi4Vol = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    i4Vol = _u4CurrentVolume;
    COPY_TO_USER(pi4Vol, &i4Vol, sizeof (UINT32));

    return TM_OK;
}

static INT32 _ioctl_SBP_SetVolume(UINT32 u4Arg)
{ 
    IOCTL_2ARG_T rArg;    
    UINT32 i4Vol;
    MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4Vol = (UINT32)rArg.au4Arg[0];    
    
    _u4CurrentVolume = i4Vol;
    _u4CurrentSelection = SBP_TABLE[_u4CurrentFreq][_u4CurrentVolume];

    if (_u4CurrentMute!=1){
        _u4CurrentSelection = SBP_TABLE[_u4CurrentFreq][_u4CurrentVolume];
        if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
        {
            AUD_StopMemStr();
            AUD_DSPCmdStop(AUD_DEC_MAIN);
        }
        rMemBuf.u4LoopCount = 0; // Infinite loop
        rPcmSetting.eSampleFreq = FS_48K;
        rPcmSetting.ePcmDataInfo = PCM_24_BITS;
        AUD_DspProcMode(AUD_DEC_MAIN, 0xdc00);
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
        AUD_PcmSetting(AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);
        AUD_StrSelect(AUD_DEC_MAIN, _u4CurrentSelection, &rMemBuf);
        AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);        
    }
    return TM_OK;
}

static INT32 _ioctl_SBP_GetFrequency(UINT32 u4Arg)
{ 
    IOCTL_2ARG_T rArg;
    int i4Freq;
    int* pi4Freq;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_2ARG_T));

    // Get arguments
    pi4Freq = (UINT32*)rArg.au4Arg[0];

    // Copy data to user memory
    switch(_u4CurrentFreq){
        case 0:
            i4Freq = 200;
            break;
        case 1:
            i4Freq = 400;
            break;
        case 2:
            i4Freq = 1000;
            break;
        case 3:
            i4Freq = 4000;
            break;
        case 4:
            i4Freq = 20000;
            break;
    }
    COPY_TO_USER(pi4Freq, &i4Freq, sizeof (UINT32));

    return TM_OK;
}

static INT32 _ioctl_SBP_SetFrequency(UINT32 u4Arg)
{ 
    IOCTL_2ARG_T rArg;    
    UINT32 i4Freq;
     MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    i4Freq = (UINT32)rArg.au4Arg[0];    
    if(i4Freq < 300)
    {
        _u4CurrentFreq = 0; //200
    }
    else if(i4Freq < 700)
    {
        _u4CurrentFreq = 1; //400
    }
    else if(i4Freq < 2500)
    {
        _u4CurrentFreq = 2; //1000
    }
    else if(i4Freq <12000 )
    {
        _u4CurrentFreq = 3; //4000
    }
    else
    {
        _u4CurrentFreq = 4; //20000
    }

    if (_u4CurrentMute!=1){
        _u4CurrentSelection = SBP_TABLE[_u4CurrentFreq][_u4CurrentVolume];
        if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
        {
            AUD_StopMemStr();
            AUD_DSPCmdStop(AUD_DEC_MAIN);
        }
        rMemBuf.u4LoopCount = 0; // Infinite loop
        rPcmSetting.eSampleFreq = FS_48K;
        rPcmSetting.ePcmDataInfo = PCM_24_BITS;
        AUD_DspProcMode(AUD_DEC_MAIN, 0xdc00);
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
        AUD_PcmSetting(AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);
        AUD_StrSelect(AUD_DEC_MAIN, _u4CurrentSelection, &rMemBuf);
        AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);        
    }    
    return TM_OK;
}

static INT32 _ioctl_SBP_GetMute(UINT32 u4Arg)
{ 
    IOCTL_2ARG_T rArg;
    Bool fgMute;
    Bool* pfgMute;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_2ARG_T));

    // Get arguments
    pfgMute = (Bool*)rArg.au4Arg[0];

    // Copy data to user memory
    fgMute = _u4CurrentMute;
    COPY_TO_USER(pfgMute, &fgMute, sizeof (UINT32));

    return TM_OK;
}

static INT32 _ioctl_SBP_SetMute(UINT32 u4Arg)
{ 
    IOCTL_2ARG_T rArg;    
    Bool fgMute;
    MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    fgMute = (UINT32)rArg.au4Arg[0];    

    if(fgMute == _u4CurrentMute){
        return TM_OK;
    }
    else{
        _u4CurrentMute = fgMute;
        if(_u4CurrentMute == 1){
            AUD_StopMemStr();
            AUD_DSPCmdStop(AUD_DEC_MAIN);
        }else{
            _u4CurrentSelection = SBP_TABLE[_u4CurrentFreq][_u4CurrentVolume];
            if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
            {
                AUD_StopMemStr();
                AUD_DSPCmdStop(AUD_DEC_MAIN);
            }
            rMemBuf.u4LoopCount = 0; // Infinite loop
            rPcmSetting.eSampleFreq = FS_48K;
            rPcmSetting.ePcmDataInfo = PCM_24_BITS;
            AUD_DspProcMode(AUD_DEC_MAIN, 0xdc00);
            AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
            AUD_PcmSetting(AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);
            AUD_StrSelect(AUD_DEC_MAIN, _u4CurrentSelection, &rMemBuf);
            AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
            AUD_DSPCmdPlay(AUD_DEC_MAIN);
        }      
    }
    return TM_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_gen_sbp(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;
    //_ApiLock();

    switch (cmd)
    {
        case PAPI_IO_GEN_SBP_GETFREQ:
           i4Ret = _ioctl_SBP_GetFrequency(arg);
           break;
       case PAPI_IO_GEN_SBP_SETFREQ:
           i4Ret = _ioctl_SBP_SetFrequency(arg);
           break;
       case PAPI_IO_GEN_SBP_GETMUTE:
           i4Ret = _ioctl_SBP_GetMute(arg);
           break;
       case PAPI_IO_GEN_SBP_SETMUTE:
           i4Ret = _ioctl_SBP_SetMute(arg);
           break;
       case PAPI_IO_GEN_SBP_GETVOLUME:
           i4Ret = _ioctl_SBP_GetVolume(arg);
           break;
       case PAPI_IO_GEN_SBP_SETVOLUME:
           i4Ret = _ioctl_SBP_SetVolume(arg);
           break;

    default:
        printk("papi_ioctl_gen_sbp: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }
    
    //_ApiUnlock();

    return i4Ret;
}

