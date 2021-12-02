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
 * $RCSfile: sif_hw_v2.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_hw_v2.c
 *  This file implement serial interface SIF function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "sif_debug.h"
#include "sif_hw.h"
#include "sif_if.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "drvcust_if.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_pinmux.h"
#include "x_hal_arm.h"
#include "x_printf.h"
LINT_EXT_HEADER_END

#include "x_pdwnc.h"
#include "pcmcia_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

/// For driver customization options
/// 1. open drain (default use open drain mode)
/// 2. scl stretch (default don't use clock stretch mode)

/// Default use interrupt mode for OSAI mode
#define CC_SIF_USE_INTERRUPT_MODE

/// Use OSAI function option
#if defined (CC_MTK_LOADER)
#define SIF_USE_OSAI            0
#ifdef CC_SIF_USE_INTERRUPT_MODE
#undef CC_SIF_USE_INTERRUPT_MODE
#endif // CC_SIF_USE_INTERRUPT_MODE
#else // defined (CC_MTK_LOADER)
#define SIF_USE_OSAI            1
#endif // defined (CC_MTK_LOADER)

/// Default use multiple data read/write
#define CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

#define SIF_TIMEOUT_MAX  (2000)


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
#ifndef PCMCIA_RawWrite32MSK
#define PCMCIA_RawWrite32MSK(offset, value, msk)  PCMCIA_RawWrite32((offset),((PCMCIA_RawRead32(offset) & ~(msk)) | (value)))
#endif
#endif


//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
extern SIF_STRUCT_T* _SIF_GetActiveChannel(void);
extern SIF_STRUCT_T* _SIF_SetActiveChannel(SIF_STRUCT_T* prStruct);


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
static HANDLE_T _hSifIsrSema      = NULL_HANDLE;
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif

static BOOL _fgSifInit            = FALSE;
static BOOL _fgSifInitStart       = FALSE;
static BOOL _fgSifInitWait        = FALSE;

static BOOL _fgEnablePdwncMaster  = FALSE;
#ifndef CC_MTK_LOADER
static BOOL _fgEnable8295bMaster0 = FALSE;
static BOOL _fgEnable8295bMaster1 = FALSE;
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
static VOID _SIF_CommonISR(UINT16 u2Vector)
{
    if(_SIF_GetActiveChannel()->rSifOps.pfn_ISRHandler != NULL)
    {
        _SIF_GetActiveChannel()->rSifOps.pfn_ISRHandler(u2Vector);
    }
    else
    {
        Printf("Un-hooked SIF ISR!\n");
    }
}
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

INT32 _SIF_ISR_TryLock(void)
{
#if SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
    return x_sema_lock(_hSifIsrSema, X_SEMA_OPTION_NOWAIT);
#else
    return 0;
#endif
}

INT32 _SIF_ISR_Unlock(void)
{
#if SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
    return x_sema_unlock(_hSifIsrSema);
#else
    return 0;
#endif
}

INT32 _SIF_ISR_Lock(void)
{
#if SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
    return x_sema_lock_timeout(_hSifIsrSema, SIF_TIMEOUT_MAX);
#else
    return 0;
#endif
}

void _SIF_ISR_DeleteSema(void)
{
#if SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
        VERIFY(x_sema_delete(_hSifIsrSema) == OSR_OK);
#endif
}

BOOL _SIF_ISR_IsPdwncMasterEnable(void)
{
    return _fgEnablePdwncMaster;
}

#ifndef CC_MTK_LOADER
BOOL _SIF_ISR_Is8295bMaster0Enable(void)
{
    return _fgEnable8295bMaster0;
}

BOOL _SIF_ISR_Is8295bMaster1Enable(void)
{
    return _fgEnable8295bMaster1;
}
#endif

//-------------------------------------------------------------------------
/** SIF_Init
 *  Initialize SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID _SIF_ISR_Init(VOID)
{
    UINT32 u4Reg;
    UINT32 u4Ret, u4Var;
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    x_os_isr_fct pfnOldIsr;
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

    u4Reg = HalCriticalStart();

    if (_fgSifInit)
    {
        HalCriticalEnd(u4Reg);
        return;
    }

    if (!_fgSifInitStart)
    {
        _fgSifInitStart = TRUE;
    }
    else
    {
        _fgSifInitWait = TRUE;
    }
    HalCriticalEnd(u4Reg);

    if (!_fgSifInitWait)
    {

        u4Ret = (UINT32) DRVCUST_InitQuery(eSifEnablePdwncMaster, &u4Var);
        if (u4Ret == 0)
        {
            _fgEnablePdwncMaster = u4Var;
        }
#ifndef CC_MTK_LOADER        
		u4Ret = (UINT32) DRVCUST_InitQuery(eSifEnable8295bMaster0, &u4Var);
        if (u4Ret == 0)
        {
            _fgEnable8295bMaster0 = u4Var;
        }

		u4Ret = (UINT32) DRVCUST_InitQuery(eSifEnable8295bMaster1, &u4Var);
        if (u4Ret == 0)
        {
            _fgEnable8295bMaster1 = u4Var;
        }
#endif

#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
        // Create ISR semaphore
        VERIFY(x_sema_create(&_hSifIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        // Register SIF ISR
        VERIFY(x_reg_isr(VECTOR_SERIAL, _SIF_CommonISR, &pfnOldIsr) == OSR_OK);
        if (_fgEnablePdwncMaster)
        {
            VERIFY(PDWNC_RegIsr(PINTNO_SIF, _SIF_CommonISR, &pfnOldIsr) == PDWNC_OK);
        }        
#ifndef CC_MTK_LOADER
		if ((_fgEnable8295bMaster0)||(_fgEnable8295bMaster1))
		{
			PCMCIA_RegIsr(PCMCIA_ISR_SIF, _SIF_CommonISR, &pfnOldIsr);
		}
#endif
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

        // init local variable
        _fgSifInit = TRUE;

    }
    else
    {
        /* Wait until SIF_Init() finish */
        while (!_fgSifInit)
        {
            static UINT32 u4Count=0;
            x_thread_delay(10);
            u4Count++;
            ASSERT(u4Count < 1000);
        }
    }
}


