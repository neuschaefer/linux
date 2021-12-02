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
 * $RCSfile: drv_scaler.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.h
 *  Brief of file drv_scaler.h.
 *  Details of file drv_scaler.h (optional).
 */

#ifndef _DRV_SCALER_DRVIF_H_
#define _DRV_SCALER_DRVIF_H_


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_typedef.h"
#include "vdp_drvif.h"
#include "debug.h"
#include "feature.h"
#include "video_def.h"
#include "vdo_misc.h"
#include "drv_scaler.h"

//#ifdef CC_SCPOS_3DTV_SUPPORT
#include "drv_tdtv_drvif.h"
//#endif

#if defined(CC_MT5396)
#define SCPIP_SUPPORT_POST_SCALER

#endif
#define SCPIP_SUPPORT_GFX_DUMP

#if defined(SCPIP_SUPPORT_POST_SCALER)
#include "drv_scaler_psc.h"
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define VDO_NONLINEAR_AUTO      1
#define VDO_NONLINEAR_AUTO_1      2

#define VDP_NONLINEAR_AUTO           255
#define UNIT_NONLINEAR               (0xA)
#define UINT_NONLINEAR_END_FACTOR    (1024)

//poclk value define
#define FLD_POCLK_SEL_OCLK      0
#define FLD_POCLK_SEL_OCLK_DIV2 1
#define FLD_POCLK_SEL_DPLL      2
#define FLD_POCLK_SEL_MIB_OCLK  3

//Main/Sub Offset
#define getPSCANOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x800)
#define getPIPOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x100)
#define getDSOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x100)
#define getUSOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x100)
#define getDramOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x100)
#define getDSPatOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x10)
#define getUSPatOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x04)
#define getImportOffset(bPath)((bPath == SV_VP_MAIN)?0:0x08)
#define getPIPOAOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x04)

// for 5398
#define getPDSOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x200)
#define getRegenOffset(bPath)((bPath == SV_VP_MAIN)?0:0x200)
#define getCtrlOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x200)
#define getMskOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x8)
#define getSCOffset(bPath) ((bPath == SV_VP_MAIN)? 0:0x200)


#define getScalerPath(u4VdpId) ((u4VdpId==VDP_1)?SV_VP_MAIN:SV_VP_PIP)
#define getChannel(bPath)  ((bPath==SV_VP_MAIN)?&_rMChannel:&_rPChannel)
#define getPicInfo(bPath)   ((bPath==SV_VP_MAIN)?&_rMPicInfo:&_rPPicInfo)
#define getDispPrm(bPath)   ((bPath==SV_VP_MAIN)?&_rMDispPrm:&_rPDispPrm)
#define getDispInfo(bPath)   ((bPath==SV_VP_MAIN)?&_rMDispInfo:&_rPDispInfo)
#define getScalerVDP(bPath)  ((bPath==SV_VP_MAIN)?VDP_1:VDP_2)
#define fireSCPOSModeChange(VDP_ID) ((VDP_ID==VDP_1)?vSetScposFlg(SCALER_MAIN_MODE_CHG):vSetScposFlg(SCALER_PIP_MODE_CHG))
#define getScalerMode(bPath) (u1ScpipGetDispMode(bPath))
#define getDramAddrInfo(u4VdpId) (&_arScalePrm[u4VdpId].rDramAddrInfo)
#define getFbmPoolInfo(u4VdpId)  (&_arScalePrm[u4VdpId].rFbmInfo)
#define getTdtvInfo(u4VdpId)    (&_arScalePrm[u4VdpId].rTdtvInfo)

#define DFT_DS_FACTOR                       (0x8000)
#define DFT_US_FACTOR                       (0x8000)
#define DFT_HPD_FACTOR                      (0x8000)
#define DFT_DS_BIT_NUM                      (15)
#define DFT_US_BIT_NUM                      (15)

/*for 5398 FPGA*/
#define DFT_FRAC_PHASE_BITS_MAIN (20)
#define DFT_FRAC_PHASE_BITS_PIP  (15)
#define DFT_FRAC_PHASE_BITS(bPath) ((bPath==SV_VP_MAIN)?DFT_FRAC_PHASE_BITS_MAIN:DFT_FRAC_PHASE_BITS_PIP)
#define DFT_UNIT_PHASE(bPath)      (1<<DFT_FRAC_PHASE_BITS(bPath))
#define DFT_FRAC_PHASE_MASK(bPath) (DFT_UNIT_PHASE(bPath)-1)

