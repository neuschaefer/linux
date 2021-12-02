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

/**
 * @file mloop_video.c
 *
 * State machine of video flow.
 *
 * @author cindy_huang@mtk.com.tw
 * @author yj_chiang@mtk.com.tw
 */

#define _MAIN_LOOP_C_


#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "drv_video.h"
#include "nptv_drvif.h"
#include "video_def.h"
#if SUPPORT_VBI
#include "drv_vbi.h"
#endif
#include "drv_tvd.h"
#include "drv_hdtv.h"
#include "drv_di.h"
#include "drv_nr.h"
#ifdef CC_MT5395
#include "drv_mjc.h"
#endif
#if SUPPORT_SCART
#include "drv_scart.h"
#endif
#include "hw_nr.h"
#include "vdp_drvif.h"
#include "srm_drvif.h"
#include "hw_scpos.h"
#include "hw_tdc.h"
#include "panel.h"
#ifndef CC_COPLAT_MT82
#include "aud_if.h"
#include "aud_drvif.h"
#else
#define AUD_AtvAvInitialSync(fgStable)
#endif
#include "nptv_debug.h"
#include "source_select.h"
#include "drv_tdc.h"
#include "srm_drvif.h"
#include "drvcust_if.h"
#include "x_timer.h"
#include "api_notify.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_scpos.h"
#include "vdo_misc.h"
#include "c_model.h"
#include "eepdef.h"
#include "drv_contrast.h"
#include "drv_autocolor.h"      //add for ON_CHIP_AUTO_COLOR
#include "drv_ycproc.h"
#include "drv_lcdim.h"
#include "drv_scaler.h"
#include "drv_upscaler.h"
#include "vdp_if.h"
//mtk01140 20080902 DTV will carry Nordig information about color space
#include "drv_mpeg.h"
#include "mute_if.h"

#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif

#include "drv_hdmi.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_bim.h"
#ifdef DRV_SUPPORT_EXTMJC
#ifdef DRV_SUPPORT_EXTMJC_MODE7
#include "osd_drvif.h"
#endif
#endif

#include "tve_if.h"
#include "hw_sw.h"
#include "hw_pdwnc.h"
#include "drv_pqstress.h"

#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#include "srm_drvif.h"
#include "sif_if.h"
#endif

#if PREPROC_ENABLE_SYNC_REGEN
#include "drv_ycproc.h"
#endif
#include "mute_if.h"
#include "vdp_display.h"
#include "drv_display.h"
#include "vdo_if.h"
#include "frametrack_drvif.h"
#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif
#include "api_srch_if.h"
#include "drv_od.h"

#ifdef CC_SCPOS_3DTV_SUPPORT
#include "drv_tdtv_drvif.h"
#endif

#define DELAY_MODE_CHG_UNMUTE 10  //[DTV00130707]
//#define DELAY_8280_UNMUTE 1
#define CC_OSD_MUTE_WA    1

////////////////////////////////////////////////////////////////////////////////
BOOL _fgVideoReady;
#ifdef DRV_SUPPORT_EXTMJC
BOOL _fgFirstPIP=FALSE;
#endif

static UINT32 _u4NSDHTotal=0xff;
static UINT32 _u4NSDVTotal=0xff;
static UINT32 _u4NSDFrameRate=0xff;

extern BOOL _fgFRS_ENA;
extern enum eSyncLockMode _eSyncLockMode;

extern UINT8 _bAgcPedEn;
extern UINT8 _bHDMIColorSpace;
////////////////////////////////////////////////////////////////////////////////

#if DELAY_MODE_CHG_UNMUTE
static UINT32 _u4MainDelayUnmute = DELAY_MODE_CHG_UNMUTE;
static UINT32 _u4PipDelayUnmute = DELAY_MODE_CHG_UNMUTE;
#endif

#ifdef CC_ENABLE_AV_SYNC                                               
#define DELAY_MODE_CHG_AUD_STABLE	3
static UINT32 _u4AudStableTriggerCnt[2]= {0,0};
#endif

//UINT32 _u48280Unmte = 0;
UINT32 _u4BypassNptvMloop =0;
UINT8 _bForceFrameRate = 0;
#if CC_OSD_MUTE_WA
UINT8 _bDddsDelayFlag = 0 ;
#endif

extern UINT8 _bFixColorSpace[2];
UINT32 _u4SigDetDone;

#ifdef CC_SUPPORT_OD
#if OD_FW_WA3
extern UINT8 u1SupportOD;
#endif
#endif
/* boot up measurement in release version, record once.*/
static BOOL b_boot_rec_once = FALSE;

#ifdef TVE_WA
extern UINT8 TVE_FirstReset;
#endif

EXTERN void vSWRegMainLoop(void);

/**
 * vFixColorSetTransform()  apply fixcolor settings to color transform
 * eg.RGB444(1)    ALWAYS        ONCE
 * after API:    0x81        0x91
 * fix mchg:    0x81        0x91
 * fix mdone:    0x01        0x11
 * drv mchg:    0x01        0x00
 * drv mdone:    0x01        0x00
 */
PRIVATE void vFixColorSetTransform(UINT8 bPath)
{
    if (bApiVideoGetFixColorSpaceMode(bPath) != SV_FIXCOLORSPACE_OFF)
    {
        UINT8 force_color_sys;
        UINT8 use_422;
        force_color_sys=(bPath?_bPIP_COLOR_SYS:_bMAIN_COLOR_SYS);
        switch (bGetVideoDecType(bPath))
        {
            case SV_VD_DVI:
                switch (bApiVideoGetFixColorSpaceMode(bPath))
                {
                    case SV_FIXCOLORSPACE_RGB_444:
                        force_color_sys=SV_COLOR_RGB;
                        break;
                    case SV_FIXCOLORSPACE_YCBCR:
                        //444 are not use in drv_ycproc
                        if (((_bDviTiming>=MODE_720p_50)&&(_bDviTiming<=MODE_1080p_60)) || ((_bDviTiming>=MODE_720p_24)&&(_bDviTiming<=MODE_720p_30)))
                        {
                            force_color_sys= SV_COLOR_HDMI_422_709;
                        }
                        else
                        {
                            force_color_sys= SV_COLOR_HDMI_422_601;
                        }
                        break;
                    case SV_FIXCOLORSPACE_YCBCR_TO_RGB:
                        force_color_sys=SV_COLOR_YCBCR_RGB;
                        break;
                    case SV_FIXCOLORSPACE_RGB_TO_YCBCR:
                        force_color_sys=SV_COLOR_HDMI_RGB;
                        break;
                }
                break;
            case SV_VD_VGA:
                if(bApiVideoGetFixColorSpaceMode(bPath) == SV_FIXCOLORSPACE_YCBCR)
                {
                    force_color_sys=SV_COLOR_YPBPR_ANALOG;
                }
                else if(bApiVideoGetFixColorSpaceMode(bPath) == SV_FIXCOLORSPACE_RGB_TO_YCBCR)
                {
                    force_color_sys=SV_COLOR_RGB_YUV;
                }
                else if(bApiVideoGetFixColorSpaceMode(bPath) == SV_FIXCOLORSPACE_RGB_444)
                {
                	force_color_sys = SV_COLOR_RGB_VGA;
                }
                else
                {
                    force_color_sys=SV_COLOR_RGB;
                }
                break;
            default:
                break;
        }

        use_422=((force_color_sys==SV_COLOR_RGB)||(force_color_sys==SV_COLOR_YCBCR_RGB) || (force_color_sys==SV_COLOR_RGB_VGA))? SV_OFF:SV_ON;
        
        if (bPath==SV_VP_MAIN)
        {
            _bMAIN_COLOR_SYS=force_color_sys;
            vDrvMain422(use_422);
            _rMChannel.bIs444=!use_422;
        }
        else
        {
            _bPIP_COLOR_SYS =force_color_sys    ;
            vDrvPip422(use_422);
            _rPChannel.bIs444=!use_422;
        }
    }  
}


