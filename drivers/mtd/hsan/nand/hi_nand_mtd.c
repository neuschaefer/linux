/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: hi_nand_mtd.c
  功能描述: nand驱动对mtd接口，支撑文系统
  版本描述: V1.0

  创建日期: D2013_04_20

  修改记录: 
            生成初稿.
******************************************************************************/
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>

#include <mach/hi_typedef.h>

#include "hi_nand.h"
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
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
extern hi_nand_operate_info g_flash_operation_info;
#endif
/*<End:   monitor flash opteration>*/
static struct nand_ecclayout g_st_nand_oob_32 =
{
    .oobfree = {{2, 30}}
};
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
/******************************************************************************
  函数功能:  monitor the mtd erase/write frequency and lock it for exclusive access
  输入参数:  
                             optmod : operation mode write/erase
                             pst_mtd: MTD device structure  
                             addr     : write/erase addr
                             ulen     : write/erase lenght
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_protect(hi_uint64 addr)
{
    /*check mtd is protect area*/
    if((HI_NAND_READONLY_LOCK == g_flash_operation_info.lock) && (addr < g_flash_operation_info.protect_max_addr))
    {
        return -HI_EINVAL;
    }
    return 0;
}
#endif
/*<End:   monitor flash opteration>*/
/******************************************************************************
  函数功能:  Get the device and lock it for exclusive access
  输入参数:  
                             chip:    the nand chip descriptor
                             mtd : MTD device structure
                             new_state:    the state which is requested
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_get_device(struct hi_nand_chip *pst_chip, struct mtd_info *pst_mtd, hi_int32 i_new_state)
{
    spinlock_t *pst_lock = &pst_chip->controller->lock;
    wait_queue_head_t *pst_wq = &pst_chip->controller->wq;
    
    DECLARE_WAITQUEUE(wait, current);

retry:
    spin_lock(pst_lock);

    /* Hardware controller shared among independend devices */
    if (!pst_chip->controller->active)
    {
        pst_chip->controller->active = pst_chip;
    }

    if (pst_chip->controller->active == pst_chip && pst_chip->en_state == HI_FL_READY)
    {
        pst_chip->en_state = i_new_state;
        spin_unlock(pst_lock);
        
        return 0;
    }
    
    if (i_new_state == HI_FL_PM_SUSPENDED)
    {
        spin_unlock(pst_lock);
        
        return (pst_chip->en_state == HI_FL_PM_SUSPENDED) ? 0 : -EAGAIN;
    }
    
    set_current_state(TASK_UNINTERRUPTIBLE);
    add_wait_queue(pst_wq, &wait);
    spin_unlock(pst_lock);
    
    schedule();
    remove_wait_queue(pst_wq, &wait);
    
    goto retry;
}

/******************************************************************************
  函数功能:  Deselect, release chip lock and wake up anyone waiting on the device
  输入参数:  
                             mtd : MTD device structure
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_void hi_nand_mtd_release_device(struct mtd_info *pst_mtd)
{
    struct hi_nand_chip *pst_chip = pst_mtd->priv;

    /* Release the controller and the chip */
    spin_lock(&pst_chip->controller->lock);
    pst_chip->controller->active = HI_NULL;
    pst_chip->en_state = HI_FL_READY;
    wake_up(&pst_chip->controller->wq);
    spin_unlock(&pst_chip->controller->lock);
}

/******************************************************************************
  函数功能:  Transfer oob to client buffer
  输入参数:  
                             chip : nand chip structure
                             oob : oob data buffer
                             ops : oob ops structure
                             len : size of oob to transfer
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_uchar8 *hi_nand_mtd_transfer_oob(struct hi_nand_chip *pst_chip,
                hi_uchar8 *puc_oob, struct mtd_oob_ops *pst_oob_ops, hi_uint32 ui_len)
{
    switch(pst_oob_ops->mode)
    {
        case MTD_OPS_PLACE_OOB:
        case MTD_OPS_RAW:
            hi_memcpy(puc_oob, pst_chip->puc_oob_poi + pst_oob_ops->ooboffs, ui_len);
            return puc_oob + ui_len;

        case MTD_OPS_AUTO_OOB:
        {
            hi_uint32 ui_bytes = 0;
            hi_uint32 ui_boffs = 0, ui_roffs = pst_oob_ops->ooboffs;
            struct nand_oobfree *pst_free = pst_chip->st_ecc.layout->oobfree;
            
            for(; pst_free->length && ui_len; pst_free++, ui_len -= ui_bytes)
            {
                /* Read request not from offset 0 ? */
                if (unlikely(ui_roffs))
                {
                    if (ui_roffs >= pst_free->length)
                    {
                        ui_roffs -= pst_free->length;
                        continue;
                    }
                    
                    ui_boffs = pst_free->offset + ui_roffs;
                    ui_bytes = HI_MIN(ui_len, (pst_free->length - ui_roffs));
                    ui_roffs = 0;
                } 
                else
                {
                    ui_bytes = HI_MIN(ui_len, pst_free->length);
                    ui_boffs = pst_free->offset;
                }
                
                hi_memcpy(puc_oob, pst_chip->puc_oob_poi + ui_boffs, ui_bytes);
                puc_oob += ui_bytes;
            }
            
            return puc_oob;
        }
        
        default:
            HI_BUG(1);
            break;
    }
    
    return HI_NULL;
}

