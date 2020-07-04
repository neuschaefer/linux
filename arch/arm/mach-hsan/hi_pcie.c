/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_pcie.c
  版 本 号   : 初稿
  生成日期   : D2014_03_13

******************************************************************************/

#include <mach/hi_hsan.h>
#include "atpconfig.h"

static hi_pcie_mng_data_s g_st_pcie_data; 

#ifdef __PRODUCT_DEFINE__
#endif

enum pcie_init_mode
{
    pcie_mode_a = 0,    //hg255s mode
    pcie_mode_b,        //home base2.0 mode
    pcie_mode_default	//for default mode
};

/* GPIO工作模式 */
typedef enum
{
    SD5610X_GPIO_MODE_IN = 0, /* 输入模式 */
    SD5610X_GPIO_MODE_OUT,    /* 输出模式 */
    SD5610X_GPIO_MODE_HW,     /* 特殊功能模式,硬件模式管脚复用 */
} sd5610x_gpio_mode_e;

/* GPIO输出电平 */
typedef enum
{
    SD5610X_GPIO_LEVEL_LOW = 0,
    SD5610X_GPIO_LEVEL_HIGH,
} sd5610x_gpio_level_e;

typedef struct {
    hi_uint32  pcie0_reset_gpio;
    hi_uint32  pcie1_reset_gpio;
} st_Pcie_Rest_GPIO;

st_Pcie_Rest_GPIO g_pcie_reset_gpio_arry[] = 
{
   { 13 , 12 }, 
   { 91 ,  0 },
   { 0  ,  0 }, 
};


extern void sd5610x_iomux_set_gpio_mode(unsigned int gpio);
extern int sd5610x_gpio_bit_write(unsigned int gpio, unsigned int level);
extern int sd5610x_gpio_bit_attr_set(unsigned int gpio, unsigned int mode);

#define WLAN_PCIE_MODE pcie_mode_b

static hi_void hi_pcie_reset_chip( hi_uchar8 uc_ch)
{
    hi_uint32 ui_val  = 0;
    hi_uint32 ui_chip = hi_get_chip_id();

    switch(ui_chip)
    {
        case HI_CHIP_ID_H_E:
            #if 0
            {
                hi_uchar8 uc_rstbit = 2; // pcie0 rst bit
                if ( 1 == uc_ch )
                {
                    uc_rstbit = 3;       // pcie1 rst bit
                }
                HI_REG_READ( g_st_pcie_data.pv_iomux_base_virt + 0x204, ui_val);
                ui_val &= ~( 1 << uc_rstbit );
                HI_REG_WRITE(g_st_pcie_data.pv_iomux_base_virt + 0x204, ui_val);
                
                mdelay(50);

                HI_REG_READ( g_st_pcie_data.pv_iomux_base_virt + 0x204, ui_val);
                ui_val |= ( 1 << uc_rstbit );
                HI_REG_WRITE(g_st_pcie_data.pv_iomux_base_virt + 0x204, ui_val);
            }
            #else
            {
                hi_iomem *pv_gpio   = (hi_iomem*)hi_io_address(HI_REG_BASE_GPIO2);
                hi_uchar8 uc_muxbit = 2; // pcie0 mux bit: GPIO78
                hi_uchar8 uc_rstbit = 14; // pcie0 gpio0 rst bit
                if ( 1 == uc_ch )
                {
                    uc_muxbit = 6;       // pcie1 mux bit: GPIO82
                    uc_rstbit = 18;
                }
                HI_REG_READ( g_st_pcie_data.pv_iomux_base_virt + 0x1A0, ui_val); 
                ui_val |= ( 1 << uc_muxbit );
                HI_REG_WRITE(g_st_pcie_data.pv_iomux_base_virt + 0x1A0, ui_val);

                // dir out
                HI_REG_READ( pv_gpio + 0x4, ui_val);
                ui_val |= ( 1 << uc_rstbit );
                HI_REG_WRITE(pv_gpio + 0x4, ui_val);

                // low
                HI_REG_READ( pv_gpio + 0x0, ui_val);
                ui_val &= ~( 1 << uc_rstbit );
                HI_REG_WRITE(pv_gpio + 0x0, ui_val);
                
                mdelay(500);

                // high
                HI_REG_READ( pv_gpio + 0x0, ui_val);
                ui_val |= ( 1 << uc_rstbit );
                HI_REG_WRITE(pv_gpio + 0x0, ui_val);
            }
            #endif
            break;            
        case HI_CHIP_ID_T_E:
            {
                if ( 0 == uc_ch )
                {

                    if ( 0 == g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie0_reset_gpio)
                    {
                        return;
                    }
                                    
                    sd5610x_iomux_set_gpio_mode(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie0_reset_gpio);
                    sd5610x_gpio_bit_attr_set(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie0_reset_gpio, SD5610X_GPIO_MODE_OUT);
                    sd5610x_gpio_bit_write(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie0_reset_gpio, SD5610X_GPIO_LEVEL_LOW);

                    mdelay(50);

                    sd5610x_gpio_bit_write(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie0_reset_gpio, SD5610X_GPIO_LEVEL_HIGH);
                    
                }
                else if ( 1 == uc_ch )
                {
                    if ( 0 == g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie1_reset_gpio)
                    {
                        return;
                    }
                    
                    sd5610x_iomux_set_gpio_mode(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie1_reset_gpio);
                    sd5610x_gpio_bit_attr_set(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie1_reset_gpio, SD5610X_GPIO_MODE_OUT);
                    sd5610x_gpio_bit_write(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie1_reset_gpio, SD5610X_GPIO_LEVEL_LOW);

                    mdelay(50);

                    sd5610x_gpio_bit_write(g_pcie_reset_gpio_arry[WLAN_PCIE_MODE].pcie1_reset_gpio, SD5610X_GPIO_LEVEL_HIGH);
                }                
            }
            break;
        default:
            break;
    }
    return;
}

