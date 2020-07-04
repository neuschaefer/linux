/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: hi_nand_drv.c
  功能描述: nand核心驱动-- 控制器(nandc)驱动
  版本描述: V1.0

  创建日期: D2013_04_20

  修改记录: 
            生成初稿.
******************************************************************************/
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/dma-mapping.h>
#include <linux/mtd/mtd.h>

#include <mach/hi_typedef.h>
#include <mach/hi_com.h>
#include <mach/hi_flash.h>

#include "hi_nand.h"
#include "hi_nand_drv.h"
#include "atpconfig.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef FLASH_GPIO_WRITE_PROTECT
#define CONFIG_FMC_NAND_FLASH_WP_PIN FLASH_WP_PIN

extern void sd5610x_iomux_set_gpio_mode(unsigned int gpio);
extern int sd5610x_gpio_bit_attr_set(unsigned int gpio, unsigned int mode);
extern int sd5610x_gpio_bit_write(unsigned int gpio, unsigned int level);

static void nand_wp_enable(void)
{
    sd5610x_gpio_bit_write(CONFIG_FMC_NAND_FLASH_WP_PIN, 0);
}

static void nand_wp_disable(void)
{
    sd5610x_gpio_bit_write(CONFIG_FMC_NAND_FLASH_WP_PIN, 1);
}
#endif


/* global nandc reg define */
static hi_nfc_reg_s *g_pst_nfc_reg;

#if 0    //nfc v300
static struct hi_nand_oob_size g_ast_nand_oob_size[] = 
{
    {HI_NAND_ECC_1BIT_E , HI_NAND_PAGE_2KB_E, 64},
    {HI_NAND_ECC_1BIT_E , HI_NAND_PAGE_4KB_E, 56},
    
    {HI_NAND_ECC_4BIT_E , HI_NAND_PAGE_2KB_E, 36},
    {HI_NAND_ECC_4BIT_E , HI_NAND_PAGE_4KB_E, 48},
    
    {HI_NAND_ECC_16BIT_E, HI_NAND_PAGE_2KB_E, 8 },
    {HI_NAND_ECC_16BIT_E, HI_NAND_PAGE_4KB_E, 32},
    
    {HI_NAND_ECC_24BIT_E, HI_NAND_PAGE_2KB_E, 32},
    {HI_NAND_ECC_24BIT_E, HI_NAND_PAGE_4KB_E, 32},
    {HI_NAND_ECC_24BIT_E, HI_NAND_PAGE_8KB_E, 32},
    {0, 0, 0}
};

/******************************************************************************
  函数功能:  get oob page size
  输入参数:  
                              pst_host : nand driver host structure
  输出参数:  无
  函数返回:  成功:oobsize      失败:0   
  函数备注:  
******************************************************************************/
hi_uint32 hi_nand_drv_get_oob_size(struct hi_nand_host *pst_host)
{
    struct hi_nand_oob_size *pst_nand_oob_size;

    for(pst_nand_oob_size = g_ast_nand_oob_size; pst_nand_oob_size->ui_ecc_type; pst_nand_oob_size++)
    {
        if((pst_nand_oob_size->ui_ecc_type == pst_host->en_ecc_type) && (pst_nand_oob_size->ui_page_type == pst_host->en_page_type))
        {
            return pst_nand_oob_size->ui_oob_size;
        }
    }

    HI_BUG(1);
    
    return 0;
}
#endif

#if 1    //nfc v500
static struct hi_nand_oob_offset g_ast_nand_oob_offset[] = 
{
    {HI_NAND_ECC_4BIT_E , HI_NAND_PAGE_2KB_E, 0, 1056},
    {HI_NAND_ECC_4BIT_E , HI_NAND_PAGE_4KB_E, 0, 3138},
    
    {HI_NAND_ECC_8BIT_E , HI_NAND_PAGE_2KB_E, 0, 1056},
    {HI_NAND_ECC_8BIT_E , HI_NAND_PAGE_4KB_E, 0, 3168},
    {HI_NAND_ECC_8BIT_E , HI_NAND_PAGE_2KB_E, 1, 1068},
    {HI_NAND_ECC_8BIT_E , HI_NAND_PAGE_4KB_E, 1, 3180},
    
    {HI_NAND_ECC_24BIT_E, HI_NAND_PAGE_2KB_E, 0, 1082},
    {HI_NAND_ECC_24BIT_E, HI_NAND_PAGE_4KB_E, 0, 3222},
    {HI_NAND_ECC_24BIT_E, HI_NAND_PAGE_8KB_E, 0, 7490},
    {0, 0, 0, 0}
};

