/****************************************************************************
*
* Copyright 2010 --2011 Broadcom Corporation.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
*****************************************************************************/

#include <linux/sched.h>
#include <linux/cpuidle.h>
#include <linux/pm.h>
#include <linux/delay.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/smp.h>
#include <linux/cpumask.h>
#include <plat/kona_pm.h>
#include <plat/pwr_mgr.h>
#include <plat/pi_mgr.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <mach/io_map.h>
#include <plat/clock.h>
#include <linux/err.h>
#include <linux/debugfs.h>
#include <mach/rdb/brcm_rdb_scu.h>
#include <mach/rdb/brcm_rdb_csr.h>
#include <mach/rdb/brcm_rdb_chipreg.h>
#include <mach/rdb/brcm_rdb_root_clk_mgr_reg.h>
#include <mach/rdb/brcm_rdb_kps_clk_mgr_reg.h>
#include <mach/rdb/brcm_rdb_kpm_clk_mgr_reg.h>
#include <mach/rdb/brcm_rdb_khub_clk_mgr_reg.h>
#include <mach/rdb/brcm_rdb_esub_clk_mgr_reg.h>
#include <mach/rdb/brcm_rdb_hsotg_ctrl.h>
#include <mach/rdb/brcm_rdb_kproc_clk_mgr_reg.h>
#include <mach/rdb/brcm_rdb_gicdist.h>
#include <mach/rdb/brcm_rdb_pwrmgr.h>
#include <mach/rdb/brcm_rdb_kona_gptimer.h>
#include <linux/workqueue.h>
#include <mach/pwr_mgr.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <mach/pm.h>
#include <mach/chipregHw_inline.h>
#include <mach/sec_api.h>
#ifdef CONFIG_BCM_MODEM
#include <linux/broadcom/bcm_rpc.h>
#endif
#include <linux/percpu.h>
#include <mach/capri_dormant.h>

extern unsigned read_sctlr(void);
extern void write_sctlr(unsigned value);


extern unsigned read_actlr(void);
extern void write_actlr(unsigned value);
extern void disable_clean_inv_dcache_v7_l1(void);



#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>
#endif /*CONFIG_HAS_WAKELOCK*/


static DEFINE_SPINLOCK(wake_up_event_lock);

static u32 force_retention;
static u32 pm_debug = 2;
static u32 pm_en_self_refresh;
static u32 enable_test;
static u32 uart_up_delay;
static u32 one_shot;

#define UNLOCK						0x00A5A501
#define SCU_OFF_MODE				0x03030303
#define SCU_DORMANT_MODE			0x03030202
#define SCU_DORMANT2_MODE_OFF		0x03030000
#define A9_SMP_BIT					(1<<6)
#define PWRCTL_BYPASS_L2OFF			0x000000C0
#define PWRCTL_BYPASS_DORMANT		0x00000080
#define PWRCTL_BYPASS_NORMAL		0x00000000
#define PWRCTL_USE_SCU				0x00000100

#define A9_SCU_NORM				0x00
#define A9_SCU_DORM				0x02
#define A9_SCU_OFF				0x03

#define DISABLE_DORMANT_MASK \
	PWRMGR_PI_DEFAULT_POWER_STATE_ARM_CORE_DORMANT_DISABLE_MASK

#if defined(DEBUG)
#define pm_dbg printk
#else
#define pm_dbg(format...)\
	do {\
		if (pm_debug && pm_debug != 2)\
			printk(format);\
	} while (0)
#endif

#define CONFIG_CAPRI_IDLE_MODE	1

static int print_clock_count(void);
static int print_sw_event_info(void);
static int arm_pll_disable(int en);
static int arm_pll_8phase_disable(int en);
static int enter_suspend_state(struct kona_idle_state *state);
#ifdef CONFIG_CAPRI_IDLE_MODE
static int enter_idle_state(struct kona_idle_state *state);
static int enter_retention_state(struct kona_idle_state *state);
static int enter_dormant_state(struct kona_idle_state *state);
#endif

enum {
	SUSPEND_WFI,
	SUSPEND_RETENTION,
	SUSPEND_DEEPSLEEP,
	SUSPEND_DORMANT,
};

enum {
	CAPRI_STATE_C0,
	CAPRI_STATE_C1,
	CAPRI_STATE_C2,
	CAPRI_STATE_C3,
};


const char *sleep_prevent_clocks[] = {
	"caph_srcmixer_clk",
	"audioh_156m_clk",
	"audioh_2p4m_clk",
	"ssp4_audio_clk",
	"ssp3_audio_clk",
	"tmon_1m_clk",

	"bsc1_clk",
	"bsc2_clk",
	"pwm_clk",
	/*"uartb_clk",*/
	"uartb2_clk",
	"uartb3_clk",
	"spum_open",
	"spum_sec",
	"ssp0_clk",

	"sdio1_clk",
	"sdio1_sleep_clk",
	"sdio2_clk",
	"sdio2_sleep_clk",
	"sdio3_clk",
	"sdio3_sleep_clk",
	"sdio4_clk",
	"sdio4_sleep_clk",
};

int suspend_mode = SUSPEND_DORMANT;
module_param_named(suspend_mode, suspend_mode,
	int, S_IRUGO | S_IWUSR | S_IWGRP);

int enable_clr_intr;
module_param_named(enable_clr_intr, enable_clr_intr,
	int, S_IRUGO | S_IWUSR | S_IWGRP);

int debug_ds;
module_param_named(debug_ds, debug_ds,
	int, S_IRUGO | S_IWUSR | S_IWGRP);

int keep_xtl_on;
module_param_named(keep_xtl_on, keep_xtl_on,
	int, S_IRUGO | S_IWUSR | S_IWGRP);

