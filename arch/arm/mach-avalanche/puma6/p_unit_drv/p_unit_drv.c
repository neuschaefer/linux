/*
 *
 * p_unit_drv.c
 * Description:
 * power control unit device driver
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2012-2015 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */

/*------------------------------------------------------------------------------
 * File Name: p_unit_drv.c
 *------------------------------------------------------------------------------
 */
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/version.h>
#include <linux/module.h>   /* for modules */
#include <linux/fs.h>       /* file_operations */
#include <linux/uaccess.h>  /* copy_(to,from)_user */
#include <linux/init.h>     /* module_init, module_exit */
#include <linux/cdev.h>     /* cdev utilities */
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/semaphore.h>
#include <linux/ioctl.h>
#include <linux/proc_fs_macros.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <mach/puma.h>

#if PUMA6_SOC_TYPE /* For Puma-6 SoC */
#include "puma6_bootcfg_ctrl.h"
#elif PUMA7_OR_NEWER_SOC_TYPE /* For Puma-7 SoC */
#include "puma7_bootcfg_ctrl.h"
#include "puma7_pp.h"
#endif

#include "iosfsb_api.h"
#include "p_unit_api.h"
#include <linux/reboot.h>
#include <linux/kthread.h>
#include "arm_atom_mbx.h"
#include "sram_api.h"
#include "hw_mutex_ctrl.h"

#define DEV_NAME  "p_unit"
//#define P_UNIT_DEBUG
#ifdef  P_UNIT_DEBUG
/* note: prints function name for you */
#  define DPRINTK(fmt, args...) printk("%-40s:%5d " fmt, __FUNCTION__,__LINE__, ## args)
#else
#  define DPRINTK(fmt, args...)
#endif

typedef struct
{
    unsigned int numOfAppCpuResetToCstate;
    unsigned int numOfAppCpuOnlyReset;
    unsigned int numOfAppCpuNagraBbuExitReset;
    unsigned int maxTimeInMsecTakeLockEmmcMutex;
    unsigned int maxTimeInMsecEmmcBlocked;
}P_Unit_Statistics_t;

typedef union
{
    struct
    {
        unsigned char   Stepping;
        unsigned char   NumMajor;
        unsigned char   NumMinor;
        unsigned char   NumRelease;
    }
    field;
    unsigned int reg;
}
P_Unit_Version_t;

static DECLARE_WAIT_QUEUE_HEAD(wq);
static dev_t                        p_unit_dev_t;
static unsigned int                 count = 1;
static struct cdev *                p_unit_cdev;
static struct class *               p_unit_udev_class;
struct semaphore                    P_Unit_Sem;
static int                          gPunitResetEvent;
struct workqueue_struct             *gPunitWorkqueue;
struct work_struct                  gPunitWork;
static P_UnitInterruptsEvent_t      interruptsEventIds[APP_LAST_INTERRUPT];
static P_Unit_Statistics_t          p_unit_stat;
static int                          isThreadFinished = 1;
static bool                         isPunitVersionSupportResetNotification = false;

#define P_UNIT_ACK_TIMEOUT_FOR_SOC_RESET_IN_MSEC 5000
#define P_UNIT_PROC_FS_BUFF_SZ (32*128)

P_UNIT_RESET_STR(rst_reason_names);
P_UNIT_RESET_ORIGIN_STR(rst_origin_names);

#define MAX_TIME_IN_MSEC(a, b) ((a) > (b) ? (a) : (b))

#ifndef MAX_UINT32
#define MAX_UINT32                  (0xFFFFFFFF)
#endif

#define MAX_TRYING_TO_LOCK_EMMC_TIME_IN_MSEC (3000) /*maximum time trying to lock sw mutex for eMMC access*/
#define MAX_TIME_HOLD_EMMC_LOCK_IN_MSEC (1000)      /*maximum time locked eMMC access*/

#define ADDR_FW_BUILD_VERSION   (0x00000086)
#define ADDR_FW_VERSION         (0x00000087)
#define ADDR_AC_BOOT_BIT        (0x000000D5)
#define ADDR_DOCSIS_PM_CMD      (0x000000D8)
#define ADDR_DOCSIS_PM_DATA     (0x000000D9)
#define ADDR_PM_DOCSIS_CMD      (0x000000DA)
#define ADDR_PM_DOCSIS_DATA     (0x000000DB)

#define P_UNIT_CMD_TYPE_IPC         (0x0 << 8)
#define P_UNIT_CMD_TYPE_BBU         (0x1 << 8)
#define P_UNIT_CMD_TYPE_WATCHDOG    (0x2 << 8)
#define P_UNIT_CMD_TYPE_RESET       (0x3 << 8)

#define P_UNIT_CMD_DATA_ATTACHED    (0x1 << 14)
#define P_UNIT_CMD_DATA_EXPECTED    (0x1 << 15)

#define P_UNIT_CMD_IDLE             (0x0000)
#define P_UNIT_CMD_ACK              (0x0001)

#define P_UNIT_CMD_RESET_REQ_COLD_RESET                         (P_UNIT_CMD_TYPE_RESET | 0x00) // Initiate a cold reset
#define P_UNIT_CMD_RESET_REQ_WARM_RESET                         (P_UNIT_CMD_TYPE_RESET | 0x01) // Initiate a warm reset
#define P_UNIT_CMD_RESET_REQ_DOCSIS_RESET                       (P_UNIT_CMD_TYPE_RESET | 0x03) // Initiate a docsis only reset
#define P_UNIT_CMD_RESET_REQ_ATOM_RESET                         (P_UNIT_CMD_TYPE_RESET | 0x02) // Initiate a atom only reset
#define P_UNIT_CMD_RESET_GET_LAST_RESET_CAUSE                   (P_UNIT_CMD_TYPE_RESET | 0x04 | P_UNIT_CMD_DATA_EXPECTED) // Returns the last reset cause indicated by firmware. If a hardware reset(not under firmware control) occurs the state indicated here is a "cold boot"
#define P_UNIT_CMD_RESET_CLR_LAST_RESET_CAUSE                   (P_UNIT_CMD_TYPE_RESET | 0x05)
#define P_UNIT_CMD_RESET_GET_DURATION_RESET_HELD                (P_UNIT_CMD_TYPE_RESET | 0x06 | P_UNIT_CMD_DATA_EXPECTED) // Returns the amount of time the reset button was held, if the last reset cause was a reset button. Unit is in milliseconds (valid range: 0-60,000 milliseconds)
#define P_UNIT_CMD_RESET_SET_WARM_RESET_ON_BUTTON               (P_UNIT_CMD_TYPE_RESET | 0x08) // When set, on a reset button press the firmware will start the warm reset sequence (warm reset by default)
#define P_UNIT_CMD_RESET_SET_COLD_RESET_ON_BUTTON               (P_UNIT_CMD_TYPE_RESET | 0x09) // When set, on a reset button press the firmware will start the cold reset sequence (warm reset by default)
#define P_UNIT_CMD_RESET_GET_DURATION_RESET_HELD_SHADOW_REG     (P_UNIT_CMD_TYPE_RESET | 0x0A | P_UNIT_CMD_DATA_EXPECTED) // Returns the amount of time the reset button was held from the shadow register, if the last reset cause was a reset button this value gets updated. Unit is in milliseconds.(valid range: 0-60,000 milliseconds)
#define P_UNIT_CMD_RESET_CLEAR_DURATION_RESET_HELD_SHADOW_REG   (P_UNIT_CMD_TYPE_RESET | 0x0B) // Clears the amount of time the reset button was held in the shadow register
#define P_UNIT_CMD_RESET_EN_DOCSIS_RESET_INDICATION             (P_UNIT_CMD_TYPE_RESET | 0x0E | P_UNIT_CMD_DATA_ATTACHED) // When enabled, if a reset request is sent to Punit firmware, an IPC will be sent to DOCSIS to inform it a reset will occur. Data Sent: 0 = Disable (default), 1 = Enable
#define P_UNIT_CMD_RESET_ALLOW_DOCSIS_INDICATION_IF_SELF_RESET  (P_UNIT_CMD_TYPE_RESET | 0x0F | P_UNIT_CMD_DATA_ATTACHED) // When enabled, if a reset request is sent to Punit firmware from DOCSIS and if enabled an IPC will be sent to DOCSIS to inform it a reset will occur. Suggested to use this mode only in a debug scenario and not in production code. Data Sent: 0 = Disable (default), 1 = Enable
#define P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION                (P_UNIT_CMD_TYPE_RESET | 0x12 | P_UNIT_CMD_DATA_ATTACHED) // If a reset request is sent to firmware and if enabled this IPC will be sent to DOCSIS to inform it a reset will occur.
#define P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION_ACK            (P_UNIT_CMD_TYPE_RESET | 0x13) // After receiving the IPC of a reset request, the DOCSIS should respond with this ACK IPC when it gives the firmware the OK to continue with the reset.
#define P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION_ACK_TIMEOUT    (P_UNIT_CMD_TYPE_RESET | 0x15 | P_UNIT_CMD_DATA_ATTACHED) // Set the timeout time in milliseconds for the DOCSIS to ack, if this time expires the reset sequence will proceed without an ack. Unit is in milliseconds (valid range: 0-60,000 milliseconds) (default 2,000  milliseconds)

