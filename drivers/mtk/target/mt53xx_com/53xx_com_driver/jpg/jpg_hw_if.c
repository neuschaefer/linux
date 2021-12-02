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
#include "x_hal_5381.h"
#include "x_bim.h"
#include "jpg_common.h"
#include "jpg_if.h"
#include "jpg_hw.h"
#include "jpg_debug.h"

#define M_JPEG_INTERFACE_START() \
	INT32				i4Ret = (INT32)E_HWJPG_OK; \
	JPEG_VLDJPG_DATA_T	*prJpgData

#define M_JPEG_INTERFACE_END() \
	return i4Ret

#define M_JPEG_HANDLE_VALIDATE() \
	prJpgData = (JPEG_VLDJPG_DATA_T *)hInstance; \
	ASSERT(prJpgData != 0); \
	ASSERT(prJpgData->rParent.eJpegClass == E_JC_VLD_DECODER); \
	ASSERT(prJpgData->rParent.zObjectSize == sizeof(JPEG_VLDJPG_DATA_T))


#define INSTANCE prJpgData

//extern void HalFlushDCache(void);
extern void HalFlushInvalidateDCache(void);

//static void _vldjpg_SaveHuffTbl(UINT8 *pu1DstAddr, const UINT8 *au1Huffval);

/* picture hw parameters */
INT32 VLDJPG_SetGlobProgressive		(JPEGHANDLE hInstance, BOOL fgProgressive)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->fgProgressive = fgProgressive;
		INSTANCE->fgGlobSet		|= D_GLOB_PROGRESSIVE;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobNumComp			(JPEGHANDLE hInstance, UINT8 u1NumComp)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u1NumComps	= u1NumComp;
		INSTANCE->fgGlobSet		|= D_GLOB_NCOMP;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobBlkWidth		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 u1BlkWidth)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		if (u1Comp < D_MAX_JPEG_HW_COMP)
		{
			INSTANCE->au1CompBlkWidth[u1Comp] = u1BlkWidth;
			INSTANCE->fgGlobSet	|= D_GLOB_BLK_W;
		}
		else
		{
			i4Ret = -(INT32)E_HWJPG_ERR_COMP_RANGE;
		}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobBlkHeight		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 u1BlkHeight)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		if (u1Comp < D_MAX_JPEG_HW_COMP)
		{
			INSTANCE->au1CompBlkHeight[u1Comp] = u1BlkHeight;
			INSTANCE->fgGlobSet	|= D_GLOB_BLK_H;
		}
		else
		{
			i4Ret = -(INT32)E_HWJPG_ERR_COMP_RANGE;
		}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobPicWidth		(JPEGHANDLE hInstance, UINT32 u4Width)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u4PicWidth	= u4Width;
		INSTANCE->fgGlobSet		|= D_GLOB_PIC_W;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobPicHeight		(JPEGHANDLE hInstance, UINT32 u4Height)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u4PicHeight	= u4Height;
		INSTANCE->fgGlobSet		|= D_GLOB_PIC_H;
	M_JPEG_INTERFACE_END();
}

#if 0
static void _vldjpg_SaveHuffTbl(UINT8 *pu1DstAddr, const UINT8 *au1Huffval)
{
	UINT8 *pu1Huff;
	INT32 i4Index;

	ASSERT(au1Huffval != NULL);
	ASSERT(pu1DstAddr != NULL);

	pu1Huff = pu1DstAddr;

	for (i4Index = 0; i4Index < 256; (i4Index += 4), (pu1Huff += 16))
	{
		// every 128 bits, write only 32 bits data, rest 96 bits fill zero
		//x_memcpy(pu1Huff, &au1Huffval[i4Index], 4);
		pu1Huff[0] = au1Huffval[i4Index + 0];
		pu1Huff[1] = au1Huffval[i4Index + 1];
		pu1Huff[2] = au1Huffval[i4Index + 2];
		pu1Huff[3] = au1Huffval[i4Index + 3];
		x_memset(pu1Huff + 4, 0, 12);
	}
	//HalFlushDCache();
	HalFlushInvalidateDCache();
}
#endif

