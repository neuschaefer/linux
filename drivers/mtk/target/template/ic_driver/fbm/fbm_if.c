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
 * $RCSfile: fbm_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_if.c
 *  This file contains implementation of exported APIs of FBM.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "fbm_drvif.h"
#include "fbm_fb.h"
#include "fbm_pool.h"
#include "fbm_cs.h"
#include "fbm_debug.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_syslog.h"
#include "x_timer.h"
#include "x_bim.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define VERIFY_FBG(id)                                  \
    ((id >= FBG_MAX_NS) ||                              \
    (_arFbg[id].ucFbgType == FBM_FBG_TYPE_UNKNOWN) ||   \
    (_arFbg[id].ucFbgId != id))

#define VERIFY_FB(gid, id)                              \
    (id >= _arFbg[gid].ucFbNs)

#define IS_REFERENCE_FB(gid, id)                        \
    ((id == _arFbg[gid].ucFbFRef) || (id == _arFbg[gid].ucFbBRef))

#define NOT_REFERENCE_FB(gid, id)                        \
    ((id != _arFbg[gid].ucFbFRef) && (id != _arFbg[gid].ucFbBRef))

#ifdef CC_VDP_FULL_ISR

#define FBM_MUTEX_LOCK(id)                              \
    (*_arFbg[id].prState = x_crit_start())

#define FBM_MUTEX_UNLOCK(id)                            \
    (x_crit_end(*_arFbg[id].prState))

#else

#define FBM_MUTEX_LOCK(id)                              \
    (VERIFY(x_sema_lock(_arFbg[id].hMutex, X_SEMA_OPTION_WAIT) == OSR_OK))

#define FBM_MUTEX_UNLOCK(id)                            \
    (VERIFY(x_sema_unlock(_arFbg[id].hMutex) == OSR_OK))

#endif


#define FBM_MEMSET(addr, value, len)                    \
    (VERIFY(x_memset((addr), value, len) == (addr)))

#define FBM_BYTE(value)                                 \
    ((UCHAR)(value & 0xFF))


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _FbgStatus(UCHAR ucFbgId);


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

// Initiated Protection
static UCHAR _FbmInitiated = 0;

#ifndef FBM_VIRTUAL_MACHINE
// frame buffer group
static FBM_FBG_T _arFbg[FBG_MAX_NS];
#else
FBM_FBG_T _arFbg[FBG_MAX_NS];
#endif
    
static UINT32 _u4NextFbgId = 0;

static FBM_POOL_T *_prPool = NULL;

static FBM_CB_FUNC_T _arFbmCbFunc;

#ifdef CC_VDP_FULL_ISR
static CRIT_STATE_T _rState;
#endif


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _FbgStatus
 *  Print FBM status
 *  @return NONE.
 */
