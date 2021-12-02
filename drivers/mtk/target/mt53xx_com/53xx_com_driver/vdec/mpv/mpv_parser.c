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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mpv_parser.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_parser.c
 *  This file contains header and closed caption parsing of MPV.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "mpv_if.h"
#include "mpv_debug.h"
#include "vdec_drvif.h"
#include "mpv_parser.h"
#include "mpv_decode.h"
#include "mpv_hw.h"
#include "mpv_config.h"
#include "mpv_errcode.h"
#ifdef CC_MT5392B
#include "vdp_drvif.h"
#endif
#ifdef MPV_VIRTUAL_CC
#include "mpv_virtualcc.h"
#endif
#include "fbm_drvif.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_mid.h"
#include "x_drv_cli.h"
#include "x_hal_926.h"
LINT_EXT_HEADER_END


// test
//#define AUTO_QMATRIX
//#define FETCH_WORKAROUND
//#define VDP_WORKAROUND
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// start code
#define PICTURE_START_CODE          0x100
#define SLICE_START_CODE_MIN      0x101
#define SLICE_START_CODE_MAX      0x1AF
#define USER_DATA_START_CODE     0x1B2
#define SEQUENCE_START_CODE       0x1B3
#define SEQUENCE_ERROR_CODE       0x1B4
#define EXTENSION_START_CODE      0x1B5
#define SEQUENCE_END_CODE           0x1B7
#define GROUP_START_CODE             0x1B8
#define SYSTEM_START_CODE_MIN   0x1B9
#define SYSTEM_START_CODE_MAX   0x1FF

// extension start code IDs
#define SEQUENCE_EXTENSION_ID                        1
#define SEQUENCE_DISPLAY_EXTENSION_ID        2
#define QUANT_MATRIX_EXTENSION_ID               3
#define COPYRIGHT_EXTENSION_ID                      4
#define SEQUENCE_SCALABLE_EXTENSION_ID      5
#define PICTURE_DISPLAY_EXTENSION_ID           7
#define PICTURE_CODING_EXTENSION_ID            8
#define PICTURE_SPATIAL_SCALABLE_EXTENSION_ID     9
#define PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID  10

#define MAX_RETRY_COUNT            30
#define MAX_USER_DATA_SIZE      200
#define MAX_SEARCH_USRDATA_CNT  305

#define LEVEL_ID_LOW        10
#define LEVEL_ID_MAIN       8
#define LEVEL_ID_HIGH_1440  6
#define LEVEL_ID_HIGH       4

//For Closed Caption
#define ATSC_IDENTIFIER  0x47413934
#define AFD_IDENTIFIER    0x44544731
#define EIA_CC    0x03
#define SCTE_CC  0x1

#define MPEG1_MAX_HSIZE  768
#define MPEG1_MAX_VSIZE  576
#define MPEG1_MAX_MB  396

#define MPEG_MAX_HSIZE  1920
#define MPEG_MAX_VSIZE  1088

#ifdef MPV_VIRTUAL_CC
#define VIRTUAL_CC_SIZE 12000
#endif

//#define LOCK_TIMEOUT // use lock currently....ilii
#ifndef LOCK_TIMEOUT
#define SEARCH_TIMEOUT    0
#define SEARCH_FINISH     1
#endif
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// Sequence Hdr
typedef struct _MPEG2_SEQ_HDR_FIELD_T
{
    /* 32 Bit */
    UINT32            u4FrameRate        :    4;
    UINT32            u4AspectRatio      :    4;
    UINT32            u4VSize                :    12;
    UINT32            u4HSize                :    12;

    /* 32 Bit */
    UINT32            fgLoadNonIntra    :    1;
    UINT32            fgLoadIntra          :    1;
    UINT32            fgContrained        :    1;
    UINT32            u4VbvSize            :    10;
    UINT32            fgMark                  :    1;
    UINT32            u4BitRate              :    18;
} MPEG2_SEQ_HDR_FIELD_T;

typedef union _MPEG2_SEQ_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                        au4Reg[2];
    MPEG2_SEQ_HDR_FIELD_T        rField;
} MPEG2_SEQ_HDR_UNION_T;

// Sequence Externsion Hdr
typedef struct _MPEG2_SEQ_EXT_HDR_FIELD_T
{
    /* 32 Bit */
    UINT32            fgMark                  :    1;
    UINT32            u4BitRateExt         :    12;
    UINT32            u4VSizeExt            :    2;
    UINT32            u4HSizeExt            :    2;
    UINT32            u4ChromaFmt       :    2;
    UINT32            fgPrgsSeq             :    1;
    UINT32            u4LevelId             :    4;
    UINT32            u4ProfileId           :     4;
    UINT32            u4ExtId                :    4;

    /* 16 Bit */
    UINT32                                            :    16;
    UINT32            u4FrameRateExtD   :    5;
    UINT32            u4FrameRateExtN   :    2;
    UINT32            fgLowDelay             :    1;
    UINT32            u4VbvSizeExt          :    8;
} MPEG2_SEQ_EXT_HDR_FIELD_T;

typedef union _MPEG2_SEQ_EXT_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                        au4Reg[2];
    MPEG2_SEQ_EXT_HDR_FIELD_T    rField;
} MPEG2_SEQ_EXT_HDR_UNION_T;

// Sequence Display Externsion Hdr
typedef struct _MPEG2_SEQ_DISP_EXT_HDR_FIELD_T
{
    /* 32/8 Bit */
    UINT32            u4MatrixCoef        :    8;
    UINT32            u4TrasferChar      :    8;
    UINT32            u4ColorPrimary    :    8;
    UINT32            fgColorDesc          :    1;
    UINT32            u4VideoFmt          :    3;
    UINT32            u4ExtId                :    4;

    /* 29 Bit */
    UINT32                                         :    3;
    UINT32            u4DispVSize         :    14;
    UINT32            fgMark                  :    1;
    UINT32            u4DispHSize         :    14;
} MPEG2_SEQ_DISP_EXT_HDR_FIELD_T;

typedef union _MPEG2_SEQ_DISP_EXT_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                                                au4Reg[2];
    MPEG2_SEQ_DISP_EXT_HDR_FIELD_T    rField;
} MPEG2_SEQ_DISP_EXT_HDR_UNION_T;


// GOP Hdr
typedef struct _MPEG2_GOP_HDR_FIELD_T
{
    /* 27 Bit */
    UINT32                                          :    5;
    UINT32            fgBrokenLink        :    1;
    UINT32            fgClosedGop         :    1;
    UINT32            u4Picture              :    6;
    UINT32            u4Second              :    6;
    UINT32            fgMark                  :    1;
    UINT32            u4Mintue               :    6;
    UINT32            u4Hour                  :    5;
    UINT32            fgDropFrame        :    1;
} MPEG2_GOP_HDR_FIELD_T;

typedef union _MPEG2_GOP_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                                 au4Reg[1];
    MPEG2_GOP_HDR_FIELD_T    rField;
} MPEG2_GOP_HDR_UNION_T;

// Picture Hdr
typedef struct _MPEG2_PIC_HDR_FIELD_T
{
    /* 29 Bit */
    UINT32                                           :    3;
    UINT32            u4VbvDelay            :    16;
    UINT32            u4PicType               :    3;
    UINT32            u4TemporalRef       :    10;
} MPEG2_PIC_HDR_FIELD_T;

typedef union _MPEG2_PIC_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                                au4Reg[1];
    MPEG2_PIC_HDR_FIELD_T    rField;
} MPEG2_PIC_HDR_UNION_T;

// P PIC Hdr
typedef struct _MPEG2_P_PIC_HDR_FIELD_T
{
    /* 4 Bit */
    UINT32                                               :    28;
    UINT32            u4ForwardFCode        :    3;
    UINT32            fgFullPelForward        :    1;
} MPEG2_P_PIC_HDR_FIELD_T;

typedef union _MPEG2_P_PIC_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                                   au4Reg[1];
    MPEG2_P_PIC_HDR_FIELD_T    rField;
} MPEG2_P_PIC_HDR_UNION_T;

// B PIC Hdr
typedef struct _MPEG2_B_PIC_HDR_FIELD_T
{
    /* 8 Bit */
    UINT32                                                  :    24;
    UINT32            u4BackwardFCode        :    3;
    UINT32            fgFullPelBackward        :    1;
    UINT32            u4ForwardFCode          :    3;
    UINT32            fgFullPelForward          :    1;
} MPEG2_B_PIC_HDR_FIELD_T;

typedef union _MPEG2_B_PIC_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                    au4Reg[1];
    MPEG2_B_PIC_HDR_FIELD_T    rField;
} MPEG2_B_PIC_HDR_UNION_T;

// Picture Externsion Hdr
typedef struct _MPEG2_PIC_EXT_HDR_FIELD_T
{
    /* 32 Bit */
    UINT32            fgChroma420            :    1;
    UINT32            fgRff                          :    1;
    UINT32            fgAlternateScan        :    1;
    UINT32            fgIntraVlc                  :    1;
    UINT32            fgQScale                    :    1;
    UINT32            fgConcealmentMV      :    1;
    UINT32            fgFramePred             :    1;
    UINT32            fgTff                           :    1;
    UINT32            u4PicStruture             :    2;
    UINT32            u4IntraDcPrecision    :    2;
    UINT32            u4FCode11                  :    4;
    UINT32            u4FCode10                  :    4;
    UINT32            u4FCode01                  :    4;
    UINT32            u4FCode00                  :    4;
    UINT32            u4ExtId                       :    4;

    /* 22 Bit */
    UINT32                                                :    10;
    UINT32            u4SubCarrierPhase     :    8;
    UINT32            u4BurstAmplitude       :    7;
    UINT32            fgSubCarrier               :    1;
    UINT32            u4FieldSequence        :    3;
    UINT32            fgVAxis                       :    1;
    UINT32            fgCompositeDisp        :    1;
    UINT32            fgPrgsFrame              :    1;
} MPEG2_PIC_EXT_HDR_FIELD_T;

typedef union _MPEG2_PIC_EXT_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                                       au4Reg[2];
    MPEG2_PIC_EXT_HDR_FIELD_T    rField;
} MPEG2_PIC_EXT_HDR_UNION_T;

// Picture Display Externsion Hdr
typedef struct _MPEG2_PIC_DISP_EXT_HDR_FIELD_T
{
    /* 17 Bit */
    UINT32                                     :    15;
    UINT32            fgMark              :    1;
    UINT32            u4Offset            :    16;
} MPEG2_PIC_DISP_EXT_HDR_FIELD_T;

typedef union _MPEG2_PIC_DISP_EXT_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                            au4Reg[1];
    MPEG2_PIC_DISP_EXT_HDR_FIELD_T    rField;
} MPEG2_PIC_DISP_EXT_HDR_UNION_T;

// CopyRight Externsion Hdr
typedef struct _MPEG2_COPYRIGHT_EXT_HDR_FIELD_T
{
    /* 22 Bit */
    UINT32                                         :    10;
    UINT32            fgMark1                :    1;
    UINT32                                         :    7;
    UINT32            fgOriginal             :    1;
    UINT32            u4CopyRightId     :    8;
    UINT32            fgCopyFlag          :    1;
    UINT32            u4ExtId                :    4;

    /* 21 Bit */
    UINT32                                         :    11;
    UINT32            fgMark2                :    1;
    UINT32            u4CopyRightNs1   :    20;

    /* 23 Bit */
    UINT32                                         :    9;
    UINT32            fgMark3                :    1;
    UINT32            u4CopyRightNs2   :    22;

    /* 22 Bit */
    UINT32                                         :    10;
    UINT32            u4CopyRightNs3   :    22;
}MPEG2_COPYRIGHT_EXT_HDR_FIELD_T;

typedef union _MPEG2_COPYRIGHT_EXT_HDR_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UINT32                                                   au4Reg[4];
    MPEG2_COPYRIGHT_EXT_HDR_FIELD_T    rField;
} MPEG2_COPYRIGHT_EXT_HDR_UNION_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define INVERSE_ENDIAN(value)        \
        (((value & 0xFF) << 24) + ((value & 0xFF00) << 8) + ((value & 0xFF0000) >> 8) + \
        ((value & 0xFF000000) >> 24))

#define CCSIZE(wp, rp, bufsize) \
        (((wp) >= (rp)) ? ((wp) - (rp)) : (((bufsize) + (wp)) - (rp)))


#define INVERSE_BIT_ORDER_8(value)      \
{                                                                \
    UCHAR ucTemp = 0;                                            \
    INT32 i4Idx;                                                  \
    for (i4Idx = 0; i4Idx < 4; i4Idx ++)                               \
    {                                                            \
        ucTemp |= ((value & (1 << i4Idx)) << (((4 - i4Idx) * 2) - 1));     \
    }                                                            \
    for(i4Idx = 4; i4Idx < 8; i4Idx++)                               \
    {                                                            \
        ucTemp |= ((value & (1 << i4Idx)) >> (((i4Idx - 4) * 2) + 1));     \
    }                                                            \
    value = ucTemp;                                              \
}

#define FBM_MEMSET(addr, value, len)    (VERIFY(x_memset((addr), value, len) == (addr)))


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UCHAR _ucLastPicStruct;
static volatile UCHAR _ucTimeout[MPV_MAX_VLD] = {0};
static UINT32 _au4CurrentQMatrixId[MPV_MAX_VLD];
static UINT32 _au4QMatrixCounter[MPV_MAX_VLD];
static VDEC_INFO_T *_prVdecInfo;
static MPV_ES_INFO_T *_prEsInfo[MPV_MAX_ES];
static VDEC_ES_INFO_T *_prVdecEsInfo[VDEC_MAX_ES];
static HANDLE_T _hVldTimer[MPV_MAX_VLD] = {0};
static MPV_USERDATA_CB_T _rUserData;
static MPV_USERDATA_CB_T _rUserDataIPAtsc;
static MPV_USERDATA_CB_T _rUserDataIPScte;
static MPV_USERDATA_CB_T _rUserDataIPAfd;
static UCHAR _aucUserDataIPBufAtsc[MAX_USERDATA_SIZE];
static UCHAR _aucUserDataIPBufScte[MAX_USERDATA_SIZE];
static UCHAR _aucUserDataIPBufAfd[MAX_USERDATA_SIZE];
static UINT32 _u4RetrieveNsAtsc;
static UINT32 _u4RetrieveNsScte;
static UINT32 _u4RetrieveNsAfd;

#ifndef LOCK_TIMEOUT
static HANDLE_T _hHWSearchTimer[MPV_MAX_VLD];
static HANDLE_T _hHWSearchMsgQ[MPV_MAX_VLD];
#else
static HANDLE_T _ahSearchFinSema[MPV_MAX_VLD];
#endif

#ifdef MPV_STATISTICS
extern HAL_TIME_T _rSwStartDecodeTime[MPV_MAX_VLD];
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifndef LOCK_TIMEOUT
static void _HWSearchFlushMsgQ(UCHAR ucMpvId)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;

    if(ucMpvId >= MPV_MAX_VLD)
    {
        ucMpvId = (MPV_MAX_VLD-1);
    }

    zMsgSize = sizeof(INT32);

    while (x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &_hHWSearchMsgQ[ucMpvId], 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_hHWSearchMsgQ[ucMpvId], &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
}

static void _HWSearchTimerTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucMpvId;
    UINT32 u4DecFin;

    ucMpvId = (UCHAR)(UINT32)pv_tag;

    if(ucMpvId >= MPV_MAX_VLD)
    {
        ASSERT(ucMpvId < MPV_MAX_VLD);
        ucMpvId = (MPV_MAX_VLD-1);
    }

    u4DecFin = SEARCH_TIMEOUT;
    VERIFY(x_msg_q_send(_hHWSearchMsgQ[ucMpvId], (void *)&u4DecFin, sizeof(UINT32), 255) == OSR_OK);

    UNUSED(pt_tm_handle);
}
#endif


static UINT32 _GetBitStream(UCHAR ucMpvId, UCHAR ucEsId, UCHAR ucShiftBit)
{
    UINT32 u4RegVal;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    _prEsInfo[ucEsId]->ucBitCount += ucShiftBit;
    u4RegVal = _MPV_VldGetBitS(ucMpvId, ucShiftBit);

    return(u4RegVal);
}


static void _ResetBitStream(UCHAR ucMpvId, UCHAR ucEsId)
{
    UCHAR ucReadPtrRes;
    UCHAR ucReadPtrResSb;
    UINT32 u4VldReadPtr;
    UINT32 u4Datain;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return;
    }

    u4VldReadPtr = _prVdecEsInfo[ucEsId]->u4VldReadPtr;

    // if u4VldReadPtr it NOT 128 bits (16 byte) aligned, remove those bits
    ucReadPtrRes = (UCHAR)(u4VldReadPtr & 0x0000000F);

    // remove 32 bit each time
    while (ucReadPtrRes > 0)
    {
        ucReadPtrResSb = (UCHAR)((ucReadPtrRes > 4) ? 32 : (ucReadPtrRes << 3));
        u4Datain = _GetBitStream(ucMpvId, ucEsId, ucReadPtrResSb);
        UNUSED(u4Datain);
        ucReadPtrRes -= (ucReadPtrResSb >> 3);
    }

    _prEsInfo[ucEsId]->ucBitCount = 0;
}


static BOOL _HwNextStartCodeInt(UCHAR ucMpvId, UCHAR ucEsId, UINT32 u4Type)
{
#ifndef LOCK_TIMEOUT
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;
#else
    INT32 i4Ret = 0;
#endif
    UCHAR ucRmvBit = 0;
    UINT32 u4NextStart;
    UINT32 ucBitCount;
    UINT32 u4DataIn;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return FALSE;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return FALSE;
    }

    //Let stream byte aligned
    ucBitCount = _prEsInfo[ucEsId]->ucBitCount ;
    ucRmvBit = (UCHAR)(8 - (ucBitCount % 8));
    if (ucRmvBit == 8)
    {
        ucRmvBit = 0;
    }

    u4NextStart = _GetBitStream(ucMpvId, ucEsId, ucRmvBit);

    switch (u4Type)
    {
    case PIC_START_CODE:
        if (u4NextStart == PICTURE_START_CODE)
        {
            return TRUE;
        }
        break;

    case SLICE_START_CODE:
        if (u4NextStart == SLICE_START_CODE_MIN)
        {
            return TRUE;
        }
        break;

    case USRDATA_START_CODE:
        if (u4NextStart == USER_DATA_START_CODE)
        {
            return TRUE;
        }
        break;

    case SEQ_START_CODE:
        if (u4NextStart == SEQUENCE_START_CODE)
        {
            return TRUE;
        }
        break;

    case EXT_START_CODE:
        if (u4NextStart == EXTENSION_START_CODE)
        {
            return TRUE;
        }
        break;

    case GOP_START_CODE:
        if (u4NextStart == GROUP_START_CODE)
        {
            return TRUE;
        }
        break;

    case ALL_START_CODE:
        if ((u4NextStart >> 8) == 1)
        {
            return TRUE;
        }
        break;

    default:
        LOG(1, "HWSearchCode Type W\n");
        break;

    }

    #ifndef LOCK_TIMEOUT
    _HWSearchFlushMsgQ  (ucMpvId);
    #else
    i4Ret = x_sema_lock(_ahSearchFinSema[u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    #endif

    _MPV_SearchStartCodeInt(ucMpvId, u4Type);

    #ifndef LOCK_TIMEOUT
    zMsgSize = sizeof(INT32);
    VERIFY(x_timer_start(_hHWSearchTimer[ucMpvId], MPV_WAIT_SEARCH_TIME, X_TIMER_FLAG_ONCE,
            _HWSearchTimerTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize, &(_hHWSearchMsgQ[ucMpvId]), 1,
        X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hHWSearchTimer[ucMpvId]) == OSR_OK);
    #else
    i4Ret = x_sema_lock_timeout(_ahSearchFinSema[ucMpvId], MPV_WAIT_SEARCH_TIME);
    #endif

    _MPV_DisableSearchMode(ucMpvId);

    #ifndef LOCK_TIMEOUT
    if(u4DecFin == SEARCH_TIMEOUT)
    #else
    if(i4Ret != OSR_OK)
    #endif
    {
        LOG(3, "HW Search Start Code (INT) Timeout! 0x%x\n", _MPV_VldRead32(ucMpvId, RO_VLD_VRPTR));
        return FALSE;
    }

    u4DataIn = _MPV_VldRead32(ucMpvId, RO_VLD_BARL);
    //LOG(3, "0x%x\n", u4DataIn);

    if ((u4DataIn >> 8) == 0x1)
    {
        LOG(7, "0x%x, 0x%x\n", u4DataIn, _MPV_VldRead32(ucMpvId, RO_VLD_VRPTR));

        return TRUE;
    }
    else
    {
        LOG(3, "HW Search Start Code (INT) Fail! 0x%x\n", _MPV_VldRead32(ucMpvId, RO_VLD_VRPTR));
        return FALSE;
    }

}


static VOID _NextStartCodeTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucMpvId;
    UINT32 u4Idx;

    ucMpvId = (UCHAR)(UINT32)pv_tag;

    if (ucMpvId < MPV_MAX_VLD)
    {
        _ucTimeout[ucMpvId] = 1;
    }
    else
    {
        ASSERT(ucMpvId < MPV_MAX_VLD);

        for (u4Idx = 0; u4Idx < MPV_MAX_VLD; u4Idx++)
        {
            _ucTimeout[u4Idx] = 1;
        }
    }

    UNUSED(pt_tm_handle);
}


static BOOL _NextStartCode(UCHAR ucMpvId, UCHAR ucEsId)
{
    UCHAR ucRmvBit = 0;
    UINT32 u4NextStart;
    UINT32 ucBitCount;
    UINT32 u4SearchCnt = 0;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return FALSE;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return FALSE;
    }

    //Let stream byte aligned
    ucBitCount = _prEsInfo[ucEsId]->ucBitCount ;
    ucRmvBit = (UCHAR)(8 - (ucBitCount % 8));
    if (ucRmvBit == 8)
    {
        ucRmvBit = 0;
    }

    u4NextStart = _GetBitStream(ucMpvId, ucEsId, ucRmvBit);

    _ucTimeout[ucMpvId] = 0;

    VERIFY(x_timer_start(_hVldTimer[ucMpvId], 500, X_TIMER_FLAG_ONCE,
        _NextStartCodeTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);

    while (_ucTimeout[ucMpvId] == 0)
    {
        //check until start code 0x000001XX
        if ((u4NextStart >> 8) == 1)
        {
            VERIFY(x_timer_stop(_hVldTimer[ucMpvId]) == OSR_OK);
            return TRUE;
        }
        u4NextStart = _GetBitStream(ucMpvId, ucEsId, 8);
        u4SearchCnt++;

        if (!(u4SearchCnt % 100))
        {
            x_thread_delay(1);
        }
    }

    LOG(6, "_NextStartCode Rptr 0x%x, Wptr 0x%x\n", _MPV_VldRead32(ucMpvId, RO_VLD_VRPTR),
            _MPV_VldRead32(ucMpvId, RO_VLD_VWPTR));

    // timeout
    return FALSE;
}


static BOOL _NextStartCodeWithCheck(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4Datain;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return FALSE;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return FALSE;
    }

