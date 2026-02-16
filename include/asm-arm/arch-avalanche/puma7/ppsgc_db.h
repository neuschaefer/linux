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

#ifndef _PPSGC_DB_H_
#define _PPSGC_DB_H_

#include "ppsgc_defs.h"

/* Un-assigned group counter ID - Must be zero for default value */
#define PPSGC_GROUP_COUNTER_UNASSIGNED      (0)

#define PPSGC_GROUP_COUNTER_HTON(_groupCounterId)   htons(_groupCounterId)
#define PPSGC_GROUP_COUNTER_NTOH(_groupCounterId)   ntohs(_groupCounterId)

/*! \fn static inline int PPSGC_Db_AssignGroupCounterId(
 *          struct PpsgcAssignedGroupCounters *assignedGroupCounters,
 *          unsigned short groupCounterId)
 *  \brief Assign group counter ID to group counters IDs table if not all
 *         table assigned.
 *  \param[in/out] assignedGroupCounters - Pointer to assigned group counters
 *          structure to assign new group counter ID in. Not assigned entry
 *          must have the value PPSGC_GROUP_COUNTER_UNASSIGNED.
 *  \param[in] groupCounterId - Group counter ID to assign in table.
 *  \return OK or error status.
 */
static inline int PPSGC_Db_AssignGroupCounterId(
		struct PpsgcAssignedGroupCounters *assignedGroupCounters,
		unsigned short                    groupCounterId)
{
	/* Validate group counter ID range */
	if (groupCounterId >= PPSGC_NUM_OF_GROUP_COUNTERS)
		return PPSGC_RC_FAILURE;

	if (assignedGroupCounters->numAssignedGroupCounters >=
		PPSGC_GROUP_COUNTERS_PER_SESSION) {
		/* Assigned group counter IDs table is full */
		return PPSGC_RC_FAILURE;
	}

	/* Assign new counter ID and update assigned group counters number */
	assignedGroupCounters->groupCounterIds[assignedGroupCounters->
		numAssignedGroupCounters] = PPSGC_GROUP_COUNTER_HTON(groupCounterId);
	assignedGroupCounters->numAssignedGroupCounters++;
	return PPSGC_RC_SUCCESS;
}

/*! \fn int PPSGC_Db_Init(void)
 *  \brief Packet Processor Session Group Counters data base initialization.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_Init(void);

/*! \fn int PPSGC_Db_Exit(void)
 *  \brief Packet Processor Session Group Counters data base deinitialization.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_Exit(void);

/*! \fn int PPSGC_Db_AllocGroupCounterId(unsigned short *groupCounterId)
 *  \brief Allocate Group Counter ID.
 *  \param[in/out] groupCounterId - Pointer to group counter ID value.
 *  \return OK or error status.
 */
int PPSGC_Db_AllocGroupCounterId(unsigned short *groupCounterId);

/*! \fn int PPSGC_Db_ReleaseGroupCounterId(unsigned short groupCounterId)
 *  \brief Release Group Counter ID.
 *  \param[in] groupCounterId - Group Counter ID to release.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_ReleaseGroupCounterId(unsigned short groupCounterId);

/*! \fn int PPSGC_Db_DeleteGroupCounterSessions(unsigned short groupCounterId)
 *  \brief Send request to PP to delete sessions list of Group Counter ID.
 *  \param[in] groupCounterId - Group Counter ID to delete sessions.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_DeleteGroupCounterSessions(unsigned short groupCounterId);

/*! \fn int PPSGC_Db_GetGroupCounterAmount
 *                      (struct PpsgcGroupCounters *totalPpCounters)
 *  \brief Retreives the total counters amount of all session in specified
 *         Group Counter.
 *  \param[in,out] totalPpCounters - Total Group Counter counters amount.
 *  \return OK or error status.
 */
int PPSGC_Db_GetGroupCounterAmount(struct PpsgcGroupCounters *totalPpCounters);

