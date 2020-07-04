/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: hi_nand.c
  功能描述: nand初始化入口及对外接口定义
  版本描述: V1.0

  创建日期: D2013_04_20

  修改记录: 
            生成初稿.
******************************************************************************/
#include <asm/setup.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <mach/hi_typedef.h>
#include <mach/hi_flash.h>

#include "../../mtdcore.h"
#include "hi_nand.h"
#include "atpnandparts.h" //xy
#include "kerneltag.h"
#include "hi_nand_drv.h"
#include "atpconfig.h"

/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
#include "bhal.h"
#endif
/*<End:   monitor flash opteration>*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef SUPPORT_ATP_QHB_NFF_CHECK_IMG
extern unsigned int iBadblockNum;
#endif

/* boot parameters flash tag struct */
extern char *g_pc_flash_info;
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
hi_nand_operate_info g_flash_operation_info;
EXPORT_SYMBOL(g_flash_operation_info);
#endif
/*<End:   monitor flash opteration>*/

extern int hi_setup_mtd_partitions( struct mtd_info *mtd,
                             const unsigned char *puc_boot_param, 
                             struct mtd_partition **ppst_parts,
                             unsigned int *pui_mtd_num);

static struct hi_nand_host *g_pst_nand_host = HI_NULL;

/******************************************************************************
  函数功能:  nand get flash spec info from boot parameter
  输入参数:  
                             pst_chip : nand chip structure
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_void hi_nand_kernel_get_spec(struct hi_nand_chip *pst_chip)
{
    struct hi_nand_spec *pst_nand_spec;
#ifdef SUPPORT_ATP_QHB_NFF_CHECK_IMG	
	hi_uchar8 *puc_data = NULL;
	hi_uint32   ui_num_blocks;
	hi_uint32   ui_num;
    hi_uchar8   uc_temp;
	 hi_int32    i,j;
    hi_uint32   ui_addr;
    hi_int32    ui_sector_size;
	hi_uint32	total = 0;
#endif	
    //xy
    pst_nand_spec = g_pc_flash_info + 2 + FLASH_INFO_SIZE;

    hi_memcpy(&pst_chip->st_spec, pst_nand_spec, sizeof(struct hi_nand_spec));
    printk("hi_nand_kernel_get_spec come in ... \n");
    
    /* nand bad block table init */
    pst_chip->puc_bbt = (hi_uchar8 *)hi_malloc(pst_chip->st_spec.ui_bbt_len);
    if (HI_NULL == pst_chip->puc_bbt)
    {
        HI_NAND_PRINTF("nand_scan_bbt: Out of memory\n");
        return;
    }

    hi_memcpy(pst_chip->puc_bbt, (hi_uchar8 *)pst_nand_spec+sizeof(struct hi_nand_spec), pst_chip->st_spec.ui_bbt_len);
    
#ifdef SUPPORT_ATP_QHB_NFF_CHECK_IMG
	ui_sector_size = pst_chip->st_spec.ui_block_size;
    ui_num_blocks = pst_chip->st_spec.ui_chip_size>> (pst_chip->st_spec.ui_bbt_erase_shift - 1);
	printk("%s %d ui_num_blocks=%u ui_sector_size=%u \n",__func__,__LINE__,ui_num_blocks,ui_sector_size);
	puc_data = (hi_uchar8 *)pst_chip->puc_bbt;
    for(i=0;i<(ui_num_blocks>>3);i++)
    {
        for(j=0;j<4;j++)
        {
            uc_temp = puc_data[i] & (0x3<<(j*2));
            ui_num  = i*4 + j;
            if(uc_temp)
            {
                ui_addr = ui_num * ui_sector_size;
				total++;
                //hi_printk("[%08d,\t0x%08x]\n",ui_num,ui_addr);
            }
        }
    }
	iBadblockNum = total;
	printk(" debug :total = %u \n",(unsigned int)total);
#endif
    return;
}

/******************************************************************************
  函数功能:  nand setup mtd partitions
  输入参数:  
                             mtd  : lmtd device structure
                             parts: mtd patition structure
                             i_nparts : partition numbers
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_void hi_nand_setup_mtd_partitions(struct mtd_info *pst_mtd,
                            struct mtd_partition **ppst_parts, hi_int32 *i_nparts)
{
    hi_uint32 ui_active_sys; //xy
    hi_uint32 ui_upg_flag = 0;
    hi_uint32 ui_sys_type = 0;

    /* get sys type & upgflag & partition info from boot parameters */
    ui_sys_type = g_pc_flash_info[0];
    ui_upg_flag = g_pc_flash_info[1];

    if((HI_TYPE_DOUBLE==ui_sys_type) && (SLAVE_ACTIVE==ui_upg_flag))
    { 
        ui_active_sys = 1;
    }
    else
    {
        ui_active_sys = 0;
    }

    atp_setup_mtd_partitions(pst_mtd, g_pc_flash_info+2, ui_active_sys, ppst_parts, i_nparts);
}

