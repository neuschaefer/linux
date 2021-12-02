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
#include "drv_scaler.h"
#include "drv_tvd.h"
#include "drv_hdtv.h"
#include "drvcust_if.h"
#include "vdp_drvif.h"
#include "vdo_misc.h"
#include "drv_display.h"
#include "nptv_debug.h"
#include "drv_di.h"

#ifdef CC_MT5395
#include "drv_mjc_if.h"
#endif

/**************************************************************************
 * Global/Static variables
 *************************************************************************/
UINT32 u4ForceFrameTrackCtrl = SCPIP_FRAME_TRACK_AUTO;
UINT32 u4ForceFrameTrackMode = 0;

#ifdef SUPPORT_SW_FRAME_CHECK
extern UINT32 _u4VDistTrackEn;
extern UINT32 _u4VDistTrackState;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/**
* @brief Get scaler input/output V sync distance count
*/
UINT32 u4ScpipGetInOutVDistance(void)
{
    return RegReadFldAlign(SCPIP_PIP1_RO_02, PIP1_RO_02_VCOUNTER_OUT_1);
}

/**
 * Get FrameTrack Mode.
 *
 * @return 0: Disable FrameTrack; 1: HW FrameTrack; 2: SW FrameTrack
 */
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


/**
    Config the V sync alignment parameters
*/
void vFrameTrackConfig(UINT32 u4LockRange, UINT32 u4LossLockRange, UINT32 u4Step) 
{
    vRegWriteFldAlign(SCPIP_PIP_OA_0E, 1, PIP_OA_0E_FRAME_SYNC_NUM);    //2 for non-standard signal, but may not lock, need fine-tune
    vRegWriteFldAlign(SCPIP_PIP_OA_0E, u4LockRange, PIP_OA_0E_LOCK_RANGE);
    vRegWriteFldAlign(SCPIP_PIP_OA_0E, 0, PIP_OA_0E_LOSS_EARLY_EN);
    vRegWriteFldAlign(SCPIP_PIP_OA_0E, u4LossLockRange, PIP_OA_0E_LOSS_LOCK_RANGE);
    vRegWriteFldAlign(SCPIP_PIP_OA_0E, u4Step, PIP_OA_0E_MOVLEN_ADJ);
    vRegWriteFldAlign(SCPIP_PIP_OA_0F, wDISPLAY_HEIGHT+5, PIP_OA_0F_RVACT);
    vRegWriteFldAlign(SCPIP_PIP_OA_0F, SV_OFF, PIP_OA_0F_FTRACK_VSYNC_SEL);
}

void vScpipFrameTrackStep(UINT8 bStep)
{    
    UINT32 u4Mode = u4GetFrameTrackMode();

    if (u4Mode == 1)  // HW Frame Track
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, bStep, PIP_OA_0E_MOVLEN_ADJ);
    }
    else if (u4Mode == 2)   // SW Frame Track
    {
        #ifdef SUPPORT_SW_FRAME_CHECK
        vSetSWFrameTrackStep((UINT32)bStep);
        #endif
    }           
}
/**
    Config the V sync alignment parameters
*/
void vScpipSwitchFrameTrackOnOff(UINT32 u4OnOff) 
{
    // can be enable when dram mode and scaler display mode
    static UINT32 u4Status=0xff;

    if(u4Status != u4OnOff)
    {
        u4Status = u4OnOff;
        LOG(5,"vScpipSwitchFrameTrackOnOff %d\n\n", u4OnOff);
    }
    else
    {
        return; // no change
    }

    #if defined(__MODEL_slt__) || defined(DRV_SUPPORT_EXTMJC) // Frame tracking always off for SLT and external MJC
    vRegWriteFldAlign(SCPIP_PIP_OA_0E, 0, PIP_OA_0E_FRAME_TRACK_EN1);
    #else
    if(u4OnOff)  // notice the order
    {    
        UINT8 bMode;
        bMode = u1ScpipGetDispMode(SV_VP_MAIN);
        if ((bMode == VDP_SCPOS_DISPMODE_AUTO ) || (bMode == VDP_SCPOS_DISPMODE_MANUAL))
        {//only set refferece Vsync after cut vporch when scaler display mode;
            vRegWriteFldAlign(SCPIP_PIP_OA_0F, SV_OFF, PIP_OA_0F_FTRACK_VSYNC_SEL);
        }
        else
        {//only set refferece Vsync after cut vporch when scaler display mode;
            vRegWriteFldAlign(SCPIP_PIP_OA_0F, SV_ON, PIP_OA_0F_FTRACK_VSYNC_SEL);
        }
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_OFF, PIP_OA_0E_FRAME_TRACK_DIRECT);  
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_ON, PIP_OA_0E_FRAME_TRACK_EN1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_OFF, PIP_OA_0E_FRAME_TRACK_EN1);
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_ON, PIP_OA_0E_FRAME_TRACK_DIRECT);
        vRegWriteFldAlign(SCPIP_PIP_OA_0F, SV_OFF, PIP_OA_0F_FTRACK_VSYNC_SEL);
    }
    #endif
}

