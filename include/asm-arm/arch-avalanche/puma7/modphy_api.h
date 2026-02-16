
/* 

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014-2015 Intel Corporation.

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

  Copyright(c) 2014 Intel Corporation. All rights reserved.

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


/*------------------------------------------------------------------------------
* File Name: modphy_api.h
* Description:
* modphy driver
*------------------------------------------------------------------------------
*/   

#ifndef  MODPHY_API_H
#define  MODPHY_API_H
/**************************************************************************/
/*      INCLUDES:                                                         */
/**************************************************************************/
#include <linux/ioctl.h>
#include "_tistdtypes.h"

/**************************************************************************/
/*      INTERFACE  DEFINES AND STRUCTS                                    */
/**************************************************************************/
#define MODPHY_DEV_NAME  "/dev/modphy_drv"
#define MODPHY_NAME "modphy_drv"

typedef enum modphyController_e
{
    MODPHY_MOCA_CLIENT_ID,
    MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID, 
    MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID, 
    MODPHY_SGMII0_1G_CLIENT_ID,
    MODPHY_SGMII1_1G_CLIENT_ID,
    MODPHY_SGMII0_2_5G_CLIENT_ID,
    MODPHY_SGMII1_2_5G_CLIENT_ID,
    MODPHY_MOCA_50MHZ_CLIENT_ID,
    MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID, 
    MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID, 
    MODPHY_NUM_OF_CLIENTS
}ModphyController_e;

typedef enum modphyLanes_e
{
    MODPHY_LANE_0 = 0,
    MODPHY_LANE_1 = 1, 
    MODPHY_LANE_2 = 2, 
    MODPHY_MAX_NUM_OF_LANES,
    MODPHY_COMMON_LANE
}ModphyLanes_e;

typedef enum modphyKGPowerStates_e
{
    /* Base KG Power States */
    MODPHY_KG_POWER_STATE_0         = 0x00000000,
    MODPHY_KG_POWER_STATE_1         = 0x00000001,
    MODPHY_KG_POWER_STATE_2         = 0x00000002,
    MODPHY_KG_POWER_STATE_3         = 0x00000003,
    MODPHY_KG_POWER_STATE_4         = 0x00000004,
    MODPHY_KG_POWER_STATE_5         = 0x00000005,
    MODPHY_KG_POWER_STATE_6         = 0x00000006,
    MODPHY_KG_POWER_STATE_7         = 0x00000007,

    /* Derived/User Defined states*/
    MODPHY_KG_POWER_STATE_0_RXSTANDBY = 0x00080000,
    MODPHY_KG_POWER_STATE_0_PLL_OFF   = 0x00090000,
    MODPHY_KG_POWER_STATE_OFF,
}ModphyKGPowerStates_e;

typedef enum modphyPowerState_e
{
    MODPHY_POWER_STATE_ACTIVE = MODPHY_KG_POWER_STATE_0,
    MODPHY_POWER_STATE_POWERGATE,
    MODPHY_POWER_STATE_RXSTANDBY,
    MODPHY_POWER_STATE_COMMON_LANE,
    MODPHY_POWER_STATE_RESET,
    MODPHY_POWER_STATE_OFF,
}ModphyPowerState_e;

typedef struct modphyClient_t
{
    ModphyController_e clientId;
    ModphyLanes_e      laneId;
}ModphyClient_t;

typedef struct modphyKgregs_t
{
    unsigned avpsCnt;
    struct
    {
        unsigned addr; /* iosfsb offset */
        unsigned val; /* value to write */
    } * avps;
} modphyKgregs_t;

typedef struct modphyClientKgregs_t
{
    ModphyController_e clientId;
    modphyKgregs_t kgRegs;
} modphyClientKgregs_t;

typedef struct modphyClientPowerGate_t
{
    ModphyClient_t modphyClient;
    ModphyPowerState_e powerStateReq;
} ModphyClientPowerGate_t;

