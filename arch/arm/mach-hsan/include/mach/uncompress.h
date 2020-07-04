/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : uncompress.h
  版 本 号   : 初稿
  生成日期   : D2014_03_13

******************************************************************************/

#ifndef __UNCOMPRESS_H__
#define __UNCOMPRESS_H__

#define AMBA_UART_DR	(*(volatile unsigned int *)0x1010F000)
#define AMBA_UART_TFL	(*(volatile unsigned int *)0x1010F07C)
#define AMBA_UART_FR	(*(volatile unsigned int *)0x1010F014)

static inline void putc(int c)
{
	while ((AMBA_UART_FR & 0x20) != 0x20)
		barrier();

	AMBA_UART_DR = c;
}

static inline void flush(void)
{
	while (AMBA_UART_TFL & 0x1)
		barrier();
}

#define arch_decomp_setup()

#endif /* __UNCOMPRESS_H__ */