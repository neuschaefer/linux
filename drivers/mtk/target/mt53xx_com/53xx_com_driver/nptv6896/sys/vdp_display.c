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
 * $Author: chang.ren $
 * $Date  $
 * $RCSfile: vdp_display.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

//#define _API_VIDEO_DISPLAY_C_

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "x_assert.h"

LINT_EXT_HEADER_BEGIN
#include "general.h"
#include "hw_scpos.h"
#include "drv_video.h"
#include "video_def.h"
#include "source_table.h"
#include "drv_common.h"
#include "vdp_drvif.h"
#include "x_timer.h"
#include "nptv_debug.h"
#include "drv_scaler.h"
#include "drv_lbox.h"
#include "vdp_display.h"
#include "vdo_misc.h"
#include "drv_scaler_drvif.h"
#ifdef DRV_SUPPORT_EXTMJC
//#include "extmjc_if.h"
#endif

//#ifndef CC_VDP_VGA_OVERSCAN_BY_AP
//#define CC_VDP_IGNORE_OVERSCAN_AT_VGA
//#endif

#ifdef CC_SRM_ON
#include "srm_drvif.h"
#endif

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#if defined(SCPIP_SUPPORT_POST_SCALER)
static UINT16 u2FscOutWidthLimit = 1920;
#ifdef IS_SUPPORT_3D_PHOTO
UINT16 wOriHeight = VDP_MAX_REGION_HEIGHT;
EXTERN TDTV_3D_CROP_INTO_T   _rM3DCropInfo;
#endif

//#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8 CalculateOutputRegion(UINT8 bPath, const VDP_DISP_REGION_T* prIn, VDP_DISP_REGION_T* prOut);
static UINT8 CalculateInputRegion(UINT8 u1Path, const VDP_PIC_OVERSCAN_REGION_T* prOverScan,  const VDP_DISP_REGION_T* prIn, VDP_DISP_REGION_T* prOut);
static UINT8 bVideoUpdateDispRegion(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight); 
static void _vVideoUpdate121DispRegion(UINT8 bPath,VDP_DISP_REGION_T* rInRegion);
//-----------------------------------------------------------------------------
/** Brief of CalculateOutputRegion.
 */
//-----------------------------------------------------------------------------
static UINT8 CalculateOutputRegion(UINT8 bPath, const VDP_DISP_REGION_T* prIn, VDP_DISP_REGION_T* prOut)
{
    //check input data
    if ((prIn == NULL) ||(prOut == NULL))
    {
	    return (SV_FAIL);
    }

    if (((prIn->u4X + prIn->u4Width) > VDP_MAX_REGION_WIDTH) ||
        ((prIn->u4Y + prIn->u4Height) > VDP_MAX_REGION_HEIGHT))
    {
	    return (SV_FAIL);
    }
    
    //transfer (0~10000) to pixel-based output window
    #if defined(SCPIP_SUPPORT_POST_SCALER)
     	prOut->u4X = DIVIDE_WITH_ROUND(u2FscGetMaxOutWidth() * prIn->u4X,VDP_MAX_REGION_WIDTH);
    	prOut->u4Width = DIVIDE_WITH_ROUND(u2FscGetMaxOutWidth()  * prIn->u4Width,VDP_MAX_REGION_WIDTH);
    #else
    	prOut->u4X = DIVIDE_WITH_ROUND(TV_WIDTH * prIn->u4X,VDP_MAX_REGION_WIDTH);
    	prOut->u4Width = DIVIDE_WITH_ROUND(TV_WIDTH * prIn->u4Width,VDP_MAX_REGION_WIDTH);
    #endif
    prOut->u4Y = DIVIDE_WITH_ROUND(TV_HEIGHT * prIn->u4Y,VDP_MAX_REGION_WIDTH);
    prOut->u4Height = DIVIDE_WITH_ROUND(TV_HEIGHT * prIn->u4Height,VDP_MAX_REGION_WIDTH);
    
    //make output width to be even pixel
    if((prOut->u4Width % 2) && !bIsScalerInput444(bPath))
    {
        if(prOut->u4X ==0)
        {
            prOut->u4Width -= 1;
        }
        else
        {
           // prOut->u4X -=1;//change this for cr[DTV00355671]
            prOut->u4Width += 1;
        }

        LOG(3, "Transfer 422 output width to be even pixel:  (x, width) =(%d, %d)\n", (UINT32)prOut->u4X, (UINT32)prOut->u4Width);
    }
    
    return (SV_SUCCESS);
}

