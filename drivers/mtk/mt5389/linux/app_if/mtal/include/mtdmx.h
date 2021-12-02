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
 * $RCSfile: mtdmx.h,v $
 * $Revision: #5 $
 *---------------------------------------------------------------------------*/

/** @file mtdmx.h
 *  This header file declares exported APIs of DMX module.
 */

#ifndef MT_DMX_H
#define MT_DMX_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MTDMX_NUM_PID_INDEX				  80
#define MTDMX_NUM_FILTER_INDEX            96
#define MTDMX_KEY_MAX_NUM                 8

#define MTDMX_TS_INDEX_0                  0
#define MTDMX_TS_INDEX_1                  1
#define MTDMX_TS_INDEX_2                  2
#define MTDMX_TS_INDEX_PVR                2
#define MTDMX_TS_INDEX_3                  3

#define MTDMX_KEY_INDEX_0                 MTDMX_TS_INDEX_0
#define MTDMX_KEY_INDEX_1                 MTDMX_TS_INDEX_1
#define MTDMX_KEY_INDEX_2                 MTDMX_TS_INDEX_2
#define MTDMX_KEY_INDEX_PVR               MTDMX_TS_INDEX_PVR
#define MTDMX_KEY_INDEX_3                 MTDMX_TS_INDEX_3

/* STC ID */
#define STC_1                             0
#define STC_2                             1
#define STC_NS                            2


/// Function bitmap of PID operations
#define MTDMX_PID_FLAG_VALID              0x00000001  ///< Enabled or not
#define MTDMX_PID_FLAG_PID                0x00000002  ///< PID value
#define MTDMX_PID_FLAG_BUFFER             0x00000004  ///< Buffer control
#define MTDMX_PID_FLAG_CALLBACK           0x00000008  ///< Callback handler
#define MTDMX_PID_FLAG_SCRAMBLE_STATE     0x00000010  ///< Scrambling state
#define MTDMX_PID_FLAG_DEVICE_ID          0x00000200  ///< Device (decoder) ID
#define MTDMX_PID_FLAG_DISABLE_OUTPUT     0x00020000  ///< Disable FIFO output
#define MTDMX_PID_FLAG_NONE               0           ///< None
#define MTDMX_PID_FLAG_ALL                (0xFFFFFFFF & ~MTDMX_PID_FLAG_DISABLE_OUTPUT)

/// Function bitmap of filter operations
#define MTDMX_FILTER_FLAG_VALID           0x00000001  ///< Enabled or not
#define MTDMX_FILTER_FLAG_PIDX            0x00000002  ///< PID index
#define MTDMX_FILTER_FLAG_OFFSET          0x00000004  ///< Offset
#define MTDMX_FILTER_FLAG_CRC             0x00000008  ///< Check CRC or not
#define MTDMX_FILTER_FLAG_PATTERN         0x00000010  ///< Pattern and mask
#define MTDMX_FILTER_FLAG_NONE            0           ///< None
#define MTDMX_FILTER_FLAG_ALL             0xffffffff  ///< All

/// DRM
#define DMX_MM_DRM_FLAG_OFFSET              0x00000001
#define DMX_MM_DRM_FLAG_ENCRYLEN            0x00000002
#define DMX_MM_DRM_FLAG_CBC                 0x00000004
#define DMX_MM_DRM_FLAG_KEY                 0x00000008
#define DMX_MM_DRM_FLAG_NONE                0x00000000
#define DMX_MM_DRM_FLAG_ALL                 0xFFFFFFFF

