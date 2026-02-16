/* 
 * pp_qos_p7.h
 * Description:
 * QoS Manager driver APIs
 
 
  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016-2017 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation. All rights reserved.

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

#ifndef PP_QOS_P7_H
#define PP_QOS_P7_H
/************************************************/
/*                    Includes                  */
/************************************************/

/************************************************/
/*                    Defines                   */
/************************************************/

#define PP_QOS_MAX_CLUSTERS          (64)
#define PP_QOS_MAX_QUEUES            (192)
#define PP_QOS_MAX_QUEUES_IN_CLUSTER AVALANCHE_PP_QOS_CLST_MAX_QCNT

#define PP_QOS_MAX_QUEUE_INDEX       (PP_QOS_MAX_QUEUES   - 1)
#define PP_QOS_MAX_CLUSTER_INDEX     (PP_QOS_MAX_CLUSTERS - 1)

#define PP_QOS_Q_REALTIME_FLAG       (1<<0)
#define PP_QOS_Q_DROP_EXCESSIVE_FLAG (1<<1)
#define PP_QOS_Q_QDA_DISABLE_FLAG    (1<<2)
#define PP_QOS_Q_LIMITED_FLAG        (1<<3)
#define PP_QOS_Q_NO_QOS_FLAG         (1<<4)
#define PP_QOS_Q_WFQ_ENABELD_FLAG    (1<<5)


#define PP_QOS_ITERATION_TICK_USEC   (25)
#define PP_QOS_ITERATION_PER_SECOND  (1000000 / PP_QOS_ITERATION_TICK_USEC)

#define INVALID_QDA_THREAD_ID        (32)
#define INVALID_QDA_QUEUE            (255)
#define PP_QOS_INVALID_CLUSTER       (255)
#define PP_QOS_DEFAULT_PRIORITY      (0)

#define PP_QOS_FW_QCFG_BLK_BASE_PHY  (0xF3E10200)
#define PP_QOS_FW_CLST_BLK_BASE_PHY  (0xF3E12200)

#ifndef CONFIG_LTQ_PPA_QOS
/* Host QoS Queues */
#define DOCSIS_HIGH_Q              191
#define DOCSIS_MED_HIGH_Q          190
#define DOCSIS_MED_Q               189
#define DOCSIS_MED_LOW_Q           188
#define DOCSIS_LOW_Q               187
#define LAN_HIGH_Q                 186
#define LAN_MED_Q                  185
#define LAN_LOW_Q                  184
#endif

#define PP_QOS_FW_DIV_SUM_BASE_PHY   (0xF3E13600)                /* 2 Bytes * 255 entries*/
#define PP_QOS_FW_MAX_SUM_ENTRIES    (255)                       /* The MAX value for sum of weights of the active queues in WFQ */
#define PP_QOS_MAX_WFQ_CLUSTERS      (5)                         /* Currently only 1 cluster can be from WFQ type */

/* Rate(in Mbps) * 1M) / iteration per second / 8(bits in byte) */
#define PP_QOS_MEGA_BIT_PER_SEC_TO_IT_CREDIT_BYTES(x)    ((Uint32)(((Uint64)(x * 1000 * 1000)) / PP_QOS_ITERATION_PER_SECOND / 8 ))
#define PP_QOS_PPS_TO_IT_CREDIT_PACKETS(x)               ((Uint16)(x / PP_QOS_ITERATION_PER_SECOND))

/************************************************/
/*                 Structures                   */
/************************************************/
/* QoS manager return code */
typedef enum
{
    PP_QOS_RC_SUCCESS,
    PP_QOS_RC_FAILURE,
    PP_QOS_RC_INVALID_PARAM,
    PP_QOS_RC_OUT_OF_MEMORY,
    PP_QOS_RC_OBJECTS_STARVATION
} PP_QOS_MGR_RET_e;

