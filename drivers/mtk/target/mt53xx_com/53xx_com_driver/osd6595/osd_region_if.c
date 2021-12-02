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
 * $RCSfile: osd_region_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_region_if.c
 *  This header file includes public function definitions of OSD region.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define OSD_INVALID_REGION_HANDLE -1
#define OSD_RGN_NODE_ALLOCATED 1
#define OSD_RGN_LIST_ALLOCATED 1


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _OSD_REGION_NODE_T
{
    UINT32 u4NodeStatus;
    INT32 i4Prev, i4Next;
    #ifdef CC_FLIP_MIRROR_SUPPORT
    UINT32 u4Original_X;
    UINT32 u4Original_Y;
    #endif
} OSD_REGION_NODE_T;

typedef struct _OSD_REGION_LIST_T
{
    UINT32 u4ListStatus;
    INT32 i4Head, i4Tail;
    INT32 i4Count;
} OSD_REGION_LIST_T;


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static OSD_REGION_NODE_T _rAllRgnNode[OSD_MAX_NUM_RGN];
static OSD_REGION_LIST_T _rAllRgnList[OSD_MAX_NUM_RGN_LIST];

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#ifdef CC_FLIP_MIRROR_SUPPORT
EXTERN BOOL gfgOSD_Mirror;
EXTERN BOOL gfgOSD_Flip;
#endif

//add for setting/getting the region list used by linux frame buffer
static UINT32 u4RGNList_FrameBuffer = OSD_MAX_NUM_RGN_LIST;

