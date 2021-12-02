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
 * $RCSfile: io_papi_fe_tsdmx.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_fe_tsdmx.c
 *  PAPI kernel driver implementation: TS demux
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "mpv_if.h"
#include "vdec_if.h"
#include "vdec_drvif.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "fbm_drvif.h"
#include "vdp_drvif.h"
#include "stc_drvif.h"
#include "x_assert.h"
#include "x_slist.h"
#include "x_hal_5381.h"
#include "papi.h"
#include "papi_types.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// Define POSITIVE_MODE_BIT if bit 1 in the mode array of _TSDMX_RequestFilter
/// means positive pattern, and bit 0 means negative pattern. Otherwise bit 1
/// in the mode array means negative pattern, bit 0 means positive pattern
///
//#define POSITIVE_MODE_BIT
#define TSDMX_DBG 0


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// Support only one demux handle currently
//#define DMX_HANDLE                  1

/// Number of filter supported
#define DMX_NUM_FILTER              DMX_NUM_FILTER_INDEX

/// Number of PID supported
#define DMX_NUM_PID                 DMX_NUM_PID_INDEX

/// Maximum section size
#define MAX_SECTION_SIZE            4096

/// Maximum PES size
#define MAX_PES_SIZE                65536

/// Maximum filter length
#define MAX_FILTER_LENGTH           16

/// Common error code
#define TM_FAIL                     -0x800


#define DMX_DEFAULT_FRAMER          1


/// Number of section logs per pid
//#define NUM_SECTION_LOG             128

/// Max/min serial numbers and the related flags
#define TSDMX_SERIAL_MASK           0x7FFF
#define TSDMX_SERIAL_INVALID        0x0000
#define TSDMX_SERIAL_MIN            0x0001
#define TSDMX_SERIAL_MAX            0x7FFF

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// Internal filter structure
// The internal filter structure has the data element u1Pidx for indexing into
// the internal PID strucutre.  This is a one-way mapping.  The internal PID
// strucutre has not way to index into the internal filter structure.
typedef struct
{
    UINT8 u1Pidx;                   ///< The pid idx used by this filter
    BOOL fgInUse;                   ///< Is this filter in-use or not?
    BOOL fgIsList;                  ///< Is it a list filter?
    UINT32 u4Timeout;               ///< Timeout in ms (only for list filter)
    UINT8 u1FilterIndex;            ///< filter index (only for timer and list filter)
} _IO_DMX_FILTER_T;

/// Internal PID structure
typedef struct
{
    UINT16 u2Pid;                   ///< PID value
    BOOL fgInUse;                   ///< Is this pid in-use or not?
    BOOL fgIsFilter;                ///< Is this pid used for filter?
    BOOL fgIsPes;                   ///< Is this pid used for PES?
    UINT8 au1Data[MAX_FILTER_LENGTH];   ///< filter data for PES
    UINT8 au1Mask[MAX_FILTER_LENGTH];   ///< filter mask for PES
} _IO_DMX_PID_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size)                                      \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size)))              \
    {                                                                       \
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Cannot read from address 0x%08X!\n", (unsigned int)(src)); \
        return TM_FAIL;                                                     \
    }                                                                       \
    if (copy_from_user((dst), (void __user *)(src), (size)))                \
    {                                                                       \
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to copy data to kernel space!\n");  \
        return TM_FAIL;                                                     \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size)                                        \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size)))             \
    {                                                                       \
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Cannot write to address 0x%08X!\n", (unsigned int)(dst)); \
        return TM_FAIL;                                                     \
    }                                                                       \
    if (copy_to_user((void __user *)(dst), (src), (size)))                  \
    {                                                                       \
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to copy data to user space!\n");    \
        return TM_FAIL;                                                     \
    }


//-----------------------------------------------------------------------------
// DBG_PRINT()
#if defined(TSDMX_DEBUG) && defined(__KERNEL__)
#define DBG_PRINT(format, ...)      printk("[TSDMX] " format, ## __VA_ARGS__)
#else
#define DBG_PRINT(format, ...)      {}
#endif  // defined(TSDMX_DEBUG) && defined(__KERNEL__)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Filter array to track filter availability and corresponding PID id
static _IO_DMX_FILTER_T _arIoFilter[DMX_NUM_FILTER];

/// Number of filters in use
static UINT32 _u4NumFilterInUse = 0;

/// PID array to track PID availability and corresponding filter id
static _IO_DMX_PID_T _arIoPid[DMX_NUM_PID];

/// Number of PID in use
static UINT32 _u4NumPidInUse = 0;

/// Critical section state
static CRIT_STATE_T _rLock;

/// Tracking if nested critical section occurred
static BOOL _fgDmxLocking = FALSE;

/// Temporary section buffer
static UINT8 _au1SecBuf[MAX_SECTION_SIZE];

/// Temporary PES buffer
static UINT8 _au1PesBuf[MAX_PES_SIZE];

/// API semaphore for serialized usage of Demux driver
static HANDLE_T _hApiSem = (HANDLE_T)NULL;

/// Video pidx
static UINT8 _u1VideoPidx = DMX_NUM_PID;

/// Audio pidx
static UINT8 _u1AudioPidx = DMX_NUM_PID;

/// Audio descriptive pidx
static UINT8 _u1AudioDescPidx = DMX_NUM_PID;

/// PCR pidx
static UINT8 _u1PcrPidx = DMX_NUM_PID;

/// Video pid
static UINT16 _u2VideoPid = 0xFFFF;

/// Audio pid
static UINT16 _u2AudioPid = 0xFFFF;

/// Audio descriptive pid
static UINT16 _u2AudioDescPid = 0xFFFF;

/// PCR pid
static UINT16 _u2PcrPid = 0xFFFF;

/// Audio format --- set by the upper-layer software
static AUD_FMT_T _eAudioFormat = AUD_TYPE_UNKNOWN;

/// Video format --- set by the upper-layer software
static ENUM_VDEC_FMT_T _eVdecFormat = VDEC_FMT_MAX;

/// Timer handler
// FIXME: put _ahTimer into _IO_DMX_FILTER_T.
static HANDLE_T _ahTimer[DMX_NUM_FILTER];

/// Debug count
#if TSDMX_DBG
static UINT32 _u4RequestCount = 0;
static UINT32 _u4CancelCount = 0;
static UINT32 _u4RequestSuccessCount = 0;
static UINT32 _u4CancelSuccessCount = 0;
#endif


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

/// Get current video source
EXTERN void _papi_src_src_GetCurrentVideoSelection(int *source, Nat32 *designator);


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
static BOOL _GetFilterId(UINT8 u1Pidx, UINT8 *pu1FilterId)
{
    UINT8 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1FilterId != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_FILTER; i++)
    {
        if (_arIoFilter[i].fgInUse && (_arIoFilter[i].u1Pidx == u1Pidx))
        {
            *pu1FilterId = i;
            fgRet = TRUE;
            break;
        }
    }

    _Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** Handler of pid stopped
 *
 *  @param  u1Pidx          The PID id
 *  @param  u2Pid           PID value
 *  @param  u4Type          Type of callback event
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _OnPidStopped(UINT8 u1Pidx, UINT16 u2Pid, UINT32 u4Type)
{
    TSDMX_CB_T rTsdmxCb;

    switch (u4Type)
    {
    case TSDMX_CB_VIDEOPIDSTOPPED:
    case TSDMX_CB_AUDIOPIDSTOPPED:
    case TSDMX_CB_AUDIODESCPIDSTOPPED:
    case TSDMX_CB_PCRPIDSTOPPED:
        break;
    default:
        return FALSE;
    }

    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Type = u4Type;
    rTsdmxCb.u1Pidx = u1Pidx;
    rTsdmxCb.u2Pid = u2Pid;

    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Overflow handler of section/PES buffers.
 *
 *  @param  u1Pidx          The PID id
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _OnBufferFull(UINT8 u1Pidx)
{
    TSDMX_CB_T rTsdmxCb;

    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Type = TSDMX_CB_OVERFLOW;
    rTsdmxCb.u1Pidx = u1Pidx;

    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Handler of scrambling state change
 *
 *  @param  u1Pidx          The PID id
 *  @param  eCode           The notification code
 *  @param  u4Data          Notification specific data
 *  @param  pvNotifyTag     Notification tag
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _ScrambleStateHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                                  UINT32 u4Data, const void *pvNotifyTag)
{
    TSDMX_CB_T rTsdmxCb;
    DMX_SCRAMBLE_STATE_T eState;
    INT32 i4MediaType, i4Status;

    ASSERT(eCode == DMX_NOTIFY_CODE_SCRAMBLE_STATE);

    // Determine media type
    if (u1Pidx == _u1VideoPidx)
    {
        i4MediaType = papi_fe_tsdmx_MediaTypeVideo;
    }
    else if (u1Pidx == _u1AudioPidx)
    {
        i4MediaType = papi_fe_tsdmx_MediaTypeAudio;
    }
    else if (u1Pidx == _u1AudioDescPidx)
    {
        i4MediaType = papi_fe_tsdmx_MediaTypeAudioDesc;
    }
    else
    {
        // Not A/V media, stop here
        return TRUE;
    }

    // Translate scramble state
    eState = (DMX_SCRAMBLE_STATE_T)u4Data;
    switch (eState)
    {
    case DMX_SCRAMBLE_STATE_CLEAR:
        i4Status = papi_fe_tsdmx_NotScrambled;
        break;

    case DMX_SCRAMBLE_STATE_SCRAMBLED:
        i4Status = papi_fe_tsdmx_Scrambled;
        break;

    default:
        i4Status = papi_fe_tsdmx_ScramblingUnknown;
        break;
    }

    // Pack media type and scramble state to u4Tag
    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Tag = (UINT32)((i4MediaType << 16) | i4Status);
    rTsdmxCb.u4Type = TSDMX_CB_SCRAMBLECHANGE;
    rTsdmxCb.u1Pidx = u1Pidx;

    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));

    UNUSED(pvNotifyTag);

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
    UINT32 u4Data, const void *pvNotifyTag)
{
    TSDMX_CB_T rTsdmxCb;
    DMX_NOTIFY_INFO_PSI_T* prInfo;
    UINT32 u4FilterMap;
    UINT16 u2Pid;
    BOOL fgInUse, fgIsFilter;

    ASSERT(u1Pidx < DMX_NUM_PID);

    _Lock();
    fgInUse    = _arIoPid[u1Pidx].fgInUse;
    fgIsFilter = _arIoPid[u1Pidx].fgIsFilter;
    u2Pid      = _arIoPid[u1Pidx].u2Pid;
    _Unlock();

    // Check if PID is really active
    if (!fgInUse)
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
        ASSERT(0);      // FIXME
        return TRUE;

    case DMX_NOTIFY_CODE_PSI:
        // Receive section
        break;

    default:
        ASSERT(0);      // FIXME
        break;
    }

    // This pid should be a filter
    ASSERT(fgIsFilter);
    ASSERT(u2Pid <= 0x1FFF);

    prInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prInfo != NULL);
    u4FilterMap = prInfo->u4MatchBitmap;

    if (u4FilterMap == 0)
    {
        DMX_PSI_UnlockBuffer(u1Pidx, prInfo->u1SerialNumber, prInfo->u4SecLen,
            prInfo->u4SecAddr);
        return TRUE;
    }

    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Type = TSDMX_CB_SECTION;
    rTsdmxCb.u4Addr = prInfo->u4SecAddr;
    rTsdmxCb.u4Len = prInfo->u4SecLen;
    rTsdmxCb.u2Pid = u2Pid;
    rTsdmxCb.u1Pidx = u1Pidx;
    rTsdmxCb.u1SerialNumber = prInfo->u1SerialNumber;
    rTsdmxCb.u4FilterMap = u4FilterMap;
    rTsdmxCb.u4Tag = 0;     // unused

    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** The internal PES callback handler.
 *  This is called by ISR, and there is no need to use critical sections.
 *
 *  @param  u1Pidx          The PID id
 *  @param  eCode           The notification code
 *  @param  u4Data          The data. Its meaning depends on the notification
 *                          code.
 *  @param  pvNotifyTag     Tag that user registered
 *
 *  @return                 TRUE on success, and FALSE on failure which also
 *                          discards PES data.
 */
