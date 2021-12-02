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
 * $RCSfile: drv_if_vdo_common.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdo_common.c
 *  Driver interface: common VDO part implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "vdp_drvif.h"
#include "mpv_if.h"
#include "dmx_if.h"
#include "nptv_if.h"
#include "aud_if.h"
#include "pmx_drvif.h"
#include "sv_const.h"
#include "panel.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
#include "vdo_ana.h"
#include "vdo_dig.h"
#include "vdo_common.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define ENABLE_SCPOS  0

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DRV_VDO_PARAM_T      _arVideoParam[DRV_VDO_NUM_PLANE];
static DRV_VDO_PARAM_T      _arVideoInterParam[DRV_VDO_NUM_PLANE];
static DRV_VDO_INTERNAL_T   _arVideoInternal[DRV_VDO_NUM_PLANE];

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

extern BOOL VdoMLInit(void);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _IsDigInputSrc(DRV_INPUT_SRC_T rSrc)
{
    if(rSrc >= DRV_IS_MPEG_1 && rSrc < DRV_IS_SCART_1)
    {
        return TRUE;
    }
    return FALSE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _DRV_VDO_SetDispFmt
 *  Get video API state
 *
 *  @param  void
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_SetDispFmt(UINT8 u1VideoIndex, 
	DRV_VDO_DISP_FMT_T rFmt)
{
    VDP_REGION_T rSrcRegion;
    VDP_REGION_T rOutRegion;
  
    switch(rFmt)
    {
        case DRV_VDO_DISP_FMT_NORMAL:   
            VERIFY(VDP_SetNonlinear(u1VideoIndex, 0) == VDP_SET_OK);
            VERIFY(VDP_Set121Map(u1VideoIndex, 0) == VDP_SET_OK);         	
            VERIFY(VDP_SetSrcRegion(u1VideoIndex, 1, rSrcRegion) == VDP_SET_OK);        
            VERIFY(VDP_SetOutRegion(u1VideoIndex, 1, rOutRegion) == VDP_SET_OK);  
            break;           
        case DRV_VDO_DISP_FMT_LETTERBOX:
            {
                UINT32 u4W;
                VERIFY(VDP_SetLBoxDetection(u1VideoIndex, 0, 0) == VDP_SET_OK);                
                VERIFY(VDP_SetNonlinear(u1VideoIndex, 0) == VDP_SET_OK);
                VERIFY(VDP_Set121Map(u1VideoIndex, 0) == VDP_SET_OK);        
                
                u4W = (_arVideoInternal[u1VideoIndex].rInfo.u2Width * PANEL_GetPanelHeight() * 1000) 
                	/ (_arVideoInternal[u1VideoIndex].rInfo.u2Height * PANEL_GetPanelWidth());
                rOutRegion.u4X = (1000 - u4W) >> 1;
                rOutRegion.u4Y = 0;                
                rOutRegion.u4Width = u4W;
                rOutRegion.u4Height = 1000;
                VERIFY(VDP_SetSrcRegion(u1VideoIndex, 1, rSrcRegion) == VDP_SET_OK);        
                VERIFY(VDP_SetOutRegion(u1VideoIndex, 0, rOutRegion) == VDP_SET_OK);
            }
            break;
        case DRV_VDO_DISP_FMT_ZOOM:        
            VERIFY(VDP_SetLBoxDetection(u1VideoIndex, 0, 0) == VDP_SET_OK);            
            VERIFY(VDP_SetNonlinear(u1VideoIndex, 0) == VDP_SET_OK);
            VERIFY(VDP_Set121Map(u1VideoIndex, 0) == VDP_SET_OK);        
        
            if( (PANEL_GetPanelWidth() * _arVideoInternal[u1VideoIndex].rInfo.u2Height) 
            	/ (PANEL_GetPanelHeight() * _arVideoInternal[u1VideoIndex].rInfo.u2Width) >= 1)
            {
                UINT32 u4H;
                u4H = (_arVideoInternal[u1VideoIndex].rInfo.u2Width * PANEL_GetPanelHeight() * 1000) 
            	/ (_arVideoInternal[u1VideoIndex].rInfo.u2Height * PANEL_GetPanelWidth());                

                rSrcRegion.u4X = 0;
                rSrcRegion.u4Y = (1000 - u4H) >> 1;
                rSrcRegion.u4Width = 1000;
                rSrcRegion.u4Height = u4H;     
            }
            else
            {
                UINT32 u4W;
                u4W = (_arVideoInternal[u1VideoIndex].rInfo.u2Height * PANEL_GetPanelWidth() * 1000) 
            	/ (_arVideoInternal[u1VideoIndex].rInfo.u2Width * PANEL_GetPanelHeight());                
                rSrcRegion.u4X = (1000 - u4W) >> 1;
                rSrcRegion.u4Y = 0;
                rSrcRegion.u4Width = u4W;
                rSrcRegion.u4Height = 1000;
            }
            VERIFY(VDP_SetSrcRegion(u1VideoIndex, 0, rSrcRegion) == VDP_SET_OK);        
            VERIFY(VDP_SetOutRegion(u1VideoIndex, 1, rOutRegion) == VDP_SET_OK);              
            break;
        case DRV_VDO_DISP_FMT_PANORAMA:     
            VERIFY(VDP_SetLBoxDetection(u1VideoIndex, 0, 0) == VDP_SET_OK);            
            VERIFY(VDP_SetSrcRegion(u1VideoIndex, 1, rSrcRegion) == VDP_SET_OK);        
            VERIFY(VDP_SetOutRegion(u1VideoIndex, 1, rOutRegion) == VDP_SET_OK);         	
            VERIFY(VDP_SetNonlinear(u1VideoIndex, 1) == VDP_SET_OK);
            VERIFY(VDP_SetNonlinearFactor(u1VideoIndex, 100, 100) == VDP_SET_OK);             
            break;
        case DRV_VDO_DISP_FMT_DOTBYDOT:     
            VERIFY(VDP_SetLBoxDetection(u1VideoIndex, 0, 0) == VDP_SET_OK);
            VERIFY(VDP_SetNonlinear(u1VideoIndex, 0) == VDP_SET_OK);
            VERIFY(VDP_Set121Map(u1VideoIndex, 1) == VDP_SET_OK);
            break;   
        case DRV_VDO_DISP_FMT_AUTO:
            VERIFY(VDP_SetLBoxDetection(u1VideoIndex, 1, 0) == VDP_SET_OK);
            VERIFY(VDP_SetNonlinear(u1VideoIndex, 0) == VDP_SET_OK);
            VERIFY(VDP_Set121Map(u1VideoIndex, 0) == VDP_SET_OK);         	
            VERIFY(VDP_SetSrcRegion(u1VideoIndex, 1, rSrcRegion) == VDP_SET_OK);        
            VERIFY(VDP_SetOutRegion(u1VideoIndex, 1, rOutRegion) == VDP_SET_OK);  
            break;              
        default:
            return DRV_E_INV_ARG;
    }
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Left = rOutRegion.u4X;
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Top = rOutRegion.u4Y;
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Width = rOutRegion.u4Width;
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Height = rOutRegion.u4Height;    
    _arVideoParam[u1VideoIndex].rDispFmt = rFmt;
    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** _DRV_VDO_Display
 *  Enable Video plane
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_DriverUnmute(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;


    rRetCode = _DRV_VDO_SetOutputWindow(u1VideoIndex, &_arVideoParam[u1VideoIndex].rVideoWindow);
#if ENABLE_SCPOS        
    if(u1VideoIndex == 0)
    {
        rRetCode = _DRV_VDO_SetDispFmt(u1VideoIndex, _arVideoParam[u1VideoIndex].rDispFmt);
    }
#endif
    rRetCode = _DRV_VDO_SetOverScanRatio(u1VideoIndex, 
        _arVideoInternal[u1VideoIndex].u1XOverscan, _arVideoInternal[u1VideoIndex].u1YOverscan);

    UNUSED(rRetCode);

    
    VERIFY(VDP_SetFreeze(u1VideoIndex, 0) == VDP_SET_OK);

#if 0
    if(bApiVideoSetVideoSrc(u1VideoIndex, _arVideoParam[u1VideoIndex].rInputSrc) == SV_FAIL)
    {
        return DRV_E_FAIL;
    }  
#else    
    bApiVideoSetVideoSrc(u1VideoIndex, _arVideoParam[u1VideoIndex].rInputSrc);
#endif

    VERIFY(VDP_SetEnable(u1VideoIndex, 1) == VDP_SET_OK);
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_Mute
 *  Disable Video plane
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_DriverMute(UINT8 u1VideoIndex)
{
#if 1
    VERIFY(VDP_SetEnable(u1VideoIndex, 0) == VDP_SET_OK);
#endif
    bApiVideoSetVideoSrc(u1VideoIndex, SV_VS_MAX);
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_VDO_Unmute(UINT8 u1VideoIndex)
{
#if 1   
    if(u1VideoIndex == 0)
    {
        if(VDP_SetInput(u1VideoIndex, 0, 0) != VDP_SET_OK)
        {
            return DRV_E_FAIL;
        }
    }
    if(VDP_SetMode(u1VideoIndex, VDP_MODE_NORMAL) != VDP_SET_OK)
    {
        return DRV_E_FAIL;
    }
#endif    
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_VDO_Mute(UINT8 u1VideoIndex)
{
    UINT32 i4Ret;
    // Blue mute color
#if 1
    i4Ret = VDP_SetBg(u1VideoIndex, _arVideoParam[u1VideoIndex].u4BackgroundColor);
    i4Ret |= VDP_SetMode(u1VideoIndex, VDP_MODE_BG);    
    if(i4Ret != VDP_SET_OK)
    {
        return DRV_E_FAIL;
    }
#endif    
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_VDO_GetInternal(UINT8 u1VideoIndex, 
	DRV_VDO_INTERNAL_T** prInternal)
{
    VERIFY(u1VideoIndex < 2);
    *prInternal = &_arVideoInternal[u1VideoIndex];
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_VDO_GetInfo(UINT8 u1VideoIndex, 
	DRV_VDO_INFO_T* prInfo)
{
    VERIFY(u1VideoIndex < 2);
    VERIFY(prInfo != NULL);
  
    prInfo->fg4_3               = _arVideoInternal[u1VideoIndex].rInfo.fg4_3;
    prInfo->fgProgressive       = _arVideoInternal[u1VideoIndex].rInfo.fgProgressive;    
    prInfo->fgVgaTiming         = _arVideoInternal[u1VideoIndex].rInfo.fgVgaTiming;    
    prInfo->rInputSrc           = _arVideoInternal[u1VideoIndex].rInfo.rInputSrc;        
    prInfo->rSigStatus          = _arVideoInternal[u1VideoIndex].rInfo.rSigStatus;        
    prInfo->u2FramePerSecond    = _arVideoInternal[u1VideoIndex].rInfo.u2FramePerSecond;        
    prInfo->u2Height            = _arVideoInternal[u1VideoIndex].rInfo.u2Height;        
    prInfo->u2Width             = _arVideoInternal[u1VideoIndex].rInfo.u2Width;        
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_VDO_GetResolution(UINT8 u1VideoIndex,
	DRV_VDO_INFO_T* prInfo, UINT32 u4Tag)
{
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_VDO_GetParameter(UINT8 u1VideoIndex, 
	DRV_VDO_PARAM_T** pprParam)
{
    VERIFY(u1VideoIndex < 2);
    *pprParam =  &(_arVideoParam[u1VideoIndex]);
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_SetParameter
 *  Set parameters of video source; the parameters being set does not take effect until a call to 
 *  DRV_VDO_Start
 *
 *  @param  u1VideoIndex          Video plane index
 *  @param  prParam			Pointer to DRV_VDO_PARAM_T structure
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_SetParameter(UINT8 u1VideoIndex, 
	DRV_VDO_PARAM_T* prParam)
{
    VERIFY(u1VideoIndex < DRV_VDO_NUM_PLANE);
    VERIFY(prParam != NULL);
    if(x_memcpy(&_arVideoInterParam[u1VideoIndex], prParam, sizeof(DRV_VDO_PARAM_T)) == NULL)
    {
        return DRV_E_FAIL;
    }
    /*
    _arVideoParam[u1VideoIndex].rInputSrc           = prParam->rInputSrc;
    _arVideoParam[u1VideoIndex].u4BackgroundColor   = prParam->u4BackgroundColor;
    _arVideoParam[u1VideoIndex].rCallbackList       = prParam->rCallbackList;
    _arVideoParam[u1VideoIndex].rVideoWindow        = prParam->rVideoWindow;
    _arVideoParam[u1VideoIndex].u2Pid               = prParam->u2Pid;
    _arVideoParam[u1VideoIndex].u2PcrPid            = prParam->u2PcrPid;    
    */
    return DRV_E_OK;    
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_Start
 *  Start playing and displaying the video source
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_Start(UINT8 u1VideoIndex)
{
    if(_arVideoInternal[u1VideoIndex].u1State == DRV_VDO_STATE_PLAY)
    {
        return DRV_E_FAIL;
    }
    _arVideoInternal[u1VideoIndex].u1State = DRV_VDO_STATE_PLAY;
    if(x_memcpy(&_arVideoParam[u1VideoIndex], &_arVideoInterParam[u1VideoIndex], sizeof(DRV_VDO_PARAM_T)) == NULL)
    {
        return DRV_E_FAIL;
    }
    if( _IsDigInputSrc(_arVideoParam[u1VideoIndex].rInputSrc))
    {
        return _DRV_VDO_DIG_Start(u1VideoIndex);
    }
    else
    { 
        return _DRV_VDO_ANA_Start(u1VideoIndex);
    }
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_Stop
 *  Stop playing the video source
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_Stop(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;
    
    if(_arVideoInternal[u1VideoIndex].u1State == DRV_VDO_STATE_STOP)
    {
        return DRV_E_OK;
    }

    if( _IsDigInputSrc(_arVideoParam[u1VideoIndex].rInputSrc))
    {

        rRetCode = _DRV_VDO_DIG_Stop(u1VideoIndex);
        
    }
    else
    { 
        _arVideoInternal[u1VideoIndex].u1State = DRV_VDO_STATE_STOP;
        rRetCode = _DRV_VDO_ANA_Stop(u1VideoIndex);
    }
    _arVideoInternal[u1VideoIndex].u1State = DRV_VDO_STATE_STOP;    
    return rRetCode;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_Freeze
 *  Pause the display of the video source
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_Freeze(UINT8 u1VideoIndex)
{
    VERIFY(VDP_SetFreeze(u1VideoIndex, 1) == VDP_SET_OK);
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_Resume
 *  Resume the display of the video source
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_Resume(UINT8 u1VideoIndex)
{
    VERIFY(VDP_SetFreeze(u1VideoIndex, 0) == VDP_SET_OK);
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_SetOutputWindow
 *  Set the display window of video
 *
 *  @param  u1VideoIndex          Video plane index
 *  @param  prWindow			Pointer to DRV_VDO_WINDOW_T structure
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_SetOutputWindow(UINT8 u1VideoIndex, 
	DRV_VDO_WINDOW_T* prWindow)
{
    VDP_REGION_T rOutRegion;
    VDP_REGION_T rSrcRegion;
    
    rOutRegion.u4X      = prWindow->u4Left;
    rOutRegion.u4Y      = prWindow->u4Top;    
    rOutRegion.u4Width  = prWindow->u4Width;
    rOutRegion.u4Height = prWindow->u4Height;
    if(VDP_SetSrcRegion(u1VideoIndex, 1, rSrcRegion) != VDP_SET_OK)
    {
        return DRV_E_FAIL;
    }    
    if(VDP_SetOutRegion(u1VideoIndex, 0, rOutRegion) != VDP_SET_OK)
    {
        return DRV_E_FAIL;
    }
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Left = prWindow->u4Left;
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Top = prWindow->u4Top;    
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Width = prWindow->u4Width;
    _arVideoParam[u1VideoIndex].rVideoWindow.u4Height = prWindow->u4Height;    
    
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_SetOverScanRatio
 *  Set the overscan ratio of video
 *
 *  @param  u1VideoIndex          Video plane index
 *  @param  u4X				Horizontal overscan ratio 
 *  @param  u4Y				Vertical overscan ratio
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_SetOverScanRatio(UINT8 u1VideoIndex,
    UINT32 u4X, UINT32 u4Y)
{
    VDP_OVERSCAN_REGION_T rOverRegion;    

    _arVideoInternal[u1VideoIndex].u1XOverscan = u4X;
    _arVideoInternal[u1VideoIndex].u1YOverscan = u4Y;    
    
    u4X >>= 1;
    u4Y >>= 1;

    rOverRegion.u4Left      = u4X;
    rOverRegion.u4Right     = u4X;    
    rOverRegion.u4Top       = u4Y;
    rOverRegion.u4Bottom    = u4Y;    
    if(VDP_SetOverScan(u1VideoIndex, rOverRegion) != VDP_SET_OK)
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_SetQuality
 *  Set video quality /processing parameters
 *
 *  @param  u1VideoIndex          Video plane index
 *  @param  u4Mask			The mask of video quality items
 *  @param  prQuality			Pointer to DRV_VDO_QUALITY_T structure
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_SetQuality(UINT8 u1VideoIndex, 
	UINT32 u4Mask, DRV_VDO_QUALITY_T* prQuality)
{
    INT32 i4Ret;
    if(u4Mask & DRV_VDO_QTYMSK_BLENDING)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_BLENDING, prQuality->u1Blending);
    }    
    if(u4Mask & DRV_VDO_QTYMSK_BRIGHTNESS)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_BRIGHTNESS, prQuality->u1Brightness);
    }
    if(u4Mask & DRV_VDO_QTYMSK_CONTRAST)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_CONTRAST, prQuality->u1Contrast);
    }	
    if(u4Mask & DRV_VDO_QTYMSK_HUE)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_HUE, prQuality->i1Hue + 32);
    }
    if(u4Mask & DRV_VDO_QTYMSK_SATURATION)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_SATURATION, prQuality->u1Saturation);
    }
    if(u4Mask & DRV_VDO_QTYMSK_CTI)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_CTI, prQuality->u1Cti);
    }        
    if(u4Mask & DRV_VDO_QTYMSK_LTI)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_LTI, prQuality->u1Lti);
    }      
/*    
    if(u4Mask & DRV_VDO_QTYMSK_SHARPNESS_ON_OFF)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_SHARPNESS_ON_OFF, prQuality->u1SharpnessOnOff);
    }        
*/    
    if(u4Mask & DRV_VDO_QTYMSK_SHARPNESS)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_SHARPNESS_ON_OFF, 1);
        i4Ret |= VDP_SetArg(u1VideoIndex, VDP_ARG_SHARPNESS, prQuality->u1Sharpness);
    }        
    if(u4Mask & DRV_VDO_QTYMSK_NR)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_NR, prQuality->u1Nr);
    }        
    if(u4Mask & DRV_VDO_QTYMSK_COLOR_SUPPRESS)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_COLOR_SUPPRESS, prQuality->u1ColorSuppress);
    }    
