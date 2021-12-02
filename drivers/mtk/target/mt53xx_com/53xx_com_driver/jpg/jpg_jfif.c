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
#include "jpg_common.h"
#include "jpg_if.h"
#include "jpg_jfif.h"
#include "jpg_debug.h"
#include "img_lib_if.h"

#define ADVANCE_BYTES(NBYTES) \
	/* this while 0 is safe */ \
	LINT_SUPPRESS_NEXT_STATEMENT(717) \
	do \
	{ \
		i4Ret = _jfifAdvanceBytes(prInstance, NBYTES); \
		if ((INT32)E_JPG_OK != i4Ret) \
		{ \
			return i4Ret; \
		} \
	} while (0)

#define JPEG_READ_BYTE(PINSTANCE, NTH_BYTE) (PINSTANCE->pu1CurrRead[NTH_BYTE])
#define JPEG_READ_2BYTES(PINSTANCE, NTH_BYTE) ((JPEG_READ_BYTE(PINSTANCE, NTH_BYTE) << 8) | JPEG_READ_BYTE(PINSTANCE, (NTH_BYTE) + 1))

/* private interfaces */
//static UINT8  _jfifReadByte			(JPEG_JFIF_HEADER_T *prInstance);
	/* read 1 byte from jpeg stream */
//static UINT16 _jfifRead2Bytes		(JPEG_JFIF_HEADER_T *prInstance);
	/* read 2 bytes from jpeg stream */
//static INT32  _jfifSkipBytes		(JPEG_JFIF_HEADER_T *prInstance, INT32 i4Bytes);
	/* skip i4Bytes bytes in jpeg stream */

/* private interfaces */
static INT32 _jfifAdvanceBytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Bytes);
	/* advance read pointer n bytes */
static INT32 _jfifCheckInstance		(const JPEG_JFIF_HEADER_T *prInstance);
	/* check instance */
static INT32 _jfifParse				(JPEG_JFIF_HEADER_T *prInstance);
	/* parse all marker in jpeg file */
static INT32 _jfifParseProgScan		(JPEG_JFIF_HEADER_T *prInstance);
	/* parse SOS markers in */
static INT32 _jfifParseScan			(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Scan);
	/* parse all marker in jpeg file */
static INT32 _jfifIsMarker			(const JPEG_JFIF_HEADER_T *prInstance);
	/* check if current read pointer points to an valid marker */
static INT32 _jfifPeekMarker		(JPEG_JFIF_HEADER_T *prInstance, UINT16 *pu2Marker);
	/* get next marker (read pointer stops on this marker) */
static INT32 _jfifSkipMarker		(JPEG_JFIF_HEADER_T *prInstance);
	/* skip current marker */
static INT32 _jfifDispatchMarker	(JPEG_JFIF_HEADER_T *prInstance, UINT16 u2Marker);
	/* dispatch this marker to proper handler routine */
static INT32 _jfifProcSOI			(JPEG_JFIF_HEADER_T *prInstance);
	/* process start of image */
static INT32 _jfifProcSOF			(JPEG_JFIF_HEADER_T *prInstance);
	/* process start of frame */
static INT32 _jfifProcSOS			(JPEG_JFIF_HEADER_T *prInstance);
	/* process start of scan */
static INT32 _jfifProcDHT			(JPEG_JFIF_HEADER_T *prInstance);
	/* process de-huffman table */
static INT32 _jfifProcDQT			(JPEG_JFIF_HEADER_T *prInstance);
	/* process de-quantization table */
static INT32 _jfifProcDRI			(JPEG_JFIF_HEADER_T *prInstance);
	/* process restart interval marker */
static INT32 _jfifProcAPP0			(JPEG_JFIF_HEADER_T *prInstance);
	/* process standard APP0 "JFIF" marker (and "JFXX" extension) */
static INT32 _jfifProcAPP1			(JPEG_JFIF_HEADER_T *prInstance);
	/* process standard APP1 "Exif" marker */
static INT32 _jfifProcAPP14			(JPEG_JFIF_HEADER_T *prInstance);
	/* process APP14 */
static INT32 _jfifProcAPPother		(JPEG_JFIF_HEADER_T *prInstance);
	/* process all non-supported APPn format */
static INT32 _jfifProcEOI			(JPEG_JFIF_HEADER_T *prInstance);
	/* process end of image marker */



//----------------------------------------------------------

/* private interfaces */

static INT32 _jfifAdvanceBytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Bytes)
{

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (((UINT32)&prInstance->pu1CurrRead[u4Bytes]) >=
		((UINT32)&prInstance->pu1JpegImage[prInstance->u4ImageByteSize]))
	{
	    if (prInstance->fgEOF)
	    {
		return -(INT32)E_JPG_ERR_PASS_END;
	}
	    else                                    //not EOF, need more data for parsing
	    {
	        UINT32 u4Remind;
	        UINT32 u4Used;

	        u4Remind = (!prInstance->u4RingRP) ? (UINT32)0 : \
	                    (UINT32)(prInstance->pu1RingBufStart + prInstance->u4RingBufSize - prInstance->pu1CurrRead);
	        u4Used = (UINT32)prInstance->pu1CurrRead - (UINT32)prInstance->pu1RingBufStart + u4Remind;
	        prInstance->u4RingRP = u4Used;
	        prInstance->u4RingData -= u4Used;	        
    	    IMG_BsBufManager();
    	    prInstance->pu1CurrRead = (prInstance->pu1CurrRead + u4Bytes) - prInstance->u4RingBufSize;
	    }
	}
    else
    {
	prInstance->pu1CurrRead += u4Bytes;
    }

	return (INT32)E_JPG_OK;
}

