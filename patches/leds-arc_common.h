#ifndef LED_ARC_COMMON
#define LED_ARC_COMMON
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define LED0_BRIGHT 0x07
#define LED1_BRIGHT 0x08
#define LED2_BRIGHT 0x09
#define LED3_BRIGHT 0x0a

#define LED0_COLOR_R 0x0f
#define LED0_COLOR_G 0x10
#define LED0_COLOR_B 0x11
#define LED1_COLOR_R 0x12
#define LED1_COLOR_G 0x13
#define LED1_COLOR_B 0x14
#define LED2_COLOR_R 0x15
#define LED2_COLOR_G 0x16
#define LED2_COLOR_B 0x17
#define LED3_COLOR_R 0x18
#define LED3_COLOR_G 0x19
#define LED3_COLOR_B 0x1a

#define DEVNAME "arc_leds"

#define LED_NUM 4
#define LED_COLOR 4
#define I2C_BUS_ID 0
#define LED_I2C_ADDR 0x29
#define LED_NAME "arc_lp5018"

#define LED_GPIO 392


/****************** STRUCT *********************/
struct i2c_client *reg_client;
struct i2c_adapter *adap;

static struct task_struct *startup_thread;
static int startup_led_color[3] = { 223, 234, 183 };
static int startup_led_sleep = 1000;

static struct platform_device arc_led_device = {
    .name = DEVNAME,
};
static struct i2c_board_info arc_led_i2c_info = {

	I2C_BOARD_INFO(LED_NAME, LED_I2C_ADDR),

};
/****************** I2C *********************/
void led_i2c_set(struct i2c_client *client, int set_address ,int set_value);
void led_i2c_get(struct i2c_client *client, int get_address ,char* result);
static void reg_i2c_device(void);
static void unreg_i2c_device(void);

/************RED************/
static void arc_red_led0_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_red_led1_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_red_led2_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_red_led3_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static enum led_brightness arc_red_led0_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_red_led1_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_red_led2_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_red_led3_color_get(struct led_classdev * led_dev);

/************GREEN************/
static void arc_green_led0_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_green_led1_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_green_led2_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_green_led3_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static enum led_brightness arc_green_led0_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_green_led1_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_green_led2_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_green_led3_color_get(struct led_classdev * led_dev);

/************BLUE************/
static void arc_blue_led0_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_blue_led1_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_blue_led2_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_blue_led3_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static enum led_brightness arc_blue_led0_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_blue_led1_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_blue_led2_color_get(struct led_classdev * led_dev);
static enum led_brightness arc_blue_led3_color_get(struct led_classdev * led_dev);
/************BRIGHT************/
static void arc_led0_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_led1_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_led2_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static void arc_led3_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value);
static enum led_brightness arc_led0_bright_get(struct led_classdev * led_dev);
static enum led_brightness arc_led1_bright_get(struct led_classdev * led_dev);
static enum led_brightness arc_led2_bright_get(struct led_classdev * led_dev);
static enum led_brightness arc_led3_bright_get(struct led_classdev * led_dev);

static struct led_classdev led_reg_cdev[LED_NUM*LED_COLOR] = 
{
	{
		.brightness_set = arc_red_led0_color_set,
		.brightness_get = arc_red_led0_color_get,
		.name = "led0-R"
	},
	{
		.brightness_set = arc_red_led1_color_set,
		.brightness_get = arc_red_led1_color_get,
		.name = "led1-R"
	},
	{
		.brightness_set = arc_red_led2_color_set,
		.brightness_get = arc_red_led2_color_get,
		.name = "led2-R"
	},
	{
		.brightness_set = arc_red_led3_color_set,
		.brightness_get = arc_red_led3_color_get,
		.name = "led3-R"
	},
	{
		.brightness_set = arc_green_led0_color_set,
		.brightness_get = arc_green_led0_color_get,
		.name = "led0-G"
	},
	{
		.brightness_set = arc_green_led1_color_set,
		.brightness_get = arc_green_led1_color_get,
		.name = "led1-G"
	},
	{
		.brightness_set = arc_green_led2_color_set,
		.brightness_get = arc_green_led2_color_get,
		.name = "led2-G"
	},
	{
		.brightness_set = arc_green_led3_color_set,
		.brightness_get = arc_green_led3_color_get,
		.name = "led3-G"
	},
	{
		.brightness_set = arc_blue_led0_color_set,
		.brightness_get = arc_blue_led0_color_get,
		.name = "led0-B"
	},
	{
		.brightness_set = arc_blue_led1_color_set,
		.brightness_get = arc_blue_led1_color_get,
		.name = "led1-B"
	},
	{
		.brightness_set = arc_blue_led2_color_set,
		.brightness_get = arc_blue_led2_color_get,
		.name = "led2-B"
	},
	{
		.brightness_set = arc_blue_led3_color_set,
		.brightness_get = arc_blue_led3_color_get,
		.name = "led3-B"
	},
	{
		.brightness_set = arc_led0_bright_set,
		.brightness_get = arc_led0_bright_get,
		.name = "led0"
	},
	{
		.brightness_set = arc_led1_bright_set,
		.brightness_get = arc_led1_bright_get,
		.name = "led1"
	},
	{
		.brightness_set = arc_led2_bright_set,
		.brightness_get = arc_led2_bright_get,
		.name = "led2"
	},
	{
		.brightness_set = arc_led3_bright_set,
		.brightness_get = arc_led3_bright_get,
		.name = "led3"
	}
}; 


#define LED_FUNCTION(color, number, value) \
switch(number) \
{ case 0: arc_ ## color ## _led0 ## _color_set(NULL, value); break; \
  case 1: arc_ ## color ## _led2 ## _color_set(NULL, value); break; \
  case 2: arc_ ## color ## _led1 ## _color_set(NULL, value); break; \
  case 3: arc_ ## color ## _led3 ## _color_set(NULL, value); break;  }
#endif
