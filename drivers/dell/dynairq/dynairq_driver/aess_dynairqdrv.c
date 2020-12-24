/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 Dynamic IRQ driver.
 *
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU
 * General Public License. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSDYNAIRQDRV_C
#include <linux/module.h> /* Needed by all modules */
#include <linux/init.h> /* Needed for module_init/exit */
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include "aess_dynairqdrv.h"
//#include "aess_gpiodrv.h"
//#include "aess_eventhandlerdrv.h"
#include "../../eventhandler/eventhandler_driver/aess_eventhandlerdrv.h"
#include "../../gpio/gpio_driver/aess_gpiodrv.h"
#include <asm/uaccess.h>
#include <asm/bitops.h>


//#define DIRQ_DEBUG
/* Debugging Message */
#ifdef DIRQ_DEBUG
#define DEBUG_MSG(fmt, args...) printk( "DynaIRQ: " fmt, ## args)
#else
#define DEBUG_MSG(fmt, args...)
#endif //DIRQ_DEBUG


//#define DIRQ_DEBUG2

/* driver name will passed by insmod command */
static char *driver_name="aess_dynairqdrv";
module_param(driver_name, charp, S_IRUGO);

/*
 ****************************************************************************
 * Global variables definitions											*
 ****************************************************************************
 */
struct gpio_dev_s
{
	unsigned int irq;
	unsigned char *name;
} gpio_dev[4] = {{ IRQ_GROUP0, "GPIO ISR 0\0" }, { IRQ_GROUP1, "GPIO ISR 1\0" }, { IRQ_GROUP2, "GPIO ISR 2\0" }, { IRQ_GROUP3, "GPIO ISR 3\0" }} ;

dev_t dirq_dev;
struct cdev *dynairq_cdev;

/*
 ****************************************************************************
 * Dynamic IRQ definitions													*
 ****************************************************************************
 */
static sIRQList stIRQListArray[MAX_DIRQ_LIST];
static int cIRQListArray=0;

/*
 ****************************************************************************
 * Generic ISR definitions													*
 ****************************************************************************
 */
/* Event handle ID */
#define NAME_GENERIC_ISR	"AESS_GENERIC_ISR"

/* Event handle ID */
typedef struct
{
	UINT16 u16NumIRQ;
	UINT32 u32EventID;
}sUserModeRecordID;


static sUserModeRecordID stUserModeISRID[32];
static UINT16 cRecUserModeID=0;
static UINT16 u16GISRDriverID=0xffff;
static UINT32 u32ISREnableFlag=0;
static UINT32 u32ISRDisableBeforInitFlag=0;


/* IRQNumSearchByIRQ()
 * Search IRQNum Item by IRQ num
 */
static struct sIrqNum * IRQNumSearchByIRQ(const int irq)
{
	int i;
	struct sIrqNum *pstIRQNum, **pp;

	DEBUG_MSG("IRQListSearchByIRQ...\n");    // JIC - compiler warning fixed
	for (i=0 ; i<cIRQListArray ; i++) {
		pp = &stIRQListArray[i].pstIRQNum;
		while ((pstIRQNum = *pp) != NULL) {
			pp = &pstIRQNum->next;
			if (irq == pstIRQNum->irq_num) {
				DEBUG_MSG("Find IRQ in %d lists.\n", i);
				return pstIRQNum;
			}
		}
	}

	return NULL;
}


/* IRQListSearch()
 * Search IRQList Item by Name
 */
static sIRQList * IRQListSearch(const char * devname)
{
	int i;
	sIRQList *pstIRQList;

	for (i=0 ; i<cIRQListArray ; i++) {
		pstIRQList = &stIRQListArray[i];
		if (0 == strcmp(pstIRQList->devname, devname)) {
			DEBUG_MSG("Find IRQ in %d\n", i);
			return pstIRQList;
		}
	}
	return NULL;
}