//for scroll osd
static UINT32 _u4ScrollRgn;
static UINT32 _u4BaseAddr;
static UINT32 _u4ScrollPitch;
static UINT32 _u4ScrollStep;
static UINT8  _u1ScrollMode = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _OsdRegionCompare(UINT32 u4Rgn1, UINT32 u4Rgn2)
{
    UINT32 u4DispY1, u4DispH1, u4DispB1;
    UINT32 u4DispY2, u4DispH2, u4DispB2;
    INT32 i4Ignore;

    i4Ignore = _OSD_RGN_GetOutputPosY(u4Rgn1, &u4DispY1);
    i4Ignore = _OSD_RGN_GetOutputHeight(u4Rgn1, &u4DispH1);
    u4DispB1 = u4DispY1 + u4DispH1;
    i4Ignore = _OSD_RGN_GetOutputPosY(u4Rgn2, &u4DispY2);
    i4Ignore = _OSD_RGN_GetOutputHeight(u4Rgn2, &u4DispH2);
    u4DispB2 = u4DispY2 + u4DispH2;

    UNUSED(i4Ignore);

    if ((u4DispB1 >= u4DispY2) && (u4DispY1 >= u4DispB2))
    {
        return 1; // region 1 > region 2
    }
    else if ((u4DispB2 >= u4DispY1) && (u4DispY2 >= u4DispB1))
    {
        return -1; // region 2 > region 1
    }
    else
    {
        return 0;
    }
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 OSD_RGN_LIST_Init(void)
{
    INT32 i4Index;
    for (i4Index = 0; i4Index < OSD_MAX_NUM_RGN_LIST; i4Index++)
    {
        _rAllRgnList[i4Index].u4ListStatus = 0;
        _rAllRgnList[i4Index].i4Head = OSD_INVALID_REGION_HANDLE;
        _rAllRgnList[i4Index].i4Tail = OSD_INVALID_REGION_HANDLE;
        _rAllRgnList[i4Index].i4Count = 0;
    }
    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_Init(void)
{
    INT32 i4Index;
    for (i4Index = 0; i4Index < OSD_MAX_NUM_RGN; i4Index++)
    {
        _rAllRgnNode[i4Index].u4NodeStatus = 0;
        _rAllRgnNode[i4Index].i4Prev = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4Index].i4Next = OSD_INVALID_REGION_HANDLE;
    }
    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_LIST_Get(UINT32 u4RgnList, INT32 i4Cmd, UINT32 * pu4Value)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    switch (i4Cmd)
    {
    case OSD_RGN_LIST_HEAD:
        *pu4Value = (UINT32)_rAllRgnList[u4RgnList].i4Head;
        break;

    case OSD_RGN_LIST_TAIL:
        *pu4Value = (UINT32)_rAllRgnList[u4RgnList].i4Tail;
        break;

    case OSD_RGN_LIST_COUNT:
        *pu4Value = (UINT32)_rAllRgnList[u4RgnList].i4Count;
        break;

    case OSD_LIST_FLAGS:
        *pu4Value = (UINT32)_rAllRgnList[u4RgnList].u4ListStatus;
        break;

    default:
        return -(INT32)OSD_RET_INV_ARG;
    }
    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_LIST_Set(UINT32 u4RgnList, INT32 i4Cmd, UINT32 u4Value)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    switch (i4Cmd)
    {
    case OSD_RGN_LIST_HEAD:
        if (u4Value >= OSD_MAX_NUM_RGN)
        {
            return -(INT32)OSD_RET_INV_REGION;
        }
        _rAllRgnList[u4RgnList].i4Head = (INT32)u4Value;
        break;

    case OSD_RGN_LIST_TAIL:
        if (u4Value >= OSD_MAX_NUM_RGN)
        {
            return -(INT32)OSD_RET_INV_REGION;
        }
        _rAllRgnList[u4RgnList].i4Tail = (INT32)u4Value;
        break;

    case OSD_RGN_LIST_COUNT:
        _rAllRgnList[u4RgnList].i4Count = (INT32)u4Value;
        break;

    case OSD_LIST_FLAGS:
        _rAllRgnList[u4RgnList].u4ListStatus = (UINT32)u4Value;
        break;

    default:
        return -(INT32)OSD_RET_INV_ARG;
    }
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** create an osd list
 *  @param pu4RgnList list id pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_LIST_Create(UINT32 * pu4RgnList)
{
    INT32 i4Index;

    if (pu4RgnList == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    //[KLUDGE]
    //Because OSD_MAX_NUM_RGN_LIST is only 20 or 30,
    //a linear search might be acceptable here.
    //Creation of a region wont happen so often,
    //hence even we have hundreds of region list,
    //it should be ok here.
    for (i4Index = 0; i4Index < OSD_MAX_NUM_RGN_LIST; i4Index++)
    {
        if ((_rAllRgnList[i4Index].u4ListStatus & OSD_RGN_LIST_ALLOCATED) == 0)
        {
            // free cell found, allocate this list, init and return it
            _rAllRgnList[i4Index].u4ListStatus |= OSD_RGN_LIST_ALLOCATED;
            _rAllRgnList[i4Index].i4Head     = OSD_INVALID_REGION_HANDLE;
            _rAllRgnList[i4Index].i4Tail     = OSD_INVALID_REGION_HANDLE;
            _rAllRgnList[i4Index].i4Count   = 0;
            *pu4RgnList = (UINT32)i4Index;
            return (INT32)OSD_RET_OK;
        }
    }

    return -(INT32)OSD_RET_OUT_OF_LIST;
}


//-----------------------------------------------------------------------------
/** delete an osd list
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_LIST_Delete(UINT32 u4RgnList)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    _rAllRgnList[u4RgnList].u4ListStatus &= ~OSD_RGN_LIST_ALLOCATED;
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** get first region id of list
 *  @param  u4RgnList list id
 *  @param  pi4HeadRegion region id pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_LIST_GetHead(UINT32 u4RgnList, INT32 * pi4HeadRegion)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    if (pi4HeadRegion == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    *pi4HeadRegion = _rAllRgnList[u4RgnList].i4Head;
    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_LIST_GetNext(UINT32 u4RgnList, INT32 i4RgnCurr, INT32 *pi4RgnNext)
{
    INT32 i4Ret;
    INT32 i4RgnNext;

    OSD_VERIFY_RGNLIST(u4RgnList);

    if (pi4RgnNext == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    i4Ret = OSD_RGN_Get((UINT32)i4RgnCurr, (INT32)OSD_RGN_NEXT, (UINT32*)&i4RgnNext);
    if (i4Ret)
    {
        return i4Ret;
    }
    *pi4RgnNext = i4RgnNext;
    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_LIST_DetachAll(UINT32 u4RgnList)
{
    INT32 i4CurrRgn, i4NextRgn;

    OSD_VERIFY_RGNLIST(u4RgnList);

    i4CurrRgn = _rAllRgnList[u4RgnList].i4Head;

    while (i4CurrRgn != OSD_INVALID_REGION_HANDLE)
    {
        i4NextRgn = _rAllRgnNode[i4CurrRgn].i4Next;
        _rAllRgnNode[i4CurrRgn].i4Prev = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4CurrRgn].i4Next = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4CurrRgn].u4NodeStatus &= ~OSD_RGN_NODE_ALLOCATED;
        //VERIFY((INT32)OSD_RET_OK == _OSD_RGN_Free((UINT32)i4CurrRgn));
        i4CurrRgn = i4NextRgn;
    }

    _rAllRgnList[u4RgnList].i4Head = OSD_INVALID_REGION_HANDLE;
    _rAllRgnList[u4RgnList].i4Tail = OSD_INVALID_REGION_HANDLE;
    _rAllRgnList[u4RgnList].i4Count = 0;

    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_LIST_GetCount(UINT32 u4RgnList, INT32 * pi4Count)
{
    INT32 i4Count;
    INT32 i4Ret;

    if (pi4Count == NULL)
    {
        return (INT32)OSD_RET_INV_ARG;
    }

    i4Ret = OSD_RGN_LIST_Get(u4RgnList, (INT32)OSD_RGN_LIST_COUNT,
                             (UINT32*)&i4Count);
    if (i4Ret)
    {
        return i4Ret;
    }

    *pi4Count = i4Count;
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** get osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  pu4Value attribute value pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Get(UINT32 u4Region, INT32 i4Cmd, UINT32 * pu4Value)
{
    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    OSD_VERIFY_REGION(u4Region);

    switch (i4Cmd)
    {
    case OSD_RGN_PREV:
        *pu4Value = (UINT32)_rAllRgnNode[u4Region].i4Prev;
        break;

    case OSD_RGN_NEXT:
        *pu4Value = (UINT32)_rAllRgnNode[u4Region].i4Next;
        break;

    case OSD_RGN_FLAGS:
        *pu4Value = _rAllRgnNode[u4Region].u4NodeStatus;
        break;

    case OSD_RGN_POS_X:
        return _OSD_RGN_GetOutputPosX(u4Region, pu4Value);

    case OSD_RGN_POS_Y:
        return _OSD_RGN_GetOutputPosY(u4Region, pu4Value);

    case OSD_RGN_BMP_W:
        return _OSD_RGN_GetInputWidth(u4Region, pu4Value);

    case OSD_RGN_BMP_H:
        return _OSD_RGN_GetInputHeight(u4Region, pu4Value);

    case OSD_RGN_DISP_W:
    case OSD_RGN_OUT_W:
        return _OSD_RGN_GetOutputWidth(u4Region, pu4Value);

    case OSD_RGN_DISP_H:
    case OSD_RGN_OUT_H:
        return _OSD_RGN_GetOutputHeight(u4Region, pu4Value);

    case OSD_RGN_COLORMODE:
        return _OSD_RGN_GetColorMode(u4Region, pu4Value);

    case OSD_RGN_ALPHA:
        return _OSD_RGN_GetAlpha(u4Region, pu4Value);

    case OSD_RGN_BMP_ADDR:
        return _OSD_RGN_GetDataAddr(u4Region, pu4Value);

    case OSD_RGN_BMP_PITCH:
    {
        UINT32 u4LineSize, u4LineSize9;
        IGNORE_RET(_OSD_RGN_GetLineSize(u4Region, &u4LineSize));
        IGNORE_RET(_OSD_RGN_GetLineSize9(u4Region, &u4LineSize9));
        *pu4Value = (u4LineSize9 << 13) | (u4LineSize << 4);
        break;
    }

    case OSD_RGN_CLIP_V:
        return _OSD_RGN_GetVClip(u4Region, pu4Value);

    case OSD_RGN_CLIP_H:
        return _OSD_RGN_GetHClip(u4Region, pu4Value);

    case OSD_RGN_PAL_LOAD:
        return _OSD_RGN_GetLoadPalette(u4Region, pu4Value);

    case OSD_RGN_PAL_ADDR:
        return _OSD_RGN_GetPaletteAddr(u4Region, pu4Value);

    case OSD_RGN_PAL_LEN:
        return _OSD_RGN_GetPaletteLen(u4Region, pu4Value);

    case OSD_RGN_STEP_V:
        return _OSD_RGN_GetVStep(u4Region, pu4Value);

    case OSD_RGN_STEP_H:
        return _OSD_RGN_GetHStep(u4Region, pu4Value);

    case OSD_RGN_COLOR_KEY:
        return _OSD_RGN_GetColorKey(u4Region, pu4Value);

    case OSD_RGN_COLOR_KEY_EN:
        return _OSD_RGN_GetColorKeyEnable(u4Region, pu4Value);

    case OSD_RGN_MIX_SEL:
        return _OSD_RGN_GetBlendMode(u4Region, pu4Value);

    case OSD_RGN_V_FLIP:
        return _OSD_RGN_GetVFlip(u4Region, pu4Value);

    case OSD_RGN_H_MIRROR:
        return _OSD_RGN_GetHMirror(u4Region, pu4Value);

    case OSD_RGN_DEC_MODE:
        return _OSD_RGN_GetDecMode(u4Region, pu4Value);
    case OSD_RGN_DEC_LINE:
        return _OSD_RGN_GetDecLine(u4Region, pu4Value);
    case OSD_RGN_DEC_EN:
        return _OSD_RGN_GetDecEn(u4Region, pu4Value);
        
    case OSD_RGN_ALPHA_SEL:
        return _OSD_RGN_GetASel(u4Region, pu4Value);
    case OSD_RGN_YR_SEL:
        return _OSD_RGN_GetYrSel(u4Region, pu4Value);
    case OSD_RGN_UG_SEL:
        return _OSD_RGN_GetUgSel(u4Region, pu4Value);
    case OSD_RGN_VB_SEL:
        return _OSD_RGN_GetVbSel(u4Region, pu4Value);
    case OSD_RGN_NEXT_EN:
        return _OSD_RGN_GetNextEnable(u4Region, pu4Value);
    case OSD_RGN_NEXT_HDR_ADDR:
        return _OSD_RGN_GetNextRegion(u4Region, pu4Value);
    case OSD_RGN_FIFO_EX:
        return _OSD_RGN_GetFifoEx(u4Region, pu4Value);

    default:
        return -(INT32)OSD_RET_INV_ARG;
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** set osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  u4Value attribute value
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Set(UINT32 u4Region, INT32 i4Cmd, UINT32 u4Value)
{
    INT32 i4Ret = OSD_RET_OK;
    
    OSD_VERIFY_REGION(u4Region);

    switch (i4Cmd)
    {
    case OSD_RGN_PREV:
        _rAllRgnNode[u4Region].i4Prev = (INT32)u4Value;
        break;

    case OSD_RGN_NEXT:
        _rAllRgnNode[u4Region].i4Next = (INT32)u4Value;
        break;

    case OSD_RGN_FLAGS:
        _rAllRgnNode[u4Region].u4NodeStatus = u4Value;
        break;

    case OSD_RGN_POS_X:
        return _OSD_RGN_SetOutputPosX(u4Region, u4Value);

    case OSD_RGN_POS_Y:
        return _OSD_RGN_SetOutputPosY(u4Region, u4Value);

    case OSD_RGN_BMP_W:
        return _OSD_RGN_SetInputWidth(u4Region, u4Value);

    case OSD_RGN_BMP_H:
        return _OSD_RGN_SetInputHeight(u4Region, u4Value);

    case OSD_RGN_DISP_W:
        return OSD_RGN_SetDisplayWidth(u4Region, u4Value);

    case OSD_RGN_DISP_H:
        return OSD_RGN_SetDisplayHeight(u4Region, u4Value);

    case OSD_RGN_OUT_W:
        return _OSD_RGN_SetOutputWidth(u4Region, u4Value);

    case OSD_RGN_OUT_H:
        return _OSD_RGN_SetOutputHeight(u4Region, u4Value);

    case OSD_RGN_COLORMODE:
        return _OSD_RGN_SetColorMode(u4Region, u4Value);

    case OSD_RGN_ALPHA:
        return _OSD_RGN_SetAlpha(u4Region, u4Value);

    case OSD_RGN_BMP_ADDR:
    	 i4Ret = _OSD_RGN_SetDataAddr(u4Region, u4Value);
         OSD_WaitVsync();
    	 OSD_WaitVsync();
        return i4Ret;

    case OSD_RGN_BMP_PITCH:
        IGNORE_RET(_OSD_RGN_SetLineSize(u4Region, (u4Value >> 4) & 0x1ff));
        IGNORE_RET(_OSD_RGN_SetLineSize9(u4Region, (u4Value >> 13) & 1));
        break;

    case OSD_RGN_CLIP_V:
    {
        UINT32 u4SrcHeight, u4OldVClip;
        // recover orginal input height
        IGNORE_RET(_OSD_RGN_GetVClip(u4Region, &u4OldVClip));
        IGNORE_RET(_OSD_RGN_GetInputHeight(u4Region, &u4SrcHeight));
        u4SrcHeight += u4OldVClip;
        if (u4SrcHeight <= u4Value)
        {
            return -(INT32)OSD_RET_INV_ARG;
        }
        //ASSERT(u4SrcHeight > u4Value);
        // cut input height
        IGNORE_RET(_OSD_RGN_SetInputHeight(u4Region, u4SrcHeight - u4Value));

        return _OSD_RGN_SetVClip(u4Region, u4Value);
    }

    case OSD_RGN_CLIP_H:
    {
        UINT32 u4SrcWidth, u4OldHClip;
        // recover orginal input width
        IGNORE_RET(_OSD_RGN_GetHClip(u4Region, &u4OldHClip));
        IGNORE_RET(_OSD_RGN_GetInputWidth(u4Region, &u4SrcWidth));
        u4SrcWidth += u4OldHClip;
        ASSERT(u4SrcWidth > u4Value);
        // cut input width
        IGNORE_RET(_OSD_RGN_SetInputWidth(u4Region, u4SrcWidth - u4Value));

        return _OSD_RGN_SetHClip(u4Region, u4Value);
    }

    case OSD_RGN_PAL_LOAD:
        return _OSD_RGN_SetLoadPalette(u4Region, u4Value);

    case OSD_RGN_PAL_ADDR:
        return _OSD_RGN_SetPaletteAddr(u4Region, u4Value);

    case OSD_RGN_PAL_LEN:
        return _OSD_RGN_SetPaletteLen(u4Region, u4Value);

    case OSD_RGN_STEP_V:
        return _OSD_RGN_SetVStep(u4Region, u4Value);

    case OSD_RGN_STEP_H:
        return _OSD_RGN_SetHStep(u4Region, u4Value);

    case OSD_RGN_COLOR_KEY:
        return _OSD_RGN_SetColorKey(u4Region, u4Value);

    case OSD_RGN_COLOR_KEY_EN:
        return _OSD_RGN_SetColorKeyEnable(u4Region, u4Value);

    case OSD_RGN_MIX_SEL:
        return _OSD_RGN_SetBlendMode(u4Region, u4Value);

    case OSD_RGN_BIG_ENDIAN:
        return OSD_RGN_SetBigEndian(u4Region, u4Value);

    case OSD_RGN_V_FLIP:
        return _OSD_RGN_SetVFlip(u4Region, u4Value);

    case OSD_RGN_H_MIRROR:
        return _OSD_RGN_SetHMirror(u4Region, u4Value);

    case OSD_RGN_DEC_MODE:
        return _OSD_RGN_SetDecMode(u4Region, u4Value);
    case OSD_RGN_DEC_LINE:
        return _OSD_RGN_SetDecLine(u4Region, u4Value);
    case OSD_RGN_DEC_EN:
        return _OSD_RGN_SetDecEn(u4Region, u4Value);
        
    //case OSD_RGN_HPX_EXT:
    //    return _OSD_RGN_SetHpxExt(u4Region, u4Value);

    //case OSD_RGN_VINTP_MODE:
    //    return _OSD_RGN_SetVIntpMode(u4Region, u4Value);

    case OSD_RGN_ENDIAN:
        return _OSD_RGN_SetEndian(u4Region, u4Value);

    case OSD_RGN_HINTP_MODE:
        return _OSD_RGN_SetHIntMode(u4Region, u4Value);

    case OSD_RGN_VSTEP_OFFSET:
        return _OSD_RGN_SetVStepOffset(u4Region, u4Value);

    case OSD_RGN_HSTEP_OFFSET:
        return _OSD_RGN_SetHStepOffset(u4Region, u4Value);

    default:
        return -(INT32)OSD_RET_INV_ARG;
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** create an osd region
 *  @param  pu4Region region id pointer
 *  @param  u4BmpWidth source bitmap width
 *  @param  u4BmpHeight source bitmap height
 *  @param  pvBitmap source bitmap address, MUST align with 16bytes
 *  @param  eColorMode, region color mode, refer to OSD_COLOR_MODE_T
 *  @param  u4BmpPitch, bytes per line, MUST align with 16bytes
 *  @param  u4DispX, region x position
 *  @param  u4DispY, region y position
 *  @param  u4DispW, output width
 *  @param  u4DispH, output height

 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_ARG invalid argument
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Create(UINT32 * pu4Region, UINT32 u4BmpWidth, UINT32 u4BmpHeight,
                     void * pvBitmap, UINT32 eColorMode, UINT32 u4BmpPitch,
                     UINT32 u4DispX, UINT32 u4DispY,
                     UINT32 u4DispW, UINT32 u4DispH)
{
    UINT32 u4Region;
    INT32 i4Ret;
    #ifdef CC_FLIP_MIRROR_SUPPORT
    //UINT32 u4FlipMirror_x,u4FlipMirror_y,scn_h,scn_w;
    UINT32 scn_h,scn_w;
    #endif

    if ((pu4Region == NULL) || (pvBitmap == NULL) ||
        (((UINT32)pvBitmap & 0xf) != 0) || (u4BmpWidth == 0) ||
        (u4BmpHeight == 0) || (u4DispW == 0) || (u4DispH == 0) ||
        ((u4BmpPitch & 0xf) != 0) || ((u4BmpPitch >> 13) != 0))
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    i4Ret = _OSD_RGN_Alloc(&u4Region);
    if (i4Ret != (INT32)OSD_RET_OK)
    {
        return i4Ret;
    }

    i4Ret = _OSD_RGN_SetNextRegion(u4Region, 0);
    i4Ret = _OSD_RGN_SetNextEnable(u4Region, 0);
    i4Ret = _OSD_RGN_SetColorMode(u4Region, eColorMode);
    i4Ret = _OSD_RGN_SetDataAddr(u4Region, ((UINT32)pvBitmap));
    i4Ret = _OSD_RGN_SetAlpha(u4Region, (UINT32)0xff);
    i4Ret = _OSD_RGN_SetInputWidth(u4Region, u4BmpWidth);
    i4Ret = _OSD_RGN_SetInputHeight(u4Region, u4BmpHeight);
    i4Ret = _OSD_RGN_SetOutputWidth(u4Region, u4DispW);
    i4Ret = _OSD_RGN_SetOutputHeight(u4Region, u4DispH);
    i4Ret = _OSD_RGN_SetOutputPosX(u4Region, u4DispX);
    i4Ret = _OSD_RGN_SetOutputPosY(u4Region, u4DispY);
    i4Ret = _OSD_RGN_SetColorKeyEnable(u4Region, 0);
    i4Ret = _OSD_RGN_SetColorKey(u4Region, 0);
    i4Ret = _OSD_RGN_SetHClip(u4Region, 0);
    i4Ret = _OSD_RGN_SetVClip(u4Region, 0);
    i4Ret = _OSD_RGN_SetAutoMode(u4Region, 1);

    if (u4BmpPitch == 0)
    {
        if ((eColorMode == (UINT32)OSD_CM_YCBCR_CLUT2) ||
            (eColorMode == (UINT32)OSD_CM_RGB_CLUT2))
        {
            u4BmpPitch = u4BmpWidth >> 2;
        }
        else if ((eColorMode == (UINT32)OSD_CM_YCBCR_CLUT4) ||
                 (eColorMode == (UINT32)OSD_CM_RGB_CLUT4))
        {
            u4BmpPitch = u4BmpWidth >> 1;
        }
        else if ((eColorMode == (UINT32)OSD_CM_YCBCR_CLUT8) ||
                 (eColorMode == (UINT32)OSD_CM_RGB_CLUT8))
        {
            u4BmpPitch = u4BmpWidth;
        }
        else if ((eColorMode == (UINT32)OSD_CM_AYCBCR8888_DIRECT32) ||
                 (eColorMode == (UINT32)OSD_CM_ARGB8888_DIRECT32))
        {
            u4BmpPitch = u4BmpWidth << 2;
        }
        else if (eColorMode == (UINT32)OSD_CM_ALPHA_BIT)
        {
            u4BmpPitch = u4BmpWidth >> 3;
        }
        else
        {
            u4BmpPitch = u4BmpWidth << 1;
        }
    }
    i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_BMP_PITCH, u4BmpPitch);
/*    i4Ret = _OSD_RGN_SetLineSize(u4Region, (u4BmpPitch >> 4) & 0x1ff);
    i4Ret = _OSD_RGN_SetLineSize8(u4Region, u4BmpPitch >> 13);*/
    i4Ret = _OSD_RGN_SetHStep(u4Region, (u4BmpWidth  == u4DispW) ? 0x1000 :
                              ((u4BmpWidth << 12)  / u4DispW));
    i4Ret = _OSD_RGN_SetVStep(u4Region, (u4BmpHeight == u4DispH) ? 0x1000 :
                              ((u4BmpHeight << 12) / u4DispH));
    i4Ret = _OSD_RGN_SetBlendMode(u4Region, (UINT32)OSD_BM_REGION);

    i4Ret = _OSD_RGN_SetASel(u4Region, 3);
    i4Ret = _OSD_RGN_SetYrSel(u4Region, 2);
    i4Ret = _OSD_RGN_SetUgSel(u4Region, 1);
    i4Ret = _OSD_RGN_SetVbSel(u4Region, 0);
     // palette mode
    if (((eColorMode <= (UINT32)OSD_CM_YCBCR_CLUT8) ||
        (eColorMode >= (UINT32)OSD_CM_RGB_CLUT2)) &&
         (eColorMode <= (UINT32)OSD_CM_RGB_CLUT8))
    {
        if ((eColorMode == (UINT32)OSD_CM_YCBCR_CLUT2) ||
            (eColorMode == (UINT32)OSD_CM_RGB_CLUT2))
        {
            i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_PAL_LEN, 0);
        }
        else if ((eColorMode == (UINT32)OSD_CM_YCBCR_CLUT4) ||
                 (eColorMode == (UINT32)OSD_CM_RGB_CLUT4))
        {
            i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_PAL_LEN, 1);
        }
        else if ((eColorMode == (UINT32)OSD_CM_YCBCR_CLUT8) ||
                 (eColorMode == (UINT32)OSD_CM_RGB_CLUT8))
        {
            i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_PAL_LEN, 2);
        }
        i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_PAL_ADDR,
                            (((UINT32)pvBitmap +
                            (u4BmpPitch * u4BmpHeight) + 0x2000) & ~0x1fff));
        i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_PAL_LOAD, 1);
    }

	#ifdef CC_FLIP_MIRROR_SUPPORT
	if(gfgOSD_Flip)
	{
	    // i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_H_MIRROR, 1);
	    i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_V_FLIP, 1);
	    //i4Ret=_OSD_BASE_GetScrnHSize(&scn_w);
	    i4Ret=_OSD_BASE_GetScrnVSize(&scn_h);
	    //i4Ret = _OSD_RGN_SetOutputPosX(u4Region, scn_w-(u4DispX+u4BmpWidth));
           i4Ret = _OSD_RGN_SetOutputPosY(u4Region, scn_h-(u4DispY+u4BmpHeight));
           _rAllRgnNode[u4Region].u4Original_Y=u4DispY;
	}
	if(gfgOSD_Mirror)
	{
	     i4Ret = OSD_RGN_Set(u4Region, (INT32)OSD_RGN_H_MIRROR, 1);
	    i4Ret=_OSD_BASE_GetScrnHSize(&scn_w);
	    i4Ret = _OSD_RGN_SetOutputPosX(u4Region, scn_w-(u4DispX+u4BmpWidth));
           _rAllRgnNode[u4Region].u4Original_X=u4DispX;
	}
	#endif
	
    UNUSED(i4Ret);
    _rAllRgnNode[u4Region].u4NodeStatus |= OSD_RGN_NODE_ALLOCATED;
    _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
    _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
    *pu4Region = u4Region;
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** delete an osd region
 *  @param  u4Region region id
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Delete(UINT32 u4Region)
{
    OSD_VERIFY_REGION(u4Region);

    if (_rAllRgnNode[u4Region].i4Prev != OSD_INVALID_REGION_HANDLE)
    {
        INT32 i4Prev;
        i4Prev = _rAllRgnNode[u4Region].i4Prev;
        if ((i4Prev >= 0) && (i4Prev < OSD_MAX_NUM_RGN))
        {
            // assign [i4Prev].i4Next = [u4Region].i4Next
            _rAllRgnNode[i4Prev].i4Next = _rAllRgnNode[u4Region].i4Next;
        }
    }

    if (_rAllRgnNode[u4Region].i4Next != OSD_INVALID_REGION_HANDLE)
    {
        INT32 i4Next;
        i4Next = _rAllRgnNode[u4Region].i4Next;
        if ((i4Next >= 0) && (i4Next < OSD_MAX_NUM_RGN))
        {
            // assign [i4Next].i4Prev = [u4Region].i4Prev
            _rAllRgnNode[i4Next].i4Prev = _rAllRgnNode[u4Region].i4Prev;
        }
    }

    VERIFY((INT32)OSD_RET_OK == _OSD_RGN_Free(u4Region));
    _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
    _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
    _rAllRgnNode[u4Region].u4NodeStatus &= ~OSD_RGN_NODE_ALLOCATED;

    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_AttachTail(UINT32 u4Region, UINT32 u4RgnList)
{
    INT32 i4OrigTail;
    UINT32 u4RgnAddr;
    UINT32 u4ColorMode;
    OSD_REGION_LIST_T * prRgnList;
    INT32 i4Ignore;

    OSD_VERIFY_REGION(u4Region);
    OSD_VERIFY_RGNLIST(u4RgnList);

    prRgnList = &_rAllRgnList[u4RgnList];
    i4OrigTail = prRgnList->i4Tail;

    if ((prRgnList->i4Head == NULL) && (prRgnList->i4Count == 0))
    {
        return OSD_RGN_Insert(u4Region, u4RgnList);
    }

    if (i4OrigTail == OSD_INVALID_REGION_HANDLE)
    {
        return -(INT32)OSD_RET_INV_LIST;
    }

    _rAllRgnList[u4RgnList].i4Count++;
    _rAllRgnList[u4RgnList].i4Tail = (INT32)u4Region;
    _rAllRgnNode[i4OrigTail].i4Next = (INT32)u4Region;
    _rAllRgnNode[u4Region].i4Prev = i4OrigTail;

    i4Ignore = _OSD_RGN_GetAddress(u4Region, &u4RgnAddr);
    ASSERT((u4RgnAddr & 0xf) == 0);
    i4Ignore = _OSD_RGN_SetNextRegion((UINT32)i4OrigTail, u4RgnAddr);
    i4Ignore = _OSD_RGN_SetNextEnable((UINT32)i4OrigTail, 1);
    i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 0); // tail means 'no next'
    i4Ignore = _OSD_RGN_SetNextRegion(u4Region, 0);

    i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
    if ((u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT2) ||
        (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT4) ||
        (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT8) ||
        (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT2) ||
        (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT4) ||
        (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8))
    {
        // check if we should load new palette
        UINT32 u4Pal1, u4Pal2;
        UINT32 u4Len1, u4Len2;
        i4Ignore = _OSD_RGN_GetPaletteAddr(u4Region, &u4Pal1);
        i4Ignore = _OSD_RGN_GetPaletteLen(u4Region, &u4Len1);
        i4Ignore = _OSD_RGN_GetPaletteAddr((UINT32)i4OrigTail, &u4Pal2);
        i4Ignore = _OSD_RGN_GetPaletteLen((UINT32)i4OrigTail, &u4Len2);
        i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, ((u4Pal1 == u4Pal2) &&
                                           (u4Len1 == u4Len2)) ? 0 : 1);
    }
    else
    {
        // not index color mode, should not load palette
        i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
    }

    UNUSED(i4Ignore);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** attach region to list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_LIST invalid list
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Insert(UINT32 u4Region, UINT32 u4RgnList)
{
    INT32 i4RgnCurr, i4RgnNext;
    OSD_REGION_LIST_T * prRgnList;
    UINT32 u4ColorMode, u4Address;
    INT32 i4Ignore = 0;

    OSD_VERIFY_REGION(u4Region);
    OSD_VERIFY_RGNLIST(u4RgnList);

    prRgnList = &_rAllRgnList[u4RgnList];
    i4RgnCurr = prRgnList->i4Head;
    if (i4RgnCurr == OSD_INVALID_REGION_HANDLE)
    {
        if (prRgnList->i4Count == 0)
        {
            _rAllRgnList[u4RgnList].i4Head = (INT32)u4Region;
            _rAllRgnList[u4RgnList].i4Tail = (INT32)u4Region;
            _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
            _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
            _rAllRgnList[u4RgnList].i4Count = 1;

            i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 0);
            i4Ignore = _OSD_RGN_SetNextRegion(u4Region, 0);

            i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
            if ((u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT2) ||
                (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT4) ||
                (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT8) ||
                (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT2) ||
                (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT4) ||
                (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8))
            {

                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 1);
            }
            else
            {
                // not index color mode, should not load palette
                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
            }
            return (INT32)OSD_RET_OK;
        }
        else
        {
            return -(INT32)OSD_RET_INV_LIST;
        }
    }

    OSD_VERIFY_REGION(i4RgnCurr);

    switch (_OsdRegionCompare(u4Region, (UINT32)i4RgnCurr))
    {
    case -1:
        prRgnList->i4Head = (INT32)u4Region;
        i4Ignore = OSD_PLA_Reflip(u4RgnList);
        _rAllRgnNode[u4Region].i4Next = i4RgnCurr;
        _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4RgnCurr].i4Prev = (INT32)u4Region;
        prRgnList->i4Count++;

        i4Ignore = _OSD_RGN_GetAddress((UINT32)i4RgnCurr, &u4Address);
        i4Ignore = _OSD_RGN_SetNextRegion(u4Region, u4Address);
        i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 1);


        i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
        if ((u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT2) ||
            (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT4) ||
            (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT8) ||
            (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT2) ||
            (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT4) ||
            (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8))
        {

            i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 1);
        }
        else
        {
            // not index color mode, should not load palette
            i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
        }

        return (INT32)OSD_RET_OK;

    case 1:
        break;

    default:
        return -(INT32)OSD_RET_REGION_COLLISION;
    }

    while ((i4RgnNext = _rAllRgnNode[i4RgnCurr].i4Next) !=
           OSD_INVALID_REGION_HANDLE)
    {
        if ((i4RgnNext < 0) || (i4RgnNext >= OSD_MAX_NUM_RGN))
        {
            i4RgnNext = OSD_INVALID_REGION_HANDLE;
            _rAllRgnNode[i4RgnCurr].i4Next = i4RgnNext;
            break;
        }

        switch (_OsdRegionCompare(u4Region, (UINT32)i4RgnNext))
        {
        case -1:
            // insert
            _rAllRgnNode[i4RgnCurr].i4Next = (INT32)u4Region;
            _rAllRgnNode[u4Region].i4Next = i4RgnNext;
            _rAllRgnNode[i4RgnNext].i4Prev = (INT32)u4Region;
            _rAllRgnNode[u4Region].i4Prev = i4RgnCurr;
            prRgnList->i4Count++;

            // maintain hw list
            i4Ignore = _OSD_RGN_GetAddress(u4Region, &u4Address);
            // current --> region
            i4Ignore = _OSD_RGN_SetNextRegion((UINT32)i4RgnCurr, u4Address);
            i4Ignore = _OSD_RGN_GetAddress((UINT32)i4RgnNext, &u4Address);
            // region --> next
            i4Ignore = _OSD_RGN_SetNextRegion(u4Region, u4Address);

            if (i4RgnNext != OSD_INVALID_REGION_HANDLE)
            {
                // valid next
                i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 1);
            }
            else
            {
                // no next
                i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 0);
            }
            i4Ignore = _OSD_RGN_SetNextEnable((UINT32)i4RgnCurr, 1);


            i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
            if ((u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT2) ||
                (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT4) ||
                (u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT8) ||
                (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT2) ||
                (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT4) ||
                (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8))
            {
                // check if we should load new palette
                UINT32 u4Pal1, u4Pal2;
                UINT32 u4Len1, u4Len2;
                i4Ignore = _OSD_RGN_GetPaletteAddr(u4Region, &u4Pal1);
                i4Ignore = _OSD_RGN_GetPaletteLen(u4Region, &u4Len1);
                i4Ignore = _OSD_RGN_GetPaletteAddr((UINT32)i4RgnCurr, &u4Pal2);
                i4Ignore = _OSD_RGN_GetPaletteLen((UINT32)i4RgnCurr, &u4Len2);
                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region,
                                                   ((u4Pal1 == u4Pal2) &&
                                                   (u4Len1 == u4Len2)) ? 0 : 1);
            }
            else
            {
                // not index color mode, should not load palette
                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
            }
            return (INT32)OSD_RET_OK;

        case 1:
            i4RgnCurr = i4RgnNext;
            break;

        default:
            return -(INT32)OSD_RET_REGION_COLLISION;
        }
    }

    UNUSED(i4Ignore);

    _rAllRgnNode[i4RgnCurr].i4Next = i4RgnNext;
    return OSD_RGN_AttachTail(u4Region, u4RgnList);
}