#ifndef MPV_VIRTUAL_PRS

    _MPV_DecReset(ucMpvId, ucEsId);
    if (!_MPV_VldInitBarrelShifter(ucMpvId, _prVdecEsInfo[ucEsId]->u4VldReadPtr))
    {
        return FALSE;
    }

    _ResetBitStream(ucMpvId, ucEsId);

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

    if	((u4Datain != SEQUENCE_START_CODE) &&
		(u4Datain != GROUP_START_CODE) &&
		(u4Datain != PICTURE_START_CODE))
    {
        return FALSE;
    }
#else
    UINT32 u4Retry = 0;

    if (!_NextStartCode(ucMpvId, ucEsId))
    {
        return FALSE;
    }
    else
    {
         u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

        while ((u4Datain != SEQUENCE_START_CODE) && (u4Datain != GROUP_START_CODE) &&
                  (u4Datain !=PICTURE_START_CODE))
        {
        LOG(1, "0x%x\n", u4Datain);
            if (++u4Retry > MAX_RETRY_COUNT)
            {
            LOG(1, "MAX_RETRY_COUNT\n");
                return FALSE;
            }
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
        }
    }
#endif

    return TRUE;
}


static BOOL _NextSliceHdr(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4NextStart;
    UINT32 ucBitCount;
    UCHAR ucRmvBit =0;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return FALSE;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return FALSE;
    }

    //Let stream byte aligned
    ucBitCount = _prEsInfo[ucEsId]->ucBitCount ;
    ucRmvBit = (UCHAR)(8 - (ucBitCount % 8));
    if (ucRmvBit == 8)
    {
        ucRmvBit = 0;
    }

    u4NextStart = _GetBitStream(ucMpvId, ucEsId, ucRmvBit);

    _ucTimeout[ucMpvId] = 0;

    VERIFY(x_timer_start(_hVldTimer[ucMpvId], 100, X_TIMER_FLAG_ONCE,
        _NextStartCodeTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);

    while (_ucTimeout[ucMpvId] == 0)
    {
        if ((u4NextStart >= SLICE_START_CODE_MIN) &&
            (u4NextStart <= SLICE_START_CODE_MAX))
        {
            VERIFY(x_timer_stop(_hVldTimer[ucMpvId]) == OSR_OK);
            return TRUE;
        }
        u4NextStart = _GetBitStream(ucMpvId, ucEsId, 8);
    }

    // timeout
    return FALSE;
}



static BOOL _HwNextSliceStartCode(UCHAR ucMpvId, UCHAR ucEsId, UINT32 u4Timeout)
{
	UINT32 u4NextStart;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return FALSE;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return FALSE;
    }

	_ucTimeout[ucMpvId] = 0;
	VERIFY(x_timer_start(_hVldTimer[ucMpvId], u4Timeout, X_TIMER_FLAG_ONCE,
	    _NextStartCodeTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);

	_MPV_VldSearchStartCode(ucMpvId);

	while (_ucTimeout[ucMpvId] == 0)
	{
		if (_MPV_VldSearchStartCodeFin(ucMpvId) == 1)
		{
			VERIFY(x_timer_stop(_hVldTimer[ucMpvId]) == OSR_OK);
			_ucTimeout[ucMpvId] = 1;
		}
	}

	u4NextStart = _GetBitStream(ucMpvId, ucEsId, 0);

	if ((u4NextStart >= SLICE_START_CODE_MIN) && (u4NextStart <= SLICE_START_CODE_MAX))
	{
	    return TRUE;
	}

	return FALSE;
}



static UINT32 _DetectSliceQp(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4Datain;
    UINT32 u4QpSum;
    UINT32 u4QpAvg;
    UINT32 u4SliceIdx;
    UINT32 u4Finish;

    u4QpAvg = 0;
    u4QpSum = 0;
    u4Finish = 0;
    u4SliceIdx = 0;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    while (u4Finish == 0)
    {
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 32); //slice start code

         u4QpSum += (u4Datain & 0xF8000000) >> 27;

        if (!_HwNextSliceStartCode(ucMpvId, ucEsId, 100))
        {
            u4Finish = 1;
        }
        u4SliceIdx++;

        if (u4SliceIdx > MPV_SLICE_QP_THRSD)
        {
            u4Finish = 1;
        }
    }

    if (u4SliceIdx)
    {
        u4QpAvg = u4QpSum/u4SliceIdx;
    }

    LOG(10, "Slice(%d) Avg QP %d \n", u4SliceIdx, u4QpAvg);

    return u4QpAvg;

}


#ifdef MPV_VIRTUAL_CC
static void _GetVirtualCc(UCHAR ucEsId)
{
    static UINT32 u4CcIdx = 0;
    UCHAR *pucCcAddr;
    UINT32 u4CcBufSize;
    UINT32 u4CcLen;
    UINT32 u4Idx;
    UINT32 u4Ptr;
    //MPV_ES_INFO_T *prEsInfo;
    MPV_CC_T *prMpvCc;
    CC_INFO_T *prCcInfo;
    CC_BUF_INFO_T *prCcBufInfo;
    INT32 i4Ret;


    LOG(6, "_GetVirtualCc\n");

    u4CcLen = 80;

    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);
    prCcInfo = &(prMpvCc->rCcInfo);
    prCcBufInfo = &(prMpvCc->rCcBufInfo);
    pucCcAddr = prMpvCc->pucCcBuf;

    u4CcBufSize = CCSIZE(prCcBufInfo->u4Wptr,
                prCcBufInfo->u4Rptr, prMpvCc->u4CcBufSize);

    if ((u4CcBufSize + u4CcLen) > (prMpvCc->u4CcBufSize))
    {
        if (prMpvCc->pfCcNfyFct)
        {
            prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                MPV_CC_COND_ERROR, (void*)MPV_CC_ERR_BUFF_OVER_FLOW);

            LOG(3, "CC Bufer Overflow\n");
        }
    }
    else
    {
        for (u4Idx = 0; u4Idx < u4CcLen; u4Idx++)
        {
            u4Ptr = (prCcBufInfo->u4Wptr+u4Idx) % prMpvCc->u4CcBufSize;
            pucCcAddr[u4Ptr] = _aucVirtualCcBuf[u4CcIdx];

            if (++u4CcIdx == VIRTUAL_CC_SIZE)
            {
                u4CcIdx = 0;
            }
        }

        prCcInfo->fgIsScte= FALSE;
        prCcInfo->fgBottomFirst = FALSE;
        prCcInfo->u4Size = u4CcLen;
        prCcInfo->u8Pts = _prVdecEsInfo[ucEsId]->u4PTS;
        prCcInfo->u4BufRptr = prCcBufInfo->u4Wptr;
        prCcBufInfo->u4Wptr = u4Ptr + 1;

        i4Ret = x_msg_q_send(prMpvCc->hCcQueue,
            (void *)prCcInfo, sizeof(CC_INFO_T), 255);

        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));

        if ((prMpvCc->pfCcNfyFct) && (i4Ret == OSR_OK))
        {
            LOG(10, "Notify Virtual Cc\n");
            prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                MPV_CC_COND_DATA_ARRIVAL, (void*)(prCcInfo->u4Size));
        }
        else if(i4Ret == OSR_TOO_MANY)
        {
            LOG(0, "Notify Virtual Cc Drop\n");
        }
    }
}
#endif


#if 0
static INT32 _DetectRefLost(MPV_ES_INFO_T* prEsInfo, UCHAR ucTemp)
{
    UINT32 u4Delta = 0;

    if ((prEsInfo->u4PreRefPTS != 0) && (_prVdecEsInfo[ucEsId]->u4PTS != 0))
    {
        if (ucTemp == B_TYPE)
        {
            if (_prVdecEsInfo[ucEsId]->u4PTS > prEsInfo->u4PreRefPTS)
            {
                if (((_prVdecEsInfo[ucEsId]->u4PTS & STC_DELTA_MSB) != STC_DELTA_MSB) ||
                    ((prEsInfo->u4PreRefPTS & STC_DELTA_MSB) != 0))
                {
                    return (REFERNCE_MISSING);
                }
            }
            else
            {
                if (((_prVdecEsInfo[ucEsId]->u4PTS & STC_DELTA_MSB) == 0) &&
                    ((prEsInfo->u4PreRefPTS & STC_DELTA_MSB) == STC_DELTA_MSB))
                {
                    return (REFERNCE_MISSING);
                }
            }
        }
        else
        {
            if (_prVdecEsInfo[ucEsId]->u4PTS > prEsInfo->u4PreRefPTS)
            {
                if (((_prVdecEsInfo[ucEsId]->u4PTS & STC_DELTA_MSB) == STC_DELTA_MSB) &&
                    ((prEsInfo->u4PreRefPTS & STC_DELTA_MSB) == 0))
                {
                    u4Delta = MPV_PTS_DRIFT + 1;
                }
                else
                {
                     u4Delta = (_prVdecEsInfo[ucEsId]->u4PTS - prEsInfo->u4PreRefPTS);
                }
            }
            else
            {
                if (((_prVdecEsInfo[ucEsId]->u4PTS & STC_DELTA_MSB) == 0) &&
                    ((prEsInfo->u4PreRefPTS & STC_DELTA_MSB) == STC_DELTA_MSB))
                {
                    u4Delta = ((0xFFFFFFFF - prEsInfo->u4PreRefPTS )+ _prVdecEsInfo[ucEsId]->u4PTS);
                }
                else
                {
                    u4Delta = MPV_PTS_DRIFT + 1;
                }
            }
        }
        //LOG(6, "u4PreRefPTS: %x, u4PTS: %x\n", prEsInfo->u4PreRefPTS, _prVdecEsInfo[ucEsId]->u4PTS);

        prEsInfo->u4PreRefPTS = _prVdecEsInfo[ucEsId]->u4PTS;

        if ((u4Delta > MPV_PTS_DRIFT) && (u4Delta < MPV_PTS_INVALID_DRIFT))
        {
            if (ucTemp == I_TYPE)
            {
                FBM_InvalidateRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId);
            }
            else if (ucTemp == P_TYPE)
            {
                return (REFERNCE_MISSING);
            }
        }
    }
    return (NONE_ERROR);
}
#endif

static INT32 _CcData(UCHAR ucMpvId, UCHAR ucEsId)
{
    //BOOL fgCcValid;
    //BOOL fgProcEm;
    BOOL fgProcCc;
    BOOL fgAddData;
    BOOL fgBottomFirst;
    //UCHAR ucEmData;
    //UCHAR ucCcType;
    UCHAR ucCcCount;
    UCHAR ucUsrDataTp;
    UCHAR ucMarker;
    UCHAR ucData;
    UCHAR ucCcData1;
    UCHAR ucCcData2;
    UCHAR *pucCcAddr;
    UCHAR *pucIPTmpAddr;
    UCHAR* pucAddr;
    INT32  i4Ret;
    UINT32 u4Datain;
    UINT32 u4Idx;
    UINT32 u4CcBufSize;
    UINT32 u4Ptr = 0;
    UINT32 u4CcLen = 0;
//    MPV_ES_INFO_T *prEsInfo;
    MPV_CC_T *prMpvCc;
    CC_INFO_T *prCcInfo;
    CC_BUF_INFO_T *prCcBufInfo;
    MPV_ES_INFO_T* prEsInfo;
    FBM_SEQ_HDR_T* prFbmSeqHdr;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);

    prEsInfo = _prEsInfo[ucEsId];
    prCcInfo = &(prMpvCc->rCcInfo);
    prCcBufInfo = &(prMpvCc->rCcBufInfo);
    pucCcAddr = prMpvCc->pucCcBuf;

    if (_prVdecEsInfo[ucEsId]->ucPicType != B_TYPE)
    {
        if ((prMpvCc->u4AtscIPTmpSize != 0) &&
            (prMpvCc->u4AtscIPTmpPts != _prVdecEsInfo[ucEsId]->u4PTS))
        {
            pucIPTmpAddr = prMpvCc->pucCcAtscIPTmpBuf;
            u4CcBufSize = CCSIZE(prCcBufInfo->u4Wptr, prCcBufInfo->u4Rptr,
                                                prMpvCc->u4CcBufSize);

            if ((u4CcBufSize + prMpvCc->u4AtscIPTmpSize) > prMpvCc->u4CcBufSize)
            {
                if (prMpvCc->pfCcNfyFct)
                {
                    LOG(3, "CC Bufer Overflow\n");
                    prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                        MPV_CC_COND_ERROR, (void*)MPV_CC_ERR_BUFF_OVER_FLOW);
                }
            }
            else
            {
                for (u4Idx = 0; u4Idx < prMpvCc->u4AtscIPTmpSize; u4Idx++)
                {
                    u4Ptr = (prCcBufInfo->u4Wptr + u4Idx) % prMpvCc->u4CcBufSize;
                    pucCcAddr[u4Ptr] = pucIPTmpAddr[u4Idx];
                }

                prCcInfo->fgIsScte= FALSE;
                prCcInfo->fgBottomFirst = prMpvCc->fgCcBottomFirst;
                prCcInfo->u4Size = prMpvCc->u4AtscIPTmpSize;
                prCcInfo->u8Pts = prMpvCc->u4AtscIPTmpPts;
                prCcInfo->u4BufRptr = prCcBufInfo->u4Wptr;
                prCcBufInfo->u4Wptr = u4Ptr + 1;

                i4Ret = x_msg_q_send(prMpvCc->hCcQueue,
                    (void *)prCcInfo, sizeof(CC_INFO_T), 255);

                VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));

                if ((prMpvCc->pfCcNfyFct) && (i4Ret == OSR_OK))
                {
                    LOG(10, "atsc Notify IP Cc 0x%x\n", prCcInfo->u8Pts);
                    prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                        MPV_CC_COND_DATA_ARRIVAL, (void*)(prCcInfo->u4Size));
                }
                else if(i4Ret == OSR_TOO_MANY)
                {
                    LOG(0, "atsc Notify IP Cc 0x%x Drop\n", prCcInfo->u8Pts);
                }
                prMpvCc->u4AtscIPTmpSize = 0;
            }

        }

        if ((prMpvCc->u4ScteIPTmpSize != 0) &&
            (prMpvCc->u4ScteIPTmpPts != _prVdecEsInfo[ucEsId]->u4PTS))
        {
            pucIPTmpAddr = prMpvCc->pucCcScteIPTmpBuf;
            u4CcBufSize = CCSIZE(prCcBufInfo->u4Wptr, prCcBufInfo->u4Rptr,
                                                prMpvCc->u4CcBufSize);

            if ((u4CcBufSize + prMpvCc->u4ScteIPTmpSize) > prMpvCc->u4CcBufSize)
            {
                if (prMpvCc->pfCcNfyFct)
                {
                    LOG(3, "CC Bufer Overflow\n");
                    prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                        MPV_CC_COND_ERROR, (void*)MPV_CC_ERR_BUFF_OVER_FLOW);
                }
            }
            else
            {
                for (u4Idx = 0; u4Idx < prMpvCc->u4ScteIPTmpSize; u4Idx++)
                {
                    u4Ptr = (prCcBufInfo->u4Wptr + u4Idx) % prMpvCc->u4CcBufSize;
                    pucCcAddr[u4Ptr] = pucIPTmpAddr[u4Idx];
                }

                prCcInfo->fgIsScte= TRUE;
                prCcInfo->fgBottomFirst = prMpvCc->fgCcBottomFirst;
                prCcInfo->u4Size = prMpvCc->u4ScteIPTmpSize;
                prCcInfo->u8Pts = prMpvCc->u4ScteIPTmpPts;
                prCcInfo->u4BufRptr = prCcBufInfo->u4Wptr;
                prCcBufInfo->u4Wptr = u4Ptr + 1;

                i4Ret = x_msg_q_send(prMpvCc->hCcQueue,
                    (void *)prCcInfo, sizeof(CC_INFO_T), 255);

                VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));

                if ((prMpvCc->pfCcNfyFct) && (i4Ret == OSR_OK))
                {
                    LOG(10, "scteNotify IP Cc 0x%x\n", prCcInfo->u8Pts);
                    prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                        MPV_CC_COND_DATA_ARRIVAL, (void*)(prCcInfo->u4Size));
                }
                else if(i4Ret == OSR_TOO_MANY)
                {
                    LOG(0, "scteNotify IP Cc 0x%x Drop\n", prCcInfo->u8Pts);
                }
                prMpvCc->u4ScteIPTmpSize = 0;
            }
        }
    }

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
  //------------------------------------------ATSC----------------------------------
    if (u4Datain == ATSC_IDENTIFIER)
    {
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 32);        //remove ATSC_IDENTIFIER
        ucUsrDataTp = (UCHAR)((u4Datain & 0xFF000000) >> 24);              //8 bits

        u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);         //remove user data type code

        if (ucUsrDataTp == EIA_CC)
        {
            if (_prEsInfo[ucEsId]->u4AtscCcUsrDataCnt == 0)
            {
                _prEsInfo[ucEsId]->u4AtscCcUsrDataCnt++;
                pucAddr = prMpvCc->pucCcTmpBuf;

                //fgProcEm = (u4Datain & 0x80000000) >> 31;    //1 bit
                fgProcCc = (BOOL)((u4Datain & 0x40000000) >> 30);        // 1bit
                fgAddData = (BOOL)((u4Datain & 0x20000000) >> 29);      // 1 bit
                ucCcCount = (UCHAR)((u4Datain & 0x1F000000) >> 24);      //5 bits
                //ucEmData = (u4Datain & 0x00FF0000) >> 16;    //8 bits

                u4Datain = _GetBitStream(ucMpvId, ucEsId, 16);

                for (u4Idx = 0; u4Idx < ucCcCount; u4Idx++)
                {
                    ucMarker = (UCHAR)((u4Datain & 0xF8000000) >> 27);    //5 bits

                    if (ucMarker != 0x1F)
                    {
                        //MPV_UnlockCcSema(ucEsId);
                        //return(USR_DAT_MKB_ERR);
                    }

                    if (fgProcCc)
                    {
                        //fgCcValid = (u4Datain & 0x04000000) >> 26;  //1 bit
                        //ucCcType = (u4Datain & 0x03000000) >> 24;  //2 bit
                        pucAddr[u4CcLen]= 0;
                        ucData = (UCHAR)((u4Datain & 0x07000000) >> 24);         //8bits
                        pucAddr[u4CcLen+1]= ucData;
                        ucCcData1 = (UCHAR)((u4Datain & 0x00FF0000) >> 16);   //8bits
                        pucAddr[u4CcLen+2] = ucCcData1;
                        ucCcData2 = (UCHAR)((u4Datain & 0x0000FF00) >> 8);     //8bits
                        pucAddr[u4CcLen+3] = ucCcData2;
                        u4CcLen +=4;
                    }
                    u4Datain = _GetBitStream(ucMpvId, ucEsId, 24);
                }

                if (u4CcLen != 0)
                {
                    fgBottomFirst = FALSE;
                    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
                    if(prFbmSeqHdr)
                    {
                        if (!prFbmSeqHdr->fgProgressiveSeq)        // Interlace Sequence
                        {
                            if(prEsInfo->rFbmPicHdr.ucPicStruct == FRM_PIC)
                            {
                                fgBottomFirst = (!prEsInfo->rFbmPicHdr.fgTopFldFirst);
                            }
                            else if(prEsInfo->rFbmPicHdr.ucPicStruct == TOP_FLD_PIC)
                            {
                                //fgBottomFirst = FALSE;
                            }
                            else // BTM_FLD_PIC
                            {
                                fgBottomFirst = TRUE;
                            }
                        }
                        else // Progress sequence
                        {
                            //fgBottomFirst = FALSE;
                        }
                    }
                    if (_prVdecEsInfo[ucEsId]->ucPicType == B_TYPE)
                    {
                        u4CcBufSize = CCSIZE(prCcBufInfo->u4Wptr,
                                    prCcBufInfo->u4Rptr, prMpvCc->u4CcBufSize);

                        if ((u4CcBufSize + u4CcLen) > (prMpvCc->u4CcBufSize))
                        {
                            if (prMpvCc->pfCcNfyFct)
                            {
                                prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                                    MPV_CC_COND_ERROR, (void*)MPV_CC_ERR_BUFF_OVER_FLOW);

                                LOG(3, "CC Bufer Overflow\n");
                            }
                        }
                        else
                        {
                            for (u4Idx = 0; u4Idx < u4CcLen; u4Idx++)
                            {
                                u4Ptr = (prCcBufInfo->u4Wptr+u4Idx) % prMpvCc->u4CcBufSize;
                                pucCcAddr[u4Ptr] = pucAddr[u4Idx];
                            }
                            prCcInfo->fgIsScte= FALSE;
                            prCcInfo->fgBottomFirst = fgBottomFirst;
                            prCcInfo->u4Size = u4CcLen;
                            prCcInfo->u8Pts = _prVdecEsInfo[ucEsId]->u4PTS;
                            prCcInfo->u4BufRptr = prCcBufInfo->u4Wptr;
                            prCcBufInfo->u4Wptr = u4Ptr + 1;

                            i4Ret = x_msg_q_send(prMpvCc->hCcQueue,
                                (void *)prCcInfo, sizeof(CC_INFO_T), 255);

                            VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));

                            if ((prMpvCc->pfCcNfyFct) && (i4Ret == OSR_OK))
                            {
                                LOG(10, "atscNotify B Cc 0x%x\n", prCcInfo->u8Pts);
                                prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                                    MPV_CC_COND_DATA_ARRIVAL, (void*)(prCcInfo->u4Size));
                            }
                            else if(i4Ret == OSR_TOO_MANY)
                            {
                                LOG(0, "atscNotify B Cc 0x%x Drop\n", prCcInfo->u8Pts);
                            }
                        }
                    }
                    else
                    {
                        x_memcpy(prMpvCc->pucCcAtscIPTmpBuf, pucAddr, u4CcLen);
                        prMpvCc->u4AtscIPTmpSize = u4CcLen;
                        prMpvCc->u4AtscIPTmpPts = _prVdecEsInfo[ucEsId]->u4PTS;
                        prMpvCc->fgCcBottomFirst = fgBottomFirst;
                    }
                }
                else
                {
                    if (_prVdecEsInfo[ucEsId]->ucPicType != B_TYPE)
                    {
                        prMpvCc->u4AtscIPTmpSize = u4CcLen;
                    }
                }

                ucMarker = (UCHAR)((u4Datain & 0xFF000000) >> 24); //8 bits

                if (ucMarker != 0xFF)
                {
                    //MPV_UnlockCcSema(ucEsId);
                    LOG(6, "cc_data markerbits2 0x%x", ucMarker);
                    //return (USR_DAT_MKB_ERR);
                }

                u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);

                if (fgAddData == 1)
                {
                    while ((u4Datain >> 8) != 0x000001)
                    {
                        //additional user data
                        u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
                    }
                }
            }
        }
        else
        {
            while ((u4Datain >> 8) != 0x000001)
            {
                //ATSC reserved user data
                u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
            }
        }
    }
  //-------------------------------------------SCTE----------------------------------
    else if (((u4Datain & 0xFF000000) >> 24) == EIA_CC)
    {
        if (_prEsInfo[ucEsId]->u4ScteCcUsrDataCnt == 0)
        {
            BOOL fgVbiData;
            UCHAR ucCheckbits;

            _prEsInfo[ucEsId]->u4ScteCcUsrDataCnt++;

            //remove user_data_type
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);

            ucCheckbits = (UCHAR)((u4Datain & 0x7E000000) >> 24);
            fgVbiData = (BOOL)((u4Datain & 0x01000000) >> 24);

            if ((fgVbiData) && (ucCheckbits == 0))
            {
                pucAddr = prMpvCc->pucCcTmpBuf;
                u4CcLen = 0;

                ucCcCount = (UCHAR)((u4Datain & 0x00F80000) >> 19);
                u4Datain = _GetBitStream(ucMpvId, ucEsId, 13);

                for (u4Idx = 0; u4Idx < ucCcCount; u4Idx++)
                {
                    ucData = (UCHAR)((u4Datain & 0xf0000000) >> 28);         //8bits
                    pucAddr[u4CcLen]= ucData;
                    ucData = (UCHAR)((u4Datain & 0x0f800000) >> 23);         //8bits
                    pucAddr[u4CcLen+1] = ucData;
                    ucCcData1 = (UCHAR)((u4Datain & 0x007F8000) >> 15);   //8bits
                    INVERSE_BIT_ORDER_8(ucCcData1);
                    pucAddr[u4CcLen+2] = ucCcData1;
                    ucCcData2 = (UCHAR)((u4Datain & 0x00007F80) >> 7);    //8bits
                    INVERSE_BIT_ORDER_8(ucCcData2);
                    pucAddr[u4CcLen+3] = ucCcData2;

                    u4CcLen += 4;

                    ucMarker = (UCHAR)((u4Datain & 0x00000040) >> 6);

                    if (ucMarker != 0x1)
                    {
                        //MPV_UnlockCcSema(ucEsId);
                        //return(USR_DAT_MKB_ERR);
                    }

                    u4Datain = _GetBitStream(ucMpvId, ucEsId, 26);
                }

                if (u4CcLen != 0)
                {
                    fgBottomFirst = FALSE;
                    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
                    if(prFbmSeqHdr)
                    {
                        if (!prFbmSeqHdr->fgProgressiveSeq)        // Interlace Sequence
                        {
                            if(prEsInfo->rFbmPicHdr.ucPicStruct == FRM_PIC)
                            {
                                fgBottomFirst = (!prEsInfo->rFbmPicHdr.fgTopFldFirst);
                            }
                            else if(prEsInfo->rFbmPicHdr.ucPicStruct == TOP_FLD_PIC)
                            {
                                //fgBottomFirst = FALSE;
                            }
                            else // BTM_FLD_PIC
                            {
                                fgBottomFirst = TRUE;
                            }
                        }
                        else // Progress sequence
                        {
                            //fgBottomFirst = FALSE;
                        }
                    }
                    if (_prVdecEsInfo[ucEsId]->ucPicType == B_TYPE)
                    {
                        u4CcBufSize = CCSIZE(prCcBufInfo->u4Wptr,
                            prCcBufInfo->u4Rptr, prMpvCc->u4CcBufSize);

                        if ((u4CcBufSize + u4CcLen) > (prMpvCc->u4CcBufSize))
                        {
                            if (prMpvCc->pfCcNfyFct)
                            {
                                prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                                    MPV_CC_COND_ERROR, (void*)MPV_CC_ERR_BUFF_OVER_FLOW);

                                LOG(3, "CC Bufer Overflow\n");
                            }
                        }
                        else
                        {
                            for (u4Idx = 0; u4Idx < u4CcLen; u4Idx++)
                            {
                                    u4Ptr = (prCcBufInfo->u4Wptr+u4Idx) % prMpvCc->u4CcBufSize;
                                    pucCcAddr[u4Ptr] = pucAddr[u4Idx];
                            }

                            prCcInfo->fgIsScte= TRUE;
                            prCcInfo->fgBottomFirst = fgBottomFirst;
                            prCcInfo->u4Size = u4CcLen;
                            prCcInfo->u8Pts = _prVdecEsInfo[ucEsId]->u4PTS;
                            prCcInfo->u4BufRptr = prCcBufInfo->u4Wptr;
                            prCcBufInfo->u4Wptr = u4Ptr + 1;

                            i4Ret = x_msg_q_send(prMpvCc->hCcQueue,
                                (void *)prCcInfo, sizeof(CC_INFO_T), 255);

                            VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));

                            if ((prMpvCc->pfCcNfyFct) && (i4Ret == OSR_OK))
                            {
                                LOG(10, "scte Notify B Cc 0x%x\n", prCcInfo->u8Pts);
                                prMpvCc->pfCcNfyFct(prMpvCc->pvCcNfyTag,
                                    MPV_CC_COND_DATA_ARRIVAL, (void*)(prCcInfo->u4Size));
                            }
                            else if(i4Ret == OSR_TOO_MANY)
                            {
                                LOG(0, "scte Notify B Cc 0x%x Drop\n", prCcInfo->u8Pts);
                            }
                        }
                    }

                    else
                    {
                        x_memcpy(prMpvCc->pucCcScteIPTmpBuf, pucAddr, u4CcLen);
                        prMpvCc->u4ScteIPTmpSize = u4CcLen;
                        prMpvCc->u4ScteIPTmpPts = _prVdecEsInfo[ucEsId]->u4PTS;
                        prMpvCc->fgCcBottomFirst = fgBottomFirst;
                    }

                }
                else
                {
                    if (_prVdecEsInfo[ucEsId]->ucPicType != B_TYPE)
                    {
                        prMpvCc->u4ScteIPTmpSize = u4CcLen;
                    }
                }
            }
        }
    }

    return (NONE_ERROR);

}

