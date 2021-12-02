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
 * $RCSfile: io_papi_mixer_vmxvl.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_mixer_vmxvl.c
 *  PAPI kernel driver implementation: mixer
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>
#include <linux/cb_data.h>
#include "papi_papionly.h"
#include "papi_types.h"

#include "video_def.h"
#include "vdo_misc.h"
#include "vdp_if.h"
#include "panel.h"
#include "sv_const.h"
#include "pmx_drvif.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define UINT_NONLINEAR_END_FACTOR (1024)
#define UNIT_NONLINEAR                      (0xA)
#define MIN_MIDSCALE_PANORAMA (70)
#define MAX_MIDSCALE_PANORAMA (120)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { return TM_ERR_BAD_PARAMETER; }      \
          } while (0)

#define USR_SPACE_ACCESS_VALIDATE(type, arg, size)                 \
    if (!access_ok(type, (void __user *)(arg), size))                            \
    {                                                       \
        printk("papi_fe_mixer_vmxvl argument error\n");     \
        return TM_ERR_BAD_PARAMETER;                                          \
    }  

#define COPY_FROM_USER(dst, src, size)              \
    if (copy_from_user(dst, (void __user *)(src), size))                        \
    {                                                       \
        printk("papi_fe_mixer_vmxvl argument copy error\n");      \
        return TM_ERR_BAD_PARAMETER;                                          \
    }                                                        

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return TM_ERR_BAD_PARAMETER;\
    }

