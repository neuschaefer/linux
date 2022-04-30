/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
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
#ifndef SPEAr_DMA_DEV_CONFIG_H
#define SPEAr_DMA_DEV_CONFIG_H

//Define the type used to identify a DMA device into the HCL
typedef enum
{
	//Scheme 00
	MEMORY_DMA_DEVICE = 0,
#ifdef CONFIG_ARCH_SPEARBASIC
	UART_0_IN_DMA_DEVICE,
	UART_0_OUT_DMA_DEVICE,
	SPI_0_IN_DMA_DEVICE,
	SPI_0_OUT_DMA_DEVICE,
	I2C_IN_DMA_DEVICE,
	I2C_OUT_DMA_DEVICE,
	IRDA_DMA_DEVICE,
	JPEG_IN_DMA_DEVICE,
	JPEG_OUT_DMA_DEVICE,
#endif

#ifdef CONFIG_ARCH_SPEARPLUS
	SPI_1_IN_DMA_DEVICE,
	SPI_1_OUT_DMA_DEVICE,
	UART_0_IN_DMA_DEVICE,
	UART_0_OUT_DMA_DEVICE,
	UART_1_IN_DMA_DEVICE,
	UART_1_OUT_DMA_DEVICE,
	SPI_2_IN_DMA_DEVICE,
	SPI_2_OUT_DMA_DEVICE,
	SPI_0_IN_DMA_DEVICE,
	SPI_0_OUT_DMA_DEVICE,
	I2C_IN_DMA_DEVICE,
	I2C_OUT_DMA_DEVICE,
	IRDA_DMA_DEVICE,
	JPEG_IN_DMA_DEVICE,
	JPEG_OUT_DMA_DEVICE,
#endif

	//Scheme 01
	RAS_0_IN_DEVICE,
	RAS_0_OUT_DEVICE,
	RAS_1_IN_DEVICE,
	RAS_1_OUT_DEVICE,
	RAS_2_IN_DEVICE,
	RAS_2_OUT_DEVICE,
	RAS_3_IN_DEVICE,
	RAS_3_OUT_DEVICE,
	RAS_4_IN_DEVICE,
	RAS_4_OUT_DEVICE,
	RAS_5_IN_DEVICE,
	RAS_5_OUT_DEVICE,
	RAS_6_IN_DEVICE,
	RAS_6_OUT_DEVICE,
	RAS_7_IN_DEVICE,
	RAS_7_OUT_DEVICE,

	//Scheme 02
#ifdef CONFIG_ARCH_SPEARPLUS
	EXT_0_IN_OUT_DEVICE,
	EXT_0_OUT_DEVICE,
	EXT_1_IN_OUT_DEVICE,
	EXT_1_OUT_DEVICE,
	EXT_2_IN_OUT_DEVICE,
	EXT_2_OUT_DEVICE,
	EXT_3_IN_OUT_DEVICE,
	EXT_3_OUT_DEVICE,
	EXT_4_IN_DEVICE,
	EXT_4_OUT_DEVICE,
	EXT_5_IN_DEVICE,
	EXT_5_OUT_DEVICE,
	EXT_6_IN_DEVICE,
	EXT_6_OUT_DEVICE,
	EXT_7_IN_DEVICE,
	EXT_7_OUT_DEVICE,
#endif

	LAST_DMA_DEVICE
}t_DMADeviceId;


//DMA0 Request Lines mapping
typedef enum
{
	//Scheme 00
#ifdef CONFIG_ARCH_SPEARBASIC
	DMA_REQ_UART_IN_0 = 2,
	DMA_REQ_UART_OUT_0 = 3,
	DMA_REQ_SPI_IN_0 = 8,
	DMA_REQ_SPI_OUT_0 = 9,
	DMA_REQ_I2C_IN_1 = 10,
	DMA_REQ_I2C_OUT_1 = 11,
	DMA_REQ_IRDA = 12,
	DMA_REQ_JPEG_IN = 14,
	DMA_REQ_JPEG_OUT = 15,
#endif
#ifdef CONFIG_ARCH_SPEARPLUS
	DMA_REQ_SPI_IN_1 = 0,
	DMA_REQ_SPI_OUT_1 = 1,
	DMA_REQ_UART_IN_0 = 2,
	DMA_REQ_UART_OUT_0 = 3,
	DMA_REQ_UART_IN_1 = 4,
	DMA_REQ_UART_OUT_1 = 5,
	DMA_REQ_SPI_IN_2 = 6,
	DMA_REQ_SPI_OUT_2 = 7,
	DMA_REQ_SPI_IN_0 = 8,
	DMA_REQ_SPI_OUT_0 = 9,
	DMA_REQ_I2C_IN_1 = 10,
	DMA_REQ_I2C_OUT_1 = 11,
	DMA_REQ_IRDA = 12,
	DMA_REQ_JPEG_IN = 14,
	DMA_REQ_JPEG_OUT = 15,
#endif

	//Scheme 01
	DMA_REQ_RAS_0_IN = 0,
	DMA_REQ_RAS_0_OUT = 1,
	DMA_REQ_RAS_1_IN = 2,
	DMA_REQ_RAS_1_OUT = 3,
	DMA_REQ_RAS_2_IN = 4,
	DMA_REQ_RAS_2_OUT = 5,
	DMA_REQ_RAS_3_IN = 6,
	DMA_REQ_RAS_3_OUT = 7,
	DMA_REQ_RAS_4_IN = 8,
	DMA_REQ_RAS_4_OUT = 9,
	DMA_REQ_RAS_5_IN = 10,
	DMA_REQ_RAS_5_OUT = 11,
	DMA_REQ_RAS_6_IN = 12,
	DMA_REQ_RAS_6_OUT = 13,
	DMA_REQ_RAS_7_IN = 14,
	DMA_REQ_RAS_7_OUT = 15,

	//Scheme 02
#ifdef CONFIG_ARCH_SPEARPLUS
	DMA_REQ_EXT_0_IN_OUT = 0,
	DMA_REQ_EXT_0_OUT = 1,
	DMA_REQ_EXT_1_IN_OUT = 2,
	DMA_REQ_EXT_1_OUT = 3,
	DMA_REQ_EXT_2_IN_OUT = 4,
	DMA_REQ_EXT_2_OUT = 5,
	DMA_REQ_EXT_3_IN_OUT = 6,
	DMA_REQ_EXT_3_OUT = 7,
	DMA_REQ_EXT_4_IN = 8,
	DMA_REQ_EXT_4_OUT = 9,
	DMA_REQ_EXT_5_IN = 10,
	DMA_REQ_EXT_5_OUT = 11,
	DMA_REQ_EXT_6_IN = 12,
	DMA_REQ_EXT_6_OUT = 13,
	DMA_REQ_EXT_7_IN = 14,
	DMA_REQ_EXT_7_OUT = 15,
#endif

	REQUEST_NONE = 16
}t_DMARequestId;