/******************************************************************************
  函数功能:  nand get chip size
  输入参数:  无
  输出参数:  无
  函数返回:  chip size
  函数备注:  
******************************************************************************/
hi_uint32 hi_nand_get_total_size(hi_void)
{
    struct hi_nand_chip *pst_chip = g_pst_nand_host->pst_chip;
    
    return pst_chip->st_spec.ui_chip_size;
}

/******************************************************************************
  函数功能:  nand get block size
  输入参数:  无
  输出参数:  无
  函数返回:  block size
  函数备注:  
******************************************************************************/
hi_uint32 hi_nand_get_block_size(hi_void)
{
    struct hi_nand_chip *pst_chip = g_pst_nand_host->pst_chip;

    return pst_chip->st_spec.ui_block_size;
}

/******************************************************************************
  函数功能:  nand get page size
  输入参数:  无
  输出参数:  无
  函数返回:  page size
  函数备注:  
******************************************************************************/
hi_uint32 hi_nand_get_page_size(hi_void)
{
    struct hi_nand_chip *pst_chip = g_pst_nand_host->pst_chip;

    return pst_chip->st_spec.ui_page_size;
}

/******************************************************************************
  函数功能:  nand get oob size
  输入参数:  无
  输出参数:  无
  函数返回:  oob size
  函数备注:  
******************************************************************************/
hi_uint32 hi_nand_get_oob_size(hi_void)
{
    struct hi_nand_chip *pst_chip = g_pst_nand_host->pst_chip;

    return pst_chip->st_spec.ui_oob_size;
}

/******************************************************************************
  函数功能:  nand get host
  输入参数:  无
  输出参数:  无
  函数返回:  nand host
  函数备注:  
******************************************************************************/
struct hi_nand_host *hi_nand_get_nand_host(hi_void)
{
    return g_pst_nand_host;
}


/******************************************************************************
  函数功能:  nand block check bad
  输入参数:  
                             ui_addr : block absolute address
  输出参数:  无
  函数返回:  0: block not bad;    1: block is bad
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_block_isbad(hi_uint32 ui_addr)
{
    return hi_nand_bbt_block_isbad(g_pst_nand_host, ui_addr);
}

/******************************************************************************
  函数功能:  nand block mark bad
  输入参数:  
                             ui_addr : block absolute address
  输出参数:  无
  函数返回:  0:OK;  -1:NOK
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_block_markbad(hi_uint32 ui_addr)
{
    return hi_nand_bbt_block_markbad(g_pst_nand_host, ui_addr);
}

/******************************************************************************
  函数功能:  nand erase all page
  输入参数:  
                             ui_offset : start erase address
                             ui_len : erase len
  输出参数:  无
  函数返回:  0:OK;   -1:NOK
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_erase( hi_uint32 ui_offset, hi_uint32 ui_len )
{
    return hi_nand_util_erase(g_pst_nand_host, ui_offset, ui_len);
}

/******************************************************************************
  函数功能:  nand write data
  输入参数:  
                             puc_buf : data buffer
                             ui_offset : start address
                             ui_len : read len
  输出参数:  无
  函数返回: 0:OK;   -1:NOK
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_read( hi_uchar8 *puc_buf, hi_uint32 ui_offset, hi_uint32 ui_len )
{
    return hi_nand_util_read(g_pst_nand_host, puc_buf, ui_offset, ui_len);
}

/******************************************************************************
  函数功能:  nand read one whole page (include data & oob)
  输入参数:  
                             puc_dst_buf : the databuffer to put data
                             ui_src_addr : read start page addrs
                             ui_len : read len
  输出参数:  无
  函数返回:         
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_read_page(hi_uchar8 *puc_dst_buf, hi_uint32 ui_src_addr)
{
    return hi_nand_util_read_page(g_pst_nand_host, puc_dst_buf, ui_src_addr);
}

/******************************************************************************
  函数功能:  nand write data
  输入参数:  
                             puc_buf : data buffer
                             ui_offset : start address
                             ui_len : write len
  输出参数:  无
  函数返回:  0:OK;   -1:NOK
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_write( hi_uchar8 *puc_buf, hi_uint32 ui_offset, hi_uint32 ui_len )
{
    return hi_nand_util_write(g_pst_nand_host, puc_buf, ui_offset, ui_len);
}

/******************************************************************************
  函数功能:  nand write one whole page (include data & oob)
  输入参数:  
                             puc_src_buf : the databuffer to put data
                             ui_dst_addr : write start page addrs
  输出参数:  无
  函数返回:         
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_write_page(hi_uchar8 *puc_src_buf, hi_uint32 ui_dst_addr)
{
    return hi_nand_util_write_page(g_pst_nand_host, puc_src_buf, ui_dst_addr);
}

/******************************************************************************
  函数功能:  nand read oob only
  输入参数:  
                             puc_buf : oob input buffer
                             ui_offset : page absolute address
  输出参数:  无
  函数返回:  0:OK;   -1:NOK
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_oob_read( hi_uchar8 *puc_buf, hi_uint32 ui_offset)
{
    hi_uint32 ui_page_addr;
    struct hi_nand_chip *pst_chip = g_pst_nand_host->pst_chip;
    
    /* get page addr (logic address) */
    ui_page_addr  = ui_offset >> pst_chip->st_spec.ui_page_shift;

    return hi_nand_drv_read_oob(g_pst_nand_host, puc_buf, ui_page_addr);
}

