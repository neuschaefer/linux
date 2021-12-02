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
#if ! defined(DRV_JPG_H)
#define DRV_JPG_H

#include "x_lint.h"

typedef void* JPEGHANDLE;

typedef enum _ENUM_JPEG_CLASS_T
{
	E_JC_UNDEFINED_CLASS,
	E_JC_JFIF_HEADER,
	E_JC_VLD_DECODER,
	E_JC_IJG_DECODER,
	E_JC_HW_FLOW,
	E_JC_SW_FLOW,
	E_JC_MEM_YUV,
	E_JC_MEM_RGB,
	E_JC_MEM_ARGB,
	E_JC_MEM_ARGB_GFX,
	E_JC_MEM_MB,
	E_JC_MEM_MB_MTK,
	E_JC_MEM_MW,
	E_JC_LAST
} ENUM_JPEG_CLASS_T;

typedef struct _JPEG_BASE_CLASS_T
{
	ENUM_JPEG_CLASS_T	eJpegClass;
	SIZE_T				zObjectSize;
} JPEG_BASE_CLASS_T;



/* types use in flw (tired) */
#define MCUROW_COMMON_BLOCK() \
	void	*pvFreePoint; \
	UINT32	u4Width, u4Height; \
	UINT32	u4McuWidth, u4McuHeight; \
	UINT32	aau4Factor[3][2]; \
	UINT32	au4MaxFactor[2]

typedef struct _YUV_MCUROW_T
{
	MCUROW_COMMON_BLOCK();
	UINT32	u4SizeY, u4SizeU, u4SizeV;
	UINT8	*pu1Y, *pu1U, *pu1V;
} YUV_MCUROW_T;

typedef struct _YUV_BLOCK_T
{
	MCUROW_COMMON_BLOCK();
	UINT32	u4SizeY, u4SizeU, u4SizeV;
	UINT8	*pu1Y, *pu1U, *pu1V;
} YUV_BLOCK_T;

typedef struct _JPG_MB_T
{
	MCUROW_COMMON_BLOCK();
	UINT32	u4SizeY, u4SizeUV;
	UINT8	*pu1Y, *pu1UV;
} JPG_MB_T;

typedef struct _ARGB_DATA_T
{
	MCUROW_COMMON_BLOCK();
	UINT32	u4NumPixel;
	UINT32	*pu4Pixel;
} ARGB_DATA_T;


/* new types for flw2 (wired) */
typedef struct _JMEM_BASE_T
{
	void	*pvFreePoint;
	UINT32	u4Width;
	UINT32	u4Height;
	UINT32	u4McuWidth;
	UINT32	u4McuHeight;
	UINT32	u4McuCount;
	UINT32	aau4CompFactor[3][2];
	UINT32	au4CompBlock[3];
	UINT32	au4MaxFactor[2];
	BOOL	fgProgScan;
	/* temp buffers */
	void*			apvNzBuffer[3];
} JMEM_BASE_T;

typedef struct _JMEM_YUV_LINEAR_T
{
	JMEM_BASE_T		rBase;
	UINT32			au4McuAdv[3];
	UINT32			au4RowAdv[3];
	UINT32			au4AllocSize[3];
	void*			pvComp[3];
	/* temp buffers */
	void*			pvCoefBuffer[3];
} JMEM_YUV_LINEAR_T;

typedef struct _JMEM_MB_MTK_T
{
	JMEM_BASE_T		rBase;
} JMEM_MB_MTK_T;

typedef struct _JMEM_MW_T
{
	JMEM_BASE_T		rBase;
	UINT32			au4McuAdv[3];
	UINT32			au4RowAdv[3];
	UINT32			au4RowSize[3];
	INT32			i4McuRows;
	void*			pvComp[3];
	JPEGHANDLE		hMem;			/* output handle - for memory cleanup */
} JMEM_MW_T;


/* jfif types start */

#define D_MAX_JPEG_COMPONENT	4
//#define D_MAX_JPEG_HW_COMP		3
#define D_MAX_NUM_APP0			2
#define D_MAX_NUM_SCAN			32

#define D_JPG_STREAM_SIZE		(1024 * 1024)

