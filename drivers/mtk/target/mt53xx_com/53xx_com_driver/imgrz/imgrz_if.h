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
 * $RCSfile: imgrz_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file imgrz_if.h
 *  image resizer driver main interfaces
 *  
 */

#ifndef IMGRZ_IF_H
#define IMGRZ_IF_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


/*lint -save -e961 */
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_clksrc.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
/*lint -restore */


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

/// String size
#define D_RZ_STR_SIZE           50

/// Software mode
#define D_RZ_HAVE_SW_MOD        (1 << (INT32)E_RZ_SW_MOD)

/// Hardware mode
#define D_RZ_HAVE_HW_MOD        (1 << (INT32)E_RZ_HW_MOD)

/// Align 16 bytes
#define D_RZ_ALIGN16(X) (((X) + 0xF) & (~0xF))	///< for scaler pitch alignment
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


/// Error types
typedef enum 
{
    E_RZ_OK = 0,
    E_RZ_INV_ARG,
    E_RZ_OUT_OF_MEM,
    E_RZ_UNINIT,
    E_RZ_UNDEF_ERR
} E_RZ_ERR_CODE_T;

/// OSD input color mode (direct/index)
typedef enum 
{
    E_RZ_OSD_IN_CM_INDEX  = 0,
    E_RZ_OSD_IN_CM_DIRECT = 1
} E_RZ_OSD_IN_CM_T;

/// OSD color mode (index)
typedef enum 
{
    E_RZ_OSD_IDX_CM_2BPP   = 1,
    E_RZ_OSD_IDX_CM_4BPP   = 2,
    E_RZ_OSD_IDX_CM_8BPP   = 3
} E_RZ_OSD_IDX_CM_T;

/// OSD color mode (direct)
typedef enum 
{
    E_RZ_OSD_DIR_CM_RGB565      = 0,
    E_RZ_OSD_DIR_CM_ARGB1555    = 1,
    E_RZ_OSD_DIR_CM_ARGB4444    = 2,
    E_RZ_OSD_DIR_CM_ARGB8888    = 3,
    E_RZ_OSD_DIR_CM_AYCbCr8888  = 3,
    E_RZ_OSD_DIR_CM_YCbYCr      = 4,
    E_RZ_OSD_DIR_CM_CbYCrY      = 5
} E_RZ_OSD_DIR_CM_T;

/// Scaling type for dealing with alpha change
typedef enum 
{
    E_RZ_A_SCL_TYPE_GENERAL          = 0,
    E_RZ_A_SCL_TYPE_REFER_TO_LEFT    = 1,
    E_RZ_A_SCL_TYPE_REFER_TO_NEAREST = 2
} E_RZ_A_SCL_TYPE_T;

/// VDO output mode
typedef enum 
{
    E_RZ_VDO_OUTMD_420 = 0,
    E_RZ_VDO_OUTMD_422 = 1,
    E_RZ_VDO_OUTMD_444 = 2
} E_RZ_VDO_OUTMD_T;

/// Inout type (video/jpeg/osd)
typedef enum 
{
    E_RZ_INOUT_TYPE_VDOMD  = 0,
    E_RZ_INOUT_TYPE_JPGMD  = 1,
    E_RZ_INOUT_TYPE_OSDMD  = 2,
    E_RZ_INOUT_TYPE_OSDUI  = 3,
    E_RZ_INOUT_TYPE_VDOENCODE=4,
    E_RZ_INOUT_TYPE_VDORMRPR=5,
    E_RZ_INOUT_TYPE_VDOENCODE_V2=6    
} E_RZ_INOUT_TYPE_T;

/// Modules
typedef enum 
{
    E_RZ_SW_MOD = 0,
    E_RZ_HW_MOD,
    E_RZ_MODULE_LAST
} E_RZ_MODULE_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  IMGRZ_Init
//
/** Initialize gfx driver.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Init(void);


//---------------------------------------------------------------------------
//  IMGRZ_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Reset(void);


//---------------------------------------------------------------------------
//  IMGRZ_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Wait(void);


//---------------------------------------------------------------------------
//  IMGRZ_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_MwFlush(void);


//---------------------------------------------------------------------------
//  IMGRZ_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Flush(void);


//---------------------------------------------------------------------------
//  IMGRZ_Lock
//
/** Take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Lock(void);


//---------------------------------------------------------------------------
//  IMGRZ_TryLock
//
/** Try to take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_TryLock(void);


//---------------------------------------------------------------------------
//  IMGRZ_Unlock
//
/** Release the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Unlock(void);


//---------------------------------------------------------------------------
//  IMGRZ_LockCmdque
//
/** Take the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_LockCmdque(void);


//---------------------------------------------------------------------------
//  IMGRZ_UnlockCmdque
//
/** Release the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_UnlockCmdque(void);


//---------------------------------------------------------------------------
//  IMGRZ_LockWaitIsr
//
/** Take the imgrz- waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_LockWaitIsr(void);

//---------------------------------------------------------------------------
//  IMGRZ_UnlockWaitIsr
//
/** Release the imgrz-waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_UnlockWaitIsr(void);

//---------------------------------------------------------------------------
//  IMGRZ_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
extern INT32 IMGRZ_QueryHwIdle(void);


//---------------------------------------------------------------------------
//  IMGRZ_SetNotify
//
/** Set notify function (for MW use).
 *
 *  @param  pfnNotifyFunc   The callback function pointer
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_SetNotify(void (*pfnNotifyFunc)(UINT32));


//---------------------------------------------------------------------------
//  IMGRZ_ReInit
//
/** Re initial imgrz.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_ReInit(void);


//---------------------------------------------------------------------------
//  IMGRZ_SetScaleOpt
//
/** Set scale option.
 *
 *  @param  eInOutType   Scale option
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_T eInOutType);


//---------------------------------------------------------------------------
//  IMGRZ_Scale
//
/** Execute scale operation.
 *
 *  @param  pvSclParam  scale parameters
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Scale(void *pvSclParam);


//---------------------------------------------------------------------------
//  IMGRZ_GetScaleOpt
//
/** get scale option.
 *  @retval void
 */
//---------------------------------------------------------------------------
extern E_RZ_INOUT_TYPE_T IMGRZ_GetScaleOpt(void);


//---------------------------------------------------------------------------
//  IMGRZ_SetDigest
//
/** Set a video output frame to a thumbnail result
 *
 *  @param  pvSrc  source parameters
 *  @param  pvDst  distination parameters
 *  @param  fgOnePass FALSE: 2pass (only video scaling), TRUE: 1 pass (scaling and color conv)
 *  @param  u4WorkingBuf  working buffer address for 2 pass
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_SetDigest(void *pvSrc, void *pvDst, BOOL fgOnePass, UINT32 u4WorkingBuf);


//---------------------------------------------------------------------------
//  IMGRZ_Break
//
/** Save imgrz parameters for breaking
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Break(void);

//---------------------------------------------------------------------------
//  IMGRZ_Resume
//
/** Resume the process of imgrz of jpg mcu row mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Resume(void);

//---------------------------------------------------------------------------
//  IMGRZ_OFF
//
/** disable clock of imgrz
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_OFF(void);
extern void IMGRZ_DMA(UINT32 u4SrcAddr,UINT32 u4DstAddr, UINT32 u4Pitch,UINT32 u4Height);

#endif // IMGRZ_IF_H



