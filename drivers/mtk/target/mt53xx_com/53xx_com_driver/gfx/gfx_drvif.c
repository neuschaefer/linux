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
 * $RCSfile: gfx_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file gfx_drvif.c
 *  internal public interfaces of gfx driver
 *
 */

/*
    lint inhibition(s)
        506 - Constant value Boolean [MTK Rule 6.1.3]
        534 - Ignoring return value of function
        774 - Boolean within 'right side of && within if' always
              evaluates to True [Reference: file gfx_if.c: lines 919, 935]
        826 - Suspicious pointer-to-pointer conversion (area too small)
        831 - Loss of sign (assignment) (signed char to unsigned long)
*/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "gfx_if.h"
#include "gfx_drvif.h"
#include "gfx_dif.h"
#include "gfx_hw.h"
#include "gfx_cmdque.h"
#include "gfx_debug.h"
#include "fbm_drvif.h"
#include "vdp_if.h"
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
#include "drv_display.h"
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

/** gfx default internal buffer size (used in alpha composition)
 *  8 lines, 16bpp per pixel, 1920 pixels per line
 */
#define GFX_DEF_BUF_SIZE    30720

#define GFX_TIME_OUT        (30 * 1000)     // 30 secs

#define GFX_SUB_PIC_COLORS   4              // sub-picture total colors

//#define GOLDEN_COMPARE

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


// acronym of gfx register name in register file
#define REG(XREG)       _prRegFile->rField.XREG

// push back relative register value in register file.
#if defined(GFX_RISC_MODE)  // risc mode
#define GFX_CMDENQ(XREG)        \
    (GFX_RiscPushBack((UINT32)(XREG), _au4RegFile[(UINT32)(XREG)]))
#else                       // cmdque mode
#define GFX_CMDENQ(XREG)        \
    (GFX_CmdQuePushBack((UINT32)(XREG), _au4RegFile[(UINT32)(XREG)]))
#endif // #if defined(GFX_RISC_MODE)


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static const UINT8 _auColorModeShift[] =
{   // byte_per_pixel
    2,      // CM_YCbCr_CLUT2
    1,      // CM_YCbCr_CLUT4
    0,      // CM_YCbCr_CLUT8
    0,      // CM_Reserved0
    1,      // CM_CbYCrY422_DIRECT16
    1,      // CM_YCbYCr422_DIRECT16
    2,      // CM_AYCbCr8888_DIRECT32
    0,      // CM_Reserved1
    2,      // CM_RGB_CLUT2
    1,      // CM_RGB_CLUT4
    0,      // CM_RGB_CLUT8
    1,      // CM_RGB565_DIRECT16
    1,      // CM_ARGB1555_DIRECT16
    1,      // CM_ARGB4444_DIRECT16
    2,      // CM_ARGB8888_DIRECT32
    0       // CM_Reserved2
};


// gfx is in flush (used in middleware).
static INT32 _i4GfxInFlush = (INT32)FALSE;

// output/input address base
static UINT8 *_pu1DstBase;
static UINT8 *_pu1SrcBase;
static UINT8 *_pu1ThirdBase;

// output/input region line size
static UINT32 _u4DstPitch;
static UINT32 _u4SrcPitch;
//static UINT32 _u4ThirdPitch;

// output/input region color mode
static UINT32 _u4DstCM;
static UINT32 _u4SrcCM;
static UINT32 _u4ThirdCM;

static UINT32 _u4DstShift;
static UINT32 _u4SrcShift;
//static UINT32 _u4ThirdShift;

// for "index to direct color bitblt" use
static UINT8 *_pu1Idx2DirSrcBase;
static UINT32 _u4CharCM;
static UINT32 _u4ThirdCharCM;

static UINT32 _au4RegFile[GREG_FILE_SIZE];

static HANDLE_T _semGfxOp = NULL_HANDLE;
static HANDLE_T _semGfxCmdque = NULL_HANDLE;
static HANDLE_T _hGfxCrashDetectTimer = NULL_HANDLE;

static BOOL _fgGfxInited = FALSE;

/*lint -save -e826 */
static MI_DIF_UNION_T *_prRegFile = (MI_DIF_UNION_T *)_au4RegFile;
/*lint -restore */

// internal working buffer for alpha-composition use
static UINT8 _au1WorkingBuf[GFX_DEF_BUF_SIZE];
static UINT8 *_pu1WorkingBuf = _au1WorkingBuf;
static UINT32 _u4WorkingBufSize = GFX_DEF_BUF_SIZE;
static UINT32 _u4WorkingPitch;

// for sub-picture decoding
static UINT8* _pu1BmpBase;
//static UINT32 _u4BmpWidth;
//static UINT32 _u4BmpHeight;

static UINT32 _u4GfxBltSwitch;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    static BOOL _fgGfxEnableBurstRead = FALSE;
#endif

#if defined(GFX_DEBUG_MODE)
    static UINT32 _u4GfxCrashDetection = GFX_DISABLE;  // GFX_ENABLE when IC back
#else
    static UINT32 _u4GfxCrashDetection = GFX_DISABLE;
#endif

#ifndef __KERNEL__
extern UINT32 Image$$RO$$Limit;
#endif

#ifdef CC_FLIP_MIRROR_SUPPORT
BOOL _fgGfxMirror = FALSE;
BOOL _fgGfxFlip = FALSE;
#endif

#if defined(CC_MT5365) || defined(CC_MT5395) 
#ifdef __KERNEL__
static UINT32 _u4DirectFBAddr = 0;
static UINT32 _u4DirectFBSize = 0;
#endif
#endif
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


#ifdef CC_FLIP_MIRROR_SUPPORT
static void _GFX_GetMirrorFlipSetting(void)
{

	UINT8 u1Config;
	u1Config = u1GetFlipMirrorConfig(); 

  if(u1Config & SYS_FLIP_CONFIG_ON)
  {
      _fgGfxFlip = TRUE;
      Printf(" GFX flip en\n");
  }
  if(u1Config & SYS_MIRROR_CONFIG_ON)
  {
      _fgGfxMirror = TRUE;
      Printf(" GFX mirror en\n");                                                                                                                              
   }	
  return;
}
#endif


//-------------------------------------------------------------------------
/** _GfxSetBurstReadMode
 *  Set Gfx Burst Read Mode
 */
//-------------------------------------------------------------------------
#if defined(CC_MT8283_OSD_COMPRESS) && defined(CC_FLIP_MIRROR_SUPPORT)
UINT8 _GFX_ReturnMirrorFlipStatus(void)
{
	UINT8 RetVal=0;

	RetVal= (UINT8)(_fgGfxFlip)|((UINT8)(_fgGfxMirror)<<1);
	//Printf(" -------MirrorFlip: %d---------!!!!\n",RetVal);
	return RetVal;
	
}
#endif

#if defined(CC_MT5360B) || defined(CC_MT5392B)
void _GfxSetBurstReadMode(BOOL fgEnBurstRead)
{
    _fgGfxEnableBurstRead = fgEnBurstRead;
}

BOOL _GfxGetBurstReadMode(VOID)
{
    return _fgGfxEnableBurstRead;
}
#endif
//-------------------------------------------------------------------------
/** _GfxAlphaCompPass
 *  alpha compose pass.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass(UINT32 u4Width, UINT32 u4Height)
{
    REG(fg_OP_MODE)    = (INT32)OP_ALPHA_COMPOSE;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GfxAlphaCompPass0
 *  alpha composite pass 0.
 *  apply ext alpha to color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass0(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Ar)
{
    UNUSED(u4DstX);
    UNUSED(u4DstY);
    
    // pass 0 : apply ar to source (put into working buffer)
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4DstShift));
    REG(fg_DST_BSAD) = (UINT32)(_pu1WorkingBuf);

    REG(fg_ALPHA_VALUE) = u4Ar;
    REG(fg_ALCOM_PASS)  = 0;

    REG(fg_OSD_WIDTH) = (_u4WorkingPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4SrcPitch >> 4);
    
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}


//-------------------------------------------------------------------------
/** _GfxAlphaCompPass1a
 *  alpha composite pass 1a.
 *  apply pixel alpha to color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass1a(void)
{
    // pass 1a : apply source alpha to color components
    REG(fg_SRC_BSAD) = (UINT32)(_pu1WorkingBuf);
    REG(fg_DST_BSAD) = (UINT32)(_pu1WorkingBuf);

    REG(fg_ALPHA_VALUE) = 255;
    REG(fg_ALCOM_PASS)  = 1;

    REG(fg_OSD_WIDTH) = (_u4WorkingPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4WorkingPitch >> 4);
    
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}


//-------------------------------------------------------------------------
/** _GfxAlphaCompPass1b
 *  alpha composite pass 1b.
 *  apply pixel alpha to color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass1b(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY)
{
    UNUSED(u4SrcX);
    UNUSED(u4SrcY);

    // pass 1b : apply destination alpha to color components
    REG(fg_SRC_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_DST_BSAD) = REG(fg_SRC_BSAD);

    REG(fg_ALPHA_VALUE) = 255;
    REG(fg_ALCOM_PASS)  = 1;

    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4DstPitch >> 4);
    
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}


//-------------------------------------------------------------------------
/** _GfxAlphaCompPass2
 *  alpha composite pass 2.
 *
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass2(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Mode)
{
    UNUSED(u4SrcX);
    UNUSED(u4SrcY);

    REG(fg_SRC_BSAD) = (UINT32)(_pu1WorkingBuf);
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_ALPHA_VALUE) = u4Mode;
    REG(fg_ALCOM_PASS)  = 2;

    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4WorkingPitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}


//-------------------------------------------------------------------------
/** _GfxAlphaCompPass3
 *  alpha composite pass 3.
 *  extrace color from composite color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass3(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY)
{
    UNUSED(u4SrcX);
    UNUSED(u4SrcY);

    REG(fg_SRC_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_DST_BSAD) = REG(fg_SRC_BSAD);

    REG(fg_ALPHA_VALUE) = 255;
    REG(fg_ALCOM_PASS)  = 3;

    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4DstPitch >> 4);
    
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _GFX_IsFlushing
 *  check whether engine is flushing
 *  
 */
//-------------------------------------------------------------------------
INT32 _GFX_IsFlushing(void)
{
    return _i4GfxInFlush;
}


//-------------------------------------------------------------------------
/** _GFX_SetFlushStatus
 *  set engine flush status
 *  
 */
//-------------------------------------------------------------------------
void _GFX_SetFlushStatus(INT32 i4Status)
{
    _i4GfxInFlush = i4Status;
}


//-------------------------------------------------------------------------
/** _GFX_SetCrashDetection
 *  enable/disable crash detection
 *  
 */
//-------------------------------------------------------------------------
void _GFX_SetCrashDetection(UINT32 u4Value)
{
    _u4GfxCrashDetection = u4Value;
}


