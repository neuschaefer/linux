/*
 * Copyright (c) 2010-2011 Broadcom Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/bug.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>

#ifdef CONFIG_ARCH_KONA

#include <chal/chal_ipc.h>
#include <mach/irqs.h>
#include <chal/chal_icd.h>
#include <mach/io_map.h>
#include <plat/pwr_mgr.h>
#define IPC_SHARED_MEM_BASE       KONA_INT_SRAM_BASE

#else

#include <csp/chal_ipc.h>
#include <csp/chal_intid.h>
#include <csp/chal_icd.h>
#define IPC_SHARED_MEM_BASE       MM_IO_BASE_SRAM

#endif

//#include <mach/vc_gpio.h>

#include <vchiq_platform_data.h>

#include "vchiq_arm.h"
#include "vchiq_kona_arm.h"
#include "vchiq_connected.h"

#include "vchiq_memdrv.h"
#include "vchiq_build_info.h"


#include <vc_mem.h>

#if defined(VCHIQ_SM_ALLOC_VCDDR)
#include "debug_sym.h"
#endif

#if defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>

static void
vchiq_early_suspend(struct early_suspend *h);
static void
vchiq_late_resume(struct early_suspend *h);

static struct early_suspend g_vchiq_early_suspend = {
	.level   = EARLY_SUSPEND_LEVEL_DISABLE_FB + 5,
	.suspend = vchiq_early_suspend,
	.resume  = vchiq_late_resume
};

#endif

#define VCHIQ_DOORBELL_IRQ BCM_INT_ID_IPC_OPEN

typedef struct {
	 unsigned int                 instNum;

	 const char                  *instance_name;
	 const VCHIQ_PLATFORM_DATA_T *platform_data;

	 struct proc_dir_entry        *instance_cfg_dir;
	 struct proc_dir_entry        *vchiq_version_cfg_entry;
	 struct proc_dir_entry        *vchiq_control_cfg_entry;

	 struct proc_dir_entry        *log_cfg_dir;
	 struct proc_dir_entry        *core_log_cfg_entry;
	 struct proc_dir_entry        *msg_log_cfg_entry;
	 struct proc_dir_entry        *sync_log_cfg_entry;
	 struct proc_dir_entry        *arm_log_cfg_entry;
	 struct proc_dir_entry        *susp_log_cfg_entry;
} VCHIQ_KERNEL_STATE_T;

struct platform_state {
	VCHIQ_ARM_STATE_T arm_state;
};

#define VCHIQ_NUM_VIDEOCORES 1


static const char *const copyright = "Copyright (c) 2011-2012 Broadcom";

static VCHIQ_KERNEL_STATE_T    *vchiq_kernel_state[VCHIQ_NUM_VIDEOCORES];
static unsigned int             vchiq_num_instances;

/* Global 'vc' proc entry used by all instances */
struct proc_dir_entry    *vc_cfg_dir;

static CHAL_IPC_HANDLE   ipcHandle;

static int               g_initialized;

static VCHIQ_STATE_T    *g_vchiq_state;
static VCHIQ_SLOT_ZERO_T *g_vchiq_slot_zero;

static int               g_use_autosuspend;
static int               g_use_suspend_timer = 1;
#ifdef CONFIG_HAS_EARLYSUSPEND
static int               g_early_susp_ctrl;
static int               g_earlysusp_suspend_allowed;
#endif
static void             *g_vchiq_ipc_shared_mem;
static int               g_vchiq_ipc_shared_mem_size;
#if defined(VCHIQ_SM_ALLOC_VCDDR)
static VC_MEM_ACCESS_HANDLE_T g_vchiq_mem_hndl;
static int                    g_vchiq_ipc_shared_mem_addr;
#endif

static irqreturn_t
vchiq_doorbell_irq(int irq, void *dev_id);

int __init
vchiq_kona_init(VCHIQ_STATE_T *state)
{
	g_vchiq_state = state;

#ifdef CONFIG_HAS_EARLYSUSPEND
	register_early_suspend(&g_vchiq_early_suspend);
#endif

	return 0;
}

void __exit
vchiq_kona_exit(VCHIQ_STATE_T *state)
{
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&g_vchiq_early_suspend);
#endif
	WARN_ON(state != g_vchiq_state);
	g_vchiq_state = NULL;
	free_irq(VCHIQ_DOORBELL_IRQ, state);
}

VCHIQ_STATUS_T
vchiq_platform_init_state(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;
	state->platform_state = kzalloc(sizeof(struct platform_state),
		GFP_KERNEL);

	status = vchiq_arm_init_state(state,
		&((struct platform_state *)state->platform_state)->arm_state);
	if (status != VCHIQ_SUCCESS) {
		kfree(state->platform_state);
		state->platform_state = NULL;
	}
	return status;
}

inline VCHIQ_ARM_STATE_T*
vchiq_platform_get_arm_state(VCHIQ_STATE_T *state)
{
	return state->platform_state
		? &((struct platform_state *)state->platform_state)->arm_state
		: NULL;
}

void
remote_event_signal(REMOTE_EVENT_T *event)
{
	wmb();

	event->fired = 1;

	dsb();         /* data barrier operation */

	if (event->armed)
		/* trigger vc interrupt */
		chal_ipc_int_vcset(ipcHandle, IPC_INTERRUPT_SOURCE_0);
}

int
vchiq_copy_from_user(void *dst, const void *src, int size)
{
	 if ((uint32_t)src < TASK_SIZE)
		return copy_from_user(dst, src, size);
	 else {
		memcpy(dst, src, size);
		return 0;
	 }
}



