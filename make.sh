#!/bin/sh

set -x

# enter scripts working directory
cd "$(cd "$(dirname "$0")" && pwd)"

KERNEL_PRJ=$(basename `pwd` | grep -oh '[0-9]\{3\}')

if [ "$USER" = "jenkins" ] ;
then
	CROSS_PREFIX="/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-"
else
#	CROSS_PREFIX="ccache /home/mike/x-tools/arm-obreey-linux-gnueabi/bin/arm-obreey-linux-gnueabi-"
	CROSS_PREFIX="/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-"
	BUILD_DIR=/tmp/${KERNEL_PRJ}_bin
	HEADERS_DIR=/tmp/${KERNEL_PRJ}_headers
	MODULES_DIR=/tmp/${KERNEL_PRJ}_modules

	install -d $BUILD_DIR
	install -d $HEADERS_DIR
	install -d $MODULES_DIR
fi

dpkg -s ccache &> /dev/null
[ $? -eq 0 ] && CROSS_PREFIX="ccache ${CROSS_PREFIX}"

NCPUS=`cat /proc/cpuinfo | grep processor | wc -l`

EPD_DIR=drivers/video/sw-epd

HG_EPD_REV=$(hg id -i -R ${EPD_DIR})
HG_EPD_BRANCH=$(hg branch -R ${EPD_DIR})

export HG_EPD_REV
export HG_EPD_BRANCH

#make INSTALL_HDR_PATH=$HEADERS_DIR INSTALL_MOD_PATH=$MODULES_DIR CROSS_COMPILE="$CROSS_PREFIX" ARCH=arm O=$BUILD_DIR -j $NCPUS $1
make INSTALL_HDR_PATH=$HEADERS_DIR INSTALL_MOD_PATH=$MODULES_DIR CROSS_COMPILE="$CROSS_PREFIX" ARCH=arm -j $NCPUS $1

