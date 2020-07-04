/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_acp.c
  版 本 号   : 初稿
  生成日期   : D2014_03_09

******************************************************************************/

#include <mach/hi_hsan.h>

#define HI_SC_PERCTRL1_REG     ( HI_REG_BASE_SCTL + 0xa0  )

#define HI_PCIE1_AWADDR33      (1 << 8 )
#define HI_PCIE1_ARADDR33      (1 << 9 )
#define HI_PCIE0_AWADDR33      (1 << 12)
#define HI_PCIE0_ARADDR33      (1 << 13)
#define HI_PIE_AWADDR33        (1 << 16)
#define HI_PIE_ARADDR33        (1 << 17)
#define HI_USB_AWADDR33        (1 << 20)
#define HI_USB_ARADDR33        (1 << 21)
#define HI_IPSEC_AWADDR33      (1 << 24)
#define HI_IPSEC_ARADDR33      (1 << 25)
#define HI_ACP_FREQ_MASK       (0x7 << 27) //bit 27~bit29

hi_void hi_acp_init(hi_void)
{
        hi_uint32 ui_value  = 0;
    
        HI_REG_READ( hi_io_address(HI_SC_PERCTRL1_REG),ui_value);
        ui_value |= (HI_PIE_ARADDR33   | HI_PIE_AWADDR33         //PIE read/write
                 |  HI_IPSEC_ARADDR33 | HI_IPSEC_AWADDR33);      //IPSEC read/write                
        ui_value &= ~(  HI_USB_ARADDR33   | HI_USB_AWADDR33);    //USB read/write  
		ui_value &= ~(  HI_PCIE0_ARADDR33 | HI_PCIE0_AWADDR33 |  HI_PCIE1_ARADDR33 | HI_PCIE1_AWADDR33); //PCIE read/write
                        
        HI_REG_WRITE(hi_io_address(HI_SC_PERCTRL1_REG),ui_value);
      
        hi_printk("\nACP:controller succ");
}

