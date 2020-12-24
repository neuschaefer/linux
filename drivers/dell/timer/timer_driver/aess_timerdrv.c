/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 Kernel Hardware Timer driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSTIMERDRV_C

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/arch/map.h>
#include <asm/arch/regs-timer.h>
#include <asm/arch/regs-irq.h>
#include <asm/arch/irqs.h>
#include "aess_timerdrv.h"


/* Debugging Message */
#if 0
#define DEBUG_MSG(fmt, args...) printk("TimerDrv: " fmt, ## args)
#else
#define DEBUG_MSG(fmt, args...)
#endif

/* enable a background timer to poll statuses of all initiated timer buses */
#if 0
#define TIMER_POLLING_STATUS
#define TIMER_POLLING_INTERVAL 500   /* 1 = 1 jiffies = 10 ms */
#endif

#ifdef TIMER_POLLING_STATUS
struct timer_list timer_polling;
#endif

#if 0
#define TIMER_MEASURE_BY_GPIO
#endif


/* Linux device data structure */ 
dev_t hwt_dev;
struct cdev *timerdrv_cdev; 

/* driver name will passed by insmod command */
static char *driver_name="aess_timerdrv";

/* TimerDrv data structure */
static sTimerInfo S_sTimerInfo[MAX_TIMER_NUMBER];

/* Record the current available timer resource */
static UINT8 S_u8CurResourceNum;
static UINT8 S_au8TimerStatus[MAX_TIMER_NUMBER] = {NOT_TIMERDRV_RESOURCE,
												   NOT_USED_YET,
												   NOT_USED_YET,
												   NOT_USED_YET,
												   NOT_USED_YET
												 };

/* wait queue, use to blocking the wait timeout process */
static wait_queue_head_t S_sWaitQueue[MAX_TIMER_NUMBER];

/* wait queue condition flag */
static UINT8 S_au8Condflag[MAX_TIMER_NUMBER];

/* Init flag */
static UINT8 S_u8InitFlag = INIT_FAIL;

/* Mutex, used for protect the AESS_TIMERDRV_ADD/AESS_TIMERDRV_ADDBH command */
static struct semaphore S_sMutex;

/* Declare tasklet and bh flag */
static UINT8 S_au8BHFlag[MAX_TIMER_NUMBER];
DECLARE_TASKLET(timerdrvbh, aess_timerdrv_bh, 0);

/* function  prototype for call back function */
typedef void (* TIMER_CALL_BACK_FUNC) (UINT32);


/******************************************************************************
*   STRUCT      :   timer_reg_type
******************************************************************************/
/**
 *  @brief   structure of timer hardware register
 *
 *****************************************************************************/
typedef struct 
{
	void *tcsr; /* timer control and status */
	void *ticr; /* timer initial conunt */
	void *tdr;  /* timer data */
} timer_reg_type;

/* timer hardware register structure array */
static timer_reg_type timer_reg[] = {
								     {
									  WPCM450_TCSR0,
                                      WPCM450_TICR0,
								      WPCM450_TDR0
								     },
								     {
									  WPCM450_TCSR1,
                                      WPCM450_TICR1,
								      WPCM450_TDR1
								     },
								     {
									  WPCM450_TCSR2,
                                      WPCM450_TICR2,
								      WPCM450_TDR2
								     },
								     {
									  WPCM450_TCSR3,
                                      WPCM450_TICR3,
								      WPCM450_TDR3
								     },
								     {
									  WPCM450_TCSR4,
                                      WPCM450_TICR4,
								      WPCM450_TDR4
								     }
								    };

typedef struct 
{
	UINT32 u32Min;      /* min settable timer initial count */
	UINT32 u32Max;      /* max settable timer initial count */
	UINT32 u32Prescale; /* clock prescale divide count */
	UINT8 u8Support;    /* support this resolution, 1:support 0:not support */
} timer_tick_type;

/* timer tick related information */
static timer_tick_type timer_tick[] 
       = {/* Min, Max, Prescale, Support */
          {1, 16777215,   0, 1},    /* 1 tick = hardware limit */
          {1, 16777215,  23, 1},    /*                    1 ns */
          {1, 16777215, 239, 1},    /*                   10 ns */
          {1,  1677721, 239, 1},    /*                  100 ns */
          {1,   167772, 239, 1},    /*                    1 ms */
          {1,    16777, 239, 1},    /*                   10 ms */
          {1,     1677, 239, 1},    /*                  100 ms */
          {1,      167, 239, 1}     /*                    1  s */
         };

#define MAX_SUPPORT_RESOLUTION sizeof(timer_tick)/sizeof(timer_tick_type)

/* reset timer hardware register */
#define RESET_TIMER(x) iowrite32(0x04000000, timer_reg[x].tcsr);

/* clear the timer interrupt flag */
#define CLR_INT_FLAG(x) iowrite32((UINT32) (1 << x), \
                                  WPCM450_TISR);

