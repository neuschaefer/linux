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
 * $RCSfile: drv_frametrack.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "frametrack_drvif.h"
#include "video_def.h"
#include "hw_scpos.h"
#include "hw_ddds.h"
#include "drv_scaler.h"
#include "drv_tvd.h"
#include "drv_hdtv.h"
#include "drvcust_if.h"
#include "vdp_drvif.h"
#include "vdo_misc.h"
#include "drv_display.h"
#include "nptv_debug.h"
#include "drv_di.h"
#include "drv_scaler_drvif.h"
#include "srm_drvif.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#endif

/**************************************************************************
 * Global/Static variables
 *************************************************************************/
UINT32 _u4FrameTrackMode = FRAME_TRACK_MODE_DDDS_BY_SCALER;
UINT32 u4ForceFrameTrackCtrl = SCPIP_FRAME_TRACK_AUTO;
UINT32 u4ForceFrameTrackMode = 1;


#ifdef SUPPORT_SW_FRAME_CHECK
extern UINT32 _u4VDistTrackEn;
extern UINT32 _u4VDistTrackState;
#endif

#ifdef SUPPORT_50_TO_120
UINT32 fg50To120;
#endif


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define FRAME_TRACK_50TO60_STEP1    1   // for fast and slow
#define FRAME_TRACK_50TO60_STEP2    2   // for faster and slower

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
BOOL fgFrameTrackIsFlgSet(UINT32 u4Flag);

/**
* @brief Get scaler input/output V sync distance count
*/
UINT32 u4ScpipGetInOutVDistance(void)
{
    return RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_VCOUNTER_OUT_1);
}

UINT32 GCD(UINT32 a, UINT32 b)
{
    while (b != 0)
    {
        UINT32 tmp = a%b;
        a = b;
        b = tmp;
    }
    return a;
}

BOOL fgFrameTrackIsFlgSet(UINT32 u4Flag)
{
    return ((_u4FrameTrackMode & u4Flag) == u4Flag);
}

void vFrameTrackSetFlg(UINT32 u4Flag)
{
    _u4FrameTrackMode |= u4Flag;
}

void vFrameTrackClrFlg(UINT32 u4Flag)
{
    _u4FrameTrackMode &= (~u4Flag);
}

UINT32 u4GetFrameTrackMode(void)
{
    UINT32 u4Mode;

    if (u4ForceFrameTrackMode)
    {
        u4Mode = u4ForceFrameTrackMode;
    }
    else
    {
        u4Mode = DRVCUST_OptGet(eFrameTrackMode);
    }
    return u4Mode;
}

void vFrameTrackDDDSConfig(UINT32 u4Step1, UINT32 u4Step2)
{
    UINT32 u4PanelVTotal = wDrvGetOutputVTotal();
    vDrvSetDDDSFrameTrackTarget(SV_DDDS_FRAMETRACK_FAST1, u4PanelVTotal + u4Step1); // Fast
    vDrvSetDDDSFrameTrackTarget(SV_DDDS_FRAMETRACK_FAST2, u4PanelVTotal + u4Step2); // Faster
    vDrvSetDDDSFrameTrackTarget(SV_DDDS_FRAMETRACK_SLOW1, u4PanelVTotal - u4Step1); // Slow
    vDrvSetDDDSFrameTrackTarget(SV_DDDS_FRAMETRACK_SLOW2, u4PanelVTotal - u4Step2); // Slower
}

UINT32 u4GetFrameTrackOnOff(void)
{
    #if 0
    UINT32 u4Mode = u4GetFrameTrackMode();

    if (u4Mode == 1)  // HW Frame Track
    {
        return RegReadFldAlign(SCPIP_PIP_OA_0E, PIP_OA_0E_FRAME_TRACK_EN1);
    }
    else if (u4Mode == 2)  // SW frame track
    {
        #ifdef SUPPORT_SW_FRAME_CHECK
        return _u4VDistTrackEn;
        #endif
    }   
    #endif
    return 0;
}

