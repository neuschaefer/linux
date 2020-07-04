/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: watchdog.c
  功能描述: 硬件看门狗驱动
  版本描述: V1.0

  创建日期: D2011_09_30

  修改记录: 
            生成初稿.
******************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach/irq.h>

#include <mach/timer.h>
#include <mach/hi_map.h>


static irqreturn_t  hi_kernel_breath_timer_interrupt(int irq, void *dev_id);
extern int sd5610x_gpio_bit_write(unsigned int gpio, unsigned int level);
static int hi_kernel_irq_bind_cpu(int ui_irq, int ui_cpu);

static struct irqaction hi_kernel_breath_timer_irq =
{
    .name       = "breath_hardware_timer",
    .flags      = IRQF_SHARED | IRQF_TIMER,
    .handler    = hi_kernel_breath_timer_interrupt,
};

/* Breath State */
typedef enum
{
    BREATH_LED_STATE_OFF = 0,
    BREATH_LED_STATE_ON,
    BREATH_LED_STATE_OFF_DELAY,
    BREATH_LED_STATE_ON_DELAY
} ATP_E_BREATH_STATE;

/* Breath status */
typedef enum
{
    BREATH_OFF = 0,
    BREATH_ON,
    BREATH_ONCE,
    BREATH_AMBER,
    BREATH_AMBER_ONCE,
    BREATH_COMPLETE_OFF  // stop breathing after breathing complete
} ATP_E_BREATH_STATUS;

#define BREATH_FREQUENCE    10000      //Breath Base time: 0.1ms
//#define BREATH_ON_DELAY     10000      //Breath on delay : 1s
//#define BREATH_OFF_DELAY    20000      //Breath off delay: 2s
#define KDUTYRATIOBASE      1          //Duty Ratio: dummy denominator
#define KDUTYMAXTHRESOLD   120        //Breathing Cycle = (breath * kDutyMaxThreshold)*(kDutyMaxThreshold / kDutyRatioBase)*2
#define KDUTYMINTHRESOLD   (KDUTYMAXTHRESOLD - KDUTYMAXTHRESOLD / KDUTYRATIOBASE)
//Breathing Cycle (on + off) = (breath * kDutyMaxThreshold)*(kDutyMaxThreshold / kDutyRatioBase)*2 = 0.1 * 120 * 120 * 2ms =1.44s *2

extern int breath_enable;
extern int breath_ledn;
extern int breath_on_delay;
extern int breath_off_delay;
extern int kDutyMaxThresold;

static int m_iBreathDirection = 1;
static int iCountDown = KDUTYMAXTHRESOLD;
static int iFlickerCount = KDUTYMAXTHRESOLD;
static int iPauseCount = 0;
static ATP_E_BREATH_STATE state = BREATH_LED_STATE_OFF;
static ATP_E_BREATH_STATUS breath_complete_off = BREATH_OFF;
static int gpio = 0;
static int gpios = 0;

/* 绑定硬中断到指定CPU */
static int hi_kernel_irq_bind_cpu(int ui_irq, int ui_cpu)
{
   struct cpumask mask;

   cpumask_clear(&mask);
   cpumask_set_cpu(ui_cpu, &mask);
   
   return irq_set_affinity(ui_irq, &mask);
}

void  hi_kernel_breath_timer_enable(int enable)
{
	static ATP_E_BREATH_STATUS breath_amber = BREATH_OFF;

	if(1 == enable)
	{
		m_iBreathDirection = 1;
	    iCountDown = kDutyMaxThresold;
		iFlickerCount = kDutyMaxThresold;
		iPauseCount = 0;
		state = BREATH_LED_STATE_OFF;	
		breath_complete_off = BREATH_OFF;
		gpio = breath_ledn;		
		writel(CFG_TIMER_CONTROL,   CFG_TIMER2_VABASE + HI_REG_TIMER1_CONTROL);
	}
	else
	{
		breath_amber = breath_enable;
		if ( BREATH_AMBER == breath_amber)  // if (gpios != 0) --gpios =0,only sigle led;gpios !=0 ,two leds;
		{
			sd5610x_gpio_bit_write(gpios, 1); //off  amber  breath
		}
		if (BREATH_COMPLETE_OFF == breath_amber && gpios != 0) //complete off  amber  breath
		{
			sd5610x_gpio_bit_write(gpios, 1); 
		}
		breath_enable = 0;
		writel(0,   				CFG_TIMER2_VABASE + HI_REG_TIMER1_CONTROL);		
	}
}
EXPORT_SYMBOL(hi_kernel_breath_timer_enable);