/* IRQListSearchByIRQ()
 * Search IRQList Item by IRQ num
 */
static sIRQList * IRQListSearchByIRQ(const int irq)
{
	int i;
	struct sIrqNum *pstIRQNum, **pp;

	DEBUG_MSG("IRQListSearchByIRQ...\n");
	for (i=0 ; i<cIRQListArray ; i++) {
		pp = &stIRQListArray[i].pstIRQNum;
		while ((pstIRQNum = *pp) != NULL) {
			pp = &pstIRQNum->next;
			if (irq == pstIRQNum->irq_num) {
				DEBUG_MSG("Find IRQ in %d lists.\n", i);
				return &stIRQListArray[i];
			}
		}
	}

	return NULL;
}


/* IRQListGetFree()
 */
static sIRQList * IRQListGetFree(void)
{
	return &stIRQListArray[cIRQListArray++];
}


/* IRQListUpdate()
 */
void IRQListUpdate(sIRQList *pstDst, sIRQList *pstSrc)
{
	memcpy(pstDst, pstSrc, sizeof(sIRQList));
}


/* aess_register_dynamic_irq_list()
 * return <0 : error
 * return =0 : success
 */
int aess_register_dynamic_irq_list(const char * devname,
								   void * dev_id,
								   unsigned long irqflags,
								   irqreturn_t (*handler)(int, void *, struct pt_regs *)
								   )
{
	sIRQList stIRQList1;
	sIRQList *pstIRQList;

	stIRQList1.pstIRQNum = NULL;
	stIRQList1.devname = devname;
	stIRQList1.dev_id = dev_id;
	stIRQList1.irqflags = irqflags;
	stIRQList1.handler = handler;

	pstIRQList = IRQListSearch(devname);

	if (NULL == pstIRQList) {
		if (cIRQListArray >= MAX_DIRQ_LIST)
			return -ENOSPC;
		else
			pstIRQList = IRQListGetFree();
	} else {
		DEBUG_MSG("Same IRQ Name found: Return.\n");
		return -EEXIST;
	}

	IRQListUpdate(pstIRQList, &stIRQList1);
	return 0;
}


/* aess_dynamic_irq_config()
 * return <0 : error
 * return =0 : success
 */
// JIC - added support for param3, which contains polarity information
static int aess_dynamic_irq_config(int irq, int type, int enable, UINT32 u32Param3)
{
	unsigned char u8IntType;
	unsigned char raw_irq;

	raw_irq = irq - NUM_DIRQ_START;

	if (enable) {
		switch (type) {
			case INTT_FALLING_EDGE:
				u8IntType = INPUT_INT_FAILING;
				break;
			case INTT_RISING_EDGE:
				u8IntType = INPUT_INT_RISING;
				break;
			case INTT_HIGH_LEVEL:
				u8IntType = INPUT_INT_HIGH;
				break;
			case INTT_LOW_LEVEL:
				u8IntType = INPUT_INT_LOW;
				break;
			case INTT_BOTH_EDGE:
				u8IntType = INPUT_INT_BOTH_EDGE;
				break;
			case INTT_BOTH_LEVEL:
				u8IntType = INPUT_INT_BOTH_LEVEL;
				break;
			default:
				return -1;
		}
	} else {
		u8IntType = INPUT_INT_OFF;
	}
	DEBUG_MSG("Set GPIO, PortPin=0x%02x, IntType=0x%02x\n", raw_irq, u8IntType);
	aess_gpio_commander(raw_irq, GPIO_CONFIG, u8IntType, &u32Param3);
	return 0;
}


/* aess_dynamic_irq_int_clear()
 * return <0 : error
 * return =0 : success
 */
int aess_dynamic_irq_int_clear(int irq)
{
	unsigned char u8TmpBuf;
	unsigned char raw_irq;

	raw_irq = irq - NUM_DIRQ_START;

	u8TmpBuf = SET_GPIO_OUTPUT_HIGH;
	aess_gpio_commander(raw_irq, GPIO_WRITE, CLR_INT_OUTPUT, (void *)&u8TmpBuf);
	return 0;
}


