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
 * $RCSfile: io_sdvideo.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdvideo.c
 *  SDAL SdVideo module ioctl interface of MT538X.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>
#include <linux/wait.h>
#include "sd_vdopath.h"

#include "sv_const.h"
#include "nptv_if.h"
#include "vdp_if.h"
#include "source_table.h"
#include "pmx_if.h"
#include "pmx_drvif.h"


#define SLEEP_TIMEOUT_WA  	1

#if SLEEP_TIMEOUT_WA
static BOOL _u4VGAAutoDone;
#endif
static DECLARE_WAIT_QUEUE_HEAD(sdal_vgaauto_wq);

#define _VDO_FLG_PORTING 1
#include "video_def.h"

#if 0
UINT32 VDP_SetOutRegionByDot(UCHAR ucVdpId, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight);
UINT32 VDP_SetInRegionByDot(UCHAR ucVdpId, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight);
UINT32 VDP_GetInRegionByDot(UCHAR ucVdpId, VDP_DISP_REGION_T* rRegion);
UINT32 VDP_GetOutRegionByDot(UCHAR ucVdpId, VDP_DISP_REGION_T* rRegion);
#endif

void Sd_ExtInGrabPcSetting(void);
SdResult _SdVideo_Disconnect(SdVideoID_k eVideoId,UINT8 FromSdImage);

static UINT8 _SdVideo_AutoAdjPath=SV_VP_MAX;
static UINT8 _SdVideo_AutoAdjFail=0;

#define CAPTURE_BASEON_PANELRESOLUTION 0
#define  SDAL_HV_POSITION 1

#define SUPPORT_AUXOUT

static void _SdVideo_SrcRegionUpdate(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings)
{    
    VDP_REGION_T rRegion;// Source Region
    UINT16 wUiHResolution;
    UINT8 Hfct=1;
#if CAPTURE_BASEON_PANELRESOLUTION
    UINT16 panel_width,panel_height,src_width,src_height;

    PMX_GetResolution(PMX_1, &panel_width, &panel_height);
    src_width=wDrvVideoInputWidth(sdal_video[eVideoId].out_id);
    src_height=wDrvVideoInputHeight(sdal_video[eVideoId].out_id);

    rRegion.u4X=(pSettings->sCaptureSize.hStart*src_width)/panel_width;
    rRegion.u4Y=(pSettings->sCaptureSize.vStart*src_height)/panel_height;
    rRegion.u4Width=(pSettings->sCaptureSize.hResolution*src_width)/panel_width;
    rRegion.u4Height=(pSettings->sCaptureSize.vResolution*src_height)/panel_height;
#else


#if !SDAL_HV_POSITION
    if((sdal_video[eVideoId].be_id>=SD_BE_IN_EXTIN0)&&(sdal_video[eVideoId].be_id<=SD_BE_IN_EXTIN1)
        &&sdal_video[eVideoId].vpath_assigned)
    {
        SdExtInID_k  id=sdal_video[eVideoId].be_id-SD_BE_IN_EXTIN0;
        rRegion.u4X=pSettings->sCaptureSize.hStart+sdal_extin[id].setting.sResolution.hStart;
        rRegion.u4Width=pSettings->sCaptureSize.hResolution;
        rRegion.u4Y=pSettings->sCaptureSize.vStart+sdal_extin[id].setting.sResolution.vStart;
        rRegion.u4Height=pSettings->sCaptureSize.vResolution;
    }
    else
#endif
    {
        rRegion.u4X=pSettings->sCaptureSize.hStart;
        rRegion.u4Width=pSettings->sCaptureSize.hResolution;
        rRegion.u4Y=pSettings->sCaptureSize.vStart;
        rRegion.u4Height=pSettings->sCaptureSize.vResolution;
    }
    wUiHResolution=wApiVideoGetInputWidth(sdal_video[eVideoId].out_id);
    if(wUiHResolution&&((wDrvVideoInputWidth(sdal_video[eVideoId].out_id)/wUiHResolution)==2))
    {
        //oversample timing for ypbpr or vga
        rRegion.u4X*=2;
        rRegion.u4Width*=2;
    }

#endif
    VDP_SetSrcRegion((UCHAR)sdal_video[eVideoId].out_id, 0 , rRegion);

}


