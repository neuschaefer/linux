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
 * $RCSfile: drv_nr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR driver
 *
 * @author mf_tien@mtk.com.tw
 * @function entry point
 *  vDrvNRInit
 *  vDrvNRSetMode
 * @user case
 *  power on -> vDrvNRInit
 *  mode change done -> vDrvNRSetMode
 *  user menu -> vDrvNRSetMode
 */

/**************************************************************************
 * Inclusions
 *************************************************************************/
#define _DRV_NR_C_

#include "general.h"
#include "hw_vdoin.h"
#include "hw_nr.h"
#include "hw_di.h"
#include "hw_scpos.h"
#include "drv_nr.h"
#include "drv_tvd.h"
#include "api_eep.h"
#include "source_select.h"
#include "source_table.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "eepdef.h"
//#include "nptv_debug.h"
#include "nr_debug.h"
#include "drv_ycproc.h"
#include "hw_ycproc.h"
#include "fbm_drvif.h"
#include "hw_tdc.h"
#include "srm_drvif.h"
#include "drv_mpeg.h"
#include "drv_scaler.h"
#include "drv_di.h"
#include "x_hal_5381.h"
#include "pe_if.h"
#include "drv_contrast.h"
#include "drv_tdtv_drvif.h"
#include "drv_noisemeter.h"

//NR global parameter
NR_PRM_T _NRParam;

//Variable for setting chroma line avg. for TVD
UINT8 _TVD_path;
NR_CHROMA_LINE_AVERAGE_MODE _TVD_Chroma_mode;

static const NR_DATA_MODE_MAPPING_T NRDatamodeMapping[] = 
{
    { E_DI_NR_OFF,     "NR OFF" },
    { E_DI_NR_Y_ONLY,  "NR Y ONLY" },
    { E_DI_NR_YC_420,  "NR YC 420" },
    { E_DI_NR_YC_422,  "NR YC 422" },
    { E_DI_NR_RGB,     "NR RGB" },
    { E_DI_NR_UNKNOWN, "Unknown" },	
};

#ifdef CC_DOWMBWMODE
EXTERN UINT8 u1DowmBWMode;
#endif

////////////////////////////////////////////////////////////////////////////////
// static function for internal use
////////////////////////////////////////////////////////////////////////////////
static void vDrvNRSetCurrentPath(UINT8 u1VdpId);
//functions for setting NR-related window size
static void vDrvNRSetWindows(UINT16 wWidth, UINT16 wHeight);
static void vDrvNRSetLocalBlockDetectorWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSetBlockMeterWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSet3DActiveWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSet3DMetricWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSetFrameBaseStillWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSetNoiseMeterWindow(UINT16 wWidth,UINT16 wHeight);
static UINT8 bDrvIsSDInterlaceTiming(UINT8 bPath);

//functions for setting Chroma line avg mode for TVD
static void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode);

static void vDrvNRCheckInputSize(void);
static void vDrvNRCheckLineBufferSetting(void);

//3D NR YC on/ off rule
static void vDrvNRDetermine3DNRYCOnoff(void);
//vga mode
static void vDrvNRCheckVGAModeOnOff(void);
//check if v bound protect should be turned on/off
static void vDrvNRCheckVBoundProtect(void);

#if NR_DISABLE_PIXEL_REP_WHEN_BLANKING_IS_SAMLL
static void vDrvNRCheckBourndaryRepeat(UINT8 bPath);
#endif

#if ANM_ENABLE
UINT8 vDrvRFNMLevel(void);
#endif
extern UINT8 bTvdSignalStrengthLevel(UINT8 bMode);

//datamode related
#ifdef CC_SCPOS_3DTV_SUPPORT    
static void vDrvNRConfigTDTVDatamode(void);
#endif
static void vDrvNRConfigSRMDatamode(void);
static void vDrvNRSetDatamodeChanged(UINT8 modeChange);
static UINT8 bDrvNRIsDatamodeChanged(void);


////////////////////////////////////////////////////////////////////////////////
//NR Process
////////////////////////////////////////////////////////////////////////////////

#if SUPPORT_SEAMLESS_MODE
/**
 * @brief Set split line for demo mode
 * @param bOnOff SV_ON/SV_OFF
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param wXCenter Split line position
 * @warning bPath does not have any effect for now
 */
void vDrv2DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
    if (bOnOff)
	{
        // 2DNR
        //enable slice mode
        vRegWriteFldAlign(NR_2DNR1F, 0x1, R_SLICE);	
        //set source on left or right
        if(bRegion == SV_LEFT)
        {
            vRegWriteFldAlign(NR_2DNR1F, 0x0, R_LEFT_SRC);	            
        }
        else
        {
            vRegWriteFldAlign(NR_2DNR1F, 0x1, R_LEFT_SRC);	            
        }
        //set slice line x position
        vRegWriteFldAlign(NR_2DNR02, wXCenter, R_UISLICEXPOS);	                    
	}
	else
	{
        // 2DNR
        //disable slice mode
        vRegWriteFldAlign(NR_2DNR1F, 0x0, R_SLICE);	
        //set source on left or right
        vRegWriteFldAlign(NR_2DNR1F, 0x1, R_LEFT_SRC);	            
        //set slice line x position
        vRegWriteFldAlign(NR_2DNR02, 0x7FF, R_UISLICEXPOS);	                    
	}
}

void vDrv3DNRDemo(UINT8 bOnOff, Region bRegion)
{
    if(bOnOff==SV_ON)
    {
        UINT8 bPscanScale = (_arMDDiPrm[SV_VP_MAIN].u1Interlace)?2:1;
        vRegWriteFldAlign(NR_3DNR3E, bRegion.wHStart, HSTART_3DNR);
        vRegWriteFldAlign(NR_3DNR3E, bRegion.wHEnd-1, HEND_3DNR);// hstart <= active <=hend    
        //Before PSCAN need factor 0.5
        vRegWriteFldAlign(NR_3DNR3F, bRegion.wVStart/bPscanScale+1, VSTART_3DNR); // 3D NR Demo mode VOffset
        vRegWriteFldAlign(NR_3DNR3F, bRegion.wVEnd/bPscanScale+1, VEND_3DNR);    // 3D NR Demo mode VOffset    
    }
    else
    {
        vDrvNRSet3DActiveWindow(_NRParam.u2NRWidth, _NRParam.u2NRHeight);
	}
}

#endif //SUPPORT_SEAMLESS_MODE

/**
 * @brief Function to load a register table.
 * @param prRegTbl table to load
 */
