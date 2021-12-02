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
 * $Author: weibo.li $
 * $Date: 2012/09/09 $
 * $RCSfile: dmx_if.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_if.h
 *  Interface of demux driver
 */


#ifndef DMX_IF_H
#define DMX_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_timer.h"
#include "mpv_drvif.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define CC_DMX_EMULATION

//#define TEST_MODE

//#define DMX_MEASURE_PSI_TIME

//#define DMX_CHECK_SECTION_CRC

#define CC_DMX_MULTIPLE_SECTION

// Use DTCP as general AES engine, so enable it by default

#define CC_DMX_AUDIO_PREBUF

#define CC_DMX_SUPPORT_MULTI2

//#define CC_DMX_TS_LOOPBACK

//#define CC_DMX_ERR_RECOVERY_TEST

//#define DMX_SUPPORT_CPRM

#ifdef CC_DMX_EMULATION
#undef DMX_FRAMER_SET0_EXTERNAL_SYNC
#undef DMX_FRAMER_SET1_EXTERNAL_SYNC
#define DMX_FRAMER_SET0_EXTERNAL_SYNC       (1)     //0: internal 1: external
#define DMX_FRAMER_SET1_EXTERNAL_SYNC       (1)     //0: internal 1: external
#endif //  CC_DMX_EMULATION

#ifdef CC_DMX_EMULATION
#define CA9_WORKAROUND
#endif

// TS130 playback
//#define CC_DMX_TS130

// The monitor thread is disabled under SLT.
//#ifndef __MODEL_slt__
//#define CC_DMX_ENABLE_MONTHREAD
//#endif  // __MODEL_slt__

//#define CC_DMX_FPGA_TEST
//#define CC_VUDU_SUPPORT

#define CC_DMX_CONN_TYPE_SUPPORT

#define CC_DMX_PES_AUDDESC

#define CC_DMX_HDCP
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/*
CC_DMX_PID_CASE_0: Emulation
CC_DMX_PID_CASE_1: Original usage
                   32 DTV/MM PID, 32 HW SF(section filter)
                   32 Record PID, 0 HW SF
CC_DMX_PID_CASE_2: ARIB
                   64 DTV, 64 HW SF
                   16 MM PID, 16 HW SF
                   64 Record PID, 64 one-byte SF
CC_DMX_PID_CASE_3: Multiple instance mode
                   32 DTV PID, 32 HW SF
                   16 MM PID,  16 HW SF
                   32 Record PID, 32 HW SF
*/
#define CC_DMX_PID_CASE_0

#if defined(CC_DMX_PID_CASE_0)
//====================================
#define DMX_NUM_PID_INDEX               (80)      ///< Number of PIDs
#define DMX_NUM_FILTER_INDEX            (80)      ///< Number of filters
#define DMX_NUM_SW_FILTER_INDEX         (0)
#define FVR_NUM_PID_INDEX               (64)
#define FVR_NUM_FILTER_INDEX            (0)
#define FVR_NUM_SW_FILTER_INDEX         (0)
//====================================
#elif defined(CC_DMX_PID_CASE_1)
//====================================
#define DMX_NUM_PID_INDEX               (32)      ///< Number of PIDs
#define DMX_NUM_FILTER_INDEX            (32)      ///< Number of filters
#define DMX_NUM_SW_FILTER_INDEX         (0)
#define FVR_NUM_PID_INDEX               (32)
#define FVR_NUM_FILTER_INDEX            (0)
#define FVR_NUM_SW_FILTER_INDEX         (0)
//====================================
#elif defined(CC_DMX_PID_CASE_2)
//====================================
#define DMX_NUM_PID_INDEX               (64 + 16)   // 64DTV, 16MM
#define DMX_NUM_FILTER_INDEX            (64 + 16)   // 64DTV, 16MM
#define DMX_NUM_SW_FILTER_INDEX         (0 + 0)
#define FVR_NUM_PID_INDEX               (64)
#define FVR_NUM_FILTER_INDEX            (0)
#define FVR_NUM_SW_FILTER_INDEX         (64)
//====================================
#elif defined(CC_DMX_PID_CASE_3)
//====================================
//====================================
#else
#error "PID usage case error!!"
#endif


#define DMX_NUM_HW_PID_COUNT            (80)
#define DMX_NUM_HW_FILTER_COUNT         (80)
#define DMX_NUM_SW_FILTER_COUNT         (80)
#define DMX_NUM_FVR_HW_PID_COUNT        (64)
#define DMX_NUM_FVR_HW_FILTER_COUNT     (64)

#define DMX_PLAY_START_PID              (0)
#define DMX_FVR_START_PID               (128)

/// Demux resource limitations
#define DMX_FRAMER_COUNT                4

#define DMX_MIN_TUNER_INDEX             0       ///< Minimum tuner index
#define DMX_MAX_TUNER_INDEX             4       ///< Maximum tuner index
                                                ///< framer0, framer1, DDI, PVR
#define DMX_MAX_CA_MODE                 8       ///< Maximum CA mode
#define DMX_MIN_FILTER_OFFSET           0       ///< Minimum filter offset
#define DMX_MAX_FILTER_OFFSET           15      ///< Maximum filter offset
#define DMX_NUM_FILTER_BANK             2       ///< Number of filter bank
#define DMX_NUM_FILTER_MODE             4       ///< Number of filter mode

#define DMX_DMEM_CA_KEY_NUM             (12)

#if defined(CC_MT5396)
#define DMX_NUM_STC_NUM                 (2)
#else
#define DMX_NUM_STC_NUM                 (1)
#endif

/// Function bitmap of PID operations
#define DMX_PID_FLAG_VALID              0x00000001  ///< Enabled or not
#define DMX_PID_FLAG_PID                0x00000002  ///< PID value
#define DMX_PID_FLAG_BUFFER             0x00000004  ///< Buffer control
#define DMX_PID_FLAG_CALLBACK           0x00000008  ///< Callback handler
#define DMX_PID_FLAG_SCRAMBLE_STATE     0x00000010  ///< Scrambling state
#define DMX_PID_FLAG_TS_INDEX           0x00000020  ///< TS index
#define DMX_PID_FLAG_PCR                0x00000040  ///< PCR mode
#define DMX_PID_FLAG_STEER              0x00000080  ///< Steering
#define DMX_PID_FLAG_DESC_MODE          0x00000100  ///< Descrambling mode, not use now
#define DMX_PID_FLAG_DEVICE_ID          0x00000200  ///< Device (decoder) ID
#define DMX_PID_FLAG_KEY_INDEX          0x00000400  ///< Key index
#define DMX_PID_FLAG_DATA_POINTERS      0x00000800  ///< Data pointers
#define DMX_PID_FLAG_PRIMARY            0x00001000  ///< Primary PID
#define DMX_PID_FLAG_STREAM_ID          0x00002000  ///< Stream ID
#define DMX_PID_FLAG_SUBSTREAM_ID       0x00004000  ///< Substream ID (SID)
#define DMX_PID_FLAG_INSTANCE_TAG       0x00008000  ///< for multiple instances
#define DMX_PID_FLAG_INPUTTYPE          0x00010000  ///< Input type (DTV, PS, MM..)
#define DMX_PID_FLAG_DISABLE_OUTPUT     0x00020000  ///< Disable FIFO output
#define DMX_PID_FLAG_FIFO_FULL          0x00040000  ///<FIFO FULL
#define DMX_PID_FLAG_NONE               0             ///< None
#define DMX_PID_FLAG_ALL                (0xffffffff & ~DMX_PID_FLAG_SCRAMBLE_STATE)

/// Function bitmap of filter operations
#define DMX_FILTER_FLAG_VALID           0x00000001  ///< Enabled or not
#define DMX_FILTER_FLAG_PIDX            0x00000002  ///< PID index
#define DMX_FILTER_FLAG_OFFSET          0x00000004  ///< Offset
#define DMX_FILTER_FLAG_CRC             0x00000008  ///< Check CRC or not
#define DMX_FILTER_FLAG_PATTERN         0x00000010  ///< Pattern and mask
#define DMX_FILTER_FLAG_MODE            0x00000020  ///< Filter mode
#define DMX_FILTER_FLAG_NONE            0           ///< None
#define DMX_FILTER_FLAG_ALL             0xffffffff  ///< All

