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
 * $RCSfile: drv_di.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file drv_di.h
 *  Brief of file drv_di.h.
 *  Details of file drv_di.h (optional).
 */

#ifndef DRV_DI_H
#define DRV_DI_H


//////////////////////////////////////////////////////////////////////////////// 
// Include files
//////////////////////////////////////////////////////////////////////////////// 

#include "x_typedef.h"
#include "general.h"
#include "feature.h"
#include "vdp_if.h"

#define MDDI_REG_NUM        0x800
#define MDDI_REGTBL_END     0xFFFF
#define MDDI_MUTE_PERIOD    15
#ifdef CC_SUPPORT_VENC
#define CC_SUPPORT_H264
#endif

#define MPY_AND_TRUN(value, x, y) \
	(((UINT32)(value) * (UINT32)(x)) / (UINT32)(y))

#define MPY_AND_CEIL(value, x, y) \
	(((UINT32)(value) * (UINT32)(x) + (UINT32)(y) - (UINT32)1) / (UINT32)(y))

#define MAKE_ALIGN(value, align) \
	(MPY_AND_CEIL((value), 1, (align)) * (align))

#define MAKE_ALIGN_TO(value, align, fix) \
	((((value-((value>>align)<<align))>fix)) \
	? ((((value>>align)+1)<<align)+fix) \
	: (((value>>align)<<align)+fix))
	
#define MPY_RATIO_SUM(value, x, y) \
	((x + y*(value-1) + (value/2)) / value)

#define MDDI_READ_FLD(u1VdpId, u2Addr, u4Fld) \
     RegReadFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Fld)

#ifndef LOG_MDDI_REG
#define MDDI_WRITE_FLD(u1VdpId, u2Addr, u4Value, u4Fld) \
    vRegWriteFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Fld)

#define MDDI_WRITE32_MSK(u1VdpId, u2Addr, u4Value,  u4Msk) \
    vRegWrite4BMsk((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Msk)
#else
void MDDI_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld);
void MDDI_WRITE32_MSK(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Msk);
#endif


#define IS_SD_HEIGHT(u1Id)      ((_arMDDiPrm[u1Id].u2Height <= 576)&&(!(_arMDDiPrm[u1Id].u2VTotal >= 1125)))

#define IS_MPEG(u1Id)       ((_arMDDiPrm[u1Id].u1DecType == SV_VD_MPEGSD) \
                                                    || (_arMDDiPrm[u1Id].u1DecType == SV_VD_MPEGHD))

#define IS_MPEG_PROG(u1Id)      (IS_MPEG(u1Id) && (!_arMDDiPrm[u1Id].u1Interlace))

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
#define IS_SUPPORT_3D_PR(u1Id)  (_arMDDiPrm[u1Id].u1Support3DPR)
#endif

typedef enum
{
    DI_DISP_OFF,
    DI_INTERLACE_DISP,
    DI_PROGRESSIVE_DISP,
    DI_DISP_UNKNOWN
} DI_DISPLAY_MODE;

typedef enum
{
    DI_NORMAL_MODE,
    DI_MIRROR_ON_MODE,
    DI_FLIP_ON_MODE,
    DI_FLIP_MIRROR_ON_MODE
} DI_FLIP_MIRROR_MODE;

typedef enum
{
    DI_YUV_MODE_Y_ONLY,
    DI_YUV_MODE_YC,
    DI_YUV_MODE_YUV
} DI_YUV_MODE;

typedef enum
{
    E_DI_FULL_1W3R,     // Normal mode
    E_DI_FULL_1W4R,     // extra buffer or displaymode
    E_DI_CBOB_1W3R,     // CBOB mode
    E_DI_CBOB_1W4R,     // extra buffer or displaymode
    E_DI_YCBOB_0W0R,    // BOB mode
    E_DI_YCBOB_1W1R,    // extra buffer or displaymode 
    E_DI_FLIP_MIRROR_1W4R,
    E_DI_FLIP_MIRROR_CBOB_1W4R,
    E_DI_FLIP_MIRROR_YCBOB_1W1R,
    E_DI_UNKNOWN
} E_DI_DMODE;

typedef enum
{
    E_DI_NR_OFF,    // NR Off mode
    E_DI_NR_Y_ONLY, // NR Y Only mode
    E_DI_NR_YC_420, // NR YC 420 mode 
    E_DI_NR_YC_422, // NR YC 422 mode 
    E_DI_NR_RGB,    // NR RGB mode
    E_DI_NR_UNKNOWN
} E_NR_DMODE;

typedef struct
{
    BOOL fg10bit;
    BOOL fg422;    
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Mode;
} MDDI_FBM_POOL_T;

typedef struct
{
    UINT32 u4HMsbPgNum;
    UINT32 u4VMsbYPgNum;
    UINT32 u4VMsbCPgNum;
    UINT32 u4HLsbPgNum;
    UINT32 u4VLsbYPgNum;
    UINT32 u4VLsbCPgNum;
} MDDI_PAGE_NUM_T;

