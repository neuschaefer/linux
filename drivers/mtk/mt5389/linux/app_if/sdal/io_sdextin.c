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
 * $RCSfile: io_sdextin.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdextin.c
 *  SDAL SdExtin module ioctl interface of MT538X.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>

#include "sd_vdopath.h"
#include "SdType.h"

#include "vdp_if.h"
#include "sv_const.h"
#include "nptv_if.h"
#include "source_table.h"
#include "drv_async.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "drv_video.h"
#include "drv_tvd.h"
#include "drv_scart.h"
#include "video_def.h"
#include "drv_adcset.h"
#include "sd_audio.h"
#include "eeprom_if.h"	// HDMI_ADDITIONAL_API
#include "drv_autocolor.h"
#include "hw_ycproc.h"
#include "drv_async.h"
#include "pmx_if.h"
#include "pmx_drvif.h"
#include "x_timer.h"
#include "util.h"
#include "vdo_misc.h"
#include "api_eep.h"
#include "eepdef.h"

SdPCAdjustInfo_t sdal_pcsetting;

static uint32_t _SdExtIn_HdmiInfoFrameChgMask;
#define CHECK_ATV_VLOCK 1
#define SDAL_HV_POSITION 1

	// SDAL_FIXME
	// Currently, there is no AP. 
#define HDMI_FAST_CONNECT 0
static UINT8 _bPCSetFirstTime;

void _SdExtIn_NotifyHandleSigChg(UINT8 u1Path,UINT8 u1SigState,UINT8 u1ReportState)
{
	int id;
	if(u1SigState==SV_VDO_STABLE)
	{
		if(((u1Path==0)&&fgIsMainDVI())||((u1Path==1)&&fgIsPipDVI()))
		{
			_SdExtIn_HdmiInfoFrameChgMask=0;//clear & wait get status
		}
		if(((u1Path==0)&&fgIsMainVga())||((u1Path==1)&&fgIsPipVga()))
		{
			_bPCSetFirstTime = TRUE;
		}
		for(id=0;id<SD_VIDEO_MAX;id++)
		{
			if(sdal_extin[id].vpath_assigned&&(sdal_extin[id].video_mod->out_id==u1Path))
			{
				sdal_extin[id].setting.bHDMIBlackLow=0;//clear
			}
		}
	}
}


void Sd_ExtInGrabPcSetting(void)
{
	INT32 vpos,vmin_off;
	vpos=wDrvVGAGetVPosition();
	vmin_off=vpos-wDrvVGAVPositionMin();
	if (vmin_off<0)
	{
		vmin_off=0;
	}

	sdal_pcsetting.frequency = wDrvVGAGetClock();
	sdal_pcsetting.phase = u1DrvVGAGetPhase() ;
	sdal_pcsetting.sPCPosition.x = vMwGetHPosition() ;
	sdal_pcsetting.sPCPosition.y = wDrvVGAVPositionMax()-vmin_off; 		
	sdal_pcsetting.sPCPosition.w = wVgaInputWidth() ;				
	sdal_pcsetting.sPCPosition.h = wVgaInputHeight() ;	
}

void Sd_ExtInSetPcSetting(SdPCAdjustInfo_t * setting)
{
	memcpy(&sdal_pcsetting,setting,sizeof(SdPCAdjustInfo_t));
}


static SdResult _SdExtIn_Init(SdPowerState_k ePowerState)
{
	UINT8 i;
	SdVPath_MutexLock();
#if HDMI_FAST_CONNECT
	vHDMIInit();
#endif
	vHDMIHPDIndepCtrl(1); //modify by ciwu
#if HDMI_FAST_CONNECT
	vHDMISetSwitch(1);
	_bHDMIState = HDMI_STATE_AUTH;
#endif
	for(i=0; i<2; i++)
	{
		sdal_extin[i].module_id=i;			
		sdal_extin[i].be_id=SD_BE_IN_EXTIN0+i;
		
		Sd_VdoPathDisconnect(0,0,SD_BE_IN_EXTIN0+i);

		sdal_extin[i].fe_connected=0;
		sdal_extin[i].fe_src=0;
		sdal_extin[i].vpath_assigned=0;
		sdal_extin[i].video_mod=0;		

 		x_memset(&sdal_extin[i].setting,0,sizeof(SdExtIn_Settings_t));
		// similar setting as original checking condition.
		sdal_extin[i].setting.sLockCheck.VSyncCount = 9;
		sdal_extin[i].setting.sLockCheck.checkTime  = 600; // ms
	}
	SdVPath_MutexUnLock();
	return SD_OK;
}

static SdResult _SdExtIn_Disconnect(SdExtInID_k eExtInId)
{
	if (eExtInId>SD_EXTIN_MAX)
		return SD_ERR_INV;

	if(!sdal_extin[eExtInId].fe_connected)
	{
		return SD_NOT_OK;
	}

	SdVPath_MutexLock();
	Sd_VdoPathDisconnect(0, 0 , eExtInId+SD_BE_IN_EXTIN0);
	SdVPath_MutexUnLock();
	return SD_OK;
}

static SdResult _SdExtIn_Connect(SdExtInID_k eExtInId, SdSource_k eInputSrc, SdVideoID_k eOutVideoId)
{
	if (eExtInId>SD_EXTIN_MAX)
		return SD_ERR_INV;
	if(sdal_extin[eExtInId].fe_connected)
	{
		//_SdExtIn_Disconnect(eExtInId);
		return SD_NOT_OK;
	}
	
	SdVPath_MutexLock();
	sdal_extin[eExtInId].fe_connected=1;
	sdal_extin[eExtInId].fe_src=eInputSrc;
	Sd_VdoPathConnect(0, eOutVideoId , eExtInId+SD_BE_IN_EXTIN0);
	SdVPath_MutexUnLock();
	return SD_OK;
}

static SdResult _SdExtIn_PcGet(SdExtInID_k eExtInId, SdPCAdjustInfo_t *pSettings)
{
	if(!(fgIsMainVga() || fgIsPipVga()))
	{
		return SD_NOT_OK;
	}
	
	// Check access of user space
	if (!access_ok(VERIFY_WRITE, (void __user *)pSettings,
				   sizeof(SdPCAdjustInfo_t)))
	{
		printk("SdExtIn_PCGet argument error\n");
		return SD_NOT_OK;
	}

	// Get the PC status 
	Sd_ExtInGrabPcSetting();	
	
	if (copy_to_user((void __user *)pSettings, &sdal_pcsetting,
					 sizeof(SdPCAdjustInfo_t)))
	{
		printk("SDAL_IO_EXTIN_PCGET copy to user error\n");
		return SD_NOT_OK;
	}		


	return SD_OK;

}