/* instance check */
static INT32 _jfifCheckInstance		(const JPEG_JFIF_HEADER_T *prInstance)
{
	if (prInstance == NULL)
	{
		// null instance
		return -(INT32)E_JPG_ERR_PARAM;
	}

	if ((prInstance->rParent.eJpegClass		!= E_JC_JFIF_HEADER) ||
		(prInstance->rParent.zObjectSize	!= sizeof(JPEG_JFIF_HEADER_T)))
	{
		// not an jfif header instance
		return -(INT32)E_JPG_ERR_PARAM;
	}

	if (!prInstance->fgInited)
	{
		// didnt init
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}

	if (prInstance->fgFatalError)
	{
		// an fatal error happened
		return -(INT32)E_JPG_ERR_FATAL;
	}

	return (INT32)E_JPG_OK;
}

/* parse header markers in jpeg file */
static INT32 _jfifParse				(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2JfifMarker;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (prInstance->fgParsed)
	{
		// already parsed
		return -(INT32)E_JPG_ERR_ALREADY;
	}

	// parse until EOI
	for(;;)
	{
		i4Ret = _jfifPeekMarker(prInstance, &u2JfifMarker);
		if ((INT32)E_JPG_OK == i4Ret)
		{
			// marker found
			i4Ret = _jfifDispatchMarker(prInstance, u2JfifMarker);

			if (((UINT16)M_EOI == u2JfifMarker) ||
				((UINT16)M_SOS == u2JfifMarker))
			{
				// stop condition reached - EOI
				if (//((prInstance->u4NumApp0 != 0) || (prInstance->rAPP1.fgAPP1)) &&
					//(prInstance->rDQT.fgDQT) &&
					/*(prInstance->rDHT.fgDcTblLoaded) &&
					(prInstance->rDHT.fgAcTblLoaded || (prInstance->rSOF.eJpegFormat != E_JPG_BASELINE)) &&*/
					(prInstance->rSOF.fgSOF) &&
					(prInstance->fgSOI) &&
					(prInstance->u4NumScan > 0) &&
					(!prInstance->fgFatalError))
				{
					prInstance->fgParsed = TRUE;
				}
				else
				{
					prInstance->fgParsed = FALSE;
				}
				break;
			}

			if ((INT32)E_JPG_OK != i4Ret)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return i4Ret;
}

/* parse rest markers in jpeg file */
static INT32 _jfifParseProgScan		(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2JfifMarker;
	UINT32 u4Scan;
	INT32 i4ProgDqtCnt = 0, i4ProgDhtCnt = 0;
	BOOL fgFirstScanMarker = TRUE;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	u4Scan = prInstance->u4CurrScan;


	// parse until EOI
	for(;;)
	{
		i4Ret = _jfifPeekMarker(prInstance, &u2JfifMarker);
		if ((INT32)E_JPG_OK == i4Ret)
		{
			// marker found
			if (fgFirstScanMarker &&
				((u2JfifMarker < (UINT16)M_RST0) || (u2JfifMarker > (UINT16)M_RST7)))
			{
				if (u2JfifMarker != (UINT16)M_SOS)
				{
					prInstance->arSOS[u4Scan].pu1ScanTableStart = prInstance->pu1CurrRead;
				}
				fgFirstScanMarker = FALSE;
			}

			// parse marker
			if ((u2JfifMarker != (UINT16)M_DQT) &&
				(u2JfifMarker != (UINT16)M_DHT))
			{
				i4Ret = _jfifDispatchMarker(prInstance, u2JfifMarker);
				if ((UINT16)M_SOS == u2JfifMarker)
				{
					// after sos marker, current scan has been updated
					u4Scan = prInstance->u4CurrScan;
					if ((INT32)E_JPG_OK == i4Ret)
					{
						fgFirstScanMarker = TRUE;
					}
				}
			}
			else
			{
				if (u2JfifMarker == (UINT16)M_DQT)
				{
					i4ProgDqtCnt++;
				}
				if (u2JfifMarker == (UINT16)M_DHT)
				{
					i4ProgDhtCnt++;
				}
				i4Ret = _jfifSkipMarker(prInstance);
			}

			if ((UINT16)M_EOI == u2JfifMarker)
			{
				// stop condition reached - EOI
				Printf("EOI reached\n");
				i4Ret = (INT32)E_JPG_OK;
				break;
			}

			if ((INT32)E_JPG_OK != i4Ret)
			{
				// dispatch error
				break;
			}
		}
		else
		{
			break;
		}
	}

	Printf("last code : %d\n", i4Ret);
	Printf("n app0 : %d\n", prInstance->u4NumApp0);
	Printf("b app1 : %d\n", prInstance->rAPP1.fgAPP1);
	Printf("n scan dqt : %d\n", i4ProgDqtCnt);
	Printf("n scan dht : %d\n", i4ProgDhtCnt);
	Printf("n scan : %d\n", u4Scan);
	Printf("p->curr_scan : %d\n", prInstance->u4CurrScan);
	while (u4Scan--)
	{
		Printf("\tscan [%02d]", u4Scan);
		Printf("\t - comp in scan %d\n", prInstance->arSOS[u4Scan].u1CompInScan);
		Printf("\t - dc id : %d %d %d %d\n",
				prInstance->arSOS[u4Scan].au1DcId[0],
				prInstance->arSOS[u4Scan].au1DcId[1],
				prInstance->arSOS[u4Scan].au1DcId[2],
				prInstance->arSOS[u4Scan].au1DcId[3]);
		Printf("\t - ac id : %d %d %d %d\n",
				prInstance->arSOS[u4Scan].au1AcId[0],
				prInstance->arSOS[u4Scan].au1AcId[1],
				prInstance->arSOS[u4Scan].au1AcId[2],
				prInstance->arSOS[u4Scan].au1AcId[3]);
		Printf("\t - ss %d se %d ah %d al %d\n",
				prInstance->arSOS[u4Scan].u1Ss,
				prInstance->arSOS[u4Scan].u1Se,
				prInstance->arSOS[u4Scan].u1AhAl >> 4,
				prInstance->arSOS[u4Scan].u1AhAl & 0xf);
		Printf("\t - table [0x%08x]\n", (UINT32)prInstance->arSOS[u4Scan].pu1ScanTableStart - (UINT32)prInstance->pu1JpegImage);
		Printf("\t - data  [0x%08x]\n", (UINT32)prInstance->arSOS[u4Scan].pu1ScanDataStart - (UINT32)prInstance->pu1JpegImage);
	}

	return (INT32)E_JPG_OK;
}

/* parse markers before a SOS */
static INT32 _jfifParseScan		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Scan)
{
	INT32 i4Ret = (INT32)E_JPG_OK;
	UINT16 u2JfifMarker;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	// parse until EOI
	for(;;)
	{
		i4Ret = _jfifPeekMarker(prInstance, &u2JfifMarker);
		if ((INT32)E_JPG_OK == i4Ret)
		{
			// parse marker
			// currently M_DQT only, maybe (u2JfifMarker == (UINT16)M_DHT)
			if ((UINT16)M_DQT == u2JfifMarker)
			{
				Printf(" - Found DQT\n");
				i4Ret = _jfifDispatchMarker(prInstance, u2JfifMarker);
			}
			else if ((UINT16)M_DHT == u2JfifMarker)
			{
				Printf(" - Found DHT\n");
				i4Ret = _jfifDispatchMarker(prInstance, u2JfifMarker);
			}
			else if ((UINT16)M_SOS == u2JfifMarker)
			{
				i4Ret = (INT32)E_JPG_OK;
				break;
			}
			else if ((UINT16)M_EOI != u2JfifMarker)
			{
				//Printf("Marker other than DQT DHT in scan : scan(%d) marker(0x%04x)\n",
				//	u4Scan, u2JfifMarker);
                          ADVANCE_BYTES(1); 
				i4Ret = _jfifSkipMarker(prInstance);
			}
			else	// EOI
			{
				Printf("EOI reached ... should not happen here. scan(%d)\n", u4Scan);
				i4Ret = (INT32)E_JPG_OK;
				break;
			}

			if ((INT32)E_JPG_OK != i4Ret)
			{
				// dispatch error
				break;
			}
		}
		else
		{
			break;
		}
	}

	return (INT32)E_JPG_OK;
}



/* check if current read pointer points to an valid marker */
static INT32 _jfifIsMarker			(const JPEG_JFIF_HEADER_T *prInstance)
{
	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if ((UINT32)prInstance->pu1CurrRead >= ((UINT32)prInstance->pu1JpegImage + prInstance->u4ImageByteSize))
	{
		return -(INT32)E_JPG_ERR_PASS_END;
	}

	if (*prInstance->pu1CurrRead == 0xff)
	{
		return (INT32)E_JPG_OK;
	}

	return -(INT32)E_JPG_ERR_NOT_A_MARKER;
}

/* get next marker (read pointer stops on this marker) */
static INT32 _jfifPeekMarker		(JPEG_JFIF_HEADER_T *prInstance, UINT16 *pu2Marker)
{
	INT32 i4Ret;
	UINT16 u2Marker;

	ASSERT(pu2Marker);
	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	while ((i4Ret = _jfifIsMarker(prInstance)) == -(INT32)E_JPG_ERR_NOT_A_MARKER)
	{
		ADVANCE_BYTES(1);
		/*
		i4Ret = _jfifAdvanceBytes(prIntsance, 1);
		if ((INT32)E_JPG_OK != i4Ret)
		{
			break;
		}
		*/
	}

	if ((INT32)E_JPG_OK != i4Ret)
	{
		return i4Ret;
	}

	u2Marker = (JPEG_READ_BYTE(prInstance, 0) << 8) + JPEG_READ_BYTE(prInstance, 1);

	// exception handling
	switch (u2Marker)
	{
	// we can process some special case here
	default:
		break;
	}
	*pu2Marker = u2Marker;

	return (INT32)E_JPG_OK;
}

/* skip current marker */
static INT32 _jfifSkipMarker		(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (_jfifIsMarker(prInstance))
	{
		// although this is not a marker
		// skip marker should return without error
		// due to its function : 'skip marker'
		return (INT32)E_JPG_OK;
	}

	// this is a marker, advance 2 bytes first
	ADVANCE_BYTES(2);

	u2Length = JPEG_READ_2BYTES(prInstance, 0);
	ADVANCE_BYTES(u2Length);

	return (INT32)E_JPG_OK;
}

/* dispatch this marker to proper handler routine */
static INT32 _jfifDispatchMarker	(JPEG_JFIF_HEADER_T *prInstance, UINT16 u2Marker)
{
	INT32 i4Ret;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	switch (u2Marker)
	{
	case M_SOI		:		// start of image
		i4Ret = _jfifProcSOI(prInstance);
		break;
	case M_SOF0		:		// baseline
	case M_SOF1		:		// extended sequential huffman
	case M_SOF2		:		// progressive huffman
	case M_SOF3		:		// lossless huffman
	case M_SOF5		:		// differential sequential huffman
	case M_SOF6		:		// differential progressive huffman
	case M_SOF7		:		// differential lossless huffman
	case M_JPG		:		// reserved jpeg extension
	case M_SOF9		:		// extended sequential arithmetic
	case M_SOF10	:		// progressive arithmetic
	case M_SOF11	:		// lossless arithmetic
	case M_SOF13	:		// differential sequential arithmetic
	case M_SOF14	:		// differential progressive arithmetic
	case M_SOF15	:		// differential lossless arithmetic
		i4Ret = _jfifProcSOF(prInstance);
		if ((INT32)E_JPG_OK != i4Ret)
		{
			//prInstance->rSOF.eJpegFormat	= E_JPG_UNSUPPORT_FORMAT;
			prInstance->fgFatalError		= TRUE;
		}
		break;
	case M_SOS		:		// start of scan
		i4Ret = _jfifProcSOS(prInstance);
		break;
	case M_EOI		:		// end of image
		i4Ret = _jfifProcEOI(prInstance);
		break;
	case M_DHT		:		// de-huffman table
		i4Ret = _jfifProcDHT(prInstance);
		break;
	case M_DQT		:		// de-quantization table
		i4Ret = _jfifProcDQT(prInstance);
		break;
	case M_DRI		:		// restart interval marker
		i4Ret = _jfifProcDRI(prInstance);
		break;
	case M_APP0		:
		i4Ret = _jfifProcAPP0(prInstance);
		break;
	case M_APP1		:
		i4Ret = _jfifProcAPP1(prInstance);
		break;
	case M_APP14	:
		i4Ret = _jfifProcAPP14(prInstance);
		break;
	case M_APP2		:		// NIKON ICC_PROFILE, EXIF FPXR (FlashPix)
	case M_APP3		:
	case M_APP4		:
	case M_APP5		:
	case M_APP6		:
	case M_APP7		:
	case M_APP8		:
	case M_APP9		:
	case M_APP10	:
	case M_APP11	:
	case M_APP12	:
	case M_APP13	:
	case M_APP15	:
		// unsupport marker, just skip it
		i4Ret = _jfifProcAPPother(prInstance);
		// already skiped in _jfifProcAPPother()
		// i4Ret = _jfifSkipMarker(prInstance);
		break;
	default			:
		// unknown marker, just skip it
		// due to unknown, we can not trust the length
		// hence advance through it
		ADVANCE_BYTES(1);
		i4Ret = _jfifSkipMarker(prInstance);
		break;
	}

	return i4Ret;
}

/* process start of image */
static INT32 _jfifProcSOI			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret; /* for ADVANCE_BYTES() error return */

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (prInstance->fgSOI)
	{
		return -(INT32)E_JPG_ERR_MULTI_SOI;
	}

	prInstance->fgSOI = TRUE;
	ADVANCE_BYTES(2);

	return (INT32)E_JPG_OK;
}