static hi_pcie_reg_info_s g_st_reg_info[HI_PCIE_NR] =
{
    {
        .ui_stat0        = 0x18,                    // sysctl:pcie0_stat0
        .ui_stat4        = 0x38,                    // sysctl:pcie0_stat4
        .ui_perictrl0    = 0xBC,                    // sysctl:pcie0_perictrl0
        .ui_perictrl1    = 0x134,                   // sysctl:pcie0_perictrl12
        .ui_clk_en       = 6,                       // crg:sc_peren1(6bit)
        .ui_srst_n       = 23,                      // crg:sc_rst_ctrl1(23bit)
        .ui_conf_base    = HI_PCIE0_MEMIO_BASE,
        .ui_dbi_base     = HI_PCIE0_DBI_BASE_ADDR,
        .uc_irq_pin      = { HI_PCIE0_IRQ_INTA,HI_PCIE0_IRQ_INTB,HI_PCIE0_IRQ_INTC,HI_PCIE0_IRQ_INTD },
    },
    {
        .ui_stat0        = 0x28,                    // SYSCTL:PCIE1_STAT0
        .ui_stat4        = 0x3C,                    // SYSCTL:PCIE1_STAT4
        .ui_perictrl0    = 0xEC,                    // SYSCTL:PCIE1_PERICTRL0
        .ui_perictrl1    = 0x140,                   // SYSCTL:PCIE1_PERICTRL12
        .ui_clk_en       = 7,                       // CRG:SC_PEREN1(7bit)
        .ui_srst_n       = 24,                      // CRG:SC_RST_CTRL1(24bit)
        .ui_conf_base    = HI_PCIE1_MEMIO_BASE,
        .ui_dbi_base     = HI_PCIE1_DBI_BASE_ADDR,
        .uc_irq_pin      = { HI_PCIE1_IRQ_INTA,HI_PCIE1_IRQ_INTB,HI_PCIE1_IRQ_INTC,HI_PCIE1_IRQ_INTD },
    },
};

