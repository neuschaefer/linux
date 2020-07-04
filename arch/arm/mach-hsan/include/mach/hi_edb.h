/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

 ******************************************************************************
  �� �� ��   : hi_edb.h
  �� �� ��   : ����
  ��������   : D2014_03_10

******************************************************************************/

#ifndef __HI_EDB_H__
#define __HI_EDB_H__

#ifdef CONFIG_HSAN_EDB
#define hi_debug(fmt,arg...)        { \
                                       hi_printk( "\r\n%s(%d):",__func__,__LINE__); \
                                       hi_printk( (hi_char8 *)fmt, ##arg ); \
                                    }
#define hi_trace()                  { \
                                       hi_printk( "\r\n%s(%d):",__func__,__LINE__); \
                                    }
#else
#define hi_debug(fmt,arg...)
#define hi_trace()
#endif

#endif /* __HI_EDB_H__ */

