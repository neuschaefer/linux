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
#include <asm/ioctl.h>
#include <asm/arch/map.h>

#if !defined (AESSGPIODRV_H)
#define AESSGPIODRV_H


/* Global include area */
/* GPIO-config command number */
#define INPUT_INT_OFF            0x0
#define INPUT_INT_RISING         0x1
#define INPUT_INT_FAILING        0x2
#define INPUT_INT_HIGH           0x3
#define INPUT_INT_LOW            0x4
#define INPUT_INT_BOTH_EDGE      0x5
#define INPUT_INT_BOTH_LEVEL      0x6

/* GPIO command type */
#define GPIO_CONFIG    0 
#define GPIO_WRITE     1
#define GPIO_READ      2


/* GPIO-input command number */
#define NORMAL_INPUT            0x0
#define INT_STATUS_INPUT                0x1
#define INT_ENABLE_INPUT                0x2
#define BLINK_INPUT                		0x3
#define OUTPUT_INPUT                	0x4
#define DIRECT_INPUT                	0x5
#define OPEN_DRAIN_INPUT                0x6

/* GPIO-output command number */
#define NORMAL_OUTPUT            0x0
#define OPEN_DRAIN_OUTPUT        0x1
#define POWER_OUTPUT             0x2
#define BLINK_OUTPUT             0x3
#define DEBOUNCE_OUTPUT          0x4
#define CLR_INT_OUTPUT               0x5
#define EVENT_TYPE_OUTPUT        0x6
#define EVENT_POLARITY_OUTPUT    0x7
#define EVENT_ENABLE_OUTPUT      0x8
#define GPIO_SELECTED_OUTPUT     0x9

/* Command value and mask */
#define SET_GPIO_OUTPUT_LOW          0x00000000
#define SET_GPIO_OUTPUT_HIGH         0x00000001



#ifdef AESSGPIODRV_C

#define MAX_CONFIG_COMMAND      7
#define MAX_WRITE_COMMAND       10  

/* type define */
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;


/* ioctl definitions */
#define AESS_GPIODRV_IOC_MAGIC    0xB5
#define AESS_GPIODRV_R           _IOWR(AESS_GPIODRV_IOC_MAGIC, 0, int)
#define AESS_GPIODRV_W           _IOWR(AESS_GPIODRV_IOC_MAGIC, 1, int)
#define AESS_GPIODRV_CFG           _IOWR(AESS_GPIODRV_IOC_MAGIC, 2, int)



/* For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1

/* init flag */
#define AESSGPIO_NOT_INIT 0
#define AESSGPIO_INIT_OK  1


/* Command value and mask */

#define GPIO_DATA_MASK           0x00000001
#define GPIO_DIRECTION_MASK      0x00000002

#define GPIO_PORT_MASK           0x000000E0
#define GPIO_PIN_MASK            0x0000001F
#define GPIO_PORT_SHIFT          0x5

#define GPIO_BLINK_DATA_MASK           0x7

/* Define the maximum port, pin number, command number */
#define MAX_GPIO_PIN            18//16
#define MAX_GPIO_PORT           8
  
/* PDID */
#define Z2_VERSION_NUM       0x03926450

/* GPIO port base address */
#define GPIO_PORT_BASE_ADDR      WPCM450_VA_GPIO
#define WPCM450_GLOBAL_CTRL_REG    WPCM450_VA_GCR

#define GLOBAL_REG_PDID_REG          				(WPCM450_GLOBAL_CTRL_REG+0x0)
#define GLOBAL_REG_PIN_SELECT1_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0xc)
#define GLOBAL_REG_PIN_SELECT2_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0x10)
#define GLOBAL_REG_GPIOP1PE_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0x1c)
#define GLOBAL_REG_GPIOP2PE_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0x20)
#define GLOBAL_REG_GPIOP3PE_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0x24)
#define GLOBAL_REG_GPIOP5PE_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0x2c)
#define GLOBAL_REG_GPIOP6PE_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0x30)
#define GLOBAL_REG_GPIOP7PE_ADDR  				(WPCM450_GLOBAL_CTRL_REG + 0x34)

