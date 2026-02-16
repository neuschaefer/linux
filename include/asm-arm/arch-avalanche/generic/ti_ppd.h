/*
 * ti_ppd.h
 * ti_ppd.h. Packet processor driver private header file.
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2009-2014 Intel Corporation.

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

  BSD LICENSE 

  Copyright(c) 2009-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _INCLUDE_TI_PPD_H
#define _INCLUDE_TI_PPD_H

/* Import the TI standard primitive "C" types defines */
#include <asm-arm/arch-avalanche/generic/_tistdtypes.h>
#include  <linux/ti_ppm.h>

/**********************************************************************
 * TI_PPD_PP_EVENT_.....
 *
 *  The following events are supported by Packet Processor Firmware.
 */
#define TI_PPD_PP_EVENT_SESSION_IDLE            0x01
#define TI_PPD_PP_EVENT_SESSION_NOTIDLE         0x02
#define TI_PPD_PP_EVENT_SESSION_SYNCTIMEOUT     0x03
#define TI_PPD_PP_EVENT_CONFIG_ERROR            0x04
#define TI_PPD_PP_EVENT_SESSION_SYNCOVERFLOW    0x05
#define TI_PPD_PP_EVENT_OVERFLOW_GLOBAL         0x10
#define TI_PPD_PP_EVENT_OVERFLOW_VPID           0x11
#define TI_PPD_PP_EVENT_OVERFLOW_SESSION        0x12
#define TI_PPD_PP_EVENT_INTFATAL_SSTATE         0x80
#define TI_PPD_INTERNAL_EVENT_TDOX_EVALUATION   0x100


/*
 * ti_ppd_init -
 *
 * Desctiption:
 *  This API initializes the Packet Processor Driver (PPD).
 *  The caller should provide the initialization configuration for Packet Processor system.
 *  The PP System is brought into operational state after successful completion of this API.
 *
 * Parameters:
 *  cfg (IN)        - Pointer to default configuration
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_init(TI_PPD_CONFIG *cfg);


/*
 * TI_PPD_IF -
 *
 * Description:
 *  Interface information associated with session's ingress/egress property.
 *  Consists of PID and VPID parameters.
 */
typedef struct ti_ppd_if
{
    TI_PP_PID   *pid;
    TI_PP_VPID  *vpid;

} TI_PPD_IF;


/*
 *                      Session management APIs
 */

/*
 * ti_ppd_create_session -
 *
 * Description:
 *  This API creates a session. Following information is required:
 *      -   Session identifier (0-255)
 *      -   Ingress parameters for classification based on L2 and/or L3
 *      -   Session type and Egress parameters for modification (if applicable)
 *
 * Notes:
 *  Following points list the constraints imposed:
 *      -   Session state is forced as SYNC REQUIRED
 *      -   L3 Modification prepared only when session is routable.
 *      -   Every L3/L4 field to be modified should have corresponding ingress
 *          field provided for checksum delta calculation. The enables flags in
 *          the descriptor will be used to deduce this information
 *      -   L2 header will be modified when enables flags in egress l2
 *          descriptor are set.
 *      -   *IMPORTANT NOTE* Current implementation requires routable sessions
 *          to have new L2 header provided * CHK *
 *      -   Minimum header information must be provided for egress L2. The
 *          enables flags in the descriptor will be used to deduce the
 *          availability of header fields.
 *      -   PID entry is always enabled in LUT L2 data
 *      -   When timeout is specified, only IDLE based timeout is supported
 *      -   For sessions with multiple egress properties, Modification record of
 *          1st egress record only is configured (and used for subsequent
 *          records).
 *          Thus, only one set of modifications is allowed across all applicable
 *          egress records.
 *
 *
 * Precondition:
 *  - ti_ppd_create_pid for ingress and egress PID
 *  - ti_ppd_enable_vpid for ingress and egress VPID
 *
 * Parameters:
 *  session_cfg (IN)    - Pointer to session description. Should be allocated
 *                        and filled by caller
 *  ingress_if (IN)     - Ingress PID and VPID information
 *  egress_if (IN)      - Array of Egress PID and VPID information
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_create_session (TI_PP_SESSION* ses_cfg, TI_PPD_IF *ingress_if, TI_PPD_IF *egress_if, int isTunnel);




/*
 * ti_ppd_session_is_tdox_set -
 *
 * Description:
 *  This API verifies if the TDOX is still enabled at the session:
 *      -   Session identifier (0-255)
 *
 * Parameters:
 *  ses_id (IN)         - Pointer to session description. Should be allocated and filled by caller
 *
 * Return:
 *  TRUE or FALSE
 */
