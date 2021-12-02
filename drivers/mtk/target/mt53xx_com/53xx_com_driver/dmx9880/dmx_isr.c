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
#include "dmx_pcr.h"
#include "dmx_debug.h"
#include "dmx_if.h"
#include "dmx_drm_drvif.h"
#include "stc_drvif.h"
#include "x_bim.h"
#include "x_hal_arm.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_timer.h"

#include "fvr.h"

#ifdef CC_DMX_EMULATION
EXTERN void _DMX_EMU_MulMatch_ISR(void);
#endif // CC_DMX_EMULATION

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// DEBUG_SECTION printfs all received sections
//
//#define DEBUG_PIC_TIME
//#define DEBUG_SECTION
#define CC_DMX_PRINF_STC_LOG

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

#ifdef CC_DMX_PES_EXT_BUFFER
// Stream ID
#define PES_ID_PROGRAM_STREAM_MAP           0xBC
#define PES_ID_PADDING_STREAM               0xBE
#define PES_ID_PRIVATE_STREAM_2             0xBF
#define PES_ID_ECM                          0xF0
#define PES_ID_EMM                          0xF1
#define PES_ID_DSMCC_STREAM                 0xF2
#define PES_ID_H222_1_E                     0xF8
#define PES_ID_PROGRAM_STREAM_DIR           0xFF
#endif  // CC_DMX_PES_EXT_BUFFER

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

// H264 Picture start codes
#define H264_PIC_START_CODE_1               0x00
#define H264_PIC_START_CODE_2               0x00
#define H264_PIC_START_CODE_3               0x01
#define H264_PIC_START_NONIDR               0x01
#define H264_PIC_START_IDR                  0x05
#define H264_PIC_START_SEI                  0x06
#define H264_PIC_START_SEQ                  0x07
#define H264_PIC_START_PIC                  0x08
#define H264_PIC_START_AU                   0x09
#define H264_PIC_START_END_OF_SEQ           0x0a
#define H264_PIC_START_PREFIX               0x0e
#define H264_PIC_START_SUB_SPS              0x0f
#define H264_PIC_START_CODED_SLICE_EXT      0x14

// VC1 Picture start codes
#define VC1_PIC_START_CODE_1               0x00
#define VC1_PIC_START_CODE_2               0x00
#define VC1_PIC_START_CODE_3               0x01
#define VC1_PIC_START_CODE_FIELD           0x0C
#define VC1_PIC_START_CODE_FRAME           0x0D
#define VC1_PIC_START_CODE_ENTRY           0x0E
#define VC1_PIC_START_CODE_SEQ             0x0F

// MPEG4 Picture start codes
#define MPEG4_PIC_START_CODE_1              0x00
#define MPEG4_PIC_START_CODE_2              0x00
#define MPEG4_PIC_START_CODE_3_1            0x01
#define MPEG4_PIC_START_CODE_3_2            0x80
#define MPEG4_PIC_START_CODE_PIC            0xB6

// AVS Picture start codes
#define AVS_PIC_START_CODE_1                0x00
#define AVS_PIC_START_CODE_2                0x00
#define AVS_PIC_START_CODE_3                0x01
#define AVS_PIC_START_CODE_SEQ_START        0xB0
#define AVS_PIC_START_CODE_SEQ_END          0xB1
#define AVS_PIC_START_CODE_SEQ_USR_DATA     0xB2
#define AVS_PIC_START_CODE_SEQ_PIC_I        0xB3
#define AVS_PIC_START_CODE_SEQ_EXT          0xB5
#define AVS_PIC_START_CODE_SEQ_PIC_PB       0xB6
#define AVS_PIC_START_CODE_SEQ_VID_EDIT     0xB7

// Hardware picture types
#define PIC_HW_TYPE_PIC                     0
#define PIC_HW_TYPE_SEQ_START               1
#define PIC_HW_TYPE_SEQ_END                 2
#define PIC_HW_TYPE_GOP                     3
#define PIC_HW_TYPE_UNKNOWN                 7

#if defined(CC_MVC_SUPPORT) || defined(CC_SW_MVC_ENABLE)
#define PIC_HEADER_LEN                      13
#else
#define PIC_HEADER_LEN                      6
#endif
#define PIC_VC1_HEADER_LEN                  10

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

#ifdef CC_DMX_PES_AUDDESC
    BOOL fgContainAD;
    UINT8 u1ADFad;
    UINT8 u1ADPan;
#endif // CC_DMX_PES_AUDDESC
} DMX_PES_HEADER_T;

enum tagFrameCodingMode
{
  PROGRESSIVE = 0,
  INTERLACEFRAME,
  INTERLACEFIELD
};

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8 _u1H264SliceNum = 0;

#ifdef CC_DMX_PRINF_STC_LOG
static BOOL _fgEnableVideoPtsInfo = FALSE;
static BOOL _fgEnableAudioPtsInfo = FALSE;
#endif

#ifdef ENABLE_MULTIMEDIA
static UINT8 _u1vopFirstFieldType[8] =
{ VC1_PIC_TYPE_I, VC1_PIC_TYPE_I, VC1_PIC_TYPE_P, VC1_PIC_TYPE_P,
  VC1_PIC_TYPE_B, VC1_PIC_TYPE_B, VC1_PIC_TYPE_BI, VC1_PIC_TYPE_BI };

static UINT8 _u1vopSecondFieldType[8] =
{ VC1_PIC_TYPE_I, VC1_PIC_TYPE_P, VC1_PIC_TYPE_I, VC1_PIC_TYPE_P,
  VC1_PIC_TYPE_B, VC1_PIC_TYPE_BI, VC1_PIC_TYPE_B, VC1_PIC_TYPE_BI };
#endif  // ENABLE_MULTIMEDIA

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef CC_DMX_PES_EXT_BUFFER
//-----------------------------------------------------------------------------
/** _DmxParsePesExtData
 *  Parse PES header
 *
 *  @param  u4HeaderBufStart    Starting address of the Header buffer
 *  @param  u4HeaderBufEnd      End address (+1) of the Header buffer
 *  @param  u4HeaderAddr        The address of the PES header
 *  @param  u4HeaderAddrOffset  The offset from the PES header for data retrieval.
 *
 *  @retval The data byte at the address of "u4HeaderAddr + u4HeaderAddrOffset".
 */
//-----------------------------------------------------------------------------
static UINT8 _Dmx_GetByte(UINT32 u4HeaderBufStart, UINT32 u4HeaderBufEnd,
    UINT32 u4HeaderAddr, UINT32 u4HeaderAddrOffset)
{
    UINT32 u4BufLen, u4ReadPointer;
    UINT8 u1Data;

    u4BufLen = u4HeaderBufEnd - u4HeaderBufStart;
    DMX_PANIC(u4HeaderAddrOffset < u4BufLen);
    DMX_PANIC_RET(0);

    u4ReadPointer = u4HeaderAddr + u4HeaderAddrOffset;
    if (u4ReadPointer >= u4HeaderBufEnd)
    {
        u4ReadPointer -= u4BufLen;
    }
    DMX_PANIC((u4HeaderBufStart <= u4ReadPointer) && (u4ReadPointer < u4HeaderBufEnd));
    DMX_PANIC_RET(0);

    u1Data = (*(UINT8*)(u4ReadPointer));

    return u1Data;
}


//-----------------------------------------------------------------------------
/** _DmxParsePesExtData
 *  Parse the PES header for the extraction of the PES Extention Field.
 *  Note: prPidStruct->u4ExtBufWp is updated in this function.
 *
 *  @param  u4HeaderInfoAddr    Address of header info
 *  @param  u4HeaderWp          Header write pointer
 *  @param  u4HeaderBufStart    Starting address of the Header buffer
 *  @param  u4HeaderBufEnd      End address (+1) of the Header buffer
 *  @param  u4HeaderByteCount   Header byte count
 *  @param  prPidStruct         PID structure
 *  @param  pu4ExtBufRp [output]
 *                              Current RP to the PES extension buffer
 *  @param  pu4ExtBufWp [output]
 *                              Current WP to the PES extension buffer
 *
 *  @retval TRUE                success
 *          FALSE               faile
 */