void _SdVideo_NotifyExtInSrcRegionUpdate(SdVideoID_k eVideoId, SdExtInID_k eExtInId)
{
    _SdVideo_SrcRegionUpdate(eVideoId,&sdal_video[eVideoId].setting);
}

static void _SdVideo_UpdateSetting(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings)
{
//        pSettings->sCaptureSize.hStart=0;
//        pSettings->sCaptureSize.vStart=0;
        pSettings->sCaptureSize.hTotal=wDrvVideoGetHTotal(sdal_video[eVideoId].out_id);
        pSettings->sCaptureSize.vTotal=wDrvVideoGetVTotal(sdal_video[eVideoId].out_id);
//        pSettings->sCaptureSize.hResolution=wDrvVideoInputWidth(sdal_video[eVideoId].out_id);
//        pSettings->sCaptureSize.vResolution=wDrvVideoInputHeight(sdal_video[eVideoId].out_id);
        pSettings->sCaptureSize.bProgressScan=bDrvVideoIsSrcInterlace(sdal_video[eVideoId].out_id)?0:1;
        pSettings->sCaptureSize.frameRate=bDrvVideoGetRefreshRate(sdal_video[eVideoId].out_id);

//        pSettings->sGeometry.x=0;
//        pSettings->sGeometry.y=0;
//        pSettings->sGeometry.w=(sdal_video[eVideoId].out_id?_rPDispInfo.wRWidth:_rMDispInfo.wRWidth);
//        pSettings->sGeometry.h=(sdal_video[eVideoId].out_id?_rPDispInfo.wRHeight:_rMDispInfo.wRHeight);

//        pSettings->bIpcOn=bDrvVideoIsSrcInterlace(sdal_video[eVideoId].out_id);
        if(sdal_video[eVideoId].out_id==SD_WINDOW_MAIN)    
            pSettings->bEnable32PullDown=bDrvVideoIsSrcInterlace(sdal_video[eVideoId].out_id);
        else    pSettings->bEnable32PullDown=0;    //PIP path no such function

		//pSettings->bGameModeOn=0;
		//pSettings->b3DGameModeOn=0;
#if FIX_GAME_MODE_FRAME_TEAR
		if(sdal_video[eVideoId].out_id==SD_WINDOW_MAIN)
		{
			VDP_SetGameMode(sdal_video[eVideoId].out_id, pSettings->bGameModeOn);
		}
		VDP_Set3DGameMode(sdal_video[eVideoId].out_id, pSettings->b3DGameModeOn);
#endif
        //pSettings->bVideoOn=(bDrvVideoGetRefreshRate(sdal_video[eVideoId].out_id)>20?1:0);
        /* others remain the same vaule of last set value, or 0 */

        _SdVideo_SrcRegionUpdate(eVideoId,pSettings);
}



static SdBool_t sdvideo_bgcolor_ycbcr=0;
static UINT32 ColorRgb2YPbPr(UINT32 u4RGB)
{
    UINT32 u4Y, u4Cb, u4Cr;
    UINT32 u4R, u4G, u4B;

    u4R = (u4RGB >> 16) & 0xff;
    u4G = (u4RGB >> 8) & 0xff;
    u4B = u4RGB & 0xff;

    u4Y = (16 + (((u4R * 66) + (u4G * 129) + (u4B * 25)) / 255)) & 0xFF;
    u4Cb = ((((128 * 255) + (u4B * 112)) - ((u4R * 38) + (u4G * 74)) ) / 255) & 0xFF;
    u4Cr = ((((128 * 255) + (u4R * 112)) - ((u4G * 94) + (u4B * 18))) / 255) & 0xFF;

    return ((u4Cr << 16) + ( u4Y<< 8) + (u4Cb));
}
SdResult Sd_VdoPath_BgColorMode(SdBool_t  bxvYCCOn)
{
    sdvideo_bgcolor_ycbcr=bxvYCCOn;
    return SD_OK;
}
SdResult Sd_VdoPath_BgColorUpdate(SdVideoID_k eVideoId)
{
     UINT32 tmp;
    //bg color, VDP in R/G/B 888format
    tmp=(sdal_video[eVideoId].setting.sBGColor.red&0xff)<<16;
    tmp|=(sdal_video[eVideoId].setting.sBGColor.green&0xff)<<8;
    tmp|=(sdal_video[eVideoId].setting.sBGColor.blue&0xff);
    if(sdvideo_bgcolor_ycbcr)
    {
        tmp=ColorRgb2YPbPr(tmp);
    }
    VDP_SetBg(sdal_video[eVideoId].out_id,tmp);
    return SD_OK;
}

