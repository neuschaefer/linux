dtb-$(CONFIG_ARCH_VEXPRESS) += $(subst ",,$(CONFIG_MSTAR_CHIP_NAME)).dtb

targets += dtbs
targets += $(dtb-y)

dtbs: $(addprefix $(obj)/, $(dtb-y))

clean-files := *.dtb