//-----------------------------------------------------------------------------
static BOOL _PesHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void *pvNotifyTag)
{
    TSDMX_CB_T rTsdmxCb;
    DMX_NOTIFY_INFO_PES_T* prInfo;
    UINT32 i, u4DataAddr, u4FltDepth;
    UINT8 au1Data[MAX_FILTER_LENGTH], u1Serial;

    ASSERT(u1Pidx < DMX_NUM_PID);
    ASSERT(_arIoPid[u1Pidx].fgIsPes);

    // Check if PID is really active
    if (!_arIoPid[u1Pidx].fgInUse)
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
        ASSERT(0);      // FIXME
        return TRUE;

    case DMX_NOTIFY_CODE_PES:
        // Receive section
        break;

    default:
        ASSERT(0);      // FIXME
        break;
    }

    prInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
    ASSERT(prInfo != NULL);
    u1Serial = prInfo->u1SerialNumber;
    u4DataAddr = prInfo->u4DataAddr;

    // Perform PES filtering.
    // Copy data to local non-ring buffer for easier data manipulation.
    // TODO: Record the filter depth in RequestPes() and compare that length of
    //       data for better performance.
    u4FltDepth = prInfo->u4DataSize;
    if (u4FltDepth > MAX_FILTER_LENGTH)
    {
        u4FltDepth = MAX_FILTER_LENGTH;
    }

    if (!DMX_PES_GetBuffer(u1Pidx, u1Serial, u4DataAddr, 0, u4FltDepth, au1Data))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Error at %s():%d\n", __FUNCTION__, __LINE__);
        return FALSE;   // Discard the PES data by returning FALSE.
    }

    for (i = 0; i < u4FltDepth; i++)
    {
        if ((_arIoPid[u1Pidx].au1Mask[i] & _arIoPid[u1Pidx].au1Data[i]) !=
            (_arIoPid[u1Pidx].au1Mask[i] & au1Data[i]))
        {
            // Discard the PES data.
            DMX_PES_UnlockBuffer(u1Pidx, u1Serial, prInfo->u4DataSize, u4DataAddr);
            return TRUE;
        }
    }

    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Type = TSDMX_CB_PES;
    rTsdmxCb.u4Addr = u4DataAddr;
    rTsdmxCb.u4Len = prInfo->u4DataSize;
    rTsdmxCb.u2Pid = _arIoPid[u1Pidx].u2Pid;
    rTsdmxCb.u1Pidx = u1Pidx;
    rTsdmxCb.u1SerialNumber = u1Serial;
    rTsdmxCb.u4FilterMap = 0;
    rTsdmxCb.u4Tag = 0;     // unused

    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));

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
static BOOL _AllocateFilter(UINT8 *pu1FltIdx, BOOL fgIsList, UINT32 u4Timeout)
{
    BOOL fgRet = FALSE;
    UINT32 i;

    ASSERT(pu1FltIdx != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_FILTER; i++)
    {
        if (!(_arIoFilter[i].fgInUse))
        {
            _arIoFilter[i].fgInUse = TRUE;
            _arIoFilter[i].fgIsList = fgIsList;
            _arIoFilter[i].u4Timeout = u4Timeout;
            *pu1FltIdx = (UINT8)i;

            ASSERT(_u4NumFilterInUse < DMX_NUM_FILTER);
            _u4NumFilterInUse++;

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

    if (_arIoFilter[u1FilterId].fgInUse)
    {
        _arIoFilter[u1FilterId].fgInUse = FALSE;
        _arIoFilter[u1FilterId].fgIsList = FALSE;
        _arIoFilter[u1FilterId].u4Timeout = 0;
        _arIoFilter[u1FilterId].u1FilterIndex = DMX_NUM_FILTER;   // set to an invalid value

        ASSERT(_u4NumFilterInUse > 0);
        _u4NumFilterInUse--;

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
 *  @param  fgIsFilter      Is this pid a section filter or not?
 *  @param  fgIsPes         Is this pid a PES filter or not?
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _AllocatePidx(UINT8 *pu1Pidx, UINT16 u2Pid, BOOL fgIsFilter, BOOL fgIsPes)
{
    BOOL fgRet = FALSE;
    UINT8 i;

    ASSERT(pu1Pidx != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_PID; i++)
    {
        if (!(_arIoPid[i].fgInUse))
        {
            _arIoPid[i].fgInUse = TRUE;
            _arIoPid[i].u2Pid = u2Pid;
            _arIoPid[i].fgIsFilter = fgIsFilter;
            _arIoPid[i].fgIsPes = fgIsPes;

            ASSERT(_u4NumPidInUse < DMX_NUM_PID);
            _u4NumPidInUse++;

            *pu1Pidx = i;
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

    if (_arIoPid[u1Pidx].fgInUse)
    {
        _arIoPid[u1Pidx].u2Pid = 0xFFFF;
        _arIoPid[u1Pidx].fgInUse = FALSE;
        _arIoPid[u1Pidx].fgIsFilter = FALSE;
        _arIoPid[u1Pidx].fgIsPes = FALSE;

        ASSERT(_u4NumPidInUse > 0);
        _u4NumPidInUse--;

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
static BOOL _FilterIdToPidx(UINT8 u1FilterId, UINT8 *pu1Pidx)
{
    UINT8 u1Pidx;
    BOOL fgRet = FALSE;

    ASSERT(pu1Pidx != NULL);

    if (u1FilterId >= DMX_NUM_FILTER)
    {
        return FALSE;
    }

    _Lock();

    if (_arIoFilter[u1FilterId].fgInUse)
    {
        u1Pidx = _arIoFilter[u1FilterId].u1Pidx;
        ASSERT(u1Pidx < DMX_NUM_PID);
        ASSERT(_arIoPid[u1Pidx].fgInUse);
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
static BOOL _GetPidxOfFilter(UINT16 u2Pid, UINT8 *pu1Pidx)
{
    UINT8 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1Pidx != NULL);

    _Lock();

    for (i = 0; i < DMX_NUM_PID; i++)
    {
        if (_arIoPid[i].fgInUse && (_arIoPid[i].u2Pid == u2Pid) && _arIoPid[i].fgIsFilter)
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
/** Remove a filter
 *
 *  @param  u1Pidx          The pid id
 *  @param  u1FilterId      The filter id
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _RemoveFilter(UINT8 u1Pidx, UINT8 u1FilterId)
{
    DMX_PID_T rPid;
    DMX_FILTER_GENERIC_T rFilter;

    if (u1Pidx >= DMX_NUM_PID)
    {
        return FALSE;
    }

    if (u1FilterId >= DMX_NUM_FILTER)
    {
        return FALSE;
    }

    // Disable the generic section filter
    rFilter.fgEnable = FALSE;
    if (!DMX_SetGenFilter(u1FilterId, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return FALSE;
    }

    // Release filter
    if (!_ReleaseFilter(u1FilterId))
    {
        return FALSE;
    }

    // Check if there is any other filter associated the same PID.
    if (!_GetFilterId(u1Pidx, &u1FilterId))
    {
        // Disable PID
        rPid.fgEnable = FALSE;
        if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Error at %s():%d\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        // Free PID
        if (!DMX_FreePid(u1Pidx))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Error at %s():%d\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        // No more filter use this PID, release it.
        if (!_ReleasePidx(u1Pidx))
        {
            return FALSE;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _GetStcId
 *  Get STC ID
 *
 *  @param  u1DecoderId     Decoder id
 *
 *  @retval The STC ID
 */
//-----------------------------------------------------------------------------
static UINT8 _GetStcId(UINT8 u1DecoderId)
{
    UINT8 u1StcId;

    switch (u1DecoderId)
    {
    case 0:
        u1StcId = STC_SRC_A1;
        break;

    case 1:
        u1StcId = STC_SRC_A2;
        break;

    default:
        u1StcId = STC_SRC_NS;       // Invalid ID
        break;
    }

    return u1StcId;
}


//-----------------------------------------------------------------------------
/** _papi_MPEGVideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _papi_MPEGVideoCallback(const DMX_PES_MSG_T *prPes)
{
    VDEC_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    switch (prPes->u4FrameType)
    {
    case PIC_TYPE_I:
        rMpvPes.ucPicType = MPV_I_TYPE;
        break;

    case PIC_TYPE_P:
        rMpvPes.ucPicType = MPV_P_TYPE;
        break;

    case PIC_TYPE_B:
        rMpvPes.ucPicType = MPV_B_TYPE;
        break;

    default:
        ASSERT(0);
        break;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;
    rMpvPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _papi_H264VideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _papi_H264VideoCallback(const DMX_PES_MSG_T *prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    u4Addr = prPes->u4FrameAddr + 4;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.u4VldReadPtr = u4Addr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;
    rMpvPes.u1SliceCount = prPes->u1SliceCount;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}


#if 0
//-----------------------------------------------------------------------------
/** _papi_VC1VideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _papi_VC1VideoCallback(const DMX_PES_MSG_T *prPes)
{
    VDEC_PES_INFO_T rVdecPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rVdecPes, 0, sizeof(rVdecPes));

    rVdecPes.fgSeqHdr = prPes->fgSeqHeader;
    rVdecPes.fgGop = prPes->fgGop;
    rVdecPes.fgDtsValid = prPes->fgPtsDts;
    rVdecPes.ucMpvId = prPes->u1DeviceId;
    rVdecPes.ucEsId = prPes->u1Channel;
    rVdecPes.ucPicType = prPes->u4FrameType;
    rVdecPes.u4FifoStart = prPes->u4BufStart;
    rVdecPes.u4FifoEnd = prPes->u4BufEnd;
    rVdecPes.u4VldReadPtr = prPes->u4FrameAddr;
    rVdecPes.u4PTS = prPes->u4Pts;
    rVdecPes.u4DTS = prPes->u4Dts;
    rVdecPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs(&rVdecPes) == 1);

}
#endif


//-----------------------------------------------------------------------------
/** _papi_WMVVideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
/*****************************************************************************
static BOOL _papi_WMVVideoCallback(const DMX_PES_MSG_T *prPes)
{
    VDEC_PES_INFO_T rVdecPes;

    ASSERT(prPes != NULL);

    rVdecPes.fgSeqHdr = prPes->fgSeqHeader;
    rVdecPes.fgGop = prPes->fgGop;
    rVdecPes.fgDtsValid = prPes->fgPtsDts;
    rVdecPes.ucMpvId = prPes->u1DeviceId;
    rVdecPes.ucEsId = prPes->u1Channel;
    rVdecPes.ucPicType = prPes->u4FrameType;
    rVdecPes.u4FifoStart = prPes->u4BufStart;
    rVdecPes.u4FifoEnd = prPes->u4BufEnd;
    rVdecPes.u4VldReadPtr = prPes->u4FrameAddr;
    rVdecPes.u4PTS = prPes->u4Pts;
    rVdecPes.u4DTS = prPes->u4Dts;
    rVdecPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs(&rVdecPes) == 1);

}
*****************************************************************************/


//-----------------------------------------------------------------------------
/** _papi_VideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _papi_VideoCallback(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet = FALSE;

    switch(DMX_GetVideoType())
    {
    case DMX_VIDEO_MPEG:
        fgRet = _papi_MPEGVideoCallback(prPes);
        break;

    case DMX_VIDEO_MPEG4:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] MPEG-4 shall not appear in DTV!\n");
        fgRet = FALSE;
        break;

    case DMX_VIDEO_H264:
        fgRet = _papi_H264VideoCallback(prPes);
        break;

    case DMX_VIDEO_VC1:
        //fgRet = _papi_VC1VideoCallback(prPes);  // No VC1 in DTV.
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] MPEG-4 shall not appear in DTV!\n");
        fgRet = FALSE;
        break;

    /*****************************************
    case DMX_VIDEO_WMV7:
    case DMX_VIDEO_WMV8:
    case DMX_VIDEO_WMV9:
        fgRet = _papi_WMVVideoCallback(prPes);
        break;
    *****************************************/

    default:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Unknown video type!\n");
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


// This function is called in the ISR context.  Therefore, no need to call
// _Lock()/_Unlock() in this function.
static void _TimerAction(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    TSDMX_CB_T rTsdmxCb;
    _IO_DMX_FILTER_T *prFilter;
    UINT8 u1FltIdx;
    BOOL fgIsList;

    ASSERT(pv_tag != NULL);
    prFilter = (_IO_DMX_FILTER_T*)pv_tag;
    u1FltIdx = prFilter->u1FilterIndex;
    fgIsList = _arIoFilter[u1FltIdx].fgIsList;

    if (u1FltIdx >= DMX_NUM_FILTER)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Timer CB: filter index > %d.\n", DMX_NUM_FILTER);
        return;
    }

    if (!fgIsList)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Timer CB: not a list filter (idx: %u).\n", u1FltIdx);
        return;
    }

    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Type = TSDMX_CB_LISTFILTERTIMEOUT;
    rTsdmxCb.u1Pidx = _arIoFilter[u1FltIdx].u1Pidx;
    rTsdmxCb.u1FilterId = u1FltIdx;

    // Issue async notification
    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));
}


//-----------------------------------------------------------------------------
/** Set video pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_SetVideoPid(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    IOCTL_2ARG_T r2Arg;
    FBM_POOL_T* prFbmPool;
    UINT32 u4Flags;
    UINT16 u2Pid;
    UINT8 u1VideoPidx;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    u2Pid = (UINT16)r2Arg.au4Arg[0];

    if (_u2VideoPid != 0xFFFF)
    {
        printk("[TSDMX] Please free video PID %u before %u is set!\n", _u2VideoPid, u2Pid);
        return TM_ERR_INSTANCE_IN_USE;
    }

    // Allocate video pidx
    if (!_AllocatePidx(&u1VideoPidx, u2Pid, FALSE, FALSE))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No PID index is available!\n");
        return TM_ERR_NO_RESOURCES;
    }

    STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
   
    if (_eVdecFormat == VDEC_FMT_MAX)
    {
        return TM_ERR_BAD_PARAMETER;
    }
    VDEC_Stop(0);
    VERIFY(VDEC_Play(0, _eVdecFormat));

    // Allocate video buffer from FBM
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);

    // Setup pid attributes
    rPid.fgEnable = TRUE;
    rPid.fgAllocateBuffer = FALSE;
    rPid.fgPrimary = TRUE;
    rPid.u1TsIndex = DMX_DEFAULT_FRAMER;;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.u2Pid = u2Pid;
    rPid.u4BufAddr = prFbmPool->u4Addr;
    rPid.u4BufSize = prFbmPool->u4Size;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.pfnScramble = _ScrambleStateHandler;
    rPid.pvScrambleTag = NULL;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;

    // Setup and enable video pid
    u4Flags = DMX_PID_FLAG_ALL | DMX_PID_FLAG_SCRAMBLE_STATE;
    if (!DMX_SetPid(u1VideoPidx, u4Flags, &rPid))
    {
        _ReleasePidx(u1VideoPidx);
        return TM_FAIL;
    }

    _Lock();
    _u1VideoPidx = u1VideoPidx;
    _u2VideoPid = u2Pid;
    _Unlock();

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Set audio pid
 *  Audio team confirmed in Nov that PAPI will make use of 3 audio decoders.
 *  Thus, the audio device id and audio FIFO index are hard-coded in this file.
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_SetAudioPid(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    IOCTL_2ARG_T r2Arg;
    UINT32 au4BufStart[2], au4BufEnd[2];
    UINT32 u4Flags;
    UINT16 u2Pid;
    UINT8 u1AudioPidx;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    u2Pid = (UINT16)r2Arg.au4Arg[0];

    if (_u2AudioPid != 0xFFFF)
    {
        printk("[TSDMX] Please free audio PID %u before %u is set!\n", _u2AudioPid, u2Pid);
        return TM_ERR_INSTANCE_IN_USE;
    }

    // Stop audio decoder
    #ifndef CC_MT5391_AUD_3_DECODER
    VERIFY(AUD_DSPCmdStop(AUD_DEC_MAIN) == AUD_OK);
    #else
    VERIFY(AUD_DSPCmdStop(AUD_DEC_MAIN) == AUD_OK);
    VERIFY(AUD_DSPCmdStop(AUD_DEC_AUX) == AUD_OK);
    #endif

    // Allocate audio buffer
    if (AUD_OK != AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
        &au4BufStart[1], &au4BufEnd[1]))
    {
        return TM_FAIL;
    }
    if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
    {
        return TM_ERR_NO_RESOURCES;
    }

    // Allocate audio pidx
    if (!_AllocatePidx(&u1AudioPidx, u2Pid, FALSE, FALSE))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No PID index is available!\n");
        return TM_ERR_NO_RESOURCES;
    }

    STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
    
    // In PAPI, 3 audio decoders are used by default.
    // TV makes use of audio device id 1 and audio FIFO 1.

    // Setup pid attributes
    rPid.fgEnable = TRUE;
    rPid.fgAllocateBuffer = FALSE;
    rPid.fgPrimary = TRUE;
    rPid.u1TsIndex = DMX_DEFAULT_FRAMER;

    #ifndef CC_MT5391_AUD_3_DECODER
    rPid.u1DeviceId = AUD_DEC_MAIN;                            // Audio (Device ID 0)
    #else
    rPid.u1DeviceId = AUD_DEC_AUX;                            // Audio (Device ID 1)
    #endif

    rPid.u1KeyIndex = 0;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.u2Pid = u2Pid;

    #ifndef CC_MT5391_AUD_3_DECODER
    rPid.u4BufAddr = au4BufStart[AUD_DEC_MAIN];                             // audio buffer index 0
    #else
    rPid.u4BufAddr = au4BufStart[AUD_DEC_AUX];                             // audio buffer index 1
    #endif

    #ifndef CC_MT5391_AUD_3_DECODER
    rPid.u4BufSize = au4BufEnd[AUD_DEC_MAIN] - au4BufStart[AUD_DEC_MAIN];   // audio buffer index 0
    #else
    rPid.u4BufSize = au4BufEnd[AUD_DEC_AUX] - au4BufStart[AUD_DEC_AUX];   // audio buffer index 1
    #endif

    rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.pfnScramble = _ScrambleStateHandler;
    rPid.pvScrambleTag = NULL;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;

    // Setup and enable audio pid
    u4Flags = DMX_PID_FLAG_ALL | DMX_PID_FLAG_SCRAMBLE_STATE;
    if (!DMX_SetPid(u1AudioPidx, u4Flags, &rPid))
    {
        _ReleasePidx(u1AudioPidx);
        return TM_FAIL;
    }

    _Lock();
    _u1AudioPidx = u1AudioPidx;
    _u2AudioPid = u2Pid;
    _Unlock();

    if (_eAudioFormat == AUD_TYPE_UNKNOWN)
    {
        return TM_ERR_BAD_PARAMETER;
    }
    ADAC_Mute(FALSE);
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY(AUD_SetDecType(AUD_DEC_MAIN,AUD_STREAM_FROM_DIGITAL_TUNER, _eAudioFormat) == AUD_OK);
    VERIFY(AUD_DSPCmdPlay(AUD_DEC_MAIN) == AUD_OK);
#else
    VERIFY(AUD_SetDecType(AUD_DEC_AUX,AUD_STREAM_FROM_DIGITAL_TUNER, _eAudioFormat) == AUD_OK);
    VERIFY(AUD_DSPCmdPlay(AUD_DEC_AUX) == AUD_OK);
    VERIFY(AUD_SetDecType(AUD_DEC_MAIN,AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_PCM) == AUD_OK);
    VERIFY(AUD_DSPCmdPlay(AUD_DEC_MAIN) == AUD_OK);
#endif
    papi_fe_sound_nfy();


    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Set audio descriptive pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_SetAudioDescriptivePid(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    UINT16 u2Pid;
    UINT8 u1AudioDescPidx;
    DMX_PID_T rPid;
    UINT32 au4BufStart[2], au4BufEnd[2];
    UINT32 u4Flags;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    u2Pid = (UINT16)r2Arg.au4Arg[0];

    if (_u2AudioDescPid != 0xFFFF)
    {
        printk("[TSDMX] Please free audio descriptive PID %u before %u is set!\n", _u2AudioDescPid, u2Pid);
        return TM_ERR_INSTANCE_IN_USE;
    }

    // Allocate audio pidx
    if (!_AllocatePidx(&u1AudioDescPidx, u2Pid, FALSE, FALSE))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No PID index is available!\n");
        return TM_ERR_NO_RESOURCES;
    }

    // Select the Down-Mix channel for audio description
    //VERIFY(AUD_AudioDescriptionChlSel(AUD_DEC_THIRD, AUD_CHL_DOWNMIX) == AUD_OK);
    // 2 for LR + ch9, 10
    AUD_DspADModeSet(AUD_DEC_MAIN, 2);
    // 1 for enable
    AUD_DspADEnable(AUD_DEC_MAIN, 1);

    // Allocate audio descriptive buffer
    if (AUD_OK != AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
        &au4BufStart[1], &au4BufEnd[1]))
    {
        return TM_FAIL;
    }
    if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
    {
        return TM_ERR_NO_RESOURCES;
    }

    // In PAPI, 3 audio decoders are used by default.
    // Audio descriptive PID makes use of audio device id 2 and audio FIFO 0.

    // Setup pid attributes
    rPid.fgEnable = TRUE;
    rPid.fgAllocateBuffer = FALSE;
    rPid.fgPrimary = TRUE;
    rPid.u1TsIndex = DMX_DEFAULT_FRAMER;;
    rPid.u1DeviceId = AUD_DEC_THIRD;        // Audio Description (Device ID 2)
    rPid.u1KeyIndex = 0;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.u2Pid = u2Pid;
    rPid.u4BufAddr = au4BufStart[AUD_DEC_MAIN];
    rPid.u4BufSize = au4BufEnd[AUD_DEC_MAIN] - au4BufStart[AUD_DEC_MAIN];
    rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.pfnScramble = _ScrambleStateHandler;
    rPid.pvScrambleTag = NULL;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;

    // Setup and enable audio descriptive pid
    u4Flags = DMX_PID_FLAG_ALL | DMX_PID_FLAG_SCRAMBLE_STATE;
    if (!DMX_SetPid(u1AudioDescPidx, u4Flags, &rPid))
    {
        _ReleasePidx(u1AudioDescPidx);
        return TM_FAIL;
    }

    _Lock();
    _u1AudioDescPidx = u1AudioDescPidx;
    _u2AudioDescPid = u2Pid;
    _Unlock();

    VERIFY(AUD_SetDecType(AUD_DEC_THIRD,AUD_STREAM_FROM_DIGITAL_TUNER, _eAudioFormat) == AUD_OK);
    VERIFY(AUD_DSPCmdPlay(AUD_DEC_THIRD) == AUD_OK);

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Set PCR pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_SetPcrPid(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    IOCTL_2ARG_T r2Arg;
    UINT32 u4Flags;
    UINT16 u2Pid;
    UINT8 u1PcrPidx, u1StcId;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    u2Pid = (UINT16)r2Arg.au4Arg[0];

    // Allocate PCR pidx
    if (!_AllocatePidx(&u1PcrPidx, u2Pid, FALSE, FALSE))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No PID index is available!\n");
        return TM_ERR_NO_RESOURCES;
    }
    
    STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
    
    // Setup PCR pid attributes
    rPid.fgEnable = TRUE;
    rPid.fgAllocateBuffer = FALSE;
    rPid.fgPrimary = FALSE;
    rPid.u1TsIndex = DMX_DEFAULT_FRAMER;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.u1SteerMode = 0;
    rPid.u2Pid = u2Pid;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = 0;
    rPid.ePidType = DMX_PID_TYPE_NONE;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.pfnScramble = NULL;
    rPid.pvScrambleTag = NULL;
    rPid.ePcrMode = DMX_PCR_MODE_NEW;

    // Setup and enable PCR pid
    u4Flags = DMX_PID_FLAG_ALL | DMX_PID_FLAG_SCRAMBLE_STATE;
    if (!DMX_SetPid(u1PcrPidx, u4Flags, &rPid))
    {
        _ReleasePidx(u1PcrPidx);
        return TM_FAIL;
    }

    // Set mapping of PID index and STC id
    u1StcId = _GetStcId(0);
    UNUSED(DMX_SetStcSrc(u1PcrPidx, u1StcId));

    _Lock();
    _u1PcrPidx = u1PcrPidx;
    _u2PcrPid = u2Pid;
    _Unlock();

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Get video pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_GetVideoPid(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    UINT32* pu4Pid;
    UINT32 u4Pid;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    pu4Pid = (UINT32*)r2Arg.au4Arg[0];

    if (_u2VideoPid > 0x1FFF)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No valid video PID is set!\n");
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Copy data to user memory
    u4Pid = _u2VideoPid;
    COPY_TO_USER(pu4Pid, &u4Pid, sizeof(UINT32));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Get audio pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_GetAudioPid(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    UINT32* pu4Pid;
    UINT32 u4Pid;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    pu4Pid = (UINT32*)r2Arg.au4Arg[0];

    if (_u2AudioPid > 0x1FFF)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No valid audio PID is set!\n");
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Copy data to user memory
    u4Pid = _u2AudioPid;
    COPY_TO_USER(pu4Pid, &u4Pid, sizeof(UINT32));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Get audio descriptive pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_GetAudioDescriptivePid(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    UINT32* pu4Pid;
    UINT32 u4Pid;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    pu4Pid = (UINT32*)r2Arg.au4Arg[0];

    if (_u2AudioDescPid > 0x1FFF)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No valid audio descriptive PID is set!\n");
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Copy data to user memory
    u4Pid = _u2AudioDescPid;
    COPY_TO_USER(pu4Pid, &u4Pid, sizeof(UINT32));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Get PCR pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_GetPcrPid(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    UINT32* pu4Pid;
    UINT32 u4Pid;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    pu4Pid = (UINT32*)r2Arg.au4Arg[0];

    if (_u2PcrPid > 0x1FFF)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No valid PCR PID is set!\n");
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Copy data to user memory
    u4Pid = _u2PcrPid;
    COPY_TO_USER(pu4Pid, &u4Pid, sizeof(UINT32));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Stop video pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_StopVideoPid(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1Pidx;

    UNUSED(u4Arg);

    u1Pidx = _u1VideoPidx;
    u2Pid = _u2VideoPid;

    if ((u2Pid > 0x1FFF) || (u1Pidx >= DMX_NUM_PID))
    {
        // Video pid is not active.  Issue async notification.
        //VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_VIDEOPIDSTOPPED));
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    VDEC_Stop(0);

    // Stop video pid
    rPid.fgEnable = FALSE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return TM_FAIL;
    }

    // Free PID
    VERIFY(DMX_FreePid(u1Pidx));
    VERIFY(_ReleasePidx(u1Pidx));

    _Lock();
    _u1VideoPidx = DMX_NUM_PID;
    _u2VideoPid = 0xFFFF;
    _Unlock();

    // Issue async notification
    VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_VIDEOPIDSTOPPED));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Stop audio pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_StopAudioPid(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1Pidx;

    UNUSED(u4Arg);

    u1Pidx = _u1AudioPidx;
    u2Pid = _u2AudioPid;

    if ((u2Pid > 0x1FFF) || (u1Pidx >= DMX_NUM_PID))
    {
        // Audio pid is not active.  Issue async notification.
        //VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_AUDIOPIDSTOPPED));
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Stop audio pid
    rPid.fgEnable = FALSE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return TM_FAIL;
    }

    // Free PID
    VERIFY(DMX_FreePid(u1Pidx));
    VERIFY(_ReleasePidx(u1Pidx));

    _Lock();
    _u1AudioPidx = DMX_NUM_PID;
    _u2AudioPid = 0xFFFF;
    _Unlock();

    // Issue async notification
    VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_AUDIOPIDSTOPPED));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Stop audio descriptive pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_StopAudioDescriptivePid(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1Pidx;

    UNUSED(u4Arg);

    u1Pidx = _u1AudioDescPidx;
    u2Pid = _u2AudioDescPid;

    if ((u2Pid > 0x1FFF) || (u1Pidx >= DMX_NUM_PID))
    {
        // Audio pid is not active.  Issue async notification.
        //VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_AUDIODESCPIDSTOPPED));
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Stop audio descriptive pid
    rPid.fgEnable = FALSE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return TM_FAIL;
    }

    // Free PID
    VERIFY(DMX_FreePid(u1Pidx));
    VERIFY(_ReleasePidx(u1Pidx));

    _Lock();
    _u1AudioDescPidx = DMX_NUM_PID;
    _u2AudioDescPid = 0xFFFF;
    _Unlock();

    // Issue async notification
    VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_AUDIODESCPIDSTOPPED));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Stop PCR pid
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_StopPcrPid(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1Pidx;

    UNUSED(u4Arg);

    u1Pidx = _u1PcrPidx;
    u2Pid = _u2PcrPid;

    if ((u2Pid > 0x1FFF) || (u1Pidx >= DMX_NUM_PID))
    {
        // PCR pid is not active.  Issue async notification.
        // VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_PCRPIDSTOPPED));
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Stop PCR pid
    rPid.fgEnable = FALSE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return TM_FAIL;
    }

    // Free PID
    VERIFY(DMX_FreePid(u1Pidx));
    VERIFY(_ReleasePidx(u1Pidx));

    // Reset mapping of PID index and STC id
    UNUSED(DMX_SetStcSrc(u1Pidx, STC_SRC_NS));

    _Lock();
    _u1PcrPidx = DMX_NUM_PID;
    _u2PcrPid = 0xFFFF;
    _Unlock();

    // Issue async notification
    VERIFY(_OnPidStopped(u1Pidx, u2Pid, TSDMX_CB_PCRPIDSTOPPED));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Request a section filter.
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_RequestFilter(UINT32 u4Arg)
{
    IOCTL_12ARG_T r12Arg;
    DMX_PID_T rPid;
    DMX_FILTER_GENERIC_T rFilter;
    UINT8 u1FltIdx, u1Pidx;
    UINT16 u2Pid;
    UINT32 u4ListLen, u4Timeout, *pu4FltIdx;
    UINT8 *pu1Mode, au1Mode[MAX_FILTER_LENGTH];
    UINT8 *pu1Data, au1Data[MAX_FILTER_LENGTH];
    UINT8 *pu1Mask, au1Mask[MAX_FILTER_LENGTH];
    UINT32 u4Flags, u4FltDepth, u4FilterIdx, u4BufSize, i;
    BOOL fgIsList, fgNewPid, fgCrc;

#if TSDMX_DBG
    _u4RequestCount++;
#endif

    // Copy arguments from user space
    COPY_FROM_USER(&r12Arg, u4Arg, sizeof(r12Arg));

    // Get arguments
    u2Pid        = (UINT16)r12Arg.au4Arg[0];
    u4BufSize    =         r12Arg.au4Arg[1];
    fgCrc        =   (BOOL)r12Arg.au4Arg[2];
    //fgContinuous =   (BOOL)r12Arg.au4Arg[3];
    //cookie     = (UINT32)r12Arg.au4Arg[4];
    u4FltDepth   =         r12Arg.au4Arg[5];
    pu1Mode      = (UINT8*)r12Arg.au4Arg[6];
    pu1Data      = (UINT8*)r12Arg.au4Arg[7];
    pu1Mask      = (UINT8*)r12Arg.au4Arg[8];
    pu4FltIdx    =(UINT32*)r12Arg.au4Arg[9];
    u4ListLen    = (UINT32)r12Arg.au4Arg[10];
    u4Timeout    = (UINT32)r12Arg.au4Arg[11];

    fgIsList = (u4ListLen > 0) ? TRUE : FALSE;

    // Allocate filter
    if (!_AllocateFilter(&u1FltIdx, fgIsList, u4Timeout))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No filter is available!\n");
        return TM_ERR_NO_RESOURCES;
    }

    // Get the pidx if there is one with the same PID value already
    fgNewPid = FALSE;
    if (!_GetPidxOfFilter(u2Pid, &u1Pidx))
    {
        fgNewPid = TRUE;
        // Allocate a pidx if this is a new PID value
        if (!_AllocatePidx(&u1Pidx, u2Pid, TRUE, FALSE))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] No PID index is available!\n");
            return TM_ERR_NO_RESOURCES;
        }
    }

    if (fgNewPid)
    {
        // Set PID
        rPid.fgEnable = FALSE;
        rPid.u1TsIndex = DMX_DEFAULT_FRAMER;
        rPid.u1DeviceId = 0;
        rPid.u1KeyIndex = 0;
        rPid.fgPrimary = TRUE;
        rPid.u2Pid = u2Pid;
        rPid.u1SteerMode = DMX_STEER_TO_FTUP;
        rPid.fgAllocateBuffer = TRUE;
        rPid.u4BufAddr = 0;
        rPid.u4BufSize = u4BufSize;
        rPid.ePidType = DMX_PID_TYPE_PSI;
        rPid.eDescMode = DMX_DESC_MODE_NONE;
        rPid.pfnNotify = _SectionHandler;
        rPid.pvNotifyTag = NULL;

        // Note the PID is not enabled yet. (no DMX_PID_FLAG_VALID)
        u4Flags = DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID |
            DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_STEER |
            DMX_PID_FLAG_DEVICE_ID | DMX_PID_FLAG_KEY_INDEX |
            DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_PRIMARY;
        if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
        {
            VERIFY(_ReleaseFilter(u1FltIdx));
            VERIFY(_ReleasePidx(u1Pidx));
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to set PID (pidx: %u) at request filter\n", u1Pidx);
            return TM_FAIL;
        }
    }

    // Update filter attributes.
    _Lock();
    _arIoFilter[u1FltIdx].u1Pidx = u1Pidx;
    _arIoFilter[u1FltIdx].u1FilterIndex = u1FltIdx;
    _Unlock();

    // Check max filter length
    if (u4FltDepth > MAX_FILTER_LENGTH)
    {
        PAPI_FE_PRINT(KERN_WARNING "[TSDMX] FilterDepth %u > %d (pidx: %u)\n",
            (unsigned int)u4FltDepth, MAX_FILTER_LENGTH, u1Pidx);
        u4FltDepth = MAX_FILTER_LENGTH;
    }

    // Get pattern and mask
    x_memset(au1Data, 0, MAX_FILTER_LENGTH);
    x_memset(au1Mask, 0, MAX_FILTER_LENGTH);
    x_memset(au1Mode, 0, MAX_FILTER_LENGTH);
    COPY_FROM_USER(au1Data, pu1Data, u4FltDepth);
    COPY_FROM_USER(au1Mask, pu1Mask, u4FltDepth);
    COPY_FROM_USER(au1Mode, pu1Mode, u4FltDepth);

    // Set filter
    rFilter.fgEnable = TRUE;
    rFilter.fgCheckCrc = fgCrc;
    rFilter.u1Pidx = u1Pidx;
    rFilter.u1Offset = 3;
    u4Flags = DMX_FILTER_FLAG_ALL;
    for (i = 0; i < MAX_FILTER_LENGTH; i++)
    {
        rFilter.au1Data[i]   = au1Data[i];
        rFilter.au1Mask[i]   = au1Mask[i];
        rFilter.au1PosNeg[i] = au1Mode[i];
    }

    if (!DMX_SetGenFilter(u1FltIdx, u4Flags, &rFilter))
    {
        // Disable PID, free PID, release filter ID, and release pidx.
        // Since PID is not enabled, the next two lines are omitted.
        //rPid.fgEnable = FALSE;
        //VERIFY(DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid));
        VERIFY(DMX_FreePid(u1Pidx));
        VERIFY(_ReleaseFilter(u1FltIdx));
        VERIFY(_ReleasePidx(u1Pidx));
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to request filter %u!\n", u1FltIdx);
        return TM_FAIL;
    }

    // Note! The system will begin to receive section data as soon as the PID
    // is enabled.  Make sure the user-space glue layer is ready for receiving
    // notifications/data.
    if (fgNewPid)
    {
        rPid.fgEnable = TRUE;
        if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Cannot enable PID (pidx: %u)\n", u1Pidx);
            return TM_FAIL;
        }
    }

    // If a list filter is requested, create and start the associated timer.
    if (fgIsList)
    {
        BOOL fgStartTimer = TRUE;
        if (_ahTimer[u1FltIdx] == NULL_HANDLE)
        {
            if (x_timer_create(&(_ahTimer[u1FltIdx])) != OSR_OK)
            {
                PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to create timer %u!\n", u1FltIdx);
                fgStartTimer = FALSE;
            }
        }
        else
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Timer %u already exists!\n", u1FltIdx);
        }

        if (fgStartTimer)
        {
            if (x_timer_start(_ahTimer[u1FltIdx], u4Timeout, X_TIMER_FLAG_REPEAT,
                _TimerAction, &(_arIoFilter[u1FltIdx])) != OSR_OK)
            {
                PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to start timer %u!\n", u1FltIdx);
            }
        }
    }

    // Return filter idx to user
    u4FilterIdx = u1FltIdx;
    COPY_TO_USER(pu4FltIdx, &u4FilterIdx, sizeof(UINT32));

#if TSDMX_DBG
    _u4RequestSuccessCount++;
#endif

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Update a section filter
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_UpdateFilter(UINT32 u4Arg)
{
    IOCTL_8ARG_T r8Arg;
    DMX_FILTER_GENERIC_T rFilter;
    UINT32* pu4Ret;
    UINT32 u4Flags, u4FltDepth, u4Ret, i;
    UINT8 u1FltIdx, u1Pidx;
    UINT8 *pu1Mode, au1Mode[MAX_FILTER_LENGTH];
    UINT8 *pu1Data, au1Data[MAX_FILTER_LENGTH];
    UINT8 *pu1Mask, au1Mask[MAX_FILTER_LENGTH];

    // Copy arguments from user space
    COPY_FROM_USER(&r8Arg, u4Arg, sizeof(r8Arg));

    // Get arguments
    u1FltIdx   = (UINT8)r8Arg.au4Arg[0];
    u4FltDepth =        r8Arg.au4Arg[1];
    pu1Mode =   (UINT8*)r8Arg.au4Arg[2];
    pu1Data =   (UINT8*)r8Arg.au4Arg[3];
    pu1Mask =   (UINT8*)r8Arg.au4Arg[4];
    pu4Ret  =  (UINT32*)r8Arg.au4Arg[5];

    // Check filter id
    if (u1FltIdx >= DMX_NUM_FILTER)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Invalid filter idx %u!\n", u1FltIdx);
        return TM_ERR_BAD_PARAMETER;
    }

    // Find corresponding pidx
    if (!_FilterIdToPidx(u1FltIdx, &u1Pidx))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Cannot find pidx for filter idx %u!\n", u1FltIdx);
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Disable filter first
    rFilter.fgEnable = FALSE;
    if (!DMX_SetGenFilter(u1FltIdx, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to disable filter %u!\n", u1FltIdx);
        return TM_FAIL;
    }

    // Check max filter length
    if (u4FltDepth > MAX_FILTER_LENGTH)
    {
        u4FltDepth = MAX_FILTER_LENGTH;
    }

    // Get pattern and mask
    x_memset(au1Data, 0, MAX_FILTER_LENGTH);
    x_memset(au1Mask, 0, MAX_FILTER_LENGTH);
    x_memset(au1Mode, 0, MAX_FILTER_LENGTH);
    COPY_FROM_USER(au1Data, pu1Data, u4FltDepth);
    COPY_FROM_USER(au1Mask, pu1Mask, u4FltDepth);
    COPY_FROM_USER(au1Mode, pu1Mode, u4FltDepth);

    // Set filter
    rFilter.fgEnable = TRUE;
    //rFilter.fgCheckCrc = fgCrc;
    rFilter.u1Pidx = u1Pidx;
    rFilter.u1Offset = 3;
    u4Flags = DMX_FILTER_FLAG_VALID | DMX_FILTER_FLAG_PIDX |
              DMX_FILTER_FLAG_PATTERN | DMX_FILTER_FLAG_MODE;
    // DMX_FILTER_FLAG_OFFSET and DMX_FILTER_FLAG_CRC are not needed.
    // DMX_FILTER_FLAG_PIDX can also be omitted.
    for (i = 0; i < MAX_FILTER_LENGTH; i++)
    {
        rFilter.au1Data[i]   = au1Data[i];
        rFilter.au1Mask[i]   = au1Mask[i];
        rFilter.au1PosNeg[i] = au1Mode[i];
    }

    if (!DMX_SetGenFilter(u1FltIdx, u4Flags, &rFilter))
    {
        // Filter and PID shall not be freed when the filter cannot be updated.
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to update filter %u!\n", u1FltIdx);
        return TM_FAIL;
    }

    // Return TRUE to user
    u4Ret = (UINT32)TRUE;
    COPY_TO_USER(pu4Ret, &u4Ret, sizeof(UINT32));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Cancel a section filter
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_CancelFilter(UINT32 u4Arg)
{
    TSDMX_CB_T rTsdmxCb;
    IOCTL_2ARG_T r2Arg;
    UINT8 u1FltIdx, u1Pidx;
    BOOL fgIsList;
    UINT16 u2Serial;

#if TSDMX_DBG
    _u4CancelCount++;
#endif

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    u1FltIdx = (UINT8)r2Arg.au4Arg[0];
    u2Serial = (UINT16)(r2Arg.au4Arg[1] & TSDMX_SERIAL_MASK);

    if (u1FltIdx >= DMX_NUM_FILTER)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Invalid filter idx %u!\n", u1FltIdx);
        return TM_ERR_BAD_PARAMETER;
    }

    _Lock();
    fgIsList = _arIoFilter[u1FltIdx].fgIsList;
    _Unlock();

    if (fgIsList)   // FIXME: fgIsList && fgInUse???
    {
        if (_ahTimer[u1FltIdx] != NULL_HANDLE)
        {
            // A timer will be stopped before it is deleted.
            if (x_timer_delete(_ahTimer[u1FltIdx]) == OSR_OK)
            {
                _ahTimer[u1FltIdx] = NULL_HANDLE;
            }
            else
            {
                PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to delete timer %u!\n", u1FltIdx);
            }
        }
    }

    // Find corresponding pidx
    if (!_FilterIdToPidx(u1FltIdx, &u1Pidx))
    {
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Remove the filter and the associated PID
    if (!_RemoveFilter(u1Pidx, u1FltIdx))
    {
        return TM_FAIL;
    }

    // Issue async notification
    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Tag = u2Serial;
    rTsdmxCb.u1Pidx = u1Pidx;
    rTsdmxCb.u1FilterId = u1FltIdx;
    rTsdmxCb.u4Type = fgIsList ? TSDMX_CB_REQUESTLISTCANCELLED : TSDMX_CB_REQUESTCANCELLED;

    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));

#if TSDMX_DBG
    _u4CancelSuccessCount++;
#endif

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Request a section list filter.
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_RequestListFilter(UINT32 u4Arg)
{
    return _TSDMX_RequestFilter(u4Arg);
}


//-----------------------------------------------------------------------------
/** Request a PES filter
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_RequestPes(UINT32 u4Arg)
{
    DMX_PID_T rPid;
    IOCTL_8ARG_T r8Arg;
    UINT32 *pu4Pidx, i;
    UINT8 *pu1Data, au1Data[MAX_FILTER_LENGTH];
    UINT8 *pu1Mask, au1Mask[MAX_FILTER_LENGTH];
    UINT32 u4BufSize, u4FltDepth, u4Pidx, u4Flags;
    UINT16 u2Pid;
    UINT8 u1Pidx;

    // Copy arguments from user space
    COPY_FROM_USER(&r8Arg, u4Arg, sizeof(r8Arg));

    u2Pid   =  (UINT16)r8Arg.au4Arg[0];
    u4BufSize  =       r8Arg.au4Arg[1];
    //cookie = (UINT32)r8Arg.au4Arg[2];
    u4FltDepth =       r8Arg.au4Arg[3];
    pu1Data =  (UINT8*)r8Arg.au4Arg[4];
    pu1Mask =  (UINT8*)r8Arg.au4Arg[5];
    pu4Pidx = (UINT32*)r8Arg.au4Arg[6];

    // Allocate a pidx
    if (!_AllocatePidx(&u1Pidx, u2Pid, FALSE, TRUE))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] No PID index is available!\n");
        return TM_ERR_NO_RESOURCES;
    }

    // Set PID
    rPid.u1TsIndex = DMX_DEFAULT_FRAMER;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.fgPrimary = TRUE;
    rPid.u2Pid = u2Pid;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = u4BufSize;
    rPid.ePidType = DMX_PID_TYPE_PES;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.pfnNotify = _PesHandler;
    rPid.pvNotifyTag = NULL;

    u4Flags = DMX_PID_FLAG_VALID  | DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID   |
              DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_STEER |
              DMX_PID_FLAG_DEVICE_ID | DMX_PID_FLAG_KEY_INDEX |
              DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_PRIMARY;
    if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
    {
        VERIFY(_ReleasePidx(u1Pidx));
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Cannot set new PID %u (pidx: %u)\n", u2Pid, u1Pidx);
        return TM_FAIL;
    }

    // Check max filter length
    if (u4FltDepth > MAX_FILTER_LENGTH)
    {
        u4FltDepth = MAX_FILTER_LENGTH;
    }

    // Get pattern and mask
    x_memset(au1Data, 0, MAX_FILTER_LENGTH);
    x_memset(au1Mask, 0, MAX_FILTER_LENGTH);
    COPY_FROM_USER(au1Data, pu1Data, u4FltDepth);
    COPY_FROM_USER(au1Mask, pu1Mask, u4FltDepth);

    // Update PID attributes
    _Lock();
    for (i = 0; i < MAX_FILTER_LENGTH; i++)
    {
        _arIoPid[u1Pidx].au1Data[i] = au1Data[i];
        _arIoPid[u1Pidx].au1Mask[i] = au1Mask[i];
    }
    _Unlock();

    // Return the PID index of the PES filter to user.
    u4Pidx = u1Pidx;
    COPY_TO_USER(pu4Pidx, &u4Pidx, sizeof(UINT32));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Cancel a PES filter
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_CancelPes(UINT32 u4Arg)
{
    TSDMX_CB_T rTsdmxCb;
    DMX_PID_T rPid;
    IOCTL_2ARG_T r2Arg;
    UINT8 u1Pidx;
    UINT16 u2Serial;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    u1Pidx = (UINT8)r2Arg.au4Arg[0];
    u2Serial = (UINT16)(r2Arg.au4Arg[1] & TSDMX_SERIAL_MASK);

    // Check pid id
    if (u1Pidx >= DMX_NUM_PID)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Invalid pidx %u!\n", u1Pidx);
        return TM_ERR_BAD_PARAMETER;
    }

    if (!_arIoPid[u1Pidx].fgInUse)
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] pidx %u is not in use!\n", u1Pidx);
        return TM_ERR_RESOURCE_NOT_OWNED;
    }

    // Disable PID
    rPid.fgEnable = FALSE;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] fail to disable pidx %u!\n", u1Pidx);
        return TM_FAIL;
    }

    // Free PID
    if (!DMX_FreePid(u1Pidx))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] fail to free pidx %u!\n", u1Pidx);
        return TM_FAIL;
    }

    // Release pidx
    if (!_ReleasePidx(u1Pidx))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] fail to release pidx %u!\n", u1Pidx);
        return TM_FAIL;
    }

    // Issue async notification
    x_memset((void*)&rTsdmxCb, 0, sizeof(rTsdmxCb));
    rTsdmxCb.u4Tag = u2Serial;
    rTsdmxCb.u4Type = TSDMX_CB_PESCANCELLED;
    rTsdmxCb.u1Pidx = u1Pidx;
    UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof(TSDMX_CB_T), &rTsdmxCb));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Get scrambling state
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_GetScramblingStatus(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    DMX_SCRAMBLE_STATE_T eState;
    INT32 i4MediaType, i4State;
    INT32* pi4Ret;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    // Get arguments
    i4MediaType =  (INT32)r2Arg.au4Arg[0];
    pi4Ret      = (INT32*)r2Arg.au4Arg[1];

    eState = DMX_SCRAMBLE_STATE_UNKNOWN;

    switch (i4MediaType)
    {
    case papi_fe_tsdmx_MediaTypeVideo:
        eState = DMX_GetScrambleState(_u1VideoPidx);
        break;

    case papi_fe_tsdmx_MediaTypeAudio:
        eState = DMX_GetScrambleState(_u1AudioPidx);
        break;

    case papi_fe_tsdmx_MediaTypeAudioDesc:
        eState = DMX_GetScrambleState(_u1AudioDescPidx);
        break;

    default:
        break;
    }

    switch (eState)
    {
    case DMX_SCRAMBLE_STATE_CLEAR:
        i4State = papi_fe_tsdmx_NotScrambled;
        break;

    case DMX_SCRAMBLE_STATE_SCRAMBLED:
        i4State = papi_fe_tsdmx_Scrambled;
        break;

    case DMX_SCRAMBLE_STATE_UNKNOWN:
    default:
        i4State = papi_fe_tsdmx_ScramblingUnknown;
        break;
    }

    // Copy data to user memory
    COPY_TO_USER(pi4Ret, &i4State, sizeof(UINT32));

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Copy section data
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval TM_OK           Successful
 *  @retval TM_FAIL         Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_CopyData(UINT32 u4Arg)
{
    IOCTL_8ARG_T r8Arg;
    UINT32 u4Addr, u4Size, u4Type;
    UINT8 u1Pidx, u1Serial;
    UINT8 *pu1Buf, *pu1KernelBuf;

    COPY_FROM_USER(&r8Arg, u4Arg, sizeof(r8Arg));

    // Get parameters
    u1Pidx = (UINT8)(r8Arg.au4Arg[0] & 0xff);
    u4Addr = (UINT32)r8Arg.au4Arg[1];
    u4Size = (UINT32)r8Arg.au4Arg[2];
    u1Serial = (UINT8)r8Arg.au4Arg[3];
    pu1Buf = (UINT8*)r8Arg.au4Arg[4];
    u4Type = r8Arg.au4Arg[5];

    // Check if user buffer is valid
    if (!access_ok(VERIFY_WRITE, (void __user *)pu1Buf, u4Size))
    {
        return TM_FAIL;
    }

    // Copy data to local buffer
    switch (u4Type)
    {
    case TSDMX_PID_TYPE_SECTION:
        ASSERT(u4Size <= MAX_SECTION_SIZE);
        if (!DMX_PSI_GetBuffer(u1Pidx, u1Serial, u4Addr, 0, u4Size, _au1SecBuf))
        {
            return TM_FAIL;
        }
        pu1KernelBuf = _au1SecBuf;
        break;

    case TSDMX_PID_TYPE_PES:
        ASSERT(u4Size <= MAX_PES_SIZE);
        if (!DMX_PES_GetBuffer(u1Pidx, u1Serial, u4Addr, 0, u4Size, _au1PesBuf))
        {
            return TM_FAIL;
        }
        pu1KernelBuf = _au1PesBuf;
        break;

    default:
        return TM_FAIL;
    }

    // Copy to user buffer
    if (copy_to_user((void __user *)pu1Buf, pu1KernelBuf, u4Size))
    {
        return TM_FAIL;
    }

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Unlock section data
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval TM_OK           Successful
 *  @retval TM_FAIL         Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_UnlockData(UINT32 u4Arg)
{
    IOCTL_8ARG_T r8Arg;
    UINT32 u4Addr, u4Size, u4Type, u4FilterMap;
    UINT8 u1Pidx, u1Serial;

    COPY_FROM_USER(&r8Arg, u4Arg, sizeof(r8Arg));

    // Get parameters
    u1Pidx = (UINT8)(r8Arg.au4Arg[0] & 0xff);
    u4Addr = (UINT32)r8Arg.au4Arg[1];
    u4Size = (UINT32)r8Arg.au4Arg[2];
    u1Serial = (UINT8)r8Arg.au4Arg[3];
    u4Type = r8Arg.au4Arg[5];
    u4FilterMap = (UINT32)r8Arg.au4Arg[6];       // not used

    switch (u4Type)
    {
    case TSDMX_PID_TYPE_SECTION:
        // Unlock section data
        if (!DMX_PSI_UnlockBuffer(u1Pidx, u1Serial, u4Size, u4Addr))
        {
            return TM_FAIL;
        }
        break;

    case TSDMX_PID_TYPE_PES:
        if (!DMX_PES_UnlockBuffer(u1Pidx, u1Serial, u4Size, u4Addr))
        {
            return TM_FAIL;
        }
        break;

    default:
        return TM_FAIL;
    }

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Set audio type
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval TM_OK           Successful
 *  @retval TM_FAIL         Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_SetAudioType(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    int papi_streamtype;

    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    papi_streamtype = (int)r2Arg.au4Arg[0];
    switch (papi_streamtype)
    {
    case papi_fe_tsdmx_StreamTypemp1a:
    case papi_fe_tsdmx_StreamTypemp2a:
    case papi_fe_tsdmx_StreamTypemp4a:
        _eAudioFormat = AUD_FMT_MPEG;
        break;

    case papi_fe_tsdmx_StreamTypeac3:
        _eAudioFormat = AUD_FMT_AC3;
        break;

    case papi_fe_tsdmx_StreamTypeaac:
    case papi_fe_tsdmx_StreamTypeheaac:
        _eAudioFormat = AUD_FMT_AAC;
        break;

    /************************************
    *** WMA is not supported in PAPI. ***
    case papi_fe_tsdmx_StreamTypewma:
        eAudioFormat = AUD_FMT_WMA;
        break;
    ************************************/

    default:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] unknown audio format\n");
        return TM_FAIL;
    }

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Set video type
 *
 *  @param u4Arg            The input arguments
 *
 *  @retval TM_OK           Successful
 *  @retval TM_FAIL         Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_SetVideoType(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    int papi_streamtype;
    DMX_VIDEO_TYPE_T eDmxVideoType;

    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    papi_streamtype = (int)r2Arg.au4Arg[0];
    switch (papi_streamtype)
    {
    case papi_fe_tsdmx_StreamTypemp1v:
    case papi_fe_tsdmx_StreamTypemp2v:
        eDmxVideoType = DMX_VIDEO_MPEG;
        _eVdecFormat = VDEC_FMT_MPV;
        break;

    case papi_fe_tsdmx_StreamTypemp4v:
    case papi_fe_tsdmx_StreamTypedivx:
        eDmxVideoType = DMX_VIDEO_MPEG4;
        _eVdecFormat = VDEC_FMT_MP4;
        break;

    case papi_fe_tsdmx_StreamTypeh264:
        eDmxVideoType = DMX_VIDEO_H264;
        _eVdecFormat = VDEC_FMT_H264;
        break;

    case papi_fe_tsdmx_StreamTypevc1:
        eDmxVideoType = DMX_VIDEO_VC1;
        _eVdecFormat = VDEC_FMT_WMV;
        break;

    default:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] unknown video format\n");
        return TM_FAIL;
    }

    if (!DMX_SetVideoType(eDmxVideoType))
    {
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to set DMX video type!\n");
        return TM_FAIL;
    }

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Set the specified Timer in the kernel glue layer.
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_Command(UINT32 u4Arg)
{
    IOCTL_2ARG_T r2Arg;
    UINT32 u4Timeout;
    UINT8 u1FltIdx, u1Command;
    DMX_FILTER_GENERIC_T rFilter;

    // Copy arguments from user space
    COPY_FROM_USER(&r2Arg, u4Arg, sizeof(r2Arg));

    u1FltIdx = (UINT8)(r2Arg.au4Arg[0] & 0xFF);
    ASSERT(u1FltIdx < DMX_NUM_FILTER);

    u1Command = (UINT8)(r2Arg.au4Arg[1] & 0xFF);
    if (/*(TSDMX_CMD_STOP_TIMER <= u1Command) && */(u1Command <= TSDMX_CMD_RESET_TIMER))
    {
        if (!_arIoFilter[u1FltIdx].fgIsList)
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Cannot stop Timer for non-list filter (idx: %u)!\n", u1FltIdx);
            return TM_FAIL;
        }
        if (_ahTimer[u1FltIdx] == NULL_HANDLE)
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Timer handler is NULL. (idx: %u)!\n", u1FltIdx);
            if (u1Command == TSDMX_CMD_STOP_TIMER)
            {
                return TM_OK;
            }
            else
            {
                return TM_ERR_RESOURCE_NOT_OWNED;
            }
        }
    }

    _Lock();
    u4Timeout = _arIoFilter[u1FltIdx].u4Timeout;
    _Unlock();
    switch (u1Command)
    {
    case TSDMX_CMD_STOP_TIMER:
        if (x_timer_stop(_ahTimer[u1FltIdx]) != OSR_OK)
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to stop timer %u!\n", u1FltIdx);
            return TM_FAIL;
        }
        break;

    case TSDMX_CMD_START_TIMER:
        if (x_timer_start(_ahTimer[u1FltIdx], u4Timeout, X_TIMER_FLAG_REPEAT,
            _TimerAction, &(_arIoFilter[u1FltIdx])) != OSR_OK)
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to start timer %u!\n", u1FltIdx);
            return TM_FAIL;
        }
        break;

    case TSDMX_CMD_RESET_TIMER:
        if (x_timer_stop(_ahTimer[u1FltIdx]) != OSR_OK)
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to stop timer %u!\n", u1FltIdx);
            return TM_FAIL;
        }
        if (x_timer_start(_ahTimer[u1FltIdx], u4Timeout, X_TIMER_FLAG_REPEAT,
            _TimerAction, &(_arIoFilter[u1FltIdx])) != OSR_OK)
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to start timer %u!\n", u1FltIdx);
            return TM_FAIL;
        }
        break;

    case TSDMX_CMD_DISABLE_FILTER:
        rFilter.fgEnable = FALSE;
        if (!DMX_SetGenFilter(u1FltIdx, DMX_FILTER_FLAG_VALID, &rFilter))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to disable filter %u!\n", u1FltIdx);
            return TM_FAIL;
        }
        break;

    case TSDMX_CMD_ENABLE_FILTER:
        rFilter.fgEnable = FALSE;
        if (!DMX_SetGenFilter(u1FltIdx, DMX_FILTER_FLAG_VALID, &rFilter))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to disable filter %u!\n", u1FltIdx);
            return TM_FAIL;
        }
        break;

    default:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] Unknown command %u! (FltIdx: %u)\n", u1Command, u1FltIdx);
        return TM_FAIL;
    }

    return TM_OK;
}