//-------------------------------------------------------------------------
/** _GFX_GetCrashDetection
 *  get crash detection
 *  
 */
//-------------------------------------------------------------------------
UINT32 _GFX_GetCrashDetection(void)
{
    return _u4GfxCrashDetection;
}


//-------------------------------------------------------------------------
/** _GFX_GetCrashDetectTimer
 *  get crash detect timer
 *  
 */
//-------------------------------------------------------------------------
HANDLE_T _GFX_GetCrashDetectTimer(void)
{
    return _hGfxCrashDetectTimer;
}


//-------------------------------------------------------------------------
/** _GFX_ReInit
 *  re-init
 *
 */
//-------------------------------------------------------------------------
void _GFX_ReInit(void)
{
    _fgGfxInited = FALSE;
    _GFX_Init();
}


//-------------------------------------------------------------------------
/** _GFX_Init
 *  init.
 *  reset gfx.
 *  reset gfx timer.
 *  clear soft reg file.
 *  init dif.
 *  init command que.
 */
//-------------------------------------------------------------------------
void _GFX_Init(void)
{
    if (_fgGfxInited)
    {
        return;
    }
    
    if (_semGfxOp == NULL_HANDLE)
    {
        VERIFY(x_sema_create(&_semGfxOp, X_SEMA_TYPE_BINARY, 
            X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if (_semGfxCmdque == NULL_HANDLE)
    {
        VERIFY(x_sema_create(&_semGfxCmdque, X_SEMA_TYPE_BINARY, 
            X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if (_hGfxCrashDetectTimer == NULL_HANDLE)
    {
        VERIFY(x_timer_create(&_hGfxCrashDetectTimer) == OSR_OK);
    }

    GFX_Reset();

    GFX_UNUSED_RET(x_memset(_au4RegFile, 0, sizeof(_au4RegFile)))
    // init dif
    GFX_DifInit();

#if defined(GFX_DEBUG_MODE)

    // init debug info
    GFX_DbgInit();
    
#endif // #if defined(GFX_DEBUG_MODE)

    // init cmdque
    GFX_CmdQueInit();

    GFX_DBG_REC_CUR_STATUS((INT32)E_GFX_IDLE);

    // register GFX_Memset() into FBM module
    FBM_RegCbFunc(FBM_CB_FUNC_GFX_MEMSET, (UINT32)GFX_Memset);

#ifdef CC_FLIP_MIRROR_SUPPORT
    _GFX_GetMirrorFlipSetting();
#endif

    _fgGfxInited = TRUE;
}


//-------------------------------------------------------------------------
/** _GFX_Reset
 *  reset all.
 *  reset dif.
 */
//-------------------------------------------------------------------------
void _GFX_Reset(void)
{
    GFX_DifReset();
}


//-------------------------------------------------------------------------
/** _GFX_Wait
 *  wait engine work complete.
 *  wait dif.
 */
//-------------------------------------------------------------------------

void _GFX_Wait(void)
{
    GFX_LockCmdque();
    
#if defined(CC_MT5365) || defined(CC_MT5395)
    #ifdef __KERNEL__    
     HalFlushInvalidateDCacheMultipleLine(VIRTUAL(_u4DirectFBAddr), _u4DirectFBSize);            
    #else    
    HalFlushInvalidateDCache();
    #endif
#else    
    HalFlushInvalidateDCache();
#endif    

    GFX_UnlockCmdque();
}


//-------------------------------------------------------------------------
/** _GFX_Wait_Vdp
 *  wait Vdp work complete.
 *  wait dif.
 */
//-------------------------------------------------------------------------

void _GFX_Wait_Vdp(void)
{
   #if defined(CC_MT5395)
  	x_thread_delay(VDP_GetCmdDelay());
   #endif
}


//-------------------------------------------------------------------------
/** _GFX_Wait
 *  wait engine work complete.
 *  wait dif.
 */
//-------------------------------------------------------------------------
void _GFX_Wait_Timeout(INT32 i4Timeout)
{
 
    if( OSR_TIMEOUT == x_sema_lock_timeout(_semGfxCmdque, (UINT32)i4Timeout))
    {
        LOG(0,"gfx timeout\n");
    }
#if defined(CC_MT5365) || defined(CC_MT5395)
    #ifdef __KERNEL__    
     HalFlushInvalidateDCacheMultipleLine(VIRTUAL(_u4DirectFBAddr), _u4DirectFBSize);            
    #else    
    HalFlushInvalidateDCache();
    #endif
#else    
    HalFlushInvalidateDCache();
#endif     

    GFX_UNUSED_RET(x_sema_unlock(_semGfxCmdque));

}


//-------------------------------------------------------------------------
/** _GFX_MwFlush
 *  force engine to do operations (for middleware use).
 *  set in flush flag.
 *  call command queue action.
 */
//-------------------------------------------------------------------------
void _GFX_MwFlush(void)
{
    GFX_DBG_INC_MW_FLUSH_COUNT();   // for debug use

    _GFX_SetFlushStatus(TRUE);  // for middleware use

#if defined(GFX_RISC_MODE)  // risc mode
    GFX_HwISR();
#else                       // cmdque mode
    GFX_CmdQueAction();
#endif
}


//-------------------------------------------------------------------------
/** _GFX_Flush
 *  force engine to do operations (for driver use).
 *  set in flush flag.
 *  call command queue action.
 */
//-------------------------------------------------------------------------
void _GFX_Flush(void)
{
#if !defined(GFX_RISC_MODE) // cmdque mode
    GFX_CmdQueAction();
#endif
    //x_memset(_prRegFile,0,sizeof(MI_DIF_UNION_T));
    REG(fg_APP_BPCOMP) = 0;
    REG(fg_THIRD_BUF_EN) = 0;
    REG(fg_ALCOM_DIFF_CM) = 0;
    REG(fg_ALCOM_SRC_KEY_EN) = 0;
    REG(fg_ALCOM_THIRD_KEY_EN) = 0;
    REG(fg_VFULL) = 0;
    REG(fg_DSTPITCH_DEC) = 0;    
    REG(fg_MIRROR) = 0;          
    REG(fg_SRCPITCH_DEC) = (UINT32) 0;
    REG(fg_SRCMIRROR) = (UINT32) 0;    
    REG(fg_PREMULT_SRC_R) = (UINT32) 0;
    REG(fg_PREMULT_DST_R) = (UINT32) 0;
    REG(fg_PREMULT_DST_W) = (UINT32) 0;    
    REG(fg_PREMULT_OVERFLOW_EN) = (UINT32) 0;
    

}


//-------------------------------------------------------------------------
/** _GFX_Lock
 *  take the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_Lock(void)
{
    GFX_UNUSED_RET(x_sema_lock(_semGfxOp, X_SEMA_OPTION_WAIT))
}


//-------------------------------------------------------------------------
/** _GFX_TryLock
 *  try to take the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_TryLock(void)
{
    GFX_UNUSED_RET(x_sema_lock(_semGfxOp, X_SEMA_OPTION_NOWAIT))
}


//-------------------------------------------------------------------------
/** _GFX_Unlock
 *  release the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_Unlock(void)
{
    GFX_UNUSED_RET(x_sema_unlock(_semGfxOp))
}


//-------------------------------------------------------------------------
/** _GFX_LockCmdque
 *  use lock_timeout to do gfx engine crash detection
 *  crash cause 1 : engine eats invalid cmd
 *
 */
//-------------------------------------------------------------------------
#if 0
void _GFX_LockCmdque(void)
{
    if (OSR_TIMEOUT == 
        x_sema_lock_timeout(_semGfxCmdque, GFX_TIME_OUT))
    {
        LOG(0, "GFX engine works too long (30 secs)......\n");

        if (!GFX_HwGetIdle())
        {
            LOG(0, "GFX engine still busy ......\n");

            // recover hw engine
            //GFX_DBG_RECOVER_ENGINE();
            
            GFX_ASSERT(GFX_HwGetIdle());
        }
    }
}
#else
void _GFX_LockCmdque(void)
{
    GFX_UNUSED_RET(x_sema_lock(_semGfxCmdque, X_SEMA_OPTION_WAIT))
}
#endif

//-------------------------------------------------------------------------
/** _GFX_UnlockCmdque
 *  release the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_UnlockCmdque(void)
{
    GFX_UNUSED_RET(x_sema_unlock(_semGfxCmdque))
}


//-------------------------------------------------------------------------
/** _GFX_QueryHwIdle
 *  query engine status
 *
 */
//-------------------------------------------------------------------------
INT32 _GFX_QueryHwIdle(void)
{
    return GFX_HwGetIdle();
}


//-------------------------------------------------------------------------
/** _GFX_SetNotify
 *  set notify function.
 *  set dif notification.
 */
//-------------------------------------------------------------------------
void _GFX_SetNotify(void (*pfnNotifyFunc)(UINT32))
{
    GFX_DifSetNotify(pfnNotifyFunc);
}


//-------------------------------------------------------------------------
/** _GFX_SetDst
 *  set output region.
 *  set output base.
 *  set output color mode.
 *  set output line size.
 *  set output address shift.
 *  enque color mode and line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetDst(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
    #ifndef __KERNEL__
    UINT32 u4HighAddr = (UINT32)&Image$$RO$$Limit; // add memory proction, avoid accessing invaild memory address
    UNUSED(u4HighAddr);
    GFX_ASSERT((UINT32)pu1Base > u4HighAddr);
    #endif
    // check 128 bits (16 bytes) alignment

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if(_fgGfxEnableBurstRead)
        GFX_CHK_64B_PITCH(u4Pitch);
    else
        GFX_CHK_PITCH(u4Pitch);
#else
        GFX_CHK_PITCH(u4Pitch);
#endif

    //GFX_CHK_PITCH(u4Pitch);
    GFX_ASSERT(pu1Base != NULL);

    _pu1DstBase = pu1Base;
    _u4DstCM    = u4ColorMode;
    _u4DstPitch = u4Pitch;
    _u4DstShift = _auColorModeShift[u4ColorMode];

    REG(fg_CM)        = u4ColorMode;
    REG(fg_OSD_WIDTH) = (u4Pitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}


//-------------------------------------------------------------------------
/** _GFX_SetSrc
 *  set input region.
 *  set input base.
 *  set input color mode.
 *  set input line size.
 *  set input address shift.
 *  enque color mode and line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetSrc(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
    #ifndef __KERNEL__
    UINT32 u4HighAddr = (UINT32)&Image$$RO$$Limit; // add memory proction, avoid accessing invaild memory address
    UNUSED(u4HighAddr);
    GFX_ASSERT((UINT32)pu1Base > u4HighAddr);
    #endif


    // check 128 bits (16 bytes) alignment
#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if(_fgGfxEnableBurstRead)
        GFX_CHK_64B_PITCH(u4Pitch);
    else
        GFX_CHK_PITCH(u4Pitch);
#else
        GFX_CHK_PITCH(u4Pitch);
#endif
    
    GFX_ASSERT(pu1Base != NULL);

    _pu1SrcBase = pu1Base;
    _u4SrcCM    = u4ColorMode;
    _u4SrcPitch = u4Pitch;
    _u4SrcShift = _auColorModeShift[u4ColorMode];

    REG(fg_SRC_CM)    = u4ColorMode;
    REG(fg_SRC_PITCH) = (u4Pitch >> 4);
    
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//-------------------------------------------------------------------------
/** _GFX_SetThird
 *  set input region.
 *  set input base.
 *  set input color mode.
 *  set input line size.
 *  set input address shift.
 *  enque color mode and line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetThird(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{

    // check 128 bits (16 bytes) alignment
#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if(_fgGfxEnableBurstRead)
        GFX_CHK_64B_PITCH(u4Pitch);
    else
        GFX_CHK_PITCH(u4Pitch);
#else
        GFX_CHK_PITCH(u4Pitch);
#endif
    
    GFX_ASSERT(pu1Base != NULL);

    _pu1ThirdBase = pu1Base;
    _u4ThirdCM    = u4ColorMode;
    //_u4ThirdPitch = u4Pitch;
    //_u4ThirdShift = _auColorModeShift[u4ColorMode];

    REG(fg_THIRD_CM)    = u4ColorMode;
    REG(fg_THIRD_PITCH) = (u4Pitch >> 4);
    
    //GFX_CMDENQ(GREG_S_OSD_WIDTH);
}


//-------------------------------------------------------------------------
/** _GFX_SetAlpha
 *  set alpha value.
 *  alpha value used in.
 *  alpha blending.
 *  alpha composition.
 */
//-------------------------------------------------------------------------
void _GFX_SetAlpha(UINT32 u4Alpha)
{
    REG(fg_ALPHA_VALUE) = u4Alpha;
    GFX_CMDENQ(GREG_BITBLT_MODE);
}


void _GFX_SetForceAlpha(UINT32 u4Enable)
{
    REG(fg_FORCE_USE_ALPHA) = u4Enable;
    GFX_CMDENQ(GREG_BITBLT_MODE);
}


void _GFX_SetARGB1555(UINT32 u4Enable)
{
    REG(fg_A1555_EN)       = u4Enable;    
}


//-------------------------------------------------------------------------
/** _GFX_SetColor
 *  set color.
 *  color value used in.
 *  rectangle fill.
 *  hline.
 *  vline.
 */
//-------------------------------------------------------------------------
void _GFX_SetColor(UINT32 u4Color)
{
    REG(fg_RECT_COLOR) = u4Color;
    GFX_CMDENQ(GREG_RECT_COLOR);
}


//-------------------------------------------------------------------------
/** _GFX_SetBmpSrc
 *  set bitmap source
 *
 */
//-------------------------------------------------------------------------
void _GFX_SetBmpSrc(UINT8 *pu1Base, UINT32 u4BmpCM)
{
    // no support BMP_CM_8BIT now
    GFX_ASSERT((u4BmpCM == (UINT32)E_BMP_CM_1BIT) ||
               (u4BmpCM == (UINT32)E_BMP_CM_2BIT) ||
               (u4BmpCM == (UINT32)E_BMP_CM_4BIT));

    _pu1BmpBase = pu1Base;
    _u4CharCM   = u4BmpCM;

    REG(fg_CHAR_CM) = u4BmpCM;
}

//-------------------------------------------------------------------------
/** _GFX_SetColorMap
 *  set color mapping
 *
 */
//-------------------------------------------------------------------------
void _GFX_SetColorMap(UINT32 u4BmpCM, UINT32 u4DstCM, 
    const UINT32 *psu4ColorMap)
{
    GFX_ASSERT(psu4ColorMap != NULL);
    
    if ((UINT32)E_BMP_CM_2BIT == u4BmpCM)
    {
        switch (u4DstCM)
        {
        case CM_YCbCr_CLUT8:
        case CM_RGB_CLUT8:
            REG(fg_COLOR_TRANS0) =
                ( psu4ColorMap[0] & 0xff)           |
                ((psu4ColorMap[1] & 0xff) << 8)     |
                ((psu4ColorMap[2] & 0xff) << 16)    |
                ((psu4ColorMap[3] & 0xff) << 24);
            break;

        case CM_RGB565_DIRECT16:
        case CM_ARGB1555_DIRECT16:
        case CM_ARGB4444_DIRECT16:
            REG(fg_COLOR_TRANS0) =
                ( psu4ColorMap[0] & 0xffff)         |
                ((psu4ColorMap[1] & 0xffff) << 16);
            REG(fg_COLOR_TRANS1) =
                ( psu4ColorMap[2] & 0xffff)         |
                ((psu4ColorMap[3] & 0xffff) << 16);
            break;

        case CM_AYCbCr8888_DIRECT32:
        case CM_ARGB8888_DIRECT32:
            REG(fg_COLOR_TRANS0) = psu4ColorMap[0];
            REG(fg_COLOR_TRANS1) = psu4ColorMap[1];
            REG(fg_COLOR_TRANS2) = psu4ColorMap[2];
            REG(fg_COLOR_TRANS3) = psu4ColorMap[3];
            break;

        default:
            return;
        }
    }
    else if ((UINT32)E_BMP_CM_4BIT == u4BmpCM)
    {
        switch (u4DstCM)
        {
        case CM_YCbCr_CLUT8:
        case CM_RGB_CLUT8:
            REG(fg_COLOR_TRANS0) =
                ( psu4ColorMap[0] & 0xff)           |
                ((psu4ColorMap[1] & 0xff) << 8)     |
                ((psu4ColorMap[2] & 0xff) << 16)    |
                ((psu4ColorMap[3] & 0xff) << 24);
            REG(fg_COLOR_TRANS1) =
                ( psu4ColorMap[4] & 0xff)        |
                ((psu4ColorMap[5] & 0xff) << 8)     |
                ((psu4ColorMap[6] & 0xff) << 16)    |
                ((psu4ColorMap[7] & 0xff) << 24);
            REG(fg_COLOR_TRANS2) =
                ( psu4ColorMap[8]  & 0xff)          |
                ((psu4ColorMap[9]  & 0xff) << 8)    |
                ((psu4ColorMap[10] & 0xff) << 16)   |
                ((psu4ColorMap[11] & 0xff) << 24);
            REG(fg_COLOR_TRANS3) =
                ( psu4ColorMap[12] & 0xff)          |
                ((psu4ColorMap[13] & 0xff) << 8)    |
                ((psu4ColorMap[14] & 0xff) << 16)   |
                ((psu4ColorMap[15] & 0xff) << 24);
            break;

        case CM_RGB565_DIRECT16:
        case CM_ARGB1555_DIRECT16:
        case CM_ARGB4444_DIRECT16:
            REG(fg_COLOR_TRANS0) =
                ( psu4ColorMap[0] & 0xffff)         |
                ((psu4ColorMap[1] & 0xffff) << 16);
            REG(fg_COLOR_TRANS1) =
                ( psu4ColorMap[2] & 0xffff)         |
                ((psu4ColorMap[3] & 0xffff) << 16);
            REG(fg_COLOR_TRANS2) =
                ( psu4ColorMap[4] & 0xffff)         |
                ((psu4ColorMap[5] & 0xffff) << 16);
            REG(fg_COLOR_TRANS3) =
                ( psu4ColorMap[6] & 0xffff)         |
                ((psu4ColorMap[7] & 0xffff) << 16);
            REG(fg_COLOR_TRANS4) =
                ( psu4ColorMap[8] & 0xffff)         |
                ((psu4ColorMap[9] & 0xffff) << 16);
            REG(fg_COLOR_TRANS5) =
                ( psu4ColorMap[10] & 0xffff)        |
                ((psu4ColorMap[11] & 0xffff) << 16);
            REG(fg_COLOR_TRANS6) =
                ( psu4ColorMap[12] & 0xffff)        |
                ((psu4ColorMap[13] & 0xffff) << 16);
            REG(fg_COLOR_TRANS7) =
                ( psu4ColorMap[14] & 0xffff)        |
                ((psu4ColorMap[15] & 0xffff) << 16);
            break;

        default:
            return;
        }
    }
    else            // for lint happy
    {
        ;
    }

    GFX_CMDENQ(GREG_COL_TRAN0);
    GFX_CMDENQ(GREG_COL_TRAN1);
    GFX_CMDENQ(GREG_COL_TRAN2);
    GFX_CMDENQ(GREG_COL_TRAN3);
    GFX_CMDENQ(GREG_COL_TRAN4);
    GFX_CMDENQ(GREG_COL_TRAN5);
    GFX_CMDENQ(GREG_COL_TRAN6);
    GFX_CMDENQ(GREG_COL_TRAN7);
}


//-------------------------------------------------------------------------
/** _GFX_SetBmpOpt
 *  set bitmap option.
 */
//-------------------------------------------------------------------------
void _GFX_SetBmpOpt(BOOL fgSubPicDec, BOOL fgRunlenDec)
{
    // sub-picture decode ?
    if (fgSubPicDec == TRUE)
    {
        REG(fg_SPU_RLDEC) = 1;
    }
    else
    {
        REG(fg_SPU_RLDEC) = 0;
    }

    // run length decode ?
    if (fgRunlenDec == TRUE)
    {
        REG(fg_RL_DEC) = 1;
    }
    else
    {
        REG(fg_RL_DEC) = 0;
    }

    GFX_CMDENQ(GREG_SRC_SIZE);
}


//-------------------------------------------------------------------------
/** _GFX_SetSrcColorKey
 *  set src color key.
 */
//-------------------------------------------------------------------------
void _GFX_SetSrcColorKey(BOOL fgSrcKeyEn, UINT32 u4SrcKey)
{
    REG(fg_ALCOM_SRC_KEY_EN) = (UINT32)fgSrcKeyEn;
    REG(fg_ALCOM_SRC_KEY) = u4SrcKey;
    GFX_CMDENQ(GREG_0x40DC);
}

//-------------------------------------------------------------------------
/** _GFX_SetThirdColorKey
 *  set src color key.
 */
//-------------------------------------------------------------------------
void _GFX_SetThirdColorKey(BOOL fgThirdKeyEn, UINT32 u4ThirdKey)
{
    REG(fg_ALCOM_THIRD_KEY_EN) = (UINT32)fgThirdKeyEn;
    REG(fg_ALCOM_THIRD_KEY) = u4ThirdKey;
    GFX_CMDENQ(GREG_0x40E0);
}


//-------------------------------------------------------------------------
/** _GFX_BmpBlt
 *  bitmap/text mapping.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_BmpBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height)
{
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1BmpBase);

    REG(fg_OP_MODE)    = (UINT32)OP_BITMAP_BLT;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_Fill
 *  rectangle fill.
 *  calculate start memory address base + (y * width) + x.
 *  set fill width.
 *  set fill height.
 *  enque output address.
 *  enque input size.
 *  enque gfx mode (include fire control).
 */
//-------------------------------------------------------------------------
void _GFX_Fill(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height)
{
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_OP_MODE)    = (UINT32)OP_RECT_FILL;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

     GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
    
}


//-------------------------------------------------------------------------
/** _GFX_HLine
 *  draw horizontal line.
 *  calculate output address.
 *  set operation mode.
 *  set width.
 *  enque relative registers.
 */
//-------------------------------------------------------------------------
void _GFX_HLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width)
{
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_OP_MODE)   = (UINT32)OP_DRAW_HLINE;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_FIRE)      = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_VLine
 *  draw vertical line.
 *  calculate output address.
 *  set operation mode.
 *  set height.
 *  enque relative registers.
 */
//-------------------------------------------------------------------------
void _GFX_VLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Height)
{
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_OP_MODE)    = (UINT32)OP_DRAW_VLINE;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)

    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_SetBltOpt
 *  set bitblt option.
 *  set transparent enable bit.
 *  set color change enable bit.
 *  set keynot enable bit.
 *  set clip enable bit.
 *  set 4bpp special mode bit.
 *  set color key min.
 *  set color key max.
 *  set color conversion enable to false.
 *  enque color key min and max.
 */
//-------------------------------------------------------------------------
void _GFX_SetBltOpt(UINT32 u4Switch, UINT32 u4ColorMin, UINT32 u4ColorMax)
{
    GFX_ASSERT(!((u4Switch & D_GFXFLAG_TRANSPARENT) && 
                 (u4Switch & D_GFXFLAG_COLORCHANGE)));
    
    REG(fg_TRANS_ENA)    = (u4Switch & D_GFXFLAG_TRANSPARENT) ? 1 : 0;
    REG(fg_KEYNOT_ENA)   = (u4Switch & D_GFXFLAG_KEYNOT)      ? 1 : 0;
    REG(fg_COLCHG_ENA)   = (u4Switch & D_GFXFLAG_COLORCHANGE) ? 1 : 0;
    REG(fg_KEYSDSEL)     = (u4Switch & D_GFXFLAG_KEYSDSEL)    ? 1 : 0;
    REG(fg_DSTPITCH_DEC) = (u4Switch & D_GFXFLAG_FLIP)        ? 1 : 0;
    REG(fg_MIRROR)       = (u4Switch & D_GFXFLAG_MIRROR)      ? 1 : 0;

    if (u4Switch & D_GFXFLAG_OVERLAP)
    {
        REG(fg_DSTPITCH_DEC) = (UINT32)1;
        REG(fg_MIRROR) = (UINT32)1;
        REG(fg_SRCPITCH_DEC) = (UINT32) 1;
        REG(fg_SRCMIRROR) = (UINT32) 1;            
    }

    REG(fg_COLOR_KEY_MIN) = u4ColorMin;
    REG(fg_COLOR_KEY_MAX) = u4ColorMax;
    REG(fg_CFMT_ENA)      = 0;

    GFX_CMDENQ(GREG_KEY_DATA0);
    GFX_CMDENQ(GREG_KEY_DATA1);
    GFX_CMDENQ(GREG_BITBLT_MODE);

    _u4GfxBltSwitch = u4Switch;
}


//-------------------------------------------------------------------------
/** _GFX_BitBlt
 *  bitblt.
 *  calculate output address.
 *  calculate input address.
 *  set bitblt op mode.
 *  set color conversion flag.
 *  enque output address register.
 *  enque input address register.
 *  enque size and clip registers.
 *  enque bitblp option register.
 *  enque mode register.
 */
//-------------------------------------------------------------------------
void _GFX_BitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY, 
    UINT32 u4Width, UINT32 u4Height)
{
    //destination base address
    if ((_u4GfxBltSwitch & D_GFXFLAG_FLIP) && 
        (_u4GfxBltSwitch & D_GFXFLAG_MIRROR))
    {
        REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                            ((u4DstY + u4Height - 1) * _u4DstPitch) + 
                            ((u4DstX + u4Width - 1) << _u4DstShift));
    }
    else if (_u4GfxBltSwitch & D_GFXFLAG_FLIP)
    {
        REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                            ((u4DstY + u4Height - 1) * _u4DstPitch) + 
                            (u4DstX << _u4DstShift));
    }
    else if (_u4GfxBltSwitch & D_GFXFLAG_MIRROR)
    {
        REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                            (u4DstY * _u4DstPitch) + 
                            ((u4DstX + u4Width - 1) << _u4DstShift));
    }
    else if (_u4GfxBltSwitch & D_GFXFLAG_OVERLAP)
    {
        REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                            ((u4DstY + u4Height - 1) * _u4DstPitch) + 
                            ((u4DstX + u4Width - 1) << _u4DstShift));        
    }
    
    else
    {
        REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                            (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    }

    if (_u4GfxBltSwitch & D_GFXFLAG_OVERLAP)
    {
        REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                            ((u4SrcY + u4Height - 1) * _u4SrcPitch) + 
                            ((u4SrcX + u4Width - 1) << _u4SrcShift));            
    }
    else
    {
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));
    }

    REG(fg_OP_MODE)    = (UINT32)OP_BITBLT;
    REG(fg_CFMT_ENA)   = (UINT32)(_u4DstCM != _u4SrcCM);
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    //REG(fg_A1555_EN)       = GFX_ENABLE;    
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
    


    // reset bitblt option
    _GFX_SetBltOpt(0, 0x0, 0xffffffff);
}


