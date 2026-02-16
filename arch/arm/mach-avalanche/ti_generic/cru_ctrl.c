/*
 *
 * cru_ctrl.c
 * Description:
 * see below
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011-2017 Intel Corporation.
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
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

/** \file   cru_ctrl.c (formerly puma6_cru_ctrl.c)
 *  \brief  PAL reset and power control APIs
 *          The Clock & Reset unit (CRU) enables power control
 *          of all the modules and peripherals. Power savings
 *          can be achieved by disabling modules (clock gating).
 *          
 *  \author     Intel
 *
 *  \version    0.1     Amihay Tabul   		Created
 */


#include <pal.h>
#include <mach/puma.h>

#if PUMA6_SOC_TYPE
#include <asm-arm/arch-avalanche/puma6/puma6_cru_ctrl.h>
#endif

#if PUMA7_SOC_TYPE
#include <asm-arm/arch-avalanche/puma7/puma7_cru_ctrl.h>
#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs_macros.h>

/* isspace */
#define ISSPACE(c)  ((c) == ' ' || ((c) >= '\t' && (c) <= '\r'))

/* CRU - Clock and Reset Unit               */
/* Docsis IP has 33 CRUs                    */
/* Clock Control registers set memory map:  */

/* Start address 0x000D_0000                */
/* End address   0x000D_FFFF                */
#define CRU_MOD_STATE_BASE    (AVALANCHE_CRU_BASE)
#define CRU_MOD_STATUS_BASE   (CRU_MOD_STATE_BASE + 0x4)
#define CRU_RSTN_CLK_EN_BASE  (CRU_MOD_STATE_BASE + 0x8) /* should not be used, for debug only */

/* CRU_MOD_STATE register fields */
/* 31:2 Reserved (R) */
/* 1:0  MOD_STATE_REG (R/W) */
#define CRU_MOD_STATE_DISABLED      (0)
#define CRU_MOD_STATE_SYNC_RST      (1)
#define CRU_MOD_STATE_CLK_DISABLE   (2)
#define CRU_MOD_STATE_ENABLE        (3)

/* CRU_MOD_STATUS register fields */
/* 31:10 Reserved (R) */
/* 9:6	CRU_CG_EN_1-4 (R) - Module CG status (4 lines) */
/* 5:2	CRU_RST_N_1-4 (R) - Module resets status (4 lines) */
/* 1:0	CRU_SM_STATE (R)  - Module State Machine State (same as in CRU_MOD_STATE values) */

/* CRU_RSTN_CLK_EN register fields */
/* 31:1 Reserved (R) */
/* 0    CRU_RSTN_CLK_EN_FORCE (R/W) Force module reset and opens the clock gater */

#define CRU_MAX_STATUS_LOOP         (1000)


/* Macros to configure the CRU REGs */
#define CRU_MOD_STATE(cru_num)     *((volatile unsigned int *)(CRU_MOD_STATE_BASE   | ((cru_num)<<4)))
#define CRU_MOD_STATUS(cru_num)    *((volatile unsigned int *)(CRU_MOD_STATUS_BASE  | ((cru_num)<<4)))
#define CRU_RSTN_CLK_EN(cru_num)   *((volatile unsigned int *)(CRU_RSTN_CLK_EN_BASE | ((cru_num)<<4)))
#define CRU_GET_SM_STATE(cru_num)  (CRU_MOD_STATUS(cru_num) & (0x3)) /* bits 0-1 are the SM_STATE */

#define DEVICE_NAME         "CruCtrl"
#define DEVICE_MAJOR        23

/* /proc/cru/status line width */
#define CRU_PROC_LINE       28

/* Max length of proc name */
#define CRU_PROC_NAME_MAX   64

/* Proc write input buffer max size */
#define CRU_PROC_KBUF_MAX   16


