/*
 *
 * csl_switchPrvt.h
 * Description: 
 * General CSL API definitions
 *
 * Copyright (c) 2007-2009 Atheros Communications, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * Includes Intel Corporation's changes/modifications dated: May 2011.
 * Changed/modified portions - Copyright © 2011, Intel Corporation.   
 *
 */


#ifndef __SWITCH_PRVT_H__
#define __SWITCH_PRVT_H__

#include "ddc_switch.h"

/* Internal enumeration for the statistic counters */

typedef enum
{
       InBytes = 0,
       InBytesHigh,
       InUnicastPkts,
       InUnicastPktsHigh,
       InMulticastPkts,
       InMulticastPktsHigh,
       InBroadcastPkts,
       InBroadcastPktsHigh,
       InDiscardPkts,
       InErrorPkts,
       InUnknownProtPkts,
       OutBytes,
       OutBytesHigh,
       OutUnicastPkts,
       OutUnicastPktsHigh,
       OutMulticastPkts,
       OutMulticastPktsHigh,
       OutBroadcastPkts,
       OutBroadcastPktsHigh,
       OutDiscardPkts,
       OutErrorPkts,
       ethAlignmentErrors,
       ethFCSErrors,
       ethSingleCollisions,
       ethMultipleCollisions,
       ethSQETestErrors,
       ethDeferredTxFrames,
       ethLateCollisions,
       ethExcessiveCollisions,
       ethInternalMacTxErrors,
       ethCarrierSenseErrors,
       ethTooLongRxFrames,
       ethInternalMacRxErrors,
       ethSymbolErrors

} SWITCH_STAT_E;


/* Operation type */

typedef enum
{
    OPER_STATUS,
    OPER_LINK,
    OPER_DUPLEX,
    OPER_DUPLEX_SET,
    OPER_SPEED,
    OPER_SPEED_SET,
    OPER_FC,
    OPER_AUTO,
    OPER_INGRESS,
    OPER_INGRESS_OFF,
    OPER_PORT_RECOGNITION_ON,
    OPER_TRANSMIT_ON_FIXED_PORT,
    OPER_VLANTUNNEL,
    OPER_RESET_STAT,
    OPER_VLAN_TABLE_SET,
}OPER_TYPE_E;

/**
    This structure defines vlan entry.
*/
typedef  struct _csl_switch_vlan_table
{
    struct _csl_switch_vlan_table * next;

    Uint16      vid;            /**<  vlan entry id         */
    Uint16      fid;            /**<   filter data base id  */
    Uint32      mem_ports;      /**<  member port bit map   */
    Uint32      tagged_ports;   /**<   bit map of tagged infomation for member port     */
    Uint32      untagged_ports; /**<   bit map of untagged infomation for member port   */
    Uint32      unmodify_ports; /**<   bit map of unmodified infomation for member port */
    Uint32      u_ports;
    Bool        learn_dis;      /**< disable address learning   */
    Bool        vid_pri_en;     /**<   enable 802.1p            */
    Uint8       vid_pri;        /**<   vlaue of 802.1p when enable vid_pri_en   */
    Bool        valid;

} CSL_SWITCH_VLAN_ENTRY;



/**
@brief This enum defines 802.1q mode type.
*/
typedef enum {
    CSL_1Q_DISABLE = 0, /**<  802.1q mode disbale, port based vlan */
    CSL_1Q_SECURE,      /**<   secure mode, packets which vid isn't in vlan table or source port isn't in vlan port member will be discarded.*/
    CSL_1Q_CHECK,       /**<   check mode, packets which vid isn't in vlan table will be discarded, packets which source port isn't in vlan port member will forward base on vlan port member*/
    CSL_1Q_FALLBACK,    /**<   fallback mode, packets which vid isn't in vlan table will forwarded base on port vlan, packet's which source port isn't in vlan port member will forward base on vlan port member.*/
    CSL_1Q_MODE_BUTT
} csl_pt_1qmode_t;

