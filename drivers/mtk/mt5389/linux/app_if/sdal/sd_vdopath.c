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
 * $RCSfile: sd_vdopath.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sd_vdopath.c
 *  mtk native video path control, connect mapping for SDAL
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#include "sd_vdopath.h"

#include "vdp_if.h"
#include "sv_const.h"
#include "nptv_if.h"
#include "source_table.h"
#include "img_lib_if.h"
#include "drvcust_if.h"

SdalVideopath  sdal_vpath[2];

SdalAvdec_t sdal_avdec[2];
SdalExtin_t sdal_extin[2];
SdalVideo_t sdal_video[2];

UINT8 sd_vdopath_initialed;

#define SUPPORT_AUXOUT
//-----------------------------------------------------------------------------
/* This is a global  notification function called from NPTV api_notify.c */
#if 0
void SdalNotifySigChg(UINT8 u1Path,UINT8 u1SigState,UINT8 u1ReportState)
{
	_SdExtIn_NotifyHandleSigChg(u1Path,u1SigState,u1ReportState);
	_SdVideo_NotifyHandleSigChg(u1Path,u1SigState,u1ReportState);
	_SdDisplay_NotifyHandleSigChg(u1Path,u1SigState,u1ReportState);
}
#else
void SdalVideoNotifySignalChanged(NOTIFY_VIDEO_T tNotify)
{
    UINT8 u1Path;
    UINT8 u1SigState;
    UINT8 u1ReportState;

    u1Path = (UINT8)tNotify.u4VdpId;
    u1SigState = (UINT8)tNotify.u4Data1;
    u1ReportState = (UINT8)tNotify.u4Data2;
    
    _SdExtIn_NotifyHandleSigChg(u1Path,u1SigState,u1ReportState);
    _SdVideo_NotifyHandleSigChg(u1Path,u1SigState,u1ReportState);
    _SdDisplay_NotifyHandleSigChg(u1Path,u1SigState,u1ReportState);
}
#endif
//-----------------------------------------------------------------------------


UINT8 Sd_VdoPathSrcMap(SdSource_k fe_src)
{
	switch(fe_src)
	{
		case	SD_SOURCE_ATV1:
				return SV_VS_ATV1;
		case 	SD_SOURCE_DTV1:
				return SV_VS_DTV1;
		case	SD_SOURCE_AV1:
				return SV_VS_CVBS1;
		case	SD_SOURCE_AV2:
				return SV_VS_CVBS2;
		case	SD_SOURCE_AV3:
				return SV_VS_CVBS3;
		case	SD_SOURCE_SVIDEO1:
				return SV_VS_S1;
		case	SD_SOURCE_SVIDEO2:
				return SV_VS_S2;
		case	SD_SOURCE_SVIDEO3:
				return SV_VS_S3;
		case	SD_SOURCE_COMP1:
				return SV_VS_YPbPr1;
		case	SD_SOURCE_COMP2:
				return SV_VS_YPbPr2;
		case	SD_SOURCE_COMP3:
				return SV_VS_YPbPr3;
		case	SD_SOURCE_COMP4:
				return SV_VS_YPbPr4;
		case 	SD_SOURCE_PC1:
				return SV_VS_VGA1;
		case	SD_SOURCE_DVI1:
				return SV_VS_DVI1;
		case	SD_SOURCE_DVI2:
				return SV_VS_DVI2;
		case	SD_SOURCE_DVI3:
				return SV_VS_DVI1;
		case	SD_SOURCE_DVI4:
				return SV_VS_DVI2;
		case	SD_SOURCE_HDMI1:
				return SV_VS_HDMI1;
		case	SD_SOURCE_HDMI2:
				return SV_VS_HDMI2;
		case	SD_SOURCE_HDMI3:
				return SV_VS_HDMI3;
		case	SD_SOURCE_HDMI4:
				return SV_VS_HDMI4;
		case	SD_SOURCE_SCART1:
				return SV_VS_SCART1;
		case	SD_SOURCE_SCART2:
				return SV_VS_SCART2;
		case	SD_SOURCE_SCART3:
				return SV_VS_SCART3;
		case	SD_SOURCE_SCART4:
				return SV_VS_SCART4;				
		case SD_SOURCE_DVR:
		case SD_SOURCE_DLINK:
		case SD_SOURCE_PCM:
		case SD_SOURCE_MEDIA:
		default:
			return SV_VS_MAX;		
	};
};