//-----------------------------------------------------------------------------
static BOOL _DmxParsePesExtData(UINT32 u4HeaderInfoAddr, UINT32 u4HeaderWp,
    UINT32 u4HeaderBufStart, UINT32 u4HeaderBufEnd, UINT32 u4HeaderByteCount,
    PID_STRUCT_T *prPidStruct, UINT32 *pu4ExtBufRp, UINT32 *pu4ExtBufWp)
{
    UINT32 u4HdrAddr, u4HdrAddrOffset;
    UINT8 u1StreamId;

    DMX_PANIC((u4HeaderInfoAddr >= u4HeaderBufStart) && (u4HeaderInfoAddr < u4HeaderBufEnd));
    DMX_PANIC_RET(FALSE);
    DMX_PANIC(prPidStruct != NULL);
    MX_PANIC_RET(FALSE);
    DMX_PANIC(pu4ExtBufRp != NULL);
    MX_PANIC_RET(FALSE);
    DMX_PANIC(pu4ExtBufWp != NULL);
    MX_PANIC_RET(FALSE);

    // Get PES header address
    u4HdrAddr = _DMX_AdvanceAddr(u4HeaderInfoAddr, PES_HEADER_EXTRA_BYTES,
        u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);

    u4HdrAddrOffset = 3;    // count the "packet_start_code_prefix"

    // Verify the PES header length for 3 more bytes.
    if (u4HeaderByteCount < (u4HdrAddrOffset + 3))
    {
        LOG(1/*5*/, "%d) PES header (len:%u, offset:%u) is too short!\n",
            __LINE__, u4HeaderByteCount, (u4HdrAddrOffset + 3));
        return FALSE;
    }
    u1StreamId = _Dmx_GetByte(u4HeaderBufStart, u4HeaderBufEnd, u4HdrAddr, u4HdrAddrOffset);
    u4HdrAddrOffset += 3;   // Count the "stream_id" and "PES_packet_length".

    if ((u1StreamId != PES_ID_PROGRAM_STREAM_MAP)   &&
        (u1StreamId != PES_ID_PADDING_STREAM)       &&
        (u1StreamId != PES_ID_PRIVATE_STREAM_2)     &&
        (u1StreamId != PES_ID_ECM)                  &&
        (u1StreamId != PES_ID_EMM)                  &&
        (u1StreamId != PES_ID_PROGRAM_STREAM_DIR)   &&
        (u1StreamId != PES_ID_DSMCC_STREAM)         &&
        (u1StreamId != PES_ID_H222_1_E))
    {
        UINT8 u1Byte0, u1Byte1, u1PtsDtsFlags;
        BOOL fgEscr, fgEsRate, fgDsmTrick, fgAddiCopy, fgPesCrc, fgPesExt;

        // Does the 3-byte flags exist?
        // Verify the PES header length for 3 more bytes.
        if (u4HeaderByteCount < (u4HdrAddrOffset + 3))
        {
            LOG(5, "%d) PES header (len:%u, offset:%u) is too short!\n",
                __LINE__, u4HeaderByteCount, (u4HdrAddrOffset + 3));
            return FALSE;
        }
        u1Byte0 = _Dmx_GetByte(u4HeaderBufStart, u4HeaderBufEnd, u4HdrAddr, u4HdrAddrOffset);
        u1Byte1 = _Dmx_GetByte(u4HeaderBufStart, u4HeaderBufEnd, u4HdrAddr, u4HdrAddrOffset + 1);
        // Byte2 is not needed.
        u4HdrAddrOffset += 3;

        // Optional check
        if (((u1Byte0 >> 6) & 0x3) != 0x2)
        {
            LOG(1/*5*/, "No \'10\' in the PES header!\n");
            return FALSE;
        }

        u1PtsDtsFlags =  (u1Byte1 >> 6) & 0x3;
        fgEscr        = ((u1Byte1 >> 5) & 0x1) ? TRUE : FALSE;
        fgEsRate      = ((u1Byte1 >> 4) & 0x1) ? TRUE : FALSE;
        fgDsmTrick    = ((u1Byte1 >> 3) & 0x1) ? TRUE : FALSE;
        fgAddiCopy    = ((u1Byte1 >> 2) & 0x1) ? TRUE : FALSE;
        fgPesCrc      = ((u1Byte1 >> 1) & 0x1) ? TRUE : FALSE;
        fgPesExt      = ((u1Byte1 >> 0) & 0x1) ? TRUE : FALSE;

        if (u1PtsDtsFlags == (UINT8)0x2)
        {
            u4HdrAddrOffset += 5;
        }
        if (u1PtsDtsFlags == (UINT8)0x3)
        {
            u4HdrAddrOffset += 10;
        }
        if (fgEscr)
        {
            u4HdrAddrOffset += 6;
        }
        if (fgEsRate)
        {
            u4HdrAddrOffset += 3;
        }
        if (fgDsmTrick)
        {
            u4HdrAddrOffset += 1;
        }
        if (fgAddiCopy)
        {
            u4HdrAddrOffset += 1;
        }
        if (fgPesCrc)
        {
            u4HdrAddrOffset += 2;
        }

        if (fgPesExt)
        {
            BOOL fgPesPrivData, fgPackHeader, fgProgPacketSeq;
            BOOL fgPstdBuffer, fgPesExt2;

            // Verify the PES header length for 1 more byte.
            if (u4HeaderByteCount < (u4HdrAddrOffset + 1))
            {
                LOG(1/*5*/, "%d) PES header (len:%u, offset:%u) is too short!\n",
                    __LINE__, u4HeaderByteCount, (u4HdrAddrOffset + 1));
                return FALSE;
            }
            u1Byte0 = _Dmx_GetByte(u4HeaderBufStart, u4HeaderBufEnd, u4HdrAddr, u4HdrAddrOffset);
            u4HdrAddrOffset += 1;

            fgPesPrivData   = ((u1Byte0 >> 7) & 0x1) ? TRUE : FALSE;
            fgPackHeader    = ((u1Byte0 >> 6) & 0x1) ? TRUE : FALSE;
            fgProgPacketSeq = ((u1Byte0 >> 5) & 0x1) ? TRUE : FALSE;
            fgPstdBuffer    = ((u1Byte0 >> 4) & 0x1) ? TRUE : FALSE;
            fgPesExt2       = ((u1Byte0 >> 0) & 0x1) ? TRUE : FALSE;

            if (fgPesPrivData)
            {
                u4HdrAddrOffset += 16;
            }
            if (fgPackHeader)
            {
                // Verify the PES header length for 1 more byte.
                if (u4HeaderByteCount < (u4HdrAddrOffset + 1))
                {
                    LOG(1/*5*/, "%d) PES header (len:%u, offset:%u) is too short!\n",
                        __LINE__, u4HeaderByteCount, (u4HdrAddrOffset + 1));
                    return FALSE;
                }
                u1Byte0 = _Dmx_GetByte(u4HeaderBufStart, u4HeaderBufEnd, u4HdrAddr, u4HdrAddrOffset);
                u4HdrAddrOffset += 1;

                u4HdrAddrOffset += u1Byte0;
            }
            if (fgProgPacketSeq)
            {
                u4HdrAddrOffset += 2;
            }
            if (fgPstdBuffer)
            {
                // Todo: Optional check for '01' here.
                u4HdrAddrOffset += 2;
            }
            if (fgPesExt2)
            {
                UINT8 u1PesExt2Len;
                UINT32 u4ExtBufStart, u4ExtBufEnd, u4ExtBufRp, u4ExtBufWp;
                UINT32 u4ExtBufWpNew, u4ExtBufLen, u4ExtBufEmpty, u4HeaderRp;

                // Verify the PES header length for 1 more byte.
                if (u4HeaderByteCount < (u4HdrAddrOffset + 1))
                {
                    LOG(1/*5*/, "%d) PES header (len:%u, offset:%u) is too short!\n",
                        __LINE__, u4HeaderByteCount, (u4HdrAddrOffset + 1));
                    return FALSE;
                }
                u1Byte0 = _Dmx_GetByte(u4HeaderBufStart, u4HeaderBufEnd, u4HdrAddr, u4HdrAddrOffset);
                u4HdrAddrOffset += 1;

                // Optional check for the marker bit
                if ((u1Byte0 & 0x80) != 0x80)
                {
                    LOG(1/*5*/, "%d) Incorrect PES header marker!\n", __LINE__);
                    return FALSE;
                }

                u1PesExt2Len = u1Byte0 & 0x7F;  // PES_extension_field_length

                // Verify the PES header length.
                if (u4HeaderByteCount < (u4HdrAddrOffset + u1PesExt2Len))
                {
                    LOG(1/*5*/, "%d) PES header (len:%u, offset:%u) is too short!\n",
                        __LINE__, u4HeaderByteCount, (u4HdrAddrOffset + u1PesExt2Len));
                    return FALSE;
                }

                // Calculate the beginning address from which to read data.
#if 1
                u4HeaderRp = u4HdrAddr + u4HdrAddrOffset;
                if (u4HeaderRp >= u4HeaderBufEnd)
                {
                    u4HeaderRp -= (u4HeaderBufEnd - u4HeaderBufStart);
                }
                DMX_PANIC((u4HeaderBufStart <= u4HeaderRp) && (u4HeaderRp < u4HeaderBufEnd));
                DMX_PANIC_RET(FALSE);
#else
                u4HeaderRp = _DMX_AdvanceAddr(u4HdrAddr, u4HdrAddrOffset,
                        u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
                ASSERT(u4HeaderRp != 0);
#endif
                // Calculate the empty size of the PES external buffer.
                // All the pointers in the PES Ext buffer are virtual pointers.
                u4ExtBufStart = prPidStruct->u4ExtBufStart;
                u4ExtBufEnd = prPidStruct->u4ExtBufEnd + 1;  // !!!
                u4ExtBufLen = u4ExtBufEnd - u4ExtBufStart;
                u4ExtBufRp = prPidStruct->u4ExtBufRp;
                u4ExtBufWp = prPidStruct->u4ExtBufWp;
                u4ExtBufEmpty = EMPTYSIZE(u4ExtBufRp, u4ExtBufWp, u4ExtBufLen);
                if (u4ExtBufEmpty < u1PesExt2Len)
                {
                    LOG(3, "PES Ext buffer is full! (empty: %d, data: %d)\n",
                        u4ExtBufEmpty, u1PesExt2Len);
                    return FALSE;
                }

                VERIFY(_DMX_CopyRingBuffer(u4ExtBufWp, u4ExtBufStart,
                    u4ExtBufEnd, u4HeaderRp, u4HeaderBufStart,
                    u4HeaderBufEnd, u1PesExt2Len) == u4ExtBufWp);

                // Calculate the new WP in the PES extension buffer.
                u4ExtBufWpNew = u4ExtBufWp + u1PesExt2Len;
                if (u4ExtBufWpNew >= u4ExtBufEnd)
                {
                    u4ExtBufWpNew -= u4ExtBufLen;
                }
                DMX_PANIC((u4ExtBufStart <= u4ExtBufWpNew) && (u4ExtBufWpNew < u4ExtBufEnd));
                DMX_PANIC_RET(FALSE);

                // Set the return values.
                *pu4ExtBufRp = u4ExtBufWp;
                *pu4ExtBufWp = u4ExtBufWpNew;

                // Bookkeeping - update the RP and WP
                prPidStruct->u4ExtBufWp = u4ExtBufWpNew;

#if 0  // Debug
                UINT8 au1Data[4];
                LOG(1, "Copy %d bytes to the PES Ext buffer.\n", u1PesExt2Len);
                LOG(1, "Local  Ext RP: %08X, WP %08X\n", u4ExtBufWp, u4ExtBufWpNew);
                LOG(1, "Global Ext RP: %08X, WP %08X\n", prPidStruct->u4ExtBufRp, prPidStruct->u4ExtBufWp);

                ASSERT((u4ExtBufStart <= u4ExtBufWp) && (u4ExtBufWp < u4ExtBufEnd));
                au1Data[0] = (*(UINT8*)u4ExtBufWp);

                u4ExtBufWp++;
                if (u4ExtBufWp >= u4ExtBufEnd) u4ExtBufWp -= u4ExtBufLen;
                ASSERT((u4ExtBufStart <= u4ExtBufWp) && (u4ExtBufWp < u4ExtBufEnd));
                au1Data[1] = (*(UINT8*)u4ExtBufWp);

                u4ExtBufWp++;
                if (u4ExtBufWp >= u4ExtBufEnd) u4ExtBufWp -= u4ExtBufLen;
                ASSERT((u4ExtBufStart <= u4ExtBufWp) && (u4ExtBufWp < u4ExtBufEnd));
                au1Data[2] = (*(UINT8*)u4ExtBufWp);

                u4ExtBufWp++;
                if (u4ExtBufWp >= u4ExtBufEnd) u4ExtBufWp -= u4ExtBufLen;
                ASSERT((u4ExtBufStart <= u4ExtBufWp) && (u4ExtBufWp < u4ExtBufEnd));
                au1Data[3] = (*(UINT8*)u4ExtBufWp);

                LOG(1, "%02X %02X %02X %02X\n",
                    au1Data[0], au1Data[1], au1Data[2], au1Data[3]);
#endif

                return TRUE;
            }
        }
    }

    return FALSE;
}
#endif  // CC_DMX_PES_EXT_BUFFER


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
static BOOL _DmxParsePesHeader(UINT8 u1Pidx, UINT32 u4HeaderInfoAddr, UINT32 u4HeaderWp,
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
#ifdef CC_DMX_PES_AUDDESC
    UINT32 u4ExtOffset;
    UINT8 u1ADFad, u1ADPan, au1ExtData[10];
#endif // CC_DMX_PES_AUDDESC

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

#ifdef CC_DMX_PES_AUDDESC
    //----------------------------------------------------
    prPesHeader->fgContainAD = FALSE;
    u1ADFad = 0;
    u1ADPan = 0;
    x_memset((void*)au1ExtData, 0, sizeof(au1ExtData));
    u4ExtOffset = 9;
    if((u4HeaderFlag & 0xC0) == 0x80)   // PTS_DTS_FLAGS
    {
        u4ExtOffset += 5;
    }
    else if((u4HeaderFlag & 0xC0) == 0xC0)  // PTS_DTS_FLAGS
    {
        u4ExtOffset += 10;
    }

    if(u4HeaderFlag & 0x20)     // ESCR_FLAG
    {
        u4ExtOffset += 6;
    }

    if(u4HeaderFlag & 0x10)     // ES_RATE_FLAG
    {
        u4ExtOffset += 3;
    }

    if(u4HeaderFlag & 0x8)     // DSM_TRICK_MODE_FLAG
    {
        u4ExtOffset += 1;
    }

    if(u4HeaderFlag & 0x4)     // ADDITIONAL_COPY_INFO_FLAG
    {
        u4ExtOffset += 1;
    }

    if(u4HeaderFlag & 0x2)     // PES_CRC_FLAG
    {
        u4ExtOffset += 2;
    }

    if(u4HeaderFlag & 0x1)     // PES_EXTENSION_FLAG
    {
        UINT32 u4HeaderTmp;

        u4HeaderTmp = u4HeaderAddr + u4ExtOffset;
        if(u4HeaderTmp >= u4BufEnd)
        {
            u4HeaderTmp -= (u4BufEnd - u4BufStart);
        }
        // Copy to non-ring buffer to ease parsing
        VERIFY(_DMX_CopyRingBuffer((UINT32)au1ExtData, (UINT32)au1ExtData,
            (UINT32)(au1ExtData + 10), u4HeaderTmp, u4BufStart,
            u4BufEnd, 10) == (UINT32)au1ExtData);

        if(au1ExtData[0] & 0x80)
        {
            // Don't know compare AD_revision_text_tag: 0x4454474144
            if((au1ExtData[2] == 0x44) && (au1ExtData[3] == 0x54) && (au1ExtData[4] == 0x47) &&
               (au1ExtData[5] == 0x41) && (au1ExtData[6] == 0x44) &&
               (au1ExtData[7] == 0x31))
            {
                u1ADFad = au1ExtData[8];
                u1ADPan = au1ExtData[9];
                prPesHeader->fgContainAD = TRUE;
            }
        }
    }

    prPesHeader->u1ADFad = u1ADFad;
    prPesHeader->u1ADPan = u1ADPan;
/*
    if(prPesHeader->fgContainAD)
    {
        LOG(5, "fan:%d, pan:%d\n", u1ADFad, u1ADPan);
    }
*/
#endif  // CC_DMX_PES_AUDDESC

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
static UINT32 _DmxGetTsExtraAuidoInfo(UINT8 u1Pidx, UINT32 u4HeaderRp,
                            UINT32 u4HeaderSize, UINT32 u4HeaderWp,
                            UINT32 u4HeaderBufStart, UINT32 u4HeaderBufEnd,
                            DMX_AUDIO_PES_T *prPes)
{
    UINT32 u4PesHeaderSize, u4HeaderByteCount;
    UINT32 u4AudioInfoAddr, u4AudioInfoEndAddr, u4ExtraLen;
    UINT8 au1AudioInfo[PES_EXTRA_AUDIO_INFO_LEN];
    UINT8 au1Header[PES_HEADER_COPY_SIZE];
    DMX_AUDIO_TYPE_T eAudioType;
    UINT8* pu1Data;
    UINT32 u4NewHeaderRp;

    DMX_PANIC(u4HeaderRp != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC(u4HeaderWp != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC(u4HeaderBufStart != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC(u4HeaderBufEnd != 0);
    DMX_PANIC_RET(0);

    if(u4HeaderRp == 0)     //make klocwork happy
    {
        return 0;
    }

    if(prPes == NULL)
    {
        return 0;
    }

    // Important: initialize the extra audio info to 0.
    prPes->u4Info = 0;

    if (_DMX_GetPidInputType(u1Pidx) != DMX_IN_PLAYBACK_TS)
    {
        return 0;
    }

    eAudioType = _DMX_TS_GetAudioType(u1Pidx);
    if ((eAudioType != DMX_AUDIO_LPCM))
    {
        return 0;
    }

    u4ExtraLen = (PID_S_W(u1Pidx, 0) >> 24) & 0xFF;
    if (u4ExtraLen > 8)
    {
        return 0;
    }

    u4NewHeaderRp = u4HeaderRp + PES_HEADER_EXTRA_BYTES;
    if(u4NewHeaderRp >= u4HeaderBufEnd)
    {
        u4NewHeaderRp -= (u4HeaderBufEnd - u4HeaderBufStart);
    }

    // Parse PES header size
    VERIFY((UINT32)au1Header == _DMX_CopyRingBuffer((UINT32)au1Header,
        (UINT32)au1Header, (UINT32)(au1Header + PES_HEADER_COPY_SIZE),
        u4NewHeaderRp, u4HeaderBufStart, u4HeaderBufEnd,
        PES_HEADER_COPY_SIZE));

    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    pu1Data = au1Header + PES_HEADER_DATA_LENGTH_OFFSET;
    u4PesHeaderSize = pu1Data[0];

    if(u4PesHeaderSize + PES_HEADER_FIELD_OFFSET + u4ExtraLen > u4HeaderByteCount)
    {
        return 0; //may lost packet?
    }

    u4AudioInfoAddr = _DMX_AdvanceAddr(u4HeaderRp,
        (INT32)u4PesHeaderSize + PES_HEADER_FIELD_OFFSET + PES_HEADER_EXTRA_BYTES,
        u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);

    DMX_PANIC(u4AudioInfoAddr != 0);
    DMX_PANIC_RET(0);
    DMX_PANIC((u4AudioInfoAddr >= u4HeaderBufStart) && (u4AudioInfoAddr < u4HeaderBufEnd));
    DMX_PANIC_RET(0);

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
        HalInvalidateDCacheMultipleLine(u4Addr, u1Len);
        HalInvalidateDCacheMultipleLine(u4BufStart, (PIC_HEADER_LEN - u1Len));
        x_memcpy((void*)h, (void*)p, u1Len);
        x_memcpy((void*)(h + u1Len), (void*)u4BufStart,
            (SIZE_T)(PIC_HEADER_LEN - u1Len));
        p = h;
    }
    else
    {
        HalInvalidateDCacheMultipleLine(u4Addr, PIC_HEADER_LEN);
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

#ifdef CC_VDEC_FMT_DETECT
    // H264
    if((u1Type & 0x1F) == H264_PIC_START_NONIDR)
    {
        if((p[4] & 0x80) == 0x80)
        {
            u4PicType = H264_PIC_TYPE_NONIDR;
        }

        return u4PicType;
    }

    if((u1Type & 0x1F) == H264_PIC_START_SEQ)
    {
        if((p[5] & 0xF) == 0)
        {
            u4PicType = H264_PIC_TYPE_SEQ;
        }

        return u4PicType;
    }
#endif  // CC_VDEC_FMT_DETECT

    // MPEG
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
/** _DmxDetectH264PictureType
 *  Detect picture type at a given address
 *
 *  @param  u4Addr          The picture address
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxDetectH264PictureType(UINT32 u4Addr, UINT32 u4BufStart,
    UINT32 u4BufEnd)
{
    UINT32 u4PicType;
    UINT8* p;
    UINT8 u1Type, h[PIC_HEADER_LEN];

    // static UINT32 u4Count = 0;

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
            return H264_PIC_TYPE_UNKNOWN;
        }
        HalInvalidateDCacheMultipleLine(u4Addr, u1Len);
        HalInvalidateDCacheMultipleLine(u4BufStart, (PIC_HEADER_LEN - u1Len));
        x_memcpy((void*)h, (void*)p, u1Len);
        x_memcpy((void*)(h + u1Len), (void*)u4BufStart,
            (SIZE_T)(PIC_HEADER_LEN - u1Len));
        p = h;
    }
    else
    {
        HalInvalidateDCacheMultipleLine(u4Addr, PIC_HEADER_LEN);
    }

    // Check start code
    if ((p[0] != H264_PIC_START_CODE_1) || (p[1] != H264_PIC_START_CODE_2) ||
        (p[2] != H264_PIC_START_CODE_3))
    {
        return H264_PIC_TYPE_UNKNOWN;
    }

    // Check picture type
    u1Type = p[3];
    u4PicType = H264_PIC_TYPE_UNKNOWN;

#ifdef CC_VDEC_FMT_DETECT
    if(u1Type == PIC_START_CODE_PIC)
    {
        u4PicType = (p[5] >> 3) & 0x7;
        if ((u4PicType != PIC_TYPE_I) && (u4PicType != PIC_TYPE_P))
        {
            u4PicType = H264_PIC_TYPE_UNKNOWN;
        }

        return u4PicType;
    }
#endif  // CC_VDEC_FMT_DETECT

    switch (u1Type & 0x1F)
    {
    case H264_PIC_START_NONIDR:
        if((p[4] & 0x80) == 0x80)
        {
            u4PicType = H264_PIC_TYPE_NONIDR;
        }
        else
        {
            _u1H264SliceNum++;
        }
        break;

    case H264_PIC_START_IDR:
        if((p[4] & 0x80) == 0x80)
        {
            u4PicType = H264_PIC_TYPE_IDR;
        }
        else
        {
            _u1H264SliceNum++;
        }
        break;

    case H264_PIC_START_SEI:
        u4PicType = H264_PIC_TYPE_SEI;
        break;

    case H264_PIC_START_SEQ:
        u4PicType = H264_PIC_TYPE_SEQ;
        break;

    case H264_PIC_START_PIC:
        u4PicType = H264_PIC_TYPE_PIC;
        break;

    case H264_PIC_START_AU:
        u4PicType = H264_PIC_TYPE_AU;
        break;
#if 0 //we don't need to return this type now
    case H264_PIC_START_END_OF_SEQ:
        u4PicType = H264_PIC_START_END_OF_SEQ;
        break;
#endif

#if defined(CC_MVC_SUPPORT) || defined(CC_SW_MVC_ENABLE)
    case H264_PIC_START_PREFIX:
        if((p[12] & 0x80) == 0x80)
        {
        u4PicType = H264_PIC_TYPE_PREFIX;
        }
        break;

    case H264_PIC_START_SUB_SPS:
        u4PicType = H264_PIC_TYPE_SUB_SPS;
        break;

    case H264_PIC_START_CODED_SLICE_EXT:
        // we detect this picture type by  { 0x011400000080, 0xff1f00000080 }
        if((p[7] & 0x80) == 0x80)
        {
            u4PicType = H264_PIC_TYPE_CODED_SLICE_EXT;
        }
        break;
#endif // CC_MVC_SUPPORT

    default:
        //Printf("Unknown #3\n");
        break;
    }

    //Printf("H264 pic type: %d, idx:%d\n", u4PicType, u4Count++);

    return u4PicType;
}

#ifdef ENABLE_MULTIMEDIA
//-----------------------------------------------------------------------------
/** _DmxDetectMPEG4PictureType
 *  Detect picture type at a given address
 *
 *  @param  u4Addr          The picture address
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxDetectMPEG4PictureType(UINT32 u4Addr, UINT32 u4BufStart,
    UINT32 u4BufEnd)
{
    UINT32 u4PicType;
    UINT8* p;
    UINT8 u1Type, h[PIC_HEADER_LEN];

    // static UINT32 u4Count = 0;

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
            return MPEG4_PIC_TYPE_UNKNOWN;
        }
        HalInvalidateDCacheMultipleLine(u4Addr, u1Len);
        HalInvalidateDCacheMultipleLine(u4BufStart, (PIC_HEADER_LEN - u1Len));
        x_memcpy((void*)h, (void*)p, u1Len);
        x_memcpy((void*)(h + u1Len), (void*)u4BufStart,
            (SIZE_T)(PIC_HEADER_LEN - u1Len));
        p = h;
    }
    else
    {
        HalInvalidateDCacheMultipleLine(u4Addr, PIC_HEADER_LEN);
    }

    // Check start code
    if ((p[0] != MPEG4_PIC_START_CODE_1) || (p[1] != MPEG4_PIC_START_CODE_2) ||
        ((p[2] != MPEG4_PIC_START_CODE_3_1) && (p[2] != MPEG4_PIC_START_CODE_3_2)))
    {
        return MPEG4_PIC_TYPE_UNKNOWN;
    }

    // Check picture type
    u1Type = p[3];
    u4PicType = MPEG4_PIC_TYPE_OTHERS;

    switch (u1Type & 0xFF)
    {
    case MPEG4_PIC_START_CODE_PIC:
        u4PicType = MPEG4_PIC_TYPE_PIC;
        break;

    default:
        // Don't care others
        break;
    }

    return u4PicType;
}

//-----------------------------------------------------------------------------
/** GetHiBit
 *  @param  pu1data          The data for getting one bit
 */
//-----------------------------------------------------------------------------
static UINT8 _DmxGetHiBit(UINT8 *pu1data)
{
    UINT8 ret = 0;

    DMX_PANIC(pu1data != NULL);
    DMX_PANIC_RET(0);

    if(pu1data == NULL)     // klocwork
    {
        return 0;
    }

    ret = (*pu1data >> 7) & 0x1;
    *pu1data = *pu1data << 1;
    return ret;
}


//-----------------------------------------------------------------------------
/** _DmxDetectVC1PictureType
 *
 *  @param  u4Addr          The picture address
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxDetectVC1PictureType(UINT32 u4Addr, UINT32 u4BufStart,
    UINT32 u4BufEnd)
{
    UINT32 u4PicType;
    UINT8* p;
    UINT8 u1Type, h[PIC_VC1_HEADER_LEN], u1BitField;
    UINT8 u1FrameCodingMode;
    static BOOL fgInterlacedSource = 0;
    static INT32 i4CurrentTemporalField = 0; // 0:1st field or frame picture, 1: 2nd field
    static UINT8 u1SecondFieldType = 0;

    if ((u4Addr < u4BufStart) || (u4Addr >= u4BufEnd))
    {
        LOG(3, "Invalid picture address 0x%08x, pidx: %u\n");

        return PIC_TYPE_UNKNOWN;
    }

    // Check if the header is across FIFO boundary
    p = (UINT8*)u4Addr;
    if (((u4Addr + PIC_VC1_HEADER_LEN) - 1) >= u4BufEnd)
    {
        UINT8 u1Len = (UINT8)(u4BufEnd - u4Addr);
        if(u1Len >= PIC_VC1_HEADER_LEN)
        {
            return VC1_PIC_TYPE_UNKNOWN;
        }
        HalInvalidateDCacheMultipleLine(u4Addr, u1Len);
        HalInvalidateDCacheMultipleLine(u4BufStart, (PIC_HEADER_LEN - u1Len));
        x_memcpy((void*)h, (void*)p, u1Len);
        x_memcpy((void*)(h + u1Len), (void*)u4BufStart,
            (SIZE_T)(PIC_VC1_HEADER_LEN - u1Len));
        p = h;
    }
    else
    {
        HalInvalidateDCacheMultipleLine(u4Addr, PIC_HEADER_LEN);
    }

    // Check start code
    if ((p[0] != VC1_PIC_START_CODE_1) || (p[1] != VC1_PIC_START_CODE_2) ||
        (p[2] != VC1_PIC_START_CODE_3))
    {
        return VC1_PIC_TYPE_UNKNOWN;
    }

    u1Type = p[3];
    u1BitField = p[4];

    u4PicType = VC1_PIC_TYPE_UNKNOWN;
    switch (u1Type & 0xFF)
    {
    case VC1_PIC_START_CODE_SEQ:
        fgInterlacedSource = ((p[9] & 0x40) == 0x40) ? 1 : 0;
        u4PicType = VC1_PIC_TYPE_SEQ_START;
        break;

    case VC1_PIC_START_CODE_ENTRY:
        u4PicType = VC1_PIC_TYPE_ENTRY;
        break;

    case VC1_PIC_START_CODE_FIELD:
    case VC1_PIC_START_CODE_FRAME:
        if( i4CurrentTemporalField == 1 )
        {
            u1FrameCodingMode = (UINT8)INTERLACEFIELD;
        }
        else
        {
            if (fgInterlacedSource)
            {
                if (0 == _DmxGetHiBit(&u1BitField))
                {
                    u1FrameCodingMode = (UINT8)PROGRESSIVE;
                }
                else
                {
                    if (0 == _DmxGetHiBit(&u1BitField))
                    {
                        u1FrameCodingMode = (UINT8)INTERLACEFRAME;
                    }
                    else
                    {
                        u1FrameCodingMode = (UINT8)INTERLACEFIELD;
                    }
                }
            }
            else
            {
                u1FrameCodingMode = (UINT8)PROGRESSIVE;
            }
        }
        if ((u1FrameCodingMode == (UINT8)PROGRESSIVE) ||
            (u1FrameCodingMode == (UINT8)INTERLACEFRAME))
        {
            if (0 == _DmxGetHiBit(&u1BitField))
            {
                u4PicType = VC1_PIC_TYPE_P;
            }
            else if (0 == _DmxGetHiBit(&u1BitField))
            {
                u4PicType = VC1_PIC_TYPE_B;
            }
            else if (0 == _DmxGetHiBit(&u1BitField))
            {
                u4PicType = VC1_PIC_TYPE_I;
            }
            else if (0 == _DmxGetHiBit(&u1BitField))
            {
                u4PicType = VC1_PIC_TYPE_BI;
            }
            else
            {
                u4PicType = VC1_PIC_TYPE_SKIP;
            }
        }
        else if(u1FrameCodingMode == (UINT8)INTERLACEFIELD)
        {
            if(i4CurrentTemporalField == 0)
            {
                UINT32 u4Code = (_DmxGetHiBit(&u1BitField) << 2) |
                                (_DmxGetHiBit(&u1BitField) << 1) |
                                (_DmxGetHiBit(&u1BitField) << 0);
                u4PicType = _u1vopFirstFieldType[u4Code];
                u1SecondFieldType = _u1vopSecondFieldType[u4Code];
            }
            else
            {
                u4PicType = u1SecondFieldType;
            }
            i4CurrentTemporalField ^= 1;
        }
        break;

    default:
        // Don't care others
        break;
    }

    return u4PicType;
}


//-----------------------------------------------------------------------------
/** _DmxDetectWMVPictureType
 *  Detect the picture type at the given address.  For now, the downstream
 *  decoder do no ask Demux to find the picture type of WMV.  Therefore, we
 *  just pick a type at will, in this case the I frame, as the picture type.
 *
 *  @param  u4Addr          The picture address
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxDetectWMVPictureType(UINT32 u4Addr, UINT32 u4BufStart,
    UINT32 u4BufEnd)
{
    UNUSED(u4Addr);
    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);
    return PIC_TYPE_I;          // Pick a picture type at will.
}
#endif  // ENABLE_MULTIMEDIA


//-----------------------------------------------------------------------------
/** _DmxDetectAVSPictureType
 *
 *  @param  u4Addr          The picture address
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxDetectAVSPictureType(UINT32 u4Addr, UINT32 u4BufStart,
    UINT32 u4BufEnd)
{
    UINT32 u4PicType;
    UINT8* p;
    UINT8 u1Type, h[PIC_HEADER_LEN];

    u4PicType = AVS_PIC_TYPE_UNKNOWN;

    if ((u4Addr < u4BufStart) || (u4Addr >= u4BufEnd))
    {
        LOG(3, "Invalid picture address 0x%08x, pidx: %u\n");

        return u4PicType;
    }

    // Check if the header is across FIFO boundary
    p = (UINT8*)u4Addr;
    if (((u4Addr + PIC_HEADER_LEN) - 1) >= u4BufEnd)
    {
        UINT8 u1Len = (UINT8)(u4BufEnd - u4Addr);
        if(u1Len >= PIC_HEADER_LEN)
        {
            return u4PicType;
        }
        HalInvalidateDCacheMultipleLine(u4Addr, u1Len);
        HalInvalidateDCacheMultipleLine(u4BufStart, (PIC_HEADER_LEN - u1Len));
        x_memcpy((void*)h, (void*)p, u1Len);
        x_memcpy((void*)(h + u1Len), (void*)u4BufStart,
            (SIZE_T)(PIC_HEADER_LEN - u1Len));
        p = h;
    }
    else
    {
        HalInvalidateDCacheMultipleLine(u4Addr, PIC_HEADER_LEN);
    }

    // Check start code
    // 00 00 01
    if ((p[0] != AVS_PIC_START_CODE_1) || (p[1] != AVS_PIC_START_CODE_2) ||
        (p[2] != AVS_PIC_START_CODE_3))
    {
        return u4PicType;
    }

    // Check picture type
    u1Type = p[3];

    switch (u1Type)
    {
    case AVS_PIC_START_CODE_SEQ_START:
        u4PicType = AVS_PIC_TYPE_SEQ_START;
        break;

    case AVS_PIC_START_CODE_SEQ_END:
        u4PicType = AVS_PIC_TYPE_SEQ_END;
        break;

    case AVS_PIC_START_CODE_SEQ_USR_DATA:
        u4PicType = AVS_PIC_TYPE_USR_DATA;
        break;

    case AVS_PIC_START_CODE_SEQ_PIC_I:
        u4PicType = AVS_PIC_TYPE_PIC_I;
        break;

    case AVS_PIC_START_CODE_SEQ_EXT:
        u4PicType = AVS_PIC_TYPE_EXT;
        break;

    case AVS_PIC_START_CODE_SEQ_PIC_PB:
        u4PicType = AVS_PIC_TYPE_PIC_PB;
        break;

    case AVS_PIC_START_CODE_SEQ_VID_EDIT:
        u4PicType = AVS_PIC_TYPE_VID_EDIT_CODE;
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
static UINT32 _DmxDetectPictureType(DMX_VIDEO_TYPE_T eVideoType, UINT32 u4Addr, UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 u4Type = PIC_TYPE_UNKNOWN;

    switch(eVideoType)
    {
    case DMX_VIDEO_MPEG:
        u4Type = _DmxDetectMPEGPictureType(u4Addr, u4BufStart, u4BufEnd);
        break;

    case DMX_VIDEO_H264:
        u4Type = _DmxDetectH264PictureType(u4Addr, u4BufStart, u4BufEnd);
        break;

#ifdef ENABLE_MULTIMEDIA
    case DMX_VIDEO_MPEG4:
        u4Type = _DmxDetectMPEG4PictureType(u4Addr, u4BufStart, u4BufEnd);
        break;

    case DMX_VIDEO_VC1:
        u4Type = _DmxDetectVC1PictureType(u4Addr, u4BufStart, u4BufEnd);
        break;

    case DMX_VIDEO_WMV7:
    case DMX_VIDEO_WMV8:
    case DMX_VIDEO_WMV9:
        u4Type = _DmxDetectWMVPictureType(u4Addr, u4BufStart, u4BufEnd);
        break;
#endif  // ENABLE_MULTIMEDIA

    case DMX_VIDEO_AVS:
        u4Type = _DmxDetectAVSPictureType(u4Addr, u4BufStart, u4BufEnd);
        break;

    default:
        //LOG(3, "Unknown video type!\n");
        break;
    }

    return u4Type;
}


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
    INT8 i1VCodeOffset;
	UINT8 u1ErrorCount = 0;

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

    u4HeaderBufStart = VIRTUAL(PID_S_W(u1Pidx, 10));
    u4HeaderBufEnd = VIRTUAL(PID_S_W(u1Pidx, 11) + 1);
    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4HeaderBufStart,
        u4HeaderBufEnd - u4HeaderBufStart);
    #endif
_HeaderHandling:
		// Important: Read HwPicNum before the Header WP is extracted.
		// uCode's write PES header flow is Header byte count -> Header -> Header WP -> HwPicNum.

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;

    u4HeaderWp = VIRTUAL(PID_S_W(u1Pidx, 13));
    DMX_PANIC(_DMX_IsAligned(u4HeaderRp, PES_HEADER_ALIGNMENT));
    DMX_PANIC_RET(FALSE);

    DMX_PANIC((u4HeaderRp >= u4HeaderBufStart) && (u4HeaderRp < u4HeaderBufEnd));
    DMX_PANIC_RET(FALSE);

    // Important: Read WP before the "header byte count" is extracted.
    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    u4FrontHeaderSize = _DMX_Align(u4HeaderByteCount, PES_HEADER_ALIGNMENT)
        + PES_HEADER_EXTRA_BYTES;
    u4HeaderSize = u4FrontHeaderSize + (u4HwPicNum * 8);	// 5368
	if(u4HeaderSize > DATASIZE(u4HeaderRp, u4HeaderWp, u4HeaderBufEnd - u4HeaderBufStart))
	{
		  LOG(3, "Header buffer size is not enough\n");
          #ifndef CC_DMX_FLUSH_ALL
          HalInvalidateDCacheMultipleLine(u4HeaderBufStart,u4HeaderBufEnd - u4HeaderBufStart);
          #endif
		  u1ErrorCount++;
		  if(u1ErrorCount < 100)
		  {
		  goto _HeaderHandling;
	      }
		  else
		  {
			  LOG(3, "Can't get reasonable picture number.\n");
			  DMX_PANIC(0);
			  DMX_PANIC_RET(FALSE);
		  }
	 }

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
        VERIFY(_DMX_UpdateVideoWritePointer(u1Pidx, u1Channel, PHYSICAL(u4Wp)));
    }

    u4NextPic = prPidStruct->u4NextPic;
    DMX_PANIC(u4NextPic <= u4HwPicNum);
    DMX_PANIC_RET(FALSE);

    for (i = u4NextPic; i < u4HwPicNum; i++)
    {
        UINT32 u4PicInfo, u4Addr, u4PhyAddr;
        UINT8 u1VCIdx;

        u4PicInfoAddr = _DMX_AdvanceAddr(u4HeaderRp,
            (INT32)(u4FrontHeaderSize + (i * 8)),
            u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
        DMX_PANIC(u4PicInfoAddr != 0);
        DMX_PANIC_RET(FALSE);
        if(u4PicInfoAddr == 0)      // klocwork
        {
            return FALSE;
        }

        u4PhyAddr = *(UINT32*)u4PicInfoAddr;
        u4Addr = VIRTUAL(u4PhyAddr);
        DMX_PANIC((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd));
        DMX_PANIC_RET(FALSE);

        u4PicInfo = _DMX_AdvanceAddr(u4HeaderRp, (INT32)(u4FrontHeaderSize + ((i * 8) + 4)),
            						 u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd);
        u1VCIdx = 0;
        for(u1VCIdx = 0; u1VCIdx < 10; u1VCIdx++)
        {
            if((*(UINT32*)u4PicInfo) & (1 << u1VCIdx))
            {
                break;
            }
        }

        if (fgIsToDecoder)
        {
            DMX_PES_HEADER_T rPesHeader;
            DMX_PES_MSG_T rPes;
            UINT32 u4FrameAddr;
            static UINT32 _u4VC1Pts = 0, _u4VC1Dts = 0;
            DMX_VIDEO_TYPE_T eVideoType;
            DMX_INPUT_TYPE_T eInputType;

            eVideoType = _DMX_GetPidxVideoType(u1Pidx);
            eInputType = _DMX_GetPidInputType(u1Pidx);

            // Parse PES header to get PTS/DTS
            x_memset((void*)&rPes, 0, sizeof(rPes));
            if (i == 0)
            {
#ifdef ENABLE_MULTIMEDIA
                if(eInputType == DMX_IN_PLAYBACK_MM)
                {
                    _DMX_MM_SetHeader(u1Pidx, &rPes);
                    if (rPes.fgEOS)
                    {
                        rPes.fgEOS = FALSE;
                        _fgSendEOSMessage = TRUE;
                    }
                    // Store PTS/DTS for VC1 for later use when (i == 0).
                    if(eVideoType == DMX_VIDEO_VC1)
                    {
                        _u4VC1Pts = rPes.u4Pts;
                        _u4VC1Dts = rPes.u4Dts;
                    }
                }
                else if (eInputType == DMX_IN_PLAYBACK_PS)
                {
                    _DMX_PS_CPU_SetVideoHeader(u1Pidx, &rPes);
                }
                else if (eInputType == DMX_IN_PLAYBACK_ES)
                {
                    // No PTS/DTS in ES.
                }
                else
#endif  // ENABLE_MULTIMEDIA
                {
                    if (_DmxParsePesHeader(u1Pidx, u4HeaderRp, u4HeaderWp,
                        u4HeaderBufStart, u4HeaderBufEnd, prPidStruct,
                        &rPesHeader))
                    {
                        rPes.u4Pts = rPesHeader.u4Pts;
                        rPes.u4Dts = rPesHeader.u4Dts;
                        rPes.fgPtsDts = TRUE;
#ifdef CC_DMX_PRINF_STC_LOG
                        if(_fgEnableVideoPtsInfo)
                        {
                            UINT32 u4Stc = STC_GetStcValue(0);
                            // Printf PTS/STC relationship
                            LOG(0, "video, pts:0x%x, stc:0x%x, dif:0x%x\n", rPes.u4Pts, u4Stc,
                                u4Stc - rPes.u4Pts);
                        }
#endif // CC_DMX_PRINF_STC_LOG
                    }
#ifdef CC_DMX_PES_EXT_BUFFER
                    if(eVideoType == DMX_VIDEO_H264)
                    {
                        UINT32 u4ExtBufRp, u4ExtBufWp;
                        u4ExtBufRp = 0;
                        u4ExtBufWp = 0;
                        if (_DmxParsePesExtData(u4HeaderRp, u4HeaderWp,
                            u4HeaderBufStart, u4HeaderBufEnd, u4HeaderByteCount,
                            prPidStruct, &u4ExtBufRp, &u4ExtBufWp))
                        {
                            rPes.u4ExtBufStart = prPidStruct->u4ExtBufStart;
                            rPes.u4ExtBufEnd = prPidStruct->u4ExtBufEnd;
                            rPes.u4ExtBufCurrentRp = u4ExtBufRp;
                            rPes.u4ExtBufCurrentWp = u4ExtBufWp;
                        }
                    }
#endif  // CC_DMX_PES_EXT_BUFFER
                }
            }

            // Prepare PES info
            i1VCodeOffset = _DMX_GetVCode_Offset(eVideoType, u1VCIdx);
            ASSERT(i1VCodeOffset != DMX_VNULL);	// New function, so add assert to debug
            u4FrameAddr = _DMX_AdvanceAddr(u4Addr, i1VCodeOffset, u4Wp, u4BufStart, u4BufEnd);  // MT5391
            rPes.ePidType = DMX_PID_TYPE_ES_VIDEO;
            rPes.u4FrameType = _DmxDetectPictureType(eVideoType, u4FrameAddr, u4BufStart, u4BufEnd);
            rPes.u4FrameAddr = PHYSICAL(u4FrameAddr);
            rPes.u4BufStart = PHYSICAL(u4BufStart);
            rPes.u4BufEnd = PHYSICAL(u4BufEnd);
            rPes.u4Wp = PHYSICAL(u4Wp);
            rPes.u1Pidx = u1Pidx;
            rPes.u1Channel = u1Channel;
            rPes.u1DeviceId = prPidStruct->u1DeviceId;
            rPes.eMsgType = DMX_PES_MSG_TYPE_PES;

            if(eVideoType == DMX_VIDEO_H264)
            {
                if((rPes.u4FrameType == H264_PIC_TYPE_NONIDR) ||
                   (rPes.u4FrameType == H264_PIC_TYPE_IDR))
                {
                    rPes.u1SliceCount = _u1H264SliceNum;
                    _u1H264SliceNum = 0;
                }
            }
            else if((eInputType == DMX_IN_PLAYBACK_MM) &&
                    (eVideoType == DMX_VIDEO_VC1))
            {
                // Retrieve stored PTS/DTS for VC1 in MM mode when (i != 0).
                rPes.u4Pts = _u4VC1Pts;
                rPes.u4Dts = _u4VC1Dts;
            }
            else if((eInputType == DMX_IN_PLAYBACK_MM) && (eVideoType == DMX_VIDEO_MPEG4))
			{
				_DMX_MM_SetHeader(u1Pidx, &rPes);
				/* sy's question: is it necessary ? */
                if (rPes.fgEOS)
                {
                    rPes.fgEOS = FALSE;
                    _fgSendEOSMessage = TRUE;
                }
			}

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

    prPidStruct->u4PesRp = VIRTUAL(u4Wp);

    return fgIsToDecoder;
}

#ifdef ENABLE_MULTIMEDIA
//-----------------------------------------------------------------------------
/** _DmxProcessVideoChunk
 *  Process video chunk
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessVideoChunk(UINT8 u1Pidx, BOOL fgComplete)
{
    UINT32 u4Wp, u4BufStart, u4BufEnd, u4PesRp;
    UINT32 u4HeaderRp, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd;
    UINT32 u4HwPicNum, u4HeaderByteCount, u4HeaderSize;
    PID_STRUCT_T* prPidStruct;
    BOOL fgIsToDecoder;
    DMX_PES_MSG_T rPes;
    UINT8 u1Channel;
    //static BOOL _fgSendEOSMessage = FALSE;

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
//    ASSERT(u4HeaderRp == VIRTUAL(PID_S_W(u1Pidx, 14)));    // Double confirm
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

    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4HeaderBufStart,
        u4HeaderBufEnd - u4HeaderBufStart);
    #endif

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;

    if (u4HwPicNum != 0)
    {
        LOG(1, "Error: non-zero (%u) picture number in video chunk channel (%u)\n",
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
    u4PesRp = VIRTUAL(prPidStruct->u4PesRp);
    DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
    DMX_PANIC_RET(FALSE);

    fgIsToDecoder = _DMX_IsToDecoder();

    // Update decoder's Write pointer and send the video chunk.
    if (fgIsToDecoder)
    {
        DMX_VIDEO_TYPE_T eVideoType;

        VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));
        VERIFY(_DMX_UpdateVideoWritePointer(u1Pidx, u1Channel, PHYSICAL(u4Wp)));

        x_memset((void*)&rPes, 0, sizeof(rPes));
        rPes.u4Pts = 0;
        rPes.u4Dts = 0;
        rPes.fgPtsDts = FALSE;
        rPes.fgEOS = FALSE;

        if(_DMX_GetPidInputType(u1Pidx) == DMX_IN_PLAYBACK_MM)
        {
            _DMX_MM_SetHeader(u1Pidx, &rPes);
/*          SWDMX send dummy final chunk now
            if (rPes.fgEOS)
            {
                rPes.fgEOS = FALSE;
                _fgSendEOSMessage = TRUE;
            }
*/
        }
        else
        {
            LOG(3, "Type not match\n");
            DMX_PANIC(0);
            DMX_PANIC_RET(FALSE);
        }

        // Prepare PES info
        // The variable u4FrameType needs to be set to an appropriate value
        // because it is checked before _DmxAddPictureHeader() is called.
        // If the check fails, then the EOS message will not be sent.
        // For H.264 format, the type "H264_PIC_TYPE_PIC" is chosen.
        // For MPEG2 or other format,  the type "PIC_TYPE_I" is chosen.
        rPes.ePidType = DMX_PID_TYPE_ES_VIDEO;
        eVideoType = _DMX_GetPidxVideoType(u1Pidx);
        if (eVideoType == DMX_VIDEO_H264)
        {
            rPes.u4FrameType = H264_PIC_TYPE_PIC;
        }
        else if (eVideoType == DMX_VIDEO_VC1)
        {
            rPes.u4FrameType = VC1_PIC_TYPE_I;
        }
        else
        {
            rPes.u4FrameType = PIC_TYPE_I;
        }
        rPes.u4FrameAddr = PHYSICAL(u4PesRp);
        rPes.u4BufStart = PHYSICAL(u4BufStart);
        rPes.u4BufEnd = PHYSICAL(u4BufEnd);
        rPes.u4Wp = PHYSICAL(u4Wp);
        rPes.u1Pidx = u1Pidx;
        rPes.u1Channel = u1Channel;
        rPes.u1DeviceId = prPidStruct->u1DeviceId;
        rPes.eMsgType = DMX_PES_MSG_TYPE_PES;

        if (!_DMX_SendPictureHeader(&rPes))
        {
            LOG(3, "Failed to send picture!\n");
        }
    }

    if (fgComplete)
    {
        // Update header read pointer
        u4HeaderRp = _DMX_AdvanceAddr(u4HeaderRp, (INT32)u4HeaderSize, u4HeaderWp,
            u4HeaderBufStart, u4HeaderBufEnd);
        DMX_PANIC(u4HeaderRp != 0);
        DMX_PANIC_RET(FALSE);
        PID_S_W(u1Pidx, 14) = PHYSICAL(u4HeaderRp);
        prPidStruct->u4NextPic = 0;
/*
        if (_fgSendEOSMessage)
        {
            _fgSendEOSMessage = FALSE;
            if (!_DMX_SendEOSMessage(u1Pidx))
            {
                LOG(5, "Failed to send the EOS message!\n");
            }
        }
*/
        // Debug
//      LOG(7, "Pidx %u payload size: %u\n", u1Pidx, rPesHeader.u4PayloadSize);
    }

    prPidStruct->u4PesRp = u4Wp;

    // Suppress warnings in lint and release builds
    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);

    return fgIsToDecoder;

}
#endif  // ENABLE_MULTIMEDIA

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