/// Steering bitmap
#define DMX_STEER_TO_1394               0x01        ///< Steering to 1394
#define DMX_STEER_TO_PVR                0x02        ///< Steering to PVR
#define DMX_STEER_TO_FTUP               0x04        ///< Steering to uP

/// Alignments
#define DMX_CA_BUFFER_ALIGNMENT         32          ///< Alignment of CA buffers

// MPEG Picture types
#define PIC_TYPE_I                      0x01        ///< I picture
#define PIC_TYPE_P                      0x02        ///< P picture
#define PIC_TYPE_B                      0x03        ///< B picture
#define PIC_TYPE_SEQ_START              0x04        ///< Sequence start
#define PIC_TYPE_GOP                    0x05        ///< GOP
#define PIC_TYPE_SEQ_END                0x06        ///< Sequence end
#define PIC_TYPE_UNKNOWN                0xff        ///< Unknown picture type
#define MPV_PIC_VALID_TYPE(x)          (((x)>=PIC_TYPE_I)&&((x)<=PIC_TYPE_SEQ_END))

// H264 Picture types
#define H264_PIC_TYPE_NONIDR            0x11
#define H264_PIC_TYPE_IDR               0x12
#define H264_PIC_TYPE_SEI               0x13
#define H264_PIC_TYPE_SEQ               0x14
#define H264_PIC_TYPE_PIC               0x15
#define H264_PIC_TYPE_AU                0x16
#define H264_PIC_TYPE_PREFIX            0x17
#define H264_PIC_TYPE_SUB_SPS           0x18
#define H264_PIC_TYPE_CODED_SLICE_EXT   0x19
#define H264_PIC_TYPE_UNKNOWN           0xff        ///< Unknown picture type
#define H264_PIC_VALID_TYPE(x)          (((x)>=H264_PIC_TYPE_NONIDR)&&((x)<=H264_PIC_TYPE_AU))

// VC1 Picture types
#define VC1_PIC_TYPE_I                  0x21
#define VC1_PIC_TYPE_P                  0x22
#define VC1_PIC_TYPE_B                  0x23
#define VC1_PIC_TYPE_BI                 0x24
#define VC1_PIC_TYPE_SKIP               0x25
#define VC1_PIC_TYPE_ENTRY              0x2E
#define VC1_PIC_TYPE_SEQ_START          0x2F
#define VC1_PIC_TYPE_UNKNOWN            0xFF
#define VC1_PIC_VALID_TYPE(x)          (((x)>=VC1_PIC_TYPE_I)&&((x)<=VC1_PIC_TYPE_SEQ_START))

// MPEG4 Picture types
#define MPEG4_PIC_TYPE_PIC              0x31
#define MPEG4_PIC_TYPE_OTHERS           0x32
#define MPEG4_PIC_TYPE_UNKNOWN          0xff        ///< Unknown picture type
#define MPEG4_PIC_VALID_TYPE(x)          (((x)>=MPEG4_PIC_TYPE_PIC)&&((x)<=MPEG4_PIC_TYPE_OTHERS))

// AVS Picture types
#define AVS_PIC_TYPE_SEQ_START          0x41
#define AVS_PIC_TYPE_SEQ_END            0x42
#define AVS_PIC_TYPE_USR_DATA           0x43
#define AVS_PIC_TYPE_PIC_I              0x44
#define AVS_PIC_TYPE_EXT                0x45
#define AVS_PIC_TYPE_PIC_PB             0x46
#define AVS_PIC_TYPE_VID_EDIT_CODE      0x47
#define AVS_PIC_TYPE_UNKNOWN            0xff        ///< Unknown picture type
#define AVS_PIC_VALID_TYPE(x)          (((x)>=AVS_PIC_TYPE_SEQ_START)&&((x)<=AVS_PIC_TYPE_VID_EDIT_CODE))


// Misc
#define DMX_MAX_SERIAL_NUM              7//15

#define DMX_STARTCODE_NONINTR_NUM       5
#define DMX_STARTCODE_INTR_NUM          5

// Multiple instance
#define DMX_ALL_INSTINDEX               (0xF0)
#define DMX_NULL_INSTINDEX              (0xFF)

//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---
EXTERN BOOL _fgAwEraserCheckBegin;

#define DMX_AW_ERR_OK                   0
#define DMX_AW_ERR_ADDRESS_RANGE        -3301
#define DMX_AW_ERR_SECTION_LEN          -3302
#define DMX_AW_ERR_NO_MEM               -3303
#define DMX_AW_ERR_COPY_BUFFER          -3304
#define DMX_AW_ERR_INSUFFICIENT_LEN     -3305
#define DMX_AW_ERR_NOT_ZEROED_OUT       -3306
#define DMX_AW_ERR_TIMEOUT              -3307

#define DMX_AW_MAX_PSI_LEN              4096
#define DMX_AW_VCT_HEADER               10
#define DMX_AW_VCT_SUBHEADER            32
// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------

#define DMX_PVRPLAY_TS_INDEX            2
#define DMX_DBM_TESTPID_IDX             31

//
// Clock
//
#define DMX_CLOCK                               (324)   //MHz
#define DMX_PVRPLAY_TIMESTAMP_DIV_BASE          (DMX_CLOCK / 27)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define DMX_IN_BROADCAST_MASK       0xF0
#define DMX_IN_PLAYBACK_MASK        0xF00

typedef enum
{
    DMX_IN_NONE         = 0,
    DMX_IN_DIAG         = 1,
    DMX_IN_BROADCAST_TS = 0x10,
    DMX_IN_PLAYBACK_TS  = 0x100,
    DMX_IN_PLAYBACK_PS  = 0x200,
    DMX_IN_PLAYBACK_MM  = 0x300,
    DMX_IN_PVR_TS       = 0x400,
    DMX_IN_PLAYBACK_ES  = 0x500
} DMX_INPUT_TYPE_T;

typedef enum
{
    DMX_SCRAMBLE_TSFLAG_ONLY,         // check flag only
    DMX_SCRAMBLE_PESSTART_ONLY,          // check PES start code only
    DMX_SCRAMBLE_BOTH_TSFLAG_PESSTART
} DMX_SCRAMBLE_TYPE_T;

typedef struct
{
    DMX_SCRAMBLE_TYPE_T eType;
    UINT8 u1Flag;
} DMX_SCRAMBLE_CONFIG_T;

/// pin set
typedef enum
{
#if defined(CC_MT5396)
    DMX_PINSET_INTERNAL = 0x0,
    DMX_PINSET_EXT1 = 0x10,
    DMX_PINSET_EXT2_0 = 0x20,
    DMX_PINSET_EXT2_1 = 0x30,
    DMX_PINSET_EXT3 = 0x40,
#elif defined(CC_MT5368)
    DMX_PINSET_INTERNAL = 0x0,
    DMX_PINSET_EXT1 = 0x10,
    DMX_PINSET_EXT1_1 = 0x20,
    DMX_PINSET_EXT1_2 = 0x30,
    DMX_PINSET_EXT2 = 0x40,
#elif defined(CC_MT5389)
    DMX_PINSET_INTERNAL = 0x0,
    DMX_PINSET_EXT1 = 0x10,
#endif
} DMX_PINSET_T;

typedef enum
{
    DMX_EDGE_POSITIVE,
    DMX_EDGE_NEGATIVE,
} DMX_EDGE_T;

typedef enum
{
    DMX_SYNC_INTERNAL,
    DMX_SYNC_EXTERNAL,
} DMX_SYNC_T;

typedef enum
{
    DMX_FRAMER_SERIAL = 0,
    DMX_FRAMER_PARALLEL = 1,
    DMX_FRAMER_TWOBIT = 2,
    DMX_FRAMER_UNKNOWN = 3
} DMX_FRAMER_MODE_T;

/// Front-end config struct
typedef struct
{
    BOOL fgEnable;
    DMX_PINSET_T ePinSet;
    DMX_FRAMER_MODE_T eMode;
    DMX_EDGE_T eEdge;
    DMX_SYNC_T eSync;
} DMX_FRONTEND_CONFIG_T;

