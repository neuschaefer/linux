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
 * $RCSfile: swdmx_rm.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_rm.c
 *  Software demux for RM - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_rm.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define __MODEL_slt__

#define GOOD  // ToDo: check why this will mess up COOK audio

#define RM_ONE_SEC_DURATION (90000 * 2)

#define RV_IVOP         0xb0
#define RV_FIVOP        0xb1
#define RV_PVOP         0xb2
#define RV_BVOP         0xb3

#define HX_KEYFRAME_FLAG  (0x1 << 1)

#define RM_BUFFER_SIZE RM_MIN((prFeederInfo->u4Size - 1024 * 2), (235 * 7 * 1024))
#define RM_KEY_TABLE_THRESHOLD (3)

#define INPUT_SRC prSwdmxInst->rFmtInfo.rSwdmxRmInfo.prDmxRangeInfo->e_mm_src_type

#define RM_VFIFO_THR_ON     ((INPUT_SRC == MM_SRC_TYPE_NETWORK_XUNLEI) || (INPUT_SRC == MM_SRC_TYPE_NETWORK_CMPB))
#define RM_VFIFO_THR_PIC_NS ((RM_VFIFO_THR_ON && i4PlaySpeed == 1)? 15: 60)
#define RM_VFIFO_MAX_PIC_NS ((RM_VFIFO_THR_ON && i4PlaySpeed == 1)? 20: 0Xffff)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define RM_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define RM_MAX(x, y) (((x) > (y)) ? (x) : (y))

#define RM_ABS(x)                  (((x) >= 0) ? (x) : -(x))

#define RV_PIC_TYPE(p)  ((p) | 0xB0)  // !!! must match the definition in mpv_comm.h

#define IS_SLOW_FWD(s)  ((s) > 1 && (s) < 1000)

#define IS_TRICK_USE_IDX(s)   ((s) < 0 ||(s) > 2000)
#define IS_TRICK_UNUSE_IDX(s) (IS_SLOW_FWD(s) || (s) == 2000) 

// multi-instance
#define SWDMX_RM_VERIFY_NULL(ptr)                                                          \
          do {                                                                              \
            if (ptr == NULL) {                                                              \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return; }                                                                   \
          } while (0)

#define SWDMX_RM_VERIFY_NULL_ERROR(ptr)                                                    \
          do {                                                                              \
            if (ptr == NULL) {                                                              \
                LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return FALSE; }                                                             \
          } while (0)

#ifdef RMLS
#define RM_LS_CHG_STRM_THRD (90000)
#endif


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static VOID compute_generic_interleave_pattern(SWDMX_INFO_T *prSwdmxInst)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    COOK_INFO_T *prCookInfo = &prSwdmxRmInfo->rCookInfo;
    INT32 codec_frame_size = prCookInfo->u2FrameSzInBytes; //_wCookFrameSz
    INT32 interleave_block_size = prCookInfo->u2CookBlockSz;  //_wCookBlockSz
    INT32 interleave_factor = prCookInfo->u2CookBlockPerSB;  //_bCookBlockPerSB)
    INT32 i = 0;
    INT32 f = 0;
    INT32 b = 0;
    BOOL even = TRUE;
    INT32 frames_per_superblock;
    INT32 frames_per_block;

    if (codec_frame_size == 0)
        return;

    frames_per_superblock = (interleave_block_size * interleave_factor) / codec_frame_size;
    frames_per_block = interleave_block_size / codec_frame_size;

    if (interleave_factor == 1)
    {
        for (i = 0; i < frames_per_superblock && i < 100; i++)
        {
            prSwdmxRmInfo->pbPattern[i] = i;
        }
    }
    else
    {
        while (i < frames_per_superblock && i < 100)
        {
            prSwdmxRmInfo->pbPattern[i] = b * frames_per_block + f;
            i++;
            b += 2;

            if (b >= interleave_factor)
            {
                if (even)
                {
                    even = FALSE;
                    b = 1;
                }
                else
                {
                    even = TRUE;
                    b = 0;
                    f++;
                }
            }
        }
    }
}


UINT32 fgCookBufCpy(SWDMX_INFO_T *prSwdmxInst, UINT8* str1, UINT8* str2, UINT32 idx1, UINT32 idx2, UINT32 NoCpy, UINT32 dwLastFrameOfst)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    COOK_INFO_T *prCookInfo = &prSwdmxRmInfo->rCookInfo;
    UINT32 dwCurFrameOfst;
    UINT32 i;

    dwCurFrameOfst = dwLastFrameOfst;

    for (i = 0; i < NoCpy; i++)
    {
        switch (dwCurFrameOfst & 0x3)
        {
        case 0:
          *(str1 + idx1 + i) = *(str2 + idx2 + i) ^ 0x37;
            break;
        case 1:
          *(str1 + idx1 + i) = *(str2 + idx2 + i) ^ 0xc5;
            break;
        case 2:
          *(str1 + idx1 + i) = *(str2 + idx2 + i) ^ 0x11;
            break;
        case 3:
          *(str1 + idx1 + i) = *(str2 + idx2 + i) ^ 0xf2;
            break;
        default:
            break;
        }

        dwCurFrameOfst++;

        if (prCookInfo->u1ChanNum == 2 && prCookInfo->u2CplQBits == 0)
        {
           if (dwCurFrameOfst == prCookInfo->u2FrameSzInBytes/2)
               dwCurFrameOfst = 0;
        }
        else
        {
           if (dwCurFrameOfst == prCookInfo->u2FrameSzInBytes)
               dwCurFrameOfst = 0;
        }
    }

    return dwCurFrameOfst;
}


static UINT32 dwAFifoSz(SWDMX_INFO_T *prSwdmxInst)
{
#ifdef COOK_WRITE_AFIFO
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    return prSwdmxRmInfo->u4AFifoSz;
#else
    UNUSED(prSwdmxInst);
    return RA_ALLOC_SIZE;
#endif
}

#ifdef COOK_WRITE_AFIFO
static UINT32 dwAFifoForCookSBSpace(SWDMX_INFO_T *prSwdmxInst)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    return DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, prSwdmxRmInfo->u1AudPid);
}
#endif

static VOID vCookFrameDeInterleave(SWDMX_INFO_T *prSwdmxInst, UINT8 *pu1Data)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    COOK_INFO_T *prCookInfo = &prSwdmxRmInfo->rCookInfo;
    int i;
    int j;
    UINT32 dwTgtFrameOfst;
    UINT32 dwLastFrameOfst;
    UINT16 wCookFrameSz = prCookInfo->u2FrameSzInBytes;
    UCHAR *pucBuf;
#ifdef COOK_WRITE_AFIFO
#ifdef CC_MT5363
    UINT32 u4AWp;
    BOOL fgRet;
#endif
#endif

    if (prSwdmxRmInfo->fgCookSBPending)
    {
        ASSERT(pu1Data == NULL && prSwdmxRmInfo->dwCurCookFrameInSB == prCookInfo->u2CookFramePerSB);
        pucBuf = prSwdmxRmInfo->pucAudBuf;
        goto COOK_DMX;
    }

#ifdef COOK_WRITE_AFIFO
    //Should not go here! Very serious error.
    if (prSwdmxRmInfo->fgAFifoForCookSBFull)
        ASSERT(0);
#endif

#ifdef COOK_WRITE_AFIFO
    pucBuf = (UCHAR *)prSwdmxRmInfo->u4AFifoSa;
#else
    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA &&
        prSwdmxRmInfo->pucAudBuf == NULL)
    {
        #ifdef SWDMX_BUFFER_REFINE
        UINT32 ui4Size  = RA_ALLOC_SIZE;
        UINT32 ui4Addr  = 0;
        UINT32 ui4Align = 16;
        SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
        prSwdmxRmInfo->pucAudBuf = (UCHAR *)ui4Addr;
        #else
        prSwdmxRmInfo->pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(RM_MAX(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        #endif

        if (prSwdmxRmInfo->pucAudBuf == NULL)
        {
            LOG(1, "Allocate RA buffer failed!\n");
            ASSERT(0);
            return;
        }
    }
    pucBuf = prSwdmxRmInfo->pucAudBuf;
#endif

    for (i = 0; i < prCookInfo->u2CookFramePerBlock; i++)
    {
       for (j = 0; j < prCookInfo->u2CookFramePerSB && j < 100; j++)
       {
           if (prSwdmxRmInfo->pbPattern[j] == prSwdmxRmInfo->dwCurCookFrameInSB + i)
               break;
       }

       dwTgtFrameOfst = j * wCookFrameSz;

       if (prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst + wCookFrameSz <= dwAFifoSz(prSwdmxInst))
       {
           fgCookBufCpy(prSwdmxInst, pucBuf, pu1Data, prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst, i*wCookFrameSz, wCookFrameSz, 0);
       }
       else
       {
          if (prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst < dwAFifoSz(prSwdmxInst))
          {
             dwLastFrameOfst =  fgCookBufCpy(prSwdmxInst, pucBuf, pu1Data, prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst, i*wCookFrameSz, dwAFifoSz(prSwdmxInst) - (prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst), 0);
             fgCookBufCpy(prSwdmxInst, pucBuf, pu1Data, 0, i*wCookFrameSz + dwAFifoSz(prSwdmxInst) - (prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst), prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst + wCookFrameSz -dwAFifoSz(prSwdmxInst),  dwLastFrameOfst);
          }
          else
          {
             fgCookBufCpy(prSwdmxInst, pucBuf, pu1Data, prSwdmxRmInfo->dwCurAFifoOfst + dwTgtFrameOfst - dwAFifoSz(prSwdmxInst), i*wCookFrameSz, wCookFrameSz, 0);
         }
       }
    }

    prSwdmxRmInfo->dwCurCookFrameInSB += prCookInfo->u2CookFramePerBlock;
    //printf("swdmx_rm 315 prSwdmxRmInfo->dwCurCookFrameInSB ==> %d\n", prSwdmxRmInfo->dwCurCookFrameInSB);

COOK_DMX:
    if (prSwdmxRmInfo->dwCurCookFrameInSB == prCookInfo->u2CookFramePerSB)
    {
        //printf("swdmx_rm 319 TX SB!!!\n");
        if (DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, prSwdmxRmInfo->u1AudPid)
            > prCookInfo->u2CookSBSz)
        {
            if (prSwdmxRmInfo->fgCookSBPending)
            {
                prSwdmxRmInfo->fgCookSBPending = FALSE;
                LOG(3, "Clear COOK SB pending, count = %d...\n", prSwdmxRmInfo->u4CookSBPendingCnt);
                prSwdmxRmInfo->u4CookSBPendingCnt = 0;
            }
        }
        else
        {
            UINT16 u2CurQNum, u2MaxQNum;
            VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
            if (u2CurQNum <= 2)
            {
                LOG(3, "Video dry due to COOK SB pending: %d!!!\n", u2CurQNum);
            }

            if (!prSwdmxRmInfo->fgCookSBPending)  // first time setting pending flag, reset the count
            {
                prSwdmxRmInfo->u4CookSBPendingCnt = 1;
                LOG(3, "COOK SB pending!!!!!!\n");
            }
            else
            {
                prSwdmxRmInfo->u4CookSBPendingCnt++;
            }
            prSwdmxInst->fgFifoFull=TRUE;
            prSwdmxRmInfo->fgCookSBPending = TRUE;
            //ASSERT(0);
            return;
        }

#ifndef COOK_WRITE_AFIFO
    {
        DMX_MM_DATA_T rDmxMMData;

        x_memset(&rDmxMMData, 0, sizeof(rDmxMMData));
        rDmxMMData.u1Idx = prSwdmxRmInfo->u1AudPid;
        rDmxMMData.u4BufStart = (UINT32)pucBuf;
        rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + RA_ALLOC_SIZE;
        rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
        rDmxMMData.u4FrameSize = prCookInfo->u2FrameSzInBytes;//u2CookSBSz;
        rDmxMMData.u4Pts = prSwdmxRmInfo->u4CookSBPts;//prSwdmxRmInfo->rRmDmxMMData.u4Pts;

        // DSP asks for sending each frame one by one instead of sending a whole SB at one time
        for (i = 0; i < prCookInfo->u2CookFramePerSB; i++)
        {
            if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
            {
                LOG(0, "Demuxer fails to move data.\n");
                ASSERT(0);
            }
            rDmxMMData.u4StartAddr += rDmxMMData.u4FrameSize;
        }
    }

#ifdef RMLS
    if (prSwdmxRmInfo->fgRmLs)
    {
        ASSERT(RM_STRM_AUD == prSwdmxRmInfo->u4RmStrmIdx);
        prSwdmxRmInfo->u4LsPts[RM_STRM_AUD] = prSwdmxRmInfo->u4CookSBPts;

        if (prSwdmxRmInfo->fgDeliverVEos)
        {
            // Video is finished, no need to change to video
        }
        else if (prSwdmxRmInfo->u4LsPts[RM_STRM_AUD] > (prSwdmxRmInfo->u4LsPts[RM_STRM_VDO] + RM_LS_CHG_STRM_THRD))
        {
            LOG(6, "419:Set prSwdmxRmInfo->fgRmChgStrm\n");
            prSwdmxRmInfo->fgRmChgStrm = TRUE;
        }
    }
#endif

#endif
        //_dwPrsAddr = dPRAFifoWp();
        //vDspSendPts(_dwLastCookAPts, _dwPrsAddr);  !!!
#if 0  // !!!
        // megaa 20080911: for tbh.rmvb which has no audio packet in the beginning
        if (_fgRmDspPlayPending && _dw1stCookAPts == 0xFFFFFFFF)
        {
          _dw1stCookAPts = _dwLastCookAPts;
        }
#endif
        prSwdmxRmInfo->dwCurCookFrameInSB = 0;
#ifdef COOK_WRITE_AFIFO
        if(prSwdmxRmInfo->dwCurAFifoOfst + prCookInfo->u2CookSBSz <= dwAFifoSz(prSwdmxInst))
        {
            prSwdmxRmInfo->dwCurAFifoOfst += prCookInfo->u2CookSBSz;
        }
        else
        {
            prSwdmxRmInfo->dwCurAFifoOfst = prSwdmxRmInfo->dwCurAFifoOfst + prCookInfo->u2CookSBSz - dwAFifoSz(prSwdmxInst);
        }
#endif
        //_dwSimAFifoWp = ((DWRD)_pbAFifo + prSwdmxRmInfo->dwCurAFifoOfst) & 0xFFFFFF;  // megaa debug 0708

#ifdef COOK_WRITE_AFIFO
#ifdef CC_MT5363
       u4AWp = PHYSICAL(prSwdmxRmInfo->u4AFifoSa + prSwdmxRmInfo->dwCurAFifoOfst);
       fgRet = DMX_MM_SendAudioChunk(prSwdmxRmInfo->u1AudPid, u4AWp, prSwdmxRmInfo->rRmDmxMMData.u4Pts, FALSE);
       ASSERT(fgRet);
       if (!fgRet) return;
#endif
#endif

#ifdef REAL_WRITE_AFIFO
        _dwSimAFifoRp = dPRAFifoRp();
        _dwSimAFifoWp = dPRAFifoWp();
#endif

#ifdef COOK_WRITE_AFIFO
        if (dwAFifoForCookSBSpace(prSwdmxInst) <= prCookInfo->u2CookSBSz)
        {
            //_dwAFifoForCookSBFullCounter++;
            prSwdmxRmInfo->fgAFifoForCookSBFull = TRUE;
            //vPsrOff();
        }
#endif
#if 0
        if(_dRmCookParseState == RM_COOKPRS_WAIT_PARSE_SB)
            _dwPreParsedSB++;
#endif
    }
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsRmPacket(
    SWDMX_INFO_T *prSwdmxInst,
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd,
    BOOL fgOneVideoFrm);

static BOOL _CheckRmPacketIndex(UCHAR *pucBuf, UINT32 u4StrmID, BOOL *pfgVer1);
static BOOL _CheckRmPacketType(SWDMX_INFO_T *prSwdmxInst, UCHAR *pucBuf, UINT8 *pu1HdrType, UINT8 *pu1TrackIdx, BOOL *fgVer1, UINT32 *pu4Timestamp, UINT16 *pu2Flag, UINT32 *pu4PktSize);

static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsWithoutIdx(
    SWDMX_INFO_T *prSwdmxInst,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

//static SWDMX_HDR_PRS_STATE_T  _SWDMX_BeforeDeliverUseIdx(UINT8 u1HdrType);

static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsUseIdx(
    SWDMX_INFO_T *prSwdmxInst,
    INT32 i4Speed,
    SWDMX_RANGE_LIST_T* prRangeList);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverRmKeyframe(
    SWDMX_INFO_T *prSwdmxInst,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl,
    UINT32* pu4CurIdx,
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed);
static BOOL _SWDMX_RmInitDmx(SWDMX_INFO_T *prSwdmxInst, UINT8 u1HdrType, BOOL fgEos);

static BOOL _SWDMX_SendRmEos(SWDMX_INFO_T *prSwdmxInst, UINT8 u1HdrType);
static BOOL _SWDMX_RmSeekCurPos(SWDMX_INFO_T *prSwdmxInst);
static BOOL _SWDMX_RmSeek(SWDMX_INFO_T *prSwdmxInst, UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList, BOOL fgNoDecrement);
static BOOL _SWDMX_RmSeekPos(SWDMX_INFO_T *prSwdmxInst, START_POS_INFO_T *ptJumpInfo);

static BOOL _SWDMX_RmGetDataFromFeeder(SWDMX_INFO_T *prSwdmxInst, FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_RmCalculateRepeatFrame(SWDMX_INFO_T *prSwdmxInst, INT32 i4PlaySpeed);

static BOOL _SWDMX_RmResetFeeder(SWDMX_INFO_T *prSwdmxInst);

static void _SWDMX_RmVideoWakeUp(SWDMX_INFO_T *prSwdmxInst);


// !!!
static void _SWDMX_RmCalculatePts(UINT8 u1HdrType, UINT8 u1TrackIdx, UINT32 u4ChunkSize)
{
}



// !!!
inline static BOOL _SWDMX_RmVdecQueueThreshold(SWDMX_INFO_T *prSwdmxInst, UINT16 u2Threshold)
{
    UINT16 u2CurQNum = 0;
    UINT16 u2MaxQNum = 0;
    VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);

    if ((u2CurQNum < u2MaxQNum) && (u2CurQNum < u2Threshold))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



static void vMpsPrsNextG(SWDMX_INFO_T *prSwdmxInst, INT32 adv, UINT32 len, INT32 dummy)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    UINT32 u4SrcBufLen = prSwdmxRmInfo->rRmDmxMMData.u4BufEnd - prSwdmxRmInfo->rRmDmxMMData.u4BufStart;

    prSwdmxRmInfo->pbHdrBuf += adv;
    if ((UINT32)prSwdmxRmInfo->pbHdrBuf >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
    {
        prSwdmxRmInfo->pbHdrBuf -= u4SrcBufLen;
    }
    prSwdmxRmInfo->wGTxLen = len;
    UNUSED(dummy);

    // megaa 20090601: ToDo: copy from ring buffer to a plain buffer
}


// megaa 20090601: ToDo: implement it
static BOOL fgRmSliceSzBufChk(void)
{
    return TRUE;
}


void vGetRvSliceSz(UINT8 u1SrcId, UINT32 *pu4SliceSzArr, UINT32 u4Rp, UINT32 u4Cnt)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    while (u4Cnt > 0)
    {
        *pu4SliceSzArr = prSwdmxRmInfo->au4SliceSzBuf[u4Rp];
        pu4SliceSzArr++;
        if (++u4Rp >= RM_SLICE_SZ_BUF_NS)
            u4Rp = 0;

        u4Cnt--;
    }

    prSwdmxRmInfo->dwSliceSzBufRp = u4Rp;
}


static void vSendVFrame(SWDMX_INFO_T *prSwdmxInst)
{
  SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
  int i;
  UINT8 *pbData = (UINT8 *)(prSwdmxRmInfo->u4VFifoWpBak/* + DRAMA_NONCACH_BASE_ADDRESS*/);
  UINT8 *pbDataWarp;

  // !!! code corresponding to rv_depack_internal.c #1040
  if (prSwdmxRmInfo->rFrameHdr.fgBrokenUpByUs && prSwdmxRmInfo->rFrameHdr.u4NumPackets > 1)
  {
    //ASSERT(prSwdmxRmInfo->rFrameHdr.u4NumPackets == 2);
    prSwdmxRmInfo->rFrameHdr.u4NumPackets = 1;
    prSwdmxRmInfo->au4SliceSz[0] += prSwdmxRmInfo->au4SliceSz[1];
  }

  // count the original frame length before ripping off ECC packets
  //for (_u4FrameLen = 0, i = 0; i < prSwdmxRmInfo->rFrameHdr.u4NumPackets; i++)
  //  _u4FrameLen += prSwdmxRmInfo->au4SliceSz[i];

  // rip off ECC packets
  for (i = prSwdmxRmInfo->rFrameHdr.u4NumPackets - 1; i >= 0; i--)
  {
    pbDataWarp = pbData + prSwdmxRmInfo->au4SliceOfst[i];
    if ((UINT32)pbDataWarp >= prSwdmxRmInfo->u4VFifoEa)
      pbDataWarp -= (prSwdmxRmInfo->u4VFifoEa - prSwdmxRmInfo->u4VFifoSa);
    if (*pbDataWarp & prSwdmxRmInfo->u1EccMask)
      prSwdmxRmInfo->rFrameHdr.u4NumPackets--;
  }

#if 0
  printf("[SWDMX] Frame #%d: %d slices (%d...)\n", prSwdmxRmInfo->u4FrmNo, prSwdmxRmInfo->rFrameHdr.u4NumPackets, prSwdmxRmInfo->au4SliceSz[0]);
  for (i = 0; i < prSwdmxRmInfo->rFrameHdr.u4NumPackets; i++)
  {
    printf("sliceSz: %d\n", prSwdmxRmInfo->au4SliceSz[i]);
  }
  for (i = 0; i < prSwdmxRmInfo->rFrameHdr.u4NumPackets; i++)
  {
    printf("sliceOfst: %d\n", prSwdmxRmInfo->au4SliceOfst[i]);
  }
#endif
#if 0
  sprintf(szName, "f%07d.bin", prSwdmxRmInfo->u4FrmNo);
  fp = fopen(szName, "wb");
  fwrite((void *)pbData, 1, _dwPrVWp - _dwPrVRp, fp);
  fclose(fp);
#endif
  //_fgFrameRdy = TRUE;

  //_dwPrVWp = _dwPrVRp;
  prSwdmxRmInfo->u4FrmNo++;
}


static void vRmAddPic(SWDMX_INFO_T *prSwdmxInst, VDEC_PES_INFO_T *prPesInfo)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    UINT8 *pbSc = (UINT8 *)(prSwdmxRmInfo->u4VFifoWpBak/* + DRAMA_NONCACH_BASE_ADDRESS*/);
    INT32 i4TrDelta;
    UINT32 i;
    UINT8 bType;
    BOOL fgDoSend = TRUE;

    // add slice info
#if 1
    prPesInfo->u4RenderPts = prSwdmxRmInfo->dwSliceSzBufWp;
    prPesInfo->u1SliceCount = prSwdmxRmInfo->rFrameHdr.u4NumPackets;
#else
    _ptPicIdx[_dwPicWrIdx].dwSliceSzBufWp = prSwdmxRmInfo->dwSliceSzBufWp;
    _ptPicIdx[_dwPicWrIdx].dwSliceNum = prSwdmxRmInfo->rFrameHdr.u4NumPackets;
#endif
    for (i = 0; i < prSwdmxRmInfo->rFrameHdr.u4NumPackets; i++)
    {
        *((UINT32 *)RM_SLICE_SZ_BUF_SA + prSwdmxRmInfo->dwSliceSzBufWp) = prSwdmxRmInfo->au4SliceSz[i];
        if (++prSwdmxRmInfo->dwSliceSzBufWp == RM_SLICE_SZ_BUF_NS)
        {
            prSwdmxRmInfo->dwSliceSzBufWp = 0;
        }
    }

    //vPicAddrAdd(dwPtr, TRUE);
    prPesInfo->u4PTS *= 90;
    //printf("[SWDMX_RM]About to call VDEC_SendEs, B4 pts = %X\n", prPesInfo->u4PTS);

    prSwdmxRmInfo->dwCurrPicTr = 0;
    for(i = 0; i < 4; i++)
    {
        if((UINT32)pbSc >= prSwdmxRmInfo->u4VFifoEa)
        {
            pbSc = (UINT8 *)prSwdmxRmInfo->u4VFifoSa;
        }

        prSwdmxRmInfo->dwCurrPicTr <<= 8;
        prSwdmxRmInfo->dwCurrPicTr |= ((*pbSc) & 0xFF);

        pbSc++ ;
    }
    pbSc = (UINT8 *)(prSwdmxRmInfo->u4VFifoWpBak/* + DRAMA_NONCACH_BASE_ADDRESS*/);

    if (prSwdmxRmInfo->u1EccMask == 0x20)  // it is RV8, tmp solution
    {
      bType = RV_PIC_TYPE(((*pbSc) & 0x18) >> 3);

      //prSwdmxRmInfo->dwCurrPicTr = (((((*pbSc)<<24) + ((*(pbSc+1))<<16) + ((*(pbSc+2))<<8) + (*(pbSc+3))) & 0xFFF80) >> 7);
      prSwdmxRmInfo->dwCurrPicTr = ((prSwdmxRmInfo->dwCurrPicTr & 0xFFF80) >> 7);

      if(bType == RV_BVOP)
      {
        if (prSwdmxRmInfo->dwFordwardRefPts >= prSwdmxRmInfo->dwBackwardRefPts)  // tmp solution for out-of-order B frames
          fgDoSend = FALSE;

        i4TrDelta = prSwdmxRmInfo->dwCurrPicTr - prSwdmxRmInfo->dwFordwardRefTr;
        if(i4TrDelta < 0)
        {
          i4TrDelta += 0x2000;
        }
        prPesInfo->u4PTS = prSwdmxRmInfo->dwFordwardRefPts + (i4TrDelta*90);
      }
      else
      {
        prSwdmxRmInfo->dwFordwardRefTr = prSwdmxRmInfo->dwBackwardRefTr;
        prSwdmxRmInfo->dwBackwardRefTr = prSwdmxRmInfo->dwCurrPicTr;
        prSwdmxRmInfo->dwFordwardRefPts = prSwdmxRmInfo->dwBackwardRefPts;
        prSwdmxRmInfo->dwBackwardRefPts = prPesInfo->u4PTS;
      }
    }
    else if (prSwdmxRmInfo->u1EccMask == 0x80)  // it is RV9 or RV10, tmp solution
    {
      bType = RV_PIC_TYPE(((*pbSc) & 0x60) >> 5);

      //prSwdmxRmInfo->dwCurrPicTr = (((((*pbSc)<<24) + ((*(pbSc+1))<<16) + ((*(pbSc+2))<<8) + (*(pbSc+3))) & 0x7FFC0) >> 6);
      prSwdmxRmInfo->dwCurrPicTr = ((prSwdmxRmInfo->dwCurrPicTr & 0x7FFC0) >> 6);

      if(bType == RV_BVOP)
      {
        if (prSwdmxRmInfo->dwFordwardRefPts >= prSwdmxRmInfo->dwBackwardRefPts)  // tmp solution for out-of-order B frames
          fgDoSend = FALSE;

        i4TrDelta = prSwdmxRmInfo->dwCurrPicTr - prSwdmxRmInfo->dwFordwardRefTr;
        if(i4TrDelta < 0)
        {
          i4TrDelta += 0x2000;
        }
        prPesInfo->u4PTS = prSwdmxRmInfo->dwFordwardRefPts + (i4TrDelta*90);
      }
      else
      {
        prSwdmxRmInfo->dwFordwardRefTr = prSwdmxRmInfo->dwBackwardRefTr;
        prSwdmxRmInfo->dwBackwardRefTr = prSwdmxRmInfo->dwCurrPicTr;
        prSwdmxRmInfo->dwFordwardRefPts = prSwdmxRmInfo->dwBackwardRefPts;
        prSwdmxRmInfo->dwBackwardRefPts = prPesInfo->u4PTS;
      }
    }
    else
    {
        ASSERT(0);
        return;
    }


#ifdef __MODEL_slt__
    if (prPesInfo->u4PTS > prSwdmxRmInfo->u4RmSltLastPicPts)
    {
        prSwdmxRmInfo->u1RmSltLastPicType = bType;
        prSwdmxRmInfo->u4RmSltLastPicPts = prPesInfo->u4PTS;
    }
#endif

    if (!fgDoSend)
        return;

#if 1
    prPesInfo->u4VldReadPtr = PHYSICAL(prSwdmxRmInfo->u4VFifoWpBak);
    prPesInfo->ucPicType = bType - 0xB0;
    prSwdmxRmInfo->u4NumSentVid++;

    if (((UINT32)((prSwdmxRmInfo->u8LastIFramePosition)>>32) == 0xFFFFFFFF)
                    && ((UINT32)(prSwdmxRmInfo->u8LastIFramePosition) == 0xFFFFFFFF))
    {
        prSwdmxRmInfo->u8LastIFramePosition = prSwdmxRmInfo->u8VFileOffsetBak;
    }

    if (bType == RV_IVOP || bType == RV_FIVOP)
    {
        //update last i frame file offset
        prSwdmxRmInfo->u8LastIPtsInfo = prPesInfo->u4PTS;
        prSwdmxRmInfo->u8LastIFramePosition = prSwdmxRmInfo->u8VFileOffsetBak;
    }


    //printf("[SWDMX_RM]About to call VDEC_SendEs, A4 pts = %X\n", prPesInfo->u4PTS);
//  Printf("VDEC_SendEs(%d, %d): 0x%08x\n", prSwdmxInst->u1SwdmxId, prSwdmxRmInfo->u4FrmNo - 1, prPesInfo->u4VldReadPtr);
    prPesInfo->rExtra.fgValid = TRUE;
    prPesInfo->rExtra.u8PTSI = prSwdmxRmInfo->u8LastIPtsInfo;
    prPesInfo->u8OffsetI = prSwdmxRmInfo->u8LastIFramePosition;
    prPesInfo->u8Offset  = prSwdmxRmInfo->u8VFileOffsetBak;
    VDEC_SendEs((void*)prPesInfo);
#else
    //vPicTpAdd(bType);
#endif

    // check index table full
    //vPrChkPicIdxFull();
}


static void vRmPIsr(SWDMX_INFO_T *prSwdmxInst, BOOL fgEos)
{
  SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
  UINT16 u2MsgQIdx;
  UINT8 u1Msg;
  SIZE_T zMsgSize;
  UINT32 u4SrcBufLen;

  do
  {
      zMsgSize = sizeof(u1Msg);
      VERIFY(x_msg_q_receive(&u2MsgQIdx, &u1Msg, &zMsgSize, &prSwdmxInst->hRmIsrMsgQ, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
  }
  while (!fgEos && prSwdmxRmInfo->dwPrsState != RMPRS_PACKET && prSwdmxRmInfo->dwPrsState != RMPRS_FRM_NEXT_MULTIPLE);

  if (fgEos)
      return;

  u4SrcBufLen = prSwdmxRmInfo->rRmDmxMMData.u4BufEnd - prSwdmxRmInfo->rRmDmxMMData.u4BufStart;
  prSwdmxRmInfo->pbHdrBuf += prSwdmxRmInfo->rRmDmxMMData.u4FrameSize;
  if ((UINT32)prSwdmxRmInfo->pbHdrBuf >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
  {
      prSwdmxRmInfo->pbHdrBuf -= u4SrcBufLen;
  }

  if (prSwdmxRmInfo->dwPrsState == RMPRS_PACKET)
  {
      vMpsPrsNextG(prSwdmxInst, 0, 12, 0);
  }
  else if (prSwdmxRmInfo->dwPrsState == RMPRS_FRM_NEXT_MULTIPLE)
  {
      vMpsPrsNextG(prSwdmxInst, 0, 2, 0);
  }
  else
  {
      ASSERT(0);
  }
}


static BOOL fgMpsPrsNextP(SWDMX_INFO_T *prSwdmxInst, INT32 adv, UINT32 len, UINT32 u4Pts)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    UINT32 u4SrcBufLen = prSwdmxRmInfo->rRmDmxMMData.u4BufEnd - prSwdmxRmInfo->rRmDmxMMData.u4BufStart;
    UINT32 u4BufStartBak;

    // check if the parsed frame size is correct
    if (len >= DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prSwdmxRmInfo->u1VidPid))
    {
    	prSwdmxInst->fgFifoFull=TRUE;
        return FALSE;  // incorrect frame size (corrupt data), return FALSE to discard it
    }

    prSwdmxRmInfo->pbHdrBuf += adv;
    if ((UINT32)prSwdmxRmInfo->pbHdrBuf >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
    {
        prSwdmxRmInfo->pbHdrBuf -= u4SrcBufLen;
    }

    prSwdmxRmInfo->wGTxLen = 0;
    prSwdmxRmInfo->rRmDmxMMData.u1Idx = prSwdmxRmInfo->u1VidPid;
    prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = (UINT32)prSwdmxRmInfo->pbHdrBuf;
    prSwdmxRmInfo->rRmDmxMMData.u4FrameSize = len;
    prSwdmxRmInfo->rRmDmxMMData.u4Pts = u4Pts;
    prSwdmxRmInfo->rRmDmxMMData.fgEOS = FALSE;

    u4BufStartBak = prSwdmxRmInfo->rRmDmxMMData.u4BufStart;
    if (prSwdmxRmInfo->rRmDmxMMData.u4StartAddr < prSwdmxRmInfo->rRmDmxMMData.u4BufStart)
        prSwdmxRmInfo->rRmDmxMMData.u4BufStart -= FEEDER_RESERVED_SIZE;

    //printf("[SWDMX_RM]About to call DMX, pts = %X\n", u4Pts * 90);
    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &prSwdmxRmInfo->rRmDmxMMData)  != TRUE)
    {
        ASSERT(0);  // !!!
        LOG(0, "Demuxer fails to move data.\n");
        //*pu4ConsumeSize = 0;  !!!
        //return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;  !!!
    }

    prSwdmxRmInfo->rRmDmxMMData.u4BufStart = u4BufStartBak;

    vRmPIsr(prSwdmxInst, FALSE);

    prSwdmxRmInfo->rRmDmxMMData.u4StartAddr += prSwdmxRmInfo->rRmDmxMMData.u4FrameSize;
    if (prSwdmxRmInfo->rRmDmxMMData.u4StartAddr >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
        prSwdmxRmInfo->rRmDmxMMData.u4StartAddr -= u4SrcBufLen;

    return TRUE;
}


static VOID vHandleRmRingBuf(SWDMX_INFO_T *prSwdmxInst)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    FEEDER_BUF_INFO_T *prFeederInfo = &prSwdmxRmInfo->rFeederInfo;
    UINT32 u4AddrDelta;

    if (prSwdmxRmInfo->u4CurDmxFeederRPtr + prSwdmxRmInfo->u4LeftBytesInFeeder > prFeederInfo->u4EndAddr)
    {
        u4AddrDelta = prFeederInfo->u4EndAddr - prSwdmxRmInfo->u4CurDmxFeederRPtr;
        if (u4AddrDelta <= FEEDER_RESERVED_SIZE)
        {
            x_memcpy((VOID*)(prFeederInfo->u4StartAddr - u4AddrDelta), (VOID*)(prSwdmxRmInfo->u4CurDmxFeederRPtr), u4AddrDelta);
            prSwdmxRmInfo->u4CurDmxFeederRPtr = prFeederInfo->u4StartAddr - u4AddrDelta;
            //printf("RM ring buffer copied!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }
    }
}


typedef struct _VID_DEC_RM_SPEC_INFO_T
{
    UINT16          ui2_frm_width;
    UINT16          ui2_frm_height;
    UINT32          ui4_spo_extra_flags;
    UINT32          aui4_specific[25];
    BOOL            b_is_rm8;
} VID_DEC_RM_SPEC_INFO_T;

static INT32 _SetRmSpecInfo(UINT8 u1SrcId, VID_DEC_RM_SPEC_INFO_T* prSeqInfo)
{
    UINT32 u4NumRprSize = 0;
    VDEC_SEQUENCE_DATA_T rVDecSeqData = {0};
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    /*UNUSED(SWDMX_SetInfo(eSWDMX_SET_RANGECALLBACK, (UINT32)_PLAYMGR_UpdateRangeCb, 0, 0));*/

//    VDEC_PlayMM(prSwdmxInst->u1VdecId, TRUE);

    if (prSeqInfo != NULL)
    {
        rVDecSeqData.u2_width = prSeqInfo->ui2_frm_width;
        rVDecSeqData.u2_height = prSeqInfo->ui2_frm_height;
        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_VARIABLE;
        u4NumRprSize = (prSeqInfo->ui4_spo_extra_flags & 0x70000) >> 16;
        if (u4NumRprSize > 0)  // It will be 0 for RV9.  For RV8, add the count of native size.
        {
            u4NumRprSize++;
        }
        if(prSeqInfo->b_is_rm8)
        {
            prSwdmxRmInfo->u1EccMask = (UINT8)0x20;
            /*SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER, 0x20, 0, 0);*/
        }
        else
        {
            prSwdmxRmInfo->u1EccMask = (UINT8)0x80;
            /*SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER, 0x80, 0, 0);*/
        }
    }
    else
    {
        ASSERT(0);
        return FALSE;
    }

    VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
    #if 0//def LINUX_TURNKEY_SOLUTION
     prSeqInfo->ui4_specific = (UINT32)addr_user_to_kernel((VOID *)prSeqInfo->ui4_specific);
    #endif
    VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_RV_RPR_INFO, u4NumRprSize, (UINT32)(prSeqInfo->aui4_specific), !prSeqInfo->b_is_rm8);
    SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_RV, 0, 0);
    return TRUE;
}

