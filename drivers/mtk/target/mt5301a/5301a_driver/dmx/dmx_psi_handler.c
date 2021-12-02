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
 * $RCSfile: dmx_psi_handler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_psi_handler.c
 *  Demux driver - psi handler module
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
#include "dmx_if.h"
#include "dmx_ide.h"

LINT_EXT_HEADER_BEGIN

#include "mpv_drvif.h"
#include "aud_drvif.h"
#include "drv_common.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Demux queue and thread paramenters
//
#define DMX_PSI_QUEUE_NAME                  "DMXPSI"
#define DMX_PSI_QUEUE_SIZE                  32

#define SECTION_COMPARE_LEN                 46

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Picture queue for MPEG decoder
static HANDLE_T _hDmxPSIQueue = NULL_HANDLE;

/// Picture thread
static HANDLE_T _hDmxPSIThread = NULL_HANDLE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifndef CC_MINI_DRIVER
//-----------------------------------------------------------------------------
/** _DmxSendToIde
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxPSIParserSendToIde(UINT8 u1PidIndex, UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 u4Rp, u4Wp;
    PID_STRUCT_T* prPidStruct;

    prPidStruct = _DMX_GetPidStruct(u1PidIndex);

    _DMX_Lock();
    u4Rp = u4Addr;
    u4Wp = u4Addr + u4Size;
    if(u4Wp > prPidStruct->u4BufEnd)
    {
        u4Wp -= prPidStruct->u4BufLen;
    }
    _DMX_Unlock();

    VERIFY(_DMX_IDE_SendPacket(u1PidIndex, prPidStruct->u4BufStart,
        prPidStruct->u4BufEnd + 1, u4Rp, u4Wp));

    _DMX_Lock();
    prPidStruct->u4Rp = u4Wp;
    _DMX_Unlock();
}


#endif  // CC_MINI_DRIVER


//-----------------------------------------------------------------------------
/** _DmxCompare
 */
