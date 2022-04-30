#
# USB peripheral controller drivers
#
obj-$(CONFIG_USB_DUMMY_HCD)	+= dummy_hcd.o
obj-$(CONFIG_USB_NET2280)	+= net2280.o
obj-$(CONFIG_USB_PXA2XX)	+= pxa2xx_udc.o
obj-$(CONFIG_USB_GOKU)		+= goku_udc.o
obj-$(CONFIG_USB_OMAP)		+= omap_udc.o
obj-$(CONFIG_USB_LH7A40X)	+= lh7a40x_udc.o
obj-$(CONFIG_USB_SPEAR_SYN)    	+= spr_udc_syn.o
obj-$(CONFIG_USB_PXA27X)	+= pxa27x_udc.o

#
# USB gadget drivers
#
obj-y	+=   usbstring.o config.o epautoconf.o

g_spr_mux-objs		:= spr_mux.o 
g_spr_printer-objs   	:= spr_printer.o 
g_spr_fax-objs        := spr_fax.o
g_spr_scanner-objs    := spr_scanner.o
g_zero-objs				:= zero.o
g_ether-objs			:= ether.o 
g_serial-objs			:= serial.o 
gadgetfs-objs			:= inode.o
g_file_storage-objs		:= file_storage.o 

ifeq ($(CONFIG_USB_ETH_RNDIS),y)
	g_ether-objs		+= rndis.o
endif
 
obj-$(CONFIG_USB_G_SPR_MUX)		+= g_spr_mux.o
obj-$(CONFIG_USB_G_SPR_PRINTER)		+= g_spr_printer.o
obj-$(CONFIG_USB_G_SPR_FAX)       += g_spr_fax.o
obj-$(CONFIG_USB_G_SPR_SCANNER)   += g_spr_scanner.o
obj-$(CONFIG_USB_ZERO)				+= g_zero.o
obj-$(CONFIG_USB_ETH)				+= g_ether.o
obj-$(CONFIG_USB_GADGETFS)			+= gadgetfs.o
obj-$(CONFIG_USB_FILE_STORAGE)		+= g_file_storage.o
obj-$(CONFIG_USB_G_SERIAL)			+= g_serial.o

