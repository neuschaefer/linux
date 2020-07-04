/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  Filename : hi_mtd_parts.c
  Function : 
  
  Date     : D2014_01_03
  
  Version  : V1.0
  Record   : 
             create file.
******************************************************************************/
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <mach/hi_typedef.h>
#include <mach/hi_flash.h>

static char g_ac_mtd_name[HI_MAX_MTD_NR][HI_NAME_LEN];
static struct mtd_partition g_ast_nand_parts[HI_MAX_MTD_NR];

int hi_setup_mtd_partitions( struct mtd_info *mtd,
                             const unsigned char *puc_boot_param, 
                             struct mtd_partition **ppst_parts,
                             unsigned int *pui_mtd_num)
{
    unsigned int ui_mtd_nr = 0;
    unsigned int ui_index  = 0;
    hi_mtd_s *pst_mtd      = NULL;

    if ((NULL == mtd) || (NULL == ppst_parts) || (NULL == pui_mtd_num) || (NULL == puc_boot_param))
    {
        return -1;
    }

    ui_mtd_nr = puc_boot_param[0];
    pst_mtd   = puc_boot_param + 1;
    
    printk("mtd partition tables[%d]:\n", ui_mtd_nr);
    for(ui_index = 0; ui_index<ui_mtd_nr; ui_index++)
    {
        printk("addr:0x%08x, len:0x%08x name:%s\n", pst_mtd[ui_index].ui_mtd_addr,
                pst_mtd[ui_index].ui_mtd_size, pst_mtd[ui_index].ac_mtd_name);
        
        memcpy(g_ac_mtd_name[ui_index], pst_mtd[ui_index].ac_mtd_name, HI_NAME_LEN-1);
        g_ac_mtd_name[ui_index][HI_NAME_LEN-1] = '\0';
        g_ast_nand_parts[ui_index].name        = g_ac_mtd_name[ui_index];
        g_ast_nand_parts[ui_index].offset      = pst_mtd[ui_index].ui_mtd_addr;
        g_ast_nand_parts[ui_index].size        = pst_mtd[ui_index].ui_mtd_size;
        g_ast_nand_parts[ui_index].ecclayout   = mtd->ecclayout;
    } 

    *pui_mtd_num = ui_mtd_nr;
    *ppst_parts  = g_ast_nand_parts;

    return 0;
}

