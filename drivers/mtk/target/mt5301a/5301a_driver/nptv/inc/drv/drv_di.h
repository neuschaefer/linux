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

#ifdef CC_UP8032_ATV 
#define VDP_NS 2
#else
#include "vdp_if.h"
#endif


#define MDDI_DS_UNIT        0x800
#define MDDI_REGTBL_END	    0xffff
#define MDDI_BLOCK_SIZE     2048

#define PSCAN_MAX_WIDTH_NRM 1920
#define PSCAN_MIN_WIDTH 360
#define PSCAN_MAX_WIDTH_POP_MAIN 960
#define PSCAN_MAX_WIDTH_POP_SUB 720
#define PSCAN_MAX_WIDTH_PIP_MAIN PSCAN_MAX_WIDTH_NRM
#define PSCAN_MAX_WIDTH_PIP_SUB 1920


#if defined(CC_MT5387) && !defined(CC_MT5363)
#define MDDI_WA1
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

#define MDDI_REG_NUM 0x800

#define MDDI_READ_FLD(u1VdpId, u2Addr, u4Fld) \
     RegReadFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Fld)

//#define LOG_MDDI_REG
#ifndef LOG_MDDI_REG
#define MDDI_WRITE_FLD(u1VdpId, u2Addr, u4Value, u4Fld) \
    vRegWriteFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Fld)

#define MDDI_WRITE32_MSK(u1VdpId, u2Addr, u4Value,  u4Msk) \
    vRegWrite4BMsk((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Msk)
#else
void MDDI_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld);
void MDDI_WRITE32_MSK(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Msk);
#endif

typedef enum
{
    DI_NORMAL_MODE = 0,
    DI_MIRROR_ON_MODE,
    DI_FLIP_ON_MODE,
    DI_FLIP_MIRROR_ON_MODE
}DI_DISPLAY_MODE;

typedef enum
{
    MDDI_422_MODE,
    MDDI_420_MODE
}DI_COLOR_MODE;

typedef enum
{
    DI_YUV_MODE_Y_ONLY,
    DI_YUV_MODE_YC,
    DI_YUV_MODE_YUV
}DI_YUV_MODE;

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
    E_DI_NR_OFF,                    // NR Off mode
    E_DI_NR_Y_ONLY,               // NR Y Only mode
    E_DI_NR_YC_420,    // NR YC 420 mode 
    E_DI_NR_YC_422,    // NR YC 422 mode 
    E_DI_NR_RGB,         // NR RGB mode
    E_DI_NR_UNKNOWN
} E_NR_DMODE;

typedef enum
{
    E_DI_OUT_V_F= 0x8,  // Output VSync rising
    E_DI_OUT_V_R= 0x4,   // Output VSync falling
    E_DI_IN_V_F = 0x2,    // Input VSync rising
    E_DI_IN_V_R = 0x1     // Input VSync falling
} E_DI_INT_MODE;

typedef struct
{
    E_DI_DMODE eDataMode;
    const CHAR* szName;
} DI_MAPPING_T;

typedef struct
{
    E_NR_DMODE eDataMode;
    const CHAR* szName;
} NR_MAPPING_T;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Mode;
    BOOL fg10bit;
    BOOL fg422;    
} MDDI_FBM_POOL_T;