void vDrvNRLoadRegTbl(const NR_REGTBL_T* prRegTbl)
{
	while (prRegTbl->u2Addr != NR_REGTBL_END)
	{
		vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
		prRegTbl++;
	}
}
 
/**
* @brief Set 3D NR IIR Path
* @param u1VdpId VDP_1/ VDP_2            
*/
void vDrvNRSetIIRPath(UINT8 u1VdpId)
{    
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }
    vRegWriteFldAlign(NR_3DNR00, (u1VdpId == VDP_1) ? 0 : 1, NR_MAIN_PIP_SEL);
}

/**
* @brief Set NR flip/mirror mode
*/ 
void vDrvNRSetFlipMirror(UINT8 u1VdpId,UINT8 u1Mode)
{
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }
    vRegWriteFldAlign(NR_3DNR00, u1Mode, NR_DISPLAY_MODE);
}

/**
* @brief Set NR read enable/disable
* @param bOnOff SV_TRUE Enable
*               SV_FALSE Disable             
*/ 
void vDrvNRSetReadEnable(UINT8 u1VdpId,UINT8 bOnOff)
{
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }
    vRegWriteFldAlign(NR_3DNR00, (bOnOff == SV_FALSE) ? 0 : 1, NR_READ_ENABLE);
}

/**
* @brief Set NR scan mode
* @param scanMode NR_SCAN_MODE_INTERLACE/ NR_SCAN_MODE_PROGRASSIVE
*/
void vDrvNRSetScanMode(UINT8 u1VdpId,NR_SCAN_MODE scanMode)
{
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }
    vRegWriteFldAlign(NR_3DNR00, (scanMode == NR_SCAN_MODE_INTERLACE) ? 0 : 1, NR_PROGRESSIVE);
}

/**
* @brief Set NR YUV mode
* @param nrYuvMode Y_ONLY/ 420/ 422/ 444
*/
void vDrvNRSetYUVMode(UINT8 u1VdpId,NR_YUV_MODE nrYuvMode)
{
    UINT8 bNrYuvMode = 0;
    UINT8 bSrc420 = 0;
    UINT8 bSwInit = 0;
    
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }    
    
    if(nrYuvMode == NR_YUV_MODE_Y_ONLY)
    {
        bNrYuvMode = 0;
        bSrc420 = 0;
        bSwInit = 0;
    }
    else if(nrYuvMode == NR_YUV_MODE_420)
    {
        bNrYuvMode = 1;
        bSrc420 = 1;
        bSwInit = 1;
    }
    else if(nrYuvMode == NR_YUV_MODE_422)
    {
        bNrYuvMode = 2;    
        bSrc420 = 0;
        bSwInit = 0;
    }
    else if(nrYuvMode == NR_YUV_MODE_444)
    {
        bNrYuvMode = 3;    
        bSrc420 = 0;
        bSwInit = 0;
    }

    //set Mode related register
    vRegWriteFldAlign(NR_3DNR00, bNrYuvMode, NRYUV_MODE);
    vRegWriteFldAlign(NR_3DNR00, bSrc420, C_SRC_420);    
    vRegWriteFldAlign(NR_3DNR00, bSwInit, C_SW_INIT);    
}

/**
* @brief Set NR Bit depth
* @param nrBitSel 8BIT/ 10BIT
*/
void vDrvNRSetBitSel(UINT8 u1VdpId,NR_BIT_SEL nrBitSel)
{
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }

    UINT32 dwVal = (nrBitSel == NR_BIT_SEL_8BIT) ? 1 : 0;
    
    vRegWriteFldMulti(NR_3DNR00,
                  P_Fld(dwVal, NR_BIT_SEL_RV) | 
                  P_Fld(dwVal, NR_BIT_SEL_RU) | 
                  P_Fld(dwVal, NR_BIT_SEL_RY));
}

/**
* @brief Set chroma line average mode. This function only keep the mode and path
*        in variable. The real hw register setting will be wrote into register until
*        vDrvNRSetChromaLineAvgModeReg() was invoked.
* @param bPath VDP_1/ VDP_2
* @param bMode 	NR_CHROMA_LINE_AVERAGE_MODE_OFF/ NR_CHROMA_LINE_AVERAGE_MODE_UP/
*           	NR_CHROMA_LINE_AVERAGE_MODE_DOWN/ NR_CHROMA_LINE_AVERAGE_MODE_121
*/
void vDrvNRSetChromaLineAvgMode(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode)
{
	_TVD_path = bPath;
	_TVD_Chroma_mode = bMode;
}

/**
* @brief Set chroma line average mode register.
* @param bPath VDP_1/ VDP_2
* @param bMode 	NR_CHROMA_LINE_AVERAGE_MODE_OFF/ NR_CHROMA_LINE_AVERAGE_MODE_UP/
*           	NR_CHROMA_LINE_AVERAGE_MODE_DOWN/ NR_CHROMA_LINE_AVERAGE_MODE_121
*/
static void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode)
{
    UINT8 bTvdPath = SV_VP_NA;
    UNUSED(bPath);
    
    //Only turn on chroma line average under TVD source
    if(fgIsMainTvd3d())
    {
        bTvdPath = SV_VP_MAIN;
    }
    if(fgIsPipTvd3d())
    {
        bTvdPath = SV_VP_PIP;
    }
    if(bDrvNRGetCurrentPath()!=bTvdPath)
    {
        vRegWriteFldAlign(NR_2DNR01, 0x0, CLAVG_EN);
        return;
    }
    
    //5365 use only 121 chroma line average.  
    if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_121)
    {
        //121   	        
        vRegWriteFldMulti(NR_2DNR01, P_Fld(SV_ON, CLAVG_EN) | 
                                                     P_Fld(SV_ON, CLAVG_FORCE_121));        
    }
    else if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_OFF)
    {
        vRegWriteFldAlign(NR_2DNR01, 0x0, CLAVG_EN);
    }
    else
    {
        printf("\n Wrong Chroma Lina Average Mode !! \n");
    }
}

/**
 * @brief Check if input width/ height is changed.
 *        If changed: 1. Check line buffer setting.
 *                    2. Check NR window setting.
 */