// Rule
//   above 0x10 is external TS
//   when above 0x10, 0=serial, 1=parallel, 2:2bit
//   when above 0x10, 1x=ext, 2x=ext2_0, 3x=ext2_1, 4x=ext3, 5x=old playbard
typedef enum                            // Set by TS index
{
#if defined(CC_MT5396)
    DMX_FE_INTERNAL = 0,
    //DMX_FE_EXTERNAL = 1,
    DMX_FE_DDI = 2,
    DMX_FE_TSOUT = 3,
    DMX_FE_CI = 4,
    DMX_FE_EXT_S = 0x10,
    DMX_FE_EXT_P = 0x11,
    DMX_FE_EXT_2BIT = 0x12,
    DMX_FE_EXT2_0_S = 0x20,
    DMX_FE_EXT2_0_P = 0x21,
    DMX_FE_EXT2_0_2BIT = 0x22,
    DMX_FE_EXT2_1_S = 0x30,
    DMX_FE_EXT2_1_P = 0x31,       // Ext2_1 no parallel, test
    DMX_FE_EXT2_1_2BIT = 0x32,
    DMX_FE_EXT3_S = 0x40,
    DMX_FE_EXT3_P = 0x41,
    DMX_FE_EXT3_2BIT = 0x42,
    DMX_FE_EXT_LEGACY_P = 0x51,
#elif defined(CC_MT5368)
    DMX_FE_INTERNAL = 0,
    //DMX_FE_EXTERNAL = 1,
    DMX_FE_DDI = 2,
    DMX_FE_TSOUT = 3,
    DMX_FE_EXT_S = 0x10,
    DMX_FE_EXT_P = 0x11,
    DMX_FE_EXT_2BIT = 0x12,
    DMX_FE_EXT_1_S = 0x20,
    DMX_FE_EXT_1_P = 0x21,
    DMX_FE_EXT_1_2BIT = 0x22,
    DMX_FE_EXT_2_S = 0x30,
    DMX_FE_EXT_2_P = 0x31,
    DMX_FE_EXT_2_2BIT = 0x32,
    DMX_FE_EXT2_0_S = 0x40,
    DMX_FE_EXT2_0_P = 0x41,
    DMX_FE_EXT2_0_2BIT = 0x42,
#elif defined(CC_MT5389)
    DMX_FE_INTERNAL = 0,
    //DMX_FE_EXTERNAL = 1,
    DMX_FE_DDI = 2,
    DMX_FE_TSOUT = 3,
    DMX_FE_EXT_S = 0x10,
    DMX_FE_EXT_P = 0x11,
    DMX_FE_EXT_2BIT = 0x12,
    DMX_FE_EXT_1_S = 0x20,
    DMX_FE_EXT_1_P = 0x21,
    DMX_FE_EXT_1_2BIT = 0x22,
    DMX_FE_EXT_2_S = 0x30,
    DMX_FE_EXT_2_P = 0x31,
    DMX_FE_EXT_2_2BIT = 0x32,
    DMX_FE_EXT2_0_S = 0x40,
    DMX_FE_EXT2_0_P = 0x41,
    DMX_FE_EXT2_0_2BIT = 0x42,
#endif
    DMX_FE_NO_TSVALID = 0x60,
    // following avoid compile error
    DMX_FE_EXT_SERIAL0 = 0xF0,
    DMX_FE_EXT_SERIAL1 = 0xF1,
    DMX_FE_EXT_PARALLEL = 0xF2,
    DMX_FE_EXT_TWOBIT0 = 0xF3,
    DMX_FE_EXT_TWOBIT1 = 0xF4,
    DMX_FE_DVB_DEMOD = 0xF5,
    DMX_FE_ATSC_DEMOD = 0xF6,
    DMX_FE_NULL = 0xFF
} DMX_FRONTEND_T;

typedef enum
{
    DMX_SOURCE_DTV0 = 0,
    DMX_SOURCE_DTV1,
    DMX_SOURCE_DTV2,
    DMX_SOURCE_TSFILE0,         // TS file
    DMX_SOURCE_TSFILE1,         // TS file
    DMX_SOURCE_DRAM0,           // AVI, MKV, ES...
    DMX_SOURCE_NUM
} DMX_SOURCE_T;

typedef enum
{
    DMX_CONN_TYPE_TUNER_0 = 0,
    DMX_CONN_TYPE_TUNER_1,
    DMX_CONN_TYPE_TUNER_2,
    DMX_CONN_TYPE_TUNER_MAX,
    DMX_CONN_TYPE_BUFAGENT_0,
    DMX_CONN_TYPE_BUFAGENT_1,
    DMX_CONN_TYPE_BUFAGENT_2,
    DMX_CONN_TYPE_BUFAGENT_3,
    DMX_CONN_TYPE_BUFAGENT_4,
    DMX_CONN_TYPE_BUFAGENT_MAX,
    DMX_CONN_TYPE_NONE,
} DMX_CONN_TYPE_T;

typedef enum
{
    DMX_EXT_FRAMER_NORMAL = 0,
    DMX_EXT_FRAMER_2_PVR_3_EXT,
    DMX_EXT_FRAMER_2_DDI_3_EXT
} DMX_EXT_FRAMER_T;

typedef enum
{
    DMX_TSFMT_188,
    DMX_TSFMT_192,
    DMX_TSFMT_192_ENCRYPT,
    DMX_TSFMT_TIMESHIFT,
    DMX_TSFMT_130,
    DMX_TSFMT_NONE
} DMX_TSFMT_T;

/// PMT data structure for PID swap
typedef struct
{
    UINT16 u2Pid;               ///< PID of this PMT
    UINT16 u2ProgramNumber;     ///< Program number
    UINT16 u2PcrPid;            ///< PCR PID
    UINT16 u2VideoPid;          ///< Video PID
    UINT16 u2AudioPid;          ///< Audio PID
    UINT8 u1AudioType;          ///< Type of audio stream
} DMX_PMT_T;

/// PID types
typedef enum
{
    DMX_PID_TYPE_NONE = 0,      ///< None
    DMX_PID_TYPE_PSI,           ///< PSI
    DMX_PID_TYPE_PES,           ///< PES
    DMX_PID_TYPE_PES_TIME,      ///< PES with time information
    DMX_PID_TYPE_ES_VIDEO,      ///< Video ES
    DMX_PID_TYPE_ES_AUDIO,      ///< Audio ES
    DMX_PID_TYPE_ES_VIDEOCLIP,  ///< Video clip ES
    DMX_PID_TYPE_ES_OTHER,       ///< Other ES
    DMX_PID_TYPE_ES_DTCP,
    DMX_PID_TYPE_TS_RAW
} DMX_PID_TYPE_T;

/// PES message type
typedef enum
{
    DMX_PES_MSG_TYPE_PES,               ///< PES message
    DMX_PES_MSG_TYPE_PURGE,             ///< Purge message
    DMX_PES_MSG_TYPE_PURGE_REENABLE,    ///< Purge and re-enable message
    DMX_PES_MSG_TYPE_ERR_HANDLE
} DMX_PES_MSG_TYPE_T;

typedef enum
{
    DMX_VIDEO_MPEG = 0,
    DMX_VIDEO_H264,
    DMX_VIDEO_AVS,
    DMX_VIDEO_VC1,
    DMX_VIDEO_MPEG4,
    DMX_VIDEO_WMV7,
    DMX_VIDEO_WMV8,
    DMX_VIDEO_WMV9,
    DMX_VIDEO_MP4_IN_WMV,
    DMX_VIDEO_RV,
    DMX_VIDEO_VP6,
    DMX_VIDEO_VP8,
    DMX_VIDEO_NONE
} DMX_VIDEO_TYPE_T;

// Specify the audio format in the bit stream.
// This enumeration is only used in PS file playback.
typedef enum
{
    DMX_AUDIO_UNKNOWN,
    DMX_AUDIO_MPEG,
    DMX_AUDIO_LPCM,
    DMX_AUDIO_AC3,
    DMX_AUDIO_DTS
} DMX_AUDIO_TYPE_T;

