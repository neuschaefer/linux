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

#define RM_KEY_TABLE_THRESHOLD (3)

#define GOOD  // ToDo: check why this will mess up COOK audio

#define RM_ONE_SEC_DURATION (90000 * 2)

#define RV_IVOP         0xb0
#define RV_FIVOP        0xb1
#define RV_PVOP         0xb2
#define RV_BVOP         0xb3

#define HX_KEYFRAME_FLAG  (0x1 << 1)

#ifdef RMLS
//UINT32 _RmLsChgStrmThrd = 90000;
//static BOOL _fgRmDbg = TRUE;
//#define RM_LS_CHG_STRM_THRD (_RmLsChgStrmThrd)
//#define RM_DBG_LOG if (_fgRmDbg) Printf
#define RM_LS_CHG_STRM_THRD (90000)
#define RM_DBG_LOG(...)
#endif

#define INPUT_SRC _rSwdmxRmInfo.prDmxRangeInfo->e_mm_src_type


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

#define abs(x)                  (((x) >= 0) ? (x) : -(x))

#define RV_PIC_TYPE(p)  ((p) | 0xB0)  // !!! must match the definition in mpv_comm.h

#define IS_SLOW_FWD(s)  ((s) > 1 && (s) < 1000)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static SWDMX_RM_INFO_T _rSwdmxRmInfo;

static FEEDER_BUF_INFO_T _rFeederInfo;

static UCHAR *_pucAudBuf = NULL;

static MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T _rPrevSample = {0};

static UINT32 _u4LastAlignOffset;

static HANDLE_T _hRmIsrMsgQ = 0;

static DMX_MM_DATA_T _rRmDmxMMData;

static UINT32 _dwPrsState;
static UINT32 _dwPktSz;
static UINT8 *_pbHdrBuf;
static UINT16 _wGTxLen;
static UINT32 _u4FrmNo = 0;

static RVFrameHdr _rFrameHdr;

#if 1  // !!! refine the follows
static UINT32 _au4SliceSz[128];  // = (UINT32 *)RM_SLICE_SZ_SA;
static UINT32 _au4SliceOfst[128];  // = (UINT32 *)(RM_SLICE_OFST_SA);

#define RM_SLICE_SZ_BUF_SA   ((UINT8 *)_au4SliceSzBuf)
#define RM_SLICE_SZ_BUF_NS   5000
static UINT32 _au4SliceSzBuf[RM_SLICE_SZ_BUF_NS];
UINT32 _dwSliceSzBufWp;
UINT32 _dwSliceSzBufRp;
#endif

UINT32 _dwFordwardRefPts = 0;
UINT32 _dwBackwardRefPts = 0;
UINT32 _dwCurrPicTr = 0;
UINT32 _dwFordwardRefTr = 0;
UINT32 _dwBackwardRefTr = 0;

static UINT32 _dwCurCookFrameInSB;
static UINT16 _pbPattern[100];
static UINT32 _dwCurAFifoOfst;
#ifdef COOK_WRITE_AFIFO
static BOOL _fgAFifoForCookSBFull = FALSE;
#endif

static INT32 _i4RmPtsAdj = -27000;

#ifdef __MODEL_slt__
UINT8 _u1RmSltLastPicType = 0;
UINT32 _u4RmSltLastPicPts = 0;
#endif

// for Real Media Logical Stream Support
#ifndef RMLS
#define _u4RmStrmIdx (RM_STRM_VDO)
#else
static BOOL _fgRmLs;
static BOOL _fgRmChgStrm;
static UINT32 _u4RmStrmIdx;
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static VOID compute_generic_interleave_pattern(VOID)
{
    INT32 codec_frame_size = _rSwdmxRmInfo.rCookInfo.u2FrameSzInBytes; //_wCookFrameSz
    INT32 interleave_block_size = _rSwdmxRmInfo.rCookInfo.u2CookBlockSz;  //_wCookBlockSz
    INT32 interleave_factor = _rSwdmxRmInfo.rCookInfo.u2CookBlockPerSB;  //_bCookBlockPerSB)
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
            _pbPattern[i] = i;
        }
    }
    else
    {
        while (i < frames_per_superblock && i < 100)
        {
            _pbPattern[i] = b * frames_per_block + f;
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


UINT32 fgCookBufCpy(UINT8* str1, UINT8* str2, UINT32 idx1, UINT32 idx2, UINT32 NoCpy, UINT32 dwLastFrameOfst)
{
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

       if (_rSwdmxRmInfo.rCookInfo.u1ChanNum == 2 && _rSwdmxRmInfo.rCookInfo.u2CplQBits == 0)
       {
           if (dwCurFrameOfst == _rSwdmxRmInfo.rCookInfo.u2FrameSzInBytes/2)
               dwCurFrameOfst = 0;
       }
       else
       {
           if (dwCurFrameOfst == _rSwdmxRmInfo.rCookInfo.u2FrameSzInBytes)
               dwCurFrameOfst = 0;
       }
  }

  return dwCurFrameOfst;
}


static UINT32 dwAFifoSz(VOID)
{
#ifdef COOK_WRITE_AFIFO
    return _rSwdmxRmInfo.u4AFifoSz;
#else
    return RA_ALLOC_SIZE;
#endif
}

#ifdef COOK_WRITE_AFIFO
static UINT32 dwAFifoForCookSBSpace(VOID)
{
    return DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO);
}
#endif

