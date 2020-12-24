/*
 * $RCSfile: aess_pecisensordrv.c,v $
 * $Revision: 1.2 $
 * $Date: 2008/10/08 12:35:45 $
 * $Author: kellyhung $
 *
 * On chip peci sensor driver.
 *  
 * Copyright (C) 2007 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#define AESSPECISENSORDRV_C
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

#include <linux/timer.h>
#include <linux/jiffies.h>

#include <linux/kfifo.h>
#include <asm/atomic.h>
#include <asm/bitops.h>


#include "aess_pecisensordrv.h"

//#define PECISENSOR_DEBUG
#ifdef PECISENSOR_DEBUG
	#define PDEBUG(fmt, args...) printk(fmt, ## args)
#else
	/* not debugging: nothing */
	#define PDEBUG(fmt, args...) 
#endif

/* driver name will passed by insmod command */
static char *driver_name = "aess_pecisensordrv"; 

static dev_t dev;
struct cdev *pecisensor_cdev;
volatile static UINT8 S_u8InitFlag = NOT_INIT;
volatile static UINT8 S_u8DrvState = PECI_DRV_IDLE_STATE;
volatile static UINT8 S_u8DrvErrorState = PECI_DRV_ERROR_NONE;
/* initiate wait queue */
DECLARE_WAIT_QUEUE_HEAD (WaitQueue);
volatile UINT8 WaitQueueFlag;
volatile UINT8 CTL_STS = 0 ;
volatile UINT32 PECIDrv;

volatile static UINT8 S_au8ClientAddr[MAX_CPU_NUMBER] = {0x30, 0x31, 0x32, 0x33};

static int aess_pecisensor_open(struct inode *inode, struct file *filp)
{	
  if (S_u8InitFlag == NOT_INIT)
  {
    PDEBUG("PECI_KN: aess_pecisensor_open, init register\n");	

    /** Enabled PECI Clock Enabled Bit */
    set_bit(PECI_CLOCK_ENABLED_BIT,(void *) (WPCM450_VA_GCR + 0x200)); 
    S_u8InitFlag = INIT_DONE;
  }
  return (STATUS_OK);
}

