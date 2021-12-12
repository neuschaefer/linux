/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 On chip PWM driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#define AESSPWMDRV_C

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/arch/map.h>

#include "aess_pwmdrv.h"
#include "../../gpio/gpio_driver/aess_gpiodrv.h"   /* for disable PWM used*/

#define PWM_DEBUG
#undef  PWM_DEBUG

// Uncomment one of the following to determine whether PWMs go to maximum
// speed or are turned off upon driver "release."
// Comment out both for PWMs to maintain previous programmed speed.
#define PWM_MAX_SPEED_ON_RELEASE      // Takes precedence
//#define PWM_OFF_ON_RELEASE

/* Debugging Message */
#ifdef PWM_DEBUG
#define DEBUG_MSG(fmt, args...) printk( "PWM: " fmt, ## args)
#else
#define DEBUG_MSG(fmt, args...)
#endif

/* driver name will passed by insmod command */
static char *driver_name = "aess_pwmdrv"; 

/* The init flag for aess_pwm_open */
static UINT8 S_u8init_flag = AESSPWM_NOT_INIT;

/* Linux device data structure */
dev_t pwm_dev;
struct cdev *pwm_cdev;

UINT8 u8InitialPWM[PWM_MAX_CHN_NUM];

static int aess_pwm_config_init(sPWMDevConfig *PWMDevConfig)
{
	UINT8  u8PWMChannel = (PWMDevConfig->u8PWMChannelNum % PWM_MAX_CHN_NUM_IN_A_MODULE);
	UINT32 u32TmpBuf = 0, u32TmpBuf2 = 0, u32TmpBuf3 = 0;
	UINT32 u32DestAddr = 0, u32CtrlAddr = 0, u32PrescaleAddr = 0, u32CSelectorAddr = 0;  
	UINT32 offset = 0;
	
    //Set initial PWM value
    // DF286034 	IDRAC6 racadm reset causes a pnp removal of the perc6i card. Event ID 
    u8InitialPWM[PWMDevConfig->u8PWMChannelNum] =  (PWMDevConfig->u8PWMDutyCycle & 0xFF);

  /**********************************************************************
	* Config PWMD register for setting frequency divider during initialize
	**********************************************************************/
	/* PWMM0_BA = B800_7000h --> offset = 0x0h
	   PWMM1_BA = B800_7100h --> offset = 0x100h*/
	if (PWMDevConfig->u8PWMChannelNum >= 4)
	{
		if (ioread32(GLOBAL_REG_PDID_REG) < Z2_VERSION_NUM)
		{
			DEBUG_MSG("This version 0x%x only support 4 channels PWM\n", ioread32(GLOBAL_REG_PDID_REG));
			return -ENODEV;
		}
		offset = 0x100;
	}
		
	/* get PWM register address */	
	u32PrescaleAddr = (UINT32)PWM_REG_PRESCALE_ADDR + offset;
	u32CSelectorAddr = (UINT32)PWM_REG_CLOCK_SELECTOR_ADDR + offset;
	u32CtrlAddr = (UINT32)PWM_REG_CONTROL_ADDR + offset;
	
	/* read PWMD value */
	u32TmpBuf = ioread32((void *) u32PrescaleAddr);
	u32TmpBuf2 = ioread32((void *) u32CSelectorAddr);
	u32TmpBuf3 = ioread32((void *) u32CtrlAddr);
	
	switch (u8PWMChannel)
	{
		case 0:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH01_BIT);
			u32TmpBuf |= ((PWM_PRESCALE_DEFALUT_NUM & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH01_BIT);
			
			/* set clock selector bit [2:0] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH0_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH0_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH0_MODE_BIT);
			break;	
		case 1:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH01_BIT);
			u32TmpBuf |= ((PWM_PRESCALE_DEFALUT_NUM & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH01_BIT);
			
			/* set clock selector bit [5:4] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH1_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH1_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH1_MODE_BIT);
			break;	
		case 2:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH23_BIT);
			u32TmpBuf |= ((PWM_PRESCALE_DEFALUT_NUM & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH23_BIT);
			
			/* set clock selector bit [5:4] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH2_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH2_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH2_MODE_BIT);
			break;	
		case 3:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH23_BIT);
			u32TmpBuf |= ((PWM_PRESCALE_DEFALUT_NUM & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH23_BIT);
			
			/* set clock selector bit [5:4] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH3_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH3_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH3_MODE_BIT);
			break;	
		default:
			return -ENODEV;
			break;
	}
	
	DEBUG_MSG("u8PWMChannel=%d, u32CSelectorAddr=0x%x, u8PWMBaseCycleFrequency=%d, PWMDTmpBuf=0x%x \n",
		PWMDevConfig->u8PWMChannelNum, u32CSelectorAddr, PWMDevConfig->u8PWMBaseCycleFrequency, u32TmpBuf2);
	
	/* write new PWM register value  */
	iowrite32(u32TmpBuf, (void *) u32PrescaleAddr);  
	iowrite32(u32TmpBuf2, (void *) u32CSelectorAddr);  
	iowrite32(u32TmpBuf3, (void *) u32CtrlAddr);  

  /****************************************************************
	* Config PWM Counter register for setting resolution
	****************************************************************/
	u32DestAddr = (UINT32)PWM_REG_COUNTER_ADDR(u8PWMChannel) + offset;

	u32TmpBuf = (UINT32)(PWMDevConfig->u8PWMFrequencyDivider & 0xFF);

	DEBUG_MSG("u8PWMChannel=%d, u32CSelectorAddr=0x%x, u8PWMFrequencyDivider=%d, PWMDTmpBuf=%x \n",
		PWMDevConfig->u8PWMChannelNum, u32DestAddr, PWMDevConfig->u8PWMFrequencyDivider, (unsigned int)u32TmpBuf);

	/* write new PWMC value  */
	iowrite32(u32TmpBuf, (void *) u32DestAddr);  
	
  /****************************************************************
	* Config PWM Comparator register for setting duty cycle
	****************************************************************/
	u32DestAddr = (UINT32)PWM_REG_COMPARATOR_ADDR(u8PWMChannel) + offset;

	u32TmpBuf = (UINT32)(PWMDevConfig->u8PWMDutyCycle & 0xFF);

	DEBUG_MSG("u8PWMChannel=%d, u32CSelectorAddr=0x%x, u8PWMDutyCycle=%d, PWMDTmpBuf=%x \n",
		PWMDevConfig->u8PWMChannelNum, u32DestAddr, PWMDevConfig->u8PWMDutyCycle, (unsigned int)u32TmpBuf);

	/* write new PWMC value  */
	iowrite32(u32TmpBuf, (void *) u32DestAddr);  
	
  /****************************************************************
	* Enable PWM
	****************************************************************/
	u32TmpBuf3 = ioread32((void *) u32CtrlAddr);

	switch (u8PWMChannel)
	{
		case 0:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH0_ENABLE_BIT);
			break;	
		case 1:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH1_ENABLE_BIT);
			break;	
		case 2:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH2_ENABLE_BIT);
			break;	
		case 3:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH3_ENABLE_BIT);
			break;	
		default:
			return -ENODEV;
			break;
	}
	
	/* write new PWM value  */
	iowrite32(u32TmpBuf3, (void *) u32CtrlAddr);  
	
  /****************************************************************
	* Enable PWM PIN
	****************************************************************/
