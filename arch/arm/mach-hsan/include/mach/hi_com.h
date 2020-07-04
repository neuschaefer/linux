/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_com.h
  版 本 号   : 初稿
  生成日期   : D2014_03_26

******************************************************************************/
#ifndef __HI_COM_H__
#define __HI_COM_H__

typedef enum
{
    HI_CHIP_ID_H_E = 1,
    HI_CHIP_ID_T_E,
    HI_CHIP_ID_NONE_E,
} HI_CHIP_ID_E;

typedef enum
{
    HI_BOOT_ROM = 0x0,
    HI_BOOT_SPIFLASH,
    HI_BOOT_NANDFLASH,
}HI_BOOT_DEV_E;

/******************************************************************************/
static inline hi_uint32 hi_get_apbclk(hi_void)
{
	return CONFIG_HSAN_BUSCLK; 
}

extern HI_CHIP_ID_E hi_get_chip_id(hi_void);
extern HI_BOOT_DEV_E hi_get_boot_sel(hi_void);
extern hi_void hi_dumpall(hi_void);
extern hi_void arch_reset(hi_char8 mode,const hi_char8 *cmd);
extern hi_int32 hi_irq_set_affinity(hi_uint32 ui_irq, hi_uint32 ui_cpu);
extern hi_void hi_acp_init(hi_void);
extern hi_void hi_l2cache_init(hi_void);

#endif /* __HI_COM_H__ */
