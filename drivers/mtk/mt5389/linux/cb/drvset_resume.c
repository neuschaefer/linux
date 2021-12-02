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
 * $RCSfile: drv_resume.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_resume.c
 *  This file implements for full driver resume.
 */
#include <linux/kernel.h> 
#include <linux/module.h>
#include <linux/opm_device.h>
#include "x_bim.h"
#include "x_assert.h"
#include "eeprom_if.h"

#include "ostg_if.h"
#include "nptv_if.h"
#include "drv_vdoclk.h"
#include "drv_display.h"
#include "drv_video.h"
#include "osd_drvif.h"

#include "drv_video.h"
#include "drv_display.h"
#include "hw_vdoin.h"
#include "gfx_if.h"



#if defined(CC_FAST_INIT)
extern void PDWNC_Reboot(void);
static int opmcls_reboot;
void opmcls_shutdown(struct opm_device *devptr)
{
	DTVCFG_T rDtvCfg;

	printk("opmcls_shutdown:opmcls_reboot 0x%x\n", opmcls_reboot);
	// if(opmcls_reboot) // force reboot when necessary
	{
		if (EEPDTV_GetCfg(&rDtvCfg))
		{
			printk("[opmcls_shutdown]Get configuration fail...\n");
			ASSERT(0);
			return ;
		}
		rDtvCfg.u1Flags3 |= ((UINT8)DTVCFG_FLAG3_FAST_BOOT);  
		rDtvCfg.u1Flags3 &= ~((UINT8)DTVCFG_FLAG3_NORMAL_BOOT_WITH_SUSPEND);               
		rDtvCfg.u1Flags3 &= ~((UINT8)DTVCFG_FLAG3_NORMAL_BOOT_WITHOUT_SUSPEND);
		if (EEPDTV_SetCfg(&rDtvCfg))
		{
			printk("[opmcls_shutdown]Set boot mode fail...\n");
			ASSERT(0);
			return;
		}
		// PDWNC_Reboot();
	}
	/* never retrun to here */
}

static struct opm_driver opmcls_driver_reboot = {
	.shutdown	= opmcls_shutdown,
	.driver		= {
		.name	= "opm_sysclass"
	},
};

static int __devinit _mt53xx_drvset_probe(struct platform_device *pdev)
{
	printk("_mt53xx_drvset_probe\n");
	return 0;
}

static int __devexit _mt53xx_drvset_remove(struct platform_device *pdev)
{
	printk("_mt53xx_drvset_remove\n");
	return 0;	
}

//BSP
extern void BspMainTask(void);

// RM SWI
extern void rm_swi_resume(void);

//SIF
extern void SIF_pm_suspend(void);
extern void SIF_pm_resume(void);

// GPIO
extern void GPIO_pm_suspend(void);
extern void GPIO_pm_resume(void);

// Tuner
extern void TUNER_pm_suspend(void);
extern void TUNER_pm_resume(void);

// Audio
extern void AUD_pm_suspend(void);
extern void AUD_pm_resume(void);

//OSD
void OSD_pm_suspend(void);
void OSD_pm_resume(void);
//PMX
void PMX_pm_suspend(void);
void PMX_pm_resume(void);
//DMX
extern void DMX_pm_resume(void);

//VDEC
void VDEC_pm_suspend(void);
void VDEC_pm_resume(void);
//B2R
void VDP_pm_suspend(void);
void VDP_pm_resume(void);
// PE
void PE_pm_resume(void);
void PE_pm_resume(void) {} // add for mt5396.compile.fail
//TVD
extern void TVD_pm_suspend(void);
extern void TVD_pm_resume(void);
extern void VBI_pm_suspend(void);
extern void VBI_pm_resume(void);
extern void TVE_pm_suspend(void);
extern void TVE_pm_resume(void);
//CEC
void CEC_pm_suspend(void);
void CEC_pm_resume(void);
//MID
void MID_pm_suspend(void);
void MID_pm_resume(void);
void MID_pm_suspend(void) {} // add for mt5396.compile.fail
void MID_pm_resume(void) {} // add for mt5396.compile.fail
// vdoin
void vDrvVideoHwInit_pm_resume(void);

