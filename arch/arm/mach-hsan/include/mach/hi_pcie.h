/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_pcie.h
  版 本 号   : 初稿
  生成日期   : D2014_03_09

******************************************************************************/
#ifndef __HI_PCIE_H__
#define __HI_PCIE_H__

typedef enum 
{
	HI_PCIE_CH0 = 0,
	HI_PCIE_CH1 = 1,
    HI_PCIE_NR  = 2,
}hi_pcie_channel_e;

typedef enum 
{
	HI_PCIE_INTA_PIN = 0,
	HI_PCIE_INTB_PIN = 1,
	HI_PCIE_INTC_PIN = 2,
	HI_PCIE_INTD_PIN = 3,
    HI_PCIE_INT_NR   = 4,
}hi_pcie_irq_pin_e;

#define HI_PCIE_IRQ_BASE		    (32)
#define HI_PCIE0_IRQ_INTA		    (59 + HI_PCIE_IRQ_BASE)
#define HI_PCIE0_IRQ_INTB		    (60 + HI_PCIE_IRQ_BASE)
#define HI_PCIE0_IRQ_INTC		    (61 + HI_PCIE_IRQ_BASE)
#define HI_PCIE0_IRQ_INTD		    (62 + HI_PCIE_IRQ_BASE)
                                    
#define HI_PCIE1_IRQ_INTA		    (63 + HI_PCIE_IRQ_BASE)
#define HI_PCIE1_IRQ_INTB		    (64 + HI_PCIE_IRQ_BASE)
#define HI_PCIE1_IRQ_INTC		    (65 + HI_PCIE_IRQ_BASE)
#define HI_PCIE1_IRQ_INTD		    (66 + HI_PCIE_IRQ_BASE)

#define HI_PCIE0_MEMIO_BASE	        (0x50000000)//配置空间
#define HI_PCIE0_DBI_BASE_ADDR      (0x10A00000)//IO空间 64k

#define HI_PCIE1_MEMIO_BASE	        (0x68000000) //配置空间
#define HI_PCIE1_DBI_BASE_ADDR		(0x10A02000) //IO空间 64k

#define HI_PCIE_BASE_ADDR_SIZE		(0x1000000)

#define HI_PCIE_IATU_TBL_CNT         3

typedef struct 
{
	hi_uint32            ui_viewport;          /* iATU Viewport Register        */
	hi_uint32            ui_region_ctrl_1;     /* Region Control 1 Register     */
	hi_uint32            ui_region_ctrl_2;     /* Region Control 2 Register     */
	hi_uint32            ui_lbar;              /* Lower Base Address Register   */
	hi_uint32            ui_ubar;              /* Upper Base Address Register   */
	hi_uint32            ui_lar;               /* Limit Address Register        */
	hi_uint32            ui_ltar;		       /* Lower Target Address Register */
	hi_uint32            ui_utar;              /* Upper Target Address Register */
}hi_pcie_iatu_s; 

typedef struct 
{
	hi_uint32            ui_stat0;
	hi_uint32            ui_stat4;
    hi_uint32            ui_perictrl0;
    hi_uint32            ui_perictrl1;
    hi_uint32            ui_clk_en;
    hi_uint32            ui_srst_n;
    hi_uint32            ui_conf_base;
    hi_uint32            ui_dbi_base;
    hi_uchar8            uc_irq_pin[HI_PCIE_INT_NR];
}hi_pcie_reg_info_s;

typedef struct 
{
	hi_int32             i_busnr;           /*root bus number*/
	
    hi_uchar8            uc_stat; 
    hi_uchar8            uc_stat_2_0; 
    hi_uchar8            uc_ch; 
    hi_uchar8            uc_rsv; 

    struct resource     *pst_io_space;
    struct resource     *pst_mem_space; 
    
	hi_iomem*            pv_conf_base;
    hi_uint32            ui_dbi_base; 
    
    hi_pcie_reg_info_s  *pst_reg;
    hi_pcie_iatu_s      *pst_iatu; 
}hi_pcie_info_s;

typedef struct
{
    hi_pcie_info_s          st_info[HI_PCIE_NR]; 
    
    hi_iomem*               pv_sys_base_virt;
    hi_iomem*               pv_crg_base_virt;
    hi_iomem*               pv_iomux_base_virt;
}hi_pcie_mng_data_s; 


#endif /* __HI_PCIE_H__ */
