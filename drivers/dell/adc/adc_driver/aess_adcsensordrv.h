/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 On chip ADC sensor driver.
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

#if !defined (AESSADCSENSORDRV_H)
#define AESSADCSENSORDRV_H

#ifdef AESSADCSENSORDRV_C

/* For temporary use, because no type.h now */
/* ============================================ */
#define    hw_char         *(volatile unsigned char *)
#define    hw_short        *(volatile unsigned short *)

/* type define */
typedef    unsigned long   uint32;
typedef    unsigned short  uint16;
typedef    unsigned char   uint8;

typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;

#define TRUE                      1
#define FALSE                     0

#define STATUS_OK     0
/* ============================================= */

/* ioctl definitions */
#define AESS_ADCDRV_IOC_MAGIC  0xBB
#define AESS_ADCDRV_R          _IOR(AESS_ADCDRV_IOC_MAGIC, 0, int)

/* init flag */
#define AESSADCSENSOR_NOT_INIT    0
#define AESSADCSENSOR_INIT_OK     1

/* ADC base address */
#define ADC_REGS_BASE   WPCM450_VA_ADC

/* ADC registers */
#define WPCM450_ADCCON	   (ADC_REGS_BASE + 0x00)
#define WPCM450_ADCDATA	   (ADC_REGS_BASE + 0x04)

/* ADCCON Register Bits */
#define WPCM450_ADCCON_ADCMUX(x)	(((x) & 0x7)<<24)
#define WPCM450_ADCCON_MUXMASK		(0x7<<24)

#define WPCM450_ADCCON_ADC_INT_EN	(1<<21)
#define WPCM450_ADCCON_ADC_INT		(1<<18)
#define WPCM450_ADCCON_ADC_EN		(1<<17)
#define WPCM450_ADCCON_ADC_RST		(1<<16)
#define WPCM450_ADCCON_ADC_CONV		(1<<13)
#define WPCM450_ADCCON_ADC_DIV(x)	(((x) & 0xFF)<<24)

#define WPCM450_ADCCON_ADC_DATA_MASK(x) ((x) & 0x3FF)

/* ADC General Defintion */
#define WPCM450_ADC_INPUT_CLK_DIV	0
#define WPCM450_ADC_CONVERT_MAX_RETRY_CNT	10	/* base on 6Mhz */

#define WPCM450_ADC_MAX_CHNL_NUM	8

#define WPCM450_ADC_CHNL0_ADCI0	0
#define WPCM450_ADC_CHNL1_ADCI1	1
#define WPCM450_ADC_CHNL2_NA		2
#define WPCM450_ADC_CHNL3_VSB12	3
#define WPCM450_ADC_CHNL4_VDD33	4
#define WPCM450_ADC_CHNL5_NA		5
#define WPCM450_ADC_CHNL6_NA		6
#define WPCM450_ADC_CHNL7_ADCI2	7


/******************************************************************************
*   STRUCT      :   sADCSensorData
******************************************************************************/
/**
 *  @brief   Structure to ADCSensor driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
	/* Set ADC channel number */
	UINT8 u8ADCChannelNum;     

	/* Set ADC reading value */
	UINT32 u32ADCReading;     
			
} sADCSensorData;

/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
 
static int aess_adcsensor_read(sADCSensorData *pADCSensorStruct);

#endif   /* AESSADCSENSORDRV_C */

#endif   /* AESSADCSENSORDRV_H */
