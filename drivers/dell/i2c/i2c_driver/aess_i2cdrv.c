/*
 * $RCSfile: aess_i2cdrv.c,v $
 * $Revision: 1.11 $
 * $Date: 2008/10/08 12:34:40 $
 * $Author: kellyhung $
 *
 * WPCM450 I2C driver.
 *  
 * Copyright (C) 2008 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSI2CDRV_C
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>
#include <asm/arch/hardware.h>

#include "aess_i2cdrv.h"
#include "../../eventhandler/eventhandler_driver/aess_eventhandlerdrv.h"
#include "../../gpio/gpio_driver/aess_gpiodrv.h"

#include <linux/timer.h>
#include <linux/jiffies.h>

#include <linux/kfifo.h>
#include <asm/atomic.h>
#include <asm/bitops.h>


/* change to the highest message level */
#if 0
#define I2C_TESTING KERN_DEBUG
#else
#define I2C_TESTING
#endif

/* enable to print out message */
#if 0
#define DEBUGP(format, args...) printk(I2C_TESTING format, ## args)
#else
#define DEBUGP(format, args...)
#endif

/* enable a background timer to poll statuses of all initiated I2C buses */
#if 0
#define I2C_POLLING_STATUS
#endif

#if 0
#define I2C_BUS_RECOVER
#endif

#if 1
#define I2C_MUX_WAIT_BUS_FREE
#endif

/* Base Address Definition */
#define AIC_BASE_ADDRESS		(WPCM450_VA_AIC)	
#define SGCR_BASE_ADDRESS		(WPCM450_VA_GCR)
#define CLK_BASE_ADDRESS    (WPCM450_VA_GCR + 0x200)
#define SMB0_BASE_ADDRESS		(WPCM450_VA_SMB0)
#define SMB1_BASE_ADDRESS		(SMB0_BASE_ADDRESS + 0x100)
#define SMB2_BASE_ADDRESS		(SMB0_BASE_ADDRESS + 0x200)
#define SMB3_BASE_ADDRESS		(SMB0_BASE_ADDRESS + 0x300)
#define SMB4_BASE_ADDRESS		(SMB0_BASE_ADDRESS + 0x400)
#define SMB5_BASE_ADDRESS		(SMB0_BASE_ADDRESS + 0x500)


/* Register type offsets in the module */
#define SGCR_MFSEL1         0x0C
#define CLK_CLKEN           0x00
#define CLK_CLKSEL          0x04
#define CLK_CLKDIV          0x08
#define CLK_PLLCON0         0x0C
#define CLK_PLLCON1         0x10

#define AIC_GEN				0x84
#define AIC_GRSR			0x88
#define SMB_SDA				0x00
#define SMB_ST				0x02
#define SMB_CST				0x04
#define SMB_CTL1			0x06
#define SMB_ADDR			0x08
#define SMB_CTL2			0x0A
#define SMB_ADDR2			0x0C
#define SMB_CTL3			0x0E

/* Device Register Definition */
#define AIC_GEN_REG				(AIC_BASE_ADDRESS + AIC_GEN)
#define AIC_GRSR_REG			(AIC_BASE_ADDRESS + AIC_GRSR)
#define SGCR_MFSEL1_REG		(SGCR_BASE_ADDRESS + SGCR_MFSEL1)

#define CLK_CLKEN_REG			(CLK_BASE_ADDRESS + CLK_CLKEN)
#define CLK_CLKSEL_REG		(CLK_BASE_ADDRESS + CLK_CLKSEL)
#define CLK_CLKDIV_REG		(CLK_BASE_ADDRESS + CLK_CLKDIV)
#define CLK_PLLCON0_REG		(CLK_BASE_ADDRESS + CLK_PLLCON0)
#define CLK_PLLCON1_REG 	(CLK_BASE_ADDRESS + CLK_PLLCON1)

#define I2C0_SDA_REG			(SMB0_BASE_ADDRESS + SMB_SDA)
#define I2C0_ST_REG				(SMB0_BASE_ADDRESS + SMB_ST)
#define I2C0_CST_REG			(SMB0_BASE_ADDRESS + SMB_CST)
#define I2C0_CTL1_REG			(SMB0_BASE_ADDRESS + SMB_CTL1)
#define I2C0_ADDR1_REG		(SMB0_BASE_ADDRESS + SMB_ADDR)
#define I2C0_CTL2_REG			(SMB0_BASE_ADDRESS + SMB_CTL2)
#define I2C0_ADDR2_REG		(SMB0_BASE_ADDRESS + SMB_ADDR2)
#define I2C0_CTL3_REG			(SMB0_BASE_ADDRESS + SMB_CTL3)

#define I2C1_SDA_REG			(SMB1_BASE_ADDRESS + SMB_SDA)  
#define I2C1_ST_REG				(SMB1_BASE_ADDRESS + SMB_ST)   
#define I2C1_CST_REG			(SMB1_BASE_ADDRESS + SMB_CST)  
#define I2C1_CTL1_REG			(SMB1_BASE_ADDRESS + SMB_CTL1) 
#define I2C1_ADDR1_REG		(SMB1_BASE_ADDRESS + SMB_ADDR) 
#define I2C1_CTL2_REG			(SMB1_BASE_ADDRESS + SMB_CTL2) 
#define I2C1_ADDR2_REG 		(SMB1_BASE_ADDRESS + SMB_ADDR2)
#define I2C1_CTL3_REG			(SMB1_BASE_ADDRESS + SMB_CTL3) 

#define I2C2_SDA_REG			(SMB2_BASE_ADDRESS + SMB_SDA)  
#define I2C2_ST_REG				(SMB2_BASE_ADDRESS + SMB_ST)   
#define I2C2_CST_REG			(SMB2_BASE_ADDRESS + SMB_CST)  
#define I2C2_CTL1_REG			(SMB2_BASE_ADDRESS + SMB_CTL1) 
#define I2C2_ADDR1_REG		(SMB2_BASE_ADDRESS + SMB_ADDR) 
#define I2C2_CTL2_REG			(SMB2_BASE_ADDRESS + SMB_CTL2) 
#define I2C2_ADDR2_REG		(SMB2_BASE_ADDRESS + SMB_ADDR2)
#define I2C2_CTL3_REG			(SMB2_BASE_ADDRESS + SMB_CTL3) 

#define I2C3_SDA_REG			(SMB3_BASE_ADDRESS + SMB_SDA)  
#define I2C3_ST_REG				(SMB3_BASE_ADDRESS + SMB_ST)   
#define I2C3_CST_REG			(SMB3_BASE_ADDRESS + SMB_CST)  
#define I2C3_CTL1_REG			(SMB3_BASE_ADDRESS + SMB_CTL1) 
#define I2C3_ADDR1_REG		(SMB3_BASE_ADDRESS + SMB_ADDR) 
#define I2C3_CTL2_REG			(SMB3_BASE_ADDRESS + SMB_CTL2) 
#define I2C3_ADDR2_REG		(SMB3_BASE_ADDRESS + SMB_ADDR2)
#define I2C3_CTL3_REG			(SMB3_BASE_ADDRESS + SMB_CTL3) 

#define I2C4_SDA_REG			(SMB4_BASE_ADDRESS + SMB_SDA)  
#define I2C4_ST_REG				(SMB4_BASE_ADDRESS + SMB_ST)   
#define I2C4_CST_REG			(SMB4_BASE_ADDRESS + SMB_CST)  
#define I2C4_CTL1_REG			(SMB4_BASE_ADDRESS + SMB_CTL1) 
#define I2C4_ADDR1_REG 		(SMB4_BASE_ADDRESS + SMB_ADDR) 
#define I2C4_CTL2_REG			(SMB4_BASE_ADDRESS + SMB_CTL2) 
#define I2C4_ADDR2_REG		(SMB4_BASE_ADDRESS + SMB_ADDR2)
#define I2C4_CTL3_REG			(SMB4_BASE_ADDRESS + SMB_CTL3) 

#define I2C5_SDA_REG			(SMB5_BASE_ADDRESS + SMB_SDA)  
#define I2C5_ST_REG				(SMB5_BASE_ADDRESS + SMB_ST)   
#define I2C5_CST_REG			(SMB5_BASE_ADDRESS + SMB_CST)  
#define I2C5_CTL1_REG			(SMB5_BASE_ADDRESS + SMB_CTL1) 
#define I2C5_ADDR1_REG		(SMB5_BASE_ADDRESS + SMB_ADDR) 
#define I2C5_CTL2_REG			(SMB5_BASE_ADDRESS + SMB_CTL2) 
#define I2C5_ADDR2_REG		(SMB5_BASE_ADDRESS + SMB_ADDR2)
#define I2C5_CTL3_REG			(SMB5_BASE_ADDRESS + SMB_CTL3) 

/* definitions for status register */
#define I2C_SLAVE_STOP          0x80
#define I2C_SDA_STATUS          0x40
#define I2C_BUS_ERROR           0x20
#define I2C_NEGATIVE_ACK        0x10
#define I2C_STALL_AFTER_START   0x08
#define I2C_NEW_MATCH           0x04
#define I2C_MASTER_MODE         0x02
#define I2C_TRANSMIT_MODE       0x01

/* definitions for control status register */
#define I2C_ARP_ADDR_MATCH      0x80
#define I2C_MATCH_SDDR_FIELD    0x40
#define I2C_TOGGLE_SCL_LINE     0x20
#define I2C_TEST_SDA_LINE       0x10
#define I2C_GLOBAL_CALL_MATCH   0x08
#define I2C_ADDR_MATCH          0x04
#define I2C_BUS_BUSY            0x02
#define I2C_BUSY                0x01

/* definitions for control register */
#define I2C_STALL_AFTER_START_ENABLE    0x80
#define I2C_NEW_MATCH_INTERRUPT_ENABLE  0x40
#define I2C_GLOBAL_CALL_MATCH_ENABLE    0x20
#define I2C_ACK                         0x10
#define I2C_INTERRUPT_ENABLE            0x04
#define I2C_STOP                        0x02
#define I2C_START                       0x01

/* definitions for control1 register */
#define I2C_STALL_AFTER_START_ENABLE    0x80
#define I2C_NEW_MATCH_INTERRUPT_ENABLE  0x40
#define I2C_GLOBAL_CALL_MATCH_ENABLE    0x20
#define I2C_ACK                         0x10
#define I2C_INTERRUPT_ENABLE            0x04
#define I2C_STOP                        0x02
#define I2C_START                       0x01

/* definitions for control2 register */
#define I2C_SCL_FREQ_BITS_6_TO_0    0xFE
#define I2C_ENABLE                  0x01

/* definitions for control3 register */
#define I2C_ARP_MATCH_ENABLE        0x04
#define I2C_SCL_FREQ_BITS_8_TO_7    0x03

/* definitions for address register */
#define I2C_SLAVE_ADDR_ENABLE       0x80
#define I2C_SLAVE_ADDR              0x7F

/* definitions for AIC_GEN register bit */
#define AIC_GEN_SMB0	26
#define AIC_GEN_SMB1	27
#define AIC_GEN_SMB2	30

/* definitions for status register bit */
#define I2C_SLAVE_STOP_BIT          7
#define I2C_SDA_STATUS_BIT          6
#define I2C_BUS_ERROR_BIT           5
#define I2C_NEGATIVE_ACK_BIT        4
#define I2C_STALL_AFTER_START_BIT   3
#define I2C_NEW_MATCH_BIT           2
#define I2C_MASTER_MODE_BIT         1
#define I2C_TRANSMIT_MODE_BIT       0

/* definitions for control status register bit */
#define I2C_ARP_ADDR_MATCH_BIT      7
#define I2C_MATCH_SDDR_FIELD_BIT    6
#define I2C_TOGGLE_SCL_LINE_BIT     5
#define I2C_TEST_SDA_LINE_BIT       4
#define I2C_GLOBAL_CALL_MATCH_BIT   3
#define I2C_ADDR_MATCH_BIT          2
#define I2C_BUS_BUSY_BIT            1
#define I2C_BUSY_BIT                0

/* definitions for control register bit */
#define I2C_STALL_AFTER_START_ENABLE_BIT    7
#define I2C_NEW_MATCH_INTERRUPT_ENABLE_BIT  6
#define I2C_GLOBAL_CALL_MATCH_ENABLE_BIT    5
#define I2C_ACK_BIT                         4
#define I2C_INTERRUPT_ENABLE_BIT            2
#define I2C_STOP_BIT                        1
#define I2C_START_BIT                       0

/* definitions for control2 register bit */
#define I2C_ENABLE_BIT                  0

/* definitions for address register bit */
#define I2C_SLAVE_ADDR_ENABLE_BIT       7

/* definitions for MUTI-Function Pin Selector */
#define I2C0_SELECT_BIT         6
#define I2C1_SELECT_BIT         7
#define I2C2_SELECT_BIT         8
#define I2C3_SELECT_BIT         0
#define I2C4_SELECT_BIT         1
#define I2C5_SELECT_BIT         2

/* definitions for I2C Bus Speed Fequency */
#define I2C_DRV_FREQ_100            0
#define I2C_DRV_FREQ_400            1
#define I2C_DRV_FREQ_50             2

/* definitions for bus status */
#define I2C_SET_BUS_STOP_BIT        0x10
#define I2C_SET_BUS_ERROR_BIT       0x08
#define I2C_SET_BUS_BUSY_BIT        0x04

