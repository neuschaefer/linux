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

#include "jpg_common.h"
#include "jpg_if.h"

#include "jpg_hw.h"
#include "jpg_jfif.h"
#include "jpg_mem.h"
#include "fbm_drvif.h" //Christie 07/0307
#include "x_mid.h"
#include "drvcust_if.h"

static void _jmem_NullFunc			(JPEGHANDLE hMem);
static void _jmem_YuvClean			(JPEGHANDLE hMem);
static void _jmem_YuvInit			(JPEGHANDLE hMem);
static void _jmem_YuvMcuFin			(JPEGHANDLE hMem);
static void _jmem_YuvRowFin			(JPEGHANDLE hMem);
static void _jmem_YuvScanFin		(JPEGHANDLE hMem);
static void _jmem_YuvGetOutput		(JPEGHANDLE hMem, void **ppvComp);

static void _jmem_MbMtkClean		(JPEGHANDLE hMem);
static void _jmem_MbMtkInit			(JPEGHANDLE hMem);
static void _jmem_MbMtkMcuFin		(JPEGHANDLE hMem);
static void _jmem_MbMtkRowFin		(JPEGHANDLE hMem);
static void _jmem_MbMtkScanFin		(JPEGHANDLE hMem);
static void _jmem_MbMtkGetOutput	(JPEGHANDLE hMem, void **ppvComp);
#ifdef JPEG_AUTO_TEST
extern INT32 _JpgVerifyCmdGetParam(INT32 i4ParamType, INT32 Param);
#endif

/* aligned and cpu-d-cache-awared memory allocation */
void* JMEM_Alloc(SIZE_T zByteSize)
{
	void *pvMem;

	pvMem = x_mem_alloc((UINT32)zByteSize);
	ASSERT(pvMem != NULL);
	if (pvMem)
	{
		//x_memset(pvMem, 0, zByteSize);
	}

	return pvMem;
}

void JMEM_Free(void* pvMem)
{
	x_mem_free(pvMem);
}

/* virtual functions */
static void _jmem_NullFunc(JPEGHANDLE hMem)
{
	UNUSED(hMem);
	/* null function, do nothing but return */
}

static void _jmem_YuvMcuFin(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_YUV_LINEAR_T*	prMemYuv;

	ASSERT(hMem != NULL);
	prJpegMem = (JPEG_OUTPUT_T*) hMem;
	prMemYuv = (JMEM_YUV_LINEAR_T*) prJpegMem->pvLocalData;
	prJpegMem->au4McuOffset[0] += prMemYuv->au4McuAdv[0];
	prJpegMem->au4McuOffset[1] += prMemYuv->au4McuAdv[1];
	prJpegMem->au4McuOffset[2] += prMemYuv->au4McuAdv[2];
}

static void _jmem_YuvRowFin(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_YUV_LINEAR_T*	prMemYuv;

	ASSERT(hMem != NULL);
	prJpegMem = (JPEG_OUTPUT_T*) hMem;
	prMemYuv = (JMEM_YUV_LINEAR_T*) prJpegMem->pvLocalData;
	prJpegMem->au4McuOffset[0] += prMemYuv->au4RowAdv[0];
	prJpegMem->au4McuOffset[1] += prMemYuv->au4RowAdv[1];
	prJpegMem->au4McuOffset[2] += prMemYuv->au4RowAdv[2];
}

static void _jmem_YuvScanFin(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T*		prJpegMem;
	ASSERT(hMem != NULL);
	prJpegMem = (JPEG_OUTPUT_T*) hMem;

	prJpegMem->au4McuOffset[0] = 0;
	prJpegMem->au4McuOffset[1] = 0;
	prJpegMem->au4McuOffset[2] = 0;
}

static void _jmem_YuvClean(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_BASE_T*		prMemBase;

	ASSERT(hMem != NULL);
	prJpegMem = (JPEG_OUTPUT_T*) hMem;
	prMemBase = (JMEM_BASE_T*) prJpegMem->pvLocalData;
	ASSERT(prMemBase != NULL);

	if (prMemBase->pvFreePoint)
	{
	      //Christie, 07/0307, use fbm instead 
		//JMEM_Free(prMemBase->pvFreePoint);
		prMemBase->pvFreePoint = (void*) NULL;
	}
}

