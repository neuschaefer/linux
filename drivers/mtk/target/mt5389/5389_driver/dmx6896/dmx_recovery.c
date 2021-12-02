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
 * $Author: jiangbo.wang $
 * $Date: 2012/07/16 $
 * $RCSfile: $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_recovery.c
 *  Demux driver - Recovery handler
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "fvr.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_timer.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_ERR_LEVEL_T _eDmxErrLevel = DMX_ERR_ASSERT;

static BOOL _fgPanicError = FALSE;

static UINT32 _u4DmxPanicTimes = 0;
static HAL_TIME_T _rDmxRecoverTime;

static UINT32 _u4dmxMicroPanicTimes = 0;
static HAL_TIME_T _rDmxMicroRecovertime;

static UINT32 _u4DmxRecoveryErr = 0;

INT32 _i4DmxErrReason = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_SetErrLevel
 */
//-----------------------------------------------------------------------------
void _DMX_SetErrLevel(DMX_ERR_LEVEL_T eLevel)
{
    _eDmxErrLevel = eLevel;
}


//-----------------------------------------------------------------------------
/** _DMX_Panic
 */
//-----------------------------------------------------------------------------
void _DMX_Panic(const CHAR* szExpress, const CHAR* szFile, INT32 i4Line)
{
    _u4DmxPanicTimes++;    
    HAL_GetTime(&_rDmxRecoverTime);

    switch(_eDmxErrLevel)
    {
    case DMX_ERR_RESET:
        _i4DmxErrReason = -1;
        LOG(3, "\nDMX panic at:\nFile: %s, line %d\n", szFile, i4Line);
        LOG(3, "\n\t%s", szExpress);
        LOG(3, "\n\n");
        UNUSED(_DMX_Stop());
        _DMX_ErrHanlde();
        break;

    case DMX_ERR_BYPASS:
        break;

    case DMX_ERR_ASSERT:
    default:
        Assert(szExpress, szFile, i4Line);
        break;
    }
}


//-----------------------------------------------------------------------------
/** _DMX_PanicMicroCode
 */
//-----------------------------------------------------------------------------
void _DMX_PanicMicroCode(void)
{
    _u4dmxMicroPanicTimes++;
    HAL_GetTime(&_rDmxMicroRecovertime);
    
    switch(_eDmxErrLevel)
    {
    case DMX_ERR_RESET:
    case DMX_ERR_BYPASS:
        LOG(3, "%s\n", __FUNCTION__);
        //*(volatile UINT32*)0x20019CC0 = 0;  // This address may be used for other purpose!
        UNUSED(_DMX_Stop());
        _DMX_ErrHanlde();
        break;

    case DMX_ERR_ASSERT:
    default:
        ASSERT(0);
        break;
    }
}


//-----------------------------------------------------------------------------
/** _DMX_PanicReset
 */