typedef struct
{
    UINT8 u1Region;
    UINT8 u1BitRes;
    UINT8 u1DecType;
    UINT8 u1Interlace;
    UINT8 u1420Mode;
    UINT8 u1OnOff;
    UINT8 u1Bob;
    UINT8 u1DebugIndex;
    UINT8 u1FreezeCnt;
    UINT8 u1StatusCnt;
    UINT8 u1ModeChg;
    #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    UINT8 u1Support3DPR;
    #endif        
    UINT16 u2BufWidth;
    UINT16 u2BufHeight;
    UINT16 u2Center;
    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT16 u2Width;
    UINT16 u2Height;
    UINT16 u2HClip;
    UINT16 u2VClip;
    UINT16 u2DTVClip;
    UINT32 u4BufAddr;
    UINT32 u4ProgDispClip;
    UINT32 u4TdTvDIType;
    E_DI_DMODE u1DataMode;
    E_DI_DMODE u1DebugDataMode;	
    E_NR_DMODE u1NRDataMode;
    E_NR_DMODE u1DebugNRDataMode;    
    DI_DISPLAY_MODE u1PSCANDisplay;
    DI_DISPLAY_MODE u1DebugPSCANDisplay;
    MDDI_PAGE_NUM_T rPgNum;
    MDDI_FBM_POOL_T rFbmPool;
} VDP_MDDI_PRM_T;

typedef struct
{
    UINT16 u2Addr;
    UINT32 u4Value;
    UINT32 u4Mask;
} MDDI_REGTBL_T;

typedef enum{
    E_DI_QUALITY_AGGRESSIVE, // MA Tend to 3D, Film Mode Tend to be entering, Edge Tend to be connected
    E_DI_QUALITY_DEFENSIVE,
    E_DI_QUALITY_NUM_MODE
} E_DI_QUALITY_MODE;

typedef enum{
    E_DI_ENGINE_IF,
    E_DI_ENGINE_PE,
    E_DI_ENGINE_FILM,    
    E_DI_ENGINE_TYPE_NUM
} E_DI_ENGINE_TYPE;

typedef void (*PThreadFunc)(void *pvArgv);

typedef struct{
    PThreadFunc pThreadFunc;
    CHAR* szThreadDescription;
} DI_PQ_THREAD;


typedef enum
{
#if defined (CC_MT5365)
    DI_CLK_TYPE_VDOIN = 0,
    DI_CLK_TYPE_OCLK = 1,
    DI_CLK_TYPE_PS_DIG_DIV_CK = 2,
    DI_CLK_TYPE_PIP_PS_DIG_DIV_CK = 3
#elif defined (CC_MT5395)
    DI_CLK_TYPE_OCLK = 0,
    DI_CLK_TYPE_OCLK_DIV_2 = 1,
    DI_CLK_TYPE_VDOIN = 2,
    DI_CLK_TYPE_PS_DIG_DIV_CK = 3,
    DI_CLK_TYPE_PIP_PS_DIG_DIV_CK = 4
#endif
} DI_OUTPUT_CLOCK_TYPE;

typedef enum
{
    DI_TG_FREE_RUN,
    DI_TG_LOCK_TO_SCALER,
} DI_OUTPUT_TIME_GEN;

//Capture Logo Function 20100929
typedef struct{
	UINT8  dram_from;
	UINT8  u1BlockMode;
	UINT8  u1OutMode;
	UINT8  u1BitRes;
	UINT8  u1ColorMode;
	UINT8  u1AFBottom;
	UINT8  u1AFYUVIndex;
	UINT16 u2Width;
	UINT16 u2Height;
	
	UINT8  u1YFrameNum; //1W1R:Y=2  1W4R:Y=5 FLIP_MIRROR_1W1R:Y=3 FLIP_MIRROR_CBOB_1W4R:Y=6
	UINT8  u1CFrameNUm; //1W1R:C=2  1W4R:C=5 FLIP_MIRROR_1W1R:C=3 FLIP_MIRROR_CBOB_1W4R:C=5
	UINT8  u1DramFlipMirror; // 1 : means pscan write dram flip/mirror  ; 0 : means scaler read flip mirror
	UINT8  u1CfgFlipMirror;  // 0 : means normal ; 1 : means mirror ; 2 : means flip ; 3 : means flip & mirror 
	
	UINT32 u4YStartMSBAddr;
	UINT32 u4YStartLSBAddr;
	UINT32 u4UStartMSBAddr;
	UINT32 u4UStartLSBAddr;
	UINT32 u4VStartMSBAddr;
	UINT32 u4VStartLSBAddr;
}DI_DISP_INFO_T;



EXTERN VDP_MDDI_PRM_T _arMDDiPrm[2];