#define DFT_PDS_FACTOR      (0x8000)

#define DIVIDE_WITH_CARRIER(dividend, divisor)      (divisor==0? ((dividend) + ((divisor) - 1)) : (((dividend) + ((divisor) - 1)) / (divisor)))
#define DIVIDE_WITH_ROUND(dividend, divisor)        (divisor==0? ((dividend) + ((divisor) >> 1)) :(((dividend) + ((divisor) >> 1)) / (divisor)))
#define RSHIFT_WITH_CARRIER(dividend, shift)        (((dividend) + ((1 << (shift)) - 1)) >> (shift))
#define RSHIFT_WITH_ROUND(dividend, shift)        (((dividend) + ((1 << (shift))/2)) >> (shift))

#define CAL_DS_FACTOR(from, to)     ((from==to || from ==0) ? DFT_DS_FACTOR : ((to << DFT_DS_BIT_NUM) / from))
#define CAL_US_FACTOR(from, to)     ((from==to || to ==0) ? DFT_US_FACTOR : ((from << DFT_US_BIT_NUM) / to))

#define ALIGN_MASK(VAL, MSK) (((VAL) + (MSK)) & (~(MSK)))
#define ALIGN_2_CEIL(val) ALIGN_MASK((val), 1)
#define ALIGN_4_CEIL(val) ALIGN_MASK((val), 3)
#define ALIGN_8_CEIL(val) ALIGN_MASK((val), 7)
#define ALIGN_16_CEIL(val) ALIGN_MASK((val), 15)
#define ALIGN_32_CEIL(val) ALIGN_MASK((val), 31)

#if defined(CC_MT5396)
#define SUPPORT_TV3D_OVERSCAN
#endif

#define SCPIP_TV3D_ALL                   (0xffffffff)
#define SCPIP_TV3D_INPUT_IS_3D           (1 << 0)   
// input/output format conversion
#define SCPIP_TV3D_SHUTTER_OUT           (1 << 1)  // L/R paired output
#define SCPIP_TV3D_FRAME_INTERLEAVE      (1 << 2)  // frame sequential to polarized
#define SCPIP_TV3D_LINE_INTERLEAVE       (1 << 3)  // side by side half to polarized

// input split method
#define SCPIP_TV3D_SBSH_SPLIT            (1 << 4)  // side by side half to frame sequential
#define SCPIP_TV3D_TNB_SPLIT             (1 << 5)  // top and bottom to frame sequential
#define SCPIP_TV3D_SBS_Q_SPLIT           (1 << 6)  // side by side to frame sequential + dequincunx
#define SCPIP_TV3D_SBS_Q_I_SPLIT         (1 << 7)  // side by side to frame sequential + dequincunx

#define SCPIP_TV3D_TNB_SW_WA             (1 << 8)  // top and bottom to frame sequential by sw

// other operations
#define SCPIP_TV3D_3D_TO_2D_DROP_R       (1 <<  9) // scaler 3d to 2d conversion, drop R frame
#define SCPIP_TV3D_3D_TO_2D_SBS          (1 << 10) // scaler 3d to 2d conversion, input SBS
#define SCPIP_TV3D_3D_TO_2D_SBS_Q        (1 << 11) // scaler 3d to 2d conversion, input SBS
#define SCPIP_TV3D_3D_TO_2D_SBS_Q_I      (1 << 12) // scaler 3d to 2d conversion, input SBS
#define SCPIP_TV3D_3D_TO_2D_TNB          (1 << 13) // scaler 3d to 2d conversion, input TNB
#define SCPIP_TV3D_3D_TO_2D_FS           (1 << 14) // scaler 3d to 2d conversion, input FS


#define SCPIP_TV3D_3D_REPEAT             (1 << 15)  // input 2d, output double frame rate
#define SCPIP_TV3D_3D_REPEAT_DS_HALF     (1 << 16) // input 2d, output double frame rate & V down scale to 1/2 output height

// FHD-PR
#define SCPIP_TV3D_FHD_PR                (1 << 17)  // scaler frame sequential to full PR
#define SCPIP_TV3D_FHD_PR_USE_V2         (1 << 18)  // scaler frame sequential to full PR, mode 2
#define SCPIP_TV3D_TYPE_MAX              19

#define DEF_TO_STR(a) #a

