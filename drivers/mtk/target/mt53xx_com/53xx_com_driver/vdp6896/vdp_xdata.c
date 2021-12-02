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
 * $RCSfile: vdp_xdata.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_xdata.c
 *  
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "hw_sw.h"

#include "video_def.h"
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "drv_scaler.h"
#include "x_os.h"
#include "x_assert.h"
#define DEFINE_IS_LOG   VDP_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END



//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define VDP_XDATA_SUPPORT


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VDP_XDATA_THREAD_DELAY (10)
#define VDP_XDATA_THREAD_STACK_SIZE (2048)
#define VDP_XDATA_THREAD_PRIORITY (231)
#define SUB_OFFSET (0x600)
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
static void _VDP_XdataThread(void *pvArg);
static void _VDP_XdataUpdateSrcRegion(UINT32 u4VdpId);
static void _VDP_XdataNormProc(UINT32 u4VdpId);
static void _VDP_XdataUpdateOverscan(UINT32 u4VdpId);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
HANDLE_T h_VdpXdataTread = NULL_HANDLE;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define getOffset(x) (((x)==VDP_1) ? 0 : SUB_OFFSET)
#define fgIsModeChgDone(u4VdpId) (((u4VdpId==VDP_1)?_bMainState:_bPipState) != VDO_STATE_WAIT_MODE_DET)
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void VDP_XdataInit()
{   
    #ifdef VDP_XDATA_SUPPORT
    INT32 i;
    ASSERT(SW_DRAM_BASE != 0);

    for(i=0; i < 2; i++)
    {
        // If default value is non-zero value, please initialize here
    }

    VERIFY(x_thread_create(&h_VdpXdataTread, 
                           "VDP_XDATA", 
                           VDP_XDATA_THREAD_STACK_SIZE, 
                           VDP_XDATA_THREAD_PRIORITY, 
                           _VDP_XdataThread, 
                           0,
                           NULL)==OSR_OK);
    #endif
}

void VDP_XdataSetSrcRegion(UINT32 u4VdpId, VDP_REGION_T rSrcRegion)
{
    #ifdef VDP_XDATA_SUPPORT    
    UINT32 pipOffset = getOffset(u4VdpId);    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_01 + pipOffset, rSrcRegion.u4X, MAIN_SRC_REGION_XOFST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_01 + pipOffset, rSrcRegion.u4Width, MAIN_SRC_REGION_WIDTH);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_02 + pipOffset, rSrcRegion.u4Y, MAIN_SRC_REGION_YOFST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_02 + pipOffset, rSrcRegion.u4Height, MAIN_SRC_REGION_HEIGHT);     
    #endif    
}

//FIXME: by Lucky
void VDP_XdataSetSrcRegionTest(UINT32 u4VdpId, VDP_REGION_T rSrcRegion)
{
    #ifdef VDP_XDATA_SUPPORT    
    UINT32 pipOffset = getOffset(u4VdpId);    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0C + pipOffset, rSrcRegion.u4X, MAIN_SRC_REGION_XOFST_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0C + pipOffset, rSrcRegion.u4Width, MAIN_SRC_REGION_WIDTH_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0D + pipOffset, rSrcRegion.u4Y, MAIN_SRC_REGION_YOFST_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0D + pipOffset, rSrcRegion.u4Height, MAIN_SRC_REGION_HEIGHT_TEST);     
    #endif    
}

//FIXME: by Lucky
void VDP_XdataGetSrcRegionTest(UINT32 u4VdpId, VDP_REGION_T* prSrcRegion)
{
    UINT32 pipOffset = getOffset(u4VdpId);    
    prSrcRegion->u4X = IO32ReadFldAlign(VDP_XDATA_MAIN_0C + pipOffset, MAIN_SRC_REGION_XOFST_TEST);
    prSrcRegion->u4Width = IO32ReadFldAlign(VDP_XDATA_MAIN_0C + pipOffset, MAIN_SRC_REGION_WIDTH_TEST);
    prSrcRegion->u4Y = IO32ReadFldAlign(VDP_XDATA_MAIN_0D + pipOffset, MAIN_SRC_REGION_YOFST_TEST);
    prSrcRegion->u4Height = IO32ReadFldAlign(VDP_XDATA_MAIN_0D + pipOffset, MAIN_SRC_REGION_HEIGHT_TEST);                
}

void VDP_XdataGetSrcRegion(UINT32 u4VdpId, VDP_REGION_T* prSrcRegion)
{
    UINT32 pipOffset = getOffset(u4VdpId);    
    prSrcRegion->u4X = IO32ReadFldAlign(VDP_XDATA_MAIN_01 + pipOffset, MAIN_SRC_REGION_XOFST);
    prSrcRegion->u4Width = IO32ReadFldAlign(VDP_XDATA_MAIN_01 + pipOffset, MAIN_SRC_REGION_WIDTH);
    prSrcRegion->u4Y = IO32ReadFldAlign(VDP_XDATA_MAIN_02 + pipOffset, MAIN_SRC_REGION_YOFST);
    prSrcRegion->u4Height = IO32ReadFldAlign(VDP_XDATA_MAIN_02 + pipOffset, MAIN_SRC_REGION_HEIGHT);                
}

void VDP_XdataSetOutRegion(UINT32 u4VdpId, VDP_REGION_T rOutRegion)
{
    #ifdef VDP_XDATA_SUPPORT    
    UINT32 pipOffset = getOffset(u4VdpId);    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_03 + pipOffset, rOutRegion.u4X, MAIN_OUT_REGION_XOFST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_03 + pipOffset, rOutRegion.u4Width, MAIN_OUT_REGION_WIDTH);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_04 + pipOffset, rOutRegion.u4Y, MAIN_OUT_REGION_YOFST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_04 + pipOffset, rOutRegion.u4Height, MAIN_OUT_REGION_HEIGHT);     
    #endif    
}

