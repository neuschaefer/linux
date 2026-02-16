/*
 *  puma6_gpio_ctrl
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2012-2014 Intel Corporation. All rights reserved.
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
 * The file contains the main data structure and API definitions for Linux GPIO driver
 *
 */

/** \file   puma6_gpio_ctrl.c
 *  \brief  GPIO config control APIs. 
 *          
 *  \author     Intel
 *
 *  \version    0.1     Created
 */

#include <pal.h> 
#include <hw_mutex_ctrl.h> 
#include "puma6_gpio_ctrl.h"
#include "aep_gpio.h"

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
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/****************************************************************************/
/****************************************************************************/
/*                          GPIO local definitions                          */
/****************************************************************************/
/****************************************************************************/
typedef unsigned int (*gpio_reg_get_t) (unsigned int addr);
typedef void (*gpio_reg_set_t)(unsigned int addr, unsigned int data);

static unsigned int gpio_reg_get_default (unsigned int addr);
static void gpio_reg_set_default(unsigned int addr,unsigned int data);

static dev_t gpio_dev_t;
static unsigned int count = 1;
static unsigned int ref = 0;
static struct cdev *gpio_cdev;
static struct class *gpio_udev_class;
static struct proc_dir_entry *procFile;
static int readGpioProc(struct seq_file *m, void *v);
gpio_reg_get_t gpio_reg_get_cb = gpio_reg_get_default;
gpio_reg_set_t gpio_reg_set_cb = gpio_reg_set_default;

/*******************************************************************************************
    GPIO Unit Register Map
Offset      Symbol              Register Name/Function                          Default
00h         GPIO_OUT_0          GPIO 31:0 pin data output register.             0000_0000
04h         GPIO_OUT_EN_0       GPIO 31:0 pin output driving register.          0000_0000
08h         GPIO_INPUT_0        GPIO 31:0 pin status input level register.      XXXX_XXXX
0Ch         GPIO_INT_STAT_0     GPIO 31:0 interrupt status register.            0000_0000
10h         GPIO_INT_EN_0       GPIO 31:0 Interrupt Enable Register             0000_0000
14h         GPIO_IN_MODE_LE_0   GPIO 31:0 interrupt Level/Edge                  0000_0000
18h         GPIO_in_MODE_RF_0   GPIO 31:0 Interrupt Rise/Fall                   0000_0000
1Ch         GPIO_MUX_CNTL       GPIO Mux Control                                0000_0000
20h         GPIO_OUT_1          GPIO 63:32 pin data output register.            0000_0000
24h         GPIO_OUT_EN_1       GPIO 63:32 pin output driving register.         0000_0000
28h         GPIO_INPUT_1        GPIO 63:32 pin status input level register.     XXXX_XXXX
2Ch         GPIO_INT_STAT_1     GPIO 63:32 interrupt status register.           0000_0000
30h         GPIO_INT_EN_1       GPIO 63:32 Interrupt Enable Register            0000_0000
34h         GPIO_IN_MODE_LE_1   GPIO 63:32 interrupt Level/Edge                 0000_0000
38h         GPIO_in_MODE_RF_1   GPIO 63:32 Interrupt Rise/Fall                  0000_0000
3Ch            Reserved         See Note                                        
40h         GPIO_OUT_2          GPIO 95:64 pin data output register.            0000_0000
44h         GPIO_OUT_EN_2       GPIO 95:64 pin output driving register.         0000_0000
48h         GPIO_INPUT_2        GPIO 95:64 pin status input level register.     XXXX_XXXX
4Ch         GPIO_INT_STAT_2     GPIO 95:64 interrupt status register.           0000_0000
50h         GPIO_INT_EN_2       GPIO 95:64 Interrupt Enable Register            0000_0000
54h         GPIO_IN_MODE_LE_2   GPIO 95:64 interrupt Level/Edge                 0000_0000
58h         GPIO_in_MODE_RF_2   GPIO 95:64 Interrupt Rise/Fall                  0000_0000
54h            Reserved         See Note                                        
60h         GPIO_OUT_3          GPIO 127:96 pin data output register.           0000_0000
64h         GPIO_OUT_EN_3       GPIO 127:96 pin output driving register.        0000_0000
68h         GPIO_INPUT_3        GPIO 127:96 pin status input level register.    XXXX_XXXX
6Ch         GPIO_INT_STAT_3     GPIO 127:96 interrupt status register.          0000_0000
70h         GPIO_INT_EN_3       GPIO 127:96 Interrupt Enable Register           0000_0000
74h         GPIO_IN_MODE_LE_3   GPIO 127:96 interrupt Level/Edge                0000_0000
78h         GPIO_in_MODE_RF_3   GPIO 127:96 Interrupt Rise/Fall                 0000_0000
7Ch            Reserved         See Note    
80h         GPIO_CLEAR_0        GPIO 31:0 pin data output register clear    
84h         GPIO_SET_0          GPIO 31:0 pin data output register set  
88h         GPIO_POLARITY_0     GPIO 31:0 pin data input polarity invert    
8Ch            Reserved         See Note    
90h         GPIO_CLEAR_1        GPIO 63:32 pin data output register clear   
94h         GPIO_SET_1          GPIO 63:32 pin data output register set 
98h         GPIO_POLARITY_1     GPIO 63:32 pin data input polarity invert   
9Ch            Reserved         See Note    
A0h         GPIO_CLEAR_2        GPIO 95:64 pin data output register clear   
A4h         GPIO_SET_2          GPIO 95:64 pin data output register set 
A8h         GPIO_POLARITY_2     GPIO 95:64 pin data input polarity invert   
ACh            Reserved         See Note    
B0h         GPIO_CLEAR_3        GPIO 127:96 pin data output register clear  
B4h         GPIO_SET_3          GPIO 127:96 pin data output register set    
B8h         GPIO_POLARITY_3     GPIO 127:96 pin data input polarity invert  
BCh         GPIO_GROUP_3        GPIO 127:112 interrupt group select 
C0h to FFh     Reserved         See Note    
***********************************************************************************************/