#define NPTV_REG_PRINT(a, b) Printf("%10u | %-40s\n", RegReadFldAlign(a, b), #b)  
#define NPTV_REG_PRINT_HEX(a, b) Printf("0x%08x | %-40s\n", RegReadFldAlign(a, b), #b) 
#define NPTV_REG_LOG(x, a, b) LOG(x, "%10u | %-40s\n", RegReadFldAlign(a, b), #b)  
#define NPTV_REG_LOG_HEX(x, a, b) LOG(x, "0x%08x | %-40s\n", RegReadFldAlign(a, b), #b) 

typedef enum {
    SCPIP_PTGEN_OFF = 0,
    SCPIP_PTGEN_AUTO,
    SCPIP_PTGEN_MANUAL,
} SCPIP_PTGEN_ONOFF;

typedef enum{
    SCALER_TDTV_IN_TYPE_2D,
    SCALER_TDTV_IN_TYPE_FS,
    SCALER_TDTV_IN_TYPE_SBS,
    SCALER_TDTV_IN_TYPE_TAB,
    SCALER_TDTV_IN_TYPE_LI,
}SCALER_TDTV_IN_TYPE;

typedef enum{
    SCALER_TDTV_OUT_TYPE_2D = 0,
    SCALER_TDTV_OUT_TYPE_PR = 1,
    SCALER_TDTV_OUT_TYPE_FPR = 2,
    SCALER_TDTV_OUT_TYPE_SBS = 3,
    SCALER_TDTV_OUT_TYPE_SG = 4,
    SCALER_TDTV_OUT_TYPE_3D_TO_2D = 5,
}SCALER_TDTV_OUT_TYPE;

typedef enum{
    SCALER_TDTV_RES_POSITION_PRE_H,
    SCALER_TDTV_RES_POSITION_PRE_V,
    SCALER_TDTV_RES_POSITION_PST_H,
    SCALER_TDTV_RES_POSITION_PST_V,
}SCALER_TDTV_RES_POSITION;

typedef enum{
    SCALER_DRAM_INIT_STATE_NOT_INIT,
    SCALER_DRAM_INIT_STATE_SRM_INITED,
    SCALER_DRAM_INIT_STATE_REG_INITED,    
} SCALER_DRAM_INIT_STATE;

typedef struct      //info from SRM
{
    UINT32 u4Addr;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4MddiDramWidth;
    UINT32 u4FbmSize;
    BOOL fg2FB;
    BOOL fg10bit;
    BOOL fg8bit;
    BOOL fg422;
} SCALER_FBM_POOL_T;

typedef struct      //calculated Dram Address info
{
    UINT32 FrameNum;
    UINT32 HMsbPageNum;
    UINT32 VMsbPageNum;
    UINT32 HLsbPageNum;
    UINT32 VLsbPageNum;
    UINT32 MsbPageNum;
    UINT32 LsbPageNum;
    UINT32 YMsbAddr;
    UINT32 YLsbAddr;
    UINT32 UMsbAddr;
    UINT32 ULsbAddr;
    UINT32 VMsbAddr;
    UINT32 VLsbAddr;
    UINT32 LowAddr;
    UINT32 HighAddr;
}SCALER_DRAM_ADDR_T;

typedef struct
{
    SCALER_TDTV_IN_TYPE eTdtvInType;
    SCALER_TDTV_OUT_TYPE eTdtvOutType;    
    E_TDTV_RES_OUTPUT ePreH;
    E_TDTV_RES_OUTPUT ePreV;
    E_TDTV_RES_OUTPUT ePstH;
    E_TDTV_RES_OUTPUT ePstV;    
    UINT32 u4IsPRUp;
}SCALER_TDTV_PRM_T;

typedef struct
{
    UINT16 u2PanelHTotal;
    UINT16 u2PanelHActive;
    UINT16 u2PanelVTotal;
    UINT16 u2PanelVActive;
    UINT32 u4DPllDividend;
    UINT32 u4DPLLDivisor;
    UINT16 u2FrameRate;
}SCALER_OUTTIMING_PRM_T;

/** Brief of VDP_SCALER_PRM_T
 */