//FIXME: by Lucky
void VDP_XdataSetOutRegionTest(UINT32 u4VdpId, VDP_REGION_T rOutRegion)
{
    #ifdef VDP_XDATA_SUPPORT    
    UINT32 pipOffset = getOffset(u4VdpId);    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0A + pipOffset, rOutRegion.u4X, MAIN_OUT_REGION_XOFST_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0A + pipOffset, rOutRegion.u4Width, MAIN_OUT_REGION_WIDTH_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0B + pipOffset, rOutRegion.u4Y, MAIN_OUT_REGION_YOFST_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0B + pipOffset, rOutRegion.u4Height, MAIN_OUT_REGION_HEIGHT_TEST);     
    #endif    
}

//FIXME: by Lucky
void VDP_XdataGetOutRegionTest(UINT32 u4VdpId, VDP_REGION_T* prOutRegion)
{
    UINT32 pipOffset = getOffset(u4VdpId);    
    prOutRegion->u4X = IO32ReadFldAlign(VDP_XDATA_MAIN_0A + pipOffset, MAIN_OUT_REGION_XOFST_TEST);
    prOutRegion->u4Width = IO32ReadFldAlign(VDP_XDATA_MAIN_0A + pipOffset, MAIN_OUT_REGION_WIDTH_TEST);
    prOutRegion->u4Y = IO32ReadFldAlign(VDP_XDATA_MAIN_0B + pipOffset, MAIN_OUT_REGION_YOFST_TEST);
    prOutRegion->u4Height = IO32ReadFldAlign(VDP_XDATA_MAIN_0B + pipOffset, MAIN_OUT_REGION_HEIGHT_TEST);                
}

void VDP_XdataGetOutRegion(UINT32 u4VdpId, VDP_REGION_T* prOutRegion)
{
    UINT32 pipOffset = getOffset(u4VdpId);    
    prOutRegion->u4X = IO32ReadFldAlign(VDP_XDATA_MAIN_03 + pipOffset, MAIN_OUT_REGION_XOFST);
    prOutRegion->u4Width = IO32ReadFldAlign(VDP_XDATA_MAIN_03 + pipOffset, MAIN_OUT_REGION_WIDTH);
    prOutRegion->u4Y = IO32ReadFldAlign(VDP_XDATA_MAIN_04 + pipOffset, MAIN_OUT_REGION_YOFST);
    prOutRegion->u4Height = IO32ReadFldAlign(VDP_XDATA_MAIN_04 + pipOffset, MAIN_OUT_REGION_HEIGHT);                
}

void VDP_XdataSetOverscan(UINT32 u4VdpId, VDP_OVERSCAN_REGION_T rOverscanRegion)
{
    #ifdef VDP_XDATA_SUPPORT  
    UINT32 pipOffset = getOffset(u4VdpId);    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_05 + pipOffset, rOverscanRegion.u4Top, MAIN_OVERSCAN_TOP);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_05 + pipOffset, rOverscanRegion.u4Bottom, MAIN_OVERSCAN_BOTTOM);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_06 + pipOffset, rOverscanRegion.u4Left, MAIN_OVERSCAN_LEFT);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_06 + pipOffset, rOverscanRegion.u4Right, MAIN_OVERSCAN_RIGHT);     
    #endif    
}

//FIXME: by Lucky
void VDP_XdataSetOverscanTest(UINT32 u4VdpId, VDP_OVERSCAN_REGION_T rOverscanRegion)
{
    #ifdef VDP_XDATA_SUPPORT
    UINT32 pipOffset = getOffset(u4VdpId);    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_08 + pipOffset, rOverscanRegion.u4Top, MAIN_OVERSCAN_TOP_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_08 + pipOffset, rOverscanRegion.u4Bottom, MAIN_OVERSCAN_BOTTOM_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_09 + pipOffset, rOverscanRegion.u4Left, MAIN_OVERSCAN_LEFT_TEST);
    vIO32WriteFldAlign(VDP_XDATA_MAIN_09 + pipOffset, rOverscanRegion.u4Right, MAIN_OVERSCAN_RIGHT_TEST);
    #endif
}

//FIXME: by Lucky
void VDP_XdataGetOverscanTest(UINT32 u4VdpId, VDP_OVERSCAN_REGION_T* prOverscanRegion)
{
    UINT32 pipOffset = getOffset(u4VdpId);
    prOverscanRegion->u4Top = IO32ReadFldAlign(VDP_XDATA_MAIN_08 + pipOffset, MAIN_OVERSCAN_TOP_TEST);
    prOverscanRegion->u4Bottom = IO32ReadFldAlign(VDP_XDATA_MAIN_08 + pipOffset, MAIN_OVERSCAN_BOTTOM_TEST);
    prOverscanRegion->u4Left = IO32ReadFldAlign(VDP_XDATA_MAIN_09 + pipOffset, MAIN_OVERSCAN_LEFT_TEST);
    prOverscanRegion->u4Right = IO32ReadFldAlign(VDP_XDATA_MAIN_09 + pipOffset, MAIN_OVERSCAN_RIGHT_TEST);
}

void VDP_XdataGetOverscan(UINT32 u4VdpId, VDP_OVERSCAN_REGION_T* prOverscanRegion)
{
    UINT32 pipOffset = getOffset(u4VdpId);    
    prOverscanRegion->u4Top = IO32ReadFldAlign(VDP_XDATA_MAIN_05 + pipOffset, MAIN_OVERSCAN_TOP);
    prOverscanRegion->u4Bottom = IO32ReadFldAlign(VDP_XDATA_MAIN_05 + pipOffset, MAIN_OVERSCAN_BOTTOM);
    prOverscanRegion->u4Left = IO32ReadFldAlign(VDP_XDATA_MAIN_06 + pipOffset, MAIN_OVERSCAN_LEFT);
    prOverscanRegion->u4Right = IO32ReadFldAlign(VDP_XDATA_MAIN_06 + pipOffset, MAIN_OVERSCAN_RIGHT);                
}

