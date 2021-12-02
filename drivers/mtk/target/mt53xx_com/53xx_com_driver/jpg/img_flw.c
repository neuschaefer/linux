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
 * $RCSfile: img_flw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file img_flw.c
 *  Image decode, scale and display interface implementation.
 */

/******************************************************************************
* Include files
******************************************************************************/
#include "img_flw.h"
#include "img_lib_if.h"
#include "fbm_drvif.h"
#include "gfx_if.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "b2r_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "vdp_if.h"
LINT_EXT_HEADER_END
#include "jpg_if.h"
#include "jpg_common.h" //return value define
#include "jpg_debug.h"

/******************************************************************************
* Local	Macros,	defines, typedefs, enums
******************************************************************************/
//#define JPG_TIME_PROFILE

#define GFX_ROTATE_C_CLOCKWISE 1
#define GFX_ROTATE_CLOCKWISE 0

#define VIRTUAL_UINT8(X) ((UINT8 *)VIRTUAL((UINT32)(X)))
#define VIRTUAL_VOID(X) ((void *)VIRTUAL((UINT32)(X)))
#define ALIGN16(X) (((X) + 0xF) & (~0xF))	///< for gfx pitch alignment

#define BS_422FRAMEBUFFER_FOR_ARGB_FORMAT	0
#ifdef CC_53XX_JPG_ARGB4444
#define BS_422FRAMEBUFFER_SIZE	0x400000
#else
#define BS_422FRAMEBUFFER_SIZE	0x0
#endif
#define BS_OFFSET (BS_422FRAMEBUFFER_FOR_ARGB_FORMAT+BS_422FRAMEBUFFER_SIZE)
#define BS_SIZE 0x80000  					///< 0.5M for input ring buffer
#define DECODE_OUT_OFFSET (BS_OFFSET + BS_SIZE)
#define DECODE_OUT_SIZE 0x700000 		///< 7M for decode out buffer, 5382 64M
//#define DECODE_OUT_SIZE 0x280000 		///< 2.5M for decode out buffer, 5381 32M

#define RESERVE_LINES 32//18  			///< reserved lines for next operation, when dec lines != gfx segment lines
#define SEG_DIFF_LINES 32 				///< diff of jpg estimate segment lines and actual segment lines from gfx

#define SCALAR_RATIO_LMT 64
#define JPG_WIDTH_LMT 17000				///< 5382 64M
#define JPG_HEIGHT_LMT 10000
#define DISP_W_LMT 1920
#define DISP_H_LMT 1080
//#define JPG_WIDTH_LMT 6000				///< 5381 32M
#define RZ_MAX_TEMP_BUF_SIZE            (DISP_W_LMT * 4 * 3)            //adjust temp buf size of imgrz

/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _SetDispBuffer(IMG_LIB_DISP_INFO_T* prDisp, IMG_REGION_T rDst, ENUM_IMG_DISP_COLOR_MODE u4DispColorMode, ENUM_IMG_ROTATE_PHASE ePhase);
static INT32 _GetBSBufInfo(JPEGHANDLE hCurDec, IMG_LIB_FILL_RING_BUF_T *prStreamBufInfo);
static void _GetMemLayout(const UINT32* pu4RowSize, const UINT32* pu4RowWidth, IMG_LIB_INFO_T* prImg, UCHAR ucFbgId);

#if 0
static void _GfxScaleInit(JPEG_HEADER_INFO_T rHdrInfo, IMG_LIB_MEM_LAYOUT_T rMem, IMG_LIB_DISP_INFO_T* prDispInfo);
static void _GfxScaleColCov(UINT32 u4Loop, ENUM_IMG_ROTATE_PHASE eRotatePhase, IMG_LIB_DISP_INFO_T* prDispInfo, IMG_LIB_MEM_LAYOUT_T* prImgLibMem);
static void _MemCopyMirror(UINT8* pu1Dst, UINT8* pu1Src, UINT32 u4ByteLen, UINT32 u4BytePerPix);
#endif
static void _GfxExeBitBlt(IMG_REGION_T rSrc, IMG_REGION_T rDst, UINT32 u4Colormode, UINT32 u4SwitchOp);
static void _GfxRotatePhase90(IMG_REGION_T rSrc, IMG_REGION_T rDst, UINT32 u4Colormode, UINT32 u4IsCClockWise, UINT32 u4DstStep);
static void _CalKeepRatioRegion(BOOL bKeepRatio, UINT32 u4SrcW, UINT32 u4SrcH, UINT32 u4DstW, UINT32 u4DstH, UINT32* pu4ScaleW, UINT32* pu4ScaleH);
static void _FreeImgInstance(IMGHANDLE* phCurImg);

static VOID _IMG_RotateJpg(ENUM_IMG_ROTATE_PHASE eRotatePhase, IMG_LIB_DISP_INFO_T* prDispInfo, IMG_LIB_MEM_LAYOUT_T* prImgLibMem);
static VOID _IMG_ColorConVert(IMG_LIB_DISP_INFO_T* prDispInfo, IMG_LIB_MEM_LAYOUT_T* prImgLibMem, ENUM_IMG_DISP_COLOR_MODE u4DisplayClrMode);

#ifdef JPG_TIME_PROFILE
static void _JpgRowStart(void);
static void _JpgRowEnd(void);
static void _FillBufStart(void);
static void _FillBufEnd(void);
static void _GfxScStart(void);
static void _GfxScEnd(void);
static void _GfxCvStart(void);
static void _GfxCvEnd(void);
#endif

/******************************************************************************
* Local	variables
******************************************************************************/
static IMG_DEBUG_INFO_T _rImgDecDbg;
static UINT32 _au4BytePerPixel[COLOR_MODE_MAX] = {2, 2,0, 0, 2, 1};		///< bit depth (left shift value), index=ENUM_IMG_DISP_COLOR_MODE
static UINT32 _au4BPP[COLOR_MODE_MAX] = {4,4,2, 2, 3, 2};				///< bit depth, index=ENUM_IMG_DISP_COLOR_MODE
static UINT32 _u4CurVPId = 1;						///< current displayed video plane ID
//Fill buffer related
//static HANDLE_T _FillBuf_hSemaphore;
static PFN_JPG_EVENT_NOTIFY _pfnJpgCB;			///< jpg fill input buffer callback function

#ifdef JPG_TIME_PROFILE
static HAL_TIME_T _rHdrStart, _rJpgRowStart, _rFillBufStart, _rGfxScStart, _rGfxCvStart;
static HAL_TIME_T _rHdrEnd, _rJpgRowEnd, _rFillBufEnd, _rGfxScEnd, _rGfxCvEnd;
static HAL_TIME_T _rJpgHwAcc, _rFillBufAcc, _rGfxScAcc, _rGfxCvAcc;
static HAL_TIME_T _rPicStart, _rPicEnd, _rTotalDuration, _rHdrDuration;
#endif


#ifdef CC_SWGAMMA
static UINT32 _u4TempStartX, _u4TempStartY;
static UINT8 *_pu1TempStartAddr;
#endif

/******************************************************************************
* Global Function definition
******************************************************************************/
/**
 * Initialize image module.
 *
 * @param eMemClass in/out buffer type.
 * @return ENUM_IMG_RET
 */
IMGHANDLE _IMG_Init(ENUM_IMG_BUFFER_TYPE eMemClass, void *pvTag)
{
    static BOOL bInited = FALSE;
    INT32 i4Ret = 0;
    void* pvHandler;
    IMG_LIB_INFO_T* prImg;
    FBM_POOL_T* prFbmPool;


    //--- Image handler alloc
    pvHandler = x_mem_alloc(sizeof(IMG_LIB_INFO_T));
    if (pvHandler == NULL)
    {
        return (IMGHANDLE)NULL;
    }
    x_memset(pvHandler, 0, sizeof(IMG_LIB_INFO_T));


    //--- JPEG decode handler alloc
    prImg = (IMG_LIB_INFO_T *)pvHandler;
    prImg->hCurDec = JDEC_AllocInstance();
    prImg->pvTag = pvTag;
    prImg->u1MemClass = (UINT8)eMemClass;
    prImg->bStopDec = FALSE;
    prImg->bIsEOF=FALSE;
    prImg->bUseMw=FALSE;
    //--- JPEG hw init
    if (JDEC_New(prImg->hCurDec))					//Decoder structures malloc and build up
    {
        _FreeImgInstance((IMGHANDLE*)&pvHandler);
        return (IMGHANDLE)NULL;
    }

    //--- JPEG memory related function register
    if (eMemClass >= RING_IN_SINGLE_OUT)			//Input buffer provide by jpg driver
    {
#ifdef CC_JPEG_PIC_MODE
        JDEC_SetDecodeMode(E_JPG_DEC_MODE_BASELINE_PIC_RZ, eMemClass);
#endif
        if (eMemClass == EXTERNAL_RING_IN_DOUBLE_OUT)
        {
#ifndef CC_JPEG_PIC_MODE
            JDEC_SetDecodeMode(E_JPG_DEC_MODE_BASELINE_PIC_RZ, EXTERNAL_RING_IN_DOUBLE_OUT);
#endif
        }
        else
        {
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
            ASSERT(prFbmPool != NULL);
            ASSERT(prFbmPool->u4Addr != NULL);

#ifndef CC_JPEG_PIC_MODE
            JDEC_SetDecodeMode(E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ, eMemClass);
#endif
            JDEC_MwSetRingBuf(prImg->hCurDec, VIRTUAL_UINT8((UINT32)prFbmPool->u4Addr + BS_OFFSET),
                              (UINT32)BS_SIZE);
        }
        i4Ret = JDEC_MwDecodeSetup(prImg->hCurDec, E_JC_MEM_MW);
    }
    else										//Input buffer provide by application
    {
        JDEC_SetDecodeMode(E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ, eMemClass);
        i4Ret = JDEC_MwDecodeSetup(prImg->hCurDec, E_JC_MEM_YUV);
    }
    if (i4Ret)
    {
        _FreeImgInstance((IMGHANDLE*)&pvHandler);
        return (IMGHANDLE)NULL;
    }
#ifdef CC_SWGAMMA
    if (eMemClass == RING_IN_SINGLE_OUT)
    {
        JDEC_BuildTable();
    }
#endif

    //--- gfx init
    if ((!bInited) && (eMemClass != LINEAR_IN_LINEAR_OUT))
    {
        GFX_Init();
#if 0
        GFXSC_Init();
        GFXSC_JpgScaleInit();
#endif
        bInited = TRUE;
    }

    //---debug info reset
    x_memset(&_rImgDecDbg, 0 , sizeof(IMG_DEBUG_INFO_T));
    _IMG_GetBufInfo(&_rImgDecDbg.rBufInfo);
    _IMG_Get(&_rImgDecDbg.rCapInfo);

    return (IMGHANDLE)pvHandler;
}


VOID _IMG_Resize(IMG_REGION_T  *prSrc, IMG_REGION_T *prDst, UINT32 u4IsVDO, UINT32 u4CM)
{
    RZ_OSD_SCL_PARAM_SET_T rScaleParam;
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    if (!u4IsVDO)
    {
        x_memset(&rScaleParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

        rScaleParam.u4SrcBase = (UINT32)prSrc->pu1Addr;
        rScaleParam.u4IsSrcDirCM = 1;
        rScaleParam.u4SrcCM = u4CM;
        rScaleParam.u4SrcHOffset = prSrc->u4StartX;//0;
        rScaleParam.u4SrcVOffset = prSrc->u4StartY;//0;
        rScaleParam.u4SrcW = prSrc->u4Width;
        rScaleParam.u4SrcH = prSrc->u4Height;
        rScaleParam.u4TgBase = (UINT32)prDst->pu1Addr;
        rScaleParam.u4TgCM = rScaleParam.u4SrcCM;
        /*if (u4CM <= E_RZ_OSD_DIR_CM_ARGB4444)               //16bpp
        {
            rScaleParam.u4SrcBufLen = prSrc->u4Width << 1;
            rScaleParam.u4TgBufLen = prDst->u4Width << 1;
        }
        else                                                //24bpp
        {
            rScaleParam.u4SrcBufLen = prSrc->u4Width << 2;
            rScaleParam.u4TgBufLen = prDst->u4Width << 2;
        } */
        rScaleParam.u4SrcBufLen = prSrc->u4Pitch;
        rScaleParam.u4TgBufLen = prDst->u4Pitch;
        rScaleParam.u4TgHOffset = prDst->u4StartX;//0;
        rScaleParam.u4TgVOffset = prDst->u4StartY;//0;
        rScaleParam.u4TgW = prDst->u4Width;
        rScaleParam.u4TgH = prDst->u4Height;
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
        IMGRZ_Scale((void *)(&rScaleParam));
    }
    else
    {
        x_memset(&rScaleParamV,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));
        rScaleParamV.u4IsRsIn = 1;
        rScaleParamV.u4InMode = u4CM;
        rScaleParamV.u4YSrcBase = (UINT32)prSrc->pu1Addr;
        rScaleParamV.u4YSrcBufLen = prSrc->u4Pitch;//prSrc->u4Width;
        rScaleParamV.u4YSrcHOffset = prSrc->u4StartX;//0;
        rScaleParamV.u4YSrcVOffset = prSrc->u4StartY;//0;
        rScaleParamV.u4YSrcW = prSrc->u4Width;
        rScaleParamV.u4YSrcH = prSrc->u4Height;
        rScaleParamV.u4CSrcBase = (UINT32)prSrc->pu1Addr + prSrc->u4CbCrOffset;
        rScaleParamV.u4CSrcHOffset = prSrc->u4StartX >> 1;//0;
        rScaleParamV.u4CSrcVOffset = prSrc->u4StartY >> (1 - u4CM);//0;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = u4CM;
        rScaleParamV.u4IsVdo2Osd = 0;
        rScaleParamV.u4YTgBase = (UINT32)prDst->pu1Addr;
        rScaleParamV.u4YTgCM = u4CM;
        rScaleParamV.u4YTgBufLen = prDst->u4Pitch;//prDst->u4Width;
        rScaleParamV.u4YTgHOffset = prDst->u4StartX;//0;
        rScaleParamV.u4YTgVOffset = prDst->u4StartY;//0;
        rScaleParamV.u4YTgW = prDst->u4Width;
        rScaleParamV.u4YTgH = prDst->u4Height;
        rScaleParamV.u4CTgBase = (UINT32)prDst->pu1Addr + prDst->u4CbCrOffset;
        if (u4CM == E_RZ_VDO_OUTMD_420)
        {
            rScaleParamV.u4CSrcW = prSrc->u4Width >> 1;
            rScaleParamV.u4CSrcH = prSrc->u4Height >> 1;
            rScaleParamV.u4CTgW = prDst->u4Width >> 1;
            rScaleParamV.u4CTgH = prDst->u4Height >> 1;
        }
        else                        //422
        {
            rScaleParamV.u4CSrcW = prSrc->u4Width >> 1;
            rScaleParamV.u4CSrcH = prSrc->u4Height;
            rScaleParamV.u4CTgW = prDst->u4Width >> 1;
            rScaleParamV.u4CTgH = prDst->u4Height;
        }
        rScaleParamV.u4SrcSwap = 0;
        rScaleParamV.u4OutSwap = 0;

        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));
    }
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

}