static struct kona_idle_state capri_cpu_states[] = {
	{
		.name = "C0",
		.desc = "suspend",
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.latency = 0,
		.target_residency = 0,
		.state = CAPRI_STATE_C0,
		.enter = enter_suspend_state,
	},
	{
		.name = "C1",
		.desc = "suspend-rtn", /*suspend-retention (XTAL ON)*/
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_XTAL_ON,
		.latency = 400,
		.target_residency = 400,
		.state = CAPRI_STATE_C1,
#ifdef CONFIG_CAPRI_IDLE_MODE
		.enter = enter_idle_state,
#else
		.enter = enter_suspend_state,
#endif
	},
	{
		.name = "C2",
		.desc = "ds-retn", /*deepsleep-retention (XTAL OFF)*/
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.latency = 600,
		.target_residency = 600,
		.state = CAPRI_STATE_C2,
#ifdef CONFIG_CAPRI_IDLE_MODE
		.enter = enter_idle_state,
#else
		.enter = enter_suspend_state,
#endif
	},

#ifdef CONFIG_CAPRI_DORMANT_MODE
	{
		.name = "C3",
		.desc = "dormant",
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.latency = 1000,
		.target_residency = 1000,
		.state = CAPRI_STATE_C3,
#ifdef CONFIG_CAPRI_IDLE_MODE
		.enter = enter_idle_state,
#else
		.enter = enter_suspend_state,
#endif
	},
#else
	{
		.name = "C3",
		.desc = "retention",
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.latency = 1000,
		.target_residency = 1000,
		.state = CAPRI_STATE_C3,
#ifdef CONFIG_CAPRI_IDLE_MODE
		.enter = enter_idle_state,
#else
		.enter = enter_suspend_state,
#endif
	}
#endif /*CONFIG_CAPRI_DORMANT_MODE*/
};
#define CAPRI_A0 0xA0
#define CAPRI_A1 0xA1
#define CAPRI_CHIPID (chipregHw_getChipId() & 0X0FF)


void pm_scu_set_power_mode(int val)
{
	int cpu;

	/*per CPU byte access to SCU*/
	cpu = get_cpu();
	writeb_relaxed(val, KONA_SCU_VA +
		SCU_POWER_STATUS_OFFSET + cpu);
	put_cpu();
}

int hdmi_clk_is_idle(bool enable)
{
	u32 reg_val;

	writel(UNLOCK, KONA_PROC_CLK_VA);
	reg_val = readl(KONA_CHIPREG_VA +
		CHIPREG_PERIPH_MISC_REG2_OFFSET);
	if (enable) {
		reg_val |=
		CHIPREG_PERIPH_MISC_REG2_HDMI_CRYSTAL_CLK_IS_IDLE_MASK;
	} else {
		reg_val &=
		~CHIPREG_PERIPH_MISC_REG2_HDMI_CRYSTAL_CLK_IS_IDLE_MASK;
	}
	writel(reg_val,
		KONA_CHIPREG_VA +
		CHIPREG_PERIPH_MISC_REG2_OFFSET);
	return 0;
}

void pm_config_pll_is_idle_bits(void)
{
	u32 is_idle_mask = CHIPREG_PERIPH_MISC_REG2_SE26M_CLK_IS_IDLE_MASK |
			CHIPREG_PERIPH_MISC_REG2_DIFF26M_CLK_IS_IDLE_MASK |
			CHIPREG_PERIPH_MISC_REG2_XTAL_ELDO_MUX_SELECT_MASK;
	u32 reg_val;

	writel(UNLOCK, KONA_PROC_CLK_VA);
	reg_val = readl(KONA_CHIPREG_VA + CHIPREG_PERIPH_MISC_REG2_OFFSET);
	reg_val |= is_idle_mask;
	writel(reg_val, KONA_CHIPREG_VA + CHIPREG_PERIPH_MISC_REG2_OFFSET);
}

void pm_config_pti_cntl(bool enable)
{
	u32 reg_val;

	writel(UNLOCK, KONA_ROOT_CLK_VA);

	/*PTI/TPIU control */
	if (enable)
		writel(0, KONA_ROOT_CLK_VA +
			ROOT_CLK_MGR_REG_VAR8PH_DIVMODE_OFFSET);

	reg_val = readl(KONA_ROOT_CLK_VA +
		ROOT_CLK_MGR_REG_VAR_TPIU_VARVDD_CLKGATE_OFFSET);
	if (enable) {
		reg_val &=
	~ROOT_CLK_MGR_REG_VAR_TPIU_VARVDD_CLKGATE_VAR_TPIU_VARVDD_CLK_EN_MASK;
	} else {
		reg_val |=
	ROOT_CLK_MGR_REG_VAR_TPIU_VARVDD_CLKGATE_VAR_TPIU_VARVDD_CLK_EN_MASK;
	}
	writel(reg_val |
ROOT_CLK_MGR_REG_VAR_TPIU_VARVDD_CLKGATE_VAR_TPIU_VARVDD_HW_SW_GATING_SEL_MASK,
		KONA_ROOT_CLK_VA +
		ROOT_CLK_MGR_REG_VAR_TPIU_VARVDD_CLKGATE_OFFSET);

	reg_val = readl(KONA_ROOT_CLK_VA +
		ROOT_CLK_MGR_REG_VAR_PTI_VARVDD_CLKGATE_OFFSET);
	if (enable)
		reg_val &=
	~ROOT_CLK_MGR_REG_VAR_PTI_VARVDD_CLKGATE_VAR_PTI_VARVDD_CLK_EN_MASK;
	else {
		reg_val |=
	ROOT_CLK_MGR_REG_VAR_PTI_VARVDD_CLKGATE_VAR_PTI_VARVDD_CLK_EN_MASK;
	}
	writel(reg_val |
ROOT_CLK_MGR_REG_VAR_PTI_VARVDD_CLKGATE_VAR_PTI_VARVDD_HW_SW_GATING_SEL_MASK,
		KONA_ROOT_CLK_VA +
		ROOT_CLK_MGR_REG_VAR_PTI_VARVDD_CLKGATE_OFFSET);
}

