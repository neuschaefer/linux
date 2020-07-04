/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: hi_nand.h
  功能描述: nand驱动接口公用头文件
  版本描述: V1.0

  创建日期: D2013_04_20

  修改记录: 
            生成初稿.
******************************************************************************/
#ifndef __HI_NAND_H__
#define __HI_NAND_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/* 最大nand ID长度 */
#define HI_NAND_ID_MAX_LEN          8

/* 最大支持8KB页大小 */
#define HI_NAND_MAX_PAGE_SIZE       0x2000

/* ECC0情况下最大支持512B,非ECC0下长度为HI_NFC_ECC_OOB_SIZE*/
#define HI_NAND_MAX_OOB_SIZE        0x200

// NAND Flash Manufacturer ID Codes
#define HI_NAND_MFR_TOSHIBA         0x98
#define HI_NAND_MFR_SAMSUNG         0xec
#define HI_NAND_MFR_FUJITSU         0x04
#define HI_NAND_MFR_NATIONAL        0x8f
#define HI_NAND_MFR_RENESAS         0x07
#define HI_NAND_MFR_STMICRO         0x20
#define HI_NAND_MFR_HYNIX           0xad
#define HI_NAND_MFR_MICRON          0x2c
#define HI_NAND_MFR_AMD             0x01

//#define CONFIG_MTD_NAND_VERIFY_WRITE
//#define CONFIG_MTD_NAND_VERIFY_ERASE

/*
  * standard nand flash commands
  */
#define HI_NAND_CMD_READ0           0
#define HI_NAND_CMD_READ1           1
#define HI_NAND_CMD_RNDOUT          5
#define HI_NAND_CMD_PAGEPROG        0x10
#define HI_NAND_CMD_READOOB         0x50
#define HI_NAND_CMD_ERASE1          0x60
#define HI_NAND_CMD_STATUS          0x70
#define HI_NAND_CMD_STATUS_MULTI    0x71
#define HI_NAND_CMD_SEQIN           0x80
#define HI_NAND_CMD_RNDIN           0x85
#define HI_NAND_CMD_READID          0x90
#define HI_NAND_CMD_ERASE2          0xd0
#define HI_NAND_CMD_RESET           0xff

/* Extended commands for large page devices */
#define HI_NAND_CMD_READSTART       0x30
#define HI_NAND_CMD_RNDOUTSTART     0xE0
#define HI_NAND_CMD_CACHEDPROG      0x15

/* Status bits */
#define HI_NAND_STATUS_FAIL         0x01
#define HI_NAND_STATUS_FAIL_N1      0x02
#define HI_NAND_STATUS_TRUE_READY   0x20
#define HI_NAND_STATUS_READY        0x40
#define HI_NAND_STATUS_WP           0x80
#define HI_NAND_ERASE_WRITE_RETRY_TIME 3

typedef enum
{
    HI_NAND_ECC_0BIT_E        = 0x0,
    HI_NAND_ECC_1BIT_E        = 0x1,
    HI_NAND_ECC_4BIT_E        = 0x2,
    HI_NAND_ECC_8BIT_E        = 0x3,
    HI_NAND_ECC_24BIT_E       = 0x4,
    HI_NAND_ECC_40BIT_E       = 0x5,
    HI_NAND_ECC_16BIT_E       = 0x6
}HI_NAND_ECC_TYPE_E;

/*
  * nand_state_t - chip states  Enumeration for NAND flash chip state
  */
typedef enum
{
    HI_FL_READY,
    HI_FL_READING,
    HI_FL_WRITING,
    HI_FL_ERASING,
    HI_FL_SYNCING,
    HI_FL_CACHEDPRG,
    HI_FL_PM_SUSPENDED,
}HI_NAND_STATE_E;

typedef enum
{
    HI_NAND_READ_E          = 0x0,
    HI_NAND_WRITE_E         = 0x1,
    HI_NAND_READ_OOB_E      = 0x2,
    HI_NAND_WRITE_OOB_E     = 0x3,
}HI_NAND_OP_E;