/******************************************************************************
  函数功能:  Transfer client buffer to oob
  输入参数:  
                             chip : nand chip structure
                             oob : oob data buffer
                             ops : oob ops structure
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_uchar8 *hi_nand_mtd_fill_oob(struct hi_nand_chip *pst_chip, hi_uchar8 *puc_oob, struct mtd_oob_ops *pst_oob_ops)
{
    hi_uint32 ui_len = pst_oob_ops->ooblen;

    switch(pst_oob_ops->mode)
    {
        case MTD_OPS_PLACE_OOB:
        case MTD_OPS_RAW:
        {
            hi_memcpy(pst_chip->puc_oob_poi + pst_oob_ops->ooboffs, puc_oob, ui_len);
            return puc_oob + ui_len;
        }

        case MTD_OPS_AUTO_OOB:
        {
            hi_uint32 ui_bytes = 0;
            hi_uint32 ui_boffs = 0;
            hi_uint32 ui_woffs = pst_oob_ops->ooboffs;
            struct nand_oobfree *pst_free = pst_chip->st_ecc.layout->oobfree;

            for(; pst_free->length && ui_len; pst_free++, ui_len -= ui_bytes)
            {
                /* Write request not from offset 0 ? */
                if (ui_woffs)
                {
                    if (ui_woffs >= pst_free->length)
                    {
                        ui_woffs -= pst_free->length;
                        continue;
                    }
                    
                    ui_boffs = pst_free->offset + ui_woffs;
                    ui_bytes = HI_MIN(ui_len, (pst_free->length - ui_woffs));
                    ui_woffs = 0;
                } 
                else
                {
                    ui_bytes = HI_MIN(ui_len, pst_free->length);
                    ui_boffs = pst_free->offset;
                }
                
                hi_memcpy(pst_chip->puc_oob_poi + ui_boffs, puc_oob, ui_bytes);
                puc_oob += ui_bytes;
            }
            
            return puc_oob;
        }
        
        default:
            HI_BUG(1);
            break;
    }
    
    return HI_NULL;
}

