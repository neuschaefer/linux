/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/

#ifndef __PNG_HAL_DEC_H
#define __PNG_HAL_DEC_H

#include "x_os.h"
#include "png_com_hdr.h"
#include "x_img_dec.h"
#include "png_hal_reg.h"
#include "x_hal_arm.h"

#define CACHE(addr)                                   ((addr) & 0x7fffffff)
#define NONCACHE(addr)                                (CACHE(addr) | 0x80000000)
#define CONFIG_DRV_FPGA_BOARD   (0)
#define CONFIG_DRV_VERIFY_SUPPORT  (1)
#define CONFIG_DRV_FAST_LOGO (0)
#define PNG_MMU_TEST         (0)
#if (PNG_MMU_TEST)
//extern void   vIIOMMU_Init(void);
//extern UINT32 u4HalGetTTB1(void);
#define  u4HalGetTTB1 HalGetMMUTableAddress
#endif
#define PNG_HAL_INVALID32      0xFFFFFFFF
#define PNG_HAL_RTN_STOPPED  4
#define PNG_HAL_RTN_DEC      3
#define PNG_HAL_RTN_SRC      2
#define PNG_HAL_RTN_DST      1
#define PNG_HAL_RTN_OK       0
#define PNG_HAL_RTN_FAIL     -1
#define PNG_HAL_ALDER_ERR    -2


#define PNG_HAL_TIMEOUT     1000

typedef struct _PNG_HAL_PIC_INFO_T
{
    PNG_IHDR            *ptIhdr;
    GFX_COLORMODE_T     eOutClrMd;
    UINT8               u4AlphaUnMatchedVal;
    UINT32              u4FifoStart;
    UINT32              u4FifoEnd;
    UINT32              u4DstBufPitch;
    UINT32              u4SliceHeight;
    UINT32              u4CropX;
    UINT32              u4CropY;
    UINT32              u4CropWidth;
    UINT32              u4CropHeight;
    UINT32              u4ResizeHor;    
    UINT32              u4ResizeVrt;
} PNG_HAL_PIC_INFO_T;

typedef struct _PNG_HAL_IDAT_INFO_T
{
    UINT32              u4ChunkType;
    UINT32              u4SrtStrmAddr;
    UINT32              u4SrtStrmLen;
    UINT32              u4DstBufStart;
    UINT32              u4DstBufEnd;
    BOOL                fgLastIDATGrp;
    BOOL                fgLastSlice;
    BOOL                fg1stIDATChunk;
} PNG_HAL_IDAT_INFO_T;

enum
{
    PNG_CLK_XTAL = 0, //0:27MHZ
    PNG_CLK_SAWLESSPLL_D1P5,//1:288MHZ
    PNG_CLK_SYSPLL_D2,//2:324MHZ
    PNG_CLK_USBPLL_D2,//3: 240MHZ
    PNG_CLK_SYSPLL_D2P5,//4: 259MHZ
    PNG_CLK_ENETPLL_D2,//5: 337MHZ
    PNG_CLK_USBPLL_D1P5,//6: 320MHZ
    PNG_CLK_ENETPLL_D3,//7: 225MHZ
    PNG_CLK_MAX
};

INT32 i4PngHalInitHwInst(void);
INT32 i4PngHalUnInitHwInst(void);
INT32 i4PngHalWaitDec(UINT32 u4HwInstId, UINT32 u4WaitTime);

INT32 i4PngHalGetHwInst(UINT32 *pu4HwInstId);
INT32 i4PngHalRelHwInst(UINT32 u4HwInstId);
INT32 i4PngHalSetScale(UINT32 u4HwInstId, UINT32 u4PngInterlaceScale,UINT32 u4PngHorScale,UINT32 u4PngVrtScale);

INT32 i4PngHalSetStop(UINT32 u4HwInstId);
INT32 i4PngHalReset(UINT32 u4HwInstId);
INT32 i4PngHalEnable(UINT32 u4HwInstId);
INT32 i4PngHalDisable(UINT32 u4HwInstId);

INT32 i4PngHalSetPicInfo(UINT32 u4HwInstId, PNG_HAL_PIC_INFO_T *ptPngPicInfo);
INT32 i4PngHalSetPLTE(UINT32 u4HwInstId, PNG_PLTE *ptPngPlte);
INT32 i4PngHalSettRNS(UINT32 u4HwInstId, PNG_TRNS *ptPngTrns, BOOL fgPlte);
INT32 i4PngHalSetbKGD(UINT32 u4HwInstId, PNG_BKGD *ptPngBkgd, PNG_PLTE *ptPngPlte, PNG_TRNS *ptPngTrns, BOOL fgPlte, BOOL fgTrns);

INT32 i4PngHalProcIDAT(UINT32 u4HwInstId, PNG_HAL_IDAT_INFO_T *ptPngIdatInfo);
INT32 i4PngHalProcNextSrc(UINT32 u4HwInstId, UINT32 u4SrcStrmSrtAddr, UINT32 u4SrcStrmLen, BOOL fgLastIDATGroup);
INT32 i4PngHalProcNextDst(UINT32 u4HwInstId, UINT32 u4DstBufStart, UINT32 u4DstBufEnd, BOOL fgLastSlice);
UINT32 u4PngHalReadCrc(UINT32 u4HwInstId);
INT32 i4PngHalSetSliceWH(UINT32 u4HwInstId, UINT32 u4SliceHeight);

INT32 i4PngHalSetAutoSkipDisable(UINT32 u4HwInstId);
INT32 i4PngHalSetAutoSkipEnable(UINT32 u4HwInstId);
INT32 i4PngHalSetAutoSkipLastIdat(UINT32 u4HwInstId, BOOL fgLastIdat);
UINT32 u4PngHalGetAutoSkipCrc(UINT32 u4HwInstId);
void* x_alloc_aligned_nc_mem(UINT32 u4Size, UINT32 u4Align);
void x_free_aligned_nc_mem(void *pUser);
#if (PNG_MMU_TEST)
void vPngMMUInit(void);
void vFlushPNGMMU(void);
#endif
void vPngSWReset(void);

#endif // __PNG_HAL_DEC_H
