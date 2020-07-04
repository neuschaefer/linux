/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_comdef.h
  版 本 号   : 初稿
  生成日期   : D2011_07_29
  最近修改   :

******************************************************************************/
#ifndef __HI_COMDEF_H__
#define __HI_COMDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_NULL               0

#define HI_DISABLE            0
#define HI_ENABLE             1

#define HI_FALSE              0
#define HI_TRUE               1

#define HI_HZ                 HZ
#define HI_FILE_S             FILE
#define HI_DIR_S              DIR

#define HI_MAX(a, b)       ( ((a) > (b)) ? (a) : (b) )
#define HI_MIN(a, b)       ( ((a) < (b)) ? (a) : (b) )

#define HI_FUNCNAME_MAXLEN    128
#define HI_NAME_MAXLEN        16

typedef enum
{
    HI_FALSE_E  = 0   , 
    HI_TRUE_E     
}hi_bool_e;

/* 用于某些硬件特性的电平配置 */
typedef enum
{
    HI_LOW_LEVEL_E = 0 , 
    HI_HIGH_LEVEL_E       
}hi_level_e;

/* 用于某些硬件特性的信号触发沿配置 */
typedef enum
{
    HI_UP_EDGE_E = 0,       /* 上升沿 */
    HI_DOWN_EDGE_E,         /* 下降沿 */        
} hi_edge_e;

/*定义的变量，用于设置一个bit为1或0*/
#define HI_BIT31_1 (0x00000001 << 31)
#define HI_BIT30_1 (0x00000001 << 30)
#define HI_BIT29_1 (0x00000001 << 29)
#define HI_BIT28_1 (0x00000001 << 28)
#define HI_BIT27_1 (0x00000001 << 27)
#define HI_BIT26_1 (0x00000001 << 26)
#define HI_BIT25_1 (0x00000001 << 25)
#define HI_BIT24_1 (0x00000001 << 24)
#define HI_BIT23_1 (0x00000001 << 23)
#define HI_BIT22_1 (0x00000001 << 22)
#define HI_BIT21_1 (0x00000001 << 21)
#define HI_BIT20_1 (0x00000001 << 20)
#define HI_BIT19_1 (0x00000001 << 19)
#define HI_BIT18_1 (0x00000001 << 18)
#define HI_BIT17_1 (0x00000001 << 17)
#define HI_BIT16_1 (0x00000001 << 16)
#define HI_BIT15_1 (0x00000001 << 15)
#define HI_BIT14_1 (0x00000001 << 14)
#define HI_BIT13_1 (0x00000001 << 13)
#define HI_BIT12_1 (0x00000001 << 12)
#define HI_BIT11_1 (0x00000001 << 11)
#define HI_BIT10_1 (0x00000001 << 10)
#define HI_BIT09_1 (0x00000001 << 9)
#define HI_BIT08_1 (0x00000001 << 8)
#define HI_BIT07_1 (0x00000001 << 7)
#define HI_BIT06_1 (0x00000001 << 6)
#define HI_BIT05_1 (0x00000001 << 5)
#define HI_BIT04_1 (0x00000001 << 4)
#define HI_BIT03_1 (0x00000001 << 3)
#define HI_BIT02_1 (0x00000001 << 2)
#define HI_BIT01_1 (0x00000001 << 1)
#define HI_BIT00_1 (0x00000001)

#define HI_BIT31_0 (~HI_BIT31_1)
#define HI_BIT30_0 (~HI_BIT30_1)
#define HI_BIT29_0 (~HI_BIT29_1)
#define HI_BIT28_0 (~HI_BIT28_1)
#define HI_BIT27_0 (~HI_BIT27_1)
#define HI_BIT26_0 (~HI_BIT26_1)
#define HI_BIT25_0 (~HI_BIT25_1)
#define HI_BIT24_0 (~HI_BIT24_1)
#define HI_BIT23_0 (~HI_BIT23_1)
#define HI_BIT22_0 (~HI_BIT22_1)
#define HI_BIT21_0 (~HI_BIT21_1)
#define HI_BIT20_0 (~HI_BIT20_1)
#define HI_BIT19_0 (~HI_BIT19_1)
#define HI_BIT18_0 (~HI_BIT18_1)
#define HI_BIT17_0 (~HI_BIT17_1)
#define HI_BIT16_0 (~HI_BIT16_1)
#define HI_BIT15_0 (~HI_BIT15_1)
#define HI_BIT14_0 (~HI_BIT14_1)
#define HI_BIT13_0 (~HI_BIT13_1)
#define HI_BIT12_0 (~HI_BIT12_1)
#define HI_BIT11_0 (~HI_BIT11_1)
#define HI_BIT10_0 (~HI_BIT10_1)
#define HI_BIT09_0 (~HI_BIT09_1)
#define HI_BIT08_0 (~HI_BIT08_1)
#define HI_BIT07_0 (~HI_BIT07_1)
#define HI_BIT06_0 (~HI_BIT06_1)
#define HI_BIT05_0 (~HI_BIT05_1)
#define HI_BIT04_0 (~HI_BIT04_1)
#define HI_BIT03_0 (~HI_BIT03_1)
#define HI_BIT02_0 (~HI_BIT02_1)
#define HI_BIT01_0 (~HI_BIT01_1)
#define HI_BIT00_0 (~HI_BIT00_1)

