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

#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "jpg_common.h"
#include "jpg_if.h"
#include "jpg_jfif.h"
#include "jpg_hw.h"
#include "fbm_drvif.h" //Christie 07/0307
#include "jpg_debug.h"

#define JPEGVERIFY(X) \
	/* following do {} while (0) should be safe */ \
	LINT_SUPPRESS_NEXT_STATEMENT(717) \
	do \
	{ \
		i4Ret = (X); \
		if (i4Ret) \
		{ \
			goto _jpg_decode_err; \
		} \
	} while (0)

#define JPEGHEADER		((JPEG_JFIF_HEADER_T*)hParser)
#define HWDECODER		((JPEG_VLDJPG_DATA_T*)hDecoder)


static INT32 _jpg_BaselineDecodeYUV(JPEGHANDLE hParser, JPEGHANDLE hDecoder, YUV_BLOCK_T *prYUV)
{
	UINT8 u1Comp, u1Block, u1Count;
	INT32 i4Ret;

	UINT32 u4BlkAdv[D_MAX_JPEG_HW_COMP] = {0};
	UINT32 u4McuWidth, u4McuHeight;
	UINT32 u4AlignAddr;
	UINT32 au4RowSize[D_MAX_JPEG_HW_BLOCK] = {0};
	UINT8 *pu1Y, *pu1U, *pu1V;
	UINT32 u4Row, u4Mcu;
	FBM_POOL_T* prFbmPool; //Christie 07/0307

	ASSERT(hParser != 0);
	ASSERT(hDecoder != 0);
	ASSERT(prYUV != NULL);
	
	JPEGVERIFY( VLDJPG_SetGlobProgressive(hDecoder, FALSE) );

	if (JPEGHEADER->rSOF.u1NumComponents > D_MAX_JPEG_HW_COMP)
	{
		return -(INT32)E_JPG_ERR_UNSUPPORT;
	}

	JPEGVERIFY( VLDJPG_SetGlobNumComp(hDecoder, JPEGHEADER->rSOF.u1NumComponents) );

	for (u1Comp = 0; u1Comp < JPEGHEADER->rSOF.u1NumComponents; u1Comp++)
	{
		JPEGVERIFY( VLDJPG_SetGlobBlkWidth(hDecoder, u1Comp, JPEGHEADER->rSOF.arSofComp[u1Comp].u1HSampFactor) );
		JPEGVERIFY( VLDJPG_SetGlobBlkHeight(hDecoder, u1Comp, JPEGHEADER->rSOF.arSofComp[u1Comp].u1VSampFactor) );
		prYUV->aau4Factor[u1Comp][0] = JPEGHEADER->rSOF.arSofComp[u1Comp].u1HSampFactor;
		prYUV->aau4Factor[u1Comp][1] = JPEGHEADER->rSOF.arSofComp[u1Comp].u1VSampFactor;
	}

	JPEGVERIFY( VLDJPG_SetGlobPicWidth(hDecoder, (UINT32)JPEGHEADER->rSOF.u2ImageWidth) );
	JPEGVERIFY( VLDJPG_SetGlobPicHeight(hDecoder, (UINT32)JPEGHEADER->rSOF.u2ImageHeight) );

	for (u1Count = 0; u1Count < JPEGHEADER->rDHT.u4NumDcTbl; u1Count++)
	{
		if (JPEGHEADER->rDHT.fgDcTblLoaded & D_BIT(u1Count))
		{
			JPEGVERIFY( VLDJPG_SetGlobDcHuffTbl(hDecoder, u1Count, JPEGHEADER->rDHT.arDcTbl[u1Count].au1Bits, JPEGHEADER->rDHT.arDcTbl[u1Count].au1HuffVal) );
		}
		if (JPEGHEADER->rDHT.fgAcTblLoaded & D_BIT(u1Count))
		{
			JPEGVERIFY( VLDJPG_SetGlobAcHuffTbl(hDecoder, u1Count, JPEGHEADER->rDHT.arAcTbl[u1Count].au1Bits, JPEGHEADER->rDHT.arAcTbl[u1Count].au1HuffVal) );
		}
	}

	// if progressive, set nz buffer here
	// nz buffer size = mcu block size * 64 bits * total number of mcu

	for (u1Count = 0; u1Count < JPEGHEADER->rDQT.u1NumQ; u1Count++)
	{
		JPEGVERIFY( VLDJPG_SetGlobQtbl(hDecoder, u1Count, &JPEGHEADER->rDQT.aau1Qtbl[u1Count][0]) );
	}

	JPEGVERIFY( VLDJPG_SetRestartInterval(hDecoder, (UINT32)(JPEGHEADER->u2RestartInterval)) );

	JPEGVERIFY( VLDJPG_ValidateGlobData(hDecoder) );

	/*
	decode a baseline picture
	1. calculate 2 things
		a. how many blocks needed for each compnent (whole picture)
		b. how many blocks should ap to advance after decode 1 mcu
		c. how many mcu (whole picture)

	2. prepare scan data
	3. prepare mcu row data
	4. prepare misc data

	5. for each mcu
		a. assign write pointer of idct output
		b. decode
		c. advance write pointer of each component
	*/


	u4McuWidth =
		(HWDECODER->u4PicWidth + ((HWDECODER->u1MaxBlkWidth * D_DCTSIZE) - 1)) /
		(HWDECODER->u1MaxBlkWidth * D_DCTSIZE);
	u4McuHeight =
		(HWDECODER->u4PicHeight + ((HWDECODER->u1MaxBlkHeight * D_DCTSIZE) - 1)) /
		(HWDECODER->u1MaxBlkHeight * D_DCTSIZE);
	/*u4McuCnt = u4McuWidth * u4McuHeight;*/

	prYUV->au4MaxFactor[0] = HWDECODER->u1MaxBlkWidth;
	prYUV->au4MaxFactor[1] = HWDECODER->u1MaxBlkHeight;

	for (u1Comp = 0; u1Comp < HWDECODER->u1NumComps; u1Comp++)
	{
		u4BlkAdv[u1Comp] = HWDECODER->au1CompBlkWidth[u1Comp] * HWDECODER->au1CompBlkHeight[u1Comp] * D_DCTSIZE * D_DCTSIZE;
		if (HWDECODER->au1CompBlkWidth[u1Comp] != 2)
		{
			au4RowSize[u1Comp] =
				((u4McuWidth + 1) & (~1)) * u4BlkAdv[u1Comp];
			/*
				align each pixel row on 16 bytes boundary
			*/
		}
		else
		{
			au4RowSize[u1Comp] = u4McuWidth * u4BlkAdv[u1Comp];
		}
		u4BlkAdv[u1Comp] = HWDECODER->au1CompBlkWidth[u1Comp] * D_DCTSIZE;
	}

	// setup output structure and yuv buffer
	prYUV->u4Width = (UINT32)JPEGHEADER->rSOF.u2ImageWidth;
	prYUV->u4Height = (UINT32)JPEGHEADER->rSOF.u2ImageHeight;
	prYUV->u4McuWidth = u4McuWidth;
	prYUV->u4McuHeight = u4McuHeight;

	prYUV->u4SizeY = au4RowSize[0] * u4McuHeight;
	prYUV->u4SizeU = au4RowSize[1] * u4McuHeight;
	prYUV->u4SizeV = au4RowSize[2] * u4McuHeight;
	//prYUV->pvFreePoint = (void *)x_mem_alloc(prYUV->u4SizeY + prYUV->u4SizeU + prYUV->u4SizeV + 31 + 128);
      //Christie 07/0307, tmp FBM buffer use for verification
	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG); //for mt5372, output 64 M bound restriction
      ASSERT(prFbmPool != NULL);
      ASSERT(prFbmPool->u4Addr != NULL);
      //ASSERT(prFbmPool->u4Size > (prYUV->u4SizeY + prYUV->u4SizeU + prYUV->u4SizeV + 31 + 128));
      prYUV->pvFreePoint = (void *)prFbmPool->u4Addr;	

	if (prYUV->pvFreePoint == NULL)
	{
		// can not allocate
		i4Ret = -(INT32)E_JPG_ERR_GENERAL;
		goto _jpg_decode_err;
	}
	/*
	else
	{
		LOG(6, "MCU Count = %d, Blocks = %d, DCT Size = %d\n", u4McuCnt, HWDECODER->u1NumBlocks, D_DCTSIZE * D_DCTSIZE);
		LOG(6, "pvFreePoint allocate @ 0x%08x, size %d\n", prYUV->pvFreePoint, u4McuCnt * HWDECODER->u1NumBlocks * D_DCTSIZE * D_DCTSIZE + 31);
	}
	*/

	x_memset((void *)(prYUV->pvFreePoint), 0xcb, prYUV->u4SizeY + prYUV->u4SizeU + prYUV->u4SizeV + 31 + 128);

	u4AlignAddr = (UINT32)prYUV->pvFreePoint;
	u4AlignAddr = (u4AlignAddr + 64 + 0x1f) & ~(0x1f);	// start of a cache line, and OW
	prYUV->pu1Y = (UINT8 *)u4AlignAddr;
	prYUV->pu1U = (UINT8 *)prYUV->pu1Y + prYUV->u4SizeY;
	prYUV->pu1V = (UINT8 *)prYUV->pu1U + prYUV->u4SizeU;

        // sync ram and data
        #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396) ||defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)(prYUV->pvFreePoint), (UINT32)(prYUV->u4SizeY + prYUV->u4SizeU + prYUV->u4SizeV + 31 + 128));
        #else
        HalFlushInvalidateDCache();
        #endif
        
	// set scan
	JPEGVERIFY( VLDJPG_SetScanData(hDecoder, JPEGHEADER->arSOS[0].pu1ScanDataStart) );
	JPEGVERIFY( VLDJPG_SetScanCompInScan(hDecoder, JPEGHEADER->arSOS[0].u1CompInScan, (UINT8*)"\0\1\2") );
	for ((u1Comp = 0), (u1Block = 0); u1Comp <
		JPEGHEADER->arSOS[0].u1CompInScan; u1Comp++)
	{
		JPEGVERIFY( VLDJPG_SetScanCompQTbl(hDecoder, u1Comp,
			JPEGHEADER->rSOF.arSofComp[u1Comp].u1QuantTblNo) );
		for (u1Count = 0; u1Count <
			(JPEGHEADER->rSOF.arSofComp[u1Comp].u1HSampFactor *
			 JPEGHEADER->rSOF.arSofComp[u1Comp].u1VSampFactor);
			u1Count++, u1Block++)
		{
			JPEGVERIFY( VLDJPG_SetScanBlkDc(hDecoder, u1Block, JPEGHEADER->arSOS[0].au1DcId[u1Comp]) );
			JPEGVERIFY( VLDJPG_SetScanBlkAc(hDecoder, u1Block, JPEGHEADER->arSOS[0].au1AcId[u1Comp]) );
		}
	}

	JPEGVERIFY( VLDJPG_SetScanAlAh(hDecoder, JPEGHEADER->arSOS[0].u1AhAl) );
	JPEGVERIFY( VLDJPG_SetScanSeSs(hDecoder, JPEGHEADER->arSOS[0].u1Se, JPEGHEADER->arSOS[0].u1Ss) );
	JPEGVERIFY( VLDJPG_SetScanLast(hDecoder, TRUE) );
	JPEGVERIFY( VLDJPG_SetScanMcuRowStart(hDecoder, 0) );
	JPEGVERIFY( VLDJPG_SetScanMcuRowEnd(hDecoder, u4McuHeight - 1) );
	JPEGVERIFY( VLDJPG_ValidateScanData(hDecoder) );

	// misc
	JPEGVERIFY( VLDJPG_SetMiscDefBlkAddr(hDecoder) );
	JPEGVERIFY( VLDJPG_ValidateMiscData(hDecoder) );

	// prepare row
	pu1Y = prYUV->pu1Y;
	pu1U = prYUV->pu1U;
	pu1V = prYUV->pu1V;

	JPEGVERIFY( VLDJPG_Switch2CleanBuffer() );

	for (u4Row = 0; u4Row < u4McuHeight; u4Row++)
	{
		JPEGVERIFY( VLDJPG_SetMcuRowCurr(hDecoder, u4Row) );
		JPEGVERIFY( VLDJPG_SetMcuRowOutBuf(hDecoder, 0, pu1Y, au4RowSize[0]) );
		JPEGVERIFY( VLDJPG_SetMcuRowOutBuf(hDecoder, 1, pu1U, au4RowSize[1]) );
		JPEGVERIFY( VLDJPG_SetMcuRowOutBuf(hDecoder, 2, pu1V, au4RowSize[2]) );
		JPEGVERIFY( VLDJPG_ValidateMcuRowData(hDecoder) );
		for (u4Mcu = 0; u4Mcu < u4McuWidth; u4Mcu++)
		{
			// prepare mcu
			JPEGVERIFY( VLDJPG_SetMcuOutBuf(hDecoder, 0, pu1Y + (u4BlkAdv[0] * u4Mcu)) );
			JPEGVERIFY( VLDJPG_SetMcuOutBuf(hDecoder, 1, pu1U + (u4BlkAdv[1] * u4Mcu)) );
			JPEGVERIFY( VLDJPG_SetMcuOutBuf(hDecoder, 2, pu1V + (u4BlkAdv[2] * u4Mcu)) );
			JPEGVERIFY( VLDJPG_SetMcuCoefBuf(hDecoder, 0, pu1Y + (u4BlkAdv[0] * u4Mcu)) );
			JPEGVERIFY( VLDJPG_SetMcuCoefBuf(hDecoder, 1, pu1U + (u4BlkAdv[1] * u4Mcu)) );
			JPEGVERIFY( VLDJPG_SetMcuCoefBuf(hDecoder, 2, pu1V + (u4BlkAdv[2] * u4Mcu)) );
			//JPEGVERIFY( VLDJPG_SetMcuDefBuf );
			JPEGVERIFY( VLDJPG_ValidateMcuData(hDecoder) );
			// decode
			JPEGVERIFY( VLDJPG_DecodeOneMcu(hDecoder) );

			if ((u4Row == 0) && (u4Mcu == 0))
			{
				JPEGVERIFY( VLDJPG_Switch2KeepBuffer() );
			}

		}
		pu1Y += au4RowSize[0];
		pu1U += au4RowSize[1];
		pu1V += au4RowSize[2];
	}

	// sync ram and data
        #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396) ||defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)(prYUV->pvFreePoint), (UINT32)(prYUV->u4SizeY + prYUV->u4SizeU + prYUV->u4SizeV + 31 + 128));
        #else
        HalFlushInvalidateDCache();
        #endif
        
	//HalFlushDCache();
	//HalInvalidateDCache();

	/* LOG(6, " -- decode done\n"); */
	i4Ret = (INT32)E_JPG_OK;

