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
 
#ifndef AESSTIMERDRV_H
#define AESSTIMERDRV_H

#ifdef AESSTIMERDRV_C 

/* IOCTL command */
#define AESS_HWTIMERDRV_IOC_MAGIC    0xC0
#define AESS_HWTIMERDRV_ADD          _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 0, int)
#define AESS_HWTIMERDRV_ADDBH        _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 1, int)
#define AESS_HWTIMERDRV_DEL          _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 2, int)
#define AESS_HWTIMERDRV_START        _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 3, int)
#define AESS_HWTIMERDRV_STOP         _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 4, int)
#define AESS_HWTIMERDRV_RESET        _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 5, int)
#define AESS_HWTIMERDRV_RESUME       _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 6, int)
#define AESS_HWTIMERDRV_WAITTIMEOUT  _IOWR(AESS_HWTIMERDRV_IOC_MAGIC, 7, int)

/* For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;
typedef u64                 UINT64;

/* init flag */
#define INIT_OK                 0x0
#define INIT_FAIL               0x1

/* Timer status */
#define NOT_USED_YET            0x0
#define USED_BY_TIMERDRV        0x1
#define USED_BY_TIMERDRV_STOP   0x2
#define NOT_TIMERDRV_RESOURCE   0x3

/* Timer resource definition */
#define TIMER_RS0               0x0
#define TIMER_RS1               0x1
#define TIMER_RS2               0x2
#define TIMER_RS3               0x3
#define TIMER_RS4               0x4

/* Callback function execute area */
#define USER_SPACE              0x1
#define KERNEL_SPACE            0x2
#define BOTTOM_HALF             0x3

/* MISC */
#define MAX_TIMER_NUMBER        0x5
#define DISABLE                 0x0
#define ENABLE                  0x1
#define CLEAR_INTERRUPT         0x1
#define USE_ORIGIN_VALUE        0x0
#define ONE_SHOT_TIMER          0x0
#define ALWAYS_TIMER            0x1
#define EXECUTE_BH              0x1

/******************************************************************************
*   STRUCT      :   sTimerInfo
******************************************************************************/
/**
 *  @brief   Structure to event handler driver related data parameter.
 *
 *****************************************************************************/
typedef struct 
{
	/** Timer number */ 
	UINT8 u8Number;

	/** Function pointer for call-back function */
	void *func;
	
	/** 32bits parameter for call back function */
	UINT32 u32Parameter;

	/** Count down value for timer, 1 tick for 25ns when CPU running 40MHz */ 
	UINT32 u32Tick;

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
	UINT8 u8Type;

	/** Timer callback function is in user space(1), kernel space(2) 
		or kernel space - bottom half type(3) */
	UINT8 u8Space;

} sTimerInfo;


/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
static void aess_timerdrv_datainit(void);

static int aess_timerdrv_open(struct inode *inode, struct file *filp);

static int aess_timerdrv_close(struct inode* inode, struct file *flip); 
 
static int aess_timerdrv_add(
							  /* Timer structure */       
							  sTimerInfo *psInfo,
							  
							  /** Timer callback function is in 
							  	user space(1), kernel space(2) or 
							  	kernel space - bottom half type(3) */
							  UINT8 u8Space,
							  
							  /* timer baseaddr for kernel space only*/
							  UINT32 *pu32Baseaddr
							);

static int aess_timerdrv_del(
							  /* Timer structure */       
							  sTimerInfo *psInfo
							);
							
static int aess_timerdrv_start(
							    /* Timer structure */       
							    sTimerInfo *psInfo
							  );
							
static int aess_timerdrv_stop(
							   /* Timer structure */       
							   sTimerInfo *psInfo
							 );
							
static int aess_timerdrv_reset(
							    /* Timer structure */       
							    sTimerInfo *psInfo
							  );
							
static int aess_timerdrv_resume(
								 /* Timer structure */       
								 sTimerInfo *psInfo
							   );

static int aess_timerdrv_waittimeout(
									  /* Timer structure */
									  sTimerInfo *psInfo
									);

static int aess_timerdrv_ioctl(struct inode * inode, struct file *flip,
							   unsigned int cmd, unsigned long arg);

int aess_timerdrv_commander(
						     /** Timer Command */
						     UINT32 u32Command,
						       
						     /** Timer number */
						     UINT8 *pu8Number,
						       
						     /** Function pointer for call-back 
						      function */
						     void *func,
                             
						     /** 32bits parameter for call back 
						      function */
						     UINT32 u32Parameter,
                             
						     /** Count down value for timer, 1 tick 
						      for 25ns when CPU running 40MHz */ 
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
						   );

static void aess_timerdrv_bh(UINT32 u32Data);

static irqreturn_t aess_timerdrv_isr(int irq, void *dev_id/*, struct pt_regs *regs*/);
  
static void aess_timerdrv_isr_handler (UINT8 u8Number);
							
int __init aess_timerdrv_init(void);

static void __exit aess_timerdrv_exit(void);                                                                                                                                                           


#else
extern int aess_timerdrv_commander(
								    /** Timer Command */
								    unsigned long int u32Command,
                                    
								    /** Timer number */
								    unsigned char *pu8Number,
                                    
								    /** Function pointer for call-back 
								      function */
								    void *func,
                                    
								    /** 32bits parameter for call back 
								      function */
								    unsigned long int u32Parameter,
                                    
								    /** Count down value for timer, 1 tick 
								      for 25ns when CPU running 40MHz */ 
								    unsigned long int u32Tick,
								    
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
								    unsigned char u8Type,
								    
								    /** Return base address */
								    unsigned long int * pu32baseaddr                                  
								  );
								 
#endif   /* AESSTIMERDRV_C */

#endif   /* AESSEVENTHANDLER_H */
