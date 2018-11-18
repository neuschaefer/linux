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
#ifndef _HCI_EVENT_H_
#define _HCI_EVENT_H_

#include <linux/switch.h>

struct hci_event_platform_data {
#define TEGRA_HCI_STATE_OC	0
#define TEGRA_HCI_STATE_OH	1
#define TEGRA_HCI_STATE_NS	2
#define TEGRA_HCI_STATE_NUM	3

#define TEGRA_HCI_STATE_OC_MASK	(1 << TEGRA_HCI_STATE_OC)
#define TEGRA_HCI_STATE_OH_MASK	(1 << TEGRA_HCI_STATE_OH)
#define TEGRA_HCI_STATE_NS_MASK	(1 << TEGRA_HCI_STATE_NS)

	int hci_state;
	struct switch_dev sdev;
	int ocdet_pin;
	int pwren_pin;
	struct work_struct work;
};

#endif /* _HCI_EVENT_H_ */
