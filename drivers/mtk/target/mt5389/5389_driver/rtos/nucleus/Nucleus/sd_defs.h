/*************************************************************************
*                                                                       
*               Copyright Mentor Graphics Corporation 2002              
*                         All Rights Reserved.                          
*                                                                       
* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS  
* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS   
* SUBJECT TO LICENSE TERMS.                                             
*                                                                       
*************************************************************************/

/*************************************************************************
*                                                                      
* FILE NAME                                    VERSION                    
*                                                                      
*      sd_defs.h                       Nucleus PLUS\Integrator\ADS 1.14.8 
*                                                                      
* COMPONENT                                                            
*                                                                      
*      SD - Serial Driver                                              
*                                                                      
* DESCRIPTION                                                          
*                                                                      
*      This file contains constant definitions and function macros     
*      for the Serial Driver module.                                   
*                                                                      
* DATA STRUCTURES                                                      
*                                                                      
*      SD_PORT     :   Structure to keep all needed info. about a port.
*                                                                      
* DEPENDENCIES                                                         
*                                                                      
*      none    
*
*                                                                      
*************************************************************************/
#ifndef SD_DEFS_H
#define SD_DEFS_H

/**************** User configurable section *************************/

/* The UART clock is 14.7456 MHz */
#define UART_CLOCK            14745600

/**************** End configurable section *************************/

/* Address of the interrupt enable register */
#define SD_INT_ENABLE           0x14000008
#define SD_INT_UART0            (1<<1)
#define SD_INT_UART1            (1<<2)

/* Address of the core module status register */
#define SD_CM_STAT              0x10000010
#define SD_CM_STAT_MASK         0x0F
#define SD_CM_OFFSET_SHIFT      6

/* The base addresses for the seperate UART registers. */
#define SD_UART0_BASE           0x16000000
#define SD_UART1_BASE           0x17000000

/* Macros for specifying which UART to use. */
#define SD_UART0                0
#define SD_UART1                1
#define SD_MAX_UARTS            2

/* UART register offsets from the UART base. */
#define SD_DR_OFFSET            0x00

#define SD_RSR_OFFSET           0x04
#define SD_ECR_OFFSET           0x04

#define SD_LCRH_OFFSET          0x08

#define SD_LCRM_OFFSET          0x0C

#define SD_LCRL_OFFSET          0x10

#define SD_CR_OFFSET            0x14

#define SD_FR_OFFSET            0x18

#define SD_IIR_OFFSET           0x1C
#define SD_I_CR_OFFSET          0x1C



/* These use generic type names, leaving off the register name
   in the macro, because they are used by generic sections of
   code which will not require changes for other UARTS. Only the
   bits these correspond to should change. */
#define SD_MODE_NORMAL              0x00

/* UART Line Control Register Bits */
#define SD_PARITY_NONE              0x00
#define SD_PARITY_EVEN              0x06
#define SD_PARITY_ODD               0x02

#define SD_DATA_BITS_5              0x00
#define SD_DATA_BITS_6              0x20
#define SD_DATA_BITS_7              0x40
#define SD_DATA_BITS_8              0x60

#define SD_STOP_BITS_1              0x00
#define SD_STOP_BITS_2              0x08

#define SD_ENABLE_FIFO              0x10

/* These use specific type names, putting the register name
   in the macro, because these macros are used by port specific
   sections of code and will most likely have different names
   on other UARTS. */

/* UART Command Register Bits */
#define SD_CR_UART_ENABLE          0x01
#define SD_CR_STAT_IRQ_ENABLE      0x08
#define SD_CR_RX_ENABLE            0x10
#define SD_CR_TX_ENABLE            0x20
#define SD_CR_RX_TO_ENABLE         0x40
#define SD_CR_LOOPBACK_ENABLE      0x80

/* UART Status Register Bits */
#define SD_RSR_FRAME_ERR            0x01
#define SD_RSR_PARITY_ERR           0x02
#define SD_RSR_BREAK_ERR            0x04
#define SD_RSR_OVERRUN              0x08

#define SD_ECR_RST_ERR_STAT         0x01

#define SD_FR_RXREADY               0x10
#define SD_FR_TXREADY               0x80
#define SD_FR_UARTBUSY              0x08

#define SD_IIR_RX_INT               0x02
#define SD_IIR_TX_INT               0x04

#define SD_I_CR_INT_CLEAR           0xFF
#define SD_I_CR_CLEAR_OTHERS        0x09

#define SD_UIVR_UART0_VECTOR        1
#define SD_UIVR_UART1_VECTOR        2

/* Define default Serial Driver settings for this board */
#define     DEFAULT_UART_PORT       SD_UART1
#define     DEFAULT_PPP_BAUD        57600
#define     DEFAULT_UART_BAUD       57600
#define     DEFAULT_UART_DATA       DATA_BITS_8
#define     DEFAULT_UART_STOP       STOP_BITS_1
#define     DEFAULT_UART_PARITY     PARITY_NONE
#define     DEFAULT_UART_MODE       MODE_NORMAL
#define     DEFAULT_UART_BUFFER     5000


