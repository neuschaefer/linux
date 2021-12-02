 /**
 * Copyright 2010 Vestel Elektronik A.S. All rights reserved.
 *
 * @file ice_wifi.h
 * @brief Interface to wifi functionality.
 *
 */

#ifndef CABOT_ICE_ICE_WIFI_H_INCLUDED
#define CABOT_ICE_ICE_WIFI_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"
#include "ice/ice_wifi_types.h"
#include "ice/ice_device_number.h"

/**
 * @brief Interface to WiFi functionality.
 */

/**
 * @brief Initialise the WiFi device.
 *
 * Initialise the required WiFi device.
 *
 * @param params The initialisation parameters indicate the required
 *        WiFi device.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiInit(const ICE_WiFiInitParams* params);

/**
 * @brief Open a connection to the WiFi device.
 *
 * Finds the WiFi interface and initialize the WiFi handle.
 * The handle is used throughout the all ICE_WiFi functions.
 * This function must be called prior to accessing any other ICE_WiFi function.
 * Attempts to open a connection to the WiFi device identified and
 * return a handle for subsequent communication with the device.
 * Dilemma about - Register a callbacks that will notify Eclipse about WiFi
 * status changes
 * This function opens a connection to the WiFi.
 *
 * @param params The open parameters indicate the required WiFi device. Params
 *        also contains pointers to a callbacks callback functions
 * 
 * @return Returns a pointer to the handle to the device on success,
 *         and a NULL pointer on failure.
 */
ICE_WiFiHandle* ICE_WiFiOpen(const ICE_WiFiOpenParams* params);

/**
 * @brief Closes the connection to the WiFi device and releases any resources.
 *
 * Closes a connection to the WiFi.
 * Brings the WiFi device down.
 *
 * @param handle The handle to the WiFi device to be closed.
 *
 * @param params Dilemma about - Pointer to ICE_BTCloseParams structure.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiClose(ICE_WiFiHandle* handle, const ICE_WiFiCloseParams* params);

/**
 * @brief This function is used to set the network access mode, usually it
 *        should use DHCP.
 *
 * @param handle The handle to the WiFi device.
 *
 * @param mode Network access mode, DHCP or Fixed IP.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiSetNetMode(ICE_WiFiHandle* handle, ICE_WiFiNetMode mode);

/**
 * @brief This function is used to set new mac address configuration.
 *
 * @param handle The handle to the WiFi device.
 *
 * @param mac_address  mac address config information.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiSetMacAddress(ICE_WiFiHandle* handle,
                                 ICE_WiFiMacAddressConfig* mac_address);

/**
 * @brief This function is used to get information about the mac address configuration.
 *
 * @param handle The handle to the WiFi device.
 *
 * @param mac_address mac address config information.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetMacAddress(ICE_WiFiHandle* handle,
                                 ICE_WiFiMacAddressConfig* mac_address);

/**
 * @brief This function is used to set dns configuration.
 *
 * @param handle The handle to the WiFi device.
 *
 * @param dns_config dns config information.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiSetDnsConfig(ICE_WiFiHandle* handle,
                                ICE_WiFiDnsConfig* dns_config);

/**
 * @brief This function is used to get information about the dns configuration.
 *
 * @param handle The handle to the WiFi device.
 *
 * @param dns_config dns config information.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetDnsConfig(ICE_WiFiHandle* handle,
                                ICE_WiFiDnsConfig* dns_config);

/**
 * @brief This function is used to search the APs.
 *
 * Scans to find the APs.
 * The scan might take a few seconds to complete.
 *
 * @param handle The handle to the WiFi.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiScanAp(ICE_WiFiHandle* handle);

/**
 * @brief This function is used to gets the scan result.
 *
 * @param handle The handle to the WiFi.
 *
 * @param data Pointer to the ICE_WiFiScanResults structure.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetScanResults(ICE_WiFiHandle* handle, ICE_WiFiScanResults *data);

/**
 * @brief This function is used to get information for the AP currently connected.
 *        Fetch the BSS info and fill the ICE_WiFiScanInfo structure.
 *
 * @param handle The handle to the WiFi.
 *
 * @param scan_info Pointer to the ICE_WiFiScanInfo structure.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetConnectedInfo(ICE_WiFiHandle* handle, ICE_WiFiScanInfo *scan_info);

/**
 * @brief This function is used to associate to an AP using a set of
 *        connections parameters, associate to AP using a set of keys
 *        (WEP,TKIP,AES,WPA2 TKIP,WPA2 AES) or without encryption.
 *
 * @param handle The handle to the WiFi.
 *
 * @param cred Pointer to connection credential.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiConnectAp(ICE_WiFiHandle* handle, ICE_WiFiCredential *cred);

/**
 * @brief This function is used to disassociate from an AP.
 *
 * @param handle The handle to the WiFi.
 *
 * @return Returns   #frost_true on success, #frost_false on failure
 */
frost_bool ICE_WiFiDisconnectAp(ICE_WiFiHandle* handle);

/**
 * @brief This function is used to get the stored credential from the driver.
 *
 * @param handle The handle to the WiFi.
 *
 * @param credential Pointer to ICE_WiFiCredential structure.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetCredential(ICE_WiFiHandle* handle, ICE_WiFiCredential *credential);

/**
 * @brief This function is used to get the link status.
 *
 * Return 1 if STA(Station-Client device) is connected (link is up), 0 if STA is
 * disconnected (link is down).
 *
 * @param handle The handle to the WiFi.
 *
 * @param is_link_up Link is up = 1, link is down = 0.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetLinkStatus(ICE_WiFiHandle* handle, ICE_WiFiLinkStatus *is_link_up);

/**
 * @brief This function is used to get the authentication status.
 *
 * @param handle The handle to the WiFi.
 *
 * @param auth_status Disconnected, authenticated, key exchange, keyed.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetAuthStatus(ICE_WiFiHandle* handle, ICE_WiFiAuthStatus *auth_status);

/**
 * @brief This function is used to get information about wifi connect status,
 *        whether device is connected to AP or not.
 *
 * @param handle The handle to the WiFi.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiIsConnected(ICE_WiFiHandle* handle);

/**
 * @brief This function is used to get the mimo bandwidth cap.
 *
 * 0 - 20 Mhz only
 * 1 - 40 Mhz
 * 2 - 20 Mhz in 2.4G, 40Mhz in 5G
 *
 * MIMO - Multiple Input Multiple Output
 *
 * @param handle The handle to the WiFi.
 *
 * @param val Bandwidth capability value.
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiGetMimoBwCap(ICE_WiFiHandle* handle,frost_uint32 *val);

/**
 * @brief This function is used to search the APs.
 *
 * Scans to find the specific AP.
 * The scan might take a few seconds to complete.
 *
 * @param handle The handle to the WiFi.
 *
 * @param name The name of SSID. 
 *
 * @return Returns #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_WiFiScanApBySSID(ICE_WiFiHandle* handle,char * name);


frost_bool ICE_WiFiRegisterEventHander(ICE_WifiEventHandler const callback_func,
                                       void const * const user_data);

frost_bool ICE_WiFiDeregisterEventHander(ICE_WifiEventHandler const callback_func,
                                       void const * const user_data);

char* ICE_WiFiGetDeviceName();

#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_WIFI_H_INCLUDED */
