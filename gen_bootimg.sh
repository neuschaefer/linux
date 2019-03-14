#! /bin/sh
PROJECT_PATH=$(pwd)
PRODUCT_NAME=$(cat ${PROJECT_PATH}/.model | grep -m 1 PRODUCT_NAME | cut -d'=' -f2,2);  ## define in build_model
KERNEL_VER=$(cat ${PROJECT_PATH}/.model | grep -m 1 KERNEL | cut -d'=' -f2,2);  ## define in .model
KERNEL_PATH="${PROJECT_PATH}"/kernel_imx${KERNEL_VER}
OUT_PATH="${PROJECT_PATH}"/out/target/product/${PRODUCT_NAME}

if [ -z ${PRODUCT_NAME} ]; then
	echo
	echo "----------------------------------------------------------"
	echo "[PRODUCT_NAME] Undefined !! execute ./build_config.sh help"
	echo "----------------------------------------------------------"
	echo
	exit -1;
fi	
	
if [ -f "${KERNEL_PATH}"/boot.img ] ; then
	rm "${KERNEL_PATH}"/boot.img
fi

if [ -f "${OUT_PATH}"/ramdisk.img ] ; then
	#"${KERNEL_PATH}"/mkbootimg --kernel "${KERNEL_PATH}"/arch/arm/boot/zImage --ramdisk "${OUT_PATH}"/ramdisk.img --base 0x80800000 --cmdline "console=ttymxc0,115200 init=/init androidboot.console=ttymxc0 max17135:pass=2,vcom=-2310000 fbmem=6M video=mxcepdcfb:E060SCM,bpp=16 no_console_suspend" --board evk_6sl_eink -o "${KERNEL_PATH}"/boot.img
	"${KERNEL_PATH}"/mkbootimg --kernel "${KERNEL_PATH}"/arch/arm/boot/zImage --ramdisk "${OUT_PATH}"/ramdisk.img --base 0x80800000 --cmdline "console=ttymxc0,115200 init=/init androidboot.console=ttymxc0 max17135:pass=2, fbmem=6M video=mxcepdcfb:E060SCM,bpp=16 no_console_suspend" --board evk_6sl_eink -o "${KERNEL_PATH}"/boot.img
	mv "${KERNEL_PATH}"/boot.img "${OUT_PATH}"/boot.img
fi
