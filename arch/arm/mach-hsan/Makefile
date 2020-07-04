#
# Makefile for the linux kernel.
#

KBUILD_AFLAGS	:=$(KBUILD_AFLAGS:-msoft-float=-Wa,-mfpu=softvfp+vfp)

LDFLAGS  		+=--no-warn-mismatch

obj-y := hi_mach.o 
obj-y += hi_serial.o
obj-m += hi_serial_uart1.o
ifeq ($(CONFIG_SMP),y)
obj-y += hi_platsmp.o hi_headsmp.o
obj-y += hi_hotplug.o
endif
obj-y += hi_cache.o hi_acp.o 
ifeq ($(CONFIG_PCI),y)
obj-y += hi_pcie.o
endif
obj-y += hi_wdg.o
obj-y += hi_common.o
obj-y += timer.o