static int aess_pecisensor_read(sPECISensorData *pPECISensorStruct)
{
	UINT8   u8ProcessorDomainNum = 0;
	UINT8   u8ProcessorNum = 0;
	UINT8   u8ClientAddr = 0;
	UINT8   u8Data = 0;
	UINT32  u32Data = 0;	
	UINT32  u32wait_count = 150000;
	UINT32  u32PECIReading = 0;	
	
	PDEBUG("PECI_KN:Enter aess_pecisensor_read \n");
		
	/* Check the buffer address exist in the structure is OK */
	if(!access_ok(VERIFY_READ, &(pPECISensorStruct->u8ProcessorDomainNum),
													 sizeof(UINT8)))
	{
		PDEBUG("PECI_KN:buffer access error\n");
		/* error occur */
		return -EFAULT;
	}
		 
	/* Check the buffer address exist in the structure is OK */
	if(!access_ok(VERIFY_WRITE, &(pPECISensorStruct->u8PECIReading),
							  sizeof(UINT8)))
	{
		PDEBUG("PECI_KN:buffer access error\n");
		/* error occur */
		return -EFAULT;
	}
	 
	u8ProcessorDomainNum = pPECISensorStruct->u8ProcessorDomainNum;   
	u8ProcessorNum = ((u8ProcessorDomainNum & PROCESS_NUM_MASK) >> GET_PROCESS_NUM);
	
	u8ClientAddr = S_au8ClientAddr[u8ProcessorNum];		
	
	/* check if the processor present */
	if (aess_pecisensor_query(pPECISensorStruct)!= STATUS_OK)
	{ 
		PDEBUG("PECI_KN:processor present error\n");
		return -ENODEV;
	}  
			
	/* Set GetTemp() command 0x01(domain 0)/0x02(domain 1) into write_data0 reg */
	if ((u8ProcessorDomainNum & DOMAIN_NUM_MASK) == DOMAIN_0)
	{
		u8Data = DOMAIN_0_CMD;
	}
	else if ((u8ProcessorDomainNum & DOMAIN_NUM_MASK) == DOMAIN_1)
	{
		/* We don't support processor domain 1 now */
		u8Data = DOMAIN_0_CMD;
	}
	else
	{
		PDEBUG("PECI_KN:domain error %d\n", u8ProcessorDomainNum);
		return -ENODEV;			
	}
	
	iowrite8(u8Data, (void *) REG_PECI_DAT_INOUT[0]);		
			
	iowrite8(1, (void *) REG_PECI_WR_LENGTH);	
	
	iowrite8(2, (void *) REG_PECI_RD_LENGTH);		
	
	iowrite8(u8ClientAddr, (void *) REG_PECI_ADDR);		

	/* PECI ping Command Setting */
	iowrite8(1, (void *) REG_PECI_CMD);	

  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
  PDEBUG("PECI_KN: REG_PECI_ADDR      is 0x%08x\n",ioread8((void *) REG_PECI_ADDR));
  PDEBUG("PECI_KN: REG_PECI_CMD       is 0x%08x\n",ioread8((void *) REG_PECI_CMD));			
  PDEBUG("PECI_KN: REG_PECI_WR_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_WR_LENGTH));
  PDEBUG("PECI_KN: REG_PECI_RD_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_RD_LENGTH));	

		
	/* Avoid the optimize, because we consecutive write the PECI_REG_COMMAND */
	//barrier();	
  for(u32Data = 1500;u32Data >0;u32Data--);	

	/* Fire command */
  set_bit(START_BUSY,(void *) REG_PECI_CTL_STS); 
		
	/* wait for command completion */	
	while(!test_bit(DONE,(void *) REG_PECI_CTL_STS))
	{
		u32wait_count--;

		/* PECI command time out */
		if(u32wait_count == 0)
		{
      PDEBUG("PECI_KN: GetTmp() command time out error\n");
      PDEBUG("PECI_KN: REG_PECI_CTL_STS = 0x%02x\n",ioread8((void *) REG_PECI_CTL_STS));

      /* Clear Abort Error Bit */
      set_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS); 

      /* Clear CRC Error Bit */
      set_bit(CRC_ERR,(void *) REG_PECI_CTL_STS); 

      return (-EPROTO);
		}		
	}   

  /* Clear Done Bit */
  set_bit(DONE,(void *) REG_PECI_CTL_STS); 

  /* Check result, if cpu exist => reutlt != 0 */
  if(test_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS))
  {
    /* Clear Abort Error Bit */
    set_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS); 
    PDEBUG("PECI_KN: Abort Error, cpu doesn't exist\n");
    return (-ENODEV);
  }	
  
  /* Check result, if cpu exist => reutlt != 0 */
  if(test_bit(CRC_ERR,(void *) REG_PECI_CTL_STS))
  {
    /* Clear CRC Error Bit */
    set_bit(CRC_ERR,(void *) REG_PECI_CTL_STS); 	    
    PDEBUG("PECI_KN: CRC Error, cpu doesn't exist\n");
    return (-ENODEV);
  }		

  /* read the processor temperature data from the data register, about the 
     value format please referenc to the PECI spec */
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT[0] is 0x%02x\n",ioread8((void *) (REG_PECI_DAT_INOUT[0])));
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT[1] is 0x%02x\n",ioread8((void *) (REG_PECI_DAT_INOUT[1])));
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT[2] is 0x%02x\n",ioread8((void *) (REG_PECI_DAT_INOUT[2])));
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT[3] is 0x%02x\n",ioread8((void *) (REG_PECI_DAT_INOUT[3])));		
  
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT0 is 0x%02x\n",ioread8((void *) REG_PECI_DAT_INOUT0));
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT1 is 0x%02x\n",ioread8((void *) REG_PECI_DAT_INOUT1));	
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT2 is 0x%02x\n",ioread8((void *) REG_PECI_DAT_INOUT2));	
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT3 is 0x%02x\n",ioread8((void *) REG_PECI_DAT_INOUT3));	
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT4 is 0x%02x\n",ioread8((void *) REG_PECI_DAT_INOUT4));	
  PDEBUG("PECI_KN: REG_PECI_DAT_INOUT5 is 0x%02x\n",ioread8((void *) REG_PECI_DAT_INOUT5));		
  u32PECIReading = (UINT32) (ioread8((void *) REG_PECI_DAT_INOUT0) + (UINT32)(ioread8((void *) REG_PECI_DAT_INOUT1) << 8));		 