void VDP_XdataSetDispmode(UINT32 u4VdpId, UINT32 u4Mode)
{
    #ifdef VDP_XDATA_SUPPORT    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_00 + getOffset(u4VdpId), u4Mode, MAIN_DISPMODE_SEL);    
    #endif    
}

UINT32 VDP_XdataGetDispmode(UINT32 u4VdpId)
{
    return IO32ReadFldAlign(VDP_XDATA_MAIN_00, MAIN_DISPMODE_SEL);
}

void VDP_XdataSetScaler10Bit(UINT32 u4VdpId, UINT32 u4Forced10BitMode)
{
    #ifdef VDP_XDATA_SUPPORT
    UINT32 u4RegVal;

    switch(u4Forced10BitMode)
    {
        case VDP_SCPOS_8BitMode:
            u4RegVal = 1;
            break;

        case VDP_SCPOS_10BitMode:
            u4RegVal = 2;
            break;

        default:
            u4RegVal = 0;
    }
    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_00 + getOffset(u4VdpId), u4RegVal, MAIN_COLOR_BIT_SEL);    
    #endif    
}

UINT32 VDP_XdataGetScaler10Bit(UINT32 u4VdpId)
{
    UINT32 u4RegVal;
    UINT32 u4Forced10BitMode;
    
    u4RegVal = IO32ReadFldAlign(VDP_XDATA_MAIN_00, MAIN_COLOR_BIT_SEL);
    switch(u4RegVal)
    {
        case 1:
            u4Forced10BitMode = VDP_SCPOS_8BitMode;
            break;

        case 2:
            u4Forced10BitMode = VDP_SCPOS_10BitMode;
            break;

        default:
            u4Forced10BitMode = 0;
    }

    return u4Forced10BitMode;    
}

void VDP_XdataSet121Map(UCHAR ucVdpId, UINT32 ucEn)
{
    #ifdef VDP_XDATA_SUPPORT    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_00 + getOffset(ucVdpId), ucEn, MAIN_121MAP);    
    #endif    
}

UINT32 VDP_XdataGet121Map(UINT32 u4VdpId)
{
    return IO32ReadFldAlign(VDP_XDATA_MAIN_00, MAIN_121MAP);
}

void VDP_XdataSetNewNonlEn(UCHAR ucVdpId, UINT32 ucEn)
{
    #ifdef VDP_XDATA_SUPPORT    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_07 + getOffset(ucVdpId), ucEn, MAIN_NEW_NONL_ENABLE);    
    #endif    
}

UINT32 VDP_XdataGetNewNonlEn(UINT32 u4VdpId)
{
    return IO32ReadFldAlign(VDP_XDATA_MAIN_07, MAIN_NEW_NONL_ENABLE);
}

void VDP_XdataSetNewNonlPrm(UCHAR ucVdpId, VDP_NEW_NONLINEAR_PRM_T rNonlPrm)
{
    #ifdef VDP_XDATA_SUPPORT    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_07 + getOffset(ucVdpId), rNonlPrm.u4SrcMiddleRatio, MAIN_NEW_NONL_SRC_MIDDLE);    
    vIO32WriteFldAlign(VDP_XDATA_MAIN_07 + getOffset(ucVdpId), rNonlPrm.u4OutMiddleRatio, MAIN_NEW_NONL_OUT_MIDDLE);    
    #endif    
}

void VDP_XdataGetNewNonlPrm(UINT32 u4VdpId,  VDP_NEW_NONLINEAR_PRM_T *prNonlPrm)
{
    prNonlPrm->u4SrcMiddleRatio = IO32ReadFldAlign(VDP_XDATA_MAIN_07, MAIN_NEW_NONL_SRC_MIDDLE);
    prNonlPrm->u4OutMiddleRatio = IO32ReadFldAlign(VDP_XDATA_MAIN_07, MAIN_NEW_NONL_OUT_MIDDLE);    
}


//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------

static void _VDP_XdataUpdateSrcRegion(UINT32 u4VdpId)
{
    VDP_REGION_T rSrcRegion;
    VDP_REGION_T rXdtaSrcRegion;
    UCHAR ucDummy;
    
    VDP_GetSrcRegion(u4VdpId, &ucDummy, &rSrcRegion);
    VDP_XdataGetSrcRegion(u4VdpId, &rXdtaSrcRegion);
    if(x_memcmp(&rSrcRegion, &rXdtaSrcRegion, sizeof(VDP_REGION_T)) != 0) 
    {
        VDP_SetSrcRegion(u4VdpId, 0, rXdtaSrcRegion);
    }
}

