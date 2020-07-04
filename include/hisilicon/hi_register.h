/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_register.h
  功能描述: 头文件
  版本描述: V1.0

  创建日期: D2012_09_27

  修改记录: 
            生成初稿.
******************************************************************************/

#ifndef __HI_REGISTER_H__
#define __HI_REGISTER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_REG_WRITE(addr, val)         (*(volatile hi_uint32*)(addr) = (val))
#define HI_REG_READ(addr, val)          ((val) = *(volatile hi_uint32*)(addr))

/* val bit 为 1 被 设置为 1 */
#define HI_REG_RW_SET_MASK(addr, val, mask)  { \
        hi_uint32 ui_data=0; \
        HI_REG_READ(addr, ui_data); \
        HI_REG_WRITE(addr, (ui_data&(~(mask)))|(val)); \
    }

/* val bit 为 1 被 设置为 1 */
#define HI_REG_RW_SET(addr, val)  { \
        hi_uint32 ui_data=0; \
        HI_REG_READ(addr, ui_data); \
        HI_REG_WRITE(addr, (ui_data|(val))); \
    }

/* val bit 为 1 被 设置为 0 */
#define HI_REG_RW_CLR(addr, val)  { \
        hi_uint32 ui_data=0; \
        HI_REG_READ(addr, ui_data); \
        HI_REG_WRITE(addr, ui_data&(~(val))); \
    }

/* bit 0..31 为 1 被 设置为 1 */
#define HI_REG_RW_SET_BIT(addr, bit)  { \
        hi_uint32 ui_data=0; \
        HI_REG_READ(addr, ui_data); \
        HI_REG_WRITE(addr, ui_data|(1<<(bit))); \
    }

/* bit 0..31 为 1 被 设置为 0 */
#define HI_REG_RW_CLR_BIT(addr, bit)  { \
        hi_uint32 ui_data=0; \
        HI_REG_READ(addr, ui_data); \
        HI_REG_WRITE(addr, ui_data&(~(1<<(bit)))); \
    }

/* 1:单BIT 操作 */
#define HI_BIT_GET(reg,bit)                ((0 ==  ((reg) & (bit)))  ? 0:1)
#define HI_BIT_SET(reg,bit)                ((reg) |= (bit)) 
#define HI_BIT_CLR(reg,bit)                ((reg) &= (~(bit))) 


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_REGISTER_H__ */

