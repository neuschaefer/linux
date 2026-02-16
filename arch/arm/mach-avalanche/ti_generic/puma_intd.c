/*
 *
 * puma_intd.c 
 * Description:
 * see below
 *
 *
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */


/** \file   puma_intd.c
    \brief  puma SoC related info required by alalanche
            interrupt distributor.

    \author     PSP TII
    \version    0.1
 */
#include <pal.h>

#define AVALANCHE_SOC_MAX_HOSTS    (2)
#if PUMA7_OR_NEWER_SOC_TYPE
#define ARM11_MAX_IP_INTS_MAPPED   (AVALANCHE_IDIST_IP_INT_NUM + AVALANCHE_IDIST1_IP_INT_NUM)
#else
#define ARM11_MAX_IP_INTS_MAPPED   (32)
#endif
#define C55x_MAX_IP_INTS_MAPPED    (00)
#define AVALANCHE_SOC_TOTAL_IP_INTS_MAPPED  ( ARM11_MAX_IP_INTS_MAPPED +\
                                              C55x_MAX_IP_INTS_MAPPED )

/* NOTE: In the below enum populate  the hosts 
 * alphabetically for  proper interrupt  mappings 
 */
enum puma_hosts {ARM11, C55X };

/*
 *Please name the structure as soc_info so that generic distributor layer works. 
 */

AVALANCHE_SOC_INFO soc_info = {
    AVALANCHE_SOC_NAME,
    AVALANCHE_SOC_MAX_HOSTS,
    AVALANCHE_SOC_TOTAL_IP_INTS_MAPPED,
        {
        {ARM11,ARM11_MAX_IP_INTS_MAPPED },
        {C55X, C55x_MAX_IP_INTS_MAPPED }
        }
       };

