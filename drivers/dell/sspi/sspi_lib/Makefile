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


all:Lib_SSPIDrv.o


Lib_SSPIDrv.o: Lib_SSPIDrv.c
	${CROSS_COMPILE}gcc -c Lib_SSPIDrv.c
	${CROSS_COMPILE}ar rcs libsspi.a Lib_SSPIDrv.o
	${CROSS_COMPILE}gcc -shared -Wl,-soname,libsspi.so -o libsspi.so Lib_SSPIDrv.o -lc



clean:
	rm -f *.o *.a *.so *.mod.c
	
cleanall: clean
	rm -rf .*.cmd *.bak *.symvers .tmp_versions
