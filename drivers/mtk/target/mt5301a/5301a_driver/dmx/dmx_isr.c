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
 * $RCSfile: dmx_isr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_isr.c
 *  Demux driver - ISR
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_ide.h"
#include "dmx_pcr.h"
#include "dmx_debug.h"
#include "dmx_if.h"
#include "stc_drvif.h"
#include "x_bim.h"
#include "x_hal_arm.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_timer.h"

#include "fvr.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// DEBUG_SECTION printfs all received sections
//
//#define DEBUG_PIC_TIME
//#define DEBUG_SECTION
//#define DMX_DEBUG_PICTURE_SEARCH

// Debug
#ifdef CC_MINI_DRIVER
#undef CC_MINI_DRIVER
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#ifdef SYNC_PES_HEADER
    #define PES_HEADER_EXTRA_BYTES          12
    #define PAYLOAD_INFO_OFFSET             8
#else
    #define PES_HEADER_EXTRA_BYTES          8
    #define PAYLOAD_INFO_OFFSET             4
#endif

//
// PES header attributes
//

#define PES_HEADER_PACKET_LENGTH_OFFSET     4
#define PES_HEADER_DATA_LENGTH_OFFSET       8

#define PES_HEADER_FLAG_OFFSET              6
#define PES_HEADER_PTS_MASK                 (1 << 7)
#define PES_HEADER_DTS_MASK                 (1 << 6)
#define PES_HEADER_ORIGINAL_MASK            (1 << 8)
#define PES_HEADER_COPYRIGHT_MASK           (1 << 9)

#define PES_PTS_FIELD_SIZE                  5
#define PES_DTS_FIELD_SIZE                  5

#define PES_HEADER_FIELD_OFFSET             9
#define PES_HEADER_PTS_OFFSET               (PES_HEADER_FIELD_OFFSET + 0)
#define PES_HEADER_DTS_OFFSET               \
    (PES_HEADER_FIELD_OFFSET + PES_PTS_FIELD_SIZE)

#define PES_HEADER_COPY_SIZE                \
    (PES_HEADER_DTS_OFFSET + PES_DTS_FIELD_SIZE + 1)
#define PES_HEADER_ALIGNMENT                4

#define PES_EXTRA_AUDIO_INFO_LEN            8

//
// Picture header attribytes
//

// MPEG Picture start codes
#define PIC_START_CODE_1                    0
#define PIC_START_CODE_2                    0
#define PIC_START_CODE_3                    1
#define PIC_START_CODE_PIC                  0
#define PIC_START_CODE_SEQ_START            0xb3
#define PIC_START_CODE_SEQ_END              0xb7
#define PIC_START_CODE_GOP                  0xb8

// Hardware picture types
#define PIC_HW_TYPE_PIC                     0
#define PIC_HW_TYPE_SEQ_START               1
#define PIC_HW_TYPE_SEQ_END                 2
#define PIC_HW_TYPE_GOP                     3
#define PIC_HW_TYPE_UNKNOWN                 7

#define PIC_HEADER_LEN                      6


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// PES header structure
typedef struct
{
    UINT32 u4Pts;
    UINT32 u4Dts;
    UINT32 u4PayloadAddr;
    UINT32 u4PayloadSize;
    BOOL fgCopyright;
    BOOL fgOriginal;
} DMX_PES_HEADER_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef DMX_DEBUG_PICTURE_SEARCH
/// Previous picture read pointers (for debugging picture finder)
static UINT32 _au4PicRp[DMX_NUM_PID_INDEX];

/// Last picture states (for debugging picture finder)
static UINT8 _au1LastPicState[DMX_NUM_PID_INDEX];
static UINT8 _au1LastPicType[DMX_NUM_PID_INDEX];    // MT5391 for new pattern matching
#endif  // DMX_DEBUG_PICTURE_SEARCH

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef DMX_DEBUG_PICTURE_SEARCH
//-----------------------------------------------------------------------------
/** _DmxPanic
 *  Panic for fatal error
 *
 *  @param  szMsg           The error message
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxPanic(CHAR* szMsg)
{
    LOG(1, "\n\nError: %s!\n", szMsg);

    BSP_HaltSystem();
}
#endif // DMX_DEBUG_PICTURE_SEARCH


//  These functions exist only in non-mini driver
#ifndef CC_MINI_DRIVER


//-----------------------------------------------------------------------------
/** _DmxSendToIde
 *  Send received packet of a PID to IDETPOUT
 *
 *  @param  u1PidIndex      PID index
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxSendToIde(UINT8 u1PidIndex)
{
    UINT32 u4Rp, u4Wp;
    PID_STRUCT_T* prPidStruct;

    prPidStruct = _DMX_GetPidStruct(u1PidIndex);
    u4Rp = prPidStruct->u4Rp;
    u4Wp = prPidStruct->u4Wp;

    VERIFY(_DMX_IDE_SendPacket(u1PidIndex, prPidStruct->u4BufStart,
        prPidStruct->u4BufEnd + 1, u4Rp, u4Wp));

    // Update PID data structure
    prPidStruct->u4Rp = u4Wp;
    PID_S_W(u1PidIndex, 9) = PHYSICAL(u4Wp);
}


#endif  // CC_MINI_DRIVER


#ifdef ENABLE_MULTIMEDIA
static BOOL _DmxParseMPEG1Header(UINT32 u4HeaderInfoAddr, UINT32 u4HeaderAddr,
                UINT32 u4HeaderWp, UINT32 u4BufStart, UINT32 u4BufEnd,
                const PID_STRUCT_T* prPidStruct, DMX_PES_HEADER_T* prPesHeader)
{
    UINT32 u4HeaderFlagAddr, u4BufSize;
    UINT32 u4PayloadSize, u4PayloadInfoAddr, u4PayloadAddr;
    UINT8 *pu1Sentinel, *pu1Data, au1Data[12], u1Offset;
    UINT32 u4Counter, u4Pts, u4Dts;
    BOOL fgRet = FALSE;
    UINT32 i;

    if((prPidStruct == NULL) || (prPesHeader == NULL))
    {
        return FALSE;
    }

    u4BufSize = u4BufEnd - u4BufStart;
    u4HeaderFlagAddr = u4HeaderAddr + PES_HEADER_FLAG_OFFSET;
    if (u4HeaderFlagAddr >= u4BufEnd)
    {
        u4HeaderFlagAddr -= u4BufSize;
        if ((u4HeaderFlagAddr < u4BufStart) || (u4HeaderFlagAddr >= u4BufEnd))
        {
            LOG(3, "MPEG1 header flag address is out of range!\n");
            return FALSE;
        }
    }

    // Rewrite x_memset as a for loop to eliminate a stupid Klocwork waring.
    //x_memset((void*)au1Data, 0, sizeof(au1Data));
    for (i = 0; i < 12; i++)
    {
        au1Data[i] = 0;
    }

    VERIFY(_DMX_CopyRingBuffer((UINT32)au1Data, (UINT32)au1Data,
        (UINT32)(au1Data + PES_HEADER_FLAG_OFFSET), u4HeaderAddr, u4BufStart,
        u4BufEnd, PES_HEADER_FLAG_OFFSET) == (UINT32)au1Data);

    u4PayloadSize = ((UINT16)(au1Data[4]) << 8) + au1Data[5];

    u4PayloadInfoAddr = _DMX_AdvanceAddr(u4HeaderInfoAddr,
        PAYLOAD_INFO_OFFSET, u4HeaderWp, u4BufStart, u4BufEnd);
    DMX_PANIC(u4PayloadInfoAddr != 0);
    DMX_PANIC_RET(FALSE);
    if(u4PayloadInfoAddr == 0)      // klocwork
    {
        return FALSE;
    }
    u4PayloadAddr = *(UINT32*)u4PayloadInfoAddr;

    u4Counter = 0;
    pu1Sentinel = (UINT8*)u4HeaderFlagAddr;
    pu1Data     = (UINT8*)u4HeaderFlagAddr;
    while ((*pu1Data) == 0xFF)      // Check for stuffing byte.
    {
        pu1Data++;
        if (((UINT32)pu1Data) >= u4BufEnd)
        {
            pu1Data = (UINT8*)((UINT32)pu1Data - u4BufSize);
        }
        if (pu1Sentinel == pu1Data)
        {
            LOG(3, "Wrapping around to the same Header location!\n");
            return FALSE;
        }
        u4Counter++;
        if (u4Counter > 0xFFFB)     // 0xFFFF - 4
        {
            LOG(3, "Stuffing byte exceeds maximum packet length!\n");
            return FALSE;
        }
    }

    u4HeaderFlagAddr = (UINT32)pu1Data;
    // Copy to non-ring buffer to ease parsing
    VERIFY(_DMX_CopyRingBuffer((UINT32)au1Data, (UINT32)au1Data,
        (UINT32)(au1Data + sizeof(au1Data)), u4HeaderFlagAddr, u4BufStart,
        u4BufEnd, sizeof(au1Data)) == (UINT32)au1Data);

    u4Pts    = 0;
    u4Dts    = 0;
    u1Offset = 0;
    if ((au1Data[0] & 0xC0) == 0x40)    // STD buffer info
    {
        u1Offset = 2;
    }

    if ((au1Data[0] & 0xF0) == 0x20)        // only PTS
    {
        u4Pts = (((UINT32)(au1Data[0 + u1Offset]) & 0x0e) << 29) |
                 ((UINT32)(au1Data[1 + u1Offset])         << 22) |
                (((UINT32)(au1Data[2 + u1Offset]) & 0xfe) << 14) |
                 ((UINT32)(au1Data[3 + u1Offset])         << 7)  |
                (((UINT32)(au1Data[4 + u1Offset]) & 0xfe) >> 1);
        fgRet = TRUE;
    }
    else if (((UINT32)(au1Data[0]) & 0xF0) == 0x30)        // PTS followed by DTS
    {
        u4Pts = (((UINT32)(au1Data[0 + u1Offset]) & 0x0e) << 29) |
                 ((UINT32)(au1Data[1 + u1Offset])         << 22) |
                (((UINT32)(au1Data[2 + u1Offset]) & 0xfe) << 14) |
                 ((UINT32)(au1Data[3 + u1Offset])         << 7)  |
                (((UINT32)(au1Data[4 + u1Offset]) & 0xfe) >> 1);
        if (((UINT32)(au1Data[5]) & 0xF0) == 0x10)
        {
            u4Dts = (((UINT32)(au1Data[5 + u1Offset]) & 0x0e) << 29) |
                     ((UINT32)(au1Data[6 + u1Offset])         << 22) |
                    (((UINT32)(au1Data[7 + u1Offset]) & 0xfe) << 14) |
                     ((UINT32)(au1Data[8 + u1Offset])         << 7)  |
                    (((UINT32)(au1Data[9 + u1Offset]) & 0xfe) >> 1);
        }
        fgRet = TRUE;
    }
    else
    {
        // I do not check the header buffer for the appearance of 0x0F.
    }

    prPesHeader->u4Pts = u4Pts;
    prPesHeader->u4Dts = u4Dts;
    prPesHeader->u4PayloadSize = u4PayloadSize;
    prPesHeader->u4PayloadAddr = u4PayloadAddr;
    prPesHeader->fgCopyright = TRUE;
    prPesHeader->fgOriginal = TRUE;

    LOG(7, "MPEG1 PTS: %d, DTS: %d\n", u4Pts, u4Dts);

    return fgRet;
}
#endif  // ENABLE_MULTIMEDIA

//-----------------------------------------------------------------------------
/** _DmxParsePesHeader
 *  Parse PES header
 *
 *  @param  u4HeaderInfoAddr    Address of header info
 *  @param  u4HeaderWp          Header write pointer
 *  @param  u4BufStart          Header buffer start address
 *  @param  u4BufEnd            Header buffer end address
 *  @param  prPidStruct         PID structure
 *  @param  prPesHeader [output]
 *                              PES header structure
 *
 *  @retval The hardware type
 */