//-------------------------------------------------------------------------
/** _GFX_Blend
 *  alpha blending.
 *  calculate input and output address.
 *  set op mode.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_Blend(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY, 
    UINT32 u4Width, UINT32 u4Height)
{
    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_OP_MODE)    = (UINT32)OP_ALPHA_BLEND;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_Compose
 *  alpha composition.
 *  set input gfx region.
 *  set output gfx region.
 *  do pass 0 ~ pass 3.
 */
//-------------------------------------------------------------------------
/*lint -save -e534 -e774 -e831  */
void _GFX_Compose(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY, 
    UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, UINT32 u4Mode)
{
    UINT32 u4MaxLines;
    UINT32 u4Line, u4TotalLines;
    INT32 i4MhpPatch = GFX_ENABLE;

    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    // Add CLEAR, SRC, SRC_OVER patches for MHP project
    // 1. CLEAR    -> use Rect Fill (fill 0x0 in rect_color)
    // 2. SRC      -> use Pass0 and Pass5 of Alpha Composition
    // 3. SRC_OVER -> consider two corner cases
    //          case1: Ar=1.0 & As=1.0
    //          case2: Ar=1.0 & As=0.0
    //          note: Ar=0.0 is the same as case2
    if (((UINT32)CM_ARGB8888_DIRECT32 == _u4SrcCM) &&
        ((UINT32)CM_ARGB8888_DIRECT32 == _u4DstCM) &&
        (GFX_ENABLE == i4MhpPatch))
    {
        if ((UINT32)E_AC_CLEAR == u4Mode)
        {
            UINT32 u4RectColor = REG(fg_RECT_COLOR);
            
            GFX_SetColor(0x0);
            GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);
            
            // restore original value
            GFX_SetColor(u4RectColor);

            return;
        }
        else if ((UINT32)E_AC_SRC == u4Mode)
        {
            if (0 == u4Ar)
            {
                UINT32 u4RectColor = REG(fg_RECT_COLOR);
                
                GFX_SetColor(0x0);
                GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);
                
                // restore original value
                GFX_SetColor(u4RectColor);
            }
            else
            {
                // 1. Pass 0
                GFX_AlphaComposePass(u4SrcX, u4SrcY, u4DstX, u4DstY, 
                    u4Width, u4Height, 0, u4Ar);
                GFX_Flush();     // flush queue
            }
            
            return;
        }
        else if (((UINT32)E_AC_SRC_OVER == u4Mode) &&
                 (0 == u4Ar))
        {
            UINT32 u4RectColor = REG(fg_RECT_COLOR);
            UINT32 u4MinColor  = REG(fg_COLOR_KEY_MIN);
            UINT32 u4MaxColor  = REG(fg_COLOR_KEY_MAX);
            UINT32 u4Switch    = 0x0;

            if (REG(fg_TRANS_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_TRANSPARENT);
            }

            if (REG(fg_KEYNOT_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_KEYNOT);
            }

            if (REG(fg_COLCHG_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_COLORCHANGE);
            }

            // BitBlt with ColorChange (from Dst to Dst)
            GFX_SetColor(0x0);
            GFX_SetBltOpt(D_GFXFLAG_COLORCHANGE, 0x00000000, 0x00FFFFFF);
            
            // set new src & dst pitch
            REG(fg_OSD_WIDTH) = _u4DstPitch >> 4;
            REG(fg_SRC_PITCH) = _u4DstPitch >> 4;

            REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + (u4DstY * _u4DstPitch) + 
                                    (u4DstX << _u4DstShift));
            REG(fg_SRC_BSAD) = (UINT32)(_pu1DstBase + (u4DstY * _u4DstPitch) + 
                                    (u4DstX << _u4DstShift));

            REG(fg_OP_MODE)    = (UINT32)OP_BITBLT;
            REG(fg_CFMT_ENA)   = (UINT32)(_u4DstCM != _u4SrcCM);
            REG(fg_SRC_WIDTH)  = u4Width;
            REG(fg_SRC_HEIGHT) = (u4Height - 1);
            REG(fg_FIRE)       = GFX_ENGINE_FIRE;
            
