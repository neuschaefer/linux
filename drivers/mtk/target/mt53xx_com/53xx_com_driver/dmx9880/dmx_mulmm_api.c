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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_multimm_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_multimm_api.c
 *  Demux multi-media driver - multiple instance
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_os.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DMX_MULTIPLE_INSTANCE_MAXNUM            (12)
#define DMX_MULTIPLE_NULL_INSTANCE              (0xFF)
#define DMX_MULTIPLE_NULL_PIDX                  (0xFF)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    DMX_CONN_TYPE_T eConnType;
    DMX_INPUT_TYPE_T eInputType;
    DMX_DECODER_CALLBACKS_T rDecoderCallback;
    DMX_VIDEO_TYPE_T eVideoType;
    DMX_TSFMT_T eTSFmt;
    UINT8 u1TsIdx;
    BOOL fgUsed;
} DMX_MUL_INSTINFO_T;

typedef struct
{
    BOOL fgUsed;
    UINT8 u1InstId;
} DMX_MUL_PIDINFO_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_MUL_INSTINFO_T _arDmxMulInstInfo[DMX_MULTIPLE_INSTANCE_MAXNUM];
static DMX_MUL_PIDINFO_T _arDmxMulPidInfo[DMX_NUM_PID_INDEX];
static UINT8 _arDmxMulInstSbt[DMX_MULTIPLE_INSTANCE_MAXNUM];

