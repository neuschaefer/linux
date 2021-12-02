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
 * $RCSfile: slt_vdp.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file slt_vdp.c
 *  This file includes SLT driver.
 */

#ifdef __MODEL_slt__

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drv_common.h"
#include "x_os.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_drv_cli.h"

#include "sv_const.h"
#include "nptv_if.h"
#include "hw_vdoin.h"
#include "video_def.h"
#include "hw_scpos.h"
#include "drv_ycproc.h"
#include "hw_ycproc.h"
#include "pe_if.h"
#include "drv_di.h"
#include "drv_scaler.h"
#include "drv_video.h"
#include "mute_if.h"
#include "drv_contrast.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VDO_NS 3
#define MPEG_NS 70
#define PTS_START 0x36b3
#define PTS_END 0x337b8

#define SLT_FREEZE_DONE 0
#define SLT_FREEZE_HDMI 1
#define SLT_FREEZE_MPEG 2
#define SLT_TEST_VGA 3

// dirty copy from vdp_scaler_hw.h
#define VDP_VD_ENABLE 2

#define VGA_X_POS 16
#define VGA_THRASHOLD 50
#define SLT_FREEZE_TIMEOUT 10000

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4SltCmd = SLT_FREEZE_DONE;
static UINT32 _u4FramePts = 0;
static UINT32 _u4HdmiField = 0;
static UINT32 _u4FreezeVdp = 0;
static INT32 _i4Bypass = 0;
#if 0 
static UINT32 _u4VsyncCounter = 0;
static UINT32 _u4VgaCompareHeight = 479;
static UINT32 _u4VgaDataLength = 1936;
static UINT32 _u4VgaFrameAddr1 = 0x36808000;
static UINT32 _u4VgaFrameAddr2 = 0x36ab1540;
#endif
static HANDLE_T _hFreezeSema = NULL_HANDLE;

static UINT32 _u4Freeze = 0;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define _DMA_R_INDEX(x, base_address) \
    base_address + ((x * 3 + 2) & ~15) + (15 - ((x * 3 + 2) % 16))


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void SLT_BypassPQFunc(UINT8 u1VdpId, INT32 bypassMask);

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SLT_Init(void)
{
    if (_hFreezeSema == NULL_HANDLE)
    {
        /* although the function only exists in debug version,
           for not being confused, use VERIFY instead */
        VERIFY(x_sema_create(&_hFreezeSema, X_SEMA_TYPE_BINARY,
               X_SEMA_STATE_LOCK) == OSR_OK);
    }
}

#if 0
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 SLT_GetCommand(void)
{
    return _u4SltCmd;
}
#endif