/* stop the timer */
#define STOP_COUNT(x) iowrite32((ioread32(timer_reg[x].tcsr) \
                                & 0xBFFFFFFF),               \
                                timer_reg[x].tcsr);

/* start the timer */
#define START_COUNT(x) iowrite32((ioread32(timer_reg[x].tcsr) \
                                 | 0x40000000),               \
                                 timer_reg[x].tcsr);

/* disable the timer interrupt (interrupt enable bit) */
#define DISABLE_INT(x) iowrite32((ioread32(timer_reg[x].tcsr) \
                                 & 0xDFFFFFFF),               \
                                 timer_reg[x].tcsr);

/* enable the timer interrupt (interrupt enable bit) */
#define ENABLE_INT(x) iowrite32((ioread32(timer_reg[x].tcsr) \
                                | 0x20000000),               \
                                (void *) timer_reg[x].tcsr);

/* initial count */
#define SET_INITIAL_COUNT(x, value) iowrite32((UINT32) value, timer_reg[x].ticr);

/* prescale setting, clock = (23+1)/24MHz = 1 us */
#define SET_PRESCALE_DIVIDE(x)                                            \
        iowrite32((ioread32(timer_reg[x].tcsr)                            \
                  | timer_tick[S_sTimerInfo[x].u8Type >> 4].u32Prescale), \
                  timer_reg[x].tcsr);

/* check timeout flag */
#define IS_TIMEOUT(x) (ioread32((void *) WPCM450_TISR) & (1 << x)) ? 1 : 0;

/* stop the timer and disable the timer interrupt */

/* start the timer and enable the timer interrupt */

/* timer register base */
#define GET_TIMER_REG_BASE(x) &timer_reg[x];

/* one-shot timer */
#define SET_ONE_SHOT_TIMER(x) iowrite32((ioread32(timer_reg[x].tcsr) \
                                        & 0xE7FFFFFF),               \
                                        timer_reg[x].tcsr);

/* periodic timer */
#define SET_PERIODIC_TIMER(x) iowrite32(((ioread32(timer_reg[x].tcsr) \
                                        & 0xE7FFFFFF) | 0x08000000),  \
                                        timer_reg[x].tcsr);

#ifdef TIMER_MEASURE_BY_GPIO
void aess_timer_set_gpio(int flag)
{
    static int gpio_init = 0;
    static int gpio_status[] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    if (!gpio_init)
    {
        /* GPIOE0~7 output low */
        *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) = 0x00;
        
        /* GPIOE0~7 as push-pull type */
        *((UINT32 *) (WPCM450_VA_GPIO + 0x18)) = 0x00;
        
        /* set GPIOE0~7 as output */
        *((UINT32 *) (WPCM450_VA_GPIO + 0x14)) = 0xFF;
        
        gpio_init = 1;
    }
    
    if (!gpio_status[flag])
    {
        *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) 
        = *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) | (1 << flag);
        
        gpio_status[flag] = 1;
    }
    else
    {
        *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) 
        = *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) & ~(1 << flag);
        
        gpio_status[flag] = 0;
    }
    
    return;
}
#endif


#ifdef TIMER_POLLING_STATUS
void aess_timer_polling_func(unsigned long data)
{
	UINT8 idx;
	
	/* DEBUGP("KN: polling...\n"); */
	
	printk("\nTISR=%x NR_IRQS=%d\n", ioread32(WPCM450_TISR), NR_IRQS);
	
	for(idx = 0; idx < 5; idx++)
	{
	    printk("t%d: TCSR=%x TICR=%x TDR=%x\n",
	            idx,
	            ioread32((void *) timer_reg[idx].tcsr),
	            ioread32((void *) timer_reg[idx].ticr),
	            ioread32((void *) timer_reg[idx].tdr));
	}
	
	printk("GEN=%x GRSR=%x IRSR=%x IASR=%x ISR=%x IMR=%x\n",
	        ioread32(WPCM450_AIC_GEN),
	        ioread32(WPCM450_AIC_GRSR),
	        ioread32(WPCM450_AIC_IRSR),
	        ioread32(WPCM450_AIC_IASR),
	        ioread32(WPCM450_AIC_ISR),
	        ioread32(WPCM450_AIC_IMR));
	        
	/* printk("SCR6=%x SCR7=%x SCR8=%x SCR12=%x SCR13=%x SCR14=%x SCR17=%x SCR18=%x\n",
	        ioread32(WPCM450_AIC_SCR6),
	        ioread32(WPCM450_AIC_SCR7),
	        ioread32(WPCM450_AIC_SCR8),
	        ioread32(WPCM450_AIC_SCR12),
	        ioread32(WPCM450_AIC_SCR13),
	        ioread32(WPCM450_AIC_SCR14),
	        ioread32(WPCM450_AIC_SCR17),
	        ioread32(WPCM450_AIC_SCR18)); */
	
	/* timer interval is TIMER_POLLING_INTERVAL */
	timer_polling.expires = jiffies + TIMER_POLLING_INTERVAL;
	add_timer(&timer_polling);
	
	return;
}
#endif


