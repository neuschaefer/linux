/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_common.c
  版 本 号   : 初稿
  生成日期   : D2014_03_10

******************************************************************************/

#include <mach/hi_hsan.h>

#define HI_CHIP_ID_H                     0x56100100
#define HI_CHIP_ID_T                     0x56102100
#define HI_CHIP_ID_MASK                  0xFFFFF000

HI_CHIP_ID_E hi_get_chip_id( hi_void )
{
    static hi_uint32 ui_chip  = HI_CHIP_ID_NONE_E;
    hi_uint32        ui_value = 0;

    if ( HI_CHIP_ID_NONE_E != ui_chip )
    {
        return ui_chip;
    }
    
    HI_REG_READ( hi_io_address(HI_REG_BASE_SCTL)+0x0800, ui_value);    
    switch(ui_value & HI_CHIP_ID_MASK)
    {
        case (HI_CHIP_ID_H & HI_CHIP_ID_MASK):
            ui_chip  = HI_CHIP_ID_H_E;    
            break;            
        case (HI_CHIP_ID_T & HI_CHIP_ID_MASK):
            ui_chip  = HI_CHIP_ID_T_E;
            break;
        default:
            ui_chip  = HI_CHIP_ID_NONE_E;
            break;
    }
    
    hi_debug("value:0x%x chip:%d",ui_value,ui_chip);
    return ui_chip;
}

hi_int32 hi_irq_set_affinity(hi_uint32 ui_irq, hi_uint32 ui_cpu)
{
    struct cpumask st_mask;

    cpumask_clear(&st_mask);
    cpumask_set_cpu(ui_cpu, &st_mask);

    hi_debug("irq:%d cpu:%d",ui_irq,ui_cpu);
    return irq_set_affinity(ui_irq, &st_mask);
}

HI_BOOT_DEV_E hi_get_boot_sel(hi_void)
{
    static hi_uint32 ui_flag  = 0;
    static hi_uint32 ui_value = 0;

    if ( 0 != ui_flag )
    {
        return (HI_BOOT_DEV_E)(ui_value&0x3);
    }
    ui_flag++;
    HI_REG_READ(hi_io_address(HI_REG_BASE_SCTL)+0x8, ui_value);

    hi_debug("value:0x%x ret:%d",ui_value,(ui_value&0x3));
    return (HI_BOOT_DEV_E)(ui_value&0x3);
}

EXPORT_SYMBOL(hi_irq_set_affinity);
EXPORT_SYMBOL(hi_get_boot_sel);
EXPORT_SYMBOL(hi_get_chip_id);

#ifdef __OVERLOAD__
#endif

hi_void arch_idle(hi_void)
{
    cpu_do_idle();
}

hi_void arch_reset(hi_char8 mode,const hi_char8 *cmd)
{   
    HI_CHIP_ID_E ui_chipid = hi_get_chip_id();

    hi_printk("\nSystem Reboot(%s)!!!",cmd);

    // close sfc sel
    HI_REG_WRITE( hi_io_address(HI_REG_BASE_CRG) + 30, 0          );
    HI_REG_WRITE( hi_io_address(HI_REG_BASE_CRG) + 18, 0xfffffff6 );
    switch ( ui_chipid )
    {
        case HI_CHIP_ID_H_E :
            /* 0x14880000先写入0x56100100,然后写入0XA9EF_FEFF.触发软复位 */
            HI_REG_WRITE( hi_io_address(HI_REG_BASE_CRG), 0x56100100);
            HI_REG_WRITE( hi_io_address(HI_REG_BASE_CRG), 0XA9EFFEFF);
            break;
        case HI_CHIP_ID_T_E :
            /* 0x14880000先写入0x56102100,然后写入0XA9EF_DEFF.触发软复位 */
            HI_REG_WRITE( hi_io_address(HI_REG_BASE_CRG), 0x56102100);
            HI_REG_WRITE( hi_io_address(HI_REG_BASE_CRG), 0XA9EFDEFF);
            break;
        default:
            break;
    }
    return;
}

hi_int32 clk_enable(struct clk *pst_clk)
{
	return 0;
}

hi_void clk_disable(struct clk *pst_clk)
{
}

hi_ulong32 clk_get_rate(struct clk *pst_clk)
{
}


