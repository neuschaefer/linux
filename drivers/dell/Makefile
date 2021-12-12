# Global environments
export CDRoot=${PWD}/..
export CT_PATH=${CDRoot}/crosstool
export CT_BIN_PATH=${CT_PATH}/arm-softfloat-linux-gnu/bin
export PLATFORM=softfloat
export ARCH=arm
export TARGET=${ARCH}-${PLATFORM}-linux-gnu
export CROSS_COMPILE=${TARGET}-
export TARGET_PREFIX=${CT_PATH}/${TARGET}/${TARGET}
PATH:=${CT_BIN_PATH}:${PATH}

DriverModule =			\
	gpio			\
	mem			\
	mtd			\
	dynairq			\
	fantach			\
	aes			\
	adc			\
	pwm			\
	kcs			\
	eventhandler		\
	vkcs			\
	peci			\
	biospost		\
	spi			\
	i2c			\
	timer			\
	sspi			\
	wdt

BuildModule=$(strip $(foreach i, $(strip ${DriverModule}), ${i}/build))

CleanModule=$(strip $(foreach i, $(strip ${DriverModule}), ${i}/clean))

CleanAllModule=$(strip $(foreach i, $(strip ${DriverModule}), ${i}/cleanall))

UninstallModule=$(strip $(foreach i, $(strip ${DriverModule}), ${i}/uninstall))

InstallModule=$(strip $(foreach i, $(strip ${DriverModule}), ${i}/install))

.PHONY: untar all build clean install cleanall ${BuildModule} ${CleanModule} ${InstallModule} ${UninstallModule}

all: untar build install 

untar:
	@for drv in ${DriverModule} ; do		\
		echo "--untar $${drv} ";		\
		tar jxf $${drv}.tar.bz2 ;			\
	done

build: ${BuildModule}

clean: ${CleanModule}

install: ${InstallModule} 
	
uninstall: ${UninstallModule}

cleanall: ${CleanAllModule}



${UninstallModule}:
	cd $(dir $@); make uninstall

${BuildModule}:
	cd $(dir $@); make build

${CleanModule}:
	cd $(dir $@); make clean
	
${CleanAllModule}:
	cd $(dir $@); make cleanall
	
${InstallModule}:
	cd $(dir $@); make install