static void vDrvNRCheckInputSize(void)
{
	UINT8 bPath;
	UINT16 u2Width;
	UINT16 u2Height;
    UINT16 u2XOffset;
    UINT16 u2YOffset;
	
    PSCAN_CLIP_INFO_T ScalerOverscanInfo;	

    //1. get current NR path
    bPath = bDrvNRGetCurrentPath();
    //2. get input height (field domain)
    if (u1ScpipGetClipInfo(bPath, &ScalerOverscanInfo))
    {
    	u2Width = ScalerOverscanInfo.wWidth;
    	u2Height = ScalerOverscanInfo.wHeight;
    	u2XOffset = ScalerOverscanInfo.wX;
    	u2YOffset = ScalerOverscanInfo.wY;
    }
    else
    {
    	u2Width = _NRParam.u2NRWidth;
    	u2Height = _NRParam.u2NRHeight;
    	u2XOffset = _NRParam.u2XOffset;
    	u2YOffset = _NRParam.u2YOffset;    	
    }

    if(bDrvVideoIsSrcInterlace(bPath))
    {
        u2Height = (u2Height+1)/2;
    }
    
    //4. if input size changed.
    if((_NRParam.u2NRWidth != u2Width) || 
       (_NRParam.u2NRHeight != u2Height) ||
       (_NRParam.u2XOffset != u2XOffset) ||
       (_NRParam.u2YOffset != u2YOffset))
    {
        LOG(3,"[NR] Input changed Width(%d->%d) Height(%d->%d)\n",_NRParam.u2NRWidth,u2Width,_NRParam.u2NRHeight,u2Height);
        //4.1 update input width/ height
        _NRParam.u2NRWidth = u2Width;
        _NRParam.u2NRHeight = u2Height;
        _NRParam.u2XOffset = u2XOffset;
        _NRParam.u2YOffset = u2YOffset;
	    
        //4.2 check line buffer setting
        vDrvNRCheckLineBufferSetting();
        //4.3 set input windows (2D/3D/Meters)
        vDrvNRSetWindows(_NRParam.u2NRWidth, _NRParam.u2NRHeight);

        #if NR_DISABLE_PIXEL_REP_WHEN_BLANKING_IS_SAMLL
        vDrvNRCheckBourndaryRepeat(bPath);	
    	#endif

        vDrvNRCheckVBoundProtect();
    }
}

/**
 * @brief check line buffer length setting
 */
static void vDrvNRCheckLineBufferSetting(void)
{
    //if widht is between 0 and 1920
    if(_NRParam.u2NRWidth<=1920)
    {
        vRegWriteFldAlign(NR_3DNR0D, 0x1, LNBUFMODE);
        vRegWriteFldAlign(NR_3DNR0D, 0x0, LNBUF_MANUAL_LENGTH);
    }
    else //if widht > 1920, use manual length
    {
        vRegWriteFldAlign(NR_3DNR0D, 0x2, LNBUFMODE);
        vRegWriteFldAlign(NR_3DNR0D, (_NRParam.u2NRWidth / 2) - 1, LNBUF_MANUAL_LENGTH);    
    }
}

/**
 *  @brief Set all NR related window
 *         1. Local block detector window
 *         2. Block meter window
 *         3. 3D NR active window
 *         4. 3D NR metric window
 *         5. Frame base still detection window
 *  @param wWidth input widht
 *  @param wHeight input height
 */
static void vDrvNRSetWindows(UINT16 wWidth, UINT16 wHeight)
{
    vDrvNRSetLocalBlockDetectorWindow(wWidth,wHeight);
    vDrvNRSetBlockMeterWindow(wWidth,wHeight);
    vDrvNRSet3DActiveWindow(wWidth,wHeight);
    vDrvNRSet3DMetricWindow(wWidth,wHeight);
    vDrvNRSetFrameBaseStillWindow(wWidth,wHeight);
    vDrvNRSetNoiseMeterWindow(wWidth,wHeight);
#ifdef ENABLE_SNRSM
	vDrvSNRSMSetWindow(wWidth, wHeight);
#endif // ENABLE_SNRSM
}

/**
 *  @brief Set local block detector window. 
 *         This window will only take effect when register "r_block_pdp" is set to enable.
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSetLocalBlockDetectorWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_2DNR01, wWidth, R_IMGWIDTH);
    vRegWriteFldAlign(NR_2DNR01, wHeight, R_IMGHEIGHT);    
}

/**
 *  @brief Set block meter window.
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSetBlockMeterWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_2DNR5A, 0x04, R_HPOS_START);
    vRegWriteFldAlign(NR_2DNR5A, 0x04, R_HPOS_END);
    vRegWriteFldAlign(NR_2DNR5A, 0x04, R_VPOS_START);
    vRegWriteFldAlign(NR_2DNR5A, 0x04, R_VPOS_END);
  
    vRegWriteFldAlign(NR_2DNR5B, (wWidth-1)>>1, R_VIDEO_WIDTH);
    vRegWriteFldAlign(NR_2DNR5B, wHeight, R_VIDEO_HEIGHT);    
}

/**
 *  @brief Set 3D NR Active window
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSet3DActiveWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_3DNR3E, 0x0, HSTART_3DNR);
//    vRegWriteFldAlign(NR_3DNR3E, 0x7ff, HEND_3DNR);    
    vRegWriteFldAlign(NR_3DNR3E, wWidth-1, HEND_3DNR);
    vRegWriteFldAlign(NR_3DNR3F, 0x0, VSTART_3DNR);
//    vRegWriteFldAlign(NR_3DNR3F, 0x7ff, VEND_3DNR);        
    vRegWriteFldAlign(NR_3DNR3F, wHeight-1, VEND_3DNR);    
}

/**
 *  @brief Set 3D NR Metric window
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSet3DMetricWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_3DNR33, 0x0, HSTART_METRIC);
    vRegWriteFldAlign(NR_3DNR33, wWidth-1, HEND_METRIC);
    vRegWriteFldAlign(NR_3DNR34, 0x1, VSTART_METRIC);
    vRegWriteFldAlign(NR_3DNR34, wHeight-1, VEND_METRIC);    
}

/**
 *  @brief Set 3D Frame base still detector window
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSetFrameBaseStillWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_3DNR48, 0x0, HSTART_FRST);
    vRegWriteFldAlign(NR_3DNR48, wWidth-1, HEND_FRST);
    vRegWriteFldAlign(NR_3DNR49, 0x1, VSTART_FRST);
    vRegWriteFldAlign(NR_3DNR49, wHeight-1, VEND_FRST);    
}

/**
 *  @brief Set noise meter window
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSetNoiseMeterWindow(UINT16 wWidth,UINT16 wHeight)
{
    
    vRegWriteFldAlign(NR_3DNR82, 0x0, HPIX_ST);   
	vRegWriteFldAlign(NR_3DNR82, (wWidth>>2)<<2, HPIX_ED);

	//to avoid garbage
    if(bDrvIsSDInterlaceTiming(bDrvNRGetCurrentPath()) == SV_TRUE && bGetVideoDecType(bDrvNRGetCurrentPath()) == SV_VD_YPBPR)
    {
        vRegWriteFldAlign(NR_3DNR80, 0x4, C_VLINE_ST);
    }
    else
    {	
	    vRegWriteFldAlign(NR_3DNR80, 0x0, C_VLINE_ST);
    }
    
	vRegWriteFldAlign(NR_3DNR80, (wHeight>>2)<<2, C_VLINE_ED);	

	vDrvNMSetBlockSize(wWidth, wHeight);
}

static UINT8 bDrvIsSDInterlaceTiming(UINT8 bPath)
{
    UINT8 timing = MODE_NOSIGNAL;
    UINT8 bRet = SV_FALSE;
    timing = bDrvVideoGetTiming(bPath);
    if(timing == MODE_525I || timing == MODE_625I || timing == MODE_525I_OVERSAMPLE || timing == MODE_625I_OVERSAMPLE)
    {
        bRet = SV_TRUE;
    }

    return bRet;
}

/**
 *  @brief Set 3D NR manual weight
 */
