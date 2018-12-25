/*
 * Contact: gqyang <gqyang <at> newbietech.com>                               
 *                                                                                   
 * License terms: GNU General Public License (GPL) version 2                         
 */       
/*
 * the following code need be exceute in sram
 * before dram enter self-refresh,cpu can not access dram.
 */
 
#include "./../super_i.h"
#define RETRY_TIMES (5)

extern char *__bss_start;
extern char *__bss_end;
static int retry = RETRY_TIMES;
static struct aw_mem_para mem_para_info;

extern void mem_flush_tlb(void);
extern void flush_icache(void);
extern void flush_dcache(void);
extern void invalidate_dcache(void);

extern unsigned int save_sp(void);
extern void restore_sp(unsigned int sp);
extern int jump_to_resume0(void* pointer);
extern void mem_flush_tlb(void);
extern void mem_preload_tlb(void);
void disable_cache_invalidate(void);
void disable_mmu(void);
void enable_mmu(void);
void set_ttbr0(void);
static __s32 suspend_with_nommu(void);
static __s32 suspend_with_mmu(void);



#ifdef RETURN_FROM_RESUME0_WITH_MMU
#define SWITCH_STACK
//#define DIRECT_RETRUN
//#define DRAM_ENTER_SELFRESH
//#define INIT_DRAM
//#define MEM_POWER_OFF
#define WITH_MMU
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
#endif

#ifdef RETURN_FROM_RESUME0_WITH_NOMMU
#define SWITCH_STACK
//#define DIRECT_RETRUN
#define DRAM_ENTER_SELFRESH
//#define INIT_DRAM
#define PRE_DISABLE_MMU 
 			/*it is not possible to disable mmu, because 
                                 u need to keep va == pa, before disable mmu
                                 so, the va must be in 0x0000 region.so u need to 
			 creat this mapping before jump to suspend.
                                 so u need ttbr0 
                                 to keep this mapping.  */
#define SET_COPRO_DEFAULT 
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
#define DISABLE_MMU
#define JUMP_WITH_NOMMU

#endif

#ifdef DIRECT_RETURN_FROM_SUSPEND
//#define SWITCH_STACK
#define DIRECT_RETRUN
#endif

#if defined(ENTER_SUPER_STANDBY) 
#define SWITCH_STACK
#undef PRE_DISABLE_MMU
//#define DIRECT_RETRUN
#define DRAM_ENTER_SELFRESH
//#define DISABLE_INVALIDATE_CACHE
//#define INIT_DRAM
//#define DISABLE_MMU
#define MEM_POWER_OFF
#define FLUSH_TLB
#define FLUSH_ICACHE
//#define FLUSH_DCACHE  //can not flush data, if u do this, the data that u dont want save will be saved.
#define INVALIDATE_DCACHE
#endif

#if defined(ENTER_SUPER_STANDBY_WITH_NOMMU) 
#define SWITCH_STACK
#define PRE_DISABLE_MMU
#define DRAM_ENTER_SELFRESH
#define DISABLE_MMU
#define MEM_POWER_OFF
#define FLUSH_TLB
#define FLUSH_ICACHE
#define INVALIDATE_DCACHE
//#define SET_COPRO_DEFAULT 
#endif

#ifdef WATCH_DOG_RESET
#define SWITCH_STACK
#define PRE_DISABLE_MMU
#define DRAM_ENTER_SELFRESH
#define DISABLE_MMU
#define START_WATCH_DOG
#define FLUSH_TLB
#define FLUSH_ICACHE
//#define FLUSH_DCACHE 
                                                        /*can not flush data, if u do this, the data that u dont want save willed. 
                                       * especillay, after u change the mapping table.
                                       */
#define INVALIDATE_DCACHE
#define SET_COPRO_DEFAULT 
#endif

/*
*********************************************************************************************************
*                                   SUPER STANDBY EXECUTE IN SRAM
*
* Description: super mem ,suspend to ram entry in sram.
*
* Arguments  : arg  pointer to the parameter that 
*
* Returns    : none
*
* Note       :
*********************************************************************************************************
*/