#ifdef DIRQ_DEBUG
void aess_show_dynamic_irq_list(void)       // JIC - compiler warning fixed
{
	sIRQList *pstIRQList;
	struct sIrqNum *pstIRQNum, **pp;
	int i;

	if (0 == cIRQListArray) {
		DEBUG_MSG("No any data in IRQ list.\n");
		return;
	}
	for (i=0;i<cIRQListArray;i++) {
		pstIRQList = &stIRQListArray[i];
		DEBUG_MSG("IRQ list %d\n", i);
		DEBUG_MSG("Devname  = %s\n", pstIRQList->devname);
		DEBUG_MSG("dev_id   = 0x%08lx\n", (UINT32) pstIRQList->dev_id);     // JIC - compiler warnings fixed
		DEBUG_MSG("irqflags = 0x%08lx\n", pstIRQList->irqflags);
		DEBUG_MSG("handler  = 0x%08lx\n", (UINT32) pstIRQList->handler);
		pp = &stIRQListArray[i].pstIRQNum;
		while ((pstIRQNum = *pp) != NULL) {
			pp = &pstIRQNum->next;
			DEBUG_MSG("irq num  = %d\n", pstIRQNum->irq_num);
		}
	}
}

int aess_call_dynamic_irq(int n)
{
	sIRQList *pstIRQList = &stIRQListArray[n];

	if (NULL == pstIRQList->handler) {
		return -1;
	}
	(pstIRQList->handler)(0,0,0);
	return 0;
}

#endif

/*
 ****************************************************************************
 * Generic ISR function for user mode ISR												*
 ****************************************************************************
 */
static irqreturn_t GenericISRFunc(int irq, void *dev_id, struct pt_regs *regs)
{
	int i;

	DEBUG_MSG("GenericISRFunc(), irq=%d\n",irq);

	/* disable irq */
	aess_dynamic_irq_config(irq,0,0,0);
	/* clear irq */
	aess_dynamic_irq_int_clear(irq);

	if (0xffff == u16GISRDriverID) {
		printk("GeneIRQ: Event handle ID no initial\n");
		return IRQ_HANDLED;
	}

	if (0 == cRecUserModeID) {
		printk("GeneIRQ: No record.\n");
		return IRQ_HANDLED;
	}

	for (i=0; i<cRecUserModeID ; i++) {
		if (irq == stUserModeISRID[i].u16NumIRQ) {
			aess_generate_driver_event(u16GISRDriverID, stUserModeISRID[i].u32EventID);
			return IRQ_HANDLED;
		}
	}
	printk("GeneIRQ: IRQ%d record not found, AUTO disable IRQ%d.\n", irq, irq);
	return IRQ_HANDLED;
}


/*
 ****************************************************************************
 * Interface of this driver 												*
 ****************************************************************************
 */
/* Open() */
static int aess_dirq_open(struct inode *inode, struct file *filp)
{
	static uint uopen = 0;

	if (0 == uopen)
	{
		DEBUG_MSG("start init aess_dirq_open\n");
		uopen = 1;
		DEBUG_MSG("finish init aess_dirq_open!\n");
	}
	return 0;
}

/* Close() */
int aess_dirq_exit(struct inode* inode, struct file *flip)
{
	DEBUG_MSG("aess_dirq_open exit!\n");
	return 0;
}

/* IOCTL()
 * return <0 : error
 * return =0 : success
 */