VCHIQ_STATUS_T
vchiq_platform_suspend(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;
	VCHIQ_ARM_STATE_T *arm_state = vchiq_platform_get_arm_state(state);
	unsigned int *wakeaddr_p = (unsigned int *)
					&g_vchiq_slot_zero->platform_data[0];

	vchiq_log_trace(vchiq_susp_log_level, "%s", __func__);

	write_lock(&arm_state->susp_res_lock);
	if (arm_state->wake_address != 0) {
		write_unlock(&arm_state->susp_res_lock);
		vchiq_log_error(vchiq_susp_log_level,
			"%s - ERROR Videocore already suspended", __func__);
		/* It should be impossible to get in to this state. */
		BUG();
		goto out;
	}
	arm_state->suspend_start_time = cpu_clock(0);
	write_unlock(&arm_state->susp_res_lock);

	/* Invalidate the wake address */
	*wakeaddr_p = ~0;
	wmb();

	/* Initiate videocore suspend */
	status = vchiq_pause_internal(state);
	if (status != VCHIQ_SUCCESS) {
		write_lock(&arm_state->susp_res_lock);
		set_suspend_state(arm_state, VC_SUSPEND_FAILED);
		if (!vchiq_videocore_wanted(state))
			start_suspend_timer(arm_state);
		vchiq_log_error(vchiq_susp_log_level,
			"VideoCore suspend failed!");
		write_unlock(&arm_state->susp_res_lock);
		goto out;
	}

	vchiq_log_info(vchiq_susp_log_level, "%s - waiting for pause event",
		__func__);


out:
	vchiq_log_trace(vchiq_susp_log_level, "%s exit %d", __func__, status);

	return status;
}

void
vchiq_platform_paused(VCHIQ_STATE_T *state)
{
	VCHIQ_ARM_STATE_T *arm_state = vchiq_platform_get_arm_state(state);
	unsigned int *wakeaddr_p = (unsigned int *)
					&g_vchiq_slot_zero->platform_data[0];

	vchiq_log_trace(vchiq_susp_log_level, "%s", __func__);

	vchiq_log_info(vchiq_susp_log_level, "%s - pause event received",
		__func__);

	do {
		cpu_relax();
		arm_state->wake_address = *wakeaddr_p;
	} while (arm_state->wake_address == ~0);

	vchiq_log_info(vchiq_susp_log_level, "%s - suspend continue received",
		__func__);

	chal_ipc_sleep_vc(ipcHandle);
	msleep(1);
	*wakeaddr_p = ~0;
	wmb();

//	vc_gpio_suspend();

#ifdef CONFIG_ARCH_KONA
	msleep(1);
	/* indicate to the PMU that videocore is in reset */
	pwr_mgr_mm_crystal_clk_is_idle(true);
#endif

	write_lock(&arm_state->susp_res_lock);
	if (arm_state->wake_address == 0) {
		set_suspend_state(arm_state, VC_SUSPEND_REJECTED);
		if (arm_state->vc_resume_state < VC_RESUME_IN_PROGRESS) {
			set_resume_state(arm_state, VC_RESUME_REQUESTED);
			request_poll(state, NULL, 0);
		}
		if (!vchiq_videocore_wanted(state))
			start_suspend_timer(arm_state);
		vchiq_log_error(vchiq_susp_log_level,
			"VideoCore suspend rejected!");
	} else {
		unsigned long long awake_time = 0;
		unsigned long nanosec_rem = 0;
		unsigned long long suspend_time = 0;
		unsigned long nanosec_susp_rem = 0;

		arm_state->sleep_start_time = cpu_clock(0);
		if (arm_state->last_wake_time) {
			awake_time = arm_state->sleep_start_time -
						arm_state->last_wake_time;
			nanosec_rem = do_div(awake_time, 1000000000);
		}

		suspend_time = arm_state->sleep_start_time -
						arm_state->suspend_start_time;
		nanosec_susp_rem = do_div(suspend_time, 1000000000);

		set_suspend_state(arm_state, VC_SUSPEND_SUSPENDED);
		/* Kick the slot handler again to see if we need to resume */
		if (arm_state->vc_resume_state == VC_RESUME_REQUESTED)
			request_poll(state, NULL, 0);
		vchiq_log_warning(vchiq_susp_log_level, "VideoCore suspended - "
			"wake address %x, (awake %lu.%06lus, suspended in "
			"%lu.%06lus)",
			arm_state->wake_address, (unsigned long)awake_time,
			nanosec_rem / 1000, (unsigned long)suspend_time,
			nanosec_susp_rem / 1000);
	}
	write_unlock(&arm_state->susp_res_lock);

	vchiq_log_trace(vchiq_susp_log_level, "%s exit", __func__);
}

VCHIQ_STATUS_T
vchiq_platform_resume(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;
	VCHIQ_ARM_STATE_T *arm_state = vchiq_platform_get_arm_state(state);

	vchiq_log_trace(vchiq_susp_log_level, "%s", __func__);

	vchiq_log_info(vchiq_susp_log_level, "Resuming VideoCore at address %x",
		arm_state->wake_address);
	arm_state->resume_start_time = cpu_clock(0);

#ifdef CONFIG_ARCH_KONA
	/* indicate to the PMU that videocore is about to come out of reset */
	pwr_mgr_mm_crystal_clk_is_idle(false);
#endif
//	vc_gpio_resume();

	/* Write the wake address to wake up videocore */
	chal_ipc_wakeup_vc(ipcHandle, arm_state->wake_address);

	vchiq_log_info(vchiq_susp_log_level,
		"Waiting for response from VideoCore...");

	vchiq_log_trace(vchiq_susp_log_level, "%s exit %d", __func__, status);
	return status;
}



void
vchiq_platform_resumed(VCHIQ_STATE_T *state)
{
	VCHIQ_ARM_STATE_T *arm_state = vchiq_platform_get_arm_state(state);
	unsigned long long sleep_time = 0;
	unsigned long nanosec_rem = 0;
	unsigned long long resume_time = 0;
	unsigned long nanosec_res_rem = 0;
	vchiq_log_trace(vchiq_susp_log_level, "%s", __func__);
	write_lock(&arm_state->susp_res_lock);
	set_resume_state(arm_state, VC_RESUME_RESUMED);
	arm_state->wake_address = 0;
	if (arm_state->sleep_start_time) {
		arm_state->last_wake_time = cpu_clock(0);
		sleep_time = arm_state->last_wake_time -
						arm_state->sleep_start_time;
		nanosec_rem = do_div(sleep_time, 1000000000);
		arm_state->sleep_start_time = 0; /* Not asleep - invalidate */
		resume_time = arm_state->last_wake_time -
						arm_state->resume_start_time;
		nanosec_res_rem = do_div(resume_time, 1000000000);
	}
	vchiq_log_warning(vchiq_susp_log_level,
		"VideoCore awake (slept %lu.%06lus, resumed in %lu.%06lus)",
		(unsigned long)sleep_time, nanosec_rem / 1000,
		(unsigned long)resume_time, nanosec_res_rem / 1000);

	/* we may have missed a poll in VCHIQ_CONNSTATE_CONNECTED so retry */
	request_poll(state, NULL, 0);
	write_unlock(&arm_state->susp_res_lock);
	vchiq_log_trace(vchiq_susp_log_level, "%s exit", __func__);
}