//dijic --dwa debug  --do not enable pin at this time --assume
// initial PWM is 0 (100%)

//	u32DestAddr = (UINT32)GLOBAL_REG_PIN_SELECT2_ADDR;
//	u32TmpBuf = ioread32((void *) u32DestAddr);
//	u32TmpBuf |= 1 << (PWM_PIN_SELECT_CH0_BIT + PWMDevConfig->u8PWMChannelNum);
//	iowrite32(u32TmpBuf, (void *) u32DestAddr);

//	DEBUG_MSG("u8PWMChannel=%d, u32CSelectorAddr=0x%x, PWMDTmpBuf=%x \n",
//		PWMDevConfig->u8PWMChannelNum, u32DestAddr, (unsigned int)u32TmpBuf);

//dijic --dwa debug  --get rid of pull up on PWM outputs
	u32DestAddr = (UINT32)WPCM450_GLOBAL_CTRL_REG + 0x1c;
  u32TmpBuf = ioread32((void *) u32DestAddr);
	DEBUG_MSG("u32CSelectorAddr=0x%x, (PE1) u32TmpBuf=%x \n",
		u32DestAddr, (unsigned int)u32TmpBuf);
 	u32TmpBuf &= 0xffffffec;
  iowrite32(u32TmpBuf, (void *) u32DestAddr);
	u32DestAddr = (UINT32)WPCM450_GLOBAL_CTRL_REG + 0x2c;
  u32TmpBuf = ioread32((void *) u32DestAddr);
	DEBUG_MSG("u32CSelectorAddr=0x%x, (PE15) u32TmpBuf=%x \n",
		u32DestAddr, (unsigned int)u32TmpBuf);
 	u32TmpBuf &= 0xfffffff0;
  iowrite32(u32TmpBuf, (void *) u32DestAddr);
