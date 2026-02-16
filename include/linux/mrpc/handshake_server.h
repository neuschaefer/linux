/*
 *
 * handshake_server.h
 * Description:
 * handshake over MRPC
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2016 Intel Corporation.
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

#ifndef __HANDSHAKE_SERVER_H__
#define __HANDSHAKE_SERVER_H__

/*Mask value to define all required sync points*/
#ifdef CONFIG_ARM_AVALANCHE_SOC

/*enum to hold processes ID to be synced*/
enum {
    HANDSHAKE_ID_CPPI = 0,
    HANDSHAKE_ID_CPPI_POST,
    HANDSHAKE_ID_CPPI_HW_MBOX,
    HANDSHAKE_ID_MODPHY,
    HANDSHAKE_ID_LAST,
};
#define SYNC_POINTS (1 << HANDSHAKE_ID_CPPI) | (1 << HANDSHAKE_ID_CPPI_POST) | (1 << HANDSHAKE_ID_CPPI_HW_MBOX) | (1 << HANDSHAKE_ID_MODPHY)

#else /* Atom Handshake Server */

/*enum to hold processes ID to be synced*/
enum {
    HANDSHAKE_ID_PP_INIT = 0,
    HANDSHAKE_ID_LAST,
};

#define SYNC_POINTS (1 << HANDSHAKE_ID_PP_INIT)

#endif

/************************************************/
/**                 API                         */
/************************************************/

/**
 * boot_status_report
 * Function to update current boot status, called by client
 * to report sync point reached
 *
 * @param int client_id - the reporting client ID
 *
 * @return -
 *      (-1) - Wrong ID reported
 *      0 - Otherwise
 */

int boot_status_report(int client_id);

#endif /* __HW_MRPC_H__ */