/******************************************************************************
  函数功能:  write one ore more pages
  输入参数:  
                             mtd : MTD device structure
                             to : offset to write to
                             ops : oob operations description structure
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_write_ops(struct mtd_info *pst_mtd, hi_uint64 ull_dst_addr, struct mtd_oob_ops *pst_oob_ops)
{
    hi_int32  i_ret;
    hi_uint32 ui_realpage, ui_column;
    hi_uint32 ui_writelen         = pst_oob_ops->len;
    hi_uchar8 *puc_oob            = pst_oob_ops->oobbuf;
    hi_uchar8 *puc_buf            = pst_oob_ops->datbuf;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;
    struct hi_nand_host *pst_host = pst_chip->pv_priv;

    pst_oob_ops->retlen = 0;
    
    if (0 == ui_writelen)
    {
        return 0;
    }

    /* reject writes, which are not page aligned */
    if (pst_oob_ops->len & ((1 << pst_chip->st_spec.ui_page_shift) - 1))
    {
        HI_NAND_PRINTF("nand_mtd_write: Attempt to write not page aligned data\n");
        return -HI_EINVAL;
    }

    ui_column = ull_dst_addr & (pst_mtd->writesize - 1);

    /* Check, if it is write protected */
    if (hi_nand_drv_wp_check())
    {
        return -HI_EIO;
    }

    ui_realpage = ull_dst_addr >> pst_chip->st_spec.ui_page_shift;

    /* Invalidate the page cache, when we write to the cached page */
    if (ull_dst_addr <= (pst_chip->i_page_buf << pst_chip->st_spec.ui_page_shift) 
        && (pst_chip->i_page_buf << pst_chip->st_spec.ui_page_shift) < (ull_dst_addr + pst_oob_ops->len))
    {
        pst_chip->i_page_buf = -1;
    }

    /* If we're not given explicit OOB data, let it be 0xFF */
    if (likely(!puc_oob))
    {
        hi_memset(pst_chip->puc_oob_poi, 0xff, pst_mtd->oobsize);
    }

    while(1)
    {
        hi_int32  ui_bytes  = pst_mtd->writesize;
        hi_uchar8 *puc_wbuf = puc_buf;

        /* Partial page write ? */
        if (unlikely(ui_column || ui_writelen < (pst_mtd->writesize - 1)))
        {
            ui_bytes = HI_MIN((ui_bytes - ui_column), ui_writelen);
            
            pst_chip->i_page_buf = -1;
            hi_memset(pst_chip->pst_buffers->uac_databuf, 0xff, pst_mtd->writesize);
            hi_memcpy(&pst_chip->pst_buffers->uac_databuf[ui_column], puc_buf, ui_bytes);
            
            puc_wbuf = pst_chip->pst_buffers->uac_databuf;
        }
        
        if (unlikely(puc_oob))
        {
            puc_oob = hi_nand_mtd_fill_oob(pst_chip, puc_oob, pst_oob_ops);
        }

        i_ret = hi_nand_drv_write(pst_host, puc_wbuf, ui_realpage);

        if (i_ret)
        {
            break;
        }

        ui_writelen -= ui_bytes;
        if (!ui_writelen)
        {
            break;
        }

        ui_column   = 0;
        puc_buf     += ui_bytes;
        
        ui_realpage++;
    }

    pst_oob_ops->retlen = pst_oob_ops->len - ui_writelen;
    if (unlikely(puc_oob))
    {
        pst_oob_ops->oobretlen = pst_oob_ops->ooblen;
    }
    
    return i_ret;
}

/******************************************************************************
  函数功能:  write data and/or out-of-band
  输入参数:  
                             mtd : MTD device structure
                             to : offset to write to
                             ops : oob operation description structure
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_do_write_oob(struct mtd_info *pst_mtd, 
                            hi_uint64 ull_dst_addr, struct mtd_oob_ops *pst_oob_ops)
{
    hi_int32 i_status;
    hi_uint32 ui_page, ui_len;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;
    struct hi_nand_host *pst_host = pst_chip->pv_priv;

    if (pst_oob_ops->mode == MTD_OPS_AUTO_OOB)
    {
        ui_len = pst_chip->st_ecc.layout->oobavail;
    }
    else
    {
        ui_len = pst_mtd->oobsize;
    }

    /* Do not allow write past end of page */
    if ((pst_oob_ops->ooboffs + pst_oob_ops->ooblen) > ui_len)
    {
        HI_NAND_PRINTF("nand_mtd_do_write_oob: Attempt to write past end of page\n");
        return -HI_EINVAL;
    }

    if (pst_oob_ops->ooboffs >= ui_len)
    {
        HI_NAND_PRINTF("nand_mtd_do_write_oob: Attempt to start write outside oob\n");
        return -HI_EINVAL;
    }

    /* Do not allow reads past end of device */
    if (ull_dst_addr >= pst_mtd->size 
        || (pst_oob_ops->ooboffs + pst_oob_ops->ooblen
            > ((pst_mtd->size >> pst_chip->st_spec.ui_page_shift) - (ull_dst_addr >> pst_chip->st_spec.ui_page_shift)) * ui_len))
    {
        HI_NAND_PRINTF("nand_mtd_do_write_oob: Attempt write beyond end of device\n");
        return -HI_EINVAL;
    }

    /* Shift to get page */
    ui_page = ull_dst_addr >> pst_chip->st_spec.ui_page_shift;

    /*
         * Reset the chip. Some chips (like the Toshiba TC5832DC found in one
         * of my DiskOnChip 2000 test units) will clear the whole data page too
         * if we don't do this. I have no clue why, but I seem to have 'fixed'
         * it in the doc2000 driver in August 1999.  dwmw2.
         */
    hi_nand_drv_reset();

    /* Check, if it is write protected */
    if (hi_nand_drv_wp_check())
    {
        return -HI_EIO;
    }

    /* Invalidate the page cache, if we write to the cached page */
    if (ui_page == pst_chip->i_page_buf)
    {
        pst_chip->i_page_buf = -1;
    }
    
    hi_memset(pst_chip->puc_oob_poi, 0xff, pst_mtd->oobsize);
    hi_nand_mtd_fill_oob(pst_chip, pst_oob_ops->oobbuf, pst_oob_ops);

    i_status = hi_nand_drv_write_oob(pst_host, pst_chip->puc_oob_poi, ui_page);

    if (i_status)
    {
        return i_status;
    }

    hi_memset(pst_chip->puc_oob_poi, 0xff, pst_mtd->oobsize);

    pst_oob_ops->oobretlen = pst_oob_ops->ooblen;

    return 0;
}

