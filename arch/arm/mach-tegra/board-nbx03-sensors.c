/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * arch/arm/mach-tegra/board-nbx03-sensors.c
 *
 * Copyright (c) 2011, NVIDIA CORPORATION, All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of NVIDIA CORPORATION nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/i2c.h>
#include <linux/nct1008_hc.h>

#include <mach/gpio.h>

#include <generated/mach-types.h>

#include "gpio-names.h"
#include "board.h"
#include "board-nbx03.h"

#include <media/ius006.h>
#include <media/cmv59dx.h>
#include <linux/delay.h>
#include <linux/time.h>

#include <linux/lis331dlh_1.h>
#include <linux/ami304.h>
#include <linux/l3g4200dh.h>

#define NCT1008_THERM2_GPIO	TEGRA_GPIO_PN5

#define GPIO_PIN_5M_28V         TEGRA_GPIO_PL0
#define GPIO_PIN_5M_18V         TEGRA_GPIO_PL1
#define GPIO_PIN_5M_12V         TEGRA_GPIO_PL2
#define GPIO_PIN_5M_RST         TEGRA_GPIO_PL3
#define GPIO_PIN_5M_PWRDWN      TEGRA_GPIO_PL4
#define GPIO_PIN_5M_28V_VCM     TEGRA_GPIO_PL5
#define GPIO_PIN_VGA_EN         TEGRA_GPIO_PL6
#define GPIO_PIN_VGA_RST        TEGRA_GPIO_PL7

extern void tegra_throttling_enable(bool enable);

static int nbx03_camera_init(void)
{
	tegra_gpio_enable(GPIO_PIN_5M_28V);
	tegra_gpio_enable(GPIO_PIN_5M_18V);
	tegra_gpio_enable(GPIO_PIN_5M_12V);
	tegra_gpio_enable(GPIO_PIN_5M_RST);
	tegra_gpio_enable(GPIO_PIN_5M_PWRDWN);
	tegra_gpio_enable(GPIO_PIN_5M_28V_VCM);
	tegra_gpio_enable(GPIO_PIN_VGA_EN);
	tegra_gpio_enable(GPIO_PIN_VGA_RST);

    gpio_request(GPIO_PIN_5M_28V, "GPIO_PIN_5M_28V");
    gpio_request(GPIO_PIN_5M_18V, "GPIO_PIN_5M_18V");
    gpio_request(GPIO_PIN_5M_12V, "GPIO_PIN_5M_12V");
    gpio_request(GPIO_PIN_5M_RST, "GPIO_PIN_5M_RST");
    gpio_request(GPIO_PIN_5M_PWRDWN, "GPIO_PIN_5M_PWRDWN");
    gpio_request(GPIO_PIN_5M_28V_VCM, "GPIO_PIN_5M_28V_VCM");
    gpio_request(GPIO_PIN_VGA_EN, "GPIO_PIN_VGA_EN");
    gpio_request(GPIO_PIN_VGA_RST, "GPIO_PIN_VGA_RST");

    gpio_direction_output(GPIO_PIN_5M_28V, 0);
    gpio_direction_output(GPIO_PIN_5M_18V, 0);
    gpio_direction_output(GPIO_PIN_5M_12V, 0);
    gpio_direction_output(GPIO_PIN_5M_RST, 0);
    gpio_direction_output(GPIO_PIN_5M_PWRDWN, 0);
    gpio_direction_output(GPIO_PIN_5M_28V_VCM, 0);
    gpio_direction_output(GPIO_PIN_VGA_EN, 0);
    gpio_direction_output(GPIO_PIN_VGA_RST, 0);

	return 0;
}

static void nbx03_nct1008_init(void)
{
	tegra_gpio_enable(NCT1008_THERM2_GPIO);
	gpio_request(NCT1008_THERM2_GPIO, "temp_alert");
	gpio_direction_input(NCT1008_THERM2_GPIO);
}

static bool throttling_enable = false;

static void xxx_tegra_throttling_enable(bool enable)
{
	if ( throttling_enable != enable ) {
		throttling_enable = enable;
		tegra_throttling_enable(enable);
	}
}

static struct nct1008_platform_data nbx03_nct1008_pdata = {
	.supported_hwrev = true,
	.ext_range = false,
	.conv_rate = 0x08,
	.offset = 0,
	.hysteresis = 0,
	.shutdown_ext_limit = 115,
	.shutdown_local_limit = 80,
	.throttling_ext_limit = 75,
	.throttling_local_limit = 65,
	.alarm_fn = xxx_tegra_throttling_enable,
};

static struct i2c_board_info nbx03_i2c4_board_info[] = {
	{
		I2C_BOARD_INFO("nct1008", 0x4C),
		.platform_data = &nbx03_nct1008_pdata,
		.irq = TEGRA_GPIO_TO_IRQ(NCT1008_THERM2_GPIO),
	},
};

static struct lis331dlh_platform_data nbx03_lis331dlh_pdata = {
	.transformation_matrix = {
		 1,  0,  0,
		 0,  1,  0,
		 0,  0,  1,
	},
};

static struct ami304_platform_data nbx03_ami304_pdata = {
	.transformation_matrix = {
		 1,  0,  0,
		 0,  1,  0,
		 0,  0,  1,
	},
};

static struct l3g4200dh_platform_data nbx03_l3g4200dh_pdata = {
	.transformation_matrix = {
		 1,  0,  0,
		 0,  1,  0,
		 0,  0,  1,
	},
};

static struct i2c_board_info nbx03_i2c_sensors_info[] = {
#ifdef CONFIG_INPUT_LIS331DLH
	{
		I2C_BOARD_INFO("lis331dlh", 0x18), /* LIS331DLH */
		.platform_data = &nbx03_lis331dlh_pdata,
	},
#endif
#ifdef CONFIG_INPUT_AMI304
	{
		I2C_BOARD_INFO("ami304", 0x0f), /* AMI304 */
		.platform_data = &nbx03_ami304_pdata,
	},
#endif
#ifdef CONFIG_INPUT_L3G4200DH
	{
		I2C_BOARD_INFO("l3g4200dh", 0x68), /* L3G4200DH */
		.platform_data = &nbx03_l3g4200dh_pdata,
	},
#endif
};

static struct i2c_board_info nbx03_camera_board_info[] = {
	{
		I2C_BOARD_INFO("ius006", 0x3D),
	},
	{
		I2C_BOARD_INFO("cmv59dx", 0x3C),
	}
};

int __init nbx03_sensors_init(void)
{
	struct board_info BoardInfo;

	nbx03_camera_init();
	nbx03_nct1008_init();

	tegra_get_board_info(&BoardInfo);

	i2c_register_board_info(3, nbx03_i2c4_board_info,
		ARRAY_SIZE(nbx03_i2c4_board_info));

#if defined(CONFIG_INPUT_LIS331DLH) || defined(CONFIG_INPUT_AMI304) || defined(CONFIG_INPUT_L3G4200DH)
	i2c_register_board_info(0, nbx03_i2c_sensors_info,
		ARRAY_SIZE(nbx03_i2c_sensors_info));
#endif

	i2c_register_board_info(2, nbx03_camera_board_info,
			ARRAY_SIZE(nbx03_camera_board_info));

	return 0;
}
