/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * arch/arm/mach-tegra/board-nbx02-sensors.c
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
#include "board-nbx02.h"

#include <media/tc35892.h>
#include <linux/regulator/consumer.h>
#include <linux/err.h>
#include <mach/tegra_odm_fuses.h>

#include <linux/lis331dlh_1.h>
#include <linux/ami304.h>
#include <linux/l3g4200dh.h>

#define NCT1008_THERM2_GPIO	TEGRA_GPIO_PN5

#define GPIO_PIN_5M_28V_VCM     TEGRA_GPIO_PL5

static struct regulator *reg_cam_vcm; /* LDO8 */

extern void tegra_throttling_enable(bool enable);

static int nbx02_camera_init(void)
{
    return 0;
}

static void nbx02_nct1008_init(void)
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

static struct nct1008_platform_data nbx02_nct1008_pdata = {
	.supported_hwrev = true,
	.ext_range = true,
	.conv_rate = 0x08,
	.offset = 0,
	.hysteresis = 0,
	.shutdown_ext_limit = 115,
	.shutdown_local_limit = 80,
	.throttling_ext_limit = 75,
	.throttling_local_limit = 65,
	.alarm_fn = xxx_tegra_throttling_enable,
};

static struct i2c_board_info nbx02_i2c4_board_info[] = {
	{
		I2C_BOARD_INFO("nct1008", 0x4C),
		.platform_data = &nbx02_nct1008_pdata,
		.irq = TEGRA_GPIO_TO_IRQ(NCT1008_THERM2_GPIO),
	},
};

static struct lis331dlh_platform_data nbx02_lis331dlh_pdata = {
	.transformation_matrix = {
		-1,  0,  0,
		 0,  1,  0,
		 0,  0, -1,
	},
};

static struct ami304_platform_data nbx02_ami304_pdata = {
	.transformation_matrix = {
		-1,  0,  0,
		 0,  1,  0,
		 0,  0, -1,
	},
};

static struct l3g4200dh_platform_data nbx02_l3g4200dh_pdata = {
	.transformation_matrix = {
		-1,  0,  0,
		 0,  1,  0,
		 0,  0, -1,
	},
};

static struct i2c_board_info nbx02_i2c_sensors_info[] = {
#ifdef CONFIG_INPUT_LIS331DLH
	{
		I2C_BOARD_INFO("lis331dlh", 0x18), /* LIS331DLH */
		.platform_data = &nbx02_lis331dlh_pdata,
	},
#endif
#ifdef CONFIG_INPUT_AMI304
	{
		I2C_BOARD_INFO("ami304", 0x0f), /* AMI304 */
		.platform_data = &nbx02_ami304_pdata,
	},
#endif
#ifdef CONFIG_INPUT_L3G4200DH
	{
		I2C_BOARD_INFO("l3g4200dh", 0x68), /* L3G4200DH */
		.platform_data = &nbx02_l3g4200dh_pdata,
	},
#endif
};

static int nbx02_tc35892_reset(void)
{
    int rc;
	tegra_gpio_enable(TEGRA_GPIO_PBB1);

    rc = gpio_request(TEGRA_GPIO_PBB1, "camera_reset");
    if (rc){
        printk("gpio camera_reset request error\n");
        return -1;
    }
    rc = gpio_direction_output(TEGRA_GPIO_PBB1, 1);
    if (rc){
        printk("gpio camera_reset set error\n");
        return -1;
    }
	rc = gpio_get_value(TEGRA_GPIO_PBB1);
	if(rc == 0){
        printk(KERN_ALERT "@@@ 20 tc35892 reset failed rc=%d\n", rc);
		return -ENODEV;
	}
    gpio_free(TEGRA_GPIO_PBB1);

    return 0;
}

