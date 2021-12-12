/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * Winbond On chip fan tach sensor driver.
 *  
 * Copyright (C) 2007 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#define AESSFANSENSORDRV_C

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>

#include "aess_fansensordrv.h"

//#define AESS_FANSENSOR_DEBUG
#ifdef AESS_FANSENSOR_DEBUG
	#define PDEBUG(fmt, args...) printk(fmt, ## args)
#else
	#define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

//#define AESS_FANSENSOR_TRACE
#ifdef AESS_FANSENSOR_TRACE
	#define PDEBUG2(fmt, args...) printk(fmt, ## args)
#else
	#define PDEBUG2(fmt, args...) /* not debugging: nothing */
#endif

/* driver name will passed by insmod command */
static char *driver_name = "aess_fansensordrv"; 

static dev_t dev;      
struct cdev *fansensor_cdev;

/* Init flag */
static UINT8 S_u8init_flag = AESSFANSENSOR_NOT_INIT;

/* for request irq use */
static UINT8 u8dummy = 0;

/* maximum fan tach input support */
#define WPCM450_MAX_FAN_TACH	 16

static sFanTachDev S_wpcm450_fan_tach[WPCM450_MAX_FAN_TACH];
static UINT8 S_wpcm450_fan_tach_select[2][2];
static UINT8 S_wpcm450_fan_tach_fail_cnt[WPCM450_MAX_FAN_TACH];

#define WPCM450_FAN_TACH_INUPT(mft, cmp)   \
    ((mft << 3) + (cmp << 2) + S_wpcm450_fan_tach_select[mft][cmp])

static struct timer_list wpcm450_fantach_timer;

#if 0
static inline void wpcm450_fan_tach_next(UINT8 mft, UINT8 cmp)
{
    UINT8 fan_in = 0;
    UINT8 fan_sel = 0;
    UINT8 i = 0;

    /* Compute Next FAN */
    i = 4;
    while (i--)
    {
        S_wpcm450_fan_tach_select[mft][cmp]
            = (S_wpcm450_fan_tach_select[mft][cmp] + 1) & 0x3;
        
        fan_in = WPCM450_FAN_TACH_INUPT(mft, cmp);
        
        if (FAN_TACH_DISABLE == S_wpcm450_fan_tach[fan_in].u8FanStatusFlag)
        {
            /* search next available fan */
            continue;
        }
        else
        {
            fan_sel = S_wpcm450_fan_tach_select[mft][cmp];
            
            if (WPCM450_CMPA == cmp)
            {
                iowrite8((UINT8) fan_sel, WPCM450_TINASEL(mft));
            }
            else
            {
                iowrite8((UINT8) fan_sel, WPCM450_TINBSEL(mft));
            }
            
            break;
        }
    }
}
#else
static inline void wpcm450_fan_tach_next(UINT8 mft, UINT8 cmp)
{
    UINT8 fan_sel = 0;

    /* Compute Next FAN */
   fan_sel =  (S_wpcm450_fan_tach_select[mft][cmp] + 1) & 0x3;

    if (WPCM450_CMPA == cmp)
    {
        iowrite8((UINT8) fan_sel, WPCM450_TINASEL(mft));
    }
    else
    {
        iowrite8((UINT8) fan_sel, WPCM450_TINBSEL(mft));
    }
    
    S_wpcm450_fan_tach_select[mft][cmp] = fan_sel;
}

#endif