#if defined(CC_MT5360B) || defined(CC_MT5392B)
            if (_fgGfxEnableBurstRead)
                REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


            GFX_CMDENQ(GREG_S_OSD_WIDTH);
            GFX_CMDENQ(GREG_DST_BSAD);
            GFX_CMDENQ(GREG_SRC_BSAD);
            GFX_CMDENQ(GREG_SRC_SIZE);
            GFX_CMDENQ(GREG_BITBLT_MODE);
            GFX_CMDENQ(GREG_G_MODE);

            // restore original value
            REG(fg_OSD_WIDTH) = _u4DstPitch >> 4;
            REG(fg_SRC_PITCH) = _u4SrcPitch >> 4;
            REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));
            GFX_SetColor(u4RectColor);
            GFX_SetBltOpt(u4Switch, u4MinColor, u4MaxColor);
            GFX_CMDENQ(GREG_S_OSD_WIDTH);
            GFX_CMDENQ(GREG_SRC_BSAD);
            GFX_CMDENQ(GREG_BITBLT_MODE);
            
            return;
        }
        else
        {
            //do nothing, for lint happy
        }
    }
    
    // calculate how many lines can we handle using working buffer
    _u4WorkingPitch = ((u4Width << _u4DstShift) + 0xf) & ~0xf;
    u4MaxLines = (_u4WorkingBufSize / _u4WorkingPitch);

    if (u4Height > u4MaxLines)
    {
        // break down whole area into sets of lines
        u4TotalLines = (u4Height - u4MaxLines);
        for (u4Line = 0; u4Line < u4TotalLines; u4Line += u4MaxLines)
        {
            _GfxAlphaCompPass0(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                    (u4DstY + u4Line), u4Ar);
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass1a();
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass1b(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                    (u4DstY + u4Line));
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass2(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                    (u4DstY + u4Line), u4Mode);
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass3(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                    (u4DstY + u4Line));
            _GfxAlphaCompPass(u4Width, u4MaxLines);
        }
        u4MaxLines = (u4Height - u4Line);
    }
    else
    {
        u4Line = 0;
        u4MaxLines = u4Height;
    }

    // process last few lines
    if (u4MaxLines)
    {
        _GfxAlphaCompPass0(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                (u4DstY + u4Line), u4Ar);
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass1a();
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass1b(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                (u4DstY + u4Line));
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass2(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                (u4DstY + u4Line), u4Mode);
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass3(u4SrcX, (u4SrcY + u4Line), u4DstX, 
                (u4DstY + u4Line));
        _GfxAlphaCompPass(u4Width, u4MaxLines);
    }

    if (((UINT32)CM_ARGB8888_DIRECT32 == _u4SrcCM) &&
        ((UINT32)CM_ARGB8888_DIRECT32 == _u4DstCM) &&
        (GFX_ENABLE == i4MhpPatch))
    {
        if ((u4Mode == (UINT32)E_AC_SRC_OVER) &&
            (255 == u4Ar))
        {
            UINT32 u4MinColor = REG(fg_COLOR_KEY_MIN);
            UINT32 u4MaxColor = REG(fg_COLOR_KEY_MAX);
            UINT32 u4Switch = 0x0;
            
            if (REG(fg_TRANS_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_TRANSPARENT);
            }

            if (REG(fg_KEYNOT_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_KEYNOT);
            }

            if (REG(fg_COLCHG_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_COLORCHANGE);
            }
            
            // restore src & dst pitch
            REG(fg_OSD_WIDTH) = _u4DstPitch >> 4;
            REG(fg_SRC_PITCH) = _u4SrcPitch >> 4;
            GFX_CMDENQ(GREG_S_OSD_WIDTH);

            // do (Transparent & Keynot)-BitBlt
            GFX_SetBltOpt((D_GFXFLAG_TRANSPARENT | D_GFXFLAG_KEYNOT), 0xFF000000, 0xFFFFFFFF);
            GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);

            // restore bltopt
            GFX_SetBltOpt(u4Switch, u4MinColor, u4MaxColor);
            GFX_CMDENQ(GREG_BITBLT_MODE);
        }
    }

    // restore src & dst pitch
    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4SrcPitch >> 4);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}