INT32 VLDJPG_SetGlobDcHuffTbl		(JPEGHANDLE hInstance, UINT8 u1DcTblNo, const UINT8 *au1Bits, const UINT8 *au1Huffval)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();        

	if (u1DcTblNo < D_MAX_JPEG_HUFF_TBL)
	{
		//_vldjpg_SaveHuffTbl((UINT8 *)VIRTUAL(INSTANCE->pu1HwDcHuffTbl + (u1DcTblNo * 1024)), au1Huffval);
		i4Ret = VLDJPG_CalcDerivedHuffTable(au1Bits, au1Huffval, &INSTANCE->arDcHuffTable[u1DcTblNo]);
		if ((INT32)E_HWJPG_OK == i4Ret)
		{
			INSTANCE->fgGlobSet	|= D_GLOB_DCHUFF;
			INSTANCE->fgScanSet	|= D_SCAN_DCHUFF;
		}
        VLDJPG_TransHuffTblToHwFormat((UINT8 *)VIRTUAL(INSTANCE->pu1HwDcHuffTbl + (u1DcTblNo * 256)), au1Huffval);
	}
	else
	{
		i4Ret = -(INT32)E_HWJPG_ERR_TBL_RANGE;
	}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobAcHuffTbl		(JPEGHANDLE hInstance, UINT8 u1AcTblNo, const UINT8 *au1Bits, const UINT8 *au1Huffval)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();

	if (u1AcTblNo < D_MAX_JPEG_HUFF_TBL)
	{
		//_vldjpg_SaveHuffTbl((UINT8 *)VIRTUAL(INSTANCE->pu1HwAcHuffTbl + (u1AcTblNo * 1024)), au1Huffval);
		i4Ret = VLDJPG_CalcDerivedHuffTable(au1Bits, au1Huffval, &INSTANCE->arAcHuffTable[u1AcTblNo]);
		if ((INT32)E_HWJPG_OK == i4Ret)
		{
			INSTANCE->fgAcHuff	|= D_BIT(u1AcTblNo);
			INSTANCE->fgGlobSet	|= D_GLOB_ACHUFF;
			INSTANCE->fgScanSet	|= D_SCAN_ACHUFF;
		}
        VLDJPG_TransHuffTblToHwFormat((UINT8 *)VIRTUAL(INSTANCE->pu1HwAcHuffTbl + (u1AcTblNo * 256)), au1Huffval);
	}
	else
	{
		i4Ret = -(INT32)E_HWJPG_ERR_TBL_RANGE;
	}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobNonZeroBuf		(JPEGHANDLE hInstance, UINT8 *pu1NzHistoryBuffer)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->pu1NonZeroBuffer	= pu1NzHistoryBuffer;
		INSTANCE->fgGlobSet			|= D_GLOB_NZBUFF;
		INSTANCE->fgScanSet			|= D_SCAN_NZBUFF;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetGlobQtbl			(JPEGHANDLE hInstance, UINT8 u1QIdx, UINT8 *pu1Qtbl)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		if (u1QIdx < D_MAX_JPEG_QTBL)
		{
			INSTANCE->apu1Qtbl[u1QIdx]	= pu1Qtbl;
			INSTANCE->fgGlobSet			|= D_GLOB_QTBL;
			INSTANCE->fgScanSet			|= D_SCAN_QTBL;
		}
		else
		{
			i4Ret = -(INT32)E_HWJPG_ERR_TBL_RANGE;
		}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetRestartInterval		(JPEGHANDLE hInstance, UINT32 u4RestartInterval)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u4RestartInterval = u4RestartInterval;
		INSTANCE->u4CurrRestartInterval = u4RestartInterval;
		INSTANCE->fgGlobSet			|= D_GLOB_RESTART;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ValidateGlobData		(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
		UINT32 fgFlagsCheck =
			(UINT32)(D_GLOB_PROGRESSIVE | D_GLOB_NCOMP | D_GLOB_BLK_W | D_GLOB_BLK_H |
			D_GLOB_PIC_W | D_GLOB_PIC_H | D_GLOB_RESTART);
			//D_GLOB_QTBL is not a must according to KB Lee
		//UINT8 u1Comp;
		UINT8 u1Block;
		UINT32 u4Count;

	M_JPEG_HANDLE_VALIDATE();

		/* progressive, check nz-buffer */
		if (INSTANCE->fgProgressive)
		{
			fgFlagsCheck |= 0;
		}
		else
		{
			fgFlagsCheck |= D_GLOB_DCHUFF | D_GLOB_ACHUFF | D_GLOB_QTBL;
		}

		if ((fgFlagsCheck & INSTANCE->fgGlobSet) != fgFlagsCheck)
		{
			
			Printf("Glob validation failed : flags_set 0x%08x flags_check 0x%08x\n",
				INSTANCE->fgGlobSet, fgFlagsCheck);
			
			INSTANCE->fgVldJpgDataReady = FALSE;
			return -(INT32)E_HWJPG_ERR_GLOB_NOT_READY;
		}

		/*
		calculate max block width and height
		since there are only 3 hardware component
		the comparisons are hardcoded
		*/
		// width
		INSTANCE->u1MaxBlkWidth		= MAX(INSTANCE->au1CompBlkWidth[0],
										  INSTANCE->au1CompBlkWidth[1]);
		INSTANCE->u1MaxBlkWidth		= MAX(INSTANCE->u1MaxBlkWidth,
										  INSTANCE->au1CompBlkWidth[2]);
		// height
		INSTANCE->u1MaxBlkHeight	= MAX(INSTANCE->au1CompBlkHeight[0],
										  INSTANCE->au1CompBlkHeight[1]);
		INSTANCE->u1MaxBlkHeight	= MAX(INSTANCE->u1MaxBlkHeight,
										  INSTANCE->au1CompBlkHeight[2]);

		/* calculate mcu per row */
		INSTANCE->u4McuPerRow		= (INSTANCE->u4PicWidth + ((INSTANCE->u1MaxBlkWidth * D_DCTSIZE) - 1)) /
										(INSTANCE->u1MaxBlkWidth * D_DCTSIZE);

		/* calculate absolute "block in which component" */
		/* this will be used via a abs-component to hw-component map */
		u1Block = 0;
		for (u4Count = 0; u4Count <
			((UINT32)INSTANCE->au1CompBlkWidth[0] *
			(UINT32)INSTANCE->au1CompBlkHeight[0]); u4Count++)
		{
		    if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
		    {
		        u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
		    }
			INSTANCE->au1MemberComp[u1Block++] = 0;
		}
		for (u4Count = 0; u4Count <
			((UINT32)INSTANCE->au1CompBlkWidth[1] *
			(UINT32)INSTANCE->au1CompBlkHeight[1]); u4Count++)
		{
		    if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
		    {
		        u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
		    }
			INSTANCE->au1MemberComp[u1Block++] = 1;
		}
		for (u4Count = 0; u4Count <
			((UINT32)INSTANCE->au1CompBlkWidth[2] *
			(UINT32)INSTANCE->au1CompBlkHeight[2]); u4Count++)
		{
		    if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
		    {
		        u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
		    }
			INSTANCE->au1MemberComp[u1Block++] = 2;
		}
		for (;u1Block < D_MAX_JPEG_HW_BLOCK; u1Block++)
		{
			/* set rest blocks to a member of component that does not exists */
			INSTANCE->au1MemberComp[u1Block] = 3;
		}


		/*
		validate all global data
		*/
		if ((INSTANCE->u1MaxBlkWidth < 1) || (INSTANCE-> u1MaxBlkWidth > 2))
		{
			return -(INT32)E_HWJPG_ERR_BLOCK;
		}

		if ((INSTANCE->u4PicWidth > 65535) || (INSTANCE->u4PicHeight > 65535))
		{
			return -(INT32)E_HWJPG_ERR_SIZE;
		}

		/*
		commit all data to hardware
		1. progressive flag
		2. huffman table (dc/ac look ahead, maxcode)
		3. membership
		*/

		// progressive
		if (INSTANCE->fgProgressive)
		{
			VERIFY(VLDJPG_Switch2Progressive() == (INT32)E_HWJPG_OK);
		}
		else
		{
			VERIFY(VLDJPG_Switch2Baseline() == (INT32)E_HWJPG_OK);
		}
		// done!
		INSTANCE->fgVldJpgDataReady = TRUE;

	M_JPEG_INTERFACE_END();
}