static SdResult _SdExtIn_PcSet(SdExtInID_k eExtInId, SdPCAdjustInfo_t *pSettings)
{
	SdPCAdjustInfo_t 	sPcSettings ;

	if(!(fgIsMainVga() || fgIsPipVga()))
	{
		return SD_NOT_OK;
	}

	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)pSettings,
				   sizeof(SdPCAdjustInfo_t)))
	{
		printk("SdExtIn_PCSet  argument error\n");
		return SD_NOT_OK;
	}

	// Copy user space argument
	if (copy_from_user(&sPcSettings, (void __user *)pSettings,
					   sizeof(SdPCAdjustInfo_t)))
	{
		printk("SdExtIn_PCSet  argument error\n");
		return SD_NOT_OK;
	}		
#if 0	
	 printk("freq %d phsae %d\n", sPcSettings.frequency, sPcSettings.phase);
	 printk("x %d y %d w %d h %d \n", sPcSettings.sPCPosition.x, sPcSettings.sPCPosition.y, sPcSettings.sPCPosition.w, sPcSettings.sPCPosition.h);
#endif	 
	 //to do something

	if(!vMwSetHPosition(0, sPcSettings.sPCPosition.x))
	{
		printk("SdExtIn_PCSet  set hpos error\n");
		return SD_NOT_OK;
	}	 

	{
		/* v-position adjust */
		INT32 pos,min,max,min_off;
		min=wDrvVGAVPositionMin();
		max=wDrvVGAVPositionMax();

		min_off=sPcSettings.sPCPosition.y-min;
		pos=max-min_off;
		if (pos<min)
		{
			pos=min;
		}
		if (pos>max)
		{
			pos=max;
		}
		if(!vMwSetVPosition(0,pos))
		{
			printk("SdExtIn_PCSet  set vpos error\n");
			return SD_NOT_OK;
		}
	}
	
	if (sPcSettings.frequency != wDrvVGAGetClock())
	{
		if(!fgApiVideoVgaSetClock(sPcSettings.frequency))
		{
			printk("SdExtIn_PCSet  set coarse error\n");
			return SD_NOT_OK;
		}
	}

	//if (sPcSettings.phase != u1DrvVGAGetPhase())
	{
		vDrvVGASetPhase(sPcSettings.phase & 0x1f);
	}
	
	 Sd_ExtInSetPcSetting(&sPcSettings);

	 if(_bPCSetFirstTime == TRUE)
	 {
		 if ((eExtInId <= SD_EXTIN1) &&
			(sdal_extin[eExtInId].vpath_assigned) &&
			(sdal_extin[eExtInId].video_mod != NULL))
		 {
		 	_bPCSetFirstTime = FALSE;
			 _VDP_SetScposDelayUnMute(sdal_extin[eExtInId].video_mod->out_id, 3, 1);
		 }	 
	 }
	 return SD_OK;

}
extern UINT16   _wDviHclk;

static void vGetResolutionInfo(SdExtInID_k eExtInId, SdExtIn_ResolutionInfo_t *sResolutionInfo)
{
	UINT8 bType, bPath;
	UINT16 wUiHResolution;
	if((eExtInId>SD_EXTIN1)||(!sdal_extin[eExtInId].vpath_assigned))
		return SD_NOT_OK;

	bPath=sdal_extin[eExtInId].video_mod->out_id;
	
	bType = bGetVideoDecType(bPath);
	switch (bType)
	{
		case SV_VD_TVD3D:
			sResolutionInfo->bHSyncPositive = FALSE ;
			sResolutionInfo->bVSyncPositive = FALSE ;	
			sResolutionInfo->hFreq = 15700;			
			break;
					 
		case SV_VD_YPBPR:
		case SV_VD_VGA: 		
			if(bType == SV_VD_YPBPR)
			{
				sResolutionInfo->bHSyncPositive = FALSE ;
				sResolutionInfo->bVSyncPositive = FALSE ;	
			}
			else
			{
				sResolutionInfo->bHSyncPositive = fgSP0Hpol ;
				sResolutionInfo->bVSyncPositive = fgSP0Vpol ;	
			}	
			sResolutionInfo->hFreq = wSP0Hclk * 100; 			
			   break;
					   
		case SV_VD_DVI:
			sResolutionInfo->bHSyncPositive = fgHDMIHsyncPolarity() ;
			sResolutionInfo->bVSyncPositive = fgHDMIVsyncPolarity()	;		
			sResolutionInfo->hFreq = _wDviHclk * 100;	
			   break;
		default:
			break;
	}
	sResolutionInfo->hResolution = wApiVideoGetInputWidth(bPath); //wDrvVideoInputWidth(bPath);
	sResolutionInfo->vResolution = wDrvVideoInputHeight(bPath);
	sResolutionInfo->bProgressScan = !bDrvVideoIsSrcInterlace(bPath);

	sResolutionInfo->hTotal = wDrvVideoGetHTotal(bPath);
	wUiHResolution=wApiVideoGetInputWidth(bPath);
	if(wUiHResolution&&((wDrvVideoInputWidth(bPath)/wUiHResolution)==2))
	{
		//oversample timing for ypbpr or vga
		if(sResolutionInfo->bProgressScan)
		{	//480p/576p  or other p mode
			sResolutionInfo->hTotal /= 2;
		}
		else
		{	//480i/576i
			sResolutionInfo->hTotal /= 4;
		}
	}
	
	sResolutionInfo->vTotal =	wDrvVideoGetVTotal(bPath); 
	sResolutionInfo->vFreq = bDrvVideoGetRefreshRate(bPath) *1000;	
	
	//to do hstart/vstart
#if SDAL_HV_POSITION
	sResolutionInfo->hStart=wDrvVideoGetPorch(bPath,SV_HPORCH_DEFAULT);
	sResolutionInfo->vStart=wDrvVideoGetPorch(bPath,SV_VPORCH_DEFAULT);
#endif

}


static  SdColorFormat_k  _SdExtInColorMap[]=
{
		SD_COLOR_FORMAT_RGB,
		SD_COLOR_FORMAT_YCbCr422,
		SD_COLOR_FORMAT_YCbCr422,
		SD_COLOR_FORMAT_YCbCr444,
		SD_COLOR_FORMAT_YCbCr444,
		SD_COLOR_FORMAT_XVYCC,
		SD_COLOR_FORMAT_XVYCC
};

const UINT8 CORSYS_MAP[2][9]={{SV_CS_AUTO, SV_CS_PAL, SV_CS_PAL_60, SV_CS_PAL_M, 
			SV_CS_PAL_N, SV_CS_SECAM, SV_CS_NTSC358, SV_CS_NTSC443, SV_CS_MAX},
			{SD_EXTIN_COR_SYS_PAL_N, SD_EXTIN_COR_SYS_PAL, SD_EXTIN_COR_SYS_PAL_M, SD_EXTIN_COR_SYS_NT358, 
			SD_EXTIN_COR_SYS_SECAM, SD_EXTIN_COR_SYS_PAL60, SD_EXTIN_COR_SYS_NT443, SD_EXTIN_COR_SYS_AUTO, SD_EXTIN_COR_SYS_MAX}};