//dwa debug

	return (STATUS_OK);
}

static int aess_pwm_config_set(sPWMDevConfig *PWMDevConfig)
{
	UINT8  u8PWMChannel = (PWMDevConfig->u8PWMChannelNum % PWM_MAX_CHN_NUM_IN_A_MODULE);
	UINT32 u32TmpBuf = 0;
	UINT32 u32DestAddr = 0;
	UINT32 offset = 0;
	
	/* PWMM0_BA = B800_7000h --> offset = 0x0h
	   PWMM1_BA = B800_7100h --> offset = 0x100h*/
	if (PWMDevConfig->u8PWMChannelNum >= 4)
	{
		if (ioread32(GLOBAL_REG_PDID_REG) < Z2_VERSION_NUM)
		{
			DEBUG_MSG("This version 0x%x only support 4 channels PWM\n", ioread32(GLOBAL_REG_PDID_REG));
			return -ENODEV;
		}
		offset = 0x100;
	}
		
  /****************************************************************
	* Config PWM Comparator register for setting duty cycle
	****************************************************************/
	u32DestAddr = (UINT32)PWM_REG_COMPARATOR_ADDR(u8PWMChannel) + offset;

	u32TmpBuf = (UINT32)(PWMDevConfig->u8PWMDutyCycle & 0xFF);

	DEBUG_MSG("u8PWMChannel=%d, u32CSelectorAddr=0x%x, u8PWMDutyCycle=%d, PWMDTmpBuf=%x \n",
		PWMDevConfig->u8PWMChannelNum, u32DestAddr, PWMDevConfig->u8PWMDutyCycle, (unsigned int)u32TmpBuf);

	/* write new PWMC value  */
	iowrite32(u32TmpBuf, (void *) u32DestAddr);

// Added by DIJIC to diable output drive when 100% PWM set

	u32DestAddr = (UINT32)GLOBAL_REG_PIN_SELECT2_ADDR;
  u32TmpBuf = ioread32((void *) u32DestAddr);
  if (0 == (PWMDevConfig->u8PWMDutyCycle & 0xFF))
  {
    /****************************************************************
  	* Disable PWM
	  ****************************************************************/
  	u32TmpBuf &= ~(1 << (PWM_PIN_SELECT_CH0_BIT + PWMDevConfig->u8PWMChannelNum));

  }
  else
  {
    /****************************************************************
	  * Enable PWM
  	****************************************************************/
  	u32TmpBuf |= 1 << (PWM_PIN_SELECT_CH0_BIT + PWMDevConfig->u8PWMChannelNum);

  };
	DEBUG_MSG("u8PWMChannel=%d, u32CSelectorAddr=0x%x, (Output Enable) u32TmpBuf=%x \n",
		PWMDevConfig->u8PWMChannelNum, u32DestAddr, (unsigned int)u32TmpBuf);
  iowrite32(u32TmpBuf, (void *) u32DestAddr);
  /****************************************************************
	* Enable PWM PIN
	****************************************************************/



	return (STATUS_OK);
}