//-----------------------------------------------------------------------------
/** dettach region from list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_REGION invalid region
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Detach(UINT32 u4Region, UINT32 u4RgnList)
{
    INT32 i4RgnCurr, i4RgnNext, i4RgnPrev;
    OSD_REGION_LIST_T * prRgnList;
    UINT32 u4Address;
    INT32 i4Ignore = 0;

    OSD_VERIFY_REGION(u4Region);
    OSD_VERIFY_RGNLIST(u4RgnList);

    prRgnList = &_rAllRgnList[u4RgnList];
    i4RgnCurr = prRgnList->i4Head;

    if (prRgnList->i4Head == OSD_INVALID_REGION_HANDLE)
    {
        return -(INT32)OSD_RET_INV_REGION;
    }

    while(i4RgnCurr != OSD_INVALID_REGION_HANDLE)
    {
        OSD_VERIFY_REGION(i4RgnCurr);
        if ((UINT32)i4RgnCurr == u4Region)
        {
            i4RgnNext = _rAllRgnNode[u4Region].i4Next;
            i4RgnPrev = _rAllRgnNode[u4Region].i4Prev;
            if (i4RgnNext != OSD_INVALID_REGION_HANDLE)
            {
                _rAllRgnNode[i4RgnNext].i4Prev = i4RgnPrev;
            }
            else // region is tail
            {
                // attached to list tail
                prRgnList->i4Tail = i4RgnPrev;
            }

            if (i4RgnPrev != OSD_INVALID_REGION_HANDLE)
            {
                _rAllRgnNode[i4RgnPrev].i4Next = i4RgnNext;
            }
            else // region is head
            {
                // new list head
                prRgnList->i4Head = i4RgnNext;
            }

            // maintain hardware list
            if ((i4RgnNext == OSD_INVALID_REGION_HANDLE) &&
                (i4RgnPrev != OSD_INVALID_REGION_HANDLE))
            {
                // region is tail
                i4Ignore = _OSD_RGN_SetNextRegion((UINT32)i4RgnPrev, 0);
                i4Ignore = _OSD_RGN_SetNextEnable((UINT32)i4RgnPrev, 0);
            }
            else if ((i4RgnNext != OSD_INVALID_REGION_HANDLE)
                     && (i4RgnPrev == OSD_INVALID_REGION_HANDLE))
            {
                // region is head, re-flip if necessary
                i4Ignore = OSD_PLA_Reflip(u4RgnList);
            }
            else if ((i4RgnNext != OSD_INVALID_REGION_HANDLE)
                     && (i4RgnPrev != OSD_INVALID_REGION_HANDLE))
            {
                i4Ignore = _OSD_RGN_GetAddress((UINT32)i4RgnNext, &u4Address);
                i4Ignore = _OSD_RGN_SetNextRegion((UINT32)i4RgnPrev, u4Address);
            }
            else
            {
                /*
                (i4RgnNext == OSD_INVALID_REGION_HANDLE) && \
                (i4RgnPrev == OSD_INVALID_REGION_HANDLE)
                */
            }

            _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
            _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
            prRgnList->i4Count--;
            if (prRgnList->i4Count == 0)
            {
                prRgnList->i4Head = OSD_INVALID_REGION_HANDLE;
                prRgnList->i4Tail = OSD_INVALID_REGION_HANDLE;
            }

            return (INT32)OSD_RET_OK;
        }
        else
        {
            i4RgnCurr = _rAllRgnNode[i4RgnCurr].i4Next;
        }
    }

    UNUSED(i4Ignore);
    return (INT32)OSD_RET_INV_REGION;
}