typedef struct
{
    UINT8 u1TsIdx;  // Ts index combine with ext ckgen
    DMX_FRONTEND_T eFrontEnd;
    DMX_TSFMT_T eTSFmt;
    DMX_VIDEO_TYPE_T eVideoType;
    UINT8 u1Instance;   // Multiple instance
} DMX_SOURCE_INFO_T;

// Specify whether the input data format is VCD (.DAT) format or not.
// This enumeration is only used in PS file playback.
typedef enum
{
    DMX_DATA_FORMAT_UNKNOWN,
    DMX_DATA_FORMAT_RAW,
    DMX_DATA_FORMAT_DAT
} DMX_DATA_FORMAT_T;

/// PES message
typedef struct
{
    DMX_PES_MSG_TYPE_T eMsgType;        ///< Message type
    DMX_PID_TYPE_T ePidType;            ///< PID type
    UINT32 u4BufStart;                  ///< Buffer start address
    UINT32 u4BufEnd;                    ///< Buffer end address
    UINT32 u4FrameAddr;                 ///< Frame (picture) address
    UINT32 u4FrameType;                 ///< Frame (picture) type
    UINT32 u4Wp;                        ///< Write pointer
    UINT32 u4Pts;                       ///< PTS
    UINT32 u4Dts;                       ///< DTS
    UINT8 u1Pidx;                       ///< PID index
    UINT8 u1Channel;                    ///< Video channel
    UINT8 u1DeviceId;                   ///< Device (decoder) ID
    UINT8 u1SerialNumber;               ///< Serial number
    BOOL fgPtsDts;                      ///< Is PTS/DTS field valid
    BOOL fgSeqHeader;                   ///< Is this frame is sequence header
    BOOL fgGop;                         ///< Is this frame is GOP

    BOOL fgEOS;                         ///< File playback: End of stream
    DMX_VIDEO_TYPE_T eVideoType;        ///< Video type of the input data (used by VC1 and WMV)
    UINT8 u1SliceCount;                ///< slice count in one picture (H.264)
    void *pvInstanceTag;                ///< Tag for supporting multiple instances (used only by v-dec)

#ifdef CC_DMX_PES_EXT_BUFFER
    // The value 0 in u4ExtBufCurrentRp/u4ExtBufCurrentWp means no
    // "extension 2 data" in the current PES header.
    UINT32 u4ExtBufStart;
    UINT32 u4ExtBufEnd;
    UINT32 u4ExtBufCurrentRp;
    UINT32 u4ExtBufCurrentWp;
#endif  // CC_DMX_PES_EXT_BUFFER
} DMX_PES_MSG_T;

/// PSI message
typedef struct
{
    UINT8 u1Pidx;
    UINT8 u1SecCount;
    UINT32 u4EndAddr;
    UINT8 u1SN;                         ///< Serial Number
    BOOL fgFvr;
} DMX_PSI_MSG_T;

/// Audio PES structure
typedef struct
{
    UINT32 u4Wp;                        ///< Write pointer
    UINT32 u4PesRp;                     ///< Previous write pointer
    UINT32 u4Pts;                       ///< PTS
    UINT32 u4Dts;                       ///< DTS
    UINT8 u1PidIndex;                   ///< PID index
    UINT8 u1DeviceId;                   ///< Device (audio decoder) ID
    BOOL fgEOS;                         ///< File playback: End of stream
    UINT32 u4Info;                      ///< Audio info (for LPCM and AC3)
    BOOL fgCopyright;                   ///< The copyright flag in PES header
    BOOL fgOriginal;                    ///< The original_or_copy flag
    void *pvInstanceTag;                ///< Tag for supporting multiple instances (used only by v-dec)
    BOOL fgForward;                     // is the audio es file in trick mode FF
	BOOL fgIgnoreData;                  ///<is for ignoring audio data (used only with EOS)
#ifdef TIME_SHIFT_SUPPORT
    UINT32 u4TickNum;
#endif

#ifdef CC_DMX_PES_AUDDESC
    BOOL fgContainAD;
    UINT8 u1ADFad;
    UINT8 u1ADPan;
#endif
} DMX_AUDIO_PES_T;

///
/// Prototypes of decoder callback functions
///

/// Callback handler of sending video PES. This handler will be called
/// whenever demux driver detects a picture start code in video ES FIFO.
typedef BOOL (*PFN_DMX_SEND_VIDEO_PES)(const DMX_PES_MSG_T* prPes);

/// Callback handler of sending audio PES. This handler will be called
/// whenever demux driver receives an audio frame.
typedef BOOL (*PFN_DMX_SEND_AUDIO_PES)(const DMX_AUDIO_PES_T* prPes);

/// Callback handler updating FIFO write pointer. This handler will be called
/// whenever demux driver updates write pointer of video ES FIFO.
typedef void (*PFN_DMX_UPDATE_WP)(UINT8 u1PidIndex, UINT32 u4Wp);

/// Decoder callback handlers
typedef struct
{
    PFN_DMX_SEND_VIDEO_PES pfnSendVideoPes;     ///< Send video PES
    PFN_DMX_UPDATE_WP pfnUpdateVideoWp;         ///< Update video WP
    PFN_DMX_SEND_AUDIO_PES pfnSendAudioPes;     ///< Send audio PES
} DMX_DECODER_CALLBACKS_T;

/// PCR mode
typedef enum
{
    DMX_PCR_MODE_NONE = 0,              ///< None
    DMX_PCR_MODE_OLD,                   ///< Old mode (5371 type)
    DMX_PCR_MODE_NEW                    ///< New mode
} DMX_PCR_MODE_T;

/// Descrambling schemes
/// Keep this just for backward compatible, not use after 5368
typedef enum
{
    DMX_DESC_MODE_NONE = 0,             ///< None
    DMX_DESC_MODE_DES_ECB,              ///< DES ECB
    DMX_DESC_MODE_DES_CBC,              ///< DES CBC
    DMX_DESC_MODE_3DES_ECB,             ///< 3DES ECB
    DMX_DESC_MODE_3DES_CBC,             ///< 3DES CBC
    DMX_DESC_MODE_DVB,                  ///< DVB
    DMX_DESC_MODE_DVB_CONF,             ///< DVB conformance
    DMX_DESC_MODE_MULTI2_BIG,           ///< Multi-2 big-endian
    DMX_DESC_MODE_MULTI2_LITTLE,        ///< Multi-2 little-endian
    DMX_DESC_MODE_AES_ECB,              ///< AES ECB
    DMX_DESC_MODE_AES_CBC,              ///< AES CBC
} DMX_DESC_MODE_T;

/// Descrambling residual termination block(RTB)
typedef enum
{
    DMX_DESC_RTB_MODE_CLEAR = 0,        ///< leave clear
    DMX_DESC_RTB_MODE_CTS,              ///< CTS
    DMX_DESC_RTB_MODE_SCTE52,           ///< SCTE-52
    DMX_DESC_RTB_MODE_MAX,              ///< END
} DMX_DESC_RTB_MODE_T;

/// Descrambling schemes
typedef enum
{
    DMX_CA_MODE_NONE = 0x80,         ///< None, start fomr 0x80 is due to set difference to DMX_DESC_MODE_T
    DMX_CA_MODE_DES,
    DMX_CA_MODE_3DES,
    DMX_CA_MODE_DVB,                  ///< DVB
    DMX_CA_MODE_DVB_CONF,             ///< DVB conformance
    DMX_CA_MODE_MULTI2_BIG,           ///< Multi-2 big-endian
    DMX_CA_MODE_MULTI2_LITTLE,        ///< Multi-2 little-endian
    DMX_CA_MODE_AES,
    DMX_CA_MODE_SMS4
} DMX_CA_MODE_T;

typedef enum
{
    DMX_CA_FB_ECB = 0,                //// No feedback
    DMX_CA_FB_CBC,
    DMX_CA_FB_CFB,
    DMX_CA_FB_OFB,
    DMX_CA_FB_CTR                     ///< Counter mode
} DMX_CA_FEEDBACK_MODE_T;

/// Notify info of PSI
typedef struct
{
    UINT32 u4SecAddr;                   ///< Section address
    UINT32 u4SecLen;                    ///< Section length
    UINT8 u1SerialNumber;               ///< Serial number
    UINT32 u4MatchBitmap;
    void *pvInstanceTag;            ///< Tag for supporting multiple instances (used only by v-dec)
} DMX_NOTIFY_INFO_PSI_T;

