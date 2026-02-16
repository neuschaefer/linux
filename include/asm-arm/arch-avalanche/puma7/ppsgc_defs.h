/*
  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution
  in the file called LICENSE.GPL.

  Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
*/

#ifndef _PPSGC_DEFS_H_
#define _PPSGC_DEFS_H_

#ifdef CONFIG_ARM_AVALANCHE_SOC
#include <asm-arm/arch-avalanche/generic/avalanche_pp_api.h>
#else
#include <linux/avalanche/generic/avalanche_pp_api.h>
#endif

#define PPSGC_NUM_OF_GROUP_COUNTERS         (512)
#define PPSGC_IF_MANE_MAX_LENGTH            (16)
#define PPSGC_GROUP_COUNTERS_PER_SESSION    (8)
#define PPSGC_RC_FAILURE                    (-1)
#define PPSGC_RC_SUCCESS                    (0)
#define PPSGC_MAX_JIFFIES_VALUE             ULLONG_MAX

/*! \var struct PpsgcAssignedGroupCounters
    \brief Structure defines the assigned session group counter IDs.
*/
struct PpsgcAssignedGroupCounters {
	unsigned char       numAssignedGroupCounters;
	unsigned short      groupCounterIds[PPSGC_GROUP_COUNTERS_PER_SESSION];
};

/*! \var struct PpsgcPpCounters
    \brief Structure defines packet processor session counters.
*/
struct PpsgcPpCounters {
	unsigned long long      totalBytesFwd;
	unsigned long long      totalPktsFwd;
	unsigned long long      multicastPktsFwd;
	unsigned long long      unicastPktsFwd;
};

/*! \var struct PpsgcGroupCounters
    \brief Structure defines packet processor sessions group counters amount.
*/
struct PpsgcGroupCounters {
	unsigned short          groupCounterId;
	struct PpsgcPpCounters  ppCounters;
};

struct PpsgcGroupSessionsInfo {
	unsigned short          groupCounterId;
	unsigned short          sessionsNumber;
	unsigned long long      lastSessionDelJiffies;
};

/*! \var struct PpsgcIfCounters
    \brief Structure defines the format of PP counters per interface device.
*/
struct PpsgcIfCounters {
	char                        ifName[PPSGC_IF_MANE_MAX_LENGTH];
	AVALANCHE_PP_VPID_STATS_t   ifCounters;
	unsigned char               includeQosDrops;
	unsigned char               AlignTo64Bit[7];
};

/*! \var struct PpsgcSfCounters
    \brief Structure defines the format of PP counters per service flow.
*/
struct PpsgcSfCounters {
	unsigned char                   ifName[PPSGC_IF_MANE_MAX_LENGTH];
	AVALANCHE_PP_QOS_QUEUE_STATS_t  sfCounters;
	unsigned int                    sfIndex;
	unsigned int                    clearFlag;
};

/*! \var struct PpsgcTpPortCounters
    \brief Structure defines the format of PP counters per transparent port.
*/
struct PpsgcTpPortCounters {
	unsigned char               ifName[PPSGC_IF_MANE_MAX_LENGTH];
	AVALANCHE_PP_VPID_STATS_t   tpCounters;
};

#endif /* _PPSGC_DEFS_H_ */