static HAL_TIME_T _checkVLockStart;
static HAL_TIME_T _checkVLockEnd;
static SdResult _SdExtIn_GetStatus(SdExtInID_k eExtInId, SdMask_t mask, SdExtIn_Status_t *pStatus, uint32_t timeout)
{
	SdExtIn_Status_t sStatus ;	
	UINT8 bPath;
	UINT8 colsys;
	
	if(eExtInId>SD_EXTIN1)
		return SD_NOT_OK;
	
	bPath=(sdal_extin[eExtInId].vpath_assigned)?sdal_extin[eExtInId].video_mod->out_id:SV_VP_MAIN;


	//printk("kr:eExtInId %d\n", eExtInId);
	//printk("kr:mask %d\n", mask);	
	// Check access of user space
	if (!access_ok(VERIFY_WRITE, (void __user *)pStatus,
				   sizeof(SdExtIn_Status_t)))
	{
		printk("_SdExtIn_GetStatus argument error\n");
		return SD_NOT_OK;
	}

	memset(&sStatus, 0, sizeof(SdExtIn_Status_t));
	if(mask &  SD_EXTIN_STATUS_RES)
	{
		vGetResolutionInfo(eExtInId, & sStatus.sResolutionInfo);	
	}

	if(mask &  SD_EXTIN_STATUS_SYNC_LOCK)
	{
		UINT32 bSigStauts = bDrvVideoSignalStatus(bPath);
#if CHECK_ATV_VLOCK
		if(((sdal_extin[eExtInId].fe_connected==1) && (sdal_extin[eExtInId].fe_src==SD_SOURCE_ATV1) && (bSigStauts != SV_VDO_STABLE)) || 
		   (sdal_extin[eExtInId].fe_connected==0))
		{
#if PD_ADC_ENABLE
			if(sdal_extin[eExtInId].fe_connected==0  && !fgIsDrvCvbsPwOn()) // not ATV sources 
			{
				vDrvCvbsAdcPWON();	
				vDrvCvbsVfePWON();
				vDrvSetTVMux(SV_VS_AT1, SV_VS_MAX);
				vUtDelay1ms(200);
				vDrvCvbsPwOnStart();
			}
#else
			if(vDrvSetTVMux(SV_VS_AT1, SV_VS_MAX))//Mux change
			{
				vUtDelay1ms(20);				
			}
#endif
			sStatus.bSyncLock = fgHwTvdVPres();
		}
		else //Other sources
#endif
		{			
			
			if(sdal_extin[eExtInId].vpath_assigned)
			{
				sStatus.bSyncLock = (bSigStauts == SV_VDO_STABLE)? TRUE:FALSE ;
			}
			else
			{
				sStatus.bSyncLock = FALSE;
			}
			
		}
	}
	
	if(mask &  SD_EXTIN_STATUS_MACRO)
	{
		UINT8 bType ;
		bType = bGetVideoDecType(bPath);
		switch (bType)
		{
			case SV_VD_TVD3D:
				if(fgHwTvdIsMacroVision())
				{
					if(fgHwTvdIsPSync())
					{
						sStatus.sMvType.MV_PSP = 1 ;
					}
					else
					{	
						if(fgHwTvdIsMvType2())
						{
							sStatus.sMvType.MV_ColourStrip2= 1 ;						
						}
						else
						{
							sStatus.sMvType.MV_ColourStrip3= 1 ;					
						}
					}
				}	
				break;
						 
			case SV_VD_YPBPR:
			case SV_VD_VGA:		
				//can't tell the mv type
				sStatus.sMvType.MV_PSP = bDrvAsyncMvStatus();				
				   break;
			default:
				break;
		}


	}

	if(mask &  SD_EXTIN_STATUS_HDMI)
	{
			if(fgIsExtSrcHDMI())
			{
				UINT16 bReadData;
				sStatus.sHdmiStatus.bHdmiEnable = _bHdmiMode ;
				//printk("bHdmiEnable =%d \n",sStatus.sHdmiStatus.bHdmiEnable);
				switch(bHDMIDeepColorStatus())
				{
					default:
					case 0:	sStatus.sHdmiStatus.eHdmiColorDepth=0; break;				
					case 1:	sStatus.sHdmiStatus.eHdmiColorDepth=1; break;
					case 2:	sStatus.sHdmiStatus.eHdmiColorDepth=2; break;
					case 3:	sStatus.sHdmiStatus.eHdmiColorDepth=3; break;				
				}
				_SdExtIn_HdmiInfoFrameChgMask|=wHDMIInfoFrameMask();
				
				if(_bHdmiMode)
				{
					if((bHDMIColorFormat(0)<SV_COLOR_HDMI_RGB)||(bHDMIColorFormat(0)>SV_COLOR_HDMI_XVYCC_709))
					{
						sStatus.sHdmiStatus.eHdmiInputFormat =SD_COLOR_FORMAT_UNKNOWN;
					}
					else 
					{
						sStatus.sHdmiStatus.eHdmiInputFormat =
							_SdExtInColorMap[bHDMIColorFormat(0)-SV_COLOR_HDMI_RGB];
					}
					switch(bHDMIGetAudSampleFreq())
					{
						case AUD_FS_44K :
							sStatus.sHdmiStatus.ePcmFsRate = SD_AUDIO_FS_44KHZ  ;
							break;
						case AUD_FS_48K :
							sStatus.sHdmiStatus.ePcmFsRate = SD_AUDIO_FS_48KHZ  ;
							break;
						case AUD_FS_32K :
							sStatus.sHdmiStatus.ePcmFsRate = SD_AUDIO_FS_32KHZ  ;
							break;					
						case AUD_FS_88K :
							sStatus.sHdmiStatus.ePcmFsRate = SD_AUDIO_FS_88KHZ  ;
							break;	
						case AUD_FS_96K :
							sStatus.sHdmiStatus.ePcmFsRate = SD_AUDIO_FS_96KHZ  ;
							break;	
						case AUD_FS_176K :
							sStatus.sHdmiStatus.ePcmFsRate = SD_AUDIO_FS_176KHZ  ;
							break;	
						default:
							break;
					}
					sStatus.sHdmiStatus.eHdmiAudio = SD_AUDIO_FORMAT_PCM ;				
				}
				else
				{
					sStatus.sHdmiStatus.eHdmiInputFormat = SD_COLOR_FORMAT_RGB  ;
				}

				
			}
			else
			{
				sStatus.sHdmiStatus.eHdmiInputFormat = SD_COLOR_FORMAT_UNKNOWN ;
			}

		}
	
	
	if(mask &   SD_EXTIN_STATUS_NOISE)
	{
		sStatus.noiseValue=bTvdNRLevel();

	}

	if(mask & SD_EXTIN_STATUS_COR_SYS)
	{
		colsys = bApiVideoGetColorSystem();
		sStatus.eCorSys = CORSYS_MAP[1][colsys];
	}

#if 0	
	printk("kr:bSyncLock %d\n", sStatus.bSyncLock);
	printk("kr:noiseValue %d\n", sStatus.noiseValue);
	printk("kr:sMvType %d\n", sStatus.sMvType);	
	printk("kr:hsyncPol %d\n", sStatus.sResolutionInfo.bHSyncPositive);	
	printk("kr:vsyncPol %d\n", sStatus.sResolutionInfo.bVSyncPositive);		
	printk("kr:progressive %d\n", sStatus.sResolutionInfo.bProgressScan);			
	printk("kr:hTotal %d\n", sStatus.sResolutionInfo.hTotal);	
	printk("kr:vTotal %d\n", sStatus.sResolutionInfo.vTotal);		
	printk("kr:hResolution %d\n", sStatus.sResolutionInfo.hResolution);	
	printk("kr:vResolution %d\n", sStatus.sResolutionInfo.vResolution);		
#endif	

	if (copy_to_user((void __user *)pStatus, &sStatus,
					 sizeof(SdExtIn_Status_t)))
	{
		printk("_SdExtIn_GetStatu copy to user error\n");
		return SD_NOT_OK;
	}

	return SD_OK;
}

