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
 * $Author: youneng.xu $
 * $Date: 2013/01/15 $
 * $RCSfile: mloop_video.c $
 * $Revision: #7 $
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "util.h"
#include "video_def.h"
#include "panel.h"
#include "source_select.h"
#include "c_model.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_bim.h"
#include "x_timer.h"
#include "hw_vdoin.h"
#include "hw_nr.h"
#include "hw_scpos.h"
#include "hw_tdc.h"
#include "hw_sw.h"
#include "hw_pdwnc.h"
#include "drv_video.h"
#include "drv_tvd.h"
#include "drv_hdtv.h"
#include "drv_di.h"
#include "drv_nr.h"
#include "drv_contrast.h"
#include "drv_autocolor.h"
#include "drv_ycproc.h"
#include "drv_lcdim.h"
#include "drv_scaler.h"
#include "drv_upscaler.h"
#include "drv_tdc.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_scpos.h"
#include "drv_od.h"
#include "drv_hdmi.h"
#include "drv_display.h"
#include "drv_mpeg.h"
#include "drv_tdtv_drvif.h"
#include "api_notify.h"
#include "api_srch_if.h"
#include "nptv_drvif.h"
#include "nptv_debug.h"
#include "vdp_drvif.h"
#include "vdp_display.h"
#include "vdo_if.h"
#include "vdo_misc.h"
#include "srm_drvif.h"
#include "tve_if.h"
#include "pe_if.h"
#include "drvcust_if.h"
#include "eepdef.h"
#include "mute_if.h"
#include "frametrack_drvif.h"

#include "drv_tdnavi.h"
#include "drv_meter.h"

#ifdef CC_MT5396
#include "drv_mjc.h"
#endif

#if SUPPORT_VBI
#include "drv_vbi.h"
#endif

#if SUPPORT_SCART
#include "drv_scart.h"
#endif

#ifndef CC_COPLAT_MT82
#include "aud_if.h"
#include "aud_drvif.h"
#else
#include "general_mt82.h"
#define AUD_AtvAvInitialSync(fgStable)
#endif

#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif

#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#include "srm_drvif.h"
#include "sif_if.h"
#ifdef DRV_SUPPORT_EXTMJC_MODE7
#include "osd_drvif.h"
#endif
#endif

#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif

#define DELAY_MODE_CHG_UNMUTE 10  //[DTV00130707]

////////////////////////////////////////////////////////////////////////////////
BOOL _fgVideoReady;
BOOL _fgESDProtect;
#ifdef DRV_SUPPORT_EXTMJC
BOOL _fgFirstPIP=FALSE;
#endif

extern UINT8 _bHDMIColorSpace;
extern UINT32 u4PeInitDone;
////////////////////////////////////////////////////////////////////////////////

#if DELAY_MODE_CHG_UNMUTE
static UINT32 _u4MainDelayUnmute = DELAY_MODE_CHG_UNMUTE;
static UINT32 _u4PipDelayUnmute = DELAY_MODE_CHG_UNMUTE;
#endif

#ifdef CC_ENABLE_AV_SYNC                                               
#define DELAY_MODE_CHG_AUD_STABLE	3
static UINT32 _u4AudStableTriggerCnt[2]= {0,0};
#endif

UINT32 _u4BypassNptvMloop = 0;

UINT32 _u4SigDetDone;

/* boot up measurement in release version, record once.*/
static BOOL b_boot_rec_once = FALSE;

#ifdef TVE_WA
extern UINT8 TVE_FirstReset;
#endif

static HAL_TIME_T _rHalTime;

#ifdef CC_AUD_MASK_AUDIO_SUPPORT
#define AUDIO_MASK_TIME_COUNT   60
static UINT8 u1AudioMaskCount = 0;
#endif

#if defined(CC_SOURCE_AUTO_DETECT) || defined(CC_SRCDETECT_CVBS_SUPPORT)
UINT8 u1SrcDet_Int_Flag = 0;
#endif


#ifdef CC_COPLAT_MT82
extern void vSetPicSettingByUI(UINT8 bPath);
#endif

void vVdoPEModeChgDone(UINT8 bPath)
{
    //1. fire Scaler mode change
    vScpipReconfig(bPath);
    
    vVdoSetColorTransform(bPath);
    vDrvDIModeChangeDone(bPath);
    #ifdef CC_MT5396
    vDrvMJCModeChangeDone(bPath);
    #endif

    // Apply all PQ related setting again
    vApiApplyPQSetting(bPath);

    // Reset OD overflow flag
    vDrvODDramOverflowReset();

    // Reset St Histogram init flag
    vDrvSatHistWinReconfig();    
}