/* special path for sdimage */
SdResult Sd_VdoPathVideo_Set(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings)
{
    VDP_REGION_T rRegion;

    if(pSettings!=&sdal_video[eVideoId].setting)
    {    //update  local maintained struct
        x_memcpy(&sdal_video[eVideoId].setting,pSettings,sizeof(SdVideo_Settings_t));
    }
#if 1    /* 3.4.7 bVideoOn means PIP On/Off*/
    if(eVideoId==0)
        Sd_VdoPathVdpEnable(sdal_video[eVideoId].out_id, 1, SDVIDEO);
    else
    {
        Sd_VdoPathVdpEnable(sdal_video[eVideoId].out_id, pSettings->bVideoOn, SDVIDEO);
        _rPChannel.bIsChannelOn = SV_ON;
    }
#else
    pSettings->bVideoOn=sdal_video[eVideoId].beo_connected;
#endif

#if FIX_GAME_MODE_FRAME_TEAR
	if(sdal_video[eVideoId].out_id==SD_WINDOW_MAIN)
	{
    VDP_SetGameMode(sdal_video[eVideoId].out_id, pSettings->bGameModeOn);
	}
#endif
    VDP_Set3DGameMode(sdal_video[eVideoId].out_id, pSettings->b3DGameModeOn);
        
    if(bDrvVideoIsSrcInterlace(sdal_video[eVideoId].out_id))
    {    
        //_VDP_vDrvDIFilmModeOnOff(sdal_video[eVideoId].out_id,pSettings->bEnable32PullDown);
#if 0
        if(sdal_video[eVideoId].out_id==SD_WINDOW_MAIN)
        {
            vDrvFilm32Enable(pSettings->bEnable32PullDown);            
        }
#else 
         vDrvDIFilmModeOnOff(sdal_video[eVideoId].out_id,pSettings->bEnable32PullDown);
#endif        
    }

    _SdVideo_SrcRegionUpdate(eVideoId,pSettings);
    
    // Output Region
    rRegion.u4X=pSettings->sGeometry.x;
    rRegion.u4Y=pSettings->sGeometry.y;
    rRegion.u4Width=pSettings->sGeometry.w;
    rRegion.u4Height=pSettings->sGeometry.h;
    
    VDP_SetOutRegion((UCHAR)sdal_video[eVideoId].out_id, 0 , rRegion);
    
    Sd_VdoPath_BgColorUpdate(eVideoId);
     return SD_OK;
}

SdResult Sd_VdoPathVideo_Get(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings)
{
    x_memcpy(pSettings,&sdal_video[eVideoId].setting,sizeof(SdVideo_Settings_t));
}


void _SdVideo_NotifyHandleSigChg(UINT8 u1Path,UINT8 u1SigState,UINT8 u1ReportState)
{
    UINT8 id;
    for (id=0; id<SD_VIDEO_MAX; id++)
    {
        if ((sdal_video[id].vpath_assigned || sdal_video[id].injectvideo) && (sdal_video[id].out_id==u1Path))
        {
            _SdVideo_UpdateSetting(id, &sdal_video[id].setting);
        }
    }
    if(_SdVideo_AutoAdjPath == u1Path)
    {
        _SdVideo_AutoAdjFail = 1;
        #if SLEEP_TIMEOUT_WA
        _u4VGAAutoDone = 1;
        #endif
        wake_up_interruptible(&sdal_vgaauto_wq);
    }
}