static void aess_timerdrv_datainit(void)
{
	UINT8 u8i = 0;    
	DEBUG_MSG("start init aess_timerdrv_driver\n");
   
	/* Clear timer structures */
	memset(S_sTimerInfo, 0, MAX_TIMER_NUMBER * sizeof(sTimerInfo));
    
	/* Release timer resources and init the wait queue */
	S_u8CurResourceNum = 0;
	
	for (u8i = 0; u8i < MAX_TIMER_NUMBER; u8i++)
	{
		if (S_au8TimerStatus[u8i] != NOT_TIMERDRV_RESOURCE)
		{
			S_au8TimerStatus[u8i] = NOT_USED_YET;
			init_waitqueue_head(&S_sWaitQueue[u8i]);
			S_au8Condflag[u8i] = 0;
			S_au8BHFlag[u8i] = 0;
			S_u8CurResourceNum++;
		}
	}
	
	init_MUTEX(&S_sMutex);
	S_u8InitFlag = INIT_OK;
	
	DEBUG_MSG("finish init aess_timerdrv_driver\n");
}


static UINT32 aess_timerdrv_checktick(UINT8 u8Type, UINT32 u32Tick)
{
    /* high nibble to choose the resolution of timer tick */
    u8Type = u8Type >> 4;
    
    DEBUG_MSG("MAX_SUPPORT_RESOLUTION=%d\n", MAX_SUPPORT_RESOLUTION);
    DEBUG_MSG("tick type=%d\n", u8Type);
    
    if(u8Type >= MAX_SUPPORT_RESOLUTION)
    {
        DEBUG_MSG("tick resoltion error %d\n", u8Type);
        return 0;
    }
    
    if (!timer_tick[u8Type].u8Support)
    {
        return 0;
    }
    
    if (u32Tick <= timer_tick[u8Type].u32Max 
        || u32Tick >= timer_tick[u8Type].u32Min)
    {
        /* adjust u32Tick */
        switch (u8Type)
        {
            case 3:
                
                u32Tick = u32Tick * 10;
                break;
                
            case 4:
                
                u32Tick = u32Tick * 100;
                break;
                
            case 5:
                
                u32Tick = u32Tick * 1000;
                break;
                
            case 6:
                
                u32Tick = u32Tick * 10000;
                break;
                
            case 7:
                
                u32Tick = u32Tick * 100000;
                break;
                
            default:
                break;
         }
    }
    else
    {
        return 0;
    }
    
    DEBUG_MSG("u32Tick=%x%x\n", 
              (UINT16) (u32Tick >> 16), 
              (UINT16) (u32Tick & 0xFFFF));
    
	return (u32Tick);
}


static int aess_timerdrv_open(struct inode *inode, struct file *filp)
{
	if (S_u8InitFlag == INIT_FAIL)
	{
		aess_timerdrv_datainit();
	}
	
	return (STATUS_OK);
}


static int aess_timerdrv_close(struct inode* inode, struct file *flip)
{
	UINT8 u8i = 0;
	DEBUG_MSG("aess_timerdrv_close exit\n");
    
	/* Disable all timer interrupt */
	for (u8i = 0; u8i < MAX_TIMER_NUMBER; u8i++)
	{
		if (S_au8TimerStatus[u8i] != NOT_TIMERDRV_RESOURCE)
		{
			/* Disable the timer */
			STOP_COUNT(u8i);
			
			/* Disable the timer interrupt */
			DISABLE_INT(u8i);
		}
	}
	
	return 0;
}


