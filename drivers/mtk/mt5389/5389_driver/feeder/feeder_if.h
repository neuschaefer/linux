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

#ifndef DRV_FEEDER_IF_H
#define DRV_FEEDER_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#if !defined(CC_UBOOT) && defined(__KERNEL__)
#include <linux/mt53xx_cb.h>
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
//#define CC_FLASH_TEST

//#ifdef FEEDER_HEADER_CACHE
#define FEEDER_CACHED_MAX_READ_SIZE (256 * 1024)
#define FEEDER_END_OF_FILE 0xffffffff
//#endif

#ifdef CC_53XX_SWDMX_V2
#define FEEDER_MULTI_INSTANCE
#ifdef CC_SKYPE_FINE_INSTANCE
#define FEEDER_FIVE_INSTANCE
#endif
#endif

#define MEDIA_SEEK_BGN             ((UINT8) 1)
#define MEDIA_SEEK_CUR             ((UINT8) 2)
#define MEDIA_SEEK_END             ((UINT8) 3)

/// Data query mode
typedef enum
{
    FEEDER_AUDIO_SOURCE = 0,    ///< Audio data
    FEEDER_VIDEO_SOURCE,        ///< Video data
    FEEDER_PROGRAM_SOURCE,      ///< Mpeg data
    FEEDER_PARSING_SOURCE,      ///< Hdr data
    FEEDER_PARSING_AUDIO,
 #ifdef FEEDER_FIVE_INSTANCE
    FEEDER_PROGRAM_SOURCE_2,      // 2nd video instance
    FEEDER_PARSING_SOURCE_2,    
    FEEDER_AUDIO_SOURCE_2,    // 2nd audio instance
    FEEDER_PARSING_AUDIO_2,    
    FEEDER_AUDIO_SOURCE_3,    // 3nd audio instance
    FEEDER_PARSING_AUDIO_3,
 #endif    
    FEEDER_SOURCE_TOTAL,         ///< Total support data type
    FEEDER_SOURCE_INVALID
} FeederSourceType;

/// Data query mode
typedef enum
{
    FEEDER_PUSH_MODE = 0,       ///< Feeder push data to Application/Receiver
    FEEDER_PULL_MODE            ///< Application/Receiver send the data request
} FeederQueryMode;

typedef enum
{
    FEEDER_TRANSMIT_OK = 0,         ///< Data transmit success
    FEEDER_TRANSMIT_FAIL,           ///< Data transmit fail
    FEEDER_TRANSMIT_BUFFER_FULL,    ///< Data transmit success & buffer full
    FEEDER_TRANSMIT_BUFFER_EMPTY    ///< Data transmit success & buffer empty
} FeederTransmitCondition;

typedef enum
{    
    FEEDER_NORMAL_MM = 0,           
    FEEDER_VIDEO_CHUNK,    
    FEEDER_AUDIO_CHUNK
} FeederMMRequestMode;

typedef enum
{
    FEEDER_SWDMX_FILESIZE,
    FEEDER_SWDMX_UNDERFLOW_CB,
    FEEDER_SWDMX_READY_CB,
    FEEDER_SWDMX_SRC_TYPE,
    FEEDER_SWDMX_THRESHOLD,
    FEEDER_SWDMX_INSTANCE,
    FEEDER_SWDMX_EOS,
    FEEDER_SWDMX_STATUS_CB,
    FEEDER_MM_USE,
    FEEDER_SWDMX_CACHESIZE    
} FeederSetInfoType;

typedef enum
{
    FEEDER_SRC_TYPE_UNKNOWN = 0,
    FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE,
    FEEDER_SRC_TYPE_NETWORK_DLNA,
    FEEDER_SRC_TYPE_NETWORK_NETFLIX,
    FEEDER_SRC_TYPE_MAX
}FeederSourceType_T;