//-------------------------------------------------------------------------
static void _FbgStatus(UCHAR ucFbgId)
{
    UCHAR ucIdx;

    if (ucFbgId >= FBG_MAX_NS)
    {
        return;
    }

    if ((_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_UNKNOWN) ||
        (_arFbg[ucFbgId].ucFbgId != ucFbgId))
    {
        LOG(1, "FBG(%d) is NULL!\n", ucFbgId);
        return;
    }

    LOG(1, "FBG(%d) Type(%d) CM(%d) Ns(%d) D(%d) F(%d) B(%d) Pool(0x%x, 0x%x)\n", 
        _arFbg[ucFbgId].ucFbgId,
        _arFbg[ucFbgId].ucFbgType,
        _arFbg[ucFbgId].ucFbgCm,
        _arFbg[ucFbgId].ucFbNs,
        _arFbg[ucFbgId].ucFbDecode,
        _arFbg[ucFbgId].ucFbFRef,
        _arFbg[ucFbgId].ucFbBRef,
        _arFbg[ucFbgId].u4FbMemoryPool,
        _arFbg[ucFbgId].u4FbMemoryPoolSize);

    for (ucIdx = 0; ucIdx < _arFbg[ucFbgId].ucFbNs; ucIdx++)
    {
        LOG(1, "\tFB(%d) S(%d) Y(0x%x) C(0x%x)\n",
            ucIdx,
            _arFbg[ucFbgId].aucFbStatus[ucIdx],
            _arFbg[ucFbgId].au4AddrY[ucIdx],
            _arFbg[ucFbgId].au4AddrC[ucIdx]);
    }
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** FBM_Init
 *  FBM initialize
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_Init(void)
{
    if (_FbmInitiated == 0)
    {
        UINT32 u4Idx;

        _FbmInitiated = 1;

        FBM_MEMSET((void*)_arFbg, 0, (sizeof(FBM_FBG_T) * FBG_MAX_NS));               

        FBM_MEMSET((void*)(&_arFbmCbFunc), 0, sizeof(FBM_CB_FUNC_T));

        // init frame buffer group
        for (u4Idx = 0; u4Idx < FBG_MAX_NS; u4Idx++)
        {
            _arFbg[u4Idx].ucFbgType = FBM_FBG_TYPE_UNKNOWN;
            _arFbg[u4Idx].ucFbgId = FBM_FBG_ID_UNKNOWN;
            _arFbg[u4Idx].ucFbgCm = FBM_CM_UNKNOWN;

            _arFbg[u4Idx].ucStcSrc = STC_SRC_NS;
            _arFbg[u4Idx].ucAvSyncMode = AV_SYNC_MODE_NS;

            // create semaphore
            VERIFY(x_sema_create(&_arFbg[u4Idx].hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

#ifdef CC_VDP_FULL_ISR
            _arFbg[u4Idx].prState = &_rState;
#endif

            // create semaphore
            VERIFY(_FBM_csema_create(&_arFbg[u4Idx].hEmptyQSemaphore, _arFbg[u4Idx].hMutex, 0) == OSR_OK);

            // create semaphore
            VERIFY(_FBM_csema_create(&_arFbg[u4Idx].hEmptyRQSemaphore, _arFbg[u4Idx].hMutex, 0) == OSR_OK);

            // create semaphore
            VERIFY(_FBM_csema_create(&_arFbg[u4Idx].hEmptyBQSemaphore, _arFbg[u4Idx].hMutex, 0) == OSR_OK);
            

#ifdef CC_VDP_FULL_ISR
            _arFbg[u4Idx].hEmptyQSemaphore.prState = &_rState;
            _arFbg[u4Idx].hEmptyRQSemaphore.prState = &_rState;
            _arFbg[u4Idx].hEmptyBQSemaphore.prState = &_rState;
#endif
        }

        // initialize frame buffer pool
        _prPool = _FBM_PoolInit();
        ASSERT(_prPool != NULL);

        // initialize frame buffer
        _FBM_FbInit(_arFbg);

        _u4NextFbgId = 0;
    }
}


//-------------------------------------------------------------------------
/** FBM_GetFbgNs
 *  Get maximum FBG number
 *  @return NONE.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFbgNs(void)
{
    return FBG_MAX_NS;
}


//-------------------------------------------------------------------------
/** FBM_CreateGroup
 *  Create FBM Group
 *  @param  ucFbgType   FBG type.
 *  @return FBG index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CreateGroup(UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4HSize, UINT32 u4VSize)
{
    UCHAR ucFbpListNs;

    UINT32 u4FbgTypeValid;
    UINT32 u4MallocSize;

    UINT32 u4FbgId;
    UINT32 u4FbgIdx;
    UINT32 u4TargetFbgIdx;
    UINT32 u4ListIdx;
    UINT32 u4FbIdx;

    FBP_LIST_T* prFbpList;

    _FBM_GetPoolList(&prFbpList, &ucFbpListNs);

    // Check frame buffer group type
    u4FbgTypeValid = 0;
    for (u4ListIdx = 0; u4ListIdx < ucFbpListNs; u4ListIdx++)
    {
        if (ucFbgType == prFbpList[u4ListIdx].ucType)
        {
            u4FbgTypeValid = 1;
        }
    }
    if ((u4FbgTypeValid == 0) || (u4VDecFmt == FBM_VDEC_UNKNOWN))
    {
        return FBM_FBG_ID_UNKNOWN;
    }

    // Search for empty frame buffer group
    u4FbgId = FBM_FBG_ID_UNKNOWN;
    u4TargetFbgIdx = _u4NextFbgId;
    for (u4FbgIdx = 0; u4FbgIdx < FBG_MAX_NS; u4FbgIdx++)
    {
        if (_arFbg[u4TargetFbgIdx].ucFbgId == FBM_FBG_ID_UNKNOWN)
        {
            u4FbgId = u4TargetFbgIdx;

            u4FbgIdx = FBG_MAX_NS;

            if (++u4TargetFbgIdx >= FBG_MAX_NS)
            {
                u4TargetFbgIdx = 0;
            }
            _u4NextFbgId = u4TargetFbgIdx;
         }

        if (++u4TargetFbgIdx >= FBG_MAX_NS)
        {
            u4TargetFbgIdx = 0;
        }
    }
    if (u4FbgId == FBM_FBG_ID_UNKNOWN)
    {
        return FBM_FBG_ID_UNKNOWN;
    }

    _arFbg[u4FbgId].u4VDecFmt = u4VDecFmt;
    
    // Clear Flag
    _arFbg[u4FbgId].u4FbgFlag = 0;

    // Determine number of frame buffer, later
    _arFbg[u4FbgId].ucFbNs = FBM_MAX_FB_NS_PER_GROUP;
    
    // Allocalte memory for picture header
    u4MallocSize = _arFbg[u4FbgId].ucFbNs * (sizeof(FBM_PIC_HDR_T));
    _arFbg[u4FbgId].pMallocAddr = x_mem_alloc(u4MallocSize);
    if (_arFbg[u4FbgId].pMallocAddr == NULL)
    {
        return FBM_FBG_ID_UNKNOWN;
    }
    FBM_MEMSET(_arFbg[u4FbgId].pMallocAddr, 0, u4MallocSize);
    _arFbg[u4FbgId].prPicHdr = (FBM_PIC_HDR_T *)_arFbg[u4FbgId].pMallocAddr;
    
    // Init frame buffer status
    for (u4FbIdx = 0; u4FbIdx < _arFbg[u4FbIdx].ucFbNs; u4FbIdx++)
    {
        _arFbg[u4FbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;
    }

    // Allocate frame buffer pool
    _arFbg[u4FbgId].u4FbMemoryPoolSize = _FBM_PoolAllocate(ucFbgType, &_arFbg[u4FbgId].u4FbMemoryPool);
    if ((_arFbg[u4FbgId].u4FbMemoryPoolSize == 0) ||
        (_arFbg[u4FbgId].u4FbMemoryPool == NULL))        
    {
        if (_arFbg[u4FbgId].pMallocAddr != NULL)
        {
            x_mem_free(_arFbg[u4FbgId].pMallocAddr);
            _arFbg[u4FbgId].pMallocAddr = NULL;
        }
        return FBM_FBG_ID_UNKNOWN;
    }

    // Settings
    _arFbg[u4FbgId].ucFbgType = ucFbgType;
    _arFbg[u4FbgId].ucFbgId = u4FbgId;

    // Config color mode
    FBM_ConfigColorMode(u4FbgId, FBM_CM_DEFAULT, u4HSize, u4VSize);

    // [LOG] FBM Create
    SYSLOG(FBM_PREFIX + 99, u4FbgId, ucFbgType, 0);

    ASSERT(_arFbg[u4FbgId].hEmptyQSemaphore.hMutex == _arFbg[u4FbgId].hMutex);
    ASSERT(_arFbg[u4FbgId].hEmptyRQSemaphore.hMutex == _arFbg[u4FbgId].hMutex);
    ASSERT(_arFbg[u4FbgId].hEmptyBQSemaphore.hMutex == _arFbg[u4FbgId].hMutex);

    return FBM_BYTE(u4FbgId);
}


//-------------------------------------------------------------------------
/** FBM_ReleaseGroup
 *  Release FBM Group
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ReleaseGroup(UCHAR ucFbgId)
{
    // Notes about mutex semaphore
    // We will not destroy mutex semaphore at release
    // Because OSAI itself is NOT threadsafe
    // Semaphore release function is dangerous

    void* pMallocAddr;
    UINT32 u4FbMemoryPool;
    HANDLE_T hMutex;

    COUNTING_SEMAPHORE_T         hEmptyQSemaphore;    // Empty Queue Counting Semaphore
    COUNTING_SEMAPHORE_T         hEmptyRQSemaphore;    // Empty R Queue Counting Semaphore
    COUNTING_SEMAPHORE_T         hEmptyBQSemaphore;    // Empty B Queue Counting Semaphore

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    // [LOG] FBM Release
    SYSLOG(FBM_PREFIX + 98, ucFbgId, _arFbg[ucFbgId].ucFbgType, 0);

    u4FbMemoryPool = _arFbg[ucFbgId].u4FbMemoryPool;
    pMallocAddr = _arFbg[ucFbgId].pMallocAddr;		

    // backup semaphore
    hMutex = _arFbg[ucFbgId].hMutex;

    hEmptyQSemaphore = _arFbg[ucFbgId].hEmptyQSemaphore;
    hEmptyRQSemaphore = _arFbg[ucFbgId].hEmptyRQSemaphore;
    hEmptyBQSemaphore = _arFbg[ucFbgId].hEmptyBQSemaphore;
    
    // Clear frame buffer group
    FBM_MEMSET(((void*)&_arFbg[ucFbgId]), 0, sizeof(FBM_FBG_T));
       
    // Init Settings, refer to FBM_Init
    _arFbg[ucFbgId].ucFbgType = FBM_FBG_TYPE_UNKNOWN;
    _arFbg[ucFbgId].ucFbgId = FBM_FBG_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbgCm = FBM_CM_UNKNOWN;
    _arFbg[ucFbgId].ucStcSrc = STC_SRC_NS;
    _arFbg[ucFbgId].ucAvSyncMode = AV_SYNC_MODE_NS;

    // restore semaphore
    _arFbg[ucFbgId].hMutex = hMutex;

#ifdef CC_VDP_FULL_ISR
    _arFbg[ucFbgId].prState = &_rState;
#endif

    _arFbg[ucFbgId].hEmptyQSemaphore = hEmptyQSemaphore;
    _arFbg[ucFbgId].hEmptyRQSemaphore = hEmptyRQSemaphore;
    _arFbg[ucFbgId].hEmptyBQSemaphore = hEmptyBQSemaphore;

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    // Release frame buffer pool
    _FBM_PoolRelease(u4FbMemoryPool);

    // Release memory
    if (pMallocAddr != NULL)
    {
        x_mem_free(pMallocAddr);
    }

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_ResetGroup
 *  Reset FBM Group
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ResetGroup(UCHAR ucFbgId)
{
    // Note about usage
    // When FBM_FLAG_RESET flag was set by vdp
    // 5381 Â²¤Æ, 20070403, Refer to FBM_ConfigColorMode

    UINT32 u4FbIdx;
    
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    // put all frame buffer into empty queue
    _arFbg[ucFbgId].rEmptyQ.ucCount = 0;
    for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
    {
        _FBM_PutFrameBufferToEmptyQ(ucFbgId, u4FbIdx);
        _arFbg[ucFbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;        
    }
    if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264)
    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);        
    }
    else
    {

    // reset BQ, RQ, and EmptyQ
    if (_arFbg[ucFbgId].u4FbgFlag & FBM_FLAG_FIX_REF_FB)
    {
        // create semaphore, Empty RQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyRQSemaphore, _arFbg[ucFbgId].hMutex, 0) == OSR_OK);

        // create semaphore, Empty BQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);
    }
    else
    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);

        // create semaphore, Empty BQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)(_arFbg[ucFbgId].ucFbNs - FBM_REF_FB_NS_PER_GROUP)) == OSR_OK);
    }
    }

    // clear display queue
    _arFbg[ucFbgId].rDisplayQ.ucReadIdx = 0;
    _arFbg[ucFbgId].rDisplayQ.ucCount = 0;

    // Init Settings
    _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbFRefValid = 0;
    _arFbg[ucFbgId].ucFbBRefValid = 0;

    // default flag
    _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_NOT_READY;    

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_InvalidateRefFrameBuffer
 *  Invalidate FBM Group reference frame buffer
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_InvalidateRefFrameBuffer(UCHAR ucFbgId)
{
    // Note about usage
    // When mpv mode change, invalidate refernce
    // Start again at next I picture

    UINT32 u4FbIdx;
    UINT32 u4LockNs;
    UINT32 u4EmptyNs;
    UINT32 u4EmptyRefNs;
    
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);
    
    // display can lock 1~2 B picture
    u4LockNs = 0;
    for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
    {
        if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
        {
            u4LockNs++;
        }
    }

    // reset counting Semaphore
    _arFbg[ucFbgId].hEmptyBQSemaphore.i4Count = 0;
    _arFbg[ucFbgId].hEmptyRQSemaphore.i4Count = 0;
    _arFbg[ucFbgId].hEmptyQSemaphore.i4Count = 0;

    // clear empty queue
    _arFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;
    _arFbg[ucFbgId].rEmptyQ.ucCount = 0;

    // clear display queue
    _arFbg[ucFbgId].rDisplayQ.ucReadIdx = 0;
    _arFbg[ucFbgId].rDisplayQ.ucCount = 0;
    _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
//    _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
//    _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbFRefValid = 0;
    _arFbg[ucFbgId].ucFbBRefValid = 0;

    u4EmptyNs = _arFbg[ucFbgId].ucFbNs - u4LockNs;

    u4EmptyRefNs = 0;
    if ((_arFbg[ucFbgId].ucFbFRef != FBM_FB_ID_UNKNOWN) &&
        (_arFbg[ucFbgId].aucFbStatus[_arFbg[ucFbgId].ucFbFRef] == FBM_FB_STATUS_LOCK))
    {
    }
    else
    {
        u4EmptyRefNs++;
    }
    if ((_arFbg[ucFbgId].ucFbBRef != FBM_FB_ID_UNKNOWN) &&
        (_arFbg[ucFbgId].aucFbStatus[_arFbg[ucFbgId].ucFbBRef] == FBM_FB_STATUS_LOCK))
    {
    }
    else
    {
        u4EmptyRefNs++;
    }

    // set counting Semaphore
    // Bug fixed 20050708, reference may locked by display
    // _arFbg[ucFbgId].hEmptyBQSemaphore.i4Count = (INT32)(u4EmptyNs - FBM_REF_FB_NS_PER_GROUP) is NOT correct!

    if (_arFbg[ucFbgId].u4FbgFlag & FBM_FLAG_FIX_REF_FB)
    {
        UINT32 u4RefNs;

        u4RefNs = 0;
        if ((_arFbg[ucFbgId].ucFbFRef != FBM_FB_ID_UNKNOWN) &&
            (_arFbg[ucFbgId].aucFbStatus[_arFbg[ucFbgId].ucFbFRef] != FBM_FB_STATUS_LOCK))
        {
            u4RefNs++;
        }
        if ((_arFbg[ucFbgId].ucFbBRef != FBM_FB_ID_UNKNOWN) &&
            (_arFbg[ucFbgId].aucFbStatus[_arFbg[ucFbgId].ucFbBRef] != FBM_FB_STATUS_LOCK))
        {
            u4RefNs++;
        }
        ASSERT(u4EmptyNs >= u4RefNs);
    
        // reset semaphore, Empty RQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyRQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)u4RefNs) == OSR_OK);

        // reset semaphore, Empty BQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)(u4EmptyNs - u4RefNs)) == OSR_OK);
    }
    else
    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)u4EmptyNs) == OSR_OK);

        // create semaphore, Empty BQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)(u4EmptyNs - u4EmptyRefNs)) == OSR_OK);
    }

    for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
    {
        if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] != FBM_FB_STATUS_LOCK) 
        {
            _arFbg[ucFbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;
            _FBM_PutFrameBufferToEmptyQ(ucFbgId, u4FbIdx);
        }
    }

    LOG(7, "Invalidate G(%d) S(%d %d %d %d) R(%02x %02x) Q(%d %d %d %d)\n", 
        ucFbgId,
        _arFbg[ucFbgId].aucFbStatus[0],
        _arFbg[ucFbgId].aucFbStatus[1],
        _arFbg[ucFbgId].aucFbStatus[2],
        _arFbg[ucFbgId].aucFbStatus[3],
        _arFbg[ucFbgId].ucFbFRef,
        _arFbg[ucFbgId].ucFbBRef,
        _arFbg[ucFbgId].rDisplayQ.ucCount,
        _arFbg[ucFbgId].rEmptyQ.ucCount,
        _arFbg[ucFbgId].hEmptyQSemaphore.i4Count,
        _arFbg[ucFbgId].hEmptyBQSemaphore.i4Count);

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.i4Count >= 0);
    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.i4Count >= 0);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.i4Count >= 0);

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_FlushLockFrameBuffer
 *  Flush FBM Group lock frame buffer
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_FlushLockFrameBuffer(UCHAR ucFbgId)
{
    // Note about usage
    // When vdp mode change, flush lock frame buffer

    UINT32 u4FbIdx;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    // put all lock frame buffer into empty queue
    for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
    {
        if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
        {

            if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264)
            {
                VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
            }
            else
            {
        
            if ((_arFbg[ucFbgId].u4FbgFlag & FBM_FLAG_FIX_REF_FB) == 0)
            {
                // release semaphore (Empty)
                VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);

                if (IS_REFERENCE_FB(ucFbgId, u4FbIdx))
                {    // release reference frame buffer
                }
                else
                {    // release B frame buffer
                
                    // release semaphore (B & Empty)
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);
                }
            }
            else
            {
                if (IS_REFERENCE_FB(ucFbgId, u4FbIdx))
                {    // release reference frame buffer

                    // release semaphore (R)
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyRQSemaphore) == OSR_OK);
                }
                else
                {    // release B frame buffer
                
                    // release semaphore (B)
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);
                }
            }
            }

            _FBM_PutFrameBufferToEmptyQ(ucFbgId, u4FbIdx);

            _arFbg[ucFbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;
        }    
    }

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_WaitUnlockFrameBuffer
 *  Wait FBM Group unlock frame buffer
 *  @param  ucFbgType   FBG type.
 *  @param  u4Timeout   time to wait.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_WaitUnlockFrameBuffer(UCHAR ucFbgId, UINT32 u4Timeout)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if ((_arFbg[ucFbgId].u4FbgFlag & FBM_FLAG_FIX_REF_FB) == 0)
    {
        UINT32 u4FbIdx;
        UINT32 u4LockFbNs;
        UINT32 u4EmptyFbNs;
    
        // unlock frame buffer Ns
        u4LockFbNs = 0;
        u4EmptyFbNs = 0;
        for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
        {
            if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
            {
                u4LockFbNs++;
            }
            else
            if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_EMPTY)
            {
                u4EmptyFbNs++;
            }
        }

        // wait for lock frame buffer
        if (u4LockFbNs != 0)
        {
            u4FbIdx = 0;
            while (u4FbIdx < (u4LockFbNs + u4EmptyFbNs))
            {
                if(_FBM_csema_lock(&_arFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_WAIT, u4Timeout) == OSR_OK)
                {
                    u4FbIdx++;
                }
                else
                {
                    u4FbIdx = _arFbg[ucFbgId].ucFbNs;
                    LOG(1, "Wait unlock timeout! (%d, %d)\n",u4LockFbNs, u4EmptyFbNs);
                }
            }
        }
    }
    else
    {
        UINT32 u4FbIdx;
        UINT32 u4LockBFbNs;
        UINT32 u4EmptyBFbNs;
        UINT32 u4LockRFbNs;
        UINT32 u4EmptyRFbNs;

        // unlock frame buffer Ns
        u4LockBFbNs = 0;
        u4EmptyBFbNs = 0;
        u4LockRFbNs = 0;
        u4EmptyRFbNs = 0;
        for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
        {
            if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
            {
                if (IS_REFERENCE_FB(ucFbgId, u4FbIdx))
                {
                    u4LockRFbNs++;
                }
                else
                {
                    u4LockBFbNs++;
                }
            }
            else
            if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_EMPTY)
            {
                if (IS_REFERENCE_FB(ucFbgId, u4FbIdx))
                {
                    u4EmptyRFbNs++;
                }
                else
                {
                    u4EmptyBFbNs++;
                }
            }
        }

        // wait for lock R frame buffer
        if (u4EmptyRFbNs != 0)
        {
            u4FbIdx = 0;
            while (u4FbIdx < (u4LockRFbNs + u4EmptyRFbNs))
            {
                if(_FBM_csema_lock(&_arFbg[ucFbgId].hEmptyRQSemaphore, X_SEMA_OPTION_WAIT, u4Timeout) == OSR_OK)
                {
                    u4FbIdx++;
                }
                else
                {
                    u4FbIdx = _arFbg[ucFbgId].ucFbNs;
                    LOG(1, "Wait unlock timeout! (%d, %d)\n",u4LockRFbNs, u4EmptyRFbNs);
                }
            }
        }

        // wait for lock B frame buffer
        if (u4EmptyBFbNs != 0)
        {
            u4FbIdx = 0;
            while (u4FbIdx < (u4LockBFbNs + u4EmptyBFbNs))
            {
                if(_FBM_csema_lock(&_arFbg[ucFbgId].hEmptyBQSemaphore, X_SEMA_OPTION_WAIT, u4Timeout) == OSR_OK)
                {
                    u4FbIdx++;
                }
                else
                {
                    u4FbIdx = _arFbg[ucFbgId].ucFbNs;
                    LOG(1, "Wait unlock timeout! (%d, %d)\n",u4LockRFbNs, u4EmptyRFbNs);
                }
            }
        }
    }

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_ReleaseDispQ
 *  Release fb in DispQ to EmptyQ.
 *  @param  ucFbgId   FBG index.
 *  @return release fb number.
 */
//-------------------------------------------------------------------------
void FBM_ReleaseDispQ(UCHAR ucFbgId)
{
    UCHAR ucCnt;
    UCHAR ucFbId;
    UCHAR ucIdx;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    } 

    ucCnt = FBM_CheckFrameBufferDispQ(ucFbgId);

    for (ucIdx = 0; ucIdx < ucCnt; ucIdx++)
    {
        ucFbId = FBM_GetFrameBufferFromDispQ(ucFbgId);
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);                            
    }
}


//-------------------------------------------------------------------------
/** FBM_ConfigColorMode
 *  Check FBG color mode
 *  @param  ucFbgType   FBG type.
 *  @param  ucFbgCm   color mode.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ConfigColorMode(UCHAR ucFbgId, UCHAR ucFbgCm, UINT32 u4HSize, UINT32 u4VSize)
{
    // Note about color
    // Support 420/422/444/420Digest

    UCHAR ucFbpListNs;

    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4MvSize;    

    UINT32 u4FbNs;
    UINT32 u4ListIdx;

    FBP_LIST_T* prFbpList;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _arFbg[ucFbgId].rEmptyQ.ucCount = 0;
    _arFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;

    _FBM_GetPoolList(&prFbpList, &ucFbpListNs);

    u4YSize = 0;
    u4CSize = 0;
    for (u4ListIdx = 0; u4ListIdx < ucFbpListNs; u4ListIdx++)
    {
        if (_arFbg[ucFbgId].ucFbgType == prFbpList[u4ListIdx].ucType)
        {
            u4YSize = prFbpList[u4ListIdx].u4YSize;
            u4CSize = prFbpList[u4ListIdx].u4CSize;

            _arFbg[ucFbgId].u4FbWidth = prFbpList[u4ListIdx].u4Width;
            _arFbg[ucFbgId].u4FbHeight = prFbpList[u4ListIdx].u4Height;

            u4ListIdx = ucFbpListNs;
        }
    }

    if (ucFbgCm == FBM_CM_420)
    {
        UINT32 u4FbIdx;
        UINT32 u4Addr;
        UINT32 u4Size;
        UINT32 u4WidthDiv = 1;

        _arFbg[ucFbgId].ucFbgCm = FBM_CM_420;

        // Calculate Frame Buffer Number
        u4Size = u4YSize + u4CSize;
        if (u4Size != 0)
        {
            if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264)        
            {
                u4HSize = FBM_ALIGN_MASK(u4HSize, FBM_B2R_H_PITCH);
                u4VSize = FBM_ALIGN_MASK(u4VSize, FBM_MPV_V_ALIGMENT);
                
                if (((_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_1080HD) && 
                    ((u4HSize < FBM_FBG_1080HD_H) || (u4VSize < FBM_FBG_1080HD_V)))
                    ||((_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_720HD) && 
                    ((u4HSize < FBM_FBG_720HD_H) || (u4VSize < FBM_FBG_720HD_V))))
                {
                    u4YSize = FBM_ALIGN_MASK((u4HSize * u4VSize), FBM_FMG_Y_ALIGMENT);
                    u4CSize = u4YSize >> 1;
                }
                u4FbNs = ((_arFbg[ucFbgId].u4FbMemoryPoolSize << 2)/7)/u4YSize;
            }
            else
            {
            u4FbNs = _arFbg[ucFbgId].u4FbMemoryPoolSize / u4Size;
            }

        }
        else
        {
            u4FbNs = 0;
            ASSERT(0);
        }

#if 0
        // Add one more Fb
        u4FbNs++;
#endif
        if (u4FbNs <= 2)
        {
            u4FbNs = _arFbg[ucFbgId].u4FbMemoryPoolSize / (u4Size >> 1);
            u4YSize = u4YSize >> 1;
            u4CSize = u4CSize >> 1;                                            
            FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_REDUCE_REF);           
            LOG(3, "Half-Ref mode\n");

            u4WidthDiv = 2;
        }

        if (u4FbNs > FBM_MAX_FB_NS_PER_GROUP)
        {
            u4FbNs = FBM_MAX_FB_NS_PER_GROUP;
        }

#ifdef DEBUG_MAX_FB
        if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_PAL)
        {
            u4FbNs = FBM_MAX_FB_NS_PER_GROUP;
        }
#endif

        _arFbg[ucFbgId].ucFbNs = u4FbNs;

        u4Addr = _arFbg[ucFbgId].u4FbMemoryPool;
        for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
        {
            // Y
            _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;

            if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_GFX_MEMSET]))
            {
                ((FBM_GFX_MEMSET_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET])
                    ((UINT8*)u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), _arFbg[ucFbgId].u4FbHeight, 0x0);  
            }
            else
            {
                ASSERT(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET] == 0);
            }
            
            u4Addr += u4YSize;            

            _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4Addr;

            if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_GFX_MEMSET]))
            {
                ((FBM_GFX_MEMSET_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET])
                ((UINT8*)u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight >> 1), 0x80);
            
            }
            else
            {
                ASSERT(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET] == 0);
            }
                        
            u4Addr += u4CSize;            
            
        }
#if 0
        for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
        {
            // C
            _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4Addr;

            if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_GFX_MEMSET]))
            {
                //((FBM_GFX_MEMSET_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET])((UINT8*)u4Addr, u4CSize, 0x80);
                ((FBM_GFX_MEMSET_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET])
                ((UINT8*)u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight >> 1), 0x80);
            }
            else
            {
                ASSERT(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET] == 0);
            }
            
//            if (GFX_Memset((UINT8*)u4Addr, u4CSize, 0x80) != (INT32) E_GFX_OK)
//            {
//                LOG(5, "GFX_Memset C fails\n");            
//            }
            u4Addr += u4CSize;
        }
#endif        
        
        if (_arFbg[ucFbgId].u4VDecFmt != FBM_VDEC_H264)
        {        
            if ((u4FbIdx >= FBM_DBK_FB_NS) && (u4FbIdx < _arFbg[ucFbgId].ucFbNs))
        { 
            UINT32 u4ExtraYSize; 
                  
    	    _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr; 
     
            switch (_arFbg[ucFbgId].ucFbgType) 
            { 
                 
                case FBM_FBG_TYPE_1080HD: 
                u4ExtraYSize = FBM_1080HD_Y_EXTRA_SIZE; 
                break; 
     
                case FBM_FBG_TYPE_720HD: 
                u4ExtraYSize = FBM_720HD_Y_EXTRA_SIZE; 
                break;                 
     
                case FBM_FBG_TYPE_PAL: 
                u4ExtraYSize = FBM_PAL_Y_EXTRA_SIZE; 
                break;                 
     
                case FBM_FBG_TYPE_NTSC: 
                u4ExtraYSize = FBM_NTSC_Y_EXTRA_SIZE; 
                break;                 
     
            default: 
                u4ExtraYSize = 0; 
                ASSERT(0); 
                break; 
            } 

            _arFbg[ucFbgId].au4AddrC[u4FbIdx] = FBM_ALIGN_MASK((u4Addr + u4ExtraYSize), FBM_MPV_C_ALIGMENT); 
        } 
        } 
        else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264)
        {
            u4MvSize = u4YSize >> 2;
 
            for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
            {
                _arFbg[ucFbgId].au4AddrMv[u4FbIdx] = u4Addr;
                u4Addr += u4MvSize;            
            }        
        }
    } 
    else
    if ((ucFbgCm == FBM_CM_422) || (ucFbgCm == FBM_CM_444))
    {
        UINT32 u4FbIdx;
        UINT32 u4Addr;
        UINT32 u4Size;

        if (ucFbgCm == FBM_CM_422)
        {
            _arFbg[ucFbgId].ucFbgCm = FBM_CM_422;
            u4CSize *= 2;    // 420 to 422
        }
        else
        {
            _arFbg[ucFbgId].ucFbgCm = FBM_CM_444;
            u4CSize *= 4;    // 420 to 444
        }

        // Calculate Frame Buffer Number
        u4Size = u4YSize + u4CSize;
        if (u4Size != 0)
        {
            u4FbNs = _arFbg[ucFbgId].u4FbMemoryPoolSize / u4Size;
        }
        else
        {
            u4FbNs = 0;
            ASSERT(0);
        } 

        if (u4FbNs > FBM_MAX_FB_NS_PER_GROUP)
        {
            u4FbNs = FBM_MAX_FB_NS_PER_GROUP;
        }

        _arFbg[ucFbgId].ucFbNs = u4FbNs;

        u4Addr = _arFbg[ucFbgId].u4FbMemoryPool;
        for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
        {
            // Y
            _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;
            u4Addr += u4YSize;

            // C
            _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4Addr;
            u4Addr += u4CSize;
        }
    }
    else
    {
        _arFbg[ucFbgId].ucFbgCm = FBM_CM_UNKNOWN;
    }

    {
        UINT32 u4FbIdx;

        if (_arFbg[ucFbgId].ucFbNs == FBM_RACING_FULLB_FB_NS)
        {
            _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_FIX_REF_FB;
            _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_RACING_FULL_B;
            LOG(5, "FBM_FLAG_RACING_FULL_B\n");
        }
        else
        if (_arFbg[ucFbgId].ucFbNs < FBM_RACING_FULLB_FB_NS)
        {
            _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_FIX_REF_FB;
            _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_RACING_HALF_B;

    		//create semaphore for 2.5 compact mode
    	    VERIFY(x_sema_create(&_arFbg[ucFbgId].hChgFieldSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);        
            
            LOG(5, "FBM_FLAG_RACING_HALF_B\n");
            ASSERT(0);
        }

#if 0
        _arFbg[ucFbgId].ucFbNs = 3;
        _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_FIX_REF_FB;
//        _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_RACING_HALF_B;
        _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_RACING_FULL_B;

		//create semaphore for 2.5 compact mode
	    //VERIFY(x_sema_create(&_arFbg[ucFbgId].hChgFieldSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
		
#endif

        if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264)
        {
            // create semaphore, Empty Q
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);        
        }
        else
        {
        if (_arFbg[ucFbgId].u4FbgFlag & FBM_FLAG_FIX_REF_FB)
        {
            // create semaphore, Empty RQ
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyRQSemaphore, _arFbg[ucFbgId].hMutex, 0) == OSR_OK);

            // create semaphore, Empty BQ
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);

#ifdef CC_VDP_FULL_ISR
            _arFbg[ucFbgId].hEmptyRQSemaphore.prState = &_rState;
            _arFbg[ucFbgId].hEmptyBQSemaphore.prState = &_rState;
#endif
        }
        else
        {
            // create semaphore, Empty Q
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);

            // create semaphore, Empty BQ
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)(_arFbg[ucFbgId].ucFbNs - FBM_REF_FB_NS_PER_GROUP)) == OSR_OK);

#ifdef CC_VDP_FULL_ISR
            _arFbg[ucFbgId].hEmptyQSemaphore.prState = &_rState;
            _arFbg[ucFbgId].hEmptyBQSemaphore.prState = &_rState;
#endif
            }
        }

        // Nivia and William Co-design
        _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;

        // put all frame buffer into empty queue
        for (u4FbIdx = 0; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
        {
            _FBM_PutFrameBufferToEmptyQ(ucFbgId, u4FbIdx);
        }
    }

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_SetFrameBufferColor
 *  Fill frame buffer with constant color
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  u4Color   color.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferColor(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Color)
{
    UCHAR* pucYAddr;
    UCHAR* pucCAddr;
    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4Idx;
    UCHAR ucY;
    UCHAR ucCb;
    UCHAR ucCr;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    switch (_arFbg[ucFbgId].ucFbgType)
    {
        case FBM_FBG_TYPE_1080HD:
            u4YSize = FBM_FBG_TYPE_1080HD_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_1080HD_C_SIZE;
            break;

        case FBM_FBG_TYPE_720HD:
            u4YSize = FBM_FBG_TYPE_720HD_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_720HD_C_SIZE;
            break;                

        case FBM_FBG_TYPE_PAL:
            u4YSize = FBM_FBG_TYPE_PAL_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_PAL_C_SIZE;
            break;                

        case FBM_FBG_TYPE_NTSC:
            u4YSize = FBM_FBG_TYPE_NTSC_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_NTSC_C_SIZE;
            break;                

        default:
            u4YSize = 0;
            u4CSize = 0;
            ASSERT(0);
            break;
    }

    if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_422)
    {
        u4CSize *= 2;
    }
    else
    if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_444)
    {
        u4CSize *= 4;
    }
    else
    {
    }

    ucY = (u4Color >> 16) & 0xFF;
    ucCb = (u4Color >> 8) & 0xFF;
    ucCr = (u4Color) & 0xFF;

    pucYAddr = (UCHAR*) _arFbg[ucFbgId].au4AddrY[ucFbId];
    pucCAddr = (UCHAR*)_arFbg[ucFbgId].au4AddrC[ucFbId];

    // use Gfx Engine in the future
    FBM_MEMSET(pucYAddr, ucY, u4YSize);

    for (u4Idx = 0; u4Idx < u4CSize; u4Idx += 2)
    {
        pucCAddr[u4Idx + 0] = ucCb;
        pucCAddr[u4Idx + 1] = ucCr;
    }
}


//-------------------------------------------------------------------------
/** FBM_SetFrameBufferZebraColor
 *  Fill frame buffer with constant color
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferZebraColor(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR* pucYAddr;
    UCHAR* pucCAddr;
    UCHAR ucYColor;
    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4Idx;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    switch (_arFbg[ucFbgId].ucFbgType)
    {
        case FBM_FBG_TYPE_1080HD:
            u4YSize = FBM_FBG_TYPE_1080HD_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_1080HD_C_SIZE;
            break;

        case FBM_FBG_TYPE_720HD:
            u4YSize = FBM_FBG_TYPE_720HD_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_720HD_C_SIZE;
            break;                

        case FBM_FBG_TYPE_PAL:
            u4YSize = FBM_FBG_TYPE_PAL_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_PAL_C_SIZE;
            break;                

        case FBM_FBG_TYPE_NTSC:
            u4YSize = FBM_FBG_TYPE_NTSC_Y_SIZE;
            u4CSize = FBM_FBG_TYPE_NTSC_C_SIZE;
            break;                

        default:
            u4YSize = 0;
            u4CSize = 0;
            ASSERT(0);
            break;
    }

    if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_422)
    {
        u4CSize *= 2;
    }
    else
    if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_444)
    {
        u4CSize *= 4;
    }
    else
    {
    }

    pucYAddr = (UCHAR*) _arFbg[ucFbgId].au4AddrY[ucFbId];
    pucCAddr = (UCHAR*)_arFbg[ucFbgId].au4AddrC[ucFbId];

    ucYColor = 0;
    for (u4Idx = 0; u4Idx < u4YSize; u4Idx++)
    {
        pucYAddr[u4Idx] = ucYColor;

        if ((u4Idx & 0x3F) == 0x3F)
        {
            ucYColor = ~ucYColor;
        }
    }

    // use Gfx Engine in the future
    FBM_MEMSET(pucCAddr, 0x80, u4CSize);
}


//-------------------------------------------------------------------------
/** FBM_SetFrameBufferPlayBackNs
 *  Set frame buffer playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  ucPlayBackNs   playback number.
 *  @param  ucChangeFieldNs   change field number.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs, UCHAR ucChangeFieldNs)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    _arFbg[ucFbgId].aucPlayBackNs[ucFbId] = ucPlayBackNs;
    _arFbg[ucFbgId].aucChangeFieldNs[ucFbId] = ucChangeFieldNs;
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferPlayBackNs
 *  Get frame buffer playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return playback number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    return _arFbg[ucFbgId].aucPlayBackNs[ucFbId];
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferChangeFieldNs
 *  Get frame buffer change field number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return change field number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferChangeFieldNs(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    return _arFbg[ucFbgId].aucChangeFieldNs[ucFbId];
}


//-------------------------------------------------------------------------
/** FBM_ChkFrameBufferLastPlayBack
 *  Check frame buffer last playback
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return 1/0 for last playback or NOT.
 */
//-------------------------------------------------------------------------
UCHAR FBM_ChkFrameBufferLastPlayBack(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 1;
    }

    if (_arFbg[ucFbgId].aucPlayBackNs[ucFbId] > 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


//-------------------------------------------------------------------------
/** FBM_QueryStatus
 *  Query FBM status
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_QueryStatus(void)
{
    UCHAR ucIdx;

    for (ucIdx = 0; ucIdx < FBG_MAX_NS; ucIdx++)
    {
        if ((_arFbg[ucIdx].ucFbgType != FBM_FBG_TYPE_UNKNOWN) &&
            (_arFbg[ucIdx].ucFbgId != FBM_FBG_ID_UNKNOWN))
        {
            _FbgStatus(_arFbg[ucIdx].ucFbgId);
        }
    }

    _FBM_PoolStatus();
}

//-------------------------------------------------------------------------
/** FBM_RegCbFunc
 */
//-------------------------------------------------------------------------
void FBM_RegCbFunc(FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr)
{
    if (eFuncType < FBM_CB_FUNC_NS)
    {
        if (u4FuncPtr == 0)
        {
            _arFbmCbFunc.au4CbFunc[eFuncType] = u4FuncPtr;
            _arFbmCbFunc.au4CbFuncCRC[eFuncType] = u4FuncPtr;
        }
        else
        {
            _arFbmCbFunc.au4CbFunc[eFuncType] = u4FuncPtr;
            _arFbmCbFunc.au4CbFuncCRC[eFuncType] = ~u4FuncPtr;
        }    
    }
    else
    {
        ASSERT(0);
    }
}

//-------------------------------------------------------------------------
/** FBM_GetCbFuncTbl
 */
//-------------------------------------------------------------------------
FBM_CB_FUNC_T* FBM_GetCbFuncTbl(void)
{
    return (&_arFbmCbFunc);
}

//-------------------------------------------------------------------------
/** FBM_FbgChgNotify
 *  FBM notify (MPEG to VDP)
 *  @param  ucFbgId   FBG index.
 *  @param  ucEsId   ESindex.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_FbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId)
{
    // Note about usage
    // This function is called back by mpeg decode to indicate that
    // Frame buffer group ID is change. 
    // It will by pass this notify to video plane driver

    if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_FBG_CHG_IND]))
    {
        ((FBM_FBG_CHG_IND_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND])(ucFbgId, ucEsId);
    }
    else
    {
        ASSERT(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND] == 0);
    }
}

//-------------------------------------------------------------------------
/** FBM_PicStructChgNotify
 *  FBM notify (MPEG to VDP)
 *  @param  ucEsId   ESindex.
 *  @param  ucPicStruct   Frame/Field.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_PicStructChgNotify(UCHAR ucEsId, UCHAR ucPicStruct)
{
    // Note about usage
    // This function is called back by mpeg decode to indicate that
    // Picture structure is change. (frame/field)
    // It will by pass this notify to video plane driver

    if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_PICSTRUCT_CHG_IND], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_PICSTRUCT_CHG_IND]))
    {
        ((FBM_PICSTRUCT_CHG_IND_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_PICSTRUCT_CHG_IND])(ucEsId, ucPicStruct);
    }
    else
    {
        ASSERT(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_PICSTRUCT_CHG_IND] == 0);
    }
}

//-------------------------------------------------------------------------
/** FBM_SetSyncStc
 *  Set AV Sync Info
 *  @param  ucFbgId   FBG index
 *  @param  ucMode   AV Sync Mode
 *  @param  ucStcId   AV Sync Source
  *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetSyncStc(UCHAR ucFbgId, UCHAR ucMode, UCHAR ucStcId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    ASSERT(ucMode < AV_SYNC_MODE_NS);
    ASSERT(ucStcId < STC_SRC_NS);
    
    _arFbg[ucFbgId].ucStcSrc = ucStcId;
    _arFbg[ucFbgId].ucAvSyncMode = ucMode;
}


//-------------------------------------------------------------------------
/** FBM_GetSyncStc
 *  GetV Sync Info
 *  @param  ucFbgId   FBG index
 *  @param  pucMode   AV Sync Mode pointer
 *  @param  pucStcId   AV Sync Source pointer
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetSyncStc(UCHAR ucFbgId, UCHAR* pucMode, UCHAR* pucStcId)
{
    if ((pucMode == NULL) || (pucStcId == NULL))
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    
    *pucMode = _arFbg[ucFbgId].ucAvSyncMode;
    *pucStcId = _arFbg[ucFbgId].ucStcSrc;
} 


//-------------------------------------------------------------------------
/** FBM_CheckPadding
 *  Check if pool memory corruption
 *  @param NONE
 *  @return NONE.
 */
//-------------------------------------------------------------------------

void FBM_CheckPadding(void)
{
#ifndef __KERNEL__
    UINT32 u4PoolIdx;
    UINT32 u4Idx;
    UINT32 u4PadAddr;
    UINT32 u4PadLen;
    UINT32 u4Addr;
    UINT32 u4Valid;

    u4Valid = 1;
    for (u4PoolIdx = 1 ; u4PoolIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolIdx++)
    {
        if ((_prPool[u4PoolIdx].u4Inherit == 0) &&
            (_prPool[u4PoolIdx].u4Addr > DRVCUST_OptGet(eFbmMemAddr)))
        {
            u4PadAddr = _prPool[u4PoolIdx].u4Addr - FBM_POOL_PADDING_SIZE;
            u4PadLen = FBM_POOL_PADDING_SIZE;

            for (u4Idx = 0; u4Idx < u4PadLen; u4Idx++)
            {            
                u4Addr = u4PadAddr + u4Idx;
            
                if (*(UCHAR*)u4Addr != FBM_POOL_PADDING)
                {
                    LOG(1, "Pool(%d) memory corruption! addr = 0x%x, val= 0x%x\n", u4PoolIdx, u4Addr, *(UCHAR*)u4Addr);

                    u4Valid = 0;

                    u4Idx = u4PadLen;
                }
            }        
        }
    }

    if (u4Valid)
    {
        LOG(1, "No memory corruption\n");
    }
#else    
    LOG(1, "NOT Support!\n");
#endif    
}


//-------------------------------------------------------------------------
/** FBM_ResetFbChangeField
 *  Wait Frame buffer change field
 *  @param  ucFbgType   FBG type.
 *  @return lock status.
 */
//-------------------------------------------------------------------------
INT32 FBM_ResetFbChangeField(UCHAR ucFbgId){

	if (VERIFY_FBG(ucFbgId))
	{
		return OSR_INV_ARG;
	}
	
    if (x_sema_lock(_arFbg[ucFbgId].hChgFieldSema, X_SEMA_OPTION_NOWAIT) == OSR_OK)
    {
    }
    else
    {
    }

    return OSR_OK;
}


//-------------------------------------------------------------------------
/** FBM_WaitFbChangeField
 *  Wait Frame buffer change field
 *  @param  ucFbgType   FBG type.
 *  @param  u4Timeout   time to wait.
 *  @return lock status.
 */
//-------------------------------------------------------------------------
INT32 FBM_WaitFbChangeField(UCHAR ucFbgId, UINT32 u4Timeout)
{
	INT32 i4Ret;

	if (VERIFY_FBG(ucFbgId))
	{
		return OSR_INV_ARG;
	}
	
	i4Ret = x_sema_lock_timeout(_arFbg[ucFbgId].hChgFieldSema, u4Timeout);

	return i4Ret;
}


//-------------------------------------------------------------------------
/** FBM_FbChangeField
 *  Frame buffer change field
 *  @param  ucFbgType   FBG type.
 *  @return unlock status.
 */
//-------------------------------------------------------------------------
void FBM_FbChangeField(UCHAR ucFbgId)
{
	if ((ucFbgId == FBM_FBG_ID_UNKNOWN) ||
		(_arFbg[ucFbgId].hChgFieldSema == 0))
	{
		return;
	}
	
    if (_arFbg[ucFbgId].hChgFieldSema)
    {
	    VERIFY(x_sema_unlock(_arFbg[ucFbgId].hChgFieldSema) == OSR_OK);
	}
	
#ifdef VDP_RACE_LOG
    VDP_FrcRacingLog('H');
#endif		

}


//-------------------------------------------------------------------------
/** FBM_SetFbNextFieldPlayBackNs
 *  Set frame buffer next field playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  ucPlayBackNs   playback number.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    _arFbg[ucFbgId].aucChgFldPlayBackNs[ucFbId] = ucPlayBackNs;

}


//-------------------------------------------------------------------------
/** FBM_GetFbNextFieldPlayBackNs
 *  Get frame buffer next field playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return playback number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    return _arFbg[ucFbgId].aucChgFldPlayBackNs[ucFbId];
}





