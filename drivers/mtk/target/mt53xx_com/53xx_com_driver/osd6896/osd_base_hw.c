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
 * $Date: 2013/01/24 $
 * $RCSfile: osd_base_hw.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_base_hw.c
 *  This header file includes function definitions of OSD base hardware...
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_timer.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/*lint -save -e960 */
#define OSD_BASE_SETGET_TMPL(NAME, FIELD) \
    INT32 _OSD_BASE_Set##NAME(UINT32 u4Value) \
    { \
        _rOsdBaseReg.rField.FIELD = u4Value; \
        return (INT32)OSD_RET_OK; \
    } \
    INT32 _OSD_BASE_Get##NAME(UINT32 *pu4Value) \
    { \
        if(pu4Value == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
        *pu4Value = _rOsdBaseReg.rField.FIELD; \
        return (INT32)OSD_RET_OK; \
    } \

/* above line is intendedly left blanc */
/*lint -restore */

#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
#ifdef CC_3D_OSD_SUPPORT
#define OSD_3D_SETGET_TMPL(NAME, FIELD) \
    INT32 _OSD_3D_Set##NAME(UINT32 u4Value) \
    { \
        _rOsd3DReg.rField.FIELD = u4Value; \
        return (INT32)OSD_RET_OK; \
    } \
    INT32 _OSD_3D_Get##NAME(UINT32 *pu4Value) \
    { \
        if(pu4Value == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
        *pu4Value = _rOsd3DReg.rField.FIELD; \
        return (INT32)OSD_RET_OK; \
    }
#endif  
#endif


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static OSD_BASE_UNION_T _rOsdBaseReg;
static volatile OSD_BASE_UNION_T* _prHwOsdBaseReg =
(OSD_BASE_UNION_T *)OSD_BASE_REG;

#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
static OSD_3D_UNION_T _rOsd3DReg;
static volatile OSD_3D_UNION_T *_prHwOsd3DReg =
    (OSD_3D_UNION_T *)OSD_3D_REG;
#endif


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _OSD_AlwaysUpdateReg(BOOL fgEnable)
{
    if (fgEnable)
    {
        IO_WRITE32MSK(OSD_BASE, 0, 2, 2);
    }
    else
    {
        IO_WRITE32MSK(OSD_BASE, 0, 0, 2);
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _OSD_UpdateReg(void)
{
    IO_WRITE32MSK(OSD_BASE, 0, 1, 1);
    //#ifdef CC_MT5363
    IO_WRITE32MSK(OSD_BASE, 0, 8, 8); //add for update OSD1 setting
    //#endif
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_BASE_GetReg(UINT32 *pOsdBaseReg)
{
    INT32 u4Idx = OSD_BASE_SKIP;

    if (pOsdBaseReg == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    for (; u4Idx<OSD_BASE_REG_NUM; u4Idx++)
    {
        pOsdBaseReg[u4Idx] = _rOsdBaseReg.au4Reg[u4Idx];
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_BASE_SetReg(const UINT32 *pOsdBaseReg)
{
    INT32 u4Idx = OSD_BASE_SKIP;

    if (pOsdBaseReg == NULL)
    {
        // to prevent trigger FSM reset
        IO_WRITE32(OSD_BASE, 4, 0);
        // delay to prevent hw crash
#ifndef CC_MTK_FPGA
        HAL_Delay_us(100);
#endif
      #if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
	IO_WRITE32(OSD_BASE, 4, 0x13);  
      #else
        IO_WRITE32(OSD_BASE, 4, 3);
     #endif

	//For syncing set data function in output stage
	IO_WRITE32MSK(OSD_BASE, 8, 0x800000,0x800000);
	IO_WRITE32MSK(OSD_BASE, 8, 0,0x800000);

        for (; u4Idx<OSD_BASE_REG_NUM; u4Idx++)
        {
            _rOsdBaseReg.au4Reg[u4Idx] = _prHwOsdBaseReg->au4Reg[u4Idx];
        }
    }
    else
    {
        for (; u4Idx<OSD_BASE_REG_NUM; u4Idx++)
        {
            _rOsdBaseReg.au4Reg[u4Idx] = pOsdBaseReg[u4Idx];
        }
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_BASE_UpdateHwReg(void)
{
    UINT32 u4Idx = OSD_BASE_SKIP;

    for (; u4Idx < OSD_BASE_REG_NUM; u4Idx++)
    {
        if ((u4Idx < OSD_BASE_SKIP_START) || (u4Idx > OSD_BASE_SKIP_END))
        {
            _prHwOsdBaseReg->au4Reg[u4Idx] = _rOsdBaseReg.au4Reg[u4Idx];
        }
    }
	
    _OSD_UpdateReg();

    return (INT32)OSD_RET_OK;
}

#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
#ifdef CC_3D_OSD_SUPPORT

INT32 _OSD_3D_SetReg(const UINT32 *p3DBaseReg)
{
  INT32 u4Idx = 0;
    if (p3DBaseReg == NULL)
    {  
        for (; u4Idx<2; u4Idx++)
        {
            _rOsd3DReg.au4Reg[u4Idx] = _prHwOsd3DReg->au4Reg[u4Idx];
        }
    }
    else
    {
        for (; u4Idx<2; u4Idx++)
        {
            _rOsd3DReg.au4Reg[u4Idx] = p3DBaseReg[u4Idx];
        }
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_3D_UpdateHwReg(void)
{
    UINT32 u4Idx = 0;

    for (; u4Idx < 2; u4Idx++)
    {
              _prHwOsd3DReg->au4Reg[u4Idx] = _rOsd3DReg.au4Reg[u4Idx];
    }

    _OSD_UpdateReg();
    return (INT32)OSD_RET_OK;
}
#endif
#endif

OSD_BASE_SETGET_TMPL(Update, fgUpdate)
OSD_BASE_SETGET_TMPL(AlwaysUpdate, fgAlwaysUpdate)
OSD_BASE_SETGET_TMPL(ResetAll, fgResetAll)
OSD_BASE_SETGET_TMPL(HsEdge, fgHsEdge)
OSD_BASE_SETGET_TMPL(VsEdge, fgVsEdge)
OSD_BASE_SETGET_TMPL(FldPol, fgFldPol)
OSD_BASE_SETGET_TMPL(Prgs, fgPrgs)
OSD_BASE_SETGET_TMPL(ChecksumEn, fgChecksumEn)
OSD_BASE_SETGET_TMPL(AutoPos, fgAutoPos)
OSD_BASE_SETGET_TMPL(YuvOut, fgYuvOut)
OSD_BASE_SETGET_TMPL(OSD12Ex, fgOSD12Ex)
OSD_BASE_SETGET_TMPL(VSyncSrcSel, fgVSyncSrcSel)
OSD_BASE_SETGET_TMPL(VsWidth, u4VsWidth)
OSD_BASE_SETGET_TMPL(Oht, u4Oht)
OSD_BASE_SETGET_TMPL(Ovt, u4Ovt)
OSD_BASE_SETGET_TMPL(ScrnHStartOsd2, u4ScrnHStartOsd2)
OSD_BASE_SETGET_TMPL(ScrnHStartOsd1, u4ScrnHStartOsd1)
OSD_BASE_SETGET_TMPL(ScrnHStartOsd3, u4ScrnHStartOsd3)
OSD_BASE_SETGET_TMPL(ScrnHStartCSR, u4ScrnHStartCSR)

OSD_BASE_SETGET_TMPL(ScrnVStartBot, u4ScrnVStartBot)
OSD_BASE_SETGET_TMPL(ScrnVStartTop, u4ScrnVStartTop)
OSD_BASE_SETGET_TMPL(ScrnVSize, u4ScrnVSize)
OSD_BASE_SETGET_TMPL(ScrnHSize, u4ScrnHSize)
OSD_BASE_SETGET_TMPL(Osd1VStart, u4Osd1VStart)
OSD_BASE_SETGET_TMPL(Osd1HStart, u4Osd1HStart)
OSD_BASE_SETGET_TMPL(Osd2VStart, u4Osd2VStart)
OSD_BASE_SETGET_TMPL(Osd2HStart, u4Osd2HStart)
//#ifdef CC_MT5363
OSD_BASE_SETGET_TMPL(Osd3VStart, u4Osd3VStart)
OSD_BASE_SETGET_TMPL(Osd3HStart, u4Osd3HStart)
//#endif

OSD_BASE_SETGET_TMPL(HsWidth, u4HsWidth)
OSD_BASE_SETGET_TMPL(DelayAdj, u4DelayAdj)

OSD_BASE_SETGET_TMPL(AutoLR, u4AutoLR)
OSD_BASE_SETGET_TMPL(LRFlag, u4LRFlag)

OSD_BASE_SETGET_TMPL(LineDup, u4LineDup)
OSD_BASE_SETGET_TMPL(LineShiftEn, u4LineShiftEn)
OSD_BASE_SETGET_TMPL(Plane2ShiftValue, u4Plane2ShiftValue)
OSD_BASE_SETGET_TMPL(Plane3ShiftValue, u4Plane3ShiftValue)


OSD_BASE_SETGET_TMPL(PlaneSwitch, u4PlaneSwitchEn)  


#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
OSD_BASE_SETGET_TMPL(OSD1Switch, fgOSD1Switch)
OSD_BASE_SETGET_TMPL(OSD2Switch, fgOSD2Switch)
OSD_BASE_SETGET_TMPL(OSD3Switch, fgOSD3Switch)

#ifdef CC_3D_OSD_SUPPORT

OSD_3D_SETGET_TMPL(TgenLREn, fgTgenLREn)
OSD_3D_SETGET_TMPL(Osd2LShiftEn, fgOsd2LShiftEn)
OSD_3D_SETGET_TMPL(PlaneSwitchEn, fgPlaneSwitchEn)

OSD_3D_SETGET_TMPL(ShutterShiftEn, fgShutterShiftEn)
OSD_3D_SETGET_TMPL(Osd1LShiftEn, fgOsd1LShiftEn)
OSD_3D_SETGET_TMPL(Osd3LShiftEn, fgOsd3LShiftEn)
OSD_3D_SETGET_TMPL(CsrLShiftEn, fgCsrLShiftEn)

OSD_3D_SETGET_TMPL(Osd2LShiftInv, fgOsd2LShiftInv)

OSD_3D_SETGET_TMPL(Osd1ShiftValue, fgOsd1ShiftValue)
OSD_3D_SETGET_TMPL(Osd2ShiftValue, fgOsd2ShiftValue)
OSD_3D_SETGET_TMPL(Osd3ShiftValue, fgOsd3ShiftValue)
OSD_3D_SETGET_TMPL(CsrShiftValue, fgCsrShiftValue)
#endif
#endif




