#
# USB peripheral controller drivers
#
ifeq ($(CONFIG_USB_GADGET_DEBUG),y)
	EXTRA_CFLAGS		+= -DDEBUG
endif

g_ci13410-fs-objs                       := ci13410_fs.o
g_ci13410-hs-objs                       := ci13410.o

obj-$(CONFIG_USB_CI13410_FS)    += g_ci13410-fs.o
obj-$(CONFIG_USB_CI13410_HS)    += g_ci13410-hs.o
obj-$(CONFIG_USB_CI13410_FS_AND_CI13410_HS) += g_ci13410-fs.o
obj-$(CONFIG_USB_CI13410_FS_AND_CI13410_HS) += g_ci13410-hs.o
obj-$(CONFIG_USB_DUMMY_HCD)	+= dummy_hcd.o
obj-$(CONFIG_USB_NET2280)	+= net2280.o
obj-$(CONFIG_USB_AMD5536UDC)	+= amd5536udc.o
obj-$(CONFIG_USB_PXA2XX)	+= pxa2xx_udc.o
obj-$(CONFIG_USB_GOKU)		+= goku_udc.o
obj-$(CONFIG_USB_OMAP)		+= omap_udc.o
obj-$(CONFIG_USB_LH7A40X)	+= lh7a40x_udc.o
obj-$(CONFIG_USB_S3C2410)	+= s3c2410_udc.o
obj-$(CONFIG_USB_AT91)		+= at91_udc.o
obj-$(CONFIG_USB_FSL_USB2)	+= fsl_usb2_udc.o
obj-$(CONFIG_USB_M66592)	+= m66592-udc.o

#
# USB gadget drivers
#
g_composite-objs                := composite.o usbstring.o config.o epautoconf.o
g_mass_storage-objs             := mass_storage.o usbstring.o config.o \
                                        epautoconf.o
g_kbdmouse-objs                 := kbdmouse.o usbstring.o config.o epautoconf.o
g_zero-objs			:= zero.o usbstring.o config.o epautoconf.o
g_ether-objs			:= ether.o usbstring.o config.o epautoconf.o
g_serial-objs			:= serial.o usbstring.o config.o epautoconf.o
g_midi-objs			:= gmidi.o usbstring.o config.o epautoconf.o
gadgetfs-objs			:= inode.o
g_file_storage-objs		:= file_storage.o usbstring.o config.o \
					epautoconf.o

ifeq ($(CONFIG_USB_ETH_RNDIS),y)
	g_ether-objs		+= rndis.o
endif

obj-$(CONFIG_USB_COMPOSITE)     += g_composite.o
obj-$(CONFIG_USB_MASS_STORAGE)  += g_mass_storage.o
obj-$(CONFIG_USB_KBDMOUSE)      += g_kbdmouse.o 
obj-$(CONFIG_USB_ZERO)		+= g_zero.o
obj-$(CONFIG_USB_ETH)		+= g_ether.o
obj-$(CONFIG_USB_GADGETFS)	+= gadgetfs.o
obj-$(CONFIG_USB_FILE_STORAGE)	+= g_file_storage.o
obj-$(CONFIG_USB_G_SERIAL)	+= g_serial.o
obj-$(CONFIG_USB_MIDI_GADGET)	+= g_midi.o