//-----------------------------------------------------------------------------
/** Clean up the resources allocated/used by the kernel space.  It is OK to use
 *  a big Lock/Unlock in this function because this function is usually called
 *  at the time when timing is not critical.
 *
 * The following resources need to be released/modified in this function:
 *  _arIoFilter[], _u4NumFilterInUse, _arIoPid[], _u4NumPidInUse, and _ahTimer
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
static INT32 _TSDMX_Cleanup(UINT32 u4Arg)
{
    UINT8 u1FltIdx, u1Pidx;
    DMX_PID_T rPid;

    for (u1FltIdx = 0; u1FltIdx < DMX_NUM_FILTER; u1FltIdx++)
    {
        _Lock();
        if (_arIoFilter[u1FltIdx].fgIsList)
        {
            if (_ahTimer[u1FltIdx] != NULL_HANDLE)
            {
                // A timer will be stopped before it is deleted.
                if (x_timer_delete(_ahTimer[u1FltIdx]) == OSR_OK)
                {
                    _ahTimer[u1FltIdx] = NULL_HANDLE;
                }
                else
                {
                    PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to delete timer %u!\n", u1FltIdx);
                }
            }
        }
        _Unlock();

        // Find corresponding pidx
        if (!_FilterIdToPidx(u1FltIdx, &u1Pidx))
        {
            // This filter is not in use.
            continue;
        }

        // Remove the filter and the associated PID
        if (!_RemoveFilter(u1Pidx, u1FltIdx))
        {
            return TM_FAIL;
        }
    }

    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID; u1Pidx++)
    {
        if (!_arIoPid[u1Pidx].fgInUse)
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] pidx %u is not in use!\n", u1Pidx);
            continue;
        }

        if ((u1Pidx == _u1AudioPidx) || (u1Pidx == _u1AudioDescPidx) ||
            (u1Pidx == _u1VideoPidx) || (u1Pidx == _u1PcrPidx))
        {
            continue;
        }

        // Disable PID
        rPid.fgEnable = FALSE;
        if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] fail to disable pidx %u!\n", u1Pidx);
            return TM_FAIL;
        }

        // Free PID
        if (!DMX_FreePid(u1Pidx))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] fail to free pidx %u!\n", u1Pidx);
            return TM_FAIL;
        }

        // Release pidx
        if (!_ReleasePidx(u1Pidx))
        {
            PAPI_FE_PRINT(KERN_ERR "[TSDMX] fail to release pidx %u!\n", u1Pidx);
            return TM_FAIL;
        }
    }

    return TM_OK;
}


//-----------------------------------------------------------------------------
// Inter-module functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Initialize TSDMX
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _TSDMX_Init(void)
{
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
    UINT8 i;
    static BOOL fgInited = FALSE;

    // Initialize demux
    if (!DMX_Init())
    {
        return FALSE;
    }

    // Default TS from frame 1 (TS0)
    VERIFY(DMX_SetFrontEnd(DMX_FE_EXT_SERIAL1));
    // Default to use falling edge trigger
    HAL_WRITE32(DEMUX0_BASE + 0x20, 0x01030055);

    // Set decoder callback functions
    rDecoderCallbacks.pfnSendVideoPes = _papi_VideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);

    // Connect to decoder
    DMX_SetToDecoder(TRUE);

    _Lock();
    if (!fgInited)
    {
        for (i = 0; i < DMX_NUM_FILTER; i++)
        {
            _ahTimer[i] = NULL_HANDLE;
        }
        fgInited = TRUE;
    }
    else
    {
        for (i = 0; i < DMX_NUM_FILTER; i++)
        {
            if (_ahTimer[i] != NULL_HANDLE)
            {
                // A timer will be stopped before it is deleted.
                if (x_timer_delete(_ahTimer[i]) == OSR_OK)
                {
                    _ahTimer[i] = NULL_HANDLE;
                }
                else
                {
                    PAPI_FE_PRINT(KERN_ERR "[TSDMX] Fail to delete timer %d!\n", i);
                }
            }
        }
    }
    _Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Is request of section filter allowed
 *
 *  @param  u4Arg           The package of parameters
 *
 *  @retval TM_OK           Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
INT32 _TSDMX_IsRequestAllowed(UINT32 u4Arg)
{
    int i4Src;
    INT32 i4Ret = TM_FAIL;
    Nat32 u4Desg;

    UNUSED(u4Arg);

    _papi_src_src_GetCurrentVideoSelection(&i4Src, &u4Desg);

    if ((i4Src == papi_src_srcid_SrcTuner) &&
        (u4Desg & papi_src_dsgid_DesignatorDigital) &&
        (_u4NumPidInUse < DMX_NUM_PID) && (_u4NumFilterInUse < DMX_NUM_FILTER))
    {
        i4Ret = TM_OK;
    }

    return i4Ret;
}

#define RET_ON_FAIL(expr) \
	if(expr != 0) \
	{ \
	    return -1; \
	}

INT32 _TSDMX_StopPids(void)
{
    RET_ON_FAIL(_TSDMX_StopAudioPid(0));
    RET_ON_FAIL(_TSDMX_StopVideoPid(0));    
    RET_ON_FAIL(_TSDMX_StopPcrPid(0));        
    RET_ON_FAIL(_TSDMX_StopAudioDescriptivePid(0));

    return TM_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** ioctl entry of tsdmx
 *
 *  @param  inode           The inode
 *  @param  file            The file
 *  @param  cmd             The command
 *  @param  arg             The arguments
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
int papi_ioctl_fe_tsdmx(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    _ApiLock();

    switch (cmd)
    {
    case PAPI_IO_FE_TSDMX_SETVIDEOPID:
        i4Ret = _TSDMX_SetVideoPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_SETAUDIOPID:
        i4Ret = _TSDMX_SetAudioPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_SETAUDIODESCRIPTIVEPID:
        i4Ret = _TSDMX_SetAudioDescriptivePid(arg);
        break;

    case PAPI_IO_FE_TSDMX_SETPCRPID:
        i4Ret = _TSDMX_SetPcrPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_GETVIDEOPID:
        i4Ret = _TSDMX_GetVideoPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_GETAUDIOPID:
        i4Ret = _TSDMX_GetAudioPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_GETAUDIODESCRIPTIVEPID:
        i4Ret = _TSDMX_GetAudioDescriptivePid(arg);
        break;

    case PAPI_IO_FE_TSDMX_GETPCRPID:
        i4Ret = _TSDMX_GetPcrPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_STOPVIDEOPID:
        i4Ret = _TSDMX_StopVideoPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_STOPAUDIOPID:
        i4Ret = _TSDMX_StopAudioPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_STOPAUDIODESCRIPTIVEPID:
        i4Ret = _TSDMX_StopAudioDescriptivePid(arg);
        break;

    case PAPI_IO_FE_TSDMX_STOPPCRPID:
        i4Ret = _TSDMX_StopPcrPid(arg);
        break;

    case PAPI_IO_FE_TSDMX_ISREQUESTALLOWED:
        i4Ret = _TSDMX_IsRequestAllowed(arg);
        break;

    case PAPI_IO_FE_TSDMX_REQUESTFILTER:
        i4Ret = _TSDMX_RequestFilter(arg);
        break;

    case PAPI_IO_FE_TSDMX_UPDATEFILTER:
        i4Ret = _TSDMX_UpdateFilter(arg);
        break;

    case PAPI_IO_FE_TSDMX_CANCELFILTER:
        i4Ret = _TSDMX_CancelFilter(arg);
        break;

    case PAPI_IO_FE_TSDMX_FREESECTIONBUFFER:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] IO command %u is not supported for now.\n", cmd);
        //i4Ret = _TSDMX_FreeSectionBuffer(arg);
        i4Ret = TM_OK;
        break;

    case PAPI_IO_FE_TSDMX_REQUESTPES:
        i4Ret = _TSDMX_RequestPes(arg);
        break;

    case PAPI_IO_FE_TSDMX_CANCELPES:
        i4Ret = _TSDMX_CancelPes(arg);
        break;

    case PAPI_IO_FE_TSDMX_FREEPESBUFFER:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] IO command %u is not supported for now.\n", cmd);
        //i4Ret = _TSDMX_FreePesBuffer(arg);
        i4Ret = TM_OK;
        break;

    case PAPI_IO_FE_TSDMX_GETSCRAMBLINGSTATUS:
        i4Ret = _TSDMX_GetScramblingStatus(arg);
        break;

    case PAPI_IO_FE_TSDMX_COPYDATA:
        i4Ret = _TSDMX_CopyData(arg);
        break;

    case PAPI_IO_FE_TSDMX_UNLOCKDATA:
        i4Ret = _TSDMX_UnlockData(arg);
        break;

    case PAPI_IO_FE_TSDMX_SETAUDIOTYPE:
        i4Ret = _TSDMX_SetAudioType(arg);
        break;

    case PAPI_IO_FE_TSDMX_SETVIDEOTYPE:
        i4Ret = _TSDMX_SetVideoType(arg);
        break;

    case PAPI_IO_FE_TSDMX_REQUESTLISTFILTER:
        i4Ret = _TSDMX_RequestListFilter(arg);
        break;

    case PAPI_IO_FE_TSDMX_COMMAND:
        i4Ret = _TSDMX_Command(arg);
        break;

    case PAPI_IO_FE_TSDMX_CLEANUP:
        i4Ret = _TSDMX_Cleanup(arg);
        break;

    default:
        PAPI_FE_PRINT(KERN_ERR "[TSDMX] unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    _ApiUnlock();

    return i4Ret;
}


