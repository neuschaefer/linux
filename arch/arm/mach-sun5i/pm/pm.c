/*
*********************************************************************************************************
*                                                    LINUX-KERNEL
*                                        newbie Linux Platform Develop Kits
*                                                   Kernel Module
*
*                                    (c) Copyright 2006-2011, kevin.z China
*                                             All Rights Reserved
*
* File    : pm.c
* By      : kevin.z
* Version : v1.0
* Date    : 2011-5-27 14:08
* Descript: power manager for newbies chips platform.
* Update  : date                auther      ver     notes
*********************************************************************************************************
*/

#include <linux/module.h>
#include <linux/suspend.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/major.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/delay.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/tlbflush.h>
#include <linux/power/aw_pm.h>
#include <asm/mach/map.h>
#include <asm/cacheflush.h>
#include "pm_i.h"

#include <mach/sys_config.h>
#include <mach/system.h>

//#define CROSS_MAPPING_STANDBY
//#define CHECK_INT_SRC

#define AW_PM_DBG   1
#undef PM_DBG
#if(AW_PM_DBG)
    #define PM_DBG(format,args...)   printk("[pm]"format,##args)
#else
    #define PM_DBG(format,args...)   do{}while(0)
#endif

#ifdef RETURN_FROM_RESUME0_WITH_NOMMU
#define PRE_DISABLE_MMU    //actually, mean ,prepare condition to disable mmu
#endif 

#ifdef ENTER_SUPER_STANDBY
#undef PRE_DISABLE_MMU
#endif

#ifdef ENTER_SUPER_STANDBY_WITH_NOMMU
#define PRE_DISABLE_MMU    //actually, mean ,prepare condition to disable mmu
#endif

#ifdef RETURN_FROM_RESUME0_WITH_MMU
#undef PRE_DISABLE_MMU
#endif

#ifdef WATCH_DOG_RESET
#define PRE_DISABLE_MMU    //actually, mean ,prepare condition to disable mmu
#endif 

//#define VERIFY_RESTORE_STATUS

/* define major number for power manager */
#define AW_PMU_MAJOR    267


extern char *standby_bin_start;
extern char *standby_bin_end;
extern char *suspend_bin_start;
extern char *suspend_bin_end;
extern char *resume0_bin_start;
extern char *resume0_bin_end;

extern int mem_arch_suspend(void);
extern int mem_arch_resume(void);
extern asmlinkage int mem_clear_runtime_context(void);
extern asmlinkage int mem_restore_runtime_context(void);
extern asmlinkage int mem_save_runtime_context(void);

extern void disable_cache(void);
extern void disable_program_flow_prediction(void);
extern void invalidate_branch_predictor(void);
extern void enable_cache(void);
extern void enable_program_flow_prediction(void);
extern void disable_dcache(void);
extern void disable_l2cache(void);
extern void set_ttbr0(void);
typedef  int (*suspend_func)(void);
void jump_to_suspend(__u32 ttbr1, suspend_func p);
extern void clear_reg_context(void);
extern void invalidate_dcache(void);

int (*mem)(void) = 0;

#if 0
//pre-allocate an area for saving dram training area
__u32 mem_dram_traning_area_back[DRAM_TRANING_SIZE];
__u32 mem_dram_backup_area[DRAM_BACKUP_SIZE];
__u32 mem_dram_backup_area1[DRAM_BACKUP_SIZE1];
__u32 mem_dram_backup_area2[DRAM_BACKUP_SIZE2]; //for training area
__u32 mem_dram_backup_compare_area2[DRAM_COMPARE_SIZE]; //for compare area
#else
//static __u32 *mem_dram_traning_area_back = NULL;
static __u32 *mem_dram_backup_area = NULL;
static __u32 *mem_dram_backup_area1 = NULL;
static __u32 *mem_dram_backup_area2 = NULL;
#endif

#ifdef CONFIG_CPU_FREQ_USR_EVNT_NOTIFY
extern void cpufreq_user_event_notify(void);
#endif

