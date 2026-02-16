/*
 * ti_ppd_pmon.c
 *  Description:
 *  Packet processor PMON file.
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

/*#include <pal_cppi41.h> Needed for header pointer ops like cache, phys */

#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>

#include <asm-arm/arch-avalanche/generic/ti_ppd.h>
#include "ppd_pvt.h"
#include "ppd_os.h"

/* Note: All calculations are base of PDSP cycle counts sampled thus
 * if the cycle counts or PDSP is disabled, the age value we get will
 * be less than the actual.
 */ 
#define PDSP_CYCLES_PER_SEC     (200000000)
#define PDSP_OFLOW_CYCLE_CNT    ((0xffffffff/PDSP_CYCLES_PER_SEC) \
                                    *PDSP_CYCLES_PER_SEC)
#define PDSP_SECS_PER_OFLOW     (0xffffffff/PDSP_CYCLES_PER_SEC)

void  ppd_pdsp_poll_timer (unsigned long data)
{
    Uint32 cycle_sec_cnt_hi, cycle_sec_cnt_lo, cookie, cycle_count, stall_count,
           cycle_cnt_hi, cycle_cnt_lo, stall_cnt_hi, stall_cnt_lo;
    ppd_pdsp_poll_cfg* pdsp_poll_cfg = (ppd_pdsp_poll_cfg*)data;
    int i;

    for (i = 0; i < PP_NUM_PDSP; i++)
    {
        PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
        if (g_is_in_psm) {
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
            return;
        }
        stall_count = g_pdsp_regs[i]->stall_count;
        cycle_count = g_pdsp_regs[i]->cycle_count;
        g_pdsp_regs[i]->cycle_count = 0;

        /* Check for the rare case when cyccle count reached 0xffffffff and
         * counter was disabled
         */
        if ((cycle_count == 0xffffffff) && ((g_pdsp_regs[i]->control 
                    & (PDSP_REG_RUN_STATE_BIT | PDSP_REG_COUNT_ENABLE_BIT))
                    == PDSP_REG_RUN_STATE_BIT))
        {
            /*printErr ("Cycle count wrap.\nRe-enabling counting...\n");*/
            g_pdsp_regs[i]->control |= PDSP_REG_COUNT_ENABLE_BIT;
        }
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

        cycle_sec_cnt_hi = g_pdsp_counts[i].cycle_sec_cnt_hi;
        cycle_sec_cnt_lo = g_pdsp_counts[i].cycle_sec_cnt_lo;
            
        cycle_cnt_hi = g_pdsp_counts[i].cycle_cnt_hi;
        cycle_cnt_lo = g_pdsp_counts[i].cycle_cnt_lo;
        
        stall_cnt_hi = g_pdsp_counts[i].stall_cnt_hi;
        stall_cnt_lo = g_pdsp_counts[i].stall_cnt_lo;
        
        cycle_sec_cnt_lo += cycle_count;

        if (cycle_sec_cnt_lo < cycle_count)
        {
            cycle_sec_cnt_hi += PDSP_SECS_PER_OFLOW;
            cycle_sec_cnt_lo += 0xffffffff - PDSP_OFLOW_CYCLE_CNT;
        }

        if (cycle_sec_cnt_lo >= PDSP_OFLOW_CYCLE_CNT)
        {
            cycle_sec_cnt_hi += PDSP_SECS_PER_OFLOW;
            cycle_sec_cnt_lo -= PDSP_OFLOW_CYCLE_CNT;
        }

        cycle_cnt_lo += cycle_count;
        if (cycle_cnt_lo < cycle_count)
            cycle_cnt_hi++;

        stall_cnt_lo += stall_count;
        if (stall_cnt_lo < stall_count)
            stall_cnt_hi++;
        
        PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
      
        g_pdsp_counts[i].cycle_sec_cnt_hi = cycle_sec_cnt_hi;
        g_pdsp_counts[i].cycle_sec_cnt_lo = cycle_sec_cnt_lo;
        
        g_pdsp_counts[i].cycle_cnt_lo = cycle_cnt_lo;
        g_pdsp_counts[i].cycle_cnt_hi = cycle_cnt_hi;

        g_pdsp_counts[i].stall_cnt_lo = stall_cnt_lo;
        g_pdsp_counts[i].stall_cnt_hi = stall_cnt_hi;

        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
    
        /* Can further reduce the lock duration by using 4 more temp variables
         */
    }

    PAL_osTimerStart (pdsp_poll_cfg->hTimer, pdsp_poll_cfg->polltime_msecs);
}

#define SECS_PER_DAY            (60*60*24)
#define SECS_PER_HR             (60*60)
#define SECS_PER_MIN            (60)