#define VERIFY_VDP_ID(id)                                   \
          do {                                              \
            if (id >= VDP_MAX_NS) { return TM_ERR_BAD_PARAMETER; }     \
          } while (0)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VMixRect _arSrcRegion[VDP_MAX_NS] = { {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}; 
static VMixRect _arDstRegion[VDP_MAX_NS] = { {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}; 
static VMixRect _arLayerRegion[VDP_MAX_NS] = { {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}; 
static int _ai4ScaleMode[VDP_MAX_NS] = {0, 0};
static VDP_AUTO_NONLINEAR_PRM_T _arNonlinearPrm[VDP_MAX_NS] = {{0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR}, {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR}};
static VMixColor _arBgColor[VDP_MAX_NS] = {{0, 0, 0}, {0, 0, 0}};
static INT16 _ai2Alpha[VDP_MAX_NS] = {255, 255};
static Bool _au4WaitCallback[VDP_MAX_NS] = {FALSE, FALSE};
static Nat32 _au4MidScaleRatio[VDP_MAX_NS] = {100, 100};
	
//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
void PAPIMIXERNotifyNewSettingComplete(UINT32 u4VdpId);
void PAPIMIXERNotifyNewSettingInterrupted(UINT32 u4VdpId);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**  @ingroup mixer_functions */
/**  @brief Hide or show this layer. */
/**  @param layerNr Video layer number. */
/**  @param hide True to hide this layer, false to show this layer. */
static INT32 _papi_mixer_vmxvl_Hide(Nat32 layerNr, Bool hide)
{
    VERIFY_VDP_ID(layerNr);

    PMX_SetPlaneHide(layerNr,hide);
	/*
    if (hide)
    {
//        INT16 i2Value;
        	
//        if (VDP_GetArg(layerNr, VDP_ARG_BLENDING, &i2Value) == VDP_SET_OK)
        {
//            _ai2Alpha[layerNr] = i2Value;  
            _ai2Alpha[layerNr] = 255;  //hack to 255
    
            if (VDP_SetArg(layerNr, VDP_ARG_BLENDING, 0) == VDP_SET_OK)
            {
                return TM_OK;
            }
        }
    }    
    else
    {
            if (VDP_SetArg(layerNr, VDP_ARG_BLENDING, _ai2Alpha[layerNr]) == VDP_SET_OK)
            {
                return TM_OK;
            }
    }
    */
    
    return TM_ERR_BAD_PARAMETER;
}

/**  @ingroup mixer_functions */
/**  @brief Set the background color for this layer. */
/**  @param layerNr Video layer number. */
/**  @param bgColor The background color that has to be set. */
static INT32 _papi_mixer_vmxvl_SetBgColor(Nat32 layerNr, VMixColor *pBgColor)
{
    // Note: we can only set te pmx bg color instead
    VERIFY_VDP_ID(layerNr);

    USR_SPACE_ACCESS_VALIDATE(VERIFY_READ, pBgColor, sizeof(VMixColor));
    COPY_FROM_USER(&_arBgColor[layerNr], pBgColor, sizeof(VMixColor));
    
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Sets the coordinates of the destination window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). */
/**  @param layerNr Video layer number. */
/**  @param dstRect Coordinates of the destination window relative to the layer window. */
static INT32 _papi_mixer_vmxvl_SetDstWindow( Nat32 layerNr, VMixRect *pDstRect )
{
    VERIFY_VDP_ID(layerNr);

    USR_SPACE_ACCESS_VALIDATE(VERIFY_READ, pDstRect, sizeof(VMixRect));
    COPY_FROM_USER(&_arDstRegion[layerNr], pDstRect, sizeof(VMixRect));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns the scale modes supported by this layer. */
/**  @param layerNr Video layer number. */
/**  @param pScaleModes Return value with the OR-ed supported scale modes. See datatype \ref tmIVmixVidLayer_ScaleMode. */
static INT32 _papi_mixer_vmxvl_GetSuppScaleModes( Nat32 layerNr, int * pScaleModes )
{
    int i4ScaleModes;

    i4ScaleModes = papi_papionly_vmixvid_ScaleModeNoScaling | papi_papionly_vmixvid_ScaleModeLinear | papi_papionly_vmixvid_ScaleModePanorama;
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pScaleModes, sizeof(int));
    COPY_TO_USER(pScaleModes, &i4ScaleModes, sizeof(int));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the aspect ratio of the input of this layer in the format numerator:denominator. If there is no information about the aspect ratio then numerator=0 and denominator=1. */
/**  @param layerNr Video layer number. */
/**  @param pNumerator Output parameter with the aspect ratio of the input, numerator part. */
/**  @param pDenominator Output parameter with the aspect ratio of the input, denominator part. */
static INT32 _papi_mixer_vmxvl_GetInputAspectRatio( Nat32 layerNr, Nat16 * pNumerator, Nat16 * pDenominator )
{
    UINT32 u4Width, u4Height;
    UINT16 u2Num, u2Den;
    UINT32 i;
    UINT32 u4Update;
    UINT32 u4TmpWidth, u4TmpHeight;
    UINT32 u4Factor[6] = {2, 3, 5, 7, 11, 13};

    VERIFY_VDP_ID(layerNr);
    VERIFY_NULL(pNumerator);
    VERIFY_NULL(pDenominator);

    u4Width = wApiVideoGetInputWidth(layerNr);
    u4Height = wDrvVideoInputHeight(layerNr);

    if ((u4Width == 0) || (u4Height == 0))
    {
        u2Num = 0;
        u2Den = 1;
    }
    else      // Get highest common factor
    {
        do
        {
            u4Update = 0;
            for (i = 0; i < 6; i++)
            {
                u4TmpWidth = u4Width/u4Factor[i];
                u4TmpHeight = u4Height/u4Factor[i];
                
                if (((u4TmpWidth*u4Factor[i]) == u4Width) && ((u4TmpHeight*u4Factor[i]) == u4Height))
                {
                    u4Width = u4TmpWidth;
                    u4Height = u4TmpHeight;
                    u4Update = 1;
                    break;
                }
            }
    
        } while (u4Update);

        u2Num = (UINT16)u4Width;
        u2Den = (UINT16)u4Height;
    }

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pNumerator, sizeof(Nat16));
    COPY_TO_USER(pNumerator, &u2Num, sizeof(Nat16));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pDenominator, sizeof(Nat16));
    COPY_TO_USER(pDenominator, &u2Den, sizeof(Nat16));    

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the minimum and maximum value for the mid scale ratio for panoramic scaling. */
/**  @param layerNr Video layer number. */
/**  @param pMinMidScaleRatio Minimum scale ratio in percentages. */
/**  @param pMaxMidScaleRatio Maximum scale ratio in percentages. */
static INT32 _papi_mixer_vmxvl_GetPanoramaMidScaleRange( Nat32 layerNr, Nat32 * pMinMidScaleRatio, Nat32 * pMaxMidScaleRatio )
{
    Nat32 u4Min, u4Max;
    VERIFY_VDP_ID(layerNr);
    VERIFY_NULL(pMinMidScaleRatio);
    VERIFY_NULL(pMaxMidScaleRatio);

    
    if (layerNr != VDP_1)
    {
        // not support 
        u4Min = 100;
        u4Max = 100;
    }
    else
    {
        INT32 i4DstHeight;
        INT32 i4SrcHeight;

        i4DstHeight = _arDstRegion[layerNr].lr.y - _arDstRegion[layerNr].ul.y;
        i4SrcHeight = _arSrcRegion[layerNr].lr.y - _arSrcRegion[layerNr].ul.y;

        if ((i4DstHeight <= 0) || (i4SrcHeight <= 0))
        {
            // not support 
            u4Min = 100;
            u4Max = 100;
        }
        else
        {
            // TBD
            // return fixed code directly
            //u4Min = i4DstHeight*100/i4SrcHeight - 5;
            //u4Max = i4DstHeight*100/i4SrcHeight + 5;
            u4Min = MIN_MIDSCALE_PANORAMA;
            u4Max = MAX_MIDSCALE_PANORAMA;
        }
    }

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMinMidScaleRatio, sizeof(Nat32));
    COPY_TO_USER(pMinMidScaleRatio, &u4Min, sizeof(Nat32));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMaxMidScaleRatio, sizeof(Nat32));
    COPY_TO_USER(pMaxMidScaleRatio, &u4Max, sizeof(Nat32));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the coordinates of the destination window on this layer. */
/**  @param layerNr Video layer number. */
/**  @param pDstRect Return value with the coordinates of the destination window. */
static INT32 _papi_mixer_vmxvl_GetDstWindow(Nat32 layerNr, VMixRect * pDstRect)
{
    VERIFY_VDP_ID(layerNr);  
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pDstRect, sizeof(VMixRect));

    COPY_TO_USER(pDstRect, &_arDstRegion[layerNr], sizeof(VMixRect));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns whether this layer is hidden or shown. */
/**  @param layerNr Video layer number. */
/**  @param pHidden Return value is true if this layer is hidden, false if this layer is shown. */
static INT32 _papi_mixer_vmxvl_GetHidden( Nat32 layerNr, Bool * pHidden )
{
    INT16 i2Value;
    Bool u4Hidden;
    VERIFY_VDP_ID(layerNr);  
    VERIFY_NULL(pHidden);

    PMX_GetPlaneHide(layerNr,&u4Hidden);
    /*    	
    if (VDP_GetArg(layerNr, VDP_ARG_BLENDING, &i2Value) != VDP_SET_OK)
    {
        return TM_ERR_BAD_PARAMETER;
    }

    if (i2Value == 0)
    {
        u4Hidden = TRUE;
    }
    else
    {
        u4Hidden = FALSE;
    }
    */
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pHidden, sizeof(Bool));
    COPY_TO_USER(pHidden, &u4Hidden, sizeof(Bool));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns the maximum window width and height for this layer. */
/**  @param layerNr Video layer number. */
/**  @param pMaxWidth Returns the maximum window width. */
/**  @param pMaxHeight Returns the maximum window height. */
static INT32 _papi_mixer_vmxvl_GetMaxLayerWindowSize( Nat32 layerNr, Nat32 * pMaxWidth, Nat32 * pMaxHeight )
{
    UINT16 u2PanelWidth, u2PanelHeight;
    Nat32 u4MaxWidth, u4MaxHeight;

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMaxWidth, sizeof(Nat32));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMaxHeight, sizeof(Nat32));

    u2PanelWidth = PANEL_GetPanelWidth();
    u2PanelHeight = PANEL_GetPanelHeight();

    if (layerNr == VDP_1)
    {
        u4MaxWidth = (Nat32)u2PanelWidth;
    }
    else if (layerNr == VDP_2)
    {
        u4MaxWidth = (Nat32)u2PanelWidth >> 1;
    }
    u4MaxHeight = (Nat32)u2PanelHeight;

    COPY_TO_USER(pMaxWidth, &u4MaxWidth, sizeof(Nat32));
    COPY_TO_USER(pMaxHeight, &u4MaxHeight, sizeof(Nat32));
      
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the coordinates of the source window on this layer. */
/**  @param layerNr Video layer number. */
/**  @param pSrcRect Return value with the coordinates of the source window. */
static INT32 _papi_mixer_vmxvl_GetSrcWindow( Nat32 layerNr, VMixRect * pSrcRect )
{
    VERIFY_VDP_ID(layerNr);  

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pSrcRect, sizeof(VMixRect));
    
    COPY_TO_USER(pSrcRect, &_arSrcRegion[layerNr], sizeof(VMixRect));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the current scale mode. */
/**  @param layerNr Video layer number. */
/**  @param pScaleMode Return value with the current scale mode. See datatype \ref tmIVmixVidLayer_ScaleMode. */
static INT32 _papi_mixer_vmxvl_GetScaleMode( Nat32 layerNr, int * pScaleMode )
{
    VERIFY_VDP_ID(layerNr);  
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pScaleMode, sizeof(int));
    COPY_TO_USER(pScaleMode, &_ai4ScaleMode[layerNr], sizeof(int));
            
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the active image area of the input of this layer (excluding black bars). If there is no information about the active image area, then (0,0,0,0) is returned. */
/**  @param layerNr Video layer number. */
/**  @param pActiveImageArea Output parameter with the active image area. */
static INT32 _papi_mixer_vmxvl_GetInputActiveImageArea( Nat32 layerNr, VMixRect * pActiveImageArea )
{  
    UINT16 x,y,w,h;
    VDP_REGION_T rSrcRegion;

    VERIFY_VDP_ID(layerNr);  
    VERIFY_NULL(pActiveImageArea);

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pActiveImageArea, sizeof(VMixRect));
    VDP_GetActiveImageArea(&x, &y, &w, &h); 

    rSrcRegion.u4X = (UINT32)x;
    rSrcRegion.u4Y = (UINT32)y;
    rSrcRegion.u4Width = (UINT32)w;
    rSrcRegion.u4Height = (UINT32)h;

    COPY_TO_USER(pActiveImageArea, &rSrcRegion, sizeof(VMixRect));
    
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns the minimum window width and height for this layer. */
/**  @param layerNr Video layer number. */
/**  @param pMinWidth Returns the minimum window width. */
/**  @param pMinHeight Returns the minimum window height. */
static INT32 _papi_mixer_vmxvl_GetMinWindowSize( Nat32 layerNr, Nat32 * pMinWidth, Nat32 * pMinHeight )
{
    Nat32 u4MinWidth, u4MinHeight;
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMinWidth, sizeof(Nat32));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMinHeight, sizeof(Nat32));

    u4MinWidth = 0;
    u4MinHeight = 0;

    COPY_TO_USER(pMinWidth, &u4MinWidth, sizeof(Nat32));
    COPY_TO_USER(pMinHeight, &u4MinHeight, sizeof(Nat32));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Sets the coordinates of the source window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). Overscan will be controlled by the middleware and application layer by setting the src window. The platform should not clip any overscan itself without src window being set. */
