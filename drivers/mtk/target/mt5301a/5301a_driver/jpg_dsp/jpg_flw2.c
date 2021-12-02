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
#include "x_debug.h"
#include "jpg_common.h"
#include "jpg_if.h"
#include "jpg_jfif.h"
#include "jpg_flw2.h"
#include "gfx_if.h"

UINT16 bDitherMatrix[4][4] = 
{
{0, 8, 2, 10},
{12, 4, 14, 6},
{3, 11, 1, 9},
{15, 7, 13, 5}
};
static IMG_LIB_INFO_T *prJpgImg;
static ENUM_IMG_ROTATE_PHASE eJpgRotateDegree;
static BOOL _fgJpgFlw2Stop = FALSE;

static UCHAR _acRange[900] = {0};
static INT32 _ai4RR[1024];
static INT32 _ai4BB[1024];
static INT32 _ai4RG[1024];
static INT32 _ai4BG[1024];
static UINT16 _au2GammaR[256], _au2GammaG[256], _au2GammaB[256];
extern void vDrvSetSWGammaFlg(UINT8 bFlagOnOff);
extern void vDrvGetOSDGammaCurve(UINT16 * bOSDGammaRCurve, UINT16 * bOSDGammaGCurve, UINT16 * bOSDGammaBCurve);
extern UINT8 bDrvGetSWGammaFlg(void);

void _JPG_SetSwGamma(BOOL fgEnable)
{
    vDrvSetSWGammaFlg((UINT8)fgEnable);
}

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
    vDrvGetOSDGammaCurve(_au2GammaR, _au2GammaG, _au2GammaB);
}

VOID JDEC_MwSaveRzInfo(IMG_LIB_INFO_T *prImg, ENUM_IMG_ROTATE_PHASE eRotateDegree)
{
    prJpgImg = prImg;
    eJpgRotateDegree = eRotateDegree;
	if(prJpgImg && eJpgRotateDegree)
	{

	}
}

