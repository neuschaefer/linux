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
 * $RCSfile: osd_plane_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_plane_hw.c
 *  This header file includes function definitions of OSD plane hardware.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_drvif.h"
#include "osd_hw.h"


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/*lint -save -e960 */
#define OSD_PLA_SETGET_TMPL(NAME, FIELD) \
    INT32 _OSD_PLA_Set##NAME(UINT32 u4Plane, UINT32 u4Value) \
    { \
        OSD_VERIFY_PLANE(u4Plane); \
        _rOsdPlaneReg[u4Plane].rField.FIELD = u4Value; \
        return (INT32)OSD_RET_OK; \
    } \
    INT32 _OSD_PLA_Get##NAME(UINT32 u4Plane, UINT32 *pu4Value) \
    { \
        OSD_VERIFY_PLANE(u4Plane); \
        if(pu4Value == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
        *pu4Value = _rOsdPlaneReg[u4Plane].rField.FIELD; \
        return (INT32)OSD_RET_OK; \
    } \

/* above line is intendedly left blanc */
/*lint -restore */

/*lint -save -e960 */
#define OSD_PLA_SETGET_ADDRESS_TMPL(NAME, FIELD) \
    INT32 _OSD_PLA_Set##NAME(UINT32 u4Plane, UINT32 u4Value) \
    { \
        OSD_VERIFY_PLANE(u4Plane); \
        _rOsdPlaneReg[u4Plane].rField.FIELD = PHYSICAL(u4Value) >> 4; \
        return (INT32)OSD_RET_OK; \
    } \
    INT32 _OSD_PLA_Get##NAME(UINT32 u4Plane, UINT32 *pu4Value) \
    { \
        OSD_VERIFY_PLANE(u4Plane); \
        if(pu4Value == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
        *pu4Value = VIRTUAL(_rOsdPlaneReg[u4Plane].rField.FIELD << 4); \
        return (INT32)OSD_RET_OK; \
    } \

/* above line is intendedly left blanc */
/*lint -restore */


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static OSD_PLA_UNION_T _rOsdPlaneReg[OSD_PLANE_MAX_NUM];
static volatile OSD_PLA_UNION_T *_prHwOsdPlaneReg[] =
{
    (OSD_PLA_UNION_T *)OSD_PLA1_REG,
    (OSD_PLA_UNION_T *)OSD_PLA2_REG,
    (OSD_PLA_UNION_T *)OSD_PLA3_REG,
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
INT32 _OSD_PLA_GetReg(UINT32 u4Plane, UINT32 *pOsdPlaneReg)
{
    UINT32 u4Idx = 0;
    OSD_VERIFY_PLANE(u4Plane);

    if (pOsdPlaneReg == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    for (; u4Idx < OSD_PLA_REG_NUM; u4Idx++)
    {
        pOsdPlaneReg[u4Idx] = _rOsdPlaneReg[u4Plane].au4Reg[u4Idx];
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_PLA_SetReg(UINT32 u4Plane, const UINT32 *pOsdPlaneReg)
{
    UINT32 u4Idx = 0;
    OSD_VERIFY_PLANE(u4Plane);

    if (pOsdPlaneReg == NULL)
    {
        /* get a copy of reset value from hw */
        for (; u4Idx < OSD_PLA_REG_NUM; u4Idx++)
        {
            _rOsdPlaneReg[u4Plane].au4Reg[u4Idx] =
            _prHwOsdPlaneReg[u4Plane]->au4Reg[u4Idx];
        }
        return (INT32)OSD_RET_OK;
    }
    else
    {
        for (; u4Idx < OSD_PLA_REG_NUM; u4Idx++)
        {
            _rOsdPlaneReg[u4Plane].au4Reg[u4Idx] = pOsdPlaneReg[u4Idx];
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
INT32 _OSD_PLA_UpdateHwReg(UINT32 u4Plane)
{
    UINT32 u4Idx = 0;
    OSD_VERIFY_PLANE(u4Plane);

    for (; u4Idx < OSD_PLA_REG_NUM; u4Idx++)
    {
        _prHwOsdPlaneReg[u4Plane]->au4Reg[u4Idx] =
        _rOsdPlaneReg[u4Plane].au4Reg[u4Idx];
    }

    _OSD_UpdateReg();
    return (INT32)OSD_RET_OK;
}

OSD_PLA_SETGET_TMPL(Enable, fgEn)
OSD_PLA_SETGET_TMPL(FakeHdr, fgFakeHdr)
OSD_PLA_SETGET_TMPL(PrngEn, fgPrngEn)
OSD_PLA_SETGET_TMPL(PrngMode, fgPrngMode)
OSD_PLA_SETGET_TMPL(AEdgeDetEn, fgAEdgeDetEn)
OSD_PLA_SETGET_TMPL(CLKeySel, fgCLKeySel)
//OSD_PLA_SETGET_TMPL(DecEn, fgDecEn)
//OSD_PLA_SETGET_TMPL(AlphaMod, fgAlphaMod)
OSD_PLA_SETGET_TMPL(HFilter, fgHFilter)
//OSD_PLA_SETGET_TMPL(VFilter, fgVFilter)
OSD_PLA_SETGET_TMPL(Blending, u4Blending)
OSD_PLA_SETGET_ADDRESS_TMPL(HeaderAddr, u4HeaderAddr)
OSD_PLA_SETGET_TMPL(Fading, u4Fading)
OSD_PLA_SETGET_TMPL(ContReqLmt, u4ContReqLmt)
OSD_PLA_SETGET_TMPL(FifoSize, u4FifoSize)
//OSD_PLA_SETGET_TMPL(VacTh16, u4VacTh16)
//OSD_PLA_SETGET_TMPL(VacTh4, fgVacTh4)