/// Feeder data transmit condition
typedef struct _FEEDER_TRANSMIT_COND_T
{
    INT32  i4TransmitResult;    ///< Transmit result
    UINT32 u4TransmitSize;      ///< Size of transmitted data 
    UINT32 u4BufAddr;           ///< The start address of the transmitted data
#ifdef CC_FLASH_TEST
    FeederMMRequestMode eMode;
    BOOL fgFirst;
#endif
} FEEDER_TRANSMIT_COND;

/// Feeder buffer information
typedef struct _FEEDER_BUF_INFO_T
{
    UINT32  u4StartAddr;
    UINT32  u4EndAddr;
    UINT32  u4ReadAddr;
    UINT32  u4WriteAddr;
    UINT32  u4LastReadAddr;
    UINT32  u4LastReadSize;
    UINT32  u4FreeSize;
    UINT32  u4BytesInBuf;
    UINT32  u4Size;
	UINT32  u4UnAckNum;
    BOOL    fgRingBuf;
} FEEDER_BUF_INFO_T;

/// Feeder file information
typedef struct _FEEDER_CACHE_INFO_T
{
    UINT32  i8CurrentPhyAddr;
    UINT32  i8CachedStartPhyAddr;
    UINT32  i8CachedEndPhyAddr;
} FEEDER_CACHE_INFO_T;


// IBC information
typedef enum
{
    FEEDER_IBC_NONE = 0,
    FEEDER_IBC_DMX_VUDU_KEY,
    FEEDER_IBC_MAX
} FeederIBCType;


typedef struct
{    
    FeederSourceType eDataType;
    UINT32 u4Id;
    UINT32 u4ReadSize;
    UINT32 u4WriteAddr;
    UINT32 u4AlignOffset;
    UINT64 u8FilePos;
    BOOL   fgPartial;
#ifdef CC_FLASH_TEST
    FeederMMRequestMode eMode;    
    UINT64 u8Pts;
    BOOL fgFirst;
#endif     
    BOOL fgEof;    
    FeederIBCType eFeederIBC;
}FEEDER_REQ_DATA_T;

/// Callbalck function if Feeder retrieves the data
#ifdef FEEDER_MULTI_INSTANCE
typedef UINT32 (*FEEDER_QUERY_CALLBACK)(UINT8 u1AttachedSrcId,
                                        FeederSourceType eSrcType,
                                        FEEDER_BUF_INFO_T *pQryBufInfo,
                                        FEEDER_REQ_DATA_T *pReqData);
#else
typedef UINT32 (*FEEDER_QUERY_CALLBACK)(FeederSourceType eSrcType,
                                             FEEDER_BUF_INFO_T *pQryBufInfo,
                                             FEEDER_REQ_DATA_T *pReqData);
#endif

/// Callbalck function if Feeder consumes the data
typedef UINT32 (*FEEDER_CONSUME_CALLBACK)(FeederSourceType eSrcType, 
                                             FEEDER_BUF_INFO_T *pQryBufInfo,
                                             UINT32 u4NewRPtr);

/// Callbalck function if Feeder retrieves the data
typedef INT32 (*FEEDER_TRANMIT_HANDLER)(FeederSourceType eSrcType,
                                        UINT32 u4QrySize,
                                         UINT64 u8Offset,
                                        FEEDER_TRANSMIT_COND *pResult,  UINT32 u4ReqId);

/// Callbalck function if Feeder buffer is overflow
typedef VOID (*FEEDER_OVERFLOW_CALLBACK)(FeederSourceType eSrcType);

/// Callbalck function if Feeder buffer is underflow
typedef VOID (*FEEDER_UNDERFLOW_CALLBACK)(FeederSourceType eSrcType);


