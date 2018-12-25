#!/bin/sh

echo "build standby"

CROSS_PREFIX="/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-"

make ARCH=arm CROSS_COMPILE=${CROSS_PREFIX} KDIR=${PWD} -C ${PWD}/arch/arm/mach-sun5i/pm/standby all