static DECLARE_MUTEX(sdvpathmutex);

void SdVPath_MutexLock(void)
{
	down(&sdvpathmutex);
	
}
void SdVPath_MutexUnLock(void)
{
	up(&sdvpathmutex);
}


static SdResult SdVPath_ConnectChk(unsigned int IsSdVideo,SdWindowID_k vpath,SdalBackend_t *be_mod,SdalVideo_t *video_mod)
{
	if(vpath>SD_WINDOW_SUB0)
		return  SD_ERR_INV;
	if(sdal_vpath[vpath].vpath_assigned && IsSdVideo && (sdal_vpath[vpath].be_mod!=be_mod)) 
	{	/*need to break deprecated vpath */
		sdal_vpath[vpath].be_mod->vpath_assigned=0;
		sdal_vpath[vpath].be_mod->video_mod=0; 
	}

	sdal_vpath[vpath].vpath_assigned=1;
	sdal_vpath[vpath].be_mod=be_mod;
	sdal_vpath[vpath].video_mod=video_mod;
	be_mod->vpath_assigned=1;
#ifdef SUPPORT_AUXOUT
	if ((vpath == SD_WINDOW_SUB0) && (sdal_vpath[SD_WINDOW_MAIN].vpath_assigned) && (sdal_vpath[SD_WINDOW_MAIN].video_mod == be_mod->video_mod)) 
	{
		// Connect both main and sub to the same input source
		// do nothing.
		// Keep pointing to previous path
	}
	else
//#else
#endif	
	{
	be_mod->video_mod=video_mod;
	}
//#endif
	video_mod->vpath_assigned=1;

	if(bApiVideoSetVideoSrc(vpath,Sd_VdoPathSrcMap(be_mod->fe_src)))
	{
		video_mod->still=0;
		return SD_OK;
	}
	else	return SD_NOT_OK;
}

SdResult Sd_VdoPathConnect(unsigned int IsSdVideo,SdVideoID_k eVideoId, SdBackEndInID_k eInputId)
{
	SdResult ret=SD_NOT_OK;
	if(IsSdVideo)
	{	//from sdvideo
		if(sdal_video[eVideoId].beo_connected && 
		   sdal_video[eVideoId].be_mod->fe_connected ) 
		{
			ret=SdVPath_ConnectChk(1,sdal_video[eVideoId].out_id,
			sdal_video[eVideoId].be_mod, &sdal_video[eVideoId]);
		}
	}
	else
	{	//from backend
		SdalBackend_t *be_mod;
		SdalVideo_t * video_mod;
		
		if(eInputId<SD_BE_IN_EXTIN0)
		{
			be_mod=(SdalBackend_t*)&sdal_avdec[eInputId];
			be_mod->fe_src=SD_SOURCE_DTV1; //for DTV1 for avdec
			be_mod->fe_connected=1;
		}
		else	be_mod=(SdalBackend_t*)&sdal_extin[eInputId-SD_BE_IN_EXTIN0];

		video_mod=&sdal_video[eVideoId];				
		if((be_mod->fe_connected && video_mod->beo_connected) &&
			(be_mod == video_mod->be_mod))
		{				
			ret=SdVPath_ConnectChk(0,video_mod->out_id, be_mod, video_mod);
		}

	}	
	return ret;
}