VOID Jpg_UpdateReadPtr(JPEGHANDLE hFlow,UINT32 u4ReadPtr)
{
    UINT32 u4NewRP;
    UINT32 u4UsedLen;
    u4NewRP = u4ReadPtr-PHYSICAL((UINT32) JPGHDR->pu1RingBufStart);
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
	prBase->eJpegClass = E_JC_DSP_FLOW;

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
	if ((prBase->eJpegClass != E_JC_DSP_FLOW) ||
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
	if ((prBase->eJpegClass != E_JC_DSP_FLOW) ||
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

	x_mem_free((void*)hDec);
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
	if ((prBase->eJpegClass		!= E_JC_DSP_FLOW) ||
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
	if ((prBase->eJpegClass		!= E_JC_DSP_FLOW) ||
			(prBase->zObjectSize	!= sizeof(JPEG_FLOW_DATA_T)))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}
	prFlow = (JPEG_FLOW_DATA_T*)hDec;
	*phJfif	= prFlow->hJpegHdr;

	return (INT32)E_JPG_OK;
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
    x_memset((VOID *)prJfif->pu1RingBufStart, 0x0, prJfif->u4RingBufSize);
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
    u4VldWptrOffset = prJfif->u4RingRP + prJfif->u4RingData;
    u4VldWptrOffset = (u4VldWptrOffset >= prJfif->u4RingBufSize) ? (u4VldWptrOffset - prJfif->u4RingBufSize) : u4VldWptrOffset;

//#ifdef CC_JPEG_8223MAYBE
//    VLDJPG_SetRiscCtrlWP(PHYSICAL((UINT32)prJfif->pu1RingBufStart) + u4VldWptrOffset);
//#endif
    
	x_crit_end(crit);

    if(prJfif->u4RingData > prJfif->u4RingBufSize)
    {
       LOG(7, "JDEC_FillBsBuf_End error occur\n");
    }

    HalFlushInvalidateDCache();
}


void JDEC_SetFileSize(JPEGHANDLE hFlow,UINT32 u4FileSize)
{
	JPEG_FLOW_DATA_T *prFlow;
	prFlow = (JPEG_FLOW_DATA_T*)hFlow;
    prFlow->u4FileSize=u4FileSize;
	return ;
}

UINT32 JDEC_GetProgress(JPEGHANDLE hFlow,JPEG_HEADER_INFO_T *jpgHdrInfo)
{
    UINT32 u4Progress;
    
    if (JPGHDR->rSOF.eJpegFormat == E_JPG_PROGRESSIVE_HUFFMAN)
    {
        u4Progress = (JPGHDR->u4CurrScan* 100) / 10;
    }
    else
    {
	   u4Progress = (jpgHdrInfo->u4TotalRow) ? (((UINT32)JPGFLW->i4CurRow) * 100)/jpgHdrInfo->u4TotalRow : 0;
    }
    return u4Progress;
}


INT32 JDEC_MwHeaderParse(JPEGHANDLE hFlow, UINT8 *pBs_start, UINT32 bufferSize, JPEG_HEADER_INFO_T *jpgHdrInfo)
{    
   INT32 i4Ret = (INT32)E_JPG_OK, i4Idx;
   JPEG_FLOW_DATA_T *prFlow;
   JPEG_JFIF_HEADER_T *prJfif;
   UINT32 u4CompH,u4CompH1,u4CompW,u4CompW1 = 0;
   UINT32 u4MaxHeight = 0,u4MaxWidth=0;
   UINT8 u1Comp;

   ASSERT(hFlow);
   ASSERT(jpgHdrInfo);
   prFlow = (JPEG_FLOW_DATA_T*)hFlow;
   prJfif = prFlow->hJpegHdr;

   //check buffer fillness, buffersize will depend on buffer fullness
   if(pBs_start)
   {
      i4Ret=JFIF_New(prFlow->hJpegHdr, pBs_start, bufferSize);  // parsing jpeg header
   }
   else
   {
      i4Ret=JFIF_New(prFlow->hJpegHdr, prJfif->pu1RingBufStart, prJfif->u4RingBufSize);  // parsing jpeg header
   }

   if(i4Ret)
   {
      return i4Ret;
   }

   u4CompW=JPGHDR->rSOF.arSofComp[0].u1HSampFactor*8;
   u4CompW1=JPGHDR->rSOF.arSofComp[1].u1HSampFactor*8;
   u4CompH=JPGHDR->rSOF.arSofComp[0].u1VSampFactor*8;
   u4CompH1=JPGHDR->rSOF.arSofComp[1].u1VSampFactor*8;
   for (u1Comp = 0; u1Comp < JPGHDR->rSOF.u1NumComponents; u1Comp++)
   {
	   if (u1Comp >= (UINT8)3)
	   {
		   return -(INT32)E_JPG_ERR_UNSUPPORT;
	   }
	   u4MaxWidth = MAX(JPGHDR->rSOF.arSofComp[u1Comp].u1HSampFactor, (UINT32)u4MaxWidth);
	   u4MaxHeight = MAX(JPGHDR->rSOF.arSofComp[u1Comp].u1VSampFactor, (UINT32)u4MaxHeight);

   }
   
   if(u4CompW ==(u4CompW1<<1))
   {
	   if(u4CompH ==(u4CompH1<<1))
	   {
	       jpgHdrInfo->u4ColorFMT=(UINT32)E_JPEG_YUV420;
	   }
	   else if(u4CompH ==u4CompH1)
	   {
		   jpgHdrInfo->u4ColorFMT=(UINT32)E_JPEG_YUV422;
	   }
	   else
	   {
		   jpgHdrInfo->u4ColorFMT=(UINT32)E_JPEG_COLOR_FMT_UNKNOWN;
	   }
   }
   else if(u4CompW==u4CompW1)
   {
	   if(u4CompH ==(u4CompH1<<1))
	   {
		  jpgHdrInfo->u4ColorFMT = (UINT32)E_JPEG_YUV422V;
	   }
	   else if(u4CompH ==u4CompH1)
	   {
		  jpgHdrInfo->u4ColorFMT = (UINT32)E_JPEG_YUV444;
	   }
	   else
	   {
		  jpgHdrInfo->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;
	   }
   }
   else
   {
	   jpgHdrInfo->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;		  
   }

     /* calculate mcu width-height and mcu size */

	jpgHdrInfo->u4Width   = (UINT32)JPGHDR->rSOF.u2ImageWidth;
	jpgHdrInfo->u4Height= (UINT32)JPGHDR->rSOF.u2ImageHeight;

    jpgHdrInfo->u4TotalCol = (u4MaxWidth == 2) ? ((jpgHdrInfo->u4Width + 15) >> 4) : ((jpgHdrInfo->u4Width +	7) >> 3);
	jpgHdrInfo->u4TotalRow = (u4MaxHeight == 2) ? ((jpgHdrInfo->u4Height + 15) >> 4) : ((jpgHdrInfo->u4Height +  7) >> 3);
	
	jpgHdrInfo->u4NumScan = JPGHDR->u4NumScan;

      //display row information
	for(i4Idx = 0; i4Idx<3; i4Idx++)
	{

	   jpgHdrInfo->au4RowWidth[i4Idx]= jpgHdrInfo->u4TotalCol*(JPGHDR->rSOF.arSofComp[i4Idx].u1HSampFactor<<3);
	   jpgHdrInfo->au4RowHeight[i4Idx]= JPGHDR->rSOF.arSofComp[i4Idx].u1VSampFactor<<3;	        

       if(jpgHdrInfo->au4RowWidth[i4Idx] & 0xf)
       {
            // expand pitch to fit into dram aligned line
            jpgHdrInfo->au4RowWidth[i4Idx] &= (~0xf);
            jpgHdrInfo->au4RowWidth[i4Idx] += 0x10;
       }
		
       jpgHdrInfo->au4RowSize[i4Idx]= jpgHdrInfo->au4RowWidth[i4Idx]*jpgHdrInfo->au4RowHeight[i4Idx];
	}

	return i4Ret;

}

VOID JDEC_SetStop(BOOL fgJdecStop)
{
    _fgJpgFlw2Stop = fgJdecStop;
}

void _jpg_RGB2Gamma(UINT32 u4Addr, UINT32 u4Width, UINT32 u4Height, UINT32 u4OffsetX, 
                    UINT32 u4OffsetY, UINT32 u4Pitch)
{
    UINT32 u4WidthMinus1, u4HeightMinus1,u4Jump, u4ActualWidth = u4Pitch / 4;
    INT32 *pi4Buf = (INT32 *)u4Addr;        
    INT32 i4TempValue;    
    INT32 i4R, i4G, i4B, i4A, i4Y, i4Cb, i4Cr, i4I, i4J, i4ILow3;    
    UINT8 *pu1Range = _acRange;   

    pu1Range += 256; 
    pi4Buf = (INT32 *)(u4Addr + (u4OffsetY * u4Pitch) + (u4OffsetX * 4));  
    u4WidthMinus1 = u4Width - 1;
    u4HeightMinus1 = u4Height - 1;
    u4Jump = u4ActualWidth - u4Width;

    for (i4I = u4HeightMinus1; i4I >= 0 ; i4I--)    
    {
        i4ILow3 = i4I & 0x3;
        for (i4J = u4WidthMinus1; i4J >= 0; i4J--)        
        {            
            i4TempValue = *pi4Buf;
            i4A = (i4TempValue & 0xff000000) >> 24;            
            i4Y = (i4TempValue & 0x00ff0000) >> 16;            
            i4Cb = (i4TempValue & 0x0000ff00) >> 8;            
            i4Cr = (i4TempValue & 0x000000ff);             
            
            i4R = (INT32)pu1Range[i4Y + _ai4RR[i4Cr]];  
            i4R = (INT32)(_au2GammaR[i4R] + bDitherMatrix[i4ILow3][i4J&3]) >> 4;
            i4R = (INT32)pu1Range[i4R];  
                        
            i4G = (INT32)pu1Range[i4Y + ((_ai4BG[i4Cb] + _ai4RG[i4Cr]) >> 16)];   
            i4G = (INT32)(_au2GammaG[i4G] + bDitherMatrix[i4ILow3][i4J&3]) >> 4;
            i4G = (INT32)pu1Range[i4G];  
                        
            i4B = (INT32)pu1Range[i4Y + _ai4BB[i4Cb]];
            i4B = (INT32)(_au2GammaB[i4B] + bDitherMatrix[i4ILow3][i4J&3]) >> 4;
            i4B = (INT32)pu1Range[i4B];  
            
            *pi4Buf = (i4A << 24) | (i4R << 16) | (i4G << 8) | i4B;
            pi4Buf++; 
        }
        pi4Buf += u4Jump;
    }
	
}

VOID JDEC_SWGamma(UINT8 *pu1Addr, IMG_LIB_DISP_INFO_T *prDispInfo, UINT32 u4X, UINT32 u4Y)
{
    if (bDrvGetSWGammaFlg())
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
}


BOOL JDEC_Get_Param(JPEGHANDLE hFlow,ENUM_JDEC_PARAM Type,UINT32 u4Param1,UINT32 u4Param2,UINT32 u4Param3)
{
	JPEG_FLOW_DATA_T *prFlow;
    ASSERT(hFlow);
	prFlow = (JPEG_FLOW_DATA_T*)hFlow;

   switch(Type)
   {
      case E_JDEC_PARAM_BS_WPTR:
	  {
	    UINT32 u4RingBufOffset;
		JPEG_JFIF_HEADER_T *prJfif;
	    ASSERT(u4Param1);
        prJfif = prFlow->hJpegHdr;
		u4RingBufOffset = prJfif->u4RingRP + prJfif->u4RingData;
		u4RingBufOffset = (u4RingBufOffset >= prJfif->u4RingBufSize) ? (u4RingBufOffset - prJfif->u4RingBufSize) : u4RingBufOffset;
        *((UINT32 *)u4Param1)=((UINT32)prJfif->pu1RingBufStart)+u4RingBufOffset;
		break;
      }

      case E_JDEC_PARAM_BS_EOF:
	  {
		JPEG_JFIF_HEADER_T *prJfif;
	    ASSERT(u4Param1);
        prJfif = prFlow->hJpegHdr;
		 *((UINT32 *)u4Param1)=(UINT32)prJfif->fgEOF;
        break;
      }
	  case E_JDEC_PARAM_FILESIZE:
	  {
	    ASSERT(u4Param1);
		*((UINT32 *)u4Param1)=prFlow->u4FileSize;
        break;
	  }

	  case E_JDEC_PARAM_CANCELED:
	  {
		  ASSERT(u4Param1);
		  *((UINT32 *)u4Param1)=_fgJpgFlw2Stop;
		  break;
	  }
	  default:
        return FALSE;
   }
   return TRUE;
}

