/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#include <mach/hi_hsan.h>


typedef volatile hi_uint32       hi_v_u32;

#define HI_SC_PEREN0     0x14880014
#define HI_SC_PERDIS0    0x14880018
#define HI_SC_PEREN1     0x14880020
#define HI_SC_PERDIS1    0x14880024
#define HI_SC_RST_CTRL0  0x1488002c


#define HI_GUSB3PIPECTL0 0x10a5c2c0
#define HI_GUSB3PIPECTL1 0x10a5c2c4



typedef struct
{
    hi_v_u32 *pv_peren0;
    hi_v_u32 *pv_perdis0;
    hi_v_u32 *pv_peren1;
    hi_v_u32 *pv_perdis1;
    hi_v_u32 *pv_rst_ctrl0;
    hi_v_u32 *pv_gusb3pipectrl0;
    hi_v_u32 *pv_gusb3pipectrl1;
} hi_usb_ctrl_reg_s;

static hi_usb_ctrl_reg_s  g_st_usb_reg_ctrl;


static atomic_t g_st_usb_cnt =
{
	.counter = 0,
};

static hi_int32 hi_usb_map_reg(hi_void)
{
    g_st_usb_reg_ctrl.pv_peren0 = ioremap(HI_SC_PEREN0, sizeof(hi_v_u32));
    if(NULL == g_st_usb_reg_ctrl.pv_peren0)
    {
        hi_printk("ioremap error: g_st_usb_reg_ctrl.pv_peren0 = NULL\r\n");
        return -1;
    }
    
    g_st_usb_reg_ctrl.pv_perdis0 = ioremap(HI_SC_PERDIS0, sizeof(hi_v_u32));
    if(NULL == g_st_usb_reg_ctrl.pv_perdis0)
    {
        hi_printk("ioremap error: g_st_usb_reg_ctrl.pv_perdis0 = NULL\r\n");
        iounmap(g_st_usb_reg_ctrl.pv_peren0);
        return -1;
    }

    g_st_usb_reg_ctrl.pv_peren1 = ioremap(HI_SC_PEREN1, sizeof(hi_v_u32));
    if(NULL == g_st_usb_reg_ctrl.pv_peren1)
    {
        hi_printk("ioremap error: g_st_usb_reg_ctrl.pv_peren1 = NULL\r\n");
        iounmap(g_st_usb_reg_ctrl.pv_peren0);
        iounmap(g_st_usb_reg_ctrl.pv_perdis0);

        return -1;
    }
    
    g_st_usb_reg_ctrl.pv_perdis1 = ioremap(HI_SC_PERDIS1, sizeof(hi_v_u32));
    if(NULL == g_st_usb_reg_ctrl.pv_perdis1)
    {
        hi_printk("ioremap error: g_st_usb_reg_ctrl.pv_perdis1 = NULL\r\n");
        iounmap(g_st_usb_reg_ctrl.pv_peren0);
        iounmap(g_st_usb_reg_ctrl.pv_perdis0);
        iounmap(g_st_usb_reg_ctrl.pv_peren1);
        return -1;
    }

    
    g_st_usb_reg_ctrl.pv_rst_ctrl0 = ioremap(HI_SC_RST_CTRL0, sizeof(hi_v_u32));
    if(NULL == g_st_usb_reg_ctrl.pv_rst_ctrl0)
    {
        hi_printk("ioremap error: g_st_usb_reg_ctrl.pv_rst_ctrl0 = NULL\r\n");
        iounmap(g_st_usb_reg_ctrl.pv_peren0);
        iounmap(g_st_usb_reg_ctrl.pv_perdis0);
        iounmap(g_st_usb_reg_ctrl.pv_peren1);
        iounmap(g_st_usb_reg_ctrl.pv_perdis1);
        return -1;
    } 

    g_st_usb_reg_ctrl.pv_gusb3pipectrl0 = ioremap(HI_GUSB3PIPECTL0, sizeof(hi_v_u32));
    if(NULL == g_st_usb_reg_ctrl.pv_gusb3pipectrl0)
    {
        hi_printk("ioremap error: g_st_usb_reg_ctrl.pv_gusb3pipectrl0 = NULL\r\n");
        iounmap(g_st_usb_reg_ctrl.pv_peren0);
        iounmap(g_st_usb_reg_ctrl.pv_perdis0);
        iounmap(g_st_usb_reg_ctrl.pv_peren1);
        iounmap(g_st_usb_reg_ctrl.pv_perdis1);
        iounmap(g_st_usb_reg_ctrl.pv_gusb3pipectrl0);
        return -1;
    } 

    
    g_st_usb_reg_ctrl.pv_gusb3pipectrl1 = ioremap(HI_GUSB3PIPECTL1, sizeof(hi_v_u32));
    if(NULL == g_st_usb_reg_ctrl.pv_gusb3pipectrl1)
    {
        hi_printk("ioremap error: g_st_usb_reg_ctrl.pv_gusb3pipectrl1 = NULL\r\n");
        iounmap(g_st_usb_reg_ctrl.pv_peren0);
        iounmap(g_st_usb_reg_ctrl.pv_perdis0);
        iounmap(g_st_usb_reg_ctrl.pv_peren1);
        iounmap(g_st_usb_reg_ctrl.pv_perdis1);
        iounmap(g_st_usb_reg_ctrl.pv_gusb3pipectrl0);
        iounmap(g_st_usb_reg_ctrl.pv_gusb3pipectrl1);
        return -1;
    } 
    
    return 0; 
}