UINT32 SLT_GetFreeze(void)
{
    return _u4Freeze;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

BOOL SLT_IsSignalStable(UINT32 u4Vdp)
{
    if(bDrvVideoSignalStatus(u4Vdp) == SV_VDO_STABLE &&
       _u4DrvVideoGetMute(u4Vdp) == 0 /* un-muted */)
    {
        return TRUE;
    }
    return FALSE;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SLT_WaitFreezeDone(void)
{
    if (_u4SltCmd != SLT_FREEZE_DONE)
    {
        x_sema_lock(_hFreezeSema, X_SEMA_OPTION_WAIT);
	//x_sema_lock_timeout(_hFreezeSema, SLT_FREEZE_TIMEOUT);
        // 2007/3/27
        // because we use vApiVideoFreeze
        // to wait enough time for vsync-align register update
        x_thread_delay(500);
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
extern UINT32 b2rState;
//static UINT32 _u4Top;
void SLT_CurrentPts(UINT32 u4Type, UINT32 u4Pts)
{
    HAL_TIME_T _rTime;
    if ((_u4SltCmd == SLT_FREEZE_MPEG) && (u4Pts == _u4FramePts))
    {
#if 1
    //HAL_GetTime(&_rTime);
    //Printf("SLT: PTS %d (%u.%6u)\n", u4Pts, _rTime.u4Seconds, _rTime.u4Micros);
    //Printf("2-%06u\n", _rTime.u4Micros);
    b2rState = 2;
#endif

#if 0
        if(_u4FreezeVdp == 0) // main
        {
            _u4Top = IO_READ32(PSCAN_BASE, 0x504) & 0x1000; //0x20024504[12]
        }
        else // pip
        {
            _u4Top = IO_READ32((IO_VIRT+0x2C000), 0x504) & 0x1000; //0x2002C504[12]
        }
#endif

        if (_u4FreezeVdp == 0)
        {
            //turn off scaler write ctrl (Main)
            //vWriteSCPOSMsk(RW_PIP_PPCTL, 0, W1_EN); 
            SLT_BypassPQFunc(0, _i4Bypass);
            //CLI_Parser("vdp.s.overscan 0 25 25 0 0"); // to avoid error lines at the top
            //x_thread_delay(20);            
            //vRegWriteFldAlign(SCALER_PIP_PIP1_09, 0, W_EN_1);
        }
        else
        {
            //turn off scaler write ctrl (PIP)           
            //vWriteSCPOSMsk(RW_PIP_PPCTL, 0, W2_EN);
            SLT_BypassPQFunc(1, _i4Bypass);
            //CLI_Parser("vdp.s.overscan 1 25 25 0 0");            
            //x_thread_delay(20);
            //vRegWriteFldAlign(SCALER_PIP_PIP2_09, 0, W_EN_2);
        }       
        //x_thread_delay(100); // wait PQ setting done
        /*
        if (_u4FreezeVdp == 0)
        {
            vWriteSCPOSMsk(RW_PIP_PPCTL, 0, W1_EN);
        }
        else
        {
            vWriteSCPOSMsk(RW_PIP_PPCTL, 0, W2_EN);
        }        
        */
        
        // freese SCPOS here...
        //vDrvMainWriteCtrl(FALSE);
        vScpipWriteCtrlOnOff(VDP_1, SV_OFF);
		vDrvAutoContrastFreeze(1);
		vScpipSetFreeze(_u4FreezeVdp, 1);
        //CLI_Parser("vdp.f 0 1");
        //vWriteVDOINMsk(OMUX00, 0x10, 0xf0, 0);

        HAL_GetTime(&_rTime);
        //Printf("SLT: Match PTS = %d (%d, %d)\n", u4Pts, _rTime.u4Seconds, _rTime.u4Micros);
        _u4SltCmd = SLT_FREEZE_DONE;

        _u4Freeze = 1;
        
        if (_hFreezeSema != NULL_HANDLE)
        {
            x_sema_unlock(_hFreezeSema);
        }
    }
#if 1
    //HAL_GetTime(&_rTime);
    //Printf("SLT: PTS %d (%u.%6u)\n", u4Pts, _rTime.u4Seconds, _rTime.u4Micros);
    //Printf("2-%06u\n", _rTime.u4Micros);
    b2rState = 3;
#endif
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
extern void vScpipSetDramWrite(UCHAR u4VdpId, UCHAR ucOnOff);

void SLT_CheckHdmiField(UINT32 u4Field)
{
    if ((_u4SltCmd == SLT_FREEZE_HDMI) && (u4Field == _u4HdmiField) &&
        SLT_IsSignalStable(_u4FreezeVdp))
    {
        if (_u4FreezeVdp == 0)
        {
            vScpipSetDramWrite(VDP_1,SV_OFF);                  // turn off write enable for scaler main
        }
        else
        {
            vScpipSetDramWrite(VDP_2,SV_OFF);                  // turn off write enable for scaler sub
        }       
        //VDP_SetFreeze(_u4FreezeVdp, TRUE);
		vDrvAutoContrastFreeze(1);
		vScpipSetFreeze(_u4FreezeVdp, 1);
        // disable inpur source mux
/*        if (_u4FreezeVdp == 0)
        {
            vWriteVDOINMsk(OMUX00, 0x10, 0xf0, 0);
        }
        else
        {
            vWriteVDOINMsk(OMUX00, 0x01, 0x0f, 0);
        }*/

        _u4SltCmd = SLT_FREEZE_DONE;
        if (_hFreezeSema != NULL)
        {
            x_sema_unlock(_hFreezeSema);
        }
    }
}


#if 0 //def CC_MT5360
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SLT_CheckVgaFrame(void)
{
    static volatile UINT8* pu1Frame = (volatile UINT8*)0;

    if (_u4SltCmd == SLT_TEST_VGA)
    {
        UINT32 u4BaseAddr = 0, u4Line = 0;
        UINT8 u1Value;
        // default is 0, so we will start with frame1
        if ((UINT32)pu1Frame == _u4VgaFrameAddr1)
        {
            (UINT32)pu1Frame = _u4VgaFrameAddr2;
        }
        else
        {
            (UINT32)pu1Frame = _u4VgaFrameAddr1;
        }
        _u4VsyncCounter++;
        for (u4Line = 0; u4Line < _u4VgaCompareHeight; u4Line++)
        {
            u1Value = pu1Frame[_DMA_R_INDEX(VGA_X_POS, u4BaseAddr)];
            if (u1Value < VGA_THRASHOLD)
            {
                vApiVideoFreeze(_u4FreezeVdp, TRUE);
                //_VDP_vDrvMainWriteCtrl(FALSE);
                _u4SltCmd = SLT_FREEZE_DONE;
                _fgVgaPass = FALSE;
                Printf("SLT_CheckVgaFrame catch, vsync=%d, line=%d, value=%d\n",
                       _u4VsyncCounter, u4Line, u1Value);
                _u4VsyncCounter = 0;
                return;
            }
            u4BaseAddr += _u4VgaDataLength;
        }
    }
}

#endif    

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SLT_FreezeMpegFrame(UINT32 u4FreezeVdp, UINT32 u4FramePts, INT32 bypassMask)
{
    _u4FramePts = u4FramePts;
    _u4FreezeVdp = u4FreezeVdp;
    _u4SltCmd = SLT_FREEZE_MPEG;
    _i4Bypass = bypassMask;
    _u4Freeze    = 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SLT_FreezeHdmiFrame(UINT32 u4FreezeVdp, UINT32 u4Field)
{
    _u4FreezeVdp = u4FreezeVdp;
    _u4HdmiField = u4Field;
    _u4SltCmd = SLT_FREEZE_HDMI;
}

#if 0

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

BOOL SLT_TestVga(BOOL fgEnable)
{
    if (fgEnable)
    {
        UINT32 u4Value;
        // skip last line
#if 0        
        u4Value = (bReadSCPOS(RW_PIP_MPVL) << 2) |
                  (bReadSCPOSMsk(RW_PIP_MPOSLEN, MVLL) >> 4);
        _u4VgaCompareHeight = (u4Value * 2) - 1;

        u4Value = (bReadSCPOS(RW_PIP_MPHL) << 2) |
                  bReadSCPOSMsk(RW_PIP_MPOSLEN, MHLL);
        _u4VgaDataLength = u4Value * 16;

        u4Value = (bReadSCPOSMsk(RW_PIP_MPAGE1H, W1STARTP121_16) << 16) |
                  (bReadSCPOS(RW_PIP_MPAGE1M) << 8) |
                  bReadSCPOS(RW_PIP_MPAGE1L);
         _u4VgaFrameAddr1 = u4Value * 64;

         u4Value = (bReadSCPOSMsk(RW_PIP_MPAGESAH, W2INC21_16) << 16) |
                   (bReadSCPOS(RW_PIP_MPAGESAM) << 8) |
                   bReadSCPOS(RW_PIP_MPAGESAL);
#endif         
        _u4VgaFrameAddr2 = _u4VgaFrameAddr1 + (u4Value * 64);

        _fgVgaPass = TRUE;
        _u4SltCmd = SLT_TEST_VGA;
    }
    else
    {
        _u4SltCmd = SLT_FREEZE_DONE;
    }
    return _fgVgaPass;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

BOOL SLT_GetInputCalibrationPass(void)
{
    return _fgInputCalibrationPass;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SLT_SetInputCalibrationPass(BOOL fgPass)
{
    _fgInputCalibrationPass = fgPass;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SLT_FreezeTopField(void)
{
    // if top now, busy waiting for bottom
    while ((IO_READ32(PSCAN_BASE, 0x3c) & 0x4) == 0)
    {
        //Printf("2");
    }

    // if bottom now, busy waiting for top
    while ((IO_READ32(PSCAN_BASE, 0x3c) & 0x4) == 0x4)
    {
        //Printf("3");
    }

    // top now
    //vApiVideoFreeze(0, TRUE);
}
#endif

void SLT_BypassPQFunc(UINT8 u1VdpId, INT32 bypassMask)
{
    if(u1VdpId == 0)
    {
#if 0        
        // bit [0]
        //Set MDDI Bob mode
        if(bypassMask & 0x1)
        {
            vDrvDIDataMode(0, E_DI_YCBOB);
        }
        // bit [1]
        //MDDI on/off
        if(bDrvVideoIsSrcInterlace(0))
        //{
           vDrvDIOnOff(0, !(bypassMask & 0x2));
        }
#endif    
        // bit [2]
        // NR : 應該可以解決大片差異的問題
        if(bypassMask & 0x4)
        {
            //vApiVideo3DNROnOff(SV_OFF);//build error
        }
        if (bypassMask & 0x8)
        {
            //vApiVideoNRTIIROnOff(SV_OFF);//build error
        }
        if (bypassMask & 0x10)
        {
            //vApiVideo2DNROnOff(SV_OFF);//build error
        }
        // bit [3]
        // Sharpness : 應該可以解決邊緣差異的問題
        if(bypassMask & 0x20)
        {
            vDrvPreSharpOnOff(VDP_1, SV_OFF);
        }
        if (bypassMask & 0x40)
        {
            vDrvPostSharpOnOff(SV_OFF);
        }
        // bit [4]
        // CTI : 應該可以解決滲色問題
        if (bypassMask & 0x80)
        {
            vDrvCTIFOnOff(SV_OFF);
        }
        // bypass scaling
        if(bypassMask & 0x100)
        {
            CLI_Parser_Arg("vdp.s.121 0 1");
        }
    }
    else 
    {
#if 0     
        // bit [0]
        //Set MDDI Bob mode
        if(bypassMask & 0x1)
        {
            vDrvDIDataMode(1, E_DI_YCBOB);
        }
        // bit [1]
        //MDDI on/off
        if(bDrvVideoIsSrcInterlace(1))
        {        
            vDrvDIOnOff(1, !(bypassMask & 0x2));
        }
#endif        
        // bit [2]
        // NR : 應該可以解決大片差異的問題
        if(bypassMask & 0x4)
        {
            //vApiVideo3DNROnOff(SV_OFF);//build error
        }
        if (bypassMask & 0x8)
        {
            //vApiVideoNRTIIROnOff(SV_OFF);//build error
        }
        if (bypassMask & 0x10)
        {
            //vApiVideo2DNROnOff(SV_OFF);//build error
        }
        // bit [3]
        // Sharpness : 應該可以解決邊緣差異的問題
        if(bypassMask & 0x20)
        {
            vDrvPreSharpOnOff(VDP_2, SV_OFF);
        }
        if (bypassMask & 0x40)
        {
            vDrvPostSharpOnOff(SV_OFF);
        }
        // bit [4]
        // CTI : 應該可以解決滲色問題
        if (bypassMask & 0x80)
        {
            vDrvCTIFOnOff(SV_OFF);
        }
        // bypass scaling
        if(bypassMask & 0x100)
        {
            CLI_Parser_Arg("vdp.s.121 1 1");
        }
    }
}


#endif