#define P_UNIT_CMD_WATCHDOG_DO_COLD_RESET                       (P_UNIT_CMD_TYPE_WATCHDOG | 0x8)
#define P_UNIT_CMD_WATCHDOG_DO_WARM_RESET                       (P_UNIT_CMD_TYPE_WATCHDOG | 0x9)
#define P_UNIT_CMD_WATCHDOG_DO_CPU_RESET                        (P_UNIT_CMD_TYPE_WATCHDOG | 0xA)
#define P_UNIT_CMD_WATCHDOG_DO_NOTHING                          (P_UNIT_CMD_TYPE_WATCHDOG | 0xB)
#define P_UNIT_CMD_ATOM_WATCHDOG_DO_NOTHING                     (P_UNIT_CMD_TYPE_WATCHDOG | 0x7)

#define P_UNIT_CMD_REQ_ATOM_BBU_RESET_TO_CSTATE                 (P_UNIT_CMD_TYPE_BBU | 0x3) //request_atom_bbu_reset_to_cstate
#define P_UNIT_CMD_BBU_EXIT_BBU_MODE                            (P_UNIT_CMD_TYPE_BBU | 0x2)
#define P_UNIT_CMD_BBU_ENTER_BBU_MODE_DONE                      (P_UNIT_CMD_TYPE_BBU | 0x1)

#define P_UNIT_CMD_PUNIT_READY                                  (P_UNIT_CMD_TYPE_IPC | 0x0002)

static void p_unit_start_np_reboot(void);
static int p_unit_print_status(void);
static int p_unit_lockEmmcAccess(void *data);
static void p_unit_notifyUser(int eventId, unsigned int destCtx);
static void p_unit_isr_cmd_ack(void);
static int p_unit_set_ack_timeout(unsigned int timeout);
int p_unit_proc_dump_fw_version(struct seq_file *m, void *v);

/* ******************************************************************** */
/*                                                                      */
/*                                                                      */
/*           P-UNIT Low Level Building Blocks ....                      */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */

/* ******************************************************************** */
static int p_unit_getFwVersion(P_Unit_Version_t * version)
{
    down(&P_Unit_Sem);
    if (iosfsb_read(IOSFSB_PUNIT_PORT, ADDR_FW_VERSION, (unsigned int *)version, IOSFSB_REG_READ))
    {
        up(&P_Unit_Sem);
        return (-1);
    }
    up(&P_Unit_Sem);
    return 0;
}
/* ******************************************************************** */


/* ******************************************************************** */
static int p_unit_getFwBuildVersion(unsigned int *BuildVersion)
{
    down(&P_Unit_Sem);
    if (iosfsb_read(IOSFSB_PUNIT_PORT, ADDR_FW_BUILD_VERSION, (unsigned int *)BuildVersion, IOSFSB_REG_READ))
    {
        up(&P_Unit_Sem);
        return (-1);
    }
    up(&P_Unit_Sem);
    return 0;
}
/* ******************************************************************** */

/**************************************************************************/
/*! \fn  static int p_unit_validateFwVersion( unsigned int version, bool *versionValid )
 **************************************************************************
 *  \brief will check if current pUnit version greater or equal than version (input)
 *  input: version: version to compare pUnit version
 *  input: versionValid: True if version valis, False if version not valid
 *  output: None
 **************************************************************************/
static int p_unit_validateFwVersion(unsigned int validateVersion, bool *versionValid )
{
    P_Unit_Version_t version;
    *versionValid = false;

    if (p_unit_getFwVersion( &version ))
    {
        printk(KERN_ERR " ERROR: function p_unit_getFwVersion has failed\n");
        return  -1;
    }
    else
    {
        if ( (version.reg & 0xFFFFFF) >= validateVersion )
        {
            *versionValid = true;
        }
    }
    return 0;
}
/* ******************************************************************** */

/* ******************************************************************** */
static int p_unit_cmd(unsigned int command)
{
    unsigned int data = command;

    DPRINTK("Enter cmd = 0x%08X \n", command);
    //Send Command to Punit
    if (iosfsb_write(IOSFSB_PUNIT_PORT, ADDR_DOCSIS_PM_CMD, data, IOSFSB_REG_WRITE))
    {
        return (-1);
    }

    PAL_sysBootCfgCtrl_WriteReg(BOOTCFG_REG_SW_INT1_SET, BOOTCFG_REG_SW_INT1_ARM11_2_PUNIT_ISR);

    //Complete Initiator Handshake with Punit
    do
    {
        if (iosfsb_read(IOSFSB_PUNIT_PORT, ADDR_DOCSIS_PM_CMD, &data, IOSFSB_REG_READ))
        {
            PAL_sysBootCfgCtrl_WriteReg(BOOTCFG_REG_SW_INT1_CLR, BOOTCFG_REG_SW_INT1_ARM11_2_PUNIT_ISR);
            return (-1);
        }

#ifdef CONFIG_MACH_PUMA6_FPGA
    data = P_UNIT_CMD_ACK;
#endif

    }
    while (P_UNIT_CMD_ACK != data);

    if (iosfsb_write(IOSFSB_PUNIT_PORT, ADDR_DOCSIS_PM_CMD, P_UNIT_CMD_IDLE, IOSFSB_REG_WRITE))
    {
        PAL_sysBootCfgCtrl_WriteReg(BOOTCFG_REG_SW_INT1_CLR, BOOTCFG_REG_SW_INT1_ARM11_2_PUNIT_ISR);
        return (-1);
    }
    PAL_sysBootCfgCtrl_WriteReg(BOOTCFG_REG_SW_INT1_CLR, BOOTCFG_REG_SW_INT1_ARM11_2_PUNIT_ISR);
    DPRINTK("Exit  cmd = 0x%08X \n", command);

    return 0;
}
/* ******************************************************************** */