static int tc35892_vcm_power_on(void)
{
    int ret = 0;
    u32 odm_rsvd[8];
    unsigned int vcm_flag;
    int CAM_VCM_POWER_BIT = 27;

    ret = tegra_fuse_read(ODM_RSVD, odm_rsvd, sizeof(odm_rsvd));
    if(ret!=0)return 0;
    vcm_flag = (odm_rsvd[7] >> CAM_VCM_POWER_BIT) & 0x01;
    printk(">>>>>>>>>>vcm_flag = %d\n",vcm_flag);
/*
 * vcm_flag =0 ,vcm power is PMIC LDO8,
 * vcm_flag =1 ,vcm power is external LDO that is using GPIO.
 */
    if(vcm_flag == 1){
        tegra_gpio_enable(GPIO_PIN_5M_28V_VCM);
        gpio_request(GPIO_PIN_5M_28V_VCM, "gpio_cam_vcm");
        gpio_direction_output(GPIO_PIN_5M_28V_VCM, 1);
        gpio_free(GPIO_PIN_5M_28V_VCM);
    }else{
        if (!reg_cam_vcm) {
            reg_cam_vcm = regulator_get(NULL, "cam_vcm");
            if (IS_ERR_OR_NULL(reg_cam_vcm)) {
                pr_err("tc35892_vcm_power_on: cam_vcm failed\n");
                reg_cam_vcm = NULL;
                return PTR_ERR(reg_cam_vcm);
            }

            ret = regulator_set_voltage(reg_cam_vcm,2850000,2850000);//shiyy
            if (ret){
                pr_err("%s: Failed to set cam_vcm to 2.85v\n", __func__);
                goto fail_put_regulator;
            }

            ret = regulator_enable(reg_cam_vcm);
            if (ret){
                pr_err("%s: Failed to enable cam_vcm\n", __func__);
                goto fail_put_regulator;
            }

        }
    }
    return 0;

fail_put_regulator:
	regulator_put(reg_cam_vcm);
	return ret;
}

static int tc35892_vcm_power_off(void)
{
    int ret = 0;
    u32 odm_rsvd[8];
    unsigned int vcm_flag;
    int CAM_VCM_POWER_BIT = 27;

    ret = tegra_fuse_read(ODM_RSVD, odm_rsvd, sizeof(odm_rsvd));
    if(ret!=0)return 0;
    vcm_flag = (odm_rsvd[7] >> CAM_VCM_POWER_BIT) & 0x01;
    printk(">>>>>>>>>>vcm_flag = %d\n",vcm_flag);
/*
 * vcm_flag =0 ,vcm power is PMIC LDO8,
 * vcm_flag =1 ,vcm power is external LDO that is using GPIO.
 */
    if(vcm_flag == 1){
        tegra_gpio_enable(GPIO_PIN_5M_28V_VCM);
        gpio_request(GPIO_PIN_5M_28V_VCM, "gpio_cam_vcm");
        gpio_direction_output(GPIO_PIN_5M_28V_VCM, 0);
        gpio_free(GPIO_PIN_5M_28V_VCM);
    }else{
        if (reg_cam_vcm) {
            regulator_disable(reg_cam_vcm);
            regulator_put(reg_cam_vcm);
            reg_cam_vcm = NULL;
        }
    }
    return 0;
}

struct tc35892_platform_data nbx02_tc35892_data = {
    .reset = nbx02_tc35892_reset,
    .vcm_power_on = tc35892_vcm_power_on,
    .vcm_power_off = tc35892_vcm_power_off,
};

static struct i2c_board_info nbx02_ioexpander_board_info[] = {
    {
        I2C_BOARD_INFO("tc35892", 0x42),
        .platform_data = &nbx02_tc35892_data,
    }
};

static struct i2c_board_info nbx02_camera_board_info[] = {
	{
		I2C_BOARD_INFO("ius006", 0x3D),
	},
	{
		I2C_BOARD_INFO("cmv59dx", 0x3C),
	}
};

int __init nbx02_sensors_init(void)
{
	struct board_info BoardInfo;

	nbx02_camera_init();
	nbx02_nct1008_init();

	tegra_get_board_info(&BoardInfo);

	i2c_register_board_info(3, nbx02_i2c4_board_info,
		ARRAY_SIZE(nbx02_i2c4_board_info));

#if defined(CONFIG_INPUT_LIS331DLH) || defined(CONFIG_INPUT_AMI304) || defined(CONFIG_INPUT_L3G4200DH)
	i2c_register_board_info(0, nbx02_i2c_sensors_info,
		ARRAY_SIZE(nbx02_i2c_sensors_info));
#endif

	i2c_register_board_info(0, nbx02_ioexpander_board_info,
			ARRAY_SIZE(nbx02_ioexpander_board_info));

	i2c_register_board_info(2, nbx02_camera_board_info,
			ARRAY_SIZE(nbx02_camera_board_info));


	return 0;
}