#if PUMA6_SOC_TYPE
/*! \fn static char* cru_get_unit_name(PAL_SYS_CRU_MODULE_T cru_module_id)
    \brief Get textual presenation of the CRU name.
    \param cru_module_id  - The CRU index number.
    \return CRU name.
*/
static char* cru_get_unit_name(PAL_SYS_CRU_MODULE_T cru_module_id)
{
    /* Not implemented for Puma6*/
    return "N/A";
}
#endif

#if PUMA7_SOC_TYPE
/*! \fn static const char* cru_get_unit_name(PAL_SYS_CRU_MODULE_T cru_module_id)
    \brief Get textual presenation of the CRU name.
    \param cru_module_id  - The CRU index number.
    \return CRU name.
*/
static const char* cru_get_unit_name(PAL_SYS_CRU_MODULE_T cru_module_id)
{
    switch (cru_module_id) 
    {
    case CRU_NUM_ARM11:
        return "ARM11";
    case CRU_NUM_C55:
        return "C55";
    case CRU_NUM_I2C_W_CONTROLLER:
        return "I2C With controller";
    case CRU_NUM_DOCSIS_MAC0:
        return "DOCSIS MAC0";
    case CRU_NUM_ARM11_L2:
        return "ARM11 L2 Cache";
    case CRU_NUM_DOCSIS_PHY0:
        return "OFDM Phy";
    case CRU_NUM_DOCSIS_PHY1:
        return "Legacy Phy";
    case CRU_NUM_PKT_PROCESSOR:
        return "Packet Processor";
    case CRU_NUM_ATOM_INTC:
        return "Atom INT Controller";
    case CRU_NUM_DOCSIS_IP_INFRA:
        return "DOCSIS IP Infrastructure";
    case CRU_NUM_BBU:
        return "BBU";
    case CRU_NUM_WDT:
        return "WD timer";
    case CRU_NUM_RAM:
        return "RAM";
    case CRU_NUM_TIMER0:
        return "Timer 0";
    case CRU_NUM_TIMER1:
        return "Timer 1";
    case CRU_NUM_TIMER2:
        return "Timer 2";
    case CRU_NUM_UART0:
        return "Uart 0";
    case CRU_NUM_UART1:
        return "Uart 1";
    case CRU_NUM_UART2:
        return "Uart 2";
    case CRU_NUM_CPSPDMA0:
        return "CPSPDMA0";
    case CRU_NUM_CPSPDMA1:
        return "CPSPDMA1";
    case CRU_NUM_BOOT_CFG:
        return "Boot Config";
    case CRU_NUM_TDM00:
        return "TDM 00";
    case CRU_NUM_TDM01:
        return "TDM 01";
    case CRU_NUM_TDM10:
        return "TDM 10";
    case CRU_NUM_TDM11:
        return "TDM 10";
    case CRU_NUM_DSP_PROXY:
        return "DSP PROXY";
    case CRU_NUM_DSP_INC:
        return "DSP INC";
    case CRU_NUM_I2C:
        return "I2C";
    case CRU_NUM_PREF_MON:
        return "Performance Monitor";
    case CRU_NUM_RESERVED:
        return "RESERVED";
    case CRU_NUM_MOCA0:
        return "MoCA 0";
    case CRU_NUM_MOCA1:
        return "MoCA 1";
    case CRU_NUM_MOCA2:
        return "MoCA 2";
    case CRU_NUM_C55_L2_CACHE:
        return "C55 L2 Cache";
    case CRU_NUM_GPIO96:
        return "GPIO 96";
    case CRU_NUM_CNT64_4_COUNTERS:
        return "Counter64 x4";
    case CRU_NUM_MAIN_SSX:
        return "Main SSX";
    case CRU_NUM_GBE:
        return "GBE";
    case CRU_NUM_UART3:
        return "Uart 3";
    case CRU_NUM_TDM20:
        return "TDM 20";
    case CRU_NUM_TDM21:
        return "TDM 21";
    case CRU_NUM_SPI_MASTER:
        return "SPI Master";
    case CRU_MAX :
        return "Invaild (Max)";

    }
    return "N/A";
}
#endif