static void _jmem_YuvInit(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_YUV_LINEAR_T*	prMemYuv;
	JMEM_BASE_T*		prMemBase;

	UINT32				au4CompUnitWidth[3];
	UINT32				au4CompUnitHeight[3];
	UINT32				au4CompSize[3];
	//UINT32				au4AllocSize[3];
	FBM_POOL_T* prFbmPool; //Christie 07/0307

	ASSERT(hMem != NULL);
	prJpegMem		= (JPEG_OUTPUT_T*) hMem;
	prMemYuv		= (JMEM_YUV_LINEAR_T*) prJpegMem->pvLocalData;
	prMemBase		= (JMEM_BASE_T*) prJpegMem->pvLocalData;
	ASSERT(prMemBase != NULL);
	//calculate unit width, height and size
	au4CompUnitWidth[0]			= prMemBase->aau4CompFactor[0][0] * D_DCTSIZE;
	au4CompUnitWidth[1]			= prMemBase->aau4CompFactor[1][0] * D_DCTSIZE;
	au4CompUnitWidth[2]			= prMemBase->aau4CompFactor[2][0] * D_DCTSIZE;
	au4CompUnitHeight[0]		= prMemBase->aau4CompFactor[0][1] * D_DCTSIZE;
	au4CompUnitHeight[1]		= prMemBase->aau4CompFactor[1][1] * D_DCTSIZE;
	au4CompUnitHeight[2]		= prMemBase->aau4CompFactor[2][1] * D_DCTSIZE;
	au4CompSize[0]				= au4CompUnitWidth[0] * au4CompUnitHeight[0];
	au4CompSize[1]				= au4CompUnitWidth[1] * au4CompUnitHeight[1];
	au4CompSize[2]				= au4CompUnitWidth[2] * au4CompUnitHeight[2];
	//calculate initial pitch of each component

     //color format setting
#ifdef JPEG_AUTO_TEST
      if(_JpgVerifyCmdGetParam(14,0))
      {
         prMemYuv->u4ColorFMT = (UINT32)E_JPEG_YUV444;
      }
	  else if((au4CompUnitWidth[0]>>1) ==au4CompUnitWidth[1])
#else
      if((au4CompUnitWidth[0]>>1) ==au4CompUnitWidth[1])
#endif
      	{
      	    if((au4CompUnitHeight[0]>>1) ==au4CompUnitHeight[1])
      	    {
      	        prMemYuv->u4ColorFMT = (UINT32)E_JPEG_YUV420;
      	    }
      	    else if(au4CompUnitHeight[0] ==au4CompUnitHeight[1])
      	    {
      	        prMemYuv->u4ColorFMT = (UINT32)E_JPEG_YUV422;
      	    }
      	    else
      	    {
              prMemYuv->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;
      	    }
      	}
      else if(au4CompUnitWidth[0] ==au4CompUnitWidth[1])
      	{
      	    if((au4CompUnitHeight[0]>>1) ==au4CompUnitHeight[1])
      	    {
      	        prMemYuv->u4ColorFMT = (UINT32)E_JPEG_YUV422V;
      	    }
      	    else if(au4CompUnitHeight[0] ==au4CompUnitHeight[1])
      	    {
      	        prMemYuv->u4ColorFMT = (UINT32)E_JPEG_YUV444;
      	    }
      	    else
      	    {
              prMemYuv->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;
      	    }
      	}
      else
      	{
           prMemYuv->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;      	
      	}
      
	prMemYuv->au4McuAdv[0]		= D_DCTSIZE * prMemBase->aau4CompFactor[0][0];
	prMemYuv->au4McuAdv[1]		= D_DCTSIZE * prMemBase->aau4CompFactor[1][0];
	prMemYuv->au4McuAdv[2]		= D_DCTSIZE * prMemBase->aau4CompFactor[2][0];

VLDJPG_SetDecMode5391(VLDJPG_GetDecMode5391());

if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU)
{
	prMemYuv->au4RowAdv[0]		= prMemBase->u4McuWidth * (au4CompSize[0] - au4CompUnitWidth[0]);
	prMemYuv->au4RowAdv[1]		= prMemBase->u4McuWidth * (au4CompSize[1] - au4CompUnitWidth[1]);
	prMemYuv->au4RowAdv[2]		= prMemBase->u4McuWidth * (au4CompSize[2] - au4CompUnitWidth[2]);
}
else if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW)
{
	prMemYuv->au4RowAdv[0]		= prMemBase->u4McuWidth * (au4CompSize[0]);
	prMemYuv->au4RowAdv[1]		= prMemBase->u4McuWidth * (au4CompSize[1]);
	prMemYuv->au4RowAdv[2]		= prMemBase->u4McuWidth * (au4CompSize[2]);
}
else if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ) ||
         (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ))
{
	prMemYuv->au4RowAdv[0]		= prMemBase->u4McuWidth * (au4CompSize[0]);
	prMemYuv->au4RowAdv[1]		= prMemYuv->au4RowAdv[0];
	prMemYuv->au4RowAdv[2]		= prMemYuv->au4RowAdv[0];
}

	prMemYuv->au4AllocSize[0]	= au4CompSize[0] * prMemBase->u4McuCount;
	prMemYuv->au4AllocSize[1]	= au4CompSize[1] * prMemBase->u4McuCount;
	prMemYuv->au4AllocSize[2]	= au4CompSize[2] * prMemBase->u4McuCount;
	prMemYuv->au4RowSize[0]		= au4CompSize[0] * prMemBase->u4McuWidth;
	prMemYuv->au4RowSize[1]		= au4CompSize[1] * prMemBase->u4McuWidth;
	prMemYuv->au4RowSize[2]		= au4CompSize[2] * prMemBase->u4McuWidth;
	prMemYuv->au4RowWidth[0]	= au4CompUnitWidth[0] * prMemBase->u4McuWidth;
	prMemYuv->au4RowWidth[1]	= au4CompUnitWidth[1] * prMemBase->u4McuWidth;
	prMemYuv->au4RowWidth[2]	= au4CompUnitWidth[2] * prMemBase->u4McuWidth;
	prMemYuv->au4RowHeight[0]	= au4CompUnitHeight[0];
	prMemYuv->au4RowHeight[1]	= au4CompUnitHeight[1];
	prMemYuv->au4RowHeight[2]	= au4CompUnitHeight[2];
	
	if (prMemBase->u4McuWidth & 1)
	{
		/* align every row to dram word : row start @ 128 bits */
		if (prMemBase->aau4CompFactor[0][0] == 1)
		{
			//prMemYuv->au4RowAdv[0]		+= 8;
			prMemYuv->au4AllocSize[0]	+= au4CompSize[0] * prMemBase->u4McuHeight;
			prMemYuv->au4RowSize[0]	+= au4CompSize[0];
			prMemYuv->au4RowAdv[0]=prMemYuv->au4RowSize[0];
		}
		if (prMemBase->aau4CompFactor[1][0] == 1)
		{
			//prMemYuv->au4RowAdv[1]		+= 8;
			prMemYuv->au4AllocSize[1]	+= au4CompSize[1] * prMemBase->u4McuHeight;
			prMemYuv->au4RowSize[1]	+= au4CompSize[1];
			prMemYuv->au4RowAdv[1]=prMemYuv->au4RowSize[1];
		}
		if (prMemBase->aau4CompFactor[2][0] == 1)
		{
			//prMemYuv->au4RowAdv[2]		+= 8;
			prMemYuv->au4AllocSize[2]	+= au4CompSize[2] * prMemBase->u4McuHeight;
			prMemYuv->au4RowSize[2]	+= au4CompSize[2];
			prMemYuv->au4RowAdv[2]=prMemYuv->au4RowSize[2];
		}
	}

	/* create idct buffer, coef buffer, nz buffer, etc ... */
	// idct buffer size = sum(n_blocks_comp) * mcu_count * 64
	// nz buffer = 1/8 idct buffer
	// coef buffer = idct buffer * 2
	// we can reuse coef buffer to store idct output, only 2 mcu additional
	// buffer needed

	if (prMemBase->fgProgScan)
	{
	    // winton, tmp FBM buffer use for verification
		//prMemBase->pvFreePoint = (void*) JMEM_Alloc(16 +
		//		(((prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2]) * 17) / 8) +
		//		((au4CompSize[0] + au4CompSize[1] + au4CompSize[2]) * 2));
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG); //for mt5372, output 64 M bound restriction
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != NULL);
        prMemBase->pvFreePoint = (void *)prFbmPool->u4Addr;
	}
	else
	{
	    //prYUV->pvFreePoint = (void *)x_mem_alloc(prYUV->u4SizeY + prYUV->u4SizeU + prYUV->u4SizeV + 31 + 128);
          //Christie 07/0307, tmp FBM buffer use for verification
	    //prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG); //for mt5372, output 64 M bound restriction
        prMemBase->pvFreePoint  =(void *) DRVCUST_OptGet(eFbmMemAddr);
        ASSERT(prMemBase->pvFreePoint != NULL);
	}

	if (!prMemBase->pvFreePoint)
	{
		// FIXME : do something ...
		return;
	}

