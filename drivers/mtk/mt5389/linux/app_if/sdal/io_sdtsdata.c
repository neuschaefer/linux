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
 * $RCSfile: io_sdtsdata.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdtsdata.c
 *  SDAL kernel driver implementation: TS data
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "sd_dmx.h"
#include "SdTSData.h"
#include "dmx_if.h"
#include "x_assert.h"
#include "x_linux.h"
#include "x_os.h"
#include <linux/sdal_ioctl.h>
#include <linux/mt53xx_cb.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// Maximum section buffer size
#define MAX_SECTION_SIZE            32768

/// Maximum supported filter length
#define MAX_FILTER_LENGTH           8       // Maximum 8-byte of every filter

/// Invalid filter id
#define INVALID_FILTER_ID           0xff

/// Maximum PES buffer size
#define MAX_PES_SIZE                65536


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// Internal filter structure
typedef struct
{
    UINT16 u2TableId;               ///< Table id
    UINT8 u1Pidx;                   ///< The pid id used of this filter
    UINT8 u1VersionNumber;          ///< Current version number
    UINT8 au1Crc32[4];              ///< CRC32
    BOOL fgVersion;                 ///< Is callbacked only on version changing
    BOOL fgInUse;                   ///< Is this filter in-use or not
} _DMX_FILTER_T;

/// Pid type
typedef enum
{
    _DMX_PID_TYPE_NONE,
    _DMX_PID_TYPE_FILTER,
    _DMX_PID_TYPE_AV,
    _DMX_PID_TYPE_PES
} _DMX_PID_TYPE_T;

/// Internal PID structure
typedef struct
{
    BOOL fgInUse;                   ///< Is this pid in-use or not
    UINT8 u1FilterId;               ///< Filter id
    UINT16 u2Pid;                   ///< PID value
    _DMX_PID_TYPE_T eType;          ///< PID type
} _DMX_PID_T;

/// Section info
typedef struct
{
    UINT16 u2SectionLength;         ///< Section length
    UINT16 u2TableIdExtension;      ///< Table id extension
    UINT8 u1TableId;                ///< Table id
    UINT8 u1VersionNumber;          ///< Version number
    UINT8 u1SectionNumber;          ///< Section number
    UINT8 u1LastSectionNumber;      ///< Last section number
    UINT8 au1Crc32[4];              ///< CRC32
    BOOL fgSectionSyntaxIndicator;  ///< Section syntax indicator
    BOOL fgPrivateIndicator;        ///< Private indicator
    BOOL fgCurrentNextIndicator;    ///< Current next indicator
} _DMX_SECTION_INFO_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return SD_NOT_OK; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return SD_NOT_OK; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return SD_NOT_OK; \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Filter array to track filter availability and corresponding PID id
static _DMX_FILTER_T _arFilter[DMX_NUM_FILTER];

/// PID array to track PID availability and corresponding filter id
static _DMX_PID_T _arPid[DMX_NUM_PID];

/// Critical section state
static CRIT_STATE_T _rLock;

/// Tracking if nested critical section occurred
static BOOL _fgDmxLocking = FALSE;

/// Temporary section buffers
static UINT8 _au1SecBuf0[MAX_SECTION_SIZE];
static UINT8 _au1SecBuf1[MAX_SECTION_SIZE];

/// Temporary PEs buffers
static UINT8 _au1PesBuf1[MAX_PES_SIZE];

/// Is callback handler installed
static BOOL _fgInstallCallback = FALSE;

/// API semaphore
static HANDLE_T _hApiSem = (HANDLE_T)NULL;


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _Lock(void)
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
static void _Unlock(void)
{
    ASSERT(_fgDmxLocking);
    _fgDmxLocking = FALSE;

    x_crit_end(_rLock);

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** Enter demux API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _ApiLock(void)
{
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        // Create API semaphore
        VERIFY(x_sema_create(&_hApiSem, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK)
            == OSR_OK);

        _fgInit = TRUE;
    }

    VERIFY(x_sema_lock(_hApiSem, X_SEMA_OPTION_WAIT) == OSR_OK);
}


//-----------------------------------------------------------------------------
/** Leave demux API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _ApiUnlock(void)
{
    VERIFY(x_sema_unlock(_hApiSem) == OSR_OK);
}


//-----------------------------------------------------------------------------
/** Get filter ID by a given PID id
 *
 *  @param u1Pidx           PID id
 *  @param[out] pu1FilterId The filter id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           No active PID id with the given PID value
 */
