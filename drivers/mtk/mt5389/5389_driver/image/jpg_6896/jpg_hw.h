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
#if ! defined(JPG_HW_H)
#define JPG_HW_H


#ifndef JPEG_VLD_V5
#if defined(CC_MT5398)
#define JPEG_VLD_V5
#endif
#endif

#ifndef JPEG_VLD_V4
#if defined(CC_MT5368)||defined(CC_MT5396) ||defined(CC_MT5389)||defined(CC_MT5398)
#define JPEG_VLD_V4
#define JDEC_QTABLE_RISC_MODE
//#define MT5368_JPG_PARTIAL_OUTPUT
//#define MT5368_VFIFO_RWPTR_TEST
#endif
#endif

#ifndef JPEG_VLD_V3
#if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396) ||defined(CC_MT5389)||defined(CC_MT5398)
#define JPEG_VLD_V3
#endif
#endif

#ifndef JPEG_VLD_V2
#if defined(CC_MT5387) && !defined(CC_MT5363)
#define JPEG_VLD_V2
#endif
#endif

#if !defined(JPEG_VLD_V3) && !defined(JPEG_VLD_V2)
#ifndef JPEG_VLD_V1
#define JPEG_VLD_V1
#endif
#endif

#if (defined(CC_MT5365) || defined(CC_MT5395)) && !defined(JPG_CLK_FROM_GRAPHIC)
#define JPG_CLK_FROM_GRAPHIC
#endif

#if defined(CC_MT5395) && !defined(JPG_CLK_FROM_GRAPHIC_5395_PLL)
#define JPG_CLK_FROM_GRAPHIC_5395_PLL
#endif

#if	defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#define JPG_CLK_FROM_JPGDEC_CKCFG
#endif

#define D_MAX_JPEG_HW_COMP		3
#define D_MAX_JPEG_HUFF_TBL		4
#define D_MAX_JPEG_QTBL			4
#define D_MAX_JPEG_HW_QTBL		2
#define D_MAX_JPEG_HW_BLOCK		10
#define D_HUFF_LOOKAHEAD_BITS	6	/* # of bits of lookahead */

#define D_DCTSIZE				8
#define D_JPEG_DRAM_ALIGN_MASK	0xf
#define D_JPEG_DRAM_ALIGN_SIZE	(D_JPEG_DRAM_ALIGN_MASK + 1)

#define D_JPEG_BASELINE			0
#define D_JPEG_DC_REFINE		1
#define D_JPEG_AC_FIRST			2
#define D_JPEG_AC_REFINE		3
#define D_JPEG_DC_FIRST			4

//#define D_JPEG_REG_DUMP
//#define D_JPEG_REG_DEBUG        // enabled for debug
//#define D_JPEG_HWFUNC_DEBUG     // enabled for debug



#define STUPID_JPEG_DELAY(XLOOP) \
	do \
	{ \
		INT32 i4StupidDelay = XLOOP; \
		for (;i4StupidDelay > 0; i4StupidDelay--); \
	} while (0)

typedef struct _DERIVED_HUFF_TABLE_T
{
	/* Basic tables: (element [0] of each array is unused) */
	INT32	ai4Maxcode[18];
		/*
			largest code of length k (-1 if none)
			(maxcode[17] is a sentinel to ensure jpeg_huff_decode terminates)
		*/
	INT32	ai4Valoffset[17];
		/*
			huffval[] offset for codes of length k
			valoffset[k] = huffval[] index of 1st symbol of code length k,
			less the smallest code of length k; so given a code of length k,
			thecorresponding symbol is huffval[code + valoffset[k]]
		*/
	INT32	ai4LookNbits[1 << D_HUFF_LOOKAHEAD_BITS];
		/* # bits, 0 if too long */
	UINT8	au1LookSym[1 << D_HUFF_LOOKAHEAD_BITS];
		/* symbol, or unused */
} DERIVED_HUFF_TABLE_T;

/* global data check flags */
#define D_FLAG_NONE				(0)

