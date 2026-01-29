// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) J. Neuschäfer <j.ne@posteo.net>

#include <linux/i2c.h>
#include <linux/mfd/core.h>

struct wiiext_id {
	u16 field[3];
};

#define WIIEXT_ID(a, b, c) (struct wiiext_id) { .field = { a, b, c } }

static inline bool wiiext_id_eq(const struct wiiext_id *a, const struct wiiext_id *b)
{
	return memcmp(a, b, sizeof(struct wiiext_id)) == 0;
}

struct wiiext {
	struct i2c_client *client;

	struct wiiext_id id;
	bool present;

	unsigned int presence_poll_interval; /* msec */
	struct delayed_work presence_work;

	unsigned int input_poll_interval; /* msec */

	struct mfd_cell subdev;
	char subdev_name[24];

	/* ensures atomicity of transfers */
	struct mutex mutex;
};

int wiiext_read(struct wiiext *wiiext, u8 reg, u8 *data, size_t length);
int wiiext_write(struct wiiext *wiiext, const u8 *data, size_t length);
int wiiext_write_one(struct wiiext *wiiext, u8 reg, u8 value);