/**
 * Set command.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
//Max temp buffer usage: 2M (1920x1080)
void _IMG_Set(IMG_BUF_PARAM_T rCmd)
{
    FBM_POOL_T *prFbmPool;
    IMG_REGION_T rSrc, rDst;
    UINT32 u4ScaleW, u4ScaleH, u4IsCClock;
    UINT32 u4ScOffsetX, u4ScOffsetY, u4DstScW, u4DstScH;
    UINT32 u4CbCrFactorH = 0;
    UINT32 u4SwitchOp = 0;

#ifdef JPG_TIME_PROFILE
    HAL_TIME_T rSetStart, rSetEnd, rSetTime;
    HAL_GetTime(&rSetStart);
#endif

    //#ifdef JPG_SEMI_HOST
#if 1
    //=== clean dst buffer in verification environment
    //x_memset(VIRTUAL_VOID(rCmd.rDstRegion.pu1Addr), 0, rCmd.rDstRegion.u4Pitch * rCmd.rDstRegion.u4TotalLine);
    if (rCmd.eDispColorMode == Y_CBCR422_SEP16)
    {
        //x_memset(VIRTUAL_VOID((UINT32)rCmd.rDstRegion.pu1Addr + rCmd.rDstRegion.u4CbCrOffset), 0x80, rCmd.rDstRegion.u4Pitch * rCmd.rDstRegion.u4TotalLine);
        u4CbCrFactorH = 0;
    }
    else if (rCmd.eDispColorMode == Y_CBCR420_SEP16)
    {
        //x_memset(VIRTUAL_VOID((UINT32)rCmd.rDstRegion.pu1Addr + rCmd.rDstRegion.u4CbCrOffset), 0x80, (rCmd.rDstRegion.u4Pitch * rCmd.rDstRegion.u4TotalLine)>>1);
        u4CbCrFactorH = 1;
    }
#else
    if (rCmd.eDispColorMode == Y_CBCR422_SEP16)
    {
        u4CbCrFactorH = 0;
    }
    else if (rCmd.eDispColorMode == Y_CBCR420_SEP16)
    {
        u4CbCrFactorH = 1;
    }
#endif

    rCmd.rSrcRegion.pu1Addr = (UINT8*)PHYSICAL((UINT32) rCmd.rSrcRegion.pu1Addr);
    rCmd.rDstRegion.pu1Addr = (UINT8*)PHYSICAL((UINT32) rCmd.rDstRegion.pu1Addr);

    //=== calculate src region to dst region scaling factor, dst w:h = h:w
    if ((rCmd.eRotateDegree == CLOCKWISE_90) || (rCmd.eRotateDegree == CLOCKWISE_270) ||
            (rCmd.eRotateDegree == CLOCKWISE_90_WITH_FLIP) || (rCmd.eRotateDegree == CLOCKWISE_270_WITH_FLIP))
    {
        u4DstScH = rCmd.rDstRegion.u4Width;
        u4DstScW = rCmd.rDstRegion.u4Height;
        u4ScOffsetX = rCmd.rDstRegion.u4StartY;
        u4ScOffsetY = rCmd.rDstRegion.u4StartX;
    }
    else	//dst w:h = w:h
    {
        u4DstScW = rCmd.rDstRegion.u4Width;
        u4DstScH = rCmd.rDstRegion.u4Height;
        u4ScOffsetX = rCmd.rDstRegion.u4StartX;
        u4ScOffsetY = rCmd.rDstRegion.u4StartY;
    }

    _CalKeepRatioRegion(rCmd.bKeepRatio, rCmd.rSrcRegion.u4Width, rCmd.rSrcRegion.u4Height,
                        u4DstScW, u4DstScH, &u4ScaleW, &u4ScaleH);

//	u4ScOffsetX = ((u4DstScW - u4ScaleW) >> 1) & (~(0x1));
//	u4ScOffsetY = ((u4DstScH - u4ScaleH) >> 1) & (~(0x1));

    //=== zoom and rotate
    if ((rCmd.eRotateDegree == CLOCKWISE_0) || (rCmd.eRotateDegree == CLOCKWISE_0_WITH_FLIP))//pure zoom function
    {
        x_memcpy((void *)&rSrc, (void *)&rCmd.rSrcRegion, sizeof(IMG_REGION_T));
        //rSrc.pu1Addr = rCmd.rSrcRegion.pu1Addr;
        //rSrc.u4Pitch = ALIGN16(u4ScaleW);
        //rSrc.u4TotalLine = u4ScaleH;
        //rSrc.u4StartX = 0;
        //rSrc.u4StartY = 0;
        //rSrc.u4Width = u4ScaleW;
        //rSrc.u4Height = u4ScaleH;
        x_memcpy((void *)&rDst, (void *)&rCmd.rDstRegion, sizeof(IMG_REGION_T));
        rDst.u4StartX = u4ScOffsetX;//0;
        rDst.u4StartY = u4ScOffsetY;//0;
        rDst.u4Width = u4ScaleW;
        rDst.u4Height = u4ScaleH;
        if ((rCmd.eDispColorMode == Y_CBCR422_SEP16) || (rCmd.eDispColorMode == Y_CBCR420_SEP16))
        {
            if (rCmd.eRotateDegree == CLOCKWISE_0_WITH_FLIP)
            {
                prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
                ASSERT(prFbmPool != NULL);
                ASSERT(prFbmPool->u4Addr != NULL);		//tmp buffer while not decoding
                ASSERT(prFbmPool->u4Size >= (rCmd.rSrcRegion.u4Width * rCmd.rSrcRegion.u4Height));
                rDst.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
            }
            _IMG_Resize(&rSrc, &rDst, 1, E_RZ_VDO_OUTMD_422);
            if (rCmd.eRotateDegree == CLOCKWISE_0_WITH_FLIP)
            {
                x_memcpy((void *)&rSrc, (void *)&rDst, sizeof(IMG_REGION_T));
                rDst.pu1Addr = rCmd.rDstRegion.pu1Addr;
                u4SwitchOp |= D_GFXFLAG_MIRROR;
                _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4SwitchOp);
                //--- CbCr flip
                rSrc.pu1Addr += rCmd.rDstRegion.u4CbCrOffset;
                //rSrc.u4Pitch = ALIGN16(u4ScaleW);
                rSrc.u4StartX >>= 1;
                rSrc.u4StartY >>= u4CbCrFactorH;
                rSrc.u4Width = u4ScaleW >> 1;
                rSrc.u4Height = u4ScaleH >> u4CbCrFactorH;

                rDst.pu1Addr += rCmd.rDstRegion.u4CbCrOffset;
                rDst.u4StartX >>= 1;
                rDst.u4StartY >>= u4CbCrFactorH;
                rDst.u4Width = u4ScaleW >> 1;
                rDst.u4Height = u4ScaleH >> u4CbCrFactorH;
                _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_ARGB4444_DIRECT16, u4SwitchOp);
            }
        }
        else    //osd degree 0
        {
            //rSrc.u4Pitch = ALIGN16(u4ScaleW * rCmd.rSrcRegion.u4Depth);
            //rDst.u4Pitch = ALIGN16(u4ScaleW * rCmd.rDstRegion.u4Depth);

            if (rCmd.eRotateDegree == CLOCKWISE_0_WITH_FLIP)
            {
                u4SwitchOp |= D_GFXFLAG_MIRROR;
                _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_AYCbCr8888_DIRECT32, u4SwitchOp);
            }
            _IMG_Resize(&rSrc, &rDst, 0, E_RZ_OSD_DIR_CM_AYCbCr8888);
        }

    }
    else											//zoom and rotate
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != NULL);		//tmp buffer while not decoding
        ASSERT(prFbmPool->u4Size >= (rCmd.rSrcRegion.u4Width * rCmd.rSrcRegion.u4Height));

        if ((rCmd.eDispColorMode == Y_CBCR422_SEP16) || (rCmd.eDispColorMode == Y_CBCR420_SEP16))
        {
            //rotate 180
            if ((rCmd.eRotateDegree == CLOCKWISE_180) || (rCmd.eRotateDegree == CLOCKWISE_180_WITH_FLIP))
            {
                x_memcpy((void *)&rSrc, (void *)&rCmd.rSrcRegion, sizeof(IMG_REGION_T));
                //rSrc.pu1Addr = rCmd.rSrcRegion.pu1Addr;
                //rSrc.u4Pitch = ALIGN16(u4ScaleW);
                //rSrc.u4TotalLine = u4ScaleH;
                //rSrc.u4StartX = 0;
                //rSrc.u4StartY = 0;
                //rSrc.u4Width = u4ScaleW;
                //rSrc.u4Height = u4ScaleH;
                x_memcpy((void *)&rDst, (void *)&rCmd.rDstRegion, sizeof(IMG_REGION_T));
                rDst.u4StartX = u4ScOffsetX;//0;
                rDst.u4StartY = u4ScOffsetY;//0;
                rDst.u4Width = u4ScaleW;
                rDst.u4Height = u4ScaleH;
                rDst.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
                _IMG_Resize(&rSrc, &rDst, 1, E_RZ_VDO_OUTMD_422);
                x_memcpy((void *)&rSrc, (void *)&rDst, sizeof(IMG_REGION_T));
                rDst.pu1Addr = rCmd.rDstRegion.pu1Addr;

                //code sync
                if (rDst.u4Width <=16)
                {
                    rDst.u4Width = 18;
                }

                if (rCmd.eRotateDegree == CLOCKWISE_180_WITH_FLIP)
                {
                    u4SwitchOp |= D_GFXFLAG_FLIP;
                }
                else
                {
                    u4SwitchOp |= D_GFXFLAG_MIRROR | D_GFXFLAG_FLIP;
                }

                _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4SwitchOp);

                //--- CbCr rotate 180
                rSrc.pu1Addr += rCmd.rDstRegion.u4CbCrOffset;
                //rSrc.u4Pitch = ALIGN16(u4ScaleW);
                rSrc.u4StartX >>= 1;
                rSrc.u4StartY >>= u4CbCrFactorH;
                rSrc.u4Width = u4ScaleW >> 1;
                rSrc.u4Height = u4ScaleH >> u4CbCrFactorH;

                rDst.pu1Addr += rCmd.rDstRegion.u4CbCrOffset;
                rDst.u4StartX >>= 1;
                rDst.u4StartY >>= u4CbCrFactorH;
                rDst.u4Width = u4ScaleW >> 1;
                rDst.u4Height = u4ScaleH >> u4CbCrFactorH;

                //code sync
                if (rDst.u4Width <=8)
                {
                    rDst.u4Width = 9;
                }

                if (rCmd.eRotateDegree == CLOCKWISE_180_WITH_FLIP)
                {
                    u4SwitchOp |= D_GFXFLAG_FLIP;
                }
                else
                {
                    u4SwitchOp |= D_GFXFLAG_MIRROR | D_GFXFLAG_FLIP;
                }

                _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_ARGB4444_DIRECT16, u4SwitchOp);
            }
            else										//zoom, rotate 90 2 step
            {
                //---Y rotate, src=tmp buffer, dst=rCmd.dst
                x_memcpy((void *)&rSrc, (void *)&rCmd.rSrcRegion, sizeof(IMG_REGION_T));
                /*rSrc.pu1Addr = rCmd.rSrcRegion.pu1Addr;
                rSrc.u4StartX = 0;
                rSrc.u4StartY = 0;
                rSrc.u4Width = rCmd.rSrcRegion.u4Width;
                rSrc.u4Height = rCmd.rSrcRegion.u4Height;
                rSrc.u4CbCrOffset = rCmd.rSrcRegion.u4CbCrOffset;*/
                x_memcpy((void *)&rDst, (void *)&rCmd.rDstRegion, sizeof(IMG_REGION_T));
                rDst.u4StartX = 0;
                rDst.u4StartY = 0;
                rDst.u4Width = u4ScaleW;//ALIGN16(u4ScaleW);
                rDst.u4Height = u4ScaleH;
                rDst.u4Pitch = ALIGN16(u4ScaleW);
                rDst.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
                _IMG_Resize(&rSrc, &rDst, 1, E_RZ_VDO_OUTMD_422);
                /*rSrc.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
                rSrc.u4Pitch = ALIGN16(u4ScaleW);
                rSrc.u4TotalLine = u4ScaleH;
                rSrc.u4StartX = 0;
                rSrc.u4StartY = 0;
                rSrc.u4Width = rDst.u4Width;
                rSrc.u4Height = rDst.u4Height;*/
                x_memcpy((void *)&rSrc, (void *)&rDst, sizeof(IMG_REGION_T));
                if ((rCmd.eRotateDegree == CLOCKWISE_90_WITH_FLIP) || (rCmd.eRotateDegree == CLOCKWISE_270_WITH_FLIP))
                {
                    rDst.pu1Addr = rCmd.rSrcRegion.pu1Addr;
                    u4SwitchOp |= D_GFXFLAG_MIRROR;
                    _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4SwitchOp);
                    rSrc.pu1Addr = rDst.pu1Addr;
                }

                rDst.u4StartX = u4ScOffsetY;
                rDst.u4StartY = u4ScOffsetX;
                rDst.u4Width = rSrc.u4Height;
                rDst.u4Height = rSrc.u4Width;
                rDst.u4Pitch = ALIGN16(rCmd.rDstRegion.u4Pitch);
                rDst.pu1Addr = rCmd.rDstRegion.pu1Addr;

                if ((rCmd.eRotateDegree == CLOCKWISE_90)||(rCmd.eRotateDegree == CLOCKWISE_90_WITH_FLIP))
                {
                    u4IsCClock = 0;
                }
                else
                {
                    //rDst.u4StartY += rDst.u4Height;
                    //code sync
                    rDst.u4StartY += rDst.u4Height -1;
                    u4IsCClock = 1;
                }
                rDst.pu1Addr = rCmd.rDstRegion.pu1Addr;
                _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4IsCClock, 1);

                //---CbCr rotate
                rSrc.u4Width >>= 1;
                rSrc.u4Height >>= u4CbCrFactorH;
                rSrc.pu1Addr = (UINT8 *)(prFbmPool->u4Addr + rCmd.rDstRegion.u4CbCrOffset);
                rDst.u4StartX = 0;
                rDst.u4StartY = 0;
                rDst.u4Width = rSrc.u4Width << 1;
                rDst.u4Height = rSrc.u4Height >> 1;
                rDst.u4Pitch = ALIGN16((rDst.u4Width >> u4CbCrFactorH) << 1);
                rDst.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
                _IMG_Resize(&rSrc, &rDst, 0, E_RZ_OSD_DIR_CM_ARGB4444);
                x_memcpy((void *)&rSrc, (void *)&rDst, sizeof(IMG_REGION_T));
                /*rSrc.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
                rSrc.u4Pitch = ALIGN16((rDst.u4Width >> u4CbCrFactorH) << 1);
                rSrc.u4Width = rDst.u4Width;
                rSrc.u4Height = rDst.u4Height;				*/
                if ((rCmd.eRotateDegree == CLOCKWISE_90_WITH_FLIP) || (rCmd.eRotateDegree == CLOCKWISE_270_WITH_FLIP))
                {
                    rDst.pu1Addr = rCmd.rSrcRegion.pu1Addr;
                    u4SwitchOp |= D_GFXFLAG_MIRROR;
                    _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_ARGB4444_DIRECT16, u4SwitchOp);
                    rSrc.pu1Addr = rDst.pu1Addr;
                }
                rDst.u4StartX = u4ScOffsetY >> 1;
                rDst.u4StartY = u4ScOffsetX >> u4CbCrFactorH;
                rDst.u4Width = u4ScaleH>>1;
                rDst.u4Height = u4ScaleW >> u4CbCrFactorH;
                rDst.u4Pitch = rCmd.rDstRegion.u4Pitch;
                if ((rCmd.eRotateDegree == CLOCKWISE_270)||(rCmd.eRotateDegree == CLOCKWISE_270_WITH_FLIP))
                {
                    rDst.u4StartY += (rDst.u4Height >> u4CbCrFactorH);
                }
                rDst.pu1Addr = rCmd.rDstRegion.pu1Addr + rCmd.rDstRegion.u4CbCrOffset;
                _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_ARGB4444_DIRECT16, u4IsCClock, 1);
            }
        }
        else                                    //OSD
        {
            if ((rCmd.eRotateDegree == CLOCKWISE_180) || (rCmd.eRotateDegree == CLOCKWISE_180_WITH_FLIP))
            {	                                            //degree 180
                x_memcpy((void *)&rSrc, (void *)&rCmd.rSrcRegion, sizeof(IMG_REGION_T));
                //rSrc.pu1Addr = rCmd.rSrcRegion.pu1Addr;
                //rSrc.u4Pitch = ALIGN16(u4ScaleW * rCmd.rSrcRegion.u4Depth);
                //rSrc.u4TotalLine = u4ScaleH;
                //rSrc.u4StartX = 0;
                //rSrc.u4StartY = 0;
                //rSrc.u4Width = u4ScaleW;
                //rSrc.u4Height = u4ScaleH;
                x_memcpy((void *)&rDst, (void *)&rCmd.rDstRegion, sizeof(IMG_REGION_T));
                rDst.u4StartX = u4ScOffsetX;
                rDst.u4StartY = u4ScOffsetY;
                rDst.u4Width = u4ScaleW;
                rDst.u4Height = u4ScaleH;
                rDst.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
                _IMG_Resize(&rSrc, &rDst, 0, E_RZ_OSD_DIR_CM_AYCbCr8888);
                x_memcpy((void *)&rSrc, (void *)&rDst, sizeof(IMG_REGION_T));
                rDst.pu1Addr = rCmd.rDstRegion.pu1Addr;
                if (rCmd.eRotateDegree == CLOCKWISE_180_WITH_FLIP)
                {
                    u4SwitchOp |= D_GFXFLAG_FLIP;
                }
                else
                {
                    u4SwitchOp |= D_GFXFLAG_MIRROR | D_GFXFLAG_FLIP;
                }
                _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_AYCbCr8888_DIRECT32, u4SwitchOp);
            }
            else                                            //degree 90 or 270
            {
                //---Y rotate, src=tmp buffer, dst=rCmd.dst
                x_memcpy((void *)&rSrc, (void *)&rCmd.rSrcRegion, sizeof(IMG_REGION_T));
                rDst.u4StartX = 0;
                rDst.u4StartY = 0;
                rDst.u4Width = u4ScaleW;//ALIGN16(u4ScaleW);
                rDst.u4Height = u4ScaleH;
                rDst.u4Pitch = ALIGN16(u4ScaleW * rCmd.rSrcRegion.u4Depth);
                rDst.pu1Addr = (UINT8 *)prFbmPool->u4Addr;
                _IMG_Resize(&rSrc, &rDst, 0, E_RZ_OSD_DIR_CM_AYCbCr8888);

                rSrc.pu1Addr = rDst.pu1Addr;
                rSrc.u4Pitch = ALIGN16(u4ScaleW * rCmd.rSrcRegion.u4Depth);
                rSrc.u4StartX = 0;
                rSrc.u4StartY = 0;
                rSrc.u4Width = u4ScaleW;
                rSrc.u4Height = u4ScaleH;

                x_memcpy((void *)&rDst, (void *)&rCmd.rDstRegion, sizeof(IMG_REGION_T));
                rDst.u4StartX = u4ScOffsetY;
                rDst.u4StartY = u4ScOffsetX;
                rDst.u4Width = u4ScaleH;
                rDst.u4Height = u4ScaleW;

                if ((rCmd.eRotateDegree == CLOCKWISE_90)||(rCmd.eRotateDegree == CLOCKWISE_90_WITH_FLIP))
                {
                    u4IsCClock = 0;
                }
                else
                {
                    rDst.u4StartY += rDst.u4Height;
                    u4IsCClock = 1;
                }
                if ((rCmd.eRotateDegree == CLOCKWISE_90_WITH_FLIP) || (rCmd.eRotateDegree == CLOCKWISE_270_WITH_FLIP))
                {
                    rDst.pu1Addr = rCmd.rSrcRegion.pu1Addr;
                    u4SwitchOp |= D_GFXFLAG_MIRROR;
                    _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_AYCbCr8888_DIRECT32, u4SwitchOp);
                    rSrc.pu1Addr = rDst.pu1Addr;
                }
                rDst.pu1Addr = rCmd.rDstRegion.pu1Addr;
                _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_AYCbCr8888_DIRECT32, u4IsCClock, 1);
            }
        }
    }