#ifdef ENABLE_MULTIMEDIA
    if((_DMX_GetPidInputType(u1PidIndex) == DMX_IN_PLAYBACK_MM) &&
       (!_DMX_MM_GetSearchStartCode(u1PidIndex)))
    {
        return _DmxProcessVideoChunk(u1PidIndex, fgComplete);
    }
#endif  // ENABLE_MULTIMEDIA

    fgProcessed = _DmxProcessVideoPesHeader(u1PidIndex, fgComplete);

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
    UINT8 u1ErrorCount = 0;
	UINT8 u1ErrorCount1 = 0;

#ifdef ENABLE_MULTIMEDIA
    static BOOL _fgSendEOSMessage = FALSE;
    DMX_INPUT_TYPE_T eInputType;
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

    x_memset((void*)&rPes, 0, sizeof(rPes));

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

    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4HeaderBufStart,(u4HeaderBufEnd - u4HeaderBufStart));
    #endif

_AuHeaderHandling:
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
	if(u4HeaderSize > DATASIZE(u4HeaderRp, u4HeaderWp, u4HeaderBufEnd - u4HeaderBufStart))
	{
        LOG(3, "Header buffer size is not enough\n");
        #ifndef CC_DMX_FLUSH_ALL
        HalInvalidateDCacheMultipleLine(u4HeaderBufStart,u4HeaderBufEnd - u4HeaderBufStart);
        #endif
        u1ErrorCount++;
        if(u1ErrorCount < 100)
        {
            goto _AuHeaderHandling;
        }
        else
        {
            LOG(3, "Can't get reasonable picture number.\n");
            DMX_PANIC(0);
            DMX_PANIC_RET(FALSE);
        }
	 }

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
    while((u4Wp < u4BufStart) || (u4Wp >= u4BufEnd))
    {
       LOG(3,"audio Wp wrong, invalid cache and read data from DRAM again %d\n",u1ErrorCount1);
	   #ifdef SYNC_PES_HEADER
       #ifndef CC_DMX_FLUSH_ALL
	   HalInvalidateDCacheMultipleLine(u4WpInfoAddr,8);
	   #endif
	   prPidStruct->u4Wp = VIRTUAL(*(UINT32*)u4WpInfoAddr);
	   #endif
	   u4Wp = VIRTUAL(prPidStruct->u4Wp);
	   u1ErrorCount1++;
	   if(u1ErrorCount1>50)
	   {
	    LOG(3,"retry read Wp from DRAM fail, DMX PANIC \n",u1ErrorCount1);
	    DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
        DMX_PANIC_RET(FALSE);
		break;
	   }
    }

    // no cache operation to AFIFO

