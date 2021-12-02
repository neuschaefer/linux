/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Defines ICE Networking API
 *
 * This API only needs to be implemented if you want to support
 * Freesat or Freeview HD Interaction Channel.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 *
 * @note This API is work in progress, and may be significantly different
 *       in the final release version.
 */

#ifndef CABOT_ICE_ICE_NETWORK_H_INCLUDED
#define CABOT_ICE_ICE_NETWORK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Status of network interface.
 */
typedef enum ICE_NetworkInterfaceStatus
{
    /**
     * Networking has not been disabled by the viewer and 
     * the network interface is physically connected and fully configured1.
     */
    ICE_NetworkInterfaceActive,

    /**
     * Networking has not been disabled by the viewer but
     * the network interface is not physically connected 
     * or is not fully configured.
     */
    ICE_NetworkInterfaceInactive,

    /**
     * Networking has been disabled through the action of the viewer.
     */
    ICE_NetworkInterfaceDisabled

} ICE_NetworkInterfaceStatus;

/**
 * @brief Get current status of the network interface.
 *
 * The return value of this function provides dynamic information 
 * to indicate the status of the network connection available 
 * to the receiver.
 * 
 * If the receiver has more than one connection, it shall respond 
 * with the status of the "best connection". A reasonable interpretation 
 * of "best" would be the connection that would retrieve content 
 * in the shortest time.
 * 
 * @return currrent status of the the best network connection available.
 */
ICE_NetworkInterfaceStatus ICE_NetworkGetInterfaceStatus();

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_NETWORK_H_INCLUDED*/