/* ******************************************************************** */
static int p_unit_cmd_wr_data(unsigned int  command, unsigned int  data)
{
    int rc;

    DPRINTK("Enter cmd = 0x%08X data = 0x%08X\n", command, data);
    //Send Data
    if(iosfsb_write(IOSFSB_PUNIT_PORT, ADDR_DOCSIS_PM_DATA, data, IOSFSB_REG_WRITE))
    {
        return (-1);
    }

    //Send Command and Wait for ACK
    rc = p_unit_cmd(command);
    DPRINTK("Exit  cmd = 0x%08X data = 0x%08X\n", command, data);

    return rc;
}
/* ******************************************************************** */


/* ******************************************************************** */
static int p_unit_cmd_rd_data(unsigned int command, unsigned int * data)
{
    int rc;

    DPRINTK("Enter cmd = 0x%08X data_ptr = 0x%p\n", command, data);
    //Send Command and Wait for ACK
    if (p_unit_cmd(command))
    {
        return (-1);
    }

    //Get Returned Data
    rc = iosfsb_read(IOSFSB_PUNIT_PORT, ADDR_DOCSIS_PM_DATA, data, IOSFSB_REG_READ);
    DPRINTK("Exit  cmd = 0x%08X data_ptr = 0x%p\n", command, data);

    return rc;
}

/* ******************************************************************** */
static int p_unit_set_configuration( void )
{
    unsigned int    pUnitVersionWithConfiguration = 0x010006;

    DPRINTK("Enter\n");

    DPRINTK("Set watchdog\n");
    if (p_unit_set_watchdog_action( WATCHDOG_DO_COLD_RESET ))
    {
        printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
        return -1;
    }
    DPRINTK("Set reset handling\n");
    if (p_unit_validateFwVersion(pUnitVersionWithConfiguration, &isPunitVersionSupportResetNotification))
    {
        printk(KERN_ERR " ERROR: function p_unit_validateFwVersion has failed\n");
        return  -1;
    }
    else
    {
        if ( isPunitVersionSupportResetNotification == true )
        {
            printk(KERN_INFO "P-UNIT : Enable reset event handling ... \n");
            down(&P_Unit_Sem);
            p_unit_cmd_wr_data(P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION_ACK_TIMEOUT, P_UNIT_ACK_TIMEOUT_FOR_SOC_RESET_IN_MSEC); // ACK timeout is 5 seconds
            p_unit_cmd_wr_data(P_UNIT_CMD_RESET_EN_DOCSIS_RESET_INDICATION, 1);
            /*Enable 2-phase reset notification*/
            p_unit_cmd_wr_data(P_UNIT_CMD_RESET_ALLOW_DOCSIS_INDICATION_IF_SELF_RESET, 1);
            up(&P_Unit_Sem);
        }
    }
    DPRINTK("Exit\n");
    return 0;
}
/* ******************************************************************** */

/**************************************************************************/
/*! \fn  static int p_unit_ack_reset_indication()
 **************************************************************************
 *  \brief send ack for reset indication
 *  input: None
 *  output: None
 **************************************************************************/
static int p_unit_ack_reset_indication(void)
{
    DPRINTK("Enter\n");
    down(&P_Unit_Sem);

    p_unit_cmd( P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION_ACK );

    up(&P_Unit_Sem);
    DPRINTK("Exit\n");
    return 0;
}

/**************************************************************************/
/*! \fn  static int p_unit_disable_atom_wd()
 **************************************************************************
 *  \brief disable Atom WD before putting cState
 *  input: None
 *  output: None
 **************************************************************************/
static int p_unit_disable_atom_wd(void)
{
    DPRINTK("Enter\n");
    down(&P_Unit_Sem);

    /*disable Atom WD*/
    p_unit_cmd( P_UNIT_CMD_ATOM_WATCHDOG_DO_NOTHING );

    up(&P_Unit_Sem);
    DPRINTK("Exit\n");
    return 0;
}

/**************************************************************************/
/*! \fn  static int p_unit_app_cpu_reset(P_UnitResetType_e resetType)
 **************************************************************************
 *  \brief send request to start APP-CPU reset
 *  input: P_UnitResetType_e resetType (Atom to cState/Atom only reset)
 *  output: None
 **************************************************************************/
static int p_unit_app_cpu_reset(P_UnitResetType_e resetType)
{
    unsigned int    pUnitVersionWithAtomOnly = 0x010103;
    bool            isPunitVersionSupportAtomOnly = false;

    DPRINTK("Enter\n");

    if (p_unit_validateFwVersion( pUnitVersionWithAtomOnly, &isPunitVersionSupportAtomOnly))
    {
        printk(KERN_ERR " ERROR: function p_unit_validateFwVersion has failed\n");
        return  -1;
    }
    else
    {
        if ( isPunitVersionSupportAtomOnly == true )
        {
            if (interruptsEventIds[APP_REBOOT_STARTED].isEventRegistered == EVENT_REGISTERED)
            {
                /* Notify user space start APP-CPU reset */
                p_unit_notifyUser(interruptsEventIds[APP_REBOOT_STARTED].eventId, interruptsEventIds[APP_REBOOT_STARTED].destCtx);
            }
            down(&P_Unit_Sem);
            if (resetType == RESET_ATOM_TO_CSTATE)
            {
                /*reset APP cpu to cState*/
                p_unit_cmd( P_UNIT_CMD_REQ_ATOM_BBU_RESET_TO_CSTATE );
            }
            /* resetType == RESET_ATOM */
            else
            {
                /*reset APP cpu only*/
                p_unit_cmd( P_UNIT_CMD_RESET_REQ_ATOM_RESET );
            }
            up(&P_Unit_Sem);
        }
        else
        {
            printk(KERN_ERR " ERROR: p_unit FW does not support APP-CPU reset\n");
            return  -1;
        }
    }
    DPRINTK("Exit\n");
    return 0;
}

/**************************************************************************/
/*! \fn  static int p_unit_upgrade( bool upgradeStatus )
 **************************************************************************
 *  \brief will lock punit in case upgrade start, and reconfigure if upgrade is done
 *  input: upgradeStatus: true -upgrade start, false- upgrade done
 *  output: None
 **************************************************************************/
static int p_unit_upgrade( bool upgradeStatus )
{
    static bool isUpgradeInProgress = false;
    char *  print_ptr;

    /*pUnit upgrade start*/
    if ( upgradeStatus )
    {
        printk("PUNIT upgrade starting...\n");
        /*This case should not happen, only if receive upgrade start again*/
        if (isUpgradeInProgress == true)
        {
            printk(KERN_ERR " ERROR: Punit upgrade already started\n");
            return -1;
        }
        isUpgradeInProgress = true;
        DPRINTK("lock semaphore \n");
        /*Lock semaphore during pUnit upgrade, no one can talk with pUnit during upgrade done in App CPU side*/
        down(&P_Unit_Sem);
        /*We don't want to get any interrupts during pUnit upgrade*/
        disable_irq(AVALANCHE_PUNIT_INT);
    }
    else /*pUnit upgrade done*/
    {
        char print_buf[P_UNIT_PROC_FS_BUFF_SZ];
        struct seq_file m = {
            .buf = print_buf,
            .count = 0,
            .size = P_UNIT_PROC_FS_BUFF_SZ
        };
        /*This case should always happen, upgrade done suppose to receive after upgrade start*/
        if (isUpgradeInProgress == true)
        {
            DPRINTK("unlock semaphore \n");
            /*Release semaphore lock*/
            up(&P_Unit_Sem);
            /*Clear interrupts during pUnit upgrade*/
            ack_irq(AVALANCHE_PUNIT_INT);
            /*Enable interrupts after pUnit upgrade is done*/
            enable_irq(AVALANCHE_PUNIT_INT);
            isUpgradeInProgress = false;
        }
        DPRINTK("Reconfigure pUnit \n");
        if (p_unit_set_configuration())
        {
            printk(KERN_ERR " ERROR: function p_unit_set_configuration has failed\n");
            return  -1;
        }

        p_unit_proc_dump_fw_version( &m, NULL );
        printk("\n==============================================================================\n");
        for ( print_ptr = &m.buf[0]; m.count;  print_ptr++, m.count-- )  { printk("%c",*print_ptr); }
        printk("==============================================================================\n");
    }

    DPRINTK("Exit\n");
    return 0;
}
/* ******************************************************************** */



