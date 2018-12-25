/* Copyright (c) 2010-2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/i2c.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/backlight.h>
#include <linux/delay.h>
#include <mach/sys_config.h>

#undef USE_TPS61161_PWM

/* TODO:
 *  - select software and hardware pwm
 *  - ???
 *
 */

/* TPS61161 timing params */
#define DELAY_TSTART 	udelay(4)
#define DELAY_TEOS 	udelay(4)
#define DELAY_THLB 	udelay(3)
#define DELAY_TLLB 	udelay(7)
#define DELAY_THHB 	udelay(7)
#define DELAY_TLHB 	udelay(3)
#define DELAY_TESDET 	udelay(286)
#define DELAY_TESDELAY 	udelay(190)
#define DELAY_TESWIN 	mdelay(1)
#define DELAY_TOFF 	mdelay(4)

#define sys_get_wvalue(n)   (*((volatile unsigned int *)(n)))          /* word input */
#define sys_put_wvalue(n,c) (*((volatile unsigned int *)(n))  = (c))   /* word output */

#define A13_PWM_BASE (0xf1c20c00)
#define PWM_MAX_BRIGHTNESS 255

static void pb_gpio_set_pin(int);

spinlock_t tps61161_spin_lock;

static int prev_bl = 0;
static int bl_ctl_num = 0;

typedef struct
{
	char enable;
	unsigned int freq;
	unsigned int pre_scal;
	unsigned int active_state;
	unsigned int duty_ns;
	unsigned int period_ns;
	unsigned int entire_cycle;
	unsigned int active_cycle;
} disp_pwm_t;

typedef struct
{
	char enable;
	unsigned int active_state;
	unsigned int duty_ns;
	unsigned int period_ns;
} pwm_info_t;

static disp_pwm_t pwm[2];

void __iomem *pwm_io = NULL;

static unsigned int pb_pwm_read_reg(unsigned int offset)
{
	unsigned int value = 0;

	value = readl(A13_PWM_BASE + offset);

	return value;
}

static int pb_pwm_write_reg(unsigned int offset, unsigned int value)
{
	writel(value, A13_PWM_BASE + offset);

	return 0;
}

static int pb_pwm_is_enabled(void)
{
	unsigned int reg;

	reg = pb_pwm_read_reg(0x200);

	printk("<1> %s(): %s, %x\n", __func__, reg & (1 << 4) ? "enabled" : "", reg);

	return reg & (1 << 4);
}

static int pb_pwm_enable(unsigned int channel, char b_en)
{
	unsigned int tmp = 0;
	unsigned int offset = 0;
	unsigned int hdl;
	static user_gpio_set_t pwm0_gpio;

	printk("<1>[IN]  %s(): %d\n", __func__, b_en);

	if (pb_pwm_is_enabled() && b_en)
		return 0;
	if (!pb_pwm_is_enabled() && !b_en)
		return 0;

	if (! b_en) {
		pb_gpio_set_pin(0);
		msleep(1);
	}

	// PWM0 used, not PWM1
	if (channel == 0) {
		script_parser_fetch("BACKLIGHT", "bl_pwm", &pwm0_gpio, sizeof(user_gpio_set_t)/sizeof(int));

		if (b_en)
			pwm0_gpio.mul_sel = 2;
		else
			pwm0_gpio.mul_sel = 0;

		hdl = gpio_request(&pwm0_gpio, 1);
		gpio_release(hdl, 2);
	}

	tmp = pb_pwm_read_reg(0x200);
	switch (channel) {
		case 0: offset = 1<<4; 	break;
		case 1:	offset = 1<<19; break;
		default:
			// TODO:
			break;
	}
	if (b_en)
		tmp |= offset;
	else
		tmp &= (~offset);
	pb_pwm_write_reg(0x200,tmp);

	if (b_en) {
		msleep(1);
		pb_gpio_set_pin(1);
	}

	return 0;
}

