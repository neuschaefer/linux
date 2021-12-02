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
 * $Author: jiangbo.wang $
 * $Date: 2012/07/18 $
 * $RCSfile: pvr_mw.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mw.c
 *  PVR driver - middleware interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "fvr_if.h"
#include "dmx_mw.h"
#include "x_lint.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
#include "cb_low.h"
#endif // LINUX_TURNKEY_SOLUTION

LINT_EXT_HEADER_BEGIN

#include "drv_comp_id.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_pvr.h"
#include "x_rm_dev_types.h"
#include "x_os.h"
#include "x_mm_common.h"

#define DEFINE_IS_LOG   DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MW_FVR_CTRL_NUM                   (1)

#define MW_FVR_META_BUF_NUM               (5)

#define PVRMW_TICK_PIC_MAX                (128)

#define PVRMW_FIFO_ALIGNMENT              (64)

#define PVRMW_BUFFER_SIZE                 (0x300000)

#define PVRMW_TICK_METABUF_SIZE           (8 * 1024)//(MW_FVR_META_BUF_NUM * PVRMW_TICK_PIC_MAX * 4)

#define PVRMW_THREAD_NAME                 "PVRMWT"
#define PVRMW_THREAD_STACK_SIZE           1024
#define PVRMW_THREAD_PRIORITY             100

#define PVRMW_QUEUE_NAME                  "PVRISRQ"
#define PVRMW_QUEUE_SIZE                  32

#define PVRMW_PRIORITY_NORMAL             128

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    BOOL fgValid;
    UINT32 u4SerialNum;
    UINT32 u4BufAddr;
} MWPVR_META_BUF_T;

typedef struct
{
    BOOL fgEnable;
    UINT8 u1Pidx;
} MWPVR_PIDX_USAGE_T;

typedef enum
{
    MWPVRCMD_TIME,
    MWPVRCMD_OVERFLOW,
    MWPVRCMD_SCRAMBLE
} MWPVRCMDTYPE;

typedef struct
{
    MWPVRCMDTYPE eCmd;
    PVR_TICK_DATA_T rTickData;
    UINT32 u4Data1;
    UINT8 u1Pidx;
} MWPVR_MSG_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PVR_NFY_INFO_T _rMwPVRNotifyInfo;

static BOOL _fgIsOverflow = FALSE;
static UINT32 _u4MwPVRTickNum = 0;
static UINT32 _u4MwPVRPktCount = 0;
static UINT32 _u4MwPVRPicIdx = 0;
static UINT32 *_pu4MwPVRPicData = NULL;

static MWPVR_META_BUF_T _arMwPVRMetaBuf[MW_FVR_META_BUF_NUM];

static CRIT_STATE_T _rMwPvrLock;
static BOOL _fgMwPvrLocking = FALSE;

static BOOL _fgMwPvrEnable = FALSE;

static MWPVR_PIDX_USAGE_T _arMwPvrPerPidxUsage[FVR_SCRAMBLE_PID_INDEX];

#ifdef LINUX_TURNKEY_SOLUTION
static UINT8 *_pu1PvrSwapAddr = NULL;
static UINT32 u4PvrSwapMaxLen = 0;
static UINT32 _u4MwPvrStreamPhyAddr;
static UINT32 _u4MwPvrMetaPhyAddr;
static UINT32 _u4MwPvrStreamMwAddr;
static UINT32 _u4MwPvrKelTotalSize;

static HANDLE_T _hPVRMwThread = NULL_HANDLE;
static HANDLE_T _hPVRMwQueue = NULL_HANDLE;
#endif // LINUX_TURNKEY_SOLUTION

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef LINUX_TURNKEY_SOLUTION
EXTERN VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
EXTERN INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
#endif

#ifdef LINUX_TURNKEY_SOLUTION
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
UINT32 _MwPvrKerlToUserAddr(UINT32 u4KerAddr)
{
    UINT32 u4PhyAddr;

    u4PhyAddr = PHYSICAL(u4KerAddr);
    UNUSED(_u4MwPvrKelTotalSize);
    ASSERT(u4PhyAddr >= _u4MwPvrStreamPhyAddr);
    ASSERT(u4PhyAddr < (_u4MwPvrStreamPhyAddr + _u4MwPvrKelTotalSize));

    return (u4PhyAddr - _u4MwPvrStreamPhyAddr + _u4MwPvrStreamMwAddr);
}

UINT32 _MwPvrUserToKerlAddr(UINT32 u4UserAddr)
{
    ASSERT(u4UserAddr >= _u4MwPvrStreamMwAddr);
    ASSERT(u4UserAddr < (_u4MwPvrStreamMwAddr + _u4MwPvrKelTotalSize));

    return VIRTUAL(u4UserAddr - _u4MwPvrStreamMwAddr + _u4MwPvrStreamPhyAddr);
}

#endif // LINUX_TURNKEY_SOLUTION

//-----------------------------------------------------------------------------
/** _MwPvrLock
 *  Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MwPvrLock(void)
{
    _rMwPvrLock = x_crit_start();

    ASSERT(!_fgMwPvrLocking);
    _fgMwPvrLocking = TRUE;

    UNUSED(_fgMwPvrLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _MwPvrUnlock
 *  Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MwPvrUnlock(void)
{
    ASSERT(_fgMwPvrLocking);
    _fgMwPvrLocking = FALSE;

    x_crit_end(_rMwPvrLock);

    UNUSED(_fgMwPvrLocking);          // Make Lint happy
}


#ifdef LINUX_TURNKEY_SOLUTION
//-----------------------------------------------------------------------------
/** set_pvr_nfy
 */
//-----------------------------------------------------------------------------
static BOOL set_pvr_nfy(PVR_NFY_COND_T e_nfy_cond, UINT32 ui4_data_1)
{
    PVR_SET_NFY_PARAM pvr_set_param;

    pvr_set_param.pv_nfy_tag = _rMwPVRNotifyInfo.pv_tag;
    pvr_set_param.e_nfy_cond = e_nfy_cond;
    if (PVR_COND_TICK == e_nfy_cond)
    {
        memcpy((VOID*)&(pvr_set_param.u.rTickData), (VOID*)ui4_data_1, sizeof(PVR_TICK_DATA_T));
    }
    else
    {
        pvr_set_param.u.ui4_data_1 = ui4_data_1;
    }
    pvr_set_param.pf_nfy = _rMwPVRNotifyInfo.pf_nfy;

    return _CB_PutEvent_Adpt(CB_DRV_PVR_NFY, sizeof(PVR_SET_NFY_PARAM), &pvr_set_param);
}

//-----------------------------------------------------------------------------
/** set_pvr_scramble_nfy
 */
//-----------------------------------------------------------------------------
static BOOL set_pvr_scramble_nfy(VOID* pv_tag, RECDEMUX_COND_T e_nfy_cond, UINT32 ui4_data_1,
                                 x_recdemux_nfy_fct pf_nfy)
{
    PVR_SCR_SET_NFY_PARAM pvr_scr_set_param;

    pvr_scr_set_param.pv_nfy_tag = pv_tag;
    pvr_scr_set_param.e_nfy_cond = e_nfy_cond;
    pvr_scr_set_param.ui4_data_1 = ui4_data_1;
    pvr_scr_set_param.pf_nfy = pf_nfy;

    return _CB_PutEvent_Adpt(CB_DRV_PVR_SCR_NFY, sizeof(PVR_SCR_SET_NFY_PARAM), &pvr_scr_set_param);
}
#endif // LINUX_TURNKEY_SOLUTION


//-----------------------------------------------------------------------------
/** _MwPvrAllocPerPidx
 */
//-----------------------------------------------------------------------------
static UINT8 _MwPvrAllocPerPidx(UINT8 u1Pidx)
{
    UINT8 i;

    for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
    {
        if(u1Pidx != 0xFF)
        {
            if(_arMwPvrPerPidxUsage[i].fgEnable && (u1Pidx == _arMwPvrPerPidxUsage[i].u1Pidx))
            {
                break;
            }
        }
    }

    if(i == FVR_SCRAMBLE_PID_INDEX)
    {
        for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
        {
            if(!_arMwPvrPerPidxUsage[i].fgEnable)
            {
                break;
            }
        }
    }

    if(i == FVR_SCRAMBLE_PID_INDEX)
    {
        return 0xFF;
    }

    _arMwPvrPerPidxUsage[i].fgEnable = TRUE;
    _arMwPvrPerPidxUsage[i].u1Pidx = u1Pidx;
    
    return i;
}


//-----------------------------------------------------------------------------
/** _MwPvrSetTimestamp
 */
//-----------------------------------------------------------------------------
static void _MwPvrSetTimestamp(void)
{
    FVR_TIMESTAMP_T rTimeStamp;

    // Set timestamp
    rTimeStamp.fgEnable = TRUE;
    rTimeStamp.u2ClkDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    rTimeStamp.u4Counter = 0;
    rTimeStamp.u4Mask = 0xFFFFFFFF;
    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);
}


