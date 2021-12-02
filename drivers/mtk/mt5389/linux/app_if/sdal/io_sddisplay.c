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
 * $RCSfile: io_sddisplay.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sddisplay.c
 *  SDAL SdDisplay module ioctl interface of MT538X.
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "SDAL.h"
#include <linux/sdal_ioctl.h>
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "osd_drvif.h"
#include "pmx_drvif.h"
#include "sv_const.h"
#include "sd_vdopath.h"
#include "api_ycproc.h"
#include "drv_vdoclk.h"	//for DDDS_GetSpreadSpectrum
#include "x_ckgen.h" 	//for BSP_GetDomainClock

static SdDisplay_Settings_t  sdal_display_setting;

#define RGB_GAIN_OFFSET_DISABLE 0

static VDP_UI_VQ_MIN_MAX_DFT_T sdal_display_MinMaxDftDefault[EVDP_UVQI_MAX] =
{
    // min   max   dft
    {   0,  100,    45},      // EVDP_UVQI_BRIGHTNESS
    {   0,  100,    100},      // EVDP_UVQI_CONTRAST
    {   0,  100,    50},      // EVDP_UVQI_HUE , S/S range ***
    {   0,  100,    55},      // EVDP_UVQI_SATURATION
    {   0,    3,     2},      // EVDP_UVQI_CTI
    {   0,    3,     2},      // EVDP_UVQI_ETI
    {   0,    100,  50},      // EVDP_UVQI_SHARPNESS
    {   0,  256,   128},      // EVDP_UVQI_COLOR_GAIN	***
    {   0,  1024,   512},      // EVDP_UVQI_COLOR_OFFSET ***
    {   0,  100,    50},      // EVDP_UVQI_COLOR_SUPPRESS
    {   0,    3,     2},      // EVDP_UVQI_NR
    {   0,    1,     1},      // EVDP_UVQI_BLACK_LVL_EXT
    {   0,    1,     1},      // EVDP_UVQI_WHITE_PEAK_LMT
    {   0,    1,     1},      // EVDP_UVQI_FLESH_TONE
    {   0,    1,     1},      // EVDP_UVQI_LUMA
    {   0,   10,   5},      // EVDP_UVQI_BACK_LIGHT_LVL
    {   0,    2,     0},      // EVDP_UVQI_ADAPTIVEBACK_LIGHT
    {   0,    3,     2},      // EVDP_UVQI_3D_NR
};

static UINT8 _SdDisplay_MChgDone;
void _SdDisplay_NotifyHandleSigChg(UINT8 u1Path,UINT8 u1SigState,UINT8 u1ReportState)
{
	_SdDisplay_MChgDone=1;

}

static SdResult _SdDisplay_Init(SdPowerState_k ePowerState)
{
	int i,j;
	for(i=0;i<EVDP_VIS_MAX;i++)
		for(j=0;j<EVDP_UVQI_MAX;j++)
			VDP_SetUiVqMinMaxDft(i,j,&sdal_display_MinMaxDftDefault[j]);

	_SdDisplay_NotifyHandleSigChg(0,SV_VDO_STABLE,0);
	_SdDisplay_NotifyHandleSigChg(1,SV_VDO_STABLE,0);

	Sd_VdoPathVdpEnable(0, 1, SDDISPLAY);
	Sd_VdoPathVdpEnable(1, 1, SDDISPLAY);

	// SS want to control PE setting, instead of always set it in modechg done.
	vApiSetPEAtModeChgDone(SV_OFF);

	return SD_OK;
}

static void _SdDisplay_SetVDP(VDP_ARG_TYPE ucType, uint32_t u4NewValue, uint32_t u4OldValue)
{
	if(u4NewValue != u4OldValue)
	{
		VDP_SetArg(SV_VP_MAIN, ucType, u4NewValue);
		VDP_SetArg(SV_VP_PIP, ucType, u4NewValue);		
		printk("Set VDP %d to %d from %d\n", ucType, u4NewValue, u4OldValue);
	}
}

static void _SdDisplay_CheckRange(uint32_t *pu4Value, uint32_t u4Min, uint32_t u4Max)
{
	*pu4Value = (*pu4Value > u4Max? u4Max : *pu4Value);
	*pu4Value = (*pu4Value < u4Min? u4Min : *pu4Value);
}