//channel: pwm channel,0/1
//pwm_info->freq:  pwm freq, in hz
//pwm_info->active_state: 0:low level; 1:high level
static int pb_pwm_set_para(unsigned int channel, pwm_info_t * pwm_info)
{
	unsigned int pre_scal[11][2] = {{1,0xf}, {120,0}, {180,1}, {240,2}, {360,3}, {480,4}, {12000,8}, {24000,9}, {36000,0xa}, {48000,0xb}, {72000,0xc}};
	unsigned int pre_scal_id = 0, entire_cycle = 16, active_cycle = 12;
	unsigned int i=0, j=0, tmp=0;
	unsigned int freq;

	freq = 1000000 / pwm_info->period_ns;

	if (freq > 366) {
		pre_scal_id = 0;
		entire_cycle = 24000000 / freq;
	}
	else {
		for (i=1; i<11; i++) {
			for (j=16;; j+=16) {
				__u32 pwm_freq = 0;
				pwm_freq = 24000000 / (pre_scal[i][0] * j);
				if (abs(pwm_freq - freq) < abs(tmp - freq)) {
					tmp = pwm_freq;
					pre_scal_id = i;
					entire_cycle = j;
					printk("<1> pre_scal:%d, entire_cycle:%d, pwm_freq:%d\n", pre_scal[i][0], j, pwm_freq);
					printk("<1> ----%d\n", tmp);
				}
				else if ((tmp < freq) && (pwm_freq < tmp))
					break;
			}
		}
	}

	active_cycle = (pwm_info->duty_ns * entire_cycle + (pwm_info->period_ns/2)) / pwm_info->period_ns;

	pwm[channel].enable = pwm_info->enable;
	pwm[channel].freq = freq;
	pwm[channel].pre_scal = pre_scal[pre_scal_id][0];
	pwm[channel].active_state = pwm_info->active_state;
	pwm[channel].duty_ns = pwm_info->duty_ns;
	pwm[channel].period_ns = pwm_info->period_ns;
	pwm[channel].entire_cycle = entire_cycle;
	pwm[channel].active_cycle = active_cycle;

	if (channel == 0) {
		pb_pwm_write_reg(0x204, ((entire_cycle - 1)<< 16) | active_cycle);
		tmp = pb_pwm_read_reg(0x200) & 0xffffff00;
		//bit6:gatting the special clock for pwm0; bit5:pwm0  active state is high level
		tmp |= ((1<<6) | (pwm_info->active_state<<5) | pre_scal[pre_scal_id][1]);
		pb_pwm_write_reg(0x200,tmp);
	}
	else {
		pb_pwm_write_reg(0x208, ((entire_cycle - 1)<< 16) | active_cycle);
		tmp = pb_pwm_read_reg(0x200) & 0xff807fff;
		//bit21:gatting the special clock for pwm1; bit20:pwm1  active state is high level
		tmp |= ((1<<21) | (pwm_info->active_state<<20) | (pre_scal[pre_scal_id][1]<<15));
		pb_pwm_write_reg(0x200,tmp);
	}

	//pb_pwm_enable(channel, pwm_info->enable);

	return 0;
}

static int pb_pwm_get_para(unsigned int channel, pwm_info_t * p)
{
	p->enable		= pwm[channel].enable;
	p->active_state	= pwm[channel].active_state;
	p->duty_ns		= pwm[channel].duty_ns;
	p->period_ns		= pwm[channel].period_ns;

	return 0;
}

static int pb_pwm_set_duty_ns(unsigned int channel, unsigned int duty_ns)
{
	unsigned int active_cycle = 0;
	unsigned int tmp;

	active_cycle = (duty_ns * pwm[channel].entire_cycle + (pwm[channel].period_ns/2)) / pwm[channel].period_ns;

	if (channel == 0) {
		tmp = pb_pwm_read_reg(0x204);
		pb_pwm_write_reg(0x204,(tmp & 0xffff0000) | active_cycle);
	}
	else {
		tmp = pb_pwm_read_reg(0x208);
		pb_pwm_write_reg(0x208,(tmp & 0xffff0000) | active_cycle);
	}

	pwm[channel].duty_ns = duty_ns;

	return 0;
}

// TODO: Remove unused fields
struct tps61161_panel_common_pdata {
	uintptr_t hw_revision_addr;
	int gpio;
	bool bl_lock;
	spinlock_t bl_spinlock;
	int (*backlight_level)(int level, int max, int min);
	int (*pmic_backlight)(int level);
	int (*rotate_panel)(void);
	int (*backlight) (int level, int mode);
	int (*panel_num)(void);
	void (*panel_config_gpio)(int);
	int (*vga_switch)(int select_vga);
	int *gpio_num;
	int mdp_core_clk_rate;
	unsigned num_mdp_clk;
	int *mdp_core_clk_table;
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *mdp_bus_scale_table;
#endif
	int mdp_rev;
	u32 ov0_wb_size;  /* overlay0 writeback size */
	u32 ov1_wb_size;  /* overlay1 writeback size */
	u32 mem_hid;
	char cont_splash_enabled;
	char mdp_iommu_split_domain;
};