BOOL _SWDMX_RmSetDecoderInfo(
    UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32 u4Para1,
    UINT32 u4Para2,
    UINT32 u4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    COOK_INFO_T *prCookInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    prCookInfo = &prSwdmxRmInfo->rCookInfo;

    if(u1Type == eSWDMX_SET_VIDEOTYPE)
    {
        prSwdmxRmInfo->u4VDecType =  u4Para1;
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)  // tmp use it to pass ECC mask from Mutil
    {
        #if 0/*temp*/
        prSwdmxRmInfo->u1EccMask = (UINT8)u4Para1;
        #endif
        _SetRmSpecInfo(u1SrcId, (VID_DEC_RM_SPEC_INFO_T*)u4Para1);
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        prSwdmxRmInfo->u4ADecType = u4Para1;
        if (u4Para1 == ENUM_SWDMX_ADEC_AAC)
        {
            prSwdmxRmInfo->u4AudSampleRate = u4Para2;
            prSwdmxRmInfo->u4AudChannelNs = u4Para3;
            prSwdmxRmInfo->i4RmPtsAdj = -27000;
            LOG(0, "RM audio is AAC\n");
        }
        else if (u4Para1 == ENUM_SWDMX_ADEC_RA)
        {
            x_memcpy((void *)prCookInfo, (void *)u4Para2, sizeof(COOK_INFO_T));
            compute_generic_interleave_pattern(prSwdmxInst);
            prSwdmxRmInfo->dwCurCookFrameInSB = 0;
            prSwdmxRmInfo->dwCurAFifoOfst = 0;
            prSwdmxRmInfo->i4RmPtsAdj = -9000;
            LOG(0, "RM audio is COOK\n");
#ifdef COOK_WRITE_AFIFO
            prSwdmxRmInfo->fgAFifoForCookSBFull = FALSE;
#endif
        }
        else
        {
            ASSERT(0);
        }
        return TRUE;
    }

    return FALSE;
}

BOOL _SWDMX_RmGetInfo(
    UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    //LOG(5, "_SWDMX_RmGetInfo u1Type = %d\n", u1Type);

    if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_RmGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx];
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if (pu4Para1) *pu4Para1 = FALSE;
        if (pu4Para2) *pu4Para2 = FALSE;
        if ( prSwdmxRmInfo->fgEnVideo
            && (prSwdmxRmInfo->prVidKeyIdxTbl)
            && (prSwdmxRmInfo->prVidKeyIdxTbl->ui4_number_of_entry >= RM_KEY_TABLE_THRESHOLD)
            )
        {
            if (pu4Para1) *pu4Para1 = TRUE;
            if (pu4Para2) *pu4Para2 = TRUE;
        }
    }
    else if(u1Type == eSWDMX_GET_AUDIO_ACTIVE_PIDX)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize,u4DmxAvailSize;
        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, 
                                                                        (DMX_PID_TYPE_T)DMX_PID_TYPE_ES_AUDIO,
                                                                        prSwdmxRmInfo->u1AudPid);
        
        *pu4Para1 = u4AudFifoSize - u4DmxAvailSize;
        
        return TRUE;
    }
    return FALSE;
}

BOOL _SWDMX_RmVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT8 u1Msg;
    INT32 i4Ret;
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    rPesInfo.ucMpvId = VLD0;
    rPesInfo.ucEsId= prSwdmxInst->u1VdecId;
    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    //rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = TRUE;  //prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;

    LOG(11, " prPes->u4Pts = %x\n",  prPes->u4Pts );

    if (rPesInfo.fgEos)
    {
        rPesInfo.u4FifoStart = (UINT32)NULL;
        rPesInfo.u4FifoEnd = (UINT32)NULL;
        rPesInfo.u4VldReadPtr = (UINT32)NULL;
        rPesInfo.fgBackward = (prSwdmxRmInfo->i4PrevSpeed<0) ? TRUE : FALSE;
        VDEC_SendEs((void*)&rPesInfo);
    }

  switch (prSwdmxRmInfo->dwPrsState)
  {
  case RMPRS_FRAME:
    prSwdmxRmInfo->dwPktSz -= prSwdmxRmInfo->rFrameHdr.u4PartialFrameSize;
    if (prSwdmxRmInfo->dwPktSz == 0)
    {
      //_u4PktNum--;

      if (prSwdmxRmInfo->rFrameHdr.u4PacketNum == prSwdmxRmInfo->rFrameHdr.u4NumPackets)  // the complete frame is ready
      {
        if (fgRmSliceSzBufChk())
        {
          vSendVFrame(prSwdmxInst);
          vRmAddPic(prSwdmxInst, &rPesInfo);

#if 0  // !!!
          if(_fgStartPlayUntilFrtSB == TRUE)
            _dMpsSkipDisp++;
#endif
        }
        else  // slice size buffer full, break Parser interrupt chain and resume it later
        {
          ASSERT(0);  // !!!
          //prSwdmxRmInfo->dwPrsState = RMPRS_WAIT_SSZ_BUF_1;  !!!
          break;
        }
      }

      prSwdmxRmInfo->dwPrsState = RMPRS_PACKET;
#if 0  // !!!
      _dwVWpBakForCookPrs = dPRVFifoWp();
      _dwLastVPktPrsState = prSwdmxRmInfo->dwPrsState;
      _dwLastParsedVPktOfst = _dwFileOfst;
#endif
      //vMpvState();

      // wake up the SWDMX thread
      i4Ret = x_msg_q_send(prSwdmxInst->hRmIsrMsgQ, (VOID *)&u1Msg, sizeof(UINT8), 255);  //vMpsPrsNextG(prSwdmxInst, 0, 12, 0);
      if (i4Ret != OSR_OK)
      {
          LOG(0, "RM msg Q sent failed case %d!!!\n", __LINE__);
          ASSERT(0);
          return FALSE;
      }
    }
    else
    {
      ASSERT(prSwdmxRmInfo->rFrameHdr.eType == RVFrameTypeLastPartial || prSwdmxRmInfo->rFrameHdr.eType == RVFrameTypeMultiple);

      if (prSwdmxRmInfo->rFrameHdr.u4PacketNum == prSwdmxRmInfo->rFrameHdr.u4NumPackets)  // the complete frame is ready
      {
        if (fgRmSliceSzBufChk())
        {
          vSendVFrame(prSwdmxInst);
          vRmAddPic(prSwdmxInst, &rPesInfo);

#if 0  // !!!
          if(_fgStartPlayUntilFrtSB == TRUE)
            _dMpsSkipDisp++;
#endif  // !!!
        }
        else  // slice size buffer full, break Parser interrupt chain and resume it later
        {
          ASSERT(0);  // !!!
          //prSwdmxRmInfo->dwPrsState = RMPRS_WAIT_SSZ_BUF_2;  !!!
          break;
        }
      }

      prSwdmxRmInfo->dwPrsState = RMPRS_FRM_NEXT_MULTIPLE;
#if 0  // !!!
      _dwVWpBakForCookPrs = dPRVFifoWp();
      _dwLastVPktPrsState = prSwdmxRmInfo->dwPrsState;
      _dwLastParsedVPktOfst = _dwFileOfst;
#endif
      //vMpvState();

      // wake up the SWDMX thread
      i4Ret = x_msg_q_send(prSwdmxInst->hRmIsrMsgQ, (VOID *)&u1Msg, sizeof(UINT8), 255);  //vMpsPrsNextG(prSwdmxInst, 0, 2, 0);
      if (i4Ret != OSR_OK)
      {
          LOG(0, "RM msg Q sent failed case 1036!!!\n");
          ASSERT(0);
      }
    }
    break;

  default:
#if 0
    //ASSERT(0);  !!! the only case should be parsing end (delivered EOS)
#else
    ASSERT(rPesInfo.fgEos);
    // wake up the SWDMX thread
    i4Ret = x_msg_q_send(prSwdmxInst->hRmIsrMsgQ, (VOID *)&u1Msg, sizeof(UINT8), 255);  //vMpsPrsNextG(prSwdmxInst, 0, 2, 0);
    if (i4Ret != OSR_OK)
    {
        LOG(0, "RM msg Q sent failed case %d!!!\n", __LINE__);
        ASSERT(0);
        return FALSE;
    }
#endif
  }

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_RmInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    INT32 i4Ret;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    UNUSED(eType);
    // Initialize feeder
    //FeederOpen(prSwdmxInst->eFeederSrc);

#if 0  // for multi-instance, something has been done in common part
    //*********************************************************
    //FeederSetBufferSize(prSwdmxInst->eFeederSrc, 5*10*1024);
    //FeederStart(prSwdmxInst->eFeederSrc);
    FeederInstallCallback(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_REQ_DONE,
    _SWDMX_FeederCallBack, (UINT32)&prSwdmxRmInfo->rFeederInfo);
    //*********************************************************

    // moved to _SWDMX_RmInitDmx
    //DMX_MM_DECODER_CALLBACKS_T rCallback; // Isaac: should be DMX_DECODER_CALLBACKS_T, Jack fix me
    DMX_DECODER_CALLBACKS_T rCallback;

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _SWDMX_RmVideoCallback;  //_MM_VideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;
#endif

    // Default some values
    prSwdmxRmInfo->u4PrevAStrmID = 0xFFFFFFFF;
    prSwdmxRmInfo->u4AudSampleRate = 44100;
    prSwdmxRmInfo->u4AudChannelNs = 2;

    prSwdmxRmInfo->fgBeginToPlay = TRUE;

    prSwdmxRmInfo->u8SeekPts = 0xFFFFFFFF;
    prSwdmxRmInfo->u8SeekPts <<= 32;
    prSwdmxRmInfo->u8SeekPts |= 0xFFFFFFFF;
    prSwdmxRmInfo->u8ASeekPts = 0xFFFFFFFF;
    prSwdmxRmInfo->u8ASeekPts <<= 32;
    prSwdmxRmInfo->u8ASeekPts |= 0xFFFFFFFF;
    prSwdmxRmInfo->u8VSeekPts = 0xFFFFFFFF;
    prSwdmxRmInfo->u8VSeekPts <<= 32;
    prSwdmxRmInfo->u8VSeekPts |= 0xFFFFFFFF;

    prSwdmxRmInfo->u4TriggerAudPts = 0xFFFFFFFF;

    prSwdmxRmInfo->u4CurRepeatFrameIdx = 0xFFFFFFFF;

    prSwdmxRmInfo->u4VStrmID = 0xFFFFFFFF;
    prSwdmxRmInfo->u4AStrmID = 0xFFFFFFFF;

    prSwdmxRmInfo->u4RmStrmIdx = RM_STRM_VDO;
#ifdef RMLS
    prSwdmxRmInfo->fgRmLs = FALSE;
    prSwdmxRmInfo->fgRmChgStrm = FALSE;
    x_memset(&prSwdmxRmInfo->u4LsPts, 0, sizeof(prSwdmxRmInfo->u4LsPts));
