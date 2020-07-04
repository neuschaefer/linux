#export PKG_NAME = service_soft_qos

# the final application name
# export APP_NAME = hi_ksoft_qos

# main module
obj-m+=hi_ksoft_qos.o

# add kvif source file at here
hi_ksoft_qos-objs += hi_kernel_soft_qos.o 

# add the special GCC compile flags
# such as local macros, debug marcos and so on
# HI_CFLAGS += -I$(HI_HISI_INC_DIR)           -I$(HI_HISI_COMM_DIR) 
# HI_CFLAGS += -I$(HI_HISI_K_SPACE_DIR)       -I$(HI_HISI_U_SPACE_DIR)
# HI_CFLAGS += -I$(HI_HISI_BASIC_K_SPACE_DIR) -I$(HI_HISI_BASIC_U_SPACE_DIR)
# HI_CFLAGS += -I$(HI_HISI_SDK_K_SPACE_DIR)   -I$(HI_HISI_SDK_U_SPACE_DIR)

# add the special link
# such as local include path, local libs and so on
HI_LDFLAGS	+=

# include template
include $(HI_SCRIPT_KO_PATH)
