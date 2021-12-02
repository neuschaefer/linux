/*-----------------------------------------------------------------------------
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 *---------------------------------------------------------------------------
 *
 * $Author: youshun.ding $
 * $Date: 2012/10/19 $
 * $RCSfile: mtk_cust.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file mtk_cust.c
 *  This C file implements the mtk53xx USB host controller customization driver.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/usb.h>
#include <linux/kthread.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>
#include <asm/uaccess.h>
#include <linux/dma-mapping.h>
#include <linux/usb/hcd.h>

#include "mtk_hcd.h"
#include "mtk_qmu_api.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MUC_NUM_PLATFORM_DEV (sizeof(MUC_aLinuxController) / sizeof(MUSB_LinuxController))

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
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

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
#ifdef CONFIG_ARCH_MT5396

static void MGC_VBusControl(uint8_t bPortNum, uint8_t bOn)
{
#ifdef USB_PWR_SWITCH_GPIO_CUST
    printk("\n");
    printk("[USB0]: 5V On, Set GPIO none.\n");
    printk("[USB1]: 5V On, Set GPIO none.\n");
#else 
    unsigned long flags=0;

    printk("[USB%d] Vbus %s.\n", bPortNum, ((bOn) ? "On": "Off"));

    local_irq_save(flags);

    if (bOn)
    {
        switch (bPortNum)
        {
            case 0:
                // GPIO 43            
                *((volatile uint32_t *)0xF000D524) |= 0x00000800;
                *((volatile uint32_t *)0xF000D504) |= 0x00000800;                            
                break;
            case 1:
                // GPIO 44            
                *((volatile uint32_t *)0xF000D524) |= 0x00001000;
                *((volatile uint32_t *)0xF000D504) |= 0x00001000;
                break;
            case 2:
                // OPTCTRL 10  
                *((volatile uint32_t *)0xF002807C) |= 0x00000400;
                *((volatile uint32_t *)0xF0028074) |= 0x00000400;
                break;
            case 3:
                // OPTCTRL 11
                *((volatile uint32_t *)0xF002807C) |= 0x00000800;
                *((volatile uint32_t *)0xF0028074) |= 0x00000800;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (bPortNum)
        {
            case 0:
                // GPIO 43            
                *((volatile uint32_t *)0xF000D524) |= 0x00000800;
                *((volatile uint32_t *)0xF000D504) &= ~0x00000800;                            
                break;
            case 1:
                // GPIO 44            
                *((volatile uint32_t *)0xF000D524) |= 0x00001000;
                *((volatile uint32_t *)0xF000D504) &= ~0x00001000;
                break;
            case 2:
                // OPTCTRL 10  
                *((volatile uint32_t *)0xF002807C) |= 0x00000400;
                *((volatile uint32_t *)0xF0028074) &= ~0x00000400;
                break;
            case 3:
                // OPTCTRL 11
                *((volatile uint32_t *)0xF002807C) |= 0x00000800;
                *((volatile uint32_t *)0xF0028074) &= ~0x00000800;
                break;
            default:
                break;
        }
    }
            
    local_irq_restore(flags);            
          
    #endif 
}

static int MGC_PhyReset(void * pBase, void * pPhyBase)
{
    uint32_t u4Reg = 0;
    uint8_t nDevCtl = 0;
    uint32_t *pu4MemTmp;

    // USB DMA enable
    pu4MemTmp = (uint32_t *)0xf0060200;
    *pu4MemTmp = 0x00cfff00;

    //Soft Reset, RG_RESET for Soft RESET
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x68);
    u4Reg |=   0x00004000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);
    u4Reg &=  ~0x00004000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);

    //otg bit setting
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x6C);
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3f2c;
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x6C, u4Reg);

    //suspendom control
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x68);
    u4Reg &=  ~0x00040000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);

    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);

    // MT5368/MT5396/MT5389 
    //0xf0050070[10]= 1 for Fs Hub + Ls Device 
    u4Reg = MGC_Read8(pBase, (uint32_t)0x71);
    u4Reg |= 0x04;
    MGC_Write8(pBase, 0x71, (uint8_t)u4Reg);

    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);     
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg |= 0x01;
    MGC_Write32(pBase, 0x604, u4Reg);
	
    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);   
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // FS/LS Slew Rate change
    u4Reg = MGC_PHY_Read32(pPhyBase, 0x10);
    u4Reg &= (uint32_t)(~0x000000ff);
    u4Reg |= (uint32_t)0x00000011;
    MGC_PHY_Write32(pPhyBase, 0x10, u4Reg);

    //eye diagram improve   
    //TX_Current_EN setting 01 , 25:24 = 01 //HS enable
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x1C);
    u4Reg |=  0x01000000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x1C, u4Reg);

    // HS Slew Rate
    u4Reg = MGC_PHY_Read32(pPhyBase, 0x10);
    u4Reg &= (uint32_t)(~0x00070000);
    u4Reg |= (uint32_t)0x00040000;
    MGC_PHY_Write32(pPhyBase, 0x10, u4Reg);

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg &= ~0x01;
    MGC_Write32(pBase,0x604, u4Reg);
	
    return 0;
}

static MUSB_LinuxController MUC_aLinuxController[] = 
{
#ifndef USB_DEVICE_MODE_SUPPORT   
    {
         /* Port 0 information. */
        .pBase = (void*)MUSB_BASE, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT0_PHYBASE)), 
        .dwIrq = VECTOR_USB0, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 6,   /* Endpoint Number : 5+1*/
        .bHub_support = FALSE,
        .MGC_pfVbusControl = MGC_VBusControl, 
        .MGC_pfPhyReset = MGC_PhyReset,          
    },