if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ)
{
	prMemYuv->pvComp[0] = (void *)(((UINT32)prMemBase->pvFreePoint + 15) & (~15));
	prMemYuv->pvComp[1] = (void *)((UINT8*)prMemYuv->pvComp[0] + prMemYuv->au4RowSize[0]);
	prMemYuv->pvComp[2] = (void *)((UINT8*)prMemYuv->pvComp[1] + prMemYuv->au4RowSize[0]);

	prMemYuv->pvComp1[0] = (void *)((UINT8*)prMemYuv->pvComp[2] + prMemYuv->au4RowSize[0]); 
	prMemYuv->pvComp1[1] = (void *)((UINT8*)prMemYuv->pvComp1[0] + prMemYuv->au4RowSize[0]);
	prMemYuv->pvComp1[2] = (void *)((UINT8*)prMemYuv->pvComp1[1] + prMemYuv->au4RowSize[0]);
	x_memset(prMemYuv->pvComp[0], 0x0, prMemYuv->au4RowSize[0]*6);
}
else if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ))
{
	prMemYuv->pvComp[0] = (void *)(((UINT32)prMemBase->pvFreePoint + 15) & (~15));
	prMemYuv->pvComp[1] = (void *)((UINT8*)prMemYuv->pvComp[0] + prMemYuv->au4AllocSize[0]);
	prMemYuv->pvComp[2] = (void *)((UINT8*)prMemYuv->pvComp[1] + prMemYuv->au4AllocSize[0]);

	prMemYuv->pvComp1[0] = (void *)((UINT8*)prMemYuv->pvComp[0] + prMemYuv->au4RowSize[0]); 
	prMemYuv->pvComp1[1] = (void *)((UINT8*)prMemYuv->pvComp[1] + prMemYuv->au4RowSize[0]);
	prMemYuv->pvComp1[2] = (void *)((UINT8*)prMemYuv->pvComp[2] + prMemYuv->au4RowSize[0]);
	x_memset(prMemYuv->pvComp[0], 0x0, prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2]);
}
else
{
	prMemYuv->pvComp[0] = (void *)(((UINT32)prMemBase->pvFreePoint + 15) & (~15));
	prMemYuv->pvComp[1] = (void *)((UINT8*)prMemYuv->pvComp[0] + prMemYuv->au4AllocSize[0]);
	prMemYuv->pvComp[2] = (void *)((UINT8*)prMemYuv->pvComp[1] + prMemYuv->au4AllocSize[1]);

	prMemYuv->pvComp1[0] = (void *)((UINT8*)prMemYuv->pvComp[0] + prMemYuv->au4RowSize[0]); 
	prMemYuv->pvComp1[1] = (void *)((UINT8*)prMemYuv->pvComp[1] + prMemYuv->au4RowSize[1]);
	prMemYuv->pvComp1[2] = (void *)((UINT8*)prMemYuv->pvComp[2] + prMemYuv->au4RowSize[2]);
	x_memset(prMemYuv->pvComp[0], 0x0, prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2]);
}

#if defined(CC_MT5365) || defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(prMemYuv->pvComp[0]), (UINT32)(prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2]));
#else
    HalFlushInvalidateDCache();
#endif