static int aess_pwm_config_info(sPWMDevConfig *PWMDevConfig)
{
	UINT8  u8PWMChannel = (PWMDevConfig->u8PWMChannelNum % PWM_MAX_CHN_NUM_IN_A_MODULE);
	UINT32 offset = 0;
	UINT32 u32TmpBuf = 0, u32TmpBuf2 = 0, u32TmpBuf3 = 0;
	UINT32 u32DestAddr = 0, u32CtrlAddr = 0, u32PrescaleAddr = 0, u32CSelectorAddr = 0;
	UINT32 shitNum = 0;
	
	/* PWMM0_BA = B800_7000h --> offset = 0x0h
	   PWMM1_BA = B800_7100h --> offset = 0x100h*/
	if (PWMDevConfig->u8PWMChannelNum >= 4)
	{
		if (ioread32(GLOBAL_REG_PDID_REG) < Z2_VERSION_NUM)
		{
			DEBUG_MSG("This version 0x%x only support 4 channels PWM\n", ioread32(GLOBAL_REG_PDID_REG));
			return -ENODEV;
		}
		offset = 0x100;
	}
		
  /****************************************************************
	* Debug using
	****************************************************************/
	/* get PWM register address */	
	u32PrescaleAddr = (UINT32)PWM_REG_PRESCALE_ADDR + offset;
	u32CSelectorAddr = (UINT32)PWM_REG_CLOCK_SELECTOR_ADDR + offset;
	u32CtrlAddr = (UINT32)PWM_REG_CONTROL_ADDR + offset;
	
	/* read PWMD value */
	u32TmpBuf = ioread32((void *) u32PrescaleAddr);
	u32TmpBuf2 = ioread32((void *) u32CSelectorAddr);
	u32TmpBuf3 = ioread32((void *) u32CtrlAddr);

	printk("*** u8PWMChannel=%d, u32PrescaleAddr[0x%x]=0x%x \n",
		PWMDevConfig->u8PWMChannelNum, (unsigned int)u32PrescaleAddr, (unsigned int)u32TmpBuf);
		
	if ((PWMDevConfig->u8PWMChannelNum % 4) < 2)
		printk("=== PrescaleAddr=0x%x \n\n", ((unsigned int)u32TmpBuf & 0xFF));
	else
		printk("=== PrescaleAddr=0x%x \n\n",(((unsigned int)u32TmpBuf >> 8) & 0xFF));

	printk("*** u32CSelectorAddr[0x%x]=0x%x \n", (unsigned int)u32CSelectorAddr, (unsigned int)u32TmpBuf2);

	shitNum = ((PWMDevConfig->u8PWMChannelNum % 4) * 4) & 0x7;
	printk("=== CSR =0x%x \n\n", ((unsigned int)u32TmpBuf2 >> shitNum));

	printk("*** u32CtrlAddr[0x%x]=0x%x \n", (unsigned int)u32CtrlAddr, (unsigned int)u32TmpBuf3);

	u32DestAddr = (UINT32)PWM_REG_COUNTER_ADDR(u8PWMChannel) + offset;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	printk("*** COUNTER_ADDR[0x%x]=0x%x \n", (unsigned int)u32DestAddr, (unsigned int)u32TmpBuf);

	u32DestAddr = (UINT32)PWM_REG_COMPARATOR_ADDR(u8PWMChannel) + offset;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	printk("*** Duty Cycle - CMR[0x%x]=0x%x \n", (unsigned int)u32DestAddr, (unsigned int)u32TmpBuf);

	u32DestAddr = (UINT32)GLOBAL_REG_PIN_SELECT2_ADDR;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	printk("*** SELECT2_ADDR[0x%x]=0x%x \n", (unsigned int)u32DestAddr, (unsigned int)u32TmpBuf);

	return (STATUS_OK);
}

