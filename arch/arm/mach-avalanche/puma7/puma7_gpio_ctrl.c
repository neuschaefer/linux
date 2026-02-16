/*
 *  puma7_gpio_ctrl
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015-2017 Intel Corporation. All rights reserved.
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

/** \file   puma7_gpio_ctrl.c
 *  \brief  GPIO config control APIs. 
 *          
 *  \author     Intel
 *
 *  \version    0.1     Created
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
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <pal.h> 
#include <hw_mutex_ctrl.h> 
#include "puma7_gpio_ctrl.h"
#include "puma7_io_ctrl.h"

/****************************************************************************/
/****************************************************************************/
/*                          GPIO local definitions                          */
/****************************************************************************/
/****************************************************************************/

static int readGpioProc(struct seq_file *m, void *v);

static dev_t gpio_dev_t;
static unsigned int count = 1;
static unsigned int ref = 0;
static struct cdev *gpio_cdev;
static struct class *gpio_udev_class;
static struct proc_dir_entry *procFile;

/****************************************************************************
    GPIO Unit Register Map
Offset      Symbol              bits    Type    Short Description
00h         GPIO_REVID          31:0    r/o
04h         GPIO_DATAIN         31:0    r/o     Data Input value 
08h         GPIO_DATAIN_SET     31:0    W       Set Data Input value 
0ch         GPIO_DATAIN_CLR     31:0    W       Clear Data Input value
10h         GPIO_DATAOUT        31:0    R       Data Output value 
14h         GPIO_DATAOUT_SET    31:0    W       Set Data Output value 
18h         GPIO_DATAOUT_CLR    31:0    W       Clear Data Output value 
1ch         GPIO_OE             31:0    R       Pin Direction value 
20h         GPIO_OE_SET         31:0    W       Set Pin Direction value 
24h         GPIO_OE_CLR         31:0    W       Cleat Pin Direction value 
28h         GPIO_EN             31:0    R       Pin Status 
2ch         GPIO_EN_SET         31:0    W       Set Pin Status 
30h         GPIO_EN_CLR         31:0    W       Clear Pin Status 
34h         GPIO_POL            31:0    R       Pin Polarity 
38h         GPIO_POL_SET        31:0    W       Set Pin Polarity
3ch         GPIO_POL_CLR        31:0    W       Clear Pin Polarity 
40h         GPIO_CNT_LATCH      31:0    R       Counter Latch Value 
44h         GPIO_SEL_PIN        31:0    R/W     Reset & Define Pin for Latch
 
   
*****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/*                          GPIO Defines                                    */
/****************************************************************************/

#define DEV_NAME          "docsis_gpio_dev"
#define GPIO_OK           (0)
#define GPIO_FAIL         (-1)
#define DIR_OUT           "Out"
#define DIR_IN            "In "
#define AVB_COUNTER_RESET_BIT 5

/***************************************
* GPIO_REVID:                          *
***************************************/
#define GPIO_REVID      (AVALANCHE_GPIO_BASE + 0x0)

/***************************************
* GPIO_DATAIN:                         *
*  Read data that wes received incase  *
*  the GPIO is configured to input     *
*  Read data that was sent out incase  *
*  the GPIO is configured to output    *
* (loopback)                           *
***************************************/
#define GPIO_DATAIN     (AVALANCHE_GPIO_BASE + 0x4)

/***************************************
* GPIO_DATAIN_SET:                     *
*  Writing 0x1 to specific bit in this *
*  register, is setting the            *
*  corresponding bit in the Data       *
*  in register                         *
***************************************/
#define GPIO_DATAIN_SET (AVALANCHE_GPIO_BASE + 0x8)

/***************************************
* GPIO_DATAIN_SET:                     *
*  Writing 0x1 to specific bit in this *
*  register, is clear the              *
*  corresponding bit in the Data       *
*  in register                         *
***************************************/
#define GPIO_DATAIN_CLR (AVALANCHE_GPIO_BASE + 0xc)

/***************************************
* GPIO_DATAOUT:                        *
*  Read the data that was requested by *
*  the set/ clr register               *
*  (Verification)                      *
***************************************/
#define GPIO_DATAOUT    (AVALANCHE_GPIO_BASE + 0x10)

/***************************************
* GPIO_DATAOUT_SET:                    *
*  Writing 0x1 to specific bit in this *
*  register is setting the             *
*  coresponding bit in the data out    *
*  register                            *
***************************************/
#define GPIO_DATAOUT_SET (AVALANCHE_GPIO_BASE + 0x14)


