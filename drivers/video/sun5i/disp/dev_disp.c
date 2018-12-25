#include "dev_disp.h"
#include "linux/pm.h"

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif


fb_info_t g_fbi;
__disp_drv_t g_disp_drv;

#define MY_BYTE_ALIGN(x) ( ( (x + (4*1024-1)) >> 12) << 12)             /* alloc based on 4K byte */
static struct alloc_struct_t boot_heap_head, boot_heap_tail;

static unsigned int gbuffer[4096];
static __u32 suspend_output_type[2] = {0,0};
static __u32 suspend_status = 0;//0:normal; suspend_status&1 != 0:in early_suspend; suspend_status&2 != 0:in suspend;
static __u32 suspend_prestep = 0; //0:after early suspend; 1:after suspend; 2:after resume; 3 :after late resume

static struct info_mm  g_disp_mm[10];
static int g_disp_mm_sel = 0;

static struct cdev *my_cdev;
static dev_t devid ;
static struct class *disp_class;

static struct resource disp_resource[DISP_IO_NUM] =
{
	[DISP_IO_SCALER0] = {
		.start = 0x01e00000,
		.end   = 0x01e0077f,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_SCALER1] = {
		.start = 0x01e20000,
		.end   = 0x01e2077f,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_IMAGE0] = {
		.start = 0x01e60000,
		.end   = 0x01e657ff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_IMAGE1] = {
		.start = 0x01e40000,
		.end   = 0x01e457ff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_LCDC0] = {
		.start = 0x01c0c000,
		.end   = 0x01c0cfff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_LCDC1] = {
		.start = 0x01c0d000,
		.end   = 0x01c0dfff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_TVEC0] = {
		.start = 0x01c0a000,
		.end   = 0x01c0afff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_TVEC1] = {
		.start = 0x01c1b000,
		.end   = 0x01c1bfff,
		.flags = IORESOURCE_MEM,
	},
	[DISP_IO_IEP] = {
		.start = 0x01e70000,
		.end   = 0x01e703ff,
		.flags = IORESOURCE_MEM,
	},
};


__s32 disp_create_heap(__u32 pHeapHead, __u32 nHeapSize)
{
    boot_heap_head.size    = boot_heap_tail.size = 0;
    boot_heap_head.address = pHeapHead;
    boot_heap_tail.address = pHeapHead + nHeapSize;
    boot_heap_head.next    = &boot_heap_tail;
    boot_heap_tail.next    = 0;

    __inf("head:%x,tail:%x\n" ,boot_heap_head.address, boot_heap_tail.address);
    return 0;
}

void *disp_malloc(__u32 num_bytes)
{
    struct alloc_struct_t *ptr, *newptr;
    __u32  actual_bytes;

    if (!num_bytes)
    {
        return 0;
    }

    actual_bytes = MY_BYTE_ALIGN(num_bytes);    /* translate the byte count to size of long type       */

    ptr = &boot_heap_head;                      /* scan from the boot_heap_head of the heap            */

    while (ptr && ptr->next)                    /* look for enough memory for alloc                    */
    {
        if (ptr->next->address >= (ptr->address + ptr->size + (8 * 1024) + actual_bytes))
        {
            break;
        }
                                                /* find enough memory to alloc                         */
        ptr = ptr->next;
    }

    if (!ptr->next)
    {
        __wrn(" it has reached the boot_heap_tail of the heap now\n");
        return 0;                   /* it has reached the boot_heap_tail of the heap now              */
    }

    newptr = (struct alloc_struct_t *)(ptr->address + ptr->size);
                                                /* create a new node for the memory block             */
    if (!newptr)
    {
        __wrn(" create the node failed, can't manage the block\n");
        return 0;                               /* create the node failed, can't manage the block     */
    }

    /* set the memory block chain, insert the node to the chain */
    newptr->address = ptr->address + ptr->size + 4*1024;
    newptr->size    = actual_bytes;
    newptr->o_size  = num_bytes;
    newptr->next    = ptr->next;
    ptr->next       = newptr;

    return (void *)newptr->address;
}

void  disp_free(void *p)
{
    struct alloc_struct_t *ptr, *prev;

	if( p == NULL )
		return;

    ptr = &boot_heap_head;                /* look for the node which po__s32 this memory block                     */
    while (ptr && ptr->next)
    {
        if (ptr->next->address == (__u32)p)
            break;              /* find the node which need to be release                              */
        ptr = ptr->next;
    }

	prev = ptr;
	ptr = ptr->next;

    if (!ptr) return;           /* the node is heap boot_heap_tail                                               */

    prev->next = ptr->next;     /* delete the node which need be released from the memory block chain  */

    return ;
}

__s32 DRV_lcd_open(__u32 sel)
{
    __u32 i = 0;
    __lcd_flow_t *flow;

//    printk("<0> Opening LCD w/o delays\n");
	if(g_disp_drv.b_lcd_open[sel] == 0)
	{
	    BSP_disp_lcd_open_before(sel);

	    flow = BSP_disp_lcd_get_open_flow(sel);
	    for(i=0; i<flow->func_num; i++)
	    {
	        flow->func[i].func(sel);
            if (flow->func[i].delay > 0) {
//                printk("<0> delaying next step for %d ms\n", flow->func[i].delay);
                mdelay(flow->func[i].delay);
            }
	    }

	    BSP_disp_lcd_open_after(sel);

		g_disp_drv.b_lcd_open[sel] = 1;
	}

    return 0;
}



__s32 DRV_lcd_close(__u32 sel)
{
    __u32 i = 0;
    __lcd_flow_t *flow;

	if(g_disp_drv.b_lcd_open[sel] == 1)
	{
	    BSP_disp_lcd_close_befor(sel);

	    flow = BSP_disp_lcd_get_close_flow(sel);
	    for(i=0; i<flow->func_num; i++)
	    {
	        flow->func[i].func(sel);
            if (flow->func[i].delay > 0) {
//                printk("<0> delaying next closing step for %d ms\n", flow->func[i].delay);
                mdelay(flow->func[i].delay);
            }
	    }

	    BSP_disp_lcd_close_after(sel);

		g_disp_drv.b_lcd_open[sel] = 0;
	}
    return 0;
}

//run the last step of lcd open flow(backlight)
__s32 disp_lcd_open_late(__u32 sel)
{
    __lcd_flow_t *flow;

	if(g_disp_drv.b_lcd_open[sel] == 0)
	{
        flow = BSP_disp_lcd_get_open_flow(sel);
        flow->func[flow->func_num-1].func(sel);
        flow->cur_step = 0;
        
	    BSP_disp_lcd_open_after(sel);

		g_disp_drv.b_lcd_open[sel] = 1;
	}

    return 0;
}
//run lcd close flow
//run lcd close flow accept the first step(backlight)
__s32 disp_lcd_close_late(__u32 sel)
{
    __u32 i = 0;
    __lcd_flow_t *close_flow;
    __lcd_flow_t *open_flow;

	if(g_disp_drv.b_lcd_open[sel] == 0)
	{

	    close_flow = BSP_disp_lcd_get_close_flow(sel);
        open_flow = BSP_disp_lcd_get_open_flow(sel);

        if(open_flow->cur_step != open_flow->func_num) //if there is task in timer list,cancel it
        {
            del_timer(&g_fbi.disp_timer[sel]);
        }
	    for(i=1; i<close_flow->func_num; i++)
	    {
	        __u32 timeout = close_flow->func[i].delay*HZ/1000;

	        close_flow->func[i].func(sel);

	    	set_current_state(TASK_INTERRUPTIBLE);
	    	schedule_timeout(timeout);
	    }
	    BSP_disp_lcd_close_after(sel);

		g_disp_drv.b_lcd_open[sel] = 0;
	}
    return 0;
}