#ifndef AUTO_QMATRIX
static void _SaveQuantMatrix(UCHAR ucMpvId, UCHAR ucEsId, BOOL fgIntra)
{
    UINT32 u4Datain;
    UCHAR ucMatrixIdx;
    UINT32* pu4Addr;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return;
    }

    if (fgIntra)
    {
        pu4Addr = _prEsInfo[ucEsId]->pu4IntraMatrix;
    }
    else
    {
        pu4Addr = _prEsInfo[ucEsId]->pu4NonIntraMatrix;
    }

    pu4Addr[0] = _GetBitStream(ucMpvId, ucEsId, 0);
    LOG(10, "%x\n", pu4Addr[0]);

    for (ucMatrixIdx = 1; ucMatrixIdx < MPV_MATRIX_SIZE; ucMatrixIdx++)
    {
        pu4Addr[ucMatrixIdx] = _GetBitStream(ucMpvId, ucEsId, 32);
        LOG(10, "%x\n", pu4Addr[ucMatrixIdx]);
    }

    #ifdef CC_MT5395
    HalFlushDCacheMultipleLine((UINT32)pu4Addr, (MPV_MATRIX_SIZE * sizeof(UINT32)));
    #else
    HalFlushInvalidateDCache();
    #endif
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 32);
    UNUSED(u4Datain);
}
#else
/*
static void _LoadQuantMatrix(UCHAR ucMpvId, UCHAR ucEsId, BOOL fgIntra)
{
    _MPV_VldAutoLoadQMat(ucMpvId, fgIntra);

    _prEsInfo[ucEsId]->ucBitCount  += (64 << 3);
}
*/
#endif

static INT32 _SeqDisplayExt(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4Datain;
    MPEG2_SEQ_DISP_EXT_HDR_UNION_T rSeqDispExtHdr;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    VDEC_ES_INFO_T* prVdecEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    prVdecEsInfo = _prVdecEsInfo[ucEsId];
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
    if(prFbmSeqHdr == NULL)
    {
        LOG(3, "_SeqDisplayExt prFbmSeqHdr null\n");
        return NONE_ERROR;
    }

    prVdecEsInfo->fgSeqDispExt = TRUE;
    rSeqDispExtHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 0);

    prVdecEsInfo->ucVideoFmt = (UCHAR)rSeqDispExtHdr.rField.u4VideoFmt;

    if (rSeqDispExtHdr.rField.fgColorDesc)
    {
        rSeqDispExtHdr.au4Reg[1] = _GetBitStream(ucMpvId, ucEsId, 32);

#if (COLOR_PRI_ERR)
        if (rSeqDispExtHdr.rField.u4ColorPrimary == 0)
        {
            return (COLOR_PRI_ERR);
        }
#endif
        if(prFbmSeqHdr->ucColourPrimaries != (UCHAR)rSeqDispExtHdr.rField.u4ColorPrimary)
        {
            prFbmSeqHdr->ucColourPrimaries = (UCHAR)rSeqDispExtHdr.rField.u4ColorPrimary;
            FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_SEQ_CHG);
        }

#if (TRA_CHA_ERR)
        if (rSeqDispExtHdr.rField.u4TrasferChar == 0)
        {
            return (TRA_CHA_ERR);
        }
#endif
        if(prFbmSeqHdr->ucTransCharacter != (UCHAR)rSeqDispExtHdr.rField.u4TrasferChar)
        {
            prFbmSeqHdr->ucTransCharacter = (UCHAR)rSeqDispExtHdr.rField.u4TrasferChar;
            FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_SEQ_CHG);
        }

#if (MAT_COE_ERR)
        if (rSeqDispExtHdr.rField.u4MatrixCoef == 0)
        {
            return (MAT_COE_ERR);
        }
#endif
        if(prFbmSeqHdr->ucMatrixCoeff != (UCHAR)rSeqDispExtHdr.rField.u4MatrixCoef)
        {
            prFbmSeqHdr->ucMatrixCoeff = (UCHAR)rSeqDispExtHdr.rField.u4MatrixCoef;
            FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_SEQ_CHG);
        }

        prFbmSeqHdr->fgColorDescrip = TRUE;
    }
    else
    {
        rSeqDispExtHdr.au4Reg[1] = _GetBitStream(ucMpvId, ucEsId, 8);
    }

#if (SEQDE_MKB_ERR)
    if (rSeqDispExtHdr.rField.fgMark == 0)
    {
        return (SEQDE_MKB_ERR);
    }
#endif

    prFbmSeqHdr->u2DHS = (UINT16)rSeqDispExtHdr.rField.u4DispHSize;
    prFbmSeqHdr->u2DVS = (UINT16)rSeqDispExtHdr.rField.u4DispVSize;
#ifdef CC_RESIZE_SMALL_IMAGE
    if(prFbmSeqHdr->fgResizeSmallPic)
    {
        prFbmSeqHdr->u2DHS = prFbmSeqHdr->u2HSize;
        prFbmSeqHdr->u2DVS = prFbmSeqHdr->u2VSize;
    }
#endif

    // get 29 bits off
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 29);
    UNUSED(u4Datain);

    //if (!_NextStartCode(ucMpvId, ucEsId))
    if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
    {
        return(NO_START_C_ERR5);
    }

    return (NONE_ERROR);
}


static INT32 _QuantMatrixExt(UCHAR ucMpvId, UCHAR ucEsId)
{
  //BYTE pbChromaIntraQMatrix[64];
  //BYTE pbChromaNonIntraQMatrix[64];

    BOOL fgLoadChromaIntraQMatrix;
    BOOL fgLoadChromaNonIntraQMatrix;
    UCHAR ucIndex;
    UINT32 u4Datain;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 4);                   // extension start code id

    _prEsInfo[ucEsId]->fgLoadIntraMatrix = u4Datain >> 31;

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 1);

    if (_prEsInfo[ucEsId]->fgLoadIntraMatrix)
    {
        LOG(7, "Pic Q-Matrix Ext (I)\n");
        _SaveQuantMatrix(ucMpvId, ucEsId, TRUE);
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
    }

    _prEsInfo[ucEsId]->fgLoadNonIntraMatrix = u4Datain >> 31;

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 1);

    if (_prEsInfo[ucEsId]->fgLoadNonIntraMatrix)
    {
        LOG(7, "Pic Q-Matrix Ext (NI)\n");

        _SaveQuantMatrix(ucMpvId, ucEsId, FALSE);

        u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
    }

    if (_prEsInfo[ucEsId]->fgLoadIntraMatrix ||
        _prEsInfo[ucEsId]->fgLoadNonIntraMatrix)
    {
        _prEsInfo[ucEsId]->u4MatrixId = _au4QMatrixCounter[ucMpvId]++;
    }

    fgLoadChromaIntraQMatrix = u4Datain >> 31;

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 1);

    if (fgLoadChromaIntraQMatrix)
    {
        for(ucIndex=0 ; ucIndex < 64 ; ucIndex++)
        {
           //pbChromaIntraQMatrix[bIndex] = u4Datain >> 24;
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
        }
    }

    fgLoadChromaNonIntraQMatrix = u4Datain>> 31;
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 1);

    if (fgLoadChromaNonIntraQMatrix)
    {
        for(ucIndex=0 ; ucIndex < 64 ; ucIndex++)
        {
            //pbChromaNonIntraQMatrix[bIndex] = u4Datain >> 24;
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
        }
    }

    //if (!_NextStartCode(ucMpvId, ucEsId))
	if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
    {
        return(NO_START_C_ERR6);
    }
    return (NONE_ERROR);
}


static INT32 _CopyrightExt(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4Datain;
    MPEG2_COPYRIGHT_EXT_HDR_UNION_T rCopyRightHdr;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    rCopyRightHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 0);
    rCopyRightHdr.au4Reg[1] = _GetBitStream(ucMpvId, ucEsId, 22);
    rCopyRightHdr.au4Reg[2] = _GetBitStream(ucMpvId, ucEsId, 21);
    rCopyRightHdr.au4Reg[3] = _GetBitStream(ucMpvId, ucEsId, 23);
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 22);
    UNUSED(u4Datain);

#if (PICCE_MKB1_ERR)
    if (rCopyRightHdr.rField.fgMark1 == 0)
    {
        return (PICCE_MKB1_ERR);
    }
#endif

#if (PICCE_MKB2_ERR)
    if (rCopyRightHdr.rField.fgMark2 == 0)
    {
        return (PICCE_MKB2_ERR);
    }
#endif

#if (PICCE_MKB3_ERR)
    if (rCopyRightHdr.rField.fgMark3 == 0)
    {
        return (PICCE_MKB3_ERR);
    }
#endif

    //if (!_NextStartCode(ucMpvId, ucEsId))
	if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
    {
        return(NO_START_C_ERR7);
    }

    return (NONE_ERROR);
}


static INT32 _PictureDisExt(UCHAR ucMpvId, UCHAR ucEsId)        //picture_display_extension()
{
    //UINT16 u2FrmCenterHOfst[3];                    //frame_center_horizontal_offset[3];
    //UINT16 u2FrmCenterVOfst[3];                    //frame_center_vertical_offset[3];
    //UINT16 u2XCenter, u2YCenter;
    UCHAR ucNumFrmCenterOfst;                     //number_of_frame_centre_offsets;
    UCHAR ucIdx;
    UINT32 u4Datain;
    MPV_ES_INFO_T* prEsInfo;
    FBM_PIC_HDR_T *prFbmPicHdr;
    FBM_SEQ_HDR_T *prFbmSeqHdr;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    prEsInfo = _prEsInfo[ucEsId];
    if(prEsInfo == NULL)
    {
        LOG(1, "_PictureDisExt prEsInfo null\n");
        return NONE_ERROR;
    }
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
    if(prFbmSeqHdr == NULL)
    {
        LOG(1, "_PictureDisExt prFbmSeqHdr null\n");
        return NONE_ERROR;
    }
    prFbmPicHdr = &(prEsInfo->rFbmPicHdr);
    if(prFbmPicHdr == NULL)
    {
        LOG(1, "_PictureDisExt prFbmPicHdr null\n");
        return NONE_ERROR;
    }

    if(prFbmSeqHdr->fgProgressiveSeq == 1)
    {
        if(prFbmPicHdr->fgRepFirstFld == 1)
        {
            if(prFbmPicHdr->fgTopFldFirst == 1)
            {
                ucNumFrmCenterOfst = 3;
            }
            else
            {
                ucNumFrmCenterOfst = 2;
            }
        }
        else
        {
            ucNumFrmCenterOfst = 1;
        }
    }
    else
    {
        if(prFbmPicHdr->ucPicStruct != 3) //// field
        {
            ucNumFrmCenterOfst = 1;
        }
        else
        {
            if(prFbmPicHdr->fgRepFirstFld==1)
            {
                ucNumFrmCenterOfst = 3;
            }
            else
            {
                ucNumFrmCenterOfst = 2;
            }
        }
    }

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 4);

    for(ucIdx = 0 ; ucIdx < ucNumFrmCenterOfst; ucIdx++)
    {
        MPEG2_PIC_DISP_EXT_HDR_UNION_T rPicDispExtHdr;

        rPicDispExtHdr.au4Reg[0] = u4Datain;
        prFbmPicHdr->au2HOffset[ucIdx] = (UINT16)rPicDispExtHdr.rField.u4Offset;

#if (PICDE_MKB1_ERR)
        if (rPicDispExtHdr.rField.fgMark == 0)
        {
            return (PICDE_MKB1_ERR);
        }
#endif

        rPicDispExtHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 17);
        prFbmPicHdr->au2VOffset[ucIdx] = (UINT16)rPicDispExtHdr.rField.u4Offset;

#if (PICDE_MKB2_ERR)
        if (rPicDispExtHdr.rField.fgMark == 0)
        {
            return (PICDE_MKB2_ERR);
        }
#endif
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 17);

        if ((prFbmPicHdr->au2HOffset[ucIdx] != 0 ) ||
            (prFbmPicHdr->au2VOffset[ucIdx] != 0))
        {
            MPV_EventNotify(MPV_PAN_SCAN, ucEsId, prFbmPicHdr->au2HOffset[ucIdx],
                                        prFbmPicHdr->au2VOffset[ucIdx]);
        }
    }

    //if (!_NextStartCode(ucMpvId, ucEsId))
	if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
    {
        return(NO_START_C_ERR8);
    }

    return (NONE_ERROR);
}


static INT32 _ExtData(UCHAR ucMpvId, UCHAR ucEsId, UCHAR ucIdx)
{
    INT32 i4RetVal;
    UCHAR ucExtStartCode;
    UINT32 u4Datain;

    while ((u4Datain = _GetBitStream(ucMpvId, ucEsId, 0)) == EXTENSION_START_CODE)
    {
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 32);
        ucExtStartCode = (UCHAR)(u4Datain >>28);

        //follows seq_extension
        if (ucIdx==0)
        {
            if (ucExtStartCode == SEQUENCE_DISPLAY_EXTENSION_ID)
            {
                i4RetVal = _SeqDisplayExt(ucMpvId, ucEsId);

                if (i4RetVal != 0)
                {
                    return(i4RetVal);
                }
            }
            else
            {
                // find next start code
                u4Datain =  _GetBitStream(ucMpvId, ucEsId, 8);
                //if (!_NextStartCode(ucMpvId, ucEsId))
				if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
                {
                    return(NO_START_C_ERR2);
                }
            }
        } // end if (bIndex==0)

        //pix_coding_extension
        if (ucIdx==2)
        {
            i4RetVal = 0;
            if (ucExtStartCode == QUANT_MATRIX_EXTENSION_ID)
            {
                i4RetVal = _QuantMatrixExt(ucMpvId, ucEsId);
            }
            else if (ucExtStartCode == COPYRIGHT_EXTENSION_ID)
            {
                i4RetVal = _CopyrightExt(ucMpvId, ucEsId);
            }
            else if (ucExtStartCode == PICTURE_DISPLAY_EXTENSION_ID)
            {
                i4RetVal = _PictureDisExt(ucMpvId, ucEsId);
            }
            else
            {
                u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
                //if (!_NextStartCode(ucMpvId, ucEsId))
				if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
                {
                    return(NO_START_C_ERR3);
                }
            }
            if (i4RetVal != 0)
            {
                return(i4RetVal);
            }
        } // end if (bIndex==2)
    } // end while((u4Datain = _GetBitStream(0)) == EXTENSION_START_CODE)
    return (NONE_ERROR);
}

static INT32 _GetUserData(UCHAR ucMpvId, UCHAR ucEsId)
{
    BOOL fgAfd = 0;
    BOOL fgAtsc = 0;
    BOOL fgScte = 0;
    UCHAR ucDataType = 0;
    UINT32 u4Datain;
    UINT32 u4SearchCnt = 0;
    UINT32 u4Ptr = 0;
    MPV_EXT_INFO_T *prExtInfo;
    MPV_ES_INFO_T *prEsInfo;
    //VDEC_INFO_T *prVdecInfo;
    PARAM_MPV_T *prMpvCounter;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    //prVdecInfo = _VDEC_GetInfo();
    prMpvCounter = _prVdecEsInfo[ucEsId]->prMpvCounter;

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);


    if (u4Datain == (UINT32)ATSC_IDENTIFIER)
    {
        fgAtsc = TRUE;
    }
    else if (((u4Datain & 0xFF000000) >> 24) == (UINT32)EIA_CC)
    {
	    fgScte = TRUE;
    }
    else if (u4Datain == (UINT32)AFD_IDENTIFIER)
    {
        fgAfd = TRUE;
    }

   _ucTimeout[ucMpvId] = 0;

    VERIFY(x_timer_start(_hVldTimer[ucMpvId], 500, X_TIMER_FLAG_ONCE,
        _NextStartCodeTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);

    while (_ucTimeout[ucMpvId] == 0)
    {
        //check until start code 0x000001XX
        if (((u4Datain >> 8) == 1) || (u4SearchCnt >= 255)) //255 is uc max size (_rUserData.ucDataLen)
        {
            VERIFY(x_timer_stop(_hVldTimer[ucMpvId]) == OSR_OK);
            if (u4SearchCnt)
            {
                if (fgAfd)
                {
                    ucDataType = (UCHAR)MPV_AFD_DATA;
                }
                else if (prEsInfo->fgPicHdr)
                {
                    ucDataType = (UCHAR)MPV_PICTURE_USER_DATA;
                }
                else if (prEsInfo->fgGopHdr)
                {
                    ucDataType = (UCHAR)MPV_GOP_USER_DATA;
                }
                else if (_prVdecEsInfo[ucEsId]->fgSeqHdr)
                {
                    ucDataType = (UCHAR)MPV_SEQUENCE_USER_DATA;
                }
                else
                {
                    ASSERT(0);
                }
                _rUserData.u4Pts = _prVdecEsInfo[ucEsId]->u4PTS;
                _rUserData.u2TemporalRef = (UINT16)prEsInfo->rFbmPicHdr.i4TemporalRef;
                _rUserData.ucPicStruct = prEsInfo->rFbmPicHdr.ucPicStruct;
                _rUserData.ucPicType = prEsInfo->rFbmPicHdr.ucPicCdTp;
                _rUserData.ucRepFirstFld = prEsInfo->rFbmPicHdr.fgRepFirstFld;
                _rUserData.ucTopFldFirst = prEsInfo->rFbmPicHdr.fgTopFldFirst;
                _rUserData.ucDataType = ucDataType;
                _rUserData.ucDataLen = (UCHAR)u4SearchCnt;
                _rUserData.ucEsId = ucEsId;
                _rUserData.fgProgressiveFrm = prEsInfo->rFbmPicHdr.fgProgressiveFrm;

                if (_prVdecEsInfo[ucEsId]->ucPicType != B_TYPE)
                {
                	if (fgAtsc)
                	{
                    	x_memcpy(&_rUserDataIPAtsc, &_rUserData, sizeof(MPV_USERDATA_CB_T));
                    	_u4RetrieveNsAtsc = prMpvCounter->u4RetrieveNs;
                        LOG(3, "ref Atsc(%d, %d)\n", u4SearchCnt, _u4RetrieveNsAtsc);
                	}
                	else if (fgScte)
                	{
                    	x_memcpy(&_rUserDataIPScte, &_rUserData, sizeof(MPV_USERDATA_CB_T));
                    	_u4RetrieveNsScte = prMpvCounter->u4RetrieveNs;
                        LOG(3, "ref Scte(%d, %d)\n", u4SearchCnt, _u4RetrieveNsScte);
                	}
                	else if (fgAfd)
                	{
                    	x_memcpy(&_rUserDataIPAfd, &_rUserData, sizeof(MPV_USERDATA_CB_T));
                    	_u4RetrieveNsAfd = prMpvCounter->u4RetrieveNs;
                        LOG(3, "ref Afd(%d, %d)\n", u4SearchCnt, _u4RetrieveNsAfd);
                	}
                }
                else
                {
                    _rUserData.u4Rptr = prExtInfo->u4UserDataWptr;
                    if (fgAtsc)
                    {
                        LOG(3, "Atsc notify B (%d, 0x%x, %d)\n", _rUserData.ucDataLen, _rUserData.u4Rptr
                        	, prMpvCounter->u4RetrieveNs);
                    }
                    else if (fgScte)
                    {
                        LOG(3, "Scte notify B (%d, 0x%x, %d)\n", _rUserData.ucDataLen, _rUserData.u4Rptr
                        	, prMpvCounter->u4RetrieveNs);
                    }
                    else if (fgAfd)
                    {
                        LOG(3, "Afd notify B (%d, 0x%x, %d)\n", _rUserData.ucDataLen, _rUserData.u4Rptr
                        	, prMpvCounter->u4RetrieveNs);
                    }

                    prExtInfo->pfUserDataCb(&_rUserData);

                    prExtInfo->u4UserDataWptr = u4Ptr + 1;

                    if (prExtInfo->u4UserDataWptr == MAX_USERDATA_SIZE)
                    {
                        prExtInfo->u4UserDataWptr = 0;
                    }
                }
            }

            return NONE_ERROR;
        }

        if (u4SearchCnt == MAX_USERDATA_SIZE)
        {
            u4SearchCnt = 0;
        }

        if (_prVdecEsInfo[ucEsId]->ucPicType != B_TYPE)
        {
        	if (fgAtsc)
        	{
            	_aucUserDataIPBufAtsc[u4SearchCnt] = (UCHAR)(u4Datain >> 24);
        	}
        	else if (fgScte)
        	{
            	_aucUserDataIPBufScte[u4SearchCnt] = (UCHAR)(u4Datain >> 24);
        	}
        	else if (fgAfd)
        	{
            	_aucUserDataIPBufAfd[u4SearchCnt] = (UCHAR)(u4Datain >> 24);
        	}
        }
        else
        {
            u4Ptr = (prExtInfo->u4UserDataWptr + u4SearchCnt) % MAX_USERDATA_SIZE;
            prExtInfo->aucUserDataBuf[u4Ptr] = (UCHAR)(u4Datain >> 24);
        }

        u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
        u4SearchCnt++;

        if (!(u4SearchCnt % 100))
        {
            x_thread_delay(1);
        }
    }

    return(NO_START_C_ERR4);
}


