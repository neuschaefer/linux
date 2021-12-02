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

#include "jdec_com_common.h"
#include "jdec_com_jfif.h"

#define ADVANCE_BYTES(NBYTES) \
		i4Ret = _jfifAdvanceBytes(prInstance, NBYTES); \
		if (E_JPG_OK != i4Ret) \
		{ \
			return i4Ret; \
		} \
/*
#define JPEG_READ_BYTE(PINSTANCE, NTH_BYTE) (PINSTANCE->pu1CurrRead[NTH_BYTE])
#define JPEG_READ_2BYTES(PINSTANCE, NTH_BYTE) ((JPEG_READ_BYTE(PINSTANCE, NTH_BYTE) << 8) | JPEG_READ_BYTE(PINSTANCE, (NTH_BYTE) + 1))
*/

#ifndef DRV_VERIFY_SUPPORT
extern void vJdecRequireData(void *pvInst, UINT32 u4StartAddr, UINT32 u4ReqSize, UINT32 u4FileOfst, BOOL fgResetPos);
extern UINT32 u4JdecCheckFifoData(UINT32 u4RdPoint, UINT32 u4WrPoint, UINT32 u4FifoStart, UINT32 u4FifoEnd);
extern void vJdecUpdateFifoPoint(UINT8 *pu1DataPoint, UINT32 u4AdvData, UINT32 u4FifoStart, UINT32 u4FifoEnd);
#endif

/* private interfaces */
static UINT8  _jfifReadByte			(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4NthByte);
	/* read 1 byte from jpeg stream */
static UINT16 _jfifRead2Bytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4NthByte);
	/* read 2 bytes from jpeg stream */
static INT32  _jfifSkipBytes		(JPEG_JFIF_HEADER_T *prInstance, INT32 i4Bytes);
	/* skip i4Bytes bytes in jpeg stream */

#define JPEG_READ_BYTE(PINSTANCE, NTH_BYTE) (_jfifReadByte(PINSTANCE, NTH_BYTE))
#define JPEG_READ_2BYTES(PINSTANCE, NTH_BYTE) (_jfifRead2Bytes(PINSTANCE, NTH_BYTE))

/* private interfaces */
static INT32 _jfifAdvanceBytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Bytes);
	/* advance read pointer n bytes */
static INT32 _jfifCheckInstance		(JPEG_JFIF_HEADER_T *prInstance);
	/* check instance */
static INT32 _jfifParse				(JPEG_JFIF_HEADER_T *prInstance);
	/* parse all marker in jpeg file */
static INT32 _jfifIsMarker			(JPEG_JFIF_HEADER_T *prInstance);
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


#ifndef MT8520
#include "assert.h"
#define ASSERT assert

void x_memset(void *pv_to, UINT8 ui1_c, SIZE_T z_l)
{
    UINT32 u4I = 0;

    for(u4I=0; u4I<z_l; u4I++)
        *(((BYTE *)pv_to) + u4I) = (BYTE)ui1_c;
}

void x_memcpy(void *pv_to, const void *pv_from, SIZE_T z_l)
{
    UINT32 u4I = 0;

    for(u4I=0; u4I<z_l; u4I++)
        *(((BYTE *)pv_to) + u4I) = *(((BYTE *)pv_from) + u4I);
}

INT32 x_memcmp(const void *pv_s1, const void *pv_s2, SIZE_T z_l)
{
    UINT32 u4I = 0;

    while(u4I < z_l)
    {
        if(*(((BYTE *)pv_s1) + u4I) != *(((BYTE *)pv_s2) + u4I))
            break;
        u4I++;
    }

    if(u4I == z_l)
      return 0;

    return 1;
}

void *x_mem_alloc(SIZE_T size)
{
  while(1);

  return 0;
}

UINT32 x_mem_free(void *addr)
{
  while(1);

  return 0;
}


INT16 Printf(const CHAR * format, ...)
{
  return 0;
}

#endif


//----------------------------------------------------------