static int aess_dirq_ioctl(struct inode * inode, struct file *flip,
						   unsigned int cmd, unsigned long arg)
{
	sIRQInfoType *pstDIRQ = NULL;
	sIRQList *pstIRQList;
  UINT32 u32ISRDisableBeforInitFlagTemp;
	uint irq_num = 0xff;
	struct sIrqNum *pstIRQNum, **pp;
	int i,findUMISR;
	int int_type, int_enable;

	DEBUG_MSG("aess_dirq_ioctl\n");

#ifdef DIRQ_DEBUG2
	printk("\n aess_dirq_ioctl  cmd == 0x%x ", cmd);
#endif

	if ((cmd != AESS_IOCTL_DRIVER_INIT) && (0 == arg)) {
		return -EFAULT;
	}

	/* Check the DynaIRQ structure address is OK */
	if (cmd == AESS_IOCTL_DYNAIRQ_INIT ||
		cmd == AESS_IOCTL_DYNAIRQ_CONFIG ||
		cmd == AESS_IOCTL_DYNAIRQ_CLEAR) {
		pstDIRQ = (sIRQInfoType *) arg;
		irq_num = pstDIRQ->param1.u16IRQNum;
		if(!access_ok(VERIFY_READ, (void *) pstDIRQ, sizeof(sIRQInfoType)))
		{
			printk("DynaIRQ: struct access error!!\n");
			/* error occur */
			return -EFAULT;
		}
		/* Check input params */
		if (((cmd == AESS_IOCTL_DYNAIRQ_INIT) && (NULL == pstDIRQ->pISRName)) ||
			(irq_num < NUM_DIRQ_START ||
			 irq_num > NUM_DIRQ_END)) {
			return -EFAULT;
		}

	}

	switch (cmd) {
		case AESS_IOCTL_DRIVER_INIT:

#ifdef DIRQ_DEBUG2
			printk("\n AESS_IOCTL_DRIVER_INIT ... ");
#endif

			/* free register IRQ */
			findUMISR = 0;
			for (i=0 ; i<cIRQListArray ; i++) {
				pp = &stIRQListArray[i].pstIRQNum;
				if (strcmp(stIRQListArray[i].devname,NAME_GENERIC_ISR)==0) {
					findUMISR = 1;
				}
				while ((pstIRQNum = *pp) != NULL) {
					pp = &pstIRQNum->next;
					kfree(pstIRQNum);
				}
				stIRQListArray[i].pstIRQNum = NULL;
			}
			/* initial global var */
			cIRQListArray = findUMISR;
			cRecUserModeID = 0;
			u16GISRDriverID = 0xffff;

			/* debug */
			{
#if 0
				printk("\n WPCM450_AIC_SCR2 = 0x%x", ioread32((void *) WPCM450_AIC_SCR2));
				printk("\n WPCM450_AIC_SCR3 = 0x%x", ioread32((void *) WPCM450_AIC_SCR3));
				printk("\n WPCM450_AIC_SCR4 = 0x%x", ioread32((void *) WPCM450_AIC_SCR4));
				printk("\n WPCM450_AIC_SCR5 = 0x%x", ioread32((void *) WPCM450_AIC_SCR5));
				printk("\n WPCM450_AIC_IMR = 0x%x \n", ioread32((void *) WPCM450_AIC_IMR));
				printk("\n GPIO_REG_PORT0_DATAIN_ADDR = 0x%x \n", ioread32((void *) GPIO_REG_PORT0_DATAIN_ADDR));
				printk("\n GPIO_REG_PORT0_CFG0_ADDR = 0x%x \n", ioread32((void *) GPIO_REG_PORT0_CFG0_ADDR));
				printk("\n GLOBAL_REG_PIN_SELECT1_ADDR = 0x%x \n", ioread32((void *) GLOBAL_REG_PIN_SELECT1_ADDR));
				printk("\n GLOBAL_REG_PIN_SELECT2_ADDR = 0x%x \n", ioread32((void *) GLOBAL_REG_PIN_SELECT2_ADDR));
				printk("\n GPIO_REG_PORT1_BLINK_CTR_ADDR = 0x%x \n", ioread32((void *) GPIO_REG_PORT1_BLINK_CTR_ADDR));
				printk("\n GPIO_REG_PORT1_CFG0_ADDR = 0x%x \n", ioread32((void *) GPIO_REG_PORT1_CFG0_ADDR));
#endif
			}
			break;

		case AESS_IOCTL_DYNAIRQ_INIT:

#ifdef DIRQ_DEBUG2
			printk("\n AESS_IOCTL_DYNAIRQ_INIT ... ");
			printk("IRQ number = %d\n", irq_num);
#endif

			//DEBUG_MSG("Trigger Info = %d\n", pstDIRQ->param3.bits.trigger);
			//DEBUG_MSG("Interrupt Auto Enable = %d\n", pstDIRQ->param3.bits.intAutoEnable);
			DEBUG_MSG("ISR name = %s\n", pstDIRQ->pISRName);

			/* Search IRQList by input name */
			pstIRQList = IRQListSearch(pstDIRQ->pISRName);
			if (NULL == pstIRQList) {
				DEBUG_MSG("No such IRQ List\n");
				return -ENXIO;
			}
			/* call GPIO API to setup ext-IRQ */
			int_type = (pstDIRQ->param3 >> 5) & 0x7;
			int_enable = test_bit(8, &pstDIRQ->param3);

			/* record which irq be setting */
			u32ISREnableFlag |= (1 << irq_num);
			if (aess_dynamic_irq_config(pstDIRQ->param1.u16IRQNum,
										int_type,
										int_enable,
										pstDIRQ->param3) < 0)
				return -EFAULT;

			/* If irq exist then return BUSY */
			if (IRQListSearchByIRQ(irq_num)) {
				return -EBUSY;
			}

			/* Save IRQ into pstDIRQ */
			pp = &pstIRQList->pstIRQNum;
			while ((pstIRQNum = *pp) != NULL) {
				pp = &pstIRQNum->next;
			}
			/* alloc new IRQ item */
			pstIRQNum = kmalloc(sizeof(struct sIrqNum), GFP_KERNEL);
			if (!pstIRQNum)
				return -ENOMEM;
			pstIRQNum->irq_num = irq_num;
			pstIRQNum->irq_enable = int_enable;
			pstIRQNum->irq_type = int_type;
			pstIRQNum->next = NULL;
			*pp = pstIRQNum;
			break;

		case AESS_IOCTL_DYNAIRQ_CONFIG:

#ifdef DIRQ_DEBUG2
			printk("\n AESS_IOCTL_DYNAIRQ_CONFIG ... ");
#endif
			/* call GPIO API to setup ext-IRQ */
			int_type = (pstDIRQ->param3 >> 5) & 0x7;
			int_enable = test_bit(8, &pstDIRQ->param3);
			if (aess_dynamic_irq_config(pstDIRQ->param1.u16IRQNum,
										int_type,
										int_enable, pstDIRQ->param3) < 0){
				return -EFAULT;
			}
			break;

		case AESS_IOCTL_DYNAIRQ_CLEAR:

#ifdef DIRQ_DEBUG2
			printk("\n AESS_IOCTL_DYNAIRQ_CLEAR ... ");
#endif

			/* Clear GIPO ISR */
			if (aess_dynamic_irq_int_clear(pstDIRQ->param1.u16IRQNum) < 0)
				return -EFAULT;
			break;

		case AESS_IOCTL_GENEISR_INIT:

#ifdef DIRQ_DEBUG2
			printk("\n AESS_IOCTL_GENEISR_INIT ... u32ISRDisableBeforInitFlag = 0x%08lx\n", u32ISRDisableBeforInitFlag);   // JIC - compiler warning fixed
#endif

			/* Set DriverID for event handler */
			if (copy_from_user(&u16GISRDriverID, (UINT16 *)arg, sizeof(UINT16)))
                            return -EFAULT;
			DEBUG_MSG("aess_dirq_ioctl, u16GISRDriverID = %d\n", u16GISRDriverID);
			/* after get event handle ID, we should reenable these irq which be disabled before event handle init */
			i = 0;
      // JIC - Use local copy of u32ISRDisableBeforInitFlag to avoid init failure that
      // can occur when interrupts happen right after they're enabled.  New bits were getting
      // set in u32ISRDisableBeforInitFlag after it had already been shifted!
      // The code kept shifting and ended up trying to access invalid IRQ numbers.
      u32ISRDisableBeforInitFlagTemp = u32ISRDisableBeforInitFlag;
      u32ISRDisableBeforInitFlag = 0;
			while (u32ISRDisableBeforInitFlagTemp)
			{
				if (u32ISRDisableBeforInitFlagTemp & 0x1)
				{
					pstIRQNum = IRQNumSearchByIRQ(i);
					if (NULL == pstIRQNum) {
						DEBUG_MSG("No such IRQNum List (%d)\n", i);
						return -ENXIO;
					}

#ifdef DIRQ_DEBUG2
					printk("*** IRQ=%x\n", i);
                    printk("*** pstIRQNum=%d\n", pstIRQNum->irq_num);
                    printk("*** pstIRQNum->irq_type=%d\n", pstIRQNum->irq_type);
                    printk("*** pstIRQNum->irq_enable=%d\n", pstIRQNum->irq_enable);
#endif

					if (aess_dynamic_irq_config(i,
												pstIRQNum->irq_type,
												pstIRQNum->irq_enable, 0) < 0)
						return -EFAULT;
				}
				u32ISRDisableBeforInitFlagTemp = u32ISRDisableBeforInitFlagTemp >> 1;
				i++;
			}
			break;

		case AESS_IOCTL_USERMODE_ISRID:

#ifdef DIRQ_DEBUG2
			printk("\n AESS_IOCTL_USERMODE_ISRID ... ");
#endif

			/* Set User Mode ISR ID for event handler */
			if (cRecUserModeID >= 32)
				return -EFAULT;
			if (copy_from_user(&stUserModeISRID[cRecUserModeID], (sUserModeRecordID *)arg, sizeof(stUserModeISRID)))
                            return -EFAULT;
			DEBUG_MSG("aess_dirq_ioctl, irq=%d, eid=%lx, c=%d\n",
					  stUserModeISRID[cRecUserModeID].u16NumIRQ,
					  stUserModeISRID[cRecUserModeID].u32EventID,
					  cRecUserModeID);
			cRecUserModeID++;
			break;

#ifdef DIRQ_UNITTEST
		case AESS_IOCTL_GENEISR_TEST:
			GenericISRFunc((int)arg,NULL,NULL);
			break;
#endif
		default:
			DEBUG_MSG("aess_dirq_ioctl, command error\n");
			return -EINVAL;
	}

	return 0;
}