#define MTDMX_AES_CMD_SID             (0x2 << 8)
#define MTDMX_MISC_CMD_SID            (0x8 << 8)
#define MTDMX_GCPU_FIFO_ALIGNMENT     (32)
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#if defined(CC_MT5365)||defined(CC_MT5395)
typedef enum
{
    MTDMX_FE_EXT_SERIAL0,
    MTDMX_FE_EXT_SERIAL1,
    MTDMX_FE_EXT_PARALLEL,
    MTDMX_FE_EXT_TWOBIT0,
    MTDMX_FE_EXT_TWOBIT1,
    MTDMX_FE_DVB_DEMOD,       // avoid compile error
    MTDMX_FE_ATSC_DEMOD
} MTDMX_FRONTEND_T;
#elif defined(CC_MT5396)
typedef enum
{
    MTDMX_FE_INTERNAL = 0,
    //DMX_FE_EXTERNAL = 1,
    MTDMX_FE_DDI = 2,
    MTDMX_FE_TSOUT = 3,
    MTDMX_FE_CI = 4,
    MTDMX_FE_EXT_S = 0x10,
    MTDMX_FE_EXT_P = 0x11,
    MTDMX_FE_EXT_2BIT = 0x12,
    MTDMX_FE_EXT2_0_S = 0x20,
    MTDMX_FE_EXT2_0_P = 0x21,
    MTDMX_FE_EXT2_0_2BIT = 0x22,
    MTDMX_FE_EXT2_1_S = 0x30,
    MTDMX_FE_EXT2_1_P = 0x31,       // Ext2_1 no parallel, test
    MTDMX_FE_EXT2_1_2BIT = 0x32,
    MTDMX_FE_EXT3_S = 0x40,
    MTDMX_FE_EXT3_P = 0x41,
    MTDMX_FE_EXT3_2BIT = 0x42,
    MTDMX_FE_EXT_LEGACY_P = 0x51,
    MTDMX_FE_NO_TSVALID = 0x60,

    // following avoid compile error
    MTDMX_FE_EXT_SERIAL0 = 0xF0,
    MTDMX_FE_EXT_SERIAL1 = 0xF1,
    MTDMX_FE_EXT_PARALLEL = 0xF2,
    MTDMX_FE_EXT_TWOBIT0 = 0xF3,
    MTDMX_FE_EXT_TWOBIT1 = 0xF4,
    MTDMX_FE_DVB_DEMOD = 0xF5,
    MTDMX_FE_ATSC_DEMOD = 0xF6,
    MTDMX_FE_NULL = 0xFF
} MTDMX_FRONTEND_T;
#else
typedef enum
{
    MTDMX_FE_ATSC_DEMOD,
    MTDMX_FE_DVB_DEMOD,
    MTDMX_FE_EXT_PARALLEL,
    MTDMX_FE_EXT_SERIAL
} MTDMX_FRONTEND_T;
#endif

/// PID types
typedef enum
{
    MTDMX_PID_TYPE_NONE = 0,      ///< None
    MTDMX_PID_TYPE_PSI,           ///< PSI
    MTDMX_PID_TYPE_PES,           ///< PES
    MTDMX_PID_TYPE_PES_TIME,      ///< PES with time information
    MTDMX_PID_TYPE_ES_VIDEO,      ///< Video ES
    MTDMX_PID_TYPE_ES_AUDIO,      ///< Audio ES
    MTDMX_PID_TYPE_ES_VIDEOCLIP,  ///< Video clip ES
    MTDMX_PID_TYPE_ES_OTHER,       ///< Other ES
    MTDMX_PID_TYPE_ES_DTCP,
    MTDMX_PID_TYPE_TS_RAW
} MTDMX_PID_TYPE_T;

enum
{
    MTDMX_GCPU_AES_D = MTDMX_AES_CMD_SID,
    MTDMX_GCPU_AES_E,
    MTDMX_GCPU_AESPK_D,
    MTDMX_GCPU_AES_G,
    MTDMX_GCPU_AES_DPAK,
    MTDMX_GCPU_AES_CMAC,
    MTDMX_GCPU_AESPK_DPAK,
    MTDMX_GCPU_AES_EPAK,
    MTDMX_GCPU_AES_D_CMP,
    MTDMX_GCPU_AESPK_E,
    MTDMX_GCPU_AESEK_E,
    MTDMX_GCPU_AESPK_EPAK,
    MTDMX_GCPU_AESPK_DCBC,
    MTDMX_GCPU_AESPK_ECBC,
    MTDMX_GCPU_AESEK_D,
    MTDMX_GCPU_AES_DCBC,
    MTDMX_GCPU_AES_ECBC,
    MTDMX_GCPU_AESPK_EK_D,
    MTDMX_GCPU_AESPK_EK_E,
    MTDMX_GCPU_AESPK_EK_DCBC,
    MTDMX_GCPU_AES_H,
    MTDMX_GCPU_AES_CTR,
    MTDMX_GCPU_AES_OFB,
    MTDMX_GCPU_AES_WRAPD,
    MTDMX_GCPU_AES_WRAPE,
    MTDMX_GCPU_AES,
};

enum
{
    MTDMX_GCPU_MEMCPY = MTDMX_MISC_CMD_SID,
    MTDMX_GCPU_DMA,
    MTDMX_GCPU_SHA_1,
    MTDMX_GCPU_SHA_256,
    MTDMX_GCPU_MD5,
    MTDMX_GCPU_SHA_224,
    MTDMX_GCPU_MEM_XOR,
    MTDMX_GCPU_RNG,
    MTDMX_GCPU_RC4_KSA,
    MTDMX_GCPU_RC4,
    MTDMX_GCPU_ROM_BIST,
    MTDMX_GCPU_PTX,
    MTDMX_GCPU_TSDESC,
};