//FIXME: by Lucky
static void _VDP_XdataUpdateSrcRegionTest(UINT32 u4VdpId)
{
    //UCHAR ucDummy;
    //VDP_REGION_T rSrcRegion;
    VDP_REGION_T rXdtaSrcRegion;
    UINT32 pipOffset = getOffset(u4VdpId);  
    UINT32 time, step, i, minsize;

    // static local variables
    static UINT32 _Test4SrcRegion = 0;
    static UINT32 _Test4SrcRegionDelay = 0;

    time = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_TIME);
    step = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_STEP) + 1;
    
    //VDP_GetSrcRegion(u4VdpId, &ucDummy, &rSrcRegion);
    VDP_XdataGetSrcRegionTest(u4VdpId, &rXdtaSrcRegion);
    if ((rXdtaSrcRegion.u4Width == 0 && rXdtaSrcRegion.u4Height == 0) || (rXdtaSrcRegion.u4X + rXdtaSrcRegion.u4Width > VDP_MAX_REGION_WIDTH) 
                                                                      || (rXdtaSrcRegion.u4Y + rXdtaSrcRegion.u4Height> VDP_MAX_REGION_HEIGHT))
    {
        _Test4SrcRegion = _Test4SrcRegionDelay = 0;
        return;
    }

    if (_Test4SrcRegionDelay-- > 0 && IO32ReadFldAlign(VDP_XDATA_MAIN_00+pipOffset, MAIN_CHG_TRIGGER) == 0)
    {
        return;
    }
    else
    {
        _Test4SrcRegionDelay = time;
    }
    //LOG(1, "my xdata input x %d, y %d, width %d, height %d \n", rXdtaSrcRegion.u4X, rXdtaSrcRegion.u4Y, rXdtaSrcRegion.u4Width, rXdtaSrcRegion.u4Height);

    minsize = (rXdtaSrcRegion.u4X + rXdtaSrcRegion.u4Width < rXdtaSrcRegion.u4Y + rXdtaSrcRegion.u4Height)? 
               rXdtaSrcRegion.u4X + rXdtaSrcRegion.u4Width:
               rXdtaSrcRegion.u4Y + rXdtaSrcRegion.u4Height;

    i = ++_Test4SrcRegion;
    if (i <= (VDP_MAX_REGION_WIDTH - minsize)/step)
    {
        if (VDP_MAX_REGION_WIDTH - rXdtaSrcRegion.u4X - i*step > rXdtaSrcRegion.u4Width)
        {
            rXdtaSrcRegion.u4Width = VDP_MAX_REGION_WIDTH - rXdtaSrcRegion.u4X - i*step;
        }
        if (VDP_MAX_REGION_HEIGHT - rXdtaSrcRegion.u4Y - i*step > rXdtaSrcRegion.u4Height)
        {
            rXdtaSrcRegion.u4Height = VDP_MAX_REGION_HEIGHT - rXdtaSrcRegion.u4Y - i*step;
        }
        VDP_SetSrcRegion(u4VdpId, 0, rXdtaSrcRegion);
    }
    else if(i <= (VDP_MAX_REGION_WIDTH - minsize)/step + rXdtaSrcRegion.u4X/step)
    {
        i = i - (VDP_MAX_REGION_WIDTH - minsize)/step;
        if (i*step < rXdtaSrcRegion.u4X)
        {
            rXdtaSrcRegion.u4X = i*step;
        }
        VDP_SetSrcRegion(u4VdpId, 0, rXdtaSrcRegion);
    }
    else if(i <= (VDP_MAX_REGION_WIDTH - minsize)/step + rXdtaSrcRegion.u4X/step + rXdtaSrcRegion.u4Y/step)
    {
        i = i - (VDP_MAX_REGION_WIDTH - minsize)/step - rXdtaSrcRegion.u4X/step;
        if (i*step < rXdtaSrcRegion.u4Y)
        {
            rXdtaSrcRegion.u4Y = i*step;
        }
        VDP_SetSrcRegion(u4VdpId, 0, rXdtaSrcRegion);
    }
    else // reset source region to 0
    {
        rXdtaSrcRegion.u4X = rXdtaSrcRegion.u4Y = rXdtaSrcRegion.u4Width = rXdtaSrcRegion.u4Height = 0;
        VDP_XdataSetSrcRegionTest(u4VdpId, rXdtaSrcRegion);
        _Test4SrcRegion = _Test4SrcRegionDelay = 0;
        LOG(1, "Last \n");
    }
    LOG(1, "loop x %d, y %d, width %d, height %d count %d\n", rXdtaSrcRegion.u4X, rXdtaSrcRegion.u4Y, rXdtaSrcRegion.u4Width, rXdtaSrcRegion.u4Height, _Test4SrcRegion);
}

static void _VDP_XdataUpdateOutRegion(UINT32 u4VdpId)
{
    VDP_REGION_T rOutRegion;
    VDP_REGION_T rXdtaOutRegion;
    UCHAR ucDummy;
    
    VDP_GetOutRegion(u4VdpId, &ucDummy, &rOutRegion);
    VDP_XdataGetOutRegion(u4VdpId, &rXdtaOutRegion);
    if(x_memcmp(&rOutRegion, &rXdtaOutRegion, sizeof(VDP_REGION_T)) != 0) 
    {
        VDP_SetOutRegion(u4VdpId, 0, rXdtaOutRegion);
    }
}

//FIXME: by Lucky
static void _VDP_OutRegionPatternTest(UINT32 u4VdpId, UINT32 step)
{
    UCHAR ucDummy;
    VDP_REGION_T rOutRegion;
    VDP_REGION_T rXdtaOutRegion;
    
    // static local variables
    static UINT32 _Test4OutRegionPattern1 = 0;

    VDP_GetOutRegion(u4VdpId, &ucDummy, &rOutRegion);
    if (_Test4OutRegionPattern1 == 0)
    {
        if (rOutRegion.u4Width == VDP_MAX_REGION_WIDTH && rOutRegion.u4Height == VDP_MAX_REGION_HEIGHT)
        {
            _Test4OutRegionPattern1 = 1;
        }
        else
        {
            return;
        }
    }
    
    if (_Test4OutRegionPattern1 < 10)
    {
        if (_Test4OutRegionPattern1 & 1) // odd
        {
            if (rOutRegion.u4Width - step >= (_Test4OutRegionPattern1+1)/2*1000)
            {
                rXdtaOutRegion.u4Width  = rOutRegion.u4Width  - step;
                rXdtaOutRegion.u4Height = rOutRegion.u4Height - step;
            }
            else
            {
                rXdtaOutRegion.u4Width  = rOutRegion.u4Width;
                rXdtaOutRegion.u4Height = rOutRegion.u4Height;
                _Test4OutRegionPattern1++;
            }
        }
        else                             // even
        {
            if (rOutRegion.u4Width + step <= VDP_MAX_REGION_WIDTH - _Test4OutRegionPattern1/2*1000)
            {
                rXdtaOutRegion.u4Width  = rOutRegion.u4Width  + step;
                rXdtaOutRegion.u4Height = rOutRegion.u4Height + step;
            }
            else
            {
                rXdtaOutRegion.u4Width  = rOutRegion.u4Width;
                rXdtaOutRegion.u4Height = rOutRegion.u4Height;
                _Test4OutRegionPattern1++;
            }
        }            
    }
    else // finish 1~9 steps
    {
        rXdtaOutRegion.u4Width  = rOutRegion.u4Width;
        rXdtaOutRegion.u4Height = rOutRegion.u4Height;
        _Test4OutRegionPattern1 = 0;
    }
    rXdtaOutRegion.u4X = (VDP_MAX_REGION_WIDTH  - rXdtaOutRegion.u4Width )/2;
    rXdtaOutRegion.u4Y = (VDP_MAX_REGION_HEIGHT - rXdtaOutRegion.u4Height)/2;
    
    VDP_SetOutRegion(u4VdpId, 0, rXdtaOutRegion);   
}

