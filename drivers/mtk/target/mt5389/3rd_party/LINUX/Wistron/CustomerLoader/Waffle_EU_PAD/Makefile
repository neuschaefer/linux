

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

-include $(PROJECT_ROOT)/path.mak

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT =
endif

DRV_ROOT := $(shell if [ -e $(DRV_ROOT)/driver.mak ]; then echo "$(DRV_ROOT)"; else echo "$(PROJECT_ROOT)/target/$(TARGET_IC)"; fi)

THIS_COMPONENT = PdwncCtrl

SRC = PdwncCtrl.c

OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS = 

CC_INC += -I$(DRV_ROOT)/3rd_party/LINUX/$(COMPANY)/d_inc \
          -I$(DRV_ROOT)/d_inc/$(COMPANY) \
          -I$(DRV_ROOT)/$(TARGET_DRIVER)/drv_cust/$(COMPANY) \
          -I$(DRV_ROOT)/$(TARGET_DRIVER)/drv_cust

#############################################################################
#
# Include the makefile common to all components
# 
include $(DRV_ROOT)/driver.mak