static struct map_desc mem_sram_md = { 
	.virtual = MEM_SW_VA_SRAM_BASE,         
	.pfn = __phys_to_pfn(MEM_SW_PA_SRAM_BASE),         
	.length = SZ_1M, 
	.type = MT_MEMORY_ITCM,  
 	};

static struct aw_pm_info standby_info = {
    .standby_para = {
		.event = SUSPEND_WAKEUP_SRC_EXINT,
		.axp_event = AXP_MEM_WAKEUP,
    },
    .pmu_arg = {
        .twi_port = 0,
        .dev_addr = 10,
    },
};

static struct int_state saved_int_state;
static struct clk_state saved_clk_state;
static struct tmr_state saved_tmr_state;
static struct twi_state saved_twi_state;
static struct gpio_state saved_gpio_state;
static struct sram_state saved_sram_state;

#ifdef GET_CYCLE_CNT
static int start = 0;
static int resume0_period = 0;
static int resume1_period = 0;

static int pm_start = 0;
static int invalidate_data_time = 0;
static int invalidate_instruct_time = 0;
static int before_restore_processor = 0;
static int after_restore_process = 0;
//static int restore_runtime_peroid = 0;

//late_resume timing
static int late_resume_start = 0;
static int backup_area_start = 0;
static int backup_area1_start = 0;
static int backup_area2_start = 0;
static int clk_restore_start = 0;
static int gpio_restore_start = 0;
static int twi_restore_start = 0;
static int int_restore_start = 0;
static int tmr_restore_start = 0;	
static int sram_restore_start = 0;
static int late_resume_end = 0;
#endif

struct aw_mem_para mem_para_info;
standby_type_e standby_type = NON_STANDBY;
EXPORT_SYMBOL(standby_type);
standby_level_e standby_level = STANDBY_INITIAL;
EXPORT_SYMBOL(standby_level);
struct standby_output_t standby_output;
EXPORT_SYMBOL(standby_output);
__u32 standby_wakeup_event = 0;
EXPORT_SYMBOL(standby_wakeup_event);
suspend_state_t g_suspend_state = PM_SUSPEND_MEM;
EXPORT_SYMBOL(g_suspend_state);

//static volatile int enter_flag = 0;
volatile int print_flag = 0;
static bool mem_allocated_flag = false;
static int dram_backup = 0;
static int standby_mode = 0;
static int suspend_status_flag = 0;

extern void create_mapping(struct map_desc *md);
extern void save_mapping(unsigned long vaddr);
extern void restore_mapping(unsigned long vaddr);
extern void save_mmu_state(struct mmu_state *saved_mmu_state);
extern void clear_mem_flag(void);
extern void save_runtime_context(__u32 *addr);
void restore_processor_ttbr0(void);
extern void flush_icache(void);
extern void flush_dcache(void);

#ifdef CHECK_INT_SRC
static void check_int_src(void)
{
#define INT_REG_0 (0x10) 
#define INT_REG_1 (0x14)
#define INT_REG_2 (0x18)


	u32 data_0 = 0;
	u32 data_1 = 0;
	u32 data_2 = 0;
	
	data_0 = *(volatile unsigned int *)(SW_VA_INT_IO_BASE + INT_REG_0);
	data_1 = *(volatile unsigned int *)(SW_VA_INT_IO_BASE + INT_REG_1);
	data_2 = *(volatile unsigned int *)(SW_VA_INT_IO_BASE + INT_REG_2);

	pr_info("INT_REG_0 = %d \n", data_0);
	pr_info("INT_REG_1 = %d \n", data_1);
	pr_info("INT_REG_2 = %d \n", data_2);

	return;

}
#endif


