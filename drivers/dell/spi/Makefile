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

ModuleName = aess_spidrv
LibName = libspi
HFileName = Lib_SPIDrv
InstallPath = ${CDRoot}/drivers/rootfs
ScriptFile = ${InstallPath}/etc/sysapps_script/spi_drv.sh

build: buildall bin

buildall:
	cd spi_driver;make kernel=${CT_PATH}_addon/${Module} all
	cd spi_lib;make all

bin:
	rm -rf binary
	mkdir -p binary/spi_driver
	mkdir -p binary/spi_lib
	cd spi_driver;cp -f ${ModuleName}.ko ../binary/spi_driver
	cd spi_lib;cp -f ${LibName}.so ../binary/spi_lib
	cd spi_lib;cp -f ${HFileName}.h ${TARGET_PREFIX}/include/


clean:
	cd spi_driver;make clean;
	cd spi_lib;make clean;
	rm -rf binary

cleanall: clean uninstall
	cd spi_driver; make cleanall
	cd spi_lib; make cleanall

uninstall:
	rm -f ${InstallPath}/lib/modules/${ModuleName}.ko
	rm -f ${ScriptFile}
	rm -f ${InstallPath}/usr/lib/${LibName}.so
	rm -f ${TARGET_PREFIX}/include/${HFileName}.h


install: uninstall
	cd binary/spi_driver;\
	mkdir -p ${InstallPath}/lib/modules; \
	cp -f ${ModuleName}.ko ${InstallPath}/lib/modules/
	cd ..
	cd spi_lib;\
	mkdir -p ${InstallPath}/usr/lib; \
	cp -f ${LibName}.so ${InstallPath}/usr/lib/; \
	cp -f ${HFileName}.h ${TARGET_PREFIX}/include/

	mkdir -p `dirname ${ScriptFile}`
	echo 'insmod /lib/modules/${ModuleName}.ko' > ${ScriptFile}
	echo 'rm -rf /dev/${ModuleName};dmesg | grep /dev/${ModuleName} | cut -c 4- > /tmp/cmd;. /tmp/cmd;rm -f /tmp/cmd' >> ${ScriptFile}