/* ******************************************************************** */
/*                                                                      */
/*                                                                      */
/*           KERNEL   APIs    ...                                       */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */

/* ******************************************************************** */
int p_unit_reset_soc( void )
{
    DPRINTK("Enter, gPunitResetEvent=%d\n", gPunitResetEvent);

    if ( isPunitVersionSupportResetNotification == true )
    {
        down(&P_Unit_Sem);
        /*Disable 2-phase reset notification*/
        p_unit_cmd_wr_data(P_UNIT_CMD_RESET_ALLOW_DOCSIS_INDICATION_IF_SELF_RESET, 0);
        up(&P_Unit_Sem);
    }
    down(&P_Unit_Sem);
    /* In case of reset event from Punit, we just need to ACK it. Otherwise, we issue cold reset */
    if (gPunitResetEvent)
    {
        p_unit_isr_cmd_ack();
        p_unit_cmd(P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION_ACK);
    }
    else
    {
        p_unit_cmd(P_UNIT_CMD_RESET_REQ_COLD_RESET);
    }
    up(&P_Unit_Sem);
    DPRINTK("Exit, gPunitResetEvent=%d\n", gPunitResetEvent);
    return 0;
}
EXPORT_SYMBOL(p_unit_reset_soc);
/* ******************************************************************** */

/* ******************************************************************** */
/* Resume APP CPU, assuming that suspend done event recieved*/
/* return success = 0, failure = -1*/
/* ******************************************************************** */
static int p_unit_app_cpu_exit_bbu_mode(void)
{
    down(&P_Unit_Sem);
    /*resume App cpu*/
    p_unit_cmd( P_UNIT_CMD_BBU_EXIT_BBU_MODE );
    up(&P_Unit_Sem);
    DPRINTK("Exit\n");
    return 0;
}

/* ******************************************************************** */


/* ******************************************************************** */
int p_unit_get_reset_reason ( unsigned long * reason )
{
    int rc = 0;
    unsigned int data;
    static bool firstRun = true;
    static unsigned int reasonData = 0;

    DPRINTK("Enter\n");
    if (firstRun) // we only read the reset reason once from Punit
    {
        down(&P_Unit_Sem);
        rc = p_unit_cmd_rd_data( P_UNIT_CMD_RESET_GET_LAST_RESET_CAUSE, (unsigned int *)&reasonData );
        up(&P_Unit_Sem);
        if ((reasonData & 0xFF) != RESET_COLD_BOOT) // if not AC boot
        {
            // read punit reg 0xd5 to see if bit1 is set (for AC boot detection)
            rc = iosfsb_read(IOSFSB_PUNIT_PORT, ADDR_AC_BOOT_BIT, (unsigned int *)&data, IOSFSB_REG_READ);
            if (rc)
            {
                printk(KERN_ERR "Error iosfsb_read of p_unit reg 0xd5 (AC-Boot bit)\n");
                return rc;
            }
            if ((data & BIT(1)) != 0) // if bit1 is set '1' we need to return AC boot type and clear the bit
            {
                reasonData &= ~(0xFF); // Reset Type: bits 7-0 ; AC boot is detected if bits 7-0 are zero!
            }
        }

        iosfsb_reg_read_modify_write(IOSFSB_PUNIT_PORT, ADDR_AC_BOOT_BIT, BIT(1), 0); // clear bit1 in reg 0xd5 only!
        firstRun = false;

    }
    *reason = reasonData;
    DPRINTK("Exit\n");

    return rc;
}
EXPORT_SYMBOL(p_unit_get_reset_reason);
/* ******************************************************************** */


/* ******************************************************************** */
int p_unit_get_reset_duration ( unsigned long * duration )
{
    int rc;

    DPRINTK("Enter\n");
    down(&P_Unit_Sem);

    rc = p_unit_cmd_rd_data( P_UNIT_CMD_RESET_GET_DURATION_RESET_HELD, (unsigned int *)duration );

    up(&P_Unit_Sem);
    DPRINTK("Exit\n");

    return rc;
}
EXPORT_SYMBOL(p_unit_get_reset_duration);
/* ******************************************************************** */

/* ******************************************************************** */
int p_unit_get_reset_duration_shadow_reg ( unsigned long * duration )
{
    int rc = 0;
    unsigned int    pUnitVersionWithRstShadow = 0x010102;
    bool            isPunitVersionSupportRstShadow = false;

    DPRINTK("Enter\n");

    *duration = 0;

    if (p_unit_validateFwVersion( pUnitVersionWithRstShadow, &isPunitVersionSupportRstShadow))
    {
        printk(KERN_ERR " ERROR: function p_unit_validateFwVersion has failed\n");
        return  -1;
    }
    else
    {
        if ( isPunitVersionSupportRstShadow == true )
        {
            down(&P_Unit_Sem);

            rc = p_unit_cmd_rd_data( P_UNIT_CMD_RESET_GET_DURATION_RESET_HELD_SHADOW_REG, (unsigned int *)duration );

            up(&P_Unit_Sem);
        }
    }
    DPRINTK("Exit\n");

    return rc;
}
EXPORT_SYMBOL(p_unit_get_reset_duration_shadow_reg);
/* ******************************************************************** */

/* ******************************************************************** */
int p_unit_clear_reset_duration_shadow_reg ( void )
{
    int rc = 0;
    unsigned int    pUnitVersionWithRstShadow = 0x010102;
    bool            isPunitVersionSupportRstShadow = false;

    DPRINTK("Enter\n");

    if (p_unit_validateFwVersion( pUnitVersionWithRstShadow, &isPunitVersionSupportRstShadow))
    {
        printk(KERN_ERR " ERROR: function p_unit_validateFwVersion has failed\n");
        return  -1;
    }
    else
    {
        if ( isPunitVersionSupportRstShadow == true )
        {
            down(&P_Unit_Sem);

            rc = p_unit_cmd( P_UNIT_CMD_RESET_CLEAR_DURATION_RESET_HELD_SHADOW_REG );

            up(&P_Unit_Sem);
        }
    }

    DPRINTK("Exit\n");

    return rc;
}
EXPORT_SYMBOL(p_unit_clear_reset_duration_shadow_reg);
/* ******************************************************************** */