/*
*********************************************************************************************************
*                           aw_pm_valid
*
*Description: determine if given system sleep state is supported by the platform;
*
*Arguments  : state     suspend state;
*
*Return     : if the state is valid, return 1, else return 0;
*
*Notes      : this is a call-back function, registered into PM core;
*
*********************************************************************************************************
*/
static int aw_pm_valid(suspend_state_t state)
{
#ifdef CHECK_IC_VERSION
	enum sw_ic_ver version = MAGIC_VER_NULL;
#endif

    PM_DBG("valid\n");
    if(!((state > PM_SUSPEND_ON) && (state < PM_SUSPEND_MAX))){
        PM_DBG("state (%d) invalid!\n", state);
        return 0;
    }
    g_suspend_state = state;

#ifdef CHECK_IC_VERSION
	if(1 == standby_mode){
			version = sw_get_ic_ver();
			if(!(MAGIC_VER_A13B == version || MAGIC_VER_A12B == version || MAGIC_VER_A10SB == version)){
				pr_info("ic version: %d not support super standby. \n", version);
				standby_mode = 0;
			}
	}
#endif
		
	//if 1 == standby_mode, actually, mean mem corresponding with super standby 
	if(PM_SUSPEND_STANDBY == state){
		if(1 == standby_mode){
			standby_type = NORMAL_STANDBY;
		}else{
			standby_type = SUPER_STANDBY;
		}
	}else if(PM_SUSPEND_MEM == state || PM_SUSPEND_BOOTFAST == state){
		if(1 == standby_mode){
			standby_type = SUPER_STANDBY;
		}else{
			standby_type = NORMAL_STANDBY;
		}
	}else if (PM_SUSPEND_PARTIAL == state){
        standby_type = NORMAL_STANDBY;
	}
	
	//allocat space for backup dram data
	if((false == mem_allocated_flag) && (SUPER_STANDBY == standby_type)){
		mem_dram_backup_area = (__u32*)kmalloc(sizeof(__u32)*DRAM_BACKUP_SIZE, GFP_KERNEL);
		if(!mem_dram_backup_area){
			goto malloc_mem_dram_backup_area_err;
		}
		
		mem_dram_backup_area1 = (__u32*)kmalloc(sizeof(__u32)*DRAM_BACKUP_SIZE1, GFP_KERNEL);
		if(!mem_dram_backup_area1){
			goto malloc_mem_dram_backup_area1_err;
		}
		
		mem_dram_backup_area2 = (__u32*)kmalloc(sizeof(__u32)*DRAM_BACKUP_SIZE2, GFP_KERNEL);
		if(!mem_dram_backup_area2){
			goto malloc_mem_dram_backup_area2_err;
		}
		
		mem_allocated_flag = true;	
#ifdef GET_CYCLE_CNT
	// init counters:
	init_perfcounters (1, 0);
#endif
	}

	//print_flag = 0;
    return 1;
	
malloc_mem_dram_backup_area2_err:
	kfree(mem_dram_backup_area1);
malloc_mem_dram_backup_area1_err:
	kfree(mem_dram_backup_area);
malloc_mem_dram_backup_area_err:
	mem_allocated_flag = false;
	
	return 0;

}


/*
*********************************************************************************************************
*                           aw_pm_begin
*
*Description: Initialise a transition to given system sleep state;
*
*Arguments  : state     suspend state;
*
*Return     : return 0 for process successed;
*
*Notes      : this is a call-back function, registered into PM core, and this function
*             will be called before devices suspened;
*********************************************************************************************************
*/
int aw_pm_begin(suspend_state_t state)
{
    PM_DBG("%d state begin\n", state);

	//set freq max
#ifdef CONFIG_CPU_FREQ_USR_EVNT_NOTIFY
	cpufreq_user_event_notify();
#endif
	
/*must init perfcounter, because delay_us and delay_ms is depandant perf counter*/
#ifndef GET_CYCLE_CNT
		backup_perfcounter();
		init_perfcounters (1, 0);
#endif

	return 0;
}


/*
*********************************************************************************************************
*                           aw_pm_prepare
*
*Description: Prepare the platform for entering the system sleep state.
*
*Arguments  : none;
*
*Return     : return 0 for process successed, and negative code for error;
*
*Notes      : this is a call-back function, registered into PM core, this function
*             will be called after devices suspended, and before device late suspend
*             call-back functions;
*********************************************************************************************************
*/
int aw_pm_prepare(void)
{
    PM_DBG("prepare\n");

    return 0;
}