static inline void wpcm450_fan_tach_start_polling(UINT8 mft, UINT8 cmp)
{
    UINT8 fan_in = 0;
    UINT8 reg_mode = 0;
    UINT8 reg_int = 0;

    fan_in = WPCM450_FAN_TACH_INUPT(mft, cmp);

    /* to check whether have any fan tach enable */
    if (FAN_TACH_DISABLE != S_wpcm450_fan_tach[fan_in].u8FanStatusFlag)
    {
        /* reset status */
        S_wpcm450_fan_tach[fan_in].u8FanStatusFlag = FAN_TACH_INIT;

        reg_int = ioread8(WPCM450_TIEN(mft));
        
        if (WPCM450_CMPA == cmp)
        {
            /* Enable interrupt */
            iowrite8((UINT8) (reg_int | (WPCM450_TIEN_TAIEN | WPCM450_TIEN_TEIEN))
                     , WPCM450_TIEN(mft));

            reg_mode =  WPCM450_TCKC_C1CSEL(WPCM450_MFT_APB_CLOCK_MODE) 
                        | ioread8(WPCM450_TCKC(mft));
            
            /* start to Capture */
            iowrite8(reg_mode, WPCM450_TCKC(mft));
        }
        else
        {
            /* Enable interrupt */
            iowrite8((UINT8) (reg_int | (WPCM450_TIEN_TBIEN | WPCM450_TIEN_TFIEN)),
                     WPCM450_TIEN(mft));
            
            reg_mode =  WPCM450_TCKC_C2CSEL(WPCM450_MFT_APB_CLOCK_MODE) 
                        | ioread8(WPCM450_TCKC(mft));
            
            /* start to Capture */
            iowrite8(reg_mode, WPCM450_TCKC(mft));
        }
    }
}

static void wpcm450_fantach_polling_func(unsigned long data)
{
    wpcm450_fan_tach_next(WPCM450_MFT0, WPCM450_CMPA);
    wpcm450_fan_tach_next(WPCM450_MFT0, WPCM450_CMPB);
    wpcm450_fan_tach_next(WPCM450_MFT1, WPCM450_CMPA);
    wpcm450_fan_tach_next(WPCM450_MFT1, WPCM450_CMPB);

    #if 0
    PDEBUG("\nEnter wpcm450_fantach_polling_func [%d][%d][%d][%d]\n",
        S_wpcm450_fan_tach_select[WPCM450_MFT0][WPCM450_CMPA],
         S_wpcm450_fan_tach_select[WPCM450_MFT0][WPCM450_CMPB],
          S_wpcm450_fan_tach_select[WPCM450_MFT1][WPCM450_CMPA],
           S_wpcm450_fan_tach_select[WPCM450_MFT1][WPCM450_CMPB]);
    #endif

    /* clear the flag and reset the counter (TCNT) */
    iowrite8((UINT8) WPCM450_TICLR_CLEAR_ALL, (void *) WPCM450_TICLR(WPCM450_MFT0));
    iowrite8((UINT8) WPCM450_TICLR_CLEAR_ALL, (void *) WPCM450_TICLR(WPCM450_MFT1));
    
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT1(WPCM450_MFT0));
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT2(WPCM450_MFT0));
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT1(WPCM450_MFT1));
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT2(WPCM450_MFT1));

    wpcm450_fan_tach_start_polling(WPCM450_MFT0, WPCM450_CMPA);
    wpcm450_fan_tach_start_polling(WPCM450_MFT0, WPCM450_CMPB);
    wpcm450_fan_tach_start_polling(WPCM450_MFT1, WPCM450_CMPA);
    wpcm450_fan_tach_start_polling(WPCM450_MFT1, WPCM450_CMPB);

    /* reset the timer interval */
    wpcm450_fantach_timer.expires = jiffies + FAN_TACH_POLLING_INTERVAL;
    add_timer(&wpcm450_fantach_timer);

    return;
}

static int aess_fansensor_open(struct inode *inode, struct file *filp)
{
		
    if (S_u8init_flag == AESSFANSENSOR_NOT_INIT)
    {   
        PDEBUG("KN: Open the Fan Sensor Device!!!\n");

        S_u8init_flag = AESSFANSENSOR_INIT_OK;
    }
    
    return (STATUS_OK);
}


static int aess_fantach_config(sFanTachData *pFanTachData)
{
    UINT8 fan_tach_chnl = 0;
    UINT32 tempreg = 0;;
    
    fan_tach_chnl = pFanTachData->u8ChannelNum;

    /* Enable FanTach Input PIN */
    tempreg = ioread32(WPCM450_MFSEL2);
    iowrite32((UINT32) (tempreg | WPCM450_MFSEL2_FLSEL(fan_tach_chnl)), WPCM450_MFSEL2);
    
    S_wpcm450_fan_tach[fan_tach_chnl].u8FanStatusFlag = FAN_TACH_INIT;
    S_wpcm450_fan_tach[fan_tach_chnl].u8FanPulsePerRev = pFanTachData->u8FanPulsePerRev;
    S_wpcm450_fan_tach_fail_cnt[fan_tach_chnl] = 0;

    return  0;
}