/* This is a global  notification function called from NPTV api_notify.c */
void SdalNotifyAutoDone(UINT8 u1Path,BOOL fgSuccess)
{
    #if SLEEP_TIMEOUT_WA
    _u4VGAAutoDone = 1;
    #endif
    wake_up_interruptible(&sdal_vgaauto_wq);
    printk("vgaauto wakeup!\n");
}

static SdResult _SdVideo_Init(SdPowerState_k ePowerState)
{
    int i;
    UINT32 pmx_order[]={PMX_MAX_INPORT_NS,PMX_OSD2,PMX_OSD3,PMX_MAIN};

    SdVPath_MutexLock();

    #if SLEEP_TIMEOUT_WA
    _u4VGAAutoDone = 1;
    #endif		
    
    Sdal_VdoPath_Initial();         
    bApiVideoMainSubSrc(SV_VS_MAX,SV_VS_MAX);
    VDP_SetFreeze(0,0);
    VDP_SetFreeze(1,0);

    for(i=0;i<2;i++)
    {
        sdal_video[i].module_id=i;    
        if(sdal_video[i].out_id<=SD_WINDOW_SUB0)
            Sd_VdoPathDisconnect(1,i,0);
        x_memset(&sdal_video[i].setting,0,sizeof(SdVideo_Settings_t));
        sdal_video[i].still=0;
        sdal_video[i].mute=0;
        sdal_video[i].injectvideo=0;
        sdal_video[i].vpath_assigned=0;
        sdal_video[i].beo_connected=0;
        sdal_video[i].be_mod=0;
    }

    sdal_video[0].out_id=SD_WINDOW_MAIN;
    sdal_video[1].out_id=SD_WINDOW_SUB0;
    sdal_video[0].setting.bVideoOn=1;
    sdal_video[1].setting.bVideoOn=0;

    PMX_SetPlaneOrderArray(pmx_order);

    SdVPath_MutexUnLock();
}


static SdResult _SdVideo_Connect(SdVideoID_k eVideoId, SdBackEndInID_k eInputId, SdWindowID_k eOutputId)
{
    SdalBackend_t *be_mod;

    if(eInputId>SD_BE_IN_EXTIN1)
        return SD_ERR_INV;

    if(eInputId<SD_BE_IN_EXTIN0)
        be_mod=(SdalBackend_t*)&sdal_avdec[eInputId];
    else
        be_mod=(SdalBackend_t*)&sdal_extin[eInputId-SD_BE_IN_EXTIN0];

    if(sdal_video[eVideoId].injectvideo)
    {
        _SdVideo_Disconnect(eVideoId,1);
//        _SdVideo_Disconnect(eVideoId,0);
        //return SD_NOT_OK;
    }
    SdVPath_MutexLock();

#ifdef SUPPORT_AUXOUT
    if((sdal_video[eVideoId].beo_connected)||sdal_video[eVideoId].vpath_assigned||sdal_vpath[eOutputId].vpath_assigned)
#else    
    if((sdal_video[eVideoId].beo_connected)||(be_mod->vpath_assigned)||sdal_video[eVideoId].vpath_assigned||sdal_vpath[eOutputId].vpath_assigned)
#endif
    {
        SdVPath_MutexUnLock();
        return SD_NOT_OK; //need disconnect first
    }
    
    sdal_video[eVideoId].beo_connected=1;
    sdal_video[eVideoId].be_id=eInputId;
    sdal_video[eVideoId].be_mod=be_mod;
    sdal_video[eVideoId].out_id=eOutputId;
//    sdal_video[eVideoId].mute=0;
    sdal_video[eVideoId].still=0;

    Sd_VdoPathConnect(1, eVideoId , eInputId );
    SdVPath_MutexUnLock();
    return SD_OK;
}