int ti_ppd_session_is_tdox_set( unsigned char ses_id );

/*
 * ti_ppd_session_tdox_change -
 *
 * Description:
 *  This API modifies the session's TDOX property. Following information is required:
 *      -   Session identifier (0-255)
 *      -   TDOX enable or disable
 *
 * Notes:
 *      -   For sessions with multiple egress properties, Modification record of
 *          1st egress record only is configured.
 *
 *
 * Precondition:
 *  - ti_ppd_create_session
 *
 * Parameters:
 *  ses_id (IN)         - Pointer to session description. Should be allocated
 *                        and filled by caller
 *  enable (IN)         - TDOX enable
 *
 * Return:
 *  0 on Success, <0 on error.
 */
void ti_ppd_session_tdox_change(unsigned char ses_id, unsigned char tdoxId, Uint32 enableFlags);


/*
 * ti_ppd_delete_session -
 *
 * Description:
 *  This API removes previously created session.
 *
 * Precondition:
 *  Session must a valid session created using ti_ppd_create_session
 *
 * Parameters:
 *  session_handle (IN) - Session identifier
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_delete_session(Uint8 session_handle);

/*
 *                          PID related APIs
 */

/*
 * ti_ppd_config_pid_range -
 *
 * Desctiption:
 *  This API configures PID range for specified CPPI port and type.
 *
 * Precondition:
 *  ti_ppd_init
 *
 * Parameters:
 *  pid_range (IN)  - Pointer to PID range configuration structure.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_config_pid_range(TI_PP_PID_RANGE *pid_range);

/*
 * ti_ppd_remove_pid_range -
 *
 * Desctiption:
 *  This API removes the PID range for the specified CPPI port.
 *
 * Precondition:
 *  ti_ppd_config_pid_range
 *
 * Parameters:
 *  port_num (IN)  - CPPI port numberfor the range to be removed.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_remove_pid_range(Uint32 port_num);

/*
 * ti_ppd_create_pid -
 *
 * Desctiption:
 *  This API creates and configures PID.
 *
 * Precondition:
 *  The PID must fall within the range installed previously by
 *  ti_ppd_config_pid_range
 *
 * Parameters:
 *  pid_params (IN)  - Pointer to PID configuration structure.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_create_pid(TI_PP_PID *pid_params);

/*
 * ti_ppd_set_pid_flags -
 *
 * Desctiption:
 *  This API sets the PID flags to specified value.
 *
 * Precondition:
 *  PID must have been created successfully by ti_ppd_create_pid
 *
 * Parameters:
 *  pid_params (IN) -   Pointer to PID configuration structure.
 *  new_flags (IN)  -   New flags.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_set_pid_flags(TI_PP_PID *pid_params, Uint32 new_flags);

/*
 * ti_ppd_delete_pid -
 *
 * Desctiption:
 *  This API removes the specified PID.
 *
 * Precondition:
 *  PID must have been created successfully by ti_ppd_create_pid
 *
 * Note:
 *  The caller must ensure that all the VPIDs based on specified PID are removed
 *  before this call (this implicitly means that all the sessions based on the
 *  respective VPID should have been removed).
 *
 * Parameters:
 *  pid_handle (IN) - PID index.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_delete_pid(Uint8 pid_handle);


/*
 *                  VPID related APIs
 */

