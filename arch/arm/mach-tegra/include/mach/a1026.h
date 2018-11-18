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
#ifndef _A1026_H_
#define _A1026_H_

#define VERSION_NBX0200		0x02
#define VERSION_NBX0300		0x03
#define VERSION_PASSTHROUGH		0x04
#define VERSION_UNKNOWN		0xff

#define A1026_PASS_THRU              (0)
#define A1026_VOICE_PROCESSING_ON    (1)

struct a1026_platform_data {
	struct i2c_client *client;
	int reset_pin;
	int wake_pin;
	struct clk *clock;
	char *clk;
        int fw_version;
        int is_awake;
};

int a1026_suspend_command(void);
int a1026_resume_command(int);

#endif /* _A1026_H_ */