#ifdef JPEG_AUTO_TEST
  if(_JpgVerifyCmdGetParam(18,0))
  	{
  	    #ifdef CC_MT5395                                                                                                                                                                    
	    MID_SetRegionEx(0, MID_AGENT_GFXHI, (UINT32)prMemYuv->pvComp[0],                                                                                                                    
							  (UINT32)prMemYuv->pvComp[0]+prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2],                                                
							  MID_FLAG_WRITEABLE);                                                                                                                                          

            MID_SetRegionEx2(1, MID_AGENT_CPU,                                                                                                                                              
                                    (UINT32)prMemYuv->pvComp[0],                                                                                                                            
                                    (UINT32)prMemYuv->pvComp[0]+prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2],                                          
                                    MID_FLAG_INVERSE,                                                                                                                                       
                                    (~(1<<MID_AGENT_GFXHI)));	                                                                                                                            
                                                                                                                                                                                            
	    MID_SetRegionEx(2, MID_AGENT_GFXHI, 0,                                                                                                                                              
							  (UINT32)prMemYuv->pvComp[0]-4,                                                                                                                                
							  MID_FLAG_INVERSE);  	                                                                                                                                        
  	    #else                                                                                                                                                                               
	    MID_SetRegionEx(0, MID_AGENT_JPEG, (UINT32)prMemYuv->pvComp[0],                                                                                                                     
							  (UINT32)prMemYuv->pvComp[0]+prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2],                                                
							  MID_FLAG_CPUACCESS);                                                                                                                                          
	    #endif                                                                                                                                                                              
            Printf("Addr [%x] size[%x]\n", (UINT32)prMemYuv->pvComp[0], (UINT32)prMemYuv->pvComp[0]+prMemYuv->au4AllocSize[0] + prMemYuv->au4AllocSize[1] + prMemYuv->au4AllocSize[2]); 	
                                                                                                                                                                                            
	    MID_EnableRegionProtect(0);                                                                                                                                                         
  	}
#endif

	// coef & nz buffer for progressive mode
	if (prMemBase->fgProgScan)
	{
	#if 0 // old code
		prMemYuv->pvCoefBuffer[0] = (void*)((UINT8*)prMemYuv->pvComp[0] +
				((au4CompSize[0] + au4CompSize[1] + au4CompSize[2]) * 2));
		prMemYuv->pvCoefBuffer[1] = (void*)((UINT8*)prMemYuv->pvCoefBuffer[0] + prMemYuv->au4AllocSize[0]);
		prMemYuv->pvCoefBuffer[2] = (void*)((UINT8*)prMemYuv->pvCoefBuffer[1] + prMemYuv->au4AllocSize[1]);

		// nz buffer should aligned on 32bits (in 1389)
		prMemBase->apvNzBuffer[0] = (void*)((UINT8*)prMemYuv->pvCoefBuffer[2] + prMemYuv->au4AllocSize[2]);
		prMemBase->apvNzBuffer[1] = (void*)((UINT8*)prMemBase->apvNzBuffer[0] + (prMemYuv->au4AllocSize[0] >> 3));
		prMemBase->apvNzBuffer[2] = (void*)((UINT8*)prMemBase->apvNzBuffer[1] + (prMemYuv->au4AllocSize[1] >> 3));
    #else
        prMemYuv->pvCoefBuffer[0] = (void*)((UINT8*)prMemYuv->pvComp[2] + prMemYuv->au4AllocSize[2]);
        prMemYuv->pvCoefBuffer[1] = (void*)((UINT8*)prMemYuv->pvCoefBuffer[0] + (prMemYuv->au4AllocSize[0] << 1));
        prMemYuv->pvCoefBuffer[2] = (void*)((UINT8*)prMemYuv->pvCoefBuffer[1] + (prMemYuv->au4AllocSize[1] << 1));

		// nz buffer should aligned on 32bits (in 1389)
		prMemBase->apvNzBuffer[0] = (void*)((UINT8*)prMemYuv->pvCoefBuffer[2] + (prMemYuv->au4AllocSize[2] << 1));
		prMemBase->apvNzBuffer[1] = (void*)((UINT8*)prMemBase->apvNzBuffer[0] + (prMemYuv->au4AllocSize[0] >> 3));
		prMemBase->apvNzBuffer[2] = (void*)((UINT8*)prMemBase->apvNzBuffer[1] + (prMemYuv->au4AllocSize[1] >> 3));
    #endif

        x_memset(prMemYuv->pvCoefBuffer[0], 0x13, (prMemYuv->au4AllocSize[0] << 1));
        x_memset(prMemYuv->pvCoefBuffer[1], 0x83, (prMemYuv->au4AllocSize[1] << 1));
        x_memset(prMemYuv->pvCoefBuffer[2], 0x83, (prMemYuv->au4AllocSize[2] << 1));

        x_memset(prMemBase->apvNzBuffer[0], 0x0, (prMemYuv->au4AllocSize[0] >> 3));
        x_memset(prMemBase->apvNzBuffer[1], 0x0, (prMemYuv->au4AllocSize[1] >> 3));
        x_memset(prMemBase->apvNzBuffer[2], 0x0, (prMemYuv->au4AllocSize[2] >> 3));

#if defined(CC_MT5365) || defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(prMemYuv->pvCoefBuffer[0]), (UINT32)((prMemYuv->au4AllocSize[0] << 1) + (prMemYuv->au4AllocSize[1] << 1) + (prMemYuv->au4AllocSize[2] << 1)));
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(prMemBase->apvNzBuffer[0]), (UINT32)((prMemYuv->au4AllocSize[0] >> 3) + (prMemYuv->au4AllocSize[1] >> 3) + (prMemYuv->au4AllocSize[2] >> 3)));
#else
    HalFlushInvalidateDCache();