//#define CC_TOI_DRIVER_V_REINIT // call nptv init functions directly
//#define CC_TOI_DRIVER_V_BASIC // enter susupend immediately after insmod dtv_driver_adpt.ko
//#define CC_TOI_DRIVER_V_ALLDRV_FOR_MT5366 // all driver suspend/resume functions for MT5366
#define CC_TOI_DRIVER_V_ALLDRV_FOR_MT5396 // all driver suspend/resume functions for MT5366

#if defined(CC_TOI_DRIVER_V_REINIT)
static void _mt53xx_modules_pm_suspend(void)
{
    OSD_pm_suspend();
    PMX_pm_suspend();
    BIM_DisableIrq(VECTOR_VDOIN);
    GFX_Lock();
    msleep(2000); //make sure mloop stop, sometimes mloop wait for SIF
}
#elif defined(CC_TOI_DRIVER_V_BASIC)
static void _mt53xx_modules_pm_suspend(void)
{
    OSD_pm_suspend();
    PMX_pm_suspend();
    GFX_Lock();
    msleep(2000); //make sure mloop stop, sometimes mloop wait for SIF
}
#elif defined(CC_TOI_DRIVER_V_ALLDRV_FOR_MT5396)
static void MM_pm_suspend(void)
{
    VDEC_pm_suspend();
    VDP_pm_suspend();
}

static void _mt53xx_modules_pm_suspend(void)
{
    CEC_pm_suspend();
    OSD_pm_suspend();
    PMX_pm_suspend();
    TVD_pm_suspend();
    VBI_pm_suspend();
    TVE_pm_suspend();
    MM_pm_suspend();

    TUNER_pm_suspend();
    AUD_pm_suspend();
    GFX_Lock();
    msleep(2000); //make sure mloop stop, sometimes mloop wait for SIF
}
#elif defined(CC_TOI_DRIVER_V_ALLDRV_FOR_MT5366)
static void MM_pm_suspend(void)
{
    VDEC_pm_suspend();
    VDP_pm_suspend();
}

static void _mt53xx_modules_pm_suspend(void)
{
    MID_pm_suspend();
    CEC_pm_suspend();
    OSD_pm_suspend();
    PMX_pm_suspend();
    TVD_pm_suspend();
    VBI_pm_suspend();
    TVE_pm_suspend();
    MM_pm_suspend();
    GPIO_pm_suspend();
    PDWNC_pm_suspend();    

    TUNER_pm_suspend();
    AUD_pm_suspend();
    GFX_Lock();
    msleep(2000); //make sure mloop stop, sometimes mloop wait for SIF
}
#else
#error "Please define CC_TOI_DRIVER_Vx"
#endif

static int _mt53xx_drvset_suspend(struct platform_device *pdev, pm_message_t state)
{
    printk("_mt53xx_drvset_suspend\n");
    opmcls_reboot=1;

    _mt53xx_modules_pm_suspend();

    return 0;
}

#define DRV_PARALLEL_INIT
#define DRV_TUNER_PARALLEL_INIT

#if defined(DRV_PARALLEL_INIT)
static void _ParallelInitThread2(void *pvArgs)
{
    //SelectPanel(14);
    LoadPanelIndex();
    vApiPanelPowerSequence(TRUE);
}
#endif

#if defined(DRV_TUNER_PARALLEL_INIT)
static void _ParallelInitTunerThread(void *pvArgs)
{
    TUNER_pm_resume();
}
#endif

static void MM_pm_resume(void)
{
    VDEC_pm_resume();
    VDP_pm_resume();
}