static int aess_fantach_read(sFanTachData *pFanTachData)
{
    UINT8 fan_in = 0;

    fan_in = pFanTachData->u8ChannelNum;

    if (S_wpcm450_fan_tach[fan_in].u16FanTachCnt != 0)
    {
        pFanTachData->u16FanSpeedReading 
            = FAN_TACH_TO_RPM(S_wpcm450_fan_tach[fan_in].u16FanTachCnt, 
                              S_wpcm450_fan_tach[fan_in].u8FanPulsePerRev);
                              
        PDEBUG2("fan %d speed %d rpm \n", fan_in, pFanTachData->u16FanSpeedReading);
    }
    else
    {
        pFanTachData->u16FanSpeedReading = 0;
    }
    
    return  0;
}

static int aess_fansensor_ioctl(struct inode * inode, struct file *filp, 
						  unsigned int cmd, unsigned long arg)
{
    UINT8 fan_tach_chnl = 0;
    int err_check = 0;

    sFanTachData *pFanTachData = (sFanTachData *) arg;

    /* Check the structure address is OK */
    if(!access_ok(VERIFY_READ, (void *) pFanTachData, sizeof(sFanTachData)))
    {
    PDEBUG("KN:struct access error!!\n");

    /* error occur */
    return -EFAULT;
    }

    if(!access_ok(VERIFY_WRITE, (void *) pFanTachData, sizeof(sFanTachData)))
    {
    PDEBUG("KN:struct access error!!\n");

    /* error occur */
    return -EFAULT;
    }

    fan_tach_chnl = pFanTachData->u8ChannelNum;     

    if (fan_tach_chnl >= WPCM450_MAX_FAN_TACH)
    {
        return -ENODEV;
    }

    switch(cmd)
    {   
        case AESS_FANTACH_CONFIG:       
            err_check = aess_fantach_config(pFanTachData);  
            break;

        case AESS_FANTACH_READ:       
            err_check = aess_fantach_read(pFanTachData);  
            break;

        default:
            PDEBUG(KERN_ERR "KN:aess_fansensor_ioctl, command error!!! \n");
            return -ENOIOCTLCMD;
    }

    PDEBUG("KN:IO control Error no is %d!!\n", err_check);

    /* 0->ok, minus->fail */
    return err_check;
}

static int aess_fansensor_release(struct inode *inode, struct file *filp)
{
    /* Disable all interrupt */
    iowrite8((UINT8) 0x00, WPCM450_TIEN(WPCM450_MFT0));
    iowrite8((UINT8) 0x00, WPCM450_TIEN(WPCM450_MFT1));
    
	return (STATUS_OK);
}

struct file_operations aess_fansensor_fops = {
	.open = aess_fansensor_open, 
	.ioctl = aess_fansensor_ioctl,
	.release = aess_fansensor_release,
};