#endif

	}
	else
	{
        if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ) ||
            (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ))
        {
//    	    x_memset(prMemYuv->pvComp[0], 0x10, prMemYuv->au4AllocSize[0]);
//    	    x_memset(prMemYuv->pvComp[1], 0x83, prMemYuv->au4AllocSize[0]);
//    	    x_memset(prMemYuv->pvComp[2], 0x83, prMemYuv->au4AllocSize[0]);
//    	    HalFlushDCache();
        }
        else
        {
//    	    x_memset(prMemYuv->pvComp[0], 0x10, prMemYuv->au4AllocSize[0]);
//    	    x_memset(prMemYuv->pvComp[1], 0x80, prMemYuv->au4AllocSize[1]);
//    	    x_memset(prMemYuv->pvComp[2], 0x80, prMemYuv->au4AllocSize[2]);
//    	    HalFlushDCache();
        }

		prMemYuv->pvCoefBuffer[0] = prMemYuv->pvComp[0];
		prMemYuv->pvCoefBuffer[1] = prMemYuv->pvComp[1];
		prMemYuv->pvCoefBuffer[2] = prMemYuv->pvComp[2];
		prMemBase->apvNzBuffer[0] = prMemYuv->pvCoefBuffer[0];
		prMemBase->apvNzBuffer[1] = prMemYuv->pvCoefBuffer[1];
		prMemBase->apvNzBuffer[2] = prMemYuv->pvCoefBuffer[2];
	}
}

static void _jmem_YuvGetOutput(JPEGHANDLE hMem, void **ppvComp)
{
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_YUV_LINEAR_T*	prMemYuv;

	ASSERT(hMem != NULL);
	prJpegMem = (JPEG_OUTPUT_T*) hMem;
	prMemYuv = (JMEM_YUV_LINEAR_T*) prJpegMem->pvLocalData;

	ASSERT(ppvComp != NULL);
	
	if (ppvComp[3] != NULL)
	{
		if ((ppvComp[4] == NULL) && (ppvComp[5] == NULL))
		{
			prMemYuv->pvComp[0] = (void *)(((UINT32)ppvComp[3] + 15) & (~15));
			prMemYuv->pvComp[1] = (void *)((UINT8*)prMemYuv->pvComp[0] + prMemYuv->au4RowSize[0]);
			prMemYuv->pvComp[2] = (void *)((UINT8*)prMemYuv->pvComp[1] + prMemYuv->au4RowSize[1]);
		}
		else if ((ppvComp[4] != NULL) && (ppvComp[5] != NULL))
		{
			prMemYuv->pvComp[0] = ppvComp[3];
			prMemYuv->pvComp[1] = ppvComp[4];
			prMemYuv->pvComp[2] = ppvComp[5];
		}
		else
		{
			/* do nothing */
		}
	}

    if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU)
    {
    	ppvComp[0] = (void *)((UINT8*)prMemYuv->pvComp[0] + prJpegMem->au4McuOffset[0]);
    	ppvComp[1] = (void *)((UINT8*)prMemYuv->pvComp[1] + prJpegMem->au4McuOffset[1]);
    	ppvComp[2] = (void *)((UINT8*)prMemYuv->pvComp[2] + prJpegMem->au4McuOffset[2]);

    	// for progressive
    	ppvComp[3] = (void *)((UINT8*)prMemYuv->pvCoefBuffer[0] + (prJpegMem->au4McuOffset[0] * 2));
    	ppvComp[4] = (void *)((UINT8*)prMemYuv->pvCoefBuffer[1] + (prJpegMem->au4McuOffset[1] * 2));
    	ppvComp[5] = (void *)((UINT8*)prMemYuv->pvCoefBuffer[2] + (prJpegMem->au4McuOffset[2] * 2));
    }
    else if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW)
    {
    	ppvComp[0] = (void *)((UINT8*)prMemYuv->pvComp[0] + (prJpegMem->au4McuOffset[0]));
    	ppvComp[1] = (void *)((UINT8*)prMemYuv->pvComp[1] + (prJpegMem->au4McuOffset[1]));
    	ppvComp[2] = (void *)((UINT8*)prMemYuv->pvComp[2] + (prJpegMem->au4McuOffset[2]));

        // for 5391 mcu-ROW
    	ppvComp[6] = (void *)((UINT8*)prMemYuv->pvComp1[0] + (prJpegMem->au4McuOffset[0]));
    	ppvComp[7] = (void *)((UINT8*)prMemYuv->pvComp1[1] + (prJpegMem->au4McuOffset[1]));
    	ppvComp[8] = (void *)((UINT8*)prMemYuv->pvComp1[2] + (prJpegMem->au4McuOffset[2]));
    }
    else if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ) ||
             (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ))
    {
    	ppvComp[0] = (void *)((UINT8*)prMemYuv->pvComp[0] + (prJpegMem->au4McuOffset[0]));
    	ppvComp[1] = (void *)((UINT8*)prMemYuv->pvComp[1] + (prJpegMem->au4McuOffset[0]));
    	ppvComp[2] = (void *)((UINT8*)prMemYuv->pvComp[2] + (prJpegMem->au4McuOffset[0]));

        // for 5391 mcu-ROW
    	ppvComp[6] = (void *)((UINT8*)prMemYuv->pvComp1[0] + (prJpegMem->au4McuOffset[0]));
    	ppvComp[7] = (void *)((UINT8*)prMemYuv->pvComp1[1] + (prJpegMem->au4McuOffset[0]));
    	ppvComp[8] = (void *)((UINT8*)prMemYuv->pvComp1[2] + (prJpegMem->au4McuOffset[0]));
    }
}