static hi_void hi_usb_map_unreg(hi_void)
{
	iounmap(g_st_usb_reg_ctrl.pv_peren0);
    iounmap(g_st_usb_reg_ctrl.pv_perdis0);
    iounmap(g_st_usb_reg_ctrl.pv_peren1);
    iounmap(g_st_usb_reg_ctrl.pv_perdis1);
  	iounmap(g_st_usb_reg_ctrl.pv_rst_ctrl0);    
    iounmap(g_st_usb_reg_ctrl.pv_gusb3pipectrl0);
    iounmap(g_st_usb_reg_ctrl.pv_gusb3pipectrl1);
}

static hi_void hi_usb_start_hcd_2(hi_void)
{
    hi_uint32 ui_val = 0;

    ui_val = *(g_st_usb_reg_ctrl.pv_peren0);
    ui_val |= 1 << 16;
    *(g_st_usb_reg_ctrl.pv_peren0) = 1 << 16;

    ui_val = *(g_st_usb_reg_ctrl.pv_peren1);
    ui_val |= 1 << 22;
    *(g_st_usb_reg_ctrl.pv_peren1) = 1 << 22;

    udelay(100);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val &= 0xFFF87FFF; 
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(10);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val |= 1 << 15;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val |= 1 << 18;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(50);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val |= 1 << 17;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(100);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val |= 1 << 16;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(100);    
}

static hi_void hi_usb_stop_hcd_2(hi_void)
{
    hi_uint32 ui_val = 0;
    
    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val  &= 0xFFF87FFF;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;
    
    ui_val = *(g_st_usb_reg_ctrl.pv_peren0);
    ui_val &= 0xFFFEFFFF;
    *(g_st_usb_reg_ctrl.pv_peren0) = ui_val;
    
    ui_val = *(g_st_usb_reg_ctrl.pv_peren1);
    ui_val &= 0xFFBFFFFF;
    *(g_st_usb_reg_ctrl.pv_peren1) = ui_val;
    
}

static hi_void hi_usb_start_hcd_3(hi_void)
{
    hi_uint32 ui_val = 0;

    ui_val = *(g_st_usb_reg_ctrl.pv_peren0);
    ui_val |= 0x10c00; 
    *(g_st_usb_reg_ctrl.pv_peren0) = ui_val;

    ui_val = *(g_st_usb_reg_ctrl.pv_peren1);
    ui_val |= 1 << 22;
    *(g_st_usb_reg_ctrl.pv_peren1) = ui_val;

    udelay(10);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val &= 0xFFF07FFF; 
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(20);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val |= 0xC0000;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(250);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val |= 0x30000;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(10);

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val |= 0x8000;
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;

    udelay(10);  

    ui_val = *(g_st_usb_reg_ctrl.pv_gusb3pipectrl0);
    ui_val &= ~0x00020000;
    *(g_st_usb_reg_ctrl.pv_gusb3pipectrl0) = ui_val;

    udelay(10);  

    ui_val = *(g_st_usb_reg_ctrl.pv_gusb3pipectrl1);
    ui_val &= ~0x00020000;
    *(g_st_usb_reg_ctrl.pv_gusb3pipectrl1) = ui_val;

       
}

static hi_void hi_usb_stop_hcd_3(hi_void)
{
	hi_uint32 ui_val = 0;

    ui_val = *(g_st_usb_reg_ctrl.pv_rst_ctrl0);
    ui_val &= 0xFFF07FFF; 
    *(g_st_usb_reg_ctrl.pv_rst_ctrl0) = ui_val;
    
    ui_val = *(g_st_usb_reg_ctrl.pv_peren0);
    ui_val &=0xFFFEF3FF;
    *(g_st_usb_reg_ctrl.pv_peren0) = ui_val;
    
    ui_val = *(g_st_usb_reg_ctrl.pv_peren1);
    ui_val &= 0xFFBFFFFF;
    *(g_st_usb_reg_ctrl.pv_peren1) = ui_val;    
}

hi_void hi_usb_start_hcd(hi_void)
{
	hi_ulong32 ul_flags = 0;

	local_irq_save(ul_flags);
	if(atomic_add_return(1, &g_st_usb_cnt) == 1)
    {
        if(0 != hi_usb_map_reg())
        {
            local_irq_restore(ul_flags);
            return; 
        }
        
        if(HI_CHIP_ID_H_E == hi_get_chip_id())
        {
            hi_usb_start_hcd_2(); 
        }
         else if(HI_CHIP_ID_T_E == hi_get_chip_id())
         {
            hi_usb_start_hcd_3(); 
         }
         
		hi_printk("hi_usb_start_hcd sucessful!\r\n");
	}
	local_irq_restore(ul_flags);
}
EXPORT_SYMBOL(hi_usb_start_hcd);

hi_void hi_usb_stop_hcd(hi_void)
{
    hi_ulong32 ul_flags = 0;

    local_irq_save(ul_flags);
    if(atomic_sub_return(1, &g_st_usb_cnt) == 0)
    {
        if(HI_CHIP_ID_H_E == hi_get_chip_id())
        {
            hi_usb_stop_hcd_2(); 
        }
        else
        {
            hi_usb_stop_hcd_3(); 
        }

  	    hi_usb_map_unreg();	
    	hi_printk("hi_usb_stop_hcd sucessful!\r\n");
    }
local_irq_restore(ul_flags);
}
EXPORT_SYMBOL(hi_usb_stop_hcd);

