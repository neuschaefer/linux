/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Internet Protocol settings
 *
 * This file defines the ICE interface for dynamic and static assignment of the
 * following IP parameters: IP address (interface address), subnet mask,
 * broadcast address, default gateway and DNS servers.
 *
 * @note this interface does not support simultaneous configuration of two or
 * more network interfaces.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_IP_H_INCLUDED
#define CABOT_ICE_ICE_IP_H_INCLUDED

#include "frost/frost_basictypes.h"

/**
 * Used to hold binary representations of IP addresses.
 */

typedef frost_uint32 ProtoAddr;

/**
 * The maximum network address length.
 */
#define ICE_INETADDRLEN 16      /* 192.168.100.100 + '\0' */
#define ICE_IP_DNS_SERVER_MAX_NUMBER    2
#define ICE_IP_IF_NAME_SIZE             128
#define ICE_IP_MAC_LENGTH  18 /* 00:E0:4C:43:0B:31 + '\0' */
#define ICE_IP_WIRED_INTERFACE_NAME "eth0"

#ifndef WIN32
char wired_interface_name[10] = "eth0\0" ;
#endif
/****************************************************************/
typedef enum ICE_IPRequest
{
    ICE_IP_Request_None,
    ICE_IP_Request_Static,
    ICE_IP_Request_Dynamic,
    ICE_IP_Request_Down,
    ICE_IP_Request_Up
}ICE_IPRequest;
/**
 * @brief Proxy Server parameters
 */
struct ICE_IPProxyParams
{
    /**
     * Flag to denote whether proxy settings are to be used
     */
    frost_bool m_use_proxy;

    /**
     * IP address or name of proxy server
     */
    const char* m_addr;

    /**
     * Proxy port number
     */
    frost_uint m_port;

    /**
     * Username used to authenticate to the proxy, NULL if using a proxy that does not require authentication
     */
    const char* m_username;

    /**
     * Password used to authenticate to the proxy, NULL if using a proxy that does not require authentication
     */
    const char* m_password;
};

typedef enum ICE_IPInterfaceState
{
    ICE_IP_Interface_State_Unknown,
    ICE_IP_Interface_State_Down,
    ICE_IP_Interface_State_Dynamic,
    ICE_IP_Interface_State_Static

}ICE_IPInterfaceState;

/**
 * Structure that defines the settings required for IP configuration.
 */
typedef struct ICE_IPParams
{
    /**
     * An indication of link status (purely for display).
     *
     * For Ethernet on Unix-like platforms, \c m_link_ok will be true if the
     * interface is up and running.
     */
    frost_bool link_ok;
    ICE_IPInterfaceState state;

    /**
     * The following addresses will be in dotted-decimal notation; for example,
     * 192.168.0.1
     */
    /*@{*/
    char ip_addr[ICE_INETADDRLEN];
    char subnet_mask[ICE_INETADDRLEN];
    char default_gateway[ICE_INETADDRLEN];
    char broadcast_addr[ICE_INETADDRLEN];
    char dns_server[ICE_IP_DNS_SERVER_MAX_NUMBER][ICE_INETADDRLEN];
    char name[ICE_IP_IF_NAME_SIZE];
    char updated_name[ICE_IP_IF_NAME_SIZE];
}ICE_IPParams;

/**
 * @brief Opaque type used to identify a network interface.
 */
typedef struct ICE_IPHandle ICE_IPHandle;

/**
 * Callback function invoked when any IP parameter changes.
 *
 * @param ip_params The new IP configuration.
 */
typedef void (*IPHandler)(void* user_data, const ICE_IPParams* ip_params);

typedef frost_int8 ICE_IPMACAddress[ICE_IP_MAC_LENGTH];
/****************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise shared data. Make ip module ready to use
 *
 * @return frost_true if all resources are created and initialized properly,
 * false otherwise.
 */
frost_bool ICE_IPInit(void);

/**
 * @brief Acquire resources required when changing IP parameters.
 *
 * @param if_name The name of the network device (for example, "eth0")
 * @param user_data The listener of IP event messages.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
ICE_IPHandle* ICE_IPOpen(const char* if_name, void* user_data);

/**
 * @brief Release resources required when changing IP parameters.
 */
ICE_IPHandle* ICE_IPClose(ICE_IPHandle* handle);