static int aess_pwm_config_debug(sPWMDevConfig *PWMDevConfig)
{
	UINT8  u8PWMChannel = (PWMDevConfig->u8PWMChannelNum % PWM_MAX_CHN_NUM_IN_A_MODULE);
	UINT32 u32TmpBuf = 0, u32TmpBuf2 = 0, u32TmpBuf3 = 0;
	UINT32 u32DestAddr = 0, u32CtrlAddr = 0, u32PrescaleAddr = 0, u32CSelectorAddr = 0;  
	UINT32 offset = 0;
	
  /**********************************************************************
	* Config PWMD register for setting frequency divider during initialize
	**********************************************************************/
	/* PWMM0_BA = B800_7000h --> offset = 0x0h
	   PWMM1_BA = B800_7100h --> offset = 0x100h*/
	if (PWMDevConfig->u8PWMChannelNum >= 4)
	{
		if (ioread32(GLOBAL_REG_PDID_REG) < Z2_VERSION_NUM)
		{
			DEBUG_MSG("This version 0x%x only support 4 channels PWM\n", ioread32(GLOBAL_REG_PDID_REG));
			return -ENODEV;
		}
		offset = 0x100;
	}
		
	/* get PWM register address */	
	u32PrescaleAddr = (UINT32)PWM_REG_PRESCALE_ADDR + offset;
	u32CSelectorAddr = (UINT32)PWM_REG_CLOCK_SELECTOR_ADDR + offset;
	u32CtrlAddr = (UINT32)PWM_REG_CONTROL_ADDR + offset;
	
	/* read PWMD value */
	u32TmpBuf = ioread32((void *) u32PrescaleAddr);
	u32TmpBuf2 = ioread32((void *) u32CSelectorAddr);
	u32TmpBuf3 = ioread32((void *) u32CtrlAddr);
	
	switch (u8PWMChannel)
	{
		case 0:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH01_BIT);
			u32TmpBuf |= ((PWMDevConfig->u8PWMDutyCycle & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH01_BIT);
			
			/* set clock selector bit [2:0] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH0_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH0_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH0_MODE_BIT);
			break;	
		case 1:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH01_BIT);
			u32TmpBuf |= 
				((PWMDevConfig->u8PWMDutyCycle & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH01_BIT);
			
			/* set clock selector bit [5:4] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH1_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH1_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH1_MODE_BIT);
			break;	
		case 2:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH23_BIT);
			u32TmpBuf |= ((PWMDevConfig->u8PWMDutyCycle & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH23_BIT);
			
			/* set clock selector bit [5:4] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH2_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH2_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH2_MODE_BIT);
			break;	
		case 3:
			/* set prescale bit[7:0]
					so far, default value is 127 */
			u32TmpBuf &= ~(PWM_PRESCALE_MASK << PWM_PRESCALE_CH23_BIT);
			u32TmpBuf |= ((PWMDevConfig->u8PWMDutyCycle & PWM_PRESCALE_MASK) << PWM_PRESCALE_CH23_BIT);
			
			/* set clock selector bit [5:4] */
			u32TmpBuf2 &= ~(PWM_CLOCK_SELECTOR_MASK << PWM_CLOCK_CH3_SELECTOR_BIT);
			u32TmpBuf2 |= 
				((PWMDevConfig->u8PWMBaseCycleFrequency & PWM_CLOCK_SELECTOR_MASK) << PWM_CLOCK_CH3_SELECTOR_BIT);
			
			/* set Toggle mode */
			u32TmpBuf3 |= (1 << PWM_CTRL_CH3_MODE_BIT);
			break;	
		default:
			return -ENODEV;
			break;
	}
	
	/* write new PWM register value  */
	iowrite32(u32TmpBuf, (void *) u32PrescaleAddr);  
	iowrite32(u32TmpBuf2, (void *) u32CSelectorAddr);  
	iowrite32(u32TmpBuf3, (void *) u32CtrlAddr);  

  /****************************************************************
	* Config PWM Counter register for setting resolution
	****************************************************************/
	u32DestAddr = (UINT32)PWM_REG_COUNTER_ADDR(u8PWMChannel) + offset;

	u32TmpBuf = (UINT32)(PWMDevConfig->u8PWMFrequencyDivider & 0xFF);

	/* write new PWMC value  */
	iowrite32(u32TmpBuf, (void *) u32DestAddr);  
	
  /****************************************************************
	* Config PWM Comparator register for setting duty cycle
	****************************************************************/
	u32DestAddr = (UINT32)PWM_REG_COMPARATOR_ADDR(u8PWMChannel) + offset;

	u32TmpBuf = (UINT32)((PWMDevConfig->u8PWMFrequencyDivider & 0xFF) / 2);

	/* write new PWMC value  */
	iowrite32(u32TmpBuf, (void *) u32DestAddr);  
	
  /****************************************************************
	* Enable PWM
	****************************************************************/
	u32TmpBuf3 = ioread32((void *) u32CtrlAddr);

	switch (u8PWMChannel)
	{
		case 0:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH0_ENABLE_BIT);
			break;	
		case 1:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH1_ENABLE_BIT);
			break;	
		case 2:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH2_ENABLE_BIT);
			break;	
		case 3:
			//Enable channel
			u32TmpBuf3 |= (1 << PWM_CTRL_CH3_ENABLE_BIT);
			break;	
		default:
			return -ENODEV;
			break;
	}
	
	/* write new PWM value  */
	iowrite32(u32TmpBuf3, (void *) u32CtrlAddr);  
	
  /****************************************************************
	* Enable PWM PIN
	****************************************************************/

	u32DestAddr = (UINT32)GLOBAL_REG_PIN_SELECT2_ADDR;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	u32TmpBuf |= 1 << (PWM_PIN_SELECT_CH0_BIT + PWMDevConfig->u8PWMChannelNum);
	iowrite32(u32TmpBuf, (void *) u32DestAddr);  

  /****************************************************************
	* Debug using
	****************************************************************/
	/* get PWM register address */	
	u32PrescaleAddr = (UINT32)PWM_REG_PRESCALE_ADDR + offset;
	u32CSelectorAddr = (UINT32)PWM_REG_CLOCK_SELECTOR_ADDR + offset;
	u32CtrlAddr = (UINT32)PWM_REG_CONTROL_ADDR + offset;
	
	/* read PWMD value */
	u32TmpBuf = ioread32((void *) u32PrescaleAddr);
	u32TmpBuf2 = ioread32((void *) u32CSelectorAddr);
	u32TmpBuf3 = ioread32((void *) u32CtrlAddr);

	printk("*** u8PWMChannel=%d, u32PrescaleAddr[0x%x]=0x%x \n",
		PWMDevConfig->u8PWMChannelNum, (unsigned int)u32PrescaleAddr, (unsigned int)u32TmpBuf);

	printk("*** u32CSelectorAddr[0x%x]=0x%x \n",
		(unsigned int)u32CSelectorAddr, (unsigned int)u32TmpBuf2);

	printk("*** u32CtrlAddr[0x%x]=0x%x \n",
		(unsigned int)u32CtrlAddr, (unsigned int)u32TmpBuf3);

	u32DestAddr = (UINT32)PWM_REG_COUNTER_ADDR(u8PWMChannel) + offset;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	printk("*** COUNTER_ADDR[0x%x]=0x%x \n",
		(unsigned int)u32DestAddr, (unsigned int)u32TmpBuf);

	u32DestAddr = (UINT32)PWM_REG_COMPARATOR_ADDR(u8PWMChannel) + offset;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	printk("*** COMPARATOR_ADDR[0x%x]=0x%x \n",
		(unsigned int)u32DestAddr, (unsigned int)u32TmpBuf);

	u32DestAddr = (UINT32)GLOBAL_REG_PIN_SELECT2_ADDR;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	printk("*** SELECT2_ADDR[0x%x]=0x%x \n",
		(unsigned int)u32DestAddr, (unsigned int)u32TmpBuf);

	return (STATUS_OK);
}