void DrvNRSetManualWeight(UINT8 bOnOff,UINT8 bWeight)
{
    vRegWriteFldAlign(NR_3DNR02, bOnOff, MANWCU);
    vRegWriteFldAlign(NR_3DNR3C, bWeight,MANBWCU);
    LOG(3,"Set manual mode (%d) weight(%d)\n",bOnOff,bWeight);
}

/**
 * @brief Initialize NR
 */
void vDrvNRInit(void)
{
    //1. Load init table    
    vDrvNRLoadRegTbl(NR_SYS_INIT);
    //2. Load init quality table
    vDrvNRQtyInit();
    //3. turn off 3D NR
    vDrvNRSet3DNROnOff(SV_OFF);

    //4. init NR window size
    _NRParam.u2NRWidth = 1;
    _NRParam.u2NRHeight = 1;

    vRegWriteFldAlign(NR_3DNR03, 0x0, BPYC2D);	            // disable 2D bypass
    vRegWriteFldAlign(PAT_MAIN_02, 0x0, C_MAIN_SELF_FIELD_VCHG);	//set filed 1 delay to 0 line
    vRegWriteFldAlign(NR_3DNR00, 0x1, C_V_BOUND_PROTECT);   //Vertical boundary protect for NR linebuf

    vRegWriteFldAlign(NR_2DNR03, 0x1 ,R_BBOUNDREPEAT10PXL);
    vRegWriteFldAlign(NR_2DNR03, 0x1 ,R_BBOUNDARYREPEATEN);

    vRegWriteFldAlign(NR_3DNR00, 0x0 ,NR_DISPLAY_MODE);

#ifdef ENABLE_SNRSM
    // init special meter
    vDrvSNRSMInit();
#endif //ENABLE_SNRSM

    //for cli debug
    _NRParam.bNRDebugPath = SV_VP_NA;    

    // init INK setting
    vRegWriteFldAlign(NR_3DNR00,1,BYPASS_INK_SEL);
    vRegWriteFldAlign(NR_3DNR00,1,NR_INK_SEL);

    //for noise meter
    vRegWriteFldAlign(NR_3DNR81, 0x1, C_DIFF_DET_EN);
    vRegWriteFldAlign(NR_3DNR81, 0x1, C_DIFF_DET_SEL);    
    vRegWriteFldAlign(NR_3DNR80, 0x1, C_SRC_SEL);
   	//NM init
    vDrvNMInit(SV_TRUE,0,6);
   	
    fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_NR, (vTDTVModeChangeCB)vDrvNROnTDTVCallback);

    #ifdef PSCAN_AUTO_VERIFY
    //disable rounding
    vRegWriteFldAlign(NR_3DNR60, 0, C_NEW_ROUND_EN);
    #endif
}

/**
 * @brief Decide 1. NR path should be on Main or PIP.
 *               2. NR bypass should be Turned on/ off.
 */
UINT8 bDrvNRDeterminePath(void)
{
    //for cli debug
    if(_NRParam.bNRDebugPath != SV_VP_NA)
    {
        _NRParam.bNRPath = ((_NRParam.bNRDebugPath==SV_VP_PIP)?SV_VP_PIP:SV_VP_MAIN);
        _NRParam.bNRBypass = SV_OFF;
        return _NRParam.bNRPath;
    }
    
    /*1. decide NR path. 
     * Rule: 
     *     1. In most of cases, NR would be on main path
     *     2. If main is 444 input and sub is ATV/ DTV, NR would be on sub
     */
    /*     
    if(bIsScalerInput444(SV_VP_MAIN) && 
       (bGetVideoDecType(SV_VP_PIP) == SV_VD_TVD3D) || (bGetVideoDecType(SV_VP_PIP) == SV_VD_MPEGHD))
    {
        _NRParam.bNRPath = SV_VP_PIP;
    }
    else
    {
        _NRParam.bNRPath = SV_VP_MAIN;
    }
    */

    //Currently, NR will always act on main path.
    //Todo: If we want to switch to sub path, need to notify srm that nr will 
    //      on sub and srm should allocate dram to sub pscan to avoid dram overlap.
    _NRParam.bNRPath = SV_VP_MAIN;
    
	/*2. Decide if NR bypass on/ off
	 * Rule:
     *	   NR will be bypassed when
	 *     1. If Current NR path is pip and is 444 input. 
	 *   or
	 *     2. If Current NR path have no source selected.
	 *   or
	 *     3. If Current NR path have no signal
	 */

	if((_NRParam.bNRPath == SV_VP_PIP && bIsScalerInput444(_NRParam.bNRPath)) || 
	   bGetVideoDecType(_NRParam.bNRPath) == SV_VD_NA ||
	   bDrvVideoSignalStatus(_NRParam.bNRPath) != SV_VDO_STABLE)
	{
	    _NRParam.bNRBypass = SV_ON;
	}
	else
	{
	    _NRParam.bNRBypass = SV_OFF;
	}
	return _NRParam.bNRPath;
}

/**
* @brief Get NR current depth
* @return NR path
*/
UINT8 bDrvNRGetCurrentPath(void)
{
    return _NRParam.bNRPath;
}

/**
* @brief Set NR current path
* @param u1VdpId VDP_1/ VDP_2
*/
void vDrvNRSetCurrentPath(UINT8 u1VdpId)
{
    vRegWriteFldAlign(NR_TOP_MAIN_00, (u1VdpId==VDP_1)?0:1, C_NR_SRC_SEL);
}

void vDrvNRSetDebugPath(UINT8 u1VdpId)
{
    _NRParam.bNRDebugPath = u1VdpId;

    vDrvNRSetSystem();
}