#endif    
    #ifdef MUSB_UTMI_SWITCH_PORT_1_2
    {
         /* Port 2 information. */
        .pBase = (void*)MUSB_BASE2, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT2_PHYBASE)), 
        .dwIrq = VECTOR_USB2, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 9,   /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,
    },
    {
         /* Port 1 information. */
        .pBase = (void*)MUSB_BASE1, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT1_PHYBASE)), 
        .dwIrq = VECTOR_USB1, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 6,   /* Endpoint Number : 5+1*/
        .bHub_support = FALSE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,
    },
    #else
    {
         /* Port 1 information. */
        .pBase = (void*)MUSB_BASE1, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT1_PHYBASE)), 
        .dwIrq = VECTOR_USB1, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 6,   /* Endpoint Number : 5+1*/
        .bHub_support = FALSE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,        
    },
    {
         /* Port 2 information. */
        .pBase = (void*)MUSB_BASE2,
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT2_PHYBASE)),         
        .dwIrq = VECTOR_USB2, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 9,   /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,        
    },
    #endif
    {
         /* Port 3 information. */
        .pBase = (void*)MUSB_BASE3, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT3_PHYBASE)),         
        .dwIrq = VECTOR_USB3, 
        .bSupportCmdQ = FALSE, 
        .bEndpoint_num = 9,   /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,        
    }
};

#elif defined(CONFIG_ARCH_MT5368)