static int aess_pwm_open(struct inode *inode, struct file *filp)
{
	if (S_u8init_flag == AESSPWM_NOT_INIT)
	{
		DEBUG_MSG("KN:start init aess_pwm_driver\n");

		S_u8init_flag = AESSPWM_INIT_OK;
	
		DEBUG_MSG("KN:finish init aess_pwm_driver!\n");

	}
	return (STATUS_OK);
}

static int aess_pwm_release(struct inode* inode, struct file *flip)
{
#ifdef PWM_MAX_SPEED_ON_RELEASE
  UINT8 u8PwmChannel;
  sPWMDevConfig PWMDevConfig;

  // PWMDevConfig.u8PWMDutyCycle = 0;      // 0 is max fan speed
  for (u8PwmChannel = 0; u8PwmChannel < PWM_MAX_CHN_NUM; u8PwmChannel++)
  {
    //Set PWM value to initial value
    // DF286034 	IDRAC6 racadm reset causes a pnp removal of the perc6i card. Event ID 
    PWMDevConfig.u8PWMDutyCycle =   u8InitialPWM[u8PwmChannel];
    PWMDevConfig.u8PWMChannelNum = u8PwmChannel;
    (void) aess_pwm_config_set(&PWMDevConfig);
  }
#else
#ifdef PWM_OFF_ON_RELEASE
	UINT32 u32TmpBuf = 0;
	UINT32 u32DestAddr = 0;       
	
	/* PWMM0_BA = B800_7000h --> offset = 0x0h
	   PWMM1_BA = B800_7100h --> offset = 0x100h*/

	/* disable PWM0 channel */
	u32DestAddr = (UINT32)PWM_REG_CONTROL_ADDR;
	u32TmpBuf = ioread32((void *) u32DestAddr);
	u32TmpBuf &= ~((1 << PWM_CTRL_CH0_MODE_BIT) | 
					(1 << PWM_CTRL_CH1_MODE_BIT) | 
					(1 << PWM_CTRL_CH2_MODE_BIT) | (
					1 << PWM_CTRL_CH3_MODE_BIT));
					
	iowrite32(u32TmpBuf, (void *) u32DestAddr);  

	if (ioread32(GLOBAL_REG_PDID_REG) >= Z2_VERSION_NUM)
	{
		/* disable PWM1 channel */
		u32DestAddr = (UINT32)PWM_REG_CONTROL_ADDR + 0x100;
		u32TmpBuf = ioread32((void *) u32DestAddr);
		u32TmpBuf &= ~((1 << PWM_CTRL_CH0_MODE_BIT) | 
						(1 << PWM_CTRL_CH1_MODE_BIT) | 
						(1 << PWM_CTRL_CH2_MODE_BIT) | 
						(1 << PWM_CTRL_CH3_MODE_BIT));
						
		iowrite32(u32TmpBuf, (void *) u32DestAddr);  
	}
#endif
#endif

	DEBUG_MSG("\n aess_pwm_release exit!\n");
	
	return 0;
}

