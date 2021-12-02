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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: gdma_ctrl.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file gdma_hw.h
 *  Gdma Hardware include file
 */

#ifndef _GDMA_CTRL_H_
#define _GDMA_CTRL_H_


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
LINT_EXT_HEADER_END

#include "x_debug.h"
#include "x_util.h"
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_stl_lib.h"
#include "x_bim.h"
#include "x_rand.h"
#include "x_timer.h"
#include "x_hal_arm.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------
// GDMA Reg Map
//---------------------------------------------------------------------------
//GDMA REG File
#ifndef GDMA_BASE
#define GDMA_BASE	            0x20001000 
#endif

#define REG_GDMA_CONFIG		0x0004
  #define FILL_MODE_MSK   (1<<12)
  #define FILL_MODE_FILL  (1<<12)
  #define FILL_MODE_DMA  (0<<12)
  #define WPROT_EN       (1<<27)
  #define SOFT_RESET   (1<<31)
  #define INTR_MSK_FLG   (1<<0)

#define REG_GDMA_STATUS		0x0008
  #define GDMA_ON   (1<<31)
#define REG_GDMA_FIRE		    0x000c
#define REG_GDMA_DATA_LEN	    0x0010
#define REG_GDMA_SRC_ADDR	    0x0014
#define REG_GDMA_DST_ADDR     0x0018
#define REG_GDMA_FILLVALUE    0x002c
#define _GDMA_READ32(OFFSET)		IO_READ32(GDMA_BASE,OFFSET)
#define _GDMA_WRITE32(OFFSET,VALUE)	IO_WRITE32(GDMA_BASE,OFFSET,VALUE)
#define _GDMA_WRITE32MSK(OFFSET,VALUE,MASK)	IO_WRITE32MSK(GDMA_BASE,OFFSET,VALUE,MASK)


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#ifndef CACHE
#define CACHE(addr)                                   ((addr) & 0x0fffffff)
#endif
#ifndef NONCACHE
#define NONCACHE(addr)                            (((addr) & 0x0fffffff) | 0x30000000)
#endif
#define CHECK_IS_CACHED(addr)               ( (((addr) & 0xf0000000) | 0x30000000)? TRUE:FALSE)



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
EXTERN INT32 _GDMA_Init(VOID);
EXTERN INT32 _GDMA_Stop(VOID);
EXTERN INT32 _GDMA_NonOverlapMemcpy( VOID *dst, const VOID *src, UINT32 n );
EXTERN VOID * _GDMA_MemSet( VOID *s, UINT32 c , UINT32 n);

#endif // _GDMA_CTRL_H_