#ifdef ENABLE_MULTIMEDIA
    eInputType = _DMX_GetPidInputType(u1Pidx);
    if(eInputType == DMX_IN_PLAYBACK_TS)
    {
        u4ExtraAudioInfoLen = _DmxGetTsExtraAuidoInfo(u1Pidx, u4HeaderRp,
            u4HeaderSize, u4HeaderWp, u4HeaderBufStart, u4HeaderBufEnd, &rPes);
        // Force set extra audio info len to zero.
        // uCode is already plus this size into PES header size in v5.43
        u4ExtraAudioInfoLen = 0;
    }
    else if(eInputType == DMX_IN_PLAYBACK_PS)
    {
        u4ExtraAudioInfoLen = 0;
        _DMX_PS_CPU_GetExtraAudioInfo(u1Pidx, &rPes);
    }
#endif  // ENABLE_MULTIMEDIA

    fgIsToDecoder = _DMX_IsToDecoder();

#ifdef TIME_SHIFT_SUPPORT
    rPes.u4TickNum = 0;
#endif

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
#ifdef CC_DMX_PES_AUDDESC
        rPes.fgContainAD = FALSE;
#endif //  CC_DMX_PES_AUDDESC

#ifdef ENABLE_MULTIMEDIA
        eInputType = _DMX_GetPidInputType(u1Pidx);
        if(eInputType == DMX_IN_PLAYBACK_MM)
        {
            _DMX_MM_SetAudioHeader(u1Pidx, &rPes);
            if (rPes.fgEOS)
            {
                rPes.fgEOS = FALSE;
                _fgSendEOSMessage = TRUE;
            }

            // if it is an EOS pes with ignore data, we will not send this data to decoder.
            // Sometimes swdmx will send eos pes with 4 bytes garbage and this make audio noise.
            if(!(_fgSendEOSMessage && rPes.fgIgnoreData))
            {
#ifdef CC_DMX_AUDIO_PREBUF
                VERIFY(_DMX_AudHandler_AddPes(&rPes));
#else
                VERIFY(_DMX_SendAudioPes(&rPes));
#endif  // CC_DMX_AUDIO_PREBUF
            }
        }