PRIVATE void vVdoSetColorTransform(UINT8 bPath)
{
    UINT8 bDecType;
    //UINT8 bIs444;
    UINT8 bCOLOR_SYS = SV_COLOR_YUV;
    //mtk01140 20080902 DTV will carry Nordig information about color space
    UINT8 bMpegColorSpace;

    if (bPath == SV_VP_MAIN)
    {
        bDecType = _rMChannel.bDecType;
        //bIs444 = _rMChannel.bIs444;
    }
    else
    {
        bDecType = _rPChannel.bDecType;
        //bIs444 = _rPChannel.bIs444;
    }

    /* Set new decoder and status */
    switch (bDecType)
    {
        case SV_VD_TVD3D:
            switch (bTvd3dGetColorSystem())
            {
                case SV_CS_SECAM:
                case SV_CS_PAL:
                case SV_CS_PAL_60:
                case SV_CS_PAL_N:
                    bCOLOR_SYS = SV_COLOR_YCBCR_PAL;
                    break;
                case SV_CS_PAL_M:
                case SV_CS_NTSC358:
                case SV_CS_NTSC443:
                    if (_bAgcPedEn)
                    {
                        bCOLOR_SYS = SV_COLOR_YCBCR_NTSC;
                    }
                    else
                    {
                        bCOLOR_SYS = SV_COLOR_YCBCR_NTSCJ;
                    }
                    break;
                default:
                    bCOLOR_SYS = SV_COLOR_YCBCR;
                    break;
            }
            break;
        case SV_VD_YPBPR:
            /* Separate Analog/Digital of YPbPr source.*/
            bCOLOR_SYS = (UINT8)SV_COLOR_YPBPR_ANALOG;
            break;

#if SUPPORT_DVI
        case SV_VD_DVI:
            if (!fgIsExtSrcCCIR())
            {
            	       if (_bHDMIColorSpace) //RGB
                    {
                    		bCOLOR_SYS =  SV_COLOR_HDMI_RGB;
            	       }
            	       else //Ycbcr
            	       {
	                        if (bHDMIClocrimetry())
	                        {
	                            bCOLOR_SYS = SV_COLOR_HDMI_422_601;
	                        }
	                        else
	                        {
	                            bCOLOR_SYS = SV_COLOR_HDMI_422_709;
	                        }
            	       }
            }
            break;
#endif
        case SV_VD_VGA:
            if (bGetICInputType(SV_VP_MAIN) ==  (UINT8)P_VGACOMP)
            {
                bCOLOR_SYS = (UINT8)SV_COLOR_YPBPR_ANALOG;
            }
            else
            {
                bCOLOR_SYS = (UINT8)SV_COLOR_RGB_VGA;
            }
            break;
            //-------------------------------------------------------------------------------
            //mtk01140 20080902 DTV will carry Nordig information about color space
            //we can use
            //These are equal to 601 color space
            //    4: ITU-R Recommendation BT.470-6 System M
            //    5: ITU-R Recommendation BT.470-6 System B, G
            //    6: Society of Motion Picture and Television Engineers 170M (1999)
            //    ??? 8: Generic film (colour filters using Illuminant C)
            //These are equal to 709 color space
            //    1: ITU-R Recommendation BT.709-5
            //    7: Society of Motion Picture and Television Engineers 240M (1999)
            //-------------------------------------------------------------------------------
        #ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
            bMpegColorSpace = u1MpegHdColourPrimary();
            if ((bMpegColorSpace == 4) ||   //ITU-R Recommendation BT.470-6 System M
                (bMpegColorSpace == 5) ||    //ITU-R Recommendation BT.470-6 System B, G
                (bMpegColorSpace == 6) ||    //Society of Motion Picture and Television Engineers 170M (1999)
                (bMpegColorSpace == 8))        //Generic film (colour filters using Illuminant C)
            {
                bCOLOR_SYS = (UINT8)SV_COLOR_YPBPR_DTV_SD;
            }
            else if (bMpegColorSpace == 1)  //ITU-R Recommendation BT.709
            {
                bCOLOR_SYS = (UINT8)SV_COLOR_YPBPR_DTV_HD;
            }
            else    //Unknown type, check input resolution directly
            {
                bCOLOR_SYS = (wDrvVideoInputHeight(bPath) >= 720) ?
                    (UINT8)SV_COLOR_YPBPR_DTV_HD : (UINT8)SV_COLOR_YPBPR_DTV_SD;
            }
            break;
        case SV_VD_MPEGSD:
            bMpegColorSpace = u1MpegSdColourPrimary();
            if ((bMpegColorSpace == 4) ||   //ITU-R Recommendation BT.470-6 System M
                (bMpegColorSpace == 5) ||    //ITU-R Recommendation BT.470-6 System B, G
                (bMpegColorSpace == 6) ||    //Society of Motion Picture and Television Engineers 170M (1999)
                (bMpegColorSpace == 8))        //Generic film (colour filters using Illuminant C)
            {
                bCOLOR_SYS = (UINT8)SV_COLOR_YPBPR_DTV_SD;
            }
            else if (bMpegColorSpace == 1)  //ITU-R Recommendation BT.709
            {
                bCOLOR_SYS = (UINT8)SV_COLOR_YPBPR_DTV_HD;
            }
            else    //Unknown type, check input resolution directly
            {
                bCOLOR_SYS = (wDrvVideoInputHeight(bPath) >= 720) ?
                    (UINT8)SV_COLOR_YPBPR_DTV_HD : (UINT8)SV_COLOR_YPBPR_DTV_SD;
            }
            break;
        #endif
        default:
            bCOLOR_SYS = (UINT8)SV_COLOR_YUV;
            break;
    }


    if (bPath == SV_VP_MAIN)
    {
        _bMAIN_COLOR_SYS = bCOLOR_SYS;
    }
    else
    {
        _bPIP_COLOR_SYS = bCOLOR_SYS;
    }
    vFixColorSetTransform(bPath);
    vDrvColorTransform(bPath);
}

UINT8 u1HdmiColorMode = SV_HDMI_MODE_AUTO;
extern UINT8 bGetSupportHdmiModeVideoTiming(void);
void vVdoCheckFixColorMode(UINT8 bPath)
{
    if (bPath > 1)
    {
        LOG(0, "mloop_video.c: vVdoCheckFixColorMode overflow\n");
        return;
    }
    else
    {
        SV_FIXCOLORSPACE_MODE bMode = SV_FIXCOLORSPACE_OFF;

        if ((bGetVideoDecType(bPath) == SV_VD_DVI)   && (bGetSupportHdmiModeVideoTiming() != 0))
        {
            switch (u1HdmiColorMode)
            {
                case SV_HDMI_MODE_GRAPHIC:
                    bMode =  bHDMIInputType()? SV_FIXCOLORSPACE_RGB_444 : SV_FIXCOLORSPACE_YCBCR_TO_RGB;
                    break;
                case SV_HDMI_MODE_VIDEO:
                    bMode =  bHDMIInputType()? SV_FIXCOLORSPACE_RGB_422 : SV_FIXCOLORSPACE_YCBCR;
                    break;
                case SV_HDMI_MODE_AUTO:
                default:
                    bMode = SV_FIXCOLORSPACE_OFF;
                    break;
            }
        }
        else if(bGetVideoDecType(bPath) == SV_VD_VGA)
        {
        	if(u1GetVGA422En()==TRUE)
        	{
        		bMode = SV_FIXCOLORSPACE_RGB_TO_YCBCR;
        	}
        	else
        	{	
        		bMode = SV_FIXCOLORSPACE_RGB_444;

        	}
        }
        vApiVideoSetFixColorSpaceMode(bPath, bMode);
    }
}

void vForceColorSettingCheck(UINT8 bPath)
{
    //1. check HDMI & VGA fix color mode
    vVdoCheckFixColorMode(bPath);
    //2. check TVE fix color mode
#ifdef CC_SUPPORT_TVE
    if (bPath == SV_VP_PIP)
    {
        vDrvSetPipColorDomain();
    }
#endif
}

static void _vApplyPQSetting(UINT8 bPath)
{
	UINT8 i;
	
    bApiFlashPqUpdateQtyMinMax(bPath);
    for (i=0; i < (UINT8)PE_ARG_NS; i++)
    {
        switch (i)
        {
            // List all the quality items that do not update after mode change done.
            //  the back light is not relative to video timing, nor main/pip change.
            case PE_ARG_ADAPTIVE_BACK_LIGHT:
            case PE_ARG_GAMMA:
            case PE_ARG_TDTV_PACKING:       
                break;                    
            default:
                UNUSED(bApiVideoProc(bPath, (PE_ARG_TYPE)i));
                break;
        }
    }
    // Flush all of setting
	vApiApplyVideoEventOutputVSync();
    vApiApplyVideoEventMJCInputVSync();
}

