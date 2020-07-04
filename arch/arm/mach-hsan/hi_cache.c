/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_cache.c
  版 本 号   : 初稿
  生成日期   : D2014_03_09

******************************************************************************/

#include <mach/hi_hsan.h>

static hi_uint32  g_ui_l2cache_base = 0;

static DEFINE_SPINLOCK(l2x0_lock);

static inline hi_void hi_l2cache_write( hi_uint32 ui_reg, hi_uint32 ui_value )
{
	HI_REG_WRITE( g_ui_l2cache_base + ui_reg, ui_value );
}

static inline hi_void hi_l2cache_sync(hi_void)
{
    hi_uint32 ui_value = 0;
    HI_REG_READ( g_ui_l2cache_base + HI_REG_L2_SYNC, ui_value );
}

static hi_void hi_l2cache_auto(hi_uint32 ui_way_num)
{
    hi_uint32 ui_value = 0;
    
    hi_trace();
    //
    hi_l2cache_sync();
    //
    ui_value = (ui_way_num << HI_BIT_L2_MAINT_AUTO_WAYADDRESS) | (0x1 << HI_BIT_L2_MAINT_AUTO_START | 0x2 );
    hi_l2cache_write( HI_REG_L2_MAINT_AUTO, ui_value );
    //
    do
    {
        HI_REG_READ(g_ui_l2cache_base + HI_REG_L2_RINT, ui_value);  
    }while( !((ui_value) & (0x1 << HI_BIT_L2_RINT_AUTO_END)) );  
    //
    HI_REG_READ(g_ui_l2cache_base + HI_REG_L2_RINT, ui_value); 
    hi_l2cache_write( HI_REG_L2_INTCLR, ui_value );
    //
    hi_l2cache_sync();
    //
    hi_trace();
}

static hi_void hi_l2cache_invalid(hi_uint32 ui_way_num)
{
    hi_uint32 ui_value = 0;
    
    hi_trace();
    //
    hi_l2cache_sync();
    //
    ui_value = (ui_way_num << HI_BIT_L2_MAINT_AUTO_WAYADDRESS) | (0x1 << HI_BIT_L2_MAINT_AUTO_START);
    hi_l2cache_write( HI_REG_L2_MAINT_AUTO, ui_value );
    //
    do
    {
        HI_REG_READ(g_ui_l2cache_base + HI_REG_L2_RINT, ui_value);  
    }while( !((ui_value) & (0x1 << HI_BIT_L2_RINT_AUTO_END)) );  
    //
    HI_REG_READ(g_ui_l2cache_base + HI_REG_L2_RINT, ui_value); 
    hi_l2cache_write( HI_REG_L2_INTCLR, ui_value );
    //
    hi_l2cache_sync();
    //
    hi_trace();
}

hi_void hi_l2cache_invalid_range(hi_ulong32 ul_start, hi_ulong32 ul_end)
{
	hi_uint32 ui_addr  = 0;
 	hi_uint32 ui_value = 0;
    hi_uint32 ui_flags; 	

	spin_lock_irqsave(&l2x0_lock, ui_flags);
	ul_start &= ~(HI_L2_LINE_SIZE - 1);
	hi_l2cache_sync();

	for ( ui_addr = ul_start; ui_addr<ul_end; ui_addr += HI_L2_LINE_SIZE )
    {
    	ui_value = ui_addr | HI_BIT_L2_INVALID_BYADDRESS;
		hi_l2cache_write(HI_REG_L2_INVALID_SYNC, ui_value );
	}

	hi_l2cache_sync();
	spin_unlock_irqrestore(&l2x0_lock, ui_flags);	

}

hi_void hi_l2cache_flush_range(hi_ulong32 ul_start, hi_ulong32 ul_end)
{
	hi_uint32 ui_addr = 0;
    hi_uint32 ui_flags;
	
	spin_lock_irqsave(&l2x0_lock, ui_flags);
	
	ul_start &= ~(HI_L2_LINE_SIZE - 1);
	hi_l2cache_sync();

	for ( ui_addr=ul_start; ui_addr<ul_end; ui_addr+=HI_L2_LINE_SIZE)
	{
		hi_l2cache_write( HI_REG_L2_CLEAN_SYNC,   ui_addr | 0x1 );
		hi_l2cache_write( HI_REG_L2_INVALID_SYNC, ui_addr | 0x1 );
	}

	hi_l2cache_sync();
	spin_unlock_irqrestore(&l2x0_lock, ui_flags);	
}