static VOID vCookFrameDeInterleave(UINT8 *pu1Data)
{
    int i;
    int j;
    UINT32 dwTgtFrameOfst;
    UINT32 dwLastFrameOfst;
    UINT16 wCookFrameSz = _rSwdmxRmInfo.rCookInfo.u2FrameSzInBytes;
    UCHAR *pucBuf;
#ifdef COOK_WRITE_AFIFO
#ifdef CC_MT5363
    UINT32 u4AWp;
    BOOL fgRet;
#endif
#endif

    if (_rSwdmxRmInfo.fgCookSBPending)
    {
        ASSERT(pu1Data == NULL && _dwCurCookFrameInSB == _rSwdmxRmInfo.rCookInfo.u2CookFramePerSB);
        pucBuf = _pucAudBuf;
        goto COOK_DMX;
    }

#ifdef COOK_WRITE_AFIFO
    //Should not go here! Very serious error.
    if (_fgAFifoForCookSBFull)
        ASSERT(0);
#endif

#ifdef COOK_WRITE_AFIFO
    pucBuf = (UCHAR *)_rSwdmxRmInfo.u4AFifoSa;
#else
    if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA &&
        _pucAudBuf == NULL)
    {
        _pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(max(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        if (_pucAudBuf == NULL)
        {
            LOG(1, "Allocate RA buffer failed!\n");
            ASSERT(0);
            return;
        }
    }
    pucBuf = _pucAudBuf;
#endif

    for (i = 0; i < _rSwdmxRmInfo.rCookInfo.u2CookFramePerBlock; i++)
    {
       for (j = 0; j < _rSwdmxRmInfo.rCookInfo.u2CookFramePerSB && j < 100; j++)
       {
           if (_pbPattern[j] == _dwCurCookFrameInSB + i)
               break;
       }

       dwTgtFrameOfst = j * wCookFrameSz;

       if (_dwCurAFifoOfst + dwTgtFrameOfst + wCookFrameSz <= dwAFifoSz())
       {
           fgCookBufCpy(pucBuf, pu1Data, _dwCurAFifoOfst + dwTgtFrameOfst, i*wCookFrameSz, wCookFrameSz, 0);
       }
       else
       {
          if (_dwCurAFifoOfst + dwTgtFrameOfst < dwAFifoSz())
          {
             dwLastFrameOfst =  fgCookBufCpy(pucBuf, pu1Data, _dwCurAFifoOfst + dwTgtFrameOfst, i*wCookFrameSz, dwAFifoSz() - (_dwCurAFifoOfst + dwTgtFrameOfst), 0);
             fgCookBufCpy(pucBuf, pu1Data, 0, i*wCookFrameSz + dwAFifoSz() - (_dwCurAFifoOfst + dwTgtFrameOfst), _dwCurAFifoOfst + dwTgtFrameOfst + wCookFrameSz -dwAFifoSz(),  dwLastFrameOfst);
          }
          else
          {
             fgCookBufCpy(pucBuf, pu1Data, _dwCurAFifoOfst + dwTgtFrameOfst - dwAFifoSz(), i*wCookFrameSz, wCookFrameSz, 0);
         }
       }
    }

    _dwCurCookFrameInSB += _rSwdmxRmInfo.rCookInfo.u2CookFramePerBlock;
    //printf("swdmx_rm 315 _dwCurCookFrameInSB ==> %d\n", _dwCurCookFrameInSB);

COOK_DMX:
    if (_dwCurCookFrameInSB == _rSwdmxRmInfo.rCookInfo.u2CookFramePerSB)
    {
        //printf("swdmx_rm 319 TX SB!!!\n");
        BOOL fgFifoEnough = (DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO) > _rSwdmxRmInfo.rCookInfo.u2CookSBSz);

        if (fgFifoEnough)
        {
            if (_rSwdmxRmInfo.fgCookSBPending)
            {
                _rSwdmxRmInfo.fgCookSBPending = FALSE;
                LOG(0, "Clear COOK SB pending, count = %d...\n", _rSwdmxRmInfo.u4CookSBPendingCnt);
                _rSwdmxRmInfo.u4CookSBPendingCnt = 0;
            }
        }
        else
        {
            UINT16 u2CurQNum, u2MaxQNum;
            VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
            if (u2CurQNum <= 2)
            {
                LOG(0, "Video dry due to COOK SB pending: %d!!!\n", u2CurQNum);
            }

            if (!_rSwdmxRmInfo.fgCookSBPending)  // first time setting pending flag, reset the count
            {
                _rSwdmxRmInfo.u4CookSBPendingCnt = 1;
                LOG(0, "COOK SB pending!!!!!!\n");
            }
            else
            {
                _rSwdmxRmInfo.u4CookSBPendingCnt++;
            }
            _rSwdmxRmInfo.fgCookSBPending = TRUE;
            //ASSERT(0);
            return;
        }

#ifndef COOK_WRITE_AFIFO
        DMX_MM_DATA_T rDmxMMData;

        x_memset(&rDmxMMData, 0, sizeof(rDmxMMData));
        rDmxMMData.u4BufStart = (UINT32)pucBuf;
        rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + RA_ALLOC_SIZE;
        rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
        rDmxMMData.u4FrameSize = _rSwdmxRmInfo.rCookInfo.u2FrameSzInBytes;//u2CookSBSz;
        rDmxMMData.u4Pts = _rSwdmxRmInfo.u4CookSBPts;

        // DSP asks for sending each frame one by one instead of sending a whole SB at one time
        for (i = 0; i < _rSwdmxRmInfo.rCookInfo.u2CookFramePerSB; i++)
        {
            if (DMX_MM_MoveData(eSWDMX_MEDIA_AUDIO, &rDmxMMData, 500) != TRUE)
            {
                LOG(0, "Demuxer fails to move data.\n");
                ASSERT(0);
            }
            rDmxMMData.u4StartAddr += rDmxMMData.u4FrameSize;
        }
#ifdef RMLS
    if (_fgRmLs)
    {
        ASSERT(RM_STRM_AUD == _u4RmStrmIdx);
        _rSwdmxRmInfo.u4LsPts[RM_STRM_AUD] = _rSwdmxRmInfo.u4CookSBPts;

        if (_rSwdmxRmInfo.fgDeliverVEos)
        {
            // Video is finished, no need to change to video
        }
        else if (_rSwdmxRmInfo.u4LsPts[RM_STRM_AUD] > (_rSwdmxRmInfo.u4LsPts[RM_STRM_VDO] + RM_LS_CHG_STRM_THRD))
        {
            RM_DBG_LOG("419:Set _fgRmChgStrm\n");
            _fgRmChgStrm = TRUE;
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
        _dwCurCookFrameInSB = 0;
#ifdef COOK_WRITE_AFIFO
        if(_dwCurAFifoOfst + _rSwdmxRmInfo.rCookInfo.u2CookSBSz <= dwAFifoSz())
        {
            _dwCurAFifoOfst += _rSwdmxRmInfo.rCookInfo.u2CookSBSz;
        }
        else
        {
            _dwCurAFifoOfst = _dwCurAFifoOfst + _rSwdmxRmInfo.rCookInfo.u2CookSBSz - dwAFifoSz();
        }
#endif
        //_dwSimAFifoWp = ((DWRD)_pbAFifo + _dwCurAFifoOfst) & 0xFFFFFF;  // megaa debug 0708

#ifdef COOK_WRITE_AFIFO
#ifdef CC_MT5363
       u4AWp = PHYSICAL(_rSwdmxRmInfo.u4AFifoSa + _dwCurAFifoOfst);
       fgRet = DMX_MM_SendAudioChunk(eSWDMX_MEDIA_AUDIO, u4AWp, _rRmDmxMMData.u4Pts, FALSE);
       ASSERT(fgRet);
       if (!fgRet) return;
#endif
#endif

#ifdef REAL_WRITE_AFIFO
        _dwSimAFifoRp = dPRAFifoRp();
        _dwSimAFifoWp = dPRAFifoWp();
#endif

#ifdef COOK_WRITE_AFIFO
        if (dwAFifoForCookSBSpace() <= _rSwdmxRmInfo.rCookInfo.u2CookSBSz)
        {
            //_dwAFifoForCookSBFullCounter++;
            _fgAFifoForCookSBFull = TRUE;
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
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd,
    BOOL fgOneVideoFrm);

static BOOL _CheckRmPacketIndex(UCHAR *pucBuf, UINT32 u4StrmID, BOOL *pfgVer1);
static BOOL _CheckRmPacketType(UCHAR *pucBuf, UINT8 *pu1HdrType, UINT8 *pu1TrackIdx, BOOL *fgVer1, UINT32 *pu4Timestamp, UINT16 *pu2Flag, UINT32 *pu4PktSize);

static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsWithoutIdx(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

//static SWDMX_HDR_PRS_STATE_T  _SWDMX_BeforeDeliverUseIdx(UINT8 u1HdrType);

static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsUseIdx(
    INT32 i4Speed,
    SWDMX_RANGE_LIST_T* prRangeList);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverRmKeyframe(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl,
    UINT32* pu4CurIdx,
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed);
static BOOL _SWDMX_RmInitDmx(UINT8 u1HdrType, BOOL fgEos);

static BOOL _SWDMX_SendRmEos(UINT8 u1HdrType);

static BOOL _SWDMX_RmSeek(UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList, BOOL fgNoDecrement);

static BOOL _SWDMX_RmGetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_RmCalculateRepeatFrame(INT32 i4PlaySpeed);

static BOOL _SWDMX_RmResetFeeder(void);

static void _SWDMX_RmVideoWakeUp(void);


// !!!
static void _SWDMX_RmCalculatePts(UINT8 u1HdrType, UINT8 u1TrackIdx, UINT32 u4ChunkSize)
{
}


static void vMpsPrsNextG(INT32 adv, UINT32 len, INT32 dummy)
{
    UINT32 u4SrcBufLen = _rRmDmxMMData.u4BufEnd - _rRmDmxMMData.u4BufStart;

    _pbHdrBuf += adv;
    if ((UINT32)_pbHdrBuf >= _rRmDmxMMData.u4BufEnd)
    {
        _pbHdrBuf -= u4SrcBufLen;
    }
    _wGTxLen = len;
    UNUSED(dummy);

    // megaa 20090601: ToDo: copy from ring buffer to a plain buffer
}


// megaa 20090601: ToDo: implement it
static BOOL fgRmSliceSzBufChk(void)
{
    return TRUE;
}


void vGetRvSliceSz(UINT32 *pu4SliceSzArr, UINT32 u4Rp, UINT32 u4Cnt)
{
    while (u4Cnt > 0)
    {
        *pu4SliceSzArr = _au4SliceSzBuf[u4Rp];
        pu4SliceSzArr++;
        if (++u4Rp >= RM_SLICE_SZ_BUF_NS)
            u4Rp = 0;

        u4Cnt--;
    }

    _dwSliceSzBufRp = u4Rp;
}


static void vSendVFrame(void)
{
  //extern DWRD _dwPrVRp;
  //extern DWRD _dwPrVWp;
  //extern volatile int _fgFrameRdy;
  //extern UINT32 _u4FrameLen;
  int i;
  //BYTE *pbData = (BYTE *)dUtNCLogDramAddr(_dVWpBak);
  UINT8 *pbData = (UINT8 *)(_rSwdmxRmInfo.u4VFifoWpBak/* + DRAMA_NONCACH_BASE_ADDRESS*/);
  UINT8 *pbDataWarp;
  //char szName[260];
  //FILE *fp;

  // !!! code corresponding to rv_depack_internal.c #1040
  if (_rFrameHdr.fgBrokenUpByUs && _rFrameHdr.u4NumPackets > 1)
  {
    //ASSERT(_rFrameHdr.u4NumPackets == 2);
    _rFrameHdr.u4NumPackets = 1;
    _au4SliceSz[0] += _au4SliceSz[1];
  }

  // count the original frame length before ripping off ECC packets
  //for (_u4FrameLen = 0, i = 0; i < _rFrameHdr.u4NumPackets; i++)
  //  _u4FrameLen += _au4SliceSz[i];

  // rip off ECC packets
  for (i = _rFrameHdr.u4NumPackets - 1; i >= 0; i--)
  {
    pbDataWarp = pbData + _au4SliceOfst[i];
    if ((UINT32)pbDataWarp >= _rSwdmxRmInfo.u4VFifoEa)
      pbDataWarp -= (_rSwdmxRmInfo.u4VFifoEa - _rSwdmxRmInfo.u4VFifoSa);
    if (*pbDataWarp & _rSwdmxRmInfo.u1EccMask)
      _rFrameHdr.u4NumPackets--;
  }

#if 0
  printf("[SWDMX] Frame #%d: %d slices (%d...)\n", _u4FrmNo, _rFrameHdr.u4NumPackets, _au4SliceSz[0]);
  for (i = 0; i < _rFrameHdr.u4NumPackets; i++)
  {
    printf("sliceSz: %d\n", _au4SliceSz[i]);
  }
  for (i = 0; i < _rFrameHdr.u4NumPackets; i++)
  {
    printf("sliceOfst: %d\n", _au4SliceOfst[i]);
  }
#endif
#if 0
  sprintf(szName, "f%07d.bin", _u4FrmNo);
  fp = fopen(szName, "wb");
  fwrite((void *)pbData, 1, _dwPrVWp - _dwPrVRp, fp);
  fclose(fp);
#endif
  //_fgFrameRdy = TRUE;

  //_dwPrVWp = _dwPrVRp;
  _u4FrmNo++;
}


static void vRmAddPic(VDEC_PES_INFO_T *prPesInfo)
{
    UINT8 *pbSc = (UINT8 *)(_rSwdmxRmInfo.u4VFifoWpBak/* + DRAMA_NONCACH_BASE_ADDRESS*/);
    INT32 i4TrDelta;
    UINT32 i;
    UINT8 bType;
    BOOL fgDoSend = TRUE;

    // add slice info
#if 1
    prPesInfo->u4RenderPts = _dwSliceSzBufWp;
    prPesInfo->u1SliceCount = _rFrameHdr.u4NumPackets;
#else
    _ptPicIdx[_dwPicWrIdx].dwSliceSzBufWp = _dwSliceSzBufWp;
    _ptPicIdx[_dwPicWrIdx].dwSliceNum = _rFrameHdr.u4NumPackets;
#endif
    for (i = 0; i < _rFrameHdr.u4NumPackets; i++)
    {
        *((UINT32 *)RM_SLICE_SZ_BUF_SA + _dwSliceSzBufWp) = _au4SliceSz[i];
        if (++_dwSliceSzBufWp == RM_SLICE_SZ_BUF_NS)
        {
            _dwSliceSzBufWp = 0;
        }
    }

    //vPicAddrAdd(dwPtr, TRUE);
    prPesInfo->u4PTS *= 90;
    //printf("[SWDMX_RM]About to call VDEC_SendEs, B4 pts = %X\n", prPesInfo->u4PTS);

    _dwCurrPicTr = 0;
    for(i = 0; i < 4; i++)
    {
        if((UINT32)pbSc >= _rSwdmxRmInfo.u4VFifoEa)
        {
            pbSc = (UINT8 *)_rSwdmxRmInfo.u4VFifoSa;
        }

        _dwCurrPicTr <<= 8;
        _dwCurrPicTr |= ((*pbSc) & 0xFF);

        pbSc++ ;
    }
    pbSc = (UINT8 *)(_rSwdmxRmInfo.u4VFifoWpBak/* + DRAMA_NONCACH_BASE_ADDRESS*/);

    if (_rSwdmxRmInfo.u1EccMask == 0x20)  // it is RV8, tmp solution
    {
      bType = RV_PIC_TYPE(((*pbSc) & 0x18) >> 3);

      //_dwCurrPicTr = (((((*pbSc)<<24) + ((*(pbSc+1))<<16) + ((*(pbSc+2))<<8) + (*(pbSc+3))) & 0xFFF80) >> 7);
      _dwCurrPicTr = ((_dwCurrPicTr & 0xFFF80) >> 7);

      if(bType == RV_BVOP)
      {
        if (_dwFordwardRefPts >= _dwBackwardRefPts)  // tmp solution for out-of-order B frames
          fgDoSend = FALSE;

        i4TrDelta = _dwCurrPicTr - _dwFordwardRefTr;
        if(i4TrDelta < 0)
        {
          i4TrDelta += 0x2000;
        }
        prPesInfo->u4PTS = _dwFordwardRefPts + (i4TrDelta*90);
      }
      else
      {
        _dwFordwardRefTr = _dwBackwardRefTr;
        _dwBackwardRefTr = _dwCurrPicTr;
        _dwFordwardRefPts = _dwBackwardRefPts;
        _dwBackwardRefPts = prPesInfo->u4PTS;
      }
    }
    else if (_rSwdmxRmInfo.u1EccMask == 0x80)  // it is RV9 or RV10, tmp solution
    {
      bType = RV_PIC_TYPE(((*pbSc) & 0x60) >> 5);

      //_dwCurrPicTr = (((((*pbSc)<<24) + ((*(pbSc+1))<<16) + ((*(pbSc+2))<<8) + (*(pbSc+3))) & 0x7FFC0) >> 6);
      _dwCurrPicTr = ((_dwCurrPicTr & 0x7FFC0) >> 6);

      if(bType == RV_BVOP)
      {
        if (_dwFordwardRefPts >= _dwBackwardRefPts)  // tmp solution for out-of-order B frames
          fgDoSend = FALSE;

        i4TrDelta = _dwCurrPicTr - _dwFordwardRefTr;
        if(i4TrDelta < 0)
        {
          i4TrDelta += 0x2000;
        }
        prPesInfo->u4PTS = _dwFordwardRefPts + (i4TrDelta*90);
      }
      else
      {
        _dwFordwardRefTr = _dwBackwardRefTr;
        _dwBackwardRefTr = _dwCurrPicTr;
        _dwFordwardRefPts = _dwBackwardRefPts;
        _dwBackwardRefPts = prPesInfo->u4PTS;
      }
    }
    else
    {
        ASSERT(0);
        return;
    }


#ifdef __MODEL_slt__
    if (prPesInfo->u4PTS > _u4RmSltLastPicPts)
    {
        _u1RmSltLastPicType = bType;
        _u4RmSltLastPicPts = prPesInfo->u4PTS;
    }
#endif

    if (!fgDoSend)
        return;

#if 1
    prPesInfo->u4VldReadPtr = PHYSICAL(_rSwdmxRmInfo.u4VFifoWpBak);
    prPesInfo->ucPicType = bType - 0xB0;
    _rSwdmxRmInfo.u4NumSentVid++;
    //printf("[SWDMX_RM]About to call VDEC_SendEs, A4 pts = %X\n", prPesInfo->u4PTS);
    VDEC_SendEs((void*)prPesInfo);
#else
    //vPicTpAdd(bType);
#endif

    // check index table full
    //vPrChkPicIdxFull();
}


static void vRmPIsr(BOOL fgEos)
{
  UINT16 u2MsgQIdx;
  UINT8 u1Msg;
  SIZE_T zMsgSize;
  UINT32 u4SrcBufLen;

  do
  {
      zMsgSize = sizeof(u1Msg);
      VERIFY(x_msg_q_receive(&u2MsgQIdx, &u1Msg, &zMsgSize, &_hRmIsrMsgQ, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
  }
  while (!fgEos && _dwPrsState != RMPRS_PACKET && _dwPrsState != RMPRS_FRM_NEXT_MULTIPLE);

  if (fgEos)
      return;

  u4SrcBufLen = _rRmDmxMMData.u4BufEnd - _rRmDmxMMData.u4BufStart;
  _pbHdrBuf += _rRmDmxMMData.u4FrameSize;
  if ((UINT32)_pbHdrBuf >= _rRmDmxMMData.u4BufEnd)
  {
      _pbHdrBuf -= u4SrcBufLen;
  }

  if (_dwPrsState == RMPRS_PACKET)
  {
      vMpsPrsNextG(0, 12, 0);
  }
  else if (_dwPrsState == RMPRS_FRM_NEXT_MULTIPLE)
  {
      vMpsPrsNextG(0, 2, 0);
  }
  else
  {
      ASSERT(0);
  }
}


static BOOL fgMpsPrsNextP(INT32 adv, UINT32 len, UINT32 u4Pts)
{
    UINT32 u4SrcBufLen = _rRmDmxMMData.u4BufEnd - _rRmDmxMMData.u4BufStart;

    // check if the parsed frame size is correct
    if (len >= DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_VIDEO))
        return FALSE;  // incorrect frame size (corrupt data), return FALSE to discard it

    _pbHdrBuf += adv;
    if ((UINT32)_pbHdrBuf >= _rRmDmxMMData.u4BufEnd)
    {
        _pbHdrBuf -= u4SrcBufLen;
    }

    _wGTxLen = 0;
    _rRmDmxMMData.u4StartAddr = (UINT32)_pbHdrBuf;
    _rRmDmxMMData.u4FrameSize = len;
    _rRmDmxMMData.u4Pts = u4Pts;

    UINT32 u4BufStartBak = _rRmDmxMMData.u4BufStart;
    if (_rRmDmxMMData.u4StartAddr < _rRmDmxMMData.u4BufStart)
        _rRmDmxMMData.u4BufStart -= FEEDER_RESERVED_SIZE;

    //printf("[SWDMX_RM]About to call DMX, pts = %X\n", u4Pts * 90);
    if (DMX_MM_MoveData(eSWDMX_MEDIA_VIDEO, &_rRmDmxMMData, 500) != TRUE)
    {
        ASSERT(0);  // !!!
        LOG(0, "Demuxer fails to move data.\n");
        //*pu4ConsumeSize = 0;  !!!
        //return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;  !!!
    }

    _rRmDmxMMData.u4BufStart = u4BufStartBak;

    vRmPIsr(FALSE);

    _rRmDmxMMData.u4StartAddr += _rRmDmxMMData.u4FrameSize;
    if (_rRmDmxMMData.u4StartAddr >= _rRmDmxMMData.u4BufEnd)
        _rRmDmxMMData.u4StartAddr -= u4SrcBufLen;

    return TRUE;
}


static VOID vHandleRmRingBuf(VOID)
{
    UINT32 u4AddrDelta;

    if (_rSwdmxRmInfo.u4CurDmxFeederRPtr + _rSwdmxRmInfo.u4LeftBytesInFeeder > _rFeederInfo.u4EndAddr)
    {
        u4AddrDelta = _rFeederInfo.u4EndAddr - _rSwdmxRmInfo.u4CurDmxFeederRPtr;
        if (u4AddrDelta <= FEEDER_RESERVED_SIZE)
        {
            x_memcpy((VOID*)(_rFeederInfo.u4StartAddr - u4AddrDelta), (VOID*)(_rSwdmxRmInfo.u4CurDmxFeederRPtr), u4AddrDelta);
            _rSwdmxRmInfo.u4CurDmxFeederRPtr = _rFeederInfo.u4StartAddr - u4AddrDelta;
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

static INT32 _SetRmSpecInfo(VID_DEC_RM_SPEC_INFO_T* prSeqInfo)
{
    UINT32 u4NumRprSize = 0;
    VDEC_SEQUENCE_DATA_T rVDecSeqData;

    /*UNUSED(SWDMX_SetInfo(eSWDMX_SET_RANGECALLBACK, (UINT32)_PLAYMGR_UpdateRangeCb, 0, 0));*/

//    VDEC_PlayMM(ES0, TRUE);

    //DMX_SetVideoType(DMX_VIDEO_RV);
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
            _rSwdmxRmInfo.u1EccMask = (UINT8)0x20;
            /*SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER, 0x20, 0, 0);*/
        }
        else
        {
            _rSwdmxRmInfo.u1EccMask = (UINT8)0x80;
            /*SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER, 0x80, 0, 0);*/
        }
    }
    else
    {
        ASSERT(0);
        return FALSE;
    }

    VDEC_SetMMParam(ES0, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
    #if 0//def LINUX_TURNKEY_SOLUTION
    prSeqInfo->ui4_specific = (UINT32)addr_user_to_kernel((VOID *)prSeqInfo->ui4_specific);
    #endif
    VDEC_SetMMParam(ES0, VDEC_MM_RV_RPR_INFO, u4NumRprSize, (UINT32)(prSeqInfo->aui4_specific), !prSeqInfo->b_is_rm8);
    SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_RV, 0, 0);
    return TRUE;
}

BOOL _SWDMX_RmSetDecoderInfo(UINT8 u1Type,
    UINT32 u4Para1,
    UINT32 u4Para2,
    UINT32 u4Para3)
{
    if(u1Type == eSWDMX_SET_VIDEOTYPE)
    {
        _rSwdmxRmInfo.u4VDecType =  u4Para1;
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)  // tmp use it to pass ECC mask from Mutil
    {
        #if 0/*temp*/
        _rSwdmxRmInfo.u1EccMask = (UINT8)u4Para1;
        #endif
        _SetRmSpecInfo((VID_DEC_RM_SPEC_INFO_T*)u4Para1);
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        _rSwdmxRmInfo.u4ADecType = u4Para1;
        if (u4Para1 == ENUM_SWDMX_ADEC_AAC)
        {
            _rSwdmxRmInfo.u4AudSampleRate = u4Para2;
            _rSwdmxRmInfo.u4AudChannelNs = u4Para3;
            _i4RmPtsAdj = -27000;
            LOG(0, "RM audio is AAC\n");
        }
        else if (u4Para1 == ENUM_SWDMX_ADEC_RA)
        {
            x_memcpy((void *)&_rSwdmxRmInfo.rCookInfo, (void *)u4Para2, sizeof(COOK_INFO_T));
            compute_generic_interleave_pattern();
            _dwCurCookFrameInSB = 0;
            _dwCurAFifoOfst = 0;
            _i4RmPtsAdj = -9000;
            LOG(0, "RM audio is COOK\n");
#ifdef COOK_WRITE_AFIFO
            _fgAFifoForCookSBFull = FALSE;
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

BOOL _SWDMX_RmGetInfo(UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    //LOG(5, "_SWDMX_RmGetInfo u1Type = %d\n", u1Type);

    if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_RmGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx];
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if (pu4Para1) *pu4Para1 = FALSE;
        if (pu4Para2) *pu4Para2 = FALSE;
        if ( _rSwdmxRmInfo.fgEnVideo
            && (_rSwdmxRmInfo.prVidKeyIdxTbl)
            && (_rSwdmxRmInfo.prVidKeyIdxTbl->ui4_number_of_entry >= RM_KEY_TABLE_THRESHOLD)
            )
        {
            if (pu4Para1) *pu4Para1 = TRUE;
            if (pu4Para2) *pu4Para2 = TRUE;
        }
    }        
    return FALSE;
}

static BOOL _SWDMX_RmVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT8 u1Msg;
    INT32 i4Ret;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
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
        rPesInfo.u4FifoStart = NULL;
        rPesInfo.u4FifoEnd = NULL;
        rPesInfo.u4VldReadPtr = NULL;
        VDEC_SendEs((void*)&rPesInfo);
    }

  switch (_dwPrsState)
  {
  case RMPRS_FRAME:
    _dwPktSz -= _rFrameHdr.u4PartialFrameSize;
    if (_dwPktSz == 0)
    {
      //_u4PktNum--;

      if (_rFrameHdr.u4PacketNum == _rFrameHdr.u4NumPackets)  // the complete frame is ready
      {
        if (fgRmSliceSzBufChk())
        {
          vSendVFrame();
          vRmAddPic(&rPesInfo);

#if 0  // !!!
          if(_fgStartPlayUntilFrtSB == TRUE)
            _dMpsSkipDisp++;
#endif
        }
        else  // slice size buffer full, break Parser interrupt chain and resume it later
        {
          ASSERT(0);  // !!!
          //_dwPrsState = RMPRS_WAIT_SSZ_BUF_1;  !!!
          break;
        }
      }

      _dwPrsState = RMPRS_PACKET;
#if 0  // !!!
      _dwVWpBakForCookPrs = dPRVFifoWp();
      _dwLastVPktPrsState = _dwPrsState;
      _dwLastParsedVPktOfst = _dwFileOfst;
#endif
      //vMpvState();

      // wake up the SWDMX thread
      i4Ret = x_msg_q_send(_hRmIsrMsgQ, (VOID *)&u1Msg, sizeof(UINT8), 255);  //vMpsPrsNextG(0, 12, 0);
      if (i4Ret != OSR_OK)
      {
          LOG(0, "RM msg Q sent failed case %d!!!\n", __LINE__);
          ASSERT(0);
          while (1);
      }
    }
    else
    {
      ASSERT(_rFrameHdr.eType == RVFrameTypeLastPartial || _rFrameHdr.eType == RVFrameTypeMultiple);

      if (_rFrameHdr.u4PacketNum == _rFrameHdr.u4NumPackets)  // the complete frame is ready
      {
        if (fgRmSliceSzBufChk())
        {
          vSendVFrame();
          vRmAddPic(&rPesInfo);

#if 0  // !!!
          if(_fgStartPlayUntilFrtSB == TRUE)
            _dMpsSkipDisp++;
#endif  // !!!
        }
        else  // slice size buffer full, break Parser interrupt chain and resume it later
        {
          ASSERT(0);  // !!!
          //_dwPrsState = RMPRS_WAIT_SSZ_BUF_2;  !!!
          break;
        }
      }

      _dwPrsState = RMPRS_FRM_NEXT_MULTIPLE;
#if 0  // !!!
      _dwVWpBakForCookPrs = dPRVFifoWp();
      _dwLastVPktPrsState = _dwPrsState;
      _dwLastParsedVPktOfst = _dwFileOfst;
#endif
      //vMpvState();

      // wake up the SWDMX thread
      i4Ret = x_msg_q_send(_hRmIsrMsgQ, (VOID *)&u1Msg, sizeof(UINT8), 255);  //vMpsPrsNextG(0, 2, 0);
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
    i4Ret = x_msg_q_send(_hRmIsrMsgQ, (VOID *)&u1Msg, sizeof(UINT8), 255);  //vMpsPrsNextG(0, 2, 0);
    if (i4Ret != OSR_OK)
    {
        LOG(0, "RM msg Q sent failed case %d!!!\n", __LINE__);
        ASSERT(0);
        while (1);
    }
#endif
  }

#if 0
    if ((_rSwdmxRmInfo.u4VDecType == ENUM_SWDMX_VDEC_RV) &&
        !rPesInfo.fgEos)
    {
        UCHAR *pucBuf;

        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        if (pucBuf[0] == 0xB6)
        {
            u4Addr = prPes->u4FrameAddr + 4;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);
            switch (pucBuf[0] >> 6)
            {
                case 0:
                    rPesInfo.ucPicType  = I_TYPE;
                    break;
            }
        }
        else if ((pucBuf[0] >= 0x20) &&
            (pucBuf[0] <= 0x2f))
        {
            rPesInfo.fgSeqHdr = TRUE;
        }
    }

    VDEC_SendEs((void*)&rPesInfo);
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_RmInit(ENUM_SWDMX_FMT_T eType)
{
    INT32 i4Ret;

    UNUSED(eType);
    // Initialize feeder
    //FeederOpen(FEEDER_PROGRAM_SOURCE);

    //*********************************************************
    //FeederSetBufferSize(FEEDER_PROGRAM_SOURCE, 5*10*1024);
    //FeederStart(FEEDER_PROGRAM_SOURCE);
    FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE,
    _SWDMX_FeederCallBack, (UINT32)&_rFeederInfo);
    //*********************************************************

    //DMX_MM_DECODER_CALLBACKS_T rCallback; // Isaac: should be DMX_DECODER_CALLBACKS_T, Jack fix me
    DMX_DECODER_CALLBACKS_T rCallback;

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _SWDMX_RmVideoCallback;  //_MM_VideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;

    // Default some values
    _rSwdmxRmInfo.u4PrevAStrmID = 0xFFFFFFFF;
    _rSwdmxRmInfo.u4AudSampleRate = 44100;
    _rSwdmxRmInfo.u4AudChannelNs = 2;

    _rSwdmxRmInfo.fgBeginToPlay = TRUE;

    _rSwdmxRmInfo.u8SeekPts = 0xFFFFFFFF;
    _rSwdmxRmInfo.u8SeekPts <<= 32;
    _rSwdmxRmInfo.u8SeekPts |= 0xFFFFFFFF;
    _rSwdmxRmInfo.u8ASeekPts = 0xFFFFFFFF;
    _rSwdmxRmInfo.u8ASeekPts <<= 32;
    _rSwdmxRmInfo.u8ASeekPts |= 0xFFFFFFFF;
    _rSwdmxRmInfo.u8VSeekPts = 0xFFFFFFFF;
    _rSwdmxRmInfo.u8VSeekPts <<= 32;
    _rSwdmxRmInfo.u8VSeekPts |= 0xFFFFFFFF;

    _rSwdmxRmInfo.u4TriggerAudPts = 0xFFFFFFFF;

    _rSwdmxRmInfo.u4CurRepeatFrameIdx = 0xFFFFFFFF;

    _rSwdmxRmInfo.u4VStrmID = 0xFFFFFFFF;
    _rSwdmxRmInfo.u4AStrmID = 0xFFFFFFFF;

#ifdef RMLS
    _u4RmStrmIdx = RM_STRM_VDO;
    _fgRmLs = FALSE;
    _fgRmChgStrm = FALSE;
    x_memset(&_rSwdmxRmInfo.u4LsPts, 0, sizeof(_rSwdmxRmInfo.u4LsPts));
#endif

    _rSwdmxRmInfo.u8RecDmxMoviPos = 0;
    _rSwdmxRmInfo.u4ModeChangPts = 0xFFFFFFFF;
    _rSwdmxRmInfo.u4PullBackAudPts = 0xFFFFFFFF;

    x_memset(&_rPrevSample, 0, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

    //DDI_BUF_ALIGNMENT == 16 and
    //for _dmx_mm_movedata  Meet the alignment and packet size (188 bytes) requirements.
    if(_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC &&
        _pucAudBuf == NULL)
    {
        _pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(max(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        if(_pucAudBuf == NULL)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            return FALSE;
        }
    }
#ifndef COOK_WRITE_AFIFO
    else if(_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA &&
        _pucAudBuf == NULL)
    {
        _pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(max(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        if(_pucAudBuf == NULL)
        {
            LOG(1, "Allocate RA buffer failed!\n");
            ASSERT(0);
            return FALSE;
        }
    }
#endif

    DMX_SetDecoderCallbacks(&rCallback);
    DMX_SetToDecoder(TRUE);

    _rSwdmxRmInfo.fgFeederInvalid = TRUE;
    _rSwdmxRmInfo.fgRepeatRequest = TRUE;

    _rSwdmxRmInfo.fgCookPtsPending = FALSE;
    _rSwdmxRmInfo.fgCookSBPending = FALSE;

    _u4FrmNo = 0;
    if (_hRmIsrMsgQ != (HANDLE_T)0)  // ToDo: delete it in uninitialization (if any)
    {
        i4Ret = x_msg_q_delete(_hRmIsrMsgQ);
        if (i4Ret != OSR_OK)
        {
            LOG(0, "RM msg Q delete failed!!!!!!\n");
            while (1);
        }
        ASSERT(i4Ret == OSR_OK);
        _hRmIsrMsgQ = (HANDLE_T)0;
    }
    if (_hRmIsrMsgQ == (HANDLE_T)0)
    {
        i4Ret = x_msg_q_create(&_hRmIsrMsgQ, "RMISRQ", sizeof(UINT8), 1);
        if (i4Ret != OSR_OK)
        {
            LOG(0, "RM msg Q create failed!!!!!!\n");
            while (1);
        }
        ASSERT(i4Ret == OSR_OK);
    }
    _dwSliceSzBufWp = 0;
    _dwSliceSzBufRp = 0;

#ifdef __MODEL_slt__
    _u4RmSltLastPicPts = 0;
    _rSwdmxRmInfo.u1EccMask = 0x80;
#endif

    return TRUE;
}


/**
*/
VOID _SWDMX_RmSetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    UINT8 u1ATrack;

    if (prRangeList == NULL)
    {
        ASSERT(0);
        LOG(1, "Demuxer Range is NULL\n");
        return;
    }

    _rSwdmxRmInfo.prDmxRangeInfo = prRangeList->prDmxRangeInfo;

    if (prRangeList != NULL &&
       prRangeList->prDmxRangeInfo != NULL)
    {
        u1ATrack = 0;
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl =
        (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                u1ATrack < RM_MAX_AUDIO_TRACK)
            {
                _rSwdmxRmInfo.prAudIdxTbl[u1ATrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_RmSetRange Audio Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxRmInfo.prAudIdxTbl[u1ATrack]));
                u1ATrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
            {
                _rSwdmxRmInfo.prVidIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_RmSetRange Video Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxRmInfo.prVidIdxTbl));
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                _rSwdmxRmInfo.prVidKeyIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_RmSetRange Key Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxRmInfo.prVidKeyIdxTbl));
            }

            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
        }

#ifndef RMLS
       _rSwdmxRmInfo.au8FileOffset[RM_STRM_VDO] = prRangeList->prDmxRangeInfo->ui8_fileoffset;
       _rSwdmxRmInfo.au8RangeSz[RM_STRM_VDO]  = prRangeList->prDmxRangeInfo->z_range_sz;
        LOG(5, "ofst/range = %lld/%lld\n",
            _rSwdmxRmInfo.au8FileOffset[RM_STRM_VDO],
            _rSwdmxRmInfo.au8RangeSz[RM_STRM_VDO]);
#else
        if (prRangeList->prDmxRangeInfo->z_range_sz2[RM_STRM_AUD] != 0)
        {
            _fgRmLs = TRUE;
#if 0
            if (_rSwdmxRmInfo.fgEnAudio)
            {
                _u4RmStrmIdx = RM_STRM_AUD;
            }
#endif
            _rSwdmxRmInfo.au8FileOffset[RM_STRM_VDO] = prRangeList->prDmxRangeInfo->ui8_fileoffset2[RM_STRM_VDO];
            _rSwdmxRmInfo.au8RangeSz[RM_STRM_VDO]  = prRangeList->prDmxRangeInfo->z_range_sz2[RM_STRM_VDO];
            _rSwdmxRmInfo.au8FileOffset[RM_STRM_AUD] = prRangeList->prDmxRangeInfo->ui8_fileoffset2[RM_STRM_AUD];
            _rSwdmxRmInfo.au8RangeSz[RM_STRM_AUD]  = prRangeList->prDmxRangeInfo->z_range_sz2[RM_STRM_AUD];
            LOG(3, "This is logical stream!!\n");
            LOG(5, "ofst/range(%d) = %lld/%lld\n",
                RM_STRM_VDO,
                _rSwdmxRmInfo.au8FileOffset[RM_STRM_VDO],
                _rSwdmxRmInfo.au8RangeSz[RM_STRM_VDO]);
            LOG(5, "ofst/range(%d) = %lld/%lld\n",
                RM_STRM_AUD,
                _rSwdmxRmInfo.au8FileOffset[RM_STRM_AUD],
                _rSwdmxRmInfo.au8RangeSz[RM_STRM_AUD]);
        }
        else
        {
            _fgRmLs = FALSE;
           _rSwdmxRmInfo.au8FileOffset[RM_STRM_VDO] = prRangeList->prDmxRangeInfo->ui8_fileoffset;
           _rSwdmxRmInfo.au8RangeSz[RM_STRM_VDO]  = prRangeList->prDmxRangeInfo->z_range_sz;
            LOG(5, "ofst/range = %lld/%lld\n",
                _rSwdmxRmInfo.au8FileOffset[RM_STRM_VDO],
                _rSwdmxRmInfo.au8RangeSz[RM_STRM_VDO]);
        }
#endif

        if (_rSwdmxRmInfo.prVidIdxTbl)
        {
            // The upper layer (mw or playmgr should prevent 0 case for rate and scale)
            if (_rSwdmxRmInfo.prVidIdxTbl->ui4_scale)
            {
                _rSwdmxRmInfo.u4VidFps =
                    (UINT32)(_rSwdmxRmInfo.prVidIdxTbl->ui4_rate/_rSwdmxRmInfo.prVidIdxTbl->ui4_scale);
            }

            //_rSwdmxRmInfo.u4VidFps = _u4RVFps;
            _rSwdmxRmInfo.u4VidFps = 30;

            // Add some error handling
            if (!_rSwdmxRmInfo.u4VidFps ||
                _rSwdmxRmInfo.u4VidFps > 60)
            {
                _rSwdmxRmInfo.u4VidFps = 30;
            }

            LOG(5, "_rSwdmxRmInfo.u4VidFps = %ld\n", _rSwdmxRmInfo.u4VidFps);
        }

        // Set the number of total audio tracks
        if (_rSwdmxRmInfo.u1TotalATrack == 0)  // tmp solution for MW not setting this
            _rSwdmxRmInfo.u1TotalATrack = u1ATrack;

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
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange
)
{
    UNUSED(i4PlaySpeed);
    UNUSED(fgSpeedChange);

    UINT32 u4CurPts = 0;
    UINT32 u4PrsState;

    //UINT64 u8Numerator = 0, u8Denominator = 0, u8Remainder = 0;

    VDEC_PTS_INFO_T rPTSInfo;

    x_memset((void*)&rPTSInfo, 0, sizeof(VDEC_PTS_INFO_T));

    //if (fgSpeedChange)
    //    printf("swdmx_rm 1187\n");

    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if (_rSwdmxRmInfo.fgBeginToPlay &&
        _rSwdmxRmInfo.prDmxRangeInfo->t_vid_start_render_pts != 0)
    {
        //UINT64 u8PtsDiff = 0;
#if defined(CC_DLNA_SUPPORT) && 0
        UINT32 u4FeederReadSize;

        // For DLNA playback
        u4FeederReadSize = (_rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] < RM_PREBUFFER_SIZE) ?
                                        (_rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx]) : (RM_PREBUFFER_SIZE);

        LOG(5, "Feeder's ready bound = %d, underflw = %d",  u4FeederReadSize, u4FeederReadSize/8);

        FeederSetReadyThreshold(u4FeederReadSize);
        FeederSetUnderFlowThreshold(u4FeederReadSize/8);
#endif
        if(_rSwdmxRmInfo.prVidIdxTbl)
        {
#if 1  // !!!
            rPTSInfo.u8Scale = (UINT64)_rSwdmxRmInfo.prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
            rPTSInfo.u4Rate = _rSwdmxRmInfo.prVidIdxTbl->ui4_rate;
#else
            if (_rSwdmxRmInfo.prVidIdxTbl->ui4_scale &&
                _rSwdmxRmInfo.prVidIdxTbl->ui4_rate)
            {
                u8Numerator =
                    (UINT64)_rSwdmxRmInfo.prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
                u8Denominator =
                    (UINT64)_rSwdmxRmInfo.prVidIdxTbl->ui4_rate;
                u8PtsDiff = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
            }
            else
            {
                u8PtsDiff = 1;
            }
            _rSwdmxRmInfo.u8PtsDiff = u8PtsDiff;
#endif
        }

        UNUSED(_SWDMX_RmSeek(_rSwdmxRmInfo.prDmxRangeInfo->t_vid_start_render_pts, prRangeList, FALSE));
        LOG(5, "Video Start Render Pts = %lld\n",
            _rSwdmxRmInfo.prDmxRangeInfo->t_vid_start_render_pts);

        _rSwdmxRmInfo.u8VSeekPts =
            _rSwdmxRmInfo.prDmxRangeInfo->t_vid_start_render_pts;
        _rSwdmxRmInfo.u8ASeekPts =
            _rSwdmxRmInfo.prDmxRangeInfo->t_vid_start_render_pts;

        _rSwdmxRmInfo.fgIsSeek = TRUE;

        if (_rSwdmxRmInfo.fgEnAudio && _rSwdmxRmInfo.fgEnVideo)
        {
            VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);
            VDEC_SetRenderFromPts(ES0, _rSwdmxRmInfo.prDmxRangeInfo->t_vid_start_render_pts);

            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxRmInfo.fgVDecIsPause = TRUE;
        }
#if 0  // !!!
        // audio only
        else if (_rSwdmxRmInfo.fgEnAudio)
        {
            if (_rSwdmxRmInfo.u1CurATrack < RM_MAX_AUDIO_TRACK)
            {
                AUD_SetStartPts(0, _rSwdmxRmInfo.u4AudPts[_rSwdmxRmInfo.u1CurATrack]);
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                AUD_SetStartPts(0, 0);
            }
        }
        // video only
        else if (_rSwdmxRmInfo.fgEnVideo)
        {
            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxRmInfo.fgVDecIsPause = TRUE;

            VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);  //!!!
            VDEC_SetRenderFromPts(ES0, _rSwdmxRmInfo.u4CurVidPts);
        }
#endif  // 0
        _rSwdmxRmInfo.fgBeginToPlay = FALSE;
    }

    if (_rSwdmxRmInfo.fgBeginToPlay &&
        i4PlaySpeed == 1000)
    {
        //UINT64 u8PtsDiff = 0;
#if defined(CC_DLNA_SUPPORT) && 0
        UINT32 u4FeederReadSize;

        // For DLNA playback
        u4FeederReadSize = (_rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] < RM_PREBUFFER_SIZE) ?
                                        (_rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx]) : (RM_PREBUFFER_SIZE);

        LOG(5, "Feeder's ready bound = %d, underflw = %d",  u4FeederReadSize, u4FeederReadSize/8);

        FeederSetReadyThreshold(u4FeederReadSize);
        FeederSetUnderFlowThreshold(u4FeederReadSize/8);
#endif
        if(_rSwdmxRmInfo.prVidIdxTbl)
        {
#if 1  // !!!
            rPTSInfo.u8Scale = (UINT64)_rSwdmxRmInfo.prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
            rPTSInfo.u4Rate = _rSwdmxRmInfo.prVidIdxTbl->ui4_rate;
#else
            if (_rSwdmxRmInfo.prVidIdxTbl->ui4_scale &&
                _rSwdmxRmInfo.prVidIdxTbl->ui4_rate)
            {
                u8Numerator =
                    (UINT64)_rSwdmxRmInfo.prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
                u8Denominator =
                    (UINT64)_rSwdmxRmInfo.prVidIdxTbl->ui4_rate;
                u8PtsDiff = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
            }
            else
            {
                u8PtsDiff = 1;
            }
            _rSwdmxRmInfo.u8PtsDiff = u8PtsDiff;
#endif
        }

        if (_rSwdmxRmInfo.fgEnAudio && _rSwdmxRmInfo.fgEnVideo)
        {
            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxRmInfo.fgVDecIsPause = TRUE;

            if (_rSwdmxRmInfo.u1CurATrack < RM_MAX_AUDIO_TRACK)
            {
#if 1
                _rSwdmxRmInfo.u4TriggerAudPts = 0;
                AUD_SetStartPts(0, _rSwdmxRmInfo.u4TriggerAudPts);
#else
                AUD_SetStartPts(0, _rSwdmxRmInfo.u4AudPts[_rSwdmxRmInfo.u1CurATrack]);
                _rSwdmxRmInfo.u4TriggerAudPts =
                    _rSwdmxRmInfo.u4AudPts[_rSwdmxRmInfo.u1CurATrack];
#endif
            }

            VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);  //!!!
            VDEC_SetRenderFromPts(ES0, _rSwdmxRmInfo.u4CurVidPts);
        }
        // audio only
        else if (_rSwdmxRmInfo.fgEnAudio)
        {
            if (_rSwdmxRmInfo.u1CurATrack < RM_MAX_AUDIO_TRACK)
            {
                AUD_SetStartPts(0, _rSwdmxRmInfo.u4AudPts[_rSwdmxRmInfo.u1CurATrack]);
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                AUD_SetStartPts(0, 0);
            }
        }
        // video only
        else if (_rSwdmxRmInfo.fgEnVideo)
        {
            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxRmInfo.fgVDecIsPause = TRUE;

            VDEC_StartPTS(0, TRUE, 0, &rPTSInfo);  //!!!
            VDEC_SetRenderFromPts(ES0, _rSwdmxRmInfo.u4CurVidPts);
        }

        // set stc for starting playback
        STC_StopStc();
        STC_SetStcValue(_rSwdmxRmInfo.u4CurVidPts - 45000);

        _rSwdmxRmInfo.fgBeginToPlay = FALSE;
    }

    // Reset some values for state change.
    if (_rSwdmxRmInfo.fgChgATrack ||
        fgSpeedChange)
    {
        if (_rSwdmxRmInfo.fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (_rSwdmxRmInfo.u4VDecType)
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
            if (!DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }

            _rSwdmxRmInfo.fgDeliverVEos = FALSE;
        }
        if (_rSwdmxRmInfo.fgDeliverAEos)
        {
            _rSwdmxRmInfo.fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(0);
        }

#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
        _rSwdmxRmInfo.fgAudIsReady = FALSE;

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    if (fgSpeedChange)
    {
        _rSwdmxRmInfo.fgSpeedChange = TRUE;

        u4CurPts = VDP_GetPts(0);
        LOG(5, "Speed change while VDP Pts = 0x%08x\n", u4CurPts);

        _rSwdmxRmInfo.u4NumSentAud = 0;

        // 1X -> 2X
        if ((_rSwdmxRmInfo.i4PrevSpeed == 1000) &&
            (i4PlaySpeed == 2000))
        {
            if (_rSwdmxRmInfo.u8RecDmxMoviPos != 0)
            {
                _SWDMX_FlushReqMsgQ();
                _rSwdmxRmInfo.i4ReqReadTimes = 0;
                _rSwdmxRmInfo.u4LeftBytesInFeeder = 0;
                _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] = _rSwdmxRmInfo.u8RecDmxMoviPos;
            }
        }

        _rSwdmxRmInfo.u8RecDmxMoviPos = 0;
        _rSwdmxRmInfo.u4ModeChangPts = 0xFFFFFFFF;
        _rSwdmxRmInfo.u4ChangeMode = 0;

        _rSwdmxRmInfo.u4VidRepeatFrame = 0;
        _rSwdmxRmInfo.fgRequestData = TRUE;

        _rSwdmxRmInfo.u8VSeekPts = 0xFFFFFFFF;
        _rSwdmxRmInfo.u8VSeekPts <<= 32;
        _rSwdmxRmInfo.u8VSeekPts += 0xFFFFFFFF;

        _rSwdmxRmInfo.u8ASeekPts = 0xFFFFFFFF;
        _rSwdmxRmInfo.u8ASeekPts <<= 32;
        _rSwdmxRmInfo.u8ASeekPts += 0xFFFFFFFF;

        if(_rSwdmxRmInfo.fgVDecIsPause)
        {
            _SWDMX_RmVideoWakeUp();
            _rSwdmxRmInfo.fgVDecIsPause = FALSE;
        }

        if(_rSwdmxRmInfo.prVidIdxTbl)
        {
#if 1  // !!!
            rPTSInfo.u8Scale = (UINT64)_rSwdmxRmInfo.prVidIdxTbl->ui4_scale * DEMUX_RM_AV_PERIOD;
            rPTSInfo.u4Rate = _rSwdmxRmInfo.prVidIdxTbl->ui4_rate;
#endif
        }
        // seek
        if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF)
            || ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
        {
            UNUSED(_SWDMX_RmSeek(u8SeekPos, prRangeList, FALSE));
            LOG(5, "u8SeekPos = %lld\n", u8SeekPos);

            _rSwdmxRmInfo.u8VSeekPts = u8SeekPos;
            _rSwdmxRmInfo.u8ASeekPts = u8SeekPos;

            MPV_FlushEsmQ(0, TRUE);
            VDEC_ReleaseDispQ(0);

            if(i4PlaySpeed >0)
            {
                VDEC_SetRenderPts(ES0, u8SeekPos);
            }
            else
            {
                VDEC_SetRenderPts(ES0, 0);
            }

            VDEC_StartPTS(ES0, TRUE, u8SeekPos, NULL);  !!!
#if 0  // this will cause video freeze after time search during FR
            //VDEC_SetRenderFromPts(0, u8SeekPos);  // !!!
#endif

            DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

            // set stc
            STC_StopStc();
            STC_SetStcValue(u8SeekPos);
        }
        // 2X -> 1X, Step -> 1X, Slow Forward -> 1X
        else if ((_rSwdmxRmInfo.i4PrevSpeed == 2000 ||
                  _rSwdmxRmInfo.i4PrevSpeed == 1 ||
                  IS_SLOW_FWD(_rSwdmxRmInfo.i4PrevSpeed))
                 &&
                 i4PlaySpeed == 1000)
        {
            UINT8 u1TrackIdx = _rSwdmxRmInfo.u1CurATrack;
            UINT32 u4AVPtsDiff  = 0;

            u4CurPts = VDP_GetPts(0) - _i4RmPtsAdj;

            if ((_rSwdmxRmInfo.i4PrevSpeed == 1) ||
                ((_rSwdmxRmInfo.i4PrevSpeed < 1000) && (_rSwdmxRmInfo.i4PrevSpeed > 1)))
            {
                STC_StartStc();
            }

            if (u1TrackIdx < RM_MAX_AUDIO_TRACK)
            {
                LOG(5, "RM Speed 2X -> 1X Cur Vid Pts = 0x%08x, Cur Aud Pts = 0x%08x\n",
                _rSwdmxRmInfo.u4CurVidPts, _rSwdmxRmInfo.u4PullBackAudPts);

                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

                if (_rSwdmxRmInfo.u4PullBackAudPts > u4CurPts)
                {
                    u4AVPtsDiff = _rSwdmxRmInfo.u4PullBackAudPts - u4CurPts;
                }

                if (u4AVPtsDiff >= 45000)
                {
                    LOG(5, "u4AVPtsDiff > 45000\n");

                    if (_rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] > _rSwdmxRmInfo.u8RecDmxMoviPos)
                    {
                        _rSwdmxRmInfo.u8RecDmxMoviPos = _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx];
                    }

                    if (_SWDMX_RmSeek(u4CurPts, prRangeList, FALSE))
                    {
#if 1  // !!!
                        //_rSwdmxRmInfo.u4ModeChangPts = u4CurPts + 27000;
                        _rSwdmxRmInfo.u4ChangeMode |= RM_CHANGE_AUDIO_MODE;
#endif
                    }
                    else
                    {
                        ASSERT(0);  // !!!
                        _rSwdmxRmInfo.u8RecDmxMoviPos = 0;
                        _rSwdmxRmInfo.u4TriggerAudPts = _rSwdmxRmInfo.u4PullBackAudPts;
                        AUD_SetStartPts(0, _rSwdmxRmInfo.u4PullBackAudPts);
                    }
                }
                else
                {
                    _rSwdmxRmInfo.u4TriggerAudPts = _rSwdmxRmInfo.u4PullBackAudPts;
                    AUD_SetStartPts(0, _rSwdmxRmInfo.u4PullBackAudPts);
                }

                LOG(5, "Rec Movi Pos = %ld\n",
                    _rSwdmxRmInfo.u8RecDmxMoviPos);
                LOG(5, "Cur Dmx Pos = %ld, \n",
                    _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx]);
            }
            else
            {
                LOG(1, "Out of Max Audio Track Number\n");
            }
        }
        else if (_rSwdmxRmInfo.i4PrevSpeed < 0 &&
            (i4PlaySpeed == 2000 || i4PlaySpeed == 1000))
        {
            UNUSED(_SWDMX_RmSeek(u4CurPts, prRangeList, FALSE));

            MPV_FlushEsmQ(0, TRUE);
            VDEC_ReleaseDispQ(0);

            DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

            _SWDMX_FlushReqMsgQ();
            _rSwdmxRmInfo.i4ReqReadTimes = 0;
            _rSwdmxRmInfo.fgReqDataMiss = FALSE;

            VDEC_SetRenderFromPts(0,
                _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90);

            if ((i4PlaySpeed == 1000) &&
                (_rSwdmxRmInfo.u1CurATrack < RM_MAX_AUDIO_TRACK))
            {
                if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA)
                {
                    _rSwdmxRmInfo.fgCookPtsPending = TRUE;
                }
                else
                {
                    _rSwdmxRmInfo.u4TriggerAudPts = _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90;  // !!! _rSwdmxRmInfo.u4AudPts[_rSwdmxRmInfo.u1CurATrack];

                    LOG(5, "VDP trigger audio = 0x%08x\n",
                        _rSwdmxRmInfo.u4TriggerAudPts);

                    AUD_SetStartPts(0, _rSwdmxRmInfo.u4TriggerAudPts);
                }
#if 1  // !!!
                VDEC_StartPTS(0, TRUE,
                    _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90,
                    &rPTSInfo);
#endif

                // set stc
                STC_StopStc();
                STC_SetStcValue(u4CurPts);
            }

            LOG(5, "RM Speed -32X/-16X/-8X/-4X/-2X -> 1X\n");
        }
        else if (!(_rSwdmxRmInfo.i4PrevSpeed == 1000 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1)))
        {
            if (!IS_SLOW_FWD(i4PlaySpeed))
            {
#if 1  // ToDo: water-proof it
                if (_rSwdmxRmInfo.i4PrevSpeed == 2000 && i4PlaySpeed >= 4000)
                {
                    UNUSED(_SWDMX_RmSeek(u4CurPts, prRangeList, TRUE));
                }
                else
#endif
                {
                    UNUSED(_SWDMX_RmSeek(u4CurPts, prRangeList, FALSE));
                }

                MPV_FlushEsmQ(0, TRUE);
                VDEC_ReleaseDispQ(0);

                DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);

                _SWDMX_FlushReqMsgQ();
                _rSwdmxRmInfo.i4ReqReadTimes = 0;
                _rSwdmxRmInfo.fgReqDataMiss = FALSE;

                // Set render pts
                if (i4PlaySpeed > 1000)
                {
                    VDEC_SetRenderFromPts(0,
                        _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90);
                    //avc test
                    VDEC_StartPTS(0, FALSE,
                        _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90,
                        /*(UINT32)_rSwdmxRmInfo.u8PtsDiff*/&rPTSInfo);
                }
                else if (i4PlaySpeed <= 1000 && i4PlaySpeed > 0)
                {
#if 1  // ToDo: water-proof it
                    if (_rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90 != u4CurPts)
                    {
                        LOG(0, "Key PTS != Cur PTS, %d vs %d!!!!!!!\n", _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90, u4CurPts);
                    }
#else
                    ASSERT(_rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90 == u4CurPts);
#endif
                    VDEC_SetRenderFromPts(0, _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90/*u4CurPts !!!*/);
                    //avc test
                    VDEC_StartPTS(0, TRUE,
                        _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90,
                        /*(UINT32)_rSwdmxRmInfo.u8PtsDiff*/&rPTSInfo);
                }
                else
                {
                    VDEC_SetRenderFromPts(0, _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts * 90);
                    VDEC_StartPTS(0, FALSE,
                        _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts * 90,
                        /*(UINT32)_rSwdmxRmInfo.u8PtsDiff*/&rPTSInfo);
                }

                if (i4PlaySpeed > 2000 || i4PlaySpeed < 0)
                {
                    _rSwdmxRmInfo.fgStartTrick = TRUE;
                }

                // Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
                if ((_rSwdmxRmInfo.i4PrevSpeed > 2000 || _rSwdmxRmInfo.i4PrevSpeed < 0) &&
                    (i4PlaySpeed == 1000) &&
                    (_rSwdmxRmInfo.u1CurATrack < RM_MAX_AUDIO_TRACK))
                {
                    if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA)
                    {
                        _rSwdmxRmInfo.fgCookPtsPending = TRUE;
                    }
                    else
                    {
                        _rSwdmxRmInfo.u4TriggerAudPts = _rSwdmxRmInfo.prCurVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90;  // !!! _rSwdmxRmInfo.u4AudPts[u1TrackIdx];
                        AUD_SetStartPts(0, _rSwdmxRmInfo.u4TriggerAudPts);
                    }

                    // set stc
                    STC_StopStc();
                    STC_SetStcValue(u4CurPts);
                }
                LOG(5, "_rSwdmxRmInfo.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
                // don't need to handle 1X->2X
            }  // !IS_SLOW_FWD(i4PlaySpeed)
        }
    }

    if ((_rSwdmxRmInfo.prCurVidKeyIdxTbl != 0) &&
        ((i4PlaySpeed > 2000) || (i4PlaySpeed < 0)))
    {
        u4PrsState = _SWDMX_RmHdrPrsUseIdx(i4PlaySpeed, prRangeList);
    }
    else if ((_rSwdmxRmInfo.prCurVidKeyIdxTbl == 0) &&
        ((i4PlaySpeed > 2000) || (i4PlaySpeed < 0)))
    {
        return eSWDMX_HDR_PRS_FAIL;
    }
    else
    {
        u4PrsState = _SWDMX_RmHdrPrsWithoutIdx(prRangeList,
                                                 i4PlaySpeed,
                                                 u8SeekPos,
                                                 fgSpeedChange);
    }