//-----------------------------------------------------------------------------
/** _MwPvrSetPid
 */
//-----------------------------------------------------------------------------
static BOOL _MwPvrSetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid)
{
    FVR_PID_T rPid;
    
    if(prPid == NULL)
    {
        return FALSE;
    }

    rPid = *prPid;
    if(u4Flags & FVR_PID_FLAG_TSINDEX)
    {
        rPid.u1TsIndex = DMX_GetFramerIndex();
    }

    if(!FVR_SetPid(u1Pidx, u4Flags, &rPid))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwPVRFreeMetaBuf
 */
//-----------------------------------------------------------------------------
static BOOL _MwPVRFreeMetaBuf(UINT8 u1Idx)
{
    if(u1Idx >= MW_FVR_META_BUF_NUM)
    {
        return FALSE;
    }

    _arMwPVRMetaBuf[u1Idx].fgValid = FALSE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwPVRAllocMetaBuf
 */
//-----------------------------------------------------------------------------
static UINT8 _MwPVRAllocMetaBuf(void)
{
    UINT8 i;
#if 0       // debug purpose
    UINT8 u1Count = 0;

    for(i=0; i<MW_FVR_META_BUF_NUM; i++)
    {
        if(!_arMwPVRMetaBuf[i].fgValid)
        {
            u1Count++;
        }
    }
    LOG(1, "Available meta buf: %d\n", u1Count);
#endif
    
    for(i=0; i<MW_FVR_META_BUF_NUM; i++)
    {
        if(!_arMwPVRMetaBuf[i].fgValid)
        {
            break;
        }
    }

    if(i == MW_FVR_META_BUF_NUM)
    {
        return 0xFF;
    }

    return i;
}


//-----------------------------------------------------------------------------
/** _MwPVRPicHandler
 */
//-----------------------------------------------------------------------------
static void _MwPVRPicHandler(UINT8 u1Pidx, UINT8 u1Type, UINT32 u4PktIdx,
                                     const void* pvTag)
{
    FVR_VIDEO_TYPE_T eVideoType;
    UINT32 u4PicIdx;
    UINT8 u1Pattern;
    
    ASSERT(_pu4MwPVRPicData != NULL);
    
    eVideoType = FVR_GetVideoType();
    
    u1Pattern = 0;
    if(eVideoType == FVR_VIDEO_MPEG)
    {
        switch(u1Type)
        {
        case FVR_PIC_I_TYPE:
            u1Pattern = 3;
            break;

        case FVR_PIC_P_TYPE:
            u1Pattern = 2;
            break;

        case FVR_PIC_B_TYPE:
            u1Pattern = 1;
            break;

        default:
            break;
        }
    }
    else if(eVideoType == FVR_VIDEO_H264)
    {
        switch(u1Type)
        {
        case FVR_PIC_H264_IDR_TYPE:
            u1Pattern = 3;
            break;

        case FVR_PIC_H264_NIDR_TYPE:
            u1Pattern = 2;
            break;

        default:
            break;
        }
    }

    _MwPvrLock();
    _pu4MwPVRPicData[_u4MwPVRPicIdx] = ((u1Pattern & 0x3) << 30) | (u4PktIdx & 0x3FFFFFFF);
    _u4MwPVRPicIdx++;
    u4PicIdx = _u4MwPVRPicIdx;
    _MwPvrUnlock();
   
    if(u4PicIdx >= PVRMW_TICK_PIC_MAX)
    {
        //Printf("Picture data exceed 0x%x\n", TICK_PIC_MAX);
        ASSERT(0);
    }

    UNUSED(pvTag);
}


//-----------------------------------------------------------------------------
/** _MwPVRDectectScrambleHandler
 */
//-----------------------------------------------------------------------------
static BOOL _MwPVRDectectScrambleHandler(UINT8 u1Pidx, FVR_SCRAMBLE_STATE_T eState)
{
    DMX_MW_PVRPID_INFO_T *prPidInfo;
    RECDEMUX_SCRS_T eScrs;

    prPidInfo = _MW_Rec_GetPidInfo(u1Pidx);
    if(prPidInfo == NULL)
    {
        LOG(1, "%s error\n", __FUNCTION__);
        return FALSE;
    }

    //Printf("%s, Pidx %u scrambling state: %s\n", __FUNCTION__, u1Pidx, 
    //            (eState == FVR_SCRAMBLE_STATE_SCRAMBLED) ? "SCRAMBLED" : "CLEAR");

    if(eState == FVR_SCRAMBLE_STATE_CLEAR)
    {
        eScrs = RECDEMUX_SCRS_CLEAR;
    }
    else if(eState == FVR_SCRAMBLE_STATE_SCRAMBLED)
    {
        eScrs = RECDEMUX_SCRS_SCRAMBLED;
    }
    else
    {
        eScrs = RECDEMUX_SCRS_UNKNOWN;
    }

    if(prPidInfo->rNfyInfo.pf_recdemux_nfy != NULL)
    {
#ifdef LINUX_TURNKEY_SOLUTION
        MWPVR_MSG_T rMsg;
        rMsg.eCmd = MWPVRCMD_SCRAMBLE;
        rMsg.u4Data1 = eScrs;
        rMsg.u1Pidx = u1Pidx;
        if (x_msg_q_send(_hPVRMwQueue, (VOID*)&rMsg, sizeof (MWPVR_MSG_T),
                                            PVRMW_PRIORITY_NORMAL) != OSR_OK)
        {
            LOG(3, "PVR MW queue full!\n");
            return FALSE;
        }
/*
        if(!set_pvr_scramble_nfy(prPidInfo->rNfyInfo.pv_tag, RECDEMUX_COND_SCRS,  (UINT32)eScrs,
                                 prPidInfo->rNfyInfo.pf_recdemux_nfy))
        {
            return FALSE;
        }
*/                                 
#else
        if(!prPidInfo->rNfyInfo.pf_recdemux_nfy(prPidInfo->rNfyInfo.pv_tag, 
                                RECDEMUX_COND_SCRS, (UINT32)eScrs))
        {
            return FALSE;
        }
#endif
    }

    UNUSED(eScrs);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwPVRTimeHandler
 */
//-----------------------------------------------------------------------------
static void _MwPVRTimeHandler(UINT32 u4Data, const void* pvTag)
{
    FVR_GBL_TIMENOTIFY_INFO_T* prTimeInfo;
    UINT32 i, u4PicIdx, u4PicSize, u4EntryNum;
    UINT32 u4PacketNum, u4PrePktNum;
    INT32 i4PktDiff;
    UINT8 u1MetaBufIdx;
    PVR_TICK_DETAIL_T *prPvrTick;
    PVR_PIC_INFO_T *prPicInfo;    
#ifdef LINUX_TURNKEY_SOLUTION
    MWPVR_MSG_T rMsg;
#endif
    ASSERT(_pu4MwPVRPicData != NULL);

    prTimeInfo = (FVR_GBL_TIMENOTIFY_INFO_T*)u4Data;
    
    u1MetaBufIdx = _MwPVRAllocMetaBuf();
    if(u1MetaBufIdx == 0xFF)
    {
        if(!FVR_Stop())
        {
            return;
        }

        _MwPvrLock();
        _fgIsOverflow = TRUE;
        _MwPvrUnlock();

        FVR_SetIsOverflow(TRUE);    // DEBUG purpose

        LOG(1, "Meta overflow\n");

        if(_rMwPVRNotifyInfo.pf_nfy != NULL)
        {
            LOG(3, "%s meta buf overflow\n", __FUNCTION__);
#ifdef LINUX_TURNKEY_SOLUTION
            rMsg.eCmd = MWPVRCMD_OVERFLOW;
            rMsg.u4Data1 = PVR_OVERFLOW_REASON_METADATA_BUF_FULL;
            if (x_msg_q_send(_hPVRMwQueue, (VOID*)&rMsg, sizeof (MWPVR_MSG_T),
                                                PVRMW_PRIORITY_NORMAL) != OSR_OK)
            {
                LOG(3, "PVR MW queue full!\n");
            }
/*            
            if(!set_pvr_nfy(PVR_COND_OVERFLOW, PVR_OVERFLOW_REASON_METADATA_BUF_FULL))
            {
                // UNUSED
            }
*/
#else                    
            if(!_rMwPVRNotifyInfo.pf_nfy(_rMwPVRNotifyInfo.pv_tag, PVR_COND_OVERFLOW, 
                                         PVR_OVERFLOW_REASON_METADATA_BUF_FULL))
            {
                LOG(3, "%s MW notify return fail\n", __FUNCTION__);
            }
#endif // LINUX_TURNKEY_SOLUTION            
        }
        return;
    }

    if(u1MetaBufIdx >= MW_FVR_META_BUF_NUM)
    {
        return;
    }

    _MwPvrLock();
    _arMwPVRMetaBuf[u1MetaBufIdx].fgValid = TRUE;
    u4PrePktNum = _u4MwPVRPktCount;
    _MwPvrUnlock();
        
    if(prTimeInfo->u4PktCount >= u4PrePktNum)
    {
        u4PacketNum = prTimeInfo->u4PktCount - u4PrePktNum;
    }
    else
    {
        // Packet count overflow
        u4PacketNum = 0xFFFFFFFF - (u4PrePktNum - prTimeInfo->u4PktCount - 1);
    }    

    ASSERT((u4PacketNum * 192) == prTimeInfo->u4Size);
//    LOG(5, "%s pkt:%d, size:0x%x\n", __FUNCTION__, u4PacketNum, prTimeInfo->u4Size);

    _MwPvrLock();
    _u4MwPVRPktCount = prTimeInfo->u4PktCount;

    // Compose meta buf
    _arMwPVRMetaBuf[u1MetaBufIdx].u4SerialNum = _u4MwPVRTickNum;
    prPvrTick = (PVR_TICK_DETAIL_T*)(_arMwPVRMetaBuf[u1MetaBufIdx].u4BufAddr);
    prPicInfo = (PVR_PIC_INFO_T*)(_arMwPVRMetaBuf[u1MetaBufIdx].u4BufAddr + sizeof(PVR_TICK_DETAIL_T));
    
    prPvrTick->ui4_tick_num = _u4MwPVRTickNum++;          // Field 0: Tick num
    PVR_TICK_DETAIL_SET_BLKS(prPvrTick, u4PacketNum);

    u4EntryNum = 0;

    // Default value
    PVR_TICK_DETAIL_SET_ENTRIES(prPvrTick, 0);

    if(u4PacketNum == 0)
    {
        _u4MwPVRPicIdx = 0;
    }
    else
    {    
        if(_u4MwPVRPicIdx > 1)
        {
            for(i=0; i<(_u4MwPVRPicIdx - 1); i++)
            {
                if(((_pu4MwPVRPicData[i] >> 30) & 0x3) == 3)
                {
                    u4PicIdx = _pu4MwPVRPicData[i] & 0x3FFFFFFF;
                    u4PicSize = (_pu4MwPVRPicData[i+1] & 0x3FFFFFFF) - 
                                (_pu4MwPVRPicData[i] & 0x3FFFFFFF);

                    i4PktDiff = (INT32)u4PicIdx - (INT32)u4PrePktNum;
                    PVR_PIC_INFO_SET_LBA_OFFSET(prPicInfo, i4PktDiff);
                    PVR_PIC_INFO_SET_PIC_TYPE(prPicInfo, 3);
                    PVR_PIC_INFO_SET_PIC_BLKS(prPicInfo, u4PicSize);
    
                    u4EntryNum++;
                }
            }

            PVR_TICK_DETAIL_SET_ENTRIES(prPvrTick, u4EntryNum);

            _pu4MwPVRPicData[0] = _pu4MwPVRPicData[_u4MwPVRPicIdx - 1];
            _u4MwPVRPicIdx = 1;
        }
        else
        {
            // Do nothing
        }
    }

    _MwPvrUnlock();

    // Notify MW
    if(_rMwPVRNotifyInfo.pf_nfy != NULL)
    {
#ifdef LINUX_TURNKEY_SOLUTION
        //PVR_TICK_DATA_T rTickData;
        //PVR_TICK_DATA_T *pt_temp;
        MWPVR_MSG_T rMsg;
        
/*
        rTickData.pv_metadata_tag = (VOID*)u1MetaBufIdx;
        rTickData.ui4_pkt_count = u4PacketNum;

        rTickData.pui1_data = (UINT8*)_MwPvrKerlToUserAddr(prTimeInfo->u4Addr);
        rTickData.pt_metadata = (UINT8*)_MwPvrKerlToUserAddr(prPvrTick);

        lock_nfy_param_buf(PARAM_PVR_NFY);
        pt_temp = (PVR_TICK_DATA_T *)get_nfy_param_buf(PARAM_PVR_NFY, sizeof(PVR_TICK_DATA_T));
        rm_copy_to_user(pt_temp, (UINT32)&rTickData, sizeof(PVR_TICK_DATA_T));
        unlock_nfy_param_buf(PARAM_PVR_NFY);

        LOG(5, "%s pkt:%d, size:0x%x, addr:0x%x\n", __FUNCTION__, u4PacketNum, prTimeInfo->u4Size,
                                            rTickData.pui1_data);

        if(!set_pvr_nfy(PVR_COND_TICK, (UINT32)pt_temp))
        {
            //LOG(3, "%s MW notify return fail\n", __FUNCTION__);
        }
*/
        x_memset((void*)&rMsg, 0, sizeof(MWPVR_MSG_T));

        rMsg.rTickData.pv_metadata_tag = (VOID*)(UINT32)u1MetaBufIdx;
        rMsg.rTickData.ui4_pkt_count = u4PacketNum;

        rMsg.rTickData.pui1_data = (UINT8*)_MwPvrKerlToUserAddr(prTimeInfo->u4Addr);
        rMsg.rTickData.pt_metadata = (void*)_MwPvrKerlToUserAddr((UINT32)prPvrTick);

        LOG(5, "%s pkt:%d, size:0x%x, addr:0x%x\n", __FUNCTION__, u4PacketNum, prTimeInfo->u4Size,
                                            rMsg.rTickData.pui1_data);

        rMsg.eCmd = MWPVRCMD_TIME;
        if (x_msg_q_send(_hPVRMwQueue, (VOID*)&rMsg, sizeof (MWPVR_MSG_T),
                                            PVRMW_PRIORITY_NORMAL) != OSR_OK)
        {
            LOG(3, "PVR MW queue full!\n");
            return;
        }        
#else
        PVR_TICK_DATA_T rTickData;
        rTickData.pv_metadata_tag = (VOID*)u1MetaBufIdx;
        rTickData.ui4_pkt_count = u4PacketNum;
        rTickData.pui1_data = (UINT8*)prTimeInfo->u4Addr;
        rTickData.pt_metadata = (PVR_METADATA_T*)prPvrTick;

        if(((rTickData.pt_metadata->t_tick_detail.ui4_blks_entries) & 0xFFFF) != rTickData.ui4_pkt_count)
        {
            ASSERT(0);
        }
        
        if(!_rMwPVRNotifyInfo.pf_nfy(_rMwPVRNotifyInfo.pv_tag, PVR_COND_TICK, (UINT32)&rTickData))
        {
            LOG(3, "%s MW notify return fail\n", __FUNCTION__);
        }
#endif // LINUX_TURNKEY_SOLUTION
    }

    UNUSED(pvTag);
}


//-----------------------------------------------------------------------------
/** _MwPVRHandler
 */
//-----------------------------------------------------------------------------
static VOID _MwPVRHandler(FVR_GBL_NOTIFY_CODE_T eCode, 
                          UINT32 u4Data, const void* pvTag)
{
    BOOL fgEnable;
    
    _MwPvrLock();
    fgEnable = _fgMwPvrEnable;
    _MwPvrUnlock();
    
    if(!fgEnable)
    {
        return;
    }

    if(eCode == FVR_GBL_NFY_OVERFLOW)
    {
        if(!FVR_Stop())
        {
            LOG(3, "FVR stop error\n");
        }
        
        _MwPvrLock();
        _fgIsOverflow = TRUE;
        _MwPvrUnlock();

        FVR_SetIsOverflow(TRUE);    // DEBUG purpose

        LOG(1, "Stream overflow\n");

        if(_rMwPVRNotifyInfo.pf_nfy != NULL)
        {
#ifdef LINUX_TURNKEY_SOLUTION
            MWPVR_MSG_T rMsg;
            rMsg.eCmd = MWPVRCMD_OVERFLOW;
            rMsg.u4Data1 = PVR_OVERFLOW_REASON_TS_BUF_FULL;
            if (x_msg_q_send(_hPVRMwQueue, (VOID*)&rMsg, sizeof (MWPVR_MSG_T),
                                                PVRMW_PRIORITY_NORMAL) != OSR_OK)
            {
                LOG(3, "PVR MW queue full!\n");
            }

/*
            if(!set_pvr_nfy(PVR_COND_OVERFLOW, PVR_OVERFLOW_REASON_TS_BUF_FULL))
            {
                // UNUSED
            }
*/
#else
            if(!_rMwPVRNotifyInfo.pf_nfy(_rMwPVRNotifyInfo.pv_tag, PVR_COND_OVERFLOW, 
                                         PVR_OVERFLOW_REASON_TS_BUF_FULL))
            {
                LOG(3, "%s MW notify return fail\n", __FUNCTION__);
            }
#endif // LINUX_TURNKEY_SOLUTION
        }        
    }
    else if(eCode == FVR_GBL_NFY_TIME)
    {
        _MwPVRTimeHandler(u4Data, pvTag);
    }
    else if(eCode == FVR_GBL_NFY_PKTCOUNT)
    {
        ASSERT(0);
    }

    UNUSED(pvTag);
}


//-----------------------------------------------------------------------------
/** _MwPVRSetCallback
 */
//-----------------------------------------------------------------------------
static BOOL _MwPVRSetCallback(const PVR_NFY_INFO_T* prNfyInfo)
{
    FVR_GBL_T rGBL;

    if(prNfyInfo == NULL)
    {
        return FALSE;
    }
    
    _rMwPVRNotifyInfo.pf_nfy = prNfyInfo->pf_nfy;
    _rMwPVRNotifyInfo.pv_tag = prNfyInfo->pv_tag;

    x_memset((void*)&rGBL, 0, sizeof(FVR_GBL_T));
    rGBL.pfnNotifyFunc = _MwPVRHandler;
    rGBL.pvNotifyTag = NULL;

    return FVR_SetGBL(FVR_GBL_FLAG_NOTIFY, &rGBL);
}


//-----------------------------------------------------------------------------
/** _MwPVREnablePid
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVREnablePid(UINT8 u1Pidx, const VOID* pv_set_info)
{
    DMX_MW_PVRPID_INFO_T *prPidInfo;
    FVR_PID_T rPid;
    BOOL fgEnable;
    UINT32 u4Flags;

    if (u1Pidx >= (PVR_STREAM_COMP_NUM + PVR_STREAM_PCR_COMP_NUM))
    {
        return RMR_INV_SET;
    }

    fgEnable = (BOOL)(UINT32)pv_set_info;

    rPid.fgEnable = fgEnable;
    u4Flags = FVR_PID_FLAG_ENABLE;

    prPidInfo = _MW_Rec_GetPidInfo(u1Pidx);
    if(prPidInfo == NULL)
    {
        return RMR_INV_SET;
    }

    if(fgEnable)
    {
        if(!prPidInfo->fgValid)
        {
            return RMR_INV_SET;
        }

        rPid.u2Pid = prPidInfo->u2PidNum;
        u4Flags |= FVR_PID_FLAG_PID;

        if(prPidInfo->rNfyInfo.pf_recdemux_nfy != NULL)
        {
            rPid.fgDetectScramble = TRUE;
            rPid.pfnDetectScrambleNotify = _MwPVRDectectScrambleHandler;
            if(((prPidInfo->u4TypeMask & ST_MASK_AUDIO) != 0) ||
               ((prPidInfo->u4TypeMask & ST_MASK_VIDEO) != 0)) 
            {
                ; //rPid.fgIsAVScramble = TRUE;
            }
            else
            {
                ; //rPid.fgIsAVScramble = FALSE;
            }
        }
        else
        {
            rPid.fgDetectScramble = FALSE;
            rPid.pfnDetectScrambleNotify = NULL;
            //rPid.fgIsAVScramble = FALSE;
            //rPid.u1PerPidIdx = 0x0;
        }

        u4Flags |= FVR_PID_FLAG_DETECT_SCRAMBLE;
    }

    //Printf("_MwPVREnablePid, Pidx:%d, enable:%d\n", u1Pidx, fgEnable);
    
    if(!_MwPvrSetPid(u1Pidx, u4Flags, &rPid))
    {
        return RMR_INV_SET;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVROutputPid
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVROutputPid(UINT8 u1Pidx, const VOID* pv_set_info)
{
    FVR_PID_T rPid;
    BOOL fgEnable;
    UINT32 u4Flags;

    if (u1Pidx >= (PVR_STREAM_COMP_NUM + PVR_STREAM_PCR_COMP_NUM))
    {
        return RMR_INV_SET;
    }

    fgEnable = (BOOL)(UINT32)pv_set_info;

    rPid.fgFreeze = !fgEnable;
    u4Flags = FVR_PID_FLAG_FREEZE;
    
    if(!_MwPvrSetPid(u1Pidx, u4Flags, &rPid))
    {
        return RMR_INV_SET;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVREnPicSearch
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVREnPicSearch(UINT8 u1Pidx, const VOID* pv_set_info)
{
    FVR_PID_T rPid;
    BOOL fgEnable;
    UINT8 u1PerPidx = 0xFF;

    if (u1Pidx >= (PVR_STREAM_COMP_NUM + PVR_STREAM_PCR_COMP_NUM))
    {
        return RMR_INV_SET;
    }

    fgEnable = (BOOL)(UINT32)pv_set_info;

    rPid.fgPicSearch = fgEnable;
    rPid.u1PicSearchMask = FVR_PIC_I_MASK | FVR_PIC_P_MASK | FVR_PIC_B_MASK;
    rPid.pfnPicSearchNotify = _MwPVRPicHandler;
    rPid.pvPicSearchNotifyTag = NULL;

    u1PerPidx = _MwPvrAllocPerPidx(u1Pidx);
    if(u1PerPidx == 0xFF)
    {
        return RMR_INV_SET;
    }
    //rPid.u1PerPidIdx = u1PerPidx;
    
    if(!_MwPvrSetPid(u1Pidx, FVR_PID_FLAG_PICSEARCH | FVR_PID_FLAG_DETECT_SCRAMBLE, &rPid))
    {
        return RMR_INV_SET;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRSetSwap
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRSetSwap(UINT8 u1Pidx, const PVR_SWAP_INFO_T* prSwapInfo)
{
    FVR_PID_T rPid;
    const PVR_SECT_DATA_T* pSecData;    
#ifdef LINUX_TURNKEY_SOLUTION
    PVR_SECT_DATA_T rSecData;
#endif // LINUX_TURNKEY_SOLUTION

    if (u1Pidx >= (PVR_STREAM_COMP_NUM + PVR_STREAM_PCR_COMP_NUM))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if(prSwapInfo == NULL)
    {
        return RMR_INV_SET;
    }

    if(prSwapInfo->e_swap_type == PVR_SWAP_TYPE_DISABLE)
    {
        rPid.fgPktSwap = FALSE;
        if(!_MwPvrSetPid(u1Pidx, FVR_PID_FLAG_PKTSWAP, &rPid))
        {
            return RMR_INV_SET;
        }

        if(!FVR_SwapFreePattern(u1Pidx))
        {
            return RMR_INV_SET;
        }

        return RMR_OK;
    }
    else if(prSwapInfo->e_swap_type == PVR_SWAP_TYPE_NULL_PKT)
    {
        if(!FVR_SwapSetNullPattern(u1Pidx))
        {
            return RMR_INV_SET;
        }
    }
    else if(prSwapInfo->e_swap_type == PVR_SWAP_TYPE_SECT_LIST)
    {
        pSecData = prSwapInfo->u.t_sect_list.at_sects;

#ifdef LINUX_TURNKEY_SOLUTION
        rm_copy_from_user(&rSecData, (VOID *)pSecData, sizeof(PVR_SECT_DATA_T));

        if (u4PvrSwapMaxLen < ((UINT32)rSecData.z_sect_len))
        {
            if (_pu1PvrSwapAddr != NULL)
            {
                x_mem_free(_pu1PvrSwapAddr);
                _pu1PvrSwapAddr = NULL;
            }
            u4PvrSwapMaxLen = ((UINT32)rSecData.z_sect_len);
            _pu1PvrSwapAddr = (UINT8 *)x_mem_alloc((UINT32)rSecData.z_sect_len);
        }
        if (_pu1PvrSwapAddr == NULL)
        {
            u4PvrSwapMaxLen = 0;
            return RMR_INV_SET;
        }

        rm_copy_from_user(_pu1PvrSwapAddr, rSecData.pv_sect_data, rSecData.z_sect_len);

        if(!FVR_SwapSetPattern(u1Pidx, (UINT8*)_pu1PvrSwapAddr, 
                           (UINT16)rSecData.z_sect_len))
        {
            return RMR_INV_SET;
        }
#else
        if(!FVR_SwapSetPattern(u1Pidx, (UINT8*)pSecData->pv_sect_data, 
                           (UINT16)pSecData->z_sect_len))
        {
            return RMR_INV_SET;
        }
#endif // LINUX_TURNKEY_SOLUTION
    }

    rPid.fgPktSwap = TRUE;
    rPid.pfnSwapNotify = NULL;
    rPid.pvSwapNotifyTag = NULL;
    if(!_MwPvrSetPid(u1Pidx, FVR_PID_FLAG_PKTSWAP, &rPid))
    {
        return RMR_INV_SET;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRSetEncryption
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRSetEncryption(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info, UINT8 u1Pidx)
{
    MM_CRYPT_INFO_T *ptEncryptInfo;
    FVR_PID_T rPid;
    UINT32 i, u4Flags, u4KeyLen;
    UINT8 u1KeyIdx, au1OddKey[16], au1EvenKey[16];
    BOOL fgMatch = FALSE;

    ptEncryptInfo = (MM_CRYPT_INFO_T*)pv_set_info;

    if(ptEncryptInfo == NULL)
    {
        return RMR_INV_ARG;
    }

    if(!_MwPVRAllocateRecKeyIdx(ptEncryptInfo, &fgMatch, &u1KeyIdx))
    {
        return RMR_INV_ARG;
    }

    if((ptEncryptInfo->u.t_pvr_crypt.e_mode == MM_CRYPT_MODE_DES_CBC) ||
       (ptEncryptInfo->u.t_pvr_crypt.e_mode == MM_CRYPT_MODE_3_DES_CBC))
    {
        LOG(3, "%s,%d not support CBC mode\n", __FUNCTION__, __LINE__);
        return RMR_INV_ARG;
    }

    //-------------------------------------------
    // Set key
    //-------------------------------------------
    u4KeyLen = 128;
    x_memset((void*)au1OddKey, 0, sizeof(au1OddKey));
    x_memset((void*)au1EvenKey, 0, sizeof(au1EvenKey));
    
    if((ptEncryptInfo->u.t_pvr_crypt.e_key_idx == MM_CRYPT_KEY_ALTERNATE) ||
       (ptEncryptInfo->u.t_pvr_crypt.e_key_idx == MM_CRYPT_KEY_ODD))
    {
        for(i=0; i<4; i++)
        {
            au1OddKey[i] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_1[0], 3 - i);
        }

        for(i=0; i<4; i++)
        {
            au1OddKey[i+4] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_1[1], 3 - i);
        }
        
        if(ptEncryptInfo->u.t_pvr_crypt.e_mode == MM_CRYPT_MODE_3_DES_EBC)
        {
            for(i=0; i<4; i++)
            {
                au1OddKey[i+8] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_2[0], 3 - i);
            }
            for(i=0; i<4; i++)
            {
                au1OddKey[i+12] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_2[1], 3 - i);
            }
        }        

        if(!FVR_SetAesKey(u1KeyIdx, FALSE, au1OddKey))
        {
            return RMR_INV_ARG;
        }
    }
       
    if((ptEncryptInfo->u.t_pvr_crypt.e_key_idx == MM_CRYPT_KEY_ALTERNATE) ||
       (ptEncryptInfo->u.t_pvr_crypt.e_key_idx == MM_CRYPT_KEY_EVEN))
    {
        for(i=0; i<4; i++)
        {
            au1EvenKey[i] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_1[0], 3 - i);
        }

        for(i=0; i<4; i++)
        {
            au1EvenKey[i+4] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_1[1], 3 - i);
        }
        
        if(ptEncryptInfo->u.t_pvr_crypt.e_mode == MM_CRYPT_MODE_3_DES_EBC)
        {
            for(i=0; i<4; i++)
            {
                au1EvenKey[i+8] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_2[0], 3 - i);
            }
            for(i=0; i<4; i++)
            {
                au1EvenKey[i+12] = GET_BYTE(ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_2[1], 3 - i);
            }
        }

        if(!FVR_SetAesKey(u1KeyIdx, TRUE, au1EvenKey))
        {
            return RMR_INV_ARG;
        }
    }
    
    switch(ptEncryptInfo->u.t_pvr_crypt.e_key_idx)
    {
    case MM_CRYPT_KEY_ODD:
        rPid.eKeyType = FVR_KEY_ODD;
        break;

    case MM_CRYPT_KEY_EVEN:
        rPid.eKeyType = FVR_KEY_EVEN;
        break;
    
    case MM_CRYPT_KEY_ALTERNATE:
        rPid.eKeyType = FVR_KEY_BOTH;
        break;

    default:
        return RMR_INV_ARG;
    }

    rPid.fgCBCMode = FALSE;
    rPid.u4KeyLen = u4KeyLen;
    rPid.u1KeyIdx = u1KeyIdx;
    rPid.fgScramble = TRUE;
    //rPid.fgScramble = FALSE;

    u4Flags = (FVR_PID_FLAG_CBC_MODE | FVR_PID_FLAG_KEY_TYPE | 
               FVR_PID_FLAG_SCRAMBLE_ENABLE | FVR_PID_FLAG_KEY_LEN);

    if(!_MwPvrSetPid(u1Pidx, u4Flags, &rPid))
    {
        return RMR_INV_SET;
    }

    //-------------------------------------------
    // Store key
    //-------------------------------------------
    if(!fgMatch)
    {
        if(!_MwPVRSaveRecKey(ptEncryptInfo, u1KeyIdx))
        {
            return RMR_INV_SET;
        }
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRUnlockBuffer
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRUnlockBuffer(DRV_COMP_ID_T* pt_comp_id,
                              PVR_UNLOCK_BUFFER_INFO_T* prBuffer) 
{
    UINT32 u4Addr, u4Size, u4KernelAddr;

    if((pt_comp_id == NULL) || (prBuffer == NULL))
    {
        return RMR_INV_SET;
    }

    u4Addr = (UINT32)prBuffer->pui1_data_addr;
    u4Size = (UINT32)prBuffer->z_data_len;

#ifdef LINUX_TURNKEY_SOLUTION
    u4KernelAddr = _MwPvrUserToKerlAddr(u4Addr);
#else
    u4KernelAddr = u4Addr;
#endif // LINUX_TURNKEY_SOLUTION

    LOG(5, "PVR unlock buffer, addr:0x%x, size:0x%x\n", u4KernelAddr, u4Size);
        
    if(!FVR_UnlockBuffer(u4KernelAddr, u4Size))
    {
        return RMR_INV_SET;
    }
        
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRStream_Connect
 *  Middleware interface - connect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStream_Connect(
                            const DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    FVR_PID_T rPid;
    UINT32 u4Flags;
    UINT8 u1Pidx;
    
    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_COMP_NUM)
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    
    x_memset((VOID*)&rPid, 0, sizeof(FVR_PID_T));
    rPid.fgEnable = FALSE;
    rPid.u2Pid = 0;
    rPid.fgScramble = FALSE;    
    rPid.fgPktSwap = FALSE;

    rPid.fgPicSearch = FALSE;
    rPid.u1PicSearchMask = 0;
    rPid.pfnPicSearchNotify = NULL;
    rPid.pvPicSearchNotifyTag = NULL;

    rPid.u1TsIndex = 0;

    u4Flags = FVR_PID_FLAG_PID | FVR_PID_FLAG_PICSEARCH | FVR_PID_FLAG_PKTSWAP |
              FVR_PID_FLAG_SCRAMBLE_ENABLE | FVR_PID_FLAG_TSINDEX;

    if(!_MwPvrSetPid(u1Pidx, u4Flags, &rPid))
    {
        return RMR_DRV_CONN_FAILED;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRStream_Disconnect
 *  Middleware interface - disconnect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStream_Disconnect(
                            const DRV_COMP_ID_T*    pt_comp_id,
                            DRV_DISC_TYPE_T         e_disc_type,
                            const VOID*             pv_disc_info,
                            SIZE_T                  z_disc_info_len)
{
    UINT8 u1Pidx, i;
    
    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_COMP_NUM)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if(!FVR_FreePid(u1Pidx))
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
    {
        if(_arMwPvrPerPidxUsage[i].fgEnable && (_arMwPvrPerPidxUsage[i].u1Pidx == u1Pidx))
        {
            _arMwPvrPerPidxUsage[i].fgEnable = FALSE;
            _arMwPvrPerPidxUsage[i].u1Pidx = 0xFF;
        }
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRStream_Get
 *  Middleware interface - get
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStream_Get(
                        const DRV_COMP_ID_T*    pt_comp_id,
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
{
    FVR_GBL_T rGBL;
    PVR_BUFFER_STATUS_T *prBufStatus;    
    FVR_PID_T rPid;
    UINT8 u1Pidx;

    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) )
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_COMP_NUM)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch (e_get_type)
    {
    case PVR_GET_TYPE_CTRL:
        if(!FVR_GetPid(u1Pidx, FVR_PID_FLAG_ENABLE, &rPid))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        *(BOOL*)pv_get_info = rPid.fgEnable;
        *pz_get_info_len = sizeof(BOOL);
        break;

    case PVR_GET_TYPE_BUFFER_STATUS:
        if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        *pz_get_info_len = sizeof(PVR_BUFFER_STATUS_T);
        prBufStatus = (PVR_BUFFER_STATUS_T*)pv_get_info;
#ifdef LINUX_TURNKEY_SOLUTION
        prBufStatus->pui1_read_ptr = (UINT8*)_MwPvrKerlToUserAddr(rGBL.u4Rp);
        prBufStatus->pui1_write_ptr = (UINT8*)_MwPvrKerlToUserAddr(rGBL.u4Wp);        
#else
        prBufStatus->pui1_read_ptr = (UINT8*)rGBL.u4Rp;
        prBufStatus->pui1_write_ptr = (UINT8*)rGBL.u4Wp;
#endif
        break;

    default:
        break;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRStream_Set
 *  Middleware interface - set
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStream_Set(
                    DRV_COMP_ID_T*      pt_comp_id,
                    DRV_SET_TYPE_T      e_set_type,
                    const VOID*         pv_set_info,
                    SIZE_T              z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UINT8 u1Pidx;

    if ((NULL == pt_comp_id))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_COMP_NUM)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
    case PVR_SET_TYPE_CTRL:
        i4Ret = _MwPVREnablePid(u1Pidx, pv_set_info);        
        break;

    case PVR_SET_TYPE_CTRL_OUTPUT:
        i4Ret = _MwPVROutputPid(u1Pidx, pv_set_info);
        break;

    case PVR_SET_TYPE_EN_PIC_SEARCH:
        i4Ret = _MwPVREnPicSearch(u1Pidx, pv_set_info);
        break;

    case PVR_SET_TYPE_SWAP:
        i4Ret = _MwPVRSetSwap(u1Pidx, (PVR_SWAP_INFO_T*)pv_set_info);
        break;

    case PVR_SET_TYPE_ENCRYPTION:        
        i4Ret = _MwPVRSetEncryption(pt_comp_id, pv_set_info, u1Pidx);            
        break;

    default:
        break;
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** _MwPVRStreamPCR_Connect
 *  Middleware interface - connect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStreamPCR_Connect(
                            const DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    FVR_PID_T rPid;
    UINT32 u4Flags;
    UINT8 u1Pidx;
    
    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    u1Pidx += PVR_STREAM_COMP_NUM;
    
    x_memset((VOID*)&rPid, 0, sizeof(FVR_PID_T));
    rPid.fgEnable = FALSE;
    rPid.u2Pid = 0;
    rPid.fgScramble = FALSE;    
    rPid.fgPktSwap = FALSE;

    rPid.fgPicSearch = FALSE;
    rPid.u1PicSearchMask = 0;
    rPid.pfnPicSearchNotify = NULL;
    rPid.pvPicSearchNotifyTag = NULL;

    u4Flags = FVR_PID_FLAG_PID | FVR_PID_FLAG_PICSEARCH | FVR_PID_FLAG_PKTSWAP |
              FVR_PID_FLAG_SCRAMBLE_ENABLE;

    if(!_MwPvrSetPid(u1Pidx, u4Flags, &rPid))
    {
        return RMR_DRV_CONN_FAILED;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRStreamPCR_Disconnect
 *  Middleware interface - disconnect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStreamPCR_Disconnect(
                            const DRV_COMP_ID_T*    pt_comp_id,
                            DRV_DISC_TYPE_T         e_disc_type,
                            const VOID*             pv_disc_info,
                            SIZE_T                  z_disc_info_len)
{
    UINT8 u1Pidx;
    
    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    u1Pidx += PVR_STREAM_COMP_NUM;

    if(!FVR_FreePid(u1Pidx))
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRStreamPCR_Get
 *  Middleware interface - get
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStreamPCR_Get(
                        const DRV_COMP_ID_T*    pt_comp_id,
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
{
    FVR_GBL_T rGBL;
    PVR_BUFFER_STATUS_T *prBufStatus;    
    FVR_PID_T rPid;
    UINT8 u1Pidx;

    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) )
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    u1Pidx += PVR_STREAM_COMP_NUM;

    switch (e_get_type)
    {
    case PVR_GET_TYPE_CTRL:
        if(!FVR_GetPid(u1Pidx, FVR_PID_FLAG_ENABLE, &rPid))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        *(BOOL*)pv_get_info = rPid.fgEnable;
        *pz_get_info_len = sizeof(BOOL);
        break;

    case PVR_GET_TYPE_BUFFER_STATUS:
        if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        prBufStatus = (PVR_BUFFER_STATUS_T*)pv_get_info;
        *pz_get_info_len = sizeof(PVR_BUFFER_STATUS_T);
        prBufStatus = (PVR_BUFFER_STATUS_T*)pv_get_info;      
#ifdef LINUX_TURNKEY_SOLUTION
        prBufStatus->pui1_read_ptr = (UINT8*)_MwPvrKerlToUserAddr(rGBL.u4Rp);
        prBufStatus->pui1_write_ptr = (UINT8*)_MwPvrKerlToUserAddr(rGBL.u4Wp);        
#else
        prBufStatus->pui1_read_ptr = (UINT8*)rGBL.u4Rp;
        prBufStatus->pui1_write_ptr = (UINT8*)rGBL.u4Wp;
#endif
        break;

    default:
        break;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRStreamPCR_Set
 *  Middleware interface - set
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRStreamPCR_Set(
                    DRV_COMP_ID_T*      pt_comp_id,
                    DRV_SET_TYPE_T      e_set_type,
                    const VOID*         pv_set_info,
                    SIZE_T              z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UINT8 u1Pidx;

    if ((NULL == pt_comp_id))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u1Pidx += PVR_STREAM_COMP_NUM;

    switch (e_set_type)
    {
    case PVR_SET_TYPE_CTRL:
        i4Ret = _MwPVREnablePid(u1Pidx, pv_set_info);        
        break;

    case PVR_SET_TYPE_CTRL_OUTPUT:
        i4Ret = _MwPVROutputPid(u1Pidx, pv_set_info);
        break;

    case PVR_SET_TYPE_EN_PIC_SEARCH:
        //i4Ret = RMR_DRV_INV_SET_INFO;
        break;

    case PVR_SET_TYPE_SWAP:
        //i4Ret = RMR_DRV_INV_SET_INFO;
        break;

    case PVR_SET_TYPE_ENCRYPTION:        
        //i4Ret = RMR_DRV_INV_SET_INFO;
        break;

    default:
        break;
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** _MwPVRCtrl_Connect
 *  Middleware interface - connect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRCtrl_Connect(
                            const DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    
    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    _u4MwPVRPktCount = 0;

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRCtrl_Disconnect
 *  Middleware interface - disconnect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRCtrl_Disconnect(
                            const DRV_COMP_ID_T*    pt_comp_id,
                            DRV_DISC_TYPE_T         e_disc_type,
                            const VOID*             pv_disc_info,
                            SIZE_T                  z_disc_info_len)
{
    
    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    // Double confirm
    if(!FVR_Stop())
    {
        return RMR_DRV_INV_CONN_INFO; 
    }

    if(!FVR_FreeGBL())
    {
        return RMR_DRV_INV_CONN_INFO; 
    }

    _rMwPVRNotifyInfo.pf_nfy = NULL;
    _rMwPVRNotifyInfo.pv_tag = NULL;

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRCtrl_Get
 *  Middleware interface - get
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRCtrl_Get(
                        const DRV_COMP_ID_T*    pt_comp_id,
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
{
    FVR_GBL_T rGBL;
    PVR_BUFFER_INFO_T* prBufInfo;
    PVR_BUFFER_STATUS_T *prBufStatus;
    UINT8 u1Pidx;
    UINT32 u4Addr, u4Size, u4Index;
    PVR_RESIDUAL_DATA_INFO_T* prResidual;
    PVR_CAP_INFO_T *prCapInfo;

    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) )
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= PVR_STREAM_COMP_NUM)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch (e_get_type)
    {
    case PVR_GET_TYPE_CTRL:
        _MwPvrLock();
        *(BOOL*)pv_get_info = _fgMwPvrEnable;
        _MwPvrUnlock();
        *pz_get_info_len = sizeof(BOOL);        
        break;

    case PVR_GET_TYPE_BUFFER_INFO:
        if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        prBufInfo = (PVR_BUFFER_INFO_T*)pv_get_info;
#ifdef LINUX_TURNKEY_SOLUTION
        prBufInfo->pui1_buf_addr = (UINT8*)_MwPvrKerlToUserAddr(rGBL.u4BufStart);
#else
        prBufInfo->pui1_buf_addr = (UINT8*)rGBL.u4BufStart;
#endif // LINUX_TURNKEY_SOLUTION

        prBufInfo->z_buf_len = (SIZE_T)rGBL.u4BufSize;
        *pz_get_info_len = sizeof(PVR_BUFFER_INFO_T);
        break;

    case PVR_GET_TYPE_BUFFER_STATUS:
        if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        prBufStatus = (PVR_BUFFER_STATUS_T*)pv_get_info;
#ifdef LINUX_TURNKEY_SOLUTION
        prBufStatus->pui1_read_ptr = (UINT8*)_MwPvrKerlToUserAddr(rGBL.u4Rp);
        prBufStatus->pui1_write_ptr = (UINT8*)_MwPvrKerlToUserAddr(rGBL.u4Wp);
#else
        prBufStatus->pui1_read_ptr = (UINT8*)rGBL.u4Rp;
        prBufStatus->pui1_write_ptr = (UINT8*)rGBL.u4Wp;
#endif // LINUX_TURNKEY_SOLUTION
        *pz_get_info_len = sizeof(PVR_BUFFER_STATUS_T);        
        break;

    case PVR_GET_TYPE_RESIDUAL_DATA:
        if(!FVR_GetResidual(&u4Addr, &u4Size, &u4Index))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        prResidual = (PVR_RESIDUAL_DATA_INFO_T*)pv_get_info;        
        *pz_get_info_len = sizeof(PVR_RESIDUAL_DATA_INFO_T);     

#ifdef LINUX_TURNKEY_SOLUTION
        prResidual->pui1_addr = (UINT8*)_MwPvrKerlToUserAddr(u4Addr);
#else
        prResidual->pui1_addr = (UINT8*)u4Addr;
#endif // LINUX_TURNKEY_SOLUTION

        prResidual->z_len = (SIZE_T)u4Size;
        UNUSED(u4Index);
        break;

    case PVR_GET_TYPE_CAP_INFO:
        prCapInfo = (PVR_CAP_INFO_T*)pv_get_info;
        *pz_get_info_len = sizeof(PVR_CAP_INFO_T); 
        prCapInfo->ui4_align_buf_start = FVR_FIFO_ALIGNMENT;
        prCapInfo->ui4_align_buf_size = FVR_FIFO_ALIGNMENT;
        prCapInfo->ui4_max_encrypt_key_num = FVR_SCRAMBLE_PID_INDEX;
        prCapInfo->ui4_max_encrypt_pid_num = FVR_NUM_PID_INDEX;
        prCapInfo->ui4_max_pid_num = FVR_NUM_PID_INDEX;
        break;

    default:
        break;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwPVRInitCtrl
 */
//-----------------------------------------------------------------------------
static BOOL _MwPVRInitCtrl(PVR_CTRL_CONFIG_T *prConfig)
{
    FVR_PID_T rFvrPid;
    FVR_GBL_T rGBL;
    UINT32 i;

    if(prConfig == NULL)
    {
        return FALSE;
    }

    _fgIsOverflow=FALSE;
	_u4MwPVRTickNum=0;
    _u4MwPVRPktCount = 0;
	_u4MwPVRPicIdx=0;
	_fgMwPvrEnable=FALSE;

    // Debug purpose
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(!FVR_GetPid(i, FVR_PID_FLAG_ENABLE, &rFvrPid))
        {
            return FALSE;
        }
        if(rFvrPid.fgEnable)
        {
            LOG(3, "There is PID enable\n");
            ASSERT(0);
        }
    }

    for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
    {
        _arMwPvrPerPidxUsage[i].fgEnable = FALSE;
        _arMwPvrPerPidxUsage[i].u1Pidx = 0xFF;
    }

    //------------------------------------------
    // Buffer
    //------------------------------------------
    if(!FVR_FreeBuffer())
    {
        return FALSE; 
    }
    
    x_memset((VOID*)&rGBL, 0x0, sizeof(FVR_GBL_T));
    rGBL.fgAllocateBuffer = FALSE;
#ifdef LINUX_TURNKEY_SOLUTION
    _u4MwPvrStreamPhyAddr = (UINT32)prConfig->pv_buf_phy;
    _u4MwPvrStreamMwAddr = (UINT32)prConfig->pv_buf_mw;
    _u4MwPvrMetaPhyAddr = (UINT32)(prConfig->pv_buf_phy + (prConfig->z_buf_size - PVRMW_TICK_METABUF_SIZE));
    _u4MwPvrKelTotalSize = prConfig->z_buf_size;

    rGBL.fgSetAlign = TRUE;
    rGBL.u4BufAlign = 1536;//FVR_FIFO_ALIGNMENT;
    rGBL.u4BufStart = VIRTUAL((UINT32)prConfig->pv_buf_phy);
    rGBL.u4BufSize = prConfig->z_buf_size - PVRMW_TICK_METABUF_SIZE;
#else
    rGBL.fgSetAlign = TRUE;
    rGBL.u4BufAlign = FVR_FIFO_ALIGNMENT;
    rGBL.u4BufStart = (UINT32)prConfig->pv_buf_phy;//rGBL.u4BufStart = (UINT32)prConfig->pv_buf;
    rGBL.u4BufSize = prConfig->z_buf_size;
#endif

    rGBL.u4TimeToNotify = prConfig->ui4_tick_period;

    if(((rGBL.u4BufStart % FVR_FIFO_ALIGNMENT) != 0) || ((rGBL.u4BufSize % FVR_FIFO_ALIGNMENT) != 0))
    {
        return FALSE;
    }

    if(!FVR_SetGBL(FVR_GBL_FLAG_BUFFER | FVR_GBL_FLAG_TIME_INFO, &rGBL))
    {
        return FALSE;
    }

    if((UINT8)prConfig->ui1_nfy_frm_mask != (UINT8)PVR_FRAME_TYPE_MASK_I)
    {
        // Current only support I mask
        return FALSE;
    }

#ifdef LINUX_TURNKEY_SOLUTION
    for(i=0; i<MW_FVR_META_BUF_NUM; i++)
    {
        _arMwPVRMetaBuf[i].u4BufAddr = 
             VIRTUAL(_u4MwPvrMetaPhyAddr + (i * (PVRMW_TICK_PIC_MAX * 4)));
    }
#endif //  LINUX_TURNKEY_SOLUTION

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _MwPVRCtrl_Set
 *  Middleware interface - set
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwPVRCtrl_Set(
                    DRV_COMP_ID_T*      pt_comp_id,
                    DRV_SET_TYPE_T      e_set_type,
                    const VOID*         pv_set_info,
                    SIZE_T              z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    BOOL fgEnable, fgRet, fgIsOverflow;
//    FVR_GBL_T rGBL;
//    MM_CRYPT_IV_T *pEncIV;
    UINT32 i;

    if ((NULL == pt_comp_id))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
    case PVR_SET_TYPE_NFY_FCT:
        fgRet = _MwPVRSetCallback((PVR_NFY_INFO_T*)pv_set_info);
        i4Ret = fgRet ? RMR_OK : RMR_INV_SET;
        break;
        
    case PVR_SET_TYPE_IV:
        ASSERT(0);
//        pEncIV = (MM_CRYPT_IV_T*)pv_set_info;
//        rGBL.u4CBCIVLow = pEncIV->aui4_init_vector[0];
//        rGBL.u4CBCIVHigh = pEncIV->aui4_init_vector[1];
//        fgRet = FVR_SetGBL(FVR_GBL_FLAG_CBC_IV, &rGBL);
//        i4Ret = fgRet ? RMR_OK : RMR_INV_SET;
        break;

    case PVR_SET_TYPE_CTRL:
        fgEnable = (BOOL)(UINT32)pv_set_info;        
        LOG(3, "%s, PVR_SET_TYPE_CTRL: %d\n", __FUNCTION__, (INT32)fgEnable);
        if(fgEnable)
        {
            for(i=0; i<MW_FVR_META_BUF_NUM; i++)
            {
                _arMwPVRMetaBuf[i].fgValid = FALSE;
                _arMwPVRMetaBuf[i].u4SerialNum = 0;
            }
            _MwPvrSetTimestamp();
            fgRet = FVR_Start();            
            if(fgRet)
            {
                _MwPvrLock();
                _fgMwPvrEnable = TRUE;
                _fgIsOverflow = FALSE;
                _MwPvrUnlock();
            }
        }
        else
        {
            fgRet = FVR_Stop();
            if(fgRet)
            {
                _MwPvrLock();
                _fgMwPvrEnable = FALSE;
                _MwPvrUnlock();
            }
        }
        i4Ret = fgRet ? RMR_OK : RMR_INV_SET;       
        break;
        
    case PVR_SET_TYPE_UNLOCK_BUFFER:
        i4Ret = _MwPVRUnlockBuffer(pt_comp_id, (PVR_UNLOCK_BUFFER_INFO_T*)pv_set_info);
        break;

    case PVR_SET_TYPE_FLUSH_BUFFER:
        LOG(3, "PVR flush buffer\n");
        if(FVR_IsRecording())
        {
            LOG(1, "%s, PVR_SET_TYPE_FLUSH_BUFFER still recording\n", __FUNCTION__);
            return RMR_INV_SET;
        }

        if(!FVR_FlushBuffer())
        {
            return RMR_INV_SET;
        }
        _MwPvrLock();
        fgEnable = _fgMwPvrEnable;
        fgIsOverflow = _fgIsOverflow;
        _MwPvrUnlock();        
        if(fgEnable)
        {
            if(!fgIsOverflow)
            {                
                LOG(3, "Oops, should overflow\n");
            }
            LOG(3, "PVR flush buffer start...\n");
            _MwPvrSetTimestamp();
            if(!FVR_Start())
            {
                return RMR_INV_SET;
            }            
        }
        _MwPvrLock();
        _u4MwPVRPktCount = 0;
        _fgIsOverflow = FALSE;
        _MwPvrUnlock();

        FVR_SetIsOverflow(FALSE);
        
        i4Ret = RMR_OK;
        break;

    case PVR_SET_TYPE_UNLOCK_METADATA:
        LOG(5, "free meta: %d\n", (UINT32)pv_set_info);
        fgRet = _MwPVRFreeMetaBuf((UINT8)(UINT32)pv_set_info);
        i4Ret = fgRet ? RMR_OK : RMR_INV_SET;
        break;

    case PVR_SET_TYPE_INIT_CTRL:
        fgRet = _MwPVRInitCtrl((PVR_CTRL_CONFIG_T*)pv_set_info);
        i4Ret = fgRet ? RMR_OK : RMR_INV_SET;
        break;

    case PVR_SET_TYPE_DEINIT_CTRL:
        if(!_MwPVRResetRecKeyInfo())
        {
            return RMR_INV_SET;
        }
        if(!FVR_FreeGBL())
        {
            return RMR_INV_SET;
        }

        for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
        {
            _arMwPvrPerPidxUsage[i].fgEnable = FALSE;
            _arMwPvrPerPidxUsage[i].u1Pidx = 0xFF;
        }
        break;

    default:
        break;
    }

    return i4Ret;
}

#ifdef LINUX_TURNKEY_SOLUTION
//-----------------------------------------------------------------------------
/** _PVRMwThread
 */
//-----------------------------------------------------------------------------
static VOID _PVRMwThread(VOID* pvArg)
{
    static MWPVR_MSG_T rMsg;
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    /**PVR_TICK_DATA_T *pt_temp;*/
    DMX_MW_PVRPID_INFO_T *prPidInfo;
    
    UNUSED(pvArg);

    while (1)
    {
        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
                        &_hPVRMwQueue, 1, X_MSGQ_OPTION_WAIT);

        if (i4Ret != OSR_OK)
        {
            LOG(1, "Error on receiving pvrmw message!\n");
            continue;
        }

        switch(rMsg.eCmd)
        {
        case MWPVRCMD_TIME: 
            #if 0
            lock_nfy_param_buf(PARAM_PVR_NFY);
            pt_temp = (PVR_TICK_DATA_T *)get_nfy_param_buf(PARAM_PVR_NFY, sizeof(PVR_TICK_DATA_T));
            i4Ret = rm_copy_to_user(pt_temp, (UINT32)&rMsg.rTickData, sizeof(PVR_TICK_DATA_T));
            if (i4Ret != 0)
            {
                LOG(5, "Error on rm_copy_to_user(i4Ret=%d)!\n", i4Ret);
                ASSERT(0);
            }
            unlock_nfy_param_buf(PARAM_PVR_NFY);
            #endif
            LOG(5, "Time: idx:%d, pkt:%d, size:0x%x, addr:0x%x\n",
                                rMsg.rTickData.pv_metadata_tag,
                                rMsg.rTickData.ui4_pkt_count, (rMsg.rTickData.ui4_pkt_count * 192),
                                rMsg.rTickData.pui1_data);
            if(!set_pvr_nfy(PVR_COND_TICK, (UINT32)&(rMsg.rTickData)))
            {
                //LOG(3, "%s MW notify return fail\n", __FUNCTION__);
            }            
            break;

        case MWPVRCMD_OVERFLOW:
            if(!set_pvr_nfy(PVR_COND_OVERFLOW, rMsg.u4Data1))
            {
                // UNUSED
            }
            break;

        case MWPVRCMD_SCRAMBLE:            
            prPidInfo = _MW_Rec_GetPidInfo(rMsg.u1Pidx);
            if(prPidInfo != NULL)
            {
                if(!set_pvr_scramble_nfy(prPidInfo->rNfyInfo.pv_tag, RECDEMUX_COND_SCRS,  
                                         (UINT32)rMsg.u4Data1,
                                         prPidInfo->rNfyInfo.pf_recdemux_nfy))
                {
                }
            }
            break;

         default:
            break;
        }        
    }

}
#endif // LINUX_TURNKEY_SOLUTION


//-----------------------------------------------------------------------------
/** _MwPVRInit
 */
//-----------------------------------------------------------------------------
static BOOL _MwPVRInit(void)
{
    static BOOL _fgInit = FALSE;
    UINT32 i, u4Addr;

    LOG(3, "%s\n", __FUNCTION__);

    if(!FVR_Init())
    {
        return FALSE;
    }

    //------------------------------------------------
    // Init global variables
    //------------------------------------------------
    if(!_MwPVRResetRecKeyInfo())
    {
        return FALSE;
    }

    _fgIsOverflow = FALSE;
    _fgMwPvrEnable = FALSE;
    _rMwPVRNotifyInfo.pf_nfy = NULL;
    _rMwPVRNotifyInfo.pv_tag = NULL;
    for(i=0; i<MW_FVR_META_BUF_NUM; i++)
    {
        _arMwPVRMetaBuf[i].fgValid = FALSE;
        _arMwPVRMetaBuf[i].u4SerialNum = 0;
    }

    if(!_fgInit)
    {
        _pu4MwPVRPicData = (UINT32*)x_mem_alloc(PVRMW_TICK_PIC_MAX * 4);
        if(_pu4MwPVRPicData == NULL)
        {
            return FALSE;
        }
        
#ifdef LINUX_TURNKEY_SOLUTION
        UNUSED(i);
        UNUSED(u4Addr);
#else
        for(i=0; i<MW_FVR_META_BUF_NUM; i++)
        {
            u4Addr = (UINT32)x_mem_alloc(PVRMW_TICK_PIC_MAX * 4);
            if(u4Addr == 0)
            {
                x_mem_free((void*)_pu4MwPVRPicData);
                return FALSE;
            }
            _arMwPVRMetaBuf[i].u4BufAddr = u4Addr;
        }
#endif

        // Create callback thread and msq
#ifdef LINUX_TURNKEY_SOLUTION
        if (x_msg_q_create(&_hPVRMwQueue, PVRMW_QUEUE_NAME,
                            sizeof (MWPVR_MSG_T), PVRMW_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create PVR MW queue!\n");
            return FALSE;
        }

        if (x_thread_create(&_hPVRMwThread, PVRMW_THREAD_NAME,
                PVRMW_THREAD_STACK_SIZE, PVRMW_THREAD_PRIORITY, _PVRMwThread, 0, NULL) != OSR_OK)
        {
            LOG(1, "Fail to create PVR MW thread!\n");
            return FALSE;
        }
#endif // LINUX_TURNKEY_SOLUTION

        _fgInit = TRUE;
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** MW_PVR_Init
 *  Initialize pvr driver for middleware interface
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL MW_PVR_Init(void)
{
    ID_IND_T at_id_ind[PVR_STREAM_COMP_NUM];
    ID_IND_T at_ctrl_ind[MW_FVR_CTRL_NUM];
    DRV_COMP_REG_T t_comp_id;
    DRV_COMP_FCT_TBL_T t_comp_fct_tbl;
    UINT32 i, u4Flags;
    INT32 i4Ret;

    if(!_MwPVRInit())
    {
        return FALSE;
    }

    //-----------------------------------------------
    // Register PVR Stream components
    //-----------------------------------------------
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwPVRStream_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwPVRStream_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwPVRStream_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwPVRStream_Set;
    t_comp_id.e_type = DRVT_PVR_STREAM;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = PVR_STREAM_COMP_NUM;
    t_comp_id.u.t_list.pt_list = at_id_ind;

    for (i = 0; i < PVR_STREAM_COMP_NUM; i++)
    {
        t_comp_id.u.t_list.pt_list[i].ui2_id = i;
        t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
        t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
    }

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

    i4Ret = x_rm_reg_comp(&t_comp_id,
                         1,                     // in port num
                         1,                     // out port num
                         NULL,                  // component name
                         u4Flags,               // flags
                         &t_comp_fct_tbl,
                         NULL,
                         0);
    if (i4Ret != RMR_OK)
    {
        return FALSE;
    }

    //-----------------------------------------------
    // Register PVR PCR Stream components
    //-----------------------------------------------
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwPVRStreamPCR_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwPVRStreamPCR_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwPVRStreamPCR_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwPVRStreamPCR_Set;
    t_comp_id.e_type = DRVT_PVR_STREAM_PCR;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = PVR_STREAM_PCR_COMP_NUM;
    t_comp_id.u.t_list.pt_list = at_id_ind;

    for (i = 0; i < PVR_STREAM_COMP_NUM; i++)
    {
        t_comp_id.u.t_list.pt_list[i].ui2_id = i;
        t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
        t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
    }

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

    i4Ret = x_rm_reg_comp(&t_comp_id,
                         1,                     // in port num
                         1,                     // out port num
                         NULL,                  // component name
                         u4Flags,               // flags
                         &t_comp_fct_tbl,
                         NULL,
                         0);
    if (i4Ret != RMR_OK)
    {
        return FALSE;
    }
    
    //-----------------------------------------------
    // Register PVR Control components
    //-----------------------------------------------
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwPVRCtrl_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwPVRCtrl_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwPVRCtrl_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwPVRCtrl_Set;
    t_comp_id.e_type = DRVT_PVR_CTRL;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = MW_FVR_CTRL_NUM;
    t_comp_id.u.t_list.pt_list = at_ctrl_ind;

    for (i = 0; i < MW_FVR_CTRL_NUM; i++)
    {
        t_comp_id.u.t_list.pt_list[i].ui2_id = i;
        t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
        t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
    }
    i4Ret = x_rm_reg_comp(&t_comp_id,
                         1,                     // in port num
                         1,                     // out port num
                         NULL,                  // component name
                         u4Flags,               // flags
                         &t_comp_fct_tbl,
                         NULL,
                         0);
    if (i4Ret != RMR_OK)
    {
        return FALSE;
    }

    return TRUE;
}