static hi_pcie_iatu_s g_st_iatu_table[HI_PCIE_NR][HI_PCIE_IATU_TBL_CNT] = 
{
    // channel 0
    {
        {
            /* 配置操作的iATU寄存器初始化 */
    		.ui_viewport	    = 0x0,          //view index
    		.ui_region_ctrl_1	= 0x4,          //ctrl 1
    		.ui_region_ctrl_2	= 0x80000000,   //ctrl 2
    		.ui_lbar		    = 0x50000000,   //base lower
    		.ui_ubar		    = 0x0,          //base upper
    		.ui_lar	            = 0x57FFFFFF,   //limit
    		.ui_ltar		    = 0x01000000,   //target lower
    		.ui_utar		    = 0x0,          //target upper
    	},
        {
            /* 配置操作的iATU寄存器初始化 */
    		.ui_viewport	    = 0x1,          //view index
    		.ui_region_ctrl_1	= 0x2,          //ctrl 1 io 64k
    		.ui_region_ctrl_2	= 0x80000000,   //ctrl 2
    		.ui_lbar		    = 0x40400000,   //base lower
    		.ui_ubar		    = 0x0,          //base upper
    		.ui_lar	            = 0x404000ff,   //limit
    		.ui_ltar		    = 0x40400000,   //target lower
    		.ui_utar		    = 0x0,          //target upper
    	},
        {
            /* 配置操作的iATU寄存器初始化 */
    		.ui_viewport	    = 0x2,          //view index
    		.ui_region_ctrl_1	= 0x0,          //ctrl 1 mem 0x400000
    		.ui_region_ctrl_2	= 0x80000000,   //ctrl 2
    		.ui_lbar		    = 0x40000000,   //base lower
    		.ui_ubar		    = 0x0,          //base upper
    		.ui_lar             = 0x403FFFFF,   //limit
    		.ui_ltar		    = 0x40000000,   //target lower
    		.ui_utar		    = 0x0,          //target upper
    	},
    },
    // channel 1
    {
        {
            /* 配置操作的iATU寄存器初始化 */
    		.ui_viewport	    = 0x0,          //view index
    		.ui_region_ctrl_1	= 0x4,          //ctrl 1
    		.ui_region_ctrl_2	= 0x80000000,   //ctrl 2
    		.ui_lbar		    = 0x68000000,   //base lower
    		.ui_ubar		    = 0x0,          //base upper
    		.ui_lar             = 0x68FFFFFF,   //limit
    		.ui_ltar		    = 0x01000000,   //target lower
    		.ui_utar		    = 0x0,          //target upper
    	},
        {
            /* 配置操作的iATU寄存器初始化 外设配置空间 */
    		.ui_viewport	    = 0x1,          //view index
    		.ui_region_ctrl_1	= 0x2,          //ctrl 1 io 64k
    		.ui_region_ctrl_2	= 0x80000000,   //ctrl 2
    		.ui_lbar		    = 0x58400000,   //base lower
    		.ui_ubar		    = 0x0,          //base upper
    		.ui_lar             = 0x584000ff,   //limit
    		.ui_ltar		    = 0x58400000,   //target lower
    		.ui_utar		    = 0x0,          //target upper
    	},
        {
            /* 配置操作的iATU寄存器初始化 io操作 外设内部寄存器*/ 
    		.ui_viewport	    = 0x2,          //view index
    		.ui_region_ctrl_1	= 0x0,          //ctrl 1 mem 0x400000
    		.ui_region_ctrl_2	= 0x80000000,   //ctrl 2
    		.ui_lbar		    = 0x58000000,   //base lower
    		.ui_ubar		    = 0x0,          //base upper
    		.ui_lar             = 0x583FFFFF,   //limit
    		.ui_ltar		    = 0x58000000,   //target lower
    		.ui_utar		    = 0x0,          //target upper
    	},
    }
};

static struct resource g_st_mem_space[HI_PCIE_NR] = 
{
    {
    	.name   = "PCIE:0 memory space",
    	.start  = 0x40000000,
    	.end    = 0x40000000 + 0xA00000 - 1, // BASE_ADDR + MEM_SPACE_SIZE - 1 
    	.flags  = IORESOURCE_MEM,
    },
    {
        .name   = "PCIE:1 memory space",
        .start  = 0x58000000,
        .end    = 0x58000000 + 0xA00000 - 1, // BASE_ADDR + MEM_SPACE_SIZE - 1 
        .flags  = IORESOURCE_MEM,
    },
};

static struct resource g_st_io_space[HI_PCIE_NR] = 
{
    {
    	.name   = "PCIE:0 io space",
    	.start  = 0x40000000 + 0xA00000,      // BASE_ADDR + MEM_SPACE_SIZE
    	.end    = 0x40000000 + 0x1000000 - 1, // BASE_ADDR + BASE_ADDR_SIZE - 1
    	.flags  = IORESOURCE_IO,
    },
    {
        .name   = "PCIE:1 io space",
        .start  = 0x58000000 + 0xA00000,      // BASE_ADDR + MEM_SPACE_SIZE
        .end    = 0x58000000 + 0x1000000 - 1, // BASE_ADDR + BASE_ADDR_SIZE - 1
        .flags  = IORESOURCE_IO,
    },
};