#if 0	
	PDEBUG("PECI_KN: reading value = 0x%08x,after calucate the reading value = %d \n",
	       u32PECIReading & PECI_READING_VALUE_MASK,
		   (((~u32PECIReading+1) & PECI_READING_VALUE_MASK) >> SHIFT_FRACTIONAL));
#endif
	
	/* Due to the sensor reading can only save 8 bits data, 
	   so we remove the fractional value */
	pPECISensorStruct->u8PECIReading = (UINT8)(((~u32PECIReading+1) & 
										PECI_READING_VALUE_MASK) >> 
										SHIFT_FRACTIONAL);
			   
	return STATUS_OK;
			
}

static int aess_pecisensor_query(sPECISensorData *pPECISensorStruct)
{
  UINT8   u8ProcessorDomainNum = 0;
  UINT8   u8ProcessorNum = 0;
  UINT8   u8ClientAddr = 0;
  UINT32  u32wait_count = 150000;	
  UINT32  u32Data = 0;	

  PDEBUG("PECI_KN:aess_pecisensor_query() Enter!\n");
  iowrite8(0x1A, (void *) REG_PECI_CTL_STS);
  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));

  /* Check the buffer address exist in the structure is OK */
  if(!access_ok(VERIFY_READ, &(pPECISensorStruct->u8ProcessorDomainNum), sizeof(UINT8)))
  {
    PDEBUG("PECI_KN:buffer access error\n");
    /* error occur */
    return -EFAULT;
  }

  u8ProcessorDomainNum = pPECISensorStruct->u8ProcessorDomainNum;   
  u8ProcessorNum = ((u8ProcessorDomainNum & PROCESS_NUM_MASK) >> GET_PROCESS_NUM); 
  u8ClientAddr = S_au8ClientAddr[u8ProcessorNum];		

  iowrite8(0, (void *) REG_PECI_WR_LENGTH);	

  iowrite8(0, (void *) REG_PECI_RD_LENGTH);		

  PDEBUG("PECI_KN:u8ClientAddr = 0x%02x\n",u8ClientAddr);

  iowrite8(u8ClientAddr, (void *) REG_PECI_ADDR);		

  /* PECI ping Command Setting */
  iowrite8(0, (void *) REG_PECI_CMD);	

#if 1	
  /* Avoid the optimize, because we consecutive write the PECI_REG_COMMAND */
  //barrier();
  for(u32Data = 1500;u32Data >0;u32Data--);	