u32 init_deep_sleep_registers(void)
{
	static struct clk *pbsc_clk;

	if (!pbsc_clk) {
		pbsc_clk = clk_get(NULL, PMU_BSC_PERI_CLK_NAME_STR);
		if (IS_ERR_OR_NULL(pbsc_clk)) {
			pr_err("pbsc_clk Inavlid clock name: %s\n", __func__);
			BUG_ON(1);
			return -EINVAL;
		}
		peri_clk_set_hw_gating_ctrl(pbsc_clk, CLK_GATING_AUTO);
	}
	pm_config_pti_cntl(true); /*Clock init should do this */
	return 0;
}

static int enable_sleep_prevention_clock(int enable)
{
	int i = 0;
	struct clk *clk;
	int no_of_clocks = 0;
	no_of_clocks = ARRAY_SIZE(sleep_prevent_clocks);

	for (i = 0; i < no_of_clocks; i++) {
		clk = clk_get(NULL, sleep_prevent_clocks[i]);
		if (IS_ERR_OR_NULL(clk)) {
			pr_info("NULL CLK - %s\n", sleep_prevent_clocks[i]);
		} else {
			if (enable)
				clk_enable(clk);
			else {
				pr_info("%s:%s use_cnt  -  %d\n",
				__func__, clk->name, clk->use_cnt);
				do {
					clk_disable(clk);
				} while (clk->use_cnt > 0);
			}
		}
	}
	if (!enable) {
		print_clock_count();
		print_sw_event_info();
	}
	return 0;
}

static int pm_enable_scu_standby(int enable)
{
	u32 reg_val = 0;
	reg_val = readl(KONA_SCU_VA + SCU_CONTROL_OFFSET);
	if (enable)
		reg_val |= SCU_CONTROL_SCU_STANDBY_EN_MASK;
	else
		reg_val &= ~SCU_CONTROL_SCU_STANDBY_EN_MASK;
	writel(reg_val, KONA_SCU_VA + SCU_CONTROL_OFFSET);
	return 0;
}

static int pm_enable_mdm_self_refresh(bool enable)
{
	u32 reg_val;

	reg_val = readl(KONA_MEMC0_NS_VA + CSR_MODEM_MIN_PWR_STATE_OFFSET);
	if (enable == true)
		reg_val &= ~CSR_MODEM_MIN_PWR_STATE_MODEM_MIN_PWR_STATE_MASK;
	else
		reg_val |= CSR_MODEM_MIN_PWR_STATE_MODEM_MIN_PWR_STATE_MASK;
	writel(reg_val, KONA_MEMC0_NS_VA + CSR_MODEM_MIN_PWR_STATE_OFFSET);


	reg_val = readl(KONA_MEMC0_NS_VA + CSR_DSP_MIN_PWR_STATE_OFFSET);
	if (enable == true)
		reg_val &= ~CSR_DSP_MIN_PWR_STATE_DSP_MIN_PWR_STATE_MASK;
	else
		reg_val |= CSR_DSP_MIN_PWR_STATE_DSP_MIN_PWR_STATE_MASK;
	writel(reg_val, KONA_MEMC0_NS_VA + CSR_DSP_MIN_PWR_STATE_OFFSET);
	return 0;
}

static int pm_enable_vc4_self_refresh(bool enable)
{
#ifndef CONFIG_LITTLE_MODE
	/*MEMC1 is only enabled for BIG HW*/
	u32 reg_val;
	reg_val = readl(KONA_MEMC1_NS_VA +
		CSR_APPS_MIN_PWR_STATE_OFFSET);
	if (enable == true)
		reg_val &= ~CSR_APPS_MIN_PWR_STATE_APPS_MIN_PWR_STATE_MASK;
	else
		reg_val |= CSR_APPS_MIN_PWR_STATE_APPS_MIN_PWR_STATE_MASK;
	writel(0, KONA_MEMC1_NS_VA +
		CSR_APPS_MIN_PWR_STATE_OFFSET);

	/*MM_500M_is_idle*/
	writel(0x3F7, KONA_MEMC1_NS_VA +
		CSR_HW_FREQ_CHANGE_CNTRL_OFFSET);

	reg_val = readl(KONA_MEMC1_NS_VA + CSR_MODEM_MIN_PWR_STATE_OFFSET);
	if (enable == true)
		reg_val &= ~CSR_MODEM_MIN_PWR_STATE_MODEM_MIN_PWR_STATE_MASK;
	else
		reg_val |= CSR_MODEM_MIN_PWR_STATE_MODEM_MIN_PWR_STATE_MASK;
	writel(reg_val, KONA_MEMC1_NS_VA + CSR_MODEM_MIN_PWR_STATE_OFFSET);

	reg_val = readl(KONA_MEMC1_NS_VA + CSR_DSP_MIN_PWR_STATE_OFFSET);
	if (enable == true)
		reg_val &= ~CSR_DSP_MIN_PWR_STATE_DSP_MIN_PWR_STATE_MASK;
	else
		reg_val |= CSR_DSP_MIN_PWR_STATE_DSP_MIN_PWR_STATE_MASK;
	writel(reg_val, KONA_MEMC1_NS_VA + CSR_DSP_MIN_PWR_STATE_OFFSET);
#endif
	return 0;
}

static int pm_enable_self_refresh(bool enable)
{
	u32 reg_val;
	if (enable == true) {
		writel(0, KONA_MEMC0_NS_VA + CSR_APPS_MIN_PWR_STATE_OFFSET);
		reg_val = readl(KONA_MEMC0_NS_VA+
			CSR_HW_FREQ_CHANGE_CNTRL_OFFSET);
		reg_val |= CSR_HW_FREQ_CHANGE_CNTRL_DDR_PLL_PWRDN_ENABLE_MASK;
		writel(reg_val, KONA_MEMC0_NS_VA+
			CSR_HW_FREQ_CHANGE_CNTRL_OFFSET);
	} else {
		writel(1, KONA_MEMC0_NS_VA +
			CSR_APPS_MIN_PWR_STATE_OFFSET);
		reg_val = readl(KONA_MEMC0_NS_VA +
			CSR_HW_FREQ_CHANGE_CNTRL_OFFSET);
		reg_val &= ~CSR_HW_FREQ_CHANGE_CNTRL_DDR_PLL_PWRDN_ENABLE_MASK;
		writel(reg_val, KONA_MEMC0_NS_VA +
			CSR_HW_FREQ_CHANGE_CNTRL_OFFSET);
	}
	return 0;
}

