/*
 *
 * cpgmac_perf_stats.h
 * Description:
 * see below
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
 *
 */


#ifndef _CPMAC_PERF_STATS_H_
#define _CPMAC_PERF_STATS_H_ 

#define rdtscl(dest)  __asm__  __volatile__("mfc0 %0,$9; nop" : "=r" (dest))
    
#define MAX_CPMAC_FUNCS                             10
#define DRV_CB_PKT_RX_SERVICE                        0
#define DRV_CB_PKT_XMIT_COMPLETION                   1
#define DRV_PKT_XMIT_QUEUE                           2
#define DRV_RX_BUFFER_ALLOC                          3
    
#define HAL_PKT_RX_SERVICE                           4
#define HAL_PKT_XMIT_COMPLETION                      5
#define HAL_PKT_XMIT_QUEUE                           6
    
#define PKT_XMIT_LATENCY                             7
#define BRIDGE_LATENCY                               8
    struct cpmac_profile {
    unsigned long start_time;
    unsigned long total_time;
    unsigned long total_pkts;
};
extern struct cpmac_profile
 cpmac_perf_stats[CPMAC_MAX_INSTANCES][MAX_CPMAC_FUNCS];
static __inline__ void cpmac_start_profile_timer(int inst, int func) 
{
    int start;
    rdtscl(start);
    cpmac_perf_stats[inst][func].start_time = start;
} static __inline__ void cpmac_profile_record(int inst, int func,
                                                  int num_of_pkts,
                                                  int start, int end) 
{
    int total_time;
    int time;
    if (end > start)
        time = end - start;
    
    else
        time = ~start + end + 1;
    total_time = cpmac_perf_stats[inst][func].total_time;
    if ((total_time + time) >= total_time)
         {
        cpmac_perf_stats[inst][func].total_time += time;
        cpmac_perf_stats[inst][func].total_pkts += num_of_pkts;
        }
}
static __inline__ void cpmac_stop_profile_timer(int inst, int func,
                                                   int num_of_pkts) 
{
    int start;
    int end;
    rdtscl(end);
    start = cpmac_perf_stats[inst][func].start_time;
    cpmac_profile_record(inst, func, num_of_pkts, start, end);
} 
#endif  /* _CPMAC_PERF_STATS_H_ */