typedef enum
{
    FEEDER_NOTIFY_NEEDDATA,   // notification of copy buffer
    FEEDER_NOTIFY_ERROR,      // notification of play error
    FEEDER_NOTIFY_FILEEND,     // notification of file end
    FEEDER_NOTIFY_CAPTION,    // notification of caption
    FEEDER_NOTIFY_FILEBEGIN,                ///< notification of fileplay begin
    FEEDER_NOTIFY_DATA_REQ_DONE,            ///for async reading
    FEEDER_NOTIFY_BUFFULL,
    FEEDER_NOTIFY_ELAPSEDTIME, // notification of timer
    FEEDER_NOTIFY_TOTALTIME,   // notification of updated total time
    FEEDER_NOTIFY_PROFILE_CHANGE,
    FEEDER_NOTIFY_SEEK_END,     // notification of seek end
    FEEDER_NOTIFY_ABORT_DONE,     // notification of abort end
    FEEDER_NOTIFY_TIME_OUT,     // notification of time out
    FEEDER_NOTIFY_ELAPSEDBYTE,     // notification of comsumed byte
    FEEDER_NOTIFY_DRAWSUBTITLE,
    FEEDER_NOTIFY_DRAW_EXT_SUBTITLE,
    FEEDER_NOTIFY_PACKET_SIZE,    //notification of netflix packet size
    FEEDER_NOTIFY_DATA_EXHAUSTED, //notification fo netflix data exhausted info
    FEEDER_NOTIFY_PVR_RANGE_NOTIFY,  // only correspond to user space
    FEEDER_NOTIFY_ACCESS_DIVX_DRM,
    FEEDER_NOTIFY_MAX
} FEEDER_NOTIFY_COND;

/// Set the callback function of the Feeder module
typedef BOOL (*FEEDER_NOTIFY)(FeederSourceType eSrcType,
                              FEEDER_NOTIFY_COND eCondition,
                              UINT32 u4Param);

/// Feeder data transmit condition
typedef struct _FEEDER_STATUS_T
{
    UINT32 u4TimePosition ;                 ///< Current time position
	UINT32 u4speed;                         ///< Play speed
	BOOL   bReversePlay;                    ///< Play direction
} FEEDER_DECODE_STATUS;

/// Set the callback function of the Feeder module
typedef INT32 (*FEEDER_GET_STATUS)(FEEDER_DECODE_STATUS *prStatus);

/// Feeder data qeury setting
typedef struct _FEEDER_TRANSMIT_T
{
    FeederQueryMode         eTargetMode;    ///< Tranmit mode of the receiver
    UINT32                  u4BufferSize;   ///< Feeder buffer size
    UINT32                  u4Tag;          ///< Query data type
    FEEDER_TRANMIT_HANDLER  pfnTransHandle; ///< Callback to transmit data
    FEEDER_NOTIFY           pfnNotify;      ///< Notify function pointer
    FEEDER_GET_STATUS       pfnGetStatus;   ///< Get the status of decode target
} FEEDER_TRANSMIT;

/// Set the data transmit parameters of the receiver
typedef BOOL (*FEEDER_SET_TRANSMIT)(FEEDER_TRANSMIT *prTransmit);

#if !defined(__KERNEL__) || defined(CC_UBOOT)
/// Information of a callback function
typedef struct
{
    UINT8 u1SwdmxId;
    UINT8 u1PlaymgrId;
    UINT32 u4SrcType;                       ///< Callback source
    UINT32 u4Condition;                     ///< Callback condition
    UINT32 u4Param;                         ///< Callback paramter
} MM_NOTIFY_INFO_T;

typedef struct _MM_FILE_OPS_T
{
    UINT64  u8Offset;
    UINT32  u4QueryID;
    UINT8   u1Whence;
} MM_FILE_OPS_T;

typedef struct
{
    MM_NOTIFY_INFO_T rMMNotify;
    MM_FILE_OPS_T rFileInfo;
    UINT32 u4QuerySize; 
    UINT32 u4WritePtr;
} MM_CALLBACK_INFO_T;

#endif

typedef void (*FEEDER_CALBACK)(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param);
#ifdef FEEDER_MULTI_INSTANCE
typedef INT64 (*FEEDER_SEEKFILEPOS_HANDLER)(UINT8 u1AttachedSrcId, FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo);
#else
typedef INT64 (*FEEDER_SEEKFILEPOS_HANDLER)(FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo);
#endif