#define GPIO_REG_EVENT_TYPE_ADDR						(GPIO_PORT_BASE_ADDR + 0)
#define GPIO_REG_EVENT_POLARITY_ADDR   			(GPIO_PORT_BASE_ADDR + 0x4)
#define GPIO_REG_EVENT_DEBOUNCE_ADDR				(GPIO_PORT_BASE_ADDR + 0x8)
#define GPIO_REG_EVENT_ENABLE_ADDR          (GPIO_PORT_BASE_ADDR + 0xc)
#define GPIO_REG_EVENT_STATUS_ADDR          (GPIO_PORT_BASE_ADDR + 0x10)

#define GPIO_REG_PORT0_CFG0_ADDR          	(GPIO_PORT_BASE_ADDR + 0x14)
#define GPIO_REG_PORT0_CFG1_ADDR          	(GPIO_PORT_BASE_ADDR + 0x18)
#define GPIO_REG_PORT0_DATAOUT_ADDR					(GPIO_PORT_BASE_ADDR + 0x1c)
#define GPIO_REG_PORT0_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x20)

#define GPIO_REG_PORT1_CFG0_ADDR          	(GPIO_PORT_BASE_ADDR + 0x24)
#define GPIO_REG_PORT1_CFG1_ADDR          	(GPIO_PORT_BASE_ADDR + 0x28)
#define GPIO_REG_PORT1_CFG2_ADDR          	(GPIO_PORT_BASE_ADDR + 0x2c)
#define GPIO_REG_PORT1_BLINK_CTR_ADDR				(GPIO_PORT_BASE_ADDR + 0x30)
#define GPIO_REG_PORT1_DATAOUT_ADDR					(GPIO_PORT_BASE_ADDR + 0x34)
#define GPIO_REG_PORT1_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x38)

#define GPIO_REG_PORT2_CFG0_ADDR          	(GPIO_PORT_BASE_ADDR + 0x3c)
#define GPIO_REG_PORT2_CFG1_ADDR          	(GPIO_PORT_BASE_ADDR + 0x40)
#define GPIO_REG_PORT2_CFG2_ADDR          	(GPIO_PORT_BASE_ADDR + 0x44)
#define GPIO_REG_PORT2_DATAOUT_ADDR					(GPIO_PORT_BASE_ADDR + 0x48)
#define GPIO_REG_PORT2_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x4c)

#define GPIO_REG_PORT3_CFG0_ADDR          	(GPIO_PORT_BASE_ADDR + 0x50)
#define GPIO_REG_PORT3_CFG1_ADDR          	(GPIO_PORT_BASE_ADDR + 0x54)
#define GPIO_REG_PORT3_CFG2_ADDR          	(GPIO_PORT_BASE_ADDR + 0x58)
#define GPIO_REG_PORT3_DATAOUT_ADDR					(GPIO_PORT_BASE_ADDR + 0x5c)
#define GPIO_REG_PORT3_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x60)

#define GPIO_REG_PORT4_CFG0_ADDR          	(GPIO_PORT_BASE_ADDR + 0x64)
#define GPIO_REG_PORT4_CFG1_ADDR          	(GPIO_PORT_BASE_ADDR + 0x68)
#define GPIO_REG_PORT4_CFG2_ADDR          	(GPIO_PORT_BASE_ADDR + 0x6c)
#define GPIO_REG_PORT4_DATAOUT_ADDR					(GPIO_PORT_BASE_ADDR + 0x70)
#define GPIO_REG_PORT4_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x74)

#define GPIO_REG_PORT5_CFG0_ADDR          	(GPIO_PORT_BASE_ADDR + 0x78)
#define GPIO_REG_PORT5_CFG1_ADDR          	(GPIO_PORT_BASE_ADDR + 0x7c)
#define GPIO_REG_PORT5_CFG2_ADDR          	(GPIO_PORT_BASE_ADDR + 0x80)
#define GPIO_REG_PORT5_DATAOUT_ADDR					(GPIO_PORT_BASE_ADDR + 0x84)
#define GPIO_REG_PORT5_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x88)

#define GPIO_REG_PORT6_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x8c)

