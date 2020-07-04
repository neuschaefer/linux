/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: hi_nand.h
  功能描述: nand核心驱动头文件
  版本描述: V1.0

  创建日期: D2013_04_20

  修改记录: 
            生成初稿.
******************************************************************************/
#ifndef __HI_NAND_DRV_H__
#define __HI_NAND_DRV_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* base bus addr define */
#define HI_NFC_BUFFER_BASE              0x20000000
#define HI_NFC_REG_BASE                 0x10A30000

#if 1    //nfc v500
#define HI_NFC_NANDC_BUF_LEN            (0x2000+0x400)  //9KB
#else    //nfc v300
#define HI_NFC_NANDC_BUF_LEN            (0x800+0x140)   //2048+320B
#endif

/* 非ECC0模式下缺省OOB长度 */
#define HI_NFC_ECC_DEFAULT_OOB_SIZE     32

#if 1    //nfc v500
/* page0 支持最大有效数据长度 */
#define HI_NFC_MAX_PAGE0_LEN            0x410   //1040B
#endif

/* 读写脉冲宽度 */
#define HI_NFC_PWIDTH_W_LCNT            0x5
#define HI_NFC_PWIDTH_R_LCNT            0x5
#define HI_NFC_PWIDTH_RW_HCNT           0x3

/* 中断使能掩码 */
#define HI_NFC_INTERRUPT_MASK           0x7FF

/* 发给NAND Flash的地址周期数 */
#define HI_NFC_RW_ADDR_CYCLE            0x5
#define HI_NFC_ERASE_ADDR_CYCLE         0x3
#define HI_NFC_READID_ADDR_CYCLE        0x1

/* nandc支持最大DMA buffer长度 */
#define HI_NFC_MAX_DMA_BUF_LEN          0x1000


/*************************
  * enum
  *************************/
typedef enum
{
    HI_NAND_ADDR_NUM_5CYCLE_E   = 0x0,
    HI_NAND_ADDR_NUM_4CYCLE_E   = 0x1,
}HI_NAND_ADDR_NUM_E;

typedef enum
{
    HI_NAND_OP_MODE_BOOT_E      = 0x0,
    HI_NAND_OP_MODE_NORMAL_E    = 0x1,
}HI_NAND_OP_MODE_E;

typedef enum
{
    HI_NAND_BUS_WIDTH_8BIT_E    = 0x0,
    HI_NAND_BUS_WIDTH_16BIT_E   = 0x1,
}HI_NAND_BUS_WIDTH_E;

typedef enum
{
    HI_NAND_PAGE_2KB_E     = 0x1,
    HI_NAND_PAGE_4KB_E     = 0x2,
    HI_NAND_PAGE_8KB_E     = 0x3
}HI_PAGE_SIZE_E;

/*************************
  * struct
  *************************/
