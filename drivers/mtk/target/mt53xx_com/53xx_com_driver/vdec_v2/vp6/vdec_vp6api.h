
/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp6.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp6.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_VP6_API_H
#define VDEC_VP6_API_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
LINT_EXT_HEADER_END
;

typedef enum
{
    VP6_DISP_IDX = 0,
    VP6_BS_TYPE,
    VP6_RRMODE,
    VP6_OUTPUT_DISP_ORDER,
    VP6_REPEAT_FRM,
    VP6_ECC_MODE,
    VP6_ECC_TYPE,
    VP6_DRAM_BANDWITH_SIMU,
    VP6_LOG_SIMU_PAT,
    VP6_WAIT_FB_TIME,
    VP6_AV_SYNC_INFO,
    VP6_TIME_PROFILE,
    VP6_TIME_PROFILE2,
    VP6_TIME_PROFILE3,
    VP6_MM_PARAM,
    VP6_SEQ_HDR_INFO,
    VP6_PARAM_BLK_TYPE = 0x100,
    VP6_PARAM_DBK_SUPPORT,
    VP6_SET_DEC_MODE,
}   VP6_PARAM_T;

extern INT32 _VDEC_Vp6Init(UINT8 u1EsId);
extern INT32 _VDEC_Vp6HdrPrs(UCHAR ucVdecId, UCHAR ucEsId);
extern INT32 _VDEC_Vp6DecStart(UCHAR ucVdecId, UCHAR ucEsId);
extern INT32 _VDEC_Vp6DecFin(UCHAR ucVdecId, UCHAR ucEsId);
extern void _VDEC_Vp6SWRst(UCHAR ucVdecId, UCHAR ucEsId);
extern void _VDEC_Vp6SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3);
extern void _VDEC_Vp6GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3);
extern void _VDEC_Vp6Stop(UCHAR ucEsId);
extern void _VDEC_Vp6GetPesStruct(UCHAR ucEsId, void **ppvPesInfo);
extern void _VDEC_Vp6DropFrame(UCHAR ucEsId);
extern void _VDEC_Vp6GetDispPic(UCHAR ucEsId, void* pvInfo);
extern void _VDEC_Vp6UpdDmxRptr(UCHAR ucEsId);
extern void _VDEC_VP6SetDmxWptr(UCHAR ucEsId, UINT32 u4Wptr);
extern BOOL _VDEC_Vp6EsFlwCtrl(UCHAR ucEsId, UCHAR* pucVldId, INT8 i1EsBufStatus, UCHAR ucSkipMode);
extern void _VDEC_Vp6SetIsr(UCHAR ucEsId);
extern BOOL _VDEC_IsSupportVP6(void);
extern void _VDEC_VP6Flush(UCHAR ucEsId, BOOL bIsDisp);
extern BOOL _VDEC_VP6IsPic(UCHAR ucEsId, VDEC_PES_INFO_T *prPes);

#endif