/// Notify info of ES data
typedef struct
{
    UINT32 u4Pts;                       ///< PTS
    UINT32 u4Dts;                       ///< DTS
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
} DMX_NOTIFY_INFO_ES_T;

/// Notify info of PES data
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
    UINT8 u1SerialNumber;               ///< Serial number    void *pvInstanceTag;            ///< Tag for supporting multiple instances (used only by v-dec)
    void *pvInstanceTag;            ///< Tag for supporting multiple instances (used only by v-dec)
} DMX_NOTIFY_INFO_PES_T;

/// Notify info of PES data
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
    UINT32 u4Pts;
    UINT32 u4Dts;
    UINT8 u1SerialNumber;               ///< Serial number
} DMX_NOTIFY_INFO_PES_TIME_T;

/// Notification structure for notification code DMX_NOTIFY_CODE_STREAM_ID
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
} DMX_NOTIFY_INFO_STREAM_ID_T;

/// Notify info of raw TS data
typedef struct
{
    UINT8 u1SerialNumber;
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
} DMX_NOTIFY_INFO_RAW_TS_T;

/// Scrambling state
typedef enum
{
    DMX_SCRAMBLE_STATE_CLEAR,           ///< Clear data
    DMX_SCRAMBLE_STATE_SCRAMBLED,       ///< Scrambled data
    DMX_SCRAMBLE_STATE_UNKNOWN          ///< Unknown data
} DMX_SCRAMBLE_STATE_T;

/// Notification code
typedef enum
{
    DMX_NOTIFY_CODE_PSI,                ///< PSI notification
    DMX_NOTIFY_CODE_ES,                 ///< ES notification
    DMX_NOTIFY_CODE_PES,                ///< PES notification
    DMX_NOTIFY_CODE_PES_TIME,           ///< PES notification with time information
    DMX_NOTIFY_CODE_SCRAMBLE_STATE,     ///< Scramble state change notification
    DMX_NOTIFY_CODE_OVERFLOW,           ///< Overflow notification
    DMX_NOTIFY_CODE_STREAM_ID,          ///< Report pre-parsed Stream IDs
    DMX_NOTIFY_CODE_RAW_TS              ///< Rwa TS notification
} DMX_NOTIFY_CODE_T;

/// Demux notification callback function
typedef BOOL (*PFN_DMX_NOTIFY)(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag);

typedef BOOL (*PFN_DMX_SORUCE_STATE_CB_FOR_VDEC)(UCHAR ucEsId, DMX_SCRAMBLE_STATE_T eState);

/// PID data structure
typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgAllocateBuffer;          ///< Allocate buffer
    BOOL fgPrimary;                 ///< Primary PID
    BOOL fgFifoFull;				///<Pid buffer fifo full
    UINT8 u1TsIndex;                ///< TS index
    UINT8 u1DeviceId;               ///< Device ID
    UINT8 u1KeyIndex;               ///< Descramble key index
    UINT8 u1SteerMode;              ///< Steering mode
    UINT16 u2Pid;                   ///< PID
    UINT32 u4BufAddr;               ///< Buffer address
    UINT32 u4BufSize;               ///< Buffer size
    UINT32 u4Rp;                    ///< Read pointer
    UINT32 u4Wp;                    ///< Write pointer
    UINT32 u4PeakBufFull;           ///< Peak buffer fullness, to estimate
                                    ///< required ES FIFO size
    DMX_PCR_MODE_T ePcrMode;        ///< PCR mode
    DMX_PID_TYPE_T ePidType;        ///< PID type
    DMX_DESC_MODE_T eDescMode;      ///< Descramble mode
    PFN_DMX_NOTIFY pfnNotify;       ///< Callback function
    void* pvNotifyTag;              ///< Tag value of callback function
    PFN_DMX_NOTIFY pfnScramble;     ///< Callback function of scramble state
    void* pvScrambleTag;            ///< Tag value of scramble callback function
    void *pvInstanceTag;            ///< Tag for supporting multiple instances (used only by v-dec)
    UINT8 u1ChannelId;
    DMX_INPUT_TYPE_T eInputType;
    DMX_SOURCE_T eSource;

    BOOL fgDisableFifoOutput;
} DMX_PID_T;

/// Memory usage of each PID
typedef struct
{
    BOOL fgValid;                   ///< Is this struct valid or not?
    UINT32 u4PeakUsage;             ///< Peak memory usage per PID (preserved between "free")
    UINT32 u4CurrentUsage;          ///< Current memory usage per PID
} DMX_PID_MEM_USAGE_T;

/// Memory usage tracker
typedef struct
{
    DMX_PID_MEM_USAGE_T arPidMemUsage[DMX_NUM_PID_INDEX];
    UINT32 u4GlobalPeak;            ///< Global peak memory usage (preserved between "free")
    UINT32 u4GlobalCurrent;         ///< Global current memory usage
} DMX_MEM_TRACKER_T;


/// Filter mode
typedef enum
{
    DMX_FILTER_MODE_SHORT = 0,      ///< Short mode
    DMX_FILTER_MODE_14_2 = 1,       ///< 14 + 2 mode
    DMX_FILTER_MODE_LONG = 2,       ///< Long mode
    DMX_FILTER_MODE_POS_NEG = 3,    ///< Positive + negative mode
    DMX_FILTER_MODE_GENERIC = 4     ///< Generic mode
} DMX_FILTER_MODE_T;

typedef enum
{
    DMX_GENFILTER_8_BYTE,
    DMX_GENFILTER_16_BYTE
} DMX_GENFILTER_MODE_T;

/// Filter structure
typedef struct
{
    BOOL fgIsFvr;
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
    UINT8 u1Pidx;                   ///< PID index
    UINT8 u1Offset;                 ///< Offset
    UINT32 au4Data[2];              ///< Pattern
    UINT32 au4Mask[2];              ///< Mask
    DMX_FILTER_MODE_T eMode;        ///< Mode
} DMX_FILTER_T;

typedef struct
{
    BOOL fgIsFvr;
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
    UINT8 u1Pidx;                   ///< PID index, 0 ~ 31
    UINT8 u1Offset;                 ///< Offset
    UINT8 au1Data[16];              ///< Pattern
    UINT8 au1Mask[16];              ///< Mask
    UINT8 au1PosNeg[16];            ///< Positive or negative
    DMX_GENFILTER_MODE_T eMode;
} DMX_FILTER_GENERIC_T;

/// PID statistics
typedef struct
{
    UINT32 u4PesCount;              ///< PES counter
    UINT32 u4PicCount;              ///< Picture counter
    UINT32 u4SecCount;              ///< Section counter
    UINT32 u4TotalErrors;           ///< Total errors
    UINT32 u4DbmErrors;             ///< DBM errors
    UINT32 u4DescErrors;            ///< Descrambler errors
    UINT32 u4PcrErrors;             ///< PCR errors
    UINT32 u4SteerErrors;           ///< Steering errors
    UINT32 u4FTuPErrors;            ///< uP errors
    UINT32 u4MemoryErrors;          ///< Memory errors
    UINT32 u4PvrErrors;             ///< PVR errors
} DMX_PID_COUNTERS_T;

/// Notification code of capture buffer
typedef enum
{
    DMX_CAPTURE_NOTIFY_CODE_RECEIVE_DATA,   ///< Receive data
    DMX_CAPTURE_NOTIFY_CODE_BUFFER_FULL     ///< Buffer overflow
} DMX_CAPTURE_NOTIFY_CODE_T;

/// Capture info
typedef struct
{
    UINT32 u4BufStart;              ///< Buffer start address
    UINT32 u4BufEnd;                ///< Buffer end address
    UINT32 u4Rp;                    ///< Read pointer
    UINT32 u4Wp;                    ///< Write pointer
    UINT32 u4DataSize;              ///< Data size
} DMX_CAPTURE_INFO_T;

/// Callback handler of capture buffer
typedef BOOL (*PFN_DMX_CAPTURE_HANDLER)(DMX_CAPTURE_NOTIFY_CODE_T eCode,
    const DMX_CAPTURE_INFO_T* prInfo, void* pvTag);

