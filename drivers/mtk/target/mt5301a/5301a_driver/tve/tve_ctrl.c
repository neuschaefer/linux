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

#ifndef _TVE_CTRL_C_
#define _TVE_CTRL_C_

#include "general.h"
#include "x_assert.h"
#include "sv_const.h"
#include "hw_vdoin.h"
#include "tve_if.h"
#include "nptv_if.h"
#include "source_table.h"
#include "video_def.h"
#include "vdp_drvif.h"
#include "tve_hal.h"
#include "source_select.h"
#include "drv_adcset.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "hw_tvd.h"
#include "srm_drvif.h"
#include "eepdef.h"
#include "api_eep.h"
#include "vdo_misc.h"
#include "hw_scpos.h"
#include "hw_di.h" 
#include "tve_debug.h"
#include "drv_tvd.h"
#include "drv_hdmi.h"
#include "drv_async.h"
#include "mute_if.h"
#include "b2r_if.h"
#include "vdp_if.h"

/******************************************************************************
* local definition, TVE configuration
******************************************************************************/

/******************************************************************************
* Function prototype
******************************************************************************/
#ifndef CC_MT5301A
static void _vApiTveSetSclPara(UINT8 u1OnOff, INT32 i4Width, INT32 i4Height)
{
    VDP_REGION_T rRegion;
    VDP_OVERSCAN_REGION_T rOvRegion;

    VDP_SetDisplayOff(VDP_2, u1OnOff);

    if(i4Width >=0 && i4Height >= 0)
    {
        rRegion.u4X = 0; 
        rRegion.u4Y = 0;
        if(VDP_GetScalerPixelBased())
        {
            rRegion.u4Width = i4Width;
            rRegion.u4Height = i4Height;        
        }
        else
        {
            rRegion.u4Width = i4Width * VDP_MAX_REGION_WIDTH / PANEL_GetPanelWidth() + (((i4Width*VDP_MAX_REGION_WIDTH)%PANEL_GetPanelWidth()!=0)?(1):(0));
            rRegion.u4Height = i4Height * VDP_MAX_REGION_HEIGHT / PANEL_GetPanelHeight() + (((i4Height*VDP_MAX_REGION_HEIGHT)%PANEL_GetPanelHeight()!=0)?(1):(0));                
        }
        VDP_SetOutRegion(VDP_2, 0, rRegion);

        //rRegion.u4Width = VDP_MAX_REGION_WIDTH;
        //rRegion.u4Height = VDP_MAX_REGION_HEIGHT;
        //VDP_SetSrcRegion(VDP_2,0, rRegion);

        rOvRegion.u4Top = 0;
        rOvRegion.u4Bottom = 0;
        rOvRegion.u4Left = 0;
        rOvRegion.u4Right = 0;
        VDP_SetOverScan(VDP_2, rOvRegion);
    }

    return;
}
#endif

static UINT8 au1DacMuteMask[2] = {TVE_DAC_MUTE_MASK_MW|TVE_DAC_MUTE_MASK_DRV,TVE_DAC_MUTE_MASK_MW|TVE_DAC_MUTE_MASK_DRV};
static UINT8 abBypPath[2] = {0, 0};
static UINT8* apu1SourceType[2] = {NULL, NULL};
extern UINT32 TVE_HalSetMixerDacSel(UCHAR DAC, UCHAR bSrc);

static void (*_TveNotifySigChg)(UINT8 bPath, UINT8 u1SigState) = NULL;
static void (*_TveNotifyUnmute)(UINT8 bPath) = NULL;
#ifndef CC_MT5301A
static void TVE_SetCCNfy(UINT8 u1Path,
                                 UINT8 u1data1, UINT8 u1data2, UINT8 u1field)
{
    UINT8 ucData[2], bEnable;
    ucData[0] = u1data1;
    ucData[1] = u1data2;

    TVE_GetCcEnable(TVE_1, &bEnable);
    if (!bEnable)
    {
        TVE_SetCcEnable(TVE_1, SV_ON);
    }
	
    TVE_SendCc(TVE_1, u1field, ucData, 0);
    TVE_SendCcIsr(TVE_1, TVE_GetFieldInfo(TVE_1), 0);
}

static void TVE_SetWSS625Nfy(UINT8 u1Path, UINT32 u4Data)
{
    UINT8 ucData[2], bEnable;
    ucData[0] = (UINT8)(u4Data&0xff);
    ucData[1] = (UINT8)((u4Data>>8)&0xff);

    TVE_GetWssEnable(TVE_1, &bEnable);
    if (!bEnable)
    {
        TVE_SetWssEnable(TVE_1, SV_ON);
    }

    TVE_SendWss(TVE_1, ucData, 2);
}