/*
 ****************************************************************************
 * GPIO ISR Functions
 ****************************************************************************
 */
static inline int IRQFuncExec(int irq)
{
	sIRQList *pstIRQList;

	pstIRQList = IRQListSearchByIRQ(irq);
	if (NULL == pstIRQList || NULL == pstIRQList->handler) {
		return -1;
	}

	/* call handler function */
	(pstIRQList->handler)(irq, pstIRQList->dev_id, NULL);

	return 0;
}


/* gpio_irq_get()
 * input irq number for start searching
 * return -1 : error, no isr found
 * return n : isr number
 */
static inline int gpio_irq_get(unsigned int *pnPort,int *pnPBit)
{
	UINT8  u8TmpBuf;

	while ((*pnPBit) <= (*pnPort)) {
		/* only check these irqs which be set by dynamic irq driver */
		if ((u32ISREnableFlag & (1 << (*pnPBit))) == 0)
		{
			(*pnPBit) ++ ;
			continue;
		}
#ifdef DIRQ_DEBUG2
		printk("\n check pin %d \n", *pnPBit);
#endif
		DEBUG_MSG("GPIO = %d\n",*pnPBit);

		aess_gpio_commander(*pnPBit, GPIO_READ, INT_ENABLE_INPUT, (void *)&u8TmpBuf);
		if (u8TmpBuf == SET_GPIO_OUTPUT_HIGH) {
			aess_gpio_commander(*pnPBit, GPIO_READ, INT_STATUS_INPUT, (void *)&u8TmpBuf);
			DEBUG_MSG("u8TmpBuf = %d\n",u8TmpBuf);
			if (u8TmpBuf == SET_GPIO_OUTPUT_HIGH) {
#ifdef DIRQ_DEBUG2
			    printk("\n Found IRQ %d \n", *pnPBit);
#endif
				DEBUG_MSG("Found IRQ\n");
				return *pnPBit;
			}
		}
		(*pnPBit) ++ ;
	}
	return -1;

}


