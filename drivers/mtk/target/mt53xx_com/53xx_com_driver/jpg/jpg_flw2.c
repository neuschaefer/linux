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
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "jpg_common.h"
#include "jpg_if.h"
#include "jpg_jfif.h"
#include "jpg_hw.h"
#include "jpg_mem.h"

#include "jpg_flw2.h"
#include "jpg_debug.h"
#include "osd_if.h"
#include "pmx_if.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_if.h"

#ifdef JPEG_VLD_V3
#ifdef VLD0_BASE
#undef VLD0_BASE
#define VLD0_BASE   JPG_VLD_BASE
#endif
#endif

#ifdef JPEG_AUTO_TEST
extern INT32 _JpgVerifyCmdGetParam(INT32 i4ParamType, INT32 Param);
extern INT32 _JpgVerifyCmdSetParam(INT32 i4ParamType,INT32 Param);
#endif
static VOID _JDEC_SetRzV(RZ_JPG_SCL_PARAM_SET_T *prJpgParam, UINT32 *pu4RowHeight);
#define JPG_COLLECT_LIMIT (1024 * 1024)

static const UINT8 _JpgBitsDcLuminance[17] =
{ 0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
static const UINT8 _JpgValDcLuminance[] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

static const UINT8 _JpgBitsDcChrominance[17] =
{ 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
static const UINT8 _JpgValDcChrominance[] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

static const UINT8 _JpgBitsAcLuminance[17] =
{ 0, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };
static const UINT8 _JpgValAcLuminance[] =
{ 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
  0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
  0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
  0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
  0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
  0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
  0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
  0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
  0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
  0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
  0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
  0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
  0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
  0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
  0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
  0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
  0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
  0xf9, 0xfa 
};

static const UINT8 _JpgBitsAcChrominance[17] =
{ 0, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };

static const UINT8 _JpgValAcChrominance[] =
{ 0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
  0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
  0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
  0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
  0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
  0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
  0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
  0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
  0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
  0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
  0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
  0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
  0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
  0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
  0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
  0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
  0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
  0xf9, 0xfa 
};

static UINT32 u4BytePerPixel[] = {0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 1, 1, 1, 2, 0}; //left shift value

//static INT32 _i4JpgDecMode;
//static void  _jpg_SetDecMode5391(INT32 i4DecMode);
//static INT32 _jpg_GetDecMode5391(void);
#ifdef JPEG_AUTO_TEST
#define JPGHW_TIME_PROFILE
#endif

#ifdef JPGHW_TIME_PROFILE
static HAL_TIME_T _rDecStart, _rRZStart;
static HAL_TIME_T _rDecEnd, _rRZEnd;
static HAL_TIME_T _rDecAcc, _rRZAcc;
static HAL_TIME_T _rDecdif, _rRZdif;
#endif
static UINT32 _u4JpgRzTempBuf;
static IMG_LIB_INFO_T *prJpgImg;
static ENUM_IMG_ROTATE_PHASE eJpgRotateDegree;
static UINT32 _u4JPGMode = 0;
static ENUM_IMG_BUFFER_TYPE _eJPGMemClass = LINEAR_IN_LINEAR_OUT;
static INT32 _jpg_RowRz5391Drv(JPEGHANDLE hFlow, UINT32 u4RowIdx, 
     UINT32 u4MCUsPerRow, UINT32 u4TotalRows, UINT8 **apu1OutputAddr);
#ifdef JPEG_AUTO_TEST
static INT32 _jpg_Flow			(JPEGHANDLE	hFlow);
static INT32 _jpg_Row			(JPEGHANDLE	hFlow, UINT32 u4Row);
#endif
static INT32 _jpg_Glob			(JPEGHANDLE	hFlow);
static INT32 _jpg_Scan			(JPEGHANDLE	hFlow, INT32 i4Scan);
static INT32 _jpg_Misc			(JPEGHANDLE	hFlow);
static BOOL _fgJpgFlw2Stop = FALSE;
static UCHAR _acRange[900] = {0};
static INT32 _ai4RR[1024];
static INT32 _ai4BB[1024];
static INT32 _ai4RG[1024];
static INT32 _ai4BG[1024];
//static UINT16 _au2GammaR[256], _au2GammaG[256], _au2GammaB[256];

static UINT32 _u4YTgWidth = 512;
static UINT32 _u4YTgHeight = 256;
//extern void vDrvSetSWGammaFlg(UINT8 bFlagOnOff);
UINT32 _JPG_GetYTgW(void)
{
    return _u4YTgWidth;
}

UINT32 _JPG_GetYTgH(void)
{
    return _u4YTgHeight;
}

void _JPG_SetYTgW(UINT32 u4Width)
{
    _u4YTgWidth = u4Width;
}

void _JPG_SetYTgH(UINT32 u4Height)
{
    _u4YTgHeight = u4Height;
}

void _JPG_SetSwGamma(BOOL fgEnable)
{
//    vDrvSetSWGammaFlg((UINT8)fgEnable);
}
//extern void vDrvGetOSDGammaCurve(UINT16 * bOSDGammaRCurve, UINT16 * bOSDGammaGCurve, UINT16 * bOSDGammaBCurve);
//extern UINT8 bDrvGetSWGammaFlg(void);

void JDEC_BuildTable(void)
{
    INT32 i4I, i4X;    
    UCHAR *ucTable = _acRange;   
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        for (i4I = 0, i4X = -128; i4I <= 255; i4I++, i4X++)    
        {        
            _ai4RR[i4I] = (91882 * i4X + 32768) >> 16;        
            _ai4BB[i4I] = (116130 * i4X + 32768) >> 16;        
            _ai4RG[i4I] = -46802 * i4X;        
            _ai4BG[i4I] = (-22554 * i4X) + 32768;    
        }
        ucTable += 256;    
        for (i4I = 0; i4I < 256; i4I++)    
        {        
            ucTable[i4I] = i4I;    
        }    
        ucTable += 128;    
        for (i4I = 128; i4I < 512; i4I++)    
        {        
            ucTable[i4I] = 255;    
        }
        fgInit = TRUE;
    }
#if 0    
    vDrvGetOSDGammaCurve(_au2GammaR, _au2GammaG, _au2GammaB);
#endif
}

/* base	jpeg decoding flow function	*/
static INT32 _jpg_Glob(JPEGHANDLE hFlow)
{
	/*
		1. set progressive flag
		2. set glob	n comp
		3. set h v factor of each comp
		4. set picture width hieght
		5. set huff	table, dc and ac
		6. if progressive flag,	set	nz buffer
		7. set q table
		8. set restart interval
		9. validate	glob data
	*/

	INT32 i4Ret	= (INT32)E_JPG_OK;
	UINT8 u1Comp;
	UINT32 u4Count;

	UINT32 u4MaxWidth;
	UINT32 u4MaxHeight;
	
	ASSERT(hFlow);

	/* set progressive flag	*/
	if (JPGHDR->rSOF.eJpegFormat ==	E_JPG_BASELINE)
	{
		TRY2( VLDJPG_SetGlobProgressive(H_JPGHW,	FALSE) );
		JPGMEMBS->fgProgScan = FALSE;
	}
	else if	(JPGHDR->rSOF.eJpegFormat == E_JPG_PROGRESSIVE_HUFFMAN)
	{
		TRY2( VLDJPG_SetGlobProgressive(H_JPGHW,	TRUE) );
		JPGMEMBS->fgProgScan = TRUE;
		//TRY2( JFIF_ParseProgScan(H_JPGHDR) );
//             JPGMEM->pfnClean(H_JPGMEM);
//             return -(INT32)E_JPG_ERR_UNSUPPORT;		
	}
	else
	{
	      Printf("Format %d Unsupport\n", JPGHDR->rSOF.eJpegFormat);
            JPGMEM->pfnClean(H_JPGMEM);
            return -(INT32)E_JPG_ERR_UNSUPPORT;
	}

	/*
		set	jpeg mem before	decode flow
			UINT32	u4Width;
			UINT32	u4Height;
	*/
	JPGMEMBS->u4Width	= (UINT32)JPGHDR->rSOF.u2ImageWidth;
	JPGMEMBS->u4Height	= (UINT32)JPGHDR->rSOF.u2ImageHeight;

	/* set num of components in	the	picture	*/
	TRY2( VLDJPG_SetGlobNumComp(H_JPGHW,	JPGHDR->rSOF.u1NumComponents) );

	/* set h v factor of each component	*/
	u4MaxWidth = 0;
	u4MaxHeight	= 0;
	for	(u1Comp	= 0; u1Comp	< JPGHDR->rSOF.u1NumComponents;	u1Comp++)
	{
		UINT8 u1FactorH;
		UINT8 u1FactorV;
		if (u1Comp >= (UINT8)3)
		{
		    return -(INT32)E_JPG_ERR_UNSUPPORT;
		}
		u1FactorH =	JPGHDR->rSOF.arSofComp[u1Comp].u1HSampFactor;
		u1FactorV =	JPGHDR->rSOF.arSofComp[u1Comp].u1VSampFactor;
		/* set hw decoder object */
		TRY2( VLDJPG_SetGlobBlkWidth(H_JPGHW, u1Comp, u1FactorH)	);
		TRY2( VLDJPG_SetGlobBlkHeight(H_JPGHW, u1Comp, u1FactorV) );
		u4MaxWidth = MAX(u4MaxWidth, (UINT32)u1FactorH);
		u4MaxHeight	= MAX(u4MaxHeight, (UINT32)u1FactorV);
		/* set jpg mem object */
		JPGMEMBS->aau4CompFactor[u1Comp][HORI] = u1FactorH;
		JPGMEMBS->aau4CompFactor[u1Comp][VERT] = u1FactorV;
		JPGMEMBS->au4CompBlock[u1Comp] = u1FactorH * u1FactorV;
	}
	if (JPGHDR->rSOF.u1NumComponents == 1)
	{
	    JPGMEMBS->aau4CompFactor[1][HORI] = 0;
		JPGMEMBS->aau4CompFactor[1][VERT] = 0;
		JPGMEMBS->au4CompBlock[1] = 0;
        JPGMEMBS->aau4CompFactor[2][HORI] = 0;
		JPGMEMBS->aau4CompFactor[2][VERT] = 0;
		JPGMEMBS->au4CompBlock[2] = 0;
	}
	JPGMEMBS->au4MaxFactor[HORI] = u4MaxWidth;
	JPGMEMBS->au4MaxFactor[VERT] = u4MaxHeight;

	/* set picture width and height	*/
	TRY2( VLDJPG_SetGlobPicWidth(H_JPGHW, (UINT32)JPGHDR->rSOF.u2ImageWidth)	);
	TRY2( VLDJPG_SetGlobPicHeight(H_JPGHW, (UINT32)JPGHDR->rSOF.u2ImageHeight) );

	/*
	  set huff table / q table in baseline mode	or first scan,
	  other	scan of	progressive	mode should	set	in scan
	  and this condition is always true :
		if (i4Scan == 0	|| (! JPGHW->fgProgressive))
	  actually,	these code can remove safely. the reason to	leave'em
	  here is just for flow	compatibility to dc's original code
	*/

	
	if (JPGHDR->rDHT.u4NumDcTbl == 0)
	{
	    x_memcpy((void *)JPGHDR->rDHT.arDcTbl[0].au1Bits, 
	            (void *)_JpgBitsDcLuminance, (UINT32)sizeof(_JpgBitsDcLuminance));
   	    x_memcpy((void *)JPGHDR->rDHT.arDcTbl[0].au1HuffVal, 
	            (void *)_JpgValDcLuminance, (UINT32)sizeof(_JpgValDcLuminance));
   	    x_memcpy((void *)JPGHDR->rDHT.arDcTbl[1].au1Bits, 
	            (void *)_JpgBitsDcChrominance, (UINT32)sizeof(_JpgBitsDcChrominance));
	    x_memcpy((void *)JPGHDR->rDHT.arDcTbl[1].au1HuffVal, 
	            (void *)_JpgValDcChrominance, (UINT32)sizeof(_JpgValDcChrominance));

	    x_memcpy((void *)JPGHDR->rDHT.arAcTbl[0].au1Bits, 
	            (void *)_JpgBitsAcLuminance, (UINT32)sizeof(_JpgBitsAcLuminance));
   	    x_memcpy((void *)JPGHDR->rDHT.arAcTbl[0].au1HuffVal, 
	            (void *)_JpgValAcLuminance, (UINT32)sizeof(_JpgValAcLuminance));
   	    x_memcpy((void *)JPGHDR->rDHT.arAcTbl[1].au1Bits, 
	            (void *)_JpgBitsAcChrominance, (UINT32)sizeof(_JpgBitsAcChrominance));
	    x_memcpy((void *)JPGHDR->rDHT.arAcTbl[1].au1HuffVal, 
	            (void *)_JpgValAcChrominance, (UINT32)sizeof(_JpgValAcChrominance));
	    
        JPGHDR->rDHT.fgDcTblLoaded = (UINT32)0x3;
        JPGHDR->rDHT.fgAcTblLoaded = (UINT32)0x3;
	}

	/* set huff	table */
	for	(u4Count = 0; u4Count < ((UINT32)D_MAX_JPEG_HUFF_TBL>>1)/*JPGHDR->rDHT.u4NumDcTbl*/; u4Count++)
	{
		if (JPGHDR->rDHT.fgDcTblLoaded & D_BIT(u4Count))
		{
	        //LOG(3,"Derive DC Huff Tbl %d\n", u4Count);
			TRY2( VLDJPG_SetGlobDcHuffTbl(H_JPGHW, (UINT8)u4Count,
										 JPGHDR->rDHT.arDcTbl[u4Count].au1Bits,
										 JPGHDR->rDHT.arDcTbl[u4Count].au1HuffVal) );
		}
		if (JPGHDR->rDHT.fgAcTblLoaded & D_BIT(u4Count))
		{
	        //LOG(3,"Derive AC Huff Tbl %d\n", u4Count);
			TRY2( VLDJPG_SetGlobAcHuffTbl(H_JPGHW, (UINT8)u4Count,
										 JPGHDR->rDHT.arAcTbl[u4Count].au1Bits,
										 JPGHDR->rDHT.arAcTbl[u4Count].au1HuffVal) );
		}
	}

	/* set q table */
	for	(u4Count = 0; u4Count < D_MAX_JPEG_QTBL/*(UINT32)JPGHDR->rDQT.u1NumQ*/; u4Count++)
	{
		TRY2( VLDJPG_SetGlobQtbl(H_JPGHW, (UINT8)u4Count, &JPGHDR->rDQT.aau1Qtbl[u4Count][0]) );
	}


	/* calculate mcu width-height and mcu size */
	JPGMEMBS->u4McuWidth = (u4MaxWidth == 2) ?
						   ((JPGHW->u4PicWidth + 15) >>	4) :
						   ((JPGHW->u4PicWidth +  7) >>	3);
	JPGMEMBS->u4McuHeight =	(u4MaxHeight ==	2) ?
							((JPGHW->u4PicHeight + 15) >> 4) :
							((JPGHW->u4PicHeight +	7) >> 3);
	JPGMEMBS->u4McuCount = JPGMEMBS->u4McuWidth	* JPGMEMBS->u4McuHeight;

	/* allocate	all	buffers	we need	using mem object */
	JPGMEM->pfnInit(H_JPGMEM);

	TRY2( VLDJPG_SetRestartInterval(H_JPGHW,	(UINT32)(JPGHDR->u2RestartInterval)) );

	TRY2( VLDJPG_ValidateGlobData(H_JPGHW) );


	CATCH_ERROR()
	{
		if (i4Ret)
		{
			JPGMEM->pfnClean(H_JPGMEM);
		}
		return i4Ret;
	}
}

static INT32 _jpg_Scan(JPEGHANDLE hFlow, INT32 i4Scan)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	UINT32 u4Comp, u4Block,	u4Count;
	UINT32 u4ProgMode;

	ASSERT(hFlow);
	/*
		1. parsing scan	related	table
		2. set scan	data start
		3. set comp	in scan
		4. associate q tbl with	components,	dc/ac id with blocks
		5. set scan	alah
		6. set scan	sess
		7. set mcu row start and row end
		8. validate	scan data
	*/

	//BC add
	JPGHW->u4McuCount = 0;

	/* parsing scan	related	table */
	if (JPGHW->fgProgressive &&	JPGHDR->arSOS[i4Scan].pu1ScanTableStart	&& i4Scan)
	{
		/* set current header parsing to scan table	start */
		JPGHDR->pu1CurrRead	= JPGHDR->arSOS[i4Scan].pu1ScanTableStart;
		/* parsing header again	*/
		Printf("Parsing	progressive	scan DQT and DHT 0x%x\n", (UINT32)JPGHDR->pu1CurrRead);
		TRY2( JFIF_ParseProgMarker(H_JPGHDR, (UINT32)i4Scan) );
		Printf("Parsing	progressive	scan DQT and DHT --	done\n");

		/* set huff	table */
		for	(u4Count = 0; u4Count <	JPGHDR->rDHT.u4NumDcTbl; u4Count++)
		{
			if (JPGHDR->rDHT.fgDcTblLoaded & D_BIT(u4Count))
			{
				TRY2( VLDJPG_SetGlobDcHuffTbl(H_JPGHW, (UINT8)u4Count,
											 JPGHDR->rDHT.arDcTbl[u4Count].au1Bits,
											 JPGHDR->rDHT.arDcTbl[u4Count].au1HuffVal) );
			}
			if (JPGHDR->rDHT.fgAcTblLoaded & D_BIT(u4Count))
			{
				TRY2( VLDJPG_SetGlobAcHuffTbl(H_JPGHW, (UINT8)u4Count,
											 JPGHDR->rDHT.arAcTbl[u4Count].au1Bits,
											 JPGHDR->rDHT.arAcTbl[u4Count].au1HuffVal) );
			}
		}

		/* set q table */
		for	(u4Count = 0; u4Count <	(UINT32)JPGHDR->rDQT.u1NumQ; u4Count++)
		{
			TRY2( VLDJPG_SetGlobQtbl(H_JPGHW, (UINT8)u4Count, &JPGHDR->rDQT.aau1Qtbl[u4Count][0]) );
		}
	}

	/* set data	start */
	TRY2( VLDJPG_SetScanData(H_JPGHW, JPGHDR->arSOS[i4Scan].pu1ScanDataStart) );

	/* set comp	in scan	*/
	TRY2( VLDJPG_SetScanCompInScan(H_JPGHW, JPGHDR->arSOS[i4Scan].u1CompInScan,
								  (UINT8*)JPGHDR->arSOS[i4Scan].au1CompId) );

	/* associate q tbl to comp,	dc/ac id to	block */
	for	((u4Comp = 0), (u4Block = 0);
		u4Comp < (UINT32)JPGHDR->arSOS[i4Scan].u1CompInScan;
		u4Comp++)
	{
	    if (u4Comp >= (UINT32)3)
	    {
	        return -(INT32)E_JPG_ERR_UNSUPPORT;
	    }
		TRY2( VLDJPG_SetScanCompQTbl(H_JPGHW, (UINT8)u4Comp,	JPGHDR->rSOF.arSofComp[u4Comp].u1QuantTblNo) );
		for	(u4Count = 0; u4Count <	JPGMEMBS->au4CompBlock[u4Comp];	u4Count++, u4Block++)
		{
			TRY2( VLDJPG_SetScanBlkDc(H_JPGHW, (UINT8)u4Block, JPGHDR->arSOS[i4Scan].au1DcId[u4Comp]) );
			TRY2( VLDJPG_SetScanBlkAc(H_JPGHW, (UINT8)u4Block, JPGHDR->arSOS[i4Scan].au1AcId[u4Comp]) );
		}
	}

	/* set huff	table in progressive mode, baseline	mode should	set	in glob	*/
	/* FIXME: remove 'if' line,	and	the	huff/q setup in	glob */
	if (JPGHW->fgProgressive &&	i4Scan)
	{
		/* set huff	table */
		for	(u4Count = 0; u4Count <	JPGHDR->rDHT.u4NumDcTbl; u4Count++)
		{
			if (JPGHDR->rDHT.fgDcTblLoaded & D_BIT(u4Count))
			{
				TRY2( VLDJPG_SetGlobDcHuffTbl(H_JPGHW, (UINT8)u4Count,
											 JPGHDR->rDHT.arDcTbl[u4Count].au1Bits,
											 JPGHDR->rDHT.arDcTbl[u4Count].au1HuffVal) );
			}
			if (JPGHDR->rDHT.fgAcTblLoaded & D_BIT(u4Count))
			{
				TRY2( VLDJPG_SetGlobAcHuffTbl(H_JPGHW, (UINT8)u4Count,
											 JPGHDR->rDHT.arAcTbl[u4Count].au1Bits,
											 JPGHDR->rDHT.arAcTbl[u4Count].au1HuffVal) );
			}
		}
		/* set q table */
		for	(u4Count = 0; u4Count <	(UINT32)JPGHDR->rDQT.u1NumQ; u4Count++)
		{
			TRY2( VLDJPG_SetGlobQtbl(H_JPGHW, (UINT8)u4Count, &JPGHDR->rDQT.aau1Qtbl[u4Count][0]) );
		}
	}


	/* set al ah */
	TRY2( VLDJPG_SetScanAlAh(H_JPGHW, JPGHDR->arSOS[i4Scan].u1AhAl) );
	/* set se ss */
	TRY2( VLDJPG_SetScanSeSs(H_JPGHW, JPGHDR->arSOS[i4Scan].u1Se, JPGHDR->arSOS[i4Scan].u1Ss) );
	/* set progressive mode	: dc_first,	dc_refine, ac_first, ac_refine */

	if (JPGHW->fgProgressive)
	{
		if (JPGHDR->arSOS[i4Scan].u1AhAl & 0xf0)
		{
			u4ProgMode = (JPGHDR->arSOS[i4Scan].u1Ss) ?	(UINT32)D_JPEG_AC_REFINE : (UINT32)D_JPEG_DC_REFINE;
		}
		else
		{
			u4ProgMode = (JPGHDR->arSOS[i4Scan].u1Ss) ?	(UINT32)D_JPEG_AC_FIRST	: (UINT32)D_JPEG_DC_FIRST;
		}
	}
	else
	{
		u4ProgMode = D_JPEG_BASELINE;
#ifdef JPEG_VLD_V3		
		VLDJPG_UseNewEngine();
        VLDJPG_SetErrorConcealment();
        VLDJPG_SetErrorDetection();
#endif
	}
	TRY2( VLDJPG_SetProgressiveMode((UINT8)u4ProgMode) );
	JPGHW->u4ProgMode =	u4ProgMode;

	/* set last	scan flag */
	TRY2( VLDJPG_SetScanLast(H_JPGHW,
							(((UINT32)i4Scan	== JPGHDR->au4CompLastScan[0]) ||
							 ((UINT32)i4Scan	== JPGHDR->au4CompLastScan[1]) ||
							 ((UINT32)i4Scan	== JPGHDR->au4CompLastScan[2]))));
	//i4Scan ==	((INT32)JPGHDR->u4NumScan -	1)

	/* mtk patent */
	TRY2( VLDJPG_SetScanMcuRowStart(H_JPGHW,	0) );
	TRY2( VLDJPG_SetScanMcuRowEnd(H_JPGHW, JPGMEMBS->u4McuHeight	- 1) );

	/* set nz buffer */
	if (JPGHW->fgProgressive)
	{
		TRY2( VLDJPG_SetGlobNonZeroBuf(H_JPGHW, (UINT8*)JPGMEMBS->apvNzBuffer[JPGHW->u1FirstCompInScan])	);
		//TRY2( VLDJPG_SetProgressiveNoneZeroHistory( (UINT8*)JPGMEMBS->apvNzBuffer[0] )	);
	}

	/* validate	scan data */
	TRY2( VLDJPG_ValidateScanData(H_JPGHW) );

	JPGHW->i4SavedEOBRUN = 0;

	CATCH_ERROR()
	{
		return i4Ret;
	}
}

