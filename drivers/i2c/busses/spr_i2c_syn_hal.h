/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by Rajeev Kumar 			*/
/*   rajeev-dlh kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/


#ifndef SPEAR_I2C_SYN_HAL_H
#define SPEAR_I2C_SYN_HAL_H

#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <linux/types.h>
#include <asm/arch/hcl_defs.h>
#include <linux/delay.h>



#define I2C_MODULE_NAME "I2C HCL Module"

#define IC_HS_MASTER_CODE 3


//Base Address of I2c Devices..

#ifdef CONFIG_ARCH_SPEARPLUS
#define SPEAr_I2C_APB_BASE VA_SPEARPLUS_START_I2C
#define APB_I2C_CFG_BASE   VA_SPEARPLUS_START_I2C + 0xf0
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
#define SPEAr_I2C_APB_BASE VA_SPEARBASIC_START_I2C
#define APB_I2C_CFG_BASE   VA_SPEARBASIC_START_I2C + 0xf0
#endif

//I2C Registers Base Address Mapping

#define IC_CON   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0000))
#define IC_TAR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0004))
#define IC_SAR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0008))
#define IC_HS_MADDR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x000C)) 	//IC_HS_MAR holds the value of I2c HS mode master code
#define IC_CMD_DATA   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0010))

#define IC_SS_SCL_HCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0014))		// sets the scl clock high =period count for standered speed.
#define IC_SS_SCL_LCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0018))	

#define IC_FS_SCL_HCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x001C))	
#define IC_FS_SCL_LCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0020))	

#define IC_HS_SCL_HCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0024))	
#define IC_HS_SCL_LCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0028))	

#define IC_INTR_STAT   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x002C))
#define IC_INTR_MASK   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0030))
#define IC_RAW_INTR_STAT ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0034))

#define IC_RX_TL   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0038))
#define IC_TX_TL   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x003C))


//Clear Interupts; Read Only Registers

#define IC_CLR_INTR     ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0040))
#define IC_CLR_RX_UNDER ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0044))
#define IC_CLR_RX_OVER  ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0048))
#define IC_CLR_TX_OVER  ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x004C))
#define IC_CLR_RD_REQ  	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0050))
#define IC_CLR_TX_ABRT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0054))
#define IC_CLR_RX_DONE 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0058))
#define IC_CLR_ACTIVITY	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x005C)) 
#define IC_CLR_STOP_DET	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0060)) 
#define IC_CLR_START_DET ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0064)) 
#define IC_CLR_GEN_CALL	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0068)) 


#define IC_ENABLE   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x006C)) //R/W
#define IC_STATUS   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0070)) //Read only
#define IC_TXFLR	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0074)) //Read only
#define IC_RXFLR	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0078)) //Read only

#define IC_TXABRT_SOURCE	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0080)) //Read/Write 

#define IC_DMA_CR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0088))
#define IC_DMA_TDLR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x008C))
#define IC_DMA_RDLR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0090))


#define IC_COMP_PARAM_1 ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x00F4))  //Read Only
#define IC_COMP_VERSION ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x00F8))  //Read Only
#define IC_COMP_TYPE	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x00Fc)) //Read Only

/*function for disabling and enabling I2C */
#define I2C_Disable()			( *IC_ENABLE &=~IC_ENABLE_0B) 
#define I2C_Enable()			( *IC_ENABLE |= IC_ENABLE_0B ) 
#define I2C_set_CON(x)			( *IC_CON = (unsigned int)x)
#define I2C_TAR(x)			( *IC_TAR = (unsigned int)x)
#define I2C_SAR(x)			( *IC_SAR = (unsigned int)x)
#define I2C_HS_MADDR(x)			( *IC_HS_MADDR = (unsigned int)x)
#define I2C_set_HS_SCL_HCNT(x)		( *IC_HS_SCL_HCNT = (unsigned int)x)
#define I2C_set_HS_SCL_LCNT(x)		( *IC_HS_SCL_LCNT = (unsigned int)x)
#define I2C_set_FS_SCL_HCNT(x)		( *IC_FS_SCL_HCNT = (unsigned int)x)
#define I2C_set_FS_SCL_LCNT(x)		( *IC_FS_SCL_LCNT = (unsigned int)x)
#define I2C_set_SS_SCL_HCNT(x)		( *IC_SS_SCL_HCNT = (unsigned int)x)
#define I2C_set_SS_SCL_LCNT(x)		( *IC_SS_SCL_LCNT = (unsigned int)x)
#define I2C_set_INTR_MASK(x)		( *IC_INTR_MASK = (unsigned int)x )
#define I2C_set_TX_TL(x)		( *IC_TX_TL = (unsigned int)x)
#define I2C_set_RX_TL(x)		( *IC_RX_TL = (unsigned int)x)
#define I2C_set_TXFLR(x)		( *IC_TXFLR = (unsigned int)x)
#define I2C_set_RXFLR(x)		( *IC_RXFLR = (unsigned int)x)
#define I2C_set_TX_ABRT_SOURCE(x)	( *IC_RXFLR = (unsigned int)x)

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define FIFO_SIZE 7
//#define BLK_SIZE 8
#define BLK_SIZE 16
#define IC_CLK 166
#define MAX_OFFSET 256
#define  BOUNDRY_CONDITION 15
//#define  BOUNDRY_CONDITION 7
//#define IC_CLK 83
#define NANO_TO_MICRO 1000		