/* definitions for control hardware */
#define I2C_DRV_REC_RESET_SMBUS     0x08
#define I2C_DRV_REC_CLK_PULSE       0x04
#define I2C_DRV_REC_RESET_OUT       0x02
#define I2C_DRV_REC_FORCE_STOP      0x01

#define CLEAR_ALL_BITS	0x00
  		
/* APB Clock Frequency in Hz units  (55MHz) */
#define APB_CLK   ((UINT32) 55000000) 

#define I2C_DRV_CLK_STR_WAIT_50US   50

#define I2C_SCL_FEQ_50KHZ           ((UINT32) 50000)
#define I2C_SCL_FEQ_100KHZ          ((UINT32) 100000)
#define I2C_SCL_FEQ_400KHZ          ((UINT32) 420000)

#define I2C_DRV_TRANS_NORMAL        0
#define I2C_DRV_TRANS_MUX           1
#define I2C_DRV_TRANS_SMBUS_BLK     2

#define I2C_2MS_UDELAY_TICKS  			2000
/******************************************************************************
*   Enum      :   eI2CDrvErrorType
******************************************************************************/
/**
 *  @brief   I2C driver error enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Successful */
	I2C_DRV_OK = 0,
	
	/** General failure */
	I2C_DRV_FAIL,
	
	/** Memory allocation failed */
	I2C_DRV_MEM_FAIL,
	
	/** Memory pool creation failed */
	I2C_DRV_POOL_FAIL,
	
	/** Queue creation failed */
	I2C_DRV_Q_FAIL,
	
	/** Event flags group creation failed */
	I2C_DRV_FLAGS_FAIL,
	
	/** Semaphore creation failed */
	I2C_DRV_SEM_FAIL,
	
	/** Timer creation failed */
	I2C_DRV_TIMER_FAIL
} eI2CDrvErrorType;

/******************************************************************************
*   Enum      :   eI2CDrvErrorStatusType
******************************************************************************/
/**
 *  @brief   I2C driver error status enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** No errors */
	I2C_DRV_ERROR_NONE = 0,
	
	/** NACK error */
	I2C_DRV_ERROR_NACK,
	
	/** Arbitration loss error */
	I2C_DRV_ERROR_AL,
	
	/** Start error */
	I2C_DRV_ERROR_START,
	
	/** Stop error */
	I2C_DRV_ERROR_STOP,
	
	/** bus busy */
	I2C_DRV_ERROR_BUSY,
	
	/** Timeout error */
	I2C_DRV_ERROR_TIMEOUT,
	
	/** General bus error */
	I2C_DRV_ERROR_BUS,
	
	/** Buffer overflow error */
	I2C_DRV_ERROR_OVRFL,
	
	/** Bus reet error */
	I2C_DRV_ERROR_RESET,
	
	/** Bus protocol error */
	I2C_DRV_ERROR_PROTOCOL	
} eI2CDrvErrorStatusType;

/******************************************************************************
*   Enum      :   eI2CDrvStateType
******************************************************************************/
/**
 *  @brief   I2C driver state machine enumeration
 *
 *****************************************************************************/
typedef enum
{
	/** Idle state */
	I2C_DRV_IDLE_STATE,
	
	/** Slave receive */
	I2C_DRV_SLV_RCV_STATE,
	
	/** Slave receive FIFO */
	I2C_DRV_SLV_RCV_FIFO_STATE,
	
	/** Slave transmit */
	I2C_DRV_SLV_XMIT_STATE,
	
	/** Master receive */
	I2C_DRV_MST_RCV_STATE,
	
	/** Master receive dummy read */
	I2C_DRV_MST_RCV_DUMMY_STATE,
	
	/** Master transmit */
	I2C_DRV_MST_XMIT_STATE,
	
	/** Start state */
	I2C_DRV_START_STATE,
	
	/** Stop state */
	I2C_DRV_STOP_STATE,
	
	/** Master transmit done */
	I2C_DRV_MST_XMIT_DONE_STATE,	
	
	/** Master receive done */
	I2C_DRV_MST_RCV_DONE_STATE,
	
	/** Slave receive done */
	I2C_DRV_SLV_RCV_DONE_STATE
} eI2CDrvStateType;

/******************************************************************************
*   Enum      :   eI2CDrvModeType
******************************************************************************/
/**
 *  @brief   I2C driver mode enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Private I2C mode */
	I2C_DRV_MODE_PI2C = 0,
	
	/** IPMB mode */
	I2C_DRV_MODE_IPMB
} eI2CDrvModeType;

/******************************************************************************
*   Enum      :   eI2CDrvCtrlHardwareType
******************************************************************************/
/**
 *  @brief   I2C driver control hardware enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Pulse the SCL manually and generate a stop condition */
	I2C_DRV_CLK_PULSE,
	
	/** Generate a stop condition */
	I2C_DRV_FORCE_STOP,
	
	/** Issue SMBus 2.0 reset pulse (SCL low for > 30ms */
	I2C_DRV_RESET_SMBUS
} eI2CDrvCtrlHardwareType;


#define I2C_DRV_MAX_BUS             6
#define I2C_DRV_MAX_PI2C_BUS        I2C_DRV_MAX_BUS
#define I2C_DRV_MAX_IPMB_BUS        I2C_DRV_MAX_BUS

#define I2C_DRV_MAX_FIFO_SIZE       4

#define I2C_DRV_BUS_0               0
#define I2C_DRV_BUS_1               1
#define I2C_DRV_BUS_2               2
#define I2C_DRV_BUS_3               3
#define I2C_DRV_BUS_4               4
#define I2C_DRV_BUS_5               5

#define I2C_DRV_MAX_MSG_SIZE        0xFF + 1
#define I2C_DRV_IPMB_MSG_MIN        7

/******************************************************************************
*   STRUCT      :   sI2CDrvBufInfoType
******************************************************************************/
/**
 *  @brief   structure of I2C driver message buffer information
 *
 *****************************************************************************/
typedef struct
{
	/** Message transmit buffer */
	UINT8 *pu8MsgSendBuffer;

	/** Message receive buffer */
	UINT8 *pu8MsgRecBuffer;

	/** Reserved */	
	UINT16 u16Reserved;

	/** I2C bus channel */
	UINT8 u8Channel;
	
	/** device slave address */
	UINT8 u8DeviceAddr;
	
	/** Bus error status */
	UINT8 u8ErrorStatus;
	
	/** Message transmit data length */
	UINT8 u8MsgSendDataSize;
	
	/** Message receive data length */
	UINT8 u8MsgRecDataSize;

	/** Trans. type setting */
	UINT8 u8TransType;	

} sI2CDrvBufInfoType;


/******************************************************************************
*   STRUCT      :   sI2CDrvBusInfoType
******************************************************************************/
/**
 *  @brief   structure of I2C driver bus information
 *
 *****************************************************************************/
typedef struct
{
	/** IPMB msg received flag */
	UINT32 u32RecFlag;

	/** ID for kernel event handler */
	UINT16 u16DriverID;
	
	/** Current start tracking count */
	UINT16 u16CurStartCount;
	
	/** Current stop tracking count */
	UINT16 u16CurStopCount;

	/** I2C bus channel */
	UINT8 u8Channel;
	
	/** Initial mode (IPMB or PI2C) */
	UINT8 u8InitMode;
	
	/** Current mode (IPMB or PI2C) */
	UINT8 u8CurMode;
	
	/** IPMB slave address */
	UINT8 u8SlaveAddr;
	
	/** I2C bus frequency selection */
	UINT8 u8Frequency;
	
	/** Bus error status */
	UINT8 u8ErrorStatus;
	
	/** Bus status */
	UINT8 u8BusStatus;
	
	/** Control hardware */
	UINT8 u8CtrlHW;

	/** Trans. type setting */
	UINT8 u8TransType;	
	
	/** Reserved */	
	UINT8 u8Reserved;
	
} sI2CDrvBusInfoType;


/******************************************************************************
*   STRUCT      :   sI2CDrvSlvRecBufType
******************************************************************************/
/**
 *  @brief   structure of I2C slave receive buffer
 *
 *****************************************************************************/
 typedef struct
{
	/** Buffer */
	UINT8 u8buffer[I2C_DRV_MAX_MSG_SIZE];
	
	/** Message length */
	UINT8 u8MsgLen;	
	
} sI2CDrvSlvRecBufType;


/******************************************************************************
*   STRUCT      :   sI2CDrvBufferType
******************************************************************************/
/**
 *  @brief   structure of I2C driver buffer
 *
 *****************************************************************************/
typedef struct
{
	/** Access semaphore */
	struct semaphore sem;
	    
	/** IPMB msg received flag */
	UINT32 u32RecFlag;

	/** Spin lock */
	spinlock_t lock;
	
	/* Message receive FIFO */
	struct kfifo *psFIFO;
	
	/** Wait queue */
	wait_queue_head_t wq;

	/** Driver ID for kernel event handler */
	UINT16 u16DriverID;

	/** Current start tracking count */
	UINT16 u16CurStartCount;

	/** Current stop tracking count */
	UINT16 u16CurStopCount;
	    
	/** I2C bus channel */
	UINT8 u8Channel;
	
	/** Initial mode (IPMB or PI2C) */
	UINT8 u8InitMode;
	
	/** Current mode (IPMB or PI2C) */
	UINT8 u8CurMode;
	
	/** IPMB slave address */
	UINT8 u8SlaveAddr;
	
	/** I2C bus frequency selection */
	UINT8 u8Frequency;
	
	/** Bus initialization status */
	UINT8 u8HasInit;
	
	/** State machine variable */
	UINT8 u8State;
	
	/** Bus error status */
	UINT8 u8ErrorStatus;
	
	/** I2C bus action finished flag */
	UINT8 u8FinishedFlag;
	
	/** Message transmit buffer */
	UINT8 u8MsgSendBuffer[I2C_DRV_MAX_MSG_SIZE];
	
	/** Message transmit counter */
	UINT8 u8MsgSendCounter;
	
	/** Message transmit data length */
	UINT8 u8MsgSendDataSize;
	
	/** Message receive buffer */
	UINT8 u8MsgRecBuffer[I2C_DRV_MAX_MSG_SIZE];
	
	/** Message receive counter */
	UINT8 u8MsgRecCounter;
	
	/** Message receive data length */
	UINT8 u8MsgRecDataSize;
	
	/** Condition checking for wait queue */
	UINT8 wq_flag;

	/** Trans. type setting */
	UINT8 u8TransType;	
	
	/** Reserved */	
	UINT8 u8Reserved1;

	/** Reserved */	
	UINT8 u8Reserved2;
	
	/** Reserved */	
	UINT8 u8Reserved3;	
		
} sI2CDrvBufferType;


/******************************************************************************
*   STRUCT      :   i2c_reg_type
******************************************************************************/
/**
 *  @brief   structure of I2C hardware register
 *
 *****************************************************************************/
typedef struct 
{
  UINT32 serial_data;
  UINT32 status;
  UINT32 control_status;
  UINT32 control1;
  UINT32 control2;  
  UINT32 control3;  
  UINT32 own_address1;
  UINT32 own_address2;  
} i2c_reg_type;

/* i2c hardware register structure array */
volatile static i2c_reg_type i2c_reg[I2C_DRV_MAX_BUS] = {
            {
                (UINT32) I2C0_SDA_REG,
                (UINT32) I2C0_ST_REG,
                (UINT32) I2C0_CST_REG,
                (UINT32) I2C0_CTL1_REG,
                (UINT32) I2C0_CTL2_REG,
                (UINT32) I2C0_CTL3_REG, 
                (UINT32) I2C0_ADDR1_REG,                               
                (UINT32) I2C0_ADDR2_REG
            },
            {
                (UINT32) I2C1_SDA_REG,
                (UINT32) I2C1_ST_REG,
                (UINT32) I2C1_CST_REG,
                (UINT32) I2C1_CTL1_REG,
                (UINT32) I2C1_CTL2_REG,
                (UINT32) I2C1_CTL3_REG, 
                (UINT32) I2C1_ADDR1_REG,                               
                (UINT32) I2C1_ADDR2_REG
            },
            {
                (UINT32) I2C2_SDA_REG,
                (UINT32) I2C2_ST_REG,
                (UINT32) I2C2_CST_REG,
                (UINT32) I2C2_CTL1_REG,
                (UINT32) I2C2_CTL2_REG,
                (UINT32) I2C2_CTL3_REG, 
                (UINT32) I2C2_ADDR1_REG,                               
                (UINT32) I2C2_ADDR2_REG
            },
            {
                (UINT32) I2C3_SDA_REG,
                (UINT32) I2C3_ST_REG,
                (UINT32) I2C3_CST_REG,
                (UINT32) I2C3_CTL1_REG,
                (UINT32) I2C3_CTL2_REG,
                (UINT32) I2C3_CTL3_REG, 
                (UINT32) I2C3_ADDR1_REG,                               
                (UINT32) I2C3_ADDR2_REG
            },
            {
                (UINT32) I2C4_SDA_REG,
                (UINT32) I2C4_ST_REG,
                (UINT32) I2C4_CST_REG,
                (UINT32) I2C4_CTL1_REG,
                (UINT32) I2C4_CTL2_REG,
                (UINT32) I2C4_CTL3_REG, 
                (UINT32) I2C4_ADDR1_REG,                               
                (UINT32) I2C4_ADDR2_REG
            },
            {
                (UINT32) I2C5_SDA_REG,
                (UINT32) I2C5_ST_REG,
                (UINT32) I2C5_CST_REG,
                (UINT32) I2C5_CTL1_REG,
                (UINT32) I2C5_CTL2_REG,
                (UINT32) I2C5_CTL3_REG, 
                (UINT32) I2C5_ADDR1_REG,                               
                (UINT32) I2C5_ADDR2_REG
            }                                                          
};


