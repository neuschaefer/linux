#!/bin/bash

# user setting
KERNEL_ROOT=$(pwd)

chip_config=$1
configname=${chip_config##*config}

# Release SOP

make_perf_release()
{
	if [ -e ./.config ]; then
		echo [Info] Perf Clean config
		make distclean
	fi
	echo =====================
	echo [Info] Default Config
	echo =====================
	cp $chip_config .config
	cp .config arch/arm/configs/mstar_config
	make defconfig KBUILD_DEFCONFIG=mstar_config
	make clean
	echo ==================
	echo [Info] Build Perf
	echo ==================
	make -j2
	make uImage
	echo
	echo [Info] Build Perf Done!
	echo
	rm -rf bsp_release_perf
	if [ -e ./kernel_release_perf_config$configname.tar.gz ]; then
		rm kernel_release_perf_config$configname.tar.gz
	fi
	if [ -e ./kernel_vmlinux_perf_config$configname.tar.gz ]; then
		rm kernel_vmlinux_perf_config$configname.tar.gz
	fi
	mkdir bsp_release_perf
	cp arch/arm/boot/uImage bsp_release_perf
	cp `find -L -name "*.ko"` bsp_release_perf
	tar -zcvf kernel_release_perf_config$configname.tar.gz bsp_release_perf
	tar -zcvf kernel_vmlinux_perf_config$configname.tar.gz vmlinux
	chmod 755 kernel_release_perf_config$configname.tar.gz
	chmod 755 kernel_vmlinux_perf_config$configname.tar.gz
	echo ==================
	echo [Info] Create Perf Kernel Release Package Done!
	echo ==================
}

make_debug_release()
{
	if [ -e ./.config ]; then
		echo [Info] Debug Clean config
		make distclean
	fi
	echo =====================
	echo [Info] Default Config
	echo =====================
	cp $chip_config .config
	cp .config arch/arm/configs/mstar_config
	make DEBUGKERNEL=1 defconfig KBUILD_DEFCONFIG=mstar_config
	make clean
	echo ==================
	echo [Info] Build Debug
	echo ==================
	make DEBUGKERNEL=1 -j2
	make DEBUGKERNEL=1 uImage
	echo
	echo [Info] Build Debug Done!
	echo
	rm -rf bsp_release_debug
	if [ -e ./kernel_release_debug_config$configname.tar.gz ]; then 
		rm kernel_release_debug_config$configname.tar.gz
	fi
	if [ -e ./kernel_vmlinux_debug_config$configname.tar.gz ]; then
		rm kernel_vmlinux_debug_config$configname.tar.gz
	fi
	mkdir bsp_release_debug
	cp arch/arm/boot/uImage bsp_release_debug
	cp `find -L -name "*.ko"` bsp_release_debug
	tar -zcvf kernel_release_debug_config$configname.tar.gz bsp_release_debug
	tar -zcvf kernel_vmlinux_debug_config$configname.tar.gz vmlinux	
	chmod 755 kernel_release_debug_config$configname.tar.gz
	chmod 755 kernel_vmlinux_debug_config$configname.tar.gz
	echo ==================
	echo [Info] Create Debug Kernel Release Package Done!
	echo ==================
}

option=$2
sh genlink_mstar2.sh
if [ "$option" == "" ]; then
	make_debug_release
	make_perf_release
elif [ "$option" == "perf" ]; then
	make_perf_release
elif [ "$option" == "debug" ]; then
	make_debug_release
else
	echo "[ERROR] usage: sh release_arm.sh config_name [perf|debug]"
fi
