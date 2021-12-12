# Related path to CD root
RelRoot=${PWD}/../../..

# Global environments
ifndef CDRoot
CDRoot=${RelRoot}
export CT_PATH=${CDRoot}/crosstool
export CT_BIN_PATH=${CT_PATH}/arm-softfloat-linux-gnu/bin
export PLATFORM=softfloat
export ARCH=arm
export TARGET=${ARCH}-${PLATFORM}-linux-gnu
export CROSS_COMPILE=${TARGET}-
PATH:=${CT_BIN_PATH}:${PATH}
endif

Module=linux-2.6.23.1
kernel=${CT_PATH}_addon/${Module}


obj-m += aess_dynairqdrv.o

all: aess_dynairqdrv.ko

aess_dynairqdrv.ko: aess_dynairqdrv.c aess_dynairqdrv.h
	make -C $(kernel) SUBDIRS=`pwd` CROSS_COMPILE=${CROSS_COMPILE} modules

clean:
	rm -f *.ko *.o *.mod.c

cleanall: clean
	rm -rf .*.cmd *.bak *.symvers .tmp_versions
	