int main(void)
{
	char    *tmpPtr = (char *)&__bss_start;
	static __u32 sp_backup = 0;
	__s32 dram_size = 0;
#ifdef GET_CYCLE_CNT
	__u32 start = 0;
#endif

	/* save stack pointer registger, switch stack to sram */
	//mark it, just for test 
#ifdef SWITCH_STACK
#ifdef PRE_DISABLE_MMU
	//busy_waiting();
	sp_backup = save_sp_nommu();
#else
	sp_backup = save_sp();
#endif
#endif	
	save_sun5i_mem_status(SUSPEND_START);

	/* flush data and instruction tlb, there is 32 items of data tlb and 32 items of instruction tlb,
	The TLB is normally allocated on a rotating basis. The oldest entry is always the next allocated */
#ifdef FLUSH_TLB
	mem_flush_tlb();
	save_sun5i_mem_status(SUSPEND_START |0x01);	
#ifdef PRE_DISABLE_MMU
	/* preload tlb for mem */
	//busy_waiting();
	//mem_preload_tlb_nommu(); //0x0000 mapping is not large enough for preload nommu tlb
						//eg: 0x01c2.... is not in the 0x0000,0000 range.
	mem_preload_tlb();
	save_sun5i_mem_status(SUSPEND_START |0x02);	
#else
	/* preload tlb for mem */
	mem_preload_tlb();
	save_sun5i_mem_status(SUSPEND_START |0x03);	
#endif

#endif	

	/* clear bss segment */
	do{*tmpPtr ++ = 0;}while(tmpPtr <= (char *)&__bss_end);
	save_sun5i_mem_status(SUSPEND_START |0x04);	
	/*get input para*/
	mem_memcpy((void *)&mem_para_info, (void *)(DRAM_BACKUP_BASE_ADDR1), sizeof(mem_para_info));
	
	/* initialise mem modules */
	mem_clk_init();
	save_sun5i_mem_status(SUSPEND_START |0x05);
	mem_int_init();
	save_sun5i_mem_status(SUSPEND_START |0x06);
	mem_tmr_init();
	save_sun5i_mem_status(SUSPEND_START |0x07);
	mem_twi_init(AXP_IICBUS);
	save_sun5i_mem_status(SUSPEND_START |0x08);

	//busy_waiting();
	while(mem_power_init(mem_para_info.axp_event)&&--retry){
		;
	}
	if(0 == retry){
		goto mem_power_init_err;
	}else{
		retry = RETRY_TIMES;
	}
    save_sun5i_mem_status(SUSPEND_START |0x09);
	

	/* dram enter self-refresh */
	//busy_waiting();
#ifdef DRAM_ENTER_SELFRESH
#ifdef GET_CYCLE_CNT
	start = get_cyclecount();
#endif
	if(dram_power_save_process()){
		goto suspend_dram_err;
	}
	//save_mem_status(SUSPEND_START |0x0c);
	
	/* gating off dram clock */
    mem_clk_dramgating(0);
#ifdef GET_CYCLE_CNT
	start = get_cyclecount() - start;
	//busy_waiting();
#endif
	save_mem_status(SUSPEND_START |0x0d);
#endif

#ifdef DISABLE_MMU
	if(suspend_with_nommu()){
		goto suspend_err;
	}
#else
	if(suspend_with_mmu()){
		goto suspend_err;
	}
#endif	
	//notice: never get here, so need watchdog, not busy_waiting.


suspend_err:
	init_DRAM();	
suspend_dram_err:
mem_power_init_err:
	
    while(mem_power_exit(mem_para_info.axp_event)&&--retry){
		;
	}
	if(0 == retry){
		return -1;
	}else{
		retry = RETRY_TIMES;
	}

	return -1;
}

__s32 suspend_with_nommu(void)
{
		disable_mmu();
		mem_flush_tlb();
		save_mem_status_nommu(SUSPEND_START |0x12);
		//after disable mmu, it is time to preload nommu, need to access dram?
		mem_preload_tlb_nommu();
		//while(1);
#ifdef SET_COPRO_DEFAULT
			set_copro_default();
			save_mem_status_nommu(SUSPEND_START |0x13);
			//busy_waiting();
			//fake_busy_waiting();
#endif
	
#ifdef JUMP_WITH_NOMMU
			save_mem_status_nommu(SUSPEND_START |0x14);
			//before jump, disable mmu
			//busy_waiting();
			//jump_to_resume0_nommu(0x40100000);
			jump_to_resume0(0x40100000);
#endif 
	
#ifdef MEM_POWER_OFF
			/*power off*/
			/*NOTICE: not support to power off yet after disable mmu.
			  * because twi use virtual addr. 
			  */
			while(mem_power_off_nommu()&&--retry){
				;
			}
			if(0 == retry){
				goto mem_power_off_nommu_err;
				
			}else{
				retry = RETRY_TIMES;
			}
			save_mem_status_nommu(SUSPEND_START |0x15);
#endif
			return 0;

mem_power_off_nommu_err:
	enable_mmu();
	return -1;

}

__s32 suspend_with_mmu(void)
{
	
#ifdef SET_COPRO_DEFAULT
		set_copro_default();
		save_mem_status(SUSPEND_START |0x13);
		//busy_waiting();
		//fake_busy_waiting();
#endif
	
	
#ifdef MEM_POWER_OFF
			/*power off*/
		//busy_waiting();
			while(mem_power_off()&&--retry){
				;
			}
			if(0 == retry){
				return -1;
			}else{
				retry = RETRY_TIMES;
			}			
			save_mem_status(SUSPEND_START |0x0f);
#endif
	
#ifdef WITH_MMU
		//busy_waiting();
		save_mem_status(SUSPEND_START |0x0f);
		//busy_waiting();
		jump_to_resume0(0xc0100000);
#endif

		return 0;

}

