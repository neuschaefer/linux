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

#include "x_typedef.h"
#include "vdp_drvif.h"
#include "debug.h"
#include "feature.h"
#include "video_def.h"
#include "vdo_misc.h"
#include "x_hal_5381.h"

#ifdef CC_SCPOS_3DTV_SUPPORT
#include "drv_tdtv_drvif.h"
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define VDO_NONLINEAR_AUTO      1
#define VDO_NONLINEAR_AUTO_1      2

#define VDP_NONLINEAR_AUTO           255
#define UNIT_NONLINEAR               (0xA)
#define UINT_NONLINEAR_END_FACTOR    (1024)

#ifdef CC_SCPOS_SUPPORT_DRAM_CHANNELB
#define SCPIP_CHANNELB_OFST (0x20000000)
//#define SCPIP_CHANNELB_OFST (BSP_GetChannelBStartAddress()<<20) 
#endif

#define getPSCANOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x800)
#define getPIPOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x80)
#define getDSOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x80)
#define getUSOffset(bPath) ((bPath == SV_VP_MAIN)?0:0x80)
#define getDispInfo(bPath)   ((bPath==SV_VP_MAIN)?&_rMDispInfo:&_rPDispInfo)
#define getChannel(bPath)  ((bPath==SV_VP_MAIN)?&_rMChannel:&_rPChannel)
#define getScalerPath(VDP_ID) ((VDP_ID==VDP_1)?SV_VP_MAIN:SV_VP_PIP)
#define getScalerVDP(bPath)  ((bPath==SV_VP_MAIN)?VDP_1:VDP_2)
#define fireSCPOSModeChange(VDP_ID) ((VDP_ID==VDP_1)?vSetScposFlg(SCALER_MAIN_MODE_CHG):vSetScposFlg(SCALER_PIP_MODE_CHG))
//#define getScalerMode(bPath) ((bPath==SV_VP_MAIN)?_arScalePrm[VDP_1].u4DispMode:_arScalePrm[VDP_2].u4DispMode)
#define getScalerMode(bPath) (u1ScpipGetDispMode(bPath))

#define DFT_DS_FACTOR                       (0x8000)
#define DFT_US_FACTOR                       (0x8000)
#define DFT_HPD_FACTOR                      (0x8000)
#define DFT_DS_BIT_NUM                      (15)
#define DFT_US_BIT_NUM                      (15)

#define DIVIDE_WITH_CARRIER(dividend, divisor)        (((dividend) + ((divisor) - 1)) / (divisor))
#define DIVIDE_WITH_ROUND(dividend, divisor)        (((dividend) + ((divisor) >> 1)) / (divisor))
#define RSHIFT_WITH_CARRIER(dividend, shift)        (((dividend) + ((1 << (shift)) - 1)) >> (shift))
#define RSHIFT_WITH_ROUND(dividend, shift)        (((dividend) + ((1 << (shift))/2)) >> (shift))

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
#define SCPIP_TV3D_SBS_Q_I_SPLIT         (1 << 7)  // side by side interlaced to frame sequential + dequincunx
#define SCPIP_TV3D_REALD                 (1 << 8)  // real D remove tag

#define SCPIP_TV3D_TNB_SW_WA             (1 << 9)  // top and bottom to frame sequential by sw

// other operations
#define SCPIP_TV3D_3D_TO_2D_DROP_R       (1 << 10) // scaler 3d to 2d conversion, drop R frame
#define SCPIP_TV3D_3D_TO_2D_SBS          (1 << 11) // scaler 3d to 2d conversion, input SBS
#define SCPIP_TV3D_3D_TO_2D_SBS_Q        (1 << 12) // scaler 3d to 2d conversion, input SBS quincunx
#define SCPIP_TV3D_3D_TO_2D_SBS_Q_I      (1 << 13) // scaler 3d to 2d conversion, input SBS quincunx interlaced
#define SCPIP_TV3D_3D_TO_2D_TNB          (1 << 14) // scaler 3d to 2d conversion, input TNB
#define SCPIP_TV3D_3D_TO_2D_FS           (1 << 15) // scaler 3d to 2d conversion, input FS


#define SCPIP_TV3D_3D_REPEAT             (1 << 16)  // input 2d, output double frame rate
#define SCPIP_TV3D_3D_REPEAT_DS_HALF     (1 << 17) // input 2d, output double frame rate & V down scale to 1/2 output height

// FHD-PR
#define SCPIP_TV3D_FHD_PR                (1 << 18)  // scaler frame sequential to full PR
#define SCPIP_TV3D_FHD_PR_USE_V2         (1 << 19)  // scaler frame sequential to full PR, mode 2

#define SCPIP_TV3D_FS_INPUT              (1 << 20)  // scaler frame sequential to full PR, mode 2
#define SCPIP_TV3D_FP_I_HDS_HALF         (1 << 21)  // frame packing interlace timing support