/*
 * ti_ppd_create_vpid -
 *
 * Desctiption:
 *  This API creates specified VPID with default flags.
 *
 * Precondition:
 *  PPD must have been initilized using ti_ppd_init
 *
 * Parameters:
 *  vpid_params (IN)  - Pointer to VPID configuration structure.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_create_vpid(TI_PP_VPID *vpid_params);

/*
 * ti_ppd_set_vpid_flags -
 *
 * Desctiption:
 *  This API sets VPID flags to specified value. Note that the old flags will be
 *  overwritten with the new ones. It is advised that the caller maintains the
 *  current copy of flags value to ensure that the VPID enable/disable status is
 *  as desired.
 *
 * Precondition:
 *  VPID must have been created successfully by ti_ppd_create_vpid
 *
 * Parameters:
 *  vpid_params (IN) - Pointer to VPID configuration structure.
 *  new_flags (IN)   - New flags
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_set_vpid_flags(TI_PP_VPID *vpid_params, Uint32 new_flags);

/*
 * ti_ppd_delete_vpid -
 *
 * Desctiption:
 *  This API removes the specified VPID.
 *
 * Precondition:
 *  VPID must have been created successfully by ti_ppd_create_vpid
 *
 * Note:
 *  The caller must also ensure that all sessions that depend on the specified
 *  VPID are removed using ti_ppd_delete_session before calling this API.
 *
 * Parameters:
 *  vpid_handle (IN) - VPID index.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_delete_vpid(Uint8 vpid_handle);


/*
 *                  Statistics APIs
 */
