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
 * $RCSfile: dmx_vcode.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_vcode.c
 *  Demux driver - Video start code
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_mm.h"
#include "dmx_debug.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_os.h"

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
// store video type per PID
typedef struct
{
    BOOL fgEnable;
    DMX_VIDEO_TYPE_T eType;          ///< Type of video
} DMX_VIDEO_TYPE_PER_PID_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#if 0  // Original
// MPEG2 start code
static DMX_STARTCODE_T _arMPEG2StartCode_NoIntr[] =
{
//-----------------------------------------
//    { 0x01b30000, 0xffff0000 },
//    { 0x01b70000, 0xffff0000 },
    { 0x01b30000, 0xfffB0000 },     // Merge from above two start code
//-----------------------------------------
    { 0x01b80000, 0xffff0000 },
    { 0x01018000, 0xff1f8000 },         // H.264
    { 0x01070000, 0xff1f000f }          // H.264
};
static DMX_STARTCODE_T _arMPEG2StartCode_Intr[] =
{
    { 0x01000008, 0xffff0038 },
    { 0x01000010, 0xffff0038 },
    { 0x01000018, 0xffff0038 }
};

// H.264 start code
static DMX_STARTCODE_T _arH264StartCode_NoIntr[] =
{
#ifdef CC_DMX_PES_EXT_BUFFER
    { 0x01090000, 0xff1f0000 },
#endif  // CC_DMX_PES_EXT_BUFFER
    { 0x01010000, 0xff1B7000 },
    { 0x01060000, 0xff1f0000 },
//-----------------------------------------
//    { 0x01000008, 0xffff0038 },         // MPEG-I
//    { 0x01000010, 0xffff0038 }          // MPEG-P
    { 0x01000000, 0xffff0020 }       // Merge from above two
//-----------------------------------------
};

static DMX_STARTCODE_T _arH264StartCode_Intr[] =
{
    { 0x01018000, 0xff1f8000 },
    { 0x01058000, 0xff1f8000 },
    { 0x01070000, 0xff1f000f },
    { 0x01080000, 0xff1f0000 },
    { 0x010a0000, 0xff1f0000 }
};

// AVS start code
static DMX_STARTCODE_T _arAVSStartCode_NoIntr[] =
{
    { 0x01B10000, 0xffff0000 },     // sequence end
    { 0x01B20000, 0xffff0000 },     // user data
    { 0x01B50000, 0xffff0000 },     // extension
    { 0x01B70000, 0xffff0000 }      // video edie code
};

static DMX_STARTCODE_T _arAVSStartCode_Intr[] =
{
    { 0x01B00000, 0xffff0000 },     // sequence start code
    { 0x01B30000, 0xffff0000 },     // I picture
    { 0x01B60000, 0xffff0000 }      // PB picture
};

#ifdef ENABLE_MULTIMEDIA
// VC1 start code
static DMX_STARTCODE_T _arVC1StartCode[] =
{
    { 0x010C0000, 0xffff0000 },
    { 0x010D0000, 0xffff0000 },
    { 0x010E0000, 0xffff0000 },
    { 0x010F0000, 0xffff0000 }
};
// MP4 start code
static DMX_STARTCODE_T _arMPEG4StartCode[] =
{
    { 0x01000000, 0xff000000 },
    { 0x01200000, 0xfff00000 },
    { 0x01B20000, 0xffff0000 },
    { 0x01B60000, 0xffff0000 }
};
#endif  // ENABLE_MULTIMEDIA

#endif

static INT8 au1DmxVOffsetMPEG2[10] =
                { -3, -3, -3, -4, -5, -5, -5, -5, DMX_VNULL, DMX_VNULL };

static INT8 au1DmxVOffsetH264[10] =
                { -3, -4, -3, -3, -7, -4, -3, -3, -3, DMX_VNULL };

static INT8 au1DmxVOffsetAVS[10] =
                { -3, -3, -3, -3, DMX_VNULL, -3, -3, -3, DMX_VNULL, DMX_VNULL };

#ifdef ENABLE_MULTIMEDIA
// VC1 start code
static DMX_STARTCODE_T _arVC1StartCode[] =
{
    { 0x0, 0x0000010C, 0x0, 0xFFFFFFFF },
    { 0x0, 0x0000010D, 0x0, 0xFFFFFFFF },
    { 0x0, 0x0000010E, 0x0, 0xFFFFFFFF },
    { 0x0, 0x0000010F, 0x0, 0xFFFFFFFF }
};
static INT8 au1DmxVOffsetVC1[10] =
                { DMX_VNULL, DMX_VNULL, DMX_VNULL, DMX_VNULL, DMX_VNULL,
                  -3, -3, -3, -3, DMX_VNULL };