/* QoS manager Qos-profile */
typedef struct
{
    PP_QOS_SCH_MODE_e sch_mode;                   /* Strict/WFQ */
    Uint8             num_of_priorities;          /* Number of priorities for strict scheduling or Number of queues in WFQ scheduling */
    Uint8             vpid_id;                    /* VPID ID */
    Uint8             grr_bitmask;                /* GRR bitmask - For GRR scheduling only */
    Uint8             profile_private_identifier;
    Uint32            egress_port_queue;          /* Egress queue number */
    Uint32            egress_queue_depth_packets; /* Egress queue depth (packets) */
    Uint32            egress_queue_depth_bytes;   /* Egress queue depth (bytes) */
    Uint32            ingress_queue_depth_packets;/* Ingress queue depth (packets) */
    Uint32            ingress_queue_depth_bytes;  /* Ingress queue depth (bytes) */
    Uint32            port_rate_limit_Mbps;       /* Rate limit (in Mbps)*/
    Uint32            port_pps_limit;             /* PPS limit */
    Uint16            queues_weights[AVALANCHE_PP_QOS_CLST_MAX_QCNT]; /* If scheduling mode is WFQ each queue has weight */
} PP_QOS_PROFILE_t;

typedef struct
{
    Uint8             pid_id;
    Uint8             num_of_priorities;          /* Number of priorities */
    Uint32            egress_port_queue;          /* Egress queue number */
    Uint32            egress_queue_depth_packets; /* Egress queue depth (packets) */
    Uint32            egress_queue_depth_bytes;   /* Egress queue depth (bytes) */
    Uint32            port_rate_limit_Mbps;       /* Rate limit (in Mbps)*/
    Uint32            port_pps_limit;             /* PPS limit */
}
PP_QOS_PORT_SHAPER_t;

/* QoS manager statistics */
typedef struct
{
    Uint8    active_shared_clusters;
    Uint8    active_host_clusters;
    Uint8    active_private_clusters[AVALANCHE_PP_MAX_PID];
    Uint8    private_clusters_free_pool_cnt[AVALANCHE_PP_MAX_PID];
    Uint8    active_shared_queues;
    Uint8    active_host_queues;
    Uint8    active_private_queues[AVALANCHE_PP_MAX_PID];
    Uint8    private_queues_free_pool_cnt[AVALANCHE_PP_MAX_PID];
    Uint32   clusters_starvation_shared;
    Uint32   queues_starvation_shared;
    Uint32   shapers_starvation;
    Uint32   active_shapers;
    Uint32   wfq_clusters;
}
PP_QOS_DB_Counters_t;

typedef struct
{
    Uint64      frwd_pkts;            /* Number of packets forwarded to the Egress Queue */
    Uint64      drp_pkts;             /* Number of packets dropped due to congestion */
}
PP_QOS_MIB_STATS_t;

/* Cluster Flags, relevent for PP_QOS_FW_CLST_CFG_t and for PP_QOS_FW_WFQ_CLST_CFG_t */
#define PP_QOS_CLST_WFQ_FLAG    (1<<0)

