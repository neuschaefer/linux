#
# arch/m68knommu/boot/images/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

MKIMAGE		:= $(srctree)/scripts/mkuboot.sh

extra-y		:= vmlinux.bin vmlinux.gz

# two make processes may write to vmlinux.gz at the same time with make -j
quiet_cmd_mygzip = GZIP    $@
cmd_mygzip = gzip -f -9 < $< > $@.$$$$ && mv $@.$$$$ $@


OBJCOPYFLAGS_vmlinux.bin := -O binary --adjust-vma 0x00000000
$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,mygzip)

quiet_cmd_uimage = UIMAGE  $@
      cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A m68k -O linux -T kernel \
               -C gzip -a 00020000 -e 00020000 -n 'Linux-$(KERNELRELEASE)' \
               -d $< $@

targets += uImage
$(obj)/uImage: $(obj)/vmlinux.gz
	$(Q)rm -f $@
	$(call cmd,uimage)
	@echo -n '  Image: $@ '
	@if [ -f $@ ]; then echo 'is ready' ; else echo 'not made'; fi

# Files generated that shall be removed upon make clean
clean-files	:= vmlinux* uImage