/******************************************************************************
  函数功能:  get oob page inner offset for bus read oob only mode
  输入参数:  
                              pst_host : nand driver host structure
  输出参数:  无
  函数返回:  offset       
  函数备注:  
******************************************************************************/
static hi_uint32 hi_nand_drv_get_oob_offset(struct hi_nand_host *pst_host)
{
    hi_uint32 ui_reg_value = 0;
    struct hi_nand_oob_offset *pst_nand_oob_offset;

    HI_REG_READ(&g_pst_nfc_reg->ui_nfc_oob_sel, ui_reg_value);
    
    for(pst_nand_oob_offset = g_ast_nand_oob_offset; pst_nand_oob_offset->ui_ecc_type; pst_nand_oob_offset++)
    {
        if((pst_nand_oob_offset->ui_ecc_type == pst_host->en_ecc_type) && (pst_nand_oob_offset->ui_page_type == pst_host->en_page_type))
        {
            if((HI_NAND_ECC_8BIT_E == pst_host->en_ecc_type) && (pst_nand_oob_offset->ui_oob_sel != (ui_reg_value & 0x1)))
            {
                continue;
            }
            else
            {
                return pst_nand_oob_offset->ui_oob_offset;
            }
        }
    }

    HI_BUG(1);

    return 0;
}
#endif

/******************************************************************************
  函数功能:  wait for nand status ready or time out
  输入参数:  无
  输出参数:  无
  函数返回:  ui_nf_status       
  函数备注:  
******************************************************************************/
static inline hi_int32 hi_nand_drv_wait_ready(hi_void)
{
    hi_nfc_status_u un_reg_nfc_status;

    hi_memset(&un_reg_nfc_status, 0, sizeof(hi_nfc_status_u));

    HI_REG_READ(&g_pst_nfc_reg->ui_nfc_status, un_reg_nfc_status.ui_value);
    while(HI_TRUE != un_reg_nfc_status.st_bits.ui_nfc_ready)
    {
        HI_REG_READ(&g_pst_nfc_reg->ui_nfc_status, un_reg_nfc_status.ui_value);
    }

    return un_reg_nfc_status.st_bits.ui_nf_status;
}

/******************************************************************************
  函数功能:  nand controller set nfc_cmd register
  输入参数:  
                             ui_cmd1 : cmd1
                             ui_cmd2 : cmd2
                             ui_cmd_status : cmd status
  输出参数:  无
  函数返回:  无       
  函数备注:  
******************************************************************************/
static hi_void hi_nand_drv_reg_set_cmd(hi_uint32 ui_cmd1, hi_uint32 ui_cmd2, hi_uint32 ui_cmd_status)
{
    hi_nfc_cmd_u un_reg_nfc_cmd;

    hi_memset(&un_reg_nfc_cmd, 0, sizeof(hi_nfc_cmd_u));

    un_reg_nfc_cmd.st_bits.ui_cmd1 = ui_cmd1;
    un_reg_nfc_cmd.st_bits.ui_cmd2 = ui_cmd2;
    un_reg_nfc_cmd.st_bits.ui_read_status_cmd = ui_cmd_status;

    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_cmd, un_reg_nfc_cmd.ui_value);

    return;
}

/******************************************************************************
  函数功能:  nand controller dma read/write
  输入参数:  
                             ui_dst_addr : read/write address
                             en_op_type : operation type read/write/read oob
  输出参数:  无
  函数返回:  0:OK;  -1:NOK,ecc err invalid     
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_drv_dma_transfer(struct hi_nand_host *pst_host, hi_uint32 ui_addr, HI_NAND_OP_E en_op_type)
{
    hi_nfc_ints_u     un_reg_nfc_ints;
    hi_nfc_addrl_u    un_reg_nfc_addrl;
    hi_nfc_addrh_u    un_reg_nfc_addrh;
    hi_nfc_dma_ctrl_u un_reg_nfc_dma_ctrl;
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;
    hi_int32 i_status;

    hi_memset(&un_reg_nfc_ints,     0, sizeof(hi_nfc_ints_u)    );
    hi_memset(&un_reg_nfc_addrl,    0, sizeof(hi_nfc_addrl_u)   );
    hi_memset(&un_reg_nfc_addrh,    0, sizeof(hi_nfc_addrh_u)   );
    hi_memset(&un_reg_nfc_dma_ctrl, 0, sizeof(hi_nfc_dma_ctrl_u));
    
    if((HI_NAND_READ_E == en_op_type) || (HI_NAND_READ_OOB_E == en_op_type))
    {
        hi_nand_drv_reg_set_cmd(0, HI_NAND_CMD_READSTART, 0);
    }
    else
    {
        hi_nand_drv_reg_set_cmd(HI_NAND_CMD_SEQIN, HI_NAND_CMD_PAGEPROG, HI_NAND_CMD_STATUS);
    }

    /* set read/write addr */
    un_reg_nfc_addrl.st_rw_bits.ui_page_addr = ui_addr&0xffff;
    un_reg_nfc_addrh.st_rw_bits.ui_addr_h    = ui_addr>>16;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_addrl, un_reg_nfc_addrl.ui_value);
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_addrh, un_reg_nfc_addrh.ui_value);

    /* enable all irq */
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_inten, HI_NFC_INTERRUPT_MASK);

    /* clear all irq */
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_intcrl, HI_NFC_INTERRUPT_MASK);

    /* set nfc_dma_ctrl reg to start dma transfer */
    un_reg_nfc_dma_ctrl.st_bits.ui_burst4_en      = HI_TRUE;
    un_reg_nfc_dma_ctrl.st_bits.ui_burst8_en      = HI_TRUE;
    un_reg_nfc_dma_ctrl.st_bits.ui_burst16_en     = HI_TRUE;
    un_reg_nfc_dma_ctrl.st_bits.ui_dma_start      = HI_TRUE;
    un_reg_nfc_dma_ctrl.st_bits.ui_dma_addr_num   = HI_NAND_ADDR_NUM_5CYCLE_E;