/* driver name will passed by insmod command */
static char *driver_name="aess_i2cdrv";

/* record the kernel i2c initial state */
static int i2c_init_state = 0;
static spinlock_t init_state_lock;

volatile sI2CDrvBufferType i2c_buffer[I2C_DRV_MAX_BUS];

#ifdef I2C_BUS_RECOVER
struct timer_list i2c_bus_recover;
#endif

#ifdef I2C_POLLING_STATUS
struct timer_list i2c_polling;
#endif

dev_t i2c_dev;
struct cdev *i2c_cdev;



/******************************************************************************
*   FUNCTION        :   set_bit_8
******************************************************************************/
/**
 *  @brief      set specific bit to 1 for byte register(len = 8)
 *
 *  @return     OK
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   None\n
 *
 *****************************************************************************/
void set_bit_8(

                /** which bit would like to set to 1 */
                int bit_number,
                
                /** target address which do bit setting */
                void *register_addr
                
                                                )
{
    UINT8 u8RegReading;
    UINT8 u8BitOperation;
    
    u8BitOperation = (UINT8)(1 << bit_number);
    
    u8RegReading = ioread8((void *) register_addr);  
    u8RegReading |= u8BitOperation;
    iowrite8(u8RegReading, (void *) register_addr);
    
    return;
}

/******************************************************************************
*   FUNCTION        :   clear_bit_8
******************************************************************************/
/**
 *  @brief      clear specific bit for byte register(len = 8)
 *
 *  @return     OK
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   None\n
 *
 *****************************************************************************/
int clear_bit_8(

                /** which bit would like to set to 0 */
                int bit_number,
                
                /** target address which do bit clear */
                void *register_addr
                
                                                )
{
    UINT8 u8RegReading;
    UINT8 u8BitOperation;
    
    u8BitOperation = (UINT8)(1 << bit_number);
    
    u8RegReading = ioread8((void *) register_addr);  
    u8RegReading &= (~u8BitOperation);
    iowrite8(u8RegReading, (void *) register_addr);
    
    return true;
}

/******************************************************************************
*   FUNCTION        :   test_bit_8
******************************************************************************/
/**
 *  @brief      get specific bit value for byte register(len = 8)
 *
 *  @return     bit value
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   None\n
 *
 *****************************************************************************/
int test_bit_8(

                /** which bit would like to set to 0 */
                int bit_number,
                
                /** target address which do bit clear */
                void *register_addr
                
                                                )
{
    UINT8 u8RegReading;
    UINT8 u8BitOperation;
    
    u8BitOperation = (UINT8)(1 << bit_number);
    
    u8RegReading = ioread8((void *) register_addr);  
    u8RegReading &= u8BitOperation;
    
    if (u8RegReading == 0)
        return 0;
    else
        return 1;
}

/******************************************************************************
*   FUNCTION        :   aess_i2c_clear_multifun_bit
******************************************************************************/
/**
 *  @brief      clear multifun bit to change to GPIO function. 
 *
 *  @return     OK
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Call from aess_i2c_config()\n
 *
 *****************************************************************************/
static int aess_i2c_clear_multifun_bit(

                /** I2C Channel Number */
                UINT8 u8BusNumber
                
                                                )
{

    volatile UINT32 u32RegReading;			

    DEBUGP("KN:Bus(%d) aess_i2c_clear_multifun_bit() enter! \n", u8BusNumber);	
    
    /* set multi function pin select */
    u32RegReading = ioread32((void *) SGCR_MFSEL1_REG);

    switch (u8BusNumber)
    {
        
        case I2C_DRV_BUS_0:
            clear_bit(I2C0_SELECT_BIT,&u32RegReading);	   	
            break;
            
        case I2C_DRV_BUS_1:
            clear_bit(I2C1_SELECT_BIT,&u32RegReading);	 
            break;
            
        case I2C_DRV_BUS_2:
            clear_bit(I2C2_SELECT_BIT,&u32RegReading);	    	
            break;   	  
            
        case I2C_DRV_BUS_3:
            clear_bit(I2C3_SELECT_BIT,&u32RegReading);	    	
            break;   	  
            
        case I2C_DRV_BUS_4:
            clear_bit(I2C4_SELECT_BIT,&u32RegReading);	    	
            break;   	  
            
        case I2C_DRV_BUS_5:
            clear_bit(I2C5_SELECT_BIT,&u32RegReading);	    	
            break;   	 
             
        default:
            DEBUGP("KN:Bus(%d) u8BusNumber Error!\n", u8BusNumber);	
            return 1;
            break;      
                      		   	  	
    }	
    
    iowrite32((UINT32) (u32RegReading), (void *) SGCR_MFSEL1_REG);
    DEBUGP("KN:Bus(%d) aess_i2c_clear_multifun_bit() data 0x%x\n", u8BusNumber, u32RegReading);	
    
    return 0;
    
}

/******************************************************************************
*   FUNCTION        :   aess_i2c_reset_SMBus
******************************************************************************/
/**
 *  @brief      pull SCL more than 25ms to reset SMBus. 
 *
 *  @return     OK
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   None\n
 *
 *****************************************************************************/
static int aess_i2c_reset_SMBus(

                /** I2C Channel Number */
                UINT8 u8BusNumber
                
                                                )
{
    /* according to wpcm450's datasheet */
    UINT8 u8SCL2GPIO[I2C_DRV_MAX_BUS] = {114, 116, 118, 31, 29, 27};
    UINT8 u8TmpBuf;

    DEBUGP("KN:Bus(%d) aess_i2c_reset_SMBus() enter!\n", u8BusNumber);	
    
    aess_i2c_clear_multifun_bit(u8BusNumber);
    u8TmpBuf = SET_GPIO_OUTPUT_LOW;
    aess_gpio_commander(u8SCL2GPIO[u8BusNumber], GPIO_WRITE, NORMAL_OUTPUT, (void *)&u8TmpBuf);
    /* delay > 35ms */
    mdelay(36);			      
    u8TmpBuf = SET_GPIO_OUTPUT_HIGH;
    aess_gpio_commander(u8SCL2GPIO[u8BusNumber], GPIO_WRITE, NORMAL_OUTPUT, (void *)&u8TmpBuf);
    
    return 0;
    
}

static int aess_i2c_config(sI2CDrvBufferType *psI2CDrvBuffer)
{
	UINT8 u8BusNumber;
	UINT16 u16SCLFrq;	
	UINT32 u32RegReading;			
	i2c_reg_type *CurrentReg;
	
	u8BusNumber = psI2CDrvBuffer->u8Channel;	
	CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
	DEBUGP("KN:Bus(%d) aess_i2c_config() enter!\n", u8BusNumber);	

	if (i2c_buffer[u8BusNumber].u8HasInit == 0)			
	{
		/* set muti function pin select */
		u32RegReading = ioread32((void *) SGCR_MFSEL1_REG);
  	
		switch (u8BusNumber)
		{
            case I2C_DRV_BUS_0:
                set_bit(I2C0_SELECT_BIT,&u32RegReading);	   	
                break;
            case I2C_DRV_BUS_1:
                set_bit(I2C1_SELECT_BIT,&u32RegReading);	 
                break;
            case I2C_DRV_BUS_2:
                set_bit(I2C2_SELECT_BIT,&u32RegReading);	    	
                break;   	  
            case I2C_DRV_BUS_3:
                set_bit(I2C3_SELECT_BIT,&u32RegReading);	    	
                break;   	  
            case I2C_DRV_BUS_4:
                set_bit(I2C4_SELECT_BIT,&u32RegReading);	    	
                break;   	  
            case I2C_DRV_BUS_5:
                set_bit(I2C5_SELECT_BIT,&u32RegReading);	    	
                break;   	  
			default:
                DEBUGP("KN: u8BusNumber Error!\n");	
                return 1;
			    break;                		   	  	
		}	
		iowrite32((UINT32) (u32RegReading), (void *) SGCR_MFSEL1_REG);		  
	}

	/* clear control1 register */
	iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->control1);
			  
	/* clear control2 register */			  
	iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->control2);			  

	/* clear control3 register */			  
	iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->control3);			

	/*--------------------------------------------------------------------------*
	* Set frequency:                                                           *
	*          SCLFRQ = T(SCL)/4/T(CLK) = FREQ(CLK)/4/FREQ(SCL)                *
	*--------------------------------------------------------------------------*/	

	#define BITS_0_TO_6_MASK 0x7f
	#define BITS_7_TO_8_MASK 0x180
  
	/* select clock frequency */
	if (psI2CDrvBuffer->u8Frequency == I2C_DRV_FREQ_100)
	{
		DEBUGP("KN:Bus(%d) aess_i2c_config() I2C_SCL_FEQ_100KHZ!\n",u8BusNumber );
		u16SCLFrq = (UINT16)(APB_CLK/((UINT32)I2C_SCL_FEQ_100KHZ)/4);   // bus_freq is KHz		
		DEBUGP("KN:Bus(%d) u16SCLFrq is 0x%04x\n",u8BusNumber,u16SCLFrq);
		iowrite8((UINT8)((u16SCLFrq & BITS_7_TO_8_MASK) >> 7) ,(void *) CurrentReg->control3);    
		iowrite8(((UINT8)((u16SCLFrq & BITS_0_TO_6_MASK)<<1))|I2C_ENABLE,(void *) CurrentReg->control2);    
	}
	else if (psI2CDrvBuffer->u8Frequency == I2C_DRV_FREQ_400)
	{
		DEBUGP("KN:Bus(%d) aess_i2c_config() I2C_SCL_FEQ_400KHZ!\n",u8BusNumber );
		u16SCLFrq = (UINT16)(APB_CLK/((UINT32)I2C_SCL_FEQ_400KHZ)/4);   // bus_freq is KHz			
		DEBUGP("KN:Bus(%d) u16SCLFrq is 0x%04xx\n",u8BusNumber,u16SCLFrq);    
		iowrite8((UINT8)((u16SCLFrq & BITS_7_TO_8_MASK) >> 7) ,(void *) CurrentReg->control3);    
		iowrite8(((UINT8)((u16SCLFrq & BITS_0_TO_6_MASK)<<1))|I2C_ENABLE,(void *) CurrentReg->control2);  	
	}
	else
	{
		DEBUGP("KN:Bus(%d) aess_i2c_config() I2C_SCL_FEQ_50KHZ!\n",u8BusNumber );
		u16SCLFrq = (UINT16)(APB_CLK/((UINT32)I2C_SCL_FEQ_50KHZ)/4);   // bus_freq is KHz			
		DEBUGP("KN:Bus(%d) u16SCLFrq is 0x%04xx\n",u8BusNumber,u16SCLFrq);    
		iowrite8((UINT8)((u16SCLFrq & BITS_7_TO_8_MASK) >> 7) ,(void *) CurrentReg->control3);    
		iowrite8(((UINT8)((u16SCLFrq & BITS_0_TO_6_MASK)<<1))|I2C_ENABLE,(void *) CurrentReg->control2);  	
	}           

	/* IPMB mode */
	if (i2c_buffer[u8BusNumber].u8CurMode == I2C_DRV_MODE_IPMB)
	{
		DEBUGP("KN: IPMB Mode psI2CDrvBuffer->u8SlaveAddr is 0x%02x!\n",psI2CDrvBuffer->u8SlaveAddr);		
		/* store slave address and enabled it */
		iowrite8((UINT8) (I2C_SLAVE_ADDR_ENABLE|(psI2CDrvBuffer->u8SlaveAddr >> 1)), 
						 (void *) CurrentReg->own_address1);

		/* enabled interrupt */
		iowrite8(0x44,(void *) CurrentReg->control1);					
	}
	else
	{
		/* enabled interrupt */
		iowrite8(0x04,(void *) CurrentReg->control1);			
	}
	
	/* set flag to indicate this bus has been initiated */
	psI2CDrvBuffer->u8HasInit = 1;
 
	DEBUGP("KN:Bus(%d) aess_i2c_config() exit!\n", u8BusNumber);			
	return 0;
}

static int aess_i2c_reset(sI2CDrvBufferType *psI2CDrvBuffer)
{
	UINT8 u8BusNumber;

	DEBUGP("KN: aess_i2c_reset(%d) enter!\n",psI2CDrvBuffer->u8Channel);	
	u8BusNumber = psI2CDrvBuffer->u8Channel;

	/* reset variable */
	if (i2c_buffer[u8BusNumber].u8InitMode == I2C_DRV_MODE_IPMB)
	{
		i2c_buffer[u8BusNumber].u8State = I2C_DRV_SLV_RCV_STATE;		
		DEBUGP("I2C Driver: aess_i2c_reset() kfifo_reset!\n");	
        		
		/* clear the memory */
		kfifo_reset(i2c_buffer[u8BusNumber].psFIFO);
	}
	else
	{
		i2c_buffer[u8BusNumber].u8State = I2C_DRV_IDLE_STATE;
	}
		
	i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_NONE;
	i2c_buffer[u8BusNumber].u8MsgSendDataSize = 0;
	i2c_buffer[u8BusNumber].u8MsgSendCounter = 0;
	i2c_buffer[u8BusNumber].u8MsgRecDataSize = 0;
	i2c_buffer[u8BusNumber].u8MsgRecCounter = 0;
	
	DEBUGP("KN: aess_i2c_reset(%d) exit!\n",u8BusNumber);
	  	
	return 0;	
}