/* ******************************************************************** */
int p_unit_set_watchdog_action(P_UnitWatchdogAction_e action)
{
    unsigned int command;

    DPRINTK("Enter\n");
    switch (action)
    {
        case WATCHDOG_DO_COLD_RESET:    command = P_UNIT_CMD_WATCHDOG_DO_COLD_RESET;    break;
        case WATCHDOG_DO_WARM_RESET:    command = P_UNIT_CMD_WATCHDOG_DO_WARM_RESET;    break;
        case WATCHDOG_DO_CPU_RESET:     command = P_UNIT_CMD_WATCHDOG_DO_CPU_RESET ;    break;
        case WATCHDOG_DO_NOTHING:       command = P_UNIT_CMD_WATCHDOG_DO_NOTHING   ;    break;
        default:
            DPRINTK("Exit\n");
            return (-1);
    }

    down(&P_Unit_Sem);

    p_unit_cmd( command );

    up(&P_Unit_Sem);
    DPRINTK("Exit\n");
    return 0;
}
EXPORT_SYMBOL(p_unit_set_watchdog_action);
/* ******************************************************************** */



/* ******************************************************************** */
/*                                                                      */
/*                                                                      */
/*           IOCTL Implementation                                       */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */

/**************************************************************************/
/*! \fn static long p_unit_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
 **************************************************************************
 *  \brief This function handles device requests.
 *  \param struct file *filp - the device file pointer
 *  \param unsigned int cmd - the command to be performed
 *  \param unsigned long arg - pointer to the user request
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static long p_unit_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;

    switch (cmd)
    {
        case P_UNIT_RESET_SOC_REG_CMD:
        {
            if (p_unit_reset_soc())
            {
                printk(KERN_ERR "Failed to p_unit_reset_soc \n");
                ret = -EFAULT;
            }
            break;
        }

        case P_UNIT_GET_LAST_RESET_REASON_CMD:
        {
            unsigned long rstReason;

            if (p_unit_get_reset_reason(&rstReason))
            {
                printk(KERN_ERR "Failed to p_unit_get_reset_reason \n");
                ret = -EFAULT;
            }
            else
            {
                if (copy_to_user((void __user *)arg, &rstReason, sizeof(unsigned long)))
                {
                    printk(KERN_ERR "Failed to copy to user \n");
                    ret = -EFAULT;
                }
            }
            break;
        }

        case P_UNIT_GET_LAST_RESET_DURATION_CMD:
        {
            unsigned long rstDuration;

            if (p_unit_get_reset_duration(&rstDuration))
            {
                printk(KERN_ERR "Failed to p_unit_get_reset_duration \n");
                ret = -EFAULT;
            }
            else
            {
                if (copy_to_user((void __user *)arg, &rstDuration, sizeof(unsigned long)))
                {
                    printk(KERN_ERR "Failed to copy to user \n");
                    ret = -EFAULT;
                }
            }
            break;
        }

        case P_UNIT_SET_WATCHDOG_ACTION_CMD:
        {
            unsigned long watchdogAction;

            if (copy_from_user(&watchdogAction, (void __user *)arg, sizeof(watchdogAction)))
            {
                printk(KERN_ERR "Failed to copy from user \n");
                return -EFAULT;
            }

            if (p_unit_set_watchdog_action((P_UnitWatchdogAction_e)watchdogAction))
            {
                printk(KERN_ERR "Failed to p_unit_set_watchdog_action \n");
                return -EFAULT;
            }

            break;
        }

        case P_UNIT_SET_APP_CPU_EXIT_BBU_MODE:
        {
            if (p_unit_app_cpu_exit_bbu_mode())
            {
                printk(KERN_ERR "Failed to p_unit_exit_bbu_mode \n");
                return -EFAULT;
            }
            break;
        }

        case P_UNIT_UPGRADE_START:
        {
            if (p_unit_upgrade(true))
            {
                printk(KERN_ERR "Failed to p_unit_upgrade \n");
                ret = -EFAULT;
            }
            break;
        }

        case P_UNIT_UPGRADE_DONE:
        {
            if (p_unit_upgrade(false))
            {
                printk(KERN_ERR "Failed to p_unit_upgrade \n");
                ret = -EFAULT;
            }
            break;
        }

        case P_UNIT_SET_EVENTS:
        {
            DPRINTK("Update interrupts events \n");
            if (copy_from_user(&interruptsEventIds, (void __user *)arg, sizeof(interruptsEventIds)))
            {
                printk(KERN_ERR "Failed to copy from user \n");
                return -EFAULT;
            }
            break;
        }

        case P_UNIT_RESET_APP_CPU_ONLY:
        {
            p_unit_app_cpu_reset(RESET_ATOM);
            break;
        }

        case P_UNIT_GET_RESET_DURATION_SHADOW_CMD:
        {
           unsigned long rstDuration;

           if (p_unit_get_reset_duration_shadow_reg(&rstDuration))
           {
               printk(KERN_ERR "Failed to p_unit_get_reset_duration_shadow_reg \n");
               ret = -EFAULT;
           }
           else
           {
               if (copy_to_user((void __user *)arg, &rstDuration, sizeof(unsigned long)))
               {
                   printk(KERN_ERR "Failed to copy to user \n");
                   ret = -EFAULT;
               }
           }
           break;
        }

        case P_UNIT_CLR_RESET_DURATION_SHADOW_CMD:
        {
           if (p_unit_clear_reset_duration_shadow_reg())
           {
               printk(KERN_ERR "Failed to p_unit_clear_reset_duration_shadow_reg \n");
               ret = -EFAULT;
           }
           break;
        }
        case P_UNIT_RESET_APP_CPU_TO_CSTATE:
        {
            p_unit_disable_atom_wd();
            p_unit_app_cpu_reset(RESET_ATOM_TO_CSTATE);
            break;
        }
        default:
            printk(KERN_ERR "%s:%d Invalid IOCTL(0x%08X) has been received \n",__FUNCTION__,__LINE__,cmd);
            ret = -ENOSYS;
            break;
    }
    return ret;
}


/* ******************************************************************** */
/*                                                                      */
/*                                                                      */
/*           Proc File System ...                                       */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */

int p_unit_proc_dump_statistics(struct seq_file *m, void *v)
{
     seq_printf(m, "Number of APP-CPU reset to cSTate = %d \n", p_unit_stat.numOfAppCpuResetToCstate);
     seq_printf(m,"Number of APP-CPU only reset = %d \n", p_unit_stat.numOfAppCpuOnlyReset);
     seq_printf(m,"Number of APP-CPU nagra exit BBU mode reset = %d \n", p_unit_stat.numOfAppCpuNagraBbuExitReset);
     seq_printf(m,"Max time it took to lock eMMc access = %d\n", p_unit_stat.maxTimeInMsecTakeLockEmmcMutex);
     seq_printf(m,"Max time eMMC access blocked in msec = %d \n", p_unit_stat.maxTimeInMsecEmmcBlocked);

     return 0;
}

int p_unit_proc_dump_fw_version(struct seq_file *m, void *v)
{
     P_Unit_Version_t   version;
     unsigned int BuildVersion;
     char fwType;

     if (p_unit_getFwVersion( &version ))
     {
         seq_printf(m," ERROR: function p_unit_getFwVersion has failed\n");
     }
     else
     {
         seq_printf(m,
                         "P-UNIT : FW version is [ %d.%d.%d ]\n",
                         version.field.NumMajor,
                         version.field.NumMinor,
                         version.field.NumRelease
                        );
         seq_printf(m,
                         "P-UNIT : FW is backward compatible down to Silicon Stepping [%02X]\n",
                         version.field.Stepping
                         );
     }
     if (p_unit_getFwBuildVersion(&BuildVersion))
     {
         seq_printf(m," ERROR: function p_unit_getFwBuildVersion has failed\n");
     }
     else
     {
         fwType = (BuildVersion >> 24);
         if ( (fwType != 'R') && (fwType != 'P') )
         {
             seq_printf(m, "P-UNIT : FW version is not official [ %c-%x ]\n",fwType, (BuildVersion & 0x00ffffff));
         }
         else
         {
             seq_printf(m, "P-UNIT : FW build version is [ %c-%x ]\n",fwType, (BuildVersion & 0x00ffffff));
         }
     }

     return 0;
}

