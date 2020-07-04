/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

 ******************************************************************************
  �� �� ��   : hi_flash.h
  �� �� ��   : ����
  ��������   : D2014_03_12

******************************************************************************/
#ifndef __HI_FLASH_H__
#define __HI_FLASH_H__

#define HI_BOOT_ROM                            0

#define HI_MTD_NFC_REG_BASE_ADDRESS            0x10A30000
#define HI_MTD_NFC_REG_BASE_ADDRESS_LEN        0x100
#define HI_MTD_NFC_BUFFER_BASE_ADDRESS         0x20000000
#define HI_MTD_NFC_BUFFER_BASE_ADDRESS_LEN     0x2400

#define HI_MTD_SFC_REG_BASE_ADDRESS            0x10A20000
#define HI_MTD_SFC_REG_BASE_ADDRESS_LEN        0x100
#define HI_MTD_SFC_CS0_BUFFER_BASE_ADDRESS     0x18000000
#define HI_MTD_SFC_CS0_BUFFER_BASE_ADDRESS_LEN 0x4000000
#define HI_MTD_SFC_CS1_BUFFER_BASE_ADDRESS     0x1c000000
#define HI_MTD_SFC_CS1_BUFFER_BASE_ADDRESS_LEN 0x4000000

#define HI_IMAGE_HEADER_MAGIC_NUM              0x44414548
#define HI_NAME_LEN                            16
#define HI_MAX_MTD_NR                          32

typedef struct {
    hi_char8   ac_mtd_name[HI_NAME_LEN];
    hi_uint32  ui_mtd_addr;
    hi_uint32  ui_mtd_size;
}hi_mtd_s;

#endif /* __HI_FLASH_H__ */