/* scan hw parameters */
INT32 VLDJPG_SetScanData			(JPEGHANDLE hInstance, UINT8 *pu1ScanDataStart)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->pu1DataStart			= pu1ScanDataStart;
		INSTANCE->fgScanSet				|= D_SCAN_DATASTART;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanCompInScan		(JPEGHANDLE hInstance, UINT8 u1CompInScan, const UINT8 *au1CompNoList)
{
	M_JPEG_INTERFACE_START();
		UINT8 u1Count;
	M_JPEG_HANDLE_VALIDATE();

	ASSERT(au1CompNoList != NULL);

		if (u1CompInScan <= INSTANCE->u1NumComps)
		{
			UINT8 u1FirstCompInScan = D_MAX_JPEG_HW_COMP;
			INSTANCE->u1NumCompsInCurrScan	= u1CompInScan;
			for (u1Count = 0; u1Count < (D_MAX_JPEG_HW_COMP + 1); u1Count++)
			{
				/* INSTANCE->afgCompInScan[D_MAX_JPEG_HW_COMP] == FALSE all the time */
				INSTANCE->afgCompInScan[u1Count] = FALSE;
			}
			for (u1Count = 0; u1Count < u1CompInScan; u1Count++)
			{
				INSTANCE->afgCompInScan[au1CompNoList[u1Count]] = TRUE;
				if (au1CompNoList[u1Count] < u1FirstCompInScan)
				{
					u1FirstCompInScan = au1CompNoList[u1Count];
				}
			}
			INSTANCE->u1FirstCompInScan		= u1FirstCompInScan;
			INSTANCE->fgScanSet				|= D_SCAN_NCOMP;
			INSTANCE->fgScanSet				|= D_SCAN_COMPIN;
			/*
			set member of component
			hardcoded due to only 3 components
			*/
			INSTANCE->u1NumBlocks =
				((INSTANCE->afgCompInScan[0]) ?
					(INSTANCE->au1CompBlkWidth[0] * INSTANCE->au1CompBlkHeight[0]) :
					0) +
				((INSTANCE->afgCompInScan[1]) ?
					(INSTANCE->au1CompBlkWidth[1] * INSTANCE->au1CompBlkHeight[1]) :
					0) +
				((INSTANCE->afgCompInScan[2]) ?
					(INSTANCE->au1CompBlkWidth[2] * INSTANCE->au1CompBlkHeight[2]) :
					0);
			
			LOG(6,"Comp in scan (%d, %d, %d)\n",
				INSTANCE->afgCompInScan[0],
				INSTANCE->afgCompInScan[1],
				INSTANCE->afgCompInScan[2]);
			LOG(6,"NUM BLOCKS = %d\n", (INT32)INSTANCE->u1NumBlocks);
			
		}
		else
		{
			i4Ret = -(INT32)E_HWJPG_ERR_COMP_RANGE;
		}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanCompQTbl		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 u1QTbl)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->au1CompQ[u1Comp]			= u1QTbl;
		INSTANCE->fgScanSet					|= D_SCAN_COMPQ;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanBlkDc			(JPEGHANDLE hInstance, UINT8 u1Block, UINT8 u1Dc)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();

		INSTANCE->au1BlkDc[u1Block]		= u1Dc;
		INSTANCE->fgScanSet				|= D_SCAN_BLKDC;
		/*
		if (u1Block < INSTANCE->u1NumBlocks)
		{
			INSTANCE->au1BlkDc[u1Block]		= u1Dc;
			INSTANCE->fgScanSet				|= D_SCAN_BLKDC;
		}
		else
		{
			i4Ret = -(INT32)E_HWJPG_ERR_BLOCK;
		}
		*/
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanBlkAc			(JPEGHANDLE hInstance, UINT8 u1Block, UINT8 u1Ac)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();

		INSTANCE->au1BlkAc[u1Block]		= u1Ac;
		INSTANCE->fgScanSet				|= D_SCAN_BLKAC;

		/*
		if (u1Block < INSTANCE->u1NumBlocks)
		{
			INSTANCE->au1BlkAc[u1Block]		= u1Ac;
			INSTANCE->fgScanSet				|= D_SCAN_BLKAC;
		}
		else
		{
			i4Ret = -(INT32)E_HWJPG_ERR_BLOCK;
		}
		*/
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanAlAh			(JPEGHANDLE hInstance, UINT8 u1AlAh)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u1AlAh					= u1AlAh;
		INSTANCE->fgScanSet					|= D_SCAN_ALAH;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanSeSs			(JPEGHANDLE hInstance, UINT8 u1Se, UINT8 u1Ss)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u1Se						= u1Se;
		INSTANCE->u1Ss						= u1Ss;
		INSTANCE->fgScanSet					|= D_SCAN_SESS;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanLast			(JPEGHANDLE hInstance, BOOL fgLastScan)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->fgLastScan				= fgLastScan;
		INSTANCE->fgScanSet					|= D_SCAN_LAST;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanMcuRowStart		(JPEGHANDLE hInstance, UINT32 u4McuRowStart)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u4McuRowStart				= u4McuRowStart;
		INSTANCE->fgScanSet					|= D_SCAN_ROWSTART;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetScanMcuRowEnd		(JPEGHANDLE hInstance, UINT32 u4McuRowEnd)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u4McuRowEnd				= u4McuRowEnd;
		INSTANCE->fgScanSet					|= D_SCAN_ROWEND;
	M_JPEG_INTERFACE_END();
}

#define D_QTBL_HW1	0
#define D_QTBL_HW2	1
INT32 VLDJPG_ValidateScanData		(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
		UINT32 fgFlagsCheck =
			(UINT32)(D_SCAN_DATASTART | D_SCAN_NCOMP | D_SCAN_COMPIN | D_SCAN_COMPQ |
			D_SCAN_BLKDC | D_SCAN_BLKAC | D_SCAN_ALAH | D_SCAN_SESS | D_SCAN_LAST);
		UINT8 u1Comp;
		UINT8 u1Block;
		UINT8 u1Count;
		UINT32 u4MembershipList;
		UINT8 u1QtblAlloc = 0;
		const UINT8 _au1BitCount[] =
		{
			0,1,				// 0 1
			1,2,				// 10 11
			1,2,2,3,			// 100 101 110 111
			1,2,2,3,2,3,3,4		// 1000 1001 1010 1011 1100 1101 1110 1111
		};
		//UINT8 u1QtblChange = 0;
		UINT32 u4DcNeedList = 0;
		UINT32 u4AcNeedList = 0;
		UINT32 u4DcHuffList, u4AcHuffList;

	M_JPEG_HANDLE_VALIDATE();

		if (INSTANCE->fgProgressive)
		{
			fgFlagsCheck |= D_SCAN_NZBUFF | D_SCAN_ROWSTART | D_SCAN_ROWEND;
		}

		if ((fgFlagsCheck & INSTANCE->fgScanSet) != fgFlagsCheck)
		{
			INSTANCE->fgScanReady = FALSE;
			return -(INT32)E_HWJPG_ERR_SCAN_NOT_READY;
		}

		if (INSTANCE->pu1DataStart == NULL)
		{
			return -(INT32)E_HWJPG_ERR_NULL_SCAN;
		}


		/* huffman table */
		for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
		{
			if (INSTANCE->fgAcHuff & D_BIT(u1Count))
			{
				VERIFY(VLDJPG_LoadAcLookAheadTable(
					u1Count, &INSTANCE->arAcHuffTable[u1Count]) == (INT32)E_HWJPG_OK);
			}
		}

		// dc maxcode tbl 0 1 2 3
		// dc valoffset tbl 0 1 2 3
		// ac maxcode tbl 0 1 2 3
		// ac valoffset tbl 0 1 2 3

		for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
		{
			VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arDcHuffTable[u1Count].ai4Maxcode) ==
				(INT32)E_HWJPG_OK);
		}
		for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
		{
			VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arDcHuffTable[u1Count].ai4Valoffset) ==
				(INT32)E_HWJPG_OK);
		}
		for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
		{
			VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arAcHuffTable[u1Count].ai4Maxcode) ==
				(INT32)E_HWJPG_OK);
		}
		for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
		{
			VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arAcHuffTable[u1Count].ai4Valoffset) ==
				(INT32)E_HWJPG_OK);
		}