/*
 * DMA Device Descriptor
 */


//Scheme 00 ****************************************************************************************************
#define MEMORY_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, REQUEST_NONE, DMA_SCH_NONE},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define UART_0_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_UART_IN_0, DMA_SCH_O},              \
	BYTE_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}


#define UART_0_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_UART_OUT_0, DMA_SCH_O},              \
	BYTE_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define SPI_0_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_1, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_SPI_IN_0, DMA_SCH_O},              \
	HALFWORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define SPI_0_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_1, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_SPI_OUT_0, DMA_SCH_O},              \
	HALFWORD_WIDTH ,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define I2C_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_I2C_IN_1, DMA_SCH_O},              \
	WORD_WIDTH, /*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define I2C_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_I2C_OUT_1, DMA_SCH_O},              \
	WORD_WIDTH, /*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define IRDA_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_IRDA, DMA_SCH_O},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define JPEG_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_JPEG_IN, DMA_SCH_O},              \
	WORD_WIDTH,/*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define JPEG_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_JPEG_OUT, DMA_SCH_O},              \
	WORD_WIDTH,/*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}


#ifdef CONFIG_ARCH_SPEARPLUS

#define SPI_1_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_1, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_SPI_IN_1, DMA_SCH_O},              \
	HALFWORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define SPI_1_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_1, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_SPI_OUT_1, DMA_SCH_O},              \
	HALFWORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define UART_1_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_UART_IN_1, DMA_SCH_O},              \
	BYTE_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define UART_1_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_UART_OUT_1, DMA_SCH_O},              \
	BYTE_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define SPI_2_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_1, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_SPI_IN_2, DMA_SCH_O},              \
	HALFWORD_WIDTH ,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define SPI_2_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_1, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_SPI_OUT_2, DMA_SCH_O},              \
	HALFWORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#endif


//Scheme 01 ****************************************************************************************************
#define RAS_0_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_0_IN, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_0_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_0_OUT, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_1_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_1_IN, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_1_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_1_OUT, DMA_SCH_1},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}


#define RAS_2_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_2_IN, DMA_SCH_1},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_2_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_2_OUT, DMA_SCH_1},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_3_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_3_IN, DMA_SCH_1},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_3_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_3_OUT, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_4_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_4_IN, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_4_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_4_OUT, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_5_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_5_IN, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_5_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_5_OUT, DMA_SCH_1},              \
	WORD_WIDTH, /*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_6_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_6_IN, DMA_SCH_1},              \
	WORD_WIDTH, /*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_6_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_6_OUT, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_7_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_7_IN, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define RAS_7_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB0_MASTER, DMA_REQ_RAS_7_OUT, DMA_SCH_1},              \
	WORD_WIDTH,/*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}


#ifdef CONFIG_ARCH_SPEARPLUS
//Scheme 02 ****************************************************************************************************
#define EXT_0_IN_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_0_IN_OUT, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_0_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_0_OUT, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_1_IN_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_1_IN_OUT, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_1_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_1_OUT, DMA_SCH_2},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}


#define EXT_2_IN_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_2_IN_OUT, DMA_SCH_2},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_2_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_2_OUT, DMA_SCH_2},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_3_IN_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_3_IN_OUT, DMA_SCH_2},              \
	WORD_WIDTH, /*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_3_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_3_OUT, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_4_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_4_IN, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_4_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_4_OUT, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_5_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_5_IN, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_5_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_5_OUT, DMA_SCH_2},              \
	WORD_WIDTH, /*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_6_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_6_IN, DMA_SCH_2},              \
	WORD_WIDTH, /*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_6_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_6_OUT, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	FALSE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_7_IN_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_7_IN, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}

#define EXT_7_OUT_DMA_DEVICE_DESCRIPTOR \
{                                                  \
	BURST_SIZE_8, /* maxBurstSize */                   \
	USER_NOT_BUFFERABLE_NOT_CACHEABLE, /* protection */\
	/* accessDesc */                               \
	{AHB1_MASTER, DMA_REQ_EXT_7_OUT, DMA_SCH_2},              \
	WORD_WIDTH,/*buffer*/    \
	TRUE  ,/* cantBeFlowController */                  \
	FALSE  /* Not used*/                 \
}
#endif
#endif //SPEAr_DMA_DEV_CONFIG_H
