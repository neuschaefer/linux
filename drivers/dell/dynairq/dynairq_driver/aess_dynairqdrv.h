/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 Dynamic IRQ driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#if !defined (AESSDYNAIRQDRV_H)
#define AESSDYNAIRQDRV_H

#include <linux/types.h>
#include <asm/types.h>
#include <asm/ioctl.h>
#include <linux/interrupt.h>

/* define the type */
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;


/* ioctl definitions */
#define AESS_IRQDRV_IOC_MAGIC		0xB9
#define AESS_IOCTL_DRIVER_INIT		_IOWR(AESS_IRQDRV_IOC_MAGIC, 0, int)
#define AESS_IOCTL_DYNAIRQ_INIT		_IOWR(AESS_IRQDRV_IOC_MAGIC, 1, int)
#define AESS_IOCTL_DYNAIRQ_CONFIG	_IOWR(AESS_IRQDRV_IOC_MAGIC, 2, int)
#define AESS_IOCTL_DYNAIRQ_CLEAR	_IOWR(AESS_IRQDRV_IOC_MAGIC, 3, int)
#define AESS_IOCTL_GENEISR_INIT		_IOWR(AESS_IRQDRV_IOC_MAGIC, 4, int)
#define AESS_IOCTL_USERMODE_ISRID	_IOWR(AESS_IRQDRV_IOC_MAGIC, 5, int)

//#define DIRQ_UNITTEST

#ifdef DIRQ_UNITTEST
#define AESS_IOCTL_GENEISR_TEST		_IOWR(AESS_IRQDRV_IOC_MAGIC, 10, int)
#endif

/* Dynamic IRQ Usable Number Range */
#define NUM_DIRQ_START	0
#define NUM_DIRQ_TOTAL	80
#define NUM_DIRQ_END	(NUM_DIRQ_START+NUM_DIRQ_TOTAL-1)
/* GPIO interrupt status register */
#define REG_GPIO_BASE	0xB0200000
#define REG_GPIO_ISR_P0	(REG_GPIO_BASE+0x080C)
#define REG_GPIO_ISR_P1	(REG_GPIO_BASE+0x081C)
#define REG_GPIO_ISR_P2	(REG_GPIO_BASE+0x082C)
#define REG_GPIO_ISR_P3	(REG_GPIO_BASE+0x083C)
#define REG_GPIO_ISR_P4	(REG_GPIO_BASE+0x084C)
#define TOTAL_PORTS_OF_GPIO	5
#define MAX_BITS_OF_1PORT	(1<<16)

/* IRQ status register */
#define WPCM450_IRQREG(x)   ((x) + WPCM450_VA_AIC)
#define WPCM450_AIC_SCR2	WPCM450_IRQREG(0x008)		/* Source control register 2 (GPIO0) */
#define WPCM450_AIC_SCR3	WPCM450_IRQREG(0x00c)		/* Source control register 3 (GPIO1) */
#define WPCM450_AIC_SCR4	WPCM450_IRQREG(0x010)		/* Source control register 4 (GPIO2) */
#define WPCM450_AIC_SCR5	WPCM450_IRQREG(0x014)		/* Source control register 5 (GPIO3) */
#define WPCM450_AIC_ISR		WPCM450_IRQREG(0x108)     	/* Interrupt status register */
#define WPCM450_AIC_IMR		WPCM450_IRQREG(0x114)		/* Interrupt mask register */

#define GPIO_PORT_BASE_ADDR      WPCM450_VA_GPIO
#define GPIO_REG_PORT0_DATAIN_ADDR          (GPIO_PORT_BASE_ADDR + 0x20)
#define GPIO_REG_PORT0_CFG0_ADDR          (GPIO_PORT_BASE_ADDR + 0x14)
#define GPIO_REG_PORT1_CFG0_ADDR          (GPIO_PORT_BASE_ADDR + 0x24)
#define GPIO_REG_PORT1_BLINK_CTR_ADDR          (GPIO_PORT_BASE_ADDR + 0x30)

#define WPCM450_GLOBAL_CTRL_REG    WPCM450_VA_GCR

#define GLOBAL_REG_PIN_SELECT1_ADDR  (WPCM450_GLOBAL_CTRL_REG + 0xc)
#define GLOBAL_REG_PIN_SELECT2_ADDR  (WPCM450_GLOBAL_CTRL_REG + 0x10)

#define NUM_IRQ_BY_PORT(port,bit)	((((port)*16)+(bit))+NUM_DIRQ_START)
#define NextGPIOIRQ(port,bit)	\
	do {				\
		bit++;			\
		if (bit>=16) {	\
			port ++;	\
			bit = 0;	\
		}				\
	} while(0)


enum INT_TRIGGER_INFO {
	INTT_LOW_LEVEL,	/* No use in vitesse */
	INTT_RISING_EDGE,
	INTT_FALLING_EDGE,
	INTT_HIGH_LEVEL,
	INTT_BOTH_EDGE,
	INTT_BOTH_LEVEL
};

/* Dynamic IRQ Paramters Structure */
typedef struct
{
	/* Parameter1*/
	union {
		ushort data;
		ushort u16IRQNum;     			// IRQ Number
	}param1;
	
	/* Parameter2 */
	union {
		ushort data;
		ushort u16Reserved;				// Reserved
	}param2;
	
	/* Parameter3 */
	UINT32 param3;
	/*
	union {
		uint data;
		struct {
			uint : 24;			  	// bit 31~8 Reserved
			uint intAutoEnable: 1;	// bit 7	Interrupt Auto Enable
			uint trigger: 2;		// bit 6:5	Trigger Info
			uint : 5;	   			// bit 4:0	Reserved
		} bits;
	}param3;
*/
	/* ISR name */
	char *pISRName;			    

} sIRQInfoType;

/* Dynamic IRQ List Structure */
#ifdef	__KERNEL__

struct sIrqNum
{
	struct sIrqNum *next;
	int irq_num;
	int irq_enable;
	int irq_type;
};


typedef struct
{
	struct sIrqNum *pstIRQNum;
	const char * devname;
	void * dev_id;
	unsigned long irqflags;
	irqreturn_t (*handler)(int, void *, struct pt_regs *);

} sIRQList;

#define MAX_DIRQ_LIST	50
int aess_register_dynamic_irq_list(const char * devname,
								   void * dev_id,
								   unsigned long irqflags,
								   irqreturn_t (*handler)(int, void *, struct pt_regs *)
								   );

int aess_dynamic_irq_int_clear(int irq);

#ifdef DIRQ_DEBUG
void aess_show_dynamic_irq_list();
int aess_call_dynamic_irq(int);
#endif

#endif /* __KERNEL__ */

#endif   /* AESSDYNAIRQDRV_H */