static int aess_timerdrv_add(
						      /* Timer structure */
						      sTimerInfo *psInfo,
						      
						      /* Timer callback function is in 
						         user space(1), kernel space(2) or 
						         kernel space - bottom half type(3) */
						      UINT8 u8Space,
						      
						      /* timer baseaddr for kernel space only */
						      UINT32 *pu32Baseaddr
							)
{
	UINT8 u8i = 0;
	UINT32 u32Tick;
    
	if (S_u8CurResourceNum > 0)
	{
		/* Get semaphore to do the add/addbh behavior */
		down_interruptible(&S_sMutex);
		
		/* Look for a available timer resource */
		for (u8i = 0; u8i < MAX_TIMER_NUMBER; u8i++)
		{
			if (S_au8TimerStatus[u8i] == NOT_USED_YET)
			{
			    /* check timer tick setting */
			    u32Tick = aess_timerdrv_checktick(psInfo->u8Type, 
			                                      psInfo->u32Tick);
			    
			    /* if the tick setting is not acceptable */
			    if (u32Tick == 0)
			    {
		            /* Release semaphore */
		            up(&S_sMutex);
		            
			        return -EINVAL;
			    }
			    
				/* Fill the timer data structure */
				S_sTimerInfo[u8i].u8Number = u8i;
				S_sTimerInfo[u8i].u32Tick = u32Tick;
				S_sTimerInfo[u8i].u8Type = psInfo->u8Type;
				S_sTimerInfo[u8i].u8Space = u8Space;
		        
				/* Save the timer number to user space */
				psInfo->u8Number = u8i;
				
				/* User space doesn't need to store the callback function */
				if (u8Space != USER_SPACE)
				{
					S_sTimerInfo[u8i].func = psInfo->func;
					S_sTimerInfo[u8i].u32Parameter = psInfo->u32Parameter;
					
					/* feed timer register base to pu32Baseaddr */
					*pu32Baseaddr = (UINT32) GET_TIMER_REG_BASE(u8i);
				}
			   
				/* Set current used timer status to USED_BY_TIMERDRV_STOP */
				S_au8TimerStatus[u8i] = USED_BY_TIMERDRV_STOP;
			   
				/* Clear the wake up condition falg */
				S_au8Condflag[u8i] = 0;
				
				/* Release semaphore */
				up(&S_sMutex);
			   
				return (STATUS_OK);
			}
		}
        
		/* Release semaphore */
		up(&S_sMutex);
		
		return -EBUSY; 
	}
	else
	{
		/* No timer resource available */
		return -EBUSY;     
	}
}


static int aess_timerdrv_del(
							  /* Timer structure */
							  sTimerInfo *psInfo
							)
{
	UINT8 u8i = psInfo->u8Number;
	/* Chenck the timer number is valid */
	if (u8i >= MAX_TIMER_NUMBER)
	{
		return -ENXIO;
	}
	
	/* Check if the specified timer is used by timer driver */
	if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV ||
		S_au8TimerStatus[u8i] == USED_BY_TIMERDRV_STOP)
	{
		/* Disable the timer */ 
		STOP_COUNT(u8i);  
		
		/* Disable the timer interrput */
		DISABLE_INT(u8i);
		
		/* Release the timer resource */ 
		memset(&S_sTimerInfo[u8i], 0, sizeof(sTimerInfo));
		S_au8TimerStatus[u8i] = NOT_USED_YET;
        
		/* Avoid the process hand on the wait queue, wake up it */
		S_au8Condflag[u8i] = 1;
		wake_up_interruptible(&S_sWaitQueue[u8i]);
        
		return (STATUS_OK);
	}
	else
	{
		/* Return no such device */
		return -ENXIO;
	}
}


static int aess_timerdrv_start(
							    /* Timer structure */
							    sTimerInfo *psInfo
							  )
{
	UINT8 u8i = psInfo->u8Number;
	
	/* Check the timer number is valid */
	if (u8i >= MAX_TIMER_NUMBER)
	{
		return -ENXIO;
	}
	
	/* Check if the specified timer is used by timer driver */
	if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV_STOP)
	{
#ifdef TIMER_MEASURE_BY_GPIO
        aess_timer_set_gpio((int) u8i);
#endif
        
        /* reset timer hardware register */
        RESET_TIMER(u8i);
        
		/* Set initial count to timer register */
		SET_INITIAL_COUNT(u8i, S_sTimerInfo[u8i].u32Tick);
		
        /* set clock prescale divide count */
        SET_PRESCALE_DIVIDE(u8i);
        
        /* check the timer is one-shot or periodic type */
        if ((S_sTimerInfo[u8i].u8Type & 0x0F) == ONE_SHOT_TIMER)
        {
            SET_ONE_SHOT_TIMER(u8i);
        }
        else
        {
            SET_PERIODIC_TIMER(u8i);
        }
        
		/* Set interrupt enable to timer register */
		ENABLE_INT(u8i);
        
		/* Enable the timer */
		START_COUNT(u8i);
        
		/* Set status to used by timerdrv */
		S_au8TimerStatus[u8i] = USED_BY_TIMERDRV;

		return (STATUS_OK);
			   
	}
	else
	{
		/* Return no such device */
		return -ENXIO;
	}
} 

static int aess_timerdrv_stop(
							   /* Timer structure */
							   sTimerInfo *psInfo
							 )
{
	UINT8 u8i = psInfo->u8Number;    
	/* Chenck the timer number is valid */
	if (u8i >= MAX_TIMER_NUMBER)
	{
		return -ENXIO;
	}
	
	/* Check if the specified timer is used by timer driver */
	if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV)
	{
		/* Disable the timer */ 
		STOP_COUNT(u8i);
		
		/* Set status to stop */
		S_au8TimerStatus[u8i] = USED_BY_TIMERDRV_STOP;
		return (STATUS_OK);
	}
	else
	{
		/* Return no such device */
		return -ENXIO;
	}
} 