// MPEG4 start code
static DMX_STARTCODE_T _arMPEG4StartCode[] =
{
    { 0x0, 0x00000120, 0x0, 0xFFFFFFF0 },
    { 0x0, 0x000001B2, 0x0, 0xFFFFFFFF },
    { 0x0, 0x000001B6, 0x0, 0xFFFFFFFF }
};
static INT8 au1DmxVOffsetMPEG4[10] =
                { DMX_VNULL, DMX_VNULL, DMX_VNULL, DMX_VNULL, DMX_VNULL,
                  -3, -3, -3, DMX_VNULL, DMX_VNULL};

#endif  // ENABLE_MULTIMEDIA

static DMX_VIDEO_TYPE_T _au1DmxTsIdxVcodeBase[4];
static DMX_VIDEO_TYPE_PER_PID_T _au1DmxVideoTypePerPid[DMX_NUM_PID_INDEX];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _DmxSetHWVideoType
 *  Set VDec start code pattern
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSetHWVideoType(DMX_VIDEO_TYPE_T eVideoType)
{
    switch(eVideoType)
    {
#if 0
    case DMX_VIDEO_MPEG:
        if(!_DMX_SetStartCodePattern_Ex(
            (UINT8)(sizeof(_arMPEG2StartCode_NoIntr)/sizeof(DMX_STARTCODE_T)),
            _arMPEG2StartCode_NoIntr,
            (UINT8)(sizeof(_arMPEG2StartCode_Intr)/sizeof(DMX_STARTCODE_T)),
            _arMPEG2StartCode_Intr))
        {
            return FALSE;
        }
        break;

    case DMX_VIDEO_H264:
        if(!_DMX_SetStartCodePattern_Ex(
            (UINT8)(sizeof(_arH264StartCode_NoIntr)/sizeof(DMX_STARTCODE_T)),
            _arH264StartCode_NoIntr,
            (UINT8)(sizeof(_arH264StartCode_Intr)/sizeof(DMX_STARTCODE_T)),
            _arH264StartCode_Intr))
        {
            return FALSE;
        }
        break;

    case DMX_VIDEO_AVS:
        if(!_DMX_SetStartCodePattern_Ex(
            (UINT8)(sizeof(_arAVSStartCode_NoIntr)/sizeof(DMX_STARTCODE_T)),
            _arAVSStartCode_NoIntr,
            (UINT8)(sizeof(_arAVSStartCode_Intr)/sizeof(DMX_STARTCODE_T)),
            _arAVSStartCode_Intr))
        {
            return FALSE;
        }
        break;
#endif

#ifdef ENABLE_MULTIMEDIA
    case DMX_VIDEO_VC1:
        if(!_DMX_SetStartCodePattern_Ex(0, NULL,
            (UINT8)(sizeof(_arVC1StartCode)/sizeof(_arVC1StartCode[0])), _arVC1StartCode))
        {
            return FALSE;
        }
        break;

    case DMX_VIDEO_WMV7:    // No need to search for any start code
    case DMX_VIDEO_WMV8:    // No need to search for any start code
    case DMX_VIDEO_WMV9:    // No need to search for any start code
    case DMX_VIDEO_MP4_IN_WMV: // No need to search for any start code
    case DMX_VIDEO_RV: // No need to search for any start code
    case DMX_VIDEO_VP6:
    case DMX_VIDEO_VP8:
        break;

    case DMX_VIDEO_MPEG4:
        if(!_DMX_SetStartCodePattern_Ex(0, NULL,
            (UINT8)(sizeof(_arMPEG4StartCode)/sizeof(_arMPEG4StartCode[0])), _arMPEG4StartCode))
        {
            return FALSE;
        }
        break;
#endif  // ENABLE_MULTIMEDIA

    default:
        LOG(1, "Unknown video type\n");
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DmxSetTsIndexVcode
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSetTsIndexVcode(UINT8 u1TsIdx, DMX_VIDEO_TYPE_T eVideoType)
{
    UINT32 u4Reg, u4Mask, u4Val;

    if(u1TsIdx >= 4)
    {
        return FALSE;
    }

    switch(eVideoType)
    {
    case DMX_VIDEO_MPEG:
        u4Val = 0;
        break;

    case DMX_VIDEO_H264:
        u4Val = 1;
        break;

    case DMX_VIDEO_AVS:
        u4Val = 2;
        break;

    default:
        if(!_DmxSetHWVideoType(eVideoType))
        {
            return FALSE;
        }
        u4Val = 3;
        break;
    }

    _DMX_Lock();
    _au1DmxTsIdxVcodeBase[u1TsIdx] = eVideoType;
    u4Reg = DMX_VCODE_W(0);
    u4Mask = (UINT32)0xFF << (u1TsIdx * 8);
    u4Reg &= ~u4Mask;
    u4Reg |= (u4Val << (u1TsIdx * 8));
    DMX_VCODE_W(0) = u4Reg;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_GetPidxVideoType
 *  Set VDec start code pattern
 */
//-----------------------------------------------------------------------------
DMX_VIDEO_TYPE_T _DMX_GetPidxVideoType(UINT8 u1Pidx)
{
    PID_STRUCT_T *prPidStruct;
    UINT8 u1TsIdx = 0;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_VIDEO_NONE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if(prPidStruct == NULL)
    {
        return DMX_VIDEO_NONE;
    }

    // MM do not have ts index information in pid structure
    if(prPidStruct->eInputType == DMX_IN_PLAYBACK_MM || prPidStruct->eInputType == DMX_IN_PLAYBACK_ES ||
        prPidStruct->eInputType == DMX_IN_PLAYBACK_PS)
    {
        if(_DMX_MUL_GetMultipleInstEnabled())
        {
            u1TsIdx = _DMX_MUL_GetTsIdx(_DMX_MUL_GetPidxInstId(u1Pidx));
        }
    }
    else    // TS and PS can get ts index from pid struct
    {
        _DMX_Lock();
        u1TsIdx = prPidStruct->u1TsIndex;
        _DMX_Unlock();

        if(_DMX_GetVideoTypeByPidx(u1Pidx) != DMX_VIDEO_NONE)
        {
            return _DMX_GetVideoTypeByPidx(u1Pidx);
        }
    }

    return _DMX_GeteVideoType(u1TsIdx);
}


//-----------------------------------------------------------------------------
/** _DMX_SetVideoType
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetVideoType(UINT8 u1TsIdx, DMX_VIDEO_TYPE_T eVideoType)
{
    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    if(_au1DmxTsIdxVcodeBase[u1TsIdx] != eVideoType)
    {
        if(!_DmxSetTsIndexVcode(u1TsIdx, eVideoType))
        {
            ASSERT(0);
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GeteVideoType
 */
//-----------------------------------------------------------------------------
DMX_VIDEO_TYPE_T _DMX_GeteVideoType(UINT8 u1TsIndex)
{
    DMX_VIDEO_TYPE_T eVideoType;

    if(u1TsIndex >= DMX_FRAMER_COUNT)
    {
        return DMX_VIDEO_NONE;
    }

    _DMX_Lock();
    eVideoType = _au1DmxTsIdxVcodeBase[u1TsIndex];
    _DMX_Unlock();

    return eVideoType;
}


//-----------------------------------------------------------------------------
/** _DMX_SetVideoTypeByPidx
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetVideoTypeByPidx(UINT8 u1Pidx, DMX_VIDEO_TYPE_T eVideoType)
{
    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _DMX_Lock();
    _au1DmxVideoTypePerPid[u1Pidx].fgEnable = TRUE;
    _au1DmxVideoTypePerPid[u1Pidx].eType = eVideoType;
    _DMX_Unlock();

    _DMX_SetPidVideoType(u1Pidx, eVideoType);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GeteVideoTypeByPidx
 */
//-----------------------------------------------------------------------------
DMX_VIDEO_TYPE_T _DMX_GetVideoTypeByPidx(UINT8 u1Pidx)
{
    DMX_VIDEO_TYPE_T eVideoType;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_VIDEO_NONE;
    }

    if(_au1DmxVideoTypePerPid[u1Pidx].fgEnable == FALSE)
    {
        return DMX_VIDEO_NONE;
    }

    _DMX_Lock();
    eVideoType = _au1DmxVideoTypePerPid[u1Pidx].eType;
    _DMX_Unlock();

    return eVideoType;
}


//-----------------------------------------------------------------------------
/** _DMX_FreeVideoTypeByPidx
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FreeVideoTypeByPidx(UINT8 u1Pidx)
{
    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _DMX_Lock();
    _au1DmxVideoTypePerPid[u1Pidx].fgEnable = FALSE;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_VCodeInit
 */
//-----------------------------------------------------------------------------
BOOL _DMX_VCodeInit(void)
{
    UINT8 i;

    for(i=0; i<4; i++)
    {
        _au1DmxTsIdxVcodeBase[i] = DMX_VIDEO_MPEG;
    }

    DMX_VCODE_W(0) = 0;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetVCode_Offset
 */
//-----------------------------------------------------------------------------
INT8 _DMX_GetVCode_Offset(DMX_VIDEO_TYPE_T eVideoType, UINT8 u1Idx)
{
    ASSERT(u1Idx < 10);
    if(u1Idx >= 10)
    {
        return DMX_VNULL;
    }

    switch(eVideoType)
    {
    case DMX_VIDEO_MPEG:
        return au1DmxVOffsetMPEG2[u1Idx];

    case DMX_VIDEO_H264:
        return au1DmxVOffsetH264[u1Idx];

    case DMX_VIDEO_AVS:
        return au1DmxVOffsetAVS[u1Idx];

#ifdef ENABLE_MULTIMEDIA
    case DMX_VIDEO_VC1:
        return au1DmxVOffsetVC1[u1Idx];

    case DMX_VIDEO_MPEG4:
        return au1DmxVOffsetMPEG4[u1Idx];
#endif

    default:
        return DMX_VNULL;
    }
}