/* private interfaces */
static UINT8  _jfifReadByte			(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4NthByte)
{
#ifndef DRV_VERIFY_SUPPORT
    UINT32 u4FifoData;

    u4FifoData = u4JdecCheckFifoData((UINT32)(prInstance->pu1CurrRead), (UINT32)(prInstance->pu1CurrWrite), prInstance->u4FifoStart, prInstance->u4FifoEnd);
    if (u4FifoData < u4NthByte)
    {
        prInstance->pu1CurrWrite = (UINT8 *)prInstance->u4FifoStart;
        prInstance->pu1CurrRead = (UINT8 *)prInstance->u4FifoStart;
        vJdecRequireData(prInstance->pvCbInst, prInstance->u4FifoStart, (4*1024), prInstance->u4HasReadData, TRUE);
        while ((UINT32)(prInstance->pu1CurrWrite) == prInstance->u4FifoStart)
        {
            //delay 1/100 sec
            x_thread_delay(1);
        }
    }
#endif
    return prInstance->pu1CurrRead[u4NthByte];
}

static UINT16 _jfifRead2Bytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4NthByte)
{
#ifndef DRV_VERIFY_SUPPORT
    UINT32 u4FifoData;

    u4FifoData = u4JdecCheckFifoData((UINT32)(prInstance->pu1CurrRead), (UINT32)(prInstance->pu1CurrWrite), prInstance->u4FifoStart, prInstance->u4FifoEnd);
    if (u4FifoData < (u4NthByte+1))
    {
        prInstance->pu1CurrWrite = (UINT8 *)prInstance->u4FifoStart;
        prInstance->pu1CurrRead = (UINT8 *)prInstance->u4FifoStart;
        vJdecRequireData(prInstance->pvCbInst, prInstance->u4FifoStart, (4*1024), prInstance->u4HasReadData, TRUE);
        while ((UINT32)(prInstance->pu1CurrWrite) == prInstance->u4FifoStart)
        {
            //delay 1/100 sec
            x_thread_delay(1);
        }
    }
#endif
    return ((prInstance->pu1CurrRead[u4NthByte] << 8) | (prInstance->pu1CurrRead[u4NthByte + 1]));
}

static INT32  _jfifSkipBytes		(JPEG_JFIF_HEADER_T *prInstance, INT32 i4Bytes)
{
	return 0;
}


void JFIF_UnusedStatics(void)
{
	JPEG_JFIF_HEADER_T *prInstance = NULL;
	_jfifSkipBytes(prInstance, 0);
}

/* private interfaces */

static INT32 _jfifAdvanceBytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Bytes)
{
#ifndef DRV_VERIFY_SUPPORT
    UINT32 u4FifoData = 0;
#endif
    VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

#ifdef DRV_VERIFY_SUPPORT
    if (prInstance->pu1CurrRead + u4Bytes >= prInstance->pu1JpegHdr + prInstance->u4ImageByteSize - prInstance->u4HasReadData)
#else
    if (prInstance->u4HasReadData >= prInstance->u4ImageByteSize)
#endif
    {
        return -E_JPG_ERR_PASS_END;
    }

#ifndef DRV_VERIFY_SUPPORT
    u4FifoData = u4JdecCheckFifoData((UINT32)(prInstance->pu1CurrRead), (UINT32)(prInstance->pu1CurrWrite), prInstance->u4FifoStart, prInstance->u4FifoEnd);
    prInstance->u4HasReadData += u4Bytes;
    if (u4FifoData < u4Bytes)
    {
        prInstance->pu1CurrWrite = (UINT8 *)prInstance->u4FifoStart;
        prInstance->pu1CurrRead = (UINT8 *)prInstance->u4FifoStart;
        vJdecRequireData(prInstance->pvCbInst, prInstance->u4FifoStart, (4*1024), prInstance->u4HasReadData, TRUE);
        while ((UINT32)(prInstance->pu1CurrWrite) == prInstance->u4FifoStart)
        {
            //delay 1/100 sec
            x_thread_delay(1);
        }
    }
    else
    {
        vJdecUpdateFifoPoint(prInstance->pu1CurrRead, u4Bytes, prInstance->u4FifoStart, prInstance->u4FifoEnd);
    }
#else
    prInstance->pu1CurrRead += u4Bytes;
#endif

    return E_JPG_OK;
}

/* instance check */
static INT32 _jfifCheckInstance		(JPEG_JFIF_HEADER_T *prInstance)
{
	if (prInstance == NULL)
	{
		// null instance
		return -E_JPG_ERR_PARAM;
	}

	if (prInstance->fgFatalError == TRUE)
	{
		// an fatal error happened
		return -E_JPG_ERR_FATAL;
	}

	return E_JPG_OK;
}

