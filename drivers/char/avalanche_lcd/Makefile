# File: drivers/char/avalanche_lcd/Makefile
#
# Makefile for the Linux lcd driver.
#

ifeq ("$(PATCHLEVEL)", "4")
EXTRA_CFLAGS := -I$(srctree)/include/asm/avalanche/generic
O_TARGET 	 := avalanche_lcd.o
list-multi   := avalanche_lcd.o
else
EXTRA_CFLAGS := -I$(srctree)/include/asm/mach-avalanche/generic
endif

obj-$(CONFIG_MIPS_AVALANCHE_LCD) += avalanche_lcd.o

avalanche_lcd-objs := lcd_drv.o  lidd_hal.o  hd44780_hal.o

ifeq ("$(PATCHLEVEL)", "4")
include $(srctree)/Rules.make

avalanche_lcd.o:      $(avalanche_lcd-objs)
	$(LD) -r -o $@ $(avalanche_lcd-objs)

clean:
	rm -f core *.o *.a *.s
endif