#define D_GLOB_PROGRESSIVE		(1 << 0)
#define D_GLOB_NCOMP			(1 << 1)
#define D_GLOB_BLK_W			(1 << 2)
#define D_GLOB_BLK_H			(1 << 3)
#define D_GLOB_PIC_W			(1 << 4)
#define D_GLOB_PIC_H			(1 << 5)
#define D_GLOB_DCHUFF			(1 << 6)	/* no need */
#define D_GLOB_ACHUFF			(1 << 7)	/* no need */
#define D_GLOB_NZBUFF			(1 << 8)	/* no need */
#define D_GLOB_QTBL				(1 << 9)	/* no need */
#define D_GLOB_RESTART			(1 << 10)
/* scan data check flags */
#define D_SCAN_DATASTART		(1 << 0)
#define D_SCAN_NCOMP			(1 << 1)
#define D_SCAN_COMPIN			(1 << 2)
#define D_SCAN_COMPQ			(1 << 3)
#define D_SCAN_BLKDC			(1 << 4)
#define D_SCAN_BLKAC			(1 << 5)
#define D_SCAN_ALAH				(1 << 6)
#define D_SCAN_SESS				(1 << 7)
#define D_SCAN_LAST				(1 << 8)
#define D_SCAN_ROWSTART			(1 << 9)
#define D_SCAN_ROWEND			(1 << 10)
#define D_SCAN_DCHUFF			(1 << 11)
#define D_SCAN_ACHUFF			(1 << 12)
#define D_SCAN_QTBL				(1 << 13)
#define D_SCAN_NZBUFF			(1 << 14)
/* mcu row check flags */
#define D_ROW_CURR				(1 << 0)
#define D_ROW_OUTBUF			(1 << 1)
#define D_ROW_OUTBUFSIZE		(1 << 2)
/* mcu check flags */
#define D_MCU_OUTADDR			(1 << 0)
#define D_MCU_COEFADDR			(1 << 1)
/* misc check flags */
#define D_MISC_ADDR_X			(1 << 0)
#define D_MISC_ADDR_Y			(1 << 1)