/* mtk mb mode layout */

static void _jmem_MbMtkMcuFin(JPEGHANDLE hMem)
{
	UNUSED(hMem);
}

static void _jmem_MbMtkRowFin(JPEGHANDLE hMem)
{
	UNUSED(hMem);
}

static void _jmem_MbMtkScanFin(JPEGHANDLE hMem)
{
	UNUSED(hMem);
}

static void _jmem_MbMtkClean(JPEGHANDLE hMem)
{
	UNUSED(hMem);
}

static void _jmem_MbMtkInit(JPEGHANDLE hMem)
{
	UNUSED(hMem);
}

static void _jmem_MbMtkGetOutput(JPEGHANDLE hMem, void **ppvComp)
{
	UNUSED(hMem);
	UNUSED(ppvComp);
}

/* mw yuv layout */

static void _jmem_MwGetOutput	(JPEGHANDLE hMem, void **ppvComp)
{
	/* mw layout use multiple buffer, buffer base will be passed in by ppvComp */
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_MW_T*			prMemMw;

	ASSERT(hMem != NULL);
	prJpegMem	= (JPEG_OUTPUT_T*) hMem;
	prMemMw		= (JMEM_MW_T*) prJpegMem->pvLocalData;

	ASSERT(ppvComp != NULL);

	if (ppvComp[3] != NULL)
	{
		if ((ppvComp[4] == NULL) && (ppvComp[5] == NULL))
		{
			prMemMw->pvComp[0] = (void *)(((UINT32)ppvComp[3] + 15) & (~15));
			prMemMw->pvComp[1] = (void *)((UINT8*)prMemMw->pvComp[0] + prMemMw->au4RowSize[0]);
			prMemMw->pvComp[2] = (void *)((UINT8*)prMemMw->pvComp[1] + prMemMw->au4RowSize[1]);
		}
		else if ((ppvComp[4] != NULL) && (ppvComp[5] != NULL))
		{
			prMemMw->pvComp[0] = ppvComp[3];
			prMemMw->pvComp[1] = ppvComp[4];
			prMemMw->pvComp[2] = ppvComp[5];
		}
		else
		{
			/* do nothing */
		}
		prMemMw->pvComp1[0] = ppvComp[6];
		prMemMw->pvComp1[1] = ppvComp[7];
		prMemMw->pvComp1[2] = ppvComp[8];
	}
#if 0
	ppvComp[0] = (void *)((UINT8*)prMemMw->pvComp[0] + prJpegMem->au4McuOffset[0]);
	ppvComp[1] = (void *)((UINT8*)prMemMw->pvComp[1] + prJpegMem->au4McuOffset[1]);
	ppvComp[2] = (void *)((UINT8*)prMemMw->pvComp[2] + prJpegMem->au4McuOffset[2]);
#else
    //for mcu-row
    ppvComp[0] = (void *)((UINT8*)prMemMw->pvComp[0] + (prJpegMem->au4McuOffset[0]));
    ppvComp[1] = (void *)((UINT8*)prMemMw->pvComp[1] + (prJpegMem->au4McuOffset[0]));
    ppvComp[2] = (void *)((UINT8*)prMemMw->pvComp[2] + (prJpegMem->au4McuOffset[0]));
    // for 5391 mcu-ROW
    ppvComp[6] = (void *)((UINT8*)prMemMw->pvComp1[0] + (prJpegMem->au4McuOffset[0]));
    ppvComp[7] = (void *)((UINT8*)prMemMw->pvComp1[1] + (prJpegMem->au4McuOffset[0]));
    ppvComp[8] = (void *)((UINT8*)prMemMw->pvComp1[2] + (prJpegMem->au4McuOffset[0]));
#endif
}