/*! \fn static char* cru_get_state_name(unsigned int cru_state)
    \brief Get textual presenation of the CRU state.
    \param cru_state  - The CRU State 0-3.
    \return CRU state in text.
*/
static const char* cru_get_state_name(unsigned int cru_state)
{
    switch (cru_state) 
    {
    case CRU_MOD_STATE_DISABLED:
        return "Disabled";
    case CRU_MOD_STATE_SYNC_RST:
        return "Sync Reset";
    case CRU_MOD_STATE_CLK_DISABLE:
        return "Clock Disable";
    case CRU_MOD_STATE_ENABLE:
        return "Enabled";
    }
    return "N/A";
}


/*****************************************************************************
 * Reset Control Module.
 *****************************************************************************/
/*! \fn void PAL_sysResetCtrl(unsigned int cru_module_id, PAL_SYS_RESET_CTRL_T reset_ctrl)
    \brief This API is used to assert or de-assert reset for a module. It will block the caller.
    \param cru_module_id Unique module id to assert/de-assert reset
    \param reset_ctrl assert/de-assert reset (IN_RESET, OUT_OF_RESET)
*/
void PAL_sysResetCtrl(unsigned int cru_module_id, PAL_SYS_RESET_CTRL_T reset_ctrl)
{
    PAL_SYS_CRU_MODULE_T module_id = (PAL_SYS_CRU_MODULE_T)(cru_module_id);
    Uint32 loop_cnt = 0;
    Uint32 cru_status;

    switch (reset_ctrl)
    {
        case IN_RESET:
        {
            cru_status = CRU_MOD_STATE_DISABLED;
            break;
        }
        case OUT_OF_RESET:
        {
            cru_status = CRU_MOD_STATE_ENABLE;
            break;
        }
        case CLK_DISABLE:
        {
            cru_status = CRU_MOD_STATE_CLK_DISABLE;
            break;
        }
        case CLK_ENABLE:
        {
            cru_status = CRU_MOD_STATE_SYNC_RST;
            break;
        }
    default:
        printk (KERN_ERR "Invalid CRU Status request (%d) \n",cru_status);
        return;

    }

    if (module_id >= CRU_MAX)
    {
        printk (KERN_ERR "Invalid CRU Id (%d) \n",module_id);
        return;
    }
    
    if (CRU_GET_SM_STATE(module_id) == cru_status)
    {
        printk (KERN_NOTICE "CRU %d is already in CRU state %d [ignore operation]\n",module_id,cru_status);
        return; /* If the current cru status is the same as the user ask for, we ignore the operation.*/
    }

    /* Set new state*/
    CRU_MOD_STATE(module_id) = cru_status;

    /* Make sure that the CRU module is indeed in the correct ask state (OUT_OF_RESET or IN_RESET or CRU_MOD_STATE_CLK_DISABLE) */
    /* This loop will block the caller for some time! */
    do
    {
        if (++loop_cnt >= CRU_MAX_STATUS_LOOP)
        {
            printk (KERN_CRIT "CRU %d is not functional, current cru status %d !! [loop_cnt=%d ; ask_status=%d]\n",cru_module_id,CRU_MOD_STATUS(module_id),loop_cnt,reset_ctrl);
            return;
        }
    } while (CRU_GET_SM_STATE(module_id) != cru_status);

    return;
}


/*! \fn PAL_SYS_RESET_CTRL_T PAL_sysGetResetStatus(unsigned int cru_module_id)
    \brief This API returns the status reset status of a module
    \param cru_module_id Unique module id whose reset status has to be read
    \return Reset assert/de-assert (IN_RESET, OUT_OF_RESET)
*/
PAL_SYS_RESET_CTRL_T PAL_sysGetResetStatus(unsigned int cru_module_id)
{
    PAL_SYS_CRU_MODULE_T module_id = (PAL_SYS_CRU_MODULE_T)(cru_module_id);


    if ( CRU_GET_SM_STATE(module_id) == CRU_MOD_STATE_ENABLE )
    {
        return OUT_OF_RESET;
    }
    else if (CRU_GET_SM_STATE(module_id) == CRU_MOD_STATE_CLK_DISABLE)
    {
        return CLK_DISABLE;
    }
    else 
    {
        return IN_RESET;
    }
}