/*    
    if(u4Mask & DRV_VDO_QTYMSK_DEINT_FILM)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_DEINT_FILM, prQuality->u1DeintFilm);
    }        
    if(u4Mask & DRV_VDO_QTYMSK_DEINT_DEINT)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_DEINT_DEINT, prQuality->u1DeintDeint);
    }          
    if(u4Mask & DRV_VDO_QTYMSK_DEINT_EDGE)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_DEINT_EDGE, prQuality->u1DeintEdge);
    }            
*/    
    if(u4Mask & DRV_VDO_QTYMSK_ADAPTIVE_LUMA)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_ADAPTIVE_LUMA, prQuality->u1AdaptiveLuma);
    }            
    if(u4Mask & DRV_VDO_QTYMSK_SCE)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_SCE, prQuality->u1Sce);
    }
    if(u4Mask & DRV_VDO_QTYMSK_R_GAIN)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_R_GAIN, prQuality->u1RGain);
    }                
    if(u4Mask & DRV_VDO_QTYMSK_G_GAIN)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_G_GAIN, prQuality->u1GGain);
    }                
    if(u4Mask & DRV_VDO_QTYMSK_B_GAIN)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_B_GAIN, prQuality->u1BGain);
    }                    
/*    
    if(u4Mask & DRV_VDO_QTYMSK_R_OFFSET)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_R_OFFSET, prQuality->u1ROffset);
    }                
    if(u4Mask & DRV_VDO_QTYMSK_G_OFFSET)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_G_OFFSET, prQuality->u1GOffset);
    }                
    if(u4Mask & DRV_VDO_QTYMSK_B_OFFSET)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_B_OFFSET, prQuality->u1BOffset);
    }                           
*/    
    if(u4Mask & DRV_VDO_QTYMSK_GAMMA)
    {
        i4Ret = PMX_SetArg(0, PMX_ARG_GAMMA, prQuality->u1Gamma);
    }       
    if(u4Mask & DRV_VDO_QTYMSK_WHITE_PEAK_LMT)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_WHITE_PEAK_LMT, prQuality->u1WhitePeakLmt);
    }                               
    if(u4Mask & DRV_VDO_QTYMSK_BACKLIGHT_LVL)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_BACK_LIGHT_LVL, prQuality->u1BackLightLvl);
    }                           
    if(u4Mask & DRV_VDO_QTYMSK_ADAPTIVE_BACKLIGHT)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_ADAPTIVE_BACK_LIGHT, prQuality->u1AdaptiveBackLight);
    }      
    if(u4Mask & DRV_VDO_QTYMSK_3D_NR)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_3D_NR, prQuality->u13dNr);
    }   