static INT32 _UsrData(UCHAR ucMpvId, UCHAR ucEsId)
{
    BOOL fgActFmt;
    UCHAR ucActFmt = 0;
    UINT32 u4Datain;
    UINT32 u4Idx;
    UINT32 u4Ptr = 0;
    MPV_ES_INFO_T *prEsInfo;
    MPV_CC_T *prMpvCc;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    MPV_EXT_INFO_T *prExtInfo;
    //VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    PARAM_MPV_T *prMpvCounter;

    prExtInfo = (MPV_EXT_INFO_T*)_MPV_GetExtInfo();
    ASSERT(prExtInfo != NULL);

    //prVdecInfo = _VDEC_GetInfo();
    prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    //remove user data start code
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 32);

    // currently, even in linux,
    // customer does not expect we callback AFD user data to them.
    // But use resolution change flow to notify them.
    // AFD data will be updated to user in VDP.
    // So add (u4Datain != AFD_IDENTIFIER)
    if (((prExtInfo->fgUserData[ucEsId]) && (prExtInfo->pfUserDataCb))
        && (u4Datain != AFD_IDENTIFIER))
    {
        #if 0
        if (((u4Datain != ATSC_IDENTIFIER) && (u4Datain != AFD_IDENTIFIER) && (((u4Datain & 0xFF000000) >> 24) != EIA_CC))
            || ((u4Datain == AFD_IDENTIFIER) && (!prExtInfo->fgAfd[ucEsId])))
        {
            //LOG(3, "not cc\n");
            if (!_HwNextStartCodeInt(ucMpvId, ucEsId, ALL_START_CODE))
            {
                return(NO_START_C_ERR15);
            }
            return(NONE_ERROR);
        }
	#endif

        if (((_prEsInfo[ucEsId]->u4AtscCcUsrDataCnt == 0) && (u4Datain == ATSC_IDENTIFIER)) ||
        	((_prEsInfo[ucEsId]->u4ScteCcUsrDataCnt == 0) && (((u4Datain & 0xFF000000) >> 24) == EIA_CC)) ||
        	((u4Datain == AFD_IDENTIFIER) && (prExtInfo->fgAfd[ucEsId])))
        {
            if (u4Datain == ATSC_IDENTIFIER)
            {
                _prEsInfo[ucEsId]->u4AtscCcUsrDataCnt ++;
            }
            else if (((u4Datain >> 24) & 0xFF) == EIA_CC)
            {
                _prEsInfo[ucEsId]->u4ScteCcUsrDataCnt ++;
            }

            if (_prVdecEsInfo[ucEsId]->ucPicType != B_TYPE)
            {
	            if ((_rUserDataIPAtsc.ucDataLen) && (_u4RetrieveNsAtsc != prMpvCounter->u4RetrieveNs))
	            {
	                for(u4Idx = 0; u4Idx < _rUserDataIPAtsc.ucDataLen; u4Idx++)
	                {
	                    u4Ptr = (prExtInfo->u4UserDataWptr + u4Idx) % MAX_USERDATA_SIZE;
	                    prExtInfo->aucUserDataBuf[u4Ptr] = _aucUserDataIPBufAtsc[u4Idx];
	                }
	                #if 0
	                _rSdUserDataIP.u4DataAddr =
	                    (UINT32)&(prExtInfo->aucUserDataBuf[prExtInfo->u4UserDataWptr]);
	                #endif
	                _rUserDataIPAtsc.u4Rptr = prExtInfo->u4UserDataWptr;
	                LOG(3, "notify atsc R (%d, 0x%x, %d, %d, %d)\n", _rUserDataIPAtsc.u2TemporalRef, _rUserDataIPAtsc.u4Rptr, _rUserDataIPAtsc.ucDataLen
	                    , _u4RetrieveNsAtsc, prMpvCounter->u4RetrieveNs);
	                prExtInfo->pfUserDataCb(&_rUserDataIPAtsc);

	                prExtInfo->u4UserDataWptr = u4Ptr + 1;

	                if (prExtInfo->u4UserDataWptr == MAX_USERDATA_SIZE)
	                {
	                    prExtInfo->u4UserDataWptr = 0;
	                }

	                _rUserDataIPAtsc.ucDataLen = 0;
	            }

	            if ((_rUserDataIPScte.ucDataLen) && (_u4RetrieveNsScte != prMpvCounter->u4RetrieveNs))
	            {
	                for(u4Idx = 0; u4Idx < _rUserDataIPScte.ucDataLen; u4Idx++)
	                {
	                    u4Ptr = (prExtInfo->u4UserDataWptr + u4Idx) % MAX_USERDATA_SIZE;
	                    prExtInfo->aucUserDataBuf[u4Ptr] = _aucUserDataIPBufScte[u4Idx];
	                }
	                #if 0
	                _rSdUserDataIP.u4DataAddr =
	                    (UINT32)&(prExtInfo->aucUserDataBuf[prExtInfo->u4UserDataWptr]);
	                #endif
	                _rUserDataIPScte.u4Rptr = prExtInfo->u4UserDataWptr;
	                LOG(3, "notify scte R (%d, 0x%x, %d, %d, %d)\n", _rUserDataIPScte.u2TemporalRef, _rUserDataIPScte.u4Rptr, _rUserDataIPScte.ucDataLen
	                    , _u4RetrieveNsScte, prMpvCounter->u4RetrieveNs);
	                prExtInfo->pfUserDataCb(&_rUserDataIPScte);

	                prExtInfo->u4UserDataWptr = u4Ptr + 1;

	                if (prExtInfo->u4UserDataWptr == MAX_USERDATA_SIZE)
	                {
	                    prExtInfo->u4UserDataWptr = 0;
	                }

	                _rUserDataIPScte.ucDataLen = 0;
	            }

	            if ((_rUserDataIPAfd.ucDataLen) && (_u4RetrieveNsAfd != prMpvCounter->u4RetrieveNs))
	            {
	                for(u4Idx = 0; u4Idx < _rUserDataIPAfd.ucDataLen; u4Idx++)
	                {
	                    u4Ptr = (prExtInfo->u4UserDataWptr + u4Idx) % MAX_USERDATA_SIZE;
	                    prExtInfo->aucUserDataBuf[u4Ptr] = _aucUserDataIPBufAfd[u4Idx];
	                }
	                #if 0
	                _rSdUserDataIP.u4DataAddr =
	                    (UINT32)&(prExtInfo->aucUserDataBuf[prExtInfo->u4UserDataWptr]);
	                #endif
	                _rUserDataIPAfd.u4Rptr = prExtInfo->u4UserDataWptr;
	                LOG(3, "notify scte R (%d, 0x%x, %d, %d, %d)\n", _rUserDataIPAfd.u2TemporalRef, _rUserDataIPAfd.u4Rptr, _rUserDataIPAfd.ucDataLen
	                    , _u4RetrieveNsAfd, prMpvCounter->u4RetrieveNs);
	                prExtInfo->pfUserDataCb(&_rUserDataIPAfd);

	                prExtInfo->u4UserDataWptr = u4Ptr + 1;

	                if (prExtInfo->u4UserDataWptr == MAX_USERDATA_SIZE)
	                {
	                    prExtInfo->u4UserDataWptr = 0;
	                }

	                _rUserDataIPAfd.ucDataLen = 0;
	            }
	        }
        	return _GetUserData(ucMpvId, ucEsId);
        }
        else
        {
            if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
            {
                return(NO_START_C_ERR15);
            }
            return(NONE_ERROR);
        }

    }
    else
    {
        prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
        if(prFbmSeqHdr == NULL)
        {
            LOG(1, "_UsrData prFbmSeqHdr null\n");
            return NONE_ERROR;
        }


        //AFD
        if (u4Datain == AFD_IDENTIFIER)
        {
            //remove afd_identifier
            // There is a bitstream with error AFD user data all the time.
            // Lead us can't decode any frame if we drop this picture.
            // Now skip this condition.
            do
            {
                u4Datain = _GetBitStream(ucMpvId, ucEsId, 32);
                if ((u4Datain >> 31) != 0)
                {
                    break;
                    //return(USR_DAT_MKB_ERR);
                }

                fgActFmt = (BOOL)((u4Datain & 0x40000000) >> 30);

                if (((u4Datain & 0x3F000000) >> 24) != 0x01)
                {
                    break;
                    //return(USR_DAT_MKB_ERR);
                }

                u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);

                if (fgActFmt == 1)
                {
                    if ((u4Datain >> 28) != 0xF)
                    {
                        break;
                        //return(USR_DAT_MKB_ERR);
                    }

                    ucActFmt = (UCHAR)((u4Datain & 0x0F000000) >> 24);

                    if (prFbmSeqHdr->ucActFmt != ucActFmt)
                    {
    	                LOG(5, "AFD %d -> %d\n", prFbmSeqHdr->ucActFmt, ucActFmt);
                        FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_SEQ_CHG);
                        prFbmSeqHdr->ucActFmt = ucActFmt;
                    }

                    u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
                }
            }
            while(0);
        }
        else
        {
            _VDEC_LockCcSema(ucEsId);

        #ifdef MPV_VIRTUAL_CC
            if ((!prEsInfo->fgVirtualCc) && (prMpvCc->fgPlayCc) && (prEsInfo->fgPicHdr))
         #else
            if ((prMpvCc->fgPlayCc) && (prEsInfo->fgPicHdr))
         #endif
            {
                INT32 i4Ret;

                i4Ret = _CcData (ucMpvId, ucEsId);

                if (i4Ret != NONE_ERROR)
                {
                    return i4Ret;
                }
            }

            _VDEC_UnlockCcSema(ucEsId);
        }

        //if (!_NextStartCode(ucMpvId, ucEsId))
        if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
        {
            return(NO_START_C_ERR15);
        }
    }

    return (NONE_ERROR);
}


static INT32 _ExtUsrData(UCHAR ucMpvId, UCHAR ucEsId, UCHAR ucIdx)
{

    INT32 i4RetVal;
    UINT32 u4Datain;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

    _prEsInfo[ucEsId]->u4AtscCcUsrDataCnt = 0;
    _prEsInfo[ucEsId]->u4ScteCcUsrDataCnt = 0;

    while ((u4Datain == EXTENSION_START_CODE) ||
                (u4Datain == USER_DATA_START_CODE))
    {

        if (u4Datain == EXTENSION_START_CODE)
        {
            if (ucIdx != 1)
            {
                i4RetVal = _ExtData(ucMpvId, ucEsId, ucIdx);

                if (i4RetVal != 0)
                {
                    return(i4RetVal);
                }
               u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
            }
            #ifdef EXT_DATA_IDX_ERR
            else
            {
               return(EXT_DATA_IDX_ERR);
            }
            #endif
        }

        if (u4Datain == USER_DATA_START_CODE)
        {

            i4RetVal = _UsrData(ucMpvId, ucEsId);

            if (i4RetVal != 0)
            {
                return(i4RetVal);
            }
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
        }
    }

    return (NONE_ERROR);
}


static INT32 _SequenceHdr(UCHAR ucMpvId, UCHAR ucEsId)
{
    BOOL fgVideoLock;
    BOOL fgDisable;
    UCHAR ucTemp;
    UCHAR ucFbgType;
    UCHAR ucFbNs;
    INT32 i4Ret;
    UINT16 u2HSize;
    UINT16 u2VSize;
    UINT16 u2OriHSize;
    UINT16 u2OriVSize;
    UINT32 u4Datain;
    UINT32 u4FbWidth;
    UINT32 u4FbHeight;
    UINT32 u4Retry;

    MPV_ES_INFO_T* prEsInfo;
    MPV_INFO_T* prMpvInfo;
    MPEG2_SEQ_HDR_UNION_T rSeqHdr;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    VDEC_INFO_T* prVdecInfo;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    PARAM_MPV_T *prMpvCounter;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    prMpvInfo = (MPV_INFO_T*)_MPV_GetMpvInfo();
    prEsInfo = _prEsInfo[ucEsId];
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecInfo = _VDEC_GetInfo();
    prMpvCounter = prVdecEsInfo->prMpvCounter;
    fgVideoLock = FALSE;

    // setting of the seq_header_flag to check MPEG1 bits
    prEsInfo->ucMpegVer = MPEG1;

    // Get Sequence Header
    rSeqHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 32);
    if (rSeqHdr.au4Reg[0] == 0x000001b4)
    {
    	rSeqHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 32);
    }
    rSeqHdr.au4Reg[1] = _GetBitStream(ucMpvId, ucEsId, 32);

#if (SEQH_MKB_ERR)
    if (rSeqHdr.rField.fgMark == 0)
    {
        // skip this sequence header only. contiue decoding this picture
        u4Retry = 0;
        do
        {
            if (u4Retry > 5)
            {
                return(SEQH_MKB_ERR);
            }
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
            if (!_NextStartCode(ucMpvId, ucEsId))
            {
                return (-1);
            }
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
            if(u4Datain == EXTENSION_START_CODE)
            {
            	prEsInfo->ucMpegVer = MPEG2;
            }
            u4Retry++;
        }
        while((u4Datain != (UINT32)PICTURE_START_CODE) || (u4Datain != (UINT32)GOP_START_CODE));
        return (SEQH_MKB_ERR);
    }
#endif

#if (BIT_RATE_RES)
    if (rSeqHdr.rField.u4BitRate == 0)
    {
        //return (BIT_RATE_RES);
    }
#endif

    u2HSize = (UINT16)rSeqHdr.rField.u4HSize;
    u2VSize = (UINT16)rSeqHdr.rField.u4VSize;
    // prevent H W are wrong
    if((u2HSize < 16) || (u2VSize < 16))
    {
        u4Retry = 0;
        do
        {
            if (u4Retry > 5)
            {
                return(SEQH_MKB_ERR);
            }
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
            if (!_NextStartCode(ucMpvId, ucEsId))
            {
                return (-1);
            }
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
            if(u4Datain == EXTENSION_START_CODE)
            {
            	prEsInfo->ucMpegVer = MPEG2;
            }
            u4Retry++;
        }
        while((u4Datain != (UINT32)PICTURE_START_CODE) || (u4Datain != (UINT32)GOP_START_CODE));
        return SEQH_MKB_ERR;
    }

    prVdecEsInfo->fgContrained = rSeqHdr.rField.fgContrained;
    
    //ucFbgType = FBM_SelectGroupType(u2HSize, u2VSize);

    prMpvCounter->u2HSize = u2HSize;
    prMpvCounter->u2VSize = u2VSize;

#ifdef CC_MPV_H_MAX
    if (u2HSize > CC_MPV_H_MAX)
    {
        u2HSize = CC_MPV_H_MAX;
    }
#endif

#ifdef CC_MPV_V_MAX
    if (u2VSize > CC_MPV_V_MAX)
    {
        u2VSize = CC_MPV_V_MAX;
    }
#endif

    if (FBM_ChkFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_RELEASE_FBG))
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        FBM_WaitUnlockFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, MPV_WAIT_DISP_UNLOCK_TIME);    // block until display unlock
        FBM_ReleaseGroup(_prVdecEsInfo[ucEsId]->ucFbgId);
        _prVdecEsInfo[ucEsId]->ucFbgId = FBM_FBG_ID_UNKNOWN;
        CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
        CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_HALF_REF);
        LOG(3, "Release FBG 0\n");
    }

    ucFbgType = FBM_SelectGroupType(u2HSize, u2VSize);

#ifdef __MODEL_slt__
    // in 5361 model, the frame buffer is limited to SD size.
    // we could not use HD to get frame buffer.
    ucFbgType = FBM_FBG_TYPE_1080HD;
#endif

    if (_prVdecEsInfo[ucEsId]->ucFbgId == FBM_FBG_ID_UNKNOWN)
    {
        LOG(5, "FbgType(%d)\n", ucFbgType);

        _prVdecEsInfo[ucEsId]->ucFbgId = FBM_CreateGroup(ucFbgType, FBM_VDEC_MPEG2, 0, 0);
        if(_prVdecEsInfo[ucEsId]->fgMMPlayback)
        {
            FBM_SetPlayMode(_prVdecEsInfo[ucEsId]->ucFbgId,FBM_FBG_MM_MODE);
        }
        if (_prVdecEsInfo[ucEsId]->ucFbgId == FBM_FBG_ID_UNKNOWN)
        {
            _prVdecEsInfo[ucEsId]->u4FbgFailCnt++;
        #ifdef CC_MINI_DRIVER
            //if (prVdecEsInfo->pfDecNfyFct)
            //{
            //    prVdecEsInfo->pfDecNfyFct(prVdecEsInfo->pvDecNfyTag, MPV_COND_NO_FBG, 0, 0);
            //}
        #endif
            return (FBG_ID_UNKNOWN);
        }

        FBM_SetSyncStc(_prVdecEsInfo[ucEsId]->ucFbgId,
            _prVdecEsInfo[ucEsId]->ucSyncMode, _prVdecEsInfo[ucEsId]->ucStcId);

        // reset flag
        CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
        CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_HALF_REF);

        #ifdef FPGA_RACING_FULLB
        {
            SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_FULLB);
            LOG(5, "MPV_RACING_MODE_FULLB\n");
        }
        #elif defined (FPGA_RACING_HALFB)
        {
            SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_HALFB);
            LOG(5, "MPV_RACING_MODE_HALFB\n");
        }
        #else
            #ifdef MPV_SEMI_HOST
            if(prEsInfo->fgRatioCLI)
            {
                FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId], FBM_FLAG_REDUCE_REF);
                SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF);
            }
            else
            #endif
            {
                ucFbNs = FBM_GetFrameBufferNs(_prVdecEsInfo[ucEsId]->ucFbgId);
                if ((ucFbNs >= FBM_DBK_FB_NS) && (prVdecInfo->eCurState != VDEC_ST_PLAY_I_FRAME))
                {
                    prEsInfo->fgDbkFbg = TRUE;

                    if (!prMpvInfo->fgDisableDbk)
                    {
                        #ifdef CC_MPV_DEBLOCKING
                        SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
                        FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_DEBLOCKING);
                        LOG(5, "DBK Enable\n");
                        #endif
                    }
                    else
                    {
                        LOG(5, "DBK Disable\n");
                    }
                }
                else if (ucFbNs == FBM_RACING_FULLB_FB_NS)
                {
                    SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_FULLB);
                    LOG(5, "MPV_RACING_MODE_FULLB\n");
                }
            }
        #endif

        if (FBM_ChkFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_REDUCE_REF))
        {
            CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
            FBM_ClrFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_DEBLOCKING);
            SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF);
            LOG(3, "RR mode\n");
        }

        if(prEsInfo->fgRatioCLI)
        {
            CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
            FBM_ClrFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_DEBLOCKING);
            LOG(5, "DBK Disable\n");
            SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF);
        }

        prEsInfo->fgChgFbgT = FALSE;
        prEsInfo->ucFbgType = ucFbgType;
        FBM_FbgChgNotify(_prVdecEsInfo[ucEsId]->ucFbgId, ucEsId);
        FBM_SetSyncStc(_prVdecEsInfo[ucEsId]->ucFbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);

        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prVdecEsInfo->ucFbgId);
            FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_NOT_READY);
            //FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM);
        }

        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
        if(prFbmSeqHdr == NULL)
        {
            LOG(1, "_SequenceHdr prFbmSeqHdr null\n");
            return NONE_ERROR;
        }
        prFbmSeqHdr->ucActFmt = ACT_FMT_DEFAULT;
        FBM_GetFrameBufferSize(_prVdecEsInfo[ucEsId]->ucFbgId, &u4FbWidth, &u4FbHeight);

        if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF))
        {
            prFbmSeqHdr->fgYRatio34 = (BOOL)prEsInfo->u4RRatio;
            prFbmSeqHdr->fgCRatio34 = (BOOL)prEsInfo->u4RRatio;
            prFbmSeqHdr->fgRRYEn = TRUE;
            prFbmSeqHdr->fgRRCEn = TRUE;

            FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_REDUCE_REF);
        }

#ifdef CC_RESIZE_SMALL_IMAGE
        // because we doese not change frame buffer group,
        // this variable is never get cleanned.
        // When we have small image and change to normal,
        // we will still use resize. That is incorrect.
        prFbmSeqHdr->fgResizeSmallPic = FALSE;
