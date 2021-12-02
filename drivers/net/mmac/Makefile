#
# Makefile for linux/drivers/net/mmac
#


#EXTRA_CFLAGS = -D__KERNEL__ -DMODULE -O -Wall -I$(INCLUDEDIR)
#CFLAGS += -O0

ifeq ($(CONFIG_PHY_SMSC8700),y)
CFLAGS += -DMT539X_LAN8700
endif

ifeq ($(CONFIG_MAC539X_DEBUG_LOG),y)
CFLAGS += -DDEBUG_ETH_ON
endif

obj-$(CONFIG_MAC539X) += mmac_driver.o
obj-$(CONFIG_MAC5363) += mmac_driver.o

mmac_driver-objs := mmac.o \
		mmac_dev.o 

#obj-$(CONFIG_MAC539X) += mmac.o
#obj-$(CONFIG_MAC539X) += mmac_dev.o
		