static SdResult _SdDisplay_Set(SdDisplay_Settings_t *pSettings)
{
	UINT32 tmp;
	SdDisplay_Settings_t  setting_old;	
	
	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)pSettings,  sizeof(SdDisplay_Settings_t) ))
	{
		printk("_SdDisplay_Set arg access error\n");
		return SD_ERR_SYS;
	}

	x_memcpy(&setting_old,&sdal_display_setting,sizeof(SdDisplay_Settings_t));

	// Copy user space argument
	if(copy_from_user(&sdal_display_setting, (void __user *)pSettings,
	       sizeof(SdDisplay_Settings_t)))
	{
		printk("_SdDisplay_Set copy_from_user error\n");
		return SD_ERR_SYS;
	}

	// Brightness 0~100
	_SdDisplay_CheckRange(&sdal_display_setting.sPictureMode.brightness, 0, 100);
	_SdDisplay_SetVDP(VDP_ARG_BRIGHTNESS, sdal_display_setting.sPictureMode.brightness, setting_old.sPictureMode.brightness);	
	// Contrast 0~100
	_SdDisplay_CheckRange(&sdal_display_setting.sPictureMode.contrast, 0, 100);	
	_SdDisplay_SetVDP(VDP_ARG_CONTRAST, sdal_display_setting.sPictureMode.contrast, setting_old.sPictureMode.contrast);		
	// Tint 0~100 (50)
	_SdDisplay_CheckRange(&sdal_display_setting.sPictureMode.tint, 0, 100);	
	_SdDisplay_SetVDP(VDP_ARG_HUE, sdal_display_setting.sPictureMode.tint, setting_old.sPictureMode.tint);	
	// Color 0~100
	_SdDisplay_CheckRange(&sdal_display_setting.sPictureMode.color, 0, 100);	
	_SdDisplay_SetVDP(VDP_ARG_SATURATION, sdal_display_setting.sPictureMode.color, setting_old.sPictureMode.color);		
	// Sharpness 0~100
	_SdDisplay_CheckRange(&sdal_display_setting.sPictureMode.sharpness, 0, 100);	
	_SdDisplay_SetVDP(VDP_ARG_SHARPNESS, sdal_display_setting.sPictureMode.sharpness, setting_old.sPictureMode.sharpness);	

	// sWhiteBalance gain 0~50  mapping to 0~256
	_SdDisplay_CheckRange(&sdal_display_setting.sWhiteBalance.rGain, 0, 50);	
	_SdDisplay_CheckRange(&sdal_display_setting.sWhiteBalance.gGain, 0, 50);
	_SdDisplay_CheckRange(&sdal_display_setting.sWhiteBalance.bGain, 0, 50);	
	_SdDisplay_SetVDP(VDP_ARG_R_GAIN, (sdal_display_setting.sWhiteBalance.rGain*256)/50, (setting_old.sWhiteBalance.rGain*256)/50);
	_SdDisplay_SetVDP(VDP_ARG_G_GAIN, (sdal_display_setting.sWhiteBalance.gGain*256)/50, (setting_old.sWhiteBalance.rGain*256)/50);
	_SdDisplay_SetVDP(VDP_ARG_B_GAIN, (sdal_display_setting.sWhiteBalance.bGain*256)/50, (setting_old.sWhiteBalance.rGain*256)/50);

	// sWhiteBalance offset 0~50  mapping to 0~1024
	_SdDisplay_CheckRange(&sdal_display_setting.sWhiteBalance.rOffset, 0, 50);	
	_SdDisplay_CheckRange(&sdal_display_setting.sWhiteBalance.gOffset, 0, 50);
	_SdDisplay_CheckRange(&sdal_display_setting.sWhiteBalance.bOffset, 0, 50);	
	_SdDisplay_SetVDP(VDP_ARG_R_OFFSET, (sdal_display_setting.sWhiteBalance.rOffset*1024)/50, (setting_old.sWhiteBalance.rOffset*1024)/50);
	_SdDisplay_SetVDP(VDP_ARG_G_OFFSET, (sdal_display_setting.sWhiteBalance.gOffset*1024)/50, (setting_old.sWhiteBalance.gOffset*1024)/50);
	_SdDisplay_SetVDP(VDP_ARG_B_OFFSET, (sdal_display_setting.sWhiteBalance.bOffset*1024)/50, (setting_old.sWhiteBalance.bOffset*1024)/50);
		
	// ColorSpace
	/*
	sdal_display_setting.sColorSettings.eColorSpace =
	SD_COLORSPACE_AUTO,
	SD_COLORSPACE_WIDE,
	SD_COLORSPACE_XVYCC,
	SD_COLORSPACE_NATIVE,
	SD_COLORSPACE_CUSTOM,
	*/
	
	// DigitalNR 0~4
	_SdDisplay_CheckRange(&sdal_display_setting.DigitalNR, 0, 4);		
	_SdDisplay_SetVDP(VDP_ARG_NR, sdal_display_setting.DigitalNR, setting_old.DigitalNR);			
	_SdDisplay_SetVDP(VDP_ARG_3D_NR, sdal_display_setting.DigitalNR, setting_old.DigitalNR);		
	
	// BlackAdjust 0~3
		// Not support
		
	// EnergySaving 0~3
		// Not support
		
	// DynamicContrast 0~3
	_SdDisplay_CheckRange(&sdal_display_setting.DynamicContrast, 0, 3);			
	_SdDisplay_SetVDP(VDP_ARG_ADAPTIVE_LUMA, sdal_display_setting.DynamicContrast, setting_old.DynamicContrast);
	 
	
	// EdgeEnhancement 0~1
	_SdDisplay_CheckRange(&sdal_display_setting.EdgeEnhancement, 0, 1);		
	_SdDisplay_SetVDP(VDP_ARG_LTI, sdal_display_setting.EdgeEnhancement, setting_old.EdgeEnhancement);

	// bActiveColorOn
		// Not Support
	
	// Gamma
	_SdDisplay_CheckRange(&sdal_display_setting.gamma, 0, 7);		
	if((setting_old.gamma!=sdal_display_setting.gamma))
	{
		//VDP_SetArg(u1Path,VDP_ARG_GAMMA, sdal_display_setting.gamma);// seems not work in this api	
		// SDAL gamma ranges 0~7, and MTK gamma ranges 0~3		
		vApiVideoGammaLoadTable(sdal_display_setting.gamma >> 1);
	}
	
	// Backlight
	_SdDisplay_CheckRange(&sdal_display_setting.backlight, 0, 10);		
	_SdDisplay_SetVDP(VDP_ARG_BACK_LIGHT_LVL, sdal_display_setting.backlight, setting_old.backlight);

	// Background Color			
	if(_SdDisplay_MChgDone ||
		((setting_old.sBackColor.red!=sdal_display_setting.sBackColor.red)||
		(setting_old.sBackColor.green!=sdal_display_setting.sBackColor.green)||
		(setting_old.sBackColor.blue!=sdal_display_setting.sBackColor.blue)))
	{
		tmp=(sdal_display_setting.sBackColor.red&0xff)<<16;
		tmp|=(sdal_display_setting.sBackColor.green&0xff)<<8;
		tmp|=(sdal_display_setting.sBackColor.blue&0xff);
		PMX_SetBg(PMX_1,tmp);
	 }
	
	// bxvYCCOn TRUE/FALSE	
	if(_SdDisplay_MChgDone||(setting_old.bxvYCCOn!=sdal_display_setting.bxvYCCOn))
	{
		vApixvYccBypass(sdal_display_setting.bxvYCCOn);
		OSD_BASE_SetYuvOutput(sdal_display_setting.bxvYCCOn);
		PMX_SetYuvMode(sdal_display_setting.bxvYCCOn);
		Sd_VdoPath_BgColorMode(sdal_display_setting.bxvYCCOn);
		Sd_VdoPath_BgColorUpdate(SD_VIDEO0);
		Sd_VdoPath_BgColorUpdate(SD_VIDEO1);
	}

	// bSpreadSpecOn
	if(_SdDisplay_MChgDone ||
		(setting_old.bSpreadSpecOn!=sdal_display_setting.bSpreadSpecOn) ||
		(setting_old.period!=sdal_display_setting.period) ||
		(setting_old.range!=sdal_display_setting.range))
	{
		if(sdal_display_setting.bSpreadSpecOn)
		{
			DDDS_SetSpreadSpectrum(sdal_display_setting.period/1000, sdal_display_setting.range/1000);
		}
		else
		{
			DDDS_SetSpreadSpectrum(sdal_display_setting.period/1000, 0);
		}
	}

	// Pixel Shift
	if(_SdDisplay_MChgDone ||
		(setting_old.bPixelShiftOn!=sdal_display_setting.bPixelShiftOn) ||
		(setting_old.HPixelShift!=sdal_display_setting.HPixelShift) ||
		(setting_old.VPixelShift!=sdal_display_setting.VPixelShift))
	{
		OSTG_SetPixelShift(sdal_display_setting.bPixelShiftOn,
		              sdal_display_setting.HPixelShift,
		              sdal_display_setting.VPixelShift);
	}
	
	_SdDisplay_MChgDone=0;