static INT32 _jpg_Misc(JPEGHANDLE hFlow)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;

	ASSERT(hFlow);

	TRY2( VLDJPG_SetMiscDefBlkAddr(H_JPGHW) );
	TRY2( VLDJPG_ValidateMiscData(H_JPGHW) );
	TRY2( VLDJPG_Switch2CleanBuffer() );

	CATCH_ERROR()
	{
		return i4Ret;
	}
}


static VOID _jpg_UpdateReadPtr(JPEGHANDLE hFlow)
{
    UINT32 u4NewRP;
    UINT32 u4UsedLen;
    			
    u4NewRP = IO_READ32(VLD0_BASE, 0xfc) - PHYSICAL((UINT32) JPGHDR->pu1RingBufStart);
    if (u4NewRP < JPGHDR->u4RingRP)
    {
        u4UsedLen = (JPGHDR->u4RingBufSize + u4NewRP) - JPGHDR->u4RingRP;
    }
    else
    {
        u4UsedLen = u4NewRP - JPGHDR->u4RingRP;
    }    			
    if(JPGHDR->u4RingData < u4UsedLen)
    {
        LOG(3, "BS Buffer data 0x%x less than used data 0x%x.\n", JPGHDR->u4RingData, u4UsedLen);
    }
    else
    {
        JPGHDR->u4RingData -= u4UsedLen;
    }			
	JPGFLW->u4DecodedSize+=u4UsedLen;
    JPGHDR->u4RingRP = u4NewRP;		
}

#ifdef JPEG_AUTO_TEST

#ifdef IMGRZ_AUTO_TEST
extern INT32 RzEmuGetParam(INT32 i4ParamType,INT32 i4Param);
#endif
static INT32 _jpg_Row(JPEGHANDLE hFlow,	UINT32 u4Row)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	
	ASSERT(hFlow);

	TRY2( VLDJPG_SetMcuRowCurr(H_JPGHW, u4Row) );
	//TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 0, pu1Y,	au4RowSize[0]) );
	//TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 1, pu1U,	au4RowSize[1]) );
	//TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 2, pu1V,	au4RowSize[2]) );
	TRY2( VLDJPG_ValidateMcuRowData(H_JPGHW)	);

	CATCH_ERROR()
	{
		return i4Ret;
	}
}
static INT32 _jpg_Row5391(JPEGHANDLE hFlow,	UINT32 u4RowIdx, 
    UINT32 u4MCUsPerRow, UINT32 u4TotalRows, UINT8 **apu1OutputAddr)
{
    JMEM_YUV_LINEAR_T *prMemYuv = NULL;

    JMEM_MW_T *prMemMw = NULL;
    JPEG_OUTPUT_T	 *prMem;
    JPEG_FLOW_DATA_T *prFlow;
	
    UINT32 au4RowSize[3];
	INT32 i4Ret	= (INT32)E_JPG_OK;
    
	ASSERT(hFlow);
	ASSERT(apu1OutputAddr != NULL);

    VERIFY(VLDJPG_SetCurrMcuRowNum5391(u4RowIdx) == (INT32)E_HWJPG_OK);

    prMemYuv = (JMEM_YUV_LINEAR_T *)(JPGMEM->pvLocalData);
    ASSERT(prMemYuv != NULL);

    prMemMw = (JMEM_MW_T *)(JPGMEM->pvLocalData);
    ASSERT(prMemMw != NULL);

    prFlow = (JPEG_FLOW_DATA_T *)hFlow;
    prMem = prFlow->hJpegMem;
    if ((u4RowIdx % 2) == 0)
    {
    	JPGMEM->pfnGetOutput(H_JPGMEM, (void**)apu1OutputAddr);
        if (prMem->rParent.eJpegClass == E_JC_MEM_YUV)
        {
    	au4RowSize[0] = prMemYuv->au4RowSize[0];
    	au4RowSize[1] = prMemYuv->au4RowSize[1];
    	au4RowSize[2] = prMemYuv->au4RowSize[2];
        }
        else
        {
            au4RowSize[0] = prMemMw->au4RowSize[0];
            au4RowSize[1] = prMemMw->au4RowSize[1];
            au4RowSize[2] = prMemMw->au4RowSize[2];
        }

    	TRY2( VLDJPG_SetMcuRowCurr(H_JPGHW, u4RowIdx) );
    	
    	TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 0, apu1OutputAddr[0], au4RowSize[0]) );
    	TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 1, apu1OutputAddr[1], au4RowSize[1]) );
    	TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 2, apu1OutputAddr[2], au4RowSize[2]) );

        // output bank1
    	TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 0, apu1OutputAddr[6], au4RowSize[0]) );
    	TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 1, apu1OutputAddr[7], au4RowSize[1]) );
    	TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 2, apu1OutputAddr[8], au4RowSize[2]) );
    	
    	TRY2( VLDJPG_ValidateMcuRowData5391(H_JPGHW, u4MCUsPerRow, u4TotalRows)	);
    }
    
	// decode
	i4Ret = VLDJPG_DecodeOneMcuRow5391(H_JPGHW, u4RowIdx);

	// update read pointer and data size of ring buffer
	if (JPGHDR->fgRingBuffer && (!JPGHDR->fgEOF))
	{
	    _jpg_UpdateReadPtr(hFlow);
        }

	CATCH_ERROR()
	{
		return i4Ret;
	}
}

static INT32 _jpg_RowRz5391(JPEGHANDLE hFlow,	UINT32 u4RowIdx, 
    UINT32 u4MCUsPerRow, UINT32 u4TotalRows, UINT8 **apu1OutputAddr)
{
    static UINT32 su4IsRzInited = FALSE;
    static UINT32 su4RzCurrYAddr, su4RzPrevYAddr;
    static UINT32 su4RzCurrCbAddr, su4RzPrevCbAddr;
    static UINT32 su4RzCurrCrAddr, su4RzPrevCrAddr;
    static UINT32 su4RzYSrcW, su4RzCbSrcW, su4RzCrSrcW;
    static UINT32 su4RzYSrcH, su4RzCbSrcH, su4RzCrSrcH;
    //static UINT32 su4RzYTgW, su4RzYTgH, su4RzCTgW, su4RzCTgH;
    static UINT32 su4RzYSrcL;
	UINT32 u4RzTagWidht=0,u4RzTagHeight=0;
	UINT32 u4BufferAddr=0;//u4BufferSize=0;
	UINT32 *pImgrzOffsetData=NULL;
    RZ_JPG_SCL_PARAM_SET_T rJpgParam;
    
	JMEM_YUV_LINEAR_T *prMemYuv = NULL;
	JPEG_VLDJPG_DATA_T *prJpgHw = NULL;
	
    UINT32 au4RowSize[3];
	INT32 i4Ret	= (INT32)E_JPG_OK;
    
	ASSERT(hFlow);
	ASSERT(apu1OutputAddr != NULL);

    VERIFY(VLDJPG_SetCurrMcuRowNum5391(u4RowIdx) == (INT32)E_HWJPG_OK);

    prMemYuv = (JMEM_YUV_LINEAR_T *)(JPGMEM->pvLocalData);
    ASSERT(prMemYuv != NULL);

    prJpgHw = ((JPEG_VLDJPG_DATA_T*)H_JPGHW);
    ASSERT(prJpgHw != NULL);

    if ((u4RowIdx % 2) == 0)
    {
    	JPGMEM->pfnGetOutput(H_JPGMEM, (void**)apu1OutputAddr);

    	au4RowSize[0] = prMemYuv->au4RowSize[0];
    	au4RowSize[1] = prMemYuv->au4RowSize[1];
    	au4RowSize[2] = prMemYuv->au4RowSize[2];

    	TRY2( VLDJPG_SetMcuRowCurr(H_JPGHW, u4RowIdx) );
    	
    	TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 0, apu1OutputAddr[0], au4RowSize[0]) );
    	TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 1, apu1OutputAddr[1], au4RowSize[1]) );
    	TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 2, apu1OutputAddr[2], au4RowSize[2]) );

        // output bank1
    	TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 0, apu1OutputAddr[6], au4RowSize[0]) );
    	TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 1, apu1OutputAddr[7], au4RowSize[1]) );
    	TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 2, apu1OutputAddr[8], au4RowSize[2]) );
    	
    	TRY2( VLDJPG_ValidateMcuRowData5391(H_JPGHW, u4MCUsPerRow, u4TotalRows)	);

        // for imgrz
        if (u4RowIdx == 0)
        {
            su4RzPrevYAddr  = (UINT32)apu1OutputAddr[0];
            su4RzPrevCbAddr = (UINT32)apu1OutputAddr[1];
            su4RzPrevCrAddr = (UINT32)apu1OutputAddr[2];
            if(_JpgVerifyCmdGetParam(21,0))
            {
				_rRZAcc.u4Seconds = 0;
				_rRZAcc.u4Micros  = 0;
            }
        }
        else
        {
            su4RzPrevYAddr  = su4RzCurrYAddr;
            su4RzPrevCbAddr = su4RzCurrCbAddr;
            su4RzPrevCrAddr = su4RzCurrCrAddr;
        }
        
        su4RzCurrYAddr  = (UINT32)apu1OutputAddr[0];
        su4RzCurrCbAddr = (UINT32)apu1OutputAddr[1];
        su4RzCurrCrAddr = (UINT32)apu1OutputAddr[2];
    }
    else
    {
        // for imgrz
        su4RzPrevYAddr  = su4RzCurrYAddr;
        su4RzPrevCbAddr = su4RzCurrCbAddr;
        su4RzPrevCrAddr = su4RzCurrCrAddr;

        su4RzCurrYAddr  = (UINT32)apu1OutputAddr[6];
        su4RzCurrCbAddr = (UINT32)apu1OutputAddr[7];
        su4RzCurrCrAddr = (UINT32)apu1OutputAddr[8];
    }
    
	// decode
	i4Ret = VLDJPG_DecodeOneMcuRow5391(H_JPGHW, u4RowIdx);

    su4RzYSrcL  = prJpgHw->u4CompPitchRow[0];
    su4RzYSrcW  = prMemYuv->au4RowWidth[0];
    su4RzCbSrcW = prMemYuv->au4RowWidth[1];
    su4RzCrSrcW = prMemYuv->au4RowWidth[2];
    su4RzYSrcH  = prMemYuv->au4RowHeight[0];
    su4RzCbSrcH = prMemYuv->au4RowHeight[1];
    su4RzCrSrcH = prMemYuv->au4RowHeight[2];
	_JDEC_SetRzV(&rJpgParam, prMemYuv->au4RowHeight);	

    LOG(3,"RZ prev Y  addr = 0x%08x \n", su4RzPrevYAddr);
    LOG(3,"RZ prev Cb addr = 0x%08x \n", su4RzPrevCbAddr);
    LOG(3,"RZ prev Cr addr = 0x%08x \n", su4RzPrevCrAddr);
    LOG(3,"RZ curr Y  addr = 0x%08x \n", su4RzCurrYAddr);
    LOG(3,"RZ curr Cb addr = 0x%08x \n", su4RzCurrCbAddr);
    LOG(3,"RZ curr Cr addr = 0x%08x \n", su4RzCurrCrAddr);

#ifdef IMGRZ_AUTO_TEST
		RzEmuGetParam(1,(INT32)(&pImgrzOffsetData));
		rJpgParam.u4YSrcHOffset = pImgrzOffsetData[0];
		rJpgParam.u4YSrcVOffset = pImgrzOffsetData[1];
		rJpgParam.u4CbSrcHOffset = pImgrzOffsetData[2];
		rJpgParam.u4CbSrcVOffset = pImgrzOffsetData[3];
		rJpgParam.u4CrSrcHOffset = pImgrzOffsetData[2];
		rJpgParam.u4CrSrcVOffset = pImgrzOffsetData[3];
	    rJpgParam.u4YTgHOffset = pImgrzOffsetData[4];;
        rJpgParam.u4YTgVOffset = pImgrzOffsetData[5];
#else
	   rJpgParam.u4YSrcHOffset = 0;
	   rJpgParam.u4YSrcVOffset = 0;
	   rJpgParam.u4CbSrcHOffset = 0;
	   rJpgParam.u4CbSrcVOffset = 0;
	   rJpgParam.u4CrSrcHOffset = 0;
	   rJpgParam.u4CrSrcVOffset = 0;
	   rJpgParam.u4YTgHOffset = 0;
	   rJpgParam.u4YTgVOffset = 0;