#ifdef CC_DMX_PS_CPU
        else if (eInputType == DMX_IN_PLAYBACK_PS)
        {
            _DMX_PS_CPU_SetAudioHeader(u1Pidx, &rPes);
            VERIFY(_DMX_SendAudioPes(&rPes));
        }
#endif  // CC_DMX_PS_CPU
        else
#endif  // ENABLE_MULTIMEDIA
        {
            if (_DmxParsePesHeader(u1Pidx, u4HeaderRp, u4HeaderWp,
                u4HeaderBufStart, u4HeaderBufEnd, prPidStruct,
                &rPesHeader))
            {
                rPes.u4Pts = rPesHeader.u4Pts;
                rPes.u4Dts = rPesHeader.u4Dts;
                rPes.fgCopyright = rPesHeader.fgCopyright;
                rPes.fgOriginal = rPesHeader.fgOriginal;
#ifdef CC_DMX_PRINF_STC_LOG
                if(_fgEnableAudioPtsInfo)
                {
                    UINT32 u4Stc = STC_GetStcValue(0);
                    // Printf PTS/STC relationship
                    LOG(0, "audio, pts:0x%x, stc:0x%x, dif:0x%x\n", rPes.u4Pts, u4Stc,
                        u4Stc - rPes.u4Pts);
                }
#endif // CC_DMX_PRINF_STC_LOG
#ifdef CC_DMX_PES_AUDDESC
                rPes.u1ADFad = rPesHeader.u1ADFad;
                rPes.u1ADPan = rPesHeader.u1ADPan;
                rPes.fgContainAD = rPesHeader.fgContainAD;
#endif  // CC_DMX_PES_AUDDESC
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
            if(_DMX_GetPidInputType(u1Pidx) == DMX_IN_PLAYBACK_MM)
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

    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4HeaderBufStart,
        u4HeaderBufEnd - u4HeaderBufStart);
    #endif

    u4HwPicNum = (*(UINT32*)u4HeaderRp) & 0xff;
//  DMX_PANIC(u4HwPicNum == 0);
//    DMX_PANIC_RET(FALSE);
    u4HeaderByteCount = (*(UINT32*)u4HeaderRp) >> 16;
    u4HeaderSize = _DMX_Align(u4HeaderByteCount, PES_HEADER_ALIGNMENT) +
        PES_HEADER_EXTRA_BYTES + (u4HwPicNum * 8);

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

        #ifndef CC_DMX_FLUSH_ALL
        HalInvalidateDCacheMultipleLine(u4BufStart, u4BufEnd - u4BufStart);
        #endif

        if (_DmxParsePesHeader(u1Pidx, u4HeaderRp, u4HeaderWp, u4HeaderBufStart,
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

    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4HeaderBufStart,
        u4HeaderBufEnd - u4HeaderBufStart);
    #endif

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

        #ifndef CC_DMX_FLUSH_ALL
        HalInvalidateDCacheMultipleLine(u4BufStart, u4BufEnd - u4BufStart);
        #endif

        if (_DmxParsePesHeader(u1Pidx, u4HeaderRp, u4HeaderWp, u4HeaderBufStart,
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
    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4BufStart, u4BufEnd - u4BufStart);
    #endif

    u4DataSize = DATASIZE(u4PesRp, u4Wp, u4BufEnd - u4BufStart);

    if(_DMX_GetPidInputType(u1Pidx) == DMX_IN_BROADCAST_TS)
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
        rPes.pvInstanceTag = prPidStruct->pvInstanceTag;

        fgProcessed = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_PES,
            (UINT32)&rPes, prPidStruct->pvNotifyTag);
    }   // if

    prPidStruct->u4PesRp = u4Wp;

    return fgProcessed;
}

