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
export CROSS_COMPILE=${TARGET}-
PATH:=${CT_BIN_PATH}:${PATH}
endif

Module=linux-2.6.23.1

ModuleName = aess_pwmdrv
InstallPath = ${CDRoot}/drivers/rootfs
ScriptFile = ${InstallPath}/etc/sysapps_script/pwm_drv.sh

build: buildall bin

buildall:
	cd pwm_driver;make kernel=${CT_PATH}_addon/${Module} all

bin:
	rm -rf binary
	mkdir -p binary/pwm_driver
	cd pwm_driver;cp -rf ${ModuleName}.ko ../binary/pwm_driver


clean:
	cd pwm_driver;make clean;
	rm -rf binary

cleanall: clean uninstall
	cd pwm_driver; make cleanall

uninstall:
	rm -rf ${InstallPath}/lib/modules/${ModuleName}.ko
	rm -f ${ScriptFile}

install: uninstall
	cd binary/pwm_driver;\
	mkdir -p ${InstallPath}/lib/modules; \
	cp -f ${ModuleName}.ko ${InstallPath}/lib/modules/
	mkdir -p `dirname ${ScriptFile}`
	echo 'insmod /lib/modules/${ModuleName}.ko' > ${ScriptFile}
	echo 'rm -rf /dev/${ModuleName};dmesg | grep /dev/${ModuleName} | cut -c 4- > /tmp/cmd;. /tmp/cmd;rm -f /tmp/cmd' >> ${ScriptFile}