static void _jmem_MwInit		(JPEGHANDLE hMem)
{
	/* mw layout doesnt allocate memory when init */
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_MW_T*			prMemMw;
	JMEM_BASE_T*		prMemBase;

	UINT32				au4CompUnitWidth[3];
	UINT32				au4CompUnitHeight[3];
	UINT32				au4CompSize[3];
	//UINT32				au4AllocSize[3];

	ASSERT(hMem != NULL);
	prJpegMem	= (JPEG_OUTPUT_T*) hMem;
	prMemMw		= (JMEM_MW_T*) prJpegMem->pvLocalData;
	prMemBase	= (JMEM_BASE_T*) prJpegMem->pvLocalData;
	ASSERT(prMemBase != NULL);
	//calculate unit width, height and size
	au4CompUnitWidth[0]			= prMemBase->aau4CompFactor[0][0] * D_DCTSIZE;
	au4CompUnitWidth[1]			= prMemBase->aau4CompFactor[1][0] * D_DCTSIZE;
	au4CompUnitWidth[2]			= prMemBase->aau4CompFactor[2][0] * D_DCTSIZE;
	au4CompUnitHeight[0]		= prMemBase->aau4CompFactor[0][1] * D_DCTSIZE;
	au4CompUnitHeight[1]		= prMemBase->aau4CompFactor[1][1] * D_DCTSIZE;
	au4CompUnitHeight[2]		= prMemBase->aau4CompFactor[2][1] * D_DCTSIZE;
	au4CompSize[0]				= au4CompUnitWidth[0] * au4CompUnitHeight[0];
	au4CompSize[1]				= au4CompUnitWidth[1] * au4CompUnitHeight[1];
	au4CompSize[2]				= au4CompUnitWidth[2] * au4CompUnitHeight[2];
	//calculate initial pitch of each component

     //color format setting
      if((au4CompUnitWidth[0]>>1) ==au4CompUnitWidth[1])
      	{
      	    if((au4CompUnitHeight[0]>>1) ==au4CompUnitHeight[1])
      	    {
      	        prMemMw->u4ColorFMT = (UINT32)E_JPEG_YUV420;
      	    }
      	    else if(au4CompUnitHeight[0] ==au4CompUnitHeight[1])
      	    {
      	        prMemMw->u4ColorFMT = (UINT32)E_JPEG_YUV422;
      	    }
      	    else
      	    {
              prMemMw->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;
      	    }
      	}
      else if(au4CompUnitWidth[0] ==au4CompUnitWidth[1])
      	{
      	    if((au4CompUnitHeight[0]>>1) ==au4CompUnitHeight[1])
      	    {
      	        prMemMw->u4ColorFMT = (UINT32)E_JPEG_YUV422V;
      	    }
      	    else if(au4CompUnitHeight[0] ==au4CompUnitHeight[1])
      	    {
      	        prMemMw->u4ColorFMT = (UINT32)E_JPEG_YUV444;
      	    }
      	    else
      	    {
              prMemMw->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;
      	    }
      	}
      else
      	{
           prMemMw->u4ColorFMT = (UINT32)E_JPEG_COLOR_FMT_UNKNOWN;      	
      	}

	prMemMw->au4McuAdv[0]		= D_DCTSIZE * prMemBase->aau4CompFactor[0][0];
	prMemMw->au4McuAdv[1]		= D_DCTSIZE * prMemBase->aau4CompFactor[1][0];
	prMemMw->au4McuAdv[2]		= D_DCTSIZE * prMemBase->aau4CompFactor[2][0];
#if 0	
	prMemMw->au4RowAdv[0]		= prMemBase->u4McuWidth * (au4CompSize[0] - au4CompUnitWidth[0]);
	prMemMw->au4RowAdv[1]		= prMemBase->u4McuWidth * (au4CompSize[1] - au4CompUnitWidth[1]);
	prMemMw->au4RowAdv[2]		= prMemBase->u4McuWidth * (au4CompSize[2] - au4CompUnitWidth[2]);
#else
	prMemMw->au4RowAdv[0]		= prMemBase->u4McuWidth * (au4CompSize[0]);
	prMemMw->au4RowAdv[1]		= prMemMw->au4RowAdv[0];
	prMemMw->au4RowAdv[2]		= prMemMw->au4RowAdv[0];
#endif
	prMemMw->au4RowSize[0]		= au4CompSize[0] * prMemBase->u4McuWidth;
	prMemMw->au4RowSize[1]		= au4CompSize[1] * prMemBase->u4McuWidth;
	prMemMw->au4RowSize[2]		= au4CompSize[2] * prMemBase->u4McuWidth;
	prMemMw->au4RowWidth[0]     	= au4CompUnitWidth[0] * prMemBase->u4McuWidth;
	prMemMw->au4RowWidth[1]	    = au4CompUnitWidth[1] * prMemBase->u4McuWidth;
	prMemMw->au4RowWidth[2] 	= au4CompUnitWidth[2] * prMemBase->u4McuWidth;
	prMemMw->au4RowHeight[0]	= au4CompUnitHeight[0];
	prMemMw->au4RowHeight[1]	= au4CompUnitHeight[1];
	prMemMw->au4RowHeight[2]	= au4CompUnitHeight[2];
	
	if (prMemBase->u4McuWidth & 1)
	{
		/* align every row to dram word : row start @ 128 bits */
		if (prMemBase->aau4CompFactor[0][0] == 1)
		{
			prMemMw->au4RowAdv[0]	+= 8;
			prMemMw->au4RowSize[0]	+= au4CompSize[0];
		}
		if (prMemBase->aau4CompFactor[1][0] == 1)
		{
			prMemMw->au4RowAdv[1]	+= 8;
			prMemMw->au4RowSize[1]	+= au4CompSize[1];
		}
		if (prMemBase->aau4CompFactor[2][0] == 1)
		{
			prMemMw->au4RowAdv[2]	+= 8;
			prMemMw->au4RowSize[2]	+= au4CompSize[2];
		}
	}

	prMemBase->pvFreePoint = (void *)NULL;

    if (prMemBase->fgProgScan)
    {
        JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU);
        VLDJPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT);
    }
    else
    {
        if (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ)
        {
            JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);
            VLDJPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);
        }
        else
        {
            JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ);
            VLDJPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ);
        }
    }
}

static void _jmem_MwClean		(JPEGHANDLE hMem)
{
	_jmem_YuvClean(hMem);
}

static void _jmem_MwMcuFin		(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_MW_T*			prMwMem;

	ASSERT(hMem != NULL);
	prJpegMem	= (JPEG_OUTPUT_T*) hMem;
	prMwMem		= (JMEM_MW_T*) prJpegMem->pvLocalData;
	prJpegMem->au4McuOffset[0] += prMwMem->au4McuAdv[0];
	prJpegMem->au4McuOffset[1] += prMwMem->au4McuAdv[1];
	prJpegMem->au4McuOffset[2] += prMwMem->au4McuAdv[2];
}