/*
 * ti_ppd_get_vpid_stats -
 *
 * Description:
 *  This API provides the statistics for the specified VPID. The statistics
 *  maintained in firmware are NOT cleared by this API.
 *
 * Note:
 *  The statistics structure should match the corresponding statistics block
 *  provided by the firmware since this API directly fills the statistics query
 *  result in the provided structure.
 *
 * Precondition:
 *  VPID must have been created successfully by ti_ppd_create_vpid
 *
 * Parameters:
 *  vpid_handle (IN)    - VPID index
 *  stats (OUT)         - Pointer to the structure to hold the statistics in.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_vpid_stats(Uint8 vpid_handle, TI_PP_VPID_STATS *stats);

/*
 * ti_ppd_clear_vpid_stats -
 *
 * Description:
 *  This API clears the statistics for specified VPID.
 *
 * Precondition:
 *  VPID must have been created successfully by ti_ppd_create_vpid
 *
 * Parameters:
 *  vpid_handle (IN)    - VPID index
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_clear_vpid_stats(Uint8 vpid_handle);

/*
 * ti_ppd_get_n_clear_vpid_stats -
 *
 * Description:
 *  This API provides the statistics for the specified VPID. The statistics
 *  maintained in firmware are cleared by this API.
 *
 * Note:
 *  The statistics structure should match the corresponding statistics block
 *  provided by the firmware since this API directly fills the statistics query
 *  result in the provided structure.
 *
 * Precondition:
 *  VPID must have been created successfully by ti_ppd_create_vpid
 *
 * Parameters:
 *  vpid_handle (IN)    - VPID index
 *  stats (OUT)         - Pointer to the structure to hold the statistics in.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_n_clear_vpid_stats(Uint8 vpid_handle,
                                    TI_PP_VPID_STATS *stats);

/*
 *  ti_ppd_get_srl_pkt_stats -
 *
 * Description:
 *  This API provides the Packet Processor global statistics. The statistics
 *  maintained in firmware are NOT cleared by this API.
 *
 * Note:
 *  The statistics structure should match the corresponding statistics block
 *  provided by the firmware since this API directly fills the statistics query
 *  result in the provided structure.
 *
 * Precondition:
 *  PPD must have been initialized by ti_ppd_init
 *
 * Parameters:
 *  stats (OUT)         - Pointer to the structure to hold the statistics in.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_srl_pkt_stats(TI_PP_GLOBAL_STATS *stats);

/*
 * ti_ppd_clear_srl_pkt_stats -
 *
 * Description:
 *  This API clears the global statistics.
 *
 * Precondition:
 *  PPD must have been initialized by ti_ppd_init
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_clear_srl_pkt_stats(void);

/*
 *  ti_ppd_get_n_clear_srl_pkt_stats -
 *
 * Description:
 *  This API provides the Packet Processor global statistics. The statistics
 *  maintained in firmware are cleared by this API.
 *
 * Note:
 *  The statistics structure should match the corresponding statistics block
 *  provided by the firmware since this API directly fills the statistics query
 *  result in the provided structure.
 *
 * Precondition:
 *  PPD must have been initialized by ti_ppd_init
 *
 * Parameters:
 *  stats (OUT)         - Pointer to the structure to hold the statistics in.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_n_clear_srl_pkt_stats(TI_PP_GLOBAL_STATS *stats);

/*
 * ti_ppd_get_session_pkt_stats -
 *
 * Description:
 *  This API provides the statistics for the specified session. The statistics
 *  maintained in firmware are NOT cleared by this API.
 *
 * Note:
 *  The statistics structure should match the corresponding statistics block
 *  provided by the firmware since this API directly fills the statistics query
 *  result in the provided structure.
 *
 * Precondition:
 *  The session  must have been created successfully by ti_ppd_create_session
 *
 * Parameters:
 *  session_handle (IN) - Session id
 *  stats (OUT)         - Pointer to the structure to hold the statistics in.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_session_pkt_stats(Uint8 session_handle,
                                    TI_PP_SESSION_STATS *stats);

/*
 * ti_ppd_clear_session_pkt_stats -
 *
 * Description:
 *  This API clears the statistics for the specified session.
 *
 * Precondition:
 *  The session  must have been created successfully by ti_ppd_create_session
 *
 * Parameters:
 *  session_handle (IN) - Session id
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_clear_session_pkt_stats(Uint8 session_handle);

/*
 * ti_ppd_get_n_clear_session_pkt_stats -
 *
 * Description:
 *  This API provides the statistics for the specified session. The statistics
 *  maintained in firmware are cleared by this API.
 *
 * Note:
 *  The statistics structure should match the corresponding statistics block
 *  provided by the firmware since this API directly fills the statistics query
 *  result in the provided structure.
 *
 * Precondition:
 *  The session  must have been created successfully by ti_ppd_create_session
 *
 * Parameters:
 *  session_handle (IN) - Session id
 *  stats (OUT)         - Pointer to the structure to hold the statistics in.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_n_clear_session_pkt_stats(Uint8 session_handle,
                                           TI_PP_SESSION_STATS *stats);

typedef void (*TI_PPD_EVENT_HANDLER) (unsigned short event_id,
                                        unsigned int param1,
                                        unsigned int param2);
Int32 ti_ppd_set_ack_suppression(int enDis);

Int32 ti_ppd_set_traffic_prioritization_mode(Uint8 priorityMode);
#define TI_PP_PRIORITY_3BIT_MASK                        0x07 
#define TI_PP_PRIORITY_TOS_PRIO_SHIFT                   5
#define TI_PP_MAPPING_DIVIDER_FOR_2_PRIORITIES_LEVELS   4
#define TI_PP_MAPPING_DIVIDER_FOR_4_PRIORITIES_LEVELS   2
#define TI_PP_MAX_PRIORITIES_LEVELS                     4                  
#define TI_PP_PRIORITY_TOS_VLAN                         0
#define TI_PP_PRIORITY_TOS_ONLY                         1
#define TI_PP_PRIORITY_VLAN_ONLY                        2
#define TI_PP_PRIORITY_DISABLE_PRIORITIZATION           3
#define TI_PP_PRIORITY_INVALID_VALUE                    0xFF

Int32 ti_ppd_set_mta_mac_address(Uint8* mtaAddress);

#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT
Int32 ti_ppd_set_tunnel_mode(int tunnelMode);
Int32 ti_ppd_set_cm_mac_address(Uint8* cmAddress);
#endif

/*
 * ti_ppd_register_event_handler -
 *
 * Description:
 *  This API is used to register Packet Processor event handler with the PPD.
 *
 * Note 1:
 *  The registered event handler will be called in (software) interrupt context
 *  and thus caller must ensure that all the constraints related to interrupt
 *  handlers are followed. For example, the event handler could just post the
 *  event to some listner and return immediately and the even would be processed
 *  in some different context.
 *
 * Note 2:
 *  The current implementation provides only one event per timer execution, that
 *  is, if there are more than one events queued up by PP before timer
 *  expiration, only the first queued event will be provided to the callback and
 *  next event(s) will be dispatched after subsequent timer expiration(s).
 *
 * Parameters:
 *  ptr_event_handler (IN) -    Pointer to event handler to be called for PPD
 *                              events.
 *
 * Return:
 *  0 on Success, -1 on Error.
 */