typedef enum _ENUM_JPEG_MARKER_CODE_T	/* JPEG marker codes */
{
	M_SOF0  = 0xffc0,
	M_SOF1  = 0xffc1,
	M_SOF2  = 0xffc2,
	M_SOF3  = 0xffc3,

	M_SOF5  = 0xffc5,
	M_SOF6  = 0xffc6,
	M_SOF7  = 0xffc7,

	M_JPG   = 0xffc8,
	M_SOF9  = 0xffc9,
	M_SOF10 = 0xffca,
	M_SOF11 = 0xffcb,

	M_SOF13 = 0xffcd,
	M_SOF14 = 0xffce,
	M_SOF15 = 0xffcf,

	M_DHT   = 0xffc4,

	M_DAC   = 0xffcc,

	M_RST0  = 0xffd0,
	M_RST1  = 0xffd1,
	M_RST2  = 0xffd2,
	M_RST3  = 0xffd3,
	M_RST4  = 0xffd4,
	M_RST5  = 0xffd5,
	M_RST6  = 0xffd6,
	M_RST7  = 0xffd7,

	M_SOI   = 0xffd8,
	M_EOI   = 0xffd9,
	M_SOS   = 0xffda,
	M_DQT   = 0xffdb,
	M_DNL   = 0xffdc,
	M_DRI   = 0xffdd,
	M_DHP   = 0xffde,
	M_EXP   = 0xffdf,

	M_APP0  = 0xffe0,
	M_APP1  = 0xffe1,
	M_APP2  = 0xffe2,
	M_APP3  = 0xffe3,
	M_APP4  = 0xffe4,
	M_APP5  = 0xffe5,
	M_APP6  = 0xffe6,
	M_APP7  = 0xffe7,
	M_APP8  = 0xffe8,
	M_APP9  = 0xffe9,
	M_APP10 = 0xffea,
	M_APP11 = 0xffeb,
	M_APP12 = 0xffec,
	M_APP13 = 0xffed,
	M_APP14 = 0xffee,
	M_APP15 = 0xffef,

	M_JPG0  = 0xfff0,
	M_JPG13 = 0xfffd,
	M_COM   = 0xfffe,

	M_TEM   = 0xff01,

	M_ERROR = 0x100
} ENUM_JPEG_MARKER_CODE_T;

/* jpeg format */
typedef enum _ENUM_JPEG_FORMAT_T
{
	E_JPG_UNKNOWN_FORMAT,
	E_JPG_BASELINE,
	E_JPG_EX_SEQ_HUFFMAN,
	E_JPG_PROGRESSIVE_HUFFMAN,
	E_JPG_EX_SEQ_ARITHMETIC,
	E_JPG_PROGRESSIVE_ARITHMETIC,
	E_JPG_LOSSLESS_HUFFMAN,
	E_JPG_DIFFERENTIAL_SEQ_HUFFMAN,
	E_JPG_DIFF_PROG_HUFFMAN,
	E_JPG_DIFF_LLESS_HUFFMAN,
	E_JPG_RESERVED_FOR_EXTENSIONS,
	E_JPG_LOSSLESS_ARITHMETIC,
	E_JPG_DIFF_SEQ_ARITHMETIC,
	E_JPG_DIFF_PROG_ARITHMETIC,
	E_JPG_UNSUPPORT_FORMAT
} ENUM_JPEG_FORMAT_T;

/* component info in SOF marker */
typedef struct _JPEG_SOF_COMP_T
{
	UINT8		u1ComponentId;
	UINT8		u1HSampFactor;
	UINT8		u1VSampFactor;
	UINT8		u1QuantTblNo;
} JPEG_SOF_COMP_T;

/* SOF data */
typedef struct _JPEG_JFIF_SOF_T
{
	BOOL					fgSOF;			/* indicate that already have an SOF marker */
	ENUM_JPEG_FORMAT_T		eJpegFormat;
	UINT8					u1DataPrecision;
	UINT16					u2ImageHeight;
	UINT16					u2ImageWidth;
	UINT8					u1NumComponents;
	UINT8					au1MapId2Index[256];
	JPEG_SOF_COMP_T			arSofComp[D_MAX_JPEG_COMPONENT];
} JPEG_JFIF_SOF_T;

/* raw de-huffman table */
typedef struct _JPEG_DHT_HUFF_TBL_T
{
	UINT8 au1Bits[17];
	UINT8 au1HuffVal[256];
} JPEG_DHT_HUFF_TBL_T;