void aess_i2c_disabled_interrupt(UINT8 u8BusNumber)
{
    i2c_reg_type *CurrentReg;

    CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
		    
    /* Disabled interrupt control */
    clear_bit_8(I2C_INTERRUPT_ENABLE_BIT ,(void *) CurrentReg->control1);	    
}

void aess_i2c_enabled_interrupt(UINT8 u8BusNumber)
{
    i2c_reg_type *CurrentReg;

    CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
		    
    /* Enabled interrupt control */
    set_bit_8(I2C_INTERRUPT_ENABLE_BIT, (void *) CurrentReg->control1);     
}


void aess_i2c_bus_recovery(UINT8 u8BusNumber)
{
	  UINT8 i = 0;
	  i2c_reg_type *CurrentReg;
    sI2CDrvBufferType *psI2CDrvBuffer;

	  CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];	  
	  psI2CDrvBuffer = (sI2CDrvBufferType *) &i2c_buffer[u8BusNumber];

    printk("KN:I2CBus(%d) aess_i2c_bus_recovery()\n", u8BusNumber);
    
	  aess_i2c_disabled_interrupt(u8BusNumber);

    if ((test_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status)) 
        || (test_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->control_status)))
		{					 			  

		     /* clear bus error bit */
        set_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status);

		    /* if i2c bus SDA line is low */			  
        if (!test_bit_8(I2C_TEST_SDA_LINE_BIT,(void *) CurrentReg->control_status))
		    {
		        #define NINE_CLOCK_PULSE	20
		
		        for (i=0; i < NINE_CLOCK_PULSE; i++)
		        {
		           /* toggle one clock */                  
                set_bit_8(I2C_TOGGLE_SCL_LINE_BIT,(void *) CurrentReg->control_status);  	
    
		           /* delay 5 us */
		           udelay(5);      				
		        }
		        
		        #undef NINE_CLOCK_PULSE
		     printk("KN:I2CBus(%d) aess_i2c_bus_recovery() Generate 7 Clock Pulse To Recovery I2C Bus!\n", u8BusNumber);		              	  			  	
		    }
		    
         /* delay 10ms */
         mdelay(10);			      
         
		    /* if i2c bus is busy */
        if (test_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->control_status))	
		    {

		        /* clear bus error bit */
            set_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status);

		        /* clear bus error bit */
            set_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->status);
		        
		        /* generate START condition */
            set_bit_8(I2C_START_BIT,(void *) CurrentReg->control1);	 
		        
		        /* set slave address 0x00 */
		        iowrite8((UINT32) (0x00), (void *) CurrentReg->serial_data);
		        
		        /* generate STOP condition */                  
            set_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control1);
		    
		     printk("KN:I2CBus(%d) aess_i2c_bus_recovery() Force Generate STOP Condition To Recovery I2C Bus!\n", u8BusNumber);
         
             /* delay 10ms */
             mdelay(100);
		    }
		    
		     /* reconfig i2c bus */
		     aess_i2c_config((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);			                
		     
		     /* reset i2c bus */
		     aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);				        

    }
	
	  aess_i2c_enabled_interrupt(u8BusNumber);
	
	  return;
}

#ifdef I2C_BUS_RECOVER

void aess_i2c_check_bus_status(UINT8 u8BusNumber)
{
	  UINT8 i;
	  i2c_reg_type *CurrentReg;
    sI2CDrvBufferType *psI2CDrvBuffer;

	  CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];	  
	  psI2CDrvBuffer = (sI2CDrvBufferType *) &i2c_buffer[u8BusNumber];

    #define I2CBUS_ERROR 0x01

		{
        if ((test_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status)) 
            || (test_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->control_status)))
			  {					 			  

		        /* clear bus error bit */
            set_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status);

			      /* if i2c bus SDA line is low */			  
            if (!test_bit_8(I2C_TEST_SDA_LINE_BIT,(void *) CurrentReg->control_status))
			      {
			          #define NINE_CLOCK_PULSE	20
			
			          for (i=0; i < NINE_CLOCK_PULSE; i++)
			          {
				            /* toggle one clock */                  
                    set_bit_8(I2C_TOGGLE_SCL_LINE_BIT,(void *) CurrentReg->control_status);  	
      	
				            /* delay 5 us */
				            udelay(5);      				
			          }
			          
			          #undef NINE_CLOCK_PULSE
			          DEBUGP("KN:I2CBus(%d) Generate 7 Clock Pulse To Recovery I2C Bus %d!\n", u8BusNumber);		              	  			  	
			      }
			      
            /* delay 10ms */
            mdelay(10);			      
            
			      /* if i2c bus is busy */
            if (test_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->control_status))	
			      {

			          /* clear bus error bit */
                set_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status);

			          /* clear bus error bit */
                set_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->status);
			          
			          /* generate START condition */
                set_bit_8(I2C_START_BIT,(void *) CurrentReg->control1);	 
			          
			          /* set slave address 0x00 */
			          iowrite8((UINT32) (0x00), (void *) CurrentReg->serial_data);
			          
			          /* generate STOP condition */                  
                set_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control1);
			      
			          DEBUGP("KN:I2CBus(%d) Force Generate STOP Condition To Recovery I2C Bus %d!\n", u8BusNumber);
            
                /* delay 10ms */
                mdelay(100);
			      }
			      
		        /* reconfig i2c bus */
		        aess_i2c_config((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);			                
		        
		        /* reset i2c bus */
		        aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);				        

        }
		}   
		#undef I2CBUS_ERROR
	
	return;
}

void aess_i2c_bus_recover_polling_func(unsigned long data)
{
	UINT8 u8BusNumber;
	
	for (u8BusNumber = 0; u8BusNumber < I2C_DRV_MAX_BUS; u8BusNumber++)
	{
		if (i2c_buffer[u8BusNumber].u8HasInit == 1)
		{
		    if (I2C_DRV_IDLE_STATE == i2c_buffer[u8BusNumber].u8State)
		    {
		        aess_i2c_disabled_interrupt(u8BusNumber);
		        aess_i2c_check_bus_status(u8BusNumber);
		        aess_i2c_enabled_interrupt(u8BusNumber);
		    }
		}
	}
	
	/* timer interval is 10s */
	i2c_bus_recover.expires = jiffies + 100;
	add_timer(&i2c_bus_recover);	
		    		
	return;
}
#endif

