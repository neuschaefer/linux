#!/bin/sh

do_fail() {
	echo "$1"
	exit -1
}

SWEPD_REPO=drivers/video/sw-epd
MODEL=$(basename `pwd` | grep -oh '[0-9]\{3\}')

hg pull -R ${SWEPD_REPO}
BRANCH=$(hg branch)
echo "Updating sw-epd to branch $BRANCH"
hg update $BRANCH --clean -R ${SWEPD_REPO}

cp -v ${SWEPD_REPO}/${MODEL}_defconfig arch/arm/configs/ || do_fail "File copy failed"

./make.sh mrproper && \
./make.sh ${MODEL}mfg_defconfig && \
./make.sh bImage && \
./make.sh modules && \
./make.sh modules_install && \
echo "All done" 
if [ "$USER" != "ky" ] ;
then
./make.sh tags >/dev/null &
fi
