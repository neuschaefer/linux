#!/bin/sh

if [ "$1" != "defconfig" -a "$1" != "menuconfig" -a "$1" != "uImage" -a "$1" != "modules" -a "$1" != "clean" ] ; then
	echo "usage: make.sh {defconfig|menuconfig|uImage|modules|clean}"
	exit 1
fi

export PATH=/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin:$PATH
#export PATH=/opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin:$PATH
#export PATH=/opt/arm-2008q3/bin:$PATH

[ $1 = "uImage" ] && rm -f uImage bImage bImage.bz2 vmlinux
[ $1 = "modules" ] && find drivers/video/sw-epd/v2/ -name '*.o' -exec rm -f \{} \;
[ $1 = "modules" ] && rm -rf modules_build
[ $1 = "modules" ] && find . -name "*.ko" -exec rm -f \{} \;

[ $1 = "defconfig" ] && make ARCH=arm CROSS_COMPILE=arm-fsl-linux-gnueabi- 624_defconfig 
[ $1 = "defconfig" ] && exit

make ARCH=arm CROSS_COMPILE=arm-fsl-linux-gnueabi- "$@" -j8

[ $1 = "uImage" ] && arm-fsl-linux-gnueabi-objcopy  -R .note.gnu.build-id -S -O binary vmlinux bImage
[ $1 = "uImage" ] && bzip2 -k bImage

[ $1 = "modules" ] && mkdir -p modules_build
[ $1 = "modules" ] && find -L . -name "*.ko" -exec cp -f \{} modules_build/ \;