static SdResult _SdExtIn_Set(SdExtInID_k eExtInId, SdExtIn_Settings_t *pSettings)
{
	// Check access of user space
	SdExtIn_Settings_t  setting_old, *pSettings_old;
	if ((eExtInId>=SD_EXTIN_MAX)|| (!access_ok(VERIFY_READ, (void __user *)pSettings,
				   sizeof(SdExtIn_Settings_t))))
	{
		printk("_SdExtIn_Set  argument error\n");
		return SD_NOT_OK;
	}

	x_memcpy(&setting_old,&sdal_extin[eExtInId].setting,sizeof(SdExtIn_Settings_t));
	pSettings_old=&setting_old;

	// Copy user space argument
	if (copy_from_user(&sdal_extin[eExtInId].setting, (void __user *)pSettings,
					   sizeof(SdExtIn_Settings_t)))
	{
		printk("_SdExtIn_Set  argument error\n");
		return SD_NOT_OK;
	}		

	if((pSettings_old->sResolution.hStart!=sdal_extin[eExtInId].setting.sResolution.hStart)
		||(pSettings_old->sResolution.vStart!=sdal_extin[eExtInId].setting.sResolution.vStart))
	{
		if(sdal_extin[eExtInId].vpath_assigned&&sdal_extin[eExtInId].video_mod)
		{
#if SDAL_HV_POSITION
			wDrvVideoSetPorch(sdal_extin[eExtInId].video_mod->out_id,SV_HPORCH_CURRENT,
				sdal_extin[eExtInId].setting.sResolution.hStart);
			wDrvVideoSetPorch(sdal_extin[eExtInId].video_mod->out_id,SV_VPORCH_CURRENT,
				sdal_extin[eExtInId].setting.sResolution.vStart);
#else
			_SdVideo_NotifyExtInSrcRegionUpdate(sdal_extin[eExtInId].video_mod->module_id,eExtInId);
#endif
		}
	}
	
	if(((sdal_extin[eExtInId].fe_connected==1) && (sdal_extin[eExtInId].fe_src==SD_SOURCE_ATV1)) || 
		   (sdal_extin[eExtInId].fe_connected==0))
		{
			UINT32 VLockThres = sdal_extin[eExtInId].setting.sLockCheck.VSyncCount;
			UINT32 Period = sdal_extin[eExtInId].setting.sLockCheck.checkTime;

			UINT32 VLockTimer, VLockCnt=0;
#if PD_ADC_ENABLE
			if(sdal_extin[eExtInId].fe_connected==0  && !fgIsDrvCvbsPwOn() && VLockThres) // not ATV sources 
			{
				vDrvCvbsAdcPWON();	
				vDrvCvbsVfePWON();
				vDrvSetTVMux(SV_VS_AT1, SV_VS_MAX);
				vUtDelay1ms(200);
				vDrvCvbsPwOnStart();
			}
#else
			if(vDrvSetTVMux(SV_VS_AT1, SV_VS_MAX))//Mux change
			{
				vUtDelay1ms(20);				
			}
#endif
			HAL_GetTime(&_checkVLockStart);
			do
			{
				HAL_GetTime(&_checkVLockEnd);
				VLockTimer = 1000000*(_checkVLockEnd.u4Seconds - _checkVLockStart.u4Seconds) + 
				                                (_checkVLockEnd.u4Micros - _checkVLockStart.u4Micros);
				if (fgHwTvdVLock()) 
				{
					VLockCnt++;
					if(VLockCnt>=VLockThres)
					{
						break;
					}
				}
				VLockTimer/=1000; // us -> ms
				vUtDelay1ms(15);
			}while((VLockTimer<Period));			
		}
	 //to do something
	 /*
	 if(fgIsMainVga() || fgIsPipVga() || fgIsMainYPbPr() || fgIsPipYPbPr())
	 {
		 vCust_R_Gain_Update( sdal_extin[eExtInId].setting.sADCRGBGain.gainRed) ;
		 vCust_G_Gain_Update( sdal_extin[eExtInId].setting.sADCRGBGain.gainGreen) ;
		 vCust_B_Gain_Update( sdal_extin[eExtInId].setting.sADCRGBGain.gainBlue) ;	 
	 }*/


	if(pSettings_old->eCorSys!=sdal_extin[eExtInId].setting.eCorSys)	
	{
		fgApiVideoColorSystem(CORSYS_MAP[0][sdal_extin[eExtInId].setting.eCorSys]);
	}
	
	if(sdal_extin[eExtInId].vpath_assigned)
	{
		vApiHDMIBlackControl(sdal_extin[eExtInId].video_mod->out_id, sdal_extin[eExtInId].setting.bHDMIBlackLow); //from ycproc,mtk01140 20071019 for HDMI black level implement by 3x3
	}
	// SDAL_FIXME
	// Currently, there is no AP. We can NOT break player and HDMI driver connection
	 vHDMITMDSIndepCTRL(sdal_extin[eExtInId].setting.bHDMIClkEnable);//mark by ciwu
	 return SD_OK;
	 
}