#if 0
E_FRAME_TRACK_STAUS  eScpipGetFrameTrackStatus(void) 
{
    #if 0
    UINT32 u4Status, u4VDist,u4VTotal, bmode;
    UINT32 u4Mode = u4GetFrameTrackMode();

    if (u4Mode == 1)  // HW Frame Track
    {
        u4Status = RegReadFldAlign(SCPIP_PIP_OA_RO_02, PIP_OA_RO_02_FRAME_TRACK_ST);
        switch(u4Status)
        {
            case 0:
                return SCPIP_FRAME_TRACK_DISABLE;
            case 5: 
                return SCPIP_FRAME_TRACK_LOCK;
            case 1:
                     u4VDist = u4ScpipGetInOutVDistance();
                     bmode =u1ScpipGetDispMode(SV_VP_MAIN);
                     if(bmode == VDP_SCPOS_PSCAN_DISPMODE)
                    {
                        u4VDist = u2DrvDIGetVcnt(VDP_1);
                    }
                    else
                    {
                        u4VDist = u4ScpipGetInOutVDistance();
                    }

                     u4VTotal = (UINT32)wDrvGetOutputVTotal();
                     if((u4VDist <=SCPIP_FRAME_TRACK_LOCK_RANGE) ||(u4VDist > (u4VTotal-SCPIP_FRAME_TRACK_LOCK_RANGE)))
                         return SCPIP_FRAME_TRACK_LOCK;
                     else
                        return SCPIP_FRAME_TRACK_NOT_STALBE; 
            default:
                return SCPIP_FRAME_TRACK_NOT_STALBE;
        }
    }
    else if (u4Mode == 2)  // SW frame track
    {
#ifdef SUPPORT_SW_FRAME_CHECK
        if (_u4VDistTrackEn)
        {
            if (_u4VDistTrackState == 3)
            {
                return SCPIP_FRAME_TRACK_LOCK;
            }
            else
            {
                return SCPIP_FRAME_TRACK_NOT_STALBE;
            }
        }
        else
        {
            return SCPIP_FRAME_TRACK_DISABLE;
        }
#else
        return SCPIP_FRAME_TRACK_DISABLE;
#endif
    }    
    else
    {
        return SCPIP_FRAME_TRACK_DISABLE;
    }
    #endif
    return SCPIP_FRAME_TRACK_DISABLE;
}
#else
E_FRAME_TRACK_STAUS  eScpipGetFrameTrackStatus(void) 
{
    UINT32 u4Mode = u4GetFrameTrackMode();

    if (u4Mode == 1)  // HW Frame Track
    {
        if(RegReadFldAlign(SCPIP_PIP_OA_RO_03, PIP_OA_RO_03_RD_TRACK_LOCK))
        {
             return SCPIP_FRAME_TRACK_LOCK;
        }
         else
        {
             return SCPIP_FRAME_TRACK_NOT_STALBE; 
        }
    }
    else if (u4Mode == 2)  // SW frame track
    {
#ifdef SUPPORT_SW_FRAME_CHECK
        if (_u4VDistTrackEn)
        {
            if (_u4VDistTrackState == 3)
            {
                return SCPIP_FRAME_TRACK_LOCK;
            }
            else
            {
                return SCPIP_FRAME_TRACK_NOT_STALBE;
            }
        }
        else
        {
            return SCPIP_FRAME_TRACK_DISABLE;
        }
#else
        return SCPIP_FRAME_TRACK_DISABLE;
#endif
    }    
    else
    {
        return SCPIP_FRAME_TRACK_DISABLE;
    }
}
#endif
void vDrvFrameTrackProc(void)
{
    UINT32 u4FscOnOff, u4PscOnOff;
    static UINT32 u4PreviousState = 0xFF;
    static UINT32 u4PreviousCount = 0xFF;
  
    if (u4ForceFrameTrackCtrl == SCPIP_FRAME_TRACK_OFF_CLI)
    {
        u4FscOnOff = SV_OFF;
        u4PscOnOff = SV_OFF;
        LOG(3, "SCPIP_FRAME_TRACK_OFF_CLI\n");
    }
    else if (u4ForceFrameTrackCtrl == SCPIP_FRAME_TRACK_ON_CLI)
    {
        u4FscOnOff = SV_ON;
        u4PscOnOff = SV_ON;
        LOG(3, "SCPIP_FRAME_TRACK_ON_CLI\n");
    }
    else if (u4ForceFrameTrackCtrl == SCPIP_FRAME_TRACK_AUTO)
    {        
        u4FscOnOff = SV_ON;
        u4PscOnOff = SV_ON;

        //1. Signal unstable. Turn off frame track
        if(bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
        {
            LOG(8,"Signal unstable. Turn off frame track.\n");
            u4FscOnOff = SV_OFF;
            u4PscOnOff = SV_OFF;
        }        
        //2. Non-Standard signal. Turn Off frame track.
        if(bDrvVideoIsNonStandardSignal(SV_VP_MAIN) == SV_TRUE)
        {
            LOG(8,"Non-Standard signal. Turn Off frame track.\n");        
            u4FscOnOff = SV_OFF;
            u4PscOnOff = SV_OFF;
        }
        //3. During Auto Search. Turn Off frame track.
        if(_fgAutoSearch)
        {
            LOG(8,"During Auto Search. Turn Off frame track.\n");                
            u4FscOnOff = SV_OFF;
            u4PscOnOff = SV_OFF;
        }
        //4. During Thumbnail mode. Turn off frame track
        if (SRM_GetMmMode()==SRM_MM_MODE_TYPE_THUMBNAIL)
        {
            LOG(8,"During Thumbnail mode. Turn off frame track.\n");                
            u4FscOnOff = SV_OFF;
            u4PscOnOff = SV_OFF;
        }

        if (getScalerMode(SV_VP_MAIN) == VDP_SCPOS_LINESYNC_DISPMODE)
        {
            u4FscOnOff = SV_OFF;
            LOG(8, "VDP_SCPOS_LINESYNC_DISPMODE turn off frame track\n");
        }

        if (u4PreviousState != u4FscOnOff)
        {
            u4PreviousState = u4FscOnOff;
            u4PreviousCount = 0;
        }

        if (u4PreviousCount < 20)
        {
            u4PreviousCount++;
            u4FscOnOff = 0xFF;
        }        
    }
    else
    {
        u4FscOnOff = SV_OFF;
        u4PscOnOff = SV_OFF;
    }
    
    if (u4FscOnOff != 0xFF)
    {
        UINT8 u1InFR;
        UINT8 u1OutFR;
    
        u1InFR = bDrvVideoGetRefreshRate(SV_VP_MAIN);
        u1OutFR = u2DrvGetInternalFrameRate();
    
        if(bDrvCheckMTKGoodStatus() == SV_TRUE || u1InFR != u1OutFR)
        {
            LOG(8, "Set frame track InFR %d OutFR %d \n", u1InFR, u1OutFR);                
            vDrvSetDDDSFrameTrackMode(DDDS_FT_FROM_SCALER);
            vScpipSwitchFrameTrackOnOff(u4FscOnOff, u4PscOnOff);
            u4PreviousCount++;
        }    
    }
}

void vFrameTrackConfig(UINT32 u4Step)

{
    //1. Reconfig FSC & PSC Frame track
    vScpipFrameTrackConfig(u4Step);

    #ifdef SUPPORT_50_TO_120
    if (fg50To120 && (fgFrameTrackIsFlgSet(FRAME_TRACK_MODE_VTOTAL_BY_MJC) || fgFrameTrackIsFlgSet(FRAME_TRACK_MODE_DDDS_BY_MJC)))
    {
        // TODO: frm mask case
        //UINT32 u4InOutGCD = GCD(arIntVdoFmt[VDO_PSC_OUT].u1FrameRate, arIntVdoFmt[VDO_FSC_IN].u1FrameRate);
        vFrameTrackDDDSConfig(FRAME_TRACK_50TO60_STEP1, FRAME_TRACK_50TO60_STEP2);
        //vDrvMJCSetFrmMask((arIntVdoFmt[VDO_FSC_IN].u1FrameRate / u4InOutGCD), 0);
    }
    else
    #endif
    {
        vFrameTrackDDDSConfig(u4Step, u4Step << 1);
        #ifdef CC_MT5396
        vDrvMJCSetFrmMask(0, 0);
        #endif
    }
}