/*
*********************************************************************************************************
*                           aw_pm_prepare_late
*
*Description: Finish preparing the platform for entering the system sleep state.
*
*Arguments  : none;
*
*Return     : return 0 for process successed, and negative code for error;
*
*Notes      : this is a call-back function, registered into PM core.
*             prepare_late is called before disabling nonboot CPUs and after
*              device drivers' late suspend callbacks have been executed;
*********************************************************************************************************
*/
int aw_pm_prepare_late(void)
{
    PM_DBG("prepare_late\n");

    return 0;
}

/*
*********************************************************************************************************
*                           aw_early_suspend
*
*Description: prepare necessary info for suspend&resume;
*
*Return     : return 0 is process successed;
*
*Notes      : -1: data is ok;
*			-2: data has been destory.
*********************************************************************************************************
*/
static int aw_early_suspend(void)
{
#define MAX_RETRY_TIMES (5)

	__s32 retry = MAX_RETRY_TIMES;
	
	//backup device state
	mem_ccu_save((__ccmu_reg_list_t *)(SW_VA_CCM_IO_BASE));
	mem_clk_save(&(saved_clk_state));
	mem_gpio_save(&(saved_gpio_state));
	mem_tmr_save(&(saved_tmr_state));
	mem_twi_save(&(saved_twi_state));
	mem_int_save(&(saved_int_state));
	mem_sram_save(&(saved_sram_state));

	//backup volt and freq state, after backup device state
	mem_twi_init(AXP_IICBUS);
	/* backup voltages */
	while(-1 == (mem_para_info.suspend_dcdc2 = mem_get_voltage(POWER_VOL_DCDC2)) && --retry){
		;
	}
	if(0 == retry){
		return -1;
	}else{
		retry = MAX_RETRY_TIMES;
	}	

	while(-1 == (mem_para_info.suspend_dcdc3 = mem_get_voltage(POWER_VOL_DCDC3)) && --retry){
		;
	}
	if(0 == retry){
		return -1;
	}else{
		retry = MAX_RETRY_TIMES;
	}	

	/*backup bus ratio*/
	mem_clk_getdiv(&mem_para_info.clk_div);
	/*backup pll ratio*/
	mem_clk_get_pll_factor(&mem_para_info.pll_factor);
	
	//backup mmu
	save_mmu_state(&(mem_para_info.saved_mmu_state));
	//backup cpu state
	__save_processor_state(&(mem_para_info.saved_cpu_context));
	//backup 0x0000,0000 page entry, size?
	save_mapping(MEM_SW_VA_SRAM_BASE);

	//backup dram area to leave space for resume0 code
	memcpy((void *)mem_dram_backup_area, (void *)DRAM_BACKUP_BASE_ADDR, sizeof(__u32)*DRAM_BACKUP_SIZE);
	dmac_flush_range((void *)mem_dram_backup_area, (void *)(mem_dram_backup_area + (sizeof(u32)) * DRAM_BACKUP_SIZE - 1));
	
	//backup dram area to reserve space for para space
	memcpy((void *)mem_dram_backup_area1, (void *)DRAM_BACKUP_BASE_ADDR1, sizeof(__u32)*DRAM_BACKUP_SIZE1);
	dmac_flush_range((void *)mem_dram_backup_area1, (void *)(mem_dram_backup_area1 + (sizeof(u32)) * DRAM_BACKUP_SIZE1 - 1));

	memcpy((void *)mem_dram_backup_area2, (void *)DRAM_BACKUP_BASE_ADDR2, sizeof(__u32)*DRAM_BACKUP_SIZE2);
	dmac_flush_range((void *)mem_dram_backup_area2, (void *)(mem_dram_backup_area2 + (sizeof(u32)) * DRAM_BACKUP_SIZE2 - 1));

	//prepare resume0 code for resume
	if((sizeof(__u32)*DRAM_BACKUP_SIZE) < ((int)&resume0_bin_end - (int)&resume0_bin_start) ){
		//judge the reserved space for resume0 is enough or not.
		return -1;
	}
	if((sizeof(__u32)*DRAM_BACKUP_SIZE1) < sizeof(mem_para_info)){
		//judge the reserved space for mem para is enough or not.
		return -1;

	}

	memcpy((void *)DRAM_BACKUP_BASE_ADDR, (void *)&resume0_bin_start, (int)&resume0_bin_end - (int)&resume0_bin_start);
	dmac_flush_range((void *)DRAM_BACKUP_BASE_ADDR, (void *)(DRAM_BACKUP_BASE_ADDR + (sizeof(u32)) * DRAM_BACKUP_SIZE -1) );
	
	//clean all the data into dram
	memcpy((void *)DRAM_BACKUP_BASE_ADDR1, (void *)&mem_para_info, sizeof(mem_para_info));
	dmac_flush_range((void *)DRAM_BACKUP_BASE_ADDR1, (void *)(DRAM_BACKUP_BASE_ADDR1 + (sizeof(u32)) * DRAM_BACKUP_SIZE1 - 1));

	//prepare dram training area data
	memcpy((void *)DRAM_BACKUP_BASE_ADDR2, (void *)DRAM_BASE_ADDR, sizeof(__u32)*DRAM_TRANING_SIZE);
	dmac_flush_range((void *)DRAM_BACKUP_BASE_ADDR2, (void *)(DRAM_BACKUP_BASE_ADDR2 + (sizeof(u32)) * DRAM_BACKUP_SIZE2 - 1));
	
	mem_arch_suspend();
	save_processor_state(); 
	
	//before creating mapping, build the coherent between cache and memory
	//clean and flush
	__cpuc_flush_kern_all();
	__cpuc_flush_user_all();

	__cpuc_coherent_user_range(0x00000000, 0xc0000000-1);
	__cpuc_coherent_kern_range(0xc0000000, 0xffffffff-1);

	//create 0x0000,0000 mapping table: 0x0000,0000 -> 0x0000,0000 
	create_mapping(&mem_sram_md);
	
#ifdef PRE_DISABLE_MMU
	//jump to sram: dram enter selfresh, and power off.
	mem = (int (*)(void))SRAM_FUNC_START_PA;
	//move standby code to sram
	memcpy((void *)SRAM_FUNC_START, (void *)&suspend_bin_start, (int)&suspend_bin_end - (int)&suspend_bin_start);	
#else
	//jump to sram: dram enter selfresh, and power off.
	mem = (int (*)(void))SRAM_FUNC_START;
	//move standby code to sram
	memcpy((void *)SRAM_FUNC_START, (void *)&suspend_bin_start, (int)&suspend_bin_end - (int)&suspend_bin_start);
#endif

	
#ifdef PRE_DISABLE_MMU
	//enable the mapping and jump
	//invalidate tlb? maybe, but now, at this situation,  0x0000 <--> 0x0000 mapping never stay in tlb before this.
	jump_to_suspend(mem_para_info.saved_cpu_context.ttb_1r, mem);
#else
	mem();
#endif

	return -2;

}

