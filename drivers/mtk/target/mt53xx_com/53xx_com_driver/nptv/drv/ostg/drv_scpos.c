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
 * $RCSfile: drv_scpos.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _DRV_SCPOS_C_
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#ifndef CC_COPLAT_MT82
#include "x_os.h"
#include "x_hal_arm.h"
#endif
#include "x_hal_5381.h"
#include "x_printf.h"
#include "general.h"
#include "hw_vdoin.h"
#include "hw_scpos.h"
#include "hw_dvi.h"
#include "hw_ckgen.h"
#include "hw_ycproc.h"
#include "hw_ospe.h"
#include "drv_scaler.h"
#include "drv_di.h"        
#include "drv_scpos.h"
#include "drv_video.h"
#include "drv_dram.h"
#include "drv_upscaler.h"
#include "drv_display.h"
#include "video_def.h"
#include "drvcust_if.h"
#include "srm_drvif.h"
#include "mute_if.h"
#include "scpos_debug.h"
#include "util.h"
#include "dram_map.h"
#include "source_table.h"
#include "source_select.h"
//#include "nptv_debug.h"

#if defined(CC_SUPPORT_TVE)
#include "tve_if.h"
#endif

#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if 0
#ifndef __KERNEL__
#define HAL_5371_SCPOS_REG                  (0x20025000)
#else
#define HAL_5371_SCPOS_REG                  (0xf0025000)
#endif
#endif

#define DFT_DS_FACTOR                       (0x8000)

// added by Peggy 20071003
#ifdef SUPPORT_DRAM_DUMP
#define DFT_DS_FACTOR                       (0x8000)
#define DFT_US_FACTOR                       (0x8000)
#define DFT_DS_BIT_NUM                      (15)
#define DFT_US_BIT_NUM                      (15)
#define ADDR_STACK_SIZE 20

UINT16 wHPosOri;
UINT16 wVPosOri;

/**************************************************************************
 * Global/Static variables
 *************************************************************************/
UINT8  _u1StackSize;  // FIXME: need to init.
UINT16  _au2AddrStack[ADDR_STACK_SIZE];
UINT32  _au4ValueStack[ADDR_STACK_SIZE];
UINT32 u4ForceFrameTrackCtrl = SCPIP_FRAME_TRACK_AUTO;
UINT32 u4ForceFrameTrackMode = 0;
extern UINT32 _u4OSTGDelta;

#ifdef SUPPORT_SW_FRAME_CHECK
extern UINT32 _u4VDistTrackEn;
extern UINT32 _u4VDistTrackState;
#endif
/**************************************************************************
 * Function Members
 *************************************************************************/
PRIVATE UINT8 bRecoverDRAMDumpStack(void);
PRIVATE UINT8 bGetScalerDRAMData(UINT32 wX, UINT32 wY, UINT8 bIs10bitMode, UINT8 bIs444, UINT32 dwAddr, UINT32 wMHL, UINT32 *pu2VR, UINT32 *pu2UB, UINT32 *pu2YG);
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#if 0
#define SCPOS_8BIT_MASK_W(reg, value, mask)  \
    (_vpu4ScposHwReg[(reg) >> 2] =  \
        (_vpu4ScposHwReg[(reg) >> 2] & (~(((UINT32)(mask) << (((reg) & 3) << 3))))) | \
        ((UINT32)((mask) & (value)) << (((reg) & 3) << 3)))

#define SCPOS_8BIT_W(reg, value)            SCPOS_8BIT_MASK_W(reg, value, 0xFF)

#define SCPOS_8BIT_R(reg)                   ((_vpu4ScposHwReg[(reg) >> 2] >> (((reg) & 3) << 3)) & 0xFF)

#define SCPOS_REG(reg)                      (_vpu4ScposHwReg[(reg) >> 2])
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_SCPOS_MUTE_MASK
UINT32 _u4MainMuteMask = 0;
UINT32 _u4PipMuteMask = 0;
UINT32 _au4MuteMaskEn[2] = {1, 1};
#else
static BOOL _fgMainMute = 0;
static BOOL _fgSubMute = 0;
static BOOL _fgMainForceMute = 0;
static BOOL _fgSubForceMute = 0;
#endif

#if CC_SCPOS_MUTE_RELATCH_WA
UINT32 _au4HwMuteStatus[2] = {0, 0};
#endif

#ifdef CC_COPLAT_MT82
BOOL _fgPIP,_fgLRPOP;
#endif

UINT8 bScpipMirrorEnable[2]= {SV_FALSE, SV_FALSE};
UINT8 bScpipFlipEnable[2] = {SV_FALSE, SV_FALSE};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _VDP_HorizontalPreScaleDownFactor.
 */
//-----------------------------------------------------------------------------
static void _vDrvSetSyncAdj(void)
{
    	vRegWriteFldAlign(SCPIP_PIP_OA_02, MAIN_ACTIVE_ADJ, PIP_OA_02_HSYNC_ADJ); 
    	vRegWriteFldAlign(SCPIP_PIP1_08, 0+_u4OSTGDelta, PIP1_08_READ_DELAY_1);
    	#if CC_CTI_WA
    	vRegWriteFldAlign(SCPIP_PIP1_08, MAIN_ACTIVE_ADJ - CC_CTI_LEFT_MARGIN +_u4OSTGDelta, PIP1_08_ACTIVE_ADJ_1); 
    	#else
    	vRegWriteFldAlign(SCPIP_PIP1_08, MAIN_ACTIVE_ADJ+_u4OSTGDelta, PIP1_08_ACTIVE_ADJ_1); 
    	#endif
    	#if SUPPORT_POP
    	vRegWriteFldAlign(SCPIP_PIP2_08, SUB_READ_DELAY+_u4OSTGDelta, PIP2_08_READ_DEALY_2);
    	vRegWriteFldAlign(SCPIP_PIP2_08, SUB_ACTIVE_ADJ+_u4OSTGDelta, PIP2_08_ACTIVE_ADJ_2); // PLC_DLY need to adjust
    	#endif
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#if CC_SCPOS_MUTE_RELATCH_WA
UINT32 u4DrvScposGetHwMuteStatus(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return 0;
    }
    return _au4HwMuteStatus[bPath];
}

void vDrvScposSetHwMuteStatus(UINT8 bPath, UINT8 u1OnOff)
{
    if (bPath >= SV_VP_NA)
    {
        return;
    }

    _au4HwMuteStatus[bPath] = u1OnOff;
}
#endif

#if 0
//-----------------------------------------------------------------------------
/** Brief of vWriteSCPOS.
 */
//-----------------------------------------------------------------------------
void vWriteSCPOS(UINT32 u4Reg, UINT8 bVal)
{
    SCPOS_8BIT_W(u4Reg, bVal);
}

//-----------------------------------------------------------------------------
/** Brief of bReadSCPOS.
 */
//-----------------------------------------------------------------------------
UINT8 bReadSCPOS(UINT32 u4Reg)
{
    return SCPOS_8BIT_R(u4Reg);
}
#endif

/**
* @brief Set scpip clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vDrvSetSCPIPClock(UINT8 bPath, SCPIP_CLOCK_TYPE eType)
{
    #ifdef CC_MT5363
    if(bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, eType, FLD_SCPIP_MAIN_CLK_SEL);
    }
    else if(bPath == SV_VP_PIP)
    {
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, eType, FLD_SCPIP_PIP_CLK_SEL);
    }
    #else // MT5387
    #ifdef CC_MT5387
    if(bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(CKGEN_VOPROCCFG, eType, FLD_SCPIP_MAIN_CLK_SEL);
    }
    else if(bPath == SV_VP_PIP)
    {
        vIO32WriteFldAlign(CKGEN_VOPROCCFG, eType, FLD_SCPIP_PIP_CLK_SEL);
    }
    #endif
    #endif
}

//-----------------------------------------------------------------------------
/** Brief of vScposInit.
 * Initial SCPOS related setting
 * @param  bPath
 * @retval void
 */