INT32 OSD_RGN_SetDisplayWidth(UINT32 u4Region, UINT32 u4Width)
{
    UINT32 u4SrcW;
    UINT32 u4Step;
    INT32 i4Ignore;

    OSD_VERIFY_REGION(u4Region);

    if (u4Width == 0)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    i4Ignore = _OSD_RGN_GetInputWidth(u4Region, &u4SrcW);
    i4Ignore = _OSD_RGN_SetOutputWidth(u4Region, u4Width);
    u4Step = (u4SrcW == u4Width) ? 0x1000 : ((u4SrcW << 12) / u4Width);
    i4Ignore = _OSD_RGN_SetHStep(u4Region, (u4Step > 0xffff) ? 0xffff : u4Step);
    UNUSED(i4Ignore);

    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_SetDisplayHeight(UINT32 u4Region, UINT32 u4Height)
{
    UINT32 u4SrcH;
    UINT32 u4Step;
    INT32 i4Ignore;

    OSD_VERIFY_REGION(u4Region);

    if (u4Height == 0)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    i4Ignore = _OSD_RGN_GetInputHeight(u4Region, &u4SrcH);
    i4Ignore = _OSD_RGN_SetOutputHeight(u4Region, u4Height);
    u4Step = (u4SrcH == u4Height) ? 0x1000 : ((u4SrcH << 12) / u4Height);
    i4Ignore = _OSD_RGN_SetVStep(u4Region, (u4Step > 0xffff) ? 0xffff : u4Step);
    UNUSED(i4Ignore);

    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_SetBigEndian(UINT32 u4Region, BOOL fgBE)
{
    OSD_VERIFY_REGION(u4Region);
    if (fgBE)
    {
        IGNORE_RET(_OSD_RGN_SetASel(u4Region, 0));
        IGNORE_RET(_OSD_RGN_SetYrSel(u4Region, 1));
        IGNORE_RET(_OSD_RGN_SetUgSel(u4Region, 2));
        IGNORE_RET(_OSD_RGN_SetVbSel(u4Region, 3));
    }
    else
    {
        IGNORE_RET(_OSD_RGN_SetASel(u4Region, 3));
        IGNORE_RET(_OSD_RGN_SetYrSel(u4Region, 2));
        IGNORE_RET(_OSD_RGN_SetUgSel(u4Region, 1));
        IGNORE_RET(_OSD_RGN_SetVbSel(u4Region, 0));
    }
    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_Dump(UINT32 u4Region)
{
    UINT32 u4Prev, u4Next, u4Flag, u4PosX, u4PosY;
    UINT32 u4BmpW, u4BmpH, u4DispW, u4DispH, u4ColorMode;
    UINT32 u4Alpha, u4BmpAddr, u4BmpPitch, u4ClipV, u4ClipH;
    UINT32 u4PalLoad, u4PalAddr, u4PalLen, u4StepV, u4StepH;
    UINT32 u4ColorKey, u4ColorKeyEn, u4MixSel, u4FifoEx, u4BigEndian;
    UINT32 u4VFlip, u4HMirror;

    OSD_VERIFY_REGION(u4Region);

    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_PREV, &u4Prev));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_NEXT, &u4Next));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_FLAGS, &u4Flag));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_POS_X, &u4PosX));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_POS_Y, &u4PosY));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_BMP_W, &u4BmpW));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_BMP_H, &u4BmpH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_DISP_W, &u4DispW));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_DISP_H, &u4DispH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_COLORMODE, &u4ColorMode));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_ALPHA, &u4Alpha));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_BMP_ADDR, &u4BmpAddr));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_BMP_PITCH, &u4BmpPitch));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_CLIP_V, &u4ClipV));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_CLIP_H, &u4ClipH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_PAL_LOAD, &u4PalLoad));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_PAL_ADDR, &u4PalAddr));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_PAL_LEN, &u4PalLen));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_STEP_V, &u4StepV));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_STEP_H, &u4StepH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_COLOR_KEY, &u4ColorKey));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_COLOR_KEY_EN, &u4ColorKeyEn));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_MIX_SEL, &u4MixSel));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_V_FLIP, &u4VFlip));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_H_MIRROR, &u4HMirror));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_FIFO_EX, &u4FifoEx));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_BIG_ENDIAN, &u4BigEndian));

    Printf("REGION[%d]\n", u4Region);
    Printf("\tCM=%d, SrcSize=(%d, %d), DstSize(%d, %d)\n",
           u4ColorMode, u4BmpW, u4BmpH, u4DispW, u4DispH);
    Printf("\tPos=(%d, %d), Clip=(%d, %d), Step=(0x%04x, 0x%04x)\n",
           u4PosX, u4PosY, u4ClipH, u4ClipV, u4StepH, u4StepV);
    Printf("\tAlpha=%d, BlendMode=%d, Key=0x%08x, KeyEn=%d\n",
           u4Alpha, u4MixSel, u4ColorKey, u4ColorKeyEn);
    Printf("\tBmpAddr=0x%08x, PalAddr=0x%08x, PalLen=%d, PalLoad=%d\n",
           u4BmpAddr, u4PalAddr, u4PalLen, u4PalLoad);
    Printf("\tVFlip=%d, HMirror=%d, LongFifo=%d, Pitch=%d\n",
           u4VFlip, u4HMirror, u4FifoEx, u4BmpPitch);
    return (INT32)OSD_RET_OK;
}