typedef enum
{
    MTDMX_TSDESC_188_PACKET_FORM,
    MTDMX_TSDESC_192_PACKET_FORM,
} MTDMX_TSDESC_PACKET_FORM;

typedef enum
{
    MTDMX_TSDESC_FIRST_PACKET_MODE,
    MTDMX_TSDESC_SUCCESSIVE_PACKET_MODE,
} MTDMX_TSDESC_MODE;

typedef enum
{
    MTDMX_TSDESC_DESC_SCHEME_CBC_SCTE52 = 0xE,
    MTDMX_TSDESC_DESC_SCHEME_CTR = 0xF,
} MTDMX_TSDESC_DESC_SCHEME;

/// Notification code
typedef enum
{
    MTDMX_NOTIFY_CODE_PSI,                ///< PSI notification
    MTDMX_NOTIFY_CODE_ES,                 ///< ES notification
    MTDMX_NOTIFY_CODE_PES,                ///< PES notification
    MTDMX_NOTIFY_CODE_PES_TIME,           ///< PES notification with time information
    MTDMX_NOTIFY_CODE_SCRAMBLE_STATE,     ///< Scramble state change notification
    MTDMX_NOTIFY_CODE_OVERFLOW,           ///< Overflow notification
    MTDMX_NOTIFY_CODE_STREAM_ID,          ///< Report pre-parsed Stream IDs
    MTDMX_NOTIFY_CODE_RAW_TS              ///< Rwa TS notification
} MTDMX_NOTIFY_CODE_T;

/// Demux notification callback function
typedef MT_RESULT_T (*PFN_MTDMX_NOTIFY)(UINT8 u1Pidx,
                                        MTDMX_NOTIFY_CODE_T eCode,
                                        UINT32 u4Data,
                                        const void* pvNotifyTag);

/// PID data structure
typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgAllocateBuffer;          ///< Allocate buffer
    UINT16 u2Pid;                   ///< PID
    UINT32 u4BufAddr;               ///< Buffer address, kernel address
    UINT32 u4BufSize;               ///< Buffer size
    MTDMX_PID_TYPE_T ePidType;      ///< PID type
    PFN_MTDMX_NOTIFY pfnNotify;     ///< Callback function
    void* pvNotifyTag;              ///< Tag value of callback function
    PFN_MTDMX_NOTIFY pfnScramble;   ///< Callback function of scramble state
    void* pvScrambleTag;            ///< Tag value of scramble callback function
    UINT8 u1DeviceId;               ///< Decoder ID
    UINT8 u1ChannelId;              ///< Channel ID
    UINT8 u1TsIndex;                ///< TS index
    BOOL fgDisableOutput;           ///< No output
} MTDMX_PID_T;


/// Filter structure
typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
    UINT8 u1Pidx;                   ///< PID index, 0 ~ 31
    UINT8 u1Offset;                 ///< Offset
    UINT8 au1Data[16];              ///< Pattern
    UINT8 au1Mask[16];              ///< Mask
} MTDMX_FILTER_T;

/// Filter structure
typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
    UINT8 u1Pidx;                   ///< PID index, 0 ~ 31
    UINT8 u1Offset;                 ///< Offset
    UINT8 au1Data[16];              ///< Pattern
    UINT8 au1Mask[16];              ///< Mask
    UINT8 au1PosNeg[16];            ///< Positive or negative
} MTDMX_GENERIC_FILTER_T;

/// PID counters
typedef struct
{
    UINT32 u4PesCount;              ///< PES counter
    UINT32 u4PicCount;              ///< Picture counter
    UINT32 u4SecCount;              ///< Section counter
} MTDMX_PID_COUNTERS_T;

typedef struct
{
    UINT8 u1TableID;                ///< Table ID
    UINT16 u2SecLen;                ///< Section length
    UINT8 u1VerNum;                 ///< Version number
    UINT32 u4CRC;                   ///< CRC value
} MTDMX_SECTION_HEADER_T;

/// Scrambling state
typedef enum
{
    MTDMX_SCRAMBLE_STATE_CLEAR,           ///< Clear data
    MTDMX_SCRAMBLE_STATE_SCRAMBLED,       ///< Scrambled data
    MTDMX_SCRAMBLE_STATE_UNKNOWN          ///< Unknown data
} MTDMX_SCRAMBLE_STATE_T;

/// Notify info of PSI
typedef struct
{
    UINT32 u4SecAddr;                   ///< Section address, kernel address
    UINT32 u4SecLen;                    ///< Section length
    UINT8 u1SerialNumber;               ///< Serial number
    UINT32 u4MatchBitmap;               ///< Match section filter bitmap
} MTDMX_NOTIFY_INFO_PSI_T;