/****************************************************************************/
/****************************************************************************/
/*                          GPIO Defines                                    */
/****************************************************************************/
/****************************************************************************/
#define DEV_NAME          "docsis_gpio_dev"
#define GPIO_OK           (0)
#define GPIO_FAIL         (-1)
#define DIR_OUT           "Out"
#define DIR_IN            "In "

/*
    PUMA6_GPIO_OUT:
    The output data register controls the logical levels on the pins that are configured as GPIO Outputs.
    These registers may be written or read.
    Address Offsets: 00h, 20h, 40h, 60h
*/
#define PUMA6_GPIO_OUT_REG                              (AVALANCHE_GPIO_BASE + 0x0)
/*
    PUMA6_GPIO_OUT_EN:
    GPIO Direction Control 
        0:  Input mode
        1:  Output mode
    Address Offsets: 04h, 24h, 44h, 64h
*/
#define PUMA6_GPIO_OUT_EN_REG                           (AVALANCHE_GPIO_BASE + 0x4)
/*
    PUMA6_GPIO_INPUT:
    These registers may be only read and reflect the logical levels on the external pins.
    Logical low signal is read as '0', and logical high level is read as '1'.
    Address Offsets: 08h, 28h, 48h, 68h
*/
#define PUMA6_GPIO_INPUT_REG                            (AVALANCHE_GPIO_BASE + 0x8)
/*
    PUMA6_GPIO_INT_STATUS:
    These registers may be read and written and represent an active interrupt status when set.
    Writing a '1' to the bit causes it to be reset. If there is pending event a cleared event may not
    be visible in a subsequent register read. 
    Address Offsets: 0Ch, 2Ch, 4Ch, 6Ch
*/
#define PUMA6_GPIO_INT_STATUS_REG                       (AVALANCHE_GPIO_BASE + 0xC)
/*
    PUMA6_GPIO_INT_EN:
    These registers may be read and written and control the operation of the pins assigned for interrupt request functions.
    All interrupt requests are directly routed into the interrupt controller. It is recommended to set up the Mode registers,
    GPIO_TYP0 and GPIO_TYP1, prior to enabling the interrupt. If a GPIO pin is a shared function and those shared signals are active,
    enabling the interrupt may cause unexpected interrupts. Interrupts are detected based on the requirements of the Mode registers
    and the state of the pin is not required to persist beyond those conditions; however, the interrupt remains asserted until the interrupt
    status bit is cleared.

    GPIO Interrupt Enable Control for pins GPIO0-GPIO31, When set to "1", the corresponding pin may generate interrupt request
    to the main interrupt controller. When cleared, the interrupt requests will not be generated.
    Note:  if a GPIO pin is configured as an output, the corresponding GPIO_INT_EN bit in GPIO_INT register must be cleared.
    Otherwise, you may see an interrupt get "looped back" to the internal logic.
    Address Offsets: 10h, 30h, 50h, 70h
*/
#define PUMA6_GPIO_INT_EN_REG                           (AVALANCHE_GPIO_BASE + 0x10)
/*
    PUMA6_GPIO_INT_MODELE:
    GPIO Interrupt Control, Bits in this register allow the selection of a level or an edge trigger for the interrupt. 
    0 - Level detection
    1 - Edge detection
    Address Offset: 14h, 34h, 54h, 74h 
*/
#define PUMA6_GPIO_INT_MODELE_REG                       (AVALANCHE_GPIO_BASE + 0x14)
/*
    PUMA6_GPIO_INT_MODERF:
    GPIO Interrupt Control, Bits in this register allow the selection of either rising or falling edge (or level) modes
    0 - rising edge
    1 - falling edge
    Address Offset: 18h, 38h, 58h, 78h 
*/
#define PUMA6_GPIO_INT_MODERF_REG                       (AVALANCHE_GPIO_BASE + 0x18)
/*
    PUMA6_GPIO_MUXCNTL:
    This register may be read and written and controls the state of port output enables for Smart Card 0 and 1, UART 1 and 2 
    (UART 0 does not share a pin), and the GBE_LINK output pins.  It also provides the PWM trigger source selection.
    See PUB PWM document for more information about the trigger inputs.  Also note that the shared GPIO/PWM signals have the 
    trigger swapped so that an adjacent pwm may act as a trigger or allow an external trigger input.
    For example shared pin GPIO_55_PWM0 may use GPIO_52_PWM3 as a trigger or as an external trigger input.
    Address Offset: 1Ch 
*/
#define PUMA6_GPIO_MUXCNTL_REG                          (AVALANCHE_GPIO_BASE + 0x1C)