static inline void wpcm450_fan_tach_compute(UINT8 mft, UINT8 cmp, UINT8 fan_in, UINT8 flag_int, UINT8 flag_mode, UINT8 flag_clear)
{
    UINT8 reg_int = 0;
    UINT8 reg_mode = 0;
    UINT16 fan_cap= 0;

    if (WPCM450_CMPA == cmp)
    {
        fan_cap = ioread16((void *) WPCM450_TCRA(mft));
    }
    else
    {
        fan_cap = ioread16((void *) WPCM450_TCRB(mft));
    }

    /* clear capature flag, H/W will auto reset the WPCM450_TCNTx */
    iowrite8((UINT8) flag_clear, (void *) WPCM450_TICLR(mft));

    if (FAN_TACH_INIT == S_wpcm450_fan_tach[fan_in].u8FanStatusFlag)
    {
        /* First capture, drop it */
        S_wpcm450_fan_tach[fan_in].u8FanStatusFlag 
            = FAN_TACH_PREPARE_TO_GET_FIRST_CAPTURE;

        /* reset counter */
        S_wpcm450_fan_tach[fan_in].u32FanTachCntTemp = 0;
    }
    else if (fan_cap > FAN_TACH_CNT_LIMIT)
    {
    	S_wpcm450_fan_tach[fan_in].u8FanStatusFlag = FAN_TACH_INIT;
    	
        PDEBUG2("0 fan %d S_wpcm450_fan_tach_fail_cnt %d cnt %d \n", 
        	fan_in, S_wpcm450_fan_tach_fail_cnt[fan_in], (WPCM450_MFT_TCNT - fan_cap));
    	S_wpcm450_fan_tach_fail_cnt[fan_in]++;
    	
	}
    else if (S_wpcm450_fan_tach[fan_in].u8FanStatusFlag < FAN_TACH_ENOUGH_SAMPLE)
    {
        /* collect the enough sampe, (ex: 2 pulse fan need to get 2 sample) */
        S_wpcm450_fan_tach[fan_in].u32FanTachCntTemp += (WPCM450_MFT_TCNT - fan_cap);
        
        PDEBUG2("\n=========================\n");
        PDEBUG2("1 fan %d cnt %d total %d \n", 
        	fan_in, (WPCM450_MFT_TCNT - fan_cap), S_wpcm450_fan_tach[fan_in].u32FanTachCntTemp);
        S_wpcm450_fan_tach[fan_in].u8FanStatusFlag++;
        
    }
    else  /* get enough sample or fan disable (u8FanStatusFlag = FAN_TACH_DISABLE) */
    {
        if (S_wpcm450_fan_tach[fan_in].u8FanStatusFlag == FAN_TACH_ENOUGH_SAMPLE)
        {
            S_wpcm450_fan_tach[fan_in].u32FanTachCntTemp += (WPCM450_MFT_TCNT - fan_cap);

        	PDEBUG2("2 fan %d cnt %d total %d \n", 
        		fan_in, (WPCM450_MFT_TCNT - fan_cap), 
        		S_wpcm450_fan_tach[fan_in].u32FanTachCntTemp);

            /* compute finial average cnt per pulse */
            S_wpcm450_fan_tach[fan_in].u16FanTachCnt 
                = S_wpcm450_fan_tach[fan_in].u32FanTachCntTemp / FAN_TACH_ENOUGH_SAMPLE;
            
        	PDEBUG2("3 fan %d avg %d S_wpcm450_fan_tach_fail_cnt = %d \n\n", 
        		fan_in, S_wpcm450_fan_tach[fan_in].u16FanTachCnt, 
        		S_wpcm450_fan_tach_fail_cnt[fan_in]);
            S_wpcm450_fan_tach_fail_cnt[fan_in] = 0;
            
            
            S_wpcm450_fan_tach[fan_in].u8FanStatusFlag = FAN_TACH_INIT;
        }

        reg_int =  ioread8(WPCM450_TIEN(mft));

        /* Disable interrupt */
        iowrite8((UINT8) (reg_int & ~flag_int), WPCM450_TIEN(mft));

        reg_mode =  ioread8(WPCM450_TCKC(mft));

        /* stop to Capture */
        iowrite8((UINT8) (reg_mode & ~flag_mode), (void *) WPCM450_TCKC(mft));
    }

    return;
}

