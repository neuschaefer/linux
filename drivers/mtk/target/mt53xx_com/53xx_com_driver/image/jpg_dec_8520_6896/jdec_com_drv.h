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
#if ! defined(DRV_JPG_H)
#define DRV_JPG_H

#include "jdec_com_common.h"
#include "jdec_com_jfif.h"

#ifndef MT8520
typedef unsigned int          SIZE_T;
#endif

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

//#define D_MAX_JPEG_HW_COMP		3
#define D_MAX_NUM_APP0			2
#define D_MAX_NUM_SCAN			32

#define D_JPG_STREAM_SIZE		(1024 * 64)


typedef struct _JPEG_JFIF_HEADER_T
{
	JPEG_BASE_CLASS_T	rParent;
	/* memory address of jpeg image */
	UINT8				*pu1JpegImage;
	/* file size of this jpeg image */
	UINT32				u4ImageByteSize;
	/* ring buffer support */
    BOOL			    fgRingBuffer;		/* in ring buffer mode or not */
	UINT32			    u4RingBufSize;		/* ring buffer size */
	UINT8			    *pu1RingBufStart;	/* ring buffer start address */
	UINT32			    u4RingRP;			/* read pointer */
	INT32			    i4RingData;			/* data size in ring buffer, might be negative */
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
	JDEC_JFIF_SOF_T		rSOF;
	/* DHT - de-huffman table */
	JDEC_JFIF_DHT_T		rDHT;
	/* DQT - de-quantization table */
	JDEC_JFIF_DQT_T		rDQT;
	/* DRI - restart interval */
	UINT16				u2RestartInterval;
	/* FMT - format flag */
	UINT32				fgFormat;			/* JFIF or Exif or ... */
	/* APP0 */
	UINT32				u4NumApp0;
	JDEC_JFIF_APP0_T	arAPP0[D_MAX_NUM_APP0];
	/* APP1 */
	JDEC_JFIF_APP1_T	rAPP1;
	/* SOS - start of scan */
	UINT32				u4NumScan;
	UINT32				u4CurrScan;
	JDEC_JFIF_SOS_T		arSOS[D_MAX_NUM_SCAN];
	JDEC_JFIF_SOS_T		rSOS;
	/* EOI */
	BOOL				fgEOI;					/* EOI reached */
	UINT8				*pu1EOI;				/* point to EOI marker */
	/* LAST SCAN */
	UINT32				au4CompLastScan[MAX_JPEG_COMPONENT];
    BOOL fgSOF;
    BOOL fgDQT;
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
INT32 VLDJPG_New					(JPEGHANDLE hInstance);
	// start hardware decoder, init data structure (allocate vld0)

/* decode flow */
INT32 JPG_DecodeYUV					(void *pvJpegImage, UINT32 u4Size, YUV_BLOCK_T *prYUV);

/* flw2 */
JPEGHANDLE JDEC_AllocInstance		(void);
INT32 JDEC_New						(JPEGHANDLE hDec);
void JDEC_FreeInstance				(JPEGHANDLE hDec);
INT32 JDEC_DecodeYUV				(JPEGHANDLE hDec, void *pvImage, SIZE_T zByteSize, JMEM_YUV_LINEAR_T **ppYuvImg);

/* JMEM group */
JPEGHANDLE JMEM_AllocInstance		(void);
INT32 JMEM_New						(JPEGHANDLE hMem, ENUM_JPEG_CLASS_T eMemClass);
void JMEM_FreeInstance				(JPEGHANDLE hMem);

void* JMEM_Alloc					(SIZE_T zByteSize);
void JMEM_Free						(void* pvMem);

INT32 JDEC_MwRowStart(JPEGHANDLE hDec);
INT32 JDEC_MwDecodeRow(JPEGHANDLE hDec);
INT32 JDEC_MwDecodeEnd(JPEGHANDLE hDec);
INT32 JDEC_MwDecodeSetup(JPEGHANDLE hFlow);
INT32 JDEC_MwGetJfif(JPEGHANDLE hDec, JPEGHANDLE *phJfif);
INT32 JDEC_MwFillBuffer(JPEGHANDLE hDec, void **ppvBuf, UINT32 *pu4Size, UINT32 u4AddSize);
INT32 JDEC_MwSetRowBuffer(JPEGHANDLE hDec, void **ppvBuffer);
INT32 JDEC_MwGetMemLayout(JPEGHANDLE hDec, JMEM_MW_T **pprMem);


#endif //DRV_JPG_H
