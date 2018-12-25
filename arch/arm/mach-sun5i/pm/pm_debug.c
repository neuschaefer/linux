#include "pm_types.h"
#include "pm.h"

static __u32 cpu_freq = 0;
static __u32 overhead = 0;
static __u32 backup_perf_counter_ctrl_reg = 0;
static __u32 backup_perf_counter_enable_reg = 0;
#define CCU_REG_VA (0xF1c20000)
#define CCU_REG_PA (0x01c20000)

//for io-measure time
#define PORT_E_CONFIG (0xf1c20890)
#define PORT_E_DATA (0xf1c208a0)
#define PORT_CONFIG PORT_E_CONFIG
#define PORT_DATA PORT_E_DATA


void busy_waiting(void)
{
#if 1
	volatile __u32 loop_flag = 1;
	while(1 == loop_flag);
	
#endif
	return;
}

void fake_busy_waiting(void)
{
#if 1
	volatile __u32 loop_flag = 2;
	while(1 == loop_flag);
	
#endif
	return;
}

/*
 * notice: when resume, boot0 need to clear the flag, 
 * in case the data in dram be destoryed result in the system is re-resume in cycle.
*/
void save_mem_flag(void)
{
	__u32 saved_flag = *(volatile __u32 *)(PERMANENT_REG);
	saved_flag &= 0xfffffffe;
	saved_flag |= 0x00000001;
	*(volatile __u32 *)(PERMANENT_REG) = saved_flag;
	return;
}

void save_mem_status(volatile __u32 val)
{
	//*(volatile __u32 *)(STATUS_REG  + 0x04) = val;
	return;
}

__u32 get_mem_status(void)
{
	return *(volatile __u32 *)(STATUS_REG  + 0x04);
}

void save_mem_status_nommu(volatile __u32 val)
{
	//*(volatile __u32 *)(STATUS_REG_PA  + 0x04) = val;
	return;
}

__u32 save_sun5i_mem_status(volatile __u32 val)
{
	__u32 tmp;
	tmp = *(volatile __u32 *)(SUN5I_STATUS_REG);
	*(volatile __u32 *)(SUN5I_STATUS_REG) = val;
	return tmp;
}

__u32 save_sun5i_mem_status_nommu(volatile __u32 val)
{
	__u32 tmp;
	tmp = *(volatile __u32 *)(SUN5I_STATUS_REG_PA);
	*(volatile __u32 *)(SUN5I_STATUS_REG_PA) = val;
	return tmp;
}

__u32 get_cyclecount (void)
{
  __u32 value;
  // Read CCNT Register
  asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));  
  return value;
}

void backup_perfcounter(void)
{
	//backup performance-counter ctrl reg
	asm volatile ("MRC p15, 0, %0, c9, c12, 0\t\n": "=r"(backup_perf_counter_ctrl_reg)); 
	
	//backup enable reg
	asm volatile ("MRC p15, 0, %0, c9, c12, 1\t\n": "=r"(backup_perf_counter_enable_reg)); 

}
	
void init_perfcounters (__u32 do_reset, __u32 enable_divider)
{
	// in general enable all counters (including cycle counter)
	__u32 value = 1;

	// peform reset:
	if (do_reset)
	{
		value |= 2;     // reset all counters to zero.
		value |= 4;     // reset cycle counter to zero.
	}

	if (enable_divider)
		value |= 8;     // enable "by 64" divider for CCNT.

	value |= 16;

	// program the performance-counter control-register:
	asm volatile ("mcr p15, 0, %0, c9, c12, 0" : : "r"(value));

	// enable all counters:
	value = 0x8000000f;
	asm volatile ("mcr p15, 0, %0, c9, c12, 1" : : "r"(value));

	// clear overflows:
	asm volatile ("MCR p15, 0, %0, c9, c12, 3" : : "r"(value));

	return;
}

void restore_perfcounter(void)
{
	
	// restore performance-counter control-register:
	asm volatile ("mcr p15, 0, %0, c9, c12, 0" : : "r"(backup_perf_counter_ctrl_reg));

	// restore enable reg
	asm volatile ("mcr p15, 0, %0, c9, c12, 1" : : "r"(backup_perf_counter_enable_reg));
	
}

void reset_counter(void)
{
	__u32 value = 0;

	asm volatile ("mrc p15, 0, %0, c9, c12, 0" : : "r"(value));
	value |= 4;     // reset cycle counter to zero.
	// program the performance-counter control-register:
	//__asm {MCR p15, 0, value, c9, c12, 0}
	asm volatile ("mcr p15, 0, %0, c9, c12, 0" : : "r"(value));
}

