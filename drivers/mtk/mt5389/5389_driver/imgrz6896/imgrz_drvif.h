/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
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
   //#define IMGRZ_SEMI_HOST_BY_USB
   //#define IMGRZ_DRAM_ACCESS_BYTE_MEASUREMENT
#endif

#define IMGRZ_RISC_MODE
#ifdef __KERNEL__  //linux use polling mode
//#define IMGRZ_POLLING
#endif


#ifndef IMGRZ_HW_V5
#if defined(CC_MT5398)
#define IMGRZ_HW_V5	//osd turbo
	#ifdef IMGRZ_HW_V5
		#define IMGRZ_HW_A_COMPOSITION
		#ifdef IMGRZ_HW_A_COMPOSITION
		#define IMGRZ_HW_A_COMPOSITION_PRELOAD_ADDR			0xc000000	//for emulation test
		#endif
	#endif	
#endif
#endif

#ifndef IMGRZ_HW_V4
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
#define IMGRZ_HW_V4	//5368 version
//#define IMGRZ_HW_IO_MMU
	#ifdef IMGRZ_HW_IO_MMU
	//#define IMGRZ_HW_IO_MMU_RW_OSD_MODE		//MMU for read and write
	#define IMGRZ_HW_IO_MMU_W_VDOJPG_MODE	//MMU only for write
	#endif
#if defined(CC_MT5396)
#define IMGRZ_HW_2ND_RZ		//0x6c000	    
#endif
#define IMGRZ_HW_A_BLENDING	
	#ifdef IMGRZ_HW_A_BLENDING
	#define IMGRZ_HW_A_BLENDING_PRELOAD_ADDR		0xc000000	//for emulation test
	#endif
#endif
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
    UINT32 u4CSrcBufLen;
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
    UINT32 u4CTgBufLen;
    UINT32 u4YTgHOffset;
    UINT32 u4YTgVOffset;
    UINT32 u4YTgW;
    UINT32 u4YTgH;
    UINT32 u4CTgBase;       // cbcr
    UINT32 u4CTgW;
    UINT32 u4CTgH;
    UINT32 u4SrcSwap;
    UINT32 u4OutSwap;
    //For webp 
    UINT32 u4IsFstBl;       // first block line of Webp
    UINT32 u4IsLstBl;       // last  block line of webp
    UINT32 u4LoopFilterEnable;
    UINT32 u4IsVdoPartialModeEnable;     // WEBP and rz hand-off on ?		
    UINT32 u4IsWebPRzOn;     // WEBP and rz hand-off on ?	
    UINT32 u4IsSameaddr;
    UINT32 u4RzTmpBuf;
    UINT32 u4Venc_skip;		//5368, E_RZ_INOUT_TYPE_VDOENCODE_V2
    UINT32 u4VdoCbCrSwap;
    UINT32 u4ABlend;
    UINT32 u4PreLoadYAddr;
    UINT32 u4PreLoadCAddr;
} RZ_VDO_SCL_PARAM_SET_T;