#ifdef RMLS
    if (_fgRmLs && _fgRmChgStrm)
    {
        ASSERT(_rSwdmxRmInfo.fgEnAudio);
        _fgRmChgStrm = FALSE;
        if (RM_STRM_VDO == _u4RmStrmIdx)
        {
            _u4RmStrmIdx = RM_STRM_AUD;
        }
        else if (RM_STRM_AUD == _u4RmStrmIdx)
        {
            _u4RmStrmIdx = RM_STRM_VDO;
        }
        else
        {
            RM_DBG_LOG("Invalid _u4RmStrmIdx: %d\n", _u4RmStrmIdx);
            ASSERT(0);
        }
        RM_DBG_LOG("Chg _u4RmStrmIdx: %d\n", _u4RmStrmIdx);
        _rSwdmxRmInfo.fgFeederInvalid = TRUE;
    }
#endif

    _rSwdmxRmInfo.i4PrevSpeed = i4PlaySpeed;
    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}

static BOOL _SWDMX_TxAACFrame(UINT16 u2FrameSize)
{
    UINT8 u1SampleRateIdx;
    DMX_MM_DATA_T rDmxMMData;

    if(_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC &&
        _pucAudBuf == NULL)
    {
        _pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(max(AAC_ALLOC_SIZE, RA_ALLOC_SIZE), 16));
        if(_pucAudBuf == NULL)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            return FALSE;
        }
    }

    u2FrameSize += AAC_FAKE_HDR_SIZE;
    x_memset(&rDmxMMData, 0, sizeof(rDmxMMData));

    /* 8b: syncword */
    _pucAudBuf[0] = 0xFF;
    /* 4b: syncword */
    /* 1b: mpeg id = 1 */
    /* 2b: layer = 0 */
    /* 1b: protection absent */
    _pucAudBuf[1] = 0xF9;
    /* 2b: profile */
    /* 4b: sampling_frequency_index */
    /* 1b: private = 0 */
    /* 1b: channel_configuration */
    switch (_rSwdmxRmInfo.u4AudSampleRate)
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

    _pucAudBuf[2] = (1 << 6) |
             ((u1SampleRateIdx<< 2) & 0x3C) |
             ((_rSwdmxRmInfo.u4AudChannelNs >> 2) & 0x1);
    /* 2b: channel_configuration */
    /* 1b: original */
    /* 1b: home */
    /* 1b: copyright_id */
    /* 1b: copyright_id_start */
    /* 2b: aac_frame_length */
    _pucAudBuf[3] = ((_rSwdmxRmInfo.u4AudChannelNs & 0x3) << 6) |
             ((u2FrameSize >> 11) & 0x3);
    /* 8b: aac_frame_length */
    _pucAudBuf[4] = ((u2FrameSize >> 3) & 0xFF);
    /* 3b: aac_frame_length */
    /* 5b: adts_buffer_fullness */
    _pucAudBuf[5] = ((u2FrameSize << 5) & 0xE0) |
             ((0x7FF >> 6) & 0x1F);
    /* 6b: adts_buffer_fullness */
    /* 2b: number_of_raw_data_blocks_in_frame */
    _pucAudBuf[6] = ((0x7FF << 2) & 0xFC);

    rDmxMMData.u4BufStart = (UINT32)_pucAudBuf;
    rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + AAC_ALLOC_SIZE;
    rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
    rDmxMMData.u4FrameSize = 7;
    rDmxMMData.u4Pts = _rRmDmxMMData.u4Pts;  //0;   !!!

    if (DMX_MM_MoveData(eSWDMX_MEDIA_AUDIO, &rDmxMMData, 500) != TRUE)
    {
        LOG(0, "Demuxer fails to move data.\n");
        return FALSE;
    } 

    return TRUE;
}

