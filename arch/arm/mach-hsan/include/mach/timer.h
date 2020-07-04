/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef ASM_HSAN_TIMER
#define ASM_HSAN_TIMER


#define CFG_TIMER_VABASE	IO_ADDRESS(HI_REG_BASE_TIMER01)
#define CFG_TIMER2_VABASE	IO_ADDRESS(HI_REG_BASE_TIMER23)
#define CFG_TIMER_CONTROL	( CFG_TIMER_ENABLE | CFG_TIMER_PERIODIC | CFG_TIMER_INTMASK | CFG_TIMER_32BIT )
#define CFG_TIMER_ONE_CONTROL	( CFG_TIMER_ENABLE | CFG_TIMER_INTMASK | CFG_TIMER_32BIT | CFG_TIMER_ONESHOT )

#define CFG_TIMER_ENABLE    (1 << 7)
#define CFG_TIMER_PERIODIC  (1 << 6)
#define CFG_TIMER_INTMASK   (1 << 5)
#define CFG_TIMER_32BIT	    (1 << 1)
#define CFG_TIMER_ONESHOT   (1 << 0)

#define CFG_TIMER0_CLK_SOURCE	((1<<18) | (1 << 16))
#define CFG_TIMER1_CLK_SOURCE	((1<<22) | (1 << 20))

#define CFG_TIMER_PRESCALE	1
#define BUSCLK_TO_TIMER_RELOAD(busclk)	(((busclk)/CFG_TIMER_PRESCALE)/HZ)
#define BUSCLK_TO_TIMER0_CLK_HZ(busclk)  ((busclk)/CFG_TIMER_PRESCALE)
#define BUSCLK_TO_TIMER1_CLK_HZ(busclk)  ((busclk)/CFG_TIMER_PRESCALE)
#define BUSCLK_TO_TIMER0_CLK_KHZ(busclk) (((busclk)/CFG_TIMER_PRESCALE)/1000)
#define BUSCLK_TO_TIMER1_CLK_KHZ(busclk) (((busclk)/CFG_TIMER_PRESCALE)/1000)


#define HSAN_GIC_IRQ_START 32
#define INTNR_TIMER_0_1           HSAN_GIC_IRQ_START+47   /* Timer 0 and 1 */
#define INTNR_TIMER_2_3           HSAN_GIC_IRQ_START+48   /* Timer 2 and 3 */


extern unsigned long long sched_clock(void);
#endif