/**
@brief This enum defines receive packets tagged mode.
*/
typedef enum {
    FAL_INVLAN_ADMIT_ALL = 0,  /**<  receive all packets include tagged and untagged */
    FAL_INVLAN_ADMIT_TAGGED,   /**<  only receive tagged packets*/
    FAL_INVLAN_ADMIT_UNTAGGED, /**<  only receive untagged packets include priority tagged */
    FAL_INVLAN_MODE_BUTT
} fal_pt_invlan_mode_t;

/**
@brief This enum defines vlan propagation mode.
*/
typedef enum {
    FAL_VLAN_PROPAGATION_DISABLE = 0, /**<  vlan propagation disable */
    FAL_VLAN_PROPAGATION_CLONE,       /**<  vlan paopagation mode is clone */
    FAL_VLAN_PROPAGATION_REPLACE,     /**<  vlan paopagation mode is repalce */
    FAL_VLAN_PROPAGATION_MODE_BUTT
}fal_vlan_propagation_mode_t;




/* callbacks types for the switch */
/* base switch */
typedef Int32   (*CSL_SWITCH_START               )(void);
typedef Int32   (*CSL_REG_VAL2OPER_STATUS        )(OPER_TYPE_E oper, Uint16 data);
typedef Uint16  (*CSL_OPER_STATUS2REG_VAL        )(OPER_TYPE_E oper,Int32 operValue,Uint16 dat);
typedef void    (*CSL_SWITCH_GET_VERSION         )(PSW_VER_INFO pSwitchVerInfo);
typedef Int32   (*CSL_PREPARE_READ_STAT_SEQUENCE )(Uint32 port);
typedef Int32   (*CSL_GET_PREPARED_COUNTERS      )(Uint32 statIndex,Uint32* counter);
typedef Uint32  (*CSL_PORT_NUM2REG_ADDR          )(OPER_TYPE_E type, Uint32 portNum);
typedef Uint32  (*CSL_PORT_NUM2PHY_ADDR          )(OPER_TYPE_E type, Uint32 portNum);

/* advanced switch */
typedef Int32   (*CSL_VLN_DEFAULT_SET )(Uint32 portNum,CSL_SWITCH_VLAN_ENTRY * pVlnEntry);
typedef Int32   (*CSL_VLN_UPDATE      )(CSL_SWITCH_VLAN_ENTRY * pVlnEntry);
typedef Int32   (*CSL_VLN_PRIORITY_SET)(Uint32 port,Uint32 priority);
typedef Uint32  (*CSL_VLN_PRIORITY_GET)(Uint32 port);
typedef Int32   (*CSL_VLN_CAPS_SET    )(Uint32 cmd, Uint32 arg);
typedef Int32   (*CSL_DELETE_MAC_ADD  )(char MacAdd[6]);
typedef Int32   (*CSL_SWITCH_LEARNING_DIS)(void);

/* HAL Low Level Access Routines */
typedef Int32   (*CSL_REG_SET)(Uint32 dev_id, Uint32 reg_addr, void *value, Uint32 value_len);
typedef Int32   (*CSL_REG_GET)(Uint32 dev_id, Uint32 reg_addr, void *value, Uint32 value_len);
typedef Int32   (*CSL_PHY_SET)(Uint32 dev_id, Uint32 phy_addr, Uint32 reg, Uint16 value);
typedef Int32   (*CSL_PHY_GET)(Uint32 dev_id, Uint32 phy_addr, Uint32 reg, Uint16 * value);

