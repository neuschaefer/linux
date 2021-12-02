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
 * $Date  $
 * $RCSfile: mloop_video_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "x_assert.h"
#include "drv_common.h"
#include "x_os.h"
#include "x_bim.h"

#include "panel.h"
#include "source_table.h"

//#include "api_hwinit.h"
#include "vdo_drvif.h"
#include "drv_video.h"
#include "hw_vdoin.h"
#include "drv_dram.h"
#include "drv_adcset.h"
#include "vdp_drvif.h"

/******************************************************************************
* Local variables
******************************************************************************/
static BOOL _bVdoMLoopInit = FALSE;
static BOOL _bVdoMLoopIsrInit = FALSE;
static x_os_isr_fct _pfnMLVIsr      = NULL;  // Mpv might register already
static x_os_isr_fct _pfnOVsyncIsr = NULL;
static HANDLE_T _hMLoopVsyncSemaphore;
HANDLE_T _hMainSubSrcSemaphore; /* for bApiVideoMainSubSrc() */

/******************************************************************************
* External Function
******************************************************************************/
void vApiVideoMainloop(void);
void vSetUSBDump(BOOL);


/******************************************************************************
* Static functions 
******************************************************************************/

/* x_os_drv_reg_isr_ex ()
  * 539x new os low level(osdrv) api to register high priority ISR(HISR), 20080505
  */
extern INT32 x_os_drv_reg_isr_ex(UINT16 ui2_vec_id, 
                x_os_isr_fct   pf_isr,
                x_os_isr_fct   *ppf_old_isr);

static void NptvMainLoop(void *pvArgs);

#define POST_PROC_ISR_WA	0


#if POST_PROC_ISR_WA /* sw workaround for output vsync hw bug */
static HANDLE_T hThread_ReEnableOVsInt = NULL_HANDLE;
HANDLE_T h_enableOVsIntSema = NULL_HANDLE;
HANDLE_T h_enableOVsIntThread = NULL_HANDLE;

static VOID _ReEnableOVsIntFunc(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    BIM_EnableIrq(VECTOR_POST_PROC);  
}

static VOID _enableOVsIntThread(VOID* pv_arg)
{
    while(1)
    {
        x_sema_lock(h_enableOVsIntSema, X_SEMA_OPTION_WAIT);
        VERIFY(x_timer_start(hThread_ReEnableOVsInt, 10, X_TIMER_FLAG_ONCE, _ReEnableOVsIntFunc, NULL) == OSR_OK);
    }
}
#endif

static void _OutputVSyncIsr(UINT16 u2Vector)
{
#if POST_PROC_ISR_WA /* sw workaround for output vsync hw bug */
    x_sema_unlock(h_enableOVsIntSema);
#endif
    if(u2Vector == VECTOR_POST_PROC)
    {
        vVdoOutISR();
    }
    VERIFY(BIM_ClearIrq(VECTOR_POST_PROC));

#if POST_PROC_ISR_WA/* sw workaround for output vsync hw bug */
    BIM_DisableIrq(VECTOR_POST_PROC);
#endif
}

static void _VSyncIsr(UINT16 u2Vector)
{
    if (u2Vector == VECTOR_VDOIN)
    {
        vVdoInISR();

         VERIFY(BIM_ClearIrq(VECTOR_VDOIN));
    }
}

#ifdef CC_SCPOS_SW_ISR
static VOID _VSyncTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    _VSyncIsr(VECTOR_VDOIN);
    _OutputVSyncIsr(VECTOR_POST_PROC);
}
#endif

extern UINT32 SW_DRAM_BASE;
void vAllocateSWReg(void)
{
	// Put SW Reg base address into 0x2700
	SW_DRAM_BASE = (UINT32)x_mem_alloc(0x10000);
	x_memset((void *)SW_DRAM_BASE, 0, 0x10000);
	vRegWrite4B(0x2700, SW_DRAM_BASE);
}

// To initialize thread that contains video main loop
// In the future, we might need to break the loop into several thread
// depend on the function of each statemachine.
BOOL VdoMLInit(void)
{
    INT32     i4Ret;
    HANDLE_T  hThread;


    if(!_bVdoMLoopInit)
    {

		// Allocate SW Register DRAM 
		vAllocateSWReg();
		
        // Create VSync Semphore
        if (x_sema_create(&_hMLoopVsyncSemaphore, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ASSERT(0);
        }

        if (x_sema_create(&_hMainSubSrcSemaphore, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ASSERT(0);
        }
#if POST_PROC_ISR_WA /* sw workaround for output vsync hw bug */
        if (x_sema_create(&h_enableOVsIntSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ASSERT(0);
        }
        VERIFY(x_thread_create(&h_enableOVsIntThread, 
                       "h_enableOVsInt", 
                       2048, 
                       20, 
                       _enableOVsIntThread, 
                       0,
                       NULL)==OSR_OK);
#endif

    
        if (!_bVdoMLoopIsrInit)
        {

#if POST_PROC_ISR_WA /* sw workaround for output vsync hw bug */
            VERIFY(x_timer_create(&hThread_ReEnableOVsInt) == OSR_OK);
#endif

            /* x_os_drv_reg_isr_ex ()
              * 539x new os low level(osdrv) api to register high priority ISR(HISR), 20080505
              */
            if ((x_os_drv_reg_isr_ex(VECTOR_VDOIN, _VSyncIsr, &_pfnMLVIsr) == OSR_OK)
                && (x_os_drv_reg_isr_ex(VECTOR_POST_PROC, _OutputVSyncIsr, & _pfnOVsyncIsr) == OSR_OK))
            {
                _bVdoMLoopIsrInit = TRUE;
            }
            else
            {
                ASSERT(0);
            }
#ifdef CC_SCPOS_SW_ISR
            VERIFY(x_timer_create(&hThread) == OSR_OK);
            VERIFY(x_timer_start(hThread, 100, X_TIMER_FLAG_REPEAT, _VSyncTimeout, NULL) == OSR_OK);
#endif

        }

        if(!_bVdoMLoopIsrInit)
        {
            return FALSE;
        }

        vApiHwInit();
        DRVCUST_HwInit();

        vApiVideoInit();
        DRVCUST_VideoInit();

        //for power saving, power down all ADC
        vDrvCvbsVfePD();
        vDrvAllHDADCPow(SV_FALSE);
                
        // Create Task
        i4Ret = x_thread_create(&hThread,
        "MLVdo_thread",
        MLVDO_STACK_SIZE,
        MLVDO_THREAD_PRIORITY,
        NptvMainLoop,
        0,
        NULL);

        VERIFY(i4Ret == OSR_OK);
        if(i4Ret == OSR_OK)
        {
            _bVdoMLoopInit = TRUE;
        }

#ifndef __KERNEL__
#ifdef LOADER_USB_UPGRADE
		// Create USB Dram Dump Thread		
		vSetUSBDump(SV_FALSE);
#endif
#endif    
    }

    return (_bVdoMLoopInit);
}

void vMLoopUnlockVSyncSem(void)
{
    VERIFY (x_sema_unlock(_hMLoopVsyncSemaphore) == OSR_OK);
}

static void NptvMainLoop(void *pvArgs)
{
    UNUSED(pvArgs);

    while(1)
    {
        VERIFY (x_sema_lock(_hMLoopVsyncSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);
            /* no source change in mloop */
        VERIFY (x_sema_lock(_hMainSubSrcSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);            
        VDP_MutexLock(); 
        vApiVideoMainloop();
        VDP_MutexUnlock();
        VERIFY (x_sema_unlock(_hMainSubSrcSemaphore) == OSR_OK); 
    }
}


