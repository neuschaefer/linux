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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: vdo_isr.c $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   Video Interrupt Service Routine
 *
 * Author:
 * -------
 *
 * Last changed:
 * -------------
 * $Author: pingan.du $
 *
 * $Modtime: 04/05/17 6:55p $
 *
 * $Revision: #2 $
****************************************************************************/

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "x_os.h"
#include "x_assert.h"

#include "general.h"
#include "hw_vdoin.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "drv_di.h"
#include "drv_display.h"
#include "drv_tvd.h"
#include "drv_hdtv.h"
#include "drv_vga.h"
#include "drv_dvi.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "drv_ycproc.h"
#include "drv_contrast.h"
#include "drv_async.h"
#include "vdp_drvif.h"
#include "pmx_if.h"
#include "drv_video.h"
#include "nptv_debug.h"
#include "api_backlt.h"
#include "c_model.h"
#include "drv_di.h"
#include "source_select.h"
#include "tve_if.h"
#include "drv_lvds.h"
#include "drv_od.h"
#include "osd_drvif.h"
#include "pe_if.h"
#ifdef SUPPORT_LOCK_FROM_SRVAD
#include "drv_tcon.h"
#endif
#if defined(CC_MT5396)
#include "drv_mjc_if.h"
#endif
#ifdef CC_SCPOS_3DTV_SUPPORT
#include "hw_mlvds.h"
#endif

#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif

#if SUPPORT_VBI
#include "drv_vbi.h"
#endif
//mtk01140 20060615 get histogram
#include "hw_ycproc.h"
extern UINT8 bAutoContrastStatus;
extern UINT8 _Lvds7To4FifoReset;
#ifdef CC_UP8032_ATV
extern UINT8  _bBlankMuteCnt;
#endif

#ifdef __MODEL_slt__
#include "slt_if.h"
#endif

#if defined(CC_MT5368) || defined(CC_MT5396)
void vLDMHW_ISR(void);
#endif

#ifdef CC_MT5396
extern UINT32 _u4VdpCmdDelay;
#endif

//EXTERN void vUtDelay1ms(UINT8 bValue);
EXTERN void PMX_OnOutputVSync(void);
EXTERN void vDrvDIVSyncCntIncr(UINT8 u1VdpId);

#ifdef CC_SCPOS_3DTV_SUPPORT
EXTERN INT32 GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val);
#endif

#ifndef __KERNEL__
EXTERN void CopyScalerDRAM(void);
#endif

#if CC_SCPOS_MUTE_RELATCH_WA
UINT32 _u4PreviousMuteMask[2] = {0, 0};
#endif

static UINT32 u4VdoinInt=0;
#ifdef CC_MT5396// reserved for 120Hz Panel
static UINT8 _u1VSyncCount =0;  //120Hz Mloop -> 60Hz MLoop
#endif

BOOL fgIsDitherInited = SV_FALSE;

PRIVATE void vClrVdoinInt(UINT32 u4mask)
{
    vRegWrite4B(INT_COLLECT, u4mask);
}

PRIVATE void vStoreVdoinInt(void)
{
    u4VdoinInt = u4RegRead4B(INT_COLLECT);
}

PRIVATE UINT32 u4LoadVdoinInt(void)
{
    return u4VdoinInt;
}

PRIVATE void vResetVdoinInt(void)
{
    vClrVdoinInt(u4VdoinInt);
}

BOOL fgIsVdoinInt(UINT32 u4mask)
{
    return (0 != (u4LoadVdoinInt()&u4mask));
}

// *********************************************************************
// Function : void vMVsyncGeneralChk(void)
// Description : The General Interrupt service routine of Main Input Source
// Parameter : UINT16 wStatus: The Interrupt Status
// Return    : void
// *********************************************************************
PRIVATE void vMVsyncGeneralChk(void)
{
    #ifdef CC_SCPOS_SW_ISR
        if (1)
    #else
        // Deal With Input Source VSYNC
        if (fgIsVdoIntMainDet())	// Main Video Input Src
    #endif
        {
            if (_rMChannel.bIsChannelOn == SV_ON)
            {
                #ifdef __MODEL_slt__
                //if (fgIsMainDVI() || fgIsMainYPbPr() || fgIsMainTvd3d())
                {
                    //check odd even field
                    SLT_CheckHdmiField(IO_READ32MSK(PSCAN_BASE, 0x080, 0x20000000)!= 0);
                }
                #endif
                vScpipOnInputVSync(SV_VP_MAIN);
            }
        }
}