#ifdef JPEG_VLD_V3
		VERIFY(VLDJPG_SetHuffTblVal(INSTANCE->pu1HwDcHuffTbl, TRUE) == (INT32)E_HWJPG_OK);
    	VERIFY(VLDJPG_SetHuffTblVal(INSTANCE->pu1HwAcHuffTbl, FALSE) == (INT32)E_HWJPG_OK);
#else
		VERIFY(VLDJOG_SetHuffAddr(INSTANCE->pu1HwDcHuffTbl, INSTANCE->pu1HwAcHuffTbl) ==
			(INT32)E_HWJPG_OK);
#endif
		// member ship
		// dc ac table number list
		u4MembershipList = 0;
		u4DcHuffList = 0;
		u4AcHuffList = 0;
		u1Block = 0;
		/*
			following block use :
				u1Count as absolute blocks in jpeg file
				u1Block as hw blocks in mt5351
		*/
		for (u1Count = 0; u1Count < D_MAX_JPEG_HW_BLOCK; u1Count++)
		{
			if (INSTANCE->afgCompInScan[INSTANCE->au1MemberComp[u1Count]])
			{
				u4MembershipList	|=
					(((UINT32)INSTANCE->au1MemberComp[u1Count] & 3) - (UINT32)INSTANCE->u1FirstCompInScan) <<
					(u1Block * 2);
				u4DcHuffList		|=
					((UINT32)INSTANCE->au1BlkDc[u1Count] & 3) <<
					(u1Block * 2);
				u4AcHuffList		|=
					((UINT32)INSTANCE->au1BlkAc[u1Count] & 3) <<
					(u1Block * 2);
				
				LOG(6, "abs block %d's member ship = %d (block %d)\n", u1Count, INSTANCE->au1MemberComp[u1Count], u1Block);
				LOG(6, "abs block %d's dc index    = %d (block %d)\n", u1Count, INSTANCE->au1BlkDc[u1Count], u1Block);
				LOG(6, "abs block %d's ac index    = %d (block %d)\n", u1Count, INSTANCE->au1BlkAc[u1Count], u1Block);
				
				u1Block++;
			}
			else
			{
				LOG(6, "INSTANCE->u1FirstCompInScan = %d\n", INSTANCE->u1FirstCompInScan);
				LOG(6, "abs block %d is not in this scan. comp %d : hwcomp %d.\n",
					u1Count,
					INSTANCE->au1MemberComp[u1Count],
					(INSTANCE->au1MemberComp[u1Count] & 3) - INSTANCE->u1FirstCompInScan);
			}
			
		}
		
		LOG(6,"Membership List = 0x%08x\n", u4MembershipList);
		LOG(6,"DC Huff List    = 0x%08x\n", u4DcHuffList);
		LOG(6,"AC Huff List    = 0x%08x\n", u4AcHuffList);
		
		VERIFY(VLDJPG_SetMemberShip(u4MembershipList) == (INT32)E_HWJPG_OK);
		VERIFY(VLDJPG_SetDcTableList(u4DcHuffList) == (INT32)E_HWJPG_OK);
		VERIFY(VLDJPG_SetAcTableList(u4AcHuffList) == (INT32)E_HWJPG_OK);



		/*
		commit hw registers
		1. q table - do smart load
		2. dc needed and ac needed and block count in scan
		*/

		// calculate how many q table should load
		u1QtblAlloc = 0;
		for (u1Comp = 0; u1Comp < D_MAX_JPEG_HW_COMP; u1Comp++)
		{
			if (INSTANCE->afgCompInScan[u1Comp])
			{
				u1QtblAlloc |= D_BIT(INSTANCE->au1CompQ[u1Comp]);
			}
		}
		// if table > D_MAX_JPEG_HW_QTBL ...
		// maybe some table should merge by application (fujifilm's case)
		// but in driver, just return error
		if (u1QtblAlloc > (UINT8)15)
		{
		    u1QtblAlloc = (UINT8)15;
		}
		if (_au1BitCount[u1QtblAlloc] > D_MAX_JPEG_HW_QTBL)
		{
			//return -(INT32)E_HWJPG_ERR_QTBL_NUM;
			u1QtblAlloc = 3;
		}

		if (_au1BitCount[u1QtblAlloc] == 2)
		{
			//u1QtblChange = 3;
		}
		else if (_au1BitCount[u1QtblAlloc] == 1)
		{
			//u1QtblChange = 1;
			VLDJPG_SetUserDefQMatrix();
		}
		else
		{
			//u1QtblChange = 0;
			VLDJPG_SetUserDefQMatrix();
		}

		// load q table(s) u1QtblAlloc indicates
		for (u1Count = 0; u1Count < D_MAX_JPEG_QTBL; u1Count++)
		{
			if (u1QtblAlloc & D_BIT(u1Count))
			{
				// this table has to be changed
				if (u1QtblAlloc/*u1QtblChange*/ & D_BIT(D_QTBL_HW1))	// hardcoded due to only 2 hw entry
				{
					// load u1Count q table into hw entry 0
					u1QtblAlloc &= (~(UINT8)D_BIT(u1Count));		// bit remove
					VERIFY(VLDJPG_LoadTblQ(D_QTBL_HW1, INSTANCE->apu1Qtbl[u1Count]) == (INT32)E_HWJPG_OK);
					INSTANCE->afgQtblLoad[D_QTBL_HW1] = TRUE;
					INSTANCE->au1QtblLoadIndex[D_QTBL_HW1] = u1Count;
					//u1QtblChange &= (~(UINT8)D_BIT(D_QTBL_HW1));
				}
				else if (u1QtblAlloc/*u1QtblChange*/ & (UINT8)D_BIT(D_QTBL_HW2))	// second entry
				{
					// load u1Count q table into hw entry 0
					u1QtblAlloc &= (~(UINT8)D_BIT(u1Count));		// bit remove
					VERIFY(VLDJPG_LoadTblQ(D_QTBL_HW2, INSTANCE->apu1Qtbl[u1Count]) == (INT32)E_HWJPG_OK);
					INSTANCE->afgQtblLoad[D_QTBL_HW2] = TRUE;
					INSTANCE->au1QtblLoadIndex[D_QTBL_HW2] = u1Count;
					//u1QtblChange &= (~(UINT8)D_BIT(D_QTBL_HW2));
				}
				else
				{
                                    if(u1QtblAlloc & D_BIT(2))
                                    {
                                        u1QtblAlloc &= (~(UINT8)D_BIT(u1Count));        // bit remove
                                        VERIFY(VLDJPG_LoadTblQ(D_QTBL_HW1, INSTANCE->apu1Qtbl[u1Count]) == (INT32)E_HWJPG_OK);
                                        INSTANCE->afgQtblLoad[D_QTBL_HW1] = TRUE;
                                        INSTANCE->au1QtblLoadIndex[D_QTBL_HW1] = u1Count;
                                    }
                                    else if(u1QtblAlloc & D_BIT(3))
                                    {
                                        u1QtblAlloc &= (~(UINT8)D_BIT(u1Count));        // bit remove
                                        VERIFY(VLDJPG_LoadTblQ(D_QTBL_HW2, INSTANCE->apu1Qtbl[u1Count]) == (INT32)E_HWJPG_OK);
                                        INSTANCE->afgQtblLoad[D_QTBL_HW2] = TRUE;
                                        INSTANCE->au1QtblLoadIndex[D_QTBL_HW2] = u1Count;
                                    }
                                    else
                                    {
                                        // impossible, or else above lines are wrong
                                        //ASSERT(FALSE);
                                    }

				}
			}
		}

		u1QtblAlloc = 0;
		u1Count = 0;
		for (u1Comp = 0; u1Comp < D_MAX_JPEG_HW_COMP; u1Comp++)
		{
			if (INSTANCE->afgCompInScan[u1Comp])
			{
				u1QtblAlloc |= ((INSTANCE->au1CompQ[u1Comp]) ? 1 : 0) << (u1Count * 2);
				u1Count++;
			}
		}

		VERIFY(VLDJPG_SetQuanTableList((UINT32)u1QtblAlloc) == (INT32)E_HWJPG_OK);

		/* pitch per row */
		if (INSTANCE->u1NumComps > (UINT8)3)
		{
		    INSTANCE->u1NumComps = (UINT8)3;
		}
		for (u1Comp = 0; u1Comp < INSTANCE->u1NumComps; u1Comp++)
		{
			INSTANCE->u4CompPitchRow[u1Comp] = 0;
		}
		u1Count = 0;
		for (u1Comp = 0; u1Comp < INSTANCE->u1NumComps; u1Comp++)
		{
			if (INSTANCE->afgCompInScan[u1Comp])
			{
				INSTANCE->u4CompPitchRow[u1Count] =
					INSTANCE->u4McuPerRow *
					(UINT32)INSTANCE->au1CompBlkWidth[u1Comp] *
					D_DCTSIZE;
				if (INSTANCE->u4CompPitchRow[u1Count] & D_JPEG_DRAM_ALIGN_MASK)
				{
					// expand pitch to fit into dram aligned line
					INSTANCE->u4CompPitchRow[u1Count] &= (~D_JPEG_DRAM_ALIGN_MASK);
					INSTANCE->u4CompPitchRow[u1Count] += D_JPEG_DRAM_ALIGN_SIZE;
				}
				u1Count++;
			}
		}

		VERIFY(VLDJPG_SetCompIdctPitch(INSTANCE->u4CompPitchRow) == (INT32)E_HWJPG_OK);
		if (INSTANCE->fgProgressive)
		{
			INT32 i32Idx;
			UINT32 au4CompPitchRow[D_MAX_JPEG_HW_COMP];
			for (i32Idx = 0; i32Idx < D_MAX_JPEG_HW_COMP; i32Idx++)
			{
				au4CompPitchRow[i32Idx] = INSTANCE->u4CompPitchRow[i32Idx] * 2;
			}
			VERIFY(VLDJPG_SetCompCoefPitch(au4CompPitchRow) == (INT32)E_HWJPG_OK);
		}
		else
		{
			VERIFY(VLDJPG_SetCompCoefPitch(INSTANCE->u4CompPitchRow) == (INT32)E_HWJPG_OK);
		}

		// calculate dc/ac needed
		// set them to all false first,
		// them set the block back if the component it belong is in this scan
		for (u1Block = 0; u1Block < D_MAX_JPEG_HW_BLOCK; u1Block++)
		{
			INSTANCE->au1DcNeed[u1Block] = 0;
			INSTANCE->au1AcNeed[u1Block] = 0;
		}
		u1Block = 0;
		for (u1Comp = 0; u1Comp < INSTANCE->u1NumComps; u1Comp++)
		{
			if (INSTANCE->afgCompInScan[u1Comp])
			{
				for (u1Count = 0; u1Count < (INSTANCE->au1CompBlkWidth[u1Comp] *
					INSTANCE->au1CompBlkHeight[u1Comp]); u1Count++)
				{
				    if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
				    {
				        u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
				    }
					INSTANCE->au1DcNeed[u1Block] = 1;
					INSTANCE->au1AcNeed[u1Block] = 1;
					u1Block++;
				}
			}
			else
			{
				// skip this component
				/*
				u1Block += INSTANCE->au1CompBlkWidth[u1Comp] * INSTANCE->au1CompBlkWidth[u1Comp];
				*/
			}
		}
		// calculate needed list
		for (u1Block = 0; u1Block < D_MAX_JPEG_HW_BLOCK; u1Block++)
		{
			if (INSTANCE->au1DcNeed[u1Block])
			{
				u4DcNeedList |= D_BIT(u1Block);
			}
			if (INSTANCE->au1AcNeed[u1Block])
			{
				u4AcNeedList |= D_BIT(u1Block);
			}
		}
		// commit to hw
		VERIFY(VLDJPG_SetAcNeededList(u4AcNeedList) == (INT32)E_HWJPG_OK);
		VERIFY(VLDJPG_SetDcNeededList(u4DcNeedList) == (INT32)E_HWJPG_OK);
		VERIFY(VLDJPG_SetBlockCount(INSTANCE->u1NumBlocks) == (INT32)E_HWJPG_OK);	// this should be place in glob section

		// set al ah
		// set se ss
		// set last scan
		VERIFY(VLDJPG_SetProgressiveAhAl(INSTANCE->u1AlAh) == (INT32)E_HWJPG_OK);
		VERIFY(VLDJPG_SetProgressiveSeSs(INSTANCE->u1Se, INSTANCE->u1Ss) == (INT32)E_HWJPG_OK);
		if (INSTANCE->fgProgressive)
		{
			VERIFY(VLDJPG_SetProgressiveLastScan(INSTANCE->fgLastScan) == (INT32)E_HWJPG_OK);
		}
		else
		{
			VERIFY(VLDJPG_SetProgressiveLastScan(TRUE) == (INT32)E_HWJPG_OK);
		}

		// set none zero history buffer
		VERIFY(VLDJPG_SetProgressiveNoneZeroHistory(INSTANCE->pu1NonZeroBuffer) == (INT32)E_HWJPG_OK);

		if (INSTANCE->fgRingBuffer)
		{
			//VERIFY(VLDJPG_SetRingBuffer(INSTANCE->pu1DataStart, INSTANCE->u4RingBufferSize) == (INT32)E_HWJPG_OK);
			VERIFY(VLDJPG_SetRingBuffer(INSTANCE->pu1RingBufStart, INSTANCE->u4RingBufferSize) == (INT32)E_HWJPG_OK);
		}
		else
		{
			VLDJPG_SetLinearBuffer();
		}

		VERIFY(VLDJPG_SetBarrelShifter(INSTANCE->pu1DataStart) == (INT32)E_HWJPG_OK);
		VERIFY(VLDJPG_InitScan() == (INT32)E_HWJPG_OK);

		// done!
		INSTANCE->fgScanReady	= TRUE;

	M_JPEG_INTERFACE_END();
}