static struct tps61161_panel_common_pdata tps61161_backlight_pdata;

static struct platform_device tps61161bl_device = {
	.name		= "tps61161_bl",
	.dev		= {
		.platform_data	= &tps61161_backlight_pdata,
	},
	.id		= -1,
};

static struct platform_device *devices[] __initdata = {
	&tps61161bl_device,
};

static unsigned int pwm_gpio = 0;

static void pb_gpio_set_pin(int value)
{
	// Do not add debug output here, timing will go crazy

	if (! pwm_gpio)
		pwm_gpio = gpio_request_ex("BACKLIGHT", "bl_ctrl");

	gpio_write_one_pin_value(pwm_gpio, !!value, "bl_ctrl");
}

static void tps61161_send_bit(int bit_data)
{
	if (bit_data == 0) {
		pb_gpio_set_pin(0);;
		DELAY_TLLB;
		pb_gpio_set_pin(1);;
		DELAY_THLB;
	} else {
		pb_gpio_set_pin(0);;
		DELAY_TLHB;
		pb_gpio_set_pin(1);;
		DELAY_THHB;
	}
}

static void tps61161_send_byte(unsigned char byte_data)
{
	int n;

	for (n = 0; n < 8; n++) {
		tps61161_send_bit((byte_data & 0x80) ? 1 : 0);
		byte_data = byte_data << 1;
	}
}

static int tps61161_set_bl(int level, int max, int min)
{
	unsigned long flags;
	int ret = 0;

	printk("[DISP]%s: set backlight to %d, min = %d, max = %d\n", __func__, level, min, max);

	spin_lock_irqsave(&tps61161_spin_lock, flags); //disable local irq and preemption
	if (level == 0) {
		pb_gpio_set_pin(0);
		prev_bl = level;
		spin_unlock_irqrestore(&tps61161_spin_lock, flags);
		return 0;
	} else if (prev_bl == 0) {
		//printk("[DISP]%s: prev_bl = 0, enter easy scale first\n", __func__);
		pb_gpio_set_pin(0);
		DELAY_TOFF;
		pb_gpio_set_pin(1);
		DELAY_TESDELAY;
		pb_gpio_set_pin(0);
		DELAY_TESDET;
		pb_gpio_set_pin(1);
		DELAY_TESWIN;
	}

	prev_bl = level;

	/* device address byte = 0x72 */
	tps61161_send_byte(0x72);

	/* t-EOS and t-start */
	pb_gpio_set_pin(0);
	DELAY_TEOS;
	pb_gpio_set_pin(1);
	DELAY_TSTART;

	/* data byte */
	tps61161_send_byte(level & 0x1F); //RFA = 0, address bit = 00, 5 bit level
	/* t-EOS */
	pb_gpio_set_pin(0);
	DELAY_TEOS;
	pb_gpio_set_pin(1);
	DELAY_TSTART;
	spin_unlock_irqrestore(&tps61161_spin_lock, flags);

	return ret;
}

static ssize_t attr_max_brightness_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
#if defined(USE_TPS61161_PWM)
	return sprintf(buf, "%s\n", "31");
#else
	return sprintf(buf, "%d\n", PWM_MAX_BRIGHTNESS);
#endif
}

static ssize_t attr_brightness_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{
	unsigned int v, duty_ns;

	if (! pb_pwm_is_enabled()) {
		printk("%s(): PWM disabled, do not set brightness\n", __func__);
		return size;
	}
	v = simple_strtoul(buf, NULL, 10);

#if defined(USE_TPS61161_PWM)
	tps61161_set_bl(v, 32, 0);
#else
	prev_bl = PWM_MAX_BRIGHTNESS - v;
	duty_ns = (prev_bl * 255 * pwm[0].period_ns / 256 + 128) / 256;
        pb_pwm_set_duty_ns(0, duty_ns);
#endif

	return size;
}