#ifdef CC_FLIP_MIRROR_SUPPORT
INT32 OSD_RGN_GetOriginalX(UINT32 u4Region, UINT32 * pu4Value)
{
	*pu4Value = (UINT32)_rAllRgnNode[u4Region].u4Original_X;
	return (INT32)OSD_RET_OK;
}
INT32 OSD_RGN_GetOriginalY(UINT32 u4Region, UINT32 * pu4Value)
{
	*pu4Value = (UINT32)_rAllRgnNode[u4Region].u4Original_Y;
	return (INT32)OSD_RET_OK;
}
#endif

//add for setting/getting the region list used by linux frame buffer
INT32 OSD_Set_FrameBuffer_RGN_LIST(UINT32 u4RgnList)
{
	u4RGNList_FrameBuffer=u4RgnList;
	return (INT32)OSD_RET_OK;
}
INT32 OSD_Get_FrameBuffer_RGN_LIST(UINT32 * pu4Value)
{
	if(OSD_MAX_NUM_RGN_LIST==u4RGNList_FrameBuffer) 
	{
		return (INT32)OSD_RET_UNINIT;
	}
	if(pu4Value == NULL) 
	{
		return (INT32)OSD_RET_INV_ARG;
	}
	*pu4Value = u4RGNList_FrameBuffer;
	return (INT32)OSD_RET_OK;
}