#if 1    //nfc v500    
    if(HI_NAND_READ_OOB_E == en_op_type)
    {
        un_reg_nfc_dma_ctrl.st_bits.ui_dma_rd_oob = HI_TRUE;
    }
#endif

#if 0    //nfc v300
    /* 5115的DMA传输支持随机读导致DMA基地址寄存器易变，需要每次读写前重新配置一下*/
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_baddr_d,  (hi_uint32)pst_host->ui_dma       );
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_baddr_d1, (hi_uint32)pst_host->ui_dma_extern);
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_baddr_oob,(hi_uint32)pst_host->ui_dma_oob   );

    /* set nfc_rd_logic_addr & nfc_rd_logoc_len reg这两个寄存器也是易变的 */
    if(HI_NAND_READ_OOB_E == en_op_type)
    {
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_rd_logic_addr, pst_chip->st_spec.ui_page_size);
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_rd_logic_len, pst_chip->st_spec.ui_oob_size);
    }
    else
    {
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_rd_logic_addr, 0);
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_rd_logic_len, pst_chip->st_spec.ui_page_size + pst_chip->st_spec.ui_oob_size);
    }
#endif

    if(HI_NAND_WRITE_E == en_op_type)
    {
        un_reg_nfc_dma_ctrl.st_bits.ui_dma_wr_en  = HI_TRUE;
    }
    else
    {
        un_reg_nfc_dma_ctrl.st_bits.ui_dma_wr_en  = HI_FALSE;
    }

    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_dma_ctrl, un_reg_nfc_dma_ctrl.ui_value);

    /* wait for dma transfer finish */
    HI_REG_READ(&g_pst_nfc_reg->ui_nfc_dma_ctrl, un_reg_nfc_dma_ctrl.ui_value);
    while(HI_TRUE == un_reg_nfc_dma_ctrl.st_bits.ui_dma_start)
    {
        HI_REG_READ(&g_pst_nfc_reg->ui_nfc_dma_ctrl, un_reg_nfc_dma_ctrl.ui_value);
        _cond_resched();    /* 禁止进程抢断 */
    }

    /* read ints status to check dma result */
    HI_REG_READ(&g_pst_nfc_reg->ui_nfc_ints, un_reg_nfc_ints.ui_value);
    if(un_reg_nfc_ints.st_bits.ui_err_invalid)
    {
        HI_NAND_PRINTF("error! ecc err invalid!\n");
        if((HI_NAND_READ_OOB_E == en_op_type)||(HI_NAND_READ_E == en_op_type))
        {
            return -EBADMSG; 
        }
        return -HI_EIO;
    }

    /*read nand device status*/
    if (HI_NAND_WRITE_E == en_op_type)
    {
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op, 1); 
        
        /* wait for ready*/
        i_status = hi_nand_drv_wait_ready();
        
        if(HI_NAND_STATUS_FAIL & i_status)
        {
            hi_printk("write addr 0x%x error!\n",ui_addr << pst_chip->st_spec.ui_page_shift);
            return -HI_EIO;
        }
    }

    return 0;
}

