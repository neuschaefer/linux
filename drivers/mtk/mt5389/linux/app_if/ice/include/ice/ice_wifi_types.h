 /**
 * Copyright 2010 Vestel Elektronik A.S. All rights reserved.
 *
 * @file ice_wifi_types.h
 * @brief Definition of wifi interface types.
 *
 * @brief This file defines the ICE structures used by the ice_wifi interface
 */

#ifndef CABOT_ICE_ICE_WIFI_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_WIFI_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

/**
 * @brief Definition of wifi interface types.
 *
 */

/**
 * @brief Key length is max 64 bytes.
 * +1 is defined in bwl.
 */
#define ICE_WIFI_MAX_KEY_LEN 64 + 1

/**
 * Length for SSID format string. Max SSID length is 32 bytes,
 * +1 is defined in bwl for EOS
 */
#define ICE_WIFI_MAX_SSID_NAME_LEN 32 + 1

/**
 * Length for MAC Address format string
 */
#define ICE_WIFI_MAC_ADDRESS_LEN 6

/**
 * Length for PASSWORD format string
 */
#define ICE_WIFI_MAX_PASSWD_LEN 4*32+1

/**
 * Length for WSEC PSK format string
 */
#define ICE_WIFI_WSEC_PSK_LEN 64

/**
 * Max length of friendly name
 */
#define ICE_WIFI_FNAME_LEN 48


typedef frost_uint32 ICE_WiFiWepIndex;

typedef frost_ulong ICE_WiFiKeyLength;

typedef frost_int8 ICE_WiFiKey[ICE_WIFI_MAX_KEY_LEN];

typedef frost_int8 ICE_WiFiSSID[ICE_WIFI_MAX_SSID_NAME_LEN];

typedef frost_ulong ICE_WiFiChannel;

typedef frost_int32 ICE_WiFiPhyNoise;

/**
 * @note Exactly 48 bit BSSID space(ethernet address).
 * Octet of ethernet address.
 */
typedef frost_uint8 ICE_WiFiBssid[ICE_WIFI_MAC_ADDRESS_LEN];

typedef frost_int8 *ICE_WiFiWPSNetIf;

typedef frost_ulong ICE_WiFiWPSPin;

typedef frost_int8 *ICE_WiFiWPSKey;

typedef frost_int8 *ICE_WiFiPmkKey[ICE_WIFI_WSEC_PSK_LEN];

/**
 * @brief RSSI (Received signal strength indication)
 */
typedef enum ICE_WiFiRSSI
{
   /**
    * Signal strength none.
    */
   ICE_wifi_signal_none,
   /**
    * Signal strength poor.
    */
    ICE_wifi_signal_poor,
   /**
    * Signal strength fair.
    */
   ICE_wifi_signal_fair,
   /**
    * Signal strength good.
    */
   ICE_wifi_signal_good,
   /**
    * Signal strength excellent.
    */
   ICE_wifi_signal_excellent,
}ICE_WiFiRSSI;

/**
 * @brief Network connection mode (IP address get mode)
 *
 */
typedef enum ICE_WiFiNetMode
{
   /**
    * DHCP
    */
   ICE_wifi_mode_dhcp = 0,

   /**
    * Fixed IP connection, only for advance users
    */
   ICE_wifi_mode_fixed

}ICE_WiFiNetMode;

/**
 * @brief Supported authentication type.
 *
 * Values are used to select the authentication
 * type used to join a network.
 *
 */
typedef enum ICE_WiFiAuthType
{
   /**
    * Open.
    */
   ICE_wifi_auth_open = 0,

   /**
    * Shared.
    */
   ICE_wifi_auth_shared,

   /**
    * Open Shared.
    */
   ICE_wifi_auth_open_shared,

   /**
    * Invalid.
    */
   ICE_wifi_auth_invalid

}ICE_WiFiAuthType;

/**
 * @brief Supported WiFi authentication mode.
 */
