/*
*************************************************************************************
*                         			      Linux
*					           USB Host Controller Driver
*
*				        (c) Copyright 2006-2012, SoftWinners Co,Ld.
*							       All Rights Reserved
*
* File Name 	: usb_hcd_servers.c
*
* Author 		: javen
*
* Description 	: USB 主机控制器驱动服务函数集
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	  2011-4-14            1.0          create this file
*
*************************************************************************************
*/
#include  "../include/sw_usb_config.h"
#include  "../include/sw_usb_board.h"
#include  "usb_hcd_servers.h"

int sw_usb_disable_ehci(__u32 usbc_no);
int sw_usb_enable_ehci(__u32 usbc_no);
int sw_usb_disable_ohci(__u32 usbc_no);
int sw_usb_enable_ohci(__u32 usbc_no);

static enum sw_usbc_type controller_type = SW_USB_EHCI;


/*
*******************************************************************************
*                     sw_usb_disable_hcd
*
* Description:
*    void
*
* Parameters:
*    void
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
int sw_usb_disable_hcd(__u32 usbc_no)
{

	if(usbc_no == 0){
#if defined(CONFIG_USB_SW_SUN5I_USB0_OTG) || defined(USB_SW_SUN5I_USB0_HOST_ONLY)
		sw_usb_disable_hcd0();
#endif
	}else if(usbc_no == 1){
#if defined(CONFIG_USB_SW_SUN5I_EHCI0) || defined(CONFIG_USB_SW_SUN5I_EHCI0_MODULE)
		if(controller_type != SW_USB_OHCI){
			sw_usb_disable_ehci(usbc_no);
		}
#endif

#if defined(CONFIG_USB_SW_SUN5I_OHCI0) || defined(CONFIG_USB_SW_SUN5I_OHCI0_MODULE)
		if(controller_type != SW_USB_EHCI){
			sw_usb_disable_ohci(usbc_no);
		}
#endif
	}else if(usbc_no == 2){
#if defined(CONFIG_USB_SW_SUN5I_EHCI1)
		if(controller_type != SW_USB_OHCI){
			sw_usb_disable_ehci(usbc_no);
		}
#endif

#if defined(CONFIG_USB_SW_SUN5I_OHCI1)
		if(controller_type != SW_USB_EHCI){
			sw_usb_disable_ohci(usbc_no);
		}
#endif
	}else{
		DMSG_PANIC("ERR: unkown usbc_no(%d)\n", usbc_no);
		return -1;
	}

    return 0;
}
EXPORT_SYMBOL(sw_usb_disable_hcd);

/*
*******************************************************************************
*                     sw_usb_enable_hcd
*
* Description:
*    void
*
* Parameters:
*    void
*
* Return value:
*    void
*
* note:
*    void
*
*******************************************************************************
*/
int sw_usb_enable_hcd(__u32 usbc_no)
{
	char *set_usbc = NULL;
    int ret = 0;

	if(usbc_no == 0){
		set_usbc = SET_USB0;
	}else if(usbc_no == 1){
		set_usbc = SET_USB1;
	}else{
		set_usbc = SET_USB2;
	}

	/* ----------get usbc_type------------- */
	ret = script_parser_fetch(set_usbc, KEY_USB_CONTROLLER_TYPE, (int *)&controller_type, 64);
	if(ret != 0){
		printk("ERR: script_parser_fetch usb_controller_type failed\n");
	}

	if(usbc_no == 0){
#if defined(CONFIG_USB_SW_SUN5I_USB0_OTG) || defined(USB_SW_SUN5I_USB0_HOST_ONLY)
		sw_usb_enable_hcd0();
#endif
	}else if(usbc_no == 1){
#if defined(CONFIG_USB_SW_SUN5I_EHCI0) || defined(CONFIG_USB_SW_SUN5I_EHCI0_MODULE)
        if(controller_type != SW_USB_OHCI){
			sw_usb_enable_ehci(usbc_no);
        }
#endif
#if defined(CONFIG_USB_SW_SUN5I_OHCI0) || defined(CONFIG_USB_SW_SUN5I_OHCI0_MODULE)
		if(controller_type != SW_USB_EHCI){
			sw_usb_enable_ohci(usbc_no);
		}
#endif
	}else if(usbc_no == 2){
#if defined(CONFIG_USB_SW_SUN5I_EHCI1)
		if(controller_type != SW_USB_OHCI){
			sw_usb_enable_ehci(usbc_no);
		}
#endif

#if defined(CONFIG_USB_SW_SUN5I_OHCI1)
		if(controller_type != SW_USB_EHCI){
			sw_usb_enable_ohci(usbc_no);
		}
#endif
	}else{
		DMSG_PANIC("ERR: unkown usbc_no(%d)\n", usbc_no);
		return -1;
	}

    return 0;
}
EXPORT_SYMBOL(sw_usb_enable_hcd);