typedef struct _JPEG_VLDJPG_DATA_T
{
	JPEG_BASE_CLASS_T	rParent;
	/* global */
	UINT32	fgGlobSet;
	BOOL	fgVldJpgDataReady;
	BOOL	fgProgressive;
	UINT8	u1NumComps;								// number of components in jpeg file
	UINT8	au1CompBlkWidth[D_MAX_JPEG_HW_COMP];	// component width in block of an mcu
	UINT8	au1CompBlkHeight[D_MAX_JPEG_HW_COMP];	// component height in block of an mcu
	UINT8	u1MaxBlkWidth, u1MaxBlkHeight;			// widest, highest
	UINT32	u4PicWidth, u4PicHeight;				// picture width and height
	UINT32	u4McuPerRow;
	UINT32	u4CompPitchRow[D_MAX_JPEG_HW_COMP];		// au1CompBlkWidth[n] * 8 * u4McuPerRow
													// byte pitch of a component
	DERIVED_HUFF_TABLE_T arDcHuffTable[4];
	DERIVED_HUFF_TABLE_T arAcHuffTable[4];

	//UINT8	au1HwDcHuffTbl[(1024 * 4) + 15];			// storage area of hw huff tables
	//UINT8	au1HwAcHuffTbl[(1024 * 4) + 15];			// for each table, 256 bytes data,
													// every 128bits use only 32 bits
													// hence 1024 bytes needed for one table
	UINT8	*pu1HwDcHuffTbl;						// aligned address, hw huff tables
	UINT8	*pu1HwAcHuffTbl;

	UINT32	fgAcHuff;								// bit pattern of loaded ac table
													// use only in look ahead table
	UINT8	u1NumBlocks;
	UINT8	au1MemberComp[D_MAX_JPEG_HW_BLOCK];		// indicates a block is a member of which component
	UINT8	*pu1NonZeroBuffer;
	UINT8	*apu1Qtbl[D_MAX_JPEG_QTBL];				// q tables in stream
	UINT32	u4RestartInterval;
	UINT32	u4CurrRestartInterval;
	UINT32	u4McuCount;
	/* current scan */
	UINT32	fgScanSet;
	BOOL	fgScanReady;
	UINT8	*pu1DataStart;
	BOOL	fgRingBuffer;
	UINT32	u4RingBufferSize;
	UINT8	*pu1RingBufStart;
	UINT8	u1NumCompsInCurrScan;					// number of components in current scan
	BOOL	afgCompInScan[D_MAX_JPEG_HW_COMP + 1];
	UINT8	au1CompQ[D_MAX_JPEG_HW_COMP];
	BOOL	afgQtblLoad[D_MAX_JPEG_HW_QTBL];		// vld q table loaded
	UINT8	au1QtblLoadIndex[D_MAX_JPEG_HW_QTBL];	// hw index to stream q tbl index
	UINT8	au1BlkDc[D_MAX_JPEG_HW_BLOCK];
	UINT8	au1BlkAc[D_MAX_JPEG_HW_BLOCK];
	UINT8	au1DcNeed[D_MAX_JPEG_HW_BLOCK];
	UINT8	au1AcNeed[D_MAX_JPEG_HW_BLOCK];
	UINT8	u1AlAh;
	UINT8	u1Se, u1Ss;
	BOOL	fgLastScan;
	UINT32	u4McuRowStart, u4McuRowEnd;				// mcu row range that can output
	UINT32	u4ScanMcuCount;
	/* current mcu row */
	UINT32	fgRowSet;
	BOOL	fgMcuRowReady;
	UINT32	u4CurrMcuRow;
	UINT8	*apu1OutputBuffer[D_MAX_JPEG_HW_COMP];
	UINT8	*apu1OutputBuffer1[D_MAX_JPEG_HW_COMP];
	UINT32	au4OutputBufferSize[D_MAX_JPEG_HW_COMP];
	UINT32	u4RowMcuCount;
	/* current mcu */
	UINT32	fgMcuSet;
	BOOL	fgMcuDataReady;
	UINT32	u4CurrMcuInRow;
	UINT8	*apu1OutputAddr[D_MAX_JPEG_HW_COMP];
	UINT8	*apu1CoefAddr[D_MAX_JPEG_HW_COMP];
	/* misc ... can be global or mcu data */
	UINT32	fgMiscSet;
	BOOL	fgMiscDataReady;
	UINT32	au4BlockAddrX[D_MAX_JPEG_HW_BLOCK];
	UINT32	au4BlockAddrY[D_MAX_JPEG_HW_BLOCK];
	/* eob run */
	INT32	i4SavedEOBRUN;
	UINT32	u4ProgMode;	/* dc_first dc_refine ac_first ac_refine */
	/* for single component in p scan */
	UINT8	u1FirstCompInScan;
	UINT32 u4SkipMcuRowNum;
} JPEG_VLDJPG_DATA_T;


