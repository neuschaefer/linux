/*the following code reside in dram when enter super mem
 * actually, these code will be load into dram when start & load the system.
 * the addr is : 0xc0f0,0000; size is 256Kbyte;
 */
	
/* these code will be located in fixed addr
 * after receive wake up signal, boot will jump to the fixed addr
 * and excecute these code, and jump to sram for continuing resume.
 */

 #include "./../super_i.h"

extern unsigned int save_sp_nommu(void);
extern void mem_flush_tlb(void);
extern void flush_icache(void);
extern void flush_dcache(void);
extern void enable_icache(void);
extern void invalidate_dcache(void);
extern void mem_preload_tlb_nommu(void);

static struct aw_mem_para mem_para_info;
extern char *resume1_bin_start;
extern char *resume1_bin_end;
static int (*resume1)(void);

#ifdef RETURN_FROM_RESUME0_WITH_MMU
#define MMU_OPENED
#define SWITCH_STACK
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
#endif

#ifdef RETURN_FROM_RESUME0_WITH_NOMMU
#undef MMU_OPENED
#define SWITCH_STACK
#define SET_COPRO_REG
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
#endif 

#if defined(ENTER_SUPER_STANDBY) || defined(ENTER_SUPER_STANDBY_WITH_NOMMU) || defined(WATCH_DOG_RESET)
#undef MMU_OPENED
#define SWITCH_STACK
#define SET_COPRO_REG
#define FLUSH_TLB
#define FLUSH_ICACHE
//#define INVALIDATE_DCACHE
#endif

// #define no_save __attribute__ ((section(".no_save")))
int main(void)
{

#ifdef SWITCH_STACK
#ifdef MMU_OPENED
	save_sp();
#else
	save_sp_nommu();
#endif
#endif

#ifndef GET_CYCLE_CNT
	init_perfcounters(1, 0);
#endif

	//busy_waiting();
#ifdef SET_COPRO_REG
	set_copro_default();
	save_mem_status_nommu(RUSUME0_START |0x01);
	save_sun5i_mem_status_nommu(RUSUME0_START |0x01);
	//busy_waiting();
	fake_busy_waiting();
#endif
	//busy_waiting();
	//save_mem_status(RUSUME0_START + 0x20);
	//why? flush tlb, then enable mmu abort.because 0x0000,0000 use ttbr0

#ifdef FLUSH_TLB
	mem_flush_tlb();
	save_sun5i_mem_status_nommu(RUSUME0_START |0x02);
#endif

#ifdef FLUSH_ICACHE
	//clean i cache
	flush_icache();
	enable_icache();
	save_sun5i_mem_status_nommu(RUSUME0_START |0x03);
#endif

#ifdef INVALIDATE_DCACHE
	/*notice: will corrupt r0 - r11*/
	invalidate_dcache();
	save_sun5i_mem_status_nommu(RUSUME0_START |0x04);
#endif

	/* preload tlb for mem */
	//mem_preload_tlb();
	//busy_waiting();

#ifdef MMU_OPENED
	save_mem_status(RUSUME0_START |0x02);
	/*restore dram training area*/
	mem_memcpy((void *)DRAM_BASE_ADDR, (void *)DRAM_BACKUP_BASE_ADDR2, sizeof(__u32)*DRAM_TRANING_SIZE);
	//busy_waiting();
	resume1 = (int (*)(void))SRAM_FUNC_START;	
	//move resume1 code from dram to sram
	mem_memcpy((void *)SRAM_FUNC_START, (void *)&resume1_bin_start, (int)&resume1_bin_end - (int)&resume1_bin_start);
	//sync
	save_mem_status(RUSUME0_START |0x03);
	//jump to sram
	resume1();
	/*never get here.*/
	save_mem_status(RUSUME0_START | 0x04);
#else

	save_mem_status_nommu(RUSUME0_START |0x02);
	save_sun5i_mem_status_nommu(RUSUME0_START |0x05);
	mem_preload_tlb_nommu();
	/*restore dram training area*/
	mem_memcpy((void *)DRAM_BASE_ADDR_PA, (void *)DRAM_BACKUP_BASE_ADDR2_PA, sizeof(__u32)*DRAM_TRANING_SIZE);


	//busy_waiting();
	resume1 = (int (*)(void))SRAM_FUNC_START_PA;	
	//move resume1 code from dram to sram
	mem_memcpy((void *)SRAM_FUNC_START_PA, (void *)&resume1_bin_start, (int)&resume1_bin_end - (int)&resume1_bin_start);
	//sync	
	save_mem_status_nommu(RUSUME0_START | 0x03);
	save_sun5i_mem_status_nommu(RUSUME0_START |0x06);

	//jump to sram
	resume1();
	/*never get here.*/
	save_mem_status_nommu(RUSUME0_START | 0x04);
	save_sun5i_mem_status_nommu(RUSUME0_START |0x07);
#endif


	while(1);
}

