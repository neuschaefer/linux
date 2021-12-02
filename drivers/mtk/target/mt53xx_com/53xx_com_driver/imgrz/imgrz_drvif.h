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
 * $RCSfile: imgrz_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file imgrz_drvif.h
 *  image resizer driver internal public interfaces
 *  
 */

#ifndef IMGRZ_DRVIF_H
#define IMGRZ_DRVIF_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#if defined(CC_DEBUG)   // debug mode
    #define IMGRZ_DEBUG_MODE
#endif


#if defined(CC_MINI_DRIVER)
    #define IMGRZ_MINI_DRIVER
    //#define IMGRZ_RISC_MODE
#endif


#if defined(IMGRZ_DEBUG_MODE)
   //#define IMGRZ_ENABLE_SW_MODE
   //#define IMGRZ_AUTO_TEST
   //#define IMGRZ_DRAM_ACCESS_BYTE_MEASUREMENT
#endif

#define IMGRZ_RISC_MODE
#ifdef __KERNEL__  //linux use polling mode
//#define IMGRZ_POLLING
#endif
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define IMGRZ_CRASH_DETECT_PERIOD   (10000)  // 10 sec

#define IMGRZ_ENABLE            1
#define IMGRZ_DISABLE           0

#define IMGRZ_TRUE              1
#define IMGRZ_FALSE             0

#define IMGRZ_SWAP              6
#define IMGRZ_MAX_LBUFL         31

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// scaling factor & offset
typedef struct _RZ_SCL_FAC_OFT_T
{
    UINT32 u4YFactor; 
    UINT32 u4YOffset;
    UINT32 u4CbFactor; 
    UINT32 u4CbOffset; 
    UINT32 u4CrFactor; 
    UINT32 u4CrOffset;

    // for VScl
    UINT32 u4VupY;
    UINT32 u4VupCb;
    UINT32 u4VupCr;
} RZ_SCL_FAC_OFT_T;

// VDO scale parameter setting
typedef struct _RZ_VDO_SCL_PARAM_SET_T
{
    UINT32 u4IsRsIn;        // block|raster based input
    UINT32 u4InMode;        // 420|422 input
    UINT32 u4YSrcBase;      // y
    UINT32 u4YSrcBufLen;
    UINT32 u4YSrcHOffset;
    UINT32 u4YSrcVOffset;
    UINT32 u4YSrcW;
    UINT32 u4YSrcH;
    UINT32 u4CSrcBase;      // cbcr
    UINT32 u4CSrcHOffset;
    UINT32 u4CSrcVOffset;
    UINT32 u4CSrcW;
    UINT32 u4CSrcH;

    UINT32 u4IsRsOut;       // block|raster based output
    UINT32 u4OutMode;       // 420|422|444 output
    UINT32 u4IsVdo2Osd;     // output in osd format ?
    UINT32 u4YTgBase;       // y
    UINT32 u4YTgCM;
    UINT32 u4YTgBufLen;
    UINT32 u4YTgHOffset;
    UINT32 u4YTgVOffset;
    UINT32 u4YTgW;
    UINT32 u4YTgH;
    UINT32 u4CTgBase;       // cbcr
    UINT32 u4CTgW;
    UINT32 u4CTgH;

	UINT32 u4VFilterMode;
	UINT32 u4HFilterMode;
    UINT32 u4SrcSwap;
    UINT32 u4OutSwap;
} RZ_VDO_SCL_PARAM_SET_T;