/* GPIO_interrupt()
 * GPIO interrupt handler for Share IRQ
 */
static irqreturn_t GPIO_interrupt(int irq_share, void *dev_id, struct pt_regs *regs, int start_p, int end_p)
{
	unsigned int nPort=0;
	int nPBit=0;
	int gpio_int=0;
	int irq;

	nPBit = start_p;
	nPort = (unsigned int)end_p; // total ping number
	//TODO://if this routine spent too much time, we might remove the 'while' to release resoure
	while (1) {
		/* Check ISR */
		irq = gpio_irq_get(&nPort,&nPBit);
		if (irq < 0) {
			if (gpio_int)
				return IRQ_HANDLED;
#ifdef DIRQ_DEBUG2
			printk("GPIO interrupt occurred, but all ISRs are 0!\n");
#endif
			return IRQ_NONE;
		}
		/* found ISR */
		gpio_int = 1;

		/* disable irq,  add this line
			 the reason is some time it can't disable irq on time when interrupt continue coming
			 so, disable irq here and wish can disable irq as soon as possible
		*/
		aess_dynamic_irq_config(irq,0,0,0);
		/* Clear GPIO ISR */
		aess_dynamic_irq_int_clear(irq);

		/* recode which irq be disabled */
		u32ISRDisableBeforInitFlag |= (1 << irq);

		/* DO ISR */

		if (IRQFuncExec(irq) < 0)
		{
#ifdef DIRQ_DEBUG2
			printk("IRQ%d hasn't handler function!\n", irq);
#endif
		}
		/* check NEXT ISR */
		//NextGPIOIRQ(nPort,nPBit);
		nPBit++;
	}
}