/// Application register its data transmit callback to the feeder
typedef struct _FEEDER_QUERY_DATA_T
{
    UINT8 u1PlaymgrAttachedId;
    UINT8 u1SwdmxAttachedId;
    UINT32                  u4QuerySize;    ///< Query data size
    FeederSourceType        eDataType;      ///< Query data type
    FEEDER_QUERY_CALLBACK   pfnSource;      ///< Callback function
    FEEDER_SEEKFILEPOS_HANDLER pfnSeekPos;    ///< Callback function
    FEEDER_CONSUME_CALLBACK pfnConsume;     ///< Callback function
    FEEDER_OVERFLOW_CALLBACK pfnOverflow;   ///< Callback function, for URI mode
    FEEDER_UNDERFLOW_CALLBACK pfnUnderflow; ///< Callback function, for URI mode
} FEEDER_QUERY_DATA;




//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define FEEDER_E_OK             (INT32)(0)          ///< Success
#define FEEDER_E_FAIL           (INT32)(-1)         ///< IO error
#define FEEDER_E_INVALID_PARAM  (INT32)(-2)         ///< Get the invalid param
#define FEEDER_E_MEMORY_ALLOC   (INT32)(-3)         ///< Memory allocation fail
#define FEEDER_E_BUFFER_FULL    (INT32)(-4)         ///< Memory buffer is full
#define FEEDER_E_STOP           (INT32)(-5)         ///< Feeder stops

#define FEEDER_NFY_STATUS_READY                 0x00000000
#define FEEDER_NFY_STATUS_IGNORE_READY   0x00000001

 

#define FEEDER_MIN_BUF_SIZE             (500*1024)

#define FEEDER_RESERVED_SIZE            (64 * 1024)

#define FEEDER_PROGRAM_BUF_START        FEEDER_RESERVED_SIZE

#if defined(FEEDER_FIVE_INSTANCE)
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024) 
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (344 * 1024)

#define FEEDER_PROGRAM_SOURCE_2_BUF_START   (FEEDER_PARSING_BUF_START+FEEDER_PARSING_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_PROGRAM_SOURCE_2_BUF_SIZE    (1128*1024)
#define FEEDER_PARSING_SOURCE_2_START      (FEEDER_PROGRAM_SOURCE_2_BUF_START+FEEDER_PROGRAM_SOURCE_2_BUF_SIZE)
#define FEEDER_PARSING_SOURCE_2_SIZE       (344 * 1024)

#define FEEDER_AUDIO_BUF_START          (FEEDER_PARSING_SOURCE_2_START+FEEDER_PARSING_SOURCE_2_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (512*1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_AUDIO_BUF_START + FEEDER_AUDIO_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (344 * 1024)

/*
#define FEEDER_AUDIO_BUF_2_START          (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_2_SIZE           (512*1024)
#define FEEDER_PARSING_AUD_BUF_2_START    (FEEDER_AUDIO_BUF_2_START + FEEDER_AUDIO_BUF_2_SIZE)
#define FEEDER_PARSING_AUD_BUF_2_SIZE     (344 * 1024)

#define FEEDER_AUDIO_BUF_3_START          (FEEDER_PARSING_AUD_BUF_2_START+FEEDER_PARSING_AUD_BUF_2_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_3_SIZE           (512*1024)
#define FEEDER_PARSING_AUD_BUF_3_START    (FEEDER_AUDIO_BUF_3_START + FEEDER_AUDIO_BUF_3_SIZE)
#define FEEDER_PARSING_AUD_BUF_3_SIZE     (344 * 1024)
*/