static hi_pcie_info_s* hi_pcie_bus_to_info(hi_int32 i_busnr)
{
    hi_uchar8       uc_cnt   = 0;
    hi_pcie_info_s *pst_info = &g_st_pcie_data.st_info[0];

    for ( uc_cnt=0; uc_cnt<HI_PCIE_NR; uc_cnt++,pst_info++ )
    {
        if ( ( i_busnr == pst_info->i_busnr ) && ( HI_ENABLE == pst_info->uc_stat ) )
        {
            return pst_info; 
        }
    }
    
    return HI_NULL; 
}

static hi_pcie_info_s* hi_pcie_bus_alloc_info( hi_void )
{
    hi_uchar8       uc_cnt   = 0;
    hi_pcie_info_s *pst_info = &g_st_pcie_data.st_info[0];

    for ( uc_cnt=0; uc_cnt<HI_PCIE_NR; uc_cnt++,pst_info++ )
    {
        if ( ( -1 == pst_info->i_busnr ) && ( HI_ENABLE == pst_info->uc_stat ) )
        {
            return pst_info; 
        }
    }
    
    return HI_NULL; 
}

static hi_int32 hi_pcie_bus_getnum( hi_void )
{
    hi_uchar8       uc_cnt    = 0;
    hi_int32        i_busnum = 0;
    hi_pcie_info_s *pst_info = &g_st_pcie_data.st_info[0];

    for ( uc_cnt=0; uc_cnt<HI_PCIE_NR; uc_cnt++,pst_info++ )
    {
        if ( HI_ENABLE == pst_info->uc_stat )
        {
            i_busnum++; 
        }
    }
    
    return i_busnum; 
}

static hi_int32 hi_pcie_read_conf(struct pci_bus* pst_bus, hi_uint32 ui_devfn, hi_int32 i_where, hi_int32 i_size, hi_uint32* pui_value)
{
	hi_uint32  ui_val  = 0; 
	hi_pcie_info_s* pst_info = hi_pcie_bus_to_info(pst_bus->number);

	if ( (HI_NULL == pst_info) || (ui_devfn > 0) )
	{
	    return -EIO;
	}
    
	HI_REG_READ((pst_info->pv_conf_base + (i_where & (~0x3))), ui_val);
    switch(i_size)
    {
        case 1:
            *pui_value = (ui_val >> ((i_where & 0x3) << 3)) & 0xff;
            break; 
        case 2:
            *pui_value = (ui_val >> ((i_where & 0x3) << 3)) & 0xffff;
            break; 
        case 4:
            *pui_value = ui_val;
            break; 
        default:
            hi_printk("\nPCI:%d read conf size(%d) err!!!",pst_bus->number,i_size);
            return -EIO;
    }
    
	return 0;
}

static hi_int32 hi_pcie_write_conf(struct pci_bus *pst_bus, hi_uint32 ui_devfn, hi_int32 i_where, hi_int32 i_size, hi_uint32 ui_value)
{
	hi_int32   i_ret   = 0;
	hi_uint32  ui_org  = 0;
	hi_pcie_info_s* pst_info = hi_pcie_bus_to_info(pst_bus->number);

	if ( HI_NULL == pst_info )
	{
	    return -EIO;
	}
    
    i_ret = hi_pcie_read_conf(pst_bus, ui_devfn, i_where, 4, &ui_org);
	if ( 0 != i_ret )
	{
		return i_ret;
	}

    switch(i_size)
    {
        case 1: 
            ui_org &= (~(0xff << ((i_where & 0x3) << 3)));
            ui_org |= (ui_value << ((i_where & 0x3) << 3));
            break; 
        case 2:
            ui_org &= (~(0xffff << ((i_where & 0x3) << 3)));
            ui_org |= (ui_value << ((i_where & 0x3) << 3));
            break; 
        case 4:
            ui_org = ui_value;
            break; 
        default:
            hi_printk("\nPCI:%d write conf size(%d) err!!!",pst_bus->number,i_size);
            break; 
    }
	HI_REG_WRITE(pst_info->pv_conf_base + (i_where & (~0x3)), ui_org);
	return 0;
}

