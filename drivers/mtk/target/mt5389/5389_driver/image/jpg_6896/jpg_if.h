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
#if ! defined(JPG_IF_H)
#define JPG_IF_H

#include "x_lint.h"
//#include "gfxsc_if.h"
#include "img_lib_if.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
#ifdef MMP_ARGB_MODE
//#define CC_SWGAMMA
#endif

#if defined(CC_MT5387) && !defined(CC_MT5363)
#define JPEG_VLD_V2
#endif

#if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396) ||defined(CC_MT5389)
#define JPEG_VLD_V3
#endif

#if !defined(JPEG_VLD_V2) && !defined(JPEG_VLD_V3)
#define JPEG_VLD_V1
#endif

#if	defined(CC_MT5365) || defined(CC_MT5395)
#define JPG_CLK_FROM_GRAPHIC
#endif

#ifdef CC_MT5395
#define JPG_CLK_FROM_GRAPHIC_5395_PLL
#endif

#if	defined(CC_MT5396) || defined(CC_MT5368) ||defined(CC_MT5389)
#define JPG_CLK_FROM_JPGDEC_CKCFG
#endif

#define CC_JPEG_PIC_MODE
#define THUMBNAIL_SHOW
//#define JPEG_AUTO_TEST
//#define PNG_VFY_AUTO_TEST
//#define PNG_DRV_FLOW_TEST
//#define JPG_SEMI_HOST_BY_USB
//#define PJPG_SEMI_HOST_BY_USB

//#define __MODEL_slt__

#ifndef  PNG_DRV_FLOW_TEST
#ifdef __MODEL_slt__
#define PNG_DRV_FLOW_TEST
#endif
#endif
//#define JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
//#define JPEG_AUTO_TEST_PERFORMANCE    //for MJPEG
//#define JDEC_HW_IO_MMU

#ifndef JPEG_AUTO_TEST
#ifdef __MODEL_slt__
#define JPEG_AUTO_TEST
#endif
#endif
//#define JPEG_AUTO_TEST
/********************************************************************
	TYPE DEFINITION
********************************************************************/

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

typedef enum _ENUM_JPEG_COLOR_FMT_T
{
      E_JPEG_YUV444,
      	E_JPEG_YUV422V,
      	E_JPEG_YUV422,
      	E_JPEG_YUV420,
      	E_JPEG_COLOR_FMT_UNKNOWN
}ENUM_JPEG_COLOR_FMT_T;

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

typedef struct _JPEG_HEADER_INFO
{
	UINT32	u4Width;  //orig image width
	UINT32	u4Height; //orig image width
	UINT32	u4TotalCol;
	UINT32	u4TotalRow;
	UINT32   u4NumScan;
	UINT32   au4RowSize[3];	
	UINT32   au4RowWidth[3]; //display image pitch, 16 bytes alignment	
	UINT32   au4RowHeight[3];	
	UINT32   u4ColorFMT;
} JPEG_HEADER_INFO_T;


typedef struct _JMEM_YUV_LINEAR_T
{
	JMEM_BASE_T		rBase;
	UINT32                u4ColorFMT;	
	UINT32			au4McuAdv[3];
	UINT32			au4RowAdv[3];
	UINT32			au4AllocSize[3];
	UINT32                au4RowSize[3];
	UINT32                au4RowWidth[3];		
	UINT32                au4RowHeight[3];	
	void*			pvComp[3];
	/* temp buffers */
	void*			pvCoefBuffer[3];
	void*			pvComp1[3];
} JMEM_YUV_LINEAR_T;

typedef struct _JMEM_MB_MTK_T
{
	JMEM_BASE_T		rBase;
} JMEM_MB_MTK_T;