#endif
    rJpgParam.u4IsRsIn = 1;        // block|raster based input
    rJpgParam.u4YSrcBase1 = su4RzCurrYAddr;     // y1
    rJpgParam.u4YSrcBase2 = su4RzPrevYAddr;     // y2
    rJpgParam.u4YSrcBufLen = su4RzYSrcL;
    rJpgParam.u4YSrcW = su4RzYSrcW;
    rJpgParam.u4YSrcH = (su4RzYSrcH * u4TotalRows);
    rJpgParam.u4CbSrcBase1 = su4RzCurrCbAddr;    // cb1
    rJpgParam.u4CbSrcBase2 = su4RzPrevCbAddr;    // cb2
    rJpgParam.u4CbSrcW = su4RzCbSrcW;
    rJpgParam.u4CbSrcH = (su4RzCbSrcH * u4TotalRows);
    rJpgParam.u4CrSrcBase1 = su4RzCurrCrAddr;    // cr1
    rJpgParam.u4CrSrcBase2 = su4RzPrevCrAddr;    // cr2
    rJpgParam.u4CrSrcW = su4RzCrSrcW;
    rJpgParam.u4CrSrcH = (su4RzCrSrcH * u4TotalRows);

	if(_JpgVerifyCmdGetParam(14,0))
	{
	    rJpgParam.u4CbSrcW = rJpgParam.u4CrSrcW = rJpgParam.u4YSrcW;
        rJpgParam.u4CrSrcH = rJpgParam.u4CbSrcH = rJpgParam.u4YSrcH;
        rJpgParam.fgGray = 1;
	}
	else
	{
	   rJpgParam.fgGray = 0;
	}
	rJpgParam.u4ColorComp = 0x7;
    rJpgParam.fgPreload = 0;
    rJpgParam.u4IsJpgSameaddr = 0;

    u4RzTagWidht=_JPG_GetYTgW();
    u4RzTagHeight=_JPG_GetYTgH();

   if(_JpgVerifyCmdGetParam(10,0)==1)
   { 
   
     rJpgParam.u4IsRsOut = 1;	  // block|raster based output
     rJpgParam.u4OutSwap = 0;
     rJpgParam.u4IsVdo2Osd = 1;     // output in osd format ?
     rJpgParam.u4YTgCM=_JpgVerifyCmdGetParam(11,0);
     _JpgVerifyCmdGetParam(6,(INT32)(&u4BufferAddr));
     rJpgParam.u4YTgBase=u4BufferAddr;       // y
     rJpgParam.u4CTgBase = u4BufferAddr;//0x0e69c000; // cbcr
    
    if(rJpgParam.u4YTgCM==E_RZ_OSD_DIR_CM_AYCbCr8888)
    {
      rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444;
	  rJpgParam.u4YTgBufLen = u4RzTagWidht * 4;
	  rJpgParam.u4CTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
    }
	else
	{
        rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
		rJpgParam.u4YTgBufLen = u4RzTagWidht*2;
		rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset)>>1;
	}
	
	rJpgParam.u4YTgBufLen=(rJpgParam.u4YTgBufLen+0xf)&(~0xf);
    rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
    rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
    rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;

	{
      INT32 u4FilePrarm[4];
      u4FilePrarm[0]=1;
	  u4FilePrarm[1]=4;
	  u4FilePrarm[2]=rJpgParam.u4YTgBase;
	  u4FilePrarm[3]=rJpgParam.u4YTgBufLen * (rJpgParam.u4YTgH+rJpgParam.u4YTgVOffset);
	  _JpgVerifyCmdSetParam(2,(INT32)u4FilePrarm);
    }
 }
 else
 {
    rJpgParam.u4IsRsOut = _JpgVerifyCmdGetParam(12,0);	  // block|raster based output
    rJpgParam.u4OutSwap = _JpgVerifyCmdGetParam(17,0);
    rJpgParam.u4IsVdo2Osd = 0;	  // output in Y/C
	if(_JpgVerifyCmdGetParam(13,0)>=0)
	{
		 rJpgParam.u4OutMode=_JpgVerifyCmdGetParam(13,0);

		 if(rJpgParam.u4OutMode==E_RZ_VDO_OUTMD_444)
		 {
			 rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
			 rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
			 rJpgParam.u4CTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
			 rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
		 }
		 else if(rJpgParam.u4OutMode==E_RZ_VDO_OUTMD_422)
		 {
			 rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
			 rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
			 rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
			 rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
		 }
		 else if(rJpgParam.u4OutMode==E_RZ_VDO_OUTMD_420)
		 {
			 rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
			 rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
			 rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
			 rJpgParam.u4CTgH = (u4RzTagHeight-rJpgParam.u4YTgVOffset)>> 1;
		 }
	 }
	 else
	 {
		  if (prMemYuv->u4ColorFMT == E_JPEG_YUV420 || prMemYuv->u4ColorFMT == E_JPEG_YUV422V)
		  {
			 rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_420;
			 rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
			 rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
			 rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
			 rJpgParam.u4CTgH = (u4RzTagHeight-rJpgParam.u4YTgVOffset) >> 1;
		  }
		 else if ((prMemYuv->u4ColorFMT == E_JPEG_YUV422) || (prMemYuv->u4ColorFMT == E_JPEG_YUV444))
		 {
			rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
			rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
			rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
			rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
			rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
		 }
		 else
		 {
			Printf("Can NOT support this jpg fmt = %u\n", prMemYuv->u4ColorFMT);
			ASSERT(0);
		 }

	}
	rJpgParam.u4YTgCM = 0;
    rJpgParam.u4YTgBufLen = u4RzTagWidht;
    rJpgParam.u4YTgBufLen=(rJpgParam.u4YTgBufLen+0xf)&(~0xf);
	_JpgVerifyCmdGetParam(6,(INT32)(&u4BufferAddr));
    rJpgParam.u4YTgBase = u4BufferAddr;       // y

    _JpgVerifyCmdGetParam(7,(INT32)(&u4BufferAddr));
	 rJpgParam.u4CTgBase = u4BufferAddr; // cbcr   
	 
     {
       INT32 u4FileParam[7]; 
	   UINT32 u4CVData=0;
	   u4FileParam[0]=2;
	   u4FileParam[1]=0;
       u4FileParam[2]=rJpgParam.u4YTgBase;
       u4FileParam[3]=rJpgParam.u4YTgBufLen * (rJpgParam.u4YTgH+rJpgParam.u4YTgVOffset);
	   u4FileParam[4]=1;
       u4FileParam[5]=rJpgParam.u4CTgBase;
	   u4CVData= rJpgParam.u4CTgH+ (rJpgParam.u4OutMode ==E_RZ_VDO_OUTMD_422 ? rJpgParam.u4YTgVOffset : (rJpgParam.u4YTgVOffset >>1));;
       u4FileParam[6]=(rJpgParam.u4CTgW+(rJpgParam.u4YTgHOffset>>1)) *u4CVData*2;
       _JpgVerifyCmdSetParam(2,(INT32)u4FileParam);
     }
   }

   LOG(3,"RZ target Y addr = 0x%08x \n", rJpgParam.u4YTgBase);
   LOG(3,"RZ target C addr = 0x%08x \n", rJpgParam.u4CTgBase);
    
    // for imgrz
    if (u4RowIdx == 0)
    {
        rJpgParam.u4IsFstBl = 1;
        rJpgParam.u4IsLstBl = 0;
    }
    else if (u4RowIdx == (u4TotalRows - 1))
    {
        rJpgParam.u4IsFstBl = 0;
        rJpgParam.u4IsLstBl = 1;        
    }
    else
    {
        rJpgParam.u4IsFstBl = 0;
        rJpgParam.u4IsLstBl = 0;
    }

    rJpgParam.u4IsJpgRzOn = 0;    // diable jpg-rz hand-off
    rJpgParam.u4JpgTempAddr = 0;
#ifdef IMGRZ_AUTO_TEST
	if(RzEmuGetParam(6,0) && su4IsRzInited && !(u4RowIdx%10))
	{
	   Printf("_IMGRZ_Break\n");
       _IMGRZ_Break();    
       _IMGRZ_OFF();
	   _IMGRZ_Resume();
	}
#endif
    if (su4IsRzInited == FALSE)
    {
        _IMGRZ_ReInit();
        su4IsRzInited = TRUE;
    }
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_JPGMD);
    _IMGRZ_Scale((void *)(&rJpgParam));

	if(_JpgVerifyCmdGetParam(21,0))
	{
		HAL_GetTime(&_rRZStart);
	}
    _IMGRZ_Flush();
    _IMGRZ_Wait();

    if(_JpgVerifyCmdGetParam(21,0))
    {
	   HAL_GetTime(&_rRZEnd);
	   HAL_GetDeltaTime(&_rRZdif, &_rRZStart, &_rRZEnd);
	   _rRZAcc.u4Seconds += _rRZdif.u4Seconds;
	   _rRZAcc.u4Micros  += _rRZdif.u4Micros;
	   if (_rRZAcc.u4Micros >= 1000000)
	   {
		   _rRZAcc.u4Seconds += _rRZAcc.u4Micros / 1000000;
		   _rRZAcc.u4Micros  %= 1000000;
	   }
    }

    if (u4RowIdx == (u4TotalRows - 1))
    {
        su4IsRzInited = FALSE;
        VLDJPG_SetPowerOff();
		if(_JpgVerifyCmdGetParam(21,0))
		{
			LOG(0,"RZ %u.%06u\n",_rRZAcc.u4Seconds, _rRZAcc.u4Micros);		
		}
    }

	// update read pointer and data size of ring buffer
	if (JPGHDR->fgRingBuffer && (!JPGHDR->fgEOF))
	{
            _jpg_UpdateReadPtr(hFlow);
	}

	CATCH_ERROR()
	{
		return i4Ret;
	}
}

static INT32 _jpg_PicRz5391(JPEGHANDLE hFlow,	UINT32 u4RowIdx, 
    UINT32 u4MCUsPerRow, UINT32 u4TotalRows, UINT8 **apu1OutputAddr)
{
    static UINT32 su4IsRzInited = FALSE;
    static UINT32 su4RzCurrYAddr, su4RzPrevYAddr;
    static UINT32 su4RzCurrCbAddr, su4RzPrevCbAddr;
    static UINT32 su4RzCurrCrAddr, su4RzPrevCrAddr;
    static UINT32 su4RzYSrcW, su4RzCbSrcW, su4RzCrSrcW;
    static UINT32 su4RzYSrcH, su4RzCbSrcH, su4RzCrSrcH;
    //static UINT32 su4RzYTgW, su4RzYTgH, su4RzCTgW, su4RzCTgH;
    static UINT32 su4RzYSrcL;
	UINT32 u4BufferAddr=0;//,u4BufferSize=0;
	UINT32 *pImgrzOffsetData=NULL;
	UINT32 u4RzTagWidht=0,u4RzTagHeight=0;
    RZ_JPG_SCL_PARAM_SET_T rJpgParam;
    
	JMEM_YUV_LINEAR_T *prMemYuv = NULL;
	JPEG_VLDJPG_DATA_T *prJpgHw = NULL;
	
    UINT32 au4RowSize[3];
	INT32 i4Ret	= (INT32)E_JPG_OK;
    
	ASSERT(hFlow);
	ASSERT(apu1OutputAddr != NULL);

    VERIFY(VLDJPG_SetCurrMcuRowNum5391(u4RowIdx) == (INT32)E_HWJPG_OK);
    
    JPGMEM->pfnGetOutput(H_JPGMEM, (void**)apu1OutputAddr);

    prMemYuv = (JMEM_YUV_LINEAR_T *)(JPGMEM->pvLocalData);
    ASSERT(prMemYuv != NULL);
    au4RowSize[0] = prMemYuv->au4RowSize[0];
    au4RowSize[1] = prMemYuv->au4RowSize[1];
    au4RowSize[2] = prMemYuv->au4RowSize[2];

    prJpgHw = ((JPEG_VLDJPG_DATA_T*)H_JPGHW);
    ASSERT(prJpgHw != NULL);

    TRY2( VLDJPG_SetMcuRowCurr(H_JPGHW, u4RowIdx) );
    	
    TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 0, apu1OutputAddr[0], au4RowSize[0]) );
    TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 1, apu1OutputAddr[1], au4RowSize[1]) );
    TRY2( VLDJPG_SetMcuRowOutBuf(H_JPGHW, 2, apu1OutputAddr[2], au4RowSize[2]) );

    // output bank1
    TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 0, apu1OutputAddr[6], au4RowSize[0]) );
    TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 1, apu1OutputAddr[7], au4RowSize[1]) );
    TRY2( VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 2, apu1OutputAddr[8], au4RowSize[2]) );
    	
    TRY2( VLDJPG_ValidateMcuRowData5391(H_JPGHW, u4MCUsPerRow, u4TotalRows)	);

    // for imgrz
    su4RzCurrYAddr  = (UINT32)apu1OutputAddr[0];
    su4RzCurrCbAddr = (UINT32)apu1OutputAddr[1];
    su4RzCurrCrAddr = (UINT32)apu1OutputAddr[2];

    su4RzPrevYAddr  = (UINT32)apu1OutputAddr[6];
    su4RzPrevCbAddr = (UINT32)apu1OutputAddr[7];
    su4RzPrevCrAddr = (UINT32)apu1OutputAddr[8];

    su4RzYSrcL  = prJpgHw->u4CompPitchRow[0];
    su4RzYSrcW  = prMemYuv->au4RowWidth[0];
    su4RzCbSrcW = prMemYuv->au4RowWidth[1];
    su4RzCrSrcW = prMemYuv->au4RowWidth[2];
    su4RzYSrcH  = prMemYuv->au4RowHeight[0];
    su4RzCbSrcH = prMemYuv->au4RowHeight[1];
    su4RzCrSrcH = prMemYuv->au4RowHeight[2];
	_JDEC_SetRzV(&rJpgParam, prMemYuv->au4RowHeight);

    LOG(3,"RZ prev Y  addr = 0x%08x \n", su4RzPrevYAddr);
    LOG(3,"RZ prev Cb addr = 0x%08x \n", su4RzPrevCbAddr);
    LOG(3,"RZ prev Cr addr = 0x%08x \n", su4RzPrevCrAddr);
    LOG(3,"RZ curr Y  addr = 0x%08x \n", su4RzCurrYAddr);
    LOG(3,"RZ curr Cb addr = 0x%08x \n", su4RzCurrCbAddr);
    LOG(3,"RZ curr Cr addr = 0x%08x \n", su4RzCurrCrAddr);

#ifdef IMGRZ_AUTO_TEST
    RzEmuGetParam(1,(INT32)&pImgrzOffsetData);
    rJpgParam.u4YSrcHOffset = pImgrzOffsetData[0];
    rJpgParam.u4YSrcVOffset = pImgrzOffsetData[1];
    rJpgParam.u4CbSrcHOffset = pImgrzOffsetData[2];
    rJpgParam.u4CbSrcVOffset = pImgrzOffsetData[3];
    rJpgParam.u4CrSrcHOffset = pImgrzOffsetData[2];
    rJpgParam.u4CrSrcVOffset = pImgrzOffsetData[3];
    rJpgParam.u4YTgHOffset = pImgrzOffsetData[4];
    rJpgParam.u4YTgVOffset = pImgrzOffsetData[5];
#else
   rJpgParam.u4YSrcHOffset = 0;
   rJpgParam.u4YSrcVOffset = 0;
   rJpgParam.u4CbSrcHOffset = 0;
   rJpgParam.u4CbSrcVOffset = 0;
   rJpgParam.u4CrSrcHOffset = 0;
   rJpgParam.u4CrSrcVOffset = 0;
   rJpgParam.u4YTgHOffset = 0;
   rJpgParam.u4YTgVOffset =0;

#endif
    rJpgParam.u4IsRsIn = 1;        // block|raster based input
    rJpgParam.u4YSrcBase1 = su4RzCurrYAddr;     // y1
    rJpgParam.u4YSrcBase2 = su4RzPrevYAddr;     // y2
    rJpgParam.u4YSrcBufLen = su4RzYSrcL;
    rJpgParam.u4YSrcW = su4RzYSrcW;
    rJpgParam.u4YSrcH = (su4RzYSrcH * u4TotalRows);
    rJpgParam.u4CbSrcBase1 = su4RzCurrCbAddr;    // cb1
    rJpgParam.u4CbSrcBase2 = su4RzPrevCbAddr;    // cb2
    rJpgParam.u4CbSrcW = su4RzCbSrcW;
    rJpgParam.u4CbSrcH = (su4RzCbSrcH * u4TotalRows);
    rJpgParam.u4CrSrcBase1 = su4RzCurrCrAddr;    // cr1
    rJpgParam.u4CrSrcBase2 = su4RzPrevCrAddr;    // cr2
    rJpgParam.u4CrSrcW = su4RzCrSrcW;
    rJpgParam.u4CrSrcH = (su4RzCrSrcH * u4TotalRows);

    _JpgVerifyCmdSetParam(22,prMemYuv->u4ColorFMT);

	if(_JpgVerifyCmdGetParam(14,0))
	{
	    rJpgParam.u4CbSrcW = rJpgParam.u4CrSrcW = rJpgParam.u4YSrcW;
        rJpgParam.u4CrSrcH = rJpgParam.u4CbSrcH = rJpgParam.u4YSrcH;
        rJpgParam.fgGray = 1;
	}
	else
	{
	   rJpgParam.fgGray = 0;
	}
	
    rJpgParam.u4ColorComp = 0x7;
    rJpgParam.fgPreload = 0;
    rJpgParam.u4IsJpgSameaddr = 0;
	u4RzTagWidht=_JPG_GetYTgW();
	u4RzTagHeight=_JPG_GetYTgH();
   if(_JpgVerifyCmdGetParam(10,0)==1)
   {
   	   rJpgParam.u4IsRsOut =1;// _JpgVerifyCmdGetParam(12,0);	   // block|raster based output
	   rJpgParam.u4OutSwap =0;// _JpgVerifyCmdGetParam(17,0);
       rJpgParam.u4IsVdo2Osd = 1;     // output in osd format ?
	   rJpgParam.u4YTgCM=_JpgVerifyCmdGetParam(11,0);
       _JpgVerifyCmdGetParam(6,(INT32)(&u4BufferAddr));
       rJpgParam.u4YTgBase = u4BufferAddr;//0x0e19c000;       // y
       rJpgParam.u4CTgBase = u4BufferAddr;//0x0e69c000; // cbcr
    
       if(rJpgParam.u4YTgCM==E_RZ_OSD_DIR_CM_AYCbCr8888)
	   {
	      rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444;
	      rJpgParam.u4YTgBufLen = u4RzTagWidht * 4;
	      rJpgParam.u4CTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
	   }
	   else
	   {
	      rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
	      rJpgParam.u4YTgBufLen = u4RzTagWidht*2;
		  rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
	   }
	
	   rJpgParam.u4YTgBufLen=(rJpgParam.u4YTgBufLen+0xf)&(~0xf);
	   rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
	   rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
	   rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
       
	   {
          INT32 u4FileParam[4];
	      u4FileParam[0]=1;
	      u4FileParam[1]=4;
	      u4FileParam[2]=rJpgParam.u4YTgBase;
	      u4FileParam[3]=rJpgParam.u4YTgBufLen * (rJpgParam.u4YTgH+rJpgParam.u4YTgVOffset);
          _JpgVerifyCmdSetParam(2,(INT32)u4FileParam);
       }
   }
   else
   {

   	   rJpgParam.u4IsRsOut = _JpgVerifyCmdGetParam(12,0);	   // block|raster based output
	   rJpgParam.u4OutSwap = _JpgVerifyCmdGetParam(17,0);
       rJpgParam.u4IsVdo2Osd = 0;	  // output in osd format ?
	   if(_JpgVerifyCmdGetParam(13,0)>=0)
		{
			rJpgParam.u4OutMode=_JpgVerifyCmdGetParam(13,0);
			if(rJpgParam.u4OutMode==E_RZ_VDO_OUTMD_444)
			{
				rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
				rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
				rJpgParam.u4CTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
				rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
			}
			else if(rJpgParam.u4OutMode==E_RZ_VDO_OUTMD_422)
			{
				rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
				rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
				rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
				rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
			}
			else if(rJpgParam.u4OutMode==E_RZ_VDO_OUTMD_420)
			{
				rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
				rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
				rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
				rJpgParam.u4CTgH = (u4RzTagHeight-rJpgParam.u4YTgVOffset)>> 1;
			}
		}
		else
		{
             if (prMemYuv->u4ColorFMT == E_JPEG_YUV420 || prMemYuv->u4ColorFMT == E_JPEG_YUV422V)
             {
                rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_420;
                rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
                rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
                rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
                rJpgParam.u4CTgH = (u4RzTagHeight-rJpgParam.u4YTgVOffset) >> 1;
             }
             else if ((prMemYuv->u4ColorFMT == E_JPEG_YUV422) || (prMemYuv->u4ColorFMT == E_JPEG_YUV444))
             {
                rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
                rJpgParam.u4YTgW = u4RzTagWidht-rJpgParam.u4YTgHOffset;
                rJpgParam.u4YTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
                rJpgParam.u4CTgW = (u4RzTagWidht-rJpgParam.u4YTgHOffset) >> 1;
                rJpgParam.u4CTgH = u4RzTagHeight-rJpgParam.u4YTgVOffset;
             }
             else
             {
                Printf("Can NOT support this jpg fmt = %u\n", prMemYuv->u4ColorFMT);
                ASSERT(0);
             }
	   }
		
	   rJpgParam.u4YTgCM = 0;
	   rJpgParam.u4YTgBufLen =u4RzTagWidht;
	   rJpgParam.u4YTgBufLen=(rJpgParam.u4YTgBufLen+0xf)&(~0xf);
       _JpgVerifyCmdGetParam(6,(INT32)(&u4BufferAddr));
       rJpgParam.u4YTgBase = u4BufferAddr;       // y
       _JpgVerifyCmdGetParam(7,(INT32)(&u4BufferAddr));
       rJpgParam.u4CTgBase = u4BufferAddr; // cbcr
       
       {
          INT32 u4FileParam[7];
		  UINT32 u4CVData;
	      u4FileParam[0]=2;
	      u4FileParam[1]=0;
	      u4FileParam[2]=rJpgParam.u4YTgBase;
	      u4FileParam[3]=rJpgParam.u4YTgBufLen * (rJpgParam.u4YTgH+rJpgParam.u4YTgVOffset);
	
	      u4FileParam[4]=1;
	      u4FileParam[5]=rJpgParam.u4CTgBase;
		  u4CVData= rJpgParam.u4CTgH+ (rJpgParam.u4OutMode ==E_RZ_VDO_OUTMD_422 ? rJpgParam.u4YTgVOffset : (rJpgParam.u4YTgVOffset >>1));
	      u4FileParam[6]=(rJpgParam.u4CTgW+(rJpgParam.u4YTgHOffset>>1)) * u4CVData*2;
          _JpgVerifyCmdSetParam(2,(INT32)u4FileParam);      
       }
    }

    LOG(3,"RZ target Y addr = 0x%08x \n", rJpgParam.u4YTgBase);
    LOG(3,"RZ target C addr = 0x%08x \n", rJpgParam.u4CTgBase);
    
    // for imgrz
    if (u4RowIdx == 0)
    {
        rJpgParam.u4IsFstBl = 1;
        rJpgParam.u4IsLstBl = 0;        
    }

    rJpgParam.u4IsJpgRzOn = 1;    // enable jpg-rz hand-off
    rJpgParam.u4JpgTempAddr = 0;

    if (su4IsRzInited == FALSE)
    {
        _IMGRZ_ReInit();
        su4IsRzInited = TRUE;
    }
    _IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_JPGMD);
    _IMGRZ_Scale((void *)(&rJpgParam));
	
	if(_JpgVerifyCmdGetParam(21,0))
		HAL_GetTime(&_rRZStart);
    _IMGRZ_Flush();
	// decode
	i4Ret = VLDJPG_DecodeOnePic5391(H_JPGHW, u4RowIdx);

    _IMGRZ_Wait();  // wait imgrz resizer

   if(_JpgVerifyCmdGetParam(21,0))
   {
	  HAL_GetTime(&_rRZEnd);
	  HAL_GetDeltaTime(&_rRZdif, &_rRZStart, &_rRZEnd);
	  LOG(0, "RZ %u.%06us\n",_rRZdif.u4Seconds, _rRZdif.u4Micros);
   }

	// update read pointer and data size of ring buffer
	if (JPGHDR->fgRingBuffer && (!JPGHDR->fgEOF))
	{
            _jpg_UpdateReadPtr(hFlow);
	}
    su4IsRzInited = FALSE;
	VLDJPG_SetPowerOff();

	CATCH_ERROR()
	{
		return i4Ret;
	}
}
 