/*
    PUMA6_GPIO_CLEAR:
    Writing a '1' to a bit in this register has the effect of clearing the corresponding bit in the GPIO output data register.
    These bits are write only. 
    Address Offsets: 80h, 90h, A0h, B0h
*/
#define PUMA6_GPIO_CLEAR_REG                            (AVALANCHE_GPIO_BASE + 0x80)
/*
    PUMA6_GPIO_SET:
    Writing a '1' to a bit in this register has the effect of setting the corresponding bit in the GPIO output data register. 
    These bits are write only. 
    Address Offsets: 84h, 94h, A4h, B4h
*/
#define PUMA6_GPIO_SET_REG                              (AVALANCHE_GPIO_BASE + 0x84)
/*
    PUMA6_GPIO_POLARITY:
    Writing a '1' to a bit in this register has the effect of inverting the corresponding data input bit. 
    Writing a '0' does not invert the corresponding data input. These bits are read/write.
    Address Offsets: 88h, 98h, A8h, B8h
*/
#define PUMA6_GPIO_POLARITY_REG                         (AVALANCHE_GPIO_BASE + 0x88)
/*
    PUMA6_GPIO_GROUP:
    Controls the routing of the GPIO interrupts to either gpio_int (legacy interrupt), gpio_irq_a, or gpio_irq_b.
    These bits are read/write.
    Address Offset: BCh
*/
#define PUMA6_GPIO_GROUP_REG                            (AVALANCHE_GPIO_BASE + 0xBC)

/****************************************************************************/
/****************************************************************************/
/*                          GPIO Helper macros                              */
/****************************************************************************/
/****************************************************************************/