static SdResult _SdExtIn_Get(SdExtInID_k eExtInId, SdExtIn_Settings_t *pSettings)
{
	SdExtIn_Settings_t  	sSettings ;
	UINT8 colsys;
	// Check access of user space
	if ((eExtInId>=SD_EXTIN_MAX)||(!access_ok(VERIFY_WRITE, (void __user *)pSettings,
				   sizeof(SdExtIn_Settings_t ))))
	{
		printk("_SdExtIn_Get argument error\n");
		return SD_NOT_OK;
	}

#if SDAL_HV_POSITION
	if(sdal_extin[eExtInId].vpath_assigned)
	{
		UINT8 bPath=sdal_extin[eExtInId].video_mod->out_id;
		sdal_extin[eExtInId].setting.sResolution.hStart=wDrvVideoGetPorch(bPath,SV_HPORCH_CURRENT);
		sdal_extin[eExtInId].setting.sResolution.vStart=wDrvVideoGetPorch(bPath,SV_VPORCH_CURRENT);
	}
#endif
	if(sdal_extin[eExtInId].vpath_assigned)
	{
		colsys = bApiVideoGetColorSystem();
		sdal_extin[eExtInId].setting.eCorSys = CORSYS_MAP[1][colsys];
	}
#if 0
	// Get  status 
	//memset(&sSettings, 0, sizeof(SdExtIn_Status_t)); LH:-O0 clear pSettings,too. strange
	if (copy_from_user( &sSettings,(void __user *)pSettings,
					 sizeof(SdExtIn_Settings_t )))
	{
		return SD_NOT_OK;
	}	
	
	//1.get resolution information 
	vGetResolutionInfo(eExtInId, & sSettings.sResolution);

	//3.get ADC RGB gain information
	if(fgIsMainVga() || fgIsPipVga() || fgIsMainYPbPr() || fgIsPipYPbPr())
	{
		sSettings.sADCRGBGain.gainRed= vCust_R_Gain_Read();		
		sSettings.sADCRGBGain.gainGreen= vCust_G_Gain_Read();	
		sSettings.sADCRGBGain.gainBlue = vCust_B_Gain_Read();
	}
	sSettings.bHDMIBlackLow=sdal_extin[eExtInId].setting.bHDMIBlackLow;
	sSettings.bHDMIClkEnable=(u1RegRead1B(PD_SYS_2)==0xdf)?1:0;
#if 0	
	printk("bgain %d\n", sSettings.sADCRGBGain.gainBlue);
	printk("ggain %d\n", sSettings.sADCRGBGain.gainGreen);	
	printk("rgain %d\n", sSettings.sADCRGBGain.gainRed);	
#endif
	
	if (copy_to_user((void __user *)pSettings, &sSettings,
					 sizeof(SdExtIn_Settings_t )))
	{
		printk("_SdExtIn_Get copy to user error\n");
		return SD_NOT_OK;
	}		
#else
	if (copy_to_user((void __user *)pSettings, &sdal_extin[eExtInId].setting,
					 sizeof(SdExtIn_Settings_t )))
	{
		printk("_SdExtIn_Get copy to user error\n");
		return SD_NOT_OK;
	}	
#endif

	return SD_OK;


}

static void _SdExtIn_Debug(void)
{
}


//begin HDMI_ADDITIONAL_API


#if 0
static SdResult _SdExtIn_GetHdmiInfoFrame(SdExtInID_k eExtInId, SdExtIn_HdmiInfoMask_k 
	eHdmiInfotype, SdExtIn_HdmiInfoFrame_t *pFrameData)
{
	switch(eHdmiInfotype)
	{
		/* change bitmask to which bit */
		case SD_HDMI_INFO_FRAME_AVI: eHdmiInfotype=0;break;
		case SD_HDMI_INFO_FRAME_SPD: eHdmiInfotype=1;break;
		case SD_HDMI_INFO_FRAME_AUDIO:eHdmiInfotype=2;break;
		case SD_HDMI_INFO_FRAME_MPEG:eHdmiInfotype=3;break;
		case SD_HDMI_INFO_FRAME_UNREC:eHdmiInfotype=4;break;
		case SD_HDMI_INFO_FRAME_GAMUT:eHdmiInfotype=5;break;
		default:
			return SD_NOT_OK;
	};
	
	if(fgIsExtSrcHDMI())//fgIsMainDVI()||fgIsPipDVI()
	{		
		// Check access of user space
		if (!access_ok(VERIFY_WRITE, (void __user *)pFrameData,
					   sizeof(SdExtIn_HdmiInfoFrame_t )))
		{
			printk("_SdExtIn_HDMIGet argument error\n");
			return SD_NOT_OK;
		}
		vHDMIGetInfoFrame(eHdmiInfotype);
		_SdExtIn_HdmiInfoFrameChgMask&=~(1<<eHdmiInfotype);
		if (copy_to_user((void __user *)pFrameData, wHDMIInfoFrame,
						 sizeof(SdExtIn_HdmiInfoFrame_t )))
		{
			printk("_SdExtIn_HDMIGet copy to user error\n");
			return SD_NOT_OK;
		}	
		return SD_OK;
	}
	return SD_NOT_OK;
}
#else
static SdResult _SdExtIn_GetHDMIStatus(SdExtInID_k eExtInId, SdMask_t mask, SdExtIn_HDMIStatus_t *pStatus, uint32_t timeout)
{
	SdExtIn_HDMIStatus_t  sStatus;
	// Check access of user space
	if (!access_ok(VERIFY_WRITE, (void __user *)pStatus,
				   sizeof(SdExtIn_HDMIStatus_t )))
	{
		printk("SdExtIn_HDMIStatus_t argument error\n");
		return SD_NOT_OK;
	}

	if((sdal_extin[eExtInId].fe_src<SD_SOURCE_HDMI1)||(sdal_extin[eExtInId].fe_src>SD_SOURCE_HDMI4)
		|| !sdal_extin[eExtInId].vpath_assigned)
		return SD_NOT_OK;

	
	if((mask>=SD_HDMI_INFO_FRAME_AVI)&&(mask<=SD_HDMI_INFO_FRAME_ACP))
		{
				mask-=1;
			if(!(_SdExtIn_HdmiInfoFrameChgMask&(1<<(mask))))
					return SD_ERR_TIMEDOUT;

				vHDMIGetInfoFrame(mask);
				//x_memcpy(sStatus.InfoFrame,wHDMIInfoFrame,sizeof(wHDMIInfoFrame));
				_SdExtIn_HdmiInfoFrameChgMask&=~(1<<mask);
		}
	else
		{
			if(fgIsExtSrcHDMI())
			{
				UINT16 bReadData;
				sStatus.bHdmiEnable = _bHdmiMode ;
				switch(bHDMIDeepColorStatus())
				{
					default:
					case 0:	sStatus.eHdmiColorDepth=0; break;				
					case 1:	sStatus.eHdmiColorDepth=1; break;
					case 2:	sStatus.eHdmiColorDepth=2; break;
					case 3:	sStatus.eHdmiColorDepth=3; break;				
				}
				_SdExtIn_HdmiInfoFrameChgMask|=wHDMIInfoFrameMask();
	//			sStatus.sHdmiStatus.HdmiInfoState=_SdExtIn_HdmiInfoFrameChgMask;
				
				if(_bHdmiMode)
				{
					if((bHDMIColorFormat(0)<SV_COLOR_HDMI_RGB)||(bHDMIColorFormat(0)>SV_COLOR_HDMI_XVYCC_709))
					{
						sStatus.eHdmiInputFormat =SD_COLOR_FORMAT_UNKNOWN;
					}
					else 
					{
						sStatus.eHdmiInputFormat =
							_SdExtInColorMap[bHDMIColorFormat(0)-SV_COLOR_HDMI_RGB];
					}
					switch(bHDMIGetAudSampleFreq())
					{
						case AUD_FS_44K :
							sStatus.ePcmFsRate = SD_AUDIO_FS_44KHZ  ;
							break;
						case AUD_FS_48K :
							sStatus.ePcmFsRate = SD_AUDIO_FS_48KHZ  ;
							break;
						case AUD_FS_32K :
							sStatus.ePcmFsRate = SD_AUDIO_FS_32KHZ  ;
							break;					
						case AUD_FS_88K :
							sStatus.ePcmFsRate = SD_AUDIO_FS_88KHZ  ;
							break;	
						case AUD_FS_96K :
							sStatus.ePcmFsRate = SD_AUDIO_FS_96KHZ  ;
							break;	
						case AUD_FS_176K :
							sStatus.ePcmFsRate = SD_AUDIO_FS_176KHZ  ;
							break;	
						default:
							break;
					}
					sStatus.eHdmiAudio = SD_AUDIO_FORMAT_PCM ;				
				}
				else
				{
					sStatus.eHdmiInputFormat = SD_COLOR_FORMAT_RGB  ;
				}

				
			}
			else
			{
				sStatus.eHdmiInputFormat = SD_COLOR_FORMAT_UNKNOWN ;
			}

		}
		if (copy_to_user((void __user *)pStatus, &sStatus,
						 sizeof(SdExtIn_HDMIStatus_t )))
		{
			printk("_SdExtIn_GetHdmiAVInfo copy to user error\n");
			return SD_NOT_OK;
		}	
		return SD_OK;

}

