/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * arch/arm/mach-tegra/board-nbx02.h
 *
 * Copyright (C) 2010 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _MACH_TEGRA_BOARD_NBX02_H
#define _MACH_TEGRA_BOARD_NBX02_H

int nbx02_regulator_init(void);
int nbx02_sdhci_init(void);
int nbx02_pinmux_init(void);
int nbx02_panel_init(void);
int nbx02_sensors_init(void);
int nbx02_kbc_init(void);
int nbx02_emc_init(void);
int nbx02_wired_jack_init(void);

/* external gpios */

/* TPS6586X gpios */
#define TPS6586X_GPIO_BASE	TEGRA_NR_GPIOS
// NR=4
#define AVDD_DSI_CSI_ENB_GPIO	TPS6586X_GPIO_BASE + 1 /* gpio2 */

///* TCA6416 gpios */
//#define TCA6416_GPIO_BASE	TEGRA_NR_GPIOS + 4
//#define CAM2_PWR_DN_GPIO	TCA6416_GPIO_BASE + 4 /* gpio4 */
//#define CAM2_RST_L_GPIO		TCA6416_GPIO_BASE + 5 /* gpio5 */
//#define CAM2_AF_PWR_DN_L_GPIO	TCA6416_GPIO_BASE + 6 /* gpio6 */
//#define CAM2_LDO_SHUTDN_L_GPIO	TCA6416_GPIO_BASE + 7 /* gpio7 */
//#define CAM2_I2C_MUX_RST_GPIO	TCA6416_GPIO_BASE + 15 /* gpio15 */

/* WM8903 GPIOs */
#define NBX02_GPIO_WM8903(_x_)	(TEGRA_NR_GPIOS + 4/**/ + (_x_))
// NR=5
#define NBX02_GPIO_WM8903_END		NBX02_GPIO_WM8903(4)

/* Audio-related GPIOs */
#define TEGRA_GPIO_CDC_IRQ		TEGRA_GPIO_PK6
#define TEGRA_GPIO_CDC_SHRT		TEGRA_GPIO_PU5
#define TEGRA_GPIO_SPKR_EN		NBX02_GPIO_WM8903(2)
#define TEGRA_GPIO_HP_DET		TEGRA_GPIO_PW2
//#define TEGRA_GPIO_INT_MIC_EN		TEGRA_GPIO_PX0
//#define TEGRA_GPIO_EXT_MIC_EN		TEGRA_GPIO_PX1

/* Interrupt numbers from external peripherals */
#define TPS6586X_INT_BASE       TEGRA_NR_IRQS
#define TPS6586X_INT_END        (TPS6586X_INT_BASE + 32)

#endif