typedef struct
{
    Uint32                      global_credit_bytes;            /* The amount of global credit bytes available to the next Qos queue in the cluster */

    Uint16                      global_credit_pkts;             /* The amount of global credit packet available to the next Qos queue in the cluster */
    Uint16                      max_global_credit_pkts;         /* The max amount of global credit pkts allowed carring over to the next queue. */

    Uint32                      max_global_credit_bytes;        /* The max amount of global credit bytes allowed carring over to the next queue. */

    Uint16                      qda_queue;                      /* The queue which we activated QDA on. INVALID_QDA_QUEUE indicates that QDA wasn't activated on any on the cluster queues */
#define                         grr_1st_in_group_bm qda_queue   /* For Grouped RR Cluster only (Grouped RR cluster does't support QDA) */

#if defined(__BIG_ENDIAN_BITFIELD)
    Uint8       egr_q_cnt:4,                                    /* The total number of egress queues sampled to obtain the egress queue congestion estimation*/
                qos_q_cnt:4;                                    /* The number of QOS queues in the cluster (1 to 9)      */
#elif defined (__LITTLE_ENDIAN_BITFIELD)
    Uint8       qos_q_cnt:4,                                    /* The total number of egress queues sampled to obtain the egress queue congestion estimation */
                egr_q_cnt:4;                                    /* The total number of egress queues in WFQ its always 1 */
#endif

    Uint8                       flags;                          /* Bit 0 : Set if the cluster is working in WFQ scheduling mode. cleared when its strict scheduling mode */

    Uint8                       qos_q3;                         /* The queue index (0 to 127) of each QOS queue in the cluster listed in priority order */
    Uint8                       qos_q2;
    Uint8                       qos_q1;
    Uint8                       qos_q0;

    Uint8                       qos_q7;
    Uint8                       qos_q6;
    Uint8                       qos_q5;
    Uint8                       qos_q4;

    Uint16                      egr_q1;                     /* The Queue index of every egress queue enumerated in Egress Queue Count */
#define                         it_counter egr_q1           /* For Host QoS Cluster only (Host QoS cluster does't support egress congested algo) */
    Uint16                      egr_q0;
#define                         it_divider egr_q0           /* For Host QoS Cluster only (Host QoS cluster does't support egress congested algo) */

    Uint16                      egr_q3;
    Uint16                      egr_q2;

    Uint32                      egr_congst_thrsh_bytes1;	/* Egress Congestion Threshold bytes point 1  */
    Uint32                      egr_congst_thrsh_bytes2;	/* Egress Congestion Threshold bytes point 2  */
    Uint32                      egr_congst_thrsh_bytes3;    /* Egress Congestion Threshold bytes point 3  */
    Uint32                      egr_congst_thrsh_bytes4;	/* Egress Congestion Threshold bytes point 4  */

    Uint16                      egr_congst_thrsh_pkts1;     /* Egress Congestion Threshold Packets point 4  */
    Uint16                      egr_congst_thrsh_pkts2;     /* Egress Congestion Threshold Packets point 3  */
    Uint16                      egr_congst_thrsh_pkts3;     /* Egress Congestion Threshold Packets point 2  */
    Uint16                      egr_congst_thrsh_pkts4;     /* Egress Congestion Threshold Packets point 1  */

    Uint16                      last_qda_cmd_pkt;
    Uint16                      last_qda_cmd_bytes;

    Uint8                       shaper_clst3;
    Uint8                       shaper_clst2;
    Uint8                       shaper_clst1;
    Uint8                       shaper_clst0;

}
PP_QOS_FW_CLST_CFG_t;

typedef struct
{
    Uint16 egr_q;                    /* The Queue index of the forwarding queue */
    Uint8  flags;                    /* Specifies how the frames in the QOS queue should be handled */
    Uint8  QDAThreadId;              /* Indicates which QDA thread id is working on that queue. INVALID_QDA_THREAD_ID means that no thread is bound to that queue */

    Uint32 iteration_credit_bytes;   /* The amount of byte credit that the queue receives every 25us. */
    Uint32 total_credit_bytes;       /* The total amount of forwarding byte credit that the queue is currently holding */
    Uint32 max_credit_bytes;         /* The max amount of forwarding byte crerdit that the queue is allowed to hold at the end of the 25US iteration */

    Uint16 iteration_credit_pkts;    /* The amount of packet credit that the queue receives every 25us. */
    Uint16 total_credit_pkts;        /* The total amount of forwarding pkt credit that the queue is currently holding */

    Uint16 max_credit_pkts;          /* The max amount of forwarding packet crerdit that the queue is allowed to hold at the end of the 25US iteration */
    Uint16 congst_thrsh_pkts;        /* The size in packets at which point the Qos queue is considered to be congested */

    Uint32 congst_thrsh_bytes;       /* The size in bytes at which point the Qos queue is considered to be congested */
    Uint32 w7;                       /* Reserved */

}
PP_QOS_FW_QUEUE_t;