/* Macro to read GPIO register from base address + offset */
#define PUMA6_GPIO_REG_GET(reg)                   gpio_reg_get_cb(reg)
#define PUMA6_GPIO_REG_SET(reg, val)              gpio_reg_set_cb(reg,val)


/* Defined to perform little endian accesses For ARM11 - If the CPU is running in little endian mode this macro will do nothing ! */
#define PUMA6_GPIO_CONVERT_CPU_TO_32LE(be_value)  (cpu_to_le32(be_value))

/* This Macro will help to calc the GPIO reg for the 0x20 spacing , we have 4 groups of Regs to hand 128 bits of GPIOs */
#define PUMA6_GPIO_REG_CALC_20(gpio_inx,reg)      (((int)((gpio_inx)/32))*0x20 + (reg)) 
/* This Macro will help to calc the GPIO reg for the 0x10 spacing , we have 4 groups of Regs to hand 128 bits of GPIOs */
#define PUMA6_GPIO_REG_CALC_10(gpio_inx,reg)      (((int)((gpio_inx)/32))*0x10 + (reg)) 

#define PUMA6_GPIO_REG_ADDR(gpio_inx,reg) \
    (((reg) > PUMA6_GPIO_MUXCNTL_REG)  ?  \
        /* The spacing is 0x10 between the 4 Regs */  \
        PUMA6_GPIO_REG_CALC_10((gpio_inx),(reg)) \
      : \
        /* The spacing is 0x20 between the 4 Regs */  \
        PUMA6_GPIO_REG_CALC_20((gpio_inx),(reg)) \
     )

/* To open debug/info prints use this macro */     
// #define GPIO_DEBUG_OUTPUT_ON
// #define GPIO_INFO_OUTPUT_ON

/* Debug */
#ifdef  GPIO_DEBUG_OUTPUT_ON
	#define GPIO_DEBUG_OUTPUT(fmt, args...) printk("Puma6 GPIO Debug %s: " fmt, __FUNCTION__ , ## args)
#else
	#define GPIO_DEBUG_OUTPUT(fmt, args...)
#endif

/* Info */
#ifdef  GPIO_INFO_OUTPUT_ON
	#define GPIO_INFO_OUTPUT(fmt, args...) printk("Puma6 GPIO Info %s: " fmt, __FUNCTION__ , ## args)
#else	
	#define GPIO_INFO_OUTPUT(fmt, args...)
#endif	



static unsigned int gpio_reg_get_default (unsigned int addr)
{
	printk(KERN_ERR "GPIO: driver is not initialized. gpio_reg_get_default return 0\n"); 
    return 0;
}

static void gpio_reg_set_default(unsigned int addr,unsigned int data)
{
    printk(KERN_ERR "GPIO: driver is not initialized. gpio_reg_set_default ignored\n"); 
    return ;
}
static unsigned int gpio_reg_get_direct (unsigned int addr)
{
    return (*((volatile unsigned int *)(addr)));
}

static void gpio_reg_set_direct (unsigned int addr,unsigned int data)
{
    ((*((volatile unsigned int *)(addr))) = (data));
    return ;
}
static unsigned int gpio_reg_get_aep (unsigned int addr)
{
    unsigned int value = 0;
    int res = 0;

    res = aep_ipc_gpio_read(addr - AVALANCHE_GPIO_BASE ,&value);
    if (res == 0)
    {
        // printk("[AEP-GPIO] Read  0x%.8X, get 0x%.8X\n",addr,value); 
        return value;
    }

    /* Error */
    printk(KERN_ERR "GPIO: Error - Read from AEP address=0x%0.8X failed (res=%d)\n",addr,res);
    return 0;
}

static void gpio_reg_set_aep (unsigned int addr,unsigned int data)
{
    //printk("[AEP-GPIO] write 0x%.8X, set 0x%.8X\n",addr,data);
    if (aep_ipc_gpio_write(addr - AVALANCHE_GPIO_BASE, data, 0xFFFFFFFF) == 0)
        return;

    /* Error */
    printk(KERN_ERR "GPIO: Error- Write to AEP address=0x%.8X, value=0x%.8X failed\n",addr,data);
    return;
}