typedef struct
{
    UINT32 u4Enable;                        //enabled 
    UINT32 u4ModChgDneFlg;                  //video mode change flag
    UINT32 u4HpsFactor;                     //HPS factor (re-calculated when scaler mode change)
    UINT32 u4HpsWidth;                      //width after pre-down
    UINT32 u4HpsAndOverscanedWidth;         //width after pre-down & overscan 
    UINT32 u4DownScaledWidth;               //width after pre-down, overscan & down
    UINT32 u4DownScaledHeight;              //height after down
    UINT32 u4FixedPreScaler;                //when 1, try to fix HPS factor to the same value. (to avoid temp. garbage)
    UINT32 u4ForcedPreScalingactor;         //HPS factor will force to this value when calculate scaling factor. (When this value isn't 0)
    UINT32 u4FirstConfig;
    UINT32 u4Forced10BitMode;               //force scaler data format to 8bit or 10 bit
    UINT32 u410BitMode;                     //scaler data format (8 or 10 bit. updated when srm set dram info)
    UINT32 u4Is444;                         //(not in use right now) scaler data format (422 or 444. update when srm set dram info)
    
    UINT32 u4121MapEn;                      //actual flag to show if scaler is in 1-to-1 mode or not
    UINT32 u4121Map;                        //1-to-1 flag set by vdp
    UINT16 u2121MapHPos;
    
    UINT32 u4DispMode;                      //from SRM to tell Scaler what mode should be set    
    UINT32 u4IsGameMode;                    //is game mode or not

    UINT32 u4MirrorEnable;                  //if mirror is enabled
    UINT32 u4FlipEnable;                    //if flip is enabled

    UINT32 u4Is422Up;                       //if is 422 up mode

    UINT32 u4IsScalerBobMode;               //if is scaler Bob mode
    UINT32 u4IsLRDataSwap;
    UINT32 u4NonlinearEn;
    UINT32 u4NonlinearCtrl;
    UINT32 u4LastNonlinearCtrl;   
    UINT32 u4NonlinearAuto;
    VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm;    ///< non-linear scaling setting for auto mode
    VDP_NONLINEAR_PRM_T rNonlinearPrm;
    VDP_NONLINEAR_PRM_T rLastNonlinearPrm;
    SCALER_DRAM_INIT_STATE eDramInitState;  //current dram init state
    UINT32 u4CntToRstDramPrm;               //counter to reset dram parameter, when count to 0, enable write enable
    UINT32 u4DramWidth;                     //Scaler Dram width from SRM
    UINT32 u4DramHeight;                    //Scaler Dram height from SRM
    UINT32 u4MddiFifoMin;                   //Pscan line buffer or fifo minimum limit (Query from Pscan)
    UINT32 u4MddiFifoMax;                   //Pscan line buffer or fifo maximum limit (Query from Pscan)
    SCALER_FBM_POOL_T rFbmInfo;            //Dram info from SRM call back
    SCALER_DRAM_ADDR_T rDramAddrInfo;       //
    /* 5365/5395 new nonlinear */
    UINT32 u4NewNonlinear;
    VDP_NEW_NONLINEAR_REG_T rNewNonlinearPrm;  
    SCALER_TDTV_PRM_T rTdtvInfo;
} VDP_SCALER_PRM_T;

#if defined(CC_MT5398)
/*for 5398 FPGA*/
typedef enum
{
    VIDEO_RES_MODULE_PREDS,
    VIDEO_RES_MODULE_REGEN,
    VIDEO_RES_MODULE_MIB,
    VIDEO_RES_MODULE_FSC,
    VIDEO_RES_MODULE_NS,
}VIDEO_RES_MODULE_E;

typedef struct
{
    UINT32 u4X;
    UINT32 u4Y;
    UINT32 u4W;
    UINT32 u4H;
}RECTANGLE_T;

typedef struct
{
    RECTANGLE_T inRes;
    RECTANGLE_T outRes;
}MODULE_RES_DESCRIPTION_T;

typedef struct
{
    RECTANGLE_T inRes;      //input resolution
    RECTANGLE_T ovsnRes;    //overscaned resolution
    RECTANGLE_T outRes;     //output resolution
}VIDEO_RES_DESCRIPTION_T;

