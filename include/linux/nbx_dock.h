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
#ifndef _NBX_DOCK_H_
#define _NBX_DOCK_H_

#include <linux/switch.h>

struct nbx_dock_platform_data {
	int ac_state;
	struct switch_dev sdev;
	int dockdet_pin;
	struct delayed_work work;
};

#endif /* _NBX_DOCK_H_ */