/*********************************************************************************************************************************/ 
/*                                                                                                                               */         
/*                                           WFQ Structures                                                                      */
/*                                                                                                                               */  
/*********************************************************************************************************************************/
typedef struct
{

    Uint32                      wfq_info;                   /* Used in the FW for internal processing */

    Uint32                      credit_per_iteration;       /* The Max bytes that can be transmitted per iteration. Automatically configured from the line rate and iteration rate */

    Uint16                      leftover_credit;            /* Temporary variable, holding the total credit of all queues that are not utilizing their whole credit */
    Uint16                      act_Qs_weight_sum;          /* Temporary variable, holding sum of all queues that are active within some calculation scope. */

    Uint16                      qda_queue;                  /* The queue which we activated QDA on. INVALID_QDA_QUEUE indicates that QDA wasn't activated on any on the cluster queues */
    
#if defined(__BIG_ENDIAN_BITFIELD)
    Uint8       egr_q_cnt:4,              /* The total number of egress queues in WFQ its always 1 */
                qos_q_cnt:4;              /* The number of QOS queues in the cluster (1 to 9)      */
#elif defined (__LITTLE_ENDIAN_BITFIELD)
    Uint8       qos_q_cnt:4,              /* The number of QOS queues in the cluster (1 to 9) */
	            egr_q_cnt:4;              /* The total number of egress queues in WFQ its always 1 */
#endif

    Uint8                       flags;                      /* Bit 0 : Set if the cluster is working in WFQ scheduling mode. cleared when its strict scheduling mode */

    Uint8                       qos_q3;                     /* The queue index (0 to 127) of each QOS queue in the cluster listed in priority order */
    Uint8                       qos_q2;
    Uint8                       qos_q1;
    Uint8                       qos_q0;

    Uint8                       qos_q7;
    Uint8                       qos_q6;
    Uint8                       qos_q5;
    Uint8                       qos_q4;

    Uint16                      egr_q1;                     /* The Queue index of every egress queue enumerated in Egress Queue Count only 1 Egress queue is valid in WFQ */
    Uint16                      egr_q0;

    Uint16                      egr_q3;
    Uint16                      egr_q2;

    Uint32                      egress_Q_pending_bytes;	  /* Number of pending bytes in the output queue  */
    Uint32                      reserve[7];               /* Not used for WFQ cluster TBD can indicate that this is wfq cluster  */
    
}
PP_QOS_FW_WFQ_CLST_CFG_t;
	

typedef struct
{
    Uint16 egr_q;                             /* The Queue index of the forwarding queue */
    Uint8  flags;                             /* Specifies how the frames in the QOS queue should be handled */                                     
    Uint8  QDAThreadId;                       /* Indicates which QDA thread id is working on that queue. INVALID_QDA_THREAD_ID means that no thread is bound to that queue */

    Uint16 queueWeight;                       /* The Weight given to queue i W(i)*/
    Uint16 creditBytePerQueue;                /* Credit byte per queue per iteration (CPQP(i))   This is the relative number of bytes per iteration for queue based on its w(i) */

    Uint32 queuePendingPkts;                  /* The number of pending pkts in the qos queue */
    Uint32 queuePendingBytes;                 /* The number of pending bytes in the qos queue */

    Int16 startCurrentCredit;                 /* The current credit for queue I in bytes at the start of the iteration */
    Int16 endCurrentCredit;                   /* The current credit for queue I in bytes at the end of the iteration */

    Uint16 queueExtraCredit;                  /* The temporary variable that holds extra credit per queue taken from the leftover Credit */
    Uint16 congstThrshPkts;                   /* The size in packets at which point the Qos queue is considered to be congested */

    Uint32 congstThrshBytes;                  /* The size in bytes at which point the Qos queue is considered to be congested */
    Uint32 w7;                                /* Reserved */

}
PP_QOS_FW_WFQ_QUEUE_t;
#ifdef __KERNEL__
/* **************************************************************************************** */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                      KERNEL only Stuff                                                   */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/* **************************************************************************************** */