//-----------------------------------------------------------------------------
/** Brief of CalculateInputRegion.
 */
//-----------------------------------------------------------------------------
extern UINT32 _u4LBoxDetectionEnable;

static UINT8 CalculateInputRegion(UINT8 u1Path, const VDP_PIC_OVERSCAN_REGION_T* prOverScan,  const VDP_DISP_REGION_T* prIn, VDP_DISP_REGION_T* prOut)
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4LBoxX = 0;
    UINT32 u4LBoxY = 0;
    UINT32 u4LBoxWidth = 0;
    UINT32 u4LBoxHeight = 0;
    UINT32 u4OverScanRight;
    UINT32 u4OverScanBottom;
    VDP_DISP_REGION_T rOverScaned;
    UINT32 u4LBoxVValid, u4LBoxHValid;
	UINT8 bMode;
	

    if ((prOverScan == NULL) || (prIn == NULL) ||(prOut == NULL))
    {
	return (SV_FAIL);
    }

    if (((prOverScan->u4Top + prOverScan->u4Bottom) > VDP_MAX_REGION_HEIGHT) ||
        ((prOverScan->u4Left + prOverScan->u4Right) > VDP_MAX_REGION_WIDTH))
    {
	return (SV_FAIL);
    }

    if (((prIn->u4X + prIn->u4Width) > VDP_MAX_REGION_WIDTH) ||
        ((prIn->u4Y + prIn->u4Height) > VDP_MAX_REGION_HEIGHT))
    {
	return (SV_FAIL);
    }

    // Get Source Size
    u4Width = wDrvVideoInputWidth(u1Path);
    u4Height = wDrvVideoInputHeight(u1Path);
    u4LBoxVValid = 0;
    u4LBoxHValid = 0;
    
	bMode = u1ScpipGetDispMode(u1Path);	 
	
	//currently, only main path support LBOX detection.
    if (u1Path == SV_VP_MAIN && LboxQueryRegion(&u4LBoxX, &u4LBoxY, &u4LBoxWidth, &u4LBoxHeight, &u4LBoxHValid, &u4LBoxVValid))
    {
        LOG(4, "Get LBox Region: (%d, %d) : (%d, %d, %d, %d)\n", u4LBoxHValid, u4LBoxVValid, u4LBoxX, u4LBoxY, u4LBoxWidth, u4LBoxHeight);
    }
    else    	
    {
        u4LBoxVValid = 0;
        u4LBoxHValid = 0;    
    }
    

    // Calculate Source Size After OverScan
#ifdef CC_VDP_IGNORE_OVERSCAN_AT_VGA
    if (fgApiVideoIsVgaTiming(u1Path) != 0)
    {
        if (u4LBoxVValid == 0)	
        {
            rOverScaned.u4Y = 0;
            rOverScaned.u4Height = u4Height;
        }
        else
        {
            rOverScaned.u4Y = u4LBoxY;
            rOverScaned.u4Height = u4LBoxHeight;
        }
        if (u4LBoxHValid == 0)
        {
            rOverScaned.u4X = 0;
            rOverScaned.u4Width = u4Width;
        }
        else
        {
            rOverScaned.u4X = u4LBoxX;
            rOverScaned.u4Width = u4LBoxWidth;
        }        	
    }
    else