#if 1    //nfc v500
/******************************************************************************
  函数功能:  nand bus read/write/read oob only
  输入参数:  
                             pst_host : nand driver host structure
                             ui_dst_addr : read/write address
                             en_op_type : operation type read/write/read oob
  输出参数:  无
  函数返回:  0:OK;  -1:NOK,ecc err invalid
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_drv_bus_rw(struct hi_nand_host *pst_host, hi_uint32 ui_addr, HI_NAND_OP_E en_op_type)
{
    hi_uint32        ui_backup_con_reg = 0;
    
    hi_nfc_op_u      un_reg_nfc_op;
    hi_nfc_con_u     un_reg_nfc_con;
    hi_nfc_ints_u    un_reg_nfc_ints;
    hi_nfc_addrl_u   un_reg_nfc_addrl;
    hi_nfc_addrh_u   un_reg_nfc_addrh;
    hi_nfc_op_para_u un_reg_nfc_op_para;
    
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;

    /* bus方式只读oob不稳定，只影响page0只读oob, 取消该功能，避免异常打印 */
    if(HI_NAND_READ_OOB_E == en_op_type)
    {
        return 0;
    }
	
    hi_memset(&un_reg_nfc_op,      0, sizeof(hi_nfc_op_u)     );
    hi_memset(&un_reg_nfc_con,     0, sizeof(hi_nfc_con_u)    );
    hi_memset(&un_reg_nfc_ints,    0, sizeof(hi_nfc_ints_u)   );
    hi_memset(&un_reg_nfc_addrl,   0, sizeof(hi_nfc_addrl_u)  );
    hi_memset(&un_reg_nfc_addrh,   0, sizeof(hi_nfc_addrh_u)  );
    hi_memset(&un_reg_nfc_op_para, 0, sizeof(hi_nfc_op_para_u));
    
    // TODO: in SECURE boot mod,page0 of block0 must set to 24bit ecc &2KB pagesize mode
    if((HI_BOOT_ROM == hi_get_boot_sel()) && (0 == ui_addr))
    {
        /* backup nfc_con reg value for normal mode */
        HI_REG_READ(&g_pst_nfc_reg->ui_nfc_con, ui_backup_con_reg);

        /* set nfc_con to 24bit 2k pagesize mode */
        un_reg_nfc_con.ui_value             = ui_backup_con_reg;
        un_reg_nfc_con.st_bits.ui_page_type = HI_NAND_PAGE_2KB_E;
        un_reg_nfc_con.st_bits.ui_ecc_type  = HI_NAND_ECC_24BIT_E;
        
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_con, un_reg_nfc_con.ui_value);
    }

    if((HI_NAND_READ_E == en_op_type) || (HI_NAND_READ_OOB_E == en_op_type))
    {
        hi_nand_drv_reg_set_cmd(0, HI_NAND_CMD_READSTART, HI_NAND_CMD_STATUS);
    }
    else
    {
        hi_nand_drv_reg_set_cmd(HI_NAND_CMD_SEQIN, HI_NAND_CMD_PAGEPROG, HI_NAND_CMD_STATUS);
    }

    /* set read/write addr */
    if(HI_NAND_READ_OOB_E == en_op_type)
    {
        if(0 == ui_addr)    /* 配置页固定为1082 */
        {
            un_reg_nfc_addrl.st_rw_bits.ui_page_inner_addr = 1082;
        }
        else
        {
            un_reg_nfc_addrl.st_rw_bits.ui_page_inner_addr = hi_nand_drv_get_oob_offset(pst_host);
        }
    }

    un_reg_nfc_addrl.st_rw_bits.ui_page_addr = ui_addr&0xffff;
    un_reg_nfc_addrh.st_rw_bits.ui_addr_h    = ui_addr>>16;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_addrl, un_reg_nfc_addrl.ui_value);
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_addrh, un_reg_nfc_addrh.ui_value);

    /* set nfc_op_para reg */
    un_reg_nfc_op_para.st_bits.ui_oob_rw_en   = HI_TRUE;
    un_reg_nfc_op_para.st_bits.ui_oob_edc_en  = HI_TRUE;
    
    if(HI_NAND_READ_OOB_E != en_op_type)
    {
        un_reg_nfc_op_para.st_bits.ui_data_rw_en  = HI_TRUE;
        un_reg_nfc_op_para.st_bits.ui_data_edc_en = HI_TRUE;
    }
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op_para, un_reg_nfc_op_para.ui_value);

    /* enable all irq */
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_inten, HI_NFC_INTERRUPT_MASK);

    /* clear all irq status */
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_intcrl, HI_NFC_INTERRUPT_MASK);

    /* copy page data(and oob) to nandc buffer */
    if(HI_NAND_WRITE_E == en_op_type)
    {
        hi_memcpy(pst_host->pv_nand_buffer, pst_host->puc_buffer, pst_chip->st_spec.ui_page_size + pst_chip->st_spec.ui_oob_size);
    }
    else
    {
        hi_memset(pst_host->pv_nand_buffer, 0xFF, pst_chip->st_spec.ui_page_size + pst_chip->st_spec.ui_oob_size);
    }

    /* set nfc_op to start read/write */
    un_reg_nfc_op.st_bits.ui_cmd1_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_addr_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_cmd2_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_wait_ready_en  = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_read_status_en = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_address_cycle  = HI_NFC_RW_ADDR_CYCLE;

    if((HI_NAND_READ_OOB_E == en_op_type) || (HI_NAND_READ_E == en_op_type))
    {
        un_reg_nfc_op.st_bits.ui_read_data_en   = HI_TRUE;
    }
    else
    {
        un_reg_nfc_op.st_bits.ui_write_data_en  = HI_TRUE;
    }

    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op, un_reg_nfc_op.ui_value);

    /* wait for ready */
    hi_nand_drv_wait_ready();

    /* read ints status to check op result */
    HI_REG_READ(&g_pst_nfc_reg->ui_nfc_ints, un_reg_nfc_ints.ui_value);
    if(un_reg_nfc_ints.st_bits.ui_err_invalid)
    {
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_con, ui_backup_con_reg);
        HI_NAND_PRINTF("error! ecc err invalid!\n");
        return -HI_EIO;
    }

    /* revert normal con reg value */
    if(0 == ui_addr)
    {
        HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_con, ui_backup_con_reg);
    }

    /* read data & oob from nandc buffer to dma */
    if((HI_NAND_READ_E == en_op_type) || (HI_NAND_READ_OOB_E == en_op_type))
    {
        if(HI_NAND_READ_E == en_op_type)
        {
            hi_memcpy(pst_host->puc_buffer, (hi_char8 *)pst_host->pv_nand_buffer, pst_chip->st_spec.ui_page_size);
        }
        
        hi_memcpy(pst_host->puc_buffer+pst_chip->st_spec.ui_page_size, (hi_char8 *)pst_host->pv_nand_buffer+pst_chip->st_spec.ui_page_size, pst_chip->st_spec.ui_oob_size );
    }

    return 0;
}
#endif