irqreturn_t aess_i2c_isr_handler(UINT8 u8BusNumber)
{
	UINT8 u8RegReading;
	i2c_reg_type *CurrentReg;	
	sI2CDrvBufferType *psI2CDrvBuffer;
	sI2CDrvSlvRecBufType *psTempBuff;	

	if ( u8BusNumber >= I2C_DRV_MAX_BUS)
	{
		DEBUGP("KN:Bus(%d) Bus Number Error!\n", u8BusNumber);
			
		return IRQ_NONE;  	
	}

	CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
	psI2CDrvBuffer = (sI2CDrvBufferType *) &i2c_buffer[u8BusNumber];

	/* if it is a New Match Status */
    if (test_bit_8(I2C_NEW_MATCH_BIT,(void *) CurrentReg->status))		
	{
		DEBUGP("KN:Bus(%d) I2C_NEW_MATCH \n", u8BusNumber);		
		switch (psI2CDrvBuffer->u8State)
		{
			case I2C_DRV_SLV_RCV_STATE:

        psI2CDrvBuffer->u8MsgRecCounter = 0 ;
								  	  	
				u8RegReading = ioread8((void *) CurrentReg->serial_data);  
				DEBUGP("KN:Bus(%d) r [0x%02x][%d]\n", u8BusNumber,u8RegReading,psI2CDrvBuffer->u8MsgRecCounter);		  	  					

				psI2CDrvBuffer->u8MsgRecBuffer[psI2CDrvBuffer->u8MsgRecCounter] = u8RegReading;			
				psI2CDrvBuffer->u8MsgRecCounter++;
				
                set_bit_8(I2C_NEW_MATCH_BIT,(void *) CurrentReg->status);		
					
				break;
							
			default:

				/* clear nak bit */
                set_bit_8(I2C_NEW_MATCH_BIT,(void *) CurrentReg->status);
				
				DEBUGP("KN:Bus(%d) default_STATE something worng!\n", u8BusNumber);
				
				break;
		}											
	}

	/* if it is a Bus Error Status */
    if (test_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status))	
	{
		DEBUGP("KN:Bus(%d) I2C_BUS_ERROR \n", u8BusNumber);			

		/* clear bus error bit */
        set_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status);
		switch (psI2CDrvBuffer->u8State)
		{
			case I2C_DRV_MST_XMIT_STATE:
			case I2C_DRV_MST_RCV_STATE:
			case I2C_DRV_SLV_RCV_STATE:
			
		    /* reconfig i2c bus */
		    aess_i2c_config((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
		    
		    /* reset */
		    aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);		
				break;			
			case I2C_DRV_IDLE_STATE:												
			default:
					
		    /* reset */
		    aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);		
				break;					
		}						
	}

	/* if it is a Negative Acknowledge Status */
    if (test_bit_8(I2C_NEGATIVE_ACK_BIT,(void *) CurrentReg->status))	
	{
		DEBUGP("KN:Bus(%d) I2C_NEGATIVE_ACK \n", u8BusNumber);	
					   		
		switch (psI2CDrvBuffer->u8State)
		{
			case I2C_DRV_MST_XMIT_STATE:
				
				DEBUGP("KN:Bus(%d) I2C_DRV_MST_XMIT_STATE\n", u8BusNumber);

				i2c_buffer[u8BusNumber].u16CurStopCount++;

				/* generate STOP condition */
				set_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control1); 
        
				/* clear nak bit */
				set_bit_8(I2C_NEGATIVE_ACK_BIT,(void *) CurrentReg->status);
				
				psI2CDrvBuffer->u8ErrorStatus = I2C_DRV_ERROR_NACK;			
				psI2CDrvBuffer->u8State = I2C_DRV_IDLE_STATE; 
		    psI2CDrvBuffer->wq_flag = 1;                                       
		    DEBUGP("KN:Bus(%d) wake_up_interruptible \n", u8BusNumber);        
		    wake_up_interruptible((void *) &psI2CDrvBuffer->wq);                        
		    DEBUGP("KN:Bus(%d) wake_up_interruptible done \n", u8BusNumber);	 
				
				break;
				
			case I2C_DRV_MST_RCV_STATE:
			
				DEBUGP("KN:Bus(%d) I2C_DRV_MST_RCV_STATE\n", u8BusNumber);
				
				i2c_buffer[u8BusNumber].u16CurStopCount++;

				/* generate STOP condition */
                set_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control1); 
        
				/* clear nak bit */
                set_bit_8(I2C_NEGATIVE_ACK_BIT,(void *) CurrentReg->status);
				
				psI2CDrvBuffer->u8ErrorStatus = I2C_DRV_ERROR_NACK;
				psI2CDrvBuffer->u8State = I2C_DRV_IDLE_STATE; 
		    psI2CDrvBuffer->wq_flag = 1;                                       
		    DEBUGP("KN:Bus(%d) wake_up_interruptible \n", u8BusNumber);        
		    wake_up_interruptible((void *) &psI2CDrvBuffer->wq);                        
		    DEBUGP("KN:Bus(%d) wake_up_interruptible done \n", u8BusNumber);	
				
				break;
										
			case I2C_DRV_IDLE_STATE:
				
				DEBUGP("KN:Bus(%d) IDLE_STATE\n", u8BusNumber);
				
				/* clear nak bit */
				set_bit_8(I2C_NEGATIVE_ACK_BIT,(void *) CurrentReg->status);
				
				DEBUGP("KN:Bus(%d) exit IDLE_STATE\n", u8BusNumber);
				
				break;
							
			default:

				/* clear nak bit */
                set_bit_8(I2C_NEGATIVE_ACK_BIT,(void *) CurrentReg->status);
				
				DEBUGP("KN:Bus(%d) default_STATE something worng!\n", u8BusNumber);
				
				break;
		}							
	}

	/* if it is a Stall After Start Status */
    if (test_bit_8(I2C_STALL_AFTER_START_BIT,(void *) CurrentReg->status))		
	{
		DEBUGP("KN:Bus(%d) I2C_STALL_AFTER_START \n", u8BusNumber);
		
		/* clear nak bit */
        set_bit_8(I2C_STALL_AFTER_START_BIT,(void *) CurrentReg->status);			
	}
	
	/* if it is a Slave Stop Status*/
    if (test_bit_8(I2C_SLAVE_STOP_BIT,(void *) CurrentReg->status))
	{
		DEBUGP("KN:Bus(%d) I2C_SLAVE_STOP \n", u8BusNumber);		
		switch (psI2CDrvBuffer->u8State)
		{
			case I2C_DRV_SLV_RCV_STATE:
											  	  								
				psI2CDrvBuffer->u8State = I2C_DRV_SLV_RCV_STATE;
        
				/* clear stop bit */
                set_bit_8(I2C_SLAVE_STOP_BIT,(void *) CurrentReg->status);			  

				/* get spin lock */
				spin_lock(psI2CDrvBuffer->lock);
						
				psTempBuff = kmalloc(sizeof(sI2CDrvSlvRecBufType), GFP_KERNEL);
				memcpy(psTempBuff->u8buffer, psI2CDrvBuffer->u8MsgRecBuffer, psI2CDrvBuffer->u8MsgRecCounter);        
				psTempBuff->u8MsgLen = psI2CDrvBuffer->u8MsgRecCounter ;
		  
				/* put recive data to kfifo */
			  
				kfifo_put(psI2CDrvBuffer->psFIFO,(unsigned char *) psTempBuff,sizeof(sI2CDrvSlvRecBufType));
			  
				kfree(psTempBuff);	

				/* release spin lock */
				spin_unlock(psI2CDrvBuffer->lock);			  
			  
				aess_generate_driver_event(psI2CDrvBuffer->u16DriverID, psI2CDrvBuffer->u32RecFlag);			  
	
				break;
							
			default:

        /* clear stop bit */
                set_bit_8(I2C_SLAVE_STOP_BIT,(void *) CurrentReg->status);
				
				DEBUGP("KN:Bus(%d) default_STATE something worng! psI2CDrvBuffer->u8State is %d \n", u8BusNumber, psI2CDrvBuffer->u8State);

				/* reset */
				aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);			
				
				break;
		}				
	}	

	/* if it is a SDA Status */
    if (test_bit_8(I2C_SDA_STATUS_BIT,(void *) CurrentReg->status))
	{
		DEBUGP("KN:Bus(%d) I2C_SDA_STATUS_BIT \n", u8BusNumber);	
		
		if (I2C_DRV_ERROR_NACK != psI2CDrvBuffer->u8ErrorStatus)
		{	
		  switch (psI2CDrvBuffer->u8State)
		  {
		  	case I2C_DRV_MST_XMIT_STATE:
		  		
		  		if (0 == psI2CDrvBuffer->u8MsgSendCounter)
		  		{
		  			DEBUGP("KN:Bus(%d) w slave address [0x%02x] \n",u8BusNumber,psI2CDrvBuffer->u8MsgSendBuffer[psI2CDrvBuffer->u8MsgSendCounter]); 					
              
		  			/* set slave address */
		  			iowrite8((UINT8) (psI2CDrvBuffer->u8MsgSendBuffer[psI2CDrvBuffer->u8MsgSendCounter]), (void *) CurrentReg->serial_data);					
		  			psI2CDrvBuffer->u8MsgSendCounter++; 			      
		  		}
		  		else
		  		{		  	
		  			/* write byte data */
		  			if (psI2CDrvBuffer->u8MsgSendCounter < psI2CDrvBuffer->u8MsgSendDataSize)
		  			{
		  				DEBUGP("KN:Bus(%d) w [0x%02x][%d]\n", u8BusNumber,psI2CDrvBuffer->u8MsgSendBuffer[psI2CDrvBuffer->u8MsgSendCounter],psI2CDrvBuffer->u8MsgSendCounter);		  	  	
      
		  				/* set byte data */
		  				iowrite8((UINT32) (psI2CDrvBuffer->u8MsgSendBuffer[psI2CDrvBuffer->u8MsgSendCounter]), (void *) CurrentReg->serial_data);					
		  				psI2CDrvBuffer->u8MsgSendCounter++; 	
		  			}
		  			else
		  			{
		  				/* transfer to master receiver mode */
		  				if (psI2CDrvBuffer->u8MsgRecDataSize > 0)
		  				{
		  					/* generate Re-Start condition */        
                                set_bit_8(I2C_START_BIT,(void *) CurrentReg->control1);	  		
		  					psI2CDrvBuffer->u8State = I2C_DRV_MST_RCV_STATE;
		  					DEBUGP("KN:Bus(%d) repeat START condition \n", u8BusNumber);            
		  				}			  		
		  				else
		  				{			  			        
		  					  /* no data needs to be read */			  			
		  					  i2c_buffer[u8BusNumber].u16CurStopCount++;
		  					  
		  					  /* generate STOP condition */                  
                                set_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control1);     
		  					  DEBUGP("KN:Bus(%d) generate STOP condition \n", u8BusNumber);
		  					  	            	
		            	/* wake up */
		            	psI2CDrvBuffer->u8ErrorStatus = I2C_DRV_ERROR_NONE;
		            	psI2CDrvBuffer->u8State = I2C_DRV_IDLE_STATE;
		            	psI2CDrvBuffer->wq_flag = 1;
		            	DEBUGP("KN:Bus(%d) wake_up_interruptible \n", u8BusNumber);
		            	wake_up_interruptible((void *) &psI2CDrvBuffer->wq);
		            	DEBUGP("KN:Bus(%d) wake_up_interruptible done \n", u8BusNumber);		  		
  	            }
		  			}
		  		}							
		  		break;
      
		  	case I2C_DRV_MST_RCV_STATE:
		  		if (0 == psI2CDrvBuffer->u8MsgRecCounter)
		  		{
		  			if (psI2CDrvBuffer->u8MsgRecDataSize == 1)
		  			{
                            set_bit_8(I2C_ACK_BIT,(void *) CurrentReg->control1);  
		  			}				  			
		  			/* set slave address */
		  			iowrite8((UINT32) (psI2CDrvBuffer->u8MsgSendBuffer[0]|1), (void *) CurrentReg->serial_data);			
		  			psI2CDrvBuffer->u8MsgRecCounter++;   	      
		  		}
		  		else
		  		{	  			 			
            if ((psI2CDrvBuffer->u8MsgRecCounter == 1)
                && (I2C_DRV_TRANS_SMBUS_BLK == psI2CDrvBuffer->u8TransType))
            {
  	  				UINT8 u8TotalByteCount = 0;			
  	  				u8RegReading = ioread8((void *) CurrentReg->serial_data);  
  	  				psI2CDrvBuffer->u8MsgRecBuffer[psI2CDrvBuffer->u8MsgRecCounter] = u8RegReading;			
              psI2CDrvBuffer->u8MsgRecCounter++;
              u8TotalByteCount = u8RegReading + 1;              
              /* The minmux byte count need to greater and equal than 2 for the most of device */
              if ((u8TotalByteCount < 2) || (u8TotalByteCount > psI2CDrvBuffer->u8MsgRecDataSize))
              {
                  /* update the receive counter */
                                set_bit_8(I2C_ACK_BIT,(void *) CurrentReg->control1);                   
                  psI2CDrvBuffer->u8MsgRecDataSize = 2;
                  DEBUGP("KN: %d To SMBus Read Count is %d\n", u8BusNumber, u8TotalByteCount);              
              }
              else
              {
                  /* update the receive counter */
                  psI2CDrvBuffer->u8MsgRecDataSize = u8TotalByteCount;
                  DEBUGP("KN: %d To SMBus Read Count is %d\n", u8BusNumber, u8TotalByteCount);
              }		              
              
            }		  			
          	else if (psI2CDrvBuffer->u8MsgRecCounter < psI2CDrvBuffer->u8MsgRecDataSize)		   	
          	{
            	if ((psI2CDrvBuffer->u8MsgRecCounter + 1) == psI2CDrvBuffer->u8MsgRecDataSize)  
            	{
                                set_bit_8(I2C_ACK_BIT,(void *) CurrentReg->control1);                
            	} 				  	  	
  	  				u8RegReading = ioread8((void *) CurrentReg->serial_data);  
  	  				psI2CDrvBuffer->u8MsgRecBuffer[psI2CDrvBuffer->u8MsgRecCounter] = u8RegReading;			
              psI2CDrvBuffer->u8MsgRecCounter++;
          	}
          	else if (psI2CDrvBuffer->u8MsgRecCounter == psI2CDrvBuffer->u8MsgRecDataSize)
          	{  
            	/* generate STOP condition */                        				  				                
                            set_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control1);  
            	          		
  	  				u8RegReading = ioread8((void *) CurrentReg->serial_data);                 			  			
  	  				psI2CDrvBuffer->u8MsgRecBuffer[psI2CDrvBuffer->u8MsgRecCounter] = u8RegReading;			
            	psI2CDrvBuffer->u8MsgRecCounter++;
                			  			
            	i2c_buffer[u8BusNumber].u16CurStopCount++;
            	
            	/* wake up */
            	if ((I2C_DRV_TRANS_SMBUS_BLK == psI2CDrvBuffer->u8TransType) && (psI2CDrvBuffer->u8MsgRecDataSize == 2))
            	{
		            psI2CDrvBuffer->u8ErrorStatus = I2C_DRV_ERROR_PROTOCOL;		          	
            	}
            	else
            	{
		            psI2CDrvBuffer->u8ErrorStatus = I2C_DRV_ERROR_NONE;
		          }
		          psI2CDrvBuffer->u8State = I2C_DRV_IDLE_STATE;
		          psI2CDrvBuffer->wq_flag = 1;
		          DEBUGP("KN:Bus(%d) wake_up_interruptible \n", u8BusNumber);
		          wake_up_interruptible((void *) &psI2CDrvBuffer->wq);
		          DEBUGP("KN:Bus(%d) wake_up_interruptible done \n", u8BusNumber);	  	
          	}	
          	else
          	{
		          DEBUGP("KN:Bus(%d) something worng!\n", u8BusNumber);			
		  
		          /* reset */
		          aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);				  			
          	}  	
		  	  }
		  		
		  		DEBUGP("KN:Bus(%d) exit I2C_DRV_MST_RCV_STATE\n", u8BusNumber);
		  		
		  		break;
		  	case I2C_DRV_SLV_RCV_STATE:
		  								  	  	
		  		u8RegReading = ioread8((void *) CurrentReg->serial_data);  
		  		DEBUGP("KN:Bus(%d) r [0x%02x][%d]\n", u8BusNumber,u8RegReading,psI2CDrvBuffer->u8MsgRecCounter);		  	  					
      
		  		psI2CDrvBuffer->u8MsgRecBuffer[psI2CDrvBuffer->u8MsgRecCounter] = u8RegReading;			
		  		psI2CDrvBuffer->u8MsgRecCounter++;								
      
		  		DEBUGP("KN:Bus(%d) exit I2C_DRV_SLV_RCV_STATE\n", u8BusNumber);
		  		
		  		break;			
		  	case I2C_DRV_IDLE_STATE:
		  	
		  		u8RegReading = ioread8((void *) CurrentReg->serial_data);  					              			
		  		  
		  		/* reset */
		  		aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
		  			
		  		break;						
		  	default:
		  		
		  		DEBUGP("KN:Bus(%d) S default_STATE  psI2CDrvBuffer->u8State is %d \n", u8BusNumber, psI2CDrvBuffer->u8State);
		  		aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
		  						
		  		break;					
		  }
		}	
	}	

	/* if it is a Global Call Match Status */
    if (test_bit_8(I2C_GLOBAL_CALL_MATCH_BIT,(void *) CurrentReg->control_status))		
	{
		printk("KN:Bus(%d) I2C_GLOBAL_CALL_MATCH \n", u8BusNumber);		
				
        set_bit_8(I2C_GLOBAL_CALL_MATCH_BIT,(void *) CurrentReg->control_status);
	}	

	/* if it is a ARP ADDR Match Status */
    if (test_bit_8(I2C_ARP_ADDR_MATCH_BIT,(void *) CurrentReg->control_status))		
	{
		printk("KN:Bus(%d) I2C_ARP_ADDR_MATCH_BIT \n", u8BusNumber);		
				
        set_bit_8(I2C_ARP_ADDR_MATCH_BIT,(void *) CurrentReg->control_status);
	}	

	/* if it is a ARP ADDR Match Status */
	if (test_bit_8(I2C_ARP_ADDR_MATCH_BIT,(void *) CurrentReg->control_status))		
	{
		printk("KN:Bus(%d) I2C_ARP_ADDR_MATCH_BIT \n", u8BusNumber);		
				
		set_bit_8(I2C_ARP_ADDR_MATCH_BIT,(void *) CurrentReg->control_status);
	}	

	/* if it is a ADDR Match Status */
    if (test_bit_8(I2C_ADDR_MATCH_BIT,(void *) CurrentReg->control_status))		
	{
		printk("KN:Bus(%d) I2C_ADDR_MATCH_BIT \n", u8BusNumber);		
				
        set_bit_8(I2C_ADDR_MATCH_BIT,(void *) CurrentReg->control_status);
	}	
		
	return IRQ_HANDLED;
}


irqreturn_t aess_i2cgroup_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	irqreturn_t irqreturn = IRQ_NONE;

	/* check I2C bus 0 interrupt status */
	if(test_bit(AIC_GEN_SMB0,(void *) (AIC_GRSR_REG)))
	{
		if (IRQ_HANDLED == aess_i2c_isr_handler(0))
		{
			clear_bit(AIC_GEN_SMB0,(void *) (AIC_GRSR_REG));			
			irqreturn = IRQ_HANDLED;
		}  	
	}   
	/* check I2C bus 1 interrupt status */
	else if(test_bit(AIC_GEN_SMB1,(void *) (AIC_GRSR_REG)))
	{
		if (IRQ_HANDLED == aess_i2c_isr_handler(1))
		{
			clear_bit(AIC_GEN_SMB1,(void *) (AIC_GRSR_REG));
			irqreturn = IRQ_HANDLED;
		}  	
	}    	
	/* check I2C bus 2 interrupt status */	
	else if(test_bit(AIC_GEN_SMB2,(void *) (AIC_GRSR_REG)))
	{
		if (IRQ_HANDLED == aess_i2c_isr_handler(2))
		{
			clear_bit(AIC_GEN_SMB2,(void *) (AIC_GRSR_REG));
		irqreturn = IRQ_HANDLED;
		}  	
	}
	else
	{
    DEBUGP("Unknow ISR!\n");
  } 	
	return irqreturn;
}