/*! \fn int PPSGC_Db_GetGroupSessionsInfo(unsigned int tableSize,
 *              struct PpsgcGroupSessionsInfo *groupSessionsInfoTable)
 *  \brief Retreives sessions information of sessions in specified
 *         Group Counter table.
 *  \param[in] tableSize - Group sessions info table size.
 *  \param[in,out] groupSessionsInfoTable - Group sessions info table.
 *  \return OK or error status.
 */
int PPSGC_Db_GetGroupSessionsInfo(unsigned int tableSize,
		struct PpsgcGroupSessionsInfo *groupSessionsInfoTable);

/*! \fn int PPSGC_Db_CreateSession
 *              (unsigned int sessionHandle, unsigned char sessionType,
 *               unsigned char multicast,    unsigned int ingressPortId,
 *               struct PpsgcAssignedGroupCounters *assignedGroupCounters)
 *  \brief Handle session creation indication from Packet Processor.
 *   Assign session to all Group Counters in assigned Group Counters table.
 *  \param[in] sessionHandle - PP session handle number.
 *  \param[in] sessionType - PP session type.
 *  \param[in] multicast - Is session multicast or unicast.
 *  \param[in] ingressPortId - input device interface index.
 *  \param[in] assignedGroupCounters - Pointer to assigned group counters
 *              IDs structure.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_CreateSession
	(unsigned int sessionHandle, unsigned char sessionType,
	 unsigned char multicast,    unsigned int ingressPortId,
	 struct PpsgcAssignedGroupCounters *assignedGroupCounters);

/*! \fn int PPSGC_Db_DeleteSession
 *          (unsigned int sessionHandle, unsigned int sessionPacketsFw)
 *  \brief Handle session deletion indication from Packet Processor.
 *   Add session counter to Group Counter dead sessions counter.
 *  \param[in] sessionHandle - PP session handle number.
 *  \param[in] sessionPacketsFw - PP session forwarded packets number.
 *  \param[in] sessionOctetsFw - PP session forwarded bytes number.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_DeleteSession
	(unsigned int sessionHandle, unsigned int sessionPacketsFw,
	 unsigned long long sessionOctetsFw);

/*! \fn int PPSGC_Db_GetIfCounters
 *                      (struct PpsgcIfCounters *ppsgcIfCounters)
 *  \brief Get packet processor interface device counters according to
 *         interface name.
 *  \param[in,out] ppsgcIfCounters - Packet processor interface device
 *      counters structure.
 *  \return OK or error status.
 */
int PPSGC_Db_GetIfCounters(struct PpsgcIfCounters *ppsgcIfCounters);

/*! \fn int PPSGC_Db_GetSfCounters(struct PpsgcSfCounters *ppsgcSfCounters)
 *  \brief Get PP service flow counters according to interface name.
 *  \param[in,out] ppsgcSfCounters - PP service flow counters structure.
 *  \return OK or error status.
 */
int PPSGC_Db_GetSfCounters(struct PpsgcSfCounters *ppsgcSfCounters);

/*! \fn int PPSGC_Db_GetTpPortCounters
 *                      (struct PpsgcTpPortCounters* ppsgcTpPortCounters)
 *  \brief Get PP transparent port counters according to interface name.
 *  \param[in,out] ppsgcTpPortCounters-PP transparent port counters structure.
 *  \return OK or error status.
 */
int PPSGC_Db_GetTpPortCounters
	(struct PpsgcTpPortCounters *ppsgcTpPortCounters);

/*! \fn int PPSGC_Db_PrintIfCounters(void)
 *  \brief Print packet processor interface device counters.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_PrintIfCounters(void);

/*! \fn int PPSGC_Db_PrintTpPortCounters(void)
 *  \brief Print acket processor transparent port counters.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_PrintTpPortCounters(void);

/*! \fn int PPSGC_Db_PrintGroupCounters(void)
 *  \brief Print allocated group counters statistics.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 */
int PPSGC_Db_PrintGroupCounters(void);
#endif /* _PPSGC_DB_H_ */