static inline void wpcm450_check_cmp(UINT8 mft, UINT8 cmp, UINT8 flag)
{
    UINT8 reg_int = 0;
    UINT8 reg_mode = 0;
    UINT8 flag_timeout;
    UINT8 flag_cap;
    UINT8 flag_clear;
    UINT8 flag_int;
    UINT8 flag_mode;
    UINT8 fan_in;

    /* FAN TACH Compute timeout */
    fan_in = WPCM450_FAN_TACH_INUPT(mft, cmp);

    if (WPCM450_CMPA == cmp)
    {
        flag_cap = WPCM450_TICTRL_TAPND;
        flag_timeout = WPCM450_TICTRL_TEPND;
        flag_int = (WPCM450_TIEN_TAIEN | WPCM450_TIEN_TEIEN);
        flag_mode = WPCM450_TCKC_C1CSEL(WPCM450_MFT_APB_CLOCK_MODE);
        flag_clear = WPCM450_TICLR_TACLR;
    }
    else
    {
        flag_cap = WPCM450_TICTRL_TBPND;
        flag_timeout = WPCM450_TICTRL_TFPND;
        flag_int = (WPCM450_TIEN_TBIEN | WPCM450_TIEN_TFIEN);
        flag_mode = WPCM450_TCKC_C2CSEL(WPCM450_MFT_APB_CLOCK_MODE);
        flag_clear = WPCM450_TICLR_TBCLR;
    }

    if (flag & flag_timeout)
    {
        reg_int =  ioread8(WPCM450_TIEN(mft));
        
        /* Disable interrupt */
        iowrite8((UINT8) (reg_int & ~flag_int), WPCM450_TIEN(mft));

        reg_mode =  ioread8(WPCM450_TCKC(mft));

        /* stop to Capture */
        iowrite8((UINT8) (reg_mode & ~flag_mode), (void *) WPCM450_TCKC(mft));

        /* If timeout occurs (FAN_TACH_TIMEOUT), the fan doesn't connect 
           or speed is lower than 10.6Hz (320RPM/pulse2). 
           In these situation, the RPM output should be zero.  */
        S_wpcm450_fan_tach[fan_in].u16FanTachCnt = 0;
    }
    else
    {
        /* Has Input Capture */
        if (flag & flag_cap)
        {
            wpcm450_fan_tach_compute(mft, cmp, fan_in, flag_int, flag_mode, flag_clear);
        }
    }
}


static irqreturn_t wpcm450_mft0_isr(int irq, void *dev_id, struct pt_regs * regs)
{
    UINT8 flag = 0;
    
    flag = ioread8((void *) WPCM450_TICTRL(WPCM450_MFT0));

    wpcm450_check_cmp(WPCM450_MFT0, WPCM450_CMPA, flag);
    wpcm450_check_cmp(WPCM450_MFT0, WPCM450_CMPB, flag);

    return IRQ_HANDLED;
    
}

static irqreturn_t wpcm450_mft1_isr(int irq, void *dev_id, struct pt_regs * regs)
{
    UINT8 flag = 0;

    flag = ioread8((void *) WPCM450_TICTRL(WPCM450_MFT1));

    //printk("[%d]wpcm450_mft1_isr flag=0x%lx cap=%ld\n", WPCM450_FAN_TACH_INUPT(WPCM450_MFT1, WPCM450_CMPA), flag,  ioread16(WPCM450_TCRA(WPCM450_MFT1)));
    wpcm450_check_cmp(WPCM450_MFT1, WPCM450_CMPA, flag);
    wpcm450_check_cmp(WPCM450_MFT1, WPCM450_CMPB, flag);

   return IRQ_HANDLED;
    
}