/*
*********************************************************************************************************
*                           verify_restore
*
*Description: verify src and dest region is the same;
*
*Return     : 0: same;
*                -1: different;
*
*Notes      : 
*********************************************************************************************************
*/
#ifdef VERIFY_RESTORE_STATUS
static int verify_restore(void *src, void *dest, int count)
{
	volatile char *s = (volatile char *)src;
	volatile char *d = (volatile char *)dest;
	
	while(count--){
		if(*(s+(count)) != *(d+(count))){
			//busy_waiting();
			return -1;
		}
	}

	return 0;
}
#endif

/*
*********************************************************************************************************
*                           aw_late_resume
*
*Description: prepare necessary info for suspend&resume;
*
*Return     : return 0 is process successed;
*
*Notes      : 
*********************************************************************************************************
*/
static void aw_late_resume(void)
{

#ifdef VERIFY_RESTORE_STATUS
	int ret = 0;
#endif
	
	memcpy((void *)&mem_para_info, (void *)(DRAM_BACKUP_BASE_ADDR1), sizeof(mem_para_info));
	mem_para_info.mem_flag = 0;
	
	//restore dram backup area1
	memcpy((void *)DRAM_BACKUP_BASE_ADDR2, (void *)mem_dram_backup_area2, sizeof(__u32)*DRAM_BACKUP_SIZE2);
	dmac_flush_range((void *)DRAM_BACKUP_BASE_ADDR2, (void *)(DRAM_BACKUP_BASE_ADDR2 + (sizeof(u32)) * DRAM_BACKUP_SIZE2 - 1));
	
	memcpy((void *)DRAM_BACKUP_BASE_ADDR1, (void *)mem_dram_backup_area1, sizeof(__u32)*DRAM_BACKUP_SIZE1);
	dmac_flush_range((void *)DRAM_BACKUP_BASE_ADDR1, (void *)(DRAM_BACKUP_BASE_ADDR1 + (sizeof(u32)) * DRAM_BACKUP_SIZE1 - 1));
	
	//restore dram backup area
	memcpy((void *)DRAM_BACKUP_BASE_ADDR, (void *)mem_dram_backup_area, sizeof(__u32)*DRAM_BACKUP_SIZE);
	dmac_flush_range((void *)DRAM_BACKUP_BASE_ADDR, (void *)(DRAM_BACKUP_BASE_ADDR + (sizeof(u32)) * DRAM_BACKUP_SIZE -1) );

	//busy_waiting();
	dram_backup = save_sun5i_mem_status(LATE_RESUME_START | 0x27);
	save_sun5i_mem_status(dram_backup);
#ifdef VERIFY_RESTORE_STATUS
	if(0 != (ret = (verify_restore((void *)mem_dram_backup_area2, (void *)DRAM_BACKUP_BASE_ADDR2, sizeof(__u32)*DRAM_BACKUP_SIZE2)))){
		save_mem_status(LATE_RESUME_START |0x21);
		save_sun5i_mem_status(LATE_RESUME_START | 0x31);
		//busy_waiting();
	}

	if(0 != (ret = verify_restore((void *)mem_dram_backup_area1, (void *)DRAM_BACKUP_BASE_ADDR1, sizeof(__u32)*DRAM_BACKUP_SIZE1))){
		save_mem_status(LATE_RESUME_START |0x22);
		save_sun5i_mem_status(LATE_RESUME_START | 0x32);
		//busy_waiting();
	}
	if(0 != (ret = verify_restore((void *)mem_dram_backup_area, (void *)DRAM_BACKUP_BASE_ADDR, sizeof(__u32)*DRAM_BACKUP_SIZE))){
		save_mem_status(LATE_RESUME_START |0x23);
		save_sun5i_mem_status(LATE_RESUME_START | 0x33);
		//busy_waiting();
	}
#endif

	//restore device state
	mem_clk_restore(&(saved_clk_state));
	mem_gpio_restore(&(saved_gpio_state));
	mem_twi_restore(&(saved_twi_state));
	mem_tmr_restore(&(saved_tmr_state));
	mem_int_restore(&(saved_int_state));
	mem_sram_restore(&(saved_sram_state));
	mem_ccu_restore((__ccmu_reg_list_t *)(SW_VA_CCM_IO_BASE));

	return;
}