typedef struct
{
    UINT8 u1Region;
    UINT16 u2Center;

    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT16 u2Width;
    UINT16 u2Height;

    UINT16 u2HClip;
    UINT16 u2VClip;
    UINT16 u2DTVClip;
	UINT8 u1SigType;
    UINT8 u1BitRes;
    UINT8 u1DecType;
    UINT8 u1Interlace;
    UINT8 u1420Mode;
    UINT8 u1OnOff;

    UINT8 u1Bob;
    UINT8 u1NewBob;	
    UINT8 u1PSCANDisplay;
    UINT8 u1DebugPSCANDisplay;
    UINT8 u1DebugIndex;
    UINT8 u1Freeze;
    UINT8 u1CsEnable;
    UINT8 u1DeRule;
    UINT8 u1StatusCnt;
    #if 0
    UINT8 u1UpdateCnt;
    UINT8 u1UpdateFlg;
    #endif
    
    UINT16 u2BufWidth;
    UINT16 u2BufHeight;
    UINT32 u4BufAddr;
    E_DI_DMODE u1DataMode;
    E_DI_DMODE u1DebugDataMode;	
    E_NR_DMODE u1NRDataMode;
    E_NR_DMODE u1DebugNRDataMode;    
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
    E_DI_ENGINE_MA,
    E_DI_ENGINE_EDGEP,
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
    DI_ROOT_CLK_27M = 0,
    DI_ROOT_CLK_172M = 1, //172.8M
    DI_ROOT_CLK_162M = 2,
    DI_ROOT_CLK_DIV = 3
} DI_ROOT_CLOCK_TYPE;

typedef enum
{
    DI_OCLK_TYPE_ROOT = 0,
    DI_OCLK_TYPE_OCLK = 1,
    DI_OCLK_TYPE_DIV_CK = 2,
    DI_OCLK_OFF = 3
} DI_OUTPUT_CLOCK_TYPE;

typedef enum
{
    DI_KCLK_TYPE_ROOT_DIV_2 = 0,
    DI_KCLK_TYPE_OCLK_DIV_2 = 1,
    DI_KCLK_TYPE_MAIN_DIV_2 = 2,
    DI_KCLK_TYPE_ROOT = 3, 
    DI_KCLK_TYPE_86M = 4,       //86.4M
    DI_KCLK_OFF = 5
} DI_KERNEL_CLOCK_TYPE;

typedef enum
{
    DI_CLK_DIV_BY_2 = 0,
    DI_CLK_DIV_BY_1 = 1
} DI_CLK_DIV_TYPE;

typedef enum
{
    DI_TG_FREE_RUN,
    DI_TG_LOCK_TO_SCALER,
} DI_OUTPUT_TIME_GEN;

extern VDP_MDDI_PRM_T _arMDDiPrm[2];


/**
 * @brief Set bit resolution
 * @param u1VdpId VDP_1/VDP_2
 * @param u1BitRes Bit resolution (8 ~ 10)
 */
void vDrvDISetBitRes(UINT8 u1VdpId, UINT8 u1BitRes);

/**
 * @brief Enable/disable MDDi FW
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 */
void vDrvDIFWOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

/**
 * @brief Load register table
 * @param u1VdpId VDP_1/VDP_2
 * @param arRegTbl Register table
 */
void vDrvDILoadRegTbl(UINT8 u1VdpId, const MDDI_REGTBL_T* prRegTbl);

/**
 * @brief Turn on/off trick mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief Enable/disable PSCAN freeze mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDIFreezeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief Check if freeze function works under the current configuration
* @param u1VdpId VDP_1/VDP_2
* @return SV_TRUE/SV_FALSE
*/
UINT8 u1DrvDIFreezable(UINT8 u1VdpId);

/**
 * @brief Turn on/off film mode Freeze
 * @param u1VdpId SV_VP_MAIN/SV_VP_PIP
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDIFilmModeFreeze(UINT8 u1VdpId, UINT8 u1OnOff);

/**
 * @brief Turn on/off film mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by UI only
 */
void vDrvDIFilmModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

/**
 * @brief Turn on/off 32 film mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 */
void vDrvDI32FilmOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

/**
 * @brief Turn on/off frame encode mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @param u1BtmFirst SV_TRUE/SV_FALSE
 * @warning This should be invoked by B2R only
 */
void vDrvDIFrmEncModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff, UINT8 u1BtmFirst);

/** 
 * @brief DTV set V Clip 
 * @param u1VdpId VDP_1/VDP_2
 * @param u2VClip Clip size */