/******************************************************************************
  函数功能:  write data and/or out-of-band
  输入参数:  
                             mtd : MTD device structure
                             to : offset to write to
                             ops : oob operation description structure
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_write_oob(struct mtd_info *pst_mtd, hi_uint64 ull_dst_addr, struct mtd_oob_ops *pst_oob_ops)
{
    hi_int32 i_ret = 0;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;

    HI_NAND_PRINTF("nand_write_oob: to = 0x%08llx, len = %i\n", ull_dst_addr, (hi_int32)pst_oob_ops->ooblen);
    
    if((MTD_OPS_PLACE_OOB != pst_oob_ops->mode) && (MTD_OPS_AUTO_OOB != pst_oob_ops->mode) && (MTD_OPS_RAW != pst_oob_ops->mode))
    {
        HI_NAND_PRINTF("nand_write_oob: not support mtd oob mode\n");
        return -HI_EINVAL;  
    }

    /* Do not allow writes past end of device */
    if (pst_oob_ops->datbuf && (ull_dst_addr + pst_oob_ops->len) > pst_mtd->size)
    {
        HI_NAND_PRINTF("nand_write_oob: Attempt write beyond end of device\n");
        return -HI_EINVAL;
    }

    pst_oob_ops->retlen = 0;
    
    hi_nand_mtd_get_device(pst_chip, pst_mtd, HI_FL_WRITING);

    if (!pst_oob_ops->datbuf)
    {
        //i_ret = hi_nand_mtd_do_write_oob(pst_mtd, ull_dst_addr, pst_oob_ops);
        i_ret = 0;
        pst_oob_ops->oobretlen = pst_oob_ops->ooblen;
    }
    else
    {
        i_ret = hi_nand_mtd_write_ops(pst_mtd, ull_dst_addr, pst_oob_ops);
    }

    hi_nand_mtd_release_device(pst_mtd);
    
    return i_ret;
}

/******************************************************************************
  函数功能:  write one ore more pages
  输入参数:  
                             mtd : MTD device structure
                             from : offset to read from
                             len : number of bytes to read
                             retlen : pointer to variable to store the number of written bytes
                             buf : the databuffer to put data
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_write(struct mtd_info *pst_mtd, hi_uint64 ull_dst_addr, 
                              hi_uint32 ui_len, hi_uint32 *pui_retlen, const hi_uchar8 *puc_buf)
{
    hi_int32 i_ret;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;

    HI_NAND_PRINTF("nand_mtd_write: to = 0x%08llx, len = 0x%08x\n", (hi_uint64)ull_dst_addr, ui_len);
    
    /* Do not allow reads past end of device */
    if ((ull_dst_addr + ui_len) > pst_mtd->size)
    {
        return -HI_EINVAL;
    }
    
    if (!ui_len)
    {
        return 0;
    }
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
    i_ret = hi_nand_mtd_protect(ull_dst_addr);
    if(i_ret)
    {
        printk("read only area, can't write!\r\n");
        return -HI_EINVAL;
    }
#endif
/*<End:   monitor flash opteration>*/      
    hi_nand_mtd_get_device(pst_chip, pst_mtd, HI_FL_WRITING);

    pst_chip->st_ops.len    = ui_len;
    pst_chip->st_ops.datbuf = (hi_uchar8 *)puc_buf;
    pst_chip->st_ops.oobbuf = HI_NULL;

    i_ret = hi_nand_mtd_write_ops(pst_mtd, ull_dst_addr, &pst_chip->st_ops);

    *pui_retlen = pst_chip->st_ops.retlen;

    hi_nand_mtd_release_device(pst_mtd);

    return i_ret;
}