/**  @param layerNr Video layer number. */
/**  @param srcRect Coordinates of the source window relative to the input window. */
static INT32 _papi_mixer_vmxvl_SetSrcWindow( Nat32 layerNr, VMixRect *pSrcRect )
{
    VERIFY_VDP_ID(layerNr);  

    USR_SPACE_ACCESS_VALIDATE(VERIFY_READ, pSrcRect, sizeof(VMixRect));
    COPY_FROM_USER(&_arSrcRegion[layerNr], pSrcRect, sizeof(VMixRect));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns whether a background color is supported for this layer or not. */
/**  @param layerNr Video layer number. */
/**  @param pBgSupp True if background color is supported, false otherwise. */
static INT32 _papi_mixer_vmxvl_GetBgSupp( Nat32 layerNr, Bool * pBgSupp )
{
    Bool u4Supp;

    u4Supp = FALSE;

    USR_SPACE_ACCESS_VALIDATE(VERIFY_READ, pBgSupp, sizeof(Bool));
    COPY_TO_USER(pBgSupp, &u4Supp, sizeof(Bool));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Sets the coordinates of the layer window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). */
/**  @param layerNr Video layer number. */
/**  @param layerRect Coordinates of the layer window relative to the output window. */
static INT32 _papi_mixer_vmxvl_SetLayerWindow( Nat32 layerNr, VMixRect *pLayerRect )
{
    VERIFY_VDP_ID(layerNr);

    USR_SPACE_ACCESS_VALIDATE(VERIFY_READ, pLayerRect, sizeof(VMixRect));
    COPY_FROM_USER(&_arLayerRegion[layerNr], pLayerRect, sizeof(VMixRect));

    // In FB Stage, use dst region
    memcpy(_arDstRegion, _arLayerRegion, sizeof(VMixRect));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns for a particular scaling mode the minimum and maximum scale factors in percentages. */
/**  @param layerNr Video layer number. */
/**  @param scaleMode The specified scaling mode. See datatype \ref tmIVmixVidLayer_ScaleMode. */
/**  @param pMinWidthPercentage The minimal horizontal scale factor. */
/**  @param pMinHeightPercentage The minimal vertical scale factor. */
/**  @param pMaxWidthPercentage The maximum horizontal scale factor. */
/**  @param pMaxHeightPercentage The maximum vertical scale factor. */
static INT32 _papi_mixer_vmxvl_GetScaleRange( Nat32 layerNr, int scaleMode, Nat32 * pMinWidthPercentage, Nat32 * pMinHeightPercentage, Nat32 * pMaxWidthPercentage, Nat32 * pMaxHeightPercentage )
{
    UINT32 u4MinWidthPercentage;
    UINT32 u4MinHeightPercentage;
    UINT32 u4MaxWidthPercentage;
    UINT32 u4MaxHeightPercentage;
    UINT32 u4InputWidth, u4InputHeight;
    UINT16 u2PanelWidth, u2PanelHeight;
    Nat32 u4MaxWidth, u4MaxHeight;

    VERIFY_VDP_ID(layerNr);  
    VERIFY_NULL(pMinWidthPercentage);
    VERIFY_NULL(pMinHeightPercentage);
    VERIFY_NULL(pMaxWidthPercentage);
    VERIFY_NULL(pMaxHeightPercentage);

    u2PanelWidth = PANEL_GetPanelWidth();
    u2PanelHeight = PANEL_GetPanelHeight();

    if (layerNr == VDP_1)
    {
        u4MaxWidth = (Nat32)u2PanelWidth;
    }
    else if (layerNr == VDP_2)
    {
        u4MaxWidth = (Nat32)u2PanelWidth >> 1;
    }
    u4MaxHeight = (Nat32)u2PanelHeight;
    
    u4InputWidth = wApiVideoGetInputWidth(layerNr);
    u4InputHeight = wDrvVideoInputHeight(layerNr);

    switch (scaleMode)
    {
        case papi_papionly_vmixvid_ScaleModeLinear:
              u4MinWidthPercentage = 100/32;
              if (u4InputWidth == 0)
              {
                  u4MaxWidthPercentage = 100;
              }
              else
              {
                  u4MaxWidthPercentage = u4MaxWidth*100/u4InputWidth;
              }
              u4MinHeightPercentage = 100/32;
              if (u4InputHeight == 0)
              {
                  u4MaxHeightPercentage = 100;
              }
              else
              {
                  u4MaxHeightPercentage = u4MaxWidth*100/u4InputHeight;
              }
        	break;
        	
        case papi_papionly_vmixvid_ScaleModePanorama:
        	// Full screen
              if (u4InputWidth == 0)
              {
                  u4MinWidthPercentage = 100;
                  u4MaxWidthPercentage = 100;
              }
              else
              {
                  u4MinWidthPercentage = u2PanelWidth*100/u4InputWidth;
                  u4MaxWidthPercentage = u2PanelWidth*100/u4InputWidth;
              }
              u4MinHeightPercentage = 100/32;
              if (u4InputHeight == 0)
              {
                  u4MaxHeightPercentage = 100;
              }
              else
              {
                  u4MaxHeightPercentage = u4MaxWidth*100/u4InputHeight;
              }
              break;
        	
        case papi_papionly_vmixvid_ScaleModeHorizontal:
              u4MinWidthPercentage = 100/32;
              if (u4InputWidth == 0)
              {
                  u4MaxWidthPercentage = 100;
              }
              else
              {
                  u4MaxWidthPercentage = u4MaxWidth*100/u4InputWidth;              	
              }
              u4MinHeightPercentage = 100;
              u4MaxHeightPercentage = 100;
              break;

        case papi_papionly_vmixvid_ScaleModeNoScaling:
        default:
        	u4MinWidthPercentage = 100;
        	u4MinHeightPercentage = 100;
        	u4MaxWidthPercentage = 100;
        	u4MaxHeightPercentage = 100;
        	break;
    }
   

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMinWidthPercentage, sizeof(Nat32));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMinHeightPercentage, sizeof(Nat32));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMaxWidthPercentage, sizeof(Nat32));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMaxHeightPercentage, sizeof(Nat32));
    COPY_TO_USER(pMinWidthPercentage, &u4MinWidthPercentage, sizeof(Nat32));
    COPY_TO_USER(pMinHeightPercentage, &u4MinHeightPercentage, sizeof(Nat32));
    COPY_TO_USER(pMaxWidthPercentage, &u4MaxWidthPercentage, sizeof(Nat32));
    COPY_TO_USER(pMaxHeightPercentage, &u4MaxHeightPercentage, sizeof(Nat32));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the coordinates of the layer window on this layer. */