static int aess_timerdrv_reset(
							    /* Timer structure */
							    sTimerInfo *psInfo
							  )
{
	UINT32 u32Tick;
	UINT8 u8i = psInfo->u8Number;
	/* Chenck the timer number is valid */
	if (u8i >= MAX_TIMER_NUMBER)
	{
		return -ENXIO;
	}
	
	/* Check if the specified timer is used by timer driver */
	if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV || 
		S_au8TimerStatus[u8i] == USED_BY_TIMERDRV_STOP)
	{
		/* Check the driver need to use the old value or new value 
		   to reset timer count value */
		if (psInfo->u32Tick == USE_ORIGIN_VALUE)
		{
			/* Use origin value to reset the timer */
			SET_INITIAL_COUNT(u8i, S_sTimerInfo[u8i].u32Tick);
		}
		else
		{
		    /* check timer tick setting */
			u32Tick = aess_timerdrv_checktick(psInfo->u8Type, 
			                                  psInfo->u32Tick);
			
			/* if the tick setting is not acceptable */
			if (u32Tick == 0)
			{
		        /* Release semaphore */
		        up(&S_sMutex);
		        
			    return -EINVAL;
			}
		    
			/* Use new value to reset the timer */
			SET_INITIAL_COUNT(u8i, u32Tick);
			
			/* Update the new value to data structure */
			S_sTimerInfo[u8i].u32Tick = u32Tick;
			
		}
		
		return (STATUS_OK);
	}
	else
	{
		/* Return no such device */
		return -ENXIO;
	}
} 


static int aess_timerdrv_resume(
							     /* Timer structure */
							     sTimerInfo *psInfo
							   )
{
	UINT8 u8i = psInfo->u8Number;
	/* Chenck the timer number is valid */
	if (u8i >= MAX_TIMER_NUMBER)
	{
		return -ENXIO;
	}
	
	/* Check if the specified timer is used by timer driver */
	if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV_STOP)
	{
        
        
        
		/* Set interrupt enable to timer register */
		ENABLE_INT(u8i);
        
		/* Enable the timer */
		START_COUNT(u8i);
        
		/* Set status to USED_BY_TIMERDRV */
		S_au8TimerStatus[u8i] = USED_BY_TIMERDRV;
        
		return (STATUS_OK);
	}
	else
	{
		/* Return no such device */
		return -ENXIO;
	}
}


static int aess_timerdrv_ioctl(struct inode * inode, struct file *flip, 
							   unsigned int cmd, unsigned long arg)
{   
	int err_check;
	sTimerInfo *psInfo = (sTimerInfo *) arg;
    
	/* Check the structure address is OK */
	if (!access_ok(VERIFY_WRITE, (void *) psInfo, sizeof(sTimerInfo)))
	{
		/* error occur */
		return -EFAULT;
	}
    
	if (S_u8InitFlag != INIT_OK)
	{
		/* Return Operation not permitted, need initi first */
		return -EPERM;
	}
	  
	switch(cmd)
	{
		case AESS_HWTIMERDRV_ADD:
			/* Pass the add command to command handler */
			err_check = aess_timerdrv_add(psInfo, USER_SPACE, NULL);
			break;
			
		case AESS_HWTIMERDRV_DEL:
			/* Pass the del command to command handler */
			err_check = aess_timerdrv_del(psInfo);
			break;
            
		case AESS_HWTIMERDRV_START:
			/* Pass the start command to command handler */
			err_check = aess_timerdrv_start(psInfo);
			break;
            
		case AESS_HWTIMERDRV_STOP:
			/* Pass the stop command to command handler */
			err_check = aess_timerdrv_stop(psInfo);
			break;
            
		case AESS_HWTIMERDRV_RESET:
			/* Pass the reset command to command handler */
			err_check = aess_timerdrv_reset(psInfo);
			break;
            
		case AESS_HWTIMERDRV_RESUME:
			/* Pass the resume command to command handler */
			err_check = aess_timerdrv_resume(psInfo);
			break;
            
		case AESS_HWTIMERDRV_WAITTIMEOUT:
			/* Pass the resume command to command handler */
			err_check = aess_timerdrv_waittimeout(psInfo);
			break;
			
		default:
			err_check = -EINVAL;
	}
	
	/* 0->ok, minus->fail */
	return err_check;
}