#ifdef JPG_TIME_PROFILE
    HAL_GetTime(&rSetEnd);
    HAL_GetDeltaTime(&rSetTime, &rSetStart, &rSetEnd);
    LOG(3, "Set operation %u.%06u\n", rSetTime.u4Seconds, rSetTime.u4Micros);
#endif

}

/**
 * Query decoding information.
 *
 * @param rStatus status and information of decoding process.
 * @return void
 */
void _IMG_Get(IMG_CAP_T* prCap)
{
    FBM_POOL_T *prFbmPool;
    FBM_POOL_T *prFbmPool_rsv3;
    //FBM_POOL_T *prFbmPool_rsv2;
    FBM_POOL_T *prFbmPool_nr;

    ASSERT(prCap != NULL);
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
    ASSERT(prFbmPool != NULL);

    prCap->u4MaxImgW = (UINT32)JPG_WIDTH_LMT;
    prCap->u4MaxImgH = (UINT32)JPG_HEIGHT_LMT;
    prCap->u4MaxScaleFactor = (UINT32)SCALAR_RATIO_LMT;
    prCap->u4MaxScaleBufSize = prFbmPool->u4Size - (UINT32)JPG_TOTAL_BUFFER_SIZE;

    prCap->u4ScaleBufAddr = prFbmPool->u4Addr + (UINT32)JPG_TOTAL_BUFFER_SIZE;
    prCap->u4WorkingAddr = prFbmPool->u4Addr;
    prFbmPool_rsv3 = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MDDI_MAIN);
    ASSERT(prFbmPool_rsv3 != NULL);
    prFbmPool_nr = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_NR_1);
    ASSERT(prFbmPool_nr != NULL);
    prCap->u4RsvAddr = prFbmPool_rsv3->u4Addr;
    prCap->u4RsvTotalSz = prFbmPool_rsv3->u4Size + prFbmPool_nr->u4Size;//prFbmPool_rsv3->u4Size;
    prCap->u4RsvSizeL = prCap->u4RsvTotalSz;//prFbmPool_rsv3->u4Size - prCap->u4MaxScaleBufSize;
    prCap->u4RsvSizeS = prFbmPool_rsv3->u4Size;
    //prFbmPool_rsv2 = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MDDI_MAIN);
    //ASSERT(prFbmPool_rsv2 != NULL);
    //prCap->u4RsvSizeS = prFbmPool_rsv2->u4Size;
}


/**
 * Set ImgSize
 *
 * @File Size.
 * @return void
 */

void _IMG_SetImgSize(IMGHANDLE hCurImg, UINT32 u4ImageSize)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;
    ASSERT(prImg!=NULL);
    JDEC_SetFileSize(prImg->hCurDec,u4ImageSize);
    return;
}

/**
 * Query decoding progress.
 *
 * @param decoding process.
 * @return void
 */
void _IMG_GetProgess(IMGHANDLE hCurImg, UINT32* pu4Status)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;

    ASSERT(prImg!=NULL);
    ASSERT(pu4Status !=NULL);

    *pu4Status = JDEC_GetProgress(prImg->hCurDec);
}


/**
 * Stop decoding procedure.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_Stop(IMGHANDLE hCurImg)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;

    //--- error checking-----------------------------------------------
    if (prImg == NULL)
    {
        return E_IMG_NULL_HANDLER;
    }

    prImg->bStopDec = TRUE;
    JDEC_SetStop(TRUE);
    return E_IMG_OK;
}


/**
 * Release handler.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
void _IMG_Release(IMGHANDLE* phCurImg)
{
    _FreeImgInstance(phCurImg);
}

/**
 * Connect video plane.
 *
 * @param u4VideoId video plane ID.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_VDP_Connect(UINT32 u4VideoId)
{
    //--- Display /video plane init
    if (VDP_SetMode((UCHAR)u4VideoId, VDP_MODE_BG)==0)
    {
        return E_IMG_DISP_VP_ERROR;
    }
    if (VDP_SetEnable((UCHAR)u4VideoId, 1)==0)
    {
        return E_IMG_DISP_VP_ERROR;
    }
    if (VDP_Image_Connect(u4VideoId)==0)
    {
        return E_IMG_DISP_VP_ERROR;
    }
    if (VDP_SetMode((UCHAR)u4VideoId, VDP_MODE_NORMAL)==0)
    {
        return E_IMG_DISP_VP_ERROR;
    }

    return E_IMG_OK;

}

/**
 * Get video plane information.
 *
 * @param rVpInfo video plane detailed information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_GetVideoPlaneInfo(IMG_VP_INFO_T *prVpInfo)
{
    VDP_IMAGE_INFO_T *prVDPInfo;
    UINT32 u4YAddr, u4CAddr;

    prVDPInfo = VDP_Image_GetInfo();
    if ((prVDPInfo == NULL) || (prVpInfo == NULL))
    {
        return E_IMG_DISP_VP_ERROR;
    }
    if (VDP_Image_GetFrameBuffer(_u4CurVPId, &u4YAddr, &u4CAddr)==0)
    {
        return E_IMG_DISP_VP_ERROR;
    }
    prVpInfo->pu1CurFrameAddr[0] = (UINT8 *)u4YAddr;
    prVpInfo->pu1CurFrameAddr[1] = (UINT8 *)u4CAddr;
    if (VDP_Image_GetFrameBuffer(1 - _u4CurVPId, &u4YAddr, &u4CAddr)==0)
    {
        return E_IMG_DISP_VP_ERROR;
    }
    prVpInfo->pu1BackFrameAddr[0] = (UINT8 *)u4YAddr;
    prVpInfo->pu1BackFrameAddr[1] = (UINT8 *)u4CAddr;

    //u4FrameBufferNs;     ///< supported number for Frame Buffer
    prVpInfo->u4Width = prVDPInfo->u4Width;
    prVpInfo->u4Height = prVDPInfo->u4Height;
    prVpInfo->u4PitchAlign = prVDPInfo->u4PitchAlignement;

    if (prVDPInfo->u4ColorMode & VDP_IMAGE_422_MODE)
    {
        prVpInfo->eDispColorMode = Y_CBCR422_SEP16;
    }
    else if (prVDPInfo->u4ColorMode & VDP_IMAGE_420_MODE)
    {
        prVpInfo->eDispColorMode = Y_CBCR420_SEP16;
    }
    return E_IMG_OK;

}

/**
 * Flip video plane buffer.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_Flip(void)
{
    VDP_IMAGE_ARG_T rInfo;
    VDP_IMAGE_INFO_T *prVDPInfo;

#ifdef JPG_TIME_PROFILE
    HAL_TIME_T rVideoFlipStart, rVideoFlipEnd, rVideoFlipTime;
    HAL_GetTime(&rVideoFlipStart);
#endif

    prVDPInfo = VDP_Image_GetInfo();
    if (prVDPInfo->u4ColorMode & VDP_IMAGE_422_MODE)
    {
        rInfo.u4ColorMode = (UINT32)VDP_IMAGE_422_MODE;
    }
    else
    {
        rInfo.u4ColorMode = (UINT32)VDP_IMAGE_420_MODE;
    }

    rInfo.u4FrameBufferId = 1-_u4CurVPId;
    rInfo.u4Width = prVDPInfo->u4Width;
    rInfo.u4Height = prVDPInfo->u4Height;
    rInfo.u4Pitch = prVDPInfo->u4Width;

    if (VDP_Image_Show(&rInfo)==0)
    {
        return E_IMG_DISP_VP_ERROR;
    }
    if (VDP_Image_Mute(0) == 0)
    {
        return E_IMG_DISP_VP_ERROR;
    }
    _u4CurVPId = 1-_u4CurVPId;

    //debug message
    UNUSED(_IMG_GetVideoPlaneInfo(&_rImgDecDbg.rVPInfo));

#ifdef JPG_TIME_PROFILE
    HAL_GetTime(&rVideoFlipEnd);
    HAL_GetDeltaTime(&rVideoFlipTime, &rVideoFlipStart, &rVideoFlipEnd);
    LOG(3, "Video flip %u.%06u\n", rVideoFlipTime.u4Seconds, rVideoFlipTime.u4Micros);
#endif

    return E_IMG_OK;

}

/**
 * Register input image buffer filling callback function.
 *
 * @param pfnReg callback function.
 * @return void
 */
void _IMG_CBreg(PFN_JPG_EVENT_NOTIFY pfnReg)
{
    _pfnJpgCB = pfnReg;
}

/**
 * Set status while buffer filling operation finish.
 *
 * @param u4Entry1Size buffer entry1 filled size.
 * @param u4Entry2Size buffer entry2 filled size.
 * @param u4IsEOF flag if end of image file.
 * @return void
 */
void _IMG_SetFillBsBufEnd(IMGHANDLE hCurImg, UINT32 u4Entry1Size, UINT32 u4Entry2Size, BOOL bIsEOF)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;

    //--- error checking-----------------------------------------------
    if (prImg == NULL)
    {
        return;
    }

    JDEC_FillBsBuf_End(prImg->hCurDec, u4Entry1Size, u4Entry2Size, bIsEOF);
    prImg->bIsEOF = bIsEOF;
    // release semaphore
    //VERIFY (x_sema_unlock(_FillBuf_hSemaphore) == OSR_OK);
}


/**
 * Get linear output buffer information.(for verification only)
 *
 * @param ppvmem data structure JMEM_YUV_LINEAR_T.
 * @return ENUIM_JPEG_RET_CODE_T
 */
INT32 _IMG_GetOutputBuffer(void *hCurImg, void **ppvmem)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;

    //--- error checking-----------------------------------------------
    if (prImg == NULL)
    {
        return (INT32)E_IMG_NULL_HANDLER;
    }

    return JDEC_MwGetMemLayout(prImg->hCurDec, ppvmem);
}


INT32 _IMG_CheckDecParam(IMGHANDLE hCurImg, IMG_PARAM_T rInfo)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;

#ifdef JPG_TIME_PROFILE
    _rJpgHwAcc.u4Seconds = 0;
    _rJpgHwAcc.u4Micros  = 0;
    _rFillBufAcc.u4Seconds = 0;
    _rFillBufAcc.u4Micros  = 0;
    _rGfxScAcc.u4Seconds = 0;
    _rGfxScAcc.u4Micros = 0;
    _rGfxCvAcc.u4Seconds = 0;
    _rGfxCvAcc.u4Micros = 0;
    HAL_GetTime(&_rPicStart);
#endif

    //--- error checking-----------------------------------------------
    if (prImg == NULL)
    {
        return (INT32)E_IMG_NULL_HANDLER;
    }

#ifdef CC_53XX_JPG_ARGB4444
    if (rInfo.eDispColorMode >= COLOR_MODE_MAX)
    {
        return (INT32)E_IMG_DISP_INFO_ERROR;
    }
#else
    if (rInfo.eDispColorMode >= ARGB4444_DIRECT16)
    {
        return (INT32)E_IMG_DISP_INFO_ERROR;
    }
#endif

    return (INT32)E_IMG_OK;

}


INT32 _IMG_DecPrepare(IMGHANDLE hCurImg, IMG_PARAM_T rInfo)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;
    JPEG_HEADER_INFO_T* prJpgHdrInfo;
    IMG_LIB_DISP_INFO_T* prDispInfo;
    IMG_REGION_T* prDecOut;					    //feedback information
    UINT32 u4OrigRowW, u4OrigRowSize, u4ScaleRowH, u4ScaleRowW, u4MemRowSize, u4ReserveSize;
    INT32 i4Ret;

    ASSERT(prImg != NULL);
    prJpgHdrInfo = (JPEG_HEADER_INFO_T*)&prImg->rHdrInfo;
    prDispInfo = (IMG_LIB_DISP_INFO_T*)&prImg->rDispInfo;

