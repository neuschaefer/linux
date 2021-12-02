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
 * $RCSfile: vdec_type.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_push.h
 *  This header file contains common constant and type definitions of MPV.
 */

#ifndef VDEC_PUSH_H
#define VDEC_PUSH_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdec_drvif.h"
#include "vdec_push.h"
LINT_EXT_HEADER_BEGIN
#include "drv_dbase.h"
#include "x_typedef.h"
#include "x_csemaphore.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VDEC_INPUT_DATA_QUEUE_NUMBER (5)
#define VDEC_INPUT_AU_QUEUE_NUMBER (100)
#define VDEC_BUF_COUNT      (5)
#define VDEC_MAX_DECODER (1) //  = VDEC_MAX_ES
#define VDEC_PUSH_MSG_Q_SIZE       (VDEC_INPUT_DATA_QUEUE_NUMBER + 10)
#define VDEC_PUSH_STACK_SIZE       (8172)
#ifdef MPV_SEMI_HOST // for FPGA, lower to cli thread. We could key in command
#define VDEC_PUSH_THREAD_PRIORITY            255
#else
#define VDEC_PUSH_THREAD_PRIORITY            60
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct {
    BOOL   fgDataSeqHdr;      // contain sequence header
    UCHAR  ucDataPicType;     // picture type
    UINT32 u4DataPTS;         // PTS
    UINT32 u4DataTag;         // caller's tag
    UINT32 u4DataAddr;        // data address
    UINT32 u4DataSize;        // data size
} VDEC_DATA_INTO_T;


typedef struct {
    BOOL fgInited;
    BOOL fgIsRing;
    BOOL fgIsPartial;   // not complete decode data

    // fifo
    UINT32 u4FifoStart;     // ring buffer start address
    UINT32 u4FifoEnd;       // ring buffer end address
    UINT32 u4FifoRpt;           // ring buffer read pointer
    UINT32 u4FifoWpt;           // ring buffer write pointer
    UINT32 u4FifoRptParse;      // ring buffer read pointer for parsing
    UINT32 u4FifoRptParseStart; // ring buffer read pointer for parsing
    UINT32 u4FifoRptValue;      // ring buffer read pointer value
    UINT32 u4FifoRptValueLast;      // ring buffer read pointer value
    UCHAR  ucFifoRptPicType;    // ring buffer read pointer value
    UCHAR  ucFifoRptPicTypeNext;    // ring buffer read pointer value
    BOOL   fgFifoRptSeqHdr;     // ring buffer read pointer value
    BOOL   fgFirst;             // ring buffer read pointer value
    BOOL   fgPicture;           // ring buffer read pointer value

    // fifo2

    // input data
    VDEC_DATA_INTO_T arData[VDEC_INPUT_DATA_QUEUE_NUMBER];
    UINT32 u4DataLastPTS;
    UINT32 u4DataLast2PTS;
    UINT32 u4DataReadIdx;
    UINT32 u4DataWriteIdx;
    UINT32 u4DataNum;
    BOOL   fgEos;

    // AU
    BOOL fgAUBuf;
    VDEC_AU_INTO_T rAUBuffer;
    VDEC_AU_INTO_T arAU[VDEC_INPUT_AU_QUEUE_NUMBER];
    UINT32 u4AUReadIdx;
    UINT32 u4AUWriteIdx;
    UINT32 u4AUNum;

    // for callback
    VDEC_PUSH_CB_T fnCb;
} VDEC_INPUT_STREAM_T;

typedef struct
{
    BOOL        fgUsed;
    UINT32      u4BufAddr;
    UINT32      u4BufSize;
} VDEC_BUF_INFO_T;

typedef struct {
    //BOOL fgGoDead;
    BOOL fgBytesReady;
    BOOL fgAUReady;
    BOOL fgFrameReady;
    BOOL fgEnDmx;
    BOOL fgNonFirst;
    BOOL fgIsRv8;
    UINT8 u1DmxId;
    UINT8 u1DmxPid;
    UINT8 ucEsId;
    UINT8 ucHwId;
    UINT16 u2MstCount;
    UINT32 u1StartCodeLenMinus1;
    UINT32 u4CurStopId;
    UINT32 u4PreviousStopId;
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4AllocBufCnt;
    ENUM_VDEC_FRAME_RATE_T eFrameRate;
    
    HANDLE_T hThread;
    HANDLE_T hMsgQ;
    VDEC_MSG_INTO_T rMsg;
    ENUM_VPUSH_STATE_T eCurStateSync;
    ENUM_VPUSH_STATE_T eCurState;
    ENUM_VPUSH_STATE_T ePreviousState;
    ENUM_VDEC_FMT_T eFmt;
    ENUM_VDEC_PUSH_FMT_T ePushFmt;
    VDEC_INPUT_STREAM_T rInpStrm;
    VDEC_BUF_INFO_T arBufInfo[VDEC_BUF_COUNT];
    BOOL fgInputBufReady;   //JPEG only
} VDEC_T;

typedef struct {
    BOOL fgInited;
    BOOL fgOccupied[VDEC_MAX_DECODER];
    VDEC_T arDec[VDEC_MAX_DECODER];
} VDEC_DECODER_T;

/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
extern BOOL _VPUSH_SetupFifoBuffer(VOID* prdec);
extern BOOL _VPUSH_PutDataDone(VOID* prdec, UINT32 u4Tag);
extern BOOL _VPUSH_Decode(VOID* prdec);
extern UCHAR _VPUSH_FbgId2EsId(UCHAR ucFbgId);
extern BOOL _VPUSH_B2R(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgIsRsIn, BOOL fgIsRsOut);
extern BOOL _VPUSH_R2B(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgIsRsIn, BOOL fgIsRsOut);
#endif // VDEC_PUSH_H