#endif

  set_bit(START_BUSY,(void *) REG_PECI_CTL_STS); 

  /* wait for command completion */	
  while(!test_bit(DONE,(void *) REG_PECI_CTL_STS))
  {
    u32wait_count--;

    /* PECI command time out */
    if(u32wait_count == 0)
    {
      /* Clear Abort Error Bit */
      set_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS); 

      /* Clear CRC Error Bit */
      set_bit(CRC_ERR,(void *) REG_PECI_CTL_STS); 
  
      PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
      PDEBUG("PECI_KN: REG_PECI_ADDR      is 0x%08x\n",ioread8((void *) REG_PECI_ADDR));
      PDEBUG("PECI_KN: REG_PECI_CMD       is 0x%08x\n",ioread8((void *) REG_PECI_CMD));			
      PDEBUG("PECI_KN: REG_PECI_WR_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_WR_LENGTH));
      PDEBUG("PECI_KN: REG_PECI_RD_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_RD_LENGTH));	
      PDEBUG("PECI_KN: Ping() command time out error\n");
      return (-EPROTO);
    }		
  }	

  /* Clear Done Bit */
  set_bit(DONE,(void *) REG_PECI_CTL_STS); 

	/* Check result, if cpu exist => reutlt != 0 */
	if(test_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS))
	{
    /* Clear Abort Error Bit */
    set_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS); 
    PDEBUG("PECI_KN: Abort Error, cpu doesn't exist\n");
    return (-ENODEV);
	}	

  /* Check result, if cpu exist => reutlt != 0 */
  if(test_bit(CRC_ERR,(void *) REG_PECI_CTL_STS))
  {
    /* Clear CRC Error Bit */
    set_bit(CRC_ERR,(void *) REG_PECI_CTL_STS); 	    
    PDEBUG("PECI_KN: CRC Error, cpu doesn't exist\n");
    return (-ENODEV);
  }		

  PDEBUG("PECI_KN:cpu exist!!!!!!!!!!\n");

  return (STATUS_OK); 
}