//-----------------------------------------------------------------------------
static BOOL _DmxCompare(UINT8 u1Data1, UINT8 u1Data2, UINT8 u1Mask)
{
    if((u1Data1 & u1Mask) == (u1Data2 & u1Mask))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
static BOOL _DmxSWFilter_Short(const FILTER_STRUCT_T *prFilter,
                               const UINT8 *pu1CompareBuf, UINT8 u1CompareLen)
{
    UINT32 i;
    UINT8 u1Data, u1Offset;

    if((prFilter == NULL) || (pu1CompareBuf == NULL))
    {
        return FALSE;
    }

    u1Offset = prFilter->arBank[0].u1Offset;

    for(i=0; i<7; i++)
    {
        if((i + u1Offset) >= u1CompareLen)
        {
            return TRUE;
        }
        u1Data = *(pu1CompareBuf + i + u1Offset);
        if(!_DmxCompare(u1Data, prFilter->arBank[0].au1Data[i+1],
            prFilter->arBank[0].au1Mask[i+1]))
        {
            return FALSE;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
static BOOL _DmxSWFilter_Long(const FILTER_STRUCT_T *prFilter,
                              const UINT8 *pu1CompareBuf, UINT8 u1CompareLen)
{
    UINT32 i;
    UINT8 u1Data, u1Offset, u1Index;

    if((prFilter == NULL) || (pu1CompareBuf == NULL))
    {
        return FALSE;
    }

    // Bank0
    u1Offset = prFilter->arBank[0].u1Offset;
    for(i=0; i<7; i++)
    {
        u1Index = i + u1Offset;
        if(u1Index >= u1CompareLen)
        {
            return TRUE;   // Out of range
        }
        u1Data = *(pu1CompareBuf + u1Index);
        if(!_DmxCompare(u1Data, prFilter->arBank[0].au1Data[i+1],
            prFilter->arBank[0].au1Mask[i+1]))
        {
            return FALSE;
        }
    }

    // Bank1
    if(prFilter->arBank[1].fgEnable)
    {
        u1Offset = prFilter->arBank[1].u1Offset;
        for(i=0; i<8; i++)
        {
            u1Index = i + u1Offset + 7;
            if(u1Index >= u1CompareLen)
            {
                return TRUE;   // Out of range
            }
            u1Data = *(pu1CompareBuf + u1Index);
            if(!_DmxCompare(u1Data, prFilter->arBank[1].au1Data[i],
                prFilter->arBank[1].au1Mask[i]))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxSWFilter_PosNeg(const FILTER_STRUCT_T *prFilter,
                                const UINT8 *pu1CompareBuf, UINT8 u1CompareLen)
{
    UINT8 i;
    UINT8 u1Data, u1Mask, u1Offset, u1Index;
    BOOL fgMask;

    if((prFilter == NULL) || (pu1CompareBuf == NULL))
    {
        return FALSE;
    }

    // Bank0
    u1Offset = prFilter->arBank[0].u1Offset;
    for(i=0; i<7; i++)
    {
        u1Index = i + u1Offset;
        if(u1Index >= u1CompareLen)
        {
            return TRUE;   // Out of range
        }
        u1Data = *(pu1CompareBuf + u1Index);
        if(!_DmxCompare(u1Data, prFilter->arBank[0].au1Data[i+1],
            prFilter->arBank[0].au1Mask[i+1]))
        {
            return FALSE;   // return immediately
        }
    }

    // Bank1, negative
    if(prFilter->arBank[1].fgEnable)
    {
        u1Data = *pu1CompareBuf;
        u1Mask = prFilter->arBank[1].au1Mask[0];
        if(u1Mask && _DmxCompare(u1Data, prFilter->arBank[1].au1Data[0], u1Mask))
        {
            return FALSE;       // return immediately
        }

        if(u1Offset >= u1CompareLen)
        {
            return TRUE;
        }

        u1Offset = prFilter->arBank[1].u1Offset;
        fgMask = FALSE;
        for(i=0; i<7; i++)
        {
            u1Index = i + u1Offset;
            if(u1Index >= u1CompareLen)
            {
                break;
            }

            if(prFilter->arBank[1].au1Mask[i+1])
            {
                fgMask = TRUE;
                break;
            }
        }
        if(!fgMask)
        {
            return TRUE;    // No any mask
        }

        for(i=0; i<7; i++)
        {
            u1Index = i + u1Offset;
            if(u1Index >= u1CompareLen)
            {
                break;
            }
            u1Data = *(pu1CompareBuf + u1Index);
            u1Mask = prFilter->arBank[1].au1Mask[i+1];
            if(u1Mask && !_DmxCompare(u1Data, prFilter->arBank[1].au1Data[i+1], u1Mask))
            {
                return TRUE;
            }
        }

        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxSWFilter_14_2(const FILTER_STRUCT_T *prFilter,
                              const UINT8 *pu1CompareBuf, UINT8 u1CompareLen)
{
    UINT8 i;
    UINT8 u1Data, u1Offset, u1Index, u1Mask;
    BOOL fgMask;

    if((prFilter == NULL) || (pu1CompareBuf == NULL))
    {
        return FALSE;
    }

    // Bank0
    u1Offset = prFilter->arBank[0].u1Offset;
    for(i=0; i<7; i++)
    {
        u1Index = i + u1Offset;
        if(u1Index >= u1CompareLen)
        {
            return TRUE;   // Out of range
        }
        u1Data = *(pu1CompareBuf + u1Index);
        if(!_DmxCompare(u1Data, prFilter->arBank[0].au1Data[i+1],
            prFilter->arBank[0].au1Mask[i+1]))
        {
            return FALSE;
        }
    }

    // Bank1
    if(prFilter->arBank[1].fgEnable)
    {
        u1Offset = u1Offset + 7;
        for(i=0; i<6; i++)
        {
            u1Index = u1Offset + i;
            if(u1Index >= u1CompareLen)
            {
                //return TRUE;   // Out of range
                break;
            }
            u1Data = *(pu1CompareBuf + u1Index);
            if(!_DmxCompare(u1Data, prFilter->arBank[1].au1Data[i],
                prFilter->arBank[1].au1Mask[i]))
            {
                return FALSE;
            }
        }

        u1Offset = prFilter->arBank[1].u1Offset;
        fgMask = FALSE;
        for(i=0; i<2; i++)
        {
            u1Index = i + u1Offset;
            if(u1Index >= u1CompareLen)
            {
                break;
            }
            if(prFilter->arBank[1].au1Mask[i + 6])
            {
                fgMask = TRUE;
                break;
            }
        }
        if(!fgMask)
        {
            return TRUE;    // No any mask
        }

        for(i=0; i<2; i++)
        {
            u1Index = i + u1Offset;
            if(u1Index >= u1CompareLen)
            {
                break;
            }
            u1Data = *(pu1CompareBuf + u1Index);
            u1Mask = prFilter->arBank[1].au1Mask[i + 6];
            if(u1Mask && !_DmxCompare(u1Data, prFilter->arBank[1].au1Data[i + 6], u1Mask))
            {
                return TRUE;
            }
        }

        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
static BOOL _DmxSWFilter_Generic(const FILTER_STRUCT_T *prFilter,
                              const UINT8 *pu1CompareBuf, UINT8 u1CompareLen)
{
    UINT8 i, u1CompMax;
    UINT8 u1Data1, u1Data2, u1Offset, u1Index;
    BOOL fgMask;

    if((prFilter == NULL) || (pu1CompareBuf == NULL))
    {
        return FALSE;
    }

    u1Offset = prFilter->rGeneric.u1Offset;
    if(u1CompareLen <= u1Offset)
    {
        return TRUE;
    }

    if((u1CompareLen - u1Offset) > 15)
    {
        u1CompMax = 15;
    }
    else
    {
        u1CompMax = u1CompareLen - u1Offset;
    }

    // Positive
    for(i=0; i<u1CompMax; i++)
    {
        u1Index = i + u1Offset;
        u1Data1 = *(pu1CompareBuf + u1Index) & (~(prFilter->rGeneric.au1PosNeg[i+1]));
        u1Data2 = prFilter->rGeneric.au1Data[i+1] & (~(prFilter->rGeneric.au1PosNeg[i+1]));
        if(!_DmxCompare(u1Data1, u1Data2, prFilter->rGeneric.au1Mask[i+1]))
        {
            return FALSE;
        }
    }

    // Negative, check negative mask exist
    fgMask = FALSE;
    for(i=0; i<u1CompMax; i++)
    {
        if((prFilter->rGeneric.au1PosNeg[i+1] & prFilter->rGeneric.au1Mask[i+1]) != 0)
        {
            fgMask = TRUE;
            break;
        }
    }

    if(!fgMask)
    {
        return TRUE;
    }

    // Negative, compare
    for(i=0; i<u1CompMax; i++)
    {
        u1Index = i + u1Offset;
        u1Data1 = *(pu1CompareBuf + u1Index) & (prFilter->rGeneric.au1PosNeg[i+1]);
        u1Data2 = prFilter->rGeneric.au1Data[i+1] & (prFilter->rGeneric.au1PosNeg[i+1]);
        if(!_DmxCompare(u1Data1, u1Data2, prFilter->rGeneric.au1Mask[i+1]))
        {
            return TRUE;
        }
    }

    if(i == u1CompMax)
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxSWFilter
 *  Driver Filter PSI after HW Table-ID only filter
 *
 *  @param  u1Pidx          PID index
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSWFilter(const FILTER_STRUCT_T *prFilter,
                         const UINT8* pu1CompareBuf, UINT8 u1CompareLen)
{
    BOOL fgRet = FALSE;

    if((prFilter == NULL) || (pu1CompareBuf == NULL))
    {
        return FALSE;
    }

    switch(prFilter->eMode)
    {
    case DMX_FILTER_MODE_SHORT:
        if(_DmxSWFilter_Short(prFilter, pu1CompareBuf, u1CompareLen))
        {
            fgRet = TRUE;
        }
        break;

    case DMX_FILTER_MODE_LONG:
        if(_DmxSWFilter_Long(prFilter, pu1CompareBuf, u1CompareLen))
        {
            fgRet = TRUE;
        }
        break;

    case DMX_FILTER_MODE_POS_NEG:
        if(_DmxSWFilter_PosNeg(prFilter, pu1CompareBuf, u1CompareLen))
        {
            fgRet = TRUE;
        }
        break;

    case DMX_FILTER_MODE_14_2:
        if(_DmxSWFilter_14_2(prFilter, pu1CompareBuf, u1CompareLen))
        {
            fgRet = TRUE;
        }
        break;

    case DMX_FILTER_MODE_GENERIC:
        if(_DmxSWFilter_Generic(prFilter, pu1CompareBuf, u1CompareLen))
        {
            fgRet = TRUE;
        }
        break;

    default:
        fgRet = FALSE;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxPSISWFilter
 *  Driver Filter PSI after HW Table-ID only filter
 *
 *  @param  u1Pidx          PID index
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPSISWFilter(UINT8 u1Pidx, const UINT8* pu1CompareBuf,
                            UINT8 u1CompareLen, UINT32 *pu4Bitmap)
{
    PID_STRUCT_T* prPidStruct;
    FILTER_STRUCT_T *prFilter;
    UINT32 i, u4SecSel;
    UINT8 u1SecCount, u1SecIdx;

    if((pu1CompareBuf == NULL) || (pu4Bitmap == NULL))
    {
        return FALSE;
    }

    *pu4Bitmap = 0;
    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    _DMX_Lock();
    u4SecSel = PID_S_W(u1Pidx, 10);
    u1SecCount = prPidStruct->u1SecCount;
    u1SecIdx = prPidStruct->u1SecIdx;
    _DMX_Unlock();

    if(u1SecCount == 1)
    {
        prFilter = _DMX_GetFilterStruct(u1SecIdx);
        if(prFilter == NULL)
        {
            LOG(1, "PSI SW Filter: wrong index 0x%x\n", u1SecIdx);
            return FALSE;
        }

        if(prFilter->eMode == DMX_FILTER_MODE_GENERIC)
        {
            if(!prFilter->rGeneric.fgEnable)
            {
                return FALSE;
            }

            if(!_DmxCompare(pu1CompareBuf[0], prFilter->rGeneric.au1Data[0],
                prFilter->rGeneric.au1Mask[0]))
            {
                return FALSE;
            }
        }
        else
        {
            if(!prFilter->arBank[0].fgEnable)
            {
                return FALSE;
            }

            if(!_DmxCompare(pu1CompareBuf[0], prFilter->arBank[0].au1Data[0],
                prFilter->arBank[0].au1Mask[0]))
            {
                return FALSE;
            }
        }

        if(_DmxSWFilter(prFilter, pu1CompareBuf, u1CompareLen))
        {
            *pu4Bitmap = (1 << u1SecIdx);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    for(i=0; i<32; i++)
    {
        if((u4SecSel >> i) & 0x1)
        {
            prFilter = _DMX_GetFilterStruct(i);
            if(prFilter == NULL)
            {
                LOG(1, "PSI SW Filter: wrong index 0x%x\n", i);
                return FALSE;
            }

            if(prFilter->eMode == DMX_FILTER_MODE_GENERIC)
            {
                if(!prFilter->rGeneric.fgEnable)
                {
                    continue;
                }

                if(!_DmxCompare(pu1CompareBuf[0], prFilter->rGeneric.au1Data[0],
                    prFilter->rGeneric.au1Mask[0]))
                {
                    continue;
                }
            }
            else
            {
                if(!prFilter->arBank[0].fgEnable)
                {
                    continue;
                }

                if(!_DmxCompare(pu1CompareBuf[0], prFilter->arBank[0].au1Data[0],
                    prFilter->arBank[0].au1Mask[0]))
                {
                    continue;
                }
            }

            if(_DmxSWFilter(prFilter, pu1CompareBuf, u1CompareLen))
            {
                *pu4Bitmap |= (1 << i);
            }
        }
    }

    if(*pu4Bitmap == 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//-----------------------------------------------------------------------------
/** _DmxPsiParserHandler
 *  The PSI parser handler
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPsiParserHandler(UINT8 u1Pidx, UINT32 u4SecAddr,
                            UINT32 u4SecLen, UINT8 u1SN, UINT32 u4Bitmap)
{
    PID_STRUCT_T* prPidStruct;
    BOOL fgEnable;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    // Copy from OneByte buffer to PID buffer
    _DMX_Lock();
    fgEnable = prPidStruct->fgEnable;
    _DMX_Unlock();

    if(!fgEnable)
    {
        return FALSE;
    }  

    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_NOTIFY_INFO_PSI_T rInfo;
        BOOL fgKeepData;

        rInfo.u4SecAddr = u4SecAddr;
        rInfo.u4SecLen = u4SecLen;
        rInfo.u1SerialNumber = u1SN;
        rInfo.u4MatchBitmap = u4Bitmap;

        LOG(7, "Send pidx %u, section 0x%08x, size %u\n", u1Pidx, u4SecAddr,
            u4SecLen);

        fgKeepData = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_PSI,
            (UINT32)&rInfo, prPidStruct->pvNotifyTag);

        if (!fgKeepData)
        {
            // Application does not want to process this, discard it
            LOG(7, "AP discards pidx %u, section 0x%08x, size %u\n",
                u1Pidx, u4SecAddr, u4SecLen);
        }
    }
    else
    {
#ifndef CC_MINI_DRIVER
        // No registered callback function, emulation mode
        if (_DMX_IDE_IsEnabled())
        {
            _DmxPSIParserSendToIde(u1Pidx, u4SecAddr, u4SecLen);
        }
        else
#endif  // CC_MINI_DRIVER
        {
            // Set RP to WP
            _DMX_Lock();
            prPidStruct->u4Rp = prPidStruct->u4Wp;
            _DMX_Unlock();
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxPSIParser
 *  The PSI section parser
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPSIParser(UINT8 u1Pidx, UINT8 u1SecCount, UINT32 u4EndAddr, UINT8 u1SN)
{
    UCHAR au1Payload[3];
    UINT8* pu1Data;
    UINT32 u4BufStart, u4BufEnd, u4BufLen;
    UINT32 u4SecRp, u4SecRp2, u4SecOrg, u4SecLen = 0, i, j;
    PID_STRUCT_T* prPidStruct;
    UINT8 au1SectionBuf[SECTION_COMPARE_LEN];
    UINT32 u4SecLen2;
    UINT8 u1CompareLen;
#ifdef DMX_MEASURE_PSI_TIME
    HAL_TIME_T rTime1, rTime2, rTimeDiff, rTimeMax;
#endif  // DMX_MEASURE_PSI_TIME
    UINT32 u4MatchSecCount = 0;
    UINT32 u4CompareLen;
    UINT32 u4SecBitmap;

    HalFlushInvalidateDCache();

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    if(!prPidStruct->fgEnable)
    {
        return TRUE;
    }

    _DMX_Lock();
    u4SecRp    = prPidStruct->u4SectionRp;
    u4BufStart = prPidStruct->u4BufStart;
    u4BufEnd   = prPidStruct->u4BufEnd + 1;     // align to 8N
    u4BufLen   = prPidStruct->u4BufLen;
    _DMX_Unlock();

    //ASSERT((u4EndAddr >= u4BufStart) && (u4EndAddr < u4BufEnd));
    if((u4EndAddr < u4BufStart) || (u4EndAddr >= u4BufEnd))
    {
        return FALSE;
    }

    _DMX_Lock();
    prPidStruct->u4Wp = u4EndAddr;
    _DMX_Unlock();

    u4SecOrg = u4SecRp;

    for (i = 0; i < u1SecCount; i++)
    {
#ifdef DMX_MEASURE_PSI_TIME
        HAL_GetTime(&rTime1);
#endif  // DMX_MEASURE_PSI_TIME
        pu1Data = (UINT8*)VIRTUAL(u4SecRp);

        for (j = 0; j < 3; j++)
        {
            au1Payload[j] = _DMX_GetByte(&pu1Data, VIRTUAL(u4BufEnd),
                u4BufLen, 0, 1);
        }

        u4SecLen = (au1Payload[1] & 0xf) << 8;
        u4SecLen |= au1Payload[2];
        u4SecLen += 3;

        // Check validity
        u4SecRp2 = _DMX_AdvanceAddr(u4SecRp, (INT32)u4SecLen,
            u4EndAddr, u4BufStart, u4BufEnd);
        if (u4SecRp2 == 0)
        {
            UINT32 u4DataSize;

            u4DataSize = DATASIZE(u4SecRp, u4EndAddr, u4BufLen);

            // Something wrong
            LOG(3, "Incorrect section data! Buf 0x%08x - 0x%08x\n",
                u4BufStart, u4BufEnd);
            LOG(3, "  Pidx: %u, sec: 0x%08x, size: %u, WP: 0x%08x, data: %u\n",
                u1Pidx, u4SecRp, u4SecLen, u4EndAddr, u4DataSize);

            u4SecRp = u4SecRp2;

            UNUSED(u4DataSize);         // Unused in release build

            continue;
        }

#if 0   //   Not necessary
        if(au1Payload[1] & 0x80)
        {
            // Section_syntax_indicator is true
            u4SecLen2 = u4SecLen - 4;
        }
        else
        {
            u4SecLen2 = u4SecLen;
        }
#else
        u4SecLen2 = u4SecLen;
#endif  // 0
        u4CompareLen = (u4SecLen2 > SECTION_COMPARE_LEN) ?
                            SECTION_COMPARE_LEN : u4SecLen2;
        u1CompareLen = (UINT8)u4CompareLen;

        // Fix Klocwork warning
        // x_memset((void*)au1SectionBuf, 0, sizeof(au1SectionBuf));
        au1SectionBuf[0] = 0;

        // For linear buffer to do compare
        VERIFY(_DMX_CopyRingBuffer((UINT32)au1SectionBuf, (UINT32)au1SectionBuf,
            (UINT32)(au1SectionBuf + SECTION_COMPARE_LEN), u4SecRp, u4BufStart,
            u4BufEnd, u1CompareLen) == (UINT32)au1SectionBuf);

#ifdef __MODEL_slt__  // for J-chip SLT
        if ((au1SectionBuf[0] == 0xC8) || (au1SectionBuf[0] == 0xC9))
        {
            _DMX_CheckVCT(u4BufStart, u4BufEnd, u4SecRp, u4SecLen2);
        }
        //---------------------------------------------------------------------
        // --- Beginning of "check for AW Eraser" ---
#else
        if (_fgAwEraserCheckBegin)
        {
            if ((au1SectionBuf[0] == 0xC8) || (au1SectionBuf[0] == 0xC9))
            {
                _DMX_AwEraserCheck(u4BufStart, u4BufEnd, u4SecRp, u4SecLen2);
                _fgAwEraserCheckBegin = FALSE;  // run once for each CLI command
            }
        }
        // --- End of "check for AW Eraser" ---
        //---------------------------------------------------------------------
#endif  // __MODEL_slt__

        if(!_DmxPSISWFilter(u1Pidx, au1SectionBuf, u1CompareLen, &u4SecBitmap))
        {
            u4SecRp = u4SecRp2;
            continue;
        }

        // Dispatch to real handlers
        if (u4SecLen > 3)
        {
            // Note section length 3 means no data at all, thus be discarded
            if(!_DmxPsiParserHandler(u1Pidx, u4SecRp, u4SecLen, u1SN, u4SecBitmap))
            {
                LOG(3, "Seccount: %d\n", u1SecCount);
                return TRUE;
            }
        }

        u4MatchSecCount++;

        u4SecRp = u4SecRp2;
#ifdef DMX_MEASURE_PSI_TIME
        HAL_GetTime(&rTime2);
        HAL_GetDeltaTime(&rTimeDiff, &rTime1, &rTime2);
        _DMX_PSI_GetMaxTime(&rTimeMax);
        if(rTimeDiff.u4Seconds > rTimeMax.u4Seconds)
        {
            _DMX_PSI_SetMaxTime(&rTimeDiff);
        }
        else if(rTimeDiff.u4Seconds == rTimeMax.u4Seconds)
        {
            if(rTimeDiff.u4Micros > rTimeMax.u4Micros)
            {
                _DMX_PSI_SetMaxTime(&rTimeDiff);
            }
        }
#endif  // DMX_MEASURE_PSI_TIME
    }

    if (u4SecRp != u4EndAddr)
    {
        UINT32 u4DataSize;

        u4DataSize = DATASIZE(u4SecOrg, u4EndAddr, u4BufLen);

        // Something wrong
        LOG(3, "Incorrect section data! Buf 0x%08x - 0x%08x\n",
            u4BufStart, u4BufEnd);
        LOG(3, "  Pidx: %u, sec: 0x%08x, size: %u, WP: 0x%08x, data: %u\n",
            u1Pidx, u4SecOrg, u4SecLen, u4EndAddr, u4DataSize);

        UNUSED(u4DataSize);         // Unused in release build
    }

    prPidStruct->u4SectionRp = u4EndAddr;

    // Debug info
    prPidStruct->rCounters.u4OneByteSecCount += u1SecCount;
    prPidStruct->rCounters.u4SecCount += u4MatchSecCount;

    LOG(7, "pidx %02u, WP 0x%08x, sec# %u, all sec# %u\n",
        u1Pidx, u4EndAddr, u1SecCount, prPidStruct->rCounters.u4SecCount);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxPSIThread
 *  The PSI thread
 *
 *  @param  prArg           The thread argument
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _DmxPSIThread(VOID* pvArg)
{
    static DMX_PSI_MSG_T rPsi;
    PID_STRUCT_T *prPidStruct;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;

    UNUSED(pvArg);

    while (1)
    {
        zMsgSize = sizeof (rPsi);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rPsi, &zMsgSize,
            &_hDmxPSIQueue, 1, X_MSGQ_OPTION_WAIT);

        // Inhibit API function calls during processing message
        _DMX_LockApi();

        if (i4Ret != OSR_OK)
        {
            _DMX_UnlockApi();
            LOG(1, "Error on receiving PSI message!\n");
            continue;
        }

        prPidStruct = _DMX_GetPidStruct(rPsi.u1Pidx);
        if (rPsi.u1SN != prPidStruct->u1SerialNumber)
        {
            _DMX_UnlockApi();
            LOG(3, "Overdue PSI message! (pidx: %d)\n", rPsi.u1Pidx);
            continue;
        }

        if(!prPidStruct->fgEnable)
        {
            _DMX_UnlockApi();
            continue;
        }

        ASSERT(zMsgSize == sizeof (rPsi));
        ASSERT(u2QueueIndex == 0);

        if(!_DmxPSIParser(rPsi.u1Pidx, rPsi.u1SecCount, rPsi.u4EndAddr, rPsi.u1SN))
        {
            LOG(3, "Error on parsing PSI message\n");
        }

        // Resume API calls
        _DMX_UnlockApi();
    }   // while (1)
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BOOL _DMX_AddPSI(DMX_PSI_MSG_T *prPSI)
{
    PID_STRUCT_T *prPidStruct;

    if(prPSI == NULL)
    {
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(prPSI->u1Pidx);
    prPSI->u1SN = prPidStruct->u1SerialNumber;

#ifndef CC_EMULATION
    if (x_msg_q_send(_hDmxPSIQueue, (VOID*)prPSI, sizeof (DMX_PSI_MSG_T),
        DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(3, "Demux PSI queue full!\n");
        return FALSE;
    }
#else
    if(!_DmxPSIParser(prPSI->u1Pidx, prPSI->u1SecCount, prPSI->u4EndAddr, prPSI->u1SN))
    {
        LOG(1, "Error on parsing PSI message\n");
    }
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_InitPSIHandler
 *  Initialize PSI interfaces
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_InitPSIHandler(void)
{
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        // Create picture queues
        if (x_msg_q_create(&_hDmxPSIQueue, DMX_PSI_QUEUE_NAME,
            sizeof (DMX_PSI_MSG_T), DMX_PSI_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create picture queue!\n");
            return FALSE;
        }

        // Create PSI thread
        if (x_thread_create(&_hDmxPSIThread, DMX_PSI_THREAD_NAME,
            DMX_PSI_THREAD_STACK_SIZE, DMX_PSI_THREAD_PRIORITY, _DmxPSIThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX thread!\n");
            return FALSE;
        }

        _fgInit = TRUE;
    }

    return TRUE;
}