/****************************************************************************/
/****************************************************************************/
/*                                GPIO     API                              */
/****************************************************************************/
/****************************************************************************/

/*
    Important Note for Puma6 SoC:
    - The CEFDK will be responsible for configuring the ATOM GPIOs.
    - The U-Boot will be responsible for configuring the ARM11 GPIOs.
    - After ARM11 is out of reset, the ATOM can only use the following GPIO regs:
        GPIO_CLEARn—GPIO Data Output Registers Clear (GPIOs 0-127).
        GPIO_SETn—GPIO Data Output Registers Set (GPIOs 0-127).
    - In ARM11, only U-Boot will use all the GPIO regs to configure the ARM11 GPIOs.
    - ATOM and ARM11 kernels will only use GPIO_CLEARn and GPIO_SETn regs.
*/ 

/*! \fn INT32 PAL_sysGpioCtrlSetDir(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction)
    \brief This API initializes the mode of a specific GPIO pin
    \param gpio_pin GPIO pin number
    \param pin_direction Direction of the gpio pin in GPIO_PIN mode
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioCtrlSetDir(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction)
{
    UINT32 gpio_out_en_reg;

    GPIO_DEBUG_OUTPUT("Puma6 debug %s gpio_pin=%d pin_direction=%d\n",__FUNCTION__,gpio_pin ,pin_direction);

    gpio_out_en_reg = PUMA6_GPIO_REG_ADDR(gpio_pin, PUMA6_GPIO_OUT_EN_REG);

    /* Take the HW mutex */
    if (hw_mutex_lock_interruptible(HW_MUTEX_GPIO) != 0)
    {
        printk("GPIO - Can't lock HW mutex\n");
        return GPIO_FAIL;
    }

    GPIO_DEBUG_OUTPUT("Puma6 debug %s gpio_out_en_reg = 0x%x gpio_out_en_reg val before = 0x%x \n",__FUNCTION__,gpio_out_en_reg,(PUMA6_GPIO_REG_GET(gpio_out_en_reg)));

    if (pin_direction == GPIO_OUTPUT_PIN )
    {
        PUMA6_GPIO_REG_SET(gpio_out_en_reg, (PUMA6_GPIO_REG_GET(gpio_out_en_reg) | PUMA6_GPIO_CONVERT_CPU_TO_32LE(BIT(gpio_pin%32))));
    }
    else /* GPIO_INPUT_PIN */
    {
        PUMA6_GPIO_REG_SET(gpio_out_en_reg, (PUMA6_GPIO_REG_GET(gpio_out_en_reg) & PUMA6_GPIO_CONVERT_CPU_TO_32LE(~BIT(gpio_pin%32))));
    }

    GPIO_DEBUG_OUTPUT("Puma6 debug %s gpio_out_en_reg = 0x%x gpio_out_en_reg val after = 0x%x \n",__FUNCTION__,gpio_out_en_reg, (PUMA6_GPIO_REG_GET(gpio_out_en_reg)));

    /* Release the HW Mutex */
    hw_mutex_unlock(HW_MUTEX_GPIO);		

    return(GPIO_OK);
}
EXPORT_SYMBOL(PAL_sysGpioCtrlSetDir);



/*! \fn PAL_SYS_GPIO_PIN_DIRECTION_T PAL_sysGpioCtrlGetDir(UINT32 gpio_pin)
    \brief This API returns the GPIO diraction, whether it's output or input
    \param gpio_pin GPIO pin number
    \return Returns (0) for GPIO_OUTPUT_PIN and (1) for GPIO_INPUT_PIN
*/
PAL_SYS_GPIO_PIN_DIRECTION_T PAL_sysGpioCtrlGetDir(UINT32 gpio_pin)
{
    UINT32 gpio_out_en_reg;

    GPIO_INFO_OUTPUT("gpio_pin=%d\n",gpio_pin);

    gpio_out_en_reg = PUMA6_GPIO_REG_ADDR(gpio_pin, PUMA6_GPIO_OUT_EN_REG);

    GPIO_DEBUG_OUTPUT("gpio_out_en_reg = 0x%x gpio_out_en_reg val = 0x%x \n",gpio_out_en_reg,(PUMA6_GPIO_REG_GET(gpio_out_en_reg)));

    if ( PUMA6_GPIO_REG_GET(gpio_out_en_reg) & PUMA6_GPIO_CONVERT_CPU_TO_32LE(BIT(gpio_pin%32)) )
    {
        GPIO_INFO_OUTPUT("gpio_pin=%d is output\n",gpio_pin);
        return (GPIO_OUTPUT_PIN);
    }
    else
    {
        GPIO_INFO_OUTPUT("gpio_pin=%d is input\n",gpio_pin);
        return (GPIO_INPUT_PIN);
    }
}