//-----------------------------------------------------------------------------
void vScposInit(UINT8 bPath)
{
    UINT8 dsOffset = getDSOffset(bPath);            
    //UINT8 usOffset = getUSOffset(bPath);   
    UINT8 pipOffset = getPIPOffset(bPath);  

    vDrvDISetOutputClock(bPath, DI_OCLK_TYPE_VDOIN);
    vDrvSetSCPIPClock(bPath, SCPIP_CLK_TYPE_VDOIN);
    vScpipPScanDispmodeWaterCtrl(bPath, SV_ON);
    
    // DS to odd pixel, the right most pixel color issue.
    vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_YC422_OPT1);
    #ifdef CC_FORCE_OLD_IMPORT
    vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_IMPORT_SEL_1);
    #else
    // new import protection flow --> Fix to frame 0    
    vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 3, PIP1_04_IMPORT_SEL_1);
    #endif
    //H Boundary protection enable
    vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, 1,PIP1_0F_BOUNDARY_H_EN_1);

    //Disable DS dither to avoid video noise
    //vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_ROUNDSEL_1);


    if (bPath == SV_VP_MAIN)
    {
        //------MAIN/Sub Commonl Part ---------------------------
        // 1080i dram mode, the left side first line abnormal 
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_ENABLE_VSYNC_FP1);
        // Video jitter when trunning on Dispmode with WXGA panel.
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_ASYNC_PROTECT1);

        // init PSCAN Dispmode + US parameters
        vRegWriteFldAlign(SCPIP_PIP1_07, 41, PIP1_07_DISPMODE_H_POS_1);
        vRegWriteFldAlign(SCPIP_US1_10, 1, US1_10_REG_PSCAN_DISP_EN_1);
        vRegWriteFldAlign(SCPIP_US1_10, 2, US1_10_REG_CLEAR_SEL_1);
        vRegWriteFldAlign(SCPIP_US1_10, 61, US1_10_REG_DISP_PRETFETCH_1);
        
        vRegWriteFldAlign(SCPIP_PIP1_0E, 1024, PIP1_0E_DISPMODE_DATA_RATE_1);
        vRegWriteFldAlign(SCPIP_PIP1_0C, 1, PIP1_0C_PSCAN_VSYNC_REGEN1); //enable vsync regen for Pscan

        //------MAIN Special Part ---------------------------
        // init US filter
        vRegWriteFldAlign(SCPIP_US1_01, 1, US1_01_HORIZONTAL_FILTER_OFF_1);

        //default: OUTSTG_VSYNC_DELSEL = 4 and OUTSTG_VSYNC_DELSEL2 = 5 in 4-taps
        vRegWriteFldAlign(OUTSTG_OS_75, 4, OS_75_VSYNC_DELSEL);
        vRegWriteFldAlign(OUTSTG_OS_75, 5, OS_75_VSYNC_DELSEL2);
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_LSB_OFF);

        _vDrvSetSyncAdj();
    }
    else
    {
        //------MAIN/Sub Commonl Part ---------------------------
        // 1080i dram mode, the left side first line abnormal 
        vRegWriteFldAlign(SCPIP_PIP_OA_09, 1, PIP_OA_09_ENABLE_VSYNC_FP2);
        // Video jitter when trunning on Dispmode with WXGA panel.
        vRegWriteFldAlign(SCPIP_PIP_OA_09, 1, PIP_OA_09_ASYNC_PROTECT2);

        // init PSCAN Dispmode + US parameters
        vRegWriteFldAlign(SCPIP_PIP2_07, 38, PIP2_07_DISPMODE_H_POS_2);
        vRegWriteFldAlign(SCPIP_US2_02, 1, US2_02_REG_PSCAN_DISP_EN_2);
        vRegWriteFldAlign(SCPIP_US2_02, 5, US2_02_REG_CLEAR_SEL_2);
        vRegWriteFldAlign(SCPIP_US2_02, 54, US2_02_REG_DISP_PRETFETCH_2);
        
        vRegWriteFldAlign(SCPIP_PIP2_0E, 1024, PIP2_0E_DISPMODE_DATA_RATE_2);
        vRegWriteFldAlign(SCPIP_PIP2_0C, 1, PIP2_0C_PSCAN_VSYNC_REGEN2);//enable vsync regen for Pscan

        //------PIP Special Part ---------------------------
        vRegWriteFldAlign(SCPIP_US2_04, 0, US2_04_VS_DATA_FH_2);
        
        #if SUPPORT_HDTV_HARDWARE_MUTE
        //enable sub path quick off function by default
        vDrvConfigQuickMute(SV_VP_PIP,SV_ON);
        #endif
        /* initially hide sub window output region */
        vRegWriteFldAlign(SCPIP_PIP2_02, 0, PIP2_02_RVL_2);
    }
}

void vScpipSetMirror(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 offset = getPIPOffset(bPath);
    /* move check to u1GetFlipMirrorConfig
    if(IS_IC_5387() || (IS_IC_5363() && (BSP_GetIcVersion() < IC_VER_5363_AB)))
    {
        // 5387, skip mirror
        // 5363 before ECO, skip mirror
    }
    else
    */
    {
        bScpipMirrorEnable[bPath]= bOnOff;

        if(bOnOff== SV_TRUE)
        {
            vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 1, PIP1_0B_HDIR_MIRROR_1);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 0, PIP1_0B_HDIR_MIRROR_1);
        }
    }
}

void vScpipSetFlip(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 offset;

    if(bScpipFlipEnable[bPath]==bOnOff)
        return;
    
    bScpipFlipEnable[bPath] = bOnOff;
    offset = getPIPOffset(bPath);

    if(bOnOff== SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 1, PIP1_0B_VDIR_MIRROR_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 0, PIP1_0B_VDIR_MIRROR_1);
    }

    if (bPath == SV_VP_MAIN)
    {
    	 #ifdef CC_SCPIP_WA_FLIP_2ND_LINE
        vRegWriteFldAlign(SCPIP_PIP1_09, bOnOff, PIP1_09_VLMASK_1);
    	 #else
        vRegWriteFldAlign(SCPIP_US1_14, bOnOff, US1_14_UP_VBOUND_EN); // enable V bound
    	 #endif
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP2_09, bOnOff, PIP1_09_VLMASK_1);
    }
}

void vScpipSetMirrorFlipAll(UINT8 bOnOff)
{
    vScpipSetMirror(SV_VP_MAIN, bOnOff);
    vScpipSetMirror(SV_VP_PIP, bOnOff);

    vScpipSetFlip(SV_VP_MAIN, bOnOff);
    vScpipSetFlip(SV_VP_PIP, bOnOff);
}

void vScpipPScanDispmodeWaterCtrl(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0A, bOnOff, PIP_OA_0A_WATER_CTRL1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0C, bOnOff, PIP_OA_0C_WATER_CTRL2);
    }

    if(bOnOff==SV_ON)
    {
        if (bPath == SV_VP_MAIN)
        {

            vRegWriteFldAlign(SCPIP_PIP_OA_0A, 0x72, PIP_OA_0A_WTHR31);
            vRegWriteFldAlign(SCPIP_PIP_OA_0A, 0x65, PIP_OA_0A_WTHR21);
            vRegWriteFldAlign(SCPIP_PIP_OA_0A, 0x60, PIP_OA_0A_WTHR11);

            // token21/11/01 keep 0x80 to prevent garbage at the right side
            vRegWriteFldAlign(SCPIP_PIP_OA_0B,  0, PIP_OA_0B_WTOKEN31);
            vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x80, PIP_OA_0B_WTOKEN21);
            vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x80, PIP_OA_0B_WTOKEN11);
            vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x80, PIP_OA_0B_WTOKEN01);
        }
        else  // sub path
        {/* //todo

            vRegWriteFldAlign(SCPIP_PIP_OA_0C, 0x90, PIP_OA_0C_WTHR32);
            vRegWriteFldAlign(SCPIP_PIP_OA_0C, 0x80, PIP_OA_0C_WTHR22);
            vRegWriteFldAlign(SCPIP_PIP_OA_0C, 0x70, PIP_OA_0C_WTHR12);

            vRegWriteFldAlign(SCPIP_PIP_OA_0D,  0, PIP_OA_0D_WTOKEN32);
            vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x80, PIP_OA_0D_WTOKEN22);
            vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x80, PIP_OA_0D_WTOKEN12);
            vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x80, PIP_OA_0D_WTOKEN02);
            */
        }
    }
}

