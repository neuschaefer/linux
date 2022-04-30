# File: drivers/net/arm/Makefile
#
# Makefile for the ARM network device drivers
#

obj-$(CONFIG_ARM_AM79C961A)	+= am79c961a.o
obj-$(CONFIG_ARM_ETHERH)	+= etherh.o
obj-$(CONFIG_ARM_ETHER3)	+= ether3.o
obj-$(CONFIG_ARM_ETHER1)	+= ether1.o
obj-$(CONFIG_ARM_AT91_ETHER)	+= at91_ether.o
obj-$(CONFIG_EP93XX_ETH)	+= ep93xx_eth.o

spr_870_Eth-objs      := spr_870_eth_syn.o spr_870_eth_hal.o rtl8363_asicdrv.o spr_870_eth_phy.o
spr_870E_Eth-objs     := spr_870E_eth_syn.o spr_870E_eth_hal.o spr_870E_eth_phy.o spr_870E_ethtool.o


obj-$(CONFIG_870_ETH_SPEAR_DRIVER) += spr_870_Eth.o
obj-$(CONFIG_870E_ETH_SPEAR_DRIVER)+= spr_870E_Eth.o

switchctl-objs := switchctl_main.o switchctl_open.o switchctl_ioctl.o 
obj-$(CONFIG_870_ETH_SPEAR_DRIVER)  += switchctl.o