/* Keep gcc happy */
struct hi_nand_chip;
struct hi_nand_host;

struct hi_nand_buffers
{
    hi_uchar8   uac_ecccalc[HI_NAND_MAX_OOB_SIZE];
    hi_uchar8   uac_ecccode[HI_NAND_MAX_OOB_SIZE];
    hi_uchar8   uac_databuf[HI_NAND_MAX_PAGE_SIZE + HI_NAND_MAX_OOB_SIZE];
    hi_uchar8  *puc_blk_buf; 
};

/*
 * Constants for ECC_MODES
 */
typedef enum
{
    HI_NAND_ECC_NONE,
    HI_NAND_ECC_SOFT,
    HI_NAND_ECC_HW,
    HI_NAND_ECC_HW_SYNDROME,
} hi_nand_ecc_modes_t;

struct hi_nand_ecc_ctrl
{
    hi_nand_ecc_modes_t mode;
    
    hi_int32    steps;
    hi_int32    size;
    hi_int32    bytes;
    hi_int32    total;
    hi_int32    prepad;
    hi_int32    postpad;
    
    struct nand_ecclayout   *layout;
    
    hi_void     (*hwctl)(struct mtd_info *mtd, hi_int32 mode);
    hi_int32    (*calculate)(struct mtd_info *mtd, const hi_uchar8 *dat, hi_uchar8 *ecc_code);
    hi_int32    (*correct)(struct mtd_info *mtd, hi_uchar8 *dat, hi_uchar8 *read_ecc, hi_uchar8 *calc_ecc);
    hi_int32    (*read_page_raw)(struct mtd_info *mtd, struct hi_nand_chip *chip, hi_uchar8 *buf);
    hi_void     (*write_page_raw)(struct mtd_info *mtd, struct hi_nand_chip *chip, const hi_uchar8 *buf);
    hi_int32    (*read_page)(struct mtd_info *mtd, struct hi_nand_chip *chip, hi_uchar8 *buf);
    hi_int32    (*read_subpage)(struct mtd_info *mtd, struct hi_nand_chip *chip, hi_uint32 offs, hi_uint32 len, hi_uchar8 *buf);
    hi_void     (*write_page)(struct mtd_info *mtd, struct hi_nand_chip *chip, const hi_uchar8 *buf);
    hi_int32    (*read_oob)(struct mtd_info *mtd, struct hi_nand_chip *chip, hi_int32 page, hi_int32 sndcmd);
    hi_int32    (*write_oob)(struct mtd_info *mtd, struct hi_nand_chip *chip, hi_int32 page);
};

struct hi_nand_hw_control
{
    spinlock_t      lock;
    struct hi_nand_chip *active;
    wait_queue_head_t wq;
};

/*
  * nand chip 基本参数
  */
struct hi_nand_spec
{
    hi_uint32   ui_options;         /* 保留对标准linux内核的支持 */
    hi_uint32   ui_chip_size;       /* 单位为BYTE，最大支持4GBYTE */
    hi_uint32   ui_block_size;
    hi_uint32   ui_page_size;
    hi_uint32   ui_oob_size;
    hi_uint32   ui_page_shift;
    hi_uint32   ui_erase_shift;
    hi_uint32   ui_bbt_erase_shift;
    hi_uint32   ui_bbt_len;
    hi_uint32   ui_ecc_type;        /* nand ecc type, support 1bit/4bit/8bit/24bit */
};

struct hi_nand_chip
{
    struct hi_nand_spec st_spec;
    
    hi_char8    *pc_name;

    hi_uchar8   *puc_oob_poi;

    HI_NAND_STATE_E en_state;
    struct hi_nand_hw_control   *controller;
    struct hi_nand_hw_control   hwcontrol;

    hi_int32    i_page_buf;
    
    struct hi_nand_ecc_ctrl     st_ecc;
    struct mtd_oob_ops          st_ops;

    struct hi_nand_buffers      *pst_buffers;
    
    hi_uchar8   *puc_bbt;

