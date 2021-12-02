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
/*
jfif header
*/

#if ! defined(JPEG_JFIF_H)
#define JPEG_JFIF_H

#include "jdec_com_common.h"
#include "jpg_if.h"

#ifndef MT8520
extern void x_memset(void *pv_to, UINT8 ui1_c, SIZE_T z_l);
extern void x_memcpy(void *pv_to, const void *pv_from, SIZE_T z_l);
#else
#include "x_common.h"
#endif


//typedef void* JPEGHANDLE;

#define MAX_JPEG_COMPONENT      4
#define D_MAX_NUM_APP0          2
#define D_MAX_NUM_SCAN          32

/* JPEG marker codes */
#if 0 // winton
typedef enum
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
} JDEC_MARKER_CODE_T;

#endif

/* jpeg format */
typedef enum
{
	E_JDEC_UNKNOWN_FORMAT,
	E_JDEC_BASELINE,
	E_JDEC_EXTENDED_SEQUENTIAL_HUFFMAN,
	E_JDEC_PROGRESSIVE_HUFFMAN,
	E_JDEC_EXTENDED_SEQUENTIAL_ARITHMETIC,
	E_JDEC_PROGRESSIVE_ARITHMETIC,
	E_JDEC_LOSSLESS_HUFFMAN,
	E_JDEC_DIFFERENTIAL_SEQ_HUFFMAN,
	E_JDEC_DIFFERENTIAL_PROGRESSIVE_HUFFMAN,
	E_JDEC_DIFFERENTIAL_LOSSLESS_HUFFMAN,
	E_JDEC_RESERVED_FOR_EXTENSIONS,
	E_JDEC_LOSSLESS_ARITHMETIC,
	E_JDEC_DIFFERENTIAL_SEQUENTIAL_ARITHMETIC,
	E_JDEC_DIFFERENTIAL_PROGRESSIVE_ARITHMETIC,
	E_JDEC_UNSUPPORT_FORMAT
} JDEC_FORMAT_T;


/* component info in SOF marker */
typedef struct _JDEC_SOF_COMP_T
{
	UINT8		u1ComponentId;
	UINT8		u1HSampFactor;
	UINT8		u1VSampFactor;
	UINT8		u1QuantTblNo;
} JDEC_SOF_COMP_T;

/* SOF data */
typedef struct _JDEC_JFIF_SOF_T
{
	BOOL					fgSOF;			/* indicate that already have an SOF marker */
	JDEC_FORMAT_T	        eJpegFormat;
	UINT8					u1DataPrecision;
	UINT16					u2ImageHeight;
	UINT16					u2ImageWidth;
	UINT8					u1NumComponents;
	UINT8					au1MapId2Index[256];
	JDEC_SOF_COMP_T			arSofComp[MAX_JPEG_COMPONENT];
} JDEC_JFIF_SOF_T;

/* raw de-huffman table */
typedef struct _JDEC_DHT_HUFF_TBL_T
{
	UINT8 au1Bits[17];
	UINT8 au1HuffVal[256];
} JDEC_DHT_HUFF_TBL_T;

/* DHT data */
typedef struct _JDEC_JFIF_DHT_T
{
	UINT32					u4NumDcTbl;
	UINT32					u4NumAcTbl;
	UINT32					fgDcTblLoaded;	/* bit mask for loaded dc table */
	UINT32					fgAcTblLoaded;	/* bit mask for loaded ac table */
	JDEC_DHT_HUFF_TBL_T		arDcTbl[4];
	JDEC_DHT_HUFF_TBL_T		arAcTbl[4];
} JDEC_JFIF_DHT_T;

/* DQT data */
typedef struct _JDEC_JFIF_DQT_T
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
} JDEC_JFIF_DQT_T;



typedef struct _JDEC_JFIF_SOS_T
{
	UINT8				u1CompInScan;
	UINT8				au1CompId[MAX_JPEG_COMPONENT];
	UINT8				au1DcId[MAX_JPEG_COMPONENT];
	UINT8				au1AcId[MAX_JPEG_COMPONENT];
	UINT8				u1Ss, u1Se, u1Ah, u1Al;
} JDEC_JFIF_SOS_T;

#if 0 // winton
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
#endif

/* APP0 - jfif, jfxx */
typedef struct _JDEC_JFIF_APP0_T
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
} JDEC_JFIF_APP0_T;

/* APP1 - exif */
typedef struct _JDEC_JFIF_APP1_T
{
	BOOL				fgAPP1;		/* APP1 marker found */
	UINT8				*pu1ExifData;
	/* TODO: implement thumbnail picture in 1st IFD here */
	/* TODO: implement partial 0th IFD tag here */
} JDEC_JFIF_APP1_T;

#if 0 // winton
typedef struct _JPEG_JFIF_HEADER_T
{
    UINT8				*pu1JpegHdr;            ///< memory address of jpeg header data
	UINT32				u4ImageByteSize;        ///< file size of this jpeg image
	UINT8				*pu1CurrRead;           ///< current read pointer
	UINT8				*pu1CurrWrite;          ///< current write pointer, new
	UINT32              u4FifoStart;            ///< Fifo Start Address
	UINT32              u4FifoEnd;              ///< Fifo Start Address
	UINT32              u4HasReadData;          ///< has read data for previous parse header, new
	void                *pvCbInst;              ///< request header bitstream callback function instance

	BOOL				fgPrsResult;		    ///< jpeg image parsed successfully
	BOOL				fgFatalError;		    ///< fatal error found and can not decode
	BOOL				fgThumbnail;		    ///< thumbnail image available (app0 or app1 marker)

	BOOL				fgSOI;				    ///< indicate that already have an SOI marker
    BOOL                fgSOF;                  ///< indicate that already have an SOF marker
    BOOL                fgDQT;                  ///< indicate that already have an DQT marker
	BOOL				fgEOI;                  ///< indicate that already have an EOI marker

	JDEC_JFIF_SOF_T		rSOF;       	        ///< SOF - start of frame
	JDEC_JFIF_DHT_T		rDHT;                   ///< DHT - de-huffman table
	JDEC_JFIF_DQT_T		rDQT;                   ///< DQT - de-quantization table
	UINT16				u2RestartInterval;      ///< DRI - restart interval
	UINT32				u4NumApp0;              ///< APP0 - application0 number
	JDEC_JFIF_APP0_T	arAPP0[D_MAX_NUM_APP0]; ///< APP0 - application0 data
	JDEC_JFIF_APP1_T	rAPP1;                  ///< APP1 - application1
	JDEC_JFIF_SOS_T		rSOS;                   ///< SOS information
	UINT32              u4BitStmSrtOfst;        ///< the file offset of jpeg bit stream start
} JPEG_JFIF_HEADER_T;
#endif

/* JFIF group - jpeg header parser */
JPEGHANDLE JFIF_AllocInstance		(void);
	// allocate an jpeg jfif header instance
void JFIF_FreeInstance				(JPEGHANDLE hInstance);
	// release an jpeg jfif header instance
INT32 JFIF_New8520						(JPEGHANDLE hInstance, void *pvJpegImage, UINT32 u4JpegSize);
	// start and parse a jpeg file image store in memory
SIZE_T JFIF_GetHeaderSize			(void);

#endif //JPEG_JFIF_H