#ifdef JPEG_AUTO_TEST_PERFORMANCE
UINT32 mh_width;
UINT32 mh_height;
#endif

/* process start of frame */
static INT32 _jfifProcSOF			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Marker;
	UINT16 u2Length;
	UINT8 u1CompIndex;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (prInstance->rSOF.fgSOF)
	{
		return -(INT32)E_JPG_ERR_MULTI_SOF;
	}


	u2Marker = JPEG_READ_2BYTES(prInstance, 0);	// 0,1
	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(2);	// skip marker length (does not count)

	/*
	 * do baseline or progressive check here, but due to we implement baseline decoder only,
	 * hence these code was marked out
	 */

	switch (u2Marker)
	{
	case M_SOF0		:
		prInstance->rSOF.eJpegFormat = E_JPG_BASELINE;
		break;
	case M_SOF1		:
		prInstance->rSOF.eJpegFormat = E_JPG_EX_SEQ_HUFFMAN;
		break;
	case M_SOF2		:
		prInstance->rSOF.eJpegFormat = E_JPG_PROGRESSIVE_HUFFMAN;
		break;
		//return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF3		:
		prInstance->rSOF.eJpegFormat = E_JPG_EX_SEQ_ARITHMETIC;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF5		:
		prInstance->rSOF.eJpegFormat = E_JPG_PROGRESSIVE_ARITHMETIC;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF6		:
		prInstance->rSOF.eJpegFormat = E_JPG_LOSSLESS_HUFFMAN;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF7		:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFFERENTIAL_SEQ_HUFFMAN;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF9		:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFF_PROG_HUFFMAN;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF10	:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFF_LLESS_HUFFMAN;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF11	:
		prInstance->rSOF.eJpegFormat = E_JPG_RESERVED_FOR_EXTENSIONS;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF13	:
		prInstance->rSOF.eJpegFormat = E_JPG_LOSSLESS_ARITHMETIC;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF14	:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFF_SEQ_ARITHMETIC;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF15	:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFF_PROG_ARITHMETIC;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	default			:
		prInstance->rSOF.eJpegFormat = E_JPG_UNSUPPORT_FORMAT;
		return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
	}

	// SOF0 & SOF1 & SOF2 parsing
	if (u2Length < 8)
	{
		return -(INT32)E_JPG_ERR_INVALID_SOF;
	}

	prInstance->rSOF.u1DataPrecision	= JPEG_READ_BYTE	(prInstance, 2);	// 2
	prInstance->rSOF.u2ImageHeight		= JPEG_READ_2BYTES	(prInstance, 3);	// 3,4
	prInstance->rSOF.u2ImageWidth		= JPEG_READ_2BYTES	(prInstance, 5);	// 5,6
	prInstance->rSOF.u1NumComponents	= JPEG_READ_BYTE	(prInstance, 7);	// 7

        #ifdef JPEG_AUTO_TEST_PERFORMANCE
        mh_height = prInstance->rSOF.u2ImageHeight;
        mh_width = prInstance->rSOF.u2ImageWidth;
        #endif

	ADVANCE_BYTES(8);
	u2Length -= 8;

	//code sync
	if ((prInstance->rSOF.u1NumComponents	<  1)||(prInstance->rSOF.u1NumComponents	>  3))
	{
		return (INT32)E_JPG_ERR_INVALID_COMPONENT;
	}
		
	// start of frame validation test
	if ((prInstance->rSOF.u2ImageHeight		== 0)	||
		(prInstance->rSOF.u2ImageWidth		== 0)	||
		(u2Length != ((UINT16)(prInstance->rSOF.u1NumComponents * 3))))
	{
		return -(INT32)E_JPG_ERR_INVALID_SOF;
	}

	for (u1CompIndex = 0; u1CompIndex < prInstance->rSOF.u1NumComponents; u1CompIndex++)
	{
		UINT8 u1CompId, u1Factor, u1QuantTblNo;
		u1CompId		= JPEG_READ_BYTE(prInstance, 0);
		u1Factor		= JPEG_READ_BYTE(prInstance, 1);
		u1QuantTblNo	= JPEG_READ_BYTE(prInstance, 2);

		prInstance->rSOF.arSofComp[u1CompIndex].u1ComponentId	= u1CompId;
		prInstance->rSOF.au1MapId2Index[u1CompId] = u1CompIndex;
		prInstance->rSOF.arSofComp[u1CompIndex].u1HSampFactor	= u1Factor >> 4;
		prInstance->rSOF.arSofComp[u1CompIndex].u1VSampFactor	= u1Factor & 0xf;
		prInstance->rSOF.arSofComp[u1CompIndex].u1QuantTblNo	= u1QuantTblNo;

		ADVANCE_BYTES(3);
	}

	prInstance->rSOF.fgSOF = TRUE;
	return (INT32)E_JPG_OK;
}

