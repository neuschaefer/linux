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


#ifndef SWDMX_MPG_DMX_H
#define SWDMX_MPG_DMX_H

#include "swdmx_drvif.h"
// move to swdmx_drvif.h
#if 0
typedef void (*PFN_SWDMX_CALLBACK)(INT32 i4Tag);
#endif 

//
// Demux
//

/// Demux filter sturcture
typedef struct
{
    UINT8                   u1Offset;           ///< Byte offset, 1 - 31
    UINT8                   u1PatternLength;    ///< Length of pattern
    BOOL                    fgCheckCrc;         ///< If check CRC
    UINT16                  u2Pid;              ///< PID value
    UINT32                  u4BufferSize;       ///< Filter buffer size
    UINT8*                  pu1Pattern;         ///< The pattern array
    UINT8*                  pu1PositiveMask;    ///< Positive mask array
    UINT8*                  pu1NegativeMask;    ///< Negative mask array
    PFN_SWDMX_CALLBACK		pfnHandler;         ///< Section callback handler
    void*                   pvNotifyTag;        ///< Tag value to handler
} SWDMX_DMX_FILTER_T;


// move these structs to swdmx_drvif.h
#if 0
/// Demux frame data (section or PES) structure
typedef struct
{
    UINT8                   u1SerialNumber;     ///< Serial number
    UINT32                  u4FrameAddr;        ///< Frame address
    UINT32                  u4Size;             ///< Frame size
} SWDMX_DMX_DATA_T;

/// Demux message type
typedef enum
{
    DMX_MSG_TYPE_PSI,                           ///< PSI message
    DMX_MSG_TYPE_PES                            ///< PES message
} DMX_MSG_TYPE_T;

/// Demux message structure
typedef struct
{
    DMX_MSG_TYPE_T      eType;                  ///< Message type
    UINT8               u1Id;                   ///< Filter id or PID id
    SWDMX_DMX_DATA_T      rData;                  ///< Type specific data
    const void*               pvNotifyTag;            ///< User tag value
} SWDMX_DMX_MSG_T;
#endif 

/// Demux PES structure
typedef struct
{
    UINT8                   u1TsIndex;          ///< Index Id
    UINT8                   u1KeyIndex;         ///< Key Index Id
    UINT16                  u2Pid;              ///< PID value
    UINT16                  u2DeviceId;         ///< Device Id
    UINT16                  u2ChannelId;         ///< Channel Id
    UINT32                  u4BufferSize;       ///< PES buffer size
    UINT32                  u4DescMode;         ///< PES Desc mode
    PFN_SWDMX_CALLBACK      pfnHandler;         ///< PES callback handler
    void*                   pvNotifyTag;        ///< User tag value
    VOID*                   pfnScramble;        ///< PES scramble notify
} SWDMX_DMX_PES_T;

typedef enum
{
    SWDMX_E_OK = 0,
    SWDMX_E_FAIL,
    SWDMX_E_BUFFER_OVERFLOW,
    SWDMX_E_BUFFER_UNDERFLOW,
    SWDMX_E_OUT_OF_MEMORY,
    SWDMX_E_INV_ARG,
    SWDMX_E_NOT_EXIST
} SWDMX_ERR_CODE_T;

//
// Demux
//

extern VOID SWDMX_DmxInit(UINT8 u1SrcId);

extern SWDMX_ERR_CODE_T SWDMX_DmxPsiAllocateFilter(UINT8 u1SrcId, SWDMX_DMX_FILTER_T* prFilter,
    UINT8* pu1FilterId);

extern SWDMX_ERR_CODE_T SWDMX_DmxPsiReleaseFilter(UINT8 u1SrcId, UINT8 u1FilterId);

extern SWDMX_ERR_CODE_T SWDMX_DmxPsiStartFilter(UINT8 u1SrcId, UINT8 u1FilterId);

extern SWDMX_ERR_CODE_T SWDMX_DmxPsiStopFilter(UINT8 u1SrcId, UINT8 u1FilterId);

extern SWDMX_ERR_CODE_T SWDMX_DmxPsiGetMessage(UINT8 u1SrcId, UINT8 u1MsgId,
    SWDMX_DMX_MSG_T* prMsg);

extern SWDMX_ERR_CODE_T SWDMX_DmxPsiCopyData(UINT8 u1SrcId, UINT8 u1FilterId,
    SWDMX_DMX_DATA_T* prData, UINT8* pu1Buf);

extern SWDMX_ERR_CODE_T SWDMX_DmxPsiUnlockData(UINT8 u1SrcId, UINT8 u1FilterId,
    SWDMX_DMX_DATA_T* prData);



extern SWDMX_ERR_CODE_T SWDMX_DmxPesAllocateBuffer(UINT8 u1SrcId, SWDMX_DMX_PES_T* prPes,
    UINT8* pu1Pidx, DMX_PID_TYPE_T ePidType);

extern SWDMX_ERR_CODE_T SWDMX_DmxPesReleaseBuffer(UINT8 u1SrcId, UINT8 u1Pidx);

extern SWDMX_ERR_CODE_T SWDMX_DmxPesStart(UINT8 u1Pidx);

extern SWDMX_ERR_CODE_T SWDMX_DmxPesStop(UINT8 u1Pidx);

extern SWDMX_ERR_CODE_T SWDMX_DmxPesGetMessage(UINT8 u1SrcId, UINT32 u4MsgId,
    SWDMX_DMX_MSG_T* prMsg);

extern SWDMX_ERR_CODE_T SWDMX_DmxPesCopyData(UINT8 u1Pidx,
    SWDMX_DMX_DATA_T* prData, UINT8* pu1Buf);

extern SWDMX_ERR_CODE_T SWDMX_DmxPesUnlockData(UINT8 u1Pidx,
    SWDMX_DMX_DATA_T* prData);

#endif