typedef struct _JMEM_MW_T
{
	JMEM_BASE_T		rBase;
	UINT32                u4ColorFMT;		
	UINT32			au4McuAdv[3];
	UINT32			au4RowAdv[3];
	UINT32			au4RowSize[3];
	UINT32                au4RowWidth[3];
	UINT32                au4RowHeight[3];	
	INT32			i4McuRows;
	void*			pvComp[3];
	JPEGHANDLE		hMem;			/* output handle - for memory cleanup */
       //for 539x imgrz
	void*			pvComp1[3];
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

// jpeg decode mode
typedef enum _ENUM_JPEG_DEC_MODE_T
{
    E_JPG_DEC_MODE_NONE,
    E_JPG_DEC_MODE_BASELINE_PIC,                     ///< Jpeg baseline picture, decode whole picture
    E_JPG_DEC_MODE_BASELINE_MCU,                     ///< Jpeg baseline picture, decode MCU
    E_JPG_DEC_MODE_BASELINE_MCU_ROW,                 ///< Jpeg baseline picture, decode MCU row
    E_JPG_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT,    ///< Jpeg progressive picture, decode MCU milti-collect
    E_JPG_DEC_MODE_PROGRESSIVE_MCU_ENHANCE,           ///< Jpeg progressive picture, decode MCU enhance decoding
    E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ,             // jpeg MCU-row + resizer 
    E_JPG_DEC_MODE_BASELINE_PIC_RZ,                  // jpeg PIC + resizer 
    E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT		//jpg video format output(Y C two buffer)
} ENUM_JPEG_DEC_MODE_T;

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
	JPEG_JFIF_SOS_T		rSOS;
	/* EOI */
	BOOL				fgEOI;					/* EOI reached */
	UINT8				*pu1EOI;				/* point to EOI marker */
	/* LAST SCAN */
	UINT32				au4CompLastScan[D_MAX_JPEG_COMPONENT];
	BOOL 				fgEOF;
} JPEG_JFIF_HEADER_T;

#ifdef THUMBNAIL_SHOW
typedef struct
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4BPP;
    UINT32 u4ThumbnailLength;
    UINT32 u4Orientation;
    UINT32 u4APP1Length;
    UINT32 u4APP2Length;
    UINT8 au1Maker[20];
    UINT8 au1Model[30];
    UINT8 au1Date[30];
    UINT8 *pu1Thumbnail;
    UINT8 *pu1APP0; 
    UINT8 *pu1APP1; 
    UINT8 *pu1APP2; 
    BOOL fgProgressive;
} IMGMETA_T;
#endif

//for verification only

typedef struct
{
    UINT32                  u4Left;
    UINT32                  u4Top;
    // u4Width * bytes-per-pixel MUST align with 16 bytes
    UINT32                  u4Width;
    UINT32                  u4Height;
    UINT32                  u4ScaleWidth;
    UINT32                  u4ScaleHeight;
    // pu1FrameBaseAddr MUST align with 16 bytes
    UINT8*                  pu1FrameBaseAddr;
} DRV_OSD_T;

typedef struct
{
    UINT32 u4DisplayW;
    UINT32 u4DisplayH;
    UINT32 u4DispPitch;
    ENUM_IMG_DISP_COLOR_MODE u4DisplayClrMode;
    UINT32 u4Rotate;
    UINT32 au4FactorH[3];
    UINT32 au4FactorW[3];
    
    UINT32 u4ScaleFMT;
    UINT32 u4ScaleW;
    UINT32 u4ScaleH;
    UINT32 u4ScaleRowH;
    UINT32 u4DecodedRow;
	UINT32 u4RowsPerSeg;
	
	UINT32 u4StartX[2];
	UINT32 u4StartY[2];

	UINT8 *pu1DisplayStart_temp[2];	//for RGB format, temp framebuffer for gfx hw to convert color model
	UINT8 *pu1DisplayStart[2];
	
	//GFXSC_SEG_RECORD_T arJpgSeg[3];	///< gfx segment for each color component
	UINT32 u4VertSegLen[3]; 			///< gfx vertical segment height for each color component
} IMG_LIB_DISP_INFO_T;