static int aess_pwm_ioctl(struct inode * inode, struct file *flip, 
						   unsigned int cmd, unsigned long arg)
{
	int err_check;
	sPWMDevConfig *PWMDevConfig;

	DEBUG_MSG(KERN_DEBUG "KN:aess_pwm_ioctl\n");
	
	PWMDevConfig = (sPWMDevConfig *) arg;

	/* Check the structure address is OK */
	if(!access_ok(VERIFY_READ, (void *) PWMDevConfig, sizeof(sPWMDevConfig)))
	{
		DEBUG_MSG("KN: struct access error!!\n");
		
		/* error occur */
		return -EFAULT;
	}
	switch(cmd)
	{
		case AESS_PWM_CONFIG_INIT: 
			DEBUG_MSG(KERN_DEBUG "KN: Start PWM Init config process!!\n");
			DEBUG_MSG("aess_pwm_ioctl[%d]\n", PWMDevConfig->u8PWMChannelNum);
			
			err_check = aess_pwm_config_init(PWMDevConfig);
			
			DEBUG_MSG(KERN_DEBUG "KN:Finish PWM Init config process!!\n");
			break;
			
		case AESS_PWM_CONFIG_SET:
		
			DEBUG_MSG(KERN_DEBUG "KN: Start PWM Set config process!!\n");
			DEBUG_MSG("aess_pwm_ioctl[%d]\n", PWMDevConfig->u8PWMChannelNum);
			
			err_check = aess_pwm_config_set(PWMDevConfig);
			
			DEBUG_MSG(KERN_DEBUG "KN:Finish PWM Set config process!!\n");
			break;
			
		case AESS_PWM_CONFIG_INFO:
		
			DEBUG_MSG(KERN_DEBUG "KN: Start PWM read config process!!\n");
			DEBUG_MSG("aess_pwm_ioctl[%d]\n", PWMDevConfig->u8PWMChannelNum);
			
			err_check = aess_pwm_config_info(PWMDevConfig);
			
			DEBUG_MSG(KERN_DEBUG "KN:Finish PWM Set config process!!\n");
			break;
			
		case AESS_PWM_CONFIG_DEBUG:
		
			DEBUG_MSG(KERN_DEBUG "KN: Start PWM read config process!!\n");
			DEBUG_MSG("aess_pwm_ioctl[%d]\n", PWMDevConfig->u8PWMChannelNum);
			
			err_check = aess_pwm_config_debug(PWMDevConfig);
			
			DEBUG_MSG(KERN_DEBUG "KN:Finish PWM Set config process!!\n");
			break;
			
		default:
			DEBUG_MSG(KERN_ERR "KN:aess_pwm_ioctl, NEWCMD=%lx OLDCMD=0xlx command error\n", cmd, AESS_PWM_CONFIG_SET);
			err_check = -EINVAL;
			break;
	}
	
	/* 0->ok, minus->fail */
	return err_check;
}