#endif

    prSwdmxRmInfo->u8RecDmxMoviPos = 0;
    prSwdmxRmInfo->u4ModeChangPts = 0xFFFFFFFF;
    prSwdmxRmInfo->u4PullBackAudPts = 0xFFFFFFFF;


    prSwdmxRmInfo->u8LastIFramePosition = 0xFFFFFFFF;
    prSwdmxRmInfo->u8LastIFramePosition <<= 32;
    prSwdmxRmInfo->u8SeekPts |= 0xFFFFFFFF;




    #ifdef SWDMX_BUFFER_REFINE
    {
    UINT32 ui4Size;
    UINT32 ui4Addr;
    UINT32 ui4Align = 16;

    ui4Size = 128 * sizeof(UINT32);
    ui4Addr = 0;
    SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
    prSwdmxRmInfo->au4SliceSz = (UINT32*)ui4Addr;
    ASSERT(prSwdmxRmInfo->au4SliceSz != NULL);

    ui4Addr = 0;
    SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
    prSwdmxRmInfo->au4SliceOfst = (UINT32*)ui4Addr;
    ASSERT(prSwdmxRmInfo->au4SliceOfst != NULL);

    ui4Addr = 0;
    ui4Size = RM_SLICE_SZ_BUF_NS * sizeof(UINT32);
    SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
    prSwdmxRmInfo->au4SliceSzBuf = (UINT32*)ui4Addr;
    ASSERT(prSwdmxRmInfo->au4SliceSzBuf != NULL);

    ui4Addr = 0;
    ui4Size = 100 * sizeof(UINT16);
    SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
    prSwdmxRmInfo->pbPattern = (UINT16*)ui4Addr;
    ASSERT(prSwdmxRmInfo->pbPattern != NULL);
    }
    #endif



//    x_memset(&_rPrevSample, 0, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

    //DDI_BUF_ALIGNMENT == 16 and
    //for _dmx_mm_movedata  Meet the alignment and packet size (188 bytes) requirements.
    if(prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_AAC &&
        prSwdmxRmInfo->pucAudBuf == NULL)
    {
        #ifdef SWDMX_BUFFER_REFINE
        UINT32 ui4Size  = AAC_ALLOC_SIZE;
        UINT32 ui4Addr  = 0;
        UINT32 ui4Align = 16;
        SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
        prSwdmxRmInfo->pucAudBuf = (UCHAR *)ui4Addr;
        #else
        prSwdmxRmInfo->pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(RM_MAX(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        #endif
        if(prSwdmxRmInfo->pucAudBuf == NULL)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            ASSERT(0);
            return FALSE;
        }
    }
#ifndef COOK_WRITE_AFIFO
    else if(prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA &&
        prSwdmxRmInfo->pucAudBuf == NULL)
    {
        #ifdef SWDMX_BUFFER_REFINE
        UINT32 ui4Size  = RA_ALLOC_SIZE;
        UINT32 ui4Addr  = 0;
        UINT32 ui4Align = 16;
        SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
        prSwdmxRmInfo->pucAudBuf = (UCHAR *)ui4Addr;
        #else
        prSwdmxRmInfo->pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(RM_MAX(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        #endif
        
        if(prSwdmxRmInfo->pucAudBuf == NULL)
        {
            LOG(1, "Allocate RA buffer failed!\n");
            ASSERT(0);
            return FALSE;
        }
    }
#endif

#if 0  // for multi-instance, moved to _SWDMX_RmInitDmx
    DMX_SetDecoderCallbacks(&rCallback);
    DMX_SetToDecoder(TRUE);
#endif

    prSwdmxRmInfo->fgFeederInvalid = TRUE;
    prSwdmxRmInfo->fgRepeatRequest = TRUE;

    prSwdmxRmInfo->fgCookPtsPending = FALSE;
    prSwdmxRmInfo->fgCookSBPending = FALSE;

    prSwdmxRmInfo->u4FrmNo = 0;
    if (prSwdmxInst->hRmIsrMsgQ != (HANDLE_T)0)  // ToDo: delete it in uninitialization (if any)
    {
        i4Ret = x_msg_q_delete(prSwdmxInst->hRmIsrMsgQ);
        if (i4Ret != OSR_OK)
        {
            LOG(0, "RM msg Q delete failed!!!!!!\n");
            ASSERT(0);
            return FALSE;
        }
        ASSERT(i4Ret == OSR_OK);
        prSwdmxInst->hRmIsrMsgQ = (HANDLE_T)0;
    }
    if (prSwdmxInst->hRmIsrMsgQ == (HANDLE_T)0)
    {
        char tmp[16];
        x_snprintf(tmp, sizeof(tmp), "RMISRQ[%d]", u1SrcId);
        i4Ret = x_msg_q_create(&prSwdmxInst->hRmIsrMsgQ, tmp, sizeof(UINT8), 1);
        if (i4Ret != OSR_OK)
        {
            LOG(0, "RM msg Q create failed!!!!!!\n");
            ASSERT(0);
            return FALSE;
        }
        ASSERT(i4Ret == OSR_OK);
    }
    prSwdmxRmInfo->dwSliceSzBufWp = 0;
    prSwdmxRmInfo->dwSliceSzBufRp = 0;

#ifdef __MODEL_slt__
    prSwdmxRmInfo->u4RmSltLastPicPts = 0;
    prSwdmxRmInfo->u1EccMask = 0x80;
#endif

    return TRUE;
}


/**
*/
VOID _SWDMX_RmSetRange(UINT8 u1SrcId, SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    UINT8 u1ATrack;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    if (prRangeList == NULL)
    {
        ASSERT(0);
        LOG(1, "Demuxer Range is NULL\n");
        return;
    }

    prSwdmxRmInfo->prDmxRangeInfo = prRangeList->prDmxRangeInfo;

    if (prRangeList != NULL &&
       prRangeList->prDmxRangeInfo != NULL)
    {
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl =
        (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;
        u1ATrack = 0;

        while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                u1ATrack < RM_MAX_AUDIO_TRACK)
            {
                prSwdmxRmInfo->prAudIdxTbl[u1ATrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_RmSetRange Audio Index table = 0x%08x\n",
                    (UINT32)(prSwdmxRmInfo->prAudIdxTbl[u1ATrack]));
                u1ATrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
            {
                prSwdmxRmInfo->prVidIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_RmSetRange Video Index table = 0x%08x\n",
                    (UINT32)(prSwdmxRmInfo->prVidIdxTbl));
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                prSwdmxRmInfo->prVidKeyIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_RmSetRange Key Index table = 0x%08x\n",
                    (UINT32)(prSwdmxRmInfo->prVidKeyIdxTbl));
            }

            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
        }

#ifndef RMLS
        prSwdmxRmInfo->au8FileOffset[RM_STRM_VDO] = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxRmInfo->au8RangeSz[RM_STRM_VDO]  = prRangeList->prDmxRangeInfo->z_range_sz;
        LOG(5, "ofst/range = %lld/%lld\n",
            prSwdmxRmInfo->au8FileOffset[RM_STRM_VDO],
            prSwdmxRmInfo->au8RangeSz[RM_STRM_VDO]);
#else
        if (prSwdmxRmInfo->fgEnAudio
            && (prRangeList->prDmxRangeInfo->z_range_sz2[RM_STRM_AUD] != 0))
        {
            prSwdmxRmInfo->fgRmLs = TRUE;
            prSwdmxRmInfo->au8FileOffset[RM_STRM_VDO] = prRangeList->prDmxRangeInfo->ui8_fileoffset2[RM_STRM_VDO];
            prSwdmxRmInfo->au8RangeSz[RM_STRM_VDO]  = prRangeList->prDmxRangeInfo->z_range_sz2[RM_STRM_VDO];
            prSwdmxRmInfo->au8FileOffset[RM_STRM_AUD] = prRangeList->prDmxRangeInfo->ui8_fileoffset2[RM_STRM_AUD];
            prSwdmxRmInfo->au8RangeSz[RM_STRM_AUD]  = prRangeList->prDmxRangeInfo->z_range_sz2[RM_STRM_AUD];
            LOG(3, "This is logical stream!!\n");
            LOG(5, "ofst/range(%d) = %lld/%lld\n",
                RM_STRM_VDO,
                prSwdmxRmInfo->au8FileOffset[RM_STRM_VDO],
                prSwdmxRmInfo->au8RangeSz[RM_STRM_VDO]);
            LOG(5, "ofst/range(%d) = %lld/%lld\n",
                RM_STRM_AUD,
                prSwdmxRmInfo->au8FileOffset[RM_STRM_AUD],
                prSwdmxRmInfo->au8RangeSz[RM_STRM_AUD]);
        }
        else
        {
            prSwdmxRmInfo->fgRmLs = FALSE;
            prSwdmxRmInfo->au8FileOffset[RM_STRM_VDO] = prRangeList->prDmxRangeInfo->ui8_fileoffset;
            prSwdmxRmInfo->au8RangeSz[RM_STRM_VDO]  = prRangeList->prDmxRangeInfo->z_range_sz;
            LOG(5, "ofst/range = %lld/%lld\n",
                prSwdmxRmInfo->au8FileOffset[RM_STRM_VDO],
                prSwdmxRmInfo->au8RangeSz[RM_STRM_VDO]);
        }
#endif

        if (prSwdmxRmInfo->prVidIdxTbl)
        {
            // The upper layer (mw or playmgr should prevent 0 case for rate and scale)
            if (prSwdmxRmInfo->prVidIdxTbl->ui4_scale)
            {
                prSwdmxRmInfo->u4VidFps =
                    (UINT32)(prSwdmxRmInfo->prVidIdxTbl->ui4_rate/prSwdmxRmInfo->prVidIdxTbl->ui4_scale);
            }

            //prSwdmxRmInfo->u4VidFps = _u4RVFps;
            prSwdmxRmInfo->u4VidFps = 30;

            // Add some error handling
            if (!prSwdmxRmInfo->u4VidFps ||
                prSwdmxRmInfo->u4VidFps > 60)
            {
                prSwdmxRmInfo->u4VidFps = 30;
            }

            LOG(5, "prSwdmxRmInfo->u4VidFps = %ld\n", prSwdmxRmInfo->u4VidFps);
        }

        // Set the number of total audio tracks
        if (prSwdmxRmInfo->u1TotalATrack == 0)  // tmp solution for MW not setting this
            prSwdmxRmInfo->u1TotalATrack = u1ATrack;

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
    else
    {
        LOG(1, "Demuxer Range is NULL\n");
    }
}


/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrs(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
{
    VDEC_PTS_INFO_T rPTSInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    UINT32 u4CurPts = 0;
    UINT32 u4PrsState = 0;

    INT32 i4PlaySpeed = 0;
    BOOL fgSpeedChange = 0;
    UINT64 u8SeekPos = 0;


    if (!prSwdmxInst || !prCmd)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    UNUSED(i4PlaySpeed);
    UNUSED(fgSpeedChange);

    x_memset((void*)&rPTSInfo, 0, sizeof(VDEC_PTS_INFO_T));

    //if (fgSpeedChange)
    //    printf("swdmx_rm 1187\n");

    // cmd dispatch
    i4PlaySpeed = prCmd->i4PlaySpeed;
    fgSpeedChange = prCmd->fgChange;
    u8SeekPos = prCmd->u8SeekPos;

    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if (prSwdmxRmInfo->fgBeginToPlay &&
        prSwdmxRmInfo->prDmxRangeInfo->t_vid_start_render_pts != 0)
    {
        //UINT64 u8PtsDiff = 0;
#if defined(CC_DLNA_SUPPORT) && 0
        UINT32 u4FeederReadSize;

        // For DLNA playback
        u4FeederReadSize = (prSwdmxRmInfo->prDmxRangeInfo->z_range_sz < RM_PREBUFFER_SIZE) ?
                                        (prSwdmxRmInfo->prDmxRangeInfo->z_range_sz) : (RM_PREBUFFER_SIZE);

        LOG(5, "Feeder's ready bound = %d, underflw = %d",  u4FeederReadSize, u4FeederReadSize/8);

        FeederSetReadyThreshold(u4FeederReadSize);
        FeederSetUnderFlowThreshold(u4FeederReadSize/8);
#endif
        if(prSwdmxRmInfo->prVidIdxTbl)
        {
#if 1  // !!!
            rPTSInfo.u8Scale = (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
            rPTSInfo.u4Rate = prSwdmxRmInfo->prVidIdxTbl->ui4_rate;
#else
            if (prSwdmxRmInfo->prVidIdxTbl->ui4_scale &&
                prSwdmxRmInfo->prVidIdxTbl->ui4_rate)
            {
                u8Numerator =
                    (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
                u8Denominator =
                    (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_rate;
                u8PtsDiff = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
            }
            else
            {
                u8PtsDiff = 1;
            }
            prSwdmxRmInfo->u8PtsDiff = u8PtsDiff;
#endif
        }

        UNUSED(_SWDMX_RmSeek(prSwdmxInst, prSwdmxRmInfo->prDmxRangeInfo->t_vid_start_render_pts, prRangeList, FALSE));
        LOG(5, "Video Start Render Pts = %lld\n",
            prSwdmxRmInfo->prDmxRangeInfo->t_vid_start_render_pts);

        prSwdmxRmInfo->u8VSeekPts =
            prSwdmxRmInfo->prDmxRangeInfo->t_vid_start_render_pts;
        prSwdmxRmInfo->u8ASeekPts =
            prSwdmxRmInfo->prDmxRangeInfo->t_vid_start_render_pts;

        prSwdmxRmInfo->fgIsSeek = TRUE;

        if (prSwdmxRmInfo->fgEnAudio && prSwdmxRmInfo->fgEnVideo)
        {
            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxRmInfo->prDmxRangeInfo->t_vid_start_render_pts);

            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxRmInfo->fgVDecIsPause = TRUE;
        }
#if 0  // !!!
        // audio only
        else if (prSwdmxRmInfo->fgEnAudio)
        {
            if (prSwdmxRmInfo->u1CurATrack < RM_MAX_AUDIO_TRACK)
            {
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4AudPts[prSwdmxRmInfo->u1CurATrack]);
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                AUD_SetStartPts(prSwdmxInst->u1AdecId, 0);
            }
        }
        // video only
        else if (prSwdmxRmInfo->fgEnVideo)
        {
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxRmInfo->fgVDecIsPause = TRUE;

            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);  //!!!
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxRmInfo->u4CurVidPts);
        }
#endif  // 0
        prSwdmxRmInfo->fgBeginToPlay = FALSE;
    }



    if (prSwdmxRmInfo->fgBeginToPlay &&
        prSwdmxInst->rCurRangeReq.fgSetRenderPosition == TRUE)
    {
        //UINT64 u8PtsDiff = 0;
        START_POS_INFO_T rRenderPos;
        UINT64 u8SeekPts;
#if defined(CC_DLNA_SUPPORT) && 0
        UINT32 u4FeederReadSize;

        // For DLNA playback
        u4FeederReadSize = (prSwdmxRmInfo->prDmxRangeInfo->z_range_sz < RM_PREBUFFER_SIZE) ?
                                        (prSwdmxRmInfo->prDmxRangeInfo->z_range_sz) : (RM_PREBUFFER_SIZE);

        LOG(5, "Feeder's ready bound = %d, underflw = %d",  u4FeederReadSize, u4FeederReadSize/8);

        FeederSetReadyThreshold(u4FeederReadSize);
        FeederSetUnderFlowThreshold(u4FeederReadSize/8);
#endif
        rRenderPos.i4_temporal_reference = prSwdmxInst->rCurRangeReq.i4TempRef;
        rRenderPos.ui2_decoding_order = prSwdmxInst->rCurRangeReq.u2DecodingOrder;
        rRenderPos.ui8_frame_position = prSwdmxInst->rCurRangeReq.u8Offset;
        rRenderPos.ui8_i_frame_position = prSwdmxInst->rCurRangeReq.u8OffsetI;
        rRenderPos.ui8_pts_info = prSwdmxInst->rCurRangeReq.u8StartPts;
        rRenderPos.ui8_i_pts_info = prSwdmxInst->rCurRangeReq.u8StartPtsI;
        rRenderPos.ui8_aud_pts_info = prSwdmxInst->rCurRangeReq.u8StartAudPts;
        rRenderPos.ui8_aud_frame_position = prSwdmxInst->rCurRangeReq.u8StartAudOffset;

        if(prSwdmxRmInfo->prVidIdxTbl)
        {
            rPTSInfo.u8Scale = (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
            rPTSInfo.u4Rate = prSwdmxRmInfo->prVidIdxTbl->ui4_rate;
        }

        if (_SWDMX_RmSeekPos(prSwdmxInst, &rRenderPos))
        {
             u8SeekPts = rRenderPos.ui8_pts_info;
        }        
        else
        {
            u8SeekPts = 0;
        }

        prSwdmxRmInfo->u8VSeekPts = u8SeekPts;
        prSwdmxRmInfo->u8ASeekPts = u8SeekPts;
        LOG(5, "Video Start Render position = %lld\n", u8SeekPts);

        if (prSwdmxRmInfo->fgEnVideo)
        {
            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, prSwdmxRmInfo->u8VSeekPts, &rPTSInfo);
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxRmInfo->u8VSeekPts);

            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxRmInfo->fgVDecIsPause = TRUE;
        }

        if (prSwdmxRmInfo->fgEnAudio && (prSwdmxRmInfo->u1CurATrack < RM_MAX_AUDIO_TRACK))
        {
            if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
            {
                prSwdmxRmInfo->fgCookPtsPending = TRUE;
            }
            else
            {
                prSwdmxRmInfo->u4TriggerAudPts = prSwdmxRmInfo->u8ASeekPts; 
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4TriggerAudPts);
            }
        }
       
        prSwdmxRmInfo->fgBeginToPlay = FALSE;
    }

    if (prSwdmxRmInfo->fgBeginToPlay &&
        i4PlaySpeed == 1000)
    {
        //UINT64 u8PtsDiff = 0;
#if defined(CC_DLNA_SUPPORT) && 0
        UINT32 u4FeederReadSize;

        // For DLNA playback
        u4FeederReadSize = (prSwdmxRmInfo->prDmxRangeInfo->z_range_sz < RM_PREBUFFER_SIZE) ?
                                        (prSwdmxRmInfo->prDmxRangeInfo->z_range_sz) : (RM_PREBUFFER_SIZE);

        LOG(5, "Feeder's ready bound = %d, underflw = %d",  u4FeederReadSize, u4FeederReadSize/8);

        FeederSetReadyThreshold(u4FeederReadSize);
        FeederSetUnderFlowThreshold(u4FeederReadSize/8);
#endif
        if(prSwdmxRmInfo->prVidIdxTbl)
        {
#if 1  // !!!
            rPTSInfo.u8Scale = (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
            rPTSInfo.u4Rate = prSwdmxRmInfo->prVidIdxTbl->ui4_rate;
#else
            if (prSwdmxRmInfo->prVidIdxTbl->ui4_scale &&
                prSwdmxRmInfo->prVidIdxTbl->ui4_rate)
            {
                u8Numerator =
                    (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
                u8Denominator =
                    (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_rate;
                u8PtsDiff = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
            }
            else
            {
                u8PtsDiff = 1;
            }
            prSwdmxRmInfo->u8PtsDiff = u8PtsDiff;
#endif
        }

        if (prSwdmxRmInfo->fgEnAudio && prSwdmxRmInfo->fgEnVideo)
        {
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxRmInfo->fgVDecIsPause = TRUE;

            if (prSwdmxRmInfo->u1CurATrack < RM_MAX_AUDIO_TRACK)
            {
#if 1
                prSwdmxRmInfo->u4TriggerAudPts = 0;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4TriggerAudPts);
#else
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4AudPts[prSwdmxRmInfo->u1CurATrack]);
                prSwdmxRmInfo->u4TriggerAudPts =
                    prSwdmxRmInfo->u4AudPts[prSwdmxRmInfo->u1CurATrack];
#endif
            }

            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);  //!!!
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxRmInfo->u4CurVidPts);
        }
        // audio only
        else if (prSwdmxRmInfo->fgEnAudio)
        {
            if (prSwdmxRmInfo->u1CurATrack < RM_MAX_AUDIO_TRACK)
            {
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4AudPts[prSwdmxRmInfo->u1CurATrack]);
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                AUD_SetStartPts(prSwdmxInst->u1AdecId, 0);
            }
        }
        // video only
        else if (prSwdmxRmInfo->fgEnVideo)
        {
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxRmInfo->fgVDecIsPause = TRUE;

            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, 0, &rPTSInfo);  //!!!
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxRmInfo->u4CurVidPts);
        }

        // set stc for starting playback
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxRmInfo->u4CurVidPts - 45000);

        prSwdmxRmInfo->fgBeginToPlay = FALSE;
    }

    // Reset some values for state change.
    if (prSwdmxRmInfo->fgChgATrack ||
        fgSpeedChange)
    {
        if (prSwdmxRmInfo->fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (prSwdmxRmInfo->u4VDecType)
            {
            case ENUM_SWDMX_VDEC_RV:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            default:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            }
            if (!DMX_MM_Set(prSwdmxRmInfo->u1VidPid, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }

            prSwdmxRmInfo->fgDeliverVEos = FALSE;
        }
        if (prSwdmxRmInfo->fgDeliverAEos)
        {
            prSwdmxRmInfo->fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
        }

#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
        prSwdmxRmInfo->fgAudIsReady = FALSE;

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    if (fgSpeedChange)
    {
        prSwdmxRmInfo->fgSpeedChange = TRUE;

        u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
        LOG(5, "Speed change while VDP Pts = 0x%08x\n", u4CurPts);

        prSwdmxRmInfo->u4NumSentAud = 0;

        // 1X -> 2X
        if ((prSwdmxRmInfo->i4PrevSpeed == 1000) &&
            (i4PlaySpeed == 2000))
        {
            if (prSwdmxRmInfo->u8RecDmxMoviPos != 0)
            {
                _SWDMX_FlushReqMsgQ(u1SrcId);
                prSwdmxRmInfo->i4ReqReadTimes = 0;
                prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
                prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] = prSwdmxRmInfo->u8RecDmxMoviPos;
            }
        }

        prSwdmxRmInfo->u8RecDmxMoviPos = 0;
        prSwdmxRmInfo->u4ModeChangPts = 0xFFFFFFFF;
        prSwdmxRmInfo->u4ChangeMode = 0;

        prSwdmxRmInfo->u4VidRepeatFrame = 0;
        prSwdmxRmInfo->fgRequestData = TRUE;

        prSwdmxRmInfo->u8VSeekPts = 0xFFFFFFFF;
        prSwdmxRmInfo->u8VSeekPts <<= 32;
        prSwdmxRmInfo->u8VSeekPts += 0xFFFFFFFF;

        prSwdmxRmInfo->u8ASeekPts = 0xFFFFFFFF;
        prSwdmxRmInfo->u8ASeekPts <<= 32;
        prSwdmxRmInfo->u8ASeekPts += 0xFFFFFFFF;

        if(prSwdmxRmInfo->fgVDecIsPause)
        {
            _SWDMX_RmVideoWakeUp(prSwdmxInst);
            prSwdmxRmInfo->fgVDecIsPause = FALSE;
        }

        if(prSwdmxRmInfo->prVidIdxTbl)
        {
#if 1  // !!!
            rPTSInfo.u8Scale = (UINT64)prSwdmxRmInfo->prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
            rPTSInfo.u4Rate = prSwdmxRmInfo->prVidIdxTbl->ui4_rate;
#endif
        }
        //seek 
        if (prCmd->fgSeek && (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))   // seek offset
        {
            if (_SWDMX_RmSeekPos(prSwdmxInst, &(prCmd->rSeekOffset)))
            {
                UINT64 u8SeekPts = prCmd->rSeekOffset.ui8_pts_info;

                LOG(5, "u8Seek I frame offset= %lld\n", u8SeekPts);

                prSwdmxRmInfo->u8VSeekPts = u8SeekPts;
                prSwdmxRmInfo->u8ASeekPts = u8SeekPts;

                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

                if(i4PlaySpeed >0)
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u8SeekPts);
                }
                else
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);
                }

                VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, u8SeekPts, NULL); // !!!

                DMX_MM_FlushBuffer(prSwdmxRmInfo->u1VidPid);
                DMX_MM_FlushBuffer(prSwdmxRmInfo->u1AudPid);

                if ((i4PlaySpeed == 1000) && (prSwdmxRmInfo->u1CurATrack < RM_MAX_AUDIO_TRACK))
                {
                    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
                    {
                        prSwdmxRmInfo->fgCookPtsPending = TRUE;
                    }
                    else
                    {
                        prSwdmxRmInfo->u4TriggerAudPts = prSwdmxRmInfo->u8ASeekPts;  // !!! prSwdmxRmInfo->u4AudPts[u1TrackIdx];
                        AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4TriggerAudPts);
                    }
                }

                // set stc
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, u8SeekPts);

                prSwdmxRmInfo->fgFeederInvalid = TRUE;

            }

        }
        else if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF)
                || ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
        {
            UNUSED(_SWDMX_RmSeek(prSwdmxInst, u8SeekPos, prRangeList, FALSE));
            LOG(5, "u8SeekPos = %lld\n", u8SeekPos);

            prSwdmxRmInfo->u8VSeekPts = u8SeekPos;
            prSwdmxRmInfo->u8ASeekPts = u8SeekPos;

            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

			if (INPUT_SRC == MM_SRC_TYPE_NETWORK_XUNLEI && i4PlaySpeed == 1)
			{
				VDEC_SetRenderFromPtsAutoPauseStartPts(prSwdmxInst->u1VdecId, u8SeekPos, TRUE, TRUE);
			}
			else if(i4PlaySpeed >0)
            {
				VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u8SeekPos);
            }
            else
            {
				VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);
            }

            VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE, u8SeekPos, NULL); // !!!