static struct pci_ops g_st_ops = 
{
	.read  = hi_pcie_read_conf,
	.write = hi_pcie_write_conf,
};

static hi_void hi_pcie_preinit(hi_void)
{
}

static hi_int32 hi_pcie_setup( hi_int32 i_nr, struct pci_sys_data* pst_sys )
{
    hi_int32        i_ret    = 0; 
	hi_pcie_info_s* pst_info = hi_pcie_bus_to_info(i_nr);

    if( HI_NULL != pst_info )
    {
    	return -EIO;        
    }

    pst_info = hi_pcie_bus_alloc_info();
    if( HI_NULL == pst_info )
    {
    	return -EIO;        
    }

	hi_printk("\nPCI:%d busnr=%d setup",pst_info->uc_ch,i_nr);

    //
	pst_info->i_busnr = i_nr; 
	pst_sys->mem_offset  = 0;
    // resource
    i_ret = request_resource(&ioport_resource, pst_info->pst_io_space );
    if ( 0 != i_ret )
    {
        hi_printk("\nPCI:%d mapping io base err!!!",pst_info->uc_ch);
        return i_ret;
    }
    i_ret = request_resource(&iomem_resource, pst_info->pst_mem_space );
    if(0 != i_ret)
    {
        hi_printk("\nPCI:%d mapping mem base err!!!",pst_info->uc_ch);
        return i_ret;
    }        
    // setup
    //pst_sys->resource[0] = pst_info->pst_io_space;
    //pst_sys->resource[1] = pst_info->pst_mem_space;
	//pst_sys->resource[2] = HI_NULL;
	pci_add_resource_offset(&pst_sys->resources, pst_info->pst_io_space, pst_sys->io_offset);
	pci_add_resource_offset(&pst_sys->resources, pst_info->pst_mem_space, pst_sys->mem_offset);
	return 1;
}

static struct pci_bus *hi_pcie_scan_bus(hi_int32 i_nr, struct pci_sys_data* pst_sys)
{
	//return pci_scan_bus(pst_sys->busnr, &g_st_ops, pst_sys );
	return pci_scan_bus(i_nr, &g_st_ops, pst_sys );
}

static hi_int32 hi_pcie_map_irq(struct pci_dev* pst_dev, hi_uchar8 uc_slot, hi_uchar8 uc_pin )
{
	hi_pcie_info_s* pst_info = hi_pcie_bus_to_info(pst_dev->bus->number);

	if ( ( HI_NULL == pst_info ) || ( (uc_pin - 1) >= HI_PCIE_INT_NR ) )
	{
	    return -EIO;
	}	
	return pst_info->pst_reg->uc_irq_pin[uc_pin - 1];
}

static struct hw_pci g_st_hw_pci = 
{
	.nr_controllers = HI_PCIE_NR,        /*default 2 controllers*/
    .preinit        = hi_pcie_preinit,
	//.swizzle        = pci_common_swizzle,
	.setup          = hi_pcie_setup,
	.scan           = hi_pcie_scan_bus,
	.map_irq        = hi_pcie_map_irq,
};

#ifdef __DONT_MODIFY__
#endif

hi_int32 hi_pcie_check_link_1_1(hi_pcie_info_s *pst_info)
{
    hi_uint32 ui_val        = 0; 
    hi_int32  i_loop        = 0;
    hi_uint32 ui_stable_cnt = 0; 
    
    do {
		HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_stat0, ui_val ); 
		if ( (0 == (ui_val & 0x20)) || (0 == (ui_val & 0x8000)) )
		{
            ui_stable_cnt = 0;
		}
		else
		{
		    ui_stable_cnt++;
		}        
		if ( 3 <= ui_stable_cnt)
		{
            hi_printk("\nPCI:%d 1.1 Device Link Up.", pst_info->uc_ch);
            return HI_ENABLE; 
		}
		mdelay(1);
    }while( i_loop++ < 500); 
    
    hi_printk("\nPCI:%d 1.1 Device Link Down.", pst_info->uc_ch);
    return HI_DISABLE; 
}