/// Notify info of PES data
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address, kernel address
    UINT32 u4DataSize;                  ///< Data size
    UINT8 u1SerialNumber;               ///< Serial number
} MTDMX_NOTIFY_INFO_PES_T;

/// Notify info of raw TS data
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
    UINT8 u1SerialNumber;
} MTDMX_NOTIFY_INFO_RAW_TS_T;

typedef enum
{
    MTDMX_VIDEO_MPEG,
    MTDMX_VIDEO_H264,
    MTDMX_VIDEO_AVS,
    MTDMX_VIDEO_VC1,
    MTDMX_VIDEO_MPEG4
} MTDMX_VIDEO_TYPE_T;

/// Descrambling schemes
typedef enum
{
    MTDMX_DESC_MODE_NONE = 0,             ///< None
    MTDMX_DESC_MODE_DES_ECB,              ///< DES ECB
    MTDMX_DESC_MODE_DES_CBC,              ///< DES CBC
    MTDMX_DESC_MODE_3DES_ECB,             ///< 3DES ECB
    MTDMX_DESC_MODE_3DES_CBC,             ///< 3DES CBC
    MTDMX_DESC_MODE_DVB,                  ///< DVB
    MTDMX_DESC_MODE_DVB_CONF,             ///< DVB conformance
    MTDMX_DESC_MODE_MULTI2_BIG,           ///< Multi-2 big-endian
    MTDMX_DESC_MODE_MULTI2_LITTLE,        ///< Multi-2 little-endian
    MTDMX_DESC_MODE_AES_ECB,              ///< AES ECB
    MTDMX_DESC_MODE_AES_CBC               ///< AES CBC
} MTDMX_DESC_MODE_T;

/// Descrambling residual termination block(RTB)
typedef enum
{
    MTDMX_DESC_RTB_MODE_CLEAR = 0,        ///< leave clear
    MTDMX_DESC_RTB_MODE_CTS,              ///< CTS
    MTDMX_DESC_RTB_MODE_SCTE52,           ///< SCTE-52
    MTDMX_DESC_RTB_MODE_MAX,              ///< END
} MTDMX_DESC_RTB_MODE_T;

typedef enum
{
    MTDMX_KEY_BIT_LEN_40 = 40,
    MTDMX_KEY_BIT_LEN_64 = 64,
    MTDMX_KEY_BIT_LEN_128 = 128,
    MTDMX_KEY_BIT_LEN_192 = 192,
    MTDMX_KEY_BIT_LEN_256 = 256,
} MTDMX_KEY_BIT_LEN;

typedef enum
{
    MTDMX_BLOCK_CIPHER_MODE_ECB,
    MTDMX_BLOCK_CIPHER_MODE_CBC,
    MTDMX_BLOCK_CIPHER_MODE_OFB,
    MTDMX_BLOCK_CIPHER_MODE_CTR,
} MTDMX_BLOCK_CIPHER_MODE;

typedef struct
{
    BOOL fgCBC;
    BOOL fgEncrypt;
    UINT8 au1IV[16];
    UINT32 u4KeyBitLen;
    UINT8 au1Key[32];
    UINT32 u4Offset;
    UINT32 u4EncryLen;
} MTDMX_DRM_AES_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    UINT8 au1Key[32];
    UINT8 au1Iv[16];
    MTDMX_KEY_BIT_LEN eKeyBitLen;
    MTDMX_BLOCK_CIPHER_MODE eMode;
    BOOL fgEncrypt;
} MTDMX_AES_PARAM_T;

typedef struct
{
    UINT32 u4Size;
    UINT32 u4SrcAddr;
    UINT32 u4DstAddr;
} MTDMX_DRM_AES_BLKDATA_T;

typedef enum
{
    MTDMX_PID_INSRC_DTV,                    // PID from NIM
    MTDMX_PID_INSRC_TIMESHIFT_PLAY,         // PID from time-shift play
    MTDMX_PID_INSRC_MM,                     // PID from multimedia (USB or network)
} MTDMX_PID_INSRC_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    MTDMX_TSDESC_PACKET_FORM eForm;
    MTDMX_TSDESC_MODE eMode;
    MTDMX_TSDESC_DESC_SCHEME eScheme;
    UINT32 u4TsState[4];
    UINT8 au1Key[2][16];
    UINT8 au1Iv[2][16];
} MTDMX_GCPU_TSDESC_PARAM_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define MTDMX_GCPU_LINER_BUFFER_START(u4Addr) (((u4Addr) % MTDMX_GCPU_FIFO_ALIGNMENT) == 0)?(u4Addr): \
                                    (u4Addr) - ((u4Addr) % MTDMX_GCPU_FIFO_ALIGNMENT)