static BOOL _CheckRmPacketType(UCHAR *pucBuf, UINT8 *pu1HdrType, UINT8 *pu1TrackIdx, BOOL *pfgVer1, UINT32 *pu4Timestamp, UINT16 *pu2Flag, UINT32 *pu4PktSize)
{
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

    if (_CheckRmPacketIndex(pucBuf, _rSwdmxRmInfo.u4VStrmID, pfgVer1) == TRUE)
    {
        if (_rSwdmxRmInfo.fgEnVideo)
            *pu1HdrType = eSWDMX_MEDIA_VIDEO;
        else
            *pu1HdrType = eSWDMX_MEDIA_SKIPPED;

        goto RM_CHK_PKT_TYPE_RTN_TRUE;
    }

    if (_rSwdmxRmInfo.u4VStrmID < 3)
        fgStrmIDChecked[_rSwdmxRmInfo.u4VStrmID] = TRUE;

    for (i = 0; (i < _rSwdmxRmInfo.u1TotalATrack) && (i < RM_MAX_AUDIO_TRACK); i++)
    {
        if (_CheckRmPacketIndex(pucBuf, _rSwdmxRmInfo.u4AStrmID, pfgVer1) == TRUE)
        {
            *pu1HdrType = eSWDMX_MEDIA_AUDIO;
            goto RM_CHK_PKT_TYPE_RTN_TRUE;
        }
    }

    if (_rSwdmxRmInfo.u4AStrmID < 3)
        fgStrmIDChecked[_rSwdmxRmInfo.u4AStrmID] = TRUE;

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
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd,
    BOOL fgOneVideoFrm)
{
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

    //DMX_MM_DATA_T _rRmDmxMMData;  megaa 20090601: change it to global, tmp solution

    UINT32 u4OldNumSentVid = _rSwdmxRmInfo.u4NumSentVid;

    UINT8 bTmpVal;

    x_memset(&_rRmDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_rSwdmxRmInfo.fgCookSBPending)
    {
        vCookFrameDeInterleave(NULL);  // this will clear fgCookSBPending or not depending on the current A FIFO condition
#ifdef RMLS
        if (_fgRmChgStrm)
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
        if (((pucBitstream + i) >= (UCHAR*)(VIRTUAL(_rFeederInfo.u4EndAddr - RM_PARSER_OFFSET))) &&
            ((u4AvailSize - i) >= RM_PARSER_OFFSET) && (!fgRingBuf))
        {
            fgRingBuf = TRUE;
            u4RingSize = (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) - (UINT32)(pucBitstream + i);
            //u4OrgRingSize = u4RingSize;

            LOG(0, "Ring Buffer start from 0x%08x to 0x%08x size = 0x%08x\n", pucBitstream,
                VIRTUAL(_rFeederInfo.u4EndAddr), u4RingSize);

            x_memcpy(ucRingBuf, (pucBitstream + i), u4RingSize);

            u4AvailSize -= (u4RingSize + i);
            pucBitstream = (UCHAR*)(VIRTUAL(_rFeederInfo.u4StartAddr));
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

            if (_CheckRmPacketType(ucTempBuf, &u1HdrType, &u1TrackIdx, &fgVer1, &u4Pts, &u2Flag, &u4PacketSize) == TRUE)
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

        if (_CheckRmPacketType(pucBitstream + i, &u1HdrType, &u1TrackIdx, &fgVer1, &u4Pts, &u2Flag, &u4PacketSize) == TRUE)
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
            _rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET + i);
        }
        else
        {
            _rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET - 1 + i);
        }
    }
    else
    {
        ASSERT(0);  // !!! verify the following code
        if (fgVer1)
        {
            _rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET + i);
        }
        else
        {
            _rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PARSER_OFFSET - 1 + i);
        }
        //pucTemp = pucBitstream + RM_PACKET_ID_OFFSET + i;  // only to shut down Lint warning
#if 0
        if (u4RingSize == 0)
        {
            //pucTemp = pucBitstream;
            _rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + RM_PACKET_ID_OFFSET);
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

            _rRmDmxMMData.u4StartAddr = (UINT32)(pucBitstream + (RM_PARSER_OFFSET - u4RingSize));
        }