/* Define data structures for management of a serial port. */

typedef struct SD_INIT_STRUCT
{
    UINT32        data_mode;
    UINT32        base_address;

    /* The following elements should be generic across other
       platforms. */
    NU_SEMAPHORE    *sd_semaphore;
    UINT32          com_port;
    UINT32          data_bits;
    UINT32          stop_bits;
    UINT32          parity;
    UINT32          baud_rate;
/************** Begin Port Specific Section ****************/
    UINT32          vector;
/************** Begin Port Specific Section ****************/
    UINT32          driver_options;
    UINT32          sd_buffer_size;

    UINT32          parity_errors;
    UINT32          frame_errors;
    UINT32          overrun_errors;
    UINT32          busy_errors;
    UINT32          general_errors;

    CHAR            *rx_buffer;
    INT             rx_buffer_read;
    INT             rx_buffer_write;
    volatile INT    rx_buffer_status; 

    /* All of the following elements are required by PPP, do not modify. */
    UINT32          communication_mode;
    CHAR            *tx_buffer;
    INT             tx_buffer_read;
    INT             tx_buffer_write;
    volatile INT    tx_buffer_status; 

} SD_PORT;

/* Defines to be used by application */
#define MODE_NORMAL             SD_MODE_NORMAL
#define MODE_AUTO_ECHO    SD_MODE_AUTO_ECHO
#define MODE_LOCAL_LOOP   SD_MODE_LOCAL_LOOP
#define MODE_REMOTE_LOOP  SD_MODE_REMOTE_LOOP

#define STOP_BITS_1       SD_STOP_BITS_1
#define STOP_BITS_2       SD_STOP_BITS_2

#define UART0             SD_UART0
#define UART1             SD_UART1

/* Defines to determine communication mode */
#define SERIAL_MODE                 0
#define SERIAL_MOUSE                3
/* MDM_NETWORK and MDM_TERMINAL do not need to be defined here
   since they are defined in PPP.  */

/* This is specifically for uHAL */
typedef VOID (*NU_PrHandler)(INT);
#define UHAL_UART0_INT            1
#define UHAL_UART1_INT            2


/***********************************************************************
 Note: everything below should be generic.
*/

#define NU_SERIAL_PORT          SD_PORT
#define PARITY_NONE             SD_PARITY_NONE
#define PARITY_EVEN             SD_PARITY_EVEN
#define PARITY_ODD              SD_PARITY_ODD

#define DATA_BITS_6             SD_DATA_BITS_6
#define DATA_BITS_7             SD_DATA_BITS_7
#define DATA_BITS_8             SD_DATA_BITS_8

#define NU_SD_Put_Char          SDC_Put_Char
#define NU_SD_Get_Char          SDC_Get_Char
#define NU_SD_Put_String        SDC_Put_String
#define NU_SD_Init_Port         SDC_Init_Port
#define NU_SD_Data_Ready        SDC_Data_Ready

#define NU_UART_SUCCESS         0
#define NU_INVALID_PARITY       -1
#define NU_INVALID_DATA_BITS    -2
#define NU_INVALID_STOP_BITS    -3
#define NU_INVALID_BAUD         -4
#define NU_INVALID_COM_PORT     -5
#define NU_INVALID_DATA_MODE    -6
#define NU_UART_LIST_FULL       -7
#define NU_INVALID_MOUSE_MODE   -8

#define NU_BUFFER_FULL          1
#define NU_BUFFER_DATA          2
#define NU_BUFFER_EMPTY         3

/* Deifine IO macros. */

/* 8 bit access */
#define SD_OUTBYTE(reg, data)   ( (*( (UINT8 *) (reg) ) ) = (UINT8)(data) )

#define SD_INBYTE(reg)          (  *( (UINT8 *) (reg) ) )

/* 16 bit access */
#define SD_OUTWORD(reg, data)   ( (*( (UINT16 *) (reg) ) ) = (data) )

#define SD_INWORD(reg)          (  *( (UINT16 *) (reg) ) )

/* 32 bit access */
#define SD_OUTDWORD(reg, data)  ( (*( (UINT32 *) (reg) ) ) = (data) )

#define SD_INDWORD(reg)         (  *( (UINT32 *) (reg) ) )

/*Macro used for converting URT to SD_PORT. This is for PPP serial driver
  backwards compatability. */
#define URT_LAYER                       SD_PORT

#define URT_TX_BUFFER_SIZE              uart->sd_buffer_size
#define URT_Get_Char                    SDC_Get_Char
#define URT_Put_Char                    SDC_Put_Char
#define URT_Reset                       SDC_Reset
#define URT_Change_Communication_Mode   SDC_Change_Communication_Mode
#define URT_Carrier                     SDC_Carrier


#endif /* ifndef SD_DEFS_H */