/*! \fn static long cru_ioctl ( struct file * filp , unsigned int cmd , unsigned long arg )
    \brief This function is used to ioctl the CruCtrl driver
    \param struct file * filp , unsigned int cmd , unsigned long arg
*/
static long cru_ioctl ( struct file * filp , unsigned int cmd , unsigned long arg )
{
    void __user *p = (void __user *)arg;
    unsigned int val;
    PAL_SYS_RESET_CTRL_T reset_ctrl;

    switch ( cmd )
    {
    case TIOCUARTINRESET:/*Disable the CRU clock*/
        reset_ctrl = CLK_DISABLE;
        break;
    case TIOCUARTOUTOFRESET:/*Enable the CRU*/
        reset_ctrl = OUT_OF_RESET;
        break;
    default:
        printk(KERN_ERR "Unsupported command\n");
        return -EINVAL;
    }

    if(get_user(val,(int *)p))
    {
        printk(KERN_ERR "Failed in get_user\n");
        return -EINVAL;
    }

    PAL_sysResetCtrl(val,reset_ctrl);

    return 0;
}

/* Structure that declares the usual file */
/* Access functions */
static struct file_operations fops =
{
    .unlocked_ioctl  = cru_ioctl,
};


/*! \fn static int cru_status_proc_read(struct seq_file *m, void *v)
    \brief /proc read function. Use to dump all CRU units status.
*/
static int cru_status_proc_read(struct seq_file *m, void *v)
{
    unsigned int i = 0;
    PAL_SYS_CRU_MODULE_T cru_module_id;
    char proc_line[CRU_PROC_LINE+1];
    unsigned int line_length = 0;

    seq_printf(m, "CRU Status:\n");
    
    for (cru_module_id = 0; cru_module_id < CRU_MAX; cru_module_id++)
    {
        /* Build line to print in form of:
           "[id] <cru name> .................... <cru state>"
        */
        strncpy(proc_line, cru_get_unit_name(cru_module_id), CRU_PROC_LINE-1);  /* copy modle name (leave one space for ' ') */
        line_length = strlen(proc_line);               
        proc_line[line_length] = ' ';                                         /* replace '\0' with ' '*/
        line_length++;
        for (i=line_length; i<CRU_PROC_LINE; i++)                               /* fill with dots */
        {
            proc_line[i] = '.';
        }
        proc_line[i] = '\0';                                                  /* add Null terminator  */
        seq_printf(m, "[%02d] %s %s\n", cru_module_id, proc_line, cru_get_state_name(CRU_GET_SM_STATE(cru_module_id)));
    }

    return 0;
}

DECLARE_PROCFS_READ_ENTRY(cru_status, cru_status_proc_read)


/*! \fn static int cru_unit_proc_read(struct seq_file *m, void *v)
    \brief /proc read function. Use to CRU unit status.
*/
static int cru_unit_proc_read(struct seq_file *m, void *v)
{
    PAL_SYS_CRU_MODULE_T cru_module_id = (PAL_SYS_CRU_MODULE_T)(m->private);

    /* Print CRU unit status*/
    seq_printf(m, "[%02d] %s: %s\n", cru_module_id, cru_get_unit_name(cru_module_id), cru_get_state_name(CRU_GET_SM_STATE(cru_module_id)));
    
    return 0;
}


