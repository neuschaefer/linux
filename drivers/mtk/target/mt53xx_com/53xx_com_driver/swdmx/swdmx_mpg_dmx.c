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
 * $RCSfile: swdmx_mpg_dmx.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx.c
 *  Driver interface: Demux implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

//#include "dmx.h"
#include "dmx_if.h"
#include "aud_drvif.h"
#include "fbm_drvif.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_os.h"
#include "swdmx_debug.h"

LINT_EXT_HEADER_END

#include "swdmx_mpg_dmx.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// Number of filter supported
#define NUM_FILTER                      32

/// Number of PID supported
#define NUM_PID                         32

/// Number of message slots
#define NUM_MESSAGE                     32


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// Internal filter structure
typedef struct
{
    BOOL fgInUse;                   ///< Is this filter in-use or not
    UINT8 u1Pidx;                   ///< The pid id used of this filter
} _DMX_FILTER_T;

/// Internal PID structure
typedef struct
{
    BOOL fgInUse;                   ///< Is this pid in-use or not
    UINT8 u1FilterId;               ///< The corresponding filter id if exist
    PFN_SWDMX_CALLBACK pfnHandler;    ///< The PES or section callback handler
} _DMX_PID_T;

/// Internal message structure
typedef struct
{
    BOOL fgInUse;                   ///< Is this message in-use or free
    SWDMX_DMX_MSG_T rMsg;             ///< The message body
} _DMX_MSG_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static varialbles
//-----------------------------------------------------------------------------

/// Filter array to track filter availability and corresponding PID id
static _DMX_FILTER_T _arFilter[NUM_FILTER];

/// PID array to track PID availability and corresponding filter id
static _DMX_PID_T _arPid[NUM_PID];

/// Message slots
static _DMX_MSG_T _arMsg[NUM_MESSAGE];

/// Critical section state
static CRIT_STATE_T _rLock;

/// Tracking if nested critical section occurred
static BOOL _fgDmxLocking = FALSE;


//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
BOOL _SWDMX_DmxReleasePid(UINT8 u1Pidx);
BOOL _SWDMX_DmxAllocatePid(UINT8* pu1Pidx);

//-----------------------------------------------------------------------------
// Static functionss
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Enter demux init
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID SWDMX_DmxInit(VOID)
{
    x_memset(&_arFilter[0], 0, (sizeof(_DMX_FILTER_T)*NUM_FILTER));
    x_memset(&_arPid[0], 0, (sizeof(_DMX_PID_T)*NUM_PID));
    x_memset(&_arMsg[0], 0, (sizeof(_DMX_MSG_T)*NUM_MESSAGE));
}