#if 0  // this will cause video freeze after time search during FR
            //VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u8SeekPos);  // !!!
#endif

            DMX_MM_FlushBuffer(prSwdmxRmInfo->u1VidPid);
            DMX_MM_FlushBuffer(prSwdmxRmInfo->u1AudPid);

            // set stc
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, u8SeekPos);
        }
        // 2X -> 1X, Step -> 1X, Slow Forward -> 1X
        else if(prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_NORMAL&&i4PlaySpeed==1000)
        {
            UINT8 u1TrackIdx = prSwdmxRmInfo->u1CurATrack;
            UINT32 u4AVPtsDiff  = 0;

            u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);

            if (prSwdmxRmInfo->i4PrevSpeed == 1)
            {
                STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
            }
            
			if (INPUT_SRC != MM_SRC_TYPE_NETWORK_XUNLEI)
			{
            u4CurPts -= prSwdmxRmInfo->i4RmPtsAdj;

            if ((prSwdmxRmInfo->i4PrevSpeed == 1) ||
                ((prSwdmxRmInfo->i4PrevSpeed < 1000) && (prSwdmxRmInfo->i4PrevSpeed > 1)))
            {
                STC_StartStc(prSwdmxInst->u1StcId);
            }
			}

            if (u1TrackIdx < RM_MAX_AUDIO_TRACK)
            {
                LOG(5, "RM Speed 2X -> 1X Cur Vid Pts = 0x%08x, Cur Aud Pts = 0x%08x\n",
                prSwdmxRmInfo->u4CurVidPts, prSwdmxRmInfo->u4PullBackAudPts);

                DMX_MM_FlushBuffer(prSwdmxRmInfo->u1AudPid);

                if (prSwdmxRmInfo->u4PullBackAudPts > u4CurPts)
                {
                    u4AVPtsDiff = prSwdmxRmInfo->u4PullBackAudPts - u4CurPts;
                }

                if (RM_VFIFO_THR_ON)
                {
                    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
                    {
                        prSwdmxRmInfo->dwCurCookFrameInSB = 0;
                        prSwdmxRmInfo->fgCookPtsPending = TRUE;
						prSwdmxRmInfo->u8ASeekPts = u4CurPts;
                    }
                    else
                    {
                        prSwdmxRmInfo->u4ModeChangPts = u4CurPts;//VDP_GetPts(prSwdmxInst->u1VdpId);
                    }
					
					// set stc
					STC_StopStc(prSwdmxInst->u1StcId);
					STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
	                STC_StartStc(prSwdmxInst->u1StcId);
                }
                else if (u4AVPtsDiff >= 45000)
                {
				    BOOL fgSeekRet=FALSE;
                    LOG(5, "u4AVPtsDiff > 45000\n");

                    if (prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] > prSwdmxRmInfo->u8RecDmxMoviPos)
                    {
                        prSwdmxRmInfo->u8RecDmxMoviPos = prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx];
                    }

					if(prSwdmxRmInfo->prVidKeyIdxTbl==NULL && prSwdmxRmInfo->fgEnVDmx)
				    {
					    fgSeekRet=_SWDMX_RmSeekCurPos(prSwdmxInst);
						if(prSwdmxRmInfo->i4PrevSpeed>2000)
						{
							MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
							VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
							DMX_MM_FlushBuffer(prSwdmxRmInfo->u1VidPid);
							_SWDMX_FlushReqMsgQ(u1SrcId);
							VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxRmInfo->u4CurVidPts);
							prSwdmxRmInfo->i4ReqReadTimes = 0;
                            prSwdmxRmInfo->fgReqDataMiss = FALSE;
							if(prSwdmxRmInfo->u4ADecType != ENUM_SWDMX_ADEC_RA)
							{
							    prSwdmxRmInfo->u4TriggerAudPts=prSwdmxRmInfo->u4CurVidPts;
								AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4CurAudPts);
							}
							STC_StopStc(prSwdmxInst->u1StcId);
                            STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxRmInfo->u4CurVidPts-25000);
						}
				    }
					else
					{
					    fgSeekRet=_SWDMX_RmSeek(prSwdmxInst, u4CurPts, prRangeList, FALSE);
					}
					
                    if (fgSeekRet)
                    {
#if 1  // !!!
						prSwdmxRmInfo->fgFeederInvalid = TRUE;
                        //prSwdmxRmInfo->u4ModeChangPts = u4CurPts + 27000;
                        prSwdmxRmInfo->u4ChangeMode |= RM_CHANGE_AUDIO_MODE;
                        if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
                        {
                            prSwdmxRmInfo->dwCurCookFrameInSB = 0;
                            prSwdmxRmInfo->fgCookPtsPending = TRUE;
                        }
#endif
                    }
                    else
                    {
                        ASSERT(0);  // !!!
                        prSwdmxRmInfo->u8RecDmxMoviPos = 0;
                        prSwdmxRmInfo->u4TriggerAudPts = prSwdmxRmInfo->u4PullBackAudPts;
                        AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4PullBackAudPts);
                    }
                }
                else
                {
                    //prSwdmxRmInfo->u4TriggerAudPts = prSwdmxRmInfo->u4PullBackAudPts;
                    //AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4PullBackAudPts);
                    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
                    {
                        prSwdmxRmInfo->dwCurCookFrameInSB = 0;
                        prSwdmxRmInfo->fgCookPtsPending = TRUE;
                    }
                    else
                    {
                        prSwdmxRmInfo->u4ModeChangPts = u4CurPts;
                    }
                }

                LOG(5, "Rec Movi Pos = %ld\n",
                    prSwdmxRmInfo->u8RecDmxMoviPos);
                LOG(5, "Cur Dmx Pos = %ld, \n",
                    prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx]);
            }
            else
            {
                LOG(1, "Out of Max Audio Track Number\n");
            }
        }
        else if (prSwdmxRmInfo->i4PrevSpeed < 0 &&
            (i4PlaySpeed == 2000 || i4PlaySpeed == 1000))
        {
            UNUSED(_SWDMX_RmSeek(prSwdmxInst, u4CurPts, prRangeList, FALSE));

            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

            DMX_MM_FlushBuffer(prSwdmxRmInfo->u1VidPid);
            DMX_MM_FlushBuffer(prSwdmxRmInfo->u1AudPid);

            _SWDMX_FlushReqMsgQ(u1SrcId);
            prSwdmxRmInfo->i4ReqReadTimes = 0;
            prSwdmxRmInfo->fgReqDataMiss = FALSE;
            prSwdmxRmInfo->fgFeederInvalid = TRUE;

            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,
                prSwdmxRmInfo->prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurSentKey].ui4_pts * 90);

            if ((i4PlaySpeed == 1000) &&
                (prSwdmxRmInfo->u1CurATrack < RM_MAX_AUDIO_TRACK))
            {
                if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
                {
                    prSwdmxRmInfo->fgCookPtsPending = TRUE;
                }
                else
                {
                    prSwdmxRmInfo->u4TriggerAudPts = prSwdmxRmInfo->prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurSentKey].ui4_pts * 90;  // !!! prSwdmxRmInfo->u4AudPts[prSwdmxRmInfo->u1CurATrack];

                    LOG(5, "VDP trigger audio = 0x%08x\n",
                        prSwdmxRmInfo->u4TriggerAudPts);

                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4TriggerAudPts);
                }
#if 1  // !!!
                VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE,
                    prSwdmxRmInfo->prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurSentKey].ui4_pts * 90,
                    &rPTSInfo);
#endif

                // set stc
                STC_StopStc(prSwdmxInst->u1StcId);
                STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
            }

            LOG(5, "RM Speed -32X/-16X/-8X/-4X/-2X -> 1X\n");
        }
        else if (((prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_IFRAME)
			       ||(prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_NORMAL&&prSwdmxRmInfo->i4PrevSpeed>2000))
			&& IS_TRICK_UNUSE_IDX(i4PlaySpeed))
        {
            // trick mode with index -> trick mode without index
            if(prSwdmxRmInfo->prVidKeyIdxTbl==NULL && prSwdmxRmInfo->fgEnVDmx)
		   	{
				UNUSED(_SWDMX_RmSeekCurPos(prSwdmxInst));
		   	}
		    else
		   	{
				UNUSED(_SWDMX_RmSeek(prSwdmxInst, u4CurPts, prRangeList, FALSE));
		   	}

            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

            DMX_MM_FlushBuffer(prSwdmxRmInfo->u1VidPid);
            DMX_MM_FlushBuffer(prSwdmxRmInfo->u1AudPid);

            _SWDMX_FlushReqMsgQ(u1SrcId);

            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);

            prSwdmxRmInfo->fgFeederInvalid = TRUE;

            // set stc
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);

            LOG(5, "RM Speed SF , FF 4x+  -> 2X, slow FF, start stc \n");

        }
        else if (!(prSwdmxRmInfo->i4PrevSpeed == 1000 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1)))
        {
            if (!IS_SLOW_FWD(i4PlaySpeed))
            {
            	UINT32 u4Pts = u4CurPts;
            	if (!(prSwdmxRmInfo->prVidIdxTbl))
            	{
            		UNUSED(_SWDMX_RmSeekCurPos(prSwdmxInst));
            	}
				else 
#if 1  // ToDo: water-proof it
				{
	                if (prSwdmxRmInfo->i4PrevSpeed == 2000 && i4PlaySpeed >= 4000)
	                {
	                    UNUSED(_SWDMX_RmSeek(prSwdmxInst, u4CurPts, prRangeList, TRUE));
	                }
	                else
#endif
	                {
	                    UNUSED(_SWDMX_RmSeek(prSwdmxInst, u4CurPts, prRangeList, FALSE));
	                }
					u4Pts = prSwdmxRmInfo->prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurSentKey].ui4_pts * 90;
				}
                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

                DMX_MM_FlushBuffer(prSwdmxRmInfo->u1VidPid);
                DMX_MM_FlushBuffer(prSwdmxRmInfo->u1AudPid);

                _SWDMX_FlushReqMsgQ(u1SrcId);
                prSwdmxRmInfo->i4ReqReadTimes = 0;
                prSwdmxRmInfo->fgReqDataMiss = FALSE;
                prSwdmxRmInfo->fgFeederInvalid = TRUE;

                // Set render pts
                if (i4PlaySpeed > 1000)
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,
                        u4Pts);
                    //avc test
                    VDEC_StartPTS(prSwdmxInst->u1VdecId, FALSE,
                        u4Pts,
                        /*(UINT32)prSwdmxRmInfo->u8PtsDiff*/&rPTSInfo);
                }
                else if (i4PlaySpeed <= 1000 && i4PlaySpeed > 0)
                {
#if 1  // ToDo: water-proof it
                    if (u4Pts!= u4CurPts)
                    {
                        LOG(0, "Key PTS != Cur PTS, %d vs %d!!!!!!!\n", u4Pts, u4CurPts);
                    }
#else
                    ASSERT(u4Pts== u4CurPts);
#endif
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4Pts/*u4CurPts !!!*/);
                    //avc test
                    VDEC_StartPTS(prSwdmxInst->u1VdecId, TRUE,
                        u4Pts,
                        /*(UINT32)prSwdmxRmInfo->u8PtsDiff*/&rPTSInfo);
                }
                else
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4Pts);
                    VDEC_StartPTS(prSwdmxInst->u1VdecId, FALSE,
                        u4Pts,
                        /*(UINT32)prSwdmxRmInfo->u8PtsDiff*/&rPTSInfo);
                }

                if (i4PlaySpeed > 2000 || i4PlaySpeed < 0)
                {
                    prSwdmxRmInfo->fgStartTrick = TRUE;
                }

                // Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
                if ((prSwdmxRmInfo->i4PrevSpeed > 2000 || prSwdmxRmInfo->i4PrevSpeed < 0) &&
                    (i4PlaySpeed == 1000) &&
                    (prSwdmxRmInfo->u1CurATrack < RM_MAX_AUDIO_TRACK))
                {
                    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
                    {
                        prSwdmxRmInfo->fgCookPtsPending = TRUE;
                    }
                    else
                    {
                        prSwdmxRmInfo->u4TriggerAudPts = u4Pts;  // !!! prSwdmxRmInfo->u4AudPts[u1TrackIdx];
                        AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4TriggerAudPts);
                    }

                    // set stc
                    STC_StopStc(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
                }
                LOG(5, "prSwdmxRmInfo->i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
                // don't need to handle 1X->2X
            }  // !IS_SLOW_FWD(i4PlaySpeed)
        }
    }

    if ((prSwdmxRmInfo->prCurVidKeyIdxTbl != 0) &&
        (prSwdmxInst->ePlayMode == SWDMX_PLAY_MODE_IFRAME))
    {
        u4PrsState = _SWDMX_RmHdrPrsUseIdx(prSwdmxInst, i4PlaySpeed, prRangeList);
    }
    else if(prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_NORMAL)
    {
        u4PrsState = _SWDMX_RmHdrPrsWithoutIdx(
                                                prSwdmxInst,
                                                prRangeList,
                                                 i4PlaySpeed,
                                                 u8SeekPos,
                                                 fgSpeedChange);
    }
	else
	{
       ASSERT(0);
	}

#ifdef RMLS
    if (prSwdmxRmInfo->fgRmLs && prSwdmxRmInfo->fgRmChgStrm)
    {
        ASSERT(prSwdmxRmInfo->fgEnAudio);
        prSwdmxRmInfo->fgRmChgStrm = FALSE;
        if (RM_STRM_VDO == prSwdmxRmInfo->u4RmStrmIdx)
        {
            prSwdmxRmInfo->u4RmStrmIdx = RM_STRM_AUD;
        }
        else if (RM_STRM_AUD == prSwdmxRmInfo->u4RmStrmIdx)
        {
            prSwdmxRmInfo->u4RmStrmIdx = RM_STRM_VDO;
        }
        else
        {
            LOG(6, "Invalid prSwdmxRmInfo->u4RmStrmIdx: %d\n", prSwdmxRmInfo->u4RmStrmIdx);
            ASSERT(0);
        }
        LOG(6, "Chg prSwdmxRmInfo->u4RmStrmIdx: %d\n", prSwdmxRmInfo->u4RmStrmIdx);
        prSwdmxRmInfo->fgFeederInvalid = TRUE;
    }
#endif

    prSwdmxRmInfo->i4PrevSpeed = i4PlaySpeed;
    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}

static BOOL _SWDMX_TxAACFrame(SWDMX_INFO_T *prSwdmxInst, UINT16 u2FrameSize)
{
    UINT8 u1SampleRateIdx;
    DMX_MM_DATA_T rDmxMMData;
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    if(prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_AAC &&
        prSwdmxRmInfo->pucAudBuf == NULL)
    {
        #ifdef SWDMX_BUFFER_REFINE
        UINT32 ui4Size  = AAC_ALLOC_SIZE;
        UINT32 ui4Addr  = 0;
        UINT32 ui4Align = 16;
        SWDMX_GetInfo(prSwdmxInst->u1SwdmxId, eSWDMX_GET_INTERNAL_BUF, &ui4Addr, &ui4Size, &ui4Align);
        prSwdmxRmInfo->pucAudBuf = (UCHAR *)ui4Addr;
        #else
        prSwdmxRmInfo->pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(RM_MAX(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        #endif
        
        if(prSwdmxRmInfo->pucAudBuf == NULL)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            ASSERT(0);
            return FALSE;
        }
    }

    u2FrameSize += AAC_FAKE_HDR_SIZE;
    x_memset(&rDmxMMData, 0, sizeof(rDmxMMData));

    /* 8b: syncword */
    prSwdmxRmInfo->pucAudBuf[0] = 0xFF;
    /* 4b: syncword */
    /* 1b: mpeg id = 1 */
    /* 2b: layer = 0 */
    /* 1b: protection absent */
    prSwdmxRmInfo->pucAudBuf[1] = 0xF9;
    /* 2b: profile */
    /* 4b: sampling_frequency_index */
    /* 1b: private = 0 */
    /* 1b: channel_configuration */
    switch (prSwdmxRmInfo->u4AudSampleRate)
    {
    case 96000:
        u1SampleRateIdx = 0;
        break;
    case 88000:
        u1SampleRateIdx = 1;
        break;
    case 64000:
        u1SampleRateIdx = 2;
        break;
    case 48000:
        u1SampleRateIdx = 3;
        break;
    case 44100:
        u1SampleRateIdx = 4;
        break;
    case 32000:
        u1SampleRateIdx = 5;
        break;
    case 24000:
        u1SampleRateIdx = 6;
        break;
    case 22050:
        u1SampleRateIdx = 7;
        break;
    case 16000:
        u1SampleRateIdx = 8;
        break;
    case 12000:
        u1SampleRateIdx = 9;
        break;
    case 11025:
        u1SampleRateIdx = 10;
        break;
    case 8000:
        u1SampleRateIdx = 11;
        break;
    default:
        u1SampleRateIdx = 4;
        break;
    }

    prSwdmxRmInfo->pucAudBuf[2] = (1 << 6) |
             ((u1SampleRateIdx<< 2) & 0x3C) |
             ((prSwdmxRmInfo->u4AudChannelNs >> 2) & 0x1);
    /* 2b: channel_configuration */
    /* 1b: original */
    /* 1b: home */
    /* 1b: copyright_id */
    /* 1b: copyright_id_start */
    /* 2b: aac_frame_length */
    prSwdmxRmInfo->pucAudBuf[3] = ((prSwdmxRmInfo->u4AudChannelNs & 0x3) << 6) |
             ((u2FrameSize >> 11) & 0x3);
    /* 8b: aac_frame_length */
    prSwdmxRmInfo->pucAudBuf[4] = ((u2FrameSize >> 3) & 0xFF);
    /* 3b: aac_frame_length */
    /* 5b: adts_buffer_fullness */
    prSwdmxRmInfo->pucAudBuf[5] = ((u2FrameSize << 5) & 0xE0) |
             ((0x7FF >> 6) & 0x1F);
    /* 6b: adts_buffer_fullness */
    /* 2b: number_of_raw_data_blocks_in_frame */
    prSwdmxRmInfo->pucAudBuf[6] = ((0x7FF << 2) & 0xFC);

    rDmxMMData.u1Idx = prSwdmxRmInfo->u1AudPid;
    rDmxMMData.u4BufStart = (UINT32)prSwdmxRmInfo->pucAudBuf;
    rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + AAC_ALLOC_SIZE;
    rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
    rDmxMMData.u4FrameSize = 7;
    rDmxMMData.u4Pts = prSwdmxRmInfo->rRmDmxMMData.u4Pts;  //0;   !!!

    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
    {
        LOG(0, "Demuxer fails to move data.\n");
        return FALSE;
    }

    return TRUE;
}

static BOOL _CheckRmPacketType(SWDMX_INFO_T *prSwdmxInst, UCHAR *pucBuf, UINT8 *pu1HdrType, UINT8 *pu1TrackIdx, BOOL *pfgVer1, UINT32 *pu4Timestamp, UINT16 *pu2Flag, UINT32 *pu4PktSize)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    INT32 i;
    BOOL fgStrmIDChecked[3];
    UINT32 u4Pts;
    UINT16 u2Flag;

    SWDMX_LOADB_DWRD(pucBuf + 6, u4Pts);
    *pu4Timestamp = u4Pts;

#if 0 // david:move this code to below for object version 1
    SWDMX_LOADB_WORD(pucBuf + 10, u2Flag);
    *pu2Flag = u2Flag;
#endif

    SWDMX_LOADB_WORD(pucBuf + 2, u2Flag);
    *pu4PktSize = u2Flag;
    LOG(7, "RM packet size = %d\n", *pu4PktSize);

    fgStrmIDChecked[0] = fgStrmIDChecked[1] = fgStrmIDChecked[2] = FALSE;

    if (_CheckRmPacketIndex(pucBuf, prSwdmxRmInfo->u4VStrmID, pfgVer1) == TRUE)
    {
        if (prSwdmxRmInfo->fgEnVideo)
            *pu1HdrType = eSWDMX_MEDIA_VIDEO;
        else
            *pu1HdrType = eSWDMX_MEDIA_SKIPPED;

        goto RM_CHK_PKT_TYPE_RTN_TRUE;
    }

    if (prSwdmxRmInfo->u4VStrmID < 3)
        fgStrmIDChecked[prSwdmxRmInfo->u4VStrmID] = TRUE;

    for (i = 0; (i < prSwdmxRmInfo->u1TotalATrack) && (i < RM_MAX_AUDIO_TRACK); i++)
    {
        if (_CheckRmPacketIndex(pucBuf, prSwdmxRmInfo->u4AStrmID, pfgVer1) == TRUE)
        {
            *pu1HdrType = eSWDMX_MEDIA_AUDIO;
            goto RM_CHK_PKT_TYPE_RTN_TRUE;
        }
    }

    if (prSwdmxRmInfo->u4AStrmID < 3)
        fgStrmIDChecked[prSwdmxRmInfo->u4AStrmID] = TRUE;

    // tmp solution for MW can't set video stream ID if video codec is not supported
    for (i = 0; i < 3; i++)
    {
        if (!fgStrmIDChecked[i])
        {
            if (_CheckRmPacketIndex(pucBuf, (UINT32)i, pfgVer1))
            {
                *pu1HdrType = eSWDMX_MEDIA_SKIPPED;
                goto RM_CHK_PKT_TYPE_RTN_TRUE;
            }
        }
    }

    // tmp solution for files having other streams with ID from 3 to 9
    for (i = 3; i < 10; i++)
    {
        if (_CheckRmPacketIndex(pucBuf, (UINT32)i, pfgVer1))
        {
            *pu1HdrType = eSWDMX_MEDIA_SKIPPED;
            goto RM_CHK_PKT_TYPE_RTN_TRUE;
        }
    }

    return FALSE;

RM_CHK_PKT_TYPE_RTN_TRUE:
    if (*pfgVer1)
    {
        SWDMX_LOADB_WORD(pucBuf + 11, u2Flag);
        *pu2Flag = u2Flag;
    }
    else
    {
        SWDMX_LOADB_WORD(pucBuf + 10, u2Flag);
        *pu2Flag = u2Flag;
    }

    return TRUE;
}