/* mcu row hw parameters */
INT32 VLDJPG_SetMcuRowCurr			(JPEGHANDLE hInstance, UINT32 u4CurrMcuRow)
{
	M_JPEG_INTERFACE_START();

	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->u4CurrMcuRow				= u4CurrMcuRow;
		INSTANCE->fgRowSet					|= D_ROW_CURR;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetMcuRowOutBuf		(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 *pu1OutBuffer, UINT32 u4OutBufSize)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->apu1OutputBuffer[u1Comp]		= pu1OutBuffer;
		INSTANCE->au4OutputBufferSize[u1Comp]	= u4OutBufSize;
		INSTANCE->fgRowSet						|= D_ROW_OUTBUF | D_ROW_OUTBUFSIZE;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetMcuRowOutBufBank15391(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 *pu1OutBuffer, UINT32 u4OutBufSize)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->apu1OutputBuffer1[u1Comp]		= pu1OutBuffer;
		INSTANCE->au4OutputBufferSize[u1Comp]	= u4OutBufSize;
		INSTANCE->fgRowSet						|= D_ROW_OUTBUF | D_ROW_OUTBUFSIZE;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ValidateMcuRowData		(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
		//UINT32 fgCheckFlags = D_ROW_OUTBUF;
		UINT32 fgCheckFlags = D_FLAG_NONE;
	M_JPEG_HANDLE_VALIDATE();
		if (INSTANCE->fgProgressive)
		{
			fgCheckFlags |= D_ROW_CURR;
		}

		if ((INSTANCE->fgRowSet & fgCheckFlags) != fgCheckFlags)
		{
			INSTANCE->fgMcuRowReady = FALSE;
			return -(INT32)E_HWJPG_ERR_ROW_NOT_READY;
		}

		/*
		till now, we have at least 1 output buffer address,
		and current mcu row (if progressive mode)
		there's no need to set anything in baseline mode,
		but in progressive, output should turn on or off deps on
		current row
		*/

		if (INSTANCE->fgProgressive)
		{
			if ((INSTANCE->u4CurrMcuRow >= INSTANCE->u4McuRowStart) &&
				(INSTANCE->u4CurrMcuRow <= INSTANCE->u4McuRowEnd))
			{
				// out of dram range, write only nz buffer
				VERIFY(VLDJPG_SetProgressiveNzOnly(FALSE) == (INT32)E_HWJPG_OK);
			}
			else
			{
				// in range, output coef | idct result
				VERIFY(VLDJPG_SetProgressiveNzOnly(TRUE) == (INT32)E_HWJPG_OK);
			}
		}

		INSTANCE->u4CurrMcuInRow = 0;		// when a mcu data has been commited to hw,
											// this counter advance one
		INSTANCE->u4RowMcuCount = 0;		// reset mcu count in row
		INSTANCE->fgMcuRowReady = TRUE;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ValidateMcuRowData5391(JPEGHANDLE hInstance, 
    UINT32 u4MCUsPerRow, UINT32 u4TotalRows)
{
	M_JPEG_INTERFACE_START();

		UINT32 fgCheckFlags = (UINT32)D_ROW_OUTBUF;
	M_JPEG_HANDLE_VALIDATE();
		if (INSTANCE->fgProgressive)
		{
			fgCheckFlags |= D_ROW_CURR;
		}

		if ((fgCheckFlags & INSTANCE->fgRowSet) != fgCheckFlags)
		{
			INSTANCE->fgMcuRowReady = FALSE;
			return -(INT32)E_HWJPG_ERR_ROW_NOT_READY;
		}

		/*
		1. if this is first mcu in an scan, set first-time flag else clear it
		2. set read write pointer (dram word align - 32 bit base)
		*/

        VLDJPG_SetCompReadAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);
        VLDJPG_SetCompWriteAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);

        // output bank 0
        VLDJPG_SetOutputBank0BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);
        // output bank 1
        VLDJPG_SetOutputBank1BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer1[0],
                (UINT32)INSTANCE->apu1OutputBuffer1[1], (UINT32)INSTANCE->apu1OutputBuffer1[2]);
                
        VLDJPG_SetMCUWidth5391(
                (UINT32)INSTANCE->au1CompBlkWidth[0] * D_DCTSIZE, 
                (UINT32)INSTANCE->au1CompBlkWidth[1] * D_DCTSIZE, 
                (UINT32)INSTANCE->au1CompBlkWidth[2] * D_DCTSIZE);
        VLDJPG_SetPicSize5391(u4MCUsPerRow, u4TotalRows);

        // Restart Interval Processing.
        if (INSTANCE->u4RestartInterval > 0)
        {
            VLDJPG_SetRestartInterval5391(INSTANCE->u4RestartInterval);
        }
                
		if ((INSTANCE->u4RestartInterval != 0) &&
			(INSTANCE->u4CurrRestartInterval == 0))
		{
			VERIFY(VLDJPG_Restart() == (INT32)E_HWJPG_OK);
			VERIFY(VLDJPG_AdvanceRestartMarker(NULL) == (INT32)E_HWJPG_OK);
			INSTANCE->u4CurrRestartInterval = INSTANCE->u4RestartInterval;
		}