hi_void hi_l2cache_clean_range(hi_ulong32 ul_start, hi_ulong32 ul_end)
{
	hi_uint32 ui_addr  = 0;
	hi_uint32 ui_value = 0;
    hi_uint32 ui_flags;

	
	spin_lock_irqsave(&l2x0_lock, ui_flags);
    ul_start &= ~(HI_L2_LINE_SIZE - 1);
	
	hi_l2cache_sync();
	

	for ( ui_addr=ul_start; ui_addr<ul_end; ui_addr+=HI_L2_LINE_SIZE )
	{
    	ui_value = ui_addr | HI_BIT_L2_CLEAN_BYADDRESS;
		hi_l2cache_write( HI_REG_L2_CLEAN_SYNC, ui_value );
	}
	
	hi_l2cache_sync();
	spin_unlock_irqrestore(&l2x0_lock, ui_flags);	 
}

hi_void hi_l2cache_sync_lock(hi_void)
{
    hi_l2cache_sync();
    hi_trace();
}

hi_void hi_l2cache_invalid_all(hi_void)
{
	hi_uint32 ui_way_num = 0;

	hi_l2cache_sync();
    //
	for(ui_way_num = 0; ui_way_num<HI_L2_WAY_NUM; ui_way_num++)
	{
		hi_l2cache_invalid(ui_way_num);
	}
	//
	hi_l2cache_sync();
}

hi_void hi_l2cache_clean_all(hi_void)
{
	hi_uint32 ui_way_num = 0;

    hi_l2cache_sync();
    //
	for(ui_way_num = 0; ui_way_num<HI_L2_WAY_NUM; ui_way_num++)
	{
		hi_l2cache_auto(ui_way_num);
	}
	//
	hi_l2cache_sync();
}

static irqreturn_t hi_l2cache_irq( hi_int32 i_irq, hi_void *pv_dev )
{
	hi_uint32 ui_value = 0;
	
    HI_REG_READ( g_ui_l2cache_base + HI_REG_L2_RINT,                ui_value); 
    hi_printk("\nL2cache:HI_REG_L2_RINT(0x%x) irq err!!!",          ui_value);
    HI_REG_READ( g_ui_l2cache_base + HI_REG_L2_SPECIAL_CHECK0,      ui_value); 
    hi_printk("\nL2cache:HI_REG_L2_SPECIAL_CHECK0(0x%x) irq err!!!",ui_value);
    HI_REG_READ( g_ui_l2cache_base + HI_REG_L2_SPECIAL_CHECK1,      ui_value); 
    hi_printk("\nL2cache:HI_REG_L2_SPECIAL_CHECK1(0x%x) irq err!!!",ui_value);
    //
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_INTCLR,         0);
    return IRQ_HANDLED;
}

hi_void hi_l2cache_init(hi_void)
{
	hi_int32  i_ret    = 0;
	hi_uint32 ui_value = 0;

    // don't support L2cache
    if ( HI_CHIP_ID_H_E == hi_get_chip_id() )
    {
        hi_printk("\nL2cache:chip don't support");
        return;
    }
    // mapping    
	g_ui_l2cache_base = hi_io_address(HI_REG_BASE_L2CACHE); 
    //
	/* disable L2cache */
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_CTRL, 0); 
    //
    HI_REG_READ(g_ui_l2cache_base + HI_REG_L2_AUCTRL, ui_value ); 

	ui_value |= (0x1 << HI_BIT_L2_AUCTRL_EVENT_BUS_EN) | (0x1 << HI_BIT_L2_AUCTRL_MONITOR_EN) | (0x1 << HI_BIT_L2_AUCTRL_SHARED_ATTRIBUTE_OVER_EN);
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_AUCTRL, ui_value); 
	/* clean last error hi_int32 */
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_RINT, 0); 
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_SPECIAL_CHECK0, 0); 
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_SPECIAL_CHECK1, 0); 
	/* enable inner INT except HI_BIT_L2_INTMASK_AUTO_END */
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_INTMASK, 0x3fff); 
    //
	hi_l2cache_invalid_all();
	/* enable L2cache */
    HI_REG_WRITE(g_ui_l2cache_base + HI_REG_L2_CTRL, 1); 
	//
	i_ret += request_irq(HI_INTNR_L2CACHE_CHK0_INT,hi_l2cache_irq,0,"L2 chk0",HI_NULL);
	i_ret += request_irq(HI_INTNR_L2CACHE_CHK1_INT,hi_l2cache_irq,0,"L2 chk1",HI_NULL);
	i_ret += request_irq(HI_INTNR_L2CACHE_INT_COMB,hi_l2cache_irq,0,"L2 com", HI_NULL);
	if ( 0 != i_ret )
	{
		hi_printk("\nL2cache:reg irq err!!!");
		return;
	}
    // reg
	outer_cache.inv_range   = hi_l2cache_invalid_range;
	outer_cache.clean_range = hi_l2cache_clean_range;
	outer_cache.flush_range = hi_l2cache_flush_range;
	//
	hi_printk("\nL2cache:controller succ");
}



