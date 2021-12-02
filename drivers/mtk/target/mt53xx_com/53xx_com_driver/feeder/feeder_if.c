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
 * Copyright (c) 2004, CrystalMedia Technology, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * 
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_mm_common.h"
#include "feeder.h"

#ifdef __KERNEL__
#define FEEDER_HEADER_CACHE
#endif
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static BOOL _fgFeederInit[FEEDER_SOURCE_INVALID] =
#ifdef FEEDER_FIVE_INSTANCE
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
#else
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
#endif
static UINT32 _u4FeederVideoFmt = 0;
static UINT32 _u4FeederAudioFmt = 0;
static BOOL _fgFeederInited=FALSE;
#ifdef FEEDER_HEADER_CACHE
static FEEDER_CACHE_INFO_T *pgrFeederCacheInfo[FEEDER_SOURCE_INVALID+1] =
#ifdef FEEDER_FIVE_INSTANCE
{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
#else
{NULL, NULL, NULL, NULL, NULL, NULL, NULL};
#endif
static UINT32 _u4FeederCacheSize[FEEDER_SOURCE_INVALID+1] =
#ifdef FEEDER_FIVE_INSTANCE
{FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE,
FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE,
FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE,
FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE,
FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE,
FEEDER_CACHED_MAX_READ_SIZE,FEEDER_CACHED_MAX_READ_SIZE,
FEEDER_CACHED_MAX_READ_SIZE};
#else
{FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE,FEEDER_CACHED_MAX_READ_SIZE, FEEDER_CACHED_MAX_READ_SIZE};
#endif
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

INT32 FeederInit()
{
  if(_fgFeederInited)
  {
     return FEEDER_E_OK;
  }

  _fgFeederInited=TRUE;
  x_memset(_fgFeederInit,0,sizeof(BOOL)*FEEDER_SOURCE_INVALID);
  #ifdef FEEDER_HEADER_CACHE
  x_memset(pgrFeederCacheInfo,0,sizeof(FEEDER_CACHE_INFO_T *)*FEEDER_SOURCE_TOTAL);
  #endif
  return _FeederInit();
}

//-----------------------------------------------------------------------------
/** Allot the receiver-related hardware resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederOpen(FeederSourceType eDataType)
{
    INT32 i4Result = FEEDER_E_OK;
    
    if (!_fgFeederInit[eDataType])
    {
        i4Result = _FeederOpen(eDataType);
        
        if (i4Result == FEEDER_E_OK)
        {
            _fgFeederInit[eDataType] = TRUE;
        }
    }
    
#ifdef FEEDER_HEADER_CACHE
    if (pgrFeederCacheInfo[eDataType] == NULL)
    {
        pgrFeederCacheInfo[eDataType] = x_mem_alloc(sizeof(FEEDER_CACHE_INFO_T));
        if (pgrFeederCacheInfo[eDataType] == NULL)
        {
            LOG(5, "FeederOpen: Memory allocation failed (pgrFeederCacheInfo)\n ");
            return (FEEDER_E_MEMORY_ALLOC);
        }
    }        
    
    x_memset(pgrFeederCacheInfo[eDataType], 0, sizeof(FEEDER_CACHE_INFO_T));
#endif
    
    return (i4Result);
}

//-----------------------------------------------------------------------------
/** Release the allotted resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederClose(FeederSourceType eDataType)
{
    if (_fgFeederInit[eDataType])
    {
        _fgFeederInit[eDataType] = FALSE;
    }

    return (_FeederClose(eDataType));
}

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
INT32 FeederStart(FeederSourceType eDataType)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    if (_FeederStart(eDataType) == FEEDER_E_OK)
    {
        if (_FeederSetTransmit(eDataType, TRUE) == FEEDER_E_OK)
        {
            return (FEEDER_E_OK);
        }

        // Set transmit paramter of the receiver failed
        UNUSED(_FeederStop(eDataType));
    }
    
    return (FEEDER_E_FAIL);
}

//-----------------------------------------------------------------------------
/** Initialize the local buffer by NULL
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederStop(FeederSourceType eDataType)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }

    UNUSED(_FeederSetTransmit(eDataType, FALSE));
    return (_FeederStop(eDataType));
}

//-----------------------------------------------------------------------------
/** Pause the data transmit to the audio decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederPause(FeederSourceType eDataType)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }

    return (_FeederPause(eDataType));
}

//-----------------------------------------------------------------------------
/** Resume the data transmit to the audio decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederResume(FeederSourceType eDataType)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }

    return (_FeederResume(eDataType));
}

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
INT32 FeederWaitFreeBufferSpace(FeederSourceType eDataType)
{
    return (_FeederWaitFreeBufferSpace(eDataType));
}

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
INT32 FeederWaitBufferEmpty(FeederSourceType eDataType)
{
    return (_FeederWaitBufferEmpty(eDataType));
}

//-----------------------------------------------------------------------------
/** Get the size of feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval the size of the feeder buffer
 */
//-----------------------------------------------------------------------------
INT32 FeederGetBufferSize(FeederSourceType eDataType)
{
    INT32 i4Size = 0;
    FEEDER_BUF_INFO_T rBufferInfo;
    
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    x_memset((void *)&rBufferInfo, 0, sizeof(FEEDER_BUF_INFO_T));
    if (_FeederGetBufferInfo(eDataType, &rBufferInfo) == FEEDER_E_OK)
    {
        i4Size = (INT32)rBufferInfo.u4FreeSize;
    }
    
    return (i4Size);
}

//-----------------------------------------------------------------------------
/** set data delay time to monitor network application
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederSetDataDelay(FeederSourceType eDataType,UINT32 u4DelayTime)
{
    return _FeederSetDataDelay(eDataType,u4DelayTime);
}

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
INT32 FeederGetBufferInfo(FeederSourceType eDataType, 
                          FEEDER_BUF_INFO_T *prBuffInfo)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    if (prBuffInfo == NULL)
    {
        LOG(3, "NULL pointer structure of the feeder buffer \n");
        return (FEEDER_E_FAIL);
    }
    
    x_memset((void *)prBuffInfo, 0, sizeof(FEEDER_BUF_INFO_T));
    if (_FeederGetBufferInfo(eDataType, prBuffInfo) == FEEDER_E_OK)
    {
        return (FEEDER_E_OK);
    }
    
    return (FEEDER_E_FAIL);
}

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
INT32 FeederUpdateWritePointer(FeederSourceType eDataType, UINT32 u4WriteSize)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    if (u4WriteSize == 0)
    {
        LOG(7, "Zero byte to be read\n");
        return (FEEDER_E_OK);
    }
    return (_FeederUpdateWritePointer(eDataType, u4WriteSize));
}

//-----------------------------------------------------------------------------
/** Reset write pointer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4WritePointer     WritePointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------

INT32 FeederSetWritePointer(FeederSourceType eDataType, UINT32 u4WritePointer)
{
    return _FeederSetWritePointer(eDataType,u4WritePointer);
}

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
INT32 FeederSetBufferSize(FeederSourceType eDataType, UINT32 u4Size)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    if (u4Size == 0)
    {
        LOG(7, "Invalid buffer size %d \n", u4Size);
        return (FEEDER_E_FAIL);
    }
    
    return (_FeederSetBufferSize(eDataType, u4Size));
}

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
INT32 FeederSubmitData(FeederSourceType eDataType, 
                       const UINT8 *puiBuffer, UINT32 u4Size)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    if (puiBuffer == NULL)
    {
        LOG(3, "NULL data buffer pointer setting \n");
        return (FEEDER_E_FAIL);
    }
    
    if (u4Size == 0)
    {
        LOG(7, "Zero byte to be transmit\n");
        return (FEEDER_E_OK);
    }
    
    return (_FeederSubmitData(eDataType, puiBuffer, u4Size));
}

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
INT32 FeederRegisterPullDataFunc(FeederSourceType eDataType, 
                                 const FEEDER_QUERY_DATA *prSetQuery)
{
    if (prSetQuery == NULL)
    {
        LOG(7, "NULL data query parameter setting \n");
        return (FEEDER_E_FAIL);
    }
    
    return (_FeederRegisterPullDataFunc(eDataType, prSetQuery));
}

//-----------------------------------------------------------------------------
/** Set the transmit parameters to the receiver
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederSetTransmit(FeederSourceType eDataType, BOOL fEnable)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }

    return (_FeederSetTransmit(eDataType, fEnable));
}


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
INT32 FeederSetRequest(FeederSourceType eDataType, UINT32 u4QrySize, UINT64 u8Offset,
                             FEEDER_TRANSMIT_COND *pResult, UINT32 u4ReqID)
{
    LOG(7,"REQ[%ld] Ofst=%X, Sz=%X\n", u4ReqID, (UINT32)(u8Offset&0xffffffff), u4QrySize);
    return (_FeederTransmitHandler(eDataType, u4QrySize, u8Offset, pResult, u4ReqID));
}


//-----------------------------------------------------------------------------
/** Set the position of feeder request
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u8Offset        Offset bytes
 *  @param  u1Whence        offset base
 *  @param  u4ReqID         Reqest ID
 */
VOID FeederSetOffset(FeederSourceType eDataType, UINT64 u8Offset, UINT8 u1Whence, UINT32 u4ReqID)
{
    _FeederSetOffset(eDataType, u8Offset, u1Whence, u4ReqID);
}


//-----------------------------------------------------------------------------
/** driver notify handler
 *
 *  @param  eSrcType       Data type (audio, video..)
 *  @param  eCondition       Query data size
 *  @param  u4Param
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
extern BOOL FeederNotifyHandle(FeederSourceType eSrcType, 
                         FEEDER_NOTIFY_COND eCondition, UINT32 u4Param)
{
    return (_FeederNotifyHandle(eSrcType, eCondition, u4Param));
}

//-----------------------------------------------------------------------------
/** Set current application to pull mode
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
VOID FeederSetAppQueryMode(FeederSourceType eDataType, FeederQueryMode eQueryMode)
{
    _FeederSetAppQueryMode(eDataType, eQueryMode);
}


//-----------------------------------------------------------------------------
/** Set the transmit decode target format
 *
 *  @param  u4VideoFmt      video decode format
 *  @param  u4AudioFmt      audio decode format
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 FeederSetDecodeFmt(UINT32 u4VideoFmt, UINT32 u4AudioFmt)
{
    _u4FeederVideoFmt = u4VideoFmt;
    _u4FeederAudioFmt = u4AudioFmt;
    return (FEEDER_E_OK);
}

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
INT32 FeederGetDecodeFmt(UINT32 *pu4VideoFmt, UINT32 *pu4AudioFmt)
{
    if ((pu4VideoFmt == NULL) || (pu4AudioFmt == NULL))
    {
        LOG(7, "FeederGetDecodeFmt: NULL data buffer pointer setting \n");
        return (FEEDER_E_FAIL);
    }
    
    *pu4VideoFmt = _u4FeederVideoFmt;
    *pu4AudioFmt = _u4FeederAudioFmt;
    return (FEEDER_E_OK);
}

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
INT32 FeederGetDecodeStatus(FeederSourceType eDataType, 
                            FEEDER_DECODE_STATUS *prStatus)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "FeederGetDecodeStatus: Invalid source type \n");
        return (FEEDER_E_FAIL);
    }

    if (prStatus == NULL)
    {
        LOG(7, "FeederGetDecodeStatus: NULL data buffer pointer setting \n");
        return (FEEDER_E_FAIL);
    }
    
    return (_FeederGetDecodeStatus(eDataType, prStatus));
}

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
                                   FEEDER_CALBACK pfnCallback, UINT32 u4Param)
{
    return ( _FeederInstallCallback(eDataType, eCallbackType, pfnCallback, u4Param));
}


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
                                     FEEDER_CALBACK pfnCallback)
{
    return ( _FeederUnInstallCallback(eDataType, eCallbackType, pfnCallback));
}
//#endif


//-----------------------------------------------------------------------------
/** Set feeder buffer to use ring buf or single buf
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  fgRing          True: ring buf, FALSE: single buf
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
VOID FeederSetRing(FeederSourceType eDataType, BOOL fgRing)
{
    _FeederSetRing(eDataType, fgRing);
 #ifdef FEEDER_HEADER_CACHE        
    x_memset(pgrFeederCacheInfo[eDataType], 0, sizeof(FEEDER_CACHE_INFO_T));
 #endif  
}


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
INT32 FeederUpdateReadPointer(FeederSourceType eDataType, UINT32 u4ReadAddr)
{
    return _FeederUpdateReadPointer(eDataType, u4ReadAddr);
}

UINT32 FeederReadSomeBytes(FeederSourceType eDataType, UINT32 u4Size, UINT32 *pu4ReadSize)
{
    FEEDER_BUF_INFO_T rFeederBufInfo;
    FEEDER_TRANSMIT_COND rResult;
    UINT32 u4ReadOffset = 0;
    FeederGetBufferInfo(eDataType, &rFeederBufInfo);

#ifdef FEEDER_HEADER_CACHE
    if ( (pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr != FEEDER_END_OF_FILE )
     && (pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr >= pgrFeederCacheInfo[eDataType]->i8CachedStartPhyAddr) 
     && (pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr + u4Size <= pgrFeederCacheInfo[eDataType]->i8CachedEndPhyAddr) )
    {
        u4ReadOffset = (UINT32)(pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr - pgrFeederCacheInfo[eDataType]->i8CachedStartPhyAddr);
    }
    else
    {
		INT32 i4Ret=FEEDER_E_OK;
        FeederSetOffset(eDataType, (UINT64)pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr, MEDIA_SEEK_BGN, 0);
        i4Ret = FeederSetRequest(eDataType, _u4FeederCacheSize[eDataType], 0, &rResult, 0);
        if(i4Ret==FEEDER_E_FAIL)
        {
			return 0;
        }
//#if defined(CC_MT5395) || defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5396)
		//HalFlushInvalidateDCacheMultipleLine(rFeederBufInfo.u4StartAddr,rResult.u4TransmitSize);
//#endif	
        pgrFeederCacheInfo[eDataType]->i8CachedStartPhyAddr = pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr;
        pgrFeederCacheInfo[eDataType]->i8CachedEndPhyAddr =  \
	    pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr + rResult.u4TransmitSize;  
		u4Size=MIN(u4Size,rResult.u4TransmitSize);
    }
    pgrFeederCacheInfo[eDataType]->i8CurrentPhyAddr += u4Size;
    *pu4ReadSize = u4Size;        
#else
    FeederSetRequest(eDataType, u4Size, 0, &rResult, 0);
    *pu4ReadSize = rResult.u4TransmitSize;
#endif    

    return (rFeederBufInfo.u4StartAddr + u4ReadOffset);
}


INT32 FeederCopyBytesLE(VOID* pvFeederObj, VOID* pv_buf, SIZE_T z_buf_leng, SIZE_T z_copy_size, UINT32* pui4_size)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr,u4WritePtr;
    UINT32 u4ReadSize=0,u4TotalRead=0,u4ReqSize=0;
    FeederSourceType eSourceType =(FeederSourceType)prMediaFeeder->h_feeder;
	INT32 i4Ret=MFDRR_OK;
#ifdef FEEDER_HEADER_CACHE
    UINT32 u4CachedSize=0;
#else
    FEEDER_BUF_INFO_T rFeederBufInfo;
#endif

    if (z_buf_leng < z_copy_size)
    {
        z_copy_size = z_buf_leng;
    }
	u4WritePtr=(UINT32)pv_buf;
	while(z_copy_size>0)
	{
#ifdef FEEDER_HEADER_CACHE
       u4CachedSize=pgrFeederCacheInfo[eSourceType]->i8CachedEndPhyAddr-pgrFeederCacheInfo[eSourceType]->i8CurrentPhyAddr;
       if(u4CachedSize)
       {
          u4ReqSize= MIN(u4CachedSize,z_copy_size);
       }
	   else
	   {
          u4ReqSize= MIN(_u4FeederCacheSize[eSourceType],z_copy_size);
	   }
#else
	   FeederGetBufferInfo(eSourceType, &rFeederBufInfo);
       u4ReqSize=MIN(z_copy_size,rFeederBufInfo.u4Size);
#endif
	   u4ReadPtr = FeederReadSomeBytes(eSourceType, u4ReqSize, &u4ReadSize);
       if(!u4ReadPtr)
       {
         i4Ret=MFDRR_FAIL;
		 break;
       }
      // copy_to_user((void __user *)u4WritePtr, (VOID *)u4ReadPtr,u4ReadSize); ?????
	   x_memcpy((VOID *)u4WritePtr, (VOID *)u4ReadPtr, u4ReadSize);
	   u4WritePtr+=u4ReadSize;
	   u4TotalRead+=u4ReadSize;
	   z_copy_size-=MIN(u4ReadSize,z_copy_size);
	   if(u4ReqSize>u4ReadSize) // maybe fileend
	   {
          break;
	   }
	}

    *pui4_size = u4TotalRead;
    return i4Ret;
}


INT32 FeederInput4BytesLE(VOID* pvFeederObj, UINT32* pui4_data)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr;
    UINT32 u4ReadSize;
    
    u4ReadPtr = FeederReadSomeBytes((FeederSourceType)prMediaFeeder->h_feeder, 4, &u4ReadSize);
	if(!u4ReadPtr)
	{
       return MFDRR_FAIL;
	}

    *pui4_data = *((UINT32 *)u4ReadPtr); 


    return MFDRR_OK;
}


INT32 FeederInput3BytesLE(VOID* pvFeederObj, UINT32* pui4_data)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr;
    UINT32 u4ReadSize;
    
    u4ReadPtr = FeederReadSomeBytes((FeederSourceType)prMediaFeeder->h_feeder, 3, &u4ReadSize);
	if(!u4ReadPtr)
	{
       return MFDRR_FAIL;
	}
    *pui4_data = *((UINT32 *)u4ReadPtr) & 0x00FFFFFF;  

    return MFDRR_OK;
}


INT32 FeederInput2BytesLE(VOID* pvFeederObj, UINT16* pui2_data)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr;
    UINT32 u4ReadSize;
    
    u4ReadPtr = FeederReadSomeBytes((FeederSourceType)prMediaFeeder->h_feeder, 2, &u4ReadSize);
	if(!u4ReadPtr)
	{
       return MFDRR_FAIL;
	}

    *pui2_data = *((UINT16 *)u4ReadPtr);  

    return MFDRR_OK;
}


INT32  FeederInput1Bytes(VOID* pvFeederObj, UINT8* pui1_data)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr;
    UINT32 u4ReadSize;
    
    u4ReadPtr = FeederReadSomeBytes((FeederSourceType)prMediaFeeder->h_feeder, 1, &u4ReadSize);
	if(!u4ReadPtr)
	{
       return MFDRR_FAIL;
	}

    *pui1_data = *((UINT8 *)u4ReadPtr);    

    return MFDRR_OK;
}


INT32 FeederSetPos(VOID* pvFeederObj, INT64 i8Offset, UINT8 u1Whence)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;       

u1Whence=u1Whence&0xf; //modify for driver build
#ifdef FEEDER_HEADER_CACHE
    switch(u1Whence)
    {
        case MEDIA_SEEK_BGN:
            pgrFeederCacheInfo[(FeederSourceType)prMediaFeeder->h_feeder]->i8CurrentPhyAddr = i8Offset;
            break;
        case MEDIA_SEEK_CUR:
            pgrFeederCacheInfo[(FeederSourceType)prMediaFeeder->h_feeder]->i8CurrentPhyAddr += i8Offset;
            break;
        case MEDIA_SEEK_END:
        pgrFeederCacheInfo[(FeederSourceType)prMediaFeeder->h_feeder]->i8CurrentPhyAddr = 
            _FeederSetOffset((FeederSourceType)prMediaFeeder->h_feeder, (UINT64)i8Offset, u1Whence, 0);        
            break;
        default :
            break;
    }
#else
    FeederSetOffset((FeederSourceType)prMediaFeeder->h_feeder, (UINT64)i8Offset, u1Whence, 0);
#endif

    //return 0;  
    return MFDRR_OK;
}


INT32 FeederGetPos(VOID* pvFeederObj, UINT64* pui8_cur_pos)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;

#ifdef FEEDER_HEADER_CACHE
    *pui8_cur_pos = (UINT64)pgrFeederCacheInfo[(FeederSourceType)prMediaFeeder->h_feeder]->i8CurrentPhyAddr;
#else
    *pui8_cur_pos = (UINT64)_FeederSetOffset((FeederSourceType)prMediaFeeder->h_feeder, 0, 2, 0); 
#endif

    return MFDRR_OK;
}


static VOID FeederBELE(UINT8 *pu1SrcAddr, UINT8 *pu1DstAddr, UINT32 u4Num)
{
    INT16 i2I;

    for (i2I = (INT16)(u4Num - 1); i2I >= 0; i2I--)
    {
        pu1DstAddr[u4Num - 1 - i2I] = pu1SrcAddr[i2I];        
    }
}

INT32 FeederCopyBytesBE(VOID* pvFeederObj, VOID* pv_buf, SIZE_T z_buf_leng, SIZE_T z_copy_size, UINT32* pui4_size)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr,u4WritePtr;
    UINT32 u4ReadSize=0,u4TotalRead=0,u4ReqSize=0;
    FeederSourceType eSourceType =(FeederSourceType)prMediaFeeder->h_feeder;
	INT32 i4Ret=MFDRR_OK;
#ifdef FEEDER_HEADER_CACHE
    UINT32 u4CachedSize=0;
#else
    FEEDER_BUF_INFO_T rFeederBufInfo;
#endif
    if (z_buf_leng < z_copy_size)
    {
        z_copy_size = z_buf_leng;
    }
	u4WritePtr=(UINT32)pv_buf;
	while(z_copy_size>0)
	{
#ifdef FEEDER_HEADER_CACHE
       u4CachedSize=pgrFeederCacheInfo[eSourceType]->i8CachedEndPhyAddr-pgrFeederCacheInfo[eSourceType]->i8CurrentPhyAddr;
       if(u4CachedSize)
       {
          u4ReqSize= MIN(u4CachedSize,z_copy_size);
       }
	   else
	   {
          u4ReqSize= MIN(_u4FeederCacheSize[eSourceType],z_copy_size);
	   }
#else
	   FeederGetBufferInfo(eSourceType, &rFeederBufInfo);
       u4ReqSize=MIN(z_copy_size,rFeederBufInfo.u4Size);
#endif
	   u4ReadPtr = FeederReadSomeBytes(eSourceType, u4ReqSize, &u4ReadSize);
       if(!u4ReadPtr)
       {
         i4Ret=MFDRR_FAIL;
		 break;
       }
       // copy_to_user((void __user *)u4WritePtr, (VOID *)u4ReadPtr,u4ReadSize); ?????
	   FeederBELE((UINT8 *)u4ReadPtr, (UINT8 *)u4WritePtr, u4ReadSize);
	   u4WritePtr+=u4ReadSize;
	   u4TotalRead+=u4ReadSize;
	   z_copy_size-=MIN(u4ReadSize,z_copy_size);
	   if(u4ReqSize>u4ReadSize) // maybe fileend
	   {
          break;
	   }
	}

    *pui4_size = u4TotalRead;
    return i4Ret;
}


INT32 FeederInput4BytesBE(VOID* pvFeederObj, UINT32* pui4_data)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr;
    UINT32 u4ReadSize;
    UINT32 u4Dst;
    
    u4ReadPtr = FeederReadSomeBytes((FeederSourceType)prMediaFeeder->h_feeder, 4, &u4ReadSize);
	if(!u4ReadPtr)
	{
       return MFDRR_FAIL;
	}

    FeederBELE((UINT8 *)u4ReadPtr, (UINT8 *)&u4Dst, 4);

    *pui4_data = u4Dst; 

    //return *((INT32 *)u4ReadPtr);  
    return MFDRR_OK;
}


INT32 FeederInput3BytesBE(VOID* pvFeederObj, UINT32* pui4_data)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr;
    UINT32 u4ReadSize;
    UINT32 u4Dst;
    
    u4ReadPtr = FeederReadSomeBytes((FeederSourceType)prMediaFeeder->h_feeder, 3, &u4ReadSize);
	if(!u4ReadPtr)
	{
       return MFDRR_FAIL;
	}

    FeederBELE((UINT8 *)u4ReadPtr, (UINT8 *)&u4Dst, 3);

    *pui4_data = (u4Dst & 0x00ffffff);  
    //return (i4Dst & 0x00ffffff);  
    return MFDRR_OK;
}


INT32 FeederInput2BytesBE(VOID* pvFeederObj, UINT16* pui2_data)
{
    MEDIA_FEEDER_T *prMediaFeeder = (MEDIA_FEEDER_T *)pvFeederObj;
    UINT32 u4ReadPtr;
    UINT32 u4ReadSize;
    UINT16 u2Dst;
    
    u4ReadPtr = FeederReadSomeBytes((FeederSourceType)prMediaFeeder->h_feeder, 2, &u4ReadSize);
	if(!u4ReadPtr)
	{
       return MFDRR_FAIL;
	}

    FeederBELE((UINT8 *)u4ReadPtr, (UINT8 *)&u2Dst, 2);

    *pui2_data = u2Dst;
    //return i2Dst;   
    return MFDRR_OK;
}


#ifdef FEEDER_MULTI_INSTANCE
VOID FeederSetBagtInfo(FeederSourceType eDataType, UINT32 u4Align, UINT32 u4BlockSize)
{
    _FeederSetBagtInfo(eDataType, u4Align, u4BlockSize);
}
#else
VOID FeederSetBagtInfo(UINT32 u4Align, UINT32 u4BlockSize)
{
    _FeederSetBagtInfo(FEEDER_PROGRAM_SOURCE, u4Align, u4BlockSize);
}
#endif

#ifdef FEEDER_MULTI_INSTANCE
VOID FeederSetFlush(FeederSourceType eDataType)
{
    _FeederSetFlush(eDataType);
}

VOID FeederResetFlushId(FeederSourceType eDataType)
{
    _FeederResetFlushId(eDataType);
}
#else
VOID FeederSetFlush(VOID)
{
    _FeederSetFlush(FEEDER_PROGRAM_SOURCE);
}

VOID FeederResetFlushId(VOID)
{
    _FeederResetFlushId(FEEDER_PROGRAM_SOURCE);
}
#endif

#ifdef FEEDER_MULTI_INSTANCE
VOID FeederSetReadyThreshold(FeederSourceType eDataType,UINT32 u4Threshold)
{
    _FeederSetReadyThreshold(eDataType, u4Threshold);
}
#else
VOID FeederSetReadyThreshold(UINT32 u4Threshold)
{
    _FeederSetReadyThreshold(FEEDER_PROGRAM_SOURCE, u4Threshold);
}
#endif

#ifdef FEEDER_MULTI_INSTANCE
VOID FeederSetUnderFlowThreshold(FeederSourceType eDataType,UINT32 u4Threshold)
{
    _FeederSetUnderFlowThreshold(eDataType, u4Threshold);
}
#else
VOID FeederSetUnderFlowThreshold(UINT32 u4Threshold)
{
    _FeederSetUnderFlowThreshold(FEEDER_PROGRAM_SOURCE, u4Threshold);
}
#endif

BOOL FeederIsReady(FeederSourceType eDataType)
{
    return  _FeederIsReady(eDataType);
}
BOOL FeederIsUnderFlow(FeederSourceType eDataType)
{
    return  _FeederIsUnderFlow(eDataType);
}

#ifdef FEEDER_MULTI_INSTANCE
VOID FeederSetInfo(FeederSourceType eDataType, FeederSetInfoType eType, UINT32 u4Para)
{
    _FeederSetInfo(eDataType,eType,u4Para);
}
#else
VOID FeederSetInfo(FeederSetInfoType eType, UINT32 u4Para)
{
    _FeederSetInfo(FEEDER_PROGRAM_SOURCE, eType, u4Para);
}
#endif

INT32 FeederSetMMP(FeederSourceType eDataType)
{
    return _FeederSetMMP(eDataType);
}

#ifdef FEEDER_MULTI_INSTANCE
VOID FeederIgnoreReady(FeederSourceType eDataType)
{
    _FeederIgnoreReady(eDataType);
}
#else
VOID FeederIgnoreReady(VOID)
{
    _FeederIgnoreReady(FEEDER_PROGRAM_SOURCE);
}
#endif

#ifdef FEEDER_MULTI_INSTANCE
UINT32 FeederGetAlignSize(FeederSourceType eDataType)
{
   return _FeederGetAlignSize(eDataType);
}
#else
UINT32 FeederGetAlignSize(void)
{
   return _FeederGetAlignSize(FEEDER_PROGRAM_SOURCE);
}
#endif


INT32 FeederSetUriMode(FeederSourceType eType, UINT32 u4BufSize, UINT32 u4KeepBufThres, UINT32 u4RebufThres)
{
    return _FeederSetUriMode(eType, u4BufSize, u4KeepBufThres, u4RebufThres);
}


INT32 FeederGetBufferFullness(FeederSourceType eType, UINT32 *pu4Len, UINT32 *pu4Percent)
{
    if (eType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    if (pu4Len == NULL || pu4Percent == NULL)
    {
        LOG(3, "NULL pointer structure of the parameters\n");
        return (FEEDER_E_FAIL);
    }
    
    if (_FeederGetBufferFullness(eType, pu4Len, pu4Percent) == FEEDER_E_OK)
    {
        return (FEEDER_E_OK);
    }
    
    return (FEEDER_E_FAIL);
}


INT32 FeederInitPreLoad(FeederSourceType eType, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    if (eType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    return _FeederInitPreLoad(eType, pu4Addr, pu4Size);
}


INT32 FeederSetPreLoadDone(FeederSourceType eType, UINT32 u4WriteSize)
{
    if (eType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "Invalid source type \n");
        return (FEEDER_E_FAIL);
    }
    
    return _FeederSetPreLoadDone(eType, u4WriteSize);
}

INT32 FeederSetCacheSize(FeederSourceType eDataType, UINT32 u4Size)
{
#ifdef FEEDER_HEADER_CACHE
    if((u4Size==0)||(u4Size>FEEDER_CACHED_MAX_READ_SIZE))
    {
        LOG(7, "Invalid feeder cache size \n");
        return (FEEDER_E_FAIL);
    }
    _u4FeederCacheSize[eDataType] = u4Size;
#endif

    return FEEDER_E_OK;
}

VOID FeederSetSkypeSourceFlag(BOOL isSkype)
{
	_FeederSetSkypeSourceFlag(isSkype);
}

BOOL FeederGetSkypeSourceFlag(void)
{
	return _FeederGetSkypeSourceFlag();
}