void vDrvNRSetForceBypass(UINT8 bOnOff)
{
    _NRParam.bForceNRBypass = bOnOff;
    vDrvNRSetSystem();
}

/**
 * @brief Set NR bypass on/off
 * @param bOnOff SV_ON: to bypass NR
 *               SV_FALSE: to enable NR
 */
void vDrvNRBypass(UINT8 bOnOff)
{
#ifdef __MODEL_slt__
	if((bGetVideoDecType(SV_VP_MAIN) == SV_VD_MPEGHD) || (bGetVideoDecType(SV_VP_MAIN) == SV_VD_MPEGSD))
	{
		return;
	}
#endif

    if(bOnOff == SV_ON) //bypass NR
    {
        //bypass Main path NR
        vRegWriteFldAlign(NR_TOP_MAIN_00, 0X0, C_MAIN_NR_EN);
        //bypass PIP path NR
        vRegWriteFldAlign(TOP_PIP_00, 0X0, C_PIP_SRC_SEL);  //For 5365 code porting
        
        //todo: turn off 3D NR??
    }
    else    //enable NR
    {
        if(_NRParam.bNRPath == SV_VP_MAIN)
        {
            //enable NR on Main path
            vRegWriteFldAlign(NR_TOP_MAIN_00, 0X1, C_MAIN_NR_EN);
            //disable NR on Sub Path
            vRegWriteFldAlign(TOP_PIP_00, 0X0, C_PIP_SRC_SEL);  //For 5365 code porting
        }
        else
        {
            //enable NR on PIP path
            vRegWriteFldAlign(TOP_PIP_00, 0X1, C_PIP_SRC_SEL);  //For 5365 code porting
            //disable NR on Main path
            vRegWriteFldAlign(NR_TOP_MAIN_00, 0X0, C_MAIN_NR_EN);
        }        
        //todo: turn on 3D NR??
    }
}

/**
 * @brief Set 2D NR On/ Off
 * @param bOnOff SV_ON/ SV_OFF
 */
void vDrvNRSet2DNROnOff(UINT8 bOnOff)
{
	_NRParam.b2DNROnOff = bOnOff;
	vRegWriteFldAlign(NR_2DNR1F, bOnOff , R_2D_ENABLE);
}

/**
 *  @brief Set deblocking On/Off
 *  @param bOnOff SV_ON/ SV_OFF
 */
void vDrvNRSetDeblockingOnOff(UINT8 bOnOff)
{
    _NRParam.bDeblockingOnOff = bOnOff;
    vRegWriteFldAlign(NR_2DNR1F, bOnOff , R_BNEWPXLBKEN);
}

/**
 *  @brief Set mosquito On/Off
 *  @param bOnOff SV_ON/ SV_OFF
 */
void vDrvNRSetMosquitoOnOff(UINT8 bOnOff)
{
    _NRParam.bMosquitoOnOff = bOnOff;
    vRegWriteFldAlign(NR_2DNR6B, bOnOff , C_MOTION_MOS3_EN);
    vRegWriteFldAlign(NR_2DNR6B, bOnOff , C_MOTION_MOS2_EN);
    vRegWriteFldAlign(NR_2DNR6B, bOnOff , C_MOTION_MOS1_EN);
    vRegWriteFldAlign(NR_2DNR6A, bOnOff , C_STILL_MOS3_EN);
    vRegWriteFldAlign(NR_2DNR6A, bOnOff , C_STILL_MOS2_EN);
    vRegWriteFldAlign(NR_2DNR6A, bOnOff , C_STILL_MOS1_EN);
}

/**
 * @brief Set Auto NR On/ Off
 * @param bOnOff SV_ON/ SV_OFF
 */

UINT32 fgDrvGetAutoNROnOff()
{
    return fgIsAutoNREnabled;
}

/**
 * @brief Set Auto NR On/ Off
 * @param bOnOff SV_ON/ SV_OFF
 */
void vDrvSetAutoNROnOff(UINT8 bOnOff)
{   
    fgIsAutoNREnabled=bOnOff;

    if(fgIsAutoNREnabled==SV_OFF)
    {
        //Reset All Condition Effect by AUTO NR
        bReducedSaturation=0;    
        vIO32WriteFld(NR_AUTO_NR_STA,0, NR_AUTO_CUR_LV);
        //bApiVideoProc(bDrvNRGetCurrentPath(), PE_ARG_3D_NR);

        if (fgIsAutoNREnabled != fgPreAutoNREnabled)
        {
            bApiVideoProc(SV_VP_MAIN, PE_ARG_SHARPNESS);//Only Work On Main Path
            bApiVideoProc(bDrvNRGetCurrentPath(),PE_ARG_SATURATION);
        }
    }
    fgPreAutoNREnabled = fgIsAutoNREnabled;
}

static void vDrvNRConfigDatamode(void)
{
    vDrvNRConfigSRMDatamode();    
    #ifdef CC_SCPOS_3DTV_SUPPORT    
    vDrvNRConfigTDTVDatamode();    
    #endif
}

UINT8 vDrvNRGetTDTVDatamode(void)
{
    return _NRParam.bNRTDTVDatamode;
}

#ifdef CC_SCPOS_3DTV_SUPPORT
static void vDrvNRConfigTDTVDatamode(void)
{
    if ((BSP_GetIcVersion()==IC_VER_5395_AA) && IS_IC_5395())
    {
        return;
    }

    if(_NRParam.bNRTDTVDatamode == E_TDTV_NR_INTERLEAVE)
    {
        //then set to leterleave mode
        vDrvNRSetScanMode(_NRParam.bNRPath, NR_SCAN_MODE_INTERLACE);
        vRegWriteFldAlign(MAIN_RSV, 0, LR_SPLIT_EN);        
    }
    else if(_NRParam.bNRTDTVDatamode == E_TDTV_NR_SNR_TNR_BYPASS)
    {
        //disable SNR/TNR
        vDrvNRSet3DNROnOff(SV_OFF);
        vDrvNRSet2DNROnOff(SV_OFF);
        vRegWriteFldAlign(NR_3DNR00, 0, BYPASS_INK_SEL);
        vRegWriteFldAlign(MAIN_RSV, 0, LR_SPLIT_EN);
    }
    else if(_NRParam.bNRTDTVDatamode == E_TDTV_NR_SYNC_PADDING)
    {
        //disable SNR/TNR
        vDrvNRSet3DNROnOff(SV_OFF);
        vDrvNRSet2DNROnOff(SV_OFF);
        vRegWriteFldAlign(NR_3DNR00, 0, BYPASS_INK_SEL);
        vRegWriteFldAlign(MAIN_RSV, 1, LR_SPLIT_EN);
        vDrvPreprocSetSyncRegen(_NRParam.bNRPath, _NRParam.u2XOffset,_NRParam.u2YOffset,_NRParam.u2NRWidth,_NRParam.u2NRHeight);
    }
    else
    {
        vRegWriteFldAlign(MAIN_RSV, 0, LR_SPLIT_EN);
        vRegWriteFldAlign(NR_3DNR00, 1, BYPASS_INK_SEL);        
        vDrvNRSetScanMode(_NRParam.bNRPath, (bDrvVideoIsSrcInterlace(_NRParam.bNRPath)==SV_TRUE)? NR_SCAN_MODE_INTERLACE:NR_SCAN_MODE_PROGRASSIVE);        
    }
}
#endif