//-----------------------------------------------------------------------------
static BOOL _GetFilterId(UINT8 u1Pidx, UINT8* pu1FilterId)
{
    UINT32 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1FilterId != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_FILTER; i++)
    {
        if (_arFilter[i].fgInUse && (_arFilter[i].u1Pidx == u1Pidx))
        {
            *pu1FilterId = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    _Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Get filter ID from table id
 *
 *  @param u1Pidx           PID id
 *  @param u1TableId        Table id
 *  @param[out] pu1FilterId The filter id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           No active PID id with the given PID value
 */
//-----------------------------------------------------------------------------
static BOOL _GetFilterIdFromTableId(UINT8 u1Pidx, UINT8 u1TableId,
    UINT8* pu1FilterId)
{
    UINT32 i;
    BOOL fgRet = FALSE;

    _Lock();

    for (i = 0; i < DMX_NUM_FILTER; i++)
    {
        if (_arFilter[i].fgInUse && (_arFilter[i].u2TableId <= 0xff) &&
            (_arFilter[i].u2TableId == (UINT16)u1TableId) &&
            (_arFilter[i].u1Pidx == u1Pidx))
        {
            *pu1FilterId = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    _Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Parse section structures
 *
 *  @param au1Buf           The section data
 *  @param u4Size           Section size
 *  @param[out] prSecInfo   The parsed section info
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _ParseSection(const UINT8* au1Buf, UINT32 u4Size,
    _DMX_SECTION_INFO_T* prSecInfo)
{
    if (u4Size < 3)
    {
        return FALSE;
    }

    if ((au1Buf == NULL) || (prSecInfo == NULL))
    {
        return FALSE;
    }

    x_memset(prSecInfo, 0, sizeof(_DMX_SECTION_INFO_T));

    prSecInfo->u1TableId = au1Buf[0];
    prSecInfo->fgSectionSyntaxIndicator = ((au1Buf[1] & 0x80) != 0);
    prSecInfo->fgPrivateIndicator = ((au1Buf[1] & 0x40) != 0);
    prSecInfo->u2SectionLength = ((au1Buf[1] & 0xf) << 8) | au1Buf[2];
    if (prSecInfo->fgSectionSyntaxIndicator)
    {
        if (u4Size < 8)
        {
            return FALSE;
        }

        prSecInfo->u2TableIdExtension = (au1Buf[3] << 8) | au1Buf[4];
        prSecInfo->u1VersionNumber = (au1Buf[5] & 0x3e) >> 1;
        prSecInfo->fgCurrentNextIndicator = ((au1Buf[5] & 1) != 0);
        prSecInfo->u1SectionNumber = au1Buf[6];
        prSecInfo->u1LastSectionNumber = au1Buf[7];
    }

    // CRC32
    prSecInfo->au1Crc32[0] = au1Buf[u4Size - 4];
    prSecInfo->au1Crc32[1] = au1Buf[u4Size - 3];
    prSecInfo->au1Crc32[2] = au1Buf[u4Size - 2];
    prSecInfo->au1Crc32[3] = au1Buf[u4Size - 1];

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Overflow handler of section buffer
 *
 *  @param  u1Pidx          The PID id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _OnBufferFull(UINT8 u1Pidx)
{
    DMX_PID_T rPid;

    // Get read/write pointers
    if (!DMX_GetPid(u1Pidx, DMX_PID_FLAG_DATA_POINTERS, &rPid))
    {
        return FALSE;
    }

    // Set read pointer to write pointer to make buffer empty
    if (!DMX_UpdateReadPointer(u1Pidx, rPid.u4Wp, rPid.u4Rp))
    {
        return FALSE;
    }

    // Buffer should be empty now
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
static BOOL _SectionHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag)
{
    _DMX_SECTION_INFO_T rSecInfo;
    SDAL_TSDATA_CB_T rTsDataCb;
    DMX_NOTIFY_INFO_PSI_T* prNotifyInfo;
    UINT32 u4Size, i;
    UINT8 u1FilterId, u1TableId;
    BOOL fgVersionChanged, fgCrcChanged;

    // Check if callback handler installed
    if (!_fgInstallCallback)
    {
        return FALSE;
    }

    // Check if PID is really active
    ASSERT(u1Pidx <= DMX_NUM_PID);
    if (!_arPid[u1Pidx].fgInUse)
    {
        return TRUE;
    }

	// Check notification code
	switch (eCode)
	{
	case DMX_NOTIFY_CODE_OVERFLOW:
		// Buffer overlow
		UNUSED(_OnBufferFull(u1Pidx));
		return TRUE;

	case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
		// Scramble state detected or changed
		return TRUE;

	case DMX_NOTIFY_CODE_PSI:
		// Receive section
		break;

	default:
		// Unexpected notification
		ASSERT(0);
		break;
	}

    // This pid should be a filter
    ASSERT(_arPid[u1Pidx].eType == _DMX_PID_TYPE_FILTER);

    prNotifyInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);

    // Find corresponding table id
    if (!DMX_PSI_GetBuffer(u1Pidx, prNotifyInfo->u1SerialNumber,
        prNotifyInfo->u4SecAddr, 0, 1, &u1TableId))
    {
        return FALSE;
    }

    // Get filter id from table id
    if (_GetFilterIdFromTableId(u1Pidx, u1TableId, &u1FilterId))
    {
        ASSERT(_arFilter[u1FilterId].u1Pidx == u1Pidx);

        if (_arFilter[u1FilterId].fgVersion)
        {
            // Get section data
            u4Size = prNotifyInfo->u4SecLen;
            if (!DMX_PSI_GetBuffer(u1Pidx, prNotifyInfo->u1SerialNumber,
                prNotifyInfo->u4SecAddr, 0, u4Size, _au1SecBuf0))
            {
                return FALSE;
            }

            // Parse section
            if (!_ParseSection(_au1SecBuf0, u4Size, &rSecInfo))
            {
                return FALSE;
            }

            // Check if version is changed
            fgVersionChanged = (rSecInfo.u1VersionNumber !=
                _arFilter[u1FilterId].u1VersionNumber);

            // Check if CRC is changed
            fgCrcChanged = (x_memcmp(_arFilter[u1FilterId].au1Crc32,
                rSecInfo.au1Crc32, 4) != 0);

            if (!fgVersionChanged && !fgCrcChanged)
            {
                // Same version number, same CRC32
                return FALSE;
            }

            // Record new version number and CRC
            _arFilter[u1FilterId].u1VersionNumber = rSecInfo.u1VersionNumber;
            for (i = 0; i < 4; i++)
            {
                _arFilter[u1FilterId].au1Crc32[i] = rSecInfo.au1Crc32[i];
            }
        }
    }
    else
    {
        u1FilterId = _arPid[u1Pidx].u1FilterId;
    }

    ASSERT(u1FilterId < DMX_NUM_FILTER);

    rTsDataCb.u1MonitorHandle = u1FilterId;
    rTsDataCb.u4Addr = prNotifyInfo->u4SecAddr;
    rTsDataCb.u4Size = prNotifyInfo->u4SecLen;
    rTsDataCb.u2Pid = _arPid[u1Pidx].u2Pid;
    rTsDataCb.u1Pidx = u1Pidx;
    rTsDataCb.u1SerialNumber = prNotifyInfo->u1SerialNumber;

    UNUSED(_CB_PutEvent(CB_SDAL_TSDATA_TRIGGER, sizeof (SDAL_TSDATA_CB_T),
        &rTsDataCb));

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
static BOOL _PesHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag)
{
    SDAL_TSDATA_CB_T rTsDataCb;
    DMX_NOTIFY_INFO_PES_T* prNotifyInfo;

    // Check if callback handler installed
    if (!_fgInstallCallback)
    {
        return FALSE;
    }

    // Check if PID is really active
    ASSERT(u1Pidx <= DMX_NUM_PID);
    if (!_arPid[u1Pidx].fgInUse)
    {
        return TRUE;
    }

	// Check notification code
	switch (eCode)
	{
	case DMX_NOTIFY_CODE_OVERFLOW:
		// Buffer overlow
//		UNUSED(_OnBufferFull(u1Pidx));
		return TRUE;

	case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
		// Scramble state detected or changed
		return TRUE;

	case DMX_NOTIFY_CODE_PES:
		// Receive PES packet
		break;

	default:
		// Unexpected notification
		ASSERT(0);
		break;
	}

    // This pid should be a PES
    ASSERT(_arPid[u1Pidx].eType == _DMX_PID_TYPE_PES);

    prNotifyInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);

    rTsDataCb.u1MonitorHandle = u1Pidx + DMX_NUM_FILTER;
    rTsDataCb.u4Addr = prNotifyInfo->u4DataAddr;
    rTsDataCb.u4Size = prNotifyInfo->u4DataSize;
    rTsDataCb.u2Pid = _arPid[u1Pidx].u2Pid;
    rTsDataCb.u1Pidx = u1Pidx;
    rTsDataCb.u1SerialNumber = prNotifyInfo->u1SerialNumber;

    UNUSED(_CB_PutEvent(CB_SDAL_TSDATA_TRIGGER, sizeof (SDAL_TSDATA_CB_T),
        &rTsDataCb));

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
static BOOL _AllocateFilter(UINT8* pu1FilterId)
{
    BOOL fgRet = FALSE;
    UINT32 i;

    ASSERT(pu1FilterId != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_FILTER; i++)
    {
        if (!_arFilter[i].fgInUse)
        {
            _arFilter[i].fgInUse = TRUE;
            *pu1FilterId = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    _Unlock();

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
static BOOL _ReleaseFilter(UINT8 u1FilterId)
{
    BOOL fgRet = FALSE;

    if (u1FilterId >= DMX_NUM_FILTER)
    {
        return FALSE;
    }

    _Lock();

    if (_arFilter[u1FilterId].fgInUse)
    {
        _arFilter[u1FilterId].fgInUse = FALSE;
        fgRet = TRUE;
    }

    _Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Allocate a PID slot
 *
 *  @param[out] pu1Pidx     The allocated PID id
 *  @param  u2Pid           PID value
 *  @param  u1FilterId      Filter id
 *  @param  eType           PID type
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _AllocatePidx(UINT8* pu1Pidx, UINT16 u2Pid, UINT8 u1FilterId,
    _DMX_PID_TYPE_T eType)
{
    BOOL fgRet = FALSE;
    UINT32 i;

    ASSERT(pu1Pidx != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_PID; i++)
    {
        if (!_arPid[i].fgInUse)
        {
            _arPid[i].fgInUse = TRUE;
            _arPid[i].u2Pid = u2Pid;
            _arPid[i].u1FilterId = u1FilterId;
            _arPid[i].eType = eType;

            *pu1Pidx = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    _Unlock();

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
static BOOL _ReleasePidx(UINT8 u1Pidx)
{
    BOOL fgRet = FALSE;

    if (u1Pidx >= DMX_NUM_PID)
    {
        return FALSE;
    }

    _Lock();

    if (_arPid[u1Pidx].fgInUse)
    {
        _arPid[u1Pidx].fgInUse = FALSE;
        _arPid[u1Pidx].u2Pid = 0xffff;
        _arPid[u1Pidx].eType = _DMX_PID_TYPE_NONE;

        fgRet = TRUE;
    }

    _Unlock();

    return fgRet;
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
static BOOL _FilterIdToPidx(UINT8 u1FilterId, UINT8* pu1Pidx)
{
    UINT8 u1Pidx;
    BOOL fgRet = FALSE;

    ASSERT(pu1Pidx != NULL);

    if (u1FilterId >= DMX_NUM_FILTER)
    {
        return FALSE;
    }

    _Lock();

    if (_arFilter[u1FilterId].fgInUse)
    {
        u1Pidx = _arFilter[u1FilterId].u1Pidx;
        ASSERT(u1Pidx <= DMX_NUM_PID);
        ASSERT(_arPid[u1Pidx].fgInUse);
        *pu1Pidx = u1Pidx;
        fgRet = TRUE;
    }

    _Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Get PID id by a given PID value in current filters
 *
 *  @param u2Pid            The PID value
 *  @param[out] pu1Pidx     PID id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           No active PID id with the given PID value
 */
//-----------------------------------------------------------------------------
static BOOL _GetPidxOfFilter(UINT16 u2Pid, UINT8* pu1Pidx)
{
    UINT32 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1Pidx != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_PID; i++)
    {
        if (_arPid[i].fgInUse && (_arPid[i].u2Pid == u2Pid) &&
            (_arPid[i].eType == _DMX_PID_TYPE_FILTER))
        {
            *pu1Pidx = i;
            fgRet = TRUE;
            break;
        }
    }

    _Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Start a PES monitor
 *
 *  @param prSettings       The monitor settings
 *  @param phM              Pointer to the monitor handle
 *
 *  @retval SD_OK           Successful
 *  @retval Others          Failed
 */
//-----------------------------------------------------------------------------
static INT32 _StartPesMonitor(SdTSData_Settings_t* prSettings,
    SdTSData_MonHandle* phM)
{
    DMX_PID_T rPid;
    SdTSData_MonHandle hM;
    UINT32 u4Flags;
    UINT8 u1Pidx;

    // Allocate a pidx
    if (!_AllocatePidx(&u1Pidx, (UINT16)prSettings->pid, INVALID_FILTER_ID,
        _DMX_PID_TYPE_PES))
    {
        return SD_NOT_OK;
    }

    // Set PID
    rPid.u1TsIndex = 0;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgPrimary = TRUE;
    rPid.u2Pid = (UINT16)prSettings->pid;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = MAX_PES_SIZE;
    rPid.ePidType = DMX_PID_TYPE_PES;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = _PesHandler;
    rPid.pvNotifyTag = NULL;

    u4Flags = DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID |
        DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_STEER |
        DMX_PID_FLAG_DEVICE_ID | DMX_PID_FLAG_KEY_INDEX |
        DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_PRIMARY;
    if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
    {
        // Todo: release pidx
        return SD_NOT_OK;
    }

    // Enable PID
    rPid.fgEnable = TRUE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return SD_NOT_OK;
    }

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)phM,
                   sizeof(SdTSData_MonHandle)))
    {
        return SD_NOT_OK;
    }

	hM = (SdTSData_MonHandle)(u1Pidx + DMX_NUM_FILTER);
    if (copy_to_user((void __user *)phM, &hM, sizeof(SdTSData_MonHandle)))
    {
        return SD_NOT_OK;
    }

    return SD_OK;
}


//-----------------------------------------------------------------------------
/** Stop a PES monitor
 *
 *  @param u1Pidx           Pidx
 *
 *  @retval SD_OK           Successful
 *  @retval Others          Failed
 */
//-----------------------------------------------------------------------------
static INT32 _StopPesMonitor(UINT8 u1Pidx)
{
    DMX_PID_T rPid;

    // Disable PID
    rPid.fgEnable = FALSE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return SD_NOT_OK;
    }

    // Release pidx
    if (!_DMX_ReleasePidx(u1Pidx))
    {
        return SD_NOT_OK;
    }

    // Free PID
    if (!DMX_FreePid(u1Pidx))
    {
        return SD_NOT_OK;
    }

    return SD_OK;
}


//-----------------------------------------------------------------------------
/** Init TSData module
 *
 *  @retval SD_OK           Successful
 *  @retval SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSData_Init(void)
{
    // Nothing to do here

    return SD_OK;
}


//-----------------------------------------------------------------------------
/** Install callback handler
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval SD_OK           Successful
 *  @retval SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSData_InstallCallback(UINT32 u4Arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDec_DmxHandle hDmx;
    SdTSData_CallBack pfnCallback;

    COPY_FROM_USER(&rArg2, u4Arg, sizeof (SDAL_IOCTL_2ARG_T));

    hDmx = (SdAVDec_DmxHandle)rArg2.au4Arg[0];
    pfnCallback = (SdTSData_CallBack)rArg2.au4Arg[1];

    // Check demux handle
    if (hDmx != (SdAVDec_DmxHandle)DMX_HANDLE)
    {
        // Incorrect demux handle
        return SD_NOT_OK;
    }

    _fgInstallCallback = TRUE;

    return SD_OK;
}


//-----------------------------------------------------------------------------
/** Uninstall callback handler
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval SD_OK           Successful
 *  @retval SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSData_UninstallCallback(UINT32 u4Arg)
{
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDec_DmxHandle hDmx;
    SdTSData_CallBack pfnCallback;

    COPY_FROM_USER(&rArg2, u4Arg, sizeof (SDAL_IOCTL_2ARG_T));

    hDmx = (SdAVDec_DmxHandle)rArg2.au4Arg[0];
    pfnCallback = (SdTSData_CallBack)rArg2.au4Arg[1];

    // Check demux handle
    if (hDmx != (SdAVDec_DmxHandle)DMX_HANDLE)
    {
        // Incorrect demux handle
        return SD_NOT_OK;
    }

    _fgInstallCallback = FALSE;

    return SD_OK;
}


//-----------------------------------------------------------------------------
/** Start a TSData monitor
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval SD_OK           Successful
 *  @retval SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSData_StartMonitor(UINT32 u4Arg)
{
    SDAL_IOCTL_4ARG_T rArg2;
    SdAVDec_DmxHandle hDmx;
    SdTSData_MonHandle* phM;
    SdTSData_MonHandle hM;
    SdTSData_Settings_t* prSettings;
    SdTSData_Settings_t rSettings;
    DMX_PID_T rPid;
    DMX_FILTER_T rFilter;
    UINT32 u4Flags, u4PatternLength, i;
    UINT8 au1Data[8], au1Mask[8];
    UINT8 u1FilterId, u1Pidx;
    BOOL fgNewPid;

    COPY_FROM_USER(&rArg2, u4Arg, sizeof (SDAL_IOCTL_4ARG_T));

    hDmx = (SdAVDec_DmxHandle)rArg2.au4Arg[0];
    prSettings = (SdTSData_Settings_t*)rArg2.au4Arg[1];
    phM = (SdTSData_MonHandle*)rArg2.au4Arg[2];
    if (prSettings == NULL)
    {
        return SD_NOT_OK;
    }

    COPY_FROM_USER(&rSettings, prSettings, sizeof (SdTSData_Settings_t));

    // Check demux handle
    if (hDmx != (SdAVDec_DmxHandle)DMX_HANDLE)
    {
        // Incorrect demux handle
        return SD_NOT_OK;
    }

    if (rSettings.eDataType == SD_TSDATA_TYPE_PES)
    {
        return _StartPesMonitor(&rSettings, phM);
    }

    // Allocate filter
    if (!_AllocateFilter(&u1FilterId))
    {
        // Out of memory
        return SD_NOT_OK;
    }

    // Get the pidx if there is one with the same PID value already
    fgNewPid = FALSE;
    if (!_GetPidxOfFilter((UINT16)rSettings.pid, &u1Pidx))
    {
        fgNewPid = TRUE;

        // Allocate a pidx if this a new PID value
        if (!_AllocatePidx(&u1Pidx, (UINT16)rSettings.pid, u1FilterId,
            _DMX_PID_TYPE_FILTER))
        {
            return SD_NOT_OK;
        }
    }

    if (fgNewPid)
    {
        // Set PID
        rPid.u1TsIndex = 0;
        rPid.u1DeviceId = 0;
        rPid.u1KeyIndex = 0;
        rPid.fgPrimary = TRUE;
        rPid.u2Pid = (UINT16)rSettings.pid;
        rPid.u1SteerMode = DMX_STEER_TO_FTUP;
        rPid.fgAllocateBuffer = TRUE;
        rPid.u4BufAddr = 0;
        rPid.u4BufSize = MAX_SECTION_SIZE;
        rPid.ePidType = DMX_PID_TYPE_PSI;
        rPid.eDescMode = DMX_DESC_MODE_NONE;
        rPid.pfnNotify = _SectionHandler;
        rPid.pvNotifyTag = NULL;

        u4Flags = DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID |
            DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_STEER |
            DMX_PID_FLAG_DEVICE_ID | DMX_PID_FLAG_KEY_INDEX |
            DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_PRIMARY;
        if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
        {
            // Todo: release filter and pid
            return SD_NOT_OK;
        }
    }

    // Set filter
    rFilter.fgCheckCrc = TRUE;
    rFilter.u1Pidx = u1Pidx;
    rFilter.u1Offset = 1;
    rFilter.eMode = DMX_FILTER_MODE_POS_NEG;
    rFilter.fgEnable = TRUE;
    u4Flags = DMX_FILTER_FLAG_OFFSET | DMX_FILTER_FLAG_CRC |
        DMX_FILTER_FLAG_MODE | DMX_FILTER_FLAG_PATTERN |
        DMX_FILTER_FLAG_PIDX |  DMX_FILTER_FLAG_VALID;

    switch (rSettings.eDataType)
    {
    case SD_TSDATA_TYPE_SECTION_TABLEID:
        u4PatternLength = 1;

        // Clean all masks
        x_memset(au1Data, 0, 8);
        x_memset(au1Mask, 0, 8);

        // Convert byte patterns to words
        au1Data[0] = (UINT8)rSettings.uParam.sTab._tableId;
        au1Mask[0] = 0xff;

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
            return SD_NOT_OK;
        }

        // Convert byte patterns to words
        rFilter.au4Mask[0] = 0;
        rFilter.au4Mask[1] = 0;

        // Set negtive mask to bank 1
        if (!DMX_SetFilter(u1FilterId, 1, u4Flags, &rFilter))
        {
            // Todo: reset filter & pid, release filter & pid
            return SD_NOT_OK;
        }
        break;

    case SD_TSDATA_TYPE_SECTION_FILTERED:
        u4PatternLength = (UINT32)MIN(rSettings.uParam.sFil._Filterlength,
            MAX_FILTER_LENGTH);

        // Clean all masks
        x_memset(au1Data, 0, 8);
        x_memset(au1Mask, 0, 8);

        COPY_FROM_USER(au1Data, rSettings.uParam.sFil._pFilterData,
            u4PatternLength);
        COPY_FROM_USER(au1Mask, rSettings.uParam.sFil._pMask, u4PatternLength);

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
            return SD_NOT_OK;
        }

        COPY_FROM_USER(au1Mask, rSettings.uParam.sFil._pInvert,
            u4PatternLength);

    	rFilter.au4Data[0] = (au1Data[0] << 24) | (au1Data[1] << 16) |
    	    (au1Data[2] << 8) | au1Data[3];
    	rFilter.au4Data[1] = (au1Data[4] << 24) | (au1Data[5] << 16) |
    		(au1Data[6] << 8) | au1Data[7];

    	rFilter.au4Mask[0] = (au1Mask[0] << 24) | (au1Mask[1] << 16) |
    	    (au1Mask[2] << 8) | au1Mask[3];
    	rFilter.au4Mask[1] = (au1Mask[4] << 24) | (au1Mask[5] << 16) |
    		(au1Mask[6] << 8) | au1Mask[7];

        // Set positive pattern to bank 0
        if (!DMX_SetFilter(u1FilterId, 1, u4Flags, &rFilter))
        {
            // Todo: reset pid, release filter & pid
            return SD_NOT_OK;
        }
        break;

    case SD_TSDATA_TYPE_PES:
        // Don't know what to do
        return SD_OK;

    default:
        // Unknown type
        return SD_NOT_OK;
    }

    _Lock();

    // Setup mapping from filter id to pidx
    _arFilter[u1FilterId].u1Pidx = u1Pidx;

    // Remember if only callback on version changing
    if (rSettings.eDataType == SD_TSDATA_TYPE_SECTION_TABLEID)
    {
        _arFilter[u1FilterId].fgVersion = rSettings.uParam.sTab._bVersion;
        _arFilter[u1FilterId].u2TableId = (UINT16)rSettings.uParam.sTab._tableId;;
    }
    else
    {
        _arFilter[u1FilterId].fgVersion = FALSE;
        _arFilter[u1FilterId].u2TableId = 0xffff;
    }

    // Reset initial version number and CRC
    _arFilter[u1FilterId].u1VersionNumber = 0xff;   // Max valid value is 31
    for (i = 0; i < 4; i++)
    {
        _arFilter[u1FilterId].au1Crc32[i] = 0xff;
    }

    _Unlock();

    // Enable PID
    rPid.fgEnable = TRUE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return SD_NOT_OK;
    }

    // Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *)phM,
                   sizeof(SdTSData_MonHandle)))
    {
        return SD_NOT_OK;
    }

	hM = (SdTSData_MonHandle)u1FilterId;
    if (copy_to_user((void __user *)phM, &hM, sizeof(SdTSData_MonHandle)))
    {
        return SD_NOT_OK;
    }

	return SD_OK;
}


//-----------------------------------------------------------------------------
/** Stop a TSData monitor
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval SD_OK           Successful
 *  @retval SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSData_StopMonitor(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    DMX_FILTER_T rFilter;
    SDAL_IOCTL_2ARG_T rArg2;
    SdAVDec_DmxHandle hDmx;
    SdTSData_MonHandle hMonitor;
    UINT8 u1MonitorHandle, u1FilterId, u1Pidx;

    COPY_FROM_USER(&rArg2, u4Arg, sizeof (SDAL_IOCTL_2ARG_T));

    hDmx = (SdAVDec_DmxHandle)rArg2.au4Arg[0];
    hMonitor = (SdTSData_MonHandle)rArg2.au4Arg[1];

    // Check demux handle
    if (hDmx != (SdAVDec_DmxHandle)DMX_HANDLE)
    {
        // Incorrect demux handle
        return SD_NOT_OK;
    }

    // Check monitor handle (filter id)
    u1MonitorHandle = (UINT8)hMonitor;
    if ((u1MonitorHandle >= DMX_NUM_FILTER) &&
        (u1MonitorHandle < (DMX_NUM_PID + DMX_NUM_FILTER)))
    {
        u1Pidx = u1MonitorHandle - DMX_NUM_FILTER;

        return _StopPesMonitor(u1Pidx);
    }
    else if (u1MonitorHandle >= (DMX_NUM_FILTER + DMX_NUM_PID))
    {
        // Incorrect filter id
        return SD_NOT_OK;
    }

    // Find corresponding pidx
    u1FilterId = u1MonitorHandle;
    if (!_FilterIdToPidx(u1FilterId, &u1Pidx))
    {
        return SD_NOT_OK;
    }

    // Disable filter
    rFilter.fgEnable = FALSE;
    if (!DMX_SetFilter(u1FilterId, 0, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return SD_NOT_OK;
    }
    if (!DMX_SetFilter(u1FilterId, 1, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return SD_NOT_OK;
    }

    // Release filter
    if (!_ReleaseFilter(u1FilterId))
    {
        return SD_NOT_OK;
    }

    // Check if there is other filters with the same PID
    if (!_GetFilterId(u1Pidx, &u1FilterId))
    {
	    // Disable PID
	    rPid.fgEnable = FALSE;
	    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
	    {
	        return SD_NOT_OK;
	    }

        // No more filter use this PID, release it
        if (!_DMX_ReleasePidx(u1Pidx))
        {
            return SD_NOT_OK;
        }

        // Free PID
        if (!DMX_FreePid(u1Pidx))
        {
            return SD_NOT_OK;
        }
    }

    return SD_OK;
}


//-----------------------------------------------------------------------------
/** Copy section data
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval SD_OK           Successful
 *  @retval SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSData_CopyData(UINT32 u4Arg)
{
    SDAL_IOCTL_8ARG_T rArg;
    UINT32 u4Addr, u4Size;
    UINT8 u1Pidx, u1SerialNumber;
    UINT8 *pu1SrcBuf, *pu1DstBuf;

    COPY_FROM_USER(&rArg, u4Arg, sizeof (SDAL_IOCTL_8ARG_T));

    // Get parameters
    u1Pidx = (UINT8)rArg.au4Arg[0];
    u4Addr = (UINT32)rArg.au4Arg[1];
    u4Size = (UINT32)rArg.au4Arg[2];
    u1SerialNumber = (UINT8)rArg.au4Arg[3];
    pu1DstBuf = (UINT8*)rArg.au4Arg[4];

    if (u1Pidx >= DMX_NUM_PID)
    {
        return SD_NOT_OK;
    }

    // Check if user buffer is valid
    if (!access_ok(VERIFY_WRITE, (void __user *)pu1DstBuf, u4Size))
    {
        return SD_NOT_OK;
    }

    // Copy data to local buffer
    if (_arPid[u1Pidx].eType == _DMX_PID_TYPE_PES)
    {
        ASSERT(u4Size <= MAX_PES_SIZE);
        pu1SrcBuf = _au1PesBuf1;
        if (!DMX_PES_GetBuffer(u1Pidx, u1SerialNumber, u4Addr, 0, u4Size,
            pu1SrcBuf))
        {
            return SD_NOT_OK;
        }
    }
    else
    {
        ASSERT(u4Size <= MAX_SECTION_SIZE);
        pu1SrcBuf = _au1SecBuf1;
        if (!DMX_PSI_GetBuffer(u1Pidx, u1SerialNumber, u4Addr, 0, u4Size,
            pu1SrcBuf))
        {
            return SD_NOT_OK;
        }
    }

    // Copy to user buffer
    if (copy_to_user((void __user *)pu1DstBuf, pu1SrcBuf, u4Size))
    {
        return SD_NOT_OK;
    }

    return SD_OK;
}


//-----------------------------------------------------------------------------
/** Unlock section data
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval SD_OK           Successful
 *  @retval SD_NOT_OK       Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSData_UnlockData(UINT32 u4Arg)
{
    SDAL_IOCTL_8ARG_T rArg;
    UINT32 u4Addr, u4Size;
    UINT8 u1Pidx, u1SerialNumber;

    COPY_FROM_USER(&rArg, u4Arg, sizeof (SDAL_IOCTL_8ARG_T));

    // Get parameters
    u1Pidx = (UINT8)rArg.au4Arg[0];
    u4Addr = (UINT32)rArg.au4Arg[1];
    u4Size = (UINT32)rArg.au4Arg[2];
    u1SerialNumber = (UINT8)rArg.au4Arg[3];
    if (u1Pidx >= DMX_NUM_PID)
    {
        return SD_NOT_OK;
    }

    // Unlock data
    if (_arPid[u1Pidx].eType == _DMX_PID_TYPE_PES)
    {
        if (!DMX_PES_UnlockBuffer(u1Pidx, u1SerialNumber, u4Size, u4Addr))
        {
            return SD_NOT_OK;
        }
    }
    else
    {
        if (!DMX_PSI_UnlockBuffer(u1Pidx, u1SerialNumber, u4Size, u4Addr))
        {
            return SD_NOT_OK;
        }
    }

    return SD_OK;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Allocate a PID slot
 *
 *  @param[out] pu1Pidx     The allocated PID id
 *  @param  u2Pid           PID value
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AllocatePidx(UINT8* pu1Pidx, UINT16 u2Pid)
{
    return _AllocatePidx(pu1Pidx, u2Pid, INVALID_FILTER_ID, _DMX_PID_TYPE_AV);
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
BOOL _DMX_ReleasePidx(UINT8 u1Pidx)
{
    return _ReleasePidx(u1Pidx);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int sdal_ioctl_sdtsdata(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    _ApiLock();

    switch (cmd)
    {
    case SDAL_IO_TSDATA_INIT:
        i4Ret = _TSData_Init();
        break;

    case SDAL_IO_TSDATA_INSTALLCALLBACK:
        i4Ret = _TSData_InstallCallback(arg);
        break;

    case SDAL_IO_TSDATA_UNINSTALLCALLBACK:
        i4Ret = _TSData_UninstallCallback(arg);
        break;

    case SDAL_IO_TSDATA_STARTMONITOR:
        i4Ret = _TSData_StartMonitor(arg);
        break;

    case SDAL_IO_TSDATA_STOPMONITOR:
        i4Ret = _TSData_StopMonitor(arg);
        break;

    case SDAL_IO_TSDATA_COPYDATA:
        i4Ret = _TSData_CopyData(arg);
        break;

    case SDAL_IO_TSDATA_UNLOCKDATA:
        i4Ret = _TSData_UnlockData(arg);
        break;

    default:
        printk("SDTSData: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    _ApiUnlock();

    return i4Ret;
}