int p_unit_proc_dump_status(struct seq_file *m, void *v)
{
    unsigned int    rstReason;
    unsigned int    rstOrigin;
    unsigned int    rstDuration;
    unsigned int    rstDurationShadow;
    unsigned int    pUnitVersionWithRstShadow = 0x010102;
    bool            isPunitVersionSupportRstShadow = false;
    unsigned int tmp;

    p_unit_get_reset_reason((unsigned long *)&tmp);

    rstReason = (tmp & 0x00FF);
    rstOrigin = (tmp & 0xFF00) >> 8;

    seq_printf (m, "P-UNIT : Last reset reason   = 0x%08X [ %-30s ]\n",rstReason, (rstReason < RESET_REASONS_MAX) ? rst_reason_names[ rstReason ] : "UNKNOWN");

    if (m->count + 128 > m->size)
    {
        seq_printf(m, "\n...\n"); /* may print nothing if previous line is already out of range. the same below */
        return 0;
    }

    seq_printf (m, "P-UNIT : Last reset origin   = 0x%08X [ %-30s ]\n",rstOrigin, (rstOrigin < RESET_ORIGIN_MAX) ? rst_origin_names[ rstOrigin ] : "UNKNOWN");

    if (m->count + 128 > m->size)
    {
        seq_printf(m, "\n...\n");
        return 0;
    }

    if (p_unit_validateFwVersion( pUnitVersionWithRstShadow, &isPunitVersionSupportRstShadow))
    {
        printk(KERN_ERR " ERROR: function p_unit_validateFwVersion has failed\n");
        return  -1;
    }
    else
    {
        if ( isPunitVersionSupportRstShadow == true )
        {
            p_unit_get_reset_duration_shadow_reg((unsigned long *)&rstDurationShadow);

            seq_printf (m, "P-UNIT : Reset duration from shadow register = %d.%d Seconds\n",rstDurationShadow/1000, rstDurationShadow%1000);

            if (m->count + 128 > m->size)
            {
                seq_printf(m,"\n...\n");
                return 0;
            }
        }
        p_unit_get_reset_duration((unsigned long *)&rstDuration);

        seq_printf (m, "P-UNIT : Last reset duration = %d.%d Seconds\n",rstDuration/1000, rstDuration%1000);

        if (m->count + 128 > m->size)
        {
            seq_printf(m,"\n...\n");
        }
    }
    return 0;
}



/**************************************************************************/
/*! \fn int p_unit_proc_control(struct file *fp, const char __user * buf, size_t count, loff_t * f_pos)
 **************************************************************************
 *  \brief proc file to configure P-Unit
 **************************************************************************/
static int p_unit_proc_control(struct file *fp, const char __user * buf, size_t count, loff_t * f_pos)
{
    unsigned char local_buf[50];
    int ret_val = 0;
    unsigned int operation;

    if (count > 50)
    {
        printk(KERN_ERR "Buffer Overflow\n");
        return -EFAULT;
    }

    if(copy_from_user(local_buf,buf,count))
        return -EFAULT;
    local_buf[count-1]='\0'; /* Ignoring last \n char */
    ret_val = count;

    if (strcmp(local_buf,"wd"))
    {
        sscanf(local_buf+2,"%d",&operation);
        p_unit_set_watchdog_action((P_UnitWatchdogAction_e)operation);
    }
    else
    {
        printk(KERN_ERR "Unknown operation, must be read ('r') or write ('w') \n");
        return -EFAULT;
    }

    return ret_val;
}


/* ******************************************************************** */
/*                                                                      */
/*                                                                      */
/*           Module init ...                                            */
/*                                                                      */
/*                                                                      */
/* ******************************************************************** */

/* Structure to map driver functions to kernel */
struct file_operations p_unit_drv_fops =
{
        .owner   = THIS_MODULE,
        .unlocked_ioctl   = p_unit_ioctl,
        .open    = NULL,
        .release = NULL,
};

static void p_unit_isr_cmd_ack(void)
{
    if (iosfsb_write(IOSFSB_PUNIT_PORT, ADDR_PM_DOCSIS_DATA, 0, IOSFSB_REG_WRITE))
    {
        printk("Error in first iosfsb_write of p_unit_isr_cmd_ack \n");
    }
    if (iosfsb_write(IOSFSB_PUNIT_PORT, ADDR_PM_DOCSIS_CMD, P_UNIT_CMD_ACK, IOSFSB_REG_WRITE))
    {
        printk("Error in second iosfsb_write of p_unit_isr_cmd_ack \n");
    }
}

static void argv_cleanup(struct subprocess_info *info)
{
    argv_free(info->argv);
}

/**************************************************************************/
/*! \fn  static void p_unit_notifyUser(int eventId)
 **************************************************************************
 *  \brief This function will run iccGen, that will notify user space on APP-CPU event
 *  input: int eventId
 *  input: unsigned int destCtx
 *  output: None
 **************************************************************************/