typedef enum
{
    DMX_CAPTURE_FRAMER0,
    DMX_CAPTURE_DDI,
    DMX_CAPTURE_PVR,
    DMX_CAPTURE_TS_IDX3
} DMX_CAPTURE_DEV_T;


#define DMX_CAPTURE_BYPASS_NONE             0x00000000
#define DMX_CAPTURE_BYPASS_FRAMER0          0x04000000
#define DMX_CAPTURE_BYPASS_FRAMER1          0x08000000
#define DMX_CAPTURE_BYPASS_PVR              0x10000000
#define DMX_CAPTURE_BYPASS_DDI              0x20000000

/// Capture settings
typedef struct
{
    UINT32 u4BytePassDev;
    UINT32 u4BufSize;                           ///< Buffer size
    UINT32 u4Threshold;                         ///< Interrupt threshold
    PFN_DMX_CAPTURE_HANDLER pfnCaptureHandler;  ///< Capture handler
    void* pvCaptureTag;                         ///< Tag of capture handler
} DMX_CAPTURE_T;

typedef struct
{
    UINT32 u4BytePassDev;
    UINT32 u4BufStart;
    UINT32 u4BufSize;                           ///< Buffer size
    UINT32 u4Threshold;                         ///< Interrupt threshold
    PFN_DMX_CAPTURE_HANDLER pfnCaptureHandler;  ///< Capture handler
    void* pvCaptureTag;                         ///< Tag of capture handler
} DMX_CAPTURE2_T;

typedef enum
{
    DMX_LOCAL_ARBITOR_DMX = 0,
    DMX_LOCAL_ARBITOR_DDI,
    DMX_LOCAL_ARBITOR_PARSER,
    DMX_LOCAL_ARBITOR_NUM
} DMX_LOCAL_ARBITOR_DEVICE_T;

typedef enum
{
    DMX_ERR_ASSERT = 0,
    DMX_ERR_RESET,
    DMX_ERR_BYPASS
} DMX_ERR_LEVEL_T;

//#ifdef CC_VUDU_SUPPORT
typedef struct
{
    DMX_DESC_MODE_T eMode;
    UINT8 u1Index;
    BOOL fgEven;
    UINT8 *pu1UniqueKey;
    BOOL fgWrappedKey;
    UINT8 *pu1Key;
    UINT8 u1KeyLen;
    BOOL fgWarppedIV;
    UINT8 *pu1IV;
    UINT8 u1IvLen;
    DMX_DESC_RTB_MODE_T eRtbMode;
} DMX_VUDU_KEY_T;
//#endif //  CC_VUDU_SUPPORT

//-----------------------------------------------------------------------------
// --- Beginning of "CI SLT from TS-output" ---
#if defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)

typedef enum
{
    DMX_TSOUT_LATCH_EDGE_POSITIVE,
    DMX_TSOUT_LATCH_EDGE_NEGATIVE,
    DMX_TSOUT_LATCH_EDGE_UNKNOWN
} DMX_TSOUT_LATCH_EDGE_T;

typedef enum
{
    DMX_TSOUT_DATAWIDTH_1,
    DMX_TSOUT_DATAWIDTH_2,
    DMX_TSOUT_DATAWIDTH_8,
    DMX_TSOUT_DATAWIDTH_UNKNOWN,
} DMX_TSOUT_DATAWIDTH_T;

typedef enum
{
    DMX_TSOUT_SYNC_CYCLE_8,
    DMX_TSOUT_SYNC_CYCLE_1,
    DMX_TSOUT_SYNC_CYCLE_UNKNOWN
} DMX_TSOUT_SYNC_CYCLE_T;

typedef enum
{
    DMX_TSOUT_ENDIAN_MSB,
    DMX_TSOUT_ENDIAN_LSB,
    DMX_TSOUT_ENDIAN_UNKNOWN
} DMX_TSOUT_ENDIAN_T;

typedef enum
{
    DMX_TSOUT_ROUTE_DBM_LOOPBACK,
    DMX_TSOUT_ROUTE_EXT_LOOPBACK,
    DMX_TSOUT_ROUTE_EXTERNAL_CI,
    DMX_TSOUT_ROUTE_INTERNAL_CI,
    DMX_TSOUT_ROUTE__UNKNOWN
} DMX_TSOUT_ROUTE_T;


EXTERN BOOL DMX_TsOut_SetLatchEdge(DMX_TSOUT_LATCH_EDGE_T eEdge);

EXTERN BOOL DMX_TsOut_SetValidCycle(UINT32 u4Cycle);

EXTERN BOOL DMX_TsOut_SetSyncCycle(DMX_TSOUT_SYNC_CYCLE_T eSyncCycle);

EXTERN BOOL DMX_TsOut_SetDataWidth(DMX_TSOUT_DATAWIDTH_T eDataWidth);

EXTERN BOOL DMX_TsIn_SetDataWidth(UINT8 u1TsIndex, DMX_TSOUT_DATAWIDTH_T eDataWidth);

EXTERN BOOL DMX_TsOut_SetEndianess(DMX_TSOUT_ENDIAN_T eEndian);

EXTERN BOOL DMX_TsOut_SetRoute(UINT8 u1TsIndex, DMX_TSOUT_ROUTE_T eRoute);

EXTERN BOOL DMX_TsOut_TransmitData(UINT8 u1TsIndex, UINT32 u4Size);

EXTERN BOOL _DMX_PVRPlay_SetTSOut(BOOL fgEnable);

EXTERN void _GenerateRandomPattern(UINT32 u4Addr, UINT32 u4Size);

EXTERN BOOL _GenerateSimpleTsHeader(UINT32 u4TsAddr, UINT32 u4TsSize,
                    UINT16 u2Pid, UINT32 u4PesAddr, UINT32 *pu4PesSize);

EXTERN BOOL _MemCmp(UINT32 u4S1, UINT32 u4S2, UINT32 u4Size, UINT32* pu4DiffS1,
                    UINT32* pu4DiffS2);

EXTERN void _DumpBuffer(const UINT8 au1Buf[], UINT32 u4Size,
    UINT32 u4BytesPerLine);

#endif  // defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)
// --- End of "CI SLT from TS-output" ---
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Data size calculation
#define DATASIZE(rp, wp, size)      \
    (((rp) <= (wp)) ? ((wp) - (rp)) : (((wp) + (size)) - (rp)))


#define EMPTYSIZE(rp, wp, size)     \
    (((wp) < (rp)) ? ((rp) - (wp)) : (((rp) + (size)) - (wp)))

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------
// Startup

//-----------------------------------------------------------------------------
/** Initialize demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_Init(void);

EXTERN VOID DMX_SetIgnorePESLen(BOOL fgEnable);

EXTERN VOID DMX_SetScrambleScheme(BOOL fgOrg);

EXTERN BOOL DMX_GetScrambleScheme(void);

EXTERN VOID DMX_SetScrambleSchemeEx(DMX_SCRAMBLE_TYPE_T eType, UINT8 u1Flag);

EXTERN BOOL DMX_GetScrambleSchemeEx(DMX_SCRAMBLE_TYPE_T *peType, UINT8 *pu1Flag);

EXTERN VOID DMX_EnablePower(BOOL fgEnable);

EXTERN BOOL DMX_SelectInputType(DMX_INPUT_TYPE_T rInputType);

EXTERN DMX_INPUT_TYPE_T DMX_GetInputType(void);


//-----------------------------------------------------------------------------
/** Reset demux driver
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_Reset(void);

EXTERN BOOL DMX_SetFrontEnd(DMX_FRONTEND_T eFEType);

EXTERN DMX_FRONTEND_T DMX_GetFrontEnd(void);

EXTERN BOOL DMX_SetFrontEndEx(UINT8 u1TsIdx, DMX_FRONTEND_T eInput);

EXTERN DMX_FRONTEND_T DMX_GetFrontEndEx(UINT8 u1TsIdex);

EXTERN BOOL DMX_CustFrontEnd(void);

EXTERN void DMX_ResetFramer(void);

EXTERN UINT8 DMX_GetFramerIndex(void);

EXTERN BOOL DMX_Start(void);

EXTERN BOOL DMX_Stop(void);

EXTERN VOID DMX_Version(void);

//---------------------------------------------------------------------
// PID operations

//-----------------------------------------------------------------------------
/** Set a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPes);

EXTERN BOOL DMX_SetPidEx(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid, DMX_SOURCE_T eSource);

//-----------------------------------------------------------------------------
/** Set a scramble detection PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetScrambleCheckPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid);

// Abadon
EXTERN BOOL DMX_SetFilePid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid);

//-----------------------------------------------------------------------------
/** DMX_TS_SetAudioType
 *
 *  @param  u1Pidx          PID index
 *  @param  eAudioType      Audio type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_TS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType);

//-----------------------------------------------------------------------------
/** _DMX_TS_GetAudioType
 *
 *  @param  u1Pidx              PID index
 *
 *  @retval eAudioType          Audio type
 */