/*
    if(u4Mask & DRV_VDO_QTYMSK_NS)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_NS, prQuality->u1Ns);
    }             
    if(u4Mask & DRV_VDO_QTYMSK_SOFT_BWS)
    {
        i4Ret = VDP_SetArg(u1VideoIndex, VDP_ARG_SOFT_BWS, prQuality->u1SoftBws);
    }   
*/    
    if(i4Ret != VDP_SET_OK)
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_InitQuality
 *  Initialize video quality & processing settings
 *
 *  @param  void
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_InitQuality(void)
{
    VERIFY(VDP_SetArg(0, VDP_ARG_ADAPTIVE_BACK_LIGHT, 0)    == VDP_SET_OK);
    
    VERIFY(VDP_SetArg(0, VDP_ARG_R_GAIN, 0x80)              == VDP_SET_OK);
    VERIFY(VDP_SetArg(0, VDP_ARG_G_GAIN, 0x80)              == VDP_SET_OK);
    VERIFY(VDP_SetArg(0, VDP_ARG_B_GAIN, 0x80)              == VDP_SET_OK);
    VERIFY(VDP_SetArg(1, VDP_ARG_R_GAIN, 0x80)              == VDP_SET_OK);
    VERIFY(VDP_SetArg(1, VDP_ARG_G_GAIN, 0x80)              == VDP_SET_OK);
    VERIFY(VDP_SetArg(1, VDP_ARG_B_GAIN, 0x80)              == VDP_SET_OK);

    VERIFY(VDP_SetArg(0, VDP_ARG_R_OFFSET, 0x80)            == VDP_SET_OK);
    VERIFY(VDP_SetArg(0, VDP_ARG_G_OFFSET, 0x80)            == VDP_SET_OK);
    VERIFY(VDP_SetArg(0, VDP_ARG_B_OFFSET, 0x80)            == VDP_SET_OK);
    VERIFY(VDP_SetArg(1, VDP_ARG_R_OFFSET, 0x80)            == VDP_SET_OK);
    VERIFY(VDP_SetArg(1, VDP_ARG_G_OFFSET, 0x80)            == VDP_SET_OK);
    VERIFY(VDP_SetArg(1, VDP_ARG_B_OFFSET, 0x80)            == VDP_SET_OK);
    return DRV_E_OK;
}
typedef struct _NAV_OPTION_T {
	// Input src enable list
	BOOL fgEnableATV;
	BOOL fgEnableCVBS1;
	BOOL fgEnableCVBS2;
	BOOL fgEnableCVBS3;
	BOOL fgEnableS1;
	BOOL fgEnableS2;
	BOOL fgEnableS3;
	BOOL fgEnableYPbPr1;
	BOOL fgEnableYPbPr2;
	BOOL fgEnableHDMI;
       BOOL fgEnableHDMI2;
	BOOL fgEnableVGA;

	BOOL fgEnableDTV;
	BOOL fgEnableI1394;

	// Audio enable
	BOOL fgEnableAudio; 

	// OSD configuration
	BOOL fgEnableOSD32;

	
	BOOL fgDTVFullScan;

       UINT32 u4PanelId;
	
} NAV_OPTION_T; 
EXTERN NAV_OPTION_T rNAVOption;
//-----------------------------------------------------------------------------
/** _DRV_VDO_Init
 *  Initialize video API common part
 *
 *  @param  void
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_Init(void)
{    
    UINT32 i;    
    // osd1 -> osd2 -> main -> pip -> osd3
    UINT32 au4PlaneOrder[5] = {VDP_3, VDP_4, VDP_2, VDP_1, VDP_5};

#ifndef __KERNEL__    
    SelectPanel(rNAVOption.u4PanelId);
#endif

    FBM_Init(); 
    VERIFY(AUD_Init() == AUD_OK);
    VERIFY(DMX_Init());        
    MPV_Init();
    VDP_Init();
    VERIFY(VdoMLInit());

    PMX_Init();   

    //VERIFY(PMX_SetBg(0, 0) == PMX_SET_OK);
    VERIFY(PMX_SetPlaneOrderArray(PMX_1, au4PlaneOrder) == PMX_SET_OK);
    

    
    for(i = 0; i < DRV_VDO_NUM_PLANE; i++)
    {
        x_memset(&_arVideoInternal[i], 0, sizeof(DRV_VDO_INTERNAL_T));
        x_memset(&_arVideoParam[i], 0, sizeof(DRV_VDO_PARAM_T));
        x_memset(&_arVideoInterParam[i], 0, sizeof(DRV_VDO_PARAM_T));        
    }     

    _arVideoParam[0].rVideoWindow.u4Height = 1000;
    _arVideoParam[0].rVideoWindow.u4Width = 1000;
    _arVideoParam[0].rVideoWindow.u4Left = 0;
    _arVideoParam[0].rVideoWindow.u4Top= 0;    
    _arVideoInternal[0].rInfo.u2Height = 480;
    _arVideoInternal[0].rInfo.u2Width = 720;
    
    VERIFY(VDP_SetBg(0, 0x0) == VDP_SET_OK);
    VERIFY(VDP_SetBg(1, 0x0) == VDP_SET_OK);
    VERIFY(VDP_SetMode(0, VDP_MODE_NORMAL) == VDP_SET_OK);
    VERIFY(VDP_SetMode(1, VDP_MODE_NORMAL) == VDP_SET_OK);
    
    _DRV_VDO_InitQuality();    
    _DRV_VDO_ANA_Init();
    _DRV_VDO_DIG_Init();

    _VDP_vDrvDIFWOnOff(0, 1);    

    /*
    PMX_Init();
    VERIFY(PMX_SetBg(0, 0) == PMX_SET_OK);
    {
        UINT32 au4PlaneOrder[4] = {3, 4, 1, 0};
        VERIFY(PMX_SetPlaneOrderArray(PMX_1, au4PlaneOrder) == PMX_SET_OK);
    }    
    VERIFY(PMX_SetEnable(0, 1) == PMX_SET_OK);  
    */
     
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_GetState
 *  Get video API state
 *
 *  @param  void
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
UINT32 _DRV_VDO_GetState(UINT8 u1VideoIndex)
{
    return _arVideoInternal[u1VideoIndex].u1State;
}

DRV_ERR_CODE_T _DRV_VDO_GetMessage(INT32 i4Tag)
{
}
