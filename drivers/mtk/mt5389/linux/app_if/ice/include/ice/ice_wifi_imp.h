#ifndef _ICE_WIFI_IMP
#define _ICE_WIFI_IMP

#include <stddef.h>
#include "x_typedef.h"

#include "u_net_wlan.h"
#include "u_net_drv_if.h"
#include "wifi_com.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ice_wifi_types.h"
#include "ice_wifi.h"
#include "c_net_wlan_ctl.h"
#include <pthread.h>

typedef struct tagice_wifi_handle_param
{
    frost_uint				u_device_id;
    frost_bool				b_wpsconnect;
    ICE_WiFiLinkStatus 		e_wifi_link_status;		// wifi link status
    frost_bool				b_connect;			// is connect AP
    ICE_WiFiNetMode			e_wifi_net_mode;
    ICE_WiFiAuthStatus		e_AuthStatus;
    frost_bool				b_wep_associate;
    void *					p_user_data;
    ICE_WifiEventHandler 	p_wifi_event_callback;
}ice_wifi_handle_param;

#define ice_printf(fmt...) \
    do	\
    {\
        printf("[ICE] FILE=%s LINE= %d ", __FILE__, __LINE__); \
        printf(fmt); \
        printf ("\n"); \
    }while(0)

#define WIFI_DEV_NUM 3

extern ice_wifi_handle_param _wifi_one_handle_param;

INT32 wifiNotifyCallback(INT32 i4MsgId, VOID *pvParam);
INT32 WiFiSCanInfo_CrentialConevert(NET_802_11_BSS_INFO_T *p_BssInfo, ICE_WiFiCredential *cred);
INT32 wifiBss_ScaninfoConvert(NET_802_11_BSS_INFO_T *p_BssInfo, ICE_WiFiScanInfo *p_ScanInfo);
int single_level_convert(NET_802_11_BSS_INFO_T *p_BssInfo, ICE_WiFiScanInfo *p_ScanInfo);

INT32 wifiSacnResult_IceConvert(NET_802_11_SCAN_RESULT_T *pScanResult,  ICE_WiFiScanResults *data);
INT32 WiFiCredential_AssociatelConvert(ICE_WiFiCredential *cred, NET_802_11_ASSOCIATE_T *pAssociate);

INT32 ICE_WiFiConnectWps(ICE_WiFiCredential *cred);
INT32 ICE_WifiConnectAuth(ICE_WiFiCredential *cred);
int wpa_wifi0_check(void);



int proc_connect_status(INT32 i4MsgId, VOID *pvParam);
int proc_wps_auth_status(INT32 i4MsgId, VOID *pvParam);
int proc_scan_result(INT32 i4MsgId, VOID *pvParam);
int proc_drive_result(INT32 i4MsgId, VOID *pvParam);


void print_scanresult(NET_802_11_SCAN_RESULT_T *pScanResult);
void print_bss_info(NET_802_11_BSS_INFO_T *p_bss_info_t);
void print_bssid (NET_802_11_BSSID_T  bssid);
void print_associata_t(NET_802_11_ASSOCIATE_T *p_associate_t);


char* get_wpsassoc_mode_str(char* str, int nsize, NET_802_11_WPS_ASSOC_MODE_T e_assoc_mode);
char *get_assoc_case_str(char* str, int nsize, NET_802_11_ASSOC_CASE_T e_assoc_case);
char* get_auth_mode_str(char* str, int nsize, NET_802_11_AUTH_MODE_T e_auth_mode);
char* get_auth_cipher_str(char* str, int nsize, NET_802_11_AUTH_CIPHER_T e_auth_cipher);
char* get_wlan_notify_str(char* str, int nsize, WLAN_NOTIFY_ID_T e_notify_id);


#endif