//-----------------------------------------------------------------------------
static BOOL _DmxParsePesHeader(UINT32 u4HeaderInfoAddr, UINT32 u4HeaderWp,
    UINT32 u4BufStart, UINT32 u4BufEnd, const PID_STRUCT_T* prPidStruct,
    DMX_PES_HEADER_T* prPesHeader)
{
    UINT64 u8Pts, u8Dts;
    UINT32 u4HeaderAddr, u4HeaderFlag, u4PayloadSize;
    UINT32 u4PayloadInfoAddr, u4PayloadAddr;
    UINT8 au1Header[PES_HEADER_COPY_SIZE];
    BOOL fgCopyright, fgOriginal;
    UINT8* pu1Data;
    INT32 i4PtsDrift;
    BOOL fgRet = FALSE;

    DMX_PANIC((u4HeaderInfoAddr >= u4BufStart) && (u4HeaderInfoAddr < u4BufEnd));
    DMX_PANIC_RET(FALSE);

    if((prPesHeader == NULL) || (prPidStruct == NULL))
    {
        return FALSE;
    }

    // Get PES header address
    u4HeaderAddr = _DMX_AdvanceAddr(u4HeaderInfoAddr, PES_HEADER_EXTRA_BYTES,
        u4HeaderWp, u4BufStart, u4BufEnd);
    DMX_PANIC(u4HeaderAddr != 0);
    DMX_PANIC_RET(FALSE);

    // Copy to non-ring buffer to ease parsing
    VERIFY(_DMX_CopyRingBuffer((UINT32)au1Header, (UINT32)au1Header,
        (UINT32)(au1Header + PES_HEADER_COPY_SIZE), u4HeaderAddr, u4BufStart,
        u4BufEnd, PES_HEADER_COPY_SIZE) == (UINT32)au1Header);

    // Get PES header flags
    pu1Data = au1Header + PES_HEADER_FLAG_OFFSET;
    u4HeaderFlag = (pu1Data[0] << 8) | pu1Data[1];

#ifdef ENABLE_MULTIMEDIA
    // Program Stream may contain MPEG1 data.
    if (_DMX_GetInputType() == DMX_IN_PLAYBACK_PS)
    {
        // If the next two bits are 10, it's MPEG2. Otherwise, it may be MPEG1.
        if ((u4HeaderFlag & 0xC000) != 0x8000)
        {
            if (_DmxParseMPEG1Header(u4HeaderInfoAddr, u4HeaderAddr, u4HeaderWp,
                                u4BufStart, u4BufEnd, prPidStruct, prPesHeader))
            {
                return TRUE;
            }
            return FALSE;
        }
    }
#endif  // ENABLE_MULTIMEDIA

    // Get PTS/DTS
    u8Pts = (u8Dts = 0);                // Parentheses make Lint happy
    if ((u4HeaderFlag & PES_HEADER_PTS_MASK) != 0)
    {
        pu1Data = au1Header + PES_HEADER_PTS_OFFSET;
        u8Pts = (((UINT64)pu1Data[0] & 0x0e) << 29) |
                (pu1Data[1] << 22) |
                ((pu1Data[2] & 0xfe) << 14) |
                (pu1Data[3] << 7) |
                ((pu1Data[4] & 0xfe) >> 1);

        if ((u4HeaderFlag & PES_HEADER_DTS_MASK) != 0)
        {
            pu1Data = au1Header + PES_HEADER_DTS_OFFSET;
            u8Dts = (((UINT64)pu1Data[0] & 0x0e) << 29) |
                    (pu1Data[1] << 22) |
                    ((pu1Data[2] & 0xfe) << 14) |
                    (pu1Data[3] << 7) |
                    ((pu1Data[4] & 0xfe) >> 1);
        }
        fgRet = TRUE;
    }

    // Get copyright and original_or_copy flags
    fgCopyright = (((u4HeaderFlag & PES_HEADER_COPYRIGHT_MASK) != 0) ? TRUE :
        FALSE);
    fgOriginal = (((u4HeaderFlag & PES_HEADER_ORIGINAL_MASK) != 0) ? TRUE:
        FALSE);

    // Get payload size
    pu1Data = au1Header + PES_HEADER_PACKET_LENGTH_OFFSET;
    u4PayloadSize = ((UINT16)(pu1Data[0]) << 8) + pu1Data[1];

    // Get payload address
    u4PayloadInfoAddr = _DMX_AdvanceAddr(u4HeaderInfoAddr,
        PAYLOAD_INFO_OFFSET, u4HeaderWp, u4BufStart, u4BufEnd);
    DMX_PANIC(u4PayloadInfoAddr != 0);
    DMX_PANIC_RET(FALSE);
    if(u4PayloadInfoAddr == 0)      // klocwork
    {
        return FALSE;
    }
    u4PayloadAddr = *(UINT32*)u4PayloadInfoAddr;

    i4PtsDrift = 0;
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        i4PtsDrift = STC_GetPtsDriftByVdoDeviceId(
            (UCHAR) prPidStruct->u1DeviceId);
    }
    else
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        i4PtsDrift = STC_GetPtsDriftByAudDeviceId(
            (UCHAR) prPidStruct->u1DeviceId);
    }

    if (i4PtsDrift != 0)
    {
        if (u8Pts != 0)
        {
            if (i4PtsDrift > 0)
            {
                u8Pts += (UINT32) i4PtsDrift;
            }
            else
            {
                u8Pts -= (UINT32) (-i4PtsDrift);
            }
        }

        if (u8Dts != 0)
        {
            if (i4PtsDrift > 0)
            {
                u8Dts += (UINT32) i4PtsDrift;
            }
            else
            {
                u8Dts -= (UINT32) (-i4PtsDrift);
            }
        }
    }

    prPesHeader->u4Pts = (UINT32)(u8Pts & 0xffffffff);
    prPesHeader->u4Dts = (UINT32)(u8Dts & 0xffffffff);
    prPesHeader->u4PayloadSize = u4PayloadSize;
    prPesHeader->u4PayloadAddr = u4PayloadAddr;
    prPesHeader->fgCopyright = fgCopyright;
    prPesHeader->fgOriginal = fgOriginal;

    LOG(7, "PTS: 0x%08x, DTS: 0x%08x, payload: 0x%08x, size: %u\n",
        prPesHeader->u4Pts, prPesHeader->u4Dts, u4PayloadAddr, u4PayloadSize);

    //Printf("PTS: 0x%08x, DTS: 0x%08x, payload: 0x%08x, size: %u\n",
    //    prPesHeader->u4Pts, prPesHeader->u4Dts, u4PayloadAddr, u4PayloadSize);

    return fgRet;
}

#ifdef ENABLE_MULTIMEDIA
//-----------------------------------------------------------------------------
/** _DmxGetExtraAuidoInfo
 *
 *  This function need to set the value of prPes->u4Info either on success or
 *  on failure.  It also needs to return 0 on error.
 *
 *  @param  u1Pidx              PID index
 *  @param  u4HeaderRp          Header Read pointer
 *  @param  u4HeaderSize        PES Header size
 *  @param  u4HeaderWp          Header write pointer
 *  @param  u4HeaderBufStart    Header buffer start address
 *  @param  u4HeaderBufEnd      Header buffer end address
 *  @param  rPes  [output]      Audio PES structure
 *
 *  @retval The length of extra audio info in the PES Header circular buffer.
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxGetExtraAuidoInfo(UINT8 u1Pidx, UINT32 u4HeaderRp,
                            UINT32 u4HeaderSize, UINT32 u4HeaderWp,
                            UINT32 u4HeaderBufStart, UINT32 u4HeaderBufEnd,
                            DMX_AUDIO_PES_T *prPes)
{
    UINT32 u4AudioInfoAddr, u4AudioInfoEndAddr, u4ExtraLen;
    UINT8 au1AudioInfo[PES_EXTRA_AUDIO_INFO_LEN];
    DMX_AUDIO_TYPE_T eAudioType;

    DMX_PANIC(u4HeaderRp != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC(u4HeaderWp != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC(u4HeaderBufStart != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC(u4HeaderBufEnd != 0);
    DMX_PANIC_RET(0);

    if(prPes == NULL)
    {
        return 0;
    }

    // Important: initialize the extra audio info to 0.
    prPes->u4Info = 0;

    if (_DMX_GetInputType() != DMX_IN_PLAYBACK_PS)
    {
        return 0;
    }

    eAudioType = _DMX_PS_GetAudioType();
    if ((eAudioType != DMX_AUDIO_LPCM) && (eAudioType != DMX_AUDIO_AC3))
    {
        return 0;
    }

    u4AudioInfoAddr = _DMX_AdvanceAddr(u4HeaderRp, (INT32)u4HeaderSize,
        u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
    DMX_PANIC(u4AudioInfoAddr != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC((u4AudioInfoAddr >= u4HeaderBufStart) && (u4AudioInfoAddr < u4HeaderBufEnd));
    DMX_PANIC_RET(0);

    u4ExtraLen = _DMX_Align((PID_S_W(u1Pidx, 2) & 0xFF), PES_HEADER_ALIGNMENT);
    if (u4ExtraLen > 8)
    {
        return 0;
    }

    u4AudioInfoEndAddr = _DMX_AdvanceAddr(u4AudioInfoAddr, (INT32)u4ExtraLen,
        u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
    if (u4AudioInfoEndAddr == 0)
    {
        return 0;
    }

    x_memset((void*)au1AudioInfo, 0, sizeof(au1AudioInfo));

    // Copy to non-ring buffer for easier manipulation
    VERIFY((UINT32)au1AudioInfo == _DMX_CopyRingBuffer((UINT32)au1AudioInfo,
        (UINT32)au1AudioInfo, (UINT32)(au1AudioInfo + PES_EXTRA_AUDIO_INFO_LEN),
        u4AudioInfoAddr, u4HeaderBufStart, u4HeaderBufEnd, PES_EXTRA_AUDIO_INFO_LEN));

    if (eAudioType == DMX_AUDIO_LPCM)
    {
        if (u4ExtraLen != 8)
        {
            return 0;
        }
        prPes->u4Info = (au1AudioInfo[4] << 16) | (au1AudioInfo[5] << 8) |
                         au1AudioInfo[6];
    }
    else if (eAudioType == DMX_AUDIO_AC3)
    {
        if (u4ExtraLen != 4)
        {
            return 0;
        }
        prPes->u4Info = (au1AudioInfo[0] << 24) | (au1AudioInfo[1] << 16) |
                        (au1AudioInfo[2] << 8) | au1AudioInfo[3];
    }

    return u4ExtraLen;
}
#endif  // ENABLE_MULTIMEDIA

//-----------------------------------------------------------------------------
/** _DmxDetectPictureType
 *  Detect picture type at a given address
 *
 *  @param  u4Addr          The picture address
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *
 *  @retval Picture type:
 *                          PIC_TYPE_UNKNOWN
 *                          PIC_TYPE_I
 *                          PIC_TYPE_P
 *                          PIC_TYPE_B
 *                          PIC_TYPE_SEQ_START
 *                          PIC_TYPE_GOP
 *                          PIC_TYPE_SEQ_END
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxDetectMPEGPictureType(UINT32 u4Addr, UINT32 u4BufStart,
    UINT32 u4BufEnd)
{
    UINT32 u4PicType;
    UINT8* p;
    UINT8 u1Type, h[PIC_HEADER_LEN];

    if ((u4Addr < u4BufStart) || (u4Addr >= u4BufEnd))
    {
        LOG(3, "Invalid picture address 0x%08x, pidx: %u\n");

        return PIC_TYPE_UNKNOWN;
    }

    // Check if the header is across FIFO boundary
    p = (UINT8*)u4Addr;
    if (((u4Addr + PIC_HEADER_LEN) - 1) >= u4BufEnd)
    {
        UINT8 u1Len = (UINT8)(u4BufEnd - u4Addr);
        if(u1Len >= PIC_HEADER_LEN)
        {
            return PIC_TYPE_UNKNOWN;
        }
        x_memcpy((void*)h, (void*)p, u1Len);
        x_memcpy((void*)(h + u1Len), (void*)u4BufStart,
            (SIZE_T)(PIC_HEADER_LEN - u1Len));
        p = h;
    }

    // Check start code
    if ((p[0] != PIC_START_CODE_1) || (p[1] != PIC_START_CODE_2) ||
        (p[2] != PIC_START_CODE_3))
    {
        return PIC_TYPE_UNKNOWN;
    }

    // Check picture type
    u1Type = p[3];
    u4PicType = PIC_TYPE_UNKNOWN;

    switch (u1Type)
    {
    case PIC_START_CODE_SEQ_START:
        u4PicType = PIC_TYPE_SEQ_START;
        break;

    case PIC_START_CODE_GOP:
        u4PicType = PIC_TYPE_GOP;
        break;

    case PIC_START_CODE_SEQ_END:
        u4PicType = PIC_TYPE_SEQ_END;
        break;

    case PIC_START_CODE_PIC:
        u4PicType = (p[5] >> 3) & 0x7;
        if ((u4PicType != PIC_TYPE_I) && (u4PicType != PIC_TYPE_P) &&
            (u4PicType != PIC_TYPE_B))
        {
            u4PicType = PIC_TYPE_UNKNOWN;
        }
        break;

    default:
        break;
    }

    return u4PicType;
}


//-----------------------------------------------------------------------------
/** _DmxDetectPictureType
 *  Detect picture type at a given address
 *
 *  @param  u4Addr          The picture address
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxDetectPictureType(UINT32 u4Addr, UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 u4Type = PIC_TYPE_UNKNOWN;

    switch(_DMX_GetVideoType())
    {
    case DMX_VIDEO_MPEG:
        u4Type = _DmxDetectMPEGPictureType(u4Addr, u4BufStart, u4BufEnd);
        break;

    default:
        //LOG(3, "Unknown video type!\n");
        break;
    }

    return u4Type;
}


#ifdef DMX_DEBUG_PICTURE_SEARCH
//-----------------------------------------------------------------------------
/** _DmxPictureTypeToHwType
 *  Convert picture type to hardware type
 *
 *  @param  u1Type          The picture type to be converted
 *
 *  @retval The hardware type
 */