void vDrvDISetDtvClip(UINT8 u1VdpId, UINT16 u2DTVClip);

/** Brief of vDrvDISetInk.
 * @brief Set Quality Ink
 */
void vDrvDISetInk(UINT8 bMode,UINT16 u2Type);

/** Brief of _vDrvDIInit.
 * @brief Initialize MDDi
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIInit(UINT8 u1VdpId);

#if SUPPORT_SEAMLESS_MODE
/** Brief of vDrvDIDemo
 * @brief Set split line for demo mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @param u1Region SV_LEFT/SV_RIGHT
 * @param u2Center Center line position
 */
void vDrvDIDemo(UINT8 u1VdpId, UINT8 u1OnOff, UINT8 u1Region, UINT16 u2XCenter);
#endif //SUPPORT_SEAMLESS_MODE

/** Brief of vDrvDIChkImpl.
 * @brief Set MDDi control registers
 * @param u1VdpId SV_VP_MAIN/SV_VP_PIP
 */
void vDrvDIChkImpl(UINT8 u1VdpId);

/* * @brief Return film mode status
 * @param u1VdpId VDP_1/VDP_2
 * @return SV_TRUE/SV_FALSE
 */
UINT8 u1DrvGetFilmOnOff(UINT8 bPath);

/*@brief Set Film Mode On/Off
*@param u1VdpId
*@param bEnable
*/
void vDrvDIFilmModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief Set PSCAN Y/C interpolation mode
* @param u1VdpId VDP_1/VDP_2
* @param bMode E_DI_NOBOB/E_DI_YCBOB/E_DI_CBOB/E_DI_FLIP_MIRROR
*/
void vDrvDISetInterpMode(UINT8 u1VdpId, E_DI_DMODE bMode);

/**
* @brief Get PSCAN Frame Delay
* @param u1VdpId SV_VP_MAIN/SV_VP_SUB
* @rtVal Delay Field of PSCAN
*/
UINT8 vDrvDIGetFrameDelay(UINT8 u1VdpId);

/**
* @brief Set DI Quality Mode
* @param eDIEngineType E_DI_ENGINE_MA/E_DI_ENGINE_EDGEP/E_DI_ENGINE_FILM
* @param eDIQualityMode E_DI_QUALITY_AGGRESSIVE / E_DI_QUALITY_DEFENSIVE
*/
void vDrvDISetQualityMode(E_DI_ENGINE_TYPE eDIEngineType,E_DI_QUALITY_MODE eDIQualityMode);

/**
 * @brief Return minimum value of Horizontal width
 * @param u1VdpId VDP_1/VDP_2
 * @return  u4MiniInputWidth         
 */
UINT32 u4DrvDIMiniInputWidth(UINT8 u1VdpId);

/**
 * @brief Return maxmum value of Horizontal width
 * @param u1VdpId VDP_1/VDP_2
 * @return  u4MaxInputWidth         
 */
UINT32 u4DrvDIMaxInputWidth(UINT8 u1VdpId);

/**
* @brief DI Frame Repeat
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
* @warning This should be invoked by B2R only
*/
void vDrvDIFrameRepeat(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief Set NR Data Mode
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetNRDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode);

/**
* @brief Set pscan output clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vDrvDISetOutputClock(UINT8 u1VdpId, DI_OUTPUT_CLOCK_TYPE eType);

/**
* @brief Set pscan output clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vDrvDISetPSCANClock(UINT8 u1VdpId, DI_OUTPUT_CLOCK_TYPE eType);

/**
* @brief Set pscan output clock div
* @param bPath VDP_1/VDP_2
* @param N numerator
* @param M denominator
*/
void vDrvDISetClkDiv(UINT8 u1VdpId, UINT16 u2N, UINT16 u2M);

