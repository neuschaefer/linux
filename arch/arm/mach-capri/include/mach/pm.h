#ifndef __CAPRI_PM_H__
#define __CAPRI_PM_H__

#include <linux/suspend.h>

#define KONA_MACH_MAX_IDLE_STATE 1
int put_CPSubsystem_to_sleep(void *data, u64 clk_idle);
void enter_wfi(void);
int capri_force_sleep(suspend_state_t state);
extern void request_suspend_state(suspend_state_t state);

#endif /*__CAPRI_PM_H__*/