#endif
    {
        if ((u4LBoxVValid == 0)	|| ((_u4LBoxDetectionEnable & LBOX_ONOFF_AP)!=LBOX_ONOFF_AP))
        {
            rOverScaned.u4Y = (u4Height * prOverScan->u4Top) / VDP_MAX_REGION_HEIGHT;
            u4OverScanBottom= (u4Height * prOverScan->u4Bottom) / VDP_MAX_REGION_HEIGHT;
            rOverScaned.u4Height = u4Height - rOverScaned.u4Y - u4OverScanBottom;
        }
        else
        {   // use LBOX result
            rOverScaned.u4Y = u4LBoxY;
            rOverScaned.u4Height = u4LBoxHeight;
        }

        if ((u4LBoxHValid == 0) || ((_u4LBoxDetectionEnable & LBOX_ONOFF_AP)!=LBOX_ONOFF_AP))
        {
            rOverScaned.u4X = (u4Width * prOverScan->u4Left) / VDP_MAX_REGION_WIDTH;
            u4OverScanRight = (u4Width * prOverScan->u4Right) / VDP_MAX_REGION_WIDTH;
            rOverScaned.u4Width = u4Width - rOverScaned.u4X - u4OverScanRight;
        }
        else
        {  // use LBOX result
            rOverScaned.u4X = u4LBoxX;
            rOverScaned.u4Width = u4LBoxWidth;
        }        
    }

    // Calculate Source Region
    prOut->u4X = DIVIDE_WITH_ROUND(rOverScaned.u4Width * prIn->u4X,VDP_MAX_REGION_WIDTH);
	prOut->u4Y = DIVIDE_WITH_ROUND(rOverScaned.u4Height * prIn->u4Y,VDP_MAX_REGION_WIDTH);
    prOut->u4Width = DIVIDE_WITH_ROUND(rOverScaned.u4Width * prIn->u4Width,VDP_MAX_REGION_WIDTH);
	prOut->u4Height = DIVIDE_WITH_ROUND(rOverScaned.u4Height * prIn->u4Height,VDP_MAX_REGION_WIDTH);
	
	
    prOut->u4X += rOverScaned.u4X;
    prOut->u4Y += rOverScaned.u4Y;
    //when (x,width) or (h,height) rounding at the same time
    if(prOut->u4X + prOut->u4Width > u4Width )
    {
       prOut->u4Width = prOut->u4Width - DIVIDE_WITH_CARRIER(prOut->u4X + prOut->u4Width - u4Width,2); 
    }
    
    if(prOut->u4Y + prOut->u4Height > u4Height )
    {
       prOut->u4Height = prOut->u4Height - DIVIDE_WITH_CARRIER(prOut->u4Y + prOut->u4Height - u4Height,2); 
    }

    LOG(3, "CalculateInputRegion bVideoUpdateSrcRegion(%d): SRC(%d, %d, %d, %d), \n",u1Path,prOut->u4X,prOut->u4Y,prOut->u4Width,prOut->u4Height);
	//when 422 input ,the prOut->u4X need to change to even,because H boudary only can protect even pixels, and
	//hp_ofst only can be set even value.
    if((prOut->u4Width % 2) && !bIsScalerInput444(u1Path))
    {
        if(prOut->u4X ==0)
        {
            prOut->u4Width -= 1;
        }
        else
        {
            prOut->u4X -=1;
            prOut->u4Width += 1;
        }

         LOG(3, "Transfer 422 input width to be even pixel:  (x, width) =(%d, %d)\n", (UINT32)prOut->u4X, (UINT32)prOut->u4Width);
    }

    if((prOut->u4Y %2)&& (bMode == VDP_SCPOS_PSCAN_DISPMODE)  && (bDrvVideoIsSrcInterlace(u1Path)))
    {
        prOut->u4Y += 1;
        if(prOut->u4Height % 2)
        {
            prOut->u4Height -= 1;
        }
        LOG(3, "Transfer PSCAN display mode input height to be even lines:  (y, height) =(%d, %d)\n", (UINT32)prOut->u4Y, (UINT32)prOut->u4Height);
    }
    return (SV_SUCCESS);
}

