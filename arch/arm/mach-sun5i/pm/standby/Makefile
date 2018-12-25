#makefile for standby.bin

INCLUDE   = -I. \
	-I$(KDIR)/include \
	-I$(KDIR)/arch/arm/mach-sun5i/include
	
STANDBY_OBJ = 	common.o         	\
		standby.o        	\
		stack.o          	\
		standby_clock.o  	\
		standby_int.o    	\
		standby_ir.o     	\
		standby_key.o    	\
		standby_power.o  	\
		standby_tmr.o    	\
		standby_twi.o    	\
		standby_usb.o    	\
		standby_delay.o  	\
		pm_debug.o       	\
		dram/dram.o      	\
		dram/dram_init.o
	
SUSPEND_OBJ =	./super/common.o              	\
		./super/stack.o               	\
		./super/super_int.o         	\
		./super/super_tmr.o         	\
		./super/super_clock.o       	\
		./super/super_twi.o         	\
		./super/super_delay.o       	\
		./super/super_power.o       	\
		./super/suspend/suspend.o 	\
		./super/dram/dram.o           	\
		./super/dram/dram_init.o      	\
		pm_debug.o                    	\
		mem_mmu_pc_asm.o              	\
		mem_mmu_pc.o                  	\
		mem_cpu.o                     	 
	
RESUME1_OBJ =	./super/common.o             \
		./super/stack.o              \
		./super/super_twi.o        \
		./super/super_delay.o      \
		./super/super_clock.o      \
		./super/super_power.o      \
		./super/super_int.o        \
		./super/super_tmr.o        \
		./super/dram/dram.o          \
		./super/dram/dram_init.o     \
		./super/resume/resume1.o \
		pm_debug.o                   \
		mem_mmu_pc.o                 \
		mem_mmu_pc_asm.o             \
		mem_cpu.o                    
	
RESUME0_OBJ =	./super/common.o             \
		./super/stack.o              \
		./super/super_twi.o        \
		./super/super_delay.o      \
		./super/super_clock.o      \
		./super/resume1_asm.o        \
		./super/resume/resume0.o \
		pm_debug.o                   \
		mem_mmu_pc_asm.o             \
		mem_cpu.o                     
	
STANDBY_OUPUTFILE = standby
SUSPEND_OUPUTFILE = suspend
RESUME1_OUPUTFILE = resume1
RESUME0_OUPUTFILE = resume0

STANDBY_LD_FILE = standby.xn
SUSPEND_LD_FILE = suspend.xn
RESUME1_LD_FILE = resume1.xn
RESUME0_LD_FILE = resume0.xn

CC = $(CROSS_COMPILE)gcc

CFLAGS = -Os -g -march=armv7-a -fno-unwind-tables -fno-asynchronous-unwind-tables -mlittle-endian  -c

all: standby suspend resume1 resume0