static int aess_pecisensor_command(sPECICommandData *pPECICommandData)
{
  UINT8 u8Index = 0;
  UINT8 u8ClientAddr = 0;
  UINT8 u8ReadLength = 0;
  UINT8 u8WriteLength = 0;	
  UINT8 u8CommandCode = 0;
  UINT32 u32Count = 0;

  PDEBUG("PECI_KN: aess_pecisensor_command() Enter!\n");

  /* Check the buffer address exist in the structure is OK */
  if(!access_ok(VERIFY_READ, (void *) pPECICommandData, sizeof(sPECICommandData)))
  {
    PDEBUG("PECI_KN: aess_pecisensor_command() buffer access error Exit!\n");		
    /* error occur */
    return -EFAULT;
  }

  if(test_bit(START_BUSY, (void *) REG_PECI_CTL_STS))
  {
    PDEBUG("PECI_KN: aess_pecisensor_command() START_BUSY return -EBUSY!\n");	
    return -EBUSY;
  }

  iowrite8(0x1A, (void *) REG_PECI_CTL_STS);
  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));

  u8ClientAddr = pPECICommandData->u8ClientAddress;
  u8ReadLength = pPECICommandData->u8ReadLength;
  u8WriteLength = pPECICommandData->u8WriteLength;	
  u8CommandCode = pPECICommandData->u8CommandCode;

  PDEBUG("PECI_KN: u8ClientAddr is 0x%02x\n",u8ClientAddr);
  PDEBUG("PECI_KN: u8WriteLength is 0x%02x\n",u8WriteLength);    
  PDEBUG("PECI_KN: u8ReadLength is 0x%02x\n",u8ReadLength);
  PDEBUG("PECI_KN: u8CommandCode is 0x%02x\n",u8CommandCode);
  
  iowrite8(u8WriteLength, (void *) REG_PECI_WR_LENGTH);	
  iowrite8(u8ReadLength , (void *) REG_PECI_RD_LENGTH);		
  iowrite8(u8ClientAddr , (void *) REG_PECI_ADDR);		
  iowrite8(u8CommandCode, (void *) REG_PECI_CMD);	

  for(u32Count = 1500; u32Count >0; u32Count--);

  PDEBUG("PECI_KN:====================\n");
  PDEBUG("PECI_KN: REG_PECI_ADDR      is 0x%02x\n",ioread8((void *) REG_PECI_ADDR));
  PDEBUG("PECI_KN: REG_PECI_CMD       is 0x%02x\n",ioread8((void *) REG_PECI_CMD));			
  PDEBUG("PECI_KN: REG_PECI_WR_LENGTH is 0x%02x\n",ioread8((void *) REG_PECI_WR_LENGTH));
  PDEBUG("PECI_KN: REG_PECI_RD_LENGTH is 0x%02x\n",ioread8((void *) REG_PECI_RD_LENGTH));	
  PDEBUG("PECI_KN:====================\n");

  for(u8Index = 0; u8Index < (u8WriteLength - 1); u8Index++)
  {
    PDEBUG("PECI_KN: u8CommandData[%d] is 0x%02x\n",u8Index,pPECICommandData->u8CommandData[u8Index]);
    iowrite8(pPECICommandData->u8CommandData[u8Index], (void *) (REG_PECI_DAT_INOUT[u8Index]));	    
  }

  for(u8Index = 0; u8Index < PECI_DATA_INOUT_REG_NUMBER; u8Index++)
  {
    PDEBUG("PECI_KN: REG_PECI_DAT_INOUT%d is 0x%02x\n",u8Index,ioread8((void *) (REG_PECI_DAT_INOUT[u8Index])));
  }
	
  PDEBUG("PECI_KN: Enable Done Interrupt!\n");
  /* Enable Done Interrupt  */
  set_bit(DONE_EN,(void *) REG_PECI_CTL_STS); 
  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));

  PDEBUG("PECI_KN: Fire command!\n");
  /* Fire command */
  set_bit(START_BUSY,(void *) REG_PECI_CTL_STS); 

  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
  PDEBUG("PECI_KN: wait for PECI transaction!\n");

  WaitQueueFlag = 0;

  /* wait for I2C master transaction */
  if (0 == wait_event_interruptible_timeout(WaitQueue, WaitQueueFlag == 1, 20))
  {
    PDEBUG("PECI_KN: aess_pecisensor_command() command time out error\n");
    PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
    PDEBUG("PECI_KN: REG_PECI_ADDR      is 0x%08x\n",ioread8((void *) REG_PECI_ADDR));
    PDEBUG("PECI_KN: REG_PECI_CMD       is 0x%08x\n",ioread8((void *) REG_PECI_CMD));			
    PDEBUG("PECI_KN: REG_PECI_WR_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_WR_LENGTH));
    PDEBUG("PECI_KN: REG_PECI_RD_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_RD_LENGTH));	
    PDEBUG("PECI_KN:======================================\n");

    PDEBUG("PECI_KN: Clear Abort Error Bit!\n");
    /* Clear Abort Error Bit */
    set_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS); 

    PDEBUG("PECI_KN: Clear CRC Error Bit!\n");
    /* Clear CRC Error Bit */
    set_bit(CRC_ERR,(void *) REG_PECI_CTL_STS); 
    PDEBUG("PECI_KN: aess_pecisensor_command() crc timeout error Exit!\n\n\n");
    return (-EPROTO);	    	 
  }

  WaitQueueFlag = 0;

  PDEBUG("PECI_KN: Check Abort Error Bit!\n");
  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));	
  /* Check result, if cpu exist => reutlt != 0 */
  if(test_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS))
  {
    PDEBUG("PECI_KN: aess_pecisensor_command() Abort Error, client doesn't exist\n");
    PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
    PDEBUG("PECI_KN: REG_PECI_ADDR      is 0x%08x\n",ioread8((void *) REG_PECI_ADDR));
    PDEBUG("PECI_KN: REG_PECI_CMD       is 0x%08x\n",ioread8((void *) REG_PECI_CMD));			
    PDEBUG("PECI_KN: REG_PECI_WR_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_WR_LENGTH));
    PDEBUG("PECI_KN: REG_PECI_RD_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_RD_LENGTH));	
    PDEBUG("PECI_KN:======================================\n");

    PDEBUG("PECI_KN: Clear Abort Error Bit!\n");
    /* Clear Abort Error Bit */
    set_bit(ABRT_ERR,(void *) REG_PECI_CTL_STS); 

    PDEBUG("PECI_KN: Clear Done Bit!\n");
    set_bit(DONE,(void *) REG_PECI_CTL_STS);	        
    PDEBUG("PECI_KN: aess_pecisensor_command() crc abort error Exit!\n\n\n");
  	return (-ENODEV);
  }	

  PDEBUG("PECI_KN: Check CRC Error Bit!\n");
  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));	
  /* Check result, if cpu exist => reutlt != 0 */
  if(test_bit(CRC_ERR,(void *) REG_PECI_CTL_STS))
  {
    PDEBUG("PECI_KN: aess_pecisensor_command() CRC Error, client doesn't exist\n");
    PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
    PDEBUG("PECI_KN: REG_PECI_ADDR      is 0x%08x\n",ioread8((void *) REG_PECI_ADDR));
    PDEBUG("PECI_KN: REG_PECI_CMD       is 0x%08x\n",ioread8((void *) REG_PECI_CMD));			
    PDEBUG("PECI_KN: REG_PECI_WR_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_WR_LENGTH));
    PDEBUG("PECI_KN: REG_PECI_RD_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_RD_LENGTH));	
    PDEBUG("PECI_KN:======================================\n");

    PDEBUG("PECI_KN: Clear CRC Error Bit!\n");
    /* Clear CRC Error Bit */
    set_bit(CRC_ERR,(void *) REG_PECI_CTL_STS); 

    PDEBUG("PECI_KN: Clear Done Bit!\n");
    set_bit(DONE,(void *) REG_PECI_CTL_STS);
    PDEBUG("PECI_KN: aess_pecisensor_command() crc error Exit!\n\n\n");		       	    
    return (-ENODEV);
  }		

  if (test_bit(DONE,(void *) REG_PECI_CTL_STS))
  {
    PDEBUG("PECI_KN: Clear Done Bit!\n");
    set_bit(DONE,(void *) REG_PECI_CTL_STS);	
  }

  /* Check the buffer address exist in the structure is OK */
  if(!access_ok(VERIFY_WRITE, (void *) pPECICommandData, sizeof(sPECICommandData)))
  {
    PDEBUG("PECI_KN: aess_pecisensor_command() buffer access error Exit!\n");		
    /* error occur */
    return -EFAULT;
  }

  for(u8Index = 0; u8Index < u8ReadLength; u8Index++)
  {
    pPECICommandData->u8CommandData[u8Index] = ioread8((void *) (REG_PECI_DAT_INOUT[u8Index]));	    
    PDEBUG("PECI_KN: u8CommandData[%d] is 0x%02x\n",u8Index,pPECICommandData->u8CommandData[u8Index]);
  }

  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
  PDEBUG("PECI_KN: REG_PECI_ADDR      is 0x%08x\n",ioread8((void *) REG_PECI_ADDR));
  PDEBUG("PECI_KN: REG_PECI_CMD       is 0x%08x\n",ioread8((void *) REG_PECI_CMD));			
  PDEBUG("PECI_KN: REG_PECI_WR_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_WR_LENGTH));
  PDEBUG("PECI_KN: REG_PECI_RD_LENGTH is 0x%08x\n",ioread8((void *) REG_PECI_RD_LENGTH));	
  PDEBUG("PECI_KN:======================================\n");
  PDEBUG("PECI_KN: aess_pecisensor_command() Exit!\n\n\n");
  return (STATUS_OK);     
}

static int aess_pecisensor_ioctl(struct inode * inode, struct file *filp, 
						  unsigned int cmd, unsigned long arg)
{
  int err_check;
  sPECISensorData *pPECISensorStruct = (sPECISensorData *) arg;
  sPECICommandData *pPECICommandData = (sPECICommandData *) arg;	

  PDEBUG("PECI_KN:aess_pecisensor_ioctl() Enter!\n");

  /* Check the structure address is OK */
  if(!access_ok(VERIFY_WRITE, (void *) pPECISensorStruct, sizeof(sPECISensorData)))
  {
    PDEBUG("PECI_KN:struct access error\n");
    /* error occur */
    return -EFAULT;
  }

  switch(cmd)
  {   
    case AESS_PECIDRV_R:	   
      PDEBUG("PECI_KN:AESS_PECIDRV_R!\n");
      err_check = aess_pecisensor_read(pPECISensorStruct);  
      break;
    case AESS_PECIDRV_Q:	   
      PDEBUG("PECI_KN:AESS_PECIDRV_Q!\n");
      err_check = aess_pecisensor_query(pPECISensorStruct);  
      break;	
    case AESS_PECIDRV_COMMAND:
      PDEBUG("PECI_KN:AESS_PECIDRV_COMMAND!\n");
      err_check = aess_pecisensor_command(pPECICommandData);  
      break;		    
    default:
      PDEBUG(KERN_ERR "PECI_KN:aess_pecisensor_ioctl, command error! \n");
      err_check = -EINVAL;
  }

  PDEBUG("PECI_KN:IO control Error no is %d\n", err_check);

  /* 0->ok, minus->fail */
  return err_check;
}

static int aess_pecisensor_release(struct inode *inode, struct file *filp)
{   
	return (STATUS_OK);
}

irqreturn_t aess_peci_isr(int irq, void *dev_id, struct pt_regs *regs)
{	
  PDEBUG("PECI_KN: aess_peci_isr Enter!\n");
  PDEBUG("PECI_KN: REG_PECI_CTL_STS   is 0x%08x\n",ioread8((void *) REG_PECI_CTL_STS));
  clear_bit(DONE_EN,(void *) REG_PECI_CTL_STS);   
  WaitQueueFlag = 1;
  wake_up_interruptible(&WaitQueue);
  PDEBUG("PECI_KN: aess_peci_isr Exit!\n");    
  return IRQ_HANDLED;
}

struct file_operations aess_pecisensor_fops = {
  .open = aess_pecisensor_open, 
  .ioctl = aess_pecisensor_ioctl,
  .release = aess_pecisensor_release,
};

int __init aess_pecisensor_init(void)
{
  int result;
  pecisensor_cdev = cdev_alloc();

  PDEBUG("Init_aess_pecisensor_module Sucess! \n");
  pecisensor_cdev->ops = &aess_pecisensor_fops;
  result = alloc_chrdev_region(&dev, 0, 1, driver_name);

  if (result < 0)
  {
    PDEBUG (KERN_ERR "PECI_KN:Registering the character device failed with %d\n", MAJOR(dev));
    return result;
  }
  
  cdev_add(pecisensor_cdev, dev, 1);

  /* Request PECI IRQ */                             
  result = request_irq(IRQ_PECI,(void *) aess_peci_isr, IRQF_SAMPLE_RANDOM | IRQF_SHARED, "PECI",(void *) &PECIDrv);

  if (result)
  {
    PDEBUG("KN: %d request IRQ error\n", result);
  }

  printk("mknod /dev/aess_pecisensordrv c %d 0\n", MAJOR(dev));

  return (STATUS_OK);
}

void __exit aess_pecisensor_exit(void)
{
  /* free the irq */
  free_irq(IRQ_PECI,(UINT32 *) &PECIDrv);

  cdev_del(pecisensor_cdev);
  unregister_chrdev_region(dev,1);
  PDEBUG ("PECI_KN:aess_pecisensor_exit \n");

  return;
}


MODULE_DESCRIPTION("AESS peci Sensor Driver");
MODULE_AUTHOR("Timothy Huang <Timothy.Huang@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_pecisensor_init);
module_exit(aess_pecisensor_exit);