/*lint -restore */


//-------------------------------------------------------------------------
/** _GFX_AlphaComposePass
 *  alpha compose pass.
 *  exported for pass 4 copy color and pass 5 copy alpha.
 */
//-------------------------------------------------------------------------
void _GFX_AlphaComposePass(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, 
    UINT32 u4Pass, UINT32 u4Param)
{
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4DstShift));

    REG(fg_ALCOM_PASS)  = u4Pass;
    REG(fg_ALPHA_VALUE) = u4Param;
    REG(fg_OSD_WIDTH)   = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH)   = (_u4SrcPitch >> 4);

    REG(fg_OP_MODE)    = (UINT32)OP_ALPHA_COMPOSE;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_SetColCnvFmt
 *  set ycbcr to rgb color conversion format.
 *  set yc format.
 *  set video standard.
 *  set video system.
 *  set mtk swap mode.
 *  enable saturate.
 */
//-------------------------------------------------------------------------
void _GFX_SetColCnvFmt(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd, 
    UINT32 u4VidSys)
{
    REG(fg_YC_FMT)      = u4YCFmt;
    REG(fg_VSTD)        = u4VidStd;
    REG(fg_VSYS)        = u4VidSys;
    REG(fg_SWAP_MODE)   = u4SwapMode;
    REG(fg_VSCLIP)      = GFX_ENABLE;
    REG(fg_ALPHA_VALUE) = 0xFF;

    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_BITBLT_MODE);

}


//-------------------------------------------------------------------------
/** _GFX_SetVFull
*/
//-------------------------------------------------------------------------
void _GFX_SetVFull(UINT32 u4VFull)
{
   REG(fg_VFULL) =  u4VFull;
}


//-------------------------------------------------------------------------
/** _GFX_SetColCnvSrc
 *  set color conversion source.
 *  luma input address, line size.
 *  chroma input address, linesize.
 *  field/frame mode.
 */
//-------------------------------------------------------------------------
void _GFX_SetColCnvSrc(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch, 
    UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic)
{
    REG(fg_SRC_BSAD)      = (UINT32)pu1SrcLuma;
    REG(fg_SRC_PITCH)     = (u4SrcLumaPitch >> 4);
    REG(fg_SRCCBCR_BSAD)  = (UINT32)pu1SrcChroma;
    REG(fg_SRCCBCR_PITCH) = (u4SrcChromaPitch >> 4);
    REG(fg_FLD_PIC)       = u4FieldPic;

    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
}


//-------------------------------------------------------------------------
/** _GFX_ColConv
 *  convert ycbcr to rgb.
 *  calculate output address.
 *  set output width height.
 */
//-------------------------------------------------------------------------
void _GFX_ColConv(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height)
{
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_OP_MODE)    = (UINT32)OP_YCBCR_TO_RGB;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_StretchBlt
 *  stretch bitblt.
 *  calculate output address.
 *  calculate input address.
 *  calculate ration.
 *  set input width height.
 *  set output width height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_StretchBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, UINT32 u4SrcH, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH)
{
    UINT32 u4MnV, u4MnH;

    GFX_ASSERT(_u4DstShift == _u4SrcShift);

    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    // calculate m n ratio for scale up and down
    if ((u4DstW == u4SrcW) || (u4DstW == 1))
    {
        u4MnH = 0x00010000;
        u4SrcW = u4DstW;
    }
    else if (u4SrcW > u4DstW)    // scale down
    {
        u4MnH = (u4SrcW << 16) / (u4DstW - 1);
        if (((u4SrcW << 16) % (u4DstW - 1)) == 0)
        {
            u4MnH--;
        }
    }
    else // (u4DstW > u4SrcW)
    {
        UINT32 u4Step;

        u4Step = (u4SrcW << 16) / (u4DstW);
        if ((u4Step * (u4DstW - 1)) < ((u4SrcW - 1) << 16))
        {
            u4MnH = (u4SrcW << 16) / (u4DstW - 1);
            if (u4MnH == 0x10000)
            {
                u4MnH--;
            }
        }
        else
        {
            u4MnH = u4Step;
        }
    }

    if ((u4DstH == u4SrcH) || (u4DstH == 1))
    {
        u4MnV = 0x00010000;
        u4SrcH = u4DstH;
    }
    else
    {
        u4MnV = (u4SrcH << 16) / (u4DstH - 1);
        if (u4MnV == 0x00010000)
        {
            u4MnV = (u4SrcH << 16) / (u4DstH);
        }
    }

    REG(fg_OP_MODE)        = (UINT32)OP_STRETCH_BITBLT;
    REG(fg_SRC_WIDTH)      = u4SrcW;
    REG(fg_SRC_HEIGHT)     = (u4SrcH - 1);
    REG(fg_STR_DST_WIDTH)  = u4DstW;
    REG(fg_STR_DST_HEIGHT) = (u4DstH - 1);
    REG(fg_STR_BLT_H)      = u4MnH;
    REG(fg_STR_BLT_V)      = u4MnV;
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;    

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_BLT_H);
    GFX_CMDENQ(GREG_STR_BLT_V);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_AlphaMapBitBlt
 *  alpha map bitblt.
 *  calculate output address.
 *  calculate input address.
 *  set alpha map bitblt op mode.
 *  set color conversion flag.
 *  enque output address register.
 *  enque input address register.
 *  enque size registers.
 *  enque mode register.
 */
//-------------------------------------------------------------------------
void _GFX_AlphaMapBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    GFX_ASSERT(_u4SrcCM == (UINT32)CM_RGB_CLUT8);

    GFX_ASSERT((_u4DstCM == (UINT32)CM_ARGB8888_DIRECT32) ||
               (_u4DstCM == (UINT32)CM_ARGB4444_DIRECT16) ||
               (_u4DstCM == (UINT32)CM_ARGB1555_DIRECT16));
    
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_OP_MODE)    = (UINT32)OP_ALPHA_MAP_BITBLT;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_SetRopOpt
 *  set rop option.
 *  rop bitblt.
 */
//-------------------------------------------------------------------------
void _GFX_SetRopOpt(UINT32 u4RopCode)
{
    REG(fg_ROP_OPCODE) = u4RopCode;
    REG(fg_ROP_NO_WR)  = GFX_DISABLE;
    GFX_CMDENQ(GREG_ROP);
}