/* parse header markers in jpeg file */
static INT32 _jfifParse				(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2JfifMarker;

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	if (prInstance->fgPrsResult == TRUE)
	{
		// already parsed
		return -E_JPG_ERR_ALREADY;
	}

	// parse until EOI
	for(;;)
	{
		i4Ret = _jfifPeekMarker(prInstance, &u2JfifMarker);
		if (E_JPG_OK == i4Ret)
		{
			// marker found
			i4Ret = _jfifDispatchMarker(prInstance, u2JfifMarker);

			if ((M_EOI == u2JfifMarker) ||
				(M_SOS == u2JfifMarker))
			{
				// stop condition reached - EOI
				if (((prInstance->u4NumApp0 != 0) || (prInstance->rAPP1.fgAPP1 == TRUE)) &&
					//(prInstance->rDQT.fgDQT == TRUE) &&
					(prInstance->rDHT.fgDcTblLoaded != FALSE) &&
					(prInstance->rDHT.fgAcTblLoaded != FALSE || prInstance->rSOF.eJpegFormat != E_JPG_BASELINE) &&
					(prInstance->fgSOF == TRUE) &&
					(prInstance->fgSOI == TRUE) &&
					(prInstance->fgFatalError == FALSE))
				{
					prInstance->fgPrsResult = TRUE;
				}
				else
				{
					prInstance->fgPrsResult = FALSE;
				}
				break;
			}

			if (E_JPG_OK != i4Ret)
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


/* check if current read pointer points to an valid marker */
static INT32 _jfifIsMarker			(JPEG_JFIF_HEADER_T *prInstance)
{
	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

#ifdef DRV_VERIFY_SUPPORT
	if (prInstance->pu1CurrRead >= (prInstance->pu1JpegHdr + prInstance->u4ImageByteSize - prInstance->u4HasReadData))
#else
	if (prInstance->u4HasReadData >= prInstance->u4ImageByteSize)
#endif
	{
		return -E_JPG_ERR_PASS_END;
	}

	if (*prInstance->pu1CurrRead == 0xff)
	{
		return E_JPG_OK;
	}

	return -E_JPG_ERR_NOT_A_MARKER;
}

/* get next marker (read pointer stops on this marker) */
static INT32 _jfifPeekMarker		(JPEG_JFIF_HEADER_T *prInstance, UINT16 *pu2Marker)
{
	INT32 i4Ret;
	UINT16 u2Marker;

	ASSERT(pu2Marker);
	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	while ((i4Ret = _jfifIsMarker(prInstance)) == -E_JPG_ERR_NOT_A_MARKER)
	{
		ADVANCE_BYTES(1);
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

	return E_JPG_OK;
}

/* skip current marker */
static INT32 _jfifSkipMarker		(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);
/*
	if (_jfifIsMarker(prInstance))
	{
		// although this is not a marker
		// skip marker should return without error
		// due to its function : 'skip marker'
		return E_JPG_OK;
	}
*/
	// this is a marker, advance 2 bytes first
	ADVANCE_BYTES(2);

	u2Length = JPEG_READ_2BYTES(prInstance, 0);
	ADVANCE_BYTES(u2Length);

	return E_JPG_OK;
}

/* dispatch this marker to proper handler routine */
static INT32 _jfifDispatchMarker	(JPEG_JFIF_HEADER_T *prInstance, UINT16 u2Marker)
{
	INT32 i4Ret;

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

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
		if (E_JPG_OK != i4Ret)
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
		break;
    case 0xFF00: // Not marker, just skip it.
        ADVANCE_BYTES(2);
        i4Ret = E_JPG_OK;
        break;
	default			:
		// unknown marker, just skip it
		// due to unknown, we can not trust the length
		// hence advance through it
		//ADVANCE_BYTES(1);
		i4Ret = _jfifSkipMarker(prInstance);
		break;
	}

	return i4Ret;
}

/* process start of image */
static INT32 _jfifProcSOI			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret; /* for ADVANCE_BYTES() error return */

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	if (prInstance->fgSOI)
	{
		return -E_JPG_ERR_MULTI_SOI;
	}

	prInstance->fgSOI = TRUE;
	ADVANCE_BYTES(2);

	return E_JPG_OK;
}

/* process start of frame */
static INT32 _jfifProcSOF			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Marker;
	UINT16 u2Length;
	UINT8 u1CompIndex;

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	if (prInstance->rSOF.fgSOF)
	{
		return -E_JPG_ERR_MULTI_SOF;
	}


	u2Marker = JPEG_READ_2BYTES(prInstance, 0);	// 0,1
	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(2);	// skip marker id (does not count)

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
		prInstance->rSOF.eJpegFormat = E_JPG_EXTENDED_SEQUENTIAL_HUFFMAN;
		break;
	case M_SOF2		:
		prInstance->rSOF.eJpegFormat = E_JPG_PROGRESSIVE_HUFFMAN;
		break;
		//return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF3		:
		prInstance->rSOF.eJpegFormat = E_JPG_EXTENDED_SEQUENTIAL_ARITHMETIC;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF5		:
		prInstance->rSOF.eJpegFormat = E_JPG_PROGRESSIVE_ARITHMETIC;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF6		:
		prInstance->rSOF.eJpegFormat = E_JPG_LOSSLESS_HUFFMAN;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF7		:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFFERENTIAL_SEQ_HUFFMAN;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF9		:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFFERENTIAL_PROGRESSIVE_HUFFMAN;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF10	:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFFERENTIAL_LOSSLESS_HUFFMAN;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF11	:
		prInstance->rSOF.eJpegFormat = E_JPG_RESERVED_FOR_EXTENSIONS;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF13	:
		prInstance->rSOF.eJpegFormat = E_JPG_LOSSLESS_ARITHMETIC;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF14	:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFFERENTIAL_SEQUENTIAL_ARITHMETIC;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	case M_SOF15	:
		prInstance->rSOF.eJpegFormat = E_JPG_DIFFERENTIAL_PROGRESSIVE_ARITHMETIC;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	default			:
		prInstance->rSOF.eJpegFormat = E_JPG_UNSUPPORT_FORMAT;
		return -E_JPG_ERR_UNSUPPORT_SOF;
	}

	// SOF0 & SOF1 & SOF2 parsing
	if (u2Length < 8)
	{
		return -E_JPG_ERR_INVALID_SOF;
	}

	prInstance->rSOF.u1DataPrecision	= JPEG_READ_BYTE	(prInstance, 2);	// 2
	prInstance->rSOF.u2ImageHeight		= JPEG_READ_2BYTES	(prInstance, 3);	// 3,4
	prInstance->rSOF.u2ImageWidth		= JPEG_READ_2BYTES	(prInstance, 5);	// 5,6
	prInstance->rSOF.u1NumComponents	= JPEG_READ_BYTE	(prInstance, 7);	// 7

	ADVANCE_BYTES(8);
	u2Length -= 8;

	// start of frame validation test
	if ((prInstance->rSOF.u2ImageHeight		== 0)	||
		(prInstance->rSOF.u2ImageWidth		== 0)	||
		(prInstance->rSOF.u1NumComponents	<  1)	||
		(prInstance->rSOF.u1NumComponents	>  3)	||
		(u2Length != prInstance->rSOF.u1NumComponents * 3))
	{
		return -E_JPG_ERR_INVALID_SOF;
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
	return E_JPG_OK;
}