typedef struct 
{
    hi_uint32 ui_nfc_con;                 /* (0x00) -NANDC配置寄存器                                                 */
    hi_uint32 ui_nfc_pwidth;              /* (0x04) -读写脉冲宽度配置寄存器                        */
    hi_uint32 ui_nfc_opidle;              /* (0x08) -操作间隔配置寄存器                                    */
    hi_uint32 ui_nfc_cmd;                 /* (0x0c) -命令字配置寄存器                                          */
    hi_uint32 ui_nfc_addrl;               /* (0x10) -低位地址配置寄存器                                    */
    hi_uint32 ui_nfc_addrh;               /* (0x14) -高位地址配置寄存器                                    */
    hi_uint32 ui_nfc_data_num;            /* (0x18) -读写数据数目配置寄存器                        */
    hi_uint32 ui_nfc_op;                  /* (0x1c) -操作寄存器                                                            */
    hi_uint32 ui_nfc_status;              /* (0x20) -状态寄存器                                                            */
    hi_uint32 ui_nfc_inten;               /* (0x24) -中断使能寄存器                                                */
    hi_uint32 ui_nfc_ints;                /* (0x28) -中断状态寄存器                                                */
    hi_uint32 ui_nfc_intcrl;              /* (0x2c) -中断清除寄存器                                                */
    hi_uint32 ui_nfc_lock;                /* (0x30) -锁地址配置寄存器                                          */
    hi_uint32 ui_nfc_lock_sa0;            /* (0x34) -锁起始地址0配置寄存器                            */
    hi_uint32 ui_nfc_lock_sa1;            /* (0x38) -锁起始地址1配置寄存器                            */
    hi_uint32 ui_nfc_lock_sa2;            /* (0x3c) -锁起始地址2配置寄存器                            */
    hi_uint32 ui_nfc_lock_sa3;            /* (0x40) -锁起始地址3配置寄存器                            */
    hi_uint32 ui_nfc_lock_ea0;            /* (0x44) -锁结束地址0配置寄存器                            */
    hi_uint32 ui_nfc_lock_ea1;            /* (0x48) -锁结束地址1配置寄存器                            */
    hi_uint32 ui_nfc_lock_ea2;            /* (0x4c) -锁结束地址2配置寄存器                            */
    hi_uint32 ui_nfc_lock_ea3;            /* (0x50) -锁结束地址3配置寄存器                            */
#if 1    //nfc v500 
    hi_uint32 ui_nfc_expcmd;              /* (0x54) -扩展页命令寄存器                                          */
    hi_uint32 ui_nfc_exbcmd;              /* (0x58) -扩展块命令寄存器                                          */
#else    //nfc v300 
    hi_uint32 ui_nfc_reserved[2];         /* (0x54-0x58) -保留                                                                    */
#endif
    hi_uint32 ui_nfc_ecc_test;            /* (0x5c) -ECC测试寄存器                                                      */
    hi_uint32 ui_nfc_dma_ctrl;            /* (0x60) -DMA控制寄存器                                                     */
    hi_uint32 ui_nfc_baddr_d;             /* (0x64) -传送数据区的基地址寄存器                  */
    hi_uint32 ui_nfc_baddr_oob;           /* (0x68) -传送OOB区的基地址寄存器                       */
    hi_uint32 ui_nfc_dma_len;             /* (0x6c) -传送长度寄存器                                                */
    hi_uint32 ui_nfc_op_para;             /* (0x70) -操作参数寄存器                                                */
    hi_uint32 ui_nfc_version;             /* (0x74) -nandc版本寄存器                                                   */
#if 1    //nfc v500 
    hi_uint32 ui_nfc_reserved1[3];        /* (0x78-0x80) -保留                                                                    */
#else    //nfc v300 
    hi_uint32 ui_nfc_buf_baddr;           /* (0x78) -BUFFER基地址寄存器                                          */
    hi_uint32 ui_nfc_rd_logic_addr;       /* (0x7c) -DMA读nandflash的逻辑地址寄存器               */
    hi_uint32 ui_nfc_rd_logic_len;        /* (0x80) -DMA读nandflash的逻辑长度寄存器               */
#endif
    hi_uint32 ui_nfc_segment_id;          /* (0x84) -DMA读写nandflash的数据段ID寄存器            */
    hi_uint32 ui_nfc_reserved2[2];        /* (0x88-0x8c) -保留                                                                    */
    hi_uint32 ui_nfc_fifo_empty;          /* (0x90) -内部fifo状态寄存器                                           */
#if 1    //nfc v500 
    hi_uint32 ui_nfc_boot_set;            /* (0x94) -boot参数设定寄存器                                         */
    hi_uint32 ui_nfc_reserved3;           /* (0x98) -保留                                                                              */
#else    //nfc v300 
    hi_uint32 ui_nfc_reserved3;           /* (0x94) -保留                                                                             */
    hi_uint32 ui_nf_status;               /* (0x98) -nandflash的sattus寄存器                                          */
#endif
    hi_uint32 ui_nfc_lp_ctrl;             /* (0x9c) -nandc 的低功耗控制寄存器                          */
    hi_uint32 ui_nfc_err_num0_buf0;       /* (0xa0) -nandflash 第一次buffer纠错统计寄存器0    */
    hi_uint32 ui_nfc_err_num1_buf0;       /* (0xa4) -nandflash 第一次buffer纠错统计寄存器1    */
    hi_uint32 ui_nfc_err_num0_buf1;       /* (0xa8) -nandflash 第二次buffer纠错统计寄存器0    */
    hi_uint32 ui_nfc_err_num1_buf1;       /* (0xac) -nandflash 第二次buffer纠错统计寄存器1    */
    hi_uint32 ui_nfc_rb_mode;             /* (0xb0) -nandflash 的ready_busy模式寄存器                     */
    hi_uint32 ui_nfc_baddr_d1;            /* (0xb4) -传送数据区的基地址寄存器1               */
    hi_uint32 ui_nfc_baddr_d2;            /* (0xb8) -传送数据区的基地址寄存器2               */
    hi_uint32 ui_nfc_baddr_d3;            /* (0xbc) -传送数据区的基地址寄存器3                */
#if 1    //nfc v500 
    hi_uint32 ui_nfc_reserved4;           /* (0xc0) -保留                                                                             */
#else    //nfc v300 
    hi_uint32 ui_nfc_randomizer;          /* (0xc0) -nandflash 的randomizer使能寄存器                      */
#endif
    hi_uint32 ui_nfc_boot_cfg;            /* (0xc4) -nandflash 的boot配置寄存器                               */
#if 1    //nfc v500 
    hi_uint32 ui_nfc_oob_sel;             /* (0xc8) -16bit ECC，OOB区长度选择寄存器              */
    hi_uint32 ui_nfc_mem_ctrl;            /* (0xcc) -例化mem管理                                                          */
    hi_uint32 ui_nfc_sync_timing;         /* (0xd0) -同步NAND读写时序参数                                 */
    hi_uint32 ui_nfc_dll_ctrl;            /* (0xd4) -DLL模块控制信息                                                */
    hi_uint32 ui_nfc_dll_status;          /* (0xd8) -DLL模块状态信息                                                */
#endif
}hi_nfc_reg_s;