typedef enum ICE_WiFiAuthMode
{
   /**
    * Disabled authentication mode.
    */
    ICE_wifi_auth_disabled = 0,

   /**
    * Open authentication mode, no protection.
    */
    ICE_wifi_auth_none = 1,

   /**
    * WPA1 Unspecified key mode.
    */
    ICE_wifi_auth_wpa_unsp = 2,

   /**
    * WPA1 preshare key mode.
    */
    ICE_wifi_auth_wpa_psk = 3,

   /**
    * WPA2 Unspecified key mode.
    */
    ICE_wifi_auth_wpa2_unsp = 4,

   /**
    * WPA2 preshare key mode.
    */
    ICE_wifi_auth_wpa2_psk = 5,

   /**
    * WiFi auth invalid.
    */
    ICE_wifi_auth_wpa_invalid = 6,

   /**
    * WEP authentication mode, only support 40
    * bits(5Bytes ASCII or 10 hexadecimal),
    * 128bits or to mode.
    */
    ICE_wifi_auth_wep = 7

}ICE_WiFiAuthMode;

/**
 * @brief WLAN Security Encryption
 */
typedef enum ICE_WiFiSecTypes
{
   /**
    * No encryption.
    */
   ICE_wifi_sec_none = 0,

   /**
    * WEP encryption.
    */
    ICE_wifi_sec_wep,

   /**
    * TKIP encryption.
    */
    ICE_wifi_sec_tkip,

   /**
    * AES encryption.
    */
    ICE_wifi_sec_aes,

   /**
    * Invalid encryption.
    */
    ICE_wifi_sec_invalid,

}ICE_WiFiSecTypes;

/**
 * @brief Supported IEEE 802.11 modes.
 */
typedef enum ICE_WiFi802_11Modes
{
  /**
   * None mode.
   */
   ICE_WiFi802_11_none = 0x000,

  /**
   * IEEE 802.11 a mode.
   */
   ICE_WiFi802_11_a    = 0x001,

  /**
   * IEEE 802.11 b mod.
   */
   ICE_WiFi802_11_b    = 0x002,

  /**
   * IEEE 802.11 g mod.
   */
   ICE_WiFi802_11_g    = 0x004,

  /**
   * IEEE 802.11 n mod.
   */
   ICE_WiFi802_11_n    = 0x008

} ICE_WiFi802_11Modes;

/**
 * @brief Link status.
 */
typedef enum ICE_WiFiLinkStatus
{
   /**
    * Link is down.
    */
    ICE_WiFiLinkIsDown = 0,
   /**
    * Link is up.
    */
    ICE_WiFiLinkIsUp   = 1,
   /**
    * Unknown link.
    */
   ICE_WiFiLinkUnknown

}ICE_WiFiLinkStatus;

/**
 * @brief Authentication status structure.
 */
typedef enum ICE_WiFiAuthStatus
{
   /**
    * Disconnected,
    */
    ICE_wifi_auth_disconnected,

   /**
    * Authenticated
    */
    ICE_wifi_auth_authenticated,

   /**
    * Key exchanged,
    */
    ICE_wifi_auth_key_exchange,

   /**
    * Keyed
    */
    ICE_wifi_auth_keyed,

    ICE_wifi_auth_status_invalid

}ICE_WiFiAuthStatus;

/**
 * @brief Supported WPS(Wi-Fi Protected Setup) types.
 */
typedef enum ICE_WiFiWPSTypes
{
   /**
     * None WPS.
     */
     ICE_wifi_wps_none,

    /**
     * WPS pin (Personal Information Number ).
     */
     ICE_wifi_wps_pin,

    /**
     * WPS pbc (Push Button Configuration).
     */
     ICE_wifi_wps_pbc

}ICE_WiFiWPSTypes;

/**
 * @brief Supported WiFi operation modes.
 */
typedef enum ICE_WiFiOpModes
{
   /**
    * ad hoc operation mode.
    */
   ICE_wifi_mode_ad_hoc,

   /**
    * Infrastructure operation mode
    */
   ICE_wifi_mode_infra
}ICE_WiFiOpModes;

/**
 * @brief Supported bands.
 */
typedef enum ICE_WiFiBand
{
   /**
    * Auto select
    */
   ICE_WiFiBandAuto = 0,

   /**
    * 5G Band.
    */
   ICE_WiFiBand5G   = 1,

   /**
    * 2G Band.
    */
   ICE_WiFiBand2G   = 2,

   /**
    * Dual Band.
    */
   ICE_WiFiBandDual = ICE_WiFiBand2G | ICE_WiFiBand5G,

   /**
    * Invalid Band.
    */
   ICE_WiFiInvalid

}ICE_WiFiBand;

/**
 * @brief Supported crypto algorithms.
 */