#ifdef CC_SWGAMMA
    _u4TempStartX = rInfo.rDstRegion.u4StartX;
    _u4TempStartY = rInfo.rDstRegion.u4StartY;
    _pu1TempStartAddr = VIRTUAL_UINT8(rInfo.rDstRegion.pu1Addr);
#endif

#ifdef CC_JPEG_PIC_MODE
    //if (rInfo.eRotateDegree != CLOCKWISE_0 || rInfo.eDispColorMode==ARGB8888_DIRECT32 ||rInfo.eDispColorMode==ARGB4444_DIRECT16)
    if (rInfo.eRotateDegree != CLOCKWISE_0)
    {
        JDEC_SetDecodeMode(E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ,
                           (ENUM_IMG_BUFFER_TYPE)prImg->u1MemClass);
    }
#endif

    //--- set display parameter------------------------------------------
    if (_SetDispBuffer(prDispInfo, rInfo.rDstRegion, rInfo.eDispColorMode, rInfo.eRotateDegree) < 0)
    {
        return (INT32)E_IMG_DISP_INFO_ERROR;
    }

    //--- Header parsing, init setup--------------------------------------
#ifdef JPG_TIME_PROFILE
    HAL_GetTime(&_rHdrStart);
#endif

    if (prImg->u1MemClass == (UINT8)EXTERNAL_RING_IN_DOUBLE_OUT)
    {
        JDEC_MwSetRingBuf(prImg->hCurDec, VIRTUAL_UINT8((UINT32)rInfo.pu1InBuffer),
                          (UINT32)rInfo.u8BufSize);
        i4Ret = JDEC_MwHeaderParse(prImg->hCurDec, VIRTUAL_UINT8(rInfo.pu1InBuffer + rInfo.rMjpg.u4RingOffset),
                                   (UINT32)rInfo.u8BufSize, prJpgHdrInfo);
    }
    else if (prImg->u1MemClass >= (UINT8)RING_IN_SINGLE_OUT)
    {
        i4Ret = JDEC_MwHeaderParse(prImg->hCurDec, NULL, 0, prJpgHdrInfo);
    }
    else
    {
        i4Ret = JDEC_MwHeaderParse(prImg->hCurDec, VIRTUAL_UINT8(rInfo.pu1InBuffer), (UINT32)rInfo.u8BufSize, prJpgHdrInfo);
    }
    JDEC_SetStop(FALSE);

    //--- check header error case and decoder limitation
    if (i4Ret == -(INT32)E_JPG_ERR_UNSUPPORT)
    {
        LOG(3, "File Format Unsupport!\n");
        return (INT32)E_IMG_FMT_UNSUPPORT;
    }
    else if (i4Ret == (INT32)E_JPG_ERR_INVALID_COMPONENT)
    {
        //code sync
        return (INT32)E_IMG_ERR_COMPONENT;
    }
    else if (i4Ret)
    {
        return (INT32)E_IMG_DEC_FAIL;
    }
    if (prJpgHdrInfo->u4Width > JPG_WIDTH_LMT)
    {
        LOG(3, "Image width %d out of limitation %d\n", prJpgHdrInfo->u4Width, (UINT32)JPG_WIDTH_LMT);
        return (INT32)E_IMG_DEC_FAIL;
    }

    //---- fit original image resolution with limitation DISP_W_LMT * DISP_H_LMT
    if ((prDispInfo->u4DisplayW==0) || (prDispInfo->u4DisplayH==0))
    {
        if ((prJpgHdrInfo->u4Width > DISP_W_LMT) || (prJpgHdrInfo->u4Height > DISP_H_LMT))
        {
            prDispInfo->u4DisplayW = DISP_W_LMT;
            prDispInfo->u4DisplayH = DISP_H_LMT;
        }
        else
        {
            prDispInfo->u4DisplayW = prJpgHdrInfo->u4Width;
            prDispInfo->u4DisplayH = prJpgHdrInfo->u4Height;
        }
    }

    //---- modify display w:h ratio to original image w:h ratio ------------------
    if ((rInfo.eRotateDegree == CLOCKWISE_0) || (rInfo.eRotateDegree == CLOCKWISE_180) ||
            (rInfo.eRotateDegree == CLOCKWISE_0_WITH_FLIP) || (rInfo.eRotateDegree == CLOCKWISE_180_WITH_FLIP))
    {
        _CalKeepRatioRegion(rInfo.bKeepRatio, prJpgHdrInfo->u4Width, prJpgHdrInfo->u4Height,
                            prDispInfo->u4DisplayW, prDispInfo->u4DisplayH,
                            &prDispInfo->u4ScaleW, &prDispInfo->u4ScaleH);
    }
    else
    {
        _CalKeepRatioRegion(rInfo.bKeepRatio, prJpgHdrInfo->u4Width, prJpgHdrInfo->u4Height,
                            prDispInfo->u4DisplayH, prDispInfo->u4DisplayW,
                            &prDispInfo->u4ScaleW, &prDispInfo->u4ScaleH);
    }

#if 0//defined(CC_MT5360) && !defined(CC_MT5360B)      //MT5360, IMGRZ has some hw limitation
    if (prJpgHdrInfo->u4Height < prDispInfo->u4ScaleH)
    {
        if ((prDispInfo->u4ScaleW % 384) < 8)
        {
            prDispInfo->u4ScaleW = (prDispInfo->u4ScaleW / 384) * 384;
        }
    }
#endif

    //IMGRZ limitation
    if (prDispInfo->u4ScaleH == 0)
    {
        prDispInfo->u4ScaleH = 4;
    }
    if (prDispInfo->u4ScaleW < 4)
    {
        //prDispInfo->u4ScaleW = 1;
        return (INT32)E_IMG_DEC_FAIL;
    }

    prDispInfo->u4ScaleRowH=0;
    prDispInfo->u4DecodedRow=0;
    //----- check scalar limitation-----------------------------------------
    if (prJpgHdrInfo->au4RowWidth[1]==0)
    {
        prJpgHdrInfo->au4RowWidth[1] = prJpgHdrInfo->au4RowWidth[0];
        prJpgHdrInfo->au4RowWidth[2] = prJpgHdrInfo->au4RowWidth[0];
        prJpgHdrInfo->au4RowHeight[1] = prJpgHdrInfo->au4RowHeight[0];
        prJpgHdrInfo->au4RowHeight[2] = prJpgHdrInfo->au4RowHeight[0];
        prJpgHdrInfo->au4RowSize[1] = prJpgHdrInfo->au4RowSize[0];
        prJpgHdrInfo->au4RowSize[2] = prJpgHdrInfo->au4RowSize[0];
        //prDispInfo->u4CompNum = 1;
    }
    else
    {
        //prDispInfo->u4CompNum = 3;
    }
    if (((prJpgHdrInfo->au4RowWidth[1] / prDispInfo->u4ScaleW) > SCALAR_RATIO_LMT) ||
            (((prJpgHdrInfo->au4RowHeight[1] * prJpgHdrInfo->u4TotalRow) /prDispInfo->u4ScaleH) > SCALAR_RATIO_LMT) ||
            ((prDispInfo->u4ScaleW / prJpgHdrInfo->au4RowWidth[1]) > SCALAR_RATIO_LMT) ||
            ((prDispInfo->u4ScaleH / (prJpgHdrInfo->au4RowHeight[1] * prJpgHdrInfo->u4TotalRow)) > SCALAR_RATIO_LMT))
    {
        LOG(3, "Scaling factor out of limitation %d\n", (UINT32)SCALAR_RATIO_LMT);
        //return (INT32)E_IMG_DEC_FAIL;
    }

    //--- Display size decided by scaling size if using internal buffer---------------------
    if (prDispInfo->u4DispPitch == 0)
    {
        if ((rInfo.eRotateDegree == CLOCKWISE_0) || (rInfo.eRotateDegree == CLOCKWISE_180))
        {
            //prDispInfo->u4ScaleW = ALIGN16(prDispInfo->u4ScaleW);
            prDispInfo->u4DisplayW = prDispInfo->u4ScaleW;
            prDispInfo->u4DisplayH = prDispInfo->u4ScaleH;
            prDispInfo->u4DispPitch = ALIGN16(prDispInfo->u4ScaleW);
        }
        else
        {
            //prDispInfo->u4ScaleH = ALIGN16(prDispInfo->u4ScaleH);
            prDispInfo->u4DisplayW = prDispInfo->u4ScaleH;
            prDispInfo->u4DisplayH = prDispInfo->u4ScaleW;
            prDispInfo->u4DispPitch = ALIGN16(prDispInfo->u4ScaleH);
        }
        if ((prDispInfo->u4DisplayClrMode == (UINT32)Y_CBCR420_SEP16)||(prDispInfo->u4DisplayClrMode == (UINT32)Y_CBCR422_SEP16))
        {
            //for Y_CBCR422_SEP16 format in mtmplayer flow
            if (prImg->u1MemClass != (UINT8)EXTERNAL_RING_IN_DOUBLE_OUT)
            {
                prDispInfo->pu1DisplayStart[1] = prDispInfo->pu1DisplayStart[0] + (prDispInfo->u4DispPitch * prDispInfo->u4DisplayH);
                
                x_memset(VIRTUAL_VOID(prDispInfo->pu1DisplayStart[0]), 0, prDispInfo->u4DispPitch * prDispInfo->u4DisplayH);
                x_memset(VIRTUAL_VOID(prDispInfo->pu1DisplayStart[1]), 0x80, (prDispInfo->u4DispPitch * prDispInfo->u4DisplayH)>>1);
            }

            //for mjpeg, when sequence header is lost e.g. prSeqInfo->b_is_raw==1, we correct the header info automatically. //DTV00093836
            if (prDispInfo->u4DisplayClrMode<COLOR_MODE_MAX)
            {
                prDispInfo->u4DispPitch <<= _au4BytePerPixel[prDispInfo->u4DisplayClrMode];
            }
        }
        else
        {
            prDispInfo->u4DispPitch <<= 2;
        }
    }

    //--- restore feedback data -------------------------------------------
    prDecOut = rInfo.prDecOut;
    prDecOut->u4Pitch = prDispInfo->u4DispPitch;
    if (prDispInfo->u4DisplayClrMode< COLOR_MODE_MAX)
    {
        prDecOut->u4Depth = _au4BPP[prDispInfo->u4DisplayClrMode];
    }
    prDecOut->u4TotalLine = prDispInfo->u4DisplayH;
    prDecOut->pu1Addr = prDispInfo->pu1DisplayStart[0];
    prDecOut->u4CbCrOffset = (UINT32)prDispInfo->pu1DisplayStart[1] - (UINT32)prDispInfo->pu1DisplayStart[0];

    _rImgDecDbg.u4ScImgAddr[0] = (UINT32)prDispInfo->pu1DisplayStart[0];
    _rImgDecDbg.u4ScImgAddr[1] = (UINT32)prDispInfo->pu1DisplayStart[1];
    _rImgDecDbg.u4ScW = prDispInfo->u4DispPitch;
    _rImgDecDbg.u4ScH = prDispInfo->u4DisplayH;
    _rImgDecDbg.u4OrigImgW = prJpgHdrInfo->u4Width;
    _rImgDecDbg.u4OrigImgH = prJpgHdrInfo->u4Height;

    //--- adjust display start addr to middle ---------------------------------
    if ((rInfo.eRotateDegree == CLOCKWISE_0) || (rInfo.eRotateDegree == CLOCKWISE_180) ||
            (rInfo.eRotateDegree == CLOCKWISE_0_WITH_FLIP) || (rInfo.eRotateDegree == CLOCKWISE_180_WITH_FLIP))
    {
        //left-top point
        prDispInfo->u4StartX[0] += (((prDispInfo->u4DisplayW - prDispInfo->u4ScaleW) >> 1) & (~0x1));
        prDispInfo->u4StartX[1] = prDispInfo->u4StartX[0];
        prDispInfo->u4StartY[0] += ((prDispInfo->u4DisplayH - prDispInfo->u4ScaleH) >> 1);
        prDispInfo->u4StartY[1] = prDispInfo->u4StartY[0] >> prDispInfo->au4FactorW[1];

        //restore feedback data
        prDecOut->u4StartX = prDispInfo->u4StartX[0];
        prDecOut->u4StartY = prDispInfo->u4StartY[0];
        prDecOut->u4Width = prDispInfo->u4ScaleW;
        prDecOut->u4Height = prDispInfo->u4ScaleH;
        /*BC
        		//left-top point
        		prDispInfo->pu1DisplayStart[0] +=
        			((prDispInfo->u4StartY[0] * prDispInfo->u4DispPitch) +
        			(prDispInfo->u4StartX[0] << _au4BytePerPixel[prDispInfo->u4DisplayClrMode]));
        		prDispInfo->pu1DisplayStart[1] +=
        			((prDispInfo->u4StartY[1] * prDispInfo->u4DispPitch) +
        			(prDispInfo->u4StartX[1] << _au4BytePerPixel[prDispInfo->u4DisplayClrMode]));
        */
        //left-bottom point
        if ((rInfo.eRotateDegree == CLOCKWISE_180) || (rInfo.eRotateDegree == CLOCKWISE_180_WITH_FLIP))
        {
            prDispInfo->pu1DisplayStart[0] += (prDispInfo->u4DispPitch * prDispInfo->u4ScaleH);
            prDispInfo->pu1DisplayStart[1] += (prDispInfo->u4DispPitch * (prDispInfo->u4ScaleH>>prDispInfo->au4FactorH[1]));
        }
    }
    else	//90 degree or 270 degree
    {
        //left-top point, startX in the unit of byte
        prDispInfo->u4StartX[0] = ((prDispInfo->u4StartX[0] + (((prDispInfo->u4DisplayW - prDispInfo->u4ScaleH) >> 1) & (~0x1))) /*<< _au4BytePerPixel[prDispInfo->u4DisplayClrMode]*/);
        prDispInfo->u4StartX[1] = prDispInfo->u4StartX[0];
        prDispInfo->u4StartY[0] += ((prDispInfo->u4DisplayH - prDispInfo->u4ScaleW) >> 1);
        prDispInfo->u4StartY[1] = prDispInfo->u4StartY[0]>>prDispInfo->au4FactorW[1];

        //restore feedback data
        prDecOut->u4StartX = prDispInfo->u4StartX[0];
        prDecOut->u4StartY = prDispInfo->u4StartY[0];
        prDecOut->u4Width = prDispInfo->u4ScaleH;
        prDecOut->u4Height = prDispInfo->u4ScaleW;

        //right-top point
        if ((rInfo.eRotateDegree == CLOCKWISE_90) || (rInfo.eRotateDegree == CLOCKWISE_90_WITH_FLIP))
        {
            prDispInfo->u4StartX[0] += (prDispInfo->u4ScaleH /*<< _au4BytePerPixel[prDispInfo->u4DisplayClrMode]*/);//should be even number for cbcr start position
            prDispInfo->u4StartX[1] = prDispInfo->u4StartX[0];
        }
    }


    //--- get rows per segment for 1 gfx operation ----------------------------
    u4OrigRowW = prJpgHdrInfo->au4RowWidth[0] + prJpgHdrInfo->au4RowWidth[1] + prJpgHdrInfo->au4RowWidth[2];
    u4OrigRowSize = prJpgHdrInfo->au4RowSize[0] + prJpgHdrInfo->au4RowSize[1] + prJpgHdrInfo->au4RowSize[2];

    //Y/Cb/Cr
    u4ScaleRowW = (ALIGN16(prDispInfo->u4ScaleW) +
                   (ALIGN16(prDispInfo->u4ScaleW >> prDispInfo->au4FactorW[1]) << 1));
    u4ScaleRowH = (prDispInfo->u4ScaleH / prJpgHdrInfo->u4TotalRow) + 1;  //row height

    u4MemRowSize =  u4OrigRowSize +	(u4ScaleRowW * u4ScaleRowH);
    u4ReserveSize = (u4OrigRowW + u4ScaleRowW) * ((UINT32)RESERVE_LINES + (UINT32)SEG_DIFF_LINES);
    prDispInfo->u4RowsPerSeg = (((UINT32)DECODE_OUT_SIZE - u4ReserveSize) / (u4MemRowSize + 32 + 15))-1;

    if (prDispInfo->u4RowsPerSeg == 0)
    {
        LOG(3, "Decoded Buffer limitation\n");
        prDecOut->pu1Addr = NULL;
        prDispInfo->u4RowsPerSeg = 1;
    }
    if (prDispInfo->u4RowsPerSeg > prJpgHdrInfo->u4TotalRow)
    {
        prDispInfo->u4RowsPerSeg = prJpgHdrInfo->u4TotalRow;
    }