typedef struct
{
    UINT32 u4HPreDsFactor;
    UINT32 u4VPreDsFactor;
    UINT32 u4ForcedHPreDsFactor;
    UINT32 u4ForcedVPreDsFactor;
    UINT32 u4PreDsInW;
    UINT32 u4PreDsInH;
    UINT32 u4PreDsOutW;
    UINT32 u4PreDsOutH;

    UINT32 u4RegenInW;
    UINT32 u4RegenInH;
    UINT32 u4RegenOutX;
    UINT32 u4RegenOutY;
    UINT32 u4RegenOutW;
    UINT32 u4RegenOutH;
    
    //fpga only
    UINT32 u4NXNRWidth;
    UINT32 u4NXNRHeight;   
    //fpga only(end)
    
	UINT32 u4InputWidth;			//input timing(pixel based)
	UINT32 u4InputHeight;

	UINT32 u4OutputX;			//output window(pixel based)
	UINT32 u4OutputY;
	UINT32 u4OutputWidth;
	UINT32 u4OutputHeight;

	UINT32 u4HDeltaPhaseIntg;	//integer
	UINT32 u4HDeltaPhaseFrac;	//fraction
	UINT32 u4VDeltaPhaseIntg;   //integer
	UINT32 u4VDeltaPhaseFrac;	//fraction	

    UINT32 u4HInitPhaseSign;
    UINT32 u4HInitPhaseIntg;
    UINT32 u4HInitPhaseFrac;
    UINT32 u4VInitPhaseSign;
    UINT32 u4VInitPhaseIntg;
    UINT32 u4VInitPhaseFrac;
    UINT32 u4HUVInitPhaseSign;
    UINT32 u4HUVInitPhaseIntg;
    UINT32 u4HUVInitPhaseFrac;

    UINT32 u4NonLinearInTotal;
    UINT32 u4NonLinearInMiddle;
    UINT32 u4NonLinearOutTotal;
    UINT32 u4NonLinearOutMiddle;

    UINT32 u4NonLinear;
    UINT32 u4NonLinearStartPos;
    UINT32 u4NonLinearMiddlePos;
    UINT32 u4NonLinearStopPos;
    UINT32 u4NonLinearPhaseSign;
    UINT32 u4NonLinearPhaseIntg; // delta
    UINT32 u4NonLinearPhaseFrac; // delta

    UINT32 u4Mirror;
    UINT32 u4Flip;

	UINT32 u4DramSRCWidth;

	UINT32 u4DramWidth;			//dram HV
	UINT32 u4DramHeight;
	UINT32 u4DramHPitch;
	UINT32 u4DramVPitch;

    UINT32 u4ForcedDramMode;    //0: auto 1:force to front mode 2: force to back mode
    UINT32 u4IsDramFrontMode;   //0: back mode 1:front mode
    
	UINT32 u4Forced10Bit;       //0: auto 1:force to 10 bit 2: force to 8 bit
	UINT32 u4Is10Bit;           //0: 8 bit 1: 10 bit

	UINT32 u4Forced444;         //0: auto 1:force to 444 2:force to 422
	UINT32 u4Is444;             //0: 422 1:444

	UINT32 u4RptGenCount;		//when gen read pointer

	UINT32 u4Dispmode;          //current scaler dispmode
    SCALER_TDTV_IN_TYPE eRegenIn3DType;	
} SCALERINFO;
#endif

EXTERN VDP_SCALER_PRM_T _arScalePrm[VDP_NS];

#ifdef CC_SCPOS_3DTV_SUPPORT
EXTERN UCHAR * SCPIP_TV3D_TYPE_NAMES[SCPIP_TV3D_TYPE_MAX];
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** Brief of vScpipImportOnOff.
 * @brief    turn on/off import protection
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipCheckImportWVsyncSel(UINT8 bPath);

EXTERN void vScpipSetImportOnOff(UINT8 bPath, UINT8 bOnOff);

//EXTERN UINT32 u4ScpipGetHwHPS(UINT8 bPath);

//EXTERN void vScpipSetHwHPSFactor(UINT32 u4VdpId, UINT32 u4Factor);

EXTERN void vScpipSetPSCANDispmode(UINT8 bPath,UINT8 bOnOff);

EXTERN void vScpipSetScalerDispmode(UINT8 bPath,UINT8 bMode);

EXTERN void vScpipSetDispModeFreeRunCount(UINT32 u4Count);

EXTERN void vScpipSetWPos(UINT32 u4VdpId, BOOL fgOnOff, UINT8 u1Index);

EXTERN void vScpipSetBufNum(UINT32 u4VdpId, UINT32 u4BufNum);