/*! \fn static int cru_unit_proc_write(struct file *fp, const char __user *buf,size_t count, loff_t *f_pos)
    \brief /proc write function. Use to turn On/Off CRU unit.
*/
static int cru_unit_proc_write(struct file *fp, const char __user *buf,size_t count, loff_t *f_pos)
{
    char kbuf[CRU_PROC_KBUF_MAX];  
    int cnt = count;
    PAL_SYS_CRU_MODULE_T cru_module_id = (PAL_SYS_CRU_MODULE_T)PDE_DATA(file_inode(fp));   /* Get CRU index */
    PAL_SYS_RESET_CTRL_T reset_ctrl;

    /* boundary check for input buffer */
    if (count > CRU_PROC_KBUF_MAX-1) 
    {
        printk(KERN_ERR "Failed to write - count is too long (count=%d)\n",count);
        return -EINVAL;
    }

    /* Copy from user buffer to kernel buffer*/
    if (copy_from_user(&kbuf, buf, count) > 0)
    {
        printk(KERN_ERR "Failed to write - failed to copy from user\n");
        return -EINVAL;
    }
   
    /* Trim trailing white space */
    while (ISSPACE(kbuf[cnt-1]))
    {
        cnt--;
    }

    /* Set Null terminator */
    kbuf[cnt] = 0;

    /* Command length should be one charecter only. '1' or '0' */
    if (strlen(kbuf) != 1) 
    {
        printk(KERN_ERR "Invalid command.\n0 - Turn Off CRU.\n1 - Turn On CRU.\n");
        return -EINVAL;
    }

    /* Parse opcode in buffer */
    if (kbuf[0] == '0')
    {
        /* Turn off CRU*/
        reset_ctrl = IN_RESET;
    }
    else if (kbuf[0] == '1')
    {
        /* Turn on CRU*/
        reset_ctrl = OUT_OF_RESET;
    }
    else
    {
        printk(KERN_ERR "Invalid command.\n0 - Turn Off CRU.\n1 - Turn On CRU.\n");
        return -EINVAL;
    }
    printk("%s - %s\n", cru_get_unit_name(cru_module_id), (reset_ctrl==IN_RESET?"OFF":"ON"));

    PAL_sysResetCtrl(cru_module_id, reset_ctrl);

    return count;
}

DECLARE_PROCFS_ENTRY(cru_unit, cru_unit_proc_read, cru_unit_proc_write)


/*! \fn static int __init cru_ctrl_init(void)
    \brief This function is used for init the CruCtrl driver
    \param none
*/
static int __init cru_ctrl_init(void)
{
    char proc_name[CRU_PROC_NAME_MAX] = {0};
    int cru_id = 0;

    printk ( KERN_INFO "Initializing Cru Control module\n");

    /* Registering device */
    if (register_chrdev ( DEVICE_MAJOR, DEVICE_NAME , &fops ) < 0 )
    {

        printk ( KERN_WARNING "memory: cannot obtain major number %d\n", DEVICE_MAJOR );
        return -EIO;
    }
    printk(KERN_INFO "CRU: /dev/%s created\n", DEVICE_NAME);

    /* Create /proc/cru/ */
    proc_mkdir("cru", NULL);
    proc_mkdir("cru/units", NULL);
    proc_create("cru/status", 0644, NULL, &cru_status_proc_fops);
    for (cru_id=0; cru_id<CRU_MAX; cru_id++)
    {
        snprintf(proc_name, CRU_PROC_NAME_MAX, "cru/units/%d", cru_id);
        proc_create_data(proc_name, 0644, NULL, &cru_unit_proc_fops, cru_id);

    }
    printk(KERN_INFO "CRU: /proc/cru/ created\n");

    return 0;

}


/*! \fn static void __exit cru_ctrl_exit(void)
    \brief This function is used for exit the CruCtrl driver
    \param none
*/
static void __exit cru_ctrl_exit(void)
{
    /* Freeing the major number */
    unregister_chrdev ( DEVICE_MAJOR , DEVICE_NAME );
    printk ( KERN_INFO "Removing Cru Control module\n");
}


/* Declaration of the init and exit functions */
module_init ( cru_ctrl_init );
module_exit ( cru_ctrl_exit );


MODULE_AUTHOR ("Intel Corporation");
MODULE_LICENSE ("GPL");
MODULE_DESCRIPTION ("Cable Modem Cru Control");