int
vchiq_platform_videocore_wanted(VCHIQ_STATE_T *state)
{
	int early_susp_override = 0;
	(void)state;
#ifdef CONFIG_HAS_EARLYSUSPEND
	early_susp_override = (!g_earlysusp_suspend_allowed) &&
		g_early_susp_ctrl;
#endif

	return early_susp_override || !g_use_autosuspend;
}

int
vchiq_platform_use_suspend_timer(void)
{
	return g_use_suspend_timer;
}

void
vchiq_dump_platform_use_state(VCHIQ_STATE_T *state)
{
	char *enabled  = "ENABLED";
	char *disabled = "DISABLED";
	char *en_dis = g_use_autosuspend ? enabled : disabled;
	vchiq_log_warning(vchiq_susp_log_level, "Autosuspend %s", en_dis);
#if defined(CONFIG_HAS_EARLYSUSPEND)
	en_dis = g_early_susp_ctrl ? enabled : disabled;
	vchiq_log_warning(vchiq_susp_log_level,
		"Early suspend control %s: (suspend allowed=%d)",
		en_dis, g_earlysusp_suspend_allowed || !g_early_susp_ctrl);
#endif
	en_dis = vchiq_platform_use_suspend_timer() ? enabled : disabled;
	vchiq_log_warning(vchiq_susp_log_level, "Suspend timer %s", en_dis);
}


/****************************************************************************
*
*   log_cfg_read
*
***************************************************************************/
#define PROC_WRITE_BUF_SIZE 256

#define VCHIQ_LOG_ERROR_STR   "error"
#define VCHIQ_LOG_WARNING_STR "warning"
#define VCHIQ_LOG_INFO_STR    "info"
#define VCHIQ_LOG_TRACE_STR   "trace"

static int log_cfg_read(char *buffer,
	char **start,
	off_t off,
	int count,
	int *eof,
	void *data)
{
	int len = 0;
	char *log_value = NULL;

	switch (*((int *)data)) {
	case VCHIQ_LOG_ERROR:
		log_value = VCHIQ_LOG_ERROR_STR;
		break;
	case VCHIQ_LOG_WARNING:
		log_value = VCHIQ_LOG_WARNING_STR;
		break;
	case VCHIQ_LOG_INFO:
		log_value = VCHIQ_LOG_INFO_STR;
		break;
	case VCHIQ_LOG_TRACE:
		log_value = VCHIQ_LOG_TRACE_STR;
		break;
	default:
		break;
	}

	len += sprintf(buffer + len,
		"log level set to %s\n",
		log_value ? log_value : "(null)");

	return len;
}


static int log_cfg_write(struct file *file,
	const char __user *buffer,
	unsigned long count,
	void *data)
{
	int *log_module = data;
	char kbuf[PROC_WRITE_BUF_SIZE + 1];

	(void)file;

	memset(kbuf, 0, PROC_WRITE_BUF_SIZE + 1);
	if (count >= PROC_WRITE_BUF_SIZE)
		count = PROC_WRITE_BUF_SIZE;

	if (copy_from_user(kbuf,
		buffer,
		count) != 0)
		return -EFAULT;
	kbuf[count - 1] = 0;

	if (strncmp("error", kbuf, strlen("error")) == 0)
		*log_module = VCHIQ_LOG_ERROR;
	else if (strncmp("warning", kbuf, strlen("warning")) == 0)
		*log_module = VCHIQ_LOG_WARNING;
	else if (strncmp("info", kbuf, strlen("info")) == 0)
		*log_module = VCHIQ_LOG_INFO;
	else if (strncmp("trace", kbuf, strlen("trace")) == 0)
		*log_module = VCHIQ_LOG_TRACE;
	else
		*log_module = VCHIQ_LOG_DEFAULT;

	return count;
}

static int version_read(char *buffer,
	char **start,
	off_t off,
	int count,
	int *eof,
	void *data)
{
	int len = 0;

	len += sprintf(buffer + len,
			"%s %s\n%s\nversion %s\n",
			vchiq_get_build_date(),
			vchiq_get_build_time(),
			copyright,
			vchiq_get_build_version());

	return len;
}

static int vchiq_control_cfg_output(char *buffer,
		char **start,
		off_t off,
		int count,
		int *eof,
		void *data)
{
	int len = 0;

	VCHIQ_ARM_STATE_T *arm_state =
			vchiq_platform_get_arm_state(g_vchiq_state);
	VCHIQ_KERNEL_STATE_T    *kernState = data;

	len += sprintf(buffer + len, "%s %s\n%s\nversion %s\n",
			vchiq_get_build_date(),
			vchiq_get_build_time(),
			copyright,
			vchiq_get_build_version());

	len += sprintf(buffer + len, "VCHIQ instance '%s' %s\n",
			kernState->instance_name,
			get_conn_state_name(g_vchiq_state->conn_state));

	len += sprintf(buffer + len, "Videocore %s\n",
			(arm_state->vc_suspend_state == VC_SUSPEND_SUSPENDED) ?
				"SUSPENDED" : "AWAKE");

	return len;

}