#if defined(CC_TOI_DRIVER_V_REINIT)
extern BOOL fgYJInit;
BOOL fgYJInit; // add for mt5396.compile.fail
extern BOOL _fgPanelOn;
BOOL _fgPanelOn; // add for mt5396.compile.fail
extern UINT8 bSI_DISPLAY_DCLK_TYPE;
static void _mt53xx_modules_pm_resume(void)
{
    #if defined(DRV_PARALLEL_INIT)
    HANDLE_T hThread2;
    #endif

    rm_swi_resume(); // no need for tux-on-ice, need for software suspend ?
    //BspMainTask();
    //FBM no need
    //DMX,AUD,MPV
    SIF_pm_resume();
    BIM_EnableIrq(VECTOR_SERIAL);
    //Panel
    fgYJInit=FALSE;
    bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;

    vDrvVideoSwInit();

    vApiHwInit();
    DRVCUST_HwInit();

    vApiVideoInit();
    DRVCUST_VideoInit();
    BIM_EnableIrq(VECTOR_VDOIN);
    BIM_EnableIrq(VECTOR_POST_PROC);

    // turn off panel
    // vApiPanelPowerSequence(FALSE);    
    _fgPanelOn = FALSE;
    // SelectPanel(12);
    vDrvOutputStageInit();
    vDrvVOFreqSet(PANEL_GetPixelClk60Hz());
    vDrvSetLCDTiming();
    OSTG_OnOutputVSync();
    OSD_UpdateTiming();
    #if defined(DRV_PARALLEL_INIT)
    x_thread_create(&hThread2, "DrvPanelPowerOnParallelInit",
	    2048,
	    100,
	    _ParallelInitThread2, 0, NULL);
    #else
    vApiPanelPowerSequence(TRUE);
    #endif

    PMX_pm_resume();
    OSD_pm_resume();

    GFX_HwReset();
    //GFX_CmdQueInit();
    GFX_CmdQueSetup();
    GFX_CmdQueReset();
    BIM_EnableIrq(VECTOR_GFX);
    GFX_Unlock();
}
#elif defined(CC_TOI_DRIVER_V_BASIC)
static void _mt53xx_modules_pm_resume(void)
{
    rm_swi_resume(); // no need for tux-on-ice, need for software suspend ?

    SIF_pm_resume();
    BIM_EnableIrq(VECTOR_SERIAL);

    PMX_pm_resume();
    OSD_pm_resume();

    GFX_HwReset();
    //GFX_CmdQueInit();
    GFX_CmdQueSetup();
    GFX_CmdQueReset();
    BIM_EnableIrq(VECTOR_GFX);
    GFX_Unlock();
}
#elif defined(CC_TOI_DRIVER_V_ALLDRV_FOR_MT5396)
static void _mt53xx_modules_pm_resume(void)
{
    #if defined(DRV_PARALLEL_INIT)
    HANDLE_T hThread2;
    #endif
    #if defined(DRV_TUNER_PARALLEL_INIT)
    HANDLE_T TunerThread;
    #endif

    rm_swi_resume(); // no need for tux-on-ice, need for software suspend ?

    SIF_pm_resume();
    BIM_EnableIrq(VECTOR_SERIAL);

    #if defined(DRV_TUNER_PARALLEL_INIT)
    x_thread_create(&TunerThread, "DrvTunerParallelInit",
	    2048,
	    100,
	    _ParallelInitTunerThread, 0, NULL);
    #else
    TUNER_pm_resume();
    #endif

    DMX_pm_resume();

    vApiHwInit();
    DRVCUST_HwInit();
    vApiVideoInit();
    DRVCUST_VideoInit();
    vDrvCvbsVfePD();
    vDrvAllHDADCPow(SV_FALSE);

    ostg_set_fgPanelOn(FALSE);
    vDrvDisplayInit_pm_resume();

    TVD_pm_resume();
    VBI_pm_resume();
    TVE_pm_resume();

    CEC_pm_resume();
    vHDMI_pm_resume();

    PMX_pm_resume();
    OSD_pm_resume();

    MM_pm_resume();

    AUD_pm_resume();

    #if defined(DRV_PARALLEL_INIT)
    x_thread_create(&hThread2, "DrvPanelPowerOnParallelInit",
	    2048,
	    100,
	    _ParallelInitThread2, 0, NULL);
    #else
    vApiPanelPowerSequence(TRUE);
    #endif

    GFX_HwReset();
    //GFX_CmdQueInit();
    GFX_CmdQueSetup();
    GFX_CmdQueReset();
    BIM_EnableIrq(VECTOR_GFX);
    GFX_Unlock();
}
#elif defined(CC_TOI_DRIVER_V_ALLDRV_FOR_MT5366)
extern void _mt5365_u0_direct_write_byte(unsigned int byte);
void _mt5365_u0_direct_write_byte(unsigned int byte) {} // add for mt5396.compile.fail
#define FASTBOOT_LOGB(c) \
    do {\
	_mt5365_u0_direct_write_byte(c);\
	_mt5365_u0_direct_write_byte(0xA);\
	_mt5365_u0_direct_write_byte(0xD);\
    } while (0)