static BOOL _CheckRmPacketIndex(UCHAR *pucBuf, UINT32 u4StrmID, BOOL *pfgVer1)
{
    INT32 i;
    UCHAR ucStreamID[2];

    // check object version
    if (pucBuf[0] != 0x00)
        return FALSE;
    if (pucBuf[1] != 0x00 && pucBuf[1] != 0x01)
        return FALSE;
    if (pucBuf[1] == 0x01)
        *pfgVer1 = TRUE;
    else
        *pfgVer1 = FALSE;

    ucStreamID[1] = (u4StrmID >> 0) & 0xff;
    ucStreamID[0] = (u4StrmID >> 8) & 0xff;

    for (i = 0; i < 2; i++)
    {
        if (pucBuf[i + 4] != ucStreamID[i])
        {
            return FALSE;
        }
    }

    if (pucBuf[2] == 0 && pucBuf[3] == 0)  // check the packet size
        return FALSE;

    return TRUE;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsRmPacket(
    SWDMX_INFO_T *prSwdmxInst,
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd,
    BOOL fgOneVideoFrm)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    COOK_INFO_T *prCookInfo = &prSwdmxRmInfo->rCookInfo;
    FEEDER_BUF_INFO_T *prFeederInfo = &prSwdmxRmInfo->rFeederInfo;
    BOOL fgRingBuf = FALSE;
    BOOL fgFindInRing = FALSE;
    BOOL fgFindPkt = FALSE;
    BOOL fgVer1 = FALSE;

    UINT8 u1HdrType;
    UINT8 u1TrackIdx = 0;

    UINT32 i = 0, j = 0;
    UINT32 u4RingSize = 0, u4RingStart = 0;//, u4OrgRingSize = 0;
    //UINT32 u4TotalConsumeSize = 0;
    UINT32 u4PacketSize, u4DmxAvailSize;
    UINT32 u4Pts = 0;
    UINT16 u2Flag;
    UINT32 u4OrgAvailSize = u4AvailSize;
    UINT32 u4SrcBufLen;

    //UINT64 u8Numerator, u8Denominator;
    //UINT64 u8Remainder;

    UCHAR ucRingBuf[RM_PARSER_OFFSET];
    UCHAR ucTempBuf[RM_PACKET_ID_OFFSET + 10];  // "+10": tmp solution for Klocwork warning
    UCHAR* pucOriginalPtr = pucBitstream;

    //DMX_MM_DATA_T prSwdmxRmInfo->rRmDmxMMData;  megaa 20090601: change it to global, tmp solution

    UINT32 u4OldNumSentVid = prSwdmxRmInfo->u4NumSentVid;

    UINT8 bTmpVal;

    x_memset(&prSwdmxRmInfo->rRmDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (prSwdmxRmInfo->fgCookSBPending)
    {
        vCookFrameDeInterleave(prSwdmxInst, NULL);  // this will clear fgCookSBPending or not depending on the current A FIFO condition
#ifdef RMLS
        if (prSwdmxRmInfo->fgRmChgStrm)
        {
            return eSWDMX_HDR_PRS_SUCCEED;
        }
#endif
    }

    if (u4AvailSize < RM_PARSER_OFFSET)
    {
        *pu4ConsumeSize = 0;
    }

    while (i < u4AvailSize)
    {
        if (((pucBitstream + i) >= (UCHAR*)(VIRTUAL(prFeederInfo->u4EndAddr - RM_PARSER_OFFSET))) &&
            ((u4AvailSize - i) >= RM_PARSER_OFFSET) && (!fgRingBuf))
        {
            fgRingBuf = TRUE;
            u4RingSize = (UINT32)(VIRTUAL(prFeederInfo->u4EndAddr)) - (UINT32)(pucBitstream + i);
            //u4OrgRingSize = u4RingSize;

            LOG(0, "Ring Buffer start from 0x%08x to 0x%08x size = 0x%08x\n", pucBitstream,
                VIRTUAL(prFeederInfo->u4EndAddr), u4RingSize);

            x_memcpy(ucRingBuf, (pucBitstream + i), u4RingSize);

            u4AvailSize -= (u4RingSize + i);
            pucBitstream = (UCHAR*)(VIRTUAL(prFeederInfo->u4StartAddr));
            i = 0;
        }

        if ((fgRingBuf) && (u4RingSize))
        {
            // Handle the ring buffer case
            if (u4RingSize > RM_PACKET_ID_OFFSET)
            {
                for (j = 0; j < RM_PACKET_ID_OFFSET; j++)
                {
                    ucTempBuf[j] = ucRingBuf[u4RingStart + j];
                }
            }
            else
            {
                for (j = 0; j < u4RingSize; j++)
                {
                    ucTempBuf[j] = ucRingBuf[u4RingStart + j];
                }
                for (; j < RM_PACKET_ID_OFFSET; j++)
                {
                    ucTempBuf[j] = pucBitstream[j - u4RingSize];
                }
            }

            if (_CheckRmPacketType(prSwdmxInst, ucTempBuf, &u1HdrType, &u1TrackIdx, &fgVer1, &u4Pts, &u2Flag, &u4PacketSize) == TRUE)
            {
                if (u4PacketSize > 12)
                {
                    fgFindPkt = TRUE;
                    fgFindInRing = TRUE;
                    LOG(0, "set fgFindInRing TRUE!!!!!!\n");
                    break;
                }
            }
            u4RingStart++;
            u4RingSize--;
            continue;
        }

        if (_CheckRmPacketType(prSwdmxInst, pucBitstream + i, &u1HdrType, &u1TrackIdx, &fgVer1, &u4Pts, &u2Flag, &u4PacketSize) == TRUE)
        {
            if (u4PacketSize > 12)
            {
                fgFindPkt = TRUE;
                break;
            }
        }

        if (i == 0 &&  // tmp solution
            *pucBitstream == 'D' &&
            *(pucBitstream+1) == 'A' &&
            *(pucBitstream+2) == 'T' &&
            *(pucBitstream+3) == 'A')
            i = 18;
        else
            i++;
    }

    // Didn't find the packet
    if (!fgFindPkt)
    {
        *pu4ConsumeSize = u4OrgAvailSize;
        return eSWDMX_HDR_PRS_FAIL;
    }

    bTmpVal = u2Flag & 0xFF;

    if ((!fgRingBuf) || (!fgFindInRing))
    {
        if (fgVer1)
        {
            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET + i);
        }
        else
        {
            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET - 1 + i);
        }
    }
    else
    {
        ASSERT(0);  // !!! verify the following code
        if (fgVer1)
        {
            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET + i);
        }
        else
        {
            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET - 1 + i);
        }
        //pucTemp = pucBitstream + RM_PACKET_ID_OFFSET + i;  // only to shut down Lint warning
#if 0
        if (u4RingSize == 0)
        {
            //pucTemp = pucBitstream;
            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PACKET_ID_OFFSET);
        }
        else
        {
            LOG(5, "fgFindInRing == TRUE u4RingSize = 0x%08x\n", u4RingSize);
            if (u4RingSize <= RM_PACKET_ID_OFFSET)
            {
                //pucTemp = (UCHAR*)(pucBitstream + (RM_PACKET_ID_OFFSET - u4RingSize));
            }
            else
            {
                u4RingStart += RM_PACKET_ID_OFFSET;

                for (j = 0; j < (u4OrgRingSize - u4RingStart); j++)
                {
                    ucTempBuf[j] = ucRingBuf[u4RingStart + j];
                }
                for (j = 0; j < (RM_PACKET_ID_OFFSET - (u4RingSize - u4RingStart)); j++)
                {
                    ucTempBuf[j + (u4OrgRingSize - u4RingStart)] = pucBitstream[j];
                }
                //pucTemp = ucTempBuf;
            }

            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + (RM_PARSER_OFFSET - u4RingSize));
        }
#endif
    }

    if (prSwdmxRmInfo->rRmDmxMMData.u4StartAddr >= (UINT32)(VIRTUAL(prFeederInfo->u4EndAddr)))
        ASSERT(0);

#if 1  // !!!
    if (prSwdmxRmInfo->u8RecDmxMoviPos)
    {
        LOG(7, "CurDmxMoviPos when mode Change = %ld\n", prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx]);
        if (prSwdmxRmInfo->u8RecDmxMoviPos != prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx])
        {
            // Find the first audio chunk since 2X -> 1X
            if (u1HdrType == eSWDMX_MEDIA_AUDIO)
            {
              if (prSwdmxRmInfo->u4ChangeMode & RM_CHANGE_AUDIO_MODE)
            {
                UINT8 u1ATrack = prSwdmxRmInfo->u1CurATrack;
                UINT32 u4ModeChangePts = VDP_GetPts(prSwdmxInst->u1B2rId);

                if (u1ATrack < RM_MAX_AUDIO_TRACK)
                {
                    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA && !(bTmpVal & HX_KEYFRAME_FLAG))  // COOK case
                    {
                        prSwdmxRmInfo->u4PullBackAudPts = u4Pts * 90;  // record the PTS for later usage
                        u1HdrType = eSWDMX_MEDIA_SKIPPED;
                    }
                    else if (u4Pts * 90 >= u4ModeChangePts - prSwdmxRmInfo->i4RmPtsAdj)  // AAC case or COOK with key frame flag
                    {
                        LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n",
                            u4Pts * 90,
                            u4ModeChangePts);
                        //VDP_TriggerAudReceive(prSwdmxRmInfo->u4AudPts[u1ATrack]);
                        prSwdmxRmInfo->u4TriggerAudPts = u4Pts * 90;
                        AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4TriggerAudPts);
                        prSwdmxRmInfo->u4ChangeMode = 0;
                        prSwdmxRmInfo->u4ModeChangPts = 0xFFFFFFFF;
                        prSwdmxRmInfo->u4PullBackAudPts = 0xFFFFFFFF;
                    }
#if 1
                    else
#else
                    else if ((u4Pts * 90 < prSwdmxRmInfo->u4ModeChangPts) &&
                        (prSwdmxRmInfo->u4ModeChangPts != 0xFFFFFFFF))
#endif
                    {
                        LOG(5, "Audio Pts (0x%08x) < Mode ChangePts (0x%08x)\n",
                            u4Pts * 90,
                            u4ModeChangePts);

                        //_SWDMX_RmCalculatePts(u1HdrType, u1ATrack, u4PacketSize);  !!!
                        prSwdmxRmInfo->u4PullBackAudPts = u4Pts * 90;  // record the PTS for later usage
                        u1HdrType = eSWDMX_MEDIA_SKIPPED;
                    }
                }
              }
            }
            else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
            {
                ASSERT(0);
            }
            else
            {
                //_SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);  !!!

                // Skip other chunks
                u1HdrType = eSWDMX_MEDIA_SKIPPED;

                if (prSwdmxRmInfo->u8RecDmxMoviPos < prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] + u4PacketSize)
                {
                    LOG(5, "rSwdmxRmInfo.u8RecDmxMoviPos < rSwdmxRmInfo.au8CurDmxFilePos[rSwdmxRmInfo.u4RmStrmIdx] + %d\n", u4PacketSize);
                    prSwdmxRmInfo->u8RecDmxMoviPos = 0;
                    prSwdmxRmInfo->u4ChangeMode = 0;
                }
            }
        }
        else
        {
            LOG(5, "rSwdmxRmInfo.u8RecDmxMoviPos == rSwdmxRmInfo.au8CurDmxFilePos[rSwdmxRmInfo.u4RmStrmIdx]\n");
            prSwdmxRmInfo->u8RecDmxMoviPos = 0;
            prSwdmxRmInfo->u4ChangeMode = 0;
        }
    }
#endif

    // Error Handling
    if (u4PacketSize > RM_BUFFER_SIZE)
    {
        LOG(3, "u4PacketSize > RM_BUFFER_SIZE Chunk size = 0x%08x\n", u4PacketSize);
        *pu4ConsumeSize = u4OrgAvailSize;
        return eSWDMX_HDR_PRS_FAIL;
    }
    else if (u4PacketSize > u4AvailSize)
    {
        LOG(3, "Chunk size is 0x%08x larger than feeder's, the size is 0x%08x\n", u4PacketSize, u4AvailSize);

        x_thread_delay(10);

        if (!fgRangeEnd)
        {
            prSwdmxRmInfo->fgIsBlock = TRUE;
            //prSwdmxRmInfo->u4BlockChunkSize = u4PacketSize;  !!!
            *pu4ConsumeSize = 0;
        }
        else
        {
            *pu4ConsumeSize = u4AvailSize;
            LOG(5, "Range End!!!\n");
        }

        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
    }
    else if (!u4PacketSize)
    {
        ASSERT(0);  // !!! ToDo: add error handling
        *pu4ConsumeSize = RM_PARSER_OFFSET + i;
        return eSWDMX_HDR_PRS_FAIL;
    }

    // Disable streams here
    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        if (prSwdmxRmInfo->fgCookSBPending)  // still pending, give up this turn
        {
            *pu4ConsumeSize = 0;
            x_thread_delay(1);
			prSwdmxInst->fgFifoFull = TRUE;
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

        u1TrackIdx = prSwdmxRmInfo->u1CurATrack;
#if 1
// temp solution
// u4AStrmID is set to 0xffffffff when init, but stop will set u4AStrmID to 0
// So we add checking fgEnAudio to workaround
        if (!prSwdmxRmInfo->fgEnAudio)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
        else
#endif
        if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA && prCookInfo->u1FlavorID > 29)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
        else if (i4PlaySpeed != SWDMX_PLAY_SPEED_1X)
        {
            //_SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);  !!!
            //prSwdmxRmInfo->u4NumSentAud++;
            prSwdmxRmInfo->u4PullBackAudPts = u4Pts * 90;  // record the PTS for later usage

            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
#if 1  // !!!
        else if (prSwdmxRmInfo->u4ModeChangPts != 0xFFFFFFFF)
        {
            if (u1TrackIdx < RM_MAX_AUDIO_TRACK)
            {
                if (u4Pts * 90 >= prSwdmxRmInfo->u4ModeChangPts)
                {
                    LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n",
                        u4Pts * 90,
                        prSwdmxRmInfo->u4ModeChangPts);
                    //VDP_TriggerAudReceive(u4Pts * 90);
                    prSwdmxRmInfo->u4TriggerAudPts = u4Pts * 90;
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, u4Pts * 90);
                    prSwdmxRmInfo->u4ModeChangPts = 0xFFFFFFFF;
                    prSwdmxRmInfo->u4ChangeMode = 0;
                }
                else
                {
                    _SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);
                    u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
            }
        }
        else if(((UINT32)(prSwdmxRmInfo->u8ASeekPts>>32) != 0xFFFFFFFF) ||
            ((UINT32)(prSwdmxRmInfo->u8ASeekPts) != 0xFFFFFFFF))
        {
            if((u1TrackIdx < RM_MAX_AUDIO_TRACK) &&
                (u4Pts * 90 < (UINT32)prSwdmxRmInfo->u8ASeekPts))
            {
                _SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);
                u1HdrType = eSWDMX_MEDIA_SKIPPED;
            }
        }
#endif
    }
    else if (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
        //_SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);  !!!
    }

    if (u1HdrType == eSWDMX_MEDIA_SKIPPED ||
        u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
        *pu4ConsumeSize = u4PacketSize + i;
        if (fgRingBuf)
        {
            *pu4ConsumeSize += ((UINT32)(VIRTUAL(prFeederInfo->u4EndAddr)) -
                                                               (UINT32)(pucOriginalPtr) -
                                                               u4RingSize);
        }
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Get available size here (we don't have to query subtitle available size here)
    if (u1HdrType != eSWDMX_MEDIA_SUBTITLE)
    {
        if (eSWDMX_MEDIA_VIDEO == u1HdrType)
        {
            u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prSwdmxRmInfo->u1VidPid);
        }
        else if (eSWDMX_MEDIA_AUDIO == u1HdrType)
        {
            u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, prSwdmxRmInfo->u1AudPid);
        }
        else
        {
            u4DmxAvailSize = 0;
            LOG(0, "Unexpected u1HdrType: %d @ %s,%d\n", u1HdrType, __FUNCTION__, __LINE__);
            ASSERT(0);
        }

        if ((u4PacketSize - (fgVer1 ? 13 : 12))> u4DmxAvailSize)
        {
            if (prSwdmxRmInfo->fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp(prSwdmxInst);
#ifdef GOOD
                prSwdmxRmInfo->fgVDecIsPause = FALSE;
#else
                //prSwdmxRmInfo->fgVDecIsPause = FALSE;  !!!
#endif
            }

            if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
                !prSwdmxRmInfo->fgEnVideo &&
                prSwdmxRmInfo->fgAudIsReady)
            {
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                prSwdmxRmInfo->fgAudIsReady = FALSE;
            }

            LOG(7, "Chunk is blocked u1HdrType = %ld, u4PacketSize = %ld, u4DmxAvailSize = %ld\n",
                u1HdrType,
                u4PacketSize,
                u4DmxAvailSize);
            *pu4ConsumeSize = 0;
            x_thread_delay(10);

#ifdef RMLS
            if (prSwdmxRmInfo->fgRmLs)
            {
                if (RM_STRM_VDO == prSwdmxRmInfo->u4RmStrmIdx)
                {
                    if (prSwdmxRmInfo->fgDeliverAEos)
                    {
                        // Audio is finished, no need to change to audio
                    }
                    else if (!prSwdmxRmInfo->fgEnAudio)
                    {
                        // Audio is off, no need to change to audio
                    }
                    else
                    {
                        LOG(3, "V fifo full:Set prSwdmxRmInfo->fgRmChgStrm\n");
                        prSwdmxRmInfo->fgRmChgStrm = TRUE;
                    }
                }
                else if (RM_STRM_AUD == prSwdmxRmInfo->u4RmStrmIdx)
                {
                    if (prSwdmxRmInfo->fgDeliverVEos)
                    {
                        // Video is finished, no need to change to video
                    }
                    else
                    {
                        LOG(3, "A fifo full:Set prSwdmxRmInfo->fgRmChgStrm\n");
                        prSwdmxRmInfo->fgRmChgStrm = TRUE;
                    }
                }
                else
                {
                    LOG(0, "Invalid prSwdmxRmInfo->u4RmStrmIdx: %d\n", prSwdmxRmInfo->u4RmStrmIdx);
                    ASSERT(0);
                }
            }
#endif
			prSwdmxInst->fgFifoFull = TRUE;
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

        _SWDMX_RmInitDmx(prSwdmxInst, u1HdrType, FALSE);
    }

    prSwdmxRmInfo->rRmDmxMMData.u4FrameSize = u4PacketSize - (fgVer1 ? 13 : 12);

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
#ifdef COOK_WRITE_AFIFO
        if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA && prSwdmxRmInfo->fgAFifoForCookSBFull)
        {
            if (dwAFifoForCookSBSpace(prSwdmxInst) <= prCookInfo->u2CookSBSz)
            {
                LOG(7, "COOK deinterleave is blocked because of AFifo near full...\n");
                *pu4ConsumeSize = 0;
                x_thread_delay(10);
				prSwdmxInst->fgFifoFull = TRUE;
                return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
            }
            else
            {
                LOG(7, "COOK deinterleave re-turned ON!!!\n");
                prSwdmxRmInfo->fgAFifoForCookSBFull = FALSE;
            }
        }
#endif
        u1TrackIdx = prSwdmxRmInfo->u1CurATrack;

        if (u1TrackIdx < RM_MAX_AUDIO_TRACK)
        {
            UINT32 u4TimeSeekAudPts = u4Pts * 90;  //prSwdmxRmInfo->u4AudPts[u1TrackIdx];  !!!

            _SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);
        prSwdmxRmInfo->u4NumSentAud++;
            //u4Pts = prSwdmxRmInfo->u4AudPts[u1TrackIdx];

            //enable audio according to pts, need fix by aud index table
#if 1  // !!!
            if(((UINT32)(prSwdmxRmInfo->u8ASeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(prSwdmxRmInfo->u8ASeekPts) != 0xFFFFFFFF))
            {
                if ((u4TimeSeekAudPts >= (UINT32)prSwdmxRmInfo->u8ASeekPts) &&
                    ((prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_AAC) ||
                     ((prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA) && (bTmpVal & HX_KEYFRAME_FLAG))))
                {
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, u4TimeSeekAudPts);

                    if (prSwdmxRmInfo->fgEnVideo)
                    {
                        VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                        prSwdmxRmInfo->u4TriggerAudPts = (UINT32)u4TimeSeekAudPts;

                        VDEC_Pause(prSwdmxInst->u1VdecId);
                        prSwdmxRmInfo->fgVDecIsPause = TRUE;
                    }

                    LOG(7, "Got Aud T PTS 0x%x, Seek 0x%x\n",
                        u4TimeSeekAudPts, (UINT32)prSwdmxRmInfo->u8ASeekPts);
                    prSwdmxRmInfo->u8ASeekPts = 0xFFFFFFFF;
                    prSwdmxRmInfo->u8ASeekPts <<= 32;
                    prSwdmxRmInfo->u8ASeekPts += 0xFFFFFFFF;
                }
                else
                {
                    //skip audio data in seek process
                    *pu4ConsumeSize = u4PacketSize + i;
                    if (fgRingBuf)
                    {
                        *pu4ConsumeSize += ((UINT32)(VIRTUAL(prFeederInfo->u4EndAddr)) -
                                                                   (UINT32)(pucOriginalPtr) -
                                                                   u4RingSize);
                    }
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
            }
#endif
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        // !!!
#if 1
        UINT16 u2MaxQNum, u2CurQNum;

        if (prSwdmxRmInfo->fgEnVideo == FALSE)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }

        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
#ifdef GOOD
        if (u2CurQNum > RM_VFIFO_THR_PIC_NS) //60/*(prSwdmxRmInfo->u4VidFps * 3)*/)
#else
        if (u2CurQNum > 60/*(prSwdmxRmInfo->u4VidFps * 3)*/ && 0)