static int pm_config_deep_sleep(void)
{
	u32 reg_val;
	arm_pll_disable(true);
	arm_pll_8phase_disable(true);

	pwr_mgr_arm_core_dormant_enable(false /*disallow dormant*/);
	pm_enable_scu_standby(true);
	pm_config_pll_is_idle_bits();
	hdmi_clk_is_idle(true);

	/*Use SCU in A9's for retention state status*/
	if (CAPRI_CHIPID >= CAPRI_A1) {
		writel_relaxed(PWRCTL_USE_SCU,\
			KONA_CHIPREG_VA + CHIPREG_PERIPH_MISC_REG3_OFFSET);
	}

	/*Configure memory for Low Power*/
	reg_val = readl(KONA_MEMC0_NS_VA+CSR_HW_FREQ_CHANGE_CNTRL_OFFSET);
	reg_val |= CSR_HW_FREQ_CHANGE_CNTRL_HW_AUTO_PWR_TRANSITION_MASK;
	writel(reg_val, KONA_MEMC0_NS_VA+CSR_HW_FREQ_CHANGE_CNTRL_OFFSET);

	pm_enable_self_refresh(true);
	pm_enable_vc4_self_refresh(true);
	pm_enable_mdm_self_refresh(true);
	return 0;
}

static int print_clock_count(void)
{
	struct clk *clk;
	clk = clk_get(NULL, ROOT_CCU_CLK_NAME_STR);
	if (IS_ERR_OR_NULL(clk)) {
		pr_err("Inavlid clock name: %s\n", __func__);
		BUG_ON(1);
		return -EINVAL;
	}
	pm_dbg("%s:  %s clock count %d\n", __func__, clk->name, clk->use_cnt);
	clk = clk_get(NULL, KHUB_CCU_CLK_NAME_STR);
	if (IS_ERR_OR_NULL(clk)) {
		pr_err("Inavlid clock name: %s\n", __func__);
		BUG_ON(1);
		return -EINVAL;
	}
	pm_dbg("%s:  %s clock count %d\n", __func__, clk->name, clk->use_cnt);

	clk = clk_get(NULL, KHUBAON_CCU_CLK_NAME_STR);
	if (IS_ERR_OR_NULL(clk)) {
		pr_err("Inavlid clock name: %s\n", __func__);
		BUG_ON(1);
		return -EINVAL;
	}
	pm_dbg("%s:  %s clock count %d\n", __func__, clk->name, clk->use_cnt);

	clk = clk_get(NULL, KPM_CCU_CLK_NAME_STR);
	if (IS_ERR_OR_NULL(clk)) {
		pr_err("Inavlid clock name: %s\n", __func__);
		BUG_ON(1);
		return -EINVAL;
	}
	pm_dbg("%s:  %s clock count %d\n", __func__, clk->name, clk->use_cnt);

	clk = clk_get(NULL, KPS_CCU_CLK_NAME_STR);
	if (IS_ERR_OR_NULL(clk)) {
		pr_err("Inavlid clock name: %s\n", __func__);
		BUG_ON(1);
		return -EINVAL;
	}
	pm_dbg("%s:  %s clock count %d\n", __func__, clk->name, clk->use_cnt);

	return 0;
}

static void clear_wakeup_events(void)
{
	unsigned long flgs;
	spin_lock_irqsave(&wake_up_event_lock, flgs);
	/*only clearing SW1 means that 0 needs to manually be cleared*/
	pwr_mgr_event_clear_events(SOFTWARE_1_EVENT,
		SOFTWARE_1_EVENT);
	pwr_mgr_event_clear_events(COMMON_INT_TO_AC_EVENT,
		COMMON_INT_TO_AC_EVENT);
	pwr_mgr_event_clear_events(COMMON_TIMER_1_EVENT,
		COMMON_TIMER_1_EVENT);
	pwr_mgr_event_clear_events(COMMON_TIMER_2_EVENT,
		COMMON_TIMER_2_EVENT);
	pwr_mgr_event_clear_events(UBRX_EVENT,
		UBRX_EVENT);
	pwr_mgr_event_clear_events(UB2RX_EVENT,
		UB2RX_EVENT);
	pwr_mgr_event_clear_events(SIMDET_EVENT,
		SIMDET_EVENT);
	pwr_mgr_event_clear_events(SIM2DET_EVENT,
		SIM2DET_EVENT);
	pwr_mgr_event_clear_events(PMU_INT_A_EVENT,
		PMU_INT_A_EVENT);
	pwr_mgr_event_clear_events(ULPI1_EVENT,
		ULPI1_EVENT);
	pwr_mgr_event_clear_events(ULPI2_EVENT,
		ULPI2_EVENT);
	pwr_mgr_event_clear_events(KEY_CI_0_EVENT,
		KEY_CI_0_EVENT);
	spin_unlock_irqrestore(&wake_up_event_lock, flgs);
}