/***************************************
* GPIO_DATAOUT_SET:                    *
*  Writing 0x1 to specific bit in this *
*  register is setting the             *
*  coresponding bit in the data out    *
*  register                            *
***************************************/
#define GPIO_DATAOUT_SET (AVALANCHE_GPIO_BASE + 0x14)

/***************************************
* GPIO_DATAOUT_CLR:                    *
*  Writing 0x1 to specific bit in this *
*  register is clearing the            *
*  coresponding bit in the data out    *
*  register                            *
***************************************/
#define GPIO_DATAOUT_CLR (AVALANCHE_GPIO_BASE + 0x18)

/***************************************
* GPIO_OEN:                            *
*  Pin direction value:                *
*  0 - input                           *
*  1 - output                          *
***************************************/
#define GPIO_OE          (AVALANCHE_GPIO_BASE + 0x1c)

/***************************************
* GPIO_OEN_SET:                        *
*  Writing 0x1 to specific bit in the  *
*  this register, set the coresponding *
*  GPIO for output                     *
***************************************/
#define GPIO_OE_SET      (AVALANCHE_GPIO_BASE + 0x20)

/***************************************
* GPIO_OEN_CLR:                        *
*  Writing 0x1 to specific bit in the  *
*  this register, set the coresponding *
*  GPIO for input                      *
***************************************/
#define GPIO_OE_CLR      (AVALANCHE_GPIO_BASE + 0x24)

/***************************************
* GPIO_EN:                             *
*  Pin status:                         *
*  0 - disable (value is set to '0'    *
*  for input and output                *
*  1- eanble                           *
***************************************/
#define GPIO_EN          (AVALANCHE_GPIO_BASE + 0x28)

/***************************************
* GPIO_EN_SET:                         *
*  Writing 0x1 to specific bit in this *
*  register is enabling the            *
*  corespinding GPIO                   *
***************************************/
#define GPIO_EN_SET      (AVALANCHE_GPIO_BASE + 0x2c)

/***************************************
* GPIO_EN_CLR:                         *
*  Writing 0x1 to specific bit in this *
*  register is disabling the           *
*  corespinding GPIO                   *
***************************************/
#define GPIO_EN_CLR      (AVALANCHE_GPIO_BASE + 0x30)

/***************************************
* GPIO_POL:                            *
* Pin polarity indication:             *
* 0 - Active high (send 1 to eanble)   *
* 1 - Active low (send 0 to enable)    *
***************************************/
#define GPIO_POL         (AVALANCHE_GPIO_BASE + 0x34)

/***************************************
* GPIO_POL_SET:                        *
* Writing 0x1 to specific bit in the   *
* register, is setting the             *
* corespinding GPIO for Active high    *
***************************************/
#define GPIO_POL_SET     (AVALANCHE_GPIO_BASE + 0x38)

/***************************************
* GPIO_POL_CLR:                        *
* Writing 0x1 to specific bit in the   *
* register, is setting the             *
* corespinding GPIO for Active low     *
***************************************/
#define GPIO_POL_CLR     (AVALANCHE_GPIO_BASE + 0x3c)

/***************************************
* GPIO_CNT_LATCH:                      *
* Reflect the internal GPIO counter    *
* value. the counter value is latched  *
* to the register upon AVb snapshot    *
* indication                           *
***************************************/
#define  GPIO_CNT_LATCH  (AVALANCHE_GPIO_BASE + 0x40)

/***************************************
* GPIO_SEL_PIN:                        *
* 31:6 Reserved                        *
* 5    AVB counter reset               *
* 4:0  Define which PGIO in is         *
*      connected to the AVB counter    *
***************************************/
#define  GPIO_SEL_PIN   (AVALANCHE_GPIO_BASE + 0x44)

/****************************************************************************/
/****************************************************************************/
/*                          GPIO Helper macros                              */
/****************************************************************************/
/****************************************************************************/

/* Macro to read GPIO register from base address + offset */
#define PUMA7_GPIO_REG_GET(reg)                   (*((volatile unsigned int *)(reg)))
#define PUMA7_GPIO_REG_SET(reg, val)              ((*((volatile unsigned int *)(reg))) = (val))

/* To open debug/info prints use this macro */     
// #define GPIO_DEBUG_OUTPUT_ON
// #define GPIO_INFO_OUTPUT_ON

/* Debug */
#ifdef  GPIO_DEBUG_OUTPUT_ON
	#define GPIO_DEBUG_OUTPUT(fmt, args...) printk("Puma7 GPIO Debug %s: " fmt, __FUNCTION__ , ## args)
