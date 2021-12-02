#make -f gcc.mak CA9=y CPU_TEST=y
#
#	@echo "$(firstword $(subst /, ,$(word 2, $(subst /drv_build/,/drv_build /, $(PWD)))))"
#	@echo "$(subst /, ,$(word 2, $(subst /drv_build/,/drv_build /, $(PWD))))"
#	@echo "$(word 2, $(subst /drv_build/,/drv_build /, $(PWD)))"
#	@echo "$(subst /drv_build/,/drv_build /, $(PWD))"
#mt5396
# mt5396 driver 5396_driver cpu
#/mt5396/driver/5396_driver/cpu
# /proj/ytchen/CC_Views/p4v/DTV/PROD_BR/DTV_X_IDTV0801/vm_proj_x/project_x/drv_build /mt5396/driver/5396_driver/cpu

SELF := cpu
MW_INC_ROOT := $(word 1, $(subst /driver/,/driver /, $(PWD)))/mw_lib/x_inc/
DRV_INC_ROOT := $(word 1, $(subst /$(SELF), , $(PWD)))

CC = arm11_mtk_le-gcc
EXEC = gtest
SRC = perf_heap_sort.c perf_mem_access_W.c rand.c number_array.c perf_mem_access_R.c perf_quick_sort.c perf_test_prime.c
SRC += dhry21a.c dhry21b.c
ASM_SRC =

ifeq "$(CA9)" "y"
SRC += perf_ca9.c
ASM_SRC += v7_pmu.s
else
SRC += perf_hal1176.c v6_example.c
ASM_SRC += v6_pmu.s
endif


CC_FLAG = -O3
CC_FLAG += -D__GCC__ -D__CC_GCC__ -D_CPU_LITTLE_ENDIAN_ -DCPU_TEST_ENABLE
CC_FLAG += -I$(MW_INC_ROOT)
CC_FLAG += -I$(DRV_INC_ROOT)/drv_inc
CC_FLAG += -I$(DRV_INC_ROOT)/inc

OBJ_DIR = .

.PHONY: clean all


all:
	ln -sf ../dhrystone/dhry21a.c
	ln -sf ../dhrystone/dhry21b.c
	ln -sf ../dhrystone/dhry.h
	$(CC) -o $(EXEC) $(SRC) $(CC_FLAG)
	@echo "Build successfully!"

clean:
	rm -f `find -type l`
	rm -rf *.o $(EXEC)