/******************************************************************************
  函数功能:  nand read/write entry(page 0 use bus rw mode, others use dma rw mode)
  输入参数:  
                             pst_host : nand driver host structure
                             ui_addr : read/write address
                             en_op_type : operation type read/write/read oob
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_drv_rw(struct hi_nand_host *pst_host, hi_uint32 ui_addr, HI_NAND_OP_E en_op_type)
{
#if 1    //nfc v500
    // TODO: in SECURE boot mod,nandc v600 page0 of block0 do not support dma transfer!
    if((HI_BOOT_ROM == hi_get_boot_sel()) && (0 == ui_addr))
    {
        struct hi_nand_chip *pst_chip = pst_host->pst_chip;

        if(HI_NAND_WRITE_E == en_op_type)
        {
            /* 支持最大有效数据长度，确保大于有效数据的页内地址填充0xff，否则会ECC出错 */
            hi_memset(pst_host->puc_buffer + HI_NFC_MAX_PAGE0_LEN, 0xFF, pst_chip->st_spec.ui_page_size-HI_NFC_MAX_PAGE0_LEN);
        }
        return hi_nand_drv_bus_rw(pst_host, ui_addr, en_op_type);
    }
    else
#endif
    {
        return hi_nand_drv_dma_transfer(pst_host, ui_addr, en_op_type);
    }
}

/******************************************************************************
  函数功能:  nand controller read page
  输入参数:  
                             pst_host : nand driver host structure
                             puc_buf : read buffer
                             ui_dst_addr : page addr
  输出参数:  无
  函数返回:  0:OK;  -1:NOK            
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_read(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_src_addr)
{
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;
    hi_int32 i_ret;
    
    /* read page data fron nand to dma buffer */
    i_ret = hi_nand_drv_rw(pst_host, ui_src_addr, HI_NAND_READ_E);
    if(i_ret)
    {
        return i_ret;
    }

    /* copy data & oob from dma buffer to output buffer */
    hi_memcpy(puc_buf, pst_host->puc_buffer, pst_chip->st_spec.ui_page_size);
    //hi_memcpy(puc_buf+pst_chip->st_spec.ui_page_size, pst_host->puc_oob_buffer, pst_chip->st_spec.ui_oob_size );

    return 0;
}

/******************************************************************************
  函数功能:  nand controller read oob only
  输入参数:  
                             pst_host : nand driver host structure
                             puc_buf : oob buffer
                             ui_src_addr : page addr
  输出参数:  无
  函数返回:  0:OK;  -1:NOK       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_read_oob(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_src_addr)
{
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;
    hi_int32 i_ret;
    
    /* read page data fron nand to dma buffer */
    i_ret = hi_nand_drv_rw(pst_host, ui_src_addr, HI_NAND_READ_OOB_E);
    if(i_ret)
    {
        return i_ret;
    }

    /* copy data from dma buffer to oob buffer */
    hi_memcpy(puc_buf, pst_host->puc_oob_buffer, pst_chip->st_spec.ui_oob_size);

    return 0;
}

/******************************************************************************
  函数功能:  nand controller write page
  输入参数:  
                             pst_host : nand driver host structure
                             puc_buf : oob buffer
                             ui_dst_addr : page addr
  输出参数:  无
  函数返回:  hi_int32       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_write(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_dst_addr)
{
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;
    hi_int32 ret = 0;
    /* copy data from dma buffer & oob buffer */
    hi_memcpy(pst_host->puc_buffer, puc_buf, pst_chip->st_spec.ui_page_size);
    //hi_memcpy(pst_host->puc_oob_buffer, puc_buf+pst_chip->st_spec.ui_page_size, pst_chip->st_spec.ui_oob_size );
#ifdef FLASH_GPIO_WRITE_PROTECT
    nand_wp_disable();
#endif
    ret = hi_nand_drv_rw(pst_host, ui_dst_addr, HI_NAND_WRITE_E);
#ifdef FLASH_GPIO_WRITE_PROTECT
    nand_wp_enable();
#endif

    if( 0 != ret)
    {
        return ret;
    }

#ifdef CONFIG_MTD_NAND_VERIFY_WRITE
    return hi_nand_drv_check_write(puc_buf,ui_dst_addr);
#else
    return 0;
#endif

}