/******************************************************************************
  函数功能:  nand write oob
  输入参数:  
                             puc_buf : oob output buffer
                             ui_offset : page absolute address
  输出参数:  无
  函数返回:  0:OK;   -1:NOK
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_oob_write( hi_uchar8 *puc_buf, hi_uint32 ui_offset )
{
    hi_uint32 ui_page_addr;
    struct hi_nand_chip *pst_chip = g_pst_nand_host->pst_chip;
    
    /* get page addr (logic address) */
    ui_page_addr  = ui_offset >> pst_chip->st_spec.ui_page_shift;
    
    return hi_nand_drv_write_oob(g_pst_nand_host, puc_buf, ui_page_addr);
}

/******************************************************************************
  函数功能:  display bad block table
  输入参数:  无
  输出参数:  无
  函数返回:  无
  函数备注:  
******************************************************************************/
hi_void hi_nand_dump_bbt(hi_void)
{
    hi_int32    i,j;
    hi_uint32   ui_addr;
    hi_int32    ui_sector_size;
    hi_uint32   ui_num_blocks;
    hi_uint32   ui_num;
    hi_uchar8   uc_temp;
    hi_uchar8   *puc_data = HI_NULL;
    struct hi_nand_chip *pst_chip = g_pst_nand_host->pst_chip;

    if(!pst_chip->puc_bbt)
    {
        HI_NAND_PRINTF("bbt not enable!\n");
        return;
    }

    ui_sector_size = pst_chip->st_spec.ui_block_size;
    ui_num_blocks = pst_chip->st_spec.ui_chip_size>> (pst_chip->st_spec.ui_bbt_erase_shift - 1);

    hi_printk("<num>\t\t<addr>\n");

    puc_data = (hi_uchar8 *)pst_chip->puc_bbt;
    for(i=0;i<(ui_num_blocks>>3);i++)
    {
        for(j=0;j<4;j++)
        {
            uc_temp = puc_data[i] & (0x3<<(j*2));
            ui_num  = i*4 + j;
            if(uc_temp)
            {
                ui_addr = ui_num * ui_sector_size;
                hi_printk("[%08d,\t0x%08x]\n",ui_num,ui_addr);
            }
        }
    }
    
    return;
}