static void _vVdoPEModeChgDone(UINT8 bPath)
{
    vVdoSetColorTransform(bPath);
    vDrvDIModeChangeDone(bPath);
#ifdef CC_MT5395
    vDrvMJCModeChangeDone(bPath);
#endif

    // Apply all PQ related setting again
    _vApplyPQSetting(bPath);

   // Reload Scalar DeRing Tbl
   vDrvDeRingInit();

    // Reset OD overflow flag
    vDrvODDramOverflowReset();

    // Set adaptive luma window
    vDrvADLSetWindow();
}

#ifdef CC_COPLAT_MT82
extern void vSetPicSettingByUI(UINT8 bPath);
#endif
PRIVATE void vVdoModeChgDone(UINT8 bPath)
{
    UINT8 use_422;
    RVChannel * pt_rv_channel= (bPath==SV_VP_MAIN)?(&_rMChannel):(&_rPChannel);

    vForceColorSettingCheck(bPath);

    use_422 = 1 ;

    if(u1GetVGA422En()==TRUE && (bGetSignalType(bPath) == SV_ST_VGA))
    {
		pt_rv_channel->bIs444 = SV_FALSE;
    }
    else
    {
	    if (bIsScalerInput444(bPath))
	    {
	        if (pt_rv_channel->bIs444 == SV_FALSE)
	        {
	            pt_rv_channel->bIs444 = SV_TRUE;
	        }
	        use_422 = 0 ;
	    }
    }
    
    if (bPath==SV_VP_MAIN)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvMain422(use_422);
    }
    else
    {
        vDrvPip422(use_422);
    }
    LOG(3, "Mode change done 422:%d \n", use_422);

    _vVdoPEModeChgDone(bPath);
    
#ifdef CC_COPLAT_MT82
    vSetPicSettingByUI(bPath);//XuYing 20090206
    // Video Source Manager Check
    vDrvVideoSRMChk(bPath);
#endif

#if MANUAL_MAIN_RST
    //fix YPbpr to AV jitter problem in 5360 , use manual reset
    if (bPath == SV_VP_MAIN)
    {
        vMainSrcResetOn();
        vMainSrcResetOff();
    }
    else
    {
        vPipSrcResetOn();
        vPipSrcResetOff();
    }
#endif

    if ((bDrvVideoSignalStatus(bPath) == SV_VDO_STABLE))
    {
#if SUPPORT_SCART
	if(((bPath==SV_VP_MAIN) && fgIsMainScart()) || 
			((bPath==SV_VP_PIP)&& fgIsPipScart()))
        {
            vDrvScartRGBFreeRun(SV_OFF);
        }
#endif
    }

#if SUPPORT_VBI
    {
        VBI_ResetSrc(bPath);
    }
#endif
#ifdef CC_UP8032_ATV
#if SUPPORT_VBI
#if SUPPORT_MM_INPUT
    //if(!fgIsExtSrcMM())
#endif
    {	
        ApiVBIServiceResetSrc(bGetVideoDecType(bPath),bDrvVideoGetType(bPath),fgIsOversampleTiming());
    }
#endif
#endif
#ifdef CC_SUPPORT_OD
#if OD_FW_WA3
    if (u1SupportOD==1)
    {
        HAL_Delay_us(100000);  //100ms
        vDrvODOnOff(SV_ON);
        LOG(0,"MODE Change Done-->OD ON\n");
    }    
#endif
#endif

}

static void vDDDSLCDSetting(UINT16 u2InVTotal, UINT8 bRefreshRate)
{
//    UINT8 bRefreshRate;
    UINT8 bLCDFreqType, bDDDSOn;
    UINT8 bOutFrameRate;

    // --------LCD Freq Settings-----------
    //bRefreshRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);

    if ( _bForceFrameRate != 0 )
    {
        bRefreshRate = _bForceFrameRate;
    }

    bLCDFreqType = SV_DCLK_60HZ;
    bDDDSOn = SV_TRUE;
    bOutFrameRate = bRefreshRate;

    if (((bRefreshRate == 25) && (DRVCUST_PanelGet(ePanel242530pOutput) == 0)) || (bRefreshRate == 50) || (bRefreshRate == 100))
    {    
        bLCDFreqType = SV_DCLK_50HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 100: 50;
    }
    else if (((bRefreshRate == 30) && (DRVCUST_PanelGet(ePanel242530pOutput) == 0)) || (bRefreshRate == 60)|| (bRefreshRate == 120))
    {
        bLCDFreqType =SV_DCLK_60HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 120: 60;
    }
    else if (((bRefreshRate == 24) && (DRVCUST_PanelGet(ePanel242530pOutput) == 0)) || (bRefreshRate == 48))
    {
        #ifdef SUPPORT_PANEL_48HZ
        bLCDFreqType =SV_DCLK_48HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 96: 48;
        #else
        bLCDFreqType =SV_DCLK_60HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 120: 60;
        #ifdef CC_SCPOS_3DTV_SUPPORT
        if( u4DrvGetDisp3DModeDclk() != 0)
        {
            bOutFrameRate = u1DrvGetDisp3DModeRefreshRate();
        }
        #endif
        #endif
    }
    else
    {
        UINT32 u4LockOthers;
        UINT32 u4Clk;
        if (DRVCUST_PanelQuery(eDDDSLockOnOthers, &u4LockOthers) != 0)
        {
            u4LockOthers = 1;
        }
        if (PANEL_IsSupport120Hz())
        {
            if (bRefreshRate >= 110)     // > 110; try 1:1 output
            {
                u4Clk = PANEL_GetPixelClk60Hz()/120 * bRefreshRate;
                bLCDFreqType = SV_DCLK_60HZ;
                bOutFrameRate = 120;
            }
            else if (bRefreshRate >= 77)  // 77 ~ 110; try 1:1 output 
            {
                u4Clk = PANEL_GetPixelClk50Hz()/100 * bRefreshRate;
                if ((u4Clk < DCLK_MAX) && (u4Clk > DCLK_MIN) && (u2InVTotal > 0) && (u4LockOthers))   // 1:1 output
                {
                    bLCDFreqType = SV_DCLK_50HZ;
                    bOutFrameRate = 100;
                }
                else    // 1:2 output
                {
                    bLCDFreqType = SV_DCLK_60HZ;
                    bOutFrameRate = 120;
                }
            }
            else if (bRefreshRate < 55)   // < 55; try 1:2 output
            {
                u4Clk = PANEL_GetPixelClk50Hz()/100 * bRefreshRate * 2;
                bLCDFreqType = SV_DCLK_50HZ;
                bOutFrameRate = 100;
            }
            else        // 55 ~ 77; try 1:2 output
            {
                u4Clk = PANEL_GetPixelClk60Hz()/120 * bRefreshRate * 2;
                bLCDFreqType = SV_DCLK_60HZ;
                bOutFrameRate = 120;
            }
        }
        else
        {
            if (bRefreshRate < 55 && (bRefreshRate != 30) && (bRefreshRate != 24)) 
            {
                u4Clk = PANEL_GetPixelClk50Hz()/50 * bRefreshRate;
                bLCDFreqType = SV_DCLK_50HZ;
                bOutFrameRate = 50;
            }
            else
            {
                u4Clk = PANEL_GetPixelClk60Hz()/60 * bRefreshRate;
                bLCDFreqType = SV_DCLK_60HZ;
                bOutFrameRate = 60;
            }
        }        

        #ifdef DRV_SUPPORT_EXTMJC
        if ((IS_COMPANION_CHIP_ON()) && (u4Clk < DCLK_MAX) && (u2InVTotal > 0) && (u4LockOthers))
        {
            // ignore DLCK_MIN checking when connecting with companion chip
            LOG(3, "===============DDDSLCDSetting: ignore DLCK_MIN/DCLK_MAX checking when connecting with companion chip %d\n",bDrvVideoGetRefreshRate(SV_VP_MAIN));
            bLCDFreqType = SV_DCLK_OTHERS;
            bOutFrameRate = bRefreshRate;
        }
        else
        #endif        
        if ((u4Clk < DCLK_MAX) && (u4Clk > DCLK_MIN) && (u2InVTotal > 0) && (u4LockOthers))
        {
            bLCDFreqType = SV_DCLK_OTHERS;
            if ((bRefreshRate < 77) && PANEL_IsSupport120Hz())
            {
                bOutFrameRate = bRefreshRate * 2;
            }
            else
            {
                bOutFrameRate = bRefreshRate;
            }
        }
        else
        {
            // Cannot turn on DDDS
            bDDDSOn = SV_FALSE;
        }

        if(DRVCUST_PanelGet(ePanel242530pOutput) != 0)
        {
            if(bRefreshRate == 24 || bRefreshRate == 25 || bRefreshRate == 30)
            {
                bOutFrameRate = bRefreshRate;
            }
        }
    }

    if ((bRefreshRate == 24) || (bRefreshRate == 25) || (bRefreshRate == 30))
    {
        #if SUPPORT_DDDS_ON_24P_25P_30P
        if (bDrvVideoIsSrcInterlace(SV_VP_MAIN))
        #endif
        {
            bDDDSOn = SV_FALSE;
        }
    }

    vDrvLCDFreqSet(bLCDFreqType, bOutFrameRate);

    // ---------DDDS Settings------------
    if (bDDDSOn ==SV_TRUE && _eSyncLockMode != eSYNCLOCK_OFF)
    {
        vDrvSetMTKGoodDclk(u2InVTotal, bRefreshRate);
        if (fgIsMainTvd3d())
        {
            _fgFRS_ENA = SV_ON;    //Delay for VCR mode: Only for TVD
        }
        else
        {
            vDrvSwitchMTKGoodDclk(SV_ON);
        }
    }
    else
    {
        _fgFRS_ENA = SV_OFF;
        vDrvSwitchMTKGoodDclk(SV_OFF);
    }

    LOG(3, "DDDSLCDSetting: bRefreshRate(%d), bLCDFreqType(%d), bDDDSOn(%d)\n", bRefreshRate, bLCDFreqType, bDDDSOn);
}