#define SS_SCL_HCNT 		0x03e8
#define SS_SCL_LCNT 		0x03e8
#define MIN_SS_SCL_HIGHTIME  	4000	 //in nanosec
#define MIN_SS_SCL_LOWTIME   	5000	 //nanosec

#define FS_SCL_HCNT 		0x003c
#define FS_SCL_LCNT 		0x0082
//#define	MIN_FS_SCL_HIGHTIME  	800	 //in nanosec
//#define MIN_FS_SCL_LOWTIME   	1700	 //nanosec
#define	MIN_FS_SCL_HIGHTIME  	602 //in nanosec
#define MIN_FS_SCL_LOWTIME   	1302	 //nanosec

#define HS_SCL_HCNT 		0x0006
#define HS_SCL_LCNT 		0x000d
#define HS_BUS_LOADING 		100
#define MIN_HS_SCL_HIGHTIME 	60	 //in nanosec
#define MIN_HS_SCL_LOWTIME   	160	 //nanosec


#define IC_ABRT_7B_ADDR_NOACK 0x0001
#define IC_ABRT_TXDATA_NOACK  0x0008




/*---------------------------------------------------------------------------------------
	I2C Control register

	IC_CON [15:7] = Reserved
	IC_CON [6] =	IC_SLAVE_DISABLE
	IC_CON [5] = 	IC_RESTART_ENABLE
	IC_CON [4] = 	IC_10BITADDR_MASTER
	IC_CON [3] = 	IC_10BITADDR_SLAVE
	IC_CON [2:1] =  SPEED
     		   	00 = None
			01 = IC_SPEED_MODE_STANDARD,        	            // Standard I2C mode (SCL < 100 KHz)
			10 = IC_SPEED_MODE_FAST,	           	    // Fast I2C mode (100 KHz <= SCL < 400 KHz)
			03 = IC_MAX_SPEED_MODE				    // 3.4 Mbits		
	IC_CON [0] = 	MASTER_MODE
	

----------------------------------------------------------------------------------------*/
typedef enum
	{
		IC_CON_SD		= 0x0040,			      //IC_SLAVE_DISABLE
		IC_CON_RE		= 0x0020,  			      //IC_RESTART_ENABLE
		IC_CON_10BITADDRMASTER	= 0x0010, 			      //IC_10BITADDR_MASTER
		IC_CON_10BITADDR_SLAVE	= 0x0008, 			      //IC_10BITADDR_SLAVE
   		IC_CON_SPH		= 0x0006,	            	      // Control Speed 3: High Speed mode
		IC_CON_SPF		= 0x0004, 			      //Control Speed 2:fast mode
		IC_CON_SPL		= 0x0002, 			      //Control Speed 1: standered mode
		IC_CON_MM 		= 0x0001			      //control master mode, 
	}t_I2C_ControlReg;


/*---------------------------------------------------------------------------------------
	I2C Target address register

	IC_TAR [15:13] = Reserved
	IC_TAR [12] =    IC_10BITADDR_MASTER
	IC_TAR [11] = 	 SPECIAL
	IC_TAR [10] =    GC_OR_START // IF 0= General Call setup; 1= StartByte
	IC_TAR [9:0] =   IC_TAR
	

----------------------------------------------------------------------------------------*/