/* process start of scan */
static INT32 _jfifProcSOS			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;
	UINT8 u1CompInScan;
	UINT8 u1CompIndex;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (!prInstance->rSOF.fgSOF)
	{
		return -(INT32)E_JPG_ERR_SOS_BEFORE_SOF;
	}

	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	u1CompInScan = JPEG_READ_BYTE(prInstance, 4);	// 4
	ADVANCE_BYTES(5);

	// sos validation
	if ((u2Length < (UINT16)((u1CompInScan * 2) + 6)) ||
		(u1CompInScan == 0) ||
		(u1CompInScan > D_MAX_JPEG_COMPONENT))
	{
		return -(INT32)E_JPG_ERR_INVALID_SOS;
	}

	// store components in scan
	prInstance->arSOS[prInstance->u4CurrScan].u1CompInScan = u1CompInScan;

	u2Length -= 3;	// skip length(2 bytes), comp in scan(1 byte)

	/*
	sos structure :
		2 bytes - SOS marker
		2 bytes - length
		1 byte  - num comp in scan
		n * 3 bytes - comp data
		1 byte  - ss
		1 byte  - se
		1 byte  - ah al
	*/
	if (prInstance->u4CurrScan != 0)
	{
		x_memcpy(
			&prInstance->arSOS[prInstance->u4CurrScan + 1],
			&prInstance->arSOS[prInstance->u4CurrScan],
			sizeof(JPEG_JFIF_SOS_T));
	}

	prInstance->arSOS[prInstance->u4CurrScan].u4ScanPass = prInstance->u4CurrScan;

	// read component id and table no
	for (u1CompIndex = 0; u1CompIndex < u1CompInScan; u1CompIndex++)
	{
		UINT8 u1CompId;
		UINT8 u1TableId;
		UINT8 u1MapedIndex;
		u1CompId = JPEG_READ_BYTE(prInstance, 0);
		u1TableId = JPEG_READ_BYTE(prInstance, 1);
		ADVANCE_BYTES(2);
		prInstance->arSOS[prInstance->u4CurrScan].au1CompId[u1CompIndex] =
			prInstance->rSOF.au1MapId2Index[u1CompId];

		u1MapedIndex = prInstance->rSOF.au1MapId2Index[u1CompId];
		if (u1MapedIndex >= (UINT8)4)
		{
		    return -(INT32)E_JPG_ERR_INVALID_SOS;
		}
		/* following data use maped index */
		prInstance->arSOS[prInstance->u4CurrScan].au1DcId[u1MapedIndex] = u1TableId >> 4;
		prInstance->arSOS[prInstance->u4CurrScan].au1AcId[u1MapedIndex] = u1TableId & 0xf;
		prInstance->au4CompLastScan[u1MapedIndex] = prInstance->u4CurrScan;
	}

	prInstance->arSOS[prInstance->u4CurrScan].u1Ss		= JPEG_READ_BYTE(prInstance, 0);
	prInstance->arSOS[prInstance->u4CurrScan].u1Se		= JPEG_READ_BYTE(prInstance, 1);
	prInstance->arSOS[prInstance->u4CurrScan].u1AhAl	= JPEG_READ_BYTE(prInstance, 2);
	ADVANCE_BYTES(3);

	prInstance->arSOS[prInstance->u4CurrScan].pu1ScanDataStart = prInstance->pu1CurrRead;
	prInstance->u4CurrScan++;
	prInstance->u4NumScan++;
	return (INT32)E_JPG_OK;
}

