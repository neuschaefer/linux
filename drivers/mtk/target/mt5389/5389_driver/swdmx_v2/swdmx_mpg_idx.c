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
/*-----------------------------------------------------------------------------
 *
 * $Author: jifeng.fan $
 * $Date: 2013/01/12 $
 * $RCSfile: swdmx_mpg_idx.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "swdmx_mpg.h"
#include "swdmx_mpg_idx.h"
#include "x_util.h"
#include "swdmx_debug.h"


#define SWDMX_MPG_IDX_PERIOD 90000
#define SWDMX_MPG_IDX_TOLERATE 2


// move the struct to swdmx_drvif.h
#if 0
typedef struct
{
    UINT32 u4TotalTime;
    UINT32 u4NewTotalTime;
    UINT32 u4BasePosition;
    UINT32 u4BaseTime;
}SWDMX_MPG_INDEX_T;
#endif 
// static variables needed move to SWMDX_MPG_INFO_T

#if 0
static UINT32 _u4SwdmxMpgIdxType = SWDMX_MPG_IDX_TYPE_NONE;
static UINT32 _u4SwdmxIdxSize = 0;
//static PVR_TICK_INDEX_T* _prTickIdx = NULL;
static SWDMX_MPG_TICK_DETAIL_T* _prSwdmxMpgTickTime = NULL;
static SWDMX_MPG_TICK_DETAIL_T* _prSwdmxMpgTickPosition = NULL;
static SWDMX_MPG_INDEX_T _rSwdmxMpgIdx;
#endif
/**
  For file only.
*/
BOOL _SWDMX_MpgIdxOpen(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4TotalTime, UINT64 u8FileSize)
{
    UINT32 i;
    UINT32 u4Tmp;
    UINT32 u4Tmp2;
    UINT64 u8Tmp;
    UINT64 u8Remainder;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    x_memset((VOID*)&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx, 
    	                 0, 
    	                 sizeof(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx));

    if((u4TotalTime == 0) || (u8FileSize == 0))
    {
        LOG(3, "_SWDMX_MpgIdxOpen u4TotalTime or u8FileSize 0\n");
        return FALSE;
    }

    if(u4Type != SWDMX_MPG_IDX_TYPE_NONE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4TotalTime = u4TotalTime;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4NewTotalTime = u4TotalTime;
        // Current Max: 0xFFFFFFFF / 90000 = 47722
        u4Tmp = (((u4TotalTime + SWDMX_MPG_IDX_PERIOD) - 1) / SWDMX_MPG_IDX_PERIOD);
        if(u4Tmp == 0)
        {
            LOG(3, "_SWDMX_MpgIdxOpen 1 u4BasePosition 0\n");
            return FALSE;
        } 
        u4Tmp2 = (u4Tmp * SWDMX_MPG_IDX_TOLERATE);
        u8Tmp = u8Div6432(u8FileSize, (UINT64)(u4Tmp), &u8Remainder);
        if(u8Tmp == 0)
        {
            LOG(3, "_SWDMX_MpgIdxOpen 2 u4BasePosition 0\n");
            return FALSE;
        } 
        
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize = u4Tmp2;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BasePosition = (UINT32)u8Tmp;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BaseTime = SWDMX_MPG_IDX_PERIOD;

        /*
        if(_prTickIdx == NULL)
        {
            _prTickIdx = x_mem_alloc(_u4SwdmxIdxSize * sizeof(PVR_TICK_INDEX_T));
        }
        else
        {
            _prTickIdx = x_mem_realloc((void*)_prTickIdx, _u4SwdmxIdxSize * sizeof(PVR_TICK_INDEX_T));
        }
        if(_prTickIdx == NULL)
        {
            LOG(3, "_SWDMX_MpgIdxOpen _prTickIdx null\n");
            return FALSE;
        }
        */
        if(u4Type & SWDMX_MPG_IDX_TYPE_TIME)
        {
            if(prSwdmxInst->prSwdmxMpgTickTime == NULL)
            {
                return FALSE;
                prSwdmxInst->prSwdmxMpgTickTime = 
                	 x_mem_alloc(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize * sizeof(SWDMX_MPG_TICK_DETAIL_T));
            }
            else
            {
                return FALSE;
                prSwdmxInst->prSwdmxMpgTickTime = 
                	  x_mem_realloc((void*)prSwdmxInst->prSwdmxMpgTickTime, 
                	                             prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize * sizeof(SWDMX_MPG_TICK_DETAIL_T));
            }
            if(prSwdmxInst->prSwdmxMpgTickTime == NULL)
            {
                LOG(3, "_SWDMX_MpgIdxOpen _prSwdmxMpgTickTime null\n");
                return FALSE;
            }
        }
        if(u4Type & SWDMX_MPG_IDX_TYPE_POSITION)
        {
            if(prSwdmxInst->prSwdmxMpgTickPosition == NULL)
            {
                prSwdmxInst->prSwdmxMpgTickPosition = 
                	x_mem_alloc(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize * sizeof(SWDMX_MPG_TICK_DETAIL_T));
            }
            else
            {
               prSwdmxInst->prSwdmxMpgTickPosition =
               	 x_mem_realloc((void*)prSwdmxInst->prSwdmxMpgTickPosition, 
               	                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize * sizeof(SWDMX_MPG_TICK_DETAIL_T));
            }
            if(prSwdmxInst->prSwdmxMpgTickPosition == NULL)
            {
                LOG(3, "_SWDMX_MpgIdxOpen _prSwdmxMpgTickPosition null\n");
                return FALSE;
            }
        }
        else
        {
            LOG(1, "_SWDMX_MpgIdxOpen _prSwdmxMpgTickPosition null\n");
            return FALSE;
        }
        
        for(i=0;i<prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize;i++)
        {
            /*
            _prTickIdx[i].ui4_tick_num = (i*SWDMX_MPG_IDX_PERIOD);
            _prTickIdx[i].ui4_flags = (1);
            */
            
            if(u4Type & SWDMX_MPG_IDX_TYPE_TIME)
            {
                //_prTickIdx[i].u.pt_time_tick = (&_prSwdmxMpgTickTime[i]);
                prSwdmxInst->prSwdmxMpgTickTime[i].fg_valid = FALSE;
                prSwdmxInst->prSwdmxMpgTickTime[i].ui4_tick_num = (i*SWDMX_MPG_IDX_PERIOD);
                prSwdmxInst->prSwdmxMpgTickTime[i].ui4_entries = 0;
                prSwdmxInst->prSwdmxMpgTickTime[i].ui8_lba = 0;
            }
            if(u4Type & SWDMX_MPG_IDX_TYPE_POSITION)
            {
                //_prTickIdx[i].u.pt_pos_tick = (&_prSwdmxMpgTickPosition[i]);
                prSwdmxInst->prSwdmxMpgTickPosition[i].fg_valid = FALSE;
                prSwdmxInst->prSwdmxMpgTickPosition[i].ui4_tick_num = 0;
                prSwdmxInst->prSwdmxMpgTickPosition[i].ui4_entries = 0;
                prSwdmxInst->prSwdmxMpgTickPosition[i].ui8_lba = (i*u8Tmp);
            }
        }
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType = u4Type;
    
    return TRUE;
}

/**
  For file only.
*/
BOOL _SWDMX_MpgIdxClose(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType = SWDMX_MPG_IDX_TYPE_NONE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize = 0;
    return TRUE;
}

/**
  For file only.
*/
BOOL _SWDMX_MpgIdxUpdate(UINT8 u1SrcId, UINT32 u4Time, UINT64 u8IPos)//, UINT64 u8PrevIPos)
{
    UINT32 index;
    UINT64 u8Remainder;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType == SWDMX_MPG_IDX_TYPE_NONE)
    {
        return FALSE;
    }
    
    #if 1
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType & SWDMX_MPG_IDX_TYPE_TIME) &&
       (prSwdmxInst->prSwdmxMpgTickTime != NULL) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BaseTime != 0))
    {
        index = (u4Time / prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BaseTime);
        if(index < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize)
        {
            if(!prSwdmxInst->prSwdmxMpgTickTime[index].fg_valid)
            {
                prSwdmxInst->prSwdmxMpgTickTime[index].ui8_lba = u8IPos;
                prSwdmxInst->prSwdmxMpgTickTime[index].ui4_tick_num = u4Time;
                prSwdmxInst->prSwdmxMpgTickTime[index].fg_valid = TRUE;
            }
            else
            {
                if(u8IPos < prSwdmxInst->prSwdmxMpgTickTime[index].ui8_lba)
                {
                    prSwdmxInst->prSwdmxMpgTickTime[index].ui8_lba = u8IPos;
                }
                if(u4Time < prSwdmxInst->prSwdmxMpgTickTime[index].ui4_tick_num)
                {
                    prSwdmxInst->prSwdmxMpgTickTime[index].ui4_tick_num = u4Time;
                }
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgIdxUpdate %d > \n");
        }
    }
    #endif
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType & SWDMX_MPG_IDX_TYPE_POSITION) &&
       (prSwdmxInst->prSwdmxMpgTickPosition != NULL) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BasePosition != 0))
    {
        index = (UINT32)u8Div6432(u8IPos, 
        	            (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BasePosition, &u8Remainder);
        if(index < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize)
        {
            if(!prSwdmxInst->prSwdmxMpgTickPosition[index].fg_valid)
            {
                prSwdmxInst->prSwdmxMpgTickPosition[index].ui8_lba = u8IPos;
                prSwdmxInst->prSwdmxMpgTickPosition[index].ui4_tick_num = u4Time;
                prSwdmxInst->prSwdmxMpgTickPosition[index].fg_valid = TRUE;
            }
            else
            {
                if(u8IPos < prSwdmxInst->prSwdmxMpgTickPosition[index].ui8_lba)
                {
                    prSwdmxInst->prSwdmxMpgTickPosition[index].ui8_lba = u8IPos;
                }
                if(u4Time < prSwdmxInst->prSwdmxMpgTickPosition[index].ui4_tick_num)
                {
                    prSwdmxInst->prSwdmxMpgTickPosition[index].ui4_tick_num = u4Time;
                }
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgIdxUpdate %d > \n");
        }
    }
    else
    {
        LOG(3, "_SWDMX_MpgIdxUpdate _u4SwdmxMpgIdxType %d\n", 
        	       prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType);
        return FALSE;
    }
    
    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgIdxGetPosByPos(UINT8 u1SrcId, BOOL fgBackAlign, UINT64 u8Pos, UINT64 *pu8RetPos)
{
    UINT32 index;
    UINT64 u8Remainder;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if(pu8RetPos == NULL)
    {
        LOG(3, "_SWDMX_MpgIdxGetPosByPos pu8RetPos null\n");
        return FALSE;
    }
    
    *pu8RetPos = u8Pos;
    if(!(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType & SWDMX_MPG_IDX_TYPE_POSITION))
    {
        LOG(3, "_SWDMX_MpgIdxGetPosByPos _u4SwdmxMpgIdxType %d\n", 
        	      prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType);
        return FALSE;
    }
    else //if(_u4SwdmxMpgIdxType == SWDMX_MPG_IDX_TYPE_POSITION)
    {
        if(//(_u4SwdmxMpgIdxType |= SWDMX_MPG_IDX_TYPE_POSITION) &&
           (prSwdmxInst->prSwdmxMpgTickPosition != NULL) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BasePosition != 0))
        {
            index = (UINT32)u8Div6432(u8Pos, 
            	            (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.rSwdmxMpgIdx.u4BasePosition, 
            	            &u8Remainder);
            if(fgBackAlign)
            {
                if((index ) < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize)
                {
                    if(prSwdmxInst->prSwdmxMpgTickPosition[index].fg_valid)
                    {
                        if(u8Pos < prSwdmxInst->prSwdmxMpgTickPosition[index].ui8_lba)
                        {
                            if(index > 0)
                            {
                                if((prSwdmxInst->prSwdmxMpgTickPosition[index-1].fg_valid) &&
                                   (u8Pos > prSwdmxInst->prSwdmxMpgTickPosition[index-1].ui8_lba))
                                {
                                    *pu8RetPos = prSwdmxInst->prSwdmxMpgTickPosition[index-1].ui8_lba;
                                }
                                /*
                                else
                                {
                                    *pu8RetPos = u8Pos;
                                }
                                */
                            }
                            /*
                            else
                            {
                                *pu8RetPos = u8Pos;
                            }
                            */
                        }
                        else
                        {
                            *pu8RetPos = prSwdmxInst->prSwdmxMpgTickPosition[index].ui8_lba;
                        }
                    }
                    /*
                    else
                    {
                        *pu8RetPos = u8Pos;
                    }
                    */
                }
            }
            else
            {
                if((index + 1) < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxIdxSize)
                {
                    if(prSwdmxInst->prSwdmxMpgTickPosition[index+1].fg_valid)
                    {
                        *pu8RetPos = prSwdmxInst->prSwdmxMpgTickPosition[index+1].ui8_lba;
                    }
                    /*
                    else
                    {
                        *pu8RetPos = u8Pos;
                    }
                    */
                }
                /*
                else
                {
                    *pu8RetPos = u8Pos;
                }
                */
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgIdxGetPosByPos _u4SwdmxMpgIdxType %d\n", 
            	       prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rIdx.u4SwdmxMpgIdxType);
            return FALSE;
        } 
    }
    
    return TRUE;
}

/**
*/
#if 0
BOOL _SWDMX_MpgIdxGetPosByTime(BOOL fgBackAlign, UINT32 u8Time, UINT64 *pu8RetPos)
{
    UINT32 index;
    UINT64 u8Remainder;
    
    *pu8RetPos = 0;
    
    if(!(_u4SwdmxMpgIdxType | SWDMX_MPG_IDX_TYPE_TIME))
    {
        LOG(3, "_SWDMX_MpgIdxGetPosByPos _u4SwdmxMpgIdxType %d\n", _u4SwdmxMpgIdxType);
        return FALSE;
    }
    else //if(_u4SwdmxMpgIdxType == SWDMX_MPG_IDX_TYPE_TIME)
    {
        index = u8Div6432(u8Time, (UINT64)_rSwdmxMpgIdx.u4BaseTime, &u8Remainder);
        if(fgBackAlign)
        {
            if(_prSwdmxMpgTickTime[index].ui8_lba > 0)
            {
                *pu8RetPos = _prSwdmxMpgTickTime[index].ui8_lba;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            if(_prSwdmxMpgTickTime[index+1].ui8_lba > 0)
            {
                *pu8RetPos = _prSwdmxMpgTickTime[index+1].ui8_lba;
            }
            else
            {
                return FALSE;
            }
        }
    }
    
    return TRUE;
}
#endif