SdResult _SdVideo_Disconnect(SdVideoID_k eVideoId,UINT8 FromSdImage)
{
    if (sdal_video[eVideoId].injectvideo && !FromSdImage)
    {
        return SD_OK;
    }
    if(sdal_video[eVideoId].injectvideo)
    {
        //printk("eVideoId=%d, out_id=%d", eVideoId, sdal_video[eVideoId].out_id);

        VDP_SetMode(sdal_video[eVideoId].out_id, VDP_MODE_BG);
        VDP_Image_Disconnect();

        if (sdal_video[eVideoId].mute)
        {
            VDP_SetMode(sdal_video[eVideoId].out_id, VDP_MODE_BG);
        }
        else
        {  
            VDP_SetMode(sdal_video[eVideoId].out_id, VDP_MODE_NORMAL);
        }

        sdal_video[eVideoId].injectvideo=0;
        VDP_SetScalerPixelBased(1);
#if 0
        if(sdal_video[eVideoId].beo_connected)
        {
            Sd_VdoPathConnect(1, eVideoId , sdal_video[eVideoId].be_id );
            //maybe need to wait mode detect done,
            // and    ExtIn.Set & Video.Set orignal parameters
        }
#endif
        //return SD_OK;    
    }

    if(!sdal_video[eVideoId].beo_connected)
        return SD_NOT_OK;
    SdVPath_MutexLock();
    if(sdal_video[eVideoId].vpath_assigned)
    {
        Sd_VdoPathDisconnect(1,eVideoId,0);
        //Sd_VdoPathVdpEnable(sdal_video[eVideoId].out_id, 0, SDVIDEO);
                //sdal_video[eVideoId].setting.bVideoOn=0;
    }
                SdVPath_MutexUnLock();
    sdal_video[eVideoId].beo_connected=0;
    sdal_video[eVideoId].be_mod=0;
    sdal_video[eVideoId].be_id=0;
    sdal_video[eVideoId].out_id=eVideoId;
        return SD_OK;
    
    //return SD_OK; //for unit test
}


static SdResult _SdVideo_Mute(SdVideoID_k eVideoId, SdBool_t bMuteOn)
{
    
    //if(sdal_video[eVideoId].vpath_assigned||sdal_video[eVideoId].injectvideo)
    {
        sdal_video[eVideoId].mute=bMuteOn;    

        if (bMuteOn)
        {
            VDP_SetMode(sdal_video[eVideoId].out_id, VDP_MODE_BG);
        }
        else
        {  
            VDP_SetMode(sdal_video[eVideoId].out_id, VDP_MODE_NORMAL);
        }
    }

    return SD_OK;
}


static SdResult _SdVideo_Set(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings)
{
    if ((eVideoId<SD_VIDEO_MAX)&&(access_ok(VERIFY_READ, (void __user *)pSettings,sizeof(SdVideo_Settings_t))))
    {
        copy_from_user(&sdal_video[eVideoId].setting,pSettings,sizeof(SdVideo_Settings_t));
        return Sd_VdoPathVideo_Set(eVideoId,&sdal_video[eVideoId].setting);
    }
    return SD_ERR_INV;
}


static SdResult _SdVideo_Get(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings)
{
#if 0
    //if(sdal_video[eVideoId].vpath_assigned||sdal_video[eVideoId].injectvideo)
    {
                return SD_OK;
    }
    //else    return SD_NOT_OK;    
#endif
    if ((eVideoId<SD_VIDEO_MAX)&&(access_ok(VERIFY_WRITE, (void __user *)pSettings,sizeof(SdVideo_Settings_t))))
    {
        copy_to_user(pSettings,&sdal_video[eVideoId].setting,sizeof(SdVideo_Settings_t));
        return SD_OK;
    }
    return SD_ERR_INV;                
}

static SdResult _SdVideo_DetectAutoCoarse(SdVideoID_k eVideoId, uint32_t *uPll)
{
    uint32_t Pll ;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)uPll,
                   sizeof(uint32_t)))
    {
        printk("_SdVideo_DetectAutoCoarse argument error\n");
        return SD_NOT_OK;
    }

    // Get the PC status 
    Pll = wDrvVideoGetHTotal(sdal_video[eVideoId].out_id)+wDrvVGAGetClock()-127 ;                
    if (copy_to_user((void __user *)uPll, &Pll,
                     sizeof(uint32_t)))
    {
        printk("_SdVideo_DetectAutoCoarse copy to user error\n");
        return SD_NOT_OK;
    }            

     return SD_OK;
}