/* process de-huffman table */
static INT32 _jfifProcDHT			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret, i;
	UINT16 u2Length;
	UINT8 au1Bits[17];
	UINT8 au1HuffVal[256];

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	for(i=0;i<sizeof(au1HuffVal); i++)
	{
		au1HuffVal[i]=0;
	}
	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(4);

	if (u2Length < (2 + 17))
	{
		return -(INT32)E_JPG_ERR_INVALID_DHT;
	}

	u2Length -= 2;

	while (u2Length > 16)
	{
		UINT32 u4Bits;
		UINT32 u4Count;
		UINT32 u4HuffIndex;
		BOOL fgIsAc;
		UINT8 u1TblIndex;

		x_memset(au1Bits, 0, 17);
		x_memset(au1HuffVal, 0, 256);

		u1TblIndex = JPEG_READ_BYTE(prInstance, 0);
		fgIsAc = (BOOL) (u1TblIndex >> 4);
		//u1TblIndex &= 0xf;
		u1TblIndex &= 0x3;	// at most 4 table each, for dc and ac
		u4Count = 0;

		for (u4Bits = 1; u4Bits < 17; u4Bits++)
		{
			au1Bits[u4Bits] = JPEG_READ_BYTE(prInstance, u4Bits);
			u4Count += au1Bits[u4Bits];
		}
		ADVANCE_BYTES(17);
		u2Length -= 17;

		/*
		if ((INT32)E_JPG_OK != i4Ret)
		{
			return i4Ret;
		}
		*/

		if (u4Count > 256)
		{
			//bogus dht
			u4Count = 256;
		}

		if ((UINT32)u2Length < u4Count)
		{
			//bogus again
			u4Count = u2Length;
		}

		// fill huff value
		for (u4HuffIndex = 0; u4HuffIndex < u4Count; u4HuffIndex++)
		{
			au1HuffVal[u4HuffIndex] = JPEG_READ_BYTE(prInstance, u4HuffIndex);
		}

		u2Length -= u4Count;
		ADVANCE_BYTES(u4Count);

		//Printf("DHT : %s table index %d\n", fgIsAc ? "AC" : "DC", u1TblIndex);

		if (fgIsAc)
		{
		    if((prInstance->rDHT.fgAcTblLoaded & (1 << u1TblIndex)) == 0)
		    {
			prInstance->rDHT.u4NumAcTbl++;
			prInstance->rDHT.fgAcTblLoaded |= 1 << u1TblIndex;
	    	}
			x_memcpy(prInstance->rDHT.arAcTbl[u1TblIndex].au1HuffVal, au1HuffVal, u4Count);
			x_memcpy(prInstance->rDHT.arAcTbl[u1TblIndex].au1Bits, au1Bits, 17);
		}
		else
		{
		    if((prInstance->rDHT.fgDcTblLoaded & (1 << u1TblIndex)) == 0)
		    {
			prInstance->rDHT.u4NumDcTbl++;
			prInstance->rDHT.fgDcTblLoaded |= 1 << u1TblIndex;
	    	}
			x_memcpy(prInstance->rDHT.arDcTbl[u1TblIndex].au1HuffVal, au1HuffVal, u4Count);
			x_memcpy(prInstance->rDHT.arDcTbl[u1TblIndex].au1Bits, au1Bits, 17);
		}
	}

	return (INT32)E_JPG_OK;
}

