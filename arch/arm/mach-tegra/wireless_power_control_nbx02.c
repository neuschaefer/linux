/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <mach/gpio.h>
#include "wireless_power_control.h"

#include "devices.h"
#include "gpio-names.h"

//#define SYSFS_WIRELESSPOWER_DEBUG_PRINTS 1

#ifdef SYSFS_WIRELESSPOWER_DEBUG_PRINTS
#define PRINT_WIRELESSPOWER(x) pr_debug x
#else
#define PRINT_WIRELESSPOWER(x)
#endif

static int wireless_power_flag = 0;

// WLAN_CHIP_PWD_N  GPIO_PP2
#define GPIO_WLAN_CHIP_PWD_N   TEGRA_GPIO_PP2

// BT_RST_N         GPIO_PP1
#define GPIO_BT_RST_N          TEGRA_GPIO_PP1

// GPS_PWR_EN       GPIO_PP0
#define GPIO_GPS_PWR_EN        TEGRA_GPIO_PP0

// WWAN_PWR_EN      GPIO_PV1
#define GPIO_WWAN_PWR_EN       TEGRA_GPIO_PV1

// WWAN_RF_ON_N     GPIO_PV2       H/z=off L=on
#define GPIO_WWAN_RF_ON_N      TEGRA_GPIO_PV2

static bool s_hWlanChipPwdN_Pin;
static bool s_hBtRstN_Pin;
static bool s_hGpsPwrEn_Pin;
static bool s_hWWanPwrEn_Pin;
static bool s_hWWanRfOnN_Pin;

static int s_wifi_onoff = 0;
static int s_bt_onoff = 0;
static int s_gps_onoff = 0;
static int s_wwan_onoff = 0;      // initial off
static int s_wwan_rf_onoff = 0;   // initial off

#define GPIODIRECTION_IN   0
#define GPIODIRECTION_OUT  1

static int nbx02_gpio_request(int gpio_num, const char *gpio_name, int direction, int value)
{
	int ret;

	ret = gpio_request(gpio_num, gpio_name);
	if (ret < 0) {
		pr_err("gpio req err: %d,%s\n", gpio_num, gpio_name);
		return ret;
	}
	if ( direction == GPIODIRECTION_IN ) { 
		ret = gpio_direction_input(gpio_num);
	} else {
		ret = gpio_direction_output(gpio_num, value);
	}
	if (ret < 0) {
		pr_err("gpio dir err: %d,%s\n", gpio_num, gpio_name);
		gpio_free(gpio_num);
		return ret;
	}

	tegra_gpio_enable(gpio_num);
	pr_info("gpio requested: %d as %s\n", gpio_num, gpio_name);

	return 0;
}

static int init(void)
{
	int ret;
	PRINT_WIRELESSPOWER(("wpc: init\n"));
	
	{
		int port = GPIO_WLAN_CHIP_PWD_N;
		int value = (s_wifi_onoff ? 1 : 0);  // power down _N
		if (s_hWlanChipPwdN_Pin == false) {
			ret = nbx02_gpio_request(port, "WlanChipPwdN", GPIODIRECTION_OUT, value);
			if (ret < 0) {
				PRINT_WIRELESSPOWER(("fail:s_hWlanChipPwdN_Pin = nbx02_gpio_request( s_hGpio, port, pin );\n"));
				return ret;
			}
		}
		gpio_set_value(port, value);
	}
	
	{
		int port = GPIO_BT_RST_N;
		int value = (s_bt_onoff ? 1 : 0);  // bt reset _N
		if (s_hBtRstN_Pin == false) {
			ret = nbx02_gpio_request(port, "BtRstN", GPIODIRECTION_OUT, value);
			if (ret < 0) {
				PRINT_WIRELESSPOWER(("fail:s_hBtRstN_Pin = nbx02_gpio_request( s_hGpio, port, pin );\n"));
				return ret;
			}
		}
		gpio_set_value(port, value);
	}
	
	{
		int port = GPIO_GPS_PWR_EN;
		int value = (s_gps_onoff ? 1 : 0);  // gps power enable
		if (s_hGpsPwrEn_Pin == false) {
			ret = nbx02_gpio_request(port, "GpsPwrEn", GPIODIRECTION_OUT, value);
			if (ret < 0) {
				PRINT_WIRELESSPOWER(("fail:s_hGpsPwrEn_Pin = nbx02_gpio_request( s_hGpio, port, pin );\n"));
				return ret;
			}
		}
		gpio_set_value(port, value);
	}
	
	{
		int port = GPIO_WWAN_RF_ON_N;
		int value = (s_wwan_rf_onoff ? 0 : 1);  // wwan rf on _N
		if (s_hWWanRfOnN_Pin == false) {
			ret = nbx02_gpio_request(port, "WWanRfOnN", GPIODIRECTION_OUT, value);
			if (ret < 0) {
				PRINT_WIRELESSPOWER(("fail:s_hWWanRfOnN_Pin = nbx02_gpio_request( s_hGpio, port, pin );\n"));
				return ret;
			}
		}
		gpio_set_value(port, value);
	}
	
	{
		int port = GPIO_WWAN_PWR_EN;
		int value = (s_wwan_onoff ? 1 : 0);  // wwan power enable
		if (s_hWWanPwrEn_Pin == false) {
			ret = nbx02_gpio_request(port, "WWanPwrEn", GPIODIRECTION_OUT, value);
			if (ret < 0) {
				PRINT_WIRELESSPOWER(("fail:s_hWWanPwrEn_Pin = nbx02_gpio_request( s_hGpio, port, pin );\n"));
				return ret;
			}
		}
		gpio_set_value(port, value);
	}
	
	return 0;
}