// *********************************************************************
// Function : void vPVsyncGeneralChk(void)
// Description : The General Interrupt service routine of Pip Input Source
// Parameter : UINT16 wStatus: The Interrupt Status
// Return    : void
// *********************************************************************
PRIVATE void vPVsyncGeneralChk(void)
{
    //UINT8 bSigStatus;

    #ifdef CC_SCPOS_SW_ISR
        if (1)
    #else
        // Deal With Input Source VSYNC
        /*lint -save -e506 -e774*/
        if (fgIsVdoIntPipDet()) // Pip Video Input Src
        /*lint restore */
    #endif
        {
            if (_rPChannel.bIsChannelOn == SV_ON)
            {
#ifdef __MODEL_slt__
                //if (fgIsPipDVI()|| fgIsPipYPbPr() || fgIsPipTvd3d())
                {
                    // check odd/even field
                    SLT_CheckHdmiField(IO_READ32MSK(PSCAN_BASE, 0x880, 0x20000000)!= 0);
                }
#endif		
                vScpipOnInputVSync(SV_VP_PIP);
        }
    }
}


// *********************************************************************
// Function : void vInputVSyncISR(UINT8 bDecType)
// Description :
// Parameter :
// Return    :
// *********************************************************************
PRIVATE void vInputVSyncISR(UINT8 bDecType)
{
//UINT8 bInputType;                  //MC20081115 add for SCART
 
	switch (bDecType)
	{
		case SV_VD_TVD3D:
			vTvd3dVSyncISR();
			break;
		case SV_VD_YPBPR:
			vHdtvISR();
			break;
		case SV_VD_VGA:
			vVgaISR();
			break;
	#if SUPPORT_DVI
		case SV_VD_DVI:
			vDviISR();
			break;
	#endif
		default:
		break;
	}

    //bInputType   = bGetICInputType(SV_VP_MAIN);                     //MC20081115 add for SCART ***
    //if((bInputType==P_FB0) ||(bInputType==P_FB1))
    if(fgIsMainScart())
    {
        vSetSP0Flg(SP0_VGA_AUTO_FLG | SP0_AUTOCOLOR_FLG);
    }                                                                                            //MC20081115 add for SCART &&&

    vApiApplyVideoEvent(PEQTY_APPLY_TIME_INPUT_VSYNC_ISR);
    
    DRVCUST_VdoInISR();
    
    vApiIsrLog(NPTV_ISR_VDOIN);    
}

// *********************************************************************
// Function : void vOutVSyncISR(void)
// Description : General check in Output Vsync
// Parameter : None
// Return    : None
// Remark    : 8205 Need ECO to provideo this VSYNC
// *********************************************************************
extern void vMLoopUnlockVSyncSem(void);