/* process de-quantization table */
static INT32 _jfifProcDQT			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;
	INT32 i4Ntbl = 0;
	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(4);

	if (u2Length < (2 + 1 + 64))
	{
		return -(INT32)E_JPG_ERR_INVALID_DHT;
	}

	u2Length -= 2;

	while (u2Length > 64)
	{
		UINT8 u1Prec;
		UINT8 u1Qtbl;

		u1Prec = JPEG_READ_BYTE(prInstance, 0);
		u1Qtbl = u1Prec & 0x3;	// table index 0 .. 3
		u1Prec >>= 4;

		if (u1Prec && (u2Length < (128 + 1)))
		{
			return -(INT32)E_JPG_ERR_INVALID_DHT;
		}

		//LOG(3,"DQT %d table index %d\n", u1Prec ? 12 : 8, u1Qtbl);

		x_memcpy(prInstance->rDQT.aau1Qtbl[u1Qtbl],
			prInstance->pu1CurrRead + 1,
			u1Prec ? (SIZE_T)128: (SIZE_T)64);
		prInstance->rDQT.afgPrec[u1Qtbl] = (BOOL)u1Prec;

		u2Length -= u1Prec ? (128 + 1) : (64 + 1);
		ADVANCE_BYTES(u1Prec ? (UINT32)(128 + 1) : (UINT32)(64 + 1));
		i4Ntbl++;
	}
	prInstance->rDQT.u1NumQ += i4Ntbl;
	prInstance->rDQT.fgDQT = TRUE;
	return (INT32)E_JPG_OK;
}

/* process restart interval marker */
static INT32 _jfifProcDRI			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(4);

	if (u2Length < 4)
	{
		return -(INT32)E_JPG_ERR_INVALID_DRI;
	}

	prInstance->u2RestartInterval = JPEG_READ_2BYTES(prInstance, 0);

	ADVANCE_BYTES(2);

	return (INT32)E_JPG_OK;

}

