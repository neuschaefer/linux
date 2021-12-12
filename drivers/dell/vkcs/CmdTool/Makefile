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



all: IPMICmd.o
	${CROSS_COMPILE}gcc -o IPMICmd IPMICmd.o IPMIRaw.a

IPMICmd.o: IPMICmd.c IPMIRaw.h
	${CROSS_COMPILE}gcc -c IPMICmd.c

clean:
	rm -f IPMICmd.o IPMICmd

cleanall: clean
	rm -f *.a *.bak

rebuild: clean all
