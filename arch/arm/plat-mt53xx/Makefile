#
# Makefile for the linux kernel.
#

# Common support
obj-y :=
obj-m :=
obj-n :=
obj-  :=

# mt53xx device support
obj-$(CONFIG_ARCH_MT5396) += timer.o
obj-$(CONFIG_ARCH_MT5368) += timer.o
obj-$(CONFIG_ARCH_MT5389) += timer.o
obj-$(CONFIG_SMP)	+= platsmp.o headsmp.o
obj-$(CONFIG_MT53XX_UP_CHECK)	+= platsmp.o headsmp.o
obj-$(CONFIG_LOCAL_TIMERS)	+= localtimer.o
obj-$(CONFIG_PCI) += mtk_pci.o mtk_pciproc.o
obj-$(CONFIG_PCI_MSI) += mtk_pcimsi.o

