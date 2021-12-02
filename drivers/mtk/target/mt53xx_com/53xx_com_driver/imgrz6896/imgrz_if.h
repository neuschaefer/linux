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
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
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
//#include "x_bim.h"
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

#define E_FIRST_RESIZER     0
#if (defined(CC_MT5396))
    #define IMGRZ_NUM   2
    #define E_SECND_RESIZER     1
#else
    #define E_SECND_RESIZER     0
    #define IMGRZ_NUM   1
#endif

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
    E_RZ_OSD_DIR_CM_CbYCrY      = 5,
    E_RZ_OSD_DIR_CM_RGBA8888  = 6
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
    E_RZ_INOUT_TYPE_VDOENCODE_V2=6, 
    E_RZ_INOUT_TYPE_OSD_CSC =7,    
} E_RZ_INOUT_TYPE_T;

/// Modules
typedef enum 
{
    E_RZ_SW_MOD = 0,
    E_RZ_HW_MOD,
    E_RZ_MODULE_LAST
} E_RZ_MODULE_T;

typedef enum 
{
	E_RZ_OSD_CSC_FORMAT_ARGB8888=0, 
	E_RZ_OSD_CSC_FORMAT_AYUV8888=1 		
} E_RZ_CSC_FORMAT_T;

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
extern void IMGRZ_Init_Ex(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Reset(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Wait(void);
extern void IMGRZ_Wait_Ex(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_MwFlush(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Flush(void);
extern void IMGRZ_Flush_Ex(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_Lock
//
/** Take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Lock(void);
extern void IMGRZ_Lock_Ex(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_TryLock
//
/** Try to take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_TryLock(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_Unlock
//
/** Release the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Unlock(void);
extern void IMGRZ_Unlock_Ex(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_LockCmdque
//
/** Take the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_LockCmdque(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_UnlockCmdque
//
/** Release the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_UnlockCmdque(void);
extern void IMGRZ_UnlockCmdque_Ex(UINT32 u4Which);


//---------------------------------------------------------------------------
//  IMGRZ_LockWaitIsr
//
/** Take the imgrz- waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_LockWaitIsr(UINT32 u4Which);

//---------------------------------------------------------------------------
//  IMGRZ_UnlockWaitIsr
//
/** Release the imgrz-waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_UnlockWaitIsr(void);
extern void IMGRZ_UnlockWaitIsr_Ex(UINT32 u4Which);

//---------------------------------------------------------------------------
//  IMGRZ_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
extern INT32 IMGRZ_QueryHwIdle(UINT32 u4Which);


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
extern void IMGRZ_SetNotify(UINT32 u4Which, void (*pfnNotifyFunc)(UINT32));


//---------------------------------------------------------------------------
//  IMGRZ_ReInit
//
/** Re initial imgrz.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_ReInit(void);
extern void IMGRZ_ReInit_Ex(UINT32 u4Which);


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
extern void IMGRZ_SetScaleOpt_Ex(UINT32 u4Which, E_RZ_INOUT_TYPE_T eInOutType);


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
extern void IMGRZ_Scale_Ex(UINT32 u4Which, void *pvSclParam);


//---------------------------------------------------------------------------
//  IMGRZ_GetScaleOpt
//
/** get scale option.
 *  @retval void
 */
//---------------------------------------------------------------------------
extern E_RZ_INOUT_TYPE_T IMGRZ_GetScaleOpt(void);
extern E_RZ_INOUT_TYPE_T IMGRZ_GetScaleOpt_Ex(UINT32 u4Which);


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
extern void IMGRZ_Break(UINT32 u4Which);

//---------------------------------------------------------------------------
//  IMGRZ_Resume
//
/** Resume the process of imgrz of jpg mcu row mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_Resume(UINT32 u4Which);

//---------------------------------------------------------------------------
//  IMGRZ_OFF
//
/** disable clock of imgrz
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_OFF(UINT32 u4Which);

//---------------------------------------------------------------------------
//  IMGRZ_RegInfo
//
/** get imgrz register address and size for 
 *  register dump
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void IMGRZ_RegInfo(UINT32 u4Which, UINT32 *pu4Addr,UINT32 *pu4Size);

extern BOOL IMGRZ_CkgenOnOff(UINT32 u4Id, BOOL fgEnable);

extern void IMGRZ_AYUV_ARGB_Conv(UINT32  u4TgBase,UINT32 u4BufLen,UINT32 u4Width, UINT32 u4Height,
    UINT32 u4OffsetX,  UINT32 u4OffsetY, BOOL fgAYUV2ARGB);
extern void IMGRZ_DMA(UINT32 u4SrcAddr,UINT32 u4DstAddr, UINT32 u4Pitch,UINT32 u4Height);
#endif // IMGRZ_IF_H