int aess_timerdrv_commander(
						     /** Timer Command */
						     UINT32 u32Command,
						     
						     /** Timer number */
						     UINT8 *pu8Number,
						     
						     /** Function pointer for 
						        call-back function */
						     void *func,
						     
						     /** 32bits parameter for 
						        call back function */
						     UINT32 u32Parameter,
						     
						     /** Count down value for timer */ 
						     UINT32 u32Tick,
						     
						     /** bit 7:4 Choose the resolution of timer tick
						                 0: hardware limit
						                 1: 1 ns
						                 2: 10 ns
						                 3: 100 ns
						                 4: 1 ms
						                 5: 10 ms
						                 6: 100 ms
						                 7: 1 s
						         
						         bit 3:0 Choose the mode tof timer
						                 0: one-shot timer 
						                 1: periodic timer (always) */
						     UINT8 u8Type,
						     
						     /** Return base address */
						     UINT32 *pu32baseaddr
						   )
{
	int err_check;
	sTimerInfo sInfo;
    
	if (S_u8InitFlag != INIT_OK)
	{
		aess_timerdrv_datainit();
	}
	  
	switch(u32Command)
	{
		case AESS_HWTIMERDRV_ADD:
			/* Check the function pointer and pu32baseaddr is OK */
			if (func == NULL || pu32baseaddr == NULL)
			{
				/* error occur */
				return -EFAULT;
			}
            
			/* Fill the needed data to data structure */
			sInfo.func = func;
			sInfo.u32Parameter = u32Parameter;
			sInfo.u32Tick = u32Tick;
			sInfo.u8Type = u8Type;
            
			/* Pass the add command to command handler */
			err_check = aess_timerdrv_add(&sInfo, KERNEL_SPACE, pu32baseaddr);
            
			/* Return the timer number */
			*pu8Number = sInfo.u8Number;
			
			break;
		    
		case AESS_HWTIMERDRV_ADDBH:
			/* Check the function pointer and pu32baseaddr is OK */
			if (func == NULL || pu32baseaddr == NULL)
			{
				DEBUG_MSG("aess_timerdrv_commander: pointer error!!\n");
				/* error occur */
				return -EFAULT;
			}
            
			/* Fill the needed data to data structure */
			sInfo.func = func;
			sInfo.u32Parameter = u32Parameter;
			sInfo.u32Tick = u32Tick;
			sInfo.u8Type = u8Type;
            
			/* Pass the addbh command to command handler */
			err_check = aess_timerdrv_add(&sInfo, BOTTOM_HALF, pu32baseaddr);
            
			/* Return the timer number */
			*pu8Number = sInfo.u8Number;
			break;
			
		case AESS_HWTIMERDRV_DEL:
			/* Fill the needed data to data structure */
			sInfo.u8Number = *pu8Number;
			
			/* Pass the del command to command handler */
			err_check = aess_timerdrv_del(&sInfo);
			break;
            
		case AESS_HWTIMERDRV_START:
			/* Fill the needed data to data structure */
			sInfo.u8Number = *pu8Number;
			
			/* Pass the start command to command handler */
			err_check = aess_timerdrv_start(&sInfo);
			break;
            
		case AESS_HWTIMERDRV_STOP:
			/* Fill the needed data to data structure */
			sInfo.u8Number = *pu8Number;
			
			/* Pass the stop command to command handler */
			err_check = aess_timerdrv_stop(&sInfo);
			break;
            
		case AESS_HWTIMERDRV_RESET:
			/* Fill the needed data to data structure */
			sInfo.u8Number = *pu8Number;
			sInfo.u32Tick = u32Tick;
		    
			/* Pass the reset command to command handler */
			err_check = aess_timerdrv_reset(&sInfo);
			break;
            
		case AESS_HWTIMERDRV_RESUME:
			/* Fill the needed data to data structure */
			sInfo.u8Number = *pu8Number;
		    
			/* Pass the resume command to command handler */
			err_check = aess_timerdrv_resume(&sInfo);
			break;            
			
		default:
			err_check = -EINVAL;
	}
	
	/* 0->ok, minus->fail */
	return err_check;
}


static int aess_timerdrv_waittimeout(
									  /* Timer structure */
									  sTimerInfo *psInfo
									)
{
	UINT8 u8i = psInfo->u8Number;       
	/* Chenck the timer number is valid */
	if (u8i >= MAX_TIMER_NUMBER)
	{
		return -ENXIO;
	}
    
	/* Check if the specified timer is used by timer driver */
	if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV || 
		S_au8TimerStatus[u8i] == USED_BY_TIMERDRV_STOP)
	{
	
		/* If no data (S_u8TotalEvent = 0), go to sleep */
		wait_event_interruptible(S_sWaitQueue[u8i], S_au8Condflag[u8i] > 0);
		S_au8Condflag[u8i] = 0;
			  
		/* If timer delete by user or not used by timer driver, return fail */
		if (S_au8TimerStatus[u8i] == NOT_USED_YET)
		{
			return (STATUS_FAIL);
		}
        
		/* If timer stop by user, return ok to free the locked process */
		if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV_STOP)
		{
			return (STATUS_OK);
		}
		
		/* Release timer resource, if timer type is one-shot or not */
		if ((S_sTimerInfo[u8i].u8Type & 0x0F) == ONE_SHOT_TIMER)
		{
            
			/* Make sure the timer never stop or killed */
			if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV)
			{
				/* Release timer resource and clear data structure */
				S_au8TimerStatus[u8i] = NOT_USED_YET;
				memset(&S_sTimerInfo[u8i], 0, sizeof(sTimerInfo));
				return (STATUS_OK);
			}
            
			/* Release timer resource and clear data structure */
			S_au8TimerStatus[u8i] = NOT_USED_YET;
			memset(&S_sTimerInfo[u8i], 0, sizeof(sTimerInfo));
			
			/* timer delete or stop by User space API */
			return -ENXIO;
		}
		else
		{
			return (STATUS_OK);
		}
	}
	else
	{
		/* Return no such device */
		return -ENXIO;
	}
}