#define FEEDER_PROGRAM_BUF_START_SKYPE	FEEDER_RESERVED_SIZE
#define FEEDER_DEFAULT_BUF_SIZE_SKYPE         (1128*1024) // 192, 188 alignment
#define FEEDER_PROGRAM_SOURCE_2_BUF_START_SKYPE   (FEEDER_PROGRAM_BUF_START_SKYPE+FEEDER_DEFAULT_BUF_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_PROGRAM_SOURCE_2_BUF_SIZE_SKYPE    (1128*1024)
#define FEEDER_AUDIO_BUF_START_SKYPE          (FEEDER_PROGRAM_SOURCE_2_BUF_START_SKYPE+FEEDER_PROGRAM_SOURCE_2_BUF_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE_SKYPE           (512*1024)
#define FEEDER_AUDIO_BUF_2_START_SKYPE          (FEEDER_AUDIO_BUF_START_SKYPE+FEEDER_AUDIO_BUF_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_2_SIZE_SKYPE           (512*1024)
#define FEEDER_AUDIO_BUF_3_START_SKYPE          (FEEDER_AUDIO_BUF_2_START_SKYPE+FEEDER_AUDIO_BUF_2_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_3_SIZE_SKYPE           (512*1024)

#elif defined(FEEDER_THREE_INSTANCE)
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024) 
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (0 * 1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (0 * 1024)
#define FEEDER_THIRD_STREAM_BUF_START   (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_THIRD_STREAM_BUF_SIZE    (1128*1024)
#define FEEDER_THIRD_PARSING_START      (FEEDER_THIRD_STREAM_BUF_START+FEEDER_THIRD_STREAM_BUF_SIZE)
#define FEEDER_THIRD_PARSING_SIZE       (0 * 1024)
#define FEEDER_AUDIO_BUF_START          (FEEDER_THIRD_PARSING_START+FEEDER_THIRD_PARSING_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (512*1024)
#elif defined(CC_FBM_TWO_FBP)
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024) 
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (344 * 1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (344*1024)
#define FEEDER_AUDIO_BUF_START          (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (1128*1024)
#else
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024) 
#define FEEDER_DEFAULT_BUF_SIZE         (2256*1024)
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (512 * 1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (0)
#define FEEDER_AUDIO_BUF_START          (FEEDER_PROGRAM_BUF_START)
#define FEEDER_AUDIO_BUF_SIZE           (FEEDER_DEFAULT_BUF_SIZE)
#endif

#if 0
#ifdef CC_FBM_TWO_FBP
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#else
#define FEEDER_DEFAULT_BUF_SIZE         (2256*1024)
#endif
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#ifdef CC_FBM_TWO_FBP
#define FEEDER_PARSING_BUF_SIZE         ( 344 * 1024)
#else
#define FEEDER_PARSING_BUF_SIZE         ( 512 * 1024)
#endif
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#ifdef CC_FBM_TWO_FBP
#define FEEDER_PARSING_AUD_BUF_SIZE     (344*1024)
#else
#define FEEDER_PARSING_AUD_BUF_SIZE     (0)
#endif

#ifdef CC_FBM_TWO_FBP
#define FEEDER_AUDIO_BUF_START          (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (1128*1024)
#else
#define FEEDER_AUDIO_BUF_START          (FEEDER_PROGRAM_BUF_START)
#define FEEDER_AUDIO_BUF_SIZE           (FEEDER_DEFAULT_BUF_SIZE)
#endif
#endif

#define FEEDER_DATA_Q_SIZE 16    // Feeder message Q size (maximum number of requests to hold)


//-----------------------------------------------------------------------------
// Prototype of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Init feeder param when system boot up.
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederInit(void);

//-----------------------------------------------------------------------------
/** Allot the receiver-related hardware resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederOpen(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Release the allotted resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederClose(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Initialize the variables which are required to transmit the data to the
 *  local buffer of the receiver
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederStart(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Initialize the local buffer by NULL
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederStop(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Pause the data transmit to the audio decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederPause(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Resume the data transmit to the audio decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederResume(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Get the free size of feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval the size of the feeder buffer
 */
//-----------------------------------------------------------------------------
extern INT32 FeederGetBufferSize(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Get the information of feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prBuffInfo      Pointers of the buffer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederGetBufferInfo(FeederSourceType eDataType, 
                                 FEEDER_BUF_INFO_T *prBuffInfo);

//-----------------------------------------------------------------------------
/** Update the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4WriteSize     Write data size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederUpdateWritePointer(FeederSourceType eDataType,
                                      UINT32 u4WriteSize);
//-----------------------------------------------------------------------------
/** Reset the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4WritePointer     WritePointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSetWritePointer(FeederSourceType eDataType,UINT32 u4WritePointer);

//-----------------------------------------------------------------------------
/** Reallocate the feeder local buffer with user-specific size
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4Size          Memory buffer size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSetBufferSize(FeederSourceType eDataType, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Transmit the data inside buffer as getting the data of Application 
 *  through the pBuffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  puiBuffer       Data buffer trasmitted by application
 *  @param  u4Size          Data size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSubmitData(FeederSourceType eDataType, 
                              const UINT8 *puiBuffer, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Application set its data query function for the feeder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prSetQuery      Query setting
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederRegisterPullDataFunc(FeederSourceType eDataType, 
                                        const FEEDER_QUERY_DATA *prSetQuery);

//-----------------------------------------------------------------------------
/** Set the transmit parameters to the receiver
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  fEnable         Enable / Disable data transmit to the receiver
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSetTransmit(FeederSourceType eDataType, BOOL fEnable);

//-----------------------------------------------------------------------------
/** Wait the event if there is more buffer space free
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
extern INT32 FeederWaitFreeBufferSpace(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Wait the event if the buffer is empty
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
extern INT32 FeederWaitBufferEmpty(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Set the transmit decode target
 *
 *  @param  u4VideoFmt      video decode format
 *  @param  u4AudioFmt      audio decode format
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSetDecodeFmt(UINT32 u4VideoFmt, UINT32 u4AudioFmt);

//-----------------------------------------------------------------------------
/** Get the transmit decode target format
 *
 *  @param  pu4VideoFmt     video decode format
 *  @param  pu4AudioFmt     audio decode format
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederGetDecodeFmt(UINT32 *pu4VideoFmt, UINT32 *pu4AudioFmt);

//-----------------------------------------------------------------------------
/** Get the state of the target decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prStatus        Decoder status
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederGetDecodeStatus(FeederSourceType eDataType, 
                                   FEEDER_DECODE_STATUS *prStatus);

//#ifndef __KERNEL__
//-----------------------------------------------------------------------------
/** Register the callback function of Feeder module
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eCallbackType   Context parameter
 *  @param  pfnCallback     Callback function pointer
 *  @param  u4Param         
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederInstallCallback(FeederSourceType eDataType,
                                   FEEDER_NOTIFY_COND eCallbackType,
                                   FEEDER_CALBACK pfnCallback, UINT32 u4Param);

//-----------------------------------------------------------------------------
/** Release the callback function of Feeder module
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eCallbackType   Context parameter
 *  @param  pfnCallback     Callback function pointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederUnInstallCallback(FeederSourceType eDataType,
                                     FEEDER_NOTIFY_COND eCallbackType,
                                     FEEDER_CALBACK pfnCallback);
//#endif

//-----------------------------------------------------------------------------
/** Set the request parameters to the receiver
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4QrySize       Query data size
 *  @param  u8Offset        Query offset from file head
 *  @param  pResult           Feeder data transmit condition
 *  @param  u4ReqID         Reqest ID
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSetRequest(FeederSourceType eDataType, UINT32 u4QrySize, UINT64 u8Offset,
                             FEEDER_TRANSMIT_COND *pResult, UINT32 u4ReqID);

//-----------------------------------------------------------------------------
/** notify handle of the driver
 *
 *  @param  eSrcType       Data type (audio, video..)
 *  @param  eCondition      Notify condition
 *  @param  u4Param
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
extern BOOL FeederNotifyHandle(FeederSourceType eSrcType, 
                         FEEDER_NOTIFY_COND eCondition, UINT32 u4Param);

#if 0
//-----------------------------------------------------------------------------
/** Set current application to pull mode
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eQueryMode    App query mode
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
#endif
extern VOID FeederSetOffset(FeederSourceType eDataType, UINT64 u8Offset, UINT8 u1Whence, UINT32 u4ReqID);
extern VOID FeederSetRing(FeederSourceType eDataType, BOOL fgRing);
extern VOID FeederSetAppQueryMode(FeederSourceType eDataType, FeederQueryMode eQueryMode);
extern INT32 FeederUpdateReadPointer(FeederSourceType eDataType, UINT32 u4ReadAddr);
extern UINT32 FeederReadSomeBytes(FeederSourceType eDataType, UINT32 u4Size, UINT32 *pu4ReadSize);
extern INT32 FeederCopyBytesLE(VOID* pvFeederObj, VOID* pv_buf, SIZE_T z_buf_leng, SIZE_T z_copy_size, UINT32* pui4_size);
extern INT32 FeederInput4BytesLE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput3BytesLE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput2BytesLE(VOID* pvFeederObj, UINT16* pui2_data);
extern INT32 FeederInput1Bytes(VOID* pvFeederObj, UINT8*  pui1_data);
extern INT32 FeederSetPos(VOID* pvFeederObj, INT64 i8Offset, UINT8 u1Whence);
extern INT32 FeederGetPos(VOID* pvFeederObj, UINT64* pui8_cur_pos);
extern INT32 FeederCopyBytesBE(VOID* pvFeederObj, VOID* pv_buf, 
                                        SIZE_T z_buf_leng, SIZE_T z_copy_size, UINT32* pui4_size);
extern INT32 FeederInput4BytesBE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput3BytesBE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput2BytesBE(VOID* pvFeederObj, UINT16* pui2_data);

#ifdef FEEDER_MULTI_INSTANCE
extern VOID FeederSetBagtInfo(FeederSourceType eDataType, UINT32 u4Align, UINT32 u4BlockSize);
extern VOID FeederSetUnderFlowThreshold(FeederSourceType eDataType, UINT32 u4Threshold);
extern VOID FeederSetReadyThreshold(FeederSourceType eDataType, UINT32 u4Threshold);
extern VOID FeederSetFlush(FeederSourceType eDataType);
extern VOID FeederResetFlushId(FeederSourceType eDataType);
extern VOID FeederSetInfo(FeederSourceType eDataType, FeederSetInfoType eType,UINT32 u4Para);
extern VOID FeederIgnoreReady(FeederSourceType eDataType);
extern UINT32 FeederGetAlignSize(FeederSourceType eDataType);
#else
extern VOID FeederSetBagtInfo(UINT32 u4Align, UINT32 u4BlockSize);
extern VOID FeederSetUnderFlowThreshold(UINT32 u4Threshold);
extern VOID FeederSetReadyThreshold(UINT32 u4Threshold);
extern VOID FeederSetFlush(VOID);
extern VOID FeederSetInfo(FeederSetInfoType eType,UINT32 u4Para);
extern VOID FeederIgnoreReady(VOID);
extern UINT32 FeederGetAlignSize(void);
#endif
extern BOOL FeederIsReady(FeederSourceType eDataType);
extern BOOL FeederIsUnderFlow(FeederSourceType eDataType);
extern INT32 FeederSetMMP(FeederSourceType eDataType);
extern INT32 FeederSetDataDelay(FeederSourceType eDataType,UINT32 u4DelayTime);
extern INT32 FeederSetUriMode(FeederSourceType eType, UINT32 u4BufSize, UINT32 u4KeepBufThres, UINT32 u4RebufThres);
extern INT32 FeederGetBufferFullness(FeederSourceType eType, UINT32 *pu4Len, UINT32 *pu4Percent);
extern INT32 FeederInitPreLoad(FeederSourceType eType, UINT32 *pu4Addr, UINT32 *pu4Size);
extern INT32 FeederSetPreLoadDone(FeederSourceType eType, UINT32 u4WriteSize);
extern INT32 FeederSetCacheSize(FeederSourceType eDataType, UINT32 u4Size);
extern UINT32 FeederCLIGetCacheSize(FeederSourceType eDataType);

extern VOID FeederSetSkypeSourceFlag(BOOL isSkype);
extern BOOL FeederGetSkypeSourceFlag(void);
#endif  // DRV_FEEDER_IF_H