_jpg_decode_err:
	return i4Ret;
}

INT32 JPG_DecodeYUV(void *pvJpegImage, UINT32 u4Size, YUV_BLOCK_T *prYUV)
{
	JPEGHANDLE hParser, hDecoder;
	INT32 i4Ret;

	ASSERT(prYUV != NULL);

	hParser = JFIF_AllocInstance();
	if (hParser == NULL)
	{
		return -(INT32)E_JPG_ERR_ALLOC_PARSER;
	}

	hDecoder = VLDJPG_AllocInstance();
	if (hDecoder == NULL)
	{
		return -(INT32)E_JPG_ERR_ALLOC_DECODER;
	}

	// initialize parser and decoder
	JPEGVERIFY( JFIF_New(hParser, pvJpegImage, u4Size) );
	JPEGVERIFY( VLDJPG_New(hDecoder) );

	if (JPEGHEADER->rSOF.eJpegFormat == E_JPG_BASELINE)
	{
		JPEGVERIFY( _jpg_BaselineDecodeYUV(hParser, hDecoder, prYUV) );
	}
	/*else if (JPEGHEADER->rSOF.eJpegFormat == E_JPG_PROGRESSIVE_HUFFMAN)
	{
		JPEGVERIFY( JFIF_ParseProgScan(hParser) );
		JPEGVERIFY( _jpg_ProgressiveDecodeYUV(hParser, hDecoder, prYUV) );
	}*/
	else
	{
	      LOG(6, "Format %d Unsupport\n", JPEGHEADER->rSOF.eJpegFormat);
		i4Ret = -(INT32)E_JPG_ERR_UNSUPPORT;
	}

_jpg_decode_err:
	JFIF_FreeInstance(hParser);
	VLDJPG_FreeInstance(hDecoder);
	return i4Ret;
}


