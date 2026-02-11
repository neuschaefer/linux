/*****************************************************************************
*  Copyright 2011 - 2012 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/
#include <linux/version.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <asm/gpio.h>
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/i2c.h>
#include <linux/mfd/bcmpmu.h>

#define PMU_DEVICE_I2C_ADDR	0x08
#define PMU_DEVICE_I2C_ADDR1	0x0C
#define PMU_INT_IRQ_NUM		BCM_INT_ID_PMU_INT
#define PMU_DEVICE_I2C_BUSNO	2

static struct bcmpmu_rw_data register_init_data[] = {
	{.map = 0, .addr = 0x14, .val = 0x75, .mask = 0xFF},
	/* set PC3_EN */
	{.map = 0, .addr = 0x15, .val = 0x02, .mask = 0x02},
	{.map = 0, .addr = 0xC9, .val = 0x30, .mask = 0xFF},
	{.map = 0, .addr = 0x30, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x31, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x32, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x33, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x34, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x35, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x36, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x37, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x38, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x39, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x3A, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x3B, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x3C, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x3D, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x3E, .val = 0xFF, .mask = 0xFF},
	{.map = 0, .addr = 0x3F, .val = 0xFF, .mask = 0xFF},

	{.map = 0, .addr = 0x40, .val = 0x38, .mask = 0x38},
	{.map = 0, .addr = 0x41, .val = 0x00, .mask = 0x04},
	{.map = 0, .addr = 0x42, .val = 0x04, .mask = 0x04},
	{.map = 0, .addr = 0x45, .val = 0x0C, .mask = 0xFF},
	{.map = 0, .addr = 0x47, .val = 0x09, .mask = 0xFF},
	{.map = 1, .addr = 0xCC, .val = 0x43, .mask = 0xFF},
	{.map = 1, .addr = 0xCD, .val = 0x7F, .mask = 0xFF},
	{.map = 1, .addr = 0xCE, .val = 0x3B, .mask = 0xFF},
	{.map = 1, .addr = 0xCF, .val = 0xF8, .mask = 0xFF},
	{.map = 1, .addr = 0xD0, .val = 0x09, .mask = 0xFF},

	{.map = 1, .addr = 0x47, .val = 0xD2, .mask = 0xFF},
	{.map = 1, .addr = 0x48, .val = 0x98, .mask = 0xFF},
	{.map = 1, .addr = 0x49, .val = 0xF0, .mask = 0xFF},
	{.map = 1, .addr = 0x4A, .val = 0x58, .mask = 0xFF},
	{.map = 1, .addr = 0x4B, .val = 0xC3, .mask = 0xFF},
	/* SDSR-1 PC pin OpMode */
	{.map = 0, .addr = 0x82, .val = 0x01, .mask = 0x3F},
	{.map = 0, .addr = 0x83, .val = 0x00, .mask = 0x3F},
	{.map = 0, .addr = 0x84, .val = 0x01, .mask = 0x3F},
	{.map = 0, .addr = 0x85, .val = 0x00, .mask = 0x3F},
	/* SDSR-2 PC pin OpMode */
	{.map = 0, .addr = 0x86, .val = 0x01, .mask = 0x3F},
	{.map = 0, .addr = 0x87, .val = 0x00, .mask = 0x3F},
	{.map = 0, .addr = 0x88, .val = 0x01, .mask = 0x3F},
	{.map = 0, .addr = 0x89, .val = 0x00, .mask = 0x3F},
	{.map = 0, .addr = 0xD0, .val = 0x24, .mask = 0x3F},
	/* IOSR-1 PC pin OpMode */
	{.map = 0, .addr = 0x7A, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x7B, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0xC3, .val = 0x35, .mask = 0x3F},
	{.map = 0, .addr = 0xC4, .val = 0x35, .mask = 0x3F},
	/* IOSR-2 PC pin OpMode */
	{.map = 0, .addr = 0x7C, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x7D, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0xC6, .val = 0x35, .mask = 0x3F},
	{.map = 0, .addr = 0xC7, .val = 0x35, .mask = 0x3F},
	/* CSR PC pin OpMode */
	{.map = 0, .addr = 0x7E, .val = 0x01, .mask = 0x3F},
	{.map = 0, .addr = 0x7F, .val = 0x00, .mask = 0x3F},
	{.map = 0, .addr = 0x80, .val = 0x12, .mask = 0x3F},
	{.map = 0, .addr = 0x81, .val = 0x12, .mask = 0x3F},
	{.map = 0, .addr = 0xC1, .val = 0x00, .mask = 0x3F},
	 /* VSR PC pin OpMode */
	{.map = 0, .addr = 0x8E, .val = 0x02, .mask = 0x3F},
	{.map = 0, .addr = 0x8F, .val = 0x00, .mask = 0x3F},
	{.map = 0, .addr = 0x90, .val = 0x02, .mask = 0x3F},
	{.map = 0, .addr = 0x91, .val = 0x00, .mask = 0x3F},
	{.map = 0, .addr = 0xD3, .val = 0x00, .mask = 0x3F},
	/* MSR PC pin OpMode */
	{.map = 0, .addr = 0x8A, .val = 0x01, .mask = 0x3F},
	{.map = 0, .addr = 0x8B, .val = 0x00, .mask = 0x3F},
	{.map = 0, .addr = 0x8C, .val = 0x09, .mask = 0x3F},
	{.map = 0, .addr = 0x8D, .val = 0x01, .mask = 0x3F},
	{.map = 0, .addr = 0xCA, .val = 0x01, .mask = 0x3F},
	/* RFLDO PC pin OpMode */
	{.map = 0, .addr = 0x60, .val = 0x22, .mask = 0xFF},
	{.map = 0, .addr = 0x61, .val = 0x2a, .mask = 0xFF},
	{.map = 0, .addr = 0x96, .val = 0x9E, .mask = 0x38},
	/* AUDLDO PC pin OpMode */
	{.map = 0, .addr = 0x72, .val = 0x02, .mask = 0xFF},
	{.map = 0, .addr = 0x73, .val = 0x02, .mask = 0xFF},
	{.map = 0, .addr = 0x9F, .val = 0xAF, .mask = 0x38},
	/* CAMLDO1 PC pin OpMode */
	{.map = 0, .addr = 0x62, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x63, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x97, .val = 0xB6, .mask = 0x38},
	/* CAMLDO2 PC pin OpMode */
	{.map = 0, .addr = 0x64, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x65, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x98, .val = 0x9C, .mask = 0x38},
	/* USBLDO PC pin OpMode */
	{.map = 0, .addr = 0x76, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x77, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0xA1, .val = 0xB9, .mask = 0x38},