static void _vVideoUpdate121DispRegion(UINT8 bPath,VDP_DISP_REGION_T* rInRegion)
{
    UINT32 u4VdpId;
    RPicInfo* picInfo;    
    RVChannel* channel;
    UINT8 decType;          //decoder type 
    BOOL bOutWidhtHalfIn121 = SV_FALSE;
    u4VdpId = getScalerVDP(bPath);
    picInfo = getPicInfo(bPath);    
	channel = getChannel(bPath);
    decType = channel->bDecType;
    
    if(_arScalePrm[u4VdpId].u4121MapEn == SV_ON) //121 is enabled, calculate and overwrite output region setting.
    {
        //error handling
        if(u2FscGetMaxOutWidth() ==0 || wDISPLAY_HEIGHT ==0)
        {
            return;
        }

	    // oversample timing need width/2 in oversample timing
	    if (( decType == (UINT8) SV_VD_YPBPR || decType == (UINT8) SV_VD_DVI) && 
	            bDrvVideoGetWidthOverSample(u4VdpId))
	    {
	        bOutWidhtHalfIn121 = SV_TRUE;
	    }
		else
		{
		    bOutWidhtHalfIn121 = SV_FALSE;
        }
            
    	if(_arScalePrm[u4VdpId].u2121MapHPos == SCPIP_121MAP_H_POS_LEFT)
    	{
    	    rInRegion->u4X  = 0;
    	}
    	else if(_arScalePrm[u4VdpId].u2121MapHPos == SCPIP_121MAP_H_POS_RIGHT)
    	{
    	    if(bOutWidhtHalfIn121)
	    	{
	    	    rInRegion->u4X  = ((((UINT32)u2FscGetMaxOutWidth() -(UINT32)(picInfo->wTargetW >> 1)) * VDP_MAX_REGION_WIDTH ) / (UINT32)u2FscGetMaxOutWidth())  ;
    	    }
			else
			{
    	        rInRegion->u4X  = ((((UINT32)u2FscGetMaxOutWidth() -(UINT32)picInfo->wTargetW) * VDP_MAX_REGION_WIDTH ) / (UINT32)u2FscGetMaxOutWidth())  ;
    		}
    	}
    	else // middle
    	{
    	    if(bOutWidhtHalfIn121)
	    	{
	            rInRegion->u4X  = (((((UINT32)u2FscGetMaxOutWidth() -(UINT32)(picInfo->wTargetW >> 1)) * VDP_MAX_REGION_WIDTH) / (UINT32)u2FscGetMaxOutWidth()) /2)  ;
	    	}
			else
			{
			    rInRegion->u4X  = (((((UINT32)u2FscGetMaxOutWidth() -(UINT32)picInfo->wTargetW) * VDP_MAX_REGION_WIDTH) / (UINT32)u2FscGetMaxOutWidth()) /2)  ;
			}
    	}
    	
        rInRegion->u4Y =      ((((wDISPLAY_HEIGHT -(UINT32)picInfo->wTargetH)/2)* VDP_MAX_REGION_HEIGHT) /wDISPLAY_HEIGHT)  ;
		if(bOutWidhtHalfIn121)
		{
		    rInRegion->u4Width =  (((UINT32)(picInfo->wTargetW >> 1) * VDP_MAX_REGION_WIDTH) / (UINT32)u2FscGetMaxOutWidth()) ;
		}
		else
		{
            rInRegion->u4Width =  (((UINT32)picInfo->wTargetW * VDP_MAX_REGION_WIDTH) / (UINT32)u2FscGetMaxOutWidth()) ;
		}
        rInRegion->u4Height = (((UINT32)picInfo->wTargetH * VDP_MAX_REGION_HEIGHT)/ wDISPLAY_HEIGHT) ;        

        //error handling
        rInRegion->u4X = (rInRegion->u4X>VDP_MAX_REGION_WIDTH)?VDP_MAX_REGION_WIDTH:rInRegion->u4X;
        rInRegion->u4Y = (rInRegion->u4Y>VDP_MAX_REGION_HEIGHT)?VDP_MAX_REGION_HEIGHT:rInRegion->u4Y;
        rInRegion->u4Width = (rInRegion->u4Width>VDP_MAX_REGION_WIDTH)?VDP_MAX_REGION_WIDTH:rInRegion->u4Width;
        rInRegion->u4Height = (rInRegion->u4Height>VDP_MAX_REGION_HEIGHT)?VDP_MAX_REGION_HEIGHT:rInRegion->u4Height;

        LOG(3,"121 is enabled. PicInfo(w,h)=(%d,%d) Update output region to (x,y,w,h)=(%d,%d,%d,%d).\n", 
                picInfo->wTargetW, picInfo->wTargetH,
                rInRegion->u4X, rInRegion->u4Y, rInRegion->u4Width, rInRegion->u4Height);
    }
}


