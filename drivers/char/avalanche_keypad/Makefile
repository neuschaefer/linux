# File: drivers/char/avalanche_keypad/Makefile
#
# Makefile for the Linux keypad driver.
#

ifeq ("$(PATCHLEVEL)", "4")
EXTRA_CFLAGS := -I$(srctree)/include/asm/avalanche/generic
O_TARGET 	 := avalanche_keypad.o
list-multi   := avalanche_keypad.o
else
EXTRA_CFLAGS := -I$(srctree)/include/asm/mach-avalanche/generic
endif

obj-$(CONFIG_MIPS_AVALANCHE_KEYPAD) += avalanche_keypad.o

avalanche_keypad-objs := keypad_drv.o keypad_hal.o

ifeq ("$(PATCHLEVEL)", "4")
include $(srctree)/Rules.make
avalanche_keypad.o:      $(avalanche_keypad-objs)
	$(LD) -r -o $@ $(avalanche_keypad-objs)

clean:
	rm -f core *.o *.a *.s
endif
	
