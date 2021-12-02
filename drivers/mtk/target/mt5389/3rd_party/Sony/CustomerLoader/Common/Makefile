

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

include $(PROJECT_ROOT)/path.mak


THIS_COMPONENT = PdwncCtrl

SRC = PdwncCtrl.c

OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS = 

CC_INC += -I$(DRV_ROOT)/$(DRIVER_DIR)/drv_cust/$(3RD_PARTIES) \
          -I$(TARGET_ROOT)/d_inc/$(3RD_PARTIES) \
          -I$(TARGET_ROOT)/3rd_party/$(3RD_PARTIES)/d_inc

#############################################################################
#
# Include the makefile common to all components
# 
include $(TARGET_ROOT)/common.mak