UINT8 vScpipGetDispmodeLineBufNum(UINT32 bPath)
{
    UINT8 bCline, bCfgCline;
    bCline = RegReadFldAlign(SCPIP_PIP_OA_06+bPath,  PIP_OA_06_C_LINE1);
    bCfgCline = RegReadFldAlign(SCPIP_PIP_OA_06+bPath,  PIP_OA_06_CFG_LINE1);

    if(bCline ==1)
    {
        return bCfgCline+2;
    }
    else // (bCline ==0)
    {
        switch (bCfgCline)
        {
            case 0: return 3;
            case 1: return 4;
            case 2: return 6;
            case 3: return 8;
            default: return 0;
        }
    }
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
//#define CC_SCPOS_WA99
void vSwitchFrameTrackOnOff(UINT32 u4OnOff) 
{
    // can be enable when dram mode and scaler display mode
    static UINT32 u4Status=0xff;
    //static UINT32 u4EarlyLossEn= SV_FALSE;
    //UINT32 u4DataSel;

    if(u4Status != u4OnOff)
    {
        u4Status = u4OnOff;
        LOG(5,"vSwitchFrameTrackOnOff %d\n\n", u4OnOff);
    }
    else
    {
    #if 0 // no matter ECO or not , Frame track off can take effect immediately
        if ((IS_IC_5387() && (BSP_GetIcVersion() <= IC_VER_5363_AA))  ||
            (IS_IC_5363() && (BSP_GetIcVersion() <= IC_VER_5387_AA)))  
        {
            // check if we need to enable early loss lock
            if (u4OnOff && (u4EarlyLossEn == SV_FALSE))
            { 
                if(fgGetFrameTrackStatus()  ==SCPIP_FRAME_TRACK_LOCK)            
                {
                    u4EarlyLossEn = SV_TRUE;
                    vRegWriteFldAlign(SCPIP_PIP_OA_0E, 1, PIP_OA_0E_LOSS_EARLY_EN);
                }
            }        
            else if(!u4OnOff && (u4EarlyLossEn == SV_TRUE))
            {
                u4EarlyLossEn = SV_FALSE;
                vRegWriteFldAlign(SCPIP_PIP_OA_0E, 0, PIP_OA_0E_LOSS_EARLY_EN);
            }
        }
        #endif
        return;
    }

    #if defined(__MODEL_slt__) || defined(DRV_SUPPORT_EXTMJC) // Frame tracking always off for SLT	and external MJC
    vRegWriteFldAlign(SCPIP_PIP_OA_0E, 0, PIP_OA_0E_FRAME_TRACK_EN1);
    #else
    if(u4OnOff)  // notice the order of TRACK_ENABLE_DIRECT and RAME_TRACK_EN1
    {    
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_OFF, PIP_OA_0E_FRAME_TRACK_ENABLE_DIRECT);  
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_ON, PIP_OA_0E_FRAME_TRACK_EN1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_OFF, PIP_OA_0E_FRAME_TRACK_EN1);
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, SV_ON, PIP_OA_0E_FRAME_TRACK_ENABLE_DIRECT);
    }
    #endif

#if 0  // drv_display will control the import protection, will confllict
    #ifdef CC_MT5387
    if (BSP_GetIcVersion() <= IC_VER_5387_AA)  
    #endif
    {
        u4DataSel = RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_SET_DATA_SEL);
        if(u4DataSel ==0)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_01, 1, PIP_OA_01_SET_DATA_SEL);
            HAL_Delay_us(500);
            vRegWriteFldAlign(SCPIP_PIP_OA_01, 0, PIP_OA_01_SET_DATA_SEL);
        }
    }
#endif
    #ifdef CC_SCPOS_WA99
    if(u4OnOff==SV_ON)
    {
         //DCHEC00005267	Scaler_PIP	Frame lock status incorrect
        // modify fsync_vsp to force trigger frame track when dram mode.
        // currently, not necessary
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
E_FRAME_TRACK_STAUS  fgGetFrameTrackStatus(void) 
{
    UINT32 u4Status, u4VDist,u4VTotal;
    UINT32 u4Mode = u4GetFrameTrackMode();

    if (u4Mode == 1)  // HW Frame Track
    {
        u4Status = RegReadFldAlign(SCPIP_PIP_OA_12, PIP_OA_12_FRAME_TRACK_ST);
        switch(u4Status)
        {
            case 0:
                return SCPIP_FRAME_TRACK_DISABLE;
            case 5: 
                return SCPIP_FRAME_TRACK_LOCK;
            case 1:
                     u4VDist = u4ScposGetInOutVDistance();
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
            if (_u4VDistTrackState)
            {
                return SCPIP_FRAME_TRACK_NOT_STALBE; 
            }
            else
            {
                return SCPIP_FRAME_TRACK_LOCK; 
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
     

#if SUPPORT_HDTV_HARDWARE_MUTE
void vDrvConfigHardwareMute(void)
{
    UINT8 mode;
    mode = getScalerMode(SV_VP_MAIN);
    //1. if Main in scaler dispmode
    if(mode==VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL)
    {
        //enable quick blank mute
        vDrvConfigQuickBlank(QUICK_BLANK_BY_MAIN);
        //disable quick off mute
        vDrvConfigQuickMute(SV_VP_MAIN,SV_OFF);
    }
    else //2. others mode
    {
        //disable quick blank mute
        vDrvConfigQuickBlank(QUICK_BLANK_NO_FUNCTION);
        //enable quick off mute
        vDrvConfigQuickMute(SV_VP_MAIN,SV_ON);
    }
}

void vDrvConfigQuickBlank(E_QUICK_BLANK_MODE mode)
{
    //only enable in YPbPr and VGA
    if (((bGetSignalType(SV_VP_MAIN) == SV_ST_YP) || (bGetSignalType(SV_VP_MAIN) == SV_ST_VGA) || (bGetSignalType(SV_VP_MAIN) == SV_ST_DVI)))
    {
        vRegWriteFldAlign(OUTSTG_OS_1A, mode, OS_1A_QUICK_BLANK_SEL);
    }
    else
    {
        mode = QUICK_BLANK_NO_FUNCTION;
        vRegWriteFldAlign(OUTSTG_OS_1A, mode, OS_1A_QUICK_BLANK_SEL);
    }
}

void vDrvConfigQuickMute(UINT8 bPath,UINT8 mode)
{
    if(bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OUTSTG_OS_1A, mode, OS_1A_MAIN_QUICK_OFF_EN);
    }
    else if(bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OUTSTG_OS_1A, mode, OS_1A_PIP_QUICK_OFF_EN);
    }
}

#endif

#if 0
//-----------------------------------------------------------------------------
/** Brief of vDrvSetBgMuteColor.
 * Set Mute Color in R/G/B Color Space for Background Screen
 * @param  void
 * @retval void
 */
//-----------------------------------------------------------------------------
void vDrvSetBgMuteColor(void)
{
    // Black
    vRegWriteFldAlign(OUTSTG_OS_72, 0, OS_72_MUTEBR);
    vRegWriteFldAlign(OUTSTG_OS_72, 0, OS_72_MUTEBG);
    vRegWriteFldAlign(OUTSTG_OS_72, 0, OS_72_MUTEBB);

}

//-----------------------------------------------------------------------------
/** Brief of vSetMainMuteColor.
 * Set Mute Color in R/G/B Color Space for Main Screen
 * @param  bMuteColor
 * @retval void
 */
//-----------------------------------------------------------------------------
void vSetMainMuteColor(UINT8 bMuteColor)
{
#ifndef CC_UP8032_ATV
    UNUSED(bMuteColor);
#endif
}

//-----------------------------------------------------------------------------
/** Brief of vSetPipMuteColor.
 * Set Mute Color in R/G/B Color Space for Pip Screen
 * @param  bMuteColor
 * @retval void
 */
//-----------------------------------------------------------------------------
void vSetPipMuteColor(UINT8 bMuteColor)
{
#ifndef CC_UP8032_ATV
    UNUSED(bMuteColor);
#endif
}
#endif
//-----------------------------------------------------------------------------
/** Brief of vDrvMuteRelatedChk.
 * Set Mute Color and Mute On/Off for Main and PIP Channel
 * @param  bPath(SV_VP_MAIN/SV_VP_PIP)
 * @retval void
 */
//-----------------------------------------------------------------------------
void vDrvMuteRelatedChk(UINT8 bPath)
{
    UNUSED(bPath);
}

void vScposMuteMaskOnOff(UINT8 bPath, UINT32 u4OnOff)
{
#ifdef CC_SCPOS_MUTE_MASK
    _au4MuteMaskEn[bPath] = u4OnOff;
#endif
}


#ifdef CC_TV_MEMO_MODE
EXTERN UINT8 bTVMemoOnOff;
#endif

//-----------------------------------------------------------------------------
/** Brief of vScposSetMuteColor.
 * Set Background Color for Main and PIP Channel
 */
//-----------------------------------------------------------------------------
void vScposSetMuteColor(UINT8 bPath, UINT32 u4Bg)
{
//Mute color in 5362B were changed to 12 bits, we need to set 8bit into 12 bits as "00xxxxxxxx00"
	UINT16 u2Red, u2Green, u2Blue;

	u2Red	= ((u4Bg >> 16) & 0xFF) << 2;
	u2Green = ((u4Bg >> 8) & 0xFF) << 2;
	u2Blue	= (u4Bg & 0xFF) << 2;
     LOG(3, "vScposSetMuteColor(%d) %d %d %d\n", bPath, u2Red, u2Green, u2Blue);
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OUTSTG_OS_53, u2Red, OS_53_R_MUTE_MAIN);
        vRegWriteFldAlign(OUTSTG_OS_52, u2Green, OS_52_G_MUTE_MAIN);
        vRegWriteFldAlign(OUTSTG_OS_52, u2Blue, OS_52_B_MUTE_MAIN);
    }
#if SUPPORT_POP
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_54, u2Red, OS_54_R_MUTE_PIP);
        vRegWriteFldAlign(OUTSTG_OS_54, u2Green, OS_54_G_MUTE_PIP);
        vRegWriteFldAlign(OUTSTG_OS_53, u2Blue, OS_53_B_MUTE_PIP);
    }