static int vchiq_control_cfg_parse(struct file *file,
	const char __user *buffer,
	unsigned long count,
	void *data)
{
	VCHIQ_KERNEL_STATE_T    *kernState = data;
	char                    *command;
	char                    kbuf[PROC_WRITE_BUF_SIZE + 1];

	(void)file;

	memset(kbuf, 0, PROC_WRITE_BUF_SIZE + 1);
	if (count >= PROC_WRITE_BUF_SIZE)
		count = PROC_WRITE_BUF_SIZE;

	if (copy_from_user(kbuf,
		buffer,
		count) != 0)
		return -EFAULT;
	kbuf[count - 1] = 0;

	command = kbuf;

	if (strncmp("connect", command, strlen("connect")) == 0) {
		if (vchiq_memdrv_initialise() != VCHIQ_SUCCESS)
			vchiq_log_error(vchiq_arm_log_level,
				"%s: failed to initialise vchiq for '%s'",
				__func__, kernState->instance_name);
		else
			vchiq_log_warning(vchiq_arm_log_level,
				"%s: initialised vchiq for '%s'", __func__,
				kernState->instance_name);
	} else if (strncmp("suspendtest", command, strlen("suspendtest"))
			== 0) {
		int success = 1;
		int cnt = 1;
		vchiq_susp_log_level = VCHIQ_LOG_INFO;
		if (g_use_autosuspend) {
			VCHIQ_ARM_STATE_T *arm_state =
				vchiq_platform_get_arm_state(g_vchiq_state);
			while (success) {
				vchiq_log_info(vchiq_susp_log_level,
					"%s: Testing suspend / resume "
					"functionality - iteration %d",
					__func__, cnt++);
				if (vchiq_use_internal(g_vchiq_state, NULL,
						USE_TYPE_VCHIQ) !=
							VCHIQ_SUCCESS) {
					success = 0;
					vchiq_log_error(vchiq_susp_log_level,
						"%s: resume FAILED", __func__);
					continue;
				}
				vchiq_log_info(vchiq_susp_log_level,
					"%s: calling vchiq_release_internal",
					__func__);
				if (vchiq_release_internal(g_vchiq_state, NULL)
						== VCHIQ_SUCCESS) {
					if (wait_for_completion_interruptible(
						&arm_state->vc_suspend_complete)
							!= 0) {
						success = 0;
						vchiq_log_error(
							vchiq_susp_log_level,
							"%s: Interrupted. "
							"Exiting...",
							__func__);
						continue;
					}
					read_lock(&arm_state->susp_res_lock);
					if (arm_state->vc_suspend_state ==
							VC_SUSPEND_FAILED) {
						success = 0;
						vchiq_log_error(
							vchiq_susp_log_level,
							"%s: suspend FAILED",
							__func__);
					}
					read_unlock(&arm_state->susp_res_lock);
				} else {
					vchiq_log_error(
						vchiq_susp_log_level,
						"%s: release FAILED",
						__func__);
					success = 0;
				}
			}
		} else {
			while (success) {
				vchiq_log_info(vchiq_susp_log_level,
					"%s: Testing suspend / resume "
					"functionality - iteration %d",
					__func__, cnt++);
				success =
					(vchiq_arm_force_suspend(g_vchiq_state)
						!= VCHIQ_ERROR);
				if (success) {
					int susp;
					vchiq_log_info(vchiq_susp_log_level,
						"%s: resuming...", __func__);
					susp = vchiq_arm_allow_resume(
						g_vchiq_state);
					if (!g_use_autosuspend && susp) {
						vchiq_log_info(
							vchiq_susp_log_level,
							"%s: ERROR: failed to "
							"resume videocore",
							__func__);
						success = 0;
					}
				}
			}
		}
		vchiq_log_info(vchiq_susp_log_level,
			"%s: Suspend / resume test exited", __func__);
	} else if (strncmp("suspend", command, strlen("suspend")) == 0) {
		/* direct control of suspend from vchiq_control */
		vchiq_log_info(vchiq_susp_log_level,
			"%s: calling vchiq_arm_force_suspend", __func__);
		if (vchiq_arm_force_suspend(g_vchiq_state) == VCHIQ_SUCCESS) {
			vchiq_log_warning(vchiq_susp_log_level,
				"%s: suspended vchiq for '%s'", __func__,
				kernState->instance_name);
		} else {
			vchiq_log_error(vchiq_susp_log_level,
				"%s: failed to suspend vchiq '%s'",
				__func__, kernState->instance_name);
		}
	} else if (strncmp("resume", command, strlen("resume")) == 0) {
		/* direct control of resume from vchiq_control */
		vchiq_log_info(vchiq_susp_log_level,
				"%s: calling vchiq_arm_allow_resume", __func__);
		if (vchiq_arm_allow_resume(g_vchiq_state) == 1) {
			vchiq_log_warning(vchiq_susp_log_level,
				"%s: resume allowed for vchiq for '%s'"
				"- videocore remains asleep",
				__func__, kernState->instance_name);
		} else {
			vchiq_log_warning(vchiq_susp_log_level,
				"%s: resumed vchiq for '%s'",
				__func__, kernState->instance_name);
		}
	} else if (strncmp("autosuspend", command, strlen("autosuspend"))
			== 0) {
		/* enable autosuspend, using vchi_service_use/release usagei
		** counters to decide when to suspend */
		g_use_autosuspend = 1;
		vchiq_log_info(vchiq_susp_log_level,
			"%s: Enabling autosuspend for vchiq instance '%s'",
			__func__, kernState->instance_name);
		vchiq_check_suspend(g_vchiq_state);
	} else if (strncmp("noautosuspend", command, strlen("noautosuspend"))
			== 0) {
		/* disable autosuspend - allow direct control of suspend/resume
		** through vchiq_control */
		g_use_autosuspend = 0;
		vchiq_log_info(vchiq_susp_log_level,
			"%s: Disabling autosuspend for vchiq instance '%s'",
			__func__, kernState->instance_name);
		vchiq_arm_allow_resume(g_vchiq_state);

	} else if (strncmp("dumpuse", command, strlen("dumpuse")) == 0) {
		/* dump usage counts for all services to determine which
		** service(s) are preventing suspend */
		vchiq_dump_service_use_state(g_vchiq_state);
	} else if (strncmp("susptimer", command, strlen("susptimer")) == 0) {
		/* enable a short timeout before suspend to allow other "use"
		** commands in */
		if (g_use_autosuspend) {
			g_use_suspend_timer = 1;
			vchiq_log_info(vchiq_susp_log_level,
				"%s: Using timeout before suspend",
				__func__);
		}
	} else if (strncmp("nosusptimer", command, strlen("nosusptimer"))
			== 0) {
		/* disable timeout before suspend - enter suspend directly on
		** usage count hitting 0 (from lp task) */
		if (g_use_autosuspend) {
			g_use_suspend_timer = 0;
			vchiq_log_info(vchiq_susp_log_level,
				"%s: Not using timeout before suspend",
				__func__);
		}
#if defined(CONFIG_HAS_EARLYSUSPEND)
	} else if (strncmp("earlysuspctrl", command, strlen("earlysuspctrl"))
			== 0) {
		/* for configs with earlysuspend, allow suspend to be blocked
		** until the earlysuspend callback is called */
		if (g_use_autosuspend) {
			g_early_susp_ctrl = 1;
			vchiq_log_info(vchiq_susp_log_level,
				"%s: Using Early Suspend control for "
				"suspend/resume",
				__func__);
			vchiq_check_resume(g_vchiq_state);
		}
	} else if (strncmp("noearlysuspctrl", command,
			strlen("noearlysuspctrl")) == 0) {
		/* disable control of suspend from earlysuspend callback */
		if (g_use_autosuspend) {
			g_early_susp_ctrl = 0;
			vchiq_log_info(vchiq_susp_log_level,
				"%s: Not using Early Suspend control for "
				"suspend/resume",
				__func__);
			vchiq_check_suspend(g_vchiq_state);
		}
#endif
	} else if (strncmp("version", command, strlen("version")) == 0) {

		vchiq_log_error(vchiq_arm_log_level,
			"%s %s\n%s\nversion %s\n",
			vchiq_get_build_date(),
			vchiq_get_build_time(),
			copyright,
			vchiq_get_build_version());
	} else {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: unknown command '%s'",
			__func__, command);
	}

	return count;
}