//        VERIFY(VLDJPG_SetCurrMcuRowNum5391(INSTANCE->u4RowMcuCount) == (INT32)E_HWJPG_OK);
    
		// done !
		INSTANCE->fgMcuRowReady = TRUE;
	M_JPEG_INTERFACE_END();
}


/* mcu hw parameters */
INT32 VLDJPG_SetMcuOutBuf			(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 *pu1CompOutAddr)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->apu1OutputAddr[u1Comp]	= pu1CompOutAddr;
		INSTANCE->fgMcuSet					|= D_MCU_OUTADDR;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetMcuCoefBuf			(JPEGHANDLE hInstance, UINT8 u1Comp, UINT8 *pu1CompCoefAddr)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->apu1CoefAddr[u1Comp]		= pu1CompCoefAddr;
		INSTANCE->fgMcuSet					|= D_MCU_COEFADDR;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetMcuDefBuf			(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
		UINT8 u1Comp;
	M_JPEG_HANDLE_VALIDATE();
		// we setup an rester scan buffer here
		// there's an chance to output directly to semi-mb mode,
		// but that needs more calculation
		for (u1Comp = 0; u1Comp < D_MAX_JPEG_HW_COMP; u1Comp++)
		{
			// do this only when this component is in this buffer
			if (INSTANCE->afgCompInScan[u1Comp])
			{
				if ((INSTANCE->apu1OutputBuffer[u1Comp] == NULL) ||
					(INSTANCE->au4OutputBufferSize[u1Comp] == 0))
				{
					return -(INT32)E_HWJPG_ERR_OUTBUF;
				}
				if (INSTANCE->au4OutputBufferSize[u1Comp] <
					(UINT32)(
						INSTANCE->au1CompBlkWidth[u1Comp] *
						INSTANCE->au1CompBlkHeight[u1Comp] *
						D_DCTSIZE * D_DCTSIZE
					))
				{
					return -(INT32)E_HWJPG_ERR_OUTBUF;
				}
				// set output buffer
				i4Ret = VLDJPG_SetMcuOutBuf(hInstance,
					u1Comp,
					INSTANCE->apu1OutputBuffer[u1Comp] +
					(D_DCTSIZE * INSTANCE->au1CompBlkWidth[u1Comp] * INSTANCE->u4RowMcuCount));
				if (i4Ret)
				{
					return i4Ret;
				}
				// set coefficient buffer
				i4Ret = VLDJPG_SetMcuCoefBuf(hInstance,
					u1Comp,
					INSTANCE->apu1OutputBuffer[u1Comp] +
					(D_DCTSIZE * INSTANCE->au1CompBlkWidth[u1Comp] * INSTANCE->u4RowMcuCount));
				if (i4Ret)
				{
					return i4Ret;
				}
			}
		}
		INSTANCE->fgMcuSet					|= D_MCU_OUTADDR | D_MCU_COEFADDR;
	M_JPEG_INTERFACE_END();
}