#define MTDMX_GCPU_LINER_BUFFER_END(u4Addr) (((u4Addr) % MTDMX_GCPU_FIFO_ALIGNMENT) == 0)?(u4Addr): \
                                    (u4Addr) + (MTDMX_GCPU_FIFO_ALIGNMENT - ((u4Addr) % MTDMX_GCPU_FIFO_ALIGNMENT))

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** MTDMX_Init
 *  Init demux driver
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_Init(void);

//-----------------------------------------------------------------------------
/** MTDMX_Reset
 *  Reset demux driver
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_Reset(void) ;

//-----------------------------------------------------------------------------
/** MTDMX_SetFrontEnd
 *  Select front end type
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetFrontEnd (MTDMX_FRONTEND_T eFE);

//-----------------------------------------------------------------------------
/** MTDMX_SetFrontEndEx
 *  Select front end type
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetFrontEndEx (UINT8 u1TsIdx, MTDMX_FRONTEND_T eFE);

//-----------------------------------------------------------------------------
/** MTDMX_SetVideoType
 *  Select video type
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetVideoType (MTDMX_VIDEO_TYPE_T eType);

//-----------------------------------------------------------------------------
/** MTDMX_Start
 *  Start demux
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_Start (void);

//-----------------------------------------------------------------------------
/** MTDMX_Stop
 *  Stop demux
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_Stop (void);

//-----------------------------------------------------------------------------
/** MTDMX_SetPid
 *  Set a PID
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetPid (UINT8 u1Pidx, UINT32 u4Flags, const MTDMX_PID_T *prPid);

//-----------------------------------------------------------------------------
/** MTDMX_GetPid
 *  Get a PID
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetPid (UINT8 u1Pidx, UINT32 u4Flags, MTDMX_PID_T *prPid);

//-----------------------------------------------------------------------------
/** MTDMX_FreePid
 *  Free a PID
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_FreePid (UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** MTDMX_GetPidIndex
 *  Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param  pu1PidIndex [out]
 *                          The PID index if found
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetPidIndex (UINT16 u2Pid, UINT8 *pu1PidIndex);

//-----------------------------------------------------------------------------
/** MTDMX_SetVideoPid
 *  Set a Video Pid
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PidNum		Video PID number
 *  @param  fgEnable		Enable video pid or not
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetVideoPid (UINT8 u1Pidx, UINT16 u2PidNum, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** MTDMX_SetAudioPid
 *  Set a Audio Pid
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PidNum		Video PID number
 *  @param  fgEnable		Enable audio pid or not
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAudioPid (UINT8 u1Pidx, UINT16 u2PidNum, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** MTDMX_SetAudioDescriptPid
 *  Set a Audio Pid
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PidNum		Video PID number
 *  @param  fgEnable		Enable audio pid or not
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAudioDescriptPid (UINT8 u1Pidx, UINT16 u2PidNum, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** MTDMX_SetPcrPid
 *  Set a PCR Pid to one PID index
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u2PcrPid		PCR PID number
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetPcrPid (UINT8 u1Pidx, UINT16 u2PcrPid);

//-----------------------------------------------------------------------------
/** MTDMX_SetPcrPidEx
 *  Set a PCR Pid to one PID index
 *
 *  @param  u1TsIdx         TS index (0 - 3)
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  u1StcSet        Stc set (0 - 1)
 *  @param  u2PcrPid        PCR PID number
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetPcrPidEx (UINT8 u1TsIdx, UINT8 u1Pidx, UINT8 u1StcSet, UINT16 u2PcrPid);

//-----------------------------------------------------------------------------
/** MTDMX_SetPcr
 *  Set a PCR
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  fgEnable        Enable PCR of the PID or not
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetPcr (UINT8 u1Pidx, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** MTDMX_SetFilter
 *  Setup a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetFilter (UINT8 u1FilterIndex,
                                    UINT32 u4Flags,
                                    const MTDMX_FILTER_T *prFilter);

//-----------------------------------------------------------------------------
/** MTDMX_GetFilter
 *  Get a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetFilter (UINT8 u1FilterIndex,
                                    UINT32 u4Flags,
                                    MTDMX_FILTER_T *prFilter);

//-----------------------------------------------------------------------------
/** MTDMX_PSI_AllocateBuffer
 *  Allocate section buffer pool
 *
 *  @param  u4Size          Buffer size
 *  @param  pu4Addr         Return kernel address
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PSI_AllocateBuffer (UINT32 u4Size, UINT32 *pu4Addr);

//-----------------------------------------------------------------------------
/** MTDMX_PSI_FreeBuffer
 *  Free a section buffer pool
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PSI_FreeBuffer (UINT32 u4Addr);

//-----------------------------------------------------------------------------
/** MTDMX_PSI_GetBuffer
 *  Copy data from a section buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address (Kernel address)
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param  pu1Dest         Destination buffer (User address)
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PSI_GetBuffer(UINT8 u1Pidx,
                                       UINT8 u1SerialNumber,
                                       UINT32 u4FrameAddr,
                                       UINT32 u4SkipSize,
                                       UINT32 u4CopySize,
                                       UCHAR* pu1Dest);

//-----------------------------------------------------------------------------
/** MTDMX_PSI_UnlockBuffer
 *  Unlock a section buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address (Kernel address)
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PSI_UnlockBuffer(UINT8 u1Pidx,
                                          UINT8 u1SerialNumber,
                                          UINT32 u4Size,
                                          UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** MTDMX_PSI_FlushBuffer
 *  Flush a section buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PSI_FlushBuffer (UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** MTDMX_PSI_GetBufferSectionHeader
 *  Get section header and CRC
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4SecSize       Section size
 *  @param  prHeaderCRC     Section header/CRC structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PSI_GetBufferSectionHeader(UINT8 u1Pidx,
                                             UINT8 u1SerialNumber,
                                             UINT32 u4FrameAddr,
                                             UINT32 u4SkipSize,
                                             UINT32 u4SecSize,
                                             MTDMX_SECTION_HEADER_T *prHeader);

//-----------------------------------------------------------------------------
/** MTDMX_PES_AllocateBuffer
 *  Allocate PES buffer pool
 *
 *  @param  u4Size          Buffer size
 *  @param  pu4Addr         Return allocated buffer address (Kernel address)
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PES_AllocateBuffer (UINT32 u4Size, UINT32 *pu4Addr);

//-----------------------------------------------------------------------------
/** MTDMX_PES_FreeBuffer
 *  Free a PES buffer pool
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PES_FreeBuffer (UINT32 u4Addr);

//-----------------------------------------------------------------------------
/** MTDMX_PES_UnlockBuffer
 *  Unlock a PES buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address (Kernel address)
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PES_UnlockBuffer (UINT8 u1Pidx,
                                           UINT8 u1SerialNumber,
                                           UINT32 u4Size,
                                           UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** MTDMX_PES_FlushBuffer
 *  Flush a PES buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PES_FlushBuffer (UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** MTDMX_PES_GetBuffer
 *  Copy data from a PES buffer
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address (Kernel mode address)
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param  pu1Dest         Destination buffer (User mode address)
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_PES_GetBuffer (UINT8 u1Pidx,
                                        UINT8 u1SerialNumber,
                                        UINT32 u4FrameAddr,
                                        UINT32 u4SkipSize,
                                        UINT32 u4CopySize,
                                        UCHAR *pu1Dest);

//-----------------------------------------------------------------------------
/** MTDMX_GetPidCounters
 *  Get the counter structure of a given pid index
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  prCounters [output]
 *                          Pointer to the PID counter structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetPidCounters (UINT8 u1Pidx,
                             MTDMX_PID_COUNTERS_T *const prCounters);

//-----------------------------------------------------------------------------
/** MTDMX_ResetPidCounters
 *  Reset counters of a given pid index
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_ResetPidCounters (UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** MTDMX_GetLockState
 *  Get lock state
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetLockState (BOOL *pfgLock);

//-----------------------------------------------------------------------------
/** MTDMX_GetScrambleState
 *  Get scramble state
 *
 *  @param  u1Pidx          PID index, 0 ~ 31
 *  @param  peScramble_Sstate    the pointer of  scramble state
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetScrambleState(UINT8 u1Pidx,MTDMX_SCRAMBLE_STATE_T* peScramble_State);

//-----------------------------------------------------------------------------
/** MTDMX_GetCurSTC
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetCurSTC(UINT32 *pu4StcHi, UINT32 *pu4StcLo);


//-----------------------------------------------------------------------------
/** MTDMX_SetGenericFilter
 *  Setup a generic section filter (including postive / negative match)
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetGenericFilter (UINT8 u1FilterIndex,
                                    UINT32 u4Flags,
                                    const MTDMX_GENERIC_FILTER_T *prFilter);

//-----------------------------------------------------------------------------
/*
 *  Get generic section filter setting
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetGenericFilter (UINT8 u1FilterIndex,
                                    UINT32 u4Flags,
                                    MTDMX_GENERIC_FILTER_T *prFilter);

//-----------------------------------------------------------------------------
/** Set DES desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fg3Des          TRUE: 3DES, FALSE: DES
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven, const UINT32 au4Key[6]);

//-----------------------------------------------------------------------------
/** Set the initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetDesIV(UINT32 u4IvHi, UINT32 u4IvLo);

//-----------------------------------------------------------------------------
/** Set the even/odd initial vector of DES
 *
 *  @param  u1Index         The key index
 *  @param  fgEven          Even or odd
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetDesEvenOddIVEx(UINT8 u1Index, BOOL fgEven, UINT32 u4IvHi, UINT32 u4IvLo);

//-----------------------------------------------------------------------------
/** Set RTB mode of DES
 *
 *  @param  eMode          MTDMX_DESC_RTB_MODE_T
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetDesRtbMode(MTDMX_DESC_RTB_MODE_T eMode);

//-----------------------------------------------------------------------------
/** Set RTB mode of DES
 *
 *  @param  u1Index         The key index
 *  @param  eMode           MTDMX_DESC_RTB_MODE_T
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetDesRtbModeEx(UINT8 u1Index, MTDMX_DESC_RTB_MODE_T eMode);

//-----------------------------------------------------------------------------
/** Set AES key len
 *
 *  @param  u4KeyLen        The key length
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesKeyLen(UINT32 u4KeyLen);

//-----------------------------------------------------------------------------
/** Set AES key len
 *
 *  @param  u1Index         The key index
 *  @param  u4KeyLen        The key length
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesKeyLenEx(UINT8 u1Index, UINT32 u4KeyLen);

//-----------------------------------------------------------------------------
/** Set AES key
 *
 *  @param  u1Index         The key index
 *  @param  fgEven          If fgEven=TRUE means set even key
 *  @param  au4Key          The key value
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8]);

//-----------------------------------------------------------------------------
/** Set the initial vector of AES
 *
 *  @param  au4Iv           The initial vector
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesIV(const UINT32 au4Iv[4]);

//-----------------------------------------------------------------------------
/** Set the even/odd initial vector of AES
 *
 *  @param  eType           Initial vector type
 *  @param  au4Iv           The initial vector
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesEvenOddIV(BOOL fgEven, const UINT32 au4Iv[4]);

//-----------------------------------------------------------------------------
/** Set the even/odd initial vector of AES
 *
 *  @param  u1Index         The key index
 *  @param  eType           Initial vector type
 *  @param  au4Iv           The initial vector
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesEvenOddIVEx(UINT8 u1Index, BOOL fgEven, const UINT32 au4Iv[4]);

//-----------------------------------------------------------------------------
/** Set RTB mode of AES
 *
 *  @param  eMode          MTDMX_DESC_RTB_MODE_T
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesRtbMode(MTDMX_DESC_RTB_MODE_T eMode);


//-----------------------------------------------------------------------------
/** Set RTB mode of AES
 *
 *  @param  u1Index         The key index
 *  @param  eMode           MTDMX_DESC_RTB_MODE_T
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesRtbModeEx(UINT8 u1Index, MTDMX_DESC_RTB_MODE_T eMode);

//-----------------------------------------------------------------------------
/** Set AES on off
 *
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetAesOn(BOOL fgAesOn);

//-----------------------------------------------------------------------------
/** Set the initial vector of AES
 *
 *  @param  u1Pidx           PID index value
 *  @param  eDescMode        Descramble mode
 *  @param  u1KyeIndex       Key index value
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetDescramblPid(UINT8 u1Pidx, MTDMX_DESC_MODE_T eDescMode, UINT8 u1KeyIndex);

//-----------------------------------------------------------------------------
/** Set the initial vector of AES
 *
 *  @param  u1Pidx           PID index value
 *  @param  eDescMode        Descramble mode
 *  @param  u1KyeIndex       Key index value
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetFileDescramblPid(UINT8 u1Pidx, MTDMX_DESC_MODE_T eDescMode, UINT8 u1KeyIndex);

//-----------------------------------------------------------------------------
/*
 *  Setup AES decryption engine
 *
 *  @param  u4Flags         Function flags
 *  @param  prDRM           AES structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_AES_SetDecryptInfo(UINT32 u4Flags, const MTDMX_DRM_AES_T *prDRM);

//-----------------------------------------------------------------------------
/*
 *  Use AES decryption engine to decrypt data
 *
 *  @param  prBlkData       AES block structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_AES_MoveBlockData(const MTDMX_DRM_AES_BLKDATA_T *prBlkData);

//-----------------------------------------------------------------------------
/*
 *  Setup AES decryption engine - extension version
 *
 *  @param  u1Idx           engine index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prDRM           AES structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_AES_AllocEngine(UINT8 *pu1Idx);

//-----------------------------------------------------------------------------
/*
 *  Setup AES decryption engine - extension version
 *
 *  @param  u1Idx           engine index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prDRM           AES structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_AES_FreeEngine(UINT8 u1Idx);

//-----------------------------------------------------------------------------
/*
 *  Setup AES decryption engine - extension version
 *
 *  @param  u1Idx           engine index (0 - 31)
 *  @param  u4Flags         Function flags
 *  @param  prDRM           AES structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_AES_SetDecryptInfoEx(UINT8 u1Idx, UINT32 u4Flags, const MTDMX_DRM_AES_T *prDRM);

//-----------------------------------------------------------------------------
/*
 *  Use AES decryption engine to decrypt data - extension version
 *
 *  @param  u1Idx           engine index (0 - 31)
 *  @param  prBlkData       AES block structure
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_AES_MoveBlockDataEx(UINT8 u1Idx, const MTDMX_DRM_AES_BLKDATA_T *prBlkData);
//-----------------------------------------------------------------------------
/*
 *  Send GCPU command
 *
 *  @param  u4Handle        GCPU Handle
 *  @param  u4Cmd           Command
 *  @param  pvParam         Parameter pointer
 *  @param  u4ParamSize     Parameter size
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GCPU_Cmd(UINT32 u4Handle, UINT32 u4Cmd, void *pvParam, UINT32 u4ParamSize, BOOL fgIommu);

//-----------------------------------------------------------------------------
/*
 *  Query DRM buffer
 *
 *  @param  pVA             Virtual address of DRM buffer
 *  @param  pPA             Physical address of DRM buffer
 *  @param  pu4Size         Size of DRM buffer
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_GetDRMBuffer(UINT32 *pu4VA, UINT32 *pu4PA, UINT32 *pu4Size);

//-----------------------------------------------------------------------------
/*
 *  Release DRM buffer
 *
 *  @param  pVA             Virtual address of DRM buffer
 *  @param  pu4Size         Size of DRM buffer
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_ReleaseDRMBuffer(UINT32 u4VA, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Set PID input source type
 *
 *  @param  u1Pidx           PID index value
 *  @param  eType            PID input source type
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetPidInputSource(UINT8 u1Pidx, MTDMX_PID_INSRC_T eType);

//-----------------------------------------------------------------------------
/** MTDMX_SetMulti2Config
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetMulti2Config(BOOL fgOfbFreeRun, UINT16 u2Iteration);

//-----------------------------------------------------------------------------
/** MTDMX_SetMulti2Key
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDMX_SetMulti2Key(UINT8 u1Index, BOOL fgEven, UINT32 au4Key[2]);

//-----------------------------------------------------------------------------
/** MTDMX_SetMulti2CbcIV
 */