#if defined(CONFIG_HAS_EARLYSUSPEND)
static void
vchiq_early_suspend(struct early_suspend *h)
{
	if (g_early_susp_ctrl)
		vchiq_log_info(vchiq_susp_log_level,
			"%s: allowing suspend in vchiq",
			__func__);
	g_earlysusp_suspend_allowed = 1;
}

static void
vchiq_late_resume(struct early_suspend *h)
{
	if (g_early_susp_ctrl)
		vchiq_log_info(vchiq_susp_log_level,
			"%s: preventing suspend in vchiq",
			__func__);
	g_earlysusp_suspend_allowed = 0;
}
#endif


/****************************************************************************
*
* vchiq_userdrv_create_instance
*
*   The lower level drivers (vchiq_memdrv or vchiq_busdrv) will call this
*   function for each videocore that exists. We then register a character
*   driver which is what userspace uses to talk to us.
*
***************************************************************************/

VCHIQ_STATUS_T vchiq_userdrv_create_instance(
	const VCHIQ_PLATFORM_DATA_T *platform_data)
{
	VCHIQ_KERNEL_STATE_T *kernState;

	vchiq_log_warning(vchiq_arm_log_level,
		"%s: [bi] vchiq_num_instances = %d, VCHIQ_NUM_VIDEOCORES = %d",
		__func__, vchiq_num_instances, VCHIQ_NUM_VIDEOCORES);

	if (vchiq_num_instances >= VCHIQ_NUM_VIDEOCORES) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: already created %d instances",
			__func__, VCHIQ_NUM_VIDEOCORES);

		return VCHIQ_ERROR;
	}

	/* Allocate some memory */
	kernState = kmalloc(sizeof(*kernState), GFP_KERNEL);
	if (kernState == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to allocate memory",
			__func__);

		return VCHIQ_ERROR;
	}

	memset(kernState, 0, sizeof(*kernState));

	vchiq_kernel_state[vchiq_num_instances] = kernState;

	/* Do some bookkeeping */
	kernState->instNum = vchiq_num_instances++;
	kernState->instance_name = platform_data->instance_name;
	kernState->platform_data = platform_data;

	/* Create proc entries */
	if (vc_cfg_dir == NULL) {
		vc_cfg_dir = proc_mkdir("vc", NULL);
		if (vc_cfg_dir == NULL) {
			vchiq_log_error(vchiq_arm_log_level,
				"%s: failed to create proc directory",
				__func__);

			return VCHIQ_ERROR;
		}
	}

	kernState->vchiq_version_cfg_entry =
		create_proc_entry("version", 0,
		vc_cfg_dir);
	if (kernState->vchiq_version_cfg_entry == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc directory",
			__func__);

		return VCHIQ_ERROR;
	}
	kernState->vchiq_version_cfg_entry->data = (void *)kernState;
	kernState->vchiq_version_cfg_entry->read_proc = &version_read;
	kernState->vchiq_version_cfg_entry->write_proc = NULL;

	kernState->instance_cfg_dir = proc_mkdir(kernState->instance_name,
		vc_cfg_dir);
	if (kernState->instance_cfg_dir == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc directory",
			__func__);

		return VCHIQ_ERROR;
	}

	kernState->vchiq_control_cfg_entry =
		create_proc_entry("vchiq_control", 0,
			kernState->instance_cfg_dir);
	if (kernState->vchiq_control_cfg_entry == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc entry",
			__func__);

		return VCHIQ_ERROR;
	}
	kernState->vchiq_control_cfg_entry->data = (void *)kernState;
	kernState->vchiq_control_cfg_entry->read_proc =
		&vchiq_control_cfg_output;
	kernState->vchiq_control_cfg_entry->write_proc =
		&vchiq_control_cfg_parse;

	kernState->log_cfg_dir =
		proc_mkdir("log", kernState->instance_cfg_dir);
	if (kernState->log_cfg_dir == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc directory", __func__);

		return VCHIQ_ERROR;
	}

	kernState->core_log_cfg_entry =
		create_proc_entry("core", 0, kernState->log_cfg_dir);
	if (kernState->core_log_cfg_entry == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc entry",
			__func__);

		return VCHIQ_ERROR;
	}
	kernState->core_log_cfg_entry->data = (void *)&vchiq_core_log_level;
	kernState->core_log_cfg_entry->read_proc = &log_cfg_read;
	kernState->core_log_cfg_entry->write_proc = &log_cfg_write;

	kernState->msg_log_cfg_entry =
		create_proc_entry("msg", 0, kernState->log_cfg_dir);
	if (kernState->msg_log_cfg_entry == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc entry",
			__func__);

		return VCHIQ_ERROR;
	}
	kernState->msg_log_cfg_entry->data = (void *)&vchiq_core_msg_log_level;
	kernState->msg_log_cfg_entry->read_proc = &log_cfg_read;
	kernState->msg_log_cfg_entry->write_proc = &log_cfg_write;

	kernState->sync_log_cfg_entry =
		create_proc_entry("sync", 0, kernState->log_cfg_dir);
	if (kernState->sync_log_cfg_entry == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc entry",
			__func__);

		return VCHIQ_ERROR;
	}
	kernState->sync_log_cfg_entry->data = (void *)&vchiq_sync_log_level;
	kernState->sync_log_cfg_entry->read_proc = &log_cfg_read;
	kernState->sync_log_cfg_entry->write_proc = &log_cfg_write;

	kernState->arm_log_cfg_entry =
		create_proc_entry("arm", 0, kernState->log_cfg_dir);
	if (kernState->arm_log_cfg_entry == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc entry",
			__func__);

		return VCHIQ_ERROR;
	}
	kernState->arm_log_cfg_entry->data = (void *)&vchiq_arm_log_level;
	kernState->arm_log_cfg_entry->read_proc = &log_cfg_read;
	kernState->arm_log_cfg_entry->write_proc = &log_cfg_write;

	kernState->susp_log_cfg_entry =
		create_proc_entry("susp", 0, kernState->log_cfg_dir);
	if (kernState->susp_log_cfg_entry == NULL) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to create proc entry",
			__func__);

		return VCHIQ_ERROR;
	}
	kernState->susp_log_cfg_entry->data = (void *)&vchiq_susp_log_level;
	kernState->susp_log_cfg_entry->read_proc = &log_cfg_read;
	kernState->susp_log_cfg_entry->write_proc = &log_cfg_write;

	vchiq_log_info(vchiq_arm_log_level,
		"%s: initialised vchiq for '%s'\n",
		__func__,
		kernState->instance_name);

	return VCHIQ_SUCCESS;
}
EXPORT_SYMBOL(vchiq_userdrv_create_instance);