#endif
    }

    if (_rRmDmxMMData.u4StartAddr >= (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)))
        ASSERT(0);

#if 1  // !!!
    if (_rSwdmxRmInfo.u8RecDmxMoviPos)
    {
        LOG(7, "CurDmxMoviPos when mode Change = %ld\n", _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx]);
        if (_rSwdmxRmInfo.u8RecDmxMoviPos != _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx])
        {
            // Find the first audio chunk since 2X -> 1X
            if (u1HdrType == eSWDMX_MEDIA_AUDIO)
            {
              if (_rSwdmxRmInfo.u4ChangeMode & RM_CHANGE_AUDIO_MODE)
            {
                UINT8 u1ATrack = _rSwdmxRmInfo.u1CurATrack;
                UINT32 u4ModeChangePts = VDP_GetPts(0);

                if (u1ATrack < RM_MAX_AUDIO_TRACK)
                {
                    if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA && !(bTmpVal & HX_KEYFRAME_FLAG))  // COOK case
                    {
                        _rSwdmxRmInfo.u4PullBackAudPts = u4Pts * 90;  // record the PTS for later usage
                        u1HdrType = eSWDMX_MEDIA_SKIPPED;
                    }
                    else if (u4Pts * 90 >= u4ModeChangePts - _i4RmPtsAdj)  // AAC case or COOK with key frame flag
                    {
                        LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n",
                            u4Pts * 90,
                            u4ModeChangePts);
                        //VDP_TriggerAudReceive(_rSwdmxRmInfo.u4AudPts[u1ATrack]);
                        _rSwdmxRmInfo.u4TriggerAudPts = u4Pts * 90;
                        AUD_SetStartPts(0, _rSwdmxRmInfo.u4TriggerAudPts);
                        _rSwdmxRmInfo.u4ChangeMode = 0;
                        _rSwdmxRmInfo.u4ModeChangPts = 0xFFFFFFFF;
                        _rSwdmxRmInfo.u4PullBackAudPts = 0xFFFFFFFF;
                    }
#if 1
                    else
#else
                    else if ((u4Pts * 90 < _rSwdmxRmInfo.u4ModeChangPts) &&
                        (_rSwdmxRmInfo.u4ModeChangPts != 0xFFFFFFFF))
#endif
                    {
                        LOG(5, "Audio Pts (0x%08x) < Mode ChangePts (0x%08x)\n",
                            u4Pts * 90,
                            u4ModeChangePts);

                        //_SWDMX_RmCalculatePts(u1HdrType, u1ATrack, u4PacketSize);  !!!
                        _rSwdmxRmInfo.u4PullBackAudPts = u4Pts * 90;  // record the PTS for later usage
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

                if (_rSwdmxRmInfo.u8RecDmxMoviPos < _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] + u4PacketSize)
                {
                    LOG(5, "_rSwdmxRmInfo.u8RecDmxMoviPos < _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] + %d\n", u4PacketSize);
                    _rSwdmxRmInfo.u8RecDmxMoviPos = 0;
                    _rSwdmxRmInfo.u4ChangeMode = 0;
                }

            }
        }
        else
        {
            LOG(5, "_rSwdmxRmInfo.u8RecDmxMoviPos == _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx]\n");
            _rSwdmxRmInfo.u8RecDmxMoviPos = 0;
            _rSwdmxRmInfo.u4ChangeMode = 0;
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
            _rSwdmxRmInfo.fgIsBlock = TRUE;
            //_rSwdmxRmInfo.u4BlockChunkSize = u4PacketSize;  !!!
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
        if (_rSwdmxRmInfo.fgCookSBPending)  // still pending, give up this turn
        {
            *pu4ConsumeSize = 0;
            x_thread_delay(1);
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

        u1TrackIdx = _rSwdmxRmInfo.u1CurATrack;
        if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA && _rSwdmxRmInfo.rCookInfo.u1FlavorID > 29)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
        else if (i4PlaySpeed != SWDMX_PLAY_SPEED_1X)
        {
            //_SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);  !!!
            //_rSwdmxRmInfo.u4NumSentAud++;
            _rSwdmxRmInfo.u4PullBackAudPts = u4Pts * 90;  // record the PTS for later usage

            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
#if 1  // !!!
        else if (_rSwdmxRmInfo.u4ModeChangPts != 0xFFFFFFFF)
        {
            if (u1TrackIdx < RM_MAX_AUDIO_TRACK)
            {
                if (u4Pts * 90 >= _rSwdmxRmInfo.u4ModeChangPts)
                {
                    LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n",
                        u4Pts * 90,
                        _rSwdmxRmInfo.u4ModeChangPts);
                    //VDP_TriggerAudReceive(u4Pts * 90);
                    _rSwdmxRmInfo.u4TriggerAudPts = u4Pts * 90;
                    AUD_SetStartPts(0, u4Pts * 90);
                    _rSwdmxRmInfo.u4ModeChangPts = 0xFFFFFFFF;
                    _rSwdmxRmInfo.u4ChangeMode = 0;
                }
                else
                {
                    _SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);
                    u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
            }
        }
        else if(((UINT32)(_rSwdmxRmInfo.u8ASeekPts>>32) != 0xFFFFFFFF) ||
            ((UINT32)(_rSwdmxRmInfo.u8ASeekPts) != 0xFFFFFFFF))
        {
            if((u1TrackIdx < RM_MAX_AUDIO_TRACK) &&
                (u4Pts * 90 < (UINT32)_rSwdmxRmInfo.u8ASeekPts))
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
            *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) -
                                                               (UINT32)(pucOriginalPtr) -
                                                               u4RingSize);
        }
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Get available size here (we don't have to query subtitle available size here)
    if (u1HdrType != eSWDMX_MEDIA_SUBTITLE)
    {
        u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
        if ((u4PacketSize - (fgVer1 ? 13 : 12))> u4DmxAvailSize)
        {
            if (_rSwdmxRmInfo.fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp();
#ifdef GOOD
                _rSwdmxRmInfo.fgVDecIsPause = FALSE;
#else
                //_rSwdmxRmInfo.fgVDecIsPause = FALSE;  !!!
#endif
            }

            if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
                !_rSwdmxRmInfo.fgEnVideo &&
                _rSwdmxRmInfo.fgAudIsReady)
            {
                AUD_MMAoutEnable(0, TRUE);
                _rSwdmxRmInfo.fgAudIsReady = FALSE;
            }

            LOG(7, "Chunk is blocked u1HdrType = %ld, u4PacketSize = %ld, u4DmxAvailSize = %ld\n",
                u1HdrType,
                u4PacketSize,
                u4DmxAvailSize);
            *pu4ConsumeSize = 0;
            x_thread_delay(10);

#ifdef RMLS
            if (_fgRmLs)
            {
                if (RM_STRM_VDO == _u4RmStrmIdx)
                {
                    if (_rSwdmxRmInfo.fgDeliverAEos)
                    {
                        // Audio is finished, no need to change to audio
                    }
                    else if (!_rSwdmxRmInfo.fgEnAudio)
                    {
                        // Audio is off, no need to change to audio
                    }
                    else
                    {
                        LOG(3, "V fifo full:Set _fgRmChgStrm\n");
                        _fgRmChgStrm = TRUE;
                    }
                }
                else if (RM_STRM_AUD == _u4RmStrmIdx)
                {
                    if (_rSwdmxRmInfo.fgDeliverVEos)
                    {
                        // Video is finished, no need to change to video
                    }
                    else
                    {
                        LOG(3, "A fifo full:Set _fgRmChgStrm\n");
                        _fgRmChgStrm = TRUE;
                    }
                }
                else
                {
                    LOG(0, "Invalid _u4RmStrmIdx: %d\n", _u4RmStrmIdx);
                    ASSERT(0);
                }
            }
#endif

            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

        _SWDMX_RmInitDmx(u1HdrType, FALSE);
    }

    _rRmDmxMMData.u4FrameSize = u4PacketSize - (fgVer1 ? 13 : 12);

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
#ifdef COOK_WRITE_AFIFO
        if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA && _fgAFifoForCookSBFull)
        {
            if (dwAFifoForCookSBSpace() <= _rSwdmxRmInfo.rCookInfo.u2CookSBSz)
            {
                LOG(7, "COOK deinterleave is blocked because of AFifo near full...\n");
                *pu4ConsumeSize = 0;
                x_thread_delay(10);
                return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
            }
            else
            {
                LOG(7, "COOK deinterleave re-turned ON!!!\n");
                _fgAFifoForCookSBFull = FALSE;
            }
        }
#endif
        u1TrackIdx = _rSwdmxRmInfo.u1CurATrack;

        if (u1TrackIdx < RM_MAX_AUDIO_TRACK)
        {
            UINT32 u4TimeSeekAudPts = u4Pts * 90;  //_rSwdmxRmInfo.u4AudPts[u1TrackIdx];  !!!

            _SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);
            _rSwdmxRmInfo.u4NumSentAud++;
            //u4Pts = _rSwdmxRmInfo.u4AudPts[u1TrackIdx];

            //enable audio according to pts, need fix by aud index table
#if 1  // !!!
            if(((UINT32)(_rSwdmxRmInfo.u8ASeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(_rSwdmxRmInfo.u8ASeekPts) != 0xFFFFFFFF))
            {
                if ((u4TimeSeekAudPts >= (UINT32)_rSwdmxRmInfo.u8ASeekPts) &&
                    (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC ||
                     _rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA && (bTmpVal & HX_KEYFRAME_FLAG)))
                {
                    AUD_SetStartPts(0, u4TimeSeekAudPts);

                    if (_rSwdmxRmInfo.fgEnVideo)
                    {
                        VDP_SetPauseMm(0, TRUE);
                        _rSwdmxRmInfo.u4TriggerAudPts = (UINT32)u4TimeSeekAudPts;

                        VDEC_Pause(ES0);
                        _rSwdmxRmInfo.fgVDecIsPause = TRUE;
                    }

                    LOG(7, "Got Aud T PTS 0x%x, Seek 0x%x\n",
                        u4TimeSeekAudPts, (UINT32)_rSwdmxRmInfo.u8ASeekPts);
                    _rSwdmxRmInfo.u8ASeekPts = 0xFFFFFFFF;
                    _rSwdmxRmInfo.u8ASeekPts <<= 32;
                    _rSwdmxRmInfo.u8ASeekPts += 0xFFFFFFFF;
                }
                else
                {
                    //skip audio data in seek process
                    *pu4ConsumeSize = u4PacketSize + i;
                    if (fgRingBuf)
                    {
                        *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) -
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

        if (_rSwdmxRmInfo.fgEnVideo == FALSE)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }

        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
#ifdef GOOD
        if (u2CurQNum > 60/*(_rSwdmxRmInfo.u4VidFps * 3)*/)
#else
        if (u2CurQNum > 60/*(_rSwdmxRmInfo.u4VidFps * 3)*/ && 0)
#endif
        {
            if (_rSwdmxRmInfo.fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp();
                _rSwdmxRmInfo.fgVDecIsPause = FALSE;
            }
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            *pu4ConsumeSize = 0;
            x_thread_delay(10);
            return eSWDMX_HDR_PRS_SUCCEED;
        }

        //u4Pts = _rSwdmxRmInfo.u4CurVidPts;

        _SWDMX_RmCalculatePts(u1HdrType, u1TrackIdx, u4PacketSize);

        // Record every video's chunk position.
        //_rSwdmxRmInfo.u8VidOffset = _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx];  !!!

        if(((UINT32)(_rSwdmxRmInfo.u8VSeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(_rSwdmxRmInfo.u8VSeekPts) != 0xFFFFFFFF))
        {
            if(u4Pts > (UINT32)_rSwdmxRmInfo.u8VSeekPts)
            {
                LOG(5, "Got Vid T PTS 0x%x, Seek 0x%x\n",
                    u4Pts, (UINT32)_rSwdmxRmInfo.u8VSeekPts);
                _rSwdmxRmInfo.u8VSeekPts = 0xFFFFFFFF;
                _rSwdmxRmInfo.u8VSeekPts <<= 32;
                _rSwdmxRmInfo.u8VSeekPts += 0xFFFFFFFF;
            }
            /*else if (prRangeList->prDmxRangeInfo->ui8_vid_duration < (UINT32)_rSwdmxRmInfo.u8VSeekPts)
            {
                if (!_rSwdmxRmInfo.fgDeliverVEos)
                {
                    LOG(5, "prRangeList->prDmxRangeInfo->ui8_vid_duration < (UINT32)_rSwdmxRmInfo.u8VSeekPts\n");
                    if (!_SWDMX_SendRmEos(eSWDMX_MEDIA_VIDEO))
                    {
                        LOG(3, "Fail to send Video Eos\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                    _rSwdmxRmInfo.fgDeliverVEos = TRUE;
                }
            }*/
        }
#endif
    }

    _rRmDmxMMData.u4BufStart = (UINT32)(VIRTUAL(_rFeederInfo.u4StartAddr));
    _rRmDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr));
    u4SrcBufLen = _rRmDmxMMData.u4BufEnd - _rRmDmxMMData.u4BufStart;
    _rRmDmxMMData.u4Pts = u4Pts * 90;
    _rRmDmxMMData.u4Dts = u4Pts * 90;

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
      if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC)
      {
        UINT32 u4Ptr = _rRmDmxMMData.u4StartAddr;
        UINT16 u2NumAACFrm, u2CurAACFrmIdx, u2CurAACFrmSz;

        SWDMX_LOADB_WORD(/*0x30000000 + */u4Ptr, u2NumAACFrm);
        u4Ptr += 2;
        if (u4Ptr >= _rRmDmxMMData.u4BufEnd)
        {
            if (u4Ptr == _rRmDmxMMData.u4BufEnd)
            {
                u4Ptr = _rRmDmxMMData.u4BufStart;
            }
            else if (u4Ptr == _rRmDmxMMData.u4BufEnd + 1)
            {
                u4Ptr = _rRmDmxMMData.u4BufStart + 1;
                u2NumAACFrm = (u2NumAACFrm & 0xFF00) | *(UINT8 *)(_rRmDmxMMData.u4BufStart);
            }
            else
            {
                ASSERT(0);
            }
        }
        u2NumAACFrm /= 16;

        _rRmDmxMMData.u4StartAddr += (u2NumAACFrm * 2 + 2);
        if (_rRmDmxMMData.u4StartAddr >= _rRmDmxMMData.u4BufEnd)
            _rRmDmxMMData.u4StartAddr -= u4SrcBufLen;

        for (u2CurAACFrmIdx = 0; u2CurAACFrmIdx < u2NumAACFrm; u2CurAACFrmIdx++)
        {
            SWDMX_LOADB_WORD(/*0x30000000 + */u4Ptr, u2CurAACFrmSz);
            u4Ptr += 2;
            if (u4Ptr >= _rRmDmxMMData.u4BufEnd)
            {
                if (u4Ptr == _rRmDmxMMData.u4BufEnd)
                {
                    u4Ptr = _rRmDmxMMData.u4BufStart;
                }
                else if (u4Ptr == _rRmDmxMMData.u4BufEnd + 1)
                {
                    u4Ptr = _rRmDmxMMData.u4BufStart + 1;
                    u2CurAACFrmSz = (u2CurAACFrmSz & 0xFF00) | *(UINT8 *)(_rRmDmxMMData.u4BufStart);
                }
                else
                {
                    ASSERT(0);
                }
            }
            _rRmDmxMMData.u4FrameSize = u2CurAACFrmSz;

            // check if the parsed frame size is correct
            if (u2CurAACFrmSz >= DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO))
            {
                LOG(0, "2575 length error: %d!!!!!!!!\n", u2CurAACFrmSz);
                *pu4ConsumeSize = u4PacketSize + i;  // incorrect frame size (corrupt data), discard this packet
                return eSWDMX_HDR_PRS_SUCCEED;
            }

            // transfer the AAC header (7 bytes) first
            if (!_SWDMX_TxAACFrame(u2CurAACFrmSz))
            {
                ASSERT(0);  // !!!
            }

            UINT32 u4BufStartBak = _rRmDmxMMData.u4BufStart;
            if (_rRmDmxMMData.u4StartAddr < _rRmDmxMMData.u4BufStart)
                _rRmDmxMMData.u4BufStart -= FEEDER_RESERVED_SIZE;

            if (DMX_MM_MoveData(eSWDMX_MEDIA_AUDIO, &_rRmDmxMMData, 500) != TRUE)
            {
                ASSERT(0);  // !!!
                LOG(0, "Demuxer fails to move data.\n");
                *pu4ConsumeSize = 0;
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }

            _rRmDmxMMData.u4BufStart = u4BufStartBak;

            _rRmDmxMMData.u4StartAddr += _rRmDmxMMData.u4FrameSize;
            if (_rRmDmxMMData.u4StartAddr >= _rRmDmxMMData.u4BufEnd)
                _rRmDmxMMData.u4StartAddr -= u4SrcBufLen;
        }

#ifdef RMLS
        if (_fgRmLs)
        {
            ASSERT(RM_STRM_AUD == _u4RmStrmIdx);
            _rSwdmxRmInfo.u4LsPts[RM_STRM_AUD] = u4Pts * 90;

            if (_rSwdmxRmInfo.fgDeliverVEos)
            {
                // Video is finished, no need to change to video
            }
            else if (_rSwdmxRmInfo.u4LsPts[RM_STRM_AUD] > (_rSwdmxRmInfo.u4LsPts[RM_STRM_VDO] + RM_LS_CHG_STRM_THRD))
            {
                RM_DBG_LOG("2891:Set _fgRmChgStrm\n");
                _fgRmChgStrm = TRUE;
            }
        }