/******************************************************************************
  函数功能:  read out-of-band data from the spare area
  输入参数:  
                             mtd : MTD device structure
                             from : offset to read from
                             ops : oob operation description structure
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_do_read_oob(struct mtd_info *pst_mtd, hi_uint64 ull_src_addr, struct mtd_oob_ops *pst_oob_ops)
{
    hi_uint32 ui_len, ui_realpage;
    hi_uint32 ui_readlen = pst_oob_ops->ooblen;
    hi_uchar8 *puc_buf = pst_oob_ops->oobbuf;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;
    struct hi_nand_host *pst_host = pst_chip->pv_priv;

    HI_NAND_PRINTF("nand_mtd_do_read_oob: from = 0x%08llx, len = %i\n", (hi_uint64)ull_src_addr, ui_readlen);

    if (pst_oob_ops->mode == MTD_OPS_AUTO_OOB)
    {
        ui_len = pst_chip->st_ecc.layout->oobavail;
    }
    else
    {
        ui_len = pst_mtd->oobsize;
    }

    if (pst_oob_ops->ooboffs >= ui_len)
    {
        HI_NAND_PRINTF("nand_mtd_do_read_oob: Attempt to start read outside oob\n");
        return -HI_EINVAL;
    }

    /* Do not allow reads past end of device */
    if (ull_src_addr >= pst_mtd->size 
        || (pst_oob_ops->ooboffs+ui_readlen
            > ((pst_mtd->size >> pst_chip->st_spec.ui_page_shift)-(ull_src_addr >> pst_chip->st_spec.ui_page_shift))*ui_len))
    {
        HI_NAND_PRINTF("nand_mtd_do_read_oob: Attempt read beyond end of device\n");
        return -HI_EINVAL;
    }
    
    /* Shift to get page */
    ui_realpage = (hi_int32)(ull_src_addr >> pst_chip->st_spec.ui_page_shift);

    while(1) 
    {
        hi_nand_drv_read_oob(pst_host, pst_chip->puc_oob_poi, ui_realpage);
     
        ui_len = HI_MIN(ui_len, ui_readlen);
        puc_buf = hi_nand_mtd_transfer_oob(pst_chip, puc_buf, pst_oob_ops, ui_len);

        ui_readlen -= ui_len;
        if (!ui_readlen)
        {
            break;
        }

        /* Increment page address */
        ui_realpage++;
    }

    pst_oob_ops->oobretlen = pst_oob_ops->ooblen;
    
    return 0;
}

/******************************************************************************
  函数功能:  read whole page (&oob)
  输入参数:  
                             mtd : MTD device structure
                             from : offset to read from
                             ops : oob ops structure
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_read_ops(struct mtd_info *pst_mtd, hi_uint64 ull_src_addr, struct mtd_oob_ops *pst_oob_ops)
{
    hi_int32  i_ret;

    hi_int32  i_ecc_invalid = 0;

    hi_uchar8 *puc_bufpoi;
    hi_uint32 ui_realpage, ui_offset, ui_read_bytes, ui_aligned;
    hi_uint32 ui_readlen    = pst_oob_ops->len;
    hi_uint32 ui_oobreadlen = pst_oob_ops->ooblen;
    hi_uchar8 *puc_buf      = pst_oob_ops->datbuf;
    hi_uchar8 *puc_oob      = pst_oob_ops->oobbuf;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;
    struct hi_nand_host *pst_host = pst_chip->pv_priv;
    hi_uchar8 *puc_pagebuf   = pst_chip->pst_buffers->uac_databuf;
    hi_uchar8 *puc_page_temp = HI_NULL;
    
    ui_realpage = (hi_uint32)(ull_src_addr >> pst_chip->st_spec.ui_page_shift);
    ui_offset   = (hi_uint32)(ull_src_addr & (pst_mtd->writesize - 1));

    puc_page_temp = (hi_uchar8 *)hi_malloc(pst_mtd->writesize + pst_mtd->oobsize);
    if(HI_NULL == puc_page_temp)
    {
        HI_NAND_PRINTF("nand_mtd_read_ops: mallco page buffer error!\n");
        return -HI_ENOMEM;
    }

    while(ui_readlen) 
    {
        ui_read_bytes = HI_MIN(pst_mtd->writesize - ui_offset, ui_readlen);
        ui_aligned = (ui_read_bytes == pst_mtd->writesize);
        
        /* Is the current page in the buffer ? */
        if (ui_realpage != pst_chip->i_page_buf || puc_oob)
        {
            puc_bufpoi = ui_aligned ? puc_page_temp : puc_pagebuf;
            
            i_ret = hi_nand_drv_read(pst_host, puc_bufpoi, ui_realpage);
            if(-EBADMSG == i_ret)
            {
                i_ecc_invalid ++;
            }

            /* Transfer not aligned data */
            if (!ui_aligned) 
            {
                if (!puc_oob)
                {
                    pst_chip->i_page_buf = ui_realpage;
                }

                hi_memcpy(puc_buf, puc_pagebuf + ui_offset, ui_read_bytes);
            }
            else
            {
                hi_memcpy(puc_buf, puc_page_temp, ui_read_bytes);
            }

            puc_buf += ui_read_bytes;

            if (unlikely(puc_oob)) 
            {
                /* Raw mode does data:oob:data:oob */
                if (pst_oob_ops->mode != MTD_OPS_RAW)
                {
                    hi_uint32 ui_toread = HI_MIN(ui_oobreadlen, pst_chip->st_ecc.layout->oobavail);
                    
                    if (ui_toread) 
                    {
                        puc_oob = hi_nand_mtd_transfer_oob(pst_chip, puc_oob, pst_oob_ops, ui_toread);
                        
                        ui_oobreadlen -= ui_toread;
                    }
                }
                else
                {
                    puc_buf = hi_nand_mtd_transfer_oob(pst_chip, puc_buf, pst_oob_ops, pst_mtd->oobsize);
                }
            }
        } 
        else 
        {
            hi_memcpy(puc_buf, puc_pagebuf + ui_offset, ui_read_bytes);
            puc_buf += ui_read_bytes;
        }

        ui_readlen -= ui_read_bytes;
        ui_offset  = 0;         /* For subsequent reads align to page boundary. */
        ui_realpage++;          /* Increment page address */
    }

    hi_free(puc_page_temp);
    
    pst_oob_ops->retlen = pst_oob_ops->len - ui_readlen;
    if (puc_oob)
    {
        pst_oob_ops->oobretlen = pst_oob_ops->ooblen - ui_oobreadlen;
    }
    if(i_ecc_invalid)
    {
        return -EBADMSG;
    }

    return 0;
}