#if 0
    prDispInfo->u4ScaleRowH = (prDispInfo->u4ScaleH * prDispInfo->u4RowsPerSeg) / prJpgHdrInfo->u4TotalRow;  //row height
#else
    prDispInfo->u4ScaleRowH = u4ScaleRowH;
#endif
    /*
    	if(prDispInfo->u4ScaleRowH <= 2)
    	{
    		LOG(3, "Scaling operation limitation\n");
    		prDecOut->pu1Addr = NULL;
    		return (INT32)E_IMG_DEC_FAIL;
    	}
    */
#ifdef JPG_TIME_PROFILE
    HAL_GetTime(&_rHdrEnd);
#endif


    //--- Get JPG and display memory layout, do gfx segmentation --------------
    _GetMemLayout(prJpgHdrInfo->au4RowSize, prJpgHdrInfo->au4RowWidth, prImg, rInfo.rMjpg.ucFbgId);
#if 0
    if (prImg->u1MemClass != (UINT8)LINEAR_IN_LINEAR_OUT)
    {
        _GfxScaleInit(prImg->rHdrInfo, prImg->rMem, &prImg->rDispInfo);
    }
#endif
    //--- Reset decoding process
    prImg->u4RowIdx = 0;
    prImg->u4SegCnt = 0;
    return (INT32)E_IMG_OK;
}


INT32 _IMG_DecOneSegment(IMGHANDLE hCurImg, UINT32 u4ImgID, ENUM_IMG_ROTATE_PHASE eRotateDegree)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;
    JPEG_HEADER_INFO_T* prJpgHdrInfo;
    //GFXSC_SEG_RECORD_T* prJpgSeg;
    //#ifdef __KERNEL__
    IMG_LIB_FILL_RING_BUF_T rStreamBufInfo;
    //#endif
    //#if !defined( NDEBUG ) || defined( JPG_TIME_PROFILE)
    IMG_LIB_DISP_INFO_T* prDispInfo;
    //#endif

    INT32 i4Ret, i4Idx;
    static UINT32 au4BufUsedLine[3] = {0, 0, 0};		//decoded buffer used lines
    static UINT32 au4DecLine[3] = {0, 0, 0};			//accumulated decoded vertical size

#if 0
    static void *pavCompAddr[3] = {0, 0, 0};			//decoded buffer write pointer
#else
    static void *pavCompAddr[6] = {0, 0, 0, 0, 0, 0};			//decoded buffer write pointer for 539x
#endif

    ASSERT(prImg != NULL);

    prJpgHdrInfo = (JPEG_HEADER_INFO_T*)&prImg->rHdrInfo;
    //prJpgSeg = (GFXSC_SEG_RECORD_T*)&prImg->rDispInfo.arJpgSeg[0];
    //#if !defined( NDEBUG ) || defined( JPG_TIME_PROFILE)
    prDispInfo = (IMG_LIB_DISP_INFO_T*)&prImg->rDispInfo;
    //#endif

    if (prImg->u4RowIdx == 0)
    {
        for (i4Idx = 2; i4Idx >= 0; i4Idx--)
        {
            au4BufUsedLine[i4Idx] = 0;
            au4DecLine[i4Idx] = 0;
            pavCompAddr[i4Idx] = prImg->rMem.pavComp[i4Idx];
            //au4SegSrcVsize[i4Idx] = prJpgSeg[i4Idx].pVertSeg[prImg->u4SegCnt].u4SrcVSize;

            //for 539x
            pavCompAddr[i4Idx + 3] = prImg->rMem.pavComp[i4Idx + 3];
        }
    }

    LOG(3,"JpegFile Info:ScanNum:%d,Widht:%d,Height:%d\n",prJpgHdrInfo->u4NumScan,prJpgHdrInfo->u4Width,prJpgHdrInfo->u4Height);

    if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16)))
    {
        //422 format, Y buffer = W*H, W is 16bytes alignment.
        prDispInfo->pu1DisplayStart_temp[1]=prDispInfo->pu1DisplayStart_temp[0]+ ((prDispInfo->u4ScaleW+0xf)&(~0xf))*prDispInfo->u4ScaleH;
    }

    JDEC_MwSaveRzInfo(prImg, eRotateDegree);
    i4Ret = JDEC_SwitchToProgDec(prImg->hCurDec, &prImg->rMem);

    //handle progressive decoding result
    if (i4Ret == (INT32)E_IMG_OK)
    {
#ifdef CC_SWGAMMA
        if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&(prImg->rDispInfo.u4DisplayClrMode==AYCBCR8888_DIRECT32))
        {
            JDEC_SWGamma(_pu1TempStartAddr, &prImg->rDispInfo, _u4TempStartX, _u4TempStartY);
        }
#endif

        if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16)))
        {
            _IMG_ColorConVert( &prImg->rDispInfo, &prImg->rMem, prImg->rDispInfo.u4DisplayClrMode);
        }

        return (INT32)E_IMG_DEC_FINISH;
    }
    else if ((i4Ret == (INT32)E_IMG_CANCEL) || (i4Ret == (INT32)E_IMG_DEC_FAIL) ||
             (i4Ret == (INT32)E_IMG_HWBUSY))
    {
#ifdef CC_SWGAMMA
        if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&(prImg->rDispInfo.u4DisplayClrMode==AYCBCR8888_DIRECT32))
        {
            JDEC_SWGamma(_pu1TempStartAddr, &prImg->rDispInfo, _u4TempStartX, _u4TempStartY);
        }
#endif        
        return i4Ret;
    }

    while (prImg->u4RowIdx < prJpgHdrInfo->u4TotalRow)
    {
        // check decode stop signal
        if (prImg->bStopDec)
        {
            return (INT32)E_IMG_CANCEL;
        }


        if ((prImg->bIsEOF==FALSE) && (_IMG_GetBSBufInfo(hCurImg, &rStreamBufInfo) > (BS_SIZE - BS_PKT_SIZE)))
        {
            return (INT32)E_IMG_BSBUF_UNDERFLOW;
        }

        // prepare display row buffer------------------------------------
        //
        // set output buffer
#ifdef JPG_TIME_PROFILE
        _JpgRowStart();
#endif
        if (prImg->u1MemClass != (UINT8)LINEAR_IN_LINEAR_OUT)
        {
            if (JDEC_MwSetRowBuffer(prImg->hCurDec, pavCompAddr))
            {
                return (INT32)E_IMG_DEC_FAIL;
            }
        }
        // start row decode-------------------------------------------
        JDEC_MwSaveRzInfo(prImg, eRotateDegree);

        i4Ret = JDEC_MwDecodeRow(prImg->hCurDec);
        if (i4Ret == E_HWJPG_BUSY)
        {
            return (INT32)E_IMG_HWBUSY;
        }
        else if (i4Ret)
        {
            return (INT32)E_IMG_DEC_FAIL;
        }
        for (i4Idx=2; i4Idx>=0; i4Idx--)
        {
            au4DecLine[i4Idx] += prJpgHdrInfo->au4RowHeight[i4Idx];
            au4BufUsedLine[i4Idx] += prJpgHdrInfo->au4RowHeight[i4Idx];
        }
        prImg->u4RowIdx++;
#ifdef JPG_TIME_PROFILE
        _JpgRowEnd();
#endif

        //if(((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16)) && prImg->rDispInfo.u4ScaleRowH>0)
        //{
        //   _IMG_ColorConVert( &prImg->rDispInfo, &prImg->rMem, prImg->rDispInfo.u4DisplayClrMode);
        //}

        if ((eRotateDegree != CLOCKWISE_0) && (prImg->rDispInfo.u4ScaleRowH > 0))
        {
            _IMG_RotateJpg(eRotateDegree, &prImg->rDispInfo, &prImg->rMem);
#ifdef LINUX_TURNKEY_SOLUTION
            if ((prImg->u4RowIdx%15)==0)
            {
                x_thread_delay(1);
            }
#endif
        }

        prImg->rDispInfo.u4DecodedRow+=prImg->rDispInfo.u4ScaleRowH;

        //--- fill buffer-----------------------------------------------------
        if (prImg->bUseMw)
        {
            if ((!prImg->bStopDec)&&(prImg->u4RowIdx<prJpgHdrInfo->u4TotalRow)&&(prImg->bIsEOF==FALSE) && (_IMG_GetBSBufInfo(hCurImg, &rStreamBufInfo) > (UINT32)(BS_SIZE>>1)))
            {
                i4Ret = _IMG_BsBufManager(hCurImg, u4ImgID);    //MW callback function
                if (i4Ret == (INT32)E_IMG_OK)
                {
                    return (INT32)E_IMG_BSBUF_UNDERFLOW;
                }
                else if (i4Ret != (INT32)E_IMG_CANCEL)
                {
                    return i4Ret;
                }
            }
        }


    }


    if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16)))
    {
        _IMG_ColorConVert( &prImg->rDispInfo, &prImg->rMem, prImg->rDispInfo.u4DisplayClrMode);
    }

#ifdef JPG_TIME_PROFILE
    LOG(3, "\nImage width %d, height %d\n", prJpgHdrInfo->u4Width, prJpgHdrInfo->u4Height);
    HAL_GetTime(&_rPicEnd);
    HAL_GetDeltaTime(&_rTotalDuration, &_rPicStart, &_rPicEnd);
    HAL_GetDeltaTime(&_rHdrDuration, &_rHdrStart, &_rHdrEnd);
    LOG(0, "%d rows per group for gfx operation\n", prDispInfo->u4RowsPerSeg);
    LOG(0, "Total/JPG Header Parse/JPG hw/GFX Sc/GFX Cv/Fill Buf(sec):\n");
    LOG(0, "profile %u.%06u/%u.%06u/%u.%06u/%u.%06u/%u.%06u/%u.%06u\n",
        _rTotalDuration.u4Seconds, _rTotalDuration.u4Micros,
        _rHdrDuration.u4Seconds, _rHdrDuration.u4Micros,
        _rJpgHwAcc.u4Seconds, _rJpgHwAcc.u4Micros,
        _rGfxScAcc.u4Seconds, _rGfxScAcc.u4Micros,
        _rGfxCvAcc.u4Seconds, _rGfxCvAcc.u4Micros,
        _rFillBufAcc.u4Seconds, _rFillBufAcc.u4Micros);
#endif

#ifdef CC_SWGAMMA
    if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&(prImg->rDispInfo.u4DisplayClrMode==AYCBCR8888_DIRECT32))
    {
        if (prImg->bUseMw)
        {
            JDEC_SWGamma(_pu1TempStartAddr, &prImg->rDispInfo, _u4TempStartX, _u4TempStartY);
        }
        else
        {
            JDEC_SWGamma(prImg->rDispInfo.pu1DisplayStart[0], &prImg->rDispInfo,
                         prImg->rDispInfo.u4StartX[0], prImg->rDispInfo.u4StartY[0]);
        }
    }
#endif

//#if !defined( NDEBUG ) || defined(JPG_TIME_PROFILE)
    //UNUSED(prDispInfo);
//#endif

    return (INT32)E_IMG_DEC_FINISH;
}


void _IMG_Scale(PFN_JPG_FILL_SC_BUF pfnFillScBufCB, IMG_SCALE_INFO_T* prScOptInfo)
{
#if 0
    UINT32 u4VertSegLen, u4SegCnt, u4Offset, u4Idx, u4RepeatLine=0;	//scale in segment
    UINT32 u4W_LH[2] = {0, 0};	//[0] for Y, [1] for C
    UINT32 u4H_RH[2] = {0, 0};	//[0] for Y, [1] for C
    GFXSC_SEG_RECORD_T rJpgSeg;
    FBM_POOL_T *prFbmPool;

    ASSERT(prScOptInfo != NULL);
    ASSERT(pfnFillScBufCB != NULL);

    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);
    ASSERT(prFbmPool->u4Size > JPG_TOTAL_BUFFER_SIZE);

    //set CbCr property, byte number
    if (prScOptInfo->eColorFormat == Y_CBCR420_SEP16)
    {
        u4W_LH[1] = 0;
        u4H_RH[1] = 1;
    }
    else if (prScOptInfo->eColorFormat == Y_CBCR422_SEP16)
    {
        u4W_LH[1] = 0;
        u4H_RH[1] = 0;
    }
    else	//444
    {
        u4W_LH[1] = 1;
        u4H_RH[1] = 0;
    }

    //set scaling vertical source length
    u4VertSegLen = ((JPG_TOTAL_BUFFER_SIZE/
                     ((prScOptInfo->srcWidth<<u4W_LH[1]) + (prScOptInfo->dstWidth<<u4W_LH[1])))
                    *prScOptInfo->srcHeight)
                   /(prScOptInfo->srcHeight + prScOptInfo->dstHeight);

    for (u4Idx = 0; u4Idx < 2; u4Idx++)	//[0] for Y, [1] for C
    {
        //scaling initial
        x_memset((void *)&rJpgSeg, 0, sizeof(GFXSC_SEG_RECORD_T));
        rJpgSeg.pu1SrcAddr =  (void *)prFbmPool->u4Addr;		//working buffer
        rJpgSeg.pu1DstAddr = rJpgSeg.pu1SrcAddr + (u4VertSegLen * prScOptInfo->srcWidth);
        if (u4Idx == 0)	//Y
        {
            rJpgSeg.u4SrcColorMode = (UINT32)CM_SC_SINGLE;
            rJpgSeg.u4DstColorMode = (UINT32)CM_SC_SINGLE;
        }
        else			//CbCr
        {
            rJpgSeg.u4SrcColorMode = (UINT32)CM_SC_ARGB4444_DIRECT16;
            rJpgSeg.u4DstColorMode = (UINT32)CM_SC_ARGB4444_DIRECT16;
        }
        rJpgSeg.u4SrcPitch = ALIGN16(prScOptInfo->srcWidth);
        rJpgSeg.u4DstPitch = ALIGN16(prScOptInfo->dstWidth);
        rJpgSeg.u4LpfEn = 1;        // always 1
        rJpgSeg.u4RelSrcX = 0;
        rJpgSeg.u4RelSrcY = 0;
        rJpgSeg.u4RelDstX = 0;
        rJpgSeg.u4RelDstY = 0;

        rJpgSeg.u4SrcWidth = (prScOptInfo->srcWidth << u4W_LH[u4Idx])>>u4Idx;	//pixel number
        rJpgSeg.u4SrcHeight = prScOptInfo->srcHeight >> u4H_RH[u4Idx];

        rJpgSeg.u4DstWidth = (prScOptInfo->dstWidth<<u4W_LH[u4Idx])>>u4Idx;		//pixel number
        rJpgSeg.u4DstHeight = prScOptInfo->dstHeight >> u4H_RH[u4Idx];

        u4VertSegLen -= (RESERVE_LINES + SEG_DIFF_LINES);
        if (u4VertSegLen > rJpgSeg.u4SrcHeight)
        {
            u4VertSegLen = rJpgSeg.u4SrcHeight;
        }
        if (rJpgSeg.u4SrcHeight > rJpgSeg.u4DstHeight)
        {	//dst
            u4VertSegLen = (u4VertSegLen * prScOptInfo->dstHeight)/prScOptInfo->srcHeight;
        }
        GFXSC_JpgYScale(&rJpgSeg, TRUE, u4VertSegLen, 0);

        //start scaling
        u4RepeatLine = 0;
        for (u4SegCnt = 0; u4SegCnt < rJpgSeg.u4VertSegCount; u4SegCnt++)
        {
            //get source from user space data to segment buffer
            pfnFillScBufCB((UINT32)(VIRTUAL_UINT8(rJpgSeg.pu1SrcAddr) + (u4RepeatLine * rJpgSeg.u4SrcPitch)),
                           (rJpgSeg.pVertSeg[u4SegCnt].u4SrcVSize - u4RepeatLine) * rJpgSeg.u4SrcPitch);

            //do scaling
            GFXSC_JpgYScale(&rJpgSeg, FALSE, u4VertSegLen, u4SegCnt);

            //copy segment data to dst buffer
            u4Offset = rJpgSeg.pVertSeg[u4SegCnt].u4DstVSize * rJpgSeg.u4DstPitch;
            x_memcpy(VIRTUAL_VOID(prScOptInfo->pu1OutBuffer), VIRTUAL_VOID(rJpgSeg.pu1DstAddr), u4Offset);
            prScOptInfo->pu1OutBuffer += u4Offset;

            //copy repeated source line buffer to the front of segment source buffer
            if (u4SegCnt < (rJpgSeg.u4VertSegCount - 1))
            {
                u4RepeatLine = (rJpgSeg.pVertSeg[u4SegCnt].u4SrcY + rJpgSeg.pVertSeg[u4SegCnt].u4SrcVSize) - rJpgSeg.pVertSeg[u4SegCnt+1].u4SrcY;
                u4Offset = (rJpgSeg.pVertSeg[u4SegCnt+1].u4SrcY - rJpgSeg.pVertSeg[u4SegCnt].u4SrcY) * rJpgSeg.u4SrcPitch;
                x_memcpy(VIRTUAL_VOID(rJpgSeg.pu1SrcAddr),
                         VIRTUAL_VOID(rJpgSeg.pu1SrcAddr + u4Offset),
                         u4RepeatLine * rJpgSeg.u4SrcPitch);
            }
        }
    }