Int32 ti_ppd_register_event_handler (TI_PPD_EVENT_HANDLER ptr_event_handler);

/*
 * ti_ppd_deregister_event_handler -
 *
 * Description:
 *  This API is used to de-register Packet Processor event handler with the PPD.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  Always 0.
 */
Int32 ti_ppd_deregister_event_handler (void);

/*
 * ti_ppd_health_check -
 *
 * Description:
 *  This API is used to get the running status of PDSP and firmware.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 if the PDSPs and firmware are running properly.
 *  Non-zero on firmware/PDSP error condition :-
 *      0x01    - CPDSP error
 *      0x02    - MPDSP error
 *      0x03    - Both CPDSP and MPDSP in error
 *      -1      - PP System Failure
 *      <-1     - indicate corresponding command error code from
 *                firmware.
 *
 * Note 1:
 *  As per the PP firmware implementation, the PDSPs respond to the health
 *  check sequentially, i.e., first, CPDSP fills its status and propagates the
 *  command to MPDSP which then adds its own status and signals the host about
 *  information availability. Thus, the status check command may not return when
 *  a PDSP is in error. Also, the status of subsequent PDSPs is NOT
 *  checked when previous PDSP was unable to respond. In such cases error code
 *  "PP System Failure" (-1) is returned. This means error code "Both PDSPs in
 *  error" is not applicable in current implementation.
 *
 * Note 2:
 *  In case of error, this API waits for sufficiently long time (TODO: approx
 *  time) to flag a PDSP error.
 */
Int32 ti_ppd_health_check (void);

/*
 *                      QOS APIs
 */