static int wifi_powercontrol(int on_off)
{
	PRINT_WIRELESSPOWER(("in wifi_powercontrol on_off=%d\n", on_off));
	
	if (on_off == 1){
		/*RESET*/
		gpio_set_value(GPIO_WLAN_CHIP_PWD_N, 0);
		msleep(20);
		/*PWR_ON*/
		gpio_set_value(GPIO_WLAN_CHIP_PWD_N, 1);  // power down _N
		/*SAVE*/
		s_wifi_onoff = 1;
		return 0;
	}
	
	if (on_off == 0){
		/*RESET*/
		gpio_set_value(GPIO_WLAN_CHIP_PWD_N, 0);
		/*SAVE*/
		s_wifi_onoff = 0;
		return 0;
	}
	
	return -EINVAL;
}

static int bluetooth_powercontrol(int on_off)
{
	PRINT_WIRELESSPOWER(("in bluetooth_powercontrol on_off=%d\n", on_off));
	
	if (on_off == 1){
		/*RESET*/
		gpio_set_value(GPIO_BT_RST_N, 0);  // bt reset _N
		msleep(20);
		/*PWR_ON*/
		gpio_set_value(GPIO_BT_RST_N, 1);  // bt reset _N
		/*SAVE*/
		s_bt_onoff = 1;
		return 0;
	}
	
	if (on_off == 0){
		/*RESET*/
		gpio_set_value(GPIO_BT_RST_N, 0);  // bt reset _N
		/*SAVE*/
		s_bt_onoff = 0;
		return 0;
	}
	
	return -EINVAL;
}

static int gps_powercontrol(int on_off)
{
	PRINT_WIRELESSPOWER(("in gps_powercontrol on_off=%d\n", on_off));
	
	if (on_off == 1){
		/*RESET*/
		gpio_set_value(GPIO_GPS_PWR_EN, 0);  // gps power enable
		msleep(20);
		/*PWR_ON*/
		gpio_set_value(GPIO_GPS_PWR_EN, 1);  // gps power enable
		/*SAVE*/
		s_gps_onoff = 1;
		return 0;
	}
	
	if (on_off == 0){
		/*RESET*/
		gpio_set_value(GPIO_GPS_PWR_EN, 0);  // gps power enable
		/*SAVE*/
		s_gps_onoff = 0;
		return 0;
	}
	
	return -EINVAL;
}

static int wwan_powercontrol(int on_off)
{
	PRINT_WIRELESSPOWER(("in wwan_powercontrol on_off=%d\n", on_off));
	
	if (on_off == 1){
		/*RESET*/
		gpio_set_value(GPIO_WWAN_PWR_EN, 0);  // wwan power enable
		msleep(20);
		/*PWR_ON*/
		gpio_set_value(GPIO_WWAN_PWR_EN, 1);  // wwan power enable
		/*SAVE*/
		s_wwan_onoff = 1;
		return 0;
	}
	
	if (on_off == 0){
		/*RESET*/
		gpio_set_value(GPIO_WWAN_PWR_EN, 0);  // wwan power enable
		/*SAVE*/
		s_wwan_onoff = 0;
		return 0;
	}
	
	return -EINVAL;
}

static int wwan_rf_powercontrol(int on_off)
{
	PRINT_WIRELESSPOWER(("in wwan_rf_powercontrol on_off=%d\n", on_off));
	
	if (on_off == 1){
		/*RESET*/
		gpio_set_value(GPIO_WWAN_RF_ON_N, 1);  // wwan rf on _N
		msleep(20);
		/*PWR_ON*/
		gpio_set_value(GPIO_WWAN_RF_ON_N, 0);  // wwan rf on _N
		/*SAVE*/
		s_wwan_rf_onoff = 1;
		return 0;
	}
	
	if (on_off == 0){
		/*RESET*/
		gpio_set_value(GPIO_WWAN_RF_ON_N, 1);  // wwan rf on _N
		/*SAVE*/
		s_wwan_rf_onoff = 0;
		return 0;
	}
	
	return -EINVAL;
}

static int wifi_powerstatus(int *p_on_off)
{
	(*p_on_off) = s_wifi_onoff;
	return 0;
}

static int bluetooth_powerstatus(int *p_on_off)
{
	(*p_on_off) = s_bt_onoff;
	return 0;
}

static int gps_powerstatus(int *p_on_off)
{
	(*p_on_off) = s_gps_onoff;
	return 0;
}

static int wwan_powerstatus(int *p_on_off)
{
	(*p_on_off) = s_wwan_onoff;
	return 0;
}