/******************************************************************************
  函数功能:  read data and/or out-of-band
  输入参数:  
                             mtd : MTD device structure
                             from : offset to read from
                             ops : oob operation description structure
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_read_oob(struct mtd_info *pst_mtd, hi_uint64 ull_src_addr, struct mtd_oob_ops *pst_oob_ops)
{
    hi_int32 i_ret = 0;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;

    if((MTD_OPS_PLACE_OOB != pst_oob_ops->mode) && (MTD_OPS_AUTO_OOB != pst_oob_ops->mode) && (MTD_OPS_RAW != pst_oob_ops->mode))
    {
        HI_NAND_PRINTF("nand_read_oob: not support mtd oob mode\n");
        return -HI_EINVAL;  
    }

    /* Do not allow reads past end of device */
    if (pst_oob_ops->datbuf && (ull_src_addr + pst_oob_ops->len) > pst_mtd->size) 
    {
        HI_NAND_PRINTF("nand_read_oob: Attempt read beyond end of device\n");
        return -HI_EINVAL;
    }

    pst_oob_ops->retlen = 0;
    
    hi_nand_mtd_get_device(pst_chip, pst_mtd, HI_FL_READING);

    if (!pst_oob_ops->datbuf)
    {
        i_ret = hi_nand_mtd_do_read_oob(pst_mtd, ull_src_addr, pst_oob_ops);
    }
    else
    {
        i_ret = hi_nand_mtd_read_ops(pst_mtd, ull_src_addr, pst_oob_ops);
    }

    hi_nand_mtd_release_device(pst_mtd);
    
    return i_ret;
}

/******************************************************************************
  函数功能:  read one ore more pages
  输入参数:  
                             mtd : MTD device structure
                             from : offset to read from
                             len : number of bytes to read
                             retlen : pointer to variable to store the number of read bytes
                             buf : the databuffer to put data
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_read(struct mtd_info *pst_mtd, hi_uint64 ull_src_addr, 
                                    hi_uint32 ui_len, hi_uint32 *pui_retlen, hi_uchar8 *puc_buf)
{
    hi_int32 i_ret;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;

    HI_NAND_PRINTF("nand_mtd_read: from = 0x%08llx, len = 0x%08x\n", (hi_uint64)ull_src_addr, ui_len);
    
    /* Do not allow reads past end of device */
    if ((ull_src_addr + ui_len) > pst_mtd->size)
    {
        HI_NAND_PRINTF("nand_mtd_read: read past end of device\n");
        return -HI_EINVAL;
    }

    if (!ui_len)
    {
        return 0;
    }

    hi_nand_mtd_get_device(pst_chip, pst_mtd, HI_FL_READING);

    pst_chip->st_ops.len = ui_len;
    pst_chip->st_ops.datbuf = puc_buf;
    pst_chip->st_ops.oobbuf = HI_NULL;

    /* read page data without oob */
    i_ret = hi_nand_mtd_read_ops(pst_mtd, ull_src_addr, &pst_chip->st_ops);

    *pui_retlen = pst_chip->st_ops.retlen;

    hi_nand_mtd_release_device(pst_mtd);

    return i_ret;
}