// JPG scale parameter setting
typedef struct _RZ_JPG_SCL_PARAM_SET_T
{
    UINT32 u4IsRsIn;        // block|raster based input
    UINT32 u4YSrcBase1;     // y1
    UINT32 u4YSrcBase2;     // y2
    UINT32 u4YSrcBufLen;
    UINT32 u4CSrcBufLen;    
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
    UINT32 u4CTgBufLen;
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

    //paritial output
    BOOL fgJpgModePartialOutput;
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

	//seamless update
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

	//csc setting
	UINT32 u4Csc_enable; 
	E_RZ_CSC_FORMAT_T	csc_src_format;
	E_RZ_CSC_FORMAT_T	csc_des_format;	
	
    UINT32 u4IsFstBl;       // first block line of png
    UINT32 u4IsLstBl;       // last  block line of png
    BOOL	 fgBlassign;
    BOOL   fgPartialMode;
    UINT32 u4RowBufHeight;
    UINT32 u4IsPngSameaddr; 
    BOOL   fgOSDTurboMode;
    BOOL   fgAlphaComposition;
    UINT32 u4AlphaComposition_Mode;    
    UINT32 u4AlphaComposition_PreloadAddr;
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

extern BOOL _IMGRZ_IsFlushing(UINT32 u4Which);

extern void _IMGRZ_SetFlushStatus(UINT32 u4Which, BOOL fgStatus);

extern void _IMGRZ_SetCrashDetection(UINT32 u4Which, UINT32 u4Value);

extern UINT32 _IMGRZ_GetCrashDetection(UINT32 u4Which);

extern HANDLE_T _IMGRZ_GetCrashDetectTimer(UINT32 u4Which);

extern UINT32 _IMGRZ_GetScaleLine(void);
extern void   _IMGRZ_RegInfo(UINT32 u4Which, UINT32 *pu4Addr,UINT32 *pu4Size);

extern void _IMGRZ_ReInit(UINT32 u4Which);

extern void _IMGRZ_Init(UINT32 u4Which);
extern void _IMGRZ_Cfg_MMU_IO(UINT32 u4Which, BOOL MMU_Read, BOOL MMU_Write);
extern void _IMGRZ_Reset(UINT32 u4Which);

extern void _IMGRZ_Wait(UINT32 u4Which);

extern void _IMGRZ_MwFlush(UINT32 u4Which);

extern void _IMGRZ_Flush(UINT32 u4Which);

extern void _IMGRZ_Lock(UINT32 u4Which);

extern void _IMGRZ_TryLock(UINT32 u4Which);

extern void _IMGRZ_Unlock(UINT32 u4Which);

extern void _IMGRZ_LockCmdque(UINT32 u4Which);

extern void _IMGRZ_UnlockCmdque(UINT32 u4Which);

extern void _IMGRZ_LockWaitIsr(UINT32 u4Which);

extern void _IMGRZ_UnlockWaitIsr(UINT32 u4Which);
extern INT32 _IMGRZ_QueryHwIdle(UINT32 u4Which);

extern void _IMGRZ_SetNotify(UINT32 u4Which, void (*pfnNotifyFunc)(UINT32));

extern void _IMGRZ_SetRasterScanInOut(UINT32 u4Which, BOOL fgRasterScanIn, BOOL fgRasterScanOut);

extern void _IMGRZ_SetInternalSRAM(UINT32 u4Which, UINT32 u4Is16Bpp, UINT32 u4LinBufLen, UINT32 u4I16BppTbuf);

extern void _IMGRZ_SetH8TapScaleFacOft(UINT32 u4Which, const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetHsaScaleFacOft(UINT32 u4Which, const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetH8TapFilterCoeff(UINT32 u4Which, const UINT32 *psu4CoeffTbl);

extern void _IMGRZ_SetV4TapScaleFacOft(UINT32 u4Which, const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetVSclScaleFacOft(UINT32 u4Which, const RZ_SCL_FAC_OFT_T *psrFacOft);

extern void _IMGRZ_SetV4TapFilterCoeff(UINT32 u4Which, const UINT32 *psu4CoeffTbl);

extern void _IMGRZ_SetABlendOpt(UINT32 u4Which, UINT32 u4Level, UINT32 u4Alpha, UINT8 uABlend);

extern void _IMGRZ_SetScaleOpt(UINT32 u4Which, E_RZ_INOUT_TYPE_T eInOutType);

extern E_RZ_INOUT_TYPE_T _IMGRZ_GetScaleOpt(UINT32 u4Which);

extern void _IMGRZ_SwitchToV4TapFilter(UINT32 u4Which);

extern void _IMGRZ_SaveToSameAddr(UINT32 u4Which, UINT32 u4AddrRST);

extern void _IMGRZ_SwitchToVSclFilter(UINT32 u4Which, UINT32 u4SrcH, UINT32 u4TgH, UINT32 u4YTgW, 
                                                E_RZ_INOUT_TYPE_T eType);

extern void _IMGRZ_BypassHsaFilter(UINT32 u4Which);

extern void _IMGRZ_BypassH8TapFilter(UINT32 u4Which);

extern void _IMGRZ_BypassVSclFilter(UINT32 u4Which);

extern void _IMGRZ_BypassV4TapFilter(UINT32 u4Which);

extern void _IMGRZ_Scale(UINT32 u4Which, void *pvSclParam);

extern UINT32 _IMGRZ_HwGetCurrentLine(UINT32 u4Which);

extern void _IMGRZ_SetHoriFilterOpt(UINT32 u4Which, 
    UINT32 u4YSrcW, UINT32 u4CbSrcW, UINT32 u4CrSrcW, 
    UINT32 u4YTgW, UINT32 u4CbTgW, UINT32 u4CrTgW, UINT32 u4IsPartialUpdate);

extern void _IMGRZ_SetVertFilterOpt(UINT32 u4Which, 
    UINT32 u4YSrcH, UINT32 u4CbSrcH, UINT32 u4CrSrcH,
    UINT32 u4YTgH, UINT32 u4CbTgH, UINT32 u4CrTgH, UINT32 u4YTgW, E_RZ_INOUT_TYPE_T eType,BOOL fgPartialMode, BOOL fgOSDTurboMode);


//===================
// VDO mode
//===================
extern void _IMGRZ_VdoSetOutMode(UINT32 u4Which, UINT32 u4IsVdo2Osd, UINT32 u4VdoOutMode);

extern void _IMGRZ_VdoSetSrcBuf(UINT32 u4Which, UINT32 u4YBaseAddr, UINT32 u4YBufLen, UINT32 u4CBufLen,
    UINT32 u4YHOffset, UINT32 u4YVOffset, 
    UINT32 u4CBaseAddr, UINT32 u4CHOffset, UINT32 u4CVOffset);

extern void _IMGRZ_VdoSetTgBuf(UINT32 u4Which, UINT32 u4YBaseAddr, UINT32 u4YBufLen,  UINT32 u4CBufLen, 
    UINT32 u4YHOffset, UINT32 u4YVOffset,
    UINT32 u4CBaseAddr);

extern void _IMGRZ_VdoScale(UINT32 u4Which, UINT32 u4YSrcW, UINT32 u4YSrcH, 
    UINT32 u4CSrcW, UINT32 u4CSrcH, 
    UINT32 u4YTgW, UINT32 u4YTgH);


extern void _IMGRZ_VdoSetSwap(UINT32 u4Which, UINT32 u4InSwap, UINT32 u4OutSwap);
extern void _IMGRZ_VdoSetVencSkip(UINT32 u4Which, UINT32 u4On_Off);	//for E_RZ_INOUT_TYPE_VDOENCODE_V2
extern void _IMGRZ_VdoSetCbCrSwap(UINT32 u4Which, UINT32 u4IsCbCrSwap);
extern void _IMGRZ_WebPSetMiscParam(UINT32 u4Which, UINT32 u4IsVdoPartialModeEnable, 
	UINT32 u4IsWebPRzOn,UINT32 u4LoopFilterEnable,UINT32 u4RzTmpBuf);

//===================
// JPG mode
//===================
extern void _IMGRZ_JpgSetTmpBuf(UINT32 u4Which, UINT32 u4BaseAddr);

extern void _IMGRZ_JpgSetParitailOutput(UINT32 u4Which, UINT32 u4IsPartailOutput);

extern void _IMGRZ_JpgSetMiscParam(UINT32 u4Which, UINT32 u4IsFstBl, UINT32 u4IsLstBl, 
    UINT32 u4JpgVfacY, UINT32 u4JpgVfacCb, UINT32 u4JpgVfacCr,
    UINT32 u4JpgRzOn, UINT32 u4ColorComp);

extern void _IMGRZ_JpgSetSrcBuf(UINT32 u4Which, UINT32 u4YBaseAddr1, UINT32 u4YBaseAddr2, 
    UINT32 u4YBufLen, 
    UINT32 u4CBufLen,
    UINT32 u4YHOffset, UINT32 u4YVOffset, 
    UINT32 u4CbBaseAddr1, UINT32 u4CbBaseAddr2, 
    UINT32 u4CbHOffset, UINT32 u4CbVOffset,
    UINT32 u4CrBaseAddr1, UINT32 u4CrBaseAddr2, 
    UINT32 u4CrHOffset, UINT32 u4CrVOffset);

extern void _IMGRZ_JpgScale(UINT32 u4Which, UINT32 u4YSrcW, UINT32 u4YSrcH, 
    UINT32 u4CbSrcW, UINT32 u4CbSrcH, 
    UINT32 u4CrSrcW, UINT32 u4CrSrcH, 
    UINT32 u4YTgW, UINT32 u4YTgH);
extern void _IMGRZ_JpgEnableBLAssign(UINT32 u4Which, UINT32 u4SrcYHeight,
  UINT32 u4SrcCbHeight,UINT32 u4SrcCrHeight);


//===================
// OSD mode
//===================
extern void _IMGRZ_OsdSetMiscParam(UINT32 u4Which, UINT32 u4IsSrcDirCM, 
    UINT32 u4CPTDataCount, UINT32 *pu4CPTData);

extern void _IMGRZ_OsdSetSrcBuf(UINT32 u4Which, UINT32 u4BaseAddr, 
    UINT32 u4IsDirCM, UINT32 u4CM, UINT32 u4BufLen, 
    UINT32 u4HOffset, UINT32 u4VOffset);

extern void _IMGRZ_OsdSetTgBuf(UINT32 u4Which, UINT32 u4BaseAddr, 
    UINT32 u4DirCM, UINT32 u4BufLen, 
    UINT32 u4HOffset, UINT32 u4VOffset);

extern void _IMGRZ_OsdScale(UINT32 u4Which, UINT32 u4SrcW, UINT32 u4SrcH, 
    UINT32 u4TgW, UINT32 u4TgH);

extern void _IMGRZ_OsdSetIdxTranslation(UINT32 u4Which, UINT32 u4CM);

extern void _IMGRZ_OsdPartialUpdate(UINT32 u4Which, RZ_OSD_SCL_PARAM_SET_T *prOsdParam);

extern void _IMGRZ_Osd_CSC(UINT32 u4Which, E_RZ_CSC_FORMAT_T src_format, E_RZ_CSC_FORMAT_T des_format);
extern BOOL _IMGRZ_WebpOn(UINT32 u4Which);
extern void _IMGRZ_OsdSetPreSrcBuf(UINT32 u4Which, UINT32 u4BaseAddr);

extern void _IMGRZ_SetOsdTurboMode(UINT32 fgTurboMode);

extern void _IMGRZ_SetOsdALU_CompositionMode(UINT32 fgAluMode, UINT32 Composition_mode);
//---------------------------------------------------------------------------
// Public functions (high-level APIs)
//---------------------------------------------------------------------------

extern void _IMGRZ_Break(UINT32 u4Which);

extern void _IMGRZ_Resume(UINT32 u4Which);

extern void _IMGRZ_OFF(UINT32 u4Which);

extern void _IMGRZ_MMU_int(UINT32 u4Which);

extern void _IMGRZ_SetProgressivePreloadMMU(UINT32 u4Which);

#endif // IMGRZ_DRVIF_H