/******************************************************************************
  函数功能:  Get the flash and manufacturer id and lookup if the type is supported 
  输入参数:  无
  输出参数:  无
  函数返回:  0:OK;   -1:NOK
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_chip_init(struct hi_nand_chip *pst_chip)
{
    struct hi_nand_host *pst_host = pst_chip->pv_priv;

    /* nand get flash spec info from boot parameter */
    hi_nand_kernel_get_spec(pst_chip);

    /* host transfer page type */
    switch(pst_chip->st_spec.ui_page_size)
    {
        case 0x800:
            pst_host->en_page_type = HI_NAND_PAGE_2KB_E;
            break;

        case 0x1000:
            pst_host->en_page_type = HI_NAND_PAGE_4KB_E;
            break;

        case 0x2000:
            pst_host->en_page_type = HI_NAND_PAGE_8KB_E;
            break;

        default:
            hi_printk("!!do not support pagesize 0x%x\n", pst_chip->st_spec.ui_page_size);
            return -HI_EINVAL;
    }

    /* host get ecc type */
    pst_host->en_ecc_type = pst_chip->st_spec.ui_ecc_type;

    /* malloc chip max page buffers */
    pst_chip->pst_buffers = hi_malloc(sizeof(*pst_chip->pst_buffers));
    if (HI_NULL == pst_chip->pst_buffers)
    {
        return -HI_ENOMEM;
    }

    hi_memset(pst_chip->pst_buffers, 0, sizeof(*pst_chip->pst_buffers));

    /* Set the internal oob buffer location, just after the page data */
    pst_chip->puc_oob_poi = pst_chip->pst_buffers->uac_databuf + pst_chip->st_spec.ui_page_size;

    return 0;
}
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
void ATP_get_fix_part_protect_addr(struct mtd_partition *pst_parts, hi_int32 i_nparts)
{
    hi_int32 i;

    for (i=0; i<i_nparts; i++)
    {
#ifdef SUPPORT_ATP_ATP_DEBUG_IAMGE  
        if (0 == strcmp(pst_parts[i].name,  "web"))
#else
        if (0 == strcmp(pst_parts[i].name,  "bundles"))
#endif
            g_flash_operation_info.protect_max_addr = (hi_uint32)(pst_parts[i].offset); 
     }

    printk(" s_sw_protect_addr_start end addr:%lx\n ",   g_flash_operation_info.protect_max_addr);
}
#endif
/*<End:   monitor flash opteration>*/

/******************************************************************************
  函数功能:  nand flash probe entry
  输入参数:  无
  输出参数:  无
  函数返回:  0:OK;   -1:NOK
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_probe(struct platform_device *pst_pltdev)
{
    hi_int32 i;
    hi_int32 i_ret      = 0;
    hi_int32 i_nparts   = 0;
    struct hi_nand_chip *pst_chip   = HI_NULL;
    struct hi_nand_host *pst_host   = HI_NULL;
    struct mtd_partition *pst_parts = NULL;

    /* 申请连续的内存空间，方便统一管理*/
    struct mtd_info *pst_mtd        = HI_NULL;
    hi_uint32 ui_host_size = sizeof(struct hi_nand_host) + sizeof(struct hi_nand_chip) + sizeof(struct mtd_info);

    g_pst_nand_host = pst_host = hi_malloc(ui_host_size);
    if (HI_NULL == pst_host)
    {
        return -HI_ENOMEM;
    }
    
    hi_memset((hi_char8 *)pst_host, 0, ui_host_size);

    platform_set_drvdata(pst_pltdev, pst_host);

    pst_host->pst_dev  = &pst_pltdev->dev;
    pst_host->pst_chip = pst_chip = (struct hi_nand_chip *)&pst_host[1];    //point to host+sizeof(struct hi_nand_host)
    pst_chip->pv_priv  = pst_host;

    pst_host->pst_mtd  = pst_mtd  = (struct mtd_info *)&pst_chip[1];        //point to host+sizeof(struct hi_nand_host)+sizeof(struct nand_chip)
    pst_mtd->priv      = pst_chip;
    
    pst_mtd->owner     = THIS_MODULE;
    pst_mtd->name      = pst_chip->pc_name = (hi_char8 *)(pst_pltdev->name);

    /* nandc pre_init,init the nandc buffer and reg remap */
    i_ret = hi_nand_drv_pre_init(pst_host);
    if(i_ret)
    {
        hi_free(pst_host);
        platform_set_drvdata(pst_pltdev, HI_NULL);
        return i_ret;
    }

    /* Get the flash and manufacturer id and lookup if the type is supported */
    i_ret = hi_nand_chip_init(pst_chip);
    if(i_ret)
    {
        hi_free(pst_host);
        platform_set_drvdata(pst_pltdev, HI_NULL);
        return i_ret;
    }

    /* nand controller innit */
    i_ret = hi_nand_drv_init(pst_host);
    if(i_ret)
    {
        dma_free_coherent(pst_host->pst_dev, HI_NAND_MAX_PAGE_SIZE+HI_NAND_MAX_OOB_SIZE, pst_host->puc_buffer, pst_host->ui_dma);
        pst_host->puc_buffer = HI_NULL;
        
        hi_free(pst_host);
        platform_set_drvdata(pst_pltdev, HI_NULL);
        return i_ret;
    }

    hi_nand_mtd_contact(pst_mtd);

        /* setup mtd partitions */