void disp_lcd_open_flow_init_status(__u32 sel)
{
    __lcd_flow_t *flow;

    flow = BSP_disp_lcd_get_open_flow(sel);
    flow->cur_step = 0;
}

void disp_lcd_open_timer(unsigned long sel)
{
    __lcd_flow_t *flow;
    __u32 timeout;

    flow = BSP_disp_lcd_get_open_flow(sel);
    flow->cur_step = (flow->cur_step == flow->func_num)? 0:flow->cur_step;

	if((g_disp_drv.b_lcd_open[sel] == 0) && (flow->cur_step != flow->func_num-1))
	{

        if(flow->cur_step == 0)
	    {
            BSP_disp_lcd_open_before(sel);
        }
        
	    flow->func[flow->cur_step].func(sel);

        timeout = flow->func[flow->cur_step].delay*HZ/1000;

        g_fbi.disp_timer[sel].function = &disp_lcd_open_timer;
        g_fbi.disp_timer[sel].data = sel;//(unsigned int)&g_fbi;
        g_fbi.disp_timer[sel].expires = jiffies + timeout;
        add_timer(&g_fbi.disp_timer[sel]);
	}

    flow->cur_step ++;
    
    return;
}


__s32 disp_set_hdmi_func(__disp_hdmi_func * func)
{
    BSP_disp_set_hdmi_func(func);

    return 0;
}

__s32 DRV_DISP_Init(void)
{
    __disp_bsp_init_para para;

    init_waitqueue_head(&g_fbi.wait[0]);
    init_waitqueue_head(&g_fbi.wait[1]);
    g_fbi.wait_count[0] = 0;
    g_fbi.wait_count[1] = 0;

    memset(&para, 0, sizeof(__disp_bsp_init_para));
    para.base_image0    = (__u32)g_fbi.base_image0;
    para.base_image1    = (__u32)g_fbi.base_image1;
    para.base_scaler0   = (__u32)g_fbi.base_scaler0;
    para.base_scaler1   = (__u32)g_fbi.base_scaler1;
    para.base_lcdc0     = (__u32)g_fbi.base_lcdc0;
    para.base_lcdc1     = (__u32)g_fbi.base_lcdc1;
    para.base_tvec0      = (__u32)g_fbi.base_tvec0;
    para.base_tvec1      = (__u32)g_fbi.base_tvec1;
    para.base_iep       = (__u32)g_fbi.base_iep;
    para.base_ccmu      = (__u32)g_fbi.base_ccmu;
    para.base_sdram     = (__u32)g_fbi.base_sdram;
    para.base_pioc      = (__u32)g_fbi.base_pioc;
    para.base_pwm       = (__u32)g_fbi.base_pwm;

    //modified by heyihang
    para.base_tp        = (__u32)g_fbi.base_tp;
	para.disp_int_process       = DRV_disp_int_process;

	memset(&g_disp_drv, 0, sizeof(__disp_drv_t));
//    init_timer(&g_disp_drv.disp_timer[0]);
//    init_timer(&g_disp_drv.disp_timer[1]);
    
    BSP_disp_init(&para);
    BSP_disp_open();


    return 0;
}

__s32 DRV_DISP_Exit(void)
{
    Fb_Exit();
    BSP_disp_close();
    BSP_disp_exit(g_disp_drv.exit_mode);

    return 0;
}


int disp_mem_request(int sel,__u32 size)
{
#ifndef FB_RESERVED_MEM
	unsigned map_size = 0;
	struct page *page;

	if(g_disp_mm[sel].info_base != 0)
		return -EINVAL;

	g_disp_mm[sel].mem_len = size;
	map_size = PAGE_ALIGN(g_disp_mm[sel].mem_len);

	page = alloc_pages(GFP_KERNEL,get_order(map_size));
	if(page != NULL)
	{
		g_disp_mm[sel].info_base = page_address(page);
		if(g_disp_mm[sel].info_base == 0)
		{
			free_pages((unsigned long)(page),get_order(map_size));
			__wrn("page_address fail!\n");
			return -ENOMEM;
		}
		g_disp_mm[sel].mem_start = virt_to_phys(g_disp_mm[sel].info_base);
		memset(g_disp_mm[sel].info_base,0,size);

		__inf("pa=0x%08lx va=0x%p size:0x%x\n",g_disp_mm[sel].mem_start, g_disp_mm[sel].info_base, size);
		return 0;
	}
	else
	{
		__wrn("alloc_pages fail!\n");
		return -ENOMEM;
	}
#else
    __u32 ret = 0;

	ret = (__u32)disp_malloc(size);
	if(ret != 0)
	{
	    g_disp_mm[sel].info_base = (void*)ret;
	    g_disp_mm[sel].mem_start = virt_to_phys(g_disp_mm[sel].info_base);
	    memset(g_disp_mm[sel].info_base,0,size);
	    __inf("pa=0x%08lx va=0x%p size:0x%x\n",g_disp_mm[sel].mem_start, g_disp_mm[sel].info_base, size);

	    return 0;
	}
	else
	{
		__wrn("disp_malloc fail!\n");
		return -ENOMEM;
	}
#endif

}

int disp_mem_release(int sel)
{
#ifndef FB_RESERVED_MEM
	unsigned map_size = PAGE_ALIGN(g_disp_mm[sel].mem_len);
	unsigned page_size = map_size;

	if(g_disp_mm[sel].info_base == 0)
		return -EINVAL;

	free_pages((unsigned long)(g_disp_mm[sel].info_base),get_order(page_size));
	memset(&g_disp_mm[sel],0,sizeof(struct info_mm));
#else
	if(g_disp_mm[sel].info_base == 0)
		return -EINVAL;

    disp_free((void *)g_disp_mm[sel].info_base);
    memset(&g_disp_mm[sel],0,sizeof(struct info_mm));
#endif

	return 0;

}