static BOOL _fgEnableMulInst = TRUE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _DMX_MUL_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_Init(void)
{
    UINT8 i;

    x_memset((void*)&_arDmxMulInstInfo, 0, sizeof(_arDmxMulInstInfo));
    _fgEnableMulInst = FALSE;

    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        _arDmxMulPidInfo[i].fgUsed = FALSE;
        _arDmxMulPidInfo[i].u1InstId = DMX_NULL_INSTINDEX;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_EnableMultipleInst
 */
//-----------------------------------------------------------------------------
void _DMX_MUL_EnableMultipleInst(BOOL fgEnable)
{
    _fgEnableMulInst = fgEnable;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_EnableMultipleInst
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_GetMultipleInstEnabled(void)
{
    return _fgEnableMulInst;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetAvailableInst
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_T eConnType)
{
    UINT8 i;

    if(eConnType != DMX_CONN_TYPE_NONE)
    {
        for(i = 0; i < DMX_MULTIPLE_INSTANCE_MAXNUM; i++)
        {
            if((_arDmxMulInstInfo[i].fgUsed) && (_arDmxMulInstInfo[i].eConnType == eConnType))
            {
                break;
            }
        }

        if(i < DMX_MULTIPLE_INSTANCE_MAXNUM)
        {
            // allocated instance
            return i;
        }
    }

    // try to find a available instance
    for(i = 0; i < DMX_MULTIPLE_INSTANCE_MAXNUM; i++)
    {
        if(!_arDmxMulInstInfo[i].fgUsed)
        {
            break;
        }
    }

    if(i >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_MULTIPLE_NULL_INSTANCE;
    }

    _arDmxMulInstInfo[i].fgUsed = TRUE;
    _arDmxMulInstInfo[i].eConnType = eConnType;

    return i;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_FreeInst
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_FreeInst(UINT8 u1Inst)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    _arDmxMulInstInfo[u1Inst].fgUsed = FALSE;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_GetPidxInstId
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetPidxInstId(UINT8 u1Pidx)
{
    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_NULL_INSTINDEX;
    }

    if(!_fgEnableMulInst)
    {
        return DMX_NULL_INSTINDEX;
    }

    if(!_arDmxMulPidInfo[u1Pidx].fgUsed)
    {
        return DMX_NULL_INSTINDEX;
    }

    return _arDmxMulPidInfo[u1Pidx].u1InstId;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetInstInputType
*/
//-----------------------------------------------------------------------------
DMX_INPUT_TYPE_T _DMX_MUL_GetInstInputType(UINT8 u1Inst)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_IN_NONE;
    }

    return _arDmxMulInstInfo[u1Inst].eInputType;
}



//-----------------------------------------------------------------------------
/** _DMX_MUL_SetInstType
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetInstType(UINT8 u1Inst, DMX_INPUT_TYPE_T eInputType)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    _arDmxMulInstInfo[u1Inst].eInputType = eInputType;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetAvailablePidx
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetAvailablePidx(UINT8 u1Inst)
{
    UINT8 i;

    LOG(5, "%s\n", __FUNCTION__);

    for(i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if(!_arDmxMulPidInfo[i].fgUsed)
        {
            break;
        }
    }

    if(i >= DMX_NUM_PID_INDEX)
    {
        return DMX_MULTIPLE_NULL_PIDX;
    }

    _arDmxMulPidInfo[i].fgUsed = TRUE;
    _arDmxMulPidInfo[i].u1InstId = u1Inst;

    return i;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetSubtitlePidx
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetSubtitlePidx(UINT8 u1Inst, UINT8 u1Pidx)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if(_arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_MM)
    {
        return FALSE;
    }

    _arDmxMulInstSbt[u1Inst] = u1Pidx;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetSubtitlePidx
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetSubtitlePidx(UINT8 u1Inst)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_MULTIPLE_NULL_PIDX;
    }

    if(_arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_MM)
    {
        return DMX_MULTIPLE_NULL_PIDX;
    }

    if(_arDmxMulInstSbt[u1Inst] >= DMX_NUM_PID_INDEX)
    {
        return DMX_MULTIPLE_NULL_PIDX;
    }

    return _arDmxMulInstSbt[u1Inst];
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetPidxInstance
 *  Temporary
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetPidxInstance(UINT8 u1Inst, UINT8 u1Pidx)
{
    if((u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM) ||
       (u1Pidx >= DMX_NUM_PID_INDEX))
    {
        return FALSE;
    }

    _arDmxMulPidInfo[u1Pidx].fgUsed = TRUE;
    _arDmxMulPidInfo[u1Pidx].u1InstId = u1Inst;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_FreePidx
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_FreePidx(UINT8 u1Pidx)
{
    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _arDmxMulPidInfo[u1Pidx].fgUsed = FALSE;
    _arDmxMulPidInfo[u1Pidx].u1InstId = DMX_NULL_INSTINDEX;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_MoveData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_MoveData(UINT8 u1Inst, DMX_MM_DATA_T *prData)
{
    BOOL fgRet = FALSE;
    DMX_VIDEO_TYPE_T eVideoType;

    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    eVideoType = _arDmxMulInstInfo[u1Inst].eVideoType;

    switch(_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:        
        prData->u1Idx = _DMX_MUL_GetTsIdx(u1Inst);

        if(!_DMX_SetVideoType(prData->u1Idx, eVideoType))
        {
            return FALSE;
        }
    
	      if(prData->u1Idx == 0)
        {
	       _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER0,0, TRUE, TRUE);//avoid the DBM Bypass after Play MM.
        }
		else if(prData->u1Idx == 1)
		{
		 _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1,0, TRUE, TRUE);//avoid the DBM Bypass after Play MM.
		}
		else
	    {
		 _DMX_DDI_SetPortEx(DMX_DDI_PORT_NO_FRAMER,0, TRUE, TRUE);//avoid the DBM Bypass after Play MM.
		}
		
        if(_DMX_SetTSFilePlayPath(prData->u1Idx, _arDmxMulInstInfo[u1Inst].eTSFmt))
        {
            fgRet = _DMX_DDI_TSMoveData(prData);
        }
        break;

    case DMX_IN_PLAYBACK_PS:

        _arDmxMulInstInfo[u1Inst].u1TsIdx = 1;
        
        if(!_DMX_SetVideoType(1, eVideoType))
        {
            return FALSE;
        }

        prData->u1Idx = u1Inst;
        if(!_DMX_SetFramerPktSize(1, 188))
        {
            return FALSE;
        }
        
        if(!_DMX_DDI_SetPacketSize(1, 188))
        {
            return FALSE;
        }        
        fgRet = _DMX_PS_CPU_MoveData(prData);
        break;

    case DMX_IN_PLAYBACK_MM:

        _arDmxMulInstInfo[u1Inst].u1TsIdx = 1;
        
        if(!_DMX_SetVideoType(1, eVideoType))
        {
            return FALSE;
        }

        _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, prData->u1Idx, TRUE, FALSE);
        
        if(!_DMX_SetFramerPktSize(1, 188))
        {
            return FALSE;
        }
        
        if(!_DMX_DDI_SetPacketSize(1, 188))
        {
            return FALSE;
        }        
        fgRet = _DMX_MM_MoveData(prData->u1Idx, prData, 5000);
        break;

    case DMX_IN_PLAYBACK_ES:
        _arDmxMulInstInfo[u1Inst].u1TsIdx = 1;
        
        if(!_DMX_SetVideoType(1, eVideoType))
        {
            return FALSE;
        }

        _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, prData->u1Idx, TRUE, FALSE);
        
        if(!_DMX_SetFramerPktSize(1, 188))
        {
            return FALSE;
        }
        
        if(!_DMX_DDI_SetPacketSize(1, 188))
        {
            return FALSE;
        }        
        fgRet = _DMX_ES_MoveData(prData->u1Idx, prData);
        break;

    default:
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_RequestReset(UINT8 u1Inst)
{
    BOOL fgRet = TRUE;
    UINT8 i;

    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    switch(_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
        fgRet = _DMX_DDI_TSRequestReset(u1Inst);
        break;

    case DMX_IN_PLAYBACK_PS:
        fgRet = _DMX_PS_CPU_Reset(u1Inst);
        break;

    case DMX_IN_PLAYBACK_MM:
        fgRet = TRUE;
        break;

    case DMX_IN_PLAYBACK_ES:
        for(i=0; i<DMX_NUM_PID_INDEX ;i++)
        {
            if((_arDmxMulPidInfo[i].u1InstId == u1Inst) && _arDmxMulPidInfo[i].fgUsed)
            {
                fgRet = _DMX_ES_Reset(i);
            }
        }
        break;

    default:
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetEmptySize
*/
//-----------------------------------------------------------------------------
UINT32 _DMX_MUL_GetEmptySize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx)
{
    UINT32 u4EmptySize = 0;

    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    switch(_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
    case DMX_IN_PLAYBACK_PS:
        u4EmptySize = _DMX_GetMinFreeBufferSize(u1Inst, eType);
        break;

    case DMX_IN_PLAYBACK_MM:
    case DMX_IN_PLAYBACK_ES:
        if(u1Pidx >= DMX_NUM_PID_INDEX)
        {
            return 0;
        }
        if(_arDmxMulPidInfo[u1Pidx].fgUsed && (_arDmxMulPidInfo[u1Pidx].u1InstId == u1Inst))
        {
            u4EmptySize = _DMX_MM_GetBufEmptySize(u1Pidx);
        }
        break;

    default:
        break;
    }

    return u4EmptySize;
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeBufferSize
 *  Get free buffer: dram buffer
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_MUL_GetMinFreeDRAMBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx)
{
    UINT32 u4EmptySize = 0;

    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return 0;
    }

    switch(_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
    case DMX_IN_PLAYBACK_PS:
        u4EmptySize = _DMX_GetMinFreeDRAMBufferSize(u1Inst, eType);
        break;

    case DMX_IN_PLAYBACK_MM:
    case DMX_IN_PLAYBACK_ES:
        if(u1Pidx >= DMX_NUM_PID_INDEX)
        {
            return 0;
        }
        if(_arDmxMulPidInfo[u1Pidx].fgUsed && (_arDmxMulPidInfo[u1Pidx].u1InstId == u1Inst))
        {
            u4EmptySize = _DMX_MM_GetBufEmptySize(u1Pidx);
        }
        break;

    default:
        break;
    }

    return u4EmptySize;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetDecoderCallbacks
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetDecoderCallbacks(UINT8 u1Inst, const DMX_DECODER_CALLBACKS_T* prCallbacks)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    _arDmxMulInstInfo[u1Inst].rDecoderCallback = *prCallbacks;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetInstDecoderCallback
 */