/******************************************************************************
  函数功能:  nand controller write oob
  输入参数:  
                             pst_host : nand driver host structure
                             puc_buf : oob buffer
                             ui_dst_addr : page addr
  输出参数:  无
  函数返回:  hi_nand_drv_rw
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_write_oob(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_dst_addr)
{
    struct hi_nand_chip *pst_chip = pst_host->pst_chip;
    hi_int32 ret = 0;
    /* copy data from dma buffer to oob buffer */
    hi_memset(pst_host->puc_buffer, 0xff, pst_chip->st_spec.ui_page_size);
    hi_memcpy(pst_host->puc_oob_buffer, puc_buf, pst_chip->st_spec.ui_oob_size );
#ifdef FLASH_GPIO_WRITE_PROTECT
    nand_wp_disable(); 
#endif
    ret = hi_nand_drv_rw(pst_host, ui_dst_addr, HI_NAND_WRITE_E);
#ifdef FLASH_GPIO_WRITE_PROTECT
    nand_wp_enable();
#endif    
    return ret;
}

/******************************************************************************
  函数功能:  nand controller erase one block
  输入参数:  
                             ui_dst_addr : block addr
  输出参数:  无
  函数返回:  i_status       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_erase(hi_uint32 ui_dst_addr)
{
    hi_int32        i_status;
    hi_nfc_op_u     un_reg_nfc_op;
    hi_nfc_addrl_u  un_reg_nfc_addrl;
#ifdef FLASH_GPIO_WRITE_PROTECT    
    nand_wp_disable(); 
#endif
    hi_memset(&un_reg_nfc_op,    0, sizeof(hi_nfc_op_u)   );
    hi_memset(&un_reg_nfc_addrl, 0, sizeof(hi_nfc_addrl_u));

    /* send erase and read status cmd */
    hi_nand_drv_reg_set_cmd(HI_NAND_CMD_ERASE1, HI_NAND_CMD_ERASE2, HI_NAND_CMD_STATUS);

    /* set erase address */
    un_reg_nfc_addrl.st_erase_bits.ui_erase_addr = ui_dst_addr;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_addrl, un_reg_nfc_addrl.ui_value);

    /* set nfc_op to start erase */
    un_reg_nfc_op.st_bits.ui_cmd1_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_addr_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_cmd2_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_wait_ready_en  = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_read_status_en = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_address_cycle  = HI_NFC_ERASE_ADDR_CYCLE;

    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op, un_reg_nfc_op.ui_value);

    /* wait for ready*/
    i_status = hi_nand_drv_wait_ready();
#ifdef FLASH_GPIO_WRITE_PROTECT    
    nand_wp_enable();   
#endif
    if(HI_NAND_STATUS_FAIL & i_status)
    {
        return -HI_EIO;
    }
    
    #ifdef CONFIG_MTD_NAND_VERIFY_ERASE
    	return hi_nand_drv_check_erase(ui_dst_addr);
	#else    
    	return 0;
	#endif
}

/******************************************************************************
  函数功能:  to read nandflash id 
  输入参数:  
                             host : nand driver host structure
                             puc_buf : output id buffer
                             uc_id_len : read id len
  输出参数:  无
  函数返回:  无       
  函数备注:  
******************************************************************************/
hi_void hi_nand_drv_read_id(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uchar8 uc_id_len)
{
    hi_uint32    ui_backup_con_reg;
    hi_nfc_op_u  un_reg_nfc_op;
    hi_nfc_con_u un_reg_nfc_con;

    if((HI_NULL == puc_buf) || (0 == uc_id_len))
    {
        return;
    }

    hi_memset(&un_reg_nfc_op,  0, sizeof(hi_nfc_op_u) );
    hi_memset(&un_reg_nfc_con, 0, sizeof(hi_nfc_con_u));

    /* backup nfc_con reg value for normal mode */
    HI_REG_READ(&g_pst_nfc_reg->ui_nfc_con, ui_backup_con_reg);
    
    /* set nfc_con to ECC0 mode */
    un_reg_nfc_con.ui_value            = ui_backup_con_reg;
    un_reg_nfc_con.st_bits.ui_ecc_type = HI_NAND_ECC_0BIT_E;
    un_reg_nfc_con.st_bits.ui_op_mode  = HI_NAND_OP_MODE_NORMAL_E;      //more or less happy
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_con, un_reg_nfc_con.ui_value);

    /* send read id and read status cmd */
    hi_nand_drv_reg_set_cmd(HI_NAND_CMD_READID, 0, HI_NAND_CMD_STATUS);

    /* set nfc_addrl to 0 */
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_addrl, 0);

    /* set nfc_data_num to read len */
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_data_num, uc_id_len);

    /* set nfc_op to start read id */
    un_reg_nfc_op.st_bits.ui_cmd1_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_addr_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_read_data_en   = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_read_status_en = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_address_cycle  = HI_NFC_READID_ADDR_CYCLE;

    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op, un_reg_nfc_op.ui_value);

    /* wait for ready*/
    hi_nand_drv_wait_ready();
    
    /* copy id info from nandc buffer */
    hi_memcpy(puc_buf, pst_host->pv_nand_buffer, uc_id_len);

    /* revert normal con reg value */
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_con, ui_backup_con_reg);

    return;
}