#if 0
	/*Temporary comment out until SIM detection implement
	* the proper handle through PMU regulator framework*/
	/* SIMLDO1 PC pin OpMode */
	{.map = 0, .addr = 0x66, .val = 0xAA, .mask = 0xFF},
	{.map = 0, .addr = 0x67, .val = 0xAA, .mask = 0xFF},
	{.map = 0, .addr = 0x99, .val = 0x88, .mask = 0x38},
	/* SIMLDO2 PC pin OpMode */
	{.map = 0, .addr = 0x68, .val = 0xAA, .mask = 0xFF},
	{.map = 0, .addr = 0x69, .val = 0xAA, .mask = 0xFF},
	{.map = 0, .addr = 0x9A, .val = 0xB0, .mask = 0x38},
#endif
	/* VIBLDO PC pin OpMode */
	{.map = 0, .addr = 0x78, .val = 0xAA, .mask = 0xFF},
	{.map = 0, .addr = 0x79, .val = 0xAA, .mask = 0xFF},
	{.map = 0, .addr = 0xA2, .val = 0xAF, .mask = 0x38},
	/* SDLDO PC pin OpMode */
	{.map = 0, .addr = 0x6A, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x6B, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x9B, .val = 0xB4, .mask = 0x38},
	/* MMCLDO1 PC pin OpMode */
	{.map = 0, .addr = 0x6E, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x6F, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x9D, .val = 0x89, .mask = 0x38},
	/* MMCLDO2 PC pin OpMode */
	{.map = 0, .addr = 0x70, .val = 0x05, .mask = 0xFF},
	{.map = 0, .addr = 0x71, .val = 0x15, .mask = 0xFF},
	{.map = 0, .addr = 0x9E, .val = 0xB1, .mask = 0x38},
	/* SDXLDO PC pin OpMode */
	{.map = 0, .addr = 0x6C, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x6D, .val = 0x01, .mask = 0xFF},
	{.map = 0, .addr = 0x9C, .val = 0x8D, .mask = 0x38},
	/* MICLDO PC pin OpMode */
	{.map = 0, .addr = 0x74, .val = 0xAA, .mask = 0xFF},
	{.map = 0, .addr = 0x75, .val = 0xAA, .mask = 0xFF},

	{.map = 1, .addr = 0xBF, .val = 0x64, .mask = 0xFF},

	/* PMU_RTC_RTC_C2C1_XOTRIM */
	{.map = 1, .addr = 0xEF, .val = 0x44, .mask = 0xFF},

};