/* VLAN */
#define VLAN_FUNC_PROTOTYPE_DEF
typedef Int32   (*CSL_VLAN_ENTRY_APPEND)        (Uint32 dev_id, const CSL_SWITCH_VLAN_ENTRY * vlan_entry);
typedef Int32   (*CSL_VLAN_CREATE)              (Uint32 dev_id, Uint32 vlan_id);
typedef Int32   (*CSL_VLAN_NEXT)                (Uint32 dev_id, Uint32 vlan_id, CSL_SWITCH_VLAN_ENTRY * p_vlan);
typedef Int32   (*CSL_VLAN_FIND)                (Uint32 dev_id, Uint32 vlan_id, CSL_SWITCH_VLAN_ENTRY * p_vlan);
typedef Int32   (*CSL_VLAN_MEMBER_UPDATE)       (Uint32 dev_id, Uint32 vlan_id, Uint32 member, Uint32 u_member);
typedef Int32   (*CSL_VLAN_DELETE)              (Uint32 dev_id, Uint32 vlan_id);
typedef Int32   (*CSL_VLAN_FLUSH)               (Uint32 dev_id);
typedef Int32   (*CSL_VLAN_FID_SET)             (Uint32 dev_id, Uint32 vlan_id, Uint32   fid);
typedef Int32   (*CSL_VLAN_FID_GET)             (Uint32 dev_id, Uint32 vlan_id, Uint32 * fid);
typedef Int32   (*CSL_VLAN_MEMBER_ADD)          (Uint32 dev_id, Uint32 vlan_id, Uint32 port_id, VLN_TAG port_info);
typedef Int32   (*CSL_VLAN_MEMBER_DEL)          (Uint32 dev_id, Uint32 vlan_id, Uint32 port_id);
typedef Int32   (*CSL_VLAN_LEARNING_STATE_SET)  (Uint32 dev_id, Uint32 vlan_id, Bool   enable);
typedef Int32   (*CSL_VLAN_LEARNING_STATE_GET)  (Uint32 dev_id, Uint32 vlan_id, Bool * enable);