static void vDrvNRConfigSRMDatamode(void)
{
    UINT8 _2DNROnOff;
    UINT8 _3DNROnOff;
    UINT8 nrSource444Mode;
    NR_SCAN_MODE fgIsInterlace;

    fgIsInterlace = (bDrvVideoIsSrcInterlace(_NRParam.bNRPath)==SV_TRUE)?NR_SCAN_MODE_INTERLACE:NR_SCAN_MODE_PROGRASSIVE;

    Printf("Config NR to Datamode(%d)\n",_NRParam.bSrm3DNRMode);

    switch(_NRParam.bSrm3DNRMode)
    {        
        case E_DI_NR_Y_ONLY:
            _2DNROnOff = SV_ON;
            _3DNROnOff = SV_ON;
            nrSource444Mode = 0;
            break;
        case E_DI_NR_YC_420:
            _2DNROnOff = SV_ON;
            _3DNROnOff = SV_ON;
            nrSource444Mode = 0;
            break;
        case E_DI_NR_YC_422:
            _2DNROnOff = SV_ON;
            _3DNROnOff = SV_ON;
            nrSource444Mode = 0;
            break;
        case E_DI_NR_RGB:
            _2DNROnOff = SV_OFF;	//RGB input can't turn on SNR
            _3DNROnOff = SV_OFF;
            nrSource444Mode = 1;
            break;
        case E_DI_NR_OFF:
    		#ifdef __MODEL_slt__        	
    		//in slt, turn off 2DNR
        	_2DNROnOff = SV_OFF;
		    #else
    		_2DNROnOff = SV_OFF;
    		#endif
            _3DNROnOff = SV_OFF;
            nrSource444Mode = 0;
            break;
        case E_DI_NR_UNKNOWN:
            _2DNROnOff = SV_OFF;
            _3DNROnOff = SV_OFF;
            nrSource444Mode = 0;
            break;
        default:
            _2DNROnOff = SV_OFF;
            _3DNROnOff = SV_OFF;
            nrSource444Mode = 0;
            return;
    }

    vDrvNRSet2DNROnOff(_2DNROnOff);
    vDrvNRSet3DNROnOff(_3DNROnOff);        

    vRegWriteFldAlign(PAT_MAIN_00, nrSource444Mode, C_MAIN_SOURCE_444); //Only enabled for 444

    vDrvNRSetScanMode(_NRParam.bNRPath, fgIsInterlace);
}

static void vDrvNRSetDatamodeChanged(UINT8 modeChange)
{
    _NRParam.bDatamodeChanged = modeChange;
}

static UINT8 bDrvNRIsDatamodeChanged(void)
{
    return _NRParam.bDatamodeChanged;
}

UINT32 tempTDTVDatamode = 0;
void vDrvNROnTDTVCallback(void)
{
    //query current TDTV datamode
    //UINT32 mode = tempTDTVDatamode;
    UINT32 mode = u4DrvTDTVNRModeQuery();

    if(_NRParam.bNRTDTVDatamode != mode)
    {
        _NRParam.bNRTDTVDatamode = mode;        
        Printf("Set TDTV datamode to(%d)\n",mode);
        vDrvNRSetDatamodeChanged(SV_TRUE);
    }    
}
    
void vDrvNRSetNRDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode)
{    
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }

    LOG(3,"PSCAN set NR data mode (%d)\n",u1DataMode);

    if(_NRParam.bSrm3DNRMode != u1DataMode)
    {
        _NRParam.bSrm3DNRMode = u1DataMode;
        vDrvNRSetDatamodeChanged(SV_TRUE);        
    }
}


/**
 * @brief Set 3D NR IIR On/ Off.
 *  This function will turn off all 3D NR function,
 *  include dram access.
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvNRSet3DNROnOff(UINT8 bOnOff)
{
    LOG(3,"Set 3D NR(%d)\n",bOnOff);
	_NRParam.b3DNROnOff = bOnOff;
	vRegWriteFldAlign(NR_3DNR00, bOnOff, TNR_ENABLE);	
	
	//determine 3D YC should be on/ off
	vDrvNRDetermine3DNRYCOnoff();
}

/**
 * @brief Get 3D NR IIR On/ Off for vDrvDIFrameRepeat function.
 *  This function will turn off all 3D NR function,
 *  include dram access.
 * @rvl bOnOff SV_ON/SV_OFF
 */
UINT8 vDrvNRGetTNROnOff(void)
{
	return _NRParam.b3DNROnOff;
}