EXTERN void vScpipSetDramWrite(UCHAR u4VdpId, UCHAR ucOnOff);
EXTERN void vScpipSetDramRead(UCHAR u4VdpId, UCHAR ucOnOff);
EXTERN UINT8 bScpipGetDramWrite(UINT32 u4VdpId);
EXTERN UINT8 bScpipGetDramRead(UINT32 u4VdpId);

#ifdef CC_FLIP_MIRROR_SUPPORT
EXTERN void vScpipConfigFlip(UINT8 bPath);
#endif



//*****************************************************************************
//  drv_scaler_hw.c
//*****************************************************************************
#if defined(CC_MT5396)
EXTERN void vScpipSetPOCLKSelReg(UINT8 poclkSel);
#endif
EXTERN void vScpipSetFSCImportOff(UINT8 bOnOff,UINT8 bPath);
EXTERN void vScpipTurnOffImportProtection(UINT8 bPath);
EXTERN void vScpipTurnOnImportProtection(UINT8 bPath);
EXTERN void vScpipUpdateImportSetting(UCHAR ucVdpId,UCHAR ucMode);

//*****************************************************************************
//  drv_scaler_nonlinear.c
//*****************************************************************************

//-----------------------------------------------------------------------------
/**
 * @brief vScpipCalNonLinear
 * Calculate non-linear up-scaler register setting
 * @param  u4VdpId: video path
 * @param  u4SrcWidth: source active width
 * @param  u4OutWidth: output active height
 * @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void vScpipCalNonLinear(UINT32 u4VdpId, UINT32 u4SrcWidth, UINT32 u4OutWidth);
EXTERN void vScpipCalNewNonlinear(const UINT32 u4VdpId, const UINT32 u4SrcWidth, const UINT32 u4OutWidth);

#ifdef CC_SCPOS_3DTV_SUPPORT
//*****************************************************************************
//  drv_scaler_3d.c
//*****************************************************************************
EXTERN BOOL fgIsScpipTv3dFlgSet(UINT32 u4VdpId, UINT32 u4Flag);
EXTERN void vScpipTv3dSetFlg(UINT32 u4VdpId, UINT32 u4Flag);
EXTERN E_TDTV_DECODER_INPUT rScpipTv3dGetInType(UINT32 u4VdpId);
EXTERN void vScpipTv3dClrFlg(UINT32 u4VdpId, UINT32 u4Flag);
#endif

EXTERN void vScpipSetTdtvIO(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, SCALER_TDTV_OUT_TYPE out);
EXTERN SCALER_TDTV_IN_TYPE eScpipGetTdtvInType(UINT32 u4VdpId);
EXTERN void vScpipSetTdtvInType(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in);
EXTERN SCALER_TDTV_OUT_TYPE eScpipGetTdtvOutType(UINT32 u4VdpId);
EXTERN void vScpipSetTdtvOutType(UINT32 u4VdpId, SCALER_TDTV_OUT_TYPE out);
EXTERN E_TDTV_RES_OUTPUT eScpipGetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos);
EXTERN void vScpipSetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos, E_TDTV_RES_OUTPUT type);    
EXTERN void vScpipUpdateSrm3DResolution(UINT32 u4VdpId);
EXTERN void vScpipFire3DResChange(UINT32 u4VdpId);
EXTERN UINT32 u4ScpipGetTDTVPRUpFlag(UINT32 u4VdpId);
EXTERN UINT8 vScpipIs3DDoneByScaler(UINT32 u4VdpId);
EXTERN UINT8 vScpipIs3DDoneByMJC(void);
EXTERN void vScpipSetDispROutInv(UINT32 bOnOff);
EXTERN void vScpipUpdate3DPrm(UINT32 u4VdpId);
EXTERN void vScpipSet3DReg(UINT32 u4VdpId);
EXTERN void vScpipUpdateUs3DFieldFlag(UINT32 u4VdpId);
EXTERN void vScpipUpdate3DPRUpFlag(UINT32 u4VdpId);

#if defined(CC_MT5398)
EXTERN void vScpipSetInputInfo(UINT32 u4VdpId, UINT32 u4InputWidth, UINT32 u4InputHeight);
EXTERN void vScpipSetOutputInfo(UINT32 u4VdpId, UINT32 u4OutputX, UINT32 u4OutputY,UINT32 u4OutputWidth, UINT32 u4OutputHeight);
EXTERN void vScpipSetPanelInfo(UINT32 u4PanelWidth, UINT32 u4PanelHeight);
#endif

#endif