void  VLDJPG_SetDecMode5391(INT32 i4DecMode);
INT32 VLDJPG_GetDecMode5391(void);
INT32 VLDJPG_BarrelShifterSkipBytes(UINT32 u4Bytes);
INT32 VLDJPG_CalcDerivedHuffTable(const UINT8 *au1Bits, const UINT8 *au1HuffVal, DERIVED_HUFF_TABLE_T *prDerivedHuffTbl);
INT32 VLDJPG_CheckDecodeFinish(void);
INT32 VLDJPG_DecodeMcu(void);
INT32 VLDJPG_GetEndOfBandRun(UINT32 *pu4EndOfBandRun);
INT32 VLDJPG_LoadAcLookAheadTable(UINT8 u1HuffTblNo, const DERIVED_HUFF_TABLE_T *prDerivedHuffTbl);
void VLDJPG_SetMaxcodeSequence(INT32 u4Value);
INT32 VLDJPG_LoadMaxcode(const INT32 *ai4Value);
void VLDJPG_TransHuffTblToHwFormat(UINT8 *pu1DstAddr, const UINT8 *au1Huffval);
INT32 VLDJOG_SetHuffAddr(UINT8 *pu1DcHuff, UINT8 *pu1AcHuff);
void VLDJPG_SetRiscCtrlWP(UINT32 u4VldWptr);
INT32 VLDJPG_LoadTblQ(UINT8 u1Qn, UINT8 *pu1Qtbl);
INT32 VLDJPG_ResetDcPrediction(void);
INT32 VLDJPG_SetAcNeededList(UINT32 u4AcNeededList);
INT32 VLDJPG_SetAcTableList(UINT32 u4AcTableList);
INT32 VLDJPG_SetBarrelShifter(UINT8 *pu1MemAddr);
INT32 VLDJPG_SetBlockAddrList(const UINT32 *au4XAddrList, const UINT32 *au4YAddrList);
INT32 VLDJPG_SetBlockCount(UINT32 u4BlockCount);
INT32 VLDJPG_SetCompCoefPitch(const UINT32 *au4CoefPitch);
INT32 VLDJPG_SetCompIdctPitch(const UINT32 *au4CompPitch);
INT32 VLDJPG_SetCompReadAddr(UINT32 u4Comp, void *avCompReadAddr);
INT32 VLDJPG_SetCompWriteAddr(UINT32 u4Comp, void *avCompWriteAddr);
INT32 VLDJPG_SetCurrMCUNum(UINT32 u4CurrMCUNum);
INT32 VLDJPG_SetDcNeededList(UINT32 u4DcNeededList);
INT32 VLDJPG_SetDcTableList(UINT32 u4DcTableList);
INT32 VLDJPG_SetEndOfBandRun(UINT32 u4EndOfBandRun);
INT32 VLDJPG_SetMemberShip(UINT32 u4Membership);
INT32 VLDJPG_SetProgressiveAhAl(UINT8 u1AhAl);
INT32 VLDJPG_SetProgressiveLastScan(BOOL fgLastScan);
INT32 VLDJPG_SetProgressiveMode(UINT8 u1ProgMode);
INT32 VLDJPG_SetProgressiveNoneZeroHistory(UINT8 *pu1NoneZeroHistory);
INT32 VLDJPG_SetProgressiveNzOnly(BOOL fgWriteNzOnly);
INT32 VLDJPG_SetProgressiveSeSs(UINT8 u1Se, UINT8 u1Ss);
INT32 VLDJPG_SetQuanTableList(UINT32 u4QuanTableList);
INT32 VLDJPG_Switch2Baseline(void);
INT32 VLDJPG_Switch2CleanBuffer(void);
INT32 VLDJPG_Switch2Jpeg(void);
INT32 VLDJPG_Switch2KeepBuffer(void);
INT32 VLDJPG_Switch2Mpeg(void);
INT32 VLDJPG_Switch2Progressive(void);
INT32 VLDJPG_SoftwareReset(void);
INT32 VLDJPG_Restart(void);
INT32 VLDJPG_AdvanceRestartMarker(UINT8 *pu1Marker);
INT32 VLDJPG_InitScan(void);
INT32 VLDJPG_CheckVideoInputWindow(void);
INT32 VLDJPG_SetRingBuffer(void *pvStartAddr, UINT32 u4ByteSize);
void VLDJPG_SetLinearBuffer(void);

//=========================interfaces=======================