#ifdef ENABLE_MULTIMEDIA
//-----------------------------------------------------------------------------
/** _DmxProcessPesTime
 *  Process PES's with extra time information
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval Data processed or not
 */
//-----------------------------------------------------------------------------
static BOOL _DmxProcessPesTime(UINT8 u1Pidx)
{
    UINT32 u4BufStart, u4BufEnd, u4PesRp, u4Wp;
    PID_STRUCT_T* prPidStruct;
    BOOL fgProcessed = FALSE;
    UINT32 u4DataSize;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
    u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
    u4PesRp = VIRTUAL(prPidStruct->u4PesRp);
    DMX_PANIC((u4PesRp >= u4BufStart) && (u4PesRp < u4BufEnd));
    DMX_PANIC_RET(FALSE);
    u4Wp = VIRTUAL(prPidStruct->u4Wp);
    DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
    DMX_PANIC_RET(FALSE);
    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4BufStart, u4BufEnd - u4BufStart);
    #endif

    u4DataSize = DATASIZE(u4PesRp, u4Wp, u4BufEnd - u4BufStart);

    if (prPidStruct->pfnNotify != NULL)
    {
        DMX_NOTIFY_INFO_PES_TIME_T rPes;
        DMX_MM_DATA_T *prDMXMMData;

        rPes.u4DataAddr = u4PesRp;
        rPes.u4DataSize = u4DataSize;
        rPes.u1SerialNumber = prPidStruct->u1SerialNumber;
        prDMXMMData = (DMX_MM_DATA_T *)_DMXMMGetData();
        rPes.u4Pts = prDMXMMData->u4Pts;
        rPes.u4Dts = prDMXMMData->u4Dts;

        fgProcessed = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_PES_TIME,
            (UINT32)&rPes, prPidStruct->pvNotifyTag);
    }   // if

    prPidStruct->u4PesRp = u4Wp;

    return fgProcessed;
}
#endif  // ENABLE_MULTIMEDIA


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
    UINT32 u4BufStart, u4BufEnd;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    prPidStruct->u4Wp = VIRTUAL(PID_S_W(u1Pidx, 8));
    u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
    u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
    #ifndef CC_DMX_FLUSH_ALL
    HalInvalidateDCacheMultipleLine(u4BufStart, u4BufEnd - u4BufStart);
    #endif

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

    //LOG(5, "Pidx %u scrambling state: %s\n", u1Pidx, fgScrambled ? "SCRAMBLED" : "CLEAR");
    LOG(6, "Pidx %u scrambling state: %s\n", u1Pidx, fgScrambled ? "SCRAMBLED" : "CLEAR");

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

    #ifdef CC_DMX_FLUSH_ALL
    HalFlushInvalidateDCache();
    #endif

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

