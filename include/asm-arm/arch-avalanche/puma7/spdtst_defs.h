/*
  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

#ifndef _SPDTST_DEFS_H_
#define _SPDTST_DEFS_H_

#include <linux/if_ether.h>

#define SPDTST_MAX_SPEED_TESTS_PAIRS_NUM     (16)
#define SPDTST_RC_FAILURE                    (-1)
#define SPDTST_RC_SUCCESS                    (0)

/* Definitions for Speedtest Helper characters device for IOCTL events */
#define SPEEDTEST_IOCTL_MAGIC           's'
#define SPDTST_START_TEST               \
	_IOWR(SPEEDTEST_IOCTL_MAGIC, 1, struct SpdtstInfo)
#define SPDTST_STOP_TEST                _IO(SPEEDTEST_IOCTL_MAGIC, 2)

/*! \var enum SpdtstIpVersion_e
    \brief enum defines IP address version.
*/
enum  SpdtstIpVersion_e {
	SPDTST_IP_VER_IPv4                  = 1,
	SPDTST_IP_VER_IPv6
};

/*! \var enum SpdtstTestAppType_e
    \brief enum defines speedtest application types.
*/
enum SpdtstTestAppType_e {
	SPDTST_TEST_IPERF3_TYPE             = 0,
	/*SPDTST_TEST_TR143_UDP_ECHO_TYPE,*/    /* Not supported */
	/*SPDTST_TEST_TR143_HTTP_DS_TYPE, */    /* Not supported */
	/*SPDTST_TEST_TR143_HTTP_US_TYPE, */    /* Not supported */
	SPDTST_TEST_MAX_APP_TYPE
};

/*! \var enum SpdtstTestTransportType_e
    \brief enum defines speedtest transport types.
*/
enum SpdtstTestTransportType_e {
	SPDTST_TEST_TRANSPORT_TCP_TYPE      = 0,
	SPDTST_TEST_TRANSPORT_UDP_TYPE,
	SPDTST_TEST_MAX_TRANSPORT_TYPE
};

/*! \var enum SpdtstTestFlowDirection_e
    \brief enum defines speedtest flow direction.
*/
enum SpdtstTestFlowDirection_e {
	SPDTST_TEST_DIRECTION_DS      = 0,
	SPDTST_TEST_DIRECTION_US,
	SPDTST_TEST_MAX_DIRECTION
};

/*! \var struct SpdtstIpAddrPair
    \brief Structure defines pair of IP address and port definition.
*/
struct SpdtstIpPortPair {
	uint16_t                portNumber;
	uint16_t                ipAddrVer;
	union {
		/* For IPv4 only the first 4 MSB are set */
		uint32_t        ipV4;
		struct in6_addr ipV6;
	}
	ipAddress;
};

/*! \var struct SpdtstIpPortPairs
    \brief Structure defines pairs of IP addresses and ports definition.
*/
struct SpdtstIpPortPairs {
	/* Number off network speedtest servers IP address and port pairs */
	uint32_t                amountOfPairs;
	/* Table of Speedtest servers IP addresses and port pairs */
	struct SpdtstIpPortPair ipPortPairs[SPDTST_MAX_SPEED_TESTS_PAIRS_NUM];
};

/*! \var struct SpdtstDeviceInfo
    \brief Speedtest network device structure.
*/
struct SpdtstDeviceInfo {
	char                        devName[IFNAMSIZ];
	unsigned char               ethAddress[ETH_ALEN];
	uint32_t                    ipV4;
	struct in6_addr             ipV6;
};

/*! \var struct SpdtstInfo
    \brief Structure contains information about Speedtest info.
*/
struct SpdtstInfo {
	/* DS & US devices info */
	struct SpdtstDeviceInfo     dsDevInfo;
	struct SpdtstDeviceInfo     usDevInfo;
	/* Speedtest servers IP address and port pairs */
	struct SpdtstIpPortPairs    ipPortInfo;
	/* US/DS neighbor device MAC address */
	unsigned char               neighEthAddr[ETH_ALEN];
};
#endif /* _SPDTST_DEFS_H_ */