typedef enum ICE_WiFiCryptoAlgo
{
  /**
   * Crypto algorithm off.
   */
   ICE_wifi_algo_off,

  /**
   * Crypto algorithm Wep1.
   */
   ICE_wifi_algo_wep1,

  /**
   * Crypto algorithm Wep128.
   */
   ICE_wifi_algo_wep128,

  /**
   * Crypto algorithm Tkip.
   */
   ICE_wifi_algo_tkip,

  /**
   * Crypto algorithm AesCcm.
   */
   ICE_wifi_algo_aes_ccm,

  /**
   * Crypto algorithm esOcbMsdu.
   */
   ICE_wifi_algo_aes_ocb_msdu,

  /**
   * Crypto algorithm AesOcbMpdu.
   */
   ICE_wifi_algo_aes_ocb_mpdu,

  /**
   * Crypto algorithm Nalg.
   */
   ICE_wifi_algo_nalg,

  /**
   * Crypto algorithm Invalid.
   */
   ICE_wifi_algo_invalid

}ICE_WiFiCryptoAlgo;

/**
 * @brief Event API's.
 */
typedef enum ICE_WiFiEventMessage
{
    /**
     * @brief Scan is completed
     */
    ICE_wifi_scan_complete,

    /**
     * @brief Device Device is alive
     */
    ICE_wifi_link_is_up,

    /**
     * @brief Device is not alive
     */
    ICE_wifi_link_is_down,

    /**
     * @brief Device status is unknown
     */
    ICE_wifi_link_unknown,

    /**
     * @brief Authenticaion failed
     */
    ICE_wifi_auth_fail,

    /**
     * @brief New SSID is set
     */
    ICE_wifi_set_ssid,

    /**
     * Connected to a wireless network
     */
    ICE_wifi_connected,

    /**
     * Disconnected from a wireless network
     */
    ICE_wifi_disconnected,

    /**
     * Roaming indication, STA is kicked from network.
     */
    ICE_wifi_roamed,

    /**
     * Dongle is unplugged.
     */
    ICE_wifi_unplugged,

    /**
     * Dongle is detected.
     */
    ICE_wifi_dongle_detected
}ICE_WiFiEventMessage;

/**
 * @brief Credential Parameters
 */
typedef struct ICE_WiFiCredential
{
   /**
    * 0 == open, 1 == share, both.
    */
   ICE_WiFiAuthType auth_type;

   /**
    * Disable, none, wpa, wpapsk, wpa2, wpa2psk.
    */
    ICE_WiFiAuthMode auth_mode;

   /**
    * WiFi security: none, wep, tkip, aes.
    */
    ICE_WiFiSecTypes sec_type;

   /**
    * Key index, ie, primary key.
    */
    ICE_WiFiWepIndex wep_index;

   /**
    * Used for 16 bytes key.
    */
   ICE_WiFiCryptoAlgo algo_override;

   /**
    * Wep key, or pmk length.
    */
    ICE_WiFiKeyLength key_length;

    /**
     * adhoc, infrastructure
     */
    ICE_WiFiOpModes op_mode;

   /**
    * Key string.
    */
    ICE_WiFiKey key;

   /**
    * SSID of the AP, ssid name, +1 for EOS.
    */
    ICE_WiFiSSID ssid;

   /**
    * BSSID of the AP.
    */
    ICE_WiFiBssid bssid;

   /**
    * WiFi band to use.
    */
   ICE_WiFiBand band;

   /**
    * WiFi channel to use.
    */
   frost_uint32 channel;

   /**
    * WPS type.
    */
   ICE_WiFiWPSTypes wps;

   /**
    * Pin number.
    */
   frost_uint32 pin;

   /**
    * Pointer to net_if.
    */
   ICE_WiFiWPSNetIf net_if;

}ICE_WiFiCredential;

/**
 * @brief Scan Info Config Parameters
 */
typedef struct ICE_WiFiScanInfo
{
   /**
    * Received signal strength status
    */
    ICE_WiFiRSSI rssi;

    /**
     * Channel status
     */
    ICE_WiFiChannel channel;

    /**
     * Physical noise status
     */
    ICE_WiFiPhyNoise phy_noise;

    /**
     * IEEE 802.11 modes
     */
    ICE_WiFi802_11Modes mode_802_11;

    /**
     * BSSIS
     */
    ICE_WiFiBssid bssid;

    /**
     * Credentials status
     */
    ICE_WiFiCredential credentials;

    /**
     * Locked status
     */
    frost_bool is_locked;

    /**
     * Wi-Fi Protected Status(WPS)
     */
    frost_bool is_wps;

}ICE_WiFiScanInfo;