#define TAR_ADDR           0x0050

typedef enum
	{
		IC_TAR_10ADM =0x1000,//                          
	 	IC_TAR_SP    =0x0800,//                        
	 	IC_TAR_GS    =0x0400,//                                
 	 	IC_TAR_9B    =0x0200,//=========================        
	 	IC_TAR_8B    =0x0100,//                                    
	 	IC_TAR_7B    =0x0080,//                                
	 	IC_TAR_6B    =0x0040,//                                 
	        IC_TAR_5B    =0x0020,// 10bit of target address
	 	IC_TAR_4B    =0x0010,// 
	 	IC_TAR_3B    =0x0008,//                         
	 	IC_TAR_2B    =0x0004,//                                 
	 	IC_TAR_1B    =0x0002,// 
	 	IC_TAR_0B    =0x0001,//==========================    
// 	 	IC_TAR_NB    =0x1c00, 
	 	IC_TAR_NB    =0x0000 // 7bit
//	 	IC_TAR_10B   =0x1000 // 10bit
	}t_I2C_tar;


/*---------------------------------------------------------------------------------------
	I2C Slave address register

	IC_SAR [15:10] = Reserved
	IC_SAR [9:0] =   IC_SAR	
#offset addr = 0x08
----------------------------------------------------------------------------------------*/

#define IC_SLAVE_ADDR	 0x0002 //

/*---------------------------------------------------------------------------------------
	I2C Data Buffer and Command register

	IC_DATA_CMD [15:9] = Reserved
	IC_DATA_CMD [8] =    CMD
	IC_DATA_CMD [7:0] =  DAT
#offset addr = 0x10
	
----------------------------------------------------------------------------------------*/

//setting cmd bit to '1'  for read

#define IC_CMD 0x0100 


/*---------------------------------------------------------------------------------------
	Interrupt Status Register [Read only registers]

	IC_INTR_STAT [15:12]=	Reserevd
        IC_INTR_STAT [11]=	R_GEN_CALL 	
   	IC_INTR_STAT [10]=	R_START_DETECT	
	IC_INTR_STAT [9]=	R_STOP_DETECT	
	IC_INTR_STAT [8]=	R_ACTIVITY	
	IC_INTR_STAT [7]=	R_RX_DONE	
	IC_INTR_STAT [6]=	R_TX_ABRT	
	IC_INTR_STAT [5]=	R_RD_REQ	
	IC_INTR_STAT [4]=	R_TX_EMPTY	     
	IC_INTR_STAT [3]=	R_TX_OVER	
	IC_INTR_STAT [2]=	R_RX_FULL	
	IC_INTR_STAT [1]=	R_RX_OVER	
	IC_INTR_STAT [0]=	R_RX_UNDER	

#offset addr= 0x2C    	

----------------------------------------------------------------------------------------*/

typedef enum
{

	IC_R_GEN_CALL 	= 0x0800,
	IC_R_START_DET	= 0x0400,
	IC_R_STOP_DET	= 0x0200,
	IC_R_ACTIVITY	= 0x0100,
	IC_R_RX_DONE	= 0x0080,
	IC_R_TX_ABRT	= 0x0040,
	IC_R_RD_REQ	= 0x0020,
	IC_R_TX_EMPTY	= 0x0010,	
	IC_R_TX_OVER	= 0x0008,
	IC_R_RX_FULL	= 0x0004,
	IC_R_RX_OVER	= 0x0002,
	IC_R_RX_UNDER	= 0x0001
    	
}t_I2CIntStatusType;
/*---------------------------------------------------------------------------------------
	Interrupt Status Mask Register [Read/Write  registers]

	IC_INTR_MASK [15:12]=	Reserevd
        IC_INTR_MASK [11]=	M_GEN_CALL 	
   	IC_INTR_MASK [10]=	M_START_DETECT	
	IC_INTR_MASK [9]=	M_STOP_DETECT	
	IC_INTR_MASK [8]=	M_ACTIVITY	
	IC_INTR_MASK [7]=	M_RX_DONE	
	IC_INTR_MASK [6]=	M_TX_ABRT	
	IC_INTR_MASK [5]=	M_RD_REQ	
	IC_INTR_MASK [4]=	M_TX_EMPTY	     
	IC_INTR_MASK [3]=	M_TX_OVER	
	IC_INTR_MASK [2]=	M_RX_FULL	
	IC_INTR_MASK [1]=	M_RX_OVER	
	IC_INTR_MASK [0]=	M_RX_UNDER	
    	
#offset addr= 0x30
----------------------------------------------------------------------------------------*/