typedef struct
{
    //void *pavComp[3];	//-- ROW_BUFFER_NUM for display multi buffer, 3 for three component
    void *pavComp[6];	//-- ROW_BUFFER_NUM for display multi buffer, 3 for three component, 3 for 5391 imgrz
    UINT8 *pau1ScDstAddr[3];
	UINT8 *pau1ScDst2Addr[2];
    UINT32 u4ScHLimit;
    UINT32 u4RzTempBuf;
    UINT32 u4CurrentUse;
} IMG_LIB_MEM_LAYOUT_T;

typedef struct
{
	JPEGHANDLE hCurDec;
	JPEG_HEADER_INFO_T rHdrInfo;	
	IMG_LIB_MEM_LAYOUT_T rMem;
	IMG_LIB_DISP_INFO_T rDispInfo;
	UINT32 u4RowIdx;
	UINT32 u4SegCnt; 	
	void *pvTag;		//for MW interface
	UINT8 u1MemClass;
	BOOL bStopDec;
	BOOL bIsEOF;
    BOOL bUseMw;
} IMG_LIB_INFO_T;
/*
//t32 host command
enum
{
	E_T32_NOP,
	E_T32_EXEC,
	E_T32_RESTART,
	E_T32_EXIT,
	E_T32_EXEC_RELOAD_SYM,
	E_T32_DETECT_HOST,
	E_T32_END
};
*/
//low level commands
INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
INT32 T32_HostDetect(void);
INT32 T32_HostSaveData(CHAR *szFileName, void *pvMemoryStart, UINT32 u4ByteSize);
INT32 T32_HostLoadData(CHAR *szFileName, void *pvMemoryStart);
INT32 T32_HostRestart(void);
INT32 T32_HostExit(void);

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
INT32 JDEC_MwDecodeSetup(JPEGHANDLE hFlow, ENUM_JPEG_CLASS_T eMemClass);
VOID JDEC_SetDecodeMode(ENUM_JPEG_DEC_MODE_T eMode, ENUM_IMG_BUFFER_TYPE eMemClass);
VOID JDEC_SetStop(BOOL fgJdecStop);
INT32 JDEC_MwGetJfif(JPEGHANDLE hDec, JPEGHANDLE *phJfif);
INT32 JDEC_MwFillBuffer(JPEGHANDLE hDec, void **ppvBuf, UINT32 *pu4Size, UINT32 u4AddSize);
INT32 JDEC_MwSetRowBuffer(JPEGHANDLE hDec, void * const * ppvBuffer);
INT32 JDEC_MwGetMemLayout(JPEGHANDLE hDec, void/*JMEM_MW_T*/ **pprMem);
INT32 JDEC_MwHeaderParse(JPEGHANDLE hDec, UINT8 *pBs_start, UINT32 bufferSize, JPEG_HEADER_INFO_T *jpgHdrInfo);
void JDEC_MwSetRingBuf(JPEGHANDLE hFlow, UINT8 *pBs_start, UINT32 bufferSize);
void JDEC_FillBsBuf_End(JPEGHANDLE hFlow, UINT32 entry1_size, UINT32 entry2_size, BOOL bEOF);
void JDEC_SetFileSize(JPEGHANDLE hFlow,UINT32 u4FileSize);

void JFIF_UnusedStatics(void);

INT32 IMG_GetOutputBuffer(void **ppmem);
void *IMG_OSD_create(UINT32 dispColorMode);

INT32 JDEC_RzRow(UINT32 u4OutAddrY, UINT32 u4OutAddrCb, UINT32 u4OutAddrCr, UINT32 u4RowIdx, 
                UINT32 u4TotalRows, UINT32 u4Comp, BOOL fgPreload);