/******************************************************************************
  函数功能:  Erase one ore more blocks
  输入参数:  
                             mtd : MTD device structure
                             instr : erase instruction
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_erase(struct mtd_info *pst_mtd, struct erase_info *pst_erase_info)
{
    hi_int32 i_ret;
    hi_uint32 ui_page, ui_pages_per_block;
    hi_uint64 ull_len;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;

    HI_NAND_PRINTF("nand_mtd_erase: start = 0x%08llx, len = 0x%08llx\n", (hi_uint64)pst_erase_info->addr, (hi_uint64)pst_erase_info->len);
/*<Start:   monitor flash opteration>*/
#ifdef SUPPORT_ATP_FLASH_MONITOR
    i_ret = hi_nand_mtd_protect(pst_erase_info->addr);
    if(i_ret)
    {
        printk("read only area, can't erase!\r\n");
        return -HI_EINVAL;
    }
#endif
/*<End:   monitor flash opteration>*/
    /* Start address must align on block boundary */
    if (pst_erase_info->addr & ((1 << pst_chip->st_spec.ui_erase_shift) - 1))
    {
        HI_NAND_PRINTF("nand_mtd_erase: Unaligned address\n");
        return -HI_EINVAL;
    }

    /* Length must align on block boundary */
    if (pst_erase_info->len & ((1 << pst_chip->st_spec.ui_erase_shift) - 1))
    {
        HI_NAND_PRINTF("nand_mtd_erase: Length not block aligned\n");
        return -HI_EINVAL;
    }

    /* Do not allow erase past end of device */
    if ((pst_erase_info->len + pst_erase_info->addr) > pst_mtd->size)
    {
        HI_NAND_PRINTF("nand_mtd_erase: Erase past end of device\n");
        return -HI_EINVAL;
    }

    pst_erase_info->fail_addr = MTD_FAIL_ADDR_UNKNOWN;

    /* Grab the lock and see if the device is available */
    hi_nand_mtd_get_device(pst_chip, pst_mtd, HI_FL_ERASING);

    /* Shift to get first page */
    ui_page = (hi_int32)(pst_erase_info->addr >> pst_chip->st_spec.ui_page_shift);

    /* Calculate pages in each block */
    ui_pages_per_block = 1 << (pst_chip->st_spec.ui_erase_shift - pst_chip->st_spec.ui_page_shift);

    /* Check, if it is write protected */
    if (hi_nand_drv_wp_check()) 
    {
        HI_NAND_PRINTF("nand_mtd_erase: Device is write protected!!!\n");
        pst_erase_info->state = MTD_ERASE_FAILED;
        goto erase_exit;
    }

    /* Loop through the pages */
    ull_len = pst_erase_info->len;

    pst_erase_info->state = MTD_ERASING;

    while(ull_len) 
    {
        /* heck if we have a bad block, we do not erase bad blocks ! */
        if(hi_nand_block_isbad(ui_page << pst_chip->st_spec.ui_page_shift))
        {
            hi_printk("nand_erase: attempt to erase a bad block at page 0x%08x\n", ui_page);
            pst_erase_info->state = MTD_ERASE_FAILED;
            goto erase_exit;
        }

        /* Invalidate the page cache, if we erase the block which contains the current cached page */
        if (ui_page <= pst_chip->i_page_buf && pst_chip->i_page_buf < (ui_page + ui_pages_per_block))
        {
            pst_chip->i_page_buf = -1;
        }

        i_ret = hi_nand_drv_erase(ui_page);
        
        /* See if block erase succeeded */
        if (i_ret)
        {
            HI_NAND_PRINTF("nand_mtd_erase: Failed erase, page 0x%08x\n", ui_page);
            pst_erase_info->state = MTD_ERASE_FAILED;
            pst_erase_info->fail_addr = ((hi_uint64)ui_page << pst_chip->st_spec.ui_page_shift);
            goto erase_exit;
        }

        /* Increment page address and decrement length */
        ull_len -= (1 << pst_chip->st_spec.ui_erase_shift);
        ui_page += ui_pages_per_block;
    }
    
    pst_erase_info->state = MTD_ERASE_DONE;

