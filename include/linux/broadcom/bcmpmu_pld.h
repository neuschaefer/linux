/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

/*******************************************************************************************
Copyright 2010 Broadcom Corporation.  All rights reserved.
f
Unless you and Broadcom execute a separate written software license agreement
governing use of this software, this software is licensed to you under the
terms of the GNU General Public License version 2, available at
http://www.gnu.org/copyleft/gpl.html (the "GPL").

Notwithstanding the above, under no circumstances may you combine this software
in any way with any other Broadcom software provided under a license other than
the GPL, without Broadcom's express prior written consent.
*******************************************************************************************/


#ifndef __BCMPMU_PLD_H__
#define __BCMPMU_PLD_H__
/* PMU_REG_PLD_PWMLEDCTRL1 */
#define PWMLED_HWSW_CTRL_BIT_MASK   0x1
#define PWMLED_HWSW_CTRL_BIT_SHIFT  0x0
#define PWMLED_SW_CTRL              (0x0 << PWMLED_HWSW_CTRL_BIT_SHIFT)
#define PWMLED_HW_CTRL              (0x1 << PWMLED_HWSW_CTRL_BIT_SHIFT)

#define PWMLED_PWDN_BIT_MASK    0x2
#define PWMLED_PWDN_BIT_SHIFT   0x1
#define PWMLED_PON              (0x0 << PWMLED_PWDN_BIT_SHIFT)
#define PWMLED_PDWN             (0x1 << PWMLED_PWDN_BIT_SHIFT)

/* PMU_REG_PLD_PWMLEDCTRL2 */
#define PWMLED_IOUT_PULSE_ON_1_0_BIT_MASK       0x03
#define PWMLED_IOUT_PULSE_ON_1_0_SHIFT_MASK     0x00
#define PWMLED_IOUT_PULSE_ON_1_0_SET(x)         ((x<<PWMLED_IOUT_PULSE_ON_1_0_SHIFT_MASK)&PWMLED_IOUT_PULSE_ON_1_0_BIT_MASK)

#define PWMLED_IOUT_PULSE_OFF_1_0_BIT_MASK      0x0c
#define PWMLED_IOUT_PULSE_OFF_1_0_SHIFT_MASK    0x02
#define PWMLED_IOUT_PULSE_OFF_1_0_SET(x)        ((x<<PWMLED_IOUT_PULSE_OFF_1_0_SHIFT_MASK)&PWMLED_IOUT_PULSE_OFF_1_0_BIT_MASK)

typedef enum {
	IOUT_PULSE_0mA = 0x00,
	IOUT_PULSE_1mA = 0x01,
	IOUT_PULSE_2_5mA = 0x02,
	IOUT_PULSE_5mA = 0x03,
} pwmled_iout_pulse_t;

#define PWMLEDCTRL2_FULL_MASK	(PWMLED_IOUT_PULSE_OFF_1_0_BIT_MASK|PWMLED_IOUT_PULSE_ON_1_0_BIT_MASK)

/* PMU_REG_PLD_PWMLEDCTRL3 */
#define PWMLED_T_PULSE_ON_2_0_BIT_MASK      0x07
#define PWMLED_T_PULSE_ON_2_0_BIT_SHIFT     0x00
#define PWMLED_T_PULSE_ON_2_0_SET(x)        ((x << PWMLED_T_PULSE_ON_2_0_BIT_SHIFT)&PWMLED_T_PULSE_ON_2_0_BIT_MASK)

typedef enum {
	T_PULSE_ON_100ms = 0x00,
	T_PULSE_ON_200ms = 0x01,
	T_PULSE_ON_500ms = 0x02,
	T_PULSE_ON_1s = 0x03,
	T_PULSE_ON_2s = 0x04,
	T_PULSE_ON_ALWAYS = 0x05,
} pwmled_t_pulse_on_t;

#define PWMLED_T_PULSE_OFF_2_0_BIT_MASK     0x38
#define PWMLED_T_PULSE_OFF_2_0_BIT_SHIFT    0x03
#define PWMLED_T_PULSE_OFF_2_0_SET(x)       ((x << PWMLED_T_PULSE_OFF_2_0_BIT_SHIFT)&PWMLED_T_PULSE_OFF_2_0_BIT_MASK)

typedef enum {
	T_PULSE_OFF_500ms = 0x00,
	T_PULSE_OFF_1s = 0x01,
	T_PULSE_OFF_2s = 0x02,
	T_PULSE_OFF_5s = 0x03,
	T_PULSE_OFF_10s = 0x04,
	T_PULSE_OFF_20s = 0x05,
} pwmled_t_pulse_off_t;

#define T_PULSE_DEFAULT  0x07

#define PWMLED_PULSE_NUM_1_0_BIT_MASK       0xc0
#define PWMLED_PULSE_NUM_1_0_BIT_SHIFT      0x06
#define PWMLED_PULSE_NUM_1_0_SET(x)         ((x << PWMLED_PULSE_NUM_1_0_BIT_SHIFT)&PWMLED_PULSE_NUM_1_0_BIT_MASK)

typedef enum {
	PULSE_NUM_1_0_1time = 0x00,
	PULSE_NUM_1_0_2time = 0x01,
	PULSE_NUM_1_0_3time = 0x02,
} pwmled_t_pulse_num_t;

#define PWMLEDCTRL3_FULL_MASK (PWMLED_T_PULSE_ON_2_0_BIT_MASK|PWMLED_T_PULSE_OFF_2_0_BIT_MASK|PWMLED_PULSE_NUM_1_0_BIT_MASK)

/* PMU_REG_PLD_PWMLEDCTRL4 */
#define PWMLED_DC_PULSE_ON_6_0_BIT_MASK     0x7F
#define PWMLED_DC_PULSE_ON_6_0_MAX          0x7F
#define PWMLED_DC_PULSE_ON_6_0_MIN          0x00
#define PWMLED_DC_PULSE_ON_6_0_SET(x)       (x & PWMLED_DC_PULSE_ON_6_0_BIT_MASK)

/* PMU_REG_PLD_PWMLEDCTRL5 */
#define PWMLED_DC_PULSE_OFF_6_0_BIT_MASK    0x7F
#define PWMLED_DC_PULSE_OFF_6_0_MAX         0x7F
#define PWMLED_DC_PULSE_OFF_6_0_MIN         0x00
#define PWMLED_DC_PULSE_OFF_6_0_SET(x)      (x & PWMLED_DC_PULSE_OFF_6_0_BIT_MASK)

/* PMU_REG_PLD_PWMLEDCTRL6 */
#define PWMLED_DC_CONSTANT_BRIGHTNESS_6_0_BIT_MASK      0x7F
#define PWMLED_DC_CONSTANT_BRIGHTNESS_6_0_MAX           0x7F
#define PWMLED_DC_CONSTANT_BRIGHTNESS_6_0_MIN           0x00
#define PWMLED_DC_CONSTANT_BRIGHTNESS_6_0_SET(x)        (x & PWMLED_DC_CONSTANT_BRIGHTNESS_6_0_BIT_MASK)

#endif
