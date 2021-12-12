# Related path to CD root
RelRoot = ${PWD}/../../..

# Global environments
ifndef CDRoot
CDRoot = ${RelRoot}
export CT_PATH=${CDRoot}/crosstool
export CT_BIN_PATH=${CT_PATH}/arm-softfloat-linux-gnu/bin
export PLATFORM=softfloat
export ARCH=arm
export TARGET=${ARCH}-${PLATFORM}-linux-gnu
export CROSS_COMPILE=${TARGET}-
export TARGET_PREFIX=${CT_PATH}/${TARGET}/${TARGET}
PATH:=${CT_BIN_PATH}:${PATH}
endif

Module=linux-2.6.23.1
kernel=${CT_PATH}_addon/${Module}


obj-m += aess_gpiodrv.o

all: aess_gpiodrv.ko

aess_gpiodrv.ko: aess_gpiodrv.c aess_gpiodrv.h
	make -C $(kernel) SUBDIRS=`pwd` CROSS_COMPILE=${CROSS_COMPILE} modules

clean:
	rm -f *.ko *.o *.mod.c

cleanall: clean
	rm -rf .*.cmd *.bak *.symvers .tmp_versions
	
