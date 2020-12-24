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
export PATH:=${CT_BIN_PATH}:${PATH}
endif

Module=linux-2.6.23.1
kernel=${CT_PATH}_addon/${Module}
MemUtility = MemTest


obj-m += aess_memdrv.o

all: aess_memdrv.ko 

aess_memdrv.ko: aess_memdrv.c aess_memdrv.h ${MemUtility}.c ${MemUtility}
	make -C $(kernel) SUBDIRS=`pwd` CROSS_COMPILE=${CROSS_COMPILE} modules
	${CROSS_COMPILE}gcc -Wall -o ${MemUtility} ${MemUtility}.c 

clean:
	rm -f *.ko *.o *.mod.c ${MemUtility}

cleanall: clean
	rm -rf .*.cmd *.bak *.symvers .tmp_versions ${MemUtility}
	