static struct bcmpmu_temp_map batt_temp_map[] = {
/* This table is hardware dependent and need to get from platform team */
/*	adc		temp*/
	{932, -400},			/* -40 C */
	{900, -350},			/* -35 C */
	{860, -300},			/* -30 C */
	{816, -250},			/* -25 C */
	{760, -200},			/* -20 C */
	{704, -150},			/* -15 C */
	{636, -100},			/* -10 C */
	{568, -50},			/* -5 C */
	{500, 0},			/* 0 C */
	{440, 50},			/* 5 C */
	{376, 100},			/* 10 C */
	{324, 150},			/* 15 C */
	{272, 200},			/* 20 C */
	{228, 250},			/* 25 C */
	{192, 300},			/* 30 C */
	{160, 350},			/* 35 C */
	{132, 400},			/* 40 C */
	{112, 450},			/* 45 C */
	{92, 500},			/* 50 C */
	{76, 550},			/* 55 C */
	{64, 600},			/* 60 C */
	{52, 650},			/* 65 C */
	{44, 700},			/* 70 C */
	{36, 750},			/* 75 C */
	{32, 800},			/* 80 C */
	{28, 850},			/* 85 C */
	{24, 900},			/* 90 C */
	{20, 950},			/* 95 C */
	{16, 1000},			/* 100 C */
};