/**
* @brief Set pscan output time gen
* @param bPath VDP_1/VDP_2
* @param eType time gen
*/
void vDrvDISetOutputTimeGen(UINT8 u1VdpId, DI_OUTPUT_TIME_GEN eType);

/**
* @brief Get horizontal blank
* @param bPath VDP_1/VDP_2
* @return  horizontal blank
*/
UINT32 dwDrvDIGetHBlank(UINT8 u1VdpId);

/**
* @brief PSCAN callback function
* @param u4Arg1 VDP_1/VDP_2
* @param u4Arg2 configuration
* @param u4SramMode sram mode
*/
void vDrvDISrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode);

/**
* @brief Set NR Data mode
* @param u1VdpId VDP_1/VDP_2
* @param u1DataMode datamode
*/
void vDrvDISetNRDebugDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode);

/**
* @brief Enable/disable debug PSCAN display mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief PSCAN rounte for mode change done
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIModeChangeDone(UINT8 u1VdpId);

/**
* @brief 
* @param u1VdpId VDP_1/VDP_2, bType E_DI_INT_MODE
*/
void vDrvDIChangeISRType(UINT8 u1VdpId, UINT8 bType);


#ifdef __MODEL_slt__
/**
* @brief Set PSCAN CRC test
*/
void vDrvDICRCTest(void);

/**
* @brief Set PSCAN CRC clear
* @bOnOff clear on/off
*/
void vDrvDISetCRCClr(UINT8 bOnOff);

/**
* @brief Set PSCAN CRC trigger
* @bOnOff clear on/off
*/
void vDrvDISetCRCTrigger(UINT8 bOnOff);

/**
* @brief Get PSCAN CRC output pixel result
* @return output pixel result
*/
UINT32 vDrvDIGetCRCResult(void);

/**
* @brief Get PSCAN Source CRC output pixel result
* @return output pixel result
*/
UINT32 vDrvDIGetSrcCRC(void);

/**
* @brief CRC pre-processing
*/
void vDrvDICRCPreProcess(void);
#endif

/**
* @brief Set field inverse
* @param u1VdpId VDP_1/VDP_2
* @param u1Inv 
*/
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv);

/**
* @brief Switch between 420/422 color mode
* @param u1VdpId VDP_1/VDP_2
* @param u1ColorMode MDDI_422_MODE/MDDI_420_MODE
*/
void vDrvDISetColorMode(UINT8 u1VdpId, UINT8 u1ColorMode);

/**
* @brief Get oversample information
* @param u1VdpId VDP_1/VDP_2
* @return oversample timing or not
*/
UINT8 bDrvDIIsOversampleOver720(UINT8 u1VdpId);

/**
* @brief Set Flip Mirror Mode
* @param bPath VDP_1/VDP_2
* @param bMode
*/
void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode);

/**
* @brief Configure Flip Mirror Mode
* @param bPath VDP_1/VDP_2
* @param bMode
*/
void vDrvDIConfigFlipMirror(void);

/**
* @brief workaround for ink error
* @param u2Width input width
* @param workaround width
*/
#ifdef MDDI_WA1
UINT16 vDrvDISetWAWidth(UINT16 u2Width);
#endif

/**
* @brief Set AUTO STOP
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief Get AUTO STOP
* @param u1VdpId VDP_1/VDP_2
* @return u1OnOff On/Off
*/
UINT8 u1DrvDIGetAutoStop(UINT8 u1VdpId);

/**
* @brief DI Field Prediction
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDISetFldPrediction(UINT8 u1VdpId, UINT8 u1OnOff);

#if 0
/**
* @brief PSCAN parameters update status
* @param u1VdpId VDP_1/VDP_2
*/
UINT8 u1DrvDIfgIsUpdateReady(UINT8 u1VdpId);
#endif


//8223 PORTING
void vDrvDIColockEn(UINT8 u1VdpId, UINT8 bEnable);
void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff); 

#endif    // DRV_DI_H