static SdResult  _SdVideo_DetectAutoFine(SdVideoID_k eVideoId, uint32_t *uPhase)    
{
    uint32_t Phase ;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)uPhase,
                   sizeof(uint32_t)))
    {
        printk("_SdVideo_DetectAutoFine argument error\n");
        return SD_NOT_OK;
    }

    // Get the PC status 
    Phase = u1DrvVGAGetPhase() ; ;                
    
    if (copy_to_user((void __user *)uPhase, &Phase,
                     sizeof(uint32_t)))
    {
        printk("_SdVideo_DetectAutoFine copy to user error\n");
        return SD_NOT_OK;
    }            

     return SD_OK;
}

static SdResult  _SdVideo_DetectAutoPostion(SdVideoID_k eVideoId, SdGeometry_t *pPostition)
{
    SdGeometry_t   sPostition ;

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)pPostition,
                   sizeof(SdGeometry_t)))
    {
        printk("_SdVideo_DetectAutoPostion argument error\n");
        return SD_NOT_OK;
    }

    // Get the PC status 
    sPostition.x = vMwGetHPosition() ;
    sPostition.y = vMwGetVPosition() ;         
    sPostition.w = wVgaInputWidth() ;                
    sPostition.h = wVgaInputHeight() ;                    
    
    if (copy_to_user((void __user *)pPostition, &sPostition,
                     sizeof(SdGeometry_t)))
    {
        printk("_SdVideo_DetectAutoPostion copy to user error\n");
        return SD_NOT_OK;
    }            

     return SD_OK;
}


static SdResult _SdVideo_DoAutoAdjust(SdVideoID_k eVideoId, SdPCAdjustInfo_t* pPcData)
{
   SdResult  eReturn = SD_OK;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)pPcData,
                   sizeof(SdPCAdjustInfo_t)))
    {
        printk("SdVideo_DoAutoAdjust  argument error\n");
        return SD_NOT_OK;
    }

    // Copy user space argument
    if (copy_from_user(&sdal_pcsetting, (void __user *)pPcData,
                       sizeof(SdPCAdjustInfo_t)))
    {
        printk("SdVideo_DoAutoAdjust  argument error\n");
        return SD_NOT_OK;
    }        
#if 0    
     printk("freq %d phsae %d\n", sPcSettings.frequency, sPcSettings.phase);
     printk("x %d y %d w %d h %d \n", sPcSettings.sPCPosition.x, sPcSettings.sPCPosition.y, sPcSettings.sPCPosition.w, sPcSettings.sPCPosition.h);
#endif     

    #if SLEEP_TIMEOUT_WA
    _u4VGAAutoDone = 0;
    #endif

     // Trigger auto 
    if((bVgaSigStatus()!=SV_VDO_STABLE) || (!sdal_video[eVideoId].vpath_assigned)
     ||(_SdVideo_AutoAdjPath!=SV_VP_MAX))
    {
        eReturn = SD_NOT_OK;
        printk("SdVideo_DoAutoAdjust  SignalStatus = %d, sdal_video[%d].vpath_assigned = %d, _SdVideo_AutoAdjPath = %d\n",
        	bVgaSigStatus(), eVideoId, sdal_video[eVideoId].vpath_assigned, _SdVideo_AutoAdjPath);
    }
    else
    {  
        _SdVideo_AutoAdjPath=sdal_video[eVideoId].out_id;
        _SdVideo_AutoAdjFail=0;
        vDrvVgaAutoStart();
        
        #if SLEEP_TIMEOUT_WA
        wait_event_interruptible_timeout(sdal_vgaauto_wq, _u4VGAAutoDone, 15*HZ);        
        if (!_u4VGAAutoDone)	
        #else
        if (interruptible_sleep_on_timeout(&sdal_vgaauto_wq, 15*HZ) == 0)
        #endif	
        {
            printk("SdVideo_DoAutoAdjust  VGA Auto Timeout\n");
            eReturn =  SD_NOT_OK;
            _SdVideo_AutoAdjPath = SV_VP_MAX;
        }
        else
        {
            _SdVideo_AutoAdjPath = SV_VP_MAX;
            if (_SdVideo_AutoAdjFail)
            {    //mode chg during Auto
                printk("SdVideo_DoAutoAdjust  mode chg during Auto\n");
                _SdVideo_AutoAdjFail=0;
                eReturn =  SD_NOT_OK;
            }
        }
    }

    if (eReturn == SD_NOT_OK)
    {
        return eReturn;
    }

    Sd_ExtInGrabPcSetting();
    if (copy_to_user( (void __user *)pPcData,&sdal_pcsetting,
                       sizeof(SdPCAdjustInfo_t)))
    {
        printk("SdVideo_DoAutoAdjust  argument error\n");
        return SD_NOT_OK;
    }
     return SD_OK;
}