INT32 OSD_Get_FrameBuffer_Resolution(UINT32* pu4Width,UINT32* pu4Height)
{
	UINT32 u4width,u4height;
	
	if(OSD_MAX_NUM_RGN_LIST==u4RGNList_FrameBuffer) 
	{
		return (INT32)OSD_RET_UNINIT;
	}
	if( (pu4Width == NULL)||(pu4Height == NULL)) 
	{
		return (INT32)OSD_RET_INV_ARG;
	}
	_OSD_RGN_GetInputWidth(_rAllRgnList[u4RGNList_FrameBuffer].i4Head, &u4width);
	_OSD_RGN_GetInputHeight(_rAllRgnList[u4RGNList_FrameBuffer].i4Head, &u4height);
	*pu4Width = u4width;
	*pu4Height = u4height;
	//Printf("OSD_Get_FrameBuffer_Resolution w=%d,h=%d\n",(*pu4Width),(*pu4Height));
	return (INT32)OSD_RET_OK;
}

//For OSD scroll
INT32 OSD_Vsync_UpdateScroll(void)
{
	UINT32 u4CurAddr;
	
	if(_u1ScrollMode==1) // Scroll mode
	{
		OSD_RGN_Get(_u4ScrollRgn, OSD_RGN_BMP_ADDR, &u4CurAddr);
		
		u4CurAddr += _u4ScrollStep;
		
		if( u4CurAddr >= (_u4BaseAddr + _u4ScrollPitch) )
		{	// Need to loop
			u4CurAddr -= _u4ScrollPitch;
		}
		
		OSD_RGN_Set(_u4ScrollRgn, OSD_RGN_BMP_ADDR, u4CurAddr);
	}
	
	return (INT32)OSD_RET_OK;
}	