/**
 * @brief Set 3D NR IIR On/ Off.
 *  This function will turn off all 3D NR function,
 *  include dram access.
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvNRSet3DNROnOffFreeze(UINT8 bOnOff)
{
    vRegWriteFldAlign(NR_3DNR00, (bOnOff==SV_ON) ? _NRParam.b3DNROnOff : SV_OFF, TNR_ENABLE);		
}


/**
 * @brief Set 3D NR function On/ Off. Dram access still working.
 *  When user select 3D NR On/ Off in OSD.
 *  This function could be called to turn on/ off 3D NR function.
 *  Be noticed that dram access will still work.
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvNRSet3DNRUIOnOff(UINT8 bOnOff)
{
    LOG(3,"Set 3D NR UI(%d)\n",bOnOff);
	_NRParam.b3DNRUIOnOff =bOnOff;
	//determine 3D YC should be on/ off	
    vDrvNRDetermine3DNRYCOnoff();
}

static void vDrvNRDetermine3DNRYCOnoff(void)
{
    UINT8 bYOnOff = SV_OFF;
    UINT8 bCOnOff = SV_OFF;

    if((_NRParam.b3DNRUIOnOff == SV_OFF) ||
    #ifdef CC_DOWMBWMODE
    u1DowmBWMode
    #endif
    )
    {
        bYOnOff = SV_OFF;
        bCOnOff = SV_OFF;
    }
    else
    {
        if(_NRParam.bSrm3DNRMode == E_DI_NR_OFF ||
           _NRParam.bSrm3DNRMode == E_DI_NR_UNKNOWN)
        {
            bYOnOff = SV_OFF;
            bCOnOff = SV_OFF;            
        }
        else if(_NRParam.bSrm3DNRMode == E_DI_NR_Y_ONLY)
        {
            bYOnOff = SV_ON;
            bCOnOff = SV_OFF;        
        }
        else if(_NRParam.bSrm3DNRMode == E_DI_NR_YC_420 ||
                _NRParam.bSrm3DNRMode == E_DI_NR_YC_422 ||
                _NRParam.bSrm3DNRMode == E_DI_NR_RGB)
        {
            bYOnOff = SV_ON;
            bCOnOff = SV_ON;        
        }
        else
        {
            bYOnOff = SV_OFF;
            bCOnOff = SV_OFF;                    
        }
    }    
    
	vRegWriteFldAlign(NR_3DNR07, bYOnOff, ENYTIIR);
	vRegWriteFldAlign(NR_3DNR07, bCOnOff, ENCTIIR);
	LOG(3,"Set 3D NR Y(%d) C(%d)\n",bYOnOff,bCOnOff);
	
}

void vDrv2DNRCliOnOff(UINT8 bMode)
{
}

void vDrv3DNRCliOnOff(UINT8 bMode)
{
}

/**
 * @brief Set parameters according to signal characteristics
 * @param bIs525 SV_TRUE(525)/SV_FALSE(625)
 */
void vDrvNRSetSystem(void)
{
    //1. Check NR should switch to Main or Sub and if NR should be bypassed.
	bDrvNRDeterminePath();
    //2.set NR Path
    vDrvNRSetCurrentPath(_NRParam.bNRPath);
    //3. if NR should be bypassed. 
    if(_NRParam.bForceNRBypass == SV_ON ||
       _NRParam.bNRBypass == SV_ON)
	{
	    //set NR bypass
		vDrvNRBypass(SV_ON);
		//Skip other actions.
		return;
	}
	else
	{
	    vDrvNRBypass(SV_OFF);
	}
	//4. check if input size changed
	vDrvNRCheckInputSize();
    //5. chroma line average
    vDrvNRSetChromaLineAvgModeReg(_TVD_path,_TVD_Chroma_mode);
    
    //6. load quality table
    vDrvNRLoadQualityTable();

    //7. check VGA mode
    vDrvNRCheckVGAModeOnOff();    //move this function call to setDatamode for PHILIPS VGA problem

    #ifdef CC_SCPOS_3DTV_SUPPORT
    vDrvNROnTDTVCallback();
    #endif
}

void vDrvNRProc(void)
{  
    //0. process NR mode change
    if(bDrvNRIsDatamodeChanged() == SV_TRUE)
    {
        vDrvNRConfigDatamode();
        vDrvNRSetDatamodeChanged(SV_FALSE);
    }
    //1. check fifo length??
    vDrvNRCheckInputSize();

    //2. Auto DeBlocking
    #if ADB_ENABLE
    if (fgIsAdaptiveFlgSet(FLG_ADAPTIVE_DB))
    {
         vDrvAutoDB();
    }
    #endif

    //3. Auto NR   
    #if ANM_ENABLE
    if(fgIsAutoNREnabled)
    {
        DynamicNRCurLevel = DRVCUST_NoiseLevel();
        
        if( DynamicNRCurLevel != NM_INVALIDLEVEL)
        {
            vDrvDynamicNR(DynamicNRCurLevel);
        }                
    }
    else
    {
        DynamicNRCurLevel = NM_INVALIDLEVEL;
    }
    #endif  //ANM_ENABLE


    #if NR_BASIC_MODE_ENABLED		
    vUpdateNRBasicModeRegister();
    #endif 

    if (fgIsAdaptiveFlgSet(FLG_ADAPTIVE_NR))
    {
        vDrvColorFrmStChg();
    }

    #ifdef ENABLE_SNRSM	
	vDrvSNRSMStep();        // special meter mainloop.
    #endif //ENABLE_SNRSM

    //NM loop
    vDrvNMProc();

}

void vDrvNRSetNrBufferCb(UINT32 u4Address, UINT32 u4Width, UINT32 u4Height,
                            BOOL fg10bit, BOOL fgLumaOnly)
{
	//UINT32 u4DramSize;

    LOG(3, "%s, a=0x%x, w=%d, h=%d\n", __FUNCTION__, u4Address, u4Width, u4Height);

	//u4DramSize = (u4Width*u4Height*(2-fgLumaOnly)*8+(6-fg10bit))/(7-fg10bit);
	//vDrvNRSetBufRegion(u4Address , u4Address+u4DramSize);
}


void vDrvNRSrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    FBM_POOL_T* prPool;

    if (u4Arg1 != VDP_1)
    {
        return;
    }

    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_NR, NULL);
    if (prPool != NULL)
    {
        vDrvNRSetNrBufferCb(prPool->u4Addr, prPool->u4Width, prPool->u4Height,
                         (u4Arg2 & FBM_POOL_MODE_10BIT) > 0,
                         (u4Arg2 & FBM_POOL_MODE_NR_3D_Y_ONLY) > 0);
    }
    else
    {
        LOG(1, "_VdpSrmNrCb: fail to get pool\n");
    }
#if 0
    if (u4Arg2 & FBM_POOL_MODE_NR_BYPASS)
    {
        //3D NR OFF
        vApiVideoNRTIIROnOff(SV_OFF);
        //2D NR OFF
        vApiVideo2DNROnOff(SV_OFF);
    }
    else if (u4Arg2 & FBM_POOL_MODE_NR_3D_Y_C)
    {
        //3D NR ON
        vApiVideoNRTIIROnOff(SV_ON);
    }
    else if (u4Arg2 & FBM_POOL_MODE_NR_3D_Y_ONLY)
    {
        //3D NR ON
        vApiVideoNRTIIROnOff(SV_ON);
    }
    else if (u4Arg2 & FBM_POOL_MODE_NR_2D)
    {
        //3D NR OFF
        vApiVideoNRTIIROnOff(SV_OFF);
    }
    else
    {
    }
#endif
}

