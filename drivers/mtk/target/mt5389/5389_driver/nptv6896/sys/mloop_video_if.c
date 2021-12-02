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
 * $Author: huiyu.yang $
 * $Date  $
 * $RCSfile: mloop_video_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "hw_vdoin.h"
#include "drv_common.h"
#include "drv_video.h"
#include "drv_dram.h"
#include "drv_adcset.h"
#include "drv_display.h"
#include "vdo_drvif.h"
#include "vdp_drvif.h"
#include "panel.h"
#include "source_table.h"

// =============================================================
// Local variables
// =============================================================
static x_os_isr_fct _pfnMLVIsr      = NULL;  // Mpv might register already
static x_os_isr_fct _pfnOVsyncIsr = NULL;
static HANDLE_T _hMLoopVsyncSemaphore;
HANDLE_T _hMainSubSrcSemaphore; /* for bApiVideoMainSubSrc() */
UINT32 SW_DRAM_BASE = 0;
UINT32 SW_DRAM_BASE_PHY = 0;

// =============================================================
// External Function
// =============================================================

void vApiVideoMainloop(void);
EXTERN void vSetUSBDump(BOOL);
EXTERN void vSWRegInit(void);

// =============================================================
//  Static functions 
// =============================================================

/* x_os_drv_reg_isr_ex ()
  * 539x new os low level(osdrv) api to register high priority ISR(HISR), 20080505
  */
extern INT32 x_os_drv_reg_isr_ex(UINT16 ui2_vec_id, 
                x_os_isr_fct   pf_isr,
                x_os_isr_fct   *ppf_old_isr);

static void NptvMainLoop(void *pvArgs);

// =============================================================
// External Variables
// =============================================================

EXTERN UINT32 SW_DRAM_BASE;

// =============================================================

#define CHECK_3D_INTERRUPT 0
#if CHECK_3D_INTERRUPT
UINT32 u4Cnt=0;		
UINT8  u1Cnt=0;
#endif

static void _OutputVSyncIsr(UINT16 u2Vector)
{
    #ifndef CC_SCPOS_SW_ISR
    UINT32 u4TCONIrqStatus;
	
    if (u2Vector == VECTOR_POST_PROC)
    {
        vDrvGetTCONIrqStatus(&u4TCONIrqStatus);
        if (u4TCONIrqStatus & INTR_OUTPUT_LINE)
        {
            vVdoOutISR();
			#if CHECK_3D_INTERRUPT
            u4Cnt = u4Cnt + 1;
			u1Cnt = u4Cnt % 2;			
			GPIO_SetOut(GPIO(52),u1Cnt6);
			#endif
        }

        if (u4TCONIrqStatus & INTR_TWO_EYE_RISING)
        {
            #if CHECK_3D_INTERRUPT
			GPIO_SetOut(GPIO(52),1);
			#endif
        }
        
        if (u4TCONIrqStatus & INTR_TWO_EYE_FALLING)
        {
            #if CHECK_3D_INTERRUPT
			GPIO_SetOut(GPIO(52),0);      
			#endif
        }

        if (u4TCONIrqStatus & INTR_LEFT_EYE_RISING)
        {
            #if CHECK_3D_INTERRUPT
			GPIO_SetOut(GPIO(52),1);
			#endif
        }

        if (u4TCONIrqStatus & INTR_LEFT_EYE_FALLING)
        {
            #if CHECK_3D_INTERRUPT
			GPIO_SetOut(GPIO(52),0);			
			#endif
        }

        if (u4TCONIrqStatus & INTR_RIGHT_EYE_RISING)
        {
            #if CHECK_3D_INTERRUPT	
			GPIO_SetOut(GPIO(53),1);   
			#endif
        }

        if (u4TCONIrqStatus & INTR_RIGHT_EYE_FALLING)
        {
            #if CHECK_3D_INTERRUPT		
			GPIO_SetOut(GPIO(53),0);                
			#endif
        }

        vDrvSetTCONIrqClear(u4TCONIrqStatus);
    }
    VERIFY(BIM_ClearIrq(VECTOR_POST_PROC));
    #else
    vVdoOutISR();
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

extern VOID* x_mem_alloc_nocache (SIZE_T z_size, UINT32* hwaddr);

// Allocate 64KB DRAM for SW Register
void vAllocateSWReg(void)
{
    static UINT8 fgInit = SV_FALSE;
	
    if (fgInit)
    {
        return;
    }else
    {
    	fgInit = SV_TRUE;
    }

	// for L2 cache issue
	#ifndef __KERNEL__	    
    Printf("=========== Nucleus ==============\n");    
	SW_DRAM_BASE_PHY = BSP_AllocAlignedDmaMemory(0x10000, 32);//(UINT32)x_mem_alloc(0x10000);
	extern void HalFlushDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);
    HalFlushDCacheMultipleLine(SW_DRAM_BASE_PHY, 0x10000);
	SW_DRAM_BASE = (SW_DRAM_BASE_PHY | 0x80000000);    
    #else    
    Printf("===========  Linux  ==============\n");    
    SW_DRAM_BASE = (UINT32)x_mem_alloc_nocache(0x10000, &SW_DRAM_BASE_PHY);
    #endif    

    Printf("Cache    Mem Addr 0x08%x\n", SW_DRAM_BASE_PHY);    
    Printf("NonCache Mem Addr 0x08%x\n", SW_DRAM_BASE);
    
    ASSERT((SW_DRAM_BASE&0x3) == 0);

    if(SW_DRAM_BASE!=0)
    {
    	x_memset((void *)SW_DRAM_BASE, 0, 0x10000);
        // Put SW Reg base address into 0x2700
    	vRegWrite4B(0x2700, SW_DRAM_BASE);
       	vSWRegInit();	
    }
    else
    {
        ASSERT(0);  // No valid SW_DRAM_BASE was allocated
    }
}