#endif
}

//-----------------------------------------------------------------------------

void vDrvOutRGBColorOnOff(UINT8 bOnOff)
{
    switch (bOnOff)
    {
    case SV_ON:
        vDrvMainMute(SV_ON);
        vRegWriteFldAlign(OS_28,1, GMT_WEN);
        vRegWriteFldAlign(OUTSTG_OS_53, 0, OS_53_R_MUTE_MAIN);
        vRegWriteFldAlign(OUTSTG_OS_52, 0, OS_52_G_MUTE_MAIN);
        vRegWriteFldAlign(OUTSTG_OS_52, 0, OS_52_B_MUTE_MAIN);
        break;

    case SV_OFF:
        vDrvMainMute(SV_OFF);
        vRegWriteFldAlign(OS_28, 1, GMTWMODE_3_0_);

        vRegWriteFldAlign(OS_28, 0, GAMMA_CS);

        vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

        vRegWriteFldAlign(OS_22, 0, N_GM_WDATA_R);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_G);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_B);        

        vRegWriteFldAlign(OS_28, 7, GAMMA_CS);

        vRegWriteFldAlign(OS_22, 0, N_GM_WDATA_R);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_G);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_B); 

        vRegWriteFldAlign(OS_28, 0, GMT_WEN);
        vRegWriteFldAlign(OS_28, 0, GMTWMODE_3_0_);
        break;

    default:
        break;
    }
}

void vDrvOutRGBColor(UINT8 RValue,UINT8 GValue,UINT8 BValue)
{
    //Set RValue

    vRegWriteFldAlign(OS_28, 0, GAMMA_CS);

    vRegWriteFldAlign(OS_28, 2, GMTWMODE_3_0_);

    vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

    vRegWriteFldAlign(OS_22, RValue, N_GM_WDATA_R);

    vRegWriteFldAlign(OS_28, 7, GAMMA_CS);

    vRegWriteFldAlign(OS_22, RValue, N_GM_WDATA_R);

    vRegWriteFldAlign(OS_28, 0, GAMMA_CS);


    //Set GValue
    vRegWriteFldAlign(OS_28, 3, GMTWMODE_3_0_);

    vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
    vRegWriteFldAlign(OS_23, GValue, N_GM_WDATA_G);

    vRegWriteFldAlign(OS_28, 7, GAMMA_CS);

    vRegWriteFldAlign(OS_23, GValue, N_GM_WDATA_G);

    vRegWriteFldAlign(OS_28, 0, GAMMA_CS);

    //Set BValue
    vRegWriteFldAlign(OS_28, 4, GMTWMODE_3_0_);

    vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
    vRegWriteFldAlign(OS_23, BValue, N_GM_WDATA_B);
    vRegWriteFldAlign(OS_28, 7, GAMMA_CS);

    vRegWriteFldAlign(OS_23, BValue, N_GM_WDATA_B);
}


/**
 * @brief dwDrvGetMuteMask

 * Return Main And PIP Mute Mode To fgApiVideoStable

 * @param  bMode
 * @retval UINT32
 */
UINT32 dwDrvGetMuteMask(UINT8 bMode)
{
    if (bMode==0)
        return _u4MainMuteMask;
    else
        return _u4PipMuteMask;
}

/**
 * @brief fgApiVideoStable

 * Return Video Stable Mode To Aud_drv

 * @param  void
 * @retval UINT8
 */
UINT8 fgApiVideoStable(UINT8 u1VideoFrame)
{
    UINT32 u4MuteMainFlag;
    UINT32 u4MutePIPFlag;
    SRM_TV_MODE_T eNewMode = SRM_GetTvMode();

    if (eNewMode == SRM_TV_MODE_TYPE_NORMAL)
    {
        u4MuteMainFlag = _u4DrvVideoGetMute(SV_VP_MAIN);
        if (u4MuteMainFlag==0)
        {
            return SV_TRUE;
        }
    }
    else if (eNewMode == SRM_TV_MODE_TYPE_PIP)
    {
        u4MuteMainFlag= _u4DrvVideoGetMute(SV_VP_MAIN);
        u4MutePIPFlag= _u4DrvVideoGetMute(SV_VP_PIP);
        if (((u1VideoFrame == 0) && (u4MuteMainFlag== 0)) ||
                ((u1VideoFrame == 1) && (u4MutePIPFlag== 0)))
        {
            return SV_TRUE;
        }
    }
    else if (eNewMode == SRM_TV_MODE_TYPE_POP)
    {
        u4MuteMainFlag= _u4DrvVideoGetMute(SV_VP_MAIN);
        u4MutePIPFlag= _u4DrvVideoGetMute(SV_VP_PIP);
        if (((u1VideoFrame == 0) && (u4MuteMainFlag== 0)) ||
                ((u1VideoFrame == 1) && (u4MutePIPFlag== 0)))
        {
            return SV_TRUE;
        }
    }
    return SV_FALSE;
}

void vDrvSetOSTGOutPattern(UINT32 u4OnOff)
{
    if (u4OnOff == SV_ON)
    {
         vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_PATGEN_EN);
    }
    else
    {
         vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_PATGEN_EN);
    }
}