static void TVE_SetWSS525Nfy(UINT8 u1Path, UINT32 u4Data)
{
    UINT8 ucData[3], bEnable;
    ucData[0] = (UINT8)(u4Data&0xff);
    ucData[1] = (UINT8)((u4Data>>8)&0xff);
    ucData[2] = (UINT8)((u4Data>>16)&0xff);

    TVE_GetWssEnable(TVE_1, &bEnable);
    if (!bEnable)
    {
        TVE_SetWssEnable(TVE_1, SV_ON);
    }

    TVE_SendWss(TVE_1, ucData, 3);
}
#endif
static void _vTveSigChgFunc(UINT8 bPath, UINT8 u1SigState)
{
#ifndef CC_MT5301A
    UINT8 ucEnable;
#endif
    if(u1SigState == SV_VDO_NOSIGNAL)
    {

        if( bApiTVEGetBypPath(DAC_1)!=TVE_BYP_EXT_PATH && 
            apu1SourceType[DAC_1] != NULL &&
            bGetVideoDecType(bPath) == bGetDecodeType(*apu1SourceType[DAC_1]) )
        {
            vApiTVESetDacMuteMask(DAC_1, TVE_DAC_MUTE_MASK_DRV, TRUE);
        }

        if( bApiTVEGetBypPath(DAC_2)!=TVE_BYP_EXT_PATH && 
            apu1SourceType[DAC_2] != NULL &&
            bGetVideoDecType(bPath) == bGetDecodeType(*apu1SourceType[DAC_2]) )
        {
            vApiTVESetDacMuteMask(DAC_2, TVE_DAC_MUTE_MASK_DRV, TRUE);
        }
#ifndef CC_MT5301A
        if(DRVCUST_OptGet(eTVESupportAFD))
        {
            TVE_SetCcEnable(TVE_1, SV_OFF);
            TVE_SetWssEnable(TVE_1, SV_OFF);
            VBI_SetTVEWSS525Nfy(NULL);
            VBI_SetTVEWSS625Nfy(NULL);
            VBI_SetTVECCNfy(NULL);	
        }
#endif
    }

#ifndef CC_MT5301A
    if(bPath == VDP_2)
    {
        if(u1SigState == SV_VDO_STABLE)
        {
            TVE_GetEnable(TVE_1, &ucEnable);
            if(ucEnable != FALSE)
            {
                if(//wApiVideoGetInputHeight(bPath) < 576 ||
                   bApiVideoGetRefreshRate(bPath) == 60 ||
                   bApiVideoGetRefreshRate(bPath) == 30 ||
                   bApiVideoGetRefreshRate(bPath) == 24)
                {
                    vApiTVESetOutputFormat(SV_TVE_OFMT_NTSC);
                    //if(DRVCUST_OptGet(eTVESupportAFD) == 1)
                    //{
                    //    TVE_SetWssEnable(TVE_1, SV_OFF);
                    //}
                }
                else
                {
                    vApiTVESetOutputFormat(SV_TVE_OFMT_PAL);
                    //if((_afgScartWithDtvSource[_rTveUsed.TveIsUsedBy] != FALSE) &&
                    //(DRVCUST_OptGet(eTVESupportAFD) == 1))
                    //{
                    //    TVE_SetWssEnable(TVE_1, SV_ON);
                    //}
                }

                if(DRVCUST_OptGet(eTVESupportAFD))
                {
                    if(bGetVideoDecType(bPath)!=SV_VD_MPEGHD &&
                       bGetVideoDecType(bPath)!=SV_VD_MPEGSD )//not DTV
                    {
                        VBI_SetTVEWSS525Nfy(TVE_SetWSS525Nfy);
                        VBI_SetTVEWSS625Nfy(TVE_SetWSS625Nfy);
                        VBI_SetTVECCNfy(TVE_SetCCNfy);
                        //enable TVE CC/WSS in VBI CB function
                    	}
                     else
                     {
                         TVE_SetWssEnable(TVE_1, SV_ON);
                     }
                }
            }
        }
    }
#endif
}


static void _vTveUnmuteFunc(UINT8 bPath)
{
    UINT8 bVideoDecType;
    
    bVideoDecType = bGetVideoDecType(bPath);

    /* if the pipe is not connected, ignore the unmute signal */
    if(bVideoDecType != SV_VD_NA && bVideoDecType != SV_VD_MAX)
    {
        if( apu1SourceType[DAC_1] != NULL &&
            bVideoDecType == bGetDecodeType(*apu1SourceType[DAC_1]) )
        {
            vApiTVESetDacMuteMask(DAC_1, TVE_DAC_MUTE_MASK_DRV, FALSE);
        }

        if( apu1SourceType[DAC_2] != NULL &&
            bVideoDecType == bGetDecodeType(*apu1SourceType[DAC_2]) )
        {
            vApiTVESetDacMuteMask(DAC_2, TVE_DAC_MUTE_MASK_DRV, FALSE);
        }
    }
}