//-----------------------------------------------------------------------------
EXTERN DMX_AUDIO_TYPE_T DMX_TS_GetAudioType(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Get a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param[out] prPid       The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid);

//-----------------------------------------------------------------------------
/** Get a PID
 *  CLI version, no API lock
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_CLIGetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid);

//-----------------------------------------------------------------------------
/** Free a PID
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_FreePid(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param[out] pu1PidIndex The PID index if found
 *
 *  @retval TRUE            Found
 *  @retval FALSE           Not found
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetPidIndex(UINT16 u2Pid, UINT8* pu1PidIndex);

//-----------------------------------------------------------------------------
/** DMX_FlushPidBuffer(UINT8 u1Pidx)
 *  Flush PID buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_FlushPidBuffer(UINT8 u1Pidx);

//---------------------------------------------------------------------
// PCR

//-----------------------------------------------------------------------------
/** Set a PCR
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  fgEnable        Enable PCR of the PID or not
 *  @param  fgNewMode       Use the new PCR mode or not
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetPcr(UINT8 u1Pidx, BOOL fgEnable, BOOL fgNewMode);

//-----------------------------------------------------------------------------
/** DMX_SetStcToDspUpdate
 *  Select STC for DSP update
 *
 *  @param  u1Id            STC index (0 - 1)
 *  @param  fgEnable        Enable PCR of the PID or not
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetStcToDspUpdate(UINT8 u1Id, BOOL fgEnable);

//---------------------------------------------------------------------
// Descrambler

//-----------------------------------------------------------------------------
/** Set a desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fg3Des          TRUE: 3DES, FALSE: DES
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven,
    const UINT32 au4Key[6]);

//-----------------------------------------------------------------------------
/** Set the initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetDesIV(UINT32 u4IvHi, UINT32 u4IvLo);
EXTERN void DMX_SetDesIVEx(UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven);
EXTERN void DMX_SetDesIVEx2(UINT8 u1Idx, UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven);

EXTERN void DMX_SetDesRtbMode(UINT8 u1Idx, DMX_DESC_RTB_MODE_T eMode);

EXTERN BOOL DMX_SetAesKeyLen(UINT8 u1Idx, UINT32 u4KeyLen);

EXTERN BOOL DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8]);

EXTERN void DMX_SetAesIV(const UINT32 au4Iv[4]);

EXTERN void DMX_SetAesIVEx(const UINT32 au4Iv[4], BOOL fgEven);

EXTERN void DMX_SetAesIVEx2(UINT8 u1Idx, const UINT32 au4Iv[4], BOOL fgEven);

EXTERN void DMX_SetAesRtbMode(UINT8 u1Idx, DMX_DESC_RTB_MODE_T eMode);
//#ifdef CC_VUDU_SUPPORT
EXTERN BOOL DMX_VUDU_SetKey(DMX_VUDU_KEY_T *prVuduKey);
//#endif //  CC_VUDU_SUPPORT

#ifdef CC_DMX_SUPPORT_MULTI2
EXTERN BOOL DMX_SetMulti2Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[2]);

EXTERN void DMX_SetMulti2CbcIV(UINT8 u1Index, UINT32 u4IvHi, UINT32 u4IvLo);

EXTERN void DMX_SetMulti2OfbIV(UINT8 u1Index, UINT32 u4IvHi, UINT32 u4IvLo);

EXTERN void DMX_SetMulti2SysKey(UINT8 u1Index, const UINT32 au4Key[8]);

EXTERN void DMX_SetMulti2Config(UINT8 u1Index, BOOL fgOfbFreeRun, UINT16 u2Iteration);
#endif  // CC_DMX_SUPPORT_MULTI2

//-----------------------------------------------------------------------------
/** Setup descrambler output buffer
 *
 *  @param  fgEnable        Enable or disable output buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCaOutputBuffer(BOOL fgEnable, BOOL fgIsPs,
    UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Threshold);

//-----------------------------------------------------------------------------
/** Setup descrambler input buffer
 *
 *  @param  fgEnable        Enable or disable input buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCaInputBuffer(BOOL fgEnable, BOOL fgIsPs,
    UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Threshold);


//---------------------------------------------------------------------
// Capture function

//-----------------------------------------------------------------------------
/** Setup capture using CA output buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCapture(BOOL fgEnable, const DMX_CAPTURE_T* prCapture);


//-----------------------------------------------------------------------------
/** Setup capture using CA output buffer, no allocate buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCapture2(BOOL fgEnable, const DMX_CAPTURE2_T* prCapture);


//-----------------------------------------------------------------------------
/** Update read pointer of capture buffer
 *
 *  @param  u4NewRp         The new read pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_UpdateCaptureReadPointer(UINT32 u4NewRp);

//-----------------------------------------------------------------------------
/** Get capture info
 *
 *  @param[out] prInfo      The capture info
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetCaptureInfo(DMX_CAPTURE_INFO_T* prInfo);

//-----------------------------------------------------------------------------
/** DMX_DemodDataCapture
 *  Setup capture using CA output buffer, not allocate buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  u4BufStart      Buffer Start
 *  @param  u4BufStart      Buffer Size
 *  @param  u4Threshold    Threshold
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_DemodDataCapture(BOOL fgEnable, const UINT32 u4BufStart,
    const UINT32 u4Size, const UINT32 u4Threshold);

//-----------------------------------------------------------------------------
/** DMX_Capture_ChannelChangeInit
 *  Setup capture buffer for channel change
 *
 *  @param  u4Size          Capture buffer size
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_Capture_ChannelChangeInit(UINT32 u4Size);

//-----------------------------------------------------------------------------
/** DMX_SetChannelChange
 *  Inform channel change
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_InformChannelChange(void);

//---------------------------------------------------------------------
// PSI

//-----------------------------------------------------------------------------
/** Setup a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetFilter(UINT8 u1FilterIndex, UINT8 u1Bank,
    UINT32 u4Flags, const DMX_FILTER_T* prFilter);

//-----------------------------------------------------------------------------
/** DMX_SetGenFilter
 *  Setup a generic section filter
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                               const DMX_FILTER_GENERIC_T* prFilter);

//-----------------------------------------------------------------------------
/** Get a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param[out] prFilter    Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
    DMX_FILTER_T* prFilter);

//-----------------------------------------------------------------------------
/** DMX_GetGenFilter
 *  Get a generic section filter setting
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                DMX_FILTER_GENERIC_T* prFilter);

//-----------------------------------------------------------------------------
/** DMX_SetVideoType
 *  Set video type
 *
 *  @param  eVDec           VDec type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetVideoType(DMX_VIDEO_TYPE_T eVDec);

//-----------------------------------------------------------------------------
/** DMX_SetVideoTypeByPidx
 *  Set video type by Pidx
 *
 *  @param  u1Pidx          PID index
 *  @param  eVideoType      VDec type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetVideoTypeByPidx(UINT8 u1Pidx, DMX_VIDEO_TYPE_T eVideoType);

//-----------------------------------------------------------------------------
/** Allocate section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Size          Buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Free a section buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_FreeBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Unlock a section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4Size, UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Flush a section buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_FlushBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Copy data from a section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param[out] pu1Dest     Destination buffer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4FrameAddr, UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);

//-----------------------------------------------------------------------------
/** Get the free size of the PTS-PSI buffer.
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval                 The free size of the PTS-PSI buffer.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DMX_PSI_GetMinFreeBufferSize(VOID);

#ifdef DMX_MEASURE_PSI_TIME
EXTERN VOID DMX_PSI_GetMaxTime(HAL_TIME_T* prTime);

EXTERN VOID DMX_ResetPSIMaxTime(void);
#endif  // DMX_MEASURE_PSI_TIME

//---------------------------------------------------------------------
// PES

//-----------------------------------------------------------------------------
/** Allocate PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Size          Buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Free a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_FreeBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Unlock a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4Size, UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Flush a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_FlushBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Copy data from a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param[out] pu1Dest     Destination buffer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4FrameAddr, UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);

//---------------------------------------------------------------------
// Inter-driver interfaces


#ifdef CC_DMX_PES_EXT_BUFFER
//-----------------------------------------------------------------------------
/** DMX_UpdateExtBufferReadPointer
 *  Update the read pointer of the PES extension buffer.
 *
 *  @param  u1PIdx          PID index
 *  @param  u4NewRp         The new read pointer
 *  @param  u4DataAddr      The address of the current PES_extension_field data
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_UpdateExtBufferReadPointer(UINT8 u1Pidx, UINT32 u4NewRp,
    UINT32 u4DataAddr);
#endif  // CC_DMX_PES_EXT_BUFFER


//-----------------------------------------------------------------------------
/** Update PID read pointer.
 *  This function is used by decoder drivers only.
 *
 *  @param  u1PIdx          PID index
 *  @param  u4Rp            The read pointer
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_UpdateReadPointer(UINT8 u1Pidx, UINT32 u4Rp,
    UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Update read pointer of video PID buffer.
 *  This function is used by video decoder driver only.
 *
 *  @param  u1Channel       The channel
 *  @param  u4Rp            The new read pointer
 *  @param  u4FrameAddr     The frame (picture) address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_UpdateVideoReadPointer(UINT8 u1Channel, UINT32 u4Rp,
    UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Set interrupt threshold for partial PES.
 *  This function is used by decoder drivers only.
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  u4Bytes         Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType,
    UINT32 u4Bytes);

//-----------------------------------------------------------------------------
/** Get interrupt threshold for partial PES.
 *  This function is used by decoder drivers only.
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param[out] pu4Bytes    Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType,
    UINT32* pu4Bytes);

//---------------------------------------------------------------------
// Misc.

//-----------------------------------------------------------------------------
/** Set decoder callback functions
 *
 *  @param  prCallbacks     The decoder callback functions
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetDecoderCallbacks(
    const DMX_DECODER_CALLBACKS_T* prDecoderCallbacks);

//-----------------------------------------------------------------------------
/** Get corresponding video channel of a given pid index
 *
 *  @param  u1Pid           PID index
 *  @param[out] pu1Channel  The channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel);

//-----------------------------------------------------------------------------
/** Set demux (DBM) to bypass mode, mainly used for frontend capture function
 *
 *  @param  u1TsIndex       TS index (0 - 1)
 *  @param  u1PacketSize    TS packet size in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetBypassMode(UINT8 u1TsIndex, UINT8 u1PacketSize);

//-----------------------------------------------------------------------------
/** Set data sink to real decoders
 *
 *  @param  fgToDecoder     TRUE: to decoders, FALSE: not to decoders
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetToDecoder(BOOL fgToDecoder);

//-----------------------------------------------------------------------------
/** Query if data sink is set to real decoders
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_IsToDecoder(void);

//---------------------------------------------------------------------
// Utilities

//-----------------------------------------------------------------------------
/** Calculate aligned address by a given address and an alignment
 *
 *  @param  u4Addr          The address to be aligned
 *  @param  u4Alignment     The alignment requirement in byte
 *
 *  @return     The aligned address
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DMX_Align(UINT32 u4Addr, UINT32 u4Alignment);

//-----------------------------------------------------------------------------
/** Query if an address is aligned or not
 *
 *  @param  u4Addr          The address to be checked
 *  @param  u4Alignment     The alignment requirement in byte
 *
 *  @retval TRUE            Aligned
 *  @retval FALSE           Not aligned
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_IsAligned(UINT32 u4Addr, UINT32 u4Alignment);

//---------------------------------------------------------------------
// Debug

EXTERN BOOL DMX_GetLockState(void);

EXTERN BOOL DMX_GetLockStateEx(UINT8 u1FramerIdx);

EXTERN BOOL DMX_IsDebugPictureFinder(void);

//-----------------------------------------------------------------------------
/** Enable or disable debugging picture finder.
 *  This function is for debugging purpose only.
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetDebugPictureFinder(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** Get the counter structure of a given pid index.
 *  This function is for debugging purpose only.
 *
 *  @param  u1Pidx          PID index
 *  @param[out] prCounters  Pointer to the PID counter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetPidCounters(UINT8 u1Pidx,
    DMX_PID_COUNTERS_T* const prCounters);

//-----------------------------------------------------------------------------
/** Reset counters of a given pid index.
 *  This function is for debugging purpose only.
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_ResetPidCounters(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Set error handling table of framers.
 *  This function is for capture tool only.
 *
 *  @param  u4Value         New value of error handling table
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetFramerErrorHandlingTable(UINT32 u4Value);

//-----------------------------------------------------------------------------
/** Get error handling table of framers.
 *  This function is for capture tool only.
 *
 *  @return     The current value of error handling table
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DMX_GetFramerErrorHandlingTable(void);

//-----------------------------------------------------------------------------
/** Get scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @return     The scramble state
 */