#endif
}


INT32 _IMG_BsBufManager(IMGHANDLE hCurImg, UINT32 u4ImgID)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;
    IMG_LIB_FILL_RING_BUF_T rStreamBufInfo;		//input buffer r/w control
    IMG_FILL_BUF_MW_T rFillBuf;

    ASSERT(prImg != NULL);

#ifdef JPG_TIME_PROFILE
    _FillBufStart();
#endif
    if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT) ||
            (prImg->u1MemClass == (UINT8)RING_IN_DOUBLE_OUT))
    {
        if (_GetBSBufInfo(prImg->hCurDec, &rStreamBufInfo))
        {
            return (INT32)E_IMG_BUF_FILL_FAIL;
        }

        if (rStreamBufInfo.rEntry1.bValid && rStreamBufInfo.rEntry2.bValid)
        {
            rFillBuf.ui4_trsn_id = 0;
            rFillBuf.pv_start_add = rStreamBufInfo.rEntry1.pvStartAddr;
            rFillBuf.ui4_required_len = rStreamBufInfo.rEntry1.u4RequiredLen;
            rFillBuf.b_reset_pos = FALSE;
            _pfnJpgCB(u4ImgID, prImg->pvTag, &rFillBuf, JPG_HW_NFY_FILL_BUF);
            rFillBuf.ui4_trsn_id = 1;
            rFillBuf.pv_start_add = rStreamBufInfo.rEntry2.pvStartAddr;
            rFillBuf.ui4_required_len = rStreamBufInfo.rEntry2.u4RequiredLen;
            _pfnJpgCB(u4ImgID, prImg->pvTag, &rFillBuf, JPG_HW_NFY_FILL_BUF);
        }
        else if (rStreamBufInfo.rEntry1.bValid)
        {
            rFillBuf.ui4_trsn_id = 1;
            rFillBuf.pv_start_add = rStreamBufInfo.rEntry1.pvStartAddr;
            rFillBuf.ui4_required_len = rStreamBufInfo.rEntry1.u4RequiredLen;
            rFillBuf.b_reset_pos = FALSE;
            _pfnJpgCB(u4ImgID, prImg->pvTag, &rFillBuf, JPG_HW_NFY_FILL_BUF);
        }
        else
        {
            return (INT32)E_IMG_CANCEL;
        }
        //VERIFY (x_sema_lock(_FillBuf_hSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
#ifdef JPG_TIME_PROFILE
    _FillBufEnd();
#endif

    return (INT32)E_IMG_OK;
}


void _IMG_NotifyStatus(IMGHANDLE hCurImg, UINT32 u4ImgID, INT32 i4Data, UINT32 u4Status)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;

    if (prImg)
    {
        _pfnJpgCB(u4ImgID, prImg->pvTag, (void*)i4Data, u4Status);
    }
}

/**
 * Get internal buffer info.
 *
 * @param prImgBufInfo buffer description.
 * @return void.
 */
void _IMG_GetBufInfo(IMG_BUF_INFO_T* prImgBufInfo)
{
    FBM_POOL_T *prFbmPool;
    ASSERT(prImgBufInfo != NULL);

    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);
    ASSERT(prFbmPool->u4Size > JPG_TOTAL_BUFFER_SIZE);

    prImgBufInfo->pu1ImageBuf = VIRTUAL_UINT8(prFbmPool->u4Addr + JPG_TOTAL_BUFFER_SIZE);
    prImgBufInfo->pu1WorkingBuf = VIRTUAL_UINT8(prFbmPool->u4Addr);
    prImgBufInfo->u4ImageBufSize = prFbmPool->u4Size - JPG_TOTAL_BUFFER_SIZE;
    prImgBufInfo->u4WorkingBufSize = JPG_TOTAL_BUFFER_SIZE;
}

/**
 * Get input buffer r/w pointer status and fill buffer information.
 *
 * @param prStreamBufInfo input ring buffer status.
 * @return void
 */
UINT32 _IMG_GetBSBufInfo(IMGHANDLE hCurImg, IMG_LIB_FILL_RING_BUF_T *prStreamBufInfo)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;
    UINT32 u4AvalSize = 0;

    ASSERT(prImg!=NULL);
    ASSERT(prStreamBufInfo!=NULL);

    if (_GetBSBufInfo(prImg->hCurDec, prStreamBufInfo))		//!=E_JPG_OK
    {
        return 0;
    }
    else
    {
        if (prStreamBufInfo->rEntry1.bValid)
        {
            u4AvalSize += prStreamBufInfo->rEntry1.u4RequiredLen;
        }
        if (prStreamBufInfo->rEntry2.bValid)
        {
            u4AvalSize += prStreamBufInfo->rEntry2.u4RequiredLen;
        }
        return u4AvalSize;
    }
}

#if 1
static VOID _IMG_ColorConVert(IMG_LIB_DISP_INFO_T* prDispInfo, IMG_LIB_MEM_LAYOUT_T* prImgLibMem, ENUM_IMG_DISP_COLOR_MODE u4DisplayClrMode)
{
    UINT8 *pu1OutAddr;
    UINT32 u4StartX,u4StartY,u4YSrcLen;
    GFX_COLOR_MODE_T u4ColorMode = CM_AYCbCr8888_DIRECT32;
    if (prDispInfo->u4Rotate==CLOCKWISE_0)
    {
        // UINT32 u4Offset=0;
        //u4Offset=prDispInfo->u4DecodedRow*(prDispInfo->u4DispPitch);
        //pu1OutAddr=prDispInfo->pu1DisplayStart[0]+u4Offset;
        pu1OutAddr = prDispInfo->pu1DisplayStart[0];
        u4StartX=prDispInfo->u4StartX[0];
        u4StartY=prDispInfo->u4StartY[0];
    }
    else
    {
        pu1OutAddr=prImgLibMem->pau1ScDstAddr[0];
        u4StartX=0;
        u4StartY=0;
    }

    switch (u4DisplayClrMode)
    {
    case AYCBCR8888_DIRECT32:
        u4ColorMode = CM_AYCbCr8888_DIRECT32;
        break;
    case ARGB8888_DIRECT32:
        u4ColorMode = CM_ARGB8888_DIRECT32;
        break;
    case ARGB4444_DIRECT16:
        u4ColorMode = CM_ARGB4444_DIRECT16;
        break;
    default:
        u4ColorMode = CM_AYCbCr8888_DIRECT32;
        break;

    }



    u4YSrcLen=(prDispInfo->u4ScaleW+ 0xF) & (~0xF);
    GFX_Lock();
    //CM_CbYCrY422_DIRECT16, CM_YCbCr_CLUT8
    GFX_SetDst(pu1OutAddr, u4ColorMode, prDispInfo->u4DispPitch);
    //GFX_SetColCnvSrc(prImgLibMem->pau1ScDst2Addr[0],u4YSrcLen,prImgLibMem->pau1ScDst2Addr[1],u4YSrcLen,0);
    GFX_SetColCnvSrc(prDispInfo->pu1DisplayStart_temp[0],u4YSrcLen,prDispInfo->pu1DisplayStart_temp[1],u4YSrcLen,0);
    GFX_SetColCnvFmt(E_YCFMT_422LINEAR,E_SWAP_0,E_VSTD_BT709,E_VSYS_COMP);
    GFX_SetAlpha((UINT32)0xff);
    //GFX_ColConv(u4StartX,u4StartY,prDispInfo->u4ScaleW,prDispInfo->u4ScaleRowH);
    GFX_ColConv(u4StartX,u4StartY,prDispInfo->u4ScaleW,prDispInfo->u4ScaleH);
    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();
}