/****************************************************************************
*
* vchiq_userdrv_suspend
*
*   The lower level drivers (vchiq_memdrv or vchiq_busdrv) will call this
*   function to suspend each videocore.
*
***************************************************************************/

VCHIQ_STATUS_T vchiq_userdrv_suspend(const VCHIQ_PLATFORM_DATA_T *platform_data)
{
	VCHIQ_KERNEL_STATE_T *kernState = NULL;
	VCHIQ_STATUS_T status;
	int i;

	for (i = 0; i < vchiq_num_instances; i++) {
		if (vchiq_kernel_state[i]->platform_data == platform_data) {
			kernState = vchiq_kernel_state[i];
			break;
		}
	}

	if (!kernState) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to find state for instance %s",
			__func__,
			platform_data->instance_name);

		return VCHIQ_ERROR;
	}


	/* force videocore to suspend, even if autosuspend didn't want us to */
	status = vchiq_arm_force_suspend(g_vchiq_state);

	if (status == VCHIQ_SUCCESS)
		vchiq_log_warning(vchiq_susp_log_level,
			"%s: suspended vchiq for '%s'",
			__func__,
			kernState->instance_name);
	else
		vchiq_log_error(vchiq_susp_log_level,
			"%s: failed to suspend vchiq '%s'",
			__func__,
			kernState->instance_name);

	return status;
}
EXPORT_SYMBOL(vchiq_userdrv_suspend);

/****************************************************************************
*
* vchiq_userdrv_resume
*
*   The lower level drivers (vchiq_memdrv or vchiq_busdrv) will call this
*   function to resume each videocore.
*
***************************************************************************/

VCHIQ_STATUS_T vchiq_userdrv_resume(const VCHIQ_PLATFORM_DATA_T *platform_data)
{
	VCHIQ_KERNEL_STATE_T *kernState = NULL;
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;
	int i, suspended;

	for (i = 0; i < vchiq_num_instances; i++) {
		if (vchiq_kernel_state[i]->platform_data == platform_data) {
			kernState = vchiq_kernel_state[i];
			break;
		}
	}

	if (!kernState) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to find state for instance %s",
			__func__,
			platform_data->instance_name);

		return VCHIQ_ERROR;
	}

	vchiq_log_info(vchiq_susp_log_level,
		"%s: resuming vchiq for '%s'", __func__,
		kernState->instance_name);

	suspended = vchiq_arm_allow_resume(g_vchiq_state);

	if (suspended) {
		vchiq_log_warning(vchiq_susp_log_level,
			"%s: vchiq for '%s' remains suspended", __func__,
			kernState->instance_name);
	} else {
		vchiq_log_warning(vchiq_susp_log_level,
			"%s: resumed vchiq for '%s'", __func__,
			kernState->instance_name);
	}

	return status;
}
EXPORT_SYMBOL(vchiq_userdrv_resume);

/*
 * Due to the limitations at the RTL level, there are some GPIO pins that
 * cannot be muxed to the videocore. As a workaround, we are using a combination
 * of the IPC doorbells and shared memory to communicate between the host and
 * videocore to control the pins that are muxed to the host.
 *
 * For the time being, we are piggybacking off of the sharedmem driver because
 * it is the fastest way. The long term solution would see the code that deals
 * with the GPIO moved to a separate service/driver.
 *
 * Operation details:
 *    - syncing:
 *         Before the videocore can request GPIO operations from the host, the
 *         videocore needs to make sure the host is up first. When the videocore
 *         boots up, it rings an IPC doorbell. If the host is already up at
 *         that moment, it will ring the same IPC doorbell back. Upon receiving
 *         the doorbell, the videocore will know the host is up and ready.
 *         Before this time, the videocore will return failure on any host GPIO
 *         requests.
 *
 *         To cover the scenario where the videocore comes up before the host,
 *         the host will automatically ring the IPC doorbell to notify the
 *         videocore.
 *
 *    - setting/clearing:
 *         1. Videocore writes GPIO pin to be set/clear into the shared memory
 *            at GPIO_MAILBOX_WRITE. If performing a set, the value is also
 *            bitwise OR'd with GPIO_MAILBOX_WRITE_SET.
 *         2. Videocore rings the IPC doorbell and waits for a reply.
 *         3. Host answers the doorbell and sets/clears the GPIO pin.
 *         4. Host rings the same IPC doorbell to notify the videocore.
 *         5. Videocore gets the doorbell and returns to the user.
 *
 *    - reading:
 *         1. Videocore writes GPIO pin to be read into the shared memory at
 *            GPIO_MAILBOX_READ.
 *         2. Videocore rings the IPC doorbell and waits for a reply.
 *         3. Host answers the doorbell and reads the value of the GPIO and
 *            stores it back into the shared memory at GPIO_MAILBOX_READ.
 *         4. Host rings the same IPC doorbell to notify the videocore.
 *         5. Videocore gets the doorbell and reads out the value of the GPIO
 *            from the shared memory and returns it to the user.
 */