#endif
        
        prFbmSeqHdr->u2HSize = u2HSize;
        prFbmSeqHdr->u2VSize = u2VSize;
        prFbmSeqHdr->u2OrgHSize = u2HSize;
        prFbmSeqHdr->u2OrgVSize = u2VSize;

        #ifdef MPV_SEMI_HOST
        prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(prFbmSeqHdr->u2OrgHSize,
                                                    MPV_BLK_ALIGMENT);
        #else
        prFbmSeqHdr->u2LineSize = (UINT16)u4FbWidth;
        #if 0//def CC_MT5360,
        //even using this method, we have the same bug as 5391
        prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(prFbmSeqHdr->u2OrgHSize,
                                                    MPV_DBK_ALIGMENT);
        #endif
        #endif

        if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF))
        {
        #ifdef MPV_SEMI_HOST
            prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK((prFbmSeqHdr->u2LineSize >> 1),
                                                        MPV_BLK_ALIGMENT);
        #else
            //prFbmSeqHdr->u2LineSize = prFbmSeqHdr->u2LineSize >> 1; ;
            prFbmSeqHdr->u2LineSize = prFbmSeqHdr->u2LineSize;
        #endif
        }

        LOG(6, "0 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                   prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,
                   prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);

        prVdecEsInfo->u2LineSize = prFbmSeqHdr->u2LineSize;
        prVdecEsInfo->u2HSize = prFbmSeqHdr->u2HSize;
        prVdecEsInfo->u2VSize = prFbmSeqHdr->u2VSize;
        prVdecEsInfo->u2OrgHSize = prFbmSeqHdr->u2OrgHSize;
        prVdecEsInfo->u2OrgVSize = prFbmSeqHdr->u2OrgVSize;

#ifdef CC_RESIZE_SMALL_IMAGE
        if((u2HSize < FBM_IMG_RESIZE_LIMITATION) ||
           (u2VSize < FBM_IMG_RESIZE_LIMITATION))
        {
            prFbmSeqHdr->fgResizeSmallPic = TRUE;
            if(u2HSize < FBM_IMG_RESIZE_LIMITATION)
            {
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            }
            else if(u2HSize > FBM_IMG_RESIZE_LIMITATION_MAX)
            {
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            }
            else
            {
                prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(u2HSize,
                                                            MPV_BLK_ALIGMENT);
                prFbmSeqHdr->u2HSize = prFbmSeqHdr->u2LineSize;
                prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2LineSize;
            }
            if(u2VSize < FBM_IMG_RESIZE_LIMITATION)
            {
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            }
            else if(u2VSize > FBM_IMG_RESIZE_LIMITATION_MAX)
            {
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            }
            else
            {
                prFbmSeqHdr->u2VSize = MPV_ALIGN_MASK(u2VSize,
                                                        MPV_BLK_ALIGMENT);
                prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
            }

            LOG(6, "-> 0 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                       prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,
                       prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);
        }
#endif

        prMpvInfo->fgChgResolution[ucEsId] = TRUE;
    }
    else
    {
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);

        if(prFbmSeqHdr == NULL)
        {
            LOG(1, "_SequenceHdr prFbmSeqHdr null\n");
            return NONE_ERROR;
        }

        if (ucFbgType == FBM_FBG_TYPE_UNKNOWN)
        {
            LOG(3, "Invalid HSize: %d, VSize: %d\n", u2HSize, u2VSize);
            return(SEQH_LOC_ERR);
        }

        u2OriHSize = prVdecEsInfo->u2OrgHSize;
        u2OriVSize = prVdecEsInfo->u2OrgVSize;

        if ((ucFbgType != prEsInfo->ucFbgType) || prEsInfo->fgChgFbgT)
        {
            LOG(3, "Fbg Type Change %d\n", ucFbgType);
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);

            FBM_WaitUnlockFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, MPV_WAIT_DISP_UNLOCK_TIME);    // block until display unlock

            FBM_ReleaseGroup(_prVdecEsInfo[ucEsId]->ucFbgId);
            _prVdecEsInfo[ucEsId]->ucFbgId = FBM_FBG_ID_UNKNOWN;
            CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
            CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_HALF_REF);

            _prVdecEsInfo[ucEsId]->ucFbgId = FBM_CreateGroup(ucFbgType, FBM_VDEC_MPEG2, 0, 0);
            if(_prVdecEsInfo[ucEsId]->fgMMPlayback)
            {
                FBM_SetPlayMode(_prVdecEsInfo[ucEsId]->ucFbgId,FBM_FBG_MM_MODE);
            }
            //ASSERT(_prVdecEsInfo[ucEsId]->ucFbgId != FBM_FBG_ID_UNKNOWN);
            if (_prVdecEsInfo[ucEsId]->ucFbgId == FBM_FBG_ID_UNKNOWN)
            {
                _prVdecEsInfo[ucEsId]->u4FbgFailCnt++;
                return (FBG_ID_UNKNOWN);
            }

            FBM_SetSyncStc(_prVdecEsInfo[ucEsId]->ucFbgId,
                _prVdecEsInfo[ucEsId]->ucSyncMode, _prVdecEsInfo[ucEsId]->ucStcId);

            // reset variable, ilii
            //_prVdecEsInfo[ucEsId]->ucDbkRefId = FBM_FB_ID_UNKNOWN;
            //_prVdecEsInfo[ucEsId]->ucDbkFbId = FBM_FB_ID_UNKNOWN;
            //_prVdecEsInfo[ucEsId]->ucFbId = FBM_FB_ID_UNKNOWN;

            #ifdef FPGA_RACING_FULLB
            {
                SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_FULLB);
                LOG(5, "MPV_RACING_MODE_FULLB\n");
            }
            #elif defined (FPGA_RACING_HALFB)
            {
                SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_HALFB);
                LOG(5, "MPV_RACING_MODE_HALFB\n");
            }
            #else
                #ifdef MPV_SEMI_HOST
                if(prEsInfo->fgRatioCLI)
                {
                    FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId], FBM_FLAG_REDUCE_REF);
                    SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF);
                }
                else
                #endif
                {
                        ucFbNs = FBM_GetFrameBufferNs(_prVdecEsInfo[ucEsId]->ucFbgId);

                        if ((ucFbNs >= FBM_DBK_FB_NS) && (prVdecInfo->eCurState != VDEC_ST_PLAY_I_FRAME))
    		            {
    		                prEsInfo->fgDbkFbg = TRUE;

    		                if (!prMpvInfo->fgDisableDbk)
    		                {
                                #ifdef CC_MPV_DEBLOCKING
    		                    SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
    		                    FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_DEBLOCKING);
    		                    LOG(5, "DBK Enable\n");
    		                    #endif
    		                }
    		                else
    		                {
    		                    LOG(5, "DBK Disable\n");
    		                }
    		            }
    		            else if (ucFbNs == FBM_RACING_FULLB_FB_NS)
    		            {
    		                SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_FULLB);
    		                LOG(5, "MPV_RACING_MODE_FULLB\n");
    		            }
                }
            #endif

            if (FBM_ChkFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_REDUCE_REF))
            {
                CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
                FBM_ClrFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_DEBLOCKING);
                SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF);
                LOG(3, "HalfRef mode\n");
            }

            if(prEsInfo->fgRatioCLI)
            {
                CLR_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK);
                FBM_ClrFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_DEBLOCKING);
                LOG(5, "DBK Disable\n");
                SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF);
            }

            MPV_EventNotify(MPV_SD_HD, ucEsId, prEsInfo->ucFbgType, ucFbgType);
            prEsInfo->fgChgFbgT = FALSE;
            prEsInfo->ucFbgType = ucFbgType;
            FBM_FbgChgNotify(_prVdecEsInfo[ucEsId]->ucFbgId, ucEsId);
            FBM_SetSyncStc(_prVdecEsInfo[ucEsId]->ucFbgId, _prVdecEsInfo[ucEsId]->ucSyncMode, _prVdecEsInfo[ucEsId]->ucStcId);

            // Check if reset is required
            if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET))
            {
                FBM_ResetGroup(prVdecEsInfo->ucFbgId);
                FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET);
                FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_NOT_READY);
                //FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM);
            }

            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
            if(prFbmSeqHdr == NULL)
            {
                ASSERT(prFbmSeqHdr != NULL);
                LOG(1, "prFbmSeqHdr null\n");
                return FB_ID_UNKNOWN;
            }
            prFbmSeqHdr->ucActFmt = ACT_FMT_DEFAULT;
            FBM_GetFrameBufferSize(_prVdecEsInfo[ucEsId]->ucFbgId, &u4FbWidth, &u4FbHeight);

            if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF))
            {
                prFbmSeqHdr->fgYRatio34 = (BOOL)prEsInfo->u4RRatio;
                prFbmSeqHdr->fgCRatio34 = (BOOL)prEsInfo->u4RRatio;
                prFbmSeqHdr->fgRRYEn = TRUE;
                prFbmSeqHdr->fgRRCEn = TRUE;

                FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_REDUCE_REF);
            }

#ifdef CC_RESIZE_SMALL_IMAGE
            // because we doese not change frame buffer group,
            // this variable is never get cleanned.
            // When we have small image and change to normal,
            // we will still use resize. That is incorrect.
            prFbmSeqHdr->fgResizeSmallPic = FALSE;
#endif
            
            prFbmSeqHdr->u2HSize = u2HSize;
            prFbmSeqHdr->u2VSize = u2VSize;
            prFbmSeqHdr->u2OrgHSize = u2HSize;
            prFbmSeqHdr->u2OrgVSize = u2VSize;

        #ifdef MPV_SEMI_HOST
            prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(prFbmSeqHdr->u2OrgHSize,
                                                        MPV_BLK_ALIGMENT);
        #else
            prFbmSeqHdr->u2LineSize = (UINT16)u4FbWidth;
            #if 0//def CC_MT5360
            prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(prFbmSeqHdr->u2OrgHSize,
                                                        MPV_DBK_ALIGMENT);
            #endif
        #endif

            if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF))
            {
            #ifdef MPV_SEMI_HOST
                prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK((prFbmSeqHdr->u2LineSize >> 1),
                                                        MPV_BLK_ALIGMENT);
            #else
                // ilii both
                //prFbmSeqHdr->u2LineSize = prFbmSeqHdr->u2LineSize >> 1; ;
                prFbmSeqHdr->u2LineSize = prFbmSeqHdr->u2LineSize;
            #endif
            }

            LOG(6, "1 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                       prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,
                       prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);

            prVdecEsInfo->u2LineSize = prFbmSeqHdr->u2LineSize;
            prVdecEsInfo->u2HSize = prFbmSeqHdr->u2HSize;
            prVdecEsInfo->u2VSize = prFbmSeqHdr->u2VSize;
            prVdecEsInfo->u2OrgHSize = prFbmSeqHdr->u2OrgHSize;
            prVdecEsInfo->u2OrgVSize = prFbmSeqHdr->u2OrgVSize;

#ifdef CC_RESIZE_SMALL_IMAGE
            if((u2HSize < FBM_IMG_RESIZE_LIMITATION) ||
               (u2VSize < FBM_IMG_RESIZE_LIMITATION))
            {
                prFbmSeqHdr->fgResizeSmallPic = TRUE;
                if(u2HSize < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                }
                else if(u2HSize > FBM_IMG_RESIZE_LIMITATION_MAX)
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                }
                else
                {
                    prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(u2HSize,
                                                                MPV_BLK_ALIGMENT);
                    prFbmSeqHdr->u2HSize = prFbmSeqHdr->u2LineSize;
                    prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2LineSize;
                }
                if(u2VSize < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                }
                else if(u2VSize > FBM_IMG_RESIZE_LIMITATION_MAX)
                {
                    prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                }
                else
                {
                    prFbmSeqHdr->u2VSize = MPV_ALIGN_MASK(u2VSize,
                                                            MPV_BLK_ALIGMENT);
                    prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
                }

                LOG(6, "-> 1 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                       prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,
                       prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);
            }
#endif

            prMpvInfo->fgChgResolution[ucEsId] = TRUE;

        }
        else if ((u2HSize != u2OriHSize) || (u2VSize != u2OriVSize))
        {
            prFbmSeqHdr->ucActFmt = ACT_FMT_DEFAULT;
            FBM_GetFrameBufferSize(_prVdecEsInfo[ucEsId]->ucFbgId, &u4FbWidth, &u4FbHeight);

            if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF))
            {
                prFbmSeqHdr->fgYRatio34 = (BOOL)prEsInfo->u4RRatio;
                prFbmSeqHdr->fgCRatio34 = (BOOL)prEsInfo->u4RRatio;
                prFbmSeqHdr->fgRRYEn = TRUE;
                prFbmSeqHdr->fgRRCEn = TRUE;

                FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_REDUCE_REF);
            }

#ifdef CC_RESIZE_SMALL_IMAGE
            // because we doese not change frame buffer group,
            // this variable is never get cleanned.
            // When we have small image and change to normal,
            // we will still use resize. That is incorrect.
            prFbmSeqHdr->fgResizeSmallPic = FALSE;
#endif
            
            prFbmSeqHdr->u2HSize = u2HSize;
            prFbmSeqHdr->u2VSize = u2VSize;
            prFbmSeqHdr->u2OrgHSize = u2HSize;
            prFbmSeqHdr->u2OrgVSize = u2VSize;

            #ifdef MPV_SEMI_HOST
            prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(prFbmSeqHdr->u2OrgHSize,
                                                        MPV_BLK_ALIGMENT);
            #else
                prFbmSeqHdr->u2LineSize = (UINT16)u4FbWidth;
                #if 0//def CC_MT5360
                prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(prFbmSeqHdr->u2OrgHSize,
                                                            MPV_DBK_ALIGMENT);
                #endif
            #endif

            if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_REDUCE_REF))
            {
            #ifdef MPV_SEMI_HOST
                prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK((prFbmSeqHdr->u2LineSize >> 1),
                                                        MPV_BLK_ALIGMENT);
            #else
                prFbmSeqHdr->u2LineSize = prFbmSeqHdr->u2LineSize;
            #endif
            }

            LOG(6, "2 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                       prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,
                       prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);

            prVdecEsInfo->u2LineSize = prFbmSeqHdr->u2LineSize;
            prVdecEsInfo->u2HSize = prFbmSeqHdr->u2HSize;
            prVdecEsInfo->u2VSize = prFbmSeqHdr->u2VSize;
            prVdecEsInfo->u2OrgHSize = prFbmSeqHdr->u2OrgHSize;
            prVdecEsInfo->u2OrgVSize = prFbmSeqHdr->u2OrgVSize;

#ifdef CC_RESIZE_SMALL_IMAGE
            if((u2HSize < FBM_IMG_RESIZE_LIMITATION) ||
               (u2VSize < FBM_IMG_RESIZE_LIMITATION))
            {
                prFbmSeqHdr->fgResizeSmallPic = TRUE;
                if(u2HSize < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                }
                else if(u2HSize > FBM_IMG_RESIZE_LIMITATION_MAX)
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                }
                else
                {
                    prFbmSeqHdr->u2LineSize = MPV_ALIGN_MASK(u2HSize,
                                                                MPV_BLK_ALIGMENT);
                    prFbmSeqHdr->u2HSize = prFbmSeqHdr->u2LineSize;
                    prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2LineSize;
                }
                if(u2VSize < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                }
                else if(u2VSize > FBM_IMG_RESIZE_LIMITATION_MAX)
                {
                    prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                }
                else
                {
                    prFbmSeqHdr->u2VSize = MPV_ALIGN_MASK(u2VSize,
                                                            MPV_BLK_ALIGMENT);
                    prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
                }

                LOG(6, "-> 2 OrgHSize %d, OrgVSize %d, HSize %d, VSize %d, LineSize %d\n",
                       prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,
                       prFbmSeqHdr->u2HSize, prFbmSeqHdr->u2VSize, prFbmSeqHdr->u2LineSize);
            }
#endif

            prMpvInfo->fgChgResolution[ucEsId] = TRUE;
        }
    }

    ucTemp = (UCHAR)rSeqHdr.rField.u4AspectRatio;
    if ((ucTemp == 0) || (ucTemp == 15))
    {
#if (ASP_RAT_RES)
        return(ASP_RAT_RES);
#else
        ucTemp = 2;       ///3:4
#endif //ASP_RAT_RES
    }

    if (prFbmSeqHdr->ucAspRatInf != ucTemp)
    {
        prFbmSeqHdr->ucAspRatInf = ucTemp;
        prMpvInfo->fgChgResolution[ucEsId] = TRUE;
    }

    ucTemp = (UCHAR)rSeqHdr.rField.u4FrameRate;
    if ((ucTemp == 0) || (ucTemp > 8))
    {
#if (FRAME_RATE_ERR)
        return(FRAME_RATE_ERR);
#else
        ucTemp = 4; //29.97Hz
#endif //FRAME_RATE_ERR
    }

    prMpvCounter->ucFrameRate = ucTemp;

    if (prFbmSeqHdr->ucFrmRatCod != ucTemp)
    {
        prFbmSeqHdr->ucFrmRatCod = ucTemp;
        prMpvInfo->fgChgResolution[ucEsId] = TRUE;
    }

    prFbmSeqHdr->u4BitRatVal = rSeqHdr.rField.u4BitRate;

    prEsInfo->fgLoadIntraMatrix = rSeqHdr.rField.fgLoadIntra;
    if (prEsInfo->fgLoadIntraMatrix)
    {
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 31);
        LOG(7, "Seq Q-Matrix Ext (I)\n");

        _SaveQuantMatrix(ucMpvId, ucEsId, TRUE);

        u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
        rSeqHdr.rField.fgLoadNonIntra = (u4Datain >> 31);
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 1);
    }
    else
    {
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 32);
    }

    prEsInfo->fgLoadNonIntraMatrix = rSeqHdr.rField.fgLoadNonIntra;
    if (prEsInfo->fgLoadNonIntraMatrix)
    {
        LOG(7, "Seq Q-Matrix Ext (NI)\n");

        _SaveQuantMatrix(ucMpvId, ucEsId, FALSE);
    }

    if (prEsInfo->fgLoadIntraMatrix || prEsInfo->fgLoadNonIntraMatrix)
    {
        prEsInfo->u4MatrixId = _au4QMatrixCounter[ucMpvId]++;
    }

#if 0
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

    if ((u4Datain != GROUP_START_CODE) &&
       (u4Datain != USER_DATA_START_CODE)&&
       (u4Datain != PICTURE_START_CODE))
    {
         prEsInfo->ucMpegVer = MPEG2;
    }
#endif

    //if (!_NextStartCode(ucMpvId, ucEsId))
	if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
    {
        return(NO_START_C_ERR1);
    }

     // parsing of MPEG2 sequence extension
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
    if (u4Datain == EXTENSION_START_CODE)
    {
        MPEG2_SEQ_EXT_HDR_UNION_T     rSeqExtHdr;

        prEsInfo->ucMpegVer = MPEG2;

        // Get Sequence Extension Header
        rSeqExtHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 32);
        rSeqExtHdr.au4Reg[1] = _GetBitStream(ucMpvId, ucEsId, 32);

        prVdecEsInfo->ucProfileId = (UCHAR)rSeqExtHdr.rField.u4ProfileId;
        prVdecEsInfo->u4VbvSizeExt = rSeqExtHdr.rField.u4VbvSizeExt;
        prVdecEsInfo->fgLowDelay = rSeqExtHdr.rField.fgLowDelay;
        
#if (SEQE_MKB_ERR)
        if (rSeqExtHdr.rField.fgMark == 0)
        {
            return (SEQE_MKB_ERR);
        }
#endif

        // extension start code identifier 0, 6, >10 reserved
#if (EXT_ST_COD_ERR)
        ucTemp = (UCHAR)rSeqExtHdr.rField.u4ExtId;
        if ((ucTemp == 0) || (ucTemp == 6) || (ucTemp > 10) )
        {
            return (EXT_ST_COD_ERR);
        }
#endif

#if (PROF_ID_RES)
        ucTemp = (UCHAR)rSeqExtHdr.rField.u4ProfileId;
        if ((ucTemp == 0) || (ucTemp >= 6))
        {
            return(PROF_ID_RES);
        }
#endif

#if (LEV_ID_RES)
        ucTemp = (UCHAR)rSeqExtHdr.rField.u4LevelId;
        if ((ucTemp != LEVEL_ID_LOW) &&
            (ucTemp != LEVEL_ID_MAIN) &&
            (ucTemp != LEVEL_ID_HIGH_1440) &&
            (ucTemp != LEVEL_ID_HIGH))
        {
            return(LEV_ID_RES);
        }
#endif

        if (prFbmSeqHdr->fgProgressiveSeq != rSeqExtHdr.rField.fgPrgsSeq)
        {
            prFbmSeqHdr->fgProgressiveSeq = rSeqExtHdr.rField.fgPrgsSeq;
            prMpvInfo->fgChgResolution[ucEsId] = TRUE;
        }

        prFbmSeqHdr->ucCrmaFmt = (UCHAR)rSeqExtHdr.rField.u4ChromaFmt;
#if (CH_FORMAT_RES)
        if (prFbmSeqHdr->ucCrmaFmt == 0)
        {
            return(CH_FORMAT_RES);
        }
#endif

        //prVdecEsInfo->u2OrgHSize += (rSeqExtHdr.rField.u4HSizeExt << 12);
        //prVdecEsInfo->u2OrgVSize += (rSeqExtHdr.rField.u4VSizeExt << 12);

        // get 16 bits off
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 16);

        //if (!_NextStartCode(ucMpvId, ucEsId))
		if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
        {
            return(NO_START_C_ERR3);
        }
    }
    else
    {
        //  DTV00002904
        //Playing 20060120-1226_ATSC_KR_Suwon_HCastle_Ch61.trp with mt5111 causes system crash.
        // In DTV, we might get error width/height
        // DTV00134865, 20090416: change limit on DTV only
        if ((!prVdecEsInfo->fgMMPlayback) &&
            ((prVdecEsInfo->u2OrgHSize > MPEG1_MAX_HSIZE) ||
             (prVdecEsInfo->u2OrgVSize > MPEG1_MAX_VSIZE)))
        {
            return (MPEG_VER_ERR);
        }

        //prFbmSeqHdr->fgProgressiveSeq = TRUE;
        // MPEG1 default settings, check progressive
        if (!prFbmSeqHdr->fgProgressiveSeq)
        {
            prFbmSeqHdr->fgProgressiveSeq = TRUE;
            prMpvInfo->fgChgResolution[ucEsId] = TRUE;
        }
    }

    // DTV00134865, 20090416: limit playback width/height
    if ((prVdecEsInfo->fgMMPlayback) &&
        ((prVdecEsInfo->u2OrgHSize > MPEG_MAX_HSIZE) ||
         (prVdecEsInfo->u2OrgVSize > MPEG_MAX_VSIZE)))
    {
        return (MPEG_VER_ERR);
    }


    if (prMpvInfo->fgChgResolution[ucEsId])
    {
        // move below to get MatrixCoefficient
        /*
        if (!prEsInfo->fgVideoLock)
        {
            prEsInfo->fgVideoLock = TRUE;
        }
        */
        fgVideoLock = TRUE;

        FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_SEQ_CHG);

#ifndef __MODEL_slt__
        if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK))
        {
            prEsInfo->u4DbkXEnd = 0;
            prEsInfo->u4DbkYEnd = 0;
        }