void vDrvSetOSTGInPattern(UINT32 u4VdpId, UINT32 u4OnOff)
{
    if (u4VdpId == VDP_1)
    {
        vRegWriteFldAlign(OUTSTG_OS_28, (u4OnOff? 1: 0), OS_28_MAIN_PTGEN_EN);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_28, (u4OnOff? 1: 0), OS_28_PIP_PTGEN_EN);
    }

}

UINT32 _dwDrvSetScalerDRAM(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4GBRYCbCr)
{
    UINT32 u4DataFormat = 0;
    UINT32 u410BitMode;
    UINT32 u4RGBMode;
    UINT32 u4GY;
    UINT32 u4BCb;
    UINT32 u4RCr;
    UINT32 u4BaseAddress;
    UINT32 u4FrameSize;
    UINT32 u4HL, u4VL;
    UINT32 u4Data1, u4Data2;
    UINT32 i, j, idx;
    UINT32 u4Offset1, u4Offset2;
    UINT32 u4DramUnitOffset;
    UINT32 u4Factor;
    UINT32 u4OverscanX, u4OverscanY;

    // support main only
    if (u4VdpId != VDP_1)
    {
        Printf("Not Support!\n");
        return 1;
    }

    // check freeze status
    if (fgMainWriteOn())
    {
        Printf("Freeze before set scaler DRAM!\n");
        return 1;
    }

    // Get Data format
    u410BitMode = RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
    u4RGBMode = fgMain444Mode();
    if (u4RGBMode && u410BitMode)
    {
        u4DataFormat = 3;
    }
    else if (u4RGBMode && (u410BitMode == 0))
    {
        u4DataFormat = 2;
    }
    else if ((u4RGBMode == 0) && u410BitMode)
    {
        u4DataFormat = 1;
    }
    else
    {
        u4DataFormat = 0;
    }
    u4GY = (u4GBRYCbCr & 0xff0000) >> 16;
    u4BCb = (u4GBRYCbCr & 0x00ff00) >> 8;
    u4RCr = (u4GBRYCbCr & 0x0000ff);


    // Get DRAM base address
    u4BaseAddress = RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1);
    u4FrameSize = RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1);
    u4BaseAddress += (u4FrameSize * RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1));
    u4BaseAddress <<= 4;

    LOG(3, "Set Scaler DRAM: Format(%d), Data(0x%X), BaseAddress(0x%X), R_Pos(%d)\n",  u4DataFormat, u4GBRYCbCr, u4BaseAddress, RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1));

    // Get DRAM parameter
    u4HL = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);
    u4VL = RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1);

    // mapping source coordination to scaler DRAM
    LOG(3, "Rect (%d, %d, %d, %d) -->", u4X, u4Y, u4Width, u4Height);
    u4Factor = (RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER));
//    u4OverscanX = (RegReadFldAlign(SCPOS_PIP_03, SCPOS_MHP_OFST_9_2) << 2) |RegReadFldAlign(SCPOS_PIP_04, SCPOS_MHP_OFST_1_0);
//    u4OverscanX = u4OverscanX << 1;    // 1 means 2 pixel
//    u4OverscanY = RegReadFldAlign(SCPOS_PIP_08, SCPOS_W1STARTP_F0OFST_9_0);

    u4X = (u4X * u4Factor/DFT_DS_FACTOR);
    u4Width = (u4Width * u4Factor/DFT_DS_FACTOR);
    LOG(3, " (%d, %d, %d, %d) -->", u4X, u4Y, u4Width, u4Height);
