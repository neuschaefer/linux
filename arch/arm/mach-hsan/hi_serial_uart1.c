/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  Filename : hi_serial_uart1.c
  Function : hsan serial device driver

  Date     : 20151126

  Version  : V1.0
  Record   :
             create file.
******************************************************************************/
#include <mach/hi_hsan.h>
extern struct uart_driver g_st_uart_driver;
extern struct uart_port g_st_uart_port[];

static hi_int32 hi_uart_init(hi_void)
{    
    uart_add_one_port(&g_st_uart_driver, &g_st_uart_port[1]);
    return 0;
}

static hi_void hi_uart_exit(hi_void)
{
    uart_remove_one_port(&g_st_uart_driver, &g_st_uart_port[1]);
}

module_init(hi_uart_init);
module_exit(hi_uart_exit);

MODULE_DESCRIPTION("hsan serial port driver");
MODULE_AUTHOR("hisilicon Inc.");
MODULE_LICENSE("GPL");