static void
service_gpio(uint32_t irq_status)
{
	uint32_t gpio_mailbox_write =
		(*(uint32_t *)(g_vchiq_ipc_shared_mem +
			IPC_SHARED_MEM_CHANNEL_ARM_OFFSET +
			IPC_SHARED_MEM_GPIO_WRITE_OFFSET));
	uint32_t gpio_mailbox_read  =
		(*(uint32_t *)(g_vchiq_ipc_shared_mem +
			IPC_SHARED_MEM_CHANNEL_ARM_OFFSET +
			IPC_SHARED_MEM_GPIO_READ_OFFSET));

#define GPIO_MAILBOX_WRITE_SET        (0x80000000)
#define GPIO_MAILBOX_WRITE_PIN_MASK   (0x7FFFFFFF)

	if (irq_status &
		(IPC_INTERRUPT_STATUS_ENABLED << IPC_INTERRUPT_SOURCE_2)) {
		uint32_t reg = gpio_mailbox_write;

		if (reg & GPIO_MAILBOX_WRITE_SET)
			/* GPIO set */
			gpio_set_value(reg & GPIO_MAILBOX_WRITE_PIN_MASK , 1);
		else
			/* GPIO clear */
			gpio_set_value(reg & GPIO_MAILBOX_WRITE_PIN_MASK , 0);

		/* Notify videocore that GPIO has been changed */
		chal_ipc_int_vcset(ipcHandle, IPC_INTERRUPT_SOURCE_2);

		irq_status &= ~(IPC_INTERRUPT_STATUS_ENABLED <<
			IPC_INTERRUPT_SOURCE_2);
	}

	if (irq_status &
		(IPC_INTERRUPT_STATUS_ENABLED << IPC_INTERRUPT_SOURCE_3)) {
		gpio_mailbox_read = gpio_get_value(gpio_mailbox_read);
		/* GPIO set */

		/* Notify videocore that GPIO has been set */
		chal_ipc_int_vcset(ipcHandle, IPC_INTERRUPT_SOURCE_3);

		irq_status &= ~(IPC_INTERRUPT_STATUS_ENABLED <<
			IPC_INTERRUPT_SOURCE_3);
	}

	if (irq_status & (IPC_INTERRUPT_STATUS_ENABLED <<
		IPC_INTERRUPT_SOURCE_4)) {
		/* Reply back to the videocore to tell them we are ready */
		chal_ipc_int_vcset(ipcHandle, IPC_INTERRUPT_SOURCE_4);

		irq_status &= ~(IPC_INTERRUPT_STATUS_ENABLED <<
			IPC_INTERRUPT_SOURCE_4);
	}
}

/*
 * Local functions
 */

static irqreturn_t
vchiq_doorbell_irq(int irq, void *dev_id)
{
	IPC_INTERRUPT_SOURCE source;
	uint32_t             status;
	VCHIQ_STATE_T       *state = dev_id;

	/* get the interrupt status value */
	chal_ipc_get_int_status(ipcHandle, &status);

	/* clear all the interrupts first */
	for (source = IPC_INTERRUPT_SOURCE_0;
		source < IPC_INTERRUPT_SOURCE_MAX; source++) {
		if (status & (IPC_INTERRUPT_STATUS_ENABLED << source))
			chal_ipc_int_clr(ipcHandle, source);
	}

	if (status & (IPC_INTERRUPT_STATUS_ENABLED << IPC_INTERRUPT_SOURCE_0))
		/* signal the stack that there is something to pick up */
		remote_event_pollall(state);
	else if ((status & (IPC_INTERRUPT_STATUS_ENABLED <<
				IPC_INTERRUPT_SOURCE_2)) ||
				(status & (IPC_INTERRUPT_STATUS_ENABLED <<
					IPC_INTERRUPT_SOURCE_3)) ||
				(status & (IPC_INTERRUPT_STATUS_ENABLED <<
					IPC_INTERRUPT_SOURCE_4)))
		/* this is a GPIO request */
		service_gpio(status);

	return IRQ_HANDLED;
}

/****************************************************************************
*
*   vchiq_memdrv_initialise
*
***************************************************************************/