static irqreturn_t  hi_kernel_breath_timer_interrupt(int irq, void *dev_id)
{
    static ATP_E_BREATH_STATUS enable = BREATH_OFF;

	enable = breath_enable;

	if (BREATH_AMBER == enable	|| BREATH_AMBER_ONCE == enable)
	{
		gpios = gpio + 1;
	}
	
	if ( ( readl(CFG_TIMER2_VABASE+HI_REG_TIMER1_RIS) ) & 0x1 )
	{
		if (BREATH_ON == enable || BREATH_ONCE == enable || BREATH_AMBER == enable
			|| BREATH_AMBER_ONCE == enable || BREATH_COMPLETE_OFF == enable)
		{
			if(gpios > 76) // green + red breath = amber  breath
			{
				gpios = gpio - 2;
			}
			if (iCountDown-- == 0)
			{
				if (BREATH_LED_STATE_ON == state)
				{
					sd5610x_gpio_bit_write(gpio, 1);
					if(BREATH_AMBER == enable || BREATH_AMBER_ONCE == enable)
					{
						sd5610x_gpio_bit_write(gpios, 1);
					}
					state = BREATH_LED_STATE_OFF;
					iCountDown = iFlickerCount;
					iPauseCount += m_iBreathDirection;
					if (iPauseCount < 0)
					{
						iPauseCount = 0;
						m_iBreathDirection = -m_iBreathDirection;
						breath_complete_off = BREATH_COMPLETE_OFF; //breath complete
						if( BREATH_ONCE == enable || BREATH_AMBER_ONCE == enable
							|| BREATH_COMPLETE_OFF == enable) //breathonce
						{
							breath_enable = BREATH_OFF;
						}	
						else 
						{
							state = BREATH_LED_STATE_OFF_DELAY;
						}
					}
				}
				else if (BREATH_LED_STATE_ON_DELAY == state)
				{
					iCountDown = breath_on_delay;
					state = BREATH_LED_STATE_ON;
				}
				else if (BREATH_LED_STATE_OFF_DELAY == state)
				{
					iCountDown = breath_off_delay;
					state = BREATH_LED_STATE_OFF;					
				}
				else
				{
					if (BREATH_COMPLETE_OFF == enable &&
						BREATH_COMPLETE_OFF == breath_complete_off)
					{
						m_iBreathDirection = 1;
					    iCountDown = kDutyMaxThresold;
						iFlickerCount = kDutyMaxThresold;
						iPauseCount = 0;
						state = BREATH_LED_STATE_OFF;	
						breath_complete_off = BREATH_OFF;	
						breath_enable = BREATH_OFF;
					}
					else 
					{
						breath_complete_off = BREATH_OFF;
						sd5610x_gpio_bit_write(gpio, 0); //breath start :off -dim up -down
						if(BREATH_AMBER == enable || BREATH_AMBER_ONCE == enable)
						{
							sd5610x_gpio_bit_write(gpios, 0);
						}
						state = BREATH_LED_STATE_ON;
						iCountDown = iPauseCount;
						iFlickerCount -= m_iBreathDirection;
						if (iFlickerCount < KDUTYMINTHRESOLD)
						{
							iFlickerCount = KDUTYMINTHRESOLD;
							m_iBreathDirection = -m_iBreathDirection;
							state = BREATH_LED_STATE_ON_DELAY;
						}
					}
				}
			}
			//state = BREATH_LED_STATE_OFF;
		}		
		
		writel(~0, CFG_TIMER2_VABASE + HI_REG_TIMER1_INTCLR);

    }

    return IRQ_HANDLED;
    
}


void hi_kernel_breath_timer_init(void)
{
    writel(0,                                      CFG_TIMER2_VABASE + HI_REG_TIMER1_CONTROL);
    writel(BREATH_FREQUENCE,         CFG_TIMER2_VABASE + HI_REG_TIMER1_RELOAD );
    writel(BREATH_FREQUENCE,         CFG_TIMER2_VABASE + HI_REG_TIMER1_BGLOAD  ); //这里延后1秒，防止冲突
	writel(0,                        CFG_TIMER2_VABASE + HI_REG_TIMER1_CONTROL); //disable hi_kernel_breath_timer	

    setup_irq(INTNR_TIMER_2_3, &hi_kernel_breath_timer_irq);

    hi_kernel_irq_bind_cpu(INTNR_TIMER_2_3, 1);
    //setup_irq(INTNR_TIMER_2_3, &sd56xx_watchdog_cpu1_irq);     

    printk("hardware timer init sucessful!\n");

}

pure_initcall(hi_kernel_breath_timer_init);