/*
*********************************************************************************************************
*                           aw_early_suspend
*
*Description: prepare necessary info for suspend&resume;
*
*Return     : return 0 is process successed;
*
*Notes      : 
*********************************************************************************************************
*/
static int aw_super_standby(suspend_state_t state)
{
	int result = 0;
	suspend_status_flag = 0;
	
mem_enter:
	if( 1 == mem_para_info.mem_flag){
		invalidate_branch_predictor();
		//must be called to invalidate I-cache inner shareable?
		// I+BTB cache invalidate
		__cpuc_flush_icache_all();
		//disable 0x0000 <---> 0x0000 mapping
		restore_processor_state();
		//destroy 0x0000 <---> 0x0000 mapping
		restore_mapping(MEM_SW_VA_SRAM_BASE);
		mem_arch_resume();
		goto resume;
	}

	save_runtime_context(mem_para_info.saved_runtime_context_svc);
	mem_para_info.mem_flag = 1;
	standby_level = STANDBY_WITH_POWER_OFF;
	mem_para_info.resume_pointer = (void *)&&mem_enter;
	//busy_waiting();
	pr_info("resume_pointer = 0x%x. \n", (unsigned int)(mem_para_info.resume_pointer));
	
#if 1
	/* config system wakeup evetn type */
	if(PM_SUSPEND_MEM == state || PM_SUSPEND_STANDBY == state){
		mem_para_info.axp_event = AXP_MEM_WAKEUP;
	}else if(PM_SUSPEND_BOOTFAST == state){
		mem_para_info.axp_event = AXP_BOOTFAST_WAKEUP;
	}
#endif	

	result = aw_early_suspend();
	if(-2 == result){
		//mem_para_info.mem_flag = 1;
		//busy_waiting();
		suspend_status_flag = 2;
		goto mem_enter;
	}else if(-1 == result){
		suspend_status_flag = 1;
		goto suspend_err;
	}
	
resume:
	aw_late_resume();
	save_sun5i_mem_status(dram_backup);
	//have been disable dcache in resume1
	enable_cache();
	
suspend_err:
	pr_info("suspend_status_flag = %d. \n", suspend_status_flag);

	return 0;

}