// To initialize thread that contains video main loop
// In the future, we might need to break the loop into several thread
// depend on the function of each statemachine.
BOOL VdoMLInit(void)
{
    static BOOL bVdoMLoopInit = FALSE;
    static BOOL bVdoMLoopIsrInit = FALSE;
    INT32     i4Ret;
    HANDLE_T  hThread;

    if (bVdoMLoopInit)
    {
        return bVdoMLoopInit;
    }

    // Allocate SW Register DRAM 
    vAllocateSWReg();
    VDP_XdataInit();
        
    // Create VSync Semaphore
    if (x_sema_create(&_hMLoopVsyncSemaphore, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        ASSERT(0);
    }

    if (x_sema_create(&_hMainSubSrcSemaphore, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        ASSERT(0);
    }

    if (bVdoMLoopIsrInit == FALSE)
    {
        #ifdef CC_SCPOS_SW_ISR
        VERIFY(x_timer_create(&hThread) == OSR_OK);
        VERIFY(x_timer_start(hThread, 100, X_TIMER_FLAG_REPEAT, _VSyncTimeout, NULL) == OSR_OK);
        bVdoMLoopIsrInit = TRUE;
        #else
        // x_os_drv_reg_isr_ex() 539x new os low level(osdrv) api to register high priority ISR(HISR)
        if ((x_os_drv_reg_isr_ex(VECTOR_VDOIN, _VSyncIsr, &_pfnMLVIsr) == OSR_OK) &&
            (x_os_drv_reg_isr_ex(VECTOR_POST_PROC, _OutputVSyncIsr, & _pfnOVsyncIsr) == OSR_OK))
        {
            bVdoMLoopIsrInit = TRUE;
        }
        else
        {
            ASSERT(0);
        }
        #endif
    }

    if (bVdoMLoopIsrInit == FALSE)
    {
        return FALSE;
    }

    vApiHwInit();
    DRVCUST_HwInit();

    vApiVideoInit();
    DRVCUST_VideoInit();

#if defined(CC_SOURCE_AUTO_DETECT) || defined(CC_SRCDETECT_CVBS_SUPPORT)
#else
    vDrvCvbsVfePD();
#endif
    vDrvAllHDADCPow(SV_FALSE);
                
    // Create Task
    i4Ret = x_thread_create(&hThread, "MLVdo_thread", MLVDO_STACK_SIZE, MLVDO_THREAD_PRIORITY, NptvMainLoop, 0, NULL);
    VERIFY(i4Ret == OSR_OK);
    if (i4Ret == OSR_OK)
    {
        bVdoMLoopInit = TRUE;
    }

    #ifndef __KERNEL__
    #ifdef LOADER_USB_UPGRADE
	// Create USB Dram Dump Thread
	vSetUSBDump(SV_FALSE);
    #endif
    #endif

    return bVdoMLoopInit;
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

