# Related path to CD root
RelRoot=${PWD}/../..

# Global environments
ifndef CDRoot
CDRoot=${RelRoot}
export CT_PATH=${CDRoot}/crosstool
export CT_BIN_PATH=${CT_PATH}/arm-softfloat-linux-gnu/bin
export PLATFORM=softfloat
export ARCH=arm
export TARGET=${ARCH}-${PLATFORM}-linux-gnu
export TARGET_PREFIX=${CT_PATH}/${TARGET}/${TARGET}
export CROSS_COMPILE=${TARGET}-
PATH:=${CT_BIN_PATH}:${PATH}
endif

Module=linux-2.6.23.1


ModuleName1 = aess_mtddrv_spinor
ModuleName2 = aess_mtddrv_nand
ModDir = mtd_driver

InstallPath = ${CDRoot}/drivers/rootfs


MOD_PATH = /lib/modules
ModInstallPath = ${InstallPath}${MOD_PATH}



build: buildall

buildall:
	cd ${ModDir};make kernel=${CT_PATH}_addon/${Module} ARCH=${ARCH} all


clean:
	cd ${ModDir}; make clean;
	
cleanall: uninstall clean
	cd ${ModDir}; make cleanall

uninstall:
	rm -rf ${InstallPath}/lib/modules/${ModuleName1}.ko
	rm -rf ${InstallPath}/lib/modules/${ModuleName2}.ko

	
install: uninstall
	mkdir -p ${ModInstallPath}
	cp -f ${ModDir}/${ModuleName1}.ko ${ModInstallPath}
	cp -f ${ModDir}/${ModuleName2}.ko ${ModInstallPath}