/* DHT data */
typedef struct _JPEG_JFIF_DHT_T
{
	UINT32					u4NumDcTbl;
	UINT32					u4NumAcTbl;
	UINT32					fgDcTblLoaded;	/* bit mask for loaded dc table */
	UINT32					fgAcTblLoaded;	/* bit mask for loaded ac table */
	JPEG_DHT_HUFF_TBL_T		arDcTbl[4];
	JPEG_DHT_HUFF_TBL_T		arAcTbl[4];
} JPEG_JFIF_DHT_T;

/* DQT data */
typedef struct _JPEG_JFIF_DQT_T
{
	/*
	although we leave 2bytes * 64 space here,
	if q table precision is 8bits, we use only
	first half (1x64) of this table
	*/
	UINT8				aau1Qtbl[4][128];
	BOOL				afgPrec[4];
	UINT8				u1NumQ;
	BOOL				fgDQT;
} JPEG_JFIF_DQT_T;

typedef enum _ENUM_APP0_THUMBNAIL_MODE_T
{
	E_THUMB_NONE			= 0x00,
	E_THUMB_JPEG			= 0x10,
	E_THUMB_1BYTE_PIX		= 0x11,
	E_THUMB_3BYTE_PIX		= 0x13
} ENUM_APP0_THUMBNAIL_MODE;

typedef enum _ENUM_APP0_TYPE_T
{
	E_APP0_JFIF,
	E_APP0_JFXX,
	E_APP0_ADOBE,
	E_APP0_OTHER
} ENUM_APP0_TYPE_T;

/* APP0 - jfif, jfxx */
typedef struct _JPEG_JFIF_APP0_T
{
	BOOL				fgAPP0;
	ENUM_APP0_TYPE_T	eApp0Type;
	/* jfif */
	UINT16				u2Version;
	UINT8				u1Units;
	UINT16				u2DensityX;
	UINT16				u2DensityY;

	/* jfxx */
	ENUM_APP0_THUMBNAIL_MODE	eThumbnailMode;

	/* common part in all thumbnail mode */
	BOOL				fgHasThumbnail;
	/* thumbnail dimension */
	UINT8				u1ThumbW;
	UINT8				u1ThumbH;
	/* jpeg thumbnail mode */
	UINT8				*pu1JpegThumbnail;	/* point to JPEG data in APP0 stream */
	/* 1 byte pixel thumbnail mode */
	UINT8				*pu1ColorData;
	UINT32				au4Palette[256];	/* force A to 255, R G B read from APP0 */
	/* 3 byte pixel thumbnail mode */
	UINT8				*pu1RGBdata;		/* point to RGB data in APP0 stream */
} JPEG_JFIF_APP0_T;

/* APP1 - exif */
typedef struct _JPEG_JFIF_APP1_T
{
	BOOL				fgAPP1;		/* APP1 marker found */
	UINT8				*pu1ExifData;
	/* TODO: implement thumbnail picture in 1st IFD here */
	/* TODO: implement partial 0th IFD tag here */
} JPEG_JFIF_APP1_T;

typedef struct _JPEG_JFIF_SOS_T
{
	UINT32				u4ScanPass;			/* scan pass */
	UINT8				u1CompInScan;
	UINT8				au1CompId[D_MAX_JPEG_COMPONENT];
	UINT8				au1DcId[D_MAX_JPEG_COMPONENT];
	UINT8				au1AcId[D_MAX_JPEG_COMPONENT];
	UINT8				u1Ss, u1Se, u1AhAl;
	UINT8				*pu1ScanTableStart;
	UINT8				*pu1ScanDataStart;
} JPEG_JFIF_SOS_T;