/**
 * @brief bVideoUpdateDispRegion
 
 * Set display position and size of the video window
 
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         wXOff: the horizontal position of the display window 
 *         wYOff: the vertical position of the display window
 *         wWidth: the width of the display window 
 *         wHeight: the height of the display window
 * @retval SV_SUCCESS/SV_FAIL
 */
static UINT8 bVideoUpdateDispRegion(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight) 
{
    RDispInfo *prDispInfo;
    VDP_DISP_REGION_T rInRegion;
    VDP_DISP_REGION_T rOutRegion;
    VDP_REGION_T rOutputRegion;

    prDispInfo = getDispInfo(bPath);

    rInRegion.u4X = wXOff;
    rInRegion.u4Y = wYOff;
    rInRegion.u4Width = wWidth;
    rInRegion.u4Height = wHeight;
    //store output region set from VDP into dispInfo->rOutRegion
    prDispInfo->rOutRegion = rInRegion;        

    //update disp region if 121 is enabled.
    _vVideoUpdate121DispRegion(bPath,&rInRegion);
        
    if (VDP_GetScalerPixelBased() == 0)
    {
        //transfer output Region from (0~10000) based to pixel-based
        if (CalculateOutputRegion(bPath, &rInRegion, &rOutRegion) != SV_SUCCESS)
        {
            return (SV_FAIL);
        }
        
        wXOff = (UINT16)rOutRegion.u4X;
        wYOff = (UINT16)rOutRegion.u4Y;
        wWidth = (UINT16)rOutRegion.u4Width;
        wHeight = (UINT16)rOutRegion.u4Height;
        //printf("1,bVideoUpdateDispRegion-->wWidth=%d---------zq\n",wWidth);  
    }
    else
    {
        //make output width to be even pixel
        if((wWidth % 2) && !bIsScalerInput444(bPath))
        {
            wWidth -= 1;
            //printf("2,bVideoUpdateDispRegion-->wWidth=%d---------zq\n",wWidth);        
        }
     }

    //if nothing changed, just return
    if((prDispInfo->wXOffset == wXOff) && 
       (prDispInfo->wYOffset == wYOff) &&
       (prDispInfo->wWidth == wWidth) && 
       (prDispInfo->wHeight == wHeight))
    {
        return (SV_SUCCESS);
    }

    prDispInfo->wXOffset = wXOff;
    prDispInfo->wYOffset = wYOff;
    prDispInfo->wWidth = wWidth;
    prDispInfo->wHeight = wHeight;

    rOutputRegion.u4X = (UINT32)wXOff;
    rOutputRegion.u4Y = (UINT32)wYOff;
    rOutputRegion.u4Width = (UINT32)wWidth;
    rOutputRegion.u4Height = (UINT32)wHeight;
    #if 1//def CC_SCPOS_SOURCE_CHANGE_NOTIFY
    LOG(4,"bVideoUpdateDispRegion and call VDP_DrvOutputRangeChgNotify \n");
    VDP_DrvOutputRangeChgNotify(bPath,rOutputRegion);
    #endif
    // Set Flag to inform SCPOS
    vScpipReconfig(getScalerVDP(bPath));    
    
    return (SV_SUCCESS);
}

/**
 *  @6896 review done
 */