EXPORT_SYMBOL(PAL_sysGpioCtrlGetDir);


/*! \fn INT32 PAL_sysGpioOutBit(UINT32 gpio_pin, INT32 value) 
    \brief This API outputs the specified value on the gpio pin
    \param gpio_pin GPIO pin number
    \param value 0/1 (TRUE/FALSE)
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioOutBit(UINT32 gpio_pin, INT32 value)
{
    UINT32 gpio_out_reg;

    GPIO_INFO_OUTPUT("gpio_pin=%d value=%d \n",gpio_pin,value);

    if ( gpio_pin >= PUMA6_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }

    if ( value ) /* SET Reg */
    {
        gpio_out_reg = PUMA6_GPIO_REG_ADDR(gpio_pin, PUMA6_GPIO_SET_REG);
    }
    else /* CLEAR Reg */
    {
        gpio_out_reg = PUMA6_GPIO_REG_ADDR(gpio_pin, PUMA6_GPIO_CLEAR_REG);
    }

    GPIO_DEBUG_OUTPUT("gpio_out_reg = 0x%x \n",gpio_out_reg);

    PUMA6_GPIO_REG_SET(gpio_out_reg, PUMA6_GPIO_CONVERT_CPU_TO_32LE(BIT(gpio_pin%32))); 

    return(GPIO_OK);
}

EXPORT_SYMBOL(PAL_sysGpioOutBit);


/*! \fn INT32 PAL_sysGpioInBit(UINT32 gpio_pin)
    \brief This API reads the specified gpio_pin and returns the value
    \param gpio_pin GPIO pin number
    \return Returns gpio status 0/1 (TRUE/FALSE), on failure returns (GPIO_FAIL)
*/
INT32 PAL_sysGpioInBit(UINT32 gpio_pin)
{
    UINT32 gpio_in_reg;
    INT32 ret_val = 0;

    GPIO_INFO_OUTPUT("gpio_pin=%d \n",gpio_pin);

    if ( gpio_pin >= PUMA6_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }

    gpio_in_reg = PUMA6_GPIO_REG_ADDR(gpio_pin,PUMA6_GPIO_INPUT_REG);

    GPIO_DEBUG_OUTPUT("gpio_in_reg = 0x%x  gpio_in_reg val = 0x%x \n",gpio_in_reg, PUMA6_GPIO_REG_GET(gpio_in_reg));

    if ( ((PUMA6_GPIO_REG_GET(gpio_in_reg)) & (PUMA6_GPIO_CONVERT_CPU_TO_32LE(BIT(gpio_pin%32)))) )
    {
        ret_val = 1;
    }

    return(ret_val);
}

/**************************************************************************/
/*! \fn static int gpio_drv_open(struct inode *inode, struct file *filp)
 **************************************************************************
 *  \brief This function is opens the gpio device.
 *  \param struct inode *inode - device node pointer
 *  \param struct file *filp - device file pointer
 *  \return int - GPIO_OK on correcet access type  otherwise  GPIO_FAIL.
 **************************************************************************/
static int gpio_drv_open(struct inode *inode, struct file *filp)
{

        printk(KERN_INFO "gpio_drv_open: ref %d\n", ++ref);
        return GPIO_OK;
}

/**************************************************************************/
/*! \fn static int gpio_drv_close(struct inode *inode, struct file *filp)
 **************************************************************************
 *  \brief This function is closes the gpio device.
 *  \param struct inode *inode - device node pointer
 *  \param struct file *filp - device file pointer
 *  \return int - GPIO_OK on correcet access type  otherwise  GPIO_FAIL.
 **************************************************************************/