#else
        prEsInfo->u4DbkYEnd = 0;
        prEsInfo->u4DbkXEnd = 0;
#endif

        //Notify Middleware Resolution Change
        #if 0 // this notify already move to vdp
        // due to scart out, mw ask us to notify too.
        // i.e. vdp and mpv both notify mw resolution change.
        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_RESOLUTION_CHG, 0, 0);
        }
        #endif

        MPV_AVDecCb(ucEsId, &(prVdecEsInfo->rAVDecStatus), AVDEC_STATUS_VIDEOLOCK);
        MPV_AVDecCb(ucEsId, &(prVdecEsInfo->rAVDecStatus), AVDEC_STATUS_VIDEOINFO);

        switch (prFbmSeqHdr->ucFrmRatCod)
        {
            case MPEG_FRAME_RATE_24_:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_23_976;
                break;
            case MPEG_FRAME_RATE_24:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_24;
                break;

            case MPEG_FRAME_RATE_25:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_25;
                break;

            case MPEG_FRAME_RATE_30_:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_29_97;
                break;
            case MPEG_FRAME_RATE_30:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_30;
                break;

            case MPEG_FRAME_RATE_50:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_50;
                break;

            case MPEG_FRAME_RATE_60_:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_59_94;
                break;
            case MPEG_FRAME_RATE_60:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_60;
                break;

            case MPEG_FRAME_RATE_UNKNOWN:
            default:
                prVdecEsInfo->u4FrameRate = VDEC_FRAME_RATE_1000_60;
                break;
        }

        prMpvInfo->fgChgResolution[ucEsId] = FALSE;
    }

    if (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME)
    {
#ifdef VDP_WORKAROUND
        prFbmSeqHdr->fgProgressiveSeq = TRUE;
#endif

        if (!prVdecEsInfo->fgIFrameDec)
        {
            if (prVdecEsInfoKeep->pfDecNfyFct)
            {
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_I_FRAME_SET_BUFF_DONE, 0, 0);
            }
            return (NO_DECODE);
        }
    }

    fgDisable = FALSE;
    if (prEsInfo->ucMpegVer == MPEG2)    // MPEG2 sequence display/scalable extension
    {
    	prVdecEsInfo->fgMPEG2 = TRUE;
    	
        if(prVdecInfo->u4OpCode & VDEC_OP_DISABLE_MPEG2)
        {
            fgDisable = TRUE;
        }
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_PS_MPEG2_DISABLE
        if((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG1_DAT) ||
           (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_PS))
        {
            fgDisable = TRUE;
        }
#endif
#ifdef CC_ES_MPEG2_DISABLE
        if(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_VIDEO_ES)
        {
            fgDisable = TRUE;
        }
#endif
#endif
        if(fgDisable)
        {
            if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
            {
                //callback, codec not support mpv data
                if (prVdecEsInfoKeep->pfDecNfyFct)
                {
                    LOG(3, "Notify Status change, mpeg2 codec not support\n");
                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                    VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                }
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
            }
            return (NO_DECODE_MPEG2);
        }
        i4Ret = _ExtUsrData(ucMpvId, ucEsId, 0);
    }
    else
    {
    	prVdecEsInfo->fgMPEG2 = FALSE;
    	
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_PS_MPEG1_DISABLE
        if((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG1_DAT) ||
           (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_PS))
        {
            fgDisable = TRUE;
        }
#endif
#ifdef CC_ES_MPEG1_DISABLE
        if(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_VIDEO_ES)
        {
            fgDisable = TRUE;
        }
#endif
#endif
        if(fgDisable)
        {
            if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
            {
                //callback, codec not support mpv data
                if (prVdecEsInfoKeep->pfDecNfyFct)
                {
                    LOG(3, "Notify Status change, mpeg1 codec not support\n");
                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                    VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                }
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
            }
            return (NO_DECODE_MPEG1);
        }
        i4Ret = _ExtUsrData(ucMpvId, ucEsId, 1);
    }

    // to make sure we got MatrixCoefficient.
    if(fgVideoLock)
    {
        if (!prEsInfo->fgVideoLock)
        {
            prEsInfo->fgVideoLock = TRUE;
        }
    }

    return i4Ret;
}


static INT32 _GroupPicHdr(UCHAR ucMpvId, UCHAR ucEsId)            //group_of_picture_header()
{
    UINT32 u4Datain;
//    MPV_ES_INFO_T* prEsInfo;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    MPEG2_GOP_HDR_UNION_T rGopHdr;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

 //   prEsInfo = _prEsInfo[ucEsId];
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
    if(prFbmSeqHdr == NULL)
    {
        LOG(1, "_GroupPicHdr prFbmSeqHdr null\n");
        return NONE_ERROR;
    }

    rGopHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 32);

#if (GOP_MKB_ERR)
    if (rGopHdr.rField.fgMark == 0)
    {
        //return(GOP_MKB_ERR);
    }
#endif

    prFbmSeqHdr->fgClosedGop = rGopHdr.rField.fgClosedGop;
    prFbmSeqHdr->fgBrokenLink = rGopHdr.rField.fgBrokenLink;
    prFbmSeqHdr->ucSeconds = (UCHAR)rGopHdr.rField.u4Second;
    prFbmSeqHdr->ucMinutes = (UCHAR)rGopHdr.rField.u4Mintue;
    prFbmSeqHdr->ucHours = (UCHAR)rGopHdr.rField.u4Hour;
    prFbmSeqHdr->ucPictures = (UCHAR)rGopHdr.rField.u4Picture;

    // get 27 bits off
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 27);
    UNUSED(u4Datain);

    //if (!_NextStartCode(ucMpvId, ucEsId))
    //if (!_HwNextStartCode(ucMpvId, ucEsId, HW_STARTCODE_TIMEOUT))
    if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
    {
        return (NO_START_C_ERR9);
    }

    return _ExtUsrData(ucMpvId, ucEsId, 1);
}


static INT32 _PicHdr(UCHAR ucMpvId, UCHAR ucEsId)
{
    BOOL fgFieldErr = FALSE;
    BOOL fgDisable;
    UCHAR ucTemp;
    UINT32 u4Datain;

    MPV_ES_INFO_T* prEsInfo;
    VDEC_INFO_T* prVdecInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    FBM_PIC_HDR_T *prFbmPicHdr;
    MPEG2_PIC_HDR_UNION_T rPicHdr;
    PARAM_MPV_T* prMpvCounter;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return NONE_ERROR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return NONE_ERROR;
    }

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

#ifdef MPV_138184
   FBM_SEQ_HDR_T *prFbmSeqHdr;

   prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
#endif

    prEsInfo = _prEsInfo[ucEsId];

    rPicHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 32);

    ucTemp = (UCHAR)rPicHdr.rField.u4PicType;
#if(PIC_COD_TYP_ERR)
    if((ucTemp == 0) || (ucTemp >= 4))
    {
        return (PIC_COD_TYP_ERR);
    }
#endif

    fgDisable = FALSE;
    if (prEsInfo->ucMpegVer == MPEG2)    // MPEG2 sequence display/scalable extension
    {
        if(prVdecInfo->u4OpCode & VDEC_OP_DISABLE_MPEG2)
        {
            fgDisable = TRUE;
        }
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_PS_MPEG2_DISABLE
        if((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG1_DAT) ||
           (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_PS))
        {
            fgDisable = FALSE;
        }
#endif
#ifdef CC_ES_MPEG2_DISABLE
        if(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_VIDEO_ES)
        {
            fgDisable = FALSE;
        }
#endif
#endif
        if(fgDisable)
        {
            if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
            {
                //callback, codec not support mpv data
                if (prVdecEsInfoKeep->pfDecNfyFct)
                {
                    LOG(3, "Notify Status change, mpeg2 codec not support\n");
                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                    VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                }
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
            }
            return (NO_DECODE_MPEG2);
        }
    }
    else if(prEsInfo->ucMpegVer == MPEG1)
    {
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_PS_MPEG1_DISABLE
        if((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG1_DAT) ||
           (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_PS))
        {
            fgDisable = TRUE;
        }
#endif
#ifdef CC_ES_MPEG1_DISABLE
        if(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_VIDEO_ES)
        {
            fgDisable = TRUE;
        }
#endif
#endif
        if(fgDisable)
        {
            if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
            {
                //callback, codec not support mpv data
                if (prVdecEsInfoKeep->pfDecNfyFct)
                {
                    LOG(3, "Notify Status change, mpeg1 codec not support\n");
                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                    VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                }
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
            }
            return (NO_DECODE_MPEG1);
        }
    }

    LOG(6, "Es%d, Pic Type = %d , TemporalRef = %d , PTS = 0x%x\n",
                ucEsId, ucTemp, rPicHdr.rField.u4TemporalRef, _prVdecEsInfo[ucEsId]->u4PTS);

    //Dmx gives the wrong picture type, mpv need to put the last reference pic to DISPQ here.
    if ((_prVdecEsInfo[ucEsId]->ucPicType == B_TYPE) && (ucTemp != B_TYPE))
    {
        if ((!IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLG_2ND_FLD_PIC)) &&
                (_prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
        {
            UCHAR ucBRefFbId;
            UCHAR ucFRefFbId;

            FBM_GetRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, &ucFRefFbId, &ucBRefFbId);

            if (ucBRefFbId != FBM_FB_ID_UNKNOWN)
            {
                UCHAR ucFbStatus;

                ucFbStatus = FBM_GetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId, ucBRefFbId);

                if(ucFbStatus == FBM_FB_STATUS_READY)
                {

                #ifdef CC_MPV_DEBLOCKING
                    if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK) && (!_prVdecEsInfo[ucEsId]->fgRefNoDbk))
                    {
                        if (_prVdecEsInfo[ucEsId]->ucDbkRefId == FBM_FB_ID_UNKNOWN) //change normal mode to DBK mode
                        {
                            LOG_MFBM("MFBM Set R Disp d, %d, %d\n", ucBRefFbId,
                                        	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId))->i4TemporalRef);
                            UNUSED(_MPV_CalcPts(ucEsId, ucBRefFbId));
                            FBM_SetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId,
                                ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                        }
                        else
                        {
                            LOG_MFBM("MFBM Set R Disp e, %d, %d\n", _prVdecEsInfo[ucEsId]->ucDbkRefId,
                                        	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, _prVdecEsInfo[ucEsId]->ucDbkRefId))->i4TemporalRef);
                            UNUSED(_MPV_CalcPts(ucEsId, _prVdecEsInfo[ucEsId]->ucDbkRefId));
                            FBM_SetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId,
                                _prVdecEsInfo[ucEsId]->ucDbkRefId, FBM_FB_STATUS_DISPLAYQ);
                            _prVdecEsInfo[ucEsId]->ucDbkRefId = FBM_FB_ID_UNKNOWN;
                            LOG_MFBM("MFBM ucDbkRefId 0xFF e\n");
                        }
                    }
                    else
                #endif
                    {
                        LOG_MFBM("MFBM Set R Disp f, %d, %d\n", ucBRefFbId,
                                        	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId))->i4TemporalRef);
                        UNUSED(_MPV_CalcPts(ucEsId, ucBRefFbId));
                        FBM_SetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId,
                            ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                    #ifdef VDP_RACE_LOG
                        VDP_FrcRacingLog('Q');
                    #endif
                    }
                }
            }
        }
    }

    if (_prVdecEsInfo[ucEsId]->fgSeqHdr)
    {
        if (ucTemp != I_TYPE)
        {
            prMpvCounter->u4NoIAfterSeqH = 1;
            MPV_EventNotify(MPV_SEQH_NOI, ucEsId, ucTemp, 0);

            if ((ucTemp == P_TYPE) && (!_prEsInfo[ucEsId]->fgDetectIntra) &&
                (prMpvCounter->u4RetrieveNs > 30 ) && (!_prVdecEsInfo[ucEsId]->fgCanDisp))
            {
                _prVdecEsInfo[ucEsId]->fgCanDisp = TRUE;
                _prVdecEsInfo[ucEsId]->fgBisOk = TRUE;
                prEsInfo->fgFreeP = TRUE;
            }
        }
    }

    if (ucTemp == B_TYPE)
    {
        if (!IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLG_2ND_FLD_PIC))
        {
            //Detect B pic Ns between 2 Ref pics
            //if (prEsInfo->fgPreB && (rPicHdr.rField.u4TemporalRef == (prEsInfo->u4PreBTemporalRef + 1)))
            if (prEsInfo->fgPreB)
            {
                prEsInfo->ucBNs++;

                if  ((prEsInfo->ucBNs == 3) &&
                    (!FBM_ChkFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_THREE_B)))
                {
                    LOG(5, "3 B\n");
                    FBM_SetFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_THREE_B);
                }
                else if (prEsInfo->ucBNs > 3)
                {
                    LOG(5, "%d B\n", prEsInfo->ucBNs);
                }
            }
            else
            {
                if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_HALFB))
                {
				    VERIFY(FBM_ResetFbChangeField(_prVdecEsInfo[ucEsId]->ucFbgId) == OSR_OK);
				}
                prEsInfo->ucBNs = 1;
            }
            prEsInfo->fgPreB = TRUE;
        }

    #ifndef MPV_SEMI_HOST
        if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK))
        {
            UINT32 u4DispQCnt;

            u4DispQCnt = FBM_CheckFrameBufferDispQ(_prVdecEsInfo[ucEsId]->ucFbgId);
            if(prVdecEsInfo->fgBigBForceEnable)
            {
                u4DispQCnt = 0;
            }

            if ((prEsInfo->ucBigB != 0) && (u4DispQCnt == 0))
            {
                LOG(5, "Disable DBK\n");
                prEsInfo->fgBNoDbk = TRUE;
#ifdef CC_MT5392B
                if(!prVdecEsInfo->fgOffMjc)
                {
                	LOG(5, "Disable MJC\n");
                    vDrvMJCDynOnOff(FALSE);
                    prVdecEsInfo->fgOffMjc = TRUE;
                }
#endif
            }
#ifdef CC_MT5392B
            else if(prEsInfo->ucBigB == 0)
            {
                if(prVdecEsInfo->fgOffMjc)
                {
                	LOG(5, "Enable MJC\n");
                    vDrvMJCDynOnOff(TRUE);
                    prVdecEsInfo->fgOffMjc = FALSE;
                }
            }
#endif
        }
    #endif
    }
    else
    {
        if ((prEsInfo->ucBNs < 3) &&
            (FBM_ChkFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_THREE_B)))
        {
            LOG(5, "No 3 B\n");
            FBM_ClrFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId, FBM_FLAG_THREE_B);
        }

        prEsInfo->fgPreB = FALSE;

        if (ucTemp == I_TYPE)
        {
            HAL_TIME_T rITime;

            //Record I pic interval
            HAL_GetTime(&rITime);
            HAL_GetDeltaTime(&(prMpvCounter->rIDeltaTime), &(rITime),
                &(prEsInfo->rILastTime));

            prEsInfo->rILastTime = rITime;

            //if (prMpvCounter->u4HaveI == 0)  //the first I
            {
                prMpvCounter->u4HaveI = 1;
                _prVdecEsInfo[ucEsId]->fgCanDisp = TRUE;
                prEsInfo->fgDetectIntra = FALSE;
                prEsInfo->fgFreeP = FALSE;
                _prVdecEsInfo[ucEsId]->fgBisOk = TRUE;
            }
        }
    }

    if ((ucTemp == B_TYPE) && (!_prVdecEsInfo[ucEsId]->fgBisOk))
    {
        return (B_WAIT_INTRA_PIC);
    }

#if 0
#ifndef MPV_VIRTUAL_PRS
    if (prEsInfo->fgDetectRefLost)
    {
        INT32 i4Ret;

        i4Ret = _DetectRefLost(prEsInfo, ucTemp);
        if (i4Ret != NONE_ERROR)
        {
            return i4Ret;
        }
    }
#endif
#endif

    prFbmPicHdr = &(prEsInfo->rFbmPicHdr);

    if (!IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLG_2ND_FLD_PIC))
    {
        prFbmPicHdr->u4PTS = _prVdecEsInfo[ucEsId]->u4PTS;
        prFbmPicHdr->u4TotlaTimeOffset = _prVdecEsInfo[ucEsId]->u4TotlaTimeOffset;
        prFbmPicHdr->u4TickNum = _prVdecEsInfo[ucEsId]->u4TickNum;
        prFbmPicHdr->u8Offset = _prVdecEsInfo[ucEsId]->u8Offset;
        // re-assign in _MPV_CalcPts
        prFbmPicHdr->u8OffsetDisp = _prVdecEsInfo[ucEsId]->u8OffsetLast;
        prFbmPicHdr->u8OffsetI = _prVdecEsInfo[ucEsId]->u8OffsetI;
        prFbmPicHdr->u2DecodingOrder = _prVdecEsInfo[ucEsId]->u2DecodingOrder;

        if ((_prVdecEsInfo[ucEsId]->fgSeqHdr) && (_prVdecEsInfo[ucEsId]->ucPicType == P_TYPE))
        {
            UCHAR ucFRefFbId;
            UCHAR ucBRefFbId;

            if (prEsInfo->fgDetectIntra)
            {
                FBM_GetRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, &ucFRefFbId, &ucBRefFbId);

                if (ucBRefFbId == FBM_FB_ID_UNKNOWN)
                {
                    _MPV_ResetDectectIntra(ucEsId);
                }
            }
            else
            {
                if (!prEsInfo->fgFreeP)
                {
                    FBM_GetRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, &ucFRefFbId, &ucBRefFbId);

                    if (ucBRefFbId == FBM_FB_ID_UNKNOWN)
                    {
                        if (prEsInfo->fgDetectIntra)
                        {
                            _MPV_ResetDectectIntra(ucEsId);
                        }
                        else
                        {
                            // we should NOT decode P picture if backward reference invalid
                            // DTV00210392 should calculate PTS
                            if((prVdecEsInfo->fgMMPlayback)&&(_prVdecEsInfo[ucEsId]->u4PTS>prVdecEsInfo->u4PTSLast))
                            {
                                prVdecEsInfo->u4PTSLast = _prVdecEsInfo[ucEsId]->u4PTS;
                                LOG(6, " calcl u4PTSLast=0x%x\n",prVdecEsInfo->u4PTSLast);
                            }
                            return P_REFERNCE_INVALID;
                        }
                    }
                }
            }
        }
        else if (_prVdecEsInfo[ucEsId]->ucPicType == P_TYPE)
        {
            UCHAR ucFRefFbId;
            UCHAR ucBRefFbId;

            if (!prEsInfo->fgFreeP)
            {
                FBM_GetRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, &ucFRefFbId, &ucBRefFbId);

                if (ucBRefFbId == FBM_FB_ID_UNKNOWN)
                {
                    if (prEsInfo->fgDetectIntra)
                    {
                        _MPV_ResetDectectIntra(ucEsId);
                    }
                    else
                    {
                        // we should NOT decode P picture if backward reference invalid
                        // DTV00210392 should calculate PTS
                        if((prVdecEsInfo->fgMMPlayback)&&(_prVdecEsInfo[ucEsId]->u4PTS>prVdecEsInfo->u4PTSLast))
                        {
                            prVdecEsInfo->u4PTSLast = _prVdecEsInfo[ucEsId]->u4PTS;
                            LOG(6, " calcl u4PTSLast=0x%x\n",prVdecEsInfo->u4PTSLast);
                        }
                        return P_REFERNCE_INVALID;
                    }
                }
            }
        }
        else if (ucTemp == B_TYPE)
        {
            UCHAR ucFRefFbId;
            UCHAR ucBRefFbId;

            FBM_GetRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, &ucFRefFbId, &ucBRefFbId);

        #ifdef MPV_138184
            if (prFbmSeqHdr->fgClosedGop)
            {
                if (ucBRefFbId == FBM_FB_ID_UNKNOWN)
                {
                    return B_REFERNCE_INVALID;
                }
            }
            else
        #endif
            if ((ucBRefFbId == FBM_FB_ID_UNKNOWN) ||
                        (ucFRefFbId == FBM_FB_ID_UNKNOWN))
            {
                // we should NOT decode P picture if backward reference invalid
                // DTV00210392 should calculate PTS
                if((prVdecEsInfo->fgMMPlayback)&&(_prVdecEsInfo[ucEsId]->u4PTS>prVdecEsInfo->u4PTSLast))
                {
                    prVdecEsInfo->u4PTSLast = _prVdecEsInfo[ucEsId]->u4PTS;
                    LOG(6, " calcl u4PTSLast=0x%x\n",prVdecEsInfo->u4PTSLast);
                }
                return B_REFERNCE_INVALID;
            }
        }
    }
    else
    {
        if (prFbmPicHdr->u4PTS == 0)
        {
            prFbmPicHdr->u4PTS = _prVdecEsInfo[ucEsId]->u4PTS;
        }
        if (prFbmPicHdr->u8Offset == 0)
        {
            prFbmPicHdr->u8Offset = _prVdecEsInfo[ucEsId]->u8Offset;
        }
        if (prFbmPicHdr->u8OffsetDisp == 0)
        {
            prFbmPicHdr->u8OffsetDisp = _prVdecEsInfo[ucEsId]->u8OffsetLast;
        }
        if (prFbmPicHdr->u8OffsetI == 0)
        {
            prFbmPicHdr->u8OffsetI = _prVdecEsInfo[ucEsId]->u8OffsetI;
        }
        if (prFbmPicHdr->u4TotlaTimeOffset == 0)
        {
            prFbmPicHdr->u4TotlaTimeOffset = _prVdecEsInfo[ucEsId]->u4TotlaTimeOffset;
        }
        if (prFbmPicHdr->u4TickNum == 0)
        {
            prFbmPicHdr->u4TickNum = _prVdecEsInfo[ucEsId]->u4TickNum;
        }
        if (prFbmPicHdr->u2DecodingOrder == 0)
        {
            prFbmPicHdr->u2DecodingOrder = _prVdecEsInfo[ucEsId]->u2DecodingOrder;
        }
    }

    prFbmPicHdr->i4TemporalRef = (INT32)rPicHdr.rField.u4TemporalRef;
    _prVdecEsInfo[ucEsId]->ucPicType = ucTemp;
    prFbmPicHdr->ucPicCdTp = ucTemp;

    if (ucTemp == B_TYPE)
    {
        MPEG2_B_PIC_HDR_UNION_T rBPicHdr;

        rBPicHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 29);

        prFbmPicHdr->ucFullPelFordVec = (UCHAR)rBPicHdr.rField.fgFullPelForward;
        prFbmPicHdr->ucFordFCode = (UCHAR)rBPicHdr.rField.u4ForwardFCode;
        prFbmPicHdr->ucFullPelBackVec = (UCHAR)rBPicHdr.rField.fgFullPelBackward;
        prFbmPicHdr->ucBackFCode = (UCHAR)rBPicHdr.rField.u4BackwardFCode;

        // get 8 bits off
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
    }
    else if (ucTemp == P_TYPE)
    {
        MPEG2_P_PIC_HDR_UNION_T rPPicHdr;

        rPPicHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 29);

        prFbmPicHdr->ucFullPelFordVec = (UCHAR)rPPicHdr.rField.fgFullPelForward;
        prFbmPicHdr->ucFordFCode = (UCHAR)rPPicHdr.rField.u4ForwardFCode;
        // clean
        prFbmPicHdr->ucFullPelBackVec = 0;
        prFbmPicHdr->ucBackFCode = 0;

        // get 4 bits off
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 4);
    }
    else
    {
        // clean
        prFbmPicHdr->ucFullPelFordVec = 0;
        prFbmPicHdr->ucFordFCode = 0;
        prFbmPicHdr->ucFullPelBackVec = 0;
        prFbmPicHdr->ucBackFCode = 0;

        // get 29 bits off
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 29);
    }

    // skip extra info
    while((u4Datain >> 31) == 1)
    {
        u4Datain = _GetBitStream(ucMpvId, ucEsId, 9);
    }

    // skip extra bit picture
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 1);

    //if (!_NextStartCode(ucMpvId, ucEsId))
	if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
    {
        return(NO_START_C_ERR10);
    }

     // parsing of picture extension
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
     if (u4Datain == EXTENSION_START_CODE)
     {
         MPEG2_PIC_EXT_HDR_UNION_T rPicExtHdr;

        rPicExtHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 32);
        rPicExtHdr.au4Reg[1] = _GetBitStream(ucMpvId, ucEsId, 32);

        prFbmPicHdr->ucfcode[0][0] = (UCHAR)rPicExtHdr.rField.u4FCode00;
        prFbmPicHdr->ucfcode[0][1] = (UCHAR)rPicExtHdr.rField.u4FCode01;
        prFbmPicHdr->ucfcode[1][0] = (UCHAR)rPicExtHdr.rField.u4FCode10;
        prFbmPicHdr->ucfcode[1][1] = (UCHAR)rPicExtHdr.rField.u4FCode11;