int disp_mmap(struct file *file, struct vm_area_struct * vma)
{
	unsigned long  physics =  g_disp_mm[g_disp_mm_sel].mem_start;// - PAGE_OFFSET;
	unsigned long mypfn = physics >> PAGE_SHIFT;
	unsigned long vmsize = vma->vm_end-vma->vm_start;

	if(remap_pfn_range(vma,vma->vm_start,mypfn,vmsize,vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

int disp_open(struct inode *inode, struct file *file)
{
    return 0;
}

int disp_release(struct inode *inode, struct file *file)
{
    return 0;
}
ssize_t disp_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

ssize_t disp_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}

static int __init disp_probe(struct platform_device *pdev)//called when platform_driver_register
{
	fb_info_t * info = NULL;
    
	__inf("disp_probe call\n");

	info = &g_fbi;
    init_timer(&info->disp_timer[0]);
    init_timer(&info->disp_timer[1]);
    
	info->dev = &pdev->dev;
	platform_set_drvdata(pdev,info);

	info->base_image0   = 0xf1e60000;
	info->base_image1   = 0xf1e40000;
	info->base_scaler0  = 0xf1e00000;
	info->base_scaler1  = 0xf1e20000;
	info->base_lcdc0    = 0xf1c0c000;
	info->base_lcdc1    = 0xf1c0d000;
	info->base_tvec0    = 0xf1c0a000;
	info->base_tvec1    = 0xf1c1b000;
	info->base_ccmu     = 0xf1c20000;
	info->base_sdram    = 0xf1c01000;
	info->base_pioc     = 0xf1c20800;
	info->base_pwm      = 0xf1c20c00;
	info->base_iep      = 0xf1e70000;

    //modified by heyihang
    info->base_tp       = 0xf1c25000;
	__inf("SCALER0 base 0x%08x\n", info->base_scaler0);
	__inf("SCALER1 base 0x%08x\n", info->base_scaler1);
	__inf("IMAGE0 base 0x%08x\n", info->base_image0+ 0x800);
	__inf("IMAGE1 base 0x%08x\n", info->base_image1+ 0x800);
	__inf("LCDC0 base 0x%08x\n", info->base_lcdc0);
	__inf("LCDC1 base 0x%08x\n", info->base_lcdc1);
	__inf("TVEC0 base 0x%08x\n", info->base_tvec0);
	__inf("TVEC1 base 0x%08x\n", info->base_tvec1);
	__inf("IEP base 0x%08x\n", info->base_iep);
	__inf("CCMU base 0x%08x\n", info->base_ccmu);
	__inf("SDRAM base 0x%08x\n", info->base_sdram);
	__inf("PIO base 0x%08x\n", info->base_pioc);
	__inf("PWM base 0x%08x\n", info->base_pwm);

	return 0;
}

static int disp_remove(struct platform_device *pdev)
{
	__inf("disp_remove call\n");

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
void backlight_early_suspend(struct early_suspend *h)
{

    printk("disp_early_suspend!!\n");

	while(Disp_eink_get_update_status(0)) {
		printk("msleep 100ms ... \n");
		msleep(100);
	}
	
    suspend_status |= 1;
    suspend_prestep = 0;

    BSP_disp_clk_off(2);

    printk("end of backlight_early_suspend ... \n");
}

void backlight_late_resume(struct early_suspend *h)
{
    printk("disp_late_resume!!\n");

    if(suspend_prestep != 2)
    {
    	BSP_disp_clk_on(2);
    }
    
    suspend_status &= (~1);
    suspend_prestep = 3;

}

static struct early_suspend backlight_early_suspend_handler =
{
    //del by heyihang.2012-12-17
    //.level   = EARLY_SUSPEND_LEVEL_DISABLE_FB,
    .level   = EARLY_SUSPEND_LEVEL_BLANK_SCREEN,    
	.suspend = backlight_early_suspend,
	.resume = backlight_late_resume,
};

#endif

static __u32 image0_reg_bak,scaler0_reg_bak;
int disp_suspend(struct platform_device *pdev, pm_message_t state)
{
    int i = 0;
    pr_info("==disp_suspend call\n");
    
#ifndef CONFIG_HAS_EARLYSUSPEND

    printk("[Old_River]CONFIG_HAS_EARLYSUSPEND not defined!!\n");
    
    for(i=0; i<2; i++)
    {

        for(i=0; i<2; i++)
        {
            suspend_output_type[i] = BSP_disp_get_output_type(i);
            if(suspend_output_type[i] == DISP_OUTPUT_TYPE_LCD)
            {
                DRV_lcd_close(i);
            }
            else if(suspend_output_type[i] == DISP_OUTPUT_TYPE_TV)
            {
                BSP_disp_tv_close(i);
            }
            else if(suspend_output_type[i] == DISP_OUTPUT_TYPE_VGA)
            {
                BSP_disp_vga_close(i);
            }
            else if(suspend_output_type[i] == DISP_OUTPUT_TYPE_HDMI)
            {
                BSP_disp_hdmi_close(i);
            }
        }
    }
#endif

	if(2 == suspend_prestep)//suspend after resume,not  after early suspend
    {
        for(i=0; i<2; i++)
        {
            if(suspend_output_type[i] == DISP_OUTPUT_TYPE_LCD)
            {
                disp_lcd_close_late(i);
            }
        }
    }
     if(SUPER_STANDBY == standby_type)
     {  
        pr_info("==disp super standby enter\n");

        image0_reg_bak = (__u32)disp_malloc(0xe00 - 0x800);
        scaler0_reg_bak = (__u32)disp_malloc(0xa18);
        BSP_disp_store_image_reg(0, image0_reg_bak);
        BSP_disp_store_scaler_reg(0, scaler0_reg_bak);
     }

    BSP_disp_hdmi_suspend();
#ifndef CONFIG_HAS_EARLYSUSPEND
    BSP_disp_clk_off(3);
#else
    BSP_disp_clk_off(1);
    BSP_disp_clk_off(2);
#endif

    suspend_status |= 2;
    suspend_prestep = 1;

    return 0;
}

int disp_resume(struct platform_device *pdev)
{
    int i = 0;
    pr_info("==disp_resume call\n");

    BSP_disp_clk_on(1);
	BSP_disp_clk_on(2);
    if(SUPER_STANDBY == standby_type)
    {
        
        pr_info("==disp super standby exit\n");
        
        BSP_disp_restore_scaler_reg(0, scaler0_reg_bak);
        BSP_disp_restore_image_reg(0, image0_reg_bak);
        BSP_disp_restore_lcdc_reg(0);
        BSP_disp_restore_tvec_reg(0);
        disp_free((void*)scaler0_reg_bak);
        disp_free((void*)image0_reg_bak);         
    }

	
    
#ifndef CONFIG_HAS_EARLYSUSPEND
    {
        for(i=0; i<2; i++)
        {
            if(suspend_output_type[i] == DISP_OUTPUT_TYPE_LCD)
        {
            DRV_lcd_open(i);
        }
        else if(suspend_output_type[i] == DISP_OUTPUT_TYPE_TV)
        {
            BSP_disp_tv_open(i);
        }
        else if(suspend_output_type[i] == DISP_OUTPUT_TYPE_VGA)
        {
            BSP_disp_vga_open(i);
            }
            else if(suspend_output_type[i] == DISP_OUTPUT_TYPE_HDMI)
            {
                BSP_disp_hdmi_open(i);
            }
        }
    }
#else

   for(i=0; i<2; i++)
    {
        if(suspend_output_type[i] == DISP_OUTPUT_TYPE_LCD)
        {
            disp_lcd_open_flow_init_status(i);
            disp_lcd_open_timer(i);//start lcd open flow
        }
    }

#endif
    
    BSP_disp_hdmi_resume();
    
    suspend_status &= (~2);
    suspend_prestep = 2;
    return 0;
}

void disp_shutdown(struct platform_device *pdev)
{
    __u32 type = 0, i = 0;

    for(i=0; i<2; i++)
    {
        type = BSP_disp_get_output_type(i);
        if(type == DISP_OUTPUT_TYPE_LCD)
        {
            DRV_lcd_close(i);
        }
    }
}

long disp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned long karg[4];
	unsigned long ubuffer[4] = {0};
	__s32 ret = 0;

	if (copy_from_user((void*)karg,(void __user*)arg, 4*sizeof(unsigned long)))
	{
		__wrn("copy_from_user fail\n");
		return -EFAULT;
	}

	ubuffer[0] = *(unsigned long*)karg;
	ubuffer[1] = (*(unsigned long*)(karg+1));
	ubuffer[2] = (*(unsigned long*)(karg+2));
	ubuffer[3] = (*(unsigned long*)(karg+3));

    if(cmd < DISP_CMD_FB_REQUEST)
    {
        if(ubuffer[0] != 0)
        {
            __wrn("para err in disp_ioctl, screen id = %d\n", (int)ubuffer[0]);
            return -1;
        }
    }
    if(suspend_status & 2)
    {
        __wrn("ioctl:%x fail when in suspend!\n", cmd);
        return -1;
    }
    
#if 0
    if(cmd!=DISP_CMD_TV_GET_INTERFACE && cmd!=DISP_CMD_HDMI_GET_HPD_STATUS && cmd!=DISP_CMD_GET_OUTPUT_TYPE 
    	&& cmd!=DISP_CMD_SCN_GET_WIDTH && cmd!=DISP_CMD_SCN_GET_HEIGHT
    	&& cmd!=DISP_CMD_VIDEO_SET_FB && cmd!=DISP_CMD_VIDEO_GET_FRAME_ID)
    {
        OSAL_PRINTF("cmd:0x%x,%ld,%ld\n",cmd, ubuffer[0], ubuffer[1]);
    }
#endif

    switch(cmd)
    {
    //----disp global----
    	case DISP_CMD_SET_BKCOLOR:
	    {
	        __disp_color_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_color_t)))
    		{
    			return  -EFAULT;
    		}
		    ret = BSP_disp_set_bk_color(ubuffer[0], &para);
		    break;
	    }

    	case DISP_CMD_SET_COLORKEY:
    	{
    	    __disp_colorkey_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_colorkey_t)))
    		{
    			return  -EFAULT;
    		}
    		ret = BSP_disp_set_color_key(ubuffer[0], &para);
		    break;
		}

    	case DISP_CMD_SET_PALETTE_TBL:
    	    if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
    	    {
    	        __wrn("para invalid in disp ioctrl DISP_CMD_SET_PALETTE_TBL,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
    	        return -1;
    	    }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[3]))
    		{
    			return  -EFAULT;
    		}
    		ret = BSP_disp_set_palette_table(ubuffer[0], (__u32 *)gbuffer, ubuffer[2], ubuffer[3]);
    		break;

    	case DISP_CMD_GET_PALETTE_TBL:
    	    if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
    	    {
    	        __wrn("para invalid in disp ioctrl DISP_CMD_GET_PALETTE_TBL,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
    	        return -1;
    	    }
    		ret = BSP_disp_get_palette_table(ubuffer[0], (__u32 *)gbuffer, ubuffer[2], ubuffer[3]);
    		if(copy_to_user((void __user *)ubuffer[1], gbuffer,ubuffer[3]))
    		{
    			return  -EFAULT;
    		}
    		break;

    	case DISP_CMD_START_CMD_CACHE:
    		ret = BSP_disp_cmd_cache(ubuffer[0]);
    		break;

    	case DISP_CMD_EXECUTE_CMD_AND_STOP_CACHE:
    		ret = BSP_disp_cmd_submit(ubuffer[0]);
    		break;

    	case DISP_CMD_GET_OUTPUT_TYPE:
    		ret =  BSP_disp_get_output_type(ubuffer[0]);
    		break;

    	case DISP_CMD_SCN_GET_WIDTH:
    		ret = 800;//BSP_disp_get_screen_width(ubuffer[0]);
    		break;

    	case DISP_CMD_SCN_GET_HEIGHT:
    		ret = 600;//BSP_disp_get_screen_height(ubuffer[0]);
    		break;

    	case DISP_CMD_SET_GAMMA_TABLE:
    	    if((ubuffer[1] == 0) || ((int)ubuffer[2] <= 0))
    	    {
    	        __wrn("para invalid in disp ioctrl DISP_CMD_SET_GAMMA_TABLE,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[2]);
    	        return -1;
    	    }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[2]))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_set_gamma_table(ubuffer[0], (__u32 *)gbuffer, ubuffer[2]);
    		break;

    	case DISP_CMD_GAMMA_CORRECTION_ON:
    		ret = BSP_disp_gamma_correction_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_GAMMA_CORRECTION_OFF:
    		ret = BSP_disp_gamma_correction_disable(ubuffer[0]);
    		break;

        case DISP_CMD_SET_BRIGHT:
            ret = BSP_disp_set_bright(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_GET_BRIGHT:
            ret = BSP_disp_get_bright(ubuffer[0]);
    		break;

        case DISP_CMD_SET_CONTRAST:
            ret = BSP_disp_set_contrast(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_GET_CONTRAST:
            ret = BSP_disp_get_contrast(ubuffer[0]);
    		break;

        case DISP_CMD_SET_SATURATION:
            ret = BSP_disp_set_saturation(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_GET_SATURATION:
            ret = BSP_disp_get_saturation(ubuffer[0]);
    		break;

        case DISP_CMD_SET_HUE:
            ret = BSP_disp_set_hue(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_GET_HUE:
            ret = BSP_disp_get_hue(ubuffer[0]);
    		break;

    	case DISP_CMD_CAPTURE_SCREEN:
    	    ret = BSP_disp_capture_screen(ubuffer[0], (__disp_capture_screen_para_t *)ubuffer[1]);
    	    break;

        case DISP_CMD_SET_SCREEN_SIZE:
            ret = BSP_disp_set_screen_size(ubuffer[0], (__disp_rectsz_t*)ubuffer[1]);
            break;

	//----iep----
        case DISP_CMD_DE_FLICKER_ON:
            ret = BSP_disp_iep_deflicker_enable(ubuffer[0], 1);
            break;

        case DISP_CMD_DE_FLICKER_OFF:
            ret = BSP_disp_iep_deflicker_enable(ubuffer[0], 0);
            break;

        case DISP_CMD_GET_DE_FLICKER_EN:
        	ret = BSP_disp_iep_get_deflicker_enable(ubuffer[0]);
        	break;
        	
        case DISP_CMD_DRC_ON:
        	ret = BSP_disp_iep_drc_enable(ubuffer[0], 1);
        	break;

        case DISP_CMD_DRC_OFF:
			ret = BSP_disp_iep_drc_enable(ubuffer[0], 0);
        	break;

		case DISP_CMD_GET_DRC_EN:
			ret = BSP_disp_iep_get_drc_enable(ubuffer[0]);
			break;
			
        case DISP_CMD_DE_FLICKER_SET_WINDOW:
        {
          	__disp_rect_t para;
    	    
    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
 
			ret = BSP_disp_iep_set_demo_win(ubuffer[0], 1, &para);
			break;
		}
		      	
        case DISP_CMD_DRC_SET_WINDOW:
		{
			__disp_rect_t para;
			
			if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_rect_t)))
			{
				__wrn("copy_from_user fail\n");
				return	-EFAULT;
			}
		
			ret = BSP_disp_iep_set_demo_win(ubuffer[0], 2, &para);
			break;
		}
    //----layer----
    	case DISP_CMD_LAYER_REQUEST:
    		ret = BSP_disp_layer_request(ubuffer[0], (__disp_layer_work_mode_t)ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_RELEASE:
    		ret = BSP_disp_layer_release(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_OPEN:
    		ret = BSP_disp_layer_open(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_CLOSE:
    		ret = BSP_disp_layer_close(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_SET_FB:
    	{
    	    __disp_fb_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_framebuffer(ubuffer[0], ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;
    	}

    	case DISP_CMD_LAYER_GET_FB:
    	{
    	    __disp_fb_t para;

    		ret = BSP_disp_layer_get_framebuffer(ubuffer[0], ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2], &para,sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_SET_SRC_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_src_window(ubuffer[0],ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;
        }

    	case DISP_CMD_LAYER_GET_SRC_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_layer_get_src_window(ubuffer[0],ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2], &para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_SET_SCN_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_screen_window(ubuffer[0],ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;
        }

    	case DISP_CMD_LAYER_GET_SCN_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_layer_get_screen_window(ubuffer[0],ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2], &para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_SET_PARA:
    	{
    	    __disp_layer_info_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_layer_info_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_layer_set_para(ubuffer[0], ubuffer[1], &para);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;
        }

    	case DISP_CMD_LAYER_GET_PARA:
    	{
    	    __disp_layer_info_t para;

    		ret = BSP_disp_layer_get_para(ubuffer[0], ubuffer[1], &para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_layer_info_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_LAYER_TOP:
    		ret = BSP_disp_layer_set_top(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_BOTTOM:
    		ret = BSP_disp_layer_set_bottom(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_ALPHA_ON:
    		ret = BSP_disp_layer_alpha_enable(ubuffer[0], ubuffer[1], 1);
    		break;

    	case DISP_CMD_LAYER_ALPHA_OFF:
    		ret = BSP_disp_layer_alpha_enable(ubuffer[0], ubuffer[1], 0);
    		break;

    	case DISP_CMD_LAYER_SET_ALPHA_VALUE:
    		ret = BSP_disp_layer_set_alpha_value(ubuffer[0], ubuffer[1], ubuffer[2]);
    		//DRV_disp_wait_cmd_finish(ubuffer[0]);
    		break;

    	case DISP_CMD_LAYER_CK_ON:
    		ret = BSP_disp_layer_colorkey_enable(ubuffer[0], ubuffer[1], 1);
    		break;

    	case DISP_CMD_LAYER_CK_OFF:
    		ret = BSP_disp_layer_colorkey_enable(ubuffer[0], ubuffer[1], 0);
    		break;

    	case DISP_CMD_LAYER_SET_PIPE:
    		ret = BSP_disp_layer_set_pipe(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

    	case DISP_CMD_LAYER_GET_ALPHA_VALUE:
    		ret = BSP_disp_layer_get_alpha_value(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_ALPHA_EN:
    		ret = BSP_disp_layer_get_alpha_enable(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_CK_EN:
    		ret = BSP_disp_layer_get_colorkey_enable(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_PRIO:
    		ret = BSP_disp_layer_get_piro(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LAYER_GET_PIPE:
    		ret = BSP_disp_layer_get_pipe(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_SMOOTH:
            ret = BSP_disp_layer_set_smooth(ubuffer[0], ubuffer[1],(__disp_video_smooth_t) ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_SMOOTH:
            ret = BSP_disp_layer_get_smooth(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_BRIGHT:
            ret = BSP_disp_layer_set_bright(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_BRIGHT:
            ret = BSP_disp_layer_get_bright(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_CONTRAST:
            ret = BSP_disp_layer_set_contrast(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_CONTRAST:
            ret = BSP_disp_layer_get_contrast(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_SATURATION:
            ret = BSP_disp_layer_set_saturation(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_SATURATION:
            ret = BSP_disp_layer_get_saturation(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_HUE:
            ret = BSP_disp_layer_set_hue(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_HUE:
            ret = BSP_disp_layer_get_hue(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_ENHANCE_ON:
            ret = BSP_disp_layer_enhance_enable(ubuffer[0], ubuffer[1], 1);
    		break;

        case DISP_CMD_LAYER_ENHANCE_OFF:
            ret = BSP_disp_layer_enhance_enable(ubuffer[0], ubuffer[1], 0);
    		break;

        case DISP_CMD_LAYER_GET_ENHANCE_EN:
            ret = BSP_disp_layer_get_enhance_enable(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_VPP_ON:
            ret = BSP_disp_layer_vpp_enable(ubuffer[0], ubuffer[1], 1);
    		break;

        case DISP_CMD_LAYER_VPP_OFF:
            ret = BSP_disp_layer_vpp_enable(ubuffer[0], ubuffer[1], 0);
    		break;

        case DISP_CMD_LAYER_GET_VPP_EN:
            ret = BSP_disp_layer_get_vpp_enable(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_LUMA_SHARP_LEVEL:
            ret = BSP_disp_layer_set_luma_sharp_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_LUMA_SHARP_LEVEL:
            ret = BSP_disp_layer_get_luma_sharp_level(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_CHROMA_SHARP_LEVEL:
            ret = BSP_disp_layer_set_chroma_sharp_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_CHROMA_SHARP_LEVEL:
            ret = BSP_disp_layer_get_chroma_sharp_level(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_WHITE_EXTEN_LEVEL:
            ret = BSP_disp_layer_set_white_exten_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_WHITE_EXTEN_LEVEL:
            ret = BSP_disp_layer_get_white_exten_level(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_LAYER_SET_BLACK_EXTEN_LEVEL:
            ret = BSP_disp_layer_set_black_exten_level(ubuffer[0], ubuffer[1], ubuffer[2]);
    		break;

        case DISP_CMD_LAYER_GET_BLACK_EXTEN_LEVEL:
            ret = BSP_disp_layer_get_black_exten_level(ubuffer[0], ubuffer[1]);
    		break;

    //----scaler----
    	case DISP_CMD_SCALER_REQUEST:
    		ret = BSP_disp_scaler_request();
    		break;

    	case DISP_CMD_SCALER_RELEASE:
    		ret = BSP_disp_scaler_release(ubuffer[1]);
    		break;

    	case DISP_CMD_SCALER_EXECUTE:
    	{
    	    __disp_scaler_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_scaler_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_scaler_start(ubuffer[1],&para);
    		break;
        }

    //----hwc----
    	case DISP_CMD_HWC_OPEN:
    		ret =  BSP_disp_hwc_enable(ubuffer[0], 1);
    		break;

    	case DISP_CMD_HWC_CLOSE:
    		ret =  BSP_disp_hwc_enable(ubuffer[0], 0);
    		break;

    	case DISP_CMD_HWC_SET_POS:
    	{
    	    __disp_pos_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_pos_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_hwc_set_pos(ubuffer[0], &para);
    		break;
        }

    	case DISP_CMD_HWC_GET_POS:
    	{
    	    __disp_pos_t para;

    		ret = BSP_disp_hwc_get_pos(ubuffer[0], &para);
    		if(copy_to_user((void __user *)ubuffer[1],&para, sizeof(__disp_pos_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_HWC_SET_FB:
    	{
    	    __disp_hwc_pattern_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_hwc_pattern_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_hwc_set_framebuffer(ubuffer[0], &para);
    		break;
        }

    	case DISP_CMD_HWC_SET_PALETTE_TABLE:
			if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
            {
                __wrn("para invalid in display ioctrl DISP_CMD_HWC_SET_PALETTE_TABLE,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
                return -1;
            }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[3]))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_hwc_set_palette(ubuffer[0], (void*)gbuffer, ubuffer[2], ubuffer[3]);
    		break;


    //----video----
    	case DISP_CMD_VIDEO_START:
    		ret = BSP_disp_video_start(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_VIDEO_STOP:
    		ret = BSP_disp_video_stop(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_VIDEO_SET_FB:
    	{
    	    __disp_video_fb_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_video_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_video_set_fb(ubuffer[0], ubuffer[1], &para);
    		break;
        }

        case DISP_CMD_VIDEO_GET_FRAME_ID:
            ret = BSP_disp_video_get_frame_id(ubuffer[0], ubuffer[1]);
    		break;

        case DISP_CMD_VIDEO_GET_DIT_INFO:
        {
            __disp_dit_info_t para;

            ret = BSP_disp_video_get_dit_info(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_dit_info_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    //----lcd----
    	case DISP_CMD_LCD_ON:
    		ret = DRV_lcd_open(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_LCD;
            }
    		break;

    	case DISP_CMD_LCD_OFF:
    		ret = DRV_lcd_close(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_NONE;
            }
    		break;

    	case DISP_CMD_LCD_SET_BRIGHTNESS:
    		ret = BSP_disp_lcd_set_bright(ubuffer[0], ubuffer[1],0);
    		break;

    	case DISP_CMD_LCD_GET_BRIGHTNESS:
    		ret = BSP_disp_lcd_get_bright(ubuffer[0]);
    		break;

    	case DISP_CMD_LCD_CPUIF_XY_SWITCH:
    		ret = BSP_disp_lcd_xy_switch(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_LCD_SET_SRC:
    		ret = BSP_disp_lcd_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

        case DISP_CMD_LCD_USER_DEFINED_FUNC:
            ret = BSP_disp_lcd_user_defined_func(ubuffer[0], ubuffer[1], ubuffer[2], ubuffer[3]);
            break;

	//----pwm----
        case DISP_CMD_PWM_SET_PARA:
            ret = pwm_set_para(ubuffer[0], (__pwm_info_t *)ubuffer[1]);
            break;

        case DISP_CMD_PWM_GET_PARA:
            ret = pwm_get_para(ubuffer[0], (__pwm_info_t *)ubuffer[1]);
            break;


    //----tv----
    	case DISP_CMD_TV_ON:
    		ret = BSP_disp_tv_open(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_TV;
            }
    		break;

    	case DISP_CMD_TV_OFF:
    		ret = BSP_disp_tv_close(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_NONE;
            }
    		break;

    	case DISP_CMD_TV_SET_MODE:
    		ret = BSP_disp_tv_set_mode(ubuffer[0], (__disp_tv_mode_t)ubuffer[1]);
    		break;

    	case DISP_CMD_TV_GET_MODE:
    		ret = BSP_disp_tv_get_mode(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_AUTOCHECK_ON:
    		ret = BSP_disp_tv_auto_check_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_AUTOCHECK_OFF:
    		ret = BSP_disp_tv_auto_check_disable(ubuffer[0]);
    		break;

    	case DISP_CMD_TV_GET_INTERFACE:
    	    if(suspend_status != 0)
    	    {
    	        ret = DISP_TV_NONE;
    	    }
    	    else
    	    {
    		    ret = BSP_disp_tv_get_interface(ubuffer[0]);
            }
    		break;

    	case DISP_CMD_TV_SET_SRC:
    		ret = BSP_disp_tv_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

        case DISP_CMD_TV_GET_DAC_STATUS:
            if(suspend_status != 0)
            {
                ret = 0;
            }
            else
            {
                ret =  BSP_disp_tv_get_dac_status(ubuffer[0], ubuffer[1]);
            }
            break;

        case DISP_CMD_TV_SET_DAC_SOURCE:
            ret =  BSP_disp_tv_set_dac_source(ubuffer[0], ubuffer[1], (__disp_tv_dac_source)ubuffer[2]);
            break;

        case DISP_CMD_TV_GET_DAC_SOURCE:
            ret =  BSP_disp_tv_get_dac_source(ubuffer[0], ubuffer[1]);
            break;

    //----hdmi----
    	case DISP_CMD_HDMI_ON:
    		ret = BSP_disp_hdmi_open(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_HDMI;
            }
    		break;

    	case DISP_CMD_HDMI_OFF:
    		ret = BSP_disp_hdmi_close(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_NONE;
            }
    		break;

    	case DISP_CMD_HDMI_SET_MODE:
    		ret = BSP_disp_hdmi_set_mode(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_HDMI_GET_MODE:
    		ret = BSP_disp_hdmi_get_mode(ubuffer[0]);
    		break;

    	case DISP_CMD_HDMI_GET_HPD_STATUS:
    	    if(suspend_status != 0)
    	    {
    	        ret = 0;
    	    }
    	    else
    	    {
    	        ret = BSP_disp_hdmi_get_hpd_status(ubuffer[0]);
    	    }
    		break;

    	case DISP_CMD_HDMI_SUPPORT_MODE:
    		ret = BSP_disp_hdmi_check_support_mode(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_HDMI_SET_SRC:
    		ret = BSP_disp_hdmi_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

    //----vga----
    	case DISP_CMD_VGA_ON:
    		ret = BSP_disp_vga_open(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_VGA;
            }
    		break;

    	case DISP_CMD_VGA_OFF:
    		ret = BSP_disp_vga_close(ubuffer[0]);
            if(suspend_status != 0)
            {
                suspend_output_type[ubuffer[0]] = DISP_OUTPUT_TYPE_NONE;
            }
    		break;

    	case DISP_CMD_VGA_SET_MODE:
    		ret = BSP_disp_vga_set_mode(ubuffer[0], (__disp_vga_mode_t)ubuffer[1]);
    		break;

    	case DISP_CMD_VGA_GET_MODE:
    		ret = BSP_disp_vga_get_mode(ubuffer[0]);
    		break;

    	case DISP_CMD_VGA_SET_SRC:
    		ret = BSP_disp_vga_set_src(ubuffer[0], (__disp_lcdc_src_t)ubuffer[1]);
    		break;

    //----sprite----
    	case DISP_CMD_SPRITE_OPEN:
    		ret = BSP_disp_sprite_open(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_CLOSE:
    		ret = BSP_disp_sprite_close(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_FORMAT:
    		ret = BSP_disp_sprite_set_format(ubuffer[0], (__disp_pixel_fmt_t)ubuffer[1], (__disp_pixel_seq_t)ubuffer[2]);
    		break;

    	case DISP_CMD_SPRITE_GLOBAL_ALPHA_ENABLE:
    		ret = BSP_disp_sprite_alpha_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_GLOBAL_ALPHA_DISABLE:
    		ret = BSP_disp_sprite_alpha_disable(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_ENABLE:
    		ret = BSP_disp_sprite_get_alpha_enable(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_GLOBAL_ALPHA_VALUE:
    		ret = BSP_disp_sprite_set_alpha_vale(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_GET_GLOBAL_ALPHA_VALUE:
    		ret = BSP_disp_sprite_get_alpha_value(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_ORDER:
    		ret = BSP_disp_sprite_set_order(ubuffer[0], ubuffer[1],ubuffer[2]);
    		break;

    	case DISP_CMD_SPRITE_GET_TOP_BLOCK:
    		ret = BSP_disp_sprite_get_top_block(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_GET_BOTTOM_BLOCK:
    		ret = BSP_disp_sprite_get_bottom_block(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_SET_PALETTE_TBL:
            if((ubuffer[1] == 0) || ((int)ubuffer[3] <= 0))
            {
                __wrn("para invalid in display ioctrl DISP_CMD_SPRITE_SET_PALETTE_TBL,buffer:0x%x, size:0x%x\n", (unsigned int)ubuffer[1], (unsigned int)ubuffer[3]);
                return -1;
            }
    		if(copy_from_user(gbuffer, (void __user *)ubuffer[1],ubuffer[3]))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret =  BSP_disp_sprite_set_palette_table(ubuffer[0], (__u32 * )gbuffer,ubuffer[2],ubuffer[3]);
    		break;

    	case DISP_CMD_SPRITE_GET_BLOCK_NUM:
    		ret = BSP_disp_sprite_get_block_number(ubuffer[0]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_REQUEST:
    	{
    	    __disp_sprite_block_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_sprite_block_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_request(ubuffer[0], &para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_RELEASE:
    		ret = BSP_disp_sprite_block_release(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_SET_SCREEN_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_screen_win(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_SCREEN_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_sprite_block_get_srceen_win(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_SET_SOURCE_WINDOW:
    	{
    	    __disp_rect_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_src_win(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_SOURCE_WINDOW:
    	{
    	    __disp_rect_t para;

    		ret = BSP_disp_sprite_block_get_src_win(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_rect_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_SET_FB:
    	{
    	    __disp_fb_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_framebuffer(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_FB:
    	{
    	    __disp_fb_t para;

    		ret = BSP_disp_sprite_block_get_framebufer(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_fb_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_SET_TOP:
    		ret = BSP_disp_sprite_block_set_top(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_SET_BOTTOM:
    		ret = BSP_disp_sprite_block_set_bottom(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_GET_PREV_BLOCK:
    		ret = BSP_disp_sprite_block_get_pre_block(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_GET_NEXT_BLOCK:
    		ret = BSP_disp_sprite_block_get_next_block(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_GET_PRIO:
    		ret = BSP_disp_sprite_block_get_prio(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_OPEN:
    		ret = BSP_disp_sprite_block_open(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_CLOSE:
    		ret = BSP_disp_sprite_block_close(ubuffer[0], ubuffer[1]);
    		break;

    	case DISP_CMD_SPRITE_BLOCK_SET_PARA:
    	{
    	    __disp_sprite_block_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[2],sizeof(__disp_sprite_block_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		ret = BSP_disp_sprite_block_set_para(ubuffer[0], ubuffer[1],&para);
    		break;
        }

    	case DISP_CMD_SPRITE_BLOCK_GET_PARA:
    	{
    	    __disp_sprite_block_para_t para;

    		ret = BSP_disp_sprite_block_get_para(ubuffer[0], ubuffer[1],&para);
    		if(copy_to_user((void __user *)ubuffer[2],&para, sizeof(__disp_sprite_block_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
    		break;
        }

		//----framebuffer----
    	case DISP_CMD_FB_REQUEST:
    	{
    	    __disp_fb_create_para_t para;

    		if(copy_from_user(&para, (void __user *)ubuffer[1],sizeof(__disp_fb_create_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
			ret = Display_Fb_Request(ubuffer[0], &para);
			break;
        }

		case DISP_CMD_FB_RELEASE:
			ret = Display_Fb_Release(ubuffer[0]);
			break;

	    case DISP_CMD_FB_GET_PARA:
	    {
    	    __disp_fb_create_para_t para;

			ret = Display_Fb_get_para(ubuffer[0], &para);
    		if(copy_to_user((void __user *)ubuffer[1],&para, sizeof(__disp_fb_create_para_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
			break;
        }

	    case DISP_CMD_GET_DISP_INIT_PARA:
	    {
    	    __disp_init_t para;

			ret = Display_get_disp_init_para(&para);
    		if(copy_to_user((void __user *)ubuffer[0],&para, sizeof(__disp_init_t)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
			break;
        }

		case DISP_CMD_MEM_REQUEST:
			ret =  disp_mem_request(ubuffer[0],ubuffer[1]);
			break;

		//----for test----
		case DISP_CMD_MEM_RELASE:
			ret =  disp_mem_release(ubuffer[0]);
			break;

		case DISP_CMD_MEM_SELIDX:
			g_disp_mm_sel = ubuffer[0];
			break;

		case DISP_CMD_MEM_GETADR:
			ret = g_disp_mm[ubuffer[0]].mem_start;
			break;

		case DISP_CMD_SUSPEND:
		{
		    pm_message_t state;

			ret = disp_suspend(0, state);
			break;
        }

		case DISP_CMD_RESUME:
			ret = disp_resume(0);
			break;

        case DISP_CMD_PRINT_REG:
            ret = BSP_disp_print_reg(1, ubuffer[0]);
            break;

         //---- eink ----
        case DISP_CMD_EINK_INIT:
        {
            __eink_init_para eink_init_para[1];

            if(copy_from_user(eink_init_para, (void __user *)ubuffer[1], sizeof(__eink_init_para)))
    		{
    		    __wrn("copy_from_user fail\n");
    			return  -EFAULT;
    		}
            
            Disp_eink_init(ubuffer[0], eink_init_para);
            
            break;
        }
        case DISP_CMD_EINK_UNINIT:
        {
            Disp_eink_exit(ubuffer[0]);
            
            break;
        }
        case DISP_CMD_EINK_UPDATE:
        {
            if (suspend_status != 0)
            {
            	/*come to standby */
				printk("DISP_CMD_EINK_UPDATE, standby mode");
               	break;
            }
			
            Disp_eink_update(ubuffer[0], ubuffer[1], ubuffer[2]);
            
            break;
        }
        case DISP_CMD_EINK_SET_MODE:
        {
            Disp_eink_set_mode(ubuffer[0], (__eink_update_mode)ubuffer[1]);
            break;
        }
        case DISP_CMD_EINK_SET_TEMPERATURE:
        {
            Disp_eink_set_temperature(ubuffer[0], ubuffer[1]);
            
            break;
        }
        case DISP_CMD_EINK_SET_ORIENTATION:
        {
            Disp_eink_set_orientation(ubuffer[0], ubuffer[1], ubuffer[2]);
            break;
        }

        //add by heyihang.2012-12-25
        case DISP_CMD_EINK_GET_UPDATE_STATUS:
        {			
            ret = Disp_eink_get_update_status(ubuffer[0]);

            break;
        }
        
		default:
		    break;
    }

	return ret;
}

__s32 Drv_disp_lcd_tcon_open(__u32 sel)
{
    TCON_open(sel);
}

__s32 Drv_disp_lcd_tcon_close(__u32 sel)
{
    TCON_close(sel);
}

__s32 Drv_disp_lcd_bl_en(__u32 sel, __bool b_en)
{
    LCD_BL_EN(sel, b_en);
}

__s32 Drv_disp_fb_layer_set_top(__u32 sel, __u32 fb_id)
{
    __u32 layer_hdl = g_fbi.layer_hdl[fb_id][sel];
    
    BSP_disp_layer_set_top(sel, layer_hdl);
}

__s32 Drv_disp_fb_layer_set_bottom(__u32 sel, __u32 fb_id)
{
    __u32 layer_hdl = g_fbi.layer_hdl[fb_id][sel];
    
    BSP_disp_layer_set_bottom(sel, layer_hdl);
}

__s32 Drv_disp_close_fb(__u32 sel, __u32 fb_id)
{
    __u32 layer_hdl = g_fbi.layer_hdl[fb_id][sel];
    
    BSP_disp_layer_close(sel, layer_hdl);
}

__s32 Drv_disp_get_fb_info(__u32 fb_id, 
                                   __u32 *xoffset, 
                                   __u32 *yoffset, 
                                   unsigned long *smem_start)
{
    *xoffset = g_fbi.fbinfo[fb_id]->var.xoffset;
    *yoffset = g_fbi.fbinfo[fb_id]->var.yoffset;
    *smem_start = g_fbi.fbinfo[fb_id]->screen_base;
}

__s32 Drv_disp_get_fb_size(__u32 fb_id, __u32 *fb_width, __u32 *fb_height)
{
    *fb_width  = g_fbi.fbinfo[fb_id]->var.xres;
    *fb_height = g_fbi.fbinfo[fb_id]->var.yres;
}

__s32 Drv_disp_Fb_Request(__u32 fb_id, __disp_fb_create_para_t *fb_para)
{
    return Display_Fb_Request(fb_id, fb_para);
}

__s32 Drv_disp_Fb_Release(__u32 fb_id)
{
    return Display_Fb_Release(fb_id);
}

__s32 Drv_disp_wait_reg_load(__u32 sel)
{
    return BSP_disp_layer_wait_reg_load(sel);
}


__s32 Drv_disp_fb_set_layer_addr_eink(__u32 sel, __u32 fb_id, __u32 index)
{
    __u32 layer_hdl = g_fbi.layer_hdl[fb_id][sel];
    __disp_layer_info_t layer_para[1];
    __u32 layer_width, layer_height;
    
    #if 1
    BSP_disp_layer_get_para(sel, layer_hdl, layer_para);
    layer_width  = layer_para->scn_win.width;
    layer_height = layer_para->scn_win.height;
    
    layer_para->fb.addr[0] = g_fbi.fbinfo[fb_id]->fix.smem_start 
                                    + 4 * index * layer_width * layer_height;
    BSP_disp_layer_set_para(sel, layer_hdl, layer_para);
    #endif
}

__s32 Drv_disp_set_fb_yoffset(__u32 sel, __u32 fb_id, __u32 yoffset)
{
    __u32 layer_hdl = g_fbi.layer_hdl[fb_id][sel];
    __disp_layer_info_t layer_para[1];
    
    #if 0
    BSP_disp_layer_get_para(sel, layer_hdl, layer_para);
    layer_para->src_win.y = yoffset;
    BSP_disp_layer_set_para(sel, layer_hdl, layer_para);
    #endif
    
    //g_fbi.fbinfo[fb_id]->var.yoffset = yoffset;
}

__s32 Drv_disp_layer_request(__u32 sel, __disp_layer_work_mode_t mode)
{    
    return BSP_disp_layer_request(sel, mode);   //ÉêÇëÓ²¼þÍ¼²ã
}

__s32 Drv_disp_layer_release(__u32 sel, __hdle hlyr)
{    
    return BSP_disp_layer_release(sel, hlyr);   //ÊÍ·ÅÓ²¼þÍ¼²ã
}

__s32 Drv_disp_layer_set_para(int sel, __hdle hlyr, __disp_layer_info_t* p_layer_info)
{
    int ret;
	ret = BSP_disp_layer_set_para(sel, hlyr, p_layer_info);	
    return ret;
}        

__s32 Drv_disp_video_get_frame_id(int sel, __hdle hlyr)
{        
    return BSP_disp_video_get_frame_id(sel, (__u32)hlyr);
}

__s32 Drv_disp_layer_open(int sel, __hdle hlyr)
{
    __s32 ret = 0;
    ret = BSP_disp_layer_open(sel, hlyr);
    return ret;
}        

__s32 Drv_disp_layer_close(int sel, __hdle hlyr)
{
    __s32 ret = 0;
    ret = BSP_disp_layer_close(sel, hlyr);
    return ret;
}  

__s32 Drv_disp_eink_panel_on(int sel)
{
    DRV_lcd_open(sel);
    return 0;
}

__s32 Drv_disp_eink_panel_off(int sel)
{
    DRV_lcd_close(sel);
    return 0;
}

static const struct file_operations disp_fops =
{
	.owner		= THIS_MODULE,
	.open		= disp_open,
	.release    = disp_release,
	.write      = disp_write,
	.read		= disp_read,
	.unlocked_ioctl	= disp_ioctl,
	.mmap       = disp_mmap,
};

static struct platform_driver disp_driver =
{
	.probe		= disp_probe,
	.remove		= disp_remove,
	.suspend    = disp_suspend,
	.resume    = disp_resume,
	.shutdown   = disp_shutdown,
	.driver		=
	{
		.name	= "disp",
		.owner	= THIS_MODULE,
	},
};


struct platform_device disp_device =
{
	.name           = "disp",
	.id		        = -1,
	.num_resources  = ARRAY_SIZE(disp_resource),
	.resource	    = disp_resource,
	.dev            = {}
};

int __init disp_module_init(void)
{
    int ret, err;

    __inf("disp_module_init\n");

    alloc_chrdev_region(&devid, 0, 1, "disp");
    my_cdev = cdev_alloc();
    cdev_init(my_cdev, &disp_fops);
    my_cdev->owner = THIS_MODULE;
    err = cdev_add(my_cdev, devid, 1);
    if (err)
    {
        __wrn("cdev_add fail\n");
        return -1;
    }

    disp_class = class_create(THIS_MODULE, "disp");
    if (IS_ERR(disp_class))
    {
        __wrn("class_create fail\n");
        return -1;
    }

    device_create(disp_class, NULL, devid, NULL, "disp");

	ret = platform_device_register(&disp_device);

	if (ret == 0)
	{
		ret = platform_driver_register(&disp_driver);
	}
#ifdef CONFIG_HAS_EARLYSUSPEND
    register_early_suspend(&backlight_early_suspend_handler);
#endif

	return ret;
}

static void __exit disp_module_exit(void)
{
	__inf("disp_module_exit\n");

#ifdef CONFIG_HAS_EARLYSUSPEND
    unregister_early_suspend(&backlight_early_suspend_handler);
#endif
    DRV_DISP_Exit();

	platform_driver_unregister(&disp_driver);
	platform_device_unregister(&disp_device);

    device_destroy(disp_class,  devid);
    class_destroy(disp_class);

    cdev_del(my_cdev);
}

EXPORT_SYMBOL(disp_set_hdmi_func);
EXPORT_SYMBOL(DRV_DISP_Init);


module_init(disp_module_init);
module_exit(disp_module_exit);

MODULE_AUTHOR("danling_xiao");
MODULE_DESCRIPTION("display driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:disp");