/*
		printk("version = 1.1");
        printk("sdal_display_setting.sPictureMode.brightness = %d\n", sdal_display_setting.sPictureMode.brightness);
        printk("sdal_display_setting.sPictureMode.contrast = %d\n", sdal_display_setting.sPictureMode.contrast);
        printk("sdal_display_setting.sPictureMode.tint= %d\n", sdal_display_setting.sPictureMode.tint);
        printk("sdal_display_setting.sPictureMode.color = %d\n", sdal_display_setting.sPictureMode.color);
        printk("sdal_display_setting.sPictureMode.sharpness = %d\n", sdal_display_setting.sPictureMode.sharpness);
        printk("sdal_display_setting.backlight = %d\n", sdal_display_setting.backlight);
        printk("sdal_display_setting.DigitalNR = %d\n", sdal_display_setting.DigitalNR);
        printk("sdal_display_setting.EdgeEnhancement = %d\n", sdal_display_setting.EdgeEnhancement);
*/

	return SD_OK;
}

static SdResult _SdDisplay_Get(SdDisplay_Settings_t *pSettings)
{
	    // Check access of user space
	    if (!access_ok(VERIFY_WRITE, (void __user *)pSettings,  sizeof(SdDisplay_Settings_t) ))
	    {
		printk("_SdDisplay_Get arg access error\n");
		return SD_ERR_SYS;
	    }

	    // Copy user space argument
	    if(copy_to_user((void __user *)pSettings, &sdal_display_setting,
		       sizeof(SdDisplay_Settings_t)))
	    {
		printk("_SdDisplay_Get copy_to_user error\n");
		return SD_ERR_SYS;
	    }
	    return SD_OK;
}