/* picture hw parameters */
INT32 VLDJPG_SetGlobProgressive		(JPEGHANDLE hInstance, BOOL fgProgressive);
INT32 VLDJPG_SetGlobNumComp			(JPEGHANDLE hInstance, UINT8 u1NumComp);
INT32 VLDJPG_SetGlobBlkWidth		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 u1BlkWidth);
INT32 VLDJPG_SetGlobBlkHeight		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 u1BlkHeight);
INT32 VLDJPG_SetGlobPicWidth		(JPEGHANDLE hInstance, UINT32 u4Width);
INT32 VLDJPG_SetGlobPicHeight		(JPEGHANDLE hInstance, UINT32 u4Height);
INT32 VLDJPG_SetGlobDcHuffTbl		(JPEGHANDLE hInstance, UINT8 u1DcTblNo, const UINT8 *au1Bits, const UINT8 *au1Huffval);
INT32 VLDJPG_SetGlobAcHuffTbl		(JPEGHANDLE hInstance, UINT8 u1AcTblNo, const UINT8 *au1Bits, const UINT8 *au1Huffval);
INT32 VLDJPG_SetGlobNonZeroBuf		(JPEGHANDLE hInstance, UINT8 *pu1NzHistoryBuffer);
INT32 VLDJPG_SetGlobQtbl			(JPEGHANDLE hInstance, UINT8 u1QIdx, UINT8 *pu1Qtbl);
INT32 VLDJPG_SetRestartInterval		(JPEGHANDLE hInstance, UINT32 u4RestartInterval);
INT32 VLDJPG_ValidateGlobData		(JPEGHANDLE hInstance);
/* scan hw parameters */
INT32 VLDJPG_SetScanData			(JPEGHANDLE hInstance, UINT8 *pu1ScanDataStart);
INT32 VLDJPG_SetScanCompInScan		(JPEGHANDLE hInstance, UINT8 u1CompInScan, const UINT8 *au1CompNoList);
INT32 VLDJPG_SetScanCompQTbl		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 u1QTbl);
INT32 VLDJPG_SetScanBlkDc			(JPEGHANDLE hInstance, UINT8 u1Block, UINT8 u1Dc);
INT32 VLDJPG_SetScanBlkAc			(JPEGHANDLE hInstance, UINT8 u1Block, UINT8 u1Ac);
INT32 VLDJPG_SetScanAlAh			(JPEGHANDLE hInstance, UINT8 u1AlAh);
INT32 VLDJPG_SetScanSeSs			(JPEGHANDLE hInstance, UINT8 u1Se, UINT8 u1Ss);
INT32 VLDJPG_SetScanLast			(JPEGHANDLE hInstance, BOOL fgLastScan);
INT32 VLDJPG_SetScanMcuRowStart		(JPEGHANDLE hInstance, UINT32 u4McuRowStart);
INT32 VLDJPG_SetScanMcuRowEnd		(JPEGHANDLE hInstance, UINT32 u4McuRowEnd);
INT32 VLDJPG_ValidateScanData		(JPEGHANDLE hInstance);
/* mcu row hw parameters */
INT32 VLDJPG_SetMcuRowCurr			(JPEGHANDLE hInstance, UINT32 u4CurrMcuRow);
INT32 VLDJPG_SetMcuRowOutBuf		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 *pu1OutBuffer, UINT32 u4OutBufSize);
INT32 VLDJPG_ValidateMcuRowData		(JPEGHANDLE hInstance);
/* mcu hw parameters */
INT32 VLDJPG_SetMcuOutBuf			(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 *pu1CompOutAddr);
INT32 VLDJPG_SetMcuCoefBuf			(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 *pu1CompCoefAddr);
INT32 VLDJPG_SetMcuDefBuf			(JPEGHANDLE hInstance);
INT32 VLDJPG_ValidateMcuData		(JPEGHANDLE hInstance);
/* misc hw parameters (must set) */
INT32 VLDJPG_SetMiscBlkAddrX		(JPEGHANDLE hInstance, UINT8 u1Block, UINT32 u4AddrX);
INT32 VLDJPG_SetMiscBlkAddrY		(JPEGHANDLE hInstance, UINT8 u1Block, UINT32 u4AddrY);
INT32 VLDJPG_SetMiscDefBlkAddr		(JPEGHANDLE hInstance);
INT32 VLDJPG_ValidateMiscData		(JPEGHANDLE hInstance);
/* clear interface */
INT32 VLDJPG_ClearGlobFlag			(JPEGHANDLE hInstance);
INT32 VLDJPG_ClearScanFlag			(JPEGHANDLE hInstance);
INT32 VLDJPG_ClearRowFlag			(JPEGHANDLE hInstance);
INT32 VLDJPG_ClearMcuFlag			(JPEGHANDLE hInstance);
INT32 VLDJPG_ClearMiscFlag			(JPEGHANDLE hInstance);
INT32 VLDJPG_DecodeOneMcu			(JPEGHANDLE hInstance);
INT32 VLDJPG_DecodeOnePic(JPEGHANDLE hInstance);
INT32 VLDJPG_CheckDecodePicFinish(JPEGHANDLE hInstance);
INT32 VLDJPG_CheckDecodingError(JPEGHANDLE hInstance);
INT32 VLDJPG_SetStop(JPEGHANDLE hInstance);
INT32 VLDJPG_CheckStop(JPEGHANDLE hInstance);
INT32 VLDJPG_SetSkipMcuRowNum		(JPEGHANDLE hInstance, UINT32 u4SkipMcuRowNum);

