/* these code will be removed to sram.
 * function: open the mmu, and jump to dram, for continuing resume*/
#include "./../super_i.h"

extern unsigned int save_sp(void);
extern int jump_to_resume(void* pointer, __u32 *addr);
extern void restore_mmu_state(struct mmu_state *saved_mmu_state);
extern void mem_flush_tlb(void);
extern void flush_icache(void);
extern void flush_dcache(void);
extern void invalidate_dcache(void);
extern void mem_preload_tlb_nommu(void);
extern void clear_reg_context(void);

extern void disable_cache(void);
extern void disable_program_flow_prediction(void);
extern void invalidate_branch_predictor(void);
extern void enable_cache(void);
extern void enable_program_flow_prediction(void);
extern void disable_dcache(void);
extern void disable_l2cache(void);

static struct aw_mem_para mem_para_info;

extern char *__bss_start;
extern char *__bss_end;
static __s32 dcdc2, dcdc3;
static __u32 sp_backup;
static char    *tmpPtr = (char *)&__bss_start;
static __u32 status = 0; 

#ifdef RETURN_FROM_RESUME0_WITH_MMU
#define MMU_OPENED
#undef POWER_OFF
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
#endif

#ifdef RETURN_FROM_RESUME0_WITH_NOMMU
#undef MMU_OPENED
#undef POWER_OFF
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
#endif

#if defined(ENTER_SUPER_STANDBY) || defined(ENTER_SUPER_STANDBY_WITH_NOMMU) || defined(WATCH_DOG_RESET)
#undef MMU_OPENED
#define POWER_OFF
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
#endif

static void restore_ccmu(void);

int main(void)
{
	/* clear bss segment */
	do{*tmpPtr ++ = 0;}while(tmpPtr <= (char *)&__bss_end);
	
#ifdef MMU_OPENED
	save_mem_status(RESUME1_START);
#else
	//busy_waiting();
	//save_mem_status_nommu(RESUME1_START);
	status = save_sun5i_mem_status_nommu(RESUME1_START);
#endif

	
#ifdef MMU_OPENED
	save_mem_status(RESUME1_START |0x02);
	//move other storage to sram: saved_resume_pointer(virtual addr), saved_mmu_state
	mem_memcpy((void *)&mem_para_info, (void *)(DRAM_BACKUP_BASE_ADDR1), sizeof(mem_para_info));
#else
	mem_preload_tlb_nommu();
	/*switch stack*/
	//save_mem_status_nommu(RESUME1_START |0x02);
	save_sun5i_mem_status_nommu(RESUME1_START |0x02);
	//move other storage to sram: saved_resume_pointer(virtual addr), saved_mmu_state
	mem_memcpy((void *)&mem_para_info, (void *)(DRAM_BACKUP_BASE_ADDR1_PA), sizeof(mem_para_info));
	/*restore mmu configuration*/
	//save_mem_status_nommu(RESUME1_START |0x03);
	save_sun5i_mem_status_nommu(RESUME1_START |0x03);
	
	//busy_waiting();
	restore_mmu_state(&(mem_para_info.saved_mmu_state));
	disable_dcache();
	save_sun5i_mem_status(RESUME1_START |0x13);

#endif

//after open mmu mapping
#ifdef FLUSH_TLB
	//busy_waiting();
	mem_flush_tlb();
	mem_preload_tlb();
#endif
	save_sun5i_mem_status(RESUME1_START |0x04);
#ifdef FLUSH_ICACHE
	//clean i cache
	flush_icache();
#endif
	
	//save_mem_status(RESUME1_START |0x05);
	save_sun5i_mem_status(RESUME1_START |0x05);
	mem_clk_init();
	//twi freq?
	setup_twi_env();
	mem_twi_init(AXP_IICBUS);
	//save_mem_status(RESUME1_START |0x07);
	save_sun5i_mem_status(RESUME1_START |0x07);

#ifdef POWER_OFF
	restore_ccmu();
#endif

	/*restore pmu config*/
#ifdef POWER_OFF
	mem_power_exit(mem_para_info.axp_event);
	//save_mem_status(RESUME1_START |0x8);
	save_sun5i_mem_status(RESUME1_START |0x8);
	/* disable watch-dog: coresponding with boot0 */
	mem_tmr_init();
	mem_tmr_disable_watchdog();
#endif

//before jump to late_resume	
#ifdef FLUSH_TLB
	//busy_waiting();
	//save_mem_status(RESUME1_START |0x9);
	save_sun5i_mem_status(RESUME1_START |0x9);
	mem_flush_tlb();
#endif

#ifdef FLUSH_ICACHE
	//clean i cache
	//save_mem_status(RESUME1_START |0xa);
	save_sun5i_mem_status(RESUME1_START |0xa);
	flush_icache();
#endif

	//before jump, invalidate data
	jump_to_resume((void *)mem_para_info.resume_pointer, mem_para_info.saved_runtime_context_svc);
	
	return;
}

void restore_ccmu(void)
{
	/* gating off dram clock */
	//mem_clk_dramgating(0);
	int i=0;
	
	for(i=0; i<6; i++){
		dram_hostport_on_off(i, 0);
	}
	
	for(i=16; i<31; i++){
		dram_hostport_on_off(i, 0);
	}
	
	dcdc2 = mem_para_info.suspend_dcdc2;
	dcdc3 = mem_para_info.suspend_dcdc3;

	while( 0 != mem_set_voltage(POWER_VOL_DCDC2, dcdc2)){
			;
	}
	while(0 != mem_set_voltage(POWER_VOL_DCDC3, dcdc3)){
			;
	}
	
	change_runtime_env(1);
	delay_ms(10);

	mem_clk_setdiv(&mem_para_info.clk_div);
	mem_clk_set_pll_factor(&mem_para_info.pll_factor);
	change_runtime_env(1);
	delay_ms(5);

	/* gating on dram clock */
	//mem_clk_dramgating(1);
	for(i=0; i<6; i++){
		dram_hostport_on_off(i, 1);
	}
	
	for(i=16; i<31; i++){
		dram_hostport_on_off(i, 1);
	}

	return;
}