static SdResult _SdDisplay_Mute(SdBool_t bMuteOn, SdBool_t bOsdMuteOn)
{
	UCHAR vdp_enable,osd_enable;
	vdp_enable=bMuteOn?0:1;
	osd_enable=bOsdMuteOn?0:1;
	Sd_VdoPathVdpEnable(0, vdp_enable, SDDISPLAY);
	Sd_VdoPathVdpEnable(1, vdp_enable, SDDISPLAY);
	OSD_PLA_Enable(0, osd_enable);
	OSD_PLA_Enable(1, osd_enable);
	OSD_PLA_Enable(2, osd_enable);
	return SD_OK;
}

static SdResult _SdDisplay_CaptureScreen(uint8_t* pDestination, SdDisplay_CaptureInfo_t *pCaptureInfo)
{
// SDAL_FIXME
    return SD_NOT_SUPPORTED;
}

static void _SdDisplay_Debug(void)
{
}

int sdal_ioctl_sddisplay(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
	SDAL_IOCTL_4ARG_T rArg12;	//current use SDAL_IOCTL_4ARG_T instead of SDAL_IOCTL_12ARG_T

	if (_IOC_SIZE(cmd)>sizeof(unsigned long))
	{
	    // Check access of user space
	    if (!access_ok(VERIFY_READ, (void __user *)arg,  _IOC_SIZE(cmd) ))
	    {
		printk("sdal_ioctl_sddisplay arg access error\n");
		return SD_ERR_SYS;
	    }
	    // Copy user space argument
	    if(copy_from_user(&rArg12, (void __user *)arg,
		       _IOC_SIZE(cmd)))
	    {
		printk("sdal_ioctl_sddisplay copy_from_user error\n");
		return SD_ERR_SYS;
	    }
	}

	switch (cmd) {
		case SDAL_IO_DISPLAY_INIT:
			return _SdDisplay_Init((SdPowerState_k)arg);
		case SDAL_IO_DISPLAY_SET:
			return _SdDisplay_Set((SdDisplay_Settings_t *)arg);
		case SDAL_IO_DISPLAY_GET:
			return _SdDisplay_Get((SdDisplay_Settings_t *)arg);
		case SDAL_IO_DISPLAY_MUTE:
			return _SdDisplay_Mute((SdBool_t)rArg12.au4Arg[0],(SdBool_t)rArg12.au4Arg[1]);
		case SDAL_IO_DISPLAY_CAPTURESCREEN:
			return _SdDisplay_CaptureScreen((uint8_t*)rArg12.au4Arg[0], (SdDisplay_CaptureInfo_t *)rArg12.au4Arg[1]);
		case SDAL_IO_DISPLAY_DEBUG:
			_SdDisplay_Debug();
			break;
		default:
	        		return SD_NOT_SUPPORTED;
	}
	return SD_OK;
}