void vVdoModeChgDone(UINT8 bPath)
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
    
    if (bPath == SV_VP_MAIN)
    {
        vDrvMain422(use_422);
    }
    else
    {
        vDrvPip422(use_422);
    }
    LOG(3, "Mode change done 422:%d \n", use_422);

    //notify mode change done for all video path module (DI, Scaler, MJC, FrameTrack....), and apply all PQ setting.
    vVdoPEModeChgDone(bPath);
    
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
	u1DrvTdNaviModeChange();
	u1DrvRealDModeChange(bPath);

    vScpipOnVdoModeChangeDone(bPath);
}

void vVdoSendEvevntToSRM(UINT8 bPath)
{
    UINT32 u4VgaMode;
    UINT32 u4ColorMode444;

#ifdef CC_SRM_ON
    UINT32 u4RefreshRate;
#endif

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

#if !SUPPORT_SCALERYUV444
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
#endif    

#ifdef CC_SRM_ON
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
PRIVATE void vVdoMainState(void)
{
    UINT8 bSigMode;
    BOOL flag;
    CRIT_STATE_T csState;

    switch (_bMainState)
    {
    /* Idle -- nothing to do for video */
    case VDO_STATE_IDLE:
        csState = x_crit_start();
        flag = fgIsMainFlgSet(MAIN_FLG_MODE_CHG);
        vClrMainFlg(MAIN_FLG_MODE_CHG);
        x_crit_end(csState);

        if (flag)
        {
        	if (_fgVideoReady&&_fgESDProtect)
            {
                LOG(3, "Video Ready-->0\n");
                _fgVideoReady = FALSE;
            }
		vCheckDeocerStable(SV_DISABLE_DEC_STABLE_CHK);
        	#if defined(CC_SOURCE_AUTO_DETECT) || defined(CC_SRCDETECT_CVBS_SUPPORT)
			u1SrcDet_Int_Flag = TRUE;
			#endif            

            #if SUPPORT_SCART
			if (fgIsMainScart())
            {
                vDrvScartRGBFreeRun(SV_ON);
            }
            #endif

            AUD_AtvAvInitialSync(FALSE);
            vDrvSwitchMTKGoodDclk(SV_OFF);

            if ((!(_fgAutoSearch && fgIsMainTvd3d())) && (fgIsMainDec() && !fgIsMainDTV()))
            {
                _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
            }
            
            //notify scaler that video state is changed.
            vScpipOnVdoModeChange(SV_VP_MAIN);

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
                LOG(3, "Change to VDO_STATE_WAIT_MODE_DET\n");
            }

            #ifdef CC_ENABLE_AV_SYNC 
            if (SV_VP_MAIN == AUD_GetPIPVideoPath())          
            {                                                 
                AUD_VdpModeChange();
			        _u4AudStableTriggerCnt[SV_VP_MAIN]= 0;
            }                                                 
            #endif

            #if defined(CC_SUPPORT_TVE) || defined(CC_SUPPORT_TVE_82xx)
            vApiTVEVideoStatusNotify(SV_VP_MAIN, SV_TVE_NOTIFY_VIDEO_MODE_CHANGE);
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

    /* Mode Change Done -- Mode Change Done! Inform UI to handle this! */
    case VDO_STATE_WAIT_MODE_DET:
        if (fgIsMainDec())
        {
            if (fgIsMainFlgSet(MAIN_FLG_MODE_CHG))
            {
                _bMainState = VDO_STATE_IDLE;
                LOG(3, "Change to VDO_STATE_IDLE #1\n");
                break;
            }

            csState = x_crit_start();
            flag = fgIsMainFlgSet(MAIN_FLG_MODE_DET_DONE);
            vClrMainFlg(MAIN_FLG_MODE_DET_DONE);
            x_crit_end(csState);

            if (flag)
            {
                #if SUPPORT_DOT_TO_DOT_PATTERN                   
                VDP_DotToDotPattern(0, 0);// when change source, close dot_to_dot pattern		
    	        #endif

                bSigMode = bDrvVideoSignalStatus(SV_VP_MAIN);

                // ---Handle mode change unmute--------
                if ((bSigMode == (UINT8)SV_VDO_STABLE))
                {
                    #if DELAY_MODE_CHG_UNMUTE
                    _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, _u4MainDelayUnmute, FALSE);
                    #else
                    if (fgIsMainDec() && !fgIsMainDTV())
                    {
                        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN,  0, FALSE);
                    }
                    #endif
                }

                if (fgIsMainTvd3d())
                {
                    vTvd3dBHModeDone();
                }

                /*    When the input source is no signal, turn off Display Mode. SJ Added 050812: Begin    */
                if (bSigMode == (UINT8)SV_VDO_NOSIGNAL)
                {
                    // will done in mode change
                    #if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
                    vApiTVEVideoStatusNotify(SV_VP_MAIN,SV_TVE_NOTIFY_VIDEO_NO_SIGNAL);
                    #endif
                }

                _bMainState = (UINT8)VDO_STATE_IDLE;
                LOG(3, "Stable, Change to VDO_STATE_IDLE #2 bSigMode = %d\n", bSigMode); 

                if (bSigMode == (UINT8)SV_VDO_STABLE)
                {
			vCheckDeocerStable(SV_ENABLE_DEC_STABLE_CHK);
			
                    if (fgIsMainVga() || fgIsMainYPbPr()
                        #if SUPPORT_SCART
    					|| fgIsMainScart()
                        #endif
                        )
                    {
                        vDrvEnableBlankLevelAdjust();
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
                    vVdoSendEvevntToSRM(SV_VP_MAIN) ;
                }

                #ifdef DRV_SUPPORT_EXTMJC
                if (bSigMode == (UINT8)SV_VDO_STABLE
                    #if CC_OSD_MUTE_WA
                                && (!_bDddsDelayFlag)
                    #endif
				)
                {
                    vDrvExtmjcRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
                }
                #endif//DRV_SUPPORT_EXTMJC

                #if SUPPORT_VBI
                _u4SigDetDone = 1;
                #endif

                if ((!_fgVideoReady)&&_fgESDProtect)
                {
                    LOG(3, "Video Ready\n");
                    _fgVideoReady = TRUE;
                }

                vApiVideoSetEvtFlg(VDO_FLG_MAIN_MODECHG_DONE);
                vApiVideoClrEvtFlg(VDO_FLG_MAIN_MODECHG);

                // Add for 5371: should we notify as soon as possible?
                if (fgIsMainDec())
                {
                    //not notify when DTV input now move into api_notify layer
                    UINT8 rep = (!fgSkipNptvNfy()) || (!fgIsMainTvd3d());

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

                //notify scaler that video state is changed.
                vScpipOnVdoModeChange(SV_VP_PIP);

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
                        vVdoSendEvevntToSRM(SV_VP_PIP) ;
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

#if SUPPORT_SCART
// SCART State Machine Gene Chang For the SCART Pin8 detection
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

void vAudMaskChk(UINT8 bPath)
{
    #ifdef CC_AUD_MASK_AUDIO_SUPPORT
    UINT32 u4MuteFlag;

    u4MuteFlag = _u4DrvVideoGetMute(bPath);
    if (u4MuteFlag == SV_OFF)
    {
        if (AUD_GetPIPVideoPath() == bPath)
        {
            if (u1AudioMaskCount)
            {
                AUD_DspSetMaskAudio(AUD_DEC_MAIN, AUD_MASK_AUDIO_OFF);
                u1AudioMaskCount = 0;
            }
        }
    }
    #endif
}

void vVdoScanModeChk(UINT8 bPath)
{
    if (fgIsSrcAtv(bPath) || fgIsSrcDTV(bPath) || (fgIsSrcDec(bPath) == SV_VD_MAX) || (fgIsSrcDec(bPath) == SV_VD_NA))
    {
         if (_fgAutoSearch != u4ApiVideoGetScanMode(bPath))
         {
            vApiVideoSetScanMode(bPath, (UINT32)_fgAutoSearch);
			//force update 3x3 or 444/422 paramter CR:DTV00140207
			vVdoModeChgDone(bPath);
         }
    }
    else  // non TVD case
    {
         if (u4ApiVideoGetScanMode(bPath))
         {
            vApiVideoSetScanMode(bPath, 0);
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
       
    //time 0 measure nptv enter period time.
    SYS_MEASURE_TIME_END(SV_MLOOP_TIME0);
    SYS_PRINT_TIME(SV_MLOOP_TIME0); 
    SYS_MEASURE_TIME_START(SV_MLOOP_TIME0);

        //time1 measure the nptv mloop time.
        SYS_MEASURE_TIME_START(SV_MLOOP_TIME1);
        
        vCheckDeocerStable(SV_CHECK_DEC_HV);
        
	// Check SW Register
	vSWRegMainLoop();

    #ifdef DYNANMICS_USB_SWITCH
	if (fgIsSrcMM() && _u4BypassNptvMloop)
	{
        vDrvMIBProc();
        vScpipNormProc();
		vDrvVideoHandleUnMute();
	}
    #endif

    if (fgIsBypassFlgSet(BYPASS_NPTV_All))
    {
        return ;
    }

    if (fgIsBypassFlgClr(BYPASS_NPTV_TDTV))
    {
        vDrvTDTVProc();
    }

    if (fgIsBypassFlgClr(BYPASS_SCAN_CHK))
    {
        vVdoScanModeChk(SV_VP_MAIN);
        vVdoScanModeChk(SV_VP_PIP);
    }

    if (fgIsBypassFlgClr(BYPASS_NPTV_HDMI))
    {
        vHDMIMainLoop();
    }

    if (fgIsBypassFlgClr(BYPASS_NPTV_TVD))
    {
        vTvd3dMainLoop();
    }

    if (fgIsBypassFlgClr(BYPASS_VDO_MAIN_STATE))
    {
        vVdoMainState();
    }

    if (fgIsBypassFlgClr(BYPASS_VDO_PIP_STATE))
    {
        vVdoPipState();
    }

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

    //#ifdef CC_AUD_MASK_AUDIO_SUPPORT
    vAudMaskChk(SV_VP_MAIN);
    vAudMaskChk(SV_VP_PIP);
    //#endif
    
    if (fgIsBypassFlgClr(BYPASS_SRC_DETECT))
    {
        #if SUPPORT_S_DETECT
        vSVPresDet();
        #endif

        #ifdef  SUPPORT_AV_COMP_AUTO
    	vAVCompAutoDet();
        #endif

        #if defined(CC_SOURCE_AUTO_DETECT) || defined(CC_SRCDETECT_CVBS_SUPPORT)
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

        #if SUPPORT_DVI
        // DVI State Machine
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
        if (fgIsBypassFlgClr(BYPASS_NPTV_SCART))
        {
            if (VSS_Support_Major(VSS_SCART))
            {
                vScartEvtchk();
                //SCART State Machine Gene Chang For the SCART Pin8 detection
                vVdoScartState();
            }
        }
        #endif
    }

    vDrvAdjustBlankLevel();
    vDrvOnChipAutoColorIteration(); // do auto color
    vDrvPGALinearityVerify();
    vDrvSDDS_Response();
    vDrvDDDS_Response();

    #if TVD_AUTO_CALIB
    if (fgIsBypassFlgClr(BYPASS_NPTV_TVD))
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
        if (fgIsBypassFlgClr(BYPASS_NPTV_NR))
        {
            vDrvNRProc();
        }
        
        if (fgIsBypassFlgClr(BYPASS_NPTV_TVD))
        {
            vTdc3dProc();
        }

        if (fgIsBypassFlgClr(BYPASS_NPTV_YCPROC))
        {
            vDrvYcprocProc();
        }
    }

    // Gamma should run even if signal unstable / no signal
    vDrvGammaProc();
    vDrvLcdimProc();


    if (fgIsBypassFlgClr(BYPASS_NPTV_PSCAN))
    {
        vDrvMIBProc();        
    }

    if (fgIsBypassFlgClr(BYPASS_NPTV_SCPOS))
    {
        vScpipNormProc();
    }

    #ifdef CC_MT5396
    if (fgIsBypassFlgClr(BYPASS_NPTV_MJC))
    {
        vDrvMJCMainloop();
    }
    #endif

    #ifdef CC_SUPPORT_TVE
    if (fgIsBypassFlgClr(BYPASS_NPTV_TVE))
    {
        _TVE_Mainloop();
    }
    #endif

    // non-standard signal normal procedure
    if (fgIsBypassFlgClr(BYPASS_NSD_PROC))
    {
        vDrvNSDNormPorc(SV_VP_MAIN);
        vDrvNSDNormPorc(SV_VP_PIP);
    }
    if (fgIsBypassFlgClr(BYPASS_NPTV_FRAME_TRACK))
    {
        vDrvFrameTrackProc();
    }
    if (fgIsBypassFlgClr(BYPASS_NPTV_DDDS))
    {
        vDddsSettingCheck();
    }

    #ifdef DRV_SUPPORT_EXTMJC
    vDrvExtmjcNormProc();
    #endif//DRV_SUPPORT_EXTMJC
    
    if (fgIsBypassFlgClr(BYPASS_NPTV_OD))
    {
        vDrvODMloop();
    }

    vDrvVideoHandleUnMute();
    #ifndef NDEBUG
    vDrvVideoDramStatusChk();
    #endif
    DRVCUST_VideoMainloop();

    SYS_MEASURE_TIME_END(SV_MLOOP_TIME1);
    SYS_PRINT_TIME(SV_MLOOP_TIME1); 
        
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