#else
	#define GPIO_DEBUG_OUTPUT(fmt, args...)
#endif

/* Info */
#ifdef  GPIO_INFO_OUTPUT_ON
	#define GPIO_INFO_OUTPUT(fmt, args...) printk("Puma7 GPIO Info %s: " fmt, __FUNCTION__ , ## args)
#else	
	#define GPIO_INFO_OUTPUT(fmt, args...)
#endif	

/****************************************************************************/
/****************************************************************************/
/*                                GPIO     API                              */
/****************************************************************************/
/****************************************************************************/

/*! \fn INT32 PAL_sysGpioCtrlSetDir(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction)
    \brief This API initializes the mode of a specific GPIO pin
    \param gpio_pin GPIO pin number
    \param pin_direction Direction of the gpio pin in GPIO_PIN mode
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioCtrlSetDir(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction)
{

    GPIO_DEBUG_OUTPUT("Request GPIO Pin = %d change direction to %s\n",gpio_pin ,(pin_direction == GPIO_OUTPUT_PIN)?"output":"input");

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }

    if (pin_direction == GPIO_OUTPUT_PIN )
    {
        GPIO_DEBUG_OUTPUT("GPIO reg = 0x%x, val before = 0x%x \n",GPIO_OE,(PUMA7_GPIO_REG_GET(GPIO_OE)));
        PUMA7_GPIO_REG_SET(GPIO_OE_SET, BIT(gpio_pin));
    }
    else /* GPIO_INPUT_PIN */
    {
        GPIO_DEBUG_OUTPUT("GPIO reg = 0x%x, val before = 0x%x \n",GPIO_OE,(PUMA7_GPIO_REG_GET(GPIO_OE)));
        PUMA7_GPIO_REG_SET(GPIO_OE_CLR, BIT(gpio_pin));
    }

    GPIO_DEBUG_OUTPUT("Val after = 0x%x \n", (PUMA7_GPIO_REG_GET(GPIO_OE)));

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
    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }
    if (PUMA7_GPIO_REG_GET(GPIO_OE) & BIT(gpio_pin))
    {
        GPIO_INFO_OUTPUT("GPIO Pin = %d is output\n",gpio_pin);
        return (GPIO_OUTPUT_PIN);
    }
    else
    {
        GPIO_INFO_OUTPUT("GPIO Pin = %d is input\n",gpio_pin);
        return (GPIO_INPUT_PIN);
    }
}

EXPORT_SYMBOL(PAL_sysGpioCtrlGetDir);


/*! \fn INT32 PAL_sysGpioOutBit(UINT32 gpio_pin, INT32 value) 
    \brief This API outputs the specified value on the gpio pin
    \param gpio_pin GPIO pin number
    \param value 0/1 (CLEAR/SET)
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioOutBit(UINT32 gpio_pin, INT32 value)
{

    GPIO_INFO_OUTPUT("GPIO Pin = %d, Value = %d \n",gpio_pin,value);

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }
    if ( value  ) /* SET Reg */
    {
        PUMA7_GPIO_REG_SET(GPIO_DATAOUT_SET, BIT(gpio_pin));
    }
    else /* CLEAR Reg */
    {
        PUMA7_GPIO_REG_SET(GPIO_DATAOUT_CLR, BIT(gpio_pin));
    }

    GPIO_DEBUG_OUTPUT("GPIO data out reg 0x%x value is %x \n",(GPIO_DATAOUT),PUMA7_GPIO_REG_GET(GPIO_DATAOUT));

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
    GPIO_INFO_OUTPUT("GPIO pin=%d \n",gpio_pin);

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }

    if (PUMA7_GPIO_REG_GET(GPIO_DATAIN) & BIT(gpio_pin))
    {
        GPIO_DEBUG_OUTPUT("GPIO pin=%d input data = 1 \n",gpio_pin);
        return 1;
    }
    GPIO_DEBUG_OUTPUT("GPIO pin=%d input data = 0 \n",gpio_pin);
    return 0;
}
EXPORT_SYMBOL(PAL_sysGpioInBit);