/* Port Vlan */
typedef Int32   (*CSL_PORT_1QMODE_GET)              (Uint32 dev_id, Uint32 port_id, csl_pt_1qmode_t * pport_1qmode);
typedef Int32   (*CSL_PORT_1QMODE_SET)              (Uint32 dev_id, Uint32 port_id, csl_pt_1qmode_t    port_1qmode);
typedef Int32   (*CSL_PORT_EGVLANMODE_GET)          (Uint32 dev_id, Uint32 port_id, VLN_TAG * pport_egvlanmode);
typedef Int32   (*CSL_PORT_EGVLANMODE_SET)          (Uint32 dev_id, Uint32 port_id, VLN_TAG    port_egvlanmode);
typedef Int32   (*CSL_PORTVLAN_MEMBER_ADD)          (Uint32 dev_id, Uint32 port_id, Uint32 mem_port_id);
typedef Int32   (*CSL_PORTVLAN_MEMBER_DEL)          (Uint32 dev_id, Uint32 port_id, Uint32 mem_port_id);
typedef Int32   (*CSL_PORTVLAN_MEMBER_UPDATE)       (Uint32 dev_id, Uint32 port_id, Uint32 mem_port_map);
typedef Int32   (*CSL_PORTVLAN_MEMBER_GET)          (Uint32 dev_id, Uint32 port_id, Uint32 * mem_port_map);
typedef Int32   (*CSL_PORT_NESTVLAN_SET)            (Uint32 dev_id, Uint32 port_id, Bool enable);
typedef Int32   (*CSL_PORT_NESTVLAN_GET)            (Uint32 dev_id, Uint32 port_id, Bool *enable);
typedef Int32   (*CSL_NESTVLAN_TPID_SET)            (Uint32 dev_id, Uint32 tpid);
typedef Int32   (*CSL_NESTVLAN_TPID_GET)            (Uint32 dev_id, Uint32 *tpid);
typedef Int32   (*CSL_PORT_DEFAULT_VID_SET)         (Uint32 dev_id, Uint32 port_id, Uint32 vid);
typedef Int32   (*CSL_PORT_DEFAULT_VID_GET)         (Uint32 dev_id, Uint32 port_id, Uint32 * vid);
typedef Int32   (*CSL_PORT_FORCE_DEFAULT_VID_SET)   (Uint32 dev_id, Uint32 port_id, Bool enable);
typedef Int32   (*CSL_PORT_FORCE_DEFAULT_VID_GET)   (Uint32 dev_id, Uint32 port_id, Bool * enable);
typedef Int32   (*CSL_PORT_FORCE_PORTVLAN_SET)      (Uint32 dev_id, Uint32 port_id, Bool enable);
typedef Int32   (*CSL_PORT_FORCE_PORTVLAN_GET)      (Uint32 dev_id, Uint32 port_id, Bool * enable);
typedef Int32   (*CSL_PORT_INVLAN_MODE_SET)         (Uint32 dev_id, Uint32 port_id, fal_pt_invlan_mode_t mode);
typedef Int32   (*CSL_PORT_INVLAN_MODE_GET)         (Uint32 dev_id, Uint32 port_id, fal_pt_invlan_mode_t * mode);
typedef Int32   (*CSL_PORT_TLS_SET)                 (Uint32 dev_id, Uint32 port_id, Bool enable);
typedef Int32   (*CSL_PORT_TLS_GET)                 (Uint32 dev_id, Uint32 port_id, Bool * enable);
typedef Int32   (*CSL_PORT_PRI_PROPAGATION_SET)     (Uint32 dev_id, Uint32 port_id, Bool enable);
typedef Int32   (*CSL_PORT_PRI_PROPAGATION_GET)     (Uint32 dev_id, Uint32 port_id, Bool * enable);
typedef Int32   (*CSL_PORT_DEFAULT_SVID_SET)        (Uint32 dev_id, Uint32 port_id, Uint32 vid);
typedef Int32   (*CSL_PORT_DEFAULT_SVID_GET)        (Uint32 dev_id, Uint32 port_id, Uint32 * vid);
typedef Int32   (*CSL_PORT_DEFAULT_CVID_SET)        (Uint32 dev_id, Uint32 port_id, Uint32 vid);
typedef Int32   (*CSL_PORT_DEFAULT_CVID_GET)        (Uint32 dev_id, Uint32 port_id, Uint32 * vid);
#if 0
typedef Int32   (*CSL_PORT_VLAN_PROPAGATION_SET)    (Uint32 dev_id, Uint32 port_id, fal_vlan_propagation_mode_t mode);
typedef Int32   (*CSL_PORT_VLAN_PROPAGATION_GET)    (Uint32 dev_id, Uint32 port_id, fal_vlan_propagation_mode_t * mode);
typedef Int32   (*CSL_PORT_VLAN_TRANS_ADD)          (Uint32 dev_id, Uint32 port_id, CSL_SWITCH_VLAN_ENTRYrans_entry_t *entry);
typedef Int32   (*CSL_PORT_VLAN_TRANS_DEL)          (Uint32 dev_id, Uint32 port_id, CSL_SWITCH_VLAN_ENTRYrans_entry_t *entry);
typedef Int32   (*CSL_PORT_VLAN_TRANS_GET)          (Uint32 dev_id, Uint32 port_id, CSL_SWITCH_VLAN_ENTRYrans_entry_t *entry);
typedef Int32   (*CSL_QINQ_MODE_SET)                (Uint32 dev_id, fal_qinq_mode_t mode);
typedef Int32   (*CSL_QINQ_MODE_GET)                (Uint32 dev_id, fal_qinq_mode_t * mode);
typedef Int32   (*CSL_PORT_QINQ_ROLE_SET)           (Uint32 dev_id, Uint32 port_id, fal_qinq_port_role_t role);
typedef Int32   (*CSL_PORT_QINQ_ROLE_GET)           (Uint32 dev_id, Uint32 port_id, fal_qinq_port_role_t * role);
typedef Int32   (*CSL_PORT_VLAN_TRANS_ITERATE)      (Uint32 dev_id, Uint32 port_id, Uint32 * iterator, CSL_SWITCH_VLAN_ENTRYrans_entry_t * entry);
#endif