typedef struct
{
    Uint16 days;
    Uint16 hrs;
    Uint16 mins;
    Uint16 secs;
} ppd_uptime_t;

static void ppd_uptime_from_pdsp_cnt (Uint32 hi_cnt, Uint32 lo_cnt, 
                                        ppd_uptime_t* uptime)
{
    /* Tick count is accumulated as 64-bit value with lower 32 bit
     * representing the actual tick count and higher 32 bits
     * indicating how many seconds the lower 32 bit count overflowed,
     * i.e., it reached maximum possible ticks to hold in 32 bit
     * range. Thus hi_cnt is converted into secs as hi_cnt *
     * secs/num_lo_cnt_overflows as below.
     */
    
   /*
    * TODO: Consider the case when hi_cnt*secs_per_overflow exceeds 32 bit
    * limit.
    */
    hi_cnt = hi_cnt + (lo_cnt/PDSP_CYCLES_PER_SEC);

    uptime->days = (Uint16) (hi_cnt / (SECS_PER_DAY));
    if (uptime->days) hi_cnt = hi_cnt % (SECS_PER_DAY);

    uptime->hrs = (Uint16) (hi_cnt / (SECS_PER_HR));
    if (uptime->hrs) hi_cnt = hi_cnt % (SECS_PER_HR);

    uptime->mins = (Uint16) (hi_cnt / (SECS_PER_MIN));
    if (uptime->mins) hi_cnt = hi_cnt % (SECS_PER_MIN);

    uptime->secs = (Uint16) (hi_cnt);
}

Int32 ti_ppd_get_pdsp_status (Uint8 pdsp_id, TI_PP_PDSP_STATUS *status)
{
    Uint32 cycle_sec_cnt_hi, cycle_sec_cnt_lo;
    Uint32 control, cookie;
    ppd_uptime_t uptime;

    if (!g_ppd_init_done || (pdsp_id >= PP_NUM_PDSP)) return -1;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    if (!g_is_in_psm)
    {
        control = g_pdsp_regs [pdsp_id]->control;

        if (control & PDSP_REG_SLEEP_BIT)
            status->state = TI_PP_PDSP_STATE_SLEEPING;
        else if (control & PDSP_REG_RUN_STATE_BIT)
            status->state = TI_PP_PDSP_STATE_RUNNING;
        else
            status->state = TI_PP_PDSP_STATE_HALTED;

        status->prog_counter = g_pdsp_regs [pdsp_id]->status & 0xffff;
    }
    else
    {
        status->state = TI_PP_PDSP_STATE_HALTED;
        status->prog_counter = 0xffff;
    }

    status->cycle_cnt_hi = g_pdsp_counts [pdsp_id].cycle_cnt_hi;
    status->cycle_cnt_lo = g_pdsp_counts [pdsp_id].cycle_cnt_lo;
    status->stall_cnt_hi = g_pdsp_counts [pdsp_id].stall_cnt_hi;
    status->stall_cnt_lo = g_pdsp_counts [pdsp_id].stall_cnt_lo;
    cycle_sec_cnt_hi = g_pdsp_counts [pdsp_id].cycle_sec_cnt_hi;
    cycle_sec_cnt_lo = g_pdsp_counts [pdsp_id].cycle_sec_cnt_lo;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    ppd_uptime_from_pdsp_cnt (cycle_sec_cnt_hi, cycle_sec_cnt_lo, &uptime); 
    status->uptime_days = uptime.days; 
    status->uptime_hrs  = uptime.hrs;
    status->uptime_mins = uptime.mins;
    status->uptime_secs = uptime.secs; 

    return 0;
}

Int32 ti_ppd_get_ses_age (Uint8 session_handle, Uint16* p_days, 
                          Uint16* p_hrs, Uint16* p_mins, Uint16* p_secs)
{
    Uint32 cookie, hi_cnt, lo_cnt, diff_hi_cnt, diff_lo_cnt;
    ppd_uptime_t uptime;
    
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    if (!IS_SESSION_SLOT_FILLED(session_handle)) {
        printErr ("%s: ***Error: session %d does not exist.\n", 
                    __FUNCTION__, session_handle);
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return -1;
    }
    hi_cnt = g_pdsp_counts [CPDSP].cycle_sec_cnt_hi;
    lo_cnt = g_pdsp_counts [CPDSP].cycle_sec_cnt_lo;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    diff_hi_cnt = hi_cnt - g_ses_timestamp [session_handle].cycle_cnt_hi;
    diff_lo_cnt = lo_cnt - g_ses_timestamp [session_handle].cycle_cnt_lo;

    if (diff_lo_cnt >= PDSP_OFLOW_CYCLE_CNT)
    {
        diff_hi_cnt -= PDSP_SECS_PER_OFLOW;
        diff_lo_cnt += PDSP_OFLOW_CYCLE_CNT;
    }

    ppd_uptime_from_pdsp_cnt (diff_hi_cnt, diff_lo_cnt, &uptime); 

    *p_days = uptime.days;
    *p_hrs  = uptime.hrs;
    *p_mins = uptime.mins;
    *p_secs = uptime.secs;
    
    return 0;
}

