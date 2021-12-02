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
 * $RCSfile: mtk_mod.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mtk_mod.c
 *  SDAL module
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "drv_ver.h"
#include "ionodes.h"
#include "cb_low.h"
#include "x_linux.h"
#include "x_serial.h"
#include "x_util.h"
#include "x_timeprofile.h"
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/cb_data.h>
#include "panel.h"
#include "x_timer.h"
#include "drv_display.h"
#include "drv_vdoclk.h"
#include "api_backlt.h"
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
#include <linux/sched.h>
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define INIT_SHOW_VER      1
#define MAX_KEVENTD_PID    50
#define KEVENTD_CMD        "events/0"
#define KEVENTD_NICE_VALUE 20


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

MODULE_LICENSE("Proprietary");


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    int                         i4Fct;
    int                         i4Pid;
    int                         i4Pri;
} SC_CB_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

extern struct file_operations sdal_fops; /* in sdal_if */
extern struct file_operations papi_fops; /* in papi_if */
extern struct file_operations mtal_fops; /* in mtal_if */
extern struct file_operations ddi_fops;/*in ddi_if*/
extern struct file_operations cli_fops; /* in cli_mod */
extern struct file_operations cb_fops; /* in cb_mod */
extern struct file_operations gpio_fops; /* in gpio_mod */
extern struct file_operations jpg_fops; /* in jpg_mod */
extern struct file_operations feeder_fops; /* in feeder_mod */
extern struct file_operations b2r_fops; /* in b2r_mod */

//extern struct file_operations eep_fops;     /* in eep_mod */
extern struct file_operations tsrec_fops; /* in tsrec_mod */
extern struct file_operations fbm_fops;
extern struct file_operations pvr_fops; /* in tsrec_mod */
#ifdef LINUX_TURNKEY_SOLUTION
#if defined(CC_ENABLE_VOMX) || defined(CC_ENABLE_AOMX)
extern struct file_operations kmem_fops; /* in kmem_mod */
#endif
#if defined(CC_ENABLE_AOMX)
extern struct file_operations aomx_fops;
#endif
#if defined(CC_ENABLE_VOMX) 
extern struct file_operations vomx_fops; /* in vdec_mod */
#endif
#endif

extern void BspMainTask(void); /* in bsp */
extern int __init _mt53fb_init(void); /* in mt53_fb */
extern void fbm_blkdev_init(void);
extern void vApiPanelPowerSequence(BOOL fgEnable);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef CC_ARIB_SUPPORT
static unsigned int target_country = COUNTRY_JP;
#else
#ifdef CC_DVBT_SUPPORT
static unsigned int target_country = COUNTRY_EU;
#else
static unsigned int target_country = COUNTRY_US;
#endif /* CC_DVBT_SUPPORT */
#endif /* CC_ARIB_SUPPORT */

static int panel_id = -1;

/* 
   To add new device driver, please allocate unused minor number from Major=241.
   DO NOT ADD ANOTHER MAJOR NUMBER!
   Please see chiling/driver/linux/ioctl/ionodes.h for current usage.
 */
static const DEVLIST_T devlist[] =
{
#ifdef CC_USE_SDAL
    {IO_NODE_MAJOR_SDAL, 0, 1, DEV_SDAL, S_IRUSR | S_IWUSR | S_IRGRP, &sdal_fops},
#endif
#ifdef CC_USE_PAPI
    {IO_NODE_MAJOR_PAPI, 0, 1, DEV_PAPI, S_IRUSR | S_IWUSR | S_IRGRP, &papi_fops},
#endif
#ifdef CC_USE_MTAL
    {IO_NODE_MAJOR_MTAL, IO_NODE_MINOR_MTAL, 1, DEV_MTAL, S_IRUSR | S_IWUSR | S_IRGRP, &mtal_fops},
#endif
#ifdef CC_USE_DDI
    {IO_NODE_MAJOR_DDI, 0, 1, DEV_DDI, S_IRUSR | S_IWUSR | S_IRGRP, &ddi_fops},
#endif
    {IO_NODE_MAJOR_CLI,  IO_NODE_MINOR_CLI,  1, DEV_CLI,  S_IRUSR | S_IWUSR | S_IRGRP, &cli_fops},
    {IO_NODE_MAJOR_CB,   IO_NODE_MINOR_CB,   1, DEV_CB,   S_IRUSR | S_IWUSR | S_IRGRP, &cb_fops},
    {IO_NODE_MAJOR_GPIO, 0, 1, DEV_GPIO, S_IRUSR | S_IWUSR | S_IRGRP, &gpio_fops},
    {IO_NODE_MAJOR_JPG,  0, 1, DEV_JPG,  S_IRUSR | S_IWUSR | S_IRGRP, &jpg_fops},
    {IO_NODE_MAJOR_FEEDER, 0, 1, DEV_FEEDER, S_IRUSR | S_IWUSR | S_IRGRP, &feeder_fops},
    {IO_NODE_MAJOR_TSREC, 0, 1, DEV_TSREC, S_IRUSR | S_IWUSR | S_IRGRP, &tsrec_fops},
    {IO_NODE_MAJOR_PVR, 0, 1, DEV_PVR, S_IRUSR | S_IWUSR | S_IRGRP, &pvr_fops},
    {IO_NODE_MAJOR_B2R, 0, 1, DEV_B2R, S_IRUSR | S_IWUSR | S_IRGRP, &b2r_fops},
#if defined(LINUX_TURNKEY_SOLUTION)
#if defined(CC_ENABLE_VOMX) || defined(CC_ENABLE_AOMX)
    {IO_NODE_MAJOR_KMEM, 0, 1, DEV_KMEM, S_IRUSR | S_IWUSR | S_IRGRP, &kmem_fops},
#endif
#if defined(CC_ENABLE_AOMX)
    {IO_NODE_MAJOR_AOMX, 0, 1, DEV_AOMX, S_IRUSR | S_IWUSR | S_IRGRP, &aomx_fops},
#endif
#if defined(CC_ENABLE_VOMX)
    {IO_NODE_MAJOR_VOMX, 0, 1, DEV_VOMX, S_IRUSR | S_IWUSR | S_IRGRP, &vomx_fops},
#endif    
#endif
    {IO_NODE_MAJOR_FBM, IO_NODE_MINOR_FBM, IO_NODE_NUMBER_FBM, DEV_FBM, S_IRUSR | S_IWUSR | S_IRGRP, &fbm_fops},
//    {IO_NODE_MAJOR_EEP,  0, DEV_EEP,  S_IRUSR | S_IWUSR | S_IRGRP, &eep_fops},
};

