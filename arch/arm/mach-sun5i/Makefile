obj-y += clock/
obj-y += dma/
obj-y += dram/
obj-y += core.o  devices.o sys_config.o gpio_eint.o
obj-$(CONFIG_PM) += pm/
ifdef CONFIG_CPU_FREQ_DVFS_HW
obj-$(CONFIG_CPU_FREQ) += cpu-freq-pmu/
else
obj-$(CONFIG_CPU_FREQ) += cpu-freq/
endif
