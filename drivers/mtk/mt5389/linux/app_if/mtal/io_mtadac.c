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
 * $RCSfile: io_mtadac.c,v $
 * $Revision: #5 $
 *---------------------------------------------------------------------------*/

/** @file io_mtadac.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtadac.h"
#include "aud_if.h"
#include "adac_if.h"
#include <linux/mtal_ioctl.h>
#include "x_bim.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifdef CC_MT5391
extern void _MT8297_Mute(UINT8 u1DacId,BOOL fgEnable);
extern void _MT8297_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
#endif
extern void _MT5365_DacChlSel(UINT8 u1DacId, AUD_CHL_DEF_T eChl);
extern void _MT5365_Mute(UINT8 u1ALRx,BOOL fgMute);

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(INT32)))                            \
    {                                                       \
        printk("mtal_ioctl_adc argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                        \
    {                                                       \
        printk("mtal_ioctl_adc argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_adc argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_adc argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_adc argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_adc argument error\n");      \
        return MTR_ERR_INV;                                          \
    }   	
/** Initialize ADAC module.
*@param void
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTADAC_Init(void)
{
    MTADAC_PRINT(" - void\n");
	
	x_os_drv_set_timestamp("ADAC_Init start");
    ADAC_Init();
	x_os_drv_set_timestamp("ADAC_Init end");

    return MTR_OK;
}

/** Mute ADAC.
*@param eDacId                     Audio dac id
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTADAC_SetMute(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    MTADAC_DAC_ID_T eDacId;
    UINT8           u1DacId = 0;
    BOOL fgMute;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eDacId = (MTADAC_DAC_ID_T)rArg.ai4Arg[0];
    fgMute = (BOOL)rArg.ai4Arg[1];

    MTADAC_PRINT(" - eDacId = %d, fgMute = %d\n",
		eDacId,
		fgMute);

#ifndef CC_MT5391
    switch(eDacId)
    {
    case MTADAC_DAC_ID_DAC0:
        u1DacId = 0;
        break;
    case MTADAC_DAC_ID_DAC1:
        u1DacId = 1;
        break;
    case MTADAC_DAC_ID_DAC2:
        u1DacId = 2;
        break;
#ifdef CC_MT5363
	case MTADAC_DAC_ID_DAC3:    
        u1DacId = 3;
        break;
#else
    case MTADAC_DAC_ID_HP:    
        u1DacId = 3;
        break;
#endif
    default:
        u1DacId = 0;
    }
#if (defined(CC_MT5365)||defined(CC_MT5395)||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389))
	ADAC_CodecMute(fgMute);
#else
    ADAC_CodecChannelMute(u1DacId, fgMute);
#endif
#else
    switch(eDacId)
    {
    case MTADAC_DAC_ID_DAC0:
        u1DacId = 0;
        break;
    case MTADAC_DAC_ID_DAC1:
        u1DacId = 1;
        break;
    case MTADAC_DAC_ID_DAC2:
        u1DacId = 2;
        break;
    case MTADAC_DAC_ID_HP:    
        u1DacId = 3;
        break;
    default:
        u1DacId = 0;
    }

    _MT8297_Mute(u1DacId, fgMute);
#endif

    return MTR_OK;
}

/** Setup ADAC refer to which channel.
*@param eDacId                     Audio dac id.
*@param eChannel                   Channel number.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTADAC_SetDacChannel(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T  rArg;
    MTADAC_DAC_ID_T    eDacId;
    MTAUDDEC_PCM_CH_T  eChannelId;
    AUD_CHL_DEF_T      eChannel;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eDacId = (MTADAC_DAC_ID_T)rArg.ai4Arg[0];
    eChannelId = (MTAUDDEC_PCM_CH_T)rArg.ai4Arg[1];

    MTADAC_PRINT(" - eDacId = %d, eChannelId = %d\n",
		eDacId,
		eChannelId);

    switch (eChannelId)
    {
    case MTAUDDEC_PCM_CH_L_R:
        eChannel = AUD_CHL_L_R;
        break;
    case MTAUDDEC_PCM_CH_LS_RS:
        eChannel = AUD_CHL_LS_RS;
        break;
    case MTAUDDEC_PCM_CH_C_SW:
        eChannel = AUD_CHL_C_SW;
        break;
    case MTAUDDEC_PCM_CH_BYPASS_L_R:
        eChannel = AUD_CHL_BYPASS;
        break;
    case MTAUDDEC_PCM_CH_DOWNMIX_L_R:
        eChannel = AUD_CHL_DOWNMIX;
        break;
    case MTAUDDEC_PCM_CH_SPEC_BYPASS_L_R:
        // Suppot multi-pair output(Copy CH9/10 to LS/RS/C/SUB)
        eChannel = AUD_CHL_LS_RS; 
        break;
    case MTAUDDEC_PCM_CH_TV_SCART_L_R:
        eChannel = AUD_CHL_AUX;  // Not support yet
        break;     
    default:
        eChannel = AUD_CHL_L_R;
    }

#if (defined(CC_MT5365)||defined(CC_MT5395)||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389))
	_MT5365_DacChlSel((UINT8)eDacId, eChannel);
#else
#ifdef CC_MT5391
	_MT8297_DacChlSel(eDacId, eChannel);
#else
    ADAC_CodecChannelMute(eDacId, eChannel);
#endif    
#endif    
        
    return MTR_OK;
}

/** Digital I2S format configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eFormat                    I2S format.
*@param eMclk                      MCLK frequency
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTADAC_SetDacFormatCfg(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;

    UINT8 u1DecId;
    MTADAC_DATA_FORMAT_T eFormat;
    MTADAC_MCLK_FREQUENCY_T eMclk;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1DecId = (UINT8)rArg.ai4Arg[0];
    eFormat = (MTADAC_DATA_FORMAT_T)rArg.ai4Arg[1];
    eMclk = (MTADAC_MCLK_FREQUENCY_T)rArg.ai4Arg[2];

    MTADAC_PRINT(" - u1DecId = %d, eFormat = %d, eMclk = %d\n",
		u1DecId,
		eFormat,
		eMclk);

    //add driver implementation code here

    return MTR_OK;
}

/** Mute Speaker.
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTADAC_SetSpeakerMute(unsigned long arg)
{
    BOOL fgMute;

    fgMute = (BOOL)arg;

    MTADAC_PRINT(" - fgMute = %d\n",
		fgMute);
    
    ADAC_GPIOPopMute(fgMute);
    ADAC_GpioAmpEnable(!fgMute);
    ADAC_GpioAmpPowerDown(fgMute);

    return MTR_OK;
}

/** Set HDMI ARC .
*@param fgMute                     on / off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTADAC_SetArcEnable(unsigned long arg)
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    MTADAC_PRINT(" - fgEnable = %d\n",
		fgEnable);
    #ifdef CC_DSP_SUPPORT_ARC
    ADAC_ArcEnable(fgEnable);
    #endif
    return MTR_OK;
}

/** Set AMP register.
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTADAC_SetAMPRegister(unsigned long arg)
{
	MTAL_IOCTL_3ARG_T rArg;
	UINT8 eAddress;
	UINT8 eValue;
	USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
	COPY_FROM_USER_2ARG(arg, rArg);
	eAddress = (UINT8)rArg.ai4Arg[0];
	eValue = (UINT8)rArg.ai4Arg[1];
	
		printf(" - eAddress = %d, eValue = %d\n",
			eAddress,
			eValue);
	
		ADAC_AmpRegWrite(eAddress,eValue);
	
		return MTR_OK;

}

int mtal_ioctl_mtadac(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
	switch (cmd)
	{
		case MTAL_IO_ADAC_INIT:
			return _MTADAC_Init();
		case MTAL_IO_ADAC_SETMUTE:
			return _MTADAC_SetMute(arg);
		case MTAL_IO_ADAC_SETDACFORMATCFG:
			return _MTADAC_SetDacFormatCfg(arg);
		case MTAL_IO_ADAC_SETSPEAKERMUTE:
			return _MTADAC_SetSpeakerMute(arg);
        case MTAL_IO_ADAC_SETDACCHANNEL:
			return _MTADAC_SetDacChannel(arg);
		case MTAL_IO_ADAC_SETAMPREGISTER:
			return _MTADAC_SetAMPRegister(arg);
        case MTAL_IO_ADAC_SETARCENABLE:
			return _MTADAC_SetArcEnable(arg);
		default:
		    return MTR_NOT_SUPPORTED;
	}
	return MTR_OK;
}