VCHIQ_STATUS_T vchiq_memdrv_initialise(void)
{
	VCHIQ_STATE_T *state;
	VCHIQ_STATUS_T status;
	int err = 0;
	int i;

	if (g_initialized) {
		vchiq_log_warning(vchiq_arm_log_level,
			"%s: already initialized", __func__);
		return 0;
	}

#if defined(VCHIQ_SM_ALLOC_VCDDR)
	VC_MEM_ADDR_T vcMemAddr;
	size_t vcMemSize;
	uint8_t *mapAddr;
	off_t  vcMapAddr;

	g_vchiq_ipc_shared_mem_size = 0;

	if (OpenVideoCoreMemory(&g_vchiq_mem_hndl) == 0) {
		if (LookupVideoCoreSymbol(g_vchiq_mem_hndl,
			VCHIQ_IPC_SHARED_MEM_SIZE_SYMBOL,
			&vcMemAddr,
			&vcMemSize)) {
			vcMapAddr = (off_t)vcMemAddr & VC_MEM_TO_ARM_ADDR_MASK;
			vcMapAddr += mm_vc_mem_phys_addr;
			mapAddr = ioremap_nocache(vcMapAddr, vcMemSize);
			if (mapAddr != 0) {
				memcpy(&g_vchiq_ipc_shared_mem_size,
					mapAddr,
					vcMemSize);
				iounmap(mapAddr);
			} else
				vchiq_log_error(vchiq_arm_log_level,
					"!!! failed to ioremap \"%s\" @ 0x%x "
					"(phys: 0x%x, size: %u).",
					VCHIQ_IPC_SHARED_MEM_SIZE_SYMBOL,
					(unsigned int)vcMapAddr,
					(unsigned int)vcMemAddr,
					(unsigned int)vcMemSize);
		} else
			vchiq_log_error(vchiq_arm_log_level,
				"!!! failed to lookup \"%s\".",
				VCHIQ_IPC_SHARED_MEM_SIZE_SYMBOL);

		if (LookupVideoCoreSymbol(g_vchiq_mem_hndl,
			VCHIQ_IPC_SHARED_MEM_SYMBOL,
			&vcMemAddr,
			&vcMemSize)) {
			vcMapAddr = (off_t)vcMemAddr & VC_MEM_TO_ARM_ADDR_MASK;
			vcMapAddr += mm_vc_mem_phys_addr;
			mapAddr = ioremap_nocache(vcMapAddr, vcMemSize);
			if (mapAddr != 0) {
				memcpy(&g_vchiq_ipc_shared_mem_addr,
					      mapAddr,
					      vcMemSize);
				iounmap(mapAddr);
			} else {
				vchiq_log_error(vchiq_arm_log_level,
					"!!! failed to ioremap \"%s\" @ 0x%x "
					"(phys: 0x%x, size: %u).",
					VCHIQ_IPC_SHARED_MEM_SYMBOL,
					(unsigned int)vcMapAddr,
					(unsigned int)vcMemAddr,
					(unsigned int)vcMemSize);
			}

			vcMapAddr = (off_t)g_vchiq_ipc_shared_mem_addr &
				VC_MEM_TO_ARM_ADDR_MASK;
			vcMapAddr = vcMapAddr + mm_vc_mem_phys_addr;
			mapAddr = ioremap_nocache(vcMapAddr,
				(size_t)g_vchiq_ipc_shared_mem_size);
			if (mapAddr != 0) {
				g_vchiq_ipc_shared_mem = mapAddr;
				/* Do not **iounmap** at this time, we can now
				** use the shared memory mapped.
				*/
			} else
				vchiq_log_error(vchiq_arm_log_level,
					"!!! failed to ioremap shared memory "
					"region for vchiq @ 0x%x "
					"(phys: 0x%x, size: %u).",
					(unsigned int)vcMapAddr,
					(unsigned int)
						g_vchiq_ipc_shared_mem_addr,
					(unsigned int)
						g_vchiq_ipc_shared_mem_size);
		} else
			vchiq_log_error(vchiq_arm_log_level,
				"!!! failed to lookup \"%s\".",
				VCHIQ_IPC_SHARED_MEM_SIZE_SYMBOL);

		CloseVideoCoreMemory(g_vchiq_mem_hndl);
	} else
		vchiq_log_error(vchiq_arm_log_level,
			"!!! failed to open videocore memory.\n");

	vchiq_log_info(vchiq_arm_log_level,
		"Videocore allocated %u (0x%x) bytes of shared memory.\n",
		g_vchiq_ipc_shared_mem_size,
		g_vchiq_ipc_shared_mem_size);
	vchiq_log_info(vchiq_arm_log_level,
		"Shared memory (0x%x) mapped @ 0x%p for kernel usage.\n",
		(unsigned int)g_vchiq_ipc_shared_mem_addr,
		g_vchiq_ipc_shared_mem);

	if ((g_vchiq_ipc_shared_mem_size == 0) ||
		  (g_vchiq_ipc_shared_mem == NULL))
		return VCHIQ_ERROR;
#else
	g_vchiq_ipc_shared_mem = IPC_SHARED_MEM_SLOTS_VIRT;
	g_vchiq_ipc_shared_mem_size = IPC_SHARED_MEM_SLOTS_SIZE;
#endif

	vchiq_log_warning(vchiq_arm_log_level,
		"%s: ipc shared memory address                       = 0x%p",
		__func__, g_vchiq_ipc_shared_mem);
	vchiq_log_warning(vchiq_arm_log_level,
		"%s: ipc shared memory size (vc+arm channels, extra) = 0x%x",
		__func__, g_vchiq_ipc_shared_mem_size);
	vchiq_log_warning(vchiq_arm_log_level,
		"%s: VCHIQ_MAX_SERVICES        = %d",
		__func__, VCHIQ_MAX_SERVICES);

	g_vchiq_slot_zero = (VCHIQ_SLOT_ZERO_T *)g_vchiq_ipc_shared_mem;
	state = g_vchiq_state;

	status = vchiq_platform_deferred_init(state, g_vchiq_slot_zero);

	if (status != VCHIQ_SUCCESS) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: vchiq_init_state failed",
			__func__);
		goto failed_init_state;
	}

	ipcHandle = chal_ipc_config(NULL);
	chal_icd_set_security(0, VCHIQ_DOORBELL_IRQ, eINT_STATE_SECURE);
	for (i = 0; i < IPC_INTERRUPT_SOURCE_MAX; i++)
		chal_ipc_int_secmode(ipcHandle, i, IPC_INTERRUPT_MODE_OPEN);

	/* clear all interrupts */
	for (i = 0; i < IPC_INTERRUPT_SOURCE_MAX; i++)
		chal_ipc_int_clr(ipcHandle, i);

	err = request_irq(VCHIQ_DOORBELL_IRQ, vchiq_doorbell_irq,
		IRQF_DISABLED, "IPC driver", state);
	if (err != 0) {
		vchiq_log_error(vchiq_arm_log_level,
			"%s: failed to register irq=%d err=%d",
			__func__,
			VCHIQ_DOORBELL_IRQ, err);
		goto failed_request_irq;
	} else
		/* Tell the videocore we are ready for servicing GPIO
		** requests */
		chal_ipc_int_vcset(ipcHandle, IPC_INTERRUPT_SOURCE_4);

	g_initialized = 1;

	vchiq_call_connected_callbacks();

	return VCHIQ_SUCCESS;

failed_request_irq:
failed_init_state:
	return VCHIQ_ERROR;
}
EXPORT_SYMBOL(vchiq_memdrv_initialise);