INT32 OSD_RGN_SetScrollInfo(UINT32 u4Rgn, UINT32 u4BaseAddress, UINT32 u4Pitch, UINT32 u4Step)
{
	if(OSD_MAX_NUM_RGN_LIST==u4Rgn) 
	{
		return (INT32)OSD_RET_UNINIT;
	}
	
	if( (u4BaseAddress == NULL)||(u4Pitch == NULL)||(u4Step == NULL)) 
	{
		return (INT32)OSD_RET_INV_ARG;
	}
	
	_u4ScrollRgn = u4Rgn;
	_u4BaseAddr = u4BaseAddress;
	_u4ScrollPitch = u4Pitch;
	_u4ScrollStep = u4Step;

	/*
	printf("OSD_RGN_SetScrollInfo : u4Rgn = %d, u4BaseAddress = 0x%x, u4Pitch = %d, u4Step = %d\n",
			_u4ScrollRgn, _u4BaseAddr, _u4ScrollPitch, _u4ScrollStep);
	*/
	
	return (INT32)OSD_RET_OK;
}

INT32 OSD_SetScrollMode(UINT8 u1Mode)
{
	_u1ScrollMode = u1Mode;
	//printf("OSD_SetScrollMode : _u1ScrollMode = %d\n", _u1ScrollMode );
	return (INT32)OSD_RET_OK;
}