INT32 JDEC_RzRowMode_Vfy(UINT32 u4YSrcBufPitch,UINT32 u4CSrcBufPitch, UINT32 u4SrcImgWidth,UINT32 u4SrcImgHeight,
			UINT32 u4OutBank0AddrY, UINT32 u4OutBank0AddrCb, UINT32 u4OutBank0AddrCr, 
			UINT32 u4OutBank1AddrY, UINT32 u4OutBank1AddrCb, UINT32 u4OutBank1AddrCr, 
			UINT32 au4RowWidth[3], UINT32 au4RowHeight[3],
			UINT32 u4DestBufPitch, UINT32 u4DestBufWidth,UINT32 u4DestBufHeight,
			UINT32 u4DestBufSA1,UINT32 u4DestBufSA2,
			UINT32 u4DestImgXOff,UINT32 u4DestImgYOff, UINT32 u4DisplayClrMode, 			
			UINT32 u4Comp, BOOL fgPreload, BOOL fgLastScan, UINT32 u4RowIdx, UINT32 u4TotalRows);
INT32 JDEC_RzRowMode(
			UINT32 u4OutBank0AddrY, UINT32 u4OutBank0AddrCb, UINT32 u4OutBank0AddrCr, 
			UINT32 u4OutBank1AddrY, UINT32 u4OutBank1AddrCb, UINT32 u4OutBank1AddrCr, 
			UINT32 u4Comp, BOOL fgPreload, BOOL fgLastScan, UINT32 u4RowIdx, UINT32 u4TotalRows);
INT32 JDEC_RzScanMode_Vfy(UINT32 u4YSrcBufPitch,UINT32 u4CSrcBufPitch, UINT32 u4SrcImgWidth,UINT32 u4SrcImgHeight,
			UINT32 u4OutBank0AddrY, UINT32 u4OutBank0AddrCb, UINT32 u4OutBank0AddrCr, 
			UINT32 u4OutBank1AddrY, UINT32 u4OutBank1AddrCb, UINT32 u4OutBank1AddrCr, 
			UINT32 au4RowWidth[3], UINT32 au4RowHeight[3],
			UINT32 u4DestBufPitch, UINT32 u4DestBufWidth,UINT32 u4DestBufHeight,
			UINT32 u4DestBufSA1,UINT32 u4DestBufSA2,
			UINT32 u4DestImgXOff,UINT32 u4DestImgYOff, UINT32 u4DisplayClrMode, 			
			UINT32 u4Comp, BOOL fgPreload, BOOL fgLastScan);        
INT32 JDEC_RzScanMode(
			UINT32 u4OutBank0AddrY, UINT32 u4OutBank0AddrCb, UINT32 u4OutBank0AddrCr, 
			UINT32 u4OutBank1AddrY, UINT32 u4OutBank1AddrCb, UINT32 u4OutBank1AddrCr, 	
			UINT32 u4Comp, BOOL fgPreload, BOOL fgLastScan);      
VOID JDEC_MwSaveRzInfo(IMG_LIB_INFO_T *prImg, ENUM_IMG_ROTATE_PHASE eRotateDegree);
UINT32 JDEC_GetProgress(JPEGHANDLE hFlow);
#ifdef THUMBNAIL_SHOW
INT32 JFIF_GetImgMetaData(IMGMETA_T *pMeta,
                                   UINT8 *pu1InBuf, UINT32 u4InSize);
void JDEC_HandleThumbnail(JPEGHANDLE hFlow);
#endif
INT32 JDEC_SwitchToProgDec(JPEGHANDLE hFlow , IMG_LIB_MEM_LAYOUT_T *prMem);
VOID JDEC_SWGamma(UINT8 *pu1Addr, IMG_LIB_DISP_INFO_T *prDispInfo, UINT32 u4X, UINT32 u4Y);
void JDEC_BuildTable(void);
void VLDJPG_MMU_init(void);
UINT32 JDEC_Check_ErrorType(void);
BOOL JDEC_CkgenOnOff(BOOL fgEnable);
#endif //JPG_IF_H