/**
 * @brief vApiTVESetNotifyFunc(pSigChgNfy, pUnmuteNfy) Set the notify function of tve path
 * @param pSigChgNfy: Signal change notify function pointer
 * @param pUnmuteNfy: unmute event notify function pointer
 * @retval void
 */
void vApiTVESetNotifyFunc(void (*pSigChgNfy)(UINT8,UINT8), void (*pUnmuteNfy)(UINT8))
{
    _TveNotifySigChg = pSigChgNfy;
    _TveNotifyUnmute = pUnmuteNfy;
    return;
}

/**
 * @brief vApiTVEVideoStatusNotify(UINT8 bPath, UINT8 bMode)
 * @param bPath: Input Source of Main
 * @param  bMode
 * @retval void
 */
void vApiTVEVideoStatusNotify(UINT8 bPath, UINT8 bMode)
{
    switch(bMode)
    {
        case SV_TVE_NOTIFY_VIDEO_NO_SIGNAL:
        case SV_TVE_NOTIFY_VIDEO_MODE_CHANGE:        
            if(bMode == SV_TVE_NOTIFY_VIDEO_MODE_CHANGE &&
               (bGetVideoDecType(bPath) == SV_VD_MPEGHD||
                bGetVideoDecType(bPath) == SV_VD_MPEGSD))
            {
                // for DTV, we ignore the mode change event
                break; 
            }
            _vTveSigChgFunc(bPath, SV_VDO_NOSIGNAL);
            if(_TveNotifySigChg != NULL)
            {
                _TveNotifySigChg(bPath, SV_VDO_NOSIGNAL);
            }
        break;
        case SV_TVE_NOTIFY_VIDEO_STABLE_SIGNAL:
            _vTveSigChgFunc(bPath, SV_VDO_STABLE);
            if(_TveNotifySigChg != NULL)
            {
                _TveNotifySigChg(bPath, SV_VDO_STABLE);
            }           
        break;    
        case SV_TVE_NOTIFY_VIDEO_UNMUTE:
            _vTveUnmuteFunc(bPath);
            if(_TveNotifyUnmute != NULL)
            {
                _TveNotifyUnmute(bPath);
            }
        break;
        case SV_TVE_NOTIFY_MAX:      
        default:
        break;
    }
}

void vApiTVEVideoMuteChangeNotify(UINT8 bPath, UINT8 bMute)
{
    switch(bMute)
    {
        case SV_ON:
            _vTveSigChgFunc(bPath, SV_VDO_NOSIGNAL);
            if(_TveNotifySigChg != NULL)
            {
                _TveNotifySigChg(bPath, SV_VDO_NOSIGNAL);
            }
        break;
        case SV_OFF:
            _vTveUnmuteFunc(bPath);
            if(_TveNotifyUnmute != NULL)
            {
                _TveNotifyUnmute(bPath);
            }
        break;
        default:
        break;
    }
}

UINT8 bApiTVEGetBypPath(UINT8 u1DacId)
{
    /* through DAC or not */
    return (abBypPath[u1DacId] & (~TVE_BYP_EXT_MUX_PATH));
}

/**
 * @brief vApiTVESetDacMuteMask(UINT8 u1Mask, UINT8 u1OnOff)
 * @param u1Mask: the mute mask 
 * @param u1OnOff: set the mute mask on or off 
 * @retval void
 */
void vApiTVESetDacMuteMask(UINT8 u1DacId, UINT8 u1Mask, UINT8 u1OnOff)
{
    if(u1DacId > 1) return;

    if(u1OnOff != FALSE)
    {
        au1DacMuteMask[u1DacId] |= u1Mask;
    }
    else
    {
        au1DacMuteMask[u1DacId] &= ~u1Mask;
    }

    if(au1DacMuteMask[u1DacId]!=0)
    {
        /* mute output */
        vApiTVEOutPathSelect(u1DacId, SV_TVE_DAC_BYPASS_CVBS);
        TVE_HalDACPowerOn(TVE_1, u1DacId, FALSE);
    }
    else
    {
        /* unmute output */
        if((abBypPath[u1DacId] & TVE_BYP_EXT_MUX_PATH) != 0)
        {
            vApiTVEOutPathSelect(u1DacId, SV_TVE_HW_BYPASS);
        }
	else
	{
            TVE_HalDACPowerOn(TVE_1, u1DacId, TRUE);
        } 
    } 
    return;
}