#if(F_CODE_00_ERR)
        if((prFbmPicHdr->ucfcode[0][0] == 0) || ((prFbmPicHdr->ucfcode[0][0] >= 10)
            && (prFbmPicHdr->ucfcode[0][0] < 15)))
        {
            return(F_CODE_00_ERR);
        }
#endif

#if(F_CODE_01_ERR)
        if((prFbmPicHdr->ucfcode[0][1] == 0) || ((prFbmPicHdr->ucfcode[0][1] >= 10)
            && (prFbmPicHdr->ucfcode[0][1] < 15)))
        {
            return(F_CODE_01_ERR);
        }
#endif

#if(F_CODE_10_ERR)
        if((prFbmPicHdr->ucfcode[1][0] == 0) || ((prFbmPicHdr->ucfcode[1][0] >= 10)
            && (prFbmPicHdr->ucfcode[1][0] < 15)))
        {
            return(F_CODE_10_ERR);
        }
#endif

#if(F_CODE_11_ERR)
        if ((prFbmPicHdr->ucfcode[1][1] == 0) || ((prFbmPicHdr->ucfcode[1][1] >= 10)
            && (prFbmPicHdr->ucfcode[1][1] < 15)))
        {
            return(F_CODE_11_ERR);
        }
#endif

        prFbmPicHdr->ucIntraDcPre = (UCHAR)rPicExtHdr.rField.u4IntraDcPrecision;
        prFbmPicHdr->ucPicStruct = (UCHAR)rPicExtHdr.rField.u4PicStruture;

        prMpvCounter->ucPicStruct = prFbmPicHdr->ucPicStruct;

        LOG(7, "Es%d, PicStruct = %d\n", ucEsId, prFbmPicHdr->ucPicStruct);

        if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_FULLB|DEC_FLAG_RACING_HALFB))
        {
            if (IS_FRMPIC(prFbmPicHdr))
            {
                if (_ucLastPicStruct != MPEG_PIC_FRAME)
                {
                    FBM_PicStructChgNotify(ucEsId, MPEG_PIC_FRAME);
                }
                _ucLastPicStruct = MPEG_PIC_FRAME;
            }
            else
            {
                if (_ucLastPicStruct != MPEG_PIC_FIELD)
                {
                    FBM_PicStructChgNotify(ucEsId, MPEG_PIC_FIELD);
                }
                _ucLastPicStruct = MPEG_PIC_FIELD;
            }

            if (!IS_FRMPIC(prFbmPicHdr))
            {
                MPV_EventNotify(MPV_FIELD, ucEsId, prFbmPicHdr->ucPicStruct,
                                            prFbmPicHdr->ucPicCdTp);
            }
        }

#if(PIC_STR_ERR)
        if(prFbmPicHdr->ucPicStruct == 0)
        {
            return(PIC_STR_ERR);
        }
#endif

        switch(prFbmPicHdr->ucPicStruct)
        {
            case TOP_FLD_PIC:
                if (!IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLG_2ND_FLD_PIC))
                {
                    prFbmPicHdr->fgTopFldFirst = TRUE;
                }
                else if (prFbmPicHdr->ucPicCdTp != B_TYPE)
                {
                    if ((prEsInfo->ucLastPicStruct != BTM_FLD_PIC) ||
                        (prEsInfo->ucLastPicTp == B_TYPE))
                    {
                        fgFieldErr = TRUE;
                    }
                }
                else
                {
                    if ((prEsInfo->ucLastPicStruct != BTM_FLD_PIC) ||
                        (prEsInfo->ucLastPicTp != B_TYPE))
                    {
                        fgFieldErr = TRUE;
                    }
                }
                break;

            case BTM_FLD_PIC:
                if (!IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLG_2ND_FLD_PIC))
                {
                    prFbmPicHdr->fgTopFldFirst = FALSE;
                }
                else if (prFbmPicHdr->ucPicCdTp != B_TYPE)
                {
                    if ((prEsInfo->ucLastPicStruct != TOP_FLD_PIC) ||
                        (prEsInfo->ucLastPicTp == B_TYPE))
                    {
                        fgFieldErr = TRUE;
                    }
                }
                else
                {
                    if ((prEsInfo->ucLastPicStruct != TOP_FLD_PIC) ||
                        (prEsInfo->ucLastPicTp != B_TYPE))
                    {
                        fgFieldErr = TRUE;
                    }
                }
                break;

            case FRM_PIC:
                if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLG_2ND_FLD_PIC))
                {
                    fgFieldErr = TRUE;
                }
                break;

            default:
                prFbmPicHdr->ucPicStruct = ERR_PIC_STRUCT;
                break;
        }

        if (fgFieldErr)
        {
            prEsInfo->ucLastPicStruct = ERR_PIC_STRUCT;
            return(FIELD_ERR);
        }

        prEsInfo->ucLastPicTp = prFbmPicHdr->ucPicCdTp;
        prEsInfo->ucLastPicStruct = prFbmPicHdr->ucPicStruct;

        if (IS_FRMPIC(prFbmPicHdr))
        {
            prFbmPicHdr->fgTopFldFirst = rPicExtHdr.rField.fgTff;
        }

        prFbmPicHdr->fgFrmPredFrmDct = rPicExtHdr.rField.fgFramePred;
        prFbmPicHdr->fgConcMotionVec = rPicExtHdr.rField.fgConcealmentMV;
        prFbmPicHdr->fgQScaleType = rPicExtHdr.rField.fgQScale;
        prFbmPicHdr->fgIntraVlcFmt = rPicExtHdr.rField.fgIntraVlc;
        prFbmPicHdr->fgAltScan = rPicExtHdr.rField.fgAlternateScan;
        prFbmPicHdr->fgRepFirstFld = rPicExtHdr.rField.fgRff;
        prFbmPicHdr->fgProgressiveFrm = rPicExtHdr.rField.fgPrgsFrame;

        if (rPicExtHdr.rField.fgCompositeDisp)
        {
            // get 22 bits off
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 22);
        }
        else
        {
            // get 2 bits off
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 2);
        }

        //if (!_NextStartCode(ucMpvId, ucEsId))
		if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
        {
            return(NO_START_C_ERR11);
        }
     }
     else
     {
        if (prEsInfo->ucMpegVer != MPEG1)
        {
            return (NO_EXT_START_CODE);
        }
         //ASSERT(prEsInfo->ucMpegVer == MPEG1);
     }

    #ifdef MPV_VIRTUAL_CC
    if (prEsInfo->fgVirtualCc)
    {
        MPV_CC_T *prMpvCc;

         prMpvCc = (MPV_CC_T*)_MPV_GetMpvCc(ucEsId);
        _VDEC_LockCcSema(ucEsId);

        if (prMpvCc->fgPlayCc)
        {
            _GetVirtualCc(ucEsId);
        }
        _VDEC_UnlockCcSema(ucEsId);
    }
    #endif

#ifdef CC_RESIZE_SMALL_IMAGE
    prFbmPicHdr->u4PicWidth = prVdecEsInfo->u2OrgHSize;
    prFbmPicHdr->u4PicWidthPitch = prVdecEsInfo->u2LineSize;
    prFbmPicHdr->u4PicHeight = prVdecEsInfo->u2OrgVSize;
#endif

    if (prEsInfo->ucMpegVer == MPEG2)
    {
        return _ExtUsrData(ucMpvId, ucEsId, 2);
    }
    else
    {
        return _ExtUsrData(ucMpvId, ucEsId, 1);
    }

}


static INT32 _VParser(UCHAR ucMpvId, UCHAR ucEsId)
{
    INT32 i4RetVal;
    UINT32 u4Retry = 0;
    UINT32 u4Datain;
    MPV_ES_INFO_T* prEsInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
//    VDEC_INFO_T* prVdecInfo;
    FBM_PIC_HDR_T *prFbmPicHdr;
    #ifdef MPV_REPEAT_TEST
    static UINT32 u4OnRepeatCount = 0;
    #endif
#ifdef MPV_STATISTICS
    HAL_TIME_T rStartGetFbmTime;
    HAL_TIME_T rStopGetFbmTime;
    HAL_TIME_T rDeltaTime;
#endif

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return POINTER_NOT_AVAILABLE;
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return POINTER_NOT_AVAILABLE;
    }

    prEsInfo = _prEsInfo[ucEsId];
    if(prEsInfo == NULL)
    {
        LOG(1, "_VParser prEsInfo null\n");
        return NONE_ERROR;
    }
    prVdecEsInfo = _prVdecEsInfo[ucEsId];
    if(prVdecEsInfo == NULL)
    {
        LOG(1, "_VParser prVdecEsInfo null\n");
        return NONE_ERROR;
    }
    _prVdecEsInfo[ucEsId]->fgSeqHdr = FALSE;
    prEsInfo->fgGopHdr = FALSE;
    prEsInfo->fgPicHdr = FALSE;

    if((!prVdecEsInfo) || (!prEsInfo))
    {
        LOG(1, "_VParser fail\n");
        return (POINTER_NOT_AVAILABLE);
    }

 //   prVdecInfo = _VDEC_GetInfo();

    // set ucBitCount=0, and get the bits before u4VldReadPtr
    _ResetBitStream(ucMpvId, ucEsId);

    // finding the first start code will be used
    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

    if ((u4Datain != SEQUENCE_START_CODE) &&
        (u4Datain != GROUP_START_CODE) &&
        (u4Datain != PICTURE_START_CODE))
    {
    #ifndef MPV_VIRTUAL_PRS
        UINT32 u4Val = 0;

        if ((_prVdecEsInfo[ucEsId]->u4VldReadPtr >= _prVdecEsInfo[ucEsId]->u4FifoStart) && (_prVdecEsInfo[ucEsId]->u4VldReadPtr <= _prVdecEsInfo[ucEsId]->u4FifoEnd))
        {
            UCHAR ucByte0;
            UCHAR ucByte1;
            UCHAR ucByte2;
            UCHAR ucByte3;

            ucByte0 = (*(UCHAR*)VIRTUAL(_prVdecEsInfo[ucEsId]->u4VldReadPtr));
            ucByte1 = (*(UCHAR*)VIRTUAL(_prVdecEsInfo[ucEsId]->u4VldReadPtr + 1));
            ucByte2 = (*(UCHAR*)VIRTUAL(_prVdecEsInfo[ucEsId]->u4VldReadPtr + 2));
            ucByte3 = (*(UCHAR*)VIRTUAL(_prVdecEsInfo[ucEsId]->u4VldReadPtr + 3));

            u4Val = (ucByte0 << 24) + (ucByte1 << 16) + (ucByte2 << 8) + ucByte3;
        }

        if ((u4Datain != u4Val) && (u4Val != 0))
        {
            LOG(1, "Rptr: 0x%x, u4Val: 0x%x, u4Datain: 0x%x\n",
                _prVdecEsInfo[ucEsId]->u4VldReadPtr, u4Val, u4Datain);

            //VERIFY(_NextStartCodeWithCheck(ucMpvId, ucEsId));

            if (!_NextStartCodeWithCheck(ucMpvId, ucEsId))
            {
                return (VLD_NO_START_CODE);
            }

            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
        }
        else
        {
            LOG(1, "Rptr: 0x%x, u4Val: 0x%x\n", _prVdecEsInfo[ucEsId]->u4VldReadPtr, u4Val);
            //ASSERT(0);
            return (NO_START_CODE);
        }
    #else
        {
            VERIFY(_NextStartCodeWithCheck(ucMpvId, ucEsId));
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
        }
    #endif
    }

    // handle sequence header
    if (u4Datain == SEQUENCE_START_CODE)
    {
        //LOG(10, "Sequence Header\n");
        LOG(5, "Seq(%d)=================================\n", prEsInfo->u4SeqCnt);
        prEsInfo->u4SeqCnt++;

        _prVdecEsInfo[ucEsId]->fgSeqHdr = TRUE;

        u4Retry = 0;
        do
        {
            _prVdecEsInfo[ucEsId]->fgSeqHdr = TRUE;
#ifdef GOPH_LOC_ERR
            if (u4Retry > 2)
            {
                _prVdecEsInfo[ucEsId]->fgSeqHdr = FALSE;
                return(SEQH_LOC_ERR);
            }
#endif
            // Reset Quantization Matrix in Sequence Header
            _MPV_VldResetMat(ucMpvId);

            i4RetVal = _SequenceHdr(ucMpvId, ucEsId);
            if(i4RetVal == SEQH_MKB_ERR) // skip this sequence header. continue decoding this picture.
            {
                _prVdecEsInfo[ucEsId]->fgSeqHdr = FALSE;
                break;
            }
            if (i4RetVal!=0)
            {
                _prVdecEsInfo[ucEsId]->fgSeqHdr = FALSE;
                return (i4RetVal);
            }
            u4Retry++;

            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
        }while (u4Datain == SEQUENCE_START_CODE);
    }

    // frame buffer group is NOT ready
    if (_prVdecEsInfo[ucEsId]->ucFbgId == FBM_FBG_ID_UNKNOWN)
    {
        return (FBG_ID_UNKNOWN);
    }

    // in multimedia playback, swdmx will send a sequence header in the begining
    // in such case, we just get sequence header and ignore this picture data.
    //if (prVdecEsInfo->fgMMPlayback && prVdecEsInfo->fgOnlyGotSeqHeader)
    // while in parsing sequence header, we can't make sure fgMMPlayback is ture or not.
    // take care of this flag "fgOnlyGotSeqHeader",
    // assign to true then false in same code segment.
    if (prVdecEsInfo->fgOnlyGotSeqHeader)
    {
        return (GET_MM_FIRST_SEQ_HEADER);
    }

    u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

    // handle GOP header
    if (u4Datain == GROUP_START_CODE)
    {
        prEsInfo->fgGopHdr = TRUE;

        LOG(10, "GOP Header\n");
        u4Retry = 0;
        do
        {
#ifdef PICH_LOC_ERR
            if (u4Retry > 2)
            {
                return(PICH_LOC_ERR);
            }
#endif
            i4RetVal = _GroupPicHdr(ucMpvId, ucEsId);

            if (i4RetVal != 0)
            {
                return(i4RetVal);
            }
            u4Retry++;

            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);
        }while (u4Datain == GROUP_START_CODE);
    }

    // handle picture header
    if (u4Datain == PICTURE_START_CODE)
    {
        prEsInfo->fgPicHdr = TRUE;

        i4RetVal = _PicHdr(ucMpvId, ucEsId);

        if (i4RetVal != 0)
        {
            if ((_prVdecEsInfo[ucEsId]->ucPicType != B_TYPE) || (i4RetVal == REFERNCE_MISSING))
            {
                SET_DECFLAG(_prVdecEsInfo[ucEsId], DEC_FLG_WAIT_NEXT_I);
            }
            return(i4RetVal);
        }
    }
#if (PICH_LOC_ERR)
    else
    {
        return (PICH_LOC_ERR);
    }
#endif

    if (_prVdecEsInfo[ucEsId]->ucPicType == B_TYPE)
    {
        if(prVdecEsInfo->fgMMPlayback)
        {
            // DTV skip picture in _MPV_EsFlwCtrl.
            // MM, we have to calculate PTS,
            // check this after picture header parsed (_VParser).
            if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SKIP_PIC))
            {
                if(!prVdecEsInfo->fgMMPlayback)
                {
                    // DTV skip picture here.
                    // MM, we have to calculate PTS,
                    // check this after picture header parsed.
                    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SKIP_PIC);

                    if (prVdecEsInfo->u2Count > prVdecEsInfo->u2UdfNs)
                    {
                        LOG(3, "Q S VLD VParser(%d) Es(%d) G(%d) Q(%d) RP(0x%08x)\n",
                            ucMpvId, prVdecEsInfo->u4EsId, prVdecEsInfo->ucFbgId, prVdecEsInfo->u2Count,
                            prVdecEsInfo->u4VldReadPtr);

                        return NO_DECODE;
                    }
                }
            }
        }
    }

    _prVdecEsInfo[ucEsId]->u4DecReadPtr = _MPV_SaveReadPtr(ucMpvId);

    MPV_UnlockVld(ucMpvId);

    _VDEC_LockFlushMutex(ucEsId);
    if (prVdecEsInfo->fgFlushEsmQ)
    {
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        _VDEC_UnlockFlushSema(ucEsId);
        _VDEC_UnlockFlushMutex(ucEsId);
        return (NO_DECODE);
    }
    _VDEC_UnlockFlushMutex(ucEsId);

    //Get Empty Frame buffer
    if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLG_2ND_FLD_PIC))
    {
        //ucFbId = FBM_GetDecFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId);
    }
    #ifdef MPV_REPEAT_TEST
    else if(prEsInfo->fgDecodeOnRepeat)
    {
        // do nothing
        LOG(1, "Decode On Repeat %d\n", u4OnRepeatCount++);
    }
    #endif
    else
    {
        #ifdef MPV_STATISTICS
        HAL_GetTime(&rStartGetFbmTime);
        #endif

        if (FBM_ChkFrameBufferFlag(_prVdecEsInfo[ucEsId]->ucFbgId , FBM_FLAG_FREEZE))
        {
            UCHAR ucFbId;

            ucFbId = FBM_GetFrameBufferFromDispQ(_prVdecEsInfo[ucEsId]->ucFbgId);
            LOG_MFBM("MFBM Set Empty j, %d\n", ucFbId);
            FBM_SetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
        }

        if ((_prVdecEsInfo[ucEsId]->ucPicType == I_TYPE) || (_prVdecEsInfo[ucEsId]->ucPicType == P_TYPE))
        {
        #ifdef CC_MPV_DEBLOCKING
            if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK))
            {
                if (prVdecEsInfo->fgRefNoDbk)
                {
                    LOG_MFBM("MFBM Set R no dbk Empty a, %d, %d\n", prVdecEsInfo->ucDbkRefId,
                    	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucDbkRefId))->i4TemporalRef);
                    FBM_SetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId,
                                         prVdecEsInfo->ucDbkRefId, FBM_FB_STATUS_EMPTY);
                    prVdecEsInfo->ucDbkRefId = FBM_FB_ID_UNKNOWN;
                    LOG_MFBM("MFBM ucDbkRefId 0xFF f\n");
                }

                _prVdecEsInfo[ucEsId]->ucFbId = FBM_GetEmptyRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId,
                                               _prVdecEsInfo[ucEsId]->u4WaitDispTime);
                LOG_MFBM("MFBM Get R Empty a, %d\n", _prVdecEsInfo[ucEsId]->ucFbId);

                _prVdecEsInfo[ucEsId]->ucDbkFbId = FBM_GetDbkEmptyRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId,
                                                _prVdecEsInfo[ucEsId]->u4WaitDispTime);
                LOG_MFBM("MFBM Get R dbk Empty a, %d\n", _prVdecEsInfo[ucEsId]->ucDbkFbId);

                prVdecEsInfo->ucDbkRefId = _prVdecEsInfo[ucEsId]->ucDbkFbId;
                LOG_MFBM("MFBM ucDbkRefId %d\n", prVdecEsInfo->ucDbkRefId);

                if (!prVdecEsInfo->fgRefNoDbk)
                {
                    UCHAR ucBRefFbId;
                    UCHAR ucFRefFbId;

                    FBM_GetRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId, &ucFRefFbId, &ucBRefFbId);
                    if ((FBM_GetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId, ucFRefFbId) != FBM_FB_STATUS_DISPLAYQ) &&
                        (FBM_GetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId, ucFRefFbId) != FBM_FB_STATUS_LOCK))
                    {
                        LOG_MFBM("MFBM Set R Empty a, %d, %d\n", ucFRefFbId,
                        	(FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFRefFbId))->i4TemporalRef);
                      FBM_SetFrameBufferStatus(_prVdecEsInfo[ucEsId]->ucFbgId,
                                         ucFRefFbId, FBM_FB_STATUS_EMPTY);
                    }
                }

            }
            else
        #endif
            {
                _prVdecEsInfo[ucEsId]->ucFbId = FBM_GetEmptyRefFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId,
                                           _prVdecEsInfo[ucEsId]->u4WaitDispTime);
                LOG_MFBM("MFBM Get R Empty b, %d\n", _prVdecEsInfo[ucEsId]->ucFbId);
            }

        }
        else if (_prVdecEsInfo[ucEsId]->ucPicType == B_TYPE)
        {
        #ifdef CC_MPV_DEBLOCKING
            if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK))
            {

                #if 1//ilii
                _prVdecEsInfo[ucEsId]->ucFbId = FBM_GetDbkSmallBFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId);
                #endif

                _prVdecEsInfo[ucEsId]->ucDbkFbId = FBM_GetEmptyBFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId,
                                                _prVdecEsInfo[ucEsId]->u4WaitDispTime);
                LOG_MFBM("MFBM Get B dbk Empty a, %d\n", _prVdecEsInfo[ucEsId]->ucDbkFbId);
                #if defined(CC_MT5363) && !defined(CC_MT5365) && !defined(CC_MT5365)
                //#ifdef CC_MT5363
                _prVdecEsInfo[ucEsId]->ucFbId = _prVdecEsInfo[ucEsId]->ucDbkFbId;
                #endif
            }
            else
        #endif
            {
	            if ((prEsInfo->ucBNs >= 2) && IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_RACING_HALFB))
            	{
            		INT32 i4Ret;
            		i4Ret =FBM_WaitFbChangeField(_prVdecEsInfo[ucEsId]->ucFbgId, MPV_WAIT_CHGFLD_TIME);

            		if (i4Ret != OSR_OK)
            		{
            		#ifdef VDP_RACE_LOG
            		    VDP_FrcRacingLog('W');
            		#endif
            		    return FB_ID_UNKNOWN;
            		}
            	}

                _prVdecEsInfo[ucEsId]->ucFbId = FBM_GetEmptyBFrameBuffer(_prVdecEsInfo[ucEsId]->ucFbgId,
                                            _prVdecEsInfo[ucEsId]->u4WaitDispTime);
                LOG_MFBM("MFBM Get B Empty a, %d\n", _prVdecEsInfo[ucEsId]->ucFbId);
            }
        }