/*
    if (u4X < u4OverscanX)
    {
        if (u4Width < (u4OverscanX - u4X))
        {
            u4Width = 0;
        }
        else
        {
            u4Width -=  u4OverscanX - u4X;
        }
        u4X = 0;
    }
    else
    {
        u4X -= u4OverscanX;
    }
    if (u4Y < u4OverscanY)
    {
        if (u4Height < (u4OverscanY - u4Y))
        {
            u4Height = 0;
        }
        else
        {
            u4Height -=  u4OverscanY - u4Y;
        }
        u4Y = 0;
    }
    else
    {
        u4Y -= u4OverscanY;
    }
*/
    if (u4DataFormat == 0) // YCbCr16 bit
    {
        u4OverscanX = u4HL * 8;      // pixel per-line in DRAM
    }
    else if (u4DataFormat == 1) // YCbCr20 bit
    {
        u4OverscanX = u4HL * 6;    // pixel per-line in DRAM
    }
    else // RGB 24
    {
        u4OverscanX = u4HL * 16/3;   // pixel per-line in DRAM
    }
    u4OverscanY = u4VL * 2;
    if (u4X + u4Width > u4OverscanX)
    {
        if (u4Width < (u4OverscanX -u4X))
        {
           u4Width = 0;
        }
        else
        {
            u4Width -= u4OverscanX -u4X;
        }
    }
    if (u4Y + u4Height > u4OverscanY)
    {
        if (u4Height < (u4OverscanY -u4Y))
        {
           u4Height = 0;
        }
        else
        {
            u4Height -= u4OverscanY -u4Y;
        }
    }
    u4Factor = RegReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_H_1);
    u4Width = (u4Width * u4Factor)/DFT_DS_FACTOR;

    u4Factor = RegReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_V_1);
    u4Height = (u4Height * u4Factor)/DFT_DS_FACTOR;
    LOG(3, " (%d, %d, %d, %d)\n", u4X, u4Y, u4Width, u4Height);

    #ifndef CC_COPLAT_MT82
    HalFlushDCache();
    HalInvalidateDCache();
    #endif

    // Set DRAM
    switch (u4DataFormat)
    {
        case 0:        // YCbCr16 bit
            u4BaseAddress += u4HL * u4Y * 16;
            u4Data1 = (u4GY << 24) | (u4BCb << 16) | (u4GY << 8) | u4RCr;
            for (j = u4Y; j < (u4Y+u4Height); j += 1)
            {
                for (i = u4X; i < (u4X+u4Width); i += 2)   // handle 2 pixels once
                {
                    u4Offset1 = i >> 3;
                    u4Offset1 = (u4Offset1 << 4) + (16 - ((i - (u4Offset1 << 3))*2) - 4);
                    vWriteDramD(u4BaseAddress + u4Offset1, u4Data1);
                }
                u4BaseAddress += u4HL * 16;  // calculate dram base address of next line
            }
            break;
        case 1:        // YCbCr20 bit
            // shift 8 bit data to 10 bit data
            u4GY = u4GY << 2;
            u4BCb = u4BCb << 2;
            u4RCr = u4RCr << 2;

            u4BaseAddress += u4HL * u4Y * 16;
            for (j = u4Y; j < (u4Y+u4Height); j += 1)
            {
                u4DramUnitOffset = u4X/6;
                u4DramUnitOffset *= 16;
                for (i = ((u4X /6) * 6); i < (u4X+u4Width); i += 6)   // handle 6 pixels once
                {
                    // pixel 0, 1
                    if (i >=  u4X)
                    {
                        u4Data2 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + 8);
                        u4Data1 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + 12);
                        u4Data2 = (u4Data2 & 0x0000ffff) | ((u4GY & 0x3f) << 26) | (u4RCr << 16);
                        u4Data1 = (u4Data1 & 0xff000000) | (u4GY << 14) | (u4BCb << 4) | ((u4GY & 0x3c0) >> 6);
                        vWriteDramD(u4BaseAddress + u4DramUnitOffset + 8, u4Data2);
                        vWriteDramD(u4BaseAddress + u4DramUnitOffset + 12, u4Data1);
                    }

                    // pixel 2, 3
                    if (((i + 2) >=  u4X) && ((i + 2) < (u4X+u4Width)))
                    {
                        u4Data1 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + 4);
                        u4Data2 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + 8);
                        u4Data1 = (u4Data1 & 0x000000ff) | (u4RCr << 8) | (u4GY << 18) | ((u4BCb & 0xf) << 28);
                        u4Data2 = (u4Data2 & 0xffff0000) | (u4GY << 6) | ((u4BCb & 0x3f0) >> 4);
                        vWriteDramD(u4BaseAddress + u4DramUnitOffset + 4, u4Data1);
                        vWriteDramD(u4BaseAddress + u4DramUnitOffset + 8, u4Data2);
                    }

                    // pixel 4, 5
                    if ((i + 4) < (u4X+u4Width))
                    {
                        u4Data2 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + 4);
                        u4Data1 = ((u4GY & 0x3) << 30) | (u4BCb << 20) | (u4GY << 10) | (u4RCr);
                        u4Data2 = (u4Data2 & 0xffffff00) | ((u4GY & 0x3fc) >> 2);
                        vWriteDramD(u4BaseAddress + u4DramUnitOffset, u4Data1);
                        vWriteDramD(u4BaseAddress + u4DramUnitOffset + 4, u4Data2);
                    }

                    u4DramUnitOffset += 16;
                }
                u4BaseAddress += u4HL * 16;  // calculate dram base address of next line
            }


            break;
        case 2:        // RGB24 bit
            u4BaseAddress += u4HL * u4Y * 16;
            for (j = u4Y; j < (u4Y+u4Height); j += 1)
            {
                for (i = u4X ; i < (u4X+u4Width); i ++)
                {
                    u4DramUnitOffset = (i >> 4) * 16 * 3;
                    idx = i - ((i >> 4) << 4);
                    switch (idx)
                    {
                        case 0:
                        case 4:
                        case 8:
                        case 12:
                        	if (idx == 0)
                        	{
                        	    u4Offset1 = 12;
                        	}
                        	else if (idx == 4)
                        	{
                                u4Offset1 = 0;
                        	}
                        	else if (idx == 8)
                        	{
                        	    u4Offset1 = 20;
                        	}
                        	else //if (idx == 12)
                        	{
                        	    u4Offset1 = 40;
                        	}
                            u4Data1 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1);
                            u4Data1 = (u4Data1 & 0x000000ff) | (u4RCr << 8) | (u4BCb << 16) | (u4GY << 24);
                            vWriteDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1, u4Data1);
                        	break;

                        case 1:
                        case 5:
                        case 9:
                        case 13:
                        	if (idx == 1)
                        	{
                        	    u4Offset1 = 8;
                        	    u4Offset2 = 12;
                        	}
                        	else if (idx == 5)
                        	{
                                u4Offset1 = 28;
                        	    u4Offset2 = 0;
                        	}
                        	else if (idx == 9)
                        	{
                        	    u4Offset1 = 16;
                        	    u4Offset2 = 20;
                        	}
                        	else //if (idx == 13)
                        	{
                        	    u4Offset1 = 36;
                        	    u4Offset2 = 40;
                        	}
                            u4Data1 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1);
                            u4Data2 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + u4Offset2);
                            u4Data1 = (u4Data1 & 0x0000ffff) | (u4RCr << 16) | (u4BCb << 24);
                            u4Data2 = (u4Data2 & 0xffffff00) | u4GY;
                            vWriteDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1, u4Data1);
                            vWriteDramD(u4BaseAddress + u4DramUnitOffset + u4Offset2, u4Data2);
                        	break;

                        case 2:
                        case 6:
                        case 10:
                        case 14:
                        	if (idx == 2)
                        	{
                        	    u4Offset1 = 4;
                        	    u4Offset2 = 8;
                        	}
                        	else if (idx == 6)
                        	{
                                u4Offset1 = 24;
                        	    u4Offset2 = 28;
                        	}
                        	else if (idx == 10)
                        	{
                        	    u4Offset1 = 44;
                        	    u4Offset2 = 16;
                        	}
                        	else //if (idx == 14)
                        	{
                        	    u4Offset1 = 32;
                        	    u4Offset2 = 36;
                        	}
                            u4Data1 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1);
                            u4Data2 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + u4Offset2);
                            u4Data1 = (u4Data1 & 0x00ffffff) | (u4RCr << 24) ;
                            u4Data2 = (u4Data2 & 0xffff0000) | (u4GY << 8) | (u4BCb);
                            vWriteDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1, u4Data1);
                            vWriteDramD(u4BaseAddress + u4DramUnitOffset + u4Offset2, u4Data2);
                        	break;

                        case 3:
                        case 7:
                        case 11:
                        case 15:
                        	if (idx == 3)
                        	{
                        	    u4Offset1 = 4;
                        	}
                        	else if (idx == 7)
                        	{
                                u4Offset1 = 24;
                        	}
                        	else if (idx == 11)
                        	{
                        	    u4Offset1 = 44;
                        	}
                        	else //if (idx == 15)
                        	{
                        	    u4Offset1 = 32;
                        	}
                            u4Data1 = dwReadDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1);
                            u4Data1 = (u4Data1 & 0xff000000) | (u4RCr) | (u4BCb << 8) | (u4GY << 16);
                            vWriteDramD(u4BaseAddress + u4DramUnitOffset + u4Offset1, u4Data1);
                        	break;
                        default:
                        	//ASSERT(0);
                        	break;
                    }
                }
                u4BaseAddress += u4HL * 16;  // calculate dram base address of next line
            }
            break;
        case 3:        // RGB30 bit
            Printf("Not Support data format!\n");
            break;

        default:
            //ASSERT(0);
            break;

    }

    #ifndef CC_COPLAT_MT82
    HalFlushDCache();
    HalInvalidateDCache();
    #endif

    return 0;
}

// for dram dump
// added by peggy 20071004
#ifdef SUPPORT_DRAM_DUMP
PRIVATE UINT8 bRecoverDRAMDumpStack(void)
{
    while (_u1StackSize > 0)
    {
        _u1StackSize --;

#ifdef MT5387_TODO
        if (_au2AddrStack[_u1StackSize] == 0xd2c4) //recover
        {
            vIO32Write4B(CKGEN_DISP_CKCFG1, _au4ValueStack[_u1StackSize]);
        }
        else
        {
            vRegWrite4B(_au2AddrStack[_u1StackSize], _au4ValueStack[_u1StackSize]);
        }
#endif
    }
    return SV_TRUE;
}


UINT8 bInitScalerDump(void)
{
    _u1StackSize = 0;

#ifdef MT5387_TODO
    /* Turn off video in interrupt */
    _au2AddrStack[_u1StackSize] = 0x240c;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(0x240c);
    vRegWrite4B(0x240c, 0xffffffff);
#endif

    /* Turn off main channel write to external memory */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_09;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_09);
    vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);

    return SV_TRUE;
}


UINT8 bReleaseScalerDump(void)
{
#if 0
    return bRecoverDRAMDumpStack();
#else
    return SV_FALSE;
#endif
}


UINT8 bInitLastStageDump(void)
{
#if 0
    UINT32 dwFrameSize, dwWriteAddr;
    UINT32 wHTotal, wVTotal, wHL, wVL, wMoSyncLen;
    UINT8 bIs10bitMode, bDSIs10bitMode, bDSIs444Mode;

    _u1StackSize = 0;

    /* 1. Set frame num to 1 */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_09;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_09);
    vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_FSEL_1);

    /* 2. Freeze screen */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_09;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_09);
    vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);

    /* 3. Bypass down scaling */
    _au2AddrStack[_u1StackSize] = SCPIP_DS1_00;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_DS1_00);
    vRegWriteFldAlign(SCPIP_DS1_00, 0x8000, DS1_00_DSCALER_H_1);
    vRegWriteFldAlign(SCPIP_DS1_00, 0x8000, DS1_00_DSCALER_V_1);

#ifdef MT5387_TODO
    /* 4. Switch to scaler clock to OCLK */
    _au2AddrStack[_u1StackSize] = 0xd2c4;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(CKGEN_DISP_CKCFG1);
    vIO32Write4BMsk(CKGEN_DISP_CKCFG1, 0x40000000, 0x40000000);