//-----------------------------------------------------------------------------
DMX_DECODER_CALLBACKS_T* _DMX_MUL_GetInstDecoderCallback(UINT8 u1Inst)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return NULL;
    }

    return &(_arDmxMulInstInfo[u1Inst].rDecoderCallback);
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetPidxDecoderCallback
 */
//-----------------------------------------------------------------------------
DMX_DECODER_CALLBACKS_T* _DMX_MUL_GetPidxDecoderCallback(UINT8 u1Pidx)
{
    UINT8 u1Inst;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return NULL;
    }

    u1Inst = _arDmxMulPidInfo[u1Pidx].u1InstId;

    if(!_arDmxMulPidInfo[u1Pidx].fgUsed || (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM))
    {
        return NULL;
    }

    return &(_arDmxMulInstInfo[u1Inst].rDecoderCallback);
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetVideoType
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetVideoType(UINT8 u1Inst, DMX_VIDEO_TYPE_T eVideoType)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    _arDmxMulInstInfo[u1Inst].eVideoType = eVideoType;

    // For TS, call HW function
/*
    if(!_DMX_SetVideoType(eSource, eVideoType))
    {
        return FALSE;
    }
*/

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetVideoTypec
 */
//-----------------------------------------------------------------------------
DMX_VIDEO_TYPE_T _DMX_MUL_GetVideoType(UINT8 u1Inst)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_VIDEO_NONE;
    }

    return _arDmxMulInstInfo[u1Inst].eVideoType;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetTSMode
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetTSMode(UINT8 u1Inst, DMX_TSFMT_T eFmt)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    if(eFmt == DMX_TSFMT_TIMESHIFT)
    {
        _arDmxMulInstInfo[u1Inst].u1TsIdx = 2;
    }
    else
    {
        _arDmxMulInstInfo[u1Inst].u1TsIdx = 1;
    }

    _arDmxMulInstInfo[u1Inst].eTSFmt = eFmt;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetTsIdx
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetTsIdx(UINT8 u1Inst)
{
    if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_VIDEO_NONE;
    }

    return _arDmxMulInstInfo[u1Inst].u1TsIdx;
}