#endif
        {
  			prSwdmxInst->fgFifoFull= TRUE;
            if (prSwdmxRmInfo->fgVDecIsPause)
            {
                if (!RM_VFIFO_THR_ON ||(RM_VFIFO_THR_ON && VDP_GetStepForward(prSwdmxInst->u1B2rId) != 2))
                {
                _SWDMX_RmVideoWakeUp(prSwdmxInst);
                prSwdmxRmInfo->fgVDecIsPause = FALSE;
                }
            }
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            *pu4ConsumeSize = 0;
            x_thread_delay(10);
            return eSWDMX_HDR_PRS_SUCCEED;
        }

        //u4Pts = prSwdmxRmInfo->u4CurVidPts;

        _SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);

        // Record every video's chunk position.
        //prSwdmxRmInfo->u8VidOffset = prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx];  !!!

        if(((UINT32)(prSwdmxRmInfo->u8VSeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(prSwdmxRmInfo->u8VSeekPts) != 0xFFFFFFFF))
        {
            if(u4Pts > (UINT32)prSwdmxRmInfo->u8VSeekPts)
            {
                LOG(5, "Got Vid T PTS 0x%x, Seek 0x%x\n",
                    u4Pts, (UINT32)prSwdmxRmInfo->u8VSeekPts);
                prSwdmxRmInfo->u8VSeekPts = 0xFFFFFFFF;
                prSwdmxRmInfo->u8VSeekPts <<= 32;
                prSwdmxRmInfo->u8VSeekPts += 0xFFFFFFFF;
            }
            /*else if (prRangeList->prDmxRangeInfo->ui8_vid_duration < (UINT32)prSwdmxRmInfo->u8VSeekPts)
            {
                if (!prSwdmxRmInfo->fgDeliverVEos)
                {
                    LOG(5, "prRangeList->prDmxRangeInfo->ui8_vid_duration < (UINT32)prSwdmxRmInfo->u8VSeekPts\n");
                    if (!_SWDMX_SendRmEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                    {
                        LOG(3, "Fail to send Video Eos\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                    prSwdmxRmInfo->fgDeliverVEos = TRUE;
                }
            }*/
        }
#endif
    }

    prSwdmxRmInfo->rRmDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prFeederInfo->u4StartAddr));
    prSwdmxRmInfo->rRmDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prFeederInfo->u4EndAddr));
    u4SrcBufLen = prSwdmxRmInfo->rRmDmxMMData.u4BufEnd - prSwdmxRmInfo->rRmDmxMMData.u4BufStart;
    prSwdmxRmInfo->rRmDmxMMData.u4Pts = u4Pts * 90;
    prSwdmxRmInfo->rRmDmxMMData.u4Dts = u4Pts * 90;

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
      if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_AAC)
      {
        UINT32 u4Ptr = prSwdmxRmInfo->rRmDmxMMData.u4StartAddr;
        UINT16 u2NumAACFrm, u2CurAACFrmIdx, u2CurAACFrmSz;
        UINT32 u4BufStartBak;

        SWDMX_LOADB_WORD(/*0x30000000 + */u4Ptr, u2NumAACFrm);
        u4Ptr += 2;
        if (u4Ptr >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
        {
            if (u4Ptr == prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
            {
                u4Ptr = prSwdmxRmInfo->rRmDmxMMData.u4BufStart;
            }
            else if (u4Ptr == prSwdmxRmInfo->rRmDmxMMData.u4BufEnd + 1)
            {
                u4Ptr = prSwdmxRmInfo->rRmDmxMMData.u4BufStart + 1;
                u2NumAACFrm = (u2NumAACFrm & 0xFF00) | *(UINT8 *)(prSwdmxRmInfo->rRmDmxMMData.u4BufStart);
            }
            else
            {
                ASSERT(0);
            }
        }
        u2NumAACFrm /= 16;

        prSwdmxRmInfo->rRmDmxMMData.u4StartAddr += (u2NumAACFrm * 2 + 2);
        if (prSwdmxRmInfo->rRmDmxMMData.u4StartAddr >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr -= u4SrcBufLen;

        for (u2CurAACFrmIdx = 0; u2CurAACFrmIdx < u2NumAACFrm; u2CurAACFrmIdx++)
        {
            SWDMX_LOADB_WORD(/*0x30000000 + */u4Ptr, u2CurAACFrmSz);
            u4Ptr += 2;
            if (u4Ptr >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
            {
                if (u4Ptr == prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
                {
                    u4Ptr = prSwdmxRmInfo->rRmDmxMMData.u4BufStart;
                }
                else if (u4Ptr == prSwdmxRmInfo->rRmDmxMMData.u4BufEnd + 1)
                {
                    u4Ptr = prSwdmxRmInfo->rRmDmxMMData.u4BufStart + 1;
                    u2CurAACFrmSz = (u2CurAACFrmSz & 0xFF00) | *(UINT8 *)(prSwdmxRmInfo->rRmDmxMMData.u4BufStart);
                }
                else
                {
                    ASSERT(0);
                }
            }
            prSwdmxRmInfo->rRmDmxMMData.u4FrameSize = u2CurAACFrmSz;

            // check if the parsed frame size is correct
            if (u2CurAACFrmSz >= DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, prSwdmxRmInfo->u1AudPid))
            {
                LOG(0, "2575 length error: %d!!!!!!!!\n", u2CurAACFrmSz);
                *pu4ConsumeSize = u4PacketSize + i;  // incorrect frame size (corrupt data), discard this packet
                prSwdmxInst->fgFifoFull=TRUE;
                return eSWDMX_HDR_PRS_SUCCEED;
            }

            // transfer the AAC header (7 bytes) first
            if (!_SWDMX_TxAACFrame(prSwdmxInst, u2CurAACFrmSz))
            {
                ASSERT(0);  // !!!
            }

            u4BufStartBak = prSwdmxRmInfo->rRmDmxMMData.u4BufStart;
            if (prSwdmxRmInfo->rRmDmxMMData.u4StartAddr < prSwdmxRmInfo->rRmDmxMMData.u4BufStart)
                prSwdmxRmInfo->rRmDmxMMData.u4BufStart -= FEEDER_RESERVED_SIZE;

            prSwdmxRmInfo->rRmDmxMMData.u1Idx = prSwdmxRmInfo->u1AudPid;
            if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &prSwdmxRmInfo->rRmDmxMMData)  != TRUE)
            {
                ASSERT(0);  // !!!
                LOG(0, "Demuxer fails to move data.\n");
                *pu4ConsumeSize = 0;
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }

            prSwdmxRmInfo->rRmDmxMMData.u4BufStart = u4BufStartBak;

            prSwdmxRmInfo->rRmDmxMMData.u4StartAddr += prSwdmxRmInfo->rRmDmxMMData.u4FrameSize;
            if (prSwdmxRmInfo->rRmDmxMMData.u4StartAddr >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
                prSwdmxRmInfo->rRmDmxMMData.u4StartAddr -= u4SrcBufLen;
        }

#ifdef RMLS
        if (prSwdmxRmInfo->fgRmLs)
        {
            ASSERT(RM_STRM_AUD == prSwdmxRmInfo->u4RmStrmIdx);
            prSwdmxRmInfo->u4LsPts[RM_STRM_AUD] = u4Pts * 90;

            if (prSwdmxRmInfo->fgDeliverVEos)
            {
                // Video is finished, no need to change to video
            }
            else if (prSwdmxRmInfo->u4LsPts[RM_STRM_AUD] > (prSwdmxRmInfo->u4LsPts[RM_STRM_VDO] + RM_LS_CHG_STRM_THRD))
            {
                LOG(6, "2891:Set prSwdmxRmInfo->fgRmChgStrm\n");
                prSwdmxRmInfo->fgRmChgStrm = TRUE;
            }
        }
#endif
      }
      else if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
      {
          if (prSwdmxRmInfo->dwCurCookFrameInSB == 0)
          {
#if 1  //add for CQ PL00118234
              if (!(bTmpVal & HX_KEYFRAME_FLAG))
              {
                  //printf("swdmx_rm 2427 skip this frame\n");
                  prSwdmxRmInfo->rRmDmxMMData.u4StartAddr += prSwdmxRmInfo->rRmDmxMMData.u4FrameSize;
                  if (prSwdmxRmInfo->rRmDmxMMData.u4StartAddr >= prSwdmxRmInfo->rRmDmxMMData.u4BufEnd)
                      prSwdmxRmInfo->rRmDmxMMData.u4StartAddr -= u4SrcBufLen;

                  goto RM_PKT_END;
              }
              else
              {
                  //printf("COOK SB PTS = 0x%X\n", u4Pts * 90);
                  prSwdmxRmInfo->u4CookSBPts = u4Pts * 90;
              }
#endif
              //_dwLastCookAPts = _dwPrsPts * 90;
              //printf("swdmx_rm 2436 beginning from this frame @ %08X\n", (UINT32)prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx]);

              if (prSwdmxRmInfo->fgCookPtsPending)
              {
                  prSwdmxRmInfo->fgCookPtsPending = FALSE;
                  prSwdmxRmInfo->u4TriggerAudPts = u4Pts * 90;
                  AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxRmInfo->u4TriggerAudPts);
              }
          }

          vCookFrameDeInterleave(prSwdmxInst, (UINT8 *)prSwdmxRmInfo->rRmDmxMMData.u4StartAddr);
      }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)  // video
    {
        BOOL fgBreak = FALSE;
        UINT16 wVal;
        UINT8 bVal;
        RVFrameHdr *prHeader = &prSwdmxRmInfo->rFrameHdr;

        prSwdmxRmInfo->pbHdrBuf = (UINT8 *)prSwdmxRmInfo->rRmDmxMMData.u4StartAddr;
        prSwdmxRmInfo->dwPktSz = u4PacketSize - (fgVer1 ? 13 : 12);
        prSwdmxRmInfo->dwPrsState = RMPRS_FRMHDR;
        vMpsPrsNextG(prSwdmxInst, 0, 2, 0);
        while (!fgBreak)
        {
            switch (prSwdmxRmInfo->dwPrsState)
            {
            case RMPRS_FRMHDR:
                prSwdmxRmInfo->dwPktSz -= 2;
                prHeader->eType = (RVFrameType)((prSwdmxRmInfo->pbHdrBuf[0] & 0xC0) >> 6);
                switch (prHeader->eType)
                {
                case RVFrameTypeLastPartial:
                  prSwdmxRmInfo->dwPktSz = prSwdmxRmInfo->dwPktSz + 0;  // just for setting breakpoint
                case RVFrameTypePartial:
                  SWDMX_LOADB_WORD(prSwdmxRmInfo->pbHdrBuf, wVal);
                  prHeader->u4NumPackets = (wVal & 0x3F80) >> 7;
                  prHeader->u4PacketNum  = wVal & 0x007F;
                  prHeader->u4FrameSize = 0xFFFFFFFF;
                  prHeader->u4PartialFrameSize = 0xFFFFFFFF;
                  prHeader->u4PartialFrameOffset = 0xFFFFFFFF;
                  prHeader->u4Timestamp = 0xFFFFFFFF;
                  prSwdmxRmInfo->dwPrsState = RMPRS_FRM_ENTIRE_SIZE;
                  vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);

                  continue;

                case RVFrameTypeWhole:
                  prHeader->u4NumPackets = 1;
                  prHeader->u4PacketNum = 1;
                  prHeader->u4FrameSize = prSwdmxRmInfo->dwPktSz;
                  prHeader->u4PartialFrameSize = prSwdmxRmInfo->dwPktSz;
                  prHeader->u4PartialFrameOffset = 0;
                  SWDMX_LOAD_BYTE(prSwdmxRmInfo->pbHdrBuf + 1, prHeader->u1SeqNum);
                  prHeader->u4Timestamp = u4Pts;  // !!! check if relative timestamp
                  //prHeader->u4HeaderSize = pbHdrBuf - prSwdmxRmInfo->pbHdrBuf;  !!! check if header size is useful or not

                  prSwdmxRmInfo->au4SliceSz[0] = prHeader->u4PartialFrameSize;
                  prSwdmxRmInfo->au4SliceOfst[0] = prHeader->u4PartialFrameOffset;

                  prSwdmxRmInfo->dwPrsState = RMPRS_FRAME;
#if 0  // megaa 20080625
                  _tPrs.bPrsFlg = VPR_ON | PR_ON;
#endif
#if 1
                  //_dVWpBak = dPRVFifoWp();  // save V Wp for picture type detection  !!!
                  {
                      DMX_PID_T  rPid;

                      x_memset(&rPid, 0, sizeof(rPid));
                      DMX_GetPid(prSwdmxRmInfo->u1VidPid, DMX_PID_FLAG_DATA_POINTERS, &rPid);

                      prSwdmxRmInfo->u4VFifoWpBak = rPid.u4Wp;
                      prSwdmxRmInfo->u8VFileOffsetBak = prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO];
                  }
                  //_dwFrmPktOfst = _dwPktOfst;
                  if (!fgMpsPrsNextP(prSwdmxInst, prSwdmxRmInfo->wGTxLen, prHeader->u4PartialFrameSize, prHeader->u4Timestamp))
                  {
                      LOG(0, "2696 length error: %d!!!!!!!!\n", prHeader->u4PartialFrameSize);
                      fgBreak = TRUE;
                  }
#else
                  _bStrmType = PR_VST;
                  _dVWpBak = dPRVFifoWp();  // save V Wp for picture type detection
                  _dwFrmPktOfst = _dwPktOfst;
                  fgMpsPrsNextP(prSwdmxInst, prSwdmxRmInfo->wGTxLen, prHeader->u4PartialFrameSize);
#endif
                  continue;

                case RVFrameTypeMultiple:
                  SWDMX_LOAD_BYTE(prSwdmxRmInfo->pbHdrBuf + 1, bVal);
                  prHeader->u4NumPackets = 1;
                  prHeader->u4PacketNum = 1;
                  prHeader->u4FrameSize = bVal & 0x7F;  // keep the 14-or-30 flag temporarily
                  prHeader->u4PartialFrameSize = 0;
                  prHeader->u4PartialFrameOffset = 0;
                  prSwdmxRmInfo->dwPrsState = RMPRS_FRM_ONE_OF_MLTI_SIZE;
                  if (bVal & 0x40)                      // check the 14-or-30 flag
                    vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);       // get the residual 8 of 14 bits
                  else
                    vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 3, 0);       // get the residual 24 of 30 bits

                  continue;
                }

                ASSERT(0);  // should never reach here

    case RMPRS_FRM_ONE_OF_MLTI_SIZE:
      if (prHeader->u4FrameSize & 0x40)  // check the 14-or-30 flag again
      {                                  // 8 of 14 bits
        prSwdmxRmInfo->dwPktSz -= 1;
        SWDMX_LOAD_BYTE(prSwdmxRmInfo->pbHdrBuf, bVal);
        prHeader->u4FrameSize &= 0x3F;   // mark off the 14-or-30 flag
        prHeader->u4FrameSize <<= 8;
        prHeader->u4FrameSize |= bVal;
      }
      else                               // 24 of 30 bits
      {
        prSwdmxRmInfo->dwPktSz -= 3;
        SWDMX_LOAD_BYTE(prSwdmxRmInfo->pbHdrBuf, bVal);
        prHeader->u4FrameSize <<= 8;
        prHeader->u4FrameSize |= bVal;
        SWDMX_LOADB_WORD(prSwdmxRmInfo->pbHdrBuf + 1, wVal);
        prHeader->u4FrameSize <<= 16;
        prHeader->u4FrameSize |= wVal;
      }
      prHeader->u4PartialFrameSize = prHeader->u4FrameSize;
      prHeader->u4Timestamp = 0xFFFFFFFF;
      prSwdmxRmInfo->dwPrsState = RMPRS_FRM_TIMESTAMP;
      vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);
      break;

    case RMPRS_FRM_TIMESTAMP:
      prSwdmxRmInfo->dwPktSz -= 2;
      SWDMX_LOADB_WORD(prSwdmxRmInfo->pbHdrBuf, wVal);
      if (prHeader->u4Timestamp != 0xFFFFFFFF)  // it is the 2nd run of this state to get the additional 16 bits
      {
        prHeader->u4Timestamp <<= 16;
        prHeader->u4Timestamp |= wVal;
        prSwdmxRmInfo->dwPrsState = RMPRS_FRM_SEQ_NUM;
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);
        continue;
      }
      prHeader->u4Timestamp = wVal & 0x3FFF;
      if (wVal & 0x4000)  // 14 bits
      {
        // we have got the whole timestamp, enter the next state
        prSwdmxRmInfo->dwPrsState = RMPRS_FRM_SEQ_NUM;
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);
      }
      else
      {
        // trigger the 2nd run
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);
      }
      continue;

    case RMPRS_FRM_ENTIRE_SIZE:
      SWDMX_LOADB_WORD(prSwdmxRmInfo->pbHdrBuf, wVal);
      prSwdmxRmInfo->dwPktSz -= 2;
      if (prHeader->u4FrameSize != 0xFFFFFFFF)  // it is the 2nd run of this state to get the additional 16 bits
      {
        prHeader->u4FrameSize <<= 16;
        prHeader->u4FrameSize |= wVal;
        prSwdmxRmInfo->dwPrsState = RMPRS_FRM_PARTIAL_OFST;
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);
        continue;
      }
      prHeader->fgBrokenUpByUs = ((wVal & 0x8000) != 0 ? 1 : 0);
      prHeader->u4FrameSize = wVal & 0x3FFF;
      if (wVal & 0x4000)  // 14 bits
      {
        // we have got the whole size, enter the next state
        prSwdmxRmInfo->dwPrsState = RMPRS_FRM_PARTIAL_OFST;
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);
      }
      else  // 30 bits
      {
        // trigger the 2nd run
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);
      }
      continue;

    case RMPRS_FRM_PARTIAL_OFST:
      SWDMX_LOADB_WORD(prSwdmxRmInfo->pbHdrBuf, wVal);
      prSwdmxRmInfo->dwPktSz -= 2;
      if (prHeader->eType == RVFrameTypePartial)
      {
        if (prHeader->u4PartialFrameOffset != 0xFFFFFFFF)  // it is the 2nd run of this state to get the additional 16 bits
        {
          prHeader->u4PartialFrameOffset <<= 16;
          prHeader->u4PartialFrameOffset |= wVal;
          prHeader->u4PartialFrameSize = prSwdmxRmInfo->dwPktSz - 1;
          prSwdmxRmInfo->dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);
          continue;
        }
        prHeader->u4PartialFrameOffset = wVal & 0x3FFF;
        if (wVal & 0x4000)  // 14 bits
        {
          // we have got the whole offset, enter the next state
          prHeader->u4PartialFrameSize = prSwdmxRmInfo->dwPktSz - 1;
          prSwdmxRmInfo->dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);
        }
        else  // 30 bits
        {
          // trigger the 2nd run
          vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);
        }
        continue;
      }
      else if (prHeader->eType == RVFrameTypeLastPartial)
      {
        if (prHeader->u4PartialFrameSize != 0xFFFFFFFF)  // it is the 2nd run of this state to get the additional 16 bits
        {
          prHeader->u4PartialFrameSize <<= 16;
          prHeader->u4PartialFrameSize |= wVal;
          prHeader->u4PartialFrameOffset = prHeader->u4FrameSize - prHeader->u4PartialFrameSize;
          prSwdmxRmInfo->dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);
          continue;
        }
        prHeader->u4PartialFrameSize = wVal & 0x3FFF;
        if (wVal & 0x4000)  // 14 bits
        {
          // we have got the whole size, enter the next state
          prHeader->u4PartialFrameOffset = prHeader->u4FrameSize - prHeader->u4PartialFrameSize;
          prSwdmxRmInfo->dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);
        }
        else  // 30 bits
        {
          // trigger the 2nd run
          vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 2, 0);
        }
        continue;
      }

      ASSERT(0);  // should never reach here

    case RMPRS_FRM_SEQ_NUM:
      SWDMX_LOAD_BYTE(prSwdmxRmInfo->pbHdrBuf, prHeader->u1SeqNum);
      prSwdmxRmInfo->dwPktSz--;
      if (prHeader->u4Timestamp == 0xFFFFFFFF)
        prHeader->u4Timestamp = u4Pts;  // !!! check if relative timestamp
      //prHeader->u4HeaderSize = pbHdrBuf - prSwdmxRmInfo->pbHdrBuf;  !!! check if header size is useful or not

      prSwdmxRmInfo->au4SliceSz[prHeader->u4PacketNum - 1] = prHeader->u4PartialFrameSize;
      prSwdmxRmInfo->au4SliceOfst[prHeader->u4PacketNum - 1] = prHeader->u4PartialFrameOffset;

      prSwdmxRmInfo->dwPrsState = RMPRS_FRAME;
#if 0  // megaa 20080625
      _tPrs.bPrsFlg = VPR_ON | PR_ON;
#endif
#if 1
      if (prHeader->u4PacketNum == 1)
      {
          DMX_PID_T  rPid;

          x_memset(&rPid, 0, sizeof(rPid));
          DMX_GetPid(prSwdmxRmInfo->u1VidPid, DMX_PID_FLAG_DATA_POINTERS, &rPid);

          prSwdmxRmInfo->u4VFifoWpBak = rPid.u4Wp;
          prSwdmxRmInfo->u8VFileOffsetBak = prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO];
          //_dVWpBak = dPRVFifoWp();  // save V Wp for picture type detection  !!!
          //_dwFrmPktOfst = _dwPktOfst;
      }
      if (!fgMpsPrsNextP(prSwdmxInst, prSwdmxRmInfo->wGTxLen, prHeader->u4PartialFrameSize, prHeader->u4Timestamp))
      {
          LOG(0, "2886 length error: %d!!!!!!!!\n", prHeader->u4PartialFrameSize);
          fgBreak = TRUE;
      }
#else
      _bStrmType = PR_VST;
      if (prHeader->u4PacketNum == 1)
      {
        _dVWpBak = dPRVFifoWp();  // save V Wp for picture type detection
        _dwFrmPktOfst = _dwPktOfst;
      }
      fgMpsPrsNextP(prSwdmxInst, prSwdmxRmInfo->wGTxLen, prHeader->u4PartialFrameSize);
#endif
      break;

    case RMPRS_FRM_NEXT_MULTIPLE:
      SWDMX_LOAD_BYTE(prSwdmxRmInfo->pbHdrBuf, bVal);
      if (bVal != 0xC0)
      {
          LOG(0, "2904 bVal != 0xC0!!!!!!\n");
          fgBreak = TRUE;  // discard this packet directly
          break;
      }
      SWDMX_LOAD_BYTE(prSwdmxRmInfo->pbHdrBuf + 1, bVal);
      prSwdmxRmInfo->dwPktSz -= 2;
      prHeader->u4NumPackets = 1;
      prHeader->u4PacketNum = 1;
      prHeader->u4FrameSize = bVal & 0x7F;  // keep the 14-or-30 flag temporarily
      prHeader->u4PartialFrameSize = 0;
      prHeader->u4PartialFrameOffset = 0;
      prSwdmxRmInfo->dwPrsState = RMPRS_FRM_ONE_OF_MLTI_SIZE;
      if (bVal & 0x40)                      // check the 14-or-30 flag
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 1, 0);       // get the residual 8 of 14 bits
      else
        vMpsPrsNextG(prSwdmxInst, prSwdmxRmInfo->wGTxLen, 3, 0);       // get the residual 24 of 30 bits
      break;

    case RMPRS_PACKET:
        if (fgOneVideoFrm)
        {
            if (prSwdmxRmInfo->u4NumSentVid > u4OldNumSentVid)  // wait until one complete video frame has been sent
            {
                fgBreak = TRUE;
            }
            else
            {
                //x_thread_delay(2);  //remove delay, when merging many packets into a frame, it'll spend too much time.

                SWDMX_LOADB_WORD(prSwdmxRmInfo->pbHdrBuf + 2, wVal);
                prSwdmxRmInfo->dwPktSz = wVal - (fgVer1 ? 13 : 12);
                prSwdmxRmInfo->pbHdrBuf += (fgVer1 ? 13 : 12);
                prSwdmxRmInfo->dwPrsState = RMPRS_FRMHDR;
                vMpsPrsNextG(prSwdmxInst, 0, 2, 0);
            }
        }
        else
        {
            fgBreak = TRUE;
        }
        break;

    default:
        ASSERT(0);
            }
        }

#ifdef RMLS
    if (prSwdmxRmInfo->fgRmLs)
    {
        ASSERT(RM_STRM_VDO == prSwdmxRmInfo->u4RmStrmIdx);
        prSwdmxRmInfo->u4LsPts[RM_STRM_VDO] = u4Pts * 90;

        if (prSwdmxRmInfo->fgDeliverAEos)
        {
            // Audio is finished, no need to change to audio
        }
        else if (!prSwdmxRmInfo->fgEnAudio)
        {
            // Audio is off, no need to change to audio
        }
        else if (prSwdmxRmInfo->u4LsPts[RM_STRM_VDO] > (prSwdmxRmInfo->u4LsPts[RM_STRM_AUD] + RM_LS_CHG_STRM_THRD))
        {
            LOG(6, "3264:Set prSwdmxRmInfo->fgRmChgStrm\n");
            prSwdmxRmInfo->fgRmChgStrm = TRUE;
        }
    }
#endif

    }
    else
    {
        ASSERT(0);
    }

RM_PKT_END:
    *pu4ConsumeSize = u4PacketSize + i;  // + RM_PARSER_OFFSET;  !!!
    if (fgRingBuf)
    {
        *pu4ConsumeSize += ((UINT32)(VIRTUAL(prFeederInfo->u4EndAddr)) -
                                                           (UINT32)(pucOriginalPtr) -
                                                           u4RingSize);
    }

    if (prSwdmxRmInfo->fgIsBlock == TRUE)
    {
        prSwdmxRmInfo->fgIsBlock = FALSE;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsWithoutIdx(
    SWDMX_INFO_T *prSwdmxInst,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    FEEDER_BUF_INFO_T *prFeederInfo = &prSwdmxRmInfo->rFeederInfo;
    BOOL fgRangeEnd = FALSE;

    UINT32 u4ReadSize = 0;
	
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState = eSWDMX_HDR_PRS_SUCCEED;

    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    if (prSwdmxRmInfo->fgFeederInvalid)
    {
        INT32 i;
        LOG(3, "Feeder is invalid, Cur Feeder ID = %ld, req times = %ld, Cur Dmx Pos = %lld\n",
            prSwdmxRmInfo->u4FeederReqID,
            prSwdmxRmInfo->i4ReqReadTimes,
            prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx]);

        for (i = 0; i < prSwdmxRmInfo->i4ReqReadTimes; i++)
        {
            _SWDMX_RmGetDataFromFeeder(prSwdmxInst, &rFeederReqData);
        }

        _SWDMX_RmResetFeeder(prSwdmxInst);

        prSwdmxRmInfo->fgIsSeek = TRUE;
        prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
        prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx] = prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx];
        prSwdmxRmInfo->fgFeederInvalid = FALSE;
        prSwdmxRmInfo->i4ReqReadTimes = 0;
        prSwdmxRmInfo->fgReqDataMiss = FALSE;
        prSwdmxRmInfo->u4FeederReqID += 1000;
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Check if demuxing is done
    if (prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] >= prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] + prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx]
#ifdef __MODEL_slt__
        || prSwdmxRmInfo->u4NumSentVid >= 30
#endif
        )
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (prSwdmxRmInfo->fgEnVideo
            && !prSwdmxRmInfo->fgDeliverVEos
#ifdef RMLS
            && (!prSwdmxRmInfo->fgRmLs ||prSwdmxRmInfo->u4RmStrmIdx == RM_STRM_VDO)
#endif
            )
        {
            if (prSwdmxRmInfo->fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp(prSwdmxInst);
                prSwdmxRmInfo->fgVDecIsPause = FALSE;
            }

            LOG(5, "Cur DmxMoviPos = %lld, range size = %lld, range offset = %lld\n",
                prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx],
                prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx],
                prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx]);

            LOG(5, "_SWDMX_RmHdrPrsWithoutIdx(%d) Deliver Video Eos\n", prSwdmxInst->u1SwdmxId);