/*
 * ti_ppd_qos_cluster_setup -
 *
 * Description:
 *  This API sets up a QOS cluster consisting of multiple QOS queues grouped
 *  together to provide multiple flows and priorities for specified egress
 *  device. Following information is required:
 *      -   Index of the cluster (0-3)
 *      -   Configuration for all the queues in the cluster and various QOS
            parameters for the cluster.
 *
 *  This API can also be used to modify a previously configured cluster.
 *
 * Precondition:
 *  -   ti_ppd_init
 *  -   If used to modify a previously enabled cluster, it must be disabled
 *      with ti_ppd_qos_cluster_disable.
 *
 * Parameters:
 *  clst_indx (IN)      - Index of the cluster to be configured
 *  clst_cfg (IN)       - Cluster configuration
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_qos_cluster_setup (Uint8 clst_indx, TI_PP_QOS_CLST_CFG* clst_cfg);

/*
 * ti_ppd_qos_cluster_enable -
 *
 * Description:
 *  This API enables specified QOS cluster.
 *
 * Precondition:
 *  -   ti_ppd_init
 *  -   The cluster must have been configured with ti_ppd_qos_cluster_setup.
 *
 * Parameters:
 *  clst_indx (IN)      - Index of the cluster to be configured
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_qos_cluster_enable (Uint8 clst_indx);

/*
 * ti_ppd_qos_cluster_disable -
 *
 * Description:
 *  This API disables specified QOS cluster.
 *
 * Precondition:
 *  -   ti_ppd_init
 *  -   The cluster must have been configured with ti_ppd_qos_cluster_setup.
 *
 * Parameters:
 *  clst_indx (IN)      - Index of the cluster to be configured
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_qos_cluster_disable (Uint8 clst_indx);

/*
 * ti_ppd_get_qos_q_stats -
 *
 * Description:
 *  This API provides statistics for specified QOS queue.
 *
 * Note:
 *  Currently there is no PP Firmware command to get QOS stats, so this API
 *  reads the QOS Queue block directly, thus the results may not reflect correct
 *  statistics at that time.
 *
 * Precondition:
 *  -   ti_ppd_init
 *  -   The specified queue must be part of a cluster configured
 *      ti_ppd_qos_cluster_setup.
 *
 * Parameters:
 *  qos_qnum (IN)       - Index of the QOS queue, offset from base queue number.
 *  stats (OUT)         - Pointer to the structure to hold the statistics.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_qos_q_stats (Uint8 qos_qnum, TI_PP_QOS_QUEUE_STATS *stats);

/*
 * ti_ppd_get_n_clear_qos_q_stats -
 *
 * Description:
 *  This API provides statistics for specified QOS queue. The statics are
 *  reset.
 *
 * Note:
 *  Currently there is no PP Firmware command to get QOS stats, so this API
 *  reads the QOS Queue block directly, thus the results may not reflect correct
 *  statistics at that time.
 *
 * Precondition:
 *  -   ti_ppd_init
 *  -   The specified queue must be part of a cluster configured
 *      ti_ppd_qos_cluster_setup.
 *
 * Parameters:
 *  qos_qnum (IN)       - Index of the QOS queue, offset from base queue number.
 *  stats (OUT)         - Pointer to the structure to hold the statistics.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_n_clear_qos_q_stats (Uint8 qos_qnum,
                                        TI_PP_QOS_QUEUE_STATS *stats);


/*
 * ti_ppd_get_version -
 *
 * Description:
 *  This API provides firmwares combined lineup version.
 *
 * Note:
 *  The version combining four numbers represents all firmwares
 *  together as single product.
 *
 * Precondition:
 *  -   ti_ppd_init
 *
 * Parameters:
 *  ver (OUT)    - Pointer to the structure to hold the version.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_version ( TI_PP_VERSION *ver );

/*
 * ti_ppd_set_qos_cluster_max_global_credit -
 *
 * Description:
 *  This API Set the max global credit of a Qos cluster.
 *
 *
 * Precondition:
 *  -   ti_ppd_init
 *
 * Parameters:
 *  byteCredit (IN)         - When true - set the maxCredit Bytes, false - set the maxCredit Pkts
 *  cluster_id (IN)         - The cluster to be set
 *  max_global_credit (IN)  - The credit value to set.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_set_qos_cluster_max_global_credit(int byteCredit, int cluster_id, int max_global_credit);

/*
 * ti_ppd_set_qos_queue_iteration_credit -
 *
 * Description:
 *  This API Set the iteration credit of a Qos queue.
 *
 *
 * Precondition:
 *  -   ti_ppd_init
 *
 * Parameters:
 *  byteCredit (IN)         - When true - set the maxCredit Bytes, false - set the maxCredit Pkts
 *  queue_id (IN)           - The queue to be set
 *  it_credit (IN)          - The iteration credit value to set.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_set_qos_queue_iteration_credit(int byteCredit, int queue_id, int it_credit);
/*
 * ti_ppd_set_qos_queue_max_credit -
 *
 * Description:
 *  This API Set the max credit of a Qos Queue.
 *
 *
 * Precondition:
 *  -   ti_ppd_init
 *
 * Parameters:
 *  byteCredit (IN)         - When true - set the maxCredit Bytes, false - set the maxCredit Pkts
 *  cluster_id (IN)         - The queue to be set
 *  max_global_credit (IN)  - The credit value to set.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_set_qos_queue_max_credit(int byteCredit, int queue_id, int max_credit);

Int32 ti_ppd_dispaly_session_info(int session_id);
Int32 ti_ppd_dispaly_qos_queue_info(int queue_id);
Int32 ti_ppd_dispaly_qos_cluster_info(int cluster_id);
#ifdef CONFIG_CABLE_VFE_COE
Int32 ti_ppd_dispaly_coepdsp_stats(void);
#endif


/*
 *                      PMON APIs - PDSP Status & Control
 */
