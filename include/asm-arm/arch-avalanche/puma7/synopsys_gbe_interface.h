/*
 *
 * synopsys_gbe_interface.h
 * Description:
 * DWC ETHER QOS driver interface
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015 Intel Corporation.
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
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#ifndef __SYNOPSYS_GBE_INTERFACE_H__

#define __SYNOPSYS_GBE_INTERFACE_H__

#ifdef CONFIG_ARM_AVALANCHE_SOC
#include "avalanche_pp_api.h"
#else
#include <linux/avalanche/generic/avalanche_pp_api.h>
#endif

typedef enum{
    DWC_ETH_QOS_INTEL_PRIVATE_CMD_START = 50,
    DWC_ETH_QOS_MUX_SET_CMD = DWC_ETH_QOS_INTEL_PRIVATE_CMD_START,
    DWC_ETH_QOS_MUX_GET_CMD,
    DWC_ETH_QOS_SET_GBE_DBG_PRINT,
    DWC_ETH_QOS_UPDATE_LINK_STATUS_CHANGE,
    DWC_ETH_QOS_SET_LINK_STATUS_CHANGE_DBG_PRINT_MODE,
    DWC_ETH_QOS_INTEL_PRIVATE_CMD_END
}DWC_ETH_QOS_CMDS;

typedef enum
{
    DBG_PRINT_NONE = 0x00,
    DBG_PRINT_RX   = 0x01,
    DBG_PRINT_TX   = 0x10,
    DBG_PRINT_BOTH = DBG_PRINT_RX | DBG_PRINT_TX
}e_print_dbg;

typedef enum
{
    DBG_LINK_STATUS_FLAGS_NONE           = 0x00,
    DBG_LINK_STATUS_FLAGS_CLI_CMD        = 0x01,
    DBG_LINK_STATUS_FLAGS_IGNORE_TIMER   = 0x10,
}e_link_status_flags;

#define PRINT_DBG_PRINT_MODE(print_mode)\
        ((print_mode) == DBG_PRINT_NONE ? "Off" :\
        ((print_mode) == DBG_PRINT_RX   ? "Rx"  :\
        ((print_mode) == DBG_PRINT_TX   ? "Tx"  :\
        ((print_mode) == DBG_PRINT_BOTH ? "Both": "UNKNOWN"))))

#define DWC_ETH_QOS_RWK_FILTER_LENGTH	8

/* common data structure between driver and application for
 * sharing info through ioctl
 * */
struct ifr_data_struct {
    unsigned int flags;
    unsigned int qInx; /* dma channel no to be configured */
    unsigned int cmd;
    unsigned int mdioNum;
    unsigned int param;
    unsigned int context_setup;
    unsigned int connected_speed;
    unsigned int connected_duplex;
    char         devName[VPID_IF_NAME_SIZE];
    unsigned int rwk_filter_values[DWC_ETH_QOS_RWK_FILTER_LENGTH];
    unsigned int rwk_filter_length;
    int command_error;
    int test_done;
    void *ptr;
};

#define NSGMII0_NAME         "nsgmii0"
#define NSGMII1_NAME         "nsgmii1"
#define NRGMII2_NAME         "nrgmii2"
#define NRGMII3_NAME         "nrgmii3"
#define NGMII_TO_ATOM_NAME   "ngmii4"

#endif /* __SYNOPSYS_GBE_INTERFACE_H__ */
