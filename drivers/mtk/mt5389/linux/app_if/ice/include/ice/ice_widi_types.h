
#ifndef _WIDI_TYPE_H_
#define _WIDI_TYPE_H_

#define MAXPATH 1024

///WIDI event.
typedef enum
{
    /// widi status.                    (param: Widi_state_type*)
    WIDI_EVENT_STATE,
    /// widi retrieve ts data start
    WIDI_EVENT_RETRIEVE_TS_START,
    /// widi retrieve ts data end
    WIDI_EVENT_RETRIEVE_TS_END,
    /// TS data output.                 (param: Widi_ts_type*)
    WIDI_EVENT_TS_DATA,
    /// Fast cursor enable or disable.
    WIDI_EVENT_FAST_CURSOR_ENABLED,
    /// Fast cursor position.           (param: Widi_position*)
    WIDI_EVENT_FAST_CURSOR_POSITION,
    /// Get DHCP. data type:            (param: Widi_DhcpStruct*)
    WIDI_EVENT_GET_DHCP,
    /// Release DHCP                    (param: int*)
    WIDI_EVENT_RELEASE_DHCP,
    /// Get HDCP key                    (param: Widi_HdcpKeyStruct*)
    WIDI_EVENT_GET_HDCP_KEY,
    /// Aes 128 Key Set                 (param: uint8_t*)
    WIDI_EVENT_SET_AES128_KEY,
    /// Aes 128 Riv Set                 (param: uint8_t*)
    WIDI_EVENT_SET_AES128_RIVKEY,
    /// Number of event type.
    WIDI_EVENT_MAX_NUM,
} EN_WIDI_EVENT;

typedef enum widi_states {
    WIDI_STATE_NONE = 0,
    WIDI_STATE_DISCONNECTED = 1,
    WIDI_STATE_DISCOVERY,
    WIDI_STATE_TRIGGERING,
    WIDI_STATE_SECURITY_CHECK,
    WIDI_STATE_PROVISIONING_START,
    WIDI_STATE_AUTHENTICATED,
    WIDI_STATE_P2P_PROVISION_DONE,
    WIDI_STATE_RECONNECTED,
    WIDI_STATE_CONNECTED,
    WIDI_STATE_RECONNECTING,
    WIDI_STATE_DATA_RECEIVED,
} EN_WIDI_STATES;

/// WIDI callback function defination
typedef void (* WIDI_EVENT_CB)(EN_WIDI_EVENT eventID, void * param, void *pUsrParam);


typedef struct
{
    /// Enable or Disable
    int       enable;
    /// Callback function
    WIDI_EVENT_CB     func;
    /// Caller pointer
    void            *pUsrParam;
} WidiEventCB;

typedef struct
{
    /// widi state
    EN_WIDI_STATES state;
    /// data
    const void* data;

} Widi_state_type;

typedef struct
{
    /// ts data pointer
    void*       ts_data;
    /// Callback function
    unsigned long size;

} Widi_ts_type;

typedef struct
{
    /// x position
    unsigned int x;
    /// y position
    unsigned int y;

} Widi_position;


typedef struct {
   unsigned char    cLC128[(128/8)];
   unsigned char    cSHA_1[20];
   unsigned char    cRXID[(40/8)];
   unsigned char    cKPUBRX[(1048/8)];
   unsigned char    cCERT_RESERVED[(16/8)];
   unsigned char    cCERT_SIGN[(3072/8)];
   unsigned char    cKPRIVRX[(2560/8)];
   unsigned char    cSHA_1p[20];
} Widi_HdcpKeyStruct;

typedef struct {
    const char* if_name;         ///< [in]  name of the interface to obtain DHCP lease on
    char* source_ip_address;     ///< [out] buffer to store the DHCP server's IP address in
    int ip_length;               ///< [in]  length of the source_ip_address buffer

} Widi_DhcpStruct;

#endif
