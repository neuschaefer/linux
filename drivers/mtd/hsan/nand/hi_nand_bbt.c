/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: hi_nand_bbt.c
  功能描述: nand驱动坏块管理相关接口
  版本描述: V1.0

  创建日期: D2013_04_20

  修改记录: 
            生成初稿.
******************************************************************************/
#include <linux/mtd/mtd.h>

#include <mach/hi_typedef.h>

#include "hi_nand.h"
#include "hi_nand_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/******************************************************************************
  函数功能:  Check if block at offset is bad
  输入参数:  
                             pst_host : nand driver host structure
                             ui_block_addr : block address
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_bbt_block_isbad(struct hi_nand_host *pst_host, hi_uint32 ui_block_addr)
{
    hi_int32 i_ret;
    hi_uint32 ui_block_num;
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;

    /* Get block number * 2 */
    ui_block_num = ui_block_addr >> (pst_chip->st_spec.ui_bbt_erase_shift - 1);
    i_ret = (pst_chip->puc_bbt[ui_block_num >> 3] >> (ui_block_num & 0x06)) & 0x03;

    return i_ret;
}

/******************************************************************************
  函数功能:  Mark block at the given offset as bad
  输入参数:  
                             pst_host : nand driver host structure
                             ui_block_addr : block address
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_bbt_block_markbad(struct hi_nand_host *pst_host, hi_uint32 ui_block_addr)
{
    hi_int32  i_ret;
    hi_uint32 ui_page, ui_block_num;
    hi_uchar8 auc_oob_buf[HI_NAND_MAX_OOB_SIZE];
    hi_uint32 ui_retry_time;
    
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;

    i_ret = hi_nand_block_isbad(ui_block_addr);
    if(i_ret)
    {
        /* If it was bad already, return success and do nothing. */
        if (i_ret > 0)
        {
            return 0;
        }

        /* else return markbad fail */
        return i_ret;
    }

    /* Get block number */
    ui_block_num = ui_block_addr >> pst_chip->st_spec.ui_bbt_erase_shift;

    /* get page address */
    ui_page = ui_block_addr >> pst_chip->st_spec.ui_page_shift;

    /* update bbt */
    pst_chip->puc_bbt[ui_block_num >> 2] |= 0x01 << ((ui_block_num & 0x03) << 1);

    /* try to write bad sign to the first byte of oob to mark bad */
    hi_memset(auc_oob_buf, 0x00, HI_NAND_MAX_OOB_SIZE);

     
     /*Due to voltage spikes and other hardware reasons, the write operation may fail in this case.
     in order to prevent misuse marked bad blocks, we need to write failed retries*/
     for ( ui_retry_time = 0 ;ui_retry_time < HI_NAND_ERASE_WRITE_RETRY_TIME;ui_retry_time++)
     {
        i_ret = hi_nand_drv_erase(ui_page);
        if(i_ret && (ui_retry_time == HI_NAND_ERASE_WRITE_RETRY_TIME-1))
        {
            /* nfc erase fail that means erase return a fail status ,it is supposed to real bad block
                       in this situations we should mark bad ! */                       
            i_ret = hi_nand_drv_write_oob(pst_host, auc_oob_buf, ui_page);           
            return i_ret;
        }
        else
        {
            break;
        }
      }
    i_ret = hi_nand_drv_write_oob(pst_host, auc_oob_buf, ui_page);

    return i_ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