static void _jmem_MwRowFin		(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T*		prJpegMem;
	JMEM_MW_T*	prMemYuv;

	ASSERT(hMem != NULL);
	prJpegMem = (JPEG_OUTPUT_T*) hMem;
	prMemYuv = (JMEM_MW_T*) prJpegMem->pvLocalData;
	prJpegMem->au4McuOffset[0] += prMemYuv->au4RowAdv[0];
	prJpegMem->au4McuOffset[1] += prMemYuv->au4RowAdv[1];
	prJpegMem->au4McuOffset[2] += prMemYuv->au4RowAdv[2];
}

static void _jmem_MwScanFin		(JPEGHANDLE hMem)
{
	_jmem_YuvScanFin(hMem);
}

static void _jmem_MwPicFin		(JPEGHANDLE hMem)
{
	UNUSED(hMem);
	/* empty */
}



/* interfaces */

JPEGHANDLE JMEM_AllocInstance(void)
{
	JPEGHANDLE hInstance;
	JPEG_BASE_CLASS_T *prRootObject;

	// allocate jfif header instance
	hInstance = (JPEGHANDLE)x_mem_alloc(sizeof(JPEG_OUTPUT_T));
	if (hInstance == NULL)
	{
		return hInstance;
	}
	x_memset(hInstance, 0, sizeof(JPEG_OUTPUT_T));
	// init its root object
	prRootObject = (JPEG_BASE_CLASS_T *)hInstance;
	prRootObject->eJpegClass = E_JC_UNDEFINED_CLASS;
	prRootObject->zObjectSize = sizeof(JPEG_OUTPUT_T);

	return hInstance;
}

INT32 JMEM_New(JPEGHANDLE hMem, ENUM_JPEG_CLASS_T eMemClass)
{
	JPEG_OUTPUT_T *prJpegMem;
	JPEG_BASE_CLASS_T *prRootObject;

	if (hMem == 0)
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}

	prJpegMem = (JPEG_OUTPUT_T*)hMem;
	prRootObject = (JPEG_BASE_CLASS_T *)hMem;

	if (prRootObject->zObjectSize != sizeof(JPEG_OUTPUT_T))
	{
		return -(INT32)E_JPG_ERR_NOT_INITED;
	}

	switch (eMemClass)
	{
	case E_JC_MEM_YUV:
		prRootObject->eJpegClass	= E_JC_MEM_YUV;
		prJpegMem->pfnGetOutput		= _jmem_YuvGetOutput;
		prJpegMem->pfnInit			= _jmem_YuvInit;
		prJpegMem->pfnClean			= _jmem_YuvClean;
		prJpegMem->pfnMcuFin		= _jmem_YuvMcuFin;
		prJpegMem->pfnRowFin		= _jmem_YuvRowFin;
		prJpegMem->pfnScanFin		= _jmem_YuvScanFin;
		prJpegMem->pfnPicFin		= _jmem_NullFunc;
		prJpegMem->pvLocalData		= x_mem_alloc(sizeof(JMEM_YUV_LINEAR_T));
		break;
	case E_JC_MEM_RGB:
		break;
	case E_JC_MEM_ARGB:
		break;
	case E_JC_MEM_ARGB_GFX:
		break;
	case E_JC_MEM_MB:
		break;
	case E_JC_MEM_MB_MTK:
		prRootObject->eJpegClass	= E_JC_MEM_MB_MTK;
		prJpegMem->pfnGetOutput		= _jmem_MbMtkGetOutput;
		prJpegMem->pfnInit			= _jmem_MbMtkInit;
		prJpegMem->pfnClean			= _jmem_MbMtkClean;
		prJpegMem->pfnMcuFin		= _jmem_MbMtkMcuFin;
		prJpegMem->pfnRowFin		= _jmem_MbMtkRowFin;
		prJpegMem->pfnScanFin		= _jmem_MbMtkScanFin;
		prJpegMem->pfnPicFin		= _jmem_NullFunc;
		prJpegMem->pvLocalData		= x_mem_alloc(sizeof(JMEM_MB_MTK_T));
		break;
	case E_JC_MEM_MW:
		prRootObject->eJpegClass	= E_JC_MEM_MW;
		prJpegMem->pfnGetOutput		= _jmem_MwGetOutput;
		prJpegMem->pfnInit			= _jmem_MwInit;
		prJpegMem->pfnClean			= _jmem_MwClean;
		prJpegMem->pfnMcuFin		= _jmem_MwMcuFin;
		prJpegMem->pfnRowFin		= _jmem_MwRowFin;
		prJpegMem->pfnScanFin		= _jmem_MwScanFin;
		prJpegMem->pfnPicFin		= _jmem_MwPicFin;
		prJpegMem->pvLocalData		= x_mem_alloc(sizeof(JMEM_MW_T));
		break;
	default:
		return -(INT32)E_JPG_ERR_PARAM;
	}

	return (INT32)E_JPG_OK;
}

void JMEM_FreeInstance(JPEGHANDLE hMem)
{
	JPEG_OUTPUT_T *prJpegMem;
	JPEG_BASE_CLASS_T *prRootObject;

	if (hMem == 0)
	{
		return;
	}

	prJpegMem = (JPEG_OUTPUT_T*)hMem;
	prRootObject = (JPEG_BASE_CLASS_T *)hMem;

	if (prRootObject->zObjectSize != sizeof(JPEG_OUTPUT_T))
	{
		return;
	}

	if (prJpegMem->pvLocalData)
	{
		prJpegMem->pfnClean(hMem);
		x_mem_free(prJpegMem->pvLocalData);
	}

	x_mem_free(prJpegMem);
}