static void aess_timerdrv_bh(UINT32 u32Data)
{
	UINT8 u8i = 0;
	TIMER_CALL_BACK_FUNC fCB;
    
	for (u8i =0; u8i <= MAX_TIMER_NUMBER; u8i++)
	{
		if (S_au8BHFlag[u8i] != EXECUTE_BH)
		{
			continue;
		}
        
		if (S_sTimerInfo[u8i].func != NULL)
		{
			/* Make sure the timer never stop or killed */
			if (S_au8TimerStatus[u8i] == USED_BY_TIMERDRV)
			{
				/* execute the callback function */
				fCB = (TIMER_CALL_BACK_FUNC) S_sTimerInfo[u8i].func;
				fCB(S_sTimerInfo[u8i].u32Parameter);
			}
		}
		
		/* Clear the bh flag */
		S_au8BHFlag[u8i] = 0;
		
		/* Release timer resource, if timer type is one-shot */
		if ((S_sTimerInfo[u8i].u8Type & 0x0F) == ONE_SHOT_TIMER)
		{
			/* Release timer resource and clear data structure */
			S_au8TimerStatus[u8i] = NOT_USED_YET;
			memset(&S_sTimerInfo[u8i], 0, sizeof(sTimerInfo));
		}
	}
}


static irqreturn_t aess_timerdrv_isr (int irq, void *dev_id /*, struct pt_regs *regs */)
{
    UINT32 u32TISR;
    
	/* Check the interrupt is caused by timer module or not. */
	u32TISR = ioread32((void *) WPCM450_TISR);
	
	DEBUG_MSG("irq=%d u32TISR=%x\n", irq, (UINT16) u32TISR);
	
	/* timer interrupt */
	if (u32TISR)
	{
		/* Process the interrupt for each channel */
		if (u32TISR & 0x00000001)
		{
		    DEBUG_MSG("T0\n");
		    
			/* Timer 0 */
			aess_timerdrv_isr_handler(TIMER_RS0);
		}
		
		if (u32TISR & 0x00000002)
		{
		    DEBUG_MSG("T1\n");
		    
			/* Timer 1 */
			aess_timerdrv_isr_handler(TIMER_RS1);
		}
		
		if (u32TISR & 0x00000004)
		{
		    DEBUG_MSG("T2\n");
		    
			/* Timer 2 */
			aess_timerdrv_isr_handler(TIMER_RS2);
		}
		
		if (u32TISR & 0x00000008)
		{
		    DEBUG_MSG("T3\n");
		    
			/* Timer 3 */
			aess_timerdrv_isr_handler(TIMER_RS3);
		}
		
		if (u32TISR & 0x00000010)
		{
		    DEBUG_MSG("T4\n");
		    
			/* Timer 4 */
			aess_timerdrv_isr_handler(TIMER_RS4);
		}
	}
	else
	{
		return IRQ_NONE;
	}
	
	u32TISR = ioread32((void *) WPCM450_TISR);
	DEBUG_MSG("irq=%d u32TISR=%x\n", irq, (UINT16) u32TISR);
	
	return IRQ_HANDLED;
}