/**
 * @brief vApiTVESetScartOutCtrl(UINT8 u1Dac_id, UINT8 u1SrcType, void *pv_extra_arg)
 * @param tTveConfig tve configuration setting from table
 * @retval void
 */
void vApiTVESetScartOutCtrl(UINT8 u1Dac_id, UINT8 *u1SrcType, void* pv_extra_arg)
{
    UINT8 au1TveOutType[2];
#ifndef CC_MT5301A
    VDP_REGION_T rRegion;
#endif
    TveUsage_t *prTveUsed = (TveUsage_t*)pv_extra_arg;
    
    if(u1Dac_id > 1 || u1SrcType == NULL || prTveUsed == NULL) return;

    apu1SourceType[u1Dac_id] = u1SrcType;

    vApiTVEGetConfig(*u1SrcType, au1TveOutType);

    /* TVE Out config */
    switch (au1TveOutType[u1Dac_id])
    {
        case SV_TVE_DAC_BYPASS_CVBS:
	    Bypass_HalSrcBypass(u1Dac_id,TVE_VDOIN_MIXER);
            abBypPath[u1Dac_id] = TVE_BYP_AVMUX_PATH;
	break;
	case SV_TVE_DAC_BYPASS_DEMOD:
            Bypass_HalSrcBypass(u1Dac_id,TVE_DEMOD_MIXER);
            abBypPath[u1Dac_id] = TVE_BYP_EXT_PATH;
	break;
	#ifdef CC_MT5363
	case SV_TVE_DAC_BYPASS_SIF:
            vDrvSetSIFADC();
            Bypass_HalSrcBypass(u1Dac_id,TVE_SIF_MIXER);
            abBypPath[u1Dac_id] = TVE_BYP_EXT_PATH;
	break;
	#endif
#ifndef CC_MT5301A
	case SV_TVE_DAC_PIP:
            /* Tve might be used by the other scartout path */
            if(prTveUsed->TveIsUsedBy == 0xf) /* tve_pip is free */
            {
                SRM_SetTvMode(SRM_TV_MODE_TYPE_SCART_OUT_ON);
                Bypass_HalPIPClk(0);    
                _vApiTveSetSclPara(TRUE, 720, 576);
                prTveUsed->TveIsUsedBy = u1Dac_id;
                if( VSS_MAJOR(*u1SrcType) == VSS_DTV || VSS_MAJOR(*u1SrcType) == VSS_DTD)
                {
                    VDP_SetInput(VDP_2, 0, 0); 
                }
                bApiVideoSetVideoSrc(SV_VP_PIP, *u1SrcType); 

                rRegion.u4X = 0; 
                rRegion.u4Y = 0;
                rRegion.u4Width = VDP_MAX_REGION_WIDTH;
                rRegion.u4Height = VDP_MAX_REGION_HEIGHT;
                VDP_SetSrcRegion(VDP_2,0, rRegion);
	
            }
            else if(apu1SourceType[1-u1Dac_id] != NULL &&
                    *apu1SourceType[u1Dac_id] == *apu1SourceType[1-u1Dac_id])
            {
                /* tve is already being controlled by TVSCART && two scart is the same source */
                TVE_HalSetMixerDacSel(u1Dac_id, 0);
            }
            else
            {
                *apu1SourceType[u1Dac_id] = SV_VS_MAX;
            }
            abBypPath[u1Dac_id] = TVE_BYP_PIP_PATH;
            Bypass_HalSrcPIP(u1Dac_id, bGetDecodeType(*u1SrcType));
        break;
        #endif
        case SV_TVE_DAC_VIDEO_IN:
        #if 0
            if(Bypass_HalTVEMode(u1DecType)) /* SD */
            {
                Bypass_HalSrcVdoin(DAC_1,u1DecType);
            }
            else /* HD */
            {
	        bApiVideoMainSubSrc(bSrcOld, bSrc);
                Bypass_HalSrcPIP(DAC_1,u1DecType);
            }
        #endif
	break;
	case SV_TVE_HW_BYPASS:
            abBypPath[u1Dac_id] = TVE_BYP_EXT_PATH | TVE_BYP_EXT_MUX_PATH;
	break;
	case SV_TVE_D2S_BYPASS:	         	    
        case SV_TVE_NA:
        case SV_TVE_MAX:	         	
            abBypPath[u1Dac_id] = 0;
            *apu1SourceType[u1Dac_id] = SV_VS_MAX;
        default:
      	break;
    }       
    vApiTVEOutPathSelect((u1Dac_id == 0)?(SV_TVE_SCART_OUT_1):(SV_TVE_SCART_OUT_2), au1TveOutType[u1Dac_id]);

    if(bApiTVEGetBypPath(u1Dac_id) == TVE_BYP_EXT_PATH)
    {
        /* if hardware bypass, release mutemask here */
        vApiTVESetDacMuteMask(u1Dac_id, TVE_DAC_MUTE_MASK_DRV, FALSE);
    }

    /* if the decoder type is the same as main && main is unmuted 
       to cover the situtation that 
       main is unmute first, then connect the monitor scart */
    if(*u1SrcType != SV_VS_MAX &&
       bGetDecodeType(*u1SrcType) == bGetVideoDecType(SV_VP_MAIN) &&
       0 == _u4DrvVideoGetMute(SV_VP_MAIN))
    {
        vApiTVESetDacMuteMask(u1Dac_id, TVE_DAC_MUTE_MASK_DRV, FALSE);
    }
}