typedef struct
{
    CSL_SWITCH_START                cslSwitchStart;
    CSL_REG_VAL2OPER_STATUS         cslRegVal2operStatus;
    CSL_OPER_STATUS2REG_VAL         cslOperStatus2regVal;
    CSL_SWITCH_GET_VERSION          cslSwitchGetVersion;
    CSL_PREPARE_READ_STAT_SEQUENCE  cslPrepareReadStatSequence;
    CSL_GET_PREPARED_COUNTERS       cslGetPreparedCounters;
    CSL_PORT_NUM2REG_ADDR           cslPortNum2RegAddr;
    CSL_PORT_NUM2PHY_ADDR           cslPortNum2PhyAddr;
    CSL_SWITCH_LEARNING_DIS         cslSwitchLearningDisable;
/* advanced switch management */

    CSL_VLN_DEFAULT_SET             cslVlnDefaultSet;
    CSL_VLN_UPDATE                  cslVlnUpdate;
    CSL_VLN_PRIORITY_SET            cslVlnPrioritySet;
    CSL_VLN_PRIORITY_GET            cslVlnPriorityGet;
    CSL_VLN_CAPS_SET                cslVlnCapsSet;
    CSL_DELETE_MAC_ADD              cslDeleteMacAddEntry;

/* HAL Low Level Access Routines */
    CSL_REG_SET                     cslRegSet;
    CSL_REG_GET                     cslRegGet;
    CSL_PHY_SET                     cslPhySet;
    CSL_PHY_GET                     cslPhyGet;

/* VLAN */
    CSL_VLAN_ENTRY_APPEND           vlan_entry_append;
    CSL_VLAN_CREATE                 vlan_creat;
    CSL_VLAN_MEMBER_UPDATE          vlan_member_update;
    CSL_VLAN_DELETE                 vlan_delete;
    CSL_VLAN_FIND                   vlan_find;
    CSL_VLAN_NEXT                   vlan_next;
    CSL_VLAN_FLUSH                  vlan_flush;
    CSL_VLAN_FID_SET                vlan_fid_set;
    CSL_VLAN_FID_GET                vlan_fid_get;
    CSL_VLAN_MEMBER_ADD             vlan_member_add;
    CSL_VLAN_MEMBER_DEL             vlan_member_del;
    CSL_VLAN_LEARNING_STATE_SET     vlan_learning_state_set;
    CSL_VLAN_LEARNING_STATE_GET     vlan_learning_state_get;

/* Port VLAN */
    CSL_PORT_1QMODE_SET             port_1qmode_set;
    CSL_PORT_1QMODE_GET             port_1qmode_get;
    CSL_PORT_EGVLANMODE_GET         port_egvlanmode_get;
    CSL_PORT_EGVLANMODE_SET         port_egvlanmode_set;
    CSL_PORTVLAN_MEMBER_ADD         portvlan_member_add;
    CSL_PORTVLAN_MEMBER_DEL         portvlan_member_del;
    CSL_PORTVLAN_MEMBER_UPDATE      portvlan_member_update;
    CSL_PORTVLAN_MEMBER_GET         portvlan_member_get;
    CSL_PORT_DEFAULT_VID_SET        port_default_vid_set;
    CSL_PORT_DEFAULT_VID_GET        port_default_vid_get;
    CSL_PORT_FORCE_DEFAULT_VID_SET  port_force_default_vid_set;
    CSL_PORT_FORCE_DEFAULT_VID_GET  port_force_default_vid_get;
    CSL_PORT_FORCE_PORTVLAN_SET     port_force_portvlan_set;
    CSL_PORT_FORCE_PORTVLAN_GET     port_force_portvlan_get;
    CSL_NESTVLAN_TPID_SET           port_nestvlan_tpid_set;
    CSL_NESTVLAN_TPID_GET           port_nestvlan_tpid_get;
    CSL_PORT_INVLAN_MODE_SET        port_invlan_mode_set;
    CSL_PORT_INVLAN_MODE_GET        port_invlan_mode_get;
    CSL_PORT_TLS_SET                port_tls_set;
    CSL_PORT_TLS_GET                port_tls_get;
    CSL_PORT_PRI_PROPAGATION_SET    port_pri_propagation_set;
    CSL_PORT_PRI_PROPAGATION_GET    port_pri_propagation_get;
    CSL_PORT_DEFAULT_SVID_SET       port_default_svid_set;
    CSL_PORT_DEFAULT_SVID_GET       port_default_svid_get;
    CSL_PORT_DEFAULT_CVID_SET       port_default_cvid_set;
    CSL_PORT_DEFAULT_CVID_GET       port_default_cvid_get;
#if 0
    CSL_PORT_VLAN_PROPAGATION_SET   port_vlan_propagation_set;
    CSL_PORT_VLAN_PROPAGATION_GET   port_vlan_propagation_get;
    CSL_PORT_VLAN_TRANS_ADD         port_vlan_trans_add;
    CSL_PORT_VLAN_TRANS_DEL         port_vlan_trans_del;
    CSL_PORT_VLAN_TRANS_GET         port_vlan_trans_get;
    CSL_QINQ_MODE_SET               qinq_mode_set;
    CSL_QINQ_MODE_GET               qinq_mode_get;
    CSL_PORT_QINQ_ROLE_SET          port_qinq_role_set;
    CSL_PORT_QINQ_ROLE_GET          port_qinq_role_get;
    CSL_PORT_VLAN_TRANS_ITERATE     port_vlan_trans_iterate;
#endif
}  CSL_SWITCH_CLBKS,
* PCSL_SWITCH_CLBKS;