#endif
      }
      else if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA)
      {
          if (_dwCurCookFrameInSB == 0)
          {
#if 1  //add for CQ PL00118234
              if (!(bTmpVal & HX_KEYFRAME_FLAG))
              {
                  //printf("swdmx_rm 2427 skip this frame\n");
                  _rRmDmxMMData.u4StartAddr += _rRmDmxMMData.u4FrameSize;
                  if (_rRmDmxMMData.u4StartAddr >= _rRmDmxMMData.u4BufEnd)
                      _rRmDmxMMData.u4StartAddr -= u4SrcBufLen;

                  goto RM_PKT_END;
              }
              else
              {
                  //printf("COOK SB PTS = 0x%X\n", u4Pts * 90);
                  _rSwdmxRmInfo.u4CookSBPts = u4Pts * 90;
              }
#endif
              //_dwLastCookAPts = _dwPrsPts * 90;
              //printf("swdmx_rm 2436 beginning from this frame @ %08X\n", (UINT32)_rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx]);

              if (_rSwdmxRmInfo.fgCookPtsPending)
              {
                  _rSwdmxRmInfo.fgCookPtsPending = FALSE;
                  _rSwdmxRmInfo.u4TriggerAudPts = u4Pts * 90;
                  AUD_SetStartPts(0, _rSwdmxRmInfo.u4TriggerAudPts);
              }
          }

          vCookFrameDeInterleave((UINT8 *)_rRmDmxMMData.u4StartAddr);
      }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)  // video
    {
        BOOL fgBreak = FALSE;
        UINT16 wVal;
        UINT8 bVal;
        RVFrameHdr *prHeader = &_rFrameHdr;

        _pbHdrBuf = (UINT8 *)_rRmDmxMMData.u4StartAddr;
        _dwPktSz = u4PacketSize - (fgVer1 ? 13 : 12);
        _dwPrsState = RMPRS_FRMHDR;
        vMpsPrsNextG(0, 2, 0);
        while (!fgBreak)
        {
            switch (_dwPrsState)
            {
            case RMPRS_FRMHDR:
                _dwPktSz -= 2;
                prHeader->eType = (RVFrameType)((_pbHdrBuf[0] & 0xC0) >> 6);
                switch (prHeader->eType)
                {
                case RVFrameTypeLastPartial:
                  _dwPktSz = _dwPktSz + 0;  // just for setting breakpoint
                case RVFrameTypePartial:
                  SWDMX_LOADB_WORD(_pbHdrBuf, wVal);
                  prHeader->u4NumPackets = (wVal & 0x3F80) >> 7;
                  prHeader->u4PacketNum  = wVal & 0x007F;
                  prHeader->u4FrameSize = 0xFFFFFFFF;
                  prHeader->u4PartialFrameSize = 0xFFFFFFFF;
                  prHeader->u4PartialFrameOffset = 0xFFFFFFFF;
                  prHeader->u4Timestamp = 0xFFFFFFFF;
                  _dwPrsState = RMPRS_FRM_ENTIRE_SIZE;
                  vMpsPrsNextG(_wGTxLen, 2, 0);

                  continue;

                case RVFrameTypeWhole:
                  prHeader->u4NumPackets = 1;
                  prHeader->u4PacketNum = 1;
                  prHeader->u4FrameSize = _dwPktSz;
                  prHeader->u4PartialFrameSize = _dwPktSz;
                  prHeader->u4PartialFrameOffset = 0;
                  SWDMX_LOAD_BYTE(_pbHdrBuf + 1, prHeader->u1SeqNum);
                  prHeader->u4Timestamp = u4Pts;  // !!! check if relative timestamp
                  //prHeader->u4HeaderSize = pbHdrBuf - _pbHdrBuf;  !!! check if header size is useful or not

                  _au4SliceSz[0] = prHeader->u4PartialFrameSize;
                  _au4SliceOfst[0] = prHeader->u4PartialFrameOffset;

                  _dwPrsState = RMPRS_FRAME;
#if 0  // megaa 20080625
                  _tPrs.bPrsFlg = VPR_ON | PR_ON;
#endif
#if 1
                  //_dVWpBak = dPRVFifoWp();  // save V Wp for picture type detection  !!!
                  {
                      DMX_PID_T  rPid;

                      DMX_GetPid(eSWDMX_MEDIA_VIDEO, DMX_PID_FLAG_DATA_POINTERS, &rPid);

                      _rSwdmxRmInfo.u4VFifoWpBak = rPid.u4Wp;
                  }
                  //_dwFrmPktOfst = _dwPktOfst;
                  if (!fgMpsPrsNextP(_wGTxLen, prHeader->u4PartialFrameSize, prHeader->u4Timestamp))
                  {
                      LOG(0, "2696 length error: %d!!!!!!!!\n", prHeader->u4PartialFrameSize);
                      fgBreak = TRUE;
                  }
#else
                  _bStrmType = PR_VST;
                  _dVWpBak = dPRVFifoWp();  // save V Wp for picture type detection
                  _dwFrmPktOfst = _dwPktOfst;
                  fgMpsPrsNextP(_wGTxLen, prHeader->u4PartialFrameSize);
#endif
                  continue;

                case RVFrameTypeMultiple:
                  SWDMX_LOAD_BYTE(_pbHdrBuf + 1, bVal);
                  prHeader->u4NumPackets = 1;
                  prHeader->u4PacketNum = 1;
                  prHeader->u4FrameSize = bVal & 0x7F;  // keep the 14-or-30 flag temporarily
                  prHeader->u4PartialFrameSize = 0;
                  prHeader->u4PartialFrameOffset = 0;
                  _dwPrsState = RMPRS_FRM_ONE_OF_MLTI_SIZE;
                  if (bVal & 0x40)                      // check the 14-or-30 flag
                    vMpsPrsNextG(_wGTxLen, 1, 0);       // get the residual 8 of 14 bits
                  else
                    vMpsPrsNextG(_wGTxLen, 3, 0);       // get the residual 24 of 30 bits

                  continue;
                }

                ASSERT(0);  // should never reach here

    case RMPRS_FRM_ONE_OF_MLTI_SIZE:
      if (prHeader->u4FrameSize & 0x40)  // check the 14-or-30 flag again
      {                                  // 8 of 14 bits
        _dwPktSz -= 1;
        SWDMX_LOAD_BYTE(_pbHdrBuf, bVal);
        prHeader->u4FrameSize &= 0x3F;   // mark off the 14-or-30 flag
        prHeader->u4FrameSize <<= 8;
        prHeader->u4FrameSize |= bVal;
      }
      else                               // 24 of 30 bits
      {
        _dwPktSz -= 3;
        SWDMX_LOAD_BYTE(_pbHdrBuf, bVal);
        prHeader->u4FrameSize <<= 8;
        prHeader->u4FrameSize |= bVal;
        SWDMX_LOADB_WORD(_pbHdrBuf + 1, wVal);
        prHeader->u4FrameSize <<= 16;
        prHeader->u4FrameSize |= wVal;
      }
      prHeader->u4PartialFrameSize = prHeader->u4FrameSize;
      prHeader->u4Timestamp = 0xFFFFFFFF;
      _dwPrsState = RMPRS_FRM_TIMESTAMP;
      vMpsPrsNextG(_wGTxLen, 2, 0);
      break;

    case RMPRS_FRM_TIMESTAMP:
      _dwPktSz -= 2;
      SWDMX_LOADB_WORD(_pbHdrBuf, wVal);
      if (prHeader->u4Timestamp != 0xFFFFFFFF)  // it is the 2nd run of this state to get the additional 16 bits
      {
        prHeader->u4Timestamp <<= 16;
        prHeader->u4Timestamp |= wVal;
        _dwPrsState = RMPRS_FRM_SEQ_NUM;
        vMpsPrsNextG(_wGTxLen, 1, 0);
        continue;
      }
      prHeader->u4Timestamp = wVal & 0x3FFF;
      if (wVal & 0x4000)  // 14 bits
      {
        // we have got the whole timestamp, enter the next state
        _dwPrsState = RMPRS_FRM_SEQ_NUM;
        vMpsPrsNextG(_wGTxLen, 1, 0);
      }
      else
      {
        // trigger the 2nd run
        vMpsPrsNextG(_wGTxLen, 2, 0);
      }
      continue;

    case RMPRS_FRM_ENTIRE_SIZE:
      SWDMX_LOADB_WORD(_pbHdrBuf, wVal);
      _dwPktSz -= 2;
      if (prHeader->u4FrameSize != 0xFFFFFFFF)  // it is the 2nd run of this state to get the additional 16 bits
      {
        prHeader->u4FrameSize <<= 16;
        prHeader->u4FrameSize |= wVal;
        _dwPrsState = RMPRS_FRM_PARTIAL_OFST;
        vMpsPrsNextG(_wGTxLen, 2, 0);
        continue;
      }
      prHeader->fgBrokenUpByUs = ((wVal & 0x8000) != 0 ? 1 : 0);
      prHeader->u4FrameSize = wVal & 0x3FFF;
      if (wVal & 0x4000)  // 14 bits
      {
        // we have got the whole size, enter the next state
        _dwPrsState = RMPRS_FRM_PARTIAL_OFST;
        vMpsPrsNextG(_wGTxLen, 2, 0);
      }
      else  // 30 bits
      {
        // trigger the 2nd run
        vMpsPrsNextG(_wGTxLen, 2, 0);
      }
      continue;

    case RMPRS_FRM_PARTIAL_OFST:
      SWDMX_LOADB_WORD(_pbHdrBuf, wVal);
      _dwPktSz -= 2;
      if (prHeader->eType == RVFrameTypePartial)
      {
        if (prHeader->u4PartialFrameOffset != 0xFFFFFFFF)  // it is the 2nd run of this state to get the additional 16 bits
        {
          prHeader->u4PartialFrameOffset <<= 16;
          prHeader->u4PartialFrameOffset |= wVal;
          prHeader->u4PartialFrameSize = _dwPktSz - 1;
          _dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(_wGTxLen, 1, 0);
          continue;
        }
        prHeader->u4PartialFrameOffset = wVal & 0x3FFF;
        if (wVal & 0x4000)  // 14 bits
        {
          // we have got the whole offset, enter the next state
          prHeader->u4PartialFrameSize = _dwPktSz - 1;
          _dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(_wGTxLen, 1, 0);
        }
        else  // 30 bits
        {
          // trigger the 2nd run
          vMpsPrsNextG(_wGTxLen, 2, 0);
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
          _dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(_wGTxLen, 1, 0);
          continue;
        }
        prHeader->u4PartialFrameSize = wVal & 0x3FFF;
        if (wVal & 0x4000)  // 14 bits
        {
          // we have got the whole size, enter the next state
          prHeader->u4PartialFrameOffset = prHeader->u4FrameSize - prHeader->u4PartialFrameSize;
          _dwPrsState = RMPRS_FRM_SEQ_NUM;
          vMpsPrsNextG(_wGTxLen, 1, 0);
        }
        else  // 30 bits
        {
          // trigger the 2nd run
          vMpsPrsNextG(_wGTxLen, 2, 0);
        }
        continue;
      }

      ASSERT(0);  // should never reach here

    case RMPRS_FRM_SEQ_NUM:
      SWDMX_LOAD_BYTE(_pbHdrBuf, prHeader->u1SeqNum);
      _dwPktSz--;
      if (prHeader->u4Timestamp == 0xFFFFFFFF)
        prHeader->u4Timestamp = u4Pts;  // !!! check if relative timestamp
      //prHeader->u4HeaderSize = pbHdrBuf - _pbHdrBuf;  !!! check if header size is useful or not

      _au4SliceSz[prHeader->u4PacketNum - 1] = prHeader->u4PartialFrameSize;
      _au4SliceOfst[prHeader->u4PacketNum - 1] = prHeader->u4PartialFrameOffset;

      _dwPrsState = RMPRS_FRAME;
#if 0  // megaa 20080625
      _tPrs.bPrsFlg = VPR_ON | PR_ON;
#endif
#if 1
      if (prHeader->u4PacketNum == 1)
      {
          DMX_PID_T  rPid;

          DMX_GetPid(eSWDMX_MEDIA_VIDEO, DMX_PID_FLAG_DATA_POINTERS, &rPid);

          _rSwdmxRmInfo.u4VFifoWpBak = rPid.u4Wp;
          //_dVWpBak = dPRVFifoWp();  // save V Wp for picture type detection  !!!
          //_dwFrmPktOfst = _dwPktOfst;
      }
      if (!fgMpsPrsNextP(_wGTxLen, prHeader->u4PartialFrameSize, prHeader->u4Timestamp))
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
      fgMpsPrsNextP(_wGTxLen, prHeader->u4PartialFrameSize);
#endif
      break;

    case RMPRS_FRM_NEXT_MULTIPLE:
      SWDMX_LOAD_BYTE(_pbHdrBuf, bVal);
      if (bVal != 0xC0)
      {
          LOG(0, "2904 bVal != 0xC0!!!!!!\n");
          fgBreak = TRUE;  // discard this packet directly
          break;
      }
      SWDMX_LOAD_BYTE(_pbHdrBuf + 1, bVal);
      _dwPktSz -= 2;
      prHeader->u4NumPackets = 1;
      prHeader->u4PacketNum = 1;
      prHeader->u4FrameSize = bVal & 0x7F;  // keep the 14-or-30 flag temporarily
      prHeader->u4PartialFrameSize = 0;
      prHeader->u4PartialFrameOffset = 0;
      _dwPrsState = RMPRS_FRM_ONE_OF_MLTI_SIZE;
      if (bVal & 0x40)                      // check the 14-or-30 flag
        vMpsPrsNextG(_wGTxLen, 1, 0);       // get the residual 8 of 14 bits
      else
        vMpsPrsNextG(_wGTxLen, 3, 0);       // get the residual 24 of 30 bits
      break;

    case RMPRS_PACKET:
        if (fgOneVideoFrm)
        {
            if (_rSwdmxRmInfo.u4NumSentVid > u4OldNumSentVid)  // wait until one complete video frame has been sent
            {
                fgBreak = TRUE;
            }
            else
            {
                //x_thread_delay(2);  //remove delay, when merging many packets into a frame, it'll spend too much time.

                SWDMX_LOADB_WORD(_pbHdrBuf + 2, wVal);
                _dwPktSz = wVal - (fgVer1 ? 13 : 12);
                _pbHdrBuf += (fgVer1 ? 13 : 12);
                _dwPrsState = RMPRS_FRMHDR;
                vMpsPrsNextG(0, 2, 0);
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
    if (_fgRmLs)
    {
        ASSERT(RM_STRM_VDO == _u4RmStrmIdx);
        _rSwdmxRmInfo.u4LsPts[RM_STRM_VDO] = u4Pts * 90;

        if (_rSwdmxRmInfo.fgDeliverAEos)
        {
            // Audio is finished, no need to change to audio
        }
        else if (!_rSwdmxRmInfo.fgEnAudio)
        {
            // Audio is off, no need to change to audio
        }
        else if (_rSwdmxRmInfo.u4LsPts[RM_STRM_VDO] > (_rSwdmxRmInfo.u4LsPts[RM_STRM_AUD] + RM_LS_CHG_STRM_THRD))
        {
            RM_DBG_LOG("3264:Set _fgRmChgStrm\n");
            _fgRmChgStrm = TRUE;
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
        *pu4ConsumeSize += ((UINT32)(VIRTUAL(_rFeederInfo.u4EndAddr)) -
                                                           (UINT32)(pucOriginalPtr) -
                                                           u4RingSize);
    }

    if (_rSwdmxRmInfo.fgIsBlock == TRUE)
    {
        _rSwdmxRmInfo.fgIsBlock = FALSE;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsWithoutIdx(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    BOOL fgRangeEnd = FALSE;

    UINT32 u4ReadSize = 0;
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState = eSWDMX_HDR_PRS_SUCCEED;

    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    if (_rSwdmxRmInfo.fgFeederInvalid)
    {
        INT32 i;
        LOG(3, "Feeder is invalid, Cur Feeder ID = %ld, req times = %ld, Cur Dmx Pos = %lld\n",
            _rSwdmxRmInfo.u4FeederReqID,
            _rSwdmxRmInfo.i4ReqReadTimes,
            _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx]);

        for (i = 0; i < _rSwdmxRmInfo.i4ReqReadTimes; i++)
        {
            _SWDMX_RmGetDataFromFeeder(&rFeederReqData);
        }

        _SWDMX_RmResetFeeder();

        _rSwdmxRmInfo.fgIsSeek = TRUE;
        _rSwdmxRmInfo.u4LeftBytesInFeeder = 0;
        _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] = _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx];
        _rSwdmxRmInfo.fgFeederInvalid = FALSE;
        _rSwdmxRmInfo.i4ReqReadTimes = 0;
        _rSwdmxRmInfo.fgReqDataMiss = FALSE;
        _rSwdmxRmInfo.u4FeederReqID += 1000;
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Check if demuxing is done
    if (_rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] >= _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] + _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx]
#ifdef __MODEL_slt__
        || _rSwdmxRmInfo.u4NumSentVid >= 30
#endif
        )
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (_rSwdmxRmInfo.fgEnVideo
            && !_rSwdmxRmInfo.fgDeliverVEos
#ifdef RMLS
            && (!_fgRmLs ||_u4RmStrmIdx == RM_STRM_VDO)
#endif
        )
        {
            if (_rSwdmxRmInfo.fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp();
                _rSwdmxRmInfo.fgVDecIsPause = FALSE;
            }

            LOG(5, "Cur DmxMoviPos = %lld, range size = %lld, range offset = %lld\n",
                _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx],
                _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx],
                _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx]);

            LOG(5, "_SWDMX_RmHdrPrsWithoutIdx Deliver Video Eos\n");

#ifdef RMLS
            if (_fgRmLs && _rSwdmxRmInfo.fgEnAudio && !_rSwdmxRmInfo.fgDeliverAEos)
            {
                // wait audio finished
                LOG(3, "Deliver Video Eos: Set _fgRmChgStrm\n");
                _fgRmChgStrm = TRUE;
            }
#endif
            if (!_SWDMX_SendRmEos(eSWDMX_MEDIA_VIDEO))
            {
                LOG(3, "Fail to send Video Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxRmInfo.fgDeliverVEos = TRUE;
        }

        if (_rSwdmxRmInfo.fgEnAudio
            && !_rSwdmxRmInfo.fgDeliverAEos
#ifdef RMLS
            && (!_fgRmLs ||_u4RmStrmIdx == RM_STRM_AUD)
#endif
        )
        {
            LOG(5, "_SWDMX_RmHdrPrsWithoutIdx Deliver Audio Eos\n");

#ifdef RMLS
            if (_fgRmLs && !_rSwdmxRmInfo.fgDeliverVEos)
            {
                // wait video finished
                LOG(3, "Deliver Audio Eos: Set _fgRmChgStrm\n");
                _fgRmChgStrm = TRUE;
            }
#endif
            if (!_SWDMX_SendRmEos(eSWDMX_MEDIA_AUDIO))
            {
                LOG(3, "Fail to send Audio Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxRmInfo.fgDeliverAEos = TRUE;
            AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
        }

        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Request data for the first time
    if (_rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] == 0)
    {
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        u4ReadSize = _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx];
        u4ReadSize = min(u4ReadSize, RM_MIN_READ_SIZE);
        _u4LastAlignOffset = 0;

        _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] = _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx];
        _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] = _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx];