static INT32 _jpg_Mcu(JPEGHANDLE hFlow,	UINT8 **apu1OutputAddr)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;

	enum
	{
		E_COMP_Y, E_COMP_U,	E_COMP_V
	};
	enum
	{
		E_IDCT_Y, E_IDCT_U,	E_IDCT_V,
		E_COEF_Y, E_COEF_U,	E_COEF_V
	};
	
	ASSERT(hFlow);
	ASSERT(apu1OutputAddr != NULL);
	
	JPGMEM->pfnGetOutput(H_JPGMEM, (void**)apu1OutputAddr);

	if (JPGMEMBS->fgProgScan)
	{
		if (!((JPGHW->u4ProgMode ==	D_JPEG_AC_FIRST) &&
				(JPGHW->i4SavedEOBRUN) &&
				(JPGHW->fgLastScan)))
		{
			// set eobrun
			TRY2( VLDJPG_SetEndOfBandRun((UINT32)JPGHW->i4SavedEOBRUN) );

			// set coef	buffer
			TRY2( VLDJPG_SetMcuCoefBuf(H_JPGHW, E_COMP_Y, apu1OutputAddr[E_COEF_Y]) );
			TRY2( VLDJPG_SetMcuCoefBuf(H_JPGHW, E_COMP_U, apu1OutputAddr[E_COEF_U]) );
			TRY2( VLDJPG_SetMcuCoefBuf(H_JPGHW, E_COMP_V, apu1OutputAddr[E_COEF_V]) );
			if (JPGHW->fgLastScan)
			{
				// set idct	buffer
				TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_Y,	apu1OutputAddr[E_IDCT_Y]) );
				TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_U,	apu1OutputAddr[E_IDCT_U]) );
				TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_V,	apu1OutputAddr[E_IDCT_V]) );
			}
			else
			{
				// set idct	buffer
				TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_Y,	apu1OutputAddr[E_COEF_Y]) );
				TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_U,	apu1OutputAddr[E_COEF_U]) );
				TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_V,	apu1OutputAddr[E_COEF_V]) );
			}

			TRY2( VLDJPG_ValidateMcuData(H_JPGHW) );
			// decode
			TRY2( VLDJPG_DecodeOneMcu(H_JPGHW) );
			// get eobrun
			TRY2( VLDJPG_GetEndOfBandRun((UINT32*)&JPGHW->i4SavedEOBRUN)	);
		}
		else
		{
			Printf("AC_FIRST &&	EOBRUN && Last Scan\n");
			if ((JPGHW->u4ProgMode == D_JPEG_AC_FIRST) && (JPGHW->i4SavedEOBRUN))
			{
				JPGHW->i4SavedEOBRUN--;
				Printf("EOBRUN--\n");
			}
		}
	}
	else
	{
		// set idct	buffer only
		TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_Y,	apu1OutputAddr[E_IDCT_Y]) );
		TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_U,	apu1OutputAddr[E_IDCT_U]) );
		TRY2( VLDJPG_SetMcuOutBuf(H_JPGHW, E_COMP_V,	apu1OutputAddr[E_IDCT_V]) );
		// dup idct	buffer to coef buffer
		TRY2( VLDJPG_SetMcuCoefBuf(H_JPGHW, E_COMP_Y, apu1OutputAddr[E_IDCT_Y]) );
		TRY2( VLDJPG_SetMcuCoefBuf(H_JPGHW, E_COMP_U, apu1OutputAddr[E_IDCT_U]) );
		TRY2( VLDJPG_SetMcuCoefBuf(H_JPGHW, E_COMP_V, apu1OutputAddr[E_IDCT_V]) );

		TRY2( VLDJPG_ValidateMcuData(H_JPGHW) );

		// decode
		i4Ret = VLDJPG_DecodeOneMcu(H_JPGHW);
		// update read pointer and data size of ring buffer
		if (JPGHDR->fgRingBuffer && (!JPGHDR->fgEOF))
		{
			_jpg_UpdateReadPtr(hFlow);
		}
	}


	CATCH_ERROR()
	{
		return i4Ret;
	}
}

static INT32 _jpg_Flow(JPEGHANDLE hFlow)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	INT32 i4Scan, i4Row, i4Col;
	INT32 i4ScanTotal, i4Height, i4Width;
	BOOL fgLastScan	= FALSE;

    // comp	 0 1 2 
    // coef  3 4 5
    // comp1 6 7 8
	UINT8 *apu1OutputAddr[9];

	ASSERT(hFlow);
	
	TRY2( _jpg_Glob(hFlow) );

	// sync	ram	and	data
        //HalFlushInvalidateDCache();

	i4ScanTotal		= (INT32)JPGHDR->u4NumScan;
	i4Height		= (INT32)JPGMEMBS->u4McuHeight;
	i4Width			= (INT32)JPGMEMBS->u4McuWidth;

	for	(i4Scan	= 0; i4Scan	< i4ScanTotal; i4Scan++)
	{
		LOG(3,"\nscan %d\n", i4Scan);
		TRY2( _jpg_Scan(hFlow, i4Scan) );
		TRY2( _jpg_Misc(hFlow) );			// FIXME
		LOG(3,"cur	scan %d, total %d\n", i4Scan, i4ScanTotal);
		if (((UINT32)i4Scan == JPGHDR->au4CompLastScan[0]) ||
			((UINT32)i4Scan == JPGHDR->au4CompLastScan[1]) ||
			((UINT32)i4Scan == JPGHDR->au4CompLastScan[2]))
		{
			LOG(3,"last scan(%d) :	%d %d %d\n",
				   i4Scan,
				   JPGHDR->au4CompLastScan[0],
				   JPGHDR->au4CompLastScan[1],
				   JPGHDR->au4CompLastScan[2]);
			fgLastScan = TRUE;
		}
		else
		{
			fgLastScan = FALSE;
		}
		JPGHW->fgLastScan =	fgLastScan;

        if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU)
        {
      		for	(i4Row = 0;	i4Row <	i4Height; i4Row++)
       		{
       			TRY2( _jpg_Row(hFlow, (UINT32)i4Row)	);
       			for	(i4Col = 0;	i4Col <	i4Width; i4Col++)
       			{
       				apu1OutputAddr[3] = NULL;
       				apu1OutputAddr[4] = NULL;
       				apu1OutputAddr[5] = NULL;				
       				TRY2( _jpg_Mcu(hFlow, apu1OutputAddr) );
       				if ((i4Row == 0) && (i4Col	== 0))
       				{
       					LOG(3,"- idct(%08x:%08x:%08x) coef(%08x:%08x:%08x)\n",
        						   (UINT32)apu1OutputAddr[0], (UINT32)apu1OutputAddr[1],
        						   (UINT32)apu1OutputAddr[2], (UINT32)apu1OutputAddr[3],
        						   (UINT32)apu1OutputAddr[4], (UINT32)apu1OutputAddr[5]);
       				}

       				if ((i4Row == 0) && (i4Col == 0))
       				{
       					TRY2( VLDJPG_Switch2KeepBuffer()	);
       				}
                    JPGMEM->pfnMcuFin(H_JPGMEM);
                }
                JPGMEM->pfnRowFin(H_JPGMEM);
            }
    		JPGMEM->pfnScanFin(H_JPGMEM);
        }
        else if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW)
        {
      		for	(i4Row = 0;	i4Row <	i4Height; i4Row++)
       		{
       			apu1OutputAddr[3] = NULL;
       			apu1OutputAddr[4] = NULL;
    			apu1OutputAddr[5] = NULL;
       			TRY2( _jpg_Row5391(hFlow, (UINT32)i4Row, (UINT32)i4Width, (UINT32)i4Height, apu1OutputAddr)	);
                
    			if (i4Row == 0)
       			{
       				TRY2( VLDJPG_Switch2KeepBuffer()	);
       			}

                JPGMEM->pfnRowFin(H_JPGMEM);
            }
    		JPGMEM->pfnScanFin(H_JPGMEM);
        }
        else if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ)
        {
      		for	(i4Row = 0;	i4Row <	i4Height; i4Row++)
       		{
       			apu1OutputAddr[3] = NULL;
       			apu1OutputAddr[4] = NULL;
    			apu1OutputAddr[5] = NULL;
       			TRY2( _jpg_RowRz5391(hFlow, (UINT32)i4Row, (UINT32)i4Width, (UINT32)i4Height, apu1OutputAddr)	);
                
    			if (i4Row == 0)
       			{
       				TRY2( VLDJPG_Switch2KeepBuffer()	);
       			}

                JPGMEM->pfnRowFin(H_JPGMEM);
            }
    		JPGMEM->pfnScanFin(H_JPGMEM);
        }
        else if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ)
        {
            apu1OutputAddr[3] = NULL;
            apu1OutputAddr[4] = NULL;
            apu1OutputAddr[5] = NULL;

            i4Row = 0;
            TRY2( VLDJPG_Switch2KeepBuffer()	);
            
            TRY2( _jpg_PicRz5391(hFlow, (UINT32)i4Row, (UINT32)i4Width, (UINT32)i4Height, apu1OutputAddr)	);
        }
	}

	CATCH_ERROR()
	{
		return i4Ret;
	}
}

INT32 JDEC_DecodeYUV(JPEGHANDLE	hDec, void *pvImage, SIZE_T	zByteSize, JMEM_YUV_LINEAR_T **ppYuvImg)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	JPEG_BASE_CLASS_T	*prBase;
	JPEG_FLOW_DATA_T	*prFlow;
	JPEG_OUTPUT_T		*prMem;

	ASSERT(ppYuvImg);
	
	if (hDec ==	(JPEGHANDLE)NULL)
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}

	prBase = (JPEG_BASE_CLASS_T*)hDec;
	if ((prBase->eJpegClass != E_JC_HW_FLOW) ||
		(prBase->zObjectSize != sizeof(JPEG_FLOW_DATA_T)))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}

	prFlow = (JPEG_FLOW_DATA_T*)hDec;
	TRY2( JFIF_New(prFlow->hJpegHdr,	pvImage, zByteSize)	);		// parsing jpeg	header
	TRY2( VLDJPG_New(prFlow->hJpegHw) );							// create hardware instance
	TRY2( JMEM_New(prFlow->hJpegMem,	E_JC_MEM_YUV) );			// create linear yuv memory


	prMem		= (JPEG_OUTPUT_T*) prFlow->hJpegMem;
	// start jpeg decode
	TRY2( _jpg_Flow(hDec) );
	// set return value
	*ppYuvImg =	(JMEM_YUV_LINEAR_T*)prMem->pvLocalData;

	CATCH_ERROR()
	{
		return i4Ret;
	}
}
#endif
static INT32 _jpg_BaselineStart(JPEGHANDLE hFlow)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	INT32 i4Scan;

	ASSERT(hFlow);
	TRY2( _jpg_Glob(hFlow) );

	// sync	ram	and	data

#if defined(CC_MT5365) || defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(JPGHDR->pu1RingBufStart), (UINT32)(JPGHDR->u4RingBufSize));
#else
    HalFlushInvalidateDCache();
#endif
	// setup ring buffer
	if(JPGHDR->fgRingBuffer)
	{
		JPGHW->fgRingBuffer		= TRUE;
		JPGHW->u4RingBufferSize	= JPGHDR->u4RingBufSize;
		JPGHW->pu1RingBufStart = JPGHDR->pu1RingBufStart;
	}
	else
	{
		JPGHW->fgRingBuffer		= FALSE;
		JPGHW->u4RingBufferSize	= 0;
	}

	i4Scan = 0;
	TRY2( _jpg_Scan(hFlow, i4Scan) );
	TRY2( _jpg_Misc(hFlow) );			// FIXME
	JPGHW->fgLastScan =	TRUE;

	CATCH_ERROR()
	{
		return i4Ret;
	}
}

static INT32 _jpg_BaselineEnd(JPEGHANDLE hFlow)
{
	ASSERT(hFlow);
	
	JPGMEM->pfnScanFin(H_JPGMEM);
	return (INT32)E_JPG_OK;
}

static INT32 _jpg_BaselineRow(JPEGHANDLE hFlow,	UINT8 *apu1Addr[9])
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	INT32 i4CurRow = 0;
		
	ASSERT(hFlow);
	i4CurRow = JPGFLW->i4CurRow;

    TRY2(_jpg_RowRz5391Drv(hFlow, (UINT32)JPGFLW->i4CurRow, JPGMEMBS->u4McuWidth, JPGMEMBS->u4McuHeight, apu1Addr));
    if (i4CurRow == 0)
    {
         TRY2(VLDJPG_Switch2KeepBuffer());
    }
	
	JPGMEM->pfnRowFin(H_JPGMEM);
	JPGFLW->i4CurRow++;

	CATCH_ERROR()
	{
	   return i4Ret;
	}
}

///////////////////////////////////////////////////////////////////////////////

JPEGHANDLE JDEC_AllocInstance(void)
{
	// allocate	memory,	memset it to zero, set its type	to JDEC
	void *pvMem;
	JPEG_BASE_CLASS_T *prBase;

	pvMem =	x_mem_alloc(sizeof(JPEG_FLOW_DATA_T));
	if (pvMem == NULL)
	{
	      LOG(0, "JDEC mem_alloc failed\n");
		return pvMem;
	}

	x_memset(pvMem,	0, sizeof(JPEG_FLOW_DATA_T));
	prBase = (JPEG_BASE_CLASS_T*)pvMem;
	prBase->zObjectSize	= sizeof(JPEG_FLOW_DATA_T);
	prBase->eJpegClass = E_JC_HW_FLOW;

	return (JPEGHANDLE)pvMem;
}

INT32 JDEC_New(JPEGHANDLE hDec)
{
	// release all handle if exists
	// re-new all handle
	JPEG_BASE_CLASS_T *prBase;
	JPEG_FLOW_DATA_T *prFlow;

	if (hDec ==	(JPEGHANDLE)NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}

	prBase = (JPEG_BASE_CLASS_T*)hDec;
	if ((prBase->eJpegClass != E_JC_HW_FLOW) ||
		(prBase->zObjectSize	!= sizeof(JPEG_FLOW_DATA_T)))
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}

	prFlow = (JPEG_FLOW_DATA_T*)hDec;
	if (prFlow->hJpegHdr)
	{
		JFIF_FreeInstance(prFlow->hJpegHdr);
	}
	prFlow->hJpegHdr = JFIF_AllocInstance();
	if(prFlow->hJpegHdr == NULL)
	{
		return -(INT32)E_JPG_ERR_ALLOC_MEM;
	}

	if (prFlow->hJpegHw)
	{
		VLDJPG_FreeInstance(prFlow->hJpegHw);
	}
	prFlow->hJpegHw	= VLDJPG_AllocInstance();
	if(prFlow->hJpegHw == NULL)
	{
		return -(INT32)E_JPG_ERR_ALLOC_MEM;
	}

	if (prFlow->hJpegMem)
	{
		JMEM_FreeInstance(prFlow->hJpegMem);
	}
	prFlow->hJpegMem = JMEM_AllocInstance();
	if(prFlow->hJpegMem == NULL)
	{
		return -(INT32)E_JPG_ERR_ALLOC_MEM;
	}
	
	return (INT32)E_JPG_OK;
}