#endif


//API which sends HDCP key to your driver.
static SdResult _SdExtIn_WriteHDCPKey(uint8_t *pBuf, uint32_t size)
{
	int  i;
	char buf[512]; //512 should big enough for 320 bytes 
	char buf2[512]; //for verify
	if (!access_ok(VERIFY_READ, (void __user *)pBuf, size))
	{
		printk("_SdExtIn_HdmiHdcp argument error\n");
		return SD_NOT_OK;
	}
	if(size>512)
	{
		printk("_SdExtIn_HdmiHdcp write size too large (%d >512)\n",size);
		return SD_NOT_OK;
	}
	if (copy_from_user(buf,(void __user *)pBuf, size))
	{
		printk("_SdExtIn_HDMIGet copy from user error\n");
		return SD_NOT_OK;
	}

	if (EEPHDCP_Write(0, buf, size))
	{
	    printk("EEPHDCP_Write fail.\n");
	    return SD_NOT_OK;
	}

	if (EEPHDCP_Read(0, buf2, size))
	{
	    printk("EEPHDCP_Read read eeprom back fail.\n");
	    return SD_NOT_OK;
	}

	for (i=0; i < size; i++)
	{
	    if (buf[i] != buf2[i])
	    {
		printk("EEPHDCP_Write write verification fail !\n");
		return SD_NOT_OK;
	    }
	}

	printk("EEPHDCP_Write write successfully !\n");

	if (EEPHDCP_Init())
	{
	    printk("EEPHDCP_Init fail.\n");
	    return SD_NOT_OK;
	}

	printk("load to HDCP SRAM successfully !\n");

	if (EEPHDCP_SetSramDevice(1, 0xA0))
	{
	    printk("EEPHDCP_SetSramDevice fail.\n");
	    return SD_NOT_OK;
	}

	printk("Open HDCP SRAM device address = 0xA0 !\n");
	#if HDMI_FAST_CONNECT
	vRegWrite1B(SYS_CTRL_1,0x95);
	vRegWrite1B(SRST_1,0x0b);//mark by ciwu
	vRegWrite1B(SRST_1,0x00);//mark by ciwu
	vHDMITMDSReset();
	vHDMIUnMuteAudio();
	vHDMIAudConfig();
        vHDMIApllClkReset();	
#endif
	return SD_OK;

}

static UINT16 GetScalerDataValue(UINT16 x, UINT16 y, UINT8 index)
{
    UINT16 val[3];
    bGetScalerDRAM(x, y, &val[0],&val[1],&val[2]);

    switch(index)
    {
        default:
        case SD_OUT_FORMAT_Y:   return val[2];
        case SD_OUT_FORMAT_CB:  return val[1];
        case SD_OUT_FORMAT_CR:  return val[0];  
        case SD_OUT_FORMAT_R:   return val[0];
        case SD_OUT_FORMAT_G:   return val[2];
        case SD_OUT_FORMAT_B:   return val[1];  
    }
}

#if 0
SdResult _SdMisc_GetFirstCalibRegion(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
SdMisc_CapturePosition_k eCapturePos, uint8_t* pData)
#else
SdResult _SdExtIn_GetFirstCalibRegion(SdSource_k eInputSrc, SdExtIn_OutFormat_k eOutputFormat, SdExtIn_CapturePosition_t *psCapturePos, uint16_t* pData)
#endif
{
    uint8_t  retval;
    UINT16 panel_width,panel_height;
    UINT16 src_width,src_height;
    SdExtIn_CapturePosition_t sCapturePos;

    if ((!access_ok(VERIFY_WRITE, (void __user *)pData,
                   sizeof(uint8_t ))))
    {
        printk("_SdExtIn_GetFirstCalibRegion argument error\n");
        return SD_NOT_OK;
    }

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)psCapturePos,  sizeof(SdExtIn_CapturePosition_t)))
    {
	printk("sdal_ioctl_sdextin::SdExtIn_GetFirstCalibRegion arg access error\n");
	return SD_ERR_SYS;
    }
    // Copy user space argument
    if(copy_from_user(&sCapturePos, (void __user *)psCapturePos,  sizeof(SdExtIn_CapturePosition_t)))
    {
	printk("sdal_ioctl_sdextin::SdExtIn_GetFirstCalibRegion copy_from_user error\n");
	return SD_ERR_SYS;
    }


//   PMX_GetResolution(PMX_1, &panel_width, &panel_height);
    panel_width = PANEL_GetPanelPhysicalWidth();
    panel_height = PANEL_GetPanelPhysicalHeight();

    if((sCapturePos.x >= panel_width) || (sCapturePos.y  >= panel_height))
    {
        return SD_NOT_OK;
    }

    //calibration must using SV_VP_MAIN
    src_width=wDrvVideoInputWidth(SV_VP_MAIN);
    src_height=wDrvVideoInputHeight(SV_VP_MAIN);
    sCapturePos.x = (sCapturePos.x*src_width)/panel_width;
    sCapturePos.y = (sCapturePos.y*src_height)/panel_height;

    switch(VSS_MAJOR(Sd_VdoPathSrcMap(eInputSrc)))
    {
        case VSS_VGA:   /* RGB*/
            if((eOutputFormat<SD_OUT_FORMAT_R)||(eOutputFormat>SD_OUT_FORMAT_B))
                return SD_NOT_OK;
            bInitScalerDump();
            retval=GetScalerDataValue(sCapturePos.y, sCapturePos.y, eOutputFormat);
            bReleaseScalerDump();
            break;

        default:        /* YCBCR*/
            if(eOutputFormat>=SD_OUT_FORMAT_R)
                return SD_NOT_OK;
            bInitScalerDump();
            retval=GetScalerDataValue(sCapturePos.x, sCapturePos.y, eOutputFormat)>>2;
            bReleaseScalerDump();
            break;
    }

    if (copy_to_user((void __user *)pData,&retval, sizeof(uint8_t )))
    {
        return SD_NOT_OK;
    }   
    return SD_OK;
}

