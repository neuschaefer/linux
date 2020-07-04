/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

 ******************************************************************************
  �� �� ��   : io.h
  �� �� ��   : ����
  ��������   : D2014_03_11

******************************************************************************/
#ifndef __IO_H__
#define __IO_H__

#define IO_SPACE_LIMIT 0xffffffff

#define __io(a)         __typesafe_io(a)
#define __mem_pci(a)    (a)

#define IO_ADDRESS(x)   (x + 0xE8000000)
#define __io_address(n) __io(IO_ADDRESS(n))


#endif /* __IO_H__ */