UINT8 bVideoUpdateSrcRegion(UINT8 bPath)
{
    UINT8 u1Interlace;
    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT16 u2HPorch;
    UINT16 u2VPorch;
    UINT32 u4XOff, u4YOff, u4Width, u4Height;        
    RPicInfo *prPicInfo;
    VDP_DISP_REGION_T rOutRegion;
    VDP_REGION_T rSourceRegion;
    #ifdef IS_SUPPORT_3D_PHOTO
    TDTV_3D_CROP_INTO_T* eCropInfo;
    UINT32 u4PanelWidth, u4PanelHeight;
    eCropInfo = ScpipGet3DCropInfo();
    u4PanelWidth = PANEL_GetPanelWidth();
    u4PanelHeight = PANEL_GetPanelHeight();
    #endif
	UINT8 bMode = 0;	 

    prPicInfo = getPicInfo(bPath);

    if (bPath == SV_VP_MAIN) 
    {
        if ((VDP_GetScalerPixelBased() ==0) && (fgIsMainDec() == SV_VD_NA) ) 
        {
            return (SV_SUCCESS);
        }
    } 
    else 
    {
        if ((VDP_GetScalerPixelBased() ==0) && (fgIsPipDec() == SV_VD_NA) ) 
        {
            return (SV_SUCCESS);
        }
    }

    LOG(3, "bVideoUpdateSrcRegion(%d): SRC(%d, %d, %d, %d), OVERSCAN(%d, %d, %d, %d)\n", 
    	bPath, prPicInfo->rSrcRegion.u4X, prPicInfo->rSrcRegion.u4Y, prPicInfo->rSrcRegion.u4Width, prPicInfo->rSrcRegion.u4Height, 
    	prPicInfo->rOverScan.u4Top, prPicInfo->rOverScan.u4Bottom, prPicInfo->rOverScan.u4Left, prPicInfo->rOverScan.u4Right);

    if (VDP_GetScalerPixelBased() == 0)        
    {
        //Check data range
        if ((prPicInfo->rSrcRegion.u4Width == 0) || (prPicInfo->rSrcRegion.u4Height == 0)) 
        {
            return (SV_FAIL);
        }
        //Check data range    
        if (((prPicInfo->rSrcRegion.u4X + prPicInfo->rSrcRegion.u4Width) > VDP_MAX_REGION_WIDTH) || 
        	((prPicInfo->rSrcRegion.u4Y + prPicInfo->rSrcRegion.u4Height) > VDP_MAX_REGION_HEIGHT))
        {
            return (SV_FAIL);
        }
        //Convert width, height into pixel based after overscan & source region. (Output stored in rOutRegion)
        if (CalculateInputRegion(bPath, &(prPicInfo->rOverScan), &(prPicInfo->rSrcRegion), &rOutRegion) != SV_SUCCESS)
        {
            return (SV_FAIL);
        }

        #if (defined(CC_SRM_ON) && defined(CC_DTV_HD_DOT_BY_DOT) &&  !defined(CC_SCPOS_DISPMODE_NOT_SUPPORT))
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SRC_RATIO + (UINT32)bPath)
            , prPicInfo->rSrcRegion.u4Width -prPicInfo->rOverScan.u4Left-prPicInfo->rOverScan.u4Right
            , prPicInfo->rSrcRegion.u4Height -prPicInfo->rOverScan.u4Top-prPicInfo->rOverScan.u4Bottom);
        #endif

        u4XOff = rOutRegion.u4X;
        u4YOff = rOutRegion.u4Y;
        u4Width = rOutRegion.u4Width;
        u4Height = rOutRegion.u4Height;
    }
    else    //pixel-based
    {
		 //copy from not pixel base 
		//when 422 input ,the prOut->u4X need to change to even,because H boudary only can protect even pixels, and
		//hp_ofst only can be set even value.
		bMode = u1ScpipGetDispMode(bPath);  

	    if((prPicInfo->rSrcRegion.u4Height %2)&& (bMode == VDP_SCPOS_PSCAN_DISPMODE)  && (bDrvVideoIsSrcInterlace(bPath)))
	    {
	        prPicInfo->rSrcRegion.u4Height += 1;
	    }

		if((prPicInfo->rSrcRegion.u4Width % 2) && !bIsScalerInput444(bPath))
  		{
	        if(prPicInfo->rSrcRegion.u4X ==0)
	        {
	            prPicInfo->rSrcRegion.u4Width -= 1;
	        }
	        else
	        {
	            prPicInfo->rSrcRegion.u4X -=1;
	            prPicInfo->rSrcRegion.u4Width += 1;
	        }
    	}

	    if((prPicInfo->rSrcRegion.u4Y %2)&& (bMode == VDP_SCPOS_PSCAN_DISPMODE)  && (bDrvVideoIsSrcInterlace(bPath)))
	    {
	        prPicInfo->rSrcRegion.u4Y += 1;
	        if(prPicInfo->rSrcRegion.u4Height % 2)
	        {
	            prPicInfo->rSrcRegion.u4Height -= 1;
	        }
	    }
		
        u4XOff = prPicInfo->rSrcRegion.u4X;
        u4YOff = prPicInfo->rSrcRegion.u4Y;
        u4Width = prPicInfo->rSrcRegion.u4Width;
        u4Height = prPicInfo->rSrcRegion.u4Height;    
    }

    /*until here, u4XOff,u4YOff,u4Width,u4Height have become pixel-based*/

    prPicInfo->wSrcW = wDrvVideoInputWidth(bPath);
    prPicInfo->wSrcH = wDrvVideoInputHeight(bPath);
    u1Interlace = bDrvVideoIsSrcInterlace(bPath);    
    u2HPorch = wDrvVideoGetHPorch(bPath);
    u2VPorch = wDrvVideoGetVPorch(bPath);        
	//update HTotal & VTotal
    u2HTotal = wDrvVideoGetHTotal(bPath);
    u2VTotal= wDrvVideoGetVTotal(bPath);

    // check if pixel-based source region is valid
    if (VDP_GetScalerPixelBased() == 0)
    {
        if (((u4XOff + u4Width) > prPicInfo->wSrcW) || ((u4YOff + u4Height) > prPicInfo->wSrcH)) 
        {
            LOG(1, "bVideoUpdateSrcRegion(%d): Invalid src region(%d, %d, %d, %d)\n", u4XOff, u4YOff, u4Width, u4Height);
            return (SV_FAIL);
        }
    }
    else
    {
        if (bDrvVideoSignalStatus(bPath) == SV_VDO_STABLE)
        {    
            // William, Pixel Based Protection
            if (u4XOff > prPicInfo->wSrcW)
            {
                u4XOff = 0;
            }
            if ((u4XOff + u4Width) > prPicInfo->wSrcW)
            {
                u4Width = (prPicInfo->wSrcW - u4XOff);
            }
        
            if (u4YOff > prPicInfo->wSrcH)
            {
                u4YOff = 0;
            }
    
            if ((u4YOff + u4Height) > prPicInfo->wSrcH)
            {
                u4Height = (prPicInfo->wSrcH - u4YOff);
            }
        }
    }

    // handle non-standard signal
    {
        UINT32 u4NTDOfst;
        u4NTDOfst = u4DrvVideoGetNSDOffset(bPath);
        if (u4NTDOfst != 0x800)
        {
            if (((u4Height + u4NTDOfst) > 0x800) && (u4NTDOfst < 0x800))
            {
                LOG(4, "Non-Standard Signal Detected (%d)! Ofst: %d\n", bPath, u4NTDOfst);
                u4Height = u4Height + u4NTDOfst - 0x800;                
            }
        }
    }