/* process standard APP0 "JFIF" marker (and "JFXX" extension) */
static INT32 _jfifProcAPP0			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	INT32 i4Index;
	UINT16 u2Length;
	UINT8 au1Identifier[5];

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	u2Length = JPEG_READ_2BYTES(prInstance, 2);	//2, 3
	ADVANCE_BYTES(4);	// skip marker and length

	if (u2Length < 8)
	{
		return -(INT32)E_JPG_ERR_INVALID_APP0;
	}
	u2Length -= 2;

	/* identifier */
	for (i4Index = 0; i4Index < 5; i4Index++)
	{
		au1Identifier[i4Index] = JPEG_READ_BYTE(prInstance, i4Index);
	}

	ADVANCE_BYTES(5);	// skip identifier bytes
	u2Length -= 5;

	if (0 == x_memcmp(au1Identifier, "JFIF\0", 5))
	{
		// jfif
		if (u2Length < 9)
		{
			return -(INT32)E_JPG_ERR_INVALID_APP0;
		}
		prInstance->arAPP0[E_APP0_JFIF].eApp0Type = E_APP0_JFIF;
		/* version */
		prInstance->arAPP0[E_APP0_JFIF].u2Version = JPEG_READ_2BYTES(prInstance, 0);
		ADVANCE_BYTES(2);
		u2Length -= 2;

		/* units */
		prInstance->arAPP0[E_APP0_JFIF].u1Units = JPEG_READ_BYTE(prInstance, 0);
		ADVANCE_BYTES(1);
		u2Length -= 1;

		/* x & y deisity */
		prInstance->arAPP0[E_APP0_JFIF].u2DensityX = JPEG_READ_2BYTES(prInstance, 0);
		prInstance->arAPP0[E_APP0_JFIF].u2DensityY = JPEG_READ_2BYTES(prInstance, 2);
		ADVANCE_BYTES(4);
		u2Length -= 4;

		/* thumbnail w & h */
		prInstance->arAPP0[E_APP0_JFIF].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
		prInstance->arAPP0[E_APP0_JFIF].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
		ADVANCE_BYTES(2);
		u2Length -= 2;

		/* basic thumbnail */
		if ((prInstance->arAPP0[E_APP0_JFIF].u1ThumbW != 0) &&
			(prInstance->arAPP0[E_APP0_JFIF].u1ThumbH != 0) &&
			(u2Length == (UINT16)((UINT16)prInstance->arAPP0[E_APP0_JFIF].u1ThumbW * (UINT16)prInstance->arAPP0[E_APP0_JFIF].u1ThumbH * 3)))
		{
			prInstance->arAPP0[E_APP0_JFIF].fgHasThumbnail = TRUE;
			prInstance->arAPP0[E_APP0_JFIF].pu1RGBdata = prInstance->pu1CurrRead;
		}

		prInstance->arAPP0[E_APP0_JFIF].fgAPP0 = TRUE;
	}
	else if (0 == x_memcmp(au1Identifier, "JFXX\0", 5))
	{
		// jfxx
		if (u2Length < 1)
		{
			return -(INT32)E_JPG_ERR_INVALID_APP0;
		}
		prInstance->arAPP0[E_APP0_JFXX].eApp0Type = E_APP0_JFXX;
		/* extension code */
		prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode = (ENUM_APP0_THUMBNAIL_MODE) JPEG_READ_BYTE(prInstance, 0);
		ADVANCE_BYTES(1);
		u2Length -= 1;

		if (prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode == E_THUMB_JPEG)
		{
			prInstance->arAPP0[E_APP0_JFXX].pu1JpegThumbnail = prInstance->pu1CurrRead;
		}
		else if (prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode == E_THUMB_1BYTE_PIX)
		{
			if (u2Length < 2)
			{
				return -(INT32)E_JPG_ERR_INVALID_APP0;
			}
			/* thumbnail w & h */
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
			ADVANCE_BYTES(2);
			u2Length -= 2;
			/* palettd & pixel validation */
			if ((prInstance->arAPP0[E_APP0_JFXX].u1ThumbW != 0) &&
				(prInstance->arAPP0[E_APP0_JFXX].u1ThumbH != 0) &&
				(u2Length == (UINT16)(((UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbW * (UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbH) + 768)))
			{
				UINT8 u1R, u1G, u1B;
				for (i4Index = 0; i4Index < 256; i4Index++)
				{
					u1R = JPEG_READ_BYTE(prInstance, 0);
					u1G = JPEG_READ_BYTE(prInstance, 1);
					u1B = JPEG_READ_BYTE(prInstance, 2);
					ADVANCE_BYTES(3);
					u2Length -= 3;
					prInstance->arAPP0[E_APP0_JFXX].au4Palette[i4Index] =
						(UINT32)(0xff000000 |
						(u1R << 16) |
						(u1G << 8) |
						u1B);
				}
				prInstance->arAPP0[E_APP0_JFXX].pu1ColorData = prInstance->pu1CurrRead;
				prInstance->arAPP0[E_APP0_JFXX].fgHasThumbnail = TRUE;
			}
		}
		else if (prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode == E_THUMB_3BYTE_PIX)
		{
			if (u2Length < 2)
			{
				return -(INT32)E_JPG_ERR_INVALID_APP0;
			}
			/* thumbnail w & h */
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
			ADVANCE_BYTES(2);
			u2Length -= 2;
			/* palettd & pixel validation */
			if ((prInstance->arAPP0[E_APP0_JFXX].u1ThumbW != 0) &&
				(prInstance->arAPP0[E_APP0_JFXX].u1ThumbH != 0) &&
				(u2Length == (UINT16)(((UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbW * (UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbH) * 3)))
			{
				prInstance->arAPP0[E_APP0_JFXX].pu1RGBdata = prInstance->pu1CurrRead;
				prInstance->arAPP0[E_APP0_JFXX].fgHasThumbnail = TRUE;
			}
		}
		else
		{
			// no thumbnail in jfxx ...
			// TODO : try more cases
			return -(INT32)E_JPG_ERR_INVALID_APP0;
		}

		prInstance->arAPP0[E_APP0_JFXX].fgAPP0 = TRUE;
	}
	else
	{
		// unsuppported tag
		// just skip it
		//ADVANCE_BYTES(u2Length);
	}

	ADVANCE_BYTES(u2Length);

	prInstance->u4NumApp0++;
	return (INT32)E_JPG_OK;
}

/* process standard APP1 "Exif" marker */
static INT32 _jfifProcAPP1			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;

	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (prInstance->rAPP1.fgAPP1)
	{
		//return -(INT32)E_JPG_ERR_MULTI_APP1;
	}

	ADVANCE_BYTES(2);
	u2Length = JPEG_READ_2BYTES(prInstance, 0);
	if (u2Length < 2)
	{
		return -(INT32)E_JPG_ERR_INVALID_APP1;
	}

	prInstance->rAPP1.pu1ExifData = prInstance->pu1CurrRead;
	prInstance->rAPP1.fgAPP1 = TRUE;
	ADVANCE_BYTES(u2Length);
	return (INT32)E_JPG_OK;
}

/* process APP14 */
static INT32 _jfifProcAPP14			(JPEG_JFIF_HEADER_T *prInstance)
{
	return _jfifProcAPP0(prInstance);
}

/* process all non-supported APPn format */
static INT32 _jfifProcAPPother		(JPEG_JFIF_HEADER_T *prInstance)
{
	return _jfifSkipMarker(prInstance);
}

/* process end of image marker */
static INT32 _jfifProcEOI			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	ASSERT(prInstance != NULL);
	VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

	if (prInstance->fgEOI)
	{
		return -(INT32)E_JPG_ERR_MULTI_EOI;
	}

	ADVANCE_BYTES(2);

	prInstance->fgEOI = TRUE;
	prInstance->pu1EOI = prInstance->pu1CurrRead;

	return (INT32)E_JPG_OK;
}



/* export interfaces */
// get sizeof instance
JPEGHANDLE JFIF_AllocInstance(void)
{
	JPEGHANDLE hInstance;
	JPEG_BASE_CLASS_T *prRootObject;

	// allocate jfif header instance
	hInstance = (JPEGHANDLE)x_mem_alloc(sizeof(JPEG_JFIF_HEADER_T));
	if (hInstance == NULL)
	{
		return hInstance;
	}
	x_memset(hInstance, 0, sizeof(JPEG_JFIF_HEADER_T));
	// init its root object
	prRootObject = (JPEG_BASE_CLASS_T *)hInstance;
	prRootObject->eJpegClass = E_JC_JFIF_HEADER;
	prRootObject->zObjectSize = sizeof(JPEG_JFIF_HEADER_T);

	return hInstance;
}

void JFIF_FreeInstance(JPEGHANDLE hInstance)
{
	x_mem_free(hInstance);
}

SIZE_T JFIF_GetHeaderSize(void)
{
	return sizeof(JPEG_JFIF_HEADER_T);
}

// start and parse a jpeg file image store in memory
INT32 JFIF_New(JPEGHANDLE hInstance, void *pvJpegImage, UINT32 u4JpegSize)
{
	INT32 i4Ret;
	JPEG_JFIF_HEADER_T *prInstance;

	prInstance = (JPEG_JFIF_HEADER_T*)hInstance;

	if (prInstance == NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}

	prInstance->pu1JpegImage = (UINT8*)pvJpegImage;
	prInstance->pu1CurrRead = prInstance->pu1JpegImage;
	prInstance->u4ImageByteSize = u4JpegSize;
	prInstance->fgInited = TRUE;

	i4Ret = _jfifParse(prInstance);
	if ((INT32)E_JPG_OK != i4Ret)
	{
		return i4Ret;
	}

	if (prInstance->fgParsed)
	{
		return (INT32)E_JPG_OK;
	}
	else
	{
		return -(INT32)E_JPG_ERR_PARSE_FAILED;
	}
}

INT32 JFIF_ParseProgScan(JPEGHANDLE hInstance)
{
	INT32 i4Ret;
	JPEG_JFIF_HEADER_T *prInstance;

	prInstance = (JPEG_JFIF_HEADER_T*)hInstance;

	if (prInstance == NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}

	i4Ret = _jfifParseProgScan(prInstance);
	return i4Ret;
}

INT32 JFIF_ParseProgMarker(JPEGHANDLE hInstance, UINT32 u4Scan)
{
	INT32 i4Ret;
	JPEG_JFIF_HEADER_T *prInstance;

	prInstance = (JPEG_JFIF_HEADER_T*)hInstance;

	if (prInstance == NULL)
	{
		return -(INT32)E_JPG_ERR_PARAM;
	}

	i4Ret = _jfifParseScan(prInstance, u4Scan);
	return i4Ret;
}

INT32 JFIF_Dump(JPEGHANDLE hInstance)
{
	INT32 i4Ret;
	JPEG_JFIF_HEADER_T *prInstance;

	prInstance = (JPEG_JFIF_HEADER_T *)hInstance;

	i4Ret = _jfifCheckInstance(prInstance);
	if ((INT32)E_JPG_OK != i4Ret)
	{
		return i4Ret;
	}


	return (INT32)E_JPG_OK;
}