#ifdef MPV_STATISTICS
        HAL_GetTime(&rStopGetFbmTime);
        HAL_GetDeltaTime(&rDeltaTime, &rStartGetFbmTime, &rStopGetFbmTime);
        LOG(6, "GetFbm, Es(%d) Pic(%d) Time (%d, %d)\n", ucEsId, _prVdecEsInfo[ucEsId]->ucPicType, rDeltaTime.u4Seconds, rDeltaTime.u4Micros);
#endif
    }

    _VDEC_LockFlushMutex(ucEsId);

    if (prVdecEsInfo->fgFlushEsmQ)
    {
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        _VDEC_UnlockFlushSema(ucEsId);
        _VDEC_UnlockFlushMutex(ucEsId);
        return (NO_DECODE);
    }
	_VDEC_UnlockFlushMutex(ucEsId);

    if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK))
    {
        LOG(6, "(%d) FbId = %d, %d\n", _prVdecEsInfo[ucEsId]->ucPicType, _prVdecEsInfo[ucEsId]->ucFbId, _prVdecEsInfo[ucEsId]->ucDbkFbId);
    }
    else
    {
        LOG(6, "FbId = %d\n", _prVdecEsInfo[ucEsId]->ucFbId);
    }

    if (_prVdecEsInfo[ucEsId]->ucFbId == FBM_FB_ID_UNKNOWN)
    {
        //ASSERT(0);
        return (FB_ID_UNKNOWN);
    }

    if (prEsInfo->ucMpegVer == MPEG1)
    {
        prEsInfo->rFbmPicHdr.ucPicStruct = FRM_PIC;
        prEsInfo->rFbmPicHdr.fgProgressiveFrm = TRUE;
        prEsInfo->rFbmPicHdr.fgRepFirstFld = FALSE;
        // clean
        prEsInfo->rFbmPicHdr.fgTopFldFirst = TRUE;

        prEsInfo->rFbmPicHdr.ucIntraDcPre = 0;
        prEsInfo->rFbmPicHdr.fgAltScan = FALSE;
        prEsInfo->rFbmPicHdr.fgIntraVlcFmt = FALSE;
        prEsInfo->rFbmPicHdr.fgTopFldFirst = FALSE;
        prEsInfo->rFbmPicHdr.fgQScaleType = FALSE;
        prEsInfo->rFbmPicHdr.fgConcMotionVec = FALSE;
        prEsInfo->rFbmPicHdr.fgFrmPredFrmDct = FALSE;
    }

    //ilii
    //if ((_prVdecEsInfo[ucEsId]->ucPicType == B_TYPE) && (!prEsInfo->fgBNoDbk))
    //{
        // _prVdecEsInfo[ucEsId]->ucFbId is a invalid value.
    //}
    //else
    {
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(_prVdecEsInfo[ucEsId]->ucFbgId, _prVdecEsInfo[ucEsId]->ucFbId);
        if(prFbmPicHdr == NULL)
        {
            LOG(1, "_VParser prFbmPicHdr null\n");
            ASSERT(prFbmPicHdr != NULL);
            return NONE_ERROR;
        }
        x_memcpy(prFbmPicHdr, &(prEsInfo->rFbmPicHdr), sizeof(FBM_PIC_HDR_T));
    }


#ifdef CC_MPV_DEBLOCKING
    if (IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK))
    {
        if (_prVdecEsInfo[ucEsId]->ucDbkFbId == FBM_FB_ID_UNKNOWN)
        {
            //ASSERT(0);
            return (FB_ID_UNKNOWN);
        }

        prFbmPicHdr = FBM_GetFrameBufferPicHdr(_prVdecEsInfo[ucEsId]->ucFbgId, _prVdecEsInfo[ucEsId]->ucDbkFbId);
        if(prFbmPicHdr == NULL)
        {
            LOG(1, "_VParser prFbmPicHdr null\n");
            ASSERT(prFbmPicHdr != NULL);
            return NONE_ERROR;
        }
        x_memcpy(prFbmPicHdr, &(prEsInfo->rFbmPicHdr), sizeof(FBM_PIC_HDR_T));
    }
#endif

    // In 2x, we have to calculate all picture PTS
    // we have to assign prFbmPicHdr with valid data.
    // then MPV_calc(...) will calculate the correct PTS.
    if(_prVdecEsInfo[ucEsId]->fgMMPlayback)
    {
        if (prEsInfo->rFbmPicHdr.ucPicCdTp == P_TYPE)
        {
            if ((prVdecEsInfo->u4MMSpeed != (UINT32)STC_SPEED_TYPE_FORWARD_1X) &&
                ((prVdecEsInfo->u4MMSpeed != (UINT32)STC_SPEED_TYPE_FORWARD_2X)
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
                 ||
                 ((prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_2X)
                  &&
                  (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
#endif
                )
                &&
                (!(((prVdecEsInfo->u4MMSpeed >= STC_SPEED_TYPE_FORWARD_1_DIV_2X) &&
                   ((prVdecEsInfo->u4MMSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X))))))
            {
                // in order to support P frame only stream.
                // add this condition
                if((prVdecEsInfo->fgCanDisp) && (prVdecEsInfo->u4DisplayQPicCnt > 0))
                {
                    LOG(6, "Speed(%d), skip P\n", prVdecEsInfo->u4MMSpeed);
                    return (NO_DECODE);
                }
            }
        }
        else if (prEsInfo->rFbmPicHdr.ucPicCdTp == B_TYPE)
        {
            if ((prVdecEsInfo->u4MMSpeed != (UINT32)STC_SPEED_TYPE_FORWARD_1X) &&
                (!(((prVdecEsInfo->u4MMSpeed >= STC_SPEED_TYPE_FORWARD_1_DIV_2X) &&
                   ((prVdecEsInfo->u4MMSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X))))))
            {
                LOG(6, "Speed(%d), skip B\n", prVdecEsInfo->u4MMSpeed);
                return (NO_DECODE);
            }
        }
    }

    MPV_LockVld(ucMpvId);

    if (!IS_DECFLAG_SET(_prVdecEsInfo[ucEsId], DEC_FLAG_DBK))
    {
        _MPV_VldHWReset(ucMpvId);
        _MPV_VldSetVFifo(ucMpvId, _prVdecEsInfo[ucEsId]->u4FifoStart, _prVdecEsInfo[ucEsId]->u4FifoEnd);

    #ifndef MPV_VIRTUAL_PRS
        _MPV_VldRisCtrlWptr(ucMpvId);
        _MPV_VldSetWptr(ucMpvId, prEsInfo->u4VldWptr);
        LOG(9, "_VParser _MPV_VldSetWptr 0x%x\n", prEsInfo->u4VldWptr);
    #endif

        VERIFY(_MPV_VldInitBarrelShifter(ucMpvId, _prVdecEsInfo[ucEsId]->u4DecReadPtr));

        if (!_NextSliceHdr(ucMpvId, ucEsId))
        {
            i4RetVal = NO_START_C_ERR16;
        }

        if (_prVdecEsInfo[ucEsId]->fgSeqHdr)
        {
            if (prEsInfo->u4QpInterval >= MPV_QP_INTERVAL)
            {
                prFbmPicHdr->u4QpAvg = _DetectSliceQp(ucMpvId, ucEsId);
                prEsInfo->u4LastQp = prFbmPicHdr->u4QpAvg;
                prEsInfo->u4QpInterval = 0;
                LOG(7, "QP=%d\n", prFbmPicHdr->u4QpAvg);
            }
        }
        else
        {
            prFbmPicHdr->u4QpAvg = prEsInfo->u4LastQp;
        }

        prEsInfo->u4QpInterval++;

    }

    _MPV_VldHWReset(ucMpvId);
    _MPV_VldSetVFifo(ucMpvId, _prVdecEsInfo[ucEsId]->u4FifoStart, _prVdecEsInfo[ucEsId]->u4FifoEnd);

#ifndef MPV_VIRTUAL_PRS
    _MPV_VldRisCtrlWptr(ucMpvId);
    _MPV_VldSetWptr(ucMpvId, prEsInfo->u4VldWptr);
    LOG(9, "_VParser _MPV_VldSetWptr 0x%x\n", prEsInfo->u4VldWptr);
#endif

    if (!_MPV_VldInitBarrelShifter(ucMpvId, _prVdecEsInfo[ucEsId]->u4DecReadPtr))
    {
        return(BAR_SHIFTER_ERR);
    }

    if ((prEsInfo->fgLoadIntraMatrix) ||(prEsInfo->fgLoadNonIntraMatrix))
    {
        if (_au4CurrentQMatrixId[ucMpvId] != prEsInfo->u4MatrixId)
        {
            if (prEsInfo->fgLoadIntraMatrix)
            {
                _MPV_VldLoadQMat(ucMpvId, _prEsInfo[ucEsId]->pu4IntraMatrix, TRUE);
                //_MPV_VldReadQMat(ucMpvId, TRUE);
            }

            if (prEsInfo->fgLoadNonIntraMatrix)
            {
                _MPV_VldLoadQMat(ucMpvId, _prEsInfo[ucEsId]->pu4NonIntraMatrix, FALSE);
                //_MPV_VldReadQMat(ucMpvId,FALSE);
            }

            _au4CurrentQMatrixId[ucMpvId] = prEsInfo->u4MatrixId;
        }

        if (prEsInfo->fgLoadIntraMatrix)
        {
            _MPV_VldRldQMat(ucMpvId, TRUE);
        }

        if (prEsInfo->fgLoadNonIntraMatrix)
        {
            _MPV_VldRldQMat(ucMpvId, FALSE);
        }
    }

    //if (!_NextSliceHdr(ucMpvId, ucEsId))
    if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)SLICE_START_CODE))
    {
        i4RetVal = NO_START_C_ERR16;
    }
// ilii both, 5381 have, FPGA 5391 don't have
    {
        UINT16 u2VSizeMB;
        UINT32 u4SliceIdx;
        FBM_SEQ_HDR_T *prFbmSeqHdr;

        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prVdecEsInfo[ucEsId]->ucFbgId);
        if(prFbmSeqHdr == NULL)
        {
            LOG(3, "_VParser prFbmSeqHdr null\n");
            return SLICE_IDX_ERROR;
        }

        u2VSizeMB = (UINT16)((prVdecEsInfo->u2OrgVSize + 15) >> 4);
        u4SliceIdx = (_MPV_VldRead32(ucMpvId, RO_VLD_BARL) & 0xFF);
        if (u4SliceIdx > u2VSizeMB)
        {
            i4RetVal = SLICE_IDX_ERROR;
        }
    }

#if 0
    if ((prEsInfo->fgDetectIntra) && (!prEsInfo->fgCanDisp))
    {
        if (!prEsInfo->fgAllIntraSlice)
        {
            if (prFbmPicHdr->ucPicCdTp == P_TYPE)
            {
                _SearchIntraSlice(ucMpvId, ucEsId);

                VERIFY(_MPV_VldInitBarrelShifter(ucMpvId, prEsInfo->u4DecReadPtr));
                if (!_NextSliceHdr(ucMpvId, ucEsId))
                {
                    i4RetVal = NO_START_C_ERR16;
                }
            }
        }
    }
#endif

    return (i4RetVal);
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _MPV_VParserInit(UCHAR ucMpvId)
{
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return;
    }

    _au4CurrentQMatrixId[ucMpvId] = ~0;
    _au4QMatrixCounter[ucMpvId] = 0;

    if (_hVldTimer[ucMpvId] == (HANDLE_T)NULL)
    {
        VERIFY(x_timer_create(&_hVldTimer[ucMpvId]) == OSR_OK);
    }
}


void _MPV_VparserReloadEs(UCHAR ucEsId)
{
    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return;
    }

    // _prEsInfo is local variable point to global one
    // ilii, change later
    //_prEsInfo[ucEsId] = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    _prEsInfo[ucEsId] = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    _prVdecEsInfo[ucEsId] = _VDEC_GetEsInfo(ucEsId);
    _prVdecInfo = _VDEC_GetInfo();
}


INT32 _MPV_VParser(UCHAR ucMpvId, UCHAR ucEsId)
{
    //INT32 i4ErrCode;
    INT32 i4Status;
    UINT32 u4RdPtr;
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    //MPV_ES_INFO_T *prEsInfo;
    MPV_CC_T *prMpvCc;
    PARAM_MPV_T* prMpvCounter;

#ifdef MPV_STATISTICS
    HAL_GetTime(&_rSwStartDecodeTime[ucMpvId]);
#endif

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return VDEC_ERR;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return VDEC_ERR;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecInfo = _VDEC_GetInfo();
    //prEsInfo = (MPV_ES_INFO_T*)_MpvGetHdlr(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    u4RdPtr = _prVdecEsInfo[ucEsId]->u4VldReadPtr;

    if (!_MPV_VldInitBarrelShifter(ucMpvId, u4RdPtr))
    {
        return(BAR_SHIFTER_ERR);
    }

    i4Status = _VParser(ucMpvId, ucEsId);

    if(i4Status != NONE_ERROR)
    {
        if (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME)
        {
            if (i4Status != NO_DECODE)
            {
                if (prVdecEsInfoKeep->pfDecNfyFct)
                {
                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_ERROR, 0, 0);
                }
            }
        }

        prMpvCc = _MPV_GetMpvCc(ucEsId);

        if (prMpvCc->fgPlayCc)
        {
            if (i4Status & NEED_EXTRACT_CC)
            {
                _MPV_UsrData(ucMpvId, ucEsId);
            }
        }

        LOG(3, "E (%d, %x)\n", ucEsId, i4Status);
        prMpvCounter->u4VpsrDropNs++;

        prVdecEsInfo->u4DecReadPtr = prVdecEsInfo->u4VldReadPtr;

        return i4Status;                 //mpv decoder will handle this return value
    }

    return (VDEC_SUCCESS);
}


UINT32 _MPV_VParserGetHdr(UCHAR ucMpvId, UCHAR ucEsId, UINT32* pu4Addr, UCHAR *pucPicType)
{
    UINT32 u4Datain;
    UINT32 u4Datain1;
    UCHAR ucTemp;


    if((pucPicType == NULL) || (pu4Addr == NULL))
    {
        ASSERT(pu4Addr != NULL);
        ASSERT(pucPicType != NULL);
        return 0;
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return 0;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return 0;
    }

    do
    {
        if (_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
        {
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

            if ((u4Datain != SEQUENCE_START_CODE) &&
                (u4Datain != GROUP_START_CODE) &&
                (u4Datain !=PICTURE_START_CODE))
            {
                u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
            }
        }
        else
        {
        #ifdef MPV_VIRTUAL_PRS
            ASSERT(0);
        #endif
            return 0;
        }

    }while ((u4Datain != SEQUENCE_START_CODE) &&
               (u4Datain != GROUP_START_CODE) &&
               (u4Datain != PICTURE_START_CODE));

    if (u4Datain == PICTURE_START_CODE)
    {
        MPEG2_PIC_HDR_UNION_T rPicHdr;

        rPicHdr.au4Reg[0] = _GetBitStream(ucMpvId, ucEsId, 32);

        ucTemp = (UCHAR)rPicHdr.rField.u4PicType;
    #if(PIC_COD_TYP_ERR)
        if((ucTemp == 0) || (ucTemp >= 5))
        {
            return (PIC_COD_TYP_ERR);
        }
    #endif

        *pucPicType = ucTemp;
    }
    else if ((u4Datain == SEQUENCE_START_CODE) || (u4Datain == GROUP_START_CODE))
    {
        ASSERT(pucPicType != NULL);
        *pucPicType = I_TYPE;
    }

    *pu4Addr = _MPV_SaveReadPtr(ucMpvId);
    LOG(6, "pu4Addr 0x%x\n", *pu4Addr);
    u4Datain1 = _GetBitStream(ucMpvId, ucEsId, 32);
    UNUSED(u4Datain1);

    return u4Datain;
}


UINT32 _MPV_SearchStartCode(UCHAR ucMpvId, UCHAR ucEsId, UINT32* pu4Addr)
{
    UINT32 u4Datain;

    if(pu4Addr == NULL)
    {
        return 0;
    }

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return 0;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return 0;
    }

    do
    {
        if (_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)ALL_START_CODE))
        {
            u4Datain = _GetBitStream(ucMpvId, ucEsId, 0);

            if ((u4Datain != SEQUENCE_START_CODE) &&
                (u4Datain != GROUP_START_CODE) &&
                (u4Datain !=PICTURE_START_CODE))
            {
                u4Datain = _GetBitStream(ucMpvId, ucEsId, 8);
            }
        }
        else
        {
        #ifdef MPV_VIRTUAL_PRS
            ASSERT(0);
        #endif
            return 0;
        }

    }while ((u4Datain != SEQUENCE_START_CODE) &&
               (u4Datain != GROUP_START_CODE) &&
               (u4Datain != PICTURE_START_CODE));

    *pu4Addr = _MPV_SaveReadPtr(ucMpvId);
    UNUSED(_GetBitStream(ucMpvId, ucEsId, 32));

    return u4Datain;
}


INT32 _MPV_ExtractCc(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4RdPtr;
    INT32 i4RetVal;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return 0;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return 0;
    }

    u4RdPtr = _prVdecEsInfo[ucEsId]->u4VldReadPtr;

    _prEsInfo[ucEsId]->u4AtscCcUsrDataCnt = 0;
    _prEsInfo[ucEsId]->u4ScteCcUsrDataCnt = 0;

    _MPV_DecReset(ucMpvId, ucEsId);

    if (!_MPV_VldInitBarrelShifter(ucMpvId, u4RdPtr))
    {
        return(BAR_SHIFTER_ERR);
    }

    //if (!_SearchPicHdr(ucMpvId, ucEsId))
    if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)PIC_START_CODE))
    {
        return NO_START_C_ERR16;
    }

    //if (!_SearchUserHdr(ucMpvId, ucEsId))
    if (!_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)USRDATA_START_CODE))
    {
        return NO_START_C_ERR14;
    }

    i4RetVal = _UsrData(ucMpvId, ucEsId);

    if (i4RetVal != 0)
    {
        return(i4RetVal);
    }

    return (NONE_ERROR);

}


void _MPV_UsrData(UCHAR ucMpvId, UCHAR ucEsId)
{
    INT32 i4Ret;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return;
    }

    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return;
    }

    //if (_SearchUserHdr(ucMpvId, ucEsId))
    if (_HwNextStartCodeInt(ucMpvId, ucEsId, (UINT32)USRDATA_START_CODE))
    {
        i4Ret = _UsrData(ucMpvId, ucEsId);
        LOG(6, "Usrdata (%d, %d)\n", ucEsId, i4Ret);
        UNUSED(i4Ret);
    }
}


void _MPV_ResetDectectIntra(UCHAR ucEsId)
{
    VDEC_ES_INFO_T* prVdecEsInfo;
    MPV_ES_INFO_T* prEsInfo;

    if(ucEsId >= MPV_MAX_ES)
    {
        LOG(1, "ucEsId >= MPV_MAX_ES\n");
        ASSERT(ucEsId < MPV_MAX_ES);
        return;
    }

    prVdecEsInfo = _prVdecEsInfo[ucEsId];
    prEsInfo = _prEsInfo[ucEsId];

    prVdecEsInfo->fgAllIntraSlice = FALSE;
    prVdecEsInfo->fgCanDisp = FALSE;
    prVdecEsInfo->fgBisOk = FALSE;
    prVdecEsInfo->u4WaitP = 0;
    prEsInfo->u4IntraSliceReg0= 0;
    prEsInfo->u4IntraSliceReg1 = 0;
    prEsInfo->u4IntraSliceReg2 = 0;
   // x_memset((void*)_prEsInfo[ucEsId]->afgIntraSlice, 0, 0xAF);

    LOG(5, "_MPV_ResetDectectIntra\n");
}


BOOL _MPV_NextSliceHdr(UCHAR ucMpvId, UCHAR ucEsId)
{
    UINT32 u4NextStart;
    UINT32 ucBitCount;
    UCHAR ucRmvBit =0;

    //Let stream byte aligned
    ucBitCount = _prEsInfo[ucEsId]->ucBitCount ;
    ucRmvBit = (UCHAR)(8 - (ucBitCount % 8));
    if (ucRmvBit == 8)
    {
        ucRmvBit = 0;
    }

    u4NextStart = _GetBitStream(ucMpvId, ucEsId, ucRmvBit);

    _ucTimeout[ucMpvId] = 0;
    VERIFY(x_timer_start(_hVldTimer[ucMpvId], 100, X_TIMER_FLAG_ONCE,
        _NextStartCodeTimeout, (void*)(UINT32)ucMpvId) == OSR_OK);

    while (_ucTimeout[ucMpvId] == 0)
    {
        if ((u4NextStart >= SLICE_START_CODE_MIN) &&
            (u4NextStart <= SLICE_START_CODE_MAX))
        {
            VERIFY(x_timer_stop(_hVldTimer[ucMpvId]) == OSR_OK);
            return TRUE;
        }
        u4NextStart = _GetBitStream(ucMpvId, ucEsId, 8);
    }

    // timeout
    return FALSE;
}


void _MPV_CreateSearchSema(UCHAR ucMpvId)
{
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return;
    }

#ifndef LOCK_TIMEOUT
    VERIFY(x_msg_q_create(&_hHWSearchMsgQ[ucMpvId], "Search-Q", sizeof(UINT32), 2) == OSR_OK);
    VERIFY(x_timer_create(&_hHWSearchTimer[ucMpvId]) == OSR_OK);
#else
    VERIFY (x_sema_create(&_ahSearchFinSema[ucMpvId], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif
}


void _MPV_DeleteSearchSema(UCHAR ucMpvId)
{
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return;
    }

#ifndef LOCK_TIMEOUT
    VERIFY(x_msg_q_delete(_hHWSearchMsgQ[ucMpvId]) == OSR_OK);
    VERIFY(x_timer_delete(_hHWSearchTimer[ucMpvId]) == OSR_OK);
#else
    VERIFY (x_sema_delete(_ahSearchFinSema[ucMpvId]) == OSR_OK);
#endif
}


void _MPV_FinSearch(UCHAR ucMpvId)
{
#ifndef LOCK_TIMEOUT
    UINT32 u4DecFin = 0;
#endif
    if(ucMpvId >= MPV_MAX_VLD)
    {
        LOG(1, "ucMpvId >= MPV_MAX_VLD\n");
        ASSERT(ucMpvId < MPV_MAX_VLD);
        return;
    }

#ifndef LOCK_TIMEOUT
    u4DecFin = SEARCH_FINISH;
    VERIFY(x_msg_q_send(_hHWSearchMsgQ[ucMpvId], (void *)&u4DecFin, sizeof(UINT32), 1) == OSR_OK);
#else
    VERIFY (x_sema_unlock(_ahSearchFinSema[ucMpvId]) == OSR_OK);
#endif
}


void _MPV_ResetUserDataIP(void)
{
   x_memset(&_rUserDataIPAtsc, 0, sizeof(MPV_USERDATA_CB_T));
   x_memset(&_rUserDataIPScte, 0, sizeof(MPV_USERDATA_CB_T));
   x_memset(&_rUserDataIPAfd, 0, sizeof(MPV_USERDATA_CB_T));
   LOG(7, "_MPV_ResetUserDataIP\n");
}