void vSendEvevntToSRM(UINT8 bPath)
{
    UINT32 u4VgaMode;
    UINT32 u4ColorMode444;

    #ifdef CC_ENABLE_AV_SYNC                                               
    if (bPath == AUD_GetPIPVideoPath())
    {
        if (bDrvVideoSignalStatus(bPath) == (UINT8)SV_VDO_STABLE)
        {
            //AUD_VdpModeChangeDone();
			_u4AudStableTriggerCnt[bPath]= DELAY_MODE_CHG_AUD_STABLE;
        }
        else
        {
            AUD_VdpModeChange(); // it should not happen
			_u4AudStableTriggerCnt[bPath]= 0;
        }
    }
    #endif //CC_ENABLE_AV_SYNC

    //u4VgaMode = (UINT32) fgApiVideoIsVgaMode(bPath, 2);
    //u4ColorMode444 = (UINT32) fgApiVideoIsVgaMode(bPath, 0);   // refer to vVdoModeChgDone _rMChannel.bIs444's configuration
    u4ColorMode444 = u4VgaMode = bIsScalerInput444(bPath);

    switch (bApiVideoGetFixColorSpaceMode(bPath))
    {
        case SV_FIXCOLORSPACE_OFF:
            break;
        case SV_FIXCOLORSPACE_RGB_444:
        case SV_FIXCOLORSPACE_YCBCR_TO_RGB:
            u4ColorMode444=1;
            u4VgaMode=1;
            break;
        default:
            u4ColorMode444=0;
            u4VgaMode=0;
            break;
    }

#ifdef CC_SRM_ON
    UINT32 u4RefreshRate;
    u4RefreshRate = (UINT32) bDrvVideoGetRefreshRate(bPath);

    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SIZE + bPath),
                    ((UINT32)wDrvVideoInputWidth(bPath) >> (bDrvVideoGetWidthOverSample(bPath)?1:0)) *
                    ((UINT32)wDrvVideoInputHeight(bPath)),
                    (UINT32)bDrvVideoIsSrcInterlace(bPath));
	SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_WIDTH+ bPath),
                    (UINT32)wDrvVideoInputWidth(bPath) >> (bDrvVideoGetWidthOverSample(bPath)?1:0) ,
                    (UINT32)wDrvVideoInputHeight(bPath));
    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_STATUS + bPath),
                  1,
                  ((u4VgaMode & 0xFF) | ((u4RefreshRate & 0xFF) << 8) | ((u4ColorMode444 & 0xFF) << 16)));

#endif

    if (bPath == SV_VP_MAIN)
    {
        /* boot up measurement in release version, record once.*/
        if (!b_boot_rec_once)
        {
            x_os_drv_set_timestamp("Main Video Stable");
            b_boot_rec_once = TRUE;
        }

#ifdef TIME_MEASUREMENT
        TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "VIDEO_STABLE");

        TMS_END(TMS_BOOT_FROM_SBY);
        TMS_END(TMS_COOL_BOOT_TIME);
        TMS_END(TMS_CHL_CHE_TIME);
        TMS_END(TMS_INP_SRC_CHE_TIME);
#endif
    }
}
/*
 * State machine of main video flow control.
 */

static HAL_TIME_T _rHalTime;

#ifdef CC_AUD_MASK_AUDIO_SUPPORT
#define AUDIO_MASK_TIME_COUNT   60
static UINT8 u1AudioMaskCount = 0;
#endif

