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


DBG=1
all: IPMIRaw.o chassis_power.o
	${CROSS_COMPILE}ar rcs chassis_power.a IPMIRaw.o chassis_power.o


IPMIRaw.o: IPMIRaw.c IPMIRaw.h
	${CROSS_COMPILE}gcc -fPIC -Wall -g -c IPMIRaw.c
	
chassis_power.o: chassis_power.c chassis_power.h IPMIRaw.o
ifndef DBG
	${CROSS_COMPILE}gcc -fPIC -Wall -g -c chassis_power.c
else
	${CROSS_COMPILE}gcc -DDBG=1 chassis_power.c IPMIRaw.c -o chassis_power_cmd_demo
endif

clean:
	rm -f *.o *.a chassis_power_cmd_demo 

cleanall: clean
	rm -f *.bak

rebuild: clean all

foo:
    