/*
  * NFC_CON reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_op_mode:1;           /* [0]-nandc操作模式                            */
        hi_uint32 ui_page_type:3;         /* [1:3]-nandflash页大小                         */
        hi_uint32 ui_bus_width:1;         /* [4]-nandflash数据线宽度                */
        hi_uint32 ui_reserved:2;          /* [5:6]-保留                                             */
        hi_uint32 ui_cs_ctrl:1;           /* [7]-片选控制                                    */
        hi_uint32 ui_rb_sel:1;            /* [8]-ready/busy信号控制                   */
        hi_uint32 ui_ecc_type:3;          /* [9:11]-ecc模式                                      */
#if 1    //nfc v500 
        hi_uint32 ui_reserved1:1;         /* [12]-保留                                              */
        hi_uint32 ui_randomizer_en:1;     /* [13]-randomizer使能                             */
        hi_uint32 ui_nf_mode:2;           /* [14:15]-nandflash接口类型选择   */
        hi_uint32 ui_reserved2:16;        /* [16:31]-保留                                         */
#else    //nfc v300 
        hi_uint32 ui_reserved1:20;        /* [12:31]-保留                                         */
#endif
    }st_bits;
} hi_nfc_con_u;

/*
  * NFC_PWIDTH reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_w_lcnt:4;            /* [0:3] -nandflash写信号低电平宽度         */
        hi_uint32 ui_r_lcnt:4;            /* [4:7] -nandflash读信号低电平宽度         */
        hi_uint32 ui_rw_hcnt:4;           /* [8:11] -nandflash读写信号高电平宽度 */
        hi_uint32 ui_reserved:20;         /* [12:31] -保留                                                       */
    }st_bits;
} hi_nfc_pwidth_u;

/*
  * NFC_CMD reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_cmd1:8;              /* [0:7]   -controller发给nandflash的第一个命令*/
        hi_uint32 ui_cmd2:8;              /* [8:15] -controller发给nandflash的第二个命令*/
        hi_uint32 ui_read_status_cmd:8;   /* [16:23] -read status命令字                                       */
        hi_uint32 ui_reserved:8;          /* [24:31] -保留                                                              */
    }st_bits;
} hi_nfc_cmd_u;

/*
  * NFC_ADDRL reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_page_inner_addr:16;  /* [0:15]-页内地址，只在bus方式只读OOB时使用    */
        hi_uint32 ui_page_addr:16;        /* [16:31]-页地址                                                                                */
    }st_rw_bits;
    struct
    {
        hi_uint32 ui_erase_addr:24;       /* [0:23]-擦除操作时的块地址    */
        hi_uint32 ui_reserved:8;          /* [24:31]-保留                                            */
    }st_erase_bits;
} hi_nfc_addrl_u;

/*
  * NFC_ADDRH reg, read/write may use, erase never use!!
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_addr_h:8;             /* [0:7]-高8bit地址空间                       */
        hi_uint32 ui_reserved:24;          /* [8:31]-保留                                            */
    }st_rw_bits;
} hi_nfc_addrh_u;

/*
  * NFC_OP reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_read_status_en:1;    /* [0]-读状态使能                                                                          */
        hi_uint32 ui_read_data_en:1;      /* [1]-读数据使能                                                                          */
        hi_uint32 ui_wait_ready_en:1;     /* [2]-等待ready/busy信号变高使能                                       */
        hi_uint32 ui_cmd2_en:1;           /* [3]-发cmd2命令使能                                                                  */
        hi_uint32 ui_write_data_en:1;     /* [4]-向nandflash写数据使能                                                      */
        hi_uint32 ui_addr_en:1;           /* [5]-向nand写操作地址使能                                                 */
        hi_uint32 ui_cmd1_en:1;           /* [6]-发cms1命令使能                                                                  */
        hi_uint32 ui_nf_cs:2;             /* [7:8]-选择操作的nandflash器件，即片选                     */
        hi_uint32 ui_address_cycle:3;     /* [9:11]-发给nandflash的地址周期数                                     */