#define GPIO_REG_PORT7_CFG0_ADDR          	(GPIO_PORT_BASE_ADDR + 0x90)
#define GPIO_REG_PORT7_CFG1_ADDR          	(GPIO_PORT_BASE_ADDR + 0x94)
#define GPIO_REG_PORT7_CFG2_ADDR          	(GPIO_PORT_BASE_ADDR + 0x98)
#define GPIO_REG_PORT7_DATAOUT_ADDR					(GPIO_PORT_BASE_ADDR + 0x9c)
#define GPIO_REG_PORT7_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0xa0)

#define GPIO_PORT_ADDR_UNIT      0x40
#define GPIO_PIN_ADDR_UNIT       0x04
#define GPIO_PIN_MAX       			128


// pin number for port
UINT8 gpio_pin_number[MAX_GPIO_PORT] = {16,16,16,16,16,15,18,14};
UINT8 gpio_port_map[GPIO_PIN_MAX] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,
    4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,
    5,5,5,5,5,5,5,8,
    6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,
    6,6,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7};

UINT8 gpio_pin_map[GPIO_PIN_MAX] = {
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    16,17,0,1,2,3,4,5,
    6,7,8,9,10,11,12,13};

UINT8 gpio_num_map[MAX_GPIO_PORT][MAX_GPIO_PIN] = {
    // port 0
    {0,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15},
    // port 1
    {16,17,18,19,20,21,22,23,
    24,25,26,27,28,29,30,31},
    // port 2
    {32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47},
    // port 3
    {48,49,50,51,52,53,54,55,
    56,57,58,59,60,61,62,63},
    // port 4
    {64,65,66,67,68,69,70,71,
    72,73,74,75,76,77,78,79},
    // port 5
    {80,81,82,83,84,85,86,87,
     88,89,90,91,92,93,94,95},
    // port 6
    {96,97,98,99,100,101,102,103,
     104,105,106,107,108,109,110,111,112,113},
    // port 7
    {114,115,116,117,118,119,120,121,
     122,123,124,125,126,127}
    };

// port address mapping
// data-in reg mapping
UINT32 gpio_port_datain_addr[MAX_GPIO_PORT] = 
{
    (UINT32) GPIO_REG_PORT0_DATAIN_ADDR,
    (UINT32) GPIO_REG_PORT1_DATAIN_ADDR,
    (UINT32) GPIO_REG_PORT2_DATAIN_ADDR,
    (UINT32) GPIO_REG_PORT3_DATAIN_ADDR,
    (UINT32) GPIO_REG_PORT4_DATAIN_ADDR,
    (UINT32) GPIO_REG_PORT5_DATAIN_ADDR,
    (UINT32) GPIO_REG_PORT6_DATAIN_ADDR,
    (UINT32) GPIO_REG_PORT7_DATAIN_ADDR
};

// data-out reg mapping
UINT32 gpio_port_dataout_addr[MAX_GPIO_PORT] = 
{
    (UINT32) GPIO_REG_PORT0_DATAOUT_ADDR,
    (UINT32) GPIO_REG_PORT1_DATAOUT_ADDR,
    (UINT32) GPIO_REG_PORT2_DATAOUT_ADDR,
    (UINT32) GPIO_REG_PORT3_DATAOUT_ADDR,
    (UINT32) GPIO_REG_PORT4_DATAOUT_ADDR,
    (UINT32) GPIO_REG_PORT5_DATAOUT_ADDR,
    0,
    (UINT32) GPIO_REG_PORT7_DATAOUT_ADDR
};

// data direction reg mapping
UINT32 gpio_port_datamode_addr[MAX_GPIO_PORT] = 
{
    (UINT32) GPIO_REG_PORT0_CFG0_ADDR,
    (UINT32) GPIO_REG_PORT1_CFG0_ADDR,
    (UINT32) GPIO_REG_PORT2_CFG0_ADDR,
    (UINT32) GPIO_REG_PORT3_CFG0_ADDR,
    (UINT32) GPIO_REG_PORT4_CFG0_ADDR,
    (UINT32) GPIO_REG_PORT5_CFG0_ADDR,
    0, // port 6 is read only
    (UINT32) GPIO_REG_PORT7_CFG0_ADDR
};