/******************************************************************************
  函数功能:  nand chip reset 
  输入参数:  无
  输出参数:  无
  函数返回:  无       
  函数备注:  
******************************************************************************/
hi_void hi_nand_drv_reset(hi_void)
{
    hi_nfc_op_u un_reg_nfc_op;
    
    hi_memset(&un_reg_nfc_op, 0, sizeof(hi_nfc_op_u));

    /* send reset and read status cmd */
    hi_nand_drv_reg_set_cmd(HI_NAND_CMD_RESET, 0, HI_NAND_CMD_STATUS);

    /* set nfc_op to start reset */ 
    un_reg_nfc_op.st_bits.ui_cmd1_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_wait_ready_en  = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_read_status_en = HI_TRUE;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op, un_reg_nfc_op.ui_value); 

    /* wait for ready */
    hi_nand_drv_wait_ready();
}

/******************************************************************************
  函数功能:  nand chip reset test, only return ok when do reset and read nand status is 'ready'
  输入参数:  无
  输出参数:  无
  函数返回:  0:OK   -1:NOK       
  函数备注:  
******************************************************************************/
static hi_int32 hi_nand_drv_reset_test(hi_void)
{
    hi_int32        i_mdelay;
    hi_nfc_op_u     un_reg_nfc_op;
    hi_nfc_status_u un_reg_nfc_status;

    hi_memset(&un_reg_nfc_op,     0, sizeof(hi_nfc_op_u)    );
    hi_memset(&un_reg_nfc_status, 0, sizeof(hi_nfc_status_u));

    /* send reset and read status cmd */
    hi_nand_drv_reg_set_cmd(HI_NAND_CMD_RESET, 0, HI_NAND_CMD_STATUS);

    /* set nfc_op reg to start reset */
    un_reg_nfc_op.st_bits.ui_cmd1_en        = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_wait_ready_en  = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_read_status_en = HI_TRUE;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op, un_reg_nfc_op.ui_value); 

    /* test for 50 times with a 1ms delay */
    for(i_mdelay=0; i_mdelay<50; i_mdelay++)
    {
        HI_REG_READ(&g_pst_nfc_reg->ui_nfc_status, un_reg_nfc_status.ui_value);
        if(HI_TRUE == un_reg_nfc_status.st_bits.ui_nfc_ready)
        {
            return  0;
        }
        
        hi_mdelay(1);
    }
    
    return -HI_EBUSY;
}

/******************************************************************************
  函数功能:  check if the chip is write protected
  输入参数:  无
  输出参数:  无
  函数返回:  0:not write protected;     1:write protected
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_wp_check(hi_void)
{
    hi_int32        i_status;
    hi_nfc_op_u     un_reg_nfc_op;
    hi_nfc_status_u un_reg_nfc_status;

    hi_memset(&un_reg_nfc_op,     0, sizeof(hi_nfc_op_u)    );
    hi_memset(&un_reg_nfc_status, 0, sizeof(hi_nfc_status_u));
#ifdef FLASH_GPIO_WRITE_PROTECT
    nand_wp_disable();
#endif
    /* send read status cmd */
    hi_nand_drv_reg_set_cmd(0, 0, HI_NAND_CMD_STATUS);

    /* set nfc_op reg to get status */
    un_reg_nfc_op.st_bits.ui_read_status_en = HI_TRUE;
    un_reg_nfc_op.st_bits.ui_wait_ready_en  = HI_TRUE;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_op, un_reg_nfc_op.ui_value); 

    /* wait for ready */
    i_status = hi_nand_drv_wait_ready();
#ifdef FLASH_GPIO_WRITE_PROTECT    
    nand_wp_enable();  
#endif
    return (i_status & HI_NAND_STATUS_WP) ? 0 : 1;
}

/******************************************************************************
  函数功能:  malloc dma buffer & set nfc_baddr/nfc_baddr1 & nfc_oob reg
  输入参数:  
                             host : nand driver host structure
  输出参数:  无
  函数返回:  0:OK;  -1:malloc dma buffer error       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_dma_init(struct hi_nand_host *pst_host)
{  
    /* malloc dma buffer*/
    pst_host->puc_buffer = dma_alloc_coherent(pst_host->pst_dev, HI_NAND_MAX_PAGE_SIZE+HI_NAND_MAX_OOB_SIZE, &pst_host->ui_dma, GFP_KERNEL);
    if (NULL == pst_host->puc_buffer)
    {
        return -HI_ENOMEM;
    }

    hi_memset(pst_host->puc_buffer, 0xff, HI_NAND_MAX_PAGE_SIZE+HI_NAND_MAX_OOB_SIZE);

    /* set extern buffer default addr for 8K pagesize device */
    pst_host->puc_buffer_extern = pst_host->puc_buffer + HI_NFC_MAX_DMA_BUF_LEN;
    pst_host->ui_dma_extern     = pst_host->ui_dma + HI_NFC_MAX_DMA_BUF_LEN;
    
    /* set oob buffer default addr */
    pst_host->puc_oob_buffer    = pst_host->puc_buffer + HI_NAND_MAX_PAGE_SIZE;
    pst_host->ui_dma_oob        = pst_host->ui_dma + HI_NAND_MAX_PAGE_SIZE;
    
    /* set nfc_baddr & nfc_baddr1 & nfc_baddr_oob reg*/
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_baddr_d,  (hi_uint32)pst_host->ui_dma       );
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_baddr_d1, (hi_uint32)pst_host->ui_dma_extern);
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_baddr_oob,(hi_uint32)pst_host->ui_dma_oob   );

    return 0;
}