static int gpio_drv_close(struct inode *inode, struct file *filp)
{
        printk(KERN_INFO "gpio_drv_close: ref %d\n", --ref);
        return GPIO_OK;
}



/**************************************************************************/
/*! \fn static long gpio_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
 **************************************************************************
 *  \brief This function is activate the the gpio device requests.
 *  \param struct file *filp - the device file pointer
 *  \param unsigned int cmd - the command to be performed
 *  \param unsigned long arg - pointer to the user request
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static long gpio_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct gpio_user_info  gpio_info;
    int ret = 0;

    /* Check for valid pointer to the parameter list */
    if (0 == arg)
    {
        printk(KERN_ERR "gpio arg == 0\n");
        return -EINVAL;
    }
    if (copy_from_user(&gpio_info, (void __user *)arg, sizeof(struct gpio_user_info)))
    {
        printk(KERN_ERR "gpio copy from user failed\n");
        return -EFAULT;
        /* Execute ioctl request */
    }
        
    switch (cmd)
    {
        /*---------------------------------------------------------------------------*/
    case GPIO_SET_DIRECTION_CMD:
            printk(KERN_DEBUG "PAL_sysGpioCtrlSetDir - gpio_pin=%x, pin_direction=%x\n",gpio_info.gpio_pin,gpio_info.pin_direction);
            if (PAL_sysGpioCtrlSetDir(gpio_info.gpio_pin, (PAL_SYS_GPIO_PIN_DIRECTION_T) gpio_info.pin_direction) == GPIO_FAIL)
            {
                ret = -ENOSYS;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case GPIO_GET_DIRECTION_CMD:
            printk(KERN_DEBUG "PAL_sysGpioCtrlGetDir - gpio_pin=%x\n",gpio_info.gpio_pin);
            gpio_info.value = PAL_sysGpioCtrlGetDir(gpio_info.gpio_pin);
            if (copy_to_user((void __user *)arg, &gpio_info, sizeof(struct gpio_user_info)))
            {
                ret = -EFAULT;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case GPIO_OUT_BIT_CMD:
            printk(KERN_DEBUG "PAL_sysGpioOutBit - gpio_pin=%x, value=%x\n",gpio_info.gpio_pin,gpio_info.value);
            if (PAL_sysGpioOutBit(gpio_info.gpio_pin, gpio_info.value) == GPIO_FAIL)
            {
                ret = -ENOSYS;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case GPIO_IN_BIT_CMD:
            printk(KERN_DEBUG "PAL_sysGpioInBit - gpio_pin=%x\n",gpio_info.gpio_pin);
            if ((gpio_info.value = PAL_sysGpioInBit(gpio_info.gpio_pin)) == GPIO_FAIL)
            {
                ret = -ENOSYS;
            }
            else
            {
                if (copy_to_user((void __user *)arg, &gpio_info, sizeof(struct gpio_user_info)))
                {
                    ret = -EFAULT;
                }
            }
            break;
        /*---------------------------------------------------------------------------*/
    default:
            printk(KERN_ERR "iosfsb no legal command given\n");
            ret = -ENOSYS;
            break;
        }
    return ret;
}


/* Structure to map driver functions to kernel */
struct file_operations gpio_drv_fops = {
        .owner   = THIS_MODULE,
        .unlocked_ioctl   = gpio_unlocked_ioctl,
        .open    = gpio_drv_open,
        .release = gpio_drv_close,
};

static int gpio_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, readGpioProc, NULL); 
}

static const struct file_operations gpio_proc_fops = {
    .open           = gpio_proc_open,
    .read           = seq_read,
    .llseek         = seq_lseek,
    .release        = single_release,
};