//FIXME: by Lucky
static void _VDP_OutRegionRandomTest1(UINT32 u4VdpId)
{
    VDP_REGION_T rXdtaOutRegion;
    UINT32 next;

    next = IO32ReadFldAlign(VDP_XDATA_MAIN_0F, MAIN_TEST_RANDOM);
    next = next*1103515245 + 12345;
    vIO32WriteFldAlign(VDP_XDATA_MAIN_0F, next, MAIN_TEST_RANDOM);

    rXdtaOutRegion.u4X = (next/64)/8192;
    rXdtaOutRegion.u4Y = (next/64)%8192;
    rXdtaOutRegion.u4Width = rXdtaOutRegion.u4Height = 1808;

    VDP_SetOutRegion(u4VdpId, 0, rXdtaOutRegion);   
}

//FIXME: by Lucky
static void _VDP_OutRegionRandomTest2(UINT32 u4VdpId, UINT32 step, VDP_REGION_T* prOutRegion)
{
    VDP_REGION_T rXdtaOutRegion;
    UINT32 next;

    // static local variables
    static UINT32 _Test4RandomX = 0;
    static UINT32 _Test4RandomY = 0;

    while (!((_Test4RandomX > prOutRegion->u4X && _Test4RandomX >= prOutRegion->u4X + step) ||
             (_Test4RandomX < prOutRegion->u4X && _Test4RandomX + step <= prOutRegion->u4X) ||
             (_Test4RandomY > prOutRegion->u4Y && _Test4RandomY >= prOutRegion->u4Y + step) ||
             (_Test4RandomY < prOutRegion->u4Y && _Test4RandomY + step <= prOutRegion->u4Y) ))
    {   // randomize next destination
        next = IO32ReadFldAlign(VDP_XDATA_MAIN_0F, MAIN_TEST_RANDOM);
        next = next*1103515245 + 12345;
        vIO32WriteFldAlign(VDP_XDATA_MAIN_0F, next, MAIN_TEST_RANDOM);
        
        _Test4RandomX = (next/64)/8192;
        _Test4RandomY = (next/64)%8192;
    }

    if (_Test4RandomX > prOutRegion->u4X)
    {
        if (prOutRegion->u4X + step <= _Test4RandomX)
        {
            rXdtaOutRegion.u4X = prOutRegion->u4X + step;
        }
        else
        {
            rXdtaOutRegion.u4X = prOutRegion->u4X;
        }
    }
    else if (_Test4RandomX < prOutRegion->u4X)
    {
        if (prOutRegion->u4X >= _Test4RandomX + step)
        {
            rXdtaOutRegion.u4X = prOutRegion->u4X - step;
        }
        else
        {
            rXdtaOutRegion.u4X = prOutRegion->u4X;
        }
    }
    else
    {
        rXdtaOutRegion.u4X = prOutRegion->u4X;
    }

    if (_Test4RandomY > prOutRegion->u4Y)
    {
        if (prOutRegion->u4Y + step <= _Test4RandomY)
        {
            rXdtaOutRegion.u4Y = prOutRegion->u4Y + step;
        }
        else
        {
            rXdtaOutRegion.u4Y = prOutRegion->u4Y;
        }
    }
    else if(_Test4RandomY < prOutRegion->u4Y)
    {
        if (prOutRegion->u4Y >= _Test4RandomY + step)
        {
            rXdtaOutRegion.u4Y = prOutRegion->u4Y - step;
        }
        else
        {
            rXdtaOutRegion.u4Y = prOutRegion->u4Y;
        }
    }
    else
    {
        rXdtaOutRegion.u4Y = prOutRegion->u4Y;
    }
    rXdtaOutRegion.u4Width = rXdtaOutRegion.u4Height = 1808;

    LOG(1, " x %d y %d random x %d y %d \n", rXdtaOutRegion.u4X, rXdtaOutRegion.u4Y, _Test4RandomX, _Test4RandomY);
    VDP_SetOutRegion(u4VdpId, 0, rXdtaOutRegion);
}

