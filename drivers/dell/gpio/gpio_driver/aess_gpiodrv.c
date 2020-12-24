/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 On chip GPIO driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSGPIODRV_C


#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "aess_gpiodrv.h"


/* Debugging Message */
#if 0
#define GPIO_DEBUG
#endif

#ifdef GPIO_DEBUG
#define DEBUG_MSG(fmt, args...) printk( "GPIO: " fmt, ## args)
#else
#define DEBUG_MSG(fmt, args...)
#endif

#if 0
#define GPIO_DEBUG2
#endif

#if 0
#define GPIO_DEBUG3
#endif


/* driver name will passed by insmod command */
static char *driver_name="aess_gpiodrv"; 

/* The init flag for aess_gpio_open */
static UINT8 S_u8init_flag = AESSGPIO_NOT_INIT;

/* Linux device data structure */
dev_t gpio_dev;
struct cdev *gpio_cdev;


/* 
	for Z1/Z2 version chip:
	Multiple Function Pin Select Register 1 (MFSEL1)
	Multiple Function Pin Select Register 2 (MFSEL2)
	when GPIO init, we need to set the pin belong to GPIO port 
	(if I/O table set type is GPIO_PORT)
	here is a map; GPIO pin and the bit number in MFSEL1 or MFSEL2
	gpio port is a index, and array record register, bit number and value
	so far, just set it is a GPIO port when do GPIO init 
	if you would like to change to other function, please do it in your driver
*/
sGPIOSelect gpio_select_tableZ1[MAX_GPIO_PORT][MAX_GPIO_PIN] =
{
    /* port 0 */
    {
        /* pin 0-3 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 4-7 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 8-11 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 12-15 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 24, 0}, 
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 24, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 24, 0}, 
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 24, 0}
    }, 
    
    /* port 1 */
    {
        /* pin 16-19 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 23, 0},  
        
        /* pin 20-23 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 24, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 25, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 26, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 27, 0}, 
        
        /* pin 24-27 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 28, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 29, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 2, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 2, 0},  
        
        /* pin 28-31 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 1, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 1, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 0, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 0, 0}
    }, 
    
    /* port 2 */
    {
        /* pin 32-35 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 3, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 4, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 5, 1},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 29, 0}, 
        
        /* pin 36-39 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 28, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0}, 
        
        /* pin 40-43 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 9, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 9, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0}, 
        
        /* pin 44-47 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0}
    }, 
    
    /* port 3 */
    {
        /* pin 48-51 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0}, 
        
        /* pin 52-55 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 11, 0}, 
        
        /* pin 56-59 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 12, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 13, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 13, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 30, 0}, 
        
        /* pin 60-63 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 31, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 10, 0}
    }, 
    
    /* port 4 */
    {
        /* pin 64-67 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 0, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 1, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 2, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 3, 0}, 
        
        /* pin 68-71 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 4, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 5, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 6, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 7, 0}, 
        
        /* pin 72-75 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 8, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 9, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 10, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 11, 0}, 
        
        /* pin 76-79 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 12, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 13, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 14, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 15, 0}
    }, 
    
    /* port 5 */
    {
        /* pin 80-83 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 16, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 17, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 19, 0},
        
        /* pin 84-87 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 14, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 14, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 14, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 14, 0},
        
        /* pin 88-91 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 14, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 14, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 15, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 16, 0},
        
        /* pin 92-95 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 16, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 17, 1},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 17, 1},
        {0, 0, 0}
    },
    
    /* port 6 */
    {
        /* pin 96-99 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 21, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 22, 0},
        {0, 0, 0},
        {0, 0, 0},  
        
        /* pin 100-103 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 104-107 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 108-111 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        
        /* pin 112-113 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0}
    },
    
    /* port 7 */
    {
        /* pin 114-115 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 6, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 6, 0},
        
        /* pin 116-119 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 7, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 7, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 8, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 8, 0}, 
        
        /* pin 120-123 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        
        /* pin 124-127 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 18, 0}
    }
};

/*
	for Z2 version chip:
	in Z1 version, these bits are reserver
*/
sGPIOSelect gpio_select_tableZ2[MAX_GPIO_PORT][MAX_GPIO_PIN]=
{
    /* port 0 */
    {
        /* pin 0-3 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 4-7 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0},
        
        /* pin 8-11 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 12-15 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 31, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 31, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 31, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 31, 0}
    },
    
    /* port 1 */
    {
        /* pin 16-19 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 22, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 23, 0},
        {0, 0, 0}, {0, 0, 0},
        
        /* pin 20-23 */
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 20, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT2_ADDR, 21, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0}, 
        {0, 0, 0},
        
        /* pin 24-27 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 28-31 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
    },
    
    /* port 2 */
    {
        /* pin 32-35 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 36-39 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 40-43 */
        {0, 0, 0}, 
        {0, 0, 0}, 
        {0, 0, 0}, 
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0},
        
        /* pin 44-47 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0},
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0}
    },
    
    /* port 3 */
    {
        /* pin 48-51 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 52-55 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 56-59 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 60-63 */
        {(UINT32) GLOBAL_REG_PIN_SELECT1_ADDR, 30, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
    },
    
    /* port 4 */
    {
        /* pin 64-67 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 68-71 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 72-75 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 76-79 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
    },
    
    /* port 5 */
    {
        /* pin 80-83 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 84-87 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 88-91 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 92-95 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
    },
    
    /* port 6 */
    {
        /* pin 96-99 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 100-103 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 104-107 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 108-111 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 112-113 */
        {0, 0, 0}, {0,0,0}
    },
    
    /* port 7 */
    {
        /* pin 114-115 */
        {0, 0, 0}, {0, 0, 0},
        
        /* pin 116-119 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 120-123 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        
        /* pin 124-127 */
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
    }
};


static int aess_gpio_open(struct inode *inode, struct file *filp)
{
    if (S_u8init_flag == AESSGPIO_NOT_INIT)
    {
        DEBUG_MSG("KN:start init aess_gpio_driver\n");

        S_u8init_flag = AESSGPIO_INIT_OK;
    
        DEBUG_MSG("KN:finish init aess_gpio_driver!\n");

    }
    
    return (STATUS_OK);
}


static int aess_gpio_close(struct inode* inode, struct file *flip)
{
    DEBUG_MSG("aess_gpio_close exit!\n");
    return 0;
}


/* 
	here only set this pin belong to GPIO port,
	if you would like to change to other function,
	need to do it in your driver
*/
int aess_gpio_multi_fun_selection(sGPIOData *pGPIOStruct)
{
    int err_check = STATUS_OK;    
    UINT32 u32TmpBuf = 0;
    UINT32 u32DestAddr = 0;
    UINT8 u8PortNum;
    UINT8 u8PinNum;
    
    /* retrieve port number and pin number */
    u8PortNum = pGPIOStruct->u8PortNum;
    u8PinNum = pGPIOStruct->u8PinNum;
    
#ifdef GPIO_DEBUG3
    printk("port=%2d pin=%2d ", u8PortNum, u8PinNum);
#endif
    
    /* get register address */
    u32DestAddr 
    = (UINT32) (gpio_select_tableZ1[u8PortNum][u8PinNum].u32RegAddr);
    
    if (u32DestAddr > 0)
    {
    
	    /* read current setting */
	    u32TmpBuf = ioread32((void *) u32DestAddr); 
	    
	#ifdef GPIO_DEBUG3
	    printk("bf_z1=%08lx ", u32TmpBuf);
	#endif
	    /* handle special case for MFSEL bit 18-20 */
	    if ((gpio_select_tableZ1[u8PortNum][u8PinNum].u32RegAddr 
	         == (UINT32) GLOBAL_REG_PIN_SELECT1_ADDR) 
	        && (gpio_select_tableZ1[u8PortNum][u8PinNum].u8RegBit == 18))
	    {
	        u32TmpBuf 
	        &= ~(0x7 << gpio_select_tableZ1[u8PortNum][u8PinNum].u8RegBit);
	        
	        u32TmpBuf 
	        |= (gpio_select_tableZ1[u8PortNum][u8PinNum].u8BitEnableValue 
	            << gpio_select_tableZ1[u8PortNum][u8PinNum].u8RegBit);
	        
	        iowrite32(u32TmpBuf, (void *) u32DestAddr);
	    }
	    
	    /* normal case */
	    else 
	    {
	        u32TmpBuf &= ~(1 << gpio_select_tableZ1[u8PortNum][u8PinNum].u8RegBit);
	        
	        u32TmpBuf 
	        |= (gpio_select_tableZ1[u8PortNum][u8PinNum].u8BitEnableValue 
	            << gpio_select_tableZ1[u8PortNum][u8PinNum].u8RegBit);
	        
	        iowrite32(u32TmpBuf, (void *) u32DestAddr);
	    }
	    
	#ifdef GPIO_DEBUG3
	    printk("af_z1=%08lx ", ioread32((void *) u32DestAddr));
	#endif
    }
    /* handle new definition for chip Z2 and newer */
    if (ioread32(GLOBAL_REG_PDID_REG) >= Z2_VERSION_NUM)
    {
        /* get register address */
        u32DestAddr 
        = (UINT32) (gpio_select_tableZ2[u8PortNum][u8PinNum].u32RegAddr);
        
        if (u32DestAddr > 0)
        {
        
	        /* read current setting */
	        u32TmpBuf = ioread32((void *) u32DestAddr); 
	        
	#ifdef GPIO_DEBUG3
	    	printk("bf_z2=%08lx\n", u32TmpBuf);
	#endif
	        u32TmpBuf &= ~(1 << gpio_select_tableZ2[u8PortNum][u8PinNum].u8RegBit);
	        
	        u32TmpBuf 
	        |= (gpio_select_tableZ2[u8PortNum][u8PinNum].u8BitEnableValue 
	            << gpio_select_tableZ2[u8PortNum][u8PinNum].u8RegBit);
	        
	        iowrite32(u32TmpBuf, (void *) u32DestAddr);  
	#ifdef GPIO_DEBUG3
	    	printk("af_z2=%08lx\n", ioread32((void *) u32DestAddr));
	#endif
	
         }
   
    }
    
    return (err_check);
}


int aess_gpio_commander(
                         /* GPIOX */
                         UINT8 u8PortPin,  
                         /* Reference to command list */
                         
                         UINT8 u8CommandType,
                         /* Reference to command list */
                         
                         UINT8 u8CommandNum,
                         /* Data buffer, right now is UINT8 */
                         
                         void *pData
                       )
{
    sGPIOData sGPIOStruct;
      
    sGPIOStruct.u8PortNum = gpio_port_map[(UINT8)(u8PortPin & 0xFF)];;
    sGPIOStruct.u8PinNum = gpio_pin_map[(UINT8)(u8PortPin & 0xFF)];

#ifdef GPIO_DEBUG2
    printk("\n aess_gpio_commander num  %d port %d  pin %d", 
        u8PortPin, sGPIOStruct.u8PortNum, sGPIOStruct.u8PinNum);
#endif
    /* Ensure port and pin number does not exceed bounds */  
    if ((sGPIOStruct.u8PortNum >= MAX_GPIO_PORT) || 
        (sGPIOStruct.u8PinNum >= gpio_pin_number[sGPIOStruct.u8PortNum]))
    {
        return -STATUS_FAIL;
    }

    /* GPIO_CONFIG command doesn't need to use the pData parameter */
    if ((pData == NULL) && (u8CommandType != GPIO_CONFIG))
    {
        return -STATUS_FAIL;
    }    

    sGPIOStruct.u8CommandType = u8CommandType;
    sGPIOStruct.u8CommandNum = u8CommandNum;
    sGPIOStruct.pData = pData;    

    /* Dispath the command */
    switch(u8CommandType)
    {
        case GPIO_CONFIG:    
            /* Ensure the command number doesn't out of range */
            if (u8CommandNum >= MAX_CONFIG_COMMAND)
            {
                return -STATUS_FAIL;           
            }        
            else
            {
                return (aess_gpio_config(&sGPIOStruct));
            }
            break;
            
        case GPIO_WRITE:    
            /* Ensure the command number doesn't out of range */
            if (u8CommandNum >= MAX_WRITE_COMMAND)
            {
                return -STATUS_FAIL;                 
            }        
            else
            {
                return (aess_gpio_write(&sGPIOStruct));
            }            
            
        case GPIO_READ:    
            /* No command number */        
            return (aess_gpio_read(&sGPIOStruct));
            break;        
                                  
        default:
            printk("KN:aess_gpio_commander, command type error\n");
            return -STATUS_FAIL;     
    }                           
}

static int aess_gpio_config(sGPIOData *pGPIOStruct)
{           
    UINT32 u32DestAddr, u32TmpBuf;
    int err_check = STATUS_OK;
    UINT8 u8InputValue = SET_GPIO_OUTPUT_LOW;
    UINT8 u8CfgCmd;
    int int_polarity;
    
    /* this register is only for all pin of port 0 and pin 24&25 of port 1 */
    /* Bits 17-16 corespond to GPIOE25 and GPIOE24. 
       Bits 15-0 correspond to pins 15-0 of port 0 */
    if ((pGPIOStruct->u8PortNum > 1) || 
        ((pGPIOStruct->u8PortNum == 1) && 
        ((pGPIOStruct->u8PinNum != 8) && (pGPIOStruct->u8PinNum != 9))))
        return -EINVAL;
    
    /* backup command */
    u8CfgCmd = pGPIOStruct->u8CommandNum;
        
    // DF320121; Optional param for INTT_BOTH_LEVEL to allow user to set next IRQ_LEVEL
    //  Ignore = 00b ;IRQ_ON_LO = 10b ;IRQ_ON_HI = 11b
    int_polarity = 0;
    if (pGPIOStruct->pData != NULL)
    {
      int_polarity = (*((UINT32 *)pGPIOStruct->pData) >> 9) & 0x3;        
    } 
    
    pGPIOStruct->pData = (void *) &u8InputValue;
    /* Set direction to input */
    pGPIOStruct->u8CommandNum = NORMAL_INPUT;
    err_check = aess_gpio_read(pGPIOStruct);
    
    /* restore command */
    pGPIOStruct->u8CommandNum = u8CfgCmd;
                   
    /* Config the specific GPIO pin */
    switch(pGPIOStruct->u8CommandNum)
    {
        case INPUT_INT_OFF:
#ifdef GPIO_DEBUG2
            printk("\n INPUT_INT_OFF\n");
#endif
            /* Set direction to input, interrupt off */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct); 
            break;
            
        case INPUT_INT_RISING:
#ifdef GPIO_DEBUG2
            printk("\n INPUT_INT_RISING\n");
#endif
            /* Set direction to input, interrupt generate in rising edge */
            /* before enabling any system interrupt, it is recommended to
               set the desired event configuration and then 
               verify that the status register is cleared */
            /* diable interrupt */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_TYPE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_POLARITY_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = CLR_INT_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* enable debounce */
			*((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
			pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
			err_check = aess_gpio_write(pGPIOStruct);
            break;
            
        case INPUT_INT_FAILING:
#ifdef GPIO_DEBUG2
            printk("\n INPUT_INT_FAILING\n");
#endif
            /* Set direction to input, interrupt generate in failing edge */
            /* before enabling any system interrupt, it is recommended to
               set the desired event configuration and then 
               verify that the status register is cleared */
            /* diable interrupt */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_TYPE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_POLARITY_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = CLR_INT_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* enable debounce */
			*((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
			pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
			err_check = aess_gpio_write(pGPIOStruct);
            break;
            
        case INPUT_INT_HIGH:
#ifdef GPIO_DEBUG2
            printk("\n INPUT_INT_HIGH\n");
#endif
            /* Set direction to input, interrupt generate in high lever */
            /* according winbond's suggestion, disable debounce */
            /* before enabling any system interrupt, it is recommended to
               set the desired event configuration and then 
               verify that the status register is cleared */
            /* diable interrupt */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* disable debounce */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* set Level type */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_TYPE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* set high level */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_POLARITY_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* clr interrupt bit */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = CLR_INT_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* enable interrupt */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* enable debounce */
			*((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
			pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
			err_check = aess_gpio_write(pGPIOStruct);
            break;
            
        case INPUT_INT_LOW:
#ifdef GPIO_DEBUG2
            printk("\n INPUT_INT_LOW\n");
#endif
            /* Set direction to input, interrupt generate in low level */
            /* according winbond's suggestion, disable debounce */
            /* before enabling any system interrupt, it is recommended to
               set the desired event configuration and then 
               verify that the status register is cleared */
            /* diable interrupt */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_TYPE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_POLARITY_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = CLR_INT_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* enable debounce */
			*((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
			pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
			err_check = aess_gpio_write(pGPIOStruct);
            break;
            
        case INPUT_INT_BOTH_EDGE:
#ifdef GPIO_DEBUG2
            printk("\n INPUT_INT_BOTH_EDGE\n");
#endif
            /* Set direction to input, interrupt generate in both edge */
            /* according winbond's suggestion, disable debounce */
            /* before enabling any system interrupt, it is recommended to
               set the desired event configuration and then verify that 
               the status register is cleared */
            /* diable interrupt */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_TYPE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* Read data from data-in buffer */
            u32DestAddr = (UINT32) (gpio_port_datain_addr[pGPIOStruct->u8PortNum]);
            u32TmpBuf = ioread32((void *) u32DestAddr);
            *((UINT8 *)(pGPIOStruct->pData)) = (UINT8)((u32TmpBuf >> pGPIOStruct->u8PinNum) & GPIO_DATA_MASK);
            
            if (*((UINT8 *)(pGPIOStruct->pData)) == SET_GPIO_OUTPUT_LOW)
            {
                *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
#ifdef GPIO_DEBUG2
                printk("\n INPUT_INT_BOTH_EDGE rising\n");
#endif
            }
            else
            {
                *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
#ifdef GPIO_DEBUG2
                printk("\n INPUT_INT_BOTH_EDGE falling\n");
#endif
            }
            
            pGPIOStruct->u8CommandNum = EVENT_POLARITY_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = CLR_INT_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            /* enable debounce */
			*((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
			pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
			err_check = aess_gpio_write(pGPIOStruct);
            break;
            
        case INPUT_INT_BOTH_LEVEL:
#ifdef GPIO_DEBUG2
            printk("\n INPUT_INT_BOTH_LEVEL, port %02x, pin %02x \n", pGPIOStruct->u8PortNum, pGPIOStruct->u8PinNum);
#endif
            /* Set direction to input, interrupt generate in both edge */
            /* according winbond's suggestion, disable debounce */
            /* before enabling any system interrupt, it is recommended to
               set the desired event configuration and then 
               verify that the status register is cleared */
            /* diable interrupt */
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_TYPE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
    // DF320121 Front Panel right Navigation button reversed
      // Optional param for INTT_BOTH_LEVEL to allow user to set next IRQ_LEVEL
    //  i.e. I think the button pressed, please tell me when its released 
      //  Ignore = 00b ;IRQ_ON_LO = 10b ;IRQ_ON_HI = 11b
            if (int_polarity == 3)
            {
              *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;        
            } 
            else if (int_polarity == 2)
            {
              *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
            }
            else
            {
              // Read data from data-in buffer
              u32DestAddr = (UINT32) (gpio_port_datain_addr[pGPIOStruct->u8PortNum]);
              u32TmpBuf = ioread32((void *) u32DestAddr);
              *((UINT8 *)(pGPIOStruct->pData)) = (UINT8)((u32TmpBuf >> pGPIOStruct->u8PinNum) & GPIO_DATA_MASK);
              
              if (*((UINT8 *)(pGPIOStruct->pData)) == SET_GPIO_OUTPUT_LOW)
              {
                  *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
              }
              else
              {
                  *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_LOW;
              }
            }
#ifdef GPIO_DEBUG2
                printk("\n INPUT_INT_BOTH_EDGE trigger %i, port %02x, pin %02x", 
                  *((UINT8 *)(pGPIOStruct->pData)), 
                  pGPIOStruct->u8PortNum, 
                  pGPIOStruct->u8PinNum);
#endif
            pGPIOStruct->u8CommandNum = EVENT_POLARITY_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);

            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = CLR_INT_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);
            
            *((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
            pGPIOStruct->u8CommandNum = EVENT_ENABLE_OUTPUT;
            err_check = aess_gpio_write(pGPIOStruct);

            /* enable debounce */
			*((UINT8 *)(pGPIOStruct->pData)) = SET_GPIO_OUTPUT_HIGH;
			pGPIOStruct->u8CommandNum = DEBOUNCE_OUTPUT;
			err_check = aess_gpio_write(pGPIOStruct);
            break;
            
        default:
            printk("KN:aess_gpio_config, command error\n");
            return -STATUS_FAIL;
    }
    
#ifdef GPIO_DEBUG2
    {
        printk("\n GPIO_REG_EVENT_TYPE_ADDR = 0x%x", ioread32((void *) GPIO_REG_EVENT_TYPE_ADDR));
        printk("\n GPIO_REG_EVENT_POLARITY_ADDR = 0x%x", ioread32((void *) GPIO_REG_EVENT_POLARITY_ADDR));
        printk("\n GPIO_REG_EVENT_ENABLE_ADDR = 0x%x", ioread32((void *) GPIO_REG_EVENT_ENABLE_ADDR));
    }
#endif
    return (STATUS_OK);
}

static int aess_gpio_read(sGPIOData *pGPIOStruct)
{
    UINT32 u32TmpBuf;
    UINT32 u32DestAddr;
    UINT32 u32ShitNum;       
    
#ifdef GPIO_DEBUG2
    printk("\n aess_gpio_read port %d  pin %d command %d", 
    pGPIOStruct->u8PortNum, pGPIOStruct->u8PinNum, pGPIOStruct->u8CommandNum);
#endif
    
    switch(pGPIOStruct->u8CommandNum)
    {
        case NORMAL_INPUT:
            /* get the address for the specific GPIO port */
            u32DestAddr = (UINT32) (gpio_port_datamode_addr[pGPIOStruct->u8PortNum]);
            if (u32DestAddr > 0)
            {
                /* Read data to the buffer */
                u32TmpBuf = ioread32((void *) u32DestAddr);

                /* Set Data direction to input */
                u32TmpBuf &= ~(1 << pGPIOStruct->u8PinNum);
                iowrite32((UINT32) u32TmpBuf, (void *) u32DestAddr); 
            }

            /* Read data from data-in buffer */
            u32DestAddr = (UINT32) (gpio_port_datain_addr[pGPIOStruct->u8PortNum]);
            u32TmpBuf = ioread32((void *) u32DestAddr);
            *((UINT8 *)(pGPIOStruct->pData)) = 
                (UINT8)((u32TmpBuf >> pGPIOStruct->u8PinNum) & GPIO_DATA_MASK);
            break;
            
        case INT_STATUS_INPUT:
            /* this register is only for all pin of port 0 and pin 24&25 of port 1 */
            /* Bits 17-16 corespond to GPIOE25 and GPIOE24. 
               Bits 15-0 correspond to pins 15-0 of port 0 */
            if ((pGPIOStruct->u8PortNum == 0) || 
                ((pGPIOStruct->u8PortNum == 1) && 
                ((pGPIOStruct->u8PinNum == 8) || (pGPIOStruct->u8PinNum == 9))))
            {
                u32DestAddr = (UINT32) GPIO_REG_EVENT_STATUS_ADDR;
                
                /* Read data to the buffer */
                u32TmpBuf = ioread32((void *) u32DestAddr);
#ifdef GPIO_DEBUG2
                printk("\n INT_STATUS_INPUT = 0x%x", u32TmpBuf);
#endif
				u32ShitNum = pGPIOStruct->u8PinNum + 8 * pGPIOStruct->u8PortNum;
                *((UINT8 *)(pGPIOStruct->pData)) = 
                   (UINT8)((u32TmpBuf >> u32ShitNum) & GPIO_DATA_MASK);
            }
            else
            {
                printk("KN:aess_gpio_read, GPIO pin error\n");
                return -EIO;
            }
            break;
            
        case INT_ENABLE_INPUT:
            /* this register is only for all pin of port 0 and pin 24&25 of port 1 */
            /* Bits 17-16 corespond to GPIOE25 and GPIOE24. 
            Bits 15-0 correspond to pins 15-0 of port 0 */
            if ((pGPIOStruct->u8PortNum == 0) || 
                ((pGPIOStruct->u8PortNum == 1) && 
                ((pGPIOStruct->u8PinNum == 8) || (pGPIOStruct->u8PinNum == 9))))
            {
                u32DestAddr = (UINT32) GPIO_REG_EVENT_ENABLE_ADDR;
                
                /* Read data to the buffer */
                u32TmpBuf = ioread32((void *) u32DestAddr);
#ifdef GPIO_DEBUG2
                printk("\n GPIO_REG_EVENT_ENABLE_ADDR = 0x%x", u32TmpBuf);
#endif
				u32ShitNum = pGPIOStruct->u8PinNum + 8 * pGPIOStruct->u8PortNum;
                *((UINT8 *)(pGPIOStruct->pData)) = 
                    (UINT8)((u32TmpBuf >> u32ShitNum) & GPIO_DATA_MASK);
            }
            else
            {
                printk("KN:aess_gpio_read, GPIO pin error\n");
                return -EIO;
            }
            break;
            
		case OUTPUT_INPUT:
            /* Read data from data-out buffer */
            u32DestAddr = (UINT32) (gpio_port_dataout_addr[pGPIOStruct->u8PortNum]);
            if (u32DestAddr > 0)
            {
	            u32TmpBuf = ioread32((void *) u32DestAddr);
#ifdef GPIO_DEBUG2
				printk("\n gpio_port_dataout_addr = 0x%x", u32TmpBuf);
#endif
	            *((UINT8 *)(pGPIOStruct->pData)) = 
	                (UINT8)((u32TmpBuf >> pGPIOStruct->u8PinNum) & GPIO_DATA_MASK);
            }
            else
            {
                printk("KN:aess_gpio_read, GPI pin is read only \n");
                return -EIO;
           	}
			break;
			
		case BLINK_INPUT:
            if ((pGPIOStruct->u8PortNum == 1) && (pGPIOStruct->u8PinNum < 8))
            {
                u32DestAddr = (UINT32) GPIO_REG_PORT1_BLINK_CTR_ADDR;
                
                /* Read data to the buffer */
                u32TmpBuf = ioread32((void *) u32DestAddr);
#ifdef GPIO_DEBUG2
                printk("\n GPIO_REG_PORT1_BLINK_CTR_ADDR = 0x%x", u32TmpBuf);
#endif
				u32ShitNum = pGPIOStruct->u8PinNum + 4 * pGPIOStruct->u8PortNum;
                *((UINT8 *)(pGPIOStruct->pData)) = 
                    (UINT8)((u32TmpBuf >> u32ShitNum) & GPIO_BLINK_DATA_MASK);
            }
            else
            {
                printk("KN:aess_gpio_read, GPIO pin don't support Blink function\n");
                return -EIO;
            }
            break;
			
		case DIRECT_INPUT:
            /* Read data from data-direction buffer */
            u32DestAddr = (UINT32) (gpio_port_datamode_addr[pGPIOStruct->u8PortNum]);
            if (u32DestAddr > 0)
            {
	            u32TmpBuf = ioread32((void *) u32DestAddr);
#ifdef GPIO_DEBUG2
				printk("\n gpio_port_datamode_addr = 0x%x", u32TmpBuf);
#endif
	            *((UINT8 *)(pGPIOStruct->pData)) = 
	                (UINT8)((u32TmpBuf >> pGPIOStruct->u8PinNum) & GPIO_DATA_MASK);
            }
            else
            {
                printk("KN:aess_gpio_read, GPI pin is read only \n");
                return -EIO;
           	}
			break;
			
		case OPEN_DRAIN_INPUT:
            u32DestAddr = (UINT32) (gpio_port_opendrain_addr[pGPIOStruct->u8PortNum]);
                
			/* Read data to the buffer */
			u32TmpBuf = ioread32((void *) u32DestAddr);
#ifdef GPIO_DEBUG2
			printk("\n gpio_port_opendrain_addr = 0x%x", u32TmpBuf);
#endif
			u32ShitNum = pGPIOStruct->u8PinNum;
			*((UINT8 *)(pGPIOStruct->pData)) = 
				(UINT8)((u32TmpBuf >> u32ShitNum) & GPIO_DATA_MASK);
			break;
            
        default:
            printk("KN:aess_gpio_read, command error\n");
            return -EIO;
            break;
    }
    
#ifdef GPIO_DEBUG2
    printk("\n u32DestAddr = 0x%x command %d  data =%d", 
        u32DestAddr, pGPIOStruct->u8CommandNum, *((UINT8 *)(pGPIOStruct->pData)));
#endif
    return (STATUS_OK);
}

static int aess_gpio_write(sGPIOData *pGPIOStruct)
{
    int err_check=STATUS_OK;    
    UINT32 u32TmpBuf = 0;
    UINT8 u8TmpBuf2;
    UINT32 u32DestAddr = 0;
    UINT8 read_flag = 1;
    UINT8 u8TmpBit = pGPIOStruct->u8PinNum;
    
    if (pGPIOStruct->u8PortNum == 6) //GPIO6 is read only
        return -EINVAL;
    
#ifdef GPIO_DEBUG2
    printk("\n aess_gpio_write port %d  pin %d", pGPIOStruct->u8PortNum, u8TmpBit);
    printk("\n command %d  data =%d", 
        pGPIOStruct->u8CommandNum, *((UINT8 *)(pGPIOStruct->pData)));
#endif
    switch(pGPIOStruct->u8CommandNum)
    {
        case NORMAL_OUTPUT:    
			/* set to output data */
			u32DestAddr = (UINT32) gpio_port_dataout_addr[pGPIOStruct->u8PortNum];
            if (u32DestAddr > 0){
        
                /* Read data to the buffer */
                u32TmpBuf = ioread32((void *) u32DestAddr);
        
				//clear bit
				u32TmpBuf &= ~(1 << u8TmpBit);
                
				/* Set Data data to output */
				u32TmpBuf |= ((*((UINT8 *)(pGPIOStruct->pData))) << pGPIOStruct->u8PinNum);
                iowrite32((UINT32) u32TmpBuf, (void *) u32DestAddr); 
            }
			else
			{
				err_check = -EINVAL;
				break;
			}

			/* set to direction data */
			(*((UINT8 *)(pGPIOStruct->pData))) = 1;
			u32DestAddr = (UINT32) (gpio_port_datamode_addr[pGPIOStruct->u8PortNum]);
            break;
            
        case OPEN_DRAIN_OUTPUT:    
            if (pGPIOStruct->u8PortNum > 0)
            u32DestAddr = (UINT32) gpio_port_opendrain_addr[pGPIOStruct->u8PortNum];
            else
                err_check = -EINVAL;
            break;
            
        case POWER_OUTPUT:    
            if (pGPIOStruct->u8PortNum > 0)
                u32DestAddr = (UINT32) gpio_port_power_addr[pGPIOStruct->u8PortNum];
            else
                err_check = -EINVAL;
            break;
            
        case BLINK_OUTPUT:
            if ((pGPIOStruct->u8PortNum == 1) && (pGPIOStruct->u8PinNum < 8))
                u32DestAddr = (UINT32) GPIO_REG_PORT1_BLINK_CTR_ADDR;
            else
                err_check = -EINVAL;
            break;
            
        case DEBOUNCE_OUTPUT:    
            if (pGPIOStruct->u8PortNum == 0)
                u32DestAddr = (UINT32) GPIO_REG_EVENT_DEBOUNCE_ADDR;
            else
                err_check = -EINVAL;
            break;
            
        case CLR_INT_OUTPUT:    
            /* this register is only for all pin of port 0 and pin 24&25 of port 1 */
            /* Bits 17-16 corespond to GPIOE25 and GPIOE24. 
               Bits 15-0 correspond to pins 15-0 of port 0 */
            if ((pGPIOStruct->u8PortNum == 0) || 
                ((pGPIOStruct->u8PortNum == 1) && 
                 ((pGPIOStruct->u8PinNum == 8) || (pGPIOStruct->u8PinNum == 9))))
            {
                u32DestAddr = (UINT32) GPIO_REG_EVENT_STATUS_ADDR;
                u8TmpBit = pGPIOStruct->u8PinNum + 8 * pGPIOStruct->u8PortNum;
            }
            else
            {
                printk("KN:aess_gpio_write, CLR_INT_OUTPUT GPIO pin error\n");
                err_check = -EINVAL;
            }
            read_flag = 0;
            break;
            
        case EVENT_TYPE_OUTPUT:    
            /* this register is only for all pin of port 0 and pin 24&25 of port 1 */
            /* Bits 17-16 corespond to GPIOE25 and GPIOE24. 
               Bits 15-0 correspond to pins 15-0 of port 0 */
            if ((pGPIOStruct->u8PortNum == 0) || 
                ((pGPIOStruct->u8PortNum == 1) && 
                 ((pGPIOStruct->u8PinNum == 8) || (pGPIOStruct->u8PinNum == 9))))
            {
                u32DestAddr = (UINT32) GPIO_REG_EVENT_TYPE_ADDR;
                u8TmpBit = pGPIOStruct->u8PinNum + 8 * pGPIOStruct->u8PortNum;
            }
            else
            {
                err_check = -EINVAL;
            }
            break;
            
        case EVENT_POLARITY_OUTPUT:    
            /* this register is only for all pin of port 0 and pin 24&25 of port 1 */
            /* Bits 17-16 corespond to GPIOE25 and GPIOE24. 
               Bits 15-0 correspond to pins 15-0 of port 0 */
            if ((pGPIOStruct->u8PortNum == 0) || 
                ((pGPIOStruct->u8PortNum == 1) && 
                 ((pGPIOStruct->u8PinNum == 8) || (pGPIOStruct->u8PinNum == 9))))
            {
                u32DestAddr = (UINT32) GPIO_REG_EVENT_POLARITY_ADDR;
                u8TmpBit = pGPIOStruct->u8PinNum + 8 * pGPIOStruct->u8PortNum;
            }
            else
            {
                err_check = -EINVAL;
            }
            break;
            
        case EVENT_ENABLE_OUTPUT:    
            /* this register is only for all pin of port 0 and pin 24&25 of port 1 */
            /* Bits 17-16 corespond to GPIOE25 and GPIOE24. 
               Bits 15-0 correspond to pins 15-0 of port 0 */
            if ((pGPIOStruct->u8PortNum == 0) || 
                ((pGPIOStruct->u8PortNum == 1) && 
                 ((pGPIOStruct->u8PinNum == 8) || (pGPIOStruct->u8PinNum == 9))))
            {
                u32DestAddr = (UINT32) GPIO_REG_EVENT_ENABLE_ADDR;
                u8TmpBit = pGPIOStruct->u8PinNum + 8 * pGPIOStruct->u8PortNum;
            }
            else
            {
                err_check = -EINVAL;
            }
            break;
            
        case GPIO_SELECTED_OUTPUT:    
            /* set to output mode */
            aess_gpio_multi_fun_selection(pGPIOStruct);
            return (err_check);          
            break;
            
        default:
            printk("KN:aess_gpio_write, command error\n");
            err_check = -EINVAL; 
    }         

    if ((err_check == -EINVAL) || (u32DestAddr == 0))
            return (err_check);          

    /* Read data to the buffer */
    u32TmpBuf = 0;
    if (read_flag == 1)
    {
        u32TmpBuf = ioread32((void *) u32DestAddr);
    }
    
    switch(pGPIOStruct->u8CommandNum)
    {
        case BLINK_OUTPUT: 
            u8TmpBuf2 = *((UINT8 *)(pGPIOStruct->pData));
            u8TmpBuf2 = u8TmpBuf2 & GPIO_BLINK_DATA_MASK;
            u32TmpBuf &= ~((UINT32)(GPIO_BLINK_DATA_MASK << (pGPIOStruct->u8PinNum * 4)));
            u32TmpBuf |= ((UINT32)(u8TmpBuf2 << (pGPIOStruct->u8PinNum * 4)));
            iowrite32(u32TmpBuf, (void *) u32DestAddr); 
            break;
            
        case GPIO_SELECTED_OUTPUT:    
            break;
            
        case EVENT_TYPE_OUTPUT: 
        default:
             
            /* Check the output data is low or high */
            if (*((UINT8 *)(pGPIOStruct->pData)) == SET_GPIO_OUTPUT_LOW)
            {
                /* Output low to the specific GPIO pin */
                u32TmpBuf &= ~(1 << u8TmpBit);
                iowrite32(u32TmpBuf, (void *) u32DestAddr);  
            }
            else if (*((UINT8 *)(pGPIOStruct->pData)) == SET_GPIO_OUTPUT_HIGH)
            {
                /* Output high to the specific GPIO pin */
                u32TmpBuf |= (1 << u8TmpBit);
                iowrite32(u32TmpBuf, (void *) u32DestAddr); 
            }    
            else
            {
                err_check = -EINVAL;                            
            }
            break;
    }
    
    return (err_check);                    
}

static int aess_gpio_ioctl(struct inode * inode, struct file *flip, 
                           unsigned int cmd, unsigned long arg)
{
    int err_check;
    sGPIOData *pGPIOStruct = (sGPIOData *) arg;
    
    DEBUG_MSG("KN:aess_gpio_ioctl\n");
    
    /* Check the structure address is OK */
    if(!access_ok(VERIFY_WRITE, (void *) pGPIOStruct, sizeof(sGPIOData)))
    {
        printk("KN:struct access error!!\n");
        /* error occur */
        return -EFAULT;
    }
      
    switch(cmd)
    {
        case AESS_GPIODRV_R:
            pGPIOStruct->u8CommandType = GPIO_READ;
            {        
                DEBUG_MSG("KN:Start Read process!!\n");
                /* Check the buffer address exist in the structure is OK */
                if(!access_ok(VERIFY_READ, pGPIOStruct->pData, sizeof(UINT8)))
                {
                    printk("KN:buffer access error!!\n");
                    /* error occur */
                    err_check = -EFAULT;
                }
                  
                err_check = aess_gpio_read(pGPIOStruct);
                DEBUG_MSG("KN:Finish Read process!!\n");
            }
            break;
            
        case AESS_GPIODRV_W:
            pGPIOStruct->u8CommandType = GPIO_WRITE;
            {
                DEBUG_MSG("KN:Start write process!!\n");
                /* Check the buffer address exist in the structure is OK */
                if(!access_ok(VERIFY_WRITE, pGPIOStruct->pData, sizeof(UINT8)))
                {
                    printk("KN:buffer access error!!\n");
                    /* error occur */
                    err_check = -EFAULT;
                }
                
                err_check = aess_gpio_write(pGPIOStruct);         
                DEBUG_MSG("KN:Finish write process!!\n");                
            }
            break;
            
        case AESS_GPIODRV_CFG:
            pGPIOStruct->u8CommandType = GPIO_CONFIG;
            {
                DEBUG_MSG("KN:Start config process!!\n");
                err_check = aess_gpio_config(pGPIOStruct);         
                DEBUG_MSG("KN:Finish config process!!\n");                
            }
            break;
            
        default:
            printk("KN:aess_gpio_ioctl, command error\n");
            err_check = -EINVAL;
    }
    
    /* 0->ok, minus->fail */
    return err_check;
}


static struct file_operations aess_gpio_fops = {
    .open = aess_gpio_open, 
    .ioctl = aess_gpio_ioctl,
    .release = aess_gpio_close,
};


int __init aess_gpio_init(void)
{
    int result;
    gpio_cdev = cdev_alloc();
    
    DEBUG_MSG("KN:init_aess_gpio_module\n");
    
    gpio_cdev->ops = &aess_gpio_fops;
    result = alloc_chrdev_region(&gpio_dev, 0, 1, driver_name);
    
    printk("mknod /dev/aess_gpiodrv c %d 0\n", MAJOR(gpio_dev));
    
    if (result < 0) 
    {
        printk("KN:Registering the GPIO character device failed with %d\n", MAJOR(gpio_dev));
        return result;
    }
    
    cdev_add(gpio_cdev, gpio_dev, 1);
    return (STATUS_OK);
}

static void __exit aess_gpio_exit(void)
{
    DEBUG_MSG("aess_gpiodrv_exit\n");
    
    cdev_del(gpio_cdev);
    unregister_chrdev_region(gpio_dev, 1);
}


MODULE_DESCRIPTION("AESS GPIO Driver");
MODULE_AUTHOR("Kopi Hsu <kopi.hsu@avocent.com>");
MODULE_LICENSE("GPL");
module_init(aess_gpio_init);
module_exit(aess_gpio_exit);
module_param(driver_name, charp, S_IRUGO);
EXPORT_SYMBOL(aess_gpio_commander);