/******************************************************************************
  函数功能:  nand drv first init(to init nandc buffer & reg ioremap for nand reset & read id)
  输入参数:  
                             host : nand driver host structure
  输出参数:  无
  函数返回:  0:OK;  -1:ioremap error(kernel only)       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_pre_init(struct hi_nand_host *pst_host)
{    
    /* regbase ioremap */
    g_pst_nfc_reg = (hi_nfc_reg_s *)ioremap_nocache(HI_MTD_NFC_REG_BASE_ADDRESS, sizeof(hi_nfc_reg_s));
    if (HI_NULL == g_pst_nfc_reg)
    {
        return -EFAULT;
    }

    /* nandc buffer space ioremap */
    pst_host->pv_nand_buffer = ioremap_nocache(HI_MTD_NFC_BUFFER_BASE_ADDRESS, HI_MTD_NFC_BUFFER_BASE_ADDRESS_LEN);
    if (HI_NULL == pst_host->pv_nand_buffer)
    {
        iounmap(g_pst_nfc_reg);
        return -EFAULT;
    }

    /* initial nandc buffer */
    hi_memset((hi_uchar8 *)pst_host->pv_nand_buffer, 0xff, HI_MTD_NFC_BUFFER_BASE_ADDRESS_LEN);
    
    return 0;
}

/******************************************************************************
  函数功能:  nand drv init
  输入参数:  
                             host : nand driver host structure
  输出参数:  无
  函数返回:  hi_nand_drv_reset_test();  -1:malloc dma buffer error       
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_init(struct hi_nand_host *pst_host)
{
    hi_nfc_con_u    un_reg_nfc_con;
    hi_nfc_pwidth_u un_reg_nfc_pwidth;
    hi_int32 i_ret;

    hi_memset(&un_reg_nfc_con,    0, sizeof(hi_nfc_con_u)   );
    hi_memset(&un_reg_nfc_pwidth, 0, sizeof(hi_nfc_pwidth_u));
    
    /* nandc set config */
    un_reg_nfc_con.st_bits.ui_op_mode   = HI_NAND_OP_MODE_NORMAL_E;
    un_reg_nfc_con.st_bits.ui_bus_width = HI_NAND_BUS_WIDTH_8BIT_E;
    un_reg_nfc_con.st_bits.ui_page_type = pst_host->en_page_type;
    un_reg_nfc_con.st_bits.ui_ecc_type  = pst_host->en_ecc_type;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_con, un_reg_nfc_con.ui_value);

    /* set nfc pwidth */
    un_reg_nfc_pwidth.st_bits.ui_w_lcnt  = HI_NFC_PWIDTH_W_LCNT;
    un_reg_nfc_pwidth.st_bits.ui_r_lcnt  = HI_NFC_PWIDTH_R_LCNT;
    un_reg_nfc_pwidth.st_bits.ui_rw_hcnt = HI_NFC_PWIDTH_RW_HCNT;
    
    HI_REG_WRITE(&g_pst_nfc_reg->ui_nfc_pwidth, un_reg_nfc_pwidth.ui_value);

#ifdef FLASH_GPIO_WRITE_PROTECT
    sd5610x_iomux_set_gpio_mode(CONFIG_FMC_NAND_FLASH_WP_PIN);
    sd5610x_gpio_bit_attr_set(CONFIG_FMC_NAND_FLASH_WP_PIN, 1);
#endif

    /* malloc dma buffer & set nfc_baddr & nfc_oob reg */
    i_ret = hi_nand_drv_dma_init(pst_host);
    if(i_ret)
    {
        /* unlikely,malloc dma buffer error */
        return i_ret;
    }
    
    return hi_nand_drv_reset_test();
}

/******************************************************************************
  函数功能:  nand drv exit
  输入参数:  
                             host : nand driver host structure
  输出参数:  无
  函数返回:  0成功    
  函数备注:  
******************************************************************************/
hi_int32 hi_nand_drv_exit(struct hi_nand_host *pst_host)
{
    dma_free_coherent(pst_host->pst_dev, HI_NAND_MAX_PAGE_SIZE+HI_NAND_MAX_OOB_SIZE, pst_host->puc_buffer, pst_host->ui_dma);
    pst_host->puc_buffer = HI_NULL;

    iounmap(pst_host->pv_nand_buffer);
    iounmap(g_pst_nfc_reg);

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