#if 1
        if (!_rSwdmxRmInfo.fgReqDataMiss)
        {

            if (FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                    u4ReadSize, /*RM_MIN_READ_SIZE, */
                    _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx],
                    &rFeederCond,
                    _rSwdmxRmInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
            {
                 _rSwdmxRmInfo.i4ReqReadTimes ++;
                if (!_SWDMX_RmGetDataFromFeeder(&rFeederReqData))
                {
                     //_rSwdmxRmInfo.fgFeederInvalid = TRUE;
                    return eSWDMX_HDR_PRS_FAIL;
                }
                _rSwdmxRmInfo.i4ReqReadTimes --;
                _u4LastAlignOffset = rFeederReqData.u4AlignOffset;
                _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
                _rSwdmxRmInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
                _rSwdmxRmInfo.fgIsSeek = FALSE;
                _rSwdmxRmInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
                LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
            }
        }
        else
        {
            LOG(5, "wait for missed data request\n");
            if (!_SWDMX_RmGetDataFromFeeder(&rFeederReqData))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            _rSwdmxRmInfo.i4ReqReadTimes --;
            _u4LastAlignOffset = rFeederReqData.u4AlignOffset;
            _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
            _rSwdmxRmInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
            _rSwdmxRmInfo.fgIsSeek = FALSE;
            _rSwdmxRmInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
            LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
        }
#else
        if (FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                u4ReadSize, /*RM_MIN_READ_SIZE, */
                _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx],
                &rFeederCond,
                _rSwdmxRmInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
        {
             _rSwdmxRmInfo.i4ReqReadTimes ++;
            if (!_SWDMX_RmGetDataFromFeeder(&rFeederReqData))
            {
                 //_rSwdmxRmInfo.fgFeederInvalid = TRUE;
                return eSWDMX_HDR_PRS_FAIL;
            }
             _rSwdmxRmInfo.i4ReqReadTimes --;
            _u4LastAlignOffset = rFeederReqData.u4AlignOffset;
            _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
            _rSwdmxRmInfo.u4CurDmxFeederRPtr = VIRTUAL(_rFeederInfo.u4StartAddr + _u4LastAlignOffset);
             _rSwdmxRmInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
            LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
        }
#endif
    }

    if ((_rSwdmxRmInfo.u4LeftBytesInFeeder <=  RM_BUFFER_SIZE) ||
        (_rSwdmxRmInfo.fgIsBlock == TRUE))
    {
        if (_rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] + _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx] > _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx])
        {
            u4ReadSize = _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] + _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx] -
                                   _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx];
        }
        u4ReadSize = min(u4ReadSize, RM_MIN_READ_SIZE);

        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }

#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        if ((!_rSwdmxRmInfo.fgFeederIsReady) ||
            (_rSwdmxRmInfo.u4LeftBytesInFeeder + u4ReadSize < RM_BUFFER_SIZE))
#else
        if (_rSwdmxRmInfo.u4LeftBytesInFeeder + u4ReadSize < RM_BUFFER_SIZE)
#endif
        {
            if (u4ReadSize > 0)
            {
                LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n",
                        _rSwdmxRmInfo.u4LeftBytesInFeeder,
                        _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx],
                        _rSwdmxRmInfo.u4FeederReqID);

              if (!_rSwdmxRmInfo.fgReqDataMiss)
              {

                if (FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                        u4ReadSize,
                        _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx],
                        &rFeederCond,
                        _rSwdmxRmInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
                {
                    _u4LastAlignOffset = 0;
                    _rSwdmxRmInfo.i4ReqReadTimes++;
                }
                else
                {
                    LOG(3, "Feeder Buffer Full\n");
                    //_rSwdmxRmInfo.fgFeederInvalid = TRUE;
                    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
                        FEEDER_E_OK)
                    {
                        LOG(1, "Feeder input buffer error.\n");
                        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                    }
                    LOG(3, "Cur Fed's write ptr = 0x%08x\n", _rFeederInfo.u4WriteAddr);
                    //return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                }
              }
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
    if (!_rSwdmxRmInfo.fgFeederIsReady && FeederIsReady(FEEDER_PROGRAM_SOURCE))
    /*if ((!_rSwdmxRmInfo.fgFeederIsReady) &&
        ((_rSwdmxRmInfo.u4LeftBytesInFeeder >= RM_PREBUFFER_SIZE) || (_rSwdmxRmInfo.u4LeftBytesInFeeder >= _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx])))*/
    {
        _rSwdmxRmInfo.fgFeederIsReady = TRUE;
    }
#endif

#ifdef SWDMX_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt;
    HAL_GetTime(&rTimeRstS);
#endif

#if defined(CC_DLNA_SUPPORT) && 0
    if ((_rSwdmxRmInfo.u4LeftBytesInFeeder > 0) && _rSwdmxRmInfo.fgFeederIsReady)
#else
    if (_rSwdmxRmInfo.u4LeftBytesInFeeder)