/* process start of scan */
static INT32 _jfifProcSOS			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;
	UINT8 u1CompInScan;
	UINT8 u1CompIndex;

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	if (prInstance->rSOF.fgSOF == FALSE)
	{
		return -E_JPG_ERR_SOS_BEFORE_SOF;
	}

	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	u1CompInScan = JPEG_READ_BYTE(prInstance, 4);	// 4
	ADVANCE_BYTES(5);

	// sos validation
	if ((u2Length < (u1CompInScan * 2 + 6)) ||
		(u1CompInScan == 0) ||
		(u1CompInScan > MAX_JPEG_COMPONENT))
	{
		return -E_JPG_ERR_INVALID_SOS;
	}

	// store components in scan
	prInstance->rSOS.u1CompInScan = u1CompInScan;

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


	// read component id and table no
	for (u1CompIndex = 0; u1CompIndex < u1CompInScan; u1CompIndex++)
	{
		UINT8 u1CompId;
		UINT8 u1TableId;
		UINT8 u1MapedIndex;

		u1CompId = JPEG_READ_BYTE(prInstance, 0);
		u1TableId = JPEG_READ_BYTE(prInstance, 1);
		ADVANCE_BYTES(2);
		prInstance->rSOS.au1CompId[u1CompIndex] = prInstance->rSOF.au1MapId2Index[u1CompId];

		u1MapedIndex = prInstance->rSOF.au1MapId2Index[u1CompId];
		/* following data use maped index */
		prInstance->rSOS.au1DcId[u1MapedIndex] = u1TableId >> 4;
		prInstance->rSOS.au1AcId[u1MapedIndex] = u1TableId & 0xf;
	}

	prInstance->rSOS.u1Ss		= JPEG_READ_BYTE(prInstance, 0);
	prInstance->rSOS.u1Se		= JPEG_READ_BYTE(prInstance, 1);
	{
	    UINT8 u1AhAl = JPEG_READ_BYTE(prInstance, 2);
	    prInstance->rSOS.u1Ah  = u1AhAl >> 4;
	    prInstance->rSOS.u1Al  = u1AhAl & 0xF;
	}
	ADVANCE_BYTES(3);

    prInstance->u4BitStmSrtOfst = prInstance->u4HasReadData;

	return E_JPG_OK;
}