#if 1    //nfc v500 
        hi_uint32 ui_read_id_en:1;        /* [12]-对于toggle nand，0:非读ID操作，1:读ID操作      */
        hi_uint32 ui_rw_reg_en:1;         /* [13]-对同步nand，0:读写数据，1:读写寄存器     */
        hi_uint32 ui_reserved:18;         /* [14:31]-保留                                                                                      */
#else    //nfc v300 
        hi_uint32 ui_reserved:20;         /* [12:31]-保留                                                                                      */
#endif
    }st_bits;
} hi_nfc_op_u;

/*
  * NFC_STATUS reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_nfc_ready:1;         /* [0]-nandc当前操作状态                                            */
        hi_uint32 ui_nf0_ready:1;         /* [1]-片选0器件的ready/busy信号状态                */
        hi_uint32 ui_nf1_ready:1;         /* [2]-片选1器件的ready/busy信号状态                */
#if 1    //nfc v500 
        hi_uint32 ui_nf2_ready:1;         /* [3]-片选2器件的ready/busy信号状态                */
        hi_uint32 ui_nf3_ready:1;         /* [4]-片选3器件的ready/busy信号状态                */
        hi_uint32 ui_reserved:3;          /* [5:7]-保留                                                                          */
        hi_uint32 ui_nf_status:8;         /* [8:15]-从nandflash读回的nandflash 的status数据    */
        hi_uint32 ui_nf_ecc_type:3;       /* [16:18]-nandc ecc模式，与CON相同，只读       */
        hi_uint32 ui_nf_randomizer_en:1;  /* [19]-randomizer状态，与CON相同个，只读    */
        hi_uint32 ui_reserved1:12;        /* [20:31] -保留                                                                     */
#else    //nfc v300 
        hi_uint32 ui_reserved:2;          /* [3:4]-保留                                                                          */
        hi_uint32 ui_nf_status:8;         /* [5:12]-从nandflash读回的nandflash 的status数据    */
        hi_uint32 ui_reserved1:19;        /* [13:31] -保留                                                                     */
#endif
    }st_bits;
} hi_nfc_status_u;

/*
  * NFC_INTS reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_op_done:1;           /* [0]-NANDC本次操作结束中断                                            */
        hi_uint32 ui_cs0_done:1;          /* [1]-片选0对应的ready/busy信号由低变高中断     */
        hi_uint32 ui_cs1_done:1;          /* [2]-片选1对应的ready/busy信号由低变高中断     */
        hi_uint32 ui_cs2_done:1;          /* [3]-片选2对应的ready/busy信号由低变高中断     */
        hi_uint32 ui_cs3_done:1;          /* [4]-片选3对应的ready/busy信号由低变高中断     */
        hi_uint32 ui_err_valid:1;         /* [5]-产生可纠正的错误中断                                          */
        hi_uint32 ui_err_invalid:1;       /* [6]-产生不可纠正的错误中断                                    */
        hi_uint32 ui_ahb_op:1;            /* [7]-NANDC读写flash器件cpu读写nandc buffer错误中断 */
        hi_uint32 ui_wr_lock:1;           /* [8]-对lock地址进行写操作错误中断                        */
        hi_uint32 ui_dma_done:1;          /* [9]-DMA传输完成中断                                                           */
        hi_uint32 ui_dma_err:1;           /* [10]-DMA传输中线出现错误中断                                 */
        hi_uint32 ui_reserved:21;         /* [11:31] -保留                                                                                 */
    }st_bits;
} hi_nfc_ints_u;

/*
  * NFC_DMA_CTRL reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_dma_start:1;         /* [0]-启动dma操作              */
        hi_uint32 ui_dma_wr_en:1;         /* [1]-dma读写使能              */
        hi_uint32 ui_reserved:2;          /* [2:3]-保留                             */
        hi_uint32 ui_burst4_en:1;         /* [4]-burst4使能                       */
        hi_uint32 ui_burst8_en:1;         /* [5]-burst8使能                       */
        hi_uint32 ui_burst16_en:1;        /* [6]-burst16使能                     */
        hi_uint32 ui_dma_addr_num:1;      /* [7]-地址数                           */
        hi_uint32 ui_dma_nf_cs:2;         /* [8:9]-选择dma片选           */