#endif
    /* 5. Get color depth */
    bDSIs10bitMode = RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
    bDSIs444Mode = RegReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1);

    /* 6. Set to DUMP_VD10BSEL and DUMP_SEL444 */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_0E;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_0E);
    vRegWriteFldAlign(SCPIP_PIP1_0E, bDSIs10bitMode, PIP1_0E_10B_SEL_DUMP);
    vRegWriteFldAlign(SCPIP_PIP1_0E, bDSIs444Mode, PIP1_0E_SEL444_DUMP);

    /* 7. Set VL_DUMP and Data_Length_DUMP */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_0D;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_0D);
    vRegWriteFldAlign(SCPIP_PIP1_0D, RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1), PIP1_0D_VL_DUMP);
    vRegWriteFldAlign(SCPIP_PIP1_0D, RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1), PIP1_0D_DATA_LENGTH_DUMP);

    /* 6. TURN ON dump mode */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP_OA_02;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP_OA_02);
    vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_DUMP_EN);
    _au2AddrStack[_u1StackSize] = SCPIP_DS1_02;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_DS1_02);
    bIs10bitMode = RegReadFldAlign(OUTSTG_OS_29, OS_29_10B_EN);
    vRegWriteFldAlign(SCPIP_DS1_02, bIs10bitMode, DS1_02_VD10BSEL_1);
    vRegWriteFldAlign(SCPIP_DS1_02, 1, DS1_02_SEL444_1);
    vUtDelay10ms(7);

#ifdef MT5387_TODO
    /* 7. Turn off video in interrupt */
    _au2AddrStack[_u1StackSize] = 0x240c;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(0x240c);
    vRegWrite4B(0x240c, 0xffffffff);
#endif

    /* 8. Get HTotal and VTotal */
    wHTotal = RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_MOHLEN) + 1;
    wVTotal = RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_MOVLEN) + 1;
    wMoSyncLen  =RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_MOSYNCLEN);
    wHTotal = wHTotal - wMoSyncLen - 1; // only keep active region

    /* 9. Get HL and VL */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_01;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_01);
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_00;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_00);
    if (bIs10bitMode) // 10 bit
    {
        wHL = (wHTotal + 3)/4;
        vRegWriteFldAlign(SCPIP_PIP1_01, wHL, PIP1_01_HL_1);
    }
    else // 8 bit
    {
        wHL = (wHTotal*3 + 15)/16;
        vRegWriteFldAlign(SCPIP_PIP1_01, wHL, PIP1_01_HL_1);
    }
    wVL = wVTotal + 1;
    vRegWriteFldAlign(SCPIP_PIP1_00, wVL, PIP1_00_VL_1);

    dwFrameSize = (wHL*16*wVL)/16; // right shift 4
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_07;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_07);
    vRegWriteFldAlign(SCPIP_PIP1_07, dwFrameSize, PIP1_07_WINC_1);

    /* 10. Get write base address */
    dwWriteAddr = RegReadFldAlign(SCPIP_PIP1_06, PIP1_06_RSTARTP_1);
    dwWriteAddr = dwWriteAddr + dwFrameSize;
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_05;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(SCPIP_PIP1_05);
    vRegWriteFldAlign(SCPIP_PIP1_05, dwWriteAddr, PIP1_05_WSTARTP_1);

    /* 11. Set HPos and VPos */
    _au2AddrStack[_u1StackSize] = OUTSTG_OS_16;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(OUTSTG_OS_16);
    vRegWriteFldAlign(OUTSTG_OS_16, 0x8000, OS_16_H_POS);
    _au2AddrStack[_u1StackSize] = OUTSTG_OS_18;
    _au4ValueStack[_u1StackSize++] = u4RegRead4B(OUTSTG_OS_18);
    vRegWriteFldAlign(OUTSTG_OS_18, 0x4000, OS_18_V_POS);

    /* 12. Turn on Main write */
    vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_W_EN_1);
    vUtDelay10ms(7);
    vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);
    return SV_TRUE;
#else  // does not support last stage dump in 5387
    return SV_FALSE;
#endif
}


UINT8 bReleaseLastStageDump(void)
{
    return bRecoverDRAMDumpStack();
}

UINT8 bGetScalerDRAMAverage(UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 *pu4VR, UINT32 *pu4UB, UINT32 *pu4YG)
{
	UINT32 u4TotalPixel;
	UINT32 u4XIndex, u4YIndex;
	UINT32 u4DataVR, u4DataUB, u4DataYG;
	UINT32 u4TotalVR=0, u4TotalUB=0, u4TotalYG=0;

	if((u4Width ==0) || (u4Height==0))
		return SV_FALSE;
	
	for(u4XIndex=0; u4XIndex<u4Width; u4XIndex++)
	{
		for(u4YIndex=0; u4YIndex<u4Height; u4YIndex++)
		{
			if(bGetScalerDRAM(u4X+u4XIndex, u4Y+u4YIndex, &u4DataVR, &u4DataUB, &u4DataYG)== SV_FALSE)
			{
				return SV_FALSE;
			}
			else
			{
				u4TotalVR +=u4DataVR;
				u4TotalUB +=u4DataUB;
				u4TotalYG +=u4DataYG;
			}
		}
	}

	u4TotalPixel= (UINT32)(u4Width * u4Height);
	if(RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1))
    	{
    		*pu4VR = (UINT32)(u4TotalVR/ u4TotalPixel);
		*pu4UB = (UINT32)(u4TotalUB/ u4TotalPixel);
		*pu4YG = (UINT32)(u4TotalYG/ u4TotalPixel);
    	}
    	else
    	{
		*pu4VR = (UINT32)(u4TotalVR/ u4TotalPixel)<<2;
		*pu4UB = (UINT32)(u4TotalUB/ u4TotalPixel)<<2;
		*pu4YG = (UINT32)(u4TotalYG/ u4TotalPixel)<<2;

    	}
	return SV_TRUE;
}

extern UINT32 _u4OsDumpEnable;
void vGetScalerDRAMInfo(tagDramDumpInfo_T *rDramDumpInfo)
{
	UINT16 DownScaleFactor, PreDownScaleFactor;    
	UINT8 bIndex;
    rDramDumpInfo->dwAddr = RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1) << 4;
    bIndex = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1);
    
    // get MHL
    rDramDumpInfo->dwMHL = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);
    // get MVL
    rDramDumpInfo->dwMVL = RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1);
    
	PreDownScaleFactor = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    DownScaleFactor = RegReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_H_1);
    rDramDumpInfo->dwWidth = ((_rMPicInfo.wTargetW * PreDownScaleFactor / 0x8000) * 
    						DownScaleFactor) / 0x8000;
    rDramDumpInfo->dwHeight = rDramDumpInfo->dwMVL - 2;
    

    rDramDumpInfo->dwSize = rDramDumpInfo->dwMHL * rDramDumpInfo->dwMVL * 16;
	rDramDumpInfo->bIs10bit = RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
    rDramDumpInfo->bIs444 = RegReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1); 

    // WINC
    rDramDumpInfo->dwWINC = RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1);
    rDramDumpInfo->u2W_POS = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1);
    rDramDumpInfo->u2R_POS = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1);

	rDramDumpInfo->dwAddr = rDramDumpInfo->dwAddr + rDramDumpInfo->dwWINC * 16 * bIndex;
	
    // prevent unfreeze by FW
    _u4OsDumpEnable = SV_TRUE;


    LOG(6, "ScalerDRAMInfo DRAM Addr = %x\n", rDramDumpInfo->dwAddr);               
                
    LOG(6, "ScalerDRAMInfo DRAM MHL = %d, MVL = %d.\n", rDramDumpInfo->dwMHL, 
                rDramDumpInfo->dwMVL);                    
}

