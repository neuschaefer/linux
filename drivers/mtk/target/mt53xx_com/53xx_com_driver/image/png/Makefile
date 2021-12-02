THIS_COMPONENT = png

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../..
endif

SRC = \
    png_com_hdr.c \
    png_hal_dec.c \
    png_drv.c \
    png_drv_decode.c \
    png_verify.c \
    
OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

DEFINES +=

CC_INC += 


#############################################################################
#
# Include the makefile common to all components


include $(DRV_ROOT)/driver.mak