void JDEC_FreeInstance(JPEGHANDLE hDec)
{
	// free	related	handle
	// clean its contain
	// free	this handle
	JPEG_BASE_CLASS_T *prBase;
	JPEG_FLOW_DATA_T *prFlow;

	if (hDec ==	(JPEGHANDLE)NULL)
	{
		return;
	}

	prBase = (JPEG_BASE_CLASS_T*)hDec;
	if ((prBase->eJpegClass != E_JC_HW_FLOW) ||
		(prBase->zObjectSize != sizeof(JPEG_FLOW_DATA_T)))
	{
		return;
	}

	prFlow = (JPEG_FLOW_DATA_T*)hDec;
	if (prFlow->hJpegHdr)
	{
		JFIF_FreeInstance(prFlow->hJpegHdr);
		prFlow->hJpegHdr = (JPEGHANDLE)NULL;
	}

	if (prFlow->hJpegHw)
	{
		VLDJPG_FreeInstance(prFlow->hJpegHw);
		prFlow->hJpegHw = (JPEGHANDLE)NULL;
	}

	if (prFlow->hJpegMem)
	{
		JMEM_FreeInstance(prFlow->hJpegMem);
		prFlow->hJpegMem = (JPEGHANDLE)NULL;
	}

	x_mem_free((void*)hDec);
}


INT32 JDEC_MwGetMemLayout(JPEGHANDLE hDec, /*JMEM_MW_T*/void **pprMem)
{
	JPEG_BASE_CLASS_T	*prBase;
	JPEG_FLOW_DATA_T	*prFlow;
	JPEG_OUTPUT_T		*prMem;

	/* check return	pointer	*/
	if (pprMem == NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}
	/* check input handle */
	if (hDec ==	(JPEGHANDLE)NULL)
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}

	prBase = (JPEG_BASE_CLASS_T*)hDec;
	if ((prBase->eJpegClass != E_JC_HW_FLOW) ||
		(prBase->zObjectSize != sizeof(JPEG_FLOW_DATA_T)))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	prFlow = (JPEG_FLOW_DATA_T*)hDec;
	prMem =	(JPEG_OUTPUT_T*)prFlow->hJpegMem;
	/* validate	prMem */
	if ((prMem == NULL)	||
			//(prMem->rParent.eJpegClass	!= E_JC_MEM_MW)	||
			(prMem->rParent.zObjectSize	!= sizeof(JPEG_OUTPUT_T)))
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}
	/* return JMEM_MW_T	to caller */

	if(prMem->rParent.eJpegClass==E_JC_MEM_MW)
	{
	    *pprMem	= (JMEM_MW_T*)prMem->pvLocalData;
	}
      else
      	{
      	    *pprMem	= (JMEM_YUV_LINEAR_T*)prMem->pvLocalData;
      	}
	return (INT32)E_JPG_OK;
}

INT32 JDEC_MwSetRowBuffer(JPEGHANDLE hDec, void * const * ppvBuffer)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	JPEG_BASE_CLASS_T	*prBase;
	JPEG_FLOW_DATA_T	*prFlow;
	JPEG_OUTPUT_T		*prOut;

	void	*apvComp[9] = {0};

	/* check return	pointer	*/
	if (ppvBuffer == NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}
	/* check input handle */
	if (hDec ==	(JPEGHANDLE)NULL)
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	prBase = (JPEG_BASE_CLASS_T*)hDec;
	if ((prBase->eJpegClass		!= E_JC_HW_FLOW) ||
			(prBase->zObjectSize	!= sizeof(JPEG_FLOW_DATA_T)))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	prFlow	= (JPEG_FLOW_DATA_T*)hDec;
	prBase	= (JPEG_BASE_CLASS_T*)prFlow->hJpegMem;
	prOut	= (JPEG_OUTPUT_T*)prFlow->hJpegMem;
	if (prOut == NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}
	if (//(prBase->eJpegClass		!= E_JC_MEM_MW)	||
			(prBase->zObjectSize	!= sizeof(JPEG_OUTPUT_T)))
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}

	/* a weird way of setting output buffer	*/
	x_memset(apvComp, 0, sizeof(void*) * 9);
	apvComp[3] = ppvBuffer[0];
	apvComp[4] = ppvBuffer[1];
	apvComp[5] = ppvBuffer[2];
#if 1
       //for 5391 imgrz
	apvComp[6] = ppvBuffer[3];
	apvComp[7] = ppvBuffer[4];
	apvComp[8] = ppvBuffer[5];
#endif	
	prOut->pfnGetOutput((JPEGHANDLE)prOut, apvComp);
	prOut->au4McuOffset[0] = 0;
	prOut->au4McuOffset[1] = 0;
	prOut->au4McuOffset[2] = 0;

	return i4Ret;
}

INT32 JDEC_MwFillBuffer(JPEGHANDLE hDec, void **ppvBuf,	UINT32 *pu4Size, UINT32 u4AddSize)
{
	UINT32 u4RingWP;
	UINT32 u4RingSpace;

	JPEG_BASE_CLASS_T	*prBase;
	JPEG_FLOW_DATA_T	*prFlow;
	JPEG_JFIF_HEADER_T	*prJfif;

	/* check return	pointer	*/
	if ((ppvBuf	== NULL) ||	(pu4Size ==	NULL))
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}
	/* check input handle */
	if (hDec ==	(JPEGHANDLE)NULL)
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	prBase = (JPEG_BASE_CLASS_T*)hDec;
	if ((prBase->eJpegClass		!= E_JC_HW_FLOW) ||
			(prBase->zObjectSize	!= sizeof(JPEG_FLOW_DATA_T)))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	prFlow = (JPEG_FLOW_DATA_T*)hDec;
	prJfif = (JPEG_JFIF_HEADER_T*)prFlow->hJpegHdr;
	/* verify jfif header */
	prBase = (JPEG_BASE_CLASS_T*)prJfif;
	if ((prBase->eJpegClass		!= E_JC_JFIF_HEADER) ||
			(prBase->zObjectSize	!= sizeof(JPEG_JFIF_HEADER_T)))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	
	u4RingWP = prJfif->u4RingRP	+ prJfif->u4RingData + u4AddSize;
	u4RingSpace = prJfif->u4RingBufSize - (prJfif->u4RingData + u4AddSize);

	if (u4RingWP >= prJfif->u4RingBufSize)
	{
		u4RingWP -= prJfif->u4RingBufSize;
	}
	
	//Printf("RP %08x WP %08x Data %d Space %d\n", prJfif->u4RingRP, u4RingWP, prJfif->u4RingData, u4RingSpace);
	
	if (u4RingSpace <= 64)
	{
		//read pointer overrun at last row is acceptable
		//but should takecare if happens when stream buffer is too
		//small for one single row
		*pu4Size = 0;
		*ppvBuf = (void*)prJfif->pu1RingBufStart;
		return (INT32)E_JPG_OK;
	}
	
	u4RingSpace -= 64;

	if ((u4RingWP + u4RingSpace) < prJfif->u4RingBufSize)
	{
		*pu4Size = u4RingSpace;
	}
	else
	{
		*pu4Size = prJfif->u4RingBufSize - u4RingWP;
	}
	
	*ppvBuf = (void*)((UINT32)prJfif->pu1RingBufStart + u4RingWP);

	/*
	if (*pu4Size >= 64)
	{
		*pu4Size -= 64;
	}
	*/

	return (INT32)E_JPG_OK;
}

INT32 JDEC_MwGetJfif(JPEGHANDLE	hDec, JPEGHANDLE *phJfif)
{
	JPEG_BASE_CLASS_T	*prBase;
	JPEG_FLOW_DATA_T	*prFlow;

	/* check return	pointer	*/
	if (phJfif == NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}
	/* check input handle */
	if (hDec ==	(JPEGHANDLE)NULL)
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}

	prBase = (JPEG_BASE_CLASS_T*)hDec;
	if ((prBase->eJpegClass		!= E_JC_HW_FLOW) ||
			(prBase->zObjectSize	!= sizeof(JPEG_FLOW_DATA_T)))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	prFlow = (JPEG_FLOW_DATA_T*)hDec;
	*phJfif	= prFlow->hJpegHdr;

	return (INT32)E_JPG_OK;
}

VOID JDEC_SetDecodeMode(ENUM_JPEG_DEC_MODE_T eMode, ENUM_IMG_BUFFER_TYPE eMemClass)
{
    _u4JPGMode = (UINT32)eMode;
    _eJPGMemClass = eMemClass;
    JPG_SetDecMode5391((INT32)eMode);
}


INT32 JDEC_MwDecodeSetup(JPEGHANDLE hFlow, ENUM_JPEG_CLASS_T eMemClass)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;
	
	ASSERT(hFlow);

	TRY2( VLDJPG_New(JPGFLW->hJpegHw) );
	TRY2( JMEM_New(JPGFLW->hJpegMem,	eMemClass));//E_JC_MEM_YUV, E_JC_MEM_MW
	if(eMemClass == E_JC_MEM_MW)
	{
            if(JPGHDR->fgParsed)
            {
	          TRY2( _jpg_Glob(hFlow) );
            }
	}
	CATCH_ERROR()
	{
		return i4Ret;
	}
}

void JDEC_MwSetRingBuf(JPEGHANDLE hFlow, UINT8 *pBs_start, UINT32 bufferSize)
{  
      JPEG_FLOW_DATA_T *prFlow;
      JPEG_JFIF_HEADER_T *prJfif;

      ASSERT(hFlow);
      prFlow = (JPEG_FLOW_DATA_T*)hFlow;
      prJfif = prFlow->hJpegHdr;
      
	prJfif->fgRingBuffer			= TRUE;
	prJfif->pu1RingBufStart			= (UINT8*)(((UINT32)pBs_start + 63) & (~63));
	prJfif->u4RingBufSize			= (bufferSize- ((UINT32)prJfif->pu1RingBufStart - (UINT32)pBs_start)) & (~63);
	prJfif->u4RingRP				= 0;
	prJfif->u4RingData				= 0;
	if (_eJPGMemClass != (UINT32)EXTERNAL_RING_IN_DOUBLE_OUT)
	{
        x_memset((VOID *)prJfif->pu1RingBufStart, 0x0, prJfif->u4RingBufSize);
    }
}

void JDEC_FillBsBuf_End(JPEGHANDLE hFlow, UINT32 entry1_size, UINT32 entry2_size, BOOL beof)
{
      JPEG_FLOW_DATA_T *prFlow;
      JPEG_JFIF_HEADER_T *prJfif;
	UINT32 u4VldWptrOffset;
	CRIT_STATE_T crit;

      ASSERT(hFlow);
      prFlow = (JPEG_FLOW_DATA_T*)hFlow;
      prJfif = prFlow->hJpegHdr;
      
      // update ringbuffer data length -----------------------------------
      crit = x_crit_start();	//protect u4RingData
      if (entry1_size > 0)
      {
          prJfif->u4RingData += entry1_size;
      }
      if (entry2_size >0)
      {
          prJfif->u4RingData += entry2_size;
      }

	prJfif->fgEOF = beof;
	if(beof)
	{
		VLDJPG_SetRiscCtrlWP(0xFFFFFFFF);
	}
	else
	{
		u4VldWptrOffset = prJfif->u4RingRP + prJfif->u4RingData;
		u4VldWptrOffset = (u4VldWptrOffset >= prJfif->u4RingBufSize) ? (u4VldWptrOffset - prJfif->u4RingBufSize) : u4VldWptrOffset;
	
      		VLDJPG_SetRiscCtrlWP(PHYSICAL((UINT32)prJfif->pu1RingBufStart) + u4VldWptrOffset);
	}
	x_crit_end(crit);

    if(prJfif->u4RingData > prJfif->u4RingBufSize)
    {
      		LOG(7, "JDEC_FillBsBuf_End error occur\n");
    }
      //sync memory/cache
#if defined(CC_MT5365) || defined(CC_MT5395)
     HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(prJfif->pu1RingBufStart), (UINT32)(prJfif->u4RingBufSize));
 #else
     HalFlushInvalidateDCache();
 #endif
}

INT32 JDEC_MwHeaderParse(JPEGHANDLE hFlow, UINT8 *pBs_start, UINT32 bufferSize, JPEG_HEADER_INFO_T *jpgHdrInfo)
{    
	INT32 i4Ret = (INT32)E_JPG_OK, i4Idx;
      JPEG_FLOW_DATA_T *prFlow;
      JPEG_OUTPUT_T* prMem;
      JMEM_BASE_T* prMemBs;
      JMEM_MW_T* prMemMw;
      JMEM_YUV_LINEAR_T* prMemLinear;
      JPEG_JFIF_HEADER_T *prJfif;

	ASSERT(hFlow);
      ASSERT(jpgHdrInfo);

      prFlow = (JPEG_FLOW_DATA_T*)hFlow;
      prMem = prFlow->hJpegMem;
      prMemMw = prMem->pvLocalData;
      prMemLinear = prMem->pvLocalData;
      prMemBs = prMem->pvLocalData;
      prJfif = prFlow->hJpegHdr;
      
	//check buffer fillness, buffersize will depend on buffer fullness
	if(pBs_start)
	{
	    if (_u4JPGMode == (UINT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ)
	    {
                bufferSize = (UINT32)(prJfif->u4RingBufSize - (pBs_start - prJfif->pu1RingBufStart));
	    }
          TRY2( JFIF_New(prFlow->hJpegHdr, pBs_start, bufferSize));  // parsing jpeg header
	}
	else
	{
          TRY2( JFIF_New(prFlow->hJpegHdr, prJfif->pu1RingBufStart, prJfif->u4RingBufSize));  // parsing jpeg header
	}
	TRY2( _jpg_BaselineStart(hFlow));

      //orig image information
	jpgHdrInfo->u4Width = prMemBs->u4Width;
	jpgHdrInfo->u4Height = prMemBs->u4Height;
	jpgHdrInfo->u4TotalRow = prMemBs->u4McuHeight;
	jpgHdrInfo->u4TotalCol = prMemBs->u4McuWidth;
	
	jpgHdrInfo->u4NumScan = JPGHDR->u4NumScan;

      //color fmt information
      if(prMem->rParent.eJpegClass==E_JC_MEM_MW)
      {
          jpgHdrInfo->u4ColorFMT = prMemMw->u4ColorFMT;
      }
      else
      	{
          jpgHdrInfo->u4ColorFMT = prMemLinear->u4ColorFMT;
      	}
      //display row information
	for(i4Idx = 0; i4Idx<3; i4Idx++)
	{
	    if(prMem->rParent.eJpegClass==E_JC_MEM_MW)
	    {
	        jpgHdrInfo->au4RowWidth[i4Idx]= prMemMw->au4RowWidth[i4Idx];
	        jpgHdrInfo->au4RowHeight[i4Idx]= prMemMw->au4RowHeight[i4Idx];	        
	    }
          else
          {
	        jpgHdrInfo->au4RowWidth[i4Idx]= prMemLinear->au4RowWidth[i4Idx];
	        jpgHdrInfo->au4RowHeight[i4Idx]= prMemLinear->au4RowHeight[i4Idx];
          }

          if (jpgHdrInfo->au4RowWidth[i4Idx] & D_JPEG_DRAM_ALIGN_MASK)
          {
              // expand pitch to fit into dram aligned line
              jpgHdrInfo->au4RowWidth[i4Idx] &= (~D_JPEG_DRAM_ALIGN_MASK);
              jpgHdrInfo->au4RowWidth[i4Idx] += D_JPEG_DRAM_ALIGN_SIZE;
          }
          jpgHdrInfo->au4RowSize[i4Idx]= jpgHdrInfo->au4RowWidth[i4Idx]*jpgHdrInfo->au4RowHeight[i4Idx];
	}
	return i4Ret;
	CATCH_ERROR()
	{
		return i4Ret;
	}	
}

INT32 JDEC_MwRowStart(JPEGHANDLE hFlow)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;

	ASSERT(hFlow);
	
	if (JPGFLW->i4CurRow ==	0)
	{
		i4Ret =	_jpg_BaselineStart(hFlow);
	}

	return i4Ret;
}

INT32 JDEC_MwDecodeEnd(JPEGHANDLE hDec)
{
	INT32 i4Ret	= (INT32)E_JPG_OK;

	ASSERT(hDec);
	TRY2( _jpg_BaselineEnd(hDec) );
	JDEC_FreeInstance(hDec);
	return (INT32)E_JPG_OK;

	CATCH_ERROR()
	{
		return i4Ret;
	}
}

INT32 JDEC_MwDecodeRow(JPEGHANDLE hDec)
{
	INT32 i4Ret;
	UINT8 *apu1Addr[9];

	x_memset(apu1Addr, 0, 9	* sizeof(UINT8*));	
	i4Ret =	_jpg_BaselineRow(hDec, apu1Addr);

	if (i4Ret)
	{
		return i4Ret;//(INT32)E_JPG_ERR_GENERAL;
	}

	return (INT32)E_JPG_OK;
}

void JDEC_SetFileSize(JPEGHANDLE hFlow,UINT32 u4FileSize)
{
	JPEG_FLOW_DATA_T *prFlow;
	prFlow = (JPEG_FLOW_DATA_T*)hFlow;
    prFlow->u4FileSize=u4FileSize;
	return ;
}
UINT32 JDEC_GetProgress(JPEGHANDLE hFlow)
{
    UINT32 u4Progress;
    
    if (JPGHDR->rSOF.eJpegFormat == E_JPG_PROGRESSIVE_HUFFMAN)
    {
        u4Progress = (JPGHDR->u4CurrScan * 100) / 10;
    }
    else
    {
		if(_u4JPGMode== E_JPG_DEC_MODE_BASELINE_PIC_RZ)
		{
			u4Progress=(JPGFLW->u4FileSize) ? ((JPGFLW->u4DecodedSize)*100)/JPGFLW->u4FileSize : 0;
		}
		else
		{
			u4Progress = (JPGMEMBS->u4McuHeight) ? (((UINT32)JPGFLW->i4CurRow) * 100)/JPGMEMBS->u4McuHeight : 0;
		}
    }
    return u4Progress;
}

