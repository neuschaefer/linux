/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_print.h
  版 本 号   : 初稿
  生成日期   : D2011_11_08

******************************************************************************/
#ifndef __HI_PRINT_H__
#define __HI_PRINT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define HI_PRINT_MEM( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uchar8) ) \
        {  if ( 0 == (i%16)) hi_printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_printk("%02x ",(*(hi_uchar8*)(ptr_tmp+i))); } hi_printk("\r\n"); \
    }

#define HI_PRINT_MEM_USHORT( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_ushort16) ) \
        {  if ( 0 == (i%16)) hi_printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_printk("%04x ",(*(hi_ushort16*)(ptr_tmp+i))); } hi_printk("\r\n"); \
    }
#define HI_PRINT_MEM_UINT( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uint32) ) \
        {  if ( 0 == (i%16)) hi_printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_printk("%08x ",(*(hi_uint32*)(ptr_tmp+i))); } hi_printk("\r\n"); \
    }
#define HI_PRINT_MEM_USHORT_HTON( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_ushort16) ) \
        {  if ( 0 == (i%16)) hi_printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_printk("%04x ",HI_HTONS(*(hi_ushort16*)(ptr_tmp+i))); } hi_printk("\r\n"); \
    }
#define HI_PRINT_MEM_UINT_HTON( len, ptr )    { \
    hi_uint32 i; hi_uchar8 *ptr_tmp = (hi_uchar8*)ptr; \
        for ( i=0; i < len; i+=sizeof(hi_uint32) ) \
        {  if ( 0 == (i%16)) hi_printk("\r\n[0x%08x] : ",i+(hi_uint32)ptr); hi_printk("%08x ",HI_NTOHL(*(hi_uint32*)(ptr_tmp+i))); } hi_printk("\r\n"); \
    }

#define HI_PRINT_MEM_DES( len, ptr,fmt,arg...) do{\
                                               hi_printk((hi_uchar8 *)fmt, ##arg);  \
                                               HI_PRINT_MEM_UINT_HTON((hi_uint32)(len),(hi_uchar8 *)(ptr));\
                                               }while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_PRINT_H__ */
