/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : io.h
  版 本 号   : 初稿
  生成日期   : D2014_03_11

******************************************************************************/
#ifndef __IO_H__
#define __IO_H__

#define IO_SPACE_LIMIT 0xffffffff

#define __io(a)         __typesafe_io(a)
#define __mem_pci(a)    (a)

#define IO_ADDRESS(x)   (x + 0xE8000000)
#define __io_address(n) __io(IO_ADDRESS(n))


#endif /* __IO_H__ */