static VOID _IMG_RotateJpg(ENUM_IMG_ROTATE_PHASE eRotatePhase, IMG_LIB_DISP_INFO_T* prDispInfo, IMG_LIB_MEM_LAYOUT_T* prImgLibMem)
{
    IMG_REGION_T rSrc, rDst;
    ENUM_IMG_DISP_COLOR_MODE u4DisplayClrMode;
    UINT32 u4ScaleW, u4ScaleRowH;

    UINT32 u4DispPitch, u4DispStartX[2], u4DispStartY[2];
    UINT32 u4IsCClock, u4StepY, u4StepC;
    UINT32 u4SwitchOp = 0;
    UINT8 *pu1DisplayStart[2];
    //UINT8 *pau1ScDstAddr[3];
    //UINT8 *pu1Addr;
    UINT8 *pu1TempInAddr, *pu1TempOutAddr;

    ASSERT(prDispInfo !=NULL);
    ASSERT(prImgLibMem!=NULL);
    u4DisplayClrMode = prDispInfo->u4DisplayClrMode;

    u4ScaleW = prDispInfo->u4ScaleW;
    u4ScaleRowH = prDispInfo->u4ScaleRowH;
    u4DispPitch = prDispInfo->u4DispPitch;

    //pu1DisplayStartVirt[0] = VIRTUAL_UINT8(pu1DisplayStart[0]);
    //pu1DisplayStartVirt[1] = VIRTUAL_UINT8(pu1DisplayStart[1]);
    //pau1ScDstAddr[0] = VIRTUAL_VOID(prImgLibMem->pau1ScDstAddr[0]);
    //pau1ScDstAddr[1] = VIRTUAL_VOID(prImgLibMem->pau1ScDstAddr[1]);

    u4DispStartX[0] = prDispInfo->u4StartX[0];
    u4DispStartX[1] = prDispInfo->u4StartX[1];
    u4DispStartY[0] = prDispInfo->u4StartY[0];
    u4DispStartY[1] = prDispInfo->u4StartY[1];

    x_memset((void *)&rSrc, 0, sizeof(IMG_REGION_T));
    x_memset((void *)&rDst, 0, sizeof(IMG_REGION_T));

    //first, do mirror operation
    if ((eRotatePhase == CLOCKWISE_0_WITH_FLIP) || (eRotatePhase == CLOCKWISE_90_WITH_FLIP) || (eRotatePhase == CLOCKWISE_270_WITH_FLIP))
    {
        pu1DisplayStart[0] = prDispInfo->pu1DisplayStart[0];
        pu1DisplayStart[1] = prDispInfo->pu1DisplayStart[1];
        u4SwitchOp |= D_GFXFLAG_MIRROR;

        pu1TempOutAddr = prImgLibMem->pau1ScDstAddr[1];
        if (u4DisplayClrMode<COLOR_MODE_MAX)
        {
            rSrc.u4Pitch = ALIGN16(u4ScaleW << _au4BytePerPixel[u4DisplayClrMode]);
        }
        rSrc.u4StartX = 0;
        rSrc.u4StartY = 0;
        rDst.u4StartX = (eRotatePhase == CLOCKWISE_0_WITH_FLIP) ? u4DispStartX[0] : (UINT32)0;
        rDst.u4StartY = (eRotatePhase == CLOCKWISE_0_WITH_FLIP) ? u4DispStartY[0] : (UINT32)0;

        rSrc.pu1Addr = prImgLibMem->pau1ScDstAddr[0];
        rSrc.u4Width = u4ScaleW;
        rSrc.u4Height = u4ScaleRowH;
        rDst.u4Pitch = (eRotatePhase == CLOCKWISE_0_WITH_FLIP) ? u4DispPitch : rSrc.u4Pitch;
        rDst.u4Width = rSrc.u4Width;
        rDst.u4Height = rSrc.u4Height;
        rDst.pu1Addr = (eRotatePhase == CLOCKWISE_0_WITH_FLIP) ? pu1DisplayStart[0] : pu1TempOutAddr;

        if (u4DisplayClrMode == (UINT32)AYCBCR8888_DIRECT32)
        {
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_AYCbCr8888_DIRECT32, u4SwitchOp);
        }
        else if (u4DisplayClrMode==ARGB8888_DIRECT32)
        {
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_ARGB8888_DIRECT32, u4SwitchOp);
        }
        else if (u4DisplayClrMode==ARGB4444_DIRECT16)
        {
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_ARGB4444_DIRECT16, u4SwitchOp);
        }
        else//not yet
        {
            //Y
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4SwitchOp);
            //CbCr
            rSrc.pu1Addr = prImgLibMem->pau1ScDstAddr[1];
            rSrc.u4Pitch = ALIGN16(u4ScaleW);
            rSrc.u4StartX = 0;
            rSrc.u4StartY = 0;
            rSrc.u4Width = u4ScaleW;
            rSrc.u4Height = u4ScaleRowH;
            rDst.pu1Addr = pu1DisplayStart[1];
            rDst.u4Pitch = u4DispPitch;
            rDst.u4Width = rSrc.u4Width;
            rDst.u4Height = rSrc.u4Height;
            rDst.u4StartX = 0;
            rDst.u4StartY = 0;
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4SwitchOp);

        }
    }

    if ((eRotatePhase == CLOCKWISE_0) || (eRotatePhase == CLOCKWISE_0_WITH_FLIP))
    {
        //don't need to do anything
    }
    else if ((eRotatePhase == CLOCKWISE_180_WITH_FLIP) || (eRotatePhase == CLOCKWISE_180))
    {
        prDispInfo->pu1DisplayStart[0] -= (u4DispPitch * u4ScaleRowH);
        prDispInfo->pu1DisplayStart[1] += u4ScaleW >> prDispInfo->au4FactorH[1];
        pu1DisplayStart[0] = prDispInfo->pu1DisplayStart[0];
        pu1DisplayStart[1] = prDispInfo->pu1DisplayStart[1];

        if (eRotatePhase == CLOCKWISE_180)
        {
            u4SwitchOp |= D_GFXFLAG_FLIP | D_GFXFLAG_MIRROR;
        }
        else
        {
            u4SwitchOp |= D_GFXFLAG_FLIP;
        }
        rSrc.pu1Addr = prImgLibMem->pau1ScDstAddr[0];
        if (u4DisplayClrMode<COLOR_MODE_MAX)
        {
            rSrc.u4Pitch = ALIGN16(u4ScaleW << _au4BytePerPixel[u4DisplayClrMode]);//ALIGN16(u4DispPitch);
        }
        rSrc.u4StartX = 0;//u4DispStartX[0];
        rSrc.u4StartY = 0;//u4DispStartY[0];
        rSrc.u4Width = u4ScaleW;
        rSrc.u4Height = u4ScaleRowH;
        rDst.pu1Addr = pu1DisplayStart[0];
        rDst.u4Pitch = u4DispPitch;
        rDst.u4Width = rSrc.u4Width;
        rDst.u4Height = rSrc.u4Height;
        rDst.u4StartX = u4DispStartX[0];
        rDst.u4StartY = u4DispStartY[0];

        if (u4DisplayClrMode == (UINT32)AYCBCR8888_DIRECT32)
        {
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_AYCbCr8888_DIRECT32, u4SwitchOp);
        }
        else if (u4DisplayClrMode==(UINT32)ARGB8888_DIRECT32)
        {
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_ARGB8888_DIRECT32, u4SwitchOp);
        }
        else if (u4DisplayClrMode==(UINT32)ARGB4444_DIRECT16)
        {
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_ARGB4444_DIRECT16, u4SwitchOp);
        }
        else//not yet
        {
            //Y
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4SwitchOp);
            //CbCr
            rSrc.pu1Addr = prImgLibMem->pau1ScDstAddr[1];
            rSrc.u4Pitch = ALIGN16(u4ScaleW);
            rSrc.u4StartX = 0;
            rSrc.u4StartY = 0;
            rSrc.u4Width = u4ScaleW;
            rSrc.u4Height = u4ScaleRowH;
            rDst.pu1Addr = pu1DisplayStart[1];
            rDst.u4Pitch = u4DispPitch;
            rDst.u4Width = rSrc.u4Width;
            rDst.u4Height = rSrc.u4Height;
            rDst.u4StartX = 0;
            rDst.u4StartY = 0;
            _GfxExeBitBlt(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4SwitchOp);
        }
    }
    else    //rotation
    {
        pu1DisplayStart[0] = prDispInfo->pu1DisplayStart[0];
        pu1DisplayStart[1] = prDispInfo->pu1DisplayStart[1];
        if ((eRotatePhase == CLOCKWISE_90_WITH_FLIP) || (eRotatePhase == CLOCKWISE_270_WITH_FLIP))
        {
            pu1TempInAddr = prImgLibMem->pau1ScDstAddr[1];
        }
        else
        {
            pu1TempInAddr = prImgLibMem->pau1ScDstAddr[0];
        }

        if (u4DisplayClrMode == (UINT32)AYCBCR8888_DIRECT32 || u4DisplayClrMode==ARGB8888_DIRECT32 || u4DisplayClrMode==ARGB4444_DIRECT16)
        {
            u4StepY = 1;
            u4StepC = 1;
        }
        else
        {
            u4StepY = 1;
            u4StepC = 2;
        }

        rSrc.pu1Addr = pu1TempInAddr;
        if (u4DisplayClrMode<COLOR_MODE_MAX)
        {
            rSrc.u4Pitch = ALIGN16(u4ScaleW << _au4BytePerPixel[u4DisplayClrMode]);
        }
        rSrc.u4StartX = 0;
        rSrc.u4StartY = 0;
        rSrc.u4Width = u4ScaleW;
        rSrc.u4Height = u4ScaleRowH;
        rDst.pu1Addr = pu1DisplayStart[0];
        rDst.u4Pitch = u4DispPitch;
        rDst.u4Width = rSrc.u4Height;
        rDst.u4Height = rSrc.u4Width;

        if ((eRotatePhase == CLOCKWISE_90) || (eRotatePhase == CLOCKWISE_90_WITH_FLIP))
        {
            u4IsCClock = 0;
            rDst.u4StartY = u4DispStartY[0];								//left-top point
            rDst.u4StartX = u4DispStartX[0] - u4ScaleRowH;
        }
        else
        {
            u4IsCClock = 1;
            rDst.u4StartY = (u4DispStartY[0] + rDst.u4Height) - 1;			//left-bottom point
            rDst.u4StartX = u4DispStartX[0];
        }

        if (u4DisplayClrMode == (UINT32)AYCBCR8888_DIRECT32)
        {
            _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_AYCbCr8888_DIRECT32, u4IsCClock, u4StepY);
        }
        else if (u4DisplayClrMode==(UINT32)ARGB8888_DIRECT32)
        {
            _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_ARGB8888_DIRECT32, u4IsCClock, u4StepY);
        }
        else if (u4DisplayClrMode==(UINT32)ARGB4444_DIRECT16)
        {
            _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_ARGB4444_DIRECT16, u4IsCClock, u4StepY);
        }
        else
        {
            //Y
            _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4IsCClock, u4StepY);

            rSrc.pu1Addr = pu1TempInAddr;
            rSrc.u4Pitch = ALIGN16(u4ScaleW);
            rSrc.u4StartX = 0;
            rSrc.u4StartY = 0;
            rSrc.u4Width = u4ScaleW;
            rSrc.u4Height = u4ScaleRowH;
            rDst.pu1Addr = pu1DisplayStart[0];
            rDst.u4Pitch = u4DispPitch;
            rDst.u4Width = rSrc.u4Width;
            rDst.u4Height = rSrc.u4Height;
            rDst.u4StartX = 0;
            rDst.u4StartY = 0;
            //CbCr
            _GfxRotatePhase90(rSrc, rDst, (UINT32)CM_YCbCr_CLUT8, u4IsCClock, u4StepC);
        }

        if ((eRotatePhase == CLOCKWISE_90) || (eRotatePhase == CLOCKWISE_90_WITH_FLIP))
        {
            prDispInfo->u4StartX[0] -= (u4ScaleRowH * u4StepY);
            //prDispInfo->u4StartX[1] -= (au4CurScaledLine[1] * u4StepC);
        }
        else
        {
            prDispInfo->u4StartX[0] += (u4ScaleRowH * u4StepY);
            //prDispInfo->u4StartX[1] += (au4CurScaledLine[1] * u4StepC);
        }
    }

    if ((eRotatePhase == CLOCKWISE_0) || (eRotatePhase == CLOCKWISE_0_WITH_FLIP))
    {
        prDispInfo->pu1DisplayStart[0] += (u4DispPitch * u4ScaleRowH);//= pu1DisplayStart[0];
        prDispInfo->pu1DisplayStart[1] += u4ScaleW >> prDispInfo->au4FactorH[1];//= pu1DisplayStart[1];
    }
}
#endif

/******************************************************************************
* Local	Function definition
******************************************************************************/
/**
 * Set display buffer parameter.
 *
 * @param pu1Src display start address.
 * @param u4SrcHorSize display buffer width.
 * @param u4SrcVerSize display buffer height.
 * @param u4Pitch display buffer bytes per line.
 * @param u4DispColorMode display buffer color mode (ENUM_IMG_DISP_COLOR_MODE).
 * @return void
 */
static INT32 _SetDispBuffer(IMG_LIB_DISP_INFO_T* prDisp, IMG_REGION_T rDst, ENUM_IMG_DISP_COLOR_MODE u4DispColorMode, ENUM_IMG_ROTATE_PHASE ePhase)
{
    FBM_POOL_T *prFbmPool;
    UINT8* pu1Src;
    UINT32 u4AvailableSize;

    ASSERT(prDisp != NULL);

    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);
    //ASSERT(prFbmPool->u4Size > JPG_TOTAL_BUFFER_SIZE);

    pu1Src = rDst.pu1Addr;
    if (pu1Src == NULL)
    {
        u4AvailableSize = prFbmPool->u4Size - JPG_TOTAL_BUFFER_SIZE;
        if (u4AvailableSize < ((rDst.u4Pitch * rDst.u4Height) <<1))
        {
            return -1;
        }
        pu1Src = (UINT8*)(prFbmPool->u4Addr + JPG_TOTAL_BUFFER_SIZE);
        rDst.u4Pitch = 0;	//modified later to fit scaling size
    }
    //check buffer limitation


    //--- clear display buffer for verification
#ifdef JPG_SEMI_HOST
    if (u4DispColorMode==AYCBCR8888_DIRECT32 || u4DispColorMode==ARGB8888_DIRECT32)
    {
        x_memset(VIRTUAL_VOID(pu1Src), 0x80, rDst.u4Height * rDst.u4Pitch);
    }
    else if (u4DispColorMode==(UINT32)Y_CBCR420_SEP16)
    {
        x_memset(VIRTUAL_VOID(pu1Src), 0, rDst.u4Height * rDst.u4Width);
        x_memset(VIRTUAL_VOID((UINT32)pu1Src + rDst.u4CbCrOffset), 0x80, (rDst.u4Height * rDst.u4Width)>>1);
    }
    else if (u4DispColorMode==(UINT32)ARGB4444_DIRECT16)
    {
        x_memset(VIRTUAL_VOID(pu1Src), 0x80, (rDst.u4Height * rDst.u4Pitch)>>1);
    }
    else
    {
        x_memset(VIRTUAL_VOID(pu1Src), 0, rDst.u4Height * rDst.u4Width);
        x_memset(VIRTUAL_VOID((UINT32)pu1Src + rDst.u4CbCrOffset), 0x80, (rDst.u4Height * rDst.u4Width));
    }
#endif

    //--- restore info in data structure
    //for ARGB temp framebuffer for gfx convertion.
    prDisp->pu1DisplayStart_temp[0]= prDisp->pu1DisplayStart_temp[1] = (UINT8*)(prFbmPool->u4Addr + JPG_WORKING_BUFFER_SIZE);


    prDisp->pu1DisplayStart[0] = pu1Src;

    if ((u4DispColorMode == Y_CBCR420_SEP16)||(u4DispColorMode == Y_CBCR422_SEP16)||(u4DispColorMode == Y_CBCR444_YCSEP24))
    {
        //for mjpeg layout
        prDisp->pu1DisplayStart[1] = pu1Src + rDst.u4CbCrOffset;
    }
    else
    {
        prDisp->pu1DisplayStart[1] = pu1Src;
    }
    prDisp->u4StartX[0] = rDst.u4StartX;
    prDisp->u4StartX[1] = 0;
    prDisp->u4StartY[0] = rDst.u4StartY;
    prDisp->u4StartY[1] = 0;
    prDisp->u4DisplayW = rDst.u4Width;// & (~0x1F); //should be multiples of 32
    prDisp->u4DisplayH = rDst.u4Height;// & (~0x1F);
    prDisp->u4DispPitch = rDst.u4Pitch;				// pixel number * byte per pixel
    prDisp->u4DisplayClrMode = u4DispColorMode;
    prDisp->u4Rotate=ePhase;


    //--- choose output display mode, output CbCr factor --------------------
    if (prDisp->u4DisplayClrMode == AYCBCR8888_DIRECT32 || prDisp->u4DisplayClrMode==(UINT32)ARGB8888_DIRECT32|| prDisp->u4DisplayClrMode==(UINT32)ARGB4444_DIRECT16)
    {
        prDisp->u4ScaleFMT = (UINT32)E_YCFMT_RES_2;
        //display image Y:Cr h, w ratio
        prDisp->au4FactorH[1] = 0;		//h, Y:Cb = 2:1
        prDisp->au4FactorW[1]= 0;			//w, Y:Cb = 2:1
        prDisp->au4FactorH[2] = 0;		//h, Y:Cb = 2:1
        prDisp->au4FactorW[2]= 0;			//w, Y:Cb = 2:1
    }
    else if (prDisp->u4DisplayClrMode == (UINT32)Y_CBCR420_SEP16)
    {
        prDisp->u4ScaleFMT = (UINT32)E_YCFMT_420LINEAR;
        //display image Y:Cr h, w ratio
        prDisp->au4FactorH[1] = 1;		//h, Y:Cb = 2:1
        prDisp->au4FactorW[1]= 1;			//w, Y:Cb = 2:1
        prDisp->au4FactorH[2] = 1;		//h, Y:Cb = 2:1
        prDisp->au4FactorW[2]= 1;			//w, Y:Cb = 2:1
    }
    else //orig 444, 422, 422V, or u4DisplayClrMode == CM_YCbYCr422_DIRECT16, use 422 display mode
    {
        prDisp->u4ScaleFMT = (UINT32)E_YCFMT_422LINEAR;
        //display image Y:Cr h, w ratio
        if ((ePhase == CLOCKWISE_90) || (ePhase == CLOCKWISE_90_WITH_FLIP) ||
                (ePhase == CLOCKWISE_270) || (ePhase == CLOCKWISE_270_WITH_FLIP))
        {
            prDisp->au4FactorH[1] = 1;
            prDisp->au4FactorW[1] = 0;
            prDisp->au4FactorH[2] = 1;
            prDisp->au4FactorW[2] = 0;
        }
        else
        {
            prDisp->au4FactorH[1] = 0;	//h, Y:Cb = 1:1
            prDisp->au4FactorW[1] = 1;     //w, Y:Cb = 2:1
            prDisp->au4FactorH[2] = 0;	//h, Y:Cb = 1:1
            prDisp->au4FactorW[2] = 1;     //w, Y:Cb = 2:1
        }
    }
    return 0;
}


/**
 * Get input buffer r/w pointer status and fill buffer information.
 *
 * @param prStreamBufInfo input ring buffer status.
 * @return void
 */
static INT32 _GetBSBufInfo(JPEGHANDLE hCurDec, IMG_LIB_FILL_RING_BUF_T *prStreamBufInfo)
{
    INT32 i4Ret;
    void *pvBuf1;
    void *pvBuf2;
    UINT32 u4Size1;
    UINT32 u4Size2;

    /* check current jpeg handle */
    if (hCurDec == NULL)
    {
        return (INT32)E_JPG_ERR_PARAM;
    }
    if (prStreamBufInfo == NULL)
    {
        return (INT32)E_JPG_ERR_PARAM;
    }

    u4Size1 = 0;
    u4Size2 = 0;
    i4Ret = JDEC_MwFillBuffer(hCurDec, &pvBuf1, &u4Size1, 0);

    if (i4Ret)
    {
        return (INT32)E_JPG_ERR_GENERAL;
    }
    i4Ret = JDEC_MwFillBuffer(hCurDec, &pvBuf2, &u4Size2, u4Size1);

    if (i4Ret)
    {
        return (INT32)E_JPG_ERR_GENERAL;
    }

    prStreamBufInfo->rEntry1.u4FillLen	= 0;
    prStreamBufInfo->rEntry2.u4FillLen	= 0;
    if (u4Size1 == 0)
    {
        prStreamBufInfo->rEntry1.bValid		= FALSE;
        prStreamBufInfo->rEntry1.u4RequiredLen	= 0;
    }
    else
    {
        prStreamBufInfo->rEntry1.bValid		= TRUE;
        prStreamBufInfo->rEntry1.pvStartAddr	= pvBuf1;
        prStreamBufInfo->rEntry1.u4RequiredLen	= u4Size1;
    }

    if (u4Size2 == 0)
    {
        prStreamBufInfo->rEntry2.bValid		= FALSE;
        prStreamBufInfo->rEntry2.u4RequiredLen	= 0;
    }
    else
    {
        prStreamBufInfo->rEntry2.bValid		= TRUE;
        prStreamBufInfo->rEntry2.pvStartAddr	= pvBuf2;
        prStreamBufInfo->rEntry2.u4RequiredLen	= u4Size2;
    }

    return (INT32)E_JPG_OK;
}


/**
 * Get memory layout for decoder and scalar.
 *
 * @param prMem memory layout structure.
 * @return void
 */