/*定义的变量，用于设置bit掩码*/
#define HI_BITMASK_LOW01 0x00000001
#define HI_BITMASK_LOW02 0x00000003
#define HI_BITMASK_LOW03 0x00000007
#define HI_BITMASK_LOW04 0x0000000f
#define HI_BITMASK_LOW05 0x0000001f
#define HI_BITMASK_LOW06 0x0000003f
#define HI_BITMASK_LOW07 0x0000007f
#define HI_BITMASK_LOW08 0x000000ff
#define HI_BITMASK_LOW09 0x000001ff
#define HI_BITMASK_LOW10 0x000003ff
#define HI_BITMASK_LOW11 0x000007ff
#define HI_BITMASK_LOW12 0x00000fff
#define HI_BITMASK_LOW16 0x0000ffff
#define HI_BITMASK_LOW18 0x0003ffff
#define HI_BITMASK_LOW20 0x000fffff
#define HI_BITMASK_LOW22 0x003fffff
#define HI_BITMASK_LOW24 0x00ffffff
#define HI_BITMASK_LOW28 0x0fffffff
#define HI_BITMASK_LOW30 0x3fffffff
#define HI_BITMASK_LOW32 0xffffffff

/* 数字宏定义 */
#define HI_NUM_0                0
#define HI_NUM_1                1
#define HI_NUM_2                2
#define HI_NUM_3                3
#define HI_NUM_4                4
#define HI_NUM_5                5
#define HI_NUM_6                6
#define HI_NUM_7                7
#define HI_NUM_8                8
#define HI_NUM_9                9
#define HI_NUM_10               10
#define HI_NUM_11               11
#define HI_NUM_12               12
#define HI_NUM_16               16
#define HI_NUM_20               20
#define HI_NUM_30               30
#define HI_NUM_32               32
#define HI_NUM_50               50
#define HI_NUM_64               64
#define HI_NUM_100              100
#define HI_NUM_127              127
#define HI_NUM_128              128
#define HI_NUM_244              244
#define HI_NUM_256              256
#define HI_NUM_500              500
#define HI_NUM_512              512
#define HI_NUM_1000             1000
#define HI_NUM_2000             2000
#define HI_NUM_10000            10000

#define HI_BIT(ui_bit) (1 << ( ui_bit ))
#define HI_GET_BIT(ui_data,ui_bit) ( ( (ui_data) >>(ui_bit) ) & 1 )

/* 获取成员在结构体中的偏移 */
#define HI_MEMBER_OFFSET(type, mem) ((hi_uint32)&((type *)0)->mem)

/* 4字节对齐返回需要补齐的余量*/
#define HI_BYTE_ALIGN_VALUE(value)  ((value&HI_NUM_3) ? (HI_NUM_4 - (value&HI_NUM_3) ) : (0) )
#define HI_BYTE_ALIGN_OFFSET(ptr)   ((hi_uint32)(ptr) & 0x3)
#define HI_BYTE_ALIGN(ptr)          ((hi_uint32)(ptr) & (~0x3))

/*检查入口指针*/
#define HI_INVALID_PTR(ptr)         if( HI_NULL == ptr ) return HI_RET_NULLPTR;

#define HI_ARRAY_SIZE(a)            (sizeof(a)/sizeof(a[0]))

#define HI_INVALID_VLAN_ID          0xFFF
/* 检查函数返回值 */
#define HI_CHECK_RET(ret)\
    do\
    {\
        if(HI_RET_SUCC != ret)\
        {\
            return ret;\
        }\
    }while(0)


/* 注册的函数指针类型定义 */
typedef hi_int32 (* HI_FUNCCALLBACK_EXT)( hi_void *pv_data,hi_uint32 ui_inlen,hi_uint32 *pui_outlen);
typedef hi_int32 (* HI_FUNCCALLBACK)( hi_void *pv_data, hi_uint32 ui_inlen );

// 魔术字
#define HI_MAGICAL                      0x68776877

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_COMDEF_H__ */
