/*****************************************************************************
* Copyright 2005 - 2009 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL"). 
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#ifndef _OTP_H
#define _OTP_H

#include <linux/ioctl.h>
#ifdef __KERNEL__
#include <linux/types.h>
#include <chal/chal_otp.h>
#else
#include <stdint.h>
#endif


#define OTP_MAGIC_1    'o'

struct otp_data {
	/* enable/disable ECC for data reading */
	unsigned int enable_ecc;

	/* row to read/write */
	unsigned int row;

	/* store the 32-bit data */
	uint32_t data;
};

/* ioctl() command numbers */
#define OTP_CMD_READ_ROW    0x80
#define OTP_CMD_WRITE_ROW   0x81
#define OTP_CMD_FAIL_ROW    0x82
#define OTP_CMD_READ_FAIL   0x83

#define BCM_OTP_DEV_NAME "otp"

/*
 * Read the OTP data from a specific row. Result is stored in the 'data' field
 */
#define OTP_IOCTL_READ_ROW   _IOWR(OTP_MAGIC_1, OTP_CMD_READ_ROW, struct otp_data)

/*
 * Write the OTP data to a specific row.
 */
#define OTP_IOCTL_WRITE_ROW  _IOWR(OTP_MAGIC_1, OTP_CMD_WRITE_ROW, struct otp_data)

/*
 * Write the fail bits of a specific OTP row.
 */
#define OTP_IOCTL_FAIL_ROW  _IOWR(OTP_MAGIC_1, OTP_CMD_FAIL_ROW, struct otp_data)

/*
 * Read the fail bits of a specific OTP row.
 */
#define OTP_IOCTL_READ_FAIL  _IOWR(OTP_MAGIC_1, OTP_CMD_READ_FAIL, struct otp_data)

#endif  /* _OTP_H */