hi_int32 hi_pcie_check_link_2_0(hi_pcie_info_s *pst_info)
{
    hi_uint32 ui_val        = 0; 
    hi_int32  i_idx         = 0;
    hi_uint32 ui_stable_cnt = 0; 

	//配置pcie2.0速率	   
	HI_REG_READ( pst_info->ui_dbi_base + 0x700 + 0x10C, ui_val);
	ui_val = ui_val | 0x20000;
	HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x10C, ui_val);
    
    do {
        HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_stat4, ui_val);
        ui_val = ui_val & 0xFE0000;
		if ( 0x460000 != ui_val )
		{
			ui_stable_cnt = 0;
		}
        else
        {
            ui_stable_cnt++;
        }
           
		if (3 <= ui_stable_cnt)
		{
			break;
		}
        mdelay(1);         
    }while(i_idx++ < 500); 

    // ctrl 0
	HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x0, ui_val);
    ui_val = 0x40000000;
    HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x0, ui_val);

	// ctrl 1
	HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x4, ui_val);
	ui_val = 0x0;
	HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x4, ui_val);   

	if ( 3 <= ui_stable_cnt )
	{
		hi_printk("\nPCI:%d 2.0 Device Link Up.", pst_info->uc_ch);
        return HI_ENABLE; 
	}
    return HI_DISABLE; 
}

static hi_void hi_pcie_install( hi_pcie_info_s *pst_info )
{
    hi_uint32 ui_val = 0; 
    hi_uint32 ui_idx = 0; 

    // clock enable
	HI_REG_READ( g_st_pcie_data.pv_crg_base_virt + 0x20, ui_val);
	ui_val |= ( 1 << pst_info->pst_reg->ui_clk_en );
	HI_REG_WRITE(g_st_pcie_data.pv_crg_base_virt + 0x20, ui_val);

    // tx
    HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl1, ui_val); 
    ui_val = 0x420539F3;
	HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl1, ui_val);

	// reset low
	HI_REG_READ( g_st_pcie_data.pv_crg_base_virt + 0x30, ui_val); 
	ui_val &= ~( 1 << pst_info->pst_reg->ui_srst_n );
    HI_REG_WRITE(g_st_pcie_data.pv_crg_base_virt + 0x30, ui_val); 
    mdelay(1);

	// reset high
	HI_REG_READ( g_st_pcie_data.pv_crg_base_virt + 0x30, ui_val);
	ui_val |= ( 1 << pst_info->pst_reg->ui_srst_n );
    HI_REG_WRITE(g_st_pcie_data.pv_crg_base_virt + 0x30, ui_val);
    mdelay(1);

    // ctrl 7 disable
	HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x1C, ui_val); 
	ui_val = 0;
	HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x1C, ui_val);

    // ctrl 1 
	HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x4, ui_val); 
	ui_val = 0x200000;
	HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x4, ui_val);

    // ctrl 6
	HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x18, ui_val);
	ui_val = 0x8000000;
	HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x18, ui_val);

	// ctrl 0
	HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0, ui_val);
	ui_val = 0x40200000;
	HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0, ui_val);

    // config dbi
    HI_REG_READ( pst_info->ui_dbi_base + 0x4, ui_val);
	ui_val = 0x100007;
	HI_REG_WRITE(pst_info->ui_dbi_base + 0x4, ui_val);
	for (ui_idx = 0; ui_idx < HI_PCIE_IATU_TBL_CNT; ui_idx++)
	{
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x200, pst_info->pst_iatu[ui_idx].ui_viewport);
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x204, pst_info->pst_iatu[ui_idx].ui_region_ctrl_1);
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x208, pst_info->pst_iatu[ui_idx].ui_region_ctrl_2);
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x20C, pst_info->pst_iatu[ui_idx].ui_lbar);
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x210, pst_info->pst_iatu[ui_idx].ui_ubar);
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x214, pst_info->pst_iatu[ui_idx].ui_lar);
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x218, pst_info->pst_iatu[ui_idx].ui_ltar);
        HI_REG_WRITE(pst_info->ui_dbi_base + 0x700 + 0x21C, pst_info->pst_iatu[ui_idx].ui_utar);
	}

    // reset chip to call board self define function
    hi_pcie_reset_chip( pst_info->uc_ch );

    // ctrl 7 enable
    HI_REG_READ( g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x1C, ui_val);
    ui_val |= ( (1 << 11) | 0x02200000);
    HI_REG_WRITE(g_st_pcie_data.pv_sys_base_virt + pst_info->pst_reg->ui_perictrl0 + 0x1C, ui_val);  

    // try 1.1
    pst_info->uc_stat = hi_pcie_check_link_1_1(pst_info); 
    if ( HI_FALSE == pst_info->uc_stat )
    {
        return; 
    }
    // try 2.0
    pst_info->uc_stat_2_0 = hi_pcie_check_link_2_0(pst_info); 
    return; 
}

