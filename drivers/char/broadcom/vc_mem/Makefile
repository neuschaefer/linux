EXTRA_CFLAGS  += -Wall -Wstrict-prototypes -Wno-trigraphs
EXTRA_CFLAGS  += -Werror
EXTRA_CFLAGS  += -I"include/linux/broadcom"
EXTRA_CFLAGS  += -I"drivers/misc/vc04_services"
EXTRA_CFLAGS  += -D__VCCOREVER__=0

obj-$(CONFIG_BCM_VC_MEM) := vc-mem.o

vc-mem-objs := vc_mem.o vc_loopaudio.o