struct regulator_consumer_supply usb_supply[] = {
	{ .supply = "usbldo_uc"},
};
static struct regulator_init_data bcm59056_usbldo_data =  {
	.constraints = {
		.name = "usbldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_MODE |
			REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(usb_supply),
	.consumer_supplies = usb_supply,
};

struct regulator_consumer_supply rf_supply[] = {
	{ .supply = "rfldo_uc"},
};
static struct regulator_init_data bcm59056_rfldo_data =  {
	.constraints = {
		.name = "rfldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_MODE |
			REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(rf_supply),
	.consumer_supplies = rf_supply,
};

struct regulator_consumer_supply cam1_supply[] = {
	{.supply = "camldo1_uc"},
	REGULATOR_SUPPLY("vdd", "1-005c"), /* tango */
	REGULATOR_SUPPLY("vdd", "0-000e"), /* ami306 */
	REGULATOR_SUPPLY("vdd", "0-0068"), /* mpu6050 */
	REGULATOR_SUPPLY("vdd", "0-0077"), /* bmp18x */
	REGULATOR_SUPPLY("vdd", "0-001d"), /* al3006 */
};
static struct regulator_init_data bcm59056_camldo1_data = {
	.constraints = {
		.name = "camldo1",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 0,
		.state_standby = {
			.disabled = 0, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(cam1_supply),
	.consumer_supplies = cam1_supply,
};

struct regulator_consumer_supply cam2_supply[] = {
	{.supply = "camldo2_uc"},
};
static struct regulator_init_data bcm59056_camldo2_data = {
	.constraints = {
		.name = "camldo2",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 0,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(cam2_supply),
	.consumer_supplies = cam2_supply,
};


struct regulator_consumer_supply sd_supply[] = {
	{.supply = "sdldo_uc"},
	REGULATOR_SUPPLY("vmmc", "sdhci.3"), /* 0x3f1b0000.sdhci */
};
static struct regulator_init_data bcm59056_sdldo_data = {
	.constraints = {
		.name = "sdldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE |  REGULATOR_CHANGE_VOLTAGE,
		.always_on = 0,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(sd_supply),
	.consumer_supplies = sd_supply,
};

struct regulator_consumer_supply sdx_supply[] = {
	{.supply = "sdxldo_uc"},
	REGULATOR_SUPPLY("vddo", "sdhci.3"), /* 0x3f1b0000.sdhci */
};
static struct regulator_init_data bcm59056_sdxldo_data = {
	.constraints = {
		.name = "sdxldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 0,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(sdx_supply),
	.consumer_supplies = sdx_supply,
};

struct regulator_consumer_supply vib_supply[] = {
	{.supply = "vibldo_uc"},
};
static struct regulator_init_data bcm59056_vibldo_data = {
	.constraints = {
		.name = "vibldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 0,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(vib_supply),
	.consumer_supplies = vib_supply,
};

struct regulator_consumer_supply aud_supply[] = {
	{.supply = "audldo_uc"},
};
static struct regulator_init_data bcm59056_audldo_data = {
	.constraints = {
		.name = "audldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(aud_supply),
	.consumer_supplies = aud_supply,
};

struct regulator_consumer_supply mic_supply[] = {
	{.supply = "micldo_uc"},
};
static struct regulator_init_data bcm59056_micldo_data = {
	.constraints = {
		.name = "micldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 0,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(mic_supply),
	.consumer_supplies = mic_supply,
};

struct regulator_consumer_supply mmc1_supply[] = {
	{.supply = "mmcldo1_uc"},
};
static struct regulator_init_data bcm59056_mmcldo1_data = {
	.constraints = {
		.name = "mmcldo1",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(mmc1_supply),
	.consumer_supplies = mmc1_supply,
};

struct regulator_consumer_supply mmc2_supply[] = {
	{.supply = "mmcldo2_uc"},
};
static struct regulator_init_data bcm59056_mmcldo2_data = {
	.constraints = {
		.name = "mmcldo2",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(mmc2_supply),
	.consumer_supplies = mmc2_supply,
};

struct regulator_consumer_supply sim_supply[] = {
	{.supply = "sim_vcc"},
};
static struct regulator_init_data bcm59056_simldo_data = {
	.constraints = {
		.name = "simldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(sim_supply),
	.consumer_supplies = sim_supply,
};


struct regulator_consumer_supply sim2_supply[] = {
	{.supply = "sim2_vcc"},
};
static struct regulator_init_data bcm59056_sim2ldo_data = {
	.constraints = {
		.name = "sim2ldo",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(sim2_supply),
	.consumer_supplies = sim2_supply,
};

struct regulator_consumer_supply hdmi_supply[] = {
	{ .supply = "hdmi_uc"},
};
static struct regulator_init_data bcm59056_hdmildo_data =  {
	.constraints = {
		.name = "hdmildo",
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(hdmi_supply),
	.consumer_supplies = hdmi_supply,
};

struct regulator_consumer_supply csr_supply[] = {
	{.supply = "csr_uc"},
};
static struct regulator_init_data bcm59056_csr_data = {
	.constraints = {
		.name = "csr",
		.min_uV = 700000,
		.max_uV = 1350000,
		.valid_ops_mask = REGULATOR_CHANGE_MODE |
			REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(csr_supply),
	.consumer_supplies = csr_supply,
};

struct regulator_consumer_supply mmsr_supply[] = {
	{.supply = "mmsr_uc"},
};
static struct regulator_init_data bcm59056_mmsr_data = {
	.constraints = {
		.name = "mmsr",
		.min_uV = 700000,
		.max_uV = 1800000,
		.valid_ops_mask = REGULATOR_CHANGE_MODE |
			REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(mmsr_supply),
	.consumer_supplies = mmsr_supply,
};

struct regulator_consumer_supply vsr_supply[] = {
	{.supply = "vsr_uc"},
};
static struct regulator_init_data bcm59056_vsr_data = {
	.constraints = {
		.name = "vsr",
		.min_uV = 700000,
		.max_uV = 1800000,
		.valid_ops_mask = REGULATOR_CHANGE_MODE |
			REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(vsr_supply),
	.consumer_supplies = vsr_supply,
};

struct regulator_consumer_supply gp1_supply[] = {
	{.supply = "gpldo1_uc"},
};
static struct regulator_init_data bcm59056_gpldo1_data = {
	.constraints = {
		.name = "gpldo1",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(gp1_supply),
	.consumer_supplies = gp1_supply,
};

struct regulator_consumer_supply gp2_supply[] = {
	{.supply = "gpldo2_uc"},
};
static struct regulator_init_data bcm59056_gpldo2_data = {
	.constraints = {
		.name = "gpldo2",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(gp2_supply),
	.consumer_supplies = gp2_supply,
};

struct regulator_consumer_supply gp3_supply[] = {
	{.supply = "gpldo3_uc"},
};
static struct regulator_init_data bcm59056_gpldo3_data = {
	.constraints = {
		.name = "gpldo3",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(gp3_supply),
	.consumer_supplies = gp3_supply,
};

struct regulator_consumer_supply gp4_supply[] = {
	{.supply = "gpldo4_uc"},
};
static struct regulator_init_data bcm59056_gpldo4_data = {
	.constraints = {
		.name = "gpldo4",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(gp4_supply),
	.consumer_supplies = gp4_supply,
};

struct regulator_consumer_supply gp5_supply[] = {
	{.supply = "gpldo5_uc"},
};
static struct regulator_init_data bcm59056_gpldo5_data = {
	.constraints = {
		.name = "gpldo5",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(gp5_supply),
	.consumer_supplies = gp5_supply,
};

struct regulator_consumer_supply gp6_supply[] = {
	{.supply = "gpldo6_uc"},
};
static struct regulator_init_data bcm59056_gpldo6_data = {
	.constraints = {
		.name = "gpldo6",
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.state_standby = {
			.disabled = 1, /* 0 for LPM, 1 for OFF */
		},
	},
	.num_consumer_supplies = ARRAY_SIZE(gp6_supply),
	.consumer_supplies = gp6_supply,
};

struct bcmpmu_regulator_init_data bcm59056_regulators[BCMPMU_REGULATOR_MAX] = {
	[BCMPMU_REGULATOR_RFLDO] = {
		BCMPMU_REGULATOR_RFLDO, &bcm59056_rfldo_data, 0x22, 0
	},
	[BCMPMU_REGULATOR_CAMLDO] = {
		BCMPMU_REGULATOR_CAMLDO, &bcm59056_camldo1_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_CAMLDO2] = {
		BCMPMU_REGULATOR_CAMLDO2, &bcm59056_camldo2_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_SDLDO] = {
		BCMPMU_REGULATOR_SDLDO, &bcm59056_sdldo_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_SDXLDO] = {
		BCMPMU_REGULATOR_SDXLDO, &bcm59056_sdxldo_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_VIBLDO] = {
		BCMPMU_REGULATOR_VIBLDO, &bcm59056_vibldo_data, 0xAA, 0
	},
	[BCMPMU_REGULATOR_AUDLDO] = {
		BCMPMU_REGULATOR_AUDLDO, &bcm59056_audldo_data, 0x02, 0
	},
	[BCMPMU_REGULATOR_MICLDO] = {
		BCMPMU_REGULATOR_MICLDO, &bcm59056_micldo_data, 0xAA, 0
	},
	[BCMPMU_REGULATOR_MMCLDO1] = {
		BCMPMU_REGULATOR_MMCLDO1, &bcm59056_mmcldo1_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_MMCLDO2] = {
		BCMPMU_REGULATOR_MMCLDO2, &bcm59056_mmcldo2_data, 0x05, 0
	},
	/*not set SIMLDO OpMode and let regulator probe to disable */
	[BCMPMU_REGULATOR_SIMLDO] = {
		BCMPMU_REGULATOR_SIMLDO, &bcm59056_simldo_data, 0xff, 0
	},
	/*Not set SIMLDO2 OpMode and let regulator to disable*/
	[BCMPMU_REGULATOR_SIM2LDO] = {
		BCMPMU_REGULATOR_SIM2LDO, &bcm59056_sim2ldo_data, 0xff, 0
	},
	[BCMPMU_REGULATOR_HDMILDO] = {
		BCMPMU_REGULATOR_HDMILDO, &bcm59056_hdmildo_data, 0x00, 0
	},
	[BCMPMU_REGULATOR_USBLDO] = {
		BCMPMU_REGULATOR_USBLDO, &bcm59056_usbldo_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_CSR_NM] = {
		BCMPMU_REGULATOR_CSR_NM, &bcm59056_csr_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_MMSR_NM] = {
		BCMPMU_REGULATOR_MMSR_NM, &bcm59056_mmsr_data, 0x01, 0
	},
	[BCMPMU_REGULATOR_VSR_NM] = {
		BCMPMU_REGULATOR_VSR_NM, &bcm59056_vsr_data, 0x02, 0
	},
	[BCMPMU_REGULATOR_GPLDO1] = {
		BCMPMU_REGULATOR_GPLDO1, &bcm59056_gpldo1_data, 0x00, 0
	},
	[BCMPMU_REGULATOR_GPLDO2] = {
		BCMPMU_REGULATOR_GPLDO2, &bcm59056_gpldo2_data, 0x00, 0
	},
	[BCMPMU_REGULATOR_GPLDO3] = {
		BCMPMU_REGULATOR_GPLDO3, &bcm59056_gpldo3_data, 0x00, 0
	},
	[BCMPMU_REGULATOR_GPLDO4] = {
		BCMPMU_REGULATOR_GPLDO4, &bcm59056_gpldo4_data, 0x00, 0
	},
	[BCMPMU_REGULATOR_GPLDO5] = {
		BCMPMU_REGULATOR_GPLDO5, &bcm59056_gpldo5_data, 0x00, 0
	},
	[BCMPMU_REGULATOR_GPLDO6] = {
		BCMPMU_REGULATOR_GPLDO6, &bcm59056_gpldo6_data, 0x00, 0
	},
};

static struct platform_device bcmpmu_audio_device = {
	.name			= "bcmpmu_audio",
	.id			= -1,
	.dev.platform_data	= NULL,
};

static struct platform_device bcmpmu_em_device = {
	.name			= "bcmpmu_em",
	.id			= -1,
	.dev.platform_data	= NULL,
};

static struct platform_device bcmpmu_otg_xceiv_device = {
	.name			= "bcmpmu_otg_xceiv",
	.id			= -1,
	.dev.platform_data	= NULL,
};

#ifdef CONFIG_BCMPMU_RPC
static struct platform_device bcmpmu_rpc = {
	.name = "bcmpmu_rpc",
	.id = -1,
	.dev.platform_data = NULL,
};
#endif

#ifdef CONFIG_CHARGER_BCMPMU_SPA
static struct platform_device bcmpmu_chrgr_spa_device = {
	.name = "bcmpmu_chrgr_pb",
	.id = -1,
	.dev.platform_data = NULL,
};
#endif

static struct platform_device *bcmpmu_client_devices[] = {
	&bcmpmu_audio_device,
#ifdef CONFIG_CHARGER_BCMPMU_SPA
	&bcmpmu_chrgr_spa_device,
#endif
	&bcmpmu_em_device,
	&bcmpmu_otg_xceiv_device,
#ifdef CONFIG_BCMPMU_RPC
	&bcmpmu_rpc,
#endif
};

static int __init bcmpmu_init_platform_hw(struct bcmpmu *bcmpmu)
{
	int i;
	printk(KERN_INFO "%s: called.\n", __func__);

	for (i = 0; i < ARRAY_SIZE(bcmpmu_client_devices); i++)
		bcmpmu_client_devices[i]->dev.platform_data = bcmpmu;
	platform_add_devices(bcmpmu_client_devices,
			ARRAY_SIZE(bcmpmu_client_devices));

	return 0;
}

static int __init bcmpmu_exit_platform_hw(struct bcmpmu *bcmpmu)
{
	printk(KERN_INFO "%s called\n", __func__);

	return 0;
}

static struct i2c_board_info pmu_info_map1 = {
	I2C_BOARD_INFO("bcmpmu_map1", PMU_DEVICE_I2C_ADDR1),
};

static struct bcmpmu_adc_setting adc_setting = {
	.tx_rx_sel_addr = 0,
	.tx_delay = 0,
	.rx_delay = 0,
};

static struct bcmpmu_charge_zone chrg_zone[] = {
	{.tl = -50, .th = 600, .v = 3000, .fc = 10, .qc = 100},	/* Zone QC */
	{.tl = -50, .th = -1, .v = 4200, .fc = 100, .qc = 0},	/* Zone LL */
	{.tl = 0, .th = 99, .v = 4200, .fc = 100, .qc = 0},	/* Zone L */
	{.tl = 100, .th = 450, .v = 4200, .fc = 100, .qc = 0},	/* Zone N */
	{.tl = 451, .th = 500, .v = 4200, .fc = 100, .qc = 0},	/* Zone H */
	{.tl = 501, .th = 600, .v = 4200, .fc = 100, .qc = 0},	/* Zone HH */
	{.tl = -50, .th = 600, .v = 0, .fc = 0, .qc = 0},	/* Zone OUT */
};

static struct bcmpmu_voltcap_map batt_voltcap_map[] = {
	/*
	* volt capacity
	*/
	{4168, 100},
	{4137, 95},
	{4091, 90},
	{4051, 85},
	{4003, 80},
	{3966, 75},
	{3934, 70},
	{3900, 65},
	{3862, 60},
	{3830, 55},
	{3808, 50},
	{3790, 45},
	{3775, 40},
	{3765, 35},
	{3756, 30},
	{3744, 25},
	{3719, 20},
	{3679, 15},
	{3664, 10},
	{3650, 8},
	{3613, 6},
	{3566, 4},
	{3500, 2},
	{3416, 0},
};

static struct bcmpmu_fg_zone fg_zone[FG_TMP_ZONE_MAX+1] = {
/* This table is default data, the real data from board file or device tree*/
	{.temp = -200,
	 .reset = 0, .fct = 520, .guardband = 100,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* -20 */
	{.temp = -150,
	 .reset = 0, .fct = 620, .guardband = 100,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* -15 */
	{.temp = -100,
	 .reset = 0, .fct = 720, .guardband = 100,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* -10 */
	{.temp = -50,
	 .reset = 0, .fct = 782, .guardband = 100,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* -5 */
	{.temp = 0,
	 .reset = 0, .fct = 845, .guardband = 100,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* 0 */
	{.temp = 50,
	 .reset = 0, .fct = 880, .guardband = 100,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* 5 */
	{.temp = 100,
	 .reset = 0, .fct = 915, .guardband = 30,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* 10 */
	{.temp = 150,
	 .reset = 0, .fct = 957, .guardband = 30,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* 15 */
	{.temp = 200,
	 .reset = 0, .fct = 1000, .guardband = 30,
	 .esr_vl_lvl = 3600, .esr_vm_lvl = 3800, .esr_vh_lvl = 4000,
	 .esr_vl = 140, .esr_vl_slope = 100, .esr_vl_offset = 0,
	 .esr_vm = 140, .esr_vm_slope = 100, .esr_vm_offset = 0,
	 .esr_vh = 140, .esr_vh_slope = 100, .esr_vh_offset = 0,
	 .esr_vf = 140, .esr_vf_slope = 100, .esr_vf_offset = 0,
	 .vcmap = &batt_voltcap_map[0],
	 .maplen = ARRAY_SIZE(batt_voltcap_map)},/* 20 */
};

#ifdef CONFIG_CHARGER_BCMPMU_EXT
static struct bcmpmu_ext_chrgr_info ext_chrgr_info = {
	.ac5v = 56,
	.iusb = 94,
	.cen = 48,
	.usus = 93,
	.uok = 55,
	.flt = 92,
	.chg = 91,
};
#endif

#ifdef CONFIG_CHARGER_BCMPMU_SPA
static void notify_spa(enum bcmpmu_event_t event, int data)
{
	printk(KERN_INFO "%s event=%d, data=%d\n",
		__func__, event, data);

	switch (event) {
	case BCMPMU_CHRGR_EVENT_CHGR_DETECTION:
		/* notify SPA driver
		spa_event_handler(SPA_EVT_CHARGER, data)
		*/
		break;
	case BCMPMU_CHRGR_EVENT_MBTEMP:
		/* notify SPA driver
		spa_event_handler(SPA_EVT_TEMP, data);
		*/
		break;
	case BCMPMU_CHRGR_EVENT_MBOV:
		/* notify SPA driver
		spa_event_handler(SPA_EVT_OVP, data);
		*/
		break;
	case BCMPMU_CHRGR_EVENT_USBOV:
		/* notify SPA driver
		spa_event_handler(SPA_EVT_OVP, data);
		*/
		break;
	case BCMPMU_CHRGR_EVENT_EOC:
		/* notify SPA driver
		spa_event_handler(SPA_EVT_EOC, 0);
		*/
		break;
	default:
		break;
	}
}
#endif

static struct bcmpmu_platform_data bcmpmu_plat_data = {
#ifdef CONFIG_KONA_PMU_BSC_HS_MODE
	/*
	* PMU in High Speed (HS) mode. I2C CLK is 3.25MHz
	* derived from 26MHz input clock.
	*
	* Rhea: PMBSC is always in HS mode, i2c_pdata is not in use.
	*/
	.i2c_pdata  = ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_HS),
#else
	.i2c_pdata  = ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_400K),
#endif
	.init = bcmpmu_init_platform_hw,
	.exit = bcmpmu_exit_platform_hw,
	.i2c_board_info_map1 = &pmu_info_map1,
	.i2c_adapter_id = PMU_DEVICE_I2C_BUSNO,
	.i2c_pagesize = 256,
	.init_data = &register_init_data[0],
	.init_max = ARRAY_SIZE(register_init_data),
	.batt_temp_in_celsius = 1,
	.batt_temp_map = &batt_temp_map[0],
	.batt_temp_map_len = ARRAY_SIZE(batt_temp_map),
	.adc_setting = &adc_setting,
	.num_of_regl = ARRAY_SIZE(bcm59056_regulators),
	.regulator_init_data = &bcm59056_regulators[0],
	.fg_smpl_rate = 2083,
	.fg_slp_rate = 32000,
	.fg_slp_curr_ua = 1000,
	.fg_factor = 976,
	.fg_sns_res = 10,
	.batt_voltcap_map = &batt_voltcap_map[0],
	.batt_voltcap_map_len = ARRAY_SIZE(batt_voltcap_map),
	.chrg_zone_map = &chrg_zone[0],
#ifdef CONFIG_USB_SWITCH_FSA9485
	.support_hw_eoc = 1,
#else
	.support_hw_eoc = 0,
#endif
	.support_fg = 1,
	.support_chrg_maint = 1,
	.chrg_resume_lvl = 4100,
#ifdef CONFIG_CHARGER_BCMPMU_EXT
	.fg_support_tc = 0,
#else
	.fg_support_tc = 1,
#endif
	.fg_tc_dn_lvl = 50, /* 5c */
	.fg_tc_up_lvl = 200, /* 20c */
	.fg_zone_settle_tm = 60,
	.fg_zone_info = &fg_zone[0],
#ifdef CONFIG_USB_SWITCH_FSA9485
	.bc = BCMPMU_BC_FSA,
#else
	.bc = BCMPMU_BC_BB_BC12,
#endif
	.batt_model = "SS,1650mAH",
	.cutoff_volt = 3300,
	.cutoff_count_max = 3,
#ifdef CONFIG_CHARGER_BCMPMU_EXT
	.batt_impedence = 200,
	.chrg_1c_rate = 3350,
	.chrg_eoc = 168,
	.fg_capacity_full = 6700*3600,
	.support_ext_chrgr = 1,
	.ext_chrgr_info = &ext_chrgr_info,
#else
	.batt_impedence = 238,
	.chrg_1c_rate = 1500,
	.chrg_eoc = 75,
	.fg_capacity_full = 1500*3600,
#endif
	.irq = PMU_INT_IRQ_NUM,
#ifdef CONFIG_CHARGER_BCMPMU_SPA
	.piggyback_chrg = 1,
	.piggyback_chrg_name = "bcm59056_charger",
	.piggyback_notify = notify_spa,
#endif
};

static struct i2c_board_info __initdata pmu_info[] = {
	{
		I2C_BOARD_INFO("bcmpmu", PMU_DEVICE_I2C_ADDR),
		.platform_data  = &bcmpmu_plat_data,
	},
};


__init int board_pmu_init(void)
{

#ifdef CONFIG_KONA_DT_BCMPMU
	bcmpmu_update_pdata_dt_batt(&bcmpmu_plat_data);
	bcmpmu_update_pdata_dt_pmu(&bcmpmu_plat_data);
#endif

	i2c_register_board_info(PMU_DEVICE_I2C_BUSNO,
				pmu_info,
				ARRAY_SIZE(pmu_info));
	return 0;
}

arch_initcall(board_pmu_init);