standby:
	$(CC) $(INCLUDE) $(CFLAGS) common.c           -o common.o
	$(CC) $(INCLUDE) $(CFLAGS) standby.c          -o standby.o
	$(CC) $(INCLUDE) $(CFLAGS) stack.S            -o stack.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_clock.c    -o standby_clock.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_int.c      -o standby_int.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_ir.c       -o standby_ir.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_key.c      -o standby_key.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_power.c    -o standby_power.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_tmr.c      -o standby_tmr.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_twi.c      -o standby_twi.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_usb.c      -o standby_usb.o
	$(CC) $(INCLUDE) $(CFLAGS) standby_delay.S    -o standby_delay.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../pm_debug.c    -o pm_debug.o 
	$(CC) $(INCLUDE) $(CFLAGS) dram/dram.c        -o dram/dram.o
	$(CC) $(INCLUDE) $(CFLAGS) dram/dram_init.c   -o dram/dram_init.o

	$(CROSS_COMPILE)ld -T $(STANDBY_LD_FILE)  -EL  $(STANDBY_OBJ) -o $(STANDBY_OUPUTFILE).elf  -Map $(STANDBY_OUPUTFILE).map
	$(CROSS_COMPILE)objdump -D $(STANDBY_OUPUTFILE).elf > $(STANDBY_OUPUTFILE).lst
	$(CROSS_COMPILE)objcopy -O binary $(STANDBY_OUPUTFILE).elf $(STANDBY_OUPUTFILE).bin

	rm -rf *.o ../*.o

	cp standby.bin standby.code

	@echo ----------------------------------------
	@echo well done!
	
suspend:
	$(CC) $(INCLUDE) $(CFLAGS) ./super/common.c                    -o ./super/common.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/stack.S                     -o ./super/stack.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_int.c               -o ./super/super_int.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_tmr.c               -o ./super/super_tmr.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_clock.c             -o ./super/super_clock.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_twi.c               -o ./super/super_twi.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_power.c             -o ./super/super_power.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_delay.S             -o ./super/super_delay.o
	
	$(CC) $(INCLUDE) $(CFLAGS) ./super/suspend/suspend.c       -o ./super/suspend/suspend.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/dram/dram.c                 -o ./super/dram/dram.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/dram/dram_init.c            -o ./super/dram/dram_init.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../pm_debug.c                     -o pm_debug.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_mmu_pc_asm.S               -o mem_mmu_pc_asm.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_mmu_pc.c                   -o mem_mmu_pc.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_cpu.c		       -o mem_cpu.o

	$(CROSS_COMPILE)ld -T $(SUSPEND_LD_FILE)  -EL  $(SUSPEND_OBJ) -o $(SUSPEND_OUPUTFILE).elf  -Map $(SUSPEND_OUPUTFILE).map
	$(CROSS_COMPILE)objdump -D $(SUSPEND_OUPUTFILE).elf > $(SUSPEND_OUPUTFILE).lst
	$(CROSS_COMPILE)objcopy -O binary $(SUSPEND_OUPUTFILE).elf $(SUSPEND_OUPUTFILE).bin

	rm -rf *.o ../*.o

	cp suspend.bin ./super/suspend/suspend.code

	@echo ----------------------------------------
	@echo well done!
	
resume1:
	$(CC) $(INCLUDE) $(CFLAGS) ./super/common.c                    -o ./super/common.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/stack.S                     -o ./super/stack.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_twi.c               -o ./super/super_twi.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_delay.S             -o ./super/super_delay.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_clock.c             -o ./super/super_clock.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_power.c             -o ./super/super_power.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_tmr.c               -o ./super/super_tmr.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_int.c               -o ./super/super_int.o
	
	$(CC) $(INCLUDE) $(CFLAGS) ./super/dram/dram.c                 -o ./super/dram/dram.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/dram/dram_init.c            -o ./super/dram/dram_init.o
	
	$(CC) $(INCLUDE) $(CFLAGS) ./super/resume/resume1.c        -o ./super/resume/resume1.o

	
	$(CC) $(INCLUDE) $(CFLAGS) ./../pm_debug.c                     -o pm_debug.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_mmu_pc.c                   -o mem_mmu_pc.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_mmu_pc_asm.S               -o mem_mmu_pc_asm.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_cpu.c		       -o mem_cpu.o
	
	
	$(CROSS_COMPILE)ld -T $(RESUME1_LD_FILE)  -EL  $(RESUME1_OBJ) -o $(RESUME1_OUPUTFILE).elf  -Map $(RESUME1_OUPUTFILE).map
	$(CROSS_COMPILE)objdump -D $(RESUME1_OUPUTFILE).elf > $(RESUME1_OUPUTFILE).lst
	$(CROSS_COMPILE)objcopy -O binary $(RESUME1_OUPUTFILE).elf $(RESUME1_OUPUTFILE).bin

	rm -rf *.o ../*.o

	cp resume1.bin ./super/resume/resume1.code

	@echo ----------------------------------------
	@echo well done!

resume0:
	$(CC) $(INCLUDE) $(CFLAGS) ./super/common.c                    -o ./super/common.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/stack.S                     -o ./super/stack.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_twi.c               -o ./super/super_twi.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_delay.S             -o ./super/super_delay.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/super_clock.c             -o ./super/super_clock.o
	$(CC) $(INCLUDE) $(CFLAGS) ./super/resume1_asm.S               -o ./super/resume1_asm.o
	
	$(CC) $(INCLUDE) $(CFLAGS) ./super/resume/resume0.c        -o ./super/resume/resume0.o
	
	$(CC) $(INCLUDE) $(CFLAGS) ./../pm_debug.c                     -o pm_debug.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_mmu_pc_asm.S               -o mem_mmu_pc_asm.o
	$(CC) $(INCLUDE) $(CFLAGS) ./../mem_cpu.c		       -o mem_cpu.o
	
	$(CROSS_COMPILE)ld -T $(RESUME0_LD_FILE)  -EL  $(RESUME0_OBJ) -o $(RESUME0_OUPUTFILE).elf  -Map $(RESUME0_OUPUTFILE).map
	$(CROSS_COMPILE)objdump -D $(RESUME0_OUPUTFILE).elf > $(RESUME0_OUPUTFILE).lst
	$(CROSS_COMPILE)objcopy -O binary $(RESUME0_OUPUTFILE).elf $(RESUME0_OUPUTFILE).bin

	rm -rf *.o ../*.o

	cp resume0.bin ./super/resume/resume0.code

	@echo ----------------------------------------
	@echo well done!
	
clean:
	-rm *.o *.map *.lst *.bin *.elf *.exe dram/*.o