static int wpcm450_fan_tach_init(void)
{
    int i;

    /* wpcm450 fan tach initialization */

    /* MFTFEN (MFT Freeze disable) */
    //iowrite32((UINT32)0x32C, 0xF000103C);

    /* Stop MFT0/MFT1 Clock */
    iowrite8((UINT8) WPCM450_MFT_NO_CLOCK_MODE, WPCM450_TCKC(WPCM450_MFT0));
    iowrite8((UINT8) WPCM450_MFT_NO_CLOCK_MODE, WPCM450_TCKC(WPCM450_MFT1));

    /* Disable all interrupt */
    iowrite8((UINT8) 0x00, WPCM450_TIEN(WPCM450_MFT0));
    iowrite8((UINT8) 0x00, WPCM450_TIEN(WPCM450_MFT1));

    /* Clear all interrupt */
    iowrite8((UINT8) WPCM450_TICLR_CLEAR_ALL, WPCM450_TICLR(WPCM450_MFT0));
    iowrite8((UINT8) WPCM450_TICLR_CLEAR_ALL, WPCM450_TICLR(WPCM450_MFT1));
    
    /* Set MFT0/MFT1 Clock Prescaler */
    iowrite8((UINT8) WPCM450_MFT_CLKPS, WPCM450_TPRSC(WPCM450_MFT0));
    iowrite8((UINT8) WPCM450_MFT_CLKPS, WPCM450_TPRSC(WPCM450_MFT1));

    /* Set MFT0/MFT1 Mode (high-to-low Transition) */
    iowrite8((UINT8) (WPCM450_TMCTRL_MDSEL(WPCM450_MFT_MODE_5)
                      | WPCM450_TMCTRL_TBEN | WPCM450_TMCTRL_TAEN),
             (void *) WPCM450_TMCTRL(WPCM450_MFT0));
    iowrite8((UINT8) (WPCM450_TMCTRL_MDSEL(WPCM450_MFT_MODE_5)
                      | WPCM450_TMCTRL_TBEN | WPCM450_TMCTRL_TAEN),
             (void *) WPCM450_TMCTRL(WPCM450_MFT1));

    /* Set MFT0/MFT1 Initial Count/Cap */
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT1(WPCM450_MFT0));
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT2(WPCM450_MFT0));
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT1(WPCM450_MFT1));
    iowrite16(WPCM450_MFT_TCNT, WPCM450_TCNT2(WPCM450_MFT1));

    /* Set MFT0/MFT1 Compare (equal to count) */
    iowrite8((UINT8) (WPCM450_TCPCFG_EQAEN | WPCM450_TCPCFG_EQBEN),
             WPCM450_TCPCFG(WPCM450_MFT0));
    iowrite8((UINT8) (WPCM450_TCPCFG_EQAEN | WPCM450_TCPCFG_EQBEN),
             WPCM450_TCPCFG(WPCM450_MFT1));

    /* Set MFT0/MFT1 Compare value */
    iowrite16(WPCM450_MFT_TCPA, WPCM450_TCPA(WPCM450_MFT0));
    iowrite16(WPCM450_MFT_TCPA, WPCM450_TCPA(WPCM450_MFT1));
    iowrite16(WPCM450_MFT_TCPB, WPCM450_TCPB(WPCM450_MFT0));
    iowrite16(WPCM450_MFT_TCPB, WPCM450_TCPB(WPCM450_MFT1));

    /* Set MFT0/MFT1 Fan Input FanIN 3,7,11,15*/
    iowrite8((UINT8) WPCM450_TINASEL_FANIN3, WPCM450_TINASEL(WPCM450_MFT0));
    iowrite8((UINT8) WPCM450_TINBSEL_FANIN7, WPCM450_TINBSEL(WPCM450_MFT0));

    iowrite8((UINT8) WPCM450_TINASEL_FANIN11, WPCM450_TINASEL(WPCM450_MFT1));
    iowrite8((UINT8) WPCM450_TINBSEL_FANIN15, WPCM450_TINBSEL(WPCM450_MFT1));

    /* Fan tach structure initialization*/
    S_wpcm450_fan_tach_select[WPCM450_MFT0][WPCM450_CMPA] = WPCM450_TINASEL_FANIN0;
    S_wpcm450_fan_tach_select[WPCM450_MFT0][WPCM450_CMPB] = WPCM450_TINBSEL_FANIN4;
    S_wpcm450_fan_tach_select[WPCM450_MFT1][WPCM450_CMPA] = WPCM450_TINASEL_FANIN8;
    S_wpcm450_fan_tach_select[WPCM450_MFT1][WPCM450_CMPB] = WPCM450_TINBSEL_FANIN12;

    for (i = 0; i < WPCM450_MAX_FAN_TACH; i++)
    {
        S_wpcm450_fan_tach[i].u8FanStatusFlag = FAN_TACH_DISABLE;
        S_wpcm450_fan_tach[i].u8FanPulsePerRev = DEFAULT_PULSE_PER_REVOLUTION;
        S_wpcm450_fan_tach[i].u16FanTachCnt = 0;
    }

    /* Request IRQ */
    if(request_irq(IRQ_MFT0, (irq_handler_t)&wpcm450_mft0_isr, IRQF_DISABLED|IRQF_SAMPLE_RANDOM, 
                   "WPCM450-MFT0", (void *) &u8dummy))
    {
        PDEBUG(KERN_ERR "WPCM450 : register irq MFT0 failed\n");
        return -EAGAIN;
    }
    if(request_irq(IRQ_MFT1, (irq_handler_t)&wpcm450_mft1_isr, IRQF_DISABLED|IRQF_SAMPLE_RANDOM,
                   "WPCM450-MFT1", (void *) &u8dummy))
    {
        PDEBUG(KERN_ERR "WPCM450 : register irq MFT1 failed\n");
        
        free_irq(IRQ_MFT0, (void *) &u8dummy);
        return -EAGAIN;
    }   

    /* Initialize Fan Tach Polling Timer */
    wpcm450_fantach_timer.data = 0;
    wpcm450_fantach_timer.function = &wpcm450_fantach_polling_func;

    /* Set Timer Interval */
    wpcm450_fantach_timer.expires = jiffies + FAN_TACH_POLLING_INTERVAL;

    init_timer(&wpcm450_fantach_timer);
    add_timer(&wpcm450_fantach_timer);

    return 0;
}