//for verification only
void *IMG_OSD_create(UINT32 dispColorMode)
{      
      DRV_OSD_T *prOsd=NULL;

      prOsd= x_mem_alloc(sizeof(DRV_OSD_T));
	  if(!prOsd)
	  {
	  	x_mem_free(prOsd);
	  	return NULL;
	  }
      x_memset(prOsd, 0, sizeof(DRV_OSD_T));
      prOsd->u4Left = 0;
      prOsd->u4Top = 0;
      // u4Width * bytes-per-pixel MUST align with 16 bytes
      prOsd->u4Width = 1360;//1366;
      prOsd->u4Height = 768;
      prOsd->u4ScaleWidth = prOsd->u4Width;
      prOsd->u4ScaleHeight = prOsd->u4Height;
      // pu1FrameBaseAddr MUST align with 16 bytes
      prOsd->pu1FrameBaseAddr = (UINT8 *)BSP_AllocAlignedDmaMemory((prOsd->u4Width*prOsd->u4Height)<<2, 16);
      if (prOsd->pu1FrameBaseAddr == NULL)
      {
		//LOG(0, "Can not allocate memory of size %d bytes for OSD\n", (prOsd->u4Width*prOsd->u4Height)<<2);
		LOG(0, "Can not allocate memory of size ## bytes for OSD\n");
		x_mem_free(prOsd);
		return NULL;
      }

      if(0)//((dispColorMode==(UINT32)OSD_CM_ARGB8888_DIRECT32)||(dispColorMode==(UINT32)OSD_CM_ARGB4444_DIRECT16))
      {
          x_memset(prOsd->pu1FrameBaseAddr, 0, (prOsd->u4Width*prOsd->u4Height)<<u4BytePerPixel[dispColorMode]);
      }
      else //yuv
      {
          x_memset(prOsd->pu1FrameBaseAddr, 0x80, (prOsd->u4Width*prOsd->u4Height)<<u4BytePerPixel[dispColorMode]);      
      }
      #if 0
      TRY2(OSD_Reset());
      rColorMode = (OSD_COLOR_MODE_T)dispColorMode;//OSD_CM_ARGB8888_DIRECT32;
      LINT_SUPPRESS_NEXT_STATEMENT(717)
      OSD_GET_PITCH_SIZE(rColorMode, prOsd->u4Width, u4PitchSize);
      TRY2(OSD_RGN_Create(&u4Region, prOsd->u4Width, prOsd->u4Height,
                               (void*)prOsd->pu1FrameBaseAddr,
                               dispColorMode, u4PitchSize,
                               prOsd->u4Left, prOsd->u4Top,
                               prOsd->u4Width, prOsd->u4Height));

      TRY2(OSD_RGN_LIST_Create(&u4List));
      TRY2(OSD_RGN_Insert(u4Region, u4List));
      TRY2(OSD_PLA_FlipTo((UINT32)OSD_PLANE_2, u4List));

      // luis@2007/5/21, unecessary in 5381
      //TRY2(OSD_SC_SetFormat32Bpp(OSD_PLANE_TO_SCALER((INT32)OSD_PLANE_2)));
      //TRY2(OSD_SC_SetSramConfiguration(2));
      TRY2(OSD_SC_Scale((INT32)OSD_SCALER_2, TRUE,
                            prOsd->u4Width, prOsd->u4Height,
                             prOsd->u4ScaleWidth, prOsd->u4ScaleHeight));

      TRY2(OSD_PLA_Enable((UINT32)OSD_PLANE_2, TRUE));
      UNUSED(PMX_SetEnable(0, 1));   
      #endif
      return (void *)prOsd;
      //CATCH_ERROR() 
      //{
        //  return NULL;
      //}      
}


static VOID _JDEC_SetRzV(RZ_JPG_SCL_PARAM_SET_T *prJpgParam, UINT32 *pu4RowHeight)
{
    if (pu4RowHeight[0] == 8)
    {
        prJpgParam->u4JpgVfacY = 0;
    }
    else if (pu4RowHeight[0] == 16)
    {
        prJpgParam->u4JpgVfacY = 1;
    }
    else if (pu4RowHeight[0] == 24)
    {
        prJpgParam->u4JpgVfacY = 2;
    }
    else if (pu4RowHeight[0] == 32)
    {
        prJpgParam->u4JpgVfacY = 3;
    }
    else
    {
        LOG(0, "Can NOT support this Y height = %u\n", pu4RowHeight[0]);
        ASSERT(0);
    }

    if (pu4RowHeight[1] == 8)
    {
        prJpgParam->u4JpgVfacCb = 0;
    }
    else if (pu4RowHeight[1] == 16)
    {
        prJpgParam->u4JpgVfacCb = 1;
    }
    else if (pu4RowHeight[1] == 24)
    {
        prJpgParam->u4JpgVfacCb = 2;
    }
    else if (pu4RowHeight[1] == 32)
    {
        prJpgParam->u4JpgVfacCb = 3;
    }
    else
    {
        prJpgParam->u4JpgVfacCb = prJpgParam->u4JpgVfacY;
        //LOG(0, "Can NOT support this Cb height = %u\n", pu4RowHeight[1]);
        //ASSERT(0);
    }
    
    if (pu4RowHeight[2] == 8)
    {
        prJpgParam->u4JpgVfacCr = 0;
    }
    else if (pu4RowHeight[2] == 16)
    {
        prJpgParam->u4JpgVfacCr = 1;
    }
    else if (pu4RowHeight[2] == 24)
    {
        prJpgParam->u4JpgVfacCr = 2;
    }
    else if (pu4RowHeight[2] == 32)
    {
        prJpgParam->u4JpgVfacCr = 3;
    }
    else
    {
        prJpgParam->u4JpgVfacCr = prJpgParam->u4JpgVfacY;
        //LOG(0, "Can NOT support this Cr height = %u\n", pu4RowHeight[2]);
        //ASSERT(0);
    }
}


VOID JDEC_MwSaveRzInfo(IMG_LIB_INFO_T *prImg, ENUM_IMG_ROTATE_PHASE eRotateDegree)
{
    prJpgImg = prImg;
    eJpgRotateDegree = eRotateDegree;
    _u4JpgRzTempBuf = prImg->rMem.u4RzTempBuf;
}
VOID JDEC_SetStop(BOOL fgJdecStop)
{
    _fgJpgFlw2Stop = fgJdecStop;
}

#if 1
INT32 JDEC_RzRow(UINT32 u4OutAddrY, UINT32 u4OutAddrCb, UINT32 u4OutAddrCr, UINT32 u4RowIdx,
                UINT32 u4TotalRows, UINT32 u4Comp, BOOL fgPreload)
{
    RZ_JPG_SCL_PARAM_SET_T rJpgParam;
    IMG_LIB_INFO_T *prImg = prJpgImg;
    JPEGHANDLE hFlow = prJpgImg->hCurDec;
    IMG_LIB_DISP_INFO_T *prDispInfo = &prJpgImg->rDispInfo;
    IMG_LIB_MEM_LAYOUT_T *prImgLibMem = &prJpgImg->rMem;
    JMEM_MW_T *prMemMw = NULL;
    JMEM_YUV_LINEAR_T *prMemYuv = NULL;
    JPEG_VLDJPG_DATA_T *prJpgHw = NULL;
    ENUM_IMG_ROTATE_PHASE eRotateDegree = eJpgRotateDegree;
    E_RZ_INOUT_TYPE_T ePreImgrzType;
    static UINT32 su4IsRzInited;
    static UINT32 su4RzCurrYAddr, su4RzPrevYAddr;
    static UINT32 su4RzCurrCbAddr, su4RzPrevCbAddr;
    static UINT32 su4RzCurrCrAddr, su4RzPrevCrAddr;
    static UINT32 su4RzYSrcW, su4RzCbSrcW, su4RzCrSrcW;
    static UINT32 su4RzYSrcH, su4RzCbSrcH, su4RzCrSrcH;
    static UINT32 su4RzYSrcL;
    UINT32 u4DispW, u4DispH;
    INT32 i4Ret = (INT32)E_JPG_OK;
    
    x_memset((void *)&rJpgParam, 0, sizeof(RZ_JPG_SCL_PARAM_SET_T));
    
    if (!u4RowIdx)
    {
        su4IsRzInited = FALSE;
    }
    
    u4DispW = prDispInfo->u4ScaleW;
    u4DispH = prDispInfo->u4ScaleH;

    if (prJpgImg->u1MemClass < RING_IN_SINGLE_OUT)
    {
        prMemYuv = (JMEM_YUV_LINEAR_T *)(JPGMEM->pvLocalData);
        ASSERT(prMemYuv != NULL);
        su4RzYSrcW  = prMemYuv->au4RowWidth[0];
        su4RzCbSrcW = prMemYuv->au4RowWidth[1];
        su4RzCrSrcW = prMemYuv->au4RowWidth[2];
        su4RzYSrcH  = prMemYuv->au4RowHeight[0];
        su4RzCbSrcH = prMemYuv->au4RowHeight[1];
        su4RzCrSrcH = prMemYuv->au4RowHeight[2];
        _JDEC_SetRzV(&rJpgParam, prMemYuv->au4RowHeight);   

    }
    else
    {    
        prMemMw = (JMEM_MW_T *)(JPGMEM->pvLocalData);
        ASSERT(prMemMw != NULL);
        su4RzYSrcW  = prMemMw->au4RowWidth[0];
        su4RzCbSrcW = prMemMw->au4RowWidth[1];
        su4RzCrSrcW = prMemMw->au4RowWidth[2];
        su4RzYSrcH  = prMemMw->au4RowHeight[0];
        su4RzCbSrcH = prMemMw->au4RowHeight[1];
        su4RzCrSrcH = prMemMw->au4RowHeight[2];
        if (u4Comp != 0x7)
        {
            //su4RzYSrcH = su4RzCbSrcH =su4RzCrSrcH = 8;
            rJpgParam.u4JpgVfacY = rJpgParam.u4JpgVfacCb = rJpgParam.u4JpgVfacCr = 0;
        }
        else
        {
            _JDEC_SetRzV(&rJpgParam, prMemMw->au4RowHeight);
        }
    }

    prJpgHw = ((JPEG_VLDJPG_DATA_T*)H_JPGHW);
    ASSERT(prJpgHw != NULL);

    su4RzYSrcL  = prJpgHw->u4CompPitchRow[0];

    if (u4RowIdx == 0)
    {
        su4RzPrevYAddr  = u4OutAddrY;
        su4RzPrevCbAddr = u4OutAddrCb;
        su4RzPrevCrAddr = u4OutAddrCr;
    }
    else
    {
        su4RzPrevYAddr  = su4RzCurrYAddr;
        su4RzPrevCbAddr = su4RzCurrCbAddr;
        su4RzPrevCrAddr = su4RzCurrCrAddr;
    }
    su4RzCurrYAddr  = u4OutAddrY;
    su4RzCurrCbAddr = u4OutAddrCb;
    su4RzCurrCrAddr = u4OutAddrCr;
    
    
    rJpgParam.u4IsRsIn = 1;        // block|raster based input
    rJpgParam.u4YSrcBase1 = su4RzCurrYAddr;     // y1
    rJpgParam.u4YSrcBase2 = su4RzPrevYAddr;     // y2
    rJpgParam.u4YSrcBufLen = su4RzYSrcL;
    rJpgParam.u4YSrcHOffset = 0;
    rJpgParam.u4YSrcVOffset = 0;
    rJpgParam.u4YSrcW = prImg->rHdrInfo.u4Width;
    rJpgParam.u4YSrcH = prImg->rHdrInfo.u4Height;
    rJpgParam.u4CbSrcBase1 = su4RzCurrCbAddr;    // cb1
    rJpgParam.u4CbSrcBase2 = su4RzPrevCbAddr;    // cb2
    rJpgParam.u4CbSrcHOffset = 0;
    rJpgParam.u4CbSrcVOffset = 0;
    rJpgParam.u4CbSrcW = (su4RzYSrcW == su4RzCbSrcW)?prImg->rHdrInfo.u4Width:(prImg->rHdrInfo.u4Width / 2);
    rJpgParam.u4CbSrcW = (rJpgParam.u4CbSrcW == (UINT32)0)? (UINT32)1 : rJpgParam.u4CbSrcW;
    rJpgParam.u4CbSrcH = (su4RzYSrcH == su4RzCbSrcH)?prImg->rHdrInfo.u4Height:(prImg->rHdrInfo.u4Height / 2);
    rJpgParam.u4CbSrcH = (rJpgParam.u4CbSrcH == (UINT32)0)? (UINT32)1 : rJpgParam.u4CbSrcH;
    rJpgParam.u4CrSrcBase1 = su4RzCurrCrAddr;    // cr1
    rJpgParam.u4CrSrcBase2 = su4RzPrevCrAddr;    // cr2
    rJpgParam.u4CrSrcHOffset = 0;
    rJpgParam.u4CrSrcVOffset = 0;
    rJpgParam.u4CrSrcW = (su4RzYSrcW == su4RzCrSrcW)?prImg->rHdrInfo.u4Width:(prImg->rHdrInfo.u4Width / 2);
    rJpgParam.u4CrSrcW = (rJpgParam.u4CrSrcW == (UINT32)0)? (UINT32)1 : rJpgParam.u4CrSrcW;
    rJpgParam.u4CrSrcH = (su4RzYSrcH == su4RzCrSrcH)?prImg->rHdrInfo.u4Height:(prImg->rHdrInfo.u4Height / 2);
    rJpgParam.u4CrSrcH = (rJpgParam.u4CrSrcH == (UINT32)0)? (UINT32)1 : rJpgParam.u4CrSrcH;
    rJpgParam.u4ColorComp = u4Comp;
    rJpgParam.u4IsRsOut = 1;       // block|raster based output
    rJpgParam.fgGray = (u4Comp == (UINT32)0xF) ? 1 : 0;
    rJpgParam.fgPreload = fgPreload;
    rJpgParam.u4OutSwap = 0;

    if (eRotateDegree == CLOCKWISE_0)    //directly output to display buffer
    {
		if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16)))
		{
			rJpgParam.u4YTgBase = (UINT32)prDispInfo->pu1DisplayStart_temp[0];
			rJpgParam.u4CTgBase = (UINT32)prDispInfo->pu1DisplayStart_temp[1];
		}
		else
		{
			rJpgParam.u4YTgBase = (UINT32)prDispInfo->pu1DisplayStart[0];
			rJpgParam.u4CTgBase = (UINT32)prDispInfo->pu1DisplayStart[1];
		}
        rJpgParam.u4IsJpgSameaddr = 0;
    }
    else	//output to scale working buf
    {
        rJpgParam.u4YTgBase = (UINT32)prImgLibMem->pau1ScDstAddr[0];
        if (prDispInfo->u4DisplayClrMode == AYCBCR8888_DIRECT32)
        {
            rJpgParam.u4CTgBase = (UINT32)prImgLibMem->pau1ScDstAddr[0];
        }
        else
        {            
            rJpgParam.u4CTgBase = (UINT32)prImgLibMem->pau1ScDstAddr[1];
        }
        rJpgParam.u4IsJpgSameaddr = 1;
    }
    if (rJpgParam.u4YSrcW < 4)rJpgParam.u4YSrcW = 4;
    if (rJpgParam.u4CbSrcW < 4)rJpgParam.u4CbSrcW = 4;
    if (rJpgParam.u4CrSrcW < 4)rJpgParam.u4CrSrcW = 4;

    u4DispW = prDispInfo->u4ScaleW;
    u4DispH = prDispInfo->u4ScaleH;
    rJpgParam.u4YTgW = (u4DispW > 4)? u4DispW : 4;
    rJpgParam.u4YTgH = u4DispH;
    if (eRotateDegree == CLOCKWISE_0 || eRotateDegree == CLOCKWISE_180 ||
        eRotateDegree == CLOCKWISE_0_WITH_FLIP || eRotateDegree == CLOCKWISE_180_WITH_FLIP) //center
    {

		if ((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16)))
		{
			rJpgParam.u4YTgHOffset = 0;
			rJpgParam.u4YTgVOffset = 0;		
			rJpgParam.u4YTgBufLen = (u4DispW + 0xF) & (~0xF);
		}
		else
		{
			rJpgParam.u4YTgHOffset = prDispInfo->u4StartX[0];
			rJpgParam.u4YTgVOffset = prDispInfo->u4StartY[0];
			rJpgParam.u4YTgBufLen = prDispInfo->u4DispPitch;
		}
    }
    else    //center op by gfx
    {
        rJpgParam.u4YTgHOffset = 0;
        rJpgParam.u4YTgVOffset = 0;    
        rJpgParam.u4YTgBufLen = u4DispW << 2;
    }    

    if (prJpgImg->u1MemClass <= RING_IN_SINGLE_OUT) 
    {
        rJpgParam.u4IsVdo2Osd = 1;     // output in osd format ?
        rJpgParam.u4CTgH = u4DispH;

        switch (prDispInfo->u4DisplayClrMode)
        {
        case AYCBCR8888_DIRECT32:
            rJpgParam.u4YTgCM = E_RZ_OSD_DIR_CM_AYCbCr8888;
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444;
            rJpgParam.u4CTgW = u4DispW;
            break;
		case ARGB8888_DIRECT32:
		case ARGB4444_DIRECT16:		
			rJpgParam.u4YTgCM = 0;
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
            rJpgParam.u4CTgW = u4DispW >> 1;
            rJpgParam.u4CTgH = u4DispH;
			rJpgParam.u4IsVdo2Osd = 0;	   // output in osd format ?		
			break;            
	 default:
 	     rJpgParam.u4YTgCM = E_RZ_OSD_DIR_CM_CbYCrY;
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;    
            rJpgParam.u4CTgW = u4DispW >> 1;
	     break;
        }
    }
    else
    {
        rJpgParam.u4IsVdo2Osd = 0;     // output in osd format ?
        rJpgParam.u4YTgCM = 0;
        switch (prDispInfo->u4DisplayClrMode)
        {
        case Y_CBCR420_SEP16:
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_420;
            rJpgParam.u4CTgW = u4DispW >> 1;
            rJpgParam.u4CTgH = u4DispH >> 1;
            break;
        case Y_CBCR422_SEP16:
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
            rJpgParam.u4CTgW = u4DispW >> 1;
            rJpgParam.u4CTgH = u4DispH;
            break;
        default:
          LOG(0,"Can NOT support this jpg fmt in video plane = %u\n", prDispInfo->u4DisplayClrMode);
          ASSERT(0);
          break;
        }
    }
    
    rJpgParam.u4IsFstBl = (u4RowIdx == (UINT32)0)? (UINT32)1 : (UINT32)0;
    rJpgParam.u4IsLstBl = (u4RowIdx == (UINT32)(u4TotalRows - 1))? (UINT32)1 : (UINT32)0;
    
    rJpgParam.u4IsJpgRzOn = 0;    // diable jpg-rz hand-off
    rJpgParam.u4JpgTempAddr = _u4JpgRzTempBuf;

    IMGRZ_Lock();
    if (su4IsRzInited == FALSE)
    {
        IMGRZ_ReInit();
        su4IsRzInited = TRUE;
    }   
    else
    {
        ePreImgrzType = IMGRZ_GetScaleOpt();
        if (ePreImgrzType != E_RZ_INOUT_TYPE_JPGMD)
        {
            IMGRZ_Unlock();
            su4IsRzInited = FALSE;
            TRY2((INT32)E_HWJPG_BUSY);
        }
    }
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_JPGMD);
    IMGRZ_Scale((void *)(&rJpgParam));
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
    if (u4RowIdx == (u4TotalRows - 1))
    {
        su4IsRzInited = FALSE;
    }
	CATCH_ERROR()
	{
		return i4Ret;
	}    
}
#endif