#define TI_PP_CPDSP_ID    0
#define TI_PP_MPDSP_ID    1
#define TI_PP_QPDSP_ID    2

#define TI_PP_PDSP_STATE_HALTED     0
#define TI_PP_PDSP_STATE_RUNNING    1
#define TI_PP_PDSP_STATE_SLEEPING   2

/**************************************************************************
 * STRUCTURE NAME : TI_PP_PDSP_STATUS
 **************************************************************************
 * DESCRIPTION   :
 *  The structure contains various PDSP status information such as  PDSP state
 *  (Running/Halted/Sleeping), Program counter (1 cycle prior), Cycle count and
 *  Stall count (these counts are 64-bit values split as two 32-bit words),
 *  uptime (days, hours, minutes, seconds) etc.
 **************************************************************************/
typedef struct TI_PP_PDSP_STATUS
{
    int             state;              /* Running status of PDSP: running,
                                           halted or sleeping */
    unsigned short  prog_counter;       /* PDSP program counter 1 cycle prior to
                                           when status was sampled */
    unsigned int    cycle_cnt_hi;       /* Count of number of cycles PDSP is
                                           'Running' (high 32 bits) */
    unsigned int    cycle_cnt_lo;       /* Count of number of cycles PDSP is
                                           'Running' (low 32 bits) */
    unsigned int    stall_cnt_hi;       /* Count of number of cycles PDSP is
                                           stalled for instruction while
                                           'Running' (high 32 bits) */
    unsigned int    stall_cnt_lo;       /* Count of number of cycles PDSP is
                                           stalled for instruction while
                                           'Running' (low 32 bits) */
    unsigned short  uptime_days;        /* PDSP uptime */
    unsigned short  uptime_hrs;         /* PDSP uptime */
    unsigned short  uptime_mins;        /* PDSP uptime */
    unsigned short  uptime_secs;        /* PDSP uptime */

} TI_PP_PDSP_STATUS;

