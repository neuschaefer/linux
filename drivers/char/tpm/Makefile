#
# Makefile for the kernel tpm device drivers.
#
obj-$(CONFIG_TCG_TPM) += tpm.o
ifdef CONFIG_ACPI
	obj-$(CONFIG_TCG_TPM) += tpm_bios.o
endif
obj-$(CONFIG_TCG_TIS) += tpm_tis.o
obj-$(CONFIG_TCG_NSC) += tpm_nsc.o
obj-$(CONFIG_TCG_ATMEL) += tpm_atmel.o
obj-$(CONFIG_TCG_INFINEON) += tpm_infineon.o
obj-$(CONFIG_TCG_TIS_GPIOSIMU_I2C_ST33) += tpm_gpio_i2c_stm_st33.o hw_gpio_i2c.o