PRIVATE void vVdoMainState(void)
{
    UINT8 bSigMode;
    BOOL flag;
    CRIT_STATE_T csState;
#ifdef DRV_SUPPORT_EXTMJC
//    static UINT8 bFirstChipConnection= SV_FALSE;
 //   UINT8 u1MsgErr;
#endif

    switch (_bMainState)
    {
            // *********************************************************************
            /* Idle -- nothing to do for video */
        case VDO_STATE_IDLE:

            csState = x_crit_start();
            flag = fgIsMainFlgSet(MAIN_FLG_MODE_CHG);
            vClrMainFlg(MAIN_FLG_MODE_CHG);

            x_crit_end(csState);

            if (flag)
            {
            #ifdef CC_SUPPORT_OD
             #if OD_FW_WA3
                if (u1SupportOD==1)
                {
                    vDrvODOnOff(SV_OFF);
                    LOG(0,"MODE Change --> OD OFF \n");
                }
            #endif    
            #endif
                #if SUPPORT_SCART
				if(fgIsMainScart())
                //if ((_bMainICIn == (UINT8)P_FB0) || (_bMainICIn == (UINT8)P_FB1) )
                {
                    vDrvScartRGBFreeRun(SV_ON);
                }
                #endif
                vScpipSwitchFrameTrackOnOff(SV_OFF);
                AUD_AtvAvInitialSync(FALSE);
                vDrvSwitchMTKGoodDclk(SV_OFF);
                vSwitchDispMode(SV_OFF);
                #if defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5395)
                /*MJC is on and PR panel*/ 
                if ((u1DrvMJCIsOnOff() == SV_ON)&&(IS_LVDS_DISP_3D_POLARIZED)) 
                {
                    bSigMode = bDrvVideoSignalStatus(SV_VP_MAIN);
                    if (bSigMode == (UINT8)SV_VDO_NOSIGNAL)
                    {
                        bDrvUpdateInternalFrameRate();
                    }
                }
                #endif

                if ((!(_fgAutoSearch && fgIsMainTvd3d())) && (fgIsMainDec() && !fgIsMainDTV()))
                {
                    _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
                }

                if (u1ScpipGetDispMode(SV_VP_MAIN)!=VDP_SCPOS_PSCAN_DISPMODE)
                {
                    _VDP_DispModeEnable((UCHAR)SV_VP_MAIN, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
                }
                #if 0
                if (u1DrvDIGetDispmodeFromSRM(SV_VP_MAIN) == DI_DISP_OFF)
                {
                    vDrvDISetDispmodeOff(SV_VP_MAIN);
                }
                #endif
                _fgFRS_ENA = SV_OFF;

                HAL_GetTime(&_rHalTime);
                LOG(9, "TVD mode change begin\t= %d.%06d\n", _rHalTime.u4Seconds, _rHalTime.u4Micros);
                #if CC_OSD_MUTE_WA
                _bDddsDelayFlag = 0 ;
                #endif

                if (fgIsMainDec())
                {
                    vDrvVideoTrigModeDetect(SV_VP_MAIN);
                    if (fgIsMainTvd3d())
                    {
                        vTvd3dBHModeChg();
                    }
                    #if SUPPORT_VBI
                    {
                        VBI_ResetSrc(SV_VP_MAIN);
                    }
                    #endif
                    _bMainState = VDO_STATE_WAIT_MODE_DET;
                    LOG(3, "Chg to Wait Mode Det\n");
                }
                #ifdef CC_ENABLE_AV_SYNC 
                if (SV_VP_MAIN == AUD_GetPIPVideoPath())          
                {                                                 
                    AUD_VdpModeChange();
			        _u4AudStableTriggerCnt[SV_VP_MAIN]= 0;
                }                                                 
                #endif
                #if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
                vApiTVEVideoStatusNotify(SV_VP_MAIN,SV_TVE_NOTIFY_VIDEO_MODE_CHANGE);
                #endif
                if (!(_fgAutoSearch && fgIsMainTvd3d()))
                {
                    vApiVideoSetEvtFlg(VDO_FLG_MAIN_MODECHG);
                    vApiVideoClrEvtFlg(VDO_FLG_MAIN_MODECHG_DONE);
                }

                if (!fgSkipNptvNfy())
                {
                    vApiNotifySigChg(SV_VP_MAIN, SV_VDO_UNKNOWN, 1);
                }
                #ifdef CC_SRM_ON
                SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_STATUS + VDP_1), 0, 0);
                #endif
            }
            break;

            // *********************************************************************
            /* Mode Change Done -- Mode Change Done! Inform UI to handle this! */
        case VDO_STATE_WAIT_MODE_DET:
            if (fgIsMainDec())
            {
                if (fgIsMainFlgSet(MAIN_FLG_MODE_CHG))
                {
                    _bMainState = VDO_STATE_IDLE;
                    LOG(3, "Chg to Idle #1\n");
                    break;
                }

                csState = x_crit_start();
                flag = fgIsMainFlgSet(MAIN_FLG_MODE_DET_DONE);
                vClrMainFlg(MAIN_FLG_MODE_DET_DONE);
                x_crit_end(csState);

                if (flag)
                {
                    #if SUPPORT_DOT_TO_DOT_PATTERN                   
		       VDP_DotToDotPattern(0, 0);//when change source,close dot_to_dot pattern		
		      #endif
		      bSigMode = bDrvVideoSignalStatus(SV_VP_MAIN);

                    // ---Handle mode change unmute--------
                    if ((bSigMode == (UINT8)SV_VDO_STABLE)
                    #if 0//SUPPORT_ATV_SNOWSCREEN
                         || (fgIsSrcAtv(SV_VP_MAIN) && (bSigMode == (UINT8)SV_VDO_NOSIGNAL) && (u4ApiVideoGetSnowScreen()))
                    #endif 
                    )
                    {
                        #if DELAY_MODE_CHG_UNMUTE
                        //_u4MainDelayUnmute=DELAY_MODE_CHG_UNMUTE;
                         _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, _u4MainDelayUnmute, FALSE);
                        #else
                        if (fgIsMainDec() && !fgIsMainDTV())
                        {
                            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN,  0, FALSE);
                        }
                        #endif
                    }
                    // end of mode change unmute

                    if (fgIsMainTvd3d())
                    {
                        vTvd3dBHModeDone();
                    }
                    /*    When the input source is no signal, turn off Display Mode. SJ Added 050812: Begin    */
                    if (bSigMode == (UINT8)SV_VDO_NOSIGNAL)
                    {
                    //will done in mode change

#if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
                        vApiTVEVideoStatusNotify(SV_VP_MAIN,SV_TVE_NOTIFY_VIDEO_NO_SIGNAL);
#endif
                    }

                    _bMainState = (UINT8)VDO_STATE_IDLE;
                    LOG(3, "Stable, Chg to Idle #2 bSigMode=%d\n ", bSigMode); 


                    if (bSigMode == (UINT8)SV_VDO_STABLE)
                    {
                        if (fgIsMainVga() || fgIsMainYPbPr()
                        #if SUPPORT_SCART
							|| fgIsMainScart()
						//	 ||((_bMainICIn == (UINT8)P_FB0) || (_bMainICIn == (UINT8)P_FB1) )
                        #endif
                        )
                        {
                            vDrvEnableBlankLevelAdjust();
                            //Printf("[Main] -------vDrvEnableBlankLevelAdjust----\n");
                        }

                        // Notify audio for AV initial synchronization in ATV mode, assume in this time video is ready to display
                        if (!fgIsMainDTV())
                        {
                            LOG(9, "***** Vdo stable notify*****\n");
                            AUD_AtvAvInitialSync(TRUE);
#ifdef CC_AUD_MASK_AUDIO_SUPPORT
                            if (SV_VP_MAIN == AUD_GetPIPVideoPath())
                            {
                                AUD_DspSetMaskAudio (AUD_DEC_MAIN, AUD_MASK_AUDIO_ON);
                                u1AudioMaskCount = AUDIO_MASK_TIME_COUNT;
                            }
#endif
                        }

                        // Input Change Notify, ReCalculate Source Region
                        #ifndef CC_UP8032_ATV
                        VDP_SourceChangeNotify(SV_VP_MAIN);
                        #else
                        vSetPicSettingByUI(SV_VP_MAIN);//XuYing 20090206
                        #endif


                        #if CC_OSD_MUTE_WA
                        if (!(_fgAutoSearch || SRM_IsEpgMode()))
                        {
                            vDDDSLCDSetting(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
                            // reset non-standard signal status
                            _u4NSDHTotal=0xff;
                            _u4NSDVTotal=0xff;
                            _u4NSDFrameRate=0xff;
                        }
                        else
                        {
                            _bDddsDelayFlag = 1 ;
                        }
                        #else
                        vDDDSLCDSetting(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
                        // reset non-standard signal status
                        _u4NSDHTotal=0xff;
                        _u4NSDVTotal=0xff;
                        _u4NSDFrameRate=0xff;
                        #endif

                        #if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
                        vApiTVEVideoStatusNotify(SV_VP_MAIN,SV_TVE_NOTIFY_VIDEO_STABLE_SIGNAL);
                        #endif

                        #ifdef CC_DRV_SUPPORT_SMICRO
                        vSMicroInputSignalChange();
                        #endif                        
                    }

                    vVdoModeChgDone(SV_VP_MAIN);
                    DRVCUST_VdoModeChgDone(SV_VP_MAIN);

                    if (bSigMode == (UINT8)SV_VDO_STABLE)
                    {
                        vSendEvevntToSRM(SV_VP_MAIN) ;
                    }

                    #ifdef DRV_SUPPORT_EXTMJC
                   // if (IS_COMPANION_CHIP_ON())
                    {
                        // Change 538x and 8280 OSD to normal mode to avoid OSD shift
                      //  LOG(2,"[vApiRedoTwoChipConnection]: OSD normal mode\n");
                      //  d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_NORMAL, &u1MsgErr);

                        //_u48280Unmte = 15;
                        if ((bSigMode == (UINT8)SV_VDO_STABLE)
                    #if CC_OSD_MUTE_WA
                                && (!_bDddsDelayFlag)
                    #endif
                                )
                        {
                            vDrvExtmjcRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
                        }

                        // Change 538x and 8280 OSD to DMA mode to avoid OSD shift
                     //   LOG(2,"[vApiRedoTwoChipConnection]: OSD DMA mode\n");
                    //    d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_DMA, &u1MsgErr);
                    }
                    #endif//DRV_SUPPORT_EXTMJC

                    #if SUPPORT_VBI
                    _u4SigDetDone = 1;
                    #endif

                    if (!_fgVideoReady)
                    {
                        LOG(3, "Video Ready");
                        _fgVideoReady = TRUE;
                    }

                    vApiVideoSetEvtFlg(VDO_FLG_MAIN_MODECHG_DONE);
                    vApiVideoClrEvtFlg(VDO_FLG_MAIN_MODECHG);

                    // Add for 5371: should we notify as soon as possible?
                    if (fgIsMainDec())
                    {
                        //not notify when DTV input now move into api_notify layer
                        UINT8 rep=(!fgSkipNptvNfy()) || (!fgIsMainTvd3d());

                        vApiNotifySigChg(SV_VP_MAIN,bSigMode,rep);

                        if (rep)
                        {
                            if (bSigMode == (UINT8)SV_VDO_STABLE)
                            {
                                vApiNotifyColorSystem(SV_VP_MAIN, bSigMode);
                                vApiNotifyResolutionChg(SV_VP_MAIN,bSigMode);
                            }
                        }
                    }

                    HAL_GetTime(&_rHalTime);
                    LOG(9, "TVD mode change notify\t= %d.%06d\n", _rHalTime.u4Seconds, _rHalTime.u4Micros);
                }
            }
            break;
        default:
            break;
    }
}

