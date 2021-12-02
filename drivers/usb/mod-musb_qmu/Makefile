##
#	@author xibin 
#

FPGA=n
QMU=y

musb_hdrc-objs := src/musb_core.o src/mt8560.o 
EXTRA_CFLAGS+=-I./drivers/usb/mod-musb_qmu/include/
obj-$(CONFIG_USB_MTK_MUSB) += musb_hdrc.o
##
#       @brief object files of this module
#
ifeq ($(CONFIG_USB_MTK_MUSB_PERIPHERAL),y)
musb_hdrc-objs+=src/musb_gadget_ep0.o src/musb_gadget.o
EXTRA_CFLAGS+=-DCONFIG_USB_GADGET_MUSB_HDRC
EXTRA_CFLAGS+=-DCONFIG_USB_MUSB_PERIPHERAL
endif

ifeq ($(CONFIG_USB_MTK_MUSB_HOST),m)
musb_hdrc-objs+=src/musb_virthub.o src/musb_host.o 
ifeq ($(CONFIG_USB_MTK_MUSB_QMU),m)
musb_hdrc-objs+=src/musb_qmu.o
EXTRA_CFLAGS+=-DCONFIG_USB_QMU_SUPPORT 
endif
EXTRA_CFLAGS+=-DCONFIG_USB_MUSB_HDRC_HCD
EXTRA_CFLAGS+=-DCONFIG_MTK_HOST
#EXTRA_CFLAGS+=-DCONFIG_MT8560_HOST
EXTRA_CFLAGS+=-DCONFIG_MTK_DTV_HOST
endif

ifeq ($(CONFIG_USB_MTK_MUSB_DMA),y)
musb_hdrc-objs+=src/musbhsdma.o
EXTRA_CFLAGS+=-DCONFIG_USB_INVENTRA_DMA
else
EXTRA_CFLAGS+=-DCONFIG_MUSB_PIO_ONLY
endif

ifeq ($(strip $(FPGA)),y)
EXTRA_CONFIG+=-DCONFIG_FPGA
endif

EXTRA_CFLAGS+=-DCONFIG_MT8560

ifeq ($(USB_ADB), true)
  EXTRA_CFLAGS		+= -DUSB_DEVICE_MODE_SUPPORT
endif