#ifdef RMLS
            if (prSwdmxRmInfo->fgRmLs && prSwdmxRmInfo->fgEnAudio && !prSwdmxRmInfo->fgDeliverAEos)
            {
                // wait audio finished
                LOG(3, "Deliver Video Eos: Set prSwdmxRmInfo->fgRmChgStrm\n");
                prSwdmxRmInfo->fgRmChgStrm = TRUE;
            }
#endif

            if (!_SWDMX_SendRmEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
            {
                LOG(3, "Fail to send Video Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxRmInfo->fgDeliverVEos = TRUE;
        }

        if (prSwdmxRmInfo->fgEnAudio
            && !prSwdmxRmInfo->fgDeliverAEos
#ifdef RMLS
            && (!prSwdmxRmInfo->fgRmLs ||prSwdmxRmInfo->u4RmStrmIdx == RM_STRM_AUD)
#endif
            )
        {
            LOG(5, "_SWDMX_RmHdrPrsWithoutIdx(%d) Deliver Audio Eos\n", prSwdmxInst->u1SwdmxId);

#ifdef RMLS
            if (prSwdmxRmInfo->fgRmLs && !prSwdmxRmInfo->fgDeliverVEos)
            {
                // wait video finished
                LOG(3, "Deliver Audio Eos: Set prSwdmxRmInfo->fgRmChgStrm\n");
                prSwdmxRmInfo->fgRmChgStrm = TRUE;
            }
#endif

            if (!_SWDMX_SendRmEos(prSwdmxInst, eSWDMX_MEDIA_AUDIO))
            {
                LOG(3, "Fail to send Audio Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxRmInfo->fgDeliverAEos = TRUE;
            AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
        }

        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Request data for the first time
    if (prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx] == 0)
    {
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxRmInfo->rFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        u4ReadSize = prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx];
        u4ReadSize = RM_MIN(u4ReadSize, RM_MIN_READ_SIZE);
        prSwdmxRmInfo->u4LastAlignOffset = 0;

        prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx] = prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx];
        prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] = prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx];

        if ((prSwdmxRmInfo->i4ReqReadTimes == 0) &&
            (FeederSetRequest(prSwdmxInst->eFeederSrc,
                u4ReadSize, /*RM_MIN_READ_SIZE, */
                prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx],
                &rFeederCond,
                prSwdmxRmInfo->u4FeederReqID++) != FEEDER_E_BUFFER_FULL))
        {

            prSwdmxRmInfo->i4ReqReadTimes ++;

            if (!_SWDMX_RmGetDataFromFeeder(prSwdmxInst, &rFeederReqData))
            {
                 //prSwdmxRmInfo->fgFeederInvalid = TRUE;
                return eSWDMX_HDR_PRS_FAIL;
            }
            prSwdmxRmInfo->i4ReqReadTimes --;

            prSwdmxRmInfo->u4LastAlignOffset = rFeederReqData.u4AlignOffset;
            prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx] += rFeederReqData.u4ReadSize - prSwdmxRmInfo->u4LastAlignOffset;
            prSwdmxRmInfo->u4CurDmxFeederRPtr = VIRTUAL(prFeederInfo->u4StartAddr + prSwdmxRmInfo->u4LastAlignOffset);
            prSwdmxRmInfo->u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
            LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
        }
    }

    if ((prSwdmxRmInfo->u4LeftBytesInFeeder <=  RM_BUFFER_SIZE) ||
        (prSwdmxRmInfo->fgIsBlock == TRUE))
    {
        if (prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] + prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx] > prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx])
        {
            u4ReadSize = prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] + prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx] -
                                   prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx];
        }
        u4ReadSize = RM_MIN(u4ReadSize, RM_MIN_READ_SIZE);

        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }

#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        if ((!prSwdmxRmInfo->fgFeederIsReady) ||
            (prSwdmxRmInfo->u4LeftBytesInFeeder + u4ReadSize < RM_BUFFER_SIZE))
#else
        if (prSwdmxRmInfo->u4LeftBytesInFeeder + u4ReadSize < RM_BUFFER_SIZE)
#endif
        {
            if (u4ReadSize > 0)
            {
                LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n",
                        prSwdmxRmInfo->u4LeftBytesInFeeder,
                        prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx],
                        prSwdmxRmInfo->u4FeederReqID);

                //request data one by one
                if ((prSwdmxRmInfo->i4ReqReadTimes == 0)  &&
                    (FeederSetRequest(prSwdmxInst->eFeederSrc,
                        u4ReadSize,
                        prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx],
                        &rFeederCond,
                        prSwdmxRmInfo->u4FeederReqID++) != FEEDER_E_BUFFER_FULL))
                {
                    prSwdmxRmInfo->u4LastAlignOffset = 0;
                    prSwdmxRmInfo->i4ReqReadTimes++;
					prSwdmxRmInfo->u4RequestSz = u4ReadSize;
                }
                #if 0
                else if (prSwdmxRmInfo->i4ReqReadTimes == 0)
                {
                    LOG(3, "Feeder Buffer Full\n");
                    prSwdmxRmInfo->fgFeederInvalid = TRUE;
                    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxRmInfo->rFeederInfo) !=
                        FEEDER_E_OK)
                    {
                        LOG(1, "Feeder input buffer error.\n");
                        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                    }
                    LOG(3, "Cur Fed's write ptr = 0x%08x\n", prFeederInfo->u4WriteAddr);
                    //return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                }
                #endif
            }
        }
    }

#if defined(CC_DLNA_SUPPORT) && 0
    if (u4ReadSize != RM_MIN_READ_SIZE)
    {
        // DLNA
        FeederSetInfo(FEEDER_SWDMX_EOS, TRUE);
    }

    // DLNA
    if (!prSwdmxRmInfo->fgFeederIsReady && FeederIsReady(prSwdmxInst->eFeederSrc))
    /*if ((!prSwdmxRmInfo->fgFeederIsReady) &&
        ((prSwdmxRmInfo->u4LeftBytesInFeeder >= RM_PREBUFFER_SIZE) || (prSwdmxRmInfo->u4LeftBytesInFeeder >= prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx])))*/
    {
        prSwdmxRmInfo->fgFeederIsReady = TRUE;
    }
#endif

#ifdef SWDMX_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt;
    HAL_GetTime(&rTimeRstS);
#endif

#if defined(CC_DLNA_SUPPORT) && 0
    if ((prSwdmxRmInfo->u4LeftBytesInFeeder > 0) && prSwdmxRmInfo->fgFeederIsReady)
#else
    if (prSwdmxRmInfo->u4LeftBytesInFeeder && _SWDMX_RmVdecQueueThreshold(prSwdmxInst, RM_VFIFO_MAX_PIC_NS))
#endif
    {
        if (prSwdmxRmInfo->fgSpeedChange && i4PlaySpeed == 1000)
        {
            if (prSwdmxRmInfo->fgEnAudio && prSwdmxRmInfo->fgEnVideo)
            {
                if(((UINT32)(prSwdmxRmInfo->u8VSeekPts>>32) != 0xFFFFFFFF)
                    || ((UINT32)(prSwdmxRmInfo->u8VSeekPts) != 0xFFFFFFFF))
                {
                    if (prRangeList->prDmxRangeInfo->ui8_vid_duration >
                        (UINT32)prSwdmxRmInfo->u8VSeekPts)
                    {
                        LOG(5, "Pause Video for A/V Sync\n");
                        VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                        VDEC_Pause(prSwdmxInst->u1VdecId);
                        prSwdmxRmInfo->fgVDecIsPause = TRUE;
                    }
                }
                else
                {
                    LOG(5, "Pause Video for A/V Sync\n");
                    VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
                    VDEC_Pause(prSwdmxInst->u1VdecId);
                    prSwdmxRmInfo->fgVDecIsPause = TRUE;
                }
            }
            prSwdmxRmInfo->fgSpeedChange = FALSE;
        }

        // Start to parse data
        u4PrsState = _SWDMX_PrsRmPacket(prSwdmxInst, (UCHAR*)(prSwdmxRmInfo->u4CurDmxFeederRPtr),
            prRangeList,
            prSwdmxRmInfo->u4LeftBytesInFeeder,
            &u4ConsumedSize,
            i4PlaySpeed,
            fgRangeEnd,
            FALSE);
    }

#ifdef SWDMX_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    LOG(0, "Swdmx parsing and deliver time = (%ld) \n", rTimeRstDt.u4Micros);
#endif

    switch (u4PrsState)
    {
    case eSWDMX_HDR_PRS_FAIL:
        // Prevent unknow chunk in the first one mega
        // If the file size is smaller than one mega, RM demuxer
        // will check if demuxing is done in the beginning of _SWDMX_RmHdrPrsWithoutIdx
        if ((prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] >= prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] + prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx]) ||
            (prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] > prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx] + ONE_MB))
        {
            LOG(3, "No RM chunks was discovered\n");
            prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] = prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] + prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx];
            prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }
        break;
    default:
        break;
    }

    if (prSwdmxRmInfo->u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] += u4ConsumedSize;
        prSwdmxRmInfo->u4LeftBytesInFeeder -= u4ConsumedSize;
    }
    else
    {
        prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
    }

    if (u4ConsumedSize != 0)
    {
        LOG(6, "u4ConsumedSize = %ld u4LeftBytesInFeeder = 0x%08x.\n",
            u4ConsumedSize, prSwdmxRmInfo->u4LeftBytesInFeeder);
    }

    if ((prSwdmxRmInfo->u4CurDmxFeederRPtr + u4ConsumedSize) >=
        VIRTUAL(prFeederInfo->u4EndAddr))
    {
        prSwdmxRmInfo->u4CurDmxFeederRPtr =  VIRTUAL(prFeederInfo->u4StartAddr) +
            ((prSwdmxRmInfo->u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(prFeederInfo->u4EndAddr));
    }
    else
    {
        prSwdmxRmInfo->u4CurDmxFeederRPtr += u4ConsumedSize;
    }

    // Update read ptr to feeder
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxRmInfo->u4CurDmxFeederRPtr));

    LOG(9, "prSwdmxRmInfo->u4CurDmxMoviPos = %ld.\n", prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx]);

    if (prSwdmxRmInfo->i4ReqReadTimes > 0)
    {
        if (!_SWDMX_RmGetDataFromFeeder(prSwdmxInst, &rFeederReqData))
        {
            //prSwdmxRmInfo->fgFeederInvalid = TRUE;
            vHandleRmRingBuf(prSwdmxInst);
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
		
		if (rFeederReqData.u4ReadSize < prSwdmxRmInfo->u4RequestSz)
		{
			//UINT32 u4ReqButNoackSize = 0;
			UINT32 u4WriteAddr, u4TempAddr;
			
			//u4ReqButNoackSize = prSwdmxRmInfo->u4RequestSz - rFeederReqData.u4ReadSize;
			
			LOG(3, "FeederSetWritePointer u4WriteAddr = 0x%08x.u4RequestSz = 0x%08x  u4ReadSize = 0x%08x \n",
			rFeederReqData.u4WriteAddr,
			prSwdmxRmInfo->u4RequestSz,
			rFeederReqData.u4ReadSize);
		
			u4WriteAddr = rFeederReqData.u4WriteAddr;
			u4TempAddr	= u4WriteAddr + rFeederReqData.u4ReadSize;
			u4WriteAddr = (u4TempAddr >= prFeederInfo->u4EndAddr) ?
						  ((u4TempAddr + prFeederInfo->u4StartAddr) - prFeederInfo->u4EndAddr) :
						  (u4TempAddr);
			
			FeederSetWritePointer(rFeederReqData.eDataType, u4WriteAddr);
			
		}

        LOG(6, "rFeederReqData.u4WriteAddr = 0x%08x. ReadSize = 0x%08x id = %ld\n",
            rFeederReqData.u4WriteAddr,
            rFeederReqData.u4ReadSize,
            rFeederReqData.u4Id);

        prSwdmxRmInfo->u4LastAlignOffset = rFeederReqData.u4AlignOffset;

        if ((rFeederReqData.u4ReadSize > prSwdmxRmInfo->u4LastAlignOffset) &&
            (rFeederReqData.u4ReadSize != 0))
        {
            prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx] += rFeederReqData.u4ReadSize - prSwdmxRmInfo->u4LastAlignOffset;
        }
        else
        {
            LOG(3, "Feeder read size is smaller than align offset!!!\n");
            prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] = prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] + prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx];
            prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }


        if(prSwdmxRmInfo->fgIsSeek)
        {
            prSwdmxRmInfo->u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + prSwdmxRmInfo->u4LastAlignOffset);
            prSwdmxRmInfo->fgIsSeek = FALSE;
            rFeederReqData.u4ReadSize -= prSwdmxRmInfo->u4LastAlignOffset;

        }

        prSwdmxRmInfo->u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        prSwdmxRmInfo->i4ReqReadTimes--;
        
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }

    vHandleRmRingBuf(prSwdmxInst);

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsUseIdx(
    SWDMX_INFO_T *prSwdmxInst,
    INT32 i4Speed,
    SWDMX_RANGE_LIST_T* prRangeList)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    BOOL fgRangeDone = TRUE;
    UINT32 *pu4VidCurIdx  = NULL;
    MM_RANGE_ELMT_IDXTBL_T *pKeyIdxTbl = prSwdmxRmInfo->prVidKeyIdxTbl;

    if (!prRangeList)
    {
        LOG(3, "Range list is NULL\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if ((i4Speed > 2000) || (i4Speed < 0))
    {
        fgRangeDone = FALSE;
        if (pKeyIdxTbl != NULL &&
            (prSwdmxRmInfo->u4CurSentKey < pKeyIdxTbl->ui4_number_of_entry))
        {

            LOG(6, "pKeyIdxTbl->ui4_number_of_entry = %x\n", pKeyIdxTbl->ui4_number_of_entry);
            LOG(6, "prSwdmxRmInfo->u4CurSentKey = %x\n", prSwdmxRmInfo->u4CurSentKey);
            pu4VidCurIdx = &prSwdmxRmInfo->u4CurSentKey;

            if (prSwdmxRmInfo->u4NumSentVid == 0)  // !!!
            {
                ASSERT(0);
#if 0
                _SWDMX_BeforeDeliverUseIdx(eSWDMX_MEDIA_VIDEO);
#endif
            }

            _SWDMX_DeliverRmKeyframe(
                prSwdmxInst,
                i4Speed,
                prSwdmxRmInfo->prVidKeyIdxTbl,
                pu4VidCurIdx,
                eSWDMX_MEDIA_VIDEO,
                prRangeList,
                i4Speed);
        }
#if 0  // ToDo: water-proof this (why AVI does not have this?)
        /* megaa 20090723: this block of code will cause many problems such as
                           1. FF4X+ to near end and return to 1X, video freezes
                           2. Play to near end, can't FF4X+
                           so I disable it.  But I forget originally why it was added...
        */
        else if (i4Speed > 2000)
        {
            fgRangeDone = TRUE;
        }
#endif
    }
    else
    {
        LOG(0, "swdmx_rm #%d error!!!!!!\n", __LINE__);
        ASSERT(0);
        return eSWDMX_HDR_PRS_FAIL;
    }
     // The last key frame
    if (prSwdmxRmInfo->u4VidRepeatFrame == 0)
    {
        if ((i4Speed > 2000) &&(pKeyIdxTbl != NULL && prSwdmxRmInfo->u4CurSentKey == pKeyIdxTbl->ui4_number_of_entry) &&
        !prSwdmxRmInfo->fgDeliverVEos)
        {
            LOG(5, "Fast Forward _SWDMX_RmHdrPrsUseIdx Deliver Video Eos\n");
            if (!_SWDMX_SendRmEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
            {
                LOG(3, "Fail to send Video Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxRmInfo->fgDeliverVEos = TRUE;
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
    }

    if (fgRangeDone)
    {
        INT32 i;
        LOG(5, "Range is finished\n");
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
        prSwdmxRmInfo->prVidIdxTbl = NULL;

        for (i = 0; (i < prSwdmxRmInfo->u1TotalATrack) && (i < RM_MAX_AUDIO_TRACK); i++)
        {
            prSwdmxRmInfo->prAudIdxTbl[i] = NULL;
        }
        prSwdmxRmInfo->u1TotalATrack = 0;

        prSwdmxRmInfo->fgBlockVid = FALSE;
        prSwdmxRmInfo->fgBlockAud = FALSE;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverRmKeyframe(
    SWDMX_INFO_T *prSwdmxInst,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl,
    UINT32* pu4CurIdx,
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    FEEDER_BUF_INFO_T *prFeederInfo = &prSwdmxRmInfo->rFeederInfo;
    UINT32 u4DmxAvailSize = 0;
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState;

    //DMX_MM_DATA_T prSwdmxRmInfo->rRmDmxMMData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    UINT32 u4WriteAddr = 0;
    UINT32 u4ReadSize = 0;

    if(prSwdmxRmInfo->fgFeederInvalid)
    {
        INT32 i;
        FEEDER_REQ_DATA_T rTmpFeederReqData;

        for (i = 0; i < prSwdmxRmInfo->i4ReqReadTimes ; i++)
        {
            _SWDMX_RmGetDataFromFeeder(prSwdmxInst, &rTmpFeederReqData);
        }

        prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
        prSwdmxRmInfo->fgRepeatRequest = TRUE;
        prSwdmxRmInfo->fgFeederInvalid = FALSE;
        prSwdmxRmInfo->u4FeederReqID = 0;
        prSwdmxRmInfo->i4ReqReadTimes = 0;
        prSwdmxRmInfo->fgReqDataMiss = FALSE;

        _SWDMX_RmResetFeeder(prSwdmxInst);

        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    x_memset(&prSwdmxRmInfo->rRmDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    x_memset(&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));

    if (eSWDMX_MEDIA_VIDEO == u1HdrType)
    {
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prSwdmxRmInfo->u1VidPid);
    }
    else if (eSWDMX_MEDIA_VIDEO == u1HdrType)
    {
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, prSwdmxRmInfo->u1AudPid);
    }
    else
    {
        u4DmxAvailSize = 0;
        LOG(0, "Unexpected u1HdrType: %d @ %s,%d\n", u1HdrType, __FUNCTION__, __LINE__);
        ASSERT(0);
    }

    LOG(6, "prSwdmxRmInfo->u4CurSentKey = %ld Key Pts = 0x%08x Dmx available size = 0x%08x\n",
        prSwdmxRmInfo->u4CurSentKey,
        prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts * 90,
        u4DmxAvailSize);

    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > u4DmxAvailSize)
    {
        LOG(7, "Demuxer Buffer is not enough Hdr type = %x u4DmxAvailSize = %ld  frame size = %ld.\n",
            u1HdrType,
            u4DmxAvailSize,
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

        x_thread_delay(1);
		prSwdmxInst->fgFifoFull = TRUE;
        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxRmInfo->rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Send sequence header for decoder's initialization?

    if (((*pu4CurIdx) < prIdxTbl->ui4_number_of_entry)&& !prSwdmxRmInfo->fgDeliverVEos)
    {
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            //UCHAR *pBuf;
            UINT16 u2MaxQNum, u2CurQNum;
            VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);

            // Temporally use 30 for fps
            // Should use fps in the fucture.
            if (u2CurQNum > prSwdmxRmInfo->u4VidFps)
            {
                x_thread_delay(1);
                prSwdmxInst->fgFifoFull=TRUE;
                return eSWDMX_HDR_PRS_SUCCEED;
            }

            if (prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] + prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx] >
                (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset))
            {
                u4ReadSize = prSwdmxRmInfo->au8RangeSz[prSwdmxRmInfo->u4RmStrmIdx] +
                               prSwdmxRmInfo->au8FileOffset[prSwdmxRmInfo->u4RmStrmIdx] -
                               (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset);
            }
            else
            {
                ASSERT(0);
            }
            u4ReadSize = RM_MIN(u4ReadSize, RM_MIN_READ_SIZE);

            LOG(7, "prSwdmxRmInfo->u4NumSentVid %ld  ui4_size = 0x%08x\n", prSwdmxRmInfo->u4NumSentVid, prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
            LOG(7, "ui4_relative_offset = %08x.\n", prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset);

            prSwdmxRmInfo->au8CurDmxFilePos[prSwdmxRmInfo->u4RmStrmIdx] = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset;

            if ((prSwdmxRmInfo->i4ReqReadTimes == 0) &&
                (FeederSetRequest(prSwdmxInst->eFeederSrc,
                u4ReadSize,
                (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset),
                &rFeederCond,
                prSwdmxRmInfo->u4FeederReqID++) != FEEDER_E_BUFFER_FULL))
            {
                prSwdmxRmInfo->i4ReqReadTimes++;
            }

            //LOG(3, "Feeder's buffer is full\n");
            if (prSwdmxRmInfo->i4ReqReadTimes > 0)
            {
                if (!_SWDMX_RmGetDataFromFeeder(prSwdmxInst, &rFeederReqData))
                {
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }

                prSwdmxRmInfo->i4ReqReadTimes--;
                prSwdmxRmInfo->u4LastAlignOffset = rFeederReqData.u4AlignOffset;
                prSwdmxRmInfo->au8CurFilePos[prSwdmxRmInfo->u4RmStrmIdx] += rFeederReqData.u4ReadSize - prSwdmxRmInfo->u4LastAlignOffset;
                prSwdmxRmInfo->u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);  //VIRTUAL(prFeederInfo->u4StartAddr + prSwdmxRmInfo->u4LastAlignOffset);
                prSwdmxRmInfo->u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
                LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
                }
                if (rFeederReqData.u4ReadSize <= rFeederReqData.u4AlignOffset ||
#if 0  // !!! ToDo: Ask Mutil to put ui4_size
                    prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > (rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset))
#else
                    u4ReadSize > (rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset))
#endif
                {
                LOG(3, "_SWDMX_DeliverRmKeyframe rFeederReqData.u4ReadSize <= rFeederReqData.u4AlignOffset\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            else
            {
                LOG(1, "i4ReqReadTimes = 0, rFeederReqData = 0\n");
            }

            vHandleRmRingBuf(prSwdmxInst);

            u4WriteAddr = VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);
            prSwdmxRmInfo->u4RecFeederWPtr = u4WriteAddr;
            prSwdmxRmInfo->fgRequestData = FALSE;

            u4PrsState = _SWDMX_PrsRmPacket(
                prSwdmxInst,
                (UCHAR*)(prSwdmxRmInfo->u4CurDmxFeederRPtr),
                prRangeList,
                prSwdmxRmInfo->u4LeftBytesInFeeder,
                &u4ConsumedSize,
                i4PlaySpeed,
                FALSE,
                TRUE);

            if (u4PrsState != eSWDMX_HDR_PRS_SUCCEED)
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;

            if(((UINT32)(prSwdmxRmInfo->u8SeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(prSwdmxRmInfo->u8SeekPts) != 0xFFFFFFFF))
            {
                UINT32 u4Pts, u4CurSTC;

                u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts * 90;
                if(u4Pts > (UINT32)prSwdmxRmInfo->u8SeekPts)
                {
                    STC_SetStcValue(prSwdmxInst->u1StcId, u4Pts - 18000);
                    u4CurSTC = STC_GetStcValue(prSwdmxInst->u1StcId);
                    LOG(1, "Got Vid T PTS 0x%x, STC 0x%x, Seek 0x%x\n",
                        u4Pts, u4CurSTC, (UINT32)prSwdmxRmInfo->u8SeekPts);
                    prSwdmxRmInfo->u8SeekPts = 0xFFFFFFFF;
                    prSwdmxRmInfo->u8SeekPts <<= 32;
                    prSwdmxRmInfo->u8SeekPts += 0xFFFFFFFF;
                }
            }
        }
        else
        {
            ASSERT(0);
        }
#if 1
        //update feeder RPTR
        prSwdmxRmInfo->u4CurDmxFeederRPtr += u4ConsumedSize;
        if(prSwdmxRmInfo->u4CurDmxFeederRPtr > VIRTUAL(prFeederInfo->u4EndAddr))
        {
            prSwdmxRmInfo->u4CurDmxFeederRPtr -= (prFeederInfo->u4EndAddr - prFeederInfo->u4StartAddr);
        }
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxRmInfo->u4CurDmxFeederRPtr));
#else
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(u4WriteAddr));
#endif
        vHandleRmRingBuf(prSwdmxInst);

        if (!_SWDMX_RmCalculateRepeatFrame(prSwdmxInst, i4Speed))
        {
            if (i4Speed > 2000)
            {
                (*pu4CurIdx)++;
            }
            else if ((i4Speed < 0) &&
                (*pu4CurIdx > 0))
            {
                (*pu4CurIdx)--;
            }
        }

        // The last key frame
        if (prSwdmxRmInfo->u4VidRepeatFrame == 0)
        {
            if ((i4Speed > 2000) &&
            (prSwdmxRmInfo->u4CurSentKey == prIdxTbl->ui4_number_of_entry) &&
            (prSwdmxRmInfo->u4CurRepeatFrameIdx == prIdxTbl->ui4_number_of_entry - 1) &&
            !prSwdmxRmInfo->fgDeliverVEos)
            {
                LOG(5, "Fast Forward _SWDMX_RmHdrPrsUseIdx Deliver Video Eos\n");
                if (!_SWDMX_SendRmEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                prSwdmxRmInfo->fgDeliverVEos = TRUE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            else if ((i4Speed < 0) &&
                        (prSwdmxRmInfo->u4CurSentKey == 0) &&
                        (prSwdmxRmInfo->u4CurRepeatFrameIdx == 0) &&
                        !prSwdmxRmInfo->fgDeliverVEos)
            {
                if (!_SWDMX_SendRmEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                {
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                prSwdmxRmInfo->fgDeliverVEos = TRUE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
        }
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}

static BOOL _SWDMX_RmResetFeeder(SWDMX_INFO_T *prSwdmxInst)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    FEEDER_BUF_INFO_T *prFeederInfo = &prSwdmxRmInfo->rFeederInfo;
    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxRmInfo->rFeederInfo) !=
        FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_RmResetFeeder Cur Fed's WPtr = 0x%08x\n", prFeederInfo->u4WriteAddr);
    prSwdmxRmInfo->u4CurDmxFeederRPtr = prFeederInfo->u4WriteAddr ;

    // Update read ptr to feeder
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxRmInfo->u4CurDmxFeederRPtr));
    return TRUE;
}


BOOL _SWDMX_RmSetStrmID(
    UINT8 u1SrcId,
    UINT8 u1HdrType,
    UINT32 u4StrmID,
    UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    UNUSED(u4StrmSubID);
    switch (u1HdrType)
    {
    case eSWDMX_STRM_TYPE_VID:
        prSwdmxRmInfo->u4VStrmID = u4StrmID;
        break;
    case eSWDMX_STRM_TYPE_AUD:
        prSwdmxRmInfo->u4AStrmID = u4StrmID;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_RmEnableStrm(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    switch (u1StrmType)
    {
    case eSWDMX_STRM_TYPE_VID:
        prSwdmxRmInfo->fgEnVideo = TRUE;
        if (prSwdmxRmInfo->fgEnVDmx == FALSE)
        {
            _SWDMX_RmInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO, FALSE);
            prSwdmxRmInfo->fgEnVDmx = TRUE;
        }
        break;
    case eSWDMX_STRM_TYPE_AUD:
        if (prSwdmxRmInfo->fgEnADmx == FALSE)
        {
            _SWDMX_RmInitDmx(prSwdmxInst, eSWDMX_MEDIA_AUDIO, FALSE);
            prSwdmxRmInfo->fgEnADmx = TRUE;
        }
        if ((prSwdmxRmInfo->u4PrevAStrmID != 0xFFFFFFFF) &&
            (prSwdmxRmInfo->u4AStrmID != prSwdmxRmInfo->u4PrevAStrmID))
        {
            prSwdmxRmInfo->fgChgATrack = TRUE;
        }
        prSwdmxRmInfo->u4PrevAStrmID = prSwdmxRmInfo->u4AStrmID;
        prSwdmxRmInfo->fgEnAudio = TRUE;
        prSwdmxRmInfo->u1TotalATrack = 1;  // tmp solution for MW not setting this
        break;
    default:
        return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_RmDisableStrm(UINT8 u1SrcId, UINT8 u1StrmType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    switch (u1StrmType)
    {
    case eSWDMX_STRM_TYPE_VID:
        if(prSwdmxRmInfo->fgVDecIsPause)
        {
            _SWDMX_RmVideoWakeUp(prSwdmxInst);
            prSwdmxRmInfo->fgVDecIsPause = FALSE;
        }
        prSwdmxRmInfo->fgEnVideo = FALSE;
        break;
    case eSWDMX_STRM_TYPE_AUD:
        prSwdmxRmInfo->fgEnAudio = FALSE;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_RmInitDmx(SWDMX_INFO_T *prSwdmxInst, UINT8 u1HdrType, BOOL fgEos)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    BOOL fgRet = FALSE;
    UINT32 u4Flag = 0;
    DMX_MM_T rDmxMMInfo;
    UINT32 u4Addr = 0, u4Size = 0;

    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!prSwdmxRmInfo->fgEnVDmx)
        {
            DMX_SetToDecoder(TRUE);

            prSwdmxRmInfo->u1VidPid =
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            if (!_SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &u4Addr, &u4Size))
            {
                LOG(3, "_SWDMX_RmInitDmx call _SWDMX_GetVideoFifo Fail\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE
                | DMX_MM_FLAG_VALID
                | DMX_MM_FLAG_BUF
                | DMX_MM_FLAG_INSTANCE_TAG
                | DMX_MM_FLAG_DEVICE_ID);

            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
            rDmxMMInfo.u1DeviceId = VLD0;
            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufSize = u4Size;
            rDmxMMInfo.u4BufAddr = u4Addr;
            prSwdmxRmInfo->u4VFifoSa = VIRTUAL(u4Addr);
            prSwdmxRmInfo->u4VFifoEa = VIRTUAL(u4Addr + u4Size);

            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

            ASSERT(prSwdmxRmInfo->u4VDecType == ENUM_SWDMX_VDEC_RV);
            LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(prSwdmxRmInfo->u1VidPid, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(prSwdmxRmInfo->u1VidPid, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        //rDmxMMInfo.ePidType = DMX_PID_TYPE_PURE_AUDIO;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!prSwdmxRmInfo->fgEnADmx)
        {
            DMX_SetToDecoder(TRUE);

            prSwdmxRmInfo->u1AudPid =
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            if(!_SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4Addr, &u4Size))
            {
                LOG(3, "_SWDMX_MpgEsSetAudioId _SWDMX_GetAudioFifo Fail\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE
                | DMX_MM_FLAG_VALID
                | DMX_MM_FLAG_BUF
                | DMX_MM_FLAG_INSTANCE_TAG
                | DMX_MM_FLAG_DEVICE_ID);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = u4Addr;//au4BufStart[0];
            rDmxMMInfo.u4BufSize = u4Size;//au4BufEnd[0] - au4BufStart[0];
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1AdecId;
            rDmxMMInfo.u1DeviceId = prSwdmxInst->u1AdecId;

            fgRet = DMX_MM_Set(prSwdmxRmInfo->u1AudPid, u4Flag, &rDmxMMInfo);
            prSwdmxRmInfo->u4AFifoSa = VIRTUAL(rDmxMMInfo.u4BufAddr);
            prSwdmxRmInfo->u4AFifoSz = rDmxMMInfo.u4BufSize;
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(prSwdmxRmInfo->u1AudPid, u4Flag, &rDmxMMInfo);
        }
    }

    return fgRet;
}


static BOOL _SWDMX_SendRmEos(SWDMX_INFO_T *prSwdmxInst, UINT8 u1HdrType)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    FEEDER_BUF_INFO_T *prFeederInfo = &prSwdmxRmInfo->rFeederInfo;
    BOOL fgRet;
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    //DMX_MM_DATA_T prSwdmxRmInfo->rRmDmxMMData;

    LOG(5, "SWDMX Send Rm Eos Hdr Type = %d\n", u1HdrType);

#ifdef __MODEL_slt__
    LOG(5, "RM SLT last pic type = 0x%X, PTS = %d\n", prSwdmxRmInfo->u1RmSltLastPicType, prSwdmxRmInfo->u4RmSltLastPicPts);
#endif

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxRmInfo->rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (prSwdmxRmInfo->u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            prSwdmxInst->fgFifoFull=TRUE;
            return FALSE;
        }
    }

    x_memset(&prSwdmxRmInfo->rRmDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_RmInitDmx(prSwdmxInst, u1HdrType, TRUE) == FALSE)
    {
        return FALSE;
    }

    prSwdmxRmInfo->rRmDmxMMData.u4BufStart = prFeederInfo->u4StartAddr;
    prSwdmxRmInfo->rRmDmxMMData.u4BufEnd = prFeederInfo->u4EndAddr;
    prSwdmxRmInfo->rRmDmxMMData.u4StartAddr = prFeederInfo->u4StartAddr;
    prSwdmxRmInfo->rRmDmxMMData.u4FrameSize = 4;
    prSwdmxRmInfo->rRmDmxMMData.fgEOS = TRUE;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        prSwdmxRmInfo->rRmDmxMMData.u1Idx = prSwdmxRmInfo->u1VidPid;
        fgRet = _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &prSwdmxRmInfo->rRmDmxMMData);
    }
    else if (eSWDMX_MEDIA_AUDIO == u1HdrType)
    {
        prSwdmxRmInfo->rRmDmxMMData.u1Idx = prSwdmxRmInfo->u1AudPid;
        fgRet = _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &prSwdmxRmInfo->rRmDmxMMData);
    }
    else
    {
        fgRet = FALSE;
        Printf("Unexpected u1HdrType: %d @ %s,%d\n", u1HdrType, __FUNCTION__, __LINE__);
        ASSERT(0);
    }

    if (fgRet)
    {
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
            vRmPIsr(prSwdmxInst, TRUE);  // to wait until DMX has finished sending EOS
    }
    else
    {
        ASSERT(0);
    }

    return fgRet;
}


/**
*/
BOOL _SWDMX_RmStop(UINT8 u1SrcId)
{
    DMX_MM_T rPid;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    rPid.fgEnable = FALSE;

    if (prSwdmxRmInfo->fgEnVDmx)
    {
        prSwdmxRmInfo->fgEnVDmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxRmInfo->u1VidPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxRmInfo->u1VidPid);
        DMX_MUL_FreePidx(prSwdmxRmInfo->u1VidPid);
    }

    if (prSwdmxRmInfo->fgEnADmx)
    {
        prSwdmxRmInfo->fgEnADmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxRmInfo->u1AudPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxRmInfo->u1AudPid);
        DMX_MUL_FreePidx(prSwdmxRmInfo->u1AudPid);
    }

    x_memset(&prSwdmxInst->rFmtInfo.rSwdmxRmInfo, 0, sizeof(SWDMX_RM_INFO_T));

    return TRUE;
}

static BOOL _SWDMX_RmSeekCurPos(SWDMX_INFO_T *prSwdmxInst)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
	VDP_POS_INTO_T rVdpPosInfo;
	UINT32 i=0;
	if(VDP_GetPosInfo(prSwdmxInst->u1B2rId,&rVdpPosInfo))
	{
	    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
        {
           prSwdmxRmInfo->dwCurCookFrameInSB = 0;
           prSwdmxRmInfo->fgCookSBPending = FALSE;
        }
		prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO]=rVdpPosInfo.u8OffsetI;
		prSwdmxRmInfo->u4CurVidPts=(UINT32)rVdpPosInfo.u8Pts;
		prSwdmxRmInfo->u4CurAudPts=rVdpPosInfo.u8AudPts;
		for (i = 0; (i < prSwdmxRmInfo->u1TotalATrack) && (i < RM_MAX_AUDIO_TRACK); i++)
		{
           prSwdmxRmInfo->u4AudPts[i]=(UINT32)rVdpPosInfo.u8AudPts;
		}
		_SWDMX_RmResetFeeder(prSwdmxInst);
		prSwdmxRmInfo->fgIsSeek = TRUE;
		prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
		prSwdmxRmInfo->au8CurFilePos[RM_STRM_VDO] = prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO];
		return TRUE;
	}
    return FALSE;
}