/*
For timebeing, COMMON_INT_TO_AC_EVENT related functions are added here
We may have to move these fucntions to somewhere else later
*/
static void clear_wakeup_interrupts(void)
{
	pm_dbg("%s\n", __func__);

/* clear interrupts for COMMON_INT_TO_AC_EVENT*/
	if (enable_clr_intr) {
		writel(0xffffffff, KONA_CHIPREG_VA +
			CHIPREG_INTERRUPT_EVENT_4_PM_CLEAR0_OFFSET);
		writel(0xffffffff, KONA_CHIPREG_VA +
			CHIPREG_INTERRUPT_EVENT_4_PM_CLEAR1_OFFSET);
		writel(0xffffffff, KONA_CHIPREG_VA +
			CHIPREG_INTERRUPT_EVENT_4_PM_CLEAR2_OFFSET);
		writel(0xffffffff, KONA_CHIPREG_VA +
			CHIPREG_INTERRUPT_EVENT_4_PM_CLEAR3_OFFSET);
		writel(0xffffffff, KONA_CHIPREG_VA +
			CHIPREG_INTERRUPT_EVENT_4_PM_CLEAR4_OFFSET);
		writel(0xffffffff, KONA_CHIPREG_VA +
			CHIPREG_INTERRUPT_EVENT_4_PM_CLEAR5_OFFSET);
		writel(0xffffffff, KONA_CHIPREG_VA +
			CHIPREG_INTERRUPT_EVENT_4_PM_CLEAR6_OFFSET);
	}
}

static void config_wakeup_interrupts(void)
{
	unsigned long flgs;

	pm_dbg("%s\n", __func__);

	spin_lock_irqsave(&wake_up_event_lock, flgs);

	/*all enabled interrupts can trigger COMMON_INT_TO_AC_EVENT*/

	writel(readl(KONA_GICDIST_VA+GICDIST_ENABLE_SET1_OFFSET),
		KONA_CHIPREG_VA+CHIPREG_INTERRUPT_EVENT_4_PM_SET0_OFFSET);
	writel(readl(KONA_GICDIST_VA+GICDIST_ENABLE_SET2_OFFSET),
		KONA_CHIPREG_VA+CHIPREG_INTERRUPT_EVENT_4_PM_SET1_OFFSET);
	writel(readl(KONA_GICDIST_VA+GICDIST_ENABLE_SET3_OFFSET),
		KONA_CHIPREG_VA+CHIPREG_INTERRUPT_EVENT_4_PM_SET2_OFFSET);
	writel(readl(KONA_GICDIST_VA+GICDIST_ENABLE_SET4_OFFSET),
		KONA_CHIPREG_VA+CHIPREG_INTERRUPT_EVENT_4_PM_SET3_OFFSET);
	writel(readl(KONA_GICDIST_VA+GICDIST_ENABLE_SET5_OFFSET),
		KONA_CHIPREG_VA+CHIPREG_INTERRUPT_EVENT_4_PM_SET4_OFFSET);
	writel(readl(KONA_GICDIST_VA+GICDIST_ENABLE_SET6_OFFSET),
		KONA_CHIPREG_VA+CHIPREG_INTERRUPT_EVENT_4_PM_SET5_OFFSET);
	writel(readl(KONA_GICDIST_VA+GICDIST_ENABLE_SET7_OFFSET),
		KONA_CHIPREG_VA+CHIPREG_INTERRUPT_EVENT_4_PM_SET6_OFFSET);

	spin_unlock_irqrestore(&wake_up_event_lock, flgs);
}