typedef enum
{

	IC_M_GEN_CALL 	= 0x0800,
   	IC_M_START_DET	= 0x0400,
	IC_M_STOP_DET	= 0x0200,
	IC_M_ACTIVITY	= 0x0100,
	IC_M_RX_DONE	= 0x0080,
	IC_M_TX_ABRT	= 0x0040,
	IC_M_RD_REQ	= 0x0020,
	IC_M_TX_EMPTY	= 0x0010,	
	IC_M_TX_OVER	= 0x0008,
	IC_M_RX_FULL	= 0x0004,
	IC_M_RX_OVER	= 0x0002,
	IC_M_RX_UNDER	= 0x0001
    	
}t_I2CIntMaskType;

/*---------------------------------------------------------------------------------------
	I2C Raw Interpol Status Register [Read only registers]

	IC_INTR_MASK [15:12]=	Reserevd
        IC_INTR_MASK [11]=	GEN_CALL 	
   	IC_INTR_MASK [10]=	START_DETECT	
	IC_INTR_MASK [9]=	STOP_DETECT	
	IC_INTR_MASK [8]=	ACTIVITY	
	IC_INTR_MASK [7]=	RX_DONE	
	IC_INTR_MASK [6]=	TX_ABRT	
	IC_INTR_MASK [5]=	RD_REQ	
	IC_INTR_MASK [4]=	TX_EMPTY	     
	IC_INTR_MASK [3]=	TX_OVER	
	IC_INTR_MASK [2]=	RX_FULL	
	IC_INTR_MASK [1]=	RX_OVER	
	IC_INTR_MASK [0]=	RX_UNDER	
    	
#offset addr= 0x34
----------------------------------------------------------------------------------------*/


typedef enum
{

	IC_GEN_CALL 		= 0x0800,
   	IC_START_DET		= 0x0400,
	IC_STOP_DET		= 0x0200,
	IC_RAW_ACTIVITY		= 0x0100,
	IC_RX_DONE		= 0x0080,
	IC_TX_ABRT		= 0x0040,
	IC_RD_REQ		= 0x0020,
	IC_TX_EMPTY		= 0x0010,	
	IC_TX_OVER		= 0x0008,
	IC_RX_FULL		= 0x0004,
	IC_RX_OVER		= 0x0002,
	IC_RX_UNDER		= 0x0001
    	
}t_I2C_RAW_IntType;

/*--------------------------------------------------------------------------------------
	I2C Enable register

	IC_ENABLE [15:1] = Reserved
	IC_ENABLE [0]   = ENABLE 

#offset address = 0x6c
---------------------------------------------------------------------------------------*/

#define IC_ENABLE_0B	0x0001

//I2c Status Register;

typedef enum
{
	IC_STATUS_SA	=0x0040,
	IC_STATUS_MA	=0x0020,
	IC_STATUS_RFF	=0x0010,
	IC_STATUS_RFNE	=0x0008,
	IC_STATUS_TFE	=0x0004,
	IC_STATUS_TFNF	=0x0002,
	IC_STATUS_ACT	=0x0001
}t_I2C_Status;


// DMA registers
/*--------------------------------------------------------------------------------------
	I2C DMA Control register

	IC_DMA_CR [31:2] = Reserved
	IC_DMA_CR [1]   =  TDMAE //0=Transmit DMA disable,1= Transmit DMA enabled
	IC_DMA_TDLR [0] = RDMAE //0= Receive DMA disable,1= Transmit DMA enabled

#offset address = 0x88
---------------------------------------------------------------------------------------*/
typedef enum
	{
		IC_TDMAE_DISABLE	= 0x0000,
		IC_TDMAE_ENABLE		= 0x0002,
		IC_RDMAE_DISABLE	= 0x0000,
		IC_RDMAE_ENABLE		= 0x0001
	}t_I2C_Dma_Control;

/*--------------------------------------------------------------------------------------
	I2C DMA Transmit Data level register

	IC_DMA_TDLR [31:2] = Reserved
	IC_DMA_TDLR [1:0]   =  DMATDl

#offset address = 0x8c
---------------------------------------------------------------------------------------*/