static void p_unit_notifyUser(int eventId, unsigned int destCtx)
{
    char buf[64];
    int len = 0;
    int argc;
    char **argv;
    char *envp[] = {"HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
    struct subprocess_info *info;

    len = sprintf(buf, "/usr/sbin/icc_genEvent %d %u", eventId, destCtx);
    buf[len] = NULL;
    DPRINTK("len = %d buf = %s... \n", len, buf);
    argv = argv_split(GFP_KERNEL, buf, &argc);
    DPRINTK("notify User on event = %d... \n", eventId);
    info = call_usermodehelper_setup(argv[0], argv, envp, GFP_KERNEL, NULL, argv_cleanup, NULL);
    if (info == NULL)
    {
        printk(KERN_ERR "failed to set arg\n");
        argv_free(argv);
    }
    else
    {
        call_usermodehelper_exec(info, UMH_WAIT_EXEC);
    }
}

/**************************************************************************/
/*! \fn  static int p_unit_lockEmmcAccess(void *data)
 **************************************************************************
 *  \brief Thread function, will lock emmc sw mutex and execute pUnit command
 *  input: void *data: action to run after locked emmc sw mutex
 *  output: None
 **************************************************************************/
static int p_unit_lockEmmcAccess(void *data)
{
    int res = 0;
    unsigned long timeBefore, timeAfter, totalTime;
    unsigned long timeBeforeLockInJiffies, timeAfterLockInJiffies;
    unsigned int waitingTimeInJiffies = 0;

    isThreadFinished = 0;
    DPRINTK("lockEmmcAccess start....\n");

    /* Start try to lock (for 3 seconds) eMMC sw mutex to disable eMMC access */
    timeBeforeLockInJiffies = jiffies;
    waitingTimeInJiffies = timeBeforeLockInJiffies + msecs_to_jiffies(MAX_TRYING_TO_LOCK_EMMC_TIME_IN_MSEC);

    do
    {
        if (try_lock_emmc_access())
        {
            res = 1;
            break;
        }
    }while (time_after(waitingTimeInJiffies, jiffies));

    /* If failed to lock eMMC need to start SOC reboot */
    if (!res)
    {
        printk(KERN_ERR "ERROR -- waiting blocking emmc access took more than %d msec \n", MAX_TRYING_TO_LOCK_EMMC_TIME_IN_MSEC);
        p_unit_start_np_reboot();
        return 0;
    }

    timeAfterLockInJiffies = jiffies;

    DPRINTK("Set emmc blocker bit \n");
    arm_atom_mbx_send_notification(ARM11_MBX_ATOM_RESET_SYNC, NULL);

    /* Measure time eMMC was locked, use 450MHz clock for accuracy */
    timeBefore = FREE_RUNNING_COUNTER_L_GET();
    p_unit_ack_reset_indication();

    /* wait maximum 1 second for the Atom to clear emmc blocker bit (indicates CEFDK finished eMMC initialization) */
    waitingTimeInJiffies = jiffies + msecs_to_jiffies(MAX_TIME_HOLD_EMMC_LOCK_IN_MSEC);
    do
    {
        if ((arm_atom_mbx_get_arm_reg() & ARM11_MBX_ATOM_RESET_SYNC) == 0)
        {
            break;
        }
    }while(time_after(waitingTimeInJiffies, jiffies));

    unlock_emmc_access();
    timeAfter = FREE_RUNNING_COUNTER_L_GET();

    if (timeBefore <= timeAfter)
    {
        totalTime = timeAfter - timeBefore;
    }
    else
    {
        totalTime = MAX_UINT32 - timeBefore + timeAfter;
    }

    /* Use clock of 450MHz */
    /* msec = cnt/clk/10^3 = cnt/(450MHz * 10^3) = cnt/(450000) */
    totalTime = totalTime/450000;

    if ((arm_atom_mbx_get_arm_reg() & ARM11_MBX_ATOM_RESET_SYNC) != 0)
    {
        printk(KERN_ERR "ERROR -- waiting for emmc_blocker clear took more than %lu msec \n", totalTime);
        p_unit_start_np_reboot();
        return 0;
    }

    /* Save maximum time eMMC was locked */
    p_unit_stat.maxTimeInMsecEmmcBlocked = MAX_TIME_IN_MSEC(p_unit_stat.maxTimeInMsecEmmcBlocked, totalTime);

    /* Save maximum time it took to lock eMMC */
    if (timeAfterLockInJiffies < timeBeforeLockInJiffies)
    {
        p_unit_stat.maxTimeInMsecTakeLockEmmcMutex = MAX_TIME_IN_MSEC(p_unit_stat.maxTimeInMsecTakeLockEmmcMutex, jiffies_to_msecs(MAX_UINT32 - timeBeforeLockInJiffies + timeAfterLockInJiffies));
    }
    else
    {
        p_unit_stat.maxTimeInMsecTakeLockEmmcMutex = MAX_TIME_IN_MSEC(p_unit_stat.maxTimeInMsecTakeLockEmmcMutex, jiffies_to_msecs(timeAfterLockInJiffies - timeBeforeLockInJiffies));
    }

    printk("Total locked emmc sw mutex time = %lu msec\n", totalTime );
    isThreadFinished = 1;
    return 0;
}

/**************************************************************************/
/*! \fn  static void p_unit_start_np_reboot(int eventId)
 **************************************************************************
 *  \brief This function will start NP-CPU reboot
 *  input: None
 *  output: None
 **************************************************************************/
static void p_unit_start_np_reboot()
{
    int ret = -ENOMEM;
    int argc;
    char **argv = argv_split(GFP_KERNEL, "/sbin/reboot", &argc);
    static char *envp[] = {"HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
    struct subprocess_info *info;

    printk(KERN_CRIT "Shutting down the system now...\n");

    info = call_usermodehelper_setup(argv[0], argv, envp, GFP_KERNEL, NULL, argv_cleanup, NULL);
    if (info == NULL)
    {
        argv_free(argv);
    }
    else
    {
        ret = call_usermodehelper_exec(info, UMH_NO_WAIT);
    }

    if ((info == NULL) || ret)
    {
        printk(KERN_ERR "Failed to reboot: forcing orderly_poweroff\n");
        orderly_poweroff(true);
    }

}

static void p_unit_workq_func(struct work_struct *work)
{
    unsigned int command = 0;
    unsigned int data = 0;
    struct task_struct *task = NULL;

    if (iosfsb_read(IOSFSB_PUNIT_PORT, ADDR_PM_DOCSIS_CMD, &command, IOSFSB_REG_READ))
    {
        printk("Error in reading Punit command from iosfsb_read \n");
    }
    if (iosfsb_read(IOSFSB_PUNIT_PORT, ADDR_PM_DOCSIS_DATA, &data, IOSFSB_REG_READ))
    {
        printk("Error in reading Punit data from iosfsb_read \n");
    }

    switch (command)
    {
        case P_UNIT_CMD_BBU_ENTER_BBU_MODE_DONE:
            DPRINTK("#########  PUNIT Interrupted - APP CPU enter bbu mode done ##########\n");
            p_unit_isr_cmd_ack();
            if (interruptsEventIds[APP_SUSPEND_DONE].isEventRegistered == EVENT_REGISTERED)
            {
                p_unit_notifyUser(interruptsEventIds[APP_SUSPEND_DONE].eventId, interruptsEventIds[APP_SUSPEND_DONE].destCtx);
            }
            break;

        case P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION:
            DPRINTK("#########  PUNIT Interrupted - reset indication ##########\n");
            if ( ((data & 0xFF) == RESET_ATOM) || ((data & 0xFF) == RESET_ATOM_TO_CSTATE) || ((data & 0xFF) == RESET_ATOM_NAGRA_BBU_EXIT) )
            {
                p_unit_isr_cmd_ack();
                if (interruptsEventIds[APP_REBOOT_STARTED].isEventRegistered == EVENT_REGISTERED)
                {
                    p_unit_notifyUser(interruptsEventIds[APP_REBOOT_STARTED].eventId, interruptsEventIds[APP_REBOOT_STARTED].destCtx);
                }

                if ((data & 0xFF) == RESET_ATOM)
                {
                    printk(" \n PUNIT Interrupt, reason - Atom only reset indication [data = 0x%x] \n", data);
                    p_unit_stat.numOfAppCpuOnlyReset++;
                }
                else if ((data & 0xFF) == RESET_ATOM_TO_CSTATE)
                {
                    printk(" \n PUNIT Interrupt, reason - Atom reset to cState [data = 0x%x] \n", data);
                    p_unit_stat.numOfAppCpuResetToCstate++;
                }
                else
                {
                    printk(" \n PUNIT Interrupt, reason - Atom reset exit bbu mode in nagra SKU [data = 0x%x] \n", data);
                    p_unit_stat.numOfAppCpuNagraBbuExitReset++;
                }

                if (isThreadFinished)
                {
                    task = kthread_run(p_unit_lockEmmcAccess,  NULL, "appCpuOnlyReset");
                }
                else
                {
                    printk(KERN_ERR "P-UNIT: lock emmc access kthread already running\n");
                }
            }
            else
            {
                gPunitResetEvent = 1;
                printk(" \n PUNIT Interrupt, reason - dual boot reset indication [data = 0x%x] \n", data);
                p_unit_start_np_reboot();
            }
            break;
        default:
            printk("\n PUNIT Interrupt - unknown command 0x%04X, data 0x%08X \n", command, data);
            p_unit_isr_cmd_ack();
            break;
    }
}

static irqreturn_t p_unit_isr(int irq, void *dev_id)
{
    /* Que work */
    queue_work(gPunitWorkqueue, &gPunitWork);

    return IRQ_HANDLED;
}

/**************************************************************************/
/*! \fn static int p_unit_print_status(void)
 **************************************************************************
 *  \brief This function print pUnit status
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static int p_unit_print_status(void)
{
    char *  print_ptr;
    char print_buf[P_UNIT_PROC_FS_BUFF_SZ];
    struct seq_file m = {
        .buf = print_buf,
        .count = 0,
        .size = P_UNIT_PROC_FS_BUFF_SZ
    };

    p_unit_proc_dump_fw_version( &m, NULL );
    p_unit_proc_dump_status( &m, NULL );

    printk("\n==============================================================================\n");
    for ( print_ptr = &m.buf[0]; m.count;  print_ptr++, m.count-- )  { printk("%c",*print_ptr); }
    printk("==============================================================================\n");

    return 0;
}

/**************************************************************************/
/*! \fn static int p_unit_set_ack_timeout(unsigned int timeout)
 **************************************************************************
 *  \brief This function set pUnit ack timeout
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static int p_unit_set_ack_timeout(unsigned int timeout)
{
    down(&P_Unit_Sem);
    p_unit_cmd_wr_data(P_UNIT_CMD_RESET_DOCSIS_RESET_INDICATION_ACK_TIMEOUT, timeout);
    up(&P_Unit_Sem);

    return 0;
}

DECLARE_PROCFS_READ_ENTRY(p_unit_status, p_unit_proc_dump_status)
DECLARE_PROCFS_READ_ENTRY(p_unit_fw_version, p_unit_proc_dump_fw_version)
DECLARE_PROCFS_READ_ENTRY(p_unit_statistics, p_unit_proc_dump_statistics)
static const struct file_operations p_unit_proc_control_fops = {
    .write = p_unit_proc_control
};

/**************************************************************************/
/*! \fn static int __init p_unit_drv_init(void)
 **************************************************************************
 *  \brief This function is the power control unit device module init function.
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static int __init p_unit_drv_init(void)
{
#ifdef CONFIG_MACH_PUMA7_FPGA /*Puma7 FPGA doesnt include Punit*/
    printk("******* PUNIT: p_unit_drv_init - OVERRIDE FOR PUM7 FPGA!! SKIP WITHOUT DOING ANYTHING *********\n");
    return(0);
#endif

    int res;
    gPunitResetEvent = 0; /* Indication if reset event has been received from Punit. Default is FALSE */

    printk(KERN_INFO "PUNIT: Intel(R) P-UNIT driver built on %s @ %s\n", __DATE__, __TIME__);

    if (alloc_chrdev_region(&p_unit_dev_t, 0, count, DEV_NAME) < 0)
    { /* count indicates how many minors we get */
        printk(KERN_ERR "%s:%d Failed to register character device region %s\n",__FUNCTION__,__LINE__,DEV_NAME);
        return (-1);
    }

    if (!(p_unit_cdev = cdev_alloc()))
    {
        printk(KERN_ERR "%s:%d Failed to allocate character device %s\n",__FUNCTION__,__LINE__,DEV_NAME);
        unregister_chrdev_region(p_unit_dev_t, count);
        return (-1);
    }

    /* Connect the file operations with the cdev                            */
    cdev_init(p_unit_cdev, &p_unit_drv_fops);

    /* Connect the major/minor number to the cdev  - Activates the device   */
    if (cdev_add(p_unit_cdev, p_unit_dev_t, count) < 0)
    {
        printk(KERN_ERR "%s:%d Failed to add character device %s\n",__FUNCTION__,__LINE__,DEV_NAME);
        cdev_del(p_unit_cdev);
        unregister_chrdev_region(p_unit_dev_t, count);
        return (-1);
    }
    /* ******************************************************************** */
    /* connection to the udev                                               */
    /* ceates a class directory under /sys/class                            */
    p_unit_udev_class = class_create(THIS_MODULE, DEV_NAME);
    /* ceates a class directory under /sys/class/DEV_NAME named DEV_NAME    */
    /* creates 3 file: dev, uevent, subsystem                               */
    device_create(p_unit_udev_class, NULL, p_unit_dev_t, NULL, "%s", DEV_NAME);


    printk(KERN_INFO "PUNIT: Create device: /dev/%s\n",DEV_NAME);

    sema_init(&P_Unit_Sem,1);

    /* Proc filesystem utilities.... */
    {
        struct proc_dir_entry * dir;

        if (NULL == (dir = proc_mkdir("P-UNIT", NULL)))
        {
            printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
            return -1;
        }

        if (NULL == (proc_create( "status" , 0, dir, &p_unit_status_proc_fops )))
        {
            printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
            return -1;
        }

        if (NULL == (proc_create( "fw_version" , 0, dir, &p_unit_fw_version_proc_fops )))
        {
            printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
            return -1;
        }

        if (NULL == (proc_create( "statistics", 0, dir, &p_unit_statistics_proc_fops )))
        {
            printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
            return -1;
        }

        if (NULL == (dir = proc_create("control", 0, dir, &p_unit_proc_control_fops)))
        {
            printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
            return -1;
        }

        printk(KERN_INFO "PUNIT: Create proc file: /proc/%s\n", "P-UNIT");
    }

    p_unit_isr_cmd_ack();

    if (p_unit_set_configuration())
    {
        printk(KERN_ERR "%s:%d ERROR ....Failed to set pUnit configuration\n",__FUNCTION__,__LINE__);
        return -1;
    }

    gPunitWorkqueue = create_workqueue("gPunitWorkqueue");
    if (gPunitWorkqueue == NULL)
    {
        printk(KERN_ERR "%s:%d ERROR ....Failed to create gPunitWorkqueue\n",__FUNCTION__,__LINE__);
        return -1;
    }
    /* Init the work */
    INIT_WORK(&gPunitWork, p_unit_workq_func);

    res = request_irq(AVALANCHE_PUNIT_INT, p_unit_isr, IRQF_TRIGGER_RISING | IRQF_DISABLED, "punit_int", &count );
    printk("%d = result from request_irq \n", res);
    if (res)
    {
        printk(KERN_ERR "PUNIT: Unable to allocate pUnit IRQ\n");
        return -1;
    }

    p_unit_print_status();

    p_unit_stat.numOfAppCpuResetToCstate = 0;
    p_unit_stat.numOfAppCpuOnlyReset = 0;
    p_unit_stat.numOfAppCpuNagraBbuExitReset = 0;
    p_unit_stat.maxTimeInMsecTakeLockEmmcMutex = 0;
    p_unit_stat.maxTimeInMsecEmmcBlocked = 0;

    return(0);
}


/**************************************************************************/
/*! \fn static void __exit p_unit_drv_exit(void)
 **************************************************************************
 *  \brief This function is the power control unit device module exit function.
 **************************************************************************/
static void __exit p_unit_drv_exit(void)
{
    if (p_unit_cdev)
    {
        cdev_del(p_unit_cdev);
    }

    unregister_chrdev_region(p_unit_dev_t, count);
    device_destroy(p_unit_udev_class, p_unit_dev_t);
    class_destroy(p_unit_udev_class);
    free_irq(AVALANCHE_PUNIT_INT, NULL);

    flush_workqueue(gPunitWorkqueue);
    destroy_workqueue(gPunitWorkqueue);

    printk(KERN_INFO "%s:%d %s device has been unregistered\n",__FUNCTION__,__LINE__,DEV_NAME);
}
/*************************************************************************************/

module_init(p_unit_drv_init);
module_exit(p_unit_drv_exit);

/* Driver identification */
MODULE_DESCRIPTION("Power Control Unit Device Driver");
MODULE_LICENSE("GPL");