/* This structure holds the data on whether power transitions are allowed by design on various clients */
typedef struct modphyKGPowerGateAccess_t {
    Bool    dlanePowerGateAllowed[MODPHY_MAX_NUM_OF_LANES];
    Bool    clanePowerGateAllowed;
    Bool    rxStandbyAllowed;
} ModphyKGPowerGateAccess_t;

/* This structure holds information on the current status including power states */
/* for the various modphy clients */
typedef struct modphyStatusData_t
{
    ModphyController_e          clientId;
    ModphyLanes_e               maxLanes;
    ModphyKGPowerStates_e       commonLanePowerState;
    ModphyKGPowerStates_e       dataLanePowerState[MODPHY_MAX_NUM_OF_LANES];
    ModphyKGPowerStates_e       powerGateState;
    ModphyKGPowerGateAccess_t   powerGateAccess;
} ModphyStatusData_t;

#ifdef __KERNEL__
/*********************************************************************************************************/
/*! \fn int modphy_init(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function inits the modphy for a specific client.
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_init(ModphyController_e modphyClientId);

/*********************************************************************************************************/
/*! \fn int modphy_configure(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function configure the modphy for a specific client.
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_lane_configure(ModphyLanes_e lane, ModphyController_e modphyClientId);

/*********************************************************************************************************/
/*! \fn int modphy_lane_power_state_transition(ModphyLanes_e lane, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function can cause a specific lane to enter or exit from different low power modes 
 *  \param[in] lane : Lane for the Client ID
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_lane_power_state_transition(ModphyLanes_e lane, ModphyController_e modphyClientId, ModphyPowerState_e powerStateReq);

/*********************************************************************************************************/
/*! \fn int modphy_tx_disable(ModphyLanes_e lane, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function disable TX lanes
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL
 ********************************************************************************************************/
int modphy_tx_disable(ModphyLanes_e lane, ModphyController_e modphyClientId);

/*********************************************************************************************************/
/*! \fn int modphy_reset_k_align(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function reset K alignment in modphy
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL
 ********************************************************************************************************/
int modphy_reset_k_align(ModphyController_e modphyClientId);

/*********************************************************************************************************/
/*! \fn void modphy_reset_client(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function resets ModPhy client. Used for ext.switches WA. Can be scheduled out.
 *  \param[in] modphyClientId : Client ID
 *  \return void
 ********************************************************************************************************/
void modphy_reset_client(ModphyController_e modphyClientId);

#endif

#define MODPHY_DRIVER_DEV_NAME                  MODPHY_DEV_NAME
#define MODPHY_DRIVER_ID                        (0xBE) 
#define MODPHY_INIT                             _IOW(MODPHY_DRIVER_ID, 1, ModphyController_e)
#define MODPHY_CFG                              _IOW(MODPHY_DRIVER_ID, 2, ModphyClient_t)
#define MODPHY_DISABLE_TX                       _IOW(MODPHY_DRIVER_ID, 3, ModphyClient_t)
#define MODPHY_RESET_K_ALIGN                    _IOW(MODPHY_DRIVER_ID, 4, ModphyController_e)
#define MODPHY_RESET_LANE                       _IOW(MODPHY_DRIVER_ID, 5, ModphyController_e)
#define MODPHY_GO2_FAST_RECOVERY                _IOW(MODPHY_DRIVER_ID, 6, ModphyClient_t)
#define MODPHY_GO2_FULL_FUNCTIONAL              _IOW(MODPHY_DRIVER_ID, 7, ModphyClient_t)
#define MODPHY_COMMON_LANE_RESET                _IOW(MODPHY_DRIVER_ID, 8, ModphyController_e)
#define MODPHY_COMMON_LANE_INIT                 _IOW(MODPHY_DRIVER_ID, 9, ModphyController_e)
#define MODPHY_SET_KGREGS                       _IOW(MODPHY_DRIVER_ID, 10, modphyClientKgregs_t)
#define MODPHY_LANE_POWER_STATE_TRANSITION      _IOW(MODPHY_DRIVER_ID, 11, ModphyClientPowerGate_t)

#endif