/* GPIO_interrupt0()
 * GPIO interrupt handler for Share IRQ for GPIO 0~3
 */
static irqreturn_t GPIO_interrupt0(int irq_share, void *dev_id, struct pt_regs *regs)
{
#ifdef DIRQ_DEBUG2
	printk("\n GPIO_interrupt0  ..");
#endif
	return(GPIO_interrupt(irq_share, dev_id, regs, 0, 3));
}


/* GPIO_interrupt1()
 * GPIO interrupt handler for Share IRQ for GPIO 11~4
 */
static irqreturn_t GPIO_interrupt1(int irq_share, void *dev_id, struct pt_regs *regs)
{
#ifdef DIRQ_DEBUG2
	printk("\n GPIO_interrupt1  ..");
#endif
	return(GPIO_interrupt(irq_share, dev_id, regs, 4, 11));
}


/* GPIO_interrupt2()
 * GPIO interrupt handler for Share IRQ for GPIO 15~12
 */
static irqreturn_t GPIO_interrupt2(int irq_share, void *dev_id, struct pt_regs *regs)
{
#ifdef DIRQ_DEBUG2
	printk("\n GPIO_interrupt2  ..");
#endif
	return(GPIO_interrupt(irq_share, dev_id, regs, 12, 15));
}


/* GPIO_interrupt3()
 * GPIO interrupt handler for Share IRQ for GPIO 25~24 (bit 17:16)
 */