    hi_void     *pv_priv;
};

typedef struct
{
    hi_uint32 ui_addr;
    hi_uint32 ui_data_len;
    hi_uint32 ui_check_bad;      /* 是否使能跳过坏块处理 */

    HI_NAND_OP_E en_op_type;

    hi_uchar8 *puc_data_buf;
    hi_uchar8 *puc_oob_buf;
}hi_nand_ops;

/* extern from hi_nand.c */
hi_uint32 hi_nand_get_total_size(hi_void);
hi_uint32 hi_nand_get_block_size(hi_void);
hi_uint32 hi_nand_get_page_size(hi_void);
hi_uint32 hi_nand_get_oob_size(hi_void);
struct hi_nand_host *hi_nand_get_nand_host(hi_void);
hi_int32 hi_nand_block_isbad(hi_uint32 ui_addr);
hi_int32 hi_nand_block_markbad(hi_uint32 ui_addr);
hi_int32 hi_nand_erase( hi_uint32 ui_offset, hi_uint32 ui_len );
hi_int32 hi_nand_read( hi_uchar8 *puc_buf, hi_uint32 ui_offset, hi_uint32 ui_len );
hi_int32 hi_nand_read_page(hi_uchar8 *puc_dst_buf, hi_uint32 ui_src_addr);
hi_int32 hi_nand_write( hi_uchar8 *puc_buf, hi_uint32 ui_offset, hi_uint32 ui_len );
hi_int32 hi_nand_write_page(hi_uchar8 *puc_src_buf, hi_uint32 ui_dst_addr);
hi_int32 hi_nand_oob_read( hi_uchar8 *puc_buf, hi_uint32 ui_offset);
hi_int32 hi_nand_oob_write( hi_uchar8 *puc_buf, hi_uint32 ui_offset );
hi_void hi_nand_dump_bbt(hi_void);

/* extern from hi_nand_bbt.c */
hi_int32 hi_nand_bbt_block_isbad(struct hi_nand_host *pst_host, hi_uint32 ui_block_addr);
hi_int32 hi_nand_bbt_block_markbad(struct hi_nand_host *pst_host, hi_uint32 ui_block_addr);

/* extern from hi_nand_mtd.c */
hi_int32 hi_nand_mtd_contact(struct mtd_info *pst_mtd);

/* extern form hi_nand_util.c */
hi_int32 hi_nand_util_erase(struct hi_nand_host *pst_host, hi_uint32 ui_dst_addr, hi_uint32 ui_len);
hi_int32 hi_nand_util_read(struct hi_nand_host *pst_host, hi_uchar8 *puc_dst_buf, hi_uint32 ui_src_addr, hi_uint32 ui_len);
hi_int32 hi_nand_util_read_page(struct hi_nand_host *pst_host, hi_uchar8 *puc_dst_buf, hi_uint32 ui_src_addr);
hi_int32 hi_nand_util_write(struct hi_nand_host *pst_host, hi_uchar8 *puc_src_buf, hi_uint32 ui_dst_addr, hi_uint32 ui_len);
hi_int32 hi_nand_util_write_page(struct hi_nand_host *pst_host, hi_uchar8 *puc_src_buf, hi_uint32 ui_dst_addr);
hi_int32 hi_nand_drv_check_erase(hi_uint32 ui_dst_addr);
hi_int32 hi_nand_drv_check_write(hi_uchar8 *puc_buf,hi_uint32 ui_page_addr);

/*************************
  * for debug
  *************************/
//#define HI_NAND_DEBUG   1

#ifdef HI_NAND_DEBUG
#define HI_NAND_PRINTF( fmt,arg... ) { \
        if ( 1 ) \
        { \
            hi_printk( fmt, ##arg );\
        } \
    }
#else
#define HI_NAND_PRINTF( fmt,arg... ) { \
        if ( 0 ) \
        { \
            hi_printk( fmt, ##arg );\
        } \
    }
#endif /* HI_NAND_DEBUG */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_NAND_H__ */