#ifdef ENABLE_MULTIMEDIA
    case DMX_PID_TYPE_PES_TIME:
        fgProcessed = _DmxProcessPesTime(u1PidIndex);
        break;
#endif  // ENABLE_MULTIMEDIA

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

    if((prPidStruct->ePidType == DMX_PID_TYPE_PES) ||
       (prPidStruct->ePidType == DMX_PID_TYPE_PES_TIME))
    {
        // Ignore fgPrcessed
        return;
    }

    if (!fgProcessed)
    {
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
    if ((_DMX_GetPidInputType(u1Pidx) == DMX_IN_PLAYBACK_MM) ||
        (_DMX_GetPidInputType(u1Pidx) == DMX_IN_PLAYBACK_PS) ||
        (_DMX_GetPidInputType(u1Pidx) == DMX_IN_PLAYBACK_ES))
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

    if ((prPidStruct->ePidType != DMX_PID_TYPE_ES_VIDEO) && (prPidStruct->ePidType != DMX_PID_TYPE_PSI))
    {
        // Do not handle FIFO full other than video ES
        return;
    }

    // Disable PID
    rPid.fgEnable = FALSE;
    VERIFY(_DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid, FALSE));

    // Send full message to thread
    if (prPidStruct->ePidType == DMX_PID_TYPE_PSI)
    {
        UNUSED(_DMX_SendPurgeMessage(u1Pidx, FALSE));
    }
    else
    {
        UNUSED(_DMX_SendPurgeMessage(u1Pidx, TRUE));
    }
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
    if((u1Pidx >= DMX_FVR_START_PID) && (u1Pidx < (DMX_FVR_START_PID + FVR_NUM_PID_INDEX)))
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
        LOG(3, "%s: invalid pid index %u, error:%d\n", __FUNCTION__, u1Pidx, u1Type);
        return TRUE;        // return true is due to play uP
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

    LOG(5, "uP error happen, INT = 0x%x !!!!!! \n", u4Status);

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
#ifndef CC_DMX_EMULATION
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
        u4Reg |= 0xFF00;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
        u4Reg &= 0xFFFF00FF;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
        break;