#ifdef IS_SUPPORT_3D_PHOTO
        if(_fgFixRegion)
        {
            u4XOff = 0;
            u4YOff = 0;
            u4Width = u4PanelWidth - (eCropInfo->u4CropLeftOffset + eCropInfo->u4CropRightOffset);
            u4Height = u4PanelHeight - (eCropInfo->u4CropTopOffset + eCropInfo->u4CropBottomOffset);
            prPicInfo->wSrcW = u4Width;
            prPicInfo->wSrcH = u4Height;
        }
            
#endif
    if ((prPicInfo->u1Interlace == u1Interlace) &&
	    (prPicInfo->wHTotal == u2HTotal) &&
    	(prPicInfo->wVTotal == u2VTotal) &&
        (prPicInfo->wHPorch == u2HPorch) &&
        (prPicInfo->wVPorch == u2VPorch) &&
        (prPicInfo->wXOffset == u4XOff) &&
        (prPicInfo->wYOffset == u4YOff) &&
        (prPicInfo->wTargetW == u4Width) &&
        (prPicInfo->wTargetH == u4Height))        
    {
        return (SV_SUCCESS);
    }
    
    prPicInfo->u1Interlace = u1Interlace;
	prPicInfo->wHTotal = u2HTotal;
	prPicInfo->wVTotal = u2VTotal;
    prPicInfo->wHPorch = u2HPorch;
    prPicInfo->wVPorch = u2VPorch;    
    prPicInfo->wXOffset = u4XOff;
    prPicInfo->wYOffset = u4YOff;
    prPicInfo->wTargetW = u4Width;
    prPicInfo->wTargetH = u4Height;

    rSourceRegion.u4X = u4XOff;
    rSourceRegion.u4Y = u4YOff;
    rSourceRegion.u4Width = u4Width;
    rSourceRegion.u4Height = u4Height;
    
    #if 1//def CC_SCPOS_SOURCE_CHANGE_NOTIFY
    LOG(4,"VDP_DriverSourceChgNotify: Driver Source Change \n");
    VDP_DrvSrcRangeChgNotify(bPath,rSourceRegion);
    #endif
    // Set Flag to inform SCPOS
    vScpipReconfig(getScalerVDP(bPath));

    return (SV_SUCCESS);
}
UINT8 bApiVideoSetDispRegion(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight)
{      
    return bVideoUpdateDispRegion(bPath, wXOff, wYOff, wWidth, wHeight);
}


