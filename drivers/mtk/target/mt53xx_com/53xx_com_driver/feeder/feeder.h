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

#ifndef DRV_FEEDER_H
#define DRV_FEEDER_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#define DEFINE_IS_LOG    FEEDER_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

#include "x_os.h"
#include "feeder_if.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// Driver state
typedef enum
{
    FEEDER_IDLE = 0,
    FEEDER_INIT,
    FEEDER_START,
    FEEDER_PAUSE,
    FEEDER_STOP
} FeederState;

/// Feeder thread information
typedef struct _FEEDER_PULL_THREAD_T
{
    BOOL        fIsTerminated;
    BOOL        fTerminateTask;
    HANDLE_T    hPullThread;
} FEEDER_PULL_THREAD_T;

/// Feeder information
typedef struct _FEEDER_INFO_T
{
    CRIT_STATE_T            rCritState;
    HANDLE_T                hGetDataSema;
    HANDLE_T                hDataEmptySema;
    HANDLE_T                hWatchTimer;
    BOOL                    bWaitBufferEmpty;
    BOOL                    bWaitFreeSpace;
    //BOOL                    bWaitRequest;
    BOOL                    bReceiveFileBeginNfy;
    UINT32                  u4TotalTransmitBytes;
    FeederQueryMode         eApplicationMode;
    FeederQueryMode         eReceiverMode;
    FEEDER_QUERY_DATA       rDataQuery;
    FEEDER_BUF_INFO_T       rFeedBuf;
    FEEDER_PULL_THREAD_T    rPullThread;
    FEEDER_TRANSMIT         rTransmitParam;
} FEEDER_INFO_T;


// For supporting URI mode
typedef enum _FEEDER_URI_STATE
{
    FEEDER_URI_NONE = 0,
    FEEDER_URI_NORMAL,
    FEEDER_URI_PENDING_KEEP_BUF,
    FEEDER_URI_PRE_LOADING
} FEEDER_URI_STATE;

typedef struct _FEEDER_URI_DATA_T
{
    FEEDER_URI_STATE eState;
    UINT32 u4FeederLimitedBufSz;
    UINT32 u4KeepBufThres;
    UINT32 u4RebufThres;
    UINT32 u4PreLoadSize;
} FEEDER_URI_DATA_T;


#ifdef FEEDER_MULTI_INSTANCE
typedef BOOL (*PFN_SWDMX_FEEDER_CB)(VOID *pTag);
#else
typedef BOOL (*PFN_SWDMX_FEEDER_CB)(VOID);
#endif
#ifdef FEEDER_MULTI_INSTANCE
typedef BOOL (*PFN_SWDMX_FEEDER_STATUS_CB)(VOID *pTag, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3);
#else
typedef BOOL (*PFN_SWDMX_FEEDER_STATUS_CB)(UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3);
#endif
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype of inter-file functions
//-----------------------------------------------------------------------------
VOID  _FeederQeuryDataTask(VOID* pvArg);
INT32 _FeederTransmitHandler(FeederSourceType eSrcType, UINT32 u4QrySize, UINT64 u8Offset,
                             FEEDER_TRANSMIT_COND *pResult, UINT32 u4ReqId);
BOOL _FeederNotifyHandle(FeederSourceType eSrcType, 
                         FEEDER_NOTIFY_COND eCondition, UINT32 u4Param);


//-----------------------------------------------------------------------------
// Prototype of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Allot the receiver-related hardware resources
 *  Init Feeder param when system bootup
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederInit(void);

