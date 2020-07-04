/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_platform.h
  版 本 号   : 初稿
  生成日期   : D2014_03_09

******************************************************************************/

#ifndef __HI_MAP_H__
#define __HI_MAP_H__

//
#define HI_REG_BASE_SCTL                   0x10100000
#define HI_REG_IOSIZE_SCTL                 0x1000

#define HI_REG_BASE_DDRC                   0x10102000
#define HI_REG_IOSIZE_DDRC                 0x1000

#define HI_REG_BASE_TIMER01                0x10104000
#define HI_REG_IOSIZE_TIMER01              0x1000
#define HI_REG_BASE_TIMER23                0x10105000
#define HI_REG_IOSIZE_TIMER23              0x1000

#define HI_CFG_TIMER0_VABASE	           hi_io_address(HI_REG_BASE_TIMER01)
#define HI_CFG_TIMER2_VABASE	           hi_io_address(HI_REG_BASE_TIMER23)

#define HI_REG_BASE_GPIO0                  0x10106000
#define HI_REG_BASE_GPIO1                  0x10107000
#define HI_REG_BASE_GPIO2                  0x10108000
#define HI_REG_BASE_GPIO3                  0x10109000
#define HI_REG_BASE_GPIO4                  0x1010A000
#define HI_REG_BASE_GPIO5                  0x1010B000
#define HI_REG_BASE_GPIO6                  0x1010C000
#define HI_REG_BASE_GPIO7                  0x1010D000
#define HI_REG_IOSIZE_GPIO                 0x100

#define HI_REG_BASE_UART0                  0x1010E000
#define HI_REG_IOSIZE_UART0                0x1000
#define HI_REG_BASE_UART1                  0x1010F000
#define HI_REG_IOSIZE_UART1                0x1000

#define HI_REG_BASE_A9_PERI                0x10180000
#define HI_REG_IOSIZE_A9_PERI              0x2000

#define HI_REG_BASE_PCIE0                  0x10A00000    
#define HI_REG_IOSIZE_PCIE0                0x10000    
#define HI_REG_BASE_PCIE1                  0x10A10000
#define HI_REG_IOSIZE_PCIE1                0x10000    

#define HI_REG_BASE_SFC                    0x10A20000            
#define HI_REG_BASE_NANDC                  0x10A30000

#define HI_REG_IO_BASE_XHCI                0x10A50000
#define HI_REG_IO_SIZE_XHCI                0x00010000
#define HI_REG_IRQ_NUM_XHCI                71

#define HI_REG_BASE_EHCI                   0x10A40000
#define HI_REG_IOSIZE_EHCI                 0x1000
#define HI_REG_BASE_OHCI                   0x10A50000
#define HI_REG_IOSIZE_OHCI                 0x1000

#define HI_REG_BASE_MMC_SD_SDIO            0x10A60000

#define HI_REG_BASE_CRG                    0x14880000
#define HI_REG_IOSIZE_CRG                  0x1000

#define HI_REG_BASE_IOMUX                  0x14900000
#define HI_REG_IOSIZE_MUX                  0x1000

#define HI_REG_BASE_L2CACHE                0x16800000    
#define HI_REG_IOSIZE_L2CACHE              0x1000    

/*CORTTX-A9 MPCORE MEMORY REGION*/
#define HI_REG_A9_PERI_SCU                 0x0000
#define HI_REG_A9_PERI_GIC_CPU             0x0100
#define HI_REG_A9_PERI_GLOBAL_TIMER        0x0200
#define HI_REG_A9_PERI_PRI_TIMER_WDT       0x0600
#define HI_REG_A9_PERI_GIC_DIST            0x1000 
#define HI_REG_SC_LOCAL_TIMER              0x600

#define HI_REG_SC_PLLCTRL4                 0x0074
#define HI_REG_SC_PLLFCTRL4                0x0078
#define HI_REG_SC_PERCTRL44                0x014C
#define HI_REG_SC_SC_TESTREG1              0x0130

#define HI_REG_TIMER_RELOAD                0x000
#define HI_REG_TIMER_VALUE                 0x004
#define HI_REG_TIMER_CONTROL               0x008
#define HI_REG_TIMER_INTCLR                0x00C
#define HI_REG_TIMER_RIS                   0x010
#define HI_REG_TIMER_MIS                   0x014
#define HI_REG_TIMER_BGLOAD                0x018

#define HI_REG_TIMER1_RELOAD               0x020
#define HI_REG_TIMER1_VALUE                0x024
#define HI_REG_TIMER1_CONTROL              0x028
#define HI_REG_TIMER1_INTCLR               0x02C
#define HI_REG_TIMER1_RIS                  0x030
#define HI_REG_TIMER1_MIS                  0x034
#define HI_REG_TIMER1_BGLOAD               0x038

#define HI_DDR_SUSPEND_SIZE                0x1000      /* size 1KB */

#define HI_GIC_CPU_BASE                    hi_io_address(HI_REG_BASE_A9_PERI + HI_REG_A9_PERI_GIC_CPU)
#define HI_GIC_DIST_BASE                   hi_io_address(HI_REG_BASE_A9_PERI + HI_REG_A9_PERI_GIC_DIST)

#define HI_TIMER_ENABLE                     ( 1 << 7 )
#define HI_TIMER_PERIODIC                   ( 1 << 6 )
#define HI_TIMER_INTMASK                    ( 1 << 5 )
#define HI_TIMER_32BIT	                    ( 1 << 1 )
#define HI_TIMER_ONESHOT                    ( 1 << 0 )
#define HI_TIMER_CLK_SOURCE                 ( (1<<22) | (1<<20) | (1 << 18) | (1 << 16) )

#define HI_TIMER_CONTROL                    ( HI_TIMER_ENABLE | HI_TIMER_PERIODIC | HI_TIMER_INTMASK | HI_TIMER_32BIT )
#define HI_TIMER_ONE_CONTROL	            ( HI_TIMER_ENABLE | HI_TIMER_ONESHOT  | HI_TIMER_INTMASK | HI_TIMER_32BIT )

#define HI_BUSCLK_TO_TIMER_RELOAD       	(CONFIG_HSAN_BUSCLK/HZ)
#define HI_BUSCLK_TO_TIMER_CLK_HZ           (CONFIG_HSAN_BUSCLK)
#define HI_BUSCLK_TO_TIMER_CLK_KHZ          (CONFIG_HSAN_BUSCLK/1000)

#endif /* __HI_MAP_H__ */