irqreturn_t aess_i2c3_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	irqreturn_t irqreturn = IRQ_NONE;
	
	if (IRQ_HANDLED == aess_i2c_isr_handler(3))
	{
		irqreturn = IRQ_HANDLED;
	}
	
	return irqreturn;
}

irqreturn_t aess_i2c4_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	irqreturn_t irqreturn = IRQ_NONE;
	
	if (IRQ_HANDLED == aess_i2c_isr_handler(4))
	{
		irqreturn = IRQ_HANDLED;
	}
	
	return irqreturn;
}

irqreturn_t aess_i2c5_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	irqreturn_t irqreturn = IRQ_NONE;
	
	if (IRQ_HANDLED == aess_i2c_isr_handler(5))
	{
		irqreturn = IRQ_HANDLED;
	}
	
	return irqreturn;
}


static int aess_i2c_wr(sI2CDrvBufferType *psI2CDrvBuffer)
{
	UINT8 u8BusNumber;
	UINT16 i;
	i2c_reg_type *CurrentReg;
	
	u8BusNumber = psI2CDrvBuffer->u8Channel;
	CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
	
	DEBUGP("KN:Bus(%d) aess_i2c_wr() enter!\n", u8BusNumber);
	
	/* if i2c bus error is detected */
    if (test_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status))	
	{
		DEBUGP("KN:Bus(%d) ERROR_BUS\n", u8BusNumber);
		
		/* recode error status */
		psI2CDrvBuffer->u8ErrorStatus = I2C_DRV_ERROR_BUS;
		
		/* setup next state */
		psI2CDrvBuffer->u8State = I2C_DRV_IDLE_STATE;
		
		return 0;
	}

	/* check for transmitter or receiver mode */
	if (psI2CDrvBuffer->u8MsgSendBuffer[0] & 0x01)
	{
		if (psI2CDrvBuffer->u8MsgRecDataSize != 0)
		{
			DEBUGP("KN:Bus(%d) Rec Data Size %d\n", u8BusNumber, psI2CDrvBuffer->u8MsgRecDataSize);
			
			/* setup next state */
			psI2CDrvBuffer->u8State = I2C_DRV_MST_RCV_STATE;
		}
		else
		{
			/* setup next state */
			psI2CDrvBuffer->u8State = I2C_DRV_IDLE_STATE;
			
			return 0;
		}
	}
	/* transmitter mode */
	else
	{
		/* setup next state */
		psI2CDrvBuffer->u8State = I2C_DRV_MST_XMIT_STATE;
	}

	/* IPMB mode */
	if (i2c_buffer[u8BusNumber].u8CurMode == I2C_DRV_MODE_IPMB)
	{
		/* disabled new match interrupt */
        clear_bit_8(I2C_NEW_MATCH_INTERRUPT_ENABLE_BIT,(void *) CurrentReg->control1);	    
	}

	/* clear control1 register and enabled interrupt */
	iowrite8(I2C_INTERRUPT_ENABLE, (void *) CurrentReg->control1);  

	i2c_buffer[u8BusNumber].u16CurStartCount++;
	
	/* become to master transmiter */
    set_bit_8(I2C_START_BIT,(void *) CurrentReg->control1);
  
	/* wait for I2C master transaction */
	if (0 == wait_event_interruptible_timeout(psI2CDrvBuffer->wq, 
											  psI2CDrvBuffer->wq_flag == 1, 
											  50))
	{	 
      if (0 == psI2CDrvBuffer->wq_flag)
      {
			     DEBUGP("KN:Bus(%d) Timeout ST:0x%02x CST:0x%02x CTL1:0x%02x CTL2:0x%02x CTL3:0x%02x ADDR1:0x%02x ADDR2:0x%02x state:%02d\n",
				          u8BusNumber,
				          ioread8((void *) i2c_reg[u8BusNumber].status),
				          ioread8((void *) i2c_reg[u8BusNumber].control_status),
				          ioread8((void *) i2c_reg[u8BusNumber].control1),
				          ioread8((void *) i2c_reg[u8BusNumber].control2),
				          ioread8((void *) i2c_reg[u8BusNumber].control3),
				          ioread8((void *) i2c_reg[u8BusNumber].own_address1),
				          ioread8((void *) i2c_reg[u8BusNumber].own_address2),
				          i2c_buffer[u8BusNumber].u8State);		
			
		       /* recode error status */
		       psI2CDrvBuffer->u8ErrorStatus = I2C_DRV_ERROR_TIMEOUT;
		       
		       DEBUGP("KN:Bus(%d) wait_event master transaction timeout\n", u8BusNumber);
      }
	}
	DEBUGP("KN:Bus(%d) transmitter done \n", u8BusNumber);	

	/* clear the flag of condition checking */
	psI2CDrvBuffer->wq_flag = 0;

	DEBUGP("KN:Bus(%d) aess_i2c_wr() exit!\n", u8BusNumber);	

#ifdef I2C_MUX_WAIT_BUS_FREE 	
/* Wait up to 2 ms for bus free condition before returning	*/
	for(i=0;i<I2C_2MS_UDELAY_TICKS;i++)
	{
	   if (test_bit_8(I2C_BUS_BUSY_BIT,(void *) i2c_reg[u8BusNumber].control_status))
	      udelay(1);
	   else
	      break;   
	}
#endif		

	if (I2C_DRV_ERROR_NONE != psI2CDrvBuffer->u8ErrorStatus)
	{
      return -EFAULT;			
	}
	else
	{
	    return 0;
  }
}

static int aess_i2c_ctrlhw(sI2CDrvBusInfoType *psI2CDrvBusInfo)
{
	UINT8 i ;
	UINT8 u8BusNumber;
	i2c_reg_type *CurrentReg;
	
	u8BusNumber = psI2CDrvBusInfo->u8Channel;	
	CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
	
	aess_i2c_disabled_interrupt(u8BusNumber);
	
	/* recode error status */
	i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_BUS;
	
	switch (psI2CDrvBusInfo->u8CtrlHW)
	{
		case I2C_DRV_CLK_PULSE:

			DEBUGP("KN: I2C_DRV_CLK_PULSE\n");		
			
			#define NINE_CLOCK_PULSE	20
			
			for (i=0; i < NINE_CLOCK_PULSE; i++)
			{
				/* toggle one clock */                  
                set_bit_8(I2C_TOGGLE_SCL_LINE_BIT,(void *) CurrentReg->control_status);  	
      	
				/* delay 5 us */
				udelay(5);      				
			}			
			
			#undef NINE_CLOCK_PULSE
			
			break;
			
		case I2C_DRV_FORCE_STOP:

			DEBUGP("KN: I2C_DRV_FORCE_STOP\n");				

			/* generate STOP condition */                  
            set_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control1);      				
			
			break;
			
		case I2C_DRV_RESET_SMBUS:

			DEBUGP("KN: I2C_DRV_RESET_SMBUS\n");					
			
			break;
			
		default:
			
			DEBUGP("KN: not support type\n");
			
			break;
	}
	
	/* reset variable */
	
  i2c_buffer[u8BusNumber].u16CurStartCount = 0;
	i2c_buffer[u8BusNumber].u16CurStopCount = 0;			
	
	if (i2c_buffer[u8BusNumber].u8InitMode == I2C_DRV_MODE_IPMB)
	{
		i2c_buffer[u8BusNumber].u8State = I2C_DRV_SLV_RCV_STATE;
	}
	else
	{
		i2c_buffer[u8BusNumber].u8State = I2C_DRV_IDLE_STATE;
	}
	
	/* recode error status */                             
	i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_NONE;
	
	aess_i2c_enabled_interrupt(u8BusNumber);
	
	return 0;
}


