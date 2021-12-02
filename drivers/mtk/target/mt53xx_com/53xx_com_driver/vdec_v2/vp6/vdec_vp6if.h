
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

#ifndef VDEC_VP6_IF_H
#define VDEC_VP6_IF_H

#include "x_lint.h"
#include "x_hal_5381.h"
#include "x_bim.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdec_vp6util.h"
#include "vdec_vp6hw.h"

// *********************************************************************
// Function
// *********************************************************************
extern UINT32 u4VDEC_HAL_VP6_ShiftGetBitStream(UINT32 u4ShiftBits);
extern UINT32 u4VDEC_HAL_VP6_GetBitStreamShift(UINT32 u4ShiftBits);
extern BOOL i4VDEC_HAL_VP6_InitBarrelShifter(UINT32 u4Id, UINT32 u4ReadPointer, UINT32 u4WritePointer, VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr);
extern UINT32 u4VDEC_HAL_VP6_ReadRdPtr(UINT32 *pu4Bits);
extern void vVDEC_HAL_VP6_GetMbxMby(UINT32 *pu4Mbx, UINT32 *pu4Mby);
extern INT32 i4VDEC_HAL_VP6_DecStart(VDEC_VP6_INFO_T *pDecInfo);
extern UINT32 u4VDEC_HAL_VP6_GetBoolCoderShift(UINT32 u4ShiftBits);
extern UINT32 u4VDEC_HAL_VP6_InitBoolCoder(UINT32 u4Id);
extern UINT32 u4VDEC_HAL_VP6_Default_Models_Init(BOOL fgSupportMult);
extern UINT32 u4VDEC_HAL_VP6_Parse_Mb_Type_Models(BOOL fgSupportMult);
extern UINT32 u4VDEC_HAL_VP6_Load_QMatrix(void);
extern UINT32 u4VDEC_HAL_VP6_Load_Filter_Coef(UINT32 u4Select);
extern UINT32 u4VDEC_HAL_VP6_VDec_ReadFinishFlag(BOOL fgFinishParam);
extern void vVDEC_HAL_VP6_VDec_ClearFinishFlag(void);
extern void _VDEC_SetVP6BsInfo(UINT32 u4Id, UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr, UINT32 u4BsFifoStart, UINT32 u4BsFifoEnd, VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr);

#endif