/*
 * State machine for pip video flow control.
 */

PRIVATE void vVdoPipState(void)
{
    UINT8 bSigMode;
    BOOL flag;
    CRIT_STATE_T csState;

    //UINT8 u1MsgErr;
#ifdef DRV_SUPPORT_EXTMJC
    SRM_TV_MODE_T _ePoPMode;
#endif
    switch (_bPipState)
    {
            // *********************************************************************
            /* Idle -- nothing to do for video */
        case VDO_STATE_IDLE:

            csState = x_crit_start();
            flag = fgIsPipFlgSet(PIP_FLG_MODE_CHG);
            vClrPipFlg(PIP_FLG_MODE_CHG);

            x_crit_end(csState);

            if (flag)
            {
#if SUPPORT_SCART
				if(fgIsPipScart())
                //if ((_bSubICIn == (UINT8)P_FB0) || (_bSubICIn == (UINT8)P_FB1) )
                {
                    vDrvScartRGBFreeRun(SV_ON);
                }
#endif
                AUD_AtvAvInitialSync(FALSE);

                if (fgIsPipDec())
                {
                    vDrvVideoTrigModeDetect(SV_VP_PIP);
                    if (fgIsPipTvd3d())
                    {
                        vTvd3dBHModeChg();
                    }
                    #if SUPPORT_VBI
                    {
                        VBI_ResetSrc(SV_VP_PIP);
                    }
                    #endif
                    _bPipState = (UINT8)VDO_STATE_WAIT_MODE_DET;
#if defined(CC_SUPPORT_TVE)||defined(CC_SUPPORT_TVE_82xx)
                    vApiTVEVideoStatusNotify(SV_VP_PIP,SV_TVE_NOTIFY_VIDEO_MODE_CHANGE);
#endif
                }
#ifdef DRV_SUPPORT_EXTMJC
                else if (!_fgAutoSearch)// PIP off
                {
                    _ePoPMode = SRM_GetTvMode();
                    if (_ePoPMode == SRM_TV_MODE_TYPE_NORMAL)
                    {
                        vDrvExtmjcRedoTwoChipConnection( SV_VP_PIP, SV_TRUE);
                    }
                }
#endif

                if (!(_fgAutoSearch && fgIsPipTvd3d()) )
                {
                    if (fgIsPipDec() && !fgIsPipDTV())
                    {
                        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP,
                                          FOREVER_MUTE, FALSE);
                    }
                    vApiVideoSetEvtFlg(VDO_FLG_PIP_MODECHG);
                    vApiVideoClrEvtFlg(VDO_FLG_PIP_MODECHG_DONE);

                }

#ifdef CC_ENABLE_AV_SYNC 
                if (SV_VP_PIP == AUD_GetPIPVideoPath())          
                {                                                 
                    AUD_VdpModeChange();
			        _u4AudStableTriggerCnt[SV_VP_PIP]= 0;
                }                                                 
#endif                

                if (!fgSkipNptvNfy())
                {
                    vApiNotifySigChg(SV_VP_PIP, SV_VDO_UNKNOWN, 1);
                }
#ifdef CC_SRM_ON
                SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_STATUS + VDP_2), 0, 0);
#endif
            }
            break;

            // *********************************************************************
            /* Mode Change Done -- Mode Change Done! Inform UI to handle this! */
        case VDO_STATE_WAIT_MODE_DET:
            if (fgIsPipDec())
            {
                if (fgIsPipFlgSet(PIP_FLG_MODE_CHG))
                {
                    _bPipState = (UINT8)VDO_STATE_IDLE;
                    break;
                }

#ifdef DRV_SUPPORT_EXTMJC
		 if(_fgFirstPIP==FALSE)
               {
               	if ((!_fgAutoSearch))
               	{
                        vDrvExtmjcRedoTwoChipConnection( SV_VP_PIP, SV_TRUE);
               	}
               	_fgFirstPIP = TRUE;
               }	
                    
#endif
                

                csState = x_crit_start();
                flag = fgIsPipFlgSet(PIP_FLG_MODE_DET_DONE);
                vClrPipFlg(PIP_FLG_MODE_DET_DONE);
                x_crit_end(csState);

                if (flag)
                {
                     bSigMode = bDrvVideoSignalStatus(SV_VP_PIP);

                    // ---Handle mode change unmute--------
                    if ((bSigMode == (UINT8)SV_VDO_STABLE)
                    #if 0//SUPPORT_ATV_SNOWSCREEN
                         || (fgIsSrcAtv(SV_VP_PIP) && (bSigMode == (UINT8)SV_VDO_NOSIGNAL) && (u4ApiVideoGetSnowScreen()))
                    #endif 
                    )
                    {
                        #if DELAY_MODE_CHG_UNMUTE
                        //_u4PipDelayUnmute = DELAY_MODE_CHG_UNMUTE;
                       _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP,  _u4PipDelayUnmute, FALSE);
                        #else
                        if (fgIsPipDec() && !fgIsPipDTV())
                        {
                            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP,
                                              0, FALSE);
                        }
                        #endif
                    }
                    // end of mode change unmute

                    if (fgIsPipTvd3d())
                    {
                        vTvd3dBHModeDone();
                    }
                    _bPipState = (UINT8)VDO_STATE_IDLE;

                    if (bSigMode == (UINT8)SV_VDO_STABLE)
                    {

                        if (fgIsPipVga() || fgIsPipYPbPr()
                        #if SUPPORT_SCART
							|| fgIsPipScart()
							// ||((_bSubICIn == (UINT8)P_FB0) || (_bSubICIn == (UINT8)P_FB1) )
                        #endif
                        )
                        {
                            vDrvEnableBlankLevelAdjust();
                            // Printf("[SUB] -------vDrvEnableBlankLevelAdjust----\n");
                        }

                        // Notify audio for AV initial synchronization in ATV mode, assume in this time video is ready to display
                        if (!fgIsPipDTV())
                        {
                            LOG(9, "***** Vdo stable notify*****\n");
                            AUD_AtvAvInitialSync(TRUE);
#ifdef CC_AUD_MASK_AUDIO_SUPPORT
                            if (SV_VP_PIP == AUD_GetPIPVideoPath())
                            {
                                AUD_DspSetMaskAudio (AUD_DEC_MAIN, AUD_MASK_AUDIO_ON);
                                u1AudioMaskCount = AUDIO_MASK_TIME_COUNT;
                            }
#endif
                        }

                        #ifndef CC_UP8032_ATV
                        // Input Change Notify, ReCalculate Source Region
                        VDP_SourceChangeNotify(SV_VP_PIP);
                        #else
                        vSetPicSettingByUI(SV_VP_PIP);//XuYing 20090206
                        #endif

#if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
                        vApiTVEVideoStatusNotify(SV_VP_PIP,SV_TVE_NOTIFY_VIDEO_STABLE_SIGNAL);
#endif
#ifdef TVE_WA
                        TVE_FirstReset = 20;
#endif
                    }

                    vVdoModeChgDone(SV_VP_PIP);
                    DRVCUST_VdoModeChgDone(SV_VP_PIP);

                    if (bSigMode == (UINT8)SV_VDO_STABLE)
                    {
                        vSendEvevntToSRM(SV_VP_PIP) ;
                    }

                    if (!_fgVideoReady)
                    {
                        LOG(3, "Video Ready");
                        _fgVideoReady = TRUE;

                    }
                    vApiVideoSetEvtFlg(VDO_FLG_PIP_MODECHG_DONE);
                    vApiVideoClrEvtFlg(VDO_FLG_PIP_MODECHG);



                    if (fgIsPipDec())
                    {    //not notify when DTV input now move into api_notify layer
                        UINT8 rep;
                        rep=(!fgSkipNptvNfy())|| (!fgIsPipTvd3d());

                        vApiNotifySigChg(SV_VP_PIP,bSigMode,rep);

                        if (rep)
                        {
                            if (bSigMode == (UINT8)SV_VDO_STABLE)
                            {
                                vApiNotifyColorSystem(SV_VP_PIP, bSigMode);
                                vApiNotifyResolutionChg(SV_VP_PIP,bSigMode);
#ifdef DRV_SUPPORT_EXTMJC                                
                                _fgFirstPIP = FALSE;
#endif
                            }
                        }
                    }
                }
            }
            break;

        default:
            ASSERT(0);
            break;
    }
}

