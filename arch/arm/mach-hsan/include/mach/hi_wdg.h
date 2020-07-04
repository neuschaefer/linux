/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: watchdog.h
  功能描述: 头文件
  版本描述: V1.0

  创建日期: D2011_09_30

  修改记录: 
            生成初稿.
******************************************************************************/
#ifndef __HI_WATCHDOG_H__
#define __HI_WATCHDOG_H__

//狗复位时间极值
#define HI_WDG_MIN_INTERVAL             1
#define HI_WDG_MAX_INTERVAL             512

/* 默认喂狗时间以及狗复位时间 */
#define HI_WDG_DFT_FEED_TIME            3
#define HI_WDG_DFT_RST_TIME             30
#define HI_WDG_CHECK_MAX_TIME           ((HI_WDG_DFT_RST_TIME-HI_WDG_DFT_FEED_TIME)*HZ)

#define HI_WDG_MONITOR_CPUID            1

/*crg ---- crg整体寄存器结构*/
typedef struct
{
    volatile hi_uint32 ui_crg_sc_sysstat;               /* 0x0 - 软复位控制寄存器 */
    volatile hi_uint32 ui_crg_ecsplll_ctrl0;            /* 0x4 - ECS PLL控制寄存器 */
    volatile hi_uint32 ui_crg_ecsplll_ctrl1;            /* 0x8 - ECS PLL频率控制寄存器 */
    volatile hi_uint32 ui_crg_ethplll_ctrl0;            /* 0xc - ETH PLL控制寄存器 */
    volatile hi_uint32 ui_crg_ethplll_ctrl1;            /* 0x10 - ETH PLL频率控制寄存器 */
    volatile hi_uint32 ui_crg_sc_peren0;                /* 0x14 - 外设时钟使能寄存器0 */
    volatile hi_uint32 ui_crg_sc_perdis0;               /* 0x18 - 外设时钟禁止寄存器0 */
    volatile hi_uint32 ui_crg_sc_perclkst0;             /* 0x1c - 外设时钟使能状态寄存器 */
    volatile hi_uint32 ui_crg_sc_peren1;                /* 0x20 - 外设时钟使能寄存器1 */
    volatile hi_uint32 ui_crg_sc_perdis1;               /* 0x24 - 外设时钟禁止寄存器1 */
    volatile hi_uint32 ui_crg_sc_perclkst1;             /* 0x28 - 外设时钟使能状态寄存器1 */
    volatile hi_uint32 ui_crg_sc_rst_ctrl0;             /* 0x2c - 外设复位控制寄存器0 */
    volatile hi_uint32 ui_crg_sc_rst_ctrl1;             /* 0x30 - 外设复位控制寄存器1 */
    volatile hi_uint32 ui_crg_sc_rst_ctrl2;             /* 0x34 - 外设复位控制寄存器2 */
    volatile hi_uint32 ui_crg_cpu_cfg;                  /* 0x38 - CPU控制寄存器 */
    volatile hi_uint32 ui_crg_hw_cfg;                   /* 0x3c - HW控制寄存器 */
    volatile hi_uint32 ui_crg_sc_perctrl0;              /* 0x40 - 外设控制寄存器0 */
    volatile hi_uint32 ui_crg_sc_perctrl1;              /* 0x44 - 外设控制寄存器1 */
    volatile hi_uint32 ui_crg_sc_perctrl2;              /* 0x48 - 外设控制寄存器2 */
    volatile hi_uint32 ui_crg_reserved_1[1];            /* 0x4c - 保留 */
    volatile hi_uint32 ui_crg_sc_perctrl3;              /* 0x50 - 外设控制寄存器3 */
    volatile hi_uint32 ui_crg_sc_perctrl4;              /* 0x54 - 外设控制寄存器4 */
    volatile hi_uint32 ui_crg_sc_perctrl5;              /* 0x58 - 外设控制寄存器5 */
    volatile hi_uint32 ui_crg_sc_perctrl6;              /* 0x5c - 外设控制寄存器6 */
    volatile hi_uint32 ui_crg_sc_perctrl7;              /* 0x60 - 外设控制寄存器7 */
    volatile hi_uint32 ui_crg_sc_perctrl8;              /* 0x64 - 外设控制寄存器8 (硬件看门狗使能控制)*/
    volatile hi_uint32 ui_crg_ram_ctrl_bus;             /* 0x68 - RAM控制寄存器 */
    volatile hi_uint32 ui_crg_wdg_init_cfg;             /* 0x6c - 看门狗初始化配置信号 */
    volatile hi_uint32 ui_crg_osc_cfg;                  /* 0x70 - 晶振配置信号 */
    volatile hi_uint32 ui_crg_reserved_2[7];            /* 0x74:8c - 保留 */
    volatile hi_uint32 ui_crg_crg_stat0;                /* 0x90 - CRG状态寄存器0 */
    volatile hi_uint32 ui_crg_dying_gasp_pre_int;       /* 0x94 - DYING_GASP中断寄存器 */
    volatile hi_uint32 ui_crg_dying_gasp_int_mask;      /* 0x98 - DYING_GASP中断掩码寄存器 */
    volatile hi_uint32 ui_crg_dying_gasp_int_insert;    /* 0x9c - DYING_GASP中断插入寄存器 */
    volatile hi_uint32 ui_crg_reserved_3[24];           /* 0xa0:fc - 保留 */
    volatile hi_uint32 ui_crg_sc_chip_id;               /* 0x100 - 芯片ID寄存器 */
    volatile hi_uint32 ui_crg_crg_rev0;                 /* 0x104 - ECO预留寄存器0 */
}hi_crg_reg_s;

/*
  * SC_PERCTRL4 reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_sfc_freq_sel           : 2   ; // [1..0] SFC接口的工作频率选择
        hi_uint32 ui_ddr_clk_freq           : 1   ; // [2] 初始上电时，DDRC工作时钟频率选择
        hi_uint32 ui_Reserved_0             : 1   ; // [3]  保留
        hi_uint32 ui_ecs_ddr_freq           : 2   ; // [5..4] A9和DDRC的时钟比例关系配置
        hi_uint32 ui_Reserved_1             : 1   ; // [6]  保留
        hi_uint32 ui_ddrc_aclk_freq_sel     : 1   ; // [7] DDRC工作时钟频率和DDRPHY的时钟比例选择
        hi_uint32 ui_Reserved_2             : 2   ; // [9..8]  保留
        hi_uint32 ui_wdg_rst_time_cfg       : 9   ; // [18..10] 看门狗复位时间配置
        hi_uint32 ui_test_clk_ctrl          : 5   ; // [23..19] 测试时钟TEST_CLK输出，以及时钟检测模块检测时钟源选择控制信号
        hi_uint32 ui_det_clk_en             : 1   ; // [24] 时钟检测模块的检测使能信号
        hi_uint32 ui_Reserved_3             : 3   ; // [27..25]  保留
        hi_uint32 ui_fephy_clk_det_sel      : 2   ; // [29..28] FEPHY的参考时钟的测试输出（包括TX和RX)
        hi_uint32 ui_Reserved_4             : 2   ; // [31..30]  保留
    }st_bits;
} hi_crg_sc_perctrl4_u;

extern hi_void hi_wdg_check(hi_void);

#endif /*__HI_WATCHDOG_H__*/