static void aess_timerdrv_isr_handler (UINT8 u8Number)
{
	TIMER_CALL_BACK_FUNC fCB;
	
#ifdef TIMER_MEASURE_BY_GPIO
        aess_timer_set_gpio((int) u8Number);
#endif
	
	/* Timeout function in user space */
	if (S_sTimerInfo[u8Number].u8Space == USER_SPACE)
	{                      
		/* Release timer resource, if timer type is one-shot */
		if ((S_sTimerInfo[u8Number].u8Type & 0x0F) == ONE_SHOT_TIMER)
		{
			/* Disable timer interrupt */
			DISABLE_INT(u8Number);
            
			/* Disable the timer */
			STOP_COUNT(u8Number);
		}
        
		/* Clear interrupt status bit */
		CLR_INT_FLAG(u8Number);
        
		/* Change the flag to wake up the aess_timerdrv_waittimeout process */
		S_au8Condflag[u8Number] = 1;            
		wake_up_interruptible(&S_sWaitQueue[u8Number]);
	}
	/* Timeout function in kernel space */
	else if(S_sTimerInfo[u8Number].u8Space == KERNEL_SPACE)
	{
		/* Release timer resource, if timer type is one-shot */
		if ((S_sTimerInfo[u8Number].u8Type & 0x0F) == ONE_SHOT_TIMER)
		{
			/* Disable timer interrupt */
			DISABLE_INT(u8Number);
			
			/* Disable the timer */
			STOP_COUNT(u8Number);
            
			/* Clear interrupt status bit */
			CLR_INT_FLAG(u8Number);
            
			/* Make sure the timer never stop or killed */
			if (S_au8TimerStatus[u8Number] == USED_BY_TIMERDRV)
			{
				/* execute the callback function */
				fCB = (TIMER_CALL_BACK_FUNC) S_sTimerInfo[u8Number].func;
				fCB(S_sTimerInfo[u8Number].u32Parameter);
			}
			/* Release timer resource and clear data structure */
			S_au8TimerStatus[u8Number] = NOT_USED_YET;
			memset(&S_sTimerInfo[u8Number], 0, sizeof(sTimerInfo));
		}
		else
		{   
            
			/* Clear interrupt status bit */
			CLR_INT_FLAG(u8Number);
		    
			/* execute the callback function */
			fCB = (TIMER_CALL_BACK_FUNC) S_sTimerInfo[u8Number].func;
			fCB(S_sTimerInfo[u8Number].u32Parameter);
		}
	}
	/* Timeout function need to do in bottom half */
	else
	{
		/* Release timer resource, if timer type is one-shot */
		if ((S_sTimerInfo[u8Number].u8Type & 0x0F) == ONE_SHOT_TIMER)
		{
			/* Disable timer interrupt */
			DISABLE_INT(u8Number);
			
			/* Disable the timer */ 
			STOP_COUNT(u8Number);
		}
        
		/* Clear interrupt status bit */
		CLR_INT_FLAG(u8Number);
		
		/* Set the bottom half flag*/
		S_au8BHFlag[u8Number] = EXECUTE_BH;
	    
		/* Schedule the timeout function */        
		tasklet_schedule(&timerdrvbh);
	}
}


struct file_operations aess_timerdrv_fops = {
	.open = aess_timerdrv_open,
	.ioctl = aess_timerdrv_ioctl,
	.release = aess_timerdrv_close,
};


int __init aess_timerdrv_init(void)
{
	int result;
	
	DEBUG_MSG("aess_timerdrv_init\n");
	
	timerdrv_cdev = cdev_alloc();
    
	timerdrv_cdev->ops = &aess_timerdrv_fops;
	result = alloc_chrdev_region(&hwt_dev, 0, 1, driver_name);
    
	if (result < 0) {
		return result;
	}
    
	cdev_add(timerdrv_cdev, hwt_dev, 1);
	
	/* show this message for hardware timer install shell used */
    printk("mknod /dev/aess_timerdrv c %d 0\n", MAJOR(hwt_dev));
	
	/* Register IRQ, assign channel 0 data structure for dev_id */
	if (request_irq(IRQ_TIMER1, aess_timerdrv_isr, IRQF_SHARED|IRQF_SAMPLE_RANDOM , "Timer 1", 
		(void *) &S_sTimerInfo[TIMER_RS1])) 
	{
		return -EBUSY;
	}
	
	if (request_irq(IRQ_TGROUP, aess_timerdrv_isr, IRQF_SHARED|IRQF_SAMPLE_RANDOM , "Timer 2,3,4", 
		(void *) &S_sTimerInfo[TIMER_RS2])) 
	{
	    free_irq(IRQ_TIMER1, (void *) &S_sTimerInfo[TIMER_RS0]);
	    
		return -EBUSY;
	}
    
    /* enable timer controller interrupt group */
    iowrite32((ioread32(WPCM450_AIC_GEN) | 0x00070000), WPCM450_AIC_GEN);
    
#ifdef TIMER_POLLING_STATUS
    /* initiate polling timer */
    timer_polling.data = 0;
    timer_polling.function = &aess_timer_polling_func;
    
    /* timer interval is TIMER_POLLING_INTERVAL */
    timer_polling.expires = jiffies + TIMER_POLLING_INTERVAL;
    
    init_timer(&timer_polling);
	add_timer(&timer_polling);
#endif
    
	return (STATUS_OK);
}


static void __exit aess_timerdrv_exit(void)
{
	DEBUG_MSG("aess_timerdrv_exit\n");
	
	/* Release the character device related resource */
	cdev_del(timerdrv_cdev);
	unregister_chrdev_region(hwt_dev, 1);
	
	/* Release IRQ */
	free_irq(IRQ_TIMER1, (void *) &S_sTimerInfo[TIMER_RS1]);
	free_irq(IRQ_TGROUP, (void *) &S_sTimerInfo[TIMER_RS2]);
	
#ifdef TIMER_POLLING_STATUS
	del_timer(&timer_polling);
#endif
}


MODULE_DESCRIPTION("AESS Timer Driver");
MODULE_AUTHOR("Justin Lee <justin.lee@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_timerdrv_init);
module_exit(aess_timerdrv_exit);
EXPORT_SYMBOL(aess_timerdrv_commander);