// JPG scale parameter setting
typedef struct _RZ_JPG_SCL_PARAM_SET_T
{
    UINT32 u4IsRsIn;        // block|raster based input
    UINT32 u4YSrcBase1;     // y1
    UINT32 u4YSrcBase2;     // y2
    UINT32 u4YSrcBufLen;
    UINT32 u4YSrcHOffset;
    UINT32 u4YSrcVOffset;
    UINT32 u4YSrcW;
    UINT32 u4YSrcH;
    UINT32 u4CbSrcBase1;    // cb1
    UINT32 u4CbSrcBase2;    // cb2
    UINT32 u4CbSrcHOffset;
    UINT32 u4CbSrcVOffset;
    UINT32 u4CbSrcW;
    UINT32 u4CbSrcH;
    UINT32 u4CrSrcBase1;    // cr1
    UINT32 u4CrSrcBase2;    // cr2
    UINT32 u4CrSrcHOffset;
    UINT32 u4CrSrcVOffset;
    UINT32 u4CrSrcW;
    UINT32 u4CrSrcH;

    UINT32 u4IsRsOut;       // block|raster based output
    UINT32 u4OutMode;       // 420|422|444 output
    UINT32 u4IsVdo2Osd;     // output in osd format ?
    UINT32 u4YTgBase;       // y
    UINT32 u4YTgCM;
    UINT32 u4YTgBufLen;
    UINT32 u4YTgHOffset;
    UINT32 u4YTgVOffset;
    UINT32 u4YTgW;
    UINT32 u4YTgH;
    UINT32 u4CTgBase;       // cbcr
    UINT32 u4CTgW;
    UINT32 u4CTgH;

    UINT32 u4IsFstBl;       // first block line of jpg
    UINT32 u4IsLstBl;       // last  block line of jpg
    UINT32 u4JpgVfacY;
    UINT32 u4JpgVfacCb;
    UINT32 u4JpgVfacCr;
    UINT32 u4IsJpgRzOn;     // jpg and rz hand-off on ?
    UINT32 u4IsJpgSameaddr; 
    UINT32 u4ColorComp;
    UINT32 u4JpgTempAddr;
    UINT32 u4OutSwap;
    BOOL   fgGray;
    BOOL   fgPreload;
    BOOL   fgBlassign;    
} RZ_JPG_SCL_PARAM_SET_T;

// OSD scale parameter setting
typedef struct _RZ_OSD_SCL_PARAM_SET_T
{
    UINT32 u4SrcBase;
    UINT32 u4IsSrcDirCM;    // direct|index color mode
    UINT32 u4SrcCM;
    UINT32 u4SrcBufLen;
    UINT32 u4SrcHOffset;
    UINT32 u4SrcVOffset;
    UINT32 u4SrcW;
    UINT32 u4SrcH;

    UINT32 u4TgBase;
    UINT32 u4TgCM;
    UINT32 u4TgBufLen;
    UINT32 u4TgHOffset;
    UINT32 u4TgVOffset;
    UINT32 u4TgW;
    UINT32 u4TgH;

    UINT32 u4CPTDataCount;
    UINT32 *pu4CPTData;
    UINT32 u4PartialUpdate;
    UINT32 u4PartialUpdate_ClipSrcx;    
    UINT32 u4PartialUpdate_ClipSrcy;    
    UINT32 u4PartialUpdate_ClipSrcw;    
    UINT32 u4PartialUpdate_ClipSrch;        
    UINT32 u4PartialUpdate_ClipTgx;    
    UINT32 u4PartialUpdate_ClipTgy;    
    UINT32 u4PartialUpdate_ClipTgw;    
    UINT32 u4PartialUpdate_ClipTgh;     
} RZ_OSD_SCL_PARAM_SET_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

// assertion
#define IMGRZ_ASSERT(x)             ASSERT(x)

// check 16-byte align
#define IMGRZ_CHK_16B_ALIGN(num)    IMGRZ_ASSERT(!(num & 0xF))

// check 32-byte align
#define IMGRZ_CHK_32B_ALIGN(num)    IMGRZ_ASSERT(!(num & 0x1F))

// do 16-byte shrink
#define IMGRZ_DO_16B_SHRINK(num)    ((UINT32)(num >> 4))