//FIXME: by Lucky
static void _VDP_XdataUpdateOutRegionTest(UINT32 u4VdpId)
{
    UCHAR ucDummy;
    VDP_REGION_T rOutRegion;
    VDP_REGION_T rXdtaOutRegion;
    UINT32 pipOffset = getOffset(u4VdpId);  
    UINT32 time, step, i, maxsize, maxdistance, rand, pat1, pat2;

    // static local variables
    static UINT32 _Test4OutRegion = 0;
    static UINT32 _Test4OutRegionDelay = 0;
    static UINT32 _Test4OutRegionStat = 0;

    time = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_TIME);
    step = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_STEP) + 1;
    rand = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_RAND);
    pat1 = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_PAT1);
    pat2 = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_PAT2);
    
    VDP_GetOutRegion(u4VdpId, &ucDummy, &rOutRegion);
    VDP_XdataGetOutRegionTest(u4VdpId, &rXdtaOutRegion);
    if ((rXdtaOutRegion.u4Width == 0 && rXdtaOutRegion.u4Height == 0) || (rXdtaOutRegion.u4X + rXdtaOutRegion.u4Width > VDP_MAX_REGION_WIDTH) 
                                                                      || (rXdtaOutRegion.u4Y + rXdtaOutRegion.u4Height> VDP_MAX_REGION_HEIGHT))
    {
        _Test4OutRegion = _Test4OutRegionDelay = 0;
        return;
    }

    if (_Test4OutRegionDelay-- > 0 && IO32ReadFldAlign(VDP_XDATA_MAIN_00+pipOffset, MAIN_CHG_TRIGGER) == 0)
    {
        return;
    }
    else
    {
        _Test4OutRegionDelay = time;
    }

    if (rand != 0)
    {
        _VDP_OutRegionRandomTest1(u4VdpId);
        return;
    }

    if (pat1 != 0)
    {
        _VDP_OutRegionPatternTest(u4VdpId, step);
        return;
    }

    if (pat2 != 0)
    {
        _VDP_OutRegionRandomTest2(u4VdpId, step, &rOutRegion);
        return;
    }

    //LOG(1, "my xdata input x %d, y %d, width %d, height %d \n", rXdtaOutRegion.u4X, rXdtaOutRegion.u4Y, rXdtaOutRegion.u4Width, rXdtaOutRegion.u4Height);
    //LOG(1, "current  input x %d, y %d, width %d, height %d \n", rOutRegion.u4X, rOutRegion.u4Y, rOutRegion.u4Width, rOutRegion.u4Height);

    maxsize = (rXdtaOutRegion.u4Width > rXdtaOutRegion.u4Height)? rXdtaOutRegion.u4Width: rXdtaOutRegion.u4Height;
    maxdistance = (rXdtaOutRegion.u4Width > rXdtaOutRegion.u4Height)? 5*((VDP_MAX_REGION_WIDTH - rXdtaOutRegion.u4Width) /step) + (VDP_MAX_REGION_HEIGHT- rXdtaOutRegion.u4Height)/step:
                                                                      5*((VDP_MAX_REGION_HEIGHT- rXdtaOutRegion.u4Height)/step) + (VDP_MAX_REGION_WIDTH - rXdtaOutRegion.u4Width) /step;

    maxdistance += ((VDP_MAX_REGION_WIDTH - rXdtaOutRegion.u4Width - rXdtaOutRegion.u4X)/step+
                    (VDP_MAX_REGION_HEIGHT- rXdtaOutRegion.u4Height- rXdtaOutRegion.u4Y)/step);

    i = ++_Test4OutRegion;
    if (i <= VDP_MAX_REGION_WIDTH/step) // from full-size to zero
    {
        rXdtaOutRegion.u4Width  = VDP_MAX_REGION_WIDTH  - i*step;
        rXdtaOutRegion.u4Height = VDP_MAX_REGION_HEIGHT - i*step;      
        VDP_SetOutRegion(u4VdpId, 0, rXdtaOutRegion);
    }
    else if (i <= VDP_MAX_REGION_WIDTH/step + maxsize/step) // change size
    {
        i -= VDP_MAX_REGION_WIDTH/step;
        if (i*step < rXdtaOutRegion.u4Width)
        {
            rXdtaOutRegion.u4Width = i*step;
        }
        if (i*step < rXdtaOutRegion.u4Height)
        {
            rXdtaOutRegion.u4Height = i*step;
        }
        VDP_SetOutRegion(u4VdpId, 0, rXdtaOutRegion);
        //LOG(1, "width %d, height %d \n", rXdtaOutRegion.u4Width, rXdtaOutRegion.u4Height);
    }
    else if(i <= VDP_MAX_REGION_WIDTH/step + maxsize/step + maxdistance) // chang position
    {
        if (_Test4OutRegionStat == 0)
        {
            if (rOutRegion.u4X + rOutRegion.u4Width + step <= VDP_MAX_REGION_WIDTH)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X + step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y;
            }
            else
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y + step;
                _Test4OutRegionStat = 1;
            }
        }
        else if (_Test4OutRegionStat == 1)
        {
            if (rOutRegion.u4Y + rOutRegion.u4Height + step <= VDP_MAX_REGION_HEIGHT)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y + step;
            }
            else
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X - step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y - step;
                _Test4OutRegionStat = 2;
            }
        }
        else if (_Test4OutRegionStat == 2)
        {
            if (rOutRegion.u4X >= step && rOutRegion.u4Y >= step)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X - step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y - step;
            }
            else
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X + step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y + step;
                _Test4OutRegionStat = 3;
            }
        }
        else if (_Test4OutRegionStat == 3)
        {
            if (rOutRegion.u4X + rOutRegion.u4Width + step <= VDP_MAX_REGION_WIDTH
             && rOutRegion.u4Y + rOutRegion.u4Height+ step <= VDP_MAX_REGION_HEIGHT)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X + step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y + step;
            }
            else
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X - step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y;
                _Test4OutRegionStat = 4;
            }
        }
        else if (_Test4OutRegionStat == 4)
        {
            if (rOutRegion.u4X >= step)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X - step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y;
            }
            else
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X + step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y - step;
                _Test4OutRegionStat = 5;
            }
        }
        else if (_Test4OutRegionStat == 5)
        {
            if (rOutRegion.u4X + rOutRegion.u4Width + step <= VDP_MAX_REGION_WIDTH && rOutRegion.u4Y >= step)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X + step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y - step;
            }
            else
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X - step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y + step;
                _Test4OutRegionStat = 6;
            }
        }
        else if (_Test4OutRegionStat == 6)
        {
            if (rOutRegion.u4X >= step && rOutRegion.u4Y + rOutRegion.u4Height + step <= VDP_MAX_REGION_HEIGHT)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X - step;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y + step;
            }
            else
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y - step;
                _Test4OutRegionStat = 7;
            }
        }
        else if (_Test4OutRegionStat == 7)
        {
            if (rOutRegion.u4Y >= step)
            {
                rXdtaOutRegion.u4X = rOutRegion.u4X;
                rXdtaOutRegion.u4Y = rOutRegion.u4Y - step;
                if (rXdtaOutRegion.u4Y < step)
                {
                    _Test4OutRegionStat = 8;
                }
            }
        }
        VDP_SetOutRegion(u4VdpId, 0, rXdtaOutRegion);
        //LOG(1, "x %d y %d movement \n", rXdtaOutRegion.u4X, rXdtaOutRegion.u4Y);
    }
    else // reset output region to 0
    {
        rXdtaOutRegion.u4X = rXdtaOutRegion.u4Y = rXdtaOutRegion.u4Width = rXdtaOutRegion.u4Height = 0;
        VDP_XdataSetOutRegionTest(u4VdpId, rXdtaOutRegion);
        _Test4OutRegion = _Test4OutRegionDelay = _Test4OutRegionStat = 0;
        LOG(1, "Last \n");
    }
    LOG(1, "loop x %d, y %d, width %d, height %d count %d, stat %d \n", rXdtaOutRegion.u4X, rXdtaOutRegion.u4Y, rXdtaOutRegion.u4Width, rXdtaOutRegion.u4Height, _Test4OutRegion, _Test4OutRegionStat);
}