/**  @param layerNr Video layer number. */
/**  @param pLayerRect Return value with the coordinates of the layer window. */
static INT32 _papi_mixer_vmxvl_GetLayerWindow( Nat32 layerNr, VMixRect * pLayerRect )
{
    VERIFY_VDP_ID(layerNr);  
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pLayerRect, sizeof(VMixRect));

    COPY_TO_USER(pLayerRect, &_arLayerRegion[layerNr], sizeof(VMixRect));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Set the mid scale ratio for panoramic scaling. */
/**  @param layerNr Video layer number. */
/**  @param midScaleRatio Mid scale ratio for panoramic scaling. */
static INT32 _papi_mixer_vmxvl_SetPanoramaMidScale( Nat32 layerNr, Nat32 midScaleRatio )
{
    VERIFY_VDP_ID(layerNr);  
    _au4MidScaleRatio[layerNr] = midScaleRatio;
    
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns the background color for this layer. */
/**  @param layerNr Video layer number. */
/**  @param pBgColor Return value with the background color for this layer. */
static INT32 _papi_mixer_vmxvl_GetBgColor( Nat32 layerNr, VMixColor * pBgColor )
{
    VERIFY_VDP_ID(layerNr);

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pBgColor, sizeof(VMixColor));
    COPY_TO_USER(pBgColor, &_arBgColor[layerNr], sizeof(VMixColor));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Select a particular scale mode */
/**  @param layerNr Video layer number. */
/**  @param scaleMode Scale mode that is set. See datatype \ref tmIVmixVidLayer_ScaleMode. */
static INT32 _papi_mixer_vmxvl_SetScaleMode( Nat32 layerNr, int scaleMode )
{
    VERIFY_VDP_ID(layerNr);  

    _ai4ScaleMode[layerNr] = scaleMode;
	
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the input resolution of this layer. */
/**  @param layerNr Video layer number. */
/**  @param pWidth Output parameter with the number of pixels per line. */
/**  @param pHeight Output parameter with the number of lines. */
static INT32 _papi_mixer_vmxvl_GetInputResolution( Nat32 layerNr, Nat32 * pWidth, Nat32 * pHeight )
{
    UINT32 u4Width, u4Height;

    VERIFY_NULL(pWidth);
    VERIFY_NULL(pHeight);

    u4Width = wApiVideoGetInputWidth(layerNr);
    u4Height = wDrvVideoInputHeight(layerNr);

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pWidth, sizeof(Nat32));
    COPY_TO_USER(pWidth, &u4Width, sizeof(Nat32));
    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pHeight, sizeof(Nat32));
    COPY_TO_USER(pHeight, &u4Height, sizeof(Nat32));    

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief The changes specified by SetLayerWindow(), SetDstWindow(), SetSrcWindow(), SetScaleMode(), and SetPanoramaMidScale() are carried out smoothly in the specified amount of time. This is handled asynchronously and the completion of the operation is signalled via a notification function. */
/**  @param layerNr Video layer number. */
/**  @param syncWithNextInput True to wait for the next field/frame, false to start immediately. */
/**  @param nrMilliSeconds Number of milliseconds that the transition should take. */
static INT32 _papi_mixer_vmxvl_ActivateNewWindowSettings(Nat32 layerNr, Bool syncWithNextInput, Nat32 nrMilliSeconds )
{
    // Note: We can only always wait for the next field/frame
    VDP_REGION_T rOutRegion;
    VDP_REGION_T rSrcRegion;
    UINT16 wUiHResolution;

    VERIFY_VDP_ID(layerNr);
    
    rOutRegion.u4X = (UINT32)_arDstRegion[layerNr].ul.x;
    rOutRegion.u4Y = (UINT32)_arDstRegion[layerNr].ul.y;
    rOutRegion.u4Width = (UINT32)(_arDstRegion[layerNr].lr.x -_arDstRegion[layerNr].ul.x);
    rOutRegion.u4Height = (UINT32)(_arDstRegion[layerNr].lr.y -_arDstRegion[layerNr].ul.y);
#if 0
    rOutRegion.u4X = (UINT32)_arLayerRegion[layerNr].ul.x;
    rOutRegion.u4Y = (UINT32)_arLayerRegion[layerNr].ul.y;
    rOutRegion.u4Width = (UINT32)(_arLayerRegion[layerNr].lr.x -_arLayerRegion[layerNr].ul.x);
    rOutRegion.u4Height = (UINT32)(_arLayerRegion[layerNr].lr.y -_arLayerRegion[layerNr].ul.y);
#endif
    rSrcRegion.u4X = (UINT32)_arSrcRegion[layerNr].ul.x;
    rSrcRegion.u4Y = (UINT32)_arSrcRegion[layerNr].ul.y;
    rSrcRegion.u4Width = (UINT32)(_arSrcRegion[layerNr].lr.x -_arSrcRegion[layerNr].ul.x);
    rSrcRegion.u4Height = (UINT32)(_arSrcRegion[layerNr].lr.y -_arSrcRegion[layerNr].ul.y);

    // take care oversample
    wUiHResolution = wApiVideoGetInputWidth(layerNr);
    if(wUiHResolution && ((wDrvVideoInputWidth(layerNr)/wUiHResolution)==2))
    {
        //oversample timing for ypbpr or vga
        rSrcRegion.u4X *= 2;
        rSrcRegion.u4Width *= 2;
    }
    	
    // set dynamic scaling
    if (nrMilliSeconds < 100)
    {
        VDP_SetDynamiceScaler(layerNr, FALSE, nrMilliSeconds);
    }
    else
    {
        VDP_SetDynamiceScaler(layerNr, TRUE, nrMilliSeconds);
    }

    //SetDstWindow()
//    VDP_SetOutRegion(layerNr, FALSE, rOutRegion);

    // FIXME
    // SetLayerWindow()
    VDP_SetOutRegion(layerNr, FALSE, rOutRegion);

    // SetSrcWindow()
    if (rSrcRegion.u4X == 0 && 
	rSrcRegion.u4Y == 0 && 
	rSrcRegion.u4Width== 0 &&
	rSrcRegion.u4Height== 0)
    {
        rSrcRegion.u4Width = wDrvVideoInputWidth(layerNr);
        rSrcRegion.u4Height = wApiVideoGetInputHeight(layerNr);
        VDP_SetSrcRegion(layerNr, FALSE, rSrcRegion);        
    }
    else
    {
        VDP_SetSrcRegion(layerNr, FALSE, rSrcRegion);
    }
    // SetPanoramaMidScale()
    
    if(rSrcRegion.u4Height)
    {
        _arNonlinearPrm[layerNr].u4EndFactor = 0x8000 * 100 * rSrcRegion.u4Width * UINT_NONLINEAR_END_FACTOR / rOutRegion.u4Width / _au4MidScaleRatio[layerNr];
    }
    else
    {
        _arNonlinearPrm[layerNr].u4EndFactor = 0x8000 * 100 * UINT_NONLINEAR_END_FACTOR / _au4MidScaleRatio[layerNr];
    }
    
    VDP_SetNonlinearFactor(layerNr, _arNonlinearPrm[layerNr].u4Slope, _arNonlinearPrm[layerNr].u4EndFactor);
    
    // SetScaleMode()
    VDP_Set121Map(layerNr, (_ai4ScaleMode[layerNr] & papi_papionly_vmixvid_ScaleModeNoScaling));
    VDP_SetNonlinear(layerNr, (_ai4ScaleMode[layerNr] & papi_papionly_vmixvid_ScaleModePanorama));

    if ((nrMilliSeconds < 100) || (_ai4ScaleMode[layerNr] & papi_papionly_vmixvid_ScaleModePanorama))  // not dynamic scaling case
    {
           // callback directly
           if (_au4WaitCallback[layerNr])
           {
               PAPIMIXERNotifyNewSettingInterrupted(layerNr);
           }
           PAPIMIXERNotifyNewSettingComplete(layerNr);
           _au4WaitCallback[layerNr] = FALSE;
    }
    else
    {
          // start to wait for scaling completed
          _au4WaitCallback[layerNr] = TRUE;
    }
    
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Return the current mid scale ratio for panoramic scaling. */
/**  @param layerNr Video layer number. */
/**  @param pMidScaleRatio Return value with the current mid scale ratio for panoramic scaling. */
static INT32 _papi_mixer_vmxvl_GetPanoramaMidScale( Nat32 layerNr, Nat32 * pMidScaleRatio )
{
    VERIFY_VDP_ID(layerNr);

    USR_SPACE_ACCESS_VALIDATE(VERIFY_WRITE, pMidScaleRatio, sizeof(Nat32));
    COPY_TO_USER(pMidScaleRatio, &_au4MidScaleRatio[layerNr], sizeof(Nat32));
	
    return TM_OK;
}

int papi_ioctl_mixer_vmxvl(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;
    IOCTL_6ARG_T rKernelArg;
    Nat32 u4LayerNr;
    
//    _ApiLock()

    // Check access of user space
    USR_SPACE_ACCESS_VALIDATE(VERIFY_READ, arg, _IOC_SIZE(cmd));

    // Copy user space argument
    COPY_FROM_USER(&rKernelArg, arg, _IOC_SIZE(cmd));

    // get layer id
    u4LayerNr = (Nat32)rKernelArg.au4Arg[0];

    switch (cmd)
    {
        case PAPI_IO_MIXER_VMXVL_HIDE:
        {
            Bool u4Hide;
            
            u4Hide = (Bool)rKernelArg.au4Arg[1];
            	
            i4Ret = _papi_mixer_vmxvl_Hide(u4LayerNr, u4Hide);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_SETBGCOLOR:
        {
            i4Ret = _papi_mixer_vmxvl_SetBgColor(u4LayerNr, (VMixColor *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_SETDSTWINDOW:
        {
            i4Ret = _papi_mixer_vmxvl_SetDstWindow(u4LayerNr, (VMixRect *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETSUPPSCALEMODES:
        {
            i4Ret = _papi_mixer_vmxvl_GetSuppScaleModes(u4LayerNr, (int *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETINPUTASPECTRATIO:
        {
            i4Ret = _papi_mixer_vmxvl_GetInputAspectRatio(u4LayerNr, (Nat16 *)rKernelArg.au4Arg[1], (Nat16 *)rKernelArg.au4Arg[2]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETPANORAMAMIDSCALERANGE:
        {
            i4Ret = _papi_mixer_vmxvl_GetPanoramaMidScaleRange(u4LayerNr, (Nat32 *)rKernelArg.au4Arg[1], (Nat32 *)rKernelArg.au4Arg[2]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETDSTWINDOW:
        {
            i4Ret = _papi_mixer_vmxvl_GetDstWindow(u4LayerNr, (VMixRect *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETHIDDEN:
        {
            i4Ret = _papi_mixer_vmxvl_GetHidden(u4LayerNr, (Bool *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETMAXLAYERWINDOWSIZE:
        {
            i4Ret = _papi_mixer_vmxvl_GetMaxLayerWindowSize(u4LayerNr, (Nat32 *)rKernelArg.au4Arg[1], (Nat32 *)rKernelArg.au4Arg[2]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETSRCWINDOW:
        {
            i4Ret = _papi_mixer_vmxvl_GetSrcWindow(u4LayerNr, (VMixRect *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETSCALEMODE:
        {
            i4Ret = _papi_mixer_vmxvl_GetScaleMode(u4LayerNr, (int *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETINPUTACTIVEIMAGEAREA:
        {
            i4Ret = _papi_mixer_vmxvl_GetInputActiveImageArea(u4LayerNr, (VMixRect *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETMINWINDOWSIZE:
        {
            i4Ret = _papi_mixer_vmxvl_GetMinWindowSize(u4LayerNr, (Nat32 *)rKernelArg.au4Arg[1], (Nat32 *)rKernelArg.au4Arg[2]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_SETSRCWINDOW:
        {
            i4Ret = _papi_mixer_vmxvl_SetSrcWindow(u4LayerNr, (VMixRect *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETBGSUPP:
        {
            i4Ret = _papi_mixer_vmxvl_GetBgSupp(u4LayerNr, (Bool *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_SETLAYERWINDOW:
        {
            i4Ret = _papi_mixer_vmxvl_SetLayerWindow(u4LayerNr, (VMixRect *)rKernelArg.au4Arg[1]);            	
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETSCALERANGE:
        {
            int i4ScaleMode;
            
            i4ScaleMode = (int)rKernelArg.au4Arg[1];
            i4Ret = _papi_mixer_vmxvl_GetScaleRange(u4LayerNr, i4ScaleMode, (Nat32 *)rKernelArg.au4Arg[2], (Nat32 *)rKernelArg.au4Arg[3], (Nat32 *)rKernelArg.au4Arg[4], (Nat32 *)rKernelArg.au4Arg[5]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETLAYERWINDOW:
        {
            i4Ret = _papi_mixer_vmxvl_GetLayerWindow(u4LayerNr, (VMixRect *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_SETPANORAMAMIDSCALE:
        {
            Nat32 u4MidScaleRatio;	

            u4MidScaleRatio = (Nat32)rKernelArg.au4Arg[1];
            i4Ret = _papi_mixer_vmxvl_SetPanoramaMidScale(u4LayerNr, u4MidScaleRatio);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETBGCOLOR:
        {
            i4Ret = _papi_mixer_vmxvl_GetBgColor(u4LayerNr, (VMixColor *)rKernelArg.au4Arg[1]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_SETSCALEMODE:
        {
            Nat32 u4ScaleMode;	
            
            u4ScaleMode = (Nat32)rKernelArg.au4Arg[1];
            i4Ret = _papi_mixer_vmxvl_SetScaleMode(u4LayerNr, u4ScaleMode);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETINPUTRESOLUTION:
        {           
            i4Ret = _papi_mixer_vmxvl_GetInputResolution(u4LayerNr, (Nat32 *)rKernelArg.au4Arg[1], (Nat32 *)rKernelArg.au4Arg[2]);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_ACTIVENEWWINDOWSETTINGS:
        {
            Bool u4Sync;
            Nat32 u4MilliSec;

            u4Sync = (Bool)rKernelArg.au4Arg[1];
            u4MilliSec = (Nat32)rKernelArg.au4Arg[2];
            i4Ret = _papi_mixer_vmxvl_ActivateNewWindowSettings(u4LayerNr, u4Sync, u4MilliSec);
            break;
        }
        case PAPI_IO_MIXER_VMXVL_GETPANORAMAMIDSCALE:
        {
            i4Ret = _papi_mixer_vmxvl_GetPanoramaMidScale(u4LayerNr, (Nat32 *)rKernelArg.au4Arg[1]);
            break;
        }

        default:
            printk("papi_ioctl_mixer_vmxvl: unknown IO command %u\n", cmd);
            i4Ret = -EFAULT;
    }

//    _ApiUnlock()

    return i4Ret;
}

// notification
void PAPIMIXERNotifyNewSettingComplete(UINT32 u4VdpId)
{
    PAPI_MIXER_CB_T rCb;

    // debug msg
    printk("PAPI_CB_MIXER_VMXVL_ONNEWSETTINGSCOMPLETED: %u\n", (unsigned int)u4VdpId);

    rCb.i4Type = PAPI_CB_MIXER_VMXVL_ONNEWSETTINGSCOMPLETED;
    rCb.u4Data1 = u4VdpId;

    // FIXME
    // cookie
    rCb.u4Data2 = 0;
    _CB_PutEvent(CB_PAPI_MIXER_TRIGGER, sizeof (PAPI_MIXER_CB_T), &rCb);
}

void PAPIMIXERNotifyNewSettingInterrupted(UINT32 u4VdpId)
{
    PAPI_MIXER_CB_T rCb;

    // debug msg
    printk("PAPI_CB_MIXER_VMXVL_ONNEWSETTINGINTERRUPTED: %u\n", (unsigned int)u4VdpId);

    rCb.i4Type = PAPI_CB_MIXER_VMXVL_ONNEWSETTINGINTERRUPTED;
    rCb.u4Data1 = u4VdpId;

    // FIXME
    // cookie
    rCb.u4Data2 = 0;
    _CB_PutEvent(CB_PAPI_MIXER_TRIGGER, sizeof (PAPI_MIXER_CB_T), &rCb);
}

void PAPIMIXERVidelLayerNotify(NOTIFY_MIXER_VIDEOLAYER_T tNotify)
{
    switch (tNotify.u4NotifyId)
    {
        case NOTIFICATION_MIXER_NEWSETTINGSCOMPLETED:
        	if (_au4WaitCallback[tNotify.u4VdpId])
        	{
                  PAPIMIXERNotifyNewSettingComplete(tNotify.u4VdpId);
                  _au4WaitCallback[tNotify.u4VdpId] = FALSE;
        	}
        	else
        	{
        	    // debug message
                  printk("papi_ioctl_mixer_vmxvl: driver complete %u\n", (unsigned int)tNotify.u4NotifyId);
        	}
        	break;
        case NOTIFICATION_MIXER_NEWSETTINGINTERRUPTED:
        	if (_au4WaitCallback[tNotify.u4VdpId])
        	{
                  PAPIMIXERNotifyNewSettingInterrupted(tNotify.u4VdpId);
        	}
        	else
        	{
        	    // debug message
                  printk("papi_ioctl_mixer_vmxvl: driver interrupt %u\n", (unsigned int)tNotify.u4NotifyId);
        	}
        	break;

        default:
              printk("papi_ioctl_mixer_vmxvl: unknown notification command %u\n", (unsigned int)tNotify.u4NotifyId);
        	break;
    }
}