static hi_int32 hi_pcie_init_ch( hi_void )
{
    hi_uint32       ui_ch      = 0; 
    hi_pcie_info_s *pst_info   = &g_st_pcie_data.st_info[0];

    for ( ui_ch = 0; ui_ch < HI_PCIE_NR; ui_ch++, pst_info++ )
    {
        // id
        pst_info->i_busnr  = -1;    // invalid id
        pst_info->uc_ch    = (hi_uchar8)ui_ch;
        pst_info->uc_stat  = HI_DISABLE;
        
        // reg iatu
        pst_info->pst_reg  = &g_st_reg_info[ui_ch];
        pst_info->pst_iatu = &g_st_iatu_table[ui_ch][0]; 

        // iomap
        pst_info->pv_conf_base = ioremap_nocache(g_st_reg_info[ui_ch].ui_conf_base, 0x1000); 
        if (HI_NULL == pst_info->pv_conf_base)
        {
            hi_printk("\nPCI:%d mapping conf base err!!!",ui_ch);
            return -EIO;
        }
        pst_info->ui_dbi_base = (hi_uint32)ioremap_nocache(g_st_reg_info[ui_ch].ui_dbi_base, 0x1000); 
        if(HI_NULL == (hi_void*)pst_info->ui_dbi_base)
        {
            hi_printk("\nPCI:%d mapping dbi base err!!!",ui_ch);
            return -EIO;
        }
        // resource
        pst_info->pst_io_space  = &g_st_io_space[ui_ch];
        pst_info->pst_mem_space = &g_st_mem_space[ui_ch];
    }
    return 0; 
}

static hi_void hi_pcie_init_sys(hi_void)
{
	g_st_pcie_data.pv_sys_base_virt   = (hi_iomem*)hi_io_address(HI_REG_BASE_SCTL); 
	g_st_pcie_data.pv_crg_base_virt   = (hi_iomem*)hi_io_address(HI_REG_BASE_CRG);
	g_st_pcie_data.pv_iomux_base_virt = (hi_iomem*)hi_io_address(HI_REG_BASE_IOMUX);
}

static hi_void hi_pcie_exit(hi_void)
{
    hi_uint32       ui_ch    = 0; 
    hi_pcie_info_s *pst_info = &g_st_pcie_data.st_info[0];

    for ( ui_ch = 0; ui_ch < HI_PCIE_NR; ui_ch++, pst_info++ )
    {
        iounmap((hi_iomem*)pst_info->ui_dbi_base);
    }
}

static hi_int32 hi_pcie_init(hi_void)
{
    hi_int32        i_ret    = 0; 
    hi_uint32       ui_ch    = 0; 
	hi_uint32       ui_val   = 0; 
    hi_pcie_info_s *pst_info = &g_st_pcie_data.st_info[0];

    // clear
    hi_memset(&g_st_pcie_data, 0, sizeof(hi_pcie_mng_data_s)); 
    //
    hi_pcie_init_sys();
    //
    i_ret = hi_pcie_init_ch(); 
    if ( 0 != i_ret )
    {
        hi_printk("\nPCI: init ch err(0x%x)!!!",i_ret);
        return i_ret;
    }    
    // reset
    HI_REG_WRITE(g_st_pcie_data.pv_iomux_base_virt + 0x500, 0     ); 
    
    // disable pcie
    HI_REG_READ( g_st_pcie_data.pv_crg_base_virt   + 0x40 , ui_val); 
	ui_val &= ~( ( 1 << 1 ) | ( 1 << 2 ) );
    HI_REG_WRITE(g_st_pcie_data.pv_crg_base_virt   + 0x40 , ui_val);     

    // install card
    for ( ui_ch = 0; ui_ch < HI_PCIE_NR; ui_ch++, pst_info++ )
    {
        hi_pcie_install(pst_info); 
    }
    // init
    g_st_hw_pci.nr_controllers = hi_pcie_bus_getnum();
	pci_common_init(&g_st_hw_pci);
    // exit
    //hi_pcie_exit();
	return 0;
}

subsys_initcall(hi_pcie_init); 