static void _mt53xx_modules_pm_resume(void)
{
    #if defined(DRV_PARALLEL_INIT)
    HANDLE_T hThread2;
    #endif
    #if defined(DRV_TUNER_PARALLEL_INIT)
    HANDLE_T TunerThread;
    #endif

    rm_swi_resume(); // no need for tux-on-ice, need for software suspend ?
    SIF_pm_resume();
    BIM_EnableIrq(VECTOR_SERIAL);
    GPIO_pm_resume();

    #if defined(DRV_TUNER_PARALLEL_INIT)
    x_thread_create(&TunerThread, "DrvTunerParallelInit",
	    2048,
	    100,
	    _ParallelInitTunerThread, 0, NULL);
    #else
    TUNER_pm_resume();
    #endif

    IRRX_pm_resume();
    BIM_EnableIrq(VECTOR_PDWNC);
    CEC_pm_resume();
    DMX_pm_resume();

    ostg_set_fgPanelOn(FALSE);
    vDrvDisplayInit_pm_resume();

    vDrvVideoHwInit_pm_resume();
    TVD_pm_resume();
    VBI_pm_resume();
    TVE_pm_resume();
    BIM_EnableIrq(VECTOR_VDOIN);
    BIM_EnableIrq(VECTOR_POST_PROC);
    // vApiHwInit();

    vHDMI_pm_resume();

    PMX_pm_resume();
    OSD_pm_resume();
    //DI_pm_resume();
    PE_pm_resume();
    BIM_EnableIrq(VECTOR_PSCAN);

    MM_pm_resume();

    AUD_pm_resume();

    #if defined(DRV_PARALLEL_INIT)
    x_thread_create(&hThread2, "DrvPanelPowerOnParallelInit",
	    2048,
	    100,
	    _ParallelInitThread2, 0, NULL);
    #else
    vApiPanelPowerSequence(TRUE);
    #endif

    GFX_HwReset();
    //GFX_CmdQueInit();
    GFX_CmdQueSetup();
    GFX_CmdQueReset();
    BIM_EnableIrq(VECTOR_GFX);
    GFX_Unlock();
    MID_pm_resume();

    BIM_EnableIrq(VECTOR_MISC);
}
#else
#error "Please define CC_TOI_DRIVER_Vx"
#endif

static int _mt53xx_drvset_resume(struct platform_device *pdev)
{
    opmcls_reboot=0;
    printk("_mt53xx_drvset_resume\n");

    _mt53xx_modules_pm_resume();
    return 0;
}


#include <linux/err.h>

static struct opm_device *device;

static struct opm_driver _mt53xx_drvset_driver = {
	.probe		= _mt53xx_drvset_probe,
	.remove		= _mt53xx_drvset_remove,
	.suspend        = _mt53xx_drvset_suspend,
	.resume         = _mt53xx_drvset_resume,
	.driver		= {
		.name	= "mt53xx_drvset",
		.owner	= THIS_MODULE,
	},
};
static struct opm_device *device;

int  mt53xx_drvset_init(void)
{
	int err;

	if ((err = opm_driver_register(&_mt53xx_drvset_driver)) < 0)
		return err;
	if ((err = opm_driver_register(&opmcls_driver_reboot)) < 0)
		return err;
	device = opm_device_register_simple("mt53xx_drvset",0,ORDER_IGNORE);
	if (IS_ERR(device))
		return -ENODEV;						 
	return 0;
}

#endif