static void _GetMemLayout(const UINT32* pu4RowSize, const UINT32* pu4RowWidth, IMG_LIB_INFO_T* prImg, UCHAR ucFbgId)
{
    FBM_POOL_T *prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);;
    //IMG_LIB_DISP_INFO_T rDispInfo = prImg->rDispInfo;
    IMG_LIB_MEM_LAYOUT_T* prMem = &prImg->rMem;
    UINT32 u4WorkingAddr, u4Size;

    ASSERT(pu4RowSize != NULL);
    ASSERT(pu4RowWidth != NULL);
    ASSERT(prImg != NULL);
    ASSERT(prMem != NULL);
    /* prepare output ping-pong buffer, rowsize + 1 linebuffer for scalar-----*/
    if (prImg->u1MemClass == (UINT8)EXTERNAL_RING_IN_DOUBLE_OUT)
    {
        FBM_GetWorkingBuffer(ucFbgId, &u4WorkingAddr, &u4Size);
        prMem->pavComp[0] = (void *)((u4WorkingAddr + 15)  & ~15);
    }
    else
    {
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != NULL);
        ASSERT(prFbmPool->u4Size > JPG_TOTAL_BUFFER_SIZE);

        //decode out buffer
        prMem->pavComp[0] = (void *)((prFbmPool->u4Addr + DECODE_OUT_OFFSET + 32 + 15) & ~15);	//alignment issue
    }
#if 0
    //for 538x
    prMem->pavComp[1] = (void *)((UINT32)prMem->pavComp[0] +
                                 (pu4RowSize[0] * rDispInfo.u4RowsPerSeg) +
                                 (pu4RowWidth[0]*((UINT32)RESERVE_LINES + (UINT32)SEG_DIFF_LINES)));
    prMem->pavComp[2] = (void *)((UINT32)prMem->pavComp[1] +
                                 (pu4RowSize[1]* rDispInfo.u4RowsPerSeg) +
                                 (pu4RowWidth[1]*((UINT32)RESERVE_LINES + (UINT32)SEG_DIFF_LINES)));
    prMem->pau1ScDstAddr[0] = (UINT8 *)((UINT32)prMem->pavComp[2] +
                                        (pu4RowSize[2]* rDispInfo.u4RowsPerSeg) +
                                        (pu4RowWidth[2]*((UINT32)RESERVE_LINES + (UINT32)SEG_DIFF_LINES)));

#else

    prMem->pavComp[3] = (void *)((UINT32)prMem->pavComp[0] + pu4RowSize[0]);
    prMem->pavComp[1] = (void *)((UINT32)prMem->pavComp[3] + pu4RowSize[0]);
    prMem->pavComp[4] = (void *)((UINT32)prMem->pavComp[1] + pu4RowSize[0]);
    prMem->pavComp[2] = (void *)((UINT32)prMem->pavComp[4] + pu4RowSize[0]);
    prMem->pavComp[5] = (void *)((UINT32)prMem->pavComp[2] + pu4RowSize[0]);
    prMem->u4RzTempBuf = ((UINT32)prMem->pavComp[5] + pu4RowSize[0] + 15) & ~15;

#endif
    //scalar out buffer
#if 1
    prMem->u4CurrentUse = prMem->u4RzTempBuf + RZ_MAX_TEMP_BUF_SIZE;
    if (prImg->u1MemClass != (UINT8)EXTERNAL_RING_IN_DOUBLE_OUT)
    {
#ifdef LINUX_TURNKEY_SOLUTION
        prMem->pau1ScDstAddr[0] = (UINT8*)(prMem->u4CurrentUse);          //for rotation
        prMem->pau1ScDstAddr[1] = (UINT8 *)(prMem->pau1ScDstAddr[0] + (200 * 1024));//for rotation
        prMem->u4CurrentUse+=(400*1024);
        if (0)//((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16))
        {
            if (prImg->rDispInfo.u4Rotate!=CLOCKWISE_0)
            {
                prMem->pau1ScDst2Addr[0]=prMem->pau1ScDstAddr[1]+(200*1024);
                prMem->pau1ScDst2Addr[1]=prMem->pau1ScDst2Addr[0]+(32*1024);
                prMem->u4CurrentUse+=(64*1024);
            }
            else
            {
                prMem->pau1ScDst2Addr[0]=prMem->pau1ScDstAddr[0];
                prMem->pau1ScDst2Addr[0]=prMem->pau1ScDstAddr[1];
            }
        }
#else
        prMem->pau1ScDstAddr[0] = (UINT8*)(prFbmPool->u4Addr + JPG_WORKING_BUFFER_SIZE);          //for rotation
        prMem->pau1ScDstAddr[1] = (UINT8 *)(prMem->pau1ScDstAddr[0] + (200 * 1024));//for rotation
        if (0)//((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16))
        {
            if (prImg->rDispInfo.u4Rotate!=CLOCKWISE_0)
            {
                prMem->pau1ScDst2Addr[0]=prMem->pau1ScDstAddr[1]+(200*1024);
                prMem->pau1ScDst2Addr[1]=prMem->pau1ScDst2Addr[0]+(32*1024);
            }
            else
            {
                prMem->pau1ScDst2Addr[0]=prMem->pau1ScDstAddr[0];
                prMem->pau1ScDst2Addr[1]=prMem->pau1ScDstAddr[1];
            }
        }
#endif
    }
#else
    prMem->pau1ScDstAddr[0] += u4YScRsvSize;    //Y
    prMem->pau1ScDstAddr[1] = (UINT8 *)(prMem->pau1ScDstAddr[0] + u4YScSize + u4CbCrScRsvSize);//Cb
    prMem->pau1ScDstAddr[2] = prMem->pau1ScDstAddr[1] + u4CbCrScSize + u4CbCrScRsvSize; //Cr

    _rImgDecDbg.u4ScWorkBufStart[0] = (UINT32)prMem->pau1ScDstAddr[0];
    _rImgDecDbg.u4ScWorkBufEnd[0] = (UINT32)prMem->pau1ScDstAddr[0] + u4YScSize;
    _rImgDecDbg.u4ScWorkBufStart[1] = (UINT32)prMem->pau1ScDstAddr[1];
    _rImgDecDbg.u4ScWorkBufEnd[1] = (UINT32)prMem->pau1ScDstAddr[1] + u4CbCrScSize;
    _rImgDecDbg.u4ScWorkBufStart[2] = (UINT32)prMem->pau1ScDstAddr[2];
    _rImgDecDbg.u4ScWorkBufEnd[2] = (UINT32)prMem->pau1ScDstAddr[2] + u4CbCrScSize;

    LOG(6,"ScaleOut WorkingBuf:Y 0x%x--0x%x, Cb 0x%x--0x%x, Cr 0x%x--0x%x\n",
        _rImgDecDbg.u4ScWorkBufStart[0],
        _rImgDecDbg.u4ScWorkBufEnd[0],
        _rImgDecDbg.u4ScWorkBufStart[1],
        _rImgDecDbg.u4ScWorkBufEnd[1],
        _rImgDecDbg.u4ScWorkBufStart[2],
        _rImgDecDbg.u4ScWorkBufEnd[2]);
#endif
}

BOOL _IMG_IsFullImgDecoding(void)
{
    FBM_POOL_T *prFbmPool1, *prFbmPool2;

#ifdef CC_DFB_SUPPORT_VDP_LAYER
	FBM_POOL_T *prFbmPoolVDP;

    prFbmPoolVDP = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPG_VDP);

    if(prFbmPoolVDP && prFbmPoolVDP->u4Size)
    {
        return TRUE;
    }
#endif
    prFbmPool1 = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG_OSD3);
    prFbmPool2 = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG_OSD4);

    if ((prFbmPool1 == NULL) || (prFbmPool2 == NULL))
    {
        return FALSE;
    }
    if ((!prFbmPool1->u4Size) || (!prFbmPool2->u4Size))
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * Gfx rotate 90 degree.
 *
 * @param
 * @return
 */
static void _GfxRotatePhase90(IMG_REGION_T rSrc, IMG_REGION_T rDst, UINT32 u4Colormode, UINT32 u4IsCClockWise, UINT32 u4DstStep)
{
    UINT32 u4Sx, u4Sy, u4Dx, u4Dy;
    UINT32 i;

    //ASSERT(rSrc.u4Width == rDst.u4Height);
    //ASSERT(rDst.u4Width == (rSrc.u4Height * u4DstStep));

    u4Sx = rSrc.u4StartX;
    u4Sy = rSrc.u4StartY;
    u4Dx = rDst.u4StartX;
    u4Dy = rDst.u4StartY;

    GFX_Lock();
    //CM_CbYCrY422_DIRECT16, CM_YCbCr_CLUT8
    GFX_SetSrc(rSrc.pu1Addr, u4Colormode, rSrc.u4Pitch);
    GFX_SetDst(rDst.pu1Addr, u4Colormode, rDst.u4Pitch);

    GFX_SetHoriToVertLineOpt(u4IsCClockWise);

    if (u4IsCClockWise == GFX_ROTATE_C_CLOCKWISE)
    {
        // rotated by 90" counter-clock-wise
        for (i = 0; i < rSrc.u4Height; i++)
        {
            GFX_HoriToVertLine(u4Sx, u4Sy, u4Dx, u4Dy, rSrc.u4Width);

            u4Sy++;
            u4Dx += u4DstStep;
        } // ~for
    }
    else
    {
        // rotated by 90" clock-wise
        u4Dx += (rDst.u4Width - u4DstStep);
        //ASSERT(u4Dx > 0);
        for (i = 0; i < rSrc.u4Height; i++)
        {
            GFX_HoriToVertLine(u4Sx, u4Sy, u4Dx, u4Dy, rSrc.u4Width);

            u4Sy++;
            u4Dx -= u4DstStep;
        } // ~for
    }

    // flush and wait
    GFX_Flush();
    GFX_Wait();

    GFX_Unlock();
}
#if 1
/**
 * Gfx
 *
 * @param
 * @return
 */
static void _GfxExeBitBlt(IMG_REGION_T rSrc, IMG_REGION_T rDst, UINT32 u4Colormode, UINT32 u4SwitchOp)
{
    UINT32 u4Sx, u4Sy, u4Dx, u4Dy, u4Width, u4Height;

    //ASSERT(rSrc.u4Width == rDst.u4Height);
    //ASSERT(rDst.u4Width == (rSrc.u4Height * u4DstStep));

    u4Sx = rSrc.u4StartX;
    u4Sy = rSrc.u4StartY;
    u4Dx = rDst.u4StartX;
    u4Dy = rDst.u4StartY;
    u4Width = rDst.u4Width;
    u4Height = rDst.u4Height;

    GFX_Lock();
    //CM_CbYCrY422_DIRECT16, CM_YCbCr_CLUT8
    GFX_SetSrc(rSrc.pu1Addr, u4Colormode, rSrc.u4Pitch);
    GFX_SetDst(rDst.pu1Addr, u4Colormode, rDst.u4Pitch);

    GFX_SetBltOpt(u4SwitchOp, 0, 0);
    GFX_BitBlt(u4Sx, u4Sy, u4Dx, u4Dy, u4Width, u4Height);

    // flush and wait
    GFX_Flush();
    GFX_Wait();

    GFX_Unlock();
}
#endif


static void _CalKeepRatioRegion(BOOL bKeepRatio, UINT32 u4SrcW, UINT32 u4SrcH,
                                UINT32 u4DstW, UINT32 u4DstH,
                                UINT32* pu4ScaleW, UINT32* pu4ScaleH)
{
    if ((pu4ScaleW == NULL) || (pu4ScaleH == NULL))
    {
        return;
    }
    if (bKeepRatio)
    {
        *pu4ScaleW = (u4DstH * u4SrcW)/u4SrcH;
        if (*pu4ScaleW <= u4DstW)
        {
            *pu4ScaleH = u4DstH;
        }
        else
        {
            *pu4ScaleW = u4DstW;
            *pu4ScaleH = (u4DstW * u4SrcH)/u4SrcW;
        }
    }
    else
    {
        *pu4ScaleW = u4DstW;
        *pu4ScaleH = u4DstH;
    }
    *pu4ScaleW &= ~(0x1);
    *pu4ScaleH &= ~(0x1);
    //u4ScaleW &= ~(0x1F);		//pitch 32 bytes align

}


void _FreeImgInstance(IMGHANDLE* phCurImg)
{
    IMG_LIB_INFO_T *prImg;

    ASSERT(phCurImg != NULL);

    prImg = (IMG_LIB_INFO_T *)(*phCurImg);
    if (prImg)
    {
        if (prImg->hCurDec)
        {
            JDEC_FreeInstance(prImg->hCurDec);
            prImg->hCurDec = (JPEGHANDLE)0;
        }
        x_mem_free(*phCurImg);
        *phCurImg = (IMGHANDLE)0;
    }
}

#ifdef JPG_TIME_PROFILE
static void _JpgRowStart(void)
{
    HAL_GetTime(&_rJpgRowStart);
}

static void _JpgRowEnd(void)
{
    HAL_TIME_T rRowTime;
    HAL_GetTime(&_rJpgRowEnd);
    HAL_GetDeltaTime(&rRowTime, &_rJpgRowStart, &_rJpgRowEnd);
    _rJpgHwAcc.u4Seconds += rRowTime.u4Seconds;
    _rJpgHwAcc.u4Micros  += rRowTime.u4Micros;
    if (_rJpgHwAcc.u4Micros >= 1000000)
    {
        _rJpgHwAcc.u4Seconds += _rJpgHwAcc.u4Micros / 1000000;
        _rJpgHwAcc.u4Micros  %= 1000000;
    }
}

static void _FillBufStart(void)
{
    HAL_GetTime(&_rFillBufStart);
}

static void _FillBufEnd(void)
{
    HAL_TIME_T rRowTime;
    HAL_GetTime(&_rFillBufEnd);
    HAL_GetDeltaTime(&rRowTime, &_rFillBufStart, &_rFillBufEnd);
    _rFillBufAcc.u4Seconds += rRowTime.u4Seconds;
    _rFillBufAcc.u4Micros  += rRowTime.u4Micros;
    if (_rFillBufAcc.u4Micros >= 1000000)
    {
        _rFillBufAcc.u4Seconds += _rFillBufAcc.u4Micros / 1000000;
        _rFillBufAcc.u4Micros  %= 1000000;
    }
}

static void _GfxScStart(void)
{
    HAL_GetTime(&_rGfxScStart);
}

static void _GfxScEnd(void)
{
    HAL_TIME_T rRowTime;
    HAL_GetTime(&_rGfxScEnd);
    HAL_GetDeltaTime(&rRowTime, &_rGfxScStart, &_rGfxScEnd);
    _rGfxScAcc.u4Seconds += rRowTime.u4Seconds;
    _rGfxScAcc.u4Micros  += rRowTime.u4Micros;
    if (_rGfxScAcc.u4Micros >= 1000000)
    {
        _rGfxScAcc.u4Seconds += _rGfxScAcc.u4Micros / 1000000;
        _rGfxScAcc.u4Micros  %= 1000000;
    }
}

static void _GfxCvStart(void)
{
    HAL_GetTime(&_rGfxCvStart);
}

static void _GfxCvEnd(void)
{
    HAL_TIME_T rRowTime;
    HAL_GetTime(&_rGfxCvEnd);
    HAL_GetDeltaTime(&rRowTime, &_rGfxCvStart, &_rGfxCvEnd);
    _rGfxCvAcc.u4Seconds += rRowTime.u4Seconds;
    _rGfxCvAcc.u4Micros  += rRowTime.u4Micros;
    if (_rGfxCvAcc.u4Micros >= 1000000)
    {
        _rGfxCvAcc.u4Seconds += _rGfxCvAcc.u4Micros / 1000000;
        _rGfxCvAcc.u4Micros  %= 1000000;
    }
}
#endif