static void MGC_VBusControl(uint8_t bPortNum, uint8_t bOn)
{
#ifdef USB_PWR_SWITCH_GPIO_CUST
    printk("\n");
    printk("[USB0]: 5V On, Set GPIO none.\n");
    printk("[USB1]: 5V On, Set GPIO none.\n");
#else 
    unsigned long flags=0;

    printk("[USB%d] Vbus %s.\n", bPortNum, ((bOn) ? "On": "Off"));

    local_irq_save(flags);

    if (bOn)
    {
        switch (bPortNum)
        {
            case 0:               
                // OPTCTRL 0        
                *((volatile uint32_t *)0xf002807c) |= 0x00000001;
                *((volatile uint32_t *)0xf0028074) |= 0x00000001;
                break;
            case 1:
                // OPTCTRL 1        
                *((volatile uint32_t *)0xf002807c) |= 0x00000002;
                *((volatile uint32_t *)0xf0028074) |= 0x00000002;
                break;
            case 2:
                // OPTCTRL 2        
                *((volatile uint32_t *)0xf002807c) |= 0x00000004;
                *((volatile uint32_t *)0xf0028074) |= 0x00000004;
                break;
            case 3:
                // OPTCTRL 3        
                *((volatile uint32_t *)0xf002807c) |= 0x00000008;
                *((volatile uint32_t *)0xf0028074) |= 0x00000008;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (bPortNum)
        {
            case 0:               
                // OPTCTRL 0        
                *((volatile uint32_t *)0xf002807c) |= 0x00000001;
                *((volatile uint32_t *)0xf0028074) &= ~0x00000001;
                break;
            case 1:
                // OPTCTRL 1        
                *((volatile uint32_t *)0xf002807c) |= 0x00000002;
                *((volatile uint32_t *)0xf0028074) &= ~0x00000002;
                break;
            case 2:
                // OPTCTRL 2        
                *((volatile uint32_t *)0xf002807c) |= 0x00000004;
                *((volatile uint32_t *)0xf0028074) &= ~0x00000004;
                break;
            case 3:
                // OPTCTRL 3        
                *((volatile uint32_t *)0xf002807c) |= 0x00000008;
                *((volatile uint32_t *)0xf0028074) &= ~0x00000008;
                break;
            default:
                break;
        }
    }
            
    local_irq_restore(flags);            
          
    #endif 
}

static int MGC_PhyReset(void * pBase, void * pPhyBase)
{
    uint32_t u4Reg = 0;
    uint8_t nDevCtl = 0;
    uint32_t *pu4MemTmp;

    // USB DMA enable
    pu4MemTmp = (uint32_t *)0xf0060200;
    *pu4MemTmp = 0x00cfff00;

    //Soft Reset, RG_RESET for Soft RESET
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x68);
    u4Reg |=   0x00004000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);
    u4Reg &=  ~0x00004000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);

    //otg bit setting
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x6C);
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3f2c;
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x6C, u4Reg);

    //suspendom control
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x68);
    u4Reg &=  ~0x00040000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);

    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);

    // MT5368/MT5396/MT5389 
    //0xf0050070[10]= 1 for Fs Hub + Ls Device 
    u4Reg = MGC_Read8(pBase, (uint32_t)0x71);
    u4Reg |= 0x04;
    MGC_Write8(pBase, 0x71, (uint8_t)u4Reg);

    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);     
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg |= 0x01;
    MGC_Write32(pBase, 0x604, u4Reg);
	
    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);   
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // FS/LS Slew Rate change
    u4Reg = MGC_PHY_Read32(pPhyBase, 0x10);
    u4Reg &= (uint32_t)(~0x000000ff);
    u4Reg |= (uint32_t)0x00000011;
    MGC_PHY_Write32(pPhyBase, 0x10, u4Reg);

    // HS Slew Rate
    u4Reg = MGC_PHY_Read32(pPhyBase, 0x10);
    u4Reg &= (uint32_t)(~0x00070000);
    u4Reg |= (uint32_t)0x00040000;
    MGC_PHY_Write32(pPhyBase, 0x10, u4Reg);

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg &= ~0x01;
    MGC_Write32(pBase,0x604, u4Reg);
	
    return 0;
}

static MUSB_LinuxController MUC_aLinuxController[] = 
{
#ifndef USB_DEVICE_MODE_SUPPORT   
    {
         /* Port 0 information. */
        .pBase = (void*)MUSB_BASE, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT0_PHYBASE)), 
        .dwIrq = VECTOR_USB0, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 6,   /* Endpoint Number : 5+1*/
        .bHub_support = FALSE,
        .MGC_pfVbusControl = MGC_VBusControl, 
        .MGC_pfPhyReset = MGC_PhyReset,          
    },
#endif    
    #ifdef MUSB_UTMI_SWITCH_PORT_1_2
    {
         /* Port 2 information. */
        .pBase = (void*)MUSB_BASE2, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT2_PHYBASE)), 
        .dwIrq = VECTOR_USB2, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 9,   /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,
    },
    {
         /* Port 1 information. */
        .pBase = (void*)MUSB_BASE1, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT1_PHYBASE)), 
        .dwIrq = VECTOR_USB1, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 6,   /* Endpoint Number : 5+1*/
        .bHub_support = FALSE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,
    },
    #else
    {
         /* Port 1 information. */
        .pBase = (void*)MUSB_BASE1, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT1_PHYBASE)), 
        .dwIrq = VECTOR_USB1, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 6,   /* Endpoint Number : 5+1*/
        .bHub_support = FALSE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,        
    },
    {
         /* Port 2 information. */
        .pBase = (void*)MUSB_BASE2,
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT2_PHYBASE)),         
        .dwIrq = VECTOR_USB2, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 9,   /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,        
    },
    #endif
    {
         /* Port 3 information. */
        .pBase = (void*)MUSB_BASE3, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT3_PHYBASE)),         
        .dwIrq = VECTOR_USB3, 
        .bSupportCmdQ = FALSE, 
        .bEndpoint_num = 9,   /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = MGC_VBusControl,
        .MGC_pfPhyReset = MGC_PhyReset,        
    }
};

#elif defined(CONFIG_ARCH_MT5389)