static struct cdev cdev_data[ARRAY_SIZE(devlist)];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void __init adjust_prio(void)
{
    int i, len;
    SC_CB_T rSysCallCb;
    struct task_struct *pTask;

    len = strlen(KEVENTD_CMD);

    for (i = 1; i < MAX_KEVENTD_PID; i++)
    {

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
        pTask = find_task_by_vpid(i);
#else
        pTask = find_task_by_pid(i);
#endif
        if (pTask)
        {
            if (strncmp(pTask->comm, KEVENTD_CMD, len) == 0)
            {
                rSysCallCb.i4Fct = 1;
                rSysCallCb.i4Pid = pTask->pid;
                rSysCallCb.i4Pri = KEVENTD_NICE_VALUE;
                _CB_PutEvent(CB_SC_TRIGGER, sizeof(SC_CB_T), &rSysCallCb);
            }
        }
    }
}

static int __init mtk_init(void)
{
	int i, ret;
	dev_t dev;
#ifdef MEASURE_DRV_INIT_TIME
    int id;
    id = TIME_RegisterEntry("mtk_init");
    TIME_StartMeasure(id);
#endif /* MEASURE_DRV_INIT_TIME */
    
#if INIT_SHOW_VER
    #define MAX_VERSION_STRING_LEN 32
    CHAR szVer1[MAX_VERSION_STRING_LEN];
    CHAR szVer2[MAX_VERSION_STRING_LEN];

    if((DRV_GetVersionString1(szVer1, MAX_VERSION_STRING_LEN) == DRVR_OK) &&
       (DRV_GetVersionString2(szVer2, MAX_VERSION_STRING_LEN) == DRVR_OK))
    {
        printk("\nMediaTek MT53XX Driver : %s (%s)\n", szVer1, szVer2);
    }
#endif //INIT_SHOW_VER

    if (target_country == COUNTRY_EU)
    {
        SetTargetCountry(COUNTRY_EU);
        printk("Set target country to EU\n");
    }
    else if (target_country == COUNTRY_JP)
    {
        SetTargetCountry(COUNTRY_JP);
        printk("Set target country to JP\n");
    }
    else
    {
        SetTargetCountry(COUNTRY_US);
        printk("Set target country to US\n");
    }

    _CB_Init();

    // System initialize
    BspMainTask();

    // Map FBM, this only needed on IC older or equal then MT5395/MT5365
    i = (int) VIRTUAL(0);

    if (panel_id != -1)
    {
        SelectPanel(panel_id);
        
        vDrvOutputStageInit();
        vDrvVOFreqSet(PANEL_GetPixelClk60Hz());
        vDrvSetLCDTiming();
        OSTG_OnOutputVSync();
        vApiPanelPowerSequence(TRUE);
    }

#ifdef MT53XX_I2C_MODULE
    mtk_i2c_init();
#endif //MT53XX_I2C_MODULE

//#ifdef MT53XX_FB_MODULE
    _mt53fb_init();
//#endif //MT53XX_FB_MODULE

    adjust_prio();

    // Register nodes and create devices
    for (i = 0; i < ARRAY_SIZE(devlist); i++)
    {
        dev = MKDEV(devlist[i].major, devlist[i].minor);
        ret = register_chrdev_region(dev, devlist[i].numbers, devlist[i].name);
        if (ret)
            goto error;

        cdev_init(&cdev_data[i], devlist[i].fops);
        kobject_set_name(&cdev_data[i].kobj, devlist[i].name, devlist[i].numbers);
        ret = cdev_add(&cdev_data[i], dev, devlist[i].numbers);
        if (ret)
        {
        error:
            printk("unable to get device id %d:%d for %s devs\n", devlist[i].major, devlist[i].minor,
                   devlist[i].name);
        }
    }

    // Register FBM blk devices.
    fbm_blkdev_init();
        
#ifdef MEASURE_DRV_INIT_TIME
    TIME_StopMeasure(id);
#endif /* MEASURE_DRV_INIT_TIME */
    
    return 0;
}

fs_initcall(mtk_init);

#ifdef CC_DVBT_SUPPORT
module_param(target_country, int, 0444);
#else
module_param(target_country, int, 0444);
#endif /* CC_DVBT_SUPPORT */
module_param(panel_id, int, 0444);