int __init aess_fansensor_init(void)
{
    int result;

    PDEBUG("Init_aess_fansensor_module Sucess!!! \n");

    fansensor_cdev = cdev_alloc();

    fansensor_cdev->ops = &aess_fansensor_fops;
    result = alloc_chrdev_region(&dev, 0, 1, driver_name);

    printk("mknod /dev/aess_fansensordrv c %d 0\n", MAJOR(dev));

    if (result < 0) {
        PDEBUG (KERN_ERR "KN:Registering the character device failed with %d\n", MAJOR(dev));
        return result;
    }

    cdev_add(fansensor_cdev, dev, 1);

    result = wpcm450_fan_tach_init();
    if (result < 0)
        return result;

#if 0

    /* TEST CODE */
    {
        int i=0, j=0;
        sFanTachData FanTachData;

        for(i=0;i<WPCM450_MAX_FAN_TACH;i++)
        {
            FanTachData.u8ChannelNum = i;
            FanTachData.u8FanPulsePerRev = 2;
            FanTachData.u16FanSpeedReading = 0;

            aess_fantach_config(&FanTachData);
        }

        aess_fansensor_open(NULL,NULL);

        i=0;
       while(i< 20)
        {
            for(j=0;j<WPCM450_MAX_FAN_TACH;j++)
            {
                FanTachData.u8ChannelNum = j;
                aess_fantach_read(&FanTachData);
                #if 1
                printk("[%ld][%02ld] status=%d, PULSE=%ld CNT=%4ld, CNTTEMP=%5ld FAN RPM=%5ld HZ=%3ld\n", 
                    i, j, S_wpcm450_fan_tach[j].u8FanStatusFlag, 
                    S_wpcm450_fan_tach[j].u8FanPulsePerRev, 
                    S_wpcm450_fan_tach[j].u16FanTachCnt, 
                    S_wpcm450_fan_tach[j].u32FanTachCntTemp,
                    FanTachData.u16FanSpeedReading,FanTachData.u16FanSpeedReading/30);

                #else
                printk("[%d][%d] FAN RPM=%ld\n", 
                    i, j, FanTachData.u16FanSpeedReading);
                #endif

            }

            printk("\n");
            
            mdelay(1000);
            i++;
        }
    }
#endif
   
	return (STATUS_OK);
}

void __exit aess_fansensor_exit(void)
{
    cdev_del(fansensor_cdev);
    unregister_chrdev_region(dev,1);
    PDEBUG ("KN:aess_fansensor_exit \n");
    
    /* Stop MFT0/MFT1 Clock */
    iowrite8((UINT8) WPCM450_MFT_NO_CLOCK_MODE, WPCM450_TCKC(WPCM450_MFT0));
    iowrite8((UINT8) WPCM450_MFT_NO_CLOCK_MODE, WPCM450_TCKC(WPCM450_MFT1));

    /* Disable all interrupt */
    iowrite8((UINT8) 0x00, WPCM450_TIEN(WPCM450_MFT0));
    iowrite8((UINT8) 0x00, WPCM450_TIEN(WPCM450_MFT1));
    
    /* Request IRQ */
    free_irq(IRQ_MFT0, (void *) &u8dummy);
    
    free_irq(IRQ_MFT1, (void *) &u8dummy);

    /* Delete Fan Tach Polling Timer */
    del_timer(&wpcm450_fantach_timer);

    return;
}

MODULE_DESCRIPTION("AESS Fan Sensor Driver");
MODULE_AUTHOR("Watt Wu <watt.wu@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_fansensor_init);
module_exit(aess_fansensor_exit);