/**************************************************************************/
/*! \fn static int __init gpio_drv_init(void)
 **************************************************************************
 *  \brief This function is the gpio device module init function.
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static int __init gpio_drv_init(void)
{
    printk(KERN_INFO "GPIO: Intel(R) GPIO driver built on %s @ %s\n", __DATE__, __TIME__);

    if (aep_ipc_gpio_is_active() == 0)
    {
        /* AEP is not active - use direct registers access */
        gpio_reg_set_cb = gpio_reg_set_direct;
        gpio_reg_get_cb = gpio_reg_get_direct;
    }
    else
    {
        /* AEP is active - use aep registers access */
        gpio_reg_set_cb = gpio_reg_set_aep;
        gpio_reg_get_cb = gpio_reg_get_aep;
		printk(KERN_INFO "GPIO: AEP mode.\n");
    }

    if (alloc_chrdev_region(&gpio_dev_t, 0, count, DEV_NAME) < 0)
    { /*count indicates how many minors we get*/
        printk(KERN_ERR "\nGPIO Failed to register character device region %s\n",DEV_NAME);
        return GPIO_FAIL;
    }

    if (!(gpio_cdev = cdev_alloc()))
    {
        printk(KERN_ERR "\nGPIO Failed in cdev_alloc %s\n",DEV_NAME);
        unregister_chrdev_region(gpio_dev_t, count);
        return GPIO_FAIL;
    }
    /* Connect the file operations with the cdev */
    cdev_init(gpio_cdev, &gpio_drv_fops);
    /* Connect the major/minor number to the cdev  - Activates the device*/
    if (cdev_add(gpio_cdev, gpio_dev_t, count) < 0)
    {
        printk(KERN_ERR "\nGPIO Failed in add character device %s\n",DEV_NAME);
        cdev_del(gpio_cdev);
        unregister_chrdev_region(gpio_dev_t, count);
        return GPIO_FAIL;
    }
    /* connection to the udev *******************************************************************/
    /* ceates a class directory under /sys/class */
    gpio_udev_class = class_create(THIS_MODULE, "gpio_class");
    /* ceates a class directory under /sys/class/DEV_NAME named DEV_NAME  */
    /* creates 3 file: dev, uevent, subsystem*/
    device_create(gpio_udev_class, NULL, gpio_dev_t, NULL, "%s", "docsis_gpio_dev");

    printk(KERN_INFO "GPIO: Create device: /dev/%s\n",DEV_NAME);

	/* Creating proc file */
	procFile = proc_create(GPIO_PROC_NAME, S_IFREG | S_IRUGO, NULL, &gpio_proc_fops);
	
	if (procFile == NULL)
	{
		remove_proc_entry(GPIO_PROC_NAME, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",GPIO_PROC_NAME);
		return -ENOMEM;
	}

    printk(KERN_INFO "GPIO: Add proc file: /proc/%s\n", GPIO_PROC_NAME);

    return GPIO_OK;

}

static int readGpioProc(struct seq_file *m, void *v)
{
	int gpioNum;
	int gpioValue;
	int gpioDirection;
    
	seq_printf(m, "\n GPIO | Dir | Val\n------|-----|-----\n");

	for (gpioNum = 0; gpioNum < PUMA6_MAX_GPIOS; ++gpioNum)
	{
		gpioDirection = PAL_sysGpioCtrlGetDir(gpioNum);
		gpioValue     = PAL_sysGpioInBit(gpioNum);
		
		seq_printf(m, "  %3d | %s | %d\n", gpioNum, (gpioDirection == GPIO_OUTPUT_PIN) ? DIR_OUT : DIR_IN, gpioValue);
    }

    return 0;
}

/**************************************************************************/
/*! \fn static void __exit gpio_drv_exit(void)
 **************************************************************************
 *  \brief This function is the gpio device module exit function.
 **************************************************************************/
static void __exit gpio_drv_exit(void)
{
    if (gpio_cdev)
        cdev_del(gpio_cdev);
    unregister_chrdev_region(gpio_dev_t, count);
    printk(KERN_INFO "\nGPIO device unregistered\n");


    device_destroy(gpio_udev_class, gpio_dev_t);
    class_destroy(gpio_udev_class);
}
/*************************************************************************************/

module_init(gpio_drv_init);
module_exit(gpio_drv_exit);


EXPORT_SYMBOL(PAL_sysGpioInBit);