typedef struct _JPEG_JFIF_HEADER_T
{
	JPEG_BASE_CLASS_T	rParent;
	/* memory address of jpeg image */
	UINT8				*pu1JpegImage;
	/* file size of this jpeg image */
	UINT32				u4ImageByteSize;
	/* ring buffer support */
		BOOL			fgRingBuffer;		/* in ring buffer mode or not */
		UINT32			u4RingBufSize;		/* ring buffer size */
		UINT8			*pu1RingBufStart;	/* ring buffer start address */
		UINT32			u4RingRP;			/* read pointer */
		UINT32			u4RingData;			/* data size in ring buffer */
	/* flags for data status */
	BOOL				fgInited;			/* jpeg image set */
	BOOL				fgParsed;			/* jpeg image parsed successfully */
	BOOL				fgFatalError;		/* fatal error found and can not decode */
	BOOL				fgThumbnail;		/* thumbnail image available (app0 or app1 marker) */
	/* current read pointer */
	UINT8				*pu1CurrRead;
	/* SOI - start of image */
	BOOL				fgSOI;				/* indicate that already have an SOI marker */
	/* SOF - start of frame */
	JPEG_JFIF_SOF_T		rSOF;
	/* DHT - de-huffman table */
	JPEG_JFIF_DHT_T		rDHT;
	/* DQT - de-quantization table */
	JPEG_JFIF_DQT_T		rDQT;
	/* DRI - restart interval */
	UINT16				u2RestartInterval;
	/* FMT - format flag */
	UINT32				fgFormat;			/* JFIF or Exif or ... */
	/* APP0 */
	UINT32				u4NumApp0;
	JPEG_JFIF_APP0_T	arAPP0[D_MAX_NUM_APP0];
	/* APP1 */
	JPEG_JFIF_APP1_T	rAPP1;
	/* SOS - start of scan */
	UINT32				u4NumScan;
	UINT32				u4CurrScan;
	JPEG_JFIF_SOS_T		arSOS[D_MAX_NUM_SCAN];
	/* EOI */
	BOOL				fgEOI;					/* EOI reached */
	UINT8				*pu1EOI;				/* point to EOI marker */
	/* LAST SCAN */
	UINT32				au4CompLastScan[D_MAX_JPEG_COMPONENT];
} JPEG_JFIF_HEADER_T;


/* jfif types end */


/* JFIF group - jpeg header parser */
JPEGHANDLE JFIF_AllocInstance		(void);
	// allocate an jpeg jfif header instance
void JFIF_FreeInstance				(JPEGHANDLE hInstance);
	// release an jpeg jfif header instance
INT32 JFIF_New						(JPEGHANDLE hInstance, void *pvJpegImage, UINT32 u4JpegSize);
	// start and parse a jpeg file image store in memory
INT32 JFIF_Dump						(JPEGHANDLE hInstance);
	// dump jpeg header parsing result
SIZE_T JFIF_GetHeaderSize			(void);
	// get size of jpeg jfif header
INT32 JFIF_ParseProgScan			(JPEGHANDLE hInstance);
	// parse progressive scan (init only)
INT32 JFIF_ParseProgMarker			(JPEGHANDLE hInstance, UINT32 u4Scan);

/* VLDJPG group - vld operations */
JPEGHANDLE VLDJPG_AllocInstance		(void);
	// allocate an vld jpeg hardware instance
void VLDJPG_FreeInstance			(JPEGHANDLE hInstance);
	// release an vld jpeg hardware instance
INT32 VLDJPG_New					(const JPEGHANDLE hInstance);
	// start hardware decoder, init data structure (allocate vld0)

/* decode flow */
INT32 JPG_DecodeYUV					(void *pvJpegImage, UINT32 u4Size, YUV_BLOCK_T *prYUV);

/* flw2 */
JPEGHANDLE JDEC_AllocInstance		(void);
INT32 JDEC_New						(JPEGHANDLE hDec);
void JDEC_FreeInstance				(JPEGHANDLE hDec);
INT32 JDEC_DecodeYUV				(JPEGHANDLE hDec, void *pvImage, SIZE_T zByteSize, JMEM_YUV_LINEAR_T **ppYuvImg);
INT32 JDEC_MwRowStart				(JPEGHANDLE hDec);

/* JMEM group */
JPEGHANDLE JMEM_AllocInstance		(void);
INT32 JMEM_New						(JPEGHANDLE hMem, ENUM_JPEG_CLASS_T eMemClass);
void JMEM_FreeInstance				(JPEGHANDLE hMem);

void* JMEM_Alloc					(SIZE_T zByteSize);
void JMEM_Free						(void* pvMem);

INT32 JDEC_MwDecodeRow(JPEGHANDLE hDec);
INT32 JDEC_MwDecodeEnd(JPEGHANDLE hDec);
INT32 JDEC_MwDecodeSetup(JPEGHANDLE hFlow);
INT32 JDEC_MwGetJfif(JPEGHANDLE hDec, JPEGHANDLE *phJfif);
INT32 JDEC_MwFillBuffer(JPEGHANDLE hDec, void **ppvBuf, UINT32 *pu4Size, UINT32 u4AddSize);
INT32 JDEC_MwSetRowBuffer(JPEGHANDLE hDec, void * const * ppvBuffer);
INT32 JDEC_MwGetMemLayout(JPEGHANDLE hDec, JMEM_MW_T **pprMem);

void JFIF_UnusedStatics(void);

#endif //DRV_JPG_H