/* process de-huffman table */
static INT32 _jfifProcDHT			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;
	UINT8 au1Bits[17];
	UINT8 au1HuffVal[256];

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(4);

	if (u2Length < (2 + 17))
	{
		return -E_JPG_ERR_INVALID_DHT;
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

		if (E_JPG_OK != i4Ret)
		{
			return i4Ret;
		}

		if (u4Count > 256)
		{
			//bogus dht
			u4Count = 256;
		}

		if (u2Length < u4Count)
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

		Printf("DHT : %s table index %d\n", fgIsAc ? "AC" : "DC", u1TblIndex);

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

	return E_JPG_OK;
}

/* process de-quantization table */
static INT32 _jfifProcDQT			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;
	INT32 i4Ntbl = 0;
	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(4);

	if (u2Length < (2 + 1 + 64))
	{
		return -E_JPG_ERR_INVALID_DHT;
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
			return -E_JPG_ERR_INVALID_DHT;
		}

		Printf("DQT %d table index %d\n", u1Prec ? 12 : 8, u1Qtbl);

#ifdef DRV_VERIFY_SUPPORT
		x_memcpy(prInstance->rDQT.aau1Qtbl[u1Qtbl],
			prInstance->pu1CurrRead + 1,
			u1Prec ? 128: 64);
#else
        {
            UINT32 u4 = 0;
            for (u4=0; u4<(u1Prec?128:64); u4++)
            {
                prInstance->rDQT.aau1Qtbl[u1Qtbl][u4] = JPEG_READ_BYTE(prInstance, u4+1);
            }
        }
#endif
		prInstance->rDQT.afgPrec[u1Qtbl] = (BOOL)u1Prec;

		u2Length -= u1Prec ? (128 + 1) : (64 + 1);
		ADVANCE_BYTES(u1Prec ? 128 + 1 : 64 + 1);
		i4Ntbl++;
	}
	prInstance->rDQT.u1NumQ += i4Ntbl;
	prInstance->rDQT.fgDQT = TRUE;
	return E_JPG_OK;
}

/* process restart interval marker */
static INT32 _jfifProcDRI			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
	ADVANCE_BYTES(4);

	if (u2Length < 4)
	{
		return -E_JPG_ERR_INVALID_DRI;
	}

	prInstance->u2RestartInterval = JPEG_READ_2BYTES(prInstance, 0);

	ADVANCE_BYTES(2);

	return E_JPG_OK;

}

