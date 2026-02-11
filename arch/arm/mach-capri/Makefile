#*****************************************************************************
# Copyright 2011-2012 Broadcom Corporation.  All rights reserved.
#
# Unless you and Broadcom execute a separate written software license
# agreement governing use of this software, this software is licensed to you
# under the terms of the GNU General Public License version 2, available at
# http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
#
# Notwithstanding the above, under no circumstances may you combine this
# software in any way with any other Broadcom software provided under a
# license other than the GPL, without Broadcom's express prior written
# consent.
#*****************************************************************************

obj-y := mach.o common.o io_map.o ipc_chip.o pinmux_table.o

obj-$(CONFIG_KONA_AVS) += capri_avs.o
obj-$(CONFIG_KONA_POWER_MGR) += capri_pwr_mgr.o
obj-$(CONFIG_KONA_PI_MGR) += capri_pi_mgr.o
obj-$(CONFIG_HAVE_CLK) += clock_capri.o
obj-$(CONFIG_MAP_SDMA) += chal/
obj-$(CONFIG_MAP_SDMA) += sdmac.o dma_mmap_test.o
obj-$(CONFIG_MAP_DMA_MMAP) += dma_mmap.o dump-mem.o
obj-$(CONFIG_MAP_DMA_TEST) += dma-test.o
obj-$(CONFIG_LITTLE_MEMC_QOS) += memc_qos.o
obj-$(CONFIG_IPC) += chal/
obj-$(CONFIG_MACH_CAPRI_CUSTOM_BOARDS_SUPPORT) += custom_boards/
obj-$(CONFIG_MFD_BCMPMU) += board-bcm59056.o
obj-$(CONFIG_CDEBUGGER) += crash_debugger.o

obj-$(CONFIG_NET_ISLAND) += ethHw_driver.o
ethHw_driver-objs := ethHw.o ethHw_asm.o
obj-$(CONFIG_MAP_AADMA) += chal/
obj-$(CONFIG_MAP_AADMA) += aadmac.o
obj-$(CONFIG_MAP_SSASW) += chal/
obj-$(CONFIG_MAP_SSASW) += ssasw.o

obj-$(CONFIG_KONA_SECURE_MONITOR_CALL) += sec_api.o

obj-$(CONFIG_MACH_CAPRI_BASE) += board_base.o
CFLAGS_board_base.o += -Iarch/arm/mach-capri/hw_cfg/BASE

obj-$(CONFIG_MACH_CAPRI_FPGA) += board_fpga.o pinmux_fpga.o
CFLAGS_board_fpga.o += -Iarch/arm/mach-capri/hw_cfg/CAPRI_FPGA \
-Iarch/arm/mach-capri/hw_cfg/BASE \
-Idrivers/videocore/host_support/include \
-Idrivers/videocore/interface/vceb/host \
-Idrivers/videocore/interface/vceb/host/linux

obj-$(CONFIG_MACH_CAPRI_STONE) += board_stone.o pinmux_stone.o board-capri-wifi.o
CFLAGS_board_stone.o += -Iarch/arm/mach-capri/hw_cfg/CAPRI_STONE \
-Iarch/arm/mach-capri/hw_cfg/BASE \
-Idrivers/videocore/host_support/include \
-Idrivers/videocore/interface/vceb/host \
-Idrivers/videocore/interface/vceb/host/linux

obj-$(CONFIG_MACH_CAPRI_SS) += board_ss.o pinmux_ss.o board-capri-wifi.o
CFLAGS_board_ss.o += -Iarch/arm/mach-capri/hw_cfg/CAPRI_SS \
-Iarch/arm/mach-capri/hw_cfg/BASE \
-Idrivers/videocore/host_support/include \
-Idrivers/videocore/interface/vceb/host \
-Idrivers/videocore/interface/vceb/host/linux

obj-$(CONFIG_MACH_CAPRI_M500) += board_m500.o pinmux_stone.o board-capri-wifi.o
CFLAGS_board_m500.o += -Iarch/arm/mach-capri/hw_cfg/CAPRI_M500 \
-Iarch/arm/mach-capri/hw_cfg/BASE \
-Idrivers/videocore/host_support/include \
-Idrivers/videocore/interface/vceb/host \
-Idrivers/videocore/interface/vceb/host/linux

obj-$(CONFIG_MACH_CAPRI_RAY) += board_ray.o pinmux_ray.o board-capri-wifi.o
CFLAGS_board_ray.o += -Iarch/arm/mach-capri/hw_cfg/CAPRI_RAY \
-Iarch/arm/mach-capri/hw_cfg/BASE \
-Idrivers/videocore/host_support/include \
-Idrivers/videocore/interface/vceb/host \
-Idrivers/videocore/interface/vceb/host/linux
obj-$(CONFIG_KONA_CPU_PM_HANDLER) += capri_pm.o capri_wfi.o capri_pm_dbg.o bmodem_sleep.o
obj-$(CONFIG_KONA_PROFILER) += capri_profiler.o

obj-$(CONFIG_CAPRI_DORMANT_MODE) += capri_dormant.o v7.o v7_c.o a9.o helpers.o \
gic.o a9_c.o capri_dormant_prepare.o

obj-$(CONFIG_MACH_CAPRI_RAY_NAND) += board_ray_nand.o
CFLAGS_board_ray_nand.o += -Iarch/arm/mach-capri/hw_cfg/CAPRI_RAY_NAND \

obj-$(CONFIG_CAPRI_PM_TEST) += capri_pm_test.o
obj-$(CONFIG_MACH_CAPRI_TABLET) += board_tablet.o pinmux_tablet.o board-capri-wifi.o
CFLAGS_board_tablet.o += -Iarch/arm/mach-capri/hw_cfg/CAPRI_TABLET \
-Iarch/arm/mach-capri/hw_cfg/BASE \
-Idrivers/videocore/host_support/include \
-Idrivers/videocore/interface/vceb/host \
-Idrivers/videocore/interface/vceb/host/linux

ifneq ($(call as-instr,.arch_extension sec,+sec),)
CFLAGS_sec_api.o += -DREQUIRES_SEC
CFLAGS_os_api.o += -DREQUIRES_SEC
CFLAGS_capri_dormant.o += -DREQUIRES_SEC
AFLAGS_v7.o :=-Wa,-march=armv7-a+sec
endif

