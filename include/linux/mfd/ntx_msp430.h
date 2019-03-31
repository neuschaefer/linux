/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2019 Jonathan Neuschäfer
 *
 * MFD access functions for the Netronix MSP430-based embedded controller.
 */

#ifndef NTX_MSP430_H
#define NTX_MSP430_H

#include <linux/types.h>

struct ntx_msp430 {
	struct device *dev;
	struct i2c_client *client;
	u16 version;
	/* TODO: a mutex? */
};

static inline bool is_new_layout(struct ntx_msp430 *mfd)
{
	return mfd->version == 0xe916;
}

int ntx_msp430_read16(struct ntx_msp430 *mfd, u8 addr);
int ntx_msp430_write16(struct ntx_msp430 *mfd, u8 addr, u16 value);
int ntx_msp430_read8(struct ntx_msp430 *mfd, u8 addr);
int ntx_msp430_write8(struct ntx_msp430 *mfd, u8 addr, u8 value);

#endif