PRIVATE void vOutVSyncISR(void)
{
    vApiApplyVideoEvent(PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    
#ifndef CC_MT5396
        vDrvADLProc(); 
#endif

    vDrvSCEFindColor();

    // dither need to be inited in vsync in case of clock violation that would cause dither fail.
    if(!fgIsDitherInited)
    {
        vDrvVideoDitherInit();
        //fgIsDitherInited = SV_TRUE;    //move to vDrvVideoDitherInit();
    }

    #ifdef CC_MT5396// reserved for 120Hz Panel
    if ((vDrvGetLCDFreq() < 77) || (_u1VSyncCount==0))
    {
        vMLoopUnlockVSyncSem();
        _u1VSyncCount =1;
    }
    else
    {
        _u1VSyncCount =0;
    }
    #else
    vMLoopUnlockVSyncSem();
    #endif

    OSD_UnlockVsyncSema();     

    DRVCUST_OutVSyncISR();

    if (IS_PANEL_PDP)
    {
	vDrvChkVcounter();
	vDrvVsyncHFpAdj();
    }

#ifdef CC_UP8032_ATV
    // handle fast blank mute
    if (_bBlankMuteCnt != 0) 
    {
        _bBlankMuteCnt--;
        if (_bBlankMuteCnt == 0)
        {
        	_bBlankMuteCnt = 0;
        	//fast blank off
        	vRegWriteFldAlign(MUTE_00, 0,  R_MUTE_POST_EN);
        }
    }
#endif
        #if CC_SCPOS_MUTE_RELATCH_WA
        if ((_u4PreviousMuteMask[SV_VP_MAIN] == 0) && u4ScpipGetMuteMask(SV_VP_MAIN))
        {
            LOG(3, "Mute Take Effect in Main\n");
            vScpipSetHwMuteStatus(SV_VP_MAIN, SV_ON);
        }
        else if (u4ScpipGetMuteMask(SV_VP_MAIN) == 0)
        {
            vScpipSetHwMuteStatus(SV_VP_MAIN, SV_OFF);
        }
        _u4PreviousMuteMask[SV_VP_MAIN] = u4ScpipGetMuteMask(SV_VP_MAIN);
        if ((_u4PreviousMuteMask[SV_VP_PIP] == 0) && u4ScpipGetMuteMask(SV_VP_PIP))
        {
            LOG(3, "Mute Take Effect in Sub\n");
            vScpipSetHwMuteStatus(SV_VP_PIP, SV_ON);
        }
        else if (u4ScpipGetMuteMask(SV_VP_PIP) == 0)
        {
            vScpipSetHwMuteStatus(SV_VP_PIP, SV_OFF);
        }
        _u4PreviousMuteMask[SV_VP_PIP] = u4ScpipGetMuteMask(SV_VP_PIP);
        #endif
        if (_Lvds7To4FifoReset)
        {
            _Lvds7To4FifoReset --;
            if (_Lvds7To4FifoReset == 0)
            {
                vDrvLVDS7To4FifoEnable();
            }
        }
        #ifdef SUPPORT_SW_FRAME_CHECK
        if(bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE)
        {
            vDrvVDistanceCheck();
        }
        #endif

        #ifdef CC_MT5396
        if (_u4VdpCmdDelay)
        {
            _u4VdpCmdDelay--;
        }
        #endif
        
#ifdef SUPPORT_LOCK_FROM_SRVAD
		vDrvTconOutVsyncISR();
#endif

    vApiIsrLog(NPTV_ISR_VDOOUT);    
}


// *********************************************************************
// Function : void vVdoInISR(void)
// Description : The Interrupt service routine of Video
// Parameter :
// Return    :
// *********************************************************************
void vVdoInISR(void)
{
    vStoreVdoinInt();
    
    if (fgIsBypassFlgSet(BYPASS_INPUT_ISR))
    {
         vResetVdoinInt();
        return ;
    } 

    // INT_MAIN_DET
    if (fgIsMainDec())
    {
        vMVsyncGeneralChk();		
    }
    // INT_PIP_DET
    if (fgIsPipDec())
    {
        vPVsyncGeneralChk();
        #if defined(TVE_WA) //&& !defined(CC_MT5363)
        TVE_OnInputVSync();
        #endif
    }

    // Call MainChannel Decoder Service Routine
    if (_rMChannel.bIsChannelOn == SV_ON)
    {
        vInputVSyncISR(_rMChannel.bDecType);
    }
    // Call PipChannel Decoder Service Routine
    if (_rPChannel.bIsChannelOn == SV_ON)
    {
        /* main/sub are different, run the fe interrupt service routine */
        if(_rMChannel.bDecType != _rPChannel.bDecType)
        {
            vInputVSyncISR(_rPChannel.bDecType);
        }
    }

    // VBI Interrupt Service Routine
    #if SUPPORT_VBI
        VBI_ISR();
    #endif
    
    vResetVdoinInt();
}


void vVdoOutISR(void)
{

    //time 0 measure nptv enter period time.
    SYS_MEASURE_TIME_END(SV_OUT_VSYNC_TIME);
    SYS_PRINT_TIME(SV_OUT_VSYNC_TIME); 
    SYS_MEASURE_TIME_START(SV_OUT_VSYNC_TIME);    

    if (fgIsBypassFlgSet(BYPASS_OUTPUT_ISR))
    {
        return ;
    }

#ifndef __KERNEL__
	CopyScalerDRAM();	
#endif

    OD_OnOutputVSync();
    OSTG_OnOutputVSync();
    #ifndef CC_UP8032_ATV
    PMX_OnOutputVSync();
    #endif

    vOutVSyncISR();

#if defined(CC_MT5368) || defined(CC_MT5396)
    // add by Polar Chou for LCDIM SPI software mode VSYNC interrupt
//    vLDMHW_ISR();
#endif
}

#ifdef CC_COPLAT_MT82
UINT8 _output_vsync;
void vMLoopUnlockVSyncSem(void)
{
    _output_vsync = TRUE;
}
#endif