//-----------------------------------------------------------------------------
/** Allot the receiver-related hardware resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederOpen(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Release the allotted resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederClose(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Initialize the variables which are required to transmit the data to the
 *  local buffer of the receiver
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederStart(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Initialize the local buffer by NULL
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederStop(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Pause the data transmit to the audio decoder
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederPause(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Resume the data transmit to the audio decoder
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederResume(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Get the information of feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  prBufferInfo    Memory address of the Feeder buffer information
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederGetBufferInfo(FeederSourceType eDataType, 
                                  const FEEDER_BUF_INFO_T *prBufferInfo);

//-----------------------------------------------------------------------------
/** Update the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4WriteSize     Write data size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederUpdateWritePointer(FeederSourceType eDataType,
                                       UINT32 u4WriteSize);

//-----------------------------------------------------------------------------
/** Reset the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4WritePointer     WritePointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederSetWritePointer(FeederSourceType eDataType,UINT32 u4WritePointer);

//-----------------------------------------------------------------------------
/** Reallocate the feeder local buffer with user-specific size
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4Size          Memory buffer size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederSetBufferSize(FeederSourceType eDataType, UINT32 u4Size);

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
extern INT32 _FeederSubmitData(FeederSourceType eDataType, 
                               const UINT8 *puiBuffer, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Application set its data query function for the feeder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  rSetQuery       Query setting
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederRegisterPullDataFunc(FeederSourceType eDataType, 
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
extern INT32 _FeederSetTransmit(FeederSourceType eDataType, BOOL fEnable);

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
extern INT32 _FeederGetDecodeStatus(FeederSourceType eDataType, 
                                    FEEDER_DECODE_STATUS *prStatus);

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
extern INT32 _FeederWaitFreeBufferSpace(FeederSourceType eDataType);

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
extern INT32 _FeederWaitBufferEmpty(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Update the read pointer of the feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4ReadAddr      current read pointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _FeederUpdateReadPointer(FeederSourceType eDataType, UINT32 u4ReadAddr);


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
extern INT32 _FeederInstallCallback(FeederSourceType eDataType,
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
*///-----------------------------------------------------------------------------
extern INT32 _FeederUnInstallCallback(FeederSourceType eDataType, 
                                                                      FEEDER_NOTIFY_COND eCallbackType, 
	                                                               FEEDER_CALBACK pfnCallback);
//#endif

//-----------------------------------------------------------------------------
/** Set current application to pull mode
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
extern VOID _FeederSetAppQueryMode(FeederSourceType eDataType, FeederQueryMode eQueryMode);

//-----------------------------------------------------------------------------
/** Set feeder buffer to use ring buf or single buf
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  fgRing          True: ring buf, FALSE: single buf
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
extern VOID _FeederSetRing(FeederSourceType eDataType, BOOL fgRing);
//-----------------------------------------------------------------------------
/** Set offset bytes for seeking
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u8Offset          Offset bytes
 *  @param  u4ReqID          Offset bytes
 *
 *  @retval position in file
 */
//-----------------------------------------------------------------------------
extern INT64 _FeederSetOffset(FeederSourceType eDataType, UINT64 u8Offset, 
                                    UINT8 u1Whence, UINT32 u4ReqID);

//-----------------------------------------------------------------------------
/** Set minimun block size for mw DMA
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4BlockSize     minimun block size     
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
extern VOID _FeederSetBagtInfo(FeederSourceType eDataType, UINT32 u4Align, UINT32 u4BlockSize);

//-----------------------------------------------------------------------------
/** flush feeder requests 
 * 
 *  @retval  
*///-----------------------------------------------------------------------------
extern VOID _FeederSetFlush(FeederSourceType eDataType);
extern VOID _FeederResetFlushId(FeederSourceType eDataType);
extern VOID _FeederSetReadyThreshold(FeederSourceType eDataType,UINT32 u4Threshold);
extern VOID _FeederSetUnderFlowThreshold(FeederSourceType eDataType,UINT32 u4Threshold);
extern BOOL _FeederIsReady(FeederSourceType eDataType);
extern BOOL _FeederIsUnderFlow(FeederSourceType eDataType);
extern VOID _FeederSetInfo(FeederSourceType eDataType, FeederSetInfoType eType,UINT32 u4Para);
extern VOID _FeederIgnoreReady(FeederSourceType eDataType);
extern UINT32 _FeederGetAlignSize(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** MMP mode, switch feeder buffer to feeder_mmp buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_FAIL   Fail
*///-----------------------------------------------------------------------------
extern INT32 _FeederSetMMP(FeederSourceType eDataType);
extern INT32 _FeederSetDataDelay(FeederSourceType eDataType,UINT32 u4DelayTime);
extern INT32 _FeederSetUriMode(FeederSourceType eDataType, UINT32 u4BufSize, UINT32 u4KeepBufThres, UINT32 u4RebufThres);

extern INT32 _FeederGetBufferFullness(FeederSourceType eDataType, UINT32 *pu4Len, UINT32 *pu4Percent);

extern INT32 _FeederInitPreLoad(FeederSourceType eDataType, UINT32 *pu4Addr, UINT32 *pu4Size);

extern INT32 _FeederSetPreLoadDone(FeederSourceType eDataType, UINT32 u4WriteSize);

extern VOID _FeederSetSkypeSourceFlag(BOOL bIsSkypeSrc);

extern BOOL _FeederGetSkypeSourceFlag(void);
#endif  // DRV_FEEDER_H