/*******************************************************************************
* cslConnectSwitch - switch initialization
*
*  This API does the callback structure initialization. Here the type of the switch is considered.
*
* INPUTS:
*       [In/Out] PCSL_SWITCH_DRV_HANDLE swHead
*
*******************************************************************************/

Int32 cslConnectSwitch(PCSL_SWITCH_CLBKS pCslSwitchClbks);

/*******************************************************************************
* cslGetVLANEntry - gets VLAN entry
*
*  Retreives vlnId entry from VLAN table. List manager.
*
* INPUTS:
*       [In] Uint32 vlnId
*
*******************************************************************************/

CSL_SWITCH_VLAN_ENTRY * cslGetVLANEntry(Uint32 vlnId);

/*******************************************************************************
* cslCreateVLANEntry - creates and adds VLAN entry to the VLAN table
*
*  Creates and adds VLAN entry to the logical VLAN table. List manager.
*
* INPUTS:
*
*******************************************************************************/

CSL_SWITCH_VLAN_ENTRY * cslCreateVLANEntry(void);

/*******************************************************************************
* cslDestroyVLANEntry - destroys VLAN entry
*
*  Removes VLAN entry from VLAN table. List manager.
*
* INPUTS:
*
*******************************************************************************/

Int32 cslDestroyVLANEntry(CSL_SWITCH_VLAN_ENTRY * elem);

/*******************************************************************************
*
* cslMarvell6063PrioritySet - Sets the default priority of the packets from this port
*
*   Sets the default priority of the packets from this port.
*
* INPUTS:
*     [In]   PCSL_SWITCH_DRV_HANDLE pDrvHead
*     [In]   Uint32                 portNum
*     [In]   Uint32                 priority
*
*
* RETURNS:
*       Status
*
*
*/

Int32 cslMarvell6063SpecialCapsSet(Uint32 cmd, Uint32 arg);


/*******************************************************************************
* cslGetVLANEntry - gets VLAN entry
*
*  Retreives vlnId entry from VLAN table. List manager.
*
* INPUTS:
*       [In] Uint32 vlnId
*
*******************************************************************************/

CSL_SWITCH_VLAN_ENTRY * cslGetVLANtable(void);

#ifdef CONFIG_ARM_AVALANCHE_ATHEROS_8328N

Int32 cslAtheros8328N_global_init(CSL_SWITCH_CLBKS * p_api);
Int32 cslAtheros8328N_vlan_init(CSL_SWITCH_CLBKS * p_api);
Int32 cslAtheros8328N_portvlan_init(CSL_SWITCH_CLBKS * p_api);

#endif


#endif /* __SWITCH_PRVT_H__ */
