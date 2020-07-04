/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_typedef.h
  功能描述   : hi_typedef.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : D2011_07_19
    修改内容   : 创建文件

******************************************************************************/
#ifndef __HI_TYPEDEF_H__
#define __HI_TYPEDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*********************************************************/
#ifndef hi_void
#define   hi_void              void
#endif

/*********************************************************/

#ifndef hi_char8
#define   hi_char8             char
#endif

#ifndef hi_short16
#define   hi_short16           short
#endif

#ifndef hi_int32
#define   hi_int32             int
#endif

#ifndef hi_long64
#define   hi_long64            long long
#endif

/*********************************************************/

#ifndef hi_uchar8
#define   hi_uchar8            unsigned char
#endif

#ifndef hi_ushort16
#define   hi_ushort16          unsigned short
#endif

#ifndef hi_uint32
#define   hi_uint32            unsigned int
#endif

#ifndef hi_ulong32
#define hi_ulong32               unsigned long
#endif

#ifndef hi_ulong64
#define   hi_ulong64           unsigned long long
#endif

/*********************************************************/

#ifndef hi_v_u8
#define   hi_v_u8              volatile unsigned char         
#endif

#ifndef hi_v_u16
#define   hi_v_u16             volatile unsigned short         
#endif

#ifndef hi_v_u32
#define   hi_v_u32             volatile unsigned int         
#endif

#ifndef hi_v_u64
#define   hi_v_u64             volatile unsigned long long   
#endif

/*********************************************************/

#ifndef hi_handle
#define   hi_handle            unsigned int
#endif

/*********************************************************/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_TYPEDEF_H__ */