//-------------------------------------------------------------------------
/** _GFX_RopBitBlt
 *  raster operation.
 *  calculate input and output address.
 *  set rop op code.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_RopBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY, 
    UINT32 u4Width, UINT32 u4Height)
{
    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_OP_MODE)    = (UINT32)OP_ROP_BITBLT;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_ComposeLoop
 *  alpha composition loop mode.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_ComposeLoop(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc)
{   

    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    if (u4Ar == 255)
    {
        REG(fg_ALCOM_NORMAL) = GFX_ENABLE;
    }
    else
    {
        REG(fg_ALCOM_NORMAL) = GFX_DISABLE;    
    }        
    
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_OP_MODE)        = (UINT32)OP_ALPHA_COMPOSE_LOOP;
    REG(fg_SRC_WIDTH)      = u4Width;
    REG(fg_SRC_HEIGHT)     = (u4Height - 1);
    REG(fg_ALCOM_AR)       = u4Ar;
    REG(fg_ALCOM_OPCODE)   = u4OpCode;
    REG(fg_ALCOM_RECT_SRC) = u4RectSrc;
    REG(fg_FIRE)           = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

    
    GFX_CMDENQ(GREG_BITBLT_MODE);
  
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_ALCOM_LOOP);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_SetIdx2DirSrc
 *  set input region.
 *  set input base.
 *  set input color mode.
 *  set input line size.
 *  set input address shift.
 *  enque line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetIdx2DirSrc(UINT8 *pu1Base, UINT32 u4SrcCM)
{
    // Src_pitch can be ignored, since gfx hw reads data continuously.
    GFX_ASSERT((u4SrcCM == (UINT32)E_BMP_CM_1BIT) ||
               (u4SrcCM == (UINT32)E_BMP_CM_2BIT) ||
               (u4SrcCM == (UINT32)E_BMP_CM_4BIT) ||
               (u4SrcCM == (UINT32)E_BMP_CM_8BIT));
    
    _pu1Idx2DirSrcBase = pu1Base;
    _u4CharCM          = u4SrcCM;
}


//-------------------------------------------------------------------------
/** _GFX_SetIdx2DirOpt
 *  set index to direct color bitblt option.
 *  set palette start address in bytes in DRAM (must 16 bytes aligned)
 *  set flag of left pixel index location in one byte of source.
 *  set flag of source line start byte aligned.
 *  enque idx2dir registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetIdx2DirOpt(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft, 
    UINT32 u4StartByteAligned)
{

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if(_fgGfxEnableBurstRead)
        GFX_CHK_64B_PITCH((UINT32)pu1PaletteBase);
    else
        GFX_CHK_PITCH((UINT32)pu1PaletteBase);
#else
        GFX_CHK_PITCH((UINT32)pu1PaletteBase);
#endif

    REG(fg_PAL_BSAD)      = (UINT32)(pu1PaletteBase);
    REG(fg_MSB_LEFT)      = u4MsbLeft;
    REG(fg_LN_ST_BYTE_AL) = u4StartByteAligned;

    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_IDX2DIR);
}


//-------------------------------------------------------------------------
/** _GFX_Idx2DirBitBlt
 *  index to direct color bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_Idx2DirBitBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height)
{
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1Idx2DirSrcBase);

    REG(fg_OP_MODE)    = (UINT32)OP_IDX_TO_DIR_BITBLT;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_CHAR_CM)    = _u4CharCM;         // source color mode
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_2DMemCompare
 *  2D memory compare operation.
 *  calculate input and output address.
 *  set rop op code = XOR.
 *  set NO_WR = 1.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_2DMemCompare(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_OP_MODE)    = (UINT32)OP_ROP_BITBLT;
    REG(fg_ROP_OPCODE) = (UINT32)E_ROP_SRC_XOR_DST;
    REG(fg_ROP_NO_WR)  = GFX_ENABLE;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

    
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_ROP);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_GetMemCompareResult
 *  get memory compare result from register
 *  if no error, return 0
 */
//-------------------------------------------------------------------------
INT32 _GFX_GetMemCompareResult(void)
{
    return GFX_HwGetMemCompareResult();
}


//-------------------------------------------------------------------------
/** _GFX_SetHoriToVertLineOpt
 *  Set DSTPITCH_DEC 0 or 1 to determine whether the bitmap
 *  rotated by 90" clockwise or Counterclockwise.
 */