#ifdef CONFIG_MTD_CMDLINE_PARTS
        static const char *part_probes[] = { "cmdlinepart", HI_NULL, };
        i_nparts = parse_mtd_partitions(pst_mtd, part_probes, &pst_parts, 0);
#else
        hi_nand_setup_mtd_partitions(pst_mtd, &pst_parts, &i_nparts);
#endif
    
        if (!i_nparts)
        {
            printk("ERROR! can not find any mtd partitions!\n");
            return -HI_ENXIO;
        }
    
        for (i = 0; i < i_nparts; i++) 
        {
            HI_NAND_PRINTF("partitions[%02d]={.name = %s, .offset = 0x%.8llx, .size = 0x%.8llx (%lluKiB) }\n",
                                    i, pst_parts[i].name, pst_parts[i].offset, pst_parts[i].size, pst_parts[i].size / 1024);
        }
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
        ATP_get_fix_part_protect_addr(pst_parts, i_nparts);
#endif
/*<End:   monitor flash opteration>*/       
        i_ret = add_mtd_partitions(pst_mtd, pst_parts, i_nparts);

#ifdef SUPPORT_ATP_WLAN
//        ATP_SYS_GetInitWlanRF(); /*delete*/ /*<  monitor flash opteration>*/
#endif

    return 0;
}

static hi_int32 hi_nand_remove(struct platform_device *pst_pltdev)
{
    struct hi_nand_host *pst_host;

    pst_host = platform_get_drvdata(pst_pltdev);
    
#ifdef CONFIG_MTD_PARTITIONS
    /* Deregister partitions */
    del_mtd_partitions(pst_host->pst_mtd);
#endif
    
    /* Deregister the device */
    del_mtd_device(pst_host->pst_mtd);

    /* Free bad block table memory */
    hi_free(pst_host->pst_chip->puc_bbt);
    hi_free(pst_host->pst_chip->pst_buffers);

    hi_nand_drv_exit(pst_host);

    hi_free(pst_host);

    platform_set_drvdata(pst_pltdev, HI_NULL);
    
    return 0;
}

static hi_void hi_nand_pltdev_release(struct device *pst_dev)
{
}

static struct resource g_ast_nand_resources[] = 
{
    {
        .start  = HI_MTD_NFC_REG_BASE_ADDRESS,
        .end    = HI_MTD_NFC_REG_BASE_ADDRESS + HI_MTD_NFC_REG_BASE_ADDRESS_LEN,
        .flags  = IORESOURCE_MEM,
    },

    {
        .start  = HI_MTD_NFC_BUFFER_BASE_ADDRESS,
        .end    = HI_MTD_NFC_BUFFER_BASE_ADDRESS + HI_MTD_NFC_BUFFER_BASE_ADDRESS_LEN,
        .flags  = IORESOURCE_MEM,
    },
};

static struct platform_driver g_st_nand_pltdrv =
{
    .driver.name    = "hi_nfc",
    .probe          = hi_nand_probe,
    .remove         = hi_nand_remove,
};

static struct platform_device g_st_nand_pltdev =
{
    .name                   = "hi_nfc",
    .id                     = -1,

    .dev.platform_data      = HI_NULL,
    .dev.dma_mask           = (hi_uint32*)~0,
    .dev.coherent_dma_mask  = (hi_uint32) ~0,
    .dev.release            = hi_nand_pltdev_release,

    .num_resources          = ARRAY_SIZE(g_ast_nand_resources),
    .resource               = g_ast_nand_resources,
};

static hi_int32 __init hi_nand_init(hi_void)
{
    hi_int32 i_ret = 0;
    
    printk("HSAN Nand Flash Controller V600 Device Driver, Version 1.0\n");
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR	
    memset(&g_flash_operation_info, 0 ,sizeof(g_flash_operation_info));
    g_flash_operation_info.lock = HI_NAND_READONLY_LOCK;
#endif
/*<End:   monitor flash opteration>*/	
    i_ret = platform_driver_register(&g_st_nand_pltdrv);
    if (i_ret < 0)
    {
        return i_ret;
    }

    i_ret = platform_device_register(&g_st_nand_pltdev);
    if (i_ret < 0)
    {
        platform_driver_unregister(&g_st_nand_pltdrv);
        return i_ret;
    }

    return i_ret;
}

static hi_void __exit hi_nand_exit (hi_void)
{
    platform_driver_unregister(&g_st_nand_pltdrv);
    platform_device_unregister(&g_st_nand_pltdev);
}


module_init(hi_nand_init);
module_exit(hi_nand_exit);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