/**
 * @brief Register a handler function which will be invoked if the status of
 *        the link changes or any of the monitored IP parameters change.
 *
 * @param  ip_handle Handle of IP
 * @param handler The function to be invoked.
 *
 * @note The handler function will be invoked with the current values of the
 *       IP Parameters when ICE_IPReg() is invoked.
 *
 * @note The ICE implementation must ensure that each array in the ICE_IPParams
 * struct contains either an empty string or a valid decimal-dotted IP address.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_IPReg(ICE_IPHandle* ip_handle, IPHandler handler);

/**
 * @brief Deregister a previously-registered handler function.  The handler
 *        function must not be invoked after ICE_IPDereg() is called.
 *
 * @param  ip_handle Handle of IP
 * @param handler The handler function.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_IPDereg(ICE_IPHandle* ip_handle, IPHandler handler);

/**
 * @brief Specify values for all IP parameters.
 *
 * @param  ip_handle Handle of IP
 * @param params The IP parameter values.
 *
 * @return #frost_true if parameters are valid and the settings are
 *         successfully requested, #frost_false otherwise.
 *
 * @note this function does not need to be synchronous; it may start a thread
 * to complete the request.  Callers will register a handler using ICE_IPReg()
 * if they wish to be notified when the parameters have changed.
 */
frost_bool ICE_IPStatic(ICE_IPHandle* ip_handle, const struct ICE_IPParams* params);

/**
 * @brief Specify that IP parameters should be assigned dynamically (for
 *        example, using DHCP).
 *
 * @return #frost_true if dynamic assignment has been successfully requested,
 *         #frost_false otherwise.
 *
 * @note this function does not need to be synchronous; it may start a thread
 * to complete the request.  Callers will register a handler using ICE_IPReg()
 * if they wish to be notified when the parameters have changed.
 */
frost_bool ICE_IPDynamic(ICE_IPHandle* ip_handle);

/**
 * @brief Enables previously opened and initialised network interface
 * (e.g. using system command: ifconfig "if_name" up)
 *
 * @return #frost_true if enableing interface has been successful,
 *         #frost_false otherwise.
 *
 * @note
 */
frost_bool ICE_IPBringLinkUp(ICE_IPHandle* ip_handle);


/**
 * @brief Disables previously opened and initialised network interface
 * (e.g. using system command: ifconfig "if_name" down)
 *
 * @return #frost_true if disableing interface has been successful,
 *         #frost_false otherwise.
 */
frost_bool ICE_IPBringLinkDown(ICE_IPHandle* ip_handle);

/**
 * @brief Gets inforamtion about  network interface
 *
 * @param  ip_handle Handle of IP
 * @return #frost_true if getting information about interface has been successful,
 *         #frost_false otherwise.
 *
 * @note
 */
frost_bool ICE_IPGetInterfaceInfo(ICE_IPHandle* ip_handle);

/**
 * @brief Sets  status of WiFi Link
 *
 * @param  ip_handle Handle of IP
 * @param  link_up defines that If link is active 
 * 
 */

void ICE_SetWiFiLinkStatus(ICE_IPHandle* ip_handle, frost_bool link_up);

/**
 * @brief Get MAC address of a specific interface from kernel space.
 *
 * @param ip_handle Network interface handle
 *
 * @param buf [out] bufer to fill in mac address
 *
 * @returns If success starting point of the buffer which contains
 *  mac address,0 otherwise.
 *
 * @note Use ICE_IPMACAddress to handle returned information.
 *  Defined in ice_ip.h
 */
frost_bool
ICE_IPGetMACAddress(ICE_IPHandle* const ip_handle,
                    ICE_IPMACAddress buf);

void ICE_SetHttpProxy(const struct ICE_IPProxyParams* params);

frost_bool ICE_IPIsDeviceAlive(ICE_IPHandle* ip_handle);

/**
 * @brief Checks whether any interface has an IP address or not
 *
 * @returns Returns true if a interface has an IP address to use for
 * network connection
 */
frost_bool ICE_IPIsIPSet(void);

/**
 * @brief Checks whether link of an interface is up or down.
 * i.e cable is plugged or not, or wireless connection is established or not.
 *
 * @returns Returns true of link is up, false otherwise.
 */
frost_bool ICE_IPIsLinkUp(void);

/**
 * @brief Checks whether any interface is enabled or not
 * i.e ifconfig up/down status
 *
 * @returns Returns true of interface is enabled, false otherwise
 */
frost_bool ICE_IPIsInterfaceEnabled(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CABOT_ICE_ICE_IP_H_INCLUDED */

