# SPDX-License-Identifier: GPL-2.0

subdir-y += fsl

dtb-$(CONFIG_OF_ALL_DTBS) := $(patsubst $(src)/%.dts,%.dtb, $(wildcard $(src)/*.dts))

dtb-$(CONFIG_MPC831x_BOARDS) += \
	kmeter1.dtb \
	mpc8313erdb.dtb \
	mpc8315erdb.dtb