//#define JPGHW_TIME_PROFILE
#ifdef JPGHW_TIME_PROFILE
static HAL_TIME_T _rDecStart, _rRZStart;
static HAL_TIME_T _rDecEnd, _rRZEnd;
static HAL_TIME_T _rDecAcc, _rRZAcc;
static HAL_TIME_T _rDecdif, _rRZdif;
#endif

static INT32 _jpg_RowRz5391Drv(JPEGHANDLE hFlow, UINT32 u4RowIdx, 
    UINT32 u4MCUsPerRow, UINT32 u4TotalRows, UINT8 **apu1OutputAddr)
{
    RZ_JPG_SCL_PARAM_SET_T rJpgParam;
    IMG_LIB_INFO_T *prImg = prJpgImg;
    ENUM_IMG_ROTATE_PHASE eRotateDegree = eJpgRotateDegree;
    IMG_LIB_DISP_INFO_T *prDispInfo = &prImg->rDispInfo;
    IMG_LIB_MEM_LAYOUT_T *prImgLibMem = &prImg->rMem;
    JMEM_MW_T *prMemMw = NULL;
    JMEM_YUV_LINEAR_T *prMemYuv = NULL;
    JPEG_VLDJPG_DATA_T *prJpgHw = NULL;
    E_RZ_INOUT_TYPE_T ePreImgrzType;
    
    static UINT32 su4IsRzInited;
    static UINT32 su4RzCurrYAddr, su4RzPrevYAddr;
    static UINT32 su4RzCurrCbAddr, su4RzPrevCbAddr;
    static UINT32 su4RzCurrCrAddr, su4RzPrevCrAddr;
    static UINT32 su4RzYSrcW, su4RzCbSrcW, su4RzCrSrcW;
    static UINT32 su4RzYSrcH, su4RzCbSrcH, su4RzCrSrcH;
    //static UINT32 su4RzYTgW, su4RzYTgH, su4RzCTgW, su4RzCTgH;
    static UINT32 su4RzYSrcL;
    static UINT32 su4PreOutAddr6,su4PreOutAddr7,su4PreOutAddr8;	
    UINT32 au4RowSize[3];
    UINT32 u4DispW, u4DispH;
    INT32 i4Ret = (INT32)E_JPG_OK;
    UINT32 u4SleepCnt = 0, u4NonSleepCnt = 0,u4RzOutputZeroCnt=0;
    
    ASSERT(hFlow);
    ASSERT(apu1OutputAddr != NULL);

    VERIFY(VLDJPG_SetCurrMcuRowNum5391(u4RowIdx) == (INT32)E_HWJPG_OK);

    x_memset(&rJpgParam,0,sizeof(RZ_JPG_SCL_PARAM_SET_T));
    
    if (!u4RowIdx)
    {
        su4IsRzInited = FALSE;
    }

    if (prImg->u1MemClass < RING_IN_SINGLE_OUT)
    {
        prMemYuv = (JMEM_YUV_LINEAR_T *)(JPGMEM->pvLocalData);
        ASSERT(prMemYuv != NULL);
        su4RzYSrcW  = prMemYuv->au4RowWidth[0];
        su4RzCbSrcW = prMemYuv->au4RowWidth[1];
        su4RzCrSrcW = prMemYuv->au4RowWidth[2];
        su4RzYSrcH  = prMemYuv->au4RowHeight[0];
        su4RzCbSrcH = prMemYuv->au4RowHeight[1];
        su4RzCrSrcH = prMemYuv->au4RowHeight[2];
        _JDEC_SetRzV(&rJpgParam, prMemYuv->au4RowHeight);   
        au4RowSize[0] = prMemYuv->au4RowSize[0];
        au4RowSize[1] = prMemYuv->au4RowSize[1];
        au4RowSize[2] = prMemYuv->au4RowSize[2];
    }
    else
    {    
        prMemMw = (JMEM_MW_T *)(JPGMEM->pvLocalData);
        ASSERT(prMemMw != NULL);
        su4RzYSrcW  = prMemMw->au4RowWidth[0];
        su4RzCbSrcW = prMemMw->au4RowWidth[1];
        su4RzCrSrcW = prMemMw->au4RowWidth[2];
        su4RzYSrcH  = prMemMw->au4RowHeight[0];
        su4RzCbSrcH = prMemMw->au4RowHeight[1];
        su4RzCrSrcH = prMemMw->au4RowHeight[2];
        _JDEC_SetRzV(&rJpgParam, prMemMw->au4RowHeight);
        au4RowSize[0] = prMemMw->au4RowSize[0];
        au4RowSize[1] = prMemMw->au4RowSize[1];
        au4RowSize[2] = prMemMw->au4RowSize[2];
    }    

    prJpgHw = ((JPEG_VLDJPG_DATA_T*)H_JPGHW);
    ASSERT(prJpgHw != NULL);
    su4RzYSrcL  = prJpgHw->u4CompPitchRow[0];

    if (((u4RowIdx % 2) == 0) || (_u4JPGMode == (UINT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ))
    {
    	JPGMEM->pfnGetOutput(H_JPGMEM, (void**)apu1OutputAddr);

    	TRY2(VLDJPG_SetMcuRowCurr(H_JPGHW, u4RowIdx));
    	
    	TRY2(VLDJPG_SetMcuRowOutBuf(H_JPGHW, 0, apu1OutputAddr[0], au4RowSize[0]));
    	TRY2(VLDJPG_SetMcuRowOutBuf(H_JPGHW, 1, apu1OutputAddr[1], au4RowSize[1]));
    	TRY2(VLDJPG_SetMcuRowOutBuf(H_JPGHW, 2, apu1OutputAddr[2], au4RowSize[2]));

        // output bank1
    	TRY2(VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 0, apu1OutputAddr[6], au4RowSize[0]));
    	TRY2(VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 1, apu1OutputAddr[7], au4RowSize[1]));
    	TRY2(VLDJPG_SetMcuRowOutBufBank15391(H_JPGHW, 2, apu1OutputAddr[8], au4RowSize[2]));
    	
    	TRY2( VLDJPG_ValidateMcuRowData5391(H_JPGHW, u4MCUsPerRow, u4TotalRows)	);

        // for imgrz
        if (u4RowIdx == 0)
        {
#ifdef JPGHW_TIME_PROFILE        
        _rDecAcc.u4Seconds = 0;
    	_rDecAcc.u4Micros  = 0;
	    _rRZAcc.u4Seconds = 0;
    	_rRZAcc.u4Micros  = 0;
#endif    	
            su4RzPrevYAddr  = (UINT32)apu1OutputAddr[0];
            su4RzPrevCbAddr = (UINT32)apu1OutputAddr[1];
            su4RzPrevCrAddr = (UINT32)apu1OutputAddr[2];
        }
        else
        {
            su4RzPrevYAddr  = su4RzCurrYAddr;
            su4RzPrevCbAddr = su4RzCurrCbAddr;
            su4RzPrevCrAddr = su4RzCurrCrAddr;
        }
        
        su4RzCurrYAddr  = (UINT32)apu1OutputAddr[0];
        su4RzCurrCbAddr = (UINT32)apu1OutputAddr[1];
        su4RzCurrCrAddr = (UINT32)apu1OutputAddr[2];
        
        if (_u4JPGMode == (UINT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ)
        {
            su4RzPrevYAddr  = (UINT32)apu1OutputAddr[6];
            su4RzPrevCbAddr = (UINT32)apu1OutputAddr[7];
            su4RzPrevCrAddr = (UINT32)apu1OutputAddr[8];
        }
        
        su4PreOutAddr6 = (UINT32)apu1OutputAddr[6];
        su4PreOutAddr7 = (UINT32)apu1OutputAddr[7];
        su4PreOutAddr8 = (UINT32)apu1OutputAddr[8];
    }
    else
    {
        // for imgrz
        su4RzPrevYAddr  = su4RzCurrYAddr;
        su4RzPrevCbAddr = su4RzCurrCbAddr;
        su4RzPrevCrAddr = su4RzCurrCrAddr;

        su4RzCurrYAddr  = su4PreOutAddr6;//(UINT32)apu1OutputAddr[6];
        su4RzCurrCbAddr = su4PreOutAddr7;//(UINT32)apu1OutputAddr[7];
        su4RzCurrCrAddr = su4PreOutAddr8;//(UINT32)apu1OutputAddr[8];
    }
    
	// decode
#ifdef JPGHW_TIME_PROFILE	
	HAL_GetTime(&_rDecStart);
#endif
    if (_u4JPGMode != (UINT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ)        //mcu row mode
    {
        i4Ret = VLDJPG_DecodeOneMcuRow5391(H_JPGHW, u4RowIdx);
        rJpgParam.u4IsRsOut = 1;  //static jpeg is raster out
        rJpgParam.u4IsJpgRzOn = 0;    // diable jpg-rz hand-off
        rJpgParam.u4OutSwap = 0;
    }
    else                                                //pic mode
    {
    
        rJpgParam.u4IsJpgRzOn = 1;    // enable jpg-rz hand-off
        if(prImg->u1MemClass == EXTERNAL_RING_IN_DOUBLE_OUT)
        {
#ifdef CC_MT5395 // Force raster out for VENC
            rJpgParam.u4IsRsOut = 1;
            rJpgParam.u4OutSwap = 0;
#else
            rJpgParam.u4IsRsOut = 0;  //motion jpeg is block out  
            rJpgParam.u4OutSwap =(UINT32)IMGRZ_SWAP;
#endif
        }
        else
        {
            rJpgParam.u4IsRsOut = 1;  //linux turnkey demo 
            rJpgParam.u4OutSwap =0;
        }
    }
    
    if ((eRotateDegree != CLOCKWISE_0) && (rJpgParam.u4JpgVfacY > rJpgParam.u4JpgVfacCb))
    {
        //422V->444, 420->422H, for generating the identical output of Y & C
        int i;
        rJpgParam.u4JpgVfacCb = rJpgParam.u4JpgVfacCr = rJpgParam.u4JpgVfacY;
        su4RzCbSrcH = su4RzCrSrcH = su4RzYSrcH;
        for (i = 7; i>=0; i--)
        {
            x_memcpy(VIRTUAL((VOID *)((UINT32)su4RzCurrCbAddr+(i*2*su4RzYSrcL))),
                VIRTUAL((VOID *)((UINT32)su4RzCurrCbAddr+(i*su4RzYSrcL))),su4RzYSrcL);
            x_memcpy(VIRTUAL((VOID *)((UINT32)su4RzCurrCbAddr+((i*2+1)*su4RzYSrcL))),
                VIRTUAL((VOID *)((UINT32)su4RzCurrCbAddr+(i*su4RzYSrcL))),su4RzYSrcL);
            x_memcpy(VIRTUAL((VOID *)((UINT32)su4RzCurrCrAddr+(i*2*su4RzYSrcL))),
                VIRTUAL((VOID *)((UINT32)su4RzCurrCrAddr+(i*su4RzYSrcL))),su4RzYSrcL);
            x_memcpy(VIRTUAL((VOID *)((UINT32)su4RzCurrCrAddr+((i*2+1)*su4RzYSrcL))),
                VIRTUAL((VOID *)((UINT32)su4RzCurrCrAddr+(i*su4RzYSrcL))),su4RzYSrcL);
        }
		
#if defined(CC_MT5365) || defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(su4RzCurrCbAddr), (UINT32)(su4RzCbSrcH*su4RzYSrcL));
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(su4RzCurrCrAddr), (UINT32)(su4RzCrSrcH*su4RzYSrcL));
#else
    HalFlushInvalidateDCache();
#endif
    }
#ifdef JPGHW_TIME_PROFILE	
    HAL_GetTime(&_rDecEnd);
    HAL_GetDeltaTime(&_rDecdif, &_rDecStart, &_rDecEnd);
	_rDecAcc.u4Seconds += _rDecdif.u4Seconds;
	_rDecAcc.u4Micros  += _rDecdif.u4Micros;
	if (_rDecAcc.u4Micros >= 1000000)
	{
		_rDecAcc.u4Seconds += _rDecAcc.u4Micros / 1000000;
		_rDecAcc.u4Micros  %= 1000000;
	}
#endif

#if 1 // use imgrz
#ifdef JPGHW_TIME_PROFILE	
	//HAL_GetTime(&_rRZStart);
#endif    

    LOG(7, "RZ prev Y  addr = 0x%08x \n", su4RzPrevYAddr);
    LOG(7, "RZ prev Cb addr = 0x%08x \n", su4RzPrevCbAddr);
    LOG(7, "RZ prev Cr addr = 0x%08x \n", su4RzPrevCrAddr);
    LOG(7, "RZ curr Y  addr = 0x%08x \n", su4RzCurrYAddr);
    LOG(7, "RZ curr Cb addr = 0x%08x \n", su4RzCurrCbAddr);
    LOG(7, "RZ curr Cr addr = 0x%08x \n", su4RzCurrCrAddr);

    rJpgParam.u4IsRsIn = 1;        // block|raster based input
    rJpgParam.u4YSrcBase1 = su4RzCurrYAddr;     // y1
    rJpgParam.u4YSrcBase2 = su4RzPrevYAddr;     // y2
    rJpgParam.u4YSrcBufLen = su4RzYSrcL;
    rJpgParam.u4YSrcHOffset = 0;
    rJpgParam.u4YSrcVOffset = 0;
    rJpgParam.u4YSrcW = prImg->rHdrInfo.u4Width;
    rJpgParam.u4YSrcH = (su4RzYSrcH == su4RzCbSrcH) ? prImg->rHdrInfo.u4Height :(prImg->rHdrInfo.u4Height&0xfffffffe);
    rJpgParam.u4CbSrcBase1 = su4RzCurrCbAddr;    // cb1
    rJpgParam.u4CbSrcBase2 = su4RzPrevCbAddr;    // cb2
    rJpgParam.u4CbSrcHOffset = 0;
    rJpgParam.u4CbSrcVOffset = 0;
    rJpgParam.u4CbSrcW = (su4RzYSrcW == su4RzCbSrcW)? rJpgParam.u4YSrcW:(rJpgParam.u4YSrcW / 2);
    rJpgParam.u4CbSrcW = (rJpgParam.u4CbSrcW == (UINT32)0)? (UINT32)1 : rJpgParam.u4CbSrcW;
    rJpgParam.u4CbSrcH = (su4RzYSrcH == su4RzCbSrcH)? rJpgParam.u4YSrcH :(rJpgParam.u4YSrcH / 2);
    rJpgParam.u4CbSrcH = (rJpgParam.u4CbSrcH == (UINT32)0)? (UINT32)1 : rJpgParam.u4CbSrcH;
    rJpgParam.u4CrSrcBase1 = su4RzCurrCrAddr;    // cr1
    rJpgParam.u4CrSrcBase2 = su4RzPrevCrAddr;    // cr2
    rJpgParam.u4CrSrcHOffset = 0;
    rJpgParam.u4CrSrcVOffset = 0;
    rJpgParam.u4CrSrcW = (su4RzYSrcW == su4RzCrSrcW)? rJpgParam.u4YSrcW :(rJpgParam.u4YSrcW / 2);
    rJpgParam.u4CrSrcW = (rJpgParam.u4CrSrcW == (UINT32)0)? (UINT32)1 : rJpgParam.u4CrSrcW;
    rJpgParam.u4CrSrcH = (su4RzYSrcH == su4RzCrSrcH)? rJpgParam.u4YSrcH:(rJpgParam.u4YSrcH / 2);
    rJpgParam.u4CrSrcH = (rJpgParam.u4CrSrcH == (UINT32)0)? (UINT32)1 : rJpgParam.u4CrSrcH;

    rJpgParam.fgPreload = 0;
    if (su4RzCbSrcW == 0 || su4RzCrSrcW == 0 || su4RzCbSrcH == 0 || su4RzCrSrcH == 0)     
    {
        rJpgParam.u4CbSrcW = rJpgParam.u4CrSrcW = rJpgParam.u4YSrcW;
        rJpgParam.u4CrSrcH = rJpgParam.u4CbSrcH = rJpgParam.u4YSrcH;
        rJpgParam.fgGray = 1;
    }
    else
    {
        rJpgParam.fgGray = 0;
    }
    if (rJpgParam.u4YSrcW < 4)rJpgParam.u4YSrcW = 4;
    if (rJpgParam.u4CbSrcW < 4)rJpgParam.u4CbSrcW = 4;
    if (rJpgParam.u4CrSrcW < 4)rJpgParam.u4CrSrcW = 4;
    rJpgParam.u4ColorComp = 0x7;
    u4DispW = prDispInfo->u4ScaleW;
    u4DispH = prDispInfo->u4ScaleH;
    rJpgParam.u4YTgW = (u4DispW > 4)? u4DispW : 4;
    rJpgParam.u4YTgH = u4DispH;
    if (eRotateDegree == CLOCKWISE_0)    //directly output to display buffer    
    {   
        if((prImg->u1MemClass == (UINT8)RING_IN_SINGLE_OUT)&&((prImg->rDispInfo.u4DisplayClrMode==ARGB8888_DIRECT32)||(prImg->rDispInfo.u4DisplayClrMode==ARGB4444_DIRECT16)))
        {
			//rJpgParam.u4YTgBase = (UINT32)prImgLibMem->pau1ScDst2Addr[0];		
			//rJpgParam.u4CTgBase = (UINT32)prImgLibMem->pau1ScDst2Addr[1];
			rJpgParam.u4YTgBase = (UINT32)prDispInfo->pu1DisplayStart_temp[0];		
			rJpgParam.u4CTgBase = (UINT32)prDispInfo->pu1DisplayStart_temp[1];			
			rJpgParam.u4IsJpgSameaddr = 0;
			rJpgParam.u4YTgHOffset = 0;
			rJpgParam.u4YTgVOffset = 0;
			rJpgParam.u4YTgBufLen = (u4DispW + 0xF) & (~0xF);//((u4DispW << 2) + 0xF) & (~0xF);
        }
		else
		{
			rJpgParam.u4YTgBase = (UINT32)prDispInfo->pu1DisplayStart[0];		
			rJpgParam.u4CTgBase = (UINT32)prDispInfo->pu1DisplayStart[1];
			rJpgParam.u4IsJpgSameaddr = 0;
			rJpgParam.u4YTgHOffset = prDispInfo->u4StartX[0];
			rJpgParam.u4YTgVOffset = prDispInfo->u4StartY[0];
			rJpgParam.u4YTgBufLen = prDispInfo->u4DispPitch;
		}

    }    
    else	//output to scale working buf    
    {   
        if(0)//((prDispInfo->u4DisplayClrMode==ARGB8888_DIRECT32)||(prDispInfo->u4DisplayClrMode==ARGB4444_DIRECT16))
        {
			rJpgParam.u4YTgBase = (UINT32)prImgLibMem->pau1ScDst2Addr[0];		
			rJpgParam.u4CTgBase = (UINT32)prImgLibMem->pau1ScDst2Addr[1];
			rJpgParam.u4YTgBufLen = (u4DispW + 0xF) & (~0xF);
        }
		else
		{
			rJpgParam.u4YTgBase = (UINT32)prImgLibMem->pau1ScDstAddr[0];
			rJpgParam.u4YTgBufLen = ((u4DispW << 2) + 0xF) & (~0xF);
		}

        rJpgParam.u4IsJpgSameaddr = 1;
        rJpgParam.u4YTgHOffset = 0;
        rJpgParam.u4YTgVOffset = 0;        
        if (prDispInfo->u4DisplayClrMode == AYCBCR8888_DIRECT32)   
        {
           rJpgParam.u4CTgBase = (UINT32)rJpgParam.u4YTgBase ;
        }
        else
        {            
            rJpgParam.u4CTgBase = (UINT32)prImgLibMem->pau1ScDstAddr[1];
        }
    }
	
    if (prImg->u1MemClass == RING_IN_SINGLE_OUT) 
    {
        rJpgParam.u4IsVdo2Osd = 1;     // output in osd format ?        
        rJpgParam.u4CTgH = u4DispH;

        switch (prDispInfo->u4DisplayClrMode)
        {
        case AYCBCR8888_DIRECT32:	
            rJpgParam.u4YTgCM = E_RZ_OSD_DIR_CM_AYCbCr8888;
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444;
            rJpgParam.u4CTgW = u4DispW;
            break;
            
		case ARGB8888_DIRECT32:
		case ARGB4444_DIRECT16:		
			rJpgParam.u4YTgCM = 0;
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
            rJpgParam.u4CTgW = u4DispW >> 1;
            rJpgParam.u4CTgH = u4DispH;
			rJpgParam.u4IsVdo2Osd = 0;	   // output in osd format ?		
			break;
	 default:
 	     rJpgParam.u4YTgCM = E_RZ_OSD_DIR_CM_CbYCrY;
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;    
            rJpgParam.u4CTgW = u4DispW >> 1;
            rJpgParam.u4IsVdo2Osd = 0;  // output in video format
	     break;
        }
    }
    else
    {
        rJpgParam.u4IsVdo2Osd = 0;     // output in VDO format ?
        rJpgParam.u4YTgCM = 0;
        switch (prDispInfo->u4DisplayClrMode)
        {
        //case E_JPEG_YUV420:
        //case E_JPEG_YUV422V:
        case Y_CBCR420_SEP16:
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_420;
            rJpgParam.u4CTgW = u4DispW >> 1;
            rJpgParam.u4CTgH = u4DispH >> 1;
            break;
        //case E_JPEG_YUV422:
        //case E_JPEG_YUV444:
        case Y_CBCR422_SEP16:
            rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422;
            rJpgParam.u4CTgW = u4DispW >> 1;
            rJpgParam.u4CTgH = u4DispH;
            break;
        default:
          LOG(0,"Can NOT support this jpg fmt in video plane = %u\n", prDispInfo->u4DisplayClrMode);
          ASSERT(0);
          break;
        }
    }

    LOG(7, "RZ target Y addr = 0x%08x \n", rJpgParam.u4YTgBase);
    LOG(7, "RZ target C addr = 0x%08x \n", rJpgParam.u4CTgBase);

    rJpgParam.u4IsFstBl = (u4RowIdx == (UINT32)0)? (UINT32)1 : (UINT32)0;
    rJpgParam.u4IsLstBl = (u4RowIdx == (UINT32)(u4TotalRows - 1))? (UINT32)1 : (UINT32)0;

    //rJpgParam.u4IsJpgRzOn = 0;    // diable jpg-rz hand-off

    rJpgParam.u4JpgTempAddr = _u4JpgRzTempBuf;

    IMGRZ_Lock();
    if (su4IsRzInited == FALSE)
    { 
        IMGRZ_ReInit();
        su4IsRzInited = TRUE;
    } 
    else
    {
        ePreImgrzType = IMGRZ_GetScaleOpt();
        if (ePreImgrzType != E_RZ_INOUT_TYPE_JPGMD)         //break
        {
            IMGRZ_Unlock();
            su4IsRzInited = FALSE;
            TRY2((INT32)E_HWJPG_BUSY);
        }
    }
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_JPGMD);
    IMGRZ_Scale((void *)(&rJpgParam));   