//-----------------------------------------------------------------------------
EXTERN DMX_SCRAMBLE_STATE_T DMX_GetScrambleState(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Demux diagnostic function.
 *  This function is for debugging purpose only.
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DMX_Diag(void);

EXTERN UINT32 DMX_GetReceivedPacketNumber(void);

EXTERN UINT32 DMX_GetDroppedPacketNumber(void);

EXTERN BOOL DMX_SetPacketSize(UINT8 u1PacketSize);

EXTERN BOOL DMX_SetPacketSizeEx(UINT8 u1TsIndex, UINT16 u1PktSize);

EXTERN UINT32 DMX_GetMinFreeBufferSize(DMX_PID_TYPE_T ePidType);

EXTERN UINT32 DMX_GetMinFreeDRAMBufferSize(DMX_PID_TYPE_T ePidType);

EXTERN UINT32 DMX_GetMinFreeBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType);

EXTERN UINT32 DMX_GetMinFreeDRAMBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType);

EXTERN void DMX_PcrInit(void);

EXTERN BOOL DMX_EnableLocalArbitor(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, BOOL fgEnable);

EXTERN BOOL DMX_DRAMMeasure(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, UINT32 u4TimeMs);

EXTERN void DMX_CheckAwSupport(BOOL *pfgDriver, BOOL *pfgMicroCode);

//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---
EXTERN void DMX_AwEraserCheck_Init(void);
EXTERN void DMX_AwEraserCheck_Free(void);
EXTERN void DMX_AwEraserCheck_Set(BOOL fgEnable);
EXTERN INT32 DMX_AwEraserCheck_Get(UINT32 u4Timeout);
// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------

#ifdef CC_DMX_TS_LOOPBACK
EXTERN BOOL DMX_Loopback_Set(void);

EXTERN BOOL DMX_Loopback_Free(void);

EXTERN BOOL DMX_Loopback_Enable(BOOL fgEnable);
#endif  // CC_DMX_TS_LOOPBACK

EXTERN void DMX_SetErrLevel(DMX_ERR_LEVEL_T eLevel);

EXTERN BOOL DMX_SetTSMode(UINT8 u1Inst, DMX_TSFMT_T eFmt);
EXTERN BOOL DMX_SetDvbKey(UINT8 u1KeyIndex, BOOL fgEven,const UINT32 au4Key[2]);
EXTERN void DMX_SetDvbIVEx(UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven);


EXTERN void DMX_SetExternalFramer(DMX_EXT_FRAMER_T eFramer);

EXTERN void DMX_GetPicQSize(UINT16* u2DmxPictureQSize, UINT16* u2DmxMaxQSize);

#endif  // DMX_IF_H