static struct file_operations aess_pwm_fops = {
	.open = aess_pwm_open, 
	.ioctl = aess_pwm_ioctl,
	.release = aess_pwm_release,     
};

int __init aess_pwm_init(void)
{
	int result;
    int i;

    //Initialize PWM values for max speed
    // DF286034 	IDRAC6 racadm reset causes a pnp removal of the perc6i card. Event ID 
    for(i=0;i<PWM_MAX_CHN_NUM;i++)
    {
        u8InitialPWM[i] = 0;
    }

	DEBUG_MSG(KERN_DEBUG "KN:init_aess_pwm_module\n");
	
	pwm_cdev = cdev_alloc();
   
	pwm_cdev->ops = &aess_pwm_fops;
	result = alloc_chrdev_region(&pwm_dev, 0, 1, driver_name);

	printk("mknod /dev/aess_pwmdrv c %d 0\n", MAJOR(pwm_dev));

	if (result < 0) {
		DEBUG_MSG (KERN_ERR "KN:Registering the PWM character device failed with %d\n", MAJOR(pwm_dev));
		return result;
	}

	cdev_add(pwm_cdev, pwm_dev, 1);
	return (STATUS_OK);
}

static void __exit aess_pwm_exit(void)
{
	DEBUG_MSG("aess_pwmdrv_exit\n");
	cdev_del(pwm_cdev);
	unregister_chrdev_region(pwm_dev, 1);
}


MODULE_DESCRIPTION("AESS PWM Driver");
MODULE_AUTHOR("Watt Wu <watt.wu@avocent.com>");
MODULE_LICENSE("GPL");
module_init(aess_pwm_init);
module_exit(aess_pwm_exit);
module_param(driver_name, charp, S_IRUGO);