static irqreturn_t GPIO_interrupt3(int irq_share, void *dev_id, struct pt_regs *regs)
{
#ifdef DIRQ_DEBUG2
	printk("\n GPIO_interrupt3  ..");
#endif
	return(GPIO_interrupt(irq_share, dev_id, regs, 24, 25));
}


/*
 ****************************************************************************
 * Initial driver															*
 ****************************************************************************
 */

static struct file_operations aess_dirq_fops = {
	.open = aess_dirq_open,
	.ioctl = aess_dirq_ioctl,
	.release = aess_dirq_exit,
};


static int __init aess_dynairq_init(void)
{
	int stat;

	DEBUG_MSG("aess_dynairq_init\n");

	stat = alloc_chrdev_region(&dirq_dev, 0, 1, driver_name);

	printk("mknod /dev/aess_dynairqdrv c %d 0\n", MAJOR(dirq_dev));

	if (stat < 0) {
		printk (KERN_ERR "DynaIRQ:Registering the Dynamic IRQ character device failed with %d\n", stat);
		return stat;
	}

	dynairq_cdev = cdev_alloc();
	cdev_init(dynairq_cdev, &aess_dirq_fops);
	cdev_add(dynairq_cdev, dirq_dev, 1);

	if (request_irq(gpio_dev[0].irq, (irq_handler_t)GPIO_interrupt0, IRQF_SHARED|IRQF_SAMPLE_RANDOM, gpio_dev[0].name, &gpio_dev[0])) {
		return -EBUSY;
	}

	if (request_irq(gpio_dev[1].irq, (irq_handler_t)GPIO_interrupt1, IRQF_SHARED|IRQF_SAMPLE_RANDOM, gpio_dev[1].name, &gpio_dev[1])) {
		return -EBUSY;
	}

	if (request_irq(gpio_dev[2].irq, (irq_handler_t)GPIO_interrupt2, IRQF_SHARED|IRQF_SAMPLE_RANDOM, gpio_dev[2].name, &gpio_dev[2])) {
		return -EBUSY;
	}

	if (request_irq(gpio_dev[3].irq, (irq_handler_t)GPIO_interrupt3, IRQF_SHARED|IRQF_SAMPLE_RANDOM, gpio_dev[3].name, &gpio_dev[3])) {
		return -EBUSY;
	}

	stat = aess_register_dynamic_irq_list(NAME_GENERIC_ISR, NULL, 0 , (void *)GenericISRFunc);
	if (stat < 0) {
		printk (KERN_ERR "DynaIRQ:Registering the Dynamic IRQ list failed with %d\n", stat);
	}

	return 0;
}


static void __exit aess_dynairq_exit(void)
{
	DEBUG_MSG("aess_dynairq_exit\n");
	free_irq(gpio_dev[0].irq, &gpio_dev[0]);
	free_irq(gpio_dev[1].irq, &gpio_dev[1]);
	free_irq(gpio_dev[2].irq, &gpio_dev[2]);
	free_irq(gpio_dev[3].irq, &gpio_dev[3]);
	cdev_del(dynairq_cdev);
	unregister_chrdev_region(dirq_dev, 1);
}

MODULE_DESCRIPTION("AESS Dynamic IRQ Driver");
MODULE_AUTHOR("Ivan.Huang@avocent.com");
MODULE_LICENSE("GPL");

module_init(aess_dynairq_init);
module_exit(aess_dynairq_exit);

EXPORT_SYMBOL(aess_register_dynamic_irq_list);
EXPORT_SYMBOL(aess_dynamic_irq_int_clear);
#ifdef DIRQ_DEBUG
EXPORT_SYMBOL(aess_show_dynamic_irq_list);
EXPORT_SYMBOL(aess_call_dynamic_irq);
#endif