//-----------------------------------------------------------------------------
static UINT8 _DmxPictureTypeToHwType(UINT8 u1Type)
{
    UINT8 u1HwType;

    switch (u1Type)
    {
    case PIC_START_CODE_PIC:
        u1HwType = PIC_HW_TYPE_PIC;
        break;

    case PIC_START_CODE_SEQ_START:
        u1HwType = PIC_HW_TYPE_SEQ_START;
        break;

    case PIC_START_CODE_SEQ_END:
        u1HwType = PIC_HW_TYPE_SEQ_END;
        break;

    case PIC_START_CODE_GOP:
        u1HwType = PIC_HW_TYPE_GOP;
        break;

    case PIC_TYPE_UNKNOWN:
    default:
        u1HwType = PIC_HW_TYPE_UNKNOWN;
        break;
    }

    return u1HwType;
}


//-----------------------------------------------------------------------------
/** _DmxPictureFinder
 *  Picture finder
 *
 *  @param  pu1Type [out]   Picture type if a picture is found
 *  @param  u1Pidx          PID index
 *  @param  u4Rp            Read pointer
 *  @param  u4Wp            Write pointer
 *
 *  @retval The picture address, 0 if no picture found
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxPictureFinder(UINT8* pu1Type, UINT8 u1Pidx, UINT32 u4Rp,
    UINT32 u4Wp)
{
    UINT8 *p, *q;
    UINT8 u1LastPicState;
    BOOL fgLoop;

    ASSERT(pu1Type != NULL);
    ASSERT(u4Rp <= u4Wp);

    if(_DMX_GetVideoType() != DMX_VIDEO_MPEG)
    {
        LOG(3, "Not MPEG type\n");
        return 0;
    }

/* MT5391 for new pattern matching
    if ((u4Wp - u4Rp) < 4)
    {
        return 0;
    }
*/

    *pu1Type = _au1LastPicType[u1Pidx];
    _au1LastPicType[u1Pidx] = PIC_TYPE_UNKNOWN;

    u1LastPicState = _au1LastPicState[u1Pidx];
    _au1LastPicState[u1Pidx] = 0;
    p = (q = (UINT8*)u4Rp);

    // Check previous state
    do
    {
        fgLoop = FALSE;
        switch (u1LastPicState)
        {
        case 1:
            if (*q != PIC_START_CODE_2)
            {
                break;
            }
            q++;
            u1LastPicState = 2;
            if ((UINT32)q == u4Wp) {
                _au1LastPicState[u1Pidx] = u1LastPicState;
                return 0;
            }
            else
            {
                fgLoop = TRUE;
            }
            break;

        case 2:
            if (*q != PIC_START_CODE_3)
            {
                break;
            }
            q++;
            u1LastPicState = 3;
            if ((UINT32)q == u4Wp)
            {
                _au1LastPicState[u1Pidx] = u1LastPicState;
                return 0;
            }
            else
            {
                fgLoop = TRUE;
            }
            break;

        case 3:
            if ((*q == PIC_START_CODE_PIC) || (*q == PIC_START_CODE_SEQ_START) ||
                (*q == PIC_START_CODE_GOP) || (*q == PIC_START_CODE_SEQ_END))
            {
                *pu1Type = *q;
                _au1LastPicType[u1Pidx] = *pu1Type;
                q++;
                u1LastPicState = 4;
                if ((UINT32)q == u4Wp)
                {
                    _au1LastPicState[u1Pidx] = u1LastPicState;
                    return 0;
                }
                else
                {
                    fgLoop = TRUE;
                }
            }
            break;

        case 4:
            q++;
            u1LastPicState = 5;
            if ((UINT32)q == u4Wp)
            {
                _au1LastPicState[u1Pidx] = u1LastPicState;
                return 0;
            }
            else
            {
                fgLoop = TRUE;
            }
            break;

        case 5:
            return (UINT32)q;

        default:
            break;
        }
    } while (fgLoop);

    while ((UINT32)p < (u4Wp - 5))
    {
        if ((p[0] == PIC_START_CODE_1) && (p[1] == PIC_START_CODE_2) &&
            (p[2] == PIC_START_CODE_3))
        {
            UINT8 u1Type = PIC_TYPE_UNKNOWN;

            switch (p[3])
            {
            case PIC_START_CODE_PIC:
            case PIC_START_CODE_SEQ_START:
            case PIC_START_CODE_GOP:
            case PIC_START_CODE_SEQ_END:
                u1Type = p[3];
                break;

            default:
                break;
            }

            if (u1Type != PIC_TYPE_UNKNOWN)
            {
                *pu1Type = u1Type;
                return (UINT32)(p + 5);
            }
        }

        p++;
    }

    // Check last 5 bytes
    u1LastPicState = 0;
    p = (UINT8*)(u4Wp - 5);
    if (((u4Wp - 5) >= u4Rp) &&
        (p[0] == PIC_START_CODE_1) && (p[1] == PIC_START_CODE_2) &&
        (p[2] == PIC_START_CODE_3) && (
         (p[3] == PIC_START_CODE_PIC) ||
         (p[3] == PIC_START_CODE_SEQ_START) ||
         (p[3] == PIC_START_CODE_GOP) ||
         (p[3] == PIC_START_CODE_SEQ_END) ))
    {
        _au1LastPicType[u1Pidx] = p[3];
        u1LastPicState = 5;
    }
    else if (((u4Wp - 5) >= (u4Rp - 1)) &&
        (p[1] == PIC_START_CODE_1) && (p[2] == PIC_START_CODE_2) &&
        (p[3] == PIC_START_CODE_3) && (
         (p[4] == PIC_START_CODE_PIC) ||
         (p[4] == PIC_START_CODE_SEQ_START) ||
         (p[4] == PIC_START_CODE_GOP) ||
         (p[4] == PIC_START_CODE_SEQ_END) ))
    {
        _au1LastPicType[u1Pidx] = p[4];
        u1LastPicState = 4;
    }
    else if (((u4Wp - 5) >= (u4Rp - 2)) &&
        (p[2] == PIC_START_CODE_1) && (p[3] == PIC_START_CODE_2) &&
        (p[4] == PIC_START_CODE_3))
    {
        u1LastPicState = 3;
    }
    else if (((u4Wp - 5) >= (u4Rp - 3)) &&
        (p[3] == PIC_START_CODE_1) && (p[4] == PIC_START_CODE_2))
    {
        u1LastPicState = 2;
    }
    else if (((u4Wp - 5) >= (u4Rp - 4)) &&
        (p[4] == PIC_START_CODE_1))
    {
        u1LastPicState = 1;
    }

    _au1LastPicState[u1Pidx] = u1LastPicState;

    return 0;
}