static int wwan_rf_powerstatus(int *p_on_off)
{
	(*p_on_off) = s_wwan_rf_onoff;
	return 0;
}


static int wireless_power_reset(void)
{
	int r; 
	if (wireless_power_flag == 0){
		r = init();
		if (r < 0) {
			return r;
		}
		wireless_power_flag = 1;
	}
	return 0;
}

//static int wpc_init(void)
//{
//PRINT_WIRELESSPOWER(("WPC_INIT---------------------\n"));
//    wireless_power_reset();
//    return 0;
//}
//late_initcall(wpc_init);

static int wireless_power_control_locked(int module, int on_off)
{
	int now;
	int r; 
	PRINT_WIRELESSPOWER(("in wireless_power_control\n"));
	PRINT_WIRELESSPOWER(("module=%d on_off=%d\n",module, on_off));
	r = wireless_power_reset();
	if ( r != 0 ){
		return r;
	}
	switch(module){
	case WPC_MODULE_WIFI:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_WIFI\n"));
		r = wifi_powerstatus(&now); 
		if ( r != 0 ){
			return r;
		}
		if (on_off == 1 && now == 1) {
			return 0;    
		} else if (on_off == 1 && now == 0) {
			return wifi_powercontrol(on_off);
		} else if (on_off == 0 && now == 1) {
			return wifi_powercontrol(on_off);
		} else if (on_off == 0 && now == 0) {
			return 0;    
		}
	case WPC_MODULE_BLUETOOTH:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_BLUETOOTH\n"));
		r = bluetooth_powerstatus(&now);     
		if ( r != 0 ){
			return r;
		}
		if (on_off == 1 && now == 1) {
			return 0;    
		} else if (on_off == 1 && now == 0) {
			return bluetooth_powercontrol(on_off);
		} else if (on_off == 0 && now == 1) {
			return bluetooth_powercontrol(on_off);
		} else if (on_off == 0 && now == 0) {
			return 0;    
		}
	case WPC_MODULE_GPS:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_GPS\n"));
		r = gps_powerstatus(&now);     
		if ( r != 0 ){
			return r;
		}
		if (on_off == 1 && now == 1) {
			return 0;    
		} else if (on_off == 1 && now == 0) {
			return gps_powercontrol(on_off);
		} else if (on_off == 0 && now == 1) {
			return gps_powercontrol(on_off);
		} else if (on_off == 0 && now == 0) {
			return 0;    
		}
	case WPC_MODULE_WWAN:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_WWAN\n"));
		r = wwan_powerstatus(&now);
		if ( r != 0 ){
			return r;
		}
		if (on_off == 1 && now == 1) {
			return 0;    
		} else if (on_off == 1 && now == 0) {
			return wwan_powercontrol(on_off);
		} else if (on_off == 0 && now == 1) {
			return wwan_powercontrol(on_off);
		} else if (on_off == 0 && now == 0) {
			return 0;    
		}
	case WPC_MODULE_WWAN_RF:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_WWAN_RF\n"));
		r = wwan_rf_powerstatus(&now);
		if ( r != 0 ){
			return r;
		}
		if (on_off == 1 && now == 1) {
			return 0;    
		} else if (on_off == 1 && now == 0) {
			return wwan_rf_powercontrol(on_off);
		} else if (on_off == 0 && now == 1) {
			return wwan_rf_powercontrol(on_off);
		} else if (on_off == 0 && now == 0) {
			return 0;    
		}
	}
	return -EINVAL;
}

static DEFINE_MUTEX(s_mutex);
int wireless_power_control(int module, int on_off)
{
	int r;
	mutex_lock(&s_mutex); 
	r = wireless_power_control_locked(module, on_off);
	mutex_unlock(&s_mutex); 
	return r;
}

static int wireless_power_status_locked(int module, int* p_on_off)
{
	int r;
	PRINT_WIRELESSPOWER(("in wireless_power_status\n"));
	PRINT_WIRELESSPOWER(("module=%d \n",module));
	r = wireless_power_reset();
	if ( r != 0 ){
		return r;
	}
	switch(module){
	case WPC_MODULE_WIFI:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_WIFI\n"));
		return wifi_powerstatus(p_on_off);
	case WPC_MODULE_BLUETOOTH:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_BLUETOOTH\n"));
		return bluetooth_powerstatus(p_on_off);
	case WPC_MODULE_GPS:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_GPS\n"));
		return gps_powerstatus(p_on_off);
	case WPC_MODULE_WWAN:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_WWAN\n"));
		return wwan_powerstatus(p_on_off);
	case WPC_MODULE_WWAN_RF:
		PRINT_WIRELESSPOWER(("in WPC_MODULE_WWAN_RF\n"));
		return wwan_rf_powerstatus(p_on_off);
	}
	return -EINVAL;
}

int wireless_power_status(int module, int* p_on_off)
{
	int r;
	mutex_lock(&s_mutex); 
	r = wireless_power_status_locked(module, p_on_off);
	mutex_unlock(&s_mutex); 
	return r;
}