/* process standard APP0 "JFIF" marker (and "JFXX" extension) */
static INT32 _jfifProcAPP0			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	INT32 i4Index;
	UINT16 u2Length;
	UINT8 au1Identifier[5];

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	return (_jfifSkipMarker(prInstance));

	u2Length = JPEG_READ_2BYTES(prInstance, 2);	// 2, 3
	ADVANCE_BYTES(4);	// skip marker and length

	if (u2Length < 8)
	{
		return -E_JPG_ERR_INVALID_APP0;
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
			return -E_JPG_ERR_INVALID_APP0;
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
			(u2Length == (UINT16)prInstance->arAPP0[E_APP0_JFIF].u1ThumbW * (UINT16)prInstance->arAPP0[E_APP0_JFIF].u1ThumbH * 3))
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
			return -E_JPG_ERR_INVALID_APP0;
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
				return -E_JPG_ERR_INVALID_APP0;
			}
			/* thumbnail w & h */
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
			ADVANCE_BYTES(2);
			u2Length -= 2;
			/* palettd & pixel validation */
			if ((prInstance->arAPP0[E_APP0_JFXX].u1ThumbW != 0) &&
				(prInstance->arAPP0[E_APP0_JFXX].u1ThumbH != 0) &&
				(u2Length == prInstance->arAPP0[E_APP0_JFXX].u1ThumbW * prInstance->arAPP0[E_APP0_JFXX].u1ThumbH + 768))
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
						0xff000000 |
						(u1R << 16) |
						(u1G << 8) |
						u1B;
				}
				prInstance->arAPP0[E_APP0_JFXX].pu1ColorData = prInstance->pu1CurrRead;
				prInstance->arAPP0[E_APP0_JFXX].fgHasThumbnail = TRUE;
			}
		}
		else if (prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode == E_THUMB_3BYTE_PIX)
		{
			if (u2Length < 2)
			{
				return -E_JPG_ERR_INVALID_APP0;
			}
			/* thumbnail w & h */
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
			prInstance->arAPP0[E_APP0_JFXX].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
			ADVANCE_BYTES(2);
			u2Length -= 2;
			/* palettd & pixel validation */
			if ((prInstance->arAPP0[E_APP0_JFXX].u1ThumbW != 0) &&
				(prInstance->arAPP0[E_APP0_JFXX].u1ThumbH != 0) &&
				(u2Length == prInstance->arAPP0[E_APP0_JFXX].u1ThumbW * prInstance->arAPP0[E_APP0_JFXX].u1ThumbH * 3))
			{
				prInstance->arAPP0[E_APP0_JFXX].pu1RGBdata = prInstance->pu1CurrRead;
				prInstance->arAPP0[E_APP0_JFXX].fgHasThumbnail = TRUE;
			}
		}
		else
		{
			// no thumbnail in jfxx ...
			// TODO : try more cases
			return -E_JPG_ERR_INVALID_APP0;
		}

		prInstance->arAPP0[E_APP0_JFXX].fgAPP0 = TRUE;
	}
	else
	{
		// unsuppported tag
		// just skip it
		ADVANCE_BYTES(u2Length);
	}

	prInstance->u4NumApp0++;
	return E_JPG_OK;
}

/* process standard APP1 "Exif" marker */
static INT32 _jfifProcAPP1			(JPEG_JFIF_HEADER_T *prInstance)
{
	INT32 i4Ret;
	UINT16 u2Length;

	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	return (_jfifSkipMarker(prInstance));

	if (prInstance->rAPP1.fgAPP1 == TRUE)
	{
		return -E_JPG_ERR_MULTI_APP1;
	}

	ADVANCE_BYTES(2);
	u2Length = JPEG_READ_2BYTES(prInstance, 0);
	if (u2Length < 2)
	{
		return -E_JPG_ERR_INVALID_APP1;
	}

	prInstance->rAPP1.pu1ExifData = prInstance->pu1CurrRead;
	prInstance->rAPP1.fgAPP1 = TRUE;
	ADVANCE_BYTES(u2Length);
	return E_JPG_OK;
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
	VERIFY(_jfifCheckInstance(prInstance) == E_JPG_OK);

	if (prInstance->fgEOI)
	{
		return -E_JPG_ERR_MULTI_EOI;
	}

	ADVANCE_BYTES(2);

	prInstance->fgEOI = TRUE;

	return E_JPG_OK;
}



/* export interfaces */
// get sizeof instance
JPEGHANDLE JFIF_AllocInstance(void)
{
	JPEGHANDLE hInstance;

	// allocate jfif header instance
	hInstance = (JPEGHANDLE)x_mem_alloc(sizeof(JPEG_JFIF_HEADER_T));
	if (hInstance == NULL)
	{
		return hInstance;
	}
	x_memset(hInstance, 0, sizeof(JPEG_JFIF_HEADER_T));

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
INT32 JFIF_New8520(JPEGHANDLE hInstance, void *pvJpegImage, UINT32 u4JpegSize)
{
	INT32 i4Ret;
	JPEG_JFIF_HEADER_T *prInstance;

	prInstance = (JPEG_JFIF_HEADER_T*)hInstance;

	if (prInstance == NULL)
	{
		return -E_JPG_ERR_PARAM;
	}

	prInstance->pu1JpegHdr = (UINT8*)pvJpegImage;
	prInstance->pu1CurrRead = prInstance->pu1JpegHdr;
	prInstance->u4ImageByteSize = u4JpegSize;

	i4Ret = _jfifParse(prInstance);
	if (E_JPG_OK != i4Ret)
	{
		return i4Ret;
	}

	if (prInstance->fgPrsResult)
	{
		return E_JPG_OK;
	}
	else
	{
		return -E_JPG_ERR_PARSE_FAILED;
	}
}