extern INT32 VLDJPG_CheckVideoReadPointer(void);

extern INT32 VLDJPG_ReadPointer(void);

extern INT32 VLDJPG_SetRiscControlWptr(void);

extern INT32 VLDJPG_ReadRiscCtrlWP(void);

extern INT32 VLDJPG_SetMcuRowOutBufBank15391(JPEGHANDLE hInstance, 
            UINT8 u1Comp, UINT8 *pu1OutBuffer, UINT32 u4OutBufSize);

extern INT32 VLDJPG_ValidateMcuRowData5391(JPEGHANDLE hInstance, 
        UINT32 u4MCUsPerRow, UINT32 u4TotalRows);

extern INT32 VLDJPG_DecodeOneMcuRow5391(JPEGHANDLE hInstance, UINT32 u4CurrMcuRow);

extern INT32 VLDJPG_DecodeOnePic5391(JPEGHANDLE hInstance, UINT32 u4CurrMcuRow);

extern INT32 VLDJPG_SetCompReadAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr);

extern INT32 VLDJPG_SetCompWriteAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr);

extern void VLDJPG_SetOutputBank0BufAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr);

extern void VLDJPG_SetOutputBank1BufAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr);

extern void VLDJPG_SetMCUWidth5391(UINT8 compNum, UINT32 u4Comp0MCUWidth, UINT32 u4Comp1MCUWidth, UINT32 u4Comp2MCUWidth);

extern void VLDJPG_SetPicSize5391(UINT32 u4MCUsPerRow, UINT32 u4TotalRows);

extern void VLDJPG_SetRestartInterval5391(UINT32 u4RestartInterval);

extern void JPG_SetDecMode5391(INT32 i4DecMode);

extern void VLDJPG_SetDecMode5391(INT32 i4DecMode);

extern INT32 VLDJPG_GetDecMode5391(void);

extern INT32 VLDJPG_DecodeFirstMcuRow5391(void);

extern INT32 VLDJPG_DecodeNextMcuRow5391(void);

extern INT32 VLDJPG_DecodePic5391(void);

extern INT32 VLDJPG_SetCurrMcuRowNum5391(UINT32 u4CurrMcuRowNum);

extern void VLDJPG_SetUserDefQMatrix(void);

extern void VLDJPG_DecodePic(void);

extern INT32 VLDJPG_SetHuffTblVal(UINT8 *au1Huffval, BOOL fgDCTbl);

extern void VLDJPG_SetPowerOff(void);

extern void VLDJPG_UseNewEngine(void);

extern void VLDJPG_SetErrorConcealment(void);

extern void VLDJPG_SetErrorDetection(void);

extern INT32 VLDJPG_DetectError(void);

extern INT32 VLDJPG_StopHw(void);

extern BOOL VLDJPG_CheckHwStop(void);
extern INT32 VLDJPG_GetChipType(void);

#ifdef JPEG_VLD_V4
extern INT32 VLDJPG_SetVideoOutMode(BOOL fgPadding16Byte, BOOL fgVideoOutput,UINT32 u4Comp0MCUWidth, UINT32 u4Comp1MCUWidth, UINT32 u4Comp0MCUHeight, UINT32 u4Comp1MCUHeight);

extern INT32 VLDJPG_SetPicturePixelSize(UINT32 u4PicPixelSize);

extern INT32 VLDJPG_SetPartialMode_StartRow(UINT32 u4StartRow);

extern void VLDJPG_SetMMU(void);

extern UINT32 VLDJPG_ReadErrorType(void);

extern UINT32 VLDJPG_ReadCKsum(void);

extern UINT32 VLDJPG_ReadCRC(void);

extern UINT32 VLDJPG_ReadCurMcuRow(void);
#endif

#endif // JPG_HW_H