//-----------------------------------------------------------------------------
void _DMX_PanicReset(void)
{
    DMX_PID_T *prDmxPid, rPid;
    DMX_INPUT_TYPE_T eInputType;
    BOOL fgToDecoder;
    DMX_DECODER_CALLBACKS_T rDeocder;
    UINT8 i, j;
    UINT8 u1PCRPid;
    FILTER_TYPE_T eFilterType;
    DMX_FILTER_T *prFilter;
    DMX_FILTER_GENERIC_T *prGenFilter;
#ifdef TIME_SHIFT_SUPPORT
    BOOL fgIsReocrd = FALSE;
    FVR_GBL_T rGBL;
    FVR_PID_T *prFVRPid = NULL;
    FVR_PID_T rFVRPid;
    FVR_TIMESTAMP_T rTimeStamp;
#endif  // TIME_SHIFT_SUPPORT
    DMX_SOURCE_INFO_T *prSource;

    prDmxPid = NULL;
    prFilter = NULL;
    prGenFilter = NULL;
    prSource = NULL;

    _DMX_Lock();
    _fgPanicError = TRUE;
    _DMX_Unlock();

    _u4DmxRecoveryErr = 0;

    // Double confirm
    UNUSED(_DMX_Stop());
#ifdef TIME_SHIFT_SUPPORT
    fgIsReocrd = _FVR_IsRecording(0);
    UNUSED(_FVR_Stop(0));
    UNUSED(_DMX_PVRPlay_Stop(TRUE));
#endif  // TIME_SHIFT_SUPPORT
    //x_thread_delay(5);
    
    //---------------------------------------
    // Backup setting
    //---------------------------------------

    // Search PCR
    u1PCRPid = 0xFF;
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if((PID_INDEX_TABLE(i) & 0x80008000) == 0x80008000)
        {
            u1PCRPid = i;
            break;
        }
    }

    prDmxPid = (DMX_PID_T*)x_mem_alloc(DMX_NUM_PID_INDEX * sizeof(DMX_PID_T));
    if(prDmxPid == NULL)
    {
        _u4DmxRecoveryErr = 1;
        goto PanicExit;
    }
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if(!_DMX_GetPid(i, DMX_PID_FLAG_ALL, &prDmxPid[i]))
        {
            _u4DmxRecoveryErr = 2;
            goto PanicExit;
        }
        if((prDmxPid[i].ePidType == DMX_PID_TYPE_PSI) && prDmxPid[i].fgAllocateBuffer &&
           (prDmxPid[i].u4BufSize != 0))
        {
            // Due to one byte section filter
            prDmxPid[i].u4BufSize /= DMX_PSI_BUF_TIMES;
        }

        rPid.fgEnable = FALSE;
        if(!_DMX_SetPid(i, DMX_PID_FLAG_VALID, &rPid, FALSE))
        {
            _u4DmxRecoveryErr = 3;
            goto PanicExit;
        }

        if(!_DMX_FreePid(i))
        {
            _u4DmxRecoveryErr = 4;
            goto PanicExit;
        }
    }

    eFilterType = _DMX_GetFilterType();
    if(eFilterType == FILTER_TYPE_LEGACY)
    {
        prFilter = (DMX_FILTER_T*)x_mem_alloc((DMX_NUM_FILTER_INDEX * 2) * sizeof(DMX_FILTER_T));
        if(prFilter == NULL)
        {
            _u4DmxRecoveryErr = 5;
            goto PanicExit;
        }
        for(i=0; i<DMX_NUM_FILTER_INDEX; i++)
        {
            for(j=0; j<2; j++)
            {
                if(!_DMX_GetFilter(i, j, DMX_FILTER_FLAG_ALL, &prFilter[(i*2) + j]))
                {
                    _u4DmxRecoveryErr = 6;
                    goto PanicExit;
                }
            }
        }
    }
    else
    {
        prGenFilter = (DMX_FILTER_GENERIC_T*)x_mem_alloc(DMX_NUM_FILTER_INDEX * sizeof(DMX_FILTER_GENERIC_T));
        if(prGenFilter == NULL)
        {
            _u4DmxRecoveryErr = 7;
            goto PanicExit;
        }
        for(i=0; i<DMX_NUM_FILTER_INDEX; i++)
        {
            if(!_DMX_GetGenFilter(i, DMX_FILTER_FLAG_ALL, &prGenFilter[i]))
            {
                _u4DmxRecoveryErr = 8;
                goto PanicExit;
            }
        }
    }

#ifdef TIME_SHIFT_SUPPORT
    if(!_FVR_GetGBL(0, FVR_GBL_FLAG_ALL, &rGBL))
    {
        _u4DmxRecoveryErr = 9;
        goto PanicExit;
    }
    prFVRPid = (FVR_PID_T*)x_mem_alloc(FVR_NUM_PID_INDEX * sizeof(FVR_PID_T));
    if(prFVRPid == NULL)
    {
        _u4DmxRecoveryErr = 10;
        goto PanicExit;
    }
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(!_FVR_GetPid(i, FVR_PID_FLAG_ALL, &prFVRPid[i]))
        {
            _u4DmxRecoveryErr = 11;
            goto PanicExit;
        }
        rFVRPid.fgEnable = FALSE;
        if(!_FVR_SetPid(i, FVR_PID_FLAG_ENABLE, &rFVRPid))
        {
            _u4DmxRecoveryErr = 12;
            goto PanicExit;
        }
        if(!_FVR_FreePid(i))
        {
            _u4DmxRecoveryErr = 13;
            goto PanicExit;
        }
    }
    if(!_FVR_FreeGBL(0))
    {
        _u4DmxRecoveryErr = 14;
        goto PanicExit;
    }
    _FVR_GetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);