// open drain reg mapping
UINT32 gpio_port_opendrain_addr[MAX_GPIO_PORT] = 
{
    (UINT32) GPIO_REG_PORT0_CFG1_ADDR,
    (UINT32) GPIO_REG_PORT1_CFG1_ADDR,
    (UINT32) GPIO_REG_PORT2_CFG1_ADDR,
    (UINT32) GPIO_REG_PORT3_CFG1_ADDR,
    (UINT32) GPIO_REG_PORT4_CFG1_ADDR,
    (UINT32) GPIO_REG_PORT5_CFG1_ADDR,
    0, // port 6 is read only
    (UINT32) GPIO_REG_PORT7_CFG1_ADDR
};

// power source reg mapping
UINT32 gpio_port_power_addr[MAX_GPIO_PORT] = 
{
    0,
    (UINT32) GPIO_REG_PORT1_CFG2_ADDR,
    (UINT32) GPIO_REG_PORT2_CFG2_ADDR,
    (UINT32) GPIO_REG_PORT3_CFG2_ADDR,
    (UINT32) GPIO_REG_PORT4_CFG2_ADDR,
    (UINT32) GPIO_REG_PORT5_CFG2_ADDR,
    0, // port 6 is read only
    (UINT32) GPIO_REG_PORT7_CFG2_ADDR
};

// power source reg mapping
UINT32 gpio_port_pxpe_addr[MAX_GPIO_PORT] = 
{
    0,
    (UINT32) GLOBAL_REG_GPIOP1PE_ADDR,
    (UINT32) GLOBAL_REG_GPIOP2PE_ADDR,
    (UINT32) GLOBAL_REG_GPIOP3PE_ADDR,
    0,
    (UINT32) GLOBAL_REG_GPIOP5PE_ADDR,
    (UINT32) GLOBAL_REG_GPIOP6PE_ADDR,
    (UINT32) GLOBAL_REG_GPIOP7PE_ADDR
};

/******************************************************************************
*   STRUCT      :   sGPIOData
******************************************************************************/
/**
 *  @brief   Structure to GPIO driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
    /* Set read/write command type */
    UINT8 u8CommandType;
    
    /* Set command number */
    UINT8 u8CommandNum; 
    
    /* Set GPIO port number */
    UINT8 u8PortNum;       
    
    /* Set GPIO Pin number */
    UINT8 u8PinNum;        
    
    /* Data buffer */
    void *pData;                

} sGPIOData;

/******************************************************************************
*   STRUCT      :   sGPIOSelect
******************************************************************************/
/**
 *  @brief   Structure to GPIO driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
    /* multiple function pin select register address */
    UINT32 u32RegAddr;
    
    /* gpio enable bit */
    UINT8 u8RegBit; 
    
    /* gpio enable value */
    UINT8 u8BitEnableValue; 
} sGPIOSelect;

/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
static int aess_gpio_write( 
                           /* GPIO Data Structure */
                           sGPIOData *psdata
                          );
  

static int aess_gpio_read( 
                          /* GPIO Data Structure */
                          sGPIOData *psdata
                         );

static int aess_gpio_config( 
                            /* GPIO Data Structure */
                            sGPIOData *psdata
                           );

int aess_gpio_commander(
                         /*bit 7~4->port, 3-0->pin */
                         UINT8 u8PortPin,  
                         /* Reference to command list */
                         UINT8 u8CommandType,
                         /* Reference to command list */
                         UINT8 u8CommandNum,
                         /* Data buffer, right now is UINT8 */
                         void *pData
                       );

#else
/* For external API to use */
extern int aess_gpio_commander(
                                  /* bit 7~4->port, 3-0->pin */
                                  UINT8 u8PortPin, 
                                  /* Reference to command list */
                                  UINT8 u8CommandType,
                                  /* Reference to command list */
                                  UINT8 u8CommandNum,
                                  /* Data buffer, right now is UINT8 */
                                  void *pData
                                );
#endif   /* AESSGPIODRV_C */

#endif   /* AESSGPIODRV_H */