/*! \fn INT32 PAL_sysGpioSetStatus(UINT32 gpio_pin, INT32 value) 
    \brief This function is disable / enable the requested GPIO pin
    \param gpio_pin GPIO pin number
    \param Status (0/1) Disable /Enable.
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioSetStatus(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_STATUS_T status)
{
    GPIO_INFO_OUTPUT("Request to %s GPIO pin=%d \n",(status)?"Eanble":"Disable",gpio_pin);

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }
    if ( status  ) /* Enable */
    {
        PUMA7_GPIO_REG_SET(GPIO_EN_SET, BIT(gpio_pin));
    }
    else /* Disable */
    {
        PUMA7_GPIO_REG_SET(GPIO_EN_CLR, BIT(gpio_pin));
    }

    GPIO_DEBUG_OUTPUT("GPIO enable reg 0x%x value is %d \n",GPIO_EN ,PUMA7_GPIO_REG_GET(GPIO_EN));
}
EXPORT_SYMBOL(PAL_sysGpioSetStatus);

/*! \fn INT32 PAL_sysGpioGetStatus(UINT32 gpio_pin) 
    \brief This function get the requested GPIO pin status
    \param gpio_pin GPIO pin number
    \return Return (0/1) Disable /Enable.
*/
INT32 PAL_sysGpioGetStatus(UINT32 gpio_pin)
{
    PAL_SYS_GPIO_PIN_STATUS_T pinStatus;
    GPIO_INFO_OUTPUT("Request to get GPIO pin=%d status\n",gpio_pin);

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }
    pinStatus =  PUMA7_GPIO_REG_GET(GPIO_EN) & BIT(gpio_pin);
    GPIO_DEBUG_OUTPUT("GPIO enable reg 0x%x value is %d \n",GPIO_EN ,pinStatus);
    return pinStatus;
}
EXPORT_SYMBOL(PAL_sysGpioGetStatus);

/*! \fn INT32 PAL_sysGpioSetPolarity(UINT32 gpio_pin, INT32 value) 
    \brief This function set requested GPIO polarity
    \param gpio_pin GPIO pin number
    \param polarity 0/1 (High/ Low).
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioSetPolarity(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_POLARITY_T polarity)
{
    GPIO_INFO_OUTPUT("Request to set polarity to %s for GPIO pin=%d \n",(status)?"High":"Low",gpio_pin);

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }
    if ( polarity  ) /* Enable */
    {
        PUMA7_GPIO_REG_SET(GPIO_POL_SET, BIT(gpio_pin));
    }
    else /* Disable */
    {
        PUMA7_GPIO_REG_SET(GPIO_POL_CLR, BIT(gpio_pin));
    }

    GPIO_DEBUG_OUTPUT("GPIO polarity reg 0x%x value is %d \n",GPIO_EN ,PUMA7_GPIO_REG_GET(GPIO_POL));

    return GPIO_OK;
}
EXPORT_SYMBOL(PAL_sysGpioSetPolarity);

/*! \fn INT32 PAL_sysGpioGetPolarity(UINT32 gpio_pin) 
    \brief This function return indication if the corresponding 
           GPIO is set to Active high of Active low
    \param gpio_pin GPIO pin number
    \return Return (0/1) Active high/ Active low
*/
INT32 PAL_sysGpioGetPolarity(UINT32 gpio_pin)
{
    INT32 pinStatus;
    GPIO_INFO_OUTPUT("Request to get GPIO pin=%d status\n",gpio_pin);

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }
    pinStatus =  PUMA7_GPIO_REG_GET(GPIO_POL) & BIT(gpio_pin);
    GPIO_DEBUG_OUTPUT("GPIO data out reg 0x%x value is %d \n",GPIO_POL ,pinStatus);
    return pinStatus;
}
EXPORT_SYMBOL(PAL_sysGpioGetPolarity);

/*! \fn INT32 PAL_sysGpioResetAVB(void) 
    \brief This function reset the AVB counter
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioResetAVB(void)
{
    GPIO_INFO_OUTPUT("Request to reset AVB counter\n");

    PUMA7_GPIO_REG_SET(GPIO_SEL_PIN, BIT(AVB_COUNTER_RESET_BIT));

    return GPIO_OK;
}
EXPORT_SYMBOL(PAL_sysGpioResetAVB);

/*! \fn INT32 PAL_sysGpioAVBconfig(UINT32 gpio_pin) 
    \brief This function defines which GPIO in is connected
           to the AVB counter
    \param gpio_pin GPIO pin number
    \return Returns (GPIO_OK) on success and (GPIO_FAIL) on failure
*/
INT32 PAL_sysGpioAVBconfig(UINT32 gpio_pin)
{
    GPIO_INFO_OUTPUT("Request to define GPIO pin=%d  as the AVB counter\n",gpio_pin);

    if ( gpio_pin >= PUMA7_MAX_GPIOS )
    {
        return(GPIO_FAIL);
    }

    PUMA7_GPIO_REG_SET(GPIO_SEL_PIN, gpio_pin);


    GPIO_DEBUG_OUTPUT("GPIO select pin reg 0x%x value is %d \n",GPIO_SEL_PIN ,PUMA7_GPIO_REG_GET(GPIO_SEL_PIN));

    return GPIO_OK;
}
EXPORT_SYMBOL(PAL_sysGpioAVBconfig);

