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
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_ps_cpu.c
 *  Demux driver for program stream playback with CPU
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx_mm.h"
#include "dmx_debug.h"
#include "x_lint.h"

//////////////////////////////////////////////////////////////////// LINT_EXT_HEADER_BEGIN
#include "gcpu_if.h"
#include "vdec_if.h"
#include "mpv_drvif.h"
#include "aud_drvif.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_hal_926.h"
//////////////////////////////////////////////////////////////////// LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PS_PES_HEADER_TMP_BUFFER            256U

#define PS_FIELD_LEN_SYNC_MARKER            12U
#define PS_FIELD_LEN_VCD_SUBHEADER          12U
#define PS_FIELD_LEN_PES_LEN                2U
#define PS_FIELD_LEN_PES_FLAGS              3U
#define PS_FIELD_LEN_PTS                    5U
#define PS_FIELD_LEN_DTS                    5U
#define PS_FIELD_LEN_PES_EXT_FLAGS          1U
#define PS_FIELD_LEN_MPEG1_STD              2U
#define PS_FIELD_LEN_MPEG1_PTS              5U
#define PS_FIELD_LEN_MPEG1_PTS_DTS          10U
#define PS_FIELD_LEN_MPEG1_HEADER_MARKER    1U

#define PS_SID_SEQUENCE_START               0xB3
#define PS_SID_SEQUENCE_END                 0xB7
#define PS_SID_PACK_START_CODE              0xBA
#define PS_SID_SYSTEM_HEADER_START_CODE     0xBB
#define PS_SID_PROGRAM_STREAM_MAP           0xBC
#define PS_SID_PRIVATE_STREAM_1             0xBD
#define PS_SID_PADDING_STREAM               0xBE
#define PS_SID_PRIVATE_STREAM_2             0xBF
#define PS_SID_ECM_STREAM                   0xF0
#define PS_SID_EMM_STREAM                   0xF1
#define PS_SID_DSMCC_STREAM                 0xF2
#define PS_SID_H222_1_E_STREAM              0xF8
#define PS_SID_PROGRAM_STREAM_DIRECTORY     0xFF

#define PS_WORD_SEQUENCE_END                0x000001B7

#define DMX_PS_CPU_UNKNOWN_PIDX             0xFF
#define PS_MAX_LEN_WITHOUT_PREFIX           20480U

#define PS_HDCP_KEY_LEN                     16
#define PS_HDCP_IV_LEN                      16

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// The following enumeration must be continuously without any gap in between
// because they are used as the indices to the array of PS_PARSER_T.
typedef enum
{
    PS_STATE_START,
    PS_STATE_SYNC_MARKER,           /* 12 bytes */
    PS_STATE_VCD_SUBHEADER,         /* 12 bytes */
    PS_STATE_PES_START_PREFIX,      /*  4 bytes */
#if 0
    PS_STATE_SYSTEM_HEADER,
#endif  // 0
    PS_STATE_PES_LEN,               /*  2 bytes */
    PS_STATE_STREAM_TYPE,           /* variable length; stream type detection */
    PS_STATE_PES_FLAGS,             /*  3 bytes */
    PS_STATE_PES_HEADER,            /* variable length */
    PS_STATE_PAYLOAD,               /* variable length */
    PS_STATE_PADDING,               /* variable length */
    PS_STATE_SEQ_END                /* Marker for end of the enumeration */
} PS_SEARCH_STATE_T;


typedef enum
{
    STREAM_UNKNOWN,
    STREAM_MPEG1,
    STREAM_MPEG2
} PS_STREAM_TYPE_T;


typedef enum
{
    PS_STATE_MPEG1_UNKNOWN,
    PS_STATE_MPEG1_STUFFING,
    PS_STATE_MPEG1_UNDETERMINED_1,
    PS_STATE_MPEG1_STD,
    PS_STATE_MPEG1_UNDETERMINED_2,
    PS_STATE_MPEG1_PTS,
    PS_STATE_MPEG1_PTS_DTS,
    PS_STATE_MPEG1_ONE_BYTE_MARKER,
    PS_STATE_MPEG1_PARSING_HEADER
} PS_STATE_MPEG1_HEADER_STATE_T;


typedef struct PS_PARSER_T
{
    PS_SEARCH_STATE_T eState;
    BOOL (*pfnPsParsing)(void);
} PS_PARSER_T;


typedef struct PS_MANAGER_T
{
    PS_SEARCH_STATE_T eSearchState;
    PS_STREAM_TYPE_T eStreamType;
    PS_STATE_MPEG1_HEADER_STATE_T eMpeg1HeaderState;

    DMX_DATA_FORMAT_T eDataFormat;  // global for any PS file
    DMX_AUDIO_TYPE_T eAudioType;    // global for any PS file
    UINT8 u1AudioInfoLen;           // global for any PS file

    UINT32 u4BufStart;              // source pointer for data transmission
    UINT32 u4BufEnd;                // source pointer for data transmission
    UINT32 u4BufSize;               // source buffer size for data transmission
    UINT32 u4BufRp;                 // source pointer for data transmission
    UINT32 u4DataSize;              // source size for data transmission
    BOOL fgEOS;

    UINT32 u4RemainingPesLen;
    UINT32 u4RemainingSectorLen;
    UINT32 u4LastWord;              // the first byte is stored in byte 0
    UINT16 u2Counter;               // Counter for recording the data length to consume

    UINT8 au1AudioInfo[8];
    BOOL fgAudioInfoCopied;         // Has the audio info copy operation finished?
    UINT32 u4AudioInfo;

    UINT8 u1StreamID;               // Stream ID (change with data transmission)
    UINT8 u1SubstreamID;            // Sub-stream ID  (change with data transmission)
    UINT16 u2PesLen;                // PES_packet_length  (change with data transmission)

    /*** PES header flags ***/
    //UINT8 u1ScramblingCtrl;
    //BOOL fgPriority;
    //BOOL fgAlignment;
    BOOL fgCopyright;
    BOOL fgOriginal;
    UINT8 u1PtsDtsFlags;
    BOOL fgEscr;
    BOOL fgEsRate;
    BOOL fgTrick;
    BOOL fgCopyInfo;
    BOOL fgPesCrc;
    BOOL fgExt;
    UINT8 u1PesHeaderLen;           /* 1 byte */
    UINT8 u1PesHeaderExtraLen;
    UINT8 au1PesHeader[PS_PES_HEADER_TMP_BUFFER];
    UINT16 u2PesHeaderCopyLen;
#ifdef CC_DMX_HDCP
    BOOL fgHDCP20;
    UINT8 au1Key[PS_HDCP_KEY_LEN];
    UINT8 au1Iv[PS_HDCP_IV_LEN];
    UINT8 au1PreviousBuf[32];
    UINT32 u4PreviousBufSize;
#endif
#if 0
    /*** PES_extension fields ***/
    BOOL fgPesPrivateData;
    BOOL fgPackHeaderField;
    BOOL fgSeqCounter;
    BOOL fgPstdBuffer;
    BOOL fgPesExt2;
    UINT8 u1PackFieldLen;           /* 1 byte */
    UINT8 u1PesExtFieldLen;         /* 7 bits */
#endif  // 0
    UINT64 u8Pts;
    UINT64 u8Dts;

    UINT32 u4TransactionCount;      // for debugging
    UINT32 u4ByteProcessed;         // for debugging
    BOOL fgFirstByteOfPayload;      // This usually is substream ID in payload
} DMX_PS_MANAGER_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define SET_PS_STATE_TO_START       (_arPsManager[_u1PsActive].eSearchState = PS_STATE_START)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static const UINT8 _arSyncMarker[PS_FIELD_LEN_SYNC_MARKER] =
                            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                             0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};

//#define CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
static BOOL _fgStarted = FALSE;
static BOOL _fgEnded = FALSE;
static UINT32 _u4StreamIdNumber = 0;
static HAL_RAW_TIME_T _rRawTime1;
static HAL_RAW_TIME_T _rRawTime2;
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD

static DMX_PS_MANAGER_T _arPsManager[2];

static UINT8 _u1PsActive = 0;

static DMX_PS_MOVEDATA_CMD_T _arPSMoveDataCmd[DMX_PS_STRUCT_NUM];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_ResetPsManager
 */
//-----------------------------------------------------------------------------
static void _Dmx_PS_CPU_ResetPsManager(UINT8 u1Inst)
{
    DMX_AUDIO_TYPE_T eAudioType;
    DMX_DATA_FORMAT_T eDataFormat;
    UINT8 u1AudioInfoLen;

    ASSERT(u1Inst < DMX_MAX_INSTANCE);
    if(u1Inst >= DMX_MAX_INSTANCE)
    {
        return;
    }

    eAudioType = _arPsManager[u1Inst].eAudioType;
    eDataFormat = _arPsManager[u1Inst].eDataFormat;
    u1AudioInfoLen = _arPsManager[u1Inst].u1AudioInfoLen;

    x_memset((void*)&_arPsManager[u1Inst], 0, sizeof(_arPsManager[u1Inst]));

    _arPsManager[u1Inst].eAudioType = eAudioType;
    _arPsManager[u1Inst].eDataFormat = eDataFormat;
    _arPsManager[u1Inst].u1AudioInfoLen = u1AudioInfoLen;
    _arPsManager[u1Inst].eSearchState = PS_STATE_START;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_SetBuffer
 *
 *  Side effect:
 *       1. Header buffer pointers:  FIFO start, FIFO end, start, write, read
 *       2. Payload buffer pointers: FIFO start, FIFO end, start, write, read
 *       3. the data pointed to by the argument prPidStruct
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_SetBuffer(UINT8 u1Idx, const DMX_PS_T *prPS,
                              PID_STRUCT_T *prPidStruct)
{
    UINT32 u4BufStart, u4PhyBufStart, u4W0 = 0;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);
    ASSERT(prPS != NULL);
    ASSERT(prPidStruct != NULL);

    if (prPS->ePidType == DMX_PID_TYPE_NONE)
    {
        return TRUE;    // No PID type, can't setup buffer
    }

    // Check if the buffer has been allocated.  (Avoid memory leakage.)
    if ((prPidStruct->u4BufStart != 0) || (prPidStruct->u4HeaderBufAddr != 0))
    {
        return FALSE;
    }

    if ((prPS->ePidType == DMX_PID_TYPE_PSI) ||
        /*(prPS->ePidType == DMX_PID_TYPE_PES) ||*/
        (prPS->ePidType == DMX_PID_TYPE_ES_OTHER))
    {
        LOG(3, "Program stream should not be of this type!\n");
        return FALSE;
    }
    else
    {
        // Section filter flags
        // bit 0:
        // bit 1: Valid bit
        // bit 2: output mode
        // bit 3:
        // bit 4: Toggle bit (for use along with bit 8 of PID index table)
        // bit 5: PID Disable
        // bit 6: To CD-FIFO
        // bit 7: Enable picture finder
        u4W0 = (UINT32)((1 << 6) |
            (((prPS->ePidType == DMX_PID_TYPE_ES_VIDEO) ? 1 : 0) << 7));

        u4W0 |= (UINT32)(1 << 1);

        if(prPS->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            //set byte2 to 0xff for default picture search by TS_IDX
            u4W0 |= 0x00FF0000;
        }

        // Setup header buffer
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(PES_HEADER_BUF_SIZE,
            DMX_HEADER_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            LOG(3, "%s:L%d: Memory allocation failed!\n", __func__, __LINE__);
            return FALSE;
        }

        _DMX_Lock();
        u4BufStart = VIRTUAL(u4BufStart);
        u4PhyBufStart = PHYSICAL(u4BufStart);
        prPidStruct->u4HeaderBufAddr = u4BufStart;
        prPidStruct->u4NextPic = 0;
        PID_S_W(u1Idx, 10) = u4PhyBufStart;    // (Header) FIFO start
        PID_S_W(u1Idx, 12) = u4PhyBufStart;    // (Header) Start
        PID_S_W(u1Idx, 13) = u4PhyBufStart;    // (Header) Write pointer
        PID_S_W(u1Idx, 14) = u4PhyBufStart;    // (Header) Read pointer
        PID_S_W(u1Idx, 11) = (u4PhyBufStart + PES_HEADER_BUF_SIZE) - 1;  // (Header) end address
        _DMX_Unlock();
    }

    // FIFO starting address
    if (prPS->fgAllocateBuffer)
    {
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(prPS->u4BufSize,
            DMX_ES_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            VERIFY(BSP_FreeAlignedDmaMemory(
                prPidStruct->u4HeaderBufAddr));
            LOG(3, "%s:L%d: Memory allocation failed!\n", __func__, __LINE__);
            return FALSE;
        }
    }
    else
    {
        ASSERT(prPS->u4BufAddr != 0);
        u4BufStart = prPS->u4BufAddr;
    }

    _DMX_Lock();
    // Preserve the Toggle bit.
    PID_S_W(u1Idx, 0) = (PID_S_W(u1Idx, 0) & (0x1 << 4)) | u4W0;

    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    u4BufStart    = VIRTUAL(u4BufStart);
    u4PhyBufStart = PHYSICAL(u4BufStart);
    prPidStruct->u4BufStart       = u4BufStart;
    prPidStruct->u4Wp             = u4BufStart;
    prPidStruct->u4Rp             = u4BufStart;
    prPidStruct->u4SectionRp      = u4BufStart;
    prPidStruct->u4PesRp          = u4BufStart;
    prPidStruct->u4BufEnd         = (u4BufStart + prPS->u4BufSize) - 1;
    prPidStruct->u4BufLen         = prPS->u4BufSize;    // FIFO size
    prPidStruct->fgAllocateBuffer = prPS->fgAllocateBuffer;
    prPidStruct->ePidType         = prPS->ePidType;
    PID_S_W(u1Idx, 5) = u4PhyBufStart;         // FIFO start
    PID_S_W(u1Idx, 7) = u4PhyBufStart;         // PES_start
    PID_S_W(u1Idx, 8) = u4PhyBufStart;         // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyBufStart;         // Read pointer
    PID_S_W(u1Idx, 6) = (u4PhyBufStart + prPS->u4BufSize) - 1; // End pointer

    prPidStruct->u1ChannelId = prPS->u1ChannelId;
    _DMX_Unlock();

    if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if (!_DMX_AllocateVideoChannel(u1Idx, prPidStruct->u1ChannelId))
        {
            return FALSE;
        }
    }

#ifdef DEBUG_MEMORY_INTRUSION
    if (_DmxIsProtectedMemory(prPidStruct))
    {
        VERIFY(_DmxSetProtectedMemory(u4PhyBufStart, PID_S_W(u1Idx, 6) + 1));
    }