#if 0
SdResult _SdMisc_SetFirstCalibValue(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
 SdMisc_ControlValue_k eGainOffset, uint16_t Data)
 #else
SdResult _SdExtIn_SetFirstCalibValue(SdSource_k eInputSrc, SdExtIn_OutFormat_k eOutputFormat, SdExtIn_ControlValue_k eGainOffset, uint16_t Data)
#endif 
{
    UINT8 u1InputSource;

    u1InputSource =  VSS_MAJOR(Sd_VdoPathSrcMap(eInputSrc));
    if (u1InputSource == VSS_SCART)
    {
        if (bDrvGetScartInputMode() == SCART_COLOR_RGB)
        {
            u1InputSource = VSS_VGA; 
        }
        else
        {
            u1InputSource = VSS_CVBS;
        }
    }
    

    /* following code not really write to EEPROM, in currecnt config, it's dram shadow*/
    switch(u1InputSource)
    {
        case VSS_ATV:
        case VSS_ATD:
        case VSS_CVBS:
        case VSS_SV:
            /* assume eOutputFormat always SD_OUT_FORMAT_Y */
            switch(eGainOffset)
            {
                case SD_CTRL_VALUE_OFFSET:
                    bDrvSetTVDOffset(Data);
                    break;
                case SD_CTRL_VALUE_GAIN:
                    bDrvSetTVDGain(Data);
                    break;
            }
            break;
        case VSS_YPBPR:
            switch(eOutputFormat)
            {
                case SD_OUT_FORMAT_Y:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(0,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP0,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(0,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN0,(UINT8)Data);
                            break;
                    }
                    break;

                case SD_OUT_FORMAT_CB:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(1,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP1,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(1,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN1,(UINT8)Data);
                            break;
                    }
                    break;

                    case SD_OUT_FORMAT_CR:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(2,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP2,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(2,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN2,(UINT8)Data);
                            break;
                    }
                    break;

                    default:
                        return SD_NOT_OK;
            }
            break;
        case VSS_VGA:
            switch(eOutputFormat)
            {
                case SD_OUT_FORMAT_R:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(2,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP2,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(2,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN2,(UINT8)Data);
                            break;
                    }
                    break;
                case SD_OUT_FORMAT_G:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(1,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP1,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(1,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN1,(UINT8)Data);
                            break;
                    }
                    break;
                    case SD_OUT_FORMAT_B:
                    switch(eGainOffset)
                    {
                        case SD_CTRL_VALUE_OFFSET:
                            bDrvSetHDADCOffset(0,0xff-(UINT8)Data);
                            fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP0,0xff-(UINT8)Data);
                            break;
                        case SD_CTRL_VALUE_GAIN:
                            bDrvSetHDADCGain(0,(UINT8)Data);
                            fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN0,(UINT8)Data);
                            break;
                    }
                    break;
                    default:
                        return SD_NOT_OK;
            }
            break;

        default:
                return SD_NOT_OK;       
    }


    return SD_OK;

}

static UINT16 GetLastStageDataValue(UINT16 x,UINT16 y, UINT8 index)
{
    UINT16 val[3];
    bGetLastStageDRAM(x, y, &val[0],&val[1],&val[2]);
    switch(index)
    {
        default:
        case SD_OUT_FORMAT_R:   return val[0];
        case SD_OUT_FORMAT_G:   return val[2];
        case SD_OUT_FORMAT_B:   return val[1];  
    }
}

#if 0
SdResult _SdMisc_GetSecondCalibRegion(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
SdMisc_CapturePosition_k eCapturePos, uint8_t* pData)
#else
SdResult _SdExtIn_GetSecondCalibRegion(SdSource_k eInputSrc, SdExtIn_OutFormat_k eOutputFormat, SdExtIn_CapturePosition_t *psCapturePos, uint16_t* pData)
#endif
{
#if 0 // SDAL_FIXME  : not support LVDS dump
    uint8_t  retval;
    UINT16 panel_width,panel_height;
    SdExtIn_CapturePosition_t sCapturePos;

    //uint16_t x,y;
    // Check access of user space
    if((eOutputFormat<SD_OUT_FORMAT_R)||(eOutputFormat>SD_OUT_FORMAT_B))
        return SD_NOT_OK;
    if ((!access_ok(VERIFY_WRITE, (void __user *)pData,
                   sizeof(uint16_t ))))
    {
        printk("_SdExtIn_GetFirstCalibRegion argument error\n");
        return SD_NOT_OK;
    }

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)psCapturePos,  sizeof(SdExtIn_CapturePosition_t)))
    {
	printk("sdal_ioctl_sdextin::_SdExtIn_GetSecondCalibRegion arg access error\n");
	return SD_ERR_SYS;
    }
    // Copy user space argument
    if(copy_from_user(&sCapturePos, (void __user *)psCapturePos,  sizeof(SdExtIn_CapturePosition_t)))
    {
	printk("sdal_ioctl_sdextin::_SdExtIn_GetSecondCalibRegion copy_from_user error\n");
	return SD_ERR_SYS;
    }
    
//    PMX_GetResolution(PMX_1, &panel_width, &panel_height);
    panel_width = PANEL_GetPanelPhysicalWidth();
    panel_height = PANEL_GetPanelPhysicalHeight();

    if((sCapturePos.x >= panel_width) ||(sCapturePos.y >= panel_height))
        return SD_NOT_OK;

    //mtk01140 20071020 don't bypass MLC while caliberation
    //vDrvMainOnOff(SV_OFF);
    bInitLastStageDump();
    retval=GetLastStageDataValue(sCapturePos.x, sCapturePos.y, eOutputFormat)>>2;
    bReleaseLastStageDump();

    if (copy_to_user((void __user *)pData,&retval, sizeof(uint8_t )))
    {
        return SD_NOT_OK;
    }   
    return SD_OK;
#else
    return SD_NOT_OK;
#endif
}

#if 0
SdResult _SdMisc_SetSecondCalibValue(SdSource_k eInputSrc, SdMisc_OutFormat_k eOutputFormat,
 SdMisc_ControlValue_k eGainOffset, uint8_t Data)
 #else