/**
 * @brief Set 3D NR Freeze on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvNRSetFreeze(UINT8 bOnOff)
{
    vRegWriteFldAlign(NR_3DNR02, bOnOff, FREEZE_3DNR);
}


UINT8 vDrvNRIsFrameStill(void)
{
	UINT8 ret = SV_FALSE;

	if(RegReadFldAlign(NR_2DNRX00, B_NEWFRST_FLAG) == 1)
	{
		ret = SV_TRUE;
	}

	return ret;
}

static void vDrvNRCheckVGAModeOnOff(void)
{
    UINT8 fgVGAOnOff = SV_FALSE;

    //rule 1: if 3D NR is in RGB mode.
    if(_NRParam.bSrm3DNRMode == E_DI_NR_RGB)
    {
        fgVGAOnOff = SV_TRUE;
    }

    //rule 2: if input is 444 and 3D NR is OFF
    if(bIsScalerInput444(_NRParam.bNRPath) && _NRParam.bSrm3DNRMode == E_DI_NR_OFF)
    {
    	fgVGAOnOff = SV_TRUE;
    }
	
    vRegWriteFldAlign(NR_3DNR05, fgVGAOnOff, VGA3D);            
}

#if NR_DISABLE_PIXEL_REP_WHEN_BLANKING_IS_SAMLL
/**
 * @brief Check if boundary repeat should be turned on/off
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
static void vDrvNRCheckBourndaryRepeat(UINT8 bPath)
{        
    UINT16 u2PreDownBlanking = 0;    
    UINT16 hTotal;
    UINT16 hActive;

    //get HTotal
    hTotal = wDrvVideoGetHTotal(bPath);                
    hActive = bDrvVideoGetWidthOverSample(bPath);    
    u2PreDownBlanking = hTotal - hActive;

	u2PreDownBlanking = (u2PreDownBlanking * u4ScpipGetHPS(bPath)) / 0x8000;
	
    LOG(3,"Blank size after pre-scaling down(%d)\n",u2PreDownBlanking);
    //disable boundary repeat when h blank(after pre-scaling down) is smaller than 40
	if(u2PreDownBlanking<= 40)
	{
//	    vRegWriteFldAlign(NR_2DNR1E, 0x1, rbBoundaryRepeatDis); //For 5365 code porting
        vRegWriteFldAlign(NR_2DNR03, 0x1 ,R_BBOUNDARYREPEATEN);
	    LOG(3,"Disable bourndary repeat function in NR\n");
	}
	else
	{
//	    vRegWriteFldAlign(NR_2DNR1E, 0x0, rbBoundaryRepeatDis); //For 5365 code porting	
        vRegWriteFldAlign(NR_2DNR03, 0x0 ,R_BBOUNDARYREPEATEN);
	}
}
#endif

static void vDrvNRCheckVBoundProtect(void)
{
    if(_NRParam.u2XOffset!=0 ||
       _NRParam.u2YOffset!=0)
    {
        vRegWriteFldAlign(NR_3DNR00, 0x0, C_V_BOUND_PROTECT);
    }
    else
    {
        vRegWriteFldAlign(NR_3DNR00, 0x1, C_V_BOUND_PROTECT);
    }
}

void vDrvNRGetNRStatus(void)
{
    //1. print nr parameter
    Printf("NR is on %s Path.\n",(_NRParam.bNRPath==SV_VP_MAIN)?"Main":"Sub");
    Printf("NR datamode (%s)\n", NRDatamodeMapping[_NRParam.bSrm3DNRMode].szName);
    Printf("NT TDTV datamode (%d)\n",_NRParam.bNRTDTVDatamode);
    Printf("NR is %s bypassed.\n",(_NRParam.bNRBypass==SV_TRUE || _NRParam.bForceNRBypass==SV_TRUE)?"":"NOT");
    Printf("2D NR is turned %s.\n",(_NRParam.b2DNROnOff==SV_ON)?"ON":"OFF");
    Printf("3D NR is turned %s.\n",(_NRParam.b3DNROnOff==SV_ON)?"ON":"OFF");    
    Printf("NR Width(%d) Height(%d)\n",_NRParam.u2NRWidth,_NRParam.u2NRHeight);
    //2. print sync regen info
    if(_NRParam.bNRPath==SV_VP_MAIN)
    {
    	Printf("Sync Regen is turned %s\n", (RegReadFldAlign(PAT_MAIN_00, C_MAIN_SYNC_MODIFY) == 1)?"ON":"OFF");
        Printf("Sync Regen size (HSTART,HEND,VSTART,VEND) = (%d,%d,%d,%d)\n",
    			RegReadFldAlign(PAT_MAIN_01, C_MAIN_SELF_HSTART),
    			RegReadFldAlign(PAT_MAIN_01, C_MAIN_SELF_HEND),
    			RegReadFldAlign(PAT_MAIN_02, C_MAIN_SELF_VSTART),
    			RegReadFldAlign(PAT_MAIN_02, C_MAIN_SELF_VEND));
    }
    else
    {
    	Printf("Sync Regen is turned %s\n", (RegReadFldAlign(PAT_PIP_00, C_PIP_SYNC_MODIFY) == 1)?"ON":"OFF");
        Printf("Sync Regen size (HSTART,HEND,VSTART,VEND) = (%d,%d,%d,%d)\n",
    			RegReadFldAlign(PAT_PIP_01, C_PIP_SELF_HSTART),
    			RegReadFldAlign(PAT_PIP_01, C_PIP_SELF_HEND),
    			RegReadFldAlign(PAT_PIP_02, C_PIP_SELF_VSTART),
    			RegReadFldAlign(PAT_PIP_02, C_PIP_SELF_VEND));    
    }
}

void vDrvNRSetState(NR_STATE state)
{
    _NRParam.bState = state;
}

UINT8 bWaitNextVSync;

void vDrvNRCheckState(void)
{
    switch(_NRParam.bState)
    {
        case NR_STATE_STABLE:
            break;
        case NR_STATE_REGEN_CHANGED:
            //keep previous setting
            _NRParam.bPreUI = _NRParam.b3DNRUIOnOff;
            //turn off TNR Y/C
			vRegWriteFldAlign(NR_3DNR00, SV_OFF, TNR_ENABLE);	
            vDrvNRSetState(NR_STATE_COUNTDOWN);
			bWaitNextVSync = 1;
            break;
        case NR_STATE_COUNTDOWN:
            //do countdown things
            bWaitNextVSync--;
            //count to zero, to next state
            if(bWaitNextVSync == 0)
            {
                vDrvNRSetState(NR_STATE_SET_TO_PREVIOUS_UI);
            }
            break;
        case NR_STATE_SET_TO_PREVIOUS_UI:
            //recover to previous UI setting
			vRegWriteFldAlign(NR_3DNR00, _NRParam.bPreUI, TNR_ENABLE);
            vDrvNRSetState(NR_STATE_STABLE);
            break;
        default:
            break;
    }
}

