#
# Makefile for USB Host Controller Drivers
#

ifeq ($(USB_ADB), true)
EXTRA_CFLAGS		+= -DUSB_DEVICE_MODE_SUPPORT
endif

ifeq ($(CONFIG_USB_DEBUG),y)
EXTRA_CFLAGS		+= -DDEBUG -O0
endif

isp1760-objs := isp1760-hcd.o isp1760-if.o
fhci-objs := fhci-hcd.o fhci-hub.o fhci-q.o fhci-mem.o \
	     fhci-tds.o fhci-sched.o
ifeq ($(CONFIG_FHCI_DEBUG),y)
fhci-objs += fhci-dbg.o
endif
xhci-hcd-objs := xhci.o xhci-mem.o xhci-pci.o xhci-ring.o xhci-hub.o xhci-dbg.o

obj-$(CONFIG_USB_WHCI_HCD)	+= whci/

obj-$(CONFIG_PCI)		+= pci-quirks.o

EXTRA_CFLAGS += -DCONFIG_USB_QMU_SUPPORT
mtk-hcd-objs := mtk_hcd.o mtk_qmu.o mtk_cust.o

obj-$(CONFIG_USB_EHCI_HCD)	+= ehci-hcd.o
obj-$(CONFIG_USB_OXU210HP_HCD)	+= oxu210hp-hcd.o
obj-$(CONFIG_USB_ISP116X_HCD)	+= isp116x-hcd.o
obj-$(CONFIG_USB_ISP1362_HCD)	+= isp1362-hcd.o
obj-$(CONFIG_USB_OHCI_HCD)	+= ohci-hcd.o
obj-$(CONFIG_USB_UHCI_HCD)	+= uhci-hcd.o
obj-$(CONFIG_USB_FHCI_HCD)	+= fhci.o
obj-$(CONFIG_USB_XHCI_HCD)	+= xhci-hcd.o
obj-$(CONFIG_USB_SL811_HCD)	+= sl811-hcd.o
obj-$(CONFIG_USB_SL811_CS)	+= sl811_cs.o
obj-$(CONFIG_USB_U132_HCD)	+= u132-hcd.o
obj-$(CONFIG_USB_R8A66597_HCD)	+= r8a66597-hcd.o
obj-$(CONFIG_USB_ISP1760_HCD)	+= isp1760.o
obj-$(CONFIG_USB_HWA_HCD)	+= hwa-hc.o
obj-$(CONFIG_USB_IMX21_HCD)	+= imx21-hcd.o
obj-$(CONFIG_USB_MTK53xx_HCD)	+= mtk-hcd.o
obj-$(CONFIG_USB_MTK5395_HCD)	+= mtk_hcd_95.o
