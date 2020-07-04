/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_edb.h
  版 本 号   : 初稿
  生成日期   : D2014_03_10

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