static SdResult _SdVideo_StopAutoAdjust(SdVideoID_k eVideoId)
{
    if (_SdVideo_AutoAdjPath == sdal_video[eVideoId].out_id)   // check if target Vdp is under Auto processing
    {
        vDrvVgaAutoStop();
        _SdVideo_AutoAdjFail = 1;
        #if SLEEP_TIMEOUT_WA
        _u4VGAAutoDone = 1;
        #endif
        wake_up_interruptible(&sdal_vgaauto_wq);
    }
    return SD_OK;
}

static SdResult _SdVideo_StillVideo(SdVideoID_k eVideoId, SdBool_t bStillOn)
{
    if(eVideoId<SD_VIDEO_MAX)
    {
        VDP_SetFreeze(sdal_video[eVideoId].out_id, bStillOn);
        sdal_video[eVideoId].still=bStillOn;
    }
    return SD_OK;
}


enum {
    FB_ADDR,
    FB_WIDTH,
    FB_HEIGHT,
    FB_COLORFMT,
    FB_SIZE
};

enum {
    FB_CFMT_YCBCR422_8BIT,
    FB_CFMT_RGB444_8BIT,
    FB_CFMT_YCBCR422_10BIT
};

UINT32 _VDP_GetFrameInfo(UINT32 u4VdpId,UINT8 u1fbinfo);

static SdResult _SdVideo_CaptureFrame(SdVideoID_k eVideoId, uint8_t* pDestination, SdVideo_CaptureInfo_t *pCaptureInfo)
{
    SdVideo_CaptureInfo_t  cap;
    UINT8 path;
    RPicInfo* picinfo;
    void * fbaddr;
    
    if (!access_ok(VERIFY_WRITE, (void __user *)pCaptureInfo,  sizeof(SdVideo_CaptureInfo_t) ))
    {
        printk("_SdVideo_CaptureFrame arg access error\n");
        return SD_ERR_SYS;
    }

    if(eVideoId<SD_VIDEO_MAX)
    {
        path=sdal_video[eVideoId].out_id;
        picinfo=(path?&_rPPicInfo:&_rMPicInfo);

        fbaddr=_VDP_GetFrameInfo(path,FB_ADDR);
        cap.Width=_VDP_GetFrameInfo(path,FB_WIDTH);
        cap.Height=_VDP_GetFrameInfo(path,FB_HEIGHT);
        cap.FrameSize=_VDP_GetFrameInfo(path,FB_SIZE);

        if(_VDP_GetFrameInfo(path,FB_COLORFMT)==FB_CFMT_RGB444_8BIT)
        {
            cap.eFormat=SD_COLOR_FORMAT_RGB;
        }
        else
        {
            cap.eFormat=SD_COLOR_FORMAT_YCbCr422;
        }
        cap.Pts=0;

        if (!access_ok(VERIFY_WRITE, (void __user *)pDestination, cap.FrameSize))
        {
            printk("_SdVideo_CaptureFrame arg access error\n");
            return SD_ERR_SYS;
        }
        
        if (copy_to_user( (void __user *)pCaptureInfo,&cap,
                           sizeof(SdVideo_CaptureInfo_t)))
        {
            printk("_SdVideo_CaptureFrame  argument error\n");
            return SD_NOT_OK;
        }
        if (copy_to_user( (void __user *)pDestination,fbaddr,
                           cap.FrameSize))
        {
            printk("_SdVideo_CaptureFrame  argument error\n");
            return SD_NOT_OK;
        }
    }

    return SD_OK;
}

