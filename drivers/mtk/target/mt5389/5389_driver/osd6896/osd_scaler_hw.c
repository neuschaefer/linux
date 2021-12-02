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
 * $RCSfile: osd_scaler_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_scaler_hw.c
 *  This header file includes function definitions of OSD scaler hardware.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/*lint -save -e960 */
#define OSD_SC_SETGET_TMPL(NAME, FIELD) \
    INT32 _OSD_SC_Set##NAME(UINT32 u4Scaler, UINT32 u4Value) \
    { \
        OSD_VERIFY_SCALER(u4Scaler); \
        _rOsdScalerReg[u4Scaler].rField.FIELD = u4Value; \
        return (INT32)OSD_RET_OK; \
    } \
    INT32 _OSD_SC_Get##NAME(UINT32 u4Scaler, UINT32 *pu4Value) \
    { \
        OSD_VERIFY_SCALER(u4Scaler); \
        if(pu4Value == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
        *pu4Value = _rOsdScalerReg[u4Scaler].rField.FIELD; \
        return (INT32)OSD_RET_OK; \
    } \

/* above line is intendedly left blanc */
/*lint -restore */


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static OSD_SC_UNION_T _rOsdScalerReg[OSD_SCALER_MAX_NUM];
static volatile OSD_SC_UNION_T *_prHwOsdScalerReg[] =
{
    (OSD_SC_UNION_T *)OSD_SC2_REG,
    (OSD_SC_UNION_T *)OSD_SC3_REG,
    #if defined(CC_MT5396)||defined(CC_MT5398)
    (OSD_SC_UNION_T *)OSD_SC1_REG,
    #endif
};


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_SC_GetReg(UINT32 u4Scaler, UINT32 *pOsdScalerReg)
{
    INT32 u4Idx = 0;
    OSD_VERIFY_SCALER(u4Scaler);

    if (pOsdScalerReg == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    for (; u4Idx<OSD_SC_REG_NUM; u4Idx++)
    {
        pOsdScalerReg[u4Idx] = _rOsdScalerReg[u4Scaler].au4Reg[u4Idx];
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_SC_SetReg(UINT32 u4Scaler, const UINT32 *pOsdScalerReg)
{
    INT32 u4Idx = 0;
    OSD_VERIFY_SCALER(u4Scaler);

    if (pOsdScalerReg == NULL)
    {
        for (; u4Idx<OSD_SC_REG_NUM; u4Idx++)
        {
            _rOsdScalerReg[u4Scaler].au4Reg[u4Idx] =
            _prHwOsdScalerReg[u4Scaler]->au4Reg[u4Idx];
        }
    }
    else
    {
        for (; u4Idx<OSD_SC_REG_NUM; u4Idx++)
        {
            _rOsdScalerReg[u4Scaler].au4Reg[u4Idx] = pOsdScalerReg[u4Idx];
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
INT32 _OSD_SC_UpdateHwReg(UINT32 u4Scaler)
{
    UINT32 u4Idx = 0;
    OSD_VERIFY_SCALER(u4Scaler);

    for (; u4Idx < OSD_SC_REG_NUM; u4Idx++)
    {
        _prHwOsdScalerReg[u4Scaler]->au4Reg[u4Idx] =
        _rOsdScalerReg[u4Scaler].au4Reg[u4Idx];
    }
    _OSD_UpdateReg();

    return (INT32)OSD_RET_OK;
}

#if defined(CC_MT5398)
OSD_SC_SETGET_TMPL(VdscEn, fgVdscEn)
OSD_SC_SETGET_TMPL(HdscEn, fgHdscEn)
OSD_SC_SETGET_TMPL(HdscStep, u4HdscStep)
OSD_SC_SETGET_TMPL(HdscOfst, u4HdscOfst)
#endif
OSD_SC_SETGET_TMPL(VuscEn, fgVuscEn)
//OSD_SC_SETGET_TMPL(VdscEn, fgVdscEn)
OSD_SC_SETGET_TMPL(HuscEn, fgHuscEn)
//OSD_SC_SETGET_TMPL(HdscEn, fgHdscEn)
//OSD_SC_SETGET_TMPL(ScLpfEn, fgScLpfEn)
OSD_SC_SETGET_TMPL(ScEn, fgScEn)
OSD_SC_SETGET_TMPL(SrcVSize, u4SrcVSize)
OSD_SC_SETGET_TMPL(SrcHSize, u4SrcHSize)
OSD_SC_SETGET_TMPL(DstVSize, u4DstVSize)
OSD_SC_SETGET_TMPL(DstHSize, u4DstHSize)
OSD_SC_SETGET_TMPL(VscHSize, u4VscHSize)
//OSD_SC_SETGET_TMPL(HdscStep, u4HdscStep)
//OSD_SC_SETGET_TMPL(HdscOfst, u4HdscOfst)
OSD_SC_SETGET_TMPL(HuscStep, u4HuscStep)
OSD_SC_SETGET_TMPL(HuscOfst, u4HuscOfst)
OSD_SC_SETGET_TMPL(VscOfstTop, u4VscOfstTop)
OSD_SC_SETGET_TMPL(VscOfstBot, u4VscOfstBot)
OSD_SC_SETGET_TMPL(VscStep, u4VscStep)
OSD_SC_SETGET_TMPL(AlphaCountEn, fgAlphaCountEn)
//OSD_SC_SETGET_TMPL(ScLpfC1, u4ScLpfC1)
//OSD_SC_SETGET_TMPL(ScLpfC2, u4ScLpfC2)
//OSD_SC_SETGET_TMPL(ScLpfC3, u4ScLpfC3)

