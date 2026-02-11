EXTRA_CFLAGS := -Werror

ifeq ($(srctree),)

#
# We weren't called from the linux make environment, redirect
#

all:
	$(MAKE) -C ../../../../drivers/char/broadcom/halaudio_drivers/bcm_capri/

.PHONY: all

else

obj-$(CONFIG_BCM_HALAUDIO_AUDIOH_CAPRI) += audioh/
obj-$(CONFIG_BCM_HALAUDIO_PCM_CAPRI) += pcm/

endif