static ssize_t attr_brightness_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", PWM_MAX_BRIGHTNESS - prev_bl);
}

static ssize_t attr_blpower_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{
	unsigned int enable, duty_ns;

	enable = simple_strtoul(buf, NULL, 10);

	pb_pwm_enable(0, enable);

	if (! enable) {
		prev_bl = PWM_MAX_BRIGHTNESS - 1;
		duty_ns = (prev_bl * 255 * pwm[0].period_ns / 256 + 128) / 256;
		pb_pwm_set_duty_ns(0, duty_ns);
	}

	return size;
}

static ssize_t attr_blpower_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", !!pb_pwm_is_enabled());
}

static ssize_t attr_dumpa_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "DUMPA:\n\t0x%08X\n\t0x%08X\n\t0x%08x\n", pb_pwm_read_reg(0x200), pb_pwm_read_reg(0x204), pb_pwm_read_reg(0x208));
}

static struct device_attribute brightness =
	__ATTR(brightness, 0666, attr_brightness_show, attr_brightness_store);
static struct device_attribute max_brightness =
	__ATTR(max_brightness, 0666, attr_max_brightness_show, NULL);
static struct device_attribute bl_power =
	__ATTR(bl_power, 0666, attr_blpower_show, attr_blpower_store);
static struct device_attribute dumpa =
	__ATTR(dumpa, 0666, attr_dumpa_show, NULL);

static int __devinit tps61161_backlight_probe(struct platform_device *pdev)
{
	int retval = 0;
	struct tps61161_panel_common_pdata *bl;

	printk("[DISP]%s\n", __func__);

	bl = (struct tps61161_panel_common_pdata *)pdev->dev.platform_data;
	if (bl) {
		bl->backlight_level = tps61161_set_bl;
		bl_ctl_num = bl->gpio;
		printk("[DISP]%s: bl_ctl_num = %d\n", __func__, bl_ctl_num);

		/* FIXME: if continuous splash is enabled, set prev_bl to
		* a non-zero value to avoid entering easyscale.
		*/
		if (bl->cont_splash_enabled)
			prev_bl = 1;
	}

	retval = device_create_file(&pdev->dev, &brightness);
	if (retval)
		printk(KERN_ERR "%s: Error, could not create attribute (brightness)\n", __func__);

	retval = device_create_file(&pdev->dev, &max_brightness);
	if (retval)
		printk(KERN_ERR "%s: Error, could not create attribute (max_brightness)\n", __func__);

	retval = device_create_file(&pdev->dev, &bl_power);
	if (retval)
		printk(KERN_ERR "%s: Error, could not create attribute (bl_power)\n", __func__);

	retval = device_create_file(&pdev->dev, &dumpa);
	if (retval)
		printk(KERN_ERR "%s: Error, could not create attribute (dumpa)\n", __func__);

	return 0;
}

static unsigned long reg1, reg2, reg3;

static int tps61161_suspend(struct platform_device *p, pm_message_t state)
{
	reg1 = pb_pwm_read_reg(0x200);
	reg2 = pb_pwm_read_reg(0x204);
	reg3 = pb_pwm_read_reg(0x208);

	return 0;
}

static int tps61161_resume(struct platform_device *p)
{
	pb_pwm_write_reg(0x200, reg1);
	pb_pwm_write_reg(0x204, reg2);
	pb_pwm_write_reg(0x208, reg3);

	return 0;
}

static struct platform_driver this_driver = {
	.probe  = tps61161_backlight_probe,
	.suspend = tps61161_suspend,
	.resume = tps61161_resume,
	.driver = {
		.name   = "tps61161_bl",
	},
};

static int __init tps61161_backlight_init(void)
{
	static pwm_info_t p_tmp;

#if 1
	pb_pwm_get_para(0, &p_tmp);
	p_tmp.enable = 1;
	p_tmp.active_state = 1;

	p_tmp.period_ns = 1000000 / 10000;
	p_tmp.duty_ns = (192 * p_tmp.period_ns) / 256;

	pb_pwm_set_para(0, &p_tmp);
#endif

	spin_lock_init(&tps61161_spin_lock);
	platform_add_devices(devices, ARRAY_SIZE(devices));

	return platform_driver_register(&this_driver);
}

module_init(tps61161_backlight_init);