//-------------------------------------------------------------------------
void _GFX_SetHoriToVertLineOpt(UINT32 u4Is90dCcw)
{
    REG(fg_DSTPITCH_DEC) = u4Is90dCcw;
    GFX_CMDENQ(GREG_BITBLT_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_HoriToVertLine
 *  gfx horizontal line to vertical line operation.
 *  calculate output address.
 *  calculate input address.
 *  enque output address register.
 *  enque input address register.
 *  enque width and height registers.
 *  enque str_width and str_height registers.
 *  enque mode register.
 */
//-------------------------------------------------------------------------
void _GFX_HoriToVertLine(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4HoriLineWidth)
{
    GFX_ASSERT(u4HoriLineWidth != 0);

    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_OP_MODE)        = (UINT32)OP_LINE_HORI_TO_VERT;
    REG(fg_SRC_WIDTH)      = u4HoriLineWidth;
    REG(fg_SRC_HEIGHT)     = 0;
    REG(fg_STR_DST_WIDTH)  = 1;
    REG(fg_STR_DST_HEIGHT) = (u4HoriLineWidth - 1);
    REG(fg_FIRE)           = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

void _GFX_SetCharCM(BOOL flag,UINT32 u4SrcCM)
{
    GFX_ASSERT((u4SrcCM == (UINT32)E_BMP_CM_1BIT) ||
               (u4SrcCM == (UINT32)E_BMP_CM_2BIT) ||
               (u4SrcCM == (UINT32)E_BMP_CM_4BIT) ||
               (u4SrcCM == (UINT32)E_BMP_CM_8BIT));

    if(flag)
    {
        _u4CharCM = u4SrcCM;
        REG(fg_CHAR_CM) = _u4CharCM;
    }
    else
    {
        _u4ThirdCharCM = u4SrcCM;
        REG(fg_THIRD_CHAR_CM) = _u4ThirdCharCM;
    }
    
}
//-------------------------------------------------------------------------
/** _GFX_SetThirdPalle
 *  set index to direct color bitblt option.
 *  set palette start address in bytes in DRAM (must 16 bytes aligned)
 *  set flag of left pixel index location in one byte of source.
 *  set flag of source line start byte aligned.
 *  enque idx2dir registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetThirdPallete(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft, 
    UINT32 u4StartByteAligned)
{

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if(_fgGfxEnableBurstRead)
        GFX_CHK_64B_PITCH((UINT32)pu1PaletteBase);
    else
        GFX_CHK_PITCH((UINT32)pu1PaletteBase);
#else
        GFX_CHK_PITCH((UINT32)pu1PaletteBase);
#endif

    REG(fg_THIRD_PAL_BSAD)      = (UINT32)(pu1PaletteBase);
    REG(fg_THIRD_MSB_LEFT)      = u4MsbLeft;
    REG(fg_THIRD_LN_ST_BYTE_AL) = u4StartByteAligned;

    GFX_CMDENQ(GREG_0x40D4);
}


void _GFX_SetCompressionOpt(UINT32 u4LineSeparate,UINT32 u4QualityMode,UINT32 u4RollBackEn)
{

    REG(fg_ROLL_BACK_EN) = u4RollBackEn;
    REG(fg_QUALITY_MODE) = u4QualityMode;
    REG(fg_LINE_SEPARATE) = u4LineSeparate;

    GFX_CMDENQ(GREG_BPCOMP_CFG);
}

//-------------------------------------------------------------------------
/** _GFX_GeneralAlphaCom
 *  New compression bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_GeneralAlphaCom(UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc,UINT32 u4ThirdEnable,BOOL fgAlcomNormal,BOOL fgOnePassCompress)
{
    UINT32 u4CMDiff;
    if(u4ThirdEnable == FALSE)
    {
        if(_u4SrcCM != _u4DstCM)
            u4CMDiff = 1;
        else
            u4CMDiff = 0;
    }
    else
    {
        if(_u4SrcCM == _u4ThirdCM == _u4DstCM)
            u4CMDiff = 0;
        else
            u4CMDiff = 1;
    }

#ifndef GOLDEN_COMPARE
    if (u4Ar == 255)
    {
        fgAlcomNormal = 1;
    }
    else
    {
        fgAlcomNormal = 0;    
    }
#endif

    REG(fg_APP_BPCOMP) =  fgOnePassCompress;
    
    REG(fg_ALCOM_NORMAL) = fgAlcomNormal;
    REG(fg_THIRD_BUF_EN) = u4ThirdEnable;
    REG(fg_ALCOM_DIFF_CM) = u4CMDiff;

    //destination base address
    //REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
    //                    (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase);
    //source base address
    //REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
    //                    (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase);
    
    REG(fg_THIRD_BSAD) = (UINT32)(_pu1ThirdBase);

    REG(fg_OP_MODE)        = (UINT32)OP_ALPHA_COMPOSE_LOOP;
    REG(fg_SRC_WIDTH)      = u4Width;
    REG(fg_SRC_HEIGHT)     = (u4Height - 1);
    REG(fg_ALCOM_AR)       = u4Ar;
    REG(fg_ALCOM_OPCODE)   = u4OpCode;
    REG(fg_ALCOM_RECT_SRC) = u4RectSrc;
    REG(fg_FIRE)           = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

    GFX_CMDENQ(GREG_0x40D0);
    GFX_CMDENQ(GREG_0x40D4);
    GFX_CMDENQ(GREG_0x40D8);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_ALCOM_LOOP);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_SetPremult
 *  Indicate Src/Dst Premult R/W 
 *  
 */
//-------------------------------------------------------------------------
void _GFX_SetPremult(UINT32 u4PremultSrcR, UINT32 u4PremultDstR, UINT32 u4PremultDstW, UINT32 u4PremultOvrflw)
{
  
    REG(fg_PREMULT_SRC_R) = u4PremultSrcR;
    REG(fg_PREMULT_DST_R) = u4PremultDstR;
    REG(fg_PREMULT_DST_W) = u4PremultDstW;
    REG(fg_PREMULT_OVERFLOW_EN) = u4PremultOvrflw;
 
}


//-------------------------------------------------------------------------
/** _GFX_StretchAlphaCom
 *  New compression bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_StretchAlphaCom(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, 
    UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH, 
    UINT32 u4Ar, UINT32 u4OpCode)
{
    UINT32 u4CMDiff;
    UINT32 u4MnV, u4MnH;
    BOOL fgAlcomNormal;

    GFX_ASSERT(_u4DstShift == _u4SrcShift);
    
    if(_u4SrcCM != _u4DstCM)
    {
        u4CMDiff = 1;
    }
    else
    {
        u4CMDiff = 0;
    }

    // calculate m n ratio for scale up and down
    if ((u4DstW == u4SrcW) || (u4DstW == 1))
    {
        u4MnH = 0x00010000;
        u4SrcW = u4DstW;
    }
    else if (u4SrcW > u4DstW)    // scale down
    {
        u4MnH = (u4SrcW << 16) / (u4DstW - 1);
        if (((u4SrcW << 16) % (u4DstW - 1)) == 0)
        {
            u4MnH--;
        }
    }
    else // (u4DstW > u4SrcW)
    {
        UINT32 u4Step;

        u4Step = (u4SrcW << 16) / (u4DstW);
        if ((u4Step * (u4DstW - 1)) < ((u4SrcW - 1) << 16))
        {
            u4MnH = (u4SrcW << 16) / (u4DstW - 1);
            if (u4MnH == 0x10000)
            {
                u4MnH--;
            }
        }
        else
        {
            u4MnH = u4Step;
        }
    }

    if ((u4DstH == u4SrcH) || (u4DstH == 1))
    {
        u4MnV = 0x00010000;
        u4SrcH = u4DstH;
    }
    else
    {
        u4MnV = (u4SrcH << 16) / (u4DstH - 1);
        if (u4MnV == 0x00010000)
        {
            u4MnV = (u4SrcH << 16) / (u4DstH);
        }
    }

#ifndef GOLDEN_COMPARE
    if (u4Ar == 255)
    {
        fgAlcomNormal = 1;
    }
    else
    {
        fgAlcomNormal = 0;    
    }
#endif    
    
    REG(fg_ALCOM_NORMAL) = fgAlcomNormal;
    REG(fg_ALCOM_DIFF_CM) = u4CMDiff;

    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase);
    //source base address
    REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase + 
                        (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));
    //REG(fg_SRC_BSAD) = (UINT32)(_pu1SrcBase);
    
    REG(fg_OP_MODE)        = (UINT32)OP_STRETCH_ALCOM;
    REG(fg_ALCOM_AR)       = u4Ar;
    REG(fg_ALCOM_OPCODE)   = u4OpCode;
    REG(fg_SRC_WIDTH)      = u4SrcW;
    REG(fg_SRC_HEIGHT)     = (u4SrcH - 1);
    REG(fg_STR_DST_WIDTH)  = u4DstW;
    REG(fg_STR_DST_HEIGHT) = (u4DstH - 1);
    REG(fg_STR_BLT_H)      = u4MnH;
    REG(fg_STR_BLT_V)      = u4MnV;    
    REG(fg_FIRE)           = GFX_ENGINE_FIRE;
   
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_BLT_H);
    GFX_CMDENQ(GREG_STR_BLT_V);    
    GFX_CMDENQ(GREG_STR_DST_SIZE);    
    GFX_CMDENQ(GREG_ALCOM_LOOP);
    GFX_CMDENQ(GREG_G_MODE);
 

}


//-------------------------------------------------------------------------
/** _GFX_NewCompressBlt
 *  New compression bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_NewCompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcCM, 
    UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width, UINT32 u4Height, UINT32 u4BufHeight,
    UINT32 u4RollBackEn, UINT32 u4QualityMode, UINT32 u4BpCompAddrEnd,BOOL fgMirrorFlip,BOOL fgBurstRead,BOOL fgLineSeparate,UINT32 u4DstPitch)
{
    //UINT32 u4DstPitch;

    //GFX_CHK_32B_ALIGN((UINT32)pu1SrcBase);
    //GFX_CHK_32B_ALIGN((UINT32)pu1DstBase);

    GFX_CHK_8B_ALIGN((UINT32)pu1SrcBase);
    GFX_CHK_8B_ALIGN((UINT32)pu1DstBase);

    GFX_ASSERT(u4SrcCM == (UINT32)CM_ARGB8888_DIRECT32 || u4SrcCM == (UINT32)CM_ARGB4444_DIRECT16 ||
    u4SrcCM == (UINT32)CM_AYCbCr8888_DIRECT32 || u4SrcCM == (UINT32)CM_YCbCr_CLUT8 || 
    u4SrcCM == (UINT32)CM_RGB_CLUT8 || u4SrcCM== (UINT32)CM_RGB565_DIRECT16 || u4SrcCM==CM_ARGB1555_DIRECT16);

    GFX_ASSERT((u4Width % 8) == 0);
    //dst_pitch = (src_pitch / 2)
    //u4DstPitch = (u4SrcPitch);

    //mirror/flip flag
#ifdef CC_FLIP_MIRROR_SUPPORT  
       if (_fgGfxFlip && _fgGfxMirror)
	  {
        REG(fg_SRCPITCH_DEC) = (UINT32) 1;
        REG(fg_SRCMIRROR) = (UINT32) 1;
        REG(fg_SRC_BSAD) = (UINT32)(pu1SrcBase + 
                           ((u4SrcY + u4Height - 1) * u4SrcPitch) + 
                           ((u4SrcX + u4Width - 1) << _auColorModeShift[u4SrcCM]));     	  	
				REG(fg_DST_BSAD) = (UINT32)(pu1DstBase + 
        		               ((u4BufHeight - u4SrcY - u4Height) * u4DstPitch) + 
        		               (u4SrcX << _auColorModeShift[u4SrcCM]));                            
	  }
	  else if (_fgGfxMirror)
		{
	      REG(fg_SRCMIRROR) = (UINT32) 1;
        REG(fg_SRC_BSAD) = (UINT32)(pu1SrcBase + 
                           (u4SrcY * u4SrcPitch) + 
                           ((u4SrcX + u4Width - 1) << _auColorModeShift[u4SrcCM]));     	  		      
				REG(fg_DST_BSAD) = (UINT32)(pu1DstBase + 
        	                (u4SrcY * u4DstPitch) + (u4SrcX << _auColorModeShift[u4SrcCM]));                                                          
		}	
		else if (_fgGfxFlip)
		{
        REG(fg_SRCPITCH_DEC) = (UINT32) 1;			
        REG(fg_SRC_BSAD) = (UINT32)(pu1SrcBase + 
                           ((u4SrcY + u4Height - 1) * u4SrcPitch) + 
                           (u4SrcX << _auColorModeShift[u4SrcCM]));     	  	        
				REG(fg_DST_BSAD) = (UINT32)(pu1DstBase + 
        		               ((u4BufHeight - u4SrcY - u4Height) * u4DstPitch) + 
        		               (u4SrcX << _auColorModeShift[u4SrcCM]));                                                          

                           
		}
	  else
#endif 
    if(fgMirrorFlip)
    {
        REG(fg_SRCPITCH_DEC) = (UINT32) 1;
        REG(fg_SRCMIRROR) = (UINT32) 1;
        REG(fg_SRC_BSAD) = (UINT32)(pu1SrcBase + 
                           ((u4SrcY + u4Height - 1) * u4SrcPitch) + 
                           ((u4SrcX + u4Width - 1) << _auColorModeShift[u4SrcCM]));            
				REG(fg_DST_BSAD) = (UINT32)(pu1DstBase + 
        		               ((u4BufHeight - u4SrcY - u4Height) * u4DstPitch) + 
        		               (u4SrcX << _auColorModeShift[u4SrcCM]));                                                                       
    }
    else
    {
        REG(fg_SRCPITCH_DEC) = (UINT32) 0;
        REG(fg_SRCMIRROR) = (UINT32) 0;
			  REG(fg_SRC_BSAD) = (UINT32)(pu1SrcBase + 
                           (u4SrcY * u4SrcPitch) + (u4SrcX << _auColorModeShift[u4SrcCM]));    
    		REG(fg_DST_BSAD) = (UINT32)(pu1DstBase + 
        	                (u4SrcY * u4DstPitch) + (u4SrcX << _auColorModeShift[u4SrcCM]));                               
    }
    
    //source base address
    //REG(fg_SRC_BSAD)  = (UINT32)(pu1SrcBase);
    REG(fg_SRC_CM)    = u4SrcCM;
    REG(fg_SRC_PITCH) = (u4SrcPitch >> 4);

    //destination base address
    //REG(fg_DST_BSAD)  = (UINT32)(pu1DstBase);

    REG(fg_CM)        = u4SrcCM;
    REG(fg_OSD_WIDTH) = (u4DstPitch >> 4);

    REG(fg_OP_MODE)    = (UINT32)OP_NEW_COMPRESS_BITBLT;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height-1);
    //REG(fg_A_METHOD)   = u4AlphaMethod;
    REG(fg_LINE_SEPARATE) = (UINT32)fgLineSeparate;
    REG(fg_ROLL_BACK_EN) = u4RollBackEn;
    REG(fg_QUALITY_MODE) = u4QualityMode;
    REG(fg_BPCOMP_AD_END) =  u4BpCompAddrEnd;

    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif

       
        GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BPCOMP_CFG);
    GFX_CMDENQ(GREG_BPCOMP_AD_END);
    GFX_CMDENQ(GREG_G_MODE);

    REG(fg_APP_BPCOMP) = 0;    
    REG(fg_DSTPITCH_DEC) = 0;        
    REG(fg_SRCPITCH_DEC) = (UINT32) 0;
    REG(fg_SRCMIRROR) = (UINT32) 0;
    GFX_CMDENQ(GREG_BITBLT_MODE);    		
}


//-------------------------------------------------------------------------
/** _GFX_CompressBlt
 *  compression bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_CompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcCM, 
    UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width, UINT32 u4Height,
    UINT32 u4AlphaMethod)
{
    UINT32 u4DstPitch;

    GFX_CHK_32B_ALIGN((UINT32)pu1SrcBase);
    GFX_CHK_32B_ALIGN((UINT32)pu1DstBase);

    GFX_ASSERT(u4SrcCM == (UINT32)CM_ARGB8888_DIRECT32);
    GFX_ASSERT((u4Width % 8) == 0);
               
    // dst_pitch = (src_pitch / 2)
    u4DstPitch = (u4SrcPitch >> 1);

    //source base address
    REG(fg_SRC_BSAD)  = (UINT32)(pu1SrcBase);
    REG(fg_SRC_CM)    = u4SrcCM;
    REG(fg_SRC_PITCH) = (u4SrcPitch >> 4);

    //destination base address
    REG(fg_DST_BSAD)  = (UINT32)(pu1DstBase);
    REG(fg_CM)        = u4SrcCM;
    REG(fg_OSD_WIDTH) = (u4DstPitch >> 4);

    REG(fg_OP_MODE)    = (UINT32)OP_COMPRESS_BITBLT;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_A_METHOD)   = u4AlphaMethod;

    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_COMPRESS);
    GFX_CMDENQ(GREG_G_MODE);
}




// normal driver
#if !defined(GFX_MINI_DRIVER)
//-------------------------------------------------------------------------
/** _GFX_SetGradOpt
 *  gfx set gradient options.
 *  x increment pixels.
 *  y increment pixels.
 *  x delta in color components (a, r, g, b), 7.1 format.
 *  y delta in color components.
 */
//-------------------------------------------------------------------------
void _GFX_SetGradOpt(UINT32 u4IncX, UINT32 u4IncY, 
    const UINT32 asu4DeltaX[4], const UINT32 asu4DeltaY[4])
{
    GFX_ASSERT(asu4DeltaX != NULL);
    GFX_ASSERT(asu4DeltaY != NULL);
        
    REG(fg_GRAD_X_PIX_INC) = u4IncX;
    REG(fg_GRAD_Y_PIX_INC) = u4IncY;

    REG(fg_DELTA_X_C0) = asu4DeltaX[0] & 0xff;
    REG(fg_DELTA_X_C1) = asu4DeltaX[1] & 0xff;
    REG(fg_DELTA_X_C2) = asu4DeltaX[2] & 0xff;
    REG(fg_DELTA_X_C3) = asu4DeltaX[3] & 0xff;

    REG(fg_DELTA_Y_C0) = asu4DeltaY[0] & 0xff;
    REG(fg_DELTA_Y_C1) = asu4DeltaY[1] & 0xff;
    REG(fg_DELTA_Y_C2) = asu4DeltaY[2] & 0xff;
    REG(fg_DELTA_Y_C3) = asu4DeltaY[3] & 0xff;

    GFX_CMDENQ(GREG_GRAD_X_DELTA);
    GFX_CMDENQ(GREG_GRAD_Y_DELTA);
}


//-------------------------------------------------------------------------
/** _GFX_GradFill
 *  gfx gradient rectangle fill.
 *  calculate output address.
 *  set gradient fill op mode.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_GradFill(UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height,
    UINT32 u4Mode)
{
    //destination base address
    REG(fg_DST_BSAD) = (UINT32)(_pu1DstBase + 
                        (u4Y * _u4DstPitch) + (u4X << _u4DstShift));

    REG(fg_OP_MODE)    = (UINT32)OP_GRAD_FILL;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_GRAD_MODE)  = u4Mode;
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_GRAD_XY_INC);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_DMA
 *  gfx dram to dram dma function.
 *  32bit align.
 *  set output address.
 *  set input address.
 *  set number of 32bit words.
 */
//-------------------------------------------------------------------------
void _GFX_DMA(UINT32 *pu4Dst, UINT32 *pu4Src, UINT32 u4NumWrd)
{
    UINT32 u4Width, u4Height;

    GFX_SetBltOpt(D_GFXFLAG_NONE, 0, 0);

    u4Width  = u4NumWrd & 0x7ff;
    u4Height = u4NumWrd >> 11;

    REG(fg_DST_BSAD) = (UINT32)(pu4Dst);
    REG(fg_SRC_BSAD) = (UINT32)(pu4Src);

    REG(fg_OP_MODE)    = (UINT32)OP_DMA;
    REG(fg_CFMT_ENA)   = GFX_DISABLE;
    REG(fg_SRC_WIDTH)  = u4Width;
    REG(fg_SRC_HEIGHT) = u4Height;
    REG(fg_CM)         = (UINT32)CM_ARGB8888_DIRECT32;
    REG(fg_FIRE)       = GFX_ENGINE_FIRE;

#if defined(CC_MT5360B) || defined(CC_MT5392B)
    if (_fgGfxEnableBurstRead)
        REG(fg_BURST_READ) = GFX_ENGINE_BURST_READ;
#endif


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);

    // restore dst color mode
    REG(fg_CM) = _u4DstCM;
}
#endif // #if !defined(GFX_MINI_DRIVER)


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** GFX_MemCompare
 *  do memory compare.
 *  use GFX_2DMemCompare() to complete this function.
 *  (high-level API)
 */
//-------------------------------------------------------------------------
INT32 GFX_MemCompare(UINT8 *pu1DstBase, UINT8 *pu1SrcBase, UINT32 u4Size)
{
    UINT32 u4Pitch = 16384;     // WIDTH[14:0]
    UINT32 i, u4Loops, u4LastSize;
    UINT8 *pu1SrcAddr, *pu1DstAddr;

    GFX_ASSERT(u4Size != 0);

    u4Loops    = u4Size / u4Pitch;
    u4LastSize = u4Size % u4Pitch;

    // lock gfx-op
    GFX_Lock();
    
    for (i = 0; i < u4Loops; i++)
    {
        pu1SrcAddr = (UINT8 *)((UINT32)pu1SrcBase + (i * u4Pitch));
        pu1DstAddr = (UINT8 *)((UINT32)pu1DstBase + (i * u4Pitch));

        GFX_SetSrc(pu1SrcAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst(pu1DstAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_2DMemCompare(0, 0, 0, 0, u4Pitch, 1);
        GFX_Flush();
        GFX_Wait();

        //check compare result
        if (GFX_GetMemCompareResult() == (INT32)E_GFX_INV_ARG)
        {
            GFX_Unlock();
            return -(INT32)E_GFX_INV_ARG;
        }
    } // ~for
    
    if (u4LastSize > 0)
    {
        pu1SrcAddr = (UINT8 *)((UINT32)pu1SrcBase + (i * u4Pitch));
        pu1DstAddr = (UINT8 *)((UINT32)pu1DstBase + (i * u4Pitch));

        GFX_SetSrc(pu1SrcAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst(pu1DstAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_2DMemCompare(0, 0, 0, 0, u4LastSize, 1);
        GFX_Flush();
        GFX_Wait();

        // check compare result
        if (GFX_GetMemCompareResult() == (INT32)E_GFX_INV_ARG)
        {
            GFX_Unlock();
            return -(INT32)E_GFX_INV_ARG;
        }
    }

    // unlock gfx-op
    GFX_Unlock();
    return (INT32)E_GFX_OK;
}


//-------------------------------------------------------------------------
/** GFX_RotateBmp
 *  use "horizontal line to vertical line operation" to 
 *  complete this function.
 *  <assumption : src-pitch = dst-pitch>
 *  (high-level API)
 */
//-------------------------------------------------------------------------
void GFX_RotateBmp(UINT8 *pu1DstBase, UINT32 u4DstPitch, 
    UINT8 *pu1SrcBase, UINT32 u4SrcPitch, UINT32 u4Width, UINT32 u4Height, 
    UINT32 u4ColorMode, UINT32 u4Is90Ccw)
{
    UINT32 i, u4Dx, u4Dy;
    UINT32 u4Sx = 0, u4Sy = 0;

    GFX_ASSERT(pu1DstBase != NULL);
    GFX_ASSERT(pu1SrcBase != NULL);
    GFX_ASSERT(u4Width != 0);
    GFX_ASSERT(u4Height != 0);
    
    // lock gfx-op
    GFX_Lock();

    GFX_SetSrc(pu1SrcBase, u4ColorMode, u4SrcPitch);
    GFX_SetDst(pu1DstBase, u4ColorMode, u4DstPitch);

    GFX_SetHoriToVertLineOpt(u4Is90Ccw);

    if (u4Is90Ccw == (UINT32)TRUE)
    {
        // rotated by 90" counter-clock-wise
        u4Dx = 0;
        u4Dy = (u4Width - 1);

        for(i = 0; i < u4Height; i++)
        {
            GFX_HoriToVertLine(u4Sx, u4Sy++, u4Dx++, u4Dy, u4Width);
        } // ~for
    }
    else
    {
        // rotated by 90" clock-wise
        u4Dx = (u4Height - 1);
        u4Dy = 0;

        for(i = 0; i < u4Height; i++)
        {
            GFX_HoriToVertLine(u4Sx, u4Sy++, u4Dx--, u4Dy, u4Width);
        } // ~for
    }

    // flush and wait
    GFX_Flush();
    GFX_Wait();

    // unlock gfx-op
    GFX_Unlock();
}


//-------------------------------------------------------------------------
/** GFX_Memset
 *  use GFX engine to do memset
 *  (high-level API)
 */
//-------------------------------------------------------------------------
void GFX_Memset(UINT8 *pu1Addr, UINT32 u4Width, UINT32 Height, 
    UINT32 u4Color)
{
    GFX_Lock();
    GFX_SetDst(pu1Addr, (UINT32)CM_RGB_CLUT8, u4Width);
    GFX_SetColor(u4Color);
    GFX_Fill(0, 0, u4Width, Height);
    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();
}


//-------------------------------------------------------------------------
/** GFX_CopyTopfieldToBotfield
 *  copy top-field to bottom field
 *  (high-level API)
 */
//-------------------------------------------------------------------------
void GFX_CopyTopfieldToBotfield(UINT8 *pu1TopStartAddr, 
    UINT32 u4TotalLines, UINT32 u4LineWidth)
{
    UINT8 *pu1BotStartAddr;
    UINT8 *pu1TopCurAddr;
    UINT8 *pu1BotCurAddr;
    UINT32 i, u4Pitch;

    GFX_ASSERT(pu1TopStartAddr != NULL);
    GFX_ASSERT(u4LineWidth != 0);

    u4Pitch = (u4LineWidth + 0xF) & ~(0xF);
    
    // bottom field start address
    pu1BotStartAddr = (UINT8 *)((UINT32)pu1TopStartAddr + u4LineWidth);

    // set top and bottom current address
    pu1TopCurAddr = pu1TopStartAddr;
    pu1BotCurAddr = pu1BotStartAddr;

    // lock gfx-op
    GFX_Lock();

    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);

    for (i = 0; i < u4TotalLines; i++)
    {
        GFX_SetSrc(pu1TopCurAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        
        GFX_SetDst(pu1BotCurAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);

        GFX_BitBlt(0, 0, 0, 0, u4LineWidth, 1);

        pu1TopCurAddr = (UINT8 *)((UINT32)pu1TopCurAddr + (2 * u4LineWidth));
        pu1BotCurAddr = (UINT8 *)((UINT32)pu1BotCurAddr + (2 * u4LineWidth));
        
    } // ~for

    // flush and wait
    GFX_Flush();
    GFX_Wait();

    // unlock gfx-op
    GFX_Unlock();
}


BOOL _GFX_ColorConvert(GFX_COLOR_CONVERT_T* prGfxColorConvert)
{
    UINT32 udx, udy;
    //UINT32 u4YcFmt;

    ASSERT(prGfxColorConvert->pu1SrcAddr != NULL );
    ASSERT(prGfxColorConvert->pu1DstAddr != NULL );
    ASSERT(prGfxColorConvert->pu1CbCr != NULL );
    
    ASSERT(prGfxColorConvert->u4Width >0);
    ASSERT(prGfxColorConvert->u4Height>0);
    
    //ASSERT(prGfxColorConvert->u4DstX >=0);
    //ASSERT(prGfxColorConvert->u4DstY >=0); 
    
    udx = (UINT32)prGfxColorConvert->u4DstX;
    udy = (UINT32)prGfxColorConvert->u4DstY;

    GFX_Lock();

    GFX_SetColCnvFmt(E_YCFMT_420LINEAR, (UINT32)E_SWAP_BLOCK, (UINT32)E_VSTD_BT709, 
        (UINT32)E_VSYS_COMP);

    GFX_SetColCnvSrc(prGfxColorConvert->pu1SrcAddr, prGfxColorConvert->u4Width,
        prGfxColorConvert->pu1CbCr, prGfxColorConvert->u4Width, 0);

    GFX_SetDst(prGfxColorConvert->pu1DstAddr, 
        prGfxColorConvert->u4DstCM, prGfxColorConvert->u4DstPitch);

    GFX_SetAlpha((UINT32)prGfxColorConvert->u1Alpha);

    GFX_ColConv(udx, udy, prGfxColorConvert->u4Width, prGfxColorConvert->u4Height);
    
    GFX_Flush();
    GFX_Wait();

    GFX_Unlock();    
    
    return TRUE;
}


void GFX_SetDirectFBMem(UINT32 u4Addr, UINT32 u4Size)
{
#if defined(CC_MT5365) || defined(CC_MT5395)
	#ifdef __KERNEL__
	_u4DirectFBAddr = u4Addr;
	_u4DirectFBSize = u4Size;
#else 
		return;
	#endif
#else 
	return;
#endif
}