#endif  // CC_DMX_EMULATION
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
        DMX_INPUT_TYPE_T eInputType;
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

        DMX_PANIC((u1Pidx < DMX_NUM_PID_INDEX) ||
                  ((u1Pidx >= DMX_FVR_START_PID) && (u1Pidx < (DMX_FVR_START_PID + FVR_NUM_PID_INDEX))));
        DMX_PANIC_RET_VOID();

        eInputType = _DMX_GetPidInputType(u1Pidx);

        //----------------------------------------------
        // Handle record interrupt
        //----------------------------------------------
#ifdef ENABLE_MULTIMEDIA
        if(u1Type >= 64)
        {
            fgOk = FALSE;
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
            fgEnabled = FALSE;
            if(u1Pidx < DMX_NUM_PID_INDEX)
            {
                fgEnabled = _DMX_IsPidEnabled(u1Pidx);

                // PS and MM not use toggle mechanism to check PID enabled or not
                if((eInputType == DMX_IN_PLAYBACK_PS) || (eInputType == DMX_IN_PLAYBACK_MM) ||
                    (eInputType == DMX_IN_PLAYBACK_ES))
                {
                    fgEnabled = TRUE;
                }
            }
            else if((u1Pidx >= DMX_FVR_START_PID) &&
                    (u1Pidx < (DMX_FVR_START_PID + FVR_NUM_PID_INDEX)))
            {
                // FVR PID status
                u1Pidx -= DMX_FVR_START_PID;
                fgEnabled = _FVR_IsPidEnabled(u1Pidx);
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
                    if(u1Pidx < DMX_NUM_PID_INDEX)
                    {
                        prPidStruct = _DMX_GetPidStruct(u1Pidx);
                        if(prPidStruct != NULL)
                        {
                            prPidStruct->u4NextPic = 0;
                        }
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
        FVR_PID_STRUCT_T *prPid;
        UINT32 u4Status;
        UINT8 u1Pidx, u1Idx;

        u4Status = DMXCMD_READ32(DMX_REG_FTuP_ERROR_STATUS_REG);

        u1Pidx = (UINT8)((u4Status >> 16) & 0xff);
        if((u1Pidx >= DMX_FVR_START_PID) && (u1Pidx < (DMX_FVR_START_PID + FVR_NUM_PID_INDEX)))
        {
            if(u1Pidx >= DMX_FVR_START_PID)
            {
                u1Pidx -= DMX_FVR_START_PID;
            }

            prPid = _FVR_GetPidStruct(u1Pidx);
            u1Idx = prPid->u1TsIndex;
        }
        else
        {
            // Should not run this
            u1Idx = 0;
        }

        // Clear interrupt
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);

        _FVR_uPErrorInt(u1Idx);
    }
#else
    UNUSED(fgRecuPErrorHandled);
#endif  // ENABLE_MULTIMEDIA

    // PCR error and status
    if (u4Status & DMX_INT_PCR_MASK)
    {
#ifdef CC_DMX_EMULATION
        _DMX_EMU_MulMatch_ISR();
#else
#ifdef __PCR_RECOVERY__
        _DMX_IntPcr(u4Status);
#else
        _DmxPcrInt(u4Status);
#endif  // __PCR_RECOVERY__
#endif // CC_DMX_EMULATION
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


void _DMX_SetPtsInfo(BOOL fgEnableVideoPts,BOOL fgEnableAudioPts)
{
    _fgEnableVideoPtsInfo = fgEnableVideoPts;
    _fgEnableAudioPtsInfo = fgEnableAudioPts;
}