static int aess_i2c_ioctl(struct inode * inode, struct file *flip, 
						  unsigned int cmd, unsigned long arg)
{
	int result;
	UINT8 u8BusNumber;
	UINT8 u8BusStatus;
	i2c_reg_type *CurrentReg;
	volatile sI2CDrvBusInfoType *psI2CDrvBusInfo;
	volatile sI2CDrvBufInfoType *psI2CDrvBufInfo;
	sI2CDrvSlvRecBufType *psTempBuff;		
	
	/* DEBUGP("KN: aess_i2c_ioctl\n"); */
	
	switch (cmd)
	{
		case AESS_I2CDRV_INIT:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_INIT\n");		
			
			/* */
			psI2CDrvBusInfo = (sI2CDrvBusInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBusInfo, 
						  sizeof(sI2CDrvBusInfoType)))
			{
				DEBUGP("KN: struct access error!\n");		
				/* error occur */
				return -EFAULT;
			}
			
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBusInfo->u8Channel;
			
			/* get semaphore */
			if(down_interruptible((struct semaphore *) &i2c_buffer[u8BusNumber].sem))
			{
				DEBUGP("KN:AESS_I2CDRV_INIT get semaphore error!\n");			
				return -ERESTARTSYS;
			}
			
            aess_i2c_reset_SMBus(u8BusNumber);
			if (i2c_buffer[u8BusNumber].u8HasInit == 0)
			{
				i2c_buffer[u8BusNumber].u8Channel = u8BusNumber;
				i2c_buffer[u8BusNumber].u8CurMode = psI2CDrvBusInfo->u8InitMode;				
				i2c_buffer[u8BusNumber].u8InitMode = psI2CDrvBusInfo->u8InitMode;
				i2c_buffer[u8BusNumber].u32RecFlag = psI2CDrvBusInfo->u32RecFlag;				
				i2c_buffer[u8BusNumber].u8SlaveAddr = psI2CDrvBusInfo->u8SlaveAddr;
				i2c_buffer[u8BusNumber].u8Frequency = psI2CDrvBusInfo->u8Frequency;
				i2c_buffer[u8BusNumber].u16DriverID = psI2CDrvBusInfo->u16DriverID;
				
				if (i2c_buffer[u8BusNumber].u8CurMode == I2C_DRV_MODE_IPMB)
				{
					i2c_buffer[u8BusNumber].u8State = I2C_DRV_SLV_RCV_STATE;
				}
				else
				{
					i2c_buffer[u8BusNumber].u8State = I2C_DRV_IDLE_STATE;
				}
				
				i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_NONE;
				
				/* clear the flag of condition checking */
				i2c_buffer[u8BusNumber].wq_flag = 0;
				
				/* initiate wait queue */
				init_waitqueue_head((void *) &i2c_buffer[u8BusNumber].wq);
				
				if (i2c_buffer[u8BusNumber].u8CurMode == I2C_DRV_MODE_IPMB)
				{
					spin_lock_init(&i2c_buffer[u8BusNumber].lock);

					i2c_buffer[u8BusNumber].psFIFO = kfifo_alloc(I2C_DRV_MAX_FIFO_SIZE * sizeof(sI2CDrvSlvRecBufType), GFP_KERNEL,(void *) &i2c_buffer[u8BusNumber].lock);
		        
					if (IS_ERR(i2c_buffer[u8BusNumber].psFIFO)) 
					{
						DEBUGP("KN: fifo allocation failed!\n");	
				    
						/* release semaphore */
						up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);	/* chtsai*/
					
						return -ERESTARTSYS;				    
					}		  
				}		
				
				result = aess_i2c_config((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
				
				if (result)
				{
					DEBUGP("KN:Bus(%d) AESS_I2CDRV_INIT error\n", u8BusNumber);
					
					/* release semaphore */
					up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
					
					return (result);
				}
			}
			
			/* release semaphore */
			up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
			
			break;
			
		case AESS_I2CDRV_CONFIG:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_CONFIG\n");

			psI2CDrvBusInfo = (sI2CDrvBusInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBusInfo, 
						  sizeof(sI2CDrvBusInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* error occur */
				return -EFAULT;
			}
			
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBusInfo->u8Channel;
			
			if (i2c_buffer[u8BusNumber].u8HasInit == 1)
			{
				/* allow to change the current mode */
				i2c_buffer[u8BusNumber].u8CurMode = psI2CDrvBusInfo->u8CurMode;
				
				/* invoke the sub function to initiate the I2C bus */
				result = aess_i2c_config((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
				
				DEBUGP("KN: reconfig\n");
				
				if (result)
				{
					return (result);
				}
			}
			
			break;
			
		case AESS_I2CDRV_WR:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_WR\n");
						
			psI2CDrvBufInfo = (sI2CDrvBufInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBufInfo, 
						  sizeof(sI2CDrvBufInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* error occur */
				return -EFAULT;
			}
			
			/* Check related parameter */
			if (I2C_DRV_TRANS_SMBUS_BLK == psI2CDrvBufInfo->u8TransType)
			{
				if ((psI2CDrvBufInfo->u8MsgRecDataSize > 0)
				    && psI2CDrvBufInfo->u8MsgRecDataSize < 3)
				{
					DEBUGP("KN: receive data lengeth of SMBUS Block read need greater than 2\n");
					/* report error status */
					psI2CDrvBufInfo->u8ErrorStatus = I2C_DRV_ERROR_PROTOCOL;              
					/* error occur */
					return -EFAULT;
				}
				DEBUGP("KN: SMBUS Block mode\n");
			}
	
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBufInfo->u8Channel;
			CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
			
			if ( psI2CDrvBufInfo->u8MsgRecDataSize >= 0xFF)
			{
				/* error occur */
				return -EFAULT;				
			}
			
			/* get semaphore */
			if(down_interruptible((struct semaphore *) &i2c_buffer[u8BusNumber].sem))
			{
				return -ERESTARTSYS;
			}			
			
			/* if i2c bus is error */
            if (test_bit_8(I2C_BUS_ERROR_BIT,(void *) i2c_reg[u8BusNumber].status))
			{
				aess_i2c_bus_recovery(u8BusNumber);
				
				/* report error status */
				psI2CDrvBufInfo->u8ErrorStatus = I2C_DRV_ERROR_BUS;
				
				/* release semaphore */
				up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);				
				DEBUGP("KN:Bus(%d) ERROR_BUS\n", u8BusNumber);
				
				return 0;
			}

			/* if i2c bus is busy */
            if (test_bit_8(I2C_BUS_BUSY_BIT,(void *) i2c_reg[u8BusNumber].control_status))
			{
				/* report error status */
				psI2CDrvBufInfo->u8ErrorStatus = I2C_DRV_ERROR_BUSY;
				
				/* release semaphore */
				up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);				
				DEBUGP("KN:Bus(%d) ERROR_BUSY\n", u8BusNumber);
				
				return 0;
			}
				
			i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_NONE;
			
			i2c_buffer[u8BusNumber].u8MsgSendBuffer[0] = psI2CDrvBufInfo->u8DeviceAddr;
			
			if (psI2CDrvBufInfo->u8MsgSendDataSize == 0)
			{
				DEBUGP("KN:Bus(%d) No sending data\n", u8BusNumber);
				i2c_buffer[u8BusNumber].u8MsgSendBuffer[0] |= 0x01;
			}
			else
			{
				if (copy_from_user ((void *) &i2c_buffer[u8BusNumber].u8MsgSendBuffer[1],
									          (void *) psI2CDrvBufInfo->pu8MsgSendBuffer,
									          psI2CDrvBufInfo->u8MsgSendDataSize))
				{
					/* release semaphore */
					up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
					
					return -EFAULT;
				}
			}
			i2c_buffer[u8BusNumber].u8TransType = psI2CDrvBufInfo->u8TransType;
			i2c_buffer[u8BusNumber].u8MsgSendDataSize = (psI2CDrvBufInfo->u8MsgSendDataSize + 1);
			i2c_buffer[u8BusNumber].u8MsgSendCounter = 0;
			i2c_buffer[u8BusNumber].u8MsgRecDataSize = psI2CDrvBufInfo->u8MsgRecDataSize;
			i2c_buffer[u8BusNumber].u8MsgRecCounter = 0;
					
			DEBUGP("KN:Bus(%d) u8MsgSendDataSize %d\n", 
				   u8BusNumber, 
				   i2c_buffer[u8BusNumber].u8MsgSendDataSize);
			DEBUGP("KN:Bus(%d) u8MsgSendCounter %d\n", 
				   u8BusNumber, 
				   i2c_buffer[u8BusNumber].u8MsgSendCounter);
			DEBUGP("KN:Bus(%d) u8MsgRecDataSize %d\n", 
				   u8BusNumber, 
				   i2c_buffer[u8BusNumber].u8MsgRecDataSize);
			DEBUGP("KN:Bus(%d) u8MsgRecCounter %d\n", 
				   u8BusNumber, 
				   i2c_buffer[u8BusNumber].u8MsgRecCounter);
			
			DEBUGP("KN:Bus(%d) Enter wr func\n", u8BusNumber);
			
			/* invoke the sub function to execute */
			result = aess_i2c_wr((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
			
			if (I2C_DRV_TRANS_SMBUS_BLK == psI2CDrvBufInfo->u8TransType)
			{
				psI2CDrvBufInfo->u8MsgRecDataSize = i2c_buffer[u8BusNumber].u8MsgRecDataSize;
			}
			
			if (result)
			{
				i2c_buffer[u8BusNumber].u8MsgRecCounter = 0;
				
				if (i2c_buffer[u8BusNumber].u8CurMode == I2C_DRV_MODE_IPMB)
				{
					i2c_buffer[u8BusNumber].u8State = I2C_DRV_SLV_RCV_STATE;
				}
				u8BusStatus = i2c_buffer[u8BusNumber].u8ErrorStatus;
        
				/* reset i2c bus to try recoverying a hung bus */
				aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
		
				/* reconfig i2c bus */
				aess_i2c_config((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
		    
				psI2CDrvBufInfo->u8ErrorStatus = u8BusStatus;
			
				/* release semaphore */
				up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);			
				
				DEBUGP("KN:Bus(%d) ioctl AESS_I2CDRV_WR Error Return!\n", u8BusNumber);
				
				return -EFAULT;
			}
			
			if (i2c_buffer[u8BusNumber].u8ErrorStatus == I2C_DRV_ERROR_NONE)
			{				
				if (i2c_buffer[u8BusNumber].u8MsgRecCounter > 0)
				{  
					DEBUGP(KERN_EMERG "KN:Bus(%d) Start to Copy data to user mode\n", u8BusNumber);
					if (copy_to_user((void *) psI2CDrvBufInfo->pu8MsgRecBuffer,
									 (void *) &i2c_buffer[u8BusNumber].u8MsgRecBuffer[1],
									 i2c_buffer[u8BusNumber].u8MsgRecCounter))
					{
						i2c_buffer[u8BusNumber].u8MsgRecCounter = 0;
						
						if (i2c_buffer[u8BusNumber].u8CurMode == I2C_DRV_MODE_IPMB)
						{
							i2c_buffer[u8BusNumber].u8State = I2C_DRV_SLV_RCV_STATE;
						}
						
						memset((void *) &i2c_buffer[u8BusNumber].u8MsgSendBuffer[0], 0, I2C_DRV_MAX_MSG_SIZE);
						memset((void *) &i2c_buffer[u8BusNumber].u8MsgRecBuffer[0], 0, I2C_DRV_MAX_MSG_SIZE);
						
						/* release semaphore */
						up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
						
						DEBUGP(KERN_EMERG "KN:Bus(%d) Copy data to user mode FAIL!!!!! \n", u8BusNumber);
						return -EFAULT;
					}
					
					memset((void *) &i2c_buffer[u8BusNumber].u8MsgSendBuffer[0], 0, I2C_DRV_MAX_MSG_SIZE);
					memset((void *) &i2c_buffer[u8BusNumber].u8MsgRecBuffer[0], 0, I2C_DRV_MAX_MSG_SIZE);
					psI2CDrvBufInfo->u8MsgRecDataSize = i2c_buffer[u8BusNumber].u8MsgRecCounter - 1;					
					i2c_buffer[u8BusNumber].u8MsgRecCounter = 0;
					DEBUGP(KERN_EMERG "KN:Bus(%d) Copy data to user mode SUCCESS!!!!! \n", u8BusNumber);
				}
			}
			else
			{			
				psI2CDrvBufInfo->u8ErrorStatus = i2c_buffer[u8BusNumber].u8ErrorStatus;
			   
				/* reset error status */
				i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_NONE;
			}
		
			if (i2c_buffer[u8BusNumber].u8CurMode == I2C_DRV_MODE_IPMB)
			{  				
					
				i2c_buffer[u8BusNumber].u8State = I2C_DRV_SLV_RCV_STATE;								   		
				
				/* clear control1 register and enabled interrupt & new match interrupt */
				iowrite8(I2C_NEW_MATCH_INTERRUPT_ENABLE | I2C_INTERRUPT_ENABLE, (void *) CurrentReg->control1);      		    			           				      						
			}
			else
			{   				
				i2c_buffer[u8BusNumber].u8State = I2C_DRV_IDLE_STATE;				 				
			}		
			i2c_buffer[u8BusNumber].u8MsgSendDataSize = 0;
			i2c_buffer[u8BusNumber].u8MsgSendCounter 	= 0;
			i2c_buffer[u8BusNumber].u8MsgRecDataSize  = 0;
			i2c_buffer[u8BusNumber].u8MsgRecCounter 	= 0;		  
		  
			/* release semaphore */
			up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);

			DEBUGP("KN:Bus(%d) Exit wr func\n", u8BusNumber);
			
			break;
			
		case AESS_I2CDRV_GET_MSG:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_GET_MSG\n");

			psI2CDrvBufInfo = (sI2CDrvBufInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBufInfo, sizeof(sI2CDrvBufInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* error occur */
				return -EFAULT;
			}
			
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBufInfo->u8Channel;		

			/* get semaphore */
			if(down_interruptible((struct semaphore *) &i2c_buffer[u8BusNumber].sem))
			{
				return -ERESTARTSYS;
			}				
							
			psTempBuff = kmalloc(sizeof(sI2CDrvSlvRecBufType), GFP_KERNEL);	
			if (kfifo_get(i2c_buffer[u8BusNumber].psFIFO,(unsigned char *) psTempBuff, sizeof(sI2CDrvSlvRecBufType))==(sizeof(sI2CDrvSlvRecBufType)))
			{
				/* IPMB packet includes slave address */
				if (copy_to_user((void *) psI2CDrvBufInfo->pu8MsgRecBuffer,
								 (void *) &psTempBuff->u8buffer[0],
								 psTempBuff->u8MsgLen))
				{
					psI2CDrvBufInfo->u8MsgRecDataSize = 0;			
			    					
					/* free allocated memory */
					kfree(psTempBuff);									
				  
					DEBUGP("KN:Bus(%d) copy_to_user error!\n", u8BusNumber);	
				  	
					/* release semaphore */
					up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);	/* chtsai */
					
					return -EFAULT;
				}
				else
				{				
			    
					/* IPMB message size includes the slave address */
					psI2CDrvBufInfo->u8MsgRecDataSize = psTempBuff->u8MsgLen;			    
						    
					/* free allocated memory */
					kfree(psTempBuff);								  
					/* if FIFO is not empty */
					if (kfifo_len(i2c_buffer[u8BusNumber].psFIFO) != 0)
					{
						DEBUGP("KN:Bus(%d) generate event to notify!\n", u8BusNumber);						
						/* generate event to notify */
						aess_generate_driver_event(i2c_buffer[u8BusNumber].u16DriverID, 
												   i2c_buffer[u8BusNumber].u32RecFlag);												   
					}					
				}				
			}
			else
			{			
				/* set data size to 0 */
				psI2CDrvBufInfo->u8MsgRecDataSize = 0;
								
				DEBUGP("KN:Bus(%d) No received data\n", u8BusNumber);
				
				/* release semaphore */
				up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);	/* chtsai */
				
				return 0;				
			}
			
			/* release semaphore */
			up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);			
			
			break;
			
		case AESS_I2CDRV_RESET:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_RESET\n");		
			
			psI2CDrvBusInfo = (sI2CDrvBusInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBusInfo, 
						  sizeof(sI2CDrvBusInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* error occur */
				return -EFAULT;
			}
			
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBusInfo->u8Channel;
			
			/* get semaphore */
			if(down_interruptible((struct semaphore *) &i2c_buffer[u8BusNumber].sem))
			{
				return -ERESTARTSYS;
			}
			
			result = aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
					
			/* release semaphore */
			up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
			
			break;
			
		case AESS_I2CDRV_GET_STATUS:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_GET_STATUS\n");
			
			psI2CDrvBusInfo = (sI2CDrvBusInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBusInfo, 
						  sizeof(sI2CDrvBusInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* error occur */
				return -EFAULT;
			}
			
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBusInfo->u8Channel;
			CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];			
			
			if (i2c_buffer[u8BusNumber].u8ErrorStatus == I2C_DRV_ERROR_NONE)
			{				
                if (test_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->control_status))				
				{
					DEBUGP("KN: %d ERROR_BUSY\n", u8BusNumber);
					
					/* report error status */
					psI2CDrvBusInfo->u8ErrorStatus = I2C_DRV_ERROR_BUSY;
				}
				else
				{
					psI2CDrvBusInfo->u8ErrorStatus = I2C_DRV_ERROR_NONE;
				}
			}
			else
			{
				psI2CDrvBusInfo->u8ErrorStatus = i2c_buffer[u8BusNumber].u8ErrorStatus;
				i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_NONE;
			}
			
			psI2CDrvBusInfo->u8ErrorStatus = I2C_DRV_ERROR_NONE;
								
			break;
			
		case AESS_I2CDRV_GET_HW_STATUS:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_GET_HW_STATUS\n"); 
			
			psI2CDrvBusInfo = (sI2CDrvBusInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBusInfo, 
						  sizeof(sI2CDrvBusInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* error occur */
				return -EFAULT;
			}
			
      #define I2C_DRV_SCL_STATUS          0x02
      #define I2C_DRV_SDA_STATUS          0x01
			
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBusInfo->u8Channel;
			CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];
		
			/* reset the variable */
			u8BusStatus = 0;
			
      if( I2C_DRV_IDLE_STATE == i2c_buffer[u8BusNumber].u8State )
      {

			/* get current start tracking count */
			psI2CDrvBusInfo->u16CurStartCount = i2c_buffer[u8BusNumber].u16CurStartCount;
			
			/* get current stop tracking count */
			psI2CDrvBusInfo->u16CurStopCount = i2c_buffer[u8BusNumber].u16CurStopCount;
			
			/* u8BusStatus 
			   bit 7 -> reserved
				   6 -> reserved
				   5 -> reserved
					   4 -> waiting for STOP to complete
				   3 -> bus error
				   2 -> bus busy
				   1 -> clock status
				   0 -> data status */
				   
                if (test_bit_8(I2C_BUS_ERROR_BIT,(void *) CurrentReg->status))				
			{
				u8BusStatus |= I2C_SET_BUS_ERROR_BIT;
			}
			
			/* if i2c bus is busy */
                if (test_bit_8(I2C_BUS_BUSY_BIT,(void *) CurrentReg->control_status))	
			{
				u8BusStatus |= I2C_SET_BUS_BUSY_BIT;
			}
			
                if (test_bit_8(I2C_TEST_SDA_LINE_BIT,(void *) CurrentReg->control_status))
			{
			  u8BusStatus |= I2C_DRV_SDA_STATUS;				
			}
			  
                if (test_bit_8(I2C_STOP_BIT,(void *) CurrentReg->control_status))
			{
			  u8BusStatus |= I2C_SET_BUS_STOP_BIT;				
			}

      DEBUGP("I2CDrv(%d)[0x%02x]:AESS_I2CDRV_GET_HW_STATUS!\n",u8BusNumber,ioread8((void *) CurrentReg->control_status));
      }
      else
      {
        u8BusStatus |= I2C_DRV_SDA_STATUS;	      	
      }
			
			u8BusStatus |= I2C_DRV_SCL_STATUS;			
			
			psI2CDrvBusInfo->u8BusStatus = u8BusStatus;

      #undef I2C_DRV_SCL_STATUS
      #undef I2C_DRV_SDA_STATUS
			
			break;
			
		case AESS_I2CDRV_CTRL_HW:
			
			DEBUGP("KN: ioctl AESS_I2CDRV_CTRL_HW\n");
			
			psI2CDrvBusInfo = (sI2CDrvBusInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psI2CDrvBusInfo, 
						  sizeof(sI2CDrvBusInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* error occur */
				return -EFAULT;
			}
			
			/* retrieve bus number */
			u8BusNumber = psI2CDrvBusInfo->u8Channel;
			
			/* get semaphore */
			if(down_interruptible((struct semaphore *) &i2c_buffer[u8BusNumber].sem))
			{
				return -ERESTARTSYS;
			}
			
			result = aess_i2c_ctrlhw((sI2CDrvBusInfoType *) psI2CDrvBusInfo);
			
			if (result)
			{
				/* release semaphore */
				up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
				
				return (result);
			}
			
			/* release semaphore */
			up((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
			
			DEBUGP("I2CDrv(%d):AESS_I2CDRV_CTRL_HW!\n",u8BusNumber);
			
			break;
			
		default:
			
			break;
	}
	
	return 0;
}


#ifdef I2C_POLLING_STATUS
void aess_i2c_polling_func(unsigned long data)
{
	UINT8 u8BusNumber;
	
	for (u8BusNumber = 0; u8BusNumber < I2C_DRV_MAX_BUS; u8BusNumber++)
	{
		if (i2c_buffer[u8BusNumber].u8HasInit == 1)
		{
			printk("KN:Bus(%d) ST:0x%02x CST:0x%02x CTL1:0x%02x CTL2:0x%02x CTL3:0x%02x ADDR1:0x%02x ADDR2:0x%02x state:%02d\n",
				   u8BusNumber,
				   ioread8((void *) i2c_reg[u8BusNumber].status),
				   ioread8((void *) i2c_reg[u8BusNumber].control_status),
				   ioread8((void *) i2c_reg[u8BusNumber].control1),
				   ioread8((void *) i2c_reg[u8BusNumber].control2),
				   ioread8((void *) i2c_reg[u8BusNumber].control3),
				   ioread8((void *) i2c_reg[u8BusNumber].own_address1),
				   ioread8((void *) i2c_reg[u8BusNumber].own_address2),
				   i2c_buffer[u8BusNumber].u8State);
		}
	}
	
	/* timer interval is 10s */
	i2c_polling.expires = jiffies + 1000;
	add_timer(&i2c_polling);
	
	return;
}
#endif


static int aess_i2c_release(struct inode *inode, struct file *filp)
{
	UINT8 u8BusNumber;
	UINT32 flags;	
	i2c_reg_type *CurrentReg;	
	
	DEBUGP("KN: aess_i2c_release enter\n");

#ifdef I2C_BUS_RECOVER
	del_timer(&i2c_bus_recover);
#endif

#ifdef I2C_POLLING_STATUS
	del_timer(&i2c_polling);
#endif
	
	for (u8BusNumber = 0; u8BusNumber < I2C_DRV_MAX_BUS; u8BusNumber++)
	{
		if (i2c_buffer[u8BusNumber].u8HasInit == 1)
		{
			CurrentReg = (i2c_reg_type *) &i2c_reg[u8BusNumber];

			/* clear status register */
			iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->status);
			  
			/* clear control status register */			  
			iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->control_status);			  

			/* clear control1 register */
			iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->control1);
			  
			/* clear control2 register */			  
			iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->control2);			  

			/* clear control3 register */			  
			iowrite8(CLEAR_ALL_BITS, (void *) CurrentReg->control3);			

			/* reset */
			aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
			
			if (i2c_buffer[u8BusNumber].u8InitMode == I2C_DRV_MODE_IPMB)
			{
				DEBUGP("KN: free kfifo memory\n");
				/* free allocated memory */
				kfifo_free(i2c_buffer[u8BusNumber].psFIFO);
			}			  
			
			/* clear the initial status flag */
			i2c_buffer[u8BusNumber].u8HasInit = 0;
		}
	}
	
	/* reset to power up state */
	spin_lock_irqsave(&init_state_lock, flags);
	i2c_init_state = 0;
	spin_unlock_irqrestore(&init_state_lock, flags);
	DEBUGP("KN: aess_i2c_release exit\n");	
	return 0;
}


static int aess_i2c_open(struct inode *inode, struct file *filp)
{
	UINT8 u8BusNumber;
	UINT32 flags;
	
	DEBUGP("KN: aess_i2c_open\n");
	
	/* memory allocate */
	
	/* reset i2c modules if reopen */
	if (i2c_init_state == 1)
	{
		DEBUGP("KN: i2c reopen\n");
#if 0
#ifdef I2C_BUS_RECOVER
		del_timer(&i2c_bus_recover);
#endif
		
#ifdef I2C_POLLING_STATUS
		del_timer(&i2c_polling);
#endif
		
		for (u8BusNumber = 0; u8BusNumber < I2C_DRV_MAX_BUS; u8BusNumber++)
		{
			if (i2c_buffer[u8BusNumber].u8HasInit == 1)			
			{
				/* reset */
				aess_i2c_reset((sI2CDrvBufferType *) &i2c_buffer[u8BusNumber]);
			}
		}
		
#ifdef I2C_BUS_RECOVER
		/* initiate polling timer, interval is 5s */
		i2c_bus_recover.expires = jiffies + 100;
		
		add_timer(&i2c_bus_recover);
#endif
		
#ifdef I2C_POLLING_STATUS
		/* initiate polling timer, interval is 10s */
		i2c_polling.expires = jiffies + 1000;
		
		add_timer(&i2c_polling);
#endif
#else
    return -EBUSY;
#endif
	}
	else
	{
		DEBUGP("KN: init variables\n");
		for (u8BusNumber = 0; u8BusNumber < I2C_DRV_MAX_BUS; u8BusNumber++)
		{
			/* initiate semaphores */
			init_MUTEX((struct semaphore *) &i2c_buffer[u8BusNumber].sem);
			
			i2c_buffer[u8BusNumber].u8HasInit = 0;			
	        i2c_buffer[u8BusNumber].u8ErrorStatus = I2C_DRV_ERROR_NONE;
	        i2c_buffer[u8BusNumber].u8MsgSendDataSize = 0;
	        i2c_buffer[u8BusNumber].u8MsgSendCounter = 0;
	        i2c_buffer[u8BusNumber].u8MsgRecDataSize = 0;
	        i2c_buffer[u8BusNumber].u8MsgRecCounter = 0;		
	        i2c_buffer[u8BusNumber].u16CurStartCount = 0;
	        i2c_buffer[u8BusNumber].u16CurStopCount = 0;			
		}
		
#ifdef I2C_BUS_RECOVER
		/* initiate polling timer */
		i2c_bus_recover.data = 0;
		i2c_bus_recover.function = &aess_i2c_bus_recover_polling_func;
		
		/* timer interval is 10s */
		i2c_bus_recover.expires = jiffies + 100;
		
		init_timer(&i2c_bus_recover);
		add_timer(&i2c_bus_recover);
#endif
		
#ifdef I2C_POLLING_STATUS
		/* initiate polling timer */
		i2c_polling.data = 0;
		i2c_polling.function = &aess_i2c_polling_func;
		
		/* timer interval is 10s */
		i2c_polling.expires = jiffies + 1000;
		
		init_timer(&i2c_polling);
		add_timer(&i2c_polling);
#endif
		spin_lock_irqsave(&init_state_lock, flags);
		i2c_init_state = 1;
		spin_unlock_irqrestore(&init_state_lock, flags);
	}
	
	return 0;
}


struct file_operations aess_i2c_fops = {
	.owner = THIS_MODULE,
	.open = aess_i2c_open,
	.release = aess_i2c_release,
	.ioctl = aess_i2c_ioctl,
};


int __init aess_i2c_init(void)
{
	int result;
	
	DEBUGP("KN: init_aess_i2c_module\n");
	
	/* Allocate a char device */
	i2c_cdev = cdev_alloc();
	
	i2c_cdev->owner = THIS_MODULE;
	i2c_cdev->ops = &aess_i2c_fops;
	
	/* Allocate a device number */
	result = alloc_chrdev_region(&i2c_dev, 0, 1, driver_name);
	
	DEBUGP("KN: Major number %d\n", MAJOR(i2c_dev));
	
	/* Show this message for i2c install shell used */
	printk("mknod /dev/aess_i2cdrv c %d 0\n", MAJOR(i2c_dev));
	
	if (result)
	{
		DEBUGP(KERN_ERR "KN: Registering the character device failed with %d\n", MAJOR(i2c_dev));
		
		return result;
	}
	
	/* Add a char device */
	cdev_add(i2c_cdev, i2c_dev, 1);
	
	/* Request SMB Group IRQ */                             
	result = request_irq(IRQ_SMBGROUP,    
						           (void *) aess_i2cgroup_isr,            
						           IRQF_SAMPLE_RANDOM | IRQF_SHARED,                
						           "I2CG",   
						           (void *) &i2c_buffer[0]);

	if (result)
	{
		DEBUGP("KN: %d request IRQ error\n", result);
	}

	/* Request SMB3 IRQ */                             
	result = request_irq(IRQ_SMB3,    
						           (void *) aess_i2c3_isr,            
						           IRQF_SAMPLE_RANDOM | IRQF_SHARED,                
						           "I2C3",
						           (void *) &i2c_buffer[0]);

	if (result)
	{
		DEBUGP("KN: %d request IRQ error\n", result);
	}

	/* Request SMB4 IRQ */                             
	result = request_irq(IRQ_SMB4,    
						           (void *) aess_i2c4_isr,            
						           IRQF_SAMPLE_RANDOM | IRQF_SHARED,                
						           "I2C4",
						           (void *) &i2c_buffer[0]);

	if (result)
	{
		DEBUGP("KN: %d request IRQ error\n", result);
	}
						 
	/* Request SMB5 IRQ */                             
	result = request_irq(IRQ_SMB5,    
						           (void *) aess_i2c5_isr,            
						           IRQF_SAMPLE_RANDOM | IRQF_SHARED,                
						           "I2C5",
						           (void *) &i2c_buffer[0]);						 						 

  spin_lock_init(&init_state_lock);

	/* Enable SMB Interrupt Group */
	set_bit(AIC_GEN_SMB2,(void *)(AIC_GEN_REG));
	set_bit(AIC_GEN_SMB1,(void *)(AIC_GEN_REG));
	set_bit(AIC_GEN_SMB0,(void *)(AIC_GEN_REG));  	   
  
	if (result)
	{
		DEBUGP("KN: %d request IRQ error\n", result);
	}
	
	return 0;
}


void __exit aess_i2c_exit(void)
{
	/* free the irq */
	free_irq(IRQ_SMB5, (void *)&i2c_buffer[0]);

	/* free the irq */
	free_irq(IRQ_SMB4, (void *)&i2c_buffer[0]);

	/* free the irq */
	free_irq(IRQ_SMB3, (void *)&i2c_buffer[0]);

	/* free the irq */
	free_irq(IRQ_SMBGROUP, (void *)&i2c_buffer[0]);
				
	/* release the char device */
	cdev_del(i2c_cdev);
	
	/* release the device number */
	unregister_chrdev_region(i2c_dev, 1);
	
	return;
}


MODULE_DESCRIPTION("AESS WPCM450 I2C Driver");
MODULE_AUTHOR("Timothy Huang <timothy.huang@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_i2c_init);
module_exit(aess_i2c_exit);