//SCART State Machine Gene Chang For the SCART Pin8 detection
#if SUPPORT_SCART
PRIVATE void vVdoScartState(void)
{
    UINT8 bScart1Status;
    UINT8 bScart2Status;

    bScart1Status = bDrvGetScartFSStatus(SV_VS_SCART1);
    if ((bScart1Status == FS_SCALE_CHANGE) || (bScart1Status == FS_ON_TO_OFF) || (bScart1Status == FS_OFF_TO_ON))
    {
        LOG(3, "scart1 fs notify %d %d\n", bScart1Status, bDrvGetScartPin8ASR(SV_VS_SCART1));
        vApiNotifyScartFastSwith(SV_VS_SCART1,bDrvGetScartPin8ASR(SV_VS_SCART1));
    }

    bScart2Status = bDrvGetScartFSStatus(SV_VS_SCART2);
    if ((bScart2Status == FS_SCALE_CHANGE) || (bScart2Status == FS_ON_TO_OFF) || (bScart2Status == FS_OFF_TO_ON))
    {
         LOG(3, "scart2 fs notify %d\n", bScart2Status, bDrvGetScartPin8ASR(SV_VS_SCART2));
        vApiNotifyScartFastSwith(SV_VS_SCART2,bDrvGetScartPin8ASR(SV_VS_SCART2));
    }
}
#endif