#endif  // DEBUG_MEMORY_INTRUSION

    HalFlushInvalidateDCache();

    LOG(8, "Set PS pixd %u buffer: 0x%08x - 0x%08x\n", u1Idx, u4BufStart,
        prPidStruct->u4BufEnd);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_FreeBuffer
 *  Free PID buffer(s)
 *
 *  @param  u1Idx           PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_FreeBuffer(UINT8 u1Idx)
{
    UINT32 u4PhyAddr;
    PID_STRUCT_T *prPidStruct;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    // _DMX_Lock() and _DMX_Unlock() are not needed because a pointer is used.
    prPidStruct = _DMX_GetPidStruct(u1Idx);

    LOG(8, "Free PS pidx %u buffer\n", u1Idx);

#ifdef DEBUG_MEMORY_INTRUSION
    if (_DmxIsProtectedMemory(prPidStruct))
    {
        VERIFY(_DmxResetProtectedMemory());
    }
#endif  // DEBUG_MEMORY_INTRUSION

    // Free buffer
    if (prPidStruct->fgAllocateBuffer)
    {
        if (!BSP_FreeAlignedDmaMemory(prPidStruct->u4BufStart))
        {
            LOG(3, "Failed to free PID buffer! pidx: %u, addr: 0x%08x\n",
                u1Idx, prPidStruct->u4BufStart);
            return FALSE;
        }
    }

    // Free header buffer
    if (!BSP_FreeAlignedDmaMemory(prPidStruct->u4HeaderBufAddr))
    {
        LOG(3, "Failed to free PID header buffer! pidx: %u, addr: 0x%08x\n",
            u1Idx, prPidStruct->u4HeaderBufAddr);
        return FALSE;
    }

    _DMX_Lock();
    // Update hardware pointers.
    u4PhyAddr = PID_S_W(u1Idx, 5);         // Buffer Start
    PID_S_W(u1Idx, 8) = u4PhyAddr;         // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyAddr;         // Read pointer
    u4PhyAddr = PID_S_W(u1Idx, 10);        // (Header) Buffer Start
    PID_S_W(u1Idx, 13) = u4PhyAddr;        // (Header) Write pointer
    PID_S_W(u1Idx, 14) = u4PhyAddr;        // (Header) Read pointer
    // Update PID state
    prPidStruct->fgAllocateBuffer = FALSE;
    prPidStruct->u4BufStart = 0;
    prPidStruct->u4BufEnd = 0;
    prPidStruct->u4BufLen = 0;
    prPidStruct->u4Rp = 0;
    prPidStruct->u4Wp = 0;
    prPidStruct->u4HeaderBufAddr = 0;
    _DMX_Unlock();

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        // Reset pidx-to-channel mapping
        _DMX_FreeVideoChannel(u1Idx);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_ResetPidBuffer
 *
 *  @param  u1Idx           PID index
 *  @parm   prPidStruct     PID data structure
 *
 *  @retval TRUE            Sucessful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_ResetPidBuffer(UINT8 u1Idx, PID_STRUCT_T *prPidStruct)
{
    UINT32 u4StartAddr;

    ASSERT(prPidStruct != NULL);

    // Check if PID is active
    if (prPidStruct->fgEnable)
    {
        LOG(3, "Cannot reset buffer of an active pid.\n");
        return FALSE;
    }

    _DMX_Lock();
    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    // Reset payload FIFO
    u4StartAddr = prPidStruct->u4BufStart;
    prPidStruct->u4Rp = u4StartAddr;
    prPidStruct->u4Wp = u4StartAddr;
    prPidStruct->u4SectionRp = u4StartAddr; // of no use
    prPidStruct->u4PesRp = u4StartAddr;     // of no use
    u4StartAddr = PHYSICAL(u4StartAddr);
    PID_S_W(u1Idx, 7) = u4StartAddr;       // PES_start
    PID_S_W(u1Idx, 8) = u4StartAddr;       // Write pointer
    PID_S_W(u1Idx, 9) = u4StartAddr;       // Read pointer

    // Reset header FIFO
    u4StartAddr = PHYSICAL(prPidStruct->u4HeaderBufAddr);
    PID_S_W(u1Idx, 12) = u4StartAddr;      // Start
    PID_S_W(u1Idx, 13) = u4StartAddr;      // Write pointer
    PID_S_W(u1Idx, 14) = u4StartAddr;      // Read pointer
    prPidStruct->u4NextPic = 0;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_EnablePid
 *
 *  @param  u1Idx           PID index
 *  @parm   prPS            PS data structure
 *  @parm   pu4PidEntry     Pointer to the value of PID index entry
 *  @parm   pu4W0           Pointer to the word 0 of PID memory
 *  @parm   prPidStruct     Internal PID data structure
 *
 *  @retval TRUE            Sucessful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_EnablePid(UINT8 u1Idx, BOOL fgEnable, UINT32 *pu4W0, PID_STRUCT_T *prPidStruct)
{
    ASSERT(pu4W0 != NULL);
    ASSERT(prPidStruct != NULL);

    if (fgEnable)
    {
        if (!_Dmx_PS_CPU_ResetPidBuffer(u1Idx, prPidStruct))
        {
            return FALSE;
        }
        // Always set bit4 to 0, no use toggle mechanism
        *pu4W0 &= (~0x10);
        //*pu4W0 |= 0x4;
    }
    else
    {
        // Copy invere of PID index table bit 8 into PID memory bit 4 (word 0)
        prPidStruct->u1SerialNumber++;
        if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
        {
            prPidStruct->u1SerialNumber = 0;
        }
    }

    prPidStruct->fgEnable = fgEnable;

    // Notify handler
    _DMX_NotifyEnablePid(u1Idx, fgEnable);

    return TRUE;
}


static BOOL _DMX_PS_TriggerMoveDataQueue(UINT8 u1Idx)
{
    if (!(u1Idx < DMX_PS_STRUCT_NUM))
    {
        return FALSE;
    }

    if(_arPSMoveDataCmd[u1Idx].fgUsed == TRUE)
    {
        if (_arPSMoveDataCmd[u1Idx].u1CmdIndex > DMX_PS_MOVE_DATA_Q_SIZE)
        {
            return FALSE;
        }

        _arPSMoveDataCmd[u1Idx].u4FrameSize = DATASIZE(_arPSMoveDataCmd[u1Idx].u4StartAddr,
            _arPSMoveDataCmd[u1Idx].u4PayloadEndAddr[_arPSMoveDataCmd[u1Idx].u1CmdIndex - 1]
            , _arPSMoveDataCmd[u1Idx].u4BufEnd - _arPSMoveDataCmd[u1Idx].u4BufStart);

        //ASSERT(_arPSMoveDataCmd[u1Idx].u4BufEnd - _arPSMoveDataCmd[u1Idx].u4BufStart > _arPSMoveDataCmd[u1Idx].u4FrameSize);
        //ASSERT(_arPSMoveDataCmd[u1Idx].u4StartAddr >= _arPSMoveDataCmd[u1Idx].u4BufStart && _arPSMoveDataCmd[u1Idx].u4StartAddr <= _arPSMoveDataCmd[u1Idx].u4BufEnd);
        //ASSERT(_arPSMoveDataCmd[u1Idx].u4FrameSize <= _arPSMoveDataCmd[u1Idx].u4BufStart - _arPSMoveDataCmd[u1Idx].u4BufEnd);

        _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, _arPSMoveDataCmd[u1Idx].u1Idx, TRUE,  FALSE);
        _DMX_PS_MoveData(_arPSMoveDataCmd[u1Idx].u1Idx, &(_arPSMoveDataCmd[u1Idx]), _arPSMoveDataCmd[u1Idx].u4TimeOut);
        
        _arPSMoveDataCmd[u1Idx].fgUsed = FALSE;
        _arPSMoveDataCmd[u1Idx].u1CmdIndex = 0;
    }

    return TRUE;
}

static BOOL _DMX_PS_TriggerAllMoveDataQueue(void)
{
    int i;

    for(i = 0; i <DMX_PS_STRUCT_NUM; i++)
    {
        if(!_DMX_PS_TriggerMoveDataQueue(i))
        {
            return FALSE;
        }
    }

    return TRUE;
}


static BOOL _DMX_PS_MoveDataQueue(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    static UINT8 u1OldIdx = 0;
    PID_STRUCT_T *prPidStruct;

    if (u1Idx >= DMX_PS_STRUCT_NUM)
    {
        return FALSE;
    }

    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    if (_arPSMoveDataCmd[u1Idx].u1CmdIndex > DMX_PS_MOVE_DATA_Q_SIZE)
    {
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, u1Idx, TRUE,  FALSE);
        // We did not queue audio data, because ISR needs to get u4AudioInfo
        return _DMX_MM_MoveData(u1Idx, prData, u4TimeOut);
    }

    if(_arPSMoveDataCmd[u1Idx].u1CmdIndex == DMX_PS_MOVE_DATA_Q_SIZE) //cmd queue is full
    {
        //trigger command queue
        if(!_DMX_PS_TriggerMoveDataQueue(u1Idx))
        {
            return FALSE;
        }
    }

    if(u1OldIdx != u1Idx) // avoid from reordering stream data
    {
        if(!_DMX_PS_TriggerMoveDataQueue(u1OldIdx))
        {
            return FALSE;
        }
    }

    if(_arPSMoveDataCmd[u1Idx].fgUsed == TRUE)
    {
        if((prData->u4Pts != _arPSMoveDataCmd[u1Idx].u4Pts))
        {
            //trigger command queue
            if(!_DMX_PS_TriggerMoveDataQueue(u1Idx))
            {
                return FALSE;
            }
        }
    }

    if(_arPSMoveDataCmd[u1Idx].fgUsed == FALSE) // queue the first command
    {
        _arPSMoveDataCmd[u1Idx].fgUsed = TRUE;
        _arPSMoveDataCmd[u1Idx].u1Idx = u1Idx;
        _arPSMoveDataCmd[u1Idx].u1StreamID = _arPsManager[_u1PsActive].u1StreamID;
        _arPSMoveDataCmd[u1Idx].u4BufStart = _arPsManager[_u1PsActive].u4BufStart;
        _arPSMoveDataCmd[u1Idx].u4BufEnd = _arPsManager[_u1PsActive].u4BufEnd;
        _arPSMoveDataCmd[u1Idx].u4StartAddr = prData->u4StartAddr;
        _arPSMoveDataCmd[u1Idx].u4Pts = prData->u4Pts;
        _arPSMoveDataCmd[u1Idx].u4Dts = prData->u4Dts;
        _arPSMoveDataCmd[u1Idx].u2SkipLen[_arPSMoveDataCmd[u1Idx].u1CmdIndex] = 0;
        _arPSMoveDataCmd[u1Idx].u2PayloadLen[_arPSMoveDataCmd[u1Idx].u1CmdIndex] = prData->u4FrameSize;
        _arPSMoveDataCmd[u1Idx].u4TimeOut = u4TimeOut;

        _arPSMoveDataCmd[u1Idx].u4PayloadEndAddr[_arPSMoveDataCmd[u1Idx].u1CmdIndex] =
             ADDR_INCR_IN_RING(prData->u4StartAddr, prData->u4FrameSize,
             _arPsManager[_u1PsActive].u4BufStart, _arPsManager[_u1PsActive].u4BufEnd);
    }
    else
    {
        _arPSMoveDataCmd[u1Idx].u2SkipLen[_arPSMoveDataCmd[u1Idx].u1CmdIndex] =
            DATASIZE(_arPSMoveDataCmd[u1Idx].u4PayloadEndAddr[_arPSMoveDataCmd[u1Idx].u1CmdIndex -1],
            prData->u4StartAddr, _arPsManager[_u1PsActive].u4BufEnd - _arPsManager[_u1PsActive].u4BufStart);
        _arPSMoveDataCmd[u1Idx].u2PayloadLen[_arPSMoveDataCmd[u1Idx].u1CmdIndex] = prData->u4FrameSize;

        _arPSMoveDataCmd[u1Idx].u4PayloadEndAddr[_arPSMoveDataCmd[u1Idx].u1CmdIndex] =
             ADDR_INCR_IN_RING(prData->u4StartAddr, prData->u4FrameSize,
             _arPsManager[_u1PsActive].u4BufStart, _arPsManager[_u1PsActive].u4BufEnd);
    }

    _arPSMoveDataCmd[u1Idx].u1CmdIndex++;
    u1OldIdx = u1Idx;

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_SetStream
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_SetStream(UINT8 u1Idx, UINT32 u4Flags, const DMX_PS_T *prPS)
{
    PID_STRUCT_T rPidStruct;
    UINT32 u4W0;

    ASSERT(prPS != NULL);

    if (u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid PID index: %u\n", u1Idx);
        return FALSE;
    }

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();
    rPidStruct = *(_DMX_GetPidStruct(u1Idx));
    _DMX_Unlock();

    // TS index
    rPidStruct.u1TsIndex = (UINT8)DDI_TS_INDEX_FOR_NO_FRAMER;

    // Device ID
    rPidStruct.u1DeviceId = 0;

    rPidStruct.eInputType = DMX_IN_PLAYBACK_PS;
    rPidStruct.eSource = DMX_SOURCE_DRAM0;

    if(u4Flags & DMX_MM_FLAG_TYPE)
    {
        rPidStruct.ePidType = prPS->ePidType;
    }

    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        rPidStruct.pfnNotify = prPS->pfnNotify;
        rPidStruct.pvNotifyTag = prPS->pvNotifyTag;
    }

    if ((u4Flags & DMX_PID_FLAG_INSTANCE_TAG) != 0)
    {
        rPidStruct.pvInstanceTag = prPS->pvInstanceTag;
    }

    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        rPidStruct.u1DeviceId = prPS->u1DeviceId;
    }

    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        if (!_Dmx_PS_CPU_SetBuffer(u1Idx, prPS, &rPidStruct))
        {
            return FALSE;
        }
    }

    // Get PID Word 0 after buffer allocation.
    _DMX_Lock();
    u4W0 = PID_S_W(u1Idx, 0);

    if ((u4Flags & DMX_PID_FLAG_STREAM_ID) != 0)
    {
        rPidStruct.u1StreamID = prPS->u1StreamID;
    }

    if ((u4Flags & DMX_PID_FLAG_SUBSTREAM_ID) != 0)
    {
        rPidStruct.u1SubstreamID = prPS->u1SubstreamID;
        rPidStruct.fgEnableSsidFilter = prPS->fgEnableSsidFilter;
    }

    _DMX_Unlock();

    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        if (!_Dmx_PS_CPU_EnablePid(u1Idx, prPS->fgEnable, &u4W0, &rPidStruct))
        {
            return FALSE;
        }

        if(prPS->fgEnable)
        {
            PID_S_W(u1Idx, 1) = 0x0;
            PID_S_W(u1Idx, 2) = 0x0;
            PID_S_W(u1Idx, 3) = 0x0;
            PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
            PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF
        }

    }

    _DMX_Lock();
    *(_DMX_GetPidStruct(u1Idx)) = rPidStruct;
    PID_S_W(u1Idx, 0) = u4W0;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_GetStream
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_GetStream(UINT8 u1Idx, UINT32 u4Flags, DMX_PS_T *prPS)
{
    PID_STRUCT_T *prPidStruct;

    ASSERT(prPS != NULL);

    if (u1Idx >= DMX_MM_STRUCT_NUM)
    {
        LOG(3, "Invalid PID index: %u\n", u1Idx);
        return FALSE;
    }

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    // _DMX_Lock() and _DMX_Unlock() are not needed because a pointer is used.
    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        prPS->fgAllocateBuffer = prPidStruct->fgAllocateBuffer;
        prPS->u4BufAddr = prPidStruct->u4BufStart;
        prPS->u4BufSize = prPidStruct->u4BufLen;
        prPS->ePidType = prPidStruct->ePidType;
    }

    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        prPS->pfnNotify   = prPidStruct->pfnNotify;
        prPS->pvNotifyTag = prPidStruct->pvNotifyTag;
    }

    if ((u4Flags & DMX_PID_FLAG_DATA_POINTERS) != 0)
    {
        prPS->u4Rp = VIRTUAL(prPidStruct->u4Rp);
        prPS->u4Wp = VIRTUAL(prPidStruct->u4Wp);
        prPS->u4PeakBufFull = prPidStruct->u4PeakBufFull;
    }

    if ((u4Flags & DMX_PID_FLAG_STREAM_ID) != 0)
    {
        prPS->u1StreamID = prPidStruct->u1StreamID;
    }

    if ((u4Flags & DMX_PID_FLAG_SUBSTREAM_ID) != 0)
    {
        prPS->u1SubstreamID = prPidStruct->u1SubstreamID;
        prPS->fgEnableSsidFilter = prPidStruct->fgEnableSsidFilter;
    }

    if ((u4Flags & DMX_PID_FLAG_INSTANCE_TAG) != 0)
    {
        prPS->pvInstanceTag = prPidStruct->pvInstanceTag;
    }

    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        prPS->u1DeviceId = prPidStruct->u1DeviceId;
    }
    // Valid bit
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        prPS->fgEnable = prPidStruct->fgEnable;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_FreeStream
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_FreeStream(UINT8 u1Idx)
{
    PID_STRUCT_T *prPidStruct;
    UINT8 u1SerialNumber;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    // _DMX_Lock() and _DMX_Unlock() are not needed because a pointer is used.
    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if (prPidStruct->fgEnable)
    {
        UINT32 u4W0;

        LOG(3, "Try to free an active PID! (pidx: %u)\n", u1Idx);

        // Stop PID
        _DMX_Lock();
        u4W0 = PID_S_W(u1Idx, 0);
        _DMX_Unlock();

        if (!_Dmx_PS_CPU_EnablePid(u1Idx, FALSE, &u4W0, prPidStruct))
        {
            LOG(5, "Failed to disable the enabled PID!\n");
            return FALSE;
        }

        _DMX_Lock();
        PID_S_W(u1Idx, 0) = u4W0;
        _DMX_Unlock();
    }

    // Stop and clear PID index table except for the Toggle bit.
    _DMX_Lock();
    PID_INDEX_TABLE(u1Idx) = PID_INDEX_TABLE(u1Idx) & (1 << 8);
    PID_S_W(u1Idx, 0) = PID_S_W(u1Idx, 0) & (1 << 4);
    _DMX_Unlock();

    // Flush MPV
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        UINT8 u1Channel;

        // Sync with MPV.
        VERIFY(_DMX_GetVideoChannel(u1Idx, &u1Channel));
        MPV_FlushEsmQ(u1Channel, TRUE);

        // No need to send purge message if serail number is applied.
        // Discard intermediate pictures, clean FIFO.
        // VERIFY(_DMX_SendPurgeMessage(u1Idx, FALSE));
    }
    else if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        // FIXME
        AUD_WaitDspFlush(prPidStruct->u1DeviceId);

        // No need to send purge message if serial number is applied.
        // Discard intermediate frames, clean FIFO.
        // VERIFY(_DMX_SendPurgeMessage(u1Idx, FALSE));
    }

    // Free buffer
    if (!_Dmx_PS_CPU_FreeBuffer(u1Idx))
    {
        return FALSE;
    }

    // Clear PID structures and PID memory.
    // Note that serial number must be preserved across freeing PID.
    // Also, peak buffer fullness is stored while channel changed.
    _DMX_Lock();
    u1SerialNumber = prPidStruct->u1SerialNumber;
    x_memset((void*)prPidStruct, 0, sizeof(*prPidStruct));
    prPidStruct->u1SerialNumber = u1SerialNumber;
    prPidStruct->u4LastWp = 0;
    prPidStruct->u1StreamID = 0;
    prPidStruct->u1SubstreamID = 0;
    prPidStruct->fgEnableSsidFilter = FALSE;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_FlushBuffer
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_FlushBuffer(UINT8 u1Idx)
{
    PID_STRUCT_T *prPidStruct;
    UINT32 u4BufStart, u4PhyBufStart;
    BOOL fgRet = FALSE;
    UINT8 u1Inst = 0;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        u1Inst = _DMX_MUL_GetPidxInstId(u1Idx);
        if(u1Inst >= DMX_MAX_INSTANCE)
        {
            return FALSE;
        }
    }

    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if (prPidStruct->u1StreamID == _arPsManager[u1Inst].u1StreamID)
    {
        _Dmx_PS_CPU_ResetPsManager(u1Inst);
    }

    _DMX_Lock();
    u4BufStart = prPidStruct->u4BufStart;
    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Idx, 7) = u4PhyBufStart;     // PES start
    PID_S_W(u1Idx, 8) = u4PhyBufStart;     // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyBufStart;     // Read pointer
    _DMX_Unlock();

    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
    case DMX_PID_TYPE_ES_AUDIO:
    case DMX_PID_TYPE_ES_VIDEOCLIP:
    case DMX_PID_TYPE_ES_OTHER:
        _DMX_Lock();
        u4BufStart = PHYSICAL(prPidStruct->u4HeaderBufAddr);
        PID_S_W(u1Idx, 12) = u4BufStart;    // Header Start
        PID_S_W(u1Idx, 13) = u4BufStart;    // Header Write pointer
        PID_S_W(u1Idx, 14) = u4BufStart;    // Header Read pointer
        prPidStruct->u4NextPic = 0;
        _DMX_Unlock();
        fgRet = TRUE;
        break;
    default:
        LOG(5, "L%d: Incorrect PID type!\n", __LINE__);
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_Reset
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_Reset(UINT8 u1Inst)
{
    UINT8 u1Idx;
    PID_STRUCT_T *prPidStruct;
    DMX_PID_TYPE_T ePidType;

    if(u1Inst >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    _Dmx_PS_CPU_ResetPsManager(u1Inst);

    for (u1Idx = 0; u1Idx < DMX_MM_STRUCT_NUM; u1Idx++)
    {
        if(_DMX_MUL_GetMultipleInstEnabled())
        {
            if(_DMX_MUL_GetPidxInstId(u1Idx) != u1Inst)
            {
                continue;
            }
        }

        prPidStruct = _DMX_GetPidStruct(u1Idx);
        if (!prPidStruct->fgEnable)
        {
            continue;
        }

        //if(prPidStruct->eInputType != DMX_IN_PLAYBACK_PS)
        //{
        //    continue;
        //}

        _DMX_Lock();
        prPidStruct->u4NextPic = 0;
        prPidStruct->u1SerialNumber++;
        if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
        {
            prPidStruct->u1SerialNumber = 0;
        }

        ePidType = prPidStruct->ePidType;
        if ((ePidType == DMX_PID_TYPE_ES_AUDIO) ||
            (ePidType == DMX_PID_TYPE_ES_VIDEO))
        {
            UINT32 u4PhyAddr;

            PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
            PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

            u4PhyAddr = (UINT32)PID_S_W(u1Idx, 5);     // get buf_start
            PID_S_W(u1Idx, 7) = u4PhyAddr;     // PES_start = buf_start
            PID_S_W(u1Idx, 8) = u4PhyAddr;     // WP = buf_start
            PID_S_W(u1Idx, 9) = u4PhyAddr;     // RP = buf_start
            prPidStruct->u4Rp = VIRTUAL(u4PhyAddr);
            prPidStruct->u4SectionRp = VIRTUAL(u4PhyAddr);
            prPidStruct->u4PesRp = VIRTUAL(u4PhyAddr);
            prPidStruct->u4Wp = VIRTUAL(u4PhyAddr);

            u4PhyAddr = (UINT32)PID_S_W(u1Idx, 10);    // get buf_start
            PID_S_W(u1Idx, 12) = u4PhyAddr;    // (Header) Start = buf_start
            PID_S_W(u1Idx, 13) = u4PhyAddr;    // (Header) WP = buf_start
            PID_S_W(u1Idx, 14) = u4PhyAddr;    // (Header) RP = buf_start
        }
        _DMX_Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_SetAudioType
 *
 *  If the input type is DMX_IN_PLAYBACK_PS, then users need to specify the
 *  format of the audio input data.  The PS micro-code will filter the number
 *  of bytes specified in "PID word 2, byte 0" from the beginning of the PES
 *  payload if "PID word 0, bit 2" is set.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_SetAudioType(UINT8 u1Idx, DMX_AUDIO_TYPE_T eAudioType)
{
    BOOL fgRet;
    PID_STRUCT_T *prPidStruct;
    UINT8 u1Inst = 0;

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);

    // _DMX_Lock() and _DMX_Unlock() are not needed because a pointer is used.
    prPidStruct = _DMX_GetPidStruct(u1Idx);

    if (prPidStruct->ePidType != DMX_PID_TYPE_ES_AUDIO)
    {
        return FALSE;
    }

    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        u1Inst = _DMX_MUL_GetPidxInstId(u1Idx);
        if(u1Inst >= DMX_MAX_INSTANCE)
        {
            return FALSE;
        }
    }

    switch (eAudioType)
    {
    case DMX_AUDIO_MPEG:
        _DMX_Lock();
        _arPsManager[u1Inst].eAudioType = eAudioType;
        _arPsManager[u1Inst].u1AudioInfoLen = 0;
        _DMX_Unlock();
        fgRet = TRUE;
        break;
    case DMX_AUDIO_LPCM:
        _DMX_Lock();
        _arPsManager[u1Inst].eAudioType = eAudioType;
        _arPsManager[u1Inst].u1AudioInfoLen = 7;
        _DMX_Unlock();
        fgRet = TRUE;
        break;
    case DMX_AUDIO_AC3:
        _DMX_Lock();
        _arPsManager[u1Inst].eAudioType = eAudioType;
        _arPsManager[u1Inst].u1AudioInfoLen = 4;
        _DMX_Unlock();
        fgRet = TRUE;
        break;
	case DMX_AUDIO_DTS:
		_DMX_Lock();
        _arPsManager[u1Inst].eAudioType = eAudioType;
        _arPsManager[u1Inst].u1AudioInfoLen = 4;
        _DMX_Unlock();
        fgRet = TRUE;
        break;
    default:
        LOG(5, "L%d: Unknown PS audio type %d!\n", __LINE__, eAudioType);
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_GetAudioType
 *
 *  Called in HISR.
 */
//-----------------------------------------------------------------------------
DMX_AUDIO_TYPE_T _DMX_PS_CPU_GetAudioType(UINT8 u1Pidx)
{
    UINT8 u1Inst = 0;

    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        u1Inst = _DMX_MUL_GetPidxInstId(u1Pidx);
        if(u1Inst >= DMX_MAX_INSTANCE)
        {
            return DMX_AUDIO_UNKNOWN;
        }
    }

    return _arPsManager[u1Inst].eAudioType;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_SetDataFormat
 *
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_SetDataFormat(UINT8 u1Inst, DMX_DATA_FORMAT_T eDataFormat)
{
    BOOL fgRet;

    if(u1Inst >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    switch (eDataFormat)
    {
    case DMX_DATA_FORMAT_DAT:
        _DMX_Lock();
        //SET_PS_STATE_TO_START;
        _arPsManager[u1Inst].eDataFormat = eDataFormat;
        _DMX_Unlock();
        fgRet = TRUE;
        break;

    case DMX_DATA_FORMAT_RAW:
        _DMX_Lock();
        //SET_PS_STATE_TO_START;
        _arPsManager[u1Inst].eDataFormat = eDataFormat;
        _DMX_Unlock();
        fgRet = TRUE;
        break;

    default:
        LOG(5, "L%d: Unknown PS data format %d!\n", __LINE__, eDataFormat);
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_GetDataFormat
 */
//-----------------------------------------------------------------------------
DMX_DATA_FORMAT_T _DMX_PS_CPU_GetDataFormat(UINT8 u1Inst)
{
    if(u1Inst >= DMX_MAX_INSTANCE)
    {
        return DMX_DATA_FORMAT_UNKNOWN;
    }

    return _arPsManager[u1Inst].eDataFormat;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_GetExtraAudioInfo
 */
//-----------------------------------------------------------------------------
void _DMX_PS_CPU_GetExtraAudioInfo(UINT8 u1Idx, DMX_AUDIO_PES_T *prPes)
{
    UINT8 u1Inst = 0;

    if (prPes == NULL)
    {
        LOG(5, "L%d: null pointer!\n");
        return;
    }

    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        u1Inst = _DMX_MUL_GetPidxInstId(u1Idx);
        if(u1Inst >= DMX_MAX_INSTANCE)
        {
            return;
        }
    }

    prPes->u4Info = _arPsManager[u1Inst].u4AudioInfo;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_Init(void)
{
    _u1PsActive = 0;
    x_memset((void*)_arPsManager, 0, sizeof(_arPsManager));
#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
    _fgStarted = FALSE;
    _fgEnded = FALSE;
    _u4StreamIdNumber = 0;
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_SetVideoHeader
*   The EOS flag shall be always set to FALSE in this function.
*   Call by HISR to get PTS information
*/
//-----------------------------------------------------------------------------
void _DMX_PS_CPU_SetVideoHeader(UINT8 u1Idx, DMX_PES_MSG_T *prPes)
{
    PID_STRUCT_T *prPidStruct;

    if (prPes == NULL)
    {
        LOG(3, "L%d: null pointer\n", __LINE__);
        return;
    }

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);
#if 1
    //not use the DMX_PS_MoveDataQueue in MT5396, so we should use _arPsManager struct to check stream ID and PTS.
    if (prPidStruct->u1StreamID != _arPSMoveDataCmd[u1Idx].u1StreamID)
    {
        LOG(3, "L%d: unmatched index!\n", __LINE__);
        return;
    }

    prPes->u4Pts= _arPSMoveDataCmd[u1Idx].u4Pts;
    prPes->u4Dts= _arPSMoveDataCmd[u1Idx].u4Dts;
#else
    if (prPidStruct->u1StreamID != _arPsManager[_u1PsActive].u1StreamID)
    {
        LOG(3, "L%d: unmatched index!\n", __LINE__);
        return;
    }

    prPes->u4Pts = (UINT32)(_arPsManager[_u1PsActive].u8Pts & 0xFFFFFFFF);
    prPes->u4Dts = (UINT32)(_arPsManager[_u1PsActive].u8Dts & 0xFFFFFFFF);
#endif    
    prPes->fgPtsDts = TRUE;

}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_SetAudioHeader
*   The EOS flag shall be always set to FALSE in this function.
*   Call by HISR to get PTS information
*/
//-----------------------------------------------------------------------------
void _DMX_PS_CPU_SetAudioHeader(UINT8 u1Idx, DMX_AUDIO_PES_T *prPes)
{
    PID_STRUCT_T *prPidStruct;

    if (prPes == NULL)
    {
        LOG(3, "L%d: null pointer\n", __LINE__);
        return;
    }

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);
    if (prPidStruct->u1StreamID != _arPsManager[_u1PsActive].u1StreamID)
    {
        LOG(3, "L%d: unmatched index!\n", __LINE__);
        return;
    }

    prPes->u4Pts = (UINT32)(_arPsManager[_u1PsActive].u8Pts & 0xFFFFFFFF);
    prPes->u4Dts = (UINT32)(_arPsManager[_u1PsActive].u8Dts & 0xFFFFFFFF);
    prPes->fgCopyright = _arPsManager[_u1PsActive].fgCopyright;
    prPes->fgOriginal = _arPsManager[_u1PsActive].fgOriginal;
}

#ifdef CC_DMX_HDCP
//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_SetHDCPKeyIv
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_SetHDCPKeyIv(UINT8 u1Inst, UINT8 *pu1Key, UINT8 *pu1Iv)
{
    if(u1Inst >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    x_memcpy(_arPsManager[u1Inst].au1Key, pu1Key, PS_HDCP_KEY_LEN);
    // only fill out the first 64 bit riv
    x_memcpy(_arPsManager[u1Inst].au1Iv, pu1Iv, PS_HDCP_KEY_LEN / 2);
    _arPsManager[u1Inst].fgHDCP20 = TRUE;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_HDCP_Decrypt
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_HDCP_Decrypt(UINT32 u4Src, UINT32 u4Len)
{
    AES_PARAM_T rAesParam;

    x_memcpy(rAesParam.au1Key, _arPsManager[_u1PsActive].au1Key, PS_HDCP_KEY_LEN);
    x_memcpy(rAesParam.au1Iv, _arPsManager[_u1PsActive].au1Iv, PS_HDCP_IV_LEN);
    rAesParam.eKeyBitLen = KEY_BIT_LEN_128;
    rAesParam.eMode = BLOCK_CIPHER_MODE_CTR;
    rAesParam.fgEncrypt = FALSE;

    rAesParam.u4SrcStartAddr = (UINT32) u4Src;
    rAesParam.u4SrcBufStart = (UINT32) _arPsManager[_u1PsActive].u4BufStart;
    rAesParam.u4SrcBufEnd = (UINT32) _arPsManager[_u1PsActive].u4BufEnd;
    rAesParam.u4DstStartAddr = (UINT32) u4Src;
    rAesParam.u4DstBufStart = (UINT32) _arPsManager[_u1PsActive].u4BufStart;
    rAesParam.u4DstBufEnd = (UINT32) _arPsManager[_u1PsActive].u4BufEnd;
    rAesParam.u4DatLen  = u4Len;

    if(GCPU_Cmd(0, GCPU_AES, &rAesParam) == S_GCPU_OK)
    {
        // update initial vector back
        x_memcpy(_arPsManager[_u1PsActive].au1Iv, rAesParam.au1Iv, PS_HDCP_IV_LEN);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_GetByte
 *  Read the byte at _rPsManager.u4BufRp, and increment _rPsManager.u4BufRp.
 *  Append the byte to _rPsManager.u4LastWord, and increment
 *  _rPsManager.u4LastBytesCounter if needed.
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_GetByte(UINT8 *pu1Data)
{
    ASSERT(pu1Data != NULL);
    *pu1Data = 0xFF;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT((_arPsManager[_u1PsActive].u4BufStart <= _arPsManager[_u1PsActive].u4BufRp) &&
            (_arPsManager[_u1PsActive].u4BufRp < _arPsManager[_u1PsActive].u4BufEnd));

    if (_arPsManager[_u1PsActive].u4DataSize == 0)
    {
        return FALSE;
    }

    if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_DAT)
    {
        if ((_arPsManager[_u1PsActive].u4RemainingSectorLen == 0) ||
            (_arPsManager[_u1PsActive].u4RemainingPesLen == 0))
        {
            SET_PS_STATE_TO_START;
            return FALSE;
        }
        _arPsManager[_u1PsActive].u4RemainingSectorLen--;
        _arPsManager[_u1PsActive].u4RemainingPesLen--;
    }
    else if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_RAW)
    {
        if (_arPsManager[_u1PsActive].u4RemainingPesLen == 0)
        {
            SET_PS_STATE_TO_START;
            return FALSE;
        }
        _arPsManager[_u1PsActive].u4RemainingPesLen--;
    }
    else
    {
        SET_PS_STATE_TO_START;
        LOG(5, "L%d: unknown data format!\n", __LINE__);
        return FALSE;
    }

    *pu1Data = *(UINT8*)_arPsManager[_u1PsActive].u4BufRp;
    _arPsManager[_u1PsActive].u4DataSize--;
    _arPsManager[_u1PsActive].u4ByteProcessed++;
    _arPsManager[_u1PsActive].u4BufRp++;
    if (_arPsManager[_u1PsActive].u4BufRp >= _arPsManager[_u1PsActive].u4BufEnd)
    {
        _arPsManager[_u1PsActive].u4BufRp -= _arPsManager[_u1PsActive].u4BufSize;
    }

    // Update the value in u4LastWord.
    _arPsManager[_u1PsActive].u4LastWord = (_arPsManager[_u1PsActive].u4LastWord << 8) | *pu1Data;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_PeekByte
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_PeekByte(UINT8 *pu1Data)
{
    ASSERT(pu1Data != NULL);
    *pu1Data = 0xFF;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT((_arPsManager[_u1PsActive].u4BufStart <= _arPsManager[_u1PsActive].u4BufRp) &&
            (_arPsManager[_u1PsActive].u4BufRp < _arPsManager[_u1PsActive].u4BufEnd));

    if (_arPsManager[_u1PsActive].u4DataSize == 0)
    {
        return FALSE;
    }

    if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_DAT)
    {
        if ((_arPsManager[_u1PsActive].u4RemainingSectorLen == 0) ||
            (_arPsManager[_u1PsActive].u4RemainingPesLen == 0))
        {
            SET_PS_STATE_TO_START;
            return FALSE;
        }
    }
    else if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_RAW)
    {
        if (_arPsManager[_u1PsActive].u4RemainingPesLen == 0)
        {
            SET_PS_STATE_TO_START;
            return FALSE;
        }
    }
    else
    {
        SET_PS_STATE_TO_START;
        LOG(5, "L%d: unknown data format!\n", __LINE__);
        return FALSE;
    }

    *pu1Data = *(UINT8*)_arPsManager[_u1PsActive].u4BufRp;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_SkipBytes
 */
//-----------------------------------------------------------------------------
static UINT32 _Dmx_PS_CPU_SkipBytes(UINT32 u4LenToSkip)
{
    UINT32 u4DataSize;
    UINT32 u4BufSize;
    UINT32 u4SkippedLen = 0;
    UINT32 u4BufRp;
    UINT8 u1TailLen, u1Offset;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return 0;
    }

    u4DataSize = _arPsManager[_u1PsActive].u4DataSize;
    u4BufSize = _arPsManager[_u1PsActive].u4BufSize;

    ASSERT((_arPsManager[_u1PsActive].u4BufStart <= _arPsManager[_u1PsActive].u4BufRp) &&
            (_arPsManager[_u1PsActive].u4BufRp < _arPsManager[_u1PsActive].u4BufEnd));

    if (u4DataSize == 0)
    {
        return 0;
    }

    if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_DAT)
    {
        UINT32 u4MinRemainingLen;

        if ((_arPsManager[_u1PsActive].u4RemainingSectorLen == 0) ||
            (_arPsManager[_u1PsActive].u4RemainingPesLen == 0))
        {
            SET_PS_STATE_TO_START;
            return 0;
        }

        u4MinRemainingLen = MIN(_arPsManager[_u1PsActive].u4RemainingSectorLen,
                                _arPsManager[_u1PsActive].u4RemainingPesLen);

        if (u4MinRemainingLen < u4LenToSkip)
        {
            u4LenToSkip = u4MinRemainingLen;
        }

        u4SkippedLen = (u4DataSize < u4LenToSkip) ? u4DataSize : u4LenToSkip;
        _arPsManager[_u1PsActive].u4RemainingSectorLen -= u4SkippedLen;
        _arPsManager[_u1PsActive].u4RemainingPesLen -= u4SkippedLen;
    }
    else if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_RAW)
    {
        if (_arPsManager[_u1PsActive].u4RemainingPesLen == 0)
        {
            SET_PS_STATE_TO_START;
            return 0;
        }

        if (_arPsManager[_u1PsActive].u4RemainingPesLen < u4LenToSkip)
        {
            u4LenToSkip = _arPsManager[_u1PsActive].u4RemainingPesLen;
        }

        u4SkippedLen = (u4DataSize < u4LenToSkip) ? u4DataSize : u4LenToSkip;

        #ifdef CC_DMX_HDCP
        if(_arPsManager[_u1PsActive].fgHDCP20)
        {
            // need to check how much data will store in previous buf
            UINT32 u4RemaingSize = 0;
            UINT32 u4Rp;

            u4RemaingSize = (u4SkippedLen % 16);
            if((u4RemaingSize != 0) &&
                (u4LenToSkip < 16) && (_arPsManager[_u1PsActive].u4PreviousBufSize != 0))
            {
                // this case is when previous buf has data
                // and we want to accumulate 16 bytes for processing
                u4Rp = _arPsManager[_u1PsActive].u4BufRp;
                UNUSED(_DMX_CopyRingBuffer((UINT32)(_arPsManager[_u1PsActive].au1PreviousBuf +
                    _arPsManager[_u1PsActive].u4PreviousBufSize),
                    (UINT32)(_arPsManager[_u1PsActive].au1PreviousBuf),
                    (UINT32)(_arPsManager[_u1PsActive].au1PreviousBuf + 32),
                    u4Rp, _arPsManager[_u1PsActive].u4BufStart, _arPsManager[_u1PsActive].u4BufEnd, u4RemaingSize));
                _arPsManager[_u1PsActive].u4PreviousBufSize += u4RemaingSize;
            }
            else if((u4RemaingSize != 0) &&
                (_arPsManager[_u1PsActive].u4RemainingPesLen - u4SkippedLen > 0))
            {
                ASSERT(_arPsManager[_u1PsActive].u4PreviousBufSize == 0);
                // this case is small piece of data in the end of buffer
                u4Rp = ADDR_INCR_IN_RING(_arPsManager[_u1PsActive].u4BufRp,
                    u4SkippedLen - u4RemaingSize, _arPsManager[_u1PsActive].u4BufStart,
                    _arPsManager[_u1PsActive].u4BufEnd);
                UNUSED(_DMX_CopyRingBuffer((UINT32)(_arPsManager[_u1PsActive].au1PreviousBuf),
                    (UINT32)(_arPsManager[_u1PsActive].au1PreviousBuf),
                    (UINT32)(_arPsManager[_u1PsActive].au1PreviousBuf + 32),
                    u4Rp, _arPsManager[_u1PsActive].u4BufStart, _arPsManager[_u1PsActive].u4BufEnd,
                    u4RemaingSize));
                _arPsManager[_u1PsActive].u4PreviousBufSize += u4RemaingSize;
            }
            else
            {
                _arPsManager[_u1PsActive].u4PreviousBufSize = 0;
            }
        }
        #endif
        _arPsManager[_u1PsActive].u4RemainingPesLen -= u4SkippedLen;
    }
    else
    {
        SET_PS_STATE_TO_START;
        LOG(5, "L%d: unknown data format!\n", __LINE__);
        return 0;
    }

    _arPsManager[_u1PsActive].u4DataSize -= u4SkippedLen;
    _arPsManager[_u1PsActive].u4ByteProcessed += u4SkippedLen;
    _arPsManager[_u1PsActive].u4BufRp += u4SkippedLen;
    if (_arPsManager[_u1PsActive].u4BufRp >= _arPsManager[_u1PsActive].u4BufEnd)
    {
        _arPsManager[_u1PsActive].u4BufRp -= u4BufSize;
    }

    u1TailLen = (u4SkippedLen < 4) ? (UINT8)u4SkippedLen : 4;
    u4BufRp = _arPsManager[_u1PsActive].u4BufRp - u1TailLen;
    u4BufRp = (u4BufRp < _arPsManager[_u1PsActive].u4BufStart) ? (u4BufRp + u4BufSize) : u4BufRp;

    for (u1Offset = 0; u1Offset < u1TailLen; u1Offset++, u4BufRp++)
    {
        if (u4BufRp >= _arPsManager[_u1PsActive].u4BufEnd)
        {
            u4BufRp -= u4BufSize;
        }
        // Update the value in u4LastWord.
        _arPsManager[_u1PsActive].u4LastWord = (_arPsManager[_u1PsActive].u4LastWord << 8) | *(UINT8*)u4BufRp;
    }

    return u4SkippedLen;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_CopyToHeaderBuffer
 */
//-----------------------------------------------------------------------------
static UINT32 _Dmx_PS_CPU_CopyToHeaderBuffer(UINT32 u4LenToCopy)
{
    UINT32 u4DataSize;
    UINT32 u4Src, u4SrcStart, u4SrcEnd;
    UINT32 u4Dst, u4DstStart, u4DstEnd;
    UINT32 u4CopyLen = 0;
    UINT32 u4BufSize;
    UINT32 u4BufRp;
    UINT8 u1TailLen, u1Offset;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return 0;
    }

    u4DataSize = _arPsManager[_u1PsActive].u4DataSize;
    u4BufSize = _arPsManager[_u1PsActive].u4BufSize;

    ASSERT((_arPsManager[_u1PsActive].u4BufStart <= _arPsManager[_u1PsActive].u4BufRp) &&
            (_arPsManager[_u1PsActive].u4BufRp < _arPsManager[_u1PsActive].u4BufEnd));

    if (u4DataSize == 0)
    {
        return 0;
    }

    u4Src = _arPsManager[_u1PsActive].u4BufRp;
    u4SrcStart = _arPsManager[_u1PsActive].u4BufStart;
    u4SrcEnd = _arPsManager[_u1PsActive].u4BufEnd;

    u4Dst = (UINT32)_arPsManager[_u1PsActive].au1PesHeader + _arPsManager[_u1PsActive].u2PesHeaderCopyLen;
    u4DstStart = (UINT32)_arPsManager[_u1PsActive].au1PesHeader;
    u4DstEnd = (UINT32)_arPsManager[_u1PsActive].au1PesHeader + PS_PES_HEADER_TMP_BUFFER;

    if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_DAT)
    {
        UINT32 u4MinRemainingLen;

        if ((_arPsManager[_u1PsActive].u4RemainingSectorLen == 0) ||
            (_arPsManager[_u1PsActive].u4RemainingPesLen == 0))
        {
            SET_PS_STATE_TO_START;
            return 0;
        }

        u4MinRemainingLen = MIN(_arPsManager[_u1PsActive].u4RemainingSectorLen,
                                _arPsManager[_u1PsActive].u4RemainingPesLen);

        if (u4MinRemainingLen < u4LenToCopy)
        {
            u4LenToCopy = u4MinRemainingLen;
        }

        u4CopyLen = (u4DataSize < u4LenToCopy) ? u4DataSize : u4LenToCopy;
        _arPsManager[_u1PsActive].u4RemainingSectorLen -= u4CopyLen;
        _arPsManager[_u1PsActive].u4RemainingPesLen -= u4CopyLen;
    }
    else if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_RAW)
    {
        if (_arPsManager[_u1PsActive].u4RemainingPesLen == 0)
        {
            SET_PS_STATE_TO_START;
            return 0;
        }

        if (_arPsManager[_u1PsActive].u4RemainingPesLen < u4LenToCopy)
        {
            u4LenToCopy = _arPsManager[_u1PsActive].u4RemainingPesLen;
        }

        u4CopyLen = (u4DataSize < u4LenToCopy) ? u4DataSize : u4LenToCopy;
        _arPsManager[_u1PsActive].u4RemainingPesLen -= u4CopyLen;
    }
    else
    {
        SET_PS_STATE_TO_START;
        LOG(5, "L%d: unknown data format!\n", __LINE__);
        return 0;
    }

    if ((_arPsManager[_u1PsActive].u2PesHeaderCopyLen + u4CopyLen) >= PS_PES_HEADER_TMP_BUFFER)
    {
        SET_PS_STATE_TO_START;
        LOG(5, "PES Header length %d >= %d\n",
            (_arPsManager[_u1PsActive].u2PesHeaderCopyLen + u4CopyLen),
            PS_PES_HEADER_TMP_BUFFER);
        return 0;
    }

    _DMX_CopyRingBuffer(u4Dst, u4DstStart, u4DstEnd,
                        u4Src, u4SrcStart, u4SrcEnd, u4CopyLen);

    _arPsManager[_u1PsActive].u2PesHeaderCopyLen += u4CopyLen;
    _arPsManager[_u1PsActive].u4DataSize -= u4CopyLen;
    _arPsManager[_u1PsActive].u4ByteProcessed += u4CopyLen;
    _arPsManager[_u1PsActive].u4BufRp += u4CopyLen;
    if (_arPsManager[_u1PsActive].u4BufRp >= _arPsManager[_u1PsActive].u4BufEnd)
    {
        _arPsManager[_u1PsActive].u4BufRp -= u4BufSize;
    }

    u1TailLen = (u4CopyLen < 4) ? (UINT8)u4CopyLen : 4;
    u4BufRp = _arPsManager[_u1PsActive].u4BufRp - u1TailLen;
    u4BufRp = (u4BufRp < _arPsManager[_u1PsActive].u4BufStart) ? (u4BufRp + u4BufSize) : u4BufRp;

    for (u1Offset = 0; u1Offset < u1TailLen; u1Offset++, u4BufRp++)
    {
        if (u4BufRp >= _arPsManager[_u1PsActive].u4BufEnd)
        {
            u4BufRp -= u4BufSize;
        }
        // Update the value in u4LastWord.
        _arPsManager[_u1PsActive].u4LastWord = (_arPsManager[_u1PsActive].u4LastWord << 8) | *(UINT8*)u4BufRp;
    }

    return u4CopyLen;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_MoveToNextState
 */
//-----------------------------------------------------------------------------
BOOL _Dmx_PS_CPU_MoveToNextState(void)
{
    BOOL fgRet = TRUE;
    PS_SEARCH_STATE_T eSearchState;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    eSearchState = _arPsManager[_u1PsActive].eSearchState;

    ASSERT(eSearchState < PS_STATE_SEQ_END);

    if (eSearchState == PS_STATE_START)
    {
        _arPsManager[_u1PsActive].u4RemainingPesLen = 0xFFFFFFFF;
        if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_DAT)
        {
            if (_arPsManager[_u1PsActive].u4RemainingSectorLen == 0)
            {
                _arPsManager[_u1PsActive].u4RemainingSectorLen = 0xFFFFFFFF;
                _arPsManager[_u1PsActive].eSearchState = PS_STATE_SYNC_MARKER;
            }
            else
            {
                _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_START_PREFIX;
            }
        }
        else if (_arPsManager[_u1PsActive].eDataFormat == DMX_DATA_FORMAT_RAW)
        {
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_START_PREFIX;
        }
        else
        {
            SET_PS_STATE_TO_START;
            LOG(5, "L%d: unknown data format!\n", __LINE__);
            fgRet = FALSE;
        }
    }
    else if (eSearchState == PS_STATE_SYNC_MARKER)
    {
        _arPsManager[_u1PsActive].eSearchState = PS_STATE_VCD_SUBHEADER;
    }
    else if (eSearchState == PS_STATE_VCD_SUBHEADER)
    {
        //_arPsManager[_u1PsActive].u4RemainingPesLen = 0xFFFFFFFF;  // already set above
        _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_START_PREFIX;
    }
    else if (eSearchState == PS_STATE_PES_START_PREFIX)
    {
        _arPsManager[_u1PsActive].u2PesLen = 0;
        _arPsManager[_u1PsActive].u1PesHeaderExtraLen = 0;
        _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_LEN;
    }
    else if (eSearchState == PS_STATE_PES_LEN)
    {
        UINT8 u1StreamID = _arPsManager[_u1PsActive].u1StreamID;

        // FIXME: Process PS_SID_SEQUENCE_START and PS_SID_SEQUENCE_END

        if (u1StreamID <= PS_SID_PACK_START_CODE)
        {
            _arPsManager[_u1PsActive].u4RemainingPesLen = 0xFFFFFFFF;
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_START_PREFIX;
        }
        else if (u1StreamID == PS_SID_SYSTEM_HEADER_START_CODE)
        {
            // Treat PS_system_header as padding info.
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_PADDING;
        }
        else if ((u1StreamID == PS_SID_PROGRAM_STREAM_MAP)      ||
                 (u1StreamID == PS_SID_PRIVATE_STREAM_2)        ||
                 (u1StreamID == PS_SID_ECM_STREAM)              ||
                 (u1StreamID == PS_SID_EMM_STREAM)              ||
                 (u1StreamID == PS_SID_PROGRAM_STREAM_DIRECTORY)||
                 (u1StreamID == PS_SID_DSMCC_STREAM)            ||
                 (u1StreamID == PS_SID_H222_1_E_STREAM))
        {
            _arPsManager[_u1PsActive].u1PesHeaderExtraLen = 0;
            _arPsManager[_u1PsActive].fgAudioInfoCopied = FALSE;
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_PAYLOAD;
        }
        else if (u1StreamID == PS_SID_PADDING_STREAM)
        {
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_PADDING;
        }
        else
        {
            _arPsManager[_u1PsActive].u1PesHeaderExtraLen = 3;
            _arPsManager[_u1PsActive].eStreamType = STREAM_UNKNOWN;
            _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_STUFFING;
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_STREAM_TYPE;
        }
    }
    else if (_arPsManager[_u1PsActive].eSearchState == PS_STATE_STREAM_TYPE)
    {
        if (_arPsManager[_u1PsActive].eStreamType == STREAM_MPEG1)
        {
            _arPsManager[_u1PsActive].u1PesHeaderLen = 0;
            _arPsManager[_u1PsActive].u2PesHeaderCopyLen = 0;
            _arPsManager[_u1PsActive].u8Pts = 0;
            _arPsManager[_u1PsActive].u8Dts = 0;
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_HEADER;
        }
        else if (_arPsManager[_u1PsActive].eStreamType == STREAM_MPEG2)
        {
#if 0
            // Make sure that PES_length is at least 3 bytes long for
            // the flags (2 bytes) and the header_length (1 byte) fields.
            if (_arPsManager[_u1PsActive].u2PesLen < 3)
            {
                SET_PS_STATE_TO_START;
                LOG(5, "MPEG-2 PES length %d < 3\n", _arPsManager[_u1PsActive].u2PesLen);
                fgRet = FALSE;
            }
            else
#endif  // 0
            {
                //_arPsManager[_u1PsActive].u1ScramblingCtrl = 0;
                //_arPsManager[_u1PsActive].fgPriority       = FALSE;
                //_arPsManager[_u1PsActive].fgAlignment      = FALSE;
                _arPsManager[_u1PsActive].fgCopyright      = FALSE;
                _arPsManager[_u1PsActive].fgOriginal       = FALSE;
                _arPsManager[_u1PsActive].u1PtsDtsFlags    = 0;
                _arPsManager[_u1PsActive].fgEscr           = FALSE;
                _arPsManager[_u1PsActive].fgEsRate         = FALSE;
                _arPsManager[_u1PsActive].fgTrick          = FALSE;
                _arPsManager[_u1PsActive].fgCopyInfo       = FALSE;
                _arPsManager[_u1PsActive].fgPesCrc         = FALSE;
                _arPsManager[_u1PsActive].fgExt            = FALSE;
                _arPsManager[_u1PsActive].u1PesHeaderLen   = 0;
                _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_FLAGS;
            }
        }
        else
        {
            SET_PS_STATE_TO_START;
            LOG(5, "L%d: unknown stream type!\n", __LINE__);
            fgRet = FALSE;
        }
    }
    else if (eSearchState == PS_STATE_PES_FLAGS)
    {
        _arPsManager[_u1PsActive].u2PesHeaderCopyLen = 0;
        _arPsManager[_u1PsActive].u8Pts = 0;
        _arPsManager[_u1PsActive].u8Dts = 0;
        _arPsManager[_u1PsActive].eSearchState = PS_STATE_PES_HEADER;
    }
    else if (eSearchState == PS_STATE_PES_HEADER)
    {
        _arPsManager[_u1PsActive].fgAudioInfoCopied = FALSE;
        _arPsManager[_u1PsActive].fgFirstByteOfPayload = TRUE;
        _arPsManager[_u1PsActive].eSearchState = PS_STATE_PAYLOAD;
    }
    else if (eSearchState == PS_STATE_PAYLOAD)
    {
        SET_PS_STATE_TO_START;
    }
    else if (eSearchState == PS_STATE_PADDING)
    {
        SET_PS_STATE_TO_START;
    }
    else
    {
        LOG(5, "Unknown state transition %d!\n", eSearchState);
        fgRet = FALSE;
    }

    // Clear some internal variables.
    _arPsManager[_u1PsActive].u2Counter = 0;
//    _arPsManager[_u1PsActive].u4LastWord = 0xFFFFFFFF;
//    _arPsManager[_u1PsActive].u1LastWordLen = 0;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateStart
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateStart(void)
{
    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_START);

    return _Dmx_PS_CPU_MoveToNextState();
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateSyncMarker
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateSyncMarker(void)
{
    static UINT32 u4AdvanceSize = 0;  // for debugging
    const UINT32 u4CheckSize = 4096;  // for debugging
    BOOL fgFoundSyncMarker = FALSE;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_SYNC_MARKER);

    while (!fgFoundSyncMarker)
    {
        UINT8 u1Data;

        if (_arPsManager[_u1PsActive].u2Counter >= PS_FIELD_LEN_SYNC_MARKER)
        {
            fgFoundSyncMarker = TRUE;
            u4AdvanceSize = 0;
            UNUSED(_Dmx_PS_CPU_MoveToNextState());
            break;
        }

        if (!_Dmx_PS_CPU_GetByte(&u1Data))
        {
            break;
        }

        u4AdvanceSize++;

        if (u1Data == _arSyncMarker[_arPsManager[_u1PsActive].u2Counter])
        {
            _arPsManager[_u1PsActive].u2Counter++;
        }
        else
        {
            if (u1Data == _arSyncMarker[0])
            {
                _arPsManager[_u1PsActive].u2Counter = 1;
            }
            else
            {
                _arPsManager[_u1PsActive].u2Counter = 0;
            }
        }
    }

    if (u4AdvanceSize > u4CheckSize)
    {
        LOG(7, "Debug: No Sync Marker is found in %d bytes!\n", u4CheckSize);
    }

    return fgFoundSyncMarker;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateVcdSubheader
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateVcdSubheader(void)
{
    UINT16 u2SkipLen;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_VCD_SUBHEADER);

    ASSERT(PS_FIELD_LEN_VCD_SUBHEADER >= _arPsManager[_u1PsActive].u2Counter);
    u2SkipLen = PS_FIELD_LEN_VCD_SUBHEADER - _arPsManager[_u1PsActive].u2Counter;
    _arPsManager[_u1PsActive].u2Counter += _Dmx_PS_CPU_SkipBytes(u2SkipLen);
    if (_arPsManager[_u1PsActive].u2Counter >= PS_FIELD_LEN_VCD_SUBHEADER)
    {
        _arPsManager[_u1PsActive].u4RemainingSectorLen = 2324;  // ignore 4-byte EDC
        UNUSED(_Dmx_PS_CPU_MoveToNextState());
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StatePesStartPrefix
 *  Get packet_start_code_prefix and stream_id.
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StatePesStartPrefix(void)
{
    UINT8 u1Data;
    BOOL fgRet = FALSE;
#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
    UINT32 u4StartCount = _arPsManager[_u1PsActive].u4ByteProcessed, u4EndCount;
    static UINT32 u4ConsumedLen = 0;
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_PES_START_PREFIX);

    while (_Dmx_PS_CPU_GetByte(&u1Data))
    {
        //_arPsManager[_u1PsActive].u4LastWord = (_arPsManager[_u1PsActive].u4LastWord << 8) | u1Data;
        if ((_arPsManager[_u1PsActive].u4LastWord & 0xFFFFFF00) == 0x00000100)
        {
#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
            if (!_fgEnded)
            {
                _u4StreamIdNumber++;
            }
            u4ConsumedLen = 0;
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
            _arPsManager[_u1PsActive].u1StreamID = _arPsManager[_u1PsActive].u4LastWord & 0xFF;
            UNUSED(_Dmx_PS_CPU_MoveToNextState());
            fgRet = TRUE;
            break;
        }
    }

#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
    if (!fgRet)
    {
        u4EndCount = _arPsManager[_u1PsActive].u4ByteProcessed;
        u4ConsumedLen += u4EndCount - u4StartCount;
        if (u4ConsumedLen > PS_MAX_LEN_WITHOUT_PREFIX)
        {
            u4ConsumedLen = 0;
            x_thread_delay(1);
        }
    }
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD

    return fgRet;
}


#if 0
//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateSystemHeader
 *
 *  This function is empty because the "PS system header" is not used in the
 *  current implementation.
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateSystemHeader(void)
{
    //UINT16 u2FieldLen = 0xFFFF;  // variable length

    return TRUE;
}
#endif  // 0


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StatePesLen
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StatePesLen(void)
{
    UINT8 u1Data;
    BOOL fgRet = FALSE;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_PES_LEN);

    while (_Dmx_PS_CPU_GetByte(&u1Data))
    {
        _arPsManager[_u1PsActive].u2Counter++;
        //_arPsManager[_u1PsActive].u4LastWord = (_arPsManager[_u1PsActive].u4LastWord << 8) | u1Data;
        if (_arPsManager[_u1PsActive].u2Counter >= PS_FIELD_LEN_PES_LEN)
        {
            _arPsManager[_u1PsActive].u2PesLen = (_arPsManager[_u1PsActive].u4LastWord & 0xFFFF);
            _arPsManager[_u1PsActive].u4RemainingPesLen = _arPsManager[_u1PsActive].u2PesLen;
            UNUSED(_Dmx_PS_CPU_MoveToNextState());
            fgRet = TRUE;
            break;
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateStreamType
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateStreamType(void)
{
    UINT8 u1Data;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_STREAM_TYPE);

    if (!_Dmx_PS_CPU_PeekByte(&u1Data))
    {
        return FALSE;
    }

    if ((u1Data & 0xC0) == 0x80)
    {
        _arPsManager[_u1PsActive].eStreamType = STREAM_MPEG2;
    }
    else
    {
        _arPsManager[_u1PsActive].eStreamType = STREAM_MPEG1;
    }

    UNUSED(_Dmx_PS_CPU_MoveToNextState());

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StatePesFlags
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StatePesFlags(void)
{
    UINT8 u1Data;
    BOOL fgRet = FALSE;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_PES_FLAGS);
    ASSERT(_arPsManager[_u1PsActive].eStreamType == STREAM_MPEG2);

    while (_Dmx_PS_CPU_GetByte(&u1Data))
    {
        _arPsManager[_u1PsActive].u2Counter++;
        //_arPsManager[_u1PsActive].u4LastWord = (_arPsManager[_u1PsActive].u4LastWord << 8) | u1Data;
        if (_arPsManager[_u1PsActive].u2Counter >= PS_FIELD_LEN_PES_FLAGS)
        {
            //_arPsManager[_u1PsActive].u1ScramblingCtrl = (_arPsManager[_u1PsActive].u4LastWord & 0x300000) >> 20;
            //_arPsManager[_u1PsActive].fgPriority     = (_arPsManager[_u1PsActive].u4LastWord & 0x080000) >> 19;
            //_arPsManager[_u1PsActive].fgAlignment    = (_arPsManager[_u1PsActive].u4LastWord & 0x040000) >> 18;
            _arPsManager[_u1PsActive].fgCopyright    = (_arPsManager[_u1PsActive].u4LastWord & 0x020000) >> 17;
            _arPsManager[_u1PsActive].fgOriginal     = (_arPsManager[_u1PsActive].u4LastWord & 0x010000) >> 16;
            _arPsManager[_u1PsActive].u1PtsDtsFlags  = (_arPsManager[_u1PsActive].u4LastWord & 0x00C000) >> 14;
            _arPsManager[_u1PsActive].fgEscr         = (_arPsManager[_u1PsActive].u4LastWord & 0x002000) >> 13;
            _arPsManager[_u1PsActive].fgEsRate       = (_arPsManager[_u1PsActive].u4LastWord & 0x001000) >> 12;
            _arPsManager[_u1PsActive].fgTrick        = (_arPsManager[_u1PsActive].u4LastWord & 0x000800) >> 11;
            _arPsManager[_u1PsActive].fgCopyInfo     = (_arPsManager[_u1PsActive].u4LastWord & 0x000400) >> 10;
            _arPsManager[_u1PsActive].fgPesCrc       = (_arPsManager[_u1PsActive].u4LastWord & 0x000200) >>  9;
            _arPsManager[_u1PsActive].fgExt          = (_arPsManager[_u1PsActive].u4LastWord & 0x000100) >>  8;
            _arPsManager[_u1PsActive].u1PesHeaderLen = _arPsManager[_u1PsActive].u4LastWord & 0x0000FF;
            UNUSED(_Dmx_PS_CPU_MoveToNextState());
            fgRet = TRUE;
            break;
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_ParseMpeg1Header
 */
//-----------------------------------------------------------------------------
static void _Dmx_PS_CPU_ParseMpeg1Header(void)
{
    UINT64 u8Pts, u8Dts;
    UINT8 u1Offset, u1Data;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return;
    }

    for (u1Offset = 0; u1Offset < _arPsManager[_u1PsActive].u1PesHeaderLen; u1Offset++)
    {
        u1Data = _arPsManager[_u1PsActive].au1PesHeader[u1Offset];
        if ((u1Data & 0xF0) == 0x20)
        {
            if ((u1Offset + 4) < _arPsManager[_u1PsActive].u1PesHeaderLen)
            {
                u8Pts = u1Data;
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                _arPsManager[_u1PsActive].u8Pts =
                    (((u8Pts >> 33) &    0x7) << 30) |
                    (((u8Pts >> 17) & 0x7FFF) << 15) |
                    (((u8Pts >>  1) & 0x7FFF) <<  0);
            }
            else
            {
                LOG(5, "L%d: insufficient header length\n", __LINE__);
            }
            break;
        }
        else if ((u1Data & 0xF0) == 0x30)
        {
            if ((u1Offset + 9) < _arPsManager[_u1PsActive].u1PesHeaderLen)
            {
                u8Pts = _arPsManager[_u1PsActive].au1PesHeader[0];
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Pts = (u8Pts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                _arPsManager[_u1PsActive].u8Pts =
                    (((u8Pts >> 33) &    0x7) << 30) |
                    (((u8Pts >> 17) & 0x7FFF) << 15) |
                    (((u8Pts >>  1) & 0x7FFF) <<  0);

                u8Dts = _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Dts = (u8Dts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Dts = (u8Dts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Dts = (u8Dts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                u8Dts = (u8Dts << 8) | _arPsManager[_u1PsActive].au1PesHeader[++u1Offset];
                _arPsManager[_u1PsActive].u8Dts =
                    (((u8Dts >> 33) &    0x7) << 30) |
                    (((u8Dts >> 17) & 0x7FFF) << 15) |
                    (((u8Dts >>  1) & 0x7FFF) <<  0);
            }
            else
            {
                LOG(5, "L%d: insufficient header length\n", __LINE__);
            }
            break;
        }
    }
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_ParseMpeg2Header
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_ParseMpeg2Header(void)
{
    UINT8 u1PesHeaderLen;
    UINT8 u1PesHeaderRp = 0;
    UINT8 *pu1PesHeader;
    #ifdef CC_DMX_HDCP
    UINT32 u4StreamCtr;
    UINT64 u8InputCtr;
    #endif

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    u1PesHeaderLen = _arPsManager[_u1PsActive].u1PesHeaderLen;
    pu1PesHeader = _arPsManager[_u1PsActive].au1PesHeader;

    if (_arPsManager[_u1PsActive].u1PtsDtsFlags & 0x2)
    {
        UINT64 u8Pts = 0;
        if ((_arPsManager[_u1PsActive].u2PesLen < 8) ||
            (_arPsManager[_u1PsActive].u2PesHeaderCopyLen < 5))
        {
            SET_PS_STATE_TO_START;
            LOG(5, "L%d: Insufficient PES length!\n", __LINE__);
            return FALSE;
        }
        if (u1PesHeaderLen >= 5)
        {
            u8Pts = (((UINT64)pu1PesHeader[0] & 0x0e) << 29) |
                             (pu1PesHeader[1] << 22)         |
                            ((pu1PesHeader[2] & 0xfe) << 14) |
                             (pu1PesHeader[3] << 7)          |
                            ((pu1PesHeader[4] & 0xfe) >> 1);
            u1PesHeaderRp += 5;
        }
        _arPsManager[_u1PsActive].u8Pts = u8Pts;

        if (_arPsManager[_u1PsActive].u1PtsDtsFlags & 0x1)
        {
            UINT64 u8Dts = 0;
            if ((_arPsManager[_u1PsActive].u2PesLen < 13) ||
                (_arPsManager[_u1PsActive].u2PesHeaderCopyLen < 10))
            {
                SET_PS_STATE_TO_START;
                LOG(5, "L%d: Insufficient PES length!\n", __LINE__);
                return FALSE;
            }
            if (u1PesHeaderLen >= 10)
            {
                u8Dts = (((UINT64)pu1PesHeader[u1PesHeaderRp + 0] & 0x0e) << 29) |
                                 (pu1PesHeader[u1PesHeaderRp + 1] << 22)         |
                                ((pu1PesHeader[u1PesHeaderRp + 2] & 0xfe) << 14) |
                                 (pu1PesHeader[u1PesHeaderRp + 3] << 7)          |
                                ((pu1PesHeader[u1PesHeaderRp + 4] & 0xfe) >> 1);
                u1PesHeaderRp += 5;
            }
            _arPsManager[_u1PsActive].u8Dts = u8Dts;
        }
    }

    #ifdef CC_DMX_HDCP
    if (_arPsManager[_u1PsActive].fgExt && _arPsManager[_u1PsActive].fgHDCP20)
    {
        // processing private data for inputCtr and streamCtr
        if(pu1PesHeader[u1PesHeaderRp] & 0x01 || pu1PesHeader[u1PesHeaderRp] & 0x80)
        {
            u1PesHeaderRp++;
            if (u1PesHeaderLen - u1PesHeaderRp < 16)
            {
                SET_PS_STATE_TO_START;
                LOG(5, "L%d: Insufficient PES length!\n", __LINE__);
                return FALSE;
            }
            u4StreamCtr = (UINT32)((pu1PesHeader[u1PesHeaderRp + 1] & 0x6) << 29) |
                             (pu1PesHeader[u1PesHeaderRp + 2] << 22)         |
                            ((pu1PesHeader[u1PesHeaderRp + 3] & 0xfe) << 14) |
                             (pu1PesHeader[u1PesHeaderRp + 4] << 7)          |
                            ((pu1PesHeader[u1PesHeaderRp + 5] & 0xfe) >> 1);

            u1PesHeaderRp += 6;
            u8InputCtr = (UINT64)(((UINT64)pu1PesHeader[u1PesHeaderRp + 1] & 0x1E) << 59) |
                             ((UINT64)pu1PesHeader[u1PesHeaderRp + 2] << 52)         |
                            (((UINT64)pu1PesHeader[u1PesHeaderRp + 3] & 0xfe) << 44) |
                             ((UINT64)pu1PesHeader[u1PesHeaderRp + 4] << 37)          |
                            ((pu1PesHeader[u1PesHeaderRp + 5] & 0xfe) << 29) |
                             (pu1PesHeader[u1PesHeaderRp + 6] << 22)          |
                            ((pu1PesHeader[u1PesHeaderRp + 7] & 0xfe) << 14) |
                             (pu1PesHeader[u1PesHeaderRp + 8] << 7)          |
                            ((pu1PesHeader[u1PesHeaderRp + 9] & 0xfe) >> 1);
            u1PesHeaderRp += 10;

            // iv = ( Riv(64) XOR streamCtr(32) ) || inputCtr(64)
            _arPsManager[_u1PsActive].au1Iv[4] ^= ((u4StreamCtr >> 24) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[5] ^= ((u4StreamCtr >> 16) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[6] ^= ((u4StreamCtr >> 8) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[7] ^= ((u4StreamCtr >> 0) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[8] ^= ((u8InputCtr >> 56) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[9] ^= ((u8InputCtr >> 48) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[10] ^= ((u8InputCtr >> 40) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[11] ^= ((u8InputCtr >> 32) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[12] ^= ((u8InputCtr >> 24) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[13] ^= ((u8InputCtr >> 16) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[14] ^= ((u8InputCtr >> 8) & 0xff);
            _arPsManager[_u1PsActive].au1Iv[15] ^= ((u8InputCtr >> 0) & 0xff);
        }
    }
    #endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateMpeg1PesHeader
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateMpeg1PesHeader(void)
{
    BOOL fgRet = FALSE;
    UINT8 u1Data;
    UINT16 u2CopyLen, u2SkipLen;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    switch (_arPsManager[_u1PsActive].eMpeg1HeaderState)
    {
        case PS_STATE_MPEG1_STUFFING:
            do
            {
                if (!_Dmx_PS_CPU_PeekByte(&u1Data))
                {
                    return FALSE;
                }

                if (u1Data == 0xFF)
                {
                    //_arPsManager[_u1PsActive].u1PesHeaderLen++;  // Do not count stuffing bytes.
                    UNUSED(_Dmx_PS_CPU_SkipBytes(1));
                    continue;
                }
                else
                {
                    _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_UNDETERMINED_1;
                    fgRet = TRUE;
                    break;
                }
            } while (1);
            break;

        case PS_STATE_MPEG1_UNDETERMINED_1:
            if (!_Dmx_PS_CPU_PeekByte(&u1Data))
            {
                return FALSE;
            }

            if ((u1Data & 0xC0) == 0x40)
            {
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_STD;
            }
            else if ((u1Data & 0xF0) == 0x20)
            {
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_PTS;
            }
            else if ((u1Data & 0xF0) == 0x30)
            {
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_PTS_DTS;
            }
            else
            {
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_ONE_BYTE_MARKER;
            }
            fgRet = TRUE;
            break;

        case PS_STATE_MPEG1_STD:
            ASSERT(PS_FIELD_LEN_MPEG1_STD >= _arPsManager[_u1PsActive].u2Counter);
            u2SkipLen = PS_FIELD_LEN_MPEG1_STD - _arPsManager[_u1PsActive].u2Counter;
            _arPsManager[_u1PsActive].u2Counter += _Dmx_PS_CPU_SkipBytes(u2SkipLen);
            if (_arPsManager[_u1PsActive].u2Counter >= PS_FIELD_LEN_MPEG1_STD)
            {
                //_arPsManager[_u1PsActive].u1PesHeaderLen += 2;
                _arPsManager[_u1PsActive].u2Counter = 0;  // Important: set counter to 0.
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_UNDETERMINED_2;
                fgRet = TRUE;
            }
            else
            {
                return FALSE;
            }
            break;

        case PS_STATE_MPEG1_UNDETERMINED_2:
            if (!_Dmx_PS_CPU_PeekByte(&u1Data))
            {
                return FALSE;
            }

            if ((u1Data & 0xF0) == 0x20)
            {
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_PTS;
            }
            else if ((u1Data & 0xF0) == 0x30)
            {
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_PTS_DTS;
            }
            else
            {
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_ONE_BYTE_MARKER;
            }
            fgRet = TRUE;
            break;

        case PS_STATE_MPEG1_PTS:
            ASSERT(PS_FIELD_LEN_MPEG1_PTS >= _arPsManager[_u1PsActive].u2Counter);
            u2CopyLen = PS_FIELD_LEN_MPEG1_PTS - _arPsManager[_u1PsActive].u2Counter;
            _arPsManager[_u1PsActive].u2Counter += _Dmx_PS_CPU_CopyToHeaderBuffer(u2CopyLen);
            if (_arPsManager[_u1PsActive].u2Counter >= PS_FIELD_LEN_MPEG1_PTS)
            {
                _arPsManager[_u1PsActive].u1PesHeaderLen += u2CopyLen;
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_PARSING_HEADER;
                fgRet = TRUE;
            }
            else
            {
                return FALSE;
            }
            break;

        case PS_STATE_MPEG1_PTS_DTS:
            ASSERT(PS_FIELD_LEN_MPEG1_PTS_DTS >= _arPsManager[_u1PsActive].u2Counter);
            u2CopyLen = PS_FIELD_LEN_MPEG1_PTS_DTS - _arPsManager[_u1PsActive].u2Counter;
            _arPsManager[_u1PsActive].u2Counter += _Dmx_PS_CPU_CopyToHeaderBuffer(u2CopyLen);
            if (_arPsManager[_u1PsActive].u2Counter >= PS_FIELD_LEN_MPEG1_PTS_DTS)
            {
                _arPsManager[_u1PsActive].u1PesHeaderLen += u2CopyLen;
                _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_PARSING_HEADER;
                fgRet = TRUE;
            }
            else
            {
                return FALSE;
            }
            break;

        case PS_STATE_MPEG1_ONE_BYTE_MARKER:
            if (!_Dmx_PS_CPU_PeekByte(&u1Data))
            {
                return FALSE;
            }
            if (u1Data != 0x0F)
            {
                LOG(5, "L%d: incorrect MPEG-1 header marker byte!\n", __LINE__);
            }
            UNUSED(_Dmx_PS_CPU_SkipBytes(1));
            _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_PARSING_HEADER;
            fgRet = TRUE;
            break;

        case PS_STATE_MPEG1_PARSING_HEADER:
            _Dmx_PS_CPU_ParseMpeg1Header();
            _arPsManager[_u1PsActive].eMpeg1HeaderState = PS_STATE_MPEG1_STUFFING;
            UNUSED(_Dmx_PS_CPU_MoveToNextState());
            fgRet = TRUE;
            break;

        default:
            SET_PS_STATE_TO_START;
            fgRet = FALSE;
            LOG(5, "L%d: unknown MPEG-1 PES header state!\n", __LINE__);
            break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateMpeg2PesHeader
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateMpeg2PesHeader(void)
{
    UINT16 u2FieldLen;
    BOOL fgRet = FALSE;
    UINT16 u2CopyLen;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    u2FieldLen = _arPsManager[_u1PsActive].u1PesHeaderLen;  // variable length

    u2CopyLen = u2FieldLen - _arPsManager[_u1PsActive].u2Counter;
    _arPsManager[_u1PsActive].u2Counter += _Dmx_PS_CPU_CopyToHeaderBuffer(u2CopyLen);
    if (_arPsManager[_u1PsActive].u2Counter >= u2FieldLen)
    {
        UNUSED(_Dmx_PS_CPU_ParseMpeg2Header());
        UNUSED(_Dmx_PS_CPU_MoveToNextState());
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }


    return fgRet;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StatePesHeader
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StatePesHeader(void)
{
    BOOL fgRet = FALSE;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_PES_HEADER);

    if (_arPsManager[_u1PsActive].eStreamType == STREAM_MPEG1)
    {
        if (_arPsManager[_u1PsActive].u1StreamID != PS_SID_PRIVATE_STREAM_2)
        {
            fgRet = _Dmx_PS_CPU_StateMpeg1PesHeader();
        }
        else
        {
            UNUSED(_Dmx_PS_CPU_MoveToNextState());
            fgRet = TRUE;
        }
    }
    else if (_arPsManager[_u1PsActive].eStreamType == STREAM_MPEG2)
    {
        fgRet = _Dmx_PS_CPU_StateMpeg2PesHeader();
    }

    return fgRet;
}

static BOOL _Dmx_PS_CPU_ProcessPes(UINT8 u1Idx, UINT32 u4Src, UINT32 u4Size)
{
    PID_STRUCT_T *prPidStruct;
    UINT32 u4BufSize, u4Wp, u4Rp, u4EmptySize;
    BOOL fgProcessed = FALSE;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(u1Idx < DMX_MM_STRUCT_NUM);
    prPidStruct = _DMX_GetPidStruct(u1Idx);

    u4BufSize = prPidStruct->u4BufLen;
    u4Wp = prPidStruct->u4Wp;
    u4Rp = prPidStruct->u4Rp;

    u4EmptySize = EMPTYSIZE(u4Rp, u4Wp, u4BufSize);

    if(u4EmptySize < u4Size)
    {
        LOG(3, "Fifo full! u4EmptySize %d < ProcessPes %d\n", u4EmptySize,
            u4Size);
        return FALSE;
    }

    // copy pes to fifo
    UNUSED(_DMX_CopyRingBuffer(prPidStruct->u4Wp, prPidStruct->u4BufStart, prPidStruct->u4BufEnd + 1,
                            u4Src, u4Src, u4Src + u4Size, u4Size));

    // Flash and invalidate cache
    HalFlushInvalidateDCacheMultipleLine(prPidStruct->u4BufStart,
        (prPidStruct->u4BufEnd + 1) - prPidStruct->u4BufStart);

    prPidStruct->u4Wp += u4Size;
    if(prPidStruct->u4Wp > prPidStruct->u4BufEnd)
    {
        prPidStruct->u4Wp -= prPidStruct->u4BufLen;
    }

    // nfy
    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_NOTIFY_INFO_PES_TIME_T rPes;

        rPes.u4DataAddr = prPidStruct->u4PesRp;
        rPes.u4DataSize = u4Size;
        rPes.u1SerialNumber = prPidStruct->u1SerialNumber;
        //rPes.pvInstanceTag = prPidStruct->pvInstanceTag;
        rPes.u4Pts = (UINT32)(_arPsManager[_u1PsActive].u8Pts & 0xFFFFFFFF);
        rPes.u4Dts = (UINT32)(_arPsManager[_u1PsActive].u8Dts & 0xFFFFFFFF);

        fgProcessed = prPidStruct->pfnNotify(u1Idx, DMX_NOTIFY_CODE_PES_TIME,
            (UINT32)&rPes, prPidStruct->pvNotifyTag);
    }   // if

    // update
    prPidStruct->u4PesRp = prPidStruct->u4Wp;

    return fgProcessed;
}

//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StatePayload
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StatePayload(void)
{
    BOOL fgRet = FALSE;
    UINT16 u2FieldLen;
    UINT16 u2PesLen;
    UINT8 u1PesHeaderLen;
    UINT8 u1PesHeaderExtraLen;
    UINT8 u1Data = 0, u1Idx, u1mIdx;
    UINT32 u4PayloadLen, u4Src, u4SrcLen;
    DMX_MM_DATA_T rMM;
    PID_STRUCT_T *prPidStruct;
    UINT32 u4Timeout = 500;
    DMX_PES_MSG_T rPes;
	BOOL isSubStreamIdMatched = FALSE;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    u2FieldLen = _arPsManager[_u1PsActive].u1AudioInfoLen;  // variable length
    u2PesLen = _arPsManager[_u1PsActive].u2PesLen;                       // error check
    u1PesHeaderLen = _arPsManager[_u1PsActive].u1PesHeaderLen;            // error check
    u1PesHeaderExtraLen = _arPsManager[_u1PsActive].u1PesHeaderExtraLen;  // error check

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_PAYLOAD);

    // The case (PES_length == 0) is handled by u4RemainingPesLen.
    // There is no need to handle it here again.

    u4PayloadLen = _arPsManager[_u1PsActive].u4RemainingPesLen;
    if (u4PayloadLen == 0)
    {
        UNUSED(_Dmx_PS_CPU_MoveToNextState());
        LOG(7, "L%d: u4PayloadLen = %d\n", u4PayloadLen);
        return FALSE;
    }

    if (u2PesLen < (u1PesHeaderLen + u1PesHeaderExtraLen))
    {
        UNUSED(_Dmx_PS_CPU_MoveToNextState());
        LOG(5, "PesLen %d < (PesHeaderLen %d + %d)\n", u2PesLen,
            u1PesHeaderLen, u1PesHeaderExtraLen);
        return FALSE;
    }

    // Find the PIDX index corresponding to the current stream ID and substream ID
    if(_arPsManager[_u1PsActive].fgFirstByteOfPayload == TRUE)
    {
        if (!_Dmx_PS_CPU_PeekByte(&u1Data))
        {
            return FALSE;
        }
        _arPsManager[_u1PsActive].fgFirstByteOfPayload = FALSE;
    }


    for (u1Idx = 0; u1Idx < DMX_MM_STRUCT_NUM; u1Idx++)
    {
        prPidStruct = _DMX_GetPidStruct(u1Idx);
        if(_DMX_MUL_GetPidxInstId(u1Idx) != _u1PsActive)
        {
            continue;
        }
        if (prPidStruct->u1StreamID == _arPsManager[_u1PsActive].u1StreamID)
        {
            break;
        }
    }
    if (u1Idx >= DMX_MM_STRUCT_NUM)
    {
        UNUSED(_Dmx_PS_CPU_SkipBytes(u4PayloadLen));
        if (_arPsManager[_u1PsActive].u4RemainingPesLen == 0)
        {
            UNUSED(_Dmx_PS_CPU_MoveToNextState());
        }
        else
        {
            // This is a special case to force the state to padding.
            // This can remove remaining data of this pes before move to next state
            _arPsManager[_u1PsActive].eSearchState = PS_STATE_PADDING;
        }
        LOG(6, "L%d: No valid Pidx is found for stream ID %d!\n",
            __LINE__, _arPsManager[_u1PsActive].u1StreamID);  // debug
        return FALSE;
    }

    // check if there is a multiple match of stream id, then check substream id
    for (u1mIdx = 0; u1mIdx < DMX_MM_STRUCT_NUM; u1mIdx++)
    {
        prPidStruct = _DMX_GetPidStruct(u1mIdx);
        if(_DMX_MUL_GetPidxInstId(u1mIdx) != _u1PsActive)
        {
            continue;
        }

        if (prPidStruct->u1StreamID == _arPsManager[_u1PsActive].u1StreamID)
        {
            if (prPidStruct->fgEnableSsidFilter)
            {
                if (prPidStruct->u1SubstreamID == u1Data)
                {
					isSubStreamIdMatched = TRUE;
                    u1Idx = u1mIdx;
                    break;
                }
            }
        }
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        if (_arPsManager[_u1PsActive].eAudioType == DMX_AUDIO_MPEG)
        {
            if (prPidStruct->fgEnableSsidFilter)
            {
            	_arPsManager[_u1PsActive].u1SubstreamID = u1Data;
            }
            // No audio info is needed.  Do nothing here.
            // _arPsManager[_u1PsActive].fgAudioInfoCopied = TRUE;
        }
        else if (_arPsManager[_u1PsActive].eAudioType == DMX_AUDIO_LPCM)
        {
            while (!_arPsManager[_u1PsActive].fgAudioInfoCopied)
            {
                if (!_Dmx_PS_CPU_GetByte(&u1Data))
                {
                    return FALSE;
                }

                ASSERT(_arPsManager[_u1PsActive].u2Counter <= u2FieldLen);
                _arPsManager[_u1PsActive].au1AudioInfo[_arPsManager[_u1PsActive].u2Counter] = u1Data;
                _arPsManager[_u1PsActive].u2Counter++;
                if (_arPsManager[_u1PsActive].u2Counter >= u2FieldLen)
                {
                    _arPsManager[_u1PsActive].fgAudioInfoCopied = TRUE;
                    _arPsManager[_u1PsActive].u1SubstreamID = _arPsManager[_u1PsActive].au1AudioInfo[0];
                    _arPsManager[_u1PsActive].u4AudioInfo =
                            (_arPsManager[_u1PsActive].au1AudioInfo[4] << 16) |
                            (_arPsManager[_u1PsActive].au1AudioInfo[5] <<  8) |
                            (_arPsManager[_u1PsActive].au1AudioInfo[6]);
                }
            }
        }
        else if (_arPsManager[_u1PsActive].eAudioType == DMX_AUDIO_AC3)
        {
            while (!_arPsManager[_u1PsActive].fgAudioInfoCopied)
            {
                if (!_Dmx_PS_CPU_GetByte(&u1Data))
                {
                    return FALSE;
                }

                ASSERT(_arPsManager[_u1PsActive].u2Counter <= u2FieldLen);
                _arPsManager[_u1PsActive].au1AudioInfo[_arPsManager[_u1PsActive].u2Counter] = u1Data;
                _arPsManager[_u1PsActive].u2Counter++;
                if (_arPsManager[_u1PsActive].u2Counter >= u2FieldLen)
                {
                    _arPsManager[_u1PsActive].fgAudioInfoCopied = TRUE;
                    _arPsManager[_u1PsActive].u1SubstreamID = _arPsManager[_u1PsActive].au1AudioInfo[0];
                    _arPsManager[_u1PsActive].u4AudioInfo =
                        (_arPsManager[_u1PsActive].au1AudioInfo[0] << 24) |
                        (_arPsManager[_u1PsActive].au1AudioInfo[1] << 16) |
                        (_arPsManager[_u1PsActive].au1AudioInfo[2] <<  8) |
                        (_arPsManager[_u1PsActive].au1AudioInfo[3]);
                }
            }
        }
	    else if(_arPsManager[_u1PsActive].eAudioType == DMX_AUDIO_DTS)
		{
		     while (!_arPsManager[_u1PsActive].fgAudioInfoCopied)
            {
                if (!_Dmx_PS_CPU_GetByte(&u1Data))
                {
                    return FALSE;
                }

                ASSERT(_arPsManager[_u1PsActive].u2Counter <= u2FieldLen);
                _arPsManager[_u1PsActive].au1AudioInfo[_arPsManager[_u1PsActive].u2Counter] = u1Data;
                _arPsManager[_u1PsActive].u2Counter++;
                if (_arPsManager[_u1PsActive].u2Counter >= u2FieldLen)
                {
                    _arPsManager[_u1PsActive].fgAudioInfoCopied = TRUE;
                    _arPsManager[_u1PsActive].u1SubstreamID = _arPsManager[_u1PsActive].au1AudioInfo[0];
                    _arPsManager[_u1PsActive].u4AudioInfo =
                        (_arPsManager[_u1PsActive].au1AudioInfo[0] << 24) |
                        (_arPsManager[_u1PsActive].au1AudioInfo[1] << 16) |
                        (_arPsManager[_u1PsActive].au1AudioInfo[2] <<  8) |
                        (_arPsManager[_u1PsActive].au1AudioInfo[3]);
                }
            }
		}
        else
        {
            LOG(5, "L%d: Unknown PS audio type!\n", __LINE__);
            return FALSE;
        }
    }

    #ifdef CC_DMX_HDCP
    if(_arPsManager[_u1PsActive].fgHDCP20 && (_arPsManager[_u1PsActive].u4PreviousBufSize != 0))
    {
        UINT32 u4HandlingSize = 0;
        // previous data handling
        if(u4PayloadLen + _arPsManager[_u1PsActive].u4PreviousBufSize >= 16)
        {
            u4HandlingSize = 16 - _arPsManager[_u1PsActive].u4PreviousBufSize;
        }
        else
        {
            u4HandlingSize = u4PayloadLen;
        }
        u4Src = (UINT32)_arPsManager[_u1PsActive].au1PreviousBuf;
        u4SrcLen = _Dmx_PS_CPU_SkipBytes(u4HandlingSize);
        u4SrcLen = _arPsManager[_u1PsActive].u4PreviousBufSize;
        ASSERT(u4SrcLen <= 16);
        // when there is no more pes data or len is 16, move this data
        if((_arPsManager[_u1PsActive].u4RemainingPesLen == 0) || (u4SrcLen == 16))
        {
            x_memset((void*)&rMM, 0, sizeof(rMM));
            rMM.u4BufStart = (UINT32)_arPsManager[_u1PsActive].au1PreviousBuf;
            rMM.u4BufEnd = (UINT32)(_arPsManager[_u1PsActive].au1PreviousBuf + 32);
            rMM.u4StartAddr = u4Src;
            rMM.u4FrameSize = u4SrcLen;
            rMM.u4Pts = (UINT32)(_arPsManager[_u1PsActive].u8Pts & 0xFFFFFFFF);
            rMM.u4Dts = (UINT32)(_arPsManager[_u1PsActive].u8Dts & 0xFFFFFFFF);

            ASSERT(u4SrcLen != 0);
            if(!_Dmx_PS_CPU_HDCP_Decrypt(u4Src, u4SrcLen))
            {
                LOG(5, "L%d: fail to decrypt HDCP!\n", __LINE__);
            }

            if (prPidStruct->fgEnable)
            {
                if (prPidStruct->fgEnableSsidFilter)
                {
                    if (prPidStruct->u1SubstreamID == _arPsManager[_u1PsActive].u1SubstreamID)
                    {
                        //_DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, u1Idx, TRUE,  FALSE);
                        //_DMX_MM_MoveData(u1Idx, &rMM, u4Timeout);
                        UNUSED(_DMX_PS_MoveDataQueue(u1Idx, &rMM, u4Timeout));
                        fgRet = TRUE;
                    }
                    else
                    {
                        fgRet = FALSE;
                        LOG(7, "L%d: Incorrect substream ID. Ignore PES payload!\n",
                            __LINE__);
                    }
                }
                else
                {
                    //_DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, u1Idx, TRUE,  FALSE);
                    //_DMX_MM_MoveData(u1Idx, &rMM, u4Timeout);
                    //UNUSED(_DMX_PS_MoveDataQueue);
                    UNUSED(_DMX_PS_MoveDataQueue(u1Idx, &rMM, u4Timeout));
                    fgRet = TRUE;
                }
            }
            _arPsManager[_u1PsActive].u4PreviousBufSize = 0;
        }
    }
    #endif

    // 1. Find the index of the stream_id.
    // 2. Transmit data [u4Src, u4SrcTail) of length u4SrcLen.
    // 3. Wait for the completion of data transmission.
    u4PayloadLen = _arPsManager[_u1PsActive].u4RemainingPesLen;
    u4Src = _arPsManager[_u1PsActive].u4BufRp;
    u4SrcLen = _Dmx_PS_CPU_SkipBytes(u4PayloadLen);
    //u4SrcTail = _arPsManager[_u1PsActive].u4BufRp;

    if (u4SrcLen == 0)
    {
        LOG(5, "L%d: fail to skip any byte!\n", __LINE__);
        return FALSE;
    }

    #ifdef CC_DMX_HDCP
    if(_arPsManager[_u1PsActive].fgHDCP20)
    {
        u4SrcLen -= _arPsManager[_u1PsActive].u4PreviousBufSize;
        if(u4SrcLen == 0)
        {
            // no more data for processing after queueing
            return TRUE;
        }
        if(!_Dmx_PS_CPU_HDCP_Decrypt(u4Src, u4SrcLen))
        {
            LOG(5, "L%d: fail to decrypt HDCP!\n", __LINE__);
        }
    }
    #endif

    if ((prPidStruct->ePidType == DMX_PID_TYPE_PES_TIME) && (isSubStreamIdMatched == TRUE))
    {
        return _Dmx_PS_CPU_ProcessPes(u1Idx, u4Src, u4SrcLen);
    }

    x_memset((void*)&rMM, 0, sizeof(rMM));
    rMM.u4BufStart = _arPsManager[_u1PsActive].u4BufStart;
    rMM.u4BufEnd = _arPsManager[_u1PsActive].u4BufEnd;
    rMM.u4StartAddr = u4Src;
    rMM.u4FrameSize = u4SrcLen;
    rMM.u4Pts = (UINT32)(_arPsManager[_u1PsActive].u8Pts & 0xFFFFFFFF);
    rMM.u4Dts = (UINT32)(_arPsManager[_u1PsActive].u8Dts & 0xFFFFFFFF);
    // rMM.fgEOS = FALSE;  // redundant
    // rMM.fgFrameHead = FALSE;  // redundant

    if (prPidStruct->fgEnable)
    {
        if (prPidStruct->fgEnableSsidFilter)
        {
            if (prPidStruct->u1SubstreamID == _arPsManager[_u1PsActive].u1SubstreamID)
            {
#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
                if (!_fgEnded)
                {
                    HAL_RAW_TIME_T rRawTime;
                    HAL_TIME_T rTime;

                    HAL_GetRawTime(&_rRawTime2);
                    HAL_GetDeltaRawTime(&rRawTime, &_rRawTime1, &_rRawTime2);
                    HAL_RawToTime(&rRawTime, &rTime);
                    LOG(1, "Stream ID counter: %u\n", _u4StreamIdNumber);
                    LOG(1, "Ticks: %u\n", rRawTime.u4Ticks);
                    LOG(1, "Cycles: %u\n", rRawTime.u4Cycles);
                    LOG(1, "Seconds: %u\n", rTime.u4Seconds);
                    LOG(1, "Micros: %u\n", rTime.u4Micros);

                    _fgEnded = TRUE;
                }
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
                //_DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, u1Idx, TRUE,  FALSE);
                //_DMX_MM_MoveData(u1Idx, &rMM, u4Timeout);
                UNUSED(_DMX_PS_MoveDataQueue(u1Idx, &rMM, u4Timeout));
                fgRet = TRUE;
            }
            else
            {
                fgRet = FALSE;
                LOG(7, "L%d: Incorrect substream ID. Ignore PES payload!\n",
                    __LINE__);
            }
        }
        else
        {
#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
            if (!_fgEnded)
            {
                HAL_RAW_TIME_T rRawTime;
                HAL_TIME_T rTime;

                HAL_GetRawTime(&_rRawTime2);
                HAL_GetDeltaRawTime(&rRawTime, &_rRawTime1, &_rRawTime2);
                HAL_RawToTime(&rRawTime, &rTime);
                LOG(1, "Stream ID counter: %u\n", _u4StreamIdNumber);
                LOG(1, "Ticks: %u\n", rRawTime.u4Ticks);
                LOG(1, "Cycles: %u\n", rRawTime.u4Cycles);
                LOG(1, "Seconds: %u\n", rTime.u4Seconds);
                LOG(1, "Micros: %u\n", rTime.u4Micros);

                _fgEnded = TRUE;
            }
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
            //_DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, u1Idx, TRUE,  FALSE);
            //_DMX_MM_MoveData(u1Idx, &rMM, u4Timeout);
            //UNUSED(_DMX_PS_MoveDataQueue);
            UNUSED(_DMX_PS_MoveDataQueue(u1Idx, &rMM, u4Timeout));
            fgRet = TRUE;
        }
    }

    // Send video SEQ_END if it is applicable.
    if ((prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO) &&
        (_arPsManager[_u1PsActive].u4LastWord == PS_WORD_SEQUENCE_END))
    {
        BOOL u1Channel;

        VERIFY(_DMX_GetVideoChannel(u1Idx, &u1Channel));

        x_memset((void*)&rPes, 0, sizeof(rPes));
        rPes.u1Pidx = u1Idx;
        rPes.u4FrameType = PIC_TYPE_SEQ_END;
        rPes.ePidType = DMX_PID_TYPE_ES_VIDEO;
        rPes.u1Channel = u1Channel;
        rPes.u1DeviceId = prPidStruct->u1DeviceId;
        rPes.eMsgType = DMX_PES_MSG_TYPE_PES;
        rPes.u4Pts = (UINT32)(_arPsManager[_u1PsActive].u8Pts & 0xFFFFFFFF);
        rPes.u4Dts = (UINT32)(_arPsManager[_u1PsActive].u8Dts & 0xFFFFFFFF);
        if (!_DMX_SendPictureHeader(&rPes))
        {
            LOG(3, "Fail to send SEQ_END!\n");
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StatePadding
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StatePadding(void)
{
    UINT32 u4PayloadLen;

    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_PADDING);

    u4PayloadLen = _arPsManager[_u1PsActive].u4RemainingPesLen;
    if (u4PayloadLen == 0)
    {
        UNUSED(_Dmx_PS_CPU_MoveToNextState());
        LOG(7, "L%d: u4PayloadLen = %d\n", u4PayloadLen);
        return FALSE;
    }

    UNUSED(_Dmx_PS_CPU_SkipBytes(u4PayloadLen));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_StateSeqEnd
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_StateSeqEnd(void)
{
    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState == PS_STATE_SEQ_END);

    return TRUE;
}



PS_PARSER_T _arPsParsing[ /* PS_STATE_SEQ_END + 1 */ ] =
{
    {PS_STATE_START,            _Dmx_PS_CPU_StateStart},
    {PS_STATE_SYNC_MARKER,      _Dmx_PS_CPU_StateSyncMarker},
    {PS_STATE_VCD_SUBHEADER,    _Dmx_PS_CPU_StateVcdSubheader},
    {PS_STATE_PES_START_PREFIX, _Dmx_PS_CPU_StatePesStartPrefix},
#if 0
    {PS_STATE_SYSTEM_HEADER,    _Dmx_PS_CPU_StateSystemHeader},
#endif  // 0
    {PS_STATE_PES_LEN,          _Dmx_PS_CPU_StatePesLen},
    {PS_STATE_STREAM_TYPE,      _Dmx_PS_CPU_StateStreamType},
    {PS_STATE_PES_FLAGS,        _Dmx_PS_CPU_StatePesFlags},
    {PS_STATE_PES_HEADER,       _Dmx_PS_CPU_StatePesHeader},
    {PS_STATE_PAYLOAD,          _Dmx_PS_CPU_StatePayload},
    {PS_STATE_PADDING,          _Dmx_PS_CPU_StatePadding},
    {PS_STATE_SEQ_END,          _Dmx_PS_CPU_StateSeqEnd}
};



//-----------------------------------------------------------------------------
/** _Dmx_PS_CPU_Parsing
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_CPU_Parsing(void)
{
    ASSERT(_u1PsActive < DMX_MAX_INSTANCE);
    if(_u1PsActive >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    ASSERT(_arPsManager[_u1PsActive].eSearchState < PS_STATE_SEQ_END);

    return _arPsParsing[_arPsManager[_u1PsActive].eSearchState].pfnPsParsing();
}


//-----------------------------------------------------------------------------
/** _DMX_PS_CPU_MoveData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PS_CPU_MoveData(DMX_MM_DATA_T *prData)
{
    UINT8 u1Idx, u1AudioPidx, u1VideoPidx, u1InstId;
    PID_STRUCT_T *prPidStruct;
    //UINT32 u4FalseEventCounter;

    u1AudioPidx = DMX_PS_CPU_UNKNOWN_PIDX;
    u1VideoPidx = DMX_PS_CPU_UNKNOWN_PIDX;

    if (prData == NULL)
    {
        return FALSE;
    }

    //LOG(5, "%s: inst:%d, eos:%d\n", __LINE__, prData->u1Idx, (INT32)prData->fgEOS);

    ASSERT(prData->u1Idx < DMX_MAX_INSTANCE);
    u1InstId = prData->u1Idx;

    if ((prData->u4StartAddr < prData->u4BufStart) ||
        (prData->u4StartAddr > prData->u4BufEnd))
    {
        LOG(5, "%s:L%d: Pointer is out of range!\n", __func__, __LINE__);
        return FALSE;
    }

    if (prData->u4FrameSize > (prData->u4BufEnd - prData->u4BufStart))
    {
        LOG(5, "%s:L%d: data size > buffer size!\n", __func__, __LINE__);
        return FALSE;
    }

    ASSERT(u1InstId < DMX_MAX_INSTANCE);
    if(u1InstId >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }

    _arPsManager[u1InstId].u4BufStart = VIRTUAL(prData->u4BufStart);
    _arPsManager[u1InstId].u4BufEnd = VIRTUAL(prData->u4BufEnd);
    _arPsManager[u1InstId].u4BufSize =
        _arPsManager[u1InstId].u4BufEnd - _arPsManager[u1InstId].u4BufStart;
    _arPsManager[u1InstId].u4BufRp = VIRTUAL(prData->u4StartAddr);
    _arPsManager[u1InstId].u4DataSize = prData->u4FrameSize;
    _arPsManager[u1InstId].fgEOS = prData->fgEOS;
    _arPsManager[u1InstId].u4TransactionCount++;

    _u1PsActive = u1InstId;

//    u4FalseEventCounter = 0;
    while (_arPsManager[_u1PsActive].u4DataSize)
    {
#ifdef CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
        if (!_fgStarted)
        {
            HAL_GetRawTime(&_rRawTime1);
            _fgStarted = TRUE;
        }
#endif  // CC_DMX_MEASURE_TIME_OF_FIRST_PS_PAYLOAD
        if (_Dmx_PS_CPU_Parsing())
        {
            // Leave it empty on purpose for now.
        }
    }

    if(!_DMX_PS_TriggerAllMoveDataQueue())
    {
        return FALSE;
    }

    // Process the EOS flag.
    if (_arPsManager[u1InstId].fgEOS)
    {
        for (u1Idx = 0; u1Idx < DMX_MM_STRUCT_NUM; u1Idx++)
        {
            prPidStruct = _DMX_GetPidStruct(u1Idx);
            if(_DMX_MUL_GetPidxInstId(u1Idx) != u1InstId)
            {
                continue;
            }
            if (prPidStruct->fgEnable)
            {
                if (u1AudioPidx == DMX_PS_CPU_UNKNOWN_PIDX)
                {
                    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
                    {
                        u1AudioPidx = u1Idx;
                        continue;
                    }
                }
                if (u1VideoPidx == DMX_PS_CPU_UNKNOWN_PIDX)
                {
                    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
                    {
                        u1VideoPidx = u1Idx;
                        continue;
                    }
                }
            }
        }

        if (u1AudioPidx != DMX_PS_CPU_UNKNOWN_PIDX)
        {
            DMX_AUDIO_PES_T rPes;
            x_memset((void*)&rPes, 0, sizeof(rPes));
            rPes.fgEOS = TRUE;
            rPes.u1PidIndex = u1AudioPidx;
            rPes.u1DeviceId = _DMX_GetPidStruct(u1AudioPidx)->u1DeviceId;
            if (!_DMX_SendAudioPes(&rPes))
            {
                return FALSE;
            }
        }
        if (u1VideoPidx != DMX_PS_CPU_UNKNOWN_PIDX)
        {
            if (!_DMX_SendEOSMessage(u1VideoPidx))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