erase_exit:

    i_ret = pst_erase_info->state == MTD_ERASE_DONE ? 0 : -HI_EIO;

    /* Deselect and wake up anyone waiting on the device */
    hi_nand_mtd_release_device(pst_mtd);

    /* Do call back function */
    if (!i_ret)
    {
        mtd_erase_callback(pst_erase_info);
    }

    /* Return more or less happy */
    return i_ret;
}

/******************************************************************************
  函数功能:  Check if block at offset is bad
  输入参数:  
                             pst_host : nand driver host structure
                             ofs : offset relative to mtd start
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_block_isbad(struct mtd_info *pst_mtd, hi_uint64 ull_block_addr)
{
    return hi_nand_block_isbad((hi_uint32)ull_block_addr);
}

/******************************************************************************
  函数功能:  Mark block at the given offset as bad
  输入参数:  
                             mtd : MTD device structure
                             ofs : offset relative to mtd start
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_mtd_block_markbad(struct mtd_info *pst_mtd, hi_uint64 ull_block_addr)
{
    return hi_nand_block_markbad((hi_uint32)ull_block_addr);
}

/******************************************************************************
  函数功能:  NAND device init
  输入参数:  
                             mtd : MTD device structure
  输出参数:  无
  函数返回:  uint       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_mtd_contact(struct mtd_info *pst_mtd)
{
    hi_int32 i;
    struct hi_nand_chip *pst_chip = pst_mtd->priv;

    if (!pst_chip->controller)
    {
        pst_chip->controller = &pst_chip->hwcontrol;
        spin_lock_init(&pst_chip->controller->lock);
        init_waitqueue_head(&pst_chip->controller->wq);
    }

    pst_chip->st_ecc.layout = &g_st_nand_oob_32;
    pst_chip->i_page_buf    = -1;             /* Invalidate the pagebuffer reference */
    pst_chip->en_state      = HI_FL_READY;    /* Initialize state */
    pst_chip->st_ecc.layout->oobavail = 0;    /* The number of bytes available for a client to place data into  the out of band area */
    
    for(i = 0; pst_chip->st_ecc.layout->oobfree[i].length; i++)
    {
        pst_chip->st_ecc.layout->oobavail += pst_chip->st_ecc.layout->oobfree[i].length;
    }

    pst_mtd->name            = pst_chip->pc_name;
    pst_mtd->size            = pst_chip->st_spec.ui_chip_size;
    pst_mtd->erasesize       = pst_chip->st_spec.ui_block_size;
    pst_mtd->writesize       = pst_chip->st_spec.ui_page_size;
    pst_mtd->writebufsize    = pst_mtd->writesize;
    pst_mtd->oobsize         = pst_chip->st_spec.ui_oob_size;
    pst_mtd->writesize_shift = pst_chip->st_spec.ui_page_shift;
    pst_mtd->erasesize_shift = pst_chip->st_spec.ui_erase_shift;
    pst_mtd->oobavail        = pst_chip->st_ecc.layout->oobavail;
    pst_mtd->ecclayout       = pst_chip->st_ecc.layout;          /* propagate ecc.layout to mtd_info */

    /* Fill in remaining MTD driver data */
    pst_mtd->type            = MTD_NANDFLASH;
    pst_mtd->flags           = MTD_CAP_NANDFLASH;
    
    pst_mtd->_erase          = hi_nand_mtd_erase;
    pst_mtd->_read           = hi_nand_mtd_read;
    pst_mtd->_write          = hi_nand_mtd_write;
    pst_mtd->_read_oob       = hi_nand_mtd_read_oob;
    pst_mtd->_write_oob      = hi_nand_mtd_write_oob;
    pst_mtd->_block_isbad    = hi_nand_mtd_block_isbad;
    pst_mtd->_block_markbad  = hi_nand_mtd_block_markbad;
    
    pst_mtd->_lock           = HI_NULL;
    pst_mtd->_unlock         = HI_NULL;
    pst_mtd->_point          = HI_NULL;
    pst_mtd->_unpoint        = HI_NULL;

    /* Build bad block table */
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