/*--------------------------------------------------------------------------------------
	I2C DMA Receive Data level register

	IC_DMA_RDLR [31:2] =   Reserved
	IC_DMA_RDLR [1:0]   =  DMARDL

#offset address = 0x90
---------------------------------------------------------------------------------------*/



//specifies the working mode of I2C
typedef enum 
{
	I2C_SLAVE_MODE = 0,   		// Device in Slave Mode
	I2C_MASTER_MODE,		// Device in Master Mode
	I2C_SLAVE_TRANSMITTER,	 	// Device in Slave transmitter Mode
	I2C_SLAVE_RECEIVER,	 	// Device in Slave Receiver Mode
	I2C_MASTER_TRANSMITTER,	 	// Device in Master transmitter Mode
	I2C_MASTER_RECEIVER	 	// Device in Master Receiver Mode
				
} t_I2C_control_mode;


//specifies the mode in which the APB interface is configured for the current transfer. 
//This mode may be separately specified for index and data transfer.

typedef enum
{
	I2C_TRANSFER_MODE_POLLING,		// Transfer takes place in polling mode
 	I2C_TRANSFER_MODE_INTERRUPT,	        // Transfer takes place in interrupt mode
   	I2C_TRANSFER_MODE_DMA			// Transfer takes place in DMA mode
 }t_I2C_transfer_mode;

/*
typedef enum
{
	IC_COMP_PARAM_1,      //0xf4
	IC_COMP_VERSION,      //0xf8
	IC_COMP_TYPE 	    //0xfc
}t_I2C_Config;
*/

typedef enum
{
	I2C_DYNAMIC_TAR_UPDATE= 0,
	I2C_DYNAMIC_NO_TAR_UPADATE
}t_I2C_tar_update;


//Addressing mode
typedef enum
{
	SEVEN_BIT_ADDRESSING=0,
	TEN_BIT_ADDRESSING

}t_I2C_addressing_mode;


//First Byte
typedef enum
	{
		ISSUE_START_BYTE=0,
		ISSUE_GENERAL_CALL
	}t_I2C_command;

//Speed Selection
typedef enum
	{
		IC_SPEED_MODE_STANDARD	= 1,        	             // Standard I2C mode (SCL < 100 KHz)
		IC_SPEED_MODE_FAST,	     		      	     // Fast I2C mode (100 KHz <= SCL < 400 KHz)
		IC_MAX_SPEED
	}t_I2C_speed;


typedef struct
	{
		u32 SlaveAddress;
		u32 slave_offset;
		t_I2C_control_mode mode;
		t_I2C_addressing_mode operation;
		t_I2C_tar_update tar_update;
		t_I2C_command FirstByte;
		t_I2C_transfer_mode TransferMode;
		t_I2C_speed	Speed;
	}t_I2C_Configure;
typedef enum
	{
		I2C_OK = 0,
		I2C_FAIL,
	 	I2C_INVALID_ADDRESS,
		I2C_RX_BUFFER_NULL,
		I2C_NO_RX_DATA,
		I2C_TX_BUFFER_NULL,
		I2C_NO_TX_DATA,
		I2C_OFFSET_ERROR,
		I2C_TX_FIFO_NOT_EMPTY
	
	}t_I2CError;


//Function declarations.

void I2C_Init (void);
void I2C_Configure(void );
void I2C_SpeedSelect(int i2c_SPD);
void I2C_Set_Address(int i2c_addr);
void I2C_DelayTX(void);
void I2C_DelayRX(void);
void I2C_TransmitByte(int i2c_cmd_data);
//unsigned char I2C_Set_Read(void);
int I2C_Set_Read(void);
void  I2C_Stop_Detect(void );
//unsigned char I2C_ReceiveByte(void);
int I2C_ReceiveByte(void);

size_t I2C_TransmitBuffer(t_I2C_Configure *configure,u8 *ptrToTxBuffer,u32 NoOfBytes,u32 slave_offset );
size_t I2C_ReceiveBuffer(t_I2C_Configure *configure,u8 *ptrToRxBuffer,u32 NoOfBytes,u32 slave_offset );
void I2C_SetHighFrequency(void );
void I2C_SetFastFrequency(void );
void I2C_SetStanderedFrequency( void);
 #endif //_I2C_H