Int32 ti_ppd_pdsp_control (Uint8 pdsp_id, Uint32 ctl_op, Ptr ctl_data)
{
    if (!g_ppd_init_done || ((ctl_op != TI_PP_PDSPCTRL_PSM) 
                                && ((pdsp_id >= PP_NUM_PDSP)
                                    || (g_is_in_psm))))
        return -1;

    switch (ctl_op)
    {
        case TI_PP_PDSPCTRL_HLT:
            g_pdsp_regs [pdsp_id]->control &= ~(PDSP_CTL_EN_BIT);
            break;

        case TI_PP_PDSPCTRL_STEP:
            g_pdsp_regs [pdsp_id]->control 
                    = (g_pdsp_regs [pdsp_id]->control & ~(PDSP_CTL_EN_BIT)) 
                        | (PDSP_REG_SINGLE_STEP_BIT);
            break;

        case TI_PP_PDSPCTRL_FREERUN:
            g_pdsp_regs [pdsp_id]->control 
                    = (g_pdsp_regs [pdsp_id]->control | (PDSP_CTL_EN_BIT)) 
                        & ~(PDSP_REG_SINGLE_STEP_BIT);
            break;

        case TI_PP_PDSPCTRL_RESUME:
            g_pdsp_regs [pdsp_id]->control |= (PDSP_CTL_EN_BIT);
            break;

        case TI_PP_PDSPCTRL_RST:
            {
            int wait_count = 10000000;
            g_pdsp_regs [pdsp_id]->control = 0;
            while(wait_count--)    
                if(g_pdsp_regs [pdsp_id]->control & (PDSP_CTL_N_RST_BIT))
                    break;

            if(!wait_count) {
                printErr ("Timeout waiting for reset complete for PDSDP(%d)\n",
                       pdsp_id);
                return (-1);
            }
            
            g_pdsp_regs [pdsp_id]->control = ((*((Uint16*)ctl_data)) << 16) 
                                                | (PDSP_CTL_EN_BIT) 
                                                | (PDSP_REG_COUNT_ENABLE_BIT);
            }
            break;

        case TI_PP_PDSPCTRL_PSM:
            {
                int i;
                if ((*((Uint32*)ctl_data)) && !g_is_in_psm)
                {
                    if (ppd_set_psm (1))
                        return -1;

                    for (i = 0; i < PP_NUM_PDSP; i++)
                        g_pdsp_regs [pdsp_id]->control &= ~(PDSP_CTL_EN_BIT);

                    g_is_in_psm = 1;

                    PAL_osTimerStop (g_pdsp_poll_cfg.hTimer);
                    PAL_osTimerStop (g_ppd_event_hdlr_cfg.hTimer);
                }
                else if (!(*((Uint32*)ctl_data)) && g_is_in_psm)
                {
                    g_is_in_psm = 0;

                    PAL_osTimerStart (g_ppd_event_hdlr_cfg.hTimer,
                            g_ppd_event_hdlr_cfg.polltime_msecs);
                    PAL_osTimerStart (g_pdsp_poll_cfg.hTimer,
                            g_pdsp_poll_cfg.polltime_msecs);

                    for (i = 0; i < PP_NUM_PDSP; i++)
                        g_pdsp_regs [pdsp_id]->control |= (PDSP_CTL_EN_BIT);
                    if (ppd_set_psm (0))
                        return -1;
                }
                else
                    return -1;
            }
            break;

        default:
            printErr ("Unsupported PDSP control option (%d)\n", ctl_op);
            return -1;
    }

    printk ("control reg val = %#x\n", g_pdsp_regs [pdsp_id]->control);
    return 0;
}

Int32 ti_ppd_set_Timer (Uint8 StartStop)
{
    if (!StartStop)
    {
        PAL_osTimerStop (g_pdsp_poll_cfg.hTimer);
        PAL_osTimerStop (g_ppd_event_hdlr_cfg.hTimer);
    }
    else
    {
        PAL_osTimerStart (g_ppd_event_hdlr_cfg.hTimer, g_ppd_event_hdlr_cfg.polltime_msecs);
        PAL_osTimerStart (g_pdsp_poll_cfg.hTimer, g_pdsp_poll_cfg.polltime_msecs);
    }
}