//-----------------------------------------------------------------------------
/** Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _SWDMX_Lock(void)
{
    _rLock = x_crit_start();

    ASSERT(!_fgDmxLocking);
    _fgDmxLocking = TRUE;

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _SWDMX_Unlock(void)
{
    ASSERT(_fgDmxLocking);
    _fgDmxLocking = FALSE;

    x_crit_end(_rLock);

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** Allocate a message slot.
 *
 *  @param[out]     pu1MsgId        The allocated message id.
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_AllocateMessage(UINT8* pu1MsgId)
{
    BOOL fgRet;
    UINT32 i;

    ASSERT(pu1MsgId != NULL);

    fgRet = FALSE;

    _SWDMX_Lock();

    for (i = 0; i < NUM_MESSAGE; i++)
    {
        if (!_arMsg[i].fgInUse)
        {
            _arMsg[i].fgInUse = TRUE;
            *pu1MsgId = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    _SWDMX_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Release a message slot
 *
 *  @param  u1MsgId         The message id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_ReleaseMessage(UINT8 u1MsgId)
{
    if (u1MsgId >= NUM_PID)
    {
        return FALSE;
    }

    if (!_arMsg[u1MsgId].fgInUse)
    {
        return FALSE;
    }

    _SWDMX_Lock();

    _arMsg[u1MsgId].fgInUse = FALSE;

    _SWDMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Allocate a filter slot
 *
 * @param[out]
 *          pu1FilterId     The allocated message id.
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_AllocateFilter(UINT8* pu1FilterId)
{
    BOOL fgRet;
    UINT32 i;

    ASSERT(pu1FilterId != NULL);

    fgRet = FALSE;

    _SWDMX_Lock();

    for (i = 0; i < NUM_FILTER; i++)
    {
        if (!_arFilter[i].fgInUse)
        {
            _arFilter[i].fgInUse = TRUE;
            *pu1FilterId = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    _SWDMX_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Release a filter slot
 *
 *  @param  u1FilterId      The filter id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_ReleaseFilter(UINT8 u1FilterId)
{
    if (u1FilterId >= NUM_FILTER)
    {
        return FALSE;
    }

    if (!_arFilter[u1FilterId].fgInUse)
    {
        return FALSE;
    }

    _SWDMX_Lock();

    _arFilter[u1FilterId].fgInUse = FALSE;

    _SWDMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Translate filter id to PID id
 *
 *  @param  u1FilterId      The filter id to be translated
 *  @param[out] pu1Pidx     The translated PID id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_FilterIdToPidx(UINT8 u1FilterId, UINT8* pu1Pidx)
{
    UINT8 u1Pidx;

    ASSERT(pu1Pidx != NULL);

    if (u1FilterId >= NUM_FILTER)
    {
    	LOG(1, "_SWDMX_FilterIdToPidx u1FilterId %d >= NUM_FILTER\n", u1FilterId);
        return FALSE;
    }

    if (!_arFilter[u1FilterId].fgInUse)
    {
    	LOG(1, "_SWDMX_FilterIdToPidx _arFilter fgInUse FALSE\n");
        return FALSE;
    }

    // Get pidx
    u1Pidx = _arFilter[u1FilterId].u1Pidx;
    if(u1Pidx >= NUM_PID)
    {
    	LOG(1, "_SWDMX_FilterIdToPidx u1Pidx %d >= NUM_PID\n", u1Pidx);
        return FALSE;
    }
    if(!_arPid[u1Pidx].fgInUse)
    {
    	LOG(1, "_SWDMX_FilterIdToPidx u1Pidx fgInUse FALSE\n");
        return FALSE;
    }

    *pu1Pidx = u1Pidx;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** The internal section callback handler
 *
 *  @param  u1Pidx          The PID id
 *  @param  eCode           The notification code
 *  @param  u4Data          The data. Its meaning depends on the notification
 *                          code.
 *  @param  pvNotifyTag     Tag that user registered
 *
 *  @return                 This function always returns TRUE
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_SectionHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag)
{
    DMX_NOTIFY_INFO_PSI_T* prNotifyInfo;
    SWDMX_DMX_MSG_T* prMsg;
    UINT8 u1FilterId, u1MsgId;

    // Check notification code
    if (eCode != DMX_NOTIFY_CODE_PSI)
    {
        return TRUE;
    }

    // Check if PID is really active
    if(u1Pidx >= NUM_PID)
    {
        ASSERT(u1Pidx < NUM_PID);
        return TRUE;
    }
    
    if (!_arPid[u1Pidx].fgInUse)
    {
        return TRUE;
    }

    // Check if corresponding filter is really active
    u1FilterId = _arPid[u1Pidx].u1FilterId;
    if (u1FilterId >= NUM_FILTER)
    {
        return TRUE;
    }
    if (!_arFilter[u1FilterId].fgInUse)
    {
        return TRUE;
    }

    // Check if user handler is registered
    if (_arPid[u1Pidx].pfnHandler == NULL)
    {
        return TRUE;
    }

    // Allocate message
    if (!_SWDMX_AllocateMessage(&u1MsgId))
    {
        return TRUE;
    }

    // Check if MSG is really active
    if(u1MsgId >= NUM_MESSAGE)
    {
        ASSERT(u1MsgId < NUM_MESSAGE);
        return TRUE;
    }
    
    // Store message
    prNotifyInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);
    prMsg = &_arMsg[u1MsgId].rMsg;
    prMsg->eType = DMX_MSG_TYPE_PSI;
    prMsg->u1Id = u1FilterId;
    ASSERT(prNotifyInfo->u1SerialNumber <= DMX_MAX_SERIAL_NUM);
    prMsg->rData.u1SerialNumber = prNotifyInfo->u1SerialNumber;
    prMsg->rData.u4FrameAddr = VIRTUAL(prNotifyInfo->u4SecAddr);
    prMsg->rData.u4Size = prNotifyInfo->u4SecLen;
    prMsg->pvNotifyTag = pvNotifyTag;

    // Call to user handler
    _arPid[u1Pidx].pfnHandler((INT32)u1MsgId);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** The internal PES callback handler
 *
 *  @param  u1Pidx          The PID id
 *  @param  eCode           The notification code
 *  @param  u4Data          The data. Its meaning depends on the notification
 *                          code.
 *  @param  pvNotifyTag     Tag that user registered
 *
 *  @return                 This function always returns TRUE
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_PesHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag)
{
    DMX_NOTIFY_INFO_PES_T* prNotifyInfo;
    SWDMX_DMX_MSG_T* prMsg;
    UINT8 u1MsgId;

    /*
    if(pvNotifyTag == NULL)
    {
    	LOG(1, "pvNotifyTag NULL\n");
        return SWDMX_E_INV_ARG;
    }*/
    
    // Check notification code
    if (eCode != DMX_NOTIFY_CODE_PES)
    {
        return TRUE;
    }

    // Check if PID is really active
    if(u1Pidx >= NUM_PID)
    {
        ASSERT(u1Pidx < NUM_PID);
        return TRUE;
    }
    
    if (!_arPid[u1Pidx].fgInUse)
    {
        return TRUE;
    }

    // Check if user handler is registered
    if (_arPid[u1Pidx].pfnHandler == NULL)
    {
        return TRUE;
    }

    // Allocate message
    if (!_SWDMX_AllocateMessage(&u1MsgId))
    {
        return TRUE;
    }

    // Check if MSG is really active
    if(u1MsgId >= NUM_MESSAGE)
    {
        ASSERT(u1MsgId < NUM_MESSAGE);
        return TRUE;
    }
    
    // Store message
    prNotifyInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);
    prMsg = &_arMsg[u1MsgId].rMsg;
    prMsg->eType = DMX_MSG_TYPE_PES;
    prMsg->u1Id = u1Pidx;
    ASSERT(prNotifyInfo->u1SerialNumber <= DMX_MAX_SERIAL_NUM);
    prMsg->rData.u1SerialNumber = prNotifyInfo->u1SerialNumber;
    prMsg->rData.u4FrameAddr = VIRTUAL(prNotifyInfo->u4DataAddr);
    prMsg->rData.u4Size = prNotifyInfo->u4DataSize;
    prMsg->pvNotifyTag = pvNotifyTag;

    // Call to user handler
    _arPid[u1Pidx].pfnHandler((INT32)u1MsgId);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-driver functionss
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** Allocate a PID slot
 *
 *  @param[out] pu1Pidx     The allocated PID id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DmxAllocatePid(UINT8* pu1Pidx)
{
    BOOL fgRet;
    UINT32 i;

    if(pu1Pidx == NULL)
    {
    	LOG(1, "pu1Pidx NULL\n");
        return FALSE;
    }

    fgRet = FALSE;

    _SWDMX_Lock();

    if(*pu1Pidx < NUM_PID)
    {
        if(!_arPid[*pu1Pidx].fgInUse)
        {
            _arPid[*pu1Pidx].fgInUse = TRUE;
            fgRet = TRUE;
        }
    }
    if(!fgRet)
    {
        for (i = 0; i < NUM_PID; i++)
        {
            if (!_arPid[i].fgInUse)
            {
                _arPid[i].fgInUse = TRUE;
                *pu1Pidx = (UINT8)i;
                fgRet = TRUE;
                break;
            }
        }
    }

    _SWDMX_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Release a PID slot
 *
 *  @param  u1Pidx          The PID id to be released
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DmxReleasePid(UINT8 u1Pidx)
{
    if (u1Pidx >= NUM_PID)
    {
        return FALSE;
    }

    if (!_arPid[u1Pidx].fgInUse)
    {
        return FALSE;
    }

    _SWDMX_Lock();

    _arPid[u1Pidx].fgInUse = FALSE;

    _SWDMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** Allocate and create a section filter
 *
 *  @param  prFilter        The structure of filter parameters to be created
 *  @param[out]
 *          pu1FilterId     Pointer to the allocated filter id
 *
 *  @retval SWDMX_E_OK                Successful
 *  @retval SWDMX_E_OUT_OF_MEMORY     Out of memory
 *  @retval SWDMX_E_FAIL              Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPsiAllocateFilter(SWDMX_DMX_FILTER_T* prFilter,
    UINT8* pu1FilterId)
{
    DMX_PID_T rPid;
    DMX_FILTER_T rFilter;
    UINT32 u4Flags;
    UINT8 au1Data[8], au1Mask[8];
    UINT8 u1Pidx, u1FilterId, u1PatternLength;

    u1FilterId = 0;
    u1Pidx = 0;
    
    if((prFilter == NULL) || (pu1FilterId == NULL))
    {
    	LOG(1, "prFilter NULL\n");
        return SWDMX_E_INV_ARG;
    }
    
    // Allocate pidx
    if (!_SWDMX_DmxAllocatePid(&u1Pidx))
    {
        return SWDMX_E_OUT_OF_MEMORY;
    }

    if(u1Pidx >= NUM_PID)
    {
        ASSERT(u1Pidx < NUM_PID);
        return SWDMX_E_OUT_OF_MEMORY;
    }
    
    // Allocate filter
    if (!_SWDMX_AllocateFilter(&u1FilterId))
    {
        // Todo: release pid
        return SWDMX_E_OUT_OF_MEMORY;
    }

    if(u1FilterId >= NUM_FILTER)
    {
        ASSERT(u1FilterId < NUM_FILTER);
        return SWDMX_E_OUT_OF_MEMORY;
    }
    
    // Set PID
    x_memset(&rPid, 0, sizeof(rPid));
    rPid.u1TsIndex = 0;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.u2Pid = prFilter->u2Pid;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = prFilter->u4BufferSize;
    rPid.ePidType = DMX_PID_TYPE_PSI;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = _SWDMX_SectionHandler;
    rPid.pvNotifyTag = prFilter->pvNotifyTag;

    u4Flags = (UINT32)(DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID | DMX_PID_FLAG_BUFFER |
        DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_STEER | DMX_PID_FLAG_DEVICE_ID |
        DMX_PID_FLAG_KEY_INDEX | DMX_PID_FLAG_DESC_MODE);
#ifdef CC_MT5365    
    if (!DMX_SetFilePid(u1Pidx, u4Flags, &rPid))
#else
    if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
#endif
    {
        // Todo: release filter and pid
        return SWDMX_E_FAIL;
    }

    // Set filter
    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgCheckCrc = prFilter->fgCheckCrc;
    rFilter.u1Pidx = u1Pidx;
    rFilter.u1Offset = prFilter->u1Offset;
    rFilter.eMode = DMX_FILTER_MODE_POS_NEG;
    u4Flags = (UINT32)(DMX_FILTER_FLAG_OFFSET | DMX_FILTER_FLAG_CRC |
        DMX_FILTER_FLAG_MODE | DMX_FILTER_FLAG_PATTERN | DMX_FILTER_FLAG_PIDX);

    u1PatternLength = (UINT8)(MIN(8, prFilter->u1PatternLength));

    // Clean all masks
    x_memset(au1Data, 0, 8);
    x_memset(au1Mask, 0, 8);

    // Convert byte patterns to words
    x_memcpy(au1Data, prFilter->pu1Pattern, u1PatternLength);
    x_memcpy(au1Mask, prFilter->pu1PositiveMask, u1PatternLength);

	rFilter.au4Data[0] = (au1Data[0] << 24) | (au1Data[1] << 16) |
	    (au1Data[2] << 8) | au1Data[3];
	rFilter.au4Data[1] = (au1Data[4] << 24) | (au1Data[5] << 16) |
		(au1Data[6] << 8) | au1Data[7];

	rFilter.au4Mask[0] = (au1Mask[0] << 24) | (au1Mask[1] << 16) |
	    (au1Mask[2] << 8) | au1Mask[3];
	rFilter.au4Mask[1] = (au1Mask[4] << 24) | (au1Mask[5] << 16) |
		(au1Mask[6] << 8) | au1Mask[7];

    // Set positive pattern to bank 0
    if (!DMX_SetFilter(u1FilterId, 0, u4Flags, &rFilter))
    {
        // Todo: reset pid, release filter & pid
        return SWDMX_E_FAIL;
    }

    // Convert byte patterns to words
    if (prFilter->pu1NegativeMask != NULL)
    {
        x_memcpy(au1Mask, prFilter->pu1NegativeMask, u1PatternLength);

    	rFilter.au4Mask[0] = (au1Mask[0] << 24) | (au1Mask[1] << 16) |
    	    (au1Mask[2] << 8) | au1Mask[3];
    	rFilter.au4Mask[1] = (au1Mask[4] << 24) | (au1Mask[5] << 16) |
    		(au1Mask[6] << 8) | au1Mask[7];
    }
    else
    {
        rFilter.au4Mask[0] = 0;
        rFilter.au4Mask[1] = 0;
    }

    // Set negtive mask to bank 1
    if (!DMX_SetFilter(u1FilterId, 1, u4Flags, &rFilter))
    {
        // Todo: reset filter & pid, release filter & pid
        return SWDMX_E_FAIL;
    }

    // Setup mapping from filter id to pidx
    _SWDMX_Lock();
    _arFilter[u1FilterId].u1Pidx = u1Pidx;
    _arPid[u1Pidx].pfnHandler = prFilter->pfnHandler;
    _arPid[u1Pidx].u1FilterId = u1FilterId;
    _SWDMX_Unlock();

    *pu1FilterId = u1FilterId;

    UNUSED(prFilter);
    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Release a section filter and its associated resources
 *
 *  @param  u1FilterId      The filter ID to be released
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPsiReleaseFilter(UINT8 u1FilterId)
{
    UINT8 u1Pidx;

    if (!_SWDMX_FilterIdToPidx(u1FilterId, &u1Pidx))
    {
        return SWDMX_E_INV_ARG;
    }

    // Release filter
    if (!_SWDMX_ReleaseFilter(u1FilterId))
    {
        return SWDMX_E_INV_ARG;
    }

    // Release PID
    if (!_SWDMX_DmxReleasePid(u1Pidx))
    {
        return SWDMX_E_INV_ARG;
    }

    // Free PID
    if (!DMX_FreePid(u1Pidx))
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Start a section filter that already been allocated (created)
 *
 *  @param  u1FilterId      The filter id to be started
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPsiStartFilter(UINT8 u1FilterId)
{
    DMX_PID_T rPid;
    DMX_FILTER_T rFilter;
    UINT8 u1Pidx;

    if (!_SWDMX_FilterIdToPidx(u1FilterId, &u1Pidx))
    {
        return SWDMX_E_INV_ARG;
    }

    // Enable filter
    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgEnable = TRUE;    
    if (!DMX_SetFilter(u1FilterId, 0, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return SWDMX_E_FAIL;
    }
    if (!DMX_SetFilter(u1FilterId, 1, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return SWDMX_E_FAIL;
    }

    // Enable PID
    x_memset(&rPid, 0, sizeof(rPid));
    rPid.fgEnable = TRUE;
#ifdef CC_MT5365      
    if (!DMX_SetFilePid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#else
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#endif
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Stop an active section filter
 *
 *  @param  u1FilterId      The filter id to be stopped
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPsiStopFilter(UINT8 u1FilterId)
{
    DMX_PID_T rPid;
    DMX_FILTER_T rFilter;
    UINT8 u1Pidx;

    if (!_SWDMX_FilterIdToPidx(u1FilterId, &u1Pidx))
    {
        return SWDMX_E_INV_ARG;
    }

    // Disable PID
    x_memset(&rPid, 0, sizeof(rPid));
    rPid.fgEnable = FALSE;
#ifdef CC_MT5365
    if (!DMX_SetFilePid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#else
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#endif
    {
        return SWDMX_E_FAIL;
    }

    // Disable filter
    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgEnable = FALSE;
    if (!DMX_SetFilter(u1FilterId, 0, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return SWDMX_E_FAIL;
    }
    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgEnable = FALSE;
    if (!DMX_SetFilter(u1FilterId, 1, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Get a section message from a message id. The message is released
 *  implicitly if this function returns successfully (SWDMX_E_OK)
 *
 *  @param  u1MsgId         The message id, which is casting from the i4Tag
 *                          parameter of filter callback handler registered in
 *                          SWDMX_DmxPsiAllocateFilter.
 *  @param[out]
 *          prMsg           The message. The eType member must be
 *                          DMX_MSG_TYPE_PSI, and u1Id member is filter id.
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_NOT_EXIST The message not exist
 *  @retval SWDMX_E_FAIL      Failed
 *
 *  @see    SWDMX_DmxPsiAllocateFilter
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPsiGetMessage(UINT8 u1MsgId, SWDMX_DMX_MSG_T* prMsg)
{
    ASSERT(prMsg != NULL);

    // Check if message is in-use
    if (u1MsgId >= NUM_MESSAGE)
    {
        return SWDMX_E_INV_ARG;
    }
    if (!_arMsg[u1MsgId].fgInUse)
    {
        return SWDMX_E_NOT_EXIST;
    }

    // Copy message
    *prMsg = _arMsg[u1MsgId].rMsg;

    // Release message
    if (!_SWDMX_ReleaseMessage(u1MsgId))
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Get data of a section filter
 *
 *  @param  u1FilterId      The filter id
 *  @param  prData          Contains parameters describling data address and
 *                          size to be copied
 *  @param[out] pu1Buf      The destination buffer. The size of the buffer must
 *                          be equal or larger than prData->u4Size
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPsiCopyData(UINT8 u1FilterId, SWDMX_DMX_DATA_T* prData,
    UINT8* pu1Buf)
{
    UINT8 u1Pidx;

    if ((pu1Buf == NULL) || (prData == NULL))
    {
        return SWDMX_E_INV_ARG;
    }

    if (!_SWDMX_FilterIdToPidx(u1FilterId, &u1Pidx))
    {
        return SWDMX_E_INV_ARG;
    }

    // Get data
    if (!DMX_PSI_GetBuffer(u1Pidx, prData->u1SerialNumber, prData->u4FrameAddr,
        0, prData->u4Size, pu1Buf))
    {
        return SWDMX_E_FAIL;
    }

    UNUSED(prData);
    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Unlock (release) section data. To simplify implementation, section data
 *  should be unlocked in the same order of calling to section filter callback
 *  handler.
 *
 *  @param  u1FilterId      The filter id
 *  @param  prData          Contains parameters describling data address and
 *                          size to be unlocked
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPsiUnlockData(UINT8 u1FilterId, SWDMX_DMX_DATA_T* prData)
{
    UINT8 u1Pidx;

    if (prData == NULL)
    {
        return SWDMX_E_INV_ARG;
    }

    if (!_SWDMX_FilterIdToPidx(u1FilterId, &u1Pidx))
    {
        return SWDMX_E_INV_ARG;
    }

    // Unlock data
    if (!DMX_PSI_UnlockBuffer(u1Pidx, prData->u1SerialNumber, prData->u4Size,
        prData->u4FrameAddr))
    {
        return SWDMX_E_FAIL;
    }

    UNUSED(prData);
    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Allocate and create a PES buffer. A PES buffer is usually used for
 * subtitle and teletext.
 *
 *  @param  prPes           The structure of PES parameters to be created
 *  @param[out]
 *          pu1Pidx         Pointer to the allocated PID id
 *
 *  @retval SWDMX_E_OK                Successful
 *  @retval SWDMX_E_OUT_OF_MEMORY     Out of memory
 *  @retval SWDMX_E_FAIL              Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPesAllocateBuffer(SWDMX_DMX_PES_T* prPes,
    UINT8* pu1Pidx, DMX_PID_TYPE_T ePidType)
{
    DMX_PID_T rPid;
    UINT32 u4Flags;
    UINT8 u1Pidx;
    FBM_POOL_T* prFbmPool;

    if((prPes == NULL) || (pu1Pidx == NULL))
    {
    	LOG(1, "pu1Pidx NULL\n");
        return SWDMX_E_INV_ARG;
    }

    u1Pidx = *pu1Pidx;
    if (!_SWDMX_DmxAllocatePid(&u1Pidx))
    {
        return SWDMX_E_OUT_OF_MEMORY;
    }

    if(u1Pidx >= NUM_PID)
    {
        ASSERT(u1Pidx < NUM_PID);
        return SWDMX_E_OUT_OF_MEMORY;
    }

    u4Flags = (UINT32)(DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID | DMX_PID_FLAG_BUFFER |
        DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_STEER | DMX_PID_FLAG_DEVICE_ID |
        DMX_PID_FLAG_KEY_INDEX | DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_PRIMARY);
    
    // Set PID
    x_memset((void*)&rPid, 0, sizeof(DMX_PID_T));
    rPid.u1TsIndex = prPes->u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = prPes->u1KeyIndex;
    rPid.u2Pid = prPes->u2Pid;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = prPes->u4BufferSize;
    rPid.ePidType = ePidType;
    rPid.eDescMode = (DMX_DESC_MODE_T)prPes->u4DescMode;
    rPid.pfnNotify = _SWDMX_PesHandler;
    rPid.pvNotifyTag = prPes->pvNotifyTag;
    rPid.fgPrimary = TRUE;

    if(ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];
#ifdef AUD_OLDFIFO_INTF
        VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
            &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
        VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
        rPid.fgAllocateBuffer= FALSE;
        rPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
        rPid.u4BufSize = 
            au4BufEnd[rPid.u1DeviceId] - au4BufStart[rPid.u1DeviceId];
    }
    else if(ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        if(prFbmPool == NULL)
        {
        	LOG(1, "FBM_GetPoolInfo Fail\n");
            return SWDMX_E_FAIL;
        }
        if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
        {
        	LOG(1, "prFbmPool->u4Addr/u4Size 0\n");
            return SWDMX_E_FAIL;
        }
        rPid.fgAllocateBuffer   = FALSE;
        rPid.u4BufAddr          = prFbmPool->u4Addr;
        rPid.u4BufSize          = prFbmPool->u4Size;
    }
    else if(ePidType == DMX_PID_TYPE_NONE)
    {
        // PCR
        rPid.fgPrimary = FALSE;
    }
    else
    {
    }

#ifdef CC_MT5365
    if (!DMX_SetFilePid(u1Pidx, u4Flags, &rPid))
#else
    if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
#endif
    {
        // Todo: release filter and pid
        return SWDMX_E_FAIL;
    }

    _SWDMX_Lock();
    _arPid[u1Pidx].pfnHandler = prPes->pfnHandler;
    _SWDMX_Unlock();

    *pu1Pidx = u1Pidx;

    UNUSED(prPes);
    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Release a PES buffer and its associated resources
 *
 *  @param  u1Pidx          The PID id
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPesReleaseBuffer(UINT8 u1Pidx)
{
    // Release PID
    if (!_SWDMX_DmxReleasePid(u1Pidx))
    {
        return SWDMX_E_INV_ARG;
    }

    // Free PID
    if (!DMX_FreePid(u1Pidx))
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Start a PES buffer
 *
 *  @param  u1Pidx          The PID id to be started
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPesStart(UINT8 u1Pidx)
{
    DMX_PID_T rPid;

    // Enable PID
    x_memset(&rPid, 0, sizeof(DMX_PID_T));
    rPid.fgEnable = TRUE;
#ifdef CC_MT5365
    if (!DMX_SetFilePid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#else
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#endif
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Stop an active PES buffer
 *
 *  @param  u1Pidx          The PID id to be stopped
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPesStop(UINT8 u1Pidx)
{
    DMX_PID_T rPid;

    // Disable PID
    x_memset(&rPid, 0, sizeof(DMX_PID_T));
    rPid.fgEnable = FALSE;
#ifdef CC_MT5365
    if (!DMX_SetFilePid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#else
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
#endif
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Get a PES message from a message id. The message is released implicitly if
 *  this function returns successfully (SWDMX_E_OK)
 *
 *  @param  u1MsgId         The message id, which is casting from the i4Tag
 *                          parameter of PES callback handler registered in
 *                          SWDMX_DmxPesAllocateBuffer
 *  @param[out]
 *          prMsg           The message. The eType member must be
 *                          DMX_MSG_TYPE_PES, and u1Id member is PID id.
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_NOT_EXIST The message not exist
 *  @retval SWDMX_E_FAIL      Failed
 *
 *  @see    SWDMX_DmxPesAllocateBuffer
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPesGetMessage(UINT32 u4MsgId, SWDMX_DMX_MSG_T* prMsg)
{
    ASSERT(prMsg != NULL);

    // Check if message is in-use
    if (u4MsgId >= NUM_MESSAGE)
    {
        return SWDMX_E_INV_ARG;
    }
    if (!_arMsg[u4MsgId].fgInUse)
    {
        return SWDMX_E_NOT_EXIST;
    }

    // Copy message
    *prMsg = _arMsg[u4MsgId].rMsg;

    // Release message
    if (_SWDMX_ReleaseMessage((UINT8)u4MsgId))
    {
        return SWDMX_E_FAIL;
    }

    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Get data from a PES buffer
 *
 *  @param  u1Pidx          The PID id
 *  @param  prData          Contains parameters describling data address and
 *                          size to be copied
 *  @param[out] pu1Buf      The destination buffer. The size of the buffer must
 *                          be equal or larger than prData->u4Size
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPesCopyData(UINT8 u1Pidx, SWDMX_DMX_DATA_T* prData,
    UINT8* pu1Buf)
{
    if ((prData == NULL) || (pu1Buf == NULL))
    {
        return SWDMX_E_INV_ARG;
    }

    if (!DMX_PES_GetBuffer(u1Pidx, prData->u1SerialNumber, prData->u4FrameAddr,
        0, prData->u4Size, pu1Buf))
    {
        return SWDMX_E_FAIL;
    }

    UNUSED(prData);
    return SWDMX_E_OK;
}


//-----------------------------------------------------------------------------
/** Unlock (release) PES data. To simplify implementation, PES data should be
 *  unlocked in the same order of calling to PES callback handler.
 *
 *  @param  u1Pidx          The PID id
 *  @param  prData          Contains parameters describling data address and
 *                          size to be unlocked
 *
 *  @retval SWDMX_E_OK        Successful
 *  @retval SWDMX_E_INV_ARG   Invalid argument
 *  @retval SWDMX_E_FAIL      Failed
 */
//-----------------------------------------------------------------------------
SWDMX_ERR_CODE_T SWDMX_DmxPesUnlockData(UINT8 u1Pidx, SWDMX_DMX_DATA_T* prData)
{
    if (prData == NULL)
    {
        return SWDMX_E_INV_ARG;
    }

    if (!DMX_PES_UnlockBuffer(u1Pidx, prData->u1SerialNumber, prData->u4Size,
        prData->u4FrameAddr))
    {
        return SWDMX_E_FAIL;
    }

    UNUSED(prData);
    return SWDMX_E_OK;
}