UINT8 bGetScalerDRAM(UINT32 u2X, UINT32 u2Y, UINT32 *pu2VR, UINT32 *pu2UB, UINT32 *pu2YG)
{
    UINT8 bIs10bitMode, bIs444;
    UINT32 dwAddr, u4HpsFactor;
    UINT32 wMHL;
    UINT32 u2XOffset, u2YOffset;
    UINT32 u2OverscanX, u2OverscanY;
    UINT32 dwDHFac, dwDVFac;

    bIs10bitMode = RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
    bIs444 = RegReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1);

    // get Dump Address
    dwAddr = RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1);
    dwAddr = (dwAddr << 4); // left shift 4 bits

    // get MHL
    wMHL = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);

    // get x, y offset in DRAM
    u2OverscanX = RegReadFldAlign(SCPIP_PIP1_04, PIP1_04_HP_OFST_1);
    u2OverscanY = RegReadFldAlign(SCPIP_PIP1_04, PIP1_04_WSTARTP_OFST_1);
    u4HpsFactor = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    u2XOffset = (UINT16)((UINT32)u2X * u4HpsFactor/DFT_DS_FACTOR);   // HPS

    if ((u2OverscanX > u2XOffset) || (u2OverscanY > u2Y))
    {
        // the target pixel is not in DRAM
        LOG(3, "Error: The target pixel is not in DRAM!!\n");
        return SV_FALSE;
    }
    u2XOffset -= u2OverscanX;
    u2YOffset = u2Y - u2OverscanY;

    // Down-scaling factor
    dwDHFac = RegReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_H_1);
    dwDVFac = RegReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_V_1);
    u2XOffset = (UINT16)((UINT32)u2XOffset * dwDHFac/DFT_DS_FACTOR);
    u2YOffset = (UINT16)((UINT32)u2YOffset * dwDVFac/DFT_DS_FACTOR);

    // Read DRAM
    if (bGetScalerDRAMData(u2XOffset, u2YOffset, bIs10bitMode, bIs444, dwAddr, wMHL, pu2VR, pu2UB, pu2YG) == SV_FALSE)
    {
        return SV_FALSE;
    }

    return SV_TRUE;
}

// To Do: need to modify
UINT8 bGetLastStageDRAM(UINT32 u2X, UINT32 u2Y, UINT32 *pu2R, UINT32 *pu2B, UINT32 *pu2G)
{
#if 0
    UINT8 bIs10bitMode;
    UINT32 dwAddr;
    UINT32 wHL, wHPos, wVPos;

    bIs10bitMode = RegReadFldAlign(OUTSTG_OS_29, OS_29_10B_EN);

    // get Dump Address
    dwAddr = RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1);
    dwAddr = (dwAddr << 4); // left shift 4 bits

    // get SHL
    wHL = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);

    // re-calculate u2X and u2Y
    wHPos = RegReadFldAlign(OUTSTG_OS_16, OS_16_H_POS);
    wVPos = RegReadFldAlign(OUTSTG_OS_18, OS_18_V_POS);

    if (wHPos > wHPosOri)
    {
        u2X = u2X + (wHPos - wHPosOri)*2;
    }
    else
    {
        u2X = u2X - (wHPosOri - wHPos)*2;
    }

    if (wVPos > wVPosOri)
    {
        u2Y = u2Y + (wVPos - wVPosOri)*2;
    }
    else
    {
        u2Y = u2Y - (wVPosOri - wVPos)*2;
    }

    // Read DRAM
    if (bGetScalerDRAMData(u2X, u2Y, bIs10bitMode, 1, dwAddr, wHL, pu2R, pu2B, pu2G) == SV_FALSE)
    {
        return SV_FALSE;
    }
    return SV_TRUE;
#else
    UNUSED(u2X);
    UNUSED(u2Y);
    UNUSED(pu2R);
    UNUSED(pu2B);
    UNUSED(pu2G);
    return SV_FALSE;
#endif
}


PRIVATE UINT8 bGetScalerDRAMData(UINT32 wX, UINT32 wY, UINT8 bIs10bitMode, UINT8 bIs444, UINT32 dwAddr, UINT32 wMHL, UINT32 *pu2VR, UINT32 *pu2UB, UINT32 *pu2YG)
{
    UINT32 dwOffset, dwData;

    if ((pu2VR == NULL) || (pu2UB == NULL) || (pu2YG == NULL))
    {
        return SV_FALSE;
    }

    // 10 bit mode
    if (bIs10bitMode == 1)
    {
        // RGB
        if (bIs444)
        {
            LOG(3, "<RGB 10 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/4)*16);

            switch (wX%4)
            {
            case 3:
                dwOffset += 0;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData&0x3FF);
                *pu2UB = (dwData>>10)&0x3FF;
                *pu2YG = (dwData>>20)&0x3FF;
                break;

            case 2:
                dwOffset += 30/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData>>6)&0x3FF;
                *pu2UB = (dwData>>16)&0x3FF;
                dwOffset += 2;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2YG = (dwData>>10)&0x3FF;
                break;

            case 1:
                dwOffset += (30*2)/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData>>4)&0x3FF;
                *pu2UB = (dwData>>14)&0x3FF;
                dwOffset += 3;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2YG = (dwData&0x3FF);
                break;

            default:
                dwOffset += (30*3)/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData>>2)&0x3FF;
                *pu2UB = (dwData>>12)&0x3FF;
                *pu2YG = (dwData>>22)&0x3FF;
                break;
            }
        }

        // YCbCr
        else
        {
            LOG(3, "<YCbCr 10 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/6)*16);

            switch (wX%6)
            {
            case 5:
            case 4:
                dwOffset += 0;
                break;

            case 3:
            case 2:
                dwOffset += 5;
                break;

            default:
                dwOffset += 10;
                break;
            }
            dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word

            if ((wX%2) == 0) // Y0Cb0Cr0
            {
                *pu2VR = (dwData&0x3FF);
                dwOffset += 10/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // xY0Cb0Y1
                *pu2UB = (dwData>>12)&0x3FF;
                *pu2YG = (dwData>>22)&0x3FF;
            }
            else // Y1Cb0Cr0
            {
                *pu2VR = (dwData&0x3FF);
                *pu2YG = (dwData>>10)&0x3FF;
                *pu2UB = (dwData>>20)&0x3FF;
            }
        }
    }

    // 8bit mode
    else
    {
        // RGB
        if (bIs444)
        {
            LOG(3, "<RGB 8 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/16)*16*3);

            switch (wX%16)
            {
            case 0:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+12)); // read a word
                *pu2VR = (dwData>>8)&0xFF;
                *pu2UB = (dwData>>16)&0xFF;
                *pu2YG = (dwData>>24)&0xFF;
                break;

            case 1:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+10)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 2:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+7)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 3:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+4)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 4:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+1)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 5:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2YG = (dwData&0xFF);
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+28)); // read a word
                *pu2VR = (dwData>>16)&0xFF;
                *pu2UB = (dwData>>24)&0xFF;
                break;

            case 6:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+27)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 7:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+24)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 8:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+21)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 9:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+18)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 10:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+16)); // read a word
                *pu2UB = (dwData&0xFF);
                *pu2YG = (dwData>>8)&0xFF;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+44)); // read a word
                *pu2VR = (dwData>>24)&0xFF;
                break;

            case 11:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+44)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 12:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+41)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 13:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+38)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 14:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+35)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 15:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+32)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;
            }
        }

        // YCbCr
        else
        {
            LOG(3, "<YCbCr 10 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/8)*16);

            switch (wX%8)
            {
            case 7:
            case 6:
                dwOffset += 0;
                break;

            case 5:
            case 4:
                dwOffset += 4;
                break;

            case 3:
            case 2:
                dwOffset += 8;
                break;

            default:
                dwOffset += 12;
                break;
            }
            dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word

            if ((wX%2) == 0) // Y0Cb0Cr0
            {
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>16)&0xFF;
                *pu2YG = (dwData>>24)&0xFF;
            }
            else // Y1Cb0Cr0
            {
                *pu2VR = (dwData&0xFF);
                *pu2YG = (dwData>>8)&0xFF;
                *pu2UB = (dwData>>16)&0xFF;
            }
        }
    }
    return SV_TRUE;
}
#endif