#if 1    //nfc v500 
        hi_uint32 ui_reserved1:1;         /* [10]-保留                               */
#else    //nfc v300 
        hi_uint32 ui_rw_nf_disable:1;     /* [10]-判断DMA读写时数据搬运同步 */
#endif
        hi_uint32 ui_wr_cmd_disable:1;    /* [11]-是否发出写命令 */
#if 1    //nfc v500 
        hi_uint32 ui_dma_rd_oob:1;        /* [12]-是否只读OOB            */
        hi_uint32 ui_reserved2:19;        /* [13:31]-保留                          */
#else    //nfc v300 
        hi_uint32 ui_reserved2:20;        /* [12:31]-保留                          */
#endif
    }st_bits;
} hi_nfc_dma_ctrl_u;

/*
  * NFC_OP_PARA reg
  */
typedef union
{
    hi_uint32 ui_value;
    struct
    {
        hi_uint32 ui_data_rw_en:1;        /* [0] -nandflash数据区读写             */
        hi_uint32 ui_oob_rw_en:1;         /* [1] -nandflash冗余区读写使能 */
        hi_uint32 ui_data_edc_en:1;       /* [2] -data区校验使能                    */
        hi_uint32 ui_oob_edc_en:1;        /* [3] -oob区校验使能                     */
#if 1    //nfc v500 
        hi_uint32 ui_reserved:28;         /* [4:31] -保留                                       */
#else    //nfc v300 
        hi_uint32 ui_data_ecc_en:1;       /* [4] -data区ECC纠错使能              */
        hi_uint32 ui_oob_ecc_en:1;        /* [5] -oob区ECC纠错使能              */
        hi_uint32 ui_reserved:26;         /* [6:31] -保留                                       */
#endif
    }st_bits;
} hi_nfc_op_para_u;

/*
  * nand driver host structure
  */
struct hi_nand_host 
{
    struct hi_nand_chip *pst_chip;
    struct mtd_info     *pst_mtd;
    struct device       *pst_dev;       /* kernel only */

    HI_NAND_ECC_TYPE_E en_ecc_type;     /* nand ecc type, support 1bit/4bit/8bit/24bit */
    HI_PAGE_SIZE_E     en_page_type;    /* nand page type, support 2k/4k/8k */

    hi_uint32 ui_dma;                   /* dma buffer physics addr */
    hi_uint32 ui_dma_extern;            /* 8k pagesize need an extern dma buffer */
    hi_uint32 ui_dma_oob;               /* oob dma physics addr */
    
    hi_uchar8 *puc_buffer;              /* dma buffer virtual addr */
    hi_uchar8 *puc_buffer_extern;        /* 8k pagesize need an extern dma buffer */
    hi_uchar8 *puc_oob_buffer;

    hi_void __iomem *pv_nand_buffer;          /* nandc buffer */
};

#if 0    //nfc v300 
/* 非ecc0下OOB长度计算 */
struct hi_nand_oob_size
{
    hi_uint32 ui_ecc_type;
    hi_uint32 ui_page_type;
    hi_uint32 ui_oob_size;
};
#endif

#if 1    //nfc v500 
/* bus方式只读OOB的情况下，包含OOB信息的sector的页内起始地址 */
struct hi_nand_oob_offset
{
    hi_uint32 ui_ecc_type;
    hi_uint32 ui_page_type;
    hi_uint32 ui_oob_sel;
    hi_uint32 ui_oob_offset;
};
#endif

/*************************
  * declare
  *************************/
hi_void  hi_nand_drv_reset(hi_void);
hi_int32 hi_nand_drv_init(struct hi_nand_host *host);
hi_int32 hi_nand_drv_exit(struct hi_nand_host *pst_host);
hi_int32 hi_nand_drv_pre_init(struct hi_nand_host *pst_host);
hi_int32 hi_nand_drv_erase(hi_uint32 ui_dst_addr);
hi_int32 hi_nand_drv_read(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_dst_addr);
hi_int32 hi_nand_drv_read_oob(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_dst_addr);
hi_int32 hi_nand_drv_write(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_dst_addr);
hi_int32 hi_nand_drv_write_oob(struct hi_nand_host *pst_host, hi_uchar8 *puc_buf, hi_uint32 ui_dst_addr);
hi_int32 hi_nand_drv_wp_check(hi_void);

#if 0    //nfc v300 
hi_uint32 hi_nand_drv_get_oob_size(struct hi_nand_host *pst_host);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_NAND_DRV_H__ */