#ifdef JPGHW_TIME_PROFILE	
	HAL_GetTime(&_rRZStart);
#endif  
    IMGRZ_Flush();
    if (_u4JPGMode == (UINT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ)              //Pic mode
    {
#if 1
        JPGFLW->u4DecodedSize=0;
        VLDJPG_DecodeOnePic(H_JPGHW);
        while (VLDJPG_CheckDecodePicFinish(H_JPGHW) != (INT32)E_HWJPG_OK)
        {            
            u4NonSleepCnt++;
#ifdef JPEG_AUTO_TEST
					  if(_JpgVerifyCmdGetParam(19,0))
					  	{
					  	    Printf("VLDJPG_SetStop()\n");
                            VLDJPG_SetStop(H_JPGHW);
					  	}
#endif
            if (JPGHDR->fgRingBuffer && (!JPGHDR->fgEOF))
            {
                _jpg_UpdateReadPtr(hFlow);
				LOG(7,"Decoding Progress=%d%\n",JDEC_GetProgress(hFlow));
                if (JPGHDR->u4RingData < (UINT32)(200 * 1024))
                {
                    LOG(3, "JPGHDR->u4RingData:%d\n", JPGHDR->u4RingData);
                    IMG_BsBufManager();  
                    u4RzOutputZeroCnt=0;
                }
                else
                {
                    x_thread_delay(1);
                    if(_IMGRZ_GetScaleLine()==0)
                    {
                       u4RzOutputZeroCnt++;
                    }
                    else
                    {
                       u4RzOutputZeroCnt=0;
                    }
                }
                u4SleepCnt++;  
                u4NonSleepCnt = 0;
            }     
            else
            {
               if(_IMGRZ_GetScaleLine()==0)
               {
               	 x_thread_delay(1);
                 u4RzOutputZeroCnt++;
                 u4SleepCnt++;  
               }
               else
               {
                 u4RzOutputZeroCnt=0;
               }         	
            }
            if (VLDJPG_CheckDecodingError(H_JPGHW) != (INT32)E_HWJPG_OK)
            {
                i4Ret = -(INT32)E_JPG_ERR_FATAL;
                break;
            }
            if (u4NonSleepCnt >= (UINT32)0x80000)
            {
                x_thread_delay(1);
                u4SleepCnt++;  
                u4NonSleepCnt = 0;
                if (VLDJPG_CheckStop(H_JPGHW) != (INT32)E_HWJPG_OK)
                {
                    i4Ret = -(INT32)E_JPG_ERR_FATAL;
                    LOG(3, "already stop\n");
                    break;
                }
            }
            if (u4SleepCnt >= (UINT32)30000 || (u4RzOutputZeroCnt>=((rJpgParam.u4YSrcH/u4DispH+10)*2)))
            {
                i4Ret = -(INT32)E_JPG_ERR_FATAL;
                LOG(3, "JPGHDR->u4RingData:%d\n", JPGHDR->u4RingData);
                break;
            }
            if (_fgJpgFlw2Stop)
            {
                i4Ret = -(INT32)E_JPG_ERR_FATAL;
                break;
            }
        }
        
        prJpgImg->u4RowIdx = (UINT32)(u4TotalRows - 1);
        u4RowIdx = prJpgImg->u4RowIdx;
#else
        i4Ret = VLDJPG_DecodeOnePic5391(H_JPGHW, u4RowIdx);
#endif

        if (i4Ret)
        {
            su4IsRzInited = FALSE;
            prJpgImg->u4RowIdx = (UINT32)(u4TotalRows - 1);

            IMGRZ_UnlockWaitIsr();
            IMGRZ_UnlockCmdque();
            IMGRZ_Unlock();
            VLDJPG_SetStop(H_JPGHW);
            TRY2(i4Ret);
        }
    }
    IMGRZ_Wait();
    prDispInfo->u4ScaleRowH =  _IMGRZ_GetScaleLine();
    IMGRZ_Unlock();    
#ifdef JPGHW_TIME_PROFILE	
    HAL_GetTime(&_rRZEnd);
    HAL_GetDeltaTime(&_rRZdif, &_rRZStart, &_rRZEnd);
	_rRZAcc.u4Seconds += _rRZdif.u4Seconds;
	_rRZAcc.u4Micros  += _rRZdif.u4Micros;
	if (_rRZAcc.u4Micros >= 1000000)
	{
		_rRZAcc.u4Seconds += _rRZAcc.u4Micros / 1000000;
		_rRZAcc.u4Micros  %= 1000000;
	}
#endif


    if (u4RowIdx == (u4TotalRows - 1))
    {
        su4IsRzInited = FALSE;
#ifdef JPGHW_TIME_PROFILE	        
        LOG(0, "decoding %u.%06u/RZ %u.%06u\n",
               _rDecAcc.u4Seconds, _rDecAcc.u4Micros, 
               _rRZAcc.u4Seconds, _rRZAcc.u4Micros);        
#endif
        VLDJPG_SetPowerOff();
    }
#endif

	// update read pointer and data size of ring buffer
	if (JPGHDR->fgRingBuffer && (!JPGHDR->fgEOF))
	{
		_jpg_UpdateReadPtr(hFlow);	
	}
	
	JPGFLW->u4DecodedSize=JPGFLW->u4FileSize;
	LOG(7,"Decoding Progress=%d%\n",JDEC_GetProgress(hFlow));
	CATCH_ERROR()
	{   
		return i4Ret;
	}
}

extern INT32 vJpegProgDec(JPEGHANDLE hParser, void **pvCoefBuffer, void **pvOutBuffer, 
                            void **pvNzBuffer);

INT32 JDEC_SwitchToProgDec(JPEGHANDLE hFlow, IMG_LIB_MEM_LAYOUT_T *prMem)
{
    JMEM_YUV_LINEAR_T *prMemYuv = NULL;
    JMEM_BASE_T* prMemBase;
    VOID *pavComp[9];
    VOID *pvCoefBuffer[3];
    UINT32 au4AllocSize[3], au4CompUnitWidth[3], au4CompUnitHeight[3], au4CompSize[3];
    INT32 i4Ret = 0;

    prMemBase = (JMEM_BASE_T*)JPGMEM->pvLocalData;
    if (!prMemBase->fgProgScan)
    {
        return -1;
    }
    if (prJpgImg->u1MemClass < RING_IN_SINGLE_OUT)
    {
        prMemYuv = (JMEM_YUV_LINEAR_T *)(JPGMEM->pvLocalData);
        UNUSED(prMemYuv);
        //i4Ret = vJpegProgDec(JPGHDR, prMemYuv->pvCoefBuffer, prMemYuv->pvComp, JPGMEMBS->apvNzBuffer);
    }
    else
    {
        au4CompUnitWidth[0] = prMemBase->aau4CompFactor[0][0] * D_DCTSIZE;
	    au4CompUnitWidth[1]	= prMemBase->aau4CompFactor[1][0] * D_DCTSIZE;
	    au4CompUnitWidth[2] = prMemBase->aau4CompFactor[2][0] * D_DCTSIZE;
	    au4CompUnitHeight[0] = prMemBase->aau4CompFactor[0][1] * D_DCTSIZE;
	    au4CompUnitHeight[1] = prMemBase->aau4CompFactor[1][1] * D_DCTSIZE;
	    au4CompUnitHeight[2] = prMemBase->aau4CompFactor[2][1] * D_DCTSIZE;
	    au4CompSize[0] = au4CompUnitWidth[0] * au4CompUnitHeight[0];
	    au4CompSize[1] = au4CompUnitWidth[1] * au4CompUnitHeight[1];
	    au4CompSize[2] = au4CompUnitWidth[2] * au4CompUnitHeight[2];        
        pavComp[0] = prMem->pavComp[0];
        pavComp[1] = prMem->pavComp[1];
        pavComp[2] = prMem->pavComp[2];
        pavComp[6] = prMem->pavComp[3];
        pavComp[7] = prMem->pavComp[4];
        pavComp[8] = prMem->pavComp[5];
        au4AllocSize[0] = prMemBase->u4McuCount * au4CompSize[0];
        au4AllocSize[1] = prMemBase->u4McuCount * au4CompSize[1];
        au4AllocSize[2] = prMemBase->u4McuCount * au4CompSize[2];
        
        pvCoefBuffer[0] = (VOID *)((prMem->u4CurrentUse + 15) & ~15);
        if ((JPGHDR->rSOF.u2ImageWidth * JPGHDR->rSOF.u2ImageHeight) <= JPG_COLLECT_LIMIT)
        {
            pvCoefBuffer[1] = (VOID *)((UINT32)pvCoefBuffer[0] + (au4AllocSize[0] << 1));
            pvCoefBuffer[2] = (VOID *)((UINT32)pvCoefBuffer[1] + (au4AllocSize[1] << 1));
            prMemBase->apvNzBuffer[0] = (VOID *)((UINT32)pvCoefBuffer[2] + (au4AllocSize[2] << 1));
        }
        else
        {
            UINT32 u4Size;
            u4Size = (prMemBase->u4McuWidth * au4CompSize[0]) << 1;
            pvCoefBuffer[2] = pvCoefBuffer[1] = pvCoefBuffer[0];
            prMemBase->apvNzBuffer[0] = (VOID *)((UINT32)pvCoefBuffer[2] + u4Size);
        }
        
        prMemBase->apvNzBuffer[1] = (VOID *)((((UINT32)prMemBase->apvNzBuffer[0] + (au4AllocSize[0] >> 3)) + 15) & ~15);
        prMemBase->apvNzBuffer[2] = (VOID *)((((UINT32)prMemBase->apvNzBuffer[1] + (au4AllocSize[1] >> 3)) + 15) & ~15);
        prMem->u4CurrentUse = (UINT32)prMemBase->apvNzBuffer[2] + (au4AllocSize[2] >> 3);
        i4Ret = vJpegProgDec(JPGHDR, pvCoefBuffer, pavComp, prMemBase->apvNzBuffer);
    }

    return i4Ret;
}

UINT16 bDitherMatrix[4][4] = 
{
{0, 8, 2, 10},
{12, 4, 14, 6},
{3, 11, 1, 9},
{15, 7, 13, 5}
};

void _jpg_RGB2Gamma(UINT32 u4Addr, UINT32 u4Width, UINT32 u4Height, UINT32 u4OffsetX, 
                    UINT32 u4OffsetY, UINT32 u4Pitch)
{
    UINT32 u4WidthMinus1, u4HeightMinus1,u4Jump, u4ActualWidth = u4Pitch / 4;
    INT32 *pi4Buf = (INT32 *)u4Addr;        
    INT32 i4TempValue;    
    INT32 i4R, i4G, i4B, i4A, i4Y, i4Cb, i4Cr, i4I, i4J;//, i4ILow3;    
    UINT8 *pu1Range = _acRange;   
#ifdef JPGHW_TIME_PROFILE	
    HAL_TIME_T _rRGBStart, _rRGBEnd, _rRGBdif;
    HAL_GetTime(&_rRGBStart); 
#endif
    pu1Range += 256; 
    pi4Buf = (INT32 *)(u4Addr + (u4OffsetY * u4Pitch) + (u4OffsetX * 4));  
    u4WidthMinus1 = u4Width - 1;
    u4HeightMinus1 = u4Height - 1;
    u4Jump = u4ActualWidth - u4Width;

    for (i4I = u4HeightMinus1; i4I >= 0 ; i4I--)    
    {
        //i4ILow3 = i4I & 0x3;
        for (i4J = u4WidthMinus1; i4J >= 0; i4J--)        
        {            
            i4TempValue = *pi4Buf;
            i4A = (i4TempValue & 0xff000000) >> 24;            
            i4Y = (i4TempValue & 0x00ff0000) >> 16;            
            i4Cb = (i4TempValue & 0x0000ff00) >> 8;            
            i4Cr = (i4TempValue & 0x000000ff);             
            
            i4R = (INT32)pu1Range[i4Y + _ai4RR[i4Cr]];  
            //i4R = (INT32)(_au2GammaR[i4R] + bDitherMatrix[i4ILow3][i4J&3]) >> 4;
            i4R = (INT32)pu1Range[i4R];  
                        
            i4G = (INT32)pu1Range[i4Y + ((_ai4BG[i4Cb] + _ai4RG[i4Cr]) >> 16)];   
            //i4G = (INT32)(_au2GammaG[i4G] + bDitherMatrix[i4ILow3][i4J&3]) >> 4;
            i4G = (INT32)pu1Range[i4G];  
                        
            i4B = (INT32)pu1Range[i4Y + _ai4BB[i4Cb]];
            //i4B = (INT32)(_au2GammaB[i4B] + bDitherMatrix[i4ILow3][i4J&3]) >> 4;
            i4B = (INT32)pu1Range[i4B];  
            
            *pi4Buf = (i4A << 24) | (i4R << 16) | (i4G << 8) | i4B;
            pi4Buf++; 
        }
        pi4Buf += u4Jump;
    }
#ifdef JPGHW_TIME_PROFILE	    
    HAL_GetTime(&_rRGBEnd);        
    HAL_GetDeltaTime(&_rRGBdif, &_rRGBStart, &_rRGBEnd);
    LOG(0, "RGB+Gamma %u.%06u\n", _rRGBdif.u4Seconds, _rRGBdif.u4Micros);  
#endif    
}

VOID JDEC_SWGamma(UINT8 *pu1Addr, IMG_LIB_DISP_INFO_T *prDispInfo, UINT32 u4X, UINT32 u4Y)
{    
        if ((eJpgRotateDegree == CLOCKWISE_0) || (eJpgRotateDegree == CLOCKWISE_0_WITH_FLIP) ||
           (eJpgRotateDegree == CLOCKWISE_180) || (eJpgRotateDegree == CLOCKWISE_180_WITH_FLIP))
        {
            _jpg_RGB2Gamma((UINT32)pu1Addr, prDispInfo->u4ScaleW, prDispInfo->u4ScaleH, 
                           u4X, u4Y, prDispInfo->u4DispPitch);
        }
        else
        {
            _jpg_RGB2Gamma((UINT32)pu1Addr, prDispInfo->u4ScaleH, prDispInfo->u4ScaleW,
                           u4X, u4Y, prDispInfo->u4DispPitch);
        }
}