// ignore return value (for lint happy)
#define IMGRZ_UNUSED_RET(X)           \
    {                               \
        INT32 i4Ignore;             \
        i4Ignore = (INT32)(X);      \
        UNUSED(i4Ignore);           \
    }


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern BOOL _IMGRZ_IsFlushing(void);

extern void _IMGRZ_SetFlushStatus(BOOL fgStatus);

extern void _IMGRZ_SetCrashDetection(UINT32 u4Value);

extern UINT32 _IMGRZ_GetCrashDetection(void);

extern HANDLE_T _IMGRZ_GetCrashDetectTimer(void);

extern UINT32 _IMGRZ_GetScaleLine(void);

extern void _IMGRZ_ReInit(void);

extern void _IMGRZ_Init(void);

extern void _IMGRZ_Reset(void);

extern void _IMGRZ_Wait(void);

extern void _IMGRZ_MwFlush(void);

extern void _IMGRZ_Flush(void);

extern void _IMGRZ_Lock(void);

extern void _IMGRZ_TryLock(void);

extern void _IMGRZ_Unlock(void);

extern void _IMGRZ_LockCmdque(void);

extern void _IMGRZ_UnlockCmdque(void);

extern void _IMGRZ_LockWaitIsr(void);

extern void _IMGRZ_UnlockWaitIsr(void);
extern INT32 _IMGRZ_QueryHwIdle(void);

extern void _IMGRZ_SetNotify(void (*pfnNotifyFunc)(UINT32));

extern void _IMGRZ_SetRasterScanInOut(BOOL fgRasterScanIn, BOOL fgRasterScanOut);

extern void _IMGRZ_SetInternalSRAM(UINT32 u4Is16Bpp, UINT32 u4LinBufLen);