/*! \fn INT32 PAL_sysGpioGetAVBcounterValue(void)
    \brief This function return the AVB counter value
    \return Return AVB counter value
*/
INT32 PAL_sysGpioGetAVBcounterValue(void)
{
    INT32 pinValue;
    GPIO_INFO_OUTPUT("Request to get AVB counter value\n");

    pinValue =  PUMA7_GPIO_REG_GET(GPIO_CNT_LATCH)
    GPIO_DEBUG_OUTPUT("GPIO AVB count reg 0x%x value is %d \n",GPIO_CNT_LATCH ,pinValue);
    return pinValue;
}
EXPORT_SYMBOL(PAL_sysGpioGetAVBcounterValue);

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
    struct io_ctrl_user_info io_ctrl_info;
    int ret = 0;

    /* Check for valid pointer to the parameter list */
    if (0 == arg)
    {
        printk(KERN_ERR "[%s] Arg == 0\n",__FUNCTION__);
        return -EINVAL;
    }
    if (cmd == GPIO_CTRL_IO_PAD)
    {        
        if (copy_from_user(&io_ctrl_info, (void __user *)arg, sizeof(struct io_ctrl_user_info)))
        {
            printk(KERN_ERR "[%s] Copy from user the IO ctrl data failed\n",__FUNCTION__);
            return -EFAULT;
            /* Execute ioctl request */
        }
    }
    else
    {
        if (copy_from_user(&gpio_info, (void __user *)arg, sizeof(struct gpio_user_info)))   
        {
            printk(KERN_ERR "[%s] Copy from user the GPIO data failed\n",__FUNCTION__);            
            return -EFAULT;
        }
    }
        
    switch (cmd)
    {
        /*---------------------------------------------------------------------------*/
    case GPIO_SET_DIRECTION_CMD:
            printk(KERN_DEBUG "[%s] Request to set GPIO pin = %d , direction = %d\n",__FUNCTION__,gpio_info.gpio_pin,gpio_info.pin_direction);
            if (PAL_sysGpioCtrlSetDir(gpio_info.gpio_pin, (PAL_SYS_GPIO_PIN_DIRECTION_T) gpio_info.pin_direction) == GPIO_FAIL)
            {
                ret = -ENOSYS;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case GPIO_GET_DIRECTION_CMD:
            printk(KERN_DEBUG "[%s] Request direction for GPIO pin = %d\n",__FUNCTION__,gpio_info.gpio_pin);
            gpio_info.value = PAL_sysGpioCtrlGetDir(gpio_info.gpio_pin);
            if (copy_to_user((void __user *)arg, &gpio_info, sizeof(struct gpio_user_info)))
            {
                ret = -EFAULT;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case GPIO_OUT_BIT_CMD:
            printk(KERN_DEBUG "[%s] Set GPIO pin = %d data out for value = %d\n",__FUNCTION__,gpio_info.gpio_pin,gpio_info.value);
            if (PAL_sysGpioOutBit(gpio_info.gpio_pin, gpio_info.value) == GPIO_FAIL)
            {
                ret = -ENOSYS;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case GPIO_IN_BIT_CMD:
            printk(KERN_DEBUG "[%s] Get GPIO pin = %d data in value\n",__FUNCTION__,gpio_info.gpio_pin);
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
    case GPIO_CTRL_IO_PAD:
            printk(KERN_DEBUG "[%s] Received request for IO ctrl config with opcode = %d, value1 = %d, value2 = %d \n",__FUNCTION__,io_ctrl_info.opcode,io_ctrl_info.value1,io_ctrl_info.value2);
            if (IO_CTRL_configPad(io_ctrl_info.opcode , io_ctrl_info.value1, io_ctrl_info.value2) == GPIO_FAIL )
            {
                ret = -ENOSYS;
            }
            break;
        /*---------------------------------------------------------------------------*/
    default:
            printk(KERN_ERR "[%s] iosfsb no legal command given\n",__FUNCTION__);
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
    printk(KERN_INFO "GPIO: Initializing Intel(R) GPIO driver\n");

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

	for (gpioNum = 0; gpioNum < PUMA7_MAX_GPIOS; ++gpioNum)
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