// interleave split method
#define SCPIP_TV3D_INTERLEAVE_SPLIT      (1 << 22)  // interleave to frame sequential
#define SCPIP_TV3D_3D_TO_2D_INTERLEAVE   (1 << 23)  // scaler 3d to 2d conversion, input interleave

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
#define SCPIP_TV3D_SBSH_PR               (1 << 24)  // side by side half to PR 5366 WA
#define SCPIP_TV3D_TNB_PR                (1 << 25)  // top and bottom to PR 5366 WA
#define SCPIP_TV3D_FP_PR                 (1 << 26)  // frame packing to PR 5366 WA
#define SCPIP_TV3D_TTD_PR                (1 << 27)  // 2D to 3D PR 5366 WA
#define SCPIP_TV3D_TYPE_MAX              28
#else
#define SCPIP_TV3D_TYPE_MAX              24
#endif


#define DEF_TO_STR(a) #a

#define NPTV_REG_PRINT(a, b) Printf("%10u | %-40s\n", RegReadFldAlign(a, b), #b)  
#define NPTV_REG_PRINT_HEX(a, b) Printf("0x%08x | %-40s\n", RegReadFldAlign(a, b), #b) 
#define NPTV_REG_LOG(x, a, b) LOG(x, "%10u | %-40s\n", RegReadFldAlign(a, b), #b)  
#define NPTV_REG_LOG_HEX(x, a, b) LOG(x, "0x%08x | %-40s\n", RegReadFldAlign(a, b), #b) 


/** Brief of VDP_SCALER_PRM_T
 */
typedef struct
{
    UINT32 u4Enable;
    UINT32 u4HpsFactor;

    UINT32 u4DynamicScale;
    UINT32 u4ResponseTime;

    UINT32 u4MddiEn;
    UINT32 u4FixedPreScaler;
    UINT32 u4ForcedPreScalingactor;
    UINT32 u4PreScalerNotOptimized;
    UINT32 u4FirstConfig;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4MddiFifoMin;
    UINT32 u4MddiFifoMax;

    UINT32 u410BitMode;
    UINT32 u4Is422;
    UINT32 u4121MapEn;
    UINT32 u4121Map;

    UINT32 u4ForceBypass;
    
    UINT32 u4DispModeEn;
    UINT32 u4DispMode;          //from SRM to tell Scaler what mode should be set
    
    UINT32 u4NonlinearEn;
    UINT32 u4NonlinearCtrl;
    UINT32 u4LastNonlinearCtrl;   
    UINT32 u4NonlinearAuto;
    VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm;    ///< non-linear scaling setting for auto mode
    VDP_NONLINEAR_PRM_T rNonlinearPrm;
    VDP_NONLINEAR_PRM_T rLastNonlinearPrm;

    //UINT32 u4Width;
    //UINT32 u4Height;

    UINT32 u4DramWidth;
    UINT32 u4DramHeight;
    UINT32 u4MddiDramWidth;

    UINT32 u4FbAddr1;
    UINT32 u4FbAddr2;
    UINT32 u4FbAddr3;
    /* 5365/5395 new nonlinear */
    UINT32 u4NewNonlinear;
    VDP_NEW_NONLINEAR_REG_T rNewNonlinearPrm;  
} VDP_SCALER_PRM_T;

EXTERN VDP_SCALER_PRM_T _arScalePrm[VDP_NS];

EXTERN RPicInfo* const _pPicInfo[];
EXTERN RDispPrm* const _pDispPrm[];
EXTERN RDispInfo* const _pDispInfo[];
EXTERN RVChannel* const _pChannel[];

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
EXTERN void vScpipImportOnOff(UINT8 bPath, UINT8 bOnOff);

EXTERN void vScpipSetPSCANDispmode(UINT8 bPath,UINT8 bMode);

EXTERN void vScpipSetScalerDispmode(UINT8 bPath,UINT8 bMode);

EXTERN void vScpipSetDispModeFreeRunCount(UINT32 u4Count);

EXTERN UINT32 u4ScpipGetDramAddrWithChannelOfst(UINT32 u4VdpId, UINT32 u4Addr);

EXTERN UINT32 u4ScpipGetWAddr(const UINT32 u4VdpId);

EXTERN UINT32 u4ScpipGetRAddr(const UINT32 u4VdpId);

EXTERN VOID vScpipSetWAddr(const UINT32 u4VdpId, UINT32 u4WAddr);

EXTERN VOID vScpipSetRAddr(const UINT32 u4VdpId, UINT32 u4RAddr);

EXTERN void vScpipSetWPos(UINT32 u4VdpId, BOOL fgOnOff, UINT8 u1Index);

EXTERN void vScpipSetBufNum(UINT32 u4VdpId, UINT32 u4BufNum);

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
//EXTERN void vScpipTv3dModeSwitch(UINT32 u4VdpId);
EXTERN void vScpipTv3dSetFlg(UINT32 u4VdpId, UINT32 u4Flag);
EXTERN void vScpipTv3dModeSwitch(void *pvArgv);
EXTERN E_TDTV_DECODER_INPUT rScpipTv3dGetInType(UINT32 u4VdpId);
EXTERN void vScpipTv3dClrFlg(UINT32 u4VdpId, UINT32 u4Flag);
EXTERN UINT32 u4ScpipGet3DOneEyeFrameRate(void);
EXTERN void vScpipSetOutputDispR(BOOL fgIsR); // set 1 is R, 0 is L

#endif

#endif