static void _VDP_XdataUpdateOverscan(UINT32 u4VdpId)
{
    VDP_OVERSCAN_REGION_T rOverscan;
    VDP_OVERSCAN_REGION_T rXdtaOverscan;
    
    VDP_GetOverScan(u4VdpId, &rOverscan);
    VDP_XdataGetOverscan(u4VdpId, &rXdtaOverscan);
    if(x_memcmp(&rOverscan, &rXdtaOverscan, sizeof(VDP_OVERSCAN_REGION_T)) != 0) 
    {
        VDP_SetOverScan(u4VdpId, rXdtaOverscan);
    }
}

//FIXME: by Lucky
static void _VDP_XdataUpdateOverscanTest(UINT32 u4VdpId)
{
    VDP_OVERSCAN_REGION_T rXdtaOverscan;
    UINT32 pipOffset = getOffset(u4VdpId);  
    UINT32 time, step, i, maxoverscan;

    // static local variables
    static UINT32 _Test4Overscan = 0;
    static UINT32 _Test4OverscanDelay = 0;

    time = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_TIME);
    step = IO32ReadFldAlign(VDP_XDATA_MAIN_0E + pipOffset, MAIN_TEST_STEP) + 1;

    VDP_XdataGetOverscanTest(u4VdpId, &rXdtaOverscan);
    if (rXdtaOverscan.u4Top == 0 && rXdtaOverscan.u4Bottom == 0 && rXdtaOverscan.u4Left == 0 && rXdtaOverscan.u4Right == 0)
    {
        _Test4Overscan = _Test4OverscanDelay = 0;
        return;
    }
    
    if (_Test4OverscanDelay-- > 0 && IO32ReadFldAlign(VDP_XDATA_MAIN_00+pipOffset, MAIN_CHG_TRIGGER) == 0)
    {
        return;
    }
    else
    {
        _Test4OverscanDelay = time;
    }
    //LOG(1, "my xdata input top %d, bottom %d, left %d, right %d time %d step %d\n", 
    //    rXdtaOverscan.u4Top, rXdtaOverscan.u4Bottom, rXdtaOverscan.u4Left, rXdtaOverscan.u4Right, time, step);

    maxoverscan = (rXdtaOverscan.u4Top > rXdtaOverscan.u4Bottom) ? rXdtaOverscan.u4Top : rXdtaOverscan.u4Bottom;
    maxoverscan = (maxoverscan > rXdtaOverscan.u4Left) ? maxoverscan : rXdtaOverscan.u4Left;
    maxoverscan = (maxoverscan > rXdtaOverscan.u4Right) ? maxoverscan : rXdtaOverscan.u4Right;
    
    i = ++_Test4Overscan;
    if (i <= (rXdtaOverscan.u4Top/step))
    {
        rXdtaOverscan.u4Top = i*step;
        rXdtaOverscan.u4Bottom = 0;
        rXdtaOverscan.u4Left = 0;
        rXdtaOverscan.u4Right = 0;
        VDP_SetOverScan(u4VdpId, rXdtaOverscan);
    }
    else if (i <= (rXdtaOverscan.u4Top/step)+(rXdtaOverscan.u4Bottom/step))
    {
        rXdtaOverscan.u4Bottom = (i - rXdtaOverscan.u4Top/step)*step;
        rXdtaOverscan.u4Top = 0;
        rXdtaOverscan.u4Left = 0;
        rXdtaOverscan.u4Right = 0;
        VDP_SetOverScan(u4VdpId, rXdtaOverscan);
    }
    else if (i <= (rXdtaOverscan.u4Top/step)+(rXdtaOverscan.u4Bottom/step)+(rXdtaOverscan.u4Left/step))        
    {
        rXdtaOverscan.u4Left = (i - rXdtaOverscan.u4Top/step - rXdtaOverscan.u4Bottom/step)*step;
        rXdtaOverscan.u4Top = 0;
        rXdtaOverscan.u4Bottom = 0;
        rXdtaOverscan.u4Right = 0;
        VDP_SetOverScan(u4VdpId, rXdtaOverscan);
    }
    else if (i <= (rXdtaOverscan.u4Top/step)+(rXdtaOverscan.u4Bottom/step)+(rXdtaOverscan.u4Left/step)+(rXdtaOverscan.u4Right/step))
    {
        rXdtaOverscan.u4Right= (i - rXdtaOverscan.u4Top/step - rXdtaOverscan.u4Bottom/step - rXdtaOverscan.u4Left/step)*step;
        rXdtaOverscan.u4Top = 0;
        rXdtaOverscan.u4Bottom = 0;
        rXdtaOverscan.u4Left = 0;
        VDP_SetOverScan(u4VdpId, rXdtaOverscan);
    }
    else if (i <= (rXdtaOverscan.u4Top/step)+(rXdtaOverscan.u4Bottom/step)+(rXdtaOverscan.u4Left/step)+(rXdtaOverscan.u4Right/step)+(maxoverscan/step))
    {
        i -= (rXdtaOverscan.u4Top/step)+(rXdtaOverscan.u4Bottom/step)+(rXdtaOverscan.u4Left/step)+(rXdtaOverscan.u4Right/step);
        if (i*step < rXdtaOverscan.u4Top)
        {
            rXdtaOverscan.u4Top= i*step;
        }
        if (i*step < rXdtaOverscan.u4Bottom)
        {
            rXdtaOverscan.u4Bottom= i*step;
        }
        if (i*step < rXdtaOverscan.u4Left)
        {
            rXdtaOverscan.u4Left = i*step;
        }
        if (i*step < rXdtaOverscan.u4Right)
        {
            rXdtaOverscan.u4Right = i*step;
        }
        VDP_SetOverScan(u4VdpId, rXdtaOverscan);
    }
    else // reset overscan setting to 0
    {
        rXdtaOverscan.u4Top = rXdtaOverscan.u4Bottom = rXdtaOverscan.u4Left = rXdtaOverscan.u4Right = 0;
        VDP_XdataSetOverscanTest(u4VdpId, rXdtaOverscan);
        _Test4Overscan = _Test4OverscanDelay = 0;
    }    
    //LOG(1, "loop T %d B %d L %d R %d count %d \n", rXdtaOverscan.u4Top, rXdtaOverscan.u4Bottom, rXdtaOverscan.u4Left, rXdtaOverscan.u4Right, _Test4Overscan);
}