/*
 * ti_ppd_get_pdsp_status -
 *
 * Description: This API provides status of PDSP.
 *
 * Note:
 *  When PSM mode is enabled, this API avoids accessing PDSP registers and thus
 *  the program counter value is don't care in such case. Also, the PDSp state
 *  is reported as TI_PP_PDSP_STATE_HALTED.
 *
 * Precondition:
 *  -   ti_ppd_init
 *
 * Parameters:
 *  pdsp_id (IN)        - Id of PDSP to get stats of: CPDSP(0), MPDSP(1),
 *                        QPDSP(2).
 *  status (OUT)        - Pointer to the structure to hold PDSP status
 *                        information.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_pdsp_status (Uint8 pdsp_id, TI_PP_PDSP_STATUS *status);

#define TI_PP_PDSPCTRL_HLT      0
#define TI_PP_PDSPCTRL_STEP     1
#define TI_PP_PDSPCTRL_FREERUN  2
#define TI_PP_PDSPCTRL_RESUME   3
#define TI_PP_PDSPCTRL_RST      4
#define TI_PP_PDSPCTRL_PSM      5
/*
 * ti_ppd_pdsp_control -
 *
 * Description: This API provides interface to control PDSPs. Following
 * operations are supported :-
 *  TI_PP_PDSPCTRL_HLT
 *      HALT pdsp execution
 *  TI_PP_PDSPCTRL_STEP
 *      Set PDSP Single step mode. This option halts the PDSP and successive
 *      RESUMEs are carried as single steps.
 *  TI_PP_PDSPCTRL_FREERUN
 *      Set free running mode, i.e., disable single step. PDSP execution is
 *      implicitly RESUMEd as a result of this command.
 *  TI_PP_PDSPCTRL_RESUME
 *      RESUME pdsp execution
 *  TI_PP_PDSPCTRL_RST
 *      RESET PDSP and start execution from specified program counter. The
 *      16-bit program counter shoule be passed by ctl_data pointer.
 *  TI_PP_PDSPCTRL_PSM
 *      Enable or Disable PSM mode. ctl_data should be passed as pointer to
 *      boolean (32-bit integer) flag indicating desired enable (!0) or disable
 *      (0) status of PSM. Note that pdsp_id value is ignored for this option.
 *
 * Note:
 *  1) Setting option TI_PP_PDSPCTRL_STEP just sets the PDSP in single step
 *  mode and halts its execution, actual single stepping should be performed by
 *  calling this API with TI_PP_PDSPCTRL_RESUME option per step till free
 *  running is enabled explicitly with option TI_PP_PDSPCTRL_FREERUN
 *  single step or halting the pdsp
 *  2) When PDSP is halted or set for single step, PMON counting for that PDSP
 *  is disabled. Thus the corresponding uptime values for pdsp and in case the
 *  controlled pdsp is CPDSP, the session up time values are no longer updated.
 *  PMON counting is re-enabled when PDSP execution is resumed.
 *  3) PMON counters are cleared for the pdsp when it is reset and the up time
 *  counting begins from 0. In case of CPDSP reset, the session counters are
 *  also cleared.
 *  4) Enabling power saving mode with TI_PP_PDSPCTRL_PSM implicitly halts all
 *  three PDSPs. Disabling PSM resumes the PDSPs.
 *  5) __IMPORTANT__ When PSM mode is enabled, no other PDSP control option is
 *  supported apart from TI_PP_PDSPCTRL_PSM. This cavet is introduced on
 *  assumption that generally TI_PP_PDSPCTRL_PSM(enable) will be followed by
 *  gating the clock to PP PDSPs (eg., using LPSC) and thus thus PDSP registers
 *  may not be not accessible during PSM mode. It is also assumed PDSPs are
 *  enabled (clock on) before this API is called with
 *  TI_PP_PDSPCTRL_PSM(disable), thus enabling all PDSP controls.
 *
 * Precondition:
 *  -   ti_ppd_init
 *
 * Parameters:
 *  pdsp_id (IN)    - Id of PDSP to control: CPDSP(0), MPDSP(1), QPDSP(2).
 *  ctl_op (IN)     - Eiter of the PDSP control options as explained above.
 *  ctl_data (IN)   - Pointer to data corresponding the pdsp control option.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_pdsp_control (Uint8 pdsp_id, Uint32 ctl_op, Ptr ctl_data);

/*
 * ti_ppd_get_ses_age -
 *
 * Description: This API returns age (uptime) of the specified session.
 *
 * Note:
 *  This API assumes the specified session being active session with no
 *  timeouts. Thus the session age is always calculated since the time of
 *  session  creation.
 *
 * Precondition:
 *  -   ti_ppd_create_session
 *
 * Parameters:
 *  session_handle (IN) - Index of the session to return the age of
 *  days (OUT)          - Pointer to hold number of days the session is up.
 *  hrs (OUT)           - Pointer to hold number of hrs the session is up.
 *  mins (OUT)          - Pointer to hold number of mins the session is up.
 *  secs (OUT)          - Pointer to hold number of secs the session is up.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_get_ses_age (Uint8 session_handle, Uint16* days,
                            Uint16* hrs, Uint16* mins, Uint16* secs);

/*
 *  ti_ppd_sram_test -
 *
 *  Description :Test Scratch RAM for all PDSPs
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 ti_ppd_sram_test(void);

/* TODO: PDSP control API */

Int32 ti_ppd_set_Timer (Uint8 StartStop);
#endif /* !_INCLUDE_TI_PPD_H */
