
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

#ifndef VDEC_VP6_HWDEC_H
#define VDEC_VP6_HWDEC_H
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdec_vp6hw.h"

// *********************************************************************
// Function
// *********************************************************************
extern void Vp6IsrInit(VDEC_VP6_INFO_T*);
extern BOOL _Vdec_VP6Hw_init(VDEC_VP6_INFO_T *);
extern INT32 _Vdec_Vp6HwDecode(VDEC_VP6_INFO_T *pDecInfo);
extern BOOL _Vdec_VP6HwGetHeaderInfo(VDEC_VP6_INFO_T *pDecInfo);
extern BOOL _Vdec_VP6Hw_DecWaitFinish(VDEC_VP6_INFO_T *pDecInfo);
extern BOOL _Vdec_VP6Hw_Release(UINT8 u1ucEsId);
extern BOOL _Vdec_VP6Hw_Reset(UINT8 u1ucEsId);
extern void _VDEC_SetVP6Wptr(UINT32 u4Wptr);
extern BOOL _VDEC_WaitVP6DecDone(VDEC_VP6_INFO_T *pDecInfo);
extern VOID _Vdec_VP6HwSetByteCount(UINT32 u4Id, UINT32 u4Ofst);

#endif