//-----------------------------------------------------------------------------
/** _DmxProcessVideoPesHeaderDebug
 *  Process Video PES header, with picture finder verification
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessVideoPesHeaderDebug(UINT8 u1Pidx)
{
    UINT32 u4Rp, u4Wp, u4BufStart, u4BufEnd, u4BufSize, u4PicNum;
    UINT32 u4HeaderRp, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd;
    UINT32 u4HwPicNum, u4HeaderByteCount, u4FrontHeaderSize;
    UINT32 u4HeaderSize;
    UINT32 u4PicInfoAddr;
    UINT8 u1Type;
    CHAR buf[128];
    PID_STRUCT_T* prPidStruct;
    BOOL fgIsToDecoder;

#ifdef DEBUG_PIC_TIME
    HAL_TIME_T rTime;
#endif  // DEBUG_PIC_TIME

#ifdef SYNC_PES_HEADER
    UINT32 u4WpInfoAddr;
#endif  // SYNC_PES_HEADER

    // Check if TO-CDFIFO is enabled
    if ((PID_S_W(u1Pidx, 0) & (1 << 6)) == 0)
    {
        // TO-CDFIFO is disabled, stop here
        return FALSE;
    }

    //---------------------------------------------------------------
    // Get hardware picture information
    //---------------------------------------------------------------
#ifdef SYNC_PES_HEADER
    u4HeaderRp = VIRTUAL(DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2));
    ASSERT(u4HeaderRp == VIRTUAL(PID_S_W(u1Pidx, 14)));    // Double confirm
#else
    u4HeaderRp = VIRTUAL(PID_S_W(u1Pidx, 14));
#endif  // SYNC_PES_HEADER

    u4HeaderWp = VIRTUAL(PID_S_W(u1Pidx, 13));
    ASSERT(_DMX_IsAligned(u4HeaderRp, PES_HEADER_ALIGNMENT));
    u4HeaderBufStart = VIRTUAL(PID_S_W(u1Pidx, 10));
    u4HeaderBufEnd = VIRTUAL(PID_S_W(u1Pidx, 11) + 1);
    ASSERT((u4HeaderRp >= u4HeaderBufStart) && (u4HeaderRp < u4HeaderBufEnd));

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;
    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    u4FrontHeaderSize = _DMX_Align(u4HeaderByteCount, PES_HEADER_ALIGNMENT)
        + PES_HEADER_EXTRA_BYTES;
    u4HeaderSize = u4FrontHeaderSize + (u4HwPicNum * 4);
    u4PicInfoAddr = _DMX_AdvanceAddr(u4HeaderRp, (INT32)u4FrontHeaderSize,
        u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
    u4PicNum = 0;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    //---------------------------------------------------------------

    u4Rp = _au4PicRp[u1Pidx];
    if (u4Rp == 0)
    {
        u4Rp = VIRTUAL(prPidStruct->u4Rp);
    }

#ifdef SYNC_PES_HEADER
    u4WpInfoAddr = _DMX_AdvanceAddr(u4HeaderRp, 4, u4HeaderWp,
        u4HeaderBufStart, u4HeaderBufEnd);
    ASSERT(u4WpInfoAddr != 0);
    prPidStruct->u4Wp = VIRTUAL(*(UINT32*)u4WpInfoAddr);
#endif  // SYNC_PES_HEADER

    u4Wp = VIRTUAL(prPidStruct->u4Wp);
    u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
    u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
    u4BufSize = u4BufEnd - u4BufStart;
    ASSERT((u4Rp >= u4BufStart) && (u4Rp < u4BufEnd));
    ASSERT((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));

    fgIsToDecoder = _DMX_IsToDecoder();

    while (1)
    {
        UINT32 u4From, u4To, u4Pic;

        if (DATASIZE(u4Rp, u4Wp, u4BufSize) < 1)    // MT5391 for new pattern matching
        {
            // Done
            break;
        }

        u4From = u4Rp;
        if (u4Rp < u4Wp)
        {
            u4To = u4Wp;
        }
        else
        {
            u4To = u4BufEnd;
        }

        u4Pic = _DmxPictureFinder(&u1Type, u1Pidx, u4From, u4To);

        if (u4Pic != 0)
        {
            UINT32 u4PicInfo, u4Addr;
            UINT8 u1HwType;

            ASSERT((u4Pic >= u4BufStart) && (u4Pic < u4BufEnd));

            // Check with PES header
            if (u4PicInfoAddr == 0)
            {
                sprintf(buf, "Miss header! Pidx: %u, addr: 0x%08x, index: %u",
                    u1Pidx, u4Pic, u4PicNum);
                _DmxPanic(buf);
                ASSERT(0);      // Make Lint happy
            }

            u4PicInfo = *(UINT32*)u4PicInfoAddr;

            u4PicInfoAddr = _DMX_AdvanceAddr(u4PicInfoAddr, 4, u4HeaderWp,
                u4HeaderBufStart, u4HeaderBufEnd);

            u1HwType = (u4PicInfo >> 28) & 0xf; // MT5391 for new pattern matching

            Printf("pic: 0x%08x, type: %u, pidx: %u\n", u4Pic, u1HwType, u1Pidx);

            if ((u1HwType == 4)||(u1HwType == 5))   // 5391
            {
                u1HwType = 0;
            }

            u4Addr = VIRTUAL(u4PicInfo & 0x0fffffff);   // MT5391 for new pattern matching
            u1Type = _DmxPictureTypeToHwType(u1Type);

            ASSERT((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd));

            if ((u1Type != u1HwType) || (u4Pic != u4Addr))
            {
                sprintf(buf, "Header! Pidx: %u, addr: 0x%08x (0x%08x),"
                    "type: %u (%u), index: %u",
                    u1Pidx, u4Addr, u4Pic, u1HwType, u1Type, u4PicNum);
                _DmxPanic(buf);
            }

            //LOG(7, "pic: 0x%08x, type: %u, pidx: %u\n", u4Pic, u1Type, u1Pidx);
#ifdef DEBUG_PIC_TIME
            // Debug
            HAL_GetTime(&rTime);
            Printf("time: %d.%d\n", rTime.u4Seconds, rTime.u4Micros);
#endif  // DEBUG_PIC_TIME
            u4PicNum++;
            u4Rp = u4Pic + 1;

            if (fgIsToDecoder)
            {
                DMX_PES_HEADER_T rPesHeader;
                DMX_PES_MSG_T rPes;
                UINT8 u1Channel;

                // Parse PES header to get PTS/DTS
                rPes.u4Pts = 0;
                rPes.u4Dts = 0;
                if (_DmxParsePesHeader(u4HeaderRp, u4HeaderWp,
                    u4HeaderBufStart, u4HeaderBufEnd, prPidStruct,
                    &rPesHeader))
                {
                    rPes.u4Pts = rPesHeader.u4Pts;
                    rPes.u4Dts = rPesHeader.u4Dts;
                }

                if (!_DMX_GetVideoChannel(u1Pidx, &u1Channel))
                {
                    LOG(3, "No video channel mapping to pidx %u!\n", u1Pidx);
                    continue;
                }

                rPes.u4FrameAddr = _DMX_AdvanceAddr(u4Addr, -5, u4Wp, u4BufStart,
                    u4BufEnd);
                rPes.u4FrameType = _DmxDetectPictureType(rPes.u4FrameAddr,
                    u4BufStart, u4BufEnd);

                if ( ! ((((u4PicInfo >> 29) == 0) && (rPes.u4FrameType == PIC_TYPE_I)) ||
                        (((u4PicInfo >> 29) == 4) && (rPes.u4FrameType == PIC_TYPE_P)) ||
                        (((u4PicInfo >> 29) == 5) && (rPes.u4FrameType == PIC_TYPE_B)) ||
                        (((u4PicInfo >> 29) == 1) && (rPes.u4FrameType == PIC_TYPE_SEQ_START)) ||
                        (((u4PicInfo >> 29) == 3) && (rPes.u4FrameType == PIC_TYPE_GOP)) ||
                        (((u4PicInfo >> 29) == 2) && (rPes.u4FrameType == PIC_TYPE_SEQ_END)))) {

                            Printf("%d\t%d\n", u4PicInfo >> 29, rPes.u4FrameType );
                }       // 5391

                rPes.u4BufStart = u4BufStart;
                rPes.u4BufEnd = u4BufEnd;
                rPes.u4Wp = u4Wp;
                rPes.u1Pidx = u1Pidx;
                rPes.u1Channel = u1Channel;
                rPes.u1DeviceId = prPidStruct->u1DeviceId;
                rPes.eMsgType = DMX_PES_MSG_TYPE_PES;

                if (!_DMX_SendPictureHeader(&rPes))
                {
                    LOG(3, "Fail to send picture!\n");
                }
            }
        }
        else
        {
            u4Rp = u4To;
        }

        if (u4Rp >= u4BufEnd)
        {
            u4Rp -= u4BufSize;
        }
    }

    // Check if picture number is correct
    if (u4PicNum != u4HwPicNum)
    {
        sprintf(buf, "Detected picture number: %u, expected: %u",
            u4HwPicNum, u4PicNum);
        _DmxPanic(buf);
    }

    // Update read pointer
    u4HeaderRp = _DMX_AdvanceAddr(u4HeaderRp, (INT32)u4HeaderSize, u4HeaderWp,
        u4HeaderBufStart, u4HeaderBufEnd);
    ASSERT(u4HeaderRp != 0);
    PID_S_W(u1Pidx, 14) = PHYSICAL(u4HeaderRp);
    prPidStruct->u4NextPic = 0;

    // Update picture read pointer
    _au4PicRp[u1Pidx] = u4Rp;

    return fgIsToDecoder;
}
#endif  // DMX_DEBUG_PICTURE_SEARCH


//-----------------------------------------------------------------------------
/** _DmxProcessVideoPesHeader
 *  Process PES header
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessVideoPesHeader(UINT8 u1Pidx, BOOL fgComplete)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4Wp, u4BufStart, u4BufEnd, u4HeaderSize, u4PicInfoAddr, i;
    UINT32 u4HeaderRp, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd;
    UINT32 u4HwPicNum, u4HeaderByteCount, u4FrontHeaderSize, u4NextPic;
    UINT32 u4HeaderOrgRp;
    UINT8 u1Channel = DMX_NULL_PIDX;
    BOOL fgIsToDecoder;
    static BOOL _fgSendEOSMessage = FALSE;

#ifdef SYNC_PES_HEADER
    UINT32 u4WpInfoAddr;
#endif

    // Check if TO-CDFIFO is enabled
    if ((PID_S_W(u1Pidx, 0) & (1 << 6)) == 0)
    {
        return FALSE;
    }

    //---------------------------------------------------------------
    // Get header information
    //---------------------------------------------------------------

#ifdef SYNC_PES_HEADER
    u4HeaderRp = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);     // 114
    u4HeaderRp = VIRTUAL(u4HeaderRp);

    // Temporarily remove the following check. FIXME
//    DMX_PANIC(u4HeaderRp == VIRTUAL(PID_S_W(u1Pidx, 14)));    // Double confirm
//    DMX_PANIC_RET(FALSE);
    if (u4HeaderRp != VIRTUAL(PID_S_W(u1Pidx, 14)))
    {
        LOG(3, "Pidx %u: uCode reports incorrect header RP 0x%08x (0x%08x)\n",
            u1Pidx, u4HeaderRp, VIRTUAL(PID_S_W(u1Pidx, 14)));
    }
#else
    u4HeaderRp = VIRTUAL(PID_S_W(u1Pidx, 14));
#endif

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;

    u4HeaderWp = VIRTUAL(PID_S_W(u1Pidx, 13));
    DMX_PANIC(_DMX_IsAligned(u4HeaderRp, PES_HEADER_ALIGNMENT));
    DMX_PANIC_RET(FALSE);
    u4HeaderBufStart = VIRTUAL(PID_S_W(u1Pidx, 10));
    u4HeaderBufEnd = VIRTUAL(PID_S_W(u1Pidx, 11) + 1);
    DMX_PANIC((u4HeaderRp >= u4HeaderBufStart) && (u4HeaderRp < u4HeaderBufEnd));
    DMX_PANIC_RET(FALSE);

    // Important: Read WP before the "header byte count" is extracted.
    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    u4FrontHeaderSize = _DMX_Align(u4HeaderByteCount, PES_HEADER_ALIGNMENT)
        + PES_HEADER_EXTRA_BYTES;
    u4HeaderSize = u4FrontHeaderSize + (u4HwPicNum * 4);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

#ifdef SYNC_PES_HEADER
    u4WpInfoAddr = _DMX_AdvanceAddr(u4HeaderRp, 4, u4HeaderWp,
        u4HeaderBufStart, u4HeaderBufEnd);

#ifdef CC_DMX_ERR_RECOVERY_TEST
    {
        EXTERN BOOL DMX_Test_Enable;
        if(DMX_Test_Enable)
        {
            u4WpInfoAddr = 0;
            DMX_Test_Enable = FALSE;
        }
    }
#endif  // CC_DMX_ERR_RECOVERY_TEST

    DMX_PANIC(u4WpInfoAddr != 0);
    DMX_PANIC_RET(FALSE);
    if(u4WpInfoAddr == 0)       // klocwork
    {
        return FALSE;
    }
    prPidStruct->u4Wp = VIRTUAL(*(UINT32*)u4WpInfoAddr);  // also updated in _DmxPesInt()
#endif

    u4Wp = VIRTUAL(prPidStruct->u4Wp);

    u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
    u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
    DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
    DMX_PANIC_RET(FALSE);

    //---------------------------------------------------------------
    // Process picture start code
    //---------------------------------------------------------------

    fgIsToDecoder = _DMX_IsToDecoder();

    // Update write pointer to decoder
    if (fgIsToDecoder)
    {
        VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));
        VERIFY(_DMX_UpdateVideoWritePointer(u1Channel, PHYSICAL(u4Wp)));
    }

    u4NextPic = prPidStruct->u4NextPic;
    DMX_PANIC(u4NextPic <= u4HwPicNum);
    DMX_PANIC_RET(FALSE);

    for (i = u4NextPic; i < u4HwPicNum; i++)
    {
        UINT32 u4PicInfo, u4Addr;

        u4PicInfoAddr = _DMX_AdvanceAddr(u4HeaderRp,
            (INT32)(u4FrontHeaderSize + (i * 4)),
            u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
        DMX_PANIC(u4PicInfoAddr != 0);
        DMX_PANIC_RET(FALSE);
        if(u4PicInfoAddr == 0)      // klocwork
        {
            return FALSE;
        }

        u4PicInfo = *(UINT32*)u4PicInfoAddr;
        u4Addr = VIRTUAL(u4PicInfo & 0x0fffffff);
        DMX_PANIC((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd));
        DMX_PANIC_RET(FALSE);

        if (fgIsToDecoder)
        {
            DMX_PES_HEADER_T rPesHeader;
            DMX_PES_MSG_T rPes;
            UINT32 u4FrameAddr;

            // Parse PES header to get PTS/DTS
            x_memset((void*)&rPes, 0, sizeof(rPes));
            if (i == 0)
            {
#ifdef ENABLE_MULTIMEDIA
                DMX_INPUT_TYPE_T eInputType;

                eInputType = _DMX_GetInputType();
                if(eInputType == DMX_IN_PLAYBACK_MM)
                {
                    _DMX_MM_SetHeader(u1Pidx, &rPes);
                    if (rPes.fgEOS)
                    {
                        rPes.fgEOS = FALSE;
                        _fgSendEOSMessage = TRUE;
                    }
                }
                else if (eInputType == DMX_IN_PLAYBACK_ES)
                {
                    // No PTS/DTS in ES.
                }
                else
#endif  // ENABLE_MULTIMEDIA
                {
                    if (_DmxParsePesHeader(u4HeaderRp, u4HeaderWp,
                        u4HeaderBufStart, u4HeaderBufEnd, prPidStruct,
                        &rPesHeader))
                    {
                        rPes.u4Pts = rPesHeader.u4Pts;
                        rPes.u4Dts = rPesHeader.u4Dts;
                        rPes.fgPtsDts = TRUE;
                    }
                }
            }

            // Prepare PES info
#if 1
            // This block of code is for supporting PS bit streams full of audio
            // PES but with scarce and concentrated (not scattered) video PES.
            UINT32 u4NewWp = 0;
            u4NewWp = u4Wp + 2;
            if (u4NewWp >= u4BufEnd)
            {
                u4NewWp = u4NewWp - (u4BufEnd - u4BufStart);
            }
            u4FrameAddr = _DMX_AdvanceAddr(u4Addr, -5, u4NewWp, u4BufStart,
                u4BufEnd);  // MT5391
#else
            u4FrameAddr = _DMX_AdvanceAddr(u4Addr, -5, u4Wp, u4BufStart,
                u4BufEnd);  // MT5391
#endif
            rPes.ePidType = DMX_PID_TYPE_ES_VIDEO;
            rPes.u4FrameType = _DmxDetectPictureType(u4FrameAddr,
                u4BufStart, u4BufEnd);
            rPes.u4FrameAddr = PHYSICAL(u4FrameAddr);
            rPes.u4BufStart = PHYSICAL(u4BufStart);
            rPes.u4BufEnd = PHYSICAL(u4BufEnd);
            rPes.u4Wp = PHYSICAL(u4Wp);
            rPes.u1Pidx = u1Pidx;
            rPes.u1Channel = u1Channel;
            rPes.u1DeviceId = prPidStruct->u1DeviceId;
            rPes.eMsgType = DMX_PES_MSG_TYPE_PES;

            // Send PES message to decoder
            if (!_DMX_SendPictureHeader(&rPes))
            {
                LOG(3, "Fail to send picture!\n");
            }
        }
    }

    if (fgComplete)
    {
        // Update header read pointer
        u4HeaderOrgRp = u4HeaderRp;
        u4HeaderRp = _DMX_AdvanceAddr(u4HeaderRp, (INT32)u4HeaderSize,
                            u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
        //DMX_PANIC(u4HeaderRp != 0);
        //DMX_PANIC_RET(FALSE);
        if(u4HeaderRp == 0)
        {
            PID_S_W(u1Pidx, 14) = PHYSICAL(u4HeaderOrgRp);
        }
        else
        {
            PID_S_W(u1Pidx, 14) = PHYSICAL(u4HeaderRp);
        }
        prPidStruct->u4NextPic = 0;
        if (_fgSendEOSMessage)
        {
            _fgSendEOSMessage = FALSE;
            if (!_DMX_SendEOSMessage(u1Pidx))
            {
                LOG(5, "Failed to send the EOS message!\n");
            }
        }
    }
    else
    {
        prPidStruct->u4NextPic = u4HwPicNum;
    }

    return fgIsToDecoder;
}


//-----------------------------------------------------------------------------
/** _DmxProcessVideoEs
 *  Process video ES
 *
 *  @param  u1PidIndex      PID index
 *  @param  fgComplete      Is packet complete
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessVideoEs(UINT8 u1PidIndex, BOOL fgComplete)
{
    BOOL fgProcessed;
#ifdef DMX_DEBUG_PICTURE_SEARCH
    if (_DMX_IsDebugPictureFinder())
    {
        fgProcessed = _DmxProcessVideoPesHeaderDebug(u1PidIndex);
    }
    else
#endif  // DMX_DEBUG_PICTURE_SEARCH
    {
        fgProcessed = _DmxProcessVideoPesHeader(u1PidIndex, fgComplete);
    }

    return fgProcessed;
}


//-----------------------------------------------------------------------------
/** _DmxProcessAudioEs
 *  Process audio ES
 *
 *  @param  u1Pidx          PID index
 *  @param  fgComplete      Is packet complete
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessAudioEs(UINT8 u1Pidx, BOOL fgComplete)
{
    DMX_PES_HEADER_T rPesHeader;
    UINT32 u4Wp, u4BufStart, u4BufEnd;
    UINT32 u4HeaderRp, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd;
    UINT32 u4HwPicNum, u4HeaderByteCount, u4HeaderSize;
    PID_STRUCT_T* prPidStruct;
    BOOL fgIsToDecoder;
    DMX_AUDIO_PES_T rPes;
    UINT32 u4ExtraAudioInfoLen = 0;
#ifdef ENABLE_MULTIMEDIA
    static BOOL _fgSendEOSMessage = FALSE;
#endif  // ENABLE_MULTIMEDIA

#ifdef SYNC_PES_HEADER
    UINT32 u4WpInfoAddr;
#endif

    // Check if TO-CDFIFO is enabled
    if ((PID_S_W(u1Pidx, 0) & (1 << 6)) == 0)
    {
        // TO-CDFIFO is disabled, stop here
        return FALSE;
    }

    //---------------------------------------------------------------
    // Get header information
    //---------------------------------------------------------------
#ifdef SYNC_PES_HEADER
    u4HeaderRp = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);     // 114
    u4HeaderRp = VIRTUAL(u4HeaderRp);

    // Temporarily remove the following check. FIXME
//    DMX_PANIC(u4HeaderRp == VIRTUAL(PID_S_W(u1Pidx, 14)));    // Double confirm
//    DMX_PANIC_RET(FALSE);
    if (u4HeaderRp != VIRTUAL(PID_S_W(u1Pidx, 14)))
    {
        LOG(3, "Pidx %u: uCode reports incorrect header RP 0x%08x (0x%08x)\n",
            u1Pidx, u4HeaderRp, VIRTUAL(PID_S_W(u1Pidx, 14)));
    }
#else
    u4HeaderRp = VIRTUAL(PID_S_W(u1Pidx, 14));
#endif

    u4HeaderWp = VIRTUAL(PID_S_W(u1Pidx, 13));
    DMX_PANIC(_DMX_IsAligned(u4HeaderRp, PES_HEADER_ALIGNMENT));
    DMX_PANIC_RET(FALSE);
    u4HeaderBufStart = VIRTUAL(PID_S_W(u1Pidx, 10));
    u4HeaderBufEnd = VIRTUAL(PID_S_W(u1Pidx, 11) + 1);
    DMX_PANIC((u4HeaderRp >= u4HeaderBufStart) && (u4HeaderRp < u4HeaderBufEnd));
    DMX_PANIC_RET(FALSE);

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;

    // Temporarily remove the following check. FIXME
//  DMX_PANIC(u4HwPicNum == 0);
//    DMX_PANIC_RET(FALSE);
    if (u4HwPicNum != 0)
    {
        LOG(1, "Error: non-zero (%u) picture number in audio channel (%u)\n",
            u4HwPicNum, u1Pidx);
    }

    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    u4HeaderSize = _DMX_Align(u4HeaderByteCount, PES_HEADER_ALIGNMENT) +
        PES_HEADER_EXTRA_BYTES;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    //---------------------------------------------------------------

#ifdef SYNC_PES_HEADER
    u4WpInfoAddr = _DMX_AdvanceAddr(u4HeaderRp, 4, u4HeaderWp,
        u4HeaderBufStart, u4HeaderBufEnd);
    DMX_PANIC(u4WpInfoAddr != 0);
    DMX_PANIC_RET(FALSE);
    if(u4WpInfoAddr == 0)       // klocwork
    {
        return FALSE;
    }
    prPidStruct->u4Wp = VIRTUAL(*(UINT32*)u4WpInfoAddr);
#endif

    u4Wp = VIRTUAL(prPidStruct->u4Wp);

    u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
    u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
    DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
    DMX_PANIC_RET(FALSE);

#ifdef ENABLE_MULTIMEDIA
    u4ExtraAudioInfoLen = _DmxGetExtraAuidoInfo(u1Pidx, u4HeaderRp,
        u4HeaderSize, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd, &rPes);
#endif  // ENABLE_MULTIMEDIA

    fgIsToDecoder = _DMX_IsToDecoder();

    if (fgIsToDecoder)
    {
        rPes.u1PidIndex = u1Pidx;
        rPes.u4Wp = PHYSICAL(u4Wp);
        rPes.u4PesRp = PHYSICAL(prPidStruct->u4PesRp);
        rPes.u1DeviceId = prPidStruct->u1DeviceId;
        rPes.u4Pts = 0;
        rPes.u4Dts = 0;
        rPes.fgEOS = FALSE;
        rPes.fgCopyright = FALSE;
        rPes.fgOriginal = FALSE;

#ifdef ENABLE_MULTIMEDIA
        if(_DMX_GetInputType() == DMX_IN_PLAYBACK_MM)
        {
            _DMX_MM_SetAudioHeader(u1Pidx, &rPes);
            if (rPes.fgEOS)
            {
                rPes.fgEOS = FALSE;
                _fgSendEOSMessage = TRUE;
            }
#ifdef CC_DMX_AUDIO_PREBUF
            VERIFY(_DMX_AudHandler_AddPes(&rPes));
#else
            VERIFY(_DMX_SendAudioPes(&rPes));
#endif  // CC_DMX_AUDIO_PREBUF
        }
        else
#endif  // ENABLE_MULTIMEDIA
        {
            if (_DmxParsePesHeader(u4HeaderRp, u4HeaderWp,
                u4HeaderBufStart, u4HeaderBufEnd, prPidStruct,
                &rPesHeader))
            {
                rPes.u4Pts = rPesHeader.u4Pts;
                rPes.u4Dts = rPesHeader.u4Dts;
                rPes.fgCopyright = rPesHeader.fgCopyright;
                rPes.fgOriginal = rPesHeader.fgOriginal;
            }

            VERIFY(_DMX_SendAudioPes(&rPes));
        }
    }

    if (fgComplete)
    {
        prPidStruct->rCounters.u4PesCount++;

        // Update header read pointer
        u4HeaderRp = _DMX_AdvanceAddr(u4HeaderRp,
            (INT32)(u4HeaderSize + u4ExtraAudioInfoLen), u4HeaderWp,
            u4HeaderBufStart, u4HeaderBufEnd);
        DMX_PANIC(u4HeaderRp != 0);
        DMX_PANIC_RET(FALSE);
        PID_S_W(u1Pidx, 14) = PHYSICAL(u4HeaderRp);
        prPidStruct->u4NextPic = 0;
#ifdef ENABLE_MULTIMEDIA
        if (_fgSendEOSMessage)
        {
            _fgSendEOSMessage = FALSE;
            x_memset((void*)&rPes, 0, sizeof(rPes));
            rPes.fgEOS = TRUE;
            rPes.u1PidIndex = u1Pidx;
            rPes.u1DeviceId = prPidStruct->u1DeviceId;
            if(_DMX_GetInputType() == DMX_IN_PLAYBACK_MM)
            {
#ifdef CC_DMX_AUDIO_PREBUF
                VERIFY(_DMX_AudHandler_AddPes(&rPes));
#else
                VERIFY(_DMX_SendAudioPes(&rPes));
#endif  // CC_DMX_AUDIO_PREBUF
            }
            else
            {
                VERIFY(_DMX_SendAudioPes(&rPes));
            }
        }
#endif  // ENABLE_MULTIMEDIA
        // Debug
//      LOG(7, "Pidx %u payload size: %u\n", u1Pidx, rPesHeader.u4PayloadSize);
    }

    prPidStruct->u4PesRp = u4Wp;

    // Suppress warnings in lint and release builds
    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);

    return fgIsToDecoder;
}


//-----------------------------------------------------------------------------
/** _DmxProcessVideoClipEs
 *  Process video clip ES's
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessVideoClipEs(UINT8 u1Pidx)
{
    UINT32 u4HeaderRp, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd;
    UINT32 u4HwPicNum, u4HeaderByteCount, u4HeaderSize;
    PID_STRUCT_T* prPidStruct;
    BOOL fgProcessed = FALSE;

    UINT32 u4WpInfoAddr;

    //---------------------------------------------------------------
    // Get header information
    //---------------------------------------------------------------
    u4HeaderRp = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);     // 114
    u4HeaderRp = VIRTUAL(u4HeaderRp);
    DMX_PANIC(u4HeaderRp == VIRTUAL(PID_S_W(u1Pidx, 14)));    // Double confirm
    DMX_PANIC_RET(FALSE);

    u4HeaderWp = VIRTUAL(PID_S_W(u1Pidx, 13));
    DMX_PANIC(_DMX_IsAligned(u4HeaderRp, PES_HEADER_ALIGNMENT));
    DMX_PANIC_RET(FALSE);
    u4HeaderBufStart = VIRTUAL(PID_S_W(u1Pidx, 10));
    u4HeaderBufEnd = VIRTUAL(PID_S_W(u1Pidx, 11) + 1);
    DMX_PANIC((u4HeaderRp >= u4HeaderBufStart) && (u4HeaderRp < u4HeaderBufEnd));
    DMX_PANIC_RET(FALSE);

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;
//  DMX_PANIC(u4HwPicNum == 0);
//    DMX_PANIC_RET(FALSE);
    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    u4HeaderSize = _DMX_Align(u4HeaderByteCount, PES_HEADER_ALIGNMENT) +
        PES_HEADER_EXTRA_BYTES + (u4HwPicNum * 4);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_PES_HEADER_T rPesHeader;
        DMX_NOTIFY_INFO_ES_T rEs;
        UINT32 u4Rp, u4Wp, u4BufStart, u4BufEnd, u4DataSize;

        u4WpInfoAddr = _DMX_AdvanceAddr(u4HeaderRp, 4, u4HeaderWp,
            u4HeaderBufStart, u4HeaderBufEnd);
        DMX_PANIC(u4WpInfoAddr != 0);
        DMX_PANIC_RET(FALSE);
        if(u4WpInfoAddr == 0)       // klocwork
        {
            return FALSE;
        }
        prPidStruct->u4Wp = *(UINT32*)u4WpInfoAddr;

        u4Wp = VIRTUAL(prPidStruct->u4Wp);
        u4Rp = VIRTUAL(prPidStruct->u4Rp);
        u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
        u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
        DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
        DMX_PANIC_RET(FALSE);
        DMX_PANIC((u4Rp >= u4BufStart) && (u4Rp < u4BufEnd));
        DMX_PANIC_RET(FALSE);
        u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufEnd - u4BufStart);

        rEs.u4DataAddr = u4Rp;
        rEs.u4DataSize = u4DataSize;
        rEs.u4Pts = 0;
        rEs.u4Dts = 0;

        if (_DmxParsePesHeader(u4HeaderRp, u4HeaderWp, u4HeaderBufStart,
            u4HeaderBufEnd, prPidStruct, &rPesHeader))
        {
            rEs.u4Pts = rPesHeader.u4Pts;
            rEs.u4Dts = rPesHeader.u4Dts;
        }

        fgProcessed = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_ES,
            (UINT32)&rEs, prPidStruct->pvNotifyTag);
    }   // if

    // Update header read pointer
    u4HeaderRp = _DMX_AdvanceAddr(u4HeaderRp, (INT32)u4HeaderSize, u4HeaderWp,
        u4HeaderBufStart, u4HeaderBufEnd);
    DMX_PANIC(u4HeaderRp != 0);
    DMX_PANIC_RET(FALSE);
    PID_S_W(u1Pidx, 14) = PHYSICAL(u4HeaderRp);
    prPidStruct->u4NextPic = 0;

    // Suppress warnings in lint and release builds
    UNUSED(u4HwPicNum);

    return fgProcessed;
}


//-----------------------------------------------------------------------------
/** _DmxProcessOtherEs
 *  Process other ES's
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessOtherEs(UINT8 u1Pidx)
{
    UINT32 u4HeaderRp, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd;
    UINT32 u4HwPicNum, u4HeaderByteCount, u4HeaderSize;
    PID_STRUCT_T* prPidStruct;
    BOOL fgProcessed = FALSE;

#ifdef SYNC_PES_HEADER
    UINT32 u4WpInfoAddr;
#endif

    //---------------------------------------------------------------
    // Get header information
    //---------------------------------------------------------------
#ifdef SYNC_PES_HEADER
    u4HeaderRp = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);     // 114
    u4HeaderRp = VIRTUAL(u4HeaderRp);
    DMX_PANIC(u4HeaderRp == VIRTUAL(PID_S_W(u1Pidx, 14)));    // Double confirm
    DMX_PANIC_RET(FALSE);
#else
    u4HeaderRp = VIRTUAL(PID_S_W(u1Pidx, 14));
#endif

    u4HeaderWp = VIRTUAL(PID_S_W(u1Pidx, 13));
    DMX_PANIC(_DMX_IsAligned(u4HeaderRp, PES_HEADER_ALIGNMENT));
    DMX_PANIC_RET(FALSE);
    u4HeaderBufStart = VIRTUAL(PID_S_W(u1Pidx, 10));
    u4HeaderBufEnd = VIRTUAL(PID_S_W(u1Pidx, 11) + 1);
    DMX_PANIC((u4HeaderRp >= u4HeaderBufStart) && (u4HeaderRp < u4HeaderBufEnd));
    DMX_PANIC_RET(FALSE);

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;
    DMX_PANIC(u4HwPicNum == 0);
    DMX_PANIC_RET(FALSE);
    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    u4HeaderSize = _DMX_Align(u4HeaderByteCount, PES_HEADER_ALIGNMENT) +
        PES_HEADER_EXTRA_BYTES;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_PES_HEADER_T rPesHeader;
        DMX_NOTIFY_INFO_ES_T rEs;
        UINT32 u4Rp, u4Wp, u4BufStart, u4BufEnd, u4DataSize;

#ifdef SYNC_PES_HEADER
        u4WpInfoAddr = _DMX_AdvanceAddr(u4HeaderRp, 4, u4HeaderWp,
            u4HeaderBufStart, u4HeaderBufEnd);
        DMX_PANIC(u4WpInfoAddr != 0);
        DMX_PANIC_RET(FALSE);
        if(u4WpInfoAddr == 0)       // klocwork
        {
            return FALSE;
        }
        prPidStruct->u4Wp = VIRTUAL(*(UINT32*)u4WpInfoAddr);
#endif

        u4Wp = VIRTUAL(prPidStruct->u4Wp);
        u4Rp = VIRTUAL(prPidStruct->u4Rp);
        u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
        u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
        DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
        DMX_PANIC_RET(FALSE);
        DMX_PANIC((u4Rp >= u4BufStart) && (u4Rp < u4BufEnd));
        DMX_PANIC_RET(FALSE);
        u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufEnd - u4BufStart);

        rEs.u4DataAddr = u4Rp;
        rEs.u4DataSize = u4DataSize;
        rEs.u4Pts = 0;
        rEs.u4Dts = 0;

        if (_DmxParsePesHeader(u4HeaderRp, u4HeaderWp, u4HeaderBufStart,
            u4HeaderBufEnd, prPidStruct, &rPesHeader))
        {
            rEs.u4Pts = rPesHeader.u4Pts;
            rEs.u4Dts = rPesHeader.u4Dts;
        }

        fgProcessed = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_ES,
            (UINT32)&rEs, prPidStruct->pvNotifyTag);
    }   // if

    // Update header read pointer
    u4HeaderRp = _DMX_AdvanceAddr(u4HeaderRp, (INT32)u4HeaderSize, u4HeaderWp,
        u4HeaderBufStart, u4HeaderBufEnd);
    DMX_PANIC(u4HeaderRp != 0);
    DMX_PANIC_RET(FALSE);
    PID_S_W(u1Pidx, 14) = PHYSICAL(u4HeaderRp);
    prPidStruct->u4NextPic = 0;

    // Suppress warnings in lint and release builds
    UNUSED(u4HwPicNum);

    return fgProcessed;
}


//-----------------------------------------------------------------------------
/** _DmxProcessPes
 *  Process PES's
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessPes(UINT8 u1Pidx)
{
    UINT32 u4BufStart, u4BufEnd, u4PesRp, u4Wp;
    PID_STRUCT_T* prPidStruct;
    BOOL fgProcessed = FALSE;
    UINT32 u4DataSize;
    UINT8 au1Data[6];
    UINT32 u4PESLen;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
    u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
    u4PesRp = VIRTUAL(prPidStruct->u4PesRp);
    DMX_PANIC((u4PesRp >= u4BufStart) && (u4PesRp < u4BufEnd));
    DMX_PANIC_RET(FALSE);
    u4Wp = VIRTUAL(prPidStruct->u4Wp);
    DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
    DMX_PANIC_RET(FALSE);

    u4DataSize = DATASIZE(u4PesRp, u4Wp, u4BufEnd - u4BufStart);

    if(_DMX_GetInputType() == DMX_IN_BROADCAST_TS)
    {
        x_memset((void*)au1Data, 0, sizeof(au1Data));
        if(_DMX_CopyRingBuffer((UINT32)au1Data, (UINT32)au1Data,
            (UINT32)(au1Data + 6), u4PesRp, u4BufStart, u4BufEnd, 6) != (UINT32)au1Data)
        {
            LOG(3, "PES copy error\n");
        }

        if( (au1Data[0]!=0x00) || (au1Data[1]!=0x00) || (au1Data[2]!=0x01) )
        {
            LOG(3, "PES HEADER Mismatch\n");
            //DMX_PANIC(0);
            //DMX_PANIC_RET(FALSE);
        }

        u4PESLen = (au1Data[4] << 8) | au1Data[5];
        u4PESLen += 6;
        if(u4DataSize != u4PESLen)
        {
            LOG(3, "PES SIZE Mismatch, datalen: 0x%x, peslen: 0x%x \n", u4DataSize, u4PESLen);
            //DMX_PANIC(0);
            //DMX_PANIC_RET(FALSE);
        }
    }

    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_NOTIFY_INFO_PES_T rPes;

        rPes.u4DataAddr = u4PesRp;
        rPes.u4DataSize = u4DataSize;
        rPes.u1SerialNumber = prPidStruct->u1SerialNumber;

        fgProcessed = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_PES,
            (UINT32)&rPes, prPidStruct->pvNotifyTag);
    }   // if

    prPidStruct->u4PesRp = u4Wp;

    return fgProcessed;
}

//-----------------------------------------------------------------------------
/** _DmxProcessDTCP
 *  Process DTCP data
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessDTCP(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;
    BOOL fgProcessed = FALSE;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    prPidStruct->u4Wp = VIRTUAL(PID_S_W(u1Pidx, 8));

    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_NOTIFY_INFO_PES_T rPes;

        rPes.u4DataAddr = 0;
        rPes.u4DataSize = 0;
        rPes.u1SerialNumber = 0;

        fgProcessed = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_PES,
            (UINT32)&rPes, prPidStruct->pvNotifyTag);
    }   // if

    return fgProcessed;
}


//-----------------------------------------------------------------------------
/** _DmxProcessScrambleChange
 *  Process scramble change notification
 *
 *  @param  u1Pidx          PID index
 *  @param  fgScrambled     The new state is scrambled or clear
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxProcessScrambleChange(UINT8 u1Pidx, BOOL fgScrambled)
{
    PID_STRUCT_T* prPidStruct;
    DMX_SCRAMBLE_STATE_T eState;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    if (fgScrambled)
    {
        eState = DMX_SCRAMBLE_STATE_SCRAMBLED;
    }
    else
    {
        eState = DMX_SCRAMBLE_STATE_CLEAR;
    }

    LOG(6, "Pidx %u scrambling state: %s\n", u1Pidx,
        fgScrambled ? "SCRAMBLED" : "CLEAR");

    // Todo: issue notification only if fgScrambled consists with the scramble
    // state in PID memory
    //
    if (prPidStruct->pfnScramble != NULL)
    {
        BOOL fgRet;

        fgRet = prPidStruct->pfnScramble(u1Pidx,
            DMX_NOTIFY_CODE_SCRAMBLE_STATE, (UINT32)eState,
            prPidStruct->pvScrambleTag);

        UNUSED(fgRet);
    }
}


//-----------------------------------------------------------------------------
/** _DmxDispatchPesHandler
 *  Dispatch PES interrupt to real handlers
 *
 *  @param  u1PidIndex      PID index
 *  @param  u1PesCount      PES count
 *  @param  fgComplete      Is PES packet complete
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxDispatchPesHandler(UINT8 u1PidIndex, UINT8 u1PesCount,
    BOOL fgComplete)
{
    PID_STRUCT_T* prPidStruct;
    BOOL fgProcessed = FALSE;

    HalFlushInvalidateDCache();

    prPidStruct = _DMX_GetPidStruct(u1PidIndex);
    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
        fgProcessed = _DmxProcessVideoEs(u1PidIndex, fgComplete);
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        fgProcessed = _DmxProcessAudioEs(u1PidIndex, fgComplete);
        break;

    case DMX_PID_TYPE_ES_VIDEOCLIP:
        fgProcessed = _DmxProcessVideoClipEs(u1PidIndex);
        break;

    case DMX_PID_TYPE_ES_OTHER:
        fgProcessed = _DmxProcessOtherEs(u1PidIndex);
        break;

    case DMX_PID_TYPE_PES:
        fgProcessed = _DmxProcessPes(u1PidIndex);
        break;

    case DMX_PID_TYPE_ES_DTCP:
        fgProcessed = _DmxProcessDTCP(u1PidIndex);
        break;

    case DMX_PID_TYPE_NONE:
        break;

    case DMX_PID_TYPE_PSI:
    default:
        DMX_PANIC(0);
        DMX_PANIC_RET_VOID();
        break;
    }

    if (!fgProcessed)
    {
#ifndef CC_MINI_DRIVER
        // Data not processed, should be consumed here to avoid FIFO full
        if (_DMX_IDE_IsEnabled())
        {
            // Send and consume data
            _DmxSendToIde(u1PidIndex);
        }
        else
#endif  // CC_MINI_DRIVER
        {
            // Drop data by set RP to WP directly
            PID_S_W(u1PidIndex, 9) = PHYSICAL(prPidStruct->u4Wp);
            prPidStruct->u4Rp = prPidStruct->u4Wp;
        }
    }

    UNUSED(u1PesCount);
}


//-----------------------------------------------------------------------------
/** _DmxPesInt
 *  Interrupt handler for PES data
 *
 *  @param  fgComplete      Is PES packet complete
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxPesInt(BOOL fgComplete)
{
    UINT32 u4Status;
    UINT8 u1Type, u1PesCount, u1Pidx, u1PendInt;
    UINT32 u4EndAddr;
    PID_STRUCT_T* prPidStruct;
    UINT32 u4CurBufFullness;

    u4Status = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG1);      // 113
    u4EndAddr = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);     // 114
    u4EndAddr = VIRTUAL(u4EndAddr);

    u1Type = GET_BYTE(u4Status, 0);         // 0: none, 1: PES, 2: PSI
    u1PesCount = GET_BYTE(u4Status, 1);     // PES count
    u1Pidx = GET_BYTE(u4Status, 2);         // PID index
    u1PendInt = GET_BYTE(u4Status, 3);

    DMX_PANIC((fgComplete && ((u1Type == 1) || (u1Type == 5))) ||
            (!fgComplete && (u1Type == 3)));
    DMX_PANIC_RET_VOID();
    DMX_PANIC(u1Pidx < DMX_NUM_PID_INDEX);
    DMX_PANIC_RET_VOID();
    UNUSED(u1PendInt);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    // Dispatch to real handlers
    prPidStruct->u4Wp = u4EndAddr;
    _DmxDispatchPesHandler(u1Pidx, u1PesCount, fgComplete);

#ifdef ENABLE_MULTIMEDIA
    if (_DMX_GetInputType() == DMX_IN_PLAYBACK_MM)
    {
        if (fgComplete && (u1Type == 5))
        {
            _DMX_MM_ISRHandler(u1Pidx);
        }
    }
#endif  // ENABLE_MULTIMEDIA

    //store peak FIFO fullness while Wp is updated
    u4CurBufFullness = DATASIZE(prPidStruct->u4Rp, prPidStruct->u4Wp,
        prPidStruct->u4BufLen);
    if(prPidStruct->u4PeakBufFull < u4CurBufFullness)
    {
        prPidStruct->u4PeakBufFull = u4CurBufFullness;
    }

    if (fgComplete)
    {
        // Debug info
        prPidStruct->rCounters.u4PesCount += u1PesCount;
        LOG(7, "INT: PES %u, pidx 0x%x, pendINT %u, WP 0x%08x, PESCount: %u\n",
            u1PesCount, u1Pidx, u1PendInt, u4EndAddr,
            prPidStruct->rCounters.u4PesCount);
    }
}


//-----------------------------------------------------------------------------
/** _DmxPsiInt
 *  Interrupt handler of PSI
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxPsiInt(void)
{
    UINT8 u1Type, u1SecCount, u1Pidx, u1PendInt;
    UINT32 u4Status, u4EndAddr;
    DMX_PSI_MSG_T rMsg;

    u4Status = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG1);      // 113
    u4EndAddr = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);     // 114
    u4EndAddr = VIRTUAL(u4EndAddr);     // end address of current section + 1,
                                        // i.e. current write pointer
    u1Type = GET_BYTE(u4Status, 0);         // 0: none, 1: PES, 2: PSI
    u1SecCount = GET_BYTE(u4Status, 1);     // PSI section count
    u1Pidx = GET_BYTE(u4Status, 2);         // PID index
    u1PendInt = GET_BYTE(u4Status, 3);      // Pending interrupts

    DMX_PANIC(u1Type == 2);
    DMX_PANIC_RET_VOID();
    DMX_PANIC(u1Pidx < DMX_NUM_PID_INDEX);
    DMX_PANIC_RET_VOID();

    rMsg.u1Pidx = u1Pidx;
    rMsg.u1SecCount = u1SecCount;
    rMsg.u4EndAddr = u4EndAddr;
    if(!_DMX_AddPSI(&rMsg))
    {
        LOG(3," Add section fail\n");
    }

    UNUSED(u1Type);
    UNUSED(u1PendInt);
}


#ifndef CC_MINI_DRIVER


//-----------------------------------------------------------------------------
/** _DmxDescramblerInt
 *  Interrupt handler of descrambler
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxDescramblerInt(void)
{
    UINT32 u4Status;
    UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Wp, u4Rp, u4DataSize;

    u4Status = DMXCMD_READ32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG);

    if (u4Status & (1 << 0))            // Output buffer interrupt
    {
        DMX_STATES_T* prStates;

        u4BufStart = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_START);
        u4BufEnd = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_END) + 1;
        u4BufSize = u4BufEnd - u4BufStart;
        u4Wp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_WP);
        u4Rp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_RP);
        u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

        DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
        DMX_PANIC_RET_VOID();
        DMX_PANIC((u4Rp >= u4BufStart) && (u4Rp < u4BufEnd));
        DMX_PANIC_RET_VOID();

        prStates = _DMX_GetStates();
        if (prStates->rCaptureSettings.pfnHandler != NULL)
        {
            // Notify capture handler
            DMX_CAPTURE_INFO_T rInfo;
            BOOL fgRet;

            rInfo.u4BufStart = u4BufStart;
            rInfo.u4BufEnd = u4BufEnd;
            rInfo.u4Rp = u4Rp;
            rInfo.u4Wp = u4Wp;
            rInfo.u4DataSize = u4DataSize;

            // Callback to handler
            fgRet = prStates->rCaptureSettings.pfnHandler(
                DMX_CAPTURE_NOTIFY_CODE_RECEIVE_DATA, &rInfo,
                prStates->rCaptureSettings.pvTag);
            UNUSED(fgRet);
        }
        else
        {
#if 1
            // No handler, discard certain data to let buffer half empty
            if (u4DataSize > (u4BufSize / 2))
            {
                u4Rp = u4Wp - (u4BufSize / 2);
                if (u4Rp < u4BufStart)
                {
                    u4Rp += u4BufSize;
                }
                DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_RP, u4Rp);
            }
#else
            // Debug, update write pointer to input buffer
            DMXCMD_WRITE32(DMX_REG_CA_IN_BUF_WP, u4Wp);
            LOG(6, "Update CA input WP: 0x%08x\n", u4Wp);
#endif
        }
    }

    if (u4Status & (1 << 1))
    {
        // Input buffer interrupt
        // Write pointer should be updated according to incoming data (from
        // output buffer?). Nothing to do here.
        u4BufStart = DMXCMD_READ32(DMX_REG_CA_IN_BUF_START);
        u4BufEnd = DMXCMD_READ32(DMX_REG_CA_IN_BUF_END) + 1;
        u4BufSize = u4BufEnd - u4BufStart;
        u4Wp = DMXCMD_READ32(DMX_REG_CA_IN_BUF_WP);
        u4Rp = DMXCMD_READ32(DMX_REG_CA_IN_BUF_RP);
        u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
#if 1
        // No handler, discard data to let buffer half full
        if (u4DataSize > (u4BufSize / 2))
        {
            u4Rp = u4Wp - (u4BufSize / 2);
            if (u4Rp < u4BufStart)
            {
                u4Rp += u4BufSize;
            }
            DMXCMD_WRITE32(DMX_REG_CA_IN_BUF_RP, u4Rp);
        }
#else
        // Debug, update read pointer to output buffer
        DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_RP, u4Rp);
        LOG(6, "Update CA output RP: 0x%08x\n", u4Rp);
#endif
    }

    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
}


//-----------------------------------------------------------------------------
/** _DmxPvrInt
 *  Interrupt handler of PVR
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxPvrInt(void)
{
#ifdef ENABLE_MULTIMEDIA
    UINT32 u4Status;

    u4Status = DMXCMD_READ32(DMX_REG_PVR_NONERR_STATUS_REG);

    if (u4Status & (1 << 1))            // Input buffer interrupt
    {
        _DMX_PVRPlay_Notify();
    }
#endif  // ENABLE_MULTIMEDIA
    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_PVR_NONERR_STATUS_REG, 1);
}


#endif  // CC_MINI_DRIVER


#ifndef __PCR_RECOVERY__

//-----------------------------------------------------------------------------
/** _DmxPcrInt
 *  Interrupt handler of PCR
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxPcrInt(UINT32 u4Status)
{
    static UINT32 _u4PcrIntCount = 0;
    UINT32 u4Status1, u4Status2;
    UINT32 u4PcrBase0Ext = 0, u4PctBase32 = 0;
    UINT8 u1Pidx;

    u4Status1 = DMXCMD_READ32(DMX_REG_PCR_NONERR_STATUS_REG1);      // 101
    u4Status2 = DMXCMD_READ32(DMX_REG_PCR_NONERR_STATUS_REG2);      // 102

    LOG(7, "STC 0x%08x 0x%08x\n", DMXCMD_READ32(62), DMXCMD_READ32(61));

    u4PcrBase0Ext = DMXCMD_READ32(DMX_REG_PCR_EXTENSION);           // 59
    u4PcrBase32 = DMXCMD_READ32(DMX_REG_PCR_BASE);                  // 60

    if (u4Status1 & 0x2)
    {
        // Update STC
        DMXCMD_WRITE32(DMX_REG_STC_EXTENSION, u4PcrBase0Ext);       // 61
        DMXCMD_WRITE32(DMX_REG_STC_BASE, u4PcrBase32);              // 62

        // Update threshold to nonzero
        DMXCMD_WRITE32(55, 0xffff);     // 16 bits
        LOG(7, "l\n");
    }

    if (u4Status1 & 0x1)
    {
        LOG(7, "t 0x%04x\n", u4Status2 >>16);
    }

    _u4PcrIntCount++;

    LOG(7, "PCR INT pidx %d PCR 0x%08x 0x%08x, PCRCount: %d\n",
        u1Pidx, u4PcrBase32, u4PcrBase0Ext, _u4PcrIntCount);

    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
    DMXCMD_WRITE32(DMX_REG_PCR2_NONERR_STATUS_REG1, 1);
}

#endif  // __PCR_RECOVERY__


//-----------------------------------------------------------------------------
/** _DmxPreparsingInt
 *
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPreparsingInt(UINT8 u1Pidx)
{
    UINT32 u4BufStart, u4BufEnd, u4Rp, u4Wp, u4BufSize, u4DataSize;
    PID_STRUCT_T* prPidStruct;

    // The caller of this function shall have checked the validity of u1Pidx.

    // Note: u4BufStart, u4BufEnd, u4Rp, and u4Wp store physical addresses.
    u4BufStart = PID_S_W(u1Pidx, 5);
    u4BufEnd   = PID_S_W(u1Pidx, 6);    // address: 8N - 1
    u4Rp       = PID_S_W(u1Pidx, 9);
    // u4Wp       = PID_S_W(u1Pidx, 8);  // Do NOT use WP, use #114 instead.
    u4Wp = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);  // 114

    DMX_PANIC(u4BufStart < u4BufEnd);
    DMX_PANIC_RET(FALSE);
    DMX_PANIC((u4BufStart <= u4Rp) && (u4Rp <= u4BufEnd));
    DMX_PANIC_RET(FALSE);
    DMX_PANIC((u4BufStart <= u4Wp) && (u4Wp <= u4BufEnd));
    DMX_PANIC_RET(FALSE);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_NOTIFY_INFO_STREAM_ID_T rInfo;

        // Get the number of pairs of {SID,SSID}.
        // UINT32 u4Entries = PID_S_W(u1Pidx, 2) & 0xFFFF;

        u4BufSize = (u4BufEnd - u4BufStart) + 1;        // 8N
        u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

        rInfo.u4DataAddr = VIRTUAL(u4Rp);
        rInfo.u4DataSize = u4DataSize;

        if(!prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_STREAM_ID,
                                (UINT32)&rInfo, (const void*)NULL))
        {
            LOG(3, "_DmxPreparsingInt notify error\n");
        }
    }

    // Copy the value in WP to RP.
    prPidStruct->u4Wp  = VIRTUAL(u4Wp);
    prPidStruct->u4Rp  = VIRTUAL(u4Wp);
    PID_S_W(u1Pidx, 9) = u4Wp;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxFifoFull
 *  FIFO full handling
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxFifoFull(UINT8 u1Pidx)
{
    DMX_PID_T rPid;
    UINT32 u4BufStart, u4BufEnd, u4Wp, u4Rp, u4BufSize, u4DataSize, u4Threshold, u4FullGap;
    PID_STRUCT_T* prPidStruct;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    // Check data size
    u4BufStart = PID_S_W(u1Pidx, 5);
    u4BufEnd = PID_S_W(u1Pidx, 6);
    u4Wp = PID_S_W(u1Pidx, 8);
    u4Rp = PID_S_W(u1Pidx, 9);
    u4BufSize = (u4BufEnd - u4BufStart) + 1;
    u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

    u4Threshold = PID_S_W(u1Pidx, 15);

    LOG(3, "Pidx %u fifo full! buf: %u, data: %u\n", u1Pidx,
        u4BufSize, u4DataSize);

    u4FullGap = ((u4BufSize / 8) * 7);
    if (u4DataSize < u4FullGap)
    {
        if((u4Threshold == 0) || ((u4Threshold * 188) > (u4BufSize - u4FullGap)))
        {
            LOG(5, "Pidx %u: FIFO full event, reset PID!\n", u1Pidx);
        }
        else
        {
            LOG(5, "Pidx %u: False FIFO full event, not reset PID!\n", u1Pidx);
            return;
        }
    }

    // Send notification if user handler is installed
    if (prPidStruct->pfnNotify != NULL)
    {
        UNUSED(prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_OVERFLOW,
            u4DataSize, prPidStruct->pvNotifyTag));
    }

    if (prPidStruct->ePidType != DMX_PID_TYPE_ES_VIDEO)
    {
        // Do not handle FIFO full other than video ES
        return;
    }

    // Disable PID
    rPid.fgEnable = FALSE;
    VERIFY(_DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid, FALSE));

    // Send full message to thread
    UNUSED(_DMX_SendPurgeMessage(u1Pidx, TRUE));
}


//-----------------------------------------------------------------------------
/** _DmxFTuPErrorInt
 *  Error handler of uP
 *
 *  @retval BOOL : return value means handle or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxFTuPErrorInt(void)
{
    UINT32 u4Status;
    UINT8 u1Pidx, u1Type;

    // Get interrupt status
    u4Status = DMXCMD_READ32(DMX_REG_FTuP_ERROR_STATUS_REG);

    u1Pidx = (UINT8)((u4Status >> 16) & 0xff);
    u1Type = (UINT8)(u4Status & 0xff);

#ifdef ENABLE_MULTIMEDIA
    if((u1Pidx >= DMX_NUM_PID_INDEX) && (u1Pidx < DMX_NUM_ALL_PID_INDEX))
    {
        // FVR record uP error
        return FALSE;
    }
#endif  // ENABLE_MULTIMEDIA

    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);

    if (u1Pidx < DMX_NUM_PID_INDEX)
    {
        PID_STRUCT_T* prPidStruct;
        BOOL fgEnabled;

        prPidStruct = _DMX_GetPidStruct(u1Pidx);
        fgEnabled = _DMX_IsPidEnabled(u1Pidx);

        if (!fgEnabled)
        {
            // Error interrupt on a disabled pid, caused by race condition
            // between disabling pid and raising interrupt.
            // Simply do nothing here
            LOG(5, "Interrupt on disabled: pidx %u, status 0x%08x\n",
                u1Pidx, u4Status);

            return TRUE;
        }

        prPidStruct->rCounters.u4FTuPErrors++;
        prPidStruct->rCounters.u4TotalErrors++;
    }
    else
    {
        LOG(3, "FTuP error: invalid pid index %u\n", u1Pidx);
    }

    switch (u1Type)
    {
    case 1:
        // TS packet dropped due to lack of output buffer space.
        // Check FIFO full
        _DmxFifoFull(u1Pidx);
        break;

    case 101:
        // uCode fatal error, PES header is missing
        LOG(1, "Pidx: %u: uCode fatal error - PES header missing!\n");
        break;

    case 255:
        LOG(1, "Pidx %u:, Error 255!\n", u1Pidx);
        break;

    default:
        break;
    }

    LOG(6, "uP error happen, INT = 0x%x !!!!!! \n", u4Status);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxDbmErrorInt
 *  Error handler of DBM
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxDbmErrorInt(void)
{
    UINT32 u4Status, u4Reg;
    UINT16 u2Pid;
    UINT8 u1Pidx;

    u4Status = DMXCMD_READ32(DMX_REG_DBM_ERROR_STATUS_REG);
    switch(u4Status & 0xFF)
    {
    case 1:
    case 2:
    case 4:
    case 32:
        break;

    case 64:
    case 128:
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
        u4Reg |= 0xFF00;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
        u4Reg &= 0xFFFF00FF;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
        break;
    default:
        LOG(3, "Unknown DBM error status (0x%02x)!\n", u4Status & 0xFF);
        break;
    }

    u2Pid = (UINT16)((u4Status >> 16) & 0x1fff);
    if (_DMX_GetPidIndex(u2Pid, &u1Pidx))
    {
        PID_STRUCT_T* prPidStruct;

        prPidStruct = _DMX_GetPidStruct(u1Pidx);
        prPidStruct->rCounters.u4DbmErrors++;
        prPidStruct->rCounters.u4TotalErrors++;
    }
    else
    {
        LOG(3, "DBM error: Invalid PID %u\n", u2Pid);
    }

    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);

    LOG(3, "DBM error! INT: 0x%08x\n", u4Status);
}


//-----------------------------------------------------------------------------
/** _DmxDescramblerErrorInt
 *  Error handler of descrambler
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxDescramblerErrorInt(void)
{
    UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Rp, u4Wp, u4Status;
    UINT8 u1Pidx;

    // Let buffer be half-full
    u4BufStart = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_START);
    u4BufEnd = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_END);
    u4BufSize = (u4BufEnd - u4BufStart) + 1;
    u4Wp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_WP);
    u4Rp = u4Wp - (u4BufSize / 2);
    if (u4Rp < u4BufStart)
    {
        u4Rp += u4BufSize;
    }

    // Disable output buffer
    DMXCMD_REG32(DMX_REG_CA_CTRL) &= ~((1 << 7) | (1 << 15));

    // Re-initialized output buffer
    DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_START, u4BufStart);
    DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_END, u4BufEnd);
    DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_RP, u4Rp);
    DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_WP, u4Wp);

    // Re-enable output buffer
    DMXCMD_REG32(DMX_REG_CA_CTRL) |= (1 << 7) | (1 << 15);

    u4Status = DMXCMD_READ32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG);
    u1Pidx = (UINT8)((u4Status >> 16) & 0xff);
    if (u1Pidx < DMX_NUM_PID_INDEX)
    {
        PID_STRUCT_T* prPidStruct;

        prPidStruct = _DMX_GetPidStruct(u1Pidx);
        prPidStruct->rCounters.u4DescErrors++;
        prPidStruct->rCounters.u4TotalErrors++;
    }
    else
    {
        LOG(3, "Descrambler error: invalid pid index %u\n", u1Pidx);
    }

    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);

    LOG(3, "Descrambler error! INT: 0x%08x\n", u4Status);
}


//-----------------------------------------------------------------------------
/** _DmxPvrErrorInt
 *  Error handler of PVR
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxPvrErrorInt(void)
{
    UINT32 u4Status;
    UINT16 u2Pid;

    u4Status = DMXCMD_READ32(DMX_REG_PVR_ERROR_STATUS_REG);
    u2Pid = (UINT16)((u4Status >> 16) & 0x1fff);

    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_PVR_ERROR_STATUS_REG, 1);

    LOG(3, "PVR error! INT: 0x%08x, PID = 0x%x\n", u4Status, u2Pid);

    // Unused in release build
    UNUSED(u2Pid);
}


//-----------------------------------------------------------------------------
/** _DmxSteerErrorInt
 *  Error handler of steering logic
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxSteerErrorInt(void)
{
    UINT32 u4Status;
    UINT8 u1Pidx, u1Type;

    u4Status = DMXCMD_READ32(DMX_REG_STEER_ERROR_STATUS_REG);
    u1Type = (UINT8)((u4Status & 0xff));
    u1Pidx = (UINT8)((u4Status >> 16) & 0xff);

    LOG(3, "Steering error! INT: 0x%08x\n", u4Status);

    if (u1Pidx < DMX_NUM_PID_INDEX)
    {
        PID_STRUCT_T* prPidStruct;

        prPidStruct = _DMX_GetPidStruct(u1Pidx);
        prPidStruct->rCounters.u4SteerErrors++;
        prPidStruct->rCounters.u4TotalErrors++;
    }
    else
    {
        LOG(3, "Steering error: invalid pid index %u\n", u1Pidx);
    }

    if (u1Type == 2)
    {
        // Lack of input buffer space at the FTuP, check FIFO full
//        _DmxFifoFull(u1Pidx);
    }

    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
}


//-----------------------------------------------------------------------------
/** _DmxDispatchFTuPInterrupt
 *  Dispatch uP interrupts
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxDispatchFTuPInterrupt(void)
{
    // Normal mode
    while (1)
    {
        PID_STRUCT_T* prPidStruct;
        UINT32 u4Status;
        UINT8 u1Pidx, u1Type, u1PendInt;
        BOOL fgEnabled, fgOk, fgScrambled;
#ifdef ENABLE_MULTIMEDIA
        UINT32 u4Status2;
#endif

        u4Status = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG1);  // 113
        u1Pidx = GET_BYTE(u4Status, 2);         // PID index
        u1Type = GET_BYTE(u4Status, 0);         // 0: none, 1: PES, 2: PSI
        u1PendInt = GET_BYTE(u4Status, 3);      // Pending interrupts

#ifdef ENABLE_MULTIMEDIA
        u4Status2 = DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2);
#endif  // ENABLE_MULTIMEDIA

        if (u1PendInt == 0)
        {
            break;
        }

        DMX_PANIC(u1Pidx < DMX_NUM_ALL_PID_INDEX);
        DMX_PANIC_RET_VOID();


        //----------------------------------------------
        // Handle record interrupt
        //----------------------------------------------
#ifdef ENABLE_MULTIMEDIA
        if(u1Type >= 64)
        {
            fgOk = FALSE;
            if(u1Pidx >= DMX_NUM_PID_INDEX)
            {
                u1Pidx -= DMX_NUM_PID_INDEX;
            }

            switch(u1Type)
            {
            case 64:    // packet count
            case 65:    // I,P,B notification
            case 66:    // Timer notification (End Addr + 1)
            case 67:    // Timer notification (long packet count)
            case 68:
                fgOk = _FVR_DispatchuPInterrupt(u4Status, u4Status2);
                break;
            default:
                // FIXME: Temporarily remove the following check
//              ASSERT(0);
                break;
            }

            if (!fgOk)
            {
                LOG(1, "uP record interrupt: pidx %u invalid reason code 0x%02x!\n",
                    u1Pidx, u1Type);
            }
        }
        else
#endif  // ENABLE_MULTIMEDIA
        {
            //----------------------------------------------
            // Handle play interrupt
            //----------------------------------------------
            prPidStruct = NULL;
            fgEnabled = FALSE;
            if(u1Pidx < DMX_NUM_PID_INDEX)
            {
                prPidStruct = _DMX_GetPidStruct(u1Pidx);
                fgEnabled = _DMX_IsPidEnabled(u1Pidx);
            }
            else
            {
                LOG(3, "%s, idx %d exceed, status 0x%08x\n", __FUNCTION__, u1Pidx,
                           u4Status);
            }

            if (!fgEnabled)
            {
                // Non-error interrupt on a disabled pid, caused by race condition
                // between disabling pid and raising interrupt.
                // Simply do nothing here
                LOG(5, "Interrupt on disabled: pidx %u, status 0x%08x\n", u1Pidx, u4Status);
            }
            else
            {
                fgOk = FALSE;
                switch (u1Type)
                {
                case 0:                 // No status to report
                    fgOk = TRUE;
                    break;

                case 1:                 // PES
                case 5:
                    DMX_PANIC((PID_S_W(u1Pidx, 0) & 1) == 0);
                    DMX_PANIC_RET_VOID();
                    _DmxPesInt(TRUE);
                    fgOk = TRUE;
                    break;

                case 2:                 // PSI
                    DMX_PANIC((PID_S_W(u1Pidx, 0) & 1) != 0);
                    DMX_PANIC_RET_VOID();
                    _DmxPsiInt();
                    fgOk = TRUE;
                    break;

                case 3:                 // Partial PES
                    DMX_PANIC((PID_S_W(u1Pidx, 0) & 1) == 0);
                    DMX_PANIC_RET_VOID();
                    _DmxPesInt(FALSE);
                    fgOk = TRUE;
                    break;

                case 4:                 // Close PES
                    DMX_PANIC((PID_S_W(u1Pidx, 0) & 1) == 0);
                    DMX_PANIC_RET_VOID();
                    PID_S_W(u1Pidx, 14) =
                        PHYSICAL(DMXCMD_READ32(DMX_REG_FTuP_NONERR_STATUS_REG2));
                    if(prPidStruct != NULL)
                    {
                        prPidStruct->u4NextPic = 0;
                    }
                    fgOk = TRUE;
                    break;

                case 16:    // incoming packet is not scrambled
                case 17:    // incoming packet is scrambled
                    fgScrambled = (u1Type == 17);
                    _DmxProcessScrambleChange(u1Pidx, fgScrambled);
                    fgOk = TRUE;
                    break;

                case 32:    // Pre-parsing interrupt
                    fgOk = _DmxPreparsingInt(u1Pidx);
                    break;

                default:
                    // FIXME: Temporarily remove the following check
//                DMX_PANIC(0);
//                DMX_PANIC_RET_VOID();
                    break;
                }   // switch

                if (!fgOk)
                {
                    LOG(1, "uP interrupt: pidx %u invalid reason code 0x%02x!\n",
                        u1Pidx, u1Type);
                }
            }   // if (!fgEnabled)
        }

        // Clear interrupt
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);

        UNUSED(prPidStruct);        // Unused in release build
    }   // while (1)
}


//-----------------------------------------------------------------------------
/** _DmxDispatchInterrupt
 *  Dispatch demux interrupt
 *
 *  @param  u4Status        Interrupt status word
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxDispatchInterrupt(UINT32 u4Status)
{
    BOOL fgRecuPErrorHandled = TRUE;

    // Check if spurious interrupt
    if (u4Status == 0)
    {
        LOG(3, "Spurious interrupt!\n");
        return;
    }

    // Check if error interrupt
    if (u4Status & DMX_INT_ERR_MASK)
    {
        LOG(6, "Error INT: 0x%08x\n", u4Status);

        if (u4Status & DMX_INT_ERR_DBM)
        {
            _DmxDbmErrorInt();
        }

        if (u4Status & DMX_INT_ERR_DESCRAMBLER)
        {
            _DmxDescramblerErrorInt();
        }

        if (u4Status & DMX_INT_ERR_PVR)
        {
            _DmxPvrErrorInt();
        }

        if (u4Status & DMX_INT_ERR_STERRING)
        {
            _DmxSteerErrorInt();
        }

        if (u4Status & DMX_INT_ERR_FTuP)
        {
            fgRecuPErrorHandled = _DmxFTuPErrorInt();
        }
    }

    // Handle normal cases
    if (u4Status & DMX_INT_STATUS_MASK)
    {
        if (u4Status & DMX_INT_STATUS_FTuP)
        {
            _DmxDispatchFTuPInterrupt();
        }

#ifndef CC_MINI_DRIVER
        if (u4Status & DMX_INT_STATUS_DESCRAMBLER)
        {
            _DmxDescramblerInt();
        }
#endif  // #ifndef CC_MINI_DRIVER

        if (u4Status & DMX_INT_STATUS_PVR)
        {
            _DmxPvrInt();
        }

    }

#ifdef ENABLE_MULTIMEDIA
    // Special case PVR uP error
    // FVR record uP error (overflow) must handle after normal uP interrupt
    if (!fgRecuPErrorHandled)
    {
        // Clear interrupt
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);

        _FVR_uPErrorInt();
    }
#else
    UNUSED(fgRecuPErrorHandled);
#endif  // ENABLE_MULTIMEDIA

    // PCR error and status
    if (u4Status & DMX_INT_PCR_MASK)
    {
#ifdef __PCR_RECOVERY__
        _DMX_IntPcr(u4Status);
#else
        _DmxPcrInt(u4Status);
#endif
    }
}


//-----------------------------------------------------------------------------
/** _DmxIrqHandler
 *  Demux interrupt handler
 *
 *  @param  u2Vector        The IRQ vector
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _DmxIrqHandler(UINT16 u2Vector)
{
    static BOOL _fgISR = FALSE;

    ASSERT(!_fgISR);
    UNUSED(_fgISR);                 // Avoid compile warning in release build
    _fgISR = TRUE;

#if !defined(__linux__)
    ASSERT(u2Vector == VECTOR_DEMUX);
    if (!BIM_IsIrqPending(VECTOR_DEMUX))
    {
        LOG(3, "Spurious demux global interrupt!\n");
    }

    // Check if it's demux interrupt
    while (BIM_IsIrqPending(VECTOR_DEMUX))
#endif
    {
        UINT32 u4Status, u4Mask;

        u4Status = DMXCMD_READ32(DMX_REG_INT_STAUS);
        u4Mask = DMXCMD_READ32(DMX_REG_INT_MASK);
        u4Status &= u4Mask;

        if (u4Status)
        {
            _DmxDispatchInterrupt(u4Status);
        }
        else
        {
            LOG(3, "Spurious demux local interrupt!\n");
        }

        // Clear interrupt
        VERIFY(BIM_ClearIrq(VECTOR_DEMUX));
    }

    _fgISR = FALSE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_InitISR
 *  Initialize interrupt handler
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_InitISR(void)
{
    static BOOL _fgInited = FALSE;
    x_os_isr_fct pfnOldIsr;

    // Register ISR
    if (!_fgInited)
    {
        if (x_reg_isr(VECTOR_DEMUX, _DmxIrqHandler, &pfnOldIsr) != OSR_OK)
        {
            LOG(1, "Error: fail to register demux ISR!\n");
        }
        UNUSED(pfnOldIsr);

        _fgInited = TRUE;
    }

    // Enable all demux interrupts
    DMXCMD_WRITE32(DMX_REG_INT_MASK, 0xffffffff);

    LOG(7, "Enable demux interrupt\n");
}