SdResult Sd_VdoPathDisconnect(unsigned int IsSdVideo,SdVideoID_k eVideoId, SdBackEndInID_k eInputId)
{
	UINT8 connect;	

	if(IsSdVideo)
	{	//from sdvideo
		if(sdal_video[eVideoId].vpath_assigned)
		{
			bApiVideoSetVideoSrc(sdal_video[eVideoId].out_id, SV_VS_MAX);
			sdal_vpath[sdal_video[eVideoId].out_id].vpath_assigned=0;
			sdal_vpath[sdal_video[eVideoId].out_id].be_mod=0;
			sdal_vpath[sdal_video[eVideoId].out_id].video_mod=0;
			sdal_video[eVideoId].be_mod->video_mod=0;
			sdal_video[eVideoId].be_mod->vpath_assigned=0;
			sdal_video[eVideoId].mute=1;
		}
		sdal_video[eVideoId].vpath_assigned=0;
		sdal_video[eVideoId].beo_connected =0;
		sdal_video[eVideoId].be_mod=0;
	}
	else
	{	//from backend
		SdalBackend_t *be_mod;
		
		if(eInputId<SD_BE_IN_EXTIN0)
		{
			be_mod=(SdalBackend_t*)&sdal_avdec[eInputId];
		}
		else
		{
			be_mod=(SdalBackend_t*)&sdal_extin[eInputId-SD_BE_IN_EXTIN0];
		}
		
		be_mod->fe_src=SD_SOURCE_MAX; //for DTV1 for avdec
		be_mod->fe_connected=0; 
		
		if(be_mod->vpath_assigned)
		{
			bApiVideoSetVideoSrc(be_mod->video_mod->out_id, SV_VS_MAX);
			be_mod->vpath_assigned=0;
			be_mod->video_mod->vpath_assigned=0;
			sdal_vpath[be_mod->video_mod->out_id].vpath_assigned=0;
			sdal_vpath[be_mod->video_mod->out_id].be_mod=0;
			sdal_vpath[be_mod->video_mod->out_id].video_mod=0;
			sdal_video[eVideoId].mute=1;
		}
		be_mod->video_mod=0;
	}	

	return SD_OK;
}


static UINT8 sdal_vdpenable[2];
UINT8 Sd_VdoPathVdpEnable(UINT8 u1VdpId, UINT8 u1Enable, UINT8 u1_mod)
{
	if(u1Enable)
	{
		sdal_vdpenable[u1VdpId]|=u1_mod;
	}
	else
	{
		sdal_vdpenable[u1VdpId]&=~u1_mod;
	}

	if(sdal_vdpenable[u1VdpId] == (SDVIDEO|SDDISPLAY))
	{
		VDP_SetEnable(u1VdpId, 1);
		return 1;
	}
	else
	{
		VDP_SetEnable(u1VdpId, 0);
		return 0;
	}
}

void vMpegModeDetDone(UCHAR ucVdpId);

UINT8 Sd_VdoPathInjectVideo(SdVideoID_k eVideoId)
{
	if(sdal_video[eVideoId].injectvideo==0)
	{
		if(sdal_video[eVideoId].vpath_assigned)
		{
			return 0;
		}

		VDP_Image_Mute(1);
		sdal_video[eVideoId].injectvideo=1;
		IMG_VDP_Connect((UINT32)eVideoId);
	}

	//bApiVideoSetVideoSrc(sdal_video[eVideoId].out_id,SV_VS_DTV1);
	//vMpegModeDetDone(sdal_video[eVideoId].out_id);
	//VDP_SetMode(sdal_video[eVideoId].out_id, VDP_MODE_NORMAL);//force un-mute
	//VDP_SetEnable(sdal_video[eVideoId].out_id, 1);//force enable
	return 1;
}


UINT8 Sdal_VdoPath_Initial(void)
{
	UINT32 i;
	CRIT_STATE_T csState;
	csState = x_crit_start();

		for(i=0;i<2;i++)
		{
		x_memset(&sdal_vpath[i],0,sizeof(SdalVideopath));
			sdal_vpath[i].module_id=i;
			sdal_avdec[i].module_id=i;
			sdal_avdec[i].be_id=SD_BE_IN_AVDEC0+i;
			sdal_extin[i].module_id=i;			
			sdal_extin[i].be_id=SD_BE_IN_EXTIN0+i;
			sdal_video[i].module_id=i;
			sdal_vdpenable[i]=0;
		}		
	sd_vdopath_initialed=1;

	x_crit_end(csState);
	return 1;
}




