# The lines below are only required because we are
# building a uImage.  As soon as we switch over to
# just building a zImage and store the suggested load
# address in the header of the Android boot.img we
# can delete these.  The kernel shouldn't care where
# it is loaded so long as it is 32kB into 128MB.

zreladdr-y      := $(CONFIG_BCM_ZRELADDR)