/************************************************/
/* QoS manager APIs                             */
/************************************************/
/**************/
/* Common     */
/**************/
/**
 *  pp_qos_mngr_init - Initialized the QoS manager DB
 *  
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_mngr_init(void);
/**
 *  pp_qos_set_basic_profile - This API should be called while
 *  QoS profile is created for NI
 *  basic profile is 1 cluster with 1 egress queue.
 *  @qos_profile: Pointer to qos profile to set
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_set_basic_profile(PP_QOS_PROFILE_t *qos_profile);
/**
 *  pp_qos_set_wfq_profile - This API should be called while QoS
 *  profile is created for NI.
 *  wfq profile is 1 cluster with 1 egress queue.
 *  WFQ is the scheduling type of the QoS configuration.
 *  @qos_profile: Pointer to qos profile to set
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_set_wfq_profile(PP_QOS_PROFILE_t *qos_profile);

/**
 *  pp_qos_is_profile_registered - This API return if the QoS
 *  profile is alreadu exist int the QoS manager
 *  @vpid_id: Pointer to qos profile to set
 *  @profile_private_identifier: identifier for profile
 *  @isSupported: pointer to return id profile is registered
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_is_profile_registered(Uint8 vpid_id, Uint16 profile_private_identifier, Bool *isRegistered);
/**
 *  pp_qos_delete_profile - This API should be called when NI
 *  need to delete QoS profile
 *  @vpid_id: Profile's VPID
 *  @profile_private_identifier: Profile's private identifier
 *  @profile_stats: To return the profile statistics before the
 *                profile will deleted.
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_delete_profile(Uint8 vpid_id, Uint16 profile_private_identifier, PP_QOS_MIB_STATS_t *profile_stats);
/**
 *  pp_qos_delete_all_profiles - This API should be called while
 *  NI is going down
 *  @vpid_id: VPID ID
 *  @vpid_stats: To return the vpid statistics before the
 *             profiles will deleted.
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_delete_all_profiles(Uint8 vpid_id, PP_QOS_MIB_STATS_t * vpid_stats);
/**
 *  pp_qos_db_get_counters - Get the QoS DB statistics
 *  @qos_db_stats: Pointer to QoS DB statistics to set
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_db_get_counters(PP_QOS_DB_Counters_t *qos_db_stats);
/**
 *  pp_qos_enable_psm - Enter to psm mode
 *  
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_enable_psm(void);
/**
 *  pp_qos_disable_psm - Exit from psm mode
 *  
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_disable_psm(void);
/**
 *  pp_qos_set_port_shaper - Config QoS shaper
 *  @port_shaper: Port shaper configuration
 *  @shaper_id: Pointer to return the new shaper ID
 *  @shaper_in_q: Pointer to return the new ingress queues
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_set_port_shaper(PP_QOS_PORT_SHAPER_t * port_shaper, Uint8 * shaper_id, Uint8 * shaper_in_q);
/**
 *  pp_qos_free_port_shaper - Free/Delete QoS shaper
 *  @shaper_id: Shaper ID
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_free_port_shaper(Uint8 shaper_id);
/**
 *  pp_qos_shaper_is_active - Check if shaper is active
 *  @shaper_id: Shaper ID
 *  @is_active: Pointer to return if active or not
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_shaper_is_active(Uint8 shaper_id, Bool *is_active);
/**
 *  pp_qos_get_port_shaper_cluster_config - Get the port shaper
 *  cluster configuration
 *  @shaper_id: Shaper ID
 *  @clst_cfg: Pointer to return the cluster configuration
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_port_shaper_cluster_config(Uint8 shaper_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg);
PP_QOS_MGR_RET_e pp_qos_set_port_shaper_cluster_config(Uint8 shaper_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg);

/**************/
/* Cluster    */
/**************/
/**
 *  pp_qos_cluster_is_active - Check if cluster is active
 *  @phy_cluster_id: Cluster ID
 *  @is_active: Pointer to return if active or not
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_cluster_is_active(Uint8 phy_cluster_id, Bool *is_active);
/**
 *  pp_qos_set_cluster_line rate - Configure cluster line rate
 *  @phy_cluster_id: Cluster ID
 *  @line_rate_bps: line rate in Mbps
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_set_cluster_line_rate (Uint8 phy_cluster_id, Uint32 line_rate_Mbps);
/**
 *  pp_qos_set_cluster_config - Config QoS cluster
 *  @phy_cluster_id: Cluster ID
 *  @vpid_id: VPID ID
 *  @clst_cfg: Pointer to requested cluster
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_set_cluster_config(Uint8 phy_cluster_id, Uint8 vpid_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg);
/**
 *  pp_qos_alloc_cluster - Alloc/Create QoS cluster
 *  @vpid_id: VPID ID
 *  @phy_cluster_id: Pointer to return the cluster ID
 *  @profile_private_identifier: unique identifier for profile
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_alloc_cluster(Uint8 vpid_id, Uint16 profile_private_identifier, Uint8 *phy_cluster_id, PP_QOS_SCH_MODE_e sch_mode);
/**
 *  pp_qos_free_cluster - Free/Delete QoS cluster
 *  @phy_cluster_id: Cluster ID
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_free_cluster(Uint8 phy_cluster_id);
/**
 *  pp_qos_get_cluster_config - Get the cluster configuration
 *  @phy_cluster_id: Cluster ID
 *  @clst_cfg: Pointer to return the cluster configuration
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_cluster_config(Uint8 phy_cluster_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg);
/**
 *  pp_qos_get_queue_and_cluster_id_by_priority - Get the
 *  cluster and queue ID for given vpid identifier and Q
 *  priority.
 *  @vpid_id: VPID ID
 *  @profile_private_identifier: Private identifier for profile
 *  @priority: queue priority - This Uint8 field represent the
 *           priority of the qos Q, 0 is the lowest priority,
 *           the max priority is the number of qos queues in the
 *           cluster.
 *  @phy_cluster_id: Pointer to return the cluster ID
 *  @phy_queue_id: Pointer to return the queue ID
 *  
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_queue_and_cluster_id_by_priority(Uint8 vpid_id, Uint16 profile_private_identifier, Uint8 priority, Uint8 *phy_cluster_id, Uint8 *phy_queue_id);
/**
 *  pp_qos_get_queue_id_by_cluster_and_priority - Get the queue
 *  ID by given cluster and priority.
 *  @phy_cluster_id: Cluster ID
 *  @priority: queue priority - This Uint8 field represent the
 *           priority of the qos Q, 0 is the lowest priority,
 *           the max priority is the number of qos queues in the
 *           cluster.
 *  @phy_queue_id: Pointer to return the queue ID
 *  
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_queue_id_by_cluster_and_priority(Uint8 phy_cluster_id, Uint8 priority, Uint8 *phy_queue_id);
/**
 *  pp_qos_get_cluster_by_identifier - Get the cluster
 *  configuration by given VPID and identifier.
 *  @profile_private_identifier: Private identifier
 *  @vpid_id: VPID ID
 *  @phy_cluster_id: Pointer to return the cluster ID
 *  @clst_cfg: Pointer to return the cluster configuration
 *  
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_cluster_by_identifier(Uint16 profile_private_identifier, Uint8 vpid_id,  Uint8 *phy_cluster_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg);
/**
 *  pp_qos_get_cluster_stats - Get QoS statistics for cluster
 *  @phy_cluster_id: Cluster ID
 *  @cluster_stats: Pointer to return the cluster
 *                statistics
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_cluster_stats(Uint8 phy_cluster_id, PP_QOS_MIB_STATS_t *cluster_stats);
/**
 *  pp_qos_get_cluster_by_queue_id - Get the cluster
 *  configuration by a given queue id
 *  @phy_queue_id: Queue ID
 *  @phy_cluster_id: Pointer to return the cluster id
 *  @clst_cfg: Pointer to return the cluster
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_cluster_by_queue_id(Uint8 phy_queue_id, Uint8 *phy_cluster_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg);
/**
 *  pp_qos_get_cluster_by_vpid_and_schedule_type - Get the
 *  cluster configuration by given VPID and scheduling type.
 *  @vpid_id: VPID ID
 *  @sch_mode: QOS Scheduling Mode
 *  @phy_cluster_id: Pointer to return the cluster ID
 *  @clst_cfg: Pointer to return the cluster configuration
 *  
 *  Returns PP_QOS_RC_SUCCESS if found a cluster.
 */