int print_sw_event_info()
{
	u32 reg_val = 0;

	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_ARM_CORE_POLICY_OFFSET + SOFTWARE_0_EVENT*4);
	pm_dbg("SW0 policy for Modem and ARM core : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA + PWRMGR_LCDTE_VI_MM_POLICY_OFFSET
		+ SOFTWARE_0_EVENT*4);
	pm_dbg("SW0 policy for MM : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA + PWRMGR_LCDTE_VI_HUB_POLICY_OFFSET
		+ SOFTWARE_0_EVENT*4);
	pm_dbg("SW0 policy for AON and HUB : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_ARM_SUBSYSTEM_POLICY_OFFSET
		+ SOFTWARE_0_EVENT*4);
	pm_dbg("SW0 policy for ARM Sub system : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_ARM_CORE_POLICY_OFFSET
		+ SOFTWARE_1_EVENT*4);
	pm_dbg("SW1 policy for Modem and ARM core : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_MM_POLICY_OFFSET + SOFTWARE_1_EVENT*4);
	pm_dbg("SW1 policy for MM : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_HUB_POLICY_OFFSET + SOFTWARE_1_EVENT*4);
	pm_dbg("SW1 policy for AON and HUB : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_ARM_SUBSYSTEM_POLICY_OFFSET
		+ SOFTWARE_1_EVENT*4);
	pm_dbg("SW1 policy for ARM Sub system : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_ARM_CORE_POLICY_OFFSET
		+ SOFTWARE_2_EVENT*4);
	pm_dbg("SW2 policy for Modem and ARM core : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_MM_POLICY_OFFSET + SOFTWARE_2_EVENT*4);
	pm_dbg("SW2 policy for MM : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_HUB_POLICY_OFFSET + SOFTWARE_2_EVENT*4);
	pm_dbg("SW2 policy for AON and HUB : %08x\n", reg_val);
	reg_val = readl(KONA_PWRMGR_VA
		+ PWRMGR_LCDTE_VI_ARM_SUBSYSTEM_POLICY_OFFSET
		+ SOFTWARE_2_EVENT*4);
	pm_dbg("SW2 policy for ARM Sub system : %08x\n", reg_val);
	return 0;
}

static int arm_pll_disable(int en)
{
	clk_set_pll_pwr_on_idle(ROOT_CCU_PLL0A, (bool) en);
	clk_set_pll_pwr_on_idle(ROOT_CCU_PLL1A, (bool) en);
	clk_set_crystal_pwr_on_idle((bool) en);
	return 0;
}
static int arm_pll_8phase_disable(int en)
{
	u32 reg_val;

	writel(UNLOCK, KONA_ROOT_CLK_VA);

	if (en) {
		reg_val = readl(KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL0CTRL0_OFFSET);
		reg_val &= ~ROOT_CLK_MGR_REG_PLL0CTRL0_PLL0_8PHASE_EN_MASK;
		writel(reg_val, KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL0CTRL0_OFFSET);
		reg_val = readl(KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL1CTRL0_OFFSET);
		reg_val &= ~ROOT_CLK_MGR_REG_PLL1CTRL0_PLL1_8PHASE_EN_MASK;
		writel(reg_val, KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL1CTRL0_OFFSET);
	} else {
		reg_val = readl(KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL0CTRL0_OFFSET);
		reg_val |= ROOT_CLK_MGR_REG_PLL0CTRL0_PLL0_8PHASE_EN_MASK;
		writel(reg_val, KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL0CTRL0_OFFSET);
		reg_val = readl(KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL1CTRL0_OFFSET);
		reg_val |= ROOT_CLK_MGR_REG_PLL1CTRL0_PLL1_8PHASE_EN_MASK;
		writel(reg_val, KONA_ROOT_CLK_VA
			+ ROOT_CLK_MGR_REG_PLL1CTRL0_OFFSET);
	}
	return 0;
}

static void capri_wfi(void)
{
	u32 timer_lsw = 0;

	enter_wfi();
	if (CAPRI_CHIPID == CAPRI_A0) {
		/* wait for Hub Clock to tick
		*(This is a HW BUG Workaround for JIRA HWCAPRI-1092))  */
		timer_lsw = readl(KONA_TMR_HUB_VA + KONA_GPTIMER_STCLO_OFFSET);
		while (timer_lsw == readl(KONA_TMR_HUB_VA \
				+ KONA_GPTIMER_STCLO_OFFSET))
			;
	}
}
#ifdef CONFIG_CAPRI_RETENTION_MODE
static int capri_retention_state(struct kona_idle_state *state)
{
	/* Code for basic retention of all A9 CCUs*/
	#define NUM_OF_CPUS 2
	int cpu;
	u32 sctlr[NUM_OF_CPUS];
	u32 actlr[NUM_OF_CPUS];

	/*per CPU access to clear/inv D$ */
	cpu = get_cpu();
	sctlr[cpu] = read_sctlr();
	put_cpu();
	disable_clean_inv_dcache_v7_l1();
	cpu = get_cpu();
	actlr[cpu] = read_actlr();
	put_cpu();
	write_actlr(read_actlr() & ~A9_SMP_BIT);
	/**/

	pm_scu_set_power_mode(A9_SCU_DORM);

	capri_wfi();

	pm_scu_set_power_mode(A9_SCU_NORM);

	/*perCpu config D$ */
	cpu = get_cpu();
	write_actlr(actlr[cpu]);
	write_sctlr(sctlr[cpu]);
	put_cpu();
	/**/

	return 0;
}
#endif

#ifdef CONFIG_CAPRI_DORMANT_MODE
void print_my_dbg_info(int core)
{
	/*extern const char *_capri__event2str[];
	* removed due to checkpatch. */
	u32 i, int_id;
	u32 read_id;

	if (core == 1) {
		int_id = readl(KONA_GICCPU_VA+0x18);
		if ((int_id & 0x3ff) == 0x3ff) {
			/* acknowledge the null */
			int_id = readl(KONA_GICCPU_VA+0xC);
			pr_info("null core1 %d\n", int_id);
		}
	} else {
		int_id = readl(KONA_GICCPU_VA +
			0x18);
		if ((int_id & 0x3ff) == 0x3ff) {
			/* acknowledge the null */
			readl(KONA_GICCPU_VA+0xC);
			int_id = readl(KONA_GICCPU_VA +
				0x18);
			read_id = readl(KONA_CHIPREG_VA
				+ CHIPREG_BOOT_2ND_ADDR_OFFSET);
			pr_info("null core0 %d %d\n",
				read_id, int_id);
		} else {
			pr_info("core0 %d\n",
				int_id);
		}
		for (i = 0; i < 0x94; i++) {
			if (i == SOFTWARE_0_EVENT ||
				i == VREQ_NONZERO_PI_MODEM_EVENT ||
				i == COMMON_TIMER_2_EVENT ||
				i == SOFTWARE_2_EVENT)
				continue;

			/*if (pwr_mgr_is_event_active(i))
			*	pr_info("%s\n",
			*		_capri__event2str[i]);
			*/
		}
	}
}

static int capri_suspend_dormant(bool enter_dormant)
{
	struct pi *pi = NULL;

	if (one_shot == 0) {
		one_shot = 1;
		init_deep_sleep_registers();
		arm_pll_disable(true);
	}

	clear_wakeup_events();
	clear_wakeup_interrupts();
	config_wakeup_interrupts();
	/*set A9's to retention state status*/

	if (force_retention)
		enable_sleep_prevention_clock(0);

	pi = pi_mgr_get(PI_MGR_PI_ID_ARM_CORE);
	BUG_ON(NULL == pi);
	pi_enable(pi, 0);

	dormant_enter(CAPRI_DORMANT_CLUSTER_DOWN);

	BUG_ON(NULL == pi);
	pi_enable(pi, 1);

	/*enable SW2 Active bit*/
	pwr_mgr_event_set(SOFTWARE_2_EVENT, 1);

	clear_wakeup_interrupts();
	/*process and clear event for wake up*/
	pwr_mgr_process_events(LCDTE_EVENT,
		BRIDGE_TO_MODEM_EVENT, false);
	pwr_mgr_process_events(USBOTG_EVENT,
		ACI_EVENT, false);
	pwr_mgr_process_events(VPM_WAKEUP_EVENT,
		ULPI2_EVENT, false);
	return 0;
}
#endif

static int capri_suspend_deepsleep(void)
{
	struct pi *pi = NULL;

	if (one_shot == 0) {
		one_shot = 1;
		init_deep_sleep_registers();
		arm_pll_disable(true);
	}

	clear_wakeup_events();
	clear_wakeup_interrupts();
	config_wakeup_interrupts();
	/*set A9's to retention state status*/

	if (debug_ds & 1)

		writel_relaxed(SCU_OFF_MODE,
		KONA_SCU_VA + SCU_POWER_STATUS_OFFSET);
	else
		writel_relaxed(SCU_DORMANT_MODE,
		KONA_SCU_VA + SCU_POWER_STATUS_OFFSET);

	if (CAPRI_CHIPID == CAPRI_A1)
		writel_relaxed(PWRCTL_BYPASS_DORMANT, \
			KONA_CHIPREG_VA + CHIPREG_PERIPH_MISC_REG3_OFFSET);

	if (force_retention)
		enable_sleep_prevention_clock(0);

	pi = pi_mgr_get(PI_MGR_PI_ID_ARM_CORE);
	BUG_ON(NULL == pi);
	pi_enable(pi, 0);
	{
		#ifdef CONFIG_CAPRI_WA_HWJIRA_1584
		/* JIRA HWCAPRI-1584 */
		udelay(100);
		#endif
	}
	capri_wfi();

	BUG_ON(NULL == pi);
	pi_enable(pi, 1);

	/*enable SW2 Active bit*/
	pwr_mgr_event_set(SOFTWARE_2_EVENT, 1);

	if (CAPRI_CHIPID == CAPRI_A1)
		writel_relaxed(PWRCTL_BYPASS_NORMAL, \
			KONA_CHIPREG_VA + CHIPREG_PERIPH_MISC_REG3_OFFSET);


	writel_relaxed(SCU_DORMANT2_MODE_OFF, KONA_SCU_VA
		+ SCU_POWER_STATUS_OFFSET);


	clear_wakeup_interrupts();
	/*process and clear event for wake up*/
	pwr_mgr_process_events(LCDTE_EVENT,
		BRIDGE_TO_MODEM_EVENT, false);
	pwr_mgr_process_events(USBOTG_EVENT,
		ACI_EVENT, false);
	pwr_mgr_process_events(VPM_WAKEUP_EVENT,
		ULPI2_EVENT, false);
	return 0;
}


static int capri_suspend_retention(void)
{
	struct pi *pi = NULL;
	/* Code for basic retention of all A9 CCUs*/

	/*disable PLL */
	arm_pll_disable(true);

	/*enable AUTOGATING BSC */
	clear_wakeup_events();
	clear_wakeup_interrupts();
	config_wakeup_interrupts();
	/*set A9's to retention state status*/

	writel(SCU_DORMANT_MODE,
		KONA_SCU_VA + SCU_POWER_STATUS_OFFSET);

	if (force_retention)
		enable_sleep_prevention_clock(0);

	pi = pi_mgr_get(PI_MGR_PI_ID_ARM_CORE);
	BUG_ON(NULL == pi);
	pi_enable(pi, 0);

	capri_wfi();

	pi = pi_mgr_get(PI_MGR_PI_ID_ARM_CORE);
	BUG_ON(NULL == pi);
	pi_enable(pi, 1);

	/*enable SW2 Active bit*/
	pwr_mgr_event_set(SOFTWARE_2_EVENT, 1);

	writel(SCU_DORMANT2_MODE_OFF, KONA_SCU_VA
		+ SCU_POWER_STATUS_OFFSET);

	/*enable PLL */
	arm_pll_disable(true);
	clear_wakeup_interrupts();
	/*process and clear event for wake up*/
	pwr_mgr_process_events(LCDTE_EVENT,
	    BRIDGE_TO_MODEM_EVENT, false);
	pwr_mgr_process_events(USBOTG_EVENT,
	    ACI_EVENT, false);
	pwr_mgr_process_events(VPM_WAKEUP_EVENT,
	    ULPI2_EVENT, false);
	return 0;
}

#ifdef CONFIG_CAPRI_IDLE_MODE
int enter_idle_state(struct kona_idle_state *state)
{
	struct pi *pi = NULL;
	unsigned long flgs;

	BUG_ON(!state);

	/*disable PLL */
	arm_pll_disable(true);

	/*Turn off XTAL only for deep sleep state*/
	if ((state->flags & CPUIDLE_FLAG_XTAL_ON) || keep_xtl_on)
		clk_set_crystal_pwr_on_idle(false);

	clear_wakeup_events();
	clear_wakeup_interrupts();
	config_wakeup_interrupts();

	pi = pi_mgr_get(PI_MGR_PI_ID_ARM_CORE);
	BUG_ON(NULL == pi);
	pi_enable(pi, 0);

	switch (state->state) {
	case CAPRI_STATE_C1:
	case CAPRI_STATE_C2:
		enter_retention_state(state);
		break;
	case CAPRI_STATE_C3:
		enter_dormant_state(state);
		break;
	default:
		enter_suspend_state(state);
		break;
	}

	spin_lock_irqsave(&wake_up_event_lock, flgs);
	/*enable SW2 Active bit*/
	pwr_mgr_event_set(SOFTWARE_2_EVENT, 1);
	clear_wakeup_interrupts();
	/*process and clear event for wake up*/
	pwr_mgr_process_events(LCDTE_EVENT,
	    BRIDGE_TO_MODEM_EVENT, false);
	pwr_mgr_process_events(USBOTG_EVENT,
	    ACI_EVENT, false);
	pwr_mgr_process_events(VPM_WAKEUP_EVENT,
	    ULPI2_EVENT, false);
	spin_unlock_irqrestore(&wake_up_event_lock, flgs);

	pi = pi_mgr_get(PI_MGR_PI_ID_ARM_CORE);
	BUG_ON(NULL == pi);
	pi_enable(pi, 1);

	if ((state->flags & CPUIDLE_FLAG_XTAL_ON) || keep_xtl_on)
		clk_set_crystal_pwr_on_idle(true);
	return -1;
}
#endif
static int enter_suspend_state(struct kona_idle_state *state)
{
	capri_wfi();
	return 0;
}

#ifdef CONFIG_CAPRI_IDLE_MODE
static int enter_retention_state(struct kona_idle_state *state)
{
	/*eventually updated to support retention IDLE entry/exit */
#ifdef CONFIG_CAPRI_RETENTION_MODE
	capri_retention_state(state);
#else
	capri_wfi();
#endif
	return 0;
}

static int enter_dormant_state(struct kona_idle_state *state)
{
	/*eventually updated to support dormant IDLE entry/exit*/
#ifdef CONFIG_CAPRI_DORMANT_IDLE
	dormant_enter(CAPRI_DORMANT_CORE_DOWN);
#else
	capri_wfi();
#endif
	return 0;
}
#endif

int kona_mach_pm_enter(suspend_state_t state)
{
	int ret = 0;

	switch (state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:

#ifdef CONFIG_HAS_WAKELOCK
	/*Don't enter WFI if any wake lock is active
	Added to take care of wake locks that gets activiated
	just before interrupts are dsiabled during suspend*/
	if (has_wake_lock(WAKE_LOCK_SUSPEND) ||
	has_wake_lock(WAKE_LOCK_IDLE)) {
		pr_info("%s:wake lock active, skip WFI\n", __func__);
		 break;
	}
#endif /*CONFIG_HAS_WAKELOCK*/

		/* suspend */
		pr_info("%s:Enter\n", __func__);
#ifdef CONFIG_BCM_MODEM
		BcmRpc_SetApSleep(1);
#endif
		switch (suspend_mode) {
		case SUSPEND_RETENTION:
			ret = capri_suspend_retention();
			break;
		case SUSPEND_DEEPSLEEP:
			ret = capri_suspend_deepsleep();
			break;
		case SUSPEND_DORMANT:
#ifdef CONFIG_CAPRI_DORMANT_MODE
			ret = capri_suspend_dormant(true);
			break;
#else
			ret = capri_suspend_deepsleep();
			break;
#endif
		case SUSPEND_WFI:
		default:
			capri_wfi();
			break;
		}
#ifdef CONFIG_BCM_MODEM
		BcmRpc_SetApSleep(0);
#endif
		break;
	default:
		pr_info("%s:Exit(error)\n", __func__);
		ret = -EINVAL;
	}

	pr_info("%s:Exit\n", __func__);
	return ret;
}

int kona_mach_get_idle_states(struct kona_idle_state **idle_states)
{
	pr_info("CAPRI => kona_mach_get_idle_states\n");
	*idle_states = capri_cpu_states;
	return ARRAY_SIZE(capri_cpu_states);
}

int __init capri_pm_init(void)
{
	pm_config_deep_sleep();
	return kona_pm_init();
}

/* Force sleep functionality utilized by at*mlpm command */
int capri_force_sleep(suspend_state_t state)
{
	int i;

	local_irq_disable();
	local_fiq_disable();

	force_retention = 1;

	/* continually attempt deep sleep */
	while (1) {
		for (i = 0; i < PWR_MGR_NUM_EVENTS; i++) {
			int test = 0;

			test |= (i == SOFTWARE_0_EVENT) ? 1 : 0;
			test |= (i == SOFTWARE_2_EVENT) ? 1 : 0;
			test |= (i == VREQ_NONZERO_PI_MODEM_EVENT) ? 1 : 0;

			if (test == 0)
				pwr_mgr_event_trg_enable(i, 0);
		}

		/* enter idle state */
		kona_mach_pm_enter(PM_SUSPEND_MEM);

	}
}

#ifdef CONFIG_DEBUG_FS

static struct clk *uartb_clk;
static int clk_active = 1;
struct delayed_work uartb_wq;

static void uartb_wq_handler(struct work_struct *work)
{
	if (force_retention) {
		if (!uartb_clk)
			uartb_clk = clk_get(NULL, "uartb_clk");
		clk_disable(uartb_clk);
		clk_active = 0;
	} else {/*releases the WFI to be active again...*/
		uart_up_delay = 0;
	}
}

void uartb_pwr_mgr_event_cb(u32 event_id, void *param)
{
	if (force_retention) {
		if (!clk_active) {
			if (!uartb_clk)
				uartb_clk = clk_get(NULL, "uartb_clk");
			clk_enable(uartb_clk);
			clk_active = 1;
		}
		cancel_delayed_work_sync(&uartb_wq);
		schedule_delayed_work(&uartb_wq,
				msecs_to_jiffies(3000));
	} else { /* After the UART interrupt is detected,
			* the system will be out of
			* retention for 3 seconds.*/
		uart_up_delay = 1;
		cancel_delayed_work_sync(&uartb_wq);
		schedule_delayed_work(&uartb_wq,
			msecs_to_jiffies(3000));
	}
}
DEFINE_SIMPLE_ATTRIBUTE(set_cp_idle_fops,
		NULL, put_CPSubsystem_to_sleep, "%llu\n");

static struct dentry *dent_capri_pm_root_dir;
int __init capri_pm_debug_init(void)
{
	INIT_DELAYED_WORK(&uartb_wq,
		uartb_wq_handler);

	pwr_mgr_register_event_handler(UBRX_EVENT,
		uartb_pwr_mgr_event_cb, NULL);

	/* create root clock dir /clock */
	dent_capri_pm_root_dir = debugfs_create_dir("capri_pm", 0);
	if (!dent_capri_pm_root_dir)
		return -ENOMEM;
	if (!debugfs_create_u32("pm_debug", S_IRUGO | S_IWUSR,
		dent_capri_pm_root_dir, (int *)&pm_debug))
		return -ENOMEM;

	if (!debugfs_create_u32("pm_en_self_refresh", S_IRUGO | S_IWUSR,
		dent_capri_pm_root_dir, (int *)&pm_en_self_refresh))
		return -ENOMEM;

	if (!debugfs_create_u32("force_retention", S_IRUGO | S_IWUSR,
		dent_capri_pm_root_dir, (int *)&force_retention))
		return -ENOMEM;

	if (!debugfs_create_u32("enable_test", S_IRUGO | S_IWUSR,
		dent_capri_pm_root_dir, (int *)&enable_test))
		return -ENOMEM;

	if (!debugfs_create_file("cp_idle",
		S_IWUSR|S_IRUSR, dent_capri_pm_root_dir,
		NULL, &set_cp_idle_fops))
			return -ENOMEM;

		return 0;
}
late_initcall(capri_pm_debug_init);

#endif

device_initcall(capri_pm_init);