#endif
    {
        if (_rSwdmxRmInfo.fgSpeedChange && i4PlaySpeed == 1000)
        {
            if (_rSwdmxRmInfo.fgEnAudio && _rSwdmxRmInfo.fgEnVideo)
            {
                if(((UINT32)(_rSwdmxRmInfo.u8VSeekPts>>32) != 0xFFFFFFFF)
                    || ((UINT32)(_rSwdmxRmInfo.u8VSeekPts) != 0xFFFFFFFF))
                {
                    if (prRangeList->prDmxRangeInfo->ui8_vid_duration >
                        (UINT32)_rSwdmxRmInfo.u8VSeekPts)
                    {
                        LOG(5, "Pause Video for A/V Sync\n");
                        VDP_SetPauseMm(0, TRUE);
                        VDEC_Pause(ES0);
                        _rSwdmxRmInfo.fgVDecIsPause = TRUE;
                    }
                }
                else
                {
                    LOG(5, "Pause Video for A/V Sync\n");
                    VDP_SetPauseMm(0, TRUE);
                    VDEC_Pause(ES0);
                    _rSwdmxRmInfo.fgVDecIsPause = TRUE;
                }
            }
            _rSwdmxRmInfo.fgSpeedChange = FALSE;
        }

        // Start to parse data
        u4PrsState = _SWDMX_PrsRmPacket((UCHAR*)(_rSwdmxRmInfo.u4CurDmxFeederRPtr),
            prRangeList,
            _rSwdmxRmInfo.u4LeftBytesInFeeder,
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
            if ((_rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] >= _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] + _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx]) ||
                (_rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] > _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx] + ONE_MB))
            {
                LOG(3, "No RM chunks was discovered\n");
                _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] = _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] + _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx];
                _rSwdmxRmInfo.u4LeftBytesInFeeder = 0;
                return eSWDMX_HDR_PRS_FAIL;
            }
            break;
        default:
            break;
    }

    if (_rSwdmxRmInfo.u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] += u4ConsumedSize;
        _rSwdmxRmInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
    }
    else
    {
        _rSwdmxRmInfo.u4LeftBytesInFeeder = 0;
    }

    if (u4ConsumedSize != 0)
    {
        LOG(6, "u4ConsumedSize = %ld u4LeftBytesInFeeder = 0x%08x.\n",
            u4ConsumedSize, _rSwdmxRmInfo.u4LeftBytesInFeeder);
    }

    if ((_rSwdmxRmInfo.u4CurDmxFeederRPtr + u4ConsumedSize) >=
        VIRTUAL(_rFeederInfo.u4EndAddr))
    {
        _rSwdmxRmInfo.u4CurDmxFeederRPtr =  VIRTUAL(_rFeederInfo.u4StartAddr) +
            ((_rSwdmxRmInfo.u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(_rFeederInfo.u4EndAddr));
    }
    else
    {
        _rSwdmxRmInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
    }

    // Update read ptr to feeder
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxRmInfo.u4CurDmxFeederRPtr));

    LOG(9, "_rSwdmxRmInfo.u4CurDmxMoviPos = %ld.\n", _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx]);

    if (_rSwdmxRmInfo.i4ReqReadTimes > 0)
    {
        if (!_SWDMX_RmGetDataFromFeeder(&rFeederReqData))
        {
            //_rSwdmxRmInfo.fgFeederInvalid = TRUE;
            vHandleRmRingBuf();
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        LOG(6, "rFeederReqData.u4WriteAddr = 0x%08x. ReadSize = 0x%08x id = %ld\n",
            rFeederReqData.u4WriteAddr,
            rFeederReqData.u4ReadSize,
            rFeederReqData.u4Id);

        _u4LastAlignOffset = rFeederReqData.u4AlignOffset;

        if ((rFeederReqData.u4ReadSize > _u4LastAlignOffset) &&
            (rFeederReqData.u4ReadSize != 0))
        {
            _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
        }
        else
        {
            LOG(3, "Feeder read size is smaller than align offset!!!\n");
            _rSwdmxRmInfo.au8CurDmxFilePos[_u4RmStrmIdx] = _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] + _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx];
            _rSwdmxRmInfo.u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }

        _rSwdmxRmInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        _rSwdmxRmInfo.i4ReqReadTimes--;

        if(_rSwdmxRmInfo.fgIsSeek)
        {
            _rSwdmxRmInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
            _rSwdmxRmInfo.fgIsSeek = FALSE;
        }
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }

    vHandleRmRingBuf();

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrsUseIdx(
        INT32 i4Speed, SWDMX_RANGE_LIST_T* prRangeList)
{
    BOOL fgRangeDone = TRUE;

    //UINT32   u4VidOffset = 0;
    //UINT32   u4AudOffset = 0;

    UINT32 *pu4VidCurIdx  = NULL;
    //UINT32 *pu4AudCurIdx = NULL;

    MM_RANGE_ELMT_IDXTBL_T *pKeyIdxTbl = _rSwdmxRmInfo.prVidKeyIdxTbl;

    if (!prRangeList)
    {
        LOG(3, "Range list is NULL\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    if ((i4Speed > 2000) || (i4Speed < 0))
    {
        fgRangeDone = FALSE;
        if (pKeyIdxTbl != NULL &&
            (_rSwdmxRmInfo.u4CurSentKey < pKeyIdxTbl->ui4_number_of_entry))
        {

            LOG(6, "pKeyIdxTbl->ui4_number_of_entry = %x\n", pKeyIdxTbl->ui4_number_of_entry);
            LOG(6, "_rSwdmxRmInfo.u4CurSentKey = %x\n", _rSwdmxRmInfo.u4CurSentKey);
            pu4VidCurIdx = &_rSwdmxRmInfo.u4CurSentKey;

            if (_rSwdmxRmInfo.u4NumSentVid == 0)  // !!!
            {
                ASSERT(0);
#if 0
                _SWDMX_BeforeDeliverUseIdx(eSWDMX_MEDIA_VIDEO);
#endif
            }

            _SWDMX_DeliverRmKeyframe(i4Speed,
                _rSwdmxRmInfo.prVidKeyIdxTbl,
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
        ASSERT(0);
        LOG(0, "swdmx_rm #%d error!!!!!!\n", __LINE__);
        while (1);
    }

    if (fgRangeDone)
    {
        INT32 i;
        LOG(5, "Range is finished\n");
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
        _rSwdmxRmInfo.prVidIdxTbl = NULL;

        for (i = 0; (i < _rSwdmxRmInfo.u1TotalATrack) && (i < RM_MAX_AUDIO_TRACK); i++)
        {
            _rSwdmxRmInfo.prAudIdxTbl[i] = NULL;
        }
        _rSwdmxRmInfo.u1TotalATrack = 0;

        _rSwdmxRmInfo.fgBlockVid = FALSE;
        _rSwdmxRmInfo.fgBlockAud = FALSE;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverRmKeyframe(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl,
    UINT32* pu4CurIdx,
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed)
{
    UINT32 u4DmxAvailSize = 0;
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState;

    //DMX_MM_DATA_T _rRmDmxMMData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    UINT32 u4WriteAddr = 0;
    UINT32 u4ReadSize = 0;

    if(_rSwdmxRmInfo.fgFeederInvalid)
    {
        INT32 i;
        FEEDER_REQ_DATA_T rTmpFeederReqData;

        for (i = 0; i < _rSwdmxRmInfo.i4ReqReadTimes ; i++)
        {
            _SWDMX_RmGetDataFromFeeder(&rTmpFeederReqData);
        }

        _rSwdmxRmInfo.u4LeftBytesInFeeder = 0;
        _rSwdmxRmInfo.fgRepeatRequest = TRUE;
        _rSwdmxRmInfo.fgFeederInvalid = FALSE;
        _rSwdmxRmInfo.u4FeederReqID = 0;
        _rSwdmxRmInfo.fgReqDataMiss = FALSE;
        _rSwdmxRmInfo.i4ReqReadTimes = 0;

        _SWDMX_RmResetFeeder();

        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    x_memset(&_rRmDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);

    LOG(6, "_rSwdmxRmInfo.u4CurSentKey = %ld Key Pts = 0x%08x Dmx available size = 0x%08x\n",
        _rSwdmxRmInfo.u4CurSentKey,
        prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts * 90,
        u4DmxAvailSize);

    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > u4DmxAvailSize)
    {
        LOG(7, "Demuxer Buffer is not enough Hdr type = %x u4DmxAvailSize = %ld  frame size = %ld.\n",
            u1HdrType,
            u4DmxAvailSize,
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

        x_thread_delay(1);
        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Send sequence header for decoder's initialization?

    if (((*pu4CurIdx) < prIdxTbl->ui4_number_of_entry)&& !_rSwdmxRmInfo.fgDeliverVEos)
    {
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            //UCHAR *pBuf;
            UINT16 u2MaxQNum, u2CurQNum;
            VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);

            // Temporally use 30 for fps
            // Should use fps in the fucture.
            if (u2CurQNum > _rSwdmxRmInfo.u4VidFps)
            {
                x_thread_delay(1);
                return eSWDMX_HDR_PRS_SUCCEED;
            }

            if (_rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] + _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx] >
                (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset))
            {
                u4ReadSize = _rSwdmxRmInfo.au8RangeSz[_u4RmStrmIdx] +
                               _rSwdmxRmInfo.au8FileOffset[_u4RmStrmIdx] -
                               (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset);
            }
            else
            {
                ASSERT(0);
            }
            u4ReadSize = min(u4ReadSize, RM_MIN_READ_SIZE);

            LOG(7, "_rSwdmxRmInfo.u4NumSentVid %ld  ui4_size = 0x%08x\n", _rSwdmxRmInfo.u4NumSentVid, prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
            LOG(7, "ui4_relative_offset = %08x.\n", prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset);

#if 1
            if (!_rSwdmxRmInfo.fgReqDataMiss)
            {

                if (FeederSetRequest(FEEDER_PROGRAM_SOURCE,
#if 0  // !!! ToDo: Ask Mutil to put ui4_size
                    prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size,
#else
                    u4ReadSize,
#endif
                    (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset),
                    &rFeederCond,
                    _rSwdmxRmInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
                {
                    _rSwdmxRmInfo.i4ReqReadTimes++;
                    //LOG(3, "Feeder's buffer is full\n");
                    if (!_SWDMX_RmGetDataFromFeeder(&rFeederReqData))
                    {
                        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                    }
                }
                else
                {
                    LOG(3, "Feeder Buffer is not enough\n");
                    return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                }

            }
            else
            {
                LOG(5, "wait for missed data request\n");
                if (!_SWDMX_RmGetDataFromFeeder(&rFeederReqData))
                {
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }
            }
#else
            if (FeederSetRequest(FEEDER_PROGRAM_SOURCE,
#if 0  // !!! ToDo: Ask Mutil to put ui4_size
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size,
#else
                u4ReadSize,
#endif
                (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset),
                &rFeederCond,
                _rSwdmxRmInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
            {
                _rSwdmxRmInfo.i4ReqReadTimes++;
            }
            else
            {
                LOG(3, "Feeder's buffer is full\n");
            }


            if (!_SWDMX_RmGetDataFromFeeder(&rFeederReqData))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
#endif

            _rSwdmxRmInfo.i4ReqReadTimes--;
            _u4LastAlignOffset = rFeederReqData.u4AlignOffset;
            _rSwdmxRmInfo.au8CurFilePos[_u4RmStrmIdx] += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
            _rSwdmxRmInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);  //VIRTUAL(_rFeederInfo.u4StartAddr + _u4LastAlignOffset);
             _rSwdmxRmInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
            LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);

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

            vHandleRmRingBuf();

            u4WriteAddr = VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);
            _rSwdmxRmInfo.u4RecFeederWPtr = u4WriteAddr;
            _rSwdmxRmInfo.fgRequestData = FALSE;

            u4PrsState = _SWDMX_PrsRmPacket((UCHAR*)(_rSwdmxRmInfo.u4CurDmxFeederRPtr),
                prRangeList,
                _rSwdmxRmInfo.u4LeftBytesInFeeder,
                &u4ConsumedSize,
                i4PlaySpeed,
                FALSE,
                TRUE);

            if (u4PrsState != eSWDMX_HDR_PRS_SUCCEED)
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;

            if(((UINT32)(_rSwdmxRmInfo.u8SeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(_rSwdmxRmInfo.u8SeekPts) != 0xFFFFFFFF))
            {
                UINT32 u4Pts, u4CurSTC;

                u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts * 90;
                if(u4Pts > (UINT32)_rSwdmxRmInfo.u8SeekPts)
                {
                    STC_SetStcValue(u4Pts - 18000);
                    u4CurSTC = STC_GetStcValue();
                    LOG(1, "Got Vid T PTS 0x%x, STC 0x%x, Seek 0x%x\n",
                        u4Pts, u4CurSTC, (UINT32)_rSwdmxRmInfo.u8SeekPts);
                    _rSwdmxRmInfo.u8SeekPts = 0xFFFFFFFF;
                    _rSwdmxRmInfo.u8SeekPts <<= 32;
                    _rSwdmxRmInfo.u8SeekPts += 0xFFFFFFFF;
                }
            }
        }
        else
        {
            ASSERT(0);
        }
#if 1
        //update feeder RPTR
        _rSwdmxRmInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
        if(_rSwdmxRmInfo.u4CurDmxFeederRPtr > VIRTUAL(_rFeederInfo.u4EndAddr))
        {
            _rSwdmxRmInfo.u4CurDmxFeederRPtr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
        }
        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxRmInfo.u4CurDmxFeederRPtr));
#else
        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(u4WriteAddr));
#endif
        vHandleRmRingBuf();

        if (!_SWDMX_RmCalculateRepeatFrame(i4Speed))
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
        if (_rSwdmxRmInfo.u4VidRepeatFrame == 0)
        {
            if ((i4Speed > 2000) &&
            (_rSwdmxRmInfo.u4CurSentKey == prIdxTbl->ui4_number_of_entry) &&
            (_rSwdmxRmInfo.u4CurRepeatFrameIdx == prIdxTbl->ui4_number_of_entry - 1) &&
            !_rSwdmxRmInfo.fgDeliverVEos)
            {
                LOG(5, "Fast Forward _SWDMX_RmHdrPrsUseIdx Deliver Video Eos\n");
                if (!_SWDMX_SendRmEos(eSWDMX_MEDIA_VIDEO))
                {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                _rSwdmxRmInfo.fgDeliverVEos = TRUE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            else if ((i4Speed < 0) &&
                        (_rSwdmxRmInfo.u4CurSentKey == 0) &&
                        (_rSwdmxRmInfo.u4CurRepeatFrameIdx == 0) &&
                        !_rSwdmxRmInfo.fgDeliverVEos)
            {
                if (!_SWDMX_SendRmEos(eSWDMX_MEDIA_VIDEO))
                {
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                _rSwdmxRmInfo.fgDeliverVEos = TRUE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
        }
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}

static BOOL _SWDMX_RmResetFeeder()
{
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
        FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_RmResetFeeder Cur Fed's WPtr = 0x%08x\n", _rFeederInfo.u4WriteAddr);
    _rSwdmxRmInfo.u4CurDmxFeederRPtr = _rFeederInfo.u4WriteAddr ;

    // Update read ptr to feeder
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxRmInfo.u4CurDmxFeederRPtr));
    return TRUE;
}


BOOL _SWDMX_RmSetStrmID(UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    UNUSED(u4StrmSubID);
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxRmInfo.u4VStrmID = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxRmInfo.u4AStrmID = u4StrmID;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_RmEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{
    //BOOL fgFindATrack;
    //INT32 i;
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxRmInfo.fgEnVideo = TRUE;
            if (_rSwdmxRmInfo.fgEnVDmx == FALSE)
            {
                _SWDMX_RmInitDmx(eSWDMX_MEDIA_VIDEO, FALSE);
                _rSwdmxRmInfo.fgEnVDmx = TRUE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if (_rSwdmxRmInfo.fgEnADmx == FALSE)
            {
                _SWDMX_RmInitDmx(eSWDMX_MEDIA_AUDIO, FALSE);
                _rSwdmxRmInfo.fgEnADmx = TRUE;
            }
            /*fgFindATrack = FALSE;
            for (i = 0; i < _rSwdmxRmInfo.u1TotalATrack; i++)
            {
                if (_rSwdmxRmInfo.prAudIdxTbl[i] &&
                    _rSwdmxRmInfo.prAudIdxTbl[i]->t_strm_id.u.t_stm_rm_id == _rSwdmxRmInfo.u4AStrmID)
                {
                    _rSwdmxRmInfo.u1CurATrack = i;
                    fgFindATrack = TRUE;
                }
            }
            if (!fgFindATrack)
            {
                LOG(3, "_SWDMX_RmEnableStrm didn't find audio stream\n");
                return FALSE;
            }
            if (_rSwdmxRmInfo.u4NumSentAud != 0)
            {
                _rSwdmxRmInfo.fgChgATrack = TRUE;
            }*/
            if ((_rSwdmxRmInfo.u4PrevAStrmID != 0xFFFFFFFF) &&
                (_rSwdmxRmInfo.u4AStrmID != _rSwdmxRmInfo.u4PrevAStrmID))
            {
                _rSwdmxRmInfo.fgChgATrack = TRUE;
            }
            _rSwdmxRmInfo.u4PrevAStrmID = _rSwdmxRmInfo.u4AStrmID;
            _rSwdmxRmInfo.fgEnAudio = TRUE;
            _rSwdmxRmInfo.u1TotalATrack = 1;  // tmp solution for MW not setting this
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_RmDisableStrm(UINT8 u1StrmType)
{
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(_rSwdmxRmInfo.fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp();
                _rSwdmxRmInfo.fgVDecIsPause = FALSE;
            }
            _rSwdmxRmInfo.fgEnVideo = FALSE;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxRmInfo.fgEnAudio = FALSE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_RmInitDmx(UINT8 u1HdrType, BOOL fgEos)
{
    BOOL fgRet = FALSE;
    UINT32 u4Flag = 0;
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;

    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!_rSwdmxRmInfo.fgEnVDmx)
        {
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = VIRTUAL(prFbmPool->u4Addr);
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
            _rSwdmxRmInfo.u4VFifoSa = VIRTUAL(prFbmPool->u4Addr);
            _rSwdmxRmInfo.u4VFifoEa = VIRTUAL(prFbmPool->u4Addr + prFbmPool->u4Size);

            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

            ASSERT(_rSwdmxRmInfo.u4VDecType == ENUM_SWDMX_VDEC_RV);
            LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!_rSwdmxRmInfo.fgEnADmx)
        {
            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);

#ifdef AUD_OLDFIFO_INTF
            if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                 &au4BufEnd[1]) != AUD_OK)
#else
            if (AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
            {
                LOG(1, "_SWDMX_SetAudioStreamID Can't get audio buffers!\n");
                return FALSE;
            }

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = au4BufStart[0];
            rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
            _rSwdmxRmInfo.u4AFifoSa = VIRTUAL(rDmxMMInfo.u4BufAddr);
            _rSwdmxRmInfo.u4AFifoSz = rDmxMMInfo.u4BufSize;
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
        }
    }

    return fgRet;
}


static BOOL _SWDMX_SendRmEos(UINT8 u1HdrType)
{
    BOOL fgRet;
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    //DMX_MM_DATA_T _rRmDmxMMData;

    LOG(5, "SWDMX Send Rm Eos Hdr Type = %d\n", u1HdrType);

#ifdef __MODEL_slt__
    LOG(5, "RM SLT last pic type = 0x%X, PTS = %d\n", _u1RmSltLastPicType, _u4RmSltLastPicPts);
#endif

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (_rSwdmxRmInfo.u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            return FALSE;
        }
    }

    x_memset(&_rRmDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_RmInitDmx(u1HdrType, TRUE) == FALSE)
    {
        return FALSE;
    }

    _rRmDmxMMData.u4BufStart = _rFeederInfo.u4StartAddr;
    _rRmDmxMMData.u4BufEnd = _rFeederInfo.u4EndAddr;
    _rRmDmxMMData.u4StartAddr = _rFeederInfo.u4StartAddr;
    _rRmDmxMMData.u4FrameSize = 4;
    _rRmDmxMMData.fgEOS = TRUE;

    fgRet = DMX_MM_MoveData(u1HdrType, &_rRmDmxMMData, 500);

    if (fgRet)
    {
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
            vRmPIsr(TRUE);  // to wait until DMX has finished sending EOS
    }
    else
    {
        ASSERT(0);
    }

    return fgRet;
}


/**
*/
BOOL _SWDMX_RmStop(VOID)
{
    DMX_MM_T rPid;

    rPid.fgEnable = FALSE;

    if (_rSwdmxRmInfo.fgEnVDmx)
    {
        _rSwdmxRmInfo.fgEnVDmx = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
    }

    if (_rSwdmxRmInfo.fgEnADmx)
    {
        _rSwdmxRmInfo.fgEnADmx = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
    }

    x_memset(&_rSwdmxRmInfo, 0, sizeof(SWDMX_RM_INFO_T));

    return TRUE;
}


static BOOL _SWDMX_RmSeek(UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList, BOOL fgNoDecrement)
{
    INT32 i;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = _rSwdmxRmInfo.prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prAudKeyIdxTbl = 0;
    UINT32* pu4AudPts;

    LOG(7, "_SWDMX_RmSeek u8SeekTime = 0x%08x\n", u8SeekTime);

    if (_rSwdmxRmInfo.u4ADecType == ENUM_SWDMX_ADEC_RA)
    {
        _dwCurCookFrameInSB = 0;
        _rSwdmxRmInfo.fgCookSBPending = FALSE;
        //printf("swdmx_rm 4694 reset _dwCurCookFrameInSB\n");
    }

    if (/*!_rSwdmxRmInfo.fgKeyExist || !!! */
        !_rSwdmxRmInfo.prVidKeyIdxTbl ||
        _rSwdmxRmInfo.prVidKeyIdxTbl->ui4_number_of_entry == 0)
    {
        LOG(5, "_SWDMX_RmSeek doesn't have the key table\n");
        return FALSE;
    }

    _rSwdmxRmInfo.prCurVidKeyIdxTbl = prVidKeyIdxTbl;
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
        _rSwdmxRmInfo.u4CurSentKey = i - (fgNoDecrement ? 0 : 1);
    }
    else
    {
        _rSwdmxRmInfo.u4CurSentKey= 0;
    }

    _rSwdmxRmInfo.u4CurRepeatFrameIdx = _rSwdmxRmInfo.u4CurSentKey;

    if (prVidKeyIdxTbl != 0)
    {
        if (u8SeekTime == 0)
        {
            _rSwdmxRmInfo.au8CurDmxFilePos[RM_STRM_VDO] = _rSwdmxRmInfo.au8FileOffset[RM_STRM_VDO];
        }
        else
        {
            _rSwdmxRmInfo.au8CurDmxFilePos[RM_STRM_VDO] = 
                prVidKeyIdxTbl->ui8_base_offset +
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_relative_offset;
        }
        _rSwdmxRmInfo.u4CurVidPts =
            prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90;

        LOG(5, "Rm Seek key frame number = 0x%08x Video offset = %lld\n",
            _rSwdmxRmInfo.u4CurSentKey,
            _rSwdmxRmInfo.au8CurDmxFilePos[RM_STRM_VDO]);

        LOG(5, "Rm Seek Video pts = 0x%08x\n",
            prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurSentKey].ui4_pts * 90);
    }

    for (i = 0; (i < _rSwdmxRmInfo.u1TotalATrack) && (i < RM_MAX_AUDIO_TRACK); i++)
    {
        if (prVidKeyIdxTbl == _rSwdmxRmInfo.prVidKeyIdxTbl)
        {
            prAudKeyIdxTbl = _rSwdmxRmInfo.prAudIdxTbl[i];
            pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;

            LOG(5, "prAudKeyIdxTbl = 0x%08x\n", prAudKeyIdxTbl);
        }
        else
        {
            pu4AudPts = (UINT32*)prAudKeyIdxTbl->u.pv_idx_tbl_entry;
        }
        if (pu4AudPts)
        {
        _rSwdmxRmInfo.u4AudPts[i] =
            pu4AudPts[_rSwdmxRmInfo.u4CurSentKey];
        LOG(5, "Rm Seek Audio pts = 0x%08x\n",
            pu4AudPts[_rSwdmxRmInfo.u4CurSentKey]);
        }
    }

    _SWDMX_RmResetFeeder();

    _rSwdmxRmInfo.fgIsSeek = TRUE;
    _rSwdmxRmInfo.u4LeftBytesInFeeder = 0;
    _rSwdmxRmInfo.au8CurFilePos[RM_STRM_VDO] = _rSwdmxRmInfo.au8CurDmxFilePos[RM_STRM_VDO];

    // Set STC, temp, should update by audio (audio master)
    //STC_SetStcValue(_rSwdmxRmInfo.u4CurVidPts);

    LOG(7, "_SWDMX_RmSeek Key = %ld\n", _rSwdmxRmInfo.u4CurSentKey);

    return TRUE;
}


static BOOL _SWDMX_RmGetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData)
{
    if (!_SWDMX_ReceiveFeederAck((VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_RmGetDataFromFeeder FeederTimeOut!!!\n");
        _rSwdmxRmInfo.fgFeederInvalid = TRUE;
        return FALSE;
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
         LOG(3, "_SWDMX_RmGetDataFromFeeder  Timeout!!!\n");

#ifdef CC_DLNA_SUPPORT
        if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            _rSwdmxRmInfo.fgReqDataMiss = TRUE;
        }
        else
#endif
        {
            _rSwdmxRmInfo.fgFeederInvalid = TRUE;
        }
        return FALSE;
    }

    if ((prFeederData->u4Id + 1) != _rSwdmxRmInfo.u4FeederReqID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        _rSwdmxRmInfo.fgFeederInvalid = TRUE;
        return FALSE;
    }

    if(!prFeederData->u4WriteAddr)
    {
        LOG(3, "Feeder Ack error id= %ld\n", prFeederData->u4Id);
        _rSwdmxRmInfo.fgFeederInvalid = TRUE;
        return FALSE;      
    }
    
    _rSwdmxRmInfo.fgReqDataMiss = FALSE;

    return TRUE;
}


static BOOL _SWDMX_RmCalculateRepeatFrame(INT32 i4PlaySpeed)
{
    UINT32 u4CurKey = _rSwdmxRmInfo.u4CurSentKey;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl = _rSwdmxRmInfo.prVidKeyIdxTbl;

    if (_rSwdmxRmInfo.u4VidRepeatFrame == 0)
    {
        if (_rSwdmxRmInfo.u4CurRepeatFrameIdx != u4CurKey &&
            (_rSwdmxRmInfo.u4CurRepeatFrameIdx < prKeyIdxTbl->ui4_number_of_entry))
        {
            UINT32 u4PtsDif = 0, u4MaxRepeatFrame;

            //u4MaxRepeatFrame = (_rSwdmxRmInfo.u4VidFps * 1000 * 2)/abs(i4PlaySpeed);
            // remove thread delay when merging packets into frame, so change max repeat frame
            u4MaxRepeatFrame = (_rSwdmxRmInfo.u4VidFps * 1000 * 8)/abs(i4PlaySpeed);

            if (i4PlaySpeed > 2000)
            {
                if ((u4CurKey < (prKeyIdxTbl->ui4_number_of_entry)) &&
                    (prKeyIdxTbl->ui4_number_of_entry > RM_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = (prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurRepeatFrameIdx].ui4_pts) * 90;

                    _rSwdmxRmInfo.u4VidRepeatFrame =
                        (UINT32)((u4PtsDif * _rSwdmxRmInfo.u4VidFps)/(90 * i4PlaySpeed));
                }
                else
                {
                    _rSwdmxRmInfo.u4VidRepeatFrame = (_rSwdmxRmInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }
            else if (i4PlaySpeed < 0)
            {
                if ((prKeyIdxTbl->ui4_number_of_entry > RM_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = (prKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxRmInfo.u4CurRepeatFrameIdx].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts) * 90;

                    _rSwdmxRmInfo.u4VidRepeatFrame =
                        (UINT32)((u4PtsDif * _rSwdmxRmInfo.u4VidFps)/(90 * abs(i4PlaySpeed)));
                }
                else
                {
                    _rSwdmxRmInfo.u4VidRepeatFrame = (_rSwdmxRmInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }

            _rSwdmxRmInfo.u4VidRepeatFrame = (u4MaxRepeatFrame > _rSwdmxRmInfo.u4VidRepeatFrame) ?
                (_rSwdmxRmInfo.u4VidRepeatFrame) : u4MaxRepeatFrame;

            if ((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||
                (u4CurKey == 0))
            {
                _rSwdmxRmInfo.u4VidRepeatFrame =
                    (RM_FIX_REPEAT_FRAME/(abs(i4PlaySpeed)/1000));

                if (_rSwdmxRmInfo.u4VidRepeatFrame > RM_MAX_REPEAT_FRAME)
                {
                    _rSwdmxRmInfo.u4VidRepeatFrame = RM_MAX_REPEAT_FRAME;
                }
                else if (_rSwdmxRmInfo.u4VidRepeatFrame < RM_MIN_REPEAT_FRAME)
                {
                    _rSwdmxRmInfo.u4VidRepeatFrame = RM_MIN_REPEAT_FRAME;
                }
            }

            LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n",
                u4PtsDif, u4CurKey, _rSwdmxRmInfo.u4VidRepeatFrame);

            if (_rSwdmxRmInfo.u4VidRepeatFrame == 0)
            {
                return FALSE;
            }

            _rSwdmxRmInfo.u4CurRepeatFrameIdx = u4CurKey;
            LOG(5, "Repeat Idx = %ld\n", _rSwdmxRmInfo.u4CurRepeatFrameIdx);

            return TRUE;
        }
        else
        {
            // Force to play the first key frame
            if (_rSwdmxRmInfo.fgStartTrick)
            {
                _rSwdmxRmInfo.fgStartTrick = FALSE;
                _rSwdmxRmInfo.u4VidRepeatFrame = (_rSwdmxRmInfo.u4VidFps * 1000)/abs(i4PlaySpeed);
                return TRUE;
            }

            return FALSE;
        }
    }

    _rSwdmxRmInfo.u4VidRepeatFrame--;
    return TRUE;
}


BOOL _SWDMX_RmAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if (_rSwdmxRmInfo.fgEnVideo)
        {
            LOG(5, "SWDMX_RmAudioCb Audio is ready\n");

            if (_rSwdmxRmInfo.u4TriggerAudPts != 0xFFFFFFFF)
            {
                VDP_TriggerAudReceive(_rSwdmxRmInfo.u4TriggerAudPts);
                LOG(5, "SWDMX_RmAudioCb VDP Trigger PTS = 0x%08x\n",
                    _rSwdmxRmInfo.u4TriggerAudPts);
                _rSwdmxRmInfo.u4TriggerAudPts = 0xFFFFFFFF;
            }
            else
            {
                LOG(5, "Trigger audio by _SWDMX_RmAudioCb\n");
                AUD_MMAoutEnable(0, TRUE);
                STC_StartStc();
            }

            if(_rSwdmxRmInfo.fgVDecIsPause)
            {
                _SWDMX_RmVideoWakeUp();
                _rSwdmxRmInfo.fgVDecIsPause = FALSE;
            }
        }
        else
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_RmAudioCb\n");
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc();
        }
        _rSwdmxRmInfo.fgAudIsReady = TRUE;
    }
    else
    {
        // impossible
        LOG(1, "_SWDMX_RmAudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }

    return TRUE;
}

static void _SWDMX_RmVideoWakeUp(void)
{
    LOG(5, "_SWDMX_RmVideoWakeUp\n");
    VDP_SetPauseMm(0, FALSE);
    switch(_rSwdmxRmInfo.u4VDecType)
    {
        case ENUM_SWDMX_VDEC_RV:
            VDEC_Play(ES0, VDEC_FMT_RV);
            break;
        default:
            LOG(1, "SWDMX_RmAudioCb Unknown Video Type\n");
            ASSERT(0);
            break;
    }
}