#endif  //TIME_SHIFT_SUPPORT

    // Backup source struct
    prSource = (DMX_SOURCE_INFO_T*)x_mem_alloc(DMX_SOURCE_NUM * sizeof(DMX_SOURCE_INFO_T));
    if(prSource == NULL)
    {
        _u4DmxRecoveryErr = 21;
        goto PanicExit;
    }

    eInputType = _DMX_GetInputType();
    fgToDecoder = _DMX_IsToDecoder();
    rDeocder = *(_DMX_GetDecoderCallbacks());    

    //---------------------------------------
    // Reset
    //---------------------------------------
    UNUSED(_DMX_Init());

    //---------------------------------------
    // Restore setting
    //---------------------------------------
    UNUSED(_DMX_SelectInputType(eInputType));
    _DMX_SetDecoderCallbacks(&rDeocder);
    _DMX_SetToDecoder(fgToDecoder);

    // DTV, support one DTV now
    //UNUSED(_DMX_SetFrontEndEx(DMX_SOURCE_DTV0, prSource[DMX_SOURCE_DTV0].eFrontEnd));
    //UNUSED(_DMX_SetFrontEndEx(DMX_SOURCE_DTV1, prSource[DMX_SOURCE_DTV1].eFrontEnd));
    //UNUSED(_DMX_SetVideoType(DMX_SOURCE_DTV0, prSource[DMX_SOURCE_DTV0].eVideoType));
    //UNUSED(_DMX_SetVideoType(DMX_SOURCE_DTV1, prSource[DMX_SOURCE_DTV1].eVideoType));
    
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if(!_DMX_SetPid(i, DMX_PID_FLAG_ALL, &prDmxPid[i], FALSE))
        {
            _u4DmxRecoveryErr = 15;
            goto PanicExit;
        }

        if(prDmxPid[i].ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            if(!_DMX_SetBufPointer(i, prDmxPid[i].u4BufAddr, prDmxPid[i].u4BufAddr + prDmxPid[i].u4BufSize - 1,
                              prDmxPid[i].u4Wp))
            {
                _u4DmxRecoveryErr = 16;
                //goto PanicExit;
            }
        }
    }

    if(eFilterType == FILTER_TYPE_LEGACY)
    {
        for(i=0; i<DMX_NUM_FILTER_INDEX; i++)
        {
            for(j=0; j<2; j++)
            {
                if(!_DMX_SetFilter(i, j, DMX_FILTER_FLAG_ALL, &prFilter[(i*2) + j]))
                {
                    _u4DmxRecoveryErr = 17;
                    goto PanicExit;
                }
            }
        }
    }
    else
    {
        for(i=0; i<DMX_NUM_FILTER_INDEX; i++)
        {
            if(!_DMX_SetGenFilter(i, DMX_FILTER_FLAG_ALL, &prGenFilter[i]))
            {
                _u4DmxRecoveryErr = 18;
                goto PanicExit;
            }
        }
    }

    if(u1PCRPid != 0xFF)
    {
        UNUSED(_DMX_SetPcr(u1PCRPid, TRUE, FALSE));
    }

#ifdef TIME_SHIFT_SUPPORT
    UNUSED(_FVR_Init());

    if(rGBL.u4BufSize > 0)
    {
        if(!_FVR_SetGBL(0, FVR_GBL_FLAG_ALL, &rGBL))
        {
            _u4DmxRecoveryErr = 19;
            goto PanicExit;
        }
    }

    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(!_FVR_SetPid(i, FVR_PID_FLAG_ALL, &prFVRPid[i]))
        {
            _u4DmxRecoveryErr = 20;
            goto PanicExit;
        }
    }
    _FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

    if(fgIsReocrd)
    {
        // Issue FVR overflow    
        _FVR_uPErrorInt(0);
    }
#endif  // TIME_SHIFT_SUPPORT

    _DMX_Lock();
    _fgPanicError = FALSE;
    _DMX_Unlock();

    // Double confirm
    UNUSED(_DMX_Start());

PanicExit:
    LOG(1, "%s:%d, recovery_err:%d\n", __FUNCTION__, __LINE__, _u4DmxRecoveryErr);  
    if (prDmxPid != NULL)
    {
        x_mem_free(prDmxPid);
    }
    if(prFilter != NULL)
    {
        x_mem_free(prFilter);
    }
    if(prGenFilter != NULL)
    {
        x_mem_free(prGenFilter);
    }
#ifdef TIME_SHIFT_SUPPORT    
    if(prFVRPid != NULL)
    {
        x_mem_free(prFVRPid);
    }
#endif  // TIME_SHIFT_SUPPORT 
    if(prSource != NULL)
    {
        x_mem_free(prSource);
    }

}


//-----------------------------------------------------------------------------
/** _DMX_PanicReset
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetIsPanicReset(void)
{
    BOOL fgPanic;

    _DMX_Lock();
    fgPanic = _fgPanicError;
    _DMX_Unlock();

    return fgPanic;
}


//-----------------------------------------------------------------------------
/** _DMX_PanicDisplay
 */
//-----------------------------------------------------------------------------
void _DMX_PanicDisplay(void)
{
    HAL_TIME_T rTime;

    HAL_GetTime(&rTime);
    LOG(1, "Current time: %d:%06d\n", rTime.u4Seconds, rTime.u4Micros);
    
    if(_u4DmxPanicTimes > 0)
    {
        LOG(1, "Latest recovery time: %d:%06d, times:%d\n", 
            _rDmxRecoverTime.u4Seconds, _rDmxRecoverTime.u4Micros, _u4DmxPanicTimes);
    }

    if(_u4dmxMicroPanicTimes > 0)
    {
        LOG(1, "Latest micro-code recovery time: %d:%06d, times:%d\n", 
                _rDmxMicroRecovertime.u4Seconds, _rDmxMicroRecovertime.u4Micros, _u4dmxMicroPanicTimes);
    }

    if((_u4DmxPanicTimes == 0) && (_u4dmxMicroPanicTimes == 0))
    {
        LOG(1, "No recovery\n");
    }
}