void change_runtime_env(__u32 mmu_flag)
{
	__u32 factor_n = 0;
	__u32 factor_k = 0;
	__u32 factor_m = 0;
	__u32 factor_p = 0;
	__u32 start = 0;
	__u32 cmu_reg = 0;
	volatile __u32 reg_val = 0;

	if(mmu_flag){
		cmu_reg = CCU_REG_VA;
	}else{
		cmu_reg = CCU_REG_PA;
	}
	//init counters:
	//init_perfcounters (1, 0);
	// measure the counting overhead:
	start = get_cyclecount();
	overhead = get_cyclecount() - start;
	//busy_waiting();
	//get runtime freq: clk src + divider ratio
	//src selection
	reg_val = *(volatile int *)(cmu_reg + 0x54);
	reg_val >>= 16;
	reg_val &= 0x3;
	if(0 == reg_val){
		//32khz osc
		cpu_freq = 32;
		
	}else if(1 == reg_val){
		//hosc, 24Mhz
		cpu_freq = 24000; 			//unit is khz
	}else if(2 == reg_val){
		//get pll_factor
		reg_val = *(volatile int *)(cmu_reg + 0x00);
		factor_p = 0x3 & (reg_val >> 16);
		factor_p = 1 << factor_p;		//1/2/4/8
		factor_n = 0x1f & (reg_val >> 8); 	//the range is 0-31
		factor_k = (0x3 & (reg_val >> 4)) + 1; 	//the range is 1-4
		factor_m = (0x3 & (reg_val >> 0)) + 1; 	//the range is 1-4
		
		//cpu_freq = (24000*factor_n*factor_k)/(factor_p*factor_m);
		cpu_freq = raw_lib_udiv(24000*factor_n*factor_k, factor_p*factor_m);
		//msg("cpu_freq = dec(%d). \n", cpu_freq);
		//busy_waiting();
	}
	
}

/*
 * input para range: 1-1000 us, so the max us_cnt equal = 1008*1000;
 */	
void delay_us(__u32 us)
{
	__u32 us_cnt = 0;
	__u32 cur = 0;
	__u32 target = 0;
	//__u32 cnt = 0;


	if(cpu_freq > 1000){
		us_cnt = ((raw_lib_udiv(cpu_freq, 1000)) + 1)*us;
	}else{
		//32 <--> 32k, 1cycle = 1s/32k =32us 
		return;
	}
	
	cur = get_cyclecount();
	target = cur - overhead + us_cnt;

#if 1
	while(!counter_after_eq(cur, target)){
		cur = get_cyclecount();
		//cnt++;
	}
#endif
	

#if 0
	__s32 s_cur = 0;
	__s32 s_target = 0;
	__s32 result = 0;

	s_cur = (__s32)(cur);
	s_target = (__s32)(target);
	result = s_cur - s_target;
	if(s_cur - s_target >= 0){
		cnt++;
	}
	while((typecheck(__u32, cur) && \
			typecheck(__u32, target) && \
			((__s32)(cur) - (__s32)(target) >= 0))){
		
			s_cur = (__s32)(cur);
			s_target = (__s32)(target);
			if(s_cur - s_target >= 0){
				cnt++;				
			}
			cur = get_cyclecount();
	}
#endif
	//busy_waiting();

	
	return;
}

void delay_ms(__u32 ms)
{
	delay_us(ms*1000);
	
	return;
}

/*
 * notice: dependant with perf counter to delay.
 */
void io_init(void)
{
	//config port output
	*(volatile unsigned int *)(PORT_CONFIG)  = 0x111111;
	
	return;
}

void io_init_high(void)
{
	__u32 data;
	
	//set port to high
	data = *(volatile unsigned int *)(PORT_DATA);
	data |= 0x3f;
	*(volatile unsigned int *)(PORT_DATA) = data;

	return;
}

void io_init_low(void)
{
	__u32 data;

	data = *(volatile unsigned int *)(PORT_DATA);
	//set port to low
	data &= 0xffffffc0;
	*(volatile unsigned int *)(PORT_DATA) = data;

	return;
}

/*
 * set pa port to high, num range is 0-7;	
 */
void io_high(int num)
{
	__u32 data;
	data = *(volatile unsigned int *)(PORT_DATA);
	//pull low 10ms
	data &= (~(1<<num));
	*(volatile unsigned int *)(PORT_DATA) = data;
	delay_us(10000);
	//pull high
	data |= (1<<num);
	*(volatile unsigned int *)(PORT_DATA) = data;

	return;
}



