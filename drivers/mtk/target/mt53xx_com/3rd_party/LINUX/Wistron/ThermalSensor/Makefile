

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

-include $(PROJECT_ROOT)/path.mak

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT =
endif

THIS_COMPONENT = ThermalSensor

SRC = ThermalSensor.c

OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS = 

CC_INC += -I$(DRV_ROOT)/3rd_party/LINUX/$(COMPANY)/d_inc \
          -I$(DRV_ROOT)/d_inc/$(COMPANY) \
          -I$(DRV_ROOT)/d_inc

#############################################################################
#
# Include the makefile common to all components
# 
include $(DRV_ROOT)/driver.mak