//-----------------------------------------------------------------------------
EXTERN 	MT_RESULT_T MTDMX_SetMulti2CbcIV(UINT32 u4IvHi, UINT32 u4IvLo);


//-----------------------------------------------------------------------------
/** MTDMX_SetMulti2OfbIV
 */
//-----------------------------------------------------------------------------
EXTERN 	MT_RESULT_T MTDMX_SetMulti2OfbIV(UINT32 u4IvHi, UINT32 u4IvLo);

//-----------------------------------------------------------------------------
/** MTDMX_SetMulti2SysKey
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T 	 MTDMX_SetMulti2SysKey(UINT32 au4Key[8]);


//-----------------------------------------------------------------------------
/** MTDRM_BIM_GetDeviceData
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDRM_BIM_GetDeviceData(UINT8 *pu1Buf, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** MTDRM_BIM_GetDmKey
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDRM_BIM_GetDmKey(UINT8 *pu1Buf, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** MTDRM_BIM_GetULPK
 */
//-----------------------------------------------------------------------------
EXTERN MT_RESULT_T MTDRM_BIM_GetULPK(UINT8 *pu1Buf, UINT32 u4Size);
EXTERN MT_RESULT_T MTDMX_SetDVBKey(UINT8 u1Index, BOOL fgEven, UINT32 au4Key[2]);

//-----------------------------------------------------------------------------
/** MTDMX_SetPidxInstance
 */
//-----------------------------------------------------------------------------

EXTERN MT_RESULT_T MTDMX_SetPidxInstance(UINT8 u4Pidx,MTDMX_PID_INSRC_T eType);


//-----------------------------------------------------------------------------
/** MTDMX_FreePidx
 */
//-----------------------------------------------------------------------------

EXTERN MT_RESULT_T MTDMX_FreePidx(UINT8 u4Pidx);


#ifdef __cplusplus
}
#endif

#endif //MT_DMX_H