/*
*********************************************************************************************************
*                           aw_pm_enter
*
*Description: Enter the system sleep state;
*
*Arguments  : state     system sleep state;
*
*Return     : return 0 is process successed;
*
*Notes      : this function is the core function for platform sleep.
*********************************************************************************************************
*/
static int aw_pm_enter(suspend_state_t state)
{
	asm volatile ("stmfd sp!, {r1-r12, lr}" );
	int (*standby)(struct aw_pm_info *arg) = 0;
	
	PM_DBG("enter state %d, standby_type =%d\n", state, standby_type);     
	if(NORMAL_STANDBY== standby_type){
		standby = (int (*)(struct aw_pm_info *arg))SRAM_FUNC_START;
		//move standby code to sram
		memcpy((void *)SRAM_FUNC_START, (void *)&standby_bin_start, (int)&standby_bin_end - (int)&standby_bin_start);
		/* config system wakeup evetn type */
		if(PM_SUSPEND_MEM == state || PM_SUSPEND_STANDBY == state || PM_SUSPEND_PARTIAL == state){
			standby_info.standby_para.axp_event = AXP_MEM_WAKEUP;
		}else if(PM_SUSPEND_BOOTFAST == state){
			standby_info.standby_para.axp_event = AXP_BOOTFAST_WAKEUP;
		}
		standby_info.standby_para.event = (SUSPEND_WAKEUP_SRC_EXINT | SUSPEND_WAKEUP_SRC_ALARM);
        standby_info.standby_para.event |= standby_wakeup_event;
        standby_wakeup_event = 0;
        #ifndef  CONFIG_USB_SW_PERIPHERAL_REMOTE
		//standby_info.standby_para.event &= ~SUSPEND_WAKEUP_SRC_USB;
        #endif
		/* goto sram and run */
		standby(&standby_info);
        standby_output.event = standby_info.standby_para.event;
        standby_output.ir_data_cnt = standby_info.standby_para.ir_data_cnt;
        if (standby_info.standby_para.ir_data_cnt != 0)
        {
            memcpy(standby_output.ir_buffer, standby_info.standby_para.ir_buffer, STANDBY_IR_BUF_SIZE);
        }
        standby_output.pio_data_cnt = standby_info.standby_para.pio_data_cnt;
        if (standby_info.standby_para.pio_data_cnt != 0)
        {
            memcpy(standby_output.pio_buffer, standby_info.standby_para.pio_buffer, STANDBY_PIO_BUF_SIZE);
        }

#ifdef CHECK_INT_SRC
		check_int_src();
#endif
	}else if(SUPER_STANDBY == standby_type){
			aw_super_standby(state);	
	}

	asm volatile ("ldmfd sp!, {r1-r12, lr}" );
	return 0;
}