UINT32 u4GetFrameTrackOnOff(void)
{
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
    return 0;
}

/**
    GetFrameTrackStatus: TRUE => lock, otherwise, 
*/
extern UINT32 _u4VDistTrackState;          // 1: down, 2: up, 3:locked

E_FRAME_TRACK_STAUS  eScpipGetFrameTrackStatus(void) 
{
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
}



void vDrvFrameTrackProc(void)
{
    UINT8 bType, bmode;
    UINT32 u4IsStdSig, u4ScalerJudge, u4OnOff;
    RTvdNSTDStatus rStatus;
    RHDTVNSTDStatus rHDTVStatus;
    UINT32 u4Mode;

    u4Mode = u4GetFrameTrackMode();
    if(_fgAutoSearch)
    {
        if (u4Mode == 1)
        {
            vScpipSwitchFrameTrackOnOff(SV_OFF);
        }
        else if (u4Mode == 2)
        {
            #ifdef SUPPORT_SW_FRAME_CHECK
            vSetSWFrameTrackOnOff(SV_OFF);
            LOG(3, "vDrvFrameTrackProc: _fgAutoSearch \n");
            #endif
        }            
        return;
    }
/*
    if (DRVCUST_OptGet(eFrameTrackMode) == 0)
    {
        vScpipSwitchFrameTrackOnOff(SV_OFF);
        return;
    }
*/
    if((u4ForceFrameTrackCtrl & SCPIP_FRAME_TRACK_OFF_CLI)==SCPIP_FRAME_TRACK_OFF_CLI)
    {
        u4OnOff = SV_OFF;
        LOG(9,"SCPIP_FRAME_TRACK_OFF_CLI\n\n");
    }
    else if((u4ForceFrameTrackCtrl & SCPIP_FRAME_TRACK_ON_CLI)==SCPIP_FRAME_TRACK_ON_CLI)
    {
        u4OnOff = SV_ON;
        LOG(9,"SCPIP_FRAME_TRACK_ON_CLI\n\n");
    }
    else if((u4ForceFrameTrackCtrl==SCPIP_FRAME_TRACK_AUTO) && (bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE))
    {
        static UINT32 _u4PreviousState = 0xff;
        static UINT32 _u4PreviousCount = 0;
        if (bDrvCheckMTKGoodStatus())
        {
            // DDDS is lock and not non-std signal, then try to enable frame track.
            //UINT8 bRefreshRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
            UINT32 u4NonFRC = 1;
            bType = bGetVideoDecType(SV_VP_MAIN);
            if (bType == SV_VD_TVD3D)
            {
                vTvd3dGetNSTDStatus(&rStatus);
                u4IsStdSig= (UINT32)(!rStatus.fgIsNSTD);
             }
             else if (bType == SV_VD_YPBPR)
             {
              vHDTVGetNSTDStatus(&rHDTVStatus);
              u4IsStdSig= (UINT32)(!rHDTVStatus.fgIsNSTD);
             }
            else
            {
                u4IsStdSig = SV_TRUE;
            }
            
            //if ((bRefreshRate == 24) || (bRefreshRate == 25) || (bRefreshRate == 30))
            //if(bDrvVideoGetRefreshRate(SV_VP_MAIN) != vDrvGetLCDFreq())
            if(bDrvVideoGetRefreshRate(SV_VP_MAIN) != u2DrvGetInternalFrameRate())
            {
                u4NonFRC = 0;
            }

            bmode =u1ScpipGetDispMode(SV_VP_MAIN);
            #ifdef CC_MT5395
            if (u4Mode == 1) //HW frame track
            {
                //enable frame track only in scaler display mode
                u4ScalerJudge= ((bmode == VDP_SCPOS_DISPMODE_AUTO) ||(bmode == VDP_SCPOS_DISPMODE_MANUAL))?1:0;  //enable frame track only in scaler display mode
            }
            else // SW frame track
            {
                #ifdef SUPPORT_SW_FRAME_CHECK
                // enable scaler frame track in scaler display mode, enable Pscan frame track in pscan display mode
                //u4ScalerJudge = ((bmode == VDP_SCPOS_DISPMODE_AUTO) || (bmode == VDP_SCPOS_DISPMODE_MANUAL) || (bmode == VDP_SCPOS_PSCAN_DISPMODE))? 1 : 0; 
                u4ScalerJudge = 1; 
                if (u1DrvMJCIsOnOff() && (!u1DrvMJCIsRepeatMode()))
                {
                    UINT32 u4VideoVTotal;
                    UINT32 u4TargetDist;
                    u4VideoVTotal = wDrvVideoGetVTotal(SV_VP_MAIN);    
                    //vSetSWFrameTrackLockRange(0x800 + u4VideoVTotal - SCPIP_FRAME_TRACK_MJC_LOCK_RANGE, 0x800 + SCPIP_FRAME_TRACK_MJC_LOCK_RANGE);
                    u4TargetDist = 0x800 + (u4VideoVTotal >> 1);
                    vSetSWFrameTrackLockRange(u4TargetDist, u4TargetDist);

                    if ((bDrvVideoGetRefreshRate(SV_VP_MAIN) == 50) ||
                        (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 60))
                    {
                        u4NonFRC = SV_TRUE;
                    }
                    else
                    {
                        u4NonFRC = SV_FALSE;
                    }
                }
                else
                {
                    vSetSWFrameTrackLockRange(0x800, 0x800);
                }
                #endif
            }
            #else
            vSetSWFrameTrackLockRange(0x800, 0x800);
            u4ScalerJudge= ((bmode == VDP_SCPOS_DISPMODE_AUTO) ||(bmode == VDP_SCPOS_DISPMODE_MANUAL))?1:0;  //enable frame track only in scaler display mode
            #endif
            u4OnOff = u4IsStdSig && u4ScalerJudge && u4NonFRC;
            LOG(9,"SCPIP_FRAME_TRACCK_AUTO[%d] u4IsStdSig[%d] u4ScalerJudge[%d] u4NonFRC[%d]\n\n",u4OnOff, u4IsStdSig, u4ScalerJudge, u4NonFRC);
        }
        else
        {
            u4OnOff = SV_OFF;    
            //LOG(3, "vDrvFrameTrackProc : DDDS unlock\n"); 
        }

        // To avoid Frame Track Toggle quickly
        if (_u4PreviousState != u4OnOff)
        {
            _u4PreviousState = u4OnOff;
            _u4PreviousCount = 0;
        }
        else if (_u4PreviousCount < 0xffffffff)
        {
            _u4PreviousCount ++;
        }

        if (_u4PreviousCount < DRVCUST_OptGet(eFrameTrackStableCnt))
        {
            u4OnOff = 0xff; // ignore
        }
    }
    else
    {
        u4OnOff=SV_OFF;
        LOG(9,"SCPIP_FRAME_TRACK_OFF\n\n");
    }

    if (u4OnOff != 0xff)
    {
        if (u4Mode == 1)
        {
            vScpipSwitchFrameTrackOnOff(u4OnOff);
        }
        else if (u4Mode == 2)
        {
            #ifdef SUPPORT_SW_FRAME_CHECK
            vSetSWFrameTrackOnOff(u4OnOff);
            #endif
        }                
    }
}

