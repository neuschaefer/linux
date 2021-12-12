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

ModuleName = VKCSDriver
InstallPath = ${CDRoot}/drivers/rootfs
ScriptFile = ${InstallPath}/etc/sysapps_script/vkcs_drv.sh

build: buildall bin

buildall:
	cd Library;make all;\
	cp -f IPMIRaw.a IPMIRaw.h ../CmdTool;\
	cd ..;
	cd CmdTool;make all;
	cd vKCSDriver; make kernel=${CT_PATH}_addon/${Module} all

bin:
	rm -rf binary
	mkdir -p binary/Library binary/CmdTool binary/vKCSDriver
	cd Library;cp -rf IPMIRaw.a IPMIRaw.h ../binary/Library
	cd CmdTool;cp -rf IPMICmd ../binary/CmdTool
	cd vKCSDriver;cp -rf ${ModuleName}.ko ../binary/vKCSDriver


clean:
	cd Library;make clean;
	cd CmdTool;make clean;rm -f IPMIRaw.a IPMIRaw.h;
	cd vKCSDriver;make clean;
	rm -rf binary

cleanall: clean uninstall
	cd Library; make cleanall
	cd CmdTool; make cleanall
	cd vKCSDriver; make cleanall

uninstall:
	rm -f ${InstallPath}/bin/IPMICmd
	rm -f ${InstallPath}/lib/modules/${ModuleName}.ko
	rm -f ${ScriptFile}

install: uninstall
	cd binary/CmdTool; \
	${CROSS_COMPILE}strip IPMICmd; \
	mkdir -p ${InstallPath}/bin; \
	cp -f IPMICmd ${InstallPath}/bin/
	cd binary/vKCSDriver; \
	mkdir -p ${InstallPath}/lib/modules;\
	cp -f ${ModuleName}.ko ${InstallPath}/lib/modules/
	mkdir -p `dirname ${ScriptFile}`
	echo 'insmod /lib/modules/${ModuleName}.ko' > ${ScriptFile}
	echo 'rm -rf /dev/vkcs;dmesg | grep /dev/vkcs | cut -c 4- > /tmp/cmd;. /tmp/cmd;rm -f /tmp/cmd' >> ${ScriptFile}
