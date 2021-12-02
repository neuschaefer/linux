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
/*-----------------------------------------------------------------------------
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp6hwdec.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp6hwdec.c
 *  This file contains implementation of Vp6 hw decoder.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_printf.h"
#include "vdec_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "srm_drvif.h"
LINT_EXT_HEADER_END
#include "vdec_vp6util.h"
#include "vdec_vp6dec.h"
#include "vdec_vp6psr.h"
#include "vdec_vp6hw.h"
#include "vdec_vp6if.h"

#define MAX_ES_NUM VDEC_MAX_ES

BOOL _Vdec_VP6ParseFrameHeader(VDEC_VP6_INFO_T *prDecInfo);
extern VDEC_VP6_INFO_T *VDEC_Vp6GetLockedInstance(void);

#ifdef VDEC_TIME_PROFILE
static HAL_TIME_T _rTimeHwS[MAX_ES_NUM], _rTimeHwDt[MAX_ES_NUM], _rTimeHwE[MAX_ES_NUM];
static UINT32 _u4DelayClock;
static UINT32 _u4DramCycle;
static UINT32 _u4DecCycle = 0;

void _VDEC_VP6TimeHwStart(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T *prDecPrm;
    UINT32 uEsId;
    
    if (!prDecInfo)
    {
        return;
    }
    uEsId       = prDecInfo->uEsId;
    prDecPrm    = &prDecInfo->rDecParam[prDecInfo->u1StrmId];

	prDecPrm->u4CurDecMs = 0;
	prDecPrm->u4CurDramCycle = 0;
	prDecPrm->u4CurDecCycle = 0;

	HAL_GetTime(&_rTimeHwS[uEsId]);
	
    _u4DelayClock = BSP_GetDomainClock(SRC_MEM_CLK); //dram cycle /sec
    _u4DramCycle  = SRM_DramMonitor(0, SRM_DRV_VDEC_MC, _u4DelayClock, 0);
}
#endif // VDEC_TIME_PROFILE

static void _Vdec_VP6HwLsr(UINT16 u2Vector)
{
    VP6_DEC_PARAM_T* prDecParam;
	UINT32 u4DecErr, u4DecDone;
	
    if (u2Vector == AVLD_VECTOR)
    {
        VDEC_VP6_INFO_T *prDecInfo = VDEC_Vp6GetLockedInstance();
        ASSERT(prDecInfo != NULL);
        u4DecDone = u4VDEC_HAL_VP6_VDec_ReadFinishFlag((BOOL)VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_FINISH_PARAM))&VP6_VPDF;
    	u4DecErr = u4VDecReadVP6VLD(RO_VP6_VLD_ERR_EX);

		if (u4DecDone)
		{
#ifdef VDEC_TIME_PROFILE
            HAL_GetTime(&_rTimeHwE[prDecInfo->uEsId]);
            HAL_GetDeltaTime(&_rTimeHwDt[prDecInfo->uEsId], &_rTimeHwS[prDecInfo->uEsId], &_rTimeHwE[prDecInfo->uEsId]);
#endif

            vVDEC_HAL_VP6_VDec_ClearFinishFlag();

            if (u4DecErr && prDecInfo->u1StrmId<MAX_STRM_NS)
            {
                UINT32 u4X, u4Y;
                prDecParam = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
                
                vVDEC_HAL_VP6_GetMbxMby(&u4X, &u4Y);
				if(VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_MULTI))
				{
					printf( "!!! Enter Error Handling Err: 0x%x, IntMsk: 0x%x, Pos: (%d, %d)\n", u4DecErr, u4VDecReadVP6VLD(RW_VP6_VLD_ETCB), u4X, u4Y);
				}
				else
				{
					printf( "!!! Enter Error Handling Err: 0x%x, IntMsk: 0x%x, Pos: (%d, %d)\n", u4DecErr, u4VDecReadVP6VLD(RW_VP6_VLD_ETCA), u4X, u4Y);
				}
                prDecParam->u4DecErrType = u4DecErr;
            }

            // Clear VDEC int. status
            //vVDecWriteVDECMisc(RW_VDEC_MISC_INT_CFG, INT_CFG_WAIT_BITS_NOP|INT_CFG_INT_CLR|INT_CFG_CLR_INT_MD);   // 0x1011
            //vVDecWriteVDECMisc(RW_VDEC_MISC_INT_CFG, INT_CFG_WAIT_BITS_NOP|INT_CFG_CLR_INT_MD);                   // 0x1001
            BIM_ClearIrq(AVLD_VECTOR);

            VERIFY(x_sema_unlock(prDecInfo->hVdecVP6FinSema) == OSR_OK);
		}
    }
}

void Vp6IsrInit(VDEC_VP6_INFO_T* prDecInfo)
{
    #ifndef VDEC_COMMON_ISR
    x_os_isr_fct pfnOldIsr;
    #endif

    if (!prDecInfo)
    {
        return;
    }

    if(VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_POLLING))
    {
        #ifdef VDEC_COMMON_ISR
        _VDEC_RegIsr(AVLD_VECTOR, NULL);
        #else
		// unreg ISR	  
		if (x_reg_isr(AVLD_VECTOR, NULL, &pfnOldIsr) != OSR_OK)
		{
			ASSERT(0);
		}
		#endif
		UNUSED(prDecInfo);
    }
	else
	{
		if(prDecInfo->hVdecVP6FinSema == (HANDLE_T)NULL)
		{
			VERIFY(x_sema_create(&prDecInfo->hVdecVP6FinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
		}
		
        #ifdef VDEC_COMMON_ISR
        _VDEC_RegIsr(AVLD_VECTOR, _Vdec_VP6HwLsr);
        #else
		// reg ISR		
		if (x_reg_isr(AVLD_VECTOR, _Vdec_VP6HwLsr, &pfnOldIsr) != OSR_OK)
		{
			ASSERT(0);
		}
		#endif
	}
}

BOOL fgIsVP6VDecComplete(VDEC_INFO_VP6_FRM_HDR_T* prFrmHdr)
{
    UINT32 u4MbX, u4MbY;  

    if (!prFrmHdr)
    {
        return FALSE;
    }
    
    vVDEC_HAL_VP6_GetMbxMby(&u4MbX, &u4MbY);

    if( (u4MbX < (prFrmHdr->u2WidthDec / 16 - 1) ) || (u4MbY < (prFrmHdr->u2HeightDec / 16 -1)) )
    {
        return FALSE;
    }
    
    return TRUE;
}

BOOL _VDEC_WaitVP6DecDone(VDEC_VP6_INFO_T *prDecInfo)
{
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    VP6_DEC_PARAM_T* prDecParam;
    UINT32 u4Cnt = 0;
    UINT32 u4CntTimeChk = 0;
    UINT32 u4MbX = 0;
    UINT32 u4MbY = 0;  
    UINT32 u4MbX_last;
    UINT32 u4MbY_last;
	UINT32 u4Flag=0;
    BOOL fgVDecComplete = FALSE;

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return FALSE;
    }

    
	u4Flag=prDecInfo->rDecParamEx.u4Flag;
    if ((!VP6_ISSET(u4Flag,VP6_DEC_FLG_POLLING))
		&&!_Vdec_VP6Hw_DecWaitFinish(prDecInfo))
    {
        LOG(1, "DEC error or timeout\n");
        ASSERT(0);
        return FALSE;
    }

    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];

#ifdef CC_VP6_EMULATION
    while(TRUE)
#else
    while (u4CntTimeChk < DEC_RETRY_NUM)
#endif
    {    
        u4Cnt ++;    
        if ((u4Cnt & 0x3f)== 0x3f)
        {
			if (prDecParam->u4DecErrType)
			{
				LOG(1, "Decode done due to dec err %d\n",prDecParam->u4DecErrType);
				break;
			}
			
			if(VP6_ISSET(u4Flag,VP6_DEC_FLG_POLLING))
			{
               if((u4VDEC_HAL_VP6_VDec_ReadFinishFlag((BOOL)VP6_ISSET(u4Flag,VP6_DEC_FLG_FINISH_PARAM))&VP6_VPDF) 
			   	|| (VP6_ISSET(u4Flag,VP6_DEC_FLG_FINISH_PARAM)&&(u4VDecReadVP6VLD(RO_VP6_VLD_ERR_EX))))
               {
				   fgVDecComplete = TRUE;
               }
			}
			else
			{
                fgVDecComplete = TRUE;
			}

			if(fgVDecComplete && VP6_ISSET(u4Flag,VP6_DEC_FLG_FINISH_PARAM))
			{
                prDecParam->u4DecErrType = u4VDecReadVP6VLD(RO_VP6_VLD_ERR_EX);
			}

            if (fgVDecComplete && fgIsVP6VDecComplete(prFrmHdr))
            {
                u4CntTimeChk = 0;
                break;
            }
            else
            {
                u4MbX_last = u4MbX;
                u4MbY_last = u4MbY;
                vVDEC_HAL_VP6_GetMbxMby(&u4MbX, &u4MbY);
                if ((u4MbX == u4MbX_last) && (u4MbY == u4MbY_last))
                {
                    u4CntTimeChk ++;
                }
                else
                {
                    u4CntTimeChk = 0;
                }
                
				if(VP6_ISSET(u4Flag,VP6_DEC_FLG_POLLING))
				{
				  x_thread_delay(5);
                }
            }
            u4Cnt = 0;
        }
    }

#ifdef VDEC_TIME_PROFILE
    prDecParam->u4CurDecMs += (_rTimeHwDt[prDecInfo->uEsId].u4Micros / 1000);
    _u4DramCycle = IO_REG32(DRAM_BASE, 0x190);
    _u4DecCycle  = IO_REG32(MC_BASE, 0x778);
    prDecParam->u4CurDecCycle  += _u4DecCycle;
    prDecParam->u4CurDramCycle += _u4DramCycle;
#endif // VDEC_TIME_PROFILE

    if ((prDecParam->u4DecErrType) || (u4CntTimeChk >= DEC_RETRY_NUM))
    {
        LOG(1, "DEC error or timeout\n");
        return FALSE;
    }

    return TRUE;
}

void _VDEC_SetVP6Wptr(UINT32 u4Wptr)
{
    _VDEC_SetVP6VldWPtr(0, u4Wptr);
    _VDEC_SetVP6VldWPtr(1, u4Wptr);
}

#ifdef VP6_DRAM_BUSY_TEST
#define DRAM_BUSY_BUF_SIZE  (64*1024)
UINT8 _au1DramBusyBuf[DRAM_BUSY_BUF_SIZE];

static void _VDEC_VP6SetDramBusy(void)
{
    IO_WRITE32((IO_VIRT + 0x07200),  0x18, IO_READ32((IO_VIRT + 0x07200), 0x18) | (1 << 25) | (1 <<31));
    IO_WRITE32((IO_VIRT + 0x07200),  0x10, _au1DramBusyBuf);     // addr
    IO_WRITE32((IO_VIRT + 0x07200),  0x14, DRAM_BUSY_BUF_SIZE);        // size
    IO_WRITE32((IO_VIRT + 0x7100), 0x4, IO_READ32((IO_VIRT + 0x7100), 0x4) & 0xFFFFFFF);
    IO_WRITE32((IO_VIRT + 0x7100), 0x14, 0xFFFFFF11);
}
#endif

BOOL _Vdec_VP6Hw_init(VDEC_VP6_INFO_T *prDecInfo)
{
    VDEC_INFO_DEC_PRM_T* prDecParamEx;

    if (!prDecInfo)
    {
        return FALSE;
    }

    prDecParamEx = &prDecInfo->rDecParamEx;
    prDecParamEx->fgAdobeMode = VP6_DEC_MODE;
    
#if defined(CC_VP6_EMULATION) || defined(__MODEL_slt__)
	#ifdef VP6_64X32_BLK_MODE
	prDecParamEx->u4AddrMode = ADDR_MODE_5351_NORMAL;
	#else
    prDecParamEx->u4AddrMode = ADDR_MODE_8520;
	#endif
#else
    prDecParamEx->u4AddrMode = ADDR_MODE_5351_NORMAL;
#endif

    _Vdec_VP6Hw_Reset(prDecInfo->uEsId);

#ifdef VP6_DRAM_BUSY_TEST
    _VDEC_VP6SetDramBusy();
#endif

    return TRUE;
}

void vVerVP6SetBufStatus(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T* prDecPrm;
    VDEC_INFO_VP6_PP_INFO_T* prPpInfo;
    UINT32 u4YSa, u4CSa;

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return;
    }

    prDecPrm = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prPpInfo = &prDecInfo->rPpInfo;
    
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucDbkFbId, &u4YSa, &u4CSa);
    prPpInfo->fgPpEnable = VDEC_PP_ENABLE;
    prPpInfo->u4PpYBufSa = u4YSa;
    prPpInfo->u4PpCBufSa = u4CSa;
}

INT32 _Vdec_Vp6HwDecode(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T *prDecPrm;
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    VDEC_INFO_DEC_PRM_T *prDecPrmEx;

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return -1;
    }

    prDecPrm    = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];
    prDecPrmEx  = &prDecInfo->rDecParamEx;

    if (prFrmHdr->ucFrameType == VP6_I_FRM)
    {
        u4VDEC_HAL_VP6_Default_Models_Init((BOOL)VP6_ISSET(prDecPrmEx->u4Flag,VP6_DEC_FLG_SUPPORT_MULTI));
        u4VDEC_HAL_VP6_Load_QMatrix();
    }
    else
    {
        u4VDEC_HAL_VP6_Parse_Mb_Type_Models((BOOL)VP6_ISSET(prDecPrmEx->u4Flag,VP6_DEC_FLG_SUPPORT_MULTI));
    }
    
    u4VDEC_HAL_VP6_Load_Filter_Coef(prFrmHdr->u4Filter_Selection);

#ifdef VDEC_VP6_HWDEBLOCK
    vVerVP6SetBufStatus(prDecInfo);
#endif

    prDecPrmEx->ucPicStruct = FRM_PIC;
    prDecPrmEx->ucPicType   = prFrmHdr->ucFrameType;
    prDecPrmEx->u4PicBW     = prFrmHdr->u2WidthDec;
    prDecPrmEx->u4PicW      = prFrmHdr->u2WidthDec;
    prDecPrmEx->u4PicH      = prFrmHdr->u2HeightDec;
	prDecPrm->u4DecErrType  = 0;
	
    i4VDEC_HAL_VP6_DecStart(prDecInfo);

    return TRUE;
}

BOOL _Vdec_VP6HwGetHeaderInfo(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T *prDecParam;
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    VDEC_PES_INFO_T *prPesInfo;
#ifdef VDEC_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt; 
    HAL_TIME_T rTimeSwS, rTimeSwDt,rTimeSwE;
#endif // VDEC_TIME_PROFILE

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return FALSE;
    }

    prPesInfo   = &prDecInfo->rPesInfo;
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstS);
#endif // VDEC_TIME_PROFILE

    _VDEC_VP6PsrRest(prDecInfo);

#ifdef CC_VP6_EMULATION
    prDecParam->u4WPtr = prPesInfo->u4VldReadPtr + prPesInfo->u4TickNum;
#endif

    prFrmHdr->u4FrameSize = (prDecParam->u4WPtr > prPesInfo->u4VldReadPtr) ?
        (prDecParam->u4WPtr - prPesInfo->u4VldReadPtr) :
        (prDecParam->u4WPtr + prPesInfo->u4FifoEnd - prPesInfo->u4VldReadPtr);

    _VDEC_SetVP6BsInfo(prDecInfo->uEsId, prPesInfo->u4VldReadPtr, prDecParam->u4WPtr, prPesInfo->u4FifoStart, prPesInfo->u4FifoEnd, NULL);

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    prDecParam->u4CurRstMs += (rTimeRstDt.u4Micros / 1000);
#endif // VDEC_TIME_PROFILE

#ifdef VP6_BOOL_CODER_TEST
    {
    UINT32 u4Cnt, u4BitCnt, u4ReadPtr = prPesInfo->u4VldReadPtr, u4ReadEnd = prDecParam->u4WPtr;
    UINT8 u1V1, u1V2, u1NN;
    UINT32 u4RegVal, u4Tmp;

    printf("Start bool coder test\n");
    vVDecWriteVP6VLD(RW_VP6_INIT_BOOL, RW_VP6_BOOL_EN);

    u4Cnt = 1;
    u4BitCnt = 0;
    while (u4ReadPtr < u4ReadEnd - 1)
    {
        u1V1 = (*((volatile UINT8*)u4ReadPtr)) << u4BitCnt;
        if (8 - u4BitCnt > u4Cnt)
        {
            u1V1 = u1V1 >> (8 - u4Cnt);
            u1V2 = 0;
        }
        else
        {
            u1V1 = u1V1 >> (8 - u4Cnt);

            u1V2 = (*((volatile UINT8*)(u4ReadPtr + 1)));
            u1V2 = u1V2 >> (8 - (u4Cnt - (8 - u4BitCnt)));
        }

        u1V1 = u1V1 | u1V2;
        u1NN = (u1V1 << 1) + (u1V1 == 0);

        vVDecWriteVP6VLD(RW_VP6_BCPR, 0x18000+u4Cnt);
        u4Tmp =  u4VDecWaitVP6GetBitsReady();
        u4RegVal = ((u4Tmp & 0xFF00) >> 8);

        if (u1V1 != u4RegVal)
        {
            printf( "ERROR !!!\n");
            ASSERT(0);
        }

        u4RegVal = (u4Tmp & 0xFF0000) >> 16;
        if (u1NN != u4RegVal)
        {
            printf( "NN !!!\n");
            ASSERT(0);
        }
        
        u4ReadPtr += ((u4BitCnt + u4Cnt) / 8);
        u4BitCnt = (u4BitCnt + u4Cnt) % 8;
        u4Cnt = (u4Cnt >= 7) ? 1 : u4Cnt + 1;
    }
    printf("End bool coder test\n");
    while(TRUE);
    }
#endif

    // Record the offset of each picture
    prDecParam->u8Offset = prPesInfo->u8Offset;

#ifdef VP6_CONTEXT_ADDR_TEST
    vVDecWriteVP6VLD(RW_CONTEXT_NR, 0x00FFFFFF);
    vVDecWriteVLDTop(RW_PRED_DRAM_B_ADDR, 0x900000);
    vVDecWriteVP6VLD(RW_MBT_B_ADDR, u4VDecReadVLDTop(RW_PRED_DRAM_B_ADDR) + u4VDecReadVP6VLD(RW_PRED_NR) * 16);
    vVDecWriteVP6VLD(RW_MVD_B_ADDR, u4VDecReadVP6VLD(RW_MBT_B_ADDR) + (u4VDecReadVP6VLD(RW_CONTEXT_NR) & 0xFF) * 16);
    vVDecWriteVP6VLD(RW_CFM_B_ADDR, u4VDecReadVP6VLD(RW_MVD_B_ADDR) + ((u4VDecReadVP6VLD(RW_CONTEXT_NR) & 0xFF00) >> 8) * 16);
#endif

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeSwS);
#endif // VDEC_TIME_PROFILE

    // Parse header
    if (!_Vdec_VP6ParseFrameHeader(prDecInfo))
    {
        return FALSE;
    }

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeSwE);
    HAL_GetDeltaTime(&rTimeSwDt, &rTimeSwS, &rTimeSwE);
    prDecParam->u4CurSwMicro += rTimeSwDt.u4Micros;
#endif // VDEC_TIME_PROFILE

#if defined(ERROR_TYPE_TEST) || defined(ERROR_STRESS_TEST) || defined(VP6_ENABLE_ERROR_CONCEALMENT)
    if (prFrmHdr->u2WidthDec == 0 || prFrmHdr->u2WidthDec > 1920)
    {
        prDecParam->u2NewWidth  = 720;
    }
    else
    {
        prDecParam->u2NewWidth = prFrmHdr->u2WidthDec;
    }
    
    if (prFrmHdr->u2HeightDec == 0 || prFrmHdr->u2HeightDec > 1088)
    {
        prDecParam->u2NewHeight = 480;
    }
    else
    {
        prDecParam->u2NewHeight = prFrmHdr->u2HeightDec;
    }
#else
    prDecParam->u2NewWidth  = prFrmHdr->u2WidthDec;
    prDecParam->u2NewHeight = prFrmHdr->u2HeightDec;
#endif

    prDecParam->fgKeyFrame  = (prFrmHdr->ucFrameType == VP6_I_FRM);
    prDecParam->fgRefreshGd = prFrmHdr->fgRefreshGoldenFrame;

    return TRUE;
}

BOOL _Vdec_VP6Hw_Release(UINT8 u1ucEsId)
{
    return TRUE;
}

BOOL _Vdec_VP6Hw_DecWaitFinish(VDEC_VP6_INFO_T *prDecInfo)
{
    if (!prDecInfo)
    {
        return FALSE;
    }
    
    if (x_sema_lock_timeout(prDecInfo->hVdecVP6FinSema, VDEC_VP6WAIT_TIME) != OSR_OK)
    {
        LOG(1,"vp6 decoder timeout\n");
        return FALSE;
    }

    return TRUE;
}

BOOL _Vdec_VP6Hw_Reset(UINT8 u1ucEsId)
{
#if !defined(TEST_ONOFF_RUNTIME)
    vVDecWriteVDECMisc(0xC8, 0xFFFFFFFF);
    vVDecWriteVDECMisc(0xCC, 0xFFFFFFFF);
#endif

    _VDEC_VP6VLDHWReset(0);
    _VDEC_VP6VLDHWReset(1);
    UNUSED(u1ucEsId);
    return TRUE;
}

VOID _Vdec_VP6HwSetByteCount(UINT32 u4Id, UINT32 u4Ofst)
{
    if (u4Id == 0)
    {
        vVDecWriteVP6VLD(RW_VP6_PAD_ZERO_MODE, 1);
        vVDecWriteVP6VLD(RW_VP6_PIC_BYTE_CNT, u4Ofst);
    }
    else
    {
        vVDecWriteVP6VLD(RW_VP6_PAD_ZERO_MODE2, 1);
        vVDecWriteVP6VLD(RW_VP6_PIC_BYTE_CNT2, u4Ofst);
    }
}

INT32 i4VP6_Parse_Intra_Header(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T *prDecParam;
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    UINT32 u4Datain;
    
    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return FALSE;
    }
    
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];
    
    u4Datain = u4VDEC_HAL_VP6_GetBitStreamShift(8);
    prDecParam->u4DecReadBytes += 1;
    prFrmHdr->ucVp3VerNo = (u4Datain >> 27);
    prFrmHdr->ucVpProfile = (VP6_PROFILE_T) ((u4Datain >> 25) & 0x3);
    prFrmHdr->ucReserved = ((u4Datain >> 24) & 0x1);    

#if !defined(ERROR_TYPE_TEST) && !defined(ERROR_STRESS_TEST) && !defined(VP6_ENABLE_ERROR_CONCEALMENT)
    // assert here CCCI_TX check sub_version with CodecID
    if ((prFrmHdr->ucVp3VerNo > 8) || (prFrmHdr->ucVp3VerNo < 6))
    {
        ASSERT(0);
        return 0;
    }

    if ((prFrmHdr->ucVpProfile == VP6_PROFILE_UNDEF_1) || (prFrmHdr->ucVpProfile == VP6_PROFILE_UNDEF_2))
    {
        ASSERT(0);
        return 0;
    }
    
    if (prFrmHdr->ucReserved != 0) 
    {
        ASSERT(0);
        return 0;
    }
#endif

    if (prFrmHdr->fgMultiStream || (prFrmHdr->ucVpProfile == VP6_PROFILE_SIMPLE)) 
    {   
        if(VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_MULTI))
        {
			u4Datain = u4VDEC_HAL_VP6_GetBitStreamShift(16);
			prDecParam->u4DecReadBytes += 2;
			prFrmHdr->u2Buff2Offset = (UINT16) ((u4Datain >> 16) & 0xFFFF);
        }
		else
		{
            LOG(2,"Vdec vp6 unsupport multi type stream1 \n");
			return 0;
		}
    }
    else
    {
        prFrmHdr->u2Buff2Offset = 0;
    }

    //Bool Coder: init range decoder
    // eason : dim_x, dim_y
    u4Datain = u4VDEC_HAL_VP6_GetBitStreamShift(32);
    prDecParam->u4DecReadBytes += 4;
    prFrmHdr->u2VFragments      = (UINT16) (u4Datain >> 24 & 0xFF);
    prFrmHdr->u2HFragments      = (UINT16) (u4Datain >> 16 & 0xFF);

    prFrmHdr->u2OutVFragments   = (UINT16) (u4Datain >> 8 & 0xFF);
    prFrmHdr->u2OutHFragments   = (UINT16) (u4Datain  & 0xFF);

    prFrmHdr->u2WidthDec        = 16 * prFrmHdr->u2HFragments;
    prFrmHdr->u2HeightDec       = 16 * prFrmHdr->u2VFragments;

    u4VDEC_HAL_VP6_InitBoolCoder(0);//??? vp56_init_range_decoder(c, buf+6, buf_size-6);

    if (prFrmHdr->u2Buff2Offset)
    {
        _Vdec_VP6HwSetByteCount(0, prFrmHdr->u2Buff2Offset - 6);
    }
    else
    {
        _Vdec_VP6HwSetByteCount(0, prFrmHdr->u4FrameSize - 6);
    }

    prFrmHdr->ucScalingMode = (UCHAR) ((u4VDEC_HAL_VP6_GetBoolCoderShift(2) ) & 0x3);
    prDecParam->u4DecReadBits += 2;

    if (prFrmHdr->ucVp3VerNo < 8)
    {
        prFrmHdr->ucVrt_Shift = 5;
    }

    prFrmHdr->fgParse_Filter_Info = ((prFrmHdr->ucVpProfile == VP6_PROFILE_ADVANCED)? TRUE: FALSE);

    return 1;
}

INT32 i4VP6_Parse_Inter_Header(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T *prDecParam;
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    UINT32 u4Datain;
    
    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return FALSE;
    }

    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];

#if !defined(ERROR_TYPE_TEST) && !defined(ERROR_STRESS_TEST) && !defined(VP6_ENABLE_ERROR_CONCEALMENT)
    if ((prFrmHdr->ucVp3VerNo > 8) || (prFrmHdr->ucVp3VerNo < 6))
    {
        ASSERT(0);
        return 0;
    }

    if ((prFrmHdr->ucVpProfile == VP6_PROFILE_UNDEF_1) || (prFrmHdr->ucVpProfile == VP6_PROFILE_UNDEF_2))
    {
        ASSERT(0);
        return 0;
    }
    
    if (prFrmHdr->ucReserved != 0) 
    {
        ASSERT(0);
        return 0;
    }
#endif

    if (prFrmHdr->fgMultiStream || (prFrmHdr->ucVpProfile == VP6_PROFILE_SIMPLE)) 
    {
	   if(VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_MULTI))
	   {
		   u4Datain = u4VDEC_HAL_VP6_GetBitStreamShift(16);
		   prDecParam->u4DecReadBytes += 2;
		   prFrmHdr->u2Buff2Offset = (UINT16) ((u4Datain >> 16) & 0xFFFF);
	   }
	   else
	   {
		   LOG(2,"Vdec vp6 unsupport multi type stream2 \n");
		   return 0;
	   }
    }
    else
    {
        prFrmHdr->u2Buff2Offset = 0;
    }

    u4VDEC_HAL_VP6_InitBoolCoder(0); //vp56_init_range_decoder(c, buf+1, buf_size-1);

    if (prFrmHdr->u2Buff2Offset)
    {
        _Vdec_VP6HwSetByteCount(0, prFrmHdr->u2Buff2Offset - 1);
    }
    else
    {
        _Vdec_VP6HwSetByteCount(0, prFrmHdr->u4FrameSize - 1);
    }

    //Table 2
    prFrmHdr->fgRefreshGoldenFrame = (u4VDEC_HAL_VP6_GetBoolCoderShift(1) )? TRUE: FALSE;
    prDecParam->u4DecReadBits += 1;
    if (prFrmHdr->ucVpProfile == VP6_PROFILE_ADVANCED)
    {
        prFrmHdr->u2LoopFilter = (u4VDEC_HAL_VP6_GetBoolCoderShift(1) )? 1: 0 ;
        prDecParam->u4DecReadBits += 1;
        if (prFrmHdr->u2LoopFilter)
        {
            prFrmHdr->fgLoopFilterSelector = (u4VDEC_HAL_VP6_GetBoolCoderShift(1) )? TRUE: FALSE;
            prDecParam->u4DecReadBits += 1;
        }
        
        if (prFrmHdr->ucVp3VerNo > 7)
        {
            prFrmHdr->fgParse_Filter_Info = (u4VDEC_HAL_VP6_GetBoolCoderShift(1) )? TRUE: FALSE;
            prDecParam->u4DecReadBits += 1;
        }
    }
    
    UNUSED(u4Datain);
    return 1;
}

const UCHAR vp56_filter_threshold[] = {
    14, 14, 13, 13, 12, 12, 10, 10,
    10, 10,  8,  8,  8,  8,  8,  8,
    8,  8,  8,  8,  8,  8,  8,  8,
    8,  8,  8,  8,  8,  8,  8,  8,
    8,  8,  8,  8,  7,  7,  7,  7,
    7,  7,  6,  6,  6,  6,  6,  6,
    5,  5,  5,  5,  4,  4,  4,  4,
    4,  4,  4,  3,  3,  3,  3,  2,
};

const UCHAR vp56_ac_dequant[64] = {
    94, 92, 90, 88, 86, 82, 78, 74,
    70, 66, 62, 58, 54, 53, 52, 51,
    50, 49, 48, 47, 46, 45, 44, 43,
    42, 40, 39, 37, 36, 35, 34, 33,
    32, 31, 30, 29, 28, 27, 26, 25,
    24, 23, 22, 21, 20, 19, 18, 17,
    16, 15, 14, 13, 12, 11, 10,  9,
    8,  7,  6,  5,  4,  3,  2,  1,
};

const UCHAR vp56_dc_dequant[64] = {
    47, 47, 47, 47, 45, 43, 43, 43,
    43, 43, 42, 41, 41, 40, 40, 40,
    40, 35, 35, 35, 35, 33, 33, 33,
    33, 32, 32, 32, 27, 27, 26, 26,
    25, 25, 24, 24, 23, 23, 19, 19,
    19, 19, 18, 18, 17, 16, 16, 16,
    16, 16, 15, 11, 11, 11, 10, 10,
    9,  8,  7,  5,  3,  3,  2,  2,
};

BOOL _Vdec_VP6ParseFrameHeader(VDEC_VP6_INFO_T *prDecInfo)
{
    VP6_DEC_PARAM_T *prDecParam;
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    VDEC_PES_INFO_T *prPesInfo;

    INT32  fgRtn = TRUE;
    UINT32 u4Datain;
    UINT32 u4FrameHeader;

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return FALSE;
    }

    prPesInfo   = &prDecInfo->rPesInfo;
    prDecParam  = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];

    prDecParam->u4DecReadBytes = 0;
    prDecParam->u4DecReadBits  = 0;
    
    prFrmHdr->u2Buff2Offset = 0;
    prFrmHdr->fgParse_Filter_Info = FALSE;
    if(prDecInfo->rDecParamEx.fgAdobeMode==VP6_DEC_MODE_OPEN_SRC)
    {
		prFrmHdr->u2LoopFilter = 1;
    }
	else
	{
		prFrmHdr->u2LoopFilter = 0;
	}
	
    u4Datain = u4VDEC_HAL_VP6_GetBitStreamShift(8);
    prDecParam->u4DecReadBytes += 1;
    u4FrameHeader = u4Datain >> 24;
    prFrmHdr->ucFrameType = (u4FrameHeader  & 0x80)? VP6_P_FRM: VP6_I_FRM;
    
    prFrmHdr->ucDctQMask = ((u4FrameHeader & 0x7E) >> 1);       
    prFrmHdr->u2Vp56_Filter_Threshold = vp56_filter_threshold[prFrmHdr->ucDctQMask];
    prFrmHdr->u4DQuant_Dc = vp56_dc_dequant[prFrmHdr->ucDctQMask] << 2;
    prFrmHdr->u4DQuant_Ac = vp56_ac_dequant[prFrmHdr->ucDctQMask] << 2;
	if(VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_MULTI))
	{
		prFrmHdr->fgMultiStream=(u4FrameHeader & 0x1);
	}
	else
	{
		prFrmHdr->fgMultiStream=0;
	}


    if (prFrmHdr->ucFrameType == VP6_I_FRM)  //I-VOP
    {
        if (i4VP6_Parse_Intra_Header(prDecInfo) == 0)
        {
            return FALSE;
        }
    } 
    else //P-VOP
    {
        if (i4VP6_Parse_Inter_Header(prDecInfo) == 0)
        {
            return FALSE;
        }
    }
	
    if (prFrmHdr->fgParse_Filter_Info)
    {
        //Parse Advanced Profile Header.       
        prFrmHdr->fgAutoSelectPMFlag =   (u4VDEC_HAL_VP6_GetBoolCoderShift(1) )? TRUE: FALSE;        
        prDecParam->u4DecReadBits += 1;
        
        if (prFrmHdr->fgAutoSelectPMFlag)
        {
            prFrmHdr->ucFilter_Mode = 2;
            prFrmHdr->u4PredictionFilterVarThresh = (u4VDEC_HAL_VP6_GetBoolCoderShift(5) );
            prFrmHdr->u4PredictionFilterMvSizeThresh = (u4VDEC_HAL_VP6_GetBoolCoderShift(3) );
            prDecParam->u4DecReadBits += 8;

            prFrmHdr->u4Sample_Variance_Threshold = (prFrmHdr->u4PredictionFilterVarThresh << prFrmHdr->ucVrt_Shift);
            prFrmHdr->u4Max_Vector_Length = 2 << prFrmHdr->u4PredictionFilterMvSizeThresh;

            //HW related variable
            if (prFrmHdr->u4Max_Vector_Length)
            {
                prFrmHdr->u4Mv_Thr_En = 1; //enable              
            }
            else
            {
                prFrmHdr->u4Mv_Thr_En = 0;
            }

            if (prFrmHdr->u4Sample_Variance_Threshold)
            {
                prFrmHdr->u4Var_Thr_En = 1;             
            }
            else
            { 
                prFrmHdr->u4Var_Thr_En = 0;
            } 
        }
        else
        {
            prFrmHdr->fgBiCubicOrBiLinearFlag = (u4VDEC_HAL_VP6_GetBoolCoderShift(1) )? TRUE: FALSE;
            prDecParam->u4DecReadBits += 1;
            if (prFrmHdr->fgBiCubicOrBiLinearFlag) 
            {
                prFrmHdr->ucFilter_Mode = 1;
                prFrmHdr->u4BilinearFilter = 0;
            }
            else
            {
                prFrmHdr->ucFilter_Mode = 0;
                prFrmHdr->u4BilinearFilter = 1;
            }
            prFrmHdr->u4Var_Thr_En = 0;
        }
        
        if (prFrmHdr->ucVp3VerNo > 7)
        {
            prFrmHdr->u4Filter_Selection = (u4VDEC_HAL_VP6_GetBoolCoderShift(4) );
            prDecParam->u4DecReadBits += 4;
        }
        else
        {
            prFrmHdr->u4Filter_Selection = 16;
        }
    }

    prFrmHdr->fgUseHuffman = (u4VDEC_HAL_VP6_GetBoolCoderShift(1) )? TRUE: FALSE;       // //// eason : marker2
    prDecParam->u4DecReadBits += 1;

	if(VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_MULTI))
	{
		if (prFrmHdr->u2Buff2Offset) 
		{
			_VDEC_SetVP6BsInfo(1, prPesInfo->u4VldReadPtr + prFrmHdr->u2Buff2Offset, prDecParam->u4WPtr, prPesInfo->u4FifoStart, prPesInfo->u4FifoEnd, prFrmHdr);
			u4VDEC_HAL_VP6_InitBoolCoder(1);  //vp56_init_range_decoder(&s->cc, buf+coeff_offset, buf_size-coeff_offset);		 
			_Vdec_VP6HwSetByteCount(1, prFrmHdr->u4FrameSize - prFrmHdr->u2Buff2Offset);
			//s->ccp = &s->cc;
		}
		else 
		{
			//Do Nothing.
			//s->ccp = &s->c;
		}
	}
	else
	{
		prFrmHdr->fgUseHuffman = FALSE;
	}
    return fgRtn;
}