static BOOL _SWDMX_RmSeek(SWDMX_INFO_T *prSwdmxInst, UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList, BOOL fgNoDecrement)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    INT32 i;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = prSwdmxRmInfo->prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prAudKeyIdxTbl = 0;
    UINT32* pu4AudPts;

    LOG(7, "_SWDMX_RmSeek u8SeekTime = 0x%08x\n", u8SeekTime);

    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
    {
        prSwdmxRmInfo->dwCurCookFrameInSB = 0;
        prSwdmxRmInfo->fgCookSBPending = FALSE;
        //printf("swdmx_rm 4694 reset prSwdmxRmInfo->dwCurCookFrameInSB\n");
    }

    if (/*!prSwdmxRmInfo->fgKeyExist || !!! */
        !prSwdmxRmInfo->prVidKeyIdxTbl ||
        prSwdmxRmInfo->prVidKeyIdxTbl->ui4_number_of_entry == 0)
    {
        LOG(5, "_SWDMX_RmSeek doesn't have the key table\n");
        return FALSE;
    }

    prSwdmxRmInfo->prCurVidKeyIdxTbl = prVidKeyIdxTbl;
    for (i = 0; i < prVidKeyIdxTbl->ui4_number_of_entry; i++)
    {
        if (prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts * 90 > u8SeekTime)
        {
            break;
        }
    }

    // Error Handling
    // i = 0 is impossible
    if (i > 0)
    {
        prSwdmxRmInfo->u4CurSentKey = i - (fgNoDecrement ? 0 : 1);
    }
    else
    {
        prSwdmxRmInfo->u4CurSentKey= 0;
    }

    prSwdmxRmInfo->u4CurRepeatFrameIdx = prSwdmxRmInfo->u4CurSentKey;

    if (prVidKeyIdxTbl != 0)
    {
        if (u8SeekTime == 0)
        {
            prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO] = prSwdmxRmInfo->au8FileOffset[RM_STRM_VDO];
        }
        else
        {
            prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO] =
                prVidKeyIdxTbl->ui8_base_offset +
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurSentKey].ui4_relative_offset;
        }
        prSwdmxRmInfo->u4CurVidPts =
            prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurSentKey].ui4_pts * 90;

        LOG(5, "Rm Seek key frame number = 0x%08x Video offset = %lld\n",
            prSwdmxRmInfo->u4CurSentKey,
            prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO]);

        LOG(5, "Rm Seek Video pts = 0x%08x\n",
            prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurSentKey].ui4_pts * 90);
    }

    for (i = 0; (i < prSwdmxRmInfo->u1TotalATrack) && (i < RM_MAX_AUDIO_TRACK); i++)
    {
        if (prVidKeyIdxTbl == prSwdmxRmInfo->prVidKeyIdxTbl)
        {
            prAudKeyIdxTbl = prSwdmxRmInfo->prAudIdxTbl[i];
            pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;

            LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
        }
        else
        {
            pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;
        }
        
        if (pu4AudPts)
        {
            prSwdmxRmInfo->u4AudPts[i] =
                pu4AudPts[prSwdmxRmInfo->u4CurSentKey];
            LOG(5, "Rm Seek Audio pts = 0x%08x\n",
                pu4AudPts[prSwdmxRmInfo->u4CurSentKey]);
        }
    }

    _SWDMX_RmResetFeeder(prSwdmxInst);

    prSwdmxRmInfo->fgIsSeek = TRUE;
    prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
    prSwdmxRmInfo->au8CurFilePos[RM_STRM_VDO] = prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO];

    // Set STC, temp, should update by audio (audio master)
    //STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxRmInfo->u4CurVidPts);

    LOG(7, "_SWDMX_RmSeek Key = %ld\n", prSwdmxRmInfo->u4CurSentKey);

    return TRUE;
}

static BOOL _SWDMX_RmSeekPos(SWDMX_INFO_T *prSwdmxInst, START_POS_INFO_T *ptJumpInfo)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;

    if (!prSwdmxInst || !ptJumpInfo)
    {
        return FALSE;
    }


    if (prSwdmxRmInfo->u4ADecType == ENUM_SWDMX_ADEC_RA)
    {
        prSwdmxRmInfo->dwCurCookFrameInSB = 0;
        prSwdmxRmInfo->fgCookSBPending = FALSE;
    }


    LOG(7, "_SWDMX_RmSeek u8SeekPos\n");

    //todo 
    prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO] = ptJumpInfo->ui8_i_frame_position;
    prSwdmxRmInfo->u4CurVidPts = ptJumpInfo->ui8_i_pts_info;

    _SWDMX_RmResetFeeder(prSwdmxInst);

    prSwdmxRmInfo->fgIsSeek = TRUE;
    prSwdmxRmInfo->u4LeftBytesInFeeder = 0;
    prSwdmxRmInfo->au8CurFilePos[RM_STRM_VDO] = prSwdmxRmInfo->au8CurDmxFilePos[RM_STRM_VDO];

    return TRUE;
}



static BOOL _SWDMX_RmGetDataFromFeeder(SWDMX_INFO_T *prSwdmxInst, FEEDER_REQ_DATA_T *prFeederData)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    if (!_SWDMX_ReceiveFeederAck(prSwdmxInst->u1SwdmxId, (VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_RmGetDataFromFeeder FeederTimeOut!!!\n");
        prSwdmxRmInfo->fgFeederInvalid = TRUE;
        return FALSE;
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
         LOG(3, "_SWDMX_RmGetDataFromFeeder invalid ++++++\n");
#if 0
#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            prSwdmxRmInfo->fgReqDataMiss = TRUE;
        }
        else
#endif
        {
            prSwdmxRmInfo->fgFeederInvalid = TRUE;
        }
#endif
        return FALSE;
    }

    if ((prFeederData->u4Id + 1) != prSwdmxRmInfo->u4FeederReqID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        prSwdmxRmInfo->fgFeederInvalid = TRUE;
        return FALSE;
    }


    if(!prFeederData->u4WriteAddr)
    {
        LOG(3, "Feeder Ack error id= %ld\n", prFeederData->u4Id);
        prSwdmxRmInfo->fgFeederInvalid = TRUE;
        return FALSE;      
    }

    prSwdmxRmInfo->fgReqDataMiss = FALSE;

    return TRUE;
}


static BOOL _SWDMX_RmCalculateRepeatFrame(SWDMX_INFO_T *prSwdmxInst, INT32 i4PlaySpeed)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    UINT32 u4CurKey = prSwdmxRmInfo->u4CurSentKey;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl = prSwdmxRmInfo->prVidKeyIdxTbl;

    if (prSwdmxRmInfo->u4VidRepeatFrame == 0)
    {
        if (prSwdmxRmInfo->u4CurRepeatFrameIdx != u4CurKey &&
            (prSwdmxRmInfo->u4CurRepeatFrameIdx < prKeyIdxTbl->ui4_number_of_entry))
        {
            UINT32 u4PtsDif = 0, u4MaxRepeatFrame;

            //u4MaxRepeatFrame = (prSwdmxRmInfo->u4VidFps * 1000 * 2)/RM_ABS(i4PlaySpeed);
            // remove thread delay when merging packets into frame, so change max repeat frame
            u4MaxRepeatFrame = (prSwdmxRmInfo->u4VidFps * 1000 * 8)/RM_ABS(i4PlaySpeed);

            if (i4PlaySpeed > 2000)
            {
                if ((u4CurKey < (prKeyIdxTbl->ui4_number_of_entry)) &&
                    (prKeyIdxTbl->ui4_number_of_entry > RM_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = (prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurRepeatFrameIdx].ui4_pts) * 90;

                    prSwdmxRmInfo->u4VidRepeatFrame =
                        (UINT32)((u4PtsDif * prSwdmxRmInfo->u4VidFps)/(90 * i4PlaySpeed));
                }
                else
                {
                    prSwdmxRmInfo->u4VidRepeatFrame = (prSwdmxRmInfo->u4VidFps * 1000)/RM_ABS(i4PlaySpeed);
                }
            }
            else if (i4PlaySpeed < 0)
            {
                if ((prKeyIdxTbl->ui4_number_of_entry > RM_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = (prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prSwdmxRmInfo->u4CurRepeatFrameIdx].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts) * 90;

                    prSwdmxRmInfo->u4VidRepeatFrame =
                        (UINT32)((u4PtsDif * prSwdmxRmInfo->u4VidFps)/(90 * RM_ABS(i4PlaySpeed)));
                }
                else
                {
                    prSwdmxRmInfo->u4VidRepeatFrame = (prSwdmxRmInfo->u4VidFps * 1000)/RM_ABS(i4PlaySpeed);
                }
            }

            prSwdmxRmInfo->u4VidRepeatFrame = (u4MaxRepeatFrame > prSwdmxRmInfo->u4VidRepeatFrame) ?
                (prSwdmxRmInfo->u4VidRepeatFrame) : u4MaxRepeatFrame;

            if ((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||
                (u4CurKey == 0))
            {
                prSwdmxRmInfo->u4VidRepeatFrame =
                    (RM_FIX_REPEAT_FRAME/(RM_ABS(i4PlaySpeed)/1000));

                if (prSwdmxRmInfo->u4VidRepeatFrame > RM_MAX_REPEAT_FRAME)
                {
                    prSwdmxRmInfo->u4VidRepeatFrame = RM_MAX_REPEAT_FRAME;
                }
                else if (prSwdmxRmInfo->u4VidRepeatFrame < RM_MIN_REPEAT_FRAME)
                {
                    prSwdmxRmInfo->u4VidRepeatFrame = RM_MIN_REPEAT_FRAME;
                }
            }

            LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n",
                u4PtsDif, u4CurKey, prSwdmxRmInfo->u4VidRepeatFrame);

            if (prSwdmxRmInfo->u4VidRepeatFrame == 0)
            {
                return FALSE;
            }

            prSwdmxRmInfo->u4CurRepeatFrameIdx = u4CurKey;
            LOG(5, "Repeat Idx = %ld\n", prSwdmxRmInfo->u4CurRepeatFrameIdx);

            return TRUE;
        }
        else
        {
            // Force to play the first key frame
            if (prSwdmxRmInfo->fgStartTrick)
            {
                prSwdmxRmInfo->fgStartTrick = FALSE;
                prSwdmxRmInfo->u4VidRepeatFrame = (prSwdmxRmInfo->u4VidFps * 1000)/RM_ABS(i4PlaySpeed);
                return TRUE;
            }

            return FALSE;
        }
    }

    prSwdmxRmInfo->u4VidRepeatFrame--;
    return TRUE;
}


BOOL _SWDMX_RmAudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_RM_INFO_T *prSwdmxRmInfo;
    SWDMX_RM_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if (prSwdmxRmInfo->fgEnVideo)
        {
            LOG(5, "SWDMX_RmAudioCb Audio is ready\n");

            if (prSwdmxRmInfo->u4TriggerAudPts != 0xFFFFFFFF)
            {
                VDP_TriggerAudReceive(prSwdmxRmInfo->u4TriggerAudPts);
                LOG(5, "SWDMX_RmAudioCb VDP Trigger PTS = 0x%08x\n",
                    prSwdmxRmInfo->u4TriggerAudPts);
                prSwdmxRmInfo->u4TriggerAudPts = 0xFFFFFFFF;
            }
            else
            {
                LOG(5, "Trigger audio by _SWDMX_RmAudioCb\n");
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                STC_StartStc(prSwdmxInst->u1StcId);
            }

            if(prSwdmxRmInfo->fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp(prSwdmxInst);
                prSwdmxRmInfo->fgVDecIsPause = FALSE;
            }
        }
        else
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_RmAudioCb\n");
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
        }
        prSwdmxRmInfo->fgAudIsReady = TRUE;
    }
    else
    {
        // impossible
        LOG(1, "_SWDMX_RmAudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }

    return TRUE;
}

static void _SWDMX_RmVideoWakeUp(SWDMX_INFO_T *prSwdmxInst)
{
    SWDMX_RM_INFO_T *prSwdmxRmInfo = &prSwdmxInst->rFmtInfo.rSwdmxRmInfo;
    LOG(5, "_SWDMX_RmVideoWakeUp\n");
    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    switch (prSwdmxRmInfo->u4VDecType)
    {
    case ENUM_SWDMX_VDEC_RV:
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_RV);
        break;
    default:
        LOG(1, "SWDMX_RmAudioCb Unknown Video Type\n");
        ASSERT(0);
        break;
    }
}



