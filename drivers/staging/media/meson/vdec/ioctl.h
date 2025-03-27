// SPDX-License-Identifier: GPL-2.0-or-later
// experimental ioctl interface for meson-vdec. not for production use.
#pragma once

#include <linux/types.h>

#define VDEC_MAGIC 'v'

// parameter struct for register transfer operations
struct vdec_rw {
	// bus number
	u16 bus;

	// register offset
	u16 reg;

	// register value
	u32 value;
};

#define VDEC_BUS_DOS	0
#define VDEC_BUS_AO	1

// start/stop the VDEC hardware
#define VDEC_START	_IO(VDEC_MAGIC, 0x40)
#define VDEC_STOP	_IO(VDEC_MAGIC, 0x41)

// read/write a register
#define VDEC_READ	_IOWR(VDEC_MAGIC, 0x42, struct vdec_rw)
#define VDEC_WRITE	_IOW(VDEC_MAGIC, 0x43, struct vdec_rw)


// load a program into IMEM
struct vdec_code {
	// pointer to code
	void __user *code;

	// size in bytes
	size_t size;
};
#define VDEC_LOAD_PROG	_IOW(VDEC_MAGIC, 0x44, struct vdec_code)