static void _SdVideo_Debug(void)
{
}

int sdal_ioctl_sdvideo(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    SDAL_IOCTL_4ARG_T rArg12;    //current use SDAL_IOCTL_4ARG_T instead of SDAL_IOCTL_12ARG_T

    if (_IOC_SIZE(cmd)>sizeof(unsigned long))
    {    
        // Check access of user space
        if (!access_ok(VERIFY_READ, (void __user *)arg,  _IOC_SIZE(cmd) ))
        {
        printk("sdal_ioctl_sdvideo arg access error\n");
        return SD_ERR_SYS;
        }
        // Copy user space argument
        if(copy_from_user(&rArg12, (void __user *)arg,
               _IOC_SIZE(cmd)))
        {
        printk("sdal_ioctl_sdvideo copy_from_user error\n");
        return SD_ERR_SYS;
        }
    }

    switch (cmd) 
    {
        case SDAL_IO_VIDEO_INIT:            
            _SdVideo_Init((SdPowerState_k)arg);
            break;
        case SDAL_IO_VIDEO_SET:
            return _SdVideo_Set((SdVideoID_k)rArg12.au4Arg[0],(SdVideo_Settings_t*)rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_GET:
            return _SdVideo_Get((SdVideoID_k)rArg12.au4Arg[0],(SdVideo_Settings_t*)rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_CONNECT:
            return _SdVideo_Connect((SdVideoID_k)rArg12.au4Arg[0]
                ,(SdBackEndInID_k)rArg12.au4Arg[1]
                ,(SdWindowID_k)rArg12.au4Arg[2]);                    
        case SDAL_IO_VIDEO_DISCONNECT:
            return _SdVideo_Disconnect((SdVideoID_k)arg,0);
        case SDAL_IO_VIDEO_MUTE:
             return _SdVideo_Mute((SdVideoID_k)rArg12.au4Arg[0], (SdBool_t)rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_DETECTAUTOCOARSE:
            return _SdVideo_DetectAutoCoarse((SdVideoID_k)rArg12.au4Arg[0], (uint32_t)rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_DETECTAUTOFINE:
            return _SdVideo_DetectAutoFine((SdVideoID_k) rArg12.au4Arg[0],(uint32_t) rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_DETECTAUTOPOSITION:
            return _SdVideo_DetectAutoPostion((SdVideoID_k) rArg12.au4Arg[0],(SdGeometry_t *) rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_DOAUTOADJUST:
            return _SdVideo_DoAutoAdjust((SdVideoID_k) rArg12.au4Arg[0],(SdPCAdjustInfo_t *) rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_STOPAUTOADJUST:
            return _SdVideo_StopAutoAdjust((SdVideoID_k) rArg12.au4Arg[0]);  
        case SDAL_IO_VIDEO_STILLVIDEO:
            return _SdVideo_StillVideo((SdVideoID_k) rArg12.au4Arg[0], (SdBool_t) rArg12.au4Arg[1]);
        case SDAL_IO_VIDEO_CAPTUREFRAME:
            return _SdVideo_CaptureFrame((SdVideoID_k) rArg12.au4Arg[0]
                ,(uint8_t*) rArg12.au4Arg[1]
                ,(SdVideo_CaptureInfo_t *)rArg12.au4Arg[2]);
        case SDAL_IO_VIDEO_DBBUG:
            _SdVideo_Debug();
            break;
        default:
            return SD_NOT_SUPPORTED;
    }
    return SD_OK;
}


// driver notification interface
void SdalVideoNotifyAutoDone(NOTIFY_VIDEO_AUTO_DONE_T tNotify)
{
    switch(tNotify.u4NotifyId)    
    {         
        case NOTIFICATION_VIDEO_AUTODONE:            
            SdalNotifyAutoDone((UINT8)tNotify.u4VdpId, (BOOL)tNotify.u4Data1);            
            break;        
        default:            
            printk("SdalVideoNotifyAutoDone: unknown notification command %u\n", tNotify.u4NotifyId);           
            break;    
    }

}