/**
 * @brief Scan results structure
 */
typedef struct ICE_WiFiScanResults
{
   /*
    * @brief Pointer to ICE_WiFiScanInfo structure.
    */
    ICE_WiFiScanInfo *scan_data;

    /*
     * @brief Number of APs found.
     */
    frost_int num_ap;

}ICE_WiFiScanResults;

/**
 * @brief MAC Address Config Parameters
 */
typedef struct ICE_WiFiMacAddressConfig
{
  /**
   *  MAC address
   */
   ICE_WiFiBssid mac_address;

}ICE_WiFiMacAddressConfig;

/**
 * @brief Net Config Parameters
 */
typedef struct ICE_WiFiNetConfig
{
   /**
    * IP Address Config, Hex format, eg.127.0.0.1 (0x0100007f not 0x7f000001)
    */
   frost_uint ip_address;

   /**
    * Subnet Mask Config, Hex format, eg. 255.255.255.0 (0x00ffffff not 0xffffff00)
    */
   frost_uint net_mask;

   /**
    * Default Gateway Config, Hex format, eg. 127.0.0.1 (0x0100007f not 0x7f000001)
    */
   frost_uint default_gateway;

   /**
    * frost_true is configuration is obtained automatically,
    * frost_false if done manually.
    *
    * @note If it is set to true, ip, subnet mask and gateway are obtained
    *      automatically, otherwise user inputs will be used.
    */
   frost_bool    is_auto;

}ICE_WiFiNetConfig;


/**
 * @brief DNS Config Parameters
 */
typedef struct ICE_WiFiDnsConfig
{
   /**
    * the first DNS server address.
    */
   frost_uint dns1;

   /**
    * the second DNS server address.
    */
   frost_uint  dns2;

   /**
    * frost_true if configuration is obtained automatically,
    *            frost_false if done manually.
    *
    * @note If it is set to true, dns info will be obtained automatically,
    *       otherwise user input will be used.
    */
   frost_bool is_auto;

}ICE_WiFiDnsConfig;

/**
 * @brief Secure Pmk Parameters
 */
typedef struct ICE_WiFiSecPmk
{
   /**
    * Octets in key material.
    */
    frost_uint16 key_len;

    /**
     * Key handling qualification.
     */
    frost_uint16 flags;

    /**
     * PMK material.
     */
    ICE_WiFiPmkKey key;

} ICE_WiFiSecPmk;

/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_WiFiHandle ICE_WiFiHandle;

/**
 * @brief WiFi device Initialisation parameters.
 */
typedef struct ICE_WiFiInitParams
{
    /**
     * Identifies the instance of the WiFi device.
     */
    ICE_DeviceNumber device_number;

} ICE_WiFiInitParams;

/**
 * @brief WiFi device open management params.
 */
typedef struct ICE_WiFiMgtOpenParams
{
    /**
     *
     */
    ;

} ICE_WiFiMgtOpenParams;

/**
 * @brief WiFi device close management params.
 */
typedef struct ICE_WiFiMgtCloseParams
{
    /**
     *
     */
    ;

} ICE_WiFiMgtCloseParams;

/**
 * @brief WiFi device open parameters (Device is UP and READY).
 */
typedef struct ICE_WiFiOpenParams
{
   /**
    * Identifies the instance of the WiFi device.
    */
   ICE_DeviceNumber device_number;

   /**
    * Management parameters for WiFi (Whether it is necessary?).
    */
   ICE_WiFiMgtOpenParams *mgt_params;

} ICE_WiFiOpenParams;

/**
 * @brief WiFi device close parameters(Device is DOWN).
 */
typedef struct ICE_WiFiCloseParams
{
   /**
    * Close management parameters for WiFi (Whether it is necessary?).
    */
   ICE_WiFiMgtCloseParams mgt_close;

} ICE_WiFiCloseParams;

/**
 * @brief Callback function to inform Eclipse of the WiFi device status.
 *
 * See the comments in ICE_WiFiStatus when this callback should be
 * called.
 *
 * @param user_data As supplied in the call to ICE_WiFiOpen().
 *
 * @param message The WiFi event message.
 */
typedef void (*ICE_WifiEventHandler)(void* user_data, ICE_WiFiEventMessage message);

#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_WIFI_TYPES_H_INCLUDED */