extern void _IMGRZ_SetH8TapScaleFacOft(const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetHsaScaleFacOft(const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetH8TapFilterCoeff(const UINT32 *psu4CoeffTbl);

extern void _IMGRZ_SetV4TapScaleFacOft(const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetVSclScaleFacOft(const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetV4TapFilterCoeff(const UINT32 *psu4CoeffTbl);

extern void _IMGRZ_SetABlendOpt(UINT32 u4Level, UINT32 u4Alpha);

extern void _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_T eInOutType);

extern E_RZ_INOUT_TYPE_T _IMGRZ_GetScaleOpt(void);

extern void _IMGRZ_SwitchToV4TapFilter(void);

extern void _IMGRZ_SaveToSameAddr(UINT32 u4AddrRST);

extern void _IMGRZ_SwitchToVSclFilter(UINT32 u4SrcH, UINT32 u4TgH, UINT32 u4YTgW, 
                                                E_RZ_INOUT_TYPE_T eType);

extern void _IMGRZ_BypassHsaFilter(void);

extern void _IMGRZ_BypassH8TapFilter(void);

extern void _IMGRZ_BypassVSclFilter(void);

extern void _IMGRZ_BypassV4TapFilter(void);

extern void _IMGRZ_Scale(void *pvSclParam);

extern UINT32 _IMGRZ_HwGetCurrentLine(void);

extern void _IMGRZ_SetHoriFilterOpt(
    UINT32 u4YSrcW, UINT32 u4CbSrcW, UINT32 u4CrSrcW, 
    UINT32 u4YTgW, UINT32 u4CbTgW, UINT32 u4CrTgW, UINT32 u4FilterType,
    UINT32 u4IsPartialUpdate);

extern void _IMGRZ_SetVertFilterOpt(
    UINT32 u4YSrcH, UINT32 u4CbSrcH, UINT32 u4CrSrcH, 
    UINT32 u4YTgH, UINT32 u4CbTgH, UINT32 u4CrTgH, UINT32 u4YTgW, 
    UINT32 u4FilterType,E_RZ_INOUT_TYPE_T eType);


//===================
// VDO mode
//===================
extern void _IMGRZ_VdoSetOutMode(UINT32 u4IsVdo2Osd, UINT32 u4VdoOutMode);

extern void _IMGRZ_VdoSetSrcBuf(UINT32 u4YBaseAddr, UINT32 u4YBufLen, 
    UINT32 u4YHOffset, UINT32 u4YVOffset, 
    UINT32 u4CBaseAddr, UINT32 u4CHOffset, UINT32 u4CVOffset);

extern void _IMGRZ_VdoSetTgBuf(UINT32 u4YBaseAddr, UINT32 u4YBufLen, 
    UINT32 u4YHOffset, UINT32 u4YVOffset,
    UINT32 u4CBaseAddr);

extern void _IMGRZ_VdoScale(UINT32 u4YSrcW, UINT32 u4YSrcH, 
    UINT32 u4CSrcW, UINT32 u4CSrcH, 
    UINT32 u4YTgW, UINT32 u4YTgH);


extern void _IMGRZ_VdoSetSwap(UINT32 u4InSwap, UINT32 u4OutSwap);
//===================
// JPG mode
//===================
extern void _IMGRZ_JpgSetTmpBuf(UINT32 u4BaseAddr);

extern void _IMGRZ_JpgSetMiscParam(UINT32 u4IsFstBl, UINT32 u4IsLstBl, 
    UINT32 u4JpgVfacY, UINT32 u4JpgVfacCb, UINT32 u4JpgVfacCr,
    UINT32 u4JpgRzOn, UINT32 u4ColorComp);

extern void _IMGRZ_JpgSetSrcBuf(UINT32 u4YBaseAddr1, UINT32 u4YBaseAddr2, 
    UINT32 u4YBufLen, 
    UINT32 u4YHOffset, UINT32 u4YVOffset, 
    UINT32 u4CbBaseAddr1, UINT32 u4CbBaseAddr2, 
    UINT32 u4CbHOffset, UINT32 u4CbVOffset,
    UINT32 u4CrBaseAddr1, UINT32 u4CrBaseAddr2, 
    UINT32 u4CrHOffset, UINT32 u4CrVOffset);

extern void _IMGRZ_JpgScale(UINT32 u4YSrcW, UINT32 u4YSrcH, 
    UINT32 u4CbSrcW, UINT32 u4CbSrcH, 
    UINT32 u4CrSrcW, UINT32 u4CrSrcH, 
    UINT32 u4YTgW, UINT32 u4YTgH);
extern void _IMGRZ_JpgEnableBLAssign(UINT32 u4SrcYHeight,
  UINT32 u4SrcCbHeight,UINT32 u4SrcCrHeight);


//===================
// OSD mode
//===================
extern void _IMGRZ_OsdSetMiscParam(UINT32 u4IsSrcDirCM, 
    UINT32 u4CPTDataCount, UINT32 *pu4CPTData);

extern void _IMGRZ_OsdSetSrcBuf(UINT32 u4BaseAddr, 
    UINT32 u4IsDirCM, UINT32 u4CM, UINT32 u4BufLen, 
    UINT32 u4HOffset, UINT32 u4VOffset);

extern void _IMGRZ_OsdSetTgBuf(UINT32 u4BaseAddr, 
    UINT32 u4DirCM, UINT32 u4BufLen, 
    UINT32 u4HOffset, UINT32 u4VOffset);

extern void _IMGRZ_OsdScale(UINT32 u4SrcW, UINT32 u4SrcH, 
    UINT32 u4TgW, UINT32 u4TgH);

extern void _IMGRZ_OsdSetIdxTranslation(UINT32 u4CM);

extern void _IMGRZ_OsdPartialUpdate(RZ_OSD_SCL_PARAM_SET_T *prOsdParam);

//---------------------------------------------------------------------------
// Public functions (high-level APIs)
//---------------------------------------------------------------------------

extern void _IMGRZ_Break(void);

extern void _IMGRZ_Resume(void);

extern void _IMGRZ_OFF(void);

#endif // IMGRZ_DRVIF_H