//static UINT

INT32 VLDJPG_ValidateMcuData		(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
		UINT8 u1Comp;
		UINT32 fgCheckFlags = (UINT32)D_MCU_OUTADDR;
	M_JPEG_HANDLE_VALIDATE();
		if (INSTANCE->fgProgressive)
		{
			fgCheckFlags |= D_MCU_COEFADDR;
		}

		if ((fgCheckFlags & INSTANCE->fgMcuSet) != fgCheckFlags)
		{
			INSTANCE->fgMcuDataReady = FALSE;
			return -(INT32)E_HWJPG_ERR_MCU_NOT_READY;
		}

		/*
		1. if this is first mcu in an scan, set first-time flag else clear it
		2. set read write pointer (dram word align - 32 bit base)
		*/

		for (u1Comp = 0; u1Comp < D_MAX_JPEG_HW_COMP; u1Comp++)
		{
			VERIFY(VLDJPG_SetCompReadAddr((UINT32)u1Comp,
				INSTANCE->apu1CoefAddr[u1Comp]) == (INT32)E_HWJPG_OK);
			VERIFY(VLDJPG_SetCompWriteAddr((UINT32)u1Comp,
				INSTANCE->apu1OutputAddr[u1Comp]) == (INT32)E_HWJPG_OK);
		}

		if ((INSTANCE->u4RestartInterval != 0) &&
			(INSTANCE->u4CurrRestartInterval == 0))
		{
			VERIFY(VLDJPG_Restart() == (INT32)E_HWJPG_OK);
			VERIFY(VLDJPG_AdvanceRestartMarker(NULL) == (INT32)E_HWJPG_OK);
			INSTANCE->u4CurrRestartInterval = INSTANCE->u4RestartInterval;
		}

		VERIFY(VLDJPG_SetCurrMCUNum(INSTANCE->u4McuCount) == (INT32)E_HWJPG_OK);
		// done !
		INSTANCE->fgMcuDataReady = TRUE;
	M_JPEG_INTERFACE_END();
}

