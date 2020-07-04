/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : irqs.h
  版 本 号   : 初稿
  生成日期   : D2014_03_09

******************************************************************************/
#ifndef __IRQS_H__
#define __IRQS_H__

#define HI_GIC_IRQ_START                    32	
#define HI_INTNR_COMMTX0					HI_GIC_IRQ_START+32	
#define HI_INTNR_COMMRX0					HI_GIC_IRQ_START+33
#define HI_INTNR_L2CACHE_INT_COMB			HI_GIC_IRQ_START+34	
#define HI_INTNR_L2CACHE_CHK0_INT			HI_GIC_IRQ_START+35	
#define HI_INTNR_L2CACHE_CHK1_INT			HI_GIC_IRQ_START+36
#define HI_INTNR_ZSP	                    HI_GIC_IRQ_START+37
#define HI_INTNR_USB_EHCI	                HI_GIC_IRQ_START+38
#define HI_INTNR_USB_OHCI	                HI_GIC_IRQ_START+39
#define HI_INTNR_SFC				        HI_GIC_IRQ_START+40
#define HI_INTNR_NANDC_S				    HI_GIC_IRQ_START+41
#define HI_INTNR_SSP					    HI_GIC_IRQ_START+42
#define HI_INTNR_I2C_S					    HI_GIC_IRQ_START+43
#define HI_INTNR_I2C_M					    HI_GIC_IRQ_START+44
#define HI_INTNR_UART0			            HI_GIC_IRQ_START+45
#define HI_INTNR_UART1			            HI_GIC_IRQ_START+46
#define HI_INTNR_TIMER_0_1			        HI_GIC_IRQ_START+47	/* Timer 0 and 1 */
#define HI_INTNR_TIMER_2_3                	HI_GIC_IRQ_START+48	/* Timer 2 and 3 */
#define HI_INTNR_GPIO_0                     HI_GIC_IRQ_START+49	/* GPIO 0 */
#define HI_INTNR_GPIO_1                     HI_GIC_IRQ_START+50	/* GPIO 1 */
#define HI_INTNR_GPIO_2	                    HI_GIC_IRQ_START+51	/* GPIO 2 */
#define HI_INTNR_GPIO_3                     HI_GIC_IRQ_START+52	/* GPIO 3 */
#define HI_INTNR_GPIO_4                     HI_GIC_IRQ_START+53	/* GPIO 4 */
#define HI_INTNR_GPIO_5	                    HI_GIC_IRQ_START+54	/* GPIO 5 */
#define HI_INTNR_GPIO_6                     HI_GIC_IRQ_START+55	/* GPIO 6 */
#define HI_INTNR_GPIO_7                     HI_GIC_IRQ_START+56	/* GPIO 7 */
#define HI_INTNR_SDIO				        HI_GIC_IRQ_START+57
#define HI_INTNR_DMAC	                    HI_GIC_IRQ_START+58
#define HI_INTNR_PCIE0_A	                HI_GIC_IRQ_START+59
#define HI_INTNR_PCIE0_B	                HI_GIC_IRQ_START+60
#define HI_INTNR_PCIE0_C	                HI_GIC_IRQ_START+61
#define HI_INTNR_PCIE0_D	                HI_GIC_IRQ_START+62
#define HI_INTNR_PCIE1_A	                HI_GIC_IRQ_START+63
#define HI_INTNR_PCIE1_B	                HI_GIC_IRQ_START+64
#define HI_INTNR_PCIE1_C	                HI_GIC_IRQ_START+65
#define HI_INTNR_PCIE1_D	                HI_GIC_IRQ_START+66
#define HI_INTNR_PMU                        HI_GIC_IRQ_START+67
#define HI_INTNR_SCI                        HI_GIC_IRQ_START+68
#define HI_INTNR_PCIE0_LINK_DOWN	        HI_GIC_IRQ_START+69
#define HI_INTNR_PCIE1_LINK_DOWN	        HI_GIC_IRQ_START+70
#define HI_INTNR_PCIE0_PM	                HI_GIC_IRQ_START+71
#define HI_INTNR_PCIE1_PM	                HI_GIC_IRQ_START+72

#define HI_MAX_GIC_NR                       1
#define HI_IRQ_LOCALTIMER                   29

#define NR_IRQS		     				    HI_GIC_IRQ_START+128  //128

#endif /* __IRQS_H__ */
