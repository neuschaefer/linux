EXTRA_CFLAGS  += -Wall -Wstrict-prototypes -Wno-trigraphs -O2

EXTRA_CFLAGS  += -I"include/linux/broadcom"
EXTRA_CFLAGS  += -I"drivers/misc/vc04_services"
EXTRA_CFLAGS  += -I"drivers/misc/vc04_services/interface/vchi"
EXTRA_CFLAGS  += -I"drivers/misc/vc04_services/interface/vchiq_arm"

EXTRA_CFLAGS  += -DOS_ASSERT_FAILURE
EXTRA_CFLAGS  += -D__STDC_VERSION=199901L
EXTRA_CFLAGS  += -D__STDC_VERSION__=199901L
EXTRA_CFLAGS  += -D__VCCOREVER__=0
EXTRA_CFLAGS  += -D__KERNEL__
EXTRA_CFLAGS  += -D__linux__
EXTRA_CFLAGS  += -Werror
obj-$(CONFIG_BCM_VC_SND) = vc-alsa.o
vc-alsa-objs             := vmcs_alsa.o vc_vchi_alsa.o vmcs_alsa_ctl.o vmcs_alsa_pcm.o