// PSCAN CTRL API
void vDrvDILoadRegTbl(UINT8 u1VdpId, const MDDI_REGTBL_T* prRegTbl);
void vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDIFreezeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDIFrmEncModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff, UINT8 u1BtmFirst);
void vDrvDISetDtvClip(UINT8 u1VdpId, UINT16 u2DTVClip);
void vDrvDIInit(UINT8 u1VdpId);
void vDrvDIFrameRepeat(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetPSCANClock(UINT8 u1VdpId, DI_OUTPUT_CLOCK_TYPE eType);
void vDrvDIChkImpl(UINT8 u1VdpId);
void vDrvDISrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode);
void vDrvDIModeChangeDone(UINT8 u1VdpId);
void vDrvDIDemo(UINT8 u1OnOff, UINT8 u1Region, UINT16 u2Center);
void vDrvDISetNRDebugDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode);
void vDrvDISetCrc(UINT16 u2Mode, UINT16 u2Arg);
#ifdef CC_DOWMBWMODE
void vDrvDISeamlessSwtich(UINT8 u1OnOff);
#endif
void vDrvDISetDramCtrl(UINT8 u1VdpId, UINT8 u1DramCOnOff);
UINT8 u1DrvDIFreezable(UINT8 u1VdpId);
UINT8 u1DrvDIGetFrameDelay(UINT8 u1VdpId);
UINT8 u1DrvDIGetDispmodeFromSRM(UINT8 u1VdpId);
UINT32 u4DrvDIMiniInputWidth(UINT8 u1VdpId);
UINT32 u4DrvDIMaxInputWidth(UINT8 u1VdpId);
UINT32 u4DrvDIGetProgDispmodeClip(UINT8 u1VdpId);

// PSCAN INT API
void vDrvDIFilmModeFreeze(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDIFilmModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
UINT8 bDrvGetFilmStatus(void);
void vDrvDISetBob(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetFilmModeOnOff(UINT8 u1Path, UINT8 u1FilmType, UINT8 u1OnOff);
void vDrvDISetQualityMode(E_DI_ENGINE_TYPE eDIEngineType, E_DI_QUALITY_MODE eDIQualityMode);
void vDrvDISetEvent(UINT8 bNum,UINT8 bMode,UINT32 u4Value);
void vDrvDISetInk(UINT8 bMode, UINT16 u2Type);

#ifdef CC_SUPPORT_H264
void vDrvDISetH264WriteMode(UINT8 bMode);
void vDrvDISendBufToIMGRZ(BOOL bFlag);
#endif

// PSCAN CLI API
EXTERN UINT8 fgDIStatusPolling;
EXTERN DI_PQ_THREAD DIPQThread[];
EXTERN void vDrvDIRecoverOnOff(UINT8 u1VdpId);

void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff);
void vDrvDIFWOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetDebugDataMode(UINT8 bPath, E_DI_DMODE bMode);
void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetPSCANDisplayForDump(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetInputBorder(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetBitRes(UINT8 u1VdpId, UINT8 u1BitRes);
void vDrvDIForceRead8bit(UINT8 u1VdpId, UINT8 b8bit);
void vDrvDISetDispmodeOff(UINT8 u1VdpId);
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv);
void vDrvDISetColorMode(UINT8 u1VdpId, UINT8 u1ColorMode);
void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode);
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv);
void vDrvDISetROIPixel(UINT8 bPath, UINT32 u4RoiPixel);
void vDrvDISetWritePrecision(UINT8 u1VdpId, UINT8 b8bit);
void vDrvDISetClkDiv(UINT8 u1VdpId, UINT16 u2N, UINT16 u2M);
void vDrvDIGetInfo(UINT8 u1VdpId, UINT8 u1DbgIdx);
void vDrvDISetOSD(UINT8 u1Item, UINT8 u1OnOff);
void vDrvDIGetBufAddr(UINT8 u1VdpId, UINT32 *u4AddrStart, UINT32 *u4AddrEnd);
void vDrvDIInitCRC(UINT8 u1VdpId,UINT8 preprocMode);
UINT8 u1DrvDIGetDebugDataMode(UINT8 u1VdpId);
UINT8 u1DrvDIGetDebugPSCANDisplay(UINT8 u1VdpId);
UINT32 u4DrvDIGetHBlank(UINT8 u1VdpId);
UINT16 u2DrvDIGetVcnt(UINT8 u1VdpId);
UINT32 u4DrvDIGetInputCRC(UINT8 u1VdpId);
UINT32 u4DrvDIGetOutputCRC(UINT8 u1VdpId);
UINT16 u2DrvDIGetVcnt(UINT8 u1VdpId);
void vDrvDIDispInfoForCapLog(UINT8 bPath, DI_DISP_INFO_T* pDispInfo); 

#endif    // DRV_DI_H