/* misc hw parameters (must set) */
INT32 VLDJPG_SetMiscBlkAddrX		(JPEGHANDLE hInstance, UINT8 u1Block, UINT32 u4AddrX)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
	if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
	{
	    u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
	}
		INSTANCE->au4BlockAddrX[u1Block]	= u4AddrX;
		INSTANCE->fgMiscSet					|= D_MISC_ADDR_X;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetMiscBlkAddrY		(JPEGHANDLE hInstance, UINT8 u1Block, UINT32 u4AddrY)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
	if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
	{
	    u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
	}
		INSTANCE->au4BlockAddrY[u1Block]	= u4AddrY;
		INSTANCE->fgMiscSet					|= D_MISC_ADDR_Y;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_SetMiscDefBlkAddr		(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
		UINT8 u1AddrX, u1AddrY, u1Block;
		UINT8 u1Comp;
	M_JPEG_HANDLE_VALIDATE();
		for ((u1Comp = 0), (u1Block = 0); u1Comp < D_MAX_JPEG_HW_COMP; u1Comp++) {
		for (u1AddrY = 0; u1AddrY < INSTANCE->au1CompBlkHeight[u1Comp]; u1AddrY++) {
		for (u1AddrX = 0; u1AddrX < INSTANCE->au1CompBlkWidth[u1Comp]; u1AddrX++)
		{
			VERIFY(VLDJPG_SetMiscBlkAddrX(hInstance, u1Block, (UINT32)u1AddrX)
				== (INT32)E_HWJPG_OK);
			VERIFY(VLDJPG_SetMiscBlkAddrY(hInstance, u1Block, (UINT32)u1AddrY)
				== (INT32)E_HWJPG_OK);
			u1Block++;
		}}}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ValidateMiscData		(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
		UINT32 fgCheckFlags = (UINT32)(D_MISC_ADDR_X | D_MISC_ADDR_Y);
	M_JPEG_HANDLE_VALIDATE();
		if ((fgCheckFlags & INSTANCE->fgMiscSet) != fgCheckFlags)
		{
			INSTANCE->fgMiscDataReady = FALSE;
			return -(INT32)E_HWJPG_ERR_MISC_NOT_READY;
		}

		VERIFY(VLDJPG_SetBlockAddrList(INSTANCE->au4BlockAddrX,
			INSTANCE->au4BlockAddrY) == (INT32)E_HWJPG_OK);

		// done !
		INSTANCE->fgMiscDataReady = TRUE;
	M_JPEG_INTERFACE_END();
}

/* clear interface */
INT32 VLDJPG_ClearGlobFlag			(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->fgGlobSet			= 0;
		INSTANCE->fgVldJpgDataReady	= FALSE;
		INSTANCE->u4McuCount		= 0;
		INSTANCE->u4RestartInterval	= 0;
		INSTANCE->u4CurrRestartInterval		= 0;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ClearScanFlag			(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->fgScanSet			= 0;
		INSTANCE->fgScanReady		= FALSE;
		INSTANCE->u4ScanMcuCount	= 0;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ClearRowFlag			(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->fgRowSet			= 0;
		INSTANCE->fgMcuRowReady		= FALSE;
		INSTANCE->u4RowMcuCount		= 0;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ClearMcuFlag			(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->fgMcuSet			= 0;
		INSTANCE->fgMcuDataReady	= FALSE;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_ClearMiscFlag			(JPEGHANDLE hInstance)
{
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();
		INSTANCE->fgMiscSet			= 0;
		INSTANCE->fgMiscDataReady	= FALSE;
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_DecodeOneMcu(JPEGHANDLE hInstance)
{
	//UINT16 u2Loop = 0;
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();

    // check video bitstream input window
	VLDJPG_CheckVideoInputWindow();

    // check video read pointer
	VLDJPG_CheckVideoReadPointer();
	
	i4Ret = VLDJPG_DecodeMcu();
	if (i4Ret != (INT32)E_HWJPG_OK)
	{
		return i4Ret;
	}
	/*
	do
	{
	} while (VLDJPG_CheckDecodeFinish() != E_HWJPG_OK);
	*/
	INSTANCE->u4ScanMcuCount++;
	INSTANCE->u4McuCount++;
	INSTANCE->u4RowMcuCount++;
	if (INSTANCE->u4CurrRestartInterval)
	{
		INSTANCE->u4CurrRestartInterval--;
	}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_DecodeOneMcuRow5391(JPEGHANDLE hInstance, UINT32 u4CurrMcuRow)
{
	//UINT16 u2Loop = 0;
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();

    // check video bitstream input window
	VLDJPG_CheckVideoInputWindow();

    // check video read pointer
	VLDJPG_CheckVideoReadPointer();

    if (u4CurrMcuRow == 0)
    {
        i4Ret = VLDJPG_DecodeFirstMcuRow5391();
    }
    else
    {
        i4Ret = VLDJPG_DecodeNextMcuRow5391();
    }

	if (i4Ret != (INT32)E_HWJPG_OK)
	{
		return i4Ret;
	}

	INSTANCE->u4ScanMcuCount++;
//	INSTANCE->u4McuCount++;
	INSTANCE->u4RowMcuCount++;
	if (INSTANCE->u4CurrRestartInterval)
	{
		INSTANCE->u4CurrRestartInterval--;
	}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_DecodeOnePic5391(JPEGHANDLE hInstance, UINT32 u4CurrMcuRow)
{
	//UINT16 u2Loop = 0;
	M_JPEG_INTERFACE_START();
	M_JPEG_HANDLE_VALIDATE();

    // check video bitstream input window
	VLDJPG_CheckVideoInputWindow();

    // check video read pointer
	VLDJPG_CheckVideoReadPointer();

    if (u4CurrMcuRow == 0)
    {
        i4Ret = VLDJPG_DecodePic5391();
    }

	if (i4Ret != (INT32)E_HWJPG_OK)
	{
		return i4Ret;
	}

	INSTANCE->u4ScanMcuCount++;
//	INSTANCE->u4McuCount++;
	INSTANCE->u4RowMcuCount++;
	if (INSTANCE->u4CurrRestartInterval)
	{
		INSTANCE->u4CurrRestartInterval--;
	}
	M_JPEG_INTERFACE_END();
}

INT32 VLDJPG_DecodeOnePic(JPEGHANDLE hInstance)
{
    VLDJPG_DecodePic();
    UNUSED(hInstance);

    return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_CheckDecodePicFinish(JPEGHANDLE hInstance)
{
    UNUSED(hInstance);
    return VLDJPG_CheckDecodeFinish();
}

INT32 VLDJPG_CheckDecodingError(JPEGHANDLE hInstance)
{
    UNUSED(hInstance);
    return VLDJPG_DetectError();
}

INT32 VLDJPG_SetStop(JPEGHANDLE hInstance)
{
    UNUSED(hInstance);
    return VLDJPG_StopHw();
}

INT32 VLDJPG_CheckStop(JPEGHANDLE hInstance)
{
    UNUSED(hInstance);

    if (VLDJPG_CheckHwStop())
    {
        return -(INT32)E_HWJPG_ERR_TIMEOUT;
    }

    return (INT32)E_HWJPG_OK;
}

