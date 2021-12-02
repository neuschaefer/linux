#
# Makefile for the linux kernel.
#
AFLAGS_swsusp.o                 := -DTEXT_OFFSET=$(TEXT_OFFSET)

# Common support
obj-y := core.o
obj-m :=
obj-n :=
obj-  :=

obj-$(CONFIG_HOTPLUG_CPU)       += hotplug.o

# Power Management
obj-$(CONFIG_PM)                += pm.o sleep.o
obj-$(CONFIG_HIBERNATION)       += cpu.o swsusp.o

ifeq "${ANDROID}" "true"
    EXTRA_CFLAGS += -DANDROID
endif
