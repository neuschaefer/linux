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
 * $RCSfile: hal.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file hal.h
 *  Hardware abstraction rountines interface.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_os.h"
#include "c_model.h"
#include "x_assert.h"

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

extern UINT32 _ui4ResVirtStart;
extern UINT32 _ui4ResVirtEnd;
extern UINT32 _u4FbmPhyStart;
extern UINT32 _u4FbmPhyEnd;
extern UINT32 _u4MaxLowPhy;


#ifndef NDEBUG
// For debug version, check & warning if it is not direct mapped area
// or possible user address (>0x80000000 & < 0xc0000000 for now)
// We should warning on physical address, but it cause too much warning now.
#define __PHY_DEBUG_CHECK(addr)    \
          ((((UINT32)(addr) >= 0xc0000000)) ?                \
            (((UINT32)(addr) <= (UINT32)__va(_u4MaxLowPhy) ||\
               (UINT32)(addr) >= 0xF0000000) ? ((UINT32)__pa(addr)) :    \
            (ASSERT(0&&"PHYSICAL() called with bad value(vmap?)"),0)) :  \
            (((UINT32)(addr) < 0x80000000) ? (UINT32)(addr) : \
            (ASSERT(0&&"PHYSICAL() called with bad value(user address?)"),0)))

// Debug version, check & warning if it is possible virtual but not in direct mapped area.
// We should warning on virtual address, but it cause too much warning now.
#define __VIRT_DEBUG_CHECK(addr)   \
          (((UINT32)(addr) <= _u4MaxLowPhy) ?                \
            ((UINT32)__va(addr)) :                           \
            (((UINT32)(addr) >= 0xBF000000) ?                \
            ((UINT32)(addr)) : (ASSERT(0&&"VIRTUAL() called with bad value"),0)))
#else
#define __PHY_DEBUG_CHECK(addr)    \
          ((((UINT32)(addr) >= 0xc0000000)) ?               \
            (UINT32)__pa(addr) : (UINT32)(addr))

#define __VIRT_DEBUG_CHECK(addr)   \
          (((UINT32)(addr) <= _u4MaxLowPhy) ?                 \
            (UINT32)__va(addr) : (UINT32)(addr))
#endif /* NDEBUG */


#define __PHYSICAL(addr)     \
    (((addr >= _ui4ResVirtStart) && (addr < _ui4ResVirtEnd)) ?    \
          ((addr - _ui4ResVirtStart + _u4FbmPhyStart)) :        \
          __PHY_DEBUG_CHECK(addr))

#define __VIRTUAL(addr)      \
    (((addr >= _u4FbmPhyStart) && (addr < _u4FbmPhyEnd)) ?    \
          (_ui4ResVirtStart + (addr - _u4FbmPhyStart)) :      \
          __VIRT_DEBUG_CHECK(addr))