/*
*********************************************************************************************************
*                           aw_pm_wake
*
*Description: platform wakeup;
*
*Arguments  : none;
*
*Return     : none;
*
*Notes      : This function called when the system has just left a sleep state, right after
*             the nonboot CPUs have been enabled and before device drivers' early resume
*             callbacks are executed. This function is opposited to the aw_pm_prepare_late;
*********************************************************************************************************
*/
static void aw_pm_wake(void)
{
    PM_DBG("platform wakeup, wakesource is:0x%x\n", standby_info.standby_para.event);
}

/*
*********************************************************************************************************
*                           aw_pm_finish
*
*Description: Finish wake-up of the platform;
*
*Arguments  : none
*
*Return     : none
*
*Notes      : This function is called right prior to calling device drivers' regular suspend
*              callbacks. This function is opposited to the aw_pm_prepare function.
*********************************************************************************************************
*/
void aw_pm_finish(void)
{
    PM_DBG("platform wakeup finish\n");
}


/*
*********************************************************************************************************
*                           aw_pm_end
*
*Description: Notify the platform that system is in work mode now.
*
*Arguments  : none
*
*Return     : none
*
*Notes      : This function is called by the PM core right after resuming devices, to indicate to
*             the platform that the system has returned to the working state or
*             the transition to the sleep state has been aborted. This function is opposited to
*             aw_pm_begin function.
*********************************************************************************************************
*/
void aw_pm_end(void)
{
	
	//standby_type = NON_STANDBY;
	//uart_init(2, 115200);
	//save_mem_status(LATE_RESUME_START |0x10);
	//print_flag = 0;
#ifndef GET_CYCLE_CNT
	#ifndef IO_MEASURE
			restore_perfcounter();
	#endif
#endif

#ifdef IO_MEASURE
	io_init();
	io_high(1);
#endif

	PM_DBG("aw_pm_end!\n");
}


/*
*********************************************************************************************************
*                           aw_pm_recover
*
*Description: Recover platform from a suspend failure;
*
*Arguments  : none
*
*Return     : none
*
*Notes      : This function alled by the PM core if the suspending of devices fails.
*             This callback is optional and should only be implemented by platforms
*             which require special recovery actions in that situation.
*********************************************************************************************************
*/
void aw_pm_recover(void)
{
    PM_DBG("aw_pm_recover\n");
}


/*
    define platform_suspend_ops which is registered into PM core.
*/
static struct platform_suspend_ops aw_pm_ops = {
    .valid = aw_pm_valid,
    .begin = aw_pm_begin,
    .prepare = aw_pm_prepare,
    .prepare_late = aw_pm_prepare_late,
    .enter = aw_pm_enter,
    .wake = aw_pm_wake,
    .finish = aw_pm_finish,
    .end = aw_pm_end,
    .recover = aw_pm_recover,
};


/*
*********************************************************************************************************
*                           aw_pm_init
*
*Description: initial pm sub-system for platform;
*
*Arguments  : none;
*
*Return     : result;
*
*Notes      :
*
*********************************************************************************************************
*/
static int __init aw_pm_init(void)
{
    PM_DBG("aw_pm_init!\n");
	
	if(SCRIPT_PARSER_OK != script_parser_fetch("pm_para", "standby_mode", &standby_mode, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		standby_mode = 0;
	}else{
		pr_info("standby_mode = %d. \n", standby_mode);
		if(1 != standby_mode){
			pr_err("%s: not support super standby. \n",  __func__);
		}
	}

    suspend_set_ops(&aw_pm_ops);
	
    return 0;
}


/*
*********************************************************************************************************
*                           aw_pm_exit
*
*Description: exit pm sub-system on platform;
*
*Arguments  : none
*
*Return     : none
*
*Notes      :
*
*********************************************************************************************************
*/
static void __exit aw_pm_exit(void)
{
	PM_DBG("aw_pm_exit!\n");
	if(true == mem_allocated_flag){
		kfree(mem_dram_backup_area);
		kfree(mem_dram_backup_area1);
		kfree(mem_dram_backup_area2);
		mem_allocated_flag = false;
	}
	suspend_set_ops(NULL);
}

module_init(aw_pm_init);
module_exit(aw_pm_exit);