/**
 * @brief bApiVideoSetSrcOverScan
 * Set overscan of the source video window
 */
UINT8 bApiVideoSetSrcOverScan(UINT8 bPath, UINT16 wTop, UINT16 wBottom, UINT16 wLeft, UINT16 wRight) 
{
    RPicInfo *prPicInfo;

    if (VDP_GetScalerPixelBased() != 0)
    {
        return (SV_FAIL);
    }

    if (((wTop + wBottom) > VDP_MAX_REGION_HEIGHT) ||
        ((wLeft + wRight) > VDP_MAX_REGION_WIDTH))
    {
        return (SV_FAIL);
    }

    if (bPath == SV_VP_MAIN) 
    {
        prPicInfo = &_rMPicInfo;
		LboxSourceChange();
    } 
    else 
    {
        prPicInfo = &_rPPicInfo;
    }

    prPicInfo->rOverScan.u4Top = wTop;
    prPicInfo->rOverScan.u4Bottom = wBottom;
    prPicInfo->rOverScan.u4Left = wLeft;
    prPicInfo->rOverScan.u4Right = wRight;

    return bVideoUpdateSrcRegion(bPath);
}

/**
 * @brief bApiVideoSetSrcRegion
 
 * Set position and size of the source video window
 
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         wXOff: the horizontal position of the source video window 
 *         wYOff: the vertical position of the source video window
 *         wWidth: the width of the source video window 
 *         wHeight: the height of the source video window
 * @retval SV_SUCCESS/SV_FAIL
 * @6896 review done
 */
UINT8 bApiVideoSetSrcRegion(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight) 
{
    RPicInfo *prPicInfo;
    VDP_DISP_REGION_T rInRegion;

    LOG(4, "bApiVideoSetSrcRegion(%d, %d, %d, %d, %d)\n", (UINT32)bPath, (UINT32)wXOff, (UINT32)wYOff, (UINT32)wWidth, (UINT32)wHeight);

    // Tadd@090203, always save input even the source is not selected(for blank) 
    // Luis@060606, for middleware driver test:
    // save input size configuration which will be used by
    // vApiVideoInputChangeNotify even when there is no active source.
    rInRegion.u4X = wXOff;
    rInRegion.u4Y = wYOff;
    rInRegion.u4Width = wWidth;
    rInRegion.u4Height = wHeight;

    //keep source region in picInfo
    if (bPath == SV_VP_MAIN) 
    {
        prPicInfo = &_rMPicInfo;
        prPicInfo->rSrcRegion = rInRegion;
        if(!(VDP_GetScalerPixelBased() ||fgIsMainDec()) ) 
        {
            return (SV_SUCCESS);
        }
    } 
    else 
    {
        prPicInfo = &_rPPicInfo;
        prPicInfo->rSrcRegion = rInRegion;
        if(!(VDP_GetScalerPixelBased() ||fgIsPipDec()) ) 
        {
            return (SV_SUCCESS);
        }
    }
    
    return (bVideoUpdateSrcRegion(bPath));
}

//#if defined(SCPIP_SUPPORT_POST_SCALER)
void vFscSetOutWidthLimit(UINT16 u2LimitWidth)
{
	u2FscOutWidthLimit = u2LimitWidth;
}

UINT16 u2FscGetMaxOutWidth(void)
{
	if(TV_WIDTH > u2FscOutWidthLimit)
	{
		return u2FscOutWidthLimit;
	}
	else
	{
		return TV_WIDTH;
	}
}
//#endif