static void MGC_VBusControl(uint8_t bPortNum, uint8_t bOn)
{
#ifdef USB_PWR_SWITCH_GPIO_CUST
    printk("\n");
    printk("[USB0]: 5V On, Set GPIO none.\n");
    printk("[USB1]: 5V On, Set GPIO none.\n");
#else 
    unsigned long flags=0;

    printk("[USB%d] Vbus %s.\n", bPortNum, ((bOn) ? "On": "Off"));

    local_irq_save(flags);

    if (bOn)
    {
        switch (bPortNum)
        {
            case 0:
                // OPTCTRL 0      
                *((volatile uint32_t *)0xf002807c) |= 0x00000001;
                *((volatile uint32_t *)0xf0028074) |= 0x00000001;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (bPortNum)
        {
            case 0:
                // OPTCTRL 0      
                *((volatile uint32_t *)0xf002807c) |= 0x00000001;
                *((volatile uint32_t *)0xf0028074) &= ~0x00000001;
                break;
            default:
                break;
        }
    }
            
    local_irq_restore(flags);            
          
    #endif 
}

static int MGC_PhyReset(void * pBase, void * pPhyBase)
{
    uint32_t u4Reg = 0;
    uint8_t nDevCtl = 0;
    uint32_t *pu4MemTmp;

    // USB DMA enable
    pu4MemTmp = (uint32_t *)0xf0060200;
    *pu4MemTmp = 0x00cfff00;

    //Soft Reset, RG_RESET for Soft RESET
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x68);
    u4Reg |=   0x00004000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);
    u4Reg &=  ~0x00004000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);

    //otg bit setting
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x6C);
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3f2c;
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x6C, u4Reg);

    //suspendom control
    u4Reg = MGC_PHY_Read32(pPhyBase,(uint32_t)0x68);
    u4Reg &=  ~0x00040000; 
    MGC_PHY_Write32(pPhyBase, (uint32_t)0x68, u4Reg);

    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);

    // MT5368/MT5396/MT5389 
    //0xf0050070[10]= 1 for Fs Hub + Ls Device 
    u4Reg = MGC_Read8(pBase, (uint32_t)0x71);
    u4Reg |= 0x04;
    MGC_Write8(pBase, 0x71, (uint8_t)u4Reg);

    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);     
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg |= 0x01;
    MGC_Write32(pBase, 0x604, u4Reg);
	
    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);   
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // FS/LS Slew Rate change
    u4Reg = MGC_PHY_Read32(pPhyBase, 0x10);
    u4Reg &= (uint32_t)(~0x000000ff);
    u4Reg |= (uint32_t)0x00000011;
    MGC_PHY_Write32(pPhyBase, 0x10, u4Reg);

    // HS Slew Rate
    u4Reg = MGC_PHY_Read32(pPhyBase, 0x10);
    u4Reg &= (uint32_t)(~0x00070000);
    u4Reg |= (uint32_t)0x00040000;
    MGC_PHY_Write32(pPhyBase, 0x10, u4Reg);

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg &= ~0x01;
    MGC_Write32(pBase,0x604, u4Reg);
	
    return 0;
}

static MUSB_LinuxController MUC_aLinuxController[] = 
{
#ifndef USB_DEVICE_MODE_SUPPORT   
    {
         /* Port 0 information. */
        .pBase = (void*)MUSB_BASE, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT0_PHYBASE)), 
        .dwIrq = VECTOR_USB0, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 9,     /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = MGC_VBusControl, 
        .MGC_pfPhyReset = MGC_PhyReset,          
    },
#endif
    {
         /* Port 1 information. */
        .pBase = (void*)MUSB_BASE1, 
        .pPhyBase = (void*)((MUSB_BASE + MUSB_PHYBASE)+ (MUSB_PORT1_PHYBASE)), 
        .dwIrq = VECTOR_USB1, 
        .bSupportCmdQ = TRUE, 
        .bEndpoint_num = 9,     /* Endpoint Number : 8+1*/
        .bHub_support = TRUE,
        .MGC_pfVbusControl = NULL,  /* Only use one gpio */
        .MGC_pfPhyReset = MGC_PhyReset,
    }
};

#else
#error CONFIG_ARCH_MTXXXX not defined !
#endif 

static struct platform_device MUC_pdev[MUC_NUM_PLATFORM_DEV];

//---------------------------------------------------------------------------
// Global function
//---------------------------------------------------------------------------
uint32_t MUCUST_GetControllerNum(void)
{
    return MUC_NUM_PLATFORM_DEV;
}

MUSB_LinuxController* MUCUST_GetControllerDev(uint32_t u4Index)
{
    return &MUC_aLinuxController[u4Index];
}

struct platform_device* MUCUST_GetPlatformDev(uint32_t u4Index)
{
    return &MUC_pdev[u4Index];
}