static void _VDP_XdataUpdateDispmode(UINT32 u4VdpId)
{
    UINT32 u4XdataDispmode;
    UINT32 u4Dispmode;
    
    u4Dispmode = VDP_GetDispmode(u4VdpId);
    u4XdataDispmode = VDP_XdataGetDispmode(u4VdpId);
    if(u4Dispmode != u4XdataDispmode) 
    {
        VDP_SetDispmode(u4VdpId, u4XdataDispmode);
    }
}

static void _VDP_XdataUpdateScaler10Bit(UINT32 u4VdpId)
{
    UINT32 u4XdataScaler10Bit;
    UINT32 u4Scaler10Bit;
    
    u4Scaler10Bit = VDP_GetScaler10BitMode(u4VdpId);
    u4XdataScaler10Bit = VDP_XdataGetScaler10Bit(u4VdpId);
    if(u4Scaler10Bit != u4XdataScaler10Bit) 
    {
        VDP_SetScaler10BitMode(u4VdpId, u4XdataScaler10Bit);
    }
}

static void _VDP_XdataUpdate121Map(UINT32 u4VdpId)
{
    UINT32 u4Xdata121Map;
    UINT32 u4121Map;
    
    u4121Map = VDP_Get121Map((UCHAR)u4VdpId);
    u4Xdata121Map = VDP_XdataGet121Map(u4VdpId);
    if(u4121Map != u4Xdata121Map) 
    {
        VDP_Set121Map((UCHAR)u4VdpId, (UCHAR)u4Xdata121Map);
    }
}

static void _VDP_XdataUpdateNewNonlEn(UINT32 u4VdpId)
{
    UCHAR ucXdataNonlEn;
    UCHAR ucNonlEn;
    
    VDP_GetNonlinear((UCHAR)u4VdpId, &ucNonlEn);
    ucXdataNonlEn = (UCHAR) VDP_XdataGetNewNonlEn(u4VdpId);
    if(ucNonlEn != ucXdataNonlEn) 
    {
        VDP_SetNonlinear((UCHAR)u4VdpId, (UCHAR)ucXdataNonlEn);
    }
}

static void _VDP_XdataUpdateNewNonlPrm(UINT32 u4VdpId)
{
    VDP_NEW_NONLINEAR_PRM_T rPrm;
    VDP_NEW_NONLINEAR_PRM_T rXdtaPrm;
    
    vScpipGetNewNonLinear(u4VdpId, &rPrm);
    VDP_XdataGetNewNonlPrm(u4VdpId, &rXdtaPrm);
    if(x_memcmp(&rPrm, &rXdtaPrm, sizeof(VDP_NEW_NONLINEAR_PRM_T)) != 0) 
    {
        VDP_SetNewNonLinear((UCHAR)u4VdpId, rXdtaPrm);
    }
}

static void _VDP_XdataNormProc(UINT32 u4VdpId)
{    
    UINT32 pipOffset = getOffset(u4VdpId);
    if(fgIsModeChgDone(u4VdpId) 
        && (IO32ReadFldAlign(VDP_XDATA_MAIN_00+pipOffset, MAIN_CHG_AUTO) 
        ||  IO32ReadFldAlign(VDP_XDATA_MAIN_00+pipOffset, MAIN_CHG_TRIGGER)))
    {
        _VDP_XdataUpdateSrcRegion(u4VdpId);
        _VDP_XdataUpdateSrcRegionTest(u4VdpId); //FIXME: by Lucky
        _VDP_XdataUpdateOutRegion(u4VdpId);
        _VDP_XdataUpdateOutRegionTest(u4VdpId); //FIXME: by Lucky
        _VDP_XdataUpdateOverscan(u4VdpId);
        _VDP_XdataUpdateOverscanTest(u4VdpId);  //FIXME: by Lucky
        _VDP_XdataUpdateDispmode(u4VdpId);
        _VDP_XdataUpdateScaler10Bit(u4VdpId);
        _VDP_XdataUpdate121Map(u4VdpId);
        _VDP_XdataUpdateNewNonlEn(u4VdpId);    
        _VDP_XdataUpdateNewNonlPrm(u4VdpId);   
        vIO32WriteFldAlign(VDP_XDATA_MAIN_00+pipOffset, 0, MAIN_CHG_TRIGGER);
    }
}

static void _VDP_XdataThread(void *pvArg)
{
    while(1)
    {
        _VDP_XdataNormProc(VDP_1);
        _VDP_XdataNormProc(VDP_2);
        
        x_thread_delay(VDP_XDATA_THREAD_DELAY);
    }
}

