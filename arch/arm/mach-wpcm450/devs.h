/* arch/arm/mach-wpcm450/devs.h
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * Header file for wpcm450 standard platform devices
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *   23-Aug-2006  clyu created file
*/
#include <linux/autoconf.h>
#include <linux/platform_device.h>


#ifdef AVI_REMOVE
extern struct platform_device wpcm_device_fmi;//zswan
extern struct platform_device wpcm_device_sc;//zswan
extern struct platform_device wpcm_device_usb;
extern struct platform_device wpcm_device_lcd;
extern struct platform_device wpcm_device_wdt;
extern struct platform_device wpcm_device_i2c;
extern struct platform_device wpcm_device_iis;
extern struct platform_device wpcm_device_rtc;
extern struct platform_device wpcm_device_adc;
extern struct platform_device wpcm_device_sdi;

extern struct platform_device wpcm_device_spi0;
extern struct platform_device wpcm_device_spi1;

extern struct platform_device wpcm_device_nand;
#endif //AVI_REMOVE

extern struct platform_device wpcm_device_timer0;
extern struct platform_device wpcm_device_timer1;
extern struct platform_device wpcm_device_timer2;
extern struct platform_device wpcm_device_timer3;
extern struct platform_device wpcm_device_timer4;

#ifdef AVI_REMOVE
extern struct platform_device wpcm_device_usbgadget;
#endif //AVI_REMOVE