/**
 * @brief vApiTVESetOutputFormat(UINT32 u4fmt)
 * @param u4fmt the output format(ntsc/pal/none)
 * @retval void
 */
void vApiTVESetOutputFormat(UINT32 u4fmt)
{
#ifdef CC_MT5301A
return;
#else
    switch(u4fmt)
    {
        case SV_TVE_OFMT_PAL:
            TVE_SetFmt(TVE_1, TVE_FMT_576I);
            _vApiTveSetSclPara(TRUE, 720, 576);
        break;
        case SV_TVE_OFMT_NTSC:
            TVE_SetFmt(TVE_1, TVE_FMT_480I);
            _vApiTveSetSclPara(TRUE, 720, 480);
        break;
        case SV_TVE_OFMT_NONE:
        default:
            _vApiTveSetSclPara(FALSE, 0, 0);
            Bypass_HalPIPClk(0);
            //SRM_SetTvMode(SRM_TV_MODE_TYPE_SCART_OUT_OFF);
        break;
    }
#endif
}

void vApiTVEQueryStatus()
{
    UINT8 tmpCnt;
    UINT8* astrMuteMask[] = {"-", "mw", "drv", "mw+drv", "unknown mute"};
    UINT8* astrSrcMajor[] = {"atv", "atd", "cvbs", "sv", "ypbpr", "vgacomp", "vga", "hdmi", "dvi", "dtv", "dtd", "scart", "reserved", "none"};
    UINT8* astrBypPath[] = {"n/a", "int_mux", "av_mux", "pip", "ext_mux", "unknown"};
    UINT8 *strMute, *strSrc, *strByp;

    LOG(2, "\n");
    LOG(2, "tvd mv(%d), hdtv mv(%d), hdmi hdcp(%d)\n", _rTvd3dStatus.fgIsMV,_bHdtvMvOn, bHDMIHDCPStatusGet()); 

    for(tmpCnt=0;tmpCnt<2;tmpCnt++)
    {
        if(au1DacMuteMask[tmpCnt] <= 3)
        {
            strMute = astrMuteMask[au1DacMuteMask[tmpCnt]];
        }
        else
        {
            strMute = astrMuteMask[4];
        }

        if(apu1SourceType[tmpCnt] != NULL && VSS_MAJOR(*apu1SourceType[tmpCnt]) <= VSS_RESERVE)
        {
            strSrc = astrSrcMajor[VSS_MAJOR(*apu1SourceType[tmpCnt])];
        }
        else
        {
            strSrc = astrSrcMajor[sizeof(astrSrcMajor)/sizeof(UINT8*)-1];
        }

        if(abBypPath[tmpCnt] <= TVE_BYP_PIP_PATH)
        {
            strByp = astrBypPath[abBypPath[tmpCnt]];
        }
        else
        {
            if(abBypPath[tmpCnt] & TVE_BYP_EXT_MUX_PATH)
            {
                strByp = astrBypPath[4];
            }
            else
            {
                strByp = astrBypPath[5];
            }
        }

        LOG(2, "Dac(%d) MuteMask [%x] -> %-6s, source -> %s%d\n", tmpCnt, au1DacMuteMask[tmpCnt], strMute, strSrc, VSS_MINOR((apu1SourceType[tmpCnt]!=NULL)?(*apu1SourceType[tmpCnt]):(SV_VS_MAX))+1);
        LOG(2, "        BypPath [%x] -> %s\n", abBypPath[tmpCnt], strByp);
    }

}

#endif /* _TVE_IF_C_ */