SdResult _SdExtIn_SetSecondCalibValue(SdSource_k eInputSrc, SdExtIn_OutFormat_k eOutputFormat, SdExtIn_ControlValue_k eGainOffset, uint16_t Data)
#endif
{
    if(eOutputFormat<SD_OUT_FORMAT_R)
        return SD_NOT_OK;
    switch(eGainOffset)
    {
        case SD_CTRL_VALUE_OFFSET:
            if(Data<0x80) 
                Data+=0x80 ;
            else    Data-=0x80 ;
            
            vApiVideoRGBOffset(SV_VP_MAIN,eOutputFormat-SD_OUT_FORMAT_R,(UINT8)Data);
            break;
        case SD_CTRL_VALUE_GAIN:
            vApiVideoRGBGain(SV_VP_MAIN,eOutputFormat-SD_OUT_FORMAT_R,(UINT8)Data);
            break;
    }
    return SD_OK;
}
extern UINT32 wHDMIInfoFrame[10];
SdResult _SdExtIn_GetHdmiInfo(SdExtInID_k eExtInId, SdExtIn_HdmiInfoType_k eInfoType, SdExtIn_HdmiInfo_t *pInfo)
{
	uint8_t  InfoType;
	switch(eInfoType)
	{
		case SD_HDMI_INFO_FRAME_AVI: InfoType=0;break;
		case SD_HDMI_INFO_FRAME_SPD:InfoType=1;break;
		case SD_HDMI_INFO_FRAME_AUDIO:InfoType=2;break;
		case SD_HDMI_INFO_FRAME_MPEG:InfoType=3;break;
		case SD_HDMI_INFO_FRAME_UNREC:InfoType=4;break;
		case SD_HDMI_INFO_FRAME_GAMUT:InfoType=5;break;
		case SD_HDMI_INFO_FRAME_ACP:InfoType=6;break;
		default:
			return SD_NOT_OK;
	};
	if(fgIsExtSrcHDMI())//fgIsMainDVI()||fgIsPipDVI()
	{		
		// Check access of user space
		if (!access_ok(VERIFY_WRITE, (void __user *)pInfo,
					   sizeof(SdExtIn_HdmiInfo_t )))
		{
			printk("_SdExtIn_HDMIGet argument error\n");
			return SD_NOT_OK;
		}
		vHDMIGetInfoFrame(InfoType);
		if (copy_to_user((void __user *)pInfo, wHDMIInfoFrame,
						 sizeof(SdExtIn_HdmiInfo_t )))
		{
			printk("_SdExtIn_HDMIGet copy to user error\n");
			return SD_NOT_OK;
		}	
		return SD_OK;
	}
    return SD_NOT_OK;
}


int sdal_ioctl_sdextin(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    SDAL_IOCTL_8ARG_T rArg12;    //current use SDAL_IOCTL_4ARG_T instead of SDAL_IOCTL_12ARG_T

    if (_IOC_SIZE(cmd)>sizeof(unsigned long))
    {    
        // Check access of user space
        if (!access_ok(VERIFY_READ, (void __user *)arg,  _IOC_SIZE(cmd) ))
        {
        printk("sdal_ioctl_sdextin arg access error\n");
        return SD_ERR_SYS;
        }
        // Copy user space argument
        if(copy_from_user(&rArg12, (void __user *)arg,
               _IOC_SIZE(cmd)))
        {
        printk("sdal_ioctl_sdextin copy_from_user error\n");
        return SD_ERR_SYS;
        }
    }    


    switch (cmd) 
    {
        case SDAL_IO_EXTIN_INIT:
            return _SdExtIn_Init((SdPowerState_k)arg);

        case SDAL_IO_EXTIN_SET:
            return _SdExtIn_Set((SdExtInID_k)rArg12.au4Arg[0],(SdExtIn_Settings_t *)rArg12.au4Arg[1]);    

        case SDAL_IO_EXTIN_GET:
            return _SdExtIn_Get((SdExtInID_k)rArg12.au4Arg[0],(SdExtIn_Settings_t *)rArg12.au4Arg[1]);

        case SDAL_IO_EXTIN_GETSTATUS:
            return _SdExtIn_GetStatus((SdExtInID_k)rArg12.au4Arg[0]
                ,(SdMask_t)rArg12.au4Arg[1]
                ,(SdExtIn_Status_t *)rArg12.au4Arg[2]
                ,(uint32_t)rArg12.au4Arg[3]);

        case SDAL_IO_EXTIN_GETHDMISTATUS:
            return _SdExtIn_GetHDMIStatus((SdExtInID_k) rArg12.au4Arg[0]
                , (SdMask_t) rArg12.au4Arg[1]
                , (SdExtIn_HDMIStatus_t*) rArg12.au4Arg[2]
                , (uint32_t) rArg12.au4Arg[3]);

        case SDAL_IO_EXTIN_CONNECT:
            return _SdExtIn_Connect((SdExtInID_k)rArg12.au4Arg[0]
                ,(SdSource_k)rArg12.au4Arg[1]
                ,(SdVideoID_k)rArg12.au4Arg[2]);                    

        case SDAL_IO_EXTIN_DISCONNECT:
            return _SdExtIn_Disconnect((SdExtInID_k)arg);

        case SDAL_IO_EXTIN_PCSET:
            return _SdExtIn_PcSet((SdExtInID_k)rArg12.au4Arg[0],(SdPCAdjustInfo_t *)rArg12.au4Arg[1]);

        case SDAL_IO_EXTIN_PCGET:
                return _SdExtIn_PcGet((SdExtInID_k)rArg12.au4Arg[0],(SdPCAdjustInfo_t *)rArg12.au4Arg[1]);

        case SDAL_IO_EXTIN_DEBUG:
            _SdExtIn_Debug();
            break;

        //begin HDMI_ADDITIONAL_API
        case SDAL_IO_EXTIN_WRITEHDCPKEY:
            return _SdExtIn_WriteHDCPKey((uint8_t *)rArg12.au4Arg[0], (uint32_t) rArg12.au4Arg[1]);

        case SDAL_IO_EXTIN_GETFIRSTCALIBREGION:
            return _SdExtIn_GetFirstCalibRegion((SdSource_k)rArg12.au4Arg[0], (SdExtIn_OutFormat_k)rArg12.au4Arg[1], 
                                                                       (SdExtIn_CapturePosition_t *)rArg12.au4Arg[2], (uint16_t*)rArg12.au4Arg[3]);
        case SDAL_IO_EXTIN_SETFIRSTCALIBVALUE:
            return _SdExtIn_SetFirstCalibValue((SdSource_k)rArg12.au4Arg[0], (SdExtIn_OutFormat_k)rArg12.au4Arg[1], 
            	                                                          (SdExtIn_ControlValue_k)rArg12.au4Arg[2], (uint16_t)rArg12.au4Arg[3]);

        case SDAL_IO_EXTIN_GETSECONDCALIBREGION:
            return _SdExtIn_GetSecondCalibRegion((SdSource_k)rArg12.au4Arg[0], (SdExtIn_OutFormat_k)rArg12.au4Arg[1], 
                                                                       (SdExtIn_CapturePosition_t *)rArg12.au4Arg[2], (uint16_t*)rArg12.au4Arg[3]);
	 
        case SDAL_IO_EXTIN_SETSECONDCALIBVALUE:
            return _SdExtIn_SetSecondCalibValue((SdSource_k)rArg12.au4Arg[0], (SdExtIn_OutFormat_k)rArg12.au4Arg[1], 
            	                                                          (SdExtIn_ControlValue_k)rArg12.au4Arg[2], (uint16_t)rArg12.au4Arg[3]);

        case SDAL_IO_EXTIN_GETHDMIINFO:
            return _SdExtIn_GetHdmiInfo((SdExtInID_k)rArg12.au4Arg[0], (SdExtIn_HdmiInfoType_k)rArg12.au4Arg[1], (SdExtIn_HdmiInfo_t *)rArg12.au4Arg[2]);	
        default:
                    return SD_NOT_SUPPORTED;
    }
    return SD_OK;
}