void vDddsSettingCheck(void)
{

#if CC_OSD_MUTE_WA
    //channel change will reset out region
    if (_bDddsDelayFlag && (!(_fgAutoSearch || SRM_IsEpgMode())) && SRM_IsMainVideoStrictStable())
    {
        vDDDSLCDSetting(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
        // reset non-standard signal status
        _u4NSDHTotal=0xff;
        _u4NSDVTotal=0xff;
        _u4NSDFrameRate=0xff;
        _bDddsDelayFlag = 0 ;
        #ifdef DRV_SUPPORT_EXTMJC
        vDrvExtmjcRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
        #endif
    }
#endif

    if ((_fgFRS_ENA == SV_ON) && fgIsMainTvd3d() 
        && (_eSyncLockMode == eSYNCLOCK_IGNORED))  	// Switch MTK Good Clock
    {
        if (bhwVCRSetting||(_na_state == 0x03)) //||!_fgHLOCK) //When VCR mode or Signal unsatble will OFF
        {
            vDrvSwitchMTKGoodDclk(SV_OFF);
        }
        else
        {
            vDrvSwitchMTKGoodDclk(SV_ON);
        }
    }
}

static void vDrvNSDNormPorc(UINT8 bPath)
{
    static UINT32 _u4NSDVOfst[SV_VP_NA] = {0x800, 0x800};
    UINT32 u4NSDVOfst;
    UINT8 bDispMode;
    UINT8 bType;

    if ((bPath >= SV_VP_NA) ||_fgAutoSearch )
    {
        return;
    }

    //When scaler dispmode, disable NSTD signal detection. 
    bDispMode = u1ScpipGetDispMode(bPath);
    if (bDispMode == VDP_SCPOS_DISPMODE_AUTO || bDispMode == VDP_SCPOS_DISPMODE_MANUAL) 
    {
        return;
    }

    bType = bGetVideoDecType(bPath);

    if (bType == SV_VD_TVD3D)
    {
        RTvdNSTDStatus rStatus;
        vTvd3dGetNSTDStatus(&rStatus);
       // if (rStatus.fgIsNSTD)
        {
            u4NSDVOfst = u4DrvVideoGetNSDOffset(bPath);
            if ((u4NSDVOfst != _u4NSDVOfst[bPath]) && (u4NSDVOfst <= 0x800))
            {
                // update source region
                UNUSED(bVideoUpdateSrcRegion(bPath));
                _u4NSDVOfst[bPath] = u4NSDVOfst;
            }

            if (bPath == SV_VP_MAIN)
            {
                if (bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
                {
                    _u4NSDHTotal=0xff;
                    _u4NSDVTotal=0xff;
                    _u4NSDFrameRate=0xff;
                }
                else
                // check if DDDS parameters are needed to update
                if((_u4NSDHTotal != rStatus.wHTotal)||(_u4NSDVTotal != rStatus.wVTotal)||(_u4NSDFrameRate != rStatus.bRefreshRate))
                {
                    vDDDSLCDSetting(rStatus.wVTotal, rStatus.bRefreshRate);
                    vDrvSetSSMDRAsInputHsync();

                    _u4NSDHTotal = rStatus.wHTotal;
                    _u4NSDVTotal = rStatus.wVTotal;
                    _u4NSDFrameRate = rStatus.bRefreshRate;
                }
            }
        }
    }
    else if (bType == SV_VD_YPBPR)
    {
        RHDTVNSTDStatus rHDTVStatus;
        vHDTVGetNSTDStatus(&rHDTVStatus);

        u4NSDVOfst = u4DrvVideoGetNSDOffset(bPath);
        if ((u4NSDVOfst != _u4NSDVOfst[bPath]) && (u4NSDVOfst <= 0x800))
        {
            // update source region
            UNUSED(bVideoUpdateSrcRegion(bPath));
            _u4NSDVOfst[bPath] = u4NSDVOfst;
        }

        if (bPath == SV_VP_MAIN)
        {
            if (bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
            {
                _u4NSDHTotal=0xff;
                _u4NSDVTotal=0xff;
                _u4NSDFrameRate=0xff;
            }
            else
            // check if DDDS parameters are needed to update
            if((_u4NSDHTotal != rHDTVStatus.wHTotal)||(_u4NSDVTotal != rHDTVStatus.wVTotal)||(_u4NSDFrameRate != rHDTVStatus.bRefreshRate))
            {
                vDDDSLCDSetting(rHDTVStatus.wVTotal, rHDTVStatus.bRefreshRate);
                vDrvSetSSMDRAsInputHsync();
                _u4NSDHTotal = rHDTVStatus.wHTotal;
                _u4NSDVTotal = rHDTVStatus.wVTotal;
                _u4NSDFrameRate = rHDTVStatus.bRefreshRate;
            }
        }

    }
}


/**
 * video state machine.
 */

void vApiVideoMainloop(void)
{
#ifdef CC_ENABLE_AV_SYNC 
	UINT32 i ;
#endif

#ifdef CC_AUD_MASK_AUDIO_SUPPORT
    UINT32 u4MuteMainFlag;
    UINT32 u4MutePIPFlag;
#endif

	// Check SW Register
	vSWRegMainLoop();	

    // dynamic local dimming
    vDrvAdaptiveLcDim();
#ifdef CC_SCPOS_3DTV_SUPPORT
    vDrvTDTVProc();
#endif

#ifdef  DYNANMICS_USB_SWITCH
	if(fgIsSrcMM() && _u4BypassNptvMloop)
	{
        //vVdoMainState();
        vDrvDIChkImpl(SV_VP_MAIN);
        vScpipNormProc();
		_vDrvVideoHandleUnMute();
	}
#endif

    // PQ SRAM Stress Test Procedure
#if defined(CC_DEBUG)
    vDrvPQStressProc();
#endif //#if defined(CC_DEBUG)
    if (_u4BypassNptvMloop & BYPASS_NPTV_All)
    {
        return ;
    }

    // Trigger SRM to enter scan mode from AP
    if (fgIsSrcAtv(SV_VP_MAIN) || fgIsMainDTV() || (fgIsMainDec() == SV_VD_MAX) || (fgIsMainDec() == SV_VD_NA))
    {
         if (_fgAutoSearch != u4ApiVideoGetScanMode(SV_VP_MAIN))
         {
            vApiVideoSetScanMode(SV_VP_MAIN, (UINT32)_fgAutoSearch);
			//force update 3x3 or 444/422 paramter CR:DTV00140207
			vVdoModeChgDone(SV_VP_MAIN);
         }
    }
    else  // non TVD case
    {
         if (u4ApiVideoGetScanMode(SV_VP_MAIN))
         {
            vApiVideoSetScanMode(SV_VP_MAIN, 0);
         }
    }
    if (fgIsSrcAtv(SV_VP_PIP) || fgIsPipDTV() || (fgIsPipDec() == SV_VD_MAX)|| (fgIsPipDec() == SV_VD_NA))
    {
         if (_fgAutoSearch != u4ApiVideoGetScanMode(SV_VP_PIP))
         {
            vApiVideoSetScanMode(SV_VP_PIP, (UINT32)_fgAutoSearch);
			vVdoModeChgDone(SV_VP_PIP);
         }
    }    
    else  // non TVD case
    {
         if (u4ApiVideoGetScanMode(SV_VP_PIP))
         {
            vApiVideoSetScanMode(SV_VP_PIP, 0);
         }
    }


    if ((_u4BypassNptvMloop & BYPASS_NPTV_HDMI) == 0)
    {
        // HDMI State Machine
        vHDMIMainLoop();
    }
    if ((_u4BypassNptvMloop & BYPASS_NPTV_TVD) == 0)
    {
        vTvd3dMainLoop();
    }

    // Main Video State Machine
    vVdoMainState();



#ifdef CC_AUD_MASK_AUDIO_SUPPORT
    u4MuteMainFlag = _u4DrvVideoGetMute(SV_VP_MAIN);
    if (u4MuteMainFlag == SV_OFF)
    {
        if (SV_VP_MAIN == AUD_GetPIPVideoPath())
        {
            if (u1AudioMaskCount)
            {
                AUD_DspSetMaskAudio (AUD_DEC_MAIN, AUD_MASK_AUDIO_OFF);
                u1AudioMaskCount = 0;
            }
        }
    }  

    u4MutePIPFlag = _u4DrvVideoGetMute(SV_VP_PIP);
    if (u4MutePIPFlag == SV_OFF)
    {
        if (SV_VP_PIP == AUD_GetPIPVideoPath())
        {
            if (u1AudioMaskCount)
            {
                AUD_DspSetMaskAudio (AUD_DEC_MAIN, AUD_MASK_AUDIO_OFF);
                u1AudioMaskCount = 0;
            }
        }
    }   

    if (u1AudioMaskCount)
    {
        u1AudioMaskCount--;
        if (!u1AudioMaskCount)
        {
            AUD_DspSetMaskAudio (AUD_DEC_MAIN, AUD_MASK_AUDIO_OFF);
        }
    }
#endif    
#ifdef CC_ENABLE_AV_SYNC 
	//Check the Main/Sub path Audio Cb trigger
	for(i=0 ; i < 2 ; i ++)
	{
		if (_u4AudStableTriggerCnt[i])
		{
			_u4AudStableTriggerCnt[i]--;
			if (!_u4AudStableTriggerCnt[i])
			{
				Printf("e\n");
				AUD_VdpModeChangeDone();
			}
		}
	}
#endif
    
#ifdef DRV_SUPPORT_EXTMJC
    if(_u48282UiUnmteTimeout)
        {
            UINT8 u1MsgErr;
            LOG(3, "8280 delay unfastmute counter: %d\n", _u48282UiUnmteTimeout);
        	_u48282UiUnmteTimeout--;
#ifdef DRV_SUPPORT_EXTMJC_MODE7
		if(_u48282UiUnmteTimeout==8)
		{
			IGNORE_RET(OSD_DMA_TransferRegion((UINT32)OSD_PLANE_2));
		}
#endif
        	
        	if (_u48282UiUnmteTimeout==0)
        	{
        	      //Printf("\n Fast Mute Time out");
        		d_custom_extmjc_set_fastmute(FALSE, &u1MsgErr);
        	}
        }
#endif



#ifdef DRV_SUPPORT_EXTMJC
    vDrvExtmjcNormProc();
#endif//DRV_SUPPORT_EXTMJC

    // PIP Video State Machine
    vVdoPipState();

#if SUPPORT_S_DETECT
    vSVPresDet();
#endif

#ifdef  SUPPORT_AV_COMP_AUTO
	vAVCompAutoDet();
#endif

#ifdef CC_SOURCE_AUTO_DETECT
		vDrvDetectSourceState();
#endif

    // VGA State Machine
    if (!_IsVgaDetectDone)
    {
        vVgaModeDetect();
    }
    else
    {
        vVgaChkModeChange();
    }

    // DVI State Machine
#if SUPPORT_DVI
    if (!_IsDviDetectDone)
    {
        vDviModeDetect();
    }
    else
    {
        vDviChkModeChange();
    }
#endif

    // HDTV State Machine
    if (!_IsHdtvDetectDone)
    {
        vHdtvModeDetect();
    }
    else
    {
        vHdtvChkModeChange();
    }

    // VGA auto state machine
    vVdoSP0AutoState();

    #if SUPPORT_SCART
    if ((_u4BypassNptvMloop & BYPASS_NPTV_SCART) == 0)
    {
        if (VSS_Support_Major(VSS_SCART))
        {
            vScartEvtchk();
            //SCART State Machine Gene Chang For the SCART Pin8 detection
            vVdoScartState();
        }
    }
    #endif

    vDrvAdjustBlankLevel();
    vDrvOnChipAutoColorIteration();  //do auto color
    vDrvPGALinearityVerify();
    vDrvSDDS_Response();
    vDrvDDDS_Response();
    #if TVD_AUTO_CALIB
    if ((_u4BypassNptvMloop & BYPASS_NPTV_TVD) == 0)
    {
        // TVD Auto calibration State Machine
        vDrvTVDAutoCalibState();
    }
    #endif

#if THERMAL_COMPENSATION_ENABLE
    vDrvThermalCompensate();
#endif

    if ((bDrvVideoSignalStatus(SV_VP_MAIN) == (UINT8)SV_VDO_STABLE) ||
        (bDrvVideoSignalStatus(SV_VP_PIP) == (UINT8)SV_VDO_STABLE))
    {
        if ((_u4BypassNptvMloop & BYPASS_NPTV_NR) == 0)
        {
            // NR adaptive control : Line calculation and Auto NR
            vDrvNRProc();
        }
        
        if ((_u4BypassNptvMloop & BYPASS_NPTV_TVD) == 0)
        {
            vTdc3dProc();
        }

        if ((_u4BypassNptvMloop & BYPASS_COLOR_PROC) == 0)
        {
            vDrvYcprocProc();
        }
    }
    
    if ((_u4BypassNptvMloop & BYPASS_NPTV_PSCAN) == 0)
    {
        vDrvDIChkImpl(SV_VP_MAIN);
        vDrvDIChkImpl(SV_VP_PIP);
    }

    if ((_u4BypassNptvMloop & BYPASS_NPTV_SCPOS) == 0)
    {
        // Scpos State Machine
        vScpipNormProc();
    }

    #ifdef CC_MT5395
    if ((_u4BypassNptvMloop & BYPASS_NPTV_MJC) == 0)
    {
        vDrvMJCMainloop();
    }
    #endif

#ifdef CC_SUPPORT_TVE
    if ((_u4BypassNptvMloop & BYPASS_NPTV_TVE) == 0)
    {
        _TVE_Mainloop();
    }
#endif

    // non-standard signal normal procedure
    if ((_u4BypassNptvMloop & BYPASS_NSD_PROC) == 0)
    {
        vDrvNSDNormPorc(SV_VP_MAIN);
        vDrvNSDNormPorc(SV_VP_PIP);
    }
    vDrvFrameTrackProc();

    if ((_u4BypassNptvMloop & BYPASS_NPTV_DDDS) == 0)
    {
        vDddsSettingCheck();
    }

    // handle mute event
    _vDrvVideoHandleUnMute();

    DRVCUST_VideoMainloop();
}

#ifdef CC_COPLAT_MT82
BOOL fgIsVideoDone(UINT8 bPath)
{
	if((bPath == SV_VP_MAIN) && (_bMainState == VDO_STATE_IDLE) && !fgIsMainFlgSet(MAIN_FLG_MODE_CHG))
	{
		return TRUE;
	}
	else if((bPath == SV_VP_PIP) && (_bPipState == VDO_STATE_IDLE) && !fgIsPipFlgSet(PIP_FLG_MODE_CHG))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL fgApiVideoIsReady()
{
	return _fgVideoReady;
}
#endif