PP_QOS_MGR_RET_e pp_qos_get_cluster_by_vpid_and_schedule_type(Uint8 vpid_id, PP_QOS_SCH_MODE_e sch_mode, Uint8 *phy_cluster_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg);
/**************/
/* Queue      */
/**************/
/**
 *  pp_qos_add_queue_to_cluster - Adds queue to cluster
 *  @phy_cluster_id: Cluster ID
 *  @user_priority: User priority of the queue
 *  @queue_cfg: Pointer to AVALANCHE_PP_QOS_QUEUE_t
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_add_queue_to_cluster(Uint8 phy_cluster_id, AVALANCHE_PP_QOS_QUEUE_t *queue_cfg, Uint8 user_priority);
/**
 *  pp_qos_del_queue_by_cluster_and_user_prio - Deletes queue
 *  from cluster
 *  @phy_cluster_id: Cluster ID
 *  @user_priority: User priority of the queue
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_del_queue_by_cluster_and_user_prio(Uint8 phy_cluster_id, Uint8 user_priority);

PP_QOS_MGR_RET_e pp_qos_get_queue_and_cluster_id_by_user_priority(Uint8 vpid_id, Uint16 profile_private_identifier, Uint8 user_priority,
																	      Uint8 *phy_cluster_id, Uint8 *phy_queue_id, Uint8 *pp_priority);

/**
 *  pp_qos_get_highest_or_lowest_queue_by_cluster - Get the
 *  Highest or lowest queue in the cluster 
 *  @phy_cluster_id: Cluster Id
 *  @highest: if set the Highest queue will be returned if not
 *          set the lowest queue will be returned
 *  @phy_queue_id: will be set with the queue num that was found
 *  @queue_cfg: The highest or lowest queue configuration 
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_highest_or_lowest_queue_by_cluster( Uint8 phy_cluster_id, Bool highest, Uint8 *phy_queue_id, AVALANCHE_PP_QOS_QUEUE_t *queue_cfg, Uint8 *user_priority);

/**
 *  pp_qos_queue_is_active - Check if queue is active
 *  @phy_queue_id: Queue ID
 *  @is_active: Pointer to return if active or not
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_queue_is_active(Uint8 phy_queue_id, Bool *is_active);
/**
 *  pp_qos_alloc_queue - Alloc/Create QoS queue
 *  @phy_cluster_id: Cluster ID
 *  @phy_queue_id: Pointer to return the queue ID
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_alloc_queue(Uint8 phy_cluster_id, Uint8 *phy_queue_id);
/**
 *  pp_qos_free_queue - Free/Delete QoS queue
 *  @phy_queue_id: Queue ID
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_free_queue(Uint8 phy_queue_id);
/**
 *  pp_qos_get_strict_queue_config - Get the queue configuration
 *  @phy_queue_id: Queue ID
 *  @queue_cfg: Pointer to return the queue configuration
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_queue_config(Uint8 phy_queue_id, AVALANCHE_PP_QOS_QUEUE_t *queue_cfg);
/**
 *  pp_qos_modify_queue_weight - Change the queue weight
 *  @phy_queue_id: Queue ID
 *  @q_weight: the new weight for the queue
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_modify_queue_weight(Uint8 phy_queue_id, Uint32 q_weight);

PP_QOS_MGR_RET_e pp_qos_update_queue_it_credits(Uint8 phy_queue_id, Uint32 pkt_credit, Uint32 bytes_credit);
/**
 *  pp_qos_modify_active_queue - Modify properties for an active
 *  queue
 *  @q_cfg: Pointer to queue new configuration
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_modify_active_queue(AVALANCHE_PP_QOS_QUEUE_t *modified_q_cfg); /* Modify properties for an active queue. */
/**
 *  pp_qos_get_queue_stats - Get QoS queue statistics
 *  @qos_qnum: Queue ID
 *  @stats: Pointer to return the queue statistics
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_queue_stats(Uint32 qos_qnum, PP_QOS_MIB_STATS_t *q_stats);
/**
 *  pp_qos_inc_session_cnt_for_queue - Increment sessions
 *  counter for QoS queue
 *  @phy_queue_id: Queue ID
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_inc_session_cnt_for_queue(Uint8 phy_queue_id);
/**
 *  pp_qos_dec_session_cnt_for_queue - Decrement sessions
 *  counter for QoS queue
 *  @phy_queue_id: Queue ID
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_dec_session_cnt_for_queue(Uint8 phy_queue_id);
/**
 *  pp_qos_set_queue_shaper - Limit queue to fixed rate (in
 *  MegaBitPerSec)
 *  @phy_queue_id: Queue ID
 *  @rate_in_mega_bits: Rate in MegaBitPerSec
 * 
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_set_queue_shaper(Uint8 phy_queue_id, Uint32 rate_in_mega_bits);
/**************/
/* VPID       */
/**************/
/**
 *  pp_qos_vpid_is_active - Check if VPID is active (active in the QoS)
 *  @vpid_id: VPID ID
 *  @is_active: Pointer to return if active or not
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */ 
PP_QOS_MGR_RET_e pp_qos_vpid_is_active(Uint8 vpid_id, Bool *is_active);
/**
 *  pp_qos_is_vpid_registered - Return True if the given VPID is
 *  currently supported by the QoS
 *  @vpid_id: VPID ID
 *  @isRegistered: Pointer to return if the VPID is active
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_is_vpid_registered(Uint8 vpid_id, Bool *isRegistered);
/**
 *  pp_qos_vpid_set_qos_mode - Set the QOS clusters of a VPID to
 *  do Qos or not
 *  @vpid_id: VPID ID
 *  @vpidQosMode: If the requsted mode is QoS/no-QoS
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_vpid_set_qos_mode(Uint8 vpid_id, Bool vpidQosMode);
/**
 *  pp_qos_get_vpid_config - Get the vpid clusters configuration
 *  @phy_cluster_id: Cluster ID
 *  @clst_cfg_arr: Pointer to array of clusters (in size of
 *           PP_QOS_MAX_CLUSTERS)
 *  @clusters_cnt: in -> Pointer to MAX size of clst_cfg array.
 *                 out -> Pointer to return the number of
 *               clusters thet filled by the function.
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_vpid_config(Uint8 vpid_id, AVALANCHE_PP_QOS_CLST_CFG_t *clst_cfg_arr, Uint8 *clusters_cnt);
/**
 *  pp_qos_get_vpid_stats - Get QoS statistics for VPID
 *  @vpid_id: VPID ID
 *  @vpid_stats: Pointer to return the vpid statistics
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_vpid_stats(Uint8 vpid_id, PP_QOS_MIB_STATS_t *vpid_stats);
/**
 *  pp_qos_get_profile_stats - Get QoS statistics for
 *  Profile
 *  @vpid_id: VPID ID
 *  @profile_private_identifier: Private identifier
 *  @profile_stats: Pointer to return the profile statistics
 *
 *  Returns PP_QOS_RC_SUCCESS if pass.
 */
PP_QOS_MGR_RET_e pp_qos_get_profile_stats(Uint8 vpid_id, Uint16 profile_private_identifier, PP_QOS_MIB_STATS_t *profile_stats);

#endif /* __KERNEL__ */

#endif /* PP_QOS_P7_H */

