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
 * $RCSfile: io_sdauxout.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdauxout.c
 *   SDAL SdAuxout Module ioctl interface of MT538X.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>
#include "sd_vdopath.h"
#include "SdType.h"

#include "sd_audio.h"
#include "drv_tve.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \         
    {                                                       \        
        printk("Sd_Auxout ACCESS argument error\n");     \
        return -1;                                          \
    }                                                       \    

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("Sd_Auxout COPY_FROM_USER error\n");      \    
        return -1;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(SDAL_IOCTL_2ARG_T)))              \         
    {                                                       \        
        printk("Sd_Auxout ACCESS argument error\n");     \
        return -1;                                          \
    }                                                       \    

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(SDAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("Sd_Auxout COPY_FROM_USER error\n");      \    
        return -1;                                          \
    }                                                       \

static AUD_DEC_INFOR_T  _arAudDec[AUD_DEC_NUM];
//-----------------------------------------------------------------------------
/** _SdAuxout_AudioMute
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static SdResult _SdAuxOut_AudioMute(SdAuxOutID_k eAuxOutId, SdBool_t bMuteOn, SdAuxOut_Audio_k eAuxAudio)
{
    INT32                i4Ret;
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    i4Ret = _SD_AUD_AuxOutMute(eAuxOutId, bMuteOn, eAuxAudio);    

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _SdAuxout_AudioStart
 *
 *  @return SD_OK	        Succeed
 *  @return SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static SdResult _SdAuxOut_AudioStart(SdAuxOutID_k eAuxOutId)
{
    SdBackEndInID_k   eInputId = SD_BE_IN_AVDEC0;
    INT32                i4Ret;
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }

    if ((eAuxOutId == SD_AUXOUT0)&&(_arAudDec[SD_SOUND1].fgConnected))
   {
      // tuner bypass
       i4Ret = _SD_AUD_Connect(SD_SOUND1, _arAudDec[SD_SOUND1].eSourcComp, eInputId);
   }
    else  if ((eAuxOutId == SD_AUXOUT1)&&(_arAudDec[SD_SOUND0].fgConnected))
   {
       // moniter out
       i4Ret = _SD_AUD_AuxOutMute(SD_AUXOUT1, FALSE, SD_AUXAUDIO_ANALOG);    
   }
    
    return i4Ret;
}

static SdResult _SdAuxOut_Init(SdPowerState_k ePowerState)
{
    // do nothing
    //video
    Bypass_Init();

    return SD_OK; 
}

static SdResult _SdAuxOut_Connect(SdAuxOutID_k eAuxOutId, SdSource_k eSRC)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    // 1. connect video
    AuxOutConnect(eAuxOutId, Sd_VdoPathSrcMap(eSRC));
    
    // 2. connect audio 
    if (eAuxOutId == SD_AUXOUT0)
    {   // tuner bypass
       _arAudDec[SD_SOUND1].eSourcComp= eSRC;
       _arAudDec[SD_SOUND1].fgConnected = TRUE;
    }
    else
    {   
        // monitor out
       _arAudDec[SD_SOUND0].eSourcComp= eSRC;
       _arAudDec[SD_SOUND0].fgConnected = TRUE;
    }

    return SD_OK;
}
 
static SdResult _SdAuxOut_Disconnect(SdAuxOutID_k eAuxOutId, SdSource_k eSRC)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    // 1. disconnect video
    AuxOutDisconnect(eAuxOutId, Sd_VdoPathSrcMap(eSRC));

    // 2. disconnect audio 
    if (eAuxOutId == SD_AUXOUT0)
    {   // tuner bypass
       _arAudDec[SD_SOUND1].fgConnected = FALSE;
    }
    else
    {   
        // monitor out , do nothing
       _arAudDec[SD_SOUND0].fgConnected = FALSE;
    }

    return SD_OK;
}

static SdResult _SdAuxOut_VideoStart(SdAuxOutID_k eAuxOutId)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    //video
    AuxOut_VideoStart(eAuxOutId);
    
    return SD_OK;
}

static SdResult _SdAuxOut_VideoStop(SdAuxOutID_k eAuxOutId)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    //video
   AuxOut_VideoStop(eAuxOutId);
   
    return SD_OK;
}

static SdResult _SdAuxOut_VideoFmt(SdAuxOutID_k eAuxOutId, SdAuxOutFmt_k eAuxOutFmt)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    //video
    AuxOut_VideoFmt(eAuxOutId, eAuxOutFmt);

    return SD_OK;
}


static SdResult _SdAuxOut_AudioStop(SdAuxOutID_k eAuxOutId)
{
    INT32                i4Ret;
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    
    if ((eAuxOutId == SD_AUXOUT0)&&(_arAudDec[SD_SOUND1].fgConnected))
    {
      // tuner bypass
       i4Ret = _SD_AUD_Disconnect(SD_SOUND1);
    }
    else if ((eAuxOutId == SD_AUXOUT1)&&(_arAudDec[SD_SOUND0].fgConnected))
    {
       // moniter out
       i4Ret = _SD_AUD_AuxOutMute(SD_AUXOUT1, TRUE, SD_AUXAUDIO_ANALOG);    
    }
    return i4Ret;
}

static SdResult _SdAuxOut_VbiStart(SdAuxOutID_k eAuxOutId, SdAuxOut_VbiDataType_k eType)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    //video
    AuxOut_VbiStart(eAuxOutId, eType, TRUE);
    
    return SD_OK;
}

static SdResult _SdAuxOut_VbiStop(SdAuxOutID_k eAuxOutId, SdAuxOut_VbiDataType_k eType)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    //video
   AuxOut_VbiStart(eAuxOutId, eType, FALSE);
   
    return SD_OK;
}


static SdResult _SdAuxOut_SendVbiData(SdAuxOutID_k eAuxOutId, SdAuxOut_VbiDataType_k eType, uint8_t *pData, uint32_t length)
{
    INT32 i4Ret;
    UINT8 *pu1Buf = NULL;    

    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
    return SD_NOT_SUPPORTED;
}

    pu1Buf = kmalloc (length, GFP_KERNEL);
    if (!pu1Buf) 
    {
        return -1;
    }

    if (copy_from_user((void *)pu1Buf, (void __user *)pData, length))
    {
        i4Ret = -1;
        goto _AuxOut_SendVbiDataEXIT;
    }

    AuxOut_SendVbiData(eAuxOutId, eType, pu1Buf, length);

_AuxOut_SendVbiDataEXIT:
    kfree(pu1Buf);
    
    return SD_OK;
}

static SdResult _SdAuxOut_VideoMute(SdAuxOutID_k eAuxOutId, SdBool_t bMuteOn)
{
    // SDAL_FIXME 
    if(eAuxOutId >= SD_AUXOUT_MAX)
    {
        return SD_NOT_SUPPORTED;
    }
    //video
    AuxOut_VideoMute(eAuxOutId, bMuteOn);
    return SD_OK;
}

static SdResult _SdAuxOut_Debug(void)
{
    // do nothing
    return SD_OK; 
}


int sdal_ioctl_sdauxout(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	SDAL_IOCTL_4ARG_T rArg12;	//current use SDAL_IOCTL_4ARG_T instead of SDAL_IOCTL_12ARG_T

	if (_IOC_SIZE(cmd)>sizeof(unsigned long))
	{	
	    // Check access of user space
	    if (!access_ok(VERIFY_READ, (void __user *)arg,  _IOC_SIZE(cmd) ))
	    {
		printk("sdal_ioctl_sdauxout  arg access error\n");
		return SD_ERR_SYS;
	    }
	    // Copy user space argument
	    if(copy_from_user(&rArg12, (void __user *)arg, _IOC_SIZE(cmd)))
	    {
		printk("sdal_ioctl_sdauxout copy_from_user error\n");
		return SD_ERR_SYS;
	    }
	}

	switch (cmd) 
	{
		case SDAL_IO_AUXOUT_INIT:
			return _SdAuxOut_Init((SdPowerState_k)arg); 			
		case SDAL_IO_AUXOUT_CONNECT:
			return _SdAuxOut_Connect((SdAuxOutID_k)rArg12.au4Arg[0], (SdSource_k)rArg12.au4Arg[1]);
		case SDAL_IO_AUXOUT_DISCONNECT:
			return _SdAuxOut_Disconnect((SdAuxOutID_k)rArg12.au4Arg[0], (SdSource_k)rArg12.au4Arg[1]);
		case SDAL_IO_AUXOUT_VIDEOSTART:
			return _SdAuxOut_VideoStart((SdAuxOutID_k)arg);
		case SDAL_IO_AUXOUT_VIDEOSTOP:
			return _SdAuxOut_VideoStop((SdAuxOutID_k)arg);
		case SDAL_IO_AUXOUT_VIDEOFMT:
			return _SdAuxOut_VideoFmt((SdAuxOutID_k)rArg12.au4Arg[0], (SdAuxOutFmt_k)rArg12.au4Arg[1]);
		case SDAL_IO_AUXOUT_AUDIOSTART:
		    return _SdAuxOut_AudioStart((SdAuxOutID_k)arg);
		case SDAL_IO_AUXOUT_AUDIOSTOP:
		    return _SdAuxOut_AudioStop((SdAuxOutID_k)arg);
		case SDAL_IO_AUXOUT_SENDVBIDATA:  
			return _SdAuxOut_SendVbiData((SdAuxOutID_k)rArg12.au4Arg[0], (SdAuxOut_VbiDataType_k)rArg12.au4Arg[1], (uint8_t *)rArg12.au4Arg[2], (uint32_t)rArg12.au4Arg[3]);
		case SDAL_IO_AUXOUT_VBISTART:  
			return _SdAuxOut_VbiStart((SdAuxOutID_k)rArg12.au4Arg[0], (SdAuxOut_VbiDataType_k)rArg12.au4Arg[1]);
		case SDAL_IO_AUXOUT_VBISTOP:  
			return _SdAuxOut_VbiStop((SdAuxOutID_k)rArg12.au4Arg[0], (SdAuxOut_VbiDataType_k)rArg12.au4Arg[1]);
		case SDAL_IO_AUXOUT_VIDEOMUTE:
			return _SdAuxOut_VideoMute((SdAuxOutID_k)rArg12.au4Arg[0], (SdBool_t)rArg12.au4Arg[1]);
		case SDAL_IO_AUXOUT_AUDIOMUTE:
		    return  _SdAuxOut_AudioMute((SdAuxOutID_k)rArg12.au4Arg[0], (SdBool_t)rArg12.au4Arg[1], (SdAuxOut_Audio_k)rArg12.au4Arg[2]);
		case SDAL_IO_AUXOUT_MACROVISIONSTART:
			break;
		case SDAL_IO_AUXOUT_MACROVISIONSTOP:
			break;
		case SDAL_IO_AUXOUT_DEBUG:
			return _SdAuxOut_Debug();
		default:
		    break;
	}
	return SD_NOT_SUPPORTED;
}

