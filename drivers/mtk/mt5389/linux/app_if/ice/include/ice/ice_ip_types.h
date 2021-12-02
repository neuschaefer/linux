 /**
 * Copyright 2010 Vestel Elektronik A.S. All rights reserved.
 *
 * @file ice_ip_types.h
 * @brief Definition of ip interface types.
 *
 * @brief This file defines the ICE structures used by the ice_ip interface
 */

#ifndef CABOT_ICE_ICE_IP_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_IP_TYPES_H_INCLUDED

#include "frost/frost_basictypes.h"

#define ICE_INETADDRLEN 16      /* 192.168.100.100 + '\0' */
#define ICE_IP_DNS_SERVER_MAX_NUMBER    2
#define ICE_IP_IF_NAME_SIZE             128
#define ICE_IP_MAC_LENGTH  18 /* 00:E0:4C:43:0B:31 + '\0' */

typedef frost_int8 ICE_IPInetAddress[ICE_INETADDRLEN];
typedef frost_int8 ICE_IPMACAddress[ICE_IP_MAC_LENGTH];
typedef frost_uint32 ProtoAddr;

/**
 * @brief Specific unique IDs for each network interface.
 * Use this to determine which interface is subject
 */
typedef enum ICE_IPDeviceID
{
    ICE_IPDeviceWired,
    ICE_IPDeviceWireless
}ICE_IPDeviceID;

typedef frost_int8 ICE_IPInterfaceName[ICE_IP_IF_NAME_SIZE];

typedef enum ICE_IPDeviceState
{
    ICE_IPDeviceDown,
    ICE_IPDeviceDynamic,
    ICE_IPDeviceStatic,
    ICE_IPDeviceUnknown
}ICE_IPDeviceState;
/**
 * @brief Structure that defines the settings required for IP configuration.
 */
typedef struct ICE_IPParams
{
    ICE_IPDeviceID id;
    ICE_IPInterfaceName name;
    ICE_IPDeviceState state;
    /**
     * The following addresses will be in dotted-decimal notation; for example,
     * 192.168.0.1
     */
    /*@{*/
    ICE_IPInetAddress ip;
    ICE_IPInetAddress subnet_mask[ICE_INETADDRLEN];
    ICE_IPInetAddress default_gateway[ICE_INETADDRLEN];
}ICE_IPParams;

/**
 * @brief Structure that defines the settings required for DNS configuration.
 */
typedef struct ICE_IPDNSParams
{
    ICE_IPDeviceID id;
    ICE_IPInterfaceName name;
    ICE_IPInetAddress dns_servers[ICE_IP_DNS_SERVER_MAX_NUMBER];
}ICE_IPDNSParams;

/**
 * @brief Opaque type used to identify a network interface.
 */
typedef struct ICE_IPHandle ICE_IPHandle;

/**
 * Callback function invoked when any IP parameter changes.
 *
 * @param ip_params The new IP configuration.
 */
typedef void (*ICE_IPStatusCallback)(void* user_data,
        const ICE_IPParams* ip_params);

/**
*@brief Structure to initialisation an IP that includes callback function.
*/

typedef struct ICE_IPInitParams
{
    ICE_IPStatusCallback callback_func;
}ICE_IPInitParams;

/**
*@brief Structure to Open an IP that includes id and name.
*/
typedef struct ICE_IPOpenParams
{
    ICE_IPDeviceID id;
    ICE_IPInetAddress name;
}ICE_IPOpenParams;



#endif /* CABOT_ICE_ICE_IP_TYPES_H_